// NewHumChamberView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NewHumChamberView.h"

#include "globle_function.h"
#include "MainFrm.h"
// CNewHumChamberView
#include "Dialog_Progess.h"

#include "WriteSingleRegDlg.h"
#include "DllFunction.h"
#include "excel9.h"
#include "../SQLiteDriver/CppSQLite3.h"
int use_minipanel_controller = 0; //是否用Minipanel 来控制  校准的箱子;  //1:enable         2: disable
 
CString HUM_Minipanel_IP;
int HUM_Minipanel_port;
int HUM_Minipanel_modbus_id;

unsigned int HUM_WRITE_REG1;
unsigned int HUM_WRITE_REG2;

CString save_data_ini_file_path;
unsigned char save_date_into_ini = 0;
UINT _Read_Testo(LPVOID pParam)
{
	CNewHumChamberView* pdlg=(CNewHumChamberView*)(pParam);
	GetPrivateProfileString(_T("Hum_Setting"),_T("MODBUS_IP"),_T("192.168.0.113"),HUM_Minipanel_IP.GetBuffer(MAX_PATH),MAX_PATH,g_cstring_ini_path);
	HUM_Minipanel_IP.ReleaseBuffer();
	HUM_Minipanel_port = GetPrivateProfileInt(_T("Hum_Setting"),_T("MODBUS_Port"),0,g_cstring_ini_path);
	HUM_Minipanel_modbus_id = GetPrivateProfileInt(_T("Hum_Setting"),_T("MODBUS_ID"),0,g_cstring_ini_path);
	
	HUM_WRITE_REG1 = GetPrivateProfileInt(_T("Hum_Setting"),_T("MODBUS_ADDRESS1"),0,g_cstring_ini_path);
	HUM_WRITE_REG2 = GetPrivateProfileInt(_T("Hum_Setting"),_T("MODBUS_ADDRESS2"),0,g_cstring_ini_path);
	use_minipanel_controller = GetPrivateProfileInt(_T("Hum_Setting"),_T("ENABLE_MINIPANEL_FUC"),0,g_cstring_ini_path);

	save_date_into_ini =  GetPrivateProfileInt(_T("Hum_Setting"),_T("ENABLE_SAVE_DATE_INTO_INI"),0,g_cstring_ini_path);
	GetPrivateProfileString(_T("Hum_Setting"),_T("INI_File_Path"),_T(""),save_data_ini_file_path.GetBuffer(MAX_PATH),MAX_PATH,g_cstring_ini_path);
	save_data_ini_file_path.ReleaseBuffer();

	if(save_data_ini_file_path.IsEmpty())
	{
		WritePrivateProfileStringW(_T("Hum_Setting"),_T("INI_File_Path"),_T("D:\\Test.ini"),g_cstring_ini_path);
		save_data_ini_file_path = _T("C:\\Test124.ini");
	}
	save_data_ini_file_path = _T("C:\\Test124.ini");
	if((save_date_into_ini != 1) && (save_date_into_ini != 2))
	{
		WritePrivateProfileStringW(_T("Hum_Setting"),_T("ENABLE_SAVE_DATE_INTO_INI"),_T("2"),g_cstring_ini_path);
		save_date_into_ini = 2;//disable
	}
	if((HUM_Minipanel_port == 0) || (HUM_Minipanel_modbus_id == 0) || (HUM_WRITE_REG1 == 0) || (HUM_WRITE_REG2 == 0) || (use_minipanel_controller == 0))
	{
		WritePrivateProfileStringW(_T("Hum_Setting"),_T("MODBUS_Port"),_T("10000"),g_cstring_ini_path);
		WritePrivateProfileStringW(_T("Hum_Setting"),_T("MODBUS_ID"),_T("254"),g_cstring_ini_path);
		WritePrivateProfileStringW(_T("Hum_Setting"),_T("MODBUS_IP"),_T("192.168.0.113"),g_cstring_ini_path);

		WritePrivateProfileStringW(_T("Hum_Setting"),_T("MODBUS_ADDRESS1"),_T("8820"),g_cstring_ini_path);
		WritePrivateProfileStringW(_T("Hum_Setting"),_T("MODBUS_ADDRESS2"),_T("8821"),g_cstring_ini_path);

		WritePrivateProfileStringW(_T("Hum_Setting"),_T("ENABLE_MINIPANEL_FUC"),_T("2"),g_cstring_ini_path);

		HUM_WRITE_REG1 = 8092;
		HUM_WRITE_REG2 = 8094;
		HUM_Minipanel_port = 10000;
		HUM_Minipanel_modbus_id = 254;
		use_minipanel_controller = 2;
	}
 
	HUM_Minipanel_modbus_id = 255;
	 

	while(TRUE)
	{
		if (pdlg->m_isstart)
		{
			pdlg->read_testo();
			::SendMessage(pdlg->m_hWnd,WM_FRESH_TESTO,0,0);
		    Sleep(300);
		}
		else{
			Sleep(1000);
		}
	}
	//else
   // {
        if (pdlg->g_Draw_dlg!=NULL)
        {
            delete pdlg->g_Draw_dlg;
            pdlg->g_Draw_dlg=NULL;
        }
  //  }	


}
UINT NEW_HUM_CHAMBER_UpdateThread(LPVOID pParam)
{		CNewHumChamberView* pDlg = (CNewHumChamberView*)(pParam);
	while(TRUE)
	{ 
// 	TRACE(_T("1...pDlg->m_hWnd == %u\r\n"),pDlg->m_hWnd);
// 	::PostMessage(pDlg->m_hWnd,WM_FRESH_CO2,0,0);
// 	Sleep(1);
// 	TRACE(_T("2...pDlg->m_hWnd == %u\r\n"),pDlg->m_hWnd);
// 	::PostMessage(pDlg->m_hWnd,WM_FRESH_HUM_CHAMBER,0,0);
// 	Sleep(1);
// 	continue;
		if (pDlg->IsWindowVisible())
		{

		    g_bPauseMultiRead = FALSE;

			if (product_register_value[pDlg->MODBUS_SENSOR_TYPE]!=2)
			{
				
				Read_Multi(pDlg->m_tstatID,&product_register_value[pDlg->MODBUS_CAL_STEPS],pDlg->MODBUS_CAL_STEPS,20);
				::SendMessage(pDlg->m_hWnd,WM_FRESH_CO2,0,0);
			} 
			else//HUM
			{
				
				Read_Multi(pDlg->m_tstatID,&product_register_value[pDlg->MODBUS_TIME_LEFT],pDlg->MODBUS_TIME_LEFT,20);
				::SendMessage(pDlg->m_hWnd,WM_FRESH_HUM_CHAMBER,0,0);
			}

 			 
 			 Sleep(800);

		}
		else
		{
			g_bPauseMultiRead = TRUE;
            if (pDlg->g_Draw_dlg!=NULL)
            {
                delete pDlg->g_Draw_dlg;
                pDlg->g_Draw_dlg=NULL;
            }
		}
		
	/*	pDlg->Fresh_Hum_Temp();*/
	}
	return 1;
}

IMPLEMENT_DYNCREATE(CNewHumChamberView, CFormView)

CNewHumChamberView::CNewHumChamberView()
	: CFormView(CNewHumChamberView::IDD)
    , m_frequence_seconds(1)
{
m_isstart=false;
m_pFreshMultiRegisters=NULL;
m_times=0;
g_Time_Offset = 0;
is_Show_Write_singleDLG=FALSE;
 m_Start=FALSE;
 hFirstThread=NULL;
 m_Write_times=0;
 m_tstatID=g_tstat_id;
 g_Draw_dlg=NULL;
 m_current_Co2_sensorpoint=0;
 m_IsReadIniFile = FALSE;
}

CNewHumChamberView::~CNewHumChamberView()
{
}

void CNewHumChamberView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK_1, m_check1);
    DDX_Control(pDX, IDC_CHECK_2, m_check2);
    DDX_Control(pDX, IDC_CHECK_3, m_check3);
    DDX_Control(pDX, IDC_CHECK_4, m_check4);
    DDX_Control(pDX, IDC_CHECK_5, m_check5);
    DDX_Control(pDX, IDC_CHECK_6, m_check6);
    DDX_Control(pDX, IDC_CHECK_7, m_check7);
    DDX_Control(pDX, IDC_CHECK_8, m_check8);
    DDX_Control(pDX, IDC_CHECK_9, m_check9);
    DDX_Control(pDX, IDC_CHECK_10, m_check10);
    //  DDX_Control(pDX, IDC_CO20, m_edit_co20);
    DDX_Control(pDX, IDC_CO21, m_edit_co21);
    //  DDX_Control(pDX, IDC_CO22, m_m_edit_co22);
    DDX_Control(pDX, IDC_CO23, m_edit_co23);
    DDX_Control(pDX, IDC_CO24, m_edit_co24);
    DDX_Control(pDX, IDC_CO25, m_edit_co25);
    DDX_Control(pDX, IDC_CO26, m_edit_co26);
    DDX_Control(pDX, IDC_CO27, m_edit_co27);
    DDX_Control(pDX, IDC_CO28, m_edit_co28);
    DDX_Control(pDX, IDC_CO29, m_edit_co29);
    DDX_Control(pDX, IDC_CO20, m_edit_co210);
    DDX_Control(pDX, IDC_CO22, m_edit_co22);
    DDX_Control(pDX, IDC_COMBO_TYPE, m_combox_type);
    DDX_Control(pDX, IDC_COMBO_USB_TESTO, m_combox_testo);
    DDX_Control(pDX, IDC_EDIT_CO2, m_edit_co2);
    DDX_Control(pDX, IDC_EDIT_PRESURE, m_edit_presure);
    DDX_Control(pDX, IDC_EDIT_TEMP, m_edit_temp);
    DDX_Control(pDX, IDC_EDIT4_HUM, m_edit_hum);
    DDX_Control(pDX, IDC_FREQ1, m_edit_freq1);
    DDX_Control(pDX, IDC_FREQ10, m_edit_freq10);
    DDX_Control(pDX, IDC_FREQ2, m_edit_freq2);
    DDX_Control(pDX, IDC_FREQ3, m_edit_freq3);
    DDX_Control(pDX, IDC_FREQ4, m_edit_freq4);
    DDX_Control(pDX, IDC_FREQ5, m_edit_freq5);
    DDX_Control(pDX, IDC_FREQ6, m_edit_freq6);
    DDX_Control(pDX, IDC_FREQ7, m_edit_freq7);
    DDX_Control(pDX, IDC_FREQ8, m_edit_freq8);
    DDX_Control(pDX, IDC_FREQ9, m_edit_freq9);
    DDX_Control(pDX, IDC_HUM1, m_edit_hum1);
    DDX_Control(pDX, IDC_HUM10, m_edit_hum10);
    DDX_Control(pDX, IDC_HUM2, m_edit_hum2);
    DDX_Control(pDX, IDC_HUM3, m_edit_hum3);
    DDX_Control(pDX, IDC_HUM4, m_edit_hum4);
    DDX_Control(pDX, IDC_HUM5, m_edit_hum5);
    DDX_Control(pDX, IDC_HUM6, m_edit_hum6);
    DDX_Control(pDX, IDC_HUM7, m_edit_hum7);
    DDX_Control(pDX, IDC_HUM8, m_edit_hum8);
    DDX_Control(pDX, IDC_HUM9, m_edit_hum9);
    DDX_Control(pDX, IDC_MASTER_ID, m_edit_master_id);
    DDX_Control(pDX, IDC_NUM_ERROR_SENSOR, m_edit_error_sensors);
    DDX_Control(pDX, IDC_NUM_SENSOR, m_edit_num_sensor);
    DDX_Control(pDX, IDC_RH1, m_edit_rh1);
    DDX_Control(pDX, IDC_RH10,m_edit_rh10);
    DDX_Control(pDX, IDC_RH2, m_edit_rh2);
    DDX_Control(pDX, IDC_RH3, m_edit_rh3);
    DDX_Control(pDX, IDC_RH4, m_edit_rh4);
    DDX_Control(pDX, IDC_RH5, m_edit_rh5);
    DDX_Control(pDX, IDC_RH6, m_edit_rh6);
    DDX_Control(pDX, IDC_RH7, m_edit_rh7);
    DDX_Control(pDX, IDC_RH8, m_edit_rh8);
    DDX_Control(pDX, IDC_RH9, m_edit_rh9);
    DDX_Control(pDX, IDC_TEMP_LEFT1, m_edit_time_left1);
    DDX_Control(pDX, IDC_TEMP_LEFT10, m_edit_time_left10);
    DDX_Control(pDX, IDC_TEMP_LEFT2, m_edit_time_left2);
    DDX_Control(pDX, IDC_TEMP_LEFT3, m_edit_time_left3);
    DDX_Control(pDX, IDC_TEMP_LEFT4, m_edit_time_left4);
    DDX_Control(pDX, IDC_TEMP_LEFT5, m_edit_time_left5);
    DDX_Control(pDX, IDC_TEMP_LEFT6, m_edit_time_left6);
    DDX_Control(pDX, IDC_TEMP_LEFT7, m_edit_time_left7);
    DDX_Control(pDX, IDC_TEMP_LEFT8, m_edit_time_left8);
    DDX_Control(pDX, IDC_TEMP_LEFT9, m_edit_time_left9);
    DDX_Control(pDX, IDC_TEMP1, m_edit_temp1);
    DDX_Control(pDX, IDC_TEMP10, m_edit_temp10);
    DDX_Control(pDX, IDC_TEMP2, m_edit_temp2);
    DDX_Control(pDX, IDC_TEMP3, m_edit_temp3);
    DDX_Control(pDX, IDC_TEMP4, m_edit_temp4);
    DDX_Control(pDX, IDC_TEMP5, m_edit_temp5);
    DDX_Control(pDX, IDC_TEMP6, m_edit_temp6);
    DDX_Control(pDX, IDC_TEMP7, m_edit_temp7);
    DDX_Control(pDX, IDC_TEMP8, m_edit_temp8);
    DDX_Control(pDX, IDC_TEMP9, m_edit_temp9);
    DDX_Control(pDX, IDC_TIME1, m_edit_time1);
    DDX_Control(pDX, IDC_TIME10, m_edit_time10);
    DDX_Control(pDX, IDC_TIME2, m_edit_time2);
    DDX_Control(pDX, IDC_TIME3, m_edit_time3);
    DDX_Control(pDX, IDC_TIME4, m_edit_time4);
    DDX_Control(pDX, IDC_TIME5, m_edit_time5);
    DDX_Control(pDX, IDC_TIME6, m_edit_time6);
    DDX_Control(pDX, IDC_TIME7, m_edit_time7);
    DDX_Control(pDX, IDC_TIME8, m_edit_time8);
    DDX_Control(pDX, IDC_TIME9, m_edit_time9);
    DDX_Control(pDX, IDC_MSFLEXGRID_INPUT3, m_msflexgrid);
    DDX_Control(pDX, IDC_STATIC_CUR_SENSOR, m_cur_sensor);
    DDX_Control(pDX, IDC_STATIC_TIMES, m_libel_times);
    DDX_Control(pDX, IDC_SKIP, m_CheckSkip);
    DDX_Control(pDX, IDC_BATCH_FLASH, m_batch_btn);
    DDX_Control(pDX, IDC_START, m_StartBtn);
    DDX_Control(pDX, IDC_STOP, m_StopBtn);
    DDX_Control(pDX, IDC_CONTINUE, m_ContinueBtn);
    DDX_Control(pDX, IDC_STATIC_TESTSTATUS, m_Static_TestStatus);
    DDX_Control(pDX, IDC_COMBO_SETTING_CO2, m_combox_co2_setting);
    DDX_Text(pDX, IDC_EDIT_FREQUENCE, m_frequence_seconds);
}

BEGIN_MESSAGE_MAP(CNewHumChamberView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNewHumChamberView::OnBnClickedButtonStart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_1, &CNewHumChamberView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK_2, &CNewHumChamberView::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_3, &CNewHumChamberView::OnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK_4, &CNewHumChamberView::OnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK_5, &CNewHumChamberView::OnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK_6, &CNewHumChamberView::OnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK_7, &CNewHumChamberView::OnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK_8, &CNewHumChamberView::OnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK_9, &CNewHumChamberView::OnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK_10, &CNewHumChamberView::OnClickedCheck10)
//	ON_EN_KILLFOCUS(IDC_TEMP1, &CNewHumChamberView::OnKillfocusTemp1)
	ON_EN_SETFOCUS(IDC_TEMP1, &CNewHumChamberView::OnSetfocusTemp1)
//	ON_EN_KILLFOCUS(IDC_TEMP10, &CNewHumChamberView::OnKillfocusTemp10)
	ON_EN_SETFOCUS(IDC_TEMP10, &CNewHumChamberView::OnSetfocusTemp10)
//	ON_EN_KILLFOCUS(IDC_TEMP2, &CNewHumChamberView::OnKillfocusTemp2)
	ON_EN_SETFOCUS(IDC_TEMP2, &CNewHumChamberView::OnSetfocusTemp2)
//	ON_EN_KILLFOCUS(IDC_TEMP3, &CNewHumChamberView::OnKillfocusTemp3)
	ON_EN_SETFOCUS(IDC_TEMP3, &CNewHumChamberView::OnSetfocusTemp3)
//	ON_EN_KILLFOCUS(IDC_TEMP4, &CNewHumChamberView::OnKillfocusTemp4)
	ON_EN_SETFOCUS(IDC_TEMP4, &CNewHumChamberView::OnSetfocusTemp4)
//	ON_EN_KILLFOCUS(IDC_TEMP5, &CNewHumChamberView::OnKillfocusTemp5)
	ON_EN_SETFOCUS(IDC_TEMP5, &CNewHumChamberView::OnSetfocusTemp5)
//	ON_EN_KILLFOCUS(IDC_TEMP6, &CNewHumChamberView::OnKillfocusTemp6)
	ON_EN_SETFOCUS(IDC_TEMP6, &CNewHumChamberView::OnSetfocusTemp6)
//	ON_EN_KILLFOCUS(IDC_TEMP7, &CNewHumChamberView::OnKillfocusTemp7)
	ON_EN_SETFOCUS(IDC_TEMP7, &CNewHumChamberView::OnSetfocusTemp7)
//	ON_EN_KILLFOCUS(IDC_TEMP8, &CNewHumChamberView::OnKillfocusTemp8)
	ON_EN_SETFOCUS(IDC_TEMP8, &CNewHumChamberView::OnSetfocusTemp8)
//	ON_EN_KILLFOCUS(IDC_TEMP9, &CNewHumChamberView::OnKillfocusTemp9)
	ON_EN_SETFOCUS(IDC_TEMP9, &CNewHumChamberView::OnSetfocusTemp9)
	ON_EN_SETFOCUS(IDC_HUM1, &CNewHumChamberView::OnSetfocusHum1)
	ON_EN_SETFOCUS(IDC_HUM10, &CNewHumChamberView::OnSetfocusHum10)
//	ON_EN_CHANGE(IDC_HUM2, &CNewHumChamberView::OnChangeHum2)
//	ON_EN_CHANGE(IDC_HUM3, &CNewHumChamberView::OnChangeHum3)
//	ON_EN_CHANGE(IDC_HUM4, &CNewHumChamberView::OnChangeHum4)
	ON_EN_SETFOCUS(IDC_HUM2, &CNewHumChamberView::OnSetfocusHum2)
	ON_EN_SETFOCUS(IDC_HUM3, &CNewHumChamberView::OnSetfocusHum3)
	ON_EN_SETFOCUS(IDC_HUM4, &CNewHumChamberView::OnSetfocusHum4)
	ON_EN_SETFOCUS(IDC_HUM5, &CNewHumChamberView::OnSetfocusHum5)
	ON_EN_SETFOCUS(IDC_HUM6, &CNewHumChamberView::OnSetfocusHum6)
	ON_EN_SETFOCUS(IDC_HUM7, &CNewHumChamberView::OnSetfocusHum7)
	ON_EN_SETFOCUS(IDC_HUM8, &CNewHumChamberView::OnSetfocusHum8)
	ON_EN_SETFOCUS(IDC_HUM9, &CNewHumChamberView::OnSetfocusHum9)
	ON_EN_SETFOCUS(IDC_TIME1, &CNewHumChamberView::OnSetfocusTime1)
	ON_EN_SETFOCUS(IDC_TIME10, &CNewHumChamberView::OnSetfocusTime10)
//	ON_EN_CHANGE(IDC_TIME2, &CNewHumChamberView::OnChangeTime2)
ON_EN_SETFOCUS(IDC_TIME2, &CNewHumChamberView::OnSetfocusTime2)
ON_EN_SETFOCUS(IDC_TIME3, &CNewHumChamberView::OnSetfocusTime3)
ON_EN_SETFOCUS(IDC_TIME4, &CNewHumChamberView::OnSetfocusTime4)
ON_EN_SETFOCUS(IDC_TIME5, &CNewHumChamberView::OnSetfocusTime5)
ON_EN_SETFOCUS(IDC_TIME6, &CNewHumChamberView::OnSetfocusTime6)
ON_EN_SETFOCUS(IDC_TIME7, &CNewHumChamberView::OnSetfocusTime7)
ON_EN_SETFOCUS(IDC_TIME8, &CNewHumChamberView::OnSetfocusTime8)
ON_EN_SETFOCUS(IDC_TIME9, &CNewHumChamberView::OnSetfocusTime9)
ON_BN_CLICKED(IDC_REFRESH_HUMBER_CHAMBER, &CNewHumChamberView::OnBnClickedRefresh)
ON_BN_CLICKED(IDC_BATCH_FLASH, &CNewHumChamberView::OnBnClickedBatchFlash)
ON_BN_CLICKED(IDC_CALIBRATION_POINT_REPORT, &CNewHumChamberView::OnBnClickedCalibrationPointReport)
ON_BN_CLICKED(IDC_START, &CNewHumChamberView::OnBnClickedStart)
ON_BN_CLICKED(IDC_STOP, &CNewHumChamberView::OnBnClickedStop)
ON_BN_CLICKED(IDC_CONTINUE, &CNewHumChamberView::OnBnClickedContinue)
ON_WM_CTLCOLOR()
ON_EN_SETFOCUS(IDC_NUM_SENSOR, &CNewHumChamberView::OnEnSetfocusNumSensor)
ON_CBN_DROPDOWN(IDC_COMBO_USB_TESTO, &CNewHumChamberView::OnCbnDropdownComboUsbTesto)
ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CNewHumChamberView::OnCbnSelchangeComboType)
ON_EN_SETFOCUS(IDC_CO21, &CNewHumChamberView::OnEnSetfocusCo21)
ON_EN_SETFOCUS(IDC_CO20, &CNewHumChamberView::OnSetfocusCo20)
ON_EN_SETFOCUS(IDC_CO22, &CNewHumChamberView::OnSetfocusCo22)
ON_EN_SETFOCUS(IDC_CO23, &CNewHumChamberView::OnSetfocusCo23)
ON_EN_SETFOCUS(IDC_CO24, &CNewHumChamberView::OnSetfocusCo24)
ON_EN_SETFOCUS(IDC_CO25, &CNewHumChamberView::OnSetfocusCo25)
ON_EN_SETFOCUS(IDC_CO26, &CNewHumChamberView::OnSetfocusCo26)
ON_EN_SETFOCUS(IDC_CO27, &CNewHumChamberView::OnSetfocusCo27)
ON_EN_SETFOCUS(IDC_CO28, &CNewHumChamberView::OnSetfocusCo28)
ON_EN_SETFOCUS(IDC_CO29, &CNewHumChamberView::OnSetfocusCo29)
ON_EN_SETFOCUS(IDC_EDIT_TSTATADDRESS, &CNewHumChamberView::OnEnSetfocusEditTstataddress)
ON_EN_SETFOCUS(IDC_EDIT_REGISTER_ADDRESS, &CNewHumChamberView::OnEnSetfocusEditRegisterAddress)
ON_EN_SETFOCUS(IDC_EDIT_CO2SCALE, &CNewHumChamberView::OnEnSetfocusEditCo2scale)
ON_EN_SETFOCUS(IDC_EDIT_STABLETIME_CHAMBER, &CNewHumChamberView::OnEnSetfocusEditStabletimeChamber)
ON_EN_SETFOCUS(IDC_EDIT_STABLETIME_SENSOR, &CNewHumChamberView::OnEnSetfocusEditStabletimeSensor)
ON_EN_SETFOCUS(IDC_EDIT_RTD1, &CNewHumChamberView::OnEnSetfocusEditRtd1)
ON_EN_SETFOCUS(IDC_EDIT_RTD2, &CNewHumChamberView::OnEnSetfocusEditRtd2)
 
ON_EN_SETFOCUS(IDC_EDIT_HUMPOINT, &CNewHumChamberView::OnEnSetfocusEditHumpoint)
ON_EN_SETFOCUS(IDC_EDIT_CO2POINT, &CNewHumChamberView::OnEnSetfocusEditCo2point)
ON_CBN_SELCHANGE(IDC_COMBO_SETTING_CO2, &CNewHumChamberView::OnCbnSelchangeComboSettingCo2)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_CALIBRATION_POINT_REPORT2, &CNewHumChamberView::OnBnClickedCalibrationPointReport2)
ON_BN_CLICKED(IDC_BUTTON_TESTO_GRAPHIC, &CNewHumChamberView::OnBnClickedButtonTestoGraphic)
ON_BN_CLICKED(IDC_CHECK_READ, &CNewHumChamberView::OnBnClickedCheckRead)
END_MESSAGE_MAP()


// CNewHumChamberView diagnostics

#ifdef _DEBUG
void CNewHumChamberView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNewHumChamberView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
 
void CNewHumChamberView::Fresh(){
	g_bPauseMultiRead = TRUE;
	 m_tstatID=g_tstat_id;
        
 Read_Multi(m_tstatID,m_SN,876,55);
 Read_Multi(m_tstatID,&m_SN[55],876+55,55);
Initial_RegisterList();
m_msflexgrid.ShowWindow(TRUE);
CStringArray EnumUSB;
CString temp;


Enum_USB_Device(EnumUSB);
m_combox_testo.ResetContent();
for (int i=0;i<EnumUSB.GetSize();i++)
{
	temp=EnumUSB.GetAt(i);
	m_combox_testo.AddString(temp);
}
m_combox_testo.SetCurSel(0);

m_current_Co2_sensorpoint=product_register_value[MODBUS_SENSOR_POINT_CALIBRATING];
Main_infor();
Calibration_Points();
Sensor_Reading_Table();
Sensor_Table();
Fresh_CalibartionPoints();
FreshUI();
Fresh_CO2();
m_CheckSkip.SetCheck(1);
if (product_register_value[MODBUS_SENSOR_TYPE]==2)
{
	if (product_register_value[MODBUS_CALIBRATE_STATE]==4||product_register_value[MODBUS_CALIBRATE_STATE]==5)
	{
		m_Start=TRUE;
		m_StartBtn.ShowWindow(FALSE);
		m_StopBtn.ShowWindow(TRUE);
		m_ContinueBtn.ShowWindow(FALSE);
	}
	if (product_register_value[MODBUS_CALIBRATE_STATE]==0)//Stop 状态
	{
		m_Start=FALSE;
		m_StartBtn.ShowWindow(TRUE);
		m_StopBtn.ShowWindow(FALSE);
		m_ContinueBtn.ShowWindow(TRUE);
	}
	else if (product_register_value[MODBUS_CALIBRATE_STATE]==255)
	{
		m_Start=FALSE;
		m_StartBtn.ShowWindow(TRUE);
		m_StopBtn.ShowWindow(FALSE);
		m_ContinueBtn.ShowWindow(FALSE);
	}
}

/*SetTimer(1,1,NULL);*/
 if (hFirstThread==NULL)
 {
 	hFirstThread = AfxBeginThread(NEW_HUM_CHAMBER_UpdateThread,this);// CreateThread(NULL,NULL,,this,NULL,0);
 }


}
void CNewHumChamberView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_combox_type.ResetContent();
	m_combox_type.AddString(_T("CO2"));
	m_combox_type.AddString(_T("Multiple Sensor"));
	m_combox_type.AddString(_T("HUM"));

	m_msflexgrid.put_Cols(5);
	m_msflexgrid.put_Rows(3);
	m_msflexgrid.put_ColWidth(0,700);
	m_msflexgrid.put_ColWidth(1,700);
	m_msflexgrid.put_ColWidth(2,700);
	m_msflexgrid.put_ColWidth(3,800);
	m_msflexgrid.put_ColWidth(4,800);

	m_msflexgrid.put_TextMatrix(0,1,_T("SN"));
	m_msflexgrid.put_TextMatrix(0,0,_T("Sensor"));
	m_msflexgrid.put_TextMatrix(0,2,_T("State"));
	m_msflexgrid.put_TextMatrix(0,3,_T("TEMP(°C)"));
	m_msflexgrid.put_TextMatrix(0,4,_T("Hum(%)"));
	m_combox_co2_setting.ResetContent();
	m_combox_co2_setting.AddString(_T("Stop"));
	m_combox_co2_setting.AddString(_T("RTD"));
	m_combox_co2_setting.AddString(_T("CO2"));
	m_combox_co2_setting.AddString(_T("HUM"));
	
}
void CNewHumChamberView::Initial_RegisterList(){

#if 1
	MODBUS_SERIALNUMBER_LOWORD	=	0	;
	MODBUS_SERIALNUMBER_HIWORD	=	2	;
	MODBUS_VERSION_NUMBER_LO	=	4	;
	MODBUS_VERSION_NUMBER_HI	=	5	;
	MODBUS_ADDRESS	=	6	;
	MODBUS_PRODUCT_MODEL	=	7	;
	MODBUS_HARDWARE_REV	=	8	;
	MODBUS_PIC_VERSION	=	9	;
	MODBUS_ADDRESS_PLUG_N_PLAY	=	10	;
	MODBUS_BASE_ADDRESS	=	15	;
	MODBUS_UPDATE_STATUS	=	16	;
	MODBUS_SERINALNUMBER_WRITE_FLAG	=	17	;
	MODBUS_TESTO_CO2	=	100	;
	MODBUS_TESTO_HUM	=	101	;
	MODBUS_TESTO_T	=	102	;
	MODBUS_TESTO_P	=	103	;
	MODBUS_CO2_CAL_POINT	=	104	;
	MODBUS_CO2_SETTING0	=	105	;
	MODBUS_CO2_SETTING1	=	106	;
	MODBUS_CO2_SETTING2	=	107	;
	MODBUS_CO2_SETTING3	=	108	;
	MODBUS_CO2_SETTING4	=	109	;
	MODBUS_CO2_SETTING5	=	110	;
	MODBUS_CO2_SETTING6	=	111	;
	MODBUS_CO2_SETTING7	=	112	;
	MODBUS_CO2_SETTING8	=	113	;
	MODBUS_CO2_SETTING9	=	114	;
	MODBUS_CO2_RTD_SETTING0	=	115	;
	MODBUS_CO2_RTD_SETTING1	=	116	;
	MODBUS_CO2_SAMETIME_NUM	=	117	;
	MODBUS_SENSOR_TYPE	=	118	;
	MODBUS_HUM_CAL_POINT	=	120	;
	MODBUS_HUM_SETTING0	=	121	;
	MODBUS_HUM_SETTING1	=	122	;
	MODBUS_HUM_SETTING2	=	123	;
	MODBUS_HUM_SETTING3	=	124	;
	MODBUS_HUM_SETTING4	=	125	;
	MODBUS_HUM_SETTING5	=	126	;
	MODBUS_HUM_SETTING6	=	127	;
	MODBUS_HUM_SETTING7	=	128	;
	MODBUS_HUM_SETTING8	=	129	;
	MODBUS_HUM_SETTING9	=	130	;
	MODBUS_ALL_SENSOR_NUM 	=	131	;
	MODBUS_CAL_STEPS	=	132	;
	MODBUS_SENSOR_NUM_CALIBRATING	=	133	;
	MODBUS_SENSOR_POINT_CALIBRATING	=	134	;
	MODBUS_CO2_TSTAT_ADDR	=	140	;
	MODBUS_SET_CO2_ADDR	=	141	;
	MODBUS_CO2_ERROE	=	142	;
	MODBUS_CHARMBER_STABLE_TIME	=	143	;
	MODBUS_SENSOR_STABLE_TIME	=	144	;
	MODBUS_TIMER_SETTING0	=	500	;
	MODBUS_TIMER_SETTING1	=	501	;
	MODBUS_TIMER_SETTING2	=	502	;
	MODBUS_TIMER_SETTING3	=	503	;
	MODBUS_TIMER_SETTING4	=	504	;
	MODBUS_TIMER_SETTING5	=	505	;
	MODBUS_TIMER_SETTING6	=	506	;
	MODBUS_TIMER_SETTING7	=	507	;
	MODBUS_TIMER_SETTING8	=	508	;
	MODBUS_TIMER_SETTING9	=	509	;
	MODBUS_TEMP_SETTING0	=	510	;
	MODBUS_TEMP_SETTING1	=	511	;
	MODBUS_TEMP_SETTING2	=	512	;
	MODBUS_TEMP_SETTING3	=	513	;
	MODBUS_TEMP_SETTING4	=	514	;
	MODBUS_TEMP_SETTING5	=	515	;
	MODBUS_TEMP_SETTING6	=	516	;
	MODBUS_TEMP_SETTING7	=	517	;
	MODBUS_TEMP_SETTING8	=	518	;
	MODBUS_TEMP_SETTING9	=	519	;
	MODBUS_SENSOR_TEST_TIMER	=	520	;
	MODBUS_RAMPING_TIMER 	=	521	;
	MODBUS_RAMPING_TIMELEFT	=	522	;
	MODBUS_TIME_LEFT 	=	523	;
	MODBUS_POINT_TOTAL	=	524	;
	MODBUS_SENSOR_TOTAL	=	525	;
	MODBUS_POINT_NUM	=	526	;
	MODBUS_SENSOR_NUM	=	527	;
	MODBUS_SENSOR_HUM	=	528	;
	MODBUS_SENSOR_TEMP	=	529	;
	MODBUS_SENSOR_FRE	=	530	;
	MODBUS_SENSOR_POINTS	=	531	;
	MODBUS_SENSOR_HUMIDITY_ER	=	532	;
	MODBUS_SENSOR_FRE_ERROR	=	533	;
	MODBUS_ERROR_SENSOR_NUM	=	534	;
	MODBUS_CALIBRATE_STATE	=	535	;
	MODBUS_WORK_STEP	=	536	;
	MODBUS_POINTS_BITS_SET	=	537	;
	MODBUS_TSTAT_ADDRESS	=	538	;
	MODBUS_SET_TEMP_ADDR  	=	539	;
	MODBUS_SET_HUM_ADDR  	=	540	;
	MODBUS_CHAMBER_TEMP_ERROR	=	541	;
	MODBUS_CHAMBER_HUM_ERROR	=	542	;
	MODBUS_REFRESH_SENSOR	=	543	;
	MODBUS_HUMCOUNT1_H      	=	544	;
	MODBUS_HUMRH1_H	=	545	;
	MODBUS_HUMCOUNT2_H      	=	546	;
	MODBUS_HUMRH2_H	=	547	;
	MODBUS_HUMCOUNT3_H  	=	548	;
	MODBUS_HUMRH3_H	=	549	;
	MODBUS_HUMCOUNT4_H  	=	550	;
	MODBUS_HUMRH4_H	=	551	;
	MODBUS_HUMCOUNT5_H  	=	552	;
	MODBUS_HUMRH5_H	=	553	;
	MODBUS_HUMCOUNT6_H  	=	554	;
	MODBUS_HUMRH6_H	=	555	;
	MODBUS_HUMCOUNT7_H 	=	556	;
	MODBUS_HUMRH7_H	=	557	;
	MODBUS_HUMCOUNT8_H  	=	558	;
	MODBUS_HUMRH8_H	=	559	;
	MODBUS_HUMCOUNT9_H  	=	560	;
	MODBUS_HUMRH9_H	=	561	;
	MODBUS_HUMCOUNT10_H 	=	562	;
	MODBUS_HUMRH10_H        	=	563	;
	MODBUS_SENSOR1_STATE 	=	564	;
	MODBUS_SENSOR2_STATE	=	565	;
	MODBUS_SENSOR3_STATE	=	566	;
	MODBUS_SENSOR4_STATE	=	567	;
	MODBUS_SENSOR5_STATE	=	568	;
	MODBUS_SENSOR6_STATE	=	569	;
	MODBUS_SENSOR7_STATE	=	570	;
	MODBUS_SENSOR8_STATE	=	571	;
	MODBUS_SENSOR9_STATE	=	572	;
	MODBUS_SENSOR10_STATE	=	573	;
	MODBUS_SENSOR11_STATE	=	574	;
	MODBUS_SENSOR12_STATE	=	575	;
	MODBUS_SENSOR13_STATE	=	576	;
	MODBUS_SENSOR14_STATE	=	577	;
	MODBUS_SENSOR15_STATE	=	578	;
	MODBUS_SENSOR16_STATE	=	579	;
	MODBUS_SENSOR17_STATE	=	580	;
	MODBUS_SENSOR18_STATE	=	581	;
	MODBUS_SENSOR19_STATE	=	582	;
	MODBUS_SENSOR20_STATE	=	583	;
	MODBUS_SENSOR21_STATE	=	584	;
	MODBUS_SENSOR22_STATE	=	585	;
	MODBUS_SENSOR23_STATE	=	586	;
	MODBUS_SENSOR24_STATE	=	587	;
	MODBUS_SENSOR25_STATE	=	588	;
	MODBUS_SENSOR26_STATE	=	589	;
	MODBUS_SENSOR27_STATE	=	590	;
	MODBUS_SENSOR28_STATE	=	591	;
	MODBUS_SENSOR29_STATE	=	592	;
	MODBUS_SENSOR30_STATE	=	593	;
	MODBUS_SENSOR31_STATE	=	594	;
	MODBUS_SENSOR32_STATE	=	595	;
	MODBUS_SENSOR33_STATE	=	596	;
	MODBUS_SENSOR34_STATE	=	597	;
	MODBUS_SENSOR35_STATE	=	598	;
	MODBUS_SENSOR36_STATE	=	599	;
	MODBUS_SENSOR37_STATE	=	600	;
	MODBUS_SENSOR38_STATE	=	601	;
	MODBUS_SENSOR39_STATE	=	602	;
	MODBUS_SENSOR40_STATE	=	603	;
	MODBUS_SENSOR41_STATE	=	604	;
	MODBUS_SENSOR42_STATE	=	605	;
	MODBUS_SENSOR43_STATE	=	606	;
	MODBUS_SENSOR44_STATE	=	607	;
	MODBUS_SENSOR45_STATE	=	608	;
	MODBUS_SENSOR46_STATE	=	609	;
	MODBUS_SENSOR47_STATE	=	610	;
	MODBUS_SENSOR48_STATE	=	611	;
	MODBUS_SENSOR49_STATE	=	612	;
	MODBUS_SENSOR50_STATE	=	613	;
	MODBUS_SENSOR51_STATE	=	614	;
	MODBUS_SENSOR52_STATE	=	615	;
	MODBUS_SENSOR53_STATE	=	616	;
	MODBUS_SENSOR54_STATE	=	617	;
	MODBUS_SENSOR55_STATE	=	618	;
	MODBUS_SENSOR56_STATE	=	619	;
	MODBUS_SENSOR57_STATE	=	620	;
	MODBUS_SENSOR58_STATE	=	621	;
	MODBUS_SENSOR59_STATE	=	622	;
	MODBUS_SENSOR60_STATE	=	623	;
	MODBUS_SENSOR61_STATE	=	624	;
	MODBUS_SENSOR62_STATE	=	625	;
	MODBUS_SENSOR63_STATE	=	626	;
	MODBUS_SENSOR64_STATE	=	627	;
	MODBUS_SENSOR65_STATE	=	628	;
	MODBUS_SENSOR66_STATE	=	629	;
	MODBUS_SENSOR67_STATE	=	630	;
	MODBUS_SENSOR68_STATE	=	631	;
	MODBUS_SENSOR69_STATE	=	632	;
	MODBUS_SENSOR70_STATE	=	633	;
	MODBUS_SENSOR71_STATE	=	634	;
	MODBUS_SENSOR72_STATE	=	635	;
	MODBUS_SENSOR73_STATE	=	636	;
	MODBUS_SENSOR74_STATE	=	637	;
	MODBUS_SENSOR75_STATE	=	638	;
	MODBUS_SENSOR76_STATE	=	639	;
	MODBUS_SENSOR77_STATE	=	640	;
	MODBUS_SENSOR78_STATE	=	641	;
	MODBUS_SENSOR79_STATE	=	642	;
	MODBUS_SENSOR80_STATE	=	643	;
	MODBUS_SENSOR81_STATE	=	644	;
	MODBUS_SENSOR82_STATE	=	645	;
	MODBUS_SENSOR83_STATE	=	646	;
	MODBUS_SENSOR84_STATE	=	647	;
	MODBUS_SENSOR85_STATE	=	648	;
	MODBUS_SENSOR86_STATE	=	649	;
	MODBUS_SENSOR87_STATE	=	650	;
	MODBUS_SENSOR88_STATE	=	651	;
	MODBUS_SENSOR89_STATE	=	652	;
	MODBUS_SENSOR90_STATE	=	653	;
	MODBUS_SENSOR91_STATE	=	654	;
	MODBUS_SENSOR92_STATE	=	655	;
	MODBUS_SENSOR93_STATE	=	656	;
	MODBUS_SENSOR94_STATE	=	657	;
	MODBUS_SENSOR95_STATE	=	658	;
	MODBUS_SENSOR96_STATE	=	659	;
	MODBUS_SENSOR97_STATE	=	660	;
	MODBUS_SENSOR98_STATE	=	661	;
	MODBUS_SENSOR99_STATE	=	662	;
	MODBUS_SENSOR100_STATE	=	663	;
	MODBUS_SENSOR101_STATE	=	664	;
	MODBUS_SENSOR102_STATE	=	665	;
	MODBUS_SENSOR103_STATE	=	666	;
	MODBUS_SENSOR104_STATE 	=	667	;
	MODBUS_SENSOR1_TEMP     	=	668	;
	MODBUS_SENSOR1_HUM	=	669	;
	MODBUS_SENSOR2_TEMP    	=	670	;
	MODBUS_SENSOR2_HUM	=	671	;
	MODBUS_SENSOR3_TEMP    	=	672	;
	MODBUS_SENSOR3_HUM	=	673	;
	MODBUS_SENSOR4_TEMP    	=	674	;
	MODBUS_SENSOR4_HUM	=	675	;
	MODBUS_SENSOR5_TEMP    	=	676	;
	MODBUS_SENSOR5_HUM	=	677	;
	MODBUS_SENSOR6_TEMP    	=	678	;
	MODBUS_SENSOR6_HUM	=	679	;
	MODBUS_SENSOR7_TEMP    	=	680	;
	MODBUS_SENSOR7_HUM	=	681	;
	MODBUS_SENSOR8_TEMP    	=	682	;
	MODBUS_SENSOR8_HUM	=	683	;
	MODBUS_SENSOR9_TEMP    	=	684	;
	MODBUS_SENSOR9_HUM	=	685	;
	MODBUS_SENSOR10_TEMP    	=	686	;
	MODBUS_SENSOR10_HUM	=	687	;
	MODBUS_SENSOR11_TEMP    	=	688	;
	MODBUS_SENSOR11_HUM	=	689	;
	MODBUS_SENSOR12_TEMP    	=	690	;
	MODBUS_SENSOR12_HUM	=	691	;
	MODBUS_SENSOR13_TEMP    	=	692	;
	MODBUS_SENSOR13_HUM	=	693	;
	MODBUS_SENSOR14_TEMP    	=	694	;
	MODBUS_SENSOR14_HUM	=	695	;
	MODBUS_SENSOR15_TEMP    	=	696	;
	MODBUS_SENSOR15_HUM	=	697	;
	MODBUS_SENSOR16_TEMP    	=	698	;
	MODBUS_SENSOR16_HUM	=	699	;
	MODBUS_SENSOR17_TEMP    	=	700	;
	MODBUS_SENSOR17_HUM	=	701	;
	MODBUS_SENSOR18_TEMP    	=	702	;
	MODBUS_SENSOR18_HUM	=	703	;
	MODBUS_SENSOR19_TEMP    	=	704	;
	MODBUS_SENSOR19_HUM	=	705	;
	MODBUS_SENSOR20_TEMP    	=	706	;
	MODBUS_SENSOR20_HUM	=	707	;
	MODBUS_SENSOR21_TEMP    	=	708	;
	MODBUS_SENSOR21_HUM	=	709	;
	MODBUS_SENSOR22_TEMP    	=	710	;
	MODBUS_SENSOR22_HUM	=	711	;
	MODBUS_SENSOR23_TEMP    	=	712	;
	MODBUS_SENSOR23_HUM	=	713	;
	MODBUS_SENSOR24_TEMP    	=	714	;
	MODBUS_SENSOR24_HUM	=	715	;
	MODBUS_SENSOR25_TEMP    	=	716	;
	MODBUS_SENSOR25_HUM	=	717	;
	MODBUS_SENSOR26_TEMP    	=	718	;
	MODBUS_SENSOR26_HUM	=	719	;
	MODBUS_SENSOR27_TEMP    	=	720	;
	MODBUS_SENSOR27_HUM	=	721	;
	MODBUS_SENSOR28_TEMP    	=	722	;
	MODBUS_SENSOR28_HUM	=	723	;
	MODBUS_SENSOR29_TEMP    	=	724	;
	MODBUS_SENSOR29_HUM	=	725	;
	MODBUS_SENSOR30_TEMP    	=	726	;
	MODBUS_SENSOR30_HUM	=	727	;
	MODBUS_SENSOR31_TEMP    	=	728	;
	MODBUS_SENSOR31_HUM	=	729	;
	MODBUS_SENSOR32_TEMP    	=	730	;
	MODBUS_SENSOR32_HUM	=	731	;
	MODBUS_SENSOR33_TEMP    	=	732	;
	MODBUS_SENSOR33_HUM	=	733	;
	MODBUS_SENSOR34_TEMP    	=	734	;
	MODBUS_SENSOR34_HUM	=	735	;
	MODBUS_SENSOR35_TEMP    	=	736	;
	MODBUS_SENSOR35_HUM	=	737	;
	MODBUS_SENSOR36_TEMP    	=	738	;
	MODBUS_SENSOR36_HUM	=	739	;
	MODBUS_SENSOR37_TEMP    	=	740	;
	MODBUS_SENSOR37_HUM	=	741	;
	MODBUS_SENSOR38_TEMP    	=	742	;
	MODBUS_SENSOR38_HUM	=	743	;
	MODBUS_SENSOR39_TEMP    	=	744	;
	MODBUS_SENSOR39_HUM	=	745	;
	MODBUS_SENSOR40_TEMP    	=	746	;
	MODBUS_SENSOR40_HUM	=	747	;
	MODBUS_SENSOR41_TEMP    	=	748	;
	MODBUS_SENSOR41_HUM	=	749	;
	MODBUS_SENSOR42_TEMP    	=	750	;
	MODBUS_SENSOR42_HUM	=	751	;
	MODBUS_SENSOR43_TEMP    	=	752	;
	MODBUS_SENSOR43_HUM	=	753	;
	MODBUS_SENSOR44_TEMP    	=	754	;
	MODBUS_SENSOR44_HUM	=	755	;
	MODBUS_SENSOR45_TEMP    	=	756	;
	MODBUS_SENSOR45_HUM	=	757	;
	MODBUS_SENSOR46_TEMP    	=	758	;
	MODBUS_SENSOR46_HUM	=	759	;
	MODBUS_SENSOR47_TEMP    	=	760	;
	MODBUS_SENSOR47_HUM	=	761	;
	MODBUS_SENSOR48_TEMP    	=	762	;
	MODBUS_SENSOR48_HUM	=	763	;
	MODBUS_SENSOR49_TEMP    	=	764	;
	MODBUS_SENSOR49_HUM	=	765	;
	MODBUS_SENSOR50_TEMP    	=	766	;
	MODBUS_SENSOR50_HUM	=	767	;
	MODBUS_SENSOR51_TEMP    	=	768	;
	MODBUS_SENSOR51_HUM	=	769	;
	MODBUS_SENSOR52_TEMP    	=	770	;
	MODBUS_SENSOR52_HUM	=	771	;
	MODBUS_SENSOR53_TEMP    	=	772	;
	MODBUS_SENSOR53_HUM	=	773	;
	MODBUS_SENSOR54_TEMP    	=	774	;
	MODBUS_SENSOR54_HUM	=	775	;
	MODBUS_SENSOR55_TEMP    	=	776	;
	MODBUS_SENSOR55_HUM	=	777	;
	MODBUS_SENSOR56_TEMP    	=	778	;
	MODBUS_SENSOR56_HUM	=	779	;
	MODBUS_SENSOR57_TEMP    	=	780	;
	MODBUS_SENSOR57_HUM	=	781	;
	MODBUS_SENSOR58_TEMP    	=	782	;
	MODBUS_SENSOR58_HUM	=	783	;
	MODBUS_SENSOR59_TEMP	=	784	;
	MODBUS_SENSOR59_HUM	=	785	;
	MODBUS_SENSOR60_TEMP    	=	786	;
	MODBUS_SENSOR60_HUM	=	787	;
	MODBUS_SENSOR61_TEMP    	=	788	;
	MODBUS_SENSOR61_HUM	=	789	;
	MODBUS_SENSOR62_TEMP    	=	790	;
	MODBUS_SENSOR62_HUM	=	791	;
	MODBUS_SENSOR63_TEMP    	=	792	;
	MODBUS_SENSOR63_HUM	=	793	;
	MODBUS_SENSOR64_TEMP    	=	794	;
	MODBUS_SENSOR64_HUM	=	795	;
	MODBUS_SENSOR65_TEMP    	=	796	;
	MODBUS_SENSOR65_HUM	=	797	;
	MODBUS_SENSOR66_TEMP    	=	798	;
	MODBUS_SENSOR66_HUM	=	799	;
	MODBUS_SENSOR67_TEMP    	=	800	;
	MODBUS_SENSOR67_HUM	=	801	;
	MODBUS_SENSOR68_TEMP    	=	802	;
	MODBUS_SENSOR68_HUM	=	803	;
	MODBUS_SENSOR69_TEMP    	=	804	;
	MODBUS_SENSOR69_HUM	=	805	;
	MODBUS_SENSOR70_TEMP    	=	806	;
	MODBUS_SENSOR70_HUM	=	807	;
	MODBUS_SENSOR71_TEMP    	=	808	;
	MODBUS_SENSOR71_HUM	=	809	;
	MODBUS_SENSOR72_TEMP    	=	810	;
	MODBUS_SENSOR72_HUM	=	811	;
	MODBUS_SENSOR73_TEMP    	=	812	;
	MODBUS_SENSOR73_HUM	=	813	;
	MODBUS_SENSOR74_TEMP    	=	814	;
	MODBUS_SENSOR74_HUM	=	815	;
	MODBUS_SENSOR75_TEMP    	=	816	;
	MODBUS_SENSOR75_HUM	=	817	;
	MODBUS_SENSOR76_TEMP    	=	818	;
	MODBUS_SENSOR76_HUM	=	819	;
	MODBUS_SENSOR77_TEMP    	=	820	;
	MODBUS_SENSOR77_HUM	=	821	;
	MODBUS_SENSOR78_TEMP    	=	822	;
	MODBUS_SENSOR78_HUM	=	823	;
	MODBUS_SENSOR79_TEMP    	=	824	;
	MODBUS_SENSOR79_HUM	=	825	;
	MODBUS_SENSOR80_TEMP    	=	826	;
	MODBUS_SENSOR80_HUM	=	827	;
	MODBUS_SENSOR81_TEMP    	=	828	;
	MODBUS_SENSOR81_HUM	=	829	;
	MODBUS_SENSOR82_TEMP    	=	830	;
	MODBUS_SENSOR82_HUM	=	831	;
	MODBUS_SENSOR83_TEMP    	=	832	;
	MODBUS_SENSOR83_HUM	=	833	;
	MODBUS_SENSOR84_TEMP    	=	834	;
	MODBUS_SENSOR84_HUM	=	835	;
	MODBUS_SENSOR85_TEMP    	=	836	;
	MODBUS_SENSOR85_HUM	=	837	;
	MODBUS_SENSOR86_TEMP    	=	838	;
	MODBUS_SENSOR86_HUM	=	839	;
	MODBUS_SENSOR87_TEMP    	=	840	;
	MODBUS_SENSOR87_HUM	=	841	;
	MODBUS_SENSOR88_TEMP    	=	842	;
	MODBUS_SENSOR88_HUM	=	843	;
	MODBUS_SENSOR89_TEMP    	=	844	;
	MODBUS_SENSOR89_HUM	=	845	;
	MODBUS_SENSOR90_TEMP    	=	846	;
	MODBUS_SENSOR90_HUM	=	847	;
	MODBUS_SENSOR91_TEMP    	=	848	;
	MODBUS_SENSOR91_HUM	=	849	;
	MODBUS_SENSOR92_TEMP    	=	850	;
	MODBUS_SENSOR92_HUM	=	851	;
	MODBUS_SENSOR93_TEMP    	=	852	;
	MODBUS_SENSOR93_HUM	=	853	;
	MODBUS_SENSOR94_TEMP    	=	854	;
	MODBUS_SENSOR94_HUM	=	855	;
	MODBUS_SENSOR95_TEMP    	=	856	;
	MODBUS_SENSOR95_HUM	=	857	;
	MODBUS_SENSOR96_TEMP    	=	858	;
	MODBUS_SENSOR96_HUM	=	859	;
	MODBUS_SENSOR97_TEMP    	=	860	;
	MODBUS_SENSOR97_HUM	=	861	;
	MODBUS_SENSOR98_TEMP    	=	862	;
	MODBUS_SENSOR98_HUM	=	863	;
	MODBUS_SENSOR99_TEMP    	=	864	;
	MODBUS_SENSOR99_HUM	=	865	;
	MODBUS_SENSOR100_TEMP   	=	866	;
	MODBUS_SENSOR100_HUM	=	867	;
	MODBUS_SENSOR101_TEMP   	=	868	;
	MODBUS_SENSOR101_HUM	=	869	;
	MODBUS_SENSOR102_TEMP   	=	870	;
	MODBUS_SENSOR102_HUM	=	871	;
	MODBUS_SENSOR103_TEMP   	=	872	;
	MODBUS_SENSOR103_HUM	=	873	;
	MODBUS_SENSOR104_TEMP   	=	874	;
	MODBUS_SENSOR104_HUM   	=	875	;
	MODBUS_SENSOR1_SN	=	876	;
	MODBUS_SENSOR2_SN	=	877	;
	MODBUS_SENSOR3_SN	=	878	;
	MODBUS_SENSOR4_SN	=	879	;
	MODBUS_SENSOR5_SN	=	880	;
	MODBUS_SENSOR6_SN	=	881	;
	MODBUS_SENSOR7_SN	=	882	;
	MODBUS_SENSOR8_SN	=	883	;
	MODBUS_SENSOR9_SN	=	884	;
	MODBUS_SENSOR10_SN	=	885	;
	MODBUS_SENSOR11_SN	=	886	;
	MODBUS_SENSOR12_SN	=	887	;
	MODBUS_SENSOR13_SN	=	888	;
	MODBUS_SENSOR14_SN	=	889	;
	MODBUS_SENSOR15_SN	=	890	;
	MODBUS_SENSOR16_SN	=	891	;
	MODBUS_SENSOR17_SN	=	892	;
	MODBUS_SENSOR18_SN	=	893	;
	MODBUS_SENSOR19_SN	=	894	;
	MODBUS_SENSOR20_SN	=	895	;
	MODBUS_SENSOR21_SN	=	896	;
	MODBUS_SENSOR22_SN	=	897	;
	MODBUS_SENSOR23_SN	=	898	;
	MODBUS_SENSOR24_SN	=	899	;
	MODBUS_SENSOR25_SN	=	900	;
	MODBUS_SENSOR26_SN	=	901	;
	MODBUS_SENSOR27_SN	=	902	;
	MODBUS_SENSOR28_SN	=	903	;
	MODBUS_SENSOR29_SN	=	904	;
	MODBUS_SENSOR30_SN	=	905	;
	MODBUS_SENSOR31_SN	=	906	;
	MODBUS_SENSOR32_SN	=	907	;
	MODBUS_SENSOR33_SN	=	908	;
	MODBUS_SENSOR34_SN	=	909	;
	MODBUS_SENSOR35_SN	=	910	;
	MODBUS_SENSOR36_SN	=	911	;
	MODBUS_SENSOR37_SN	=	912	;
	MODBUS_SENSOR38_SN	=	913	;
	MODBUS_SENSOR39_SN	=	914	;
	MODBUS_SENSOR40_SN	=	915	;
	MODBUS_SENSOR41_SN	=	916	;
	MODBUS_SENSOR42_SN	=	917	;
	MODBUS_SENSOR43_SN	=	918	;
	MODBUS_SENSOR44_SN	=	919	;
	MODBUS_SENSOR45_SN	=	920	;
	MODBUS_SENSOR46_SN	=	921	;
	MODBUS_SENSOR47_SN	=	922	;
	MODBUS_SENSOR48_SN	=	923	;
	MODBUS_SENSOR49_SN	=	924	;
	MODBUS_SENSOR50_SN	=	925	;
	MODBUS_SENSOR51_SN	=	926	;
	MODBUS_SENSOR52_SN	=	927	;
	MODBUS_SENSOR53_SN	=	928	;
	MODBUS_SENSOR54_SN	=	929	;
	MODBUS_SENSOR55_SN	=	930	;
	MODBUS_SENSOR56_SN	=	931	;
	MODBUS_SENSOR57_SN	=	932	;
	MODBUS_SENSOR58_SN	=	933	;
	MODBUS_SENSOR59_SN	=	934	;
	MODBUS_SENSOR60_SN	=	935	;
	MODBUS_SENSOR61_SN	=	936	;
	MODBUS_SENSOR62_SN	=	937	;
	MODBUS_SENSOR63_SN	=	938	;
	MODBUS_SENSOR64_SN	=	939	;
	MODBUS_SENSOR65_SN	=	940	;
	MODBUS_SENSOR66_SN	=	941	;
	MODBUS_SENSOR67_SN	=	942	;
	MODBUS_SENSOR68_SN	=	943	;
	MODBUS_SENSOR69_SN	=	944	;
	MODBUS_SENSOR70_SN 	=	945	;
	MODBUS_SENSOR71_SN	=	946	;
	MODBUS_SENSOR72_SN	=	947	;
	MODBUS_SENSOR73_SN	=	948	;
	MODBUS_SENSOR74_SN	=	949	;
	MODBUS_SENSOR75_SN	=	950	;
	MODBUS_SENSOR76_SN	=	951	;
	MODBUS_SENSOR77_SN	=	952	;
	MODBUS_SENSOR78_SN	=	953	;
	MODBUS_SENSOR79_SN	=	954	;
	MODBUS_SENSOR80_SN	=	955	;
	MODBUS_SENSOR81_SN	=	956	;
	MODBUS_SENSOR82_SN	=	957	;
	MODBUS_SENSOR83_SN	=	958	;
	MODBUS_SENSOR84_SN	=	959	;
	MODBUS_SENSOR85_SN	=	960	;
	MODBUS_SENSOR86_SN	=	961	;
	MODBUS_SENSOR87_SN	=	962	;
	MODBUS_SENSOR88_SN	=	963	;
	MODBUS_SENSOR89_SN	=	964	;
	MODBUS_SENSOR90_SN	=	965	;
	MODBUS_SENSOR91_SN	=	966	;
	MODBUS_SENSOR92_SN	=	967	;
	MODBUS_SENSOR93_SN	=	968	;
	MODBUS_SENSOR94_SN	=	969	;
	MODBUS_SENSOR95_SN	=	970	;
	MODBUS_SENSOR96_SN	=	971	;
	MODBUS_SENSOR97_SN	=	972	;
	MODBUS_SENSOR98_SN	=	973	;
	MODBUS_SENSOR99_SN	=	974	;
	MODBUS_SENSOR100_SN	=	975	;
	MODBUS_SENSOR101_SN	=	976	;
	MODBUS_SENSOR102_SN	=	977	;
	MODBUS_SENSOR103_SN	=	978	;
	MODBUS_SENSOR104_SN	=	979	;
	MODBUS_CLEAR_TABLE_MANUAL	=	980	;
	MODBUS_CALIBRATE_MANUAL	=	981	;
	MODBUS_FRE_MANUAL	=	982	;
	MODBUS_HUM_MANUAL 	=	983	;
	MODBUS_RESULT2  	=	984	;
	MODBUS_TEST0	=	985	;
	MODBUS_TEST1	=	986	;
	MODBUS_TEST2	=	987	;
	MODBUS_TEST3	=	988	;
	MODBUS_TEST4	=	989	;
	MODBUS_TEST5	=	990	;
	MODBUS_TEST6	=	991	;
	MODBUS_FRE_TEMP0	=	992	;
	MODBUS_FRE_TEMP1	=	993	;
	MODBUS_FRE_TEMP2	=	994	;
	MODBUS_FRE_TEMP3	=	995	;
	MODBUS_FRE_TEMP4	=	996	;
	MODBUS_FRE_TEMP5	=	997	;
	MODBUS_FRE_TEMP6	=	998	;
	MODBUS_FRE_TEMP7	=	999	;
	MODBUS_FRE_TEMP8	=	1000	;
	MODBUS_FRE_TEMP9	=	1001	;
	MODBUS_HUM_TEMP0	=	1002	;
	MODBUS_HUM_TEMP1	=	1003	;
	MODBUS_HUM_TEMP2	=	1004	;
	MODBUS_HUM_TEMP3	=	1005	;
	MODBUS_HUM_TEMP4	=	1006	;
	MODBUS_HUM_TEMP5	=	1007	;
	MODBUS_HUM_TEMP6	=	1008	;
	MODBUS_HUM_TEMP7	=	1009	;
	MODBUS_HUM_TEMP8	=	1010	;
	MODBUS_HUM_TEMP9	=	1011	;
	MODBUS_REGISTER_END	=	1012	;
#endif
	T3Register temp;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
#if 1
	if (!SqliteDBT3000.tableExists("HumChamber_Reglist"))
	{
		return;
	}
	CString SQL = _T("select * from HumChamber_Reglist");
	q = SqliteDBT3000.execQuery((UTF8MBSTR)SQL);
	 
	_variant_t vartemp;
	while(!q.eof())
	{
		temp.regID=q.getIntField("RegID"); 
		temp.regName = q.getValuebyName(L"RegName");
		q.nextRow();
		m_vecT3Register.push_back(temp);
	}
	SqliteDBT3000.closedb();
#endif 
#if 1
	MODBUS_SERIALNUMBER_LOWORD	=	Get_RegID(_T("	MODBUS_SERIALNUMBER_LOWORD	"))	;
	MODBUS_SERIALNUMBER_HIWORD	=	Get_RegID(_T("	MODBUS_SERIALNUMBER_HIWORD	"))	;
	MODBUS_VERSION_NUMBER_LO	=	Get_RegID(_T("	MODBUS_VERSION_NUMBER_LO	"))	;
	MODBUS_VERSION_NUMBER_HI	=	Get_RegID(_T("	MODBUS_VERSION_NUMBER_HI	"))	;
	MODBUS_ADDRESS	=	Get_RegID(_T("	MODBUS_ADDRESS	"))	;
	MODBUS_PRODUCT_MODEL	=	Get_RegID(_T("	MODBUS_PRODUCT_MODEL	"))	;
	MODBUS_HARDWARE_REV	=	Get_RegID(_T("	MODBUS_HARDWARE_REV	"))	;
	MODBUS_PIC_VERSION	=	Get_RegID(_T("	MODBUS_PIC_VERSION	"))	;
	MODBUS_ADDRESS_PLUG_N_PLAY	=	Get_RegID(_T("	MODBUS_ADDRESS_PLUG_N_PLAY	"))	;
	MODBUS_BASE_ADDRESS	=	Get_RegID(_T("	MODBUS_BASE_ADDRESS	"))	;
	MODBUS_UPDATE_STATUS	=	Get_RegID(_T("	MODBUS_UPDATE_STATUS	"))	;
	MODBUS_SERINALNUMBER_WRITE_FLAG	=	Get_RegID(_T("	MODBUS_SERINALNUMBER_WRITE_FLAG	"))	;
	MODBUS_TESTO_CO2	=	Get_RegID(_T("	MODBUS_TESTO_CO2	"))	;
	MODBUS_TESTO_HUM	=	Get_RegID(_T("	MODBUS_TESTO_HUM	"))	;
	MODBUS_TESTO_T	=	Get_RegID(_T("	MODBUS_TESTO_T	"))	;
	MODBUS_TESTO_P	=	Get_RegID(_T("	MODBUS_TESTO_P	"))	;
	MODBUS_CO2_CAL_POINT	=	Get_RegID(_T("	MODBUS_CO2_CAL_POINT	"))	;
	MODBUS_CO2_SETTING0	=	Get_RegID(_T("	MODBUS_CO2_SETTING0	"))	;
	MODBUS_CO2_SETTING1	=	Get_RegID(_T("	MODBUS_CO2_SETTING1	"))	;
	MODBUS_CO2_SETTING2	=	Get_RegID(_T("	MODBUS_CO2_SETTING2	"))	;
	MODBUS_CO2_SETTING3	=	Get_RegID(_T("	MODBUS_CO2_SETTING3	"))	;
	MODBUS_CO2_SETTING4	=	Get_RegID(_T("	MODBUS_CO2_SETTING4	"))	;
	MODBUS_CO2_SETTING5	=	Get_RegID(_T("	MODBUS_CO2_SETTING5	"))	;
	MODBUS_CO2_SETTING6	=	Get_RegID(_T("	MODBUS_CO2_SETTING6	"))	;
	MODBUS_CO2_SETTING7	=	Get_RegID(_T("	MODBUS_CO2_SETTING7	"))	;
	MODBUS_CO2_SETTING8	=	Get_RegID(_T("	MODBUS_CO2_SETTING8	"))	;
	MODBUS_CO2_SETTING9	=	Get_RegID(_T("	MODBUS_CO2_SETTING9	"))	;
	MODBUS_CO2_RTD_SETTING0	=	Get_RegID(_T("	MODBUS_CO2_RTD_SETTING0	"))	;
	MODBUS_CO2_RTD_SETTING1	=	Get_RegID(_T("	MODBUS_CO2_RTD_SETTING1	"))	;
	MODBUS_CO2_SAMETIME_NUM	=	Get_RegID(_T("	MODBUS_CO2_SAMETIME_NUM	"))	;
	MODBUS_SENSOR_TYPE	=	Get_RegID(_T("	MODBUS_SENSOR_TYPE	"))	;
	MODBUS_HUM_CAL_POINT	=	Get_RegID(_T("	MODBUS_HUM_CAL_POINT	"))	;
	MODBUS_HUM_SETTING0	=	Get_RegID(_T("	MODBUS_HUM_SETTING0	"))	;
	MODBUS_HUM_SETTING1	=	Get_RegID(_T("	MODBUS_HUM_SETTING1	"))	;
	MODBUS_HUM_SETTING2	=	Get_RegID(_T("	MODBUS_HUM_SETTING2	"))	;
	MODBUS_HUM_SETTING3	=	Get_RegID(_T("	MODBUS_HUM_SETTING3	"))	;
	MODBUS_HUM_SETTING4	=	Get_RegID(_T("	MODBUS_HUM_SETTING4	"))	;
	MODBUS_HUM_SETTING5	=	Get_RegID(_T("	MODBUS_HUM_SETTING5	"))	;
	MODBUS_HUM_SETTING6	=	Get_RegID(_T("	MODBUS_HUM_SETTING6	"))	;
	MODBUS_HUM_SETTING7	=	Get_RegID(_T("	MODBUS_HUM_SETTING7	"))	;
	MODBUS_HUM_SETTING8	=	Get_RegID(_T("	MODBUS_HUM_SETTING8	"))	;
	MODBUS_HUM_SETTING9	=	Get_RegID(_T("	MODBUS_HUM_SETTING9	"))	;
	MODBUS_ALL_SENSOR_NUM 	=	Get_RegID(_T("	MODBUS_ALL_SENSOR_NUM 	"))	;
	MODBUS_CAL_STEPS	=	Get_RegID(_T("	MODBUS_CAL_STEPS	"))	;
	MODBUS_SENSOR_NUM_CALIBRATING	=	Get_RegID(_T("	MODBUS_SENSOR_NUM_CALIBRATING	"))	;
	MODBUS_SENSOR_POINT_CALIBRATING	=	Get_RegID(_T("	MODBUS_SENSOR_POINT_CALIBRATING	"))	;
	MODBUS_CO2_TSTAT_ADDR	=	Get_RegID(_T("	MODBUS_CO2_TSTAT_ADDR	"))	;
	MODBUS_SET_CO2_ADDR	=	Get_RegID(_T("	MODBUS_SET_CO2_ADDR	"))	;
	MODBUS_CO2_ERROE	=	Get_RegID(_T("	MODBUS_CO2_ERROE	"))	;
	MODBUS_CHARMBER_STABLE_TIME	=	Get_RegID(_T("	MODBUS_CHARMBER_STABLE_TIME	"))	;
	MODBUS_SENSOR_STABLE_TIME	=	Get_RegID(_T("	MODBUS_SENSOR_STABLE_TIME	"))	;
	MODBUS_TIMER_SETTING0	=	Get_RegID(_T("	MODBUS_TIMER_SETTING0	"))	;
	MODBUS_TIMER_SETTING1	=	Get_RegID(_T("	MODBUS_TIMER_SETTING1	"))	;
	MODBUS_TIMER_SETTING2	=	Get_RegID(_T("	MODBUS_TIMER_SETTING2	"))	;
	MODBUS_TIMER_SETTING3	=	Get_RegID(_T("	MODBUS_TIMER_SETTING3	"))	;
	MODBUS_TIMER_SETTING4	=	Get_RegID(_T("	MODBUS_TIMER_SETTING4	"))	;
	MODBUS_TIMER_SETTING5	=	Get_RegID(_T("	MODBUS_TIMER_SETTING5	"))	;
	MODBUS_TIMER_SETTING6	=	Get_RegID(_T("	MODBUS_TIMER_SETTING6	"))	;
	MODBUS_TIMER_SETTING7	=	Get_RegID(_T("	MODBUS_TIMER_SETTING7	"))	;
	MODBUS_TIMER_SETTING8	=	Get_RegID(_T("	MODBUS_TIMER_SETTING8	"))	;
	MODBUS_TIMER_SETTING9	=	Get_RegID(_T("	MODBUS_TIMER_SETTING9	"))	;
	MODBUS_TEMP_SETTING0	=	Get_RegID(_T("	MODBUS_TEMP_SETTING0	"))	;
	MODBUS_TEMP_SETTING1	=	Get_RegID(_T("	MODBUS_TEMP_SETTING1	"))	;
	MODBUS_TEMP_SETTING2	=	Get_RegID(_T("	MODBUS_TEMP_SETTING2	"))	;
	MODBUS_TEMP_SETTING3	=	Get_RegID(_T("	MODBUS_TEMP_SETTING3	"))	;
	MODBUS_TEMP_SETTING4	=	Get_RegID(_T("	MODBUS_TEMP_SETTING4	"))	;
	MODBUS_TEMP_SETTING5	=	Get_RegID(_T("	MODBUS_TEMP_SETTING5	"))	;
	MODBUS_TEMP_SETTING6	=	Get_RegID(_T("	MODBUS_TEMP_SETTING6	"))	;
	MODBUS_TEMP_SETTING7	=	Get_RegID(_T("	MODBUS_TEMP_SETTING7	"))	;
	MODBUS_TEMP_SETTING8	=	Get_RegID(_T("	MODBUS_TEMP_SETTING8	"))	;
	MODBUS_TEMP_SETTING9	=	Get_RegID(_T("	MODBUS_TEMP_SETTING9	"))	;
	MODBUS_SENSOR_TEST_TIMER	=	Get_RegID(_T("	MODBUS_SENSOR_TEST_TIMER	"))	;
	MODBUS_RAMPING_TIMER 	=	Get_RegID(_T("	MODBUS_RAMPING_TIMER 	"))	;
	MODBUS_RAMPING_TIMELEFT	=	Get_RegID(_T("	MODBUS_RAMPING_TIMELEFT	"))	;
	MODBUS_TIME_LEFT 	=	Get_RegID(_T("	MODBUS_TIME_LEFT 	"))	;
	MODBUS_POINT_TOTAL	=	Get_RegID(_T("	MODBUS_POINT_TOTAL	"))	;
	MODBUS_SENSOR_TOTAL	=	Get_RegID(_T("	MODBUS_SENSOR_TOTAL	"))	;
	MODBUS_POINT_NUM	=	Get_RegID(_T("	MODBUS_POINT_NUM	"))	;
	MODBUS_SENSOR_NUM	=	Get_RegID(_T("	MODBUS_SENSOR_NUM	"))	;
	MODBUS_SENSOR_HUM	=	Get_RegID(_T("	MODBUS_SENSOR_HUM	"))	;
	MODBUS_SENSOR_TEMP	=	Get_RegID(_T("	MODBUS_SENSOR_TEMP	"))	;
	MODBUS_SENSOR_FRE	=	Get_RegID(_T("	MODBUS_SENSOR_FRE	"))	;
	MODBUS_SENSOR_POINTS	=	Get_RegID(_T("	MODBUS_SENSOR_POINTS	"))	;
	MODBUS_SENSOR_HUMIDITY_ER	=	Get_RegID(_T("	MODBUS_SENSOR_HUMIDITY_ER	"))	;
	MODBUS_SENSOR_FRE_ERROR	=	Get_RegID(_T("	MODBUS_SENSOR_FRE_ERROR	"))	;
	MODBUS_ERROR_SENSOR_NUM	=	Get_RegID(_T("	MODBUS_ERROR_SENSOR_NUM	"))	;
	MODBUS_CALIBRATE_STATE	=	Get_RegID(_T("	MODBUS_CALIBRATE_STATE	"))	;
	MODBUS_WORK_STEP	=	Get_RegID(_T("	MODBUS_WORK_STEP	"))	;
	MODBUS_POINTS_BITS_SET	=	Get_RegID(_T("	MODBUS_POINTS_BITS_SET	"))	;
	MODBUS_TSTAT_ADDRESS	=	Get_RegID(_T("	MODBUS_TSTAT_ADDRESS	"))	;
	MODBUS_SET_TEMP_ADDR  	=	Get_RegID(_T("	MODBUS_SET_TEMP_ADDR  	"))	;
	MODBUS_SET_HUM_ADDR  	=	Get_RegID(_T("	MODBUS_SET_HUM_ADDR  	"))	;
	MODBUS_CHAMBER_TEMP_ERROR	=	Get_RegID(_T("	MODBUS_CHAMBER_TEMP_ERROR	"))	;
	MODBUS_CHAMBER_HUM_ERROR	=	Get_RegID(_T("	MODBUS_CHAMBER_HUM_ERROR	"))	;
	MODBUS_REFRESH_SENSOR	=	Get_RegID(_T("	MODBUS_REFRESH_SENSOR	"))	;
	MODBUS_HUMCOUNT1_H      	=	Get_RegID(_T("	MODBUS_HUMCOUNT1_H      	"))	;
	MODBUS_HUMRH1_H	=	Get_RegID(_T("	MODBUS_HUMRH1_H	"))	;
	MODBUS_HUMCOUNT2_H      	=	Get_RegID(_T("	MODBUS_HUMCOUNT2_H      	"))	;
	MODBUS_HUMRH2_H	=	Get_RegID(_T("	MODBUS_HUMRH2_H	"))	;
	MODBUS_HUMCOUNT3_H  	=	Get_RegID(_T("	MODBUS_HUMCOUNT3_H  	"))	;
	MODBUS_HUMRH3_H	=	Get_RegID(_T("	MODBUS_HUMRH3_H	"))	;
	MODBUS_HUMCOUNT4_H  	=	Get_RegID(_T("	MODBUS_HUMCOUNT4_H  	"))	;
	MODBUS_HUMRH4_H	=	Get_RegID(_T("	MODBUS_HUMRH4_H	"))	;
	MODBUS_HUMCOUNT5_H  	=	Get_RegID(_T("	MODBUS_HUMCOUNT5_H  	"))	;
	MODBUS_HUMRH5_H	=	Get_RegID(_T("	MODBUS_HUMRH5_H	"))	;
	MODBUS_HUMCOUNT6_H  	=	Get_RegID(_T("	MODBUS_HUMCOUNT6_H  	"))	;
	MODBUS_HUMRH6_H	=	Get_RegID(_T("	MODBUS_HUMRH6_H	"))	;
	MODBUS_HUMCOUNT7_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT7_H 	"))	;
	MODBUS_HUMRH7_H	=	Get_RegID(_T("	MODBUS_HUMRH7_H	"))	;
	MODBUS_HUMCOUNT8_H  	=	Get_RegID(_T("	MODBUS_HUMCOUNT8_H  	"))	;
	MODBUS_HUMRH8_H	=	Get_RegID(_T("	MODBUS_HUMRH8_H	"))	;
	MODBUS_HUMCOUNT9_H  	=	Get_RegID(_T("	MODBUS_HUMCOUNT9_H  	"))	;
	MODBUS_HUMRH9_H	=	Get_RegID(_T("	MODBUS_HUMRH9_H	"))	;
	MODBUS_HUMCOUNT10_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT10_H 	"))	;
	MODBUS_HUMRH10_H        	=	Get_RegID(_T("	MODBUS_HUMRH10_H        	"))	;
	MODBUS_SENSOR1_STATE 	=	Get_RegID(_T("	MODBUS_SENSOR1_STATE 	"))	;
	MODBUS_SENSOR2_STATE	=	Get_RegID(_T("	MODBUS_SENSOR2_STATE	"))	;
	MODBUS_SENSOR3_STATE	=	Get_RegID(_T("	MODBUS_SENSOR3_STATE	"))	;
	MODBUS_SENSOR4_STATE	=	Get_RegID(_T("	MODBUS_SENSOR4_STATE	"))	;
	MODBUS_SENSOR5_STATE	=	Get_RegID(_T("	MODBUS_SENSOR5_STATE	"))	;
	MODBUS_SENSOR6_STATE	=	Get_RegID(_T("	MODBUS_SENSOR6_STATE	"))	;
	MODBUS_SENSOR7_STATE	=	Get_RegID(_T("	MODBUS_SENSOR7_STATE	"))	;
	MODBUS_SENSOR8_STATE	=	Get_RegID(_T("	MODBUS_SENSOR8_STATE	"))	;
	MODBUS_SENSOR9_STATE	=	Get_RegID(_T("	MODBUS_SENSOR9_STATE	"))	;
	MODBUS_SENSOR10_STATE	=	Get_RegID(_T("	MODBUS_SENSOR10_STATE	"))	;
	MODBUS_SENSOR11_STATE	=	Get_RegID(_T("	MODBUS_SENSOR11_STATE	"))	;
	MODBUS_SENSOR12_STATE	=	Get_RegID(_T("	MODBUS_SENSOR12_STATE	"))	;
	MODBUS_SENSOR13_STATE	=	Get_RegID(_T("	MODBUS_SENSOR13_STATE	"))	;
	MODBUS_SENSOR14_STATE	=	Get_RegID(_T("	MODBUS_SENSOR14_STATE	"))	;
	MODBUS_SENSOR15_STATE	=	Get_RegID(_T("	MODBUS_SENSOR15_STATE	"))	;
	MODBUS_SENSOR16_STATE	=	Get_RegID(_T("	MODBUS_SENSOR16_STATE	"))	;
	MODBUS_SENSOR17_STATE	=	Get_RegID(_T("	MODBUS_SENSOR17_STATE	"))	;
	MODBUS_SENSOR18_STATE	=	Get_RegID(_T("	MODBUS_SENSOR18_STATE	"))	;
	MODBUS_SENSOR19_STATE	=	Get_RegID(_T("	MODBUS_SENSOR19_STATE	"))	;
	MODBUS_SENSOR20_STATE	=	Get_RegID(_T("	MODBUS_SENSOR20_STATE	"))	;
	MODBUS_SENSOR21_STATE	=	Get_RegID(_T("	MODBUS_SENSOR21_STATE	"))	;
	MODBUS_SENSOR22_STATE	=	Get_RegID(_T("	MODBUS_SENSOR22_STATE	"))	;
	MODBUS_SENSOR23_STATE	=	Get_RegID(_T("	MODBUS_SENSOR23_STATE	"))	;
	MODBUS_SENSOR24_STATE	=	Get_RegID(_T("	MODBUS_SENSOR24_STATE	"))	;
	MODBUS_SENSOR25_STATE	=	Get_RegID(_T("	MODBUS_SENSOR25_STATE	"))	;
	MODBUS_SENSOR26_STATE	=	Get_RegID(_T("	MODBUS_SENSOR26_STATE	"))	;
	MODBUS_SENSOR27_STATE	=	Get_RegID(_T("	MODBUS_SENSOR27_STATE	"))	;
	MODBUS_SENSOR28_STATE	=	Get_RegID(_T("	MODBUS_SENSOR28_STATE	"))	;
	MODBUS_SENSOR29_STATE	=	Get_RegID(_T("	MODBUS_SENSOR29_STATE	"))	;
	MODBUS_SENSOR30_STATE	=	Get_RegID(_T("	MODBUS_SENSOR30_STATE	"))	;
	MODBUS_SENSOR31_STATE	=	Get_RegID(_T("	MODBUS_SENSOR31_STATE	"))	;
	MODBUS_SENSOR32_STATE	=	Get_RegID(_T("	MODBUS_SENSOR32_STATE	"))	;
	MODBUS_SENSOR33_STATE	=	Get_RegID(_T("	MODBUS_SENSOR33_STATE	"))	;
	MODBUS_SENSOR34_STATE	=	Get_RegID(_T("	MODBUS_SENSOR34_STATE	"))	;
	MODBUS_SENSOR35_STATE	=	Get_RegID(_T("	MODBUS_SENSOR35_STATE	"))	;
	MODBUS_SENSOR36_STATE	=	Get_RegID(_T("	MODBUS_SENSOR36_STATE	"))	;
	MODBUS_SENSOR37_STATE	=	Get_RegID(_T("	MODBUS_SENSOR37_STATE	"))	;
	MODBUS_SENSOR38_STATE	=	Get_RegID(_T("	MODBUS_SENSOR38_STATE	"))	;
	MODBUS_SENSOR39_STATE	=	Get_RegID(_T("	MODBUS_SENSOR39_STATE	"))	;
	MODBUS_SENSOR40_STATE	=	Get_RegID(_T("	MODBUS_SENSOR40_STATE	"))	;
	MODBUS_SENSOR41_STATE	=	Get_RegID(_T("	MODBUS_SENSOR41_STATE	"))	;
	MODBUS_SENSOR42_STATE	=	Get_RegID(_T("	MODBUS_SENSOR42_STATE	"))	;
	MODBUS_SENSOR43_STATE	=	Get_RegID(_T("	MODBUS_SENSOR43_STATE	"))	;
	MODBUS_SENSOR44_STATE	=	Get_RegID(_T("	MODBUS_SENSOR44_STATE	"))	;
	MODBUS_SENSOR45_STATE	=	Get_RegID(_T("	MODBUS_SENSOR45_STATE	"))	;
	MODBUS_SENSOR46_STATE	=	Get_RegID(_T("	MODBUS_SENSOR46_STATE	"))	;
	MODBUS_SENSOR47_STATE	=	Get_RegID(_T("	MODBUS_SENSOR47_STATE	"))	;
	MODBUS_SENSOR48_STATE	=	Get_RegID(_T("	MODBUS_SENSOR48_STATE	"))	;
	MODBUS_SENSOR49_STATE	=	Get_RegID(_T("	MODBUS_SENSOR49_STATE	"))	;
	MODBUS_SENSOR50_STATE	=	Get_RegID(_T("	MODBUS_SENSOR50_STATE	"))	;
	MODBUS_SENSOR51_STATE	=	Get_RegID(_T("	MODBUS_SENSOR51_STATE	"))	;
	MODBUS_SENSOR52_STATE	=	Get_RegID(_T("	MODBUS_SENSOR52_STATE	"))	;
	MODBUS_SENSOR53_STATE	=	Get_RegID(_T("	MODBUS_SENSOR53_STATE	"))	;
	MODBUS_SENSOR54_STATE	=	Get_RegID(_T("	MODBUS_SENSOR54_STATE	"))	;
	MODBUS_SENSOR55_STATE	=	Get_RegID(_T("	MODBUS_SENSOR55_STATE	"))	;
	MODBUS_SENSOR56_STATE	=	Get_RegID(_T("	MODBUS_SENSOR56_STATE	"))	;
	MODBUS_SENSOR57_STATE	=	Get_RegID(_T("	MODBUS_SENSOR57_STATE	"))	;
	MODBUS_SENSOR58_STATE	=	Get_RegID(_T("	MODBUS_SENSOR58_STATE	"))	;
	MODBUS_SENSOR59_STATE	=	Get_RegID(_T("	MODBUS_SENSOR59_STATE	"))	;
	MODBUS_SENSOR60_STATE	=	Get_RegID(_T("	MODBUS_SENSOR60_STATE	"))	;
	MODBUS_SENSOR61_STATE	=	Get_RegID(_T("	MODBUS_SENSOR61_STATE	"))	;
	MODBUS_SENSOR62_STATE	=	Get_RegID(_T("	MODBUS_SENSOR62_STATE	"))	;
	MODBUS_SENSOR63_STATE	=	Get_RegID(_T("	MODBUS_SENSOR63_STATE	"))	;
	MODBUS_SENSOR64_STATE	=	Get_RegID(_T("	MODBUS_SENSOR64_STATE	"))	;
	MODBUS_SENSOR65_STATE	=	Get_RegID(_T("	MODBUS_SENSOR65_STATE	"))	;
	MODBUS_SENSOR66_STATE	=	Get_RegID(_T("	MODBUS_SENSOR66_STATE	"))	;
	MODBUS_SENSOR67_STATE	=	Get_RegID(_T("	MODBUS_SENSOR67_STATE	"))	;
	MODBUS_SENSOR68_STATE	=	Get_RegID(_T("	MODBUS_SENSOR68_STATE	"))	;
	MODBUS_SENSOR69_STATE	=	Get_RegID(_T("	MODBUS_SENSOR69_STATE	"))	;
	MODBUS_SENSOR70_STATE	=	Get_RegID(_T("	MODBUS_SENSOR70_STATE	"))	;
	MODBUS_SENSOR71_STATE	=	Get_RegID(_T("	MODBUS_SENSOR71_STATE	"))	;
	MODBUS_SENSOR72_STATE	=	Get_RegID(_T("	MODBUS_SENSOR72_STATE	"))	;
	MODBUS_SENSOR73_STATE	=	Get_RegID(_T("	MODBUS_SENSOR73_STATE	"))	;
	MODBUS_SENSOR74_STATE	=	Get_RegID(_T("	MODBUS_SENSOR74_STATE	"))	;
	MODBUS_SENSOR75_STATE	=	Get_RegID(_T("	MODBUS_SENSOR75_STATE	"))	;
	MODBUS_SENSOR76_STATE	=	Get_RegID(_T("	MODBUS_SENSOR76_STATE	"))	;
	MODBUS_SENSOR77_STATE	=	Get_RegID(_T("	MODBUS_SENSOR77_STATE	"))	;
	MODBUS_SENSOR78_STATE	=	Get_RegID(_T("	MODBUS_SENSOR78_STATE	"))	;
	MODBUS_SENSOR79_STATE	=	Get_RegID(_T("	MODBUS_SENSOR79_STATE	"))	;
	MODBUS_SENSOR80_STATE	=	Get_RegID(_T("	MODBUS_SENSOR80_STATE	"))	;
	MODBUS_SENSOR81_STATE	=	Get_RegID(_T("	MODBUS_SENSOR81_STATE	"))	;
	MODBUS_SENSOR82_STATE	=	Get_RegID(_T("	MODBUS_SENSOR82_STATE	"))	;
	MODBUS_SENSOR83_STATE	=	Get_RegID(_T("	MODBUS_SENSOR83_STATE	"))	;
	MODBUS_SENSOR84_STATE	=	Get_RegID(_T("	MODBUS_SENSOR84_STATE	"))	;
	MODBUS_SENSOR85_STATE	=	Get_RegID(_T("	MODBUS_SENSOR85_STATE	"))	;
	MODBUS_SENSOR86_STATE	=	Get_RegID(_T("	MODBUS_SENSOR86_STATE	"))	;
	MODBUS_SENSOR87_STATE	=	Get_RegID(_T("	MODBUS_SENSOR87_STATE	"))	;
	MODBUS_SENSOR88_STATE	=	Get_RegID(_T("	MODBUS_SENSOR88_STATE	"))	;
	MODBUS_SENSOR89_STATE	=	Get_RegID(_T("	MODBUS_SENSOR89_STATE	"))	;
	MODBUS_SENSOR90_STATE	=	Get_RegID(_T("	MODBUS_SENSOR90_STATE	"))	;
	MODBUS_SENSOR91_STATE	=	Get_RegID(_T("	MODBUS_SENSOR91_STATE	"))	;
	MODBUS_SENSOR92_STATE	=	Get_RegID(_T("	MODBUS_SENSOR92_STATE	"))	;
	MODBUS_SENSOR93_STATE	=	Get_RegID(_T("	MODBUS_SENSOR93_STATE	"))	;
	MODBUS_SENSOR94_STATE	=	Get_RegID(_T("	MODBUS_SENSOR94_STATE	"))	;
	MODBUS_SENSOR95_STATE	=	Get_RegID(_T("	MODBUS_SENSOR95_STATE	"))	;
	MODBUS_SENSOR96_STATE	=	Get_RegID(_T("	MODBUS_SENSOR96_STATE	"))	;
	MODBUS_SENSOR97_STATE	=	Get_RegID(_T("	MODBUS_SENSOR97_STATE	"))	;
	MODBUS_SENSOR98_STATE	=	Get_RegID(_T("	MODBUS_SENSOR98_STATE	"))	;
	MODBUS_SENSOR99_STATE	=	Get_RegID(_T("	MODBUS_SENSOR99_STATE	"))	;
	MODBUS_SENSOR100_STATE	=	Get_RegID(_T("	MODBUS_SENSOR100_STATE	"))	;
	MODBUS_SENSOR101_STATE	=	Get_RegID(_T("	MODBUS_SENSOR101_STATE	"))	;
	MODBUS_SENSOR102_STATE	=	Get_RegID(_T("	MODBUS_SENSOR102_STATE	"))	;
	MODBUS_SENSOR103_STATE	=	Get_RegID(_T("	MODBUS_SENSOR103_STATE	"))	;
	MODBUS_SENSOR104_STATE 	=	Get_RegID(_T("	MODBUS_SENSOR104_STATE 	"))	;
	MODBUS_SENSOR1_TEMP     	=	Get_RegID(_T("	MODBUS_SENSOR1_TEMP     	"))	;
	MODBUS_SENSOR1_HUM	=	Get_RegID(_T("	MODBUS_SENSOR1_HUM	"))	;
	MODBUS_SENSOR2_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR2_TEMP    	"))	;
	MODBUS_SENSOR2_HUM	=	Get_RegID(_T("	MODBUS_SENSOR2_HUM	"))	;
	MODBUS_SENSOR3_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR3_TEMP    	"))	;
	MODBUS_SENSOR3_HUM	=	Get_RegID(_T("	MODBUS_SENSOR3_HUM	"))	;
	MODBUS_SENSOR4_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR4_TEMP    	"))	;
	MODBUS_SENSOR4_HUM	=	Get_RegID(_T("	MODBUS_SENSOR4_HUM	"))	;
	MODBUS_SENSOR5_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR5_TEMP    	"))	;
	MODBUS_SENSOR5_HUM	=	Get_RegID(_T("	MODBUS_SENSOR5_HUM	"))	;
	MODBUS_SENSOR6_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR6_TEMP    	"))	;
	MODBUS_SENSOR6_HUM	=	Get_RegID(_T("	MODBUS_SENSOR6_HUM	"))	;
	MODBUS_SENSOR7_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR7_TEMP    	"))	;
	MODBUS_SENSOR7_HUM	=	Get_RegID(_T("	MODBUS_SENSOR7_HUM	"))	;
	MODBUS_SENSOR8_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR8_TEMP    	"))	;
	MODBUS_SENSOR8_HUM	=	Get_RegID(_T("	MODBUS_SENSOR8_HUM	"))	;
	MODBUS_SENSOR9_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR9_TEMP    	"))	;
	MODBUS_SENSOR9_HUM	=	Get_RegID(_T("	MODBUS_SENSOR9_HUM	"))	;
	MODBUS_SENSOR10_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR10_TEMP    	"))	;
	MODBUS_SENSOR10_HUM	=	Get_RegID(_T("	MODBUS_SENSOR10_HUM	"))	;
	MODBUS_SENSOR11_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR11_TEMP    	"))	;
	MODBUS_SENSOR11_HUM	=	Get_RegID(_T("	MODBUS_SENSOR11_HUM	"))	;
	MODBUS_SENSOR12_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR12_TEMP    	"))	;
	MODBUS_SENSOR12_HUM	=	Get_RegID(_T("	MODBUS_SENSOR12_HUM	"))	;
	MODBUS_SENSOR13_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR13_TEMP    	"))	;
	MODBUS_SENSOR13_HUM	=	Get_RegID(_T("	MODBUS_SENSOR13_HUM	"))	;
	MODBUS_SENSOR14_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR14_TEMP    	"))	;
	MODBUS_SENSOR14_HUM	=	Get_RegID(_T("	MODBUS_SENSOR14_HUM	"))	;
	MODBUS_SENSOR15_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR15_TEMP    	"))	;
	MODBUS_SENSOR15_HUM	=	Get_RegID(_T("	MODBUS_SENSOR15_HUM	"))	;
	MODBUS_SENSOR16_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR16_TEMP    	"))	;
	MODBUS_SENSOR16_HUM	=	Get_RegID(_T("	MODBUS_SENSOR16_HUM	"))	;
	MODBUS_SENSOR17_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR17_TEMP    	"))	;
	MODBUS_SENSOR17_HUM	=	Get_RegID(_T("	MODBUS_SENSOR17_HUM	"))	;
	MODBUS_SENSOR18_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR18_TEMP    	"))	;
	MODBUS_SENSOR18_HUM	=	Get_RegID(_T("	MODBUS_SENSOR18_HUM	"))	;
	MODBUS_SENSOR19_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR19_TEMP    	"))	;
	MODBUS_SENSOR19_HUM	=	Get_RegID(_T("	MODBUS_SENSOR19_HUM	"))	;
	MODBUS_SENSOR20_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR20_TEMP    	"))	;
	MODBUS_SENSOR20_HUM	=	Get_RegID(_T("	MODBUS_SENSOR20_HUM	"))	;
	MODBUS_SENSOR21_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR21_TEMP    	"))	;
	MODBUS_SENSOR21_HUM	=	Get_RegID(_T("	MODBUS_SENSOR21_HUM	"))	;
	MODBUS_SENSOR22_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR22_TEMP    	"))	;
	MODBUS_SENSOR22_HUM	=	Get_RegID(_T("	MODBUS_SENSOR22_HUM	"))	;
	MODBUS_SENSOR23_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR23_TEMP    	"))	;
	MODBUS_SENSOR23_HUM	=	Get_RegID(_T("	MODBUS_SENSOR23_HUM	"))	;
	MODBUS_SENSOR24_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR24_TEMP    	"))	;
	MODBUS_SENSOR24_HUM	=	Get_RegID(_T("	MODBUS_SENSOR24_HUM	"))	;
	MODBUS_SENSOR25_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR25_TEMP    	"))	;
	MODBUS_SENSOR25_HUM	=	Get_RegID(_T("	MODBUS_SENSOR25_HUM	"))	;
	MODBUS_SENSOR26_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR26_TEMP    	"))	;
	MODBUS_SENSOR26_HUM	=	Get_RegID(_T("	MODBUS_SENSOR26_HUM	"))	;
	MODBUS_SENSOR27_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR27_TEMP    	"))	;
	MODBUS_SENSOR27_HUM	=	Get_RegID(_T("	MODBUS_SENSOR27_HUM	"))	;
	MODBUS_SENSOR28_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR28_TEMP    	"))	;
	MODBUS_SENSOR28_HUM	=	Get_RegID(_T("	MODBUS_SENSOR28_HUM	"))	;
	MODBUS_SENSOR29_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR29_TEMP    	"))	;
	MODBUS_SENSOR29_HUM	=	Get_RegID(_T("	MODBUS_SENSOR29_HUM	"))	;
	MODBUS_SENSOR30_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR30_TEMP    	"))	;
	MODBUS_SENSOR30_HUM	=	Get_RegID(_T("	MODBUS_SENSOR30_HUM	"))	;
	MODBUS_SENSOR31_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR31_TEMP    	"))	;
	MODBUS_SENSOR31_HUM	=	Get_RegID(_T("	MODBUS_SENSOR31_HUM	"))	;
	MODBUS_SENSOR32_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR32_TEMP    	"))	;
	MODBUS_SENSOR32_HUM	=	Get_RegID(_T("	MODBUS_SENSOR32_HUM	"))	;
	MODBUS_SENSOR33_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR33_TEMP    	"))	;
	MODBUS_SENSOR33_HUM	=	Get_RegID(_T("	MODBUS_SENSOR33_HUM	"))	;
	MODBUS_SENSOR34_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR34_TEMP    	"))	;
	MODBUS_SENSOR34_HUM	=	Get_RegID(_T("	MODBUS_SENSOR34_HUM	"))	;
	MODBUS_SENSOR35_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR35_TEMP    	"))	;
	MODBUS_SENSOR35_HUM	=	Get_RegID(_T("	MODBUS_SENSOR35_HUM	"))	;
	MODBUS_SENSOR36_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR36_TEMP    	"))	;
	MODBUS_SENSOR36_HUM	=	Get_RegID(_T("	MODBUS_SENSOR36_HUM	"))	;
	MODBUS_SENSOR37_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR37_TEMP    	"))	;
	MODBUS_SENSOR37_HUM	=	Get_RegID(_T("	MODBUS_SENSOR37_HUM	"))	;
	MODBUS_SENSOR38_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR38_TEMP    	"))	;
	MODBUS_SENSOR38_HUM	=	Get_RegID(_T("	MODBUS_SENSOR38_HUM	"))	;
	MODBUS_SENSOR39_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR39_TEMP    	"))	;
	MODBUS_SENSOR39_HUM	=	Get_RegID(_T("	MODBUS_SENSOR39_HUM	"))	;
	MODBUS_SENSOR40_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR40_TEMP    	"))	;
	MODBUS_SENSOR40_HUM	=	Get_RegID(_T("	MODBUS_SENSOR40_HUM	"))	;
	MODBUS_SENSOR41_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR41_TEMP    	"))	;
	MODBUS_SENSOR41_HUM	=	Get_RegID(_T("	MODBUS_SENSOR41_HUM	"))	;
	MODBUS_SENSOR42_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR42_TEMP    	"))	;
	MODBUS_SENSOR42_HUM	=	Get_RegID(_T("	MODBUS_SENSOR42_HUM	"))	;
	MODBUS_SENSOR43_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR43_TEMP    	"))	;
	MODBUS_SENSOR43_HUM	=	Get_RegID(_T("	MODBUS_SENSOR43_HUM	"))	;
	MODBUS_SENSOR44_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR44_TEMP    	"))	;
	MODBUS_SENSOR44_HUM	=	Get_RegID(_T("	MODBUS_SENSOR44_HUM	"))	;
	MODBUS_SENSOR45_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR45_TEMP    	"))	;
	MODBUS_SENSOR45_HUM	=	Get_RegID(_T("	MODBUS_SENSOR45_HUM	"))	;
	MODBUS_SENSOR46_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR46_TEMP    	"))	;
	MODBUS_SENSOR46_HUM	=	Get_RegID(_T("	MODBUS_SENSOR46_HUM	"))	;
	MODBUS_SENSOR47_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR47_TEMP    	"))	;
	MODBUS_SENSOR47_HUM	=	Get_RegID(_T("	MODBUS_SENSOR47_HUM	"))	;
	MODBUS_SENSOR48_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR48_TEMP    	"))	;
	MODBUS_SENSOR48_HUM	=	Get_RegID(_T("	MODBUS_SENSOR48_HUM	"))	;
	MODBUS_SENSOR49_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR49_TEMP    	"))	;
	MODBUS_SENSOR49_HUM	=	Get_RegID(_T("	MODBUS_SENSOR49_HUM	"))	;
	MODBUS_SENSOR50_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR50_TEMP    	"))	;
	MODBUS_SENSOR50_HUM	=	Get_RegID(_T("	MODBUS_SENSOR50_HUM	"))	;
	MODBUS_SENSOR51_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR51_TEMP    	"))	;
	MODBUS_SENSOR51_HUM	=	Get_RegID(_T("	MODBUS_SENSOR51_HUM	"))	;
	MODBUS_SENSOR52_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR52_TEMP    	"))	;
	MODBUS_SENSOR52_HUM	=	Get_RegID(_T("	MODBUS_SENSOR52_HUM	"))	;
	MODBUS_SENSOR53_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR53_TEMP    	"))	;
	MODBUS_SENSOR53_HUM	=	Get_RegID(_T("	MODBUS_SENSOR53_HUM	"))	;
	MODBUS_SENSOR54_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR54_TEMP    	"))	;
	MODBUS_SENSOR54_HUM	=	Get_RegID(_T("	MODBUS_SENSOR54_HUM	"))	;
	MODBUS_SENSOR55_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR55_TEMP    	"))	;
	MODBUS_SENSOR55_HUM	=	Get_RegID(_T("	MODBUS_SENSOR55_HUM	"))	;
	MODBUS_SENSOR56_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR56_TEMP    	"))	;
	MODBUS_SENSOR56_HUM	=	Get_RegID(_T("	MODBUS_SENSOR56_HUM	"))	;
	MODBUS_SENSOR57_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR57_TEMP    	"))	;
	MODBUS_SENSOR57_HUM	=	Get_RegID(_T("	MODBUS_SENSOR57_HUM	"))	;
	MODBUS_SENSOR58_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR58_TEMP    	"))	;
	MODBUS_SENSOR58_HUM	=	Get_RegID(_T("	MODBUS_SENSOR58_HUM	"))	;
	MODBUS_SENSOR59_TEMP	=	Get_RegID(_T("	MODBUS_SENSOR59_TEMP	"))	;
	MODBUS_SENSOR59_HUM	=	Get_RegID(_T("	MODBUS_SENSOR59_HUM	"))	;
	MODBUS_SENSOR60_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR60_TEMP    	"))	;
	MODBUS_SENSOR60_HUM	=	Get_RegID(_T("	MODBUS_SENSOR60_HUM	"))	;
	MODBUS_SENSOR61_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR61_TEMP    	"))	;
	MODBUS_SENSOR61_HUM	=	Get_RegID(_T("	MODBUS_SENSOR61_HUM	"))	;
	MODBUS_SENSOR62_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR62_TEMP    	"))	;
	MODBUS_SENSOR62_HUM	=	Get_RegID(_T("	MODBUS_SENSOR62_HUM	"))	;
	MODBUS_SENSOR63_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR63_TEMP    	"))	;
	MODBUS_SENSOR63_HUM	=	Get_RegID(_T("	MODBUS_SENSOR63_HUM	"))	;
	MODBUS_SENSOR64_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR64_TEMP    	"))	;
	MODBUS_SENSOR64_HUM	=	Get_RegID(_T("	MODBUS_SENSOR64_HUM	"))	;
	MODBUS_SENSOR65_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR65_TEMP    	"))	;
	MODBUS_SENSOR65_HUM	=	Get_RegID(_T("	MODBUS_SENSOR65_HUM	"))	;
	MODBUS_SENSOR66_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR66_TEMP    	"))	;
	MODBUS_SENSOR66_HUM	=	Get_RegID(_T("	MODBUS_SENSOR66_HUM	"))	;
	MODBUS_SENSOR67_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR67_TEMP    	"))	;
	MODBUS_SENSOR67_HUM	=	Get_RegID(_T("	MODBUS_SENSOR67_HUM	"))	;
	MODBUS_SENSOR68_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR68_TEMP    	"))	;
	MODBUS_SENSOR68_HUM	=	Get_RegID(_T("	MODBUS_SENSOR68_HUM	"))	;
	MODBUS_SENSOR69_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR69_TEMP    	"))	;
	MODBUS_SENSOR69_HUM	=	Get_RegID(_T("	MODBUS_SENSOR69_HUM	"))	;
	MODBUS_SENSOR70_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR70_TEMP    	"))	;
	MODBUS_SENSOR70_HUM	=	Get_RegID(_T("	MODBUS_SENSOR70_HUM	"))	;
	MODBUS_SENSOR71_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR71_TEMP    	"))	;
	MODBUS_SENSOR71_HUM	=	Get_RegID(_T("	MODBUS_SENSOR71_HUM	"))	;
	MODBUS_SENSOR72_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR72_TEMP    	"))	;
	MODBUS_SENSOR72_HUM	=	Get_RegID(_T("	MODBUS_SENSOR72_HUM	"))	;
	MODBUS_SENSOR73_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR73_TEMP    	"))	;
	MODBUS_SENSOR73_HUM	=	Get_RegID(_T("	MODBUS_SENSOR73_HUM	"))	;
	MODBUS_SENSOR74_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR74_TEMP    	"))	;
	MODBUS_SENSOR74_HUM	=	Get_RegID(_T("	MODBUS_SENSOR74_HUM	"))	;
	MODBUS_SENSOR75_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR75_TEMP    	"))	;
	MODBUS_SENSOR75_HUM	=	Get_RegID(_T("	MODBUS_SENSOR75_HUM	"))	;
	MODBUS_SENSOR76_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR76_TEMP    	"))	;
	MODBUS_SENSOR76_HUM	=	Get_RegID(_T("	MODBUS_SENSOR76_HUM	"))	;
	MODBUS_SENSOR77_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR77_TEMP    	"))	;
	MODBUS_SENSOR77_HUM	=	Get_RegID(_T("	MODBUS_SENSOR77_HUM	"))	;
	MODBUS_SENSOR78_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR78_TEMP    	"))	;
	MODBUS_SENSOR78_HUM	=	Get_RegID(_T("	MODBUS_SENSOR78_HUM	"))	;
	MODBUS_SENSOR79_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR79_TEMP    	"))	;
	MODBUS_SENSOR79_HUM	=	Get_RegID(_T("	MODBUS_SENSOR79_HUM	"))	;
	MODBUS_SENSOR80_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR80_TEMP    	"))	;
	MODBUS_SENSOR80_HUM	=	Get_RegID(_T("	MODBUS_SENSOR80_HUM	"))	;
	MODBUS_SENSOR81_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR81_TEMP    	"))	;
	MODBUS_SENSOR81_HUM	=	Get_RegID(_T("	MODBUS_SENSOR81_HUM	"))	;
	MODBUS_SENSOR82_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR82_TEMP    	"))	;
	MODBUS_SENSOR82_HUM	=	Get_RegID(_T("	MODBUS_SENSOR82_HUM	"))	;
	MODBUS_SENSOR83_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR83_TEMP    	"))	;
	MODBUS_SENSOR83_HUM	=	Get_RegID(_T("	MODBUS_SENSOR83_HUM	"))	;
	MODBUS_SENSOR84_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR84_TEMP    	"))	;
	MODBUS_SENSOR84_HUM	=	Get_RegID(_T("	MODBUS_SENSOR84_HUM	"))	;
	MODBUS_SENSOR85_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR85_TEMP    	"))	;
	MODBUS_SENSOR85_HUM	=	Get_RegID(_T("	MODBUS_SENSOR85_HUM	"))	;
	MODBUS_SENSOR86_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR86_TEMP    	"))	;
	MODBUS_SENSOR86_HUM	=	Get_RegID(_T("	MODBUS_SENSOR86_HUM	"))	;
	MODBUS_SENSOR87_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR87_TEMP    	"))	;
	MODBUS_SENSOR87_HUM	=	Get_RegID(_T("	MODBUS_SENSOR87_HUM	"))	;
	MODBUS_SENSOR88_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR88_TEMP    	"))	;
	MODBUS_SENSOR88_HUM	=	Get_RegID(_T("	MODBUS_SENSOR88_HUM	"))	;
	MODBUS_SENSOR89_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR89_TEMP    	"))	;
	MODBUS_SENSOR89_HUM	=	Get_RegID(_T("	MODBUS_SENSOR89_HUM	"))	;
	MODBUS_SENSOR90_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR90_TEMP    	"))	;
	MODBUS_SENSOR90_HUM	=	Get_RegID(_T("	MODBUS_SENSOR90_HUM	"))	;
	MODBUS_SENSOR91_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR91_TEMP    	"))	;
	MODBUS_SENSOR91_HUM	=	Get_RegID(_T("	MODBUS_SENSOR91_HUM	"))	;
	MODBUS_SENSOR92_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR92_TEMP    	"))	;
	MODBUS_SENSOR92_HUM	=	Get_RegID(_T("	MODBUS_SENSOR92_HUM	"))	;
	MODBUS_SENSOR93_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR93_TEMP    	"))	;
	MODBUS_SENSOR93_HUM	=	Get_RegID(_T("	MODBUS_SENSOR93_HUM	"))	;
	MODBUS_SENSOR94_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR94_TEMP    	"))	;
	MODBUS_SENSOR94_HUM	=	Get_RegID(_T("	MODBUS_SENSOR94_HUM	"))	;
	MODBUS_SENSOR95_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR95_TEMP    	"))	;
	MODBUS_SENSOR95_HUM	=	Get_RegID(_T("	MODBUS_SENSOR95_HUM	"))	;
	MODBUS_SENSOR96_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR96_TEMP    	"))	;
	MODBUS_SENSOR96_HUM	=	Get_RegID(_T("	MODBUS_SENSOR96_HUM	"))	;
	MODBUS_SENSOR97_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR97_TEMP    	"))	;
	MODBUS_SENSOR97_HUM	=	Get_RegID(_T("	MODBUS_SENSOR97_HUM	"))	;
	MODBUS_SENSOR98_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR98_TEMP    	"))	;
	MODBUS_SENSOR98_HUM	=	Get_RegID(_T("	MODBUS_SENSOR98_HUM	"))	;
	MODBUS_SENSOR99_TEMP    	=	Get_RegID(_T("	MODBUS_SENSOR99_TEMP    	"))	;
	MODBUS_SENSOR99_HUM	=	Get_RegID(_T("	MODBUS_SENSOR99_HUM	"))	;
	MODBUS_SENSOR100_TEMP   	=	Get_RegID(_T("	MODBUS_SENSOR100_TEMP   	"))	;
	MODBUS_SENSOR100_HUM	=	Get_RegID(_T("	MODBUS_SENSOR100_HUM	"))	;
	MODBUS_SENSOR101_TEMP   	=	Get_RegID(_T("	MODBUS_SENSOR101_TEMP   	"))	;
	MODBUS_SENSOR101_HUM	=	Get_RegID(_T("	MODBUS_SENSOR101_HUM	"))	;
	MODBUS_SENSOR102_TEMP   	=	Get_RegID(_T("	MODBUS_SENSOR102_TEMP   	"))	;
	MODBUS_SENSOR102_HUM	=	Get_RegID(_T("	MODBUS_SENSOR102_HUM	"))	;
	MODBUS_SENSOR103_TEMP   	=	Get_RegID(_T("	MODBUS_SENSOR103_TEMP   	"))	;
	MODBUS_SENSOR103_HUM	=	Get_RegID(_T("	MODBUS_SENSOR103_HUM	"))	;
	MODBUS_SENSOR104_TEMP   	=	Get_RegID(_T("	MODBUS_SENSOR104_TEMP   	"))	;
	MODBUS_SENSOR104_HUM   	=	Get_RegID(_T("	MODBUS_SENSOR104_HUM   	"))	;
	MODBUS_SENSOR1_SN	=	Get_RegID(_T("	MODBUS_SENSOR1_SN	"))	;
	MODBUS_SENSOR2_SN	=	Get_RegID(_T("	MODBUS_SENSOR2_SN	"))	;
	MODBUS_SENSOR3_SN	=	Get_RegID(_T("	MODBUS_SENSOR3_SN	"))	;
	MODBUS_SENSOR4_SN	=	Get_RegID(_T("	MODBUS_SENSOR4_SN	"))	;
	MODBUS_SENSOR5_SN	=	Get_RegID(_T("	MODBUS_SENSOR5_SN	"))	;
	MODBUS_SENSOR6_SN	=	Get_RegID(_T("	MODBUS_SENSOR6_SN	"))	;
	MODBUS_SENSOR7_SN	=	Get_RegID(_T("	MODBUS_SENSOR7_SN	"))	;
	MODBUS_SENSOR8_SN	=	Get_RegID(_T("	MODBUS_SENSOR8_SN	"))	;
	MODBUS_SENSOR9_SN	=	Get_RegID(_T("	MODBUS_SENSOR9_SN	"))	;
	MODBUS_SENSOR10_SN	=	Get_RegID(_T("	MODBUS_SENSOR10_SN	"))	;
	MODBUS_SENSOR11_SN	=	Get_RegID(_T("	MODBUS_SENSOR11_SN	"))	;
	MODBUS_SENSOR12_SN	=	Get_RegID(_T("	MODBUS_SENSOR12_SN	"))	;
	MODBUS_SENSOR13_SN	=	Get_RegID(_T("	MODBUS_SENSOR13_SN	"))	;
	MODBUS_SENSOR14_SN	=	Get_RegID(_T("	MODBUS_SENSOR14_SN	"))	;
	MODBUS_SENSOR15_SN	=	Get_RegID(_T("	MODBUS_SENSOR15_SN	"))	;
	MODBUS_SENSOR16_SN	=	Get_RegID(_T("	MODBUS_SENSOR16_SN	"))	;
	MODBUS_SENSOR17_SN	=	Get_RegID(_T("	MODBUS_SENSOR17_SN	"))	;
	MODBUS_SENSOR18_SN	=	Get_RegID(_T("	MODBUS_SENSOR18_SN	"))	;
	MODBUS_SENSOR19_SN	=	Get_RegID(_T("	MODBUS_SENSOR19_SN	"))	;
	MODBUS_SENSOR20_SN	=	Get_RegID(_T("	MODBUS_SENSOR20_SN	"))	;
	MODBUS_SENSOR21_SN	=	Get_RegID(_T("	MODBUS_SENSOR21_SN	"))	;
	MODBUS_SENSOR22_SN	=	Get_RegID(_T("	MODBUS_SENSOR22_SN	"))	;
	MODBUS_SENSOR23_SN	=	Get_RegID(_T("	MODBUS_SENSOR23_SN	"))	;
	MODBUS_SENSOR24_SN	=	Get_RegID(_T("	MODBUS_SENSOR24_SN	"))	;
	MODBUS_SENSOR25_SN	=	Get_RegID(_T("	MODBUS_SENSOR25_SN	"))	;
	MODBUS_SENSOR26_SN	=	Get_RegID(_T("	MODBUS_SENSOR26_SN	"))	;
	MODBUS_SENSOR27_SN	=	Get_RegID(_T("	MODBUS_SENSOR27_SN	"))	;
	MODBUS_SENSOR28_SN	=	Get_RegID(_T("	MODBUS_SENSOR28_SN	"))	;
	MODBUS_SENSOR29_SN	=	Get_RegID(_T("	MODBUS_SENSOR29_SN	"))	;
	MODBUS_SENSOR30_SN	=	Get_RegID(_T("	MODBUS_SENSOR30_SN	"))	;
	MODBUS_SENSOR31_SN	=	Get_RegID(_T("	MODBUS_SENSOR31_SN	"))	;
	MODBUS_SENSOR32_SN	=	Get_RegID(_T("	MODBUS_SENSOR32_SN	"))	;
	MODBUS_SENSOR33_SN	=	Get_RegID(_T("	MODBUS_SENSOR33_SN	"))	;
	MODBUS_SENSOR34_SN	=	Get_RegID(_T("	MODBUS_SENSOR34_SN	"))	;
	MODBUS_SENSOR35_SN	=	Get_RegID(_T("	MODBUS_SENSOR35_SN	"))	;
	MODBUS_SENSOR36_SN	=	Get_RegID(_T("	MODBUS_SENSOR36_SN	"))	;
	MODBUS_SENSOR37_SN	=	Get_RegID(_T("	MODBUS_SENSOR37_SN	"))	;
	MODBUS_SENSOR38_SN	=	Get_RegID(_T("	MODBUS_SENSOR38_SN	"))	;
	MODBUS_SENSOR39_SN	=	Get_RegID(_T("	MODBUS_SENSOR39_SN	"))	;
	MODBUS_SENSOR40_SN	=	Get_RegID(_T("	MODBUS_SENSOR40_SN	"))	;
	MODBUS_SENSOR41_SN	=	Get_RegID(_T("	MODBUS_SENSOR41_SN	"))	;
	MODBUS_SENSOR42_SN	=	Get_RegID(_T("	MODBUS_SENSOR42_SN	"))	;
	MODBUS_SENSOR43_SN	=	Get_RegID(_T("	MODBUS_SENSOR43_SN	"))	;
	MODBUS_SENSOR44_SN	=	Get_RegID(_T("	MODBUS_SENSOR44_SN	"))	;
	MODBUS_SENSOR45_SN	=	Get_RegID(_T("	MODBUS_SENSOR45_SN	"))	;
	MODBUS_SENSOR46_SN	=	Get_RegID(_T("	MODBUS_SENSOR46_SN	"))	;
	MODBUS_SENSOR47_SN	=	Get_RegID(_T("	MODBUS_SENSOR47_SN	"))	;
	MODBUS_SENSOR48_SN	=	Get_RegID(_T("	MODBUS_SENSOR48_SN	"))	;
	MODBUS_SENSOR49_SN	=	Get_RegID(_T("	MODBUS_SENSOR49_SN	"))	;
	MODBUS_SENSOR50_SN	=	Get_RegID(_T("	MODBUS_SENSOR50_SN	"))	;
	MODBUS_SENSOR51_SN	=	Get_RegID(_T("	MODBUS_SENSOR51_SN	"))	;
	MODBUS_SENSOR52_SN	=	Get_RegID(_T("	MODBUS_SENSOR52_SN	"))	;
	MODBUS_SENSOR53_SN	=	Get_RegID(_T("	MODBUS_SENSOR53_SN	"))	;
	MODBUS_SENSOR54_SN	=	Get_RegID(_T("	MODBUS_SENSOR54_SN	"))	;
	MODBUS_SENSOR55_SN	=	Get_RegID(_T("	MODBUS_SENSOR55_SN	"))	;
	MODBUS_SENSOR56_SN	=	Get_RegID(_T("	MODBUS_SENSOR56_SN	"))	;
	MODBUS_SENSOR57_SN	=	Get_RegID(_T("	MODBUS_SENSOR57_SN	"))	;
	MODBUS_SENSOR58_SN	=	Get_RegID(_T("	MODBUS_SENSOR58_SN	"))	;
	MODBUS_SENSOR59_SN	=	Get_RegID(_T("	MODBUS_SENSOR59_SN	"))	;
	MODBUS_SENSOR60_SN	=	Get_RegID(_T("	MODBUS_SENSOR60_SN	"))	;
	MODBUS_SENSOR61_SN	=	Get_RegID(_T("	MODBUS_SENSOR61_SN	"))	;
	MODBUS_SENSOR62_SN	=	Get_RegID(_T("	MODBUS_SENSOR62_SN	"))	;
	MODBUS_SENSOR63_SN	=	Get_RegID(_T("	MODBUS_SENSOR63_SN	"))	;
	MODBUS_SENSOR64_SN	=	Get_RegID(_T("	MODBUS_SENSOR64_SN	"))	;
	MODBUS_SENSOR65_SN	=	Get_RegID(_T("	MODBUS_SENSOR65_SN	"))	;
	MODBUS_SENSOR66_SN	=	Get_RegID(_T("	MODBUS_SENSOR66_SN	"))	;
	MODBUS_SENSOR67_SN	=	Get_RegID(_T("	MODBUS_SENSOR67_SN	"))	;
	MODBUS_SENSOR68_SN	=	Get_RegID(_T("	MODBUS_SENSOR68_SN	"))	;
	MODBUS_SENSOR69_SN	=	Get_RegID(_T("	MODBUS_SENSOR69_SN	"))	;
	MODBUS_SENSOR70_SN 	=	Get_RegID(_T("	MODBUS_SENSOR70_SN 	"))	;
	MODBUS_SENSOR71_SN	=	Get_RegID(_T("	MODBUS_SENSOR71_SN	"))	;
	MODBUS_SENSOR72_SN	=	Get_RegID(_T("	MODBUS_SENSOR72_SN	"))	;
	MODBUS_SENSOR73_SN	=	Get_RegID(_T("	MODBUS_SENSOR73_SN	"))	;
	MODBUS_SENSOR74_SN	=	Get_RegID(_T("	MODBUS_SENSOR74_SN	"))	;
	MODBUS_SENSOR75_SN	=	Get_RegID(_T("	MODBUS_SENSOR75_SN	"))	;
	MODBUS_SENSOR76_SN	=	Get_RegID(_T("	MODBUS_SENSOR76_SN	"))	;
	MODBUS_SENSOR77_SN	=	Get_RegID(_T("	MODBUS_SENSOR77_SN	"))	;
	MODBUS_SENSOR78_SN	=	Get_RegID(_T("	MODBUS_SENSOR78_SN	"))	;
	MODBUS_SENSOR79_SN	=	Get_RegID(_T("	MODBUS_SENSOR79_SN	"))	;
	MODBUS_SENSOR80_SN	=	Get_RegID(_T("	MODBUS_SENSOR80_SN	"))	;
	MODBUS_SENSOR81_SN	=	Get_RegID(_T("	MODBUS_SENSOR81_SN	"))	;
	MODBUS_SENSOR82_SN	=	Get_RegID(_T("	MODBUS_SENSOR82_SN	"))	;
	MODBUS_SENSOR83_SN	=	Get_RegID(_T("	MODBUS_SENSOR83_SN	"))	;
	MODBUS_SENSOR84_SN	=	Get_RegID(_T("	MODBUS_SENSOR84_SN	"))	;
	MODBUS_SENSOR85_SN	=	Get_RegID(_T("	MODBUS_SENSOR85_SN	"))	;
	MODBUS_SENSOR86_SN	=	Get_RegID(_T("	MODBUS_SENSOR86_SN	"))	;
	MODBUS_SENSOR87_SN	=	Get_RegID(_T("	MODBUS_SENSOR87_SN	"))	;
	MODBUS_SENSOR88_SN	=	Get_RegID(_T("	MODBUS_SENSOR88_SN	"))	;
	MODBUS_SENSOR89_SN	=	Get_RegID(_T("	MODBUS_SENSOR89_SN	"))	;
	MODBUS_SENSOR90_SN	=	Get_RegID(_T("	MODBUS_SENSOR90_SN	"))	;
	MODBUS_SENSOR91_SN	=	Get_RegID(_T("	MODBUS_SENSOR91_SN	"))	;
	MODBUS_SENSOR92_SN	=	Get_RegID(_T("	MODBUS_SENSOR92_SN	"))	;
	MODBUS_SENSOR93_SN	=	Get_RegID(_T("	MODBUS_SENSOR93_SN	"))	;
	MODBUS_SENSOR94_SN	=	Get_RegID(_T("	MODBUS_SENSOR94_SN	"))	;
	MODBUS_SENSOR95_SN	=	Get_RegID(_T("	MODBUS_SENSOR95_SN	"))	;
	MODBUS_SENSOR96_SN	=	Get_RegID(_T("	MODBUS_SENSOR96_SN	"))	;
	MODBUS_SENSOR97_SN	=	Get_RegID(_T("	MODBUS_SENSOR97_SN	"))	;
	MODBUS_SENSOR98_SN	=	Get_RegID(_T("	MODBUS_SENSOR98_SN	"))	;
	MODBUS_SENSOR99_SN	=	Get_RegID(_T("	MODBUS_SENSOR99_SN	"))	;
	MODBUS_SENSOR100_SN	=	Get_RegID(_T("	MODBUS_SENSOR100_SN	"))	;
	MODBUS_SENSOR101_SN	=	Get_RegID(_T("	MODBUS_SENSOR101_SN	"))	;
	MODBUS_SENSOR102_SN	=	Get_RegID(_T("	MODBUS_SENSOR102_SN	"))	;
	MODBUS_SENSOR103_SN	=	Get_RegID(_T("	MODBUS_SENSOR103_SN	"))	;
	MODBUS_SENSOR104_SN	=	Get_RegID(_T("	MODBUS_SENSOR104_SN	"))	;
	MODBUS_CLEAR_TABLE_MANUAL	=	Get_RegID(_T("	MODBUS_CLEAR_TABLE_MANUAL	"))	;
	MODBUS_CALIBRATE_MANUAL	=	Get_RegID(_T("	MODBUS_CALIBRATE_MANUAL	"))	;
	MODBUS_FRE_MANUAL	=	Get_RegID(_T("	MODBUS_FRE_MANUAL	"))	;
	MODBUS_HUM_MANUAL 	=	Get_RegID(_T("	MODBUS_HUM_MANUAL 	"))	;
	MODBUS_RESULT2  	=	Get_RegID(_T("	MODBUS_RESULT2  	"))	;
	MODBUS_TEST0	=	Get_RegID(_T("	MODBUS_TEST0	"))	;
	MODBUS_TEST1	=	Get_RegID(_T("	MODBUS_TEST1	"))	;
	MODBUS_TEST2	=	Get_RegID(_T("	MODBUS_TEST2	"))	;
	MODBUS_TEST3	=	Get_RegID(_T("	MODBUS_TEST3	"))	;
	MODBUS_TEST4	=	Get_RegID(_T("	MODBUS_TEST4	"))	;
	MODBUS_TEST5	=	Get_RegID(_T("	MODBUS_TEST5	"))	;
	MODBUS_TEST6	=	Get_RegID(_T("	MODBUS_TEST6	"))	;
	MODBUS_FRE_TEMP0	=	Get_RegID(_T("	MODBUS_FRE_TEMP0	"))	;
	MODBUS_FRE_TEMP1	=	Get_RegID(_T("	MODBUS_FRE_TEMP1	"))	;
	MODBUS_FRE_TEMP2	=	Get_RegID(_T("	MODBUS_FRE_TEMP2	"))	;
	MODBUS_FRE_TEMP3	=	Get_RegID(_T("	MODBUS_FRE_TEMP3	"))	;
	MODBUS_FRE_TEMP4	=	Get_RegID(_T("	MODBUS_FRE_TEMP4	"))	;
	MODBUS_FRE_TEMP5	=	Get_RegID(_T("	MODBUS_FRE_TEMP5	"))	;
	MODBUS_FRE_TEMP6	=	Get_RegID(_T("	MODBUS_FRE_TEMP6	"))	;
	MODBUS_FRE_TEMP7	=	Get_RegID(_T("	MODBUS_FRE_TEMP7	"))	;
	MODBUS_FRE_TEMP8	=	Get_RegID(_T("	MODBUS_FRE_TEMP8	"))	;
	MODBUS_FRE_TEMP9	=	Get_RegID(_T("	MODBUS_FRE_TEMP9	"))	;
	MODBUS_HUM_TEMP0	=	Get_RegID(_T("	MODBUS_HUM_TEMP0	"))	;
	MODBUS_HUM_TEMP1	=	Get_RegID(_T("	MODBUS_HUM_TEMP1	"))	;
	MODBUS_HUM_TEMP2	=	Get_RegID(_T("	MODBUS_HUM_TEMP2	"))	;
	MODBUS_HUM_TEMP3	=	Get_RegID(_T("	MODBUS_HUM_TEMP3	"))	;
	MODBUS_HUM_TEMP4	=	Get_RegID(_T("	MODBUS_HUM_TEMP4	"))	;
	MODBUS_HUM_TEMP5	=	Get_RegID(_T("	MODBUS_HUM_TEMP5	"))	;
	MODBUS_HUM_TEMP6	=	Get_RegID(_T("	MODBUS_HUM_TEMP6	"))	;
	MODBUS_HUM_TEMP7	=	Get_RegID(_T("	MODBUS_HUM_TEMP7	"))	;
	MODBUS_HUM_TEMP8	=	Get_RegID(_T("	MODBUS_HUM_TEMP8	"))	;
	MODBUS_HUM_TEMP9	=	Get_RegID(_T("	MODBUS_HUM_TEMP9	"))	;
	MODBUS_REGISTER_END	=	Get_RegID(_T("	MODBUS_REGISTER_END	"))	;
#endif
}
int  CNewHumChamberView::Get_RegID(CString Name)
{
	Name.TrimLeft();
	Name.TrimRight();
	int regid=-1;
	vector<T3Register>::iterator iter;
	for (iter=m_vecT3Register.begin();iter!=m_vecT3Register.end();iter++)
	{
		if (iter->regName==Name)
		{
			regid=iter->regID;
			break;
		}
	}
	return regid;
}
void CNewHumChamberView::Main_infor(){
CString StrTemp;
StrTemp.Format(_T("%d"),product_register_value[MODBUS_ADDRESS]);
m_edit_master_id.SetWindowTextW(StrTemp);
int IntTemp=product_register_value[MODBUS_SENSOR_TYPE];
if (IntTemp<=0)
{
m_combox_type.SetCurSel(0);
}
else if (IntTemp>=2)
{
m_combox_type.SetCurSel(2);
}
else
{
m_combox_type.SetCurSel(1);
}
StrTemp.Format(_T("%d"),product_register_value[MODBUS_SENSOR_TOTAL]);
m_edit_num_sensor.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_ERROR_SENSOR_NUM]);
m_edit_error_sensors.SetWindowTextW(StrTemp);

StrTemp.Format(_T("%d ppm"),product_register_value[MODBUS_TESTO_CO2]);
m_edit_co2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f %%"),(float)(short)product_register_value[MODBUS_TESTO_HUM]/10.0);
m_edit_hum.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f C"),(float)(short)product_register_value[MODBUS_TESTO_T]/10.0);
m_edit_temp.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d Pa"),product_register_value[MODBUS_TESTO_P]);
m_edit_presure.SetWindowTextW(StrTemp);

}
void CNewHumChamberView::Calibration_Points(){
CString StrTemp;


StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING0]/10.0);
m_edit_temp1.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING1]/10.0);
m_edit_temp2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING2]/10.0);
m_edit_temp3.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING3]/10.0);
m_edit_temp4.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING4]/10.0);
m_edit_temp5.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING5]/10.0);
m_edit_temp6.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING6]/10.0);
m_edit_temp7.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING7]/10.0);
m_edit_temp8.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING8]/10.0);
m_edit_temp9.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEMP_SETTING9]/10.0);
m_edit_temp10.SetWindowTextW(StrTemp);

StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING0]/10.0);
m_edit_hum1.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING1]/10.0);
m_edit_hum2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING2]/10.0);
m_edit_hum3.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING3]/10.0);
m_edit_hum4.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING4]/10.0);
m_edit_hum5.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING5]/10.0);
m_edit_hum6.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING6]/10.0);
m_edit_hum7.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING7]/10.0);
m_edit_hum8.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING8]/10.0);
m_edit_hum9.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_SETTING9]/10.0);
m_edit_hum10.SetWindowTextW(StrTemp);

StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING0]);
m_edit_co21.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING1]);
m_edit_co22.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING2]);
m_edit_co23.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING3]);
m_edit_co24.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING4]);
m_edit_co25.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING5]);
m_edit_co26.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING6]);
m_edit_co27.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING7]);
m_edit_co28.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING8]);
m_edit_co29.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_SETTING9]);
m_edit_co210.SetWindowTextW(StrTemp);

StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING0]);
m_edit_time1.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING1]);
m_edit_time2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING2]);
m_edit_time3.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING3]);
m_edit_time4.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING4]);
m_edit_time5.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING5]);
m_edit_time6.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING6]);
m_edit_time7.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING7]);
m_edit_time8.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING8]);
m_edit_time9.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING9]);
m_edit_time10.SetWindowTextW(StrTemp);


StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING0]);
m_edit_time1.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING1]);
m_edit_time2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING2]);
m_edit_time3.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING3]);
m_edit_time4.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING4]);
m_edit_time5.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING5]);
m_edit_time6.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING6]);
m_edit_time7.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING7]);
m_edit_time8.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING8]);
m_edit_time9.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_SETTING9]);
m_edit_time10.SetWindowTextW(StrTemp);

 
Time_left=product_register_value[MODBUS_TIME_LEFT];
Hour=Time_left/3600;
Minute=(Time_left-Hour*3600)/60;
Second=Time_left%60;
StrTemp.Format(_T("%d:%d:%d"),Hour,Minute,Second);
CString StrDefault=_T("0:0:0");
switch(product_register_value[MODBUS_POINT_NUM])
{
case 1:
	{
		m_edit_time_left1.SetWindowTextW(StrTemp);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 2:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrTemp);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 3:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrTemp);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 4:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrTemp);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 5:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrTemp);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 6:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrTemp);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 7:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrTemp);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 8:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrTemp);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 9:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrTemp);
		m_edit_time_left10.SetWindowTextW(StrDefault);
	}break;
case 10:
	{
		m_edit_time_left1.SetWindowTextW(StrDefault);
		m_edit_time_left2.SetWindowTextW(StrDefault);
		m_edit_time_left3.SetWindowTextW(StrDefault);
		m_edit_time_left4.SetWindowTextW(StrDefault);
		m_edit_time_left5.SetWindowTextW(StrDefault);
		m_edit_time_left6.SetWindowTextW(StrDefault);
		m_edit_time_left7.SetWindowTextW(StrDefault);
		m_edit_time_left8.SetWindowTextW(StrDefault);
		m_edit_time_left9.SetWindowTextW(StrDefault);
		m_edit_time_left10.SetWindowTextW(StrTemp);
	}break;
default:
	break;}
//Check
BOOL Checked;
int IntTemp=product_register_value[MODBUS_POINTS_BITS_SET];
Checked=Get_Bit_FromRegister(IntTemp,1);
if (Checked)
{
m_check1.SetCheck(1);
GetDlgItem(IDC_TEMP1)->EnableWindow(TRUE);
GetDlgItem(IDC_HUM1)->EnableWindow(TRUE);
GetDlgItem(IDC_TIME1)->EnableWindow(TRUE);
GetDlgItem(IDC_CO21)->EnableWindow(TRUE);
} 
else
{
	GetDlgItem(IDC_TEMP1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP1)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM1)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME1)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO21)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO21)->EnableWindow(FALSE);

m_check1.SetCheck(0);
}
Checked=Get_Bit_FromRegister(IntTemp,2);
if (Checked)
{
	m_check2.SetCheck(1);
	GetDlgItem(IDC_TEMP2)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM2)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME2)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO22)->EnableWindow(TRUE);
} 
else
{
	m_check2.SetCheck(0);
	GetDlgItem(IDC_TEMP2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP2)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM2)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME2)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO22)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO22)->EnableWindow(FALSE);

}
Checked=Get_Bit_FromRegister(IntTemp,3);
if (Checked)
{
	m_check3.SetCheck(1);
	GetDlgItem(IDC_TEMP3)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM3)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME3)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO23)->EnableWindow(TRUE);
} 
else
{
	m_check3.SetCheck(0);
	GetDlgItem(IDC_TEMP3)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP3)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM3)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM3)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME3)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME3)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO23)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO23)->EnableWindow(FALSE);

}
Checked=Get_Bit_FromRegister(IntTemp,4);
if (Checked)
{
	m_check4.SetCheck(1);
	GetDlgItem(IDC_TEMP4)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM4)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME4)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO24)->EnableWindow(TRUE);
} 
else
{
	m_check4.SetCheck(0);
	GetDlgItem(IDC_TEMP4)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP4)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM4)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM4)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME4)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME4)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO24)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO24)->EnableWindow(FALSE);
}
Checked=Get_Bit_FromRegister(IntTemp,5);
if (Checked)
{
	m_check5.SetCheck(1);
	GetDlgItem(IDC_TEMP5)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM5)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME5)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO25)->EnableWindow(TRUE);
} 
else
{
	m_check5.SetCheck(0);
	GetDlgItem(IDC_TEMP5)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP5)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM5)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM5)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME5)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME5)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO25)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO25)->EnableWindow(FALSE);
}
Checked=Get_Bit_FromRegister(IntTemp,6);
if (Checked)
{
	m_check6.SetCheck(1);
	GetDlgItem(IDC_TEMP6)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM6)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME6)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO26)->EnableWindow(TRUE);
} 
else
{
	m_check6.SetCheck(0);
	GetDlgItem(IDC_TEMP6)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP6)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM6)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM6)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME6)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME6)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO26)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO26)->EnableWindow(FALSE);


}
Checked=Get_Bit_FromRegister(IntTemp,7);
if (Checked)
{
	m_check7.SetCheck(1);
	GetDlgItem(IDC_TEMP7)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM7)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME7)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO27)->EnableWindow(TRUE);
} 
else
{
	m_check7.SetCheck(0);
	GetDlgItem(IDC_TEMP7)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP7)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM7)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM7)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME7)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME7)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO27)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO27)->EnableWindow(FALSE);

}
Checked=Get_Bit_FromRegister(IntTemp,8);
if (Checked)
{
	m_check8.SetCheck(1);
	GetDlgItem(IDC_TEMP8)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM8)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME8)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO28)->EnableWindow(TRUE);
} 
else
{
	m_check8.SetCheck(0);
	GetDlgItem(IDC_TEMP8)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP8)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM8)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM8)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME8)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME8)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO28)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO28)->EnableWindow(FALSE);

}
Checked=Get_Bit_FromRegister(IntTemp,9);
if (Checked)
{
	m_check9.SetCheck(1);
	GetDlgItem(IDC_TEMP9)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM9)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME9)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO29)->EnableWindow(TRUE);
} 
else
{
	m_check9.SetCheck(0);
	GetDlgItem(IDC_TEMP9)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP9)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM9)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM9)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME9)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME9)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO29)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO29)->EnableWindow(FALSE);

}
Checked=Get_Bit_FromRegister(IntTemp,10);
if (Checked)
{
	m_check10.SetCheck(1);
	GetDlgItem(IDC_TEMP10)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM10)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME10)->EnableWindow(TRUE);
	GetDlgItem(IDC_CO20)->EnableWindow(TRUE);
} 
else
{
	m_check10.SetCheck(0);
	GetDlgItem(IDC_TEMP10)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP10)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM10)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM10)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME10)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME10)->EnableWindow(FALSE);

	GetDlgItem(IDC_CO20)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CO20)->EnableWindow(FALSE);

}

}

void CNewHumChamberView::Fresh_CalibartionPoints(){

     CString Status_Calibartion=_T("");

	 if (product_register_value[MODBUS_SENSOR_TYPE]==2)
	 {	
		 unsigned short current_sensor=product_register_value[MODBUS_POINT_NUM];
		 if (product_register_value[MODBUS_CALIBRATE_STATE]==254)
		 {
			 Status_Calibartion=_T("                      Ready");
			 m_Static_TestStatus.SetWindowTextW(Status_Calibartion);
			 m_Start=FALSE;
		 }
		 else if (product_register_value[MODBUS_CALIBRATE_STATE]==255)
		 {
			 Status_Calibartion=_T("                      Finished");
			 m_Static_TestStatus.SetWindowTextW(Status_Calibartion);
			 m_Start=FALSE;
		 }
		 else if (product_register_value[MODBUS_CALIBRATE_STATE]==4)
		 {
			 m_Start=TRUE;

			 Status_Calibartion.Format(_T("The current sensor is %d,Temp=%0.1f C,Hum=%0.1f%%"),current_sensor,(float)(short)product_register_value[MODBUS_TEMP_SETTING0+current_sensor-1]/10.0,(float)(short)product_register_value[MODBUS_HUM_SETTING0+current_sensor-1]/10.0);
			 m_Static_TestStatus.SetWindowTextW(Status_Calibartion);
		 }
		 else  if (product_register_value[MODBUS_CALIBRATE_STATE]==5)
		 {
			 m_Start=TRUE;


			 Status_Calibartion.Format(_T("The current sensor is %d,Temp=%0.1f C,Hum=%0.1f%%"),current_sensor,(float)(short)product_register_value[MODBUS_TEMP_SETTING0+current_sensor-1]/10.0,(float)(short)product_register_value[MODBUS_HUM_SETTING0+current_sensor-1]/10.0);
			 m_Static_TestStatus.SetWindowTextW(Status_Calibartion);
		 }
		 else
		 {
			 m_Start=TRUE;


			 Status_Calibartion.Format(_T("The current sensor is %d,Temp=%0.1f C,Hum=%0.1f%%"),current_sensor,(float)(short)product_register_value[MODBUS_TEMP_SETTING0+current_sensor-1]/10.0,(float)(short)product_register_value[MODBUS_HUM_SETTING0+current_sensor-1]/10.0);
			 m_Static_TestStatus.SetWindowTextW(Status_Calibartion);
		 }
	 
// 		 CString StrTemp;
// 		 Time_left=product_register_value[MODBUS_TIME_LEFT];
// 		 Hour=Time_left/3600;
// 		 Minute=(Time_left-Hour*3600)/60;
// 		 Second=Time_left%60;
// 		 StrTemp.Format(_T("%d:%d:%d"),Hour,Minute,Second);
// 
// 		 CString StrDefault=_T("0:0:0");
// 		 switch(current_sensor)
// 		 {
// 		 case 1:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrTemp);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 
// 
// 
// 
// 			 }break;
// 		 case 2:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrTemp);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 3:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrTemp);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 4:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrTemp);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 5:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrTemp);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 6:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrTemp);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 7:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrTemp);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 8:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrTemp);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 9:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrTemp);
// 				 m_edit_time_left10.SetWindowTextW(StrDefault);
// 			 }break;
// 		 case 10:
// 			 {
// 				 m_edit_time_left1.SetWindowTextW(StrDefault);
// 				 m_edit_time_left2.SetWindowTextW(StrDefault);
// 				 m_edit_time_left3.SetWindowTextW(StrDefault);
// 				 m_edit_time_left4.SetWindowTextW(StrDefault);
// 				 m_edit_time_left5.SetWindowTextW(StrDefault);
// 				 m_edit_time_left6.SetWindowTextW(StrDefault);
// 				 m_edit_time_left7.SetWindowTextW(StrDefault);
// 				 m_edit_time_left8.SetWindowTextW(StrDefault);
// 				 m_edit_time_left9.SetWindowTextW(StrDefault);
// 				 m_edit_time_left10.SetWindowTextW(StrTemp);
// 			 }break;
// 		 default:
// 			 break;}

		 ////Check
		 //BOOL Checked;
		 //int IntTemp=product_register_value[MODBUS_POINTS_BITS_SET];
		 //Checked=Get_Bit_FromRegister(IntTemp,1);
		 //if (Checked)
			// {

			//	 GetDlgItem(IDC_TEMP1)->EnableWindow(TRUE);
			//	 GetDlgItem(IDC_HUM1)->EnableWindow(TRUE);
			//	 GetDlgItem(IDC_TIME1)->EnableWindow(TRUE);
			//	 GetDlgItem(IDC_CO21)->EnableWindow(TRUE);
			// } 
		 //else
			// {
			//	 GetDlgItem(IDC_TEMP1)->SetWindowText(_T("0"));
			//	 GetDlgItem(IDC_TEMP1)->EnableWindow(FALSE);
			//	 GetDlgItem(IDC_HUM1)->SetWindowText(_T("0"));
			//	 GetDlgItem(IDC_HUM1)->EnableWindow(FALSE);
			//	 GetDlgItem(IDC_TIME1)->SetWindowText(_T("0"));
			//	 GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(_T("0:0:0"));
			//	 GetDlgItem(IDC_TIME1)->EnableWindow(FALSE);

			//	 GetDlgItem(IDC_CO21)->SetWindowText(_T("0"));
			//	 GetDlgItem(IDC_CO21)->EnableWindow(FALSE);


			// }
		 //Checked=Get_Bit_FromRegister(IntTemp,2);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP2)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM2)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME2)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO22)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP2)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP2)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM2)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM2)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME2)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME2)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO22)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO22)->EnableWindow(FALSE);

		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,3);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP3)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM3)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME3)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO23)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP3)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP3)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM3)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM3)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME3)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME3)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO23)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO23)->EnableWindow(FALSE);

		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,4);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP4)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM4)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME4)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO24)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP4)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP4)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM4)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM4)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME4)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME4)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO24)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO24)->EnableWindow(FALSE);
		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,5);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP5)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM5)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME5)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO25)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP5)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP5)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM5)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM5)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME5)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME5)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO25)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO25)->EnableWindow(FALSE);
		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,6);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP6)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM6)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME6)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO26)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP6)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP6)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM6)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM6)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME6)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME6)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO26)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO26)->EnableWindow(FALSE);


		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,7);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP7)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM7)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME7)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO27)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP7)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP7)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM7)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM7)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME7)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME7)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO27)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO27)->EnableWindow(FALSE);

		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,8);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP8)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM8)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME8)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO28)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP8)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP8)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM8)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM8)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME8)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME8)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO28)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO28)->EnableWindow(FALSE);

		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,9);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP9)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM9)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME9)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO29)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP9)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP9)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM9)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM9)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME9)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME9)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO29)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO29)->EnableWindow(FALSE);

		 //}
		 //Checked=Get_Bit_FromRegister(IntTemp,10);
		 //if (Checked)
		 //{

			// GetDlgItem(IDC_TEMP10)->EnableWindow(TRUE);
			// GetDlgItem(IDC_HUM10)->EnableWindow(TRUE);
			// GetDlgItem(IDC_TIME10)->EnableWindow(TRUE);
			// GetDlgItem(IDC_CO20)->EnableWindow(TRUE);
		 //} 
		 //else
		 //{

			// GetDlgItem(IDC_TEMP10)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP10)->EnableWindow(FALSE);
			// GetDlgItem(IDC_HUM10)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_HUM10)->EnableWindow(FALSE);
			// GetDlgItem(IDC_TIME10)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(_T("0:0:0"));
			// GetDlgItem(IDC_TIME10)->EnableWindow(FALSE);

			// GetDlgItem(IDC_CO20)->SetWindowText(_T("0"));
			// GetDlgItem(IDC_CO20)->EnableWindow(FALSE);

		 //}
	 } 
	


	
	 


}

void CNewHumChamberView::Sensor_Reading_Table(){
int IntTemp=product_register_value[MODBUS_SENSOR_TOTAL];
if (IntTemp>=1&&IntTemp<=104)
{
     m_msflexgrid.put_Rows(IntTemp+1);

	 CString temp,temp1;
	 for (int i=1;i<=IntTemp;i++)
	 {

		 temp.Format(_T("%d"),i);
		 m_msflexgrid.put_TextMatrix(i,0,temp);
		 if (m_SN[MODBUS_SENSOR1_SN+i-1-MODBUS_SENSOR1_SN]!=65535)
		 {
			 temp.Format(_T("%d"),m_SN[MODBUS_SENSOR1_SN+i-1-MODBUS_SENSOR1_SN]);
			 m_msflexgrid.put_TextMatrix(i,1,temp);
		 } 


		 switch (product_register_value[MODBUS_SENSOR1_STATE +i-1])
		 {
		 case 0:
			 {
				 temp1=_T("Not Tested");
				 break;
			 }
		 case 1:
			 {
				 temp1=_T("OK");
				 break;
			 }
		 case 2:
			 {
				 temp1=_T("Bad Hum");
				 break;
			 }
		 case 3:
			 {
				 temp1=_T("Bad Temp");
				 break;
			 }
		 case 4:
			 {
				 temp1=_T("Bad Communications");
				 break;
			 }
		 case 5:
			 {
				 temp1=_T("Testing");
				 break;
			 }
		 default:
			 {
				 temp1=_T("Not Tested");
				 break;
			 }
		 }
		 m_msflexgrid.put_TextMatrix(i,2,temp1);

		 temp.Format(_T("%0.1f"),(product_register_value[MODBUS_SENSOR1_TEMP+2*(i-1)]/10.0));
		 m_msflexgrid.put_TextMatrix(i,3,temp);
		 temp.Format(_T("%0.1f"),(product_register_value[MODBUS_SENSOR1_TEMP+1+2*(i-1)]/10.0));
		 m_msflexgrid.put_TextMatrix(i,4,temp);
	 } 


}


}

void CNewHumChamberView::Sensor_Table(){
CString StrTemp;
StrTemp.Format(_T("Sensor=%d"),product_register_value[MODBUS_SENSOR_NUM]);
m_cur_sensor.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H]);
m_edit_freq1.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+2]);
m_edit_freq2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+4]);
m_edit_freq3.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+6]);
m_edit_freq4.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+8]);
m_edit_freq5.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+10]);
m_edit_freq6.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+12]);
m_edit_freq7.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+14]);
m_edit_freq8.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+16]);
m_edit_freq9.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMRH1_H+18]);
m_edit_freq10.SetWindowTextW(StrTemp);

StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H]);
m_edit_rh1.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+2]);
m_edit_rh2.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+4]);
m_edit_rh3.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+6]);
m_edit_rh4.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+8]);
m_edit_rh5.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+10]);
m_edit_rh6.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+12]);
m_edit_rh7.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+14]);
m_edit_rh8.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+16]);
m_edit_rh9.SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+18]);
m_edit_rh10.SetWindowTextW(StrTemp);
 

}
BEGIN_EVENTSINK_MAP(CNewHumChamberView, CFormView)
	ON_EVENT(CNewHumChamberView, IDC_MSFLEXGRID_INPUT3, DISPID_CLICK, CNewHumChamberView::ClickMsflexgridInput3, VTS_NONE)
	ON_EVENT(CNewHumChamberView, IDC_MSFLEXGRID_INPUT3, DISPID_DBLCLICK, CNewHumChamberView::DblClickMsflexgridInput3, VTS_NONE)
END_EVENTSINK_MAP()


 
void CNewHumChamberView::ClickMsflexgridInput3()
{
	long lRow,lCol;
	lRow = m_msflexgrid.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid.get_ColSel(); //获取点击的列号
	if(lRow==0)
		return;
	CRect rect;
	m_msflexgrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;



	//if (m_nCurCol==4)
	//{
	//	Set(628,_wtof(strValue)*10);
	//}
	//if (m_nCurCol==3)
	//{
	//	Set(627,_wtof(strValue)*10);
	//}
	//if (m_nCurCol==1)
	//{
	//	//m_btnflash.ShowWindow(SW_SHOW);
	//	//m_btnflash.SetWindowText(_T("Flash")); //显示文本
	//	//m_btnflash.MoveWindow(rc); //移动到选中格的位置，覆盖
	//}
	//if (m_nCurCol==0)
	//{
	//	UINT temp=m_nCurRow;
	//	CString str_text;
	//	int ret=WriteValueToRegID(CurrentTestSensor.Start_ID,temp);

	//	if (!ret)
	//	{
	//		AfxMessageBox(_T("Write Fail"));
	//	}
	//	str_text.Format(_T("%d"),(product_register_value[CurrentTestSensor.Start_ID])); 
	//	GetDlgItem(IDC_SENSOR_ID)->SetWindowText(str_text);

	//	Sleep(2000);
	//	ret=Read_Multi(g_tstat_id,&product_register_value[633],633,20);
	//	if (ret<0)
	//	{
	//		AfxMessageBox(_T("Reading Fail"));
	//	}
	//	else
	//	{
	//		for (int row=1;row<m_msflexgrid.get_Rows();row++)
	//		{
	//			for (int i=0;i<m_msflexgrid.get_Cols();i++)
	//			{
	//				m_msflexgrid.put_Row(row);
	//				m_msflexgrid.put_Col(i);
	//				m_msflexgrid.put_CellBackColor(RGB(255,255,255));
	//			}
	//		}
	//		for (int i=0;i<m_msflexgrid.get_Cols();i++)
	//		{
	//			m_msflexgrid.put_Row(m_nCurRow);
	//			m_msflexgrid.put_Col(i);
	//			m_msflexgrid.put_CellBackColor(RGB(50,205,50));
	//		}
	//	}
	//	Update_SensorTable();



	//	AfxMessageBox(_T("Update Complete"));
	//}




}

void CNewHumChamberView::OnBnClickedButtonStart()
{
    UpdateData();
	if (m_isstart)
	{

		close_testo();
		m_isstart=false;
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("Start"));
		GetDlgItem(IDC_COMBO_USB_TESTO)->EnableWindow(TRUE);
	}
	else{

        /*   g_vectRegisters.clear();
        Registers_Infor Reg_Infor_Temp;
        Reg_Infor_Temp.Point_Color = RGB(0,0,255);
        Reg_Infor_Temp.Reg_Name = _T("Temperature");
        g_vectRegisters.push_back(Reg_Infor_Temp); 
        Reg_Infor_Temp.Point_Color = RGB(255,0,0);
        Reg_Infor_Temp.Reg_Name = _T("Humidity");
        g_vectRegisters.push_back(Reg_Infor_Temp); */

		start();
		if (m_isstart)
		{
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("Stop"));
			GetDlgItem(IDC_COMBO_USB_TESTO)->EnableWindow(FALSE);
		}
		 
	}
}
void CNewHumChamberView::start(){
	CString Str_USB;
	GetDlgItem(IDC_COMBO_USB_TESTO)->GetWindowText(Str_USB);


	if (Open_Testo_USB(Str_USB)==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("Can't connect to testo device"));
		m_isstart=false;
		return;
	}
	/*int ret_n = Open_Socket2(_T("192.168.0.113"),10000);
	if(ret_n < 0)
	{
		AfxMessageBox(_T("Can't connect to T3-BB/LB/TB"));
		m_isstart=false;
		return;
	}*/
	for (int i=0;i<4;i++)
	{
		m_value[i] = 0;
	}
	m_isstart=true;
	if (m_pFreshMultiRegisters==NULL)
	{
	   m_pFreshMultiRegisters = AfxBeginThread(_Read_Testo,this);
	}
	
}
extern bool has_change_connect_ip;
void CNewHumChamberView::read_testo(){
   // Sleep(1000);
	for (int i=0;i<4;i++)
	{
		m_value[i] = 0;
	}
	short temp_value[4];
	if (m_IsReadIniFile)
	{
		temp_value[0] = GetPrivateProfileInt(_T("EnhancedRegister"), _T("VALUE_0"), 0, save_data_ini_file_path);
		temp_value[1] = GetPrivateProfileInt(_T("EnhancedRegister"), _T("VALUE_1"), 0, save_data_ini_file_path);
		temp_value[2] = GetPrivateProfileInt(_T("EnhancedRegister"), _T("VALUE_2"), 0, save_data_ini_file_path);
		Sleep(500);
	} 
	else
	{
		ReadTestoDeviceData(m_value);
		temp_value[0] = temp_value[1] = temp_value[2] = temp_value[3] = 0;
		temp_value[0] = (short)m_value[0];
		temp_value[3] = (short)m_value[1];
		temp_value[1] = (short)(m_value[3] * 10);    //湿度
		temp_value[2] = (short)(m_value[2] * 10);	//温度


	 
		 	CString key_word_temp; CString temp_value_cstring;
		 
			temp_value_cstring.Format(_T("%d"), temp_value[0]); 
			WritePrivateProfileStringW(_T("EnhancedRegister"), L"VALUE_0", temp_value_cstring, save_data_ini_file_path);
			temp_value_cstring.Format(_T("%d"), temp_value[2]);
			WritePrivateProfileStringW(_T("EnhancedRegister"), L"VALUE_1", temp_value_cstring, save_data_ini_file_path);
			temp_value_cstring.Format(_T("%d"), temp_value[1]);
			WritePrivateProfileStringW(_T("EnhancedRegister"), L"VALUE_2", temp_value_cstring, save_data_ini_file_path);
		 
	}
	
	//if (m_value[0]<=1.0)
	//{
	//  return;
	//}
	//unsigned short temp_value[4];
	//temp_value[0] = temp_value[1]=temp_value[2]=temp_value[3]=0;
	//temp_value[0]=(unsigned short)m_value[0];
	//temp_value[3]=(unsigned short)m_value[1];
	//temp_value[1]=(unsigned short)(m_value[3]*10);  //湿度
	//temp_value[2]=(unsigned short)(m_value[2]*10);	//温度

	
	

    if(  (m_value[2] == m_value[3])  && (m_value[2] == m_value[4] )&& (m_value[2] == m_value[1]))
    {
        return;
    }

	 
		
		
		
	//}

    CString   strlog;
    strlog.Format(_T("PPM %0.1f  ,CO2 = %0.1f   ,Temp = %0.1f     ,Hum =  %0.1f"),m_value[0],m_value[1],m_value[3],m_value[2]);
    write_T3000_log_file(strlog); 


	if(use_minipanel_controller == 1)	//1:enable         2: disable
	{
		int Value_HUM, Value_TEMP;
		short Hum_high, Hum_low;
		short Temp_high, Temp_low;
		Value_HUM = (int)temp_value[1] * 100;
		Value_TEMP = (int)temp_value[2] * 100;

		Hum_high = Value_HUM / 65535;
		 Hum_low = Value_HUM % 65535;

		 Temp_high = Value_TEMP / 65535;
		 Temp_low = Value_TEMP % 65535;
		if(has_change_connect_ip)
		{
			int ret_n = Open_Socket2(HUM_Minipanel_IP,HUM_Minipanel_port);
			if(ret_n > 0)
			{
				has_change_connect_ip = false;
				int last_com_type = GetLastCommunicationType();
				SetCommunicationType(1);
				unsigned short DataWrite[2];
				DataWrite[0] = Hum_high;
				DataWrite[1] = Hum_low;
				int ret1 = 
				Write_Multi_org_short(HUM_Minipanel_modbus_id, DataWrite, HUM_WRITE_REG1, 2);

				DataWrite[0] = Temp_high;
				DataWrite[1] = Temp_low;
				ret1 =
					Write_Multi_org_short(HUM_Minipanel_modbus_id, DataWrite, HUM_WRITE_REG2, 2);
				     

				    SetCommunicationType(last_com_type);
			}
		}
		else
		{
			int last_com_type = GetLastCommunicationType();
			SetCommunicationType(1);
			 int ret1,ret2,ret3,ret4;
			 
			 unsigned short DataWrite[2];
			 DataWrite[0] = Hum_high;
			 DataWrite[1] = Hum_low;
			 ret1 =
				 Write_Multi_org_short(HUM_Minipanel_modbus_id, DataWrite, HUM_WRITE_REG1, 2);

			 DataWrite[0] = Temp_high;
			 DataWrite[1] = Temp_low;
			 ret1 =
				 Write_Multi_org_short(HUM_Minipanel_modbus_id, DataWrite, HUM_WRITE_REG2, 2);

			if((ret1 < 0) && (ret2 < 0))
			{
				Open_Socket2(HUM_Minipanel_IP,HUM_Minipanel_port);
			}
			SetCommunicationType(last_com_type);
		}
	}


	m_times+=1;
	if (is_connect())
	{
 /*	register_critical_section.Lock();*/
 /*Sleep(10000);*/
 /*int ret = 1;*/
	g_tstat_id=m_tstatID;
 	int ret=Write_Multi_org_short(m_tstatID,(unsigned short *)temp_value,MODBUS_TESTO_CO2,4);
 	/*register_critical_section.Unlock();*/
 	if (ret>0)
 	{
 	  m_Write_times+=1;
 	}
	} 
	 
	//Sleep(1000);

	
}


LRESULT CNewHumChamberView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message==WM_FRESH_TESTO)
	{
	
	    CString  StrTemp;
		StrTemp.Format(_T("%0.0f ppm"),m_value[0]);
		m_edit_co2.SetWindowTextW(StrTemp);
		StrTemp.Format(_T("%0.0f Pa"),m_value[1]);
		m_edit_presure.SetWindowTextW(StrTemp);
		StrTemp.Format(_T("%0.1f C"),m_value[2]);
		m_edit_temp.SetWindowTextW(StrTemp);
		StrTemp.Format(_T("%0.1f %%"),m_value[3]);
		m_edit_hum.SetWindowTextW(StrTemp);
		StrTemp.Format(_T("Read Testo Times=%0.0f,Write Times=%0.0f"),m_times,m_Write_times);
		m_libel_times.SetWindowTextW(StrTemp);
		
		return 1;
	}
	else if (message==WM_FRESH_HUM_CHAMBER)
	{
	
	   Fresh_CalibartionPoints();
	   return 1;
	}
	else if (message==WM_FRESH_CO2)
	{
	
	   Fresh_CO2_Time();
	  return 1; 
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


 


void CNewHumChamberView::OnDestroy()
{
	g_bPauseMultiRead = FALSE;
 
	if (hFirstThread!=NULL)
	{
		if (WaitForSingleObject(hFirstThread->m_hThread, 1000) != WAIT_OBJECT_0)

			//Sleep(500);//wait for the end of the thread.
			if (hFirstThread!=NULL) 

			{
				if (WaitForSingleObject(hFirstThread->m_hThread, 3000) == WAIT_OBJECT_0)
				{

				}
				else
				{		
					BOOL bRet = TerminateThread(hFirstThread->m_hThread,0);

					hFirstThread=NULL;
				}

			}
	}

	if (m_pFreshMultiRegisters!=NULL)
	{
		if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 1000) != WAIT_OBJECT_0)

			//Sleep(500);//wait for the end of the thread.
			if (m_pFreshMultiRegisters!=NULL) 

			{
				if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 3000) == WAIT_OBJECT_0)
				{

				}
				else
				{		
					BOOL bRet = TerminateThread(m_pFreshMultiRegisters->m_hThread,0);

					m_pFreshMultiRegisters=NULL;
				}

			}
	}
	close_testo();
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}

unsigned short CNewHumChamberView::Get_Checks(){
	unsigned short Reg_Value=0;

	if (m_check10.GetCheck()!=0)
	{
		Reg_Value+=1<<9;
	}
	if (m_check9.GetCheck()!=0)
	{
		Reg_Value+=1<<8;
	}
	if (m_check8.GetCheck()!=0)
	{
		Reg_Value+=1<<7;
	}
	if (m_check7.GetCheck()!=0)
	{
		Reg_Value+=1<<6;
	}
	if (m_check6.GetCheck()!=0)
	{
		Reg_Value+=1<<5;
	}
	if (m_check5.GetCheck()!=0)
	{
		Reg_Value+=1<<4;
	}
	if (m_check4.GetCheck()!=0)
	{
		Reg_Value+=1<<3;
	}
	if (m_check3.GetCheck()!=0)
	{
		Reg_Value+=1<<2;
	}
	if (m_check2.GetCheck()!=0)
	{
		Reg_Value+=1<<1;
	}
	if (m_check1.GetCheck()!=0)
	{
		Reg_Value+=1;
	}
	return Reg_Value;
}
void CNewHumChamberView::OnBnClickedCheck1()
{
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
	/*register_critical_section.Unlock();*/
	if (ret<0)
	{
		if (m_check1.GetCheck()==0)
		{
			m_check1.SetCheck(0);
		}
		else
		{
			m_check1.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnBnClickedCheck2()
{
		unsigned short VLV=Get_Checks();
		/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	  register_critical_section.Unlock();*/
	if (ret<0)
	{
		if (m_check2.GetCheck()==0)
		{
			m_check2.SetCheck(0);
		}
		else
		{
			m_check2.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck3()
{
	
	 
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	register_critical_section.Unlock();*/
	if (ret<0)
	{
		if (m_check3.GetCheck()==0)
		{
			m_check3.SetCheck(0);
		}
		else
		{
			m_check3.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck4()
{
	
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check4.GetCheck()==0)
		{
			m_check4.SetCheck(0);
		}
		else
		{
			m_check4.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck5()
{
	
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check5.GetCheck()==0)
		{
			m_check5.SetCheck(0);
		}
		else
		{
			m_check5.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck6()
{
	
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
	/*register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check6.GetCheck()==0)
		{
			m_check6.SetCheck(0);
		}
		else
		{
			m_check6.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck7()
{
	
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
	/*register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check7.GetCheck()==0)
		{
			m_check7.SetCheck(0);
		}
		else
		{
			m_check7.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck8()
{
	
	unsigned short VLV=Get_Checks();
/*	register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check8.GetCheck()==0)
		{
			m_check8.SetCheck(0);
		}
		else
		{
			m_check8.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck9()
{
	
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check9.GetCheck()==0)
		{
			m_check9.SetCheck(0);
		}
		else
		{
			m_check9.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


void CNewHumChamberView::OnClickedCheck10()
{
	
	unsigned short VLV=Get_Checks();
	/*register_critical_section.Lock();*/
	int ret= write_one(m_tstatID,MODBUS_POINTS_BITS_SET,VLV);
/*	register_critical_section.Unlock();*/

	if (ret<0)
	{
		if (m_check10.GetCheck()==0)
		{
			m_check10.SetCheck(0);
		}
		else
		{
			m_check10.SetCheck(1);
		}

		AfxMessageBox(_T("Try again!"));
		return;
	} 
	product_register_value[MODBUS_POINTS_BITS_SET]=VLV;
	Calibration_Points();
}


//void CNewHumChamberView::OnKillfocusTemp1()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp1()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP1)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING0,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}

void CNewHumChamberView::Set(int id,int value){
	m_ID=id;
	m_Value=value;
}
//void CNewHumChamberView::OnKillfocusTemp10()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp10()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP10)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING9,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp2()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp2()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP2)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING1,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp3()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp3()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP3)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING2,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp4()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp4()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP4)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING3,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp5()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp5()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP5)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING4,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp6()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp6()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP6)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING5,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp7()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp7()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP7)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING6,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


//void CNewHumChamberView::OnKillfocusTemp8()
//{
//	
//}


void CNewHumChamberView::OnSetfocusTemp8()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP8)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING7,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}





void CNewHumChamberView::OnSetfocusTemp9()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TEMP9)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)temp_float*10;
	Set(MODBUS_TEMP_SETTING8,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


BOOL CNewHumChamberView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{

		if(pMsg->wParam == VK_F5)
		{
			if (is_Show_Write_singleDLG)
			{
				CWriteSingleRegDlg m_writesingleregdlg;
				m_writesingleregdlg.Set(m_ID,m_Value); 
				m_writesingleregdlg.DoModal();

				Main_infor();
				Calibration_Points();
				Sensor_Reading_Table();
				Sensor_Table();
				//FreshGrid();
				is_Show_Write_singleDLG=FALSE;
			}

			return 1;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}


void CNewHumChamberView::OnSetfocusHum1()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM1)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING0,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum10()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM10)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING9,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


 
void CNewHumChamberView::OnSetfocusHum2()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM2)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING1,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum3()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM3)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING2,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum4()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM4)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING3,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum5()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM5)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING4,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum6()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM6)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING5,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum7()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM7)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING6,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum8()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM8)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING7,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusHum9()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_HUM9)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float*10);
	Set(MODBUS_HUM_SETTING8,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime1()
{
	// TODO: Add your control notification handler code here
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME1)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING0,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime10()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME10)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING9,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime2()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME2)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING1,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime3()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME3)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING2,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime4()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME4)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING3,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime5()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME5)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING4,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime6()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME6)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING5,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime7()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME7)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING6,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime8()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME8)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING7,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusTime9()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_TIME9)->GetWindowText(str_text);
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_TIMER_SETTING8,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnBnClickedRefresh()
{
  	if (is_connect())
  	{ 
      /* register_critical_section.Lock();*/

        
		for(int  i=0;i<5;i++)
		{
			Read_Multi(m_tstatID,&product_register_value[MODBUS_SENSOR1_STATE +i*100],MODBUS_SENSOR1_STATE +i*100,100);
		 
		}
	 	/*register_critical_section.Unlock();*/
		  Sensor_Reading_Table();
 	}
 	else
 	{
 		AfxMessageBox(_T("Disconnect"));
 	}
}


void CNewHumChamberView::DblClickMsflexgridInput3()
{
	long lRow,lCol;
	lRow = m_msflexgrid.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid.get_ColSel(); //获取点击的列号
	if(lRow==0)
		return;
	CRect rect;
	m_msflexgrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol; 
	if (is_connect())
	{
		if (m_nCurCol==0)
		{
			UINT temp=m_nCurRow;
			CString str_text;
		
			  int ret =write_one(m_tstatID,MODBUS_REFRESH_SENSOR,0);
			 
			  if (ret<0)
			  {
				  AfxMessageBox(_T("Write Fail"));
				  return;
			  }
			
			  ret=write_one(m_tstatID,MODBUS_SENSOR_NUM,temp);
			
			if (ret<0)
			{
				AfxMessageBox(_T("Write Fail"));
				return;
			}
			product_register_value[MODBUS_SENSOR_NUM]=temp;
 
            int times=10;
			/*Sleep(2000);*/
			int  flash=Read_One(m_tstatID,MODBUS_REFRESH_SENSOR);
			while ((flash!=1)&&(times>0))
			{
			    flash=Read_One(m_tstatID,MODBUS_REFRESH_SENSOR);
				if (flash==1)
				{
				 break;
				}
				--times;
			}
			if (flash!=1)
			{
			AfxMessageBox(_T("Time Out!"));
			return;
			}
			ret=Read_Multi(m_tstatID,&product_register_value[MODBUS_HUMCOUNT1_H],MODBUS_HUMCOUNT1_H ,20);
			
            if (ret<0)
			{
				AfxMessageBox(_T("Reading Fail"));
			}
			 Sensor_Table();

			 for (int row=1;row<m_msflexgrid.get_Rows();row++)
			 {
				 for (int i=0;i<m_msflexgrid.get_Cols();i++)
				 {
					 m_msflexgrid.put_Row(row);
					 m_msflexgrid.put_Col(i);
					 m_msflexgrid.put_CellBackColor(RGB(255,255,255));
				 }
			 }
			 for (int i=0;i<m_msflexgrid.get_Cols();i++)
			 {
				 m_msflexgrid.put_Row(m_nCurRow);
				 m_msflexgrid.put_Col(i);
				 m_msflexgrid.put_CellBackColor(RGB(50,205,50));
			 }

			AfxMessageBox(_T("Update Complete"));
			return;
		} 
		 
	}
	else
	{
		AfxMessageBox(_T("Disconnect"));
	}
}


void CNewHumChamberView::OnBnClickedBatchFlash()
{
	m_CheckSkip.EnableWindow(FALSE);
	m_batch_btn.EnableWindow(FALSE);
	int skip;
	CString temp1;
	unsigned short RegAdd;
	unsigned long SN;
	skip=m_CheckSkip.GetCheck();

	CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
	pDlg->Create(IDD_DIALOG10_Progress, this);
	pDlg->ShowProgress(0,0);
	pDlg->ShowWindow(SW_SHOW);
	RECT RECT_SET1;
	GetClientRect(&RECT_SET1);
	pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);
	float progress;
	int totalsensors=product_register_value[MODBUS_SENSOR_TOTAL];
	for (int i=1;i<=totalsensors;i++)
	{
		if (product_register_value[MODBUS_SENSOR1_STATE+i-1]!=1)
		{

			continue;

		}


		if (pDlg!=NULL)
		{
			progress=float((i+1)*(100/totalsensors));
			pDlg->ShowProgress(int(progress),(int)progress);
		} 

		RegAdd=MODBUS_SENSOR1_SN+i-1;

		if (skip==1)
		{
			if (product_register_value[RegAdd]==65535||product_register_value[RegAdd]==0)//无序列号,就才写入
			{
				SN=GetNewSerialNumber();
                WriteSerialNumber(SN);
				int ret=write_one(m_tstatID,RegAdd,SN,5);
				if (ret>0)
				{
					
					product_register_value[RegAdd]=SN;
					temp1.Format(_T("%d"),product_register_value[RegAdd]);
					m_msflexgrid.put_TextMatrix(i,1,temp1);
				} 
				else
				{
					temp1=_T("Write Fail");

					m_msflexgrid.put_TextMatrix(i,1,temp1); 
				}

			} 
		} 
		else
		{
			SN=GetNewSerialNumber();
            WriteSerialNumber(SN);
			int ret=write_one(m_tstatID,RegAdd,SN,5);
			if (ret>0)
			{
				
				product_register_value[RegAdd]=SN;
				temp1.Format(_T("%d"),product_register_value[RegAdd]);
				m_msflexgrid.put_TextMatrix(i,1,temp1);
			} 
			else
			{
				temp1=_T("Write Fail");
				//   temp1.Format(_T("%d"),product_register_value[RegAdd]);
				m_msflexgrid.put_TextMatrix(i,1,temp1);

			}

		}

	}
	pDlg->ShowWindow(SW_HIDE);
	if(pDlg!=NULL)
	{delete pDlg;
	pDlg=NULL;}
	m_CheckSkip.EnableWindow(TRUE);
	m_batch_btn.EnableWindow(TRUE);
}
unsigned long CNewHumChamberView::GetNewSerialNumber(){
	int nSerialNumber;		
	CStdioFile file;
	CString filepath;
	filepath=g_strExePth+_T("Sensor Serial Record.txt");
	if(!file.Open(filepath, CFile::shareDenyNone|CFile::modeReadWrite))
	{

		return -1;
	}

	//----------------------------get the serialnumber--------------------------------------
	///////////////////// read a line from file to the lastest line
	CString strLine,strTemp;	


	while(file.ReadString(strTemp))
	{
		strLine = strTemp;		
	}	
	strTemp = strLine.Mid(0, strLine.Find(_T(',')));
	nSerialNumber = _wtoi(strTemp);//the serialnumber
	file.Close();
	return nSerialNumber+1;
}
void CNewHumChamberView::WriteSerialNumber(unsigned long SN){
	CStdioFile file;
	CString filepath;
	CString SerialDate;
	CTime time = CTime::GetCurrentTime();

	SerialDate.Format(_T("\n%d,%d-%d-%d  %d:%d:%d"),SN,time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());

	filepath=g_strExePth+_T("Sensor Serial Record.txt");
	if(!file.Open(filepath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return ;
	}

	file.SeekToEnd();

	file.WriteString(SerialDate.GetBuffer());
	//  file.Flush();
	file.Close();
}



void CNewHumChamberView::OnBnClickedStart()
{
	int ret=write_one(m_tstatID,MODBUS_CALIBRATE_STATE,2);
	if (ret>0)
	{
		m_Start=TRUE;
 		m_StartBtn.ShowWindow(FALSE);
 		m_StopBtn.ShowWindow(TRUE);
 		m_ContinueBtn.ShowWindow(FALSE);

	}
}


void CNewHumChamberView::OnBnClickedStop()
{
	int ret=write_one(m_tstatID,MODBUS_CALIBRATE_STATE,0);
	if (ret>0)
	{
		m_Start=FALSE;
 		m_StartBtn.ShowWindow(TRUE);
 		m_StopBtn.ShowWindow(FALSE);
 		m_ContinueBtn.ShowWindow(TRUE);
	}
}


void CNewHumChamberView::OnBnClickedContinue()
{
	int ret= write_one(m_tstatID,MODBUS_CALIBRATE_STATE,1);
	if (ret>0)
	{
		m_Start=TRUE;
 		m_StartBtn.ShowWindow(FALSE);
 		m_StopBtn.ShowWindow(TRUE);
 		m_ContinueBtn.ShowWindow(FALSE);
	}
}


 
 


void CNewHumChamberView::OnEnSetfocusNumSensor()
{
	float temp_float;
	CString str_text;
	unsigned short Temp_UINT;
	GetDlgItem(IDC_NUM_SENSOR)->GetWindowText(str_text);

	temp_float=(float)_wtof(str_text);
	Temp_UINT=(UINT)temp_float;

	m_ID=MODBUS_SENSOR_TOTAL;
	m_Value=Temp_UINT;
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnCbnDropdownComboUsbTesto()
{CStringArray EnumUSB;
CString temp;
	Enum_USB_Device(EnumUSB);
	m_combox_testo.ResetContent();
	for (int i=0;i<EnumUSB.GetSize();i++)
	{
		temp=EnumUSB.GetAt(i);
		m_combox_testo.AddString(temp);
	}
	m_combox_testo.SetCurSel(0);
}


void CNewHumChamberView::OnCbnSelchangeComboType()
{ 
	  int sel=m_combox_type.GetCurSel();
	  int ret=write_one(m_tstatID,MODBUS_SENSOR_TYPE,sel);
	  if (ret>0)
	  {
	  product_register_value[MODBUS_SENSOR_TYPE]=sel;
	  }
	  else
	  {
	  AfxMessageBox(_T("Try again!"));
	  }
	  FreshUI();
}

void CNewHumChamberView::FreshUI(){
if (product_register_value[MODBUS_SENSOR_TYPE]==2)//HUM
{
	m_check1.ShowWindow(TRUE);
	m_check10.ShowWindow(TRUE);
	m_check2.ShowWindow(TRUE);
	m_check3.ShowWindow(TRUE);
	m_check4.ShowWindow(TRUE);
	m_check5.ShowWindow(TRUE);
	m_check6.ShowWindow(TRUE);
	m_check7.ShowWindow(TRUE);
	m_check8.ShowWindow(TRUE);
	m_check9.ShowWindow(TRUE);
	m_edit_temp1.ShowWindow(TRUE);
	m_edit_temp10.ShowWindow(TRUE);
	m_edit_temp2.ShowWindow(TRUE);
	m_edit_temp3.ShowWindow(TRUE);
	m_edit_temp4.ShowWindow(TRUE);
	m_edit_temp5.ShowWindow(TRUE);
	m_edit_temp6.ShowWindow(TRUE);
	m_edit_temp7.ShowWindow(TRUE);
	m_edit_temp8.ShowWindow(TRUE);
	m_edit_temp9.ShowWindow(TRUE);
	m_edit_time1.ShowWindow(TRUE);
	m_edit_time10.ShowWindow(TRUE);
	m_edit_time2.ShowWindow(TRUE);
	m_edit_time3.ShowWindow(TRUE);
	m_edit_time4.ShowWindow(TRUE);
	m_edit_time5.ShowWindow(TRUE);
	m_edit_time6.ShowWindow(TRUE);
	m_edit_time7.ShowWindow(TRUE);
	m_edit_time8.ShowWindow(TRUE);
	m_edit_time9.ShowWindow(TRUE);
	m_edit_time_left1.ShowWindow(TRUE);
	m_edit_time_left10.ShowWindow(TRUE);
	m_edit_time_left2.ShowWindow(TRUE);
	m_edit_time_left3.ShowWindow(TRUE);
	m_edit_time_left4.ShowWindow(TRUE);
	m_edit_time_left5.ShowWindow(TRUE);
	m_edit_time_left6.ShowWindow(TRUE);
	m_edit_time_left7.ShowWindow(TRUE);
	m_edit_time_left8.ShowWindow(TRUE);
	m_edit_time_left9.ShowWindow(TRUE);
	m_msflexgrid.ShowWindow(TRUE);
	 m_CheckSkip.ShowWindow(TRUE);
	 m_batch_btn.ShowWindow(TRUE);
	 GetDlgItem(IDC_REFRESH_HUMBER_CHAMBER)->ShowWindow(TRUE);
	 GetDlgItem(IDC_CALIBRATION_POINT_REPORT)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_SENSOR_TABLE)->ShowWindow(TRUE);
	 m_cur_sensor.ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_FREQ)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_RH)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_1)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_2)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_3)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_4)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_5)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_6)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_7)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_8)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_9)->ShowWindow(TRUE);
	 GetDlgItem(IDC_STATIC_NO_10)->ShowWindow(TRUE);
	 m_edit_freq1.ShowWindow(TRUE);
	m_edit_freq10.ShowWindow(TRUE);
	m_edit_freq2.ShowWindow(TRUE);
	m_edit_freq3.ShowWindow(TRUE);
	m_edit_freq4.ShowWindow(TRUE);
	m_edit_freq5.ShowWindow(TRUE);
	m_edit_freq6.ShowWindow(TRUE);
	m_edit_freq7.ShowWindow(TRUE);
	m_edit_freq8.ShowWindow(TRUE);
	m_edit_freq9.ShowWindow(TRUE);
	m_edit_rh1.ShowWindow(TRUE);
	m_edit_rh10.ShowWindow(TRUE);
	m_edit_rh2.ShowWindow(TRUE);
	m_edit_rh3.ShowWindow(TRUE);
	m_edit_rh4.ShowWindow(TRUE);
	m_edit_rh5.ShowWindow(TRUE);
	m_edit_rh6.ShowWindow(TRUE);
	m_edit_rh7.ShowWindow(TRUE);
	m_edit_rh8.ShowWindow(TRUE);
	m_edit_rh9.ShowWindow(TRUE);



	GetDlgItem(IDC_STATIC_TSTATADDRESS)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_REGSTERADDRESS)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_CO2SCALE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHAMBER_STABLE_TIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_SENSOR_STABLE_TIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_TSTATADDRESS)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_REGISTER_ADDRESS)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_CO2SCALE)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_STABLETIME_CHAMBER)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_STABLETIME_SENSOR)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_RTD1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_RTD2)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_RTD1)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_RTD2)->ShowWindow(FALSE);


	GetDlgItem(IDC_STATIC_NO_CALIPOINT)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_TEMP_CP)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_CP_TIME)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_CP_TIME_LEFT)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_CO2_POINT)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_CO2POINT)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_HUMPOINT)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_HUMPOINT)->ShowWindow(FALSE);

	GetDlgItem(IDC_START)->ShowWindow(TRUE);
	GetDlgItem(IDC_STOP)->ShowWindow(TRUE);
	GetDlgItem(IDC_CONTINUE)->ShowWindow(TRUE);



	GetDlgItem(IDC_STATIC_SETTING_CO2)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_SETTING_CO2)->ShowWindow(FALSE);
}
else
{



	GetDlgItem(IDC_STATIC_SETTING_CO2)->ShowWindow(TRUE);
	GetDlgItem(IDC_COMBO_SETTING_CO2)->ShowWindow(TRUE);



	GetDlgItem(IDC_START)->ShowWindow(FALSE);
	GetDlgItem(IDC_STOP)->ShowWindow(FALSE);
	GetDlgItem(IDC_CONTINUE)->ShowWindow(FALSE);



	GetDlgItem(IDC_STATIC_CO2_POINT)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_CO2POINT)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_HUMPOINT)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_HUMPOINT)->ShowWindow(TRUE);

	GetDlgItem(IDC_STATIC_NO_CALIPOINT)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_TEMP_CP)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_CP_TIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_CP_TIME_LEFT)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_TSTATADDRESS)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_REGSTERADDRESS)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_CO2SCALE)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_CHAMBER_STABLE_TIME)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_SENSOR_STABLE_TIME)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_TSTATADDRESS)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_REGISTER_ADDRESS)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_CO2SCALE)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_STABLETIME_CHAMBER)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_STABLETIME_SENSOR)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_RTD1)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_RTD2)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_RTD1)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_RTD2)->ShowWindow(TRUE);


	m_check1.ShowWindow(FALSE);
	m_check10.ShowWindow(FALSE);
	m_check2.ShowWindow(FALSE);
	m_check3.ShowWindow(FALSE);
	m_check4.ShowWindow(FALSE);
	m_check5.ShowWindow(FALSE);
	m_check6.ShowWindow(FALSE);
	m_check7.ShowWindow(FALSE);
	m_check8.ShowWindow(FALSE);
	m_check9.ShowWindow(FALSE);
	m_edit_temp1.ShowWindow(FALSE);
	m_edit_temp10.ShowWindow(FALSE);
	m_edit_temp2.ShowWindow(FALSE);
	m_edit_temp3.ShowWindow(FALSE);
	m_edit_temp4.ShowWindow(FALSE);
	m_edit_temp5.ShowWindow(FALSE);
	m_edit_temp6.ShowWindow(FALSE);
	m_edit_temp7.ShowWindow(FALSE);
	m_edit_temp8.ShowWindow(FALSE);
	m_edit_temp9.ShowWindow(FALSE);
	m_edit_time1.ShowWindow(FALSE);
	m_edit_time10.ShowWindow(FALSE);
	m_edit_time2.ShowWindow(FALSE);
	m_edit_time3.ShowWindow(FALSE);
	m_edit_time4.ShowWindow(FALSE);
	m_edit_time5.ShowWindow(FALSE);
	m_edit_time6.ShowWindow(FALSE);
	m_edit_time7.ShowWindow(FALSE);
	m_edit_time8.ShowWindow(FALSE);
	m_edit_time9.ShowWindow(FALSE);
	m_edit_time_left1.ShowWindow(FALSE);
	m_edit_time_left10.ShowWindow(FALSE);
	m_edit_time_left2.ShowWindow(FALSE);
	m_edit_time_left3.ShowWindow(FALSE);
	m_edit_time_left4.ShowWindow(FALSE);
	m_edit_time_left5.ShowWindow(FALSE);
	m_edit_time_left6.ShowWindow(FALSE);
	m_edit_time_left7.ShowWindow(FALSE);
	m_edit_time_left8.ShowWindow(FALSE);
	m_edit_time_left9.ShowWindow(FALSE);
	m_msflexgrid.ShowWindow(FALSE);
	m_CheckSkip.ShowWindow(FALSE);
	m_batch_btn.ShowWindow(FALSE);
	GetDlgItem(IDC_REFRESH_HUMBER_CHAMBER)->ShowWindow(FALSE);
		GetDlgItem(IDC_CALIBRATION_POINT_REPORT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_SENSOR_TABLE)->ShowWindow(FALSE);
	m_cur_sensor.ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_FREQ)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_RH)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_3)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_5)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_6)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_7)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_8)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_9)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NO_10)->ShowWindow(FALSE);
	m_edit_freq1.ShowWindow(FALSE);
	m_edit_freq10.ShowWindow(FALSE);
	m_edit_freq2.ShowWindow(FALSE);
	m_edit_freq3.ShowWindow(FALSE);
	m_edit_freq4.ShowWindow(FALSE);
	m_edit_freq5.ShowWindow(FALSE);
	m_edit_freq6.ShowWindow(FALSE);
	m_edit_freq7.ShowWindow(FALSE);
	m_edit_freq8.ShowWindow(FALSE);
	m_edit_freq9.ShowWindow(FALSE);
	m_edit_rh1.ShowWindow(FALSE);
	m_edit_rh10.ShowWindow(FALSE);
	m_edit_rh2.ShowWindow(FALSE);
	m_edit_rh3.ShowWindow(FALSE);
	m_edit_rh4.ShowWindow(FALSE);
	m_edit_rh5.ShowWindow(FALSE);
	m_edit_rh6.ShowWindow(FALSE);
	m_edit_rh7.ShowWindow(FALSE);
	m_edit_rh8.ShowWindow(FALSE);
	m_edit_rh9.ShowWindow(FALSE);
}
}


void CNewHumChamberView::OnEnSetfocusCo21()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO21)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING0,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo20()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO20)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING9,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo22()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO22)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING1,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo23()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO23)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING2,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo24()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO24)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING3,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo25()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO25)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING4,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo26()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO26)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING5,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo27()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO27)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING6,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo28()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO28)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING7,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnSetfocusCo29()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_CO29)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_SETTING8,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}

void CNewHumChamberView::Fresh_CO2(){
CString StrTemp;
    StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_TSTAT_ADDR]);
	GetDlgItem(IDC_EDIT_TSTATADDRESS)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_SET_CO2_ADDR]);
	GetDlgItem(IDC_EDIT_REGISTER_ADDRESS)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_ERROE]);
	GetDlgItem(IDC_EDIT_CO2SCALE)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_CHARMBER_STABLE_TIME]);
	GetDlgItem(IDC_EDIT_STABLETIME_CHAMBER)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_SENSOR_STABLE_TIME]);
	GetDlgItem(IDC_EDIT_STABLETIME_SENSOR)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_RTD_SETTING0]);
	GetDlgItem(IDC_EDIT_RTD1)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_RTD_SETTING1]);
	GetDlgItem(IDC_EDIT_RTD2)->SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),product_register_value[MODBUS_CO2_CAL_POINT]);
	GetDlgItem(IDC_EDIT_CO2POINT)->SetWindowText(StrTemp);
    StrTemp.Format(_T("%d"),product_register_value[MODBUS_HUM_CAL_POINT]);	
	GetDlgItem(IDC_EDIT_HUMPOINT)->SetWindowText(StrTemp);
	if (product_register_value[MODBUS_CAL_STEPS]>=0&&product_register_value[MODBUS_CAL_STEPS]<=3)
	{
	m_combox_co2_setting.SetCurSel(product_register_value[MODBUS_CAL_STEPS]);
	}
	StrTemp.Empty();
	if (product_register_value[MODBUS_CAL_STEPS]==0)
	{
		StrTemp+=_T("Status:Stop ");
	} 
	else if (product_register_value[MODBUS_CAL_STEPS]==1)
	{
		StrTemp+=_T("Status:RTD ");
	}
	else if (product_register_value[MODBUS_CAL_STEPS]==2)
	{
		StrTemp+=_T("Status:CO2 ");
	}
	else if (product_register_value[MODBUS_CAL_STEPS]==3)
	{
		StrTemp+=_T("Status:HUM ");
	}
	else if (product_register_value[MODBUS_CAL_STEPS]==9)
	{
		StrTemp+=_T("Status:Finished");
	}
	else
	{
		StrTemp+=_T("Status:ERROR ");
	}
	CString temp;
	temp.Format(_T("Current Sensor:%d "),product_register_value[MODBUS_SENSOR_NUM_CALIBRATING]);
	StrTemp+=temp;
	temp.Format(_T("Current Point:%d "),product_register_value[MODBUS_SENSOR_POINT_CALIBRATING]);
	StrTemp+=temp;
	m_Static_TestStatus.SetWindowTextW(StrTemp);
}
 


void CNewHumChamberView::OnEnSetfocusEditTstataddress()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_TSTATADDRESS)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_TSTAT_ADDR,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditRegisterAddress()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_REGISTER_ADDRESS)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_SET_CO2_ADDR,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditCo2scale()
{float temp_float;
CString str_text;
UINT temp_Int;
GetDlgItem(IDC_EDIT_CO2SCALE)->GetWindowText(str_text);
//str_text.Delete(str_text.GetLength()-1,1);//delete % 
temp_float=(float)_wtof(str_text);
temp_Int=(UINT)(temp_float);
Set(MODBUS_CO2_ERROE,temp_Int);
is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditStabletimeChamber()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_STABLETIME_CHAMBER)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CHARMBER_STABLE_TIME,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditStabletimeSensor()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_STABLETIME_SENSOR)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_SENSOR_STABLE_TIME,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditRtd1()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_RTD1)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_RTD_SETTING0,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditRtd2()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_RTD2)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_RTD_SETTING1,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}

 


void CNewHumChamberView::OnEnSetfocusEditHumpoint()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_CO2POINT)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_HUM_CAL_POINT,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnEnSetfocusEditCo2point()
{
	float temp_float;
	CString str_text;
	UINT temp_Int;
	GetDlgItem(IDC_EDIT_HUMPOINT)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	temp_Int=(UINT)(temp_float);
	Set(MODBUS_CO2_CAL_POINT,temp_Int);
	is_Show_Write_singleDLG=TRUE;
}


void CNewHumChamberView::OnCbnSelchangeComboSettingCo2()
{
	int sel=m_combox_co2_setting.GetCurSel();
	if (sel==product_register_value[MODBUS_CAL_STEPS])
	{
	return;
	}
	int ret=write_one(m_tstatID,MODBUS_CAL_STEPS,sel);

}
void CNewHumChamberView::Fresh_CO2_Time(){
	CString StrTemp;
	StrTemp.Empty();
	if (product_register_value[MODBUS_CAL_STEPS]==0)
	{
		StrTemp+=_T("Status:Stop ");
	} 
	else if (product_register_value[MODBUS_CAL_STEPS]==1)
	{
		StrTemp+=_T("Status:RTD ");
	}
	else if (product_register_value[MODBUS_CAL_STEPS]==2)
	{
		StrTemp+=_T("Status:CO2 ");
	}
	else if (product_register_value[MODBUS_CAL_STEPS]==3)
	{
		StrTemp+=_T("Status:HUM ");
	}
	else if (product_register_value[MODBUS_CAL_STEPS]==9)
	{
		StrTemp+=_T("Status:Finish ");
	}
	else
	{
		StrTemp+=_T("Status:Default ");
	}
	CString temp;
	temp.Format(_T("Current Sensor:%d "),product_register_value[MODBUS_SENSOR_NUM_CALIBRATING]);
	StrTemp+=temp;
	temp.Format(_T("Current Point:%d "),product_register_value[MODBUS_SENSOR_POINT_CALIBRATING]);
	StrTemp+=temp;
	m_Static_TestStatus.SetWindowTextW(StrTemp);
}

void CNewHumChamberView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	if (message==WM_FRESH_TESTO)
// 	{

//  		CString  StrTemp;
//  		StrTemp.Format(_T("%0.0f ppm"),m_value[0]);
//  		m_edit_co2.SetWindowTextW(StrTemp);
//  		StrTemp.Format(_T("%0.0f Pa"),m_value[1]);
//  		m_edit_presure.SetWindowTextW(StrTemp);
//  		StrTemp.Format(_T("%0.1f C"),m_value[2]);
//  		m_edit_temp.SetWindowTextW(StrTemp);
//  		StrTemp.Format(_T("%0.1f %%"),m_value[3]);
//  		m_edit_hum.SetWindowTextW(StrTemp);
//  		StrTemp.Format(_T("Read Testo Times=%0.0f,Write Times=%0.0f"),m_times,m_Write_times);
//  		m_libel_times.SetWindowTextW(StrTemp);

// 		return 1;
// 	}
// 	else if (message==WM_FRESH_HUM_CHAMBER)
// 	{

// 		Fresh_CalibartionPoints();
// 	U+=1;
// 	TRACE(_T("2...OnTimer %0.1f\r\n"),U);
// 		return 1;
// 	}
// 	else if (message==WM_FRESH_CO2)
// 	{

		//Fresh_CO2_Time();
// 		return 1; 
// 	}

	CFormView::OnTimer(nIDEvent);
}

void CNewHumChamberView::OnBnClickedCalibrationPointReport()
{
	CString strFilter;
	CString strFilename;
	//float curtemp,curhum,sensortemp,sensorhum;
   /* strFilter =_T( "Text File|*.csv|All File|*.*|");
    CFileDialog dlg(false,_T("*.xls"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
    if (dlg.DoModal() != IDOK)
        return;
    strFilename=dlg.GetPathName();*/
    _Application app;    
    Workbooks books;
    _Workbook book;
    Worksheets sheets;
    _Worksheet sheet;
    Range range;
    Range rgMyRge1, rgMyRge2; 	
    COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    Sleep(2000);
 
  

     	if (!app.CreateDispatch(_T("Excel.Application"),NULL)) 
     	{ 
     		AfxMessageBox(_T("Create Excel false!")); 
     		 return;
     	} 
    //遍历所有行  
        strFilename=g_strExePth+_T("Certificate of Calibration.xls");

        books.AttachDispatch(app.GetWorkbooks()); 
        book.AttachDispatch(books.Add(_variant_t(strFilename)));

        sheets.AttachDispatch(book.GetWorksheets());	
        sheet.AttachDispatch(sheets.GetItem(_variant_t("Sheet1")));

        range.AttachDispatch(sheet.GetCells()); 
        CString SerialDate;
        CTime time = CTime::GetCurrentTime();
        //11行 1列
        SerialDate.Format(_T("Calibration Date:"),time.GetYear(),time.GetMonth(),time.GetDay());
        range.SetItem(_variant_t((long)(11)),_variant_t((long)(1)),_variant_t(SerialDate));
        //11行 1列
        SerialDate.Format(_T("%d-%d-%d"),time.GetYear(),time.GetMonth(),time.GetDay());
        range.SetItem(_variant_t((long)(11)),_variant_t((long)(5)),_variant_t(SerialDate));
        //13行 1列
        SerialDate.Format(_T("Certificate Date:"),time.GetYear(),time.GetMonth(),time.GetDay());
        range.SetItem(_variant_t((long)(13)),_variant_t((long)(1)),_variant_t(SerialDate));
        SerialDate.Format(_T("%d-%d-%d"),time.GetYear(),time.GetMonth(),time.GetDay());
        range.SetItem(_variant_t((long)(13)),_variant_t((long)(5)),_variant_t(SerialDate));

       range.AttachDispatch(sheet.GetCells());
       CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
       pDlg->Create(IDD_DIALOG10_Progress, this);
       pDlg->ShowProgress(0,0);
       pDlg->ShowWindow(SW_SHOW);
       RECT RECT_SET1;
       GetClientRect(&RECT_SET1);
       pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);
       CurrentCol=1;
       CurrentRow=27;
        for (int i=0;i<product_register_value[MODBUS_SENSOR_TOTAL]-1;i++)
        {
            float progress;
            if (pDlg!=NULL)
            {
                progress=float((i+1)*(100/(2*product_register_value[MODBUS_SENSOR_TOTAL])));
                pDlg->ShowProgress(int(progress),(int)progress);
            } 

            rgMyRge1.AttachDispatch(range.GetItem(COleVariant((long)CurrentRow+1+i), COleVariant((long)CurrentRow+1+i)).pdispVal, TRUE);
            rgMyRge1.AttachDispatch(rgMyRge1.GetEntireRow(), TRUE);

            rgMyRge2.AttachDispatch(range.GetItem(COleVariant((long)CurrentRow+i), COleVariant((long)1+i)).pdispVal, TRUE);
            rgMyRge2.AttachDispatch(rgMyRge2.GetEntireRow(), TRUE);

            rgMyRge2.Copy(covOptional);
            rgMyRge1.Insert(COleVariant((long)CurrentRow+1+i));
        }
    CString rowContent;  
    CurrentCol=1;
    CurrentRow=27;
 

    
    CString Rowtemp;
    for (int i=1;i<=product_register_value[MODBUS_SENSOR_TOTAL];i++)
    {   

         float progress;
        if (pDlg!=NULL)
        {
          //  progress=float((i+1)*(100/product_register_value[NumSensors.Start_ID]));

            progress=float((product_register_value[MODBUS_SENSOR_TOTAL]+i+1)*(100/(2*product_register_value[MODBUS_SENSOR_TOTAL])));

            pDlg->ShowProgress(int(progress),(int)progress);
        } 

        if (product_register_value[MODBUS_SENSOR1_STATE +i-1]!=1)
        {
        continue;
        }

   
      
        rowContent.Format(_T("%d"),m_SN[i-1]);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(rowContent));
        //Current Temp
        
        //rowContent.Format(_T("%0.1f"),curtemp);
        //range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2)),_variant_t(rowContent));
        // //Current Hum
        //rowContent.Format(_T("%0.1f"),curhum);
        //range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(3)),_variant_t(rowContent));
		float curtemp,curhum,sensortemp,sensorhum;
		curtemp=(float)product_register_value[MODBUS_SENSOR_HUM]/10.0;
		curhum= (float)product_register_value[MODBUS_SENSOR_TEMP]/10.0;

        sensortemp=(float)product_register_value[MODBUS_SENSOR1_TEMP+2*(i-1)]/10.0;
        rowContent.Format(_T("%0.1f"),sensortemp);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(4)),_variant_t(rowContent)); 

		sensorhum=(float)product_register_value[MODBUS_SENSOR1_HUM+2*(i-1)]/10.0;
        rowContent.Format(_T("%0.1f"),(sensorhum));
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(5)),_variant_t(rowContent));
        
        rowContent.Format(_T("%0.1f"),(curtemp-sensortemp));
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(6)),_variant_t(rowContent));

        rowContent.Format(_T("%0.1f"),(curhum-sensorhum));
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(7)),_variant_t(rowContent));

        rowContent=_T("Pass");
         range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(8)),_variant_t(rowContent));
         
        ++CurrentRow;

    } 
    

   pDlg->ShowWindow(SW_HIDE);
    if(pDlg!=NULL)
    {delete pDlg;
    pDlg=NULL;}

    AfxMessageBox(_T("Export Completely"));

 

    app.SetVisible(true); 
    range.ReleaseDispatch(); 
    sheet.ReleaseDispatch(); 
    sheets.ReleaseDispatch(); 
    book.ReleaseDispatch(); 
    books.ReleaseDispatch();
    app.ReleaseDispatch(); 
}

void CNewHumChamberView::OnBnClickedCalibrationPointReport2()
{
    CString  Product_Head_File_Name;
    CString strFilter;
    CString strFilename;
    CString strTemp;
    CString RegisterName;
    CString RegisterID;
    CString logstr;
    _variant_t  temp_variant;
    _Application app;    
    Workbooks books;
    _Workbook book;
    Worksheets sheets;
    _Worksheet sheet;
    Range range;
    Range rgMyRge1, rgMyRge2; 	
    COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

    int CurrentRow;


    if (!app.CreateDispatch(_T("Excel.Application"),NULL)) 
    { 
        AfxMessageBox(_T("Create Excel false!")); 
        return;
    } 
    //遍历所有行  
    strFilename=g_strExePth+_T("Calibration Sensor Report.xls");

    books.AttachDispatch(app.GetWorkbooks()); 
    book.AttachDispatch(books.Add(_variant_t(strFilename)));

    sheets.AttachDispatch(book.GetWorksheets());	
    sheet.AttachDispatch(sheets.GetItem(_variant_t("Sheet1")));

    range.AttachDispatch(sheet.GetCells()); 
    CString ContentData;
    CTime time = CTime::GetCurrentTime();

    ContentData.Format(_T("Calibration Date"));
    range.SetItem(_variant_t((long)(1)),_variant_t((long)(1)),_variant_t(ContentData));

    ContentData.Format(_T("%d-%d-%d"),time.GetYear(),time.GetMonth(),time.GetDay());
    range.SetItem(_variant_t((long)(1)),_variant_t((long)(3)),_variant_t(ContentData));

    range.AttachDispatch(sheet.GetCells());


    int realrow = 0; 
    for (int i=0;i<(int)product_register_value[MODBUS_SENSOR_TOTAL];i++)
    {  
        if (product_register_value[MODBUS_SENSOR1_STATE +i]!=1)
        {
            continue;
        }
        realrow++;
       /* int ret = write_one(g_tstat_id,MODBUS_SENSOR_NUM,i+1);*/

		 int ret =write_one(m_tstatID,MODBUS_REFRESH_SENSOR,0);
			 
			  /*if (ret<0)
			  {
				  AfxMessageBox(_T("Write Fail"));
				  return;
			  }*/
			
			  ret=write_one(m_tstatID,MODBUS_SENSOR_NUM,i+1);
			
			/*if (ret<0)
			{
				AfxMessageBox(_T("Write Fail"));
				return;
			}*/
			product_register_value[MODBUS_SENSOR_NUM]=i+1;
 
            int times=10;
			/*Sleep(2000);*/
			int  flash=Read_One(m_tstatID,MODBUS_REFRESH_SENSOR);
			while ((flash!=1)&&(times>0))
			{
			    flash=Read_One(m_tstatID,MODBUS_REFRESH_SENSOR);
				if (flash==1)
				{
				 break;
				}
				Sleep(500);
				--times;
			}
			/*if (flash!=1)
			{
			AfxMessageBox(_T("Time Out!"));
			return;
			}*/

        Sleep(200);
        CurrentRow = 3 + 7*(realrow-1);
        ContentData = _T("Serial Number");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData)); 
        ContentData.Format(_T("%d"),m_SN[i]);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2)),_variant_t(ContentData));
       
        unsigned short databuffer[10];
          ret = Read_Multi(m_tstatID,&databuffer[0],527,10,5);
         if (ret<0)
         {   Sleep(1000);
             Read_Multi(m_tstatID,&databuffer[0],527,10,5);
         }
        ContentData = _T("Points");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(3)),_variant_t(ContentData));
        ContentData.Format(_T("%d"),databuffer[531 - 527]);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(4)),_variant_t(ContentData)); 

        ContentData = _T("Frequency");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(5)),_variant_t(ContentData));
        ContentData.Format(_T("%d"),databuffer[530 - 527]);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(6)),_variant_t(ContentData));

        ++CurrentRow;
        ContentData = _T("Temperature");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));
        ContentData.Format(_T("%0.1f"),((float)((short)databuffer[529 - 527]))/10);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2)),_variant_t(ContentData));

        ContentData = _T("Humidity");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(3)),_variant_t(ContentData));
        ContentData.Format(_T("%0.1f"),((float)((short)databuffer[528 - 527]))/10);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(4)),_variant_t(ContentData));

         ret=Read_Multi(m_tstatID,&product_register_value[MODBUS_HUMCOUNT1_H],MODBUS_HUMCOUNT1_H ,20);
         if (ret<0)
         {
          ret=Read_Multi(m_tstatID,&product_register_value[MODBUS_HUMCOUNT1_H],MODBUS_HUMCOUNT1_H ,20);
         }
          
        
        ++CurrentRow;
        ContentData = _T("RH");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));

        for (int j = 0;j<10;j++)
        {
            ContentData.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_HUMRH1_H  + 2*j]))/10);
            range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2+j)),_variant_t(ContentData));
        }
        ++CurrentRow;
        ContentData = _T("Frequency");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));
        for (int j = 0;j<10;j++)
        {
            ContentData.Format(_T("%d"),(short)product_register_value[ MODBUS_HUMCOUNT1_H+ 2*j]);
            range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2+j)),_variant_t(ContentData));
        } 


    }



    AfxMessageBox(_T("Export Completely"));

    app.SetVisible(true); 
    range.ReleaseDispatch(); 
    sheet.ReleaseDispatch(); 
    sheets.ReleaseDispatch(); 
    book.ReleaseDispatch(); 
    books.ReleaseDispatch();
    app.ReleaseDispatch(); 
}


void CNewHumChamberView::OnBnClickedButtonTestoGraphic()
{

      
}


void CNewHumChamberView::OnBnClickedCheckRead()
{
	if (m_IsReadIniFile)
	{
		m_IsReadIniFile = FALSE;
	}
	else
	{
		m_IsReadIniFile = TRUE;
	}
	 
   ((CButton *)GetDlgItem(IDC_CHECK_READ))->SetCheck(m_IsReadIniFile);
	 
	
}
