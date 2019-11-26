#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// CBacnetAlarmLog dialog

class CBacnetAlarmLog : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetAlarmLog)

public:
	CBacnetAlarmLog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetAlarmLog();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_ALARMLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT Fresh_Alarmlog_List(WPARAM wParam,LPARAM lParam);
	void Initial_List();
	ListCtrlEx::CListCtrlEx m_alarmlog_list;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnHdnItemclickListAlarmlog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListAlarmlog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT  AlarmLogMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	bool window_max; // 标记是否已经最大化;
	void Reset_Alarm_Rect();
    afx_msg void OnBnClickedButtonEmailAlarm();
};

const int ALARMLOG_NUM = 0;
const int ALARMLOG_PANEL = 1;
const int ALARMLOG_MESSAGE = 2;
const int ALARMLOG_TIME = 3;
const int ALARMLOG_ACK = 4;
const int ALARMLOG_RES = 5;
const int ALARMLOG_DEL = 6;

const int ALARMLOG_COL_NUMBER = 10;
