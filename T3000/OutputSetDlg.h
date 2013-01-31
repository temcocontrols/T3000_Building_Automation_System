#pragma once
#include "afxwin.h"
#include "msflexgrid1.h"

// COutputSetDlg dialog

class COutputSetDlg : public CDialog
{
	DECLARE_DYNAMIC(COutputSetDlg)

public:
	COutputSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutputSetDlg();

// Dialog Data
	enum { IDD = IDD_OUTPUTSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	void Fresh_Grid();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
public:
	int m_nCurRow;
	int m_nCurCol;
	int m_version;
	int m_nModeType;
	int m_outRows;
	CMsflexgrid m_FlexGrid;
	CComboBox m_OutValueCmbox;
	CComboBox m_outAMCmbox;
	CComboBox m_outRangCmbox;
	CComboBox m_outFunCmbox;
	afx_msg void OnCbnKillfocusOvaluecombo();
	afx_msg void OnCbnKillfocusOamcombo();
	afx_msg void OnCbnSelchangeOvaluecombo();
	afx_msg void OnCbnSelchangeOamcombo();
//	afx_msg void OnCbnKillfocusOrangcombo();
	afx_msg void OnCbnSelchangeOrangcombo();
	afx_msg void OnCbnKillfocusOfuncombo();
	afx_msg void OnCbnSelchangeOfuncombo();
	CEdit m_OutValueEdt;
	afx_msg void OnEnKillfocusValueedit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRefreshbutton();
	afx_msg void OnEnKillfocusOutputnameedit();
	CEdit m_outputNameEDIT;
	CComboBox m_Interlockcombo;
	afx_msg void OnCbnSelchangeCombolock();
	afx_msg void OnCbnKillfocusCombolock();
};
