#pragma once
#include "afxwin.h"


// CEnterValueDlg dialog

class CEnterValueDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterValueDlg)

public:
	CEnterValueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnterValueDlg();
	 
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_ENTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_StrValue;
	afx_msg void OnBnClickedOk();
	CEdit m_Value_Edit;
	UINT m_dataformate;
	CString m_static_StrValue;
};
