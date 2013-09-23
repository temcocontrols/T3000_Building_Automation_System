#pragma once
#include "afxwin.h"


// CMyAboutDlg dialog

class CMyAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyAboutDlg)

public:
	CMyAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyAboutDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
