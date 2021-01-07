// BacnetEmailAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetEmailAlarm.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetEmailAlarm 对话框

IMPLEMENT_DYNAMIC(CBacnetEmailAlarm, CDialogEx)

CBacnetEmailAlarm::CBacnetEmailAlarm(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_EMAIL_ALARM, pParent)
{

}

CBacnetEmailAlarm::~CBacnetEmailAlarm()
{
}

void CBacnetEmailAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetEmailAlarm, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CBacnetEmailAlarm::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBacnetEmailAlarm 消息处理程序


void CBacnetEmailAlarm::OnBnClickedButton1()
{
#ifdef ENABLE_T3_EMAIL
    // TODO: 在此添加控件通知处理程序代码
    CString cs_smtp_server;
    CString cs_smtp_port;
    CString cs_email_Address;
    CString cs_user_name;
    CString cs_password;
    GetDlgItem(IDC_EDIT_SMTP_SERVER)->GetWindowTextW(cs_smtp_server);
    GetDlgItem(IDC_EDIT_PORT_NUMBER)->GetWindowTextW(cs_smtp_port);
    GetDlgItem(IDC_EDIT_EMAIL)->GetWindowTextW(cs_email_Address);
    GetDlgItem(IDC_EDIT_USERNAME)->GetWindowTextW(cs_user_name);
    GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(cs_password);

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->GetLBText(nSel, temp_string);
    if (nSel == 0)
        Device_Email_Point.reg.secure_connection_type = 0;
    else if (nSel == 1)
        Device_Email_Point.reg.secure_connection_type = 1;
    else if (nSel == 2)
        Device_Email_Point.reg.secure_connection_type = 2;

   
    WideCharToMultiByte(CP_ACP, NULL, cs_smtp_server.GetBuffer(), -1, Device_Email_Point.reg.smtp_domain, 40, NULL, NULL);
    WideCharToMultiByte(CP_ACP, NULL, cs_email_Address.GetBuffer(), -1, Device_Email_Point.reg.email_address, 60, NULL, NULL);
    WideCharToMultiByte(CP_ACP, NULL, cs_user_name.GetBuffer(), -1, Device_Email_Point.reg.user_name, 60, NULL, NULL);
    WideCharToMultiByte(CP_ACP, NULL, cs_password.GetBuffer(), -1, Device_Email_Point.reg.password, 20, NULL, NULL);

    int temp_port = _wtoi(cs_smtp_port);
    Device_Email_Point.reg.smtp_port = temp_port;

    CStringArray temparray;
    SplitCStringA(temparray, cs_smtp_server, _T("."));
    if ((temparray.GetSize() == 4))
    {
        Device_Email_Point.reg.smtp_type = 0;
    }
    else
    {
        Device_Email_Point.reg.smtp_type = 1;
    }

    if (Write_Private_Data_Blocking(WRITE_EMAIL_ALARM, 0, 0) <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Configuration failed!"));
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Configuration success!"));
    }

#endif

}


BOOL CBacnetEmailAlarm::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    Fresh_UI();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CBacnetEmailAlarm::Fresh_UI()
{
    //Device_Email_Point
    //unsigned char smtp_type;  //  0   ipaddress   // 1   domain
    //unsigned char smtp_ip[4];
    //char smtp_domain[40];
    //unsigned short smtp_port;
    //char email_address[60];
    //char user_name[60];
    //char password[20];
    //char secure_connection_type;  //0 -NULL   1-SSL   2-TLS

    ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->ResetContent();
    ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->AddString(_T("NULL"));
    ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->AddString(_T("SSL"));
    ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->AddString(_T("TLS"));

    CString cs_smtp_server;
    CString cs_smtp_port;
    CString cs_email_Address;
    CString cs_user_name;
    CString cs_password;
    if (Device_Email_Point.reg.smtp_type == 0)
    {
        cs_smtp_server.Format(_T("%u.%u.%u.%u"), Device_Email_Point.reg.smtp_ip[0],
            Device_Email_Point.reg.smtp_ip[1], Device_Email_Point.reg.smtp_ip[2], Device_Email_Point.reg.smtp_ip[3]);
    }
    else
    {
        MultiByteToWideChar(CP_ACP, 0, (char *)Device_Email_Point.reg.smtp_domain, (int)strlen((char *)Device_Email_Point.reg.smtp_domain) + 1,
            cs_smtp_server.GetBuffer(MAX_PATH), MAX_PATH);
        cs_smtp_server.ReleaseBuffer();
    }

    cs_smtp_port.Format(_T("%u"), Device_Email_Point.reg.smtp_port);

    MultiByteToWideChar(CP_ACP, 0, (char *)Device_Email_Point.reg.email_address, (int)strlen((char *)Device_Email_Point.reg.email_address) + 1,
        cs_email_Address.GetBuffer(MAX_PATH), MAX_PATH);
    cs_email_Address.ReleaseBuffer();
    MultiByteToWideChar(CP_ACP, 0, (char *)Device_Email_Point.reg.password, (int)strlen((char *)Device_Email_Point.reg.password) + 1,
        cs_password.GetBuffer(MAX_PATH), MAX_PATH);
    cs_password.ReleaseBuffer();
    MultiByteToWideChar(CP_ACP, 0, (char *)Device_Email_Point.reg.user_name, (int)strlen((char *)Device_Email_Point.reg.user_name) + 1,
        cs_user_name.GetBuffer(MAX_PATH), MAX_PATH);
    cs_user_name.ReleaseBuffer();


    cs_email_Address.Trim();
    cs_user_name.Trim();
    cs_smtp_server.Trim();
    GetDlgItem(IDC_EDIT_SMTP_SERVER)->SetWindowTextW(cs_smtp_server);
    GetDlgItem(IDC_EDIT_PORT_NUMBER)->SetWindowTextW(cs_smtp_port);
    GetDlgItem(IDC_EDIT_EMAIL)->SetWindowTextW(cs_email_Address);
    GetDlgItem(IDC_EDIT_USERNAME)->SetWindowTextW(cs_user_name);
    GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW(cs_password);

    if (Device_Email_Point.reg.secure_connection_type == 0)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->SetCurSel(0);
    }
    else if (Device_Email_Point.reg.secure_connection_type == 1)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->SetCurSel(1);
    }
    else if (Device_Email_Point.reg.secure_connection_type == 2)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->SetCurSel(2);
    }
    else
        ((CComboBox *)GetDlgItem(IDC_COMBO_SECURE_TYPE))->SetCurSel(0);
}

BOOL CBacnetEmailAlarm::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}
