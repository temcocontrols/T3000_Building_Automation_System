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
	afx_msg void OnNMDblclkListMonitor(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_monitor_time_picker;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMKillfocusDatetimepickerMonitor(NMHDR *pNMHDR, LRESULT *pResult);
	int m_row;
	int m_col;
	HICON hIcon;
	void Set_Input_Range_And_count();
	afx_msg void OnBnClickedBtnMonitorGraphic();
	void OnBnClickedBtnMonitorDeleteAll();
	void OnBnClickedBtnMonitorDeleteSelected();
	void OnBnClickedBtnMonitorDeleteLocal();

	afx_msg void OnBnClickedBtnMonitorRefresh();
	afx_msg void OnBnClickedButtonMonitorTest();
//	afx_msg void OnNMKillfocusListMonitor(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMKillfocusListMonitorInput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListMonitor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListMonitorInput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg LRESULT Graphic_Window_Own_Message(WPARAM wParam, LPARAM lParam);
	void Reg_Hotkey();
	void Unreg_Hotkey();
	static	DWORD WINAPI  Readmonitorthreadfun(LPVOID lpVoid);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	void Reset_Monitor_Rect();
	void OnSize(UINT nType, int cx, int cy);
	void OnSysCommand(UINT nID, LPARAM lParam);
	bool window_max; // 标记是否已经最大化;
	int read_type;

	CString flash_ctring;
	unsigned int flash_count;
	unsigned int flash_step;//闪烁的颜色控制;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	void Check_New_DB();
    BOOL GetMonitorReadPackage(int digtal_or_analog, int nIndex);
    void StringToBit(int digtal_or_analog);
   
};

int read_monitordata(int digtal_or_analog = 1,unsigned int lefttime = 0,unsigned int righttime = 0);

//const int MONITOR_NUM = 0;
//const int MONITOR_LABEL = 1;
//const int MONITOR_INTERVAL = 2;
//const int MONITOR_LOG_TIME = 3;
//const int MONITOR_UNITS = 4;
//const int MONITOR_STATUS = 5 ;
//const int MONITOR_DATA_SIZE = 6;
//const int MONITOR_COL_NUMBER = 7;

const int MONITOR_NUM = 0;
const int MONITOR_LABEL = 1;
const int MONITOR_INTERVAL = 2;
const int MONITOR_STATUS = 3 ;
const int MONITOR_DATA_SIZE = 4;
const int MONITOR_COL_NUMBER = 5;


const CString CS_DAYS = _T("Days");
const CString CS_HOURS = _T("Hours");
const CString CS_MINUTES =  _T("Minutes");

const int MONITOR_MESSAGE_CREATE = 1;
const int MONITOR_MESSAGE_DELETE = 2;

#define  TEMP_DATA_READ_COUNT  1

