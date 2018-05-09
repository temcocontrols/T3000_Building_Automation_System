#pragma once
#include "resource.h"

// CRenameDlg dialog

class CRenameDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameDlg)

public:
	CRenameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRenameDlg();

	// Dialog Data
	enum { IDD = IDD_RENAME_DLG };
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_nodename;
	afx_msg void OnBnClickedOk();
};