#pragma once
#include "../msflexgrid1.h"

#include "../MainFrm.h"
#include "afxwin.h"
#include "../global_struct.h"

#include "../CM5/ListCtrlEx.h"

#include "../ChartCtrl/ChartCtrl.h"
#include "../ChartCtrl/ChartLineSerie.h"
#define AirQualityTimer							1

typedef struct AIRQUALITY
{
	int iaddress;
	CString CStvalue;
} airquality;


// CAirQuality form view

class CAirQuality : public CFormView
{
	DECLARE_DYNCREATE(CAirQuality)

protected:
public:
	CAirQuality(); // protected constructor used by dynamic creation
	virtual ~CAirQuality();

public:
	enum
	{
		IDD = IDD_DIALOG_AIRQUALITY
	};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEnKillfocustempBacklight();
	afx_msg void OnEnKillfocusMaxScaleAQ();
	afx_msg void OnEnKillfocusChangeHumName();
	afx_msg void OnEnChangeBacklight();
	//afx_msg void OnEnKillfocusBackLightTime();


	afx_msg void OnBnClickedButton_Change_ID();
	afx_msg void OnBnClickedButton_Graphic();
	afx_msg void OnBnClickedButton_Factory_Reset();

	afx_msg void OnBnClickedButton_UserTable_Add();
	afx_msg void OnBnClickedButton_UserTable_Del();


	afx_msg void OnCbnSelchangeComboDisplay();
	afx_msg void OnCbnSelchangeComboBacklight();
	afx_msg void OnBnClickedButton_Tranducer();
	afx_msg void OnBnClickedButton_PID();
	void ShowChecks();
	afx_msg void OnBnClickedUser();
	afx_msg void OnBnClickedDefault();

	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	HANDLE hFirstThread;
	void Fresh();
	void Fresh_Window();
	virtual void OnInitialUpdate();
	void Initial_RegisterList();
	vector<T3Register> m_vecT3Register;
	afx_msg void OnDestroy();
	void ShowAirqualityDialog();
	int Get_RegID(CString Name);


	void InitChartData();


	// 	void InitChartCtrl();
	// 	void DrawerPoint();
	// 	void UpdatePoint();

	void Fresh_Input_Output();
	afx_msg void OnCbnSelendcancelBaudratecombo();
	unsigned short Default_Value[30];
public:
	int m_product_model;
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
	int MODBUS_SERIALNUMBER_LOWORD;
	int MODBUS_SERIALNUMBER_HIWORD;
	int MODBUS_VERSION_NUMBER_LO;
	int MODBUS_VERSION_NUMBER_HI;
	int MODBUS_ADDRESS;
	int MODBUS_PRODUCT_MODEL;
	int MODBUS_HARDWARE_REV;
	int MODBUS_PIC_VERSION;
	int MODBUS_HUM_PIC_VERSION;
	int MODBUS_ADDRESS_PLUG_N_PLAY;
	int MODBUS_BASE_ADDRESS;
	int MODBUS_UPDATE_STATUS;
	int MODBUS_UPDATE_PTR_HI;
	int MODBUS_UPDATE_PTR_LO;
	int MODBUS_SCRATCHPAD_ADDRESS;
	int MODBUS_TEMPRATURE_CHIP_F;
	int MODBUS_TEMPRATURE_CHIP;
	int MODBUS_HUM;
	int MODBUS_AQ;
	int MODBUS_CALIBRATION_TEMP;
	int MODBUS_CALIBRATION_AQ;
	int MODBUS_CALIBRATION_HUM;
	int MODBUS_TEMP_SELECT;
	int MODBUS_DAC_OFFSET;
	int MODBUS_DELAY;
	int MODBUS_SEQUENCE;
	int MODBUS_DEGC_OR_F;
	int MODBUS_POWERUP_MODE;
	int MODBUS_AUTO_ONLY;
	int MODBUS_OUTPUT_RANGE;
	int MODBUS_FACTORY_DEFAULTS;
	int MODBUS_EXTERNAL_SENSOR_0;
	int MODBUS_EXTERNAL_SENSOR_1;
	int MODBUS_INFO_BYTE;
	int MODBUS_BAUDRATE;
	int MODBUS_HUM_FILTER;
	int MODBUS_AQ_FILTER;
	int MODBUS_DIGITAL_IN1;
	int MODBUS_CYCLING_DELAY;
	int MODBUS_CHANGOVER_DELAY;
	int MODBUS_OVERRIDE_TIMER;
	int MODBUS_OVERRIDE_TIMER_LEFT;
	int MODBUS_FILTER;
	int MODBUS_HEAT_COOL_CONFIG;
	int MODBUS_INTERNAL_TEMP_IC;
	int MODBUS_INTERNAL_THERMISTOR;
	int MODBUS_CALIBRATION_INTERNAL_THERMISTOR;
	int MODBUS_CALIBRATION_ANALOG_IN2;
	int MODBUS_INPUT1_SELECT;
	int MODBUS_UNIVERSAL_NIGHTSET;
	int MODBUS_HEAT_ORIGINAL_TABLE;
	int MODBUS_COOL_ORIGINAL_TABLE;
	int MODBUS_PID2_MODE_OPERATION;
	int MODBUS_VALVE_TRAVEL_TIME;
	int MODBUS_VALVE_PERCENT;
	int MODBUS_INTERLOCK_OUTPUT1;
	int MODBUS_INTERLOCK_OUTPUT2;
	int MODBUS_INTERLOCK_OUTPUT3;
	int MODBUS_INTERLOCK_OUTPUT4;
	int MODBUS_INTERLOCK_OUTPUT5;
	int MODBUS_INTERLOCK_OUTPUT6;
	int MODBUS_INTERLOCK_OUTPUT7;
	int MODBUS_SETPOINT_INCREASE;
	int MODBUS_FREEZE_TEMP_SETPOINT;
	int MODBUS_FREEZE_DELAY_ON;
	int MODBUS_FREEZE_DELAY_OFF;
	int MODBUS_ANALOG1_FUNCTION;
	int MODBUS_ANALOG2_FUNCTION;
	int MODBUS_DIGITAL1_FUNCTION;
	int MODBUS_DISPLAY_HUNDRED;
	int MODBUS_DISPLAY_TEN;
	int MODBUS_DISPLAY_DIGITAL;
	int MODBUS_DISPLAY_STATUS;
	int MODBUS_INPUT_MANU_ENABLE;
	int MODBUS_OUTPUT_MANU_ENABLE;
	int MODBUS_DIGITAL_INPUT;
	int MODBUS_DEAD_MASTER;
	int MODBUS_ROUND_DISPLAY;
	int MODBUS_MIN_ADDRESS;
	int MODBUS_MAX_ADDRESS;
	int MODBUS_FAN_GRIDPOINT;
	int MODBUS_MODE_GRIDPOINT;
	int MODBUS_HOLD_GRIDPOINT;
	int MODBUS_CONFIGURATION;
	int MODBUS_UTIL_MODE;
	int MODBUS_EEPROM_SIZE;
	int MODBUS_TIMER_SELECT;
	int MODBUS_OUTPUT1_FUNCTION;
	int MODBUS_ROTATION_OUT2;
	int MODBUS_ROTATION_OUT3;
	int MODBUS_ROTATION_OUT4;
	int MODBUS_ROTATION_OUT5;
	int MODBUS_OUTPUT2_FUNCTION;
	int MODBUS_OUTPUT3_FUNCTION;
	int MODBUS_OUTPUT4_FUNCTION;
	int MODBUS_OUTPUT5_FUNCTION;
	int MODBUS_PIR_STAGE;
	int MODBUS_AIR_CALIBRATION_POINT4;
	int MODBUS_AIR_CALIBRATION_POINT1;
	int MODBUS_AIR_CALIBRATION_POINT2;
	int MODBUS_AIR_CALIBRATION_POINT3;
	int MODUBS_HUMIDITY_RH;
	int MODBUS_HUMIDITY_FREQUENCY;
	int MODBUS_HUM_PIC_UPDATE;
	int MODBUS_HUM_CAL_NUM;
	int MODBUS_HUM_CAL_EREASE;
	int MODBUS_HUM_CURRENT_DEFAULT;
	int MODBUS_HUM_PICDATAOK;
	int MODBUS_HUM_OFFSETFLAG;
	int MODBUS_HUMCOUNT1_H;
	int MODBUS_HUMRH1_H;
	int MODBUS_HUMCOUNT2_H;
	int MODBUS_HUMRH2_H;
	int MODBUS_HUMCOUNT3_H;
	int MODBUS_HUMRH3_H;
	int MODBUS_HUMCOUNT4_H;
	int MODBUS_HUMRH4_H;
	int MODBUS_HUMCOUNT5_H;
	int MODBUS_HUMRH5_H;
	int MODBUS_HUMCOUNT6_H;
	int MODBUS_HUMRH6_H;
	int MODBUS_HUMCOUNT7_H;
	int MODBUS_HUMRH7_H;
	int MODBUS_HUMCOUNT8_H;
	int MODBUS_HUMRH8_H;
	int MODBUS_HUMCOUNT9_H;
	int MODBUS_HUMRH9_H;
	int MODBUS_HUMCOUNT10_H;
	int MODBUS_HUMRH10_H;
	int MODBUS_TEM_RANGE_LOWER;
	int MODBUS_TEM_RANGE_UPPER;
	int MODBUS_HUM_RANGE_LOWER;
	int MODBUS_HUM_RANGE_UPPER;
	int MODBUS_AQ_RANGE_LOWER;
	int MODBUS_AQ_RANGE_UPPER;
	int MODBUS_TEMP_CURRENT_OUTPUT;
	int MODBUS_HUM_CURRENT_OUTPUT;
	int MODBUS_AQ_CURRENT_OUTPUT;
	int MODBUS_TEMP_VOLTAGE_OUTPUT;
	int MODBUS_HUM_VOLTAGE_OUTPUT;
	int MODBUS_AQ_VOLTAGE_OUTPUT;
	int MODBUS_CALIBRATION_MODE;
	int MODBUS_SCROLL_MODE;
	int MODBUS_AQ_LEVEL0;
	int MODBUS_AQ_LEVEL1;
	int MODBUS_AQ_LEVEL2;
	int MODBUS_OUTPUT_MODE;
	int MODBUS_TEMP_MANU_OUTPUT;
	int MODBUS_HUM_MANU_OUTPUT;
	int MODBUS_AQ_MANU_OUTPUT;
	int MODBUS_HUM_CALIBRATION_LOCKX;
	int MODBUS_HUM_CALIBRATION_LOCKY;

	DECLARE_EVENTSINK_MAP()
	/*void ClickMsflexgrid1();*/
public:
	//CMsflexgrid m_sensor_calibrate_table;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CComboBox m_combox_displayer;
	CEdit m_edit_backlightTime;
	CComboBox m_combox_backlightTime;
	CEdit m_edit_calibration;
	int m_nCurRow;
	int m_nCurCol;
	CButton m_check_user;
	CButton m_check_default;
	CStatic m_Picture;
private: //画图所需要的数据
	int m_Draw_User;
	int m_offset;
	int m_Xmax;
	int m_Xmin;
	int m_Ymax;
	int m_Ymin;
	short User_Xdata[10];
	short User_Ydata[10];
	short Factory_Xdata[10];
	short Factory_Ydata[10];


public:
	CChartCtrl m_ChartCtrl;

	ListCtrlEx::CListCtrlEx m_input_list;
	ListCtrlEx::CListCtrlEx m_output_list;
	ListCtrlEx::CListCtrlEx m_user_list;
	ListCtrlEx::CListCtrlEx m_factory_list;
	int m_list_type;// list Type is output or input ,input = 0 ,output =1.
	//Here ,
public:
	void Initial_InputList();
	void Initial_OutputList();
	void Initial_UserList();
	void Initial_FactoryList();
	afx_msg LRESULT InputMessageCallBack_AQ(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Input_List_AQ(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Input_Item_AQ(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickList1_AQ(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickList2_AQ(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnNMClickList_User(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit m_max_scale_aq_edit;
	int m_current_TstatID;
	CComboBox m_Combox_baudrate;
};
