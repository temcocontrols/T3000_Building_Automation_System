// CMqttConfig.cpp : Hub MQTT configuration dialog (Network settings -> TCP/IP page)
//
// Transport
//   Primary  : BACnet private transfer, READ_MQTT_CFG (46) / WRITE_MQTT_CFG (146),
//              payload = Str_Mqtt_point (400 bytes, see CM5/ud_str.h) - same path as WireGuard.
//   Fallback : Modbus holding registers 2301-2450 (Read_Multi / Write_Multi_short).
//
// Modbus register map (2301-2450, must match Hub modbus.h MODBUS_MQTT_*).
// Strings are packed 2 chars per register, first char in the HIGH byte.
//   2301        enable          (R/W, 0/1)
//   2302        connected       (RO)
//   2303        port            (R/W)
//   2304-2335   broker[64]      (32 regs)
//   2336-2351   username[32]    (16 regs)
//   2352-2367   password[32]    (16 regs)
//   2368-2383   client_id[32]   (16 regs)
//   2384-2415   pub_topic[64]   (32 regs)
//   2416-2447   sub_topic[64]   (32 regs)
//   2448-2450   spare
//

#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"
#include "global_define.h"
#include "afxdialogex.h"
#include "CMqttConfig.h"

#define MODBUS_MQTT_ENABLE        2301
#define MODBUS_MQTT_CONNECTED     2302
#define MODBUS_MQTT_PORT          2303
#define MODBUS_MQTT_BROKER        2304
#define MODBUS_MQTT_USERNAME      2336
#define MODBUS_MQTT_PASSWORD      2352
#define MODBUS_MQTT_CLIENT_ID     2368
#define MODBUS_MQTT_PUB_TOPIC     2384
#define MODBUS_MQTT_SUB_TOPIC     2416
#define MODBUS_MQTT_REG_END       2447                                   /* last register used */
#define MODBUS_MQTT_REG_COUNT     (MODBUS_MQTT_REG_END - MODBUS_MQTT_ENABLE + 1)  /* 147 */
#define MODBUS_MQTT_CHUNK         50                                     /* regs per Modbus request */

/* UI defaults (also used when the device returns an unconfigured/blank block) */
#define MQTT_DEFAULT_PORT         1883
#define MQTT_DEFAULT_BROKER       "broker.hivemq.com"
#define MQTT_DEFAULT_PUB_TOPIC    "temco/test/hub/pub"
#define MQTT_DEFAULT_SUB_TOPIC    "temco/test/hub/sub"
#define MQTT_DEFAULT_CLIENT_ID    "temco_hub"

/* ---- helpers ---------------------------------------------------------- */

static CString MqttAnsiField(const char* p, size_t n)
{
	char buf[128] = { 0 };
	if (n > sizeof(buf) - 1)
		n = sizeof(buf) - 1;
	memcpy(buf, p, n);
	buf[n] = '\0';
	return CString(buf);
}

static void MqttSetField(char* dst, size_t dst_size, const CString& s)
{
	memset(dst, 0, dst_size);
	CT2CA sz(s);
	strncpy_s(dst, dst_size, sz, _TRUNCATE);
}

static void MqttRegsToString(const unsigned short* regs, int nregs, char* dst, size_t dst_size)
{
	memset(dst, 0, dst_size);
	for (int i = 0; i < nregs && (size_t)(i * 2 + 1) < dst_size; i++)
	{
		dst[i * 2] = (char)((regs[i] >> 8) & 0xff);
		dst[i * 2 + 1] = (char)(regs[i] & 0xff);
	}
	dst[dst_size - 1] = '\0';
}

static void MqttStringToRegs(const char* src, size_t src_size, unsigned short* regs, int nregs)
{
	for (int i = 0; i < nregs; i++)
	{
		unsigned char hi = ((size_t)(i * 2) < src_size) ? (unsigned char)src[i * 2] : 0;
		unsigned char lo = ((size_t)(i * 2 + 1) < src_size) ? (unsigned char)src[i * 2 + 1] : 0;
		regs[i] = (unsigned short)((hi << 8) | lo);
	}
}

static BOOL MqttReadRegs(unsigned short* regs, int start, int count)
{
	int done = 0;
	while (done < count)
	{
		int n = count - done;
		if (n > MODBUS_MQTT_CHUNK)
			n = MODBUS_MQTT_CHUNK;
		if (Read_Multi((unsigned char)g_tstat_id, regs + done, (unsigned short)(start + done), n, 3) < 0)
			return FALSE;
		done += n;
	}
	return TRUE;
}

static BOOL MqttWriteRegs(unsigned short* regs, int start, int count)
{
	int done = 0;
	while (done < count)
	{
		int n = count - done;
		if (n > MODBUS_MQTT_CHUNK)
			n = MODBUS_MQTT_CHUNK;
		if (Write_Multi_short((unsigned char)g_tstat_id, regs + done, (unsigned short)(start + done), n, 3) <= 0)
			return FALSE;
		done += n;
	}
	return TRUE;
}

/* ---- dialog ----------------------------------------------------------- */

IMPLEMENT_DYNAMIC(CMqttConfig, CDialogEx)

CMqttConfig::CMqttConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MQTT_CONFIG, pParent)
{
}

CMqttConfig::~CMqttConfig()
{
}

void CMqttConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMqttConfig, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MQTT, &CMqttConfig::OnBnClickedCheckEnableMqtt)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_MQTT_CFG, &CMqttConfig::OnBnClickedButtonWriteMqttCfg)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_MQTT_CFG, &CMqttConfig::OnBnClickedButtonCancelMqttCfg)
END_MESSAGE_MAP()

void CMqttConfig::SetDefaults(Str_Mqtt_point& pt)
{
	memset(&pt, 0, sizeof(pt));
	pt.reg.enable = 0;
	pt.reg.port = MQTT_DEFAULT_PORT;
	strcpy_s(pt.reg.broker, sizeof(pt.reg.broker), MQTT_DEFAULT_BROKER);
	strcpy_s(pt.reg.pub_topic, sizeof(pt.reg.pub_topic), MQTT_DEFAULT_PUB_TOPIC);
	strcpy_s(pt.reg.sub_topic, sizeof(pt.reg.sub_topic), MQTT_DEFAULT_SUB_TOPIC);
	strcpy_s(pt.reg.client_id, sizeof(pt.reg.client_id), MQTT_DEFAULT_CLIENT_ID);
}

/* A never-configured Hub returns zeros (or 0xFF from erased flash). */
BOOL CMqttConfig::IsBlank(const Str_Mqtt_point& pt)
{
	return (pt.reg.port == 0 || pt.reg.port == 0xFFFF) &&
		(pt.reg.broker[0] == '\0' || (unsigned char)pt.reg.broker[0] == 0xFF);
}

BOOL CMqttConfig::Load()
{
	if (LoadViaBacnet())
		return TRUE;
	return LoadViaModbus();
}

BOOL CMqttConfig::LoadViaBacnet()
{
	return GetPrivateData_Blocking(g_bac_instance, READ_MQTT_CFG, 0, 0,
		sizeof(Str_Mqtt_point)) >= 0;
}

BOOL CMqttConfig::LoadViaModbus()
{
	unsigned short regs[MODBUS_MQTT_REG_COUNT] = { 0 };
	if (!MqttReadRegs(regs, MODBUS_MQTT_ENABLE, MODBUS_MQTT_REG_COUNT))
		return FALSE;

	#define R(addr) (regs[(addr) - MODBUS_MQTT_ENABLE])
	memset(&Device_Mqtt_Point, 0, sizeof(Device_Mqtt_Point));
	Device_Mqtt_Point.reg.enable = (unsigned char)(R(MODBUS_MQTT_ENABLE) & 0xff);
	Device_Mqtt_Point.reg.connected = (unsigned char)(R(MODBUS_MQTT_CONNECTED) & 0xff);
	Device_Mqtt_Point.reg.port = R(MODBUS_MQTT_PORT);
	MqttRegsToString(&R(MODBUS_MQTT_BROKER), 32, Device_Mqtt_Point.reg.broker, sizeof(Device_Mqtt_Point.reg.broker));
	MqttRegsToString(&R(MODBUS_MQTT_USERNAME), 16, Device_Mqtt_Point.reg.username, sizeof(Device_Mqtt_Point.reg.username));
	MqttRegsToString(&R(MODBUS_MQTT_PASSWORD), 16, Device_Mqtt_Point.reg.password, sizeof(Device_Mqtt_Point.reg.password));
	MqttRegsToString(&R(MODBUS_MQTT_CLIENT_ID), 16, Device_Mqtt_Point.reg.client_id, sizeof(Device_Mqtt_Point.reg.client_id));
	MqttRegsToString(&R(MODBUS_MQTT_PUB_TOPIC), 32, Device_Mqtt_Point.reg.pub_topic, sizeof(Device_Mqtt_Point.reg.pub_topic));
	MqttRegsToString(&R(MODBUS_MQTT_SUB_TOPIC), 32, Device_Mqtt_Point.reg.sub_topic, sizeof(Device_Mqtt_Point.reg.sub_topic));
	#undef R
	return TRUE;
}

BOOL CMqttConfig::WriteViaBacnet()
{
	return Write_Private_Data_Blocking(WRITE_MQTT_CFG, 0, 0) > 0;
}

BOOL CMqttConfig::WriteViaModbus()
{
	unsigned short regs[MODBUS_MQTT_REG_COUNT] = { 0 };

	#define R(addr) (regs[(addr) - MODBUS_MQTT_ENABLE])
	R(MODBUS_MQTT_ENABLE) = Device_Mqtt_Point.reg.enable;
	R(MODBUS_MQTT_PORT) = Device_Mqtt_Point.reg.port;
	MqttStringToRegs(Device_Mqtt_Point.reg.broker, sizeof(Device_Mqtt_Point.reg.broker), &R(MODBUS_MQTT_BROKER), 32);
	MqttStringToRegs(Device_Mqtt_Point.reg.username, sizeof(Device_Mqtt_Point.reg.username), &R(MODBUS_MQTT_USERNAME), 16);
	MqttStringToRegs(Device_Mqtt_Point.reg.password, sizeof(Device_Mqtt_Point.reg.password), &R(MODBUS_MQTT_PASSWORD), 16);
	MqttStringToRegs(Device_Mqtt_Point.reg.client_id, sizeof(Device_Mqtt_Point.reg.client_id), &R(MODBUS_MQTT_CLIENT_ID), 16);
	MqttStringToRegs(Device_Mqtt_Point.reg.pub_topic, sizeof(Device_Mqtt_Point.reg.pub_topic), &R(MODBUS_MQTT_PUB_TOPIC), 32);
	MqttStringToRegs(Device_Mqtt_Point.reg.sub_topic, sizeof(Device_Mqtt_Point.reg.sub_topic), &R(MODBUS_MQTT_SUB_TOPIC), 32);

	/* 'connected' (2302) is read-only: write enable (2301) and then port..sub_topic (2303-2447). */
	if (!MqttWriteRegs(&R(MODBUS_MQTT_ENABLE), MODBUS_MQTT_ENABLE, 1))
		return FALSE;
	BOOL ok = MqttWriteRegs(&R(MODBUS_MQTT_PORT), MODBUS_MQTT_PORT, MODBUS_MQTT_REG_END - MODBUS_MQTT_PORT + 1);
	#undef R
	return ok;
}

BOOL CMqttConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/* Field limits = struct sizes - 1 (NUL terminator). */
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_BROKER))->SetLimitText(63);
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_USERNAME))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_PASSWORD))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_CLIENT_ID))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_PUB_TOPIC))->SetLimitText(63);
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_SUB_TOPIC))->SetLimitText(63);
	((CEdit*)GetDlgItem(IDC_EDIT_MQTT_PORT))->SetLimitText(5);

	/* Device_Mqtt_Point was filled by the caller (READ_MQTT_CFG) before DoModal. */
	LoadToUi();
	UpdateUIState();
	return TRUE;
}

void CMqttConfig::LoadToUi()
{
	if (IsBlank(Device_Mqtt_Point))
	{
		SetDefaults(Device_Mqtt_Point);
	}

	CheckDlgButton(IDC_CHECK_ENABLE_MQTT,
		Device_Mqtt_Point.reg.enable ? BST_CHECKED : BST_UNCHECKED);
	SetDlgItemInt(IDC_EDIT_MQTT_PORT, Device_Mqtt_Point.reg.port);

	SetDlgItemText(IDC_EDIT_MQTT_BROKER, MqttAnsiField(Device_Mqtt_Point.reg.broker, sizeof(Device_Mqtt_Point.reg.broker)));
	SetDlgItemText(IDC_EDIT_MQTT_USERNAME, MqttAnsiField(Device_Mqtt_Point.reg.username, sizeof(Device_Mqtt_Point.reg.username)));
	SetDlgItemText(IDC_EDIT_MQTT_PASSWORD, MqttAnsiField(Device_Mqtt_Point.reg.password, sizeof(Device_Mqtt_Point.reg.password)));
	SetDlgItemText(IDC_EDIT_MQTT_CLIENT_ID, MqttAnsiField(Device_Mqtt_Point.reg.client_id, sizeof(Device_Mqtt_Point.reg.client_id)));
	SetDlgItemText(IDC_EDIT_MQTT_PUB_TOPIC, MqttAnsiField(Device_Mqtt_Point.reg.pub_topic, sizeof(Device_Mqtt_Point.reg.pub_topic)));
	SetDlgItemText(IDC_EDIT_MQTT_SUB_TOPIC, MqttAnsiField(Device_Mqtt_Point.reg.sub_topic, sizeof(Device_Mqtt_Point.reg.sub_topic)));

	SetDlgItemText(IDC_STATIC_MQTT_STATUS,
		Device_Mqtt_Point.reg.connected ? _T("Connected") : _T("Not connected"));
}

void CMqttConfig::SaveFromUi()
{
	CString str;

	Device_Mqtt_Point.reg.enable =
		(IsDlgButtonChecked(IDC_CHECK_ENABLE_MQTT) == BST_CHECKED) ? 1 : 0;

	BOOL bTrans = FALSE;
	UINT nPort = GetDlgItemInt(IDC_EDIT_MQTT_PORT, &bTrans);
	if (bTrans && nPort > 0 && nPort <= 65535)
		Device_Mqtt_Point.reg.port = (unsigned short)nPort;

	GetDlgItemText(IDC_EDIT_MQTT_BROKER, str);
	str.Trim();
	MqttSetField(Device_Mqtt_Point.reg.broker, sizeof(Device_Mqtt_Point.reg.broker), str);
	GetDlgItemText(IDC_EDIT_MQTT_USERNAME, str);
	MqttSetField(Device_Mqtt_Point.reg.username, sizeof(Device_Mqtt_Point.reg.username), str);
	GetDlgItemText(IDC_EDIT_MQTT_PASSWORD, str);
	MqttSetField(Device_Mqtt_Point.reg.password, sizeof(Device_Mqtt_Point.reg.password), str);
	GetDlgItemText(IDC_EDIT_MQTT_CLIENT_ID, str);
	str.Trim();
	MqttSetField(Device_Mqtt_Point.reg.client_id, sizeof(Device_Mqtt_Point.reg.client_id), str);
	GetDlgItemText(IDC_EDIT_MQTT_PUB_TOPIC, str);
	str.Trim();
	MqttSetField(Device_Mqtt_Point.reg.pub_topic, sizeof(Device_Mqtt_Point.reg.pub_topic), str);
	GetDlgItemText(IDC_EDIT_MQTT_SUB_TOPIC, str);
	str.Trim();
	MqttSetField(Device_Mqtt_Point.reg.sub_topic, sizeof(Device_Mqtt_Point.reg.sub_topic), str);
}

void CMqttConfig::UpdateUIState()
{
	BOOL bEnabled = (IsDlgButtonChecked(IDC_CHECK_ENABLE_MQTT) == BST_CHECKED);

	GetDlgItem(IDC_EDIT_MQTT_BROKER)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_MQTT_PORT)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_MQTT_USERNAME)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_MQTT_PASSWORD)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_MQTT_CLIENT_ID)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_MQTT_PUB_TOPIC)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_MQTT_SUB_TOPIC)->EnableWindow(bEnabled);

	/* Apply must stay enabled so the user can write enable=0 to the device. */
	GetDlgItem(IDC_BUTTON_WRITE_MQTT_CFG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CANCEL_MQTT_CFG)->EnableWindow(TRUE);
}

BOOL CMqttConfig::ValidateInput()
{
	if (IsDlgButtonChecked(IDC_CHECK_ENABLE_MQTT) != BST_CHECKED)
		return TRUE;

	CString str;
	GetDlgItemText(IDC_EDIT_MQTT_BROKER, str);
	str.Trim();
	if (str.IsEmpty())
	{
		MessageBox(_T("Please enter the MQTT broker address."), _T("MQTT"), MB_ICONERROR);
		return FALSE;
	}

	BOOL bTrans = FALSE;
	UINT nPort = GetDlgItemInt(IDC_EDIT_MQTT_PORT, &bTrans);
	if (!bTrans || nPort == 0 || nPort > 65535)
	{
		MessageBox(_T("Port must be between 1 and 65535."), _T("MQTT"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

void CMqttConfig::OnBnClickedCheckEnableMqtt()
{
	UpdateUIState();
}

void CMqttConfig::OnBnClickedButtonWriteMqttCfg()
{
	if (!ValidateInput())
		return;

	SaveFromUi();

	if (!WriteViaBacnet() && !WriteViaModbus())
	{
		MessageBox(_T("Failed to apply MQTT configuration."), _T("MQTT"), MB_ICONERROR);
		return;
	}

	MessageBox(_T("MQTT configuration applied successfully."), _T("MQTT"), MB_ICONINFORMATION);
}

void CMqttConfig::OnBnClickedButtonCancelMqttCfg()
{
	OnCancel();
}
