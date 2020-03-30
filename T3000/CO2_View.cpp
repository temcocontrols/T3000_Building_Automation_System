// CO2_View.cpp : implementation file
// CO2 coded by Fance 2013 07 20
// Already finished CO2 with RS485.
#include "stdafx.h"
#include "T3000.h"
#include "CO2_View.h"
#include "global_function.h"
#include "CM5\MyOwnListCtrl.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include <bitset>
#include "GraphicMode.h"
// CCO2_View
extern CString Pressure_Units[8];
enum
{
    P_inWC = 0,
    P_KPa = 1,
    P_Psi = 2,
    P_mmHg = 3,
    P_inHg = 4,
    P_Kg_cm2 = 5,
    P_atmosphere = 6,
    P_bar = 7,
    P_Unit_End,
};
int decimal_num[2][8] = {
    {2,4,5,2,5,6,6,6},		//default unit = inWC
    {0,0,1,0,2,3,3,3}		//default unit = Psi
};

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
    m_product_type= 2;
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
	DDX_Control(pDX, IDC_CO2_BAUDRATECOMBO, m_co2_baudRateCombox);
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
	// DDX_Control(pDX, IDC_LIST_CO2_EXTERNAL_SENSOR, m_co2_external_sensor_list);
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
	DDX_Control(pDX, IDC_LIST_INPUT_AQ, m_input_list);
	DDX_Control(pDX, IDC_LIST_OUTPUT_AQ, m_output_list);
	DDX_Control(pDX, IDC_BUTTON_ONOFF, m_button_onoff);
	DDX_Control(pDX, IDC_EDIT1_NCO2, m_edit_nco2);
	DDX_Control(pDX, IDC_EDIT_RTIME, m_edit_rtime);
	DDX_Control(pDX, IDC_EDIT_RTIME2, m_edit_adjustment);
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
   // ON_MESSAGE(WM_LIST_ITEM_CHANGED,Save_List_Item)
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
   // ON_BN_CLICKED(IDC_BTN_CO2_CLEAR_CAL, &CCO2_View::OnBnClickedBtnCo2ClearCal)
    ON_BN_CLICKED(IDC_RADIO_HUMIDITY_HEAT_ENABLE, &CCO2_View::OnBnClickedRadioHumidityHeatEnable)
    ON_BN_CLICKED(IDC_RADIO_HUMIDITY_HEAT_DISABLE, &CCO2_View::OnBnClickedRadioHumidityHeatDisable)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_HUMIDITY, &CCO2_View::OnEnKillfocusEditCo2Humidity)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2VALUE, &CCO2_View::OnEnKillfocusEditCo2value)
    ON_EN_KILLFOCUS(IDC_EDIT_VALUE_GRID, &CCO2_View::OnEnKillfocusEditValueGrid)

    ON_WM_TIMER()
    ON_EN_KILLFOCUS(IDC_ID_CO2_EDIT, &CCO2_View::OnEnKillfocusIdCo2Edit)
    ON_CBN_SELCHANGE(IDC_CO2_BAUDRATECOMBO, &CCO2_View::OnCbnSelchangeCo2Baudratecombo)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Item_List)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Lists)
    ON_NOTIFY(NM_CLICK, IDC_LIST_INPUT_AQ, &CCO2_View::OnNMClickList_Input)
    ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT_AQ, &CCO2_View::OnNMClickList_Output)
   // ON_NOTIFY(NM_CLICK, IDC_LIST_CO2_EXTERNAL_SENSOR, &CCO2_View::OnNMClickList_CO2List)  //Fresh_Lists
    ON_BN_CLICKED(IDC_GRAPIC, &CCO2_View::OnBnClickedGrapic)
    ON_BN_CLICKED(IDC_TEMP_SENSOR, &CCO2_View::OnBnClickedTempSensor)
    ON_BN_CLICKED(IDC_HUM_SENSOR, &CCO2_View::OnBnClickedHumSensor)
	  ON_EN_KILLFOCUS(IDC_EDIT1_NCO2, &CCO2_View::OnEnKillfocusEdit1Nco2)
	  ON_EN_KILLFOCUS(IDC_EDIT_RTIME, &CCO2_View::OnEnKillfocusEditRtime)
	  ON_BN_CLICKED(IDC_BUTTON_ONOFF, &CCO2_View::OnBnClickedButtonOnoff)
	  ON_EN_KILLFOCUS(IDC_EDIT_RTIME2, &CCO2_View::OnEnKillfocusEditRtime2)
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
/// <summary>
/// Load CO2's Window
/// </summary>
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

    if (product_register_value[7] == PM_CO2_NODE)
    {
        m_product_type = 3;
    }
    else
    {
        Initial_Registerlist();
        bitset<16> SensorBit(product_register_value[14]);
        if (SensorBit[0])
        {
            m_product_type = 1;
        }
        else
        {
            m_product_type = 2;
        }
    }



    SH_Window();


    if (m_product_type == 1)
    {
	    Read_Multi(g_tstat_id,ValueData,10000,10);
        Fresh_CO2();
    }
    else if(m_product_type == 2)
    {
        Initial_InputList();
        Initial_OutputList();
    }
    else if (m_product_type == 3)
    {
      CString strTemp;
      strTemp.Format (_T("%d"),product_register_value[112]);
      GetDlgItem (IDC_CO2_PREPARE_ALARM_SETPOINT)->SetWindowTextW(strTemp);
      strTemp.Format (_T("%d"),product_register_value[113]);
       GetDlgItem (IDC_CO2_ALARM_SETPOINT)->SetWindowTextW(strTemp);
      strTemp.Format (_T("%d"),product_register_value[109]);
      GetDlgItem (IDC_CO2_CALIBRATING_OFFSET)->SetWindowTextW(strTemp);

      if (product_register_value[124] == 0)
      {
            CButton *pTempButton;
            pTempButton = (CButton *)(GetDlgItem (IDC_TEMP_SENSOR)) ;
            pTempButton->SetCheck (1);

            pTempButton = (CButton *)(GetDlgItem (IDC_HUM_SENSOR)) ;
            pTempButton->SetCheck (0);

      }
      else
      {
          CButton *pTempButton;
          pTempButton = (CButton *)(GetDlgItem (IDC_TEMP_SENSOR)) ;
          pTempButton->SetCheck (0);

          pTempButton = (CButton *)(GetDlgItem (IDC_HUM_SENSOR)) ;
          pTempButton->SetCheck (1);
      }

      Initial_InputList();
      Initial_OutputList();

	  Fresh_CO2Node_Compensatin();
    }


        GetDlgItem(IDC_BTN_CO2_REFRESH)->EnableWindow(FALSE);







}
/// <summary>
/// Show ,Hide some window Fields ,according to different devices
/// </summary>
void CCO2_View::SH_Window(){
    CString temp_id;
    float m_fFirmwareVersion=0;
    UINT m_nSerialNumber=0;
    unsigned short m_nHardwareVersion=0;
    temp_id.Format(_T("%d"),product_register_value[6]);
    m_co2_idAdressEdit.SetWindowTextW(temp_id);



    CString temp_firmversion;
    temp_firmversion.Format(_T("%0.2f"),((float)(product_register_value[5]*256+product_register_value[4]))/10.0);

    m_co2_firmwareversion= _wtof(temp_firmversion);//get_curtstat_version();
    m_co2_serialNumber=get_serialnumber();
    m_co2_hardwareversion=product_register_value[8];//8
    UpdateData(FALSE);

    if (m_product_type == 1)
    {
       GetDlgItem (IDC_PRODUCT_NAME)->SetWindowTextW(L"CO2");
        if(product_register_value[CO2_485_MODBUS_BAUDRATE]>=0&&product_register_value[CO2_485_MODBUS_BAUDRATE]<=1)
            m_co2_baudRateCombox.SetCurSel(product_register_value[CO2_485_MODBUS_BAUDRATE]);
        else
            m_co2_baudRateCombox.SetCurSel(0);

        CString temp_firmversion;
        temp_firmversion.Format(_T("%0.2f"),((float)(product_register_value[CO2_485_MODBUS_VERSION_NUMBER_HI]*256+product_register_value[CO2_485_MODBUS_VERSION_NUMBER_LO]))/10.0);

        m_co2_firmwareversion= _wtof(temp_firmversion);//get_curtstat_version();
        m_co2_serialNumber=get_serialnumber();
        m_co2_hardwareversion=product_register_value[CO2_485_MODBUS_HARDWARE_REV];//8
        UpdateData(FALSE);

        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT3)->ShowWindow(SW_HIDE);


          GetDlgItem(IDC_CO2_BAUDRATECOMBO)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_RELATIVE_HUMIDITY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_CO2)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_CO2VALUE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_PPM)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALARM_SETTING)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_RADIO_ALARM_MANUAL)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_RADIO_ALARM_AUTO)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALARM_STATE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_ALARM_STATE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_PASSWORD)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_RADIO_PASSWORD_ENABLE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_RADIO_PASSWORD_DISABLE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_CO2_PASSWOR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_HEATINNG_HUM_SENSOR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_ENABLE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_DISABLE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_MSFLEXGRID_INPUT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_CO2_SENSOR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_PREPARE_ALARM_SETPOINT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALARM_SETPOINT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_CALIBRATING_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BTN_CO2_CLEAR_CAL)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALARM_MENU)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALON)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALOFF)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_MBT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_BT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_ALARM_ON_TIME)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_ALARM_OFF_TIME)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_CO2_BLOCK_TIME)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_CO2_BACKLIGHT_TIME)->ShowWindow(SW_SHOW);

        GetDlgItem(IDC_STATIC_CALIBRATION_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_SETPOINT)->ShowWindow(SW_SHOW);

         GetDlgItem(IDC_STATIC_INPUT_SETTING)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LIST_INPUT_AQ)->ShowWindow(SW_HIDE);
       // GetDlgItem(IDC_LIST_OUTPUT_AQ)->ShowWindow(SW_HIDE);
      //  GetDlgItem(IDC_STATIC_OUTPUT_SETTING)->ShowWindow(SW_HIDE);


        GetDlgItem(IDC_STATIC_LAB_DATE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_TIME)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_DATETIMEPICKER_TIME)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_DATETIMEPICKER1)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BUTTON_CO2_SYNC_TIME)->ShowWindow(SW_SHOW);

//         GetDlgItem(IDC_TEMP_SENSOR)->ShowWindow(SW_HIDE);
//         GetDlgItem(IDC_HUM_SENSOR)->ShowWindow(SW_HIDE);

         GetDlgItem(IDC_STATIC_BAUDRATE)->ShowWindow(SW_SHOW);


    }
    else if (m_product_type == 2)
    {
          GetDlgItem (IDC_PRODUCT_NAME)->SetWindowTextW(L"Pressure");
          GetDlgItem(IDC_STATIC_LAB_DATE)->ShowWindow(SW_HIDE);
          GetDlgItem(IDC_STATIC_TIME)->ShowWindow(SW_HIDE);

          GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT2)->ShowWindow(SW_HIDE);
          GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT3)->ShowWindow(SW_HIDE);

          if(product_register_value[CO2_485_MODBUS_BAUDRATE]>=0&&product_register_value[CO2_485_MODBUS_BAUDRATE]<=1)
              m_co2_baudRateCombox.SetCurSel(product_register_value[CO2_485_MODBUS_BAUDRATE]);
          else
              m_co2_baudRateCombox.SetCurSel(0);

        GetDlgItem(IDC_CO2_DATETIMEPICKER_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_DATETIMEPICKER1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_CO2_SYNC_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_BAUDRATECOMBO)->ShowWindow(SW_SHOW);

        GetDlgItem(IDC_STATIC_INPUT_SETTING)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_LIST_INPUT_AQ)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_LIST_OUTPUT_AQ)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_OUTPUT_SETTING)->ShowWindow(SW_SHOW);


        GetDlgItem(IDC_STATIC_RELATIVE_HUMIDITY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_CO2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2VALUE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PPM)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_SETTING)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_ALARM_MANUAL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_ALARM_AUTO)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_STATE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_STATE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PASSWORD)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_PASSWORD_ENABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_PASSWORD_DISABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_PASSWOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_HEATINNG_HUM_SENSOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_ENABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_DISABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_MSFLEXGRID_INPUT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_CO2_SENSOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_PREPARE_ALARM_SETPOINT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_SETPOINT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_CALIBRATING_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BTN_CO2_CLEAR_CAL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_MENU)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALON)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALOFF)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MBT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_BT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_ON_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_OFF_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_BLOCK_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_BACKLIGHT_TIME)->ShowWindow(SW_HIDE);
        // GetDlgItem(IDC_LIST_CO2_EXTERNAL_SENSOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_CALIBRATION_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_SETPOINT)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_TEMP_SENSOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_HUM_SENSOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_BAUDRATE)->ShowWindow(SW_SHOW);
    }
    else
    {
        GetDlgItem (IDC_PRODUCT_NAME)->SetWindowTextW(L"CO2 NODE");

        GetDlgItem(IDC_STATIC_RELATIVE_HUMIDITY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_CO2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2VALUE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PPM)->ShowWindow(SW_HIDE);


		 GetDlgItem(IDC_STATIC_BAUDRATE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_BAUDRATECOMBO)->ShowWindow(SW_SHOW);
		m_co2_baudRateCombox.SetCurSel(product_register_value[15]);


        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT3)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_SETTING)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_ALARM_MANUAL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_ALARM_AUTO)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_STATE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_STATE)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_PASSWORD)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_PASSWORD_ENABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_PASSWORD_DISABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_PASSWOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_HEATINNG_HUM_SENSOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_ENABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO_HUMIDITY_HEAT_DISABLE)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_MSFLEXGRID_INPUT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_CO2_SENSOR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_PRE_ALARM_SETPOINT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_PREPARE_ALARM_SETPOINT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ALARM_SETPOINT)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CO2_CALIBRATING_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BTN_CO2_CLEAR_CAL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALARM_MENU)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALON)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ALOFF)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MBT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_BT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_ON_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_OFF_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_BLOCK_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CO2_BACKLIGHT_TIME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_CALIBRATION_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_ALARM_SETPOINT)->ShowWindow(SW_SHOW);

        GetDlgItem(IDC_STATIC_INPUT_SETTING)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_LIST_INPUT_AQ)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_LIST_OUTPUT_AQ)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_OUTPUT_SETTING)->ShowWindow(SW_SHOW);


        GetDlgItem(IDC_STATIC_LAB_DATE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_DATETIMEPICKER_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CO2_DATETIMEPICKER1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_CO2_SYNC_TIME)->ShowWindow(SW_HIDE);


    }
}
/// <summary>
/// Load CO2's page
/// </summary>
void CCO2_View::Fresh_CO2(){
     Initial_List();
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
	Initial_InputList();
  Initial_OutputList();

   // Fresh_External_List();


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

        strUnit=strTemp1;
    }
    else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
    {
        i_internal_temp = product_register_value[CO2_485_MODBUS_TEMPERATURE_F_INTERNAL];
        i_external_temp = product_register_value[CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL];

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

//      if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//�ڲ�
//      {
//          TempValue.Format(_T("%0.1f"),f_internal_temp);
//      }
//      else
//      {
//         TempValue.Format(_T("%0.1f"),f_external_temp);
//      }


	BOOL AM=Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],1);
	if (AM)
	{
		StrAM=_T("Manual");
		TempValue.Format(_T("%0.1f"),(float)((short)product_register_value[CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM])/10);
	}
	else
	{
		StrAM=_T("Auto");
		TempValue.Format(_T("%0.1f"),f_external_temp);
	}
	m_grid_input.put_TextMatrix(1,3,StrAM);
	m_grid_input.put_TextMatrix(1,4,TempValue);
	AM=Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],2);
	if (AM)
	{
		StrAM=_T("Manual");
		TempValue.Format(_T("%0.1f"),(float)((short)product_register_value[CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM])/10);
	}
	else
	{
		StrAM=_T("Auto");
		TempValue.Format(_T("%0.1f"),(float)(product_register_value[CO2_485_MODBUS_HUMIDITY_RH])/10.0);
	}
	m_grid_input.put_TextMatrix(2,3,StrAM);
	m_grid_input.put_TextMatrix(2,4,TempValue);
	AM=Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],3);
	if (AM)
	{
		StrAM=_T("Manual");
		TempValue.Format(_T("%d"),(short)product_register_value[CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2]);
	}
	else
	{
		StrAM=_T("Auto");
		TempValue.Format(_T("%d"),product_register_value[CO2_485_MODBUS_INTERNAL_CO2_PPM]);
	}
	m_grid_input.put_TextMatrix(3,3,StrAM);
	m_grid_input.put_TextMatrix(3,4,TempValue);


}

void CCO2_View::Node_SHOW_TEMP()
{
    int i_internal_temp = 0;
    int i_external_temp = 0;
    float f_internal_temp = 0;
    float f_external_temp = 0;
    CString temp_internal_value,temp_external_value;



    CString strTemp1,strTemp2,strUnit,strHUM,strCO2;
    strTemp1.Format(_T("%cC"),176);
    strTemp2.Format(_T("%cF"),176);





    if(product_register_value[125] == 0)
    {
        i_internal_temp = product_register_value[119];
        i_external_temp = product_register_value[121];

        strUnit=strTemp1;
    }
    else if((product_register_value[125] == 1))
    {
        i_internal_temp = product_register_value[120];
        i_external_temp = product_register_value[122];

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

    if(product_register_value[124] == 0)//�ڲ�
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
//      m_co2_external_sensor_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
//      m_co2_external_sensor_list.SetExtendedStyle(m_co2_external_sensor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
//
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_DEVICE_ID, _T("Device ID"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_SERIAL_NUM, _T("Serial Number"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_PPM, _T("External PPM"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_PRE_ALARM_SP, _T("Prepare Alarm Setpoint"), 130, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_ALARM_SP, _T("Alarm Setpoint"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
//      m_co2_external_sensor_list.InsertColumn(CO2_EXTERNAL_CAL_OFFSET, _T("Calibrating Offset"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
//      //Fresh_External_List();
//      g_hwnd_now = this->m_hWnd;
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


     m_grid_input.put_TextMatrix(1,1,_T("Temperature"));
     m_grid_input.put_TextMatrix(2,1,_T("Hum"));
     m_grid_input.put_TextMatrix(3,1,_T("CO2"));


     m_grid_input.put_TextMatrix(2,2,_T("%"));
     m_grid_input.put_TextMatrix(3,2,_T("ppm"));
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
            strTemp=_T("��");
        }
        else
        {
            strTemp=_T("�H");
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
    
}


void CCO2_View::OnCbnSelchangeComboCo2SensorSel()
{
    
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
            if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))//��ǰ��1   ѡ����0
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

    
    for (int i=0;i<(int)Change_Color_ID.size();i++)
    {
        if(/*nCtlColor==CTLCOLOR_EDIT &&*/pWnd->GetDlgCtrlID()==Change_Color_ID.at(i))//ע���˴��ģ�pWnd->��������ûЧ��
        {
            pDC->SetTextColor(RGB(0,0,0));
            pDC->SetBkColor(RGB(255,0,0));	//�����ı�����ɫ
            pDC->SetBkMode(TRANSPARENT);	//���ñ���͸��
            hbr = (HBRUSH)m_brush;
        }

    }
    
    return hbr;
}


void CCO2_View::OnEnKillfocusEditExternalTemp()
{
    
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
    if (m_product_type == 3)
    {
        if(m_edit_pre_alarm_setpoint != product_register_value[112])
        {
            Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,112,m_edit_pre_alarm_setpoint,
                product_register_value[112],this->m_hWnd,IDC_CO2_PREPARE_ALARM_SETPOINT,_T("Prepare Alarm Setpoint"));
        }
    }
    else
    {



        if(m_edit_pre_alarm_setpoint != product_register_value[CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT])
        {
            Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT,m_edit_pre_alarm_setpoint,
                product_register_value[CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT],this->m_hWnd,IDC_CO2_PREPARE_ALARM_SETPOINT,_T("Prepare Alarm Setpoint"));
        }
    }
}


void CCO2_View::OnEnKillfocusCo2AlarmSetpoint()
{
    
    UpdateData();
    if (m_product_type == 3)
    {
        if(m_edit_alarm_setpoint != product_register_value[113])
        {
            Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,113,m_edit_alarm_setpoint,
                product_register_value[113],this->m_hWnd,IDC_CO2_ALARM_SETPOINT,_T("Alarm Setpoint"));
        }
    }
    else
    {
        if(m_edit_alarm_setpoint != product_register_value[CO2_485_MODBUS_INT_ALARM_SETPOINT])
        {
            Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INT_ALARM_SETPOINT,m_edit_alarm_setpoint,
                product_register_value[CO2_485_MODBUS_INT_ALARM_SETPOINT],this->m_hWnd,IDC_CO2_ALARM_SETPOINT,_T("Alarm Setpoint"));
        }
    }

}


void CCO2_View::OnEnKillfocusCo2CalibratingOffset()
{
    
    UpdateData();
    if (m_product_type == 3)
    {
        if(m_edit_calibrating_offset != product_register_value[109])
        {
            Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,109,m_edit_calibrating_offset,
                product_register_value[109],this->m_hWnd,IDC_CO2_CALIBRATING_OFFSET,_T("Calibrating Offset"));
        }
    }
    else
    {
        if(m_edit_calibrating_offset != product_register_value[CO2_485_MODBUS_INT_CO2_OFFSET])
        {
            Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_INT_CO2_OFFSET,m_edit_calibrating_offset,
                product_register_value[CO2_485_MODBUS_INT_CO2_OFFSET],this->m_hWnd,IDC_CO2_CALIBRATING_OFFSET,_T("Calibrating Offset"));
        }
    }

}


void CCO2_View::OnBnClickedRadioAlarmManual()
{
    
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
// LRESULT CCO2_View::Save_List_Item(WPARAM wParam,LPARAM lParam)
// {
//     int Changed_Item = (int)wParam;
//     int Changed_SubItem = (int)lParam;
//     int temp_write_reg = 0;
//     int temp_reg_value = 0;
//     CString temp_list_cstring;
//     CString temp_cs_change_info;
//     temp_list_cstring = m_co2_external_sensor_list.GetItemText(Changed_Item,Changed_SubItem);
//     if(CheckString(temp_list_cstring)==false)
//         return 1;
//     temp_reg_value = _wtoi(temp_list_cstring);
//     if(Changed_SubItem == CO2_EXTERNAL_PPM)
//     {
//         temp_write_reg = CO2_485_MODBUS_EXTERNAL_CO2_PPM_START + Changed_Item;
//         temp_cs_change_info.Format(_T("External PPM"));
//     }
//     else if(Changed_SubItem == CO2_EXTERNAL_PRE_ALARM_SP)
//     {
//         temp_write_reg = CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START + Changed_Item;
//         temp_cs_change_info.Format(_T("External Prepare Alarm Setpoint"));
//     }
//     else if(Changed_SubItem == CO2_EXTERNAL_ALARM_SP)
//     {
//         temp_write_reg = CO2_485_MODBUS_EXT_ALARM_SETPOINT_START + Changed_Item;
//         temp_cs_change_info.Format(_T("Alarm Setpoint"));
//     }
//     else if(Changed_SubItem == CO2_EXTERNAL_CAL_OFFSET)
//     {
//         temp_write_reg = CO2_485_MODBUS_EXT_CO2_OFFSET_START + Changed_Item;
//         temp_cs_change_info.Format(_T("Calibrating Offset"));
//     }
//     else
//     {
//         return -2;
//     }
//
//     if(temp_reg_value != product_register_value[temp_write_reg])
//     {
//       //  Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,temp_write_reg,temp_reg_value,
//        //     product_register_value[temp_write_reg],this->m_hWnd,IDC_LIST_CO2_EXTERNAL_SENSOR,temp_cs_change_info);
//     }
//
//     return 0;
// }

bool CheckString( CString str )
{

    int nCount = str.GetLength(); // �����ַ�����
    for ( int i = 0; i < nCount; i ++ )
    {
        if(i==0)
        {
            if(str.GetAt(i) == '-')
                continue;
        }


        if ( 0 == isdigit( str.GetAt(i) ) ) // �������־��ñ�־λ
        {
            return FALSE;
            //break;// �˳�
        }
    }

    return TRUE;
}



void CCO2_View::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

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

    
}

void CCO2_View::OnNMKillfocusCo2Datetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
    
    *pResult = 0;
    Check_DayTime();
}

void CCO2_View::OnNMKillfocusCo2DatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult)
{
    
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
    AfxMessageBox(_T("Sync Time Successful!"));
}

void CCO2_View::OnBnClickedRadioPasswordEnable()
{
    
    if(product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE] != TRUE)
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PASSWORD_ENABLE,1,
            product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE],this->m_hWnd,IDC_RADIO_PASSWORD_ENABLE,_T("Enable Password"));
    }
}

void CCO2_View::OnBnClickedRadioPasswordDisable()
{
    
    if(product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE] != FALSE)
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PASSWORD_ENABLE,0,
            product_register_value[CO2_485_MODBUS_PASSWORD_ENABLE],this->m_hWnd,IDC_RADIO_PASSWORD_ENABLE,_T("Disable Password"));
    }
}

void CCO2_View::OnEnKillfocusEditCo2Passwor()
{
    
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
    
    UpdateData();
    if(m_co2_block_time != product_register_value[CO2_485_MODBUS_MENU_BLOCK_SECONDS])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_MENU_BLOCK_SECONDS,m_co2_block_time,
            product_register_value[CO2_485_MODBUS_MENU_BLOCK_SECONDS],this->m_hWnd,IDC_EDIT_CO2_BLOCK_TIME,_T("Menu Block Time"));
    }
}

void CCO2_View::OnEnKillfocusEditCo2BacklightTime()
{
    
    UpdateData();
    if(m_co2_backlight_time != product_register_value[CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS,m_co2_backlight_time,
            product_register_value[CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("Backlight Keep Time"));
    }
}

LRESULT CCO2_View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    
    if(MsgT3000ViewFresh==message)
    {
        if(this->IsWindowVisible())	//add by Fance ,if window not visible, it's unnecessary to fresh it;
            Fresh();
    }
    return CFormView::WindowProc(message, wParam, lParam);
}

void CCO2_View::OnBnClickedBtnCo2Refresh()
{
    

}
extern  HANDLE Read_Mute;
//DWORD WINAPI CCO2_View::StartRefresh(LPVOID lpVoid)
//{
//    CCO2_View *pParent = (CCO2_View *)lpVoid;
//
//    while(TRUE)
//    {
//		 if (b_pause_refresh_tree)
//		 {
//			Sleep(1000);
//			continue;
//		 }
//
//        if (pParent->m_product_type == 2)
//        {
//            Sleep(1000);
//            if(pParent->IsWindowVisible())
//            {
//                CString strTemp ;
//
//
//
//               bitset<16> AM(product_register_value[280]);
//
//               if (!AM[2])
//               {
//                  Read_Multi(g_tstat_id,&product_register_value[710],710,10);
//                  pParent->SendMessage(WM_REFRESH_BAC_INPUT_LIST,1,0);
//               }
//               else
//               {
//                 //  Read_Multi(g_tstat_id,&product_register_value[280],280,10);
//               }
//
//
//
//
//
////                 bitset<16> AM(product_register_value[280]);
////                 int Input_Value = -1;
////                 if (!AM[2])
////                 {
////                     pParent->m_input_list.SetItemText(0,2,Global_String_AUTO);
////                     Input_Value = (short)product_register_value[711] ;
////                 }
////                 else
////                 {
////                     pParent->m_input_list.SetItemText(0,2,Global_String_MANUAL);
////                     Input_Value = (short)product_register_value[283] ;
////                 }
////
////                 unsigned char default_unit = product_register_value[702];
////                 unsigned char current_unit = product_register_value[701];
////                 float input_rate   = pParent->get_units_ratio(current_unit,default_unit);
////                 float input_value = 0;
////                 int decimal_x = 0;
////                 if (default_unit == 0)
////                 {
////                     decimal_x = 0 ;
////                 }
////                 else
////                 {
////                     decimal_x = 1;
////                 }
////                 int decimal_number = decimal_num[decimal_x][current_unit];
////                 if (default_unit == 0)
////                 {
////                     input_value=input_rate*Input_Value ;
////                     input_value =input_value/100;
////                 }
////                 else
////                 {
////                     input_value=input_rate*Input_Value ;
////                     input_value =input_value/10;
////                 }
////                 CString dataformat;
////                 dataformat.Format(_T("%%0.%df"),decimal_number);
////                 strTemp.Format(dataformat,input_value);
////                  pParent->m_input_list.SetItemText(0,3,strTemp);
//            }
//            continue;
//        }
//        if (pParent->m_product_type == 3)
//        {
//            Sleep(1000);
//            if(pParent->IsWindowVisible())
//            {
//                if (!is_connect()||g_bPauseMultiRead)
//                {
//                    Sleep(1000);
//                    continue;
//                }
//                CString strTemp ;
//                Read_Multi(g_tstat_id,&product_register_value[100],100,50);
//                pParent->m_list_type = 0;
//                pParent->SendMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
//            }
//            continue;
//        }
//        if (!is_connect()||g_bPauseMultiRead)
//        {
//            Sleep(1000);
//            continue;
//        }
//        if (!pParent->m_fresh_Grid)
//        {
//            Sleep(2000);
//            continue;
//        }
//
//        if(!no_mouse_keyboard_event_enable_refresh)
//        {
//            Sleep(3000);
//            continue ;
//        }
//        else
//        {
//            Sleep(2000);
//        }
//
//
//
//
//        int read_ret=0;
//        register_critical_section.Lock();
//        for(int i=0;i<7;i++)
//        {
//            if(!no_mouse_keyboard_event_enable_refresh)
//            {break ;}
//            if (!pParent->m_fresh_Grid)
//            {
//                break ;
//            }
//            if(Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100)>0)
//                read_ret++;
//        }
//		Read_Multi(g_tstat_id,pParent->ValueData,10000,10);
//        register_critical_section.Unlock();
//        if(!no_mouse_keyboard_event_enable_refresh)
//        {continue ;}
//        if (!pParent->m_fresh_Grid)
//        {
//
//            continue;
//        }
//        if(read_ret<7)
//            pParent->SendMessage(WM_REFRESH_CO2_DLG,REFRESH_FAIL,0);
//
//        if(pParent->IsWindowVisible())
//        {
//            pParent->PostMessage(WM_REFRESH_CO2_DLG,REFRESH_DLG,0);
//            pParent->PostMessage(WM_REFRESH_CO2_DLG,ENABLE_REFRESH_BUTTON,0);
//
//            pParent->PostMessage(WM_REFRESH_BAC_INPUT_LIST,1,0);
//        }
//        else
//        {
//
//        }
//
//
//
//    }
//
//    /*pParent->RefreshThread=NULL;*/
//    return 0;
//
//}

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
    
    UpdateData();
    if(m_alarm_on_time != product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME,m_alarm_on_time,
            product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("User Defined Alarm On Time"));
    }

}

void CCO2_View::OnEnKillfocusCo2AlarmOffTime()
{
    
    UpdateData();
    if(m_alarm_off_time != product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME])
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME,m_alarm_off_time,
            product_register_value[CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME],this->m_hWnd,IDC_EDIT_CO2_BACKLIGHT_TIME,_T("User Defined Alarm Off Time"));
    }
}

// void CCO2_View::OnBnClickedBtnCo2ClearCal()
// {
//
// }

void CCO2_View::OnBnClickedRadioHumidityHeatEnable()
{
    
    if(product_register_value[CO2_485_MODBUS_HUM_SENSOR_HEATING] != TRUE)
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,CO2_485_MODBUS_HUM_SENSOR_HEATING,1,
            product_register_value[CO2_485_MODBUS_HUM_SENSOR_HEATING],this->m_hWnd,IDC_RADIO_HUMIDITY_HEAT_ENABLE,_T("Humidity Sensor Heating"));
    }
}

void CCO2_View::OnBnClickedRadioHumidityHeatDisable()
{
    
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
            //AfxMessageBox(_T("Your Firmware is old,please update it!"));
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
        CO2_485_MODBUS_SCAN_END                 =  509;

        CO2_485_MODBUS_GET_NODES_PARA_START   =510;
        CO2_485_MODBUS_GET_NODES_PARA_END     = 541;
        CO2_485_MODBUS_SCAN_OCCUPY_START  = 542;
        CO2_485_MODBUS_SCAN_OCCUPY_END   = 573  ;
        CO2_485_MODBUS_SCAN_ONLINE_START    = 574                           ;
        CO2_485_MODBUS_SCAN_ONLINE_END      = 605                           ;

        CO2_485_MODBUS_PRESSURE_SENSOR_MODEL        = 700                  ;
        CO2_485_MODBUS_PRESSURE_UNIT            = 701                      ;
        CO2_485_MODBUS_PRESSURE_UNIT_DEFAULT         = 702                 ;
        CO2_485_MODBUS_OUTPUT_RANGE_MIN_PRESSURE          = 703            ;
        CO2_485_MODBUS_OUTPUT_RANGE_MAX_PRESSURE           = 704           ;
        CO2_485_MODBUS_PRESSURE_FILTER                     = 705           ;
        CO2_485_MODBUS_PRESSURE_SLOPE                    = 706             ;
        CO2_485_MODBUS_PRESSURE_INTERCEPT               = 707              ;
        CO2_485_MODBUS_INPUT_AUTO_MANUAL_PRE           = 708               ;
        CO2_485_MODBUS_INPUTPUT_MANUAL_VALUE_PRE       = 709               ;
        CO2_485_MODBUS_PREESURE_AD                      = 710              ;
        CO2_485_MODBUS_PRESSURE_VALUE_ORG              = 711               ;
        CO2_485_MODBUS_PRESSURE_VALUE_ORG_OFFSET       = 712               ;
        CO2_485_MODBUS_PRESSURE_VALUE_INDEX           = 713                ;
        CO2_485_MODBUS_PRESSURE_VALUE_BASE_L           = 714               ;
        CO2_485_MODBUS_PRESSURE_VALUE_BASE_H           = 715               ;
        CO2_485_MODBUS_PRESSURE_CAL_POINT             = 716                ;
        CO2_485_MODBUS_PRESSURE_CAL_PR0                = 717               ;
        CO2_485_MODBUS_PRESSURE_CAL_AD0               =718                ;
        CO2_485_MODBUS_PRESSURE_CAL_PR1                   = 719            ;
        CO2_485_MODBUS_PRESSURE_CAL_AD1                 = 720              ;
        CO2_485_MODBUS_PRESSURE_CAL_PR2               = 721                ;
        CO2_485_MODBUS_PRESSURE_CAL_AD2              = 722                 ;
        CO2_485_MODBUS_PRESSURE_CAL_PR3              = 723                 ;
        CO2_485_MODBUS_PRESSURE_CAL_AD3             = 724                  ;
        CO2_485_MODBUS_PRESSURE_CAL_PR4              = 725                 ;
        CO2_485_MODBUS_PRESSURE_CAL_AD4             = 726                  ;
        CO2_485_MODBUS_PRESSURE_CAL_PR5           = 727                    ;
        CO2_485_MODBUS_PRESSURE_CAL_AD5          = 728                     ;
        CO2_485_MODBUS_PRESSURE_CAL_PR6            = 729                   ;
        CO2_485_MODBUS_PRESSURE_CAL_AD6           = 730                    ;
        CO2_485_MODBUS_PRESSURE_CAL_PR7           = 731                    ;
        CO2_485_MODBUS_PRESSURE_CAL_AD7           = 732                    ;
        CO2_485_MODBUS_PRESSURE_CAL_PR8           = 733                    ;
        CO2_485_MODBUS_PRESSURE_CAL_AD8           = 734                    ;
        CO2_485_MODBUS_PRESSURE_CAL_PR9           = 735                    ;
        CO2_485_MODBUS_PRESSURE_CAL_AD9           = 736                    ;
        CO2_485_MODBUS_TABLE_SEL                    = 737                  ;
        CO2_485_MODBUS_USER_CAL_POINT               = 738                  ;
        CO2_485_MODBUS_USER_CAL_PR0                  = 739                 ;
        CO2_485_MODBUS_USER_CAL_AD0                 = 740                  ;
        CO2_485_MODBUS_USER_CAL_PR9                = 757                   ;
        CO2_485_MODBUS_USER_CAL_AD9                 = 758                  ;
        CO2_485_MODBUS_PRESSURE_VALUE_AD            =800                  ;
        CO2_485_MODBUS_PRESSURE_TEMPER_AD           =801                  ;
        CO2_485_MODBUS_PRESSURE_VALUE               =802                  ;
        CO2_485_MODBUS_PRESSURE_TEMPER              =803                  ;
        CO2_485_MODBUS_PRESSURE_STATUS              =804                  ;
        CO2_485_MODBUS_PRESSURE_READ_ENABLE         =805                  ;
        CO2_485_MODBUS_PRESSURE_READ_ALL            =806                  ;
        CO2_485_MODBUS_PRESSURE_READ_ONLY           =807                  ;
        CO2_485_MODBUS_PRESSURE_OFFSET              =808                  ;
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
    lRow = m_grid_input.get_RowSel();//��ȡ�������к�
    lCol = m_grid_input.get_ColSel(); //��ȡ�������к�
    if(lRow>m_grid_input.get_Rows()) //�������������������кţ�����������Ч��
        return;
    if(lRow == 0) //�������������У�Ҳ��Ч
        return;
    CRect rect;
    m_grid_input.GetWindowRect(rect); //��ȡ�����ؼ��Ĵ��ھ���
    ScreenToClient(rect);
    //ת��Ϊ�ͻ�������
    // MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"��(twips)"��
    //��Ҫ����ת��Ϊ���أ�1440��= 1Ӣ��
    CDC* pDC =GetDC();
    //�������ص���羵�ת������
    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    //����ѡ�и������Ͻǵ�����(����Ϊ��λ)
    long y = m_grid_input.get_RowPos(lRow)/nTwipsPerDotY;
    long x = m_grid_input.get_ColPos(lCol)/nTwipsPerDotX;
    //����ѡ�и��ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
    long width = m_grid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
    long height = m_grid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
    //�γ�ѡ�и����ڵľ�������
    CRect rc(x,y,x+width,y+height);
    //ת�������ԶԻ���������
    rc.OffsetRect(rect.left+1,rect.top+1);
    //��ȡѡ�и����ı���Ϣ
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

        m_co2_temp_unit.MoveWindow(rc);			//�ƶ���ѡ�и���λ�ã�����
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

        m_co2_temp_unit.MoveWindow(rc);			//�ƶ���ѡ�и���λ�ã�����
        m_co2_temp_unit.BringWindowToTop();
        m_co2_temp_unit.SelectString(-1,strValue);
        m_co2_temp_unit.SetFocus();
    }
    if (lCol==4)
    {
        if (/*Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow)*/TRUE)//Manual
        {

            m_co2_temp_unit.ShowWindow(SW_HIDE);
            m_upButton.ShowWindow(SW_HIDE);
            m_downButton.ShowWindow(SW_HIDE);
            m_inValueEdit.ShowWindow(SW_SHOW);
            m_inValueEdit.MoveWindow(rc); //�ƶ���ѡ�и���λ�ã�����
            m_inValueEdit.ShowWindow(SW_SHOW);

            m_inValueEdit.BringWindowToTop();
            //m_RangCombox.SelectString(-1,strValue);
            m_inValueEdit.SetWindowText(strValue);
            m_inValueEdit.SetFocus(); //��ȡ����
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
        //if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//�ڲ�
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
//             if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//�ڲ�
//             {
//                 RegAdd=RegAdd_IN;
//             }
//             else
//             {
                RegAdd=RegAdd_Ex;
          //  }
        }
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAdd,m_value,
            product_register_value[RegAdd],this->m_hWnd,IDC_EDIT_VALUE_GRID,_T("Calibrate Value"));

    }
    if (m_nCurRow==2)
    {
        unsigned short m_value=(unsigned short)(_wtof(TempValue)*10);
        if (Get_Bit_FromRegister(product_register_value[CO2_485_MODBUS_OUTPUT_AUTO_MANUAL],m_nCurRow))
        {


			 RegAdd_IN=CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM;
        }
        else
        {
             RegAdd_IN=CO2_485_MODBUS_HUMIDITY_RH;
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

   // C02_SHOW_TEMP();

	m_inValueEdit.ShowWindow(SW_HIDE);

}

void CCO2_View::OnLButtonDown(CPoint point)
{


    int RegAdd_IN,RegAdd_Ex;
    CRect lRect;
    m_downButton.GetWindowRect(lRect); //��ȡ�����ؼ��Ĵ��ھ���
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
            if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//�ڲ�
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
            if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//�ڲ�
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
//	
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
        if(product_register_value[CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT] == 0)//�ڲ�
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
void CCO2_View::OnCbnSelchangeCo2Baudratecombo()
{
    g_bPauseMultiRead = TRUE;
	int temp_write_ret = 0;

	int RegAddress = CO2_485_MODBUS_BAUDRATE;
	if (product_register_value[7] == PM_CO2_RS485)
	{
		RegAddress = CO2_485_MODBUS_BAUDRATE;
	}
	else
	{
		RegAddress = 15;
	}
   temp_write_ret = write_one(g_tstat_id, RegAddress, m_co2_baudRateCombox.GetCurSel());
   if(temp_write_ret < 0)	//��ʱ���ж������⣬�豸���Լ����в������ڻأ����»ز�����.
   {
	   MessageBox(_T("Write timeout."));
	   return;
   }
   else
   {
	   SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change baudrate success!"));
   }
   // MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
    /*else
    product_register_value[CO2_485_MODBUS_BAUDRATE] = m_co2_baudRateCombox.GetCurSel();*/
    CString SqlText;
    if (m_co2_baudRateCombox.GetCurSel()==0)
    {
        SqlText.Format(_T("update ALL_NODE set Bautrate = '9600' where Serial_ID='%d'"),get_serialnumber());
         Change_BaudRate(9600);
    }
    else
    {
        SqlText.Format(_T("update ALL_NODE set Bautrate = '19200' where Serial_ID='%d'"),get_serialnumber());
         Change_BaudRate(19200);
    }
	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	SqliteDBBuilding.execDML((UTF8MBSTR)SqlText);

    SqliteDBBuilding.closedb();

	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);


   // pFrame->ScanTstatInDB();
    g_bPauseMultiRead = FALSE;
}


void CCO2_View::Initial_InputList(){
  if (m_product_type != 3)
  {
      int AddressValue = -1;
      CString strTemp;
      m_input_list.ShowWindow(SW_HIDE);
      m_input_list.DeleteAllItems();
      while ( m_input_list.DeleteColumn (0)) ;

      m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
      m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
      m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
      m_input_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(4, _T("Range"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(5, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

      g_hwnd_now = this->m_hWnd;
      m_input_list.InsertItem(0,_T("1"));
      m_input_list.SetItemText(0,1,_T("Pressure"));

      if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(4))
      {
          ListCtrlEx::CStrList strlist;
          for (int i=0;i<(int)sizeof(Pressure_Units)/sizeof(Pressure_Units[0]);i++)
          {
              strlist.push_back(Pressure_Units[i]);
          }
          m_input_list.SetCellStringList(0, 4, strlist);
      }


      bitset<16> AM(product_register_value[280]);
      int Input_Value = -1;
      if (!AM[2])
      {
          m_input_list.SetItemText(0,2,Global_String_AUTO);
          Input_Value = (short)product_register_value[711] ;
      }
      else
      {
          m_input_list.SetItemText(0,2,Global_String_MANUAL);
          Input_Value = (short)product_register_value[283] ;
      }

      unsigned char default_unit = product_register_value[702];
      unsigned char current_unit = product_register_value[701];
      float input_rate   = get_units_ratio(current_unit,default_unit);
      float input_value = 0;
      int decimal_x = 0;
      if (default_unit == 0)
      {
          decimal_x = 0 ;
      }
      else
      {
          decimal_x = 1;
      }
      int decimal_number = decimal_num[decimal_x][current_unit];
      if (default_unit == 0)
      {
          input_value=input_rate*Input_Value ;
          input_value =input_value/100;
      }
      else
      {
          input_value=input_rate*Input_Value ;
          input_value =input_value/10;
      }
      CString dataformat;
      dataformat.Format(_T("%%0.%df"),decimal_number);
      strTemp.Format(dataformat,input_value);
      m_input_list.SetItemText(0,3,strTemp);

      strTemp = Pressure_Units[current_unit];
      m_input_list.SetItemText(0,4,strTemp);
      strTemp.Format(_T("%d"),product_register_value[705]);
      m_input_list.SetItemText(0,5,strTemp);
      m_input_list.ShowWindow(SW_SHOW);
  }

  if (m_product_type == 3)
  {
      int AddressValue = -1;
      CString strTemp;
      //m_input_list.ShowWindow(SW_HIDE);
      m_input_list.DeleteAllItems();
      while ( m_input_list.DeleteColumn (0)) ;

      m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
      m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
      m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
      m_input_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(2, _T("Value"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(3, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_input_list.InsertColumn(4, _T("Calibration"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

      g_hwnd_now = this->m_hWnd;
      m_input_list.InsertItem(0,_T("1"));
      m_input_list.InsertItem(1,_T("2"));
      m_input_list.InsertItem(2,_T("3"));

      m_input_list.SetItemText(0,1,_T("Temperature"));
      m_input_list.SetItemText(1,1,_T("Hum"));
      m_input_list.SetItemText(2,1,_T("CO2"));








      int i_internal_temp = 0;
      int i_external_temp = 0;
      float f_internal_temp = 0;
      float f_external_temp = 0;
      CString temp_internal_value,temp_external_value;



      CString strTemp1,strTemp2,strUnit,strHUM,strCO2;
      strTemp1.Format(_T("%cC"),176);
      strTemp2.Format(_T("%cF"),176);

      if(product_register_value[125] == 0)
      {
          i_internal_temp = product_register_value[119];
          i_external_temp = product_register_value[121];

          strUnit=strTemp1;
      }
      else if((product_register_value[125] == 1))
      {
          i_internal_temp = product_register_value[120];
          i_external_temp = product_register_value[122];

          strUnit=strTemp2;
      }
      else
      {
          /*return;*/
      }
      f_internal_temp = (float)i_internal_temp / 10;
      f_external_temp = (float)i_external_temp / 10;
      CString  TempValue,StrAM;

	  /// <summary>
	  /// Here ,according to 124 value ,to read different register value.
	  /// </summary>
      if(product_register_value[124] == 0)
      {
          TempValue.Format(_T("%0.1f"),f_internal_temp);
      }
      else
      {
          TempValue.Format(_T("%0.1f"),f_external_temp);
      }

      m_input_list.SetItemText(0,2,TempValue);
      strHUM.Format(_T("%0.1f"),(float)(product_register_value[116])/10.0);

       m_input_list.SetItemText(1,2,strHUM);
       strCO2.Format(_T("%d"),product_register_value[108]);

       m_input_list.SetItemText(2,2,strCO2);

        m_input_list.SetItemText(0,3,strUnit);
        m_input_list.SetItemText(1,3,_T("%"));
        m_input_list.SetItemText(2,3,_T("ppm"));

        m_input_list.SetItemText(0,4,_T("UNUSED"));
        m_input_list.SetItemText(1,4,_T("UNUSED"));
        strTemp.Format (_T("%d"),product_register_value[111]);
        m_input_list.SetItemText(2,4,strTemp);

  }

}

void CCO2_View::Initial_OutputList(){
if (m_product_type == 1)
{
	int AddressValue = -1;
	CString strTemp;
	float flValue = 0;
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
	m_output_list.InsertColumn(6, _T("Unit"), 85, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

	m_output_list.InsertItem(0,_T("1"));
	m_output_list.InsertItem(1,_T("2"));
	m_output_list.InsertItem(2,_T("3"));

	m_output_list.SetItemText(0,1,_T("Hum"));
	m_output_list.SetItemText(1,1,_T("Temperature"));
	m_output_list.SetItemText(2,1,_T("CO2"));

	AddressValue =284 ;
	int output_range=product_register_value[AddressValue];
	if (output_range==3){
		strTemp=_T("0-10v");
	}
	else if (output_range==2)
	{
		strTemp=_T("0-5v");
	}
	else if (output_range==1)
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
	float FloatValue = 0;

	if (output_range == 1)
	{
		AddressValue =10003 ;
		flValue = (((float)((short)ValueData[AddressValue-10000]))/1024)*25;
		strTemp.Format (_T("%0.2f"),flValue);
		m_output_list.SetItemText(0,2,strTemp);

		AddressValue =10004 ;
		flValue = (((float)((short)ValueData[AddressValue-10000]))/1024)*25;
		strTemp.Format (_T("%0.2f"),flValue);
		m_output_list.SetItemText(1,2,strTemp);

		AddressValue =10005 ;
		flValue = (((float)((short)ValueData[AddressValue-10000]))/1024)*25;
		strTemp.Format (_T("%0.2f"),flValue);
		m_output_list.SetItemText(2,2,strTemp);
	}
	else
	{
		AddressValue =10000 ;
		  FloatValue = (short)ValueData[AddressValue-10000];
		flValue = (FloatValue/1024)*10;
		strTemp.Format (_T("%0.2f"),flValue);
		m_output_list.SetItemText(0,2,strTemp);

		AddressValue =10001 ;
		FloatValue = (short)ValueData[AddressValue-10000];
		flValue = (FloatValue/1024)*10;

		strTemp.Format (_T("%0.2f"),flValue);
		m_output_list.SetItemText(1,2,strTemp);

		AddressValue =10002 ;
		FloatValue = (short)ValueData[AddressValue-10000];
		flValue = (FloatValue/1024)*10;
		strTemp.Format (_T("%0.2f"),flValue);
		m_output_list.SetItemText(2,2,strTemp);
	}






	FloatValue = (short)product_register_value[287];
	flValue = (FloatValue)/10;
	strTemp.Format (_T("%0.1f"),flValue);
	m_output_list.SetItemText(0,4,strTemp);



	FloatValue = (short)product_register_value[288];
	flValue = (FloatValue)/10;
	strTemp.Format (_T("%0.1f"),flValue);
	m_output_list.SetItemText(0,5,strTemp);


	FloatValue = (short)product_register_value[285];
	flValue = (FloatValue)/10;
	strTemp.Format (_T("%0.1f"),flValue);

	m_output_list.SetItemText(1,4,strTemp);


	FloatValue = (short)product_register_value[286];
	flValue = (FloatValue)/10;
	strTemp.Format (_T("%0.1f"),flValue);

	m_output_list.SetItemText(1,5,strTemp);

	strTemp.Format (_T("%d"),product_register_value[289]);
	m_output_list.SetItemText(2,4,strTemp);
	strTemp.Format (_T("%d"),product_register_value[290]);
	m_output_list.SetItemText(2,5,strTemp);



	CString strTemp1,strTemp2,strUnit,strHUM,strCO2;
	strTemp1.Format(_T("%cC"),176);
	strTemp2.Format(_T("%cF"),176);

	if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)
	{


		strUnit=strTemp1;
	}
	else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
	{


		strUnit=strTemp2;
	}
	m_output_list.SetItemText(1,6,strUnit);
	strTemp = L"%";

	m_output_list.SetItemText(0,6,strTemp);

	strTemp = L"ppm";
	m_output_list.SetItemText(2,6,strTemp);
	m_output_list.ShowWindow (SW_SHOW);
}
else if (m_product_type == 3)
{
    int AddressValue = -1;
    CString strTemp;

    m_output_list.DeleteAllItems();
    while ( m_output_list.DeleteColumn (0)) ;

    m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
    m_output_list.InsertColumn(0, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_output_list.InsertColumn(1, _T("Full Label"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

    m_output_list.InsertColumn(2, _T("Range"), 45, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_output_list.InsertColumn(3, _T("Min Out Scale"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_output_list.InsertColumn(4, _T("Max Out Scale"), 85, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(5, _T("Unit"), 85, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

    m_output_list.InsertItem(0,_T("1"));
    m_output_list.InsertItem(1,_T("2"));
    m_output_list.InsertItem(2,_T("3"));
    m_output_list.SetItemText(0,1,_T("Temperature"));
    m_output_list.SetItemText(1,1,_T("Hum"));
    m_output_list.SetItemText(2,1,_T("CO2"));

    AddressValue =127 ;


    int output_range=product_register_value[AddressValue];
    if (output_range==3){
        strTemp=_T("0-10v");
    }
    else if (output_range==2)
    {
        strTemp=_T("0-5v");
    }
    else if (output_range==1)
    {
        strTemp=_T("4-20mA");
    }
    else
    {
        strTemp=_T("");
    }
    m_output_list.SetItemText(0,2,strTemp);
    m_output_list.SetItemText(1,2,strTemp);
    m_output_list.SetItemText(2,2,strTemp);

	float tc =(float)((short) product_register_value[128]);
	if (product_register_value[125] == 0)
	{
		strTemp.Format(_T("%.1f"),tc/10 );
	}
	else
	{
		strTemp.Format(_T("%.1f"), CTwoF(tc/10));
	}

   
    m_output_list.SetItemText(0,3,strTemp);

	tc = (float)((short)product_register_value[129]);
	if (product_register_value[125] == 0)
	{
		strTemp.Format(_T("%.1f"), tc / 10);
	}
	else
	{
		strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
	}

  
    m_output_list.SetItemText(0,4,strTemp);

	tc = (float)((short)product_register_value[130]);
	if (product_register_value[125] == 0)
	{
		strTemp.Format(_T("%.1f"), tc / 10);
	}
	else
	{
		strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
	}
    
    m_output_list.SetItemText(1,3,strTemp);
	tc = (float)((short)product_register_value[131]);
	if (product_register_value[125] == 0)
	{
		strTemp.Format(_T("%.1f"), tc / 10);
	}
	else
	{
		strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
	}
    m_output_list.SetItemText(1,4,strTemp);
	tc = (float)((short)product_register_value[132]);
	if (product_register_value[125] == 0)
	{
		strTemp.Format(_T("%.1f"), tc / 10);
	}
	else
	{
		strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
	}
    m_output_list.SetItemText(2,3,strTemp);
	tc = (float)((short)product_register_value[133]);
	if (product_register_value[125] == 0)
	{
		strTemp.Format(_T("%.1f"), tc / 10);
	}
	else
	{
		strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
	}
    m_output_list.SetItemText(2,4,strTemp);



	CString strTemp1, strTemp2, strUnit, strHUM, strCO2;
	strTemp1.Format(_T("%cC"), 176);
	strTemp2.Format(_T("%cF"), 176);

	if (product_register_value[125] == 0)
	{


		strUnit = strTemp1;
	}
	else if (product_register_value[125] == 1)
	{


		strUnit = strTemp2;
	}
	m_output_list.SetItemText(0, 5, strUnit);
	strTemp = L"%";

	m_output_list.SetItemText(1, 5, strTemp);

	strTemp = L"ppm";
	m_output_list.SetItemText(2, 5, strTemp);

    m_output_list.ShowWindow (SW_SHOW);

}
else
{
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
    g_hwnd_now = this->m_hWnd;
    m_output_list.InsertItem(0,_T("1"));
    m_output_list.SetItemText(0,1,_T("Pressure"));
    //Range
    AddressValue =284 ;


    int output_range=product_register_value[AddressValue];
    if (output_range==3){
        strTemp=_T("0-10v");
    }
    else if (output_range==2)
    {
        strTemp=_T("0-5v");
    }
    else if (output_range==1)
    {
        strTemp=_T("4-20mA");
    }
    else
    {
        strTemp=_T("");
    }
    m_output_list.SetItemText(0,3,strTemp);

    //Value
    AddressValue =280;
    int ret = read_one(g_tstat_id,1002);
        if (ret>-1)
        {
            product_register_value[1002] = ret;
        }
          ret = read_one(g_tstat_id,1005);
        if (ret>-1)
        {
            product_register_value[1005] = ret;
        }


        if (output_range==1||output_range==2)
        {
            strTemp.Format(_T("%.2f v"),((float)((short)product_register_value[1002]))*1.024);
        }
        else if (output_range==3)
        {

            strTemp.Format(_T("%.2f ma"),((float)((short)product_register_value[1005]))*2.5*1.024);
        }
        else{
            strTemp=_T("0");
        }

    m_output_list.SetItemText(0,2,strTemp);

    //Min out scale
    AddressValue =289 ;
    strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[AddressValue]/10.0);
    m_output_list.SetItemText(0,4,strTemp);
    //Max out scale
    AddressValue =290 ;
    strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[AddressValue]/10.0);
    m_output_list.SetItemText(0,5,strTemp);

    m_output_list.ShowWindow(SW_SHOW);
    }
}

float CCO2_View::CTwoF(float tc)
{
	return (((tc * 9) / 5) + 320) / 10.0;
}

float CCO2_View::FTwoC(float tf)
{
	return (5 * (10 * tf - 320)) / 9;
}
// *************************************************************************************
// 	input: input the unit type
//	output: get the  ratio
//  get the ratio on the mmhg and the ratio by 10^7.
// *************************************************************************************
float CCO2_View::get_mmhg_ratio(unsigned char unit)  //rate = unit/mmhg*10^7
{

    float Ftemp;  //10^7

    switch (unit)
    {
    case P_inWC:         //W.C				0.01		 0
        Ftemp = 5353000;
        break;
    case P_KPa:          //KPa 			0.0001		 0
        Ftemp = 1332800;
        break;
    case P_Psi:          //Psi				0.00001		0.1
        Ftemp = 193400;
        break;
    case P_mmHg:         //mmHg			0.01		0
        Ftemp = 10000000;
        break;
    case P_inHg:         //inches of Hg	0.00001		0.01
        Ftemp = 393700;
        break;
    case P_Kg_cm2:       //Kg/cm2			0.000001	0.001
        Ftemp = 13595;
        break;
    case P_atmosphere:   //atmosphere		0.000001	0.001
        Ftemp = 13158;
        break;
    case P_bar:          //bar				0.000001	0.001
        Ftemp = 13328;
        break;

    }
    return Ftemp;
}

// *************************************************************************************
// 	input: new unit   default unit
//	output: get the  ratio between the two unit
// *************************************************************************************
float CCO2_View::get_units_ratio(unsigned char new_unit,unsigned char default_unit)
{
    float Ftemp;
    Ftemp = get_mmhg_ratio(new_unit)/get_mmhg_ratio(default_unit);
    return Ftemp;
}

LRESULT CCO2_View::Change_Item_List(WPARAM wParam,LPARAM lParam){

    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
    int AddressValue = -1;
    CString temp_task_info;
     int RegAddress_INT,RegAddress_EXT,RegAddress;
      if (m_list_type == 0)
    {
        CString New_CString =  m_input_list.GetItemText(Changed_Item,Changed_SubItem);
        CString cstemp_value;

        if (m_product_type ==3)
        {
              if (Changed_SubItem ==2)
              {

                  if (Changed_Item == 0)
                  {


                      if(product_register_value[125] == 0)
                      {
                          RegAddress_INT = 119;
                          RegAddress_EXT = 121;


                      }
                      else if((product_register_value[125] == 1))
                      {
                          RegAddress_INT  =  120 ;
                          RegAddress_EXT  =  122 ;


                      }
                      else
                      {
                          /*return;*/
                      }


                      if(product_register_value[124] == 0)//�ڲ�
                      {
                         RegAddress = RegAddress_INT;
                      }
                      else
                      {
                         RegAddress = RegAddress_EXT;
                      }

                      int ItemValue = (int)(_wtof (New_CString) * 10);
                      if (product_register_value[RegAddress]!=ItemValue)
                      {
                          int ret = write_one(g_tstat_id,RegAddress,ItemValue);
                          if (ret> 0)
                          {
                              product_register_value[RegAddress]=ItemValue;
                              PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                          }
                      }
                  }
                 /* m_input_list.SetItemText(0,2,TempValue);
                  strHUM.Format(_T("%0.1f"),(float)(product_register_value[116])/10.0);

                  m_input_list.SetItemText(1,2,strHUM);
                  strCO2.Format(_T("%d"),product_register_value[108]);

                  m_input_list.SetItemText(2,2,strCO2);*/
                  if (Changed_Item == 1)
                  {
                      int ItemValue = (int)(_wtof (New_CString) * 10);
                      RegAddress = 116;
                      if (product_register_value[RegAddress]!=ItemValue)
                      {
                          int ret = write_one(g_tstat_id,RegAddress,ItemValue);
                          if (ret> 0)
                          {
                              product_register_value[RegAddress]=ItemValue;
                              PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                          }
                      }
                  }
                  if (Changed_Item == 2)
                  {
                      int ItemValue = _wtoi (New_CString) ;
                      RegAddress = 108;
                      if (product_register_value[RegAddress]!=ItemValue)
                      {
                          int ret = write_one(g_tstat_id,RegAddress,ItemValue);
                          if (ret> 0)
                          {
                              product_register_value[RegAddress]=ItemValue;
                              PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                          }
                      }
                  }

              }

              if (Changed_SubItem == 4)
              {
                if (Changed_Item == 2)
                {
                    int ItemValue = _wtoi (New_CString) ;
                    RegAddress = 111;
                    if (product_register_value[RegAddress]!=ItemValue)
                    {
                        int ret = write_one(g_tstat_id,RegAddress,ItemValue);
                        if (ret> 0)
                        {
                            product_register_value[RegAddress]=ItemValue;
                            PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                        }
                    }
                }
                else
                {
                    m_input_list.Set_Edit (false);
                    return 0;
                }
              }
        }
        else
        {
            //Value
            if (Changed_SubItem == 3)//Value
            {


                unsigned char default_unit = product_register_value[702];
                unsigned char current_unit = product_register_value[701];
                float input_rate   = get_units_ratio(current_unit,default_unit);
                float f_input_value = 0;
                int   Input_Value = 0;


                float  ItemValue = _wtof(New_CString);

                if (default_unit == 0)
                {
                    f_input_value =ItemValue*100;
                    Input_Value=(int)(f_input_value/input_rate);
                }
                else
                {
                    f_input_value =ItemValue*10;
                    Input_Value=(int)(f_input_value/input_rate);
                }

                if (Changed_Item == 0)
                {
                    AddressValue =  280;
                    bitset<16> AM(product_register_value[AddressValue]);
                    //if (!AM[2])//Auto
                    //{
                    //    m_input_list.Set_Edit(false);
                    //    return 0;
                    //    AddressValue =711;

                    //    if (product_register_value[AddressValue]!=Input_Value)
                    //    {
                    //        int ret = write_one(g_tstat_id,AddressValue,Input_Value);
                    //        if (ret> 0)
                    //        {
                    //            product_register_value[AddressValue]=Input_Value;
                    //        }
                    //    }


                    //}
                    if (AM[2])//Manual
                    {
                        if (Changed_Item == 0)
                        {
                            AddressValue =283;
                            if (product_register_value[AddressValue]!=Input_Value)
                            {
                                int ret = write_one(g_tstat_id,AddressValue,Input_Value);
                                if (ret> 0)
                                {
                                    product_register_value[AddressValue]=Input_Value;
                                }
                            }
                        }
                    }
                    else
                    {
                        m_input_list.Set_Edit(false) ;
                        return 0;
                    }
                }

            }
            //Filter
            if (Changed_SubItem == 5)
            {
                int ItemValue = _wtoi(New_CString);
                if (Changed_Item == 0)
                {
                    AddressValue =705;
                    if (product_register_value[AddressValue]!=ItemValue)
                    {
                        int ret = write_one(g_tstat_id,AddressValue,ItemValue);
                        if (ret> 0)
                        {
                            product_register_value[AddressValue]=ItemValue;
                        }
                    }

                }
            }
            //Range
            int Int_NewRange = 0;
            if (Changed_SubItem == 4)
            {
                for (int i=0;i<(int)sizeof(Pressure_Units)/sizeof(Pressure_Units[0]);i++)
                {
                    if (New_CString.CompareNoCase(Pressure_Units[i]) == 0)
                    {
                        Int_NewRange = i;
                        break;
                    }

                }
                unsigned char current_unit = product_register_value[701];
                if (current_unit != Int_NewRange)
                {
                    int ret = write_one(g_tstat_id,701,Int_NewRange);
                    if (ret>0)
                    {
                        product_register_value[701] = Int_NewRange;
                    }
                }

            }
        }


        PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
    }
    else if(m_list_type == 1)
    {
        if (m_product_type == 3)
        {
            CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
            int ItemValue = (int)(_wtof(New_CString)*10);

			if (product_register_value[125]!=0&&Changed_Item == 0)
			{
				ItemValue = (int)(FTwoC(_wtof(New_CString)) * 10);
			}
            if (Changed_SubItem == 3)
            {
                    int RegAddress = 128 + 2*Changed_Item;
                    int RegValue = (short)product_register_value[RegAddress];
                    if (RegValue!=ItemValue)
                    {
                        int ret = write_one(g_tstat_id,RegAddress,ItemValue);
                        if (ret> 0)
                        {
                            product_register_value[RegAddress]=ItemValue;
                            PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                        }
                    }
            }
            if (Changed_SubItem == 4)
            {
                int RegAddress = 129 + 2*Changed_Item;
                int RegValue = (short)product_register_value[RegAddress];
                if (RegValue!=ItemValue)
                {
                    int ret = write_one(g_tstat_id,RegAddress,ItemValue);
                    if (ret> 0)
                    {
                        product_register_value[RegAddress]=ItemValue;
                        PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                    }
                }
            }

        }
		if (m_product_type == 1)
		{
			CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
			int ItemValue = (int)(_wtof(New_CString)*10);
			if (Changed_SubItem == 4)
			{
  				if (Changed_Item == 0)
 				{
				     RegAddress = 287;
				     int RegValue = (short)product_register_value[RegAddress];
				     if (RegValue!=ItemValue)
				     {
				     	int ret = write_one(g_tstat_id,RegAddress,ItemValue);
				     	if (ret> 0)
				     	{
				     		product_register_value[RegAddress]=ItemValue;
				     		PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
				     	}
				     }
				 }

				if (Changed_Item == 1)
				{
					RegAddress = 285;
					int RegValue = (short)product_register_value[RegAddress];
					if (RegValue!=ItemValue)
					{
						int ret = write_one(g_tstat_id,RegAddress,ItemValue);
						if (ret> 0)
						{
							product_register_value[RegAddress]=ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
						}
					}
				}

				if (Changed_Item == 2)
				{
					RegAddress = 289;
					int RegValue = (short)product_register_value[RegAddress];
					ItemValue = ItemValue/10;
					if (RegValue!=ItemValue)
					{
						int ret = write_one(g_tstat_id,RegAddress,ItemValue);
						if (ret> 0)
						{
							product_register_value[RegAddress]=ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
						}
					}
				}

			}
			if (Changed_SubItem == 5)
			{
				if (Changed_Item == 0)
				{
					RegAddress = 288;
					int RegValue = (short)product_register_value[RegAddress];
					if (RegValue!=ItemValue)
					{
						int ret = write_one(g_tstat_id,RegAddress,ItemValue);
						if (ret> 0)
						{
							product_register_value[RegAddress]=ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
						}
					}
				}
				if (Changed_Item == 1)
				{
					RegAddress = 286;
					int RegValue = (short)product_register_value[RegAddress];
					if (RegValue!=ItemValue)
					{
						int ret = write_one(g_tstat_id,RegAddress,ItemValue);
						if (ret> 0)
						{
							product_register_value[RegAddress]=ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
						}
					}
				}
				if (Changed_Item == 2)
				{
					RegAddress = 290;
					int RegValue = (short)product_register_value[RegAddress];
					ItemValue = ItemValue/10;
					if (RegValue!=ItemValue)
					{
						int ret = write_one(g_tstat_id,RegAddress,ItemValue);
						if (ret> 0)
						{
							product_register_value[RegAddress]=ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
						}
					}
				}
			}
		}
        else
        {
            CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
            int ItemValue = (int)(_wtof(New_CString)*10);

            if (Changed_SubItem == 4)
            {
                if (Changed_Item == 0)
                {
                    int RegValue = (short)product_register_value[289];
                    if (RegValue!=ItemValue)
                    {
                        int ret = write_one(g_tstat_id,289,ItemValue);
                        if (ret> 0)
                        {
                            product_register_value[289]=ItemValue;
                            PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                        }

                    }
                }

            }
            if (Changed_SubItem == 5)
            {
                if (Changed_Item == 0)
                {
                    int RegValue = (short)product_register_value[290];
                    if (RegValue!=ItemValue)
                    {
                        int ret = write_one(g_tstat_id,290,ItemValue);
                        if (ret> 0)
                        {
                            product_register_value[290]=ItemValue;
                            PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                        }
                    }
                }
            }
        }



    }
    else
    {

    }

    return 0;
}

void CCO2_View::OnNMClickList_Input(NMHDR *pNMHDR, LRESULT *pResult){
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    HWND TEMP = g_hwnd_now;
    HWND TEMP1  = this->m_hWnd;
    int AddressValue = -1;
    CString temp_cstring;
    long lRow,lCol;
    m_input_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_input_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_input_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;


    if(lRow>m_input_list.GetItemCount()) //�������������������кţ�����������Ч��
        return;
    if(lRow<0)
        return;
    CString New_CString;
    CString temp_task_info;

    CString temp1;
    CStringArray temparray;

     if (m_product_type == 3)
     {
        if (lCol == 3)
        {
            if (lRow == 0)
            {
                if (product_register_value[125] == 0)
                {
                    if (write_one (g_tstat_id,125,1)>0)
                    {
                        product_register_value[125] = 1;
                    }
                }
                else
                {
                    if (write_one (g_tstat_id,125,0)>0)
                    {
                        product_register_value[125] = 0;
                    }
                }

                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
				Initial_OutputList();
            }
            else
            {
             m_input_list.Set_Edit (false);
             return;

            }
        }
        if (lCol == 4)
        {
            if (lRow <=1)
            {
                m_input_list.Set_Edit (false);
                return;
            }
        }
     }
     else
     {
         if (lCol == 2)	//	A/M
         {
             AddressValue =280;
             bitset<16> AM(product_register_value[AddressValue]);
             if(AM[2])
             {
                 AM[2]=FALSE;
             }
             else
             {
                 AM[2]=TRUE;
             }
             int AM_Int = AM.to_ulong();
             if (product_register_value[AddressValue]!=AM_Int)
             {
                 int ret = write_one(g_tstat_id,AddressValue,AM_Int);
                 if (ret> 0)
                 {
                     product_register_value[AddressValue]=AM_Int;
                     PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                 }
             }

         }
         if (lCol == 3)
         {
             AddressValue =280;
             bitset<16> AM(product_register_value[AddressValue]);
             if(!AM[2])
             {
                 m_input_list.Set_Edit(false);
                 return ;
             }
         }
     }


    m_list_type = 0;
}
void CCO2_View::OnNMClickList_Output(NMHDR *pNMHDR, LRESULT *pResult){
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    CString temp_cstring;
    m_output_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_output_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_output_list.SubItemHitTest(&lvinfo);

    m_list_type = 1;




}
// void CCO2_View::OnNMClickList_CO2List(NMHDR *pNMHDR, LRESULT *pResult){
//     LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//
//     CString temp_cstring;
//     long lRow,lCol;
//     m_co2_external_sensor_list.Set_Edit(true);
//     DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
//     CPoint point( LOWORD(dwPos), HIWORD(dwPos));
//     m_co2_external_sensor_list.ScreenToClient(&point);
//     LVHITTESTINFO lvinfo;
//     lvinfo.pt=point;
//     lvinfo.flags=LVHT_ABOVE;
//     int nItem=m_co2_external_sensor_list.SubItemHitTest(&lvinfo);
//
//     lRow = lvinfo.iItem;
//     lCol = lvinfo.iSubItem;
//
//     if(lRow>m_co2_external_sensor_list.GetItemCount()) //�������������������кţ�����������Ч��
//         return;
//     if(lRow<0)
//         return;
//
//     m_list_type = 3;
//
// }

LRESULT CCO2_View::Fresh_Lists(WPARAM wParam,LPARAM lParam){
    CString strTemp;
     int fresh_input = (int) wParam;
    int AddressValue = -1;
     if (m_product_type !=3)
     {
         if (fresh_input == 1)
         {
             bitset<16> AM(product_register_value[280]);
             int Input_Value = -1;
             if (!AM[2])
             {
                 m_input_list.SetItemText(0,2,Global_String_AUTO);
                 Input_Value = (short)product_register_value[711] ;
             }
             else
             {
                 m_input_list.SetItemText(0,2,Global_String_MANUAL);
                 Input_Value = (short)product_register_value[283] ;
             }

             unsigned char default_unit = product_register_value[702];
             unsigned char current_unit = product_register_value[701];
             float input_rate   = get_units_ratio(current_unit,default_unit);
             float input_value = 0;
             int decimal_x = 0;
             if (default_unit == 0)
             {
                 decimal_x = 0 ;
             }
             else
             {
                 decimal_x = 1;
             }
             int decimal_number = decimal_num[decimal_x][current_unit];
             if (default_unit == 0)
             {
                 input_value=input_rate*Input_Value ;
                 input_value =input_value/100;
             }
             else
             {
                 input_value=input_rate*Input_Value ;
                 input_value =input_value/10;
             }
             CString dataformat;
             dataformat.Format(_T("%%0.%df"),decimal_number);
             strTemp.Format(dataformat,input_value);

             m_input_list.SetItemText(0,3,strTemp);
             return 1;
         }
     }



    if (m_list_type == 0)
    {
        if (m_product_type == 3)
        {
            int AddressValue = -1;
            CString strTemp;
            //m_input_list.ShowWindow(SW_HIDE);
            m_input_list.DeleteAllItems();


            g_hwnd_now = this->m_hWnd;
            m_input_list.InsertItem(0,_T("1"));
            m_input_list.InsertItem(1,_T("2"));
            m_input_list.InsertItem(2,_T("3"));

            m_input_list.SetItemText(0,1,_T("Temperature"));
            m_input_list.SetItemText(1,1,_T("Hum"));
            m_input_list.SetItemText(2,1,_T("CO2"));








            int i_internal_temp = 0;
            int i_external_temp = 0;
            float f_internal_temp = 0;
            float f_external_temp = 0;
            CString temp_internal_value,temp_external_value;



            CString strTemp1,strTemp2,strUnit,strHUM,strCO2;
            strTemp1.Format(_T("%cC"),176);
            strTemp2.Format(_T("%cF"),176);

            if(product_register_value[125] == 0)
            {
                i_internal_temp = product_register_value[119];
                i_external_temp = product_register_value[121];

                strUnit=strTemp1;
            }
            else if((product_register_value[125] == 1))
            {
                i_internal_temp = product_register_value[120];
                i_external_temp = product_register_value[122];

                strUnit=strTemp2;
            }
            else
            {
                /*return;*/
            }
            f_internal_temp = (float)i_internal_temp / 10;
            f_external_temp = (float)i_external_temp / 10;
            CString  TempValue,StrAM;


            if(product_register_value[124] == 0)//�ڲ�
            {
                TempValue.Format(_T("%0.1f"),f_internal_temp);
            }
            else
            {
                TempValue.Format(_T("%0.1f"),f_external_temp);
            }

            m_input_list.SetItemText(0,2,TempValue);
            strHUM.Format(_T("%0.1f"),(float)(product_register_value[116])/10.0);

            m_input_list.SetItemText(1,2,strHUM);
            strCO2.Format(_T("%d"),product_register_value[108]);

            m_input_list.SetItemText(2,2,strCO2);

            m_input_list.SetItemText(0,3,strUnit);
            m_input_list.SetItemText(1,3,_T("%"));
            m_input_list.SetItemText(2,3,_T("ppm"));

            m_input_list.SetItemText(0,4,_T("UNUSED"));
            m_input_list.SetItemText(1,4,_T("UNUSED"));
            strTemp.Format (_T("%d"),product_register_value[111]);
            m_input_list.SetItemText(2,4,strTemp);
        }
        else
        {
            m_input_list.DeleteAllItems();
            m_input_list.InsertItem(0,_T("1"));
            m_input_list.SetItemText(0,1,_T("Pressure"));

            if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(4))
            {
                ListCtrlEx::CStrList strlist;
                for (int i=0;i<(int)sizeof(Pressure_Units)/sizeof(Pressure_Units[0]);i++)
                {
                    strlist.push_back(Pressure_Units[i]);
                }
                m_input_list.SetCellStringList(0, 4, strlist);
            }

            bitset<16> AM(product_register_value[280]);
            int Input_Value = -1;
            if (!AM[2])
            {
                m_input_list.SetItemText(0,2,Global_String_AUTO);
                Input_Value = (short)product_register_value[711] ;
            }
            else
            {
                m_input_list.SetItemText(0,2,Global_String_MANUAL);
                Input_Value = (short)product_register_value[283] ;
            }

            unsigned char default_unit = product_register_value[702];
            unsigned char current_unit = product_register_value[701];
            float input_rate   = get_units_ratio(current_unit,default_unit);
            float input_value = 0;
            int decimal_x = 0;
            if (default_unit == 0)
            {
                decimal_x = 0 ;
            }
            else
            {
                decimal_x = 1;
            }
            int decimal_number = decimal_num[decimal_x][current_unit];
            if (default_unit == 0)
            {
                input_value=input_rate*Input_Value ;
                input_value =input_value/100;
            }
            else
            {
                input_value=input_rate*Input_Value ;
                input_value =input_value/10;
            }
            CString dataformat;
            dataformat.Format(_T("%%0.%df"),decimal_number);
            strTemp.Format(dataformat,input_value);

            m_input_list.SetItemText(0,3,strTemp);
            strTemp = Pressure_Units[current_unit];
            m_input_list.SetItemText(0,4,strTemp);
            strTemp.Format(_T("%d"),product_register_value[705]);
            m_input_list.SetItemText(0,5,strTemp);
        }


    }
    else if (m_list_type == 1)
    {
        if (m_product_type == 3)
        {
            int AddressValue = -1;
            CString strTemp;

            m_output_list.DeleteAllItems();

            g_hwnd_now = this->m_hWnd;

            m_output_list.InsertItem(0,_T("1"));
            m_output_list.InsertItem(1,_T("2"));
            m_output_list.InsertItem(2,_T("3"));
            m_output_list.SetItemText(0,1,_T("Temperature"));
            m_output_list.SetItemText(1,1,_T("Hum"));
            m_output_list.SetItemText(2,1,_T("CO2"));

            AddressValue =127 ;


            int output_range=product_register_value[AddressValue];
            if (output_range==3){
                strTemp=_T("0-10v");
            }
            else if (output_range==2)
            {
                strTemp=_T("0-5v");
            }
            else if (output_range==1)
            {
                strTemp=_T("4-20mA");
            }
            else
            {
                strTemp=_T("");
            }
            m_output_list.SetItemText(0,2,strTemp);
            m_output_list.SetItemText(1,2,strTemp);
            m_output_list.SetItemText(2,2,strTemp);


			float tc = (float)((short)product_register_value[128]);
			if (product_register_value[125] == 0)
			{
				strTemp.Format(_T("%.1f"), tc / 10);
			}
			else
			{
				strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
			}


			m_output_list.SetItemText(0, 3, strTemp);

			tc = (float)((short)product_register_value[129]);
			if (product_register_value[125] == 0)
			{
				strTemp.Format(_T("%.1f"), tc / 10);
			}
			else
			{
				strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
			}


			m_output_list.SetItemText(0, 4, strTemp);

			tc = (float)((short)product_register_value[130]);
			if (product_register_value[125] == 0)
			{
				strTemp.Format(_T("%.1f"), tc / 10);
			}
			else
			{
				strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
			}

			m_output_list.SetItemText(1, 3, strTemp);
			tc = (float)((short)product_register_value[131]);
			if (product_register_value[125] == 0)
			{
				strTemp.Format(_T("%.1f"), tc / 10);
			}
			else
			{
				strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
			}
			m_output_list.SetItemText(1, 4, strTemp);
			tc = (float)((short)product_register_value[132]);
			if (product_register_value[125] == 0)
			{
				strTemp.Format(_T("%.1f"), tc / 10);
			}
			else
			{
				strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
			}
			m_output_list.SetItemText(2, 3, strTemp);
			tc = (float)((short)product_register_value[133]);
			if (product_register_value[125] == 0)
			{
				strTemp.Format(_T("%.1f"), tc / 10);
			}
			else
			{
				strTemp.Format(_T("%.1f"), CTwoF(tc / 10));
			}
			m_output_list.SetItemText(2, 4, strTemp);



			CString strTemp1, strTemp2, strUnit, strHUM, strCO2;
			strTemp1.Format(_T("%cC"), 176);
			strTemp2.Format(_T("%cF"), 176);

			if (product_register_value[125] == 0)
			{


				strUnit = strTemp1;
			}
			else if (product_register_value[125] == 1)
			{


				strUnit = strTemp2;
			}
			m_output_list.SetItemText(0, 5, strUnit);
			strTemp = L"%";

			m_output_list.SetItemText(1, 5, strTemp);

			strTemp = L"ppm";
			m_output_list.SetItemText(2, 5, strTemp);



			 

            m_output_list.ShowWindow (SW_SHOW);
        }

		else if (m_product_type == 1)
		{
		   float flValue = 0;

			m_output_list.DeleteAllItems();

			g_hwnd_now = this->m_hWnd;

			m_output_list.InsertItem(0,_T("1"));
			m_output_list.InsertItem(1,_T("2"));
			m_output_list.InsertItem(2,_T("3"));

			m_output_list.SetItemText(0,1,_T("Hum"));
			m_output_list.SetItemText(1,1,_T("Temperature"));
			m_output_list.SetItemText(2,1,_T("CO2"));

			AddressValue =284 ;
			int output_range=product_register_value[AddressValue];
			if (output_range==3){
				strTemp=_T("0-10v");
			}
			else if (output_range==2)
			{
				strTemp=_T("0-5v");
			}
			else if (output_range==1)
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
			float FloatValue = 0;

			if (output_range == 1)
			{
				AddressValue =10003 ;
				flValue = (((float)((short)ValueData[AddressValue-10000]))/1024)*0.025;
				strTemp.Format (_T("%0.2f"),flValue);
				m_output_list.SetItemText(0,2,strTemp);

				AddressValue =10004 ;
				flValue = (((float)((short)ValueData[AddressValue-10000]))/1024)*0.025;
				strTemp.Format (_T("%0.2f"),flValue);
				m_output_list.SetItemText(1,2,strTemp);

				AddressValue =10005 ;
				flValue = (((float)((short)ValueData[AddressValue-10000]))/1024)*0.025;
				strTemp.Format (_T("%0.2f"),flValue);
				m_output_list.SetItemText(2,2,strTemp);
			}
			else
			{
				AddressValue =10000 ;
				FloatValue = (short)ValueData[AddressValue-10000];
				flValue = (FloatValue/1024)*10;
				strTemp.Format (_T("%0.2f"),flValue);
				m_output_list.SetItemText(0,2,strTemp);

				AddressValue =10001 ;
				FloatValue = (short)ValueData[AddressValue-10000];
				flValue = (FloatValue/1024)*10;

				strTemp.Format (_T("%0.2f"),flValue);
				m_output_list.SetItemText(1,2,strTemp);

				AddressValue =10002 ;
				FloatValue = (short)ValueData[AddressValue-10000];
				flValue = (FloatValue/1024)*10;
				strTemp.Format (_T("%0.2f"),flValue);
				m_output_list.SetItemText(2,2,strTemp);
			}






			FloatValue = (short)product_register_value[287];
			flValue = (FloatValue)/10;
			strTemp.Format (_T("%0.1f"),flValue);
			m_output_list.SetItemText(0,4,strTemp);



			FloatValue = (short)product_register_value[288];
			flValue = (FloatValue)/10;
			strTemp.Format (_T("%0.1f"),flValue);
			m_output_list.SetItemText(0,5,strTemp);


			FloatValue = (short)product_register_value[285];
			flValue = (FloatValue)/10;
			strTemp.Format (_T("%0.1f"),flValue);

			m_output_list.SetItemText(1,4,strTemp);


			FloatValue = (short)product_register_value[286];
			flValue = (FloatValue)/10;
			strTemp.Format (_T("%0.1f"),flValue);

			m_output_list.SetItemText(1,5,strTemp);

			strTemp.Format (_T("%d"),product_register_value[289]);
			m_output_list.SetItemText(2,4,strTemp);
			strTemp.Format (_T("%d"),product_register_value[290]);
			m_output_list.SetItemText(2,5,strTemp);



			CString strTemp1,strTemp2,strUnit,strHUM,strCO2;
			strTemp1.Format(_T("%cC"),176);
			strTemp2.Format(_T("%cF"),176);

			if(product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 0)
			{


				strUnit=strTemp1;
			}
			else if((product_register_value[CO2_485_MODBUS_DEG_C_OR_F] == 1))
			{


				strUnit=strTemp2;
			}
			m_output_list.SetItemText(1,6,strUnit);
			strTemp = L"%";

			m_output_list.SetItemText(0,6,strTemp);

			strTemp = L"ppm";
			m_output_list.SetItemText(2,6,strTemp);
		}
        else
        {
            int AddressValue = -1;
            CString strTemp;

            m_output_list.DeleteAllItems();

            m_output_list.InsertItem(0,_T("1"));
            m_output_list.SetItemText(0,1,_T("Pressure"));
            //Range
            AddressValue =284 ;
            int output_range=product_register_value[AddressValue];
            if (output_range==1){
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

            //Value
            AddressValue =280;
            int ret = read_one(g_tstat_id,1002);
            if (ret>-1)
            {
                product_register_value[1002] = ret;
            }
            ret = read_one(g_tstat_id,1005);
            if (ret>-1)
            {
                product_register_value[1005] = ret;
            }


            if (output_range==1||output_range==2)
            {
                strTemp.Format(_T("%.2f v"),((float)((short)product_register_value[1002]))*1.024);
            }
            else if (output_range==3)
            {

                strTemp.Format(_T("%.2f ma"),((float)((short)product_register_value[1005]))*2.5*1.024);
            }
            else{
                strTemp=_T("0");
            }

            m_output_list.SetItemText(0,2,strTemp);

            //Min out scale
            AddressValue =289 ;
            strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[AddressValue]/10.0);
            m_output_list.SetItemText(0,4,strTemp);
            //Max out scale
            AddressValue =290 ;
            strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[AddressValue]/10.0);
            m_output_list.SetItemText(0,5,strTemp);
        }

    }
    return 0;
}

void CCO2_View::OnBnClickedGrapic()
{
    LoadRegistersGraphicMode();
    CGraphicMode dlg;
    dlg.DoModal();
    g_hwnd_now = this->m_hWnd;
}


void CCO2_View::OnBnClickedTempSensor()
{
    if (product_register_value[124] == 0)
    {
        return ;
    }
    int ret = write_one (g_tstat_id,124,0);
    if(ret>0)
    {
        product_register_value[124] = 0;
    }

    if (product_register_value[124] == 0)
    {
        CButton *pTempButton;
        pTempButton = (CButton *)(GetDlgItem (IDC_TEMP_SENSOR)) ;
        pTempButton->SetCheck (1);

        pTempButton = (CButton *)(GetDlgItem (IDC_HUM_SENSOR)) ;
        pTempButton->SetCheck (0);

    }
    else
    {
        CButton *pTempButton;
        pTempButton = (CButton *)(GetDlgItem (IDC_TEMP_SENSOR)) ;
        pTempButton->SetCheck (0);

        pTempButton = (CButton *)(GetDlgItem (IDC_HUM_SENSOR)) ;
        pTempButton->SetCheck (1);
    }
    m_list_type = 0;
    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
}


void CCO2_View::OnBnClickedHumSensor()
{
    if (product_register_value[124] == 1)
    {
        return ;
    }
    int ret = write_one (g_tstat_id,124,1);
    if(ret>0)
    {
        product_register_value[124] = 1;
    }
    if (product_register_value[124] == 0)
    {
        CButton *pTempButton;
        pTempButton = (CButton *)(GetDlgItem (IDC_TEMP_SENSOR)) ;
        pTempButton->SetCheck (1);

        pTempButton = (CButton *)(GetDlgItem (IDC_HUM_SENSOR)) ;
        pTempButton->SetCheck (0);

    }
    else
    {
        CButton *pTempButton;
        pTempButton = (CButton *)(GetDlgItem (IDC_TEMP_SENSOR)) ;
        pTempButton->SetCheck (0);

        pTempButton = (CButton *)(GetDlgItem (IDC_HUM_SENSOR)) ;
        pTempButton->SetCheck (1);
    }
    m_list_type = 0;
    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
}

void CCO2_View::Fresh_CO2Node_Compensatin()
{
	if (product_register_value[500] == 0)
	{
		m_button_onoff.SetWindowTextW(L"OFF");
	}
	else
	{
		m_button_onoff.SetWindowTextW(L"ON");
	}

	CString strTemp;
	strTemp.Format(_T("%d"),product_register_value[501]);
	m_edit_nco2.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),product_register_value[503]);
	m_edit_rtime.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),product_register_value[502]);
	m_edit_adjustment.SetWindowTextW(strTemp);
}

void CCO2_View::OnEnKillfocusEdit1Nco2()
{
	CString strTemp;
	m_edit_nco2.GetWindowTextW(strTemp);
	int co2value = _wtoi(strTemp);
	if (co2value<390||co2value>500)
	{
		AfxMessageBox(_T("out of range"));
	}

	int ret = write_one(g_tstat_id,501,co2value);
	if (ret>0)
	{
		product_register_value[501] = co2value;
	}
	Fresh_CO2Node_Compensatin();
}


void CCO2_View::OnEnKillfocusEditRtime()
{
	CString strTemp;
	m_edit_rtime.GetWindowText(strTemp);
	int rtime = _wtoi(strTemp);
	if (rtime<2||rtime>30)
	{
		AfxMessageBox(_T("out of range"));
	}
	int ret = write_one(g_tstat_id,503,rtime);
	if (ret>0)
	{
		product_register_value[503]=rtime;
	}
	Fresh_CO2Node_Compensatin();

}


void CCO2_View::OnBnClickedButtonOnoff()
{
	 if (product_register_value[500] == 0)
	 {
		int ret = write_one(g_tstat_id,500,1);
		if (ret>0)
		{
			product_register_value[500]=1;
		}
	 }
	 else
	 {
		 int ret = write_one(g_tstat_id,500,0);
		 if (ret>0)
		 {
			 product_register_value[500]=0;
		 }
	 }
	 Fresh_CO2Node_Compensatin();
}


void CCO2_View::OnEnKillfocusEditRtime2()
{
	CString strTemp;
	m_edit_adjustment.GetWindowText(strTemp);
	int rtime = _wtoi(strTemp);
	if (rtime<1||rtime>10)
	{
		AfxMessageBox(_T("out of range"));
	}
	int ret = write_one(g_tstat_id,502,rtime);
	if (ret>0)
	{
		product_register_value[502]=rtime;
	}
	Fresh_CO2Node_Compensatin();
}
