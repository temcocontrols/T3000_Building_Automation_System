// BacnetSettingTcpip.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingTcpip.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "MainFrm.h"
#include "ping.h"
#include "ShowMessageDlg.h"
#include "WifiConfigDlg.h"
#include "BacnetSettingHealth.h"
// CBacnetSettingTcpip 对话框

IMPLEMENT_DYNAMIC(CBacnetSettingTcpip, CDialogEx)

CBacnetSettingTcpip::CBacnetSettingTcpip(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_SETTING_TCPIP, pParent)
{

}

CBacnetSettingTcpip::~CBacnetSettingTcpip()
{
}

void CBacnetSettingTcpip::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_COMBO_BACNET_SETTING_COM0, m_device_com0);
    DDX_Control(pDX, IDC_EDIT_SETTING_PORT, m_edit_port);

	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingTcpip, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM0, &CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingSubCom)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM1, &CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingCom1)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM2, &CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingMainCom)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE0, &CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingBaudrate0)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE1, &CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingBaudrate1)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE2, &CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingBaudrate2)
    ON_BN_CLICKED(IDC_RADIO_USB_DEVICE, &CBacnetSettingTcpip::OnBnClickedRadioUsbDevice)
    ON_BN_CLICKED(IDC_RADIO_USB_HOST, &CBacnetSettingTcpip::OnBnClickedRadioUsbHost)
    ON_BN_CLICKED(IDC_BUTTON_HEALTH, &CBacnetSettingTcpip::OnBnClickedButtonHealth)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_ZEIGBEE_PANID, &CBacnetSettingTcpip::OnEnKillfocusEditSettingZeigbeePanid)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PORT, &CBacnetSettingTcpip::OnEnKillfocusEditSettingPort)

    ON_BN_CLICKED(IDC_RADIO_BAC_IP_AUTO, &CBacnetSettingTcpip::OnBnClickedBtnBacIPAuto)
    ON_BN_CLICKED(IDC_RADIO_BAC_IP_STATIC, &CBacnetSettingTcpip::OnBnClickedBtnBacIPStatic)
    ON_BN_CLICKED(IDC_BUTTON_BAC_IP_CHANGED, &CBacnetSettingTcpip::OnBnClickedBtnBacIPChange)
    ON_BN_CLICKED(IDC_BUTTON_WIFI_CONFIGURATION, &CBacnetSettingTcpip::OnBnClickedButtonWifiConfiguration)
    ON_CBN_SELCHANGE(IDC_COMBO_PARITY_BIT, &CBacnetSettingTcpip::OnCbnSelchangeComboParityBit)
    ON_CBN_SELCHANGE(IDC_COMBO_PARITY_BIT3, &CBacnetSettingTcpip::OnCbnSelchangeComboParityBit3)
    ON_CBN_SELCHANGE(IDC_COMBO_STOP_BIT, &CBacnetSettingTcpip::OnCbnSelchangeComboStopBit)
    ON_CBN_SELCHANGE(IDC_COMBO_STOP_BIT3, &CBacnetSettingTcpip::OnCbnSelchangeComboStopBit3)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
END_MESSAGE_MAP()




LRESULT  CBacnetSettingTcpip::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
    _MessageInvokeIDInfo *pInvoke = (_MessageInvokeIDInfo *)lParam;
    bool msg_result = WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    CString Show_Results;
    CString temp_cs = pInvoke->task_info;
    if (msg_result)
    {
        Show_Results = temp_cs + _T("Success!");
        SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);

    }
    else
    {
        //memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//还原没有改对的值
        PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        Show_Results = temp_cs + _T("Fail!");
        SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);

    }

    if (pInvoke)
        delete pInvoke;
    return 0;
}


// CBacnetSettingTcpip 消息处理程序


void CBacnetSettingTcpip::OnBnClickedBtnBacIPAuto()
{
    UCHAR temp_data = 0;
    temp_data = Device_Basic_Setting.reg.tcp_type;
    Device_Basic_Setting.reg.tcp_type = 1;

    CString temp_task_info;
    temp_task_info.Format(_T("IP address has been changed! \r\nRebooting now! Please wait."));
    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        //更改失败恢复原状;
        Device_Basic_Setting.reg.tcp_type = temp_data;
        if (Device_Basic_Setting.reg.tcp_type == 0)
        {
            ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(false);
            ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(true);
        }
        else if (Device_Basic_Setting.reg.tcp_type == 1)
        {
            ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(true);
            ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(false);
        }
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
    }
    else
    {
        //更改成功
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);


        CShowMessageDlg dlg;
        CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
        dlg.SetStaticText(temp_task_info);
        //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
        dlg.SetStaticTextColor(RGB(0, 0, 255));
        dlg.SetStaticTextSize(25, 20);
        dlg.SetProgressAutoClose(300, 100, EVENT_IP_AUTO);
        dlg.SetHwnd(pFrame->m_hWnd, MY_RETRY_IP_CHANGE_MESSAGE);
        dlg.DoModal();
        refresh_tree_status_immediately = true;
    }

}

void CBacnetSettingTcpip::OnBnClickedBtnBacIPStatic()
{
    Device_Basic_Setting.reg.tcp_type = 0;
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
}

void CBacnetSettingTcpip::OnBnClickedBtnBacIPChange()
{

    BYTE address1, address2, address3, address4;
    BYTE subnet1, subnet2, subnet3, subnet4;
    BYTE gatway1, gatway2, gatway3, gatway4;
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->GetAddress(address1, address2, address3, address4);
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->GetAddress(subnet1, subnet2, subnet3, subnet4);
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->GetAddress(gatway1, gatway2, gatway3, gatway4);
    bool ip_change_flag = false;


    CString strIP;
    strIP.Format(_T("%u.%u.%u.%u"), address1, address2, address3, address4);
    CPing p1;
    CPingReply pr1;
    //如果是修改网关和子网掩码 就不用测试是否ping的通.
    if (address1 != Device_Basic_Setting.reg.ip_addr[0] || address2 != Device_Basic_Setting.reg.ip_addr[1] || address3 != Device_Basic_Setting.reg.ip_addr[2] || address4 != Device_Basic_Setting.reg.ip_addr[3])
    {
        if (p1.Ping1((LPCTSTR)strIP, pr1))
        {
            //如果ping 的通就说明要改的IP地址是存在的，有人在用的;

            MessageBox(_T("This IP address has been occupied"), _T("Warning"));
            return;
        }
    }



    Device_Basic_Setting.reg.ip_addr[0] = address1;
    Device_Basic_Setting.reg.ip_addr[1] = address2;
    Device_Basic_Setting.reg.ip_addr[2] = address3;
    Device_Basic_Setting.reg.ip_addr[3] = address4;
    Device_Basic_Setting.reg.subnet[0] = subnet1;
    Device_Basic_Setting.reg.subnet[1] = subnet2;
    Device_Basic_Setting.reg.subnet[2] = subnet3;
    Device_Basic_Setting.reg.subnet[3] = subnet4;
    Device_Basic_Setting.reg.gate_addr[0] = gatway1;
    Device_Basic_Setting.reg.gate_addr[1] = gatway2;
    Device_Basic_Setting.reg.gate_addr[2] = gatway3;
    Device_Basic_Setting.reg.gate_addr[3] = gatway4;
    bool isstatic = ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->GetCheck(); //返回1表示选上，0表示没选上;
    if (isstatic == true)
        Device_Basic_Setting.reg.tcp_type = 0;
    else
        Device_Basic_Setting.reg.tcp_type = 1;
    if (g_protocol == MODBUS_RS485)  //  若是Modbus 485 的协议
    {
        int test_value1 = 0;
        int test_value2 = 0;
        unsigned short write_buffer[200];
        memset(write_buffer, 0, 400);
        memcpy(write_buffer, &Device_Basic_Setting, sizeof(Str_Setting_Info));
        for (int j = 0;j < 200;j++)
        {
            write_buffer[j] = htons(write_buffer[j]);
        }
        test_value1 = Write_Multi_org_short(g_tstat_id, write_buffer, BAC_SETTING_START_REG, 100, 4);

        test_value2 = Write_Multi_org_short(g_tstat_id, &write_buffer[100], BAC_SETTING_START_REG + 100, 100, 4);
        if ((test_value1 >= 0) && (test_value2 >= 0))
        {
            ip_change_flag = true;
        }
    }
    else
    {
        if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
        {
            CString temp_task_info;
            temp_task_info.Format(_T("Change IP Address Information Timeout!"));
            MessageBox(temp_task_info);
        }
        else
        {
            ip_change_flag = true;

        }
    }

    if (ip_change_flag)
    {
        //在Ip 修改成功后 更新数据库;
        m_tcp_type = Device_Basic_Setting.reg.tcp_type;
        CString strnewipadress;
        strnewipadress.Format(_T("%u.%u.%u.%u"), address1, address2, address3, address4);
        CString temp_task_info;
        temp_task_info.Format(_T("IP address has been changed! \r\nRebooting now! Please wait."));

        CString strSql;
        CppSQLite3DB SqliteDBBuilding;
        CppSQLite3Table table;
        CppSQLite3Query q;
        SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

        CString temp_serial_cs;
        temp_serial_cs.Format(_T("%u"), g_selected_serialnumber);
        strSql.Format(_T("select * from ALL_NODE where Serial_ID = '%s' "), temp_serial_cs);
        //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
        q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
        while (!q.eof())
        {
            strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Serial_ID= '%s'"), strnewipadress, temp_serial_cs);
            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            q.nextRow();
        }

        refresh_tree_status_immediately = true;
        CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

        for (int i = 0; i < pFrame->m_product.size(); i++)
        {
            if (g_selected_serialnumber == pFrame->m_product.at(i).serial_number)
            {
                pFrame->m_product.at(i).BuildingInfo.strIp = strIP;
                break;
            }
        }
        if (g_protocol == MODBUS_RS485)
        {
            MessageBox(_T("IP Address configured successfully!"));
            return;
        }

        CShowMessageDlg dlg;

        dlg.SetStaticText(temp_task_info);
        //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
        dlg.SetStaticTextColor(RGB(0, 0, 255));
        dlg.SetStaticTextSize(25, 20);
        dlg.SetProgressAutoClose(250, 100, EVENT_IP_STATIC_CHANGE);
        dlg.SetChangedIPaddress(strIP);
        dlg.SetHwnd(pFrame->m_hWnd, MY_RETRY_IP_CHANGE_MESSAGE);
        dlg.DoModal();

        return;
    }

    //m_reboot_time_left = 10;
    //SetTimer(TIMER_IP_CHANGED_RECONNECT, 1000, NULL);

}




BOOL CBacnetSettingTcpip::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        return 1;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}



void CBacnetSettingTcpip::OnBnClickedButtonWifiConfiguration()
{
    // TODO: 在此添加控件通知处理程序代码

        CWifiConfigDlg dlg;
        dlg.SetUI_Type(UI_TYPE_WIFI_REV2);
        dlg.DoModal();
}



void CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingSubCom()
{
    unsigned char temp_com0_data = Device_Basic_Setting.reg.com0_config;
    UpdateData();
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->GetCurSel();
    m_device_com0.GetLBText(nSel, temp_string);


    for (int i = 0;i<MAX_COM_TYPE;i++)
    {
        if (temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0)
        {
            if (i == MAIN_MSTP)   //原由： 1 为 MSTP Slave     9为 MSTP Master   现在统统改为MSTP
                i = MSTP_MASTER;
            if (g_selected_product_id != PM_TSTAT10)
            {
                if (i == MSTP_MASTER)
                {
                    if ((Device_Basic_Setting.reg.com2_config == MAIN_MSTP) ||
                        (Device_Basic_Setting.reg.com2_config == MSTP_MASTER))
                    {
                        //想配置为mstp 必须其他的口设置为 空;
                        MessageBox(_T("With the current version of firmware , only one Bacnet MSTP subnet is supported, please select MSTP on only ONE of the RS485 ports"), _T("Message"));
                        ::PostMessage(m_setting_dlg_hwnd, WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);
                        return;
                    }
                }
                //else if ((i == MAIN_MODBUS) || (i == SUB_MODBUS))
                //{
                //    if ((Device_Basic_Setting.reg.com2_config == MSTP_MASTER) ||
                //        (Device_Basic_Setting.reg.com2_config == MAIN_MSTP))
                //    {
                //        MessageBox(_T("With the current version of firmware , only one Bacnet MSTP subnet is supported, please select MSTP on only ONE of the RS485 ports"), _T("Message"));
                //        ::PostMessage(m_setting_dlg_hwnd, WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);
                //        return;
                //    }
                //}
            }



            Device_Basic_Setting.reg.com0_config = i;
            break;
        }
    }
    if (temp_com0_data == Device_Basic_Setting.reg.com0_config)  // 若无变化，则退出;
        return;

    if ((g_protocol == MODBUS_RS485) ||
        (MODBUS_BACNET_MSTP == g_protocol))
    {

        bool modbus_0_bacnet_1 = 0;
        if ((Device_Basic_Setting.reg.com0_config == MAIN_MSTP) ||
            (Device_Basic_Setting.reg.com0_config == MSTP_MASTER))
        {
            modbus_0_bacnet_1 = 1;
        }
        else if ((Device_Basic_Setting.reg.com0_config == NOUSE) ||
            (Device_Basic_Setting.reg.com0_config == SUB_MODBUS) ||
            (Device_Basic_Setting.reg.com0_config == MAIN_MODBUS))
        {
            modbus_0_bacnet_1 = 0;
        }
        else
        {
            return;
        }
        if (g_selected_product_id == PM_TSTAT10)
        {
            //并且 TSTAT10 需要  重启 协议更改 才能生效;
            if (modbus_0_bacnet_1 == 1) //如果是要改到mstp协议
                Device_Basic_Setting.reg.com0_config = 1;  //1 是TSTAT10 的mstp协议;
            else
                Device_Basic_Setting.reg.com0_config = 2;  //2 是TSTAT10的modbus协议

        }
        ChangeDeviceProtocol(modbus_0_bacnet_1, Device_Basic_Setting.reg.modbus_id, 59, Device_Basic_Setting.reg.com0_config, 0, g_strCurBuildingDatabasefilePath);
        
        return;
    }
    Device_Basic_Setting.reg.reset_default = 150;
    CString temp_task_info;
    temp_task_info.Format(_T("Change serial port 0 "));
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    ClearSubNetInfo();
}


void CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingCom1()
{


    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->GetLBText(nSel, temp_string);
    for (int i = 0;i<MAX_COM_TYPE;i++)
    {
        if (temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0)
        {
            Device_Basic_Setting.reg.com1_config = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change serial port 1 "));
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    ClearSubNetInfo();
}


void CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingMainCom()
{
    unsigned char temp_main_value = Device_Basic_Setting.reg.com2_config;
    UpdateData();
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->GetLBText(nSel, temp_string);
    for (int i = 0;i<MAX_COM_TYPE;i++)
    {
        if (temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0)
        {
            if (i == MAIN_MSTP)   //原由： 1 为 MSTP Slave     9为 MSTP Master   现在统统改为MSTP
                i = MSTP_MASTER;
            if (i == MSTP_MASTER)
            {
                if ((Device_Basic_Setting.reg.com0_config == MSTP_MASTER) || (Device_Basic_Setting.reg.com0_config == MAIN_MSTP))
                {
                    //想配置为mstp 必须其他的口设置为 空;
                    MessageBox(_T("With the current version of firmware , only one Bacnet MSTP subnet is supported, please select MSTP on only ONE of the RS485 ports"), _T("Message"));
                    ::PostMessage(m_setting_dlg_hwnd, WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);
                    return;
                }
            }
            //else if ((i == MAIN_MODBUS) || (i == SUB_MODBUS))
            //{
            //    if ((Device_Basic_Setting.reg.com0_config == MSTP_MASTER) ||
            //        (Device_Basic_Setting.reg.com0_config == MAIN_MSTP))
            //    {
            //        MessageBox(_T("With the current version of firmware , only one Bacnet MSTP subnet is supported, please select MSTP on only ONE of the RS485 ports"), _T("Message"));
            //        ::PostMessage(m_setting_dlg_hwnd, WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);
            //        return;
            //    }
            //}

            Device_Basic_Setting.reg.com2_config = i;
            break;
        }
    }

    if (temp_main_value == Device_Basic_Setting.reg.com2_config)  // 若无变化，则退出;
        return;

    if ((g_protocol == MODBUS_RS485) ||
        (MODBUS_BACNET_MSTP == g_protocol))
    {

        bool modbus_0_bacnet_1 = 0;
        if ((Device_Basic_Setting.reg.com2_config == MAIN_MSTP) ||
            (Device_Basic_Setting.reg.com2_config == MSTP_MASTER))
        {
            modbus_0_bacnet_1 = 1;
        }
        else if ((Device_Basic_Setting.reg.com2_config == NOUSE) ||
            (Device_Basic_Setting.reg.com2_config == SUB_MODBUS) ||
            (Device_Basic_Setting.reg.com2_config == MAIN_MODBUS))
        {
            modbus_0_bacnet_1 = 0;
        }
        else
        {
            return;
        }
        //if (g_selected_product_id == PM_TSTAT10)
        //{
        //    //并且 TSTAT10 需要  重启 协议更改 才能生效;
        //    if (modbus_0_bacnet_1 == 1) //如果是要改到mstp协议
        //        Device_Basic_Setting.reg.com0_config = 1;  //1 是TSTAT10 的mstp协议;
        //    else
        //        Device_Basic_Setting.reg.com0_config = 2;  //2 是TSTAT10的modbus协议

        //}
        ChangeDeviceProtocol(modbus_0_bacnet_1, Device_Basic_Setting.reg.modbus_id, 61, Device_Basic_Setting.reg.com2_config, 0, g_strCurBuildingDatabasefilePath);

        return;
    }


#if 0    //暂时屏蔽     以后在客户升级许多次 后 在  禁止许 Asix 改;
    if (Device_Basic_Setting.reg.panel_type == PM_MINIPANEL)
    {
        if ((Device_Basic_Setting.reg.com2_config == 1) ||
            (Device_Basic_Setting.reg.com2_config == 9))
        {
            CBacnetRemotePortWarning Dlg;
            Dlg.SetWindowType(MESSAGE_ASIX_MAINPORT);
            Dlg.DoModal();
            return;
        }
    }
#endif
    Device_Basic_Setting.reg.reset_default = 150;
    CString temp_task_info;
    temp_task_info.Format(_T("Change serial port 2 "));
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    ClearSubNetInfo();
}

void CBacnetSettingTcpip::ClearSubNetInfo()
{
    CShowMessageDlg dlg;
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    dlg.SetStaticText(_T("Changing Subnet Baud Rate and Protocol\r\nMake sure all subnet devices share these same settings"));
    //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
    dlg.SetStaticTextColor(RGB(0, 0, 255));
    dlg.SetStaticTextSize(25, 20);
    dlg.SetProgressAutoClose(5000, 1, EVENT_WARNING_CHANGE_PROTOCOL_BAUDRATE);

    int ret_cusunits = 0;
    ret_cusunits = WritePrivateData_Blocking(g_bac_instance, WRITE_SETTING_COMMAND, 0, 0);
    if (ret_cusunits < 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Scanning RS485 Main  and RS485 Sub Failed !"));
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Ready to scan RS485 Main  and RS485 Sub port!"));
    }
    dlg.DoModal();
}

void CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingBaudrate0()
{

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]);i++)
    {
        if (temp_string.CompareNoCase(Baudrate_Array[i]) == 0)
        {
            Device_Basic_Setting.reg.com_baudrate0 = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change serial port 0 baudrate "));
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    ClearSubNetInfo();
}



void CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingBaudrate1()
{

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]);i++)
    {
        if (temp_string.CompareNoCase(Baudrate_Array[i]) == 0)
        {
            Device_Basic_Setting.reg.com_baudrate1 = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change serial port 1 baudrate "));
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    ClearSubNetInfo();
}


void CBacnetSettingTcpip::OnCbnSelchangeComboBacnetSettingBaudrate2()
{

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]);i++)
    {
        if (temp_string.CompareNoCase(Baudrate_Array[i]) == 0)
        {
            Device_Basic_Setting.reg.com_baudrate2 = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change serial port 2 baudrate "));
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    ClearSubNetInfo();
}



void CBacnetSettingTcpip::OnEnKillfocusEditSettingZeigbeePanid()
{
    // TODO: 在此添加控件通知处理程序代码

    CString temp_cstring;
    ((CEdit *)GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->GetWindowTextW(temp_cstring);
    int temp_panid = _wtoi(temp_cstring);
    if ((temp_panid >0) && (temp_panid <65535) && (temp_panid != Device_Basic_Setting.reg.zigbee_panid))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the zigbee pan ID to %u ?"), temp_panid);
        if (IDYES == MessageBox(temp_warning, _T("Notoce"), MB_YESNO))
        {
            unsigned short old_panid = Device_Basic_Setting.reg.zigbee_panid;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.zigbee_panid = (unsigned short)temp_panid;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change zigbee Pan id failed!"));
                Device_Basic_Setting.reg.zigbee_panid = old_panid;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change zigbee Pan id success!"));
            }
        }
    }
}




void CBacnetSettingTcpip::OnBnClickedRadioUsbHost()
{

    if (Device_Basic_Setting.reg.usb_mode != 1)
    {
        Device_Basic_Setting.reg.usb_mode = 1;
        CString temp_task_info;
        temp_task_info.Format(_T("Change USB feature (USB Host) "));
        Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    }

}

void CBacnetSettingTcpip::OnBnClickedButtonHealth()
{
    //#ifdef DEBUG
    //    ShutDownMstpGlobal(5);
    //    return;
    //#endif // DEBUG


    GetPrivateData_Blocking(g_bac_instance, READ_MISC, 0, 0, sizeof(Str_MISC));
    CBacnetSettingHealth Health_Dlg;
    Health_Dlg.DoModal();
}

void CBacnetSettingTcpip::OnBnClickedRadioUsbDevice()
{

    if (Device_Basic_Setting.reg.usb_mode != 0)
    {
        Device_Basic_Setting.reg.usb_mode = 0;
        CString temp_task_info;
        temp_task_info.Format(_T("Change USB feature (USB Device) "));
        Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    }

}


void CBacnetSettingTcpip::OnEnKillfocusEditSettingPort()
{

    CString temp_cstring;
    m_edit_port.GetWindowTextW(temp_cstring);
    int temp_port = _wtoi(temp_cstring);
    if ((temp_port >0) && (temp_port <65535) && (temp_port != Device_Basic_Setting.reg.modbus_port))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the modbus port to %u ?"), temp_port);
        if (IDYES == MessageBox(temp_warning, _T("Notoce"), MB_YESNO))
        {
            unsigned short old_port = Device_Basic_Setting.reg.modbus_port;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.modbus_port = (unsigned short)temp_port;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change port failed!"));
                Device_Basic_Setting.reg.modbus_port = old_port;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change port success!"));
            }
        }
    }
}





void CBacnetSettingTcpip::OnCbnSelchangeComboParityBit()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_PARITY_BIT))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_PARITY_BIT))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Com_Parity_bit) / sizeof(Com_Parity_bit[0]);i++)
    {
        if (temp_string.CompareNoCase(Com_Parity_bit[i]) == 0)
        {
            Device_Basic_Setting.reg.uart_parity[0] = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change RS485 SUB Port Parity Bit to %s"), temp_string);
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSettingTcpip::OnCbnSelchangeComboParityBit3()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_PARITY_BIT3))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_PARITY_BIT3))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Com_Parity_bit) / sizeof(Com_Parity_bit[0]);i++)
    {
        if (temp_string.CompareNoCase(Com_Parity_bit[i]) == 0)
        {
            Device_Basic_Setting.reg.uart_parity[2] = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change RS485 Main Port Parity Bit to %s"), temp_string);
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSettingTcpip::OnCbnSelchangeComboStopBit()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_STOP_BIT))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_STOP_BIT))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Com_Stop_bit) / sizeof(Com_Stop_bit[0]);i++)
    {
        if (temp_string.CompareNoCase(Com_Stop_bit[i]) == 0)
        {
            Device_Basic_Setting.reg.uart_stopbit[0] = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change RS485 SUB Port Stop Bit to %s"), temp_string);
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSettingTcpip::OnCbnSelchangeComboStopBit3()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_STOP_BIT3))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_STOP_BIT3))->GetLBText(nSel, temp_string);
    for (int i = 0;i<sizeof(Com_Stop_bit) / sizeof(Com_Stop_bit[0]);i++)
    {
        if (temp_string.CompareNoCase(Com_Stop_bit[i]) == 0)
        {
            Device_Basic_Setting.reg.uart_stopbit[2] = i;
            break;
        }
    }
    CString temp_task_info;
    temp_task_info.Format(_T("Change RS485 Main Port Stop Bit to %s"), temp_string);
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


BOOL CBacnetSettingTcpip::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if ((nID == IDC_COMBO_PARITY_BIT) ||
            (nID == IDC_COMBO_PARITY_BIT2) ||
            (nID == IDC_COMBO_PARITY_BIT3) ||
            (nID == IDC_COMBO_STOP_BIT) ||
            (nID == IDC_COMBO_STOP_BIT2) ||
            (nID == IDC_COMBO_STOP_BIT3))
        {
            //  这里就是你要显示的Tooltips，可以根据需要来定制
            CString strToolTips;
            strToolTips.Format(_T("Default parity bit : None.   Default stop bit : 1"));
            pTTT->lpszText = strToolTips.AllocSysString();

            pTTT->hinst = NULL;
            return(TRUE);
        }
    }

    return(FALSE);
}


BOOL CBacnetSettingTcpip::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    EnableToolTips(TRUE);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
