// BacnetSetting.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSetting.h"
#include "afxdialogex.h"
#include "globle_function.h"

// CBacnetSetting dialog

IMPLEMENT_DYNAMIC(CBacnetSetting, CDialogEx)

CBacnetSetting::CBacnetSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetSetting::IDD, pParent)
{

}

CBacnetSetting::~CBacnetSetting()
{
}

void CBacnetSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIME_PICKER, m_cm5_time_picker);
	DDX_Control(pDX, IDC_DATE_PICKER, m_cm5_date_picker);
}


BEGIN_MESSAGE_MAP(CBacnetSetting, CDialogEx)
	ON_MESSAGE(WM_FRESH_SETTING_UI,Fresh_Setting_UI)
	ON_BN_CLICKED(IDC_BUTTON_BAC_TEST, &CBacnetSetting::OnBnClickedButtonBacTest)
	ON_BN_CLICKED(IDC_BAC_ENABLE_EDIT_TIME, &CBacnetSetting::OnBnClickedBacEnableEditTime)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, &CBacnetSetting::OnNMKillfocusDatePicker)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TIME_PICKER, &CBacnetSetting::OnNMKillfocusTimePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATE_PICKER, &CBacnetSetting::OnNMSetfocusDatePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_PICKER, &CBacnetSetting::OnNMSetfocusTimePicker)
	ON_BN_CLICKED(IDC_BTN_BAC_WRITE_TIME, &CBacnetSetting::OnBnClickedBtnBacWriteTime)
	ON_BN_CLICKED(IDC_BAC_SYNC_LOCAL_PC, &CBacnetSetting::OnBnClickedBtnBacSYNCTime)
	ON_BN_CLICKED(IDC_RADIO_BAC_IP_AUTO, &CBacnetSetting::OnBnClickedBtnBacIPAuto)
	ON_BN_CLICKED(IDC_RADIO_BAC_IP_STATIC, &CBacnetSetting::OnBnClickedBtnBacIPStatic)
	ON_BN_CLICKED(IDC_BUTTON_BAC_IP_CHANGED, &CBacnetSetting::OnBnClickedBtnBacIPChange)
	ON_BN_CLICKED(IDC_BUTTON_BAC_IP_CANCLE, &CBacnetSetting::OnBnClickedBtnBacIPCancle)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REBOOT_DEVICE, &CBacnetSetting::OnBnClickedButtonRebootDevice)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TCPIP, &CBacnetSetting::OnBnClickedButtonResetTcpip)
	ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_FLASH, &CBacnetSetting::OnBnClickedButtonEraseFlash)
END_MESSAGE_MAP()


// CBacnetSetting message handlers





void CBacnetSetting::OnBnClickedButtonBacTest()
{
	// TODO: Add your control notification handler code here

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
	temp_cs_show.Format(_T("Task ID = %d. Read time "),g_invoke_id);
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);


}


void CBacnetSetting::OnBnClickedBacEnableEditTime()
{
	// TODO: Add your control notification handler code here
	//g_invoke_id = GetPrivateData(
	//	177,
	//	GETSERIALNUMBERINFO, 
	//	0,
	//	0,
	//	sizeof(Str_Serial_info));
	//if(g_invoke_id<0)
	//	MessageBox(_T("111"));
#if 1

	static bool edit_status = false;
	if(edit_status == false)
	{
		edit_status = true;
		m_cm5_date_picker.EnableWindow(1);
		m_cm5_time_picker.EnableWindow(1);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(1);
		GetDlgItem(IDC_BAC_ENABLE_EDIT_TIME)->SetWindowTextW(_T("Disable Edit"));
	}
	else
	{
		edit_status = false;
		m_cm5_date_picker.EnableWindow(0);
		m_cm5_time_picker.EnableWindow(0);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
		GetDlgItem(IDC_BAC_ENABLE_EDIT_TIME)->SetWindowTextW(_T("Enable Edit"));
	}
#endif
}

void CBacnetSetting::Get_Time_Edit_By_Control()
{
	CTime temp_day;

	m_cm5_date_picker.GetTime(temp_day);

	int temp_year	= temp_day.GetYear();
	if(temp_year > 2000)
		temp_year = temp_year - 2000;
	Device_time.year = temp_year;
	Device_time.month = temp_day.GetMonth();
	Device_time.dayofmonth = temp_day.GetDay();
	Device_time.dayofweek = temp_day.GetDayOfWeek() -1;
	Device_time.dayofyear = 1;


	CTime temp_time;
	m_cm5_time_picker.GetTime(temp_time);
	Device_time.ti_hour = temp_time.GetHour();
	Device_time.ti_min = temp_time.GetMinute();
	Device_time.ti_sec = temp_time.GetMinute();
	CString temp_task_info;
	temp_task_info.Format(_T("Write Device Time.Changed to %02d-%02d %02d:%02d  "),
		Device_time.month,Device_time.dayofmonth,
		Device_time.ti_hour,Device_time.ti_min);
	Post_Write_Message(g_bac_instance,RESTARTMINI_COMMAND,0,0,sizeof(Time_block_mini),this->m_hWnd,temp_task_info);
}

void CBacnetSetting::OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}


void CBacnetSetting::OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}




void CBacnetSetting::OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	*pResult = 0;
}


void CBacnetSetting::OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	*pResult = 0;
}

void CBacnetSetting::OnBnClickedBtnBacSYNCTime()
{
	CTime	TimeTemp;
	CTime temp_time;
	int nyear,nmonth,nday,nhour,nmin,nsec;
	temp_time = CTime::GetCurrentTime();
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

	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	Get_Time_Edit_By_Control();

}

void CBacnetSetting::OnBnClickedBtnBacWriteTime()
{
	// TODO: Add your control notification handler code here

	Get_Time_Edit_By_Control();

}

void CBacnetSetting::OnBnClickedBtnBacIPAuto()
{
	Device_Basic_Setting.reg.tcp_type = 1;

	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);
}

void CBacnetSetting::OnBnClickedBtnBacIPStatic()
{
	Device_Basic_Setting.reg.tcp_type = 0;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
}

void CBacnetSetting::OnBnClickedBtnBacIPChange()
{
	BYTE address1,address2,address3,address4;
	BYTE subnet1, subnet2, subnet3, subnet4;
	BYTE gatway1,gatway2,gatway3,gatway4;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->GetAddress(address1,address2,address3,address4);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->GetAddress(subnet1,subnet2,subnet3,subnet4);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->GetAddress(gatway1,gatway2,gatway3,gatway4);
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
	CString temp_task_info;
	temp_task_info.Format(_T("Change IP Address Information "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}

void CBacnetSetting::OnBnClickedBtnBacIPCancle()
{
	PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
}



LRESULT CBacnetSetting::Fresh_Setting_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;
	CString temp_cs;
	int temp_year;
	CTime	TimeTemp;
	CString temp_hw_version = _T("Unknow");
	CString temp_mcu_version = _T("Unknow");
	CString temp_pic_version = _T("Unknow");
	CString temp_c8051_version = _T("Unknow");
	CString temp_5964_version = _T("Unknow");
	CString temp_bootloader_version = _T("Unknow");
	switch(command_type)
	{
	case READ_SETTING_COMMAND:
		if(Device_time.year<2000)
			temp_year = Device_time.year + 2000;
		TimeTemp = CTime(temp_year,Device_time.month,Device_time.dayofmonth,Device_time.ti_hour,Device_time.ti_min,Device_time.ti_sec);


		m_cm5_time_picker.SetFormat(_T("HH:mm"));
		m_cm5_time_picker.SetTime(&TimeTemp);

		//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
		m_cm5_date_picker.SetTime(&TimeTemp);

		((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->SetAddress(Device_Basic_Setting.reg.ip_addr[0],
			Device_Basic_Setting.reg.ip_addr[1],Device_Basic_Setting.reg.ip_addr[2],Device_Basic_Setting.reg.ip_addr[3]);
		((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->SetAddress(Device_Basic_Setting.reg.subnet[0],
			Device_Basic_Setting.reg.subnet[1],Device_Basic_Setting.reg.subnet[2],Device_Basic_Setting.reg.subnet[3]);
		((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->SetAddress(Device_Basic_Setting.reg.gate_addr[0],
			Device_Basic_Setting.reg.gate_addr[1],Device_Basic_Setting.reg.gate_addr[2],Device_Basic_Setting.reg.gate_addr[3]);

		if(Device_Basic_Setting.reg.tcp_type == 1)
		{
			((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(true);
			((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(false);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);
		}
		else if(Device_Basic_Setting.reg.tcp_type == 0)
		{
			((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(false);
			((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(true);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
		}

		if(bacnet_device_type == PRODUCT_CM5)
		{
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("CM5"));
		}
		else if(bacnet_device_type == BIG_MINIPANEL)
		{
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("Big Minipanel"));
		}
		else if(bacnet_device_type == SMALL_MINIPANEL)
		{
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("Small Minipanel"));
		}
		else
		{
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("Unknow device"));
		}

		if((Device_Basic_Setting.reg.mini_type == 1) || (Device_Basic_Setting.reg.mini_type == 2))
		{
			temp_hw_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.harware_rev);
			temp_pic_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware1_rev);
			temp_c8051_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware2_rev);
			temp_5964_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware3_rev);	
			temp_mcu_version.Format(_T("%d.%d"),Device_Basic_Setting.reg.pro_info.firmware0_rev_main,Device_Basic_Setting.reg.pro_info.firmware0_rev_sub);
			temp_bootloader_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.bootloader_rev);
		}
		((CEdit *)GetDlgItem(IDC_STATIC_SEETING_HARDWARE_VERSION))->SetWindowTextW(temp_hw_version);
		((CEdit *)GetDlgItem(IDC_STATIC_SEETING_MCU_VERSION))->SetWindowTextW(temp_mcu_version);
		((CEdit *)GetDlgItem(IDC_STATIC_SEETING_PIC_VERSION))->SetWindowTextW(temp_pic_version);
		((CEdit *)GetDlgItem(IDC_STATIC_SEETING_C8051_VERSION))->SetWindowTextW(temp_c8051_version);
		((CEdit *)GetDlgItem(IDC_STATIC_SEETING_SM5964_VERSION2))->SetWindowTextW(temp_5964_version);
		((CEdit *)GetDlgItem(IDC_STATIC_SEETING_BOOTLOADER_VERSION))->SetWindowTextW(temp_bootloader_version);
		
		break;
	default: 
		break;
	}


	return 0;
}




BOOL CBacnetSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_setting_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_setting_dlg_hwnd;
	m_cm5_date_picker.EnableWindow(0);
	m_cm5_time_picker.EnableWindow(0);
	GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(FALSE);
	m_cm5_time_picker.SetFormat(_T("HH:mm"));

	HICON hIcon = NULL; 
	HINSTANCE hInstResource    = NULL; 
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_OK), RT_GROUP_ICON); 
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_OK), IMAGE_ICON, 24, 24, 0); 
	((CButton *)GetDlgItem(IDC_BUTTON_BAC_IP_CHANGED))->SetIcon(hIcon);

	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_EXIT), RT_GROUP_ICON); 
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_EXIT), IMAGE_ICON, 24, 24, 0); 
	((CButton *)GetDlgItem(IDC_BUTTON_BAC_IP_CANCLE))->SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetSetting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//if(this->IsWindowVisible())
	//{
	//	PostMessage(WM_REFRESH_BAC_TSTAT_LIST,NULL,NULL);
	//	if(bac_select_device_online)
	//		Post_Refresh_Message(g_bac_instance,READTSTAT_T3000,0,BAC_TSTAT_COUNT - 1,sizeof(Str_TstatInfo_point),BAC_TSTAT_GROUP);
	//}

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetSetting::OnBnClickedButtonRebootDevice()
{
	// TODO: Add your control notification handler code here
	//Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RESET_REGISTER,77,0,this->m_hWnd,NULL,_T("Reboot Device"));
	//if(GetCommunicationHandle() == NULL)
	//{

	//	CreateThread(NULL,NULL,ConnectToDevice,NULL,NULL,NULL);
	//}
	



	write_one(g_tstat_id,RESET_REGISTER,SPECIAL_COMMAND_REBOOT);
}


void CBacnetSetting::OnBnClickedButtonResetTcpip()
{
	// TODO: Add your control notification handler code here
	write_one(g_tstat_id,RESET_REGISTER,SPECIAL_COMMAND_RESET_TCP);
}

void CBacnetSetting::OnBnClickedButtonEraseFlash()
{
	// TODO: Add your control notification handler code here
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RESET_REGISTER,SPECIAL_COMMAND_ERASE_FLASH,0,this->m_hWnd,NULL,_T("Erase Flash"));
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

		//Post_Refresh_Message(g_bac_instance,READINPUT_T3000,pInvoke->mRow,pInvoke->mRow,sizeof(Str_in_point), BAC_INPUT_GROUP);
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		//memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//还原没有改对的值
		PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//这里调用 刷新线程重新刷新会方便一点;
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}

	if(pInvoke)
		delete pInvoke;
	return 0;
}


