// BacnetSettingBasicInfo.cpp : Implementation file
// BacnetSettingBasicInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingBasicInfo.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "BacnetSettingHealth.h"
#include "BacnetSettingParamter.h"
#include "MainFrm.h"
// CBacnetSettingBasicInfo dialog box
// CBacnetSettingBasicInfo 对话框

IMPLEMENT_DYNAMIC(CBacnetSettingBasicInfo, CDialogEx)

CBacnetSettingBasicInfo::CBacnetSettingBasicInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_SETTING_BASIC, pParent)
{

}

CBacnetSettingBasicInfo::~CBacnetSettingBasicInfo()
{
}


LRESULT  CBacnetSettingBasicInfo::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
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
        // Restore values that were not changed correctly
        //memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//还原没有改对的值
        // Call refresh thread here to re-refresh, which is more convenient
        PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        Show_Results = temp_cs + _T("Fail!");
        SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);

    }

    if (pInvoke)
        delete pInvoke;
    return 0;
}



void CBacnetSettingBasicInfo::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_EDIT_SETTING_OBJ_INSTANCE, m_setting_obj_instance);
    DDX_Control(pDX, IDC_EDIT_SETTING_MODBUS_ID, m_edit_modbus_id);



    DDX_Control(pDX, IDC_STATIC_SETTING_PANEL_TITLE, m_static_panel_title);
    DDX_Control(pDX, IDC_EDIT_SETTING_PANEL, m_edit_panel);

    DDX_Control(pDX, IDC_STATIC_SETTING_NODES_LABEL_TITLE, m_static_nodes_title);  //Panel Name  
    DDX_Control(pDX, IDC_EDIT_SETTING_NODES_LABEL_SETTING, m_edit_nodes_label);
    //DDX_Control(pDX, IDC_EDIT_SETTING_ALIAS_NAME, m_edit_zone_name);
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingBasicInfo, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_OBJ_INSTANCE, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingObjInstance)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MODBUS_ID, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingModbusId)

    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_ON, &CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOn)
    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_OFF, &CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOff)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MAX_MASTER, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingMaxMaster)
    ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)

    ON_EN_KILLFOCUS(IDC_EDIT_LCD_DELAY_OFF_TIME, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingLcdDelayOff)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PANEL, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingPanel)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_NODES_LABEL_SETTING, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingNodesLabelSetting)

    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_DELAY_OFF, &CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdDelayOff)
    ON_BN_CLICKED(IDC_BUTTON_LCD_SETTING, &CBacnetSettingBasicInfo::OnBnClickedButtonLcdSetting)
    //ON_EN_KILLFOCUS(IDC_EDIT_SETTING_ALIAS_NAME, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingAliasName)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_BIP_NETWORK2, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingBipNetwork2)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MSTP_NETWORK, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingMstpNetwork)
    ON_BN_CLICKED(IDC_BUTTON1, &CBacnetSettingBasicInfo::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBacnetSettingBasicInfo message handlers
// CBacnetSettingBasicInfo 消息处理程序

void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingObjInstance()
{

    CString temp_cstring;
    m_setting_obj_instance.GetWindowTextW(temp_cstring);
    unsigned int temp_obj_instance = unsigned int(_wtoi(temp_cstring));
    if ((temp_obj_instance >0) && (temp_obj_instance <= MAX_OBJ_INSTANCE) && (temp_obj_instance != Device_Basic_Setting.reg.object_instance))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the bacnet object instance to %u ?"), temp_obj_instance);
        if (IDYES == MessageBox(temp_warning, _T("Notice"), MB_YESNO))
        {
            // Save before writing; restore original value if write fails
            unsigned int old_object_instance = Device_Basic_Setting.reg.object_instance;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.object_instance = (unsigned int)temp_obj_instance;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change object instance failed!"));
                Device_Basic_Setting.reg.object_instance = old_object_instance;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                g_bac_instance = temp_obj_instance;
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change object instance success!"));
            }
        }
        else
        {
            PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        }
    }
}


void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingModbusId()
{

    CString temp_cstring;
    m_edit_modbus_id.GetWindowTextW(temp_cstring);
    unsigned int temp_modbusid = unsigned int(_wtoi(temp_cstring));
    if ((temp_modbusid<1) || (temp_modbusid >254))
    {
        MessageBox(_T("Invalid value."));
        return;
    }
    if ((temp_modbusid >0) && (temp_modbusid <= 254) && (temp_modbusid != Device_Basic_Setting.reg.modbus_id))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the modbus ID to %u ?"), temp_modbusid);
        if (IDYES == MessageBox(temp_warning, _T("Notice"), MB_YESNO))
        {
            unsigned char old_modbusid = Device_Basic_Setting.reg.modbus_id;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.modbus_id = (unsigned char)temp_modbusid;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Modbus RTU ID and Bacnet MSTP MAC ID failed!"));
                Device_Basic_Setting.reg.modbus_id = old_modbusid;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                ChangeModbusDB(Device_Basic_Setting.reg.n_serial_number, Device_Basic_Setting.reg.modbus_id, g_strCurBuildingDatabasefilePath);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Modbus RTU ID and Bacnet MSTP MAC ID success!"));
            }
        }
        else
        {
            PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        }
    }
}



void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingMaxMaster()
{
    // TODO: 在此添加控件通知处理程序代码

    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_SETTING_MAX_MASTER, temp_cstring);
    unsigned int temp_max_master = unsigned int(_wtoi(temp_cstring));
    if ((temp_max_master<1) || (temp_max_master >255))
    {
        MessageBox(_T("Invalid value."));
        return;
    }
    if ((temp_max_master >0) && (temp_max_master <= 254) && (temp_max_master != Device_Basic_Setting.reg.max_master))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the max master to %u ?"), temp_max_master);
        if (IDYES == MessageBox(temp_warning, _T("Notice"), MB_YESNO))
        {
            unsigned char old_max_master = Device_Basic_Setting.reg.max_master;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.max_master = (unsigned char)temp_max_master;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change max master failed!"));
                Device_Basic_Setting.reg.max_master = old_max_master;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change max master success!"));
            }
        }
        else
        {
            PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        }
    }
}


void CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOn()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Change LCD Background Light ON "));
    if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 519)//519
    {
        Device_Basic_Setting.reg.LCD_Display = 1;
    }
    else
    {
        GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(false);
        Device_Basic_Setting.reg.LCD_Display = 255;
    }
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);



}


void CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOff()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Change LCD Background Light OFF "));
    Device_Basic_Setting.reg.LCD_Display = 0;
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(false);
}

void CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdDelayOff()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Change LCD Background Light ON "));
    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_LCD_DELAY_OFF_TIME, temp_cstring);
    int n_delay_time;
    n_delay_time = (unsigned char)(_wtoi(temp_cstring));
    if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 519)//519
    {
        GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(true);
        Device_Basic_Setting.reg.LCD_Display = n_delay_time;
    }
    else
    {
        return;
    }
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingLcdDelayOff()
{
    OnBnClickedRadioSettingLcdDelayOff();
}

void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingPanel()
{

    CString temp_cstring;
    m_edit_panel.GetWindowTextW(temp_cstring);
    int temp_panel = _wtoi(temp_cstring);
    if ((temp_panel >0) && (temp_panel <255) && (temp_panel != Device_Basic_Setting.reg.panel_number))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the panel number to %u ?"), temp_panel);
        if (IDYES == MessageBox(temp_warning, _T("Notice"), MB_YESNO))
        {
            unsigned char old_panel = Device_Basic_Setting.reg.panel_number;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.panel_number = (unsigned char)temp_panel;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change panel number failed!"));
                Device_Basic_Setting.reg.panel_number = old_panel;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                Station_NUM = temp_panel;
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change panel number success!"));
            }
        }
    }
}


void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingNodesLabelSetting()
{

    CString temp_cstring;
    m_edit_nodes_label.GetWindowTextW(temp_cstring);
    if (temp_cstring.GetLength() >= 20)
    {
        MessageBox(_T("Nodes label length must between 0-20"), _T("Notice"), MB_OK | MB_ICONINFORMATION);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        return;
    }

    if (Device_Basic_Setting.reg.en_panel_name)
    {
        char temp_char[30];
        WideCharToMultiByte(CP_ACP, NULL, temp_cstring.GetBuffer(), -1, temp_char, 20, NULL, NULL);
        int n_ret = memcmp(temp_char, Device_Basic_Setting.reg.panel_name, 20);
        if (n_ret == 0)
            return;
        memcpy(Device_Basic_Setting.reg.panel_name, temp_char, 20);
        if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nodes label failed!"));
            memset(Device_Basic_Setting.reg.panel_name, 0, 20);
            PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        }
        else
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nodes label success!"));


            CppSQLite3DB SqliteDBBuilding;
            CppSQLite3Table table;
            CppSQLite3Query q;
            SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


            CString temp_serial;
            CString strSql;
            temp_serial.Format(_T("%u"), g_selected_serialnumber);
            strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"), temp_serial);

            q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
            //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

            while (!q.eof())
            {
                strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"), temp_cstring, temp_serial);
                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                q.nextRow();

            }

            SqliteDBBuilding.closedb();
            CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
            if (selected_product_index < pFrame->m_product.size())
            {
                pFrame->m_pTreeViewCrl->SetItemText(pFrame->m_product.at(selected_product_index).product_item, temp_cstring);
                pFrame->m_product.at(selected_product_index).NameShowOnTree = temp_cstring;
            }
        }
    }


}





BOOL CBacnetSettingBasicInfo::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        int temp_focus_id = GetFocus()->GetDlgCtrlID();
        if ((temp_focus_id == IDC_EDIT_SETTING_PANEL) ||
            (temp_focus_id == IDC_EDIT_SETTING_NODES_LABEL_SETTING) ||
            //(temp_focus_id == IDC_EDIT_SETTING_ALIAS_NAME) ||
            (temp_focus_id == IDC_EDIT_SETTING_OBJ_INSTANCE) )
        {
            GetDlgItem(IDC_STATIC_SEETING_SERIAL_NUMBER_2)->SetFocus();
        }
        return 1;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CBacnetSettingBasicInfo::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // TODO:  在此添加额外的初始化

    return false;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}



void CBacnetSettingBasicInfo::OnBnClickedButtonLcdSetting()
{
    // TODO: 在此添加控件通知处理程序代码
    CBacnetSettingParamter Dlg;
    Dlg.DoModal();
}

#if 0
void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingAliasName()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    m_edit_zone_name.GetWindowTextW(temp_cstring);
    if (temp_cstring.GetLength() > 10)
    {
        MessageBox(_T("Nodes label length must between 1-10"), _T("Notice"), MB_OK | MB_ICONINFORMATION);
        PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        return;
    }

    if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 600)
    {
        char temp_char[30];
        WideCharToMultiByte(CP_ACP, NULL, temp_cstring.GetBuffer(), -1, temp_char, 20, NULL, NULL);
        int n_ret = memcmp(temp_char, Device_Basic_Setting.reg.zone_name, 10);
        if (n_ret == 0)
            return;
        memcpy(Device_Basic_Setting.reg.zone_name, temp_char, 10);
        if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nodes label failed!"));
            memset(Device_Basic_Setting.reg.zone_name, 0, 10);
            PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
        }
        else
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nodes label success!"));
        }
    }


}
#endif

void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingBipNetwork2()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    ((CEdit*)GetDlgItem(IDC_EDIT_SETTING_BIP_NETWORK2))->GetWindowTextW(temp_cstring);
    unsigned int temp_bip_network = unsigned int(_wtoi(temp_cstring));
    if ((temp_bip_network > 0) && (temp_bip_network <= 65535) && (temp_bip_network != Device_Basic_Setting.reg.network_number_hi * 256 + Device_Basic_Setting.reg.network_number))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the bacnet network to %u ?"), temp_bip_network);
        if (IDYES == MessageBox(temp_warning, _T("Notice"), MB_YESNO))
        {
            unsigned int old_network = Device_Basic_Setting.reg.network_number_hi * 256 + Device_Basic_Setting.reg.network_number;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.network_number = (unsigned int)temp_bip_network%256;
            Device_Basic_Setting.reg.network_number_hi = (unsigned int)temp_bip_network / 256;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change bacnet network failed!"));
                Device_Basic_Setting.reg.network_number = old_network % 256;;
                Device_Basic_Setting.reg.network_number_hi = old_network / 256;;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {      
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change bacnet network success!"));
            }
        }
        else
        {
            PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        }
    }
}


void CBacnetSettingBasicInfo::OnEnKillfocusEditSettingMstpNetwork()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    ((CEdit*)GetDlgItem(IDC_EDIT_SETTING_MSTP_NETWORK))->GetWindowTextW(temp_cstring);
    unsigned int temp_mstp_network = unsigned int(_wtoi(temp_cstring));
    if ((temp_mstp_network >= 0) && (temp_mstp_network <= 65535) && (temp_mstp_network != Device_Basic_Setting.reg.mstp_network_number))
    {
        CString temp_warning;
        temp_warning.Format(_T("Do you really want to change the bacnet MSTP network number to %u ?"), temp_mstp_network);
        if (IDYES == MessageBox(temp_warning, _T("Notice"), MB_YESNO))
        {
            unsigned int old_mstp_network = Device_Basic_Setting.reg.mstp_network_number;	//写之前先保存起来；写失败 恢复原值;
            Device_Basic_Setting.reg.mstp_network_number = temp_mstp_network;
            if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change bacnet MSTP network number failed!"));
                Device_Basic_Setting.reg.mstp_network_number = old_mstp_network;
                PostMessage(WM_FRESH_CM_LIST, READ_SETTING_COMMAND, NULL);
            }
            else
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change bacnet MSTP network number success!"));
            }
        }
        else
        {
            PostMessage(WM_FRESH_SETTING_UI, READ_SETTING_COMMAND, NULL);//这里调用 刷新线程重新刷新会方便一点;
        }
    }
}
#if 0
static CString AnsiToCString(const char* s)
{
    if (!s) return CString();
    wchar_t buf[256] = { 0 };
    MultiByteToWideChar(CP_ACP, 0, s, -1, buf, _countof(buf));
    return CString(buf);
}

static void TraceInputPoint(int npanel_id, int input_idx)
{
    // 保护性检查，避免越界访问
    if (npanel_id < 0) return;
    extern std::vector<std::vector<Str_in_point>> g_Input_data; // 若项目中定义不同，请按实际声明调整
    try
    {
        // 访问元素（与代码中其它地方使用的语法一致）
        const Str_in_point& pt = g_Input_data.at(npanel_id).at(input_idx);

        CString desc = AnsiToCString((char*)pt.description);
        CString label = AnsiToCString((char*)pt.label);

        CString log;
        log.Format(_T("g_Input_data[%d][%d]: description=\"%s\", label=\"%s\", value=%d, filter=%d, decom=%d, sub_id=%d, sub_product=%d, control=%d, auto_manual=%d, digital_analog=%d, calibration_sign=%d, sub_number=%d,  calibration=%u, range=%u"),
            npanel_id, input_idx,
            (LPCTSTR)desc,
            (LPCTSTR)label,
            (int)pt.value,
            (int)pt.filter,
            (int)pt.decom,
            (int)pt.sub_id,
            (int)pt.sub_product,
            (int)pt.control,
            (int)pt.auto_manual,
            (int)pt.digital_analog,
            (int)pt.calibration_sign,
            (int)pt.sub_number,
            (unsigned int)pt.calibration_h,
            (unsigned int)pt.range
        );

        TRACE(_T("%s\r\n"), (LPCTSTR)log);
    }
    catch (...)
    {
        TRACE(_T("TraceInputPoint: invalid panel_id or input_idx (%d, %d)\r\n"), npanel_id, input_idx);
    }
}

void test1()
{
    int temp_panel_id = 144;// json.get("panelId", Json::nullValue).asInt();
    int temp_serial_number = 240488;// json.get("serialNumber", Json::nullValue).asInt();
    int entry_type = BAC_VAR;// json.get("entryType", Json::nullValue).asInt();
    int entry_index_start = 3;// json.get("entryIndexStart", Json::nullValue).asInt();
    int entry_index_end = 55;// json.get("entryIndexEnd", Json::nullValue).asInt();
    int entry_objectinstance = 240488;// json.get("objectinstance", Json::nullValue).asInt();
            // Log incoming request
            CString logMsg;
            logMsg.Format(_T("GET_WEBVIEW_LIST: PanelId=%d, SerialNumber=%d, EntryType=%d, StartIndex=%d, EndIndex=%d"),
                temp_panel_id, temp_serial_number, entry_type, entry_index_start, entry_index_end);


            if (g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number != temp_serial_number)
            {
                return;
            }
            unsigned int temp_objectinstance = g_Device_Basic_Setting[temp_panel_id].reg.object_instance;
            if ((temp_objectinstance == 0) ||
                (temp_objectinstance >= 0X3FFFFF) ||
                (temp_objectinstance != entry_objectinstance))
            {
                return;
            }

            // Add device main info to data array

            int point_idx = 0;
            switch (entry_type)
            {
            case BAC_IN:
            {
                if (entry_index_end >= entry_index_start)
                {
                    // 计算需要几个块（向上取整）
                    int totalCount = entry_index_end - entry_index_start + 1;
                    int groupSize = BAC_READ_INPUT_GROUP_NUMBER;
                    int read_input_group = (totalCount + groupSize - 1) / groupSize;

                    for (int i = 0; i < read_input_group; ++i)
                    {
                        int temp_start = entry_index_start + i * groupSize;
                        if (temp_start > entry_index_end) // 防止越界
                            break;

                        int temp_end = temp_start + groupSize - 1;
                        if (temp_end > entry_index_end)
                            temp_end = entry_index_end;

                        // 再次确保不超过全局最大点数
                        if (temp_start >= BAC_INPUT_ITEM_COUNT)
                            break;
                        if (temp_end >= BAC_INPUT_ITEM_COUNT)
                            temp_end = BAC_INPUT_ITEM_COUNT - 1;

                        // 示例：对每个块进行阻塞读取
                        if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READINPUT_T3000,
                            (uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_in_point), 4) > 0)
                        {
                            // 将读取到的临时数据复制到全局缓存
                            for (int idx = temp_start; idx <= temp_end; ++idx)
                            {
                                if (idx < BAC_INPUT_ITEM_COUNT) {
                                    g_Input_data[temp_panel_id].at(idx) = s_Input_data[idx];
                                }
                            }
                            if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
                            {
                                int elementCount = temp_end - temp_start + 1; // 本块实际元素个数
                                // 调试输出（可选）
                                CString dbg;
                                dbg.Format(_T("obj=%d,input ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), entry_objectinstance, i, temp_start, temp_end, elementCount);
                                DFTrace(dbg);
                            }
                            Sleep(SEND_COMMAND_DELAY_TIME);
                        }
                        else
                        {
                            CString errorLog;
                            errorLog.Format(_T("ERROR: Read input failed start=%d, end=%d"),
                                temp_start, temp_end);
                            continue;
                        }

                        int npanel_id = temp_panel_id;
                        for (int i = temp_start; i < temp_end; i++)
                        {
                            int input_idx = i;
                            TraceInputPoint(npanel_id, i);
                        }

                    } // for groups
                }
                break;
            }
            case BAC_OUT:
            {
                if (entry_index_end >= entry_index_start)
                {
                    // 计算需要几个块（向上取整）
                    int totalCount = entry_index_end - entry_index_start + 1;
                    int groupSize = BAC_READ_OUTPUT_GROUP_NUMBER;
                    int read_output_group = (totalCount + groupSize - 1) / groupSize;

                    for (int i = 0; i < read_output_group; ++i)
                    {
                        int temp_start = entry_index_start + i * groupSize;
                        if (temp_start > entry_index_end) // 防止越界
                            break;

                        int temp_end = temp_start + groupSize - 1;
                        if (temp_end > entry_index_end)
                            temp_end = entry_index_end;

                        // 再次确保不超过全局最大点数
                        if (temp_start >= BAC_OUTPUT_ITEM_COUNT)
                            break;
                        if (temp_end >= BAC_OUTPUT_ITEM_COUNT)
                            temp_end = BAC_OUTPUT_ITEM_COUNT - 1;

                        // 示例：对每个块进行阻塞读取
                        if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READOUTPUT_T3000,
                            (uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_out_point), 4) > 0)
                        {
                            // 将读取到的临时数据复制到全局缓存
                            for (int idx = temp_start; idx <= temp_end; ++idx)
                            {
                                if (idx < BAC_OUTPUT_ITEM_COUNT) {
                                    g_Output_data[temp_panel_id].at(idx) = s_Output_data[idx];
                                }
                            }
                            if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
                            {
                                int elementCount = temp_end - temp_start + 1; // 本块实际元素个数
                                // 调试输出（可选）
                                CString dbg;
                                dbg.Format(_T("obj=%d,output ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), entry_objectinstance, i, temp_start, temp_end, elementCount);
                                DFTrace(dbg);
                            }
                            Sleep(SEND_COMMAND_DELAY_TIME);
                        }
                        else
                        {
                            continue;
                        }

                        int npanel_id = temp_panel_id;
                        for (int i = temp_start; i < temp_end; i++)
                        {
                            int output_idx = i;
 point_idx++;
                        }
                    } // for groups
                }
                break;
            }
            case BAC_VAR:
            {
                if (entry_index_end >= entry_index_start)
                {
                    // 计算需要几个块（向上取整）
                    int totalCount = entry_index_end - entry_index_start + 1;
                    int groupSize = BAC_READ_VARIABLE_GROUP_NUMBER;
                    int read_variable_group = (totalCount + groupSize - 1) / groupSize;

                    for (int i = 0; i < read_variable_group; ++i)
                    {
                        int temp_start = entry_index_start + i * groupSize;
                        if (temp_start > entry_index_end) // 防止越界
                            break;

                        int temp_end = temp_start + groupSize - 1;
                        if (temp_end > entry_index_end)
                            temp_end = entry_index_end;

                        // 再次确保不超过全局最大点数
                        if (temp_start >= BAC_VARIABLE_ITEM_COUNT)
                            break;
                        if (temp_end >= BAC_VARIABLE_ITEM_COUNT)
                            temp_end = BAC_VARIABLE_ITEM_COUNT - 1;

                        // 示例：对每个块进行阻塞读取
                        if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READVARIABLE_T3000,
                            (uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_variable_point), 4) > 0)
                        {
                            // 将读取到的临时数据复制到全局缓存
                            for (int idx = temp_start; idx <= temp_end; ++idx)
                            {
                                if (idx < BAC_VARIABLE_ITEM_COUNT) {
                                    g_Variable_data[temp_panel_id].at(idx) = s_Variable_data[idx];
                                }
                            }
                            if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
                            {
                                int elementCount = temp_end - temp_start + 1; // 本块实际元素个数
                                // 调试输出（可选）
                                CString dbg;
                                dbg.Format(_T("obj=%d,variable ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), entry_objectinstance, i, temp_start, temp_end, elementCount);
                                DFTrace(dbg);
                            }
                            Sleep(SEND_COMMAND_DELAY_TIME);
                        }
                        else
                        {

                            continue;
                        }

                        int npanel_id = temp_panel_id;
                        for (int i = temp_start; i < temp_end; i++)
                        {
                            int var_idx = i;point_idx++;
                        }
                    } // for groups
                }
                break;
            }
            default:
                break;
            }

}
#endif
#include "BacnetSettingAdvParameter.h"
void CBacnetSettingBasicInfo::OnBnClickedButton1()
{
#if 0
    test1();
    return;
#endif
#if 0
    int entry_index_end = 45;
    int entry_index_start = 13;
    if (entry_index_end < entry_index_start)
    {
        // 无效范围，直接跳过
    }
    else {
        // 计算需要几个块（向上取整）
        int totalCount = entry_index_end - entry_index_start + 1;
        int groupSize = BAC_READ_INPUT_GROUP_NUMBER;
        int read_input_group = (totalCount + groupSize - 1) / groupSize;

        for (int i = 0; i < read_input_group; ++i)
        {
            int temp_start = entry_index_start + i * groupSize;
            if (temp_start > entry_index_end) // 防止越界
                break;

            int temp_end = temp_start + groupSize - 1;
            if (temp_end > entry_index_end)
                temp_end = entry_index_end;

            // 再次确保不超过全局最大点数
            if (temp_start >= BAC_INPUT_ITEM_COUNT)
                break;
            if (temp_end >= BAC_INPUT_ITEM_COUNT)
                temp_end = BAC_INPUT_ITEM_COUNT - 1;

            int elementCount = temp_end - temp_start + 1; // 本块实际元素个数

            // 调试输出（可选）
            CString dbg;
            dbg.Format(_T("Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), i, temp_start, temp_end, elementCount);
            TRACE(dbg);

        } // for groups
    }
#endif

    // TODO: 在此添加控件通知处理程序代码
    BacnetSettingAdvParameter dlg;
    dlg.DoModal();
}
