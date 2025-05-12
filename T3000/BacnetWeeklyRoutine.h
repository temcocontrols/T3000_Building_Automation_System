#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
// BacnetWeeklyRoutine dialog

class BacnetWeeklyRoutine : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetWeeklyRoutine)

public:
	BacnetWeeklyRoutine(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetWeeklyRoutine();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_WEEKLY_ROUTINES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_weeklyr_list;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void Initial_List();
	afx_msg LRESULT  WeeklyMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Weekly_List(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickListBacWeekly(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT Fresh_Weekly_Routine_Item(WPARAM wParam,LPARAM lParam);
	//afx_msg LRESULT  WeeklyResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonWeeklyScheduleEdit();
    afx_msg void OnBnClickedButtonWeeklysubPanelScheduleEdit();
	afx_msg void OnNMDblclkListWeeklySchedule(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
	void Reg_Hotkey();
	void Unreg_Hotkey();
	void Reset_Weekly_Rect();
	void OnSize(UINT nType, int cx, int cy);
	void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	bool window_max; // 标记是否已经最大化;
};


