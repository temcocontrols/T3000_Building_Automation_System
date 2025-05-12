// TstatSetpointDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatSetpointDetail.h"
#include "afxdialogex.h"
#include "global_function.h"

// CTstatSetpointDetail 对话框

IMPLEMENT_DYNAMIC(CTstatSetpointDetail, CDialogEx)

CTstatSetpointDetail::CTstatSetpointDetail(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TSTAT_SETPOINT, pParent)
{

}

CTstatSetpointDetail::~CTstatSetpointDetail()
{
}

void CTstatSetpointDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_TSTAT_SETPOINT, m_tstat_setpoint_list);

}


BEGIN_MESSAGE_MAP(CTstatSetpointDetail, CDialogEx)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_Setpoint_Item)
    ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_LIST, Fresh_Setpoint_List)

    ON_MESSAGE(MY_RESUME_DATA, ReadDataAgain)
END_MESSAGE_MAP()

LRESULT  CTstatSetpointDetail::ReadDataAgain(WPARAM wParam, LPARAM lParam)
{
    int read_ret = -1;
    int read_value_flag = 0;;
    read_value_flag = (int)wParam;
    if((read_value_flag == HOME_MODE) || (read_value_flag == WORK_MODE))
        read_ret = Read_Multi(g_tstat_id, &product_register_value[300], 300, 100, 3);
    else if (read_value_flag == SLEEP_MODE)
    {
        read_ret = Read_Multi(g_tstat_id, &product_register_value[770], 770, 10, 3);
    }
    else if (read_value_flag == AWAY_MODE)
    {
        read_ret = Read_Multi(g_tstat_id, &product_register_value[970], 970, 10, 3);
    }
    else
    {
        return 1;
    }

    if (read_ret >= 0)
    {
        Tstat_Setpoint_data.clear();
        Check_Tstat_Home_Setpoint();
        Check_Tstat_Work_Setpoint();
        Check_Tstat_Sleep_Setpoint();
        Check_Tstat_Away_Setpoint();

        Fresh_Setpoint_List(NULL, NULL);
    }


}
void CTstatSetpointDetail::Fresh_window()
{

}
// CTstatSetpointDetail 消息处理程序

LRESULT CTstatSetpointDetail::Fresh_Setpoint_Item(WPARAM wParam, LPARAM lParam)
{
    int cmp_ret;//compare if match it will 0;
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;

    CString cs_changed_item;
    cs_changed_item = m_tstat_setpoint_list.GetItemText(Changed_Item, Changed_SubItem);
    if (Changed_SubItem == TSTAT_SETPOINT_TYPE)
    {
        if (cs_changed_item.CompareNoCase(SetpointType[0]) == 0)
        {
            Tstat_Setpoint_data.at(Changed_Item).type.m_value = 0;
            Tstat_Setpoint_data.at(Changed_Item).type.showvalue = SetpointType[0];
        }
        else
        {
            Tstat_Setpoint_data.at(Changed_Item).type.m_value = 1;
            Tstat_Setpoint_data.at(Changed_Item).type.showvalue = SetpointType[1];
        }
        
        switch (Tstat_Setpoint_data.at(Changed_Item).name.m_value)
        {
        case WORK_MODE:
            MNIGHT = Tstat_Setpoint_data.at(Changed_Item).name.m_value;
            break;
        case HOME_MODE:
            MDAY = Tstat_Setpoint_data.at(Changed_Item).name.m_value;
            break;
        case AWAY_MODE:
            MAWAY = Tstat_Setpoint_data.at(Changed_Item).name.m_value;
            break;
        case SLEEP_MODE:
            MSLEEP = Tstat_Setpoint_data.at(Changed_Item).name.m_value;
            break;
        default:
            break;
        }
    }
    else if ((Changed_SubItem >= TSTAT_SETPOINT_SETPOINT) && (Changed_SubItem <= TSTAT_SETPOINT_HEAT_DB))
    {
        float temp_value = 0;
        int temp_value_mul_10 = 0;
        temp_value_mul_10 = _wtof(cs_changed_item) * 10;

        int temp_reg = 0;

        switch (Changed_SubItem)
        {
        case TSTAT_SETPOINT_SETPOINT:
            temp_reg = Tstat_Setpoint_data.at(Changed_Item).setpoint.modbus_reg;
            break;
        case TSTAT_SETPOINT_HEAT_SP:
            temp_reg = Tstat_Setpoint_data.at(Changed_Item).heat_setpoint.modbus_reg;
            break;
        case TSTAT_SETPOINT_COOL_SP:
            temp_reg = Tstat_Setpoint_data.at(Changed_Item).cool_setpoint.modbus_reg;
            break;
        case TSTAT_SETPOINT_COOL_DB:
            temp_reg = Tstat_Setpoint_data.at(Changed_Item).cool_db.modbus_reg;
            break;
        case TSTAT_SETPOINT_HEAT_DB:
            temp_reg = Tstat_Setpoint_data.at(Changed_Item).heat_db.modbus_reg;
            break;

        default:
            return -1;
            break;
            
        }
        int ret = write_one(g_tstat_id, (unsigned short)temp_reg, temp_value_mul_10);
        if (ret >= 0)
        {
            product_register_value[temp_reg] = temp_value_mul_10;
            PostMessage(MY_RESUME_DATA, Tstat_Setpoint_data.at(Changed_Item).name.m_value, NULL);
        }
    }
    return 1;
}


LRESULT CTstatSetpointDetail::Fresh_Setpoint_List(WPARAM wParam, LPARAM lParam)
{
    // Str_in_point Get_Str_in_Point(int index);
    int Fresh_Item;
    int isFreshOne = (int)lParam;

    for (int i = 0; i < (int)Tstat_Setpoint_data.size(); i++)
    {
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_ITEM, Tstat_Setpoint_data.at(i).name.showvalue);
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_TYPE, Tstat_Setpoint_data.at(i).type.showvalue);
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_SETPOINT, Tstat_Setpoint_data.at(i).setpoint.showvalue);
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_HEAT_SP, Tstat_Setpoint_data.at(i).heat_setpoint.showvalue);
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_COOL_SP, Tstat_Setpoint_data.at(i).cool_setpoint.showvalue);
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_COOL_DB, Tstat_Setpoint_data.at(i).cool_db.showvalue);
        m_tstat_setpoint_list.SetItemText(i, TSTAT_SETPOINT_HEAT_DB, Tstat_Setpoint_data.at(i).heat_db.showvalue);
    }
    return 0;
}



void CTstatSetpointDetail::Initial_List()
{
    m_tstat_setpoint_list.ShowWindow(SW_HIDE);
    m_tstat_setpoint_list.DeleteAllItems();
    while (m_tstat_setpoint_list.DeleteColumn(0));

    m_tstat_setpoint_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    //m_tstat_setpoint_list.SetExtendedStyle(m_tstat_setpoint_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_tstat_setpoint_list.SetExtendedStyle(m_tstat_setpoint_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_ITEM, _T(" "), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_TYPE, _T("Scrollbar Type"), 100, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_SETPOINT, _T("Setpoint"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_HEAT_SP, _T("HeatSP"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_COOL_SP, _T("CoolSP"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_COOL_DB, _T("CoolDeadband"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_tstat_setpoint_list.InsertColumn(TSTAT_SETPOINT_HEAT_DB, _T("HeatDeadband"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

    m_tstat_setpoint_list.SetListHwnd(this->m_hWnd);
    CRect list_rect, win_rect;
    m_tstat_setpoint_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
    m_tstat_setpoint_list.Set_My_WindowRect(win_rect);
    m_tstat_setpoint_list.Set_My_ListRect(list_rect);

    m_tstat_setpoint_list.DeleteAllItems();

    for (int i = 0; i < (int)Tstat_Setpoint_data.size(); i++)
    {
        CString temp_item;
        temp_item.Format(_T("%d"), i + 1);
        m_tstat_setpoint_list.InsertItem(i, temp_item);
        m_tstat_setpoint_list.SetCellEnabled(i, 0, 0);

        if (ListCtrlEx::ComboBox == m_tstat_setpoint_list.GetColumnType(TSTAT_SETPOINT_TYPE))
        {
            ListCtrlEx::CStrList strlist;
            strlist.push_back(SetpointType[0]);
            strlist.push_back(SetpointType[1]);
            m_tstat_setpoint_list.SetCellStringList(i, TSTAT_SETPOINT_TYPE, strlist);
        }

        for (int x = 0;x<TSTAT_SETPOINT_MAX;x++)
        {
            if ((i % 2) == 0)
                m_tstat_setpoint_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
            else
                m_tstat_setpoint_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
        }
    }
    m_tstat_setpoint_list.ShowWindow(SW_SHOW);
    Fresh_Setpoint_List(NULL, NULL);
}




BOOL CTstatSetpointDetail::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    Tstat_Setpoint_data.clear();
    Check_Tstat_Home_Setpoint();
    Check_Tstat_Work_Setpoint();
    Check_Tstat_Sleep_Setpoint();
    Check_Tstat_Away_Setpoint();
    // TODO:  在此添加额外的初始化
    Initial_List();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}



const int TSTAT8_DAY_SETPOINT = 345;
const int TSTAT8_DAY_HEATING_SETPOINT = 349;    
const int TSTAT8_DAY_COOLING_SETPOINT = 348;
const int TSTAT8_DAY_COOLING_DEADBAND = 346;
const int TSTAT8_DAY_HEATING_DEADBAND = 347;

const int TSTAT8_NIGHT_SETPOINT = 350;
const int TSTAT8_NIGHT_HEATING_SETPOINT = 354;
const int TSTAT8_NIGHT_COOLING_SETPOINT = 355;
const int TSTAT8_NIGHT_COOLING_DEADBAND = 353;
const int TSTAT8_NIGHT_HEATING_DEADBAND = 352;


const int TSTAT8_AWAY_SETPOINT = 972;
const int TSTAT8_AWAY_HEATING_SETPOINT = 974;
const int TSTAT8_AWAY_COOLING_SETPOINT = 973;
const int TSTAT8_AWAY_COOLING_DEADBAND = 975;
const int TSTAT8_AWAY_HEATING_DEADBAND = 976;

const int TSTAT8_SLEEP_SETPOINT = 773;
const int TSTAT8_SLEEP_HEATING_SETPOINT = 775;
const int TSTAT8_SLEEP_COOLING_SETPOINT = 774;
const int TSTAT8_SLEEP_COOLING_DEADBAND = 771;
const int TSTAT8_SLEEP_HEATING_DEADBAND = 772;

void Check_Tstat_Work_Setpoint()
{
    Str_tstat_setpoint temp;
    temp.name.m_value = WORK_MODE;
    temp.name.showvalue = _T("Work Mode");
    temp.type.m_value = MNIGHT;
    CString cstype_mode;
    if (temp.type.m_value == 1)
        cstype_mode = _T("Dual");
    else
        cstype_mode = _T("Single");
    temp.type.showvalue = cstype_mode;
    temp.setpoint.modbus_reg = TSTAT8_NIGHT_SETPOINT;
    temp.setpoint.m_value = product_register_value[TSTAT8_NIGHT_SETPOINT];
    temp.setpoint.showvalue.Format(_T("%.1f"), ((float)temp.setpoint.m_value) / 10);

    temp.heat_setpoint.modbus_reg = TSTAT8_NIGHT_HEATING_SETPOINT;
    temp.heat_setpoint.m_value = product_register_value[TSTAT8_NIGHT_HEATING_SETPOINT];
    temp.heat_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.heat_setpoint.m_value) / 10);

    temp.cool_setpoint.modbus_reg = TSTAT8_NIGHT_COOLING_SETPOINT;
    temp.cool_setpoint.m_value = product_register_value[TSTAT8_NIGHT_COOLING_SETPOINT];
    temp.cool_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.cool_setpoint.m_value) / 10);

    temp.cool_db.modbus_reg = TSTAT8_NIGHT_COOLING_DEADBAND;
    temp.cool_db.m_value = product_register_value[TSTAT8_NIGHT_COOLING_DEADBAND];
    temp.cool_db.showvalue.Format(_T("%.1f"), ((float)temp.cool_db.m_value) / 10);

    temp.heat_db.modbus_reg = TSTAT8_NIGHT_HEATING_DEADBAND;
    temp.heat_db.m_value = product_register_value[TSTAT8_NIGHT_HEATING_DEADBAND];
    temp.heat_db.showvalue.Format(_T("%.1f"), ((float)temp.heat_db.m_value) / 10);
    Tstat_Setpoint_data.push_back(temp);
}
void Check_Tstat_Away_Setpoint()
{
    Str_tstat_setpoint temp;
    temp.name.m_value = AWAY_MODE;
    temp.name.showvalue = _T("Away Mode");
    temp.type.m_value = MAWAY;
    CString cstype_mode;
    if (temp.type.m_value == 1)
        cstype_mode = _T("Dual");
    else
        cstype_mode = _T("Single");
    temp.type.showvalue = cstype_mode;
    temp.setpoint.modbus_reg = TSTAT8_AWAY_SETPOINT;
    temp.setpoint.m_value = product_register_value[TSTAT8_AWAY_SETPOINT];
    temp.setpoint.showvalue.Format(_T("%.1f"), ((float)temp.setpoint.m_value) / 10);

    temp.heat_setpoint.modbus_reg = TSTAT8_AWAY_HEATING_SETPOINT;
    temp.heat_setpoint.m_value = product_register_value[TSTAT8_AWAY_HEATING_SETPOINT];
    temp.heat_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.heat_setpoint.m_value) / 10);

    temp.cool_setpoint.modbus_reg = TSTAT8_AWAY_COOLING_SETPOINT;
    temp.cool_setpoint.m_value = product_register_value[TSTAT8_AWAY_COOLING_SETPOINT];
    temp.cool_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.cool_setpoint.m_value) / 10);

    temp.cool_db.modbus_reg = TSTAT8_AWAY_COOLING_DEADBAND;
    temp.cool_db.m_value = product_register_value[TSTAT8_AWAY_COOLING_DEADBAND];
    temp.cool_db.showvalue.Format(_T("%.1f"), ((float)temp.cool_db.m_value) / 10);

    temp.heat_db.modbus_reg = TSTAT8_AWAY_HEATING_DEADBAND;
    temp.heat_db.m_value = product_register_value[TSTAT8_AWAY_HEATING_DEADBAND];
    temp.heat_db.showvalue.Format(_T("%.1f"), ((float)temp.heat_db.m_value) / 10);
    Tstat_Setpoint_data.push_back(temp);
}
void Check_Tstat_Sleep_Setpoint()
{
    Str_tstat_setpoint temp;
    temp.name.m_value = SLEEP_MODE;
    temp.name.showvalue = _T("Sleep Mode");
    temp.type.m_value = MSLEEP;
    CString cstype_mode;
    if (temp.type.m_value == 1)
        cstype_mode = _T("Dual");
    else
        cstype_mode = _T("Single");
    temp.type.showvalue = cstype_mode;
    temp.setpoint.modbus_reg = TSTAT8_SLEEP_SETPOINT;
    temp.setpoint.m_value = product_register_value[TSTAT8_SLEEP_SETPOINT];
    temp.setpoint.showvalue.Format(_T("%.1f"), ((float)temp.setpoint.m_value) / 10);

    temp.heat_setpoint.modbus_reg = TSTAT8_SLEEP_HEATING_SETPOINT;
    temp.heat_setpoint.m_value = product_register_value[TSTAT8_SLEEP_HEATING_SETPOINT];
    temp.heat_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.heat_setpoint.m_value) / 10);

    temp.cool_setpoint.modbus_reg = TSTAT8_SLEEP_COOLING_SETPOINT;
    temp.cool_setpoint.m_value = product_register_value[TSTAT8_SLEEP_COOLING_SETPOINT];
    temp.cool_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.cool_setpoint.m_value) / 10);

    temp.cool_db.modbus_reg = TSTAT8_SLEEP_COOLING_DEADBAND;
    temp.cool_db.m_value = product_register_value[TSTAT8_SLEEP_COOLING_DEADBAND];
    temp.cool_db.showvalue.Format(_T("%.1f"), ((float)temp.cool_db.m_value) / 10);

    temp.heat_db.modbus_reg = TSTAT8_SLEEP_HEATING_DEADBAND;
    temp.heat_db.m_value = product_register_value[TSTAT8_SLEEP_HEATING_DEADBAND];
    temp.heat_db.showvalue.Format(_T("%.1f"), ((float)temp.heat_db.m_value) / 10);
    Tstat_Setpoint_data.push_back(temp);
}
void Check_Tstat_Home_Setpoint()
{
    Str_tstat_setpoint temp;
    temp.name.m_value = HOME_MODE;
    temp.name.showvalue = _T("Home Mode");
    temp.type.m_value = MDAY;
    CString cstype_mode;
    if (temp.type.m_value == 1)
        cstype_mode = _T("Dual");
    else
        cstype_mode = _T("Single");
    temp.type.showvalue = cstype_mode;
    temp.setpoint.modbus_reg = TSTAT8_DAY_SETPOINT;
    temp.setpoint.m_value = product_register_value[TSTAT8_DAY_SETPOINT];
    temp.setpoint.showvalue.Format(_T("%.1f"), ((float)temp.setpoint.m_value) / 10);

    temp.heat_setpoint.modbus_reg = TSTAT8_DAY_HEATING_SETPOINT;
    temp.heat_setpoint.m_value = product_register_value[TSTAT8_DAY_HEATING_SETPOINT];
    temp.heat_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.heat_setpoint.m_value) / 10);

    temp.cool_setpoint.modbus_reg = TSTAT8_DAY_COOLING_SETPOINT;
    temp.cool_setpoint.m_value = product_register_value[TSTAT8_DAY_COOLING_SETPOINT];
    temp.cool_setpoint.showvalue.Format(_T("%.1f"), ((float)temp.cool_setpoint.m_value) / 10);

    temp.cool_db.modbus_reg = TSTAT8_DAY_COOLING_DEADBAND;
    temp.cool_db.m_value = product_register_value[TSTAT8_DAY_COOLING_DEADBAND];
    temp.cool_db.showvalue.Format(_T("%.1f"), ((float)temp.cool_db.m_value) / 10);

    temp.heat_db.modbus_reg = TSTAT8_DAY_HEATING_DEADBAND;
    temp.heat_db.m_value = product_register_value[TSTAT8_DAY_HEATING_DEADBAND];
    temp.heat_db.showvalue.Format(_T("%.1f"), ((float)temp.heat_db.m_value) / 10);
    Tstat_Setpoint_data.push_back(temp);


}



