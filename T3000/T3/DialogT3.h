#pragma once
#include "../msflexgrid1.h"
#include "../resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "../global_struct.h"
#define FLEXGRID_CELL_COLOR						13421772
#define FLEXGRID_CELL_GRAY_COLOR				13421772





typedef struct T3_MODEL 
{
	int iaddress;
	CString CStName;
	CString CStvalue;
	CString CStautomannel;
	CString CStrange;
}t3model;

// CDialogT3 form view

class CDialogT3 : public CFormView
{
	DECLARE_DYNCREATE(CDialogT3)

protected:
public:
	CDialogT3();           // protected constructor used by dynamic creation
	virtual ~CDialogT3();

public:
	enum { IDD = IDD_DIALOG_T3 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	DECLARE_MESSAGE_MAP()
	int m_address;
	float m_firmware;
	long m_serial;
	int m_hardware;
	CString m_model;
	int m_picVersion;
	CMsflexgrid m_msflexgrid_input;
	CMsflexgrid m_msflexgrid_output;
	virtual void OnInitialUpdate();
	DECLARE_EVENTSINK_MAP()
	void ClickInputMsflexgrid();
	void ClickOutputMsflexgrid();
 void OnCbnSelchangeType();
public:
	void Fresh();
	void ShowDialogData();
	 

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	unsigned short shmPRegister[512];
	vector<t3model>vect3model;
	t3model m_t3model;
	int comnum;//��ȡ���ں�
	BOOL MiniUpdateData();
	void Updatedatabase();
	CProgressCtrl m_progress;
	CEdit m_inNameEdt;
	void Initial_RegisterList();
	int  Get_RegID(CString Name);
	vector<T3Register> m_vecT3Register;
private:
	int     SN_LOW									;
	int 	SN_HI									;
	int 	EPROM_VER_NUMBER						;
	int 	FIRMWARE_VER_NUMBER						;
	int 	MODBUS_ID								;
	int 	PRODUCT_MODEL							;
	int 	HARDWARE_VER_NUMBER						;
	int 	PIC_VER_NUMBER							;
	int 	CALIBRATION_OUTPUTS						;
	int 	BAUDRATE								;
	int 	RESPONSE_DELAY							;
	int 	OUTPUT1_RELAY							;
	int 	OUTPUT2_RELAY							;
	int 	OUTPUT3_RELAY							;
	int 	OUTPUT4_RELAY							;
	int 	OUTPUT5_RELAY							;
	int 	OUTPUT6_RELAY							;
	int 	OUTPUT7_RELAY							;
	int 	OUTPUT8_RELAY							;
	int 	OUTPUT1_ANALOG							;
	int 	OUTPUT2_ANALOG							;
	int 	OUTPUT3_ANALOG							;
	int 	OUTPUT4_ANALOG							;
	int 	SWITCH1_STATUS							;
	int 	SWITCH2_STATUS							;
	int 	SWITCH3_BANK							;
	int 	INPUT1_PULSE_COUNT_HIGHT				;
	int 	INPUT1_PULSE_COUNT_LOW					;
	int 	INPUT2_PULSE_COUNT_HIGHT				;
	int 	INPUT2_PULSE_COUNT_LOW					;
	int 	INPUT3_PULSE_COUNT_HIGHT				;
	int 	INPUT3_PULSE_COUNT_LOW					;
	int 	INPUT4_PULSE_COUNT_HIGHT				;
	int 	INPUT4_PULSE_COUNT_LOW					;
	int 	INPUT5_PULSE_COUNT_HIGHT				;
	int 	INPUT5_PULSE_COUNT_LOW					;
	int 	INPUT6_PULSE_COUNT_HIGHT				;
	int 	INPUT6_PULSE_COUNT_LOW					;
	int 	INPUT7_PULSE_COUNT_HIGHT				;
	int 	INPUT7_PULSE_COUNT_LOW					;
	int 	INPUT8_PULSE_COUNT_HIGHT				;
	int 	INPUT8_PULSE_COUNT_LOW					;
	int 	INPUT9_PULSE_COUNT_HIGHT				;
	int 	INPUT9_PULSE_COUNT_LOW					;
	int 	INPUT10_PULSE_COUNT_HIGHT				;
	int 	INPUT10_PULSE_COUNT_LOW					;
	int 	DATE_STAMP_INPUT1_YEAR					;
	int 	DATE_STAMP_INPUT1_MONTH					;
	int 	DATE_STAMP_INPUT1_DAY					;
	int 	DATE_STAMP_INPUT1_HOUR					;
	int 	DATE_STAMP_INPUT1_MINUTE				;
	int 	DATE_STAMP_INPUT2_YEAR					;
	int 	DATE_STAMP_INPUT2_MONTH					;
	int 	DATE_STAMP_INPUT2_DAY					;
	int 	DATE_STAMP_INPUT2_HOUR					;
	int 	DATE_STAMP_INPUT2_MINUTE				;
	int 	DATE_STAMP_INPUT3_YEAR					;
	int 	DATE_STAMP_INPUT3_MONTH					;
	int 	DATE_STAMP_INPUT3_DAY					;
	int 	DATE_STAMP_INPUT3_HOUR					;
	int 	DATE_STAMP_INPUT3_MINUTE				;
	int 	DATE_STAMP_INPUT4_YEAR					;
	int 	DATE_STAMP_INPUT4_MONTH					;
	int 	DATE_STAMP_INPUT4_DAY					;
	int 	DATE_STAMP_INPUT4_HOUR					;
	int 	DATE_STAMP_INPUT4_MINUTE				;
	int 	DATE_STAMP_INPUT5_YEAR					;
	int 	DATE_STAMP_INPUT5_MONTH					;
	int 	DATE_STAMP_INPUT5_DAY					;
	int 	DATE_STAMP_INPUT5_HOUR					;
	int 	DATE_STAMP_INPUT5_MINUTE				;
	int 	DATE_STAMP_INPUT6_YEAR					;
	int 	DATE_STAMP_INPUT6_MONTH					;
	int 	DATE_STAMP_INPUT6_DAY					;
	int 	DATE_STAMP_INPUT6_HOUR					;
	int 	DATE_STAMP_INPUT6_MINUTE				;
	int 	DATE_STAMP_INPUT7_YEAR					;
	int 	DATE_STAMP_INPUT7_MONTH					;
	int 	DATE_STAMP_INPUT7_DAY					;
	int 	DATE_STAMP_INPUT7_HOUR					;
	int 	DATE_STAMP_INPUT7_MINUTE				;
	int 	DATE_STAMP_INPUT8_YEAR					;
	int 	DATE_STAMP_INPUT8_MONTH					;
	int 	DATE_STAMP_INPUT8_DAY					;
	int 	DATE_STAMP_INPUT8_HOUR					;
	int 	DATE_STAMP_INPUT8_MINUTE				;
	int 	DATE_STAMP_INPUT9_YEAR					;
	int 	DATE_STAMP_INPUT9_MONTH					;
	int 	DATE_STAMP_INPUT9_DAY					;
	int 	DATE_STAMP_INPUT9_HOUR					;
	int 	DATE_STAMP_INPUT9_MINUTE				;
	int 	DATE_STAMP_INPUT10_YEAR					;
	int 	DATE_STAMP_INPUT10_MONTH				;
	int 	DATE_STAMP_INPUT10_DAY					;
	int 	DATE_STAMP_INPUT10_HOUR					;
	int 	DATE_STAMP_INPUT10_MINUTE				;
	int 	AUTO_MANUAL_OUTPUTS						;
	int 	ORIGIGINAL_AD_DATA_INPUT1				;
	int 	ORIGIGINAL_AD_DATA_INPUT2				;
	int 	ORIGIGINAL_AD_DATA_INPUT3				;
	int 	ORIGIGINAL_AD_DATA_INPUT4				;
	int 	ORIGIGINAL_AD_DATA_INPUT5				;
	int 	ORIGIGINAL_AD_DATA_INPUT6				;
	int 	ORIGIGINAL_AD_DATA_INPUT7				;
	int 	ORIGIGINAL_AD_DATA_INPUT8				;
	int 	ORIGIGINAL_AD_DATA_INPUT9				;
	int 	ORIGIGINAL_AD_DATA_INPUT10				;
	int 	RANGE_INPUT1							;
	int 	RANGE_INPUT2							;
	int 	RANGE_INPUT3							;
	int 	RANGE_INPUT4							;
	int 	RANGE_INPUT5							;
	int 	RANGE_INPUT6							;
	int 	RANGE_INPUT7							;
	int 	RANGE_INPUT8							;
	int     RANGE_INPUT9							;
	int 	RANGE_INPUT10							;
	int 	FILTER_INPUT1							;
	int 	FILTER_INPUT2							;
	int 	FILTER_INPUT3							;
	int 	FILTER_INPUT4							;
	int 	FILTER_INPUT5							;
	int 	FILTER_INPUT6							;
	int 	FILTER_INPUT7							;
	int 	FILTER_INPUT8							;
	int 	FILTER_INPUT9							;
	int 	FILTER_INPUT10							;
	int 	LIGHTING_ZONE_TIME_INPUT1				;
	int 	LIGHTING_ZONE_TIME_INPUT2				;
	int 	LIGHTING_ZONE_TIME_INPUT3				;
	int 	LIGHTING_ZONE_TIME_INPUT4				;
	int 	LIGHTING_ZONE_TIME_INPUT5				;
	int 	LIGHTING_ZONE_TIME_INPUT6				;
	int 	LIGHTING_ZONE_TIME_INPUT7				;
	int 	LIGHTING_ZONE_TIME_INPUT8				;
	int 	LIGHTING_ZONE_TIME_INPUT9				;
	int 	LIGHTING_ZONE_TIME_INPUT10				;
	int 	ZONE_TIME_LEFT_INPUT1					;
	int 	ZONE_TIME_LEFT_INPUT2					;
	int 	ZONE_TIME_LEFT_INPUT3					;
	int 	ZONE_TIME_LEFT_INPUT4					;
	int 	ZONE_TIME_LEFT_INPUT5					;
	int 	ZONE_TIME_LEFT_INPUT6					;
	int 	ZONE_TIME_LEFT_INPUT7					;
	int 	ZONE_TIME_LEFT_INPUT8					;
	int 	ZONE_TIME_LEFT_INPUT9					;
	int 	ZONE_TIME_LEFT_INPUT10					;
	int 	LC_DISABLE_ENABLE						;
	int 	SELECT_INPUT_AS_TRIGGER_1				;
	int 	SELECT_INPUT_AS_TRIGGER_2				;
	int 	SELECT_INPUT_AS_TRIGGER_3				;
	int 	SELECT_INPUT_AS_TRIGGER_4				;
	int 	SELECT_INPUT_AS_TRIGGER_5				;
	int 	SELECT_INPUT_AS_TRIGGER_6				;
	int 	SELECT_INPUT_AS_TRIGGER_7				;
	int 	SELECT_INPUT_AS_TRIGGER_8				;
public:
	CComboBox m_comboxRange;
};