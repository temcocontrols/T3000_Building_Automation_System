#pragma once

#include "afxwin.h"
#include "msflexgrid1.h"

// C5EInputDialog dialog

class C5EInputDialog : public CDialog
{
	DECLARE_DYNAMIC(C5EInputDialog)

public:
	C5EInputDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~C5EInputDialog();

// Dialog Data
	enum { IDD = IDD_5EINPUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMsflexgrid m_flexGrd;
	afx_msg void OnBnClickedOk();
	CEdit m_valueEdt;
	CComboBox m_rangeCombx;
	void ReloadDataValue();
	DECLARE_EVENTSINK_MAP()
	void Click5emsflexgrid();
	afx_msg void OnCbnSetfocus5erangecombo();
	afx_msg void OnCbnKillfocus5erangecombo();
	afx_msg void OnEnSetfocus5evalueedit();
	afx_msg void OnEnKillfocus5evalueedit();
public:
	float m_fFirmwareVersion;
	int m_nCurRow;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedExit();
	afx_msg void OnCbnSelchange5erangecombo();
	afx_msg void OnCbnSelendok5erangecombo();
};
