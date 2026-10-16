#include "stdafx.h"
#include "T3000.h"
#include "CGsmStatus.h"
#include "global_function.h"
#include "afxdialogex.h"

/* Must match Hub modbus.h MODBUS_GSM_* */
#define MODBUS_GSM_MODEM_STATE     2251
#define MODBUS_GSM_OPERATOR_START  2265
#define MODBUS_GSM_APN_START       2281
#define MODBUS_GSM_REG_COUNT       46   /* 2251 .. 2296 inclusive */

#define GSM_FLAG_SIM_READY      (1u << 0)
#define GSM_FLAG_REGISTERED     (1u << 1)
#define GSM_FLAG_ATTACHED       (1u << 2)
#define GSM_FLAG_CONNECTED      (1u << 3)
#define GSM_FLAG_AT_READY       (1u << 4)
#define GSM_FLAG_SIM1_PRESENT   (1u << 5)
#define GSM_FLAG_SIM2_PRESENT   (1u << 6)
#define GSM_FLAG_STATUS_VALID   (1u << 7)

IMPLEMENT_DYNAMIC(CGsmStatus, CDialogEx)

CGsmStatus::CGsmStatus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GSM_STATUS, pParent)
	, m_sim1_present(FALSE)
	, m_sim2_present(FALSE)
{
}

CGsmStatus::~CGsmStatus()
{
}

void CGsmStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGsmStatus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GSM_REFRESH, &CGsmStatus::OnBnClickedButtonGsmRefresh)
	ON_BN_CLICKED(IDC_BUTTON_GSM_CLOSE, &CGsmStatus::OnBnClickedButtonGsmClose)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

BOOL CGsmStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ApplyToUi();
	SetDlgItemText(IDC_EDIT_GSM_SOURCE, _T("BACnet"));
	return TRUE;
}

BOOL CGsmStatus::RefreshFromDevice()
{
	BOOL ok = LoadViaBacnet();
	CString source = _T("BACnet");
	if (!ok) {
		ok = LoadViaModbus();
		source = _T("Modbus");
	}

	if (!ok) {
		MessageBox(_T("Failed to read GSM status over BACnet and Modbus."),
			_T("GSM Status"), MB_ICONWARNING);
		SetDlgItemText(IDC_EDIT_GSM_SOURCE, _T("Read failed"));
		return FALSE;
	}

	ApplyToUi();
	SetDlgItemText(IDC_EDIT_GSM_SOURCE, source);
	return TRUE;
}

BOOL CGsmStatus::LoadViaBacnet()
{
	if (GetPrivateData_Blocking(g_bac_instance, READ_GSM_STATUS, 0, 0,
		sizeof(Str_Gsm_status_point)) < 0) {
		return FALSE;
	}
	return TRUE;
}

BOOL CGsmStatus::LoadViaModbus()
{
	unsigned short regs[MODBUS_GSM_REG_COUNT] = { 0 };
	int ret = Read_Multi((unsigned char)g_tstat_id, regs,
		MODBUS_GSM_MODEM_STATE, MODBUS_GSM_REG_COUNT, 3);
	if (ret < 0) {
		return FALSE;
	}

	memset(&Device_Gsm_Status_Point, 0, sizeof(Device_Gsm_Status_Point));
	Device_Gsm_Status_Point.reg.modem_state = (unsigned char)(regs[0] & 0xff);
	Device_Gsm_Status_Point.reg.ppp_state = (unsigned char)(regs[1] & 0xff);
	Device_Gsm_Status_Point.reg.flags = (unsigned char)(regs[2] & 0xff);
	Device_Gsm_Status_Point.reg.active_sim = (unsigned char)(regs[3] & 0xff);
	Device_Gsm_Status_Point.reg.csq = (unsigned char)(regs[4] & 0xff);
	Device_Gsm_Status_Point.reg.rssi_dbm = (signed char)(regs[5] & 0xff);
	Device_Gsm_Status_Point.reg.creg_stat = (unsigned char)(regs[6] & 0xff);
	Device_Gsm_Status_Point.reg.cereg_stat = (unsigned char)(regs[7] & 0xff);
	Device_Gsm_Status_Point.reg.ip[0] = (unsigned char)(regs[8] & 0xff);
	Device_Gsm_Status_Point.reg.ip[1] = (unsigned char)(regs[9] & 0xff);
	Device_Gsm_Status_Point.reg.ip[2] = (unsigned char)(regs[10] & 0xff);
	Device_Gsm_Status_Point.reg.ip[3] = (unsigned char)(regs[11] & 0xff);
	Device_Gsm_Status_Point.reg.status_age_s = regs[12];
	Device_Gsm_Status_Point.reg.last_error = regs[13];

	for (int i = 0; i < 16; i++) {
		unsigned short w = regs[(MODBUS_GSM_OPERATOR_START - MODBUS_GSM_MODEM_STATE) + i];
		Device_Gsm_Status_Point.reg.operator_name[i * 2] = (char)((w >> 8) & 0xff);
		Device_Gsm_Status_Point.reg.operator_name[i * 2 + 1] = (char)(w & 0xff);
	}
	for (int i = 0; i < 16; i++) {
		unsigned short w = regs[(MODBUS_GSM_APN_START - MODBUS_GSM_MODEM_STATE) + i];
		Device_Gsm_Status_Point.reg.apn[i * 2] = (char)((w >> 8) & 0xff);
		Device_Gsm_Status_Point.reg.apn[i * 2 + 1] = (char)(w & 0xff);
	}
	Device_Gsm_Status_Point.reg.operator_name[31] = '\0';
	Device_Gsm_Status_Point.reg.apn[31] = '\0';
	return TRUE;
}

CString CGsmStatus::ModemStateName(unsigned char state)
{
	switch (state) {
	case 0: return _T("OFF");
	case 1: return _T("BOOTING");
	case 2: return _T("AT_READY");
	case 3: return _T("SIM_READY");
	case 4: return _T("REGISTERED");
	case 5: return _T("ATTACHED");
	case 6: return _T("CONNECTED");
	case 7: return _T("ERROR");
	default: return _T("UNKNOWN");
	}
}

CString CGsmStatus::PppStateName(unsigned char state)
{
	switch (state) {
	case 0: return _T("IDLE");
	case 1: return _T("WAIT_UART");
	case 2: return _T("MODEM_READY");
	case 3: return _T("STARTING");
	case 4: return _T("RUNNING");
	case 5: return _T("STOPPING");
	case 6: return _T("ERROR");
	default: return _T("UNKNOWN");
	}
}

CString CGsmStatus::FlagsText(unsigned char flags)
{
	CString s;
	if (flags & GSM_FLAG_STATUS_VALID) s += _T("valid ");
	if (flags & GSM_FLAG_AT_READY) s += _T("AT ");
	if (flags & GSM_FLAG_SIM_READY) s += _T("SIM ");
	if (flags & GSM_FLAG_REGISTERED) s += _T("reg ");
	if (flags & GSM_FLAG_ATTACHED) s += _T("attach ");
	if (flags & GSM_FLAG_CONNECTED) s += _T("PPP ");
	if (flags & GSM_FLAG_SIM1_PRESENT) s += _T("SIM1 ");
	if (flags & GSM_FLAG_SIM2_PRESENT) s += _T("SIM2 ");
	if (s.IsEmpty())
		s = _T("(none)");
	return s;
}

void CGsmStatus::UpdateSimButtons()
{
	const unsigned char flags = Device_Gsm_Status_Point.reg.flags;
	m_sim1_present = (flags & GSM_FLAG_SIM1_PRESENT) ? TRUE : FALSE;
	m_sim2_present = (flags & GSM_FLAG_SIM2_PRESENT) ? TRUE : FALSE;

	if (!m_sim1_present && !m_sim2_present)
		SetDlgItemText(IDC_STATIC_GSM_NO_SIM, _T("No sim"));
	else
		SetDlgItemText(IDC_STATIC_GSM_NO_SIM, _T(""));

	CWnd *p1 = GetDlgItem(IDC_BTN_GSM_SIM1);
	CWnd *p2 = GetDlgItem(IDC_BTN_GSM_SIM2);
	if (p1) p1->Invalidate(FALSE);
	if (p2) p2->Invalidate(FALSE);
}

void CGsmStatus::DrawSimButton(LPDRAWITEMSTRUCT lpDrawItemStruct, BOOL present)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (pDC == NULL)
		return;

	CRect rc(lpDrawItemStruct->rcItem);
	const COLORREF fill = present ? RGB(198, 239, 206) : RGB(255, 205, 210);
	const COLORREF border = present ? RGB(46, 125, 50) : RGB(198, 40, 40);
	const COLORREF text = present ? RGB(27, 94, 32) : RGB(183, 28, 28);

	pDC->FillSolidRect(rc, fill);
	CBrush brBorder(border);
	pDC->FrameRect(rc, &brBorder);

	CString label;
	GetDlgItemText(lpDrawItemStruct->CtlID, label);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(text);
	pDC->DrawText(label, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CGsmStatus::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_BTN_GSM_SIM1) {
		DrawSimButton(lpDrawItemStruct, m_sim1_present);
		return;
	}
	if (nIDCtl == IDC_BTN_GSM_SIM2) {
		DrawSimButton(lpDrawItemStruct, m_sim2_present);
		return;
	}
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGsmStatus::ApplyToUi()
{
	const Str_Gsm_status_point &g = Device_Gsm_Status_Point;
	CString t;
	CString name;

	UpdateSimButtons();

	name = ModemStateName(g.reg.modem_state);
	t.Format(_T("%u (%s)"), g.reg.modem_state, (LPCTSTR)name);
	SetDlgItemText(IDC_EDIT_GSM_MODEM_STATE, t);

	name = PppStateName(g.reg.ppp_state);
	t.Format(_T("%u (%s)"), g.reg.ppp_state, (LPCTSTR)name);
	SetDlgItemText(IDC_EDIT_GSM_PPP_STATE, t);

	name = FlagsText(g.reg.flags);
	t.Format(_T("0x%02X  %s"), g.reg.flags, (LPCTSTR)name);
	SetDlgItemText(IDC_EDIT_GSM_FLAGS, t);

	{
		CString active;
		CString sim1 = (g.reg.flags & GSM_FLAG_SIM1_PRESENT) ? _T("present") : _T("absent");
		CString sim2 = (g.reg.flags & GSM_FLAG_SIM2_PRESENT) ? _T("present") : _T("absent");
		if (g.reg.active_sim == 1)
			active = _T("SIM1");
		else if (g.reg.active_sim == 2)
			active = _T("SIM2");
		else
			active = _T("None");
		if (!(g.reg.flags & GSM_FLAG_SIM1_PRESENT) && !(g.reg.flags & GSM_FLAG_SIM2_PRESENT))
			t.Format(_T("No SIM detected  (active: %s)"), (LPCTSTR)active);
		else
			t.Format(_T("SIM1 %s, SIM2 %s  (active: %s)"),
				(LPCTSTR)sim1, (LPCTSTR)sim2, (LPCTSTR)active);
		SetDlgItemText(IDC_EDIT_GSM_ACTIVE_SIM, t);
	}

	if (g.reg.csq == 99)
		t = _T("99 (unknown)");
	else
		t.Format(_T("%u"), g.reg.csq);
	SetDlgItemText(IDC_EDIT_GSM_CSQ, t);

	t.Format(_T("%d dBm"), (int)g.reg.rssi_dbm);
	SetDlgItemText(IDC_EDIT_GSM_RSSI, t);

	t.Format(_T("%u"), g.reg.creg_stat);
	SetDlgItemText(IDC_EDIT_GSM_CREG, t);

	t.Format(_T("%u"), g.reg.cereg_stat);
	SetDlgItemText(IDC_EDIT_GSM_CEREG, t);

	t.Format(_T("%u.%u.%u.%u"),
		g.reg.ip[0], g.reg.ip[1], g.reg.ip[2], g.reg.ip[3]);
	SetDlgItemText(IDC_EDIT_GSM_IP, t);

	t.Format(_T("%u s"), g.reg.status_age_s);
	SetDlgItemText(IDC_EDIT_GSM_AGE, t);

	t.Format(_T("0x%04X"), g.reg.last_error);
	SetDlgItemText(IDC_EDIT_GSM_LAST_ERROR, t);

	char op[33] = { 0 };
	char apn[33] = { 0 };
	memcpy(op, g.reg.operator_name, 32);
	memcpy(apn, g.reg.apn, 32);
	SetDlgItemText(IDC_EDIT_GSM_OPERATOR, CString(op));
	SetDlgItemText(IDC_EDIT_GSM_APN, CString(apn));
}

void CGsmStatus::OnBnClickedButtonGsmRefresh()
{
	RefreshFromDevice();
}

void CGsmStatus::OnBnClickedButtonGsmClose()
{
	EndDialog(IDCANCEL);
}
