#pragma once

#include "global_struct.h"
#include "msflexgrid1.h"
#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
 

class T38AI8AO : public CFormView
{
	DECLARE_DYNCREATE(T38AI8AO)

public:
	T38AI8AO();           // protected constructor used by dynamic creation
	virtual ~T38AI8AO();
	
public:
	enum { IDD = IDD_T38AI8AO };
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
void InitialRegister();
int  Get_RegID(CString Name);
//vector<T3Register> m_Reglist;
bool m_isinput;
int m_curcol;
int m_currow;
void Fresh();
void InitialDialog();
public:
	CMsflexgrid m_msflexgrid_input;
	CMsflexgrid m_msflexgrid_output;
	CString m_address;
	HANDLE hFirstThread;
private:
	vector<T3Register> m_vecT3Register;
	int	SN_LOW	;
	int	SN_HI	;
	int	EPROM_VER_NUMBER	;
	int	FIRMWARE_VER_NUMBER	;
	int	MODBUS_ID	;
	int	PRODUCT_MODEL	;
	int	HARDWARE_VER_NUMBER	;
	int	PIC_VER_NUMBER	;
	int	CALIBRATION_OUTPUTS	;
	int	BAUDRATE	;
	int	RESPONSE_DELAY	;

	int	OUTPUT1	;
	int	OUTPUT2	;
	int	OUTPUT3	;
	int	OUTPUT4	;
	int	OUTPUT5	;
	int	OUTPUT6	;
	int	OUTPUT7	;
	int	OUTPUT8	;
	int	INPUT1	;
	int	INPUT2	;
	int	INPUT3	;
	int	INPUT4	;
	int	INPUT5	;
	int	INPUT6	;
	int	INPUT7	;
	int	INPUT8	;
	int	SWITCH1_STATUS	;
	int	SWITCH2_STATUS	;
	int	RANGE_INPUT1	;
	int	RANGE_INPUT2	;
	int	RANGE_INPUT3	;
	int	RANGE_INPUT4	;
	int	RANGE_INPUT5	;
	int	RANGE_INPUT6	;
	int	RANGE_INPUT7	;
	int	RANGE_INPUT8	;
	int	FILTER_INPUT1	;
	int	FILTER_INPUT2	;
	int	FILTER_INPUT3	;
	int	FILTER_INPUT4	;
	int	FILTER_INPUT5	;
	int	FILTER_INPUT6	;
	int	FILTER_INPUT7	;
	int	FILTER_INPUT8	;
public:
	CComboBox m_brandratebombox;
	CComboBox m_delaycombox;
	afx_msg void OnCbnSelchangeBrandrate();
//	afx_msg void OnCbnSelchangeDelay();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridInput();
	void ClickMsflexgridOutput();
	CComboBox m_comboxRange;
	CEdit m_inNameEdt;
	afx_msg void OnEnKillfocusEditName();
	afx_msg void OnCbnSelchangeRangecombo();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonReset();
    afx_msg void OnEnKillfocusEditResponseDelay();
};


