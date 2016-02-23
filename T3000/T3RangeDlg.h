#pragma once
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"

// CT3RangeDlg dialog
#define  WM_T3_RANGE WM_USER + 3301
class CT3RangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CT3RangeDlg)

public:
	CT3RangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CT3RangeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_T3_RANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_show_unit;
	CStatic m_rang_pic;
	CStaticEx m_static_range_units_select;
	int m_current_range;
	int m_input_Analog_select; 
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Initial_window();
	afx_msg void OnBnClickedClick_Radio();
	 
    int m_speed_count;
	afx_msg void OnEnKillfocusEditRangeSelect();
};
