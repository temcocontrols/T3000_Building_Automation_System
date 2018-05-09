#pragma once

#include "CM5/SliderBoth.h"


// CWeeklySchedule dialog

class CWeeklySchedule : public CDialog
{
	DECLARE_DYNAMIC(CWeeklySchedule)

public:
	CWeeklySchedule(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWeeklySchedule();

// Dialog Data
	enum { IDD = IDD_WEEKLY_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_apply,m_clear,m_mf;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonMf();
	SliderBoth slider_Mon,slider_Tue,slider_Wed,slider_Thu,slider_Fri,slider_Sat,slider_Sun;
	CRect rc;

	afx_msg void OnStnClickedStatic11();
};
