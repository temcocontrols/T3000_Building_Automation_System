// RemotePtpLogin.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RemotePtpLogin.h"
#include "afxdialogex.h"

extern unsigned int try_connect_serial;
extern bool ptp_cancel_login;
// CRemotePtpLogin dialog

IMPLEMENT_DYNAMIC(CRemotePtpLogin, CDialogEx)

CRemotePtpLogin::CRemotePtpLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemotePtpLogin::IDD, pParent)
{

}

CRemotePtpLogin::~CRemotePtpLogin()
{
}

void CRemotePtpLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRemotePtpLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_OK, &CRemotePtpLogin::OnBnClickedButtonLoginOk)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_CANCEL, &CRemotePtpLogin::OnBnClickedButtonLoginCancel)
END_MESSAGE_MAP()


// CRemotePtpLogin message handlers


BOOL CRemotePtpLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	CString temp_cs;
	temp_cs.Format(_T("%u"),try_connect_serial);
	GetDlgItem(IDC_EDIT_LOGIN_SERIAL_NUMBER)->SetWindowTextW(temp_cs);
	GetDlgItem(IDC_EDIT_LOGIN_USER_NAME)->SetFocus();
	return false;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CRemotePtpLogin::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CRect list_rect,win_rect;
			CWnd * window_focus =	GetFocus();
			if(window_focus == NULL)
				return CDialogEx::PreTranslateMessage(pMsg);
			if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_LOGIN_SERIAL_NUMBER)
			{
				GetDlgItem(IDC_EDIT_LOGIN_USER_NAME)->SetFocus();
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_LOGIN_USER_NAME)
			{
				GetDlgItem(IDC_EDIT_LOGIN_PASSWORD)->SetFocus();
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_LOGIN_PASSWORD)
			{
				GetDlgItem(IDC_BUTTON_LOGIN_OK)->SetFocus();
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_BUTTON_LOGIN_OK)
			{
				GetDlgItem(IDC_BUTTON_LOGIN_OK)->SetFocus();
			}
			return 1;

			
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRemotePtpLogin::OnBnClickedButtonLoginOk()
{
	
	CString temp_serial_number;
	CString temp_user_name;
	CString temp_password;
	GetDlgItemTextW(IDC_EDIT_LOGIN_USER_NAME,temp_user_name);
	GetDlgItemTextW(IDC_EDIT_LOGIN_PASSWORD,temp_password);
		GetDlgItemTextW(IDC_EDIT_LOGIN_SERIAL_NUMBER,temp_serial_number);
	remote_connect_serial_number = (unsigned int)_wtoi(temp_serial_number);
	memset(ptpLoginName,0,30);
	memset(ptpLoginPassword,0,20);
	WideCharToMultiByte( CP_ACP, 0, temp_user_name.GetBuffer(), -1, ptpLoginName, 30, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, temp_password.GetBuffer(), -1, ptpLoginPassword, 20, NULL, NULL );
	PostMessage(WM_CLOSE,NULL,NULL);
}


void CRemotePtpLogin::OnBnClickedButtonLoginCancel()
{
	
	ptp_cancel_login = true;
	PostMessage(WM_CLOSE,NULL,NULL);
}
