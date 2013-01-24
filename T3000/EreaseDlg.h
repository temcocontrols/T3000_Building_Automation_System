#pragma once
#include "afxwin.h"


// CEreaseDlg dialog

class CEreaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CEreaseDlg)

public:
	CEreaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEreaseDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMyOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusEdit1();
	virtual BOOL OnInitDialog();
	CEdit m_tstatIDEdit;
	int m_nTstatID;
};
