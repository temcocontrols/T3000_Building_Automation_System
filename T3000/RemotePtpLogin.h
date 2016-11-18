#pragma once


// CRemotePtpLogin dialog

class CRemotePtpLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CRemotePtpLogin)

public:
	CRemotePtpLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemotePtpLogin();

// Dialog Data
	enum { IDD = IDD_DIALOG_PTP_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLoginOk();
	afx_msg void OnBnClickedButtonLoginCancel();
};
