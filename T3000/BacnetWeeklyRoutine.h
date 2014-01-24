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
	virtual void OnCancel();
	void Reg_Hotkey();
	void Unreg_Hotkey();
};

const int WEEKLY_ROUTINE_NUM = 0;
const int WEEKLY_ROUTINE_FULL_LABLE = 1;
const int WEEKLY_ROUTINE_AUTO_MANUAL = 2;
const int WEEKLY_ROUTINE_OUTPUT = 3;
const int WEEKLY_ROUTINE_HOLIDAY1 = 4;
const int WEEKLY_ROUTINE_STATE1 = 5;
const int WEEKLY_ROUTINE_HOLIDAY2 = 6;
const int WEEKLY_ROUTINE_STATE2 = 7;
const int WEEKLY_ROUTINE_LABEL = 8;

const int WEEKLY_COL_NUMBER = 9;
