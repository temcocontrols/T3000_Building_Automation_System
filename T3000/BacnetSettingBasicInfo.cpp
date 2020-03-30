// BacnetSettingBasicInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingBasicInfo.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "BacnetSettingHealth.h"
#include "MainFrm.h"
// CBacnetSettingBasicInfo 对话框

IMPLEMENT_DYNAMIC(CBacnetSettingBasicInfo, CDialogEx)

CBacnetSettingBasicInfo::CBacnetSettingBasicInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_SETTING_BASIC, pParent)
{

}

CBacnetSettingBasicInfo::~CBacnetSettingBasicInfo()
{
}

void CBacnetSettingBasicInfo::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_EDIT_SETTING_OBJ_INSTANCE, m_setting_obj_instance);
    DDX_Control(pDX, IDC_EDIT_SETTING_MODBUS_ID, m_edit_modbus_id);



    DDX_Control(pDX, IDC_STATIC_SETTING_PANEL_TITLE, m_static_panel_title);
    DDX_Control(pDX, IDC_EDIT_SETTING_PANEL, m_edit_panel);

    DDX_Control(pDX, IDC_STATIC_SETTING_NODES_LABEL_TITLE, m_static_nodes_title);  //Panel Name  
    DDX_Control(pDX, IDC_EDIT_SETTING_NODES_LABEL_SETTING, m_edit_nodes_label);


	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingBasicInfo, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_OBJ_INSTANCE, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingObjInstance)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MODBUS_ID, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingModbusId)

    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_ON, &CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOn)
    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_OFF, &CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOff)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MAX_MASTER, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingMaxMaster)


    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PANEL, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingPanel)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_NODES_LABEL_SETTING, &CBacnetSettingBasicInfo::OnEnKillfocusEditSettingNodesLabelSetting)

END_MESSAGE_MAP()


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
        if (IDYES == MessageBox(temp_warning, _T("Notoce"), MB_YESNO))
        {
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
        if (IDYES == MessageBox(temp_warning, _T("Notoce"), MB_YESNO))
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
        if (IDYES == MessageBox(temp_warning, _T("Notoce"), MB_YESNO))
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

    Device_Basic_Setting.reg.LCD_Display = 1;

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSettingBasicInfo::OnBnClickedRadioSettingLcdOff()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Change LCD Background Light OFF "));

    Device_Basic_Setting.reg.LCD_Display = 0;

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
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
        if (IDYES == MessageBox(temp_warning, _T("Notoce"), MB_YESNO))
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
