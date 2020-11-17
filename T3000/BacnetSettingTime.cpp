// BacnetSettingTime.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingTime.h"
#include "afxdialogex.h"
#include "global_function.h"

extern int pc_time_to_basic_delt ; //用于时间转换 ，各个时区之间。
extern int panel_time_to_basic_delt ;
// CBacnetSettingTime 对话框

IMPLEMENT_DYNAMIC(CBacnetSettingTime, CDialogEx)

CBacnetSettingTime::CBacnetSettingTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_SETTING_TIME, pParent)
{

}

CBacnetSettingTime::~CBacnetSettingTime()
{
}

void CBacnetSettingTime::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_TIME_PICKER, m_cm5_time_picker);
    DDX_Control(pDX, IDC_DATE_PICKER, m_cm5_date_picker);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingTime, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_BAC_TEST, &CBacnetSettingTime::OnBnClickedButtonRefreshTime)
    ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, &CBacnetSettingTime::OnNMKillfocusDatePicker)
    ON_NOTIFY(NM_KILLFOCUS, IDC_TIME_PICKER, &CBacnetSettingTime::OnNMKillfocusTimePicker)
    ON_NOTIFY(NM_SETFOCUS, IDC_DATE_PICKER, &CBacnetSettingTime::OnNMSetfocusDatePicker)
    ON_NOTIFY(NM_SETFOCUS, IDC_TIME_PICKER, &CBacnetSettingTime::OnNMSetfocusTimePicker)
    ON_BN_CLICKED(IDC_BAC_SYNC_LOCAL_PC, &CBacnetSettingTime::OnBnClickedBtnBacSYNCTime)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_TIME_ZONE, &CBacnetSettingTime::OnCbnSelchangeComboBacnetSettingTimeZone)
    ON_BN_CLICKED(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME, &CBacnetSettingTime::OnBnClickedCheckSettingZoneDaylightTime)
    ON_BN_CLICKED(IDC_RADIO_SETTING_SYNC_TIME, &CBacnetSettingTime::OnBnClickedCheckSettingSyncTime)
    ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_TIME_SERVER, &CBacnetSettingTime::OnCbnSelchangeComboBacnetSettingTimeServer)
    ON_BN_CLICKED(IDC_RADIO_SETTING_SYNC_PC, &CBacnetSettingTime::OnBnClickedRadioSettingSyncPc)
    ON_CBN_KILLFOCUS(IDC_COMBO_BACNET_SETTING_TIME_SERVER, &CBacnetSettingTime::OnCbnKillfocusComboBacnetSettingTimeServer)
    ON_BN_CLICKED(IDC_BUTTON_SYNC_TIME, &CBacnetSettingTime::OnBnClickedButtonSyncTime)
END_MESSAGE_MAP()


// CBacnetSettingTime 消息处理程序




void CBacnetSettingTime::OnBnClickedButtonRefreshTime()
{


    int	resend_count = 0;
    do
    {
        resend_count++;
        if (resend_count>20)
            return;
        g_invoke_id = GetPrivateData(g_bac_instance,
            TIME_COMMAND, 0,
            0,
            sizeof(Time_block_mini));
        Sleep(200);
    } while (g_invoke_id<0);

    CString temp_cs_show;
    temp_cs_show.Format(_T("Read time "));
    Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID, g_invoke_id, BacNet_hwd, temp_cs_show);

}


void CBacnetSettingTime::Get_Time_Edit_By_Control()
{
    CString temp_task_info;
    if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub <= 469)
    {
        CTime temp_day;
        m_cm5_date_picker.GetTime(temp_day);

        int temp_year = temp_day.GetYear();
        if (temp_year > 2000)
            temp_year = temp_year - 2000;
        Device_time.old_time.year = temp_year;
        Device_time.old_time.month = temp_day.GetMonth();
        Device_time.old_time.dayofmonth = temp_day.GetDay();
        Device_time.old_time.dayofweek = temp_day.GetDayOfWeek() - 1;
        Device_time.old_time.dayofyear = 1;


        CTime temp_time;
        m_cm5_time_picker.GetTime(temp_time);
        Device_time.old_time.ti_hour = temp_time.GetHour();
        Device_time.old_time.ti_min = temp_time.GetMinute();
        Device_time.old_time.ti_sec = temp_time.GetSecond();

        temp_task_info.Format(_T("Write Device Time.Changed to %02d-%02d %02d:%02d  "),
            Device_time.old_time.month, Device_time.old_time.dayofmonth,
            Device_time.old_time.ti_hour, Device_time.old_time.ti_min);
    }
    else
    {
        CTime temp_start_day;
        CTime temp_start_time;
        m_cm5_date_picker.GetTime(temp_start_day);
        m_cm5_time_picker.GetTime(temp_start_time);
        int start_year = temp_start_day.GetYear();
        int start_month = temp_start_day.GetMonth();
        int start_day = temp_start_day.GetDay();
        int start_hour = temp_start_time.GetHour();
        int start_minute = temp_start_time.GetMinute();
        int start_sec = temp_start_time.GetSecond();
        unsigned long  temp_time_long = time(NULL);
        CTime temp_start(start_year, start_month, start_day, start_hour, start_minute, start_sec);

        Device_time.new_time.n_time = temp_time_long;
        memset(Device_time.new_time.reserved, 0, 3);
    }

}

void CBacnetSettingTime::OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{


    Get_Time_Edit_By_Control();
    *pResult = 0;
}


void CBacnetSettingTime::OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{

    Get_Time_Edit_By_Control();
    *pResult = 0;
}




void CBacnetSettingTime::OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
    Get_Time_Edit_By_Control();
    *pResult = 0;
}


void CBacnetSettingTime::OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
    Get_Time_Edit_By_Control();
    *pResult = 0;
}

void CBacnetSettingTime::OnBnClickedBtnBacSYNCTime()
{
    CTime	TimeTemp;
    CTime temp_time;
    int nyear, nmonth, nday, nhour, nmin, nsec;

    unsigned long  temp_time_long = time(NULL);
    time_t scale_time = temp_time_long;

    if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 469)
    {
        panel_time_to_basic_delt = Device_Basic_Setting.reg.time_zone * 360 / 10;
        //因为本地CDateTimeCtrl 在设置时间的时候 会默认 加上 电脑的时区，但是显示的时候要显示 设备所选时区，所以 要 变换.
        if (Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
            scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt;
        else if (Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
        {
            CTime	temptimevalue;
            time_t temp_t_time;
            temp_t_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt;
            temptimevalue = temp_t_time;
            int temp_month = temptimevalue.GetMonth();
            int temp_day = temptimevalue.GetDay();
            int day_of_year = temp_month * 30 + temp_day;
            if ((day_of_year > 135) && (day_of_year < 255))
            {
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt + 3600; //如果选中夏令时 需要显示的时候加一个小时
            }
            else
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt; //如果选中夏令时 需要显示的时候加一个小时
        }
            
        else
            scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt; // 其他值当作没有夏令时处理.

                                                                                            //如果本地是夏令时  要求传递给 设备的是 不带夏令时的时间.
                                                                                            //if (DaylightBias != 0)
                                                                                            //{
                                                                                            //    scale_time = scale_time - 3600;
                                                                                            //}

    }

    temp_time = scale_time;

    if (temp_time.GetYear()<2000)
        nyear = temp_time.GetYear() + 2000;
    else
        nyear = temp_time.GetYear();
    nmonth = temp_time.GetMonth();
    nday = temp_time.GetDay();
    nhour = temp_time.GetHour();
    nmin = temp_time.GetMinute();
    nsec = temp_time.GetSecond();
    TimeTemp = CTime(nyear, nmonth, nday, nhour, nmin, nsec);

    m_cm5_time_picker.SetFormat(_T("HH:mm"));
    m_cm5_time_picker.SetTime(&TimeTemp);

    m_cm5_date_picker.SetTime(&TimeTemp);

    Get_Time_Edit_By_Control();

    //TSTAT10 有时候 写同步时间的时候 会通讯挂掉;
    //暂时屏蔽TSTAT10 同步时间 后就不通讯;
   // if (g_selected_product_id == PM_TSTAT10)
    //    return;
    if (WritePrivateData_Blocking(g_bac_instance, WRITE_TIMECOMMAND, 0, 0, sizeof(Time_block_mini)) < 0)
    {
        MessageBox(_T("An error occurred while device synchronizing with local PC. This operation returned because the timeout period expired."));
    }
    else
    {
        MessageBox(_T("The clock was successfully synchronized with local PC."));
    }
}

void CBacnetSettingTime::OnBnClickedBtnBacWriteTime()
{


    Get_Time_Edit_By_Control();

}


void CBacnetSettingTime::OnCbnSelchangeComboBacnetSettingTimeZone()
{

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->GetLBText(nSel, temp_string);
    if (nSel >= sizeof(Time_Zone_Value) / sizeof(Time_Zone_Value[0]))
        return;
    Device_Basic_Setting.reg.time_zone = Time_Zone_Value[nSel];

    CString temp_task_info;
    temp_task_info.Format(_T("Change Time Zone to "));
    temp_task_info = temp_task_info + temp_string;
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
    Sleep(1000);
    OnBnClickedButtonRefreshTime();
}


void CBacnetSettingTime::OnBnClickedCheckSettingZoneDaylightTime()
{


    CString temp_task_info;
    if (Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
    {
        Device_Basic_Setting.reg.time_zone_summer_daytime = 0;
        temp_task_info.Format(_T("Disable Automatically adjust clock for Daylight Saving Time "));
    }
    else
    {
        Device_Basic_Setting.reg.time_zone_summer_daytime = 1;
        temp_task_info.Format(_T("Enable Automatically adjust clock for Daylight Saving Time"));
    }

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);

    Sleep(1000);
    OnBnClickedButtonRefreshTime();
}


#include "BacnetSetting.h"
void CBacnetSettingTime::OnBnClickedButtonSyncTime()
{


    Device_Basic_Setting.reg.reset_default = 99;
    CString temp_task_info;
    temp_task_info.Format(_T("SYNC Time Server "));
    ((CButton *)GetDlgItem(IDC_BUTTON_SYNC_TIME))->EnableWindow(FALSE);
    SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("SYNC Timer Server Now.Please Wait....."));
    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        Sleep(1);
    }
    else
    {
        ((CBacnetSetting *)pDialog[WINDOW_SETTING])->SetTimer(1, 4000, NULL);
    }



}





void CBacnetSettingTime::OnCbnKillfocusComboBacnetSettingTimeServer()
{

    CString temp_string;
    GetDlgItemText(IDC_COMBO_BACNET_SETTING_TIME_SERVER, temp_string);
    if (!temp_string.IsEmpty())
    {
        if ((temp_string.CompareNoCase(Time_Server_Name[0]) == 0) ||
            (temp_string.CompareNoCase(Time_Server_Name[1]) == 0) ||
            (temp_string.CompareNoCase(Time_Server_Name[2]) == 0))
        {
            if (temp_string.CompareNoCase(Time_Server_Name[0]) == 0)
            {
                Device_Basic_Setting.reg.en_sntp = 2;
            }
            else if (temp_string.CompareNoCase(Time_Server_Name[1]) == 0)
            {
                Device_Basic_Setting.reg.en_sntp = 3;
            }
            else if (temp_string.CompareNoCase(Time_Server_Name[2]) == 0)
            {
                Device_Basic_Setting.reg.en_sntp = 4;
            }
            CString temp_task_info;
            temp_task_info.Format(_T("Change sntp server "));
            Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
        }
        else
        {
            Device_Basic_Setting.reg.en_sntp = 5;
            WideCharToMultiByte(CP_ACP, NULL, temp_string.GetBuffer(), -1, Device_Basic_Setting.reg.sntp_server, 30, NULL, NULL);
            CString temp_task_info;
            temp_task_info.Format(_T("Change sntp server to "));
            temp_task_info = temp_task_info + temp_string;
            Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
        }
    }



}


void CBacnetSettingTime::OnBnClickedRadioSettingSyncPc()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Synchronize with Local PC "));

    //GetDlgItem(IDC_DATE_PICKER)->EnableWindow(true);
    //GetDlgItem(IDC_TIME_PICKER)->EnableWindow(true);
    GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(true);
    GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(false);

    Device_Basic_Setting.reg.time_sync_auto_manual = 1;

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);

}




void CBacnetSettingTime::OnCbnSelchangeComboBacnetSettingTimeServer()
{

    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->GetLBText(nSel, temp_string);
    if (nSel == 0)
        Device_Basic_Setting.reg.en_sntp = 2;
    else if (nSel == 1)
        Device_Basic_Setting.reg.en_sntp = 3;
    else if (nSel == 2)
        Device_Basic_Setting.reg.en_sntp = 4;
    else if (nSel == 3)
        Device_Basic_Setting.reg.en_sntp = 5;
    else if (nSel == 4)
        Device_Basic_Setting.reg.en_sntp = 6;
    ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
    CString temp_task_info;
    temp_task_info.Format(_T("Change sntp time server to "));
    temp_task_info = temp_task_info + temp_string;
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);

}


void CBacnetSettingTime::OnBnClickedCheckSettingSyncTime()
{

    CString temp_task_info;
    temp_task_info.Format(_T("Change configuration to Synchronize with the time server "));

    GetDlgItem(IDC_DATE_PICKER)->EnableWindow(false);
    GetDlgItem(IDC_TIME_PICKER)->EnableWindow(false);
    GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(false);
    GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(true);

    Device_Basic_Setting.reg.time_sync_auto_manual = 0;
    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);



    if (Device_Basic_Setting.reg.en_sntp == 2)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
        ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(0);
    }
    if (Device_Basic_Setting.reg.en_sntp == 3)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
        ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(1);
    }
    else if (Device_Basic_Setting.reg.en_sntp == 4)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
        ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(2);
    }

}



BOOL CBacnetSettingTime::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        int temp_focus_id = GetFocus()->GetDlgCtrlID();
        if (
            (temp_focus_id == IDC_EDIT_TIME_UPDATE) ||
            (temp_focus_id == IDC_COMBO_BACNET_SETTING_TIME_SERVER))
        {

            GetDlgItem(IDC_EDIT_TIME_UPDATE)->SetFocus();
        }
        return 1;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CBacnetSettingTime::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME)->SetFocus();
    return FALSE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
