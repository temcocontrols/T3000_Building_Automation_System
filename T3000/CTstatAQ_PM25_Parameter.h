#pragma once
#include "afxdialogex.h"


// CTstatAQ_PM25_Parameter 

class CTstatAQ_PM25_Parameter : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatAQ_PM25_Parameter)

public:
	CTstatAQ_PM25_Parameter(CWnd* pParent = nullptr);   // 
	virtual ~CTstatAQ_PM25_Parameter();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TSTAT_AQ_PM25_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusEditPPm25Value();
	afx_msg void OnEnKillfocusEditPPm10Value2();
	afx_msg void OnEnKillfocusEditPPm25Min();
	afx_msg void OnEnKillfocusEditPPm25Max();
	afx_msg void OnEnKillfocusEditPPm10Min();
	afx_msg void OnEnKillfocusEditPPm10Max();
	afx_msg void OnEnKillfocusEditPPm25Filter();
	afx_msg void OnEnKillfocusEditPPm10Filter();
	afx_msg LRESULT  PM25MessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRadioPmAuto();
	afx_msg void OnBnClickedRadioPmManual();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
