#pragma once



#include "msflexgrid1.h"
#include "afxwin.h"
#include "schedule.h"

// CConfig_Routines 对话框

class CConfig_Routines : public CDialog
{
	DECLARE_DYNAMIC(CConfig_Routines)
public:
	CConfig_Routines(CWnd* pParent = NULL,CString strtype = _T(""));   // 标准构造函数
	virtual ~CConfig_Routines();

// 对话框数据
	enum { IDD = IDD_CONFIG_ROUTINES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	//add function by manual
	int m_sn;
	int row_row,row_col;//for first row is clicked,setfocus is last row and first line,killfocus is first line and first row
    void load_grid();
	void on_select();
	void tab_move();
	void shift_tab_move();
	void left_move();
	void right_move();
	void up_move();
	void down_move();
	void read_addr(description3 *temp_description,int number,unsigned char addr2);
	void fresh_one_row(unsigned char addr2);
	CString m_before_focus_string;
	void clear_addr(unsigned char addr2);
	void write_addr(unsigned char addr2);
	void put_row_col(description3 temp,unsigned char t_addr);

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
	CButton m_exit_button;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	CButton m_clear_row_button;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_strtype;
};
