#pragma once
#include "afxcmn.h"

#define IDD_PARTS_VENDORS 1777
// CPVDlg dialog

class CPVDlg : public CDialog
{
	DECLARE_DYNAMIC(CPVDlg)

public:
	CPVDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPVDlg();
	BOOL OnInitDialog();
	void InitTab();
// Dialog Data
	enum { IDD = IDD_PARTS_VENDORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_Tab;
};
