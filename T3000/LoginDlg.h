#pragma once
#include "afxwin.h"
#include "../SQLiteDriver/CppSQLite3.h"

// CLoginDlg dialog

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(BOOL &ok_or_no,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
 
	CEdit m_userEdit;
	CEdit m_psswordEdit;
	afx_msg void OnBnClickedOk();
	BOOL m_bOk;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
};
