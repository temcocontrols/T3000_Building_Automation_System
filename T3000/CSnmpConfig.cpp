// CSnmpConfig.cpp : Hub SNMP agent configuration dialog (Network settings -> TCP/IP page)
//
// Transport
//   Primary  : BACnet private transfer, READ_SNMP_CFG (47) / WRITE_SNMP_CFG (147),
//              payload = Str_Snmp_point (400 bytes, see CM5/ud_str.h) - same path as WireGuard.
//   Fallback : Modbus holding registers 2451-2550 (Read_Multi / Write_Multi_short).
//
// Modbus register map (2451-2550, must match Hub modbus.h MODBUS_SNMP_*).
// Strings are packed 2 chars per register, first char in the HIGH byte.
// The trap destination IP uses one register per octet (same as the GSM IP).
//   2451        enable           (R/W, 0/1)
//   2452        running          (RO)
//   2453        port             (R/W)
//   2454-2469   ro_community[32] (16 regs)
//   2470-2485   rw_community[32] (16 regs)
//   2486-2489   trap_dest_ip[4]  (4 regs, 1 octet each)
//   2490        trap_dest_port
//   2491-2506   sys_name[32]     (16 regs)
//   2507-2522   sys_location[32] (16 regs)
//   2523-2538   sys_contact[32]  (16 regs)
//   2539-2550   spare
//

#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"
#include "global_define.h"
#include "afxdialogex.h"
#include "CSnmpConfig.h"

#define MODBUS_SNMP_ENABLE        2451
#define MODBUS_SNMP_RUNNING       2452
#define MODBUS_SNMP_PORT          2453
#define MODBUS_SNMP_RO_COMMUNITY  2454
#define MODBUS_SNMP_RW_COMMUNITY  2470
#define MODBUS_SNMP_TRAP_IP       2486
#define MODBUS_SNMP_TRAP_PORT     2490
#define MODBUS_SNMP_SYS_NAME      2491
#define MODBUS_SNMP_SYS_LOCATION  2507
#define MODBUS_SNMP_SYS_CONTACT   2523
#define MODBUS_SNMP_REG_END       2538                                   /* last register used */
#define MODBUS_SNMP_REG_COUNT     (MODBUS_SNMP_REG_END - MODBUS_SNMP_ENABLE + 1)  /* 88 */
#define MODBUS_SNMP_CHUNK         50                                     /* regs per Modbus request */

/* UI defaults (also used when the device returns an unconfigured/blank block) */
#define SNMP_DEFAULT_PORT         161
#define SNMP_DEFAULT_TRAP_PORT    162
#define SNMP_DEFAULT_RO           "public"
#define SNMP_DEFAULT_RW           "private"
#define SNMP_DEFAULT_SYS_NAME     "Temco Hub"
#define SNMP_DEFAULT_SYS_LOCATION "Temco Controls"
#define SNMP_DEFAULT_SYS_CONTACT  "support@temcocontrols.com"

/* ---- helpers ---------------------------------------------------------- */

static CString SnmpAnsiField(const char* p, size_t n)
{
	char buf[128] = { 0 };
	if (n > sizeof(buf) - 1)
		n = sizeof(buf) - 1;
	memcpy(buf, p, n);
	buf[n] = '\0';
	return CString(buf);
}

static void SnmpSetField(char* dst, size_t dst_size, const CString& s)
{
	memset(dst, 0, dst_size);
	CT2CA sz(s);
	strncpy_s(dst, dst_size, sz, _TRUNCATE);
}

static void SnmpRegsToString(const unsigned short* regs, int nregs, char* dst, size_t dst_size)
{
	memset(dst, 0, dst_size);
	for (int i = 0; i < nregs && (size_t)(i * 2 + 1) < dst_size; i++)
	{
		dst[i * 2] = (char)((regs[i] >> 8) & 0xff);
		dst[i * 2 + 1] = (char)(regs[i] & 0xff);
	}
	dst[dst_size - 1] = '\0';
}

static void SnmpStringToRegs(const char* src, size_t src_size, unsigned short* regs, int nregs)
{
	for (int i = 0; i < nregs; i++)
	{
		unsigned char hi = ((size_t)(i * 2) < src_size) ? (unsigned char)src[i * 2] : 0;
		unsigned char lo = ((size_t)(i * 2 + 1) < src_size) ? (unsigned char)src[i * 2 + 1] : 0;
		regs[i] = (unsigned short)((hi << 8) | lo);
	}
}

static BOOL SnmpReadRegs(unsigned short* regs, int start, int count)
{
	int done = 0;
	while (done < count)
	{
		int n = count - done;
		if (n > MODBUS_SNMP_CHUNK)
			n = MODBUS_SNMP_CHUNK;
		if (Read_Multi((unsigned char)g_tstat_id, regs + done, (unsigned short)(start + done), n, 3) < 0)
			return FALSE;
		done += n;
	}
	return TRUE;
}

static BOOL SnmpWriteRegs(unsigned short* regs, int start, int count)
{
	int done = 0;
	while (done < count)
	{
		int n = count - done;
		if (n > MODBUS_SNMP_CHUNK)
			n = MODBUS_SNMP_CHUNK;
		if (Write_Multi_short((unsigned char)g_tstat_id, regs + done, (unsigned short)(start + done), n, 3) <= 0)
			return FALSE;
		done += n;
	}
	return TRUE;
}

/* ---- dialog ----------------------------------------------------------- */

IMPLEMENT_DYNAMIC(CSnmpConfig, CDialogEx)

CSnmpConfig::CSnmpConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SNMP_CONFIG, pParent)
{
}

CSnmpConfig::~CSnmpConfig()
{
}

void CSnmpConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnmpConfig, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SNMP, &CSnmpConfig::OnBnClickedCheckEnableSnmp)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_SNMP_CFG, &CSnmpConfig::OnBnClickedButtonWriteSnmpCfg)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_SNMP_CFG, &CSnmpConfig::OnBnClickedButtonCancelSnmpCfg)
END_MESSAGE_MAP()

void CSnmpConfig::SetDefaults(Str_Snmp_point& pt)
{
	memset(&pt, 0, sizeof(pt));
	pt.reg.enable = 0;
	pt.reg.port = SNMP_DEFAULT_PORT;
	strcpy_s(pt.reg.ro_community, sizeof(pt.reg.ro_community), SNMP_DEFAULT_RO);
	strcpy_s(pt.reg.rw_community, sizeof(pt.reg.rw_community), SNMP_DEFAULT_RW);
	pt.reg.trap_dest_port = SNMP_DEFAULT_TRAP_PORT;
	strcpy_s(pt.reg.sys_name, sizeof(pt.reg.sys_name), SNMP_DEFAULT_SYS_NAME);
	strcpy_s(pt.reg.sys_location, sizeof(pt.reg.sys_location), SNMP_DEFAULT_SYS_LOCATION);
	strcpy_s(pt.reg.sys_contact, sizeof(pt.reg.sys_contact), SNMP_DEFAULT_SYS_CONTACT);
}

/* A never-configured Hub returns zeros (or 0xFF from erased flash). */
BOOL CSnmpConfig::IsBlank(const Str_Snmp_point& pt)
{
	return (pt.reg.port == 0 || pt.reg.port == 0xFFFF) &&
		(pt.reg.ro_community[0] == '\0' || (unsigned char)pt.reg.ro_community[0] == 0xFF);
}

BOOL CSnmpConfig::Load()
{
	if (LoadViaBacnet())
		return TRUE;
	return LoadViaModbus();
}

BOOL CSnmpConfig::LoadViaBacnet()
{
	return GetPrivateData_Blocking(g_bac_instance, READ_SNMP_CFG, 0, 0,
		sizeof(Str_Snmp_point)) >= 0;
}

BOOL CSnmpConfig::LoadViaModbus()
{
	unsigned short regs[MODBUS_SNMP_REG_COUNT] = { 0 };
	if (!SnmpReadRegs(regs, MODBUS_SNMP_ENABLE, MODBUS_SNMP_REG_COUNT))
		return FALSE;

	#define R(addr) (regs[(addr) - MODBUS_SNMP_ENABLE])
	memset(&Device_Snmp_Point, 0, sizeof(Device_Snmp_Point));
	Device_Snmp_Point.reg.enable = (unsigned char)(R(MODBUS_SNMP_ENABLE) & 0xff);
	Device_Snmp_Point.reg.running = (unsigned char)(R(MODBUS_SNMP_RUNNING) & 0xff);
	Device_Snmp_Point.reg.port = R(MODBUS_SNMP_PORT);
	SnmpRegsToString(&R(MODBUS_SNMP_RO_COMMUNITY), 16, Device_Snmp_Point.reg.ro_community, sizeof(Device_Snmp_Point.reg.ro_community));
	SnmpRegsToString(&R(MODBUS_SNMP_RW_COMMUNITY), 16, Device_Snmp_Point.reg.rw_community, sizeof(Device_Snmp_Point.reg.rw_community));
	for (int i = 0; i < 4; i++)
		Device_Snmp_Point.reg.trap_dest_ip[i] = (unsigned char)(R(MODBUS_SNMP_TRAP_IP + i) & 0xff);
	Device_Snmp_Point.reg.trap_dest_port = R(MODBUS_SNMP_TRAP_PORT);
	SnmpRegsToString(&R(MODBUS_SNMP_SYS_NAME), 16, Device_Snmp_Point.reg.sys_name, sizeof(Device_Snmp_Point.reg.sys_name));
	SnmpRegsToString(&R(MODBUS_SNMP_SYS_LOCATION), 16, Device_Snmp_Point.reg.sys_location, sizeof(Device_Snmp_Point.reg.sys_location));
	SnmpRegsToString(&R(MODBUS_SNMP_SYS_CONTACT), 16, Device_Snmp_Point.reg.sys_contact, sizeof(Device_Snmp_Point.reg.sys_contact));
	#undef R
	return TRUE;
}

BOOL CSnmpConfig::WriteViaBacnet()
{
	return Write_Private_Data_Blocking(WRITE_SNMP_CFG, 0, 0) > 0;
}

BOOL CSnmpConfig::WriteViaModbus()
{
	unsigned short regs[MODBUS_SNMP_REG_COUNT] = { 0 };

	#define R(addr) (regs[(addr) - MODBUS_SNMP_ENABLE])
	R(MODBUS_SNMP_ENABLE) = Device_Snmp_Point.reg.enable;
	R(MODBUS_SNMP_PORT) = Device_Snmp_Point.reg.port;
	SnmpStringToRegs(Device_Snmp_Point.reg.ro_community, sizeof(Device_Snmp_Point.reg.ro_community), &R(MODBUS_SNMP_RO_COMMUNITY), 16);
	SnmpStringToRegs(Device_Snmp_Point.reg.rw_community, sizeof(Device_Snmp_Point.reg.rw_community), &R(MODBUS_SNMP_RW_COMMUNITY), 16);
	for (int i = 0; i < 4; i++)
		R(MODBUS_SNMP_TRAP_IP + i) = Device_Snmp_Point.reg.trap_dest_ip[i];
	R(MODBUS_SNMP_TRAP_PORT) = Device_Snmp_Point.reg.trap_dest_port;
	SnmpStringToRegs(Device_Snmp_Point.reg.sys_name, sizeof(Device_Snmp_Point.reg.sys_name), &R(MODBUS_SNMP_SYS_NAME), 16);
	SnmpStringToRegs(Device_Snmp_Point.reg.sys_location, sizeof(Device_Snmp_Point.reg.sys_location), &R(MODBUS_SNMP_SYS_LOCATION), 16);
	SnmpStringToRegs(Device_Snmp_Point.reg.sys_contact, sizeof(Device_Snmp_Point.reg.sys_contact), &R(MODBUS_SNMP_SYS_CONTACT), 16);

	/* 'running' (2452) is read-only: write enable (2451) and then port..sys_contact (2453-2538). */
	if (!SnmpWriteRegs(&R(MODBUS_SNMP_ENABLE), MODBUS_SNMP_ENABLE, 1))
		return FALSE;
	BOOL ok = SnmpWriteRegs(&R(MODBUS_SNMP_PORT), MODBUS_SNMP_PORT, MODBUS_SNMP_REG_END - MODBUS_SNMP_PORT + 1);
	#undef R
	return ok;
}

BOOL CSnmpConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/* Field limits = struct sizes - 1 (NUL terminator). */
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_PORT))->SetLimitText(5);
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_RO_COMMUNITY))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_RW_COMMUNITY))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_TRAP_PORT))->SetLimitText(5);
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_SYS_NAME))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_SYS_LOCATION))->SetLimitText(31);
	((CEdit*)GetDlgItem(IDC_EDIT_SNMP_SYS_CONTACT))->SetLimitText(31);

	/* Device_Snmp_Point was filled by the caller (READ_SNMP_CFG) before DoModal. */
	LoadToUi();
	UpdateUIState();
	return TRUE;
}

void CSnmpConfig::LoadToUi()
{
	if (IsBlank(Device_Snmp_Point))
	{
		SetDefaults(Device_Snmp_Point);
	}

	CheckDlgButton(IDC_CHECK_ENABLE_SNMP,
		Device_Snmp_Point.reg.enable ? BST_CHECKED : BST_UNCHECKED);
	SetDlgItemInt(IDC_EDIT_SNMP_PORT, Device_Snmp_Point.reg.port);

	SetDlgItemText(IDC_EDIT_SNMP_RO_COMMUNITY, SnmpAnsiField(Device_Snmp_Point.reg.ro_community, sizeof(Device_Snmp_Point.reg.ro_community)));
	SetDlgItemText(IDC_EDIT_SNMP_RW_COMMUNITY, SnmpAnsiField(Device_Snmp_Point.reg.rw_community, sizeof(Device_Snmp_Point.reg.rw_community)));

	CIPAddressCtrl* pTrapIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SNMP_TRAP_IP);
	if (pTrapIP)
	{
		pTrapIP->SetAddress(
			Device_Snmp_Point.reg.trap_dest_ip[0],
			Device_Snmp_Point.reg.trap_dest_ip[1],
			Device_Snmp_Point.reg.trap_dest_ip[2],
			Device_Snmp_Point.reg.trap_dest_ip[3]);
	}
	SetDlgItemInt(IDC_EDIT_SNMP_TRAP_PORT, Device_Snmp_Point.reg.trap_dest_port);

	SetDlgItemText(IDC_EDIT_SNMP_SYS_NAME, SnmpAnsiField(Device_Snmp_Point.reg.sys_name, sizeof(Device_Snmp_Point.reg.sys_name)));
	SetDlgItemText(IDC_EDIT_SNMP_SYS_LOCATION, SnmpAnsiField(Device_Snmp_Point.reg.sys_location, sizeof(Device_Snmp_Point.reg.sys_location)));
	SetDlgItemText(IDC_EDIT_SNMP_SYS_CONTACT, SnmpAnsiField(Device_Snmp_Point.reg.sys_contact, sizeof(Device_Snmp_Point.reg.sys_contact)));

	SetDlgItemText(IDC_STATIC_SNMP_STATUS,
		Device_Snmp_Point.reg.running ? _T("Running") : _T("Stopped"));
}

void CSnmpConfig::SaveFromUi()
{
	CString str;

	Device_Snmp_Point.reg.enable =
		(IsDlgButtonChecked(IDC_CHECK_ENABLE_SNMP) == BST_CHECKED) ? 1 : 0;

	BOOL bTrans = FALSE;
	UINT nPort = GetDlgItemInt(IDC_EDIT_SNMP_PORT, &bTrans);
	if (bTrans && nPort > 0 && nPort <= 65535)
		Device_Snmp_Point.reg.port = (unsigned short)nPort;

	GetDlgItemText(IDC_EDIT_SNMP_RO_COMMUNITY, str);
	SnmpSetField(Device_Snmp_Point.reg.ro_community, sizeof(Device_Snmp_Point.reg.ro_community), str);
	GetDlgItemText(IDC_EDIT_SNMP_RW_COMMUNITY, str);
	SnmpSetField(Device_Snmp_Point.reg.rw_community, sizeof(Device_Snmp_Point.reg.rw_community), str);

	CIPAddressCtrl* pTrapIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SNMP_TRAP_IP);
	if (pTrapIP)
	{
		BYTE b1, b2, b3, b4;
		pTrapIP->GetAddress(b1, b2, b3, b4);	/* blank control returns 0.0.0.0 */
		Device_Snmp_Point.reg.trap_dest_ip[0] = b1;
		Device_Snmp_Point.reg.trap_dest_ip[1] = b2;
		Device_Snmp_Point.reg.trap_dest_ip[2] = b3;
		Device_Snmp_Point.reg.trap_dest_ip[3] = b4;
	}

	bTrans = FALSE;
	UINT nTrapPort = GetDlgItemInt(IDC_EDIT_SNMP_TRAP_PORT, &bTrans);
	if (bTrans && nTrapPort <= 65535)
		Device_Snmp_Point.reg.trap_dest_port = (unsigned short)nTrapPort;

	GetDlgItemText(IDC_EDIT_SNMP_SYS_NAME, str);
	SnmpSetField(Device_Snmp_Point.reg.sys_name, sizeof(Device_Snmp_Point.reg.sys_name), str);
	GetDlgItemText(IDC_EDIT_SNMP_SYS_LOCATION, str);
	SnmpSetField(Device_Snmp_Point.reg.sys_location, sizeof(Device_Snmp_Point.reg.sys_location), str);
	GetDlgItemText(IDC_EDIT_SNMP_SYS_CONTACT, str);
	SnmpSetField(Device_Snmp_Point.reg.sys_contact, sizeof(Device_Snmp_Point.reg.sys_contact), str);
}

void CSnmpConfig::UpdateUIState()
{
	BOOL bEnabled = (IsDlgButtonChecked(IDC_CHECK_ENABLE_SNMP) == BST_CHECKED);

	GetDlgItem(IDC_EDIT_SNMP_PORT)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_SNMP_RO_COMMUNITY)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_SNMP_RW_COMMUNITY)->EnableWindow(bEnabled);
	GetDlgItem(IDC_IPADDRESS_SNMP_TRAP_IP)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_SNMP_TRAP_PORT)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_SNMP_SYS_NAME)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_SNMP_SYS_LOCATION)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_SNMP_SYS_CONTACT)->EnableWindow(bEnabled);

	/* Apply must stay enabled so the user can write enable=0 to the device. */
	GetDlgItem(IDC_BUTTON_WRITE_SNMP_CFG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CANCEL_SNMP_CFG)->EnableWindow(TRUE);
}

BOOL CSnmpConfig::ValidateInput()
{
	if (IsDlgButtonChecked(IDC_CHECK_ENABLE_SNMP) != BST_CHECKED)
		return TRUE;

	BOOL bTrans = FALSE;
	UINT nPort = GetDlgItemInt(IDC_EDIT_SNMP_PORT, &bTrans);
	if (!bTrans || nPort == 0 || nPort > 65535)
	{
		MessageBox(_T("SNMP port must be between 1 and 65535."), _T("SNMP"), MB_ICONERROR);
		return FALSE;
	}

	CString str;
	GetDlgItemText(IDC_EDIT_SNMP_RO_COMMUNITY, str);
	if (str.IsEmpty())
	{
		MessageBox(_T("Please enter the read-only community string."), _T("SNMP"), MB_ICONERROR);
		return FALSE;
	}

	bTrans = FALSE;
	UINT nTrapPort = GetDlgItemInt(IDC_EDIT_SNMP_TRAP_PORT, &bTrans);
	if (!bTrans || nTrapPort > 65535)
	{
		MessageBox(_T("Trap port must be between 0 and 65535."), _T("SNMP"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

void CSnmpConfig::OnBnClickedCheckEnableSnmp()
{
	UpdateUIState();
}

void CSnmpConfig::OnBnClickedButtonWriteSnmpCfg()
{
	if (!ValidateInput())
		return;

	SaveFromUi();

	if (!WriteViaBacnet() && !WriteViaModbus())
	{
		MessageBox(_T("Failed to apply SNMP configuration."), _T("SNMP"), MB_ICONERROR);
		return;
	}

	MessageBox(_T("SNMP configuration applied successfully."), _T("SNMP"), MB_ICONINFORMATION);
}

void CSnmpConfig::OnBnClickedButtonCancelSnmpCfg()
{
	OnCancel();
}
