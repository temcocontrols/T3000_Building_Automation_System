#pragma once
#include "afxwin.h"
#include "msflexgrid1.h"
#include "CM5/ListCtrlEx.h"
#include "CM5\MyOwnListCtrl.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "msflexgrid1.h"
 
#define ALARM_AUTO   0
#define ALARM_MANULE 1

#define ALARM_OFF		0
#define ALARM_DEFINED	1
#define ALARM_ON		2


#define CO2_PPM					1
#define PREPARE_ALARM_PPM		2
#define ALARM_PPM				3
#define CALIBRATION_PPM_OFFSET	4





#define	FW_VERSION					206
#define	FW_VER_HIGH					(FW_VERSION / 100)
#define	FW_VER_LOW					(FW_VERSION % 100)

#define	PRODUCT_ID					32

#define	FLASH_OFFSET				0x70000
//#define	FLASH_END					0xffff

#define TOTAL_EE_PARAMETERS			208

#define	ORIGINALADDRESSVALUE		200

#define	MAX_EXT_CO2					254
#define	SCAN_DB_SIZE				5

// CCO2_View form view
/// <summary>
/// CCO2_View
/// </summary>
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
	void Fresh_CO2();
    void SH_Window();
	CEdit m_co2_idAdressEdit;
	
	float m_co2_firmwareversion;
	UINT m_co2_hardwareversion;
	UINT m_co2_serialNumber;
	float CTwoF(float tc);
	float FTwoC(float tf);
	bool m_temp_unit_c0_f1;
	CString cs_temp_unit;
	bool m_start_tip;




	int m_nCurRow;
	int m_nCurCol;

public:
	void Get_CO2_Temperature_unit(CString &strTemp);
	void CO2_Alarm_Set();
	void CO2_Grid_init();
	void C02_SHOW_TEMP();
    void Node_SHOW_TEMP();
	 void Initial_List();
	 
	void Check_DayTime();
	void Check_HourTime();
	void Show_PassWord();
	void Show_Each_Edit_Time();
	CComboBox m_co2_alarm_state;
	CComboBox m_co2_temp_unit;
	CComboBox m_co2_baudRateCombox;
	CComboBox m_sensor_sel;
	CButton m_co2_alarm_mode_ctr;
	BOOL m_alarm_ctrl_manual_auto;
	int m_alarm_on_time;
	int m_alarm_off_time;
	int m_edit_pre_alarm_setpoint;
	int m_edit_alarm_setpoint;
	int m_edit_calibrating_offset;

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
	//afx_msg LRESULT Save_List_Item(WPARAM wParam,LPARAM lParam);
	//ListCtrlEx::CListCtrlEx m_co2_external_sensor_list;
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
	//static DWORD WINAPI StartRefresh(LPVOID lpVoid);
	afx_msg void OnEnKillfocusCo2AlarmOnTime();
	afx_msg void OnEnKillfocusCo2AlarmOffTime();
	//afx_msg void OnBnClickedBtnCo2ClearCal();
	afx_msg void OnBnClickedRadioHumidityHeatEnable();
	afx_msg void OnBnClickedRadioHumidityHeatDisable();
    void Initial_Registerlist();

	int m_message_down;
	int m_message_up;

	int m_value;       
	int m_address;
	int m_times;
	unsigned short ValueData[10];
	bool m_fresh_Grid;
    private:
        int	CO2_485_MODBUS_SERIALNUMBER_LOWORD	;
        int	CO2_485_MODBUS_SERIALNUMBER_HIWORD	;
        int	CO2_485_MODBUS_VERSION_NUMBER_LO	;
        int	CO2_485_MODBUS_VERSION_NUMBER_HI	;
        int	CO2_485_MODBUS_ADDRESS	;
        int	CO2_485_MODBUS_PRODUCT_MODEL	;
        int	CO2_485_MODBUS_HARDWARE_REV	;
        int	CO2_485_MODBUS_PIC_VERSION	;
        int	CO2_485_MODBUS_ADDRESS_PLUG_N_PLAY	;
        int	CO2_485_MODBUS_BASE_ADDRESS	;
        int	CO2_485_MODBUS_UPDATE_STATUS	;
        int	CO2_485_MODBUS_SERINALNUMBER_WRITE_FLAG	;
        int	CO2_485_MODBUS_TEMPERATURE_SENSOR_SELECT	;
        int	CO2_485_MODBUS_DEG_C_OR_F	;
        int	CO2_485_MODBUS_TEMPERATURE_C_INTERNAL	;
        int	CO2_485_MODBUS_TEMPERATURE_F_INTERNAL	;
        int	CO2_485_MODBUS_TEMPERATURE_C_EXTERNAL	;
        int	CO2_485_MODBUS_TEMPERATURE_F_EXTERNAL	;

        int	CO2_485_MODBUS_HUMIDITY_RH	;
        int	CO2_485_MODBUS_HUMIDITY_FREQUENCY	;
        int	CO2_485_MODBUS_HUM_SENSOR_HEATING	;

        int	CO2_485_MODBUS_INTERNAL_SENSOR_EXIST	;
        int	CO2_485_MODBUS_INTERNAL_CO2_PPM	;
        int	CO2_485_MODBUS_EXTERNAL_CO2_PPM_START	;

        int	CO2_485_MODBUS_ALARM_AUTO_MANUAL	;
        int	CO2_485_MODBUS_PRE_ALARM_SETTING_ON_TIME	;
        int	CO2_485_MODBUS_PRE_ALARM_SETTING_OFF_TIME	;
        int	CO2_485_MODBUS_ALARM_DELAY_TIME	;
        int	CO2_485_MODBUS_INT_PRE_ALARM_SETPOINT	;
        int	CO2_485_MODBUS_INT_ALARM_SETPOINT	;
        int	CO2_485_MODBUS_INT_CO2_OFFSET	;
        int	CO2_485_MODBUS_CO2_SLOPE_DETECT_VALUE	;
        int	CO2_485_MODBUS_CO2_FILTER	;
        int	CO2_485_MODBUS_EXT_PRE_ALARM_SETPOINT_START	;
        int	CO2_485_MODBUS_EXT_ALARM_SETPOINT_START	;
        int	CO2_485_MODBUS_EXT_CO2_OFFSET_START	;
        int	CO2_485_MODBUS_OUTPUT_AUTO_MANUAL	;
        int	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_TEM	;
        int	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_HUM	;
        int	CO2_485_MODBUS_OUTPUT_MANUAL_VALUE_CO2	;
        int	CO2_485_MODBUS_OUTPUT_RANGE_MIN_TEM	;
        int	CO2_485_MODBUS_OUTPUT_RANGE_MAX_TEM	;
        int	CO2_485_MODBUS_OUTPUT_RANGE_MIN_HUM	;
        int	CO2_485_MODBUS_OUTPUT_RANGE_MAX_HUM	;
        int	CO2_485_MODBUS_OUTPUT_RANGE_MIN_CO2	;
        int	CO2_485_MODBUS_OUTPUT_RANGE_MAX_CO2	;
        int	CO2_485_MODBUS_INFO_BYTE	;
        int	CO2_485_MODBUS_BAUDRATE	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_1	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_2	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_3	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_4	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_5	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_6	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_7	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_8	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_9	;
        int	CO2_485_MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_10	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_1	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_2	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_3	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_4	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_5	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_6	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_7	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_8	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_9	;
        int	CO2_485_MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_10	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_1	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_2	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_3	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_4	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_5	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_6	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_7	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_8	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_9	;
        int	CO2_485_MODBUS_CO2_OUTPUT_CURRENT_CALIBRATION_10	;
        int	CO2_485_MODBUS_RTC_SEC	;
        int	CO2_485_MODBUS_RTC_MIN	;
        int	CO2_485_MODBUS_RTC_HOUR	;
        int	CO2_485_MODBUS_RTC_DAY	;
        int	CO2_485_MODBUS_RTC_WEEK	;
        int	CO2_485_MODBUS_RTC_MONTH	;
        int	CO2_485_MODBUS_RTC_YEAR	;
        int	CO2_485_MODBUS_PASSWORD_ENABLE	;
        int	CO2_485_MODBUS_USER_PASSWORD0	;
        int	CO2_485_MODBUS_USER_PASSWORD1	;
        int	CO2_485_MODBUS_USER_PASSWORD2	;
        int	CO2_485_MODBUS_USER_PASSWORD3	;
        int	CO2_485_MODBUS_MENU_BLOCK_SECONDS	;
        int	CO2_485_MODBUS_BACKLIGHT_KEEP_SECONDS	;
        int	CO2_485_MODBUS_EXTERNAL_NODES_PLUG_AND_PLAY	;
        int	CO2_485_MODBUS_SCAN_DB_CTR	;
        int	CO2_485_MODBUS_RESET_SCAN_DB	;
        int	CO2_485_MODBUS_SCAN_START	;
		int CO2_485_OIUTPUT_MODE ;
        int CO2_485_MODBUS_SCAN_END ;            
        int CO2_485_MODBUS_GET_NODES_PARA_START; 
        int CO2_485_MODBUS_GET_NODES_PARA_END ;
        int CO2_485_MODBUS_SCAN_OCCUPY_START ;
        int CO2_485_MODBUS_SCAN_OCCUPY_END    ;

       int CO2_485_MODBUS_SCAN_ONLINE_START                              ;
       int CO2_485_MODBUS_SCAN_ONLINE_END                                ;
       int CO2_485_MODBUS_PRESSURE_SENSOR_MODEL                          ;
       int CO2_485_MODBUS_PRESSURE_UNIT                                  ;
       int CO2_485_MODBUS_PRESSURE_UNIT_DEFAULT                          ;
       int CO2_485_MODBUS_OUTPUT_RANGE_MIN_PRESSURE                      ;
       int CO2_485_MODBUS_OUTPUT_RANGE_MAX_PRESSURE                      ;
       int CO2_485_MODBUS_PRESSURE_FILTER                                ;
       int CO2_485_MODBUS_PRESSURE_SLOPE                                 ;
       int CO2_485_MODBUS_PRESSURE_INTERCEPT                             ;
       int CO2_485_MODBUS_INPUT_AUTO_MANUAL_PRE                          ;
       int CO2_485_MODBUS_INPUTPUT_MANUAL_VALUE_PRE                      ;
       int CO2_485_MODBUS_PREESURE_AD                                    ;
       int CO2_485_MODBUS_PRESSURE_VALUE_ORG                             ;
       int CO2_485_MODBUS_PRESSURE_VALUE_ORG_OFFSET                      ;
       int CO2_485_MODBUS_PRESSURE_VALUE_INDEX                           ;
       int CO2_485_MODBUS_PRESSURE_VALUE_BASE_L                          ;
       int CO2_485_MODBUS_PRESSURE_VALUE_BASE_H                          ;
       int CO2_485_MODBUS_PRESSURE_CAL_POINT                             ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR0                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD0                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR1                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD1                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR2                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD2                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR3                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD3                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR4                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD4                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR5                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD5                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR6                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD6                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR7                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD7                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR8                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD8                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_PR9                               ;
       int CO2_485_MODBUS_PRESSURE_CAL_AD9                               ;
       int CO2_485_MODBUS_TABLE_SEL                                      ;
       int CO2_485_MODBUS_USER_CAL_POINT                                 ;
       int CO2_485_MODBUS_USER_CAL_PR0                                   ;
       int CO2_485_MODBUS_USER_CAL_AD0                                   ;
       int CO2_485_MODBUS_USER_CAL_PR9                                   ;
       int CO2_485_MODBUS_USER_CAL_AD9                                   ;
       int CO2_485_MODBUS_PRESSURE_VALUE_AD                              ;
       int CO2_485_MODBUS_PRESSURE_TEMPER_AD                             ;
       int CO2_485_MODBUS_PRESSURE_VALUE                                 ;
       int CO2_485_MODBUS_PRESSURE_TEMPER                                ;
       int CO2_485_MODBUS_PRESSURE_STATUS                                ;
       int CO2_485_MODBUS_PRESSURE_READ_ENABLE                           ;
       int CO2_485_MODBUS_PRESSURE_READ_ALL                              ;
       int CO2_485_MODBUS_PRESSURE_READ_ONLY                             ;
       int CO2_485_MODBUS_PRESSURE_OFFSET                                ;
             

public:
	afx_msg void OnEnKillfocusEditCo2Humidity();
	UINT m_co2_value;
	afx_msg void OnEnKillfocusEditCo2value();
	CMsflexgrid m_grid_input;
	void ClickMsflexgridInput();
	CMFCButton m_upButton;
	CMFCButton m_downButton;
	CEdit m_inValueEdit;
	afx_msg void OnEnKillfocusEditValueGrid();
	 void OnLButtonDown(CPoint pt);
//	afx_msg void OnBnDropDownDownbutton(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedUpbutton();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_test_edit;
    int m_product_type; //1.CO2,2.Pressure sensor,3 CO2 Node
	afx_msg void OnEnKillfocusIdCo2Edit();
	afx_msg void OnCbnSelchangeCo2Baudratecombo();
    ListCtrlEx::CListCtrlEx m_input_list;
    ListCtrlEx::CListCtrlEx m_output_list;
    void Initial_InputList();
    void Initial_OutputList();
    void Initial_Node_InputList();
    void Initial_Node_OutputList();
    afx_msg void OnNMClickList_Input(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickList_Output(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickList_CO2List(NMHDR *pNMHDR, LRESULT *pResult);
    float get_mmhg_ratio(unsigned char unit);
    float get_units_ratio(unsigned char new_unit,unsigned char default_unit);
    afx_msg LRESULT Change_Item_List(WPARAM wParam,LPARAM lParam);
     
    afx_msg LRESULT Fresh_Lists(WPARAM wParam,LPARAM lParam);
    int m_list_type ;

    afx_msg void OnBnClickedGrapic();
    afx_msg void OnBnClickedTempSensor();
    afx_msg void OnBnClickedHumSensor();
	CButton m_button_onoff;
	CEdit m_edit_nco2;
	CEdit m_edit_rtime;
	afx_msg void OnEnKillfocusEdit1Nco2();
	afx_msg void OnEnKillfocusEditRtime();
	void Fresh_CO2Node_Compensatin();
	afx_msg void OnBnClickedButtonOnoff();
	afx_msg void OnEnKillfocusEditRtime2();
	CEdit m_edit_adjustment;
};
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

