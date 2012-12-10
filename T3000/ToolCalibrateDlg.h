#include "msflexgrid1.h"
#include "afxwin.h"
#pragma once



class CToolCalibrateDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolCalibrateDlg)

public:
	CToolCalibrateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolCalibrateDlg();

// Dialog Data
	enum { IDD = IDD_CALIBRATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid m_FlexGrid;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnCbnSelchangeCombo1();
	DECLARE_EVENTSINK_MAP()
	void ClickGridflashMsflexgrid();
public:
	int row_row,row_col;
	CComboBox m_subNetList;
};
