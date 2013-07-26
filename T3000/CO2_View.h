#pragma once
#include "afxwin.h"
#include "msflexgrid1.h"
#include "CM5/ListCtrlEx.h"
#include "CM5\MyOwnListCtrl.h"
#include "afxcmn.h"
#include "afxdtctl.h"

#define ALARM_AUTO   0
#define ALARM_MANULE 1

#define ALARM_OFF		0
#define ALARM_DEFINED	1
#define ALARM_ON		2


#define CO2_PPM					1
#define PREPARE_ALARM_PPM		2
#define ALARM_PPM				3
#define CALIBRATION_PPM_OFFSET	4

// CCO2_View form view

class CCO2_View : public CFormView
{
	DECLARE_DYNCREATE(CCO2_View)

public:
	CCO2_View();           // protected constructor used by dynamic creation
	virtual ~CCO2_View();

public:
	enum { IDD = IDD_CO2_DIALOG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void Fresh();
	CEdit m_co2_idAdressEdit;
	
	float m_co2_firmwareversion;
	UINT m_co2_hardwareversion;
	UINT m_co2_serialNumber;

	bool m_temp_unit_c0_f1;
	CString cs_temp_unit;
	
public:
	void Get_CO2_Temperature_unit(CString &strTemp);
	void CO2_Alarm_Set();
	void CO2_Grid_init();
	void C02_SHOW_TEMP();
	void Initial_List();
	void Fresh_External_List();
	void Check_DayTime();
	void Check_HourTime();
	void Show_PassWord();
	void Show_Each_Edit_Time();
	CComboBox m_co2_alarm_state;
	CComboBox m_co2_temp_unit;
	CComboBox m_co2_braudRateCombox;
	CComboBox m_sensor_sel;
	CButton m_co2_alarm_mode_ctr;
	BOOL m_alarm_ctrl_manual_auto;
	short m_alarm_on_time;
	short m_alarm_off_time;
	short m_edit_pre_alarm_setpoint;
	short m_edit_alarm_setpoint;
	short m_edit_calibrating_offset;

	afx_msg void OnBnClickedCo2Enableidbutton();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridCo2();
	float m_f_internal_temp;
	float m_f_external_temp;
	CBrush m_brush;
	CWinThread* CO2_Thread; 
	HANDLE RefreshThread;
	afx_msg void OnCbnSelchangeComboCo2SensorSel();
	afx_msg LRESULT ResumeCO2MessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCo2TempUnit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusEditInternalTemp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnKillfocusEditExternalTemp();
	afx_msg void OnEnKillfocusCo2PrepareAlarmSetpoint();
	afx_msg void OnEnKillfocusCo2AlarmSetpoint();
	afx_msg void OnEnKillfocusCo2CalibratingOffset();
	afx_msg void OnBnClickedRadioAlarmManual();
	afx_msg void OnBnClickedRadioAlarmAuto();
	afx_msg void OnCbnSelchangeCo2AlarmState();
	afx_msg LRESULT Save_List_Item(WPARAM wParam,LPARAM lParam);
	ListCtrlEx::CListCtrlEx m_co2_external_sensor_list;
	virtual void OnInitialUpdate();
	float m_humidity_value;
	CDateTimeCtrl m_co2_day_picker;
	CDateTimeCtrl m_co2_time_picker;
	afx_msg void OnNMKillfocusCo2Datetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusCo2DatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCo2SyncTime();
	CString m_cs_password;
	afx_msg void OnBnClickedRadioPasswordEnable();
	afx_msg void OnBnClickedRadioPasswordDisable();
	afx_msg void OnEnKillfocusEditCo2Passwor();
	int m_co2_block_time;
	int m_co2_backlight_time;
	afx_msg void OnEnKillfocusEditCo2BlockTime();
	afx_msg void OnEnKillfocusEditCo2BacklightTime();
	afx_msg LRESULT  DealMessage(WPARAM wParam,LPARAM lParam);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnCo2Refresh();
	static DWORD WINAPI StartRefresh(LPVOID lpVoid);
	afx_msg void OnEnKillfocusCo2AlarmOnTime();
	afx_msg void OnEnKillfocusCo2AlarmOffTime();
};

typedef enum
{
	CO2_485_MODBUS_SERIALNUMBER_LOWORD = 0,             
	CO2_485_MODBUS_SERIALNUMBER_HIWORD = 2,
	CO2_485_MODBUS_VERSION_NUMBER_LO = 4,
	CO2_485_MODBUS_VERSION_NUMBER_HI = 5,
	CO2_485_MODBUS_ADDRESS = 6,
	CO2_485_MODBUS_PRODUCT_MODEL,
	CO2_485_MODBUS_HARDWARE_REV,
	CO2_485_MODBUS_PIC_VERSION,
	CO2_485_MODBUS_ADDRESS_PLUG_N_PLAY,
	CO2_485_MODBUS_BASE_ADDRESS = 15,			// base address select
	CO2_485_MODBUS_UPDATE_STATUS = 16,			// status for update flash
	CO2_485_MODBUS_SERINALNUMBER_WRITE_FLAG,

	CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT = 100,	//0 internal sensor
	CO2_485_MODBUS_DEG_C_OR_F,				// 101
	CO2_485_MODBUS_TEMPERATURE_C_INTERNAL,	// 102
	CO2_485_MODBUS_TEMPERATURE_F_INTERNAL,	// 103
	CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL,	// 104
	CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL,	// 105

	CO2_485_MODBUS_HUMIDITY_RH,				// 106,  relative humidity in percentage, 112
	CO2_485_MODBUS_HUMIDITY_FREQUENCY, 		// 107, raw frequency reading
	CO2_485_MODBUS_HUM_SENSOR_HEATING,		// 108

	CO2_485_MODBUS_INTERNAL_SENSOR_EXIST,	// 109, Internal co2 sensor exist, 0 = no, 1 = yes.
	CO2_485_MODBUS_INTERNAL_CO2_PPM,		// 110, internal co2 ppm
	CO2_485_MODBUS_EXTERNAL_CO2_PPM_START,	// 111, external c02 ppm
	CO2_485_MODBUS_ALARM_AUTO_MANUAL = 161,	// 161, bit7: 0 = auto, 1 = manual; bit0:1 = pre_alarm; bit1: 1 = continuous_alarm; bit(1:0): 00 = stop_ alarm		
	CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME,	// 162
	CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME,	// 163
	CO2_485_MODBUS_ALARM_DELAY_TIME,			// 164

	CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT,		// 165
	CO2_485_MODBUS_INT_ALARM_SETPOINT,			// 166
	CO2_485_MODBUS_INT_CO2_OFFSET,				// 167

	CO2_485_MODBUS_CO2_SLOPE_DETECT_VALUE,		// 168
	CO2_485_MODBUS_CO2_FILTER,					// 169

	CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START,// 170
	CO2_485_MODBUS_EXT_ALARM_SETPOINT_START = 220, // 220
	CO2_485_MODBUS_EXT_CO2_OFFSET_START = 270, // 270
	//CO2_485_MODBUS_EXT_CO2_OFFSET_END = 320,

	CO2_485_MODBUS_OUTPUT_AUTO_MANUAL = 320,		// 320
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM,
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM,
	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2,

	CO2_485_MODBUS_OUTPUT_RANGE_MIN_TEM,	// 324				
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_TEM,
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_HUM,				
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_HUM,
	CO2_485_MODBUS_OUTPUT_RANGE_MIN_CO2,				
	CO2_485_MODBUS_OUTPUT_RANGE_MAX_CO2,


	CO2_485_MODBUS_INFO_BYTE, 				// 330
	CO2_485_MODBUS_BAUDRATE,  				// 331

	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_1,// 333
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_2,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_3,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_4,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_5,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_6,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_7,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_8,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_9,
	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_10,

	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_1,// 342
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_2,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_3,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_4,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_5,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_6,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_7,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_8,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_9,
	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_10,

	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_1,// 352
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_2,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_3,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_4,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_5,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_6,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_7,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_8,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_9,
	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_10,
	CO2_485_MODBUS_RTC_SEC,			// 362			
	CO2_485_MODBUS_RTC_MIN,							
	CO2_485_MODBUS_RTC_HOUR,
	CO2_485_MODBUS_RTC_DAY,
	CO2_485_MODBUS_RTC_WEEK,
	CO2_485_MODBUS_RTC_MONTH,
	CO2_485_MODBUS_RTC_YEAR,
	CO2_485_MODBUS_PASSWORD_ENABLE,	// 369
	CO2_485_MODBUS_USER_PASSWORD0, 					
	CO2_485_MODBUS_USER_PASSWORD1,
	CO2_485_MODBUS_USER_PASSWORD2,
	CO2_485_MODBUS_USER_PASSWORD3,
	CO2_485_MODBUS_MENU_BLOCK_SECONDS,
	CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS,
	CO2_485_MODBUS_EXTERNAL_NODES_PLUG_AND_PLAY,	// 376
	CO2_485_MODBUS_SCAN_DB_CTR,						// 377
	CO2_485_MODBUS_RESET_SCAN_DB,					
	CO2_485_MODBUS_SCAN_START,						// 379						


} MODBUS_CO2_485_REGISTER_LIST;

const int CO2_EXTERNAL_NUM = 0;
const int CO2_EXTERNAL_DEVICE_ID = 1;
const int CO2_EXTERNAL_SERIAL_NUM = 2;
const int CO2_EXTERNAL_PPM = 3;
const int CO2_EXTERNAL_PRE_ALARM_SP = 4;
const int CO2_EXTERNAL_ALARM_SP = 5;
const int CO2_EXTERNAL_CAL_OFFSET = 6;

const int CO2_SCAN_DB_SIZE	=	5;
bool CheckString( CString str );

#define WM_REFRESH_CO2_DLG WM_USER + 311

const int REFRESH_DLG = 1;
const int ENABLE_REFRESH_BUTTON = 2;
const int REFRESH_FAIL = 3;

