// CO2NetView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2NetView.h"
#include "MainFrm.h"
 #include "../SQLiteDriver/CppSQLite3.h"
#include "Dialog_Progess.h"
#define CO2NETVIEWFRESH   WM_USER+1008

// CCO2NetView
DWORD WINAPI _ReadMultiRegisters_CO2(LPVOID pParam)
{
    CCO2NetView* pView=(CCO2NetView*)(pParam);
    BOOL bFirst=TRUE;
	//return 0;
    while(pView->IsWindowVisible())
    {
	 
		if (!is_connect())
		{
			Sleep(10);
			continue;
		}
        Sleep(5000);
        //register_critical_section.Lock();
        //for(int i=0; i<12; i++) //Modify
        //{
        //    int multy_ret = 0;
        //    multy_ret = Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
        //}
        //register_critical_section.Unlock();

	/*	register_critical_section.Lock();
		for(int i=0; i<11; i++)
		{
			int multy_ret = 0;
			multy_ret = Read_Multi(g_tstat_id,&pView->External_ArrayData[i*100],pView->CO2_NET_MODBUS_CO2_EXTERNAL_START+i*100,100);
		}
		register_critical_section.Unlock();*/

		/*register_critical_section.Lock();
		for(int i=0; i<13; i++)
		{
			int multy_ret = 0;
			multy_ret = Read_Multi(g_tstat_id,&pView->Scan_ArrayData[i*100],pView->CO2_NET_MODBUS_SCAN_START+i*100,100);
		}
		register_critical_section.Unlock();*/


		//if(pView->m_hWnd!=NULL)
		//{
		//	::SendMessage(pView->m_hWnd,CO2NETVIEWFRESH,0,0);
  //      }



    }

    return 1;
}
IMPLEMENT_DYNCREATE(CCO2NetView, CFormView)

CCO2NetView::CCO2NetView()
    : CFormView(CCO2NetView::IDD)
{
    Flag_Reg=FALSE;
	memset(External_ArrayData,0,1100);
	memset(Scan_ArrayData,0,1300);
}


CCO2NetView::~CCO2NetView()
{

    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);

}

void CCO2NetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_CO2_EDIT, m_edit_IDAddress);
	DDX_Control(pDX, IDC_SERIALNUM_CO2_EDIT, m_edit_SN);
	DDX_Control(pDX, IDC_FIRMWARV_CO2_EDIT, m_Edit_FVevsion);
	DDX_Control(pDX, IDC_HARDWAREV_CO2_EDIT, m_Edit_HardVer);
	DDX_Control(pDX, IDC_CO2_BAUDRATECOMBO, m_Combox_baudrate);
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
	DDX_Control(pDX, IDC_EDIT_CO2_VALUE, m_edit_co2_value);
	//  DDX_Control(pDX, IDC_CO2_INPUT_LIST, m_co2_inputs_list);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_grid_input);
	DDX_Control(pDX, IDC_LIST_USER, m_user_list);
	DDX_Control(pDX, IDC_LIST_OUTPUT_AQ, m_output_list);
	DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
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
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_VALUE, &CCO2NetView::OnEnKillfocusEditCo2Value)
    ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
    ON_MESSAGE(MY_READ_DATA_CALLBACK, ReadDataCallBack)

    ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CCO2NetView::OnEnKillfocusEditName)
    ON_BN_CLICKED(IDC_RADIO_CO2_CAL_ENABLE, &CCO2NetView::OnBnClickedRadioCo2CalEnable)
    ON_BN_CLICKED(IDC_RADIO_CO2_CAL_DISABLE, &CCO2NetView::OnBnClickedRadioCo2CalDisable)
    ON_BN_CLICKED(IDC_RADIO_HUMIDITY_HEAT_ENABLE, &CCO2NetView::OnBnClickedRadioHumidityHeatEnable)
END_MESSAGE_MAP()


// CCO2NetView diagnostics
void CCO2NetView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    m_brush.CreateSolidBrush(RGB(255,0,0));
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

void CCO2NetView::Fresh()
{

    if (!Flag_Reg)
    {
        Initial_Registerlist();
        Flag_Reg=TRUE;
    }
    g_NEED_MULTI_READ = FALSE;
      //register_critical_section.Lock();
      //for(int i=0; i<40; i++)
      //{
      //    Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
      //}
      //register_critical_section.Unlock();

    Initial_Window();
   
    product_register_value[3131] = read_one(g_tstat_id, 3131);
    if (product_register_value[3131] == 0)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_CO2_CAL_ENABLE))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_CO2_CAL_DISABLE))->SetCheck(0);
    }
    else if(product_register_value[3131] == 1)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_CO2_CAL_ENABLE))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_CO2_CAL_DISABLE))->SetCheck(1);
    }
	if (product_register_value[7] == STM32_CO2_RS485)
	{
		GetDlgItem(IDC_STATIC_IP_INFOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_IP_MODEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_IPModel)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAC_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MAC_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_IPADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GATEWAY_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SUBNET_MASK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IPADDRESS_IP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IPADDRESS_Gateway)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IPADDRESS_Subnet)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_IP_PORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ListeningPort)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_HIDE);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_IP_INFOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_IP_MODEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_IPModel)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MAC_ADDRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MAC_ADDRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_IPADDRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_GATEWAY_ADDRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SUBNET_MASK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IPADDRESS_IP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IPADDRESS_Gateway)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IPADDRESS_Subnet)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_IP_PORT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ListeningPort)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_SHOW);
	}

    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    hFirstThread=NULL;
    if (!hFirstThread)
    {
        hFirstThread = CreateThread(NULL,NULL,_ReadMultiRegisters_CO2,this,NULL,0);
    }
}
void CCO2NetView::Fresh_View()
{
    Initial_Window();
}
void CCO2NetView::Initial_Registerlist()
{
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
void CCO2NetView::Initial_Window()
{
    CString strTemp;
    int IntTemp;
    IntTemp=product_register_value[CO2_NET_MODBUS_ADDRESS];
    strTemp.Format(_T("%d"),IntTemp);
    m_edit_IDAddress.SetWindowText(strTemp);

    IntTemp=product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD]+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD+1]*256+
            product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD]*256*256+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256;
    strTemp.Format(_T("%d"),IntTemp);
    m_edit_SN.SetWindowText(strTemp);
    strTemp.Format(_T("%0.1f"),((float)(product_register_value[CO2_NET_MODBUS_SOFTWARE_VERSION_HI]*256+product_register_value[CO2_NET_MODBUS_SOFTWARE_VERSION_LO]))/10);
    m_Edit_FVevsion.SetWindowText(strTemp);
    strTemp.Format(_T("%u"),product_register_value[CO2_NET_MODBUS_HARDWARE_REV]);
    m_Edit_HardVer.SetWindowText(strTemp);

    m_Combox_baudrate.ResetContent();
    m_Combox_baudrate.AddString(_T("9600"));
    m_Combox_baudrate.AddString(_T("19200"));
	m_Combox_baudrate.AddString(_T("38400"));
	m_Combox_baudrate.AddString(_T("57600"));
	m_Combox_baudrate.AddString(_T("115200"));
	if (product_register_value[7] == STM32_CO2_NET||product_register_value[7] == STM32_CO2_RS485)
	{
		CO2_NET_MODBUS_BAUDRATE = 15;
	}
	 
    m_Combox_baudrate.SetCurSel(product_register_value[CO2_NET_MODBUS_BAUDRATE]);
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
    strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL]);
    m_edit_co2_value.SetWindowText(strTemp);
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


    Fresh_Grid();
    Show_InputList();
	Initial_OutputList();
	Initial_VarList();
}
void CCO2NetView::Show_InputList()
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

    //CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS	=	202	;
    //CO2_NET_MODBUS_INTERNAL_TEMPERATURE_FAHRENHEIT	=	203	;
    //CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_CELSIUS	=	204	;
    //CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_FAHRENHEIT	=	205	;
    if(product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F] == 0)
    {
        i_internal_temp = product_register_value[CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS];
        i_external_temp = product_register_value[CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_CELSIUS];
        // 		((CStatic *)GetDlgItem(IDC_STATIC_CO2_UNIT1))->SetWindowText(strTemp1);
        // 		((CStatic *)GetDlgItem(IDC_STATIC_CO2_UNIT2))->SetWindowText(strTemp1);
        /*m_co2_temp_unit.SetCurSel(0);*/
        strUnit=strTemp1;
    }
    else if((product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F] == 1))
    {
        i_internal_temp = product_register_value[CO2_NET_MODBUS_INTERNAL_TEMPERATURE_FAHRENHEIT];
        i_external_temp = product_register_value[CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_FAHRENHEIT];
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

    if(product_register_value[CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
    {
        TempValue.Format(_T("%0.1f"),f_internal_temp);
    }
    else
    {
        TempValue.Format(_T("%0.1f"),f_external_temp);
    }
    m_grid_input.put_TextMatrix(1,4,TempValue);

    BOOL AM=Get_Bit_FromRegister(product_register_value[CO2_NET_MODBUS_OUTPUT_AUTO_MANUAL],1);
    if (AM)
    {
        StrAM=_T("Manual");
    }
    else
    {
        StrAM=_T("Auto");
    }
    m_grid_input.put_TextMatrix(1,3,StrAM);

    AM=Get_Bit_FromRegister(product_register_value[CO2_NET_MODBUS_OUTPUT_AUTO_MANUAL],2);
    if (AM)
    {
        StrAM=_T("Manual");
    }
    else
    {
        StrAM=_T("Auto");
    }
    m_grid_input.put_TextMatrix(2,3,StrAM);

    AM=Get_Bit_FromRegister(product_register_value[CO2_NET_MODBUS_OUTPUT_AUTO_MANUAL],3);
    if (AM)
    {
        StrAM=_T("Manual");
    }
    else
    {
        StrAM=_T("Auto");
    }
    m_grid_input.put_TextMatrix(3,3,StrAM);
    //CO2_NET_MODBUS_HUMIDITY	=	207	;

    strHUM.Format(_T("%0.1f"),(float)(product_register_value[CO2_NET_MODBUS_HUMIDITY])/10.0);
    m_grid_input.put_TextMatrix(2,4,strHUM);
    strCO2.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_INTERNAL]);
    m_grid_input.put_TextMatrix(3,4,strCO2);
}
void CCO2NetView::Fresh_Grid()
{
    
    CString StrTemp;
    int Temp;
    for (int i=1; i<255; i++)
    {
	   // Read_Multi(g_tstat_id,TempDataArray,CO2_NET_MODBUS_SCAN_START+(i-1)*5,5);
        if (Scan_ArrayData[(i-1)*5]==0)
        {
            continue; 
        }
        StrTemp.Format(_T("%d"),i);
        m_msflexgrid.put_TextMatrix(i,0,StrTemp);
        StrTemp.Format(_T("%d"),Scan_ArrayData[CO2_NET_MODBUS_SCAN_START+(i-1)*5]);
        m_msflexgrid.put_TextMatrix(i,1,StrTemp);
        Temp=Scan_ArrayData[(i-1)*5+1]+
             Scan_ArrayData[(i-1)*5+2]*255+
             Scan_ArrayData[(i-1)*5+3]*255*255+
             Scan_ArrayData[(i-1)*5+4]*255*255*255;
        StrTemp.Format(_T("%d"),Temp);
        m_msflexgrid.put_TextMatrix(i,2,StrTemp);


        //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_PPM,temp_cs_ppm);
        //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_PRE_ALARM_SP,temp_cs_pre_alarm_sp);
        //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_ALARM_SP,temp_cs_alarm_sp);
        //m_co2_external_sensor_list.SetItemText(i-1,CO2_EXTERNAL_CAL_OFFSET,temp_cs_cal_offset);
        /* temp_cs_pre_alarm_sp.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START +  i- 1]);
         temp_cs_alarm_sp.Format(_T("%d"),product_register_value[CO2_485_MODBUS_EXT_ALARM_SETPOINT_START + i - 1]);
         temp_cs_cal_offset.Format(_T("%d"),(short)product_register_value[CO2_485_MODBUS_EXT_CO2_OFFSET_START + i - 1]);*/

        if(External_ArrayData[	i - 1 ] == 65535 || External_ArrayData[ i-1 ] == 0)
        {
            StrTemp.Format(_T("No Sensor"));
			m_msflexgrid.put_TextMatrix(i,3,StrTemp);
			//StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_START + i - 1]);
			StrTemp=_T("");
			m_msflexgrid.put_TextMatrix(i,4,StrTemp);
			//StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_START + i - 1]);
			StrTemp=_T("");
			m_msflexgrid.put_TextMatrix(i,5,StrTemp);
			//StrTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_START + i - 1]);
			StrTemp=_T("");
			m_msflexgrid.put_TextMatrix(i,6,StrTemp);
        }
        else
        {
            StrTemp.Format(_T("%d"),External_ArrayData[ i - 1]);
			m_msflexgrid.put_TextMatrix(i,3,StrTemp);
			StrTemp.Format(_T("%d"),External_ArrayData[CO2_NET_MODBUS_CO2_EXTERNAL_PREALARM_SETPOINT_START + i - 1-CO2_NET_MODBUS_CO2_EXTERNAL_START]);
			m_msflexgrid.put_TextMatrix(i,4,StrTemp);
			StrTemp.Format(_T("%d"),External_ArrayData[CO2_NET_MODBUS_CO2_EXTERNAL_ALARM_SETPOINT_START + i - 1-CO2_NET_MODBUS_CO2_EXTERNAL_START]);
			m_msflexgrid.put_TextMatrix(i,5,StrTemp);
			StrTemp.Format(_T("%d"),External_ArrayData[CO2_NET_MODBUS_CO2_EXTERNAL_OFFSET_START + i - 1-CO2_NET_MODBUS_CO2_EXTERNAL_START]);
			m_msflexgrid.put_TextMatrix(i,6,StrTemp);
        }
       

    }
}
void CCO2NetView::Show_PassWord()
{
	unsigned short DataTemp[10];
	Read_Multi(g_tstat_id,DataTemp,CO2_NET_MODBUS_PASSWORD_ENABLE,10);
	if (DataTemp[CO2_NET_MODBUS_PASSWORD_ENABLE-CO2_NET_MODBUS_PASSWORD_ENABLE]==0)
	{
		m_Check_Enable_Password.SetCheck(0);
	}
	else
	{
		m_Check_Enable_Password.SetCheck(1);
    }
    CString PassWord=_T("");
    CString temp_password[4];
    temp_password[0].Format(_T("%c"),DataTemp[CO2_NET_MODBUS_USER_PASSWORD0-CO2_NET_MODBUS_PASSWORD_ENABLE]);
    temp_password[1].Format(_T("%c"),DataTemp[CO2_NET_MODBUS_USER_PASSWORD1-CO2_NET_MODBUS_PASSWORD_ENABLE]);
    temp_password[2].Format(_T("%c"),DataTemp[CO2_NET_MODBUS_USER_PASSWORD2-CO2_NET_MODBUS_PASSWORD_ENABLE]);
    temp_password[3].Format(_T("%c"),DataTemp[CO2_NET_MODBUS_USER_PASSWORD3-CO2_NET_MODBUS_PASSWORD_ENABLE]);
    for (int i=0; i<4; i++)
    {
        PassWord = PassWord + temp_password[i];
    }
    m_Edit_Password.SetWindowText(PassWord);

}
void CCO2NetView::Show_TCPIP()
{   
	if (product_register_value[7] == STM32_CO2_RS485)
	{
		return;
	}
    if (product_register_value[7] == STM32_CO2_NET)
    {
		CO2_NET_MODBUS_MAC_ADDRESS_START = 40;
    }
    CString strtemp;
    strtemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+1],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+2],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+3],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+4],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+5]);
    m_Edit_MACAddress.SetWindowText(strtemp);
	if (product_register_value[7] == STM32_CO2_NET)
	{
		CO2_NET_MODBUS_IP_MODE = 46;
	}
    m_Combox_IpModel.SetCurSel(product_register_value[CO2_NET_MODBUS_IP_MODE]);
    //CO2_NET_MODBUS_IP_ADDRESS_START
    //CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START
    //CO2_NET_MODBUS_GATEWAY_ADDRESS_START
    //CO2_NET_MODBUS_TCP_SERVER_ENABLE
    //CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START
	if (product_register_value[7] == STM32_CO2_NET)
	{
		CO2_NET_MODBUS_IP_ADDRESS_START = 47;
		CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START = 51;
		CO2_NET_MODBUS_GATEWAY_ADDRESS_START = 55;
		CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START = 60;
	}
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
void CCO2NetView::Show_Temperature()
{
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
    {
        temp=(float)product_register_value[CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS]/10.0;
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



void CCO2NetView::OnBnClickedBtnCo2ClearCal()
{

}

LRESULT CCO2NetView::OnFreshView(WPARAM wParam, LPARAM lParam)
{
    Fresh_View();
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

    for (int i=0; i<password.GetLength(); i++)
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

int CCO2NetView::get_serialnumber()
{
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
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
            CString sql;
            sql.Format(_T("select * from ALL_NODE where Serial_ID='%d' "),serialno);
            q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
            
            if(!q.eof())
            {
                CString prodcut_name,product_id,screen_name;
                prodcut_name.Format(_T("%s:%d--%d"),GetProductName(product_register_value[7]),serialno,ID);
                product_id.Format(_T("%d"),ID);
                screen_name.Format(_T("Screen(S:%d--%d)"),serialno,ID);

                try
                {
                    
				 
						CString strSql;
					strSql.Format(_T("Update ALL_NODE set Product_name = '%s' ,Product_ID = '%s' ,Screen_Name = '%s'  where Serial_ID='%d' "),
						prodcut_name,
						product_id,
						screen_name,
						serialno);
						SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                }
                catch(...)
                {


                }
                g_tstat_id=ID;

                CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
                ::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
            }

            SqliteDBBuilding.closedb();
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
    Show_InputList();
}

void CCO2NetView::OnCbnSelchangeComboCo2SensorSel()
{
    int sel=m_Edit_SensorSelect.GetCurSel();
    write_one(g_tstat_id,CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT,sel);
    product_register_value[CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT]=sel;

    Show_Temperature();

    Show_InputList();
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
	if (product_register_value[7] == STM32_CO2_NET)
	{
		CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_GHOST_START = 60+15;
		CO2_NET_MODBUS_IP_ADDRESS_GHOST_START = 47+15; 
		CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START = 51+15;
		CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START = 55+15;
		CO2_NET_MODBUS_ENABLE_GHOST = 76;
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
				CppSQLite3DB SqliteDBBuilding;
				CppSQLite3Table table;
				CppSQLite3Query q;
				SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


                CString strSql;
                //strSql.Format(_T("update Building set Ip_Address='%s' where Ip_Address='%s'"),strIP,pPraent->m_strIP);
                //t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

                // 改node
                CString strSID;

                strSID.Format(_T("%d"), get_serialnumber());
                strSql.Format(_T("update ALL_NODE set Bautrate='%s',Com_Port='%s' where Serial_ID='%s'"),strIP,strPort,strSID); //bautrate 放IP
                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

                SqliteDBBuilding.closedb();

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

    if(product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME]==IntTemp)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME,IntTemp,
                        product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME],this->m_hWnd,IDC_CO2_ALARM_ON_TIME,_T("Alarm on Time"));

    //write_one(g_tstat_id,CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME,IntTemp);
    //product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME]=IntTemp;
    // strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_ON_TIME]);
    // m_Edit_AlarmOn.SetWindowText(strTemp);
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
    if(product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]==IntTemp)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME,IntTemp,
                        product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME],this->m_hWnd,IDC_CO2_ALARM_OFF_TIME,_T("Alarm off Time"));

    /*  write_one(g_tstat_id,CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME,IntTemp);
      product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]=IntTemp;
      strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_PRE_ALARM_SETTING_OFF_TIME]);
      m_Edit_AlarmOff.SetWindowText(strTemp);*/
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

    if(product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]==IntTemp)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_MENU_BLOCK_SECONDS,IntTemp,
                        product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS],this->m_hWnd,IDC_EDIT_CO2_BLOCK_TIME,_T("Menu Block Time"));
    /* write_one(g_tstat_id,CO2_NET_MODBUS_MENU_BLOCK_SECONDS,IntTemp);
     product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]=IntTemp;
     strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_MENU_BLOCK_SECONDS]);
     m_Edit_MenuBlockTimes.SetWindowText(strTemp);*/
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

    //write_one(g_tstat_id,CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS,IntTemp);
    //product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]=IntTemp;
    //strTemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]);
    //m_Edit_BacklightTime.SetWindowText(strTemp);

    if(product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS]==IntTemp)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS,IntTemp,
                        product_register_value[CO2_NET_MODBUS_BACKLIGHT_KEEP_SECONDS],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("Backlight Time"));

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
    //CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
    //创建对话框窗口
    //pDlg->Create(IDD_DIALOG10_Progress, this);

    //居中显示
    //pDlg->CenterWindow();
    //void MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );
    //pDlg->MoveWindow(100,100,500,1000);
    //pDlg->ShowProgress(0,0);
    //显示对话框窗口
    //pDlg->ShowWindow(SW_SHOW);
   // RECT RECT_SET1;
   // GetClientRect(&RECT_SET1);
    //pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);
    float progress;
    int it=0;
    for(int i=0; i<4; i++)
    {
        Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
       // if (pDlg!=NULL)
       // {
            progress=float((it+1)*(100/4));
            g_progress_persent = progress;
            //pDlg->ShowProgress(int(progress),(int)progress);
            ++it;
       // }
            Sleep(100);
    }
    Initial_Window();
   // pDlg->ShowProgress(100,100);
   // pDlg->ShowWindow(SW_HIDE);
   // if(pDlg!=NULL)
   //     delete pDlg;
   // pDlg=NULL;

}

void CCO2NetView::OnEnKillfocusEditCo2Value()
{
    CString str;
    //str.Format(_T("%d"),product_register_value[275]);
    //GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->GetWindowText(str);
    m_edit_co2_value.GetWindowText(str);
    int nValue= _wtoi(str);

    if(product_register_value[CO2_NET_MODBUS_CO2_INTERNAL]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_NET_MODBUS_CO2_INTERNAL,nValue,
                        product_register_value[CO2_NET_MODBUS_CO2_INTERNAL],this->m_hWnd,IDC_EDIT_CO2_VALUE,_T("CO2 Value"));
}

HBRUSH CCO2NetView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

    
    for (int i=0; i<(int)Change_Color_ID.size(); i++)
    {
        if(pWnd->GetDlgCtrlID()==Change_Color_ID.at(i))//注意此处的（pWnd->），否则没效果
        {
            pDC->SetTextColor(RGB(0,0,0));
            pDC->SetBkColor(RGB(255,0,0));//设置文本背景色
            pDC->SetBkMode(TRANSPARENT);//设置背景透明
            hbr = (HBRUSH)m_brush;
        }

    }
    
    return hbr;
}

LRESULT  CCO2NetView::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
    UINT temp_id;
    _MessageWriteOneInfo *Write_Struct_feedback =(_MessageWriteOneInfo *)lParam;
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    vector <int>::iterator Iter;
    if(msg_result)
    {
        int indexid = -1;
        for (int i=0; i<(int)Change_Color_ID.size(); i++)
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
        for (int i=0; i<(int)Change_Color_ID.size(); i++)
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
    //Reflesh_ParameterDlg();
    Initial_Window();
    GetDlgItem(temp_id)->Invalidate();

    return 0;
}

LRESULT  CCO2NetView::ReadDataCallBack(WPARAM wParam, LPARAM lParam)
{
    _MessageReadOneInfo *Read_Struct_feedback =(_MessageReadOneInfo *)lParam;
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    if(msg_result)
    {
        product_register_value[Read_Struct_feedback->address]=Read_Struct_feedback->new_value;
    }
    if(Read_Struct_feedback!=NULL)
        delete Read_Struct_feedback;
    return 1;
}


BOOL CCO2NetView::PreTranslateMessage(MSG* pMsg)
{
    
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
    return CFormView::PreTranslateMessage(pMsg);
}


LRESULT CCO2NetView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    

    return CFormView::WindowProc(message, wParam, lParam);
}

void CCO2NetView::Initial_OutputList()
{
		unsigned short TempDataArray[100];
		int AddressValue = -1;
		CString strTemp;
		m_output_list.ShowWindow(SW_HIDE);
		m_output_list.DeleteAllItems();
		while ( m_output_list.DeleteColumn (0)) ;

		m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
		m_output_list.InsertColumn(0, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		m_output_list.InsertColumn(1, _T("Full Label"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_output_list.InsertColumn(2, _T("Value"), 45, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_output_list.InsertColumn(3, _T("Range"), 45, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_output_list.InsertColumn(4, _T("Min Out Scale"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_output_list.InsertColumn(5, _T("Max Out Scale"), 85, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		//不现实 AM
		
		m_output_list.InsertColumn(6, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	

		g_hwnd_now = this->m_hWnd;
		m_output_list.InsertItem(0,_T("1"));
		m_output_list.InsertItem(1,_T("2"));
		m_output_list.InsertItem(2,_T("3"));
		m_output_list.SetItemText(0,1,_T("Temperature"));
		m_output_list.SetItemText(1,1,_T("Humidity"));
		m_output_list.SetItemText(2,1,_T("CO2"));

        Read_Multi(g_tstat_id,TempDataArray,1250,100);
		AddressValue =1255 - 1250;
		strTemp.Format(_T("%0.1f"),((float)(short)(TempDataArray[AddressValue]))/10.0);
		m_output_list.SetItemText(0,4,strTemp);

		AddressValue =1256 - 1250;
		strTemp.Format(_T("%0.1f"),(float)(short)(TempDataArray[AddressValue])/10.0);
		m_output_list.SetItemText(0,5,strTemp);
	 


		AddressValue =1257 - 1250;
		strTemp.Format(_T("%0.1f"),((float)(short)(TempDataArray[AddressValue]))/10.0);
		m_output_list.SetItemText(1,4,strTemp);

		AddressValue =1258 - 1250;
		strTemp.Format(_T("%0.1f"),(float)(short)(TempDataArray[AddressValue])/10.0);
		m_output_list.SetItemText(1,5,strTemp);

		AddressValue =1259 - 1250;
		strTemp.Format(_T("%0.1f"),((float)(short)(TempDataArray[AddressValue]))/10.0);
		m_output_list.SetItemText(2,4,strTemp);

		AddressValue =1260 - 1250;
		strTemp.Format(_T("%d"),(unsigned short)(TempDataArray[AddressValue]));
		m_output_list.SetItemText(2,5,strTemp);
 

 
		AddressValue =1254 - 1250;
	 

		int output_range=TempDataArray[AddressValue];
		if (output_range==1)
		{
			strTemp=_T("0-10v");
		}
		else if (output_range==2)
		{
			strTemp=_T("0-5v");
		}
		else if (output_range==3)
		{
			strTemp=_T("4-20mA");
		}
		else
		{
			strTemp=_T("");
		}
		m_output_list.SetItemText(0,3,strTemp);
		m_output_list.SetItemText(1,3,strTemp);
		m_output_list.SetItemText(2,3,strTemp);



		AddressValue = 1250 - 1250;
		bitset<16> AM(TempDataArray[AddressValue]);
		if (!AM[0])
			m_output_list.SetItemText(0,6,Global_String_AUTO);
		else
			m_output_list.SetItemText(0,6,Global_String_MANUAL);

		if (!AM[1])
			m_output_list.SetItemText(1,6,Global_String_AUTO);
		else
			m_output_list.SetItemText(1,6,Global_String_MANUAL);

		if (!AM[2])
			m_output_list.SetItemText(2,6,Global_String_AUTO);
		else
			m_output_list.SetItemText(2,6,Global_String_MANUAL);

// 		AddressValue =1255 - 1250;
// 		strTemp.Format(_T("%0.1f"),((float)(short)(0-product_register_value[AddressValue]))/10.0);
// 		m_output_list.SetItemText(0,4,strTemp);
		 Read_Multi(g_tstat_id,TempDataArray,3060,100);
		float offsetvalue,Vtemp,Vhum;
		float Temp,HUM;

		if (output_range==1||output_range == 2)
		{
			Temp=((float)((short)TempDataArray[3066 - 3060]));
			Vtemp=(Temp)/100;
			strTemp.Format(_T("%.2f v"),Vtemp);
		}
		else if (output_range==3)
		{
			Temp=((float)((short)TempDataArray[3066 - 3060]));
			Vtemp=(Temp)/100;
			strTemp.Format(_T("%.2f ma"),Vtemp);
		}
		else
		{
			strTemp=_T("0");
		}
		m_output_list.SetItemText(0,2,strTemp);
 
		if (output_range==1||output_range == 2)
		{
			Temp=((float)((short)TempDataArray[3067 - 3060]));
			Vtemp=(Temp)/100;
			strTemp.Format(_T("%.2f v"),Vtemp);
		}
		else if (output_range==3)
		{
			Temp=((float)((short)TempDataArray[3067 - 3060]));
			Vtemp=(Temp)/100;
			strTemp.Format(_T("%.2f ma"),Vtemp);
		}
		else
		{
			strTemp=_T("0");
		}
		m_output_list.SetItemText(1,2,strTemp);

		if (output_range==1||output_range == 2)
		{
			Temp=((float)((short)TempDataArray[3068 - 3060]));
			Vtemp=(Temp)/100;
			strTemp.Format(_T("%.2f v"),Vtemp);
		}
		else if (output_range==3)
		{
			Temp=((float)((short)TempDataArray[3068 - 3060]));
			Vtemp=(Temp)/100;
			strTemp.Format(_T("%.2f ma"),Vtemp);
		}
		else
		{
			strTemp=_T("0");
		}
		m_output_list.SetItemText(2,2,strTemp);




		


	m_output_list.ShowWindow(SW_SHOW);
}
 
void CCO2NetView::Initial_VarList()
{
	//m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	//m_input_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(4, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(5, _T("Calibration"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	unsigned short TempDataArray[100];
	
	CString strTemp;
	int AddressValue = 0;
	m_user_list.DeleteAllItems();
	while ( m_user_list.DeleteColumn (0)) ;


	m_user_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_user_list.SetExtendedStyle(m_user_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));

	m_user_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_user_list.InsertColumn(1, _T("Full Label"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(4, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(5, _T("Calibration"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);


	g_hwnd_now = this->m_hWnd;

	CString index;
	for (int i=0; i< 4; i++)
	{
		index.Format(_T("%d"),i+1);
		m_user_list.InsertItem(i,index);
	}
	int MODBUS_DEW_PT = 370;   //unit.C
	int MODBUS_DEW_PT_F = MODBUS_DEW_PT +1;  //unit.F
	int MODBUS_PWS= MODBUS_DEW_PT +2;    //unit.hPa
	int MODBUS_MIX_RATIO= MODBUS_DEW_PT +3;  //unit.g/kg
	int MODBUS_ENTHALPY= MODBUS_DEW_PT +4;  //unit.kJ/kg
	int MODBUS_CF = 102;
	if (product_register_value[7] == PM_HUMTEMPSENSOR)
	{
		MODBUS_DEW_PT = 482;   //unit.C
		MODBUS_DEW_PT_F = MODBUS_DEW_PT +1;  //unit.F
		MODBUS_PWS= 488;    //unit.hPa
		MODBUS_MIX_RATIO= 489;  //unit.g/kg
		MODBUS_ENTHALPY= 490;  //unit.kJ/kg
	}
	else if ((product_register_value[7] == STM32_CO2_RS485) ||
              (product_register_value[7] == STM32_CO2_NET))
	{
		MODBUS_CF	=	201;
		MODBUS_DEW_PT = 3049;   //unit.C
		MODBUS_DEW_PT_F = MODBUS_DEW_PT +1;  //unit.F
		MODBUS_PWS= 3053;    //unit.hPa
		MODBUS_MIX_RATIO= 3054;  //unit.g/kg
		MODBUS_ENTHALPY= 3055;  //unit.kJ/kg
	}
	else
	{
		MODBUS_DEW_PT = 370;   //unit.C
		MODBUS_DEW_PT_F = MODBUS_DEW_PT +1;  //unit.F
		MODBUS_PWS= MODBUS_DEW_PT +2;    //unit.hPa
		MODBUS_MIX_RATIO= MODBUS_DEW_PT +3;  //unit.g/kg
		MODBUS_ENTHALPY= MODBUS_DEW_PT +4;  //unit.kJ/kg
	}
	Read_Multi(g_tstat_id,TempDataArray,MODBUS_DEW_PT,100);
	if (product_register_value[7] == PM_HUMTEMPSENSOR||
        product_register_value[7] == PM_AirQuality||
        product_register_value[7]==PM_HUM_R||
        product_register_value[7] == STM32_CO2_RS485 ||
        product_register_value[7] == STM32_CO2_NET
        )
	{
		m_user_list.SetItemText(0,1,_T("Dew Point"));

		m_user_list.SetItemText(0,2,NO_APPLICATION);



		if(product_register_value[MODBUS_CF]==0)
		{
			AddressValue =MODBUS_DEW_PT;
			strTemp.Format(_T("%0.1f"),((float)((short)TempDataArray[AddressValue-MODBUS_DEW_PT]))/10.0);
			m_user_list.SetItemText(0,3,strTemp);
			m_user_list.SetItemText(0,4,Global_String_C);
		}
		else
		{

			AddressValue =MODBUS_DEW_PT_F;
			strTemp.Format(_T("%0.1f"),((float)((short)TempDataArray[AddressValue-MODBUS_DEW_PT]))/10.0);
			m_user_list.SetItemText(0,3,strTemp);
			m_user_list.SetItemText(0,4,Glocal_String_F);
		}





		//strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);
		m_user_list.SetItemText(0,5,NO_APPLICATION );

		//
		m_user_list.SetItemText (1,1,L"Partial Pressure");
		m_user_list.SetItemText(1,2,NO_APPLICATION);
		strTemp.Format(_T("%0.1f"),((float)((short)TempDataArray[MODBUS_PWS-MODBUS_DEW_PT]))/10.0);
		m_user_list.SetItemText(1,3,strTemp);
		m_user_list.SetItemText(1,4,L"hPa");
		m_user_list.SetItemText(1,5,NO_APPLICATION );


		m_user_list.SetItemText (2,1,L"Mixing Ratio");
		m_user_list.SetItemText(2,2,NO_APPLICATION);
		strTemp.Format(_T("%0.1f"),((float)((short)TempDataArray[MODBUS_MIX_RATIO-MODBUS_DEW_PT]))/10.0);
		m_user_list.SetItemText(2,3,strTemp);
		m_user_list.SetItemText(2,4,L"g/Kg");
		m_user_list.SetItemText(2,5,NO_APPLICATION );

		m_user_list.SetItemText (3,1,L"Enthalpy");
		m_user_list.SetItemText(3,2,NO_APPLICATION);
		strTemp.Format(_T("%0.1f"),((float)((short)TempDataArray[MODBUS_ENTHALPY-MODBUS_DEW_PT]))/10.0);
		m_user_list.SetItemText(3,3,strTemp);
		m_user_list.SetItemText(3,4,L"KJ/kg");
		m_user_list.SetItemText(3,5,NO_APPLICATION );

	}

}

BEGIN_EVENTSINK_MAP(CCO2NetView, CFormView)
	ON_EVENT(CCO2NetView, IDC_MSFLEXGRID_INPUT, DISPID_DBLCLICK, CCO2NetView::DblClickMsflexgridInput, VTS_NONE)
END_EVENTSINK_MAP()


void CCO2NetView::DblClickMsflexgridInput()
{
	 m_isinput=TRUE;
	//m_comboxRange.ShowWindow(FALSE);
	UpdateData(FALSE);

	long lRow,lCol;
	lRow = m_grid_input.get_RowSel();//获取点击的行号	
	lCol = m_grid_input.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_grid_input.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_grid_input.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_grid_input.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_grid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_grid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_grid_input.get_TextMatrix(lRow,lCol);
 	m_oldname=strValue;
  	m_curcol=lCol;
  	m_currow=lRow;
	if (lCol == 5)//Calibartion
	{
		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //全选//
	}
}


void CCO2NetView::OnEnKillfocusEditName()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
	float Value=_wtof(strTemp);
// 	if (strTemp.Compare(m_oldname)==0)
// 	{
// 		return;
// 	}
	int Internal_Address = 0;
	int External_Address = 0;
	if (m_isinput)
	{
		 
		if (m_curcol==5)
		{
 
			if (m_currow == 1)
			{
				if (product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F] == 0)
				{
					Internal_Address = CO2_NET_MODBUS_INTERNAL_TEMPERATURE_CELSIUS;
					External_Address = CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_CELSIUS;


				}
				else if ((product_register_value[CO2_NET_MODBUS_TEMPERATURE_DEGREE_C_OR_F] == 1))
				{
					Internal_Address = CO2_NET_MODBUS_INTERNAL_TEMPERATURE_FAHRENHEIT;
					External_Address = CO2_NET_MODBUS_EXTERNAL_TEMPERATURE_FAHRENHEIT;


				}

				int RegAddress = 0;

				if (product_register_value[CO2_NET_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//内部
				{
					RegAddress = Internal_Address;

				}
				else
				{
					RegAddress = External_Address;
				}
				write_one(g_tstat_id, RegAddress, Value * 10);
				product_register_value[RegAddress] = Value * 10;
				
			}

			//int	CO2_NET_MODBUS_OUTPUT_MANUAL_VALUE_TEM;
			//int	CO2_NET_MODBUS_OUTPUT_MANUAL_VALUE_HUM;
			//int	CO2_NET_MODBUS_OUTPUT_MANUAL_VALUE_CO2;

// 		   if (m_currow == 2)
// 		   {
// 			   strHUM.Format(_T("%0.1f"), (float)(product_register_value[CO2_NET_MODBUS_HUMIDITY]) / 10.0);
// 			   m_grid_input.put_TextMatrix(2, 4, strHUM);
// 			   strCO2.Format(_T("%d"), product_register_value[CO2_NET_MODBUS_CO2_INTERNAL]);
// 			   m_grid_input.put_TextMatrix(3, 4, strCO2);
// 		   }
// 		   if (m_currow == 3)
// 		   {
// 			   strHUM.Format(_T("%0.1f"), (float)(product_register_value[CO2_NET_MODBUS_HUMIDITY]) / 10.0);
// 			   m_grid_input.put_TextMatrix(2, 4, strHUM);
// 			   strCO2.Format(_T("%d"), product_register_value[CO2_NET_MODBUS_CO2_INTERNAL]);
// 			   m_grid_input.put_TextMatrix(3, 4, strCO2);
// 		   }
			
		   Show_InputList();
		}

	}
	 
}


void CCO2NetView::OnBnClickedRadioCo2CalEnable()
{
    // TODO: 在此添加控件通知处理程序代码
    if (product_register_value[3131] != 0)
    {
        Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 3131, 0,
            product_register_value[3131], this->m_hWnd, IDC_RADIO_CO2_CAL_ENABLE, _T("Auto Calibration Enable"));
    }
}


void CCO2NetView::OnBnClickedRadioCo2CalDisable()
{
    // TODO: 在此添加控件通知处理程序代码
    if (product_register_value[3131] != 1)
    {
        Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 3131, 1,
            product_register_value[3131], this->m_hWnd, IDC_RADIO_CO2_CAL_ENABLE, _T("Auto Calibration Disable"));
    }
}


void CCO2NetView::OnBnClickedRadioHumidityHeatEnable()
{
    // TODO: 在此添加控件通知处理程序代码
}
