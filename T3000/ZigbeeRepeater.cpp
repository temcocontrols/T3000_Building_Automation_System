// ZigbeeRepeater.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "ZigbeeRepeater.h"
#include "global_function.h"
#include "MainFrm.h"
// CZigbeeRepeater
extern tree_product selected_product_Node; // 选中的设备信息;
IMPLEMENT_DYNCREATE(CZigbeeRepeater, CFormView)

CZigbeeRepeater::CZigbeeRepeater()
	: CFormView(IDD_DIALOG_ZIGBEE_REPEATER)
{

}

CZigbeeRepeater::~CZigbeeRepeater()
{
}

void CZigbeeRepeater::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_ZIGBEE_REPEATER, m_zigbee_repeater_list);
    DDX_Control(pDX, IDC_LIST_ZIGBEE_INPUT_MOUDLE, m_zigbee_input_list);
}

BEGIN_MESSAGE_MAP(CZigbeeRepeater, CFormView)
    ON_CBN_SELCHANGE(IDC_COMBO_REPEATER_BAUDRATE, &CZigbeeRepeater::OnCbnSelchangeComboRepeaterBaudrate)
    ON_CBN_SELCHANGE(IDC_COMBO_REPEATER_MODE, &CZigbeeRepeater::OnCbnSelchangeComboRepeaterMode)
    ON_EN_KILLFOCUS(IDC_EDIT_MODBUSID, &CZigbeeRepeater::OnEnKillfocusEditModbusid)
    ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL, &CZigbeeRepeater::OnEnKillfocusEditChannel)
    ON_EN_KILLFOCUS(IDC_EDIT_PANID, &CZigbeeRepeater::OnEnKillfocusEditPanid)
    ON_BN_CLICKED(IDC_BUTTON_ZIGBEE_REFRESH, &CZigbeeRepeater::OnBnClickedButtonZigbeeRefresh)
END_MESSAGE_MAP()


// CZigbeeRepeater 诊断

#ifdef _DEBUG
void CZigbeeRepeater::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CZigbeeRepeater::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CZigbeeRepeater 消息处理程序

void CZigbeeRepeater::Fresh()
{
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);
    Initial_List();
    Initial_Moudle_List();
    ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_BAUDRATE))->ResetContent();
    for (int i = 0; i < (sizeof(Repeater_Baudrate_Array) / sizeof(Repeater_Baudrate_Array[0])); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_BAUDRATE))->AddString(Repeater_Baudrate_Array[i]);
    }


    ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_MODE))->ResetContent();
    for (int i = 0; i < (sizeof(Repeater_Mode_Array) / sizeof(Repeater_Mode_Array[0])); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_MODE))->AddString(Repeater_Mode_Array[i]);
    }

    CString temp_baudrate;
    if (product_register_value[21] <= (sizeof(Repeater_Baudrate_Array) / sizeof(Repeater_Baudrate_Array[0])))
        temp_baudrate = Repeater_Baudrate_Array[product_register_value[21]];
    ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_BAUDRATE))->SetWindowTextW(temp_baudrate);

    CString temp_mode;
    if (product_register_value[23] <= (sizeof(Repeater_Mode_Array) / sizeof(Repeater_Mode_Array[0])))
        temp_mode = Repeater_Mode_Array[product_register_value[23]];
    ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_MODE))->SetWindowTextW(temp_mode);

    cs_serial_numer.Format(_T("%u"), product_register_value[3] * 256 * 256 * 256 + product_register_value[2] * 256 * 256 + product_register_value[1] * 256 + product_register_value[0]);
    GetDlgItem(IDC_STATIC_SERIALNUM)->SetWindowText(cs_serial_numer);

    CString cs_modbusid;
    cs_modbusid.Format(_T("%u"), product_register_value[6]);
    GetDlgItem(IDC_EDIT_MODBUSID)->SetWindowText(cs_modbusid);
    
    CString cs_channel;
    cs_channel.Format(_T("%u"), product_register_value[24]);
    GetDlgItem(IDC_EDIT_CHANNEL)->SetWindowText(cs_channel);

    CString cs_panid;
    cs_panid.Format(_T("%u"), product_register_value[22]);
    GetDlgItem(IDC_EDIT_PANID)->SetWindowText(cs_panid);

    GetDlgItem(IDC_BUTTON_OK)->SetFocus();
    
    bool add_new = false;
    m_zigbee_repeater_list.DeleteAllItems();

    int loop_count = product_register_value[300];

    for (int i = 301; i < 301 + loop_count; i++)
    {
        //Zigbee repeater 先读到ID 然后 在判断 数据库中是否存在，不存在就加入;
        if (product_register_value[i] != 0)
        {
            int temp_count = m_zigbee_repeater_list.GetItemCount();
            CString temp_cs;
            temp_cs.Format(_T("%d"), temp_count + 1);
            m_zigbee_repeater_list.InsertItem(temp_count, temp_cs);

            int temp_modbus_id = 0;
            int temp_rssi = 0;
            temp_modbus_id = product_register_value[i] >> 8;
            temp_rssi = product_register_value[i] & 0x00FF;
            CString cs_temp_modbusid;
            CString cs_temp_rssi;
            cs_temp_modbusid.Format(_T("%d"), temp_modbus_id);
            cs_temp_rssi.Format(_T("%d"), (char)temp_rssi);
            m_zigbee_repeater_list.SetItemText(temp_count, 1, cs_temp_modbusid);
            m_zigbee_repeater_list.SetItemText(temp_count, 2, cs_temp_rssi);

            unsigned short read_data[100] = {0};
            int nmultyRet = Read_Multi(temp_modbus_id, &read_data[0], 0, 32, 6);
            if (nmultyRet < 0)
                continue;

            CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
            CppSQLite3DB SqliteDBBuilding;
            SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
            unsigned int nSerialNumber;
            unsigned short nDevice_Type;
            nSerialNumber = read_data[0] + read_data[1] * 256 + read_data[2] * 256 * 256 + read_data[3] * 256 * 256 * 256;
            CString cs_subdevice_serialnum;
            cs_subdevice_serialnum.Format(_T("%u"), nSerialNumber);
            CppSQLite3Table m_table;
            CppSQLite3Query m_q;
            CString strSql;
            strSql.Format(_T("select * from ALL_NODE where Serial_ID = '%s'"), cs_subdevice_serialnum);
            m_q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
            m_table = SqliteDBBuilding.getTable((UTF8MBSTR)strSql);

            temp_count = m_table.numRows();
            CString temp_pid;
            _variant_t temp_variant;




            nDevice_Type = read_data[7];
            unsigned char modbus_id = 0;

            CString product_name;
            CString product_class_id;
            product_class_id.Format(_T("%d"), nDevice_Type);
            product_name = GetProductName(nDevice_Type);
            CString cs_baudrate;
            CString str_parents_serial;
            str_parents_serial = cs_serial_numer;
            CString  str_panel_number;
            CString str_object_instance;
            CString is_custom;
            CString str_hw_version;
            CString str_fw_version;
            CString str_n_port;
            CString  temp_pro4;
            temp_pro4 = _T("0");
            //selected_product_Node
            str_n_port.Format(_T("%d"), selected_product_Node.ncomport);

            if (temp_count > 0)
            {
                strSql.Format(_T("update ALL_NODE set  Product_class_ID = '%s',  Bautrate ='%s',Com_Port ='%s',Product_ID ='%s', Protocol ='0',Online_Status = 1,Parent_SerialNum = '%s' where Serial_ID = '%s'"), 
                    product_class_id, temp_baudrate, str_n_port, cs_temp_modbusid, str_parents_serial, cs_subdevice_serialnum);
            }
            else
                strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('" + pFrame->m_strCurMainBuildingName + "','" + pFrame->m_strCurSubBuldingName + "','" + cs_subdevice_serialnum + "','floor1','room1','" + product_name + "','" + product_class_id + "','" + cs_temp_modbusid + "','""','" + temp_baudrate + "','Modbus_and_Bacnet','" + str_hw_version + "','" + str_fw_version + "','" + str_n_port + "','0','" + temp_pro4 + "','1','" + str_parents_serial + "' ,'" + str_panel_number + "' ,'" + str_object_instance + "' ,'" + is_custom + "' )"));
           
            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

            SqliteDBBuilding.closedb();

            add_new = true;
        }

    }

    if (add_new)
    {
        CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
        ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
    }

    loop_count = product_register_value[52];
    for (int i = 0; i < loop_count; i++)
    {
        CString CS_ITEM;
        CString CS_MODBUSID;
        CString CS_VOLTAGE;
        CString CS_STATUS;
        CString CS_TEMPERATURE_1;
        CString CS_TEMPERATURE_2;
        CString CS_RSSI;
        //int temp_count = m_zigbee_input_list.GetItemCount();
        CS_ITEM.Format(_T("%d"), i + 1);
        m_zigbee_input_list.InsertItem(i, CS_ITEM);

        CS_MODBUSID.Format(_T("%d"), product_register_value[53 + i]);

        CS_VOLTAGE.Format(_T("%.1f"), ((float)product_register_value[53 + loop_count + i])/10);


        if (product_register_value[53 + 2 * loop_count + i] == 0)
        {
            CS_STATUS = Repeater_STATUS_Array[0];
        }
        else if (product_register_value[53 + 2 * loop_count + i] == 1)
        {
            CS_STATUS = Repeater_STATUS_Array[1];
        }
        else
        {
            CS_STATUS = Repeater_STATUS_Array[2];
        }

        //  65136  为 -40度
        if ((product_register_value[53 + loop_count * 3 + i] <= 65136) &&
            product_register_value[53 + loop_count * 3 + i] >= 1200)
        {
            CS_TEMPERATURE_1.Format(_T("No sensor"));
        }
        else
        {
            short temp_v1;
            temp_v1 = product_register_value[53 + loop_count * 3 + i];
            CString temp_int;
            int n_int;
            temp_int.Format(_T("%d"), temp_v1);
            n_int = _wtoi(temp_int);
            CS_TEMPERATURE_1.Format(_T("%.1f"),((float)n_int / 10));
        }
          
        if ((product_register_value[53 + loop_count * 4 + i] <= 65136) &&
            product_register_value[53 + loop_count * 4 + i] >= 1200)
        {
            CS_TEMPERATURE_2.Format(_T("No sensor"));
        }
        else
        {
            short temp_v1;
            temp_v1 = product_register_value[53 + loop_count * 4 + i];
            CString temp_int;
            int n_int;
            temp_int.Format(_T("%d"), temp_v1);
            n_int = _wtoi(temp_int);
            CS_TEMPERATURE_2.Format(_T("%.1f"), ((float)n_int / 10));
        }

        CS_RSSI.Format(_T("%d"), (char)product_register_value[53 + loop_count * 5 + i]);

        m_zigbee_input_list.SetItemText(i, ZIGBEE_MOUDLE_MODBUSID, CS_MODBUSID);
        m_zigbee_input_list.SetItemText(i, ZIGBEE_MOUDLE_VOLTAGE, CS_VOLTAGE);
        m_zigbee_input_list.SetItemText(i, ZIGBEE_MOUDLE_STATUS, CS_STATUS);
        m_zigbee_input_list.SetItemText(i, ZIGBEE_MOUDLE_TEMPERATURE_1, CS_TEMPERATURE_1);
        m_zigbee_input_list.SetItemText(i, ZIGBEE_MOUDLE_TEMPERATURE_2, CS_TEMPERATURE_2);
        m_zigbee_input_list.SetItemText(i, ZIGBEE_MOUDLE_RSSI, CS_RSSI);
    }

}

void CZigbeeRepeater::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    //Setting the refresh button,add by Fan
    HICON hIcon = NULL;
    HINSTANCE hInstResource = NULL;
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_REFRESH), RT_GROUP_ICON);
    hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_REFRESH), IMAGE_ICON, 32, 32, 0);
    ((CButton *)GetDlgItem(IDC_BUTTON_ZIGBEE_REFRESH))->SetIcon(hIcon);

    // TODO: 在此添加专用代码和/或调用基类
}



void CZigbeeRepeater::Initial_List()
{
    m_zigbee_repeater_list.DeleteAllItems();
    while (m_zigbee_repeater_list.DeleteColumn(0));

    m_zigbee_repeater_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    m_zigbee_repeater_list.SetExtendedStyle(m_zigbee_repeater_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_zigbee_repeater_list.InsertColumn(0, _T("Item"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_zigbee_repeater_list.InsertColumn(1, _T("Modbus ID"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_zigbee_repeater_list.InsertColumn(2, _T("RSSI (dBm)"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_zigbee_repeater_list.SetListHwnd(this->m_hWnd);
    //m_zigbee_repeater_list.SetWhetherShowBkCol(false);
    CRect list_rect, win_rect;
    m_zigbee_repeater_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);

    m_zigbee_repeater_list.DeleteAllItems();
}




void CZigbeeRepeater::Initial_Moudle_List()
{
    m_zigbee_input_list.DeleteAllItems();
    while (m_zigbee_input_list.DeleteColumn(0));

    m_zigbee_input_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    m_zigbee_input_list.SetExtendedStyle(m_zigbee_repeater_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_ITEM,    _T("Item"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_MODBUSID, _T("Modbus ID"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_VOLTAGE, _T("Voltage( V )"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_STATUS, _T("Status"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_TEMPERATURE_1, _T("Temperature 1 (Deg.C)"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_TEMPERATURE_2, _T("Temperature 2 (Deg.C)"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_zigbee_input_list.InsertColumn(ZIGBEE_MOUDLE_RSSI, _T("RSSI (dBm)"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_zigbee_input_list.SetListHwnd(this->m_hWnd);
    //m_zigbee_input_list.SetWhetherShowBkCol(false);
    CRect list_rect, win_rect;
    m_zigbee_input_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
    m_zigbee_repeater_list.DeleteAllItems();

}



void CZigbeeRepeater::OnCbnSelchangeComboRepeaterBaudrate()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_BAUDRATE))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_BAUDRATE))->GetLBText(nSel, temp_string);
    int temp_baudrate = product_register_value[21];
    for (int i = 0;i<sizeof(Repeater_Baudrate_Array) / sizeof(Repeater_Baudrate_Array[0]);i++)
    {
        if (temp_string.CompareNoCase(Repeater_Baudrate_Array[i]) == 0)
        {
            temp_baudrate = i;
            break;
        }
    }

    int nret = write_one(g_tstat_id, 21, temp_baudrate);
    if (nret < 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change baudrate failed!"));
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
        UpdateDeviceDB(NULL, Repeater_Baudrate_Array[temp_baudrate]);
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CZigbeeRepeater::OnCbnSelchangeComboRepeaterMode()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_MODE))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_REPEATER_MODE))->GetLBText(nSel, temp_string);
    int temp_baudrate = product_register_value[23];
    for (int i = 0;i<sizeof(Repeater_Mode_Array) / sizeof(Repeater_Mode_Array[0]);i++)
    {
        if (temp_string.CompareNoCase(Repeater_Mode_Array[i]) == 0)
        {
            temp_baudrate = i;
            break;
        }
    }

    int nret = write_one(g_tstat_id, 23, temp_baudrate);
    if (nret < 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change mode failed!"));
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CZigbeeRepeater::OnEnKillfocusEditModbusid()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_MODBUSID)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    int nret = write_one(g_tstat_id, 6, temp_int);
    if (nret < 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Modbus ID failed!"));
    }
    else
    {
        UpdateDeviceDB(temp_int, _T(""));
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CZigbeeRepeater::OnEnKillfocusEditChannel()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_CHANNEL)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 11) || (temp_int > 26))
    {
        MessageBox(_T("Invalid channel"));
        return;
    }
    int nret = write_one(g_tstat_id, 24, temp_int);
    if (nret < 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change channel failed!"));
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CZigbeeRepeater::OnEnKillfocusEditPanid()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_PANID)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);

    int nret = write_one(g_tstat_id, 22, temp_int);
    if (nret < 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change PAN ID failed!"));
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}

void CZigbeeRepeater::UpdateDeviceDB(int nmodbusid , CString cs_baudrate)
{
    CString cs_modbusid;
    cs_modbusid.Format(_T("%d"), nmodbusid);

    CppSQLite3DB SqliteDBBuilding;
    CppSQLite3Table table;
    CppSQLite3Query q;
    SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
    CString strSql;
    try
    {
        if (nmodbusid != 0)
        {
            strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"), cs_modbusid, cs_serial_numer);
            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
        }

        if (!cs_baudrate.IsEmpty())
        {
            strSql.Format(_T("update ALL_NODE set Bautrate ='%s' where Serial_ID = '%s'"), cs_baudrate, cs_serial_numer);
            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
        }

    }
    catch (_com_error *e)
    {
        AfxMessageBox(e->ErrorMessage());
    }
    SqliteDBBuilding.closedb();

    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
}


void CZigbeeRepeater::OnBnClickedButtonZigbeeRefresh()
{
    // TODO: 在此添加控件通知处理程序代码

    int i;
    int item_read = 0;
    float progress;
    for (i = 0; i < 4; i++)
    {
        int itemp = 0;
        itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 5);
        if (itemp < 0)
        {
            break;
        }
        else
        {
            progress = float((item_read + 1)*(100 / 4));
            g_progress_persent = progress;
        }
        item_read++;
        Sleep(100);
    }
    g_progress_persent = 0;
    Fresh();
}
