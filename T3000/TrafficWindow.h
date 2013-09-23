#pragma once
#include "afxwin.h"


// TrafficWindow dialog

class TrafficWindow : public CDialogEx
{
	DECLARE_DYNAMIC(TrafficWindow)

public:
	TrafficWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~TrafficWindow();

// Dialog Data
	enum { IDD = IDD_TRAFFIC_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic staticTrafficText;
	virtual void PostNcDestroy();
	CWnd* h_mbPollWindow;
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	virtual BOOL DestroyWindow();
	virtual void OnCancel();
	void OnTrafficWindowDestroy();
	afx_msg void OnNcDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox listTrafficWindow;
	CButton btnClearTraffic;
	afx_msg void OnClickedBtnClearTraffic();
	afx_msg void OnClickedCheckAutoscroll();
	CButton listAutoScroll;
};
