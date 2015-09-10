#pragma once
#include "afxwin.h"


// CEnterValueDlg dialog

class CEnterValueDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterValueDlg)

public:
	CEnterValueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnterValueDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ENTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString m_string_value;
    virtual BOOL OnInitDialog();
    CEdit m_edit_value;
};
