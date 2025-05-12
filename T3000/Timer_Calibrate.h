#pragma once
#include "afxdtctl.h"
#include "atlcomtime.h"
#include "afxwin.h"


// Timer_Calibrate 对话框

class Timer_Calibrate : public CDialog
{
	DECLARE_DYNAMIC(Timer_Calibrate)

public:
	Timer_Calibrate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Timer_Calibrate();

// 对话框数据
	enum { IDD = IDD_DIA_Timer_Calibrate };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	unsigned short machine_time[8];
	bool had_select;
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_machine_time;
	afx_msg void OnBnClickedOk();
	COleDateTime m_date_time;
	COleDateTime m_time_time;
	afx_msg void OnNMSetfocusDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_time_zone;
	CString m_time_zone_str;
	CComboBox m_building_time_zone;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	COleDateTime m_NCDateCtrl;
	COleDateTime m_NCTimeCtrl;
	CEdit m_DatetimeEdit;
};
