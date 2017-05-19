#pragma once

#include "msflexgrid1.h"
#include "afxwin.h"
#include "schedule.h"



class Weekly_Routines : public CDialog
{
	DECLARE_DYNAMIC(Weekly_Routines)

public:
	Weekly_Routines(CWnd* pParent = NULL,CString strtype = _T(""));   // 标准构造函数
/*	Weekly_Routines(CWnd* pParent = NULL,CString strtype = _T(""));*/
	virtual ~Weekly_Routines();

// 对话框数据
	enum { IDD = IDD_WEEKLY_Routines };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	//add function by manual
	int row_row,row_col;//for first row is clicked,setfocus is last row and first line,killfocus is first line and first row
    void load_grid();
	void on_select();
	void tab_move();
	void shift_tab_move();
	void left_move();
	void right_move();
	void up_move();
	void down_move();
	description read_addr(unsigned char addr2);
	CString m_before_focus_string;
	void write_addr(unsigned char addr2);
	void clear_addr(unsigned char addr2);
	void put_row_col(description temp,unsigned char t_addr);
	void OnWeeklySet();
	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid m_FlexGrid;
	virtual BOOL OnInitDialog();	
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
	CEdit m_Change;
	CComboBox m_combo_col_row;
	CString m_sChange;
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnCbnSetfocusCombo1();
	afx_msg void OnCbnKillfocusCombo1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	CButton m_exit_button;
	afx_msg void OnBnClickedClear();
	afx_msg void OnTimer(UINT nIDEvent);
	CButton m_clear_row_button;
	void KeyDownMsflexgrid1(short* KeyCode, short Shift);
//	void MouseDownMsflexgrid1(short Button, short Shift, long x, long y);
	void DblClickMsflexgrid1();
	CString m_strInfo;
	afx_msg void OnPaint();
	CComboBox m_holidayCombx;
	afx_msg void OnCbnSelchangeHolidaycombx();
	afx_msg void OnCbnKillfocusHolidaycombx();
	CString m_strproducttype;
};
