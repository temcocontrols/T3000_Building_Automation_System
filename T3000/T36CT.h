#pragma once

#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "global_struct.h"

// T36CT form view

class T36CT : public CFormView
{
	DECLARE_DYNCREATE(T36CT)

public:
	T36CT();           // protected constructor used by dynamic creation
	virtual ~T36CT();
     virtual void OnInitialUpdate();
public:
	enum { IDD = IDD_T36CT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
   
#endif
#endif
public:
    void Fresh();
    void InitialDialog();
    void Initial_RegisterList();
    int  Get_RegID(CString Name);
    void InitialTableName();
    vector<T3Register> m_vecT3Register;
    int m_sn;
    CString m_oldname;
    int m_curcol;
    int m_currow;
	HANDLE hFirstThread;
private:
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
    int	DEADMASTER_TIME	;
    int	REAL_STATUS	;
    int	RESPONSE_DELAY	;
    int	OUTPUT1	;
    int	OUTPUT2	;
    int	OUTPUT3	;
    int	OUTPUT4	;
    int	OUTPUT5	;
    int	INPUT1_CT	;
    int	INPUT2_CT	;
    int	INPUT3_CT	;
    int	INPUT4_CT	;
    int	INPUT5_CT	;
    int	INPUT6_CT	;
    int	INPUT1	;
    int	INPUT2	;
    int	INPUT3	;
    int	INPUT4	;
    int	INPUT5	;
    int	INPUT6	;
    int	INPUT7	;
    int	INPUT8	;
    int	INPUT9	;
    int	INPUT10	;
    int	SWITCH1_BANK	;
    int	SWITCH2_BANK	;
    int	RANGE_INPUT1	;
    int	RANGE_INPUT2	;
    int	RANGE_INPUT3	;
    int	RANGE_INPUT4	;
    int	RANGE_INPUT5	;
    int	RANGE_INPUT6	;
    int	RANGE_INPUT7	;
    int	RANGE_INPUT8	;
    int	RANGE_INPUT9	;
    int	RANGE_INPUT10	;
    int	RANGE_INPUT11	;
    int	RANGE_INPUT12	;
    int	RANGE_INPUT13	;
    int	RANGE_INPUT14	;
    int	RANGE_INPUT15	;
    int	RANGE_INPUT16	;
    int	FILTER_INPUT1	;
    int	FILTER_INPUT2	;
    int	FILTER_INPUT3	;
    int	FILTER_INPUT4	;
    int	FILTER_INPUT5	;
    int	FILTER_INPUT6	;
    int	FILTER_INPUT7	;
    int	FILTER_INPUT8	;
    int	FILTER_INPUT9	;
    int	FILTER_INPUT10	;
    int	FILTER_INPUT11	;
    int	FILTER_INPUT12	;
    int	FILTER_INPUT13	;
    int	FILTER_INPUT14	;
    int	FILTER_INPUT15	;
    int	FILTER_INPUT16	;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedChangeid();
    CMsflexgrid m_msflexgrid_input;
    CMsflexgrid m_msflexgrid_output;
    CMsflexgrid m_msflexgrid_Ainput;
    DECLARE_EVENTSINK_MAP()
    void ClickMsflexgridOutput();
    CEdit m_inNameEdt;
    CComboBox m_comboxRange;
    void ClickMsflexgridInput();
    void ClickMsflexgridInput2();
    int grid_flag;
    //CT-Input grid_flag=1
    //Analog-Input grid_flag=2
    //Output grid_flag=3
    afx_msg void OnEnKillfocusEditName();
    afx_msg void OnCbnSelchangeRangecombo();
	CComboBox m_brandrateBoxbox;
	afx_msg void OnCbnSelchangeBaudratecombo();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	void EnterCellMsflexgridOutput();
	afx_msg void OnBnClickedButtonReset();
};


