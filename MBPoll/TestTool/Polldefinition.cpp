// Polldefinition.cpp : implementation file
//

#include "stdafx.h"
#include "TestTool.h"
#include "Polldefinition.h"
#include "afxdialogex.h"
#include "global_variable_extern.h"
#include "globle_function.h"
// CPolldefinition dialog

IMPLEMENT_DYNAMIC(CPolldefinition, CDialogEx)

CPolldefinition::CPolldefinition(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPolldefinition::IDD, pParent)
	, m_poll_id(0)
	, m_poll_address(0)
	, m_poll_length(0)
	, m_poll_scan_interval(0)
{

}

CPolldefinition::~CPolldefinition()
{
}

void CPolldefinition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SLAVE_ID, m_poll_id);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_poll_address);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_poll_length);
	DDX_Text(pDX, IDC_EDIT_SCAN_RATE, m_poll_scan_interval);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, m_poll_function_ctrl);
}


BEGIN_MESSAGE_MAP(CPolldefinition, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPolldefinition::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CPolldefinition::OnBnClickedOk)
//	ON_WM_KEYDOWN()
//ON_WM_VKEYTOITEM()
ON_BN_CLICKED(IDCANCEL, &CPolldefinition::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON_READ_ONCE, &CPolldefinition::OnBnClickedButtonReadOnce)
END_MESSAGE_MAP()


// CPolldefinition message handlers


BOOL CPolldefinition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPolldefinition::InitAllControls()
{
	 m_poll_id = g_read_ID;
	 m_poll_address = g_read_start_address;
	 m_poll_length = g_read_length;
	 m_poll_scan_interval = g_read_interval;

	 m_poll_function_ctrl.ResetContent();
	/* m_poll_function_ctrl.AddString(_T("01 COIL STATUS"));
	 m_poll_function_ctrl.AddString(_T("02 INPUT STATUS"));*/
	 m_poll_function_ctrl.AddString(_T("03    HOLDING REGISTER"));
	 m_poll_function_ctrl.SetCurSel(0);
	 if(g_auto_read)
	 {
		((CButton *) GetDlgItem(IDC_CHECK_AUTO_READ))->SetCheck(1);
		((CButton *) GetDlgItem(IDC_BUTTON_READ_ONCE))->EnableWindow(FALSE);
	 }
	 else
	 {
		 ((CButton *) GetDlgItem(IDC_CHECK_AUTO_READ))->SetCheck(0);
		 ((CButton *) GetDlgItem(IDC_BUTTON_READ_ONCE))->EnableWindow(TRUE);
	 }
	 UpdateData(FALSE);
}

bool CPolldefinition::Save_Parameter()
{
	UpdateData(TRUE);

	g_read_ID = m_poll_id;
	g_read_start_address = m_poll_address;
	g_read_length = m_poll_length;
	g_read_interval = m_poll_scan_interval;
	g_read_function = 3;
	if( ((CButton *) GetDlgItem(IDC_CHECK_AUTO_READ))->GetCheck()==1)
	{
		g_auto_read = 1;
	}
	else
	{
		g_auto_read = 0;
	}

	if((g_read_ID<0)||(g_read_ID>255))
	{
		AfxMessageBox(_T("The Slave ID must between 1 and 255"));
		return false;
	}
	else if((g_read_length<0)||(g_read_length>127))
	{
		AfxMessageBox(_T("The Length must between 1 and 127"));
		return false;
	}
	
	if(g_read_length>=127)
		g_read_length=126;

	// TODO: Add your control notification handler code here
	CString temp_slave_id,temp_function,temp_start_address,temp_read_length,temp_scan_time,temp_auto_read;
	//temp_slave_id.form
	temp_slave_id.Format(_T("%d"),g_read_ID);
	temp_function.Format(_T("%d"),g_read_function);
	temp_start_address.Format(_T("%d"),g_read_start_address);
	temp_read_length.Format(_T("%d"),g_read_length);
	temp_scan_time.Format(_T("%d"),g_read_interval);
	temp_auto_read.Format(_T("%d"),g_auto_read);

	WritePrivateProfileStringW(_T("PollDefinition"),_T("SlaveID"),temp_slave_id,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("PollDefinition"),_T("Function"),temp_function,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("PollDefinition"),_T("Start Address"),temp_start_address,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("PollDefinition"),_T("Read Length"),temp_read_length,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("PollDefinition"),_T("Scan Time"),temp_scan_time,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("PollDefinition"),_T("Auto Read"),temp_auto_read,Program_ConfigFile_Path);

	::PostMessage(main_hwnd,MY_SHOW_MSG,10,NULL);
	return true;
}

void CPolldefinition::OnBnClickedButton1()
{
	if(Save_Parameter())
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
}


void CPolldefinition::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(Save_Parameter())
		CDialogEx::OnOK();
}


//void CPolldefinition::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: Add your message handler code here and/or call default
//	GetDlgItem(IDC_BUTTON1)->EnableWindow(1);
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}


//int CPolldefinition::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return CDialogEx::OnVKeyToItem(nKey, pListBox, nIndex);
//}


BOOL CPolldefinition::PreTranslateMessage(MSG* pMsg)
{
//	int aaaa;
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if((pMsg->wParam>=0x30 && pMsg->wParam<=0x39)||(pMsg->wParam>=0x60 && pMsg->wParam<=0x69))	
			GetDlgItem(IDC_BUTTON1)->EnableWindow(1);//如果有数字键按下或者小键盘的案件按下，则将 应用的案件enable
		break;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPolldefinition::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CPolldefinition::OnBnClickedButtonReadOnce()
{
	// TODO: Add your control notification handler code here
	Post_Thread_Message(MY_READ_MULTY,my_readmult);
}

