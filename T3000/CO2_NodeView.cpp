// CO2_NodeView.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2_NodeView.h"
#include "global_function.h"
#include "MainFrm.h"
#include "CO2_AUTO_CALIBRATION.h"
// CCO2_NodeView
extern tree_product selected_product_Node; // 选中的设备信息;
IMPLEMENT_DYNCREATE(CCO2_NodeView, CFormView)

int CO2_MODBUS_CO2_BKCAL_ONOFF = 500;
int CO2_MODBUS_CO2_NATURE_LEVEL = 501;
int CO2_MODBUS_CO2_MIN_ADJ = 502;
int CO2_MODBUS_CO2_CAL_DAYS = 503;
int CO2_MODBUS_CO2_LOWEST_X_DAYS = 0;
int CO2_MODBUS_CO2_FIGURE = 0;
int CO2_MODBUS_CO2_USER_ADJ = 0;

CCO2_NodeView::CCO2_NodeView()
	: CFormView(IDD_DIALOG_CO2_NODE)
{

}

CCO2_NodeView::~CCO2_NodeView()
{
}

void CCO2_NodeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CO2_NODE_LIST, m_co2_node_list);
}

BEGIN_MESSAGE_MAP(CCO2_NodeView, CFormView)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_CO2_Node_Item)
    ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_LIST, Fresh_CO2_Node_List)
    ON_EN_CHANGE(IDC_EDIT_CO2_NODE_ROLL_TIME_INTERVAL, &CCO2_NodeView::OnEnChangeEditCo2NodeRollTimeInterval)
    ON_MESSAGE(WM_CO2_NODE_THREAD_READ, UpdateUI)
    ON_EN_CHANGE(IDC_EDIT_CO2_NODE_MODBUS_ID, &CCO2_NodeView::OnEnChangeEditCo2NodeModbusId)
    ON_EN_CHANGE(IDC_EDIT_CO2_NODE_BACKLIGHT_TIME, &CCO2_NodeView::OnEnChangeEditCo2NodeBacklightTime)
    ON_CBN_SELCHANGE(IDC_COMBO_CO2_NODE_BAUDRATE, &CCO2_NodeView::OnCbnSelchangeComboCo2NodeBaudrate)
    ON_CBN_SELCHANGE(IDC_COMBO_CO2_NODE_PROTOCOL, &CCO2_NodeView::OnCbnSelchangeComboCo2NodeProtocol)
    ON_CBN_SELCHANGE(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT, &CCO2_NodeView::OnCbnSelchangeComboCo2NodeTemperatureUnit)
    ON_BN_CLICKED(IDC_RADIO1, &CCO2_NodeView::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &CCO2_NodeView::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_RADIO3, &CCO2_NodeView::OnBnClickedRadio3)
    ON_BN_CLICKED(IDC_RADIO4, &CCO2_NodeView::OnBnClickedRadio4)
    ON_BN_CLICKED(IDC_RADIO5, &CCO2_NodeView::OnBnClickedRadio5)
    ON_BN_CLICKED(IDC_RADIO6, &CCO2_NodeView::OnBnClickedRadio6)
    ON_BN_CLICKED(IDC_RADIO7, &CCO2_NodeView::OnBnClickedRadio7)
    ON_BN_CLICKED(IDC_RADIO8, &CCO2_NodeView::OnBnClickedRadio8)
    ON_BN_CLICKED(IDC_RADIO9, &CCO2_NodeView::OnBnClickedRadio9)
    ON_BN_CLICKED(IDC_RADIO10, &CCO2_NodeView::OnBnClickedRadio10)
    ON_BN_CLICKED(IDC_RADIO11, &CCO2_NodeView::OnBnClickedRadio11)
    ON_BN_CLICKED(IDC_RADIO12, &CCO2_NodeView::OnBnClickedRadio12)
    ON_BN_CLICKED(IDC_RADIO13, &CCO2_NodeView::OnBnClickedRadio13)
    ON_BN_CLICKED(IDC_RADIO14, &CCO2_NodeView::OnBnClickedRadio14)
    ON_BN_CLICKED(IDC_RADIO15, &CCO2_NodeView::OnBnClickedRadio15)
    ON_BN_CLICKED(IDC_RADIO16, &CCO2_NodeView::OnBnClickedRadio16)
    ON_BN_CLICKED(IDC_BUTTON_OK, &CCO2_NodeView::OnBnClickedButtonOk)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_NODE_MODBUS_ID, &CCO2_NodeView::OnEnKillfocusEditCo2NodeModbusId)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_NODE_ROLL_TIME_INTERVAL, &CCO2_NodeView::OnEnKillfocusEditCo2NodeRollTimeInterval)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_NODE_BACKLIGHT_TIME, &CCO2_NodeView::OnEnKillfocusEditCo2NodeBacklightTime)
    ON_EN_KILLFOCUS(IDC_EDIT_FAIR_ALARM_SETPOINT, &CCO2_NodeView::OnEnKillfocusEditFairAlarmSetpoint)
    ON_EN_KILLFOCUS(IDC_EDIT_POOR_ALARM_SETPOINT, &CCO2_NodeView::OnEnKillfocusEditPoorAlarmSetpoint)
    ON_BN_CLICKED(IDC_RADIO_SCROLL1, &CCO2_NodeView::OnBnClickedRadioScroll1)
    ON_BN_CLICKED(IDC_RADIO_SCROLL2, &CCO2_NodeView::OnBnClickedRadioScroll2)
    ON_BN_CLICKED(IDC_RADIO_SCROLL3, &CCO2_NodeView::OnBnClickedRadioScroll3)
    ON_BN_CLICKED(IDC_RADIO_SCROLL4, &CCO2_NodeView::OnBnClickedRadioScroll4)
    ON_BN_CLICKED(IDC_RADIO_SCROLL5, &CCO2_NodeView::OnBnClickedRadioScroll5)
    ON_BN_CLICKED(IDC_RADIO_SCROLL6, &CCO2_NodeView::OnBnClickedRadioScroll6)
    ON_BN_CLICKED(IDC_RADIO_SCROLL7, &CCO2_NodeView::OnBnClickedRadioScroll7)
    ON_BN_CLICKED(IDC_RADIO_SCROLL8, &CCO2_NodeView::OnBnClickedRadioScroll8)
    ON_BN_CLICKED(IDC_RADIO_SCROLL9, &CCO2_NodeView::OnBnClickedRadioScroll9)
    ON_BN_CLICKED(IDC_RADIO_SCROLL10, &CCO2_NodeView::OnBnClickedRadioScroll10)
    ON_BN_CLICKED(IDC_RADIO_SCROLL11, &CCO2_NodeView::OnBnClickedRadioScroll11)
    ON_BN_CLICKED(IDC_RADIO_SCROLL12, &CCO2_NodeView::OnBnClickedRadioScroll12)
    ON_BN_CLICKED(IDC_RADIO_SCROLL13, &CCO2_NodeView::OnBnClickedRadioScroll13)
    ON_BN_CLICKED(IDC_RADIO_SCROLL14, &CCO2_NodeView::OnBnClickedRadioScroll14)
    ON_BN_CLICKED(IDC_RADIO_SCROLL15, &CCO2_NodeView::OnBnClickedRadioScroll15)
    ON_BN_CLICKED(IDC_RADIO_SCROLL16, &CCO2_NodeView::OnBnClickedRadioScroll16)
    ON_BN_CLICKED(IDC_RADIO_SCROLL17, &CCO2_NodeView::OnBnClickedRadioScroll17)
    ON_BN_CLICKED(IDC_RADIO_SCROLL18, &CCO2_NodeView::OnBnClickedRadioScroll18)
    ON_BN_CLICKED(IDC_RADIO_SCROLL19, &CCO2_NodeView::OnBnClickedRadioScroll19)
    ON_BN_CLICKED(IDC_RADIO_SCROLL20, &CCO2_NodeView::OnBnClickedRadioScroll20)
    ON_BN_CLICKED(IDC_RADIO_SCROLL21, &CCO2_NodeView::OnBnClickedRadioScroll21)
    ON_BN_CLICKED(IDC_RADIO_SCROLL22, &CCO2_NodeView::OnBnClickedRadioScroll22)
    ON_EN_KILLFOCUS(IDC_EDIT_DLG_NATURE_CO2, &CCO2_NodeView::OnEnKillfocusEditNatureCo2)
    ON_EN_KILLFOCUS(IDC_EDIT_DLG_MAX_MIN_ADJ_PERDAY, &CCO2_NodeView::OnEnKillfocusEditMaxMinAdjPerday)
    ON_EN_KILLFOCUS(IDC_EDIT_DLG_LOOK_DAYS, &CCO2_NodeView::OnEnKillfocusEditLookDays)
    ON_CBN_SELCHANGE(IDC_COMBO_DLG_CO2_AUTO_CAL, &CCO2_NodeView::OnCbnSelchangeComboCo2AutoCal)
END_MESSAGE_MAP()


// CCO2_NodeView 诊断

#ifdef _DEBUG
void CCO2_NodeView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCO2_NodeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

HANDLE h_co2_node_thread = NULL;
// CCO2_NodeView 消息处理程序
void CCO2_NodeView::Fresh()
{
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);

    CO2_MODBUS_CO2_BKCAL_ONOFF = 500;
    CO2_MODBUS_CO2_NATURE_LEVEL = 501;
    CO2_MODBUS_CO2_MIN_ADJ = 502;
    CO2_MODBUS_CO2_CAL_DAYS = 503;

    if (h_co2_node_thread == NULL)
    {
        h_co2_node_thread = CreateThread(NULL, NULL, UpdateCO2_Note_Thread, this, NULL, NULL);
        CloseHandle(h_co2_node_thread);
    }
    UpdateUI(NULL,NULL);
}

LRESULT CCO2_NodeView::UpdateUI(WPARAM wParam, LPARAM lParam)
{
    Fresh_CO2_Node_List(NULL, NULL);
    Update_Static_UI();
    return 1;
}


DWORD WINAPI UpdateCO2_Note_Thread(LPVOID lPvoid)
{
    CCO2_NodeView * mparent = (CCO2_NodeView *)lPvoid;
    //while (mparent->IsWindowVisible())
    //{
        for (int i = 0; i < 5; i++)
        {
            int itemp = 0;
            itemp = Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100, 5);
        }
        PostMessage(mparent->m_hWnd, WM_CO2_NODE_THREAD_READ, NULL, NULL);
        Sleep(15000);
    //}


    h_co2_node_thread = NULL;
    return 1;
}





const CString CO2_Node_Baudrate_Array[] =
{
    _T("9600"),
    _T("19200"), //6
    _T("38400"),
    _T("57600"),
    _T("115200"),	//9
    _T("76800")
};

const CString CO2_Node_Protocol[] =
{
    _T("Bacnet MSTP"),  //0
    _T("Modbus RS485")  // 3
};


void CCO2_NodeView::Initial_List()
{
    m_co2_node_list.ShowWindow(SW_HIDE);
    m_co2_node_list.DeleteAllItems();
    while (m_co2_node_list.DeleteColumn(0));

    m_co2_node_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    m_co2_node_list.SetExtendedStyle(m_co2_node_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_co2_node_list.InsertColumn(CO2_NODE_TYPE_NAME, _T("Name"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_co2_node_list.InsertColumn(CO2_NODE_NAME_UNIT, _T("Unit"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_co2_node_list.InsertColumn(CO2_NODE_TYPE_VALUE, _T("Value"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.InsertColumn(CO2_NODE_SETPOINT, _T("Setpoint"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.InsertColumn(CO2_NODE_OUTPUT_VALUE, _T("Output Value"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.InsertColumn(CO2_NODE_OUTPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.InsertColumn(CO2_NODE_MIN_VALUE , _T("Output Min Value"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.InsertColumn(CO2_NODE_MAX_VALUE , _T("Output Max Value"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.InsertColumn(CO2_NODE_CALIBRATION, _T("Calibration"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_co2_node_list.SetListHwnd(this->m_hWnd);
    CRect list_rect, win_rect;
    m_co2_node_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);

    m_co2_node_list.DeleteAllItems();

    m_co2_node_list.InsertItem(0, _T("CO2"));
    m_co2_node_list.InsertItem(1, _T("Temperature"));
    m_co2_node_list.InsertItem(2, _T("Humidity"));

    m_co2_node_list.ShowWindow(SW_SHOW);


    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_BAUDRATE))->ResetContent();
    for (int i = 0; i < (sizeof(CO2_Node_Baudrate_Array) / sizeof(CO2_Node_Baudrate_Array[0])); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_BAUDRATE))->AddString(CO2_Node_Baudrate_Array[i]);
    }
   
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_AUTO_CAL))->ResetContent();
    for (int i = 0; i < (sizeof(CO2_Node_Auto_Cal) / sizeof(CO2_Node_Auto_Cal[0])); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_AUTO_CAL))->AddString(CO2_Node_Auto_Cal[i]);
    }
    

    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_PROTOCOL))->ResetContent();
    for (int i = 0; i < (sizeof(CO2_Node_Protocol) / sizeof(CO2_Node_Protocol[0])); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_PROTOCOL))->AddString(CO2_Node_Protocol[i]);
    }


    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->ResetContent();
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->AddString(_T("Deg.C"));
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->AddString(_T("Deg.F"));
}



void CCO2_NodeView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // TODO: 在此添加专用代码和/或调用基类
    Initial_List();
    
}

LRESULT CCO2_NodeView::Fresh_CO2_Node_Item(WPARAM wParam, LPARAM lParam)
{
    int cmp_ret;//compare if match it will 0;
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
    int nret = 0;
    int int_value = 0;
    CString temp_task_info;
    CString New_CString = m_co2_node_list.GetItemText(Changed_Item, Changed_SubItem);
    if (Changed_SubItem == CO2_NODE_TYPE_VALUE)
    {
        int nreg = 0;
        if (Changed_Item == ROW_CO2)
        {
            int_value = _wtoi(New_CString);
            nret = write_one(g_tstat_id, 108, int_value);
        }
        else if (Changed_Item == ROW_TEMPERATURE)
        {
            float nvalue = _wtof(New_CString);
            int nwritevalue = (int)(nvalue * 10);
            //if (product_register_value[124] == 0)
            //{
            //    if (product_register_value[125] == 0)
            //    {
            //        nreg = 121;
            //    }
            //    else
            //    {
            //        nreg = 122;
            //    }
            //}
            //else
            //{
                if (product_register_value[125] == 0)
                {
                    nreg = 119;
                }
                else
                {
                    nreg = 120;
                }
            //}
            nret = write_one(g_tstat_id, nreg, nwritevalue);
        }
        else if (Changed_Item == ROW_HUMIDITY)
        {
            float nvalue = _wtof(New_CString);
            int nwritevalue = (int)(nvalue * 10);
            nret = write_one(g_tstat_id, 116, nwritevalue);
        }
    }
    else if (Changed_SubItem == CO2_NODE_SETPOINT)
    {
        if (Changed_Item == ROW_CO2)
        {
            int_value = _wtoi(New_CString);
            nret = write_one(g_tstat_id, 166, int_value);
        }
        else if (Changed_Item == ROW_TEMPERATURE)
        {
            float nvalue = _wtof(New_CString);
            int nwritevalue = (int)(nvalue * 10);
            nret = write_one(g_tstat_id, 167, nwritevalue);
        }
        else if (Changed_Item == ROW_HUMIDITY)
        {
            float nvalue = _wtof(New_CString);
            int nwritevalue = (int)(nvalue * 10);
            nret = write_one(g_tstat_id, 168, nwritevalue);
        }
    }
    else if (Changed_SubItem == CO2_NODE_MIN_VALUE)
    {
        if (Changed_Item == ROW_CO2)
        {
            int_value = _wtoi(New_CString);
            nret = write_one(g_tstat_id, 132, int_value);
        }
        else if (Changed_Item == ROW_TEMPERATURE)
        {
            int_value = (unsigned short)_wtoi(New_CString);
            nret = write_one(g_tstat_id, 128, int_value);
        }
        else if (Changed_Item == ROW_HUMIDITY)
        {
            int_value = _wtoi(New_CString);
            nret = write_one(g_tstat_id, 130, int_value);
        }
    }
    else if (Changed_SubItem == CO2_NODE_MAX_VALUE)
    {
        if (Changed_Item == ROW_CO2)
        {
            int_value = _wtoi(New_CString);
            nret = write_one(g_tstat_id, 133, int_value);
        }
        else if (Changed_Item == ROW_TEMPERATURE)
        {
            int_value = (unsigned short)_wtoi(New_CString);
            nret = write_one(g_tstat_id, 129, int_value);
        }
        else if (Changed_Item == ROW_HUMIDITY)
        {
            int_value = _wtoi(New_CString);
            nret = write_one(g_tstat_id, 131, int_value);
        }
    }
    else if (Changed_SubItem == CO2_NODE_CALIBRATION)
    {
        if (product_register_value[127] == 1)
        {
            if (Changed_Item == ROW_CO2)
            {
                int_value = _wtoi(New_CString);
                nret = write_one(g_tstat_id, 147, int_value);
            }
            else if (Changed_Item == ROW_TEMPERATURE)
            {
                int_value = _wtoi(New_CString);
                nret = write_one(g_tstat_id, 148, int_value);
            }
            else if (Changed_Item == ROW_HUMIDITY)
            {
                int_value = _wtoi(New_CString);
                nret = write_one(g_tstat_id, 149, int_value);
            }
        }
        else if ((product_register_value[127] == 2) || (product_register_value[127] == 3))
        {
            if (Changed_Item == ROW_CO2)
            {
                int_value = _wtoi(New_CString);
                nret = write_one(g_tstat_id, 144, int_value);
            }
            else if (Changed_Item == ROW_TEMPERATURE)
            {
                int_value = _wtoi(New_CString);
                nret = write_one(g_tstat_id, 145, int_value);
            }
            else if (Changed_Item == ROW_HUMIDITY)
            {
                int_value = _wtoi(New_CString);
                nret = write_one(g_tstat_id, 146, int_value);
            }
        }
    }



    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    return 1;
}

void CCO2_NodeView::Update_Static_UI()
{
    CString cs_serial_numer;
    cs_serial_numer.Format(_T("%u"), product_register_value[3]*256*256*256 + product_register_value[2] *256*256 + product_register_value[1] *256 + product_register_value[0]);
    GetDlgItem(IDC_EDIT_CO2_NODE_SERIALNUMBER)->SetWindowText(cs_serial_numer);

    CString cs_modbusid;
    cs_modbusid.Format(_T("%d"), product_register_value[6]);
    GetDlgItem(IDC_EDIT_CO2_NODE_MODBUS_ID)->SetWindowText(cs_modbusid);

    CString cs_temp;
    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_NATURE_LEVEL]);
    GetDlgItem(IDC_EDIT_DLG_NATURE_CO2)->SetWindowText(cs_temp);

    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_MIN_ADJ]);
    GetDlgItem(IDC_EDIT_DLG_MAX_MIN_ADJ_PERDAY)->SetWindowText(cs_temp);

    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_CAL_DAYS]);
    GetDlgItem(IDC_EDIT_DLG_LOOK_DAYS)->SetWindowText(cs_temp);


    if (product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF] <= (sizeof(CO2_Node_Auto_Cal) / sizeof(CO2_Node_Auto_Cal[0])))
        cs_temp = CO2_Node_Auto_Cal[product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF]];
    else
        cs_temp.Empty();
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_AUTO_CAL))->SetWindowTextW(cs_temp);

    if (product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF] == 1)
    {
        GetDlgItem(IDC_EDIT_DLG_NATURE_CO2)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_DLG_MAX_MIN_ADJ_PERDAY)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_DLG_LOOK_DAYS)->EnableWindow(false);
    }
    else
    {
        GetDlgItem(IDC_EDIT_DLG_NATURE_CO2)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_DLG_MAX_MIN_ADJ_PERDAY)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_DLG_LOOK_DAYS)->EnableWindow(true);
    }

    CString temp_baudrate;
    if(product_register_value[15] <= (sizeof(CO2_Node_Baudrate_Array) / sizeof(CO2_Node_Baudrate_Array[0])))
        temp_baudrate = CO2_Node_Baudrate_Array[product_register_value[15]];
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_BAUDRATE))->SetWindowTextW(temp_baudrate);

    CString temp_protocol;
    if (product_register_value[21] == 0)
        temp_protocol = CO2_Node_Protocol[0];
    else if(product_register_value[21] == 3)
    {
        temp_protocol = CO2_Node_Protocol[1];
    }
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_PROTOCOL))->SetWindowTextW(temp_protocol);

    CString temp_roll_time;
    temp_roll_time.Format(_T("%d"), product_register_value[171]);
    GetDlgItem(IDC_EDIT_CO2_NODE_ROLL_TIME_INTERVAL)->SetWindowText(temp_roll_time);

    CString temp_backlight;
    temp_backlight.Format(_T("%d"), product_register_value[158]);
    GetDlgItem(IDC_EDIT_CO2_NODE_BACKLIGHT_TIME)->SetWindowText(temp_backlight);


    if (product_register_value[125] == 0)
        ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->SetWindowTextW(_T("Deg.C"));
    else if (product_register_value[125] == 1)
        ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->SetWindowTextW(_T("Deg.F"));

    CString cs_fair_ppm_setpoint;
    CString cs_poor_ppm_setpoint;
    cs_fair_ppm_setpoint.Format(_T("%d"), product_register_value[112]);
    cs_poor_ppm_setpoint.Format(_T("%d"), product_register_value[113]);
    GetDlgItem(IDC_EDIT_FAIR_ALARM_SETPOINT)->SetWindowText(cs_fair_ppm_setpoint);
    GetDlgItem(IDC_EDIT_POOR_ALARM_SETPOINT)->SetWindowText(cs_poor_ppm_setpoint);

    Show_Main_radio();
    Show_Scroll_radio();
}

LRESULT CCO2_NodeView::Fresh_CO2_Node_List(WPARAM wParam, LPARAM lParam)
{
    int Fresh_Item;
    int isFreshOne = (int)lParam;

    Fresh_Item = (int)wParam;

    CString cs_co2_value;
    CString cs_temp_value;
    CString cs_Hum_value;
    cs_co2_value.Format(_T("%u"), product_register_value[108]);

    //125寄存器 为0 则为C   1为F
    //if (product_register_value[124] == 0)
    //{
    //    if (product_register_value[125] == 0)
    //    {
    //        cs_temp_value.Format(_T("%.1f"), ((float)product_register_value[121]) / 10);
    //    }
    //    else
    //    {
    //        cs_temp_value.Format(_T("%.1f"), ((float)product_register_value[122]) / 10);
    //    }
    //}
    //else
    //{
        if (product_register_value[125] == 0)
        {
            cs_temp_value.Format(_T("%.1f"), ((float)product_register_value[119]) / 10);
        }
        else
        {
            cs_temp_value.Format(_T("%.1f"), ((float)product_register_value[120]) / 10);
        }
    //}

    cs_Hum_value.Format(_T("%.1f"), ((float)product_register_value[116]) / 10);

    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_TYPE_VALUE, cs_co2_value);
    m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_TYPE_VALUE, cs_temp_value);
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_TYPE_VALUE, cs_Hum_value);

    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_NAME_UNIT, _T("ppm"));
    if (product_register_value[125] == 0)
        m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_NAME_UNIT, _T("Deg.C"));
    else if (product_register_value[125] == 1)
        m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_NAME_UNIT, _T("Deg.F"));
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_NAME_UNIT, _T("%"));


    CString cs_co2_setpoint;
    CString cs_temp_setpoint;
    CString cs_hum_setpoint;
    cs_co2_setpoint.Format(_T("%d"), product_register_value[166]);
    cs_temp_setpoint.Format(_T("%.1f"), ((float)product_register_value[167]) / 10);
    cs_hum_setpoint.Format(_T("%.1f"), ((float)product_register_value[168]) / 10);

    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_SETPOINT, cs_co2_setpoint);
    m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_SETPOINT, cs_temp_setpoint);
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_SETPOINT, cs_hum_setpoint);

    CString cs_output_co2;
    CString cs_output_temp;
    CString cs_output_hum;
    cs_output_co2.Format(_T("%.1f"), ((float)product_register_value[141]) / 10);
    cs_output_temp.Format(_T("%.1f"), ((float)product_register_value[142]) / 10);
    cs_output_hum.Format(_T("%.1f"), ((float)product_register_value[143]) / 10);

    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_OUTPUT_VALUE, cs_output_co2);
    m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_OUTPUT_VALUE, cs_output_temp);
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_OUTPUT_VALUE, cs_output_hum);
    if (product_register_value[127] == 1)
    {
        m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_OUTPUT_RANGE, _T("4-20ma"));
        m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_OUTPUT_RANGE, _T("4-20ma"));
        m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_OUTPUT_RANGE, _T("4-20ma"));
    }
    else if (product_register_value[127] == 2)
    {
        m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_OUTPUT_RANGE, _T("0-5V"));
        m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_OUTPUT_RANGE, _T("0-5V"));
        m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_OUTPUT_RANGE, _T("0-5V"));
    }
    else if (product_register_value[127] == 3)
    {
        m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_OUTPUT_RANGE, _T("0-10V"));
        m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_OUTPUT_RANGE, _T("0-10V"));
        m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_OUTPUT_RANGE, _T("0-10V"));
    }

    CString cs_calibration_co2;
    CString cs_calibration_temp;
    CString cs_calibration_hum;
    if (product_register_value[127] == 1)
    {
        cs_calibration_co2.Format(_T("%d"), product_register_value[147]);
        cs_calibration_temp.Format(_T("%d"), product_register_value[148]);
        cs_calibration_hum.Format(_T("%d"), product_register_value[149]);
    }
    else if ((product_register_value[127] == 2) || (product_register_value[127] == 3))
    {
        cs_calibration_co2.Format(_T("%d"), product_register_value[144]);
        cs_calibration_temp.Format(_T("%d"), product_register_value[145]);
        cs_calibration_hum.Format(_T("%d"), product_register_value[146]);
    }
    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_CALIBRATION, cs_calibration_co2);
    m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_CALIBRATION, cs_calibration_temp);
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_CALIBRATION, cs_calibration_hum);


    CString temp_co2_min_value;
    CString temp_co2_max_value;

    CString temp_temp_min_value;
    CString temp_temp_max_value;

    CString temp_hum_min_value;
    CString temp_hum_max_value;
    temp_co2_min_value.Format(_T("%d"), product_register_value[132]);
    temp_co2_max_value.Format(_T("%d"), product_register_value[133]);

    temp_temp_min_value.Format(_T("%.1f"), ((float)product_register_value[128])/10);
    temp_temp_max_value.Format(_T("%.1f"), (float)product_register_value[129]/10);

    temp_hum_min_value.Format(_T("%.1f"), ((float)product_register_value[130])/10);
    temp_hum_max_value.Format(_T("%.1f"), ((float)product_register_value[131])/10);

    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_MIN_VALUE, temp_co2_min_value);
    m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_MIN_VALUE, temp_temp_min_value);
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_MIN_VALUE, temp_hum_min_value);

    m_co2_node_list.SetItemText(ROW_CO2, CO2_NODE_MAX_VALUE, temp_co2_max_value);
    m_co2_node_list.SetItemText(ROW_TEMPERATURE, CO2_NODE_MAX_VALUE, temp_temp_max_value);
    m_co2_node_list.SetItemText(ROW_HUMIDITY, CO2_NODE_MAX_VALUE, temp_hum_max_value);
    return 0;
}

void CCO2_NodeView::Show_Scroll_radio()
{
    bitset<16> Temp_bitset(product_register_value[170]);
    if (Temp_bitset[0])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL1))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL2))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL1))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL2))->SetCheck(1);
    }

    if (Temp_bitset[1])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL3))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL4))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL3))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL4))->SetCheck(1);
    }

    if (Temp_bitset[2])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL5))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL6))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL5))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL6))->SetCheck(1);
    }


    if (Temp_bitset[3])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL7))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL8))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL7))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL8))->SetCheck(1);
    }

    if (Temp_bitset[4])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL9))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL10))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL9))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL10))->SetCheck(1);
    }

    if (Temp_bitset[5])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL11))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL12))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL11))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL12))->SetCheck(1);
    }

    if (Temp_bitset[6])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL13))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL14))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL13))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL14))->SetCheck(1);
    }

    if (Temp_bitset[8])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL15))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL16))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL15))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL16))->SetCheck(1);
    }

    if (Temp_bitset[9])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL17))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL18))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL17))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL18))->SetCheck(1);
    }

    if (Temp_bitset[10])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL19))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL20))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL19))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL20))->SetCheck(1);
    }

    if (Temp_bitset[11])
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL21))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL22))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL21))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_SCROLL22))->SetCheck(1);
    }
}

void CCO2_NodeView::Show_Main_radio()
{
    bitset<16> Temp_bitset(product_register_value[169]);
    if (Temp_bitset[0])
    {
        ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(1);
    }

    if (Temp_bitset[1])
    {
        ((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(1);
    }

    if (Temp_bitset[2])
    {
        ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(1);
    }

    if (Temp_bitset[4])
    {
        ((CButton *)GetDlgItem(IDC_RADIO7))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO7))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(1);
    }

    if (Temp_bitset[5])
    {
        ((CButton *)GetDlgItem(IDC_RADIO9))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO10))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO9))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO10))->SetCheck(1);
    }

    if (Temp_bitset[6])
    {
        ((CButton *)GetDlgItem(IDC_RADIO11))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO12))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO11))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO12))->SetCheck(1);
    }

    if (Temp_bitset[7])
    {
        ((CButton *)GetDlgItem(IDC_RADIO13))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO14))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO13))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO14))->SetCheck(1);
    }

    if (Temp_bitset[8])
    {
        ((CButton *)GetDlgItem(IDC_RADIO15))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO16))->SetCheck(0);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO15))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO16))->SetCheck(1);
    }
}

void CCO2_NodeView::OnEnChangeEditCo2NodeModbusId()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CFormView::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。


}

void CCO2_NodeView::OnEnChangeEditCo2NodeRollTimeInterval()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CFormView::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码

}


void CCO2_NodeView::OnEnChangeEditCo2NodeBacklightTime()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CFormView::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码

}


void CCO2_NodeView::OnCbnSelchangeComboCo2NodeBaudrate()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_BAUDRATE))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_BAUDRATE))->GetLBText(nSel, temp_string);
    int temp_baudrate = product_register_value[15];
    for (int i = 0;i<sizeof(CO2_Node_Baudrate_Array) / sizeof(CO2_Node_Baudrate_Array[0]);i++)
    {
        if (temp_string.CompareNoCase(CO2_Node_Baudrate_Array[i]) == 0)
        {
            temp_baudrate = i;
            break;
        }
    }

    int nret = write_one(g_tstat_id, 15, temp_baudrate);
    Sleep(1000);
    unsigned short temp_array[200];
    Read_Multi(g_tstat_id, temp_array, 0, 100);  //Bug , CO2Node 一定要在用以前的读一下，新的baudrate 才能用
    if (nret <= 0)
    {
        MessageBox(_T("Change baudrate failed!"));
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change baudrate failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));

        int temp_brandrate = 19200;
        temp_brandrate = _wtoi(CO2_Node_Baudrate_Array[temp_baudrate]);
        CString SqlText;

        SqlText.Format(_T("update ALL_NODE set Bautrate = '%d' where Serial_ID='%d'"), temp_brandrate, get_serialnumber());
        Change_BaudRate(temp_brandrate);


        CppSQLite3DB SqliteDBBuilding;
        CppSQLite3Table table;
        CppSQLite3Query q;
        SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

        SqliteDBBuilding.execDML((UTF8MBSTR)SqlText);
        SqliteDBBuilding.closedb();
        CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
        pFrame->ScanTstatInDB();

    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnCbnSelchangeComboCo2NodeProtocol()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    bool  modbus_0_bacnet_1 = 0;
    int sub_node = 0;

    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_PROTOCOL))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_PROTOCOL))->GetLBText(nSel, temp_string);
    int temp_protocol = product_register_value[21];
    for (int i = 0;i<sizeof(CO2_Node_Protocol) / sizeof(CO2_Node_Protocol[0]);i++)
    {
        if (temp_string.CompareNoCase(CO2_Node_Protocol[i]) == 0)
        {
            if(i == 0)
                temp_protocol = 0;
            if (i == 1)
                temp_protocol = 3;
            break;
        }
    }

    if (temp_protocol == 3)
    {
        modbus_0_bacnet_1 = 0;
    }
    else if (temp_protocol == 0)
    {
        modbus_0_bacnet_1 = 1;
    }

    if (selected_product_Node.note_parent_serial_number != 0)
        sub_node = 1;
    else
        sub_node = 0;

    ChangeDeviceProtocol(modbus_0_bacnet_1, product_register_value[6], 21, temp_protocol, sub_node, g_strCurBuildingDatabasefilePath);
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
    return;
}


void CCO2_NodeView::OnCbnSelchangeComboCo2NodeTemperatureUnit()
{
    // TODO: 在此添加控件通知处理程序代码

    CString temp_cs;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_CO2_NODE_TEMPERATURE_UNIT))->GetLBText(nSel, temp_cs);

    int temp_value = product_register_value[125];
    if (temp_cs.CompareNoCase(_T("Deg.C")) == 0)
    {
        temp_value = 0;
    }
    else if (temp_cs.CompareNoCase(_T("Deg.F")) == 0)
    {
        temp_value = 1;
    }

    int nret = write_one(g_tstat_id, 125, temp_value);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Deg.C or Deg.F failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }

    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}

void CCO2_NodeView::UpdateRadio_ScrollDisplay()
{
    bitset<16> Temp_bitset(product_register_value[170]);

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL1))->GetCheck() == true)
    {
        Temp_bitset[0] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL2))->GetCheck() == true)
    {
        Temp_bitset[0] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL3))->GetCheck() == true)
    {
        Temp_bitset[1] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL4))->GetCheck() == true)
    {
        Temp_bitset[1] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL5))->GetCheck() == true)
    {
        Temp_bitset[2] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL6))->GetCheck() == true)
    {
        Temp_bitset[2] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL7))->GetCheck() == true)
    {
        Temp_bitset[3] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL8))->GetCheck() == true)
    {
        Temp_bitset[3] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL9))->GetCheck() == true)
    {
        Temp_bitset[4] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL10))->GetCheck() == true)
    {
        Temp_bitset[4] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL11))->GetCheck() == true)
    {
        Temp_bitset[5] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL12))->GetCheck() == true)
    {
        Temp_bitset[5] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL13))->GetCheck() == true)
    {
        Temp_bitset[7] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL14))->GetCheck() == true)
    {
        Temp_bitset[7] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL15))->GetCheck() == true)
    {
        Temp_bitset[8] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL16))->GetCheck() == true)
    {
        Temp_bitset[8] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL17))->GetCheck() == true)
    {
        Temp_bitset[9] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL18))->GetCheck() == true)
    {
        Temp_bitset[9] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL19))->GetCheck() == true)
    {
        Temp_bitset[10] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL20))->GetCheck() == true)
    {
        Temp_bitset[10] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL21))->GetCheck() == true)
    {
        Temp_bitset[11] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_SCROLL22))->GetCheck() == true)
    {
        Temp_bitset[11] = 0;
    }

    product_register_value[170] = (unsigned short)Temp_bitset.to_ulong();
    int nret = write_one(g_tstat_id, 170, product_register_value[170]);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }

    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}

void CCO2_NodeView::UpdateRadio_Main()
{
    bitset<16> Temp_bitset(product_register_value[169]);

    if (((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck() == true)
    {
        Temp_bitset[0] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck() == true)
    {
        Temp_bitset[0] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck() == true)
    {
        Temp_bitset[1] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO4))->GetCheck() == true)
    {
        Temp_bitset[1] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck() == true)
    {
        Temp_bitset[2] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO6))->GetCheck() == true)
    {
        Temp_bitset[2] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO7))->GetCheck() == true)
    {
        Temp_bitset[4] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO8))->GetCheck() == true)
    {
        Temp_bitset[4] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO9))->GetCheck() == true)
    {
        Temp_bitset[5] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO10))->GetCheck() == true)
    {
        Temp_bitset[5] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO11))->GetCheck() == true)
    {
        Temp_bitset[6] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO12))->GetCheck() == true)
    {
        Temp_bitset[6] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO13))->GetCheck() == true)
    {
        Temp_bitset[7] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO14))->GetCheck() == true)
    {
        Temp_bitset[7] = 0;
    }

    if (((CButton *)GetDlgItem(IDC_RADIO15))->GetCheck() == true)
    {
        Temp_bitset[8] = 1;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO16))->GetCheck() == true)
    {
        Temp_bitset[8] = 0;
    }
    product_register_value[169] = (unsigned short)Temp_bitset.to_ulong();
    int nret = write_one(g_tstat_id, 169, product_register_value[169]);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }

    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnBnClickedRadio1()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio2()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio3()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio4()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio5()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio6()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio7()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio8()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio9()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio10()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio11()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio12()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio13()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio14()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio15()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedRadio16()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_Main();
}


void CCO2_NodeView::OnBnClickedButtonOk()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CCO2_NodeView::OnEnKillfocusEditCo2NodeModbusId()
{
    // TODO: 在此添加控件通知处理程序代码
    // TODO:  在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_CO2_NODE_MODBUS_ID)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    int nret = write_one(g_tstat_id, 6, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Modbus ID failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnEnKillfocusEditCo2NodeRollTimeInterval()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_CO2_NODE_ROLL_TIME_INTERVAL)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    int nret = write_one(g_tstat_id, 171, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change roll internal time failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnEnKillfocusEditCo2NodeBacklightTime()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_CO2_NODE_BACKLIGHT_TIME)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    int nret = write_one(g_tstat_id, 158, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change backlight time failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnEnKillfocusEditFairAlarmSetpoint()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_FAIR_ALARM_SETPOINT)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    int nret = write_one(g_tstat_id, 112, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change fair alarm setpoint time failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnEnKillfocusEditPoorAlarmSetpoint()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_POOR_ALARM_SETPOINT)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    int nret = write_one(g_tstat_id, 113, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change poor alarm setpoint time failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnBnClickedRadioScroll1()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll2()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll3()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll4()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll5()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll6()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll7()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll8()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll9()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll10()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll11()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll12()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll13()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll14()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll15()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll16()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll17()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll18()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll19()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll20()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll21()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnBnClickedRadioScroll22()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateRadio_ScrollDisplay();
}


void CCO2_NodeView::OnEnKillfocusEditNatureCo2()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_DLG_NATURE_CO2)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 390) || (temp_int > 500))
    {
        MessageBox(_T("Nature's CO2 ppm should be in this range (390 - 500)"));
        return;
    }

    int nret = write_one(g_tstat_id, CO2_MODBUS_CO2_NATURE_LEVEL, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nature Co2 Value (ppm) failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnEnKillfocusEditMaxMinAdjPerday()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_DLG_MAX_MIN_ADJ_PERDAY)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 1) || (temp_int > 10))
    {
        MessageBox(_T("Maximum adjustment per day should be in this range (1 - 10)"));
        return;
    }

    int nret = write_one(g_tstat_id, CO2_MODBUS_CO2_MIN_ADJ, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nature Co2 Value (ppm) failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnEnKillfocusEditLookDays()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItem(IDC_EDIT_DLG_LOOK_DAYS)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 7) || (temp_int > 30))
    {
        MessageBox(_T("Maximum adjustment per day should be in this range (7 - 30)"));
        return;
    }

    int nret = write_one(g_tstat_id, CO2_MODBUS_CO2_CAL_DAYS, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Nature Co2 Value (ppm) failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }
    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}


void CCO2_NodeView::OnCbnSelchangeComboCo2AutoCal()
{
    // TODO: 在此添加控件通知处理程序代码




    CString temp_cs;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_AUTO_CAL))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_AUTO_CAL))->GetLBText(nSel, temp_cs);

    int temp_value = product_register_value[CO2_MODBUS_CO2_CAL_DAYS];
    for (int i = 0;i<sizeof(CO2_Node_Auto_Cal) / sizeof(CO2_Node_Auto_Cal[0]);i++)
    {
        if (temp_cs.CompareNoCase(CO2_Node_Auto_Cal[i]) == 0)
        {
            temp_value = i;
            break;
        }
    }

    int nret = write_one(g_tstat_id, CO2_MODBUS_CO2_CAL_DAYS, temp_value);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change status failed!"));
        PostMessage(WM_CO2_NODE_THREAD_READ, NULL, NULL);
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }

    ((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFocus();
}
