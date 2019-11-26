// BacnetSettingDyndns.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingDyndns.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetSettingDyndns 对话框

IMPLEMENT_DYNAMIC(CBacnetSettingDyndns, CDialogEx)

CBacnetSettingDyndns::CBacnetSettingDyndns(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_SETTING_DYNDNS, pParent)
{

}

CBacnetSettingDyndns::~CBacnetSettingDyndns()
{
}

void CBacnetSettingDyndns::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_EDIT_DYNDNS_USER_NAME, m_dyndns_user);
    DDX_Control(pDX, IDC_EDIT_DYNDNS_PASSWORD, m_dyndns_password);
    DDX_Control(pDX, IDC_EDIT_DYNDNS_DOMAIN, m_dyndns_domain);
    DDX_Control(pDX, IDC_EDIT_TIME_UPDATE, m_edit_ddns_update_time);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingDyndns, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_USER_NAME, &CBacnetSettingDyndns::OnEnKillfocusEditDyndnsUserName)
    ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_PASSWORD, &CBacnetSettingDyndns::OnEnKillfocusEditDyndnsPassword)
    ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_DOMAIN, &CBacnetSettingDyndns::OnEnKillfocusEditDyndnsDomain)
    ON_BN_CLICKED(IDC_CHECK_SETTING_DYNDNS, &CBacnetSettingDyndns::OnBnClickedCheckSettingDyndns)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_DDNS_SERVER, &CBacnetSettingDyndns::OnCbnSelchangeComboBacnetSettingDdnsServer)
    ON_EN_KILLFOCUS(IDC_EDIT_TIME_UPDATE, &CBacnetSettingDyndns::OnEnKillfocusEditTimeUpdate)
END_MESSAGE_MAP()


// CBacnetSettingDyndns 消息处理程序



void CBacnetSettingDyndns::OnEnKillfocusEditDyndnsUserName()
{

    CString temp_cstring;
    m_dyndns_user.GetWindowTextW(temp_cstring);
    if (temp_cstring.GetLength() >= DYNDNS_MAX_USERNAME_SIZE)
    {
        MessageBox(_T("Nodes label length must between 0-32"), _T("Notice"), MB_OK | MB_ICONINFORMATION);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        return;
    }
    char temp_char[DYNDNS_MAX_USERNAME_SIZE + 1];
    memset(temp_char, 0, DYNDNS_MAX_USERNAME_SIZE + 1);
    WideCharToMultiByte(CP_ACP, NULL, temp_cstring.GetBuffer(), -1, temp_char, DYNDNS_MAX_USERNAME_SIZE, NULL, NULL);
    int n_ret = memcmp(temp_char, Device_Basic_Setting.reg.dyndns_user, DYNDNS_MAX_USERNAME_SIZE);
    if (n_ret == 0)
        return;
    memcpy(Device_Basic_Setting.reg.dyndns_user, temp_char, DYNDNS_MAX_USERNAME_SIZE);
    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Dyndns user name failed!"));
        memset(Device_Basic_Setting.reg.dyndns_user, 0, DYNDNS_MAX_USERNAME_SIZE);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Dyndns user name success!"));
    }
}


void CBacnetSettingDyndns::OnEnKillfocusEditDyndnsPassword()
{

    CString temp_cstring;
    m_dyndns_password.GetWindowTextW(temp_cstring);
    if (temp_cstring.GetLength() >= DYNDNS_MAX_PASSWORD_SIZE)
    {
        MessageBox(_T("Nodes label length must between 0-32"), _T("Notice"), MB_OK | MB_ICONINFORMATION);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        return;
    }
    char temp_char[DYNDNS_MAX_PASSWORD_SIZE + 1];
    memset(temp_char, 0, DYNDNS_MAX_PASSWORD_SIZE + 1);
    WideCharToMultiByte(CP_ACP, NULL, temp_cstring.GetBuffer(), -1, temp_char, DYNDNS_MAX_PASSWORD_SIZE, NULL, NULL);
    int n_ret = memcmp(temp_char, Device_Basic_Setting.reg.dyndns_pass, DYNDNS_MAX_PASSWORD_SIZE);
    if (n_ret == 0)
        return;
    memcpy(Device_Basic_Setting.reg.dyndns_pass, temp_char, DYNDNS_MAX_PASSWORD_SIZE);
    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Dyndns password failed!"));
        memset(Device_Basic_Setting.reg.dyndns_pass, 0, DYNDNS_MAX_PASSWORD_SIZE);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Dyndns password success!"));
    }
}


void CBacnetSettingDyndns::OnEnKillfocusEditDyndnsDomain()
{

    CString temp_cstring;
    m_dyndns_domain.GetWindowTextW(temp_cstring);
    if (temp_cstring.GetLength() >= DYNDNS_MAX_DOMAIN_SIZE)
    {
        MessageBox(_T("Nodes label length must between 0-32"), _T("Notice"), MB_OK | MB_ICONINFORMATION);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        return;
    }
    char temp_char[DYNDNS_MAX_DOMAIN_SIZE + 1];
    memset(temp_char, 0, DYNDNS_MAX_DOMAIN_SIZE + 1);
    WideCharToMultiByte(CP_ACP, NULL, temp_cstring.GetBuffer(), -1, temp_char, DYNDNS_MAX_DOMAIN_SIZE, NULL, NULL);
    int n_ret = memcmp(temp_char, Device_Basic_Setting.reg.dyndns_domain, DYNDNS_MAX_DOMAIN_SIZE);
    if (n_ret == 0)
        return;
    memcpy(Device_Basic_Setting.reg.dyndns_domain, temp_char, DYNDNS_MAX_DOMAIN_SIZE);
    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Dyndns domain failed!"));
        memset(Device_Basic_Setting.reg.dyndns_domain, 0, DYNDNS_MAX_DOMAIN_SIZE);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change dyndns domain success!"));
    }
}


void CBacnetSettingDyndns::OnBnClickedCheckSettingDyndns()
{



    CString temp_task_info;
    if (Device_Basic_Setting.reg.en_dyndns == 1)
    {
        Device_Basic_Setting.reg.en_dyndns = 2;
        temp_task_info.Format(_T("Enable Dyndns "));
    }
    else
    {
        Device_Basic_Setting.reg.en_dyndns = 1;
        temp_task_info.Format(_T("Disable Dyndns "));
    }

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);

}


void CBacnetSettingDyndns::OnCbnSelchangeComboBacnetSettingDdnsServer()
{

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->GetLBText(nSel, temp_string);
    if (nSel == 0)
        Device_Basic_Setting.reg.dyndns_provider = 0;
    else if (nSel == 1)
        Device_Basic_Setting.reg.dyndns_provider = 1;
    else if (nSel == 2)
        Device_Basic_Setting.reg.dyndns_provider = 2;
    else if (nSel == 3)
        Device_Basic_Setting.reg.dyndns_provider = 3;
    else if (nSel == 4)
        Device_Basic_Setting.reg.dyndns_provider = 4;

    CString temp_task_info;
    temp_task_info.Format(_T("Change DDNS server to "));
    temp_task_info = temp_task_info + temp_string;
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSettingDyndns::OnEnKillfocusEditTimeUpdate()
{

    CString temp_cstring;
    m_edit_ddns_update_time.GetWindowTextW(temp_cstring);
    int update_time = _wtoi(temp_cstring);


    if ((temp_cstring.IsEmpty()) || (update_time == 0) || (update_time >65535))
    {
        MessageBox(_T("value must between 0-65535"), _T("Notice"), MB_OK | MB_ICONINFORMATION);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        return;
    }

    Device_Basic_Setting.reg.dyndns_update_time = update_time;

    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change DDNS Time failed!"));
        Device_Basic_Setting.reg.dyndns_update_time = 0;
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change DDNS Time success!"));
    }

}


BOOL CBacnetSettingDyndns::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        int temp_focus_id = GetFocus()->GetDlgCtrlID();
        if (
            (temp_focus_id == IDC_EDIT_DYNDNS_USER_NAME) ||
            (temp_focus_id == IDC_EDIT_DYNDNS_PASSWORD) ||
            (temp_focus_id == IDC_EDIT_DYNDNS_DOMAIN) ||
            (temp_focus_id == IDC_EDIT_TIME_UPDATE))
        {

            GetDlgItem(IDC_EDIT_TIME_UPDATE)->SetFocus();
        }
        return 1;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
