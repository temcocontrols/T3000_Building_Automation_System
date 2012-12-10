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
	//CMsflexgrid m_msflexgrid;
	
public:
	virtual BOOL OnInitDialog();
	CMsflexgrid m_msflexgrid;
	CComboBox m_controlcombo;
	CString m_strcombo;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridConfigure();
	afx_msg void OnCbnKillfocusComboSelect();
	CEdit m_edit;
	afx_msg void OnEnKillfocusEdit1();
	WORD m_switch[24];
	WORD m_overridetime[24];
	afx_msg void OnBnClickedButtonSend();
};
