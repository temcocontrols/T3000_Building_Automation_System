// PowerMeterList.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "PowerMeterList.h"
#include "global_function.h"
#include "afxdialogex.h"
bool powermeter_thread_on = true;
const int POWERMETER_VOLTAGE_FREQUENCY = 0;
//const int POWERMETER_PERIOD = 1;
const int POWERMETER_VRMS = 1;
const int POWERMETER_VRMS_CAL = 2;
const int POWERMETER_IRMS = 3;
const int POWERMETER_IRMS_CAL = 4;
const int POWERMETER_ACTIVE_POWER = 5;
const int POWERMETER_LAENERGY = 6;
const int POWERMETER_FACTOR = 7;
const int POWERMETER_MAX_COUNT = 8;

// CPowerMeterList 对话框

IMPLEMENT_DYNAMIC(CPowerMeterList, CDialogEx)

CPowerMeterList::CPowerMeterList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POWER_METER, pParent)
{
    powermeter_thread_on = true;
}

CPowerMeterList::~CPowerMeterList()
{
    powermeter_thread_on = false;
}

void CPowerMeterList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_POWER_METER, m_powermeter_list);
    DDX_Control(pDX, IDC_POWERMETER_TITLE, m_powermeter_title);
}


BEGIN_MESSAGE_MAP(CPowerMeterList, CDialogEx)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_PowerMeter_Item)
    ON_MESSAGE(WM_REFRESH_POWERMETER_LIST, Fresh_PowerMeter_List)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_POWER_LAENERGT_U, &CPowerMeterList::OnBnClickedButtonClearPowerLaenergtU)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_POWER_LAENERGT_V, &CPowerMeterList::OnBnClickedButtonClearPowerLaenergtV)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_POWER_LAENERGT_W, &CPowerMeterList::OnBnClickedButtonClearPowerLaenergtW)
    ON_BN_CLICKED(IDC_RADIO3, &CPowerMeterList::OnBnClickedRadio3)
    ON_BN_CLICKED(IDC_RADIO_EXTERNAL_CT, &CPowerMeterList::OnBnClickedRadioExternalCt)
    ON_BN_CLICKED(IDC_RADIO_POWER_INTERNAL_CT, &CPowerMeterList::OnBnClickedRadioPowerInternalCt)
    ON_EN_CHANGE(IDC_EDIT2, &CPowerMeterList::OnEnChangeEdit2)
    ON_EN_CHANGE(IDC_EDIT4, &CPowerMeterList::OnEnChangeEdit4)
    ON_EN_KILLFOCUS(IDC_EDIT_RATIO_VALUE, &CPowerMeterList::OnEnKillfocusEditRatioValue)
    ON_STN_CLICKED(IDC_POWERMETER_TITLE, &CPowerMeterList::OnStnClickedPowermeterTitle)
END_MESSAGE_MAP()


// CPowerMeterList 消息处理程序
DWORD WINAPI UpdatePowerMeterThread(LPVOID lPvoid);

BOOL CPowerMeterList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    Initial_List();
    int temp_value;
    temp_value = read_one(g_tstat_id, 1071);
    if (temp_value >= 0)
    {
        product_register_value[1071] = temp_value;
        CString temp_ratio;
        temp_ratio.Format(_T("%.2f"), ((float)product_register_value[1071]) / 100);
        GetDlgItem(IDC_EDIT_RATIO_VALUE)->SetWindowTextW(temp_ratio);
    }


    HANDLE refreshthread = NULL;
    refreshthread = CreateThread(NULL, NULL, UpdatePowerMeterThread, this, NULL, NULL);
    CloseHandle(refreshthread);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

DWORD WINAPI UpdatePowerMeterThread(LPVOID lPvoid)
{
    CPowerMeterList * mparent = (CPowerMeterList *)lPvoid;
    int read_ret[5] = {0};
    while (powermeter_thread_on)
    {
        memset(read_ret, 0, 5*sizeof(int));
        for (int i = 0; i < 4; i++)
        {
            read_ret[i] = Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100, 3);
        }

            read_ret[4] = Read_Multi(g_tstat_id, &product_register_value[1070], 1070, 20, 3);
        
            if (read_ret[0] && read_ret[1] && read_ret[2] && read_ret[3] && read_ret[4])
            {
                ::PostMessage(mparent->m_hWnd, WM_REFRESH_POWERMETER_LIST, NULL, NULL);
            }
            Sleep(5000);
    }
    Sleep(1);
    return 0;
}

BOOL CPowerMeterList::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CPowerMeterList::Initial_List()
{

    CString temp_cs_size, temp_cs_free;
    m_powermeter_title.SetWindowTextW(_T("UVW Three-Phase"));
    m_powermeter_title.textColor(RGB(0, 0, 0));
    //m_powermeter_title.bkColor(RGB(0,0,255));
    m_powermeter_title.setFont(35, 25, NULL, _T("Arial"));

    m_powermeter_list.ShowWindow(SW_HIDE);
    m_powermeter_list.DeleteAllItems();
    while (m_powermeter_list.DeleteColumn(0));

    m_powermeter_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    //m_powermeter_list.SetExtendedStyle(m_powermeter_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_powermeter_list.SetExtendedStyle(m_powermeter_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_powermeter_list.InsertColumn(0, _T("Power Meter"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_powermeter_list.InsertColumn(1, _T("Phase U"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_powermeter_list.InsertColumn(2, _T("Phase V"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_powermeter_list.InsertColumn(3, _T("Phase W"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_powermeter_list.InsertColumn(4, _T("Unit"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
   
    m_powermeter_list.SetListHwnd(this->m_hWnd);
    CRect list_rect, win_rect;
    m_powermeter_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
    m_powermeter_list.Set_My_WindowRect(win_rect);
    m_powermeter_list.Set_My_ListRect(list_rect);

    m_powermeter_list.DeleteAllItems();

    CString temp_item;
    temp_item.Format(_T("Voltage Frequency"));
    m_powermeter_list.InsertItem(POWERMETER_VOLTAGE_FREQUENCY, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_VOLTAGE_FREQUENCY, 4, _T("HZ"));
    //temp_item.Format(_T("Powermeter Period"));
    //m_powermeter_list.InsertItem(POWERMETER_PERIOD, temp_item);

    temp_item.Format(_T("VRMS Value"));
    m_powermeter_list.InsertItem(POWERMETER_VRMS, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_VRMS, 4, _T("Volts"));

    temp_item.Format(_T("VRMS Calibration"));
    m_powermeter_list.InsertItem(POWERMETER_VRMS_CAL, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_VRMS_CAL, 4, _T("Volts"));

    temp_item.Format(_T("IRMS Value"));
    m_powermeter_list.InsertItem(POWERMETER_IRMS, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_IRMS, 4, _T("Volts"));

    temp_item.Format(_T("IRMS Calibration"));
    m_powermeter_list.InsertItem(POWERMETER_IRMS_CAL, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_IRMS_CAL, 4, _T("Volts"));

    temp_item.Format(_T("Active Power"));
    m_powermeter_list.InsertItem(POWERMETER_ACTIVE_POWER, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_ACTIVE_POWER, 4, _T("Watt"));

    temp_item.Format(_T("Laenergy"));
    m_powermeter_list.InsertItem(POWERMETER_LAENERGY, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_LAENERGY, 4, _T("Kwh"));

    temp_item.Format(_T("Factor"));
    m_powermeter_list.InsertItem(POWERMETER_FACTOR, temp_item);
    m_powermeter_list.SetItemText(POWERMETER_FACTOR, 4, _T("%"));
    
    for (int  i = 1; i < 4; i++)
    {
        m_powermeter_list.SetCellEnabled(POWERMETER_VOLTAGE_FREQUENCY, i, 0);
        m_powermeter_list.SetCellEnabled(POWERMETER_VRMS, i, 0);
        m_powermeter_list.SetCellEnabled(POWERMETER_IRMS, i, 0);
        m_powermeter_list.SetCellEnabled(POWERMETER_ACTIVE_POWER, i, 0);
        m_powermeter_list.SetCellEnabled(POWERMETER_FACTOR, i, 0);
        m_powermeter_list.SetCellEnabled(POWERMETER_LAENERGY, i, 0);
    }
    
    for (int x = 0;x<5;x++)
    {
        for (int i = 0; i < POWERMETER_MAX_COUNT; i++)
        {
            if ((i % 2) == 0)
                m_powermeter_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
            else
                m_powermeter_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
        }

    }

    m_powermeter_list.ShowWindow(SW_SHOW);
}



LRESULT CPowerMeterList::Fresh_PowerMeter_Item(WPARAM wParam, LPARAM lParam)
{
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;

    CString temp_task_info;
    CString New_CString = m_powermeter_list.GetItemText(Changed_Item, Changed_SubItem);


    if (Changed_Item == POWERMETER_VRMS_CAL)
    {
        float temp_value = 0;
        int write_value = 0;
        temp_value = _wtof(New_CString);
        write_value = (int)(temp_value * 10);
        int write_ret = 0;
        write_ret = write_one(g_tstat_id, 120 + (Changed_SubItem - 1)*100, write_value, 3);
    }
    else if (Changed_Item == POWERMETER_IRMS_CAL)
    {
        float temp_value = 0;
        int write_value = 0;
        temp_value = _wtof(New_CString);
        write_value = (int)(temp_value * 10);
        int write_ret = 0;
        write_ret = write_one(g_tstat_id, 125 + (Changed_SubItem - 1) * 100, write_value, 3);
    }

    return 0;
}



LRESULT CPowerMeterList::Fresh_PowerMeter_List(WPARAM wParam, LPARAM lParam)
{
    CString temp_voltage_frequency[3];
    CString temp_period[3];
    CString temp_vrms[3];
    CString temp_irms[3];
    CString temp_vrms_cal[3];
    CString temp_irms_cal[3];
    CString temp_active_power[3];
    CString temp_laenergy[3];
    CString temp_factor[3];


    for (int i = 0; i < 3; i++)
    {
        temp_voltage_frequency[i].Format(_T("%.1f"), ((float)product_register_value[103 + i*100]) / 10);
        temp_period[i].Format(_T("%u"), product_register_value[104 + i * 100]);

        temp_vrms[i].Format(_T("%.1f"), ((float)product_register_value[117 + i * 100]) / 10);
        temp_irms[i].Format(_T("%.1f"), ((float)product_register_value[118 + i * 100]) / 10);

        temp_vrms_cal[i].Format(_T("%.1f"), ((float)product_register_value[120 + i * 100]) / 10);
        temp_irms_cal[i].Format(_T("%.1f"), ((float)product_register_value[125 + i * 100]) / 10);

        temp_active_power[i].Format(_T("%.1f"), ((float)product_register_value[130 + i * 100]) / 10);
        temp_laenergy[i].Format(_T("%.2f"), ((float)(product_register_value[148 + i * 100] * 65536 + product_register_value[149 + i * 100])) / 10000);
        temp_factor[i].Format(_T("%d"), product_register_value[160 + i * 100] );
    }
    for (int i = 1; i < 4; i++)
    {
        m_powermeter_list.SetItemText(POWERMETER_VOLTAGE_FREQUENCY, i, temp_voltage_frequency[i-1]);

        //m_powermeter_list.SetItemText(POWERMETER_PERIOD, i, temp_period[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_VRMS, i, temp_vrms[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_VRMS_CAL, i, temp_vrms_cal[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_IRMS, i, temp_irms[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_IRMS_CAL, i, temp_irms_cal[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_ACTIVE_POWER, i, temp_active_power[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_LAENERGY, i, temp_laenergy[i-1]);
        m_powermeter_list.SetItemText(POWERMETER_FACTOR, i, temp_factor[i-1]);

    }

    if (product_register_value[1070] == 0)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_POWER_INTERNAL_CT))->SetCheck(true);
        ((CButton *)GetDlgItem(IDC_RADIO_EXTERNAL_CT))->SetCheck(false);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_RADIO_POWER_INTERNAL_CT))->SetCheck(false);
        ((CButton *)GetDlgItem(IDC_RADIO_EXTERNAL_CT))->SetCheck(true);
    }



    CString temp_total_active_power;
    temp_total_active_power.Format(_T("%.1f"), ((float)(product_register_value[1080]*65536 + product_register_value[1081])) / 10);
    GetDlgItem(IDC_EDIT_ACTICE_POWER)->SetWindowTextW(temp_total_active_power);

    CString temp_total_energy;
    temp_total_energy.Format(_T("%.1f"), ((float)(product_register_value[1082] * 65536 + product_register_value[1083])) / 10);
    GetDlgItem(IDC_EDIT_TOTAL_ENERGY)->SetWindowTextW(temp_total_energy);

    return 0;
}






void CPowerMeterList::OnBnClickedButtonClearPowerLaenergtU()
{
    // TODO: 在此添加控件通知处理程序代码
    m_powermeter_list.SetItemText(POWERMETER_LAENERGY, 1, _T("0"));
    int write_ret = 0;
    write_ret = write_one(g_tstat_id, 148, 0, 3);
    if (write_ret >= 0)
    {
        MessageBox(_T("Clear phase(U) laenergy success!"));
    }
}


void CPowerMeterList::OnBnClickedButtonClearPowerLaenergtV()
{
    // TODO: 在此添加控件通知处理程序代码
    m_powermeter_list.SetItemText(POWERMETER_LAENERGY, 2, _T("0"));
    int write_ret = 0;
    write_ret = write_one(g_tstat_id, 248, 0, 3);
    if (write_ret >= 0)
    {
        MessageBox(_T("Clear phase(V) laenergy success!"));
    }
}


void CPowerMeterList::OnBnClickedButtonClearPowerLaenergtW()
{
    // TODO: 在此添加控件通知处理程序代码
    m_powermeter_list.SetItemText(POWERMETER_LAENERGY, 3, _T("0"));

    int write_ret = 0;
    write_ret = write_one(g_tstat_id, 348, 0, 3);
    if (write_ret >= 0)
    {
        MessageBox(_T("Clear phase(W) laenergy success!"));
    }
}


void CPowerMeterList::OnBnClickedRadio3()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CPowerMeterList::OnBnClickedRadioExternalCt()
{
    // TODO: 在此添加控件通知处理程序代码
    int write_ret = 0;
    write_ret = write_one(g_tstat_id, 1070, 1, 3);
    if (write_ret <= 0)
    {
        MessageBox(_T("Write CT type Fail!"));
    }
}


void CPowerMeterList::OnBnClickedRadioPowerInternalCt()
{
    // TODO: 在此添加控件通知处理程序代码
    int write_ret = 0;
    write_ret = write_one(g_tstat_id, 1070, 0, 3);
    if (write_ret <= 0)
    {
        MessageBox(_T("Write CT type Fail!"));
    }
}


void CPowerMeterList::OnEnChangeEdit2()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}


void CPowerMeterList::OnEnChangeEdit4()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}


void CPowerMeterList::OnEnKillfocusEditRatioValue()
{
    // TODO: 在此添加控件通知处理程序代码
    int write_ret = 0;
    CString tem_value;
    GetDlgItemText(IDC_EDIT_RATIO_VALUE, tem_value);
    float f_value;
    f_value = _wtof(tem_value);
    unsigned short i_value;
    i_value = (unsigned short)f_value * 100;
    write_ret = write_one(g_tstat_id, 1071, i_value, 3);
    if (write_ret <= 0)
    {
        MessageBox(_T("Write CT Ratio Fail!"));
    }

}


void CPowerMeterList::OnStnClickedPowermeterTitle()
{
    // TODO: 在此添加控件通知处理程序代码
}
