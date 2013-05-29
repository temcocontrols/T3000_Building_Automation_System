#pragma once
#include "afxwin.h"
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
	CComboBox m_co2_braudRateCombox;
	float m_co2_firmwareversion;
	UINT m_co2_hardwareversion;
	UINT m_co2_serialNumber;

	bool m_temp_unit_c0_f1;
	CString cs_temp_unit;
	CComboBox m_co2_temp_unit;
public:
	void Get_CO2_Temperature_unit(CString &strTemp);
	void CO2_Alarm_Set();
	void CO2_Grid_init();

	CComboBox m_co2_alarm_state;
	CButton m_co2_alarm_mode_ctr;
	BOOL m_alarm_ctrl_manual_auto;
	short m_alarm_on_time;
	short m_alarm_off_time;
	short m_edit_pre_alarm_setpoint;
	short m_edit_alarm_setpoint;
	short m_edit_calibrating_offset;
	CMsflexgrid m_flex_co2_grid;
	afx_msg void OnBnClickedCo2Enableidbutton();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridCo2();
};


