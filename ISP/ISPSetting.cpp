// ISPSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ISP.h"
#include "ISPSetting.h"
#include "afxdialogex.h"
extern bool auto_flash_mode;
extern unsigned int Remote_timeout;
extern unsigned int nflash_receive_to_send_delay_time;
extern CString SettingPath;
// CISPSetting dialog

IMPLEMENT_DYNAMIC(CISPSetting, CDialogEx)

CISPSetting::CISPSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CISPSetting::IDD, pParent)
{

}

CISPSetting::~CISPSetting()
{
}

void CISPSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CISPSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_OK, &CISPSetting::OnBnClickedButtonSettingOk)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCEL, &CISPSetting::OnBnClickedButtonSettingCancel)
END_MESSAGE_MAP()


// CISPSetting message handlers


void CISPSetting::OnBnClickedButtonSettingOk()
{
	// TODO: Add your control notification handler code here
	CString temptimeout;
	GetDlgItemText(IDC_EDIT_SETTING_TIMEOUT,temptimeout);
	unsigned int temp_value;
	temp_value = _wtoi(temptimeout);
	if(temp_value < 50)
	{
		if(!auto_flash_mode)
			MessageBox(_T("Please input a higher number"),_T("Information"),MB_OK | MB_INACTIVE);
		return;
	}
	else
	{
		WritePrivateProfileStringW(_T("Setting"),_T("REMOTE_TIMEOUT"),temptimeout,SettingPath);
		Remote_timeout = temp_value;
		PostMessage(WM_CLOSE,NULL,NULL);
	}

    CString tempresponse;
    GetDlgItemText(IDC_EDIT_SETTING_RESPONSE_TIME, tempresponse);
    temp_value = _wtoi(tempresponse);

    WritePrivateProfileStringW(_T("Setting"), _T("ResponseTime"), tempresponse, SettingPath);
    nflash_receive_to_send_delay_time = temp_value;
    PostMessage(WM_CLOSE, NULL, NULL);

}


void CISPSetting::OnBnClickedButtonSettingCancel()
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE,NULL,NULL);
}


BOOL CISPSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonSettingOk();
			return 1;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CISPSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString temp_timeout;
	temp_timeout.Format(_T("%d"),Remote_timeout);
	SetDlgItemText(IDC_EDIT_SETTING_TIMEOUT,temp_timeout);
	// TODO:  Add extra initialization here

    CString temp_response;
    temp_response.Format(_T("%u"), nflash_receive_to_send_delay_time);
    SetDlgItemText(IDC_EDIT_SETTING_RESPONSE_TIME, temp_response);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
