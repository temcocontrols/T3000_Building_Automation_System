#pragma once

#include "afxwin.h"
#include "msflexgrid1.h"
#include "T3000.h"
#include "MainFrm.h"
// CIONameConfig dialog

class CIONameConfig : public CDialog
{
	DECLARE_DYNAMIC(CIONameConfig)

public:
	CIONameConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIONameConfig();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_IONAMECONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid m_IONametable;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridIotable();
	void GetAllIOName();
	int m_nCurRow;
	int m_nCurCol;
	CEdit m_nameEditor;
	CString m_oldname;
	CString m_newname;
	BOOL m_changed;
	BOOL m_Clicked;
	 void GetIOName();
	afx_msg void OnEnKillfocusNameedit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
