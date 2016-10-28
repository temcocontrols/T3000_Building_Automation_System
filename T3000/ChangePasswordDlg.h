#pragma once
#include "afxwin.h"


// CChangePasswordDlg dialog

class CChangePasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePasswordDlg)

public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChangePasswordDlg();

// Dialog Data
	enum { IDD = IDD_CHANGE_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CEdit m_userEdt;
	CEdit m_userOldPwEdt;
	CEdit m_newPwEdt;
	CEdit m_verifyPwEdt;

 
};
