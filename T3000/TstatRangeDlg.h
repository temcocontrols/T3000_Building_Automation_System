#pragma once
#include "afxwin.h"

#include "CM5\CStatic\staticex.h"
// CTstatRangeDlg dialog
#define  WM_TSTAT_RANGE WM_USER + 3300
class CTstatRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatRangeDlg)

public:
	CTstatRangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTstatRangeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TSTAT_RANGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_show_unit;
	CStaticEx m_static_range_units_select;
	CStatic m_rang_pic;
	virtual BOOL OnInitDialog();
	void Initial_window();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Click_Radio();
public:
  
	int m_input_Analog_select;
	 
	//afx_msg void OnEnKillfocusEditRangeSelect();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnSetfocusEditRangeSelect();
	afx_msg void OnBnClickedOk();
	int m_current_range;
	afx_msg void OnBnClickedRadioT0();
	afx_msg void OnBnClickedRadioT1();
	afx_msg void OnBnClickedRadioT2();
	afx_msg void OnBnClickedRadioT3();
	afx_msg void OnBnClickedRadioT4();
	afx_msg void OnBnClickedRadio();
	afx_msg void OnBnClickedRadioT6();
	afx_msg void OnBnClickedRadioT7();
	afx_msg void OnBnClickedRadioT8();
	afx_msg void OnBnClickedRadioT9();
	afx_msg void OnBnClickedRadioT10();
	afx_msg void OnBnClickedRadioT11();
};
