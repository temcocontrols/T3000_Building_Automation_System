#pragma once
#include "../msflexgrid1.h"
#include "../ado/ADO.h"
#include "../MainFrm.h"
#include "afxwin.h"
#include "../global_struct.h"

#define AirQualityTimer							1

typedef struct AIRQUALITY 
{
	int iaddress;
	CString CStvalue;
}airquality;


// CAirQuality form view

class CAirQuality : public CFormView
{
	DECLARE_DYNCREATE(CAirQuality)

protected:
public:
	CAirQuality();           // protected constructor used by dynamic creation
	virtual ~CAirQuality();

public:
	enum { IDD = IDD_DIALOG_AIRQUALITY };
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
afx_msg void OnEnKillfocustempfilter();
afx_msg void OnEnKillfocusBackLightTime();
afx_msg void OnEnKillfocushumfilter();
afx_msg void OnEnKillfocusAQfilter();
afx_msg void OnEnKillfocustemp();
afx_msg void OnEnKillfocushum();
afx_msg void OnEnKillfocusAQ();
afx_msg void OnEnKillfocustemplow();
afx_msg void OnEnKillfocustemphigh();
afx_msg void OnEnKillfocushumlow();
afx_msg void OnEnKillfocushumhigh();
afx_msg void OnEnKillfocusaqlow();
afx_msg void OnEnKillfocusaqhigh();
afx_msg void OnCbnCFUnits();
afx_msg void OnBnClickedButton_Change_ID();
afx_msg void OnSetFocusStopFresh();
afx_msg void OnCbnSelchangeComboDisplay();
virtual BOOL PreTranslateMessage(MSG* pMsg);
CWinThread* m_pFreshBackground;
	void Fresh();
	void Fresh_Window();
	virtual void OnInitialUpdate();
	void Initial_RegisterList();
    vector<T3Register> m_vecT3Register;
	afx_msg void OnDestroy();
	void ShowAirqualityDialog();
	int  Get_RegID(CString Name);
	void ShowInput();
	void ShowOutput();
	void ShowCalibrationPointTable();
	bool m_fresh_data;
	CEdit m_edit_address;
	CEdit m_edit_sn;
	CEdit m_edit_AQ;
	CEdit m_edit_aq_filter;
	CEdit m_edit_baudrate;
	CEdit m_edit_fv;
	CEdit m_edit_hum;
	CEdit m_edit_hum_filter;
	CEdit m_edit_hv;
	CEdit m_edit_temp_filter;
	CEdit m_edit_temperature;
	CEdit m_edit_aq_high;
	CEdit m_edit_aq_low;
	CEdit m_edit_aq_output;
	CEdit m_edit_hum_high;
	CEdit m_edit_hum_low;
	CEdit m_edit_hum_output;
	CEdit m_edit_output_range;
	CEdit m_edit_temp_high;
	CEdit m_edit_temp_low;
	CEdit m_edit_temp_output;
	CComboBox m_combox_units;
	CEdit m_edit_model;
public:
	int	MODBUS_SERIALNUMBER_LOWORD	;
	int	MODBUS_SERIALNUMBER_HIWORD	;
	int	MODBUS_VERSION_NUMBER_LO	;
	int	MODBUS_VERSION_NUMBER_HI	;
	int	MODBUS_ADDRESS	;
	int	MODBUS_PRODUCT_MODEL	;
	int	MODBUS_HARDWARE_REV	;
	int	MODBUS_PIC_VERSION	;
	int	MODBUS_HUM_PIC_VERSION	;
	int	MODBUS_ADDRESS_PLUG_N_PLAY	;
	int	MODBUS_BASE_ADDRESS 	;
	int	MODBUS_UPDATE_STATUS	;
	int	MODBUS_UPDATE_PTR_HI	;
	int	MODBUS_UPDATE_PTR_LO	;
	int	MODBUS_SCRATCHPAD_ADDRESS	;
	int	MODBUS_TEMPRATURE_CHIP_F	;
	int	MODBUS_TEMPRATURE_CHIP 	;
	int	MODBUS_HUM            	;
	int	MODBUS_AQ            	;
	int	MODBUS_CALIBRATION_TEMP    	;
	int	MODBUS_CALIBRATION_AQ 	;
	int	MODBUS_CALIBRATION_HUM 	;
	int	MODBUS_TEMP_SELECT	;
	int	MODBUS_DAC_OFFSET	;
	int	MODBUS_DELAY	;
	int	MODBUS_SEQUENCE	;
	int	MODBUS_DEGC_OR_F 	;
	int	MODBUS_POWERUP_MODE  	;
	int	MODBUS_AUTO_ONLY 	;
	int	MODBUS_OUTPUT_RANGE	;
	int	MODBUS_FACTORY_DEFAULTS     	;
	int	MODBUS_EXTERNAL_SENSOR_0	;
	int	MODBUS_EXTERNAL_SENSOR_1	;
	int	MODBUS_INFO_BYTE	;
	int	MODBUS_BAUDRATE    	;
	int	MODBUS_HUM_FILTER	;
	int	MODBUS_AQ_FILTER	;
	int	MODBUS_DIGITAL_IN1	;
	int	MODBUS_CYCLING_DELAY 	;
	int	MODBUS_CHANGOVER_DELAY	;
	int	MODBUS_OVERRIDE_TIMER	;
	int	MODBUS_OVERRIDE_TIMER_LEFT	;
	int	MODBUS_FILTER	;
	int	MODBUS_HEAT_COOL_CONFIG	;
	int	MODBUS_INTERNAL_TEMP_IC	;
	int	MODBUS_INTERNAL_THERMISTOR	;
	int	MODBUS_CALIBRATION_INTERNAL_THERMISTOR	;
	int	MODBUS_CALIBRATION_ANALOG_IN2	;
	int	MODBUS_INPUT1_SELECT                          	;
	int	MODBUS_UNIVERSAL_NIGHTSET                  	;
	int	MODBUS_HEAT_ORIGINAL_TABLE                 	;
	int	MODBUS_COOL_ORIGINAL_TABLE                 	;
	int	MODBUS_PID2_MODE_OPERATION	;
	int	MODBUS_VALVE_TRAVEL_TIME 	;
	int	MODBUS_VALVE_PERCENT	;
	int	MODBUS_INTERLOCK_OUTPUT1	;
	int	MODBUS_INTERLOCK_OUTPUT2	;
	int	MODBUS_INTERLOCK_OUTPUT3	;
	int	MODBUS_INTERLOCK_OUTPUT4	;
	int	MODBUS_INTERLOCK_OUTPUT5	;
	int	MODBUS_INTERLOCK_OUTPUT6	;
	int	MODBUS_INTERLOCK_OUTPUT7	;
	int	MODBUS_SETPOINT_INCREASE	;
	int	MODBUS_FREEZE_TEMP_SETPOINT	;
	int	MODBUS_FREEZE_DELAY_ON	;
	int	MODBUS_FREEZE_DELAY_OFF	;
	int	MODBUS_ANALOG1_FUNCTION	;
	int	MODBUS_ANALOG2_FUNCTION	;
	int	MODBUS_DIGITAL1_FUNCTION	;
	int	MODBUS_DISPLAY_HUNDRED	;
	int	MODBUS_DISPLAY_TEN	;
	int	MODBUS_DISPLAY_DIGITAL	;
	int	MODBUS_DISPLAY_STATUS	;
	int	MODBUS_INPUT_MANU_ENABLE	;
	int	MODBUS_OUTPUT_MANU_ENABLE	;
	int	MODBUS_DIGITAL_INPUT	;
	int	MODBUS_DEAD_MASTER	;
	int	MODBUS_ROUND_DISPLAY	;
	int	MODBUS_MIN_ADDRESS 	;
	int	MODBUS_MAX_ADDRESS	;
	int	MODBUS_FAN_GRIDPOINT	;
	int	MODBUS_MODE_GRIDPOINT	;
	int	MODBUS_HOLD_GRIDPOINT	;
	int	MODBUS_CONFIGURATION	;
	int	MODBUS_UTIL_MODE	;
	int	MODBUS_EEPROM_SIZE	;
	int	MODBUS_TIMER_SELECT	;
	int	MODBUS_OUTPUT1_FUNCTION	;
	int	MODBUS_ROTATION_OUT2	;
	int	MODBUS_ROTATION_OUT3	;
	int	MODBUS_ROTATION_OUT4	;
	int	MODBUS_ROTATION_OUT5	;
	int	MODBUS_OUTPUT2_FUNCTION	;
	int	MODBUS_OUTPUT3_FUNCTION	;
	int	MODBUS_OUTPUT4_FUNCTION	;
	int	MODBUS_OUTPUT5_FUNCTION	;
	int	MODBUS_PIR_STAGE	;
	int	MODBUS_AIR_CALIBRATION_POINT4	;
	int	MODBUS_AIR_CALIBRATION_POINT1	;
	int	MODBUS_AIR_CALIBRATION_POINT2	;
	int	MODBUS_AIR_CALIBRATION_POINT3	;
	int	MODUBS_HUMIDITY_RH 	;
	int	MODBUS_HUMIDITY_FREQUENCY	;
	int	MODBUS_HUM_PIC_UPDATE	;
	int	MODBUS_HUM_CAL_NUM 	;
	int	MODBUS_HUM_CAL_EREASE 	;
	int	MODBUS_HUM_CURRENT_DEFAULT 	;
	int	MODBUS_HUM_PICDATAOK 	;
	int	MODBUS_HUM_OFFSETFLAG  	;
	int	MODBUS_HUMCOUNT1_H   	;
	int	MODBUS_HUMRH1_H	;
	int	MODBUS_HUMCOUNT2_H 	;
	int	MODBUS_HUMRH2_H	;
	int	MODBUS_HUMCOUNT3_H 	;
	int	MODBUS_HUMRH3_H	;
	int	MODBUS_HUMCOUNT4_H 	;
	int	MODBUS_HUMRH4_H	;
	int	MODBUS_HUMCOUNT5_H 	;
	int	MODBUS_HUMRH5_H	;
	int	MODBUS_HUMCOUNT6_H 	;
	int	MODBUS_HUMRH6_H	;
	int	MODBUS_HUMCOUNT7_H 	;
	int	MODBUS_HUMRH7_H	;
	int	MODBUS_HUMCOUNT8_H 	;
	int	MODBUS_HUMRH8_H	;
	int	MODBUS_HUMCOUNT9_H 	;
	int	MODBUS_HUMRH9_H	;
	int	MODBUS_HUMCOUNT10_H 	;
	int	MODBUS_HUMRH10_H	;
	int	MODBUS_TEM_RANGE_LOWER  	;
	int	MODBUS_TEM_RANGE_UPPER	;
	int	MODBUS_HUM_RANGE_LOWER	;
	int	MODBUS_HUM_RANGE_UPPER	;
	int	MODBUS_AQ_RANGE_LOWER	;
	int	MODBUS_AQ_RANGE_UPPER 	;
	int	MODBUS_TEMP_CURRENT_OUTPUT      	;
	int	MODBUS_HUM_CURRENT_OUTPUT	;
	int	MODBUS_AQ_CURRENT_OUTPUT	;
	int	MODBUS_TEMP_VOLTAGE_OUTPUT	;
	int	MODBUS_HUM_VOLTAGE_OUTPUT	;
	int	MODBUS_AQ_VOLTAGE_OUTPUT  	;
	int	MODBUS_CALIBRATION_MODE  	;
	int	MODBUS_SCROLL_MODE	;
	int	MODBUS_AQ_LEVEL0	;
	int	MODBUS_AQ_LEVEL1	;
	int	MODBUS_AQ_LEVEL2	;
	int	MODBUS_OUTPUT_MODE	;
	int	MODBUS_TEMP_MANU_OUTPUT	;
	int	MODBUS_HUM_MANU_OUTPUT	;
	int	MODBUS_AQ_MANU_OUTPUT	;
	int	MODBUS_HUM_CALIBRATION_LOCKX    	;
	int	MODBUS_HUM_CALIBRATION_LOCKY 	;


public:
	CMsflexgrid m_sensor_calibrate_table;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CComboBox m_combox_displayer;
	CComboBox m_edit_backlightTime;
};


