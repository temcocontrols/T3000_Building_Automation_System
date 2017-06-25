// BacnetRemotePortWarning.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRemotePortWarning.h"
#include "afxdialogex.h"


// CBacnetRemotePortWarning dialog

IMPLEMENT_DYNAMIC(CBacnetRemotePortWarning, CDialogEx)

CBacnetRemotePortWarning::CBacnetRemotePortWarning(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetRemotePortWarning::IDD, pParent)
{

}

CBacnetRemotePortWarning::~CBacnetRemotePortWarning()
{
}

void CBacnetRemotePortWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_REMOTE_PORT_WARNING, m_static_remote_warning_info);
	DDX_Control(pDX, IDC_STATIC_IGNORE_TEXT, m_static_ignore_text);
}


BEGIN_MESSAGE_MAP(CBacnetRemotePortWarning, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_REMOTE_PORT_WARNING, &CBacnetRemotePortWarning::OnBnClickedCheckRemotePortWarning)
END_MESSAGE_MAP()


// CBacnetRemotePortWarning message handlers


BOOL CBacnetRemotePortWarning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_static_remote_warning_info.SetWindowTextW(_T("In order to connect to a remote system over the internet\r\nthe remote device requires access to port 502 TCP and \
47808 UDP. You can set this up in the router at the remote site.\r\n You will need to use port forwarding to route these ports\r\n to the panel with ID=1 at the remote site.\r\n\r\n\
Port 47808 is fixed while the port 502 is user configurable\r\nthe setting can be found in the same advanced settings \r\ndialog where you defined the IP address for the controller"));
	m_static_remote_warning_info.textColor(RGB(0,0,0));
	m_static_remote_warning_info.setFont(24,18,NULL,_T("Arial"));


	m_static_ignore_text.SetWindowTextW(_T("Ignore this dialog from now on"));
	m_static_ignore_text.textColor(RGB(0,0,0));
	//m_static_remote_warning_info.bkColor(RGB(0,255,255));
	m_static_ignore_text.setFont(24,18,NULL,_T("Arial"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetRemotePortWarning::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetRemotePortWarning::OnBnClickedCheckRemotePortWarning()
{
	
	//if(GetPrivateProfileInt(_T("Setting") ,_T("ENABLE_PORT_WARNING") ,1,g_cstring_ini_path ) == 1)
	if(((CButton *)GetDlgItem(IDC_CHECK_REMOTE_PORT_WARNING))->GetCheck())
	{
		WritePrivateProfileString(_T("Setting") ,_T("ENABLE_PORT_WARNING"),_T("0"),g_cstring_ini_path);
	}
	else
	{
		WritePrivateProfileString(_T("Setting") ,_T("ENABLE_PORT_WARNING"),_T("1"),g_cstring_ini_path);
	}
}
