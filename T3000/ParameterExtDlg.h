#pragma once
#include "afxwin.h"

#include "CM5\CStatic\staticex.h"
// CParameterExtDlg 

class CParameterExtDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParameterExtDlg)

public:
	CParameterExtDlg(CWnd* pParent = NULL);   // 
	virtual ~CParameterExtDlg();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERDIALOG_EXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    CStaticEx m_edit_delay_time;
    CStaticEx m_edit_time_remaining;
    CStaticEx m_edit_min_pwm;
    afx_msg void OnEnKillfocusEditParaExtDelay();
    afx_msg void OnEnKillfocusEditParaExtMinPwm();
    afx_msg void OnBnClickedButtonParaExtOk();
    afx_msg void OnEnKillfocusEditParaExtTimeRemaining();
    CEdit m_zigbee_panid;
    afx_msg void OnEnKillfocusEditZigbeePanId();
};
