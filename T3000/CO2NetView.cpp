// CO2NetView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2NetView.h"
#include "MainFrm.h"
#include "ado/ADO.h"
#include "Dialog_Progess.h"
#define CO2NETVIEWFRESH   WM_USER+20000

// CCO2NetView
DWORD WINAPI _ReadMultiRegisters_CO2(LPVOID pParam)
{
	CCO2NetView* pView=(CCO2NetView*)(pParam);
	BOOL bFirst=TRUE;
	while(1)
	{
		if (!is_connect())
		{
			Sleep(10);
			continue;
		}
		Sleep(2000);
		for(int i=0;i<27;i++) //Modify 
		{
			register_critical_section.Lock();
			int multy_ret = 0;

			multy_ret = Read_Multi(g_tstat_id,&pView->product_register_value[i*100],i*100,100);
			register_critical_section.Unlock();
		}

		if(pView->m_hWnd!=NULL)
		{
			::PostMessage(pView->m_hWnd,CO2NETVIEWFRESH,0,0);
		}

		 

	}



	return 1;
}
IMPLEMENT_DYNCREATE(CCO2NetView, CFormView)

CCO2NetView::CCO2NetView()
	: CFormView(CCO2NetView::IDD)
{
Flag_Reg=FALSE;
}


CCO2NetView::~CCO2NetView()
{if(hFirstThread != NULL)
TerminateThread(hFirstThread, 0);
}

void CCO2NetView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ID_CO2_EDIT, m_edit_IDAddress);
    DDX_Control(pDX, IDC_SERIALNUM_CO2_EDIT, m_edit_SN);
    DDX_Control(pDX, IDC_FIRMWARV_CO2_EDIT, m_Edit_FVevsion);
    DDX_Control(pDX, IDC_HARDWAREV_CO2_EDIT, m_Edit_HardVer);
    DDX_Control(pDX, IDC_CO2_BRAUDRATECOMBO, m_Combox_braudrate);
    DDX_Control(pDX, IDC_EDIT_CO2_HUMIDITY, m_Edit_RHum);
    DDX_Control(pDX, IDC_RADIO_ALARM_MANUAL, m_AlarmSetting_M);
    DDX_Control(pDX, IDC_RADIO_ALARM_AUTO, m_AlarmSetting_Auto);
    DDX_Control(pDX, IDC_CO2_ALARM_STATE, m_Combox_CO2State);

    DDX_Control(pDX, IDC_EDIT_CO2_PASSWOR, m_Edit_Password);
    DDX_Control(pDX, IDC_COMBO_IPModel, m_Combox_IpModel);
    DDX_Control(pDX, IDC_MAC_ADDRESS, m_Edit_MACAddress);
    DDX_Control(pDX, IDC_IPADDRESS_IP, m_IPCtrl_IpAddress);
    DDX_Control(pDX, IDC_IPADDRESS_Gateway, m_IpCtrl_Gateway);
    DDX_Control(pDX, IDC_IPADDRESS_Subnet, m_IpCtrl_Subnet);
    DDX_Control(pDX, IDC_EDIT_ListeningPort, m_Edit_Port);
    DDX_Control(pDX, IDC_EDIT_EXTERNAL_TEMP, m_Edit_External_Temp);
    DDX_Control(pDX, IDC_EDIT_INTERNAL_TEMP, m_Internal_Temp);
    DDX_Control(pDX, IDC_CO2_TEMP_UNIT, m_Combox_TempUnit);
    DDX_Control(pDX, IDC_COMBO_CO2_SENSOR_SEL, m_Edit_SensorSelect);
    DDX_Control(pDX, IDC_CO2_CALIBRATING_OFFSET, m_Edit_Calibrate_Offset);
    DDX_Control(pDX, IDC_CO2_ALARM_SETPOINT, m_Edit_Alarm_Setpoint);
    DDX_Control(pDX, IDC_CO2_PREPARE_ALARM_SETPOINT, m_Edit_PreAlarm_Setpoint);
    DDX_Control(pDX, IDC_CO2_ALARM_ON_TIME, m_Edit_AlarmOn);
    DDX_Control(pDX, IDC_CO2_ALARM_OFF_TIME, m_Edit_AlarmOff);
    DDX_Control(pDX, IDC_EDIT_CO2_BLOCK_TIME, m_Edit_MenuBlockTimes);
    DDX_Control(pDX, IDC_EDIT_CO2_BACKLIGHT_TIME, m_Edit_BacklightTime);
    DDX_Control(pDX, IDC_RADIO_HUMIDITY_HEAT_ENABLE, m_HeatingHumiditySensor_Enable);
    DDX_Control(pDX, IDC_RADIO_HUMIDITY_HEAT_DISABLE, m_HeatingHumiditySensor_Disable);
    DDX_Control(pDX, IDC_ENABLE_PASSWORD, m_Check_Enable_Password);
    DDX_Control(pDX, IDC_MSFLEXGRID1, m_msflexgrid);
    DDX_Control(pDX, IDC_CO2_DATETIMEPICKER_TIME, m_co2_time_picker);
    DDX_Control(pDX, IDC_CO2_DATETIMEPICKER1, m_co2_day_picker);
}

BEGIN_MESSAGE_MAP(CCO2NetView, CFormView)
    ON_CBN_SELCHANGE(IDC_CO2_TEMP_UNIT, &CCO2NetView::OnCbnSelchangeCo2TempUnit)
    ON_BN_CLICKED(IDC_BTN_CO2_CLEAR_CAL, &CCO2NetView::OnBnClickedBtnCo2ClearCal)
    ON_BN_CLICKED(IDC_RADIO_ALARM_MANUAL, &CCO2NetView::OnBnClickedRadioAlarmManual)
    ON_BN_CLICKED(IDC_RADIO_ALARM_AUTO, &CCO2NetView::OnBnClickedRadioAlarmAuto)
    ON_CBN_SELCHANGE(IDC_CO2_ALARM_STATE, &CCO2NetView::OnCbnSelchangeCo2AlarmState)
    ON_BN_CLICKED(IDC_ENABLE_PASSWORD, &CCO2NetView::OnBnClickedEnablePassword)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_PASSWOR, &CCO2NetView::OnEnKillfocusEditCo2Passwor)
    ON_CBN_SELCHANGE(IDC_COMBO_IPModel, &CCO2NetView::OnCbnSelchangeComboIpmodel)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CCO2NetView::OnBnClickedButtonApply)
    ON_BN_CLICKED(IDC_CO2_ENABLEIDBUTTON, &CCO2NetView::OnBnClickedCo2Enableidbutton)
    ON_EN_KILLFOCUS(IDC_ID_CO2_EDIT, &CCO2NetView::OnEnKillfocusIdCo2Edit)
    ON_EN_KILLFOCUS(IDC_CO2_PREPARE_ALARM_SETPOINT, &CCO2NetView::OnEnKillfocusCo2PrepareAlarmSetpoint)
    ON_EN_KILLFOCUS(IDC_CO2_ALARM_SETPOINT, &CCO2NetView::OnEnKillfocusCo2AlarmSetpoint)
    ON_CBN_SELCHANGE(IDC_COMBO_CO2_SENSOR_SEL, &CCO2NetView::OnCbnSelchangeComboCo2SensorSel)
    ON_EN_KILLFOCUS(IDC_CO2_CALIBRATING_OFFSET, &CCO2NetView::OnEnKillfocusCo2CalibratingOffset)
    ON_EN_KILLFOCUS(IDC_CO2_ALARM_ON_TIME, &CCO2NetView::OnEnKillfocusCo2AlarmOnTime)
    ON_EN_KILLFOCUS(IDC_CO2_ALARM_OFF_TIME, &CCO2NetView::OnEnKillfocusCo2AlarmOffTime)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_BLOCK_TIME, &CCO2NetView::OnEnKillfocusEditCo2BlockTime)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_BACKLIGHT_TIME, &CCO2NetView::OnEnKillfocusEditCo2BacklightTime)
    ON_BN_CLICKED(IDC_BUTTON_CO2_SYNC_TIME, &CCO2NetView::OnBnClickedButtonCo2SyncTime)
    ON_BN_CLICKED(IDC_BTN_CO2_REFRESH, &CCO2NetView::OnBnClickedBtnCo2Refresh)
	ON_MESSAGE(CO2NETVIEWFRESH, OnFreshView)
END_MESSAGE_MAP()


// CCO2NetView diagnostics
void CCO2NetView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
     
}
#ifdef _DEBUG
void CCO2NetView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCO2NetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CCO2NetView::Fresh(){
	if (!Flag_Reg)
	{
		Initial_Registerlist();
		Flag_Reg=TRUE;
	}
	for(int i=0;i<27;i++){
		Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
	}
	Initial_Window();
	if(hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
	hFirstThread=NULL;
	if (!hFirstThread)
	{
		hFirstThread = CreateThread(NULL,NULL,_ReadMultiRegisters_CO2,this,NULL,0);
	}
}
void CCO2NetView::Initial_Registerlist(){
    CO2_NET_MODBUS_SERIALNUMBER_LOWORD	=	0	;
    CO2_NET_MODBUS_SERIALNUMBER_HIWORD	=	2	;
    CO2_NET_MODBUS_SOFTWARE_VERSION_LO 	=	4	;
    CO2_NET_MODBUS_SOFTWARE_VERSION_HI 	=	5	;
    CO2_NET_MODBUS_ADDRESS 	=	6	;
    CO2_NET_MODBUS_PRODUCT_MODEL 	=	7	;
    CO2_NET_MODBUS_HARDWARE_REV	=	8	;
    CO2_NET_MODBUS_PIC_REV	=	9	;
    CO2_NET_MODBUS_ADDRESS_PLUG_N_PLAY	=	10	;
    CO2_NET_MODBUS_TIME_ZONE	=	11	;
    CO2_NET_MODBUS_BAUDRATE 	=	12	;
    CO2_NET_MODBUS_DAYLIGHT_ENABLE	=	13	;
    CO2_NET_MODBUS_BOOTLOADER_VERSION	=	14	;
    CO2_NET_MODBUS_RESET_FLASH	=	15	;
    CO2_NET_MODBUS_UPDATE_STATUS 	=	16	;
    CO2_NET_MODBUS_INIT_WR_TIME	=	17	;
    CO2_NET_MODBUS_TIME_SERVER_SELECT 	=	22	;
    CO2_NET_MODBUS_SERINALNUMBER_WRITE_FLAG	=	25	;
    CO2_NET_MODBUS_REFRESH_STATUS 	=	90	;
    CO2_NET_MODBUS_REFRESH_FLASH	=	91	;
    CO2_NET_MODBUS_REFRESH_FLASH_FREQ 	=	92	;
    CO2_NET_MODBUS_MAC_ADDRESS_CHANGE_ENABLE	=	93	;
    CO2_NET_MODBUS_REFRESH_FLASH_TIMER	=	94	;
    CO2_NET_MODBUS_MAC_ADDRESS_START	=	100	;
    CO2_NET_MODBUS_IP_MODE	=	106	;
    CO2_NET_MODBUS_IP_ADDRESS_START	=	107	;
    CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START	=	111	;
    CO2_NET_MODBUS_GATEWAY_ADDRESS_START	=	115	;
    CO2_NET_MODBUS_TCP_SERVER_ENABLE 	=	119	;
    CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START	=	120	;

    CO2_NET_MODBUS_IP_MODE_GHOST	=	121	;

    CO2_NET_MODBUS_IP_ADDRESS_GHOST_START	=	122	;
    CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START	=	126	;
    CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START	=	130	;
    CO2_NET_MODBUS_TCP_SERVER_ENABLE_GHOST 	=	134	;
    CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_GHOST_START 	=	135	;
    CO2_NET_MODBUS_ENABLE_GHOST	=	136	;
    CO2_NET_MODBUS_SCAN_COMMAND	=	172	;
    CO2_NET_MODBUS_SUBNET_TYPE 	=	173	;
    CO2_NET_MODBUS_NTP_COMMAND	=	174	;
    CO2_NET_MODBUS_TIME_SERVER0_IP_START	=	175	;
    CO2_NET_MODBUS_TIME_SERVER1_IP_START	=	176	;
    CO2_NET_MODBUS_TIME_SERVER2_IP_START	=	183	;
    CO2_NET_MODBUS_TIME_SERVER3_IP_START	=	187	;
    CO2_NET_MODBUS_TIME_SERVER4_IP_START	=	191	;
    CO2_NET_MODBUS_TIME_SERVER5_IP_START	=	195	;
    CO2_NET_MODBUS_TIME_SYNC_RESULT	=	199	;
    CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT	=	200	;
    CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F	=	201	;
    CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS	=	202	;
    CO2_NET_MODBUS_INTERNAL_TEMPERATURE_FAHRENHEIT	=	203	;
    CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_CELSIUS	=	204	;
    CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_FAHRENHEIT	=	205	;
    CO2_NET_MODBUS_TEMPERATURE_OFFSET_INTERNAL	=	206	;
    CO2_NET_MODBUS_HUMIDITY	=	207	;
    CO2_NET_MODBUS_HUMIDITY_FREQUENCY	=	208	;
    CO2_NET_MODBUS_HUMIDITY_CALIBRATION_TABLE_COUNTER	=	209	;
    CO2_NET_MODBUS_CO2_INTERNAL_EXIST	=	210	;
    CO2_NET_MODBUS_CO2_INTERNAL	=	211	;
    CO2_NET_MODBUS_CO2_INTERNAL_OFFSET	=	212	;
    CO2_NET_MODBUS_CO2_INTERNAL_PREALARM_SETPOINT	=	213	;
    CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT	=	214	;
    CO2_NET_MODBUS_CO2_EXTERNAL_START	=	215	;
    CO2_NET_MODBUS_CO2_EXTERANL_END	=	468	;
    CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_START	=	469	;
    CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_END	=	722	;
    CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_START	=	723	;
    CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_END	=	976	;
    CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_START 	=	977	;
    CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_END	=	1230	;
    CO2_NET_MODBUS_CO2_SLOPE_DETECT_VALUE	=	1231	;
    CO2_NET_MODBUS_CO2_FILTER	=	1232	;
    CO2_NET_MODBUS_PASSWORD_ENABLE	=	1233	;
    CO2_NET_MODBUS_USER_PASSWORD0 	=	1234	;
    CO2_NET_MODBUS_USER_PASSWORD1	=	1235	;
    CO2_NET_MODBUS_USER_PASSWORD2	=	1236	;
    CO2_NET_MODBUS_USER_PASSWORD3	=	1237	;
    CO2_NET_MODBUS_RTC_CENTURY	=	1238	;
    CO2_NET_MODBUS_RTC_YEAR	=	1239	;
    CO2_NET_MODBUS_RTC_MONTH	=	1240	;
    CO2_NET_MODBUS_RTC_DAY	=	1241	;
    CO2_NET_MODBUS_RTC_WEEK	=	1242	;
    CO2_NET_MODBUS_RTC_HOUR	=	1243	;
    CO2_NET_MODBUS_RTC_MINUTE	=	1244	;
    CO2_NET_MODBUS_RTC_SECOND	=	1245	;
    CO2_NET_MODBUS_ALARM_AUTO_MANUAL	=	1246	;
    CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME	=	1247	;
    CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME	=	1248	;
    CO2_NET_MODBUS_ALARM_DELAY_TIME	=	1249	;
    CO2_NET_MODBUS_OUTPUT_AUTO_MANUAL	=	1250	;
    CO2_NET_MODBUS_OUTPUT_MANUAL_VALUE_TEM	=	1251	;
    CO2_NET_MODBUS_OUTPUT_MANUAL_VALUE_HUM	=	1252	;
    CO2_NET_MODBUS_OUTPUT_MANUAL_VALUE_CO2	=	1253	;
    CO2_NET_MODBUS_OUTPUT_MODE	=	1254	;
    CO2_NET_MODBUS_OUTPUT_RANGE_MIN_TEM	=	1255	;
    CO2_NET_MODBUS_OUTPUT_RANGE_MAX_TEM	=	1256	;
    CO2_NET_MODBUS_OUTPUT_RANGE_MIN_HUM	=	1257	;
    CO2_NET_MODBUS_OUTPUT_RANGE_MAX_HUM	=	1258	;
    CO2_NET_MODBUS_OUTPUT_RANGE_MIN_CO2	=	1259	;
    CO2_NET_MODBUS_OUTPUT_RANGE_MAX_CO2	=	1260	;
    CO2_NET_MODBUS_MENU_BLOCK_SECONDS	=	1261	;
    CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS	=	1262	;
    CO2_NET_MODBUS_EXTERNAL_NODES_PLUG_AND_PLAY 	=	1263	;
    CO2_NET_MODBUS_SCAN_DB_CTR	=	1264	;
    CO2_NET_MODBUS_RESET_SCAN_DB	=	1265	;
    CO2_NET_MODBUS_SCAN_START	=	1266	;
    CO2_NET_MODBUS_SCAN_END 	=	2535	;
    CO2_NET_MODBUS_GET_NODES_PARA_START 	=	2536	;
    CO2_NET_MODBUS_GET_NODES_PARA_END 	=	2568	;
    CO2_NET_MODBUS_SCAN_OCCUPY_START	=	2600	;
    CO2_NET_MODBUS_SCAN_OCCUPY_END	=	2632	;
    CO2_NET_MODBUS_SCAN_ONLINE_START	=	10000	;
    CO2_NET_MODBUS_SCAN_ONLINE_END	=	10001	;
    CO2_NET_MODBUS_IDLE_ID	=	10002	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_READ	=	10003	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM0	=	10004	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ0	=	10005	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM1	=	10006	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ1	=	10007	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM2	=	10008	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ2	=	10009	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM3	=	10010	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ3	=	10011	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM4	=	10012	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ4	=	10013	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM5	=	10014	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ5	=	10015	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM6	=	10016	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ6	=	10017	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM7	=	10018	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ7	=	10019	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM8	=	10020	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ8	=	10021	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_HUM9	=	10022	;
    CO2_NET_MODBUS_HUMIDITY_SENSOR_TABLE_FREQ9	=	10023	;

}
void CCO2NetView::Initial_Window(){
CString strTemp;
int IntTemp;
IntTemp=product_register_value[CO2_NET_MODBUS_ADDRESS];
strTemp.Format(_T("%d"),IntTemp);
m_edit_IDAddress.SetWindowText(strTemp);

IntTemp=product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD]+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD+1]*255+
        product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD]*255*255+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD+1]*255*255*255;
strTemp.Format(_T("%d"),IntTemp);
m_edit_SN.SetWindowText(strTemp);
strTemp.Format(_T("%u.%u"),product_register_value[CO2_NET_MODBUS_SOFTWARE_VERSION_HI],product_register_value[CO2_NET_MODBUS_SOFTWARE_VERSION_LO]);
m_Edit_FVevsion.SetWindowText(strTemp);
strTemp.Format(_T("%u"),product_register_value[CO2_NET_MODBUS_HARDWARE_REV]);
m_Edit_HardVer.SetWindowText(strTemp);

m_Combox_braudrate.ResetContent();
m_Combox_braudrate.AddString(_T("9600"));
m_Combox_braudrate.AddString(_T("19200"));
m_Combox_braudrate.SetCurSel(product_register_value[CO2_NET_MODBUS_BAUDRATE]);
float fTemp;
fTemp = (float)product_register_value[CO2_NET_MODBUS_HUMIDITY]/10;
if(fTemp>100)
{
    (CEdit *)GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->EnableWindow(FALSE);
    //m_humidity_value = 0;
    m_Edit_RHum.SetWindowText(_T("0"));
}
else
{
    strTemp.Format(_T("%0.1f"),fTemp);
    m_Edit_RHum.SetWindowText(strTemp);
    (CEdit *)GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->EnableWindow(TRUE);
}
//Alarm Setting 
 unsigned short temp_data=product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL];
 temp_data=temp_data>>7;
 if(temp_data==1)
 {
      m_AlarmSetting_M.SetCheck(1);
      m_AlarmSetting_Auto.SetCheck(0);
 }
 else
 {
     m_AlarmSetting_M.SetCheck(0);
     m_AlarmSetting_Auto.SetCheck(1);
 }
 temp_data=product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL];
 temp_data=temp_data & 0x0003;
 if(temp_data<3)
     m_Combox_CO2State.SetCurSel(temp_data);
 //Password Setting
 Show_PassWord();
 //TCP
 Show_TCPIP();
 Show_Temperature();
 //
 //CO2_NET_MODBUS_CO2_INTERNAL_EXIST	=	210	;
 //CO2_NET_MODBUS_CO2_INTERNAL	=	211	;
 //CO2_NET_MODBUS_CO2_INTERNAL_OFFSET	=	212	;
 //CO2_NET_MODBUS_CO2_INTERNAL_PREALARM_SETPOINT	=	213	;
 //CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT	=	214	;
 //CEdit m_Edit_Calibrate_Offset;
 //CEdit m_Edit_Alarm_Setpoint;
 //CEdit m_Edit_PreAlarm_Setpoint;
 strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_OFFSET]);
  m_Edit_Calibrate_Offset.SetWindowText(strTemp);
 strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_PREALARM_SETPOINT]);
 m_Edit_PreAlarm_Setpoint.SetWindowText(strTemp);
 strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT]);
  m_Edit_Alarm_Setpoint.SetWindowText(strTemp);



  //CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME	=	1247	;
  //CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME	=	1248	;
  //CO2_NET_MODBUS_MENU_BLOCK_SECONDS	=	1261	;
  //CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS	=	1262	;
  // CEdit m_Edit_AlarmOn;
  //CEdit m_Edit_AlarmOff;
  //CEdit m_Edit_MenuBlockTimes;
  //CEdit m_Edit_BacklightTime;
   strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME]);
   m_Edit_AlarmOn.SetWindowText(strTemp);
   strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
   m_Edit_AlarmOff.SetWindowText(strTemp);
   strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
   m_Edit_MenuBlockTimes.SetWindowText(strTemp);
   strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
   m_Edit_BacklightTime.SetWindowText(strTemp);

   //Draw Grid
   //T("NUM"), 40, ListCtrlEx::Normal, LVC
   //    _ID, _T("Device ID"), 80, ListCtrlEx:
   //_NUM, _T("Serial Number"), 80, ListCt
   //    T("External PPM"), 100, ListCtrlEx::E
   //    ARM_SP, _T("Prepare Alarm Setpoint"),
   //    SP, _T("Alarm Setpoint"), 100, ListCt
   //    FSET, _T("Calibrating Offset"), 100, 
   m_msflexgrid.put_Cols(7);
   m_msflexgrid.put_Rows(255);
   m_msflexgrid.put_TextMatrix(0,0,_T("Num"));
   m_msflexgrid.put_TextMatrix(0,1,_T("Device ID"));
   m_msflexgrid.put_TextMatrix(0,2,_T("Serial Number"));
   m_msflexgrid.put_TextMatrix(0,3,_T("External PPM"));
   m_msflexgrid.put_TextMatrix(0,4,_T("Prepare Alarm Setpoint"));
   m_msflexgrid.put_TextMatrix(0,5,_T("Alarm Setpoint"));
   m_msflexgrid.put_TextMatrix(0,6,_T("Calibrating Offset"));
   Fresh_Grid();
 
}
void CCO2NetView::Fresh_Grid(){
    //CO2_NET_MODBUS_SCAN_START	=	1266	;ID+SN
    //CO2_NET_MODBUS_SCAN_END 	=	2535	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_START	=	215	;
    //CO2_NET_MODBUS_CO2_EXTERANL_END	=	468	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_START	=	469	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_END	=	722	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_START	=	723	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_END	=	976	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_START 	=	977	;
    //CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_END	=	1230	;
    CString StrTemp;
    int Temp;
    for (int i=1;i<255;i++)
    {
      if (product_register_value[CO2_NET_MODBUS_SCAN_START+5*(i-1)]==0)
      {
      continue;
      }
      StrTemp.Format(_T("%d"),i); 
      m_msflexgrid.put_TextMatrix(i,0,StrTemp);
      StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_SCAN_START+5*(i-1)]); 
      m_msflexgrid.put_TextMatrix(i,1,StrTemp);
      Temp=product_register_value[CO2_NET_MODBUS_SCAN_START+5*(i-1)+1]+
           product_register_value[CO2_NET_MODBUS_SCAN_START+5*(i-1)+2]*255+
           product_register_value[CO2_NET_MODBUS_SCAN_START+5*(i-1)+3]*255*255+
           product_register_value[CO2_NET_MODBUS_SCAN_START+5*(i-1)+4]*255*255*255;
     StrTemp.Format(_T("%d"),Temp); 
     m_msflexgrid.put_TextMatrix(i,2,StrTemp);




     //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_PPM,temp_cs_ppm);
     //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_PRE_ALARM_SP,temp_cs_pre_alarm_sp);
     //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_ALARM_SP,temp_cs_alarm_sp);
     //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_CAL_OFFSET,temp_cs_cal_offset);
    /* temp_cs_pre_alarm_sp.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START +  i- 1]);
     temp_cs_alarm_sp.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXT_ALARM_SETPOINT_START + i - 1]);
     temp_cs_cal_offset.Format(_T("%d"),(short)product_register_value[CO2_485_MODBUS_EXT_CO2_OFFSET_START + i - 1]);*/

     if(product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_START + i - 1] == 65535)
     {
         StrTemp.Format(_T("No Sensor"));   
     }
     else
     {
         StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_START + i - 1]);  
     }
     m_msflexgrid.put_TextMatrix(i,3,StrTemp);

     StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_START + i - 1]); 
     m_msflexgrid.put_TextMatrix(i,4,StrTemp);
     StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_START + i - 1]); 
     m_msflexgrid.put_TextMatrix(i,5,StrTemp);
     StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_START + i - 1]); 
     m_msflexgrid.put_TextMatrix(i,6,StrTemp);
       
    }
}
void CCO2NetView::Show_PassWord()
{
    if (product_register_value[CO2_NET_MODBUS_PASSWORD_ENABLE]==0)
    {
    m_Check_Enable_Password.SetCheck(0);
    } 
    else
    {
    m_Check_Enable_Password.SetCheck(1);
    }
    CString PassWord=_T("");
    CString temp_password[4];
    temp_password[0].Format(_T("%c"),product_register_value[CO2_NET_MODBUS_USER_PASSWORD0]);
    temp_password[1].Format(_T("%c"),product_register_value[CO2_NET_MODBUS_USER_PASSWORD1]);
    temp_password[2].Format(_T("%c"),product_register_value[CO2_NET_MODBUS_USER_PASSWORD2]);
    temp_password[3].Format(_T("%c"),product_register_value[CO2_NET_MODBUS_USER_PASSWORD3]);
    for (int i=0;i<4;i++)
    {
        PassWord = PassWord + temp_password[i];
    }
    m_Edit_Password.SetWindowText(PassWord);
    
} 
void CCO2NetView::Show_TCPIP(){
CString strtemp;
    strtemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+1],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+2],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+3],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+4],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+5]);
    m_Edit_MACAddress.SetWindowText(strtemp);

    m_Combox_IpModel.SetCurSel(product_register_value[CO2_NET_MODBUS_IP_MODE]);
    //CO2_NET_MODBUS_IP_ADDRESS_START
    //CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START
    //CO2_NET_MODBUS_GATEWAY_ADDRESS_START
    //CO2_NET_MODBUS_TCP_SERVER_ENABLE
    //CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START
    m_IPCtrl_IpAddress.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START],
    (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START+1],
    (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START+2],
    (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START+3]);
    m_IpCtrl_Gateway.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START],
    (BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START+1],
    (BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START+2],
    (BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START+3]);
    m_IpCtrl_Subnet.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START],
    (BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START+1],
    (BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START+2],
    (BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START+3]);
    strtemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START]);
    m_Edit_Port.SetWindowText(strtemp);
}
void CCO2NetView::Show_Temperature(){
    m_Edit_SensorSelect.ResetContent();
    m_Edit_SensorSelect.InsertString(0,_T("Internal"));
    m_Edit_SensorSelect.InsertString(1,_T("External"));
    m_Edit_SensorSelect.SetCurSel(product_register_value[CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT]);

    CString strTemp1,strTemp2;
    strTemp1.Format(_T("%cC"),176);
    strTemp2.Format(_T("%cF"),176);
    m_Combox_TempUnit.ResetContent();
    m_Combox_TempUnit.AddString(strTemp1);
    m_Combox_TempUnit.AddString(strTemp2);
    m_Combox_TempUnit.SetCurSel(product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F]);

    //CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT	=	200	;

    //CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F	=	201	;

    //CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS	=	202	;
    //CO2_NET_MODBUS_INTERNAL_TEMPERATURE_FAHRENHEIT	=	203	;

    //CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_CELSIUS	=	204	;
    //CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_FAHRENHEIT	=	205	;
    //CO2_NET_MODBUS_TEMPERATURE_OFFSET_INTERNAL	=	206	;
  
    CString i_internal_temp,i_external_temp;
    float temp;
    if(product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F] == 0)		
    {   temp=(float)product_register_value[CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS]/10.0;
        i_internal_temp.Format(_T("%0.1f"),temp);
        temp=(float)product_register_value[CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_CELSIUS]/10.0;
        i_external_temp.Format(_T("%0.1f"),temp);
        i_external_temp+=strTemp1;
        i_internal_temp+=strTemp1;
        m_Internal_Temp.SetWindowText(i_internal_temp);
        m_Edit_External_Temp.SetWindowText(i_external_temp);
     
        
    }
    else if((product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F] == 1))
    {
        temp=(float)product_register_value[CO2_NET_MODBUS_INTERNAL_TEMPERATURE_FAHRENHEIT]/10.0;
        i_internal_temp.Format(_T("%0.1f"),temp);
        temp=(float)product_register_value[CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_FAHRENHEIT]/10.0;
        i_external_temp.Format(_T("%0.1f"),temp);
        i_external_temp+=strTemp2;
        i_internal_temp+=strTemp2;
        m_Internal_Temp.SetWindowText(i_internal_temp);
        m_Edit_External_Temp.SetWindowText(i_external_temp);
 
    }
    else
    {
        return;
    }

}
void CCO2NetView::Get_CO2_Temperature_unit(CString &strTemp)
{
    UINT uint_temp=GetOEMCP();//get system is for chinese or english
    if(uint_temp!=936 && uint_temp!=950)
    {
        if(product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F]==0)	//121
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
        if(product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F]==0)//121
        {
            strTemp=_T("℃");
        }
        else
        {
            strTemp=_T("℉");
        }
    }
}

#include "Table_Tamplete.h"

void CCO2NetView::OnBnClickedBtnCo2ClearCal()
{
     CTable_Tamplete dlg;
    // dlg=new CTable_Tamplete;
   //  dlg->SetWindowText(_T("External Calibration List"));
     dlg.DoModal();
}

LRESULT CCO2NetView::OnFreshView(WPARAM wParam, LPARAM lParam){
	Fresh();
	return 0;
}

void CCO2NetView::OnBnClickedRadioAlarmManual()
{
    unsigned short temp_value;
    temp_value = product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL] | 0x80;
    if(temp_value != product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_ALARM_AUTO_MANUAL,temp_value,
            product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL],this->m_hWnd,IDC_RADIO_ALARM_MANUAL,_T("Alarm Auto Manual Mode"));
    }
}


void CCO2NetView::OnBnClickedRadioAlarmAuto()
{
    unsigned short temp_value;
    temp_value = product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL] & 0x7f;
    if(temp_value != product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_ALARM_AUTO_MANUAL,temp_value,
            product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL],this->m_hWnd,IDC_RADIO_ALARM_AUTO,_T("Alarm Auto Manual Mode"));
    }
}


void CCO2NetView::OnCbnSelchangeCo2AlarmState()
{

    int temp_item =	m_Combox_CO2State.GetCurSel();
    int write_value =0;
    write_value = (product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL] & 0xfc) | temp_item;
    if(write_value != product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_ALARM_AUTO_MANUAL,write_value,
            product_register_value[CO2_NET_MODBUS_ALARM_AUTO_MANUAL],this->m_hWnd,IDC_CO2_ALARM_STATE,_T("Alarm State"));
    }
}


void CCO2NetView::OnBnClickedEnablePassword()
{
  int temp_value= m_Check_Enable_Password.GetCheck();
  write_one(g_tstat_id,CO2_NET_MODBUS_PASSWORD_ENABLE,temp_value);
  product_register_value[CO2_NET_MODBUS_PASSWORD_ENABLE]=temp_value;
  Show_PassWord();
}


void CCO2NetView::OnEnKillfocusEditCo2Passwor()
{
     CString password;
     m_Edit_Password.GetWindowText(password);
     if (password.GetLength()!=4)
     {
      AfxMessageBox(_T("The password length must be four chars"));
      return;
     } 
     
     for (int i=0;i<password.GetLength();i++)
     {
       unsigned short temp=password.GetAt(i);
       write_one(g_tstat_id,CO2_NET_MODBUS_USER_PASSWORD0 + i,temp);
       product_register_value[CO2_NET_MODBUS_USER_PASSWORD0 + i]=temp;
     }
     Show_PassWord();
}





void CCO2NetView::OnBnClickedCo2Enableidbutton()
{
     
    if(m_edit_IDAddress.IsWindowEnabled())
    {
        m_edit_IDAddress.EnableWindow(FALSE);
        ((CButton *)GetDlgItem(IDC_CO2_ENABLEIDBUTTON))->SetWindowTextW(_T("Enable Change"));
    }
    else
    {
        m_edit_IDAddress.EnableWindow(TRUE);
        ((CButton *)GetDlgItem(IDC_CO2_ENABLEIDBUTTON))->SetWindowTextW(_T("Disable Change"));
    }
}

int CCO2NetView::get_serialnumber(){
    return product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD]+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD+1]*256+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD]*256*256+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256;

}
void CCO2NetView::OnEnKillfocusIdCo2Edit()
{
    CString newid;
    GetDlgItem(IDC_ID_CO2_EDIT)->GetWindowText(newid);
    int serialno=get_serialnumber();
    int ID=_wtoi(newid);
    if (newid!=g_tstat_id)
    {
        int ret=write_one(g_tstat_id,6,ID);
        if (ret>0)
        {
            CADO ado;
            ado.OnInitADOConn();
            CString sql;
            sql.Format(_T("select * from ALL_NODE where Serial_ID='%d' "),serialno);
            ado.m_pRecordset = ado.OpenRecordset(sql);
            ado.m_pRecordset->MoveFirst();
            while(!ado.m_pRecordset->EndOfFile)
            {
                CString prodcut_name,product_id,screen_name;
                prodcut_name.Format(_T("%s:%d--%d"),GetProductName(product_register_value[7]),serialno,ID);
                product_id.Format(_T("%d"),ID);
                screen_name.Format(_T("Screen(S:%d--%d)"),serialno,ID);

                try 
                {
                    ado.m_pRecordset->PutCollect("Product_name",(_bstr_t)(prodcut_name));
                    ado.m_pRecordset->PutCollect("Product_ID",(_bstr_t)(product_id));
                    ado.m_pRecordset->PutCollect("Screen_Name",(_bstr_t)(screen_name));
                    ado.m_pRecordset->Update();
                    ado.m_pRecordset->MoveNext();

                }
                catch(...)
                {


                }
                g_tstat_id=ID;

                CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
                ::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
            }
        } 
        else
        {
            AfxMessageBox(_T("Fail,Please try again!"));
        }
    } 
    else
    {
        AfxMessageBox(_T("The same ID ,you input!"));
    }
}


void CCO2NetView::OnEnKillfocusCo2PrepareAlarmSetpoint()
{

    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_OFFSET]);
    //m_Edit_Calibrate_Offset.SetWindowText(strTemp);
     CString strTemp;
     m_Edit_PreAlarm_Setpoint.GetWindowText(strTemp);
     unsigned short IntTemp=_wtoi(strTemp);
    write_one(g_tstat_id,CO2_NET_MODBUS_CO2_INTERNAL_PREALARM_SETPOINT,IntTemp);
    product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_PREALARM_SETPOINT]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_PREALARM_SETPOINT]);
    m_Edit_PreAlarm_Setpoint.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT]);
    //m_Edit_Alarm_Setpoint.SetWindowText(strTemp);

}


void CCO2NetView::OnEnKillfocusCo2AlarmSetpoint()
{
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_OFFSET]);
    //m_Edit_Calibrate_Offset.SetWindowText(strTemp);
    CString strTemp;
    m_Edit_Alarm_Setpoint.GetWindowText(strTemp);
    unsigned short IntTemp=_wtoi(strTemp);
    write_one(g_tstat_id,CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT,IntTemp);
    product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT]);
    m_Edit_Alarm_Setpoint.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT]);
    //m_Edit_Alarm_Setpoint.SetWindowText(strTemp);
}
void CCO2NetView::OnEnKillfocusCo2CalibratingOffset()
{
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_OFFSET]);
    //m_Edit_Calibrate_Offset.SetWindowText(strTemp);
    CString strTemp;
    m_Edit_Calibrate_Offset.GetWindowText(strTemp);
    unsigned short IntTemp=_wtoi(strTemp);
    write_one(g_tstat_id,CO2_NET_MODBUS_CO2_INTERNAL_OFFSET,IntTemp);
    product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_OFFSET]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_OFFSET]);
    m_Edit_Calibrate_Offset.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL_ALARM_SETPOINT]);
    //m_Edit_Alarm_Setpoint.SetWindowText(strTemp);
}
void CCO2NetView::OnCbnSelchangeCo2TempUnit()
{
    int sel=m_Combox_TempUnit.GetCurSel();
    write_one(g_tstat_id,CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F,sel);
    product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F]=sel;
    Show_Temperature();
}
void CCO2NetView::OnCbnSelchangeComboCo2SensorSel()
{
     int sel=m_Edit_SensorSelect.GetCurSel();
     write_one(g_tstat_id,CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT,sel);
     product_register_value[CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT]=sel;
   
   Show_Temperature();
}
////////////////////////IP Config/////////////////////////////////////
void CCO2NetView::OnCbnSelchangeComboIpmodel()
{
    
}


void CCO2NetView::OnBnClickedButtonApply()
{
     int m_nListenPort=0;
     BeginWaitCursor();
	//m_nListenPort	
	CString strText;
	m_Edit_Port.GetWindowText(strText);
	if(!strText.IsEmpty())
		m_nListenPort=_wtoi(strText);
	if(m_nListenPort>=12767)
	{
		AfxMessageBox(_T("The listen port number is too big, please change it."));
		return;
	}
	if(m_nListenPort<=0)
	{
		AfxMessageBox(_T("The listen port number must be greater than 0!"));
		return;
	}
	write_one(g_tstat_id,CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_GHOST_START,m_nListenPort);
    product_register_value[CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_GHOST_START]=m_nListenPort;
	BYTE address1,address2,address3,address4;
	m_IPCtrl_IpAddress.GetAddress(address1,address2,address3,address4);
int n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START,address1);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START+1,address2);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START+2,address3);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START+3,address4);
     
	m_IpCtrl_Gateway.GetAddress(address1,address2,address3,address4);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+0,address1);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+1,address2);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+2,address3);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+3,address4);
	m_IpCtrl_Subnet.GetAddress(address1,address2,address3,address4);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START,address1);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START+1,address2);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START+2,address3);
	n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START+3,address4);

    n=write_one(g_tstat_id,CO2_NET_MODBUS_ENABLE_GHOST,1);//使能之后
    Sleep(5*1000);



	CMainFrame* pPraent=(CMainFrame*)GetParent();

	CString strBuilding,strSubBuilding;
	strBuilding=pPraent->m_strCurMainBuildingName;
	strSubBuilding=pPraent->m_strCurSubBuldingName;
	CString strIP,strPort;
	strPort.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_GHOST_START]);
     // BYTE address1,address2,address3,address4;
	m_IPCtrl_IpAddress.GetAddress(address1,address2,address3,address4);
	strIP.Format(_T("%d.%d.%d.%d"),address1,address2,address3,address4);
	if(g_CommunicationType==1)//TCP
	{
		int nPort;
		nPort=_wtoi(strPort);
		//m_strIP=build_info.strIp; 
		g_CommunicationType=1;
		SetCommunicationType(g_CommunicationType);
		BOOL bOk=FALSE;
		bOk=Open_Socket2(strIP,nPort);
		CString strInfo;

		strInfo.Format((_T("Open IP:%s successful")),strIP);//prompt info;
		SetPaneString(1,strInfo);
		if(bOk)
		{
			try
			{

			_ConnectionPtr t_pCon;//for ado connection
			t_pCon.CreateInstance(_T("ADODB.Connection"));
			t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
			CString strSql;
			//strSql.Format(_T("update Building set Ip_Address='%s' where Ip_Address='%s'"),strIP,pPraent->m_strIP);
			//t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

			// 改node
			CString strSID;
          
			strSID.Format(_T("%d"), get_serialnumber());
			strSql.Format(_T("update ALL_NODE set Bautrate='%s',Com_Port='%s' where Serial_ID='%s'"),strIP,strPort,strSID); //bautrate 放IP
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);


			if(t_pCon->State)
				t_pCon->Close();
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			pPraent->m_strIP=strIP;
			pPraent->ScanTstatInDB();

		}
		else
		{

		}



	}
	EndWaitCursor();
}




void CCO2NetView::OnEnKillfocusCo2AlarmOnTime()
{
   CString strTemp;
   unsigned short IntTemp;
   m_Edit_AlarmOn.GetWindowText(strTemp);
   IntTemp=_wtoi(strTemp);
   write_one(g_tstat_id,CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME,IntTemp);
   product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME]);
    m_Edit_AlarmOn.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
    //m_Edit_AlarmOff.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
    //m_Edit_MenuBlockTimes.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
    //m_Edit_BacklightTime.SetWindowText(strTemp);
}


void CCO2NetView::OnEnKillfocusCo2AlarmOffTime()
{
    CString strTemp;
    unsigned short IntTemp;
    m_Edit_AlarmOff.GetWindowText(strTemp);
    IntTemp=_wtoi(strTemp);
    write_one(g_tstat_id,CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME,IntTemp);
    product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
    m_Edit_AlarmOff.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
    //m_Edit_AlarmOff.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
    //m_Edit_MenuBlockTimes.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
    //m_Edit_BacklightTime.SetWindowText(strTemp);
}


void CCO2NetView::OnEnKillfocusEditCo2BlockTime()
{
    CString strTemp;
    unsigned short IntTemp;
    m_Edit_MenuBlockTimes.GetWindowText(strTemp);
    IntTemp=_wtoi(strTemp);
    write_one(g_tstat_id,CO2_NET_MODBUS_MENU_BLOCK_SECONDS,IntTemp);
    product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
    m_Edit_MenuBlockTimes.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
    //m_Edit_AlarmOff.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
    //m_Edit_MenuBlockTimes.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
    //m_Edit_BacklightTime.SetWindowText(strTemp);
}


void CCO2NetView::OnEnKillfocusEditCo2BacklightTime()
{
    CString strTemp;
    unsigned short IntTemp;
    m_Edit_BacklightTime.GetWindowText(strTemp);
    IntTemp=_wtoi(strTemp);
    write_one(g_tstat_id,CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS,IntTemp);
    product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]=IntTemp;
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
    m_Edit_BacklightTime.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
    //m_Edit_AlarmOff.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
    //m_Edit_MenuBlockTimes.SetWindowText(strTemp);
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
    //m_Edit_BacklightTime.SetWindowText(strTemp);
}


void CCO2NetView::OnBnClickedButtonCo2SyncTime()
{
     CTime TimeTemp=TimeTemp.GetCurrentTime();
     m_co2_time_picker.SetTime(&TimeTemp);
     m_co2_day_picker.SetTime(&TimeTemp);
     UpdateData();
     Check_DayTime();
     Check_HourTime();
}
void CCO2NetView::Check_DayTime()
{


   

    CTime temp_time;
    int cyear,cmonth,cday;
    m_co2_day_picker.GetTime(temp_time);
    cyear = temp_time.GetYear();
    if(cyear>1800)
        cyear = cyear%100;
    cmonth = temp_time.GetMonth();
    cday = temp_time.GetDay();
    if(cyear != product_register_value[CO2_NET_MODBUS_RTC_YEAR])
    {
        if(write_one(g_tstat_id,CO2_NET_MODBUS_RTC_YEAR,cyear,3)<0)
        {
            MessageBox(_T("Operate failure,Please try again"));
            return;
        }
    }

    if(cmonth != product_register_value[CO2_NET_MODBUS_RTC_MONTH])
    {
        if(write_one(g_tstat_id,CO2_NET_MODBUS_RTC_MONTH,cmonth,3)<0)
        {
            MessageBox(_T("Operate failure,Please try again"));
            return;
        }
    }

    if(cday != product_register_value[CO2_NET_MODBUS_RTC_DAY])
    {
        if(write_one(g_tstat_id,CO2_NET_MODBUS_RTC_DAY,cday,3)<0)
        {
            MessageBox(_T("Operate failure,Please try again"));
            return;
        }
    }
}

void CCO2NetView::Check_HourTime()
{


  

    CTime temp_time;
    int chour,cmin,csec;
    m_co2_time_picker.GetTime(temp_time);
    chour = temp_time.GetHour();
    cmin = temp_time.GetMinute();
    csec = temp_time.GetSecond();

    if(chour != product_register_value[CO2_NET_MODBUS_RTC_HOUR])
    {
        if(write_one(g_tstat_id,CO2_NET_MODBUS_RTC_HOUR,chour,3)<0)
        {
            MessageBox(_T("Operate failure,Please try again"));
            return;
        }
    }

    if(cmin != product_register_value[CO2_NET_MODBUS_RTC_MINUTE])
    {
        if(write_one(g_tstat_id,CO2_NET_MODBUS_RTC_MINUTE,cmin,3)<0)
        {
            MessageBox(_T("Operate failure,Please try again"));
            return;
        }
    }
}

void CCO2NetView::OnBnClickedBtnCo2Refresh()
{
    CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
    //创建对话框窗口
    pDlg->Create(IDD_DIALOG10_Progress, this);

    //居中显示
    //pDlg->CenterWindow();
    //void MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );
    //pDlg->MoveWindow(100,100,500,1000);
    pDlg->ShowProgress(0,0);
    //显示对话框窗口
    pDlg->ShowWindow(SW_SHOW);
    RECT RECT_SET1;
    GetClientRect(&RECT_SET1);
    pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);
    float progress;
    int it=0;
    for(int i=0;i<27;i++){
        Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
        if (pDlg!=NULL)
        {
            progress=float((it+1)*(100/27));
            pDlg->ShowProgress(int(progress),(int)progress);
            ++it;
        } 
    }
    Initial_Window();
    pDlg->ShowProgress(100,100);
    pDlg->ShowWindow(SW_HIDE);
    if(pDlg!=NULL)
        delete pDlg;
    pDlg=NULL;

}
