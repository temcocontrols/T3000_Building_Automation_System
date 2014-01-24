#pragma once
#include "MainFrm.h"
#include "afxcmn.h"
#include "afxwin.h"

// CARDDlg dialog

class CARDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CARDDlg)

public:
	CARDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CARDDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_REMOTE_CONNECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_ipaddress;
	CEdit m_porteditor;
};
