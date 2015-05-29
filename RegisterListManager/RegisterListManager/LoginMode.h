#pragma once


// CLoginMode dialog

class CLoginMode : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginMode)

public:
	CLoginMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginMode();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CString m_user_name;
    CString m_password;
public:
    afx_msg void OnBnClickedOk();
};
