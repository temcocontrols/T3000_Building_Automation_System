#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
#include "afxdtctl.h"
// CBacnetScheduleTime dialog

class CBacnetScheduleTime : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetScheduleTime)

public:
	CBacnetScheduleTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetScheduleTime();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_SCHEDULE_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Initial_List();
	ListCtrlEx::CListCtrlEx m_schedule_time_list;
	afx_msg LRESULT Fresh_Schedual_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Schedual_Item(WPARAM wParam,LPARAM lParam);
	CDateTimeCtrl m_schedual_time_picker;
	afx_msg void OnNMClickListScheduleTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusDatetimepicker1Schedual(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCopyMon_Fri();
    afx_msg void OnBnClickedClearSchedual();
	//afx_msg LRESULT  SchedualTimeResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
private:
	int m_row;
	int m_col;
	int m_hour;
	int m_minute;
};




const int SCHEDULE_ON1 = 0;
const int SCHEDULE_OFF1 = 1;
const int SCHEDULE_ON2 = 2;
const int SCHEDULE_OFF2 = 3;
const int SCHEDULE_ON3 = 4;
const int SCHEDULE_OFF3 = 5;
const int SCHEDULE_ON4 = 6;
const int SCHEDULE_OFF5 = 7;