#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include "afxdtctl.h"

// CBacnetMonitor dialog

class CBacnetMonitor : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetMonitor)

public:
	CBacnetMonitor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetMonitor();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_monitor_list;
	ListCtrlEx::CListCtrlEx m_monitor_input_list;
	void Initial_List();
	afx_msg LRESULT MonitorMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Monitor_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Monitor_Input_List(WPARAM wParam,LPARAM lParam);	
	afx_msg LRESULT Fresh_MCallBack_Item(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Monitor_Input_Item(WPARAM wParam,LPARAM lParam);
	afx_msg	LRESULT Fresh_Monitor_Item(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickListMonitor(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_monitor_time_picker;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMKillfocusDatetimepickerMonitor(NMHDR *pNMHDR, LRESULT *pResult);
	int m_row;
	int m_col;
	HICON hIcon;
	void Set_Input_Range_And_count();
	afx_msg void OnBnClickedBtnMonitorGraphic();
	afx_msg void OnBnClickedBtnMonitorRefresh();
	afx_msg void OnBnClickedButtonMonitorTest();
//	afx_msg void OnNMKillfocusListMonitor(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMKillfocusListMonitorInput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListMonitor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListMonitorInput(NMHDR *pNMHDR, LRESULT *pResult);
};

const int MONITOR_NUM = 0;
const int MONITOR_LABEL = 1;
const int MONITOR_INTERVAL = 2;
const int MONITOR_LENGTH = 3;
const int MONITOR_UNITS = 4 ;
const int MONITOR_STATUS = 5 ;

const int MONITOR_COL_NUMBER = 14;

const CString CS_DAYS = _T("Days");
const CString CS_HOURS = _T("Hours");
const CString CS_MINUTES =  _T("Minutes");

