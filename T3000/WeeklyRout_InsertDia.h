#pragma once


#include "msflexgrid1.h"
#include "afxwin.h"
#include "Schedule_grid.h"

class WeeklyRout_InsertDia : public CDialog
{
	DECLARE_DYNAMIC(WeeklyRout_InsertDia)
public:
	WeeklyRout_InsertDia(unsigned char row,CString strtyp = _T(""),CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WeeklyRout_InsertDia();
// 对话框数据
	enum { IDD = IDD_WEEKLY_ROUTINES_INSERT_DIA };
public:
	unsigned char m_addr;////////////////////the row number
	int row_row,row_col;//for first row is clicked,setfocus is last row and first line,killfocus is first line and first row
    void load_grid();
	void write_grid();
	void tab_move();
	void shift_tab_move();
	void left_move();
	void right_move();
	void up_move();
	void down_move();
	void get_two_value_from_one_grid(CString str2,unsigned char &a,unsigned char &b);
	void on_select();
	void col_copy_to_col(int base_col,int order_col);
	void disable_one_day(int day_number);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid m_FlexGrid;
	CComboBox m_combo_col_row;
	CEdit m_Change;
	CString m_sChange;
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnEnSetfocusEdit1();
	bool inspect_input_string(int Row,int Col,CString &str);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnCbnSetfocusCombo1();
	afx_msg void OnCbnKillfocusCombo1();
	void ClickMsflexgrid1();
	afx_msg void OnDestroy();
protected:
	virtual void OnCancel();
public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_exit_button;
	afx_msg void OnBnClickedCopyTo();
	BOOL m_mon;
	BOOL m_tue;
	BOOL m_wed;
	BOOL m_thu;
	BOOL m_fri;
	BOOL m_sat;
	BOOL m_sun;
	BOOL m_hol1;
	BOOL m_hol2;
	CButton m_mon_ctrl;
	CButton m_tue_ctrl;
	CButton m_wed_ctrl;
	CButton m_thu_ctrl;
	CButton m_fri_ctrl;
	CButton m_sat_ctrl;
	CButton m_sun_ctrl;
	CButton m_hol1_ctrl;
	CButton m_hol2_ctrl;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	CString m_strtype;
};
