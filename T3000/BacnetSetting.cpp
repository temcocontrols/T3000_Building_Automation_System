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
#include "BacnetRemotePortWarning.h"

// CBacnetSetting dialog
extern HTREEITEM  hTreeItem_retry;
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
#define TIMER_REFRESH_READ_DELAY    15000
extern int ok_button_press ; //确定按钮
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

    DDX_Control(pDX, IDC_TAB_SETTING, m_setting_tab);
}


BEGIN_MESSAGE_MAP(CBacnetSetting, CDialogEx)
	ON_MESSAGE(WM_FRESH_SETTING_UI,Fresh_Setting_UI)

	ON_WM_TIMER()
	ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
    ON_WM_CLOSE()
    ON_WM_HELPINFO()


	ON_BN_CLICKED(IDC_BUTTON_SETTING_LDF, &CBacnetSetting::OnBnClickedButtonSettingLdf)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEAN_DB, &CBacnetSetting::OnBnClickedButtonSettingCleanDb)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADD_INTO_DB, &CBacnetSetting::OnBnClickedButtonSettingAddIntoDb)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_GSM_MODUAL, &CBacnetSetting::OnBnClickedButtonSettingGsmModual)
	ON_BN_CLICKED(IDC_CHECK_SETTING_PAP, &CBacnetSetting::OnBnClickedCheckSettingPap)
	ON_BN_CLICKED(IDC_BUTTON_BAC_SETTING_OK, &CBacnetSetting::OnBnClickedButtonBacSettingOk)
	ON_BN_CLICKED(IDC_BUTTON_BAC_SHOW_ZIGBEE, &CBacnetSetting::OnBnClickedButtonBacShowZigbee)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()

	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_REBOOT_DEVICE, &CBacnetSetting::OnBnClickedButtonRebootDevice)
	ON_WM_VSCROLL()


   // ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MSTP_ID, &CBacnetSetting::OnEnKillfocusEditSettingMstpId)

    
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTING, &CBacnetSetting::OnTcnSelchangeTabSetting)
END_MESSAGE_MAP()


// CBacnetSetting message handlers





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

void Getminitypename(unsigned char nmini_type, CString &ret_name)
{
    switch (nmini_type)
    {
    case PRODUCT_CM5:
        ret_name = _T("CM5");
        break;
    case MINIPANELARM:
    case BIG_MINIPANEL:
        ret_name = _T("T3-BB");
        break;
    case MINIPANELARM_LB:
    case SMALL_MINIPANEL:
        ret_name = _T("T3-LB");
        break;
    case MINIPANELARM_TB:
    case TINY_MINIPANEL:
    case TINY_EX_MINIPANEL:
        ret_name = _T("T3-TB");
        break;
    case T3_TB_11I:
        ret_name = _T("T3-TB-11I");
        break;
    case MINIPANELARM_NB:
        ret_name = _T("T3-NB");
        break;
    case T3_TSTAT10:
        ret_name = _T("TSTAT10");
        break;
    case T3_OEM:
        ret_name = _T("T3-OEM");
        break;
    default:
        ret_name = _T(" ");
        break;
    }
    if (nmini_type < 5)
        ret_name = ret_name + _T("(Asix)");

}

LRESULT CBacnetSetting::Fresh_Setting_UI(WPARAM wParam, LPARAM lParam)
{
#if 1
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
    memset(&lp_time_zone, 0, sizeof(TIME_ZONE_INFORMATION));

    ::GetTimeZoneInformation(&lp_time_zone);
    DaylightBias = lp_time_zone.DaylightBias;


    n_tempBias = 0 - lp_time_zone.Bias;
    n_tempBias = (n_tempBias * 100) / 60;
    CTime temp_time;
    temp_time = CTime::GetCurrentTime();

    pc_time_to_basic_delt = n_tempBias * 360 / 10;


    switch (command_type)
    {
    case READ_SETTING_COMMAND:
    {
        ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_IP))->SetAddress(Device_Basic_Setting.reg.ip_addr[0],
            Device_Basic_Setting.reg.ip_addr[1], Device_Basic_Setting.reg.ip_addr[2], Device_Basic_Setting.reg.ip_addr[3]);
        ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->SetAddress(Device_Basic_Setting.reg.subnet[0],
            Device_Basic_Setting.reg.subnet[1], Device_Basic_Setting.reg.subnet[2], Device_Basic_Setting.reg.subnet[3]);
        ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->SetAddress(Device_Basic_Setting.reg.gate_addr[0],
            Device_Basic_Setting.reg.gate_addr[1], Device_Basic_Setting.reg.gate_addr[2], Device_Basic_Setting.reg.gate_addr[3]);
        bacnet_device_type = Device_Basic_Setting.reg.mini_type;
        if ((Device_Basic_Setting.reg.mini_type == T3_TSTAT10) ||
            (Device_Basic_Setting.reg.mini_type == T3_OEM))
        {
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->EnableWindow(FALSE);
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->EnableWindow(FALSE);
            ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
            ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
            ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);
            ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_BUTTON_BAC_IP_CHANGED))->EnableWindow(FALSE);
            m_page_tcpip.m_edit_port.EnableWindow(false);
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_BUTTON_HEALTH))->EnableWindow(FALSE);
        }
        else
        {
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_BUTTON_HEALTH))->EnableWindow(true);
             m_page_tcpip.m_edit_port.EnableWindow(true);
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->EnableWindow(TRUE);
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->EnableWindow(TRUE);
            ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_BUTTON_BAC_IP_CHANGED))->EnableWindow(TRUE);
            if (Device_Basic_Setting.reg.tcp_type == 1)
            {
                m_tcp_type = 1;
                ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(true);
                ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(false);
                ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
                ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
                ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);
                
            }
            else if (Device_Basic_Setting.reg.tcp_type == 0)
            {
                m_tcp_type = 0;
                ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(false);
                ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(true);
                ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
                ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
                ((CIPAddressCtrl *)m_page_tcpip.GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
            }
        }


        //硬件版本大于 26 代表是arm的版本.
        if (Device_Basic_Setting.reg.pro_info.harware_rev >= 26)
        {
            m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(FALSE);
        }
        else
        {
            m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(TRUE);
        }

        //版本大于38.6 的才有在setting 里面改port 的功能
        if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 476)
        {
            CString temp_port;
            temp_port.Format(_T("%u"), Device_Basic_Setting.reg.modbus_port);
            m_page_tcpip.m_edit_port.SetWindowTextW(temp_port);
        }
        else
        {
            m_page_tcpip.m_edit_port.EnableWindow(false);
        }


        ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT))->ResetContent();
        ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT2))->ResetContent();
        ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT3))->ResetContent();
        for (int i = 0; i < sizeof(Com_Parity_bit) / sizeof(Com_Parity_bit[0]); i++)
        {
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT))->AddString(Com_Parity_bit[i]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT2))->AddString(Com_Parity_bit[i]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT3))->AddString(Com_Parity_bit[i]);
        }

#pragma region stop_bit
        ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT))->ResetContent();
        ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT2))->ResetContent();
        ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT3))->ResetContent();
        for (int i = 0; i < sizeof(Com_Stop_bit) / sizeof(Com_Stop_bit[0]); i++)
        {
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT))->AddString(Com_Stop_bit[i]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT2))->AddString(Com_Stop_bit[i]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT3))->AddString(Com_Stop_bit[i]);
        }

        if (Device_Basic_Setting.reg.uart_parity[0] <= sizeof(Com_Parity_bit) / sizeof(Com_Parity_bit[0]))
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT))->SetCurSel(Device_Basic_Setting.reg.uart_parity[0]);
        if (Device_Basic_Setting.reg.uart_parity[1] <= sizeof(Com_Parity_bit) / sizeof(Com_Parity_bit[0]))
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT2))->SetCurSel(Device_Basic_Setting.reg.uart_parity[1]);
        if (Device_Basic_Setting.reg.uart_parity[2] <= sizeof(Com_Parity_bit) / sizeof(Com_Parity_bit[0]))
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT3))->SetCurSel(Device_Basic_Setting.reg.uart_parity[2]);

        if (Device_Basic_Setting.reg.uart_stopbit[0] <= sizeof(Com_Stop_bit) / sizeof(Com_Stop_bit[0]))
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT))->SetCurSel(Device_Basic_Setting.reg.uart_stopbit[0]);
        if (Device_Basic_Setting.reg.uart_stopbit[1] <= sizeof(Com_Stop_bit) / sizeof(Com_Stop_bit[0]))
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT2))->SetCurSel(Device_Basic_Setting.reg.uart_stopbit[1]);
        if (Device_Basic_Setting.reg.uart_stopbit[2] <= sizeof(Com_Stop_bit) / sizeof(Com_Stop_bit[0]))
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT3))->SetCurSel(Device_Basic_Setting.reg.uart_stopbit[2]);

        if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 497)//497
        {
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT))->EnableWindow(TRUE);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT2))->EnableWindow(false);  //zigbee 口始终固定;
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT3))->EnableWindow(TRUE);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT))->EnableWindow(TRUE);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT2))->EnableWindow(false);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT3))->EnableWindow(TRUE);

        }
        else
        {
            m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT)->EnableWindow(false);
            m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT2)->EnableWindow(false);
            m_page_tcpip.GetDlgItem(IDC_COMBO_PARITY_BIT3)->EnableWindow(false);
            m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT)->EnableWindow(false);
            m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT2)->EnableWindow(false);
            m_page_tcpip.GetDlgItem(IDC_COMBO_STOP_BIT3)->EnableWindow(false);
        }
#pragma endregion




        //20180201 fandu  ARM 的 板子才有 zone schedual 这个功能
        //版本大于46.1 的才有在setting 里面改port 的功能


        if (Device_Basic_Setting.reg.en_plug_n_play == 1)
        {
            ((CButton *)GetDlgItem(IDC_CHECK_SETTING_PAP))->SetCheck(true);
        }
        else
        {
            ((CButton *)GetDlgItem(IDC_CHECK_SETTING_PAP))->SetCheck(false);
        }


        unsigned long time_setting_now = (unsigned long)time(NULL);

        if ((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
        {
            CString temp123;
            temp123.Format(_T("%u"), Device_Basic_Setting.reg.time_update_since_1970);
            DFTrace(temp123);
        }

        if ((Device_Basic_Setting.reg.time_update_since_1970 < 1420041600) || (Device_Basic_Setting.reg.time_update_since_1970 > 1735660800))
        {
            ((CEdit *)m_page_time.GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(_T("No Reply"));
        }
        else
        {
            //CTime time_scaletime;
            CString strTime;
            time_t scale_time = Device_Basic_Setting.reg.time_update_since_1970;

            strTime = Check_Time(time_setting_now, Device_Basic_Setting.reg.time_update_since_1970);

            //time_scaletime = scale_time;
            //strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");
            ((CEdit *)m_page_time.GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(strTime);
        }
        CString temp_boot_time;
        unsigned int n_boot_time;
        int n_runday;int n_run_hour;int n_run_minite;
        n_boot_time = product_register_value[94] + product_register_value[95] * 65536;
        n_runday = n_boot_time / (3600 * 24);
        n_run_hour = (n_boot_time % (3600 * 24)) /3600;
        n_run_minite = (n_boot_time % (3600 * 24)) % 3600 /60;
        temp_boot_time.Format(_T("%d Days   %d Hours   %d Minites"), n_runday, n_run_hour, n_run_minite);
        m_page_time.GetDlgItem(IDC_EDIT_DEVICE_RUNTIME)->SetWindowTextW(temp_boot_time);
        if (Device_Basic_Setting.reg.en_dyndns == 0)
        {
            m_page_dyndns.GetDlgItem(IDC_CHECK_SETTING_DYNDNS)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_DYNDNS)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_SETTING_USER_NAME)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_DYNDNS_USER_NAME)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_SETTING_PASSWORD)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_DYNDNS_PASSWORD)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_SETTING_DOMAIN)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_DYNDNS_DOMAIN)->EnableWindow(FALSE);

            m_page_dyndns.GetDlgItem(IDC_STATIC_DDNS_SERVER)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_CHECK_AUTO)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_TIME_UPDATE)->EnableWindow(FALSE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_MIN_TIME)->EnableWindow(FALSE);

        }
        else
        {

            m_page_dyndns.GetDlgItem(IDC_CHECK_SETTING_DYNDNS)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_DYNDNS)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_SETTING_USER_NAME)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_DYNDNS_USER_NAME)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_SETTING_PASSWORD)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_DYNDNS_PASSWORD)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_SETTING_DOMAIN)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_DYNDNS_DOMAIN)->EnableWindow(TRUE);

            m_page_dyndns.GetDlgItem(IDC_STATIC_DDNS_SERVER)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_CHECK_AUTO)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_EDIT_TIME_UPDATE)->EnableWindow(TRUE);
            m_page_dyndns.GetDlgItem(IDC_STATIC_MIN_TIME)->EnableWindow(TRUE);

            if (Device_Basic_Setting.reg.en_dyndns == 2)
            {
                ((CButton *)m_page_dyndns.GetDlgItem(IDC_CHECK_SETTING_DYNDNS))->SetCheck(true);
            }
            else
            {
                Device_Basic_Setting.reg.en_dyndns = 1;
                ((CButton *)m_page_dyndns.GetDlgItem(IDC_CHECK_SETTING_DYNDNS))->SetCheck(false);
            }

            ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->ResetContent();
            ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[0]);
            ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[1]);
            ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[2]);
            ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[3]);
            ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[4]);

            if (Device_Basic_Setting.reg.dyndns_provider == 0)
                ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(0);
            else if (Device_Basic_Setting.reg.dyndns_provider == 1)
                ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(1);
            else if (Device_Basic_Setting.reg.dyndns_provider == 2)
                ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(2);
            else if (Device_Basic_Setting.reg.dyndns_provider == 3)
                ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(3);
            else if (Device_Basic_Setting.reg.dyndns_provider == 4)
                ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(4);
            else
            {
                Device_Basic_Setting.reg.dyndns_provider = 4;
                ((CComboBox *)m_page_dyndns.GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(4);
            }


            CString temp_time;
            temp_time.Format(_T("%u"), Device_Basic_Setting.reg.dyndns_update_time);
            m_page_dyndns.m_edit_ddns_update_time.SetWindowText(temp_time);

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
            m_page_dyndns.m_dyndns_user.SetWindowTextW(temp_dyndns_user);
            m_page_dyndns.m_dyndns_password.SetWindowTextW(temp_dyndns_password);
            m_page_dyndns.m_dyndns_domain.SetWindowTextW(temp_dyndns_domain);
        }

        if (Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
        {
            ((CButton *)m_page_time.GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME))->SetCheck(false);
        }
        else
        {
            Device_Basic_Setting.reg.time_zone_summer_daytime = 1;
            ((CButton *)m_page_time.GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME))->SetCheck(true);
        }

        if (Device_Basic_Setting.reg.zegbee_exsit == 0x74)
        {
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_BUTTON_BAC_SHOW_ZIGBEE))->EnableWindow(TRUE);
            m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(TRUE);
            ((CEdit *)m_page_tcpip.GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->EnableWindow(TRUE);

        }
        else
        {
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_BUTTON_BAC_SHOW_ZIGBEE))->EnableWindow(FALSE);
            m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(FALSE);
            ((CEdit *)m_page_tcpip.GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->EnableWindow(FALSE);
        }

        if ((Device_Basic_Setting.reg.mini_type == T3_TSTAT10) ||
            (Device_Basic_Setting.reg.mini_type == T3_OEM))
        {
            if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 539)
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_BUTTON_LCD_SETTING))->EnableWindow(true);
            else
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_BUTTON_LCD_SETTING))->EnableWindow(false);
        }
        else
        {
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_BUTTON_LCD_SETTING))->EnableWindow(false);
        }

        if ((Device_Basic_Setting.reg.mini_type == PRODUCT_CM5) ||
            Device_Basic_Setting.reg.mini_type == BIG_MINIPANEL ||
            Device_Basic_Setting.reg.mini_type == T3_TSTAT10 ||
            Device_Basic_Setting.reg.mini_type == T3_OEM ||
            Device_Basic_Setting.reg.mini_type == MINIPANELARM)
        {
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->EnableWindow(true);
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->EnableWindow(true);
        }
        else
        {
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->EnableWindow(false);
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->EnableWindow(false);
        }

        if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 519)//519
        {
            m_page_basic_info.GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(false);
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->EnableWindow(false);
            if (Device_Basic_Setting.reg.LCD_Display == 0)
            {
                //1 常灭
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(false);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(true);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->SetCheck(false);
            }
            else if (Device_Basic_Setting.reg.LCD_Display == 1)
            {
                //1 常亮
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(true);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(false);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->SetCheck(false);
            }
        }
        else
        {
            ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->EnableWindow(true);
           
            if (Device_Basic_Setting.reg.LCD_Display == 0)
            {
                //1 常灭
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(false);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(true);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->SetCheck(false);
                m_page_basic_info.GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(false);
            }
            else if (Device_Basic_Setting.reg.LCD_Display == 255)
            {
                //255 常亮
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(true);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(false);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->SetCheck(false);
                m_page_basic_info.GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(false);
            }
            else
            {
                CString temp_lcd_delay_time;
                //1 常亮
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_ON))->SetCheck(false);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_OFF))->SetCheck(false);
                ((CButton *)m_page_basic_info.GetDlgItem(IDC_RADIO_SETTING_LCD_DELAY_OFF))->SetCheck(true);
                m_page_basic_info.GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->EnableWindow(true);
                temp_lcd_delay_time.Format(_T("%d"), Device_Basic_Setting.reg.LCD_Display);
                m_page_basic_info.GetDlgItem(IDC_EDIT_LCD_DELAY_OFF_TIME)->SetWindowText(temp_lcd_delay_time);
            }
        }




        if (Device_Basic_Setting.reg.en_sntp == 0)
        {
            m_page_time.GetDlgItem(IDC_STATIC_SETTING_TIMER_SERVER)->ShowWindow(FALSE);
            m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME)->ShowWindow(FALSE);
            m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->ShowWindow(FALSE);

            m_page_time.GetDlgItem(IDC_STATIC_SETTING_TIME_ZONE)->ShowWindow(FALSE);
            m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE)->ShowWindow(FALSE);
        }
        else
        {
            m_page_time.GetDlgItem(IDC_STATIC_SETTING_TIMER_SERVER)->ShowWindow(TRUE);
            m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME)->ShowWindow(TRUE);
            m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->ShowWindow(TRUE);
            m_page_time.GetDlgItem(IDC_STATIC_SETTING_TIME_ZONE)->ShowWindow(TRUE);
            m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE)->ShowWindow(TRUE);

            ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->ResetContent();
            for (int j = 0;j < sizeof(Time_Server_Name) / sizeof(Time_Server_Name[0]);j++)
            {
                ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->AddString(Time_Server_Name[j]);
            }
            ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->ResetContent();
            for (int z = 0;z < sizeof(Time_Zone_Name) / sizeof(Time_Zone_Name[0]);z++)
            {
                ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->AddString(Time_Zone_Name[z]);
                if (Device_Basic_Setting.reg.time_zone == Time_Zone_Value[z])
                    ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->SetWindowTextW(Time_Zone_Name[z]);
            }

            if (Device_Basic_Setting.reg.time_sync_auto_manual == 0)
            {
                m_page_time.GetDlgItem(IDC_DATE_PICKER)->EnableWindow(false);
                m_page_time.GetDlgItem(IDC_TIME_PICKER)->EnableWindow(false);
                m_page_time.GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(false);
                
                m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->EnableWindow(true);
                m_page_time.GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(true);

                ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
                ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_PC))->SetCheck(false);

                if ((Device_Basic_Setting.reg.en_sntp == 2) ||
                    (Device_Basic_Setting.reg.en_sntp == 3) ||
                    (Device_Basic_Setting.reg.en_sntp == 4) ||
                    (Device_Basic_Setting.reg.en_sntp == 5))
                {


                    if (Device_Basic_Setting.reg.en_sntp == 2)
                    {
                        ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
                        ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(0);
                    }
                    if (Device_Basic_Setting.reg.en_sntp == 3)
                    {
                        ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
                        ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(1);
                    }
                    else if (Device_Basic_Setting.reg.en_sntp == 4)
                    {
                        ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);
                        ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(2);
                    }
                    if (Device_Basic_Setting.reg.en_sntp == 5)
                    {
                        ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(true);

                        CString temp_cs1;
                        MultiByteToWideChar(CP_ACP, 0, Device_Basic_Setting.reg.sntp_server, (int)strlen((char *)Device_Basic_Setting.reg.sntp_server) + 1,
                            temp_cs1.GetBuffer(MAX_PATH), MAX_PATH);
                        temp_cs1.ReleaseBuffer();

                        ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->AddString(temp_cs1);
                        ((CComboBox *)m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(3);
                    }
                }
            }
            else if (Device_Basic_Setting.reg.time_sync_auto_manual == 1)
            {
                m_page_time.GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(true);
                m_page_time.GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->EnableWindow(false);
                m_page_time.GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(false);

                ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_TIME))->SetCheck(false);
                ((CButton *)m_page_time.GetDlgItem(IDC_RADIO_SETTING_SYNC_PC))->SetCheck(true);
            }

        }
        CString temp_panel_number;
        CString temp_nodes_label;
        if (Device_Basic_Setting.reg.en_panel_name)
        {
            temp_panel_number.Format(_T("%u"), Device_Basic_Setting.reg.panel_number);
            MultiByteToWideChar(CP_ACP, 0, (char *)Device_Basic_Setting.reg.panel_name, (int)strlen((char *)Device_Basic_Setting.reg.panel_name) + 1, temp_nodes_label.GetBuffer(MAX_PATH), MAX_PATH);
            temp_nodes_label.ReleaseBuffer();

        }
        else
        {
            temp_panel_number.Format(_T("%u"), bac_gloab_panel);
            CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
            if (selected_product_index <  pFrame->m_product.size())
            {
                temp_nodes_label = pFrame->m_product.at(selected_product_index).NameShowOnTree;
            }
            else
            {
                temp_nodes_label.Empty();
            }

        }
        m_page_basic_info.m_edit_panel.SetWindowTextW(temp_panel_number);
        m_page_basic_info.m_edit_nodes_label.SetWindowTextW(temp_nodes_label);

        if (Device_Basic_Setting.reg.usb_mode == 1)
        {
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_USB_DEVICE))->SetCheck(false);
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_USB_HOST))->SetCheck(true);
            (CButton *)GetDlgItem(IDC_BUTTON_SETTING_GSM_MODUAL)->EnableWindow(TRUE);
        }
        else if (Device_Basic_Setting.reg.usb_mode == 0)
        {
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_USB_DEVICE))->SetCheck(true);
            ((CButton *)m_page_tcpip.GetDlgItem(IDC_RADIO_USB_HOST))->SetCheck(false);
            (CButton *)GetDlgItem(IDC_BUTTON_SETTING_GSM_MODUAL)->EnableWindow(FALSE);

        }

        if (Device_Basic_Setting.reg.sd_exist == SD_STATUS_NO)
        {
            ((CStatic *)m_page_basic_info.GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("No SD Card"));
        }
        else if (Device_Basic_Setting.reg.sd_exist == SD_STATUS_NORMAL)
        {
            ((CStatic *)m_page_basic_info.GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("Normal"));
        }
        else if (Device_Basic_Setting.reg.sd_exist == SD_STATUS_FILESYSTEM_ERROR)
        {
            ((CStatic *)m_page_basic_info.GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("File System error,Only supprot FAT32"));
        }
        else
        {
            ((CStatic *)m_page_basic_info.GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("unknown"));
        }

        if ((bacnet_device_type == BIG_MINIPANEL ||
            bacnet_device_type == MINIPANELARM ||
            bacnet_device_type == MINIPANELARM_LB ||
            bacnet_device_type == MINIPANELARM_TB) ||
            bacnet_device_type == T3_TB_11I ||
            bacnet_device_type == SMALL_MINIPANEL ||
            bacnet_device_type == TINY_MINIPANEL ||
            bacnet_device_type == TINY_EX_MINIPANEL ||
            bacnet_device_type == MINIPANELARM_NB ||
            bacnet_device_type == T3_TSTAT10 ||
            bacnet_device_type == T3_OEM ||
            bacnet_device_type == PRODUCT_CM5)
        {
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->ResetContent();
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[NOUSE]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);

            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->ResetContent();
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[NOUSE]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);

            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->ResetContent();
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[NOUSE]);
            //((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MAIN_MSTP]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MSTP_MASTER]);

            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->EnableWindow(TRUE);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->EnableWindow(TRUE);
            if (Device_Basic_Setting.reg.com0_config < MAX_COM_TYPE)
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com0_config]);
            if (Device_Basic_Setting.reg.com1_config < MAX_COM_TYPE)
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com1_config]);
            if (Device_Basic_Setting.reg.com2_config < MAX_COM_TYPE)
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com2_config]);


            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->EnableWindow(1);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(1);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->EnableWindow(1);
                          
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->ResetContent();
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->ResetContent();
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->ResetContent();

            //Fandu 2017-12-07  新的arm板子的com0 支持所有的波特率.
            if ((Device_Basic_Setting.reg.mini_type == MINIPANELARM) ||
                (Device_Basic_Setting.reg.mini_type == MINIPANELARM_LB) ||
                (Device_Basic_Setting.reg.mini_type == MINIPANELARM_TB) ||
                (Device_Basic_Setting.reg.mini_type == T3_TB_11I) ||
                (Device_Basic_Setting.reg.mini_type == PRODUCT_CM5) ||
                (Device_Basic_Setting.reg.mini_type == MINIPANELARM_NB) ||
                (Device_Basic_Setting.reg.mini_type == T3_OEM) ||
                (Device_Basic_Setting.reg.mini_type == T3_TSTAT10) )
            {
                for (int x = 0;x< (sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]));x++)
                {
                    ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[x]);
                }
            }
            else
            {
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[UART_9600]);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[UART_19200]);
            }



            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->AddString(Baudrate_Array[UART_19200]);
            ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(FALSE);
            for (int x = 0;x< (sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]));x++)
            {
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->AddString(Baudrate_Array[x]);
            }

            //Fandu 2017-12-07  新的arm板子的com0 支持所有的波特率.
            if ((Device_Basic_Setting.reg.mini_type == MINIPANELARM) ||
                (Device_Basic_Setting.reg.mini_type == MINIPANELARM_LB) ||
                (Device_Basic_Setting.reg.mini_type == MINIPANELARM_TB) ||
                (Device_Basic_Setting.reg.mini_type == T3_TB_11I) ||
                (Device_Basic_Setting.reg.mini_type == PRODUCT_CM5) ||
                (Device_Basic_Setting.reg.mini_type == MINIPANELARM_NB) ||
                (Device_Basic_Setting.reg.mini_type == T3_OEM) ||
                (Device_Basic_Setting.reg.mini_type == T3_TSTAT10))
            {
                if (Device_Basic_Setting.reg.com_baudrate0 < sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]))
                    ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(Device_Basic_Setting.reg.com_baudrate0);

                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->ResetContent();
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[NOUSE]);
                //((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MSTP]);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MSTP_MASTER]);
                if (Device_Basic_Setting.reg.com0_config < MAX_COM_TYPE)
                    ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com0_config]);
            }
            else
            {
                if (Device_Basic_Setting.reg.com_baudrate0 == UART_9600)
                {
                    ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(0);
                }
                else if (Device_Basic_Setting.reg.com_baudrate0 == UART_19200)
                {
                    ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(1);
                }
            }

            if (Device_Basic_Setting.reg.com_baudrate1 == UART_19200)
            {
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->SetCurSel(0);
            }
            else
            {
                Device_Basic_Setting.reg.com_baudrate1 = UART_19200;
            }

            if (Device_Basic_Setting.reg.com_baudrate2 < sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]))
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->SetCurSel(Device_Basic_Setting.reg.com_baudrate2);


            if ((bacnet_device_type == T3_TSTAT10) ||
                (bacnet_device_type == T3_OEM))
            {
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->EnableWindow(0);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->EnableWindow(0);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(0);
                ((CComboBox *)m_page_tcpip.GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->EnableWindow(0);
            }

        }
        CString temp_productname;
        Getminitypename(Device_Basic_Setting.reg.mini_type, temp_productname);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(temp_productname);
        

        if (
            (bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM) ||
            (Device_Basic_Setting.reg.mini_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB) ||
            (Device_Basic_Setting.reg.mini_type == TINY_MINIPANEL || bacnet_device_type == MINIPANELARM_TB) ||
            (bacnet_device_type == T3_TB_11I) ||
            (Device_Basic_Setting.reg.mini_type == TINY_EX_MINIPANEL) ||
            (Device_Basic_Setting.reg.mini_type == MINIPANELARM_NB) ||
            (Device_Basic_Setting.reg.mini_type == PRODUCT_CM5) ||
            (Device_Basic_Setting.reg.mini_type == T3_OEM) ||
            (Device_Basic_Setting.reg.mini_type == T3_TSTAT10)

            )
        {
            temp_hw_version.Format(_T("%d"), Device_Basic_Setting.reg.pro_info.harware_rev);
            temp_pic_version.Format(_T("%d"), Device_Basic_Setting.reg.pro_info.frimware1_rev);
            temp_c8051_version.Format(_T("%d"), Device_Basic_Setting.reg.pro_info.frimware2_rev);
            temp_5964_version.Format(_T("%d"), Device_Basic_Setting.reg.pro_info.frimware3_rev);
            temp_mcu_version.Format(_T("%d.%d"), Device_Basic_Setting.reg.pro_info.firmware0_rev_main, Device_Basic_Setting.reg.pro_info.firmware0_rev_sub);
            temp_bootloader_version.Format(_T("%d"), Device_Basic_Setting.reg.pro_info.bootloader_rev);
        }
        if ((Device_Basic_Setting.reg.mini_type == T3_TSTAT10) ||
            (Device_Basic_Setting.reg.mini_type == T3_OEM))
        {
            temp_pic_version.Format(_T(" "));
            temp_c8051_version.Format(_T(" ") );
            temp_5964_version.Format(_T(" "));
        }
        //temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
        if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 417)
            temp_serial_number.Format(_T("%u"), Device_Basic_Setting.reg.n_serial_number);
        else
            temp_serial_number.Format(_T("%u"), g_selected_serialnumber);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_HARDWARE_VERSION))->SetWindowTextW(temp_hw_version);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_MCU_VERSION))->SetWindowTextW(temp_mcu_version);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_PIC_VERSION))->SetWindowTextW(temp_pic_version);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_C8051_VERSION))->SetWindowTextW(temp_c8051_version);
        //((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_SM5964_VERSION2))->SetWindowTextW(temp_5964_version);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_BOOTLOADER_VERSION))->SetWindowTextW(temp_bootloader_version);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_STATIC_SEETING_SERIAL_NUMBER_2))->SetWindowTextW(temp_serial_number);


        CString temp_object;
        CString temp_mac_address;
        CString temp_mstp_network;
        CString temp_bip_network;
        CString temp_modbus_id;
        CString temp_mstp_id;
        temp_object.Format(_T("%u"), Device_Basic_Setting.reg.object_instance);
        if (((Device_Basic_Setting.reg.mac_addr[0] == 0) && (Device_Basic_Setting.reg.mac_addr[1] == 0) && (Device_Basic_Setting.reg.mac_addr[2] == 0) && (Device_Basic_Setting.reg.mac_addr[3] == 0) && (Device_Basic_Setting.reg.mac_addr[4] == 0) && (Device_Basic_Setting.reg.mac_addr[5] == 0) ) ||
            ((Device_Basic_Setting.reg.mac_addr[0] == 0xff) && (Device_Basic_Setting.reg.mac_addr[1] == 0xff) && (Device_Basic_Setting.reg.mac_addr[2] == 0xff) && (Device_Basic_Setting.reg.mac_addr[3] == 0xff) && (Device_Basic_Setting.reg.mac_addr[4] == 0xff) && (Device_Basic_Setting.reg.mac_addr[5] == 0xff)) )
        {
            temp_mac_address.Empty();
        }
        else
        {
            temp_mac_address.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"), Device_Basic_Setting.reg.mac_addr[0], Device_Basic_Setting.reg.mac_addr[1], Device_Basic_Setting.reg.mac_addr[2],
                Device_Basic_Setting.reg.mac_addr[3], Device_Basic_Setting.reg.mac_addr[4], Device_Basic_Setting.reg.mac_addr[5]);
        }
        temp_mac_address.MakeUpper();
        temp_mstp_network.Format(_T("%u"), Device_Basic_Setting.reg.mstp_network_number);
        temp_bip_network.Format(_T("%u"), Device_Basic_Setting.reg.network_number);
        if (Device_Basic_Setting.reg.modbus_id == 0)
        {
            temp_modbus_id.Empty();
            ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->EnableWindow(FALSE);
        }
        else
        {
            temp_modbus_id.Format(_T("%d"), Device_Basic_Setting.reg.modbus_id);
            ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->EnableWindow(TRUE);
        }

        temp_mstp_id.Format(_T("%d"), Device_Basic_Setting.reg.mstp_id);

        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_OBJ_INSTANCE))->SetWindowTextW(temp_object);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_MAC_ADDRESS))->SetWindowTextW(temp_mac_address);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_MSTP_NETWORK))->SetWindowTextW(temp_mstp_network);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_BIP_NETWORK2))->SetWindowTextW(temp_bip_network);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->SetWindowTextW(temp_modbus_id);

        CString cszigbeeanid;
        cszigbeeanid.Format(_T("%d"), Device_Basic_Setting.reg.zigbee_panid);
        ((CEdit *)m_page_tcpip.GetDlgItem(IDC_EDIT_SETTING_ZEIGBEE_PANID))->SetWindowTextW(cszigbeeanid);

        CString csmax_master;
        csmax_master.Format(_T("%u"), Device_Basic_Setting.reg.max_master);
        ((CEdit *)m_page_basic_info.GetDlgItem(IDC_EDIT_SETTING_MAX_MASTER))->SetWindowTextW(csmax_master);

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

                unsigned char temp_need_sync = 0;
                if (Device_time.new_time.n_time >= temp_time_long)  //设备时间大于本地时间
                {
                    if (Device_time.new_time.n_time > (temp_time_long + 60))  //大于本地一分钟以上 需同步
                        temp_need_sync = 1;
                }
                else if(Device_time.new_time.n_time > temp_time_long - 180) // 比本地时间小3分钟以内 ， 不同步
                {
                        temp_need_sync = 0;
                }
                else
                    temp_need_sync = 1; //小3分钟以上，需要同步;

                n_ignore_sync_time = GetPrivateProfileInt(_T("SYNC_Time"), _T("ignore_pop"), 0, g_cstring_ini_path);
                last_ignore_sync_time = GetPrivateProfileInt(_T("SYNC_Time"), _T("ignore_pop_time"), 0, g_cstring_ini_path);
                int delta_time = 0;
                delta_time = temp_time_long - last_ignore_sync_time;
                if ((n_ignore_sync_time == 1) && (delta_time  < 3600*24*3) && (delta_time >= 0))  //3天内 客户点了忽略，才不提醒;
                {
                    temp_need_sync = 0;
                }
                else
                {
                    WritePrivateProfileString(_T("SYNC_Time"), _T("ignore_pop"), _T("0"), g_cstring_ini_path);
                }
                //if (abs(long(temp_time_long - Device_time.new_time.n_time)) > 600)
                if (temp_need_sync == 1)
                {
                    CShowMessageDlg dlg;

                    CString cs_pc_time;
                    CString panel_time;
                    Time32toCString(Device_time.new_time.n_time, panel_time);
//#ifdef DEBUG
//
//                    CString temp_test;
//                    temp_test.Format(_T("%u"), Device_time.new_time.n_time);
//                    MessageBox(temp_test);
//#endif // DEBUG

                    
                    Time32toCString(temp_time_long, cs_pc_time);

                    CString temp_message;
                    temp_message.Format(_T("This device is set to automatically synchronize with a locally connected computer.\r\nAre you sure you want to sync to your PC?\r\n%s (Current Time) \r\n%s (New Time) \r\n"), panel_time, cs_pc_time);
                    dlg.SetStaticText(temp_message);
                    //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
                    dlg.SetStaticTextColor(RGB(0, 0, 255));
                    dlg.SetStaticTextSize(25, 20);
                    dlg.SetEvent(EVENT_SYNC_TIME);
                    dlg.DoModal();

                    if (ok_button_press == 1)
                    {
                        m_page_time.OnBnClickedBtnBacSYNCTime();
                    }
                }
            }
            else
            {
                m_page_time.OnBnClickedBtnBacSYNCTime();
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
            unsigned long  temp_time_long123 = time(NULL);
            unsigned long  temp_time_long = Device_time.new_time.n_time;

            //**********************************************************
            //2019 05 20 Fance 用于处理 电脑勾选了夏令时 引起的 T3 显示时间 与实际时间总是相差一小时的问题;
            TIME_ZONE_INFORMATION tzi;
            GetTimeZoneInformation(&tzi);
            //short computer_DaylightBias = tzi.DaylightBias * 60;

            //CString temp_debug;
            //temp_debug.Format(_T("Time(NULL) = %u\r\ntzi.DaylightBias = %d\r\ntzi.StandardBias=%d\r\n\Device Time=%u"), temp_time_long123, tzi.DaylightBias, tzi.StandardBias, temp_time_long);
            //MessageBox(temp_debug);
            //**********************************************************
            panel_time_to_basic_delt = Device_Basic_Setting.reg.time_zone * 360 / 10;

            //因为本地CDateTimeCtrl 在设置时间的时候 会默认 加上 电脑的时区，但是显示的时候要显示 设备所选时区，所以 要 变换.
            time_t scale_time;
            if (Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt;
            else if (Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
            {
                CTime	temptimevalue;
                time_t temp_t_time;
                temp_t_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt;
                temptimevalue = temp_t_time;
                int temp_month =temptimevalue.GetMonth();
                int temp_day = temptimevalue.GetDay();
                int day_of_year = temp_month * 30 + temp_day;
                if ((day_of_year > 135) && (day_of_year < 255))
                {
                    scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt + 3600; //如果选中夏令时 需要显示的时候加一个小时
                }
                else
                    scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt ; //如果选中夏令时 需要显示的时候加一个小时
                
            }
            else
                scale_time = temp_time_long - pc_time_to_basic_delt + panel_time_to_basic_delt; // 其他值当作没有夏令时处理.
            //TimeTemp = scale_time + computer_DaylightBias; //20200525
            TimeTemp = scale_time;//+ computer_DaylightBias;
            //CString temp_string;
            //temp_string.Format(_T("%d PC\r\n%d Panel\r\ntzi.DaylightBias = %d\r\n PC_tziBias = %d"), 
            //    temp_time_long123, 
            //    temp_time_long, 
            //    tzi.DaylightBias, 
            //    tzi.Bias);
            //MessageBox(temp_string);
        }

        //减去系统现在的时区 ，然后在加上 minipanel自己的时区.
        m_page_time.m_cm5_time_picker.SetFormat(_T("HH:mm"));
        m_page_time.m_cm5_time_picker.SetTime(&TimeTemp);
        m_page_time.m_cm5_date_picker.SetTime(&TimeTemp);
#pragma endregion about_time
    }
    break;
    case READ_EMAIL_ALARM:
    {
        m_page_email.Fresh_UI();
    }
        break;
    default:
        break;
    }
    //GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->SetFocus();
    return 0;

#endif
}



void CBacnetSetting::InitialTab()
{
    m_setting_tab.InsertItem(SETTING_BASIC,      _T("Basic information      "));
    m_setting_tab.InsertItem(SETTING_TCP,        _T("Communication          "));
    m_setting_tab.InsertItem(SETTING_TIME,       _T("Time                   "));
    m_setting_tab.InsertItem(SETTING_DYNDNS,     _T("Dyndns                 "));
    m_setting_tab.InsertItem(SETTING_EMAIL,      _T("Email                  "));
    m_setting_tab.InsertItem(SETTING_USER_LOGIN, _T("User Login             "));
    m_setting_tab.InsertItem(SETTING_EXPANSION_IO, _T("Expansion IO           "));
    //m_setting_tab.InsertItem(SETTING_WIFI, _T("Wifi           "));
    //创建两个对话框
    m_page_basic_info.Create(IDD_DIALOG_BACNET_SETTING_BASIC, &m_setting_tab);
    m_page_tcpip.Create(IDD_DIALOG_BACNET_SETTING_TCPIP, &m_setting_tab);
    m_page_time.Create(IDD_DIALOG_BACNET_SETTING_TIME, &m_setting_tab);
    m_page_dyndns.Create(IDD_DIALOG_BACNET_SETTING_DYNDNS, &m_setting_tab);
    m_page_email.Create(IDD_DIALOG_BACNET_EMAIL_ALARM, &m_setting_tab);
    m_page_user.Create(IDD_DIALOG_BACNET_USER_CONFIG, &m_setting_tab);
    m_page_expansion_io.Create(IDD_DIALOG_BACNET_EXT_IO_CONFIG, &m_setting_tab);
    m_page_wifi.Create(IDD_DIALOG_WIFI_CONFIG, &m_setting_tab);
    //m_page_wifi.ModifyStyle(WS_CAPTION , WS_CLIPCHILDREN, NULL);

    m_page_wifi.ModifyStyle(WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_CLIPSIBLINGS | DS_MODALFRAME,
        WS_CHILD | WS_CLIPCHILDREN, SWP_NOMOVE);

    //m_page_wifi.ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, NULL, NULL);
    //IDD_DIALOG_BACNET_USER_CONFIG
    //设定在Tab内显示的范围
    CRect rc;
    m_setting_tab.GetClientRect(rc);
    rc.top += 25;
    rc.bottom -= 10;
    rc.left += 10;
    rc.right -= 10;
    m_page_basic_info.MoveWindow(&rc);
    m_page_tcpip.MoveWindow(&rc);
    m_page_time.MoveWindow(&rc);
    m_page_dyndns.MoveWindow(&rc);
    m_page_email.MoveWindow(&rc);
    m_page_user.MoveWindow(&rc);
    m_page_expansion_io.MoveWindow(&rc);
    //m_page_wifi.MoveWindow(&rc);
    //把对话框对象指针保存起来
    tab_dialog[SETTING_BASIC] = &m_page_basic_info;
    tab_dialog[SETTING_TCP] = &m_page_tcpip;
    tab_dialog[SETTING_TIME] = &m_page_time;
    tab_dialog[SETTING_DYNDNS] = &m_page_dyndns;
    tab_dialog[SETTING_EMAIL] = &m_page_email;
    tab_dialog[SETTING_USER_LOGIN] = &m_page_user;
    tab_dialog[SETTING_EXPANSION_IO] = &m_page_expansion_io;

    //tab_dialog[SETTING_WIFI] = &m_page_wifi;
    //显示初始页面
    tab_dialog[SETTING_BASIC]->ShowWindow(SW_SHOW);
    tab_dialog[SETTING_TCP]->ShowWindow(SW_HIDE);
    tab_dialog[SETTING_TIME]->ShowWindow(SW_HIDE);
    tab_dialog[SETTING_DYNDNS]->ShowWindow(SW_HIDE);
    tab_dialog[SETTING_EMAIL]->ShowWindow(SW_HIDE);
    tab_dialog[SETTING_USER_LOGIN]->ShowWindow(SW_HIDE);
    tab_dialog[SETTING_EXPANSION_IO]->ShowWindow(SW_HIDE);
    //tab_dialog[SETTING_WIFI]->ShowWindow(SW_HIDE);
    //保存当前选择
    m_CurSelTab = 0;

}


BOOL CBacnetSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    InitialTab();
	InitScrollbar();
	SetWindowTextW(_T("Setting"));
	m_setting_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_setting_dlg_hwnd;
#if 0
    m_page_time.m_cm5_date_picker.EnableWindow(0);
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
#endif
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
               KillTimer(TIMER_SYNC_TIMER);
               if (GetPrivateData_Blocking(g_bac_instance, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
               {
                   //判断同步结果是否成功
                   if (Device_Basic_Setting.reg.sync_time_results != 1)
                   {
                       MessageBox(_T("SYNC time failed , No Reply from server"));
                   }
                   else
                   {
                       CTime time_scaletime;
                       CString strTime;
                       time_t scale_time = Device_Basic_Setting.reg.time_update_since_1970;
                       time_scaletime = scale_time;
                       strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");
                       SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("SYNC time sucess"));
                       ((CEdit *)m_page_time.GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(strTime);
                   }
               }
               m_page_time.GetDlgItem(IDC_BUTTON_SYNC_TIME)->EnableWindow(TRUE);

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








BOOL CBacnetSetting::OnHelpInfo(HELPINFO* pHelpInfo)
{
	 

	HWND hWnd;

	if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_PANEL_CONFIGURATION);
	return (hWnd != NULL);

	return CDialogEx::OnHelpInfo(pHelpInfo);
}

void CBacnetSetting::OnBnClickedButtonBacSettingOk()
{
    //unsigned long  temp_time_long = time(NULL);
    //g_Print.Format(_T("PC Time is %d  ,Panel's Time is %d"), temp_time_long, Device_time.new_time.n_time);
    //DFTrace(g_Print);
    //MessageBox(g_Print);
    //return;

//#ifdef _DEBUG
//
//    BACNET_APPLICATION_DATA_VALUE temp_value;
//    int invoke_id = Bacnet_Read_Properties_Blocking(133641, (BACNET_OBJECT_TYPE)4, 2, PROP_PRIORITY_ARRAY, temp_value, 3);
//
//    Sleep(1);
//    return;
//
//
//    Bacnet_Read_Property_Multiple();
//    return;
//#endif
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		CString temp_task_info;
		temp_task_info.Format(_T("Write into device timeout!"));
		SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
	}
	refresh_tree_status_immediately = true;
}






void CBacnetSetting::OnBnClickedButtonBacShowZigbee()
{
	
	CTstatZigbeeLogic dlg;
	dlg.DoModal();
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
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height() - DELTA_HEIGHT, NULL);
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





void CBacnetSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
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















void CBacnetSetting::OnTcnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    tab_dialog[m_CurSelTab]->ShowWindow(SW_HIDE);
    //得到新的页面索引
    m_CurSelTab = m_setting_tab.GetCurSel();
    if (SETTING_USER_LOGIN == m_CurSelTab)
    {
        if (!Check_Function(g_selected_product_id, g_protocol, F_SETTING_USER_LOGIN))
        {
            return;
        }
        bool b_ret = true;
        for (int i = 0; i<BAC_USER_LOGIN_GROUP; i++)
        {
            CString Mession_ret;
            int end_temp_instance = 0;
            end_temp_instance = BAC_READ_USER_LOGIN_INFO_REMAINDER + (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i;
            if (end_temp_instance >= BAC_USER_LOGIN_COUNT)
                end_temp_instance = BAC_USER_LOGIN_COUNT - 1;
            if (GetPrivateData_Blocking(g_bac_instance, READUSER_T3000, (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i, end_temp_instance, sizeof(Str_userlogin_point)) > 0)
            {
                Mession_ret.Format(_T("Read user login table from %d to %d success."), (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i, end_temp_instance);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
                Sleep(SEND_COMMAND_DELAY_TIME);
            }
            else
            {
                b_ret = false;
                Mession_ret.Format(_T("Read user login table from %d to %d timeout."), (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i, end_temp_instance);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
            }
        }

        if (b_ret)
        {
            m_page_user.Fresh_User_List(NULL, NULL);
        }
    }
    else if (SETTING_EXPANSION_IO == m_CurSelTab)
    {
        if (!Check_Function(g_selected_product_id, g_protocol, F_EXPANSION_IO))
        {
            return;
        }
        if (GetPrivateData_Blocking(g_bac_instance, READEXT_IO_T3000, 0, BAC_EXTIO_COUNT - 1, sizeof(Str_Extio_point)) < 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Expansion IO read timeout!"));
        }
        else
        {
            m_page_expansion_io.Initial_List();
            m_page_expansion_io.Fresh_Extio_List(NULL, NULL);
        }

    }
    //else if (SETTING_WIFI == m_CurSelTab)
    //{
    //    //m_page_wifi.OnInitDialog();
    //}
    //把新的页面显示出来
    tab_dialog[m_CurSelTab]->ShowWindow(SW_SHOW);
    *pResult = 0;
}
