#pragma once
#include "afxdtctl.h"


// CSetTimeDlg dialog

class CSetTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetTimeDlg)

public:
	CSetTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetTimeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_DIALOG_SETTIME
};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strTime;
	CDateTimeCtrl m_TimePicker;
	virtual BOOL OnInitDialog();
};
