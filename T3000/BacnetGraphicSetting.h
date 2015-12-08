#pragma once
#include "afxdtctl.h"


// CBacnetGraphicSetting dialog

class CBacnetGraphicSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetGraphicSetting)

public:
	CBacnetGraphicSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetGraphicSetting();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_GRAPHIC_TIME_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CDateTimeCtrl m_grp_start_day;
	CDateTimeCtrl m_grp_start_time;
	CDateTimeCtrl m_grp_end_day;
	CDateTimeCtrl m_grp_end_time;
	afx_msg void OnBnClickedCancel();
};
