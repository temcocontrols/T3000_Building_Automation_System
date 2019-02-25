// BacnetSetting.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSetting.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "Bacnetaddintodb.h"
#include "BacnetATCommand.h"
#include "BacnetSettingHealth.h"
#include "TstatZigbeeLogic.h"
#include "BacnetIOConfig.h"
#include "MainFrm.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "BacnetTstatSchedule.h"
#include "ping.h"
#include "ShowMessageDlg.h"
#include "BacnetTstatSchedule.h"
// CBacnetSetting dialog
extern bool cancle_send ;
bool show_user_list_window = false;
CBacnetTstatSchedule *BacnetTstatSchedule_Window = NULL;
LONG n_tempBias;
LONG DaylightBias = 0;
int pc_time_to_basic_delt = 0; //用于时间转换 ，各个时区之间。
int panel_time_to_basic_delt = 0; 
#define TIMER_SYNC_TIMER    1
#define TIMER_REFRESH_READ    2
#define TIMER_IP_CHANGED_RECONNECT 3
#define TIMER_CONTROL_BUTTON       4
#define TIMER_REFRESH_READ_DELAY    15000

IMPLEMENT_DYNAMIC(CBacnetSetting, CDialogEx)

CBacnetSetting::CBacnetSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetSetting::IDD, pParent)
{
		window_max = true;
}

CBacnetSetting::~CBacnetSetting()
{
}

void CBacnetSetting::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TIME_PICKER, m_cm5_time_picker);
    DDX_Control(pDX, IDC_DATE_PICKER, m_cm5_date_picker);
    DDX_Control(pDX, IDC_COMBO_BACNET_SETTING_COM0, m_device_com0);
    DDX_Control(pDX, IDC_STATIC_SETTING_PANEL_TITLE, m_static_panel_title);
    DDX_Control(pDX, IDC_EDIT_SETTING_PANEL, m_edit_panel);
    DDX_Control(pDX, IDC_STATIC_SETTING_NODES_LABEL_TITLE, m_static_nodes_title);
    DDX_Control(pDX, IDC_EDIT_SETTING_NODES_LABEL_SETTING, m_edit_nodes_label);
    DDX_Control(pDX, IDC_EDIT_DYNDNS_USER_NAME, m_dyndns_user);
    DDX_Control(pDX, IDC_EDIT_DYNDNS_PASSWORD, m_dyndns_password);
    DDX_Control(pDX, IDC_EDIT_DYNDNS_DOMAIN, m_dyndns_domain);
    DDX_Control(pDX, IDC_EDIT_TIME_UPDATE, m_edit_ddns_update_time);
    DDX_Control(pDX, IDC_EDIT_SETTING_PORT, m_edit_port);
    DDX_Control(pDX, IDC_EDIT_SETTING_OBJ_INSTANCE, m_setting_obj_instance);
    DDX_Control(pDX, IDC_EDIT_SETTING_MODBUS_ID, m_edit_modbus_id);
    DDX_Control(pDX, IDC_EDIT_NETWORK_SUB, m_network_sub);
    DDX_Control(pDX, IDC_EDIT_NETWORK_ZIGBEE, m_network_zigbee);
    DDX_Control(pDX, IDC_EDIT_NETWORK_MAIN, m_network_main);
}


BEGIN_MESSAGE_MAP(CBacnetSetting, CDialogEx)
	ON_MESSAGE(WM_FRESH_SETTING_UI,Fresh_Setting_UI)
	ON_BN_CLICKED(IDC_BUTTON_BAC_TEST, &CBacnetSetting::OnBnClickedButtonBacTest)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, &CBacnetSetting::OnNMKillfocusDatePicker)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TIME_PICKER, &CBacnetSetting::OnNMKillfocusTimePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATE_PICKER, &CBacnetSetting::OnNMSetfocusDatePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_PICKER, &CBacnetSetting::OnNMSetfocusTimePicker)
	ON_BN_CLICKED(IDC_BAC_SYNC_LOCAL_PC, &CBacnetSetting::OnBnClickedBtnBacSYNCTime)
	ON_BN_CLICKED(IDC_RADIO_BAC_IP_AUTO, &CBacnetSetting::OnBnClickedBtnBacIPAuto)
	ON_BN_CLICKED(IDC_RADIO_BAC_IP_STATIC, &CBacnetSetting::OnBnClickedBtnBacIPStatic)
	ON_BN_CLICKED(IDC_BUTTON_BAC_IP_CHANGED, &CBacnetSetting::OnBnClickedBtnBacIPChange)
	ON_WM_TIMER()
	ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM0, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom0)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM1, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom1)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM2, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom2)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_LDF, &CBacnetSetting::OnBnClickedButtonSettingLdf)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE0, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate0)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE1, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate1)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE2, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate2)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEAN_DB, &CBacnetSetting::OnBnClickedButtonSettingCleanDb)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADD_INTO_DB, &CBacnetSetting::OnBnClickedButtonSettingAddIntoDb)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_USER_LIST, &CBacnetSetting::OnBnClickedButtonSettingUserList)
	ON_BN_CLICKED(IDC_RADIO_USB_DEVICE, &CBacnetSetting::OnBnClickedRadioUsbDevice)
	ON_BN_CLICKED(IDC_RADIO_USB_HOST, &CBacnetSetting::OnBnClickedRadioUsbHost)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_GSM_MODUAL, &CBacnetSetting::OnBnClickedButtonSettingGsmModual)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PANEL, &CBacnetSetting::OnEnKillfocusEditSettingPanel)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_NODES_LABEL_SETTING, &CBacnetSetting::OnEnKillfocusEditSettingNodesLabelSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_TIME_SERVER, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeServer)
	ON_BN_CLICKED(IDC_RADIO_SETTING_SYNC_TIME, &CBacnetSetting::OnBnClickedCheckSettingSyncTime)
	ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_USER_NAME, &CBacnetSetting::OnEnKillfocusEditDyndnsUserName)
	ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_PASSWORD, &CBacnetSetting::OnEnKillfocusEditDyndnsPassword)
	ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_DOMAIN, &CBacnetSetting::OnEnKillfocusEditDyndnsDomain)
	ON_BN_CLICKED(IDC_CHECK_SETTING_DYNDNS, &CBacnetSetting::OnBnClickedCheckSettingDyndns)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_DDNS_SERVER, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingDdnsServer)
	ON_EN_KILLFOCUS(IDC_EDIT_TIME_UPDATE, &CBacnetSetting::OnEnKillfocusEditTimeUpdate)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_TIME_ZONE, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeZone)
	ON_BN_CLICKED(IDC_CHECK_SETTING_PAP, &CBacnetSetting::OnBnClickedCheckSettingPap)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PORT, &CBacnetSetting::OnEnKillfocusEditSettingPort)
	ON_BN_CLICKED(IDC_BUTTON_HEALTH, &CBacnetSetting::OnBnClickedButtonHealth)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_OBJ_INSTANCE, &CBacnetSetting::OnEnKillfocusEditSettingObjInstance)
	ON_WM_HELPINFO()

	ON_BN_CLICKED(IDC_BUTTON_BAC_SETTING_OK, &CBacnetSetting::OnBnClickedButtonBacSettingOk)
	ON_BN_CLICKED(IDC_BUTTON_BAC_SETTING_CANCEL, &CBacnetSetting::OnBnClickedButtonBacSettingCancel)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_TIME, &CBacnetSetting::OnBnClickedButtonSyncTime)
	ON_BN_CLICKED(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME, &CBacnetSetting::OnBnClickedCheckSettingZoneDaylightTime)
	ON_BN_CLICKED(IDC_BUTTON_BAC_SHOW_ZIGBEE, &CBacnetSetting::OnBnClickedButtonBacShowZigbee)
	ON_CBN_KILLFOCUS(IDC_COMBO_BACNET_SETTING_TIME_SERVER, &CBacnetSetting::OnCbnKillfocusComboBacnetSettingTimeServer)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MODBUS_ID, &CBacnetSetting::OnEnKillfocusEditSettingModbusId)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SETTING_IO_CONFIG, &CBacnetSetting::OnBnClickedButtonSettingIoConfig)
	ON_BN_CLICKED(IDC_BUTTON_REBOOT_DEVICE, &CBacnetSetting::OnBnClickedButtonRebootDevice)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ZONE_SCHEDULE, &CBacnetSetting::OnBnClickedButtonZoneSchedule)
    ON_BN_CLICKED(IDC_BUTTON_SETTING_DONE, &CBacnetSetting::OnBnClickedButtonSettingDone)
    ON_BN_CLICKED(IDC_RADIO_SETTING_SYNC_PC, &CBacnetSetting::OnBnClickedRadioSettingSyncPc)
    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_ON, &CBacnetSetting::OnBnClickedRadioSettingLcdOn)
    ON_BN_CLICKED(IDC_RADIO_SETTING_LCD_OFF, &CBacnetSetting::OnBnClickedRadioSettingLcdOff)
    ON_EN_KILLFOCUS(IDC_EDIT_NETWORK_SUB, &CBacnetSetting::OnEnKillfocusEditNetworkSub)
    ON_EN_KILLFOCUS(IDC_EDIT_NETWORK_ZIGBEE, &CBacnetSetting::OnEnKillfocusEditNetworkZigbee)
    ON_EN_KILLFOCUS(IDC_EDIT_NETWORK_MAIN, &CBacnetSetting::OnEnKillfocusEditNetworkMain)
   // ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MSTP_ID, &CBacnetSetting::OnEnKillfocusEditSettingMstpId)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_ZEIGBEE_PANID, &CBacnetSetting::OnEnKillfocusEditSettingZeigbeePanid)
    ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MAX_MASTER, &CBacnetSetting::OnEnKillfocusEditSettingMaxMaster)
END_MESSAGE_MAP()


// CBacnetSetting message handlers





void CBacnetSetting::OnBnClickedButtonBacTest()
{
	

	int	resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>20)
			return;
		g_invoke_id = GetPrivateData(g_bac_instance,
			TIME_COMMAND,0,
			0,
			sizeof(Time_block_mini));
		Sleep(200);
	} while (g_invoke_id<0);

	CString temp_cs_show;
	temp_cs_show.Format(_T("Read time "));
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

}


void CBacnetSetting::Get_Time_Edit_By_Control()
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

	Post_Write_Message(g_bac_instance,RESTARTMINI_COMMAND,0,0,sizeof(Time_block_mini),this->m_hWnd,temp_task_info);
}

void CBacnetSetting::OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	

	Get_Time_Edit_By_Control();
	*pResult = 0;
}


void CBacnetSetting::OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	Get_Time_Edit_By_Control();
	*pResult = 0;
}




void CBacnetSetting::OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	Get_Time_Edit_By_Control();
	*pResult = 0;
}


void CBacnetSetting::OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	Get_Time_Edit_By_Control();
	*pResult = 0;
}

void CBacnetSetting::OnBnClickedBtnBacSYNCTime()
{
	CTime	TimeTemp;
	CTime temp_time;
	int nyear,nmonth,nday,nhour,nmin,nsec;

    unsigned long  temp_time_long = time(NULL);

    time_t scale_time = temp_time_long;

    if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 469)
    {
        panel_time_to_basic_delt = Device_Basic_Setting.reg.time_zone * 360 / 10;
        //因为本地CDateTimeCtrl 在设置时间的时候 会默认 加上 电脑的时区，但是显示的时候要显示 设备所选时区，所以 要 变换.
        if (Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
            scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt;
        else if (Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
            scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt + 3600; //如果选中夏令时 需要显示的时候加一个小时
        else
            scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt; // 其他值当作没有夏令时处理.

        //如果本地是夏令时  要求传递给 设备的是 不带夏令时的时间.
        //if (DaylightBias != 0)
        //{
        //    scale_time = scale_time - 3600;
        //}

    }

    temp_time = scale_time;






	//temp_time = CTime::GetCurrentTime();
	if(temp_time.GetYear()<2000)
		nyear = temp_time.GetYear() + 2000;
	else
		nyear = temp_time.GetYear();
	nmonth = temp_time.GetMonth();
	nday = temp_time.GetDay();
	nhour = temp_time.GetHour();
	nmin = temp_time.GetMinute();
	nsec = temp_time.GetSecond();
	TimeTemp = CTime(nyear,nmonth,nday,nhour,nmin,nsec);

	m_cm5_time_picker.SetFormat(_T("HH:mm"));
	m_cm5_time_picker.SetTime(&TimeTemp);

	//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
	m_cm5_date_picker.SetTime(&TimeTemp);

	Get_Time_Edit_By_Control();
    //Device_Basic_Setting.reg.time_zone = n_tempBias;
    Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0);
}

void CBacnetSetting::OnBnClickedBtnBacWriteTime()
{
	

	Get_Time_Edit_By_Control();

}

void CBacnetSetting::OnBnClickedBtnBacIPAuto()
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

void CBacnetSetting::OnBnClickedBtnBacIPStatic()
{
	Device_Basic_Setting.reg.tcp_type = 0;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
}
extern HTREEITEM  hTreeItem_retry ;
void CBacnetSetting::OnBnClickedBtnBacIPChange()
{
#if 0
    unsigned short test_array[1000];
    memset(test_array, 0, 2000);
    int ntest_ret = GetPrivateBacnetToModbusData(95237, 0, 100, test_array);
    Sleep(1000);
    if (ntest_ret > 0)
    {
        CString total_char_test;
        if ((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
        {
            char * temp_print_test = NULL;
            temp_print_test = (char *)&test_array;
            for (int i = 0; i< ntest_ret; i++)
            {
                CString temp_char_test;
                temp_char_test.Format(_T("%02x"), (unsigned char)*temp_print_test);
                temp_char_test.MakeUpper();
                temp_print_test++;
                total_char_test = total_char_test + temp_char_test + _T(" ");
            }
            DFTrace(total_char_test);
        }
    }
    else
    {
        DFTrace(_T("No replay"));
    }

    return;
    //memset(test_array, 0, 2000);
    //memcpy(test_array, "11223344556677889900AABBCC", 46);
    //int ntest_ret =  WritePrivateBacnetToModbusData(g_bac_instance, 10000, 23, test_array);
    //Sleep(2000);

    //memset(test_array, 0, 2000);
    //test_array[0] = 0x04;
    //int ntest_ret = WritePrivateBacnetToModbusData(g_bac_instance, 205, 1, test_array);
    //Sleep(2000);
    //return;
#endif
	BYTE address1,address2,address3,address4;
	BYTE subnet1, subnet2, subnet3, subnet4;
	BYTE gatway1,gatway2,gatway3,gatway4;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->GetAddress(address1,address2,address3,address4);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->GetAddress(subnet1,subnet2,subnet3,subnet4);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->GetAddress(gatway1,gatway2,gatway3,gatway4);


//    if (0 == (address1 != Device_Basic_Setting.reg.ip_addr[0] || address2 != Device_Basic_Setting.reg.ip_addr[1] || address3 != Device_Basic_Setting.reg.ip_addr[2] || address4 != Device_Basic_Setting.reg.ip_addr[3] ||
//        subnet1 != Device_Basic_Setting.reg.subnet[0] || subnet2 != Device_Basic_Setting.reg.subnet[1] || subnet3 != Device_Basic_Setting.reg.subnet[2] || subnet4 != Device_Basic_Setting.reg.subnet[3] ||
//        gatway1 != Device_Basic_Setting.reg.gate_addr[0] || gatway2 != Device_Basic_Setting.reg.gate_addr[1] || gatway3 != Device_Basic_Setting.reg.gate_addr[2] || gatway4 != Device_Basic_Setting.reg.gate_addr[3]) //||
///*        m_tcp_type != Device_Basic_Setting.reg.tcp_type*/)
//    {
//        //fandu 20180201 如果没有变更就不要往设备里写了。
//        return;
//    }

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
		Device_Basic_Setting.reg.subnet[0]  = subnet1;
		Device_Basic_Setting.reg.subnet[1]  = subnet2;
		Device_Basic_Setting.reg.subnet[2]  = subnet3;
		Device_Basic_Setting.reg.subnet[3]  = subnet4;
		Device_Basic_Setting.reg.gate_addr[0] = gatway1;
		Device_Basic_Setting.reg.gate_addr[1] = gatway2;
		Device_Basic_Setting.reg.gate_addr[2] = gatway3;
		Device_Basic_Setting.reg.gate_addr[3] = gatway4;
		bool isstatic = ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->GetCheck(); //返回1表示选上，0表示没选上;
		if(isstatic == true)
			Device_Basic_Setting.reg.tcp_type = 0;
		else
			Device_Basic_Setting.reg.tcp_type = 1;
		//Device_Basic_Setting.reg.tcp_type = isstatic;
		//CString temp_task_info;
		//temp_task_info.Format(_T("Change IP Address Information !"));
		//Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
		{
			CString temp_task_info;
			temp_task_info.Format(_T("Change IP Address Information Timeout!"));
			MessageBox(temp_task_info);
		}
		else
		{
			//在Ip 修改成功后 更新数据库;
            m_tcp_type = Device_Basic_Setting.reg.tcp_type;
			CString strnewipadress;
			strnewipadress.Format(_T("%u.%u.%u.%u"),address1,address2,address3,address4);
			CString temp_task_info;
			temp_task_info.Format(_T("IP address has been changed! \r\nRebooting now! Please wait."));

			CString strSql;
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			CString temp_serial_cs;
			temp_serial_cs.Format(_T("%u"),g_selected_serialnumber);
			strSql.Format(_T("select * from ALL_NODE where Serial_ID = '%s' "),temp_serial_cs);
			//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			while(!q.eof())
			{
				strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Serial_ID= '%s'"),strnewipadress,temp_serial_cs);
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

CString Check_Time(unsigned long local_time , unsigned int device_time)
{
	CString ret_cstring;
	if (local_time < device_time)
	{
		ret_cstring = _T("No Reply");
	}
	else if (device_time + 3600 * 24 * 30 < local_time)
	{
		ret_cstring = _T("1 month ago");
	}
	else if (device_time + 3600 * 24 * 7 < local_time)
	{
		ret_cstring = _T("7 days ago");
	}
	else if (device_time + 3600 * 24 < local_time)
	{
		ret_cstring = _T("1 day ago");
	}
	else if (device_time + 3600 * 12 < local_time)
	{
		ret_cstring = _T(" 12 hours ago");
	}
	else if (device_time + 3600 < local_time)
	{
		ret_cstring = _T(" 1 hour ago");
	}
	else if (device_time + 1800 < local_time)
	{
		ret_cstring = _T(" 30 minutes ago");
	}
	else if (device_time + 600 < local_time)
	{
		ret_cstring = _T(" 10 minutes ago");
	}
	else if (device_time + 300 < local_time)
	{
		ret_cstring = _T(" 5 minutes ago");
	}
	else if (device_time + 60 < local_time)
	{
		ret_cstring = _T(" 1 minute ago");
	}
	else if (device_time < local_time)
	{
		ret_cstring = _T("10 seconds ago");
	}

	return ret_cstring;
}







LRESULT CBacnetSetting::Fresh_Setting_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;
	CString temp_cs;
	int temp_year;
	CTime	TimeTemp;
	CString temp_hw_version = _T("Unknown");
	CString temp_mcu_version = _T("Unknown");
	CString temp_pic_version = _T("Unknown");
	CString temp_c8051_version = _T("Unknown");
	CString temp_5964_version = _T("Unknown");
	CString temp_bootloader_version = _T("Unknown");
	CString temp_serial_number = _T("Unknown");
	
	TIME_ZONE_INFORMATION lp_time_zone;
	memset(&lp_time_zone,0,sizeof(TIME_ZONE_INFORMATION));

	::GetTimeZoneInformation(&lp_time_zone);
     DaylightBias = lp_time_zone.DaylightBias;


	 n_tempBias = 0 - lp_time_zone.Bias;
	 n_tempBias = (n_tempBias*100)/60;
     CTime temp_time;
     temp_time = CTime::GetCurrentTime();
	 
     pc_time_to_basic_delt = n_tempBias * 360 / 10;


	switch(command_type)
	{
	case READ_SETTING_COMMAND:
		{
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->SetAddress(Device_Basic_Setting.reg.ip_addr[0],
				Device_Basic_Setting.reg.ip_addr[1],Device_Basic_Setting.reg.ip_addr[2],Device_Basic_Setting.reg.ip_addr[3]);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->SetAddress(Device_Basic_Setting.reg.subnet[0],
				Device_Basic_Setting.reg.subnet[1],Device_Basic_Setting.reg.subnet[2],Device_Basic_Setting.reg.subnet[3]);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->SetAddress(Device_Basic_Setting.reg.gate_addr[0],
				Device_Basic_Setting.reg.gate_addr[1],Device_Basic_Setting.reg.gate_addr[2],Device_Basic_Setting.reg.gate_addr[3]);
            bacnet_device_type = Device_Basic_Setting.reg.mini_type;
			if(Device_Basic_Setting.reg.tcp_type == 1)
			{
                m_tcp_type = 1;
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(true);
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(false);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);
			}
			else if(Device_Basic_Setting.reg.tcp_type == 0)
			{
                m_tcp_type = 0;
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(false);
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(true);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
			}

			//硬件版本大于 26 代表是arm的版本. 
			if(Device_Basic_Setting.reg.pro_info.harware_rev >= 26)
			{
				GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(TRUE);
			}

			//版本大于38.6 的才有在setting 里面改port 的功能
			if(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 +Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 476)
			{
				CString temp_port;
				temp_port.Format(_T("%u"),Device_Basic_Setting.reg.modbus_port);
				m_edit_port.EnableWindow(true);
				m_edit_port.SetWindowTextW(temp_port);
			}
			else
			{
				m_edit_port.EnableWindow(false);
			}

            //版本大于38.6 的才有在setting 里面改port 的功能
            //if ((Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub) > 470)
            //{
            //    CString temp_value;
            //    m_network_sub.ShowWindow(1);
            //    m_network_zigbee.ShowWindow(1);
            //    m_network_main.ShowWindow(1);
            //    temp_value.Format(_T("%d"), Device_Basic_Setting.reg.network_ID[0]);
            //    m_network_sub.SetWindowTextW(temp_value);
            //    temp_value.Format(_T("%d"), Device_Basic_Setting.reg.network_ID[1]);
            //    m_network_zigbee.SetWindowTextW(temp_value);
            //    temp_value.Format(_T("%d"), Device_Basic_Setting.reg.network_ID[2]);
            //    m_network_main.SetWindowTextW(temp_value);
            //}
            //else
            //{
            //    m_network_sub.EnableWindow(0);
            //    m_network_zigbee.EnableWindow(0);
            //    m_network_main.EnableWindow(0);
            //}
            

            //20180201 fandu  ARM 的 板子才有 zone schedual 这个功能
			//版本大于46.1 的才有在setting 里面改port 的功能
			if ((Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 461) 
                && (Device_Basic_Setting.reg.pro_info.bootloader_rev >= 49))
			{
				GetDlgItem(IDC_BUTTON_ZONE_SCHEDULE)->EnableWindow(true);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_ZONE_SCHEDULE)->EnableWindow(false);
			}

			if(Device_Basic_Setting.reg.en_plug_n_play == 1)
			{
				((CButton *)GetDlgItem(IDC_CHECK_SETTING_PAP))->SetCheck(true);
			}
			else
			{
				((CButton *)GetDlgItem(IDC_CHECK_SETTING_PAP))->SetCheck(false);
			}


			unsigned long time_setting_now = (unsigned long)time(NULL);

			if((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
			{
				CString temp123;
				temp123.Format(_T("%u"),Device_Basic_Setting.reg.time_update_since_1970);
				DFTrace(temp123);
			}

			if((Device_Basic_Setting.reg.time_update_since_1970 < 1420041600)  || (Device_Basic_Setting.reg.time_update_since_1970 > 1735660800))
			{
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(_T("No Reply"));
			}
			else
			{
				//CTime time_scaletime;
				CString strTime;
				time_t scale_time  = Device_Basic_Setting.reg.time_update_since_1970;

				strTime = Check_Time(time_setting_now,Device_Basic_Setting.reg.time_update_since_1970);

				//time_scaletime = scale_time;
				//strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(strTime);
			}


			if(Device_Basic_Setting.reg.en_dyndns == 0)
			{
				GetDlgItem(IDC_CHECK_SETTING_DYNDNS)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_DYNDNS)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SETTING_USER_NAME)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DYNDNS_USER_NAME)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SETTING_PASSWORD)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DYNDNS_PASSWORD)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SETTING_DOMAIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DYNDNS_DOMAIN)->EnableWindow(FALSE);

				GetDlgItem(IDC_STATIC_DDNS_SERVER)->EnableWindow(FALSE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_CHECK_AUTO)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_TIME_UPDATE)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_MIN_TIME)->EnableWindow(FALSE);
	
			}
			else
			{

				GetDlgItem(IDC_CHECK_SETTING_DYNDNS)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_DYNDNS)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_USER_NAME)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_DYNDNS_USER_NAME)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_PASSWORD)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_DYNDNS_PASSWORD)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_DOMAIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_DYNDNS_DOMAIN)->EnableWindow(TRUE);

				GetDlgItem(IDC_STATIC_DDNS_SERVER)->EnableWindow(TRUE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_CHECK_AUTO)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_TIME_UPDATE)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_MIN_TIME)->EnableWindow(TRUE);

				if(Device_Basic_Setting.reg.en_dyndns == 2)
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_DYNDNS))->SetCheck(true);
				}
				else
                {
                Device_Basic_Setting.reg.en_dyndns = 1;
                ((CButton *)GetDlgItem(IDC_CHECK_SETTING_DYNDNS))->SetCheck(false);
                }

                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->ResetContent();
                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[0]);
                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[1]);
                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[2]);
                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[3]);

                //if(Device_Basic_Setting.reg.dyndns_provider == 0)
                //	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[0]);
                //else if(Device_Basic_Setting.reg.dyndns_provider == 1)
                //	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[1]);
                //else if(Device_Basic_Setting.reg.dyndns_provider == 2)
                //	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[2]);
                //else if(Device_Basic_Setting.reg.dyndns_provider == 3)
                //	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[3]);
                //else
                //	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(_T(""));

                if (Device_Basic_Setting.reg.dyndns_provider == 0)
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(0);
                else if (Device_Basic_Setting.reg.dyndns_provider == 1)
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(1);
                else if (Device_Basic_Setting.reg.dyndns_provider == 2)
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(2);
                else if (Device_Basic_Setting.reg.dyndns_provider == 3)
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(3);
                else
                {
                    Device_Basic_Setting.reg.dyndns_provider = 3;
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(3);
                }


                CString temp_time;
                temp_time.Format(_T("%u"), Device_Basic_Setting.reg.dyndns_update_time);
                m_edit_ddns_update_time.SetWindowText(temp_time);

                CString temp_dyndns_user;
                CString temp_dyndns_password;
                CString temp_dyndns_domain;
                MultiByteToWideChar(CP_ACP, 0, (char *)Device_Basic_Setting.reg.dyndns_user, (int)strlen((char *)Device_Basic_Setting.reg.dyndns_user) + 1, temp_dyndns_user.GetBuffer(MAX_PATH), MAX_PATH);
                temp_dyndns_user.ReleaseBuffer();
                temp_dyndns_user = temp_dyndns_user.Left(31);
                temp_dyndns_user = temp_dyndns_user.Trim();
                MultiByteToWideChar(CP_ACP, 0, (char *)Device_Basic_Setting.reg.dyndns_pass, (int)strlen((char *)Device_Basic_Setting.reg.dyndns_pass) + 1, temp_dyndns_password.GetBuffer(MAX_PATH), MAX_PATH);
                temp_dyndns_password.ReleaseBuffer();
                temp_dyndns_password = temp_dyndns_password.Left(31);
                temp_dyndns_password = temp_dyndns_password.Trim();
                MultiByteToWideChar(CP_ACP, 0, (char *)Device_Basic_Setting.reg.dyndns_domain, (int)strlen((char *)Device_Basic_Setting.reg.dyndns_domain) + 1, temp_dyndns_domain.GetBuffer(MAX_PATH), MAX_PATH);
                temp_dyndns_domain.ReleaseBuffer();
                temp_dyndns_domain = temp_dyndns_domain.Left(31);
                temp_dyndns_domain = temp_dyndns_domain.Trim();
                m_dyndns_user.SetWindowTextW(temp_dyndns_user);
                m_dyndns_password.SetWindowTextW(temp_dyndns_password);
                m_dyndns_domain.SetWindowTextW(temp_dyndns_domain);
            }

            if (Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
            {
                ((CButton *)GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME))->SetCheck(false);
            }
            else
            {
                Device_Basic_Setting.reg.time_zone_summer_daytime = 1;
                ((CButton *)GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME))->SetCheck(true);
            }

            if (Device_Basic_Setting.reg.zegbee_exsit == 0x74)
            {
                ((CButton *)GetDlgItem(IDC_BUTTON_BAC_SHOW_ZIGBEE))->EnableWindow(TRUE);
                GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(TRUE);
                ((CEdit *)GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->EnableWindow(TRUE);
                
            }
            else
            {
                ((CButton *)GetDlgItem(IDC_BUTTON_BAC_SHOW_ZIGBEE))->EnableWindow(FALSE);
                GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(FALSE);
                ((CEdit *)GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->EnableWindow(FALSE);
            }

            if (Device_Basic_Setting.reg.LCD_Display == 0)
            {
                //1 常灭
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(false);
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(true);
            }
            else if (Device_Basic_Setting.reg.LCD_Display == 1)
            {
                //1 常亮
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(true);
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(false);
            }

            if ((Device_Basic_Setting.reg.mini_type == PRODUCT_CM5) ||
                Device_Basic_Setting.reg.mini_type == BIG_MINIPANEL)
            {
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->EnableWindow(true);
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->EnableWindow(true);
            }
            else
            {
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->EnableWindow(false);
                ((CButton *)GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->EnableWindow(false);
            }

			if(Device_Basic_Setting.reg.en_sntp == 0)
			{
				GetDlgItem(IDC_STATIC_SETTING_TIMER_SERVER)->ShowWindow(FALSE);
				GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME)->ShowWindow(FALSE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->ShowWindow(FALSE);

				GetDlgItem(IDC_STATIC_SETTING_TIME_ZONE)->ShowWindow(FALSE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE)->ShowWindow(FALSE);			
			}
            else
            {
                GetDlgItem(IDC_STATIC_SETTING_TIMER_SERVER)->ShowWindow(TRUE);
                GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME)->ShowWindow(TRUE);
                GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->ShowWindow(TRUE);
                GetDlgItem(IDC_STATIC_SETTING_TIME_ZONE)->ShowWindow(TRUE);
                GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE)->ShowWindow(TRUE);

                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->ResetContent();
                for (int j = 0;j < sizeof(Time_Server_Name) / sizeof(Time_Server_Name[0]);j++)
                {
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->AddString(Time_Server_Name[j]);
                }
                ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->ResetContent();
                for (int z = 0;z < sizeof(Time_Zone_Name) / sizeof(Time_Zone_Name[0]);z++)
                {
                    ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->AddString(Time_Zone_Name[z]);
                    if (Device_Basic_Setting.reg.time_zone == Time_Zone_Value[z])
                        ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->SetWindowTextW(Time_Zone_Name[z]);
                }

                if (Device_Basic_Setting.reg.time_sync_auto_manual == 0)
                {
                    GetDlgItem(IDC_DATE_PICKER)->EnableWindow(false);
                    GetDlgItem(IDC_TIME_PICKER)->EnableWindow(false);
                    GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(false);

                    GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->EnableWindow(true);
                    GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(true);

                    ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
                    ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_PC))->SetCheck(false);

                    if ((Device_Basic_Setting.reg.en_sntp == 2) ||
                        (Device_Basic_Setting.reg.en_sntp == 3) ||
                        (Device_Basic_Setting.reg.en_sntp == 4) ||
                        (Device_Basic_Setting.reg.en_sntp == 5))
                    {


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
                        if (Device_Basic_Setting.reg.en_sntp == 5)
                        {
                            ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);

                            CString temp_cs1;
                            MultiByteToWideChar(CP_ACP, 0, Device_Basic_Setting.reg.sntp_server, (int)strlen((char *)Device_Basic_Setting.reg.sntp_server) + 1,
                                temp_cs1.GetBuffer(MAX_PATH), MAX_PATH);
                            temp_cs1.ReleaseBuffer();

                            ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->AddString(temp_cs1);
                            ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(3);
                        }
                    }
                }
                else if (Device_Basic_Setting.reg.time_sync_auto_manual == 1)
                {
                    //GetDlgItem(IDC_DATE_PICKER)->EnableWindow(true);
                    //GetDlgItem(IDC_TIME_PICKER)->EnableWindow(true);
                    GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(true);
                    GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->EnableWindow(false);
                    GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(false);


                    ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(false);
                    ((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_PC))->SetCheck(true);
                }

            }
			CString temp_panel_number;
			CString temp_nodes_label;
			if(Device_Basic_Setting.reg.en_panel_name)
			{
				temp_panel_number.Format(_T("%u"),Device_Basic_Setting.reg.panel_number);
				MultiByteToWideChar( CP_ACP, 0, (char *)Device_Basic_Setting.reg.panel_name, 	(int)strlen((char *)Device_Basic_Setting.reg.panel_name)+1,temp_nodes_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_nodes_label.ReleaseBuffer();	

			}
			else
			{
				temp_panel_number.Format(_T("%u"),bac_gloab_panel);
				CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
				if(selected_product_index <  pFrame->m_product.size())
				{
					temp_nodes_label = pFrame->m_product.at(selected_product_index).NameShowOnTree;
				}
				else
				{
					temp_nodes_label.Empty();
				}
				
			}
			m_edit_panel.SetWindowTextW(temp_panel_number);
			m_edit_nodes_label.SetWindowTextW(temp_nodes_label);

			if(Device_Basic_Setting.reg.usb_mode == 1)
			{
				((CButton *)GetDlgItem(IDC_RADIO_USB_DEVICE))->SetCheck(false);
				((CButton *)GetDlgItem(IDC_RADIO_USB_HOST))->SetCheck(true);
				(CButton *)GetDlgItem(IDC_BUTTON_SETTING_GSM_MODUAL)->ShowWindow(TRUE);
			}
			else if(Device_Basic_Setting.reg.usb_mode == 0)
			{
				((CButton *)GetDlgItem(IDC_RADIO_USB_DEVICE))->SetCheck(true);
				((CButton *)GetDlgItem(IDC_RADIO_USB_HOST))->SetCheck(false);
				(CButton *)GetDlgItem(IDC_BUTTON_SETTING_GSM_MODUAL)->ShowWindow(FALSE);

			}

			if(Device_Basic_Setting.reg.sd_exist == SD_STATUS_NO)
			{
				((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("No SD Card"));
			}
			else if(Device_Basic_Setting.reg.sd_exist == SD_STATUS_NORMAL)
			{
				((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("Normal"));
			}
            else if (Device_Basic_Setting.reg.sd_exist == SD_STATUS_FILESYSTEM_ERROR)
            {
                ((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("File System error,Only supprot FAT32"));
            }
			else
			{
				((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("unknown"));
			}

			if((bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM || bacnet_device_type == MINIPANELARM_LB || bacnet_device_type == MINIPANELARM_TB) || (bacnet_device_type == SMALL_MINIPANEL) || (bacnet_device_type == TINY_MINIPANEL) || (bacnet_device_type == TINY_EX_MINIPANEL) || (bacnet_device_type == PRODUCT_CM5))
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[NOUSE]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[NOUSE]);
				//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MSTP]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[NOUSE]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MAIN_MSTP]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MSTP_MASTER]);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->EnableWindow(TRUE);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->EnableWindow(TRUE);
				if(Device_Basic_Setting.reg.com0_config < MAX_COM_TYPE)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com0_config]);
				if(Device_Basic_Setting.reg.com1_config < MAX_COM_TYPE)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com1_config]);
				if(Device_Basic_Setting.reg.com2_config < MAX_COM_TYPE)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com2_config]);


				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->EnableWindow(1);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(1);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->EnableWindow(1);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->ResetContent();

				//Fandu 2017-12-07  新的arm板子的com0 支持所有的波特率.
				if ((Device_Basic_Setting.reg.mini_type == MINIPANELARM) ||
					(Device_Basic_Setting.reg.mini_type == MINIPANELARM_LB) ||
					(Device_Basic_Setting.reg.mini_type == MINIPANELARM_TB))
				{
					for (int x = 0;x< (sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]));x++)
					{
						((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[x]);
					}
				}
				else
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[UART_9600]);
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[UART_19200]);
				}



				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->AddString(Baudrate_Array[UART_19200]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(FALSE);
				for (int x=0;x< (sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]));x++)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->AddString(Baudrate_Array[x]);
				}

				//Fandu 2017-12-07  新的arm板子的com0 支持所有的波特率.
				if ((Device_Basic_Setting.reg.mini_type == MINIPANELARM) ||
					(Device_Basic_Setting.reg.mini_type == MINIPANELARM_LB) ||
					(Device_Basic_Setting.reg.mini_type == MINIPANELARM_TB))
				{
					if (Device_Basic_Setting.reg.com_baudrate0 < sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]))
						((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(Device_Basic_Setting.reg.com_baudrate0);
				}
				else
				{
					if (Device_Basic_Setting.reg.com_baudrate0 == UART_9600)
					{
						((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(0);
					}
					else if (Device_Basic_Setting.reg.com_baudrate0 == UART_19200)
					{
						((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(1);
					}
				}



				if(Device_Basic_Setting.reg.com_baudrate1 == UART_19200)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->SetCurSel(0);
				}
				else
				{
					Device_Basic_Setting.reg.com_baudrate1 = UART_19200;
				}
					
				if(Device_Basic_Setting.reg.com_baudrate2 < sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]))
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->SetCurSel(Device_Basic_Setting.reg.com_baudrate2);

			}

            
			if(Device_Basic_Setting.reg.mini_type == PRODUCT_CM5)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("CM5"));
			}
			else if (Device_Basic_Setting.reg.mini_type == MINIPANELARM)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-BB"));
			}
			else if (Device_Basic_Setting.reg.mini_type == MINIPANELARM_LB)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-LB"));
			}
			else if (Device_Basic_Setting.reg.mini_type == MINIPANELARM_TB)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-TB"));
			}
			else if (Device_Basic_Setting.reg.mini_type == BIG_MINIPANEL)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-BB"));
			}
			else if(Device_Basic_Setting.reg.mini_type == SMALL_MINIPANEL)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-LB"));
			}
			else if((Device_Basic_Setting.reg.mini_type == TINY_MINIPANEL) || (Device_Basic_Setting.reg.mini_type == TINY_EX_MINIPANEL))
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-TB"));
			}
			else
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("Unknown device"));
			}

			//((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3Controller"));

			if(
				(bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM) ||
				(Device_Basic_Setting.reg.mini_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB) ||
				(Device_Basic_Setting.reg.mini_type == TINY_MINIPANEL || bacnet_device_type == MINIPANELARM_TB) ||
				(Device_Basic_Setting.reg.mini_type == TINY_EX_MINIPANEL) ||
				(Device_Basic_Setting.reg.mini_type == PRODUCT_CM5)
				
				)
			{
				temp_hw_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.harware_rev);
				temp_pic_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware1_rev);
				temp_c8051_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware2_rev);
				temp_5964_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware3_rev);	
				temp_mcu_version.Format(_T("%d.%d"),Device_Basic_Setting.reg.pro_info.firmware0_rev_main,Device_Basic_Setting.reg.pro_info.firmware0_rev_sub);
				temp_bootloader_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.bootloader_rev);
			}
			//temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
			if(  ((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) *10   +  (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 417)
				temp_serial_number.Format(_T("%u"),Device_Basic_Setting.reg.n_serial_number);
			else
				temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_HARDWARE_VERSION))->SetWindowTextW(temp_hw_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_MCU_VERSION))->SetWindowTextW(temp_mcu_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_PIC_VERSION))->SetWindowTextW(temp_pic_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_C8051_VERSION))->SetWindowTextW(temp_c8051_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_SM5964_VERSION2))->SetWindowTextW(temp_5964_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_BOOTLOADER_VERSION))->SetWindowTextW(temp_bootloader_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_SERIAL_NUMBER_2))->SetWindowTextW(temp_serial_number);

			if((Device_Basic_Setting.reg.user_name == 2) || (Device_Basic_Setting.reg.user_name = 1))
			{
				GetDlgItem(IDC_BUTTON_SETTING_USER_LIST)->ShowWindow(true);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_SETTING_USER_LIST)->ShowWindow(false);
			}
			CString temp_object;
			CString temp_mac_address;
			CString temp_mstp_network;
			CString temp_bip_network;
			CString temp_modbus_id;
            CString temp_mstp_id;
			temp_object.Format(_T("%u"),Device_Basic_Setting.reg.object_instance);
			temp_mac_address.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"),Device_Basic_Setting.reg.mac_addr[0],Device_Basic_Setting.reg.mac_addr[1],Device_Basic_Setting.reg.mac_addr[2],
															Device_Basic_Setting.reg.mac_addr[3],Device_Basic_Setting.reg.mac_addr[4],Device_Basic_Setting.reg.mac_addr[5]);
			temp_mac_address.MakeUpper();
			temp_mstp_network.Format(_T("%u"),Device_Basic_Setting.reg.mstp_network_number);
			temp_bip_network.Format(_T("%u"),Device_Basic_Setting.reg.network_number);
			if(Device_Basic_Setting.reg.modbus_id == 0)
			{
				temp_modbus_id.Empty();
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->EnableWindow(FALSE);
			}
			else
			{
				temp_modbus_id.Format(_T("%d"),Device_Basic_Setting.reg.modbus_id);
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->EnableWindow(TRUE);
			}

            temp_mstp_id.Format(_T("%d"), Device_Basic_Setting.reg.mstp_id);

			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_OBJ_INSTANCE))->SetWindowTextW(temp_object);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MAC_ADDRESS))->SetWindowTextW(temp_mac_address);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MSTP_NETWORK))->SetWindowTextW(temp_mstp_network);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_BIP_NETWORK2))->SetWindowTextW(temp_bip_network);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->SetWindowTextW(temp_modbus_id);

            CString cszigbeeanid;
            cszigbeeanid.Format(_T("%d"), Device_Basic_Setting.reg.zigbee_panid);
            ((CEdit *)GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->SetWindowTextW(cszigbeeanid);
            
            CString csmax_master;
            csmax_master.Format(_T("%u"), Device_Basic_Setting.reg.max_master);
            ((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MAX_MASTER))->SetWindowTextW(csmax_master);
            
		}
		break;
	case TIME_COMMAND:
		{

        //20180108 Fan判断 minipanel 的大体时间是否一致，不一致自动更改;
        if (Device_Basic_Setting.reg.time_sync_auto_manual == 1)
        {
            if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 469)
            {
                unsigned long  temp_time_long = time(NULL);
                if (abs(long(temp_time_long - Device_time.new_time.n_time)) > 600)
                    OnBnClickedBtnBacSYNCTime();
            }
            else
            {
                //OnBnClickedBtnBacSYNCTime();
            }

        }


			#pragma  region about_time

        if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub <= 469)
        {
            if (Device_time.old_time.year < 2000)
                temp_year = Device_time.old_time.year + 2000;
            if ((Device_time.old_time.month == 0) || (Device_time.old_time.dayofmonth == 0))
                return 1;
            TimeTemp = CTime(temp_year, Device_time.old_time.month, Device_time.old_time.dayofmonth, Device_time.old_time.ti_hour, Device_time.old_time.ti_min, Device_time.old_time.ti_sec);
        }
        else
        {
            unsigned long  temp_time_long123    = time(NULL);
            unsigned long  temp_time_long = Device_time.new_time.n_time;


            panel_time_to_basic_delt  = Device_Basic_Setting.reg.time_zone * 360 / 10;

            //因为本地CDateTimeCtrl 在设置时间的时候 会默认 加上 电脑的时区，但是显示的时候要显示 设备所选时区，所以 要 变换.
            time_t scale_time;
            if(Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt;
            else if(Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt + 3600; //如果选中夏令时 需要显示的时候加一个小时
            else
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt; // 其他值当作没有夏令时处理.
            TimeTemp = scale_time;
        }
        //减去系统现在的时区 ，然后在加上 minipanel自己的时区.


			m_cm5_time_picker.SetFormat(_T("HH:mm"));
			m_cm5_time_picker.SetTime(&TimeTemp);
			//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
			m_cm5_date_picker.SetTime(&TimeTemp);
			#pragma endregion about_time
		}
		break;
	default: 
		break;
	}
	//GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->SetFocus();
	return 0;
}




BOOL CBacnetSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	InitScrollbar();
	SetWindowTextW(_T("Setting"));
	m_setting_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_setting_dlg_hwnd;
	m_cm5_date_picker.EnableWindow(0);
	m_cm5_time_picker.EnableWindow(0);
	GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(1);
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(FALSE);
	m_cm5_time_picker.SetFormat(_T("HH:mm"));

	HICON hIcon = NULL; 
	HINSTANCE hInstResource    = NULL; 
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_OK), RT_GROUP_ICON); 
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_OK), IMAGE_ICON, 24, 24, 0); 
	((CButton *)GetDlgItem(IDC_BUTTON_BAC_IP_CHANGED))->SetIcon(hIcon);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_SETTING);
	SetIcon(m_hIcon,TRUE);
	ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BAC_SETTING_OK)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetSetting::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		int temp_focus_id = GetFocus()->GetDlgCtrlID();
		if((temp_focus_id == IDC_EDIT_SETTING_PANEL ) ||
		   (temp_focus_id == IDC_EDIT_SETTING_NODES_LABEL_SETTING) ||
			(temp_focus_id == IDC_EDIT_DYNDNS_USER_NAME) ||
			(temp_focus_id == IDC_EDIT_DYNDNS_PASSWORD) ||
			(temp_focus_id == IDC_EDIT_DYNDNS_DOMAIN) ||
			(temp_focus_id == IDC_EDIT_TIME_UPDATE) ||
			(temp_focus_id == IDC_EDIT_SETTING_PORT) ||
			(temp_focus_id == IDC_EDIT_SETTING_OBJ_INSTANCE) ||
			(temp_focus_id == IDC_COMBO_BACNET_SETTING_TIME_SERVER))
		{

			GetDlgItem(IDC_BUTTON_BAC_SETTING_OK)->SetFocus();
		}
		return 1;
	}
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 100,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetSetting::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case TIMER_SYNC_TIMER:
		{
			   ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SETTING);
			   KillTimer(TIMER_SYNC_TIMER);
               //判断同步结果是否成功
               if(Device_Basic_Setting.reg.sync_time_results != 1)
			   {
				   MessageBox(_T("SYNC time failed , No Reply from server"));
			   }
			   else
			   {
				   CTime time_scaletime;
				   CString strTime;
				   time_t scale_time  = Device_Basic_Setting.reg.time_update_since_1970;
				   time_scaletime = scale_time;
				   strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");
				    SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("SYNC time sucess")); 
					((CEdit *)GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(strTime);
			   }
               ((CButton *)GetDlgItem(IDC_BUTTON_SYNC_TIME))->EnableWindow(TRUE);

		}
		break;
	case TIMER_REFRESH_READ:
	{
		KillTimer(TIMER_REFRESH_READ);
		if (this->IsWindowVisible())
		{

			if (g_protocol == MODBUS_RS485)
			{
				::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, 0, READ_SETTING_COMMAND);//第二个参数 In
			}
			else
			{
				if (bac_select_device_online)
					::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_SETTING);
			}
		}

	}
	break;
    case TIMER_IP_CHANGED_RECONNECT:
    {
        m_reboot_time_left--;
        if (m_reboot_time_left <= 0)
        {
            KillTimer(TIMER_IP_CHANGED_RECONNECT);
            CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
            ::PostMessage(pFrame->m_hWnd, MY_RETRY_MESSAGE, NULL, NULL);
        }
        else
        {
            CString Show_Results;
            Show_Results.Format(_T("System is restarting %d . Please wait!"), m_reboot_time_left);
            SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
        }
    }
        break;
    case TIMER_CONTROL_BUTTON:
        KillTimer(TIMER_CONTROL_BUTTON);
        GetDlgItem(IDC_BUTTON_SETTING_USER_LIST)->EnableWindow(TRUE);
        break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}



// CBacnetInput message handlers
LRESULT  CBacnetSetting::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	else
	{
		//memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//还原没有改对的值
		PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//这里调用 刷新线程重新刷新会方便一点;
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	KillTimer(TIMER_REFRESH_READ);
	SetTimer(TIMER_REFRESH_READ, TIMER_REFRESH_READ_DELAY, NULL);

	if(pInvoke)
		delete pInvoke;
	return 0;
}




void CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom0()
{
	
	UpdateData();
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->GetCurSel();	
	m_device_com0.GetLBText(nSel,temp_string);


	for(int i=0;i<MAX_COM_TYPE;i++)
	{
		if(temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0 )
		{
			Device_Basic_Setting.reg.com0_config = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 0 "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom1()
{
	

	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->GetLBText(nSel,temp_string);
	for(int i=0;i<MAX_COM_TYPE;i++)
	{
		if(temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0 )
		{
			Device_Basic_Setting.reg.com1_config = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 1 "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom2()
{
	

	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->GetLBText(nSel,temp_string);
	for(int i=0;i<MAX_COM_TYPE;i++)
	{
		if(temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0 )
		{
			Device_Basic_Setting.reg.com2_config = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 2 "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}



void CBacnetSetting::OnCancel()
{
	

	//CDialogEx::OnCancel();
}


void CBacnetSetting::OnBnClickedButtonSettingLdf()
{
	
	if(IDYES == MessageBox(_T("Are you sure you want load factory default"),_T("Warning"),MB_YESNOCANCEL | MB_ICONINFORMATION))
	{
		Device_Basic_Setting.reg.reset_default = 88;
		CString temp_task_info;
		temp_task_info.Format(_T("Load Factory Default "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
	}
	
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate0()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]);i++)
	{
		if(temp_string.CompareNoCase(Baudrate_Array[i]) == 0 )
		{
			Device_Basic_Setting.reg.com_baudrate0 = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 0 baudrate "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}



void CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate1()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]);i++)
	{
		if(temp_string.CompareNoCase(Baudrate_Array[i]) == 0 )
		{
			Device_Basic_Setting.reg.com_baudrate1 = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 1 baudrate "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate2()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]);i++)
	{
		if(temp_string.CompareNoCase(Baudrate_Array[i]) == 0 )
		{
			Device_Basic_Setting.reg.com_baudrate2 = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 2 baudrate "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnBnClickedButtonSettingCleanDb()
{
	
	if(IDYES == MessageBox(_T("Are you sure you want clean the device database"),_T("Warning"),MB_YESNOCANCEL | MB_ICONINFORMATION))
	{
		Device_Basic_Setting.reg.reset_default = 150;
		CString temp_task_info;
		temp_task_info.Format(_T("Clear device "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
	}
}


extern bool cancle_send ;
void CBacnetSetting::OnBnClickedButtonSettingAddIntoDb()
{
	
	CBacnetaddintodb dlg;
	dlg.DoModal();

	if(!cancle_send)
	{
		CString temp_task_info;
		temp_task_info.Format(_T("Add device into database "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SUB_ID_BY_HAND,0,0,254,this->m_hWnd,temp_task_info);
	}

}


void CBacnetSetting::OnBnClickedButtonSettingUserList()
{
    GetDlgItem(IDC_BUTTON_SETTING_USER_LIST)->EnableWindow(FALSE);
    SetTimer(TIMER_CONTROL_BUTTON, 4000, NULL);
	show_user_list_window = true;
	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_READ_USER_LOGIN_INFO);
}


void CBacnetSetting::OnBnClickedRadioUsbDevice()
{
	
	if(Device_Basic_Setting.reg.usb_mode != 0)
	{
		Device_Basic_Setting.reg.usb_mode = 0;
		CString temp_task_info;
		temp_task_info.Format(_T("Change USB feature (USB Device) "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
	}

}


void CBacnetSetting::OnBnClickedRadioUsbHost()
{
	
		if(Device_Basic_Setting.reg.usb_mode != 1)
		{
			Device_Basic_Setting.reg.usb_mode = 1;
			CString temp_task_info;
			temp_task_info.Format(_T("Change USB feature (USB Host) "));
			Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		}

}


void CBacnetSetting::OnBnClickedButtonSettingGsmModual()
{
	
	CBacnetATCommand Dlg;
	Dlg.DoModal();
}


void CBacnetSetting::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	CDialogEx::OnClose();
}


void CBacnetSetting::OnEnKillfocusEditSettingPanel()
{
	
	CString temp_cstring;
	m_edit_panel.GetWindowTextW(temp_cstring);
	int temp_panel = _wtoi(temp_cstring);
	if((temp_panel >0) && (temp_panel <255) && (temp_panel != Device_Basic_Setting.reg.panel_number))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the panel number to %u ?"),temp_panel);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned char old_panel = Device_Basic_Setting.reg.panel_number;	//写之前先保存起来；写失败 恢复原值;
			Device_Basic_Setting.reg.panel_number = (unsigned char)temp_panel;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change panel number failed!"));
				Device_Basic_Setting.reg.panel_number = old_panel;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				Station_NUM = temp_panel;
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change panel number success!"));
			}
		}
	}
}


void CBacnetSetting::OnEnKillfocusEditSettingNodesLabelSetting()
{
	
	CString temp_cstring;
	m_edit_nodes_label.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=20)
	{
		MessageBox(_T("Nodes label length must between 0-20"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}

	if(Device_Basic_Setting.reg.en_panel_name)
	{
		char temp_char[30];
		WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,20,NULL,NULL);
		int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.panel_name,20);
		if(n_ret == 0)
			return;
		memcpy(Device_Basic_Setting.reg.panel_name,temp_char,20);
		if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Nodes label failed!"));
			memset(Device_Basic_Setting.reg.panel_name,0,20);
			PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Nodes label success!"));


			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


			CString temp_serial;
			CString strSql;
			temp_serial.Format(_T("%u"),g_selected_serialnumber);
			strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"),temp_serial);
			
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

			while(!q.eof())
			{
				strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"),temp_cstring,temp_serial);
				SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				q.nextRow();
				 
			}

			SqliteDBBuilding.closedb();
			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			if(selected_product_index < pFrame->m_product.size())
			{
				pFrame->m_pTreeViewCrl->SetItemText(pFrame->m_product.at(selected_product_index).product_item,temp_cstring);
				pFrame->m_product.at(selected_product_index).NameShowOnTree = temp_cstring;
			}
		}
	}


}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeServer()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->GetLBText(nSel,temp_string);
	if(nSel == 0)
		Device_Basic_Setting.reg.en_sntp = 2;
	else if(nSel == 1)
		Device_Basic_Setting.reg.en_sntp = 3;
	else if(nSel == 2)
		Device_Basic_Setting.reg.en_sntp = 4;
	else if(nSel == 3)
		Device_Basic_Setting.reg.en_sntp = 5;
	else if(nSel == 4)
		Device_Basic_Setting.reg.en_sntp = 6;
	((CButton *)GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
	CString temp_task_info;
	temp_task_info.Format(_T("Change sntp time server to "));
	temp_task_info = temp_task_info + temp_string;
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnBnClickedCheckSettingSyncTime()
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


void CBacnetSetting::OnEnKillfocusEditDyndnsUserName()
{
	
	CString temp_cstring;
	m_dyndns_user.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=DYNDNS_MAX_USERNAME_SIZE)
	{
		MessageBox(_T("Nodes label length must between 0-32"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}
	char temp_char[DYNDNS_MAX_USERNAME_SIZE + 1];
	memset(temp_char , 0 ,DYNDNS_MAX_USERNAME_SIZE + 1);
	WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,DYNDNS_MAX_USERNAME_SIZE,NULL,NULL);
	int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.dyndns_user,DYNDNS_MAX_USERNAME_SIZE);
	if(n_ret == 0)
		return;
	memcpy(Device_Basic_Setting.reg.dyndns_user,temp_char,DYNDNS_MAX_USERNAME_SIZE);
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns user name failed!"));
		memset(Device_Basic_Setting.reg.dyndns_user,0,DYNDNS_MAX_USERNAME_SIZE);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns user name success!"));
	}
}


void CBacnetSetting::OnEnKillfocusEditDyndnsPassword()
{
	
	CString temp_cstring;
	m_dyndns_password.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=DYNDNS_MAX_PASSWORD_SIZE)
	{
		MessageBox(_T("Nodes label length must between 0-32"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}
	char temp_char[DYNDNS_MAX_PASSWORD_SIZE + 1];
	memset(temp_char , 0 ,DYNDNS_MAX_PASSWORD_SIZE + 1);
	WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,DYNDNS_MAX_PASSWORD_SIZE,NULL,NULL);
	int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.dyndns_pass,DYNDNS_MAX_PASSWORD_SIZE);
	if(n_ret == 0)
		return;
	memcpy(Device_Basic_Setting.reg.dyndns_pass,temp_char,DYNDNS_MAX_PASSWORD_SIZE);
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns password failed!"));
		memset(Device_Basic_Setting.reg.dyndns_pass,0,DYNDNS_MAX_PASSWORD_SIZE);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns password success!"));
	}
}


void CBacnetSetting::OnEnKillfocusEditDyndnsDomain()
{
	
	CString temp_cstring;
	m_dyndns_domain.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=DYNDNS_MAX_DOMAIN_SIZE)
	{
		MessageBox(_T("Nodes label length must between 0-32"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}
	char temp_char[DYNDNS_MAX_DOMAIN_SIZE + 1];
	memset(temp_char , 0 ,DYNDNS_MAX_DOMAIN_SIZE + 1);
	WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,DYNDNS_MAX_DOMAIN_SIZE,NULL,NULL);
	int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.dyndns_domain,DYNDNS_MAX_DOMAIN_SIZE);
	if(n_ret == 0)
		return;
	memcpy(Device_Basic_Setting.reg.dyndns_domain,temp_char,DYNDNS_MAX_DOMAIN_SIZE);
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns domain failed!"));
		memset(Device_Basic_Setting.reg.dyndns_domain,0,DYNDNS_MAX_DOMAIN_SIZE);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change dyndns domain success!"));
	}
}


void CBacnetSetting::OnBnClickedCheckSettingDyndns()
{
	

		
		CString temp_task_info;
		if(Device_Basic_Setting.reg.en_dyndns == 1)
		{
			Device_Basic_Setting.reg.en_dyndns = 2;
			temp_task_info.Format(_T("Enable Dyndns "));
		}
		else
		{
			Device_Basic_Setting.reg.en_dyndns = 1;
			temp_task_info.Format(_T("Disable Dyndns "));
		}

		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingDdnsServer()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->GetLBText(nSel,temp_string);
	if(nSel == 0)
		Device_Basic_Setting.reg.dyndns_provider = 0;
	else if(nSel == 1)
		Device_Basic_Setting.reg.dyndns_provider = 1;
	else if(nSel == 2)
		Device_Basic_Setting.reg.dyndns_provider = 2;
	else if(nSel == 3)
		Device_Basic_Setting.reg.dyndns_provider = 3;

	CString temp_task_info;
	temp_task_info.Format(_T("Change DDNS server to "));
	temp_task_info = temp_task_info + temp_string;
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnEnKillfocusEditTimeUpdate()
{
	
	CString temp_cstring;
	m_edit_ddns_update_time.GetWindowTextW(temp_cstring);
	int update_time = _wtoi(temp_cstring);
	
	
	if((temp_cstring.IsEmpty()) || (update_time ==0) ||  (update_time >65535))
	{
		MessageBox(_T("value must between 0-65535"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}

	Device_Basic_Setting.reg.dyndns_update_time = update_time;

	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change DDNS Time failed!"));
		Device_Basic_Setting.reg.dyndns_update_time = 0;
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change DDNS Time success!"));
	}

}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeZone()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->GetLBText(nSel,temp_string);
	if(nSel >= sizeof(Time_Zone_Value)/sizeof(Time_Zone_Value[0]))
		return;
	Device_Basic_Setting.reg.time_zone = Time_Zone_Value[nSel];

	CString temp_task_info;
	temp_task_info.Format(_T("Change Time Zone to "));
	temp_task_info = temp_task_info + temp_string;
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
    Sleep(1000);
    OnBnClickedButtonBacTest();
}


void CBacnetSetting::OnBnClickedCheckSettingPap()
{
	
		CString temp_task_info;
		if(Device_Basic_Setting.reg.en_plug_n_play != 1)
		{
			Device_Basic_Setting.reg.en_plug_n_play = 1;
			temp_task_info.Format(_T("Enable Plug And Play "));
		}
		else
		{
			Device_Basic_Setting.reg.en_plug_n_play = 0;
			temp_task_info.Format(_T("Disable Plug And Play "));
		}

		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnEnKillfocusEditSettingPort()
{
	
	CString temp_cstring;
	m_edit_port.GetWindowTextW(temp_cstring);
	int temp_port = _wtoi(temp_cstring);
	if((temp_port >0) && (temp_port <65535) && (temp_port != Device_Basic_Setting.reg.modbus_port))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the modbus port to %u ?"),temp_port);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned short old_port = Device_Basic_Setting.reg.modbus_port;	//写之前先保存起来；写失败 恢复原值;
			Device_Basic_Setting.reg.modbus_port = (unsigned short)temp_port;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change port failed!"));
				Device_Basic_Setting.reg.modbus_port = old_port;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change port success!"));
			}
		}
	}
}


void CBacnetSetting::OnBnClickedButtonHealth()
{
	GetPrivateData_Blocking(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC));
	CBacnetSettingHealth Health_Dlg;
	Health_Dlg.DoModal();
}


void CBacnetSetting::OnEnKillfocusEditSettingObjInstance()
{
	
	CString temp_cstring;
	m_setting_obj_instance.GetWindowTextW(temp_cstring);
	unsigned int temp_obj_instance = unsigned int(_wtoi(temp_cstring));
	if((temp_obj_instance >0) && (temp_obj_instance <= MAX_OBJ_INSTANCE) && (temp_obj_instance != Device_Basic_Setting.reg.object_instance))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the bacnet object instance to %u ?"),temp_obj_instance);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned int old_object_instance = Device_Basic_Setting.reg.object_instance;	//写之前先保存起来；写失败 恢复原值;
			Device_Basic_Setting.reg.object_instance = (unsigned int)temp_obj_instance;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance failed!"));
				Device_Basic_Setting.reg.object_instance = old_object_instance;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				g_bac_instance = temp_obj_instance;
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance success!"));
			}
		}
		else
		{
			PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//这里调用 刷新线程重新刷新会方便一点;
		}
	}
}


BOOL CBacnetSetting::OnHelpInfo(HELPINFO* pHelpInfo)
{
	 
	// 	if((m_latest_protocol == PROTOCOL_BACNET_IP) || (m_latest_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP))
	// 	{
	HWND hWnd;

	if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_PANEL_CONFIGURATION);
	return (hWnd != NULL);
	// 	}
	// 	else{
	// 		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	// 	}
	return CDialogEx::OnHelpInfo(pHelpInfo);
}

void CBacnetSetting::OnBnClickedButtonBacSettingOk()
{
	
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		CString temp_task_info;
		temp_task_info.Format(_T("Write into device timeout!"));
		SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
	}
	refresh_tree_status_immediately = true;
}


void CBacnetSetting::OnBnClickedButtonBacSettingCancel()
{
	
	 PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
	 PostMessage(WM_FRESH_SETTING_UI,TIME_COMMAND,NULL);
}


void CBacnetSetting::OnBnClickedButtonSyncTime()
{
	

	Device_Basic_Setting.reg.reset_default = 99;
	CString temp_task_info;
	temp_task_info.Format(_T("SYNC Time Server "));
	((CButton *)GetDlgItem(IDC_BUTTON_SYNC_TIME))->EnableWindow(FALSE);
	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("SYNC Timer Server Now.Please Wait....."));
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		Sleep(1);
	}
	else
	{
		SetTimer(TIMER_SYNC_TIMER,4000,NULL);
		//SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Send SYNC time success!"));
	}


	
}


void CBacnetSetting::OnBnClickedCheckSettingZoneDaylightTime()
{
	
	 
	CString temp_task_info;
	if(Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
	{
		Device_Basic_Setting.reg.time_zone_summer_daytime = 0;
		temp_task_info.Format(_T("Disable Automatically adjust clock for Daylight Saving Time "));
	}
	else
	{
		Device_Basic_Setting.reg.time_zone_summer_daytime = 1;
		temp_task_info.Format(_T("Enable Automatically adjust clock for Daylight Saving Time"));
	}

	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

    Sleep(1000);
    OnBnClickedButtonBacTest();
}


void CBacnetSetting::OnBnClickedButtonBacShowZigbee()
{
	
	CTstatZigbeeLogic dlg;
	dlg.DoModal();
}


void CBacnetSetting::OnCbnKillfocusComboBacnetSettingTimeServer()
{
	
	CString temp_string;
	GetDlgItemText(IDC_COMBO_BACNET_SETTING_TIME_SERVER,temp_string);
	if(!temp_string.IsEmpty())
	{
		if((temp_string.CompareNoCase(Time_Server_Name[0])  == 0) ||
			(temp_string.CompareNoCase(Time_Server_Name[1])  == 0) ||
			(temp_string.CompareNoCase(Time_Server_Name[2])  == 0))
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
			WideCharToMultiByte(CP_ACP,NULL,temp_string.GetBuffer(),-1,Device_Basic_Setting.reg.sntp_server,30,NULL,NULL);
			CString temp_task_info;
			temp_task_info.Format(_T("Change sntp server to "));
			temp_task_info = temp_task_info + temp_string;
			Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		}
	}



}



void CBacnetSetting::Reset_Setting_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 90,temp_mynew_rect.top + 70,500,700, NULL);


	return;

}



void CBacnetSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(this->m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
	}
}


void CBacnetSetting::OnSysCommand(UINT nID, LPARAM lParam)
{
	 
	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 100,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}




void CBacnetSetting::OnEnKillfocusEditSettingModbusId()
{
	
	CString temp_cstring;
	m_edit_modbus_id.GetWindowTextW(temp_cstring);
	unsigned int temp_modbusid = unsigned int(_wtoi(temp_cstring));
	if((temp_modbusid<1) || (temp_modbusid >254))
	{
		MessageBox(_T("Invalid value."));
		return;
	}
	if((temp_modbusid >0) && (temp_modbusid <= 254) && (temp_modbusid != Device_Basic_Setting.reg.modbus_id))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the modbus ID to %u ?"),temp_modbusid);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned char old_modbusid = Device_Basic_Setting.reg.modbus_id;	//写之前先保存起来；写失败 恢复原值;
			Device_Basic_Setting.reg.modbus_id = (unsigned char)temp_modbusid;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance failed!"));
				Device_Basic_Setting.reg.modbus_id = old_modbusid;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance success!"));
			}
		}
		else
		{
			PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//这里调用 刷新线程重新刷新会方便一点;
		}
	}
}


void CBacnetSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBacnetSetting::OnBnClickedButtonSettingIoConfig()
{
	
	//版本大于38.6 的才有在setting 里面改port 的功能
	if(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 +Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 438)
	{
		MessageBox(_T("This feature need the newest firmware."));
		return;
	}


	if(GetPrivateData_Blocking(g_bac_instance,READEXT_IO_T3000,0,BAC_EXTIO_COUNT - 1,sizeof(Str_Extio_point)) < 0)
	{
		MessageBox(_T("Read data timeout"));
		return;
	}
	CBacnetIOConfig IOdlg;
	IOdlg.DoModal();
}


void CBacnetSetting::OnBnClickedButtonRebootDevice()
{
	
	//版本大于38.6 的才有在setting 里面改port 的功能
	if(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 +Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 441)
	{
		if(IDYES == MessageBox(_T("Are you sure you want reboot device"),_T("Warning"),MB_YESNOCANCEL | MB_ICONINFORMATION))
		{
			Device_Basic_Setting.reg.reset_default = 111;
			CString temp_task_info;
			temp_task_info.Format(_T("Reboot device "));
			Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		}
	}
	else
	{
		MessageBox(_T("Your panel version is too old."),_T("Notice"),MB_OK);
	}

}

void CBacnetSetting::InitScrollbar()
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);    
	scrollinfo.nPage=20;    //设置滑块大小
	scrollinfo.nMax=75;     //设置滚动条的最大位置0--75
	SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
}

void CBacnetSetting::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 

	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	int unit=3;        
	switch (nSBCode)  
	{      
	case SB_LINEUP:          //Scroll one line up
		scrollinfo.nPos -= 1;  
		if (scrollinfo.nPos<scrollinfo.nMin)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,unit); 
		break;  
	case SB_LINEDOWN:           //Scroll one line down
		scrollinfo.nPos += 1;  
		if (scrollinfo.nPos+scrollinfo.nPage>scrollinfo.nMax)  //此处一定要注意加上滑块的长度，再作判断
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-unit);  
		break;  
	case SB_PAGEUP:            //Scroll one page up.
		scrollinfo.nPos -= 5;  
		if (scrollinfo.nPos<=scrollinfo.nMin)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,unit*5);  
		break;  
	case SB_PAGEDOWN:        //Scroll one page down        
		scrollinfo.nPos += 5;  
		if (scrollinfo.nPos+scrollinfo.nPage>=scrollinfo.nMax)  //此处一定要注意加上滑块的长度，再作判断
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-unit*5);  
		break;  
	case SB_ENDSCROLL:      //End scroll     
		break;  
	case SB_THUMBPOSITION:  //Scroll to the absolute position. The current position is provided in nPos
		break;  
	case SB_THUMBTRACK:                  //Drag scroll box to specified position. The current position is provided in nPos
		ScrollWindow(0,(scrollinfo.nPos-nPos)*unit);  
		scrollinfo.nPos = nPos;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;  
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}




//#include "GroupScheduleDlg.h"
void CBacnetSetting::OnBnClickedButtonZoneSchedule()
{


	b_stop_read_tstat_schedule = false;  //是否继续读取标志，若后面数据为空则退出循环体.

    //在获取前清空缓存值.
    for (int i = 0;i < BAC_TSTAT_SCHEDULE;i++)
    {
        Str_tstat_schedule temp_tstat_schedule;
        memset(&m_tatat_schedule_data.at(i).all, 0, sizeof(Str_tstat_schedule));
    }

	for (int i = 0;i<BAC_TSTAT_SCHEDULE_GROUP;i++)
	{
		int end_temp_instance = 0;
		int ret_n;
		end_temp_instance = BAC_TSTAT_SCHEDULE_REMAINDER + (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i;
		if (end_temp_instance >= BAC_TSTAT_SCHEDULE)
			end_temp_instance = BAC_TSTAT_SCHEDULE - 1;
		CString n_temp_result;
		ret_n = GetPrivateData_Blocking(g_bac_instance, READ_TSTATE_SCHEDULE_T3000, (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i, end_temp_instance, sizeof(Str_tstat_schedule));
		if (ret_n)
		{
			n_temp_result.Format(_T("Read Tstat From %d to %d success."), (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i, end_temp_instance);
			SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
			Sleep(10);
			if (b_stop_read_tstat_schedule)
			{
				n_temp_result.Format(_T("Reading tstat schedule complete"));
				SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
				Sleep(10);
				break;
			}
		}
		else
		{
			n_temp_result.Format(_T("Reading tstat schedule From %d to %d failed."), (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i, end_temp_instance);
			SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
			return ;
		}
	}

	b_stop_read_tstat_schedule = false;

    //显示非模态对话框;
    if (BacnetTstatSchedule_Window != NULL)
    {
        delete BacnetTstatSchedule_Window;
        BacnetTstatSchedule_Window = NULL;
    }
    BacnetTstatSchedule_Window = new CBacnetTstatSchedule;
    BacnetTstatSchedule_Window->Create(IDD_DIALOG_BACNET_TSTAT_SCHEDULE, this);
    BacnetTstatSchedule_Window->ShowWindow(SW_SHOW);
}





void CBacnetSetting::OnBnClickedButtonSettingDone()
{
    // TODO: 在此添加控件通知处理程序代码
    //OnBnClickedBtnBacIPChange();
}


void CBacnetSetting::OnBnClickedRadioSettingSyncPc()
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


void CBacnetSetting::OnBnClickedRadioSettingLcdOn()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Change LCD Background Light ON "));

    Device_Basic_Setting.reg.LCD_Display = 1;

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSetting::OnBnClickedRadioSettingLcdOff()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_task_info;
    temp_task_info.Format(_T(" Change LCD Background Light OFF "));

    Device_Basic_Setting.reg.LCD_Display = 0;

    Post_Write_Message(g_bac_instance, (int8_t)WRITE_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), this->m_hWnd, temp_task_info);
}


void CBacnetSetting::OnEnKillfocusEditNetworkSub()
{
    // TODO: 在此添加控件通知处理程序代码
 
}


void CBacnetSetting::OnEnKillfocusEditNetworkZigbee()
{
    // TODO: 在此添加控件通知处理程序代码
 
}


void CBacnetSetting::OnEnKillfocusEditNetworkMain()
{
    // TODO: 在此添加控件通知处理程序代码
 
}


void CBacnetSetting::OnEnKillfocusEditSettingZeigbeePanid()
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


void CBacnetSetting::OnEnKillfocusEditSettingMaxMaster()
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
