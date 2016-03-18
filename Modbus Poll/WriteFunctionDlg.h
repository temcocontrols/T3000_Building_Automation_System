#pragma once
#include "afxwin.h"


// CWriteFunctionDlg dialog

class CWriteFunctionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteFunctionDlg)

public:
	CWriteFunctionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteFunctionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_startaddress;
	UINT m_endaddress;
	int m_value;
	UINT m_device_id;
	afx_msg void OnBnClickedOk();
	CListBox m_log_list;
};
