#pragma once

#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "global_struct.h"

class T38I13O : public CFormView
{
	DECLARE_DYNCREATE(T38I13O)

public:
	T38I13O();           // protected constructor used by dynamic creation
	virtual ~T38I13O();
	 
public:
	enum { IDD = IDD_T38I13O };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
public:
    void Fresh();
    void InitialDialog();
	void Initial_RegisterList();
	void InitialTableName();
	int  Get_RegID(CString Name);
	vector<T3Register> m_vecT3Register;
	bool m_isinput;
	int m_curcol;
	int m_currow;
	int m_sn;
	CString m_oldname;
	HANDLE hFirstThread;
public:
	    int	SN_LOW											 ;
		int	SN_HI											 ;
		int	EPROM_VER_NUMBER								 ;
		int	FIRMWARE_VER_NUMBER								 ;
		int	MODBUS_ID										 ;
		int	PRODUCT_MODEL									 ;
		int	HARDWARE_VER_NUMBER								 ;
		int	PIC_VER_NUMBER									 ;
		int	CALIBRATION_OUTPUTS								 ;
		int	BAUDRATE										 ;
		int	RESPONSE_DELAY									 ;
		int	OUTPUT1											 ;
		int	OUTPUT2											 ;
		int	OUTPUT3											 ;
		int	OUTPUT4											 ;
		int	OUTPUT5											 ;
		int	OUTPUT6											 ;
		int	OUTPUT7											 ;
		int	OUTPUT8											 ;
		int	OUTPUT9											 ;
		int	OUTPUT10										 ;
		int	OUTPUT11										 ;
		int	OUTPUT12										 ;
		int	OUTPUT13										 ;
		int	SWITCH1_STATUS									 ;
		int	SWITCH2_STATUS									 ;
		int	INPUT1_PULSE_COUNT_HIGHT						 ;
		int	INPUT1_PULSE_COUNT_LOW							 ;
		int	INPUT2_PULSE_COUNT_HIGHT						 ;
		int	INPUT2_PULSE_COUNT_LOW							 ;
		int	INPUT3_PULSE_COUNT_HIGHT						 ;
		int	INPUT3_PULSE_COUNT_LOW							 ;
		int	INPUT4_PULSE_COUNT_HIGHT						 ;
		int	INPUT4_PULSE_COUNT_LOW							 ;
		int	INPUT5_PULSE_COUNT_HIGHT						 ;
		int	INPUT5_PULSE_COUNT_LOW							 ;
		int	INPUT6_PULSE_COUNT_HIGHT						 ;
		int	INPUT6_PULSE_COUNT_LOW							 ;
		int	INPUT7_PULSE_COUNT_HIGHT						 ;
		int	INPUT7_PULSE_COUNT_LOW							 ;
		int	INPUT8_PULSE_COUNT_HIGHT						 ;
		int	INPUT8_PULSE_COUNT_LOW							 ;
		int	DATE_STAMP_INPUT1_YEAR							 ;
		int	DATE_STAMP_INPUT1_MONTH							 ;
		int	DATE_STAMP_INPUT1_DAY							 ;
		int	DATE_STAMP_INPUT1_HOUR							 ;
		int	DATE_STAMP_INPUT1_MINUTE						 ;
		int	DATE_STAMP_INPUT2_YEAR							 ;
		int	DATE_STAMP_INPUT2_MONTH							 ;
		int	DATE_STAMP_INPUT2_DAY							 ;
		int	DATE_STAMP_INPUT2_HOUR							 ;
		int	DATE_STAMP_INPUT2_MINUTE						 ;
		int	DATE_STAMP_INPUT3_YEAR							 ;
		int	DATE_STAMP_INPUT3_MONTH							 ;
		int	DATE_STAMP_INPUT3_DAY							 ;
		int	DATE_STAMP_INPUT3_HOUR							 ;
		int	DATE_STAMP_INPUT3_MINUTE						 ;
		int	DATE_STAMP_INPUT4_YEAR							 ;
		int	DATE_STAMP_INPUT4_MONTH							 ;
		int	DATE_STAMP_INPUT4_DAY							 ;
		int	DATE_STAMP_INPUT4_HOUR							 ;
		int	DATE_STAMP_INPUT4_MINUTE						 ;
		int	DATE_STAMP_INPUT5_YEAR							 ;
		int	DATE_STAMP_INPUT5_MONTH							 ;
		int	DATE_STAMP_INPUT5_DAY							 ;
		int	DATE_STAMP_INPUT5_HOUR							 ;
		int	DATE_STAMP_INPUT5_MINUTE						 ;
		int	DATE_STAMP_INPUT6_YEAR							 ;
		int	DATE_STAMP_INPUT6_MONTH							 ;
		int	DATE_STAMP_INPUT6_DAY							 ;
		int	DATE_STAMP_INPUT6_HOUR							 ;
		int	DATE_STAMP_INPUT6_MINUTE						 ;
		int	DATE_STAMP_INPUT7_YEAR							 ;
		int	DATE_STAMP_INPUT7_MONTH							 ;
		int	DATE_STAMP_INPUT7_DAY							 ;
		int	DATE_STAMP_INPUT7_HOUR							 ;
		int	DATE_STAMP_INPUT7_MINUTE						 ;
		int	DATE_STAMP_INPUT8_YEAR							 ;
		int	DATE_STAMP_INPUT8_MONTH							 ;
		int	DATE_STAMP_INPUT8_DAY							 ;
		int	DATE_STAMP_INPUT8_HOUR							 ;
		int	DATE_STAMP_INPUT8_MINUTE						 ;
		int	ORIGIGINAL_AD_DATA_INPUT1						 ;
		int	ORIGIGINAL_AD_DATA_INPUT2						 ;
		int	ORIGIGINAL_AD_DATA_INPUT3						 ;
		int	ORIGIGINAL_AD_DATA_INPUT4						 ;
		int	ORIGIGINAL_AD_DATA_INPUT5						 ;
		int	ORIGIGINAL_AD_DATA_INPUT6						 ;
		int	ORIGIGINAL_AD_DATA_INPUT7						 ;
		int	ORIGIGINAL_AD_DATA_INPUT8						 ;
		int	RANGE_INPUT1									 ;
		int	RANGE_INPUT2									 ;
		int	RANGE_INPUT3									 ;
		int	RANGE_INPUT4									 ;
		int	RANGE_INPUT5									 ;
		int	RANGE_INPUT6									 ;
		int	RANGE_INPUT7									 ;
		int	RANGE_INPUT8									 ;
		int	FILTER_INPUT1									 ;
		int	FILTER_INPUT2									 ;
		int	FILTER_INPUT3									 ;
		int	FILTER_INPUT4									 ;
		int	FILTER_INPUT5									 ;
		int	FILTER_INPUT6									 ;
		int	FILTER_INPUT7									 ;
		int	FILTER_INPUT8									 ;
		int	LIGHTING_ZONE_TIME_INPUT1						 ;
		int	LIGHTING_ZONE_TIME_INPUT2						 ;
		int	LIGHTING_ZONE_TIME_INPUT3						 ;
		int	LIGHTING_ZONE_TIME_INPUT4						 ;
		int	LIGHTING_ZONE_TIME_INPUT5						 ;
		int	LIGHTING_ZONE_TIME_INPUT6						 ;
		int	LIGHTING_ZONE_TIME_INPUT7						 ;
		int	LIGHTING_ZONE_TIME_INPUT8						 ;
		int	ZONE_TIME_LEFT_INPUT1							 ;
		int	ZONE_TIME_LEFT_INPUT2							 ;
		int	ZONE_TIME_LEFT_INPUT3							 ;
		int	ZONE_TIME_LEFT_INPUT4							 ;
		int	ZONE_TIME_LEFT_INPUT5							 ;
		int	ZONE_TIME_LEFT_INPUT6							 ;
		int	ZONE_TIME_LEFT_INPUT7							 ;
		int	ZONE_TIME_LEFT_INPUT8							 ;
		int	AUTO_MANUAL_OUTPUTS								 ;
		int	LIGHT_SWITCH_OUTPUT1							 ;
		int	LIGHT_SWITCH_OUTPUT2							 ;
		int	LIGHT_SWITCH_OUTPUT3							 ;
		int	LIGHT_SWITCH_OUTPUT4							 ;
		int	LIGHT_SWITCH_OUTPUT5							 ;
		int	LIGHT_SWITCH_OUTPUT6							 ;
		int	LIGHT_SWITCH_OUTPUT7							 ;
		int	LIGHT_SWITCH_OUTPUT8							 ;
		int	LIGHT_SWITCH_OUTPUT9							 ;
		int	LIGHT_SWITCH_OUTPUT10							 ;
		int	LIGHT_SWITCH_OUTPUT11							 ;
		int	LIGHT_SWITCH_OUTPUT12							 ;
		int	LIGHT_SWITCH_OUTPUT13							 ;

public:
	CMsflexgrid m_msflexgrid_input;
	CMsflexgrid m_msflexgrid_output;
	CComboBox m_comboxRange;
	CEdit m_inNameEdt;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridInput();
	afx_msg void OnCbnSelchangeRangecombo();
	void ClickMsflexgridOutput();
	afx_msg void OnEnKillfocusEditName();
	CComboBox m_brandratebombox;
//	CComboBox m_delaycombox;
	afx_msg void OnCbnSelchangeBrandrate();
//	afx_msg void OnCbnSelchangeDelay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonReset();
    afx_msg void OnEnKillfocusEditResponseDelay();
};


