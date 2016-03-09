#pragma once


// CChangePSWDlg dialog

class CChangePSWDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangePSWDlg)

public:
	CChangePSWDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChangePSWDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGE_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString m_str_user;
    CString m_str_password;
    afx_msg void OnBnClickedOk();
};
