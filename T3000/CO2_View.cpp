// CO2_View.cpp : implementation file
// CO2 coded by Fance 2013 07 20
// Already finished CO2 with RS485.

#include "stdafx.h"
#include "T3000.h"
#include "CO2_View.h"
#include "globle_function.h"
#include "CM5\MyOwnListCtrl.h"
#include "bado\BADO.h"
// CCO2_View

IMPLEMENT_DYNCREATE(CCO2_View, CFormView)

CCO2_View::CCO2_View()
	: CFormView(CCO2_View::IDD)
	, m_alarm_ctrl_manual_auto(FALSE)
	, m_alarm_on_time(0)
	, m_alarm_off_time(0)
	, m_edit_pre_alarm_setpoint(0)
	, m_edit_alarm_setpoint(0)
	, m_edit_calibrating_offset(0)
	, m_f_internal_temp(0)
	, m_f_external_temp(0)
	, m_humidity_value(0)
	, m_cs_password(_T(""))
	, m_co2_block_time(0)
	, m_co2_backlight_time(0)
	, m_co2_value(0)
{

	m_co2_firmwareversion = 0.0f;
	m_co2_hardwareversion = 0;
	m_co2_serialNumber = 0;
	m_brush.CreateSolidBrush(RGB(255,0,0));
	m_times=0;
	m_fresh_Grid=TRUE;
}

CCO2_View::~CCO2_View()
{

	if(RefreshThread != NULL)
		TerminateThread(RefreshThread, 0);

}

void CCO2_View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_CO2_EDIT, m_co2_idAdressEdit);
	DDX_Control(pDX, IDC_CO2_BRAUDRATECOMBO, m_co2_braudRateCombox);
	DDX_Text(pDX, IDC_FIRMWARV_CO2_EDIT, m_co2_firmwareversion);
	DDX_Text(pDX, IDC_HARDWAREV_CO2_EDIT, m_co2_hardwareversion);
	DDX_Text(pDX, IDC_SERIALNUM_CO2_EDIT, m_co2_serialNumber);
	DDX_Control(pDX, IDC_CO2_TEMP_UNIT, m_co2_temp_unit);
	DDX_Control(pDX, IDC_CO2_ALARM_STATE, m_co2_alarm_state);
	DDX_Control(pDX, IDC_RADIO_ALARM_MANUAL, m_co2_alarm_mode_ctr);
	DDX_Radio(pDX, IDC_RADIO_ALARM_MANUAL, m_alarm_ctrl_manual_auto);
	DDX_Text(pDX, IDC_CO2_ALARM_ON_TIME, m_alarm_on_time);
	DDX_Text(pDX, IDC_CO2_ALARM_OFF_TIME, m_alarm_off_time);
	DDX_Text(pDX, IDC_CO2_PREPARE_ALARM_SETPOINT, m_edit_pre_alarm_setpoint);
	DDX_Text(pDX, IDC_CO2_ALARM_SETPOINT, m_edit_alarm_setpoint);
	DDX_Text(pDX, IDC_CO2_CALIBRATING_OFFSET, m_edit_calibrating_offset);

	DDX_Text(pDX, IDC_EDIT_INTERNAL_TEMP, m_f_internal_temp);
	DDX_Text(pDX, IDC_EDIT_EXTERNAL_TEMP, m_f_external_temp);
	DDX_Control(pDX, IDC_COMBO_CO2_SENSOR_SEL, m_sensor_sel);
	DDX_Control(pDX, IDC_LIST_CO2_EXTERNAL_SENSOR, m_co2_external_sensor_list);
	DDX_Text(pDX, IDC_EDIT_CO2_HUMIDITY, m_humidity_value);
	DDX_Control(pDX, IDC_CO2_DATETIMEPICKER1, m_co2_day_picker);
	DDX_Control(pDX, IDC_CO2_DATETIMEPICKER_TIME, m_co2_time_picker);
	DDX_Text(pDX, IDC_EDIT_CO2_PASSWOR, m_cs_password);
	DDX_Text(pDX, IDC_EDIT_CO2_BLOCK_TIME, m_co2_block_time);
	DDX_Text(pDX, IDC_EDIT_CO2_BACKLIGHT_TIME, m_co2_backlight_time);
	DDX_Text(pDX, IDC_EDIT_CO2VALUE, m_co2_value);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_grid_input);
	DDX_Control(pDX, IDC_UPBUTTON, m_upButton);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_downButton);
	DDX_Control(pDX, IDC_EDIT_VALUE_GRID, m_inValueEdit);
	DDX_Control(pDX, IDC_EDIT1_TEST, m_test_edit);
}

BEGIN_MESSAGE_MAP(CCO2_View, CFormView)
	ON_BN_CLICKED(IDC_CO2_ENABLEIDBUTTON, &CCO2_View::OnBnClickedCo2Enableidbutton)
	ON_CBN_SELCHANGE(IDC_COMBO_CO2_SENSOR_SEL, &CCO2_View::OnCbnSelchangeComboCo2SensorSel)
	ON_MESSAGE(MY_RESUME_DATA, ResumeCO2MessageCallBack)
	ON_CBN_SELCHANGE(IDC_CO2_TEMP_UNIT, &CCO2_View::OnCbnSelchangeCo2TempUnit)
	ON_EN_KILLFOCUS(IDC_EDIT_INTERNAL_TEMP, &CCO2_View::OnEnKillfocusEditInternalTemp)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT_EXTERNAL_TEMP, &CCO2_View::OnEnKillfocusEditExternalTemp)
	ON_EN_KILLFOCUS(IDC_CO2_PREPARE_ALARM_SETPOINT, &CCO2_View::OnEnKillfocusCo2PrepareAlarmSetpoint)
	ON_EN_KILLFOCUS(IDC_CO2_ALARM_SETPOINT, &CCO2_View::OnEnKillfocusCo2AlarmSetpoint)
	ON_EN_KILLFOCUS(IDC_CO2_CALIBRATING_OFFSET, &CCO2_View::OnEnKillfocusCo2CalibratingOffset)
	ON_BN_CLICKED(IDC_RADIO_ALARM_MANUAL, &CCO2_View::OnBnClickedRadioAlarmManual)
	ON_BN_CLICKED(IDC_RADIO_ALARM_AUTO, &CCO2_View::OnBnClickedRadioAlarmAuto)
	ON_CBN_SELCHANGE(IDC_CO2_ALARM_STATE, &CCO2_View::OnCbnSelchangeCo2AlarmState)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Save_List_Item)	
	ON_NOTIFY(NM_KILLFOCUS, IDC_CO2_DATETIMEPICKER1, &CCO2_View::OnNMKillfocusCo2Datetimepicker1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_CO2_DATETIMEPICKER_TIME, &CCO2_View::OnNMKillfocusCo2DatetimepickerTime)
	ON_BN_CLICKED(IDC_BUTTON_CO2_SYNC_TIME, &CCO2_View::OnBnClickedButtonCo2SyncTime)
	ON_BN_CLICKED(IDC_RADIO_PASSWORD_ENABLE, &CCO2_View::OnBnClickedRadioPasswordEnable)
	ON_BN_CLICKED(IDC_RADIO_PASSWORD_DISABLE, &CCO2_View::OnBnClickedRadioPasswordDisable)
	ON_EN_KILLFOCUS(IDC_EDIT_CO2_PASSWOR, &CCO2_View::OnEnKillfocusEditCo2Passwor)
	ON_EN_KILLFOCUS(IDC_EDIT_CO2_BLOCK_TIME, &CCO2_View::OnEnKillfocusEditCo2BlockTime)
	ON_EN_KILLFOCUS(IDC_EDIT_CO2_BACKLIGHT_TIME, &CCO2_View::OnEnKillfocusEditCo2BacklightTime)
	ON_BN_CLICKED(IDC_BTN_CO2_REFRESH, &CCO2_View::OnBnClickedBtnCo2Refresh)
	ON_MESSAGE(WM_REFRESH_CO2_DLG, &CCO2_View::DealMessage)
	ON_EN_KILLFOCUS(IDC_CO2_ALARM_ON_TIME, &CCO2_View::OnEnKillfocusCo2AlarmOnTime)
	ON_EN_KILLFOCUS(IDC_CO2_ALARM_OFF_TIME, &CCO2_View::OnEnKillfocusCo2AlarmOffTime)
	ON_BN_CLICKED(IDC_BTN_CO2_CLEAR_CAL, &CCO2_View::OnBnClickedBtnCo2ClearCal)
	ON_BN_CLICKED(IDC_RADIO_HUMIDITY_HEAT_ENABLE, &CCO2_View::OnBnClickedRadioHumidityHeatEnable)
	ON_BN_CLICKED(IDC_RADIO_HUMIDITY_HEAT_DISABLE, &CCO2_View::OnBnClickedRadioHumidityHeatDisable)
	ON_EN_KILLFOCUS(IDC_EDIT_CO2_HUMIDITY, &CCO2_View::OnEnKillfocusEditCo2Humidity)
	ON_EN_KILLFOCUS(IDC_EDIT_CO2VALUE, &CCO2_View::OnEnKillfocusEditCo2value)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE_GRID, &CCO2_View::OnEnKillfocusEditValueGrid)
	//ON_BN_CLICKED(IDC_DOWNBUTTON, &CCO2_View::OnBnClickedDownbutton)
//	ON_NOTIFY(BCN_DROPDOWN, IDC_DOWNBUTTON, &CCO2_View::OnBnDropDownDownbutton)
    //ON_BN_CLICKED(IDC_UPBUTTON, &CCO2_View::OnBnClickedUpbutton)
	

//	ON_WM_LBUTTONDOWN()
ON_WM_TIMER()
ON_EN_KILLFOCUS(IDC_ID_CO2_EDIT, &CCO2_View::OnEnKillfocusIdCo2Edit)
ON_CBN_SELCHANGE(IDC_CO2_BRAUDRATECOMBO, &CCO2_View::OnCbnSelchangeCo2Braudratecombo)
END_MESSAGE_MAP()


// CCO2_View diagnostics

#ifdef _DEBUG
void CCO2_View::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCO2_View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CCO2_View::Fresh()
{
   g_NEED_MULTI_READ = FALSE;
	m_upButton.SetImage(IDB_UPBMP);
	m_downButton.SetImage(IDB_DOWNBMP);
	m_upButton.SetWindowText(_T(""));
	m_downButton.SetWindowText(_T(""));
	m_upButton.SizeToContent();
	m_downButton.SizeToContent();
	m_upButton.Invalidate();
	m_downButton.Invalidate();
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);

	Initial_Registerlist();
	Fresh_CO2();
	GetDlgItem(IDC_BTN_CO2_REFRESH)->EnableWindow(FALSE);
	if(RefreshThread==NULL)
		RefreshThread = CreateThread(NULL,NULL,StartRefresh,this,NULL,NULL);

}
void CCO2_View::Fresh_CO2(){
	CString temp_id;

	float m_fFirmwareVersion=0;
	UINT m_nSerialNumber=0;
	unsigned short m_nHardwareVersion=0;
	temp_id.Format(_T("%d"),product_register_value[6]);
	m_co2_idAdressEdit.SetWindowTextW(temp_id);

	if(product_register_value[CO2_485_MODBUS_BAUDRATE]>=0&&product_register_value[CO2_485_MODBUS_BAUDRATE]<=1)	
		m_co2_braudRateCombox.SetCurSel(product_register_value[CO2_485_MODBUS_BAUDRATE]);
	else
		m_co2_braudRateCombox.SetCurSel(0);

//	m_co2_braudRateCombox.EnableWindow(0);

	CString temp_firmversion;
	temp_firmversion.Format(_T("%0.2f"),((float)(product_register_value[CO2_485_MODBUS_VERSION_NUMBER_HI]*256+product_register_value[CO2_485_MODBUS_VERSION_NUMBER_LO]))/10.0);

	m_co2_firmwareversion= _wtof(temp_firmversion);//get_curtstat_version();
	m_co2_serialNumber=get_serialnumber();
	m_co2_hardwareversion=product_register_value[CO2_485_MODBUS_HARDWARE_REV];//8
// 	m_co2_temp_unit.ResetContent();
// 	m_co2_temp_unit.AddString(_T("℃"));
// 	m_co2_temp_unit.AddString(_T("℉"));
// 	Get_CO2_Temperature_unit(cs_temp_unit);
// 	m_co2_temp_unit.SetCurSel(product_register_value[CO2_485_MODBUS_DEG_C_OR_F]);

	C02_SHOW_TEMP();

	m_humidity_value = (float)(product_register_value[CO2_485_MODBUS_HUMIDITY_RH])/10.0;
	if(m_humidity_value>100)
	{
		(CEdit *)GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->EnableWindow(FALSE);
		m_humidity_value = 0;
	}
	else
	{
		(CEdit *)GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->EnableWindow(TRUE);
	}
	CO2_Alarm_Set();
	//CO2_Grid_init();
	//Initial_List();
	Fresh_External_List();


	if(product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE] == 1)
	{
		((CButton *)GetDlgItem(IDC_RADIO_PASSWORD_ENABLE))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_PASSWORD_DISABLE))->SetCheck(0);
		(GetDlgItem(IDC_EDIT_CO2_PASSWOR)->EnableWindow(TRUE));
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_PASSWORD_ENABLE))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_PASSWORD_DISABLE))->SetCheck(1);
		(GetDlgItem(IDC_EDIT_CO2_PASSWOR)->EnableWindow(FALSE));
	}


	if(product_register_value[CO2_485_MODBUS_HUM_SENSOR_HEATING] == 1)
	{
		((CButton *)GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_ENABLE))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_DISABLE))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_ENABLE))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_DISABLE))->SetCheck(1);
	}



	int cyear,cmonth,cday,chour,cmin,csec;
	cyear = product_register_value[CO2_485_MODBUS_RTC_YEAR];
	if(cyear<100)
		cyear = cyear + 2000;
	cmonth = product_register_value[CO2_485_MODBUS_RTC_MONTH];
	if(cmonth>12 || cmonth<=0)
		cmonth = 1;
	cday = product_register_value[CO2_485_MODBUS_RTC_DAY];
	if(cday > 31 || cday <=0)
		cday = 1;
	chour = product_register_value[CO2_485_MODBUS_RTC_HOUR];
	if(chour>23 )
		chour = 1;
	cmin = product_register_value[CO2_485_MODBUS_RTC_MIN];
	if(cmin >59)
		cmin = 0;
	csec = product_register_value[CO2_485_MODBUS_RTC_SEC];
	if(csec>59)
		csec = 0;


	CTime TimeTemp(cyear,cmonth,cday,chour,cmin,csec);

	m_co2_time_picker.SetFormat(_T("HH:mm"));
	m_co2_day_picker.SetTime(&TimeTemp);
	m_co2_time_picker.SetTime(&TimeTemp);

	m_co2_value=product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM];

	Show_PassWord();
	Show_Each_Edit_Time();
	UpdateData(FALSE);
}
void CCO2_View::Show_Each_Edit_Time()
{
	m_co2_block_time = product_register_value[CO2_485_MODBUS_MENU_BLOCK_SECONDS];
	m_co2_backlight_time = product_register_value[CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS];
	UpdateData(FALSE);
}

void CCO2_View::Show_PassWord()
{
	CString PassWord=_T("");
	CString temp_password[4];
	temp_password[0].Format(_T("%c"),product_register_value[CO2_485_MODBUS_USER_PASSWORD0]);
	temp_password[1].Format(_T("%c"),product_register_value[CO2_485_MODBUS_USER_PASSWORD1]);
	temp_password[2].Format(_T("%c"),product_register_value[CO2_485_MODBUS_USER_PASSWORD2]);
	temp_password[3].Format(_T("%c"),product_register_value[CO2_485_MODBUS_USER_PASSWORD3]);
	for (int i=0;i<4;i++)
	{
		PassWord = PassWord + temp_password[i];
	}
	m_cs_password = PassWord;
	UpdateData(FALSE);
}

void CCO2_View::C02_SHOW_TEMP()
{
	int i_internal_temp = 0;
	int i_external_temp = 0;
	float f_internal_temp = 0;
	float f_external_temp = 0;
	CString temp_internal_value,temp_external_value;
// 	m_sensor_sel.ResetContent();
// 	m_sensor_sel.InsertString(0,_T("Internal"));
// 	m_sensor_sel.InsertString(1,_T("External"));

		
	CString strTemp1,strTemp2,strUnit,strHUM,strCO2;
	strTemp1.Format(_T("%cC"),176);
	strTemp2.Format(_T("%cF"),176);


// 	m_co2_temp_unit.ResetContent();
// 	m_co2_temp_unit.AddString(strTemp1);
// 	m_co2_temp_unit.AddString(strTemp2);


	if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)		
	{
		i_internal_temp = product_register_value[CO2_485_MODBUS_TEMPERATURE_C_INTERNAL];
		i_external_temp = product_register_value[CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL];
// 		((CStatic *)GetDlgItem(IDC_STATIC_CO2_UNIT1))->SetWindowText(strTemp1);
// 		((CStatic *)GetDlgItem(IDC_STATIC_CO2_UNIT2))->SetWindowText(strTemp1);
		/*m_co2_temp_unit.SetCurSel(0);*/
		strUnit=strTemp1;
	}
	else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
	{
		i_internal_temp = product_register_value[CO2_485_MODBUS_TEMPERATURE_F_INTERNAL];
		i_external_temp = product_register_value[CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL];
// 		((CStatic *)GetDlgItem(IDC_STATIC_CO2_UNIT1))->SetWindowText(strTemp2);
// 		((CStatic *)GetDlgItem(IDC_STATIC_CO2_UNIT2))->SetWindowText(strTemp2);
		/*m_co2_temp_unit.SetCurSel(1);*/
		strUnit=strTemp2;
	}
	else
	{
		/*return;*/
	}
	f_internal_temp = (float)i_internal_temp / 10;
	f_external_temp = (float)i_external_temp / 10;
    CString  TempValue,StrAM;
	m_grid_input.put_TextMatrix(1,2,strUnit);
	
	if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
	{
		 TempValue.Format(_T("%0.1f"),f_internal_temp);
	}
	else
	{
	     TempValue.Format(_T("%0.1f"),f_external_temp);
	}
    m_grid_input.put_TextMatrix(1,4,TempValue);

	BOOL AM=Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],1);
	if (AM)
	{
		StrAM=_T("Manual");
	} 
	else
	{
		StrAM=_T("Auto");
	}
	 m_grid_input.put_TextMatrix(1,3,StrAM);

	 AM=Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],2);
	if (AM)
	{
		StrAM=_T("Manual");
	} 
	else
	{
		StrAM=_T("Auto");
	}
	m_grid_input.put_TextMatrix(2,3,StrAM);

	 AM=Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],3);
	if (AM)
	{
		StrAM=_T("Manual");
	} 
	else
	{
		StrAM=_T("Auto");
	}
	m_grid_input.put_TextMatrix(3,3,StrAM);

	strHUM.Format(_T("%0.1f"),(float)(product_register_value[CO2_485_MODBUS_HUMIDITY_RH])/10.0);
	m_grid_input.put_TextMatrix(2,4,strHUM);
	           strCO2.Format(_T("%d"),product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM]);
	m_grid_input.put_TextMatrix(3,4,strCO2);
}


void CCO2_View::Initial_List()
{
	m_co2_external_sensor_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_co2_external_sensor_list.SetExtendedStyle(m_co2_external_sensor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);

	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_DEVICE_ID, _T("Device ID"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_SERIAL_NUM, _T("Serial Number"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_PPM, _T("External PPM"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_PRE_ALARM_SP, _T("Prepare Alarm Setpoint"), 130, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_ALARM_SP, _T("Alarm Setpoint"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_CAL_OFFSET, _T("Calibrating Offset"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//Fresh_External_List();
	g_hwnd_now = this->m_hWnd;
	//m_input_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_input_dlg_hwnd;
	m_grid_input.put_Cols(6);
	m_grid_input.put_Rows(4);
	m_grid_input.put_TextMatrix(0,1,_T("Name"));
	m_grid_input.put_TextMatrix(0,2,_T("Range"));
	m_grid_input.put_TextMatrix(0,3,_T("A/M"));
	m_grid_input.put_TextMatrix(0,4,_T("Value"));
	m_grid_input.put_TextMatrix(0,5,_T("Calibration"));

	m_grid_input.put_TextMatrix(1,0,_T("1"));
	m_grid_input.put_TextMatrix(2,0,_T("2"));
	m_grid_input.put_TextMatrix(3,0,_T("3"));
	

	m_grid_input.put_TextMatrix(1,1,_T("Tempreture"));
	m_grid_input.put_TextMatrix(2,1,_T("Hum"));
	m_grid_input.put_TextMatrix(3,1,_T("CO2"));
	 

	m_grid_input.put_TextMatrix(2,2,_T("%"));
	m_grid_input.put_TextMatrix(3,2,_T("ppm"));
}


	//377	1	Device number in the scan database, include the master unit itself.
	//378	1	Set 1 to clear the scan database
	//379..383	5	First device of the database, the display unit take it.
	//5 bytes: 1st = address,  2nd..5th = serial number
	//384..388	5	Second device of the database, the first external sensor.
	//5 bytes: 1st = address,  2nd..5th = serial number
	//If the address is 0 or 255, that means not device behind.	
//170..219 The pre_alarm ppm set point of external co2 sensor. Support 50 external nodes.
//220..269 The continuous_alarm ppm set point of external co2 sensor. Support 50 external nodes.
//270..319 The ppm offset for calibrating the external co2 sensor ppm. Support 50 external nodes.
void CCO2_View::Fresh_External_List()
{
	int i_temp_count;
	m_co2_external_sensor_list.DeleteAllItems();
	i_temp_count = product_register_value[CO2_485_MODBUS_SCAN_DB_CTR];
	for (int i=1;i<i_temp_count;i++)
	{
		long temp_serialnumber =0;
		int  temp_id =0;
		CString temp_cs_num,temp_cs_id,temp_cs_serial,temp_cs_pre_alarm_sp,temp_cs_alarm_sp,temp_cs_cal_offset,temp_cs_ppm;
		temp_cs_num.Format(_T("%d"),i);
		m_co2_external_sensor_list.InsertItem(i-1,temp_cs_num);
		temp_id = product_register_value[CO2_485_MODBUS_SCAN_START + i*CO2_SCAN_DB_SIZE];
		temp_cs_id.Format(_T("%d"),temp_id);
		

		temp_serialnumber = product_register_value[CO2_485_MODBUS_SCAN_START + i*CO2_SCAN_DB_SIZE +4]<<24 |
			                product_register_value[CO2_485_MODBUS_SCAN_START + i*CO2_SCAN_DB_SIZE +3]<<16 |
			                product_register_value[CO2_485_MODBUS_SCAN_START + i*CO2_SCAN_DB_SIZE +2]<<8  |
			                product_register_value[CO2_485_MODBUS_SCAN_START + i*CO2_SCAN_DB_SIZE +1];

		temp_cs_serial.Format(_T("%d"),temp_serialnumber);
		

        temp_cs_pre_alarm_sp.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START +  i- 1]);
        temp_cs_alarm_sp.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXT_ALARM_SETPOINT_START + i - 1]);
        temp_cs_cal_offset.Format(_T("%d"),(short)product_register_value[CO2_485_MODBUS_EXT_CO2_OFFSET_START + i - 1]);

        if(product_register_value[CO2_485_MODBUS_EXTERNAL_CO2_PPM_START + i - 1] == 65535)
        {
            temp_cs_ppm.Format(_T("No Sensor"));
            m_co2_external_sensor_list.SetCellEnabled(i-1,CO2_EXTERNAL_PPM,0);
        }
        else
        {
            temp_cs_ppm.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXTERNAL_CO2_PPM_START + i - 1]);
            m_co2_external_sensor_list.SetCellEnabled(i-1,CO2_EXTERNAL_PPM,1);
        }
			
		m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_DEVICE_ID,temp_cs_id);
		m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_SERIAL_NUM,temp_cs_serial);
		m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_PPM,temp_cs_ppm);
		m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_PRE_ALARM_SP,temp_cs_pre_alarm_sp);
		m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_ALARM_SP,temp_cs_alarm_sp);
		m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_CAL_OFFSET,temp_cs_cal_offset);
	}
	

}

void CCO2_View::CO2_Alarm_Set()
{
	CString cs_alarm_on_time,cs_alarm_off_time;
	bool alarm_mode=ALARM_AUTO;
	unsigned short temp_data=product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL];
	temp_data=temp_data>>7;
	if(temp_data==1)
	{
		m_alarm_ctrl_manual_auto = 0;
		m_co2_alarm_state.EnableWindow(TRUE);
	}
	else
	{
		m_alarm_ctrl_manual_auto = 1;
		m_co2_alarm_state.EnableWindow(FALSE);
	}
	temp_data=product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL];
	temp_data=temp_data & 0x0003;
	if(temp_data<3)
	m_co2_alarm_state.SetCurSel(temp_data);

	cs_alarm_on_time.Format(_T("%d"), product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME]);
	cs_alarm_off_time.Format(_T("%d"),product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);

	m_alarm_on_time =  product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME];
	m_alarm_off_time = product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME];

	m_edit_pre_alarm_setpoint = product_register_value[CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT];
	m_edit_alarm_setpoint =     product_register_value[CO2_485_MODBUS_INT_ALARM_SETPOINT];
	m_edit_calibrating_offset = product_register_value[CO2_485_MODBUS_INT_CO2_OFFSET];
	UpdateData(FALSE);
	//if(product_register_value[118])
}

void CCO2_View::Get_CO2_Temperature_unit(CString &strTemp)
{
	UINT uint_temp=GetOEMCP();//get system is for chinese or english
	if(uint_temp!=936 && uint_temp!=950)
	{
		if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F]==0)	//121
		{
			strTemp.Format(_T("%cC"),176);
		}
		else
		{
			strTemp.Format(_T("%cF"),176);
		}
	}
	else
	{
		//Chinese.
		if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F]==0)//121
		{
			strTemp=_T("℃");
		}
		else
		{
			strTemp=_T("℉");
		}
	}
}
// CCO2_View message handlers


#include "EreaseDlg.h"
void CCO2_View::OnBnClickedCo2Enableidbutton()
{
    g_bPauseMultiRead = TRUE;
	CEreaseDlg Dlg;
	Dlg.DoModal();
	g_bPauseMultiRead = FALSE;
	// TODO: Add your control notification handler code here
	//if(m_co2_idAdressEdit.IsWindowEnabled())
	//{
	//	m_co2_idAdressEdit.EnableWindow(FALSE);
	//	((CButton *)GetDlgItem(IDC_CO2_ENABLEIDBUTTON))->SetWindowTextW(_T("Enable Change"));
	//}
	//else
	//{
	//	m_co2_idAdressEdit.EnableWindow(TRUE);
	//	((CButton *)GetDlgItem(IDC_CO2_ENABLEIDBUTTON))->SetWindowTextW(_T("Disable Change"));
	//}
}
BEGIN_EVENTSINK_MAP(CCO2_View, CFormView)
	ON_EVENT(CCO2_View, IDC_MSFLEXGRID_CO2, DISPID_CLICK, CCO2_View::ClickMsflexgridCo2, VTS_NONE)
	ON_EVENT(CCO2_View, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, CCO2_View::ClickMsflexgridInput, VTS_NONE)
END_EVENTSINK_MAP()


void CCO2_View::ClickMsflexgridCo2()
{
	// TODO: Add your message handler code here
}


void CCO2_View::OnCbnSelchangeComboCo2SensorSel()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int temp_cursel = m_sensor_sel.GetCurSel();
	if(temp_cursel != product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT,temp_cursel,
			product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT],this->m_hWnd,IDC_COMBO_CO2_SENSOR_SEL,_T("Display Sensor Select"));
	}
}


//Add 20130724  by Fance
LRESULT  CCO2_View::ResumeCO2MessageCallBack(WPARAM wParam, LPARAM lParam)
{
	UINT temp_id;
	_MessageWriteOneInfo *Write_Struct_feedback =(_MessageWriteOneInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	vector <int>::iterator Iter;
	if(msg_result)
	{
		int indexid = -1;
		for (int i=0;i<(int)Change_Color_ID.size();i++)
		{
			if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
				continue;
			else
				indexid = i;
		}

		if(indexid!=-1)
		{
			Iter = Change_Color_ID.begin()+indexid;
			Change_Color_ID.erase(Iter);
		}

		CString temp;
		temp.Format(_T("Change \"%s\" value from %d to %d success!"),
			Write_Struct_feedback->Changed_Name,
			Write_Struct_feedback->old_value,
			Write_Struct_feedback->new_value);
		temp_id = Write_Struct_feedback->CTRL_ID;
		SetPaneString(1,temp);
		product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->new_value;
		if(Write_Struct_feedback!=NULL)
			delete Write_Struct_feedback;
		//MessageBox(temp);
	}
	else
	{
		CString temp;
		temp.Format(_T("Change \"%s\" value from %d to %d Fail!"),
			Write_Struct_feedback->Changed_Name,
			Write_Struct_feedback->old_value,
			Write_Struct_feedback->new_value);

		temp_id = Write_Struct_feedback->CTRL_ID;
		SetPaneString(1,temp);
		Beep(10,100);
		product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->old_value;
		//GetDlgItem(Write_Struct_feedback->CTRL_ID)->SetWindowTextW(_T(""));

		int indexid = -1;
		for (int i=0;i<(int)Change_Color_ID.size();i++)
		{
			if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
				continue;
			else
				indexid = i;
		}
		Iter = Change_Color_ID.begin()+indexid;
		if(indexid!=-1)
		{
			Iter = Change_Color_ID.begin()+indexid;
			Change_Color_ID.erase(Iter);
		}

		if(Write_Struct_feedback!=NULL)
		{
			delete Write_Struct_feedback;
		}
	}
	Fresh();
	GetDlgItem(temp_id)->Invalidate();

	return 0;
}



void CCO2_View::OnCbnSelchangeCo2TempUnit()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nCurCol==2)
	{
		int temp_cursel = m_co2_temp_unit.GetCurSel();
		if(temp_cursel != product_register_value[CO2_485_MODBUS_DEG_C_OR_F])
		{
			Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_DEG_C_OR_F,temp_cursel,
				product_register_value[CO2_485_MODBUS_DEG_C_OR_F],this->m_hWnd,IDC_CO2_TEMP_UNIT,_T("Display Unit Select"));
		}
		C02_SHOW_TEMP();
	}
	if (m_nCurCol==3)
	{
	  int sel=m_co2_temp_unit.GetCurSel();
	  int m_value=1<<(m_nCurRow-1);
	  int m_write_value;
	  if (sel==Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))
	  {
	  return;
	  }
	  else
	  {
	    if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))//当前是1   选择是0
	    {
		  m_write_value=product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL]-m_value;
	    }
		else
		{
		m_write_value=product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL]+m_value;
		}
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_OUTPUT_AUTO_MANUAL,m_write_value,
			product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],this->m_hWnd,IDC_CO2_TEMP_UNIT,_T("Display Unit Select"));
			C02_SHOW_TEMP();

	  }
	  
	  
	}
	
	
}


BOOL CCO2_View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
			temp_focus->SetFocus();

			return 1;
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN &&(GetDlgItem(IDC_DOWNBUTTON)->GetSafeHwnd() == pMsg->hwnd))
	{
		 m_message_down=WM_LBUTTONDOWN;
		 m_times=1;
		 OnLButtonDown(pMsg->pt);
		 SetTimer(1,100,NULL);
		 m_fresh_Grid=false;
	}
	else if (pMsg->message == WM_LBUTTONUP&&(GetDlgItem(IDC_DOWNBUTTON)->GetSafeHwnd() == pMsg->hwnd))
	{
	   m_message_down=WM_LBUTTONUP;
	  //AfxMessageBox(_T("UP"));
 	   if (m_nCurRow==2)
 	   {
 		   if (m_value<0)
 		   {
 			   AfxMessageBox(_T("out of scale"));
 			   return FALSE;
 		   }
 	   }
 
 	  int ret=write_one(g_tstat_id,m_address,(unsigned short)m_value,10);
 	  if (ret>0)
 	  {
 	  product_register_value[m_address]=m_value;
 	  }
 	  else
 	  {
 	  AfxMessageBox(_T("Write Error"));
 	  }
	  
	   KillTimer(1);
	   m_fresh_Grid=true;
	   C02_SHOW_TEMP();
	   
	}
	else if (pMsg->message == WM_LBUTTONDOWN &&(GetDlgItem(IDC_UPBUTTON)->GetSafeHwnd() == pMsg->hwnd))
	{
		m_message_up=WM_LBUTTONDOWN;
		m_times=1;
		OnLButtonDown( pMsg->pt);
		SetTimer(1,100,NULL);
		 m_fresh_Grid=false;

	}
	else if (pMsg->message == WM_LBUTTONUP&&(GetDlgItem(IDC_UPBUTTON)->GetSafeHwnd() == pMsg->hwnd))
	{
		m_message_up=WM_LBUTTONUP;
		if (m_nCurRow==2)
		{
			if (m_value>1000)
			{
				AfxMessageBox(_T("out of scale"));
				return FALSE;
			}
		}
		
 		int ret=write_one(g_tstat_id,m_address,(unsigned short)m_value,10);
 		if (ret>0)
 		{
 			product_register_value[m_address]=m_value;
 		}
 		else
 		{
 			AfxMessageBox(_T("Write Error"));
 		}
		KillTimer(1);
		  m_fresh_Grid=true;
		   C02_SHOW_TEMP();
	}//m_message_up

	return CFormView::PreTranslateMessage(pMsg);
}


void CCO2_View::OnEnKillfocusEditInternalTemp()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int ivalue=0;
	ivalue = m_f_internal_temp *10;
	if(m_co2_temp_unit.GetCurSel())
	{
		if(ivalue == product_register_value[CO2_485_MODBUS_TEMPERATURE_F_INTERNAL])
			return;
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_TEMPERATURE_F_INTERNAL,ivalue,
			product_register_value[CO2_485_MODBUS_TEMPERATURE_F_INTERNAL],this->m_hWnd,IDC_EDIT_INTERNAL_TEMP,_T("Internal Temperature F"));
	}
	else
	{
		if(ivalue == product_register_value[CO2_485_MODBUS_TEMPERATURE_C_INTERNAL])
			return;
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_TEMPERATURE_C_INTERNAL,ivalue,
			product_register_value[CO2_485_MODBUS_TEMPERATURE_C_INTERNAL],this->m_hWnd,IDC_EDIT_INTERNAL_TEMP,_T("Internal Temperature C"));
	}
	
}


HBRUSH CCO2_View::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	for (int i=0;i<(int)Change_Color_ID.size();i++)
	{
		if(/*nCtlColor==CTLCOLOR_EDIT &&*/pWnd->GetDlgCtrlID()==Change_Color_ID.at(i))//注意此处的（pWnd->），否则没效果
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(255,0,0));//设置文本背景色
			pDC->SetBkMode(TRANSPARENT);//设置背景透明
			hbr = (HBRUSH)m_brush;
		}

	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CCO2_View::OnEnKillfocusEditExternalTemp()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int ivalue=0;
	ivalue = m_f_external_temp *10;
	if(m_co2_temp_unit.GetCurSel())
	{
		if(ivalue == product_register_value[CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL])
			return;
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL,ivalue,
			product_register_value[CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL],this->m_hWnd,IDC_EDIT_EXTERNAL_TEMP,_T("External Temperature F"));
	}
	else
	{
		if(ivalue == product_register_value[CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL])
			return;
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL,ivalue,
			product_register_value[CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL],this->m_hWnd,IDC_EDIT_EXTERNAL_TEMP,_T("External Temperature C"));
	}
}


void CCO2_View::OnEnKillfocusCo2PrepareAlarmSetpoint()
{
	 
	UpdateData();
	if(m_edit_pre_alarm_setpoint != product_register_value[CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT,m_edit_pre_alarm_setpoint,
			product_register_value[CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT],this->m_hWnd,IDC_CO2_PREPARE_ALARM_SETPOINT,_T("Prepare Alarm Setpoint"));
	}
}


void CCO2_View::OnEnKillfocusCo2AlarmSetpoint()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_edit_alarm_setpoint != product_register_value[CO2_485_MODBUS_INT_ALARM_SETPOINT])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INT_ALARM_SETPOINT,m_edit_alarm_setpoint,
			product_register_value[CO2_485_MODBUS_INT_ALARM_SETPOINT],this->m_hWnd,IDC_CO2_ALARM_SETPOINT,_T("Alarm Setpoint"));
	}
}


void CCO2_View::OnEnKillfocusCo2CalibratingOffset()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_edit_calibrating_offset != product_register_value[CO2_485_MODBUS_INT_CO2_OFFSET])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INT_CO2_OFFSET,m_edit_calibrating_offset,
			product_register_value[CO2_485_MODBUS_INT_CO2_OFFSET],this->m_hWnd,IDC_CO2_CALIBRATING_OFFSET,_T("Calibrating Offset"));
	}
}


void CCO2_View::OnBnClickedRadioAlarmManual()
{
	// TODO: Add your control notification handler code here
	unsigned short temp_value;
	temp_value = product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL] | 0x80;
	if(temp_value != product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_ALARM_AUTO_MANUAL,temp_value,
			product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL],this->m_hWnd,IDC_RADIO_ALARM_MANUAL,_T("Alarm Auto Manual Mode"));
	}

}


void CCO2_View::OnBnClickedRadioAlarmAuto()
{
	// TODO: Add your control notification handler code here
	unsigned short temp_value;
	temp_value = product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL] & 0x7f;
	if(temp_value != product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_ALARM_AUTO_MANUAL,temp_value,
			product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL],this->m_hWnd,IDC_RADIO_ALARM_MANUAL,_T("Alarm Auto Manual Mode"));
	}
}


void CCO2_View::OnCbnSelchangeCo2AlarmState()
{
	// TODO: Add your control notification handler code here
		UpdateData();
	int temp_item =	m_co2_alarm_state.GetCurSel();
	int write_value =0;
	write_value = (product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL] & 0xfc) | temp_item;
	if(write_value != product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_ALARM_AUTO_MANUAL,write_value,
			product_register_value[CO2_485_MODBUS_ALARM_AUTO_MANUAL],this->m_hWnd,IDC_CO2_ALARM_STATE,_T("Alarm State"));
	}
}
LRESULT CCO2_View::Save_List_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	int temp_write_reg = 0;
	int temp_reg_value = 0;
	CString temp_list_cstring;
	CString temp_cs_change_info;
	temp_list_cstring = m_co2_external_sensor_list.GetItemText(Changed_Item,Changed_SubItem);
	if(CheckString(temp_list_cstring)==false)
		return 1;
	temp_reg_value = _wtoi(temp_list_cstring);
	if(Changed_SubItem == CO2_EXTERNAL_PPM)
	{
		temp_write_reg = CO2_485_MODBUS_EXTERNAL_CO2_PPM_START + Changed_Item;
		temp_cs_change_info.Format(_T("External PPM"));
	}
	else if(Changed_SubItem == CO2_EXTERNAL_PRE_ALARM_SP)
	{
		temp_write_reg = CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START + Changed_Item;
		temp_cs_change_info.Format(_T("External Prepare Alarm Setpoint"));
	}
	else if(Changed_SubItem == CO2_EXTERNAL_ALARM_SP)
	{
		temp_write_reg = CO2_485_MODBUS_EXT_ALARM_SETPOINT_START + Changed_Item;
		temp_cs_change_info.Format(_T("Alarm Setpoint"));	
	}
	else if(Changed_SubItem == CO2_EXTERNAL_CAL_OFFSET)
	{
		temp_write_reg = CO2_485_MODBUS_EXT_CO2_OFFSET_START + Changed_Item;
		temp_cs_change_info.Format(_T("Calibrating Offset"));	
	}
	else
	{
		return -2;
	}

	if(temp_reg_value != product_register_value[temp_write_reg])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,temp_write_reg,temp_reg_value,
			product_register_value[temp_write_reg],this->m_hWnd,IDC_LIST_CO2_EXTERNAL_SENSOR,temp_cs_change_info);
	}

		return 0;
}

bool CheckString( CString str )
{

	int nCount = str.GetLength(); // 获得字符个数
	for ( int i = 0; i < nCount; i ++ )
	{
		if(i==0)
		{
			if(str.GetAt(i) == '-')
				continue;
		}
		

		if ( 0 == isdigit( str.GetAt(i) ) ) // 不是数字就置标志位
		{
			return FALSE;
			//break;// 退出
		}
	}

	return TRUE;
}



void CCO2_View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	Initial_List();
	m_start_tip=TRUE;
	HICON hIcon = NULL; 
	//hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_INPUT);
	//((CButton *)GetDlgItem(IDC_BUTTON_CM5_ADVANCE))->SetIcon(hIcon);
	HINSTANCE hInstResource    = NULL; 
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_REFRESH), RT_GROUP_ICON); 
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_REFRESH), IMAGE_ICON, 32, 32, 0); 
	((CButton *)GetDlgItem(IDC_BTN_CO2_REFRESH))->SetIcon(hIcon);
	RefreshThread = NULL;
	//CO2_Thread=AfxBeginThread(_ReadCo2MultiRegisters,this,THREAD_PRIORITY_NORMAL,0,0,NULL ); 
	//CO2_Thread->SuspendThread();

	// TODO: Add your specialized code here and/or call the base class
}





void CCO2_View::OnNMKillfocusCo2Datetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	Check_DayTime();
}


void CCO2_View::OnNMKillfocusCo2DatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;	
	Check_HourTime();
}

void CCO2_View::Check_DayTime()
{
	CTime temp_time;
	int cyear,cmonth,cday;
	m_co2_day_picker.GetTime(temp_time);
	cyear = temp_time.GetYear();
	if(cyear>1800)
		cyear = cyear%100;
	cmonth = temp_time.GetMonth();
	cday = temp_time.GetDay();
	if(cyear != product_register_value[CO2_485_MODBUS_RTC_YEAR])
	{
		if(write_one(g_tstat_id,CO2_485_MODBUS_RTC_YEAR,cyear,3)<0)
		{
			MessageBox(_T("Operate failure,Please try again"));
			return;
		}
		product_register_value[CO2_485_MODBUS_RTC_YEAR]=cyear;
	}

	if(cmonth != product_register_value[CO2_485_MODBUS_RTC_MONTH])
	{
		if(write_one(g_tstat_id,CO2_485_MODBUS_RTC_MONTH,cmonth,3)<0)
		{
			MessageBox(_T("Operate failure,Please try again"));
			return;
		}
		product_register_value[CO2_485_MODBUS_RTC_MONTH]=cmonth;
	}

	if(cday != product_register_value[CO2_485_MODBUS_RTC_DAY])
	{
		if(write_one(g_tstat_id,CO2_485_MODBUS_RTC_DAY,cday,3)<0)
		{
			MessageBox(_T("Operate failure,Please try again"));
			return;
		}
		product_register_value[CO2_485_MODBUS_RTC_DAY]=cday;
	}
	
}

void CCO2_View::Check_HourTime()
{
	CTime temp_time;
	int chour,cmin,csec;
	m_co2_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	csec = temp_time.GetSecond();

	if(chour != product_register_value[CO2_485_MODBUS_RTC_HOUR])
	{
		if(write_one(g_tstat_id,CO2_485_MODBUS_RTC_HOUR,chour,3)<0)
		{
			MessageBox(_T("Operate failure,Please try again"));
			return;
		}
	}

	if(cmin != product_register_value[CO2_485_MODBUS_RTC_MIN])
	{
		if(write_one(g_tstat_id,CO2_485_MODBUS_RTC_MIN,cmin,3)<0)
		{
			MessageBox(_T("Operate failure,Please try again"));
			return;
		}
	}
}

void CCO2_View::OnBnClickedButtonCo2SyncTime()
{
	// TODO: Add your control notification handler code here
	CTime TimeTemp=TimeTemp.GetCurrentTime();
	m_co2_time_picker.SetTime(&TimeTemp);
	m_co2_day_picker.SetTime(&TimeTemp);
	UpdateData();
	Check_DayTime();
	Check_HourTime();
	int cyear,cmonth,cday,chour,cmin,csec;
	cyear = product_register_value[CO2_485_MODBUS_RTC_YEAR];
	if(cyear<100)
		cyear = cyear + 2000;
	cmonth = product_register_value[CO2_485_MODBUS_RTC_MONTH];
	if(cmonth>12 || cmonth<=0)
		cmonth = 1;
	cday = product_register_value[CO2_485_MODBUS_RTC_DAY];
	if(cday > 31 || cday <=0)
		cday = 1;
	chour = product_register_value[CO2_485_MODBUS_RTC_HOUR];
	if(chour>23 )
		chour = 1;
	cmin = product_register_value[CO2_485_MODBUS_RTC_MIN];
	if(cmin >59)
		cmin = 0;
	csec = product_register_value[CO2_485_MODBUS_RTC_SEC];
	if(csec>59)
		csec = 0;


	CTime TimeTemp1(cyear,cmonth,cday,chour,cmin,csec);

	m_co2_time_picker.SetFormat(_T("HH:mm"));
	m_co2_day_picker.SetTime(&TimeTemp1);
	m_co2_time_picker.SetTime(&TimeTemp1);
}


void CCO2_View::OnBnClickedRadioPasswordEnable()
{
	// TODO: Add your control notification handler code here
	if(product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE] != TRUE)
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PASSWORD_ENABLE,1,
			product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE],this->m_hWnd,IDC_RADIO_PASSWORD_ENABLE,_T("Enable Password"));
	}
}


void CCO2_View::OnBnClickedRadioPasswordDisable()
{
	// TODO: Add your control notification handler code here
	if(product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE] != FALSE)
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PASSWORD_ENABLE,0,
			product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE],this->m_hWnd,IDC_RADIO_PASSWORD_ENABLE,_T("Disable Password"));
	}
}


void CCO2_View::OnEnKillfocusEditCo2Passwor()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_cs_password.GetLength()!=4)
	{
		MessageBox(_T("The password length must be 4 !"));
		return;
	}
	bool bpassword_changed = false;
	CString temp;
	CString temp_password[4];
	unsigned short short_password[4];
	temp = m_cs_password;
	for (int i=0;i<4;i++)
	{
		temp_password[i] = temp.Left(1);
		temp =  temp.Right(3-i);

		short_password[i] = _wtoi(temp_password[i]);
		short_password[i] = short_password[i] + 0x30;
		if(product_register_value[CO2_485_MODBUS_USER_PASSWORD0 + i] != short_password[i])
		{
			if(write_one(g_tstat_id,CO2_485_MODBUS_USER_PASSWORD0 + i,short_password[i])<0)
			{
				MessageBox(_T("Write Password Failure,Please Try Again"));
				Show_PassWord();
				return;
			}
			else
			{
				product_register_value[CO2_485_MODBUS_USER_PASSWORD0 + i] = short_password[i];
				SetPaneString(1,_T("Change Password Success!!"));
			}
		}
	}

}


void CCO2_View::OnEnKillfocusEditCo2BlockTime()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_co2_block_time != product_register_value[CO2_485_MODBUS_MENU_BLOCK_SECONDS])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_MENU_BLOCK_SECONDS,m_co2_block_time,
			product_register_value[CO2_485_MODBUS_MENU_BLOCK_SECONDS],this->m_hWnd,IDC_EDIT_CO2_BLOCK_TIME,_T("Menu Block Time"));
	}
}


void CCO2_View::OnEnKillfocusEditCo2BacklightTime()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_co2_backlight_time != product_register_value[CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS,m_co2_backlight_time,
			product_register_value[CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("Backlight Keep Time"));
	}
}


LRESULT CCO2_View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(MsgT3000ViewFresh==message)
	{
		if(this->IsWindowVisible())	//add by Fance ,if window not visible, it's unnecessary to fresh it;
			Fresh();
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


void CCO2_View::OnBnClickedBtnCo2Refresh()
{
	// TODO: Add your control notification handler code here

}
extern  HANDLE Read_Mute;
DWORD WINAPI CCO2_View::StartRefresh(LPVOID lpVoid)
{
	CCO2_View *pParent = (CCO2_View *)lpVoid;

	while(TRUE){
	    
		if (!is_connect()||g_bPauseMultiRead)
		{
		Sleep(1000);
		continue;
		}
		if (!pParent->m_fresh_Grid)
		{
		  Sleep(2000);
		  continue;
		}
		
		if(!no_mouse_keyboard_event_enable_refresh) 
		{
		Sleep(3000);
		continue ;
		}
		else
		{
		Sleep(2000);
		}
		
	int read_ret=0;
	register_critical_section.Lock();
	for(int i=0;i<7;i++)
	{
		if(!no_mouse_keyboard_event_enable_refresh) 
		{break ;}
		if (!pParent->m_fresh_Grid)
		{
			break ;
		}
		if(Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100)>0)
			read_ret++;
	}
	register_critical_section.Unlock();
	if(!no_mouse_keyboard_event_enable_refresh) 
	{continue ;}
	if (!pParent->m_fresh_Grid)
	{
		 
		continue;
	}
	if(read_ret<7)
		pParent->SendMessage(WM_REFRESH_CO2_DLG,REFRESH_FAIL,0);

	if(pParent->IsWindowVisible())
	{
		pParent->PostMessage(WM_REFRESH_CO2_DLG,REFRESH_DLG,0);
		pParent->PostMessage(WM_REFRESH_CO2_DLG,ENABLE_REFRESH_BUTTON,0);
	}



	}
	
	/*pParent->RefreshThread=NULL;*/
	return 0;

}

LRESULT  CCO2_View::DealMessage(WPARAM wParam,LPARAM lParam)
{
	int command = wParam;
	switch(command)
	{
	case REFRESH_DLG:
		Fresh_CO2();
		break;
	case ENABLE_REFRESH_BUTTON:
		GetDlgItem(IDC_BTN_CO2_REFRESH)->EnableWindow(TRUE);
		break;
	case  REFRESH_FAIL:
		if(this->IsWindowVisible())
			MessageBox(_T("Refresh failure,Please try again!"));
		break;
	default:
		break;
	}

	return 0;
}


void CCO2_View::OnEnKillfocusCo2AlarmOnTime()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_alarm_on_time != product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME,m_alarm_on_time,
			product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("User Defined Alarm On Time"));
	}
	
}


void CCO2_View::OnEnKillfocusCo2AlarmOffTime()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_alarm_off_time != product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME])
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME,m_alarm_off_time,
			product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("User Defined Alarm Off Time"));
	}
}


void CCO2_View::OnBnClickedBtnCo2ClearCal()
{
	// TODO: Add your control notification handler code here
	int item_count = m_co2_external_sensor_list.GetItemCount();
	for (int i=0;i<item_count;i++)
	{
		if(write_one(g_tstat_id,CO2_485_MODBUS_EXT_CO2_OFFSET_START + i,0,3)<0)
		{
			MessageBox(_T("Write register failure,Please try again!"));
			Fresh_External_List();
			return;
		}
		else
		{
			product_register_value[CO2_485_MODBUS_EXT_CO2_OFFSET_START + i] = 0;
		}
	}
	OnBnClickedBtnCo2Refresh();
	SetPaneString(1,_T("Clear External Calibration Offset Success!! "));
}


void CCO2_View::OnBnClickedRadioHumidityHeatEnable()
{
	// TODO: Add your control notification handler code here
	if(product_register_value[CO2_485_MODBUS_HUM_SENSOR_HEATING] != TRUE)
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_HUM_SENSOR_HEATING,1,
			product_register_value[CO2_485_MODBUS_HUM_SENSOR_HEATING],this->m_hWnd,IDC_RADIO_HUMIDITY_HEAT_ENABLE,_T("Humidity Sensor Heating"));
	}
}


void CCO2_View::OnBnClickedRadioHumidityHeatDisable()
{
	// TODO: Add your control notification handler code here
	if(product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE] != FALSE)
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_HUM_SENSOR_HEATING,0,
			product_register_value[CO2_485_MODBUS_HUM_SENSOR_HEATING],this->m_hWnd,IDC_RADIO_HUMIDITY_HEAT_DISABLE,_T("Humidity Sensor Heating"));
	}
}
void CCO2_View::Initial_Registerlist(){

 float fv=(float)(product_register_value[4]+product_register_value[5]*256)/10.0;
 if (m_start_tip)
 {
    if (fv<3.0)
    {
	  AfxMessageBox(_T("Your Firmware is old,please update it!"));
    }
    
 }
 
if (fv<2.0)
{
	CO2_485_MODBUS_SERIALNUMBER_LOWORD	=	0	;
	CO2_485_MODBUS_SERIALNUMBER_HIWORD	=	2	;
	CO2_485_MODBUS_VERSION_NUMBER_LO	=	4	;
	CO2_485_MODBUS_VERSION_NUMBER_HI	=	5	;
	CO2_485_MODBUS_ADDRESS	=	6	;
	CO2_485_MODBUS_PRODUCT_MODEL	=	7	;
	CO2_485_MODBUS_HARDWARE_REV	=	8	;
	CO2_485_MODBUS_PIC_VERSION	=	9	;
	CO2_485_MODBUS_ADDRESS_PLUG_N_PLAY	=	10	;
	CO2_485_MODBUS_BASE_ADDRESS	=	15	;
	CO2_485_MODBUS_UPDATE_STATUS	=	16	;
	CO2_485_MODBUS_SERINALNUMBER_WRITE_FLAG	=	17	;
	CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT	=	100	;
	CO2_485_MODBUS_DEG_C_OR_F	=	101	;
	CO2_485_MODBUS_TEMPERATURE_C_INTERNAL	=	102	;
	CO2_485_MODBUS_TEMPERATURE_F_INTERNAL	=	103	;
	CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL	=	104	;
	CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL	=	105	;
	CO2_485_MODBUS_HUMIDITY_RH	=	106	;
	CO2_485_MODBUS_HUMIDITY_FREQUENCY	=	107	;
	CO2_485_MODBUS_HUM_SENSOR_HEATING	=	108	;
	CO2_485_MODBUS_INTERNAL_SENSOR_EXIST	=	109	;
	CO2_485_MODBUS_INTERNAL_CO2_PPM	=	110	;
	CO2_485_MODBUS_EXTERNAL_CO2_PPM_START	=	111	;
	CO2_485_MODBUS_ALARM_AUTO_MANUAL	=	161	;
	CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME	=	162	;
	CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME	=	163	;
	CO2_485_MODBUS_ALARM_DELAY_TIME	=	164	;
	CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT	=	165	;
	CO2_485_MODBUS_INT_ALARM_SETPOINT	=	166	;
	CO2_485_MODBUS_INT_CO2_OFFSET	=	167	;
	CO2_485_MODBUS_CO2_SLOPE_DETECT_VALUE	=	168	;
	CO2_485_MODBUS_CO2_FILTER	=	169	;
	CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START	=	170	;
	CO2_485_MODBUS_EXT_ALARM_SETPOINT_START	=	220	;
	CO2_485_MODBUS_EXT_CO2_OFFSET_START	=	270	;
	CO2_485_MODBUS_OUTPUT_AUTO_MANUAL	=	320	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM	=	321	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM	=	322	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2	=	323	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_TEM	=	324	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_TEM	=	325	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_HUM	=	326	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_HUM	=	327	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_CO2	=	328	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_CO2	=	329	;
	CO2_485_MODBUS_INFO_BYTE	=	330	;
	CO2_485_MODBUS_BAUDRATE	=	331	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_1	=	332	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_2	=	333	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_3	=	334	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_4	=	335	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_5	=	336	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_6	=	337	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_7	=	338	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_8	=	339	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_9	=	340	;
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_10	=	341	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_1	=	342	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_2	=	343	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_3	=	344	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_4	=	345	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_5	=	346	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_6	=	347	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_7	=	348	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_8	=	349	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_9	=	350	;
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_10	=	351	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_1	=	352	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_2	=	353	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_3	=	354	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_4	=	355	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_5	=	356	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_6	=	357	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_7	=	358	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_8	=	359	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_9	=	360	;
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_10	=	361	;
	CO2_485_MODBUS_RTC_SEC	=	362	;
	CO2_485_MODBUS_RTC_MIN	=	363	;
	CO2_485_MODBUS_RTC_HOUR	=	364	;
	CO2_485_MODBUS_RTC_DAY	=	365	;
	CO2_485_MODBUS_RTC_WEEK	=	366	;
	CO2_485_MODBUS_RTC_MONTH	=	367	;
	CO2_485_MODBUS_RTC_YEAR	=	368	;
	CO2_485_MODBUS_PASSWORD_ENABLE	=	369	;
	CO2_485_MODBUS_USER_PASSWORD0	=	370	;
	CO2_485_MODBUS_USER_PASSWORD1	=	371	;
	CO2_485_MODBUS_USER_PASSWORD2	=	372	;
	CO2_485_MODBUS_USER_PASSWORD3	=	373	;
	CO2_485_MODBUS_MENU_BLOCK_SECONDS	=	374	;
	CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS	=	375	;
	CO2_485_MODBUS_EXTERNAL_NODES_PLUG_AND_PLAY	=	376	;
	CO2_485_MODBUS_SCAN_DB_CTR	=	377	;
	CO2_485_MODBUS_RESET_SCAN_DB	=	378	;
	CO2_485_MODBUS_SCAN_START	=	379	;
}
else if (fv>=3.0)
{
	CO2_485_MODBUS_SERIALNUMBER_LOWORD	=	0	;
	CO2_485_MODBUS_SERIALNUMBER_HIWORD	=	2	;
	CO2_485_MODBUS_VERSION_NUMBER_LO	=	4	;
	CO2_485_MODBUS_VERSION_NUMBER_HI	=	5	;
	CO2_485_MODBUS_ADDRESS	=	6	;
	CO2_485_MODBUS_PRODUCT_MODEL	=	7	;
	CO2_485_MODBUS_HARDWARE_REV	=	8	;
	CO2_485_MODBUS_PIC_VERSION	=	9	;
	CO2_485_MODBUS_ADDRESS_PLUG_N_PLAY	=	10	;
	CO2_485_MODBUS_BASE_ADDRESS	=	15	;
	CO2_485_MODBUS_UPDATE_STATUS	=	16	;
	CO2_485_MODBUS_SERINALNUMBER_WRITE_FLAG	=	17	;
	CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT	=	100	;
	CO2_485_MODBUS_DEG_C_OR_F	=	101	;
	CO2_485_MODBUS_TEMPERATURE_C_INTERNAL	=	102	;
	CO2_485_MODBUS_TEMPERATURE_F_INTERNAL	=	103	;
	CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL	=	104	;
	CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL	=	105	;
	CO2_485_MODBUS_HUMIDITY_RH	=	106	;
	CO2_485_MODBUS_HUMIDITY_FREQUENCY	=	107	;
	CO2_485_MODBUS_HUM_SENSOR_HEATING	=	108	;
	CO2_485_MODBUS_INTERNAL_SENSOR_EXIST	=	109	;
	CO2_485_MODBUS_INTERNAL_CO2_PPM	=	110	;
	CO2_485_MODBUS_EXTERNAL_CO2_PPM_START	=	111	;

	CO2_485_MODBUS_ALARM_AUTO_MANUAL	=	151	;
	CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME	=	152	;
	CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME	=	153	;
	CO2_485_MODBUS_ALARM_DELAY_TIME	=	154	;
	CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT	=	155	;
	CO2_485_MODBUS_INT_ALARM_SETPOINT	=	156	;
	CO2_485_MODBUS_INT_CO2_OFFSET	=	157	;
	CO2_485_MODBUS_CO2_SLOPE_DETECT_VALUE	=	158	;
	CO2_485_MODBUS_CO2_FILTER	=	159	;
	CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START	=	160	;
	CO2_485_MODBUS_EXT_ALARM_SETPOINT_START	=	200	;
	CO2_485_MODBUS_EXT_CO2_OFFSET_START	=	240	;
	CO2_485_MODBUS_OUTPUT_AUTO_MANUAL	=	280	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM	=	281	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM	=	282	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2	=	283	;
	CO2_485_OIUTPUT_MODE=284;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_TEM	=	285	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_TEM	=	286	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_HUM	=	287	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_HUM	=	288	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_CO2	=	289	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_CO2	=	290	;
	CO2_485_MODBUS_INFO_BYTE	=	291	;
	CO2_485_MODBUS_BAUDRATE	=	292	;
   

	CO2_485_MODBUS_RTC_SEC	=	293	;
	CO2_485_MODBUS_RTC_MIN	=	294	;
	CO2_485_MODBUS_RTC_HOUR	=	295	;
	CO2_485_MODBUS_RTC_DAY	=	296	;
	CO2_485_MODBUS_RTC_WEEK	=	297	;
	CO2_485_MODBUS_RTC_MONTH	=	298	;
	CO2_485_MODBUS_RTC_YEAR	=	299	;
	CO2_485_MODBUS_PASSWORD_ENABLE	=	300	;
	CO2_485_MODBUS_USER_PASSWORD0	=	301	;
	CO2_485_MODBUS_USER_PASSWORD1	=	302	;
	CO2_485_MODBUS_USER_PASSWORD2	=	303	;
	CO2_485_MODBUS_USER_PASSWORD3	=	304	;
	CO2_485_MODBUS_MENU_BLOCK_SECONDS	=	305	;
	CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS	=	306	;
	CO2_485_MODBUS_EXTERNAL_NODES_PLUG_AND_PLAY	=	307	;
	CO2_485_MODBUS_SCAN_DB_CTR	=	308	;
	CO2_485_MODBUS_RESET_SCAN_DB	=	309	;
	CO2_485_MODBUS_SCAN_START	=	310	;

}
else
{

	CO2_485_MODBUS_SERIALNUMBER_LOWORD	=	0	;
	CO2_485_MODBUS_SERIALNUMBER_HIWORD	=	2	;
	CO2_485_MODBUS_VERSION_NUMBER_LO	=	4	;
	CO2_485_MODBUS_VERSION_NUMBER_HI	=	5	;
	CO2_485_MODBUS_ADDRESS	=	6	;
	CO2_485_MODBUS_PRODUCT_MODEL	=	7	;
	CO2_485_MODBUS_HARDWARE_REV	=	8	;
	CO2_485_MODBUS_PIC_VERSION	=	9	;
	CO2_485_MODBUS_ADDRESS_PLUG_N_PLAY	=	10	;
	CO2_485_MODBUS_BASE_ADDRESS	=	15	;
	CO2_485_MODBUS_UPDATE_STATUS	=	16	;
	CO2_485_MODBUS_SERINALNUMBER_WRITE_FLAG	=	17	;
	CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT	=	100	;
	CO2_485_MODBUS_DEG_C_OR_F	=	101	;
	CO2_485_MODBUS_TEMPERATURE_C_INTERNAL	=	102	;
	CO2_485_MODBUS_TEMPERATURE_F_INTERNAL	=	103	;
	CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL	=	104	;
	CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL	=	105	;
	CO2_485_MODBUS_HUMIDITY_RH	=	106	;
	CO2_485_MODBUS_HUMIDITY_FREQUENCY	=	107	;
	CO2_485_MODBUS_HUM_SENSOR_HEATING	=	108	;
	CO2_485_MODBUS_INTERNAL_SENSOR_EXIST	=	109	;
	CO2_485_MODBUS_INTERNAL_CO2_PPM	=	110	;
	CO2_485_MODBUS_EXTERNAL_CO2_PPM_START	=	111	;
	CO2_485_MODBUS_ALARM_AUTO_MANUAL	=	161	;
	CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME	=	162	;
	CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME	=	163	;
	CO2_485_MODBUS_ALARM_DELAY_TIME	=	164	;
	CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT	=	165	;
	CO2_485_MODBUS_INT_ALARM_SETPOINT	=	166	;
	CO2_485_MODBUS_INT_CO2_OFFSET	=	167	;
	CO2_485_MODBUS_CO2_SLOPE_DETECT_VALUE	=	168	;
	CO2_485_MODBUS_CO2_FILTER	=	169	;
	CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START	=	170	;
	CO2_485_MODBUS_EXT_ALARM_SETPOINT_START	=	220	;
	CO2_485_MODBUS_EXT_CO2_OFFSET_START	=	270	;
	CO2_485_MODBUS_OUTPUT_AUTO_MANUAL	=	320	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM	=	321	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM	=	322	;
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2	=	323	;
	CO2_485_OIUTPUT_MODE= 324 ;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_TEM	=	325	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_TEM	=	326	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_HUM	=	327	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_HUM	=	328	;
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_CO2	=	329	;
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_CO2	=	330	;
	CO2_485_MODBUS_INFO_BYTE	=	331	;
	CO2_485_MODBUS_BAUDRATE	=	332	;


	CO2_485_MODBUS_RTC_SEC	=	333	;
	CO2_485_MODBUS_RTC_MIN	=	334	;
	CO2_485_MODBUS_RTC_HOUR	=	335	;
	CO2_485_MODBUS_RTC_DAY	=	336	;
	CO2_485_MODBUS_RTC_WEEK	=	337	;
	CO2_485_MODBUS_RTC_MONTH	=	338	;
	CO2_485_MODBUS_RTC_YEAR	=	339	;
	CO2_485_MODBUS_PASSWORD_ENABLE	=	340	;
	CO2_485_MODBUS_USER_PASSWORD0	=	341	;
	CO2_485_MODBUS_USER_PASSWORD1	=	342	;
	CO2_485_MODBUS_USER_PASSWORD2	=	343	;
	CO2_485_MODBUS_USER_PASSWORD3	=	344	;
	CO2_485_MODBUS_MENU_BLOCK_SECONDS	=	345	;
	CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS	=	346	;
	CO2_485_MODBUS_EXTERNAL_NODES_PLUG_AND_PLAY	=	347	;
	CO2_485_MODBUS_SCAN_DB_CTR	=	348	;
	CO2_485_MODBUS_RESET_SCAN_DB	=	349	;
	CO2_485_MODBUS_SCAN_START	=	350	;
}



}

void CCO2_View::OnEnKillfocusEditCo2Humidity()
{
	// m_humidity_value = (float)(product_register_value[CO2_485_MODBUS_HUMIDITY_RH]/10);
	UpdateData();
	int ivalue=0;
	ivalue = m_humidity_value *10;
	 
		if(ivalue == product_register_value[CO2_485_MODBUS_HUMIDITY_RH])
			return;
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_HUMIDITY_RH,ivalue,
			product_register_value[CO2_485_MODBUS_HUMIDITY_RH],this->m_hWnd,IDC_EDIT_CO2_HUMIDITY,_T("Humidity"));
	 
}


void CCO2_View::OnEnKillfocusEditCo2value()
{
	UpdateData();
	int ivalue=0;
	ivalue = m_co2_value;

	if(ivalue == product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM])
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INTERNAL_CO2_PPM,ivalue,
		product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM],this->m_hWnd,IDC_EDIT_CO2VALUE,_T("CO2 Value"));
}


void CCO2_View::ClickMsflexgridInput()
{
	m_grid_input.SetFocus();
	long lRow,lCol;
	lRow = m_grid_input.get_RowSel();//获取点击的行号	
	lCol = m_grid_input.get_ColSel(); //获取点击的列号
	if(lRow>m_grid_input.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_grid_input.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); 
	//转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_grid_input.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_grid_input.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_grid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_grid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_grid_input.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	if (lRow==1&&lCol==2)//Range
	{
 		m_co2_temp_unit.ShowWindow(SW_SHOW);
 		m_upButton.ShowWindow(SW_HIDE);
 		m_downButton.ShowWindow(SW_HIDE);
 		m_inValueEdit.ShowWindow(SW_HIDE);

		CString strTemp1,strTemp2;
		strTemp1.Format(_T("%cC"),176);
		strTemp2.Format(_T("%cF"),176);
		m_co2_temp_unit.ResetContent();
		m_co2_temp_unit.AddString(strTemp1);
		m_co2_temp_unit.AddString(strTemp2);
		m_co2_temp_unit.ShowWindow(SW_SHOW);

		m_co2_temp_unit.MoveWindow(rc);			//移动到选中格的位置，覆盖
		m_co2_temp_unit.BringWindowToTop();
		m_co2_temp_unit.SelectString(-1,strValue);
		m_co2_temp_unit.SetFocus();	
	}
	if (lCol==5)//Calibration
	{

 		m_co2_temp_unit.ShowWindow(SW_HIDE);
 		m_upButton.ShowWindow(SW_SHOW);
 		m_downButton.ShowWindow(SW_SHOW);
 		m_inValueEdit.ShowWindow(SW_HIDE);

		CRect rcUp(rc.left,rc.top,rc.left+rc.Width()/2,rc.bottom);
		CRect rcDown(rc.left+rc.Width()/2,rc.top,rc.right,rc.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();
	}
	if (lCol==3)
	{
 		m_co2_temp_unit.ShowWindow(SW_SHOW);
 		m_upButton.ShowWindow(SW_HIDE);
 		m_downButton.ShowWindow(SW_HIDE);
 		m_inValueEdit.ShowWindow(SW_HIDE);

		m_co2_temp_unit.ResetContent();
		m_co2_temp_unit.AddString(_T("Auto"));
		m_co2_temp_unit.AddString(_T("Manual"));
		m_co2_temp_unit.ShowWindow(SW_SHOW);

		m_co2_temp_unit.MoveWindow(rc);			//移动到选中格的位置，覆盖
		m_co2_temp_unit.BringWindowToTop();
		m_co2_temp_unit.SelectString(-1,strValue);
		m_co2_temp_unit.SetFocus();	
	}
	if (lCol==4)
	{
	    if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))//Manual
	    {

 			m_co2_temp_unit.ShowWindow(SW_HIDE);
 			m_upButton.ShowWindow(SW_HIDE);
 			m_downButton.ShowWindow(SW_HIDE);
 			m_inValueEdit.ShowWindow(SW_SHOW);
			m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_inValueEdit.ShowWindow(SW_SHOW);

			m_inValueEdit.BringWindowToTop();
			//m_RangCombox.SelectString(-1,strValue);
			m_inValueEdit.SetWindowText(strValue);
			m_inValueEdit.SetFocus(); //获取焦点
	    }
	    

	}
}


void CCO2_View::OnEnKillfocusEditValueGrid()
{


	CString  TempValue;
	m_inValueEdit.GetWindowTextW(TempValue);
		int RegAdd_Ex,RegAdd_IN;
	if (m_nCurRow==1)
	{
 		if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)		
 		{
 			RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_C_INTERNAL;
 			RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL;
 		}
 		else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
 		{
 
 
 			RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_F_INTERNAL;
 			RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL;
 		}
 		else
 		{
 			return;
 		}

		int RegAdd=0;
		unsigned short m_value=(unsigned short)(_wtof(TempValue)*10);
		//if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
		//{
		//	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_IN,m_value,
		//		product_register_value[RegAdd_IN],this->m_hWnd,IDC_EDIT_VALUE_GRID,_T("Calibrate Value"));
		//}
		//else
		//{
		//	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_Ex,m_value,
		//		product_register_value[RegAdd_Ex],this->m_hWnd,IDC_EDIT_VALUE_GRID,_T("Calibrate Value"));
		//}
		//CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM	=	321	;
		//CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM	=	322	;
		//CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2	=	323	;
		if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))
		{
			RegAdd=CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM;
		}
		else
		{
			if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
			{
				 RegAdd=RegAdd_IN;
			}
			else
			{
				RegAdd=RegAdd_Ex;
			}
		}
			Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd,m_value,
				product_register_value[RegAdd],this->m_hWnd,IDC_EDIT_VALUE_GRID,_T("Calibrate Value"));

	}
	if (m_nCurRow==2)
	{
		unsigned short m_value=(unsigned short)(_wtof(TempValue)*10);
		if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))
		{
			RegAdd_IN=CO2_485_MODBUS_HUMIDITY_RH;
		}
		else
		{
			RegAdd_IN=CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM;	
		}
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_IN,m_value,
			product_register_value[RegAdd_IN],this->m_hWnd,IDC_EDIT_VALUE_GRID,_T("Calibrate Value"));
	}
	if (m_nCurRow==3)
	{
		if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))
		{
			RegAdd_IN=CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2;
		}
		else
		{
			RegAdd_IN=CO2_485_MODBUS_INTERNAL_CO2_PPM;
		}
		
			unsigned short m_value=(unsigned short)(_wtoi(TempValue));
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_IN,m_value,
			product_register_value[RegAdd_IN],this->m_hWnd,IDC_EDIT_VALUE_GRID,_T("Calibrate Value"));
	}

	 C02_SHOW_TEMP();



}


void CCO2_View::OnLButtonDown(CPoint point)
{ 

  
 int RegAdd_IN,RegAdd_Ex;
	CRect lRect;
	m_downButton.GetWindowRect(lRect); //获取表格控件的窗口矩形
	if((point.x>=lRect.left && point.x<=lRect.right) && (point.y>=lRect.top && point.y<=lRect.bottom))
	{
		if (m_nCurRow==1)
		{
			if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)		
			{
				RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_C_INTERNAL;
				RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL;
			}
			else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
			{
				RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_F_INTERNAL;
				RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL;
			}
			else
			{
				return;
			}
			CString  TempValue;
			m_value=product_register_value[RegAdd_IN]-1;
			if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
			{
				m_value=product_register_value[RegAdd_IN];
				m_address=RegAdd_IN;
			}
			else
			{
				m_value=product_register_value[RegAdd_Ex];
				m_address=RegAdd_Ex;
			}
		}
		if (m_nCurRow==2)//HUM
		{
		    m_address=CO2_485_MODBUS_HUMIDITY_RH;
		    m_value=product_register_value[CO2_485_MODBUS_HUMIDITY_RH];
		}
		if (m_nCurRow==3)
		{
			m_address=CO2_485_MODBUS_INTERNAL_CO2_PPM;
			m_value=product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM];

		}
	}
   m_upButton.GetWindowRect(lRect);
   if((point.x>=lRect.left && point.x<=lRect.right) && (point.y>=lRect.top && point.y<=lRect.bottom))
   {
	   if (m_nCurRow==1)
	   {
		   if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)		
		   {
			   RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_C_INTERNAL;
			   RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL;
		   }
		   else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
		   {
			   RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_F_INTERNAL;
			   RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL;
		   }
		   else
		   {
			   return;
		   }
		   CString  TempValue;
		   m_value=product_register_value[RegAdd_IN]-1;
		   if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
		   {
			   m_value=product_register_value[RegAdd_IN];
			   m_address=RegAdd_IN;
		   }
		   else
		   {
			   m_value=product_register_value[RegAdd_Ex];
			   m_address=RegAdd_Ex;
		   }
	   }
	   if (m_nCurRow==2)//HUM
	   {
		   m_address=CO2_485_MODBUS_HUMIDITY_RH;
		   m_value=product_register_value[CO2_485_MODBUS_HUMIDITY_RH];
	   }
	   if (m_nCurRow==3)
	   {
		   m_address=CO2_485_MODBUS_INTERNAL_CO2_PPM;
		   m_value=product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM];

	   }
   }
}


//void CCO2_View::OnBnDropDownDownbutton(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


void CCO2_View::OnBnClickedUpbutton()
{
	
	int RegAdd_Ex,RegAdd_IN;
	if (m_nCurRow==1)
	{

		if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)		
		{
			RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_C_INTERNAL;
			RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL;
		}
		else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
		{


			RegAdd_IN=CO2_485_MODBUS_TEMPERATURE_F_INTERNAL;
			RegAdd_Ex=CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL;
		}
		else
		{
			return;
		}

		CString  TempValue;
		m_inValueEdit.GetWindowTextW(TempValue);
		unsigned short m_value=product_register_value[RegAdd_IN]+1;
		if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
		{
			/*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_IN,m_value,
				product_register_value[RegAdd_IN],this->m_hWnd,IDC_DOWNBUTTON,_T("Calibrate Value"));*/
				m_value=product_register_value[RegAdd_IN]+1;

			int ret=	write_one(g_tstat_id,RegAdd_IN,m_value);
			if (ret>0)
			{
			product_register_value[RegAdd_IN]=m_value;
			}
			else
			{
			AfxMessageBox(_T("Write,Fail!"));
			}
			
		}
		else
		{
			/*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_Ex,m_value,
				product_register_value[RegAdd_Ex],this->m_hWnd,IDC_DOWNBUTTON,_T("Calibrate Value"));*/
				m_value=product_register_value[RegAdd_Ex]+1;
			int ret=	write_one(g_tstat_id,RegAdd_Ex,m_value);
			if (ret>0)
			{
				product_register_value[RegAdd_Ex]=m_value;
			}
			else
			{
				AfxMessageBox(_T("Write,Fail!"));
			}

		}
	}
	//strHUM.Format(_T("%0.1f"),(float)(product_register_value[CO2_485_MODBUS_HUMIDITY_RH])/10.0);
	//m_grid_input.put_TextMatrix(2,4,strHUM);
	//strCO2.Format(_T("%d"),product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM]);
	if (m_nCurRow==2)//HUM
	{
		RegAdd_IN=CO2_485_MODBUS_HUMIDITY_RH;
		unsigned short m_value=product_register_value[CO2_485_MODBUS_HUMIDITY_RH]+1;
// 		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_IN,m_value,
// 			product_register_value[RegAdd_IN],this->m_hWnd,IDC_DOWNBUTTON,_T("Calibrate Value"));
		int ret=	write_one(g_tstat_id,RegAdd_IN,m_value);
		if (ret>0)
		{
			product_register_value[RegAdd_IN]=m_value;
		}
		else
		{
			AfxMessageBox(_T("Write,Fail!"));
		}
	}
	if (m_nCurRow==3)
	{
		RegAdd_IN=CO2_485_MODBUS_INTERNAL_CO2_PPM;
		unsigned short m_value=product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM]+1;
// 		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd_IN,m_value,
// 			product_register_value[RegAdd_IN],this->m_hWnd,IDC_DOWNBUTTON,_T("Calibrate Value"));
		int ret=	write_one(g_tstat_id,RegAdd_IN,m_value);
		if (ret>0)
		{
			product_register_value[RegAdd_IN]=m_value;
		}
		else
		{
			AfxMessageBox(_T("Write,Fail!"));
		}
	}
   C02_SHOW_TEMP();
}

 
void CCO2_View::OnTimer(UINT_PTR nIDEvent)
{
	 
	CString TempValue;
			
		if (m_message_up==WM_LBUTTONDOWN)
		{ 
		     ++m_times;
 		    if (m_times<=40)
 		    {
			 m_value+=1;
		    }
 		    else if (m_times>40&&m_times<100)
 		    {
 			 m_value+=10;
 		    }
 			else if (m_times>=150)
 			{
 			 m_value+=100;
 			}
			
		    
			
			if (m_nCurRow!=3)
			{
				TempValue.Format(_T("%0.1f"),(float)m_value/10.0);
			}
			else
			{
				TempValue.Format(_T("%d"),m_value);
			}
			TRACE(TempValue+_T("\n"));
			
			m_grid_input.put_TextMatrix(m_nCurRow,m_nCurCol-1,TempValue);
 			TempValue.Format(_T("%d"),m_times);
 			TRACE(TempValue+_T("\n"));

		}
		if (m_message_down==WM_LBUTTONDOWN)
		{
		    ++m_times;
			 
			if (m_times<=40)
			{
				m_value-=1;
			}
			else if (m_times>40&&m_times<100)
			{
				m_value-=10;
			}
			else if (m_times>=150)
			{
				m_value-=100;
			}

		 
			if (m_nCurRow!=3)
			{
				TempValue.Format(_T("%0.1f"),(float)m_value/10.0);
			}
			else
			{
				TempValue.Format(_T("%d"),m_value);
			}
			m_grid_input.put_TextMatrix(m_nCurRow,m_nCurCol-1,TempValue);
		}

	CFormView::OnTimer(nIDEvent);
}


void CCO2_View::OnEnKillfocusIdCo2Edit()
{
	 
}

#include "MainFrm.h"
void CCO2_View::OnCbnSelchangeCo2Braudratecombo()
{
	g_bPauseMultiRead = TRUE;

	   write_one(g_tstat_id, CO2_485_MODBUS_BAUDRATE, m_co2_braudRateCombox.GetCurSel());
		//MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		/*else
		product_register_value[CO2_485_MODBUS_BAUDRATE] = m_co2_braudRateCombox.GetCurSel();*/
	   CString SqlText;
	   if (m_co2_braudRateCombox.GetCurSel()==0)
	   {
		   SqlText.Format(_T("update ALL_NODE set Bautrate = '9600' where Serial_ID='%d'"),get_serialnumber());
		  // Change_BaudRate(9600);
	   } 
	   else
	   {
		   SqlText.Format(_T("update ALL_NODE set Bautrate = '19200' where Serial_ID='%d'"),get_serialnumber());
		  // Change_BaudRate(19200);
	   }
	   CBADO bado;
	   bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	   bado.OnInitADOConn(); 
	   bado.m_pConnection->Execute(SqlText.GetString(),NULL,adCmdText);

	   CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	   pFrame->ScanTstatInDB();
	g_bPauseMultiRead = FALSE;
}
