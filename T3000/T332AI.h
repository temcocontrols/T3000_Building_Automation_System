#pragma once


#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "global_struct.h"
// T332AI form view

class T332AI : public CFormView
{
	DECLARE_DYNCREATE(T332AI)

public:
	T332AI();           // protected constructor used by dynamic creation
	virtual ~T332AI();
	int m_sn;
	CString m_oldname;
public:
	enum { IDD = IDD_T332AI };
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
	int  Get_RegID(CString Name);
	vector<T3Register> m_vecT3Register;
	 
	int m_curcol;
	int m_currow;
   
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
	int	RESPONSE_DELAY	;
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
	int	INPUT11	;
	int	INPUT12	;
	int	INPUT13	;
	int	INPUT14	;
	int	INPUT15	;
	int	INPUT16	;
	int	INPUT17	;
	int	INPUT18	;
	int	INPUT19	;
	int	INPUT20	;
	int	INPUT21	;
	int	INPUT22	;
	int	INPUT23	;
	int	INPUT24	;
	int	INPUT25	;
	int	INPUT26	;
	int	INPUT27	;
	int	INPUT28	;
	int	INPUT29	;
	int	INPUT30	;
	int	INPUT31	;
	int	INPUT32	;
	int	Range1	;
	int	Range2	;
	int	Range3	;
	int	Range4	;
	int	Range5	;
	int	Range6	;
	int	Range7	;
	int	Range8	;
	int	Range9	;
	int	Range10	;
	int	Range11	;
	int	Range12	;
	int	Range13	;
	int	Range14	;
	int	Range15	;
	int	Range16	;
	int	Range17	;
	int	Range18	;
	int	Range19	;
	int	Range20	;
	int	Range21	;
	int	Range22	;
	int	Range23	;
	int	Range24	;
	int	Range25	;
	int	Range26	;
	int	Range27	;
	int	Range28	;
	int	Range29	;
	int	Range30	;
	int	Range31	;
	int	Range32	;

	int	Filter1	;
	int	Filter2	;
	int	Filter3	;
	int	Filter4	;
	int	Filter5	;
	int	Filter6	;
	int	Filter7	;
	int	Filter8	;
	int	Filter9	;
	int	Filter10	;
	int	Filter11	;
	int	Filter12	;
	int	Filter13	;
	int	Filter14	;
	int	Filter15	;
	int	Filter16	;
	int	Filter17	;
	int	Filter18	;
	int	Filter19	;
	int	Filter20	;
	int	Filter21	;
	int	Filter22	;
	int	Filter23	;
	int	Filter24	;
	int	Filter25	;
	int	Filter26	;
	int	Filter27	;
	int	Filter28	;
	int	Filter29	;
	int	Filter30	;
	int	Filter31	;
	int	Filter32	;


public:
	CMsflexgrid m_msflexgrid_input;
	CEdit m_inNameEdt;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridInput();
	afx_msg void OnEnKillfocusEditName();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	CComboBox m_comboxRange;
	afx_msg void OnCbnSelchangeRangecombo();
	afx_msg void OnBnClickedButtonReset();
//    afx_msg void OnCbnSelchangeDelay();
    afx_msg void OnEnKillfocusEditResponseDelay();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};


