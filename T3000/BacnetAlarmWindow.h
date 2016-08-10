#pragma once
#include "afxwin.h"
#include "CM5/CStatic/staticex.h"

// CBacnetAlarmWindow dialog

class CBacnetAlarmWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetAlarmWindow)

public:
	CBacnetAlarmWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetAlarmWindow();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_ALARM_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT  RedrawScreeneditWindow(WPARAM wParam, LPARAM lParam);
	CStaticEx m_static_bac_alarm;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
