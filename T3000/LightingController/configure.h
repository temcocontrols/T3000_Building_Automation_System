#pragma once
#include "../msflexgrid1.h"
#include "afxwin.h"
#include "..\MainFrm.h"



// Cconfigure dialog

class Cconfigure : public CDialog
{
	DECLARE_DYNAMIC(Cconfigure)

public:
	Cconfigure(CWnd* pParent = NULL);   // standard constructor
	virtual ~Cconfigure();

// Dialog Data
	enum { IDD = IDD_DIALOG_Configure };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	 
	
public:
    int m_sn;
    int m_CurRow;
	int m_CurCol;
	CString m_oldname;
	CString m_newname;
	virtual BOOL OnInitDialog();
    void Refresh();
    void ChangingLeftTime();
	CMsflexgrid m_msflexgrid;
	CComboBox m_controlcombo;
    HANDLE hFirstThread;
	CString m_strcombo;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridConfigure();
	afx_msg void OnCbnKillfocusComboSelect();
	CEdit m_edit;
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnCbnSelchangeRangecombo();
	WORD m_switch[24];
	WORD m_overridetime[24];
	WORD m_MB[24];
	WORD m_DT[24];
    WORD m_LT[24];
    WORD m_AM[24];
	afx_msg void OnBnClickedButtonSend();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
