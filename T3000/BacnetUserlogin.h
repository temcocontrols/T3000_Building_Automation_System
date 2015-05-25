#pragma once
#include "afxwin.h"

#include "CM5/CStatic/staticex.h"
// CBacnetUserlogin dialog

class CBacnetUserlogin : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetUserlogin)

public:
	CBacnetUserlogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetUserlogin();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_USER_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT  RedrawLoginWindow(WPARAM wParam, LPARAM lParam);

public:
	int m_cxScreen;
	int m_cyScreen;
	afx_msg void OnBnClickedButtonLogin();
	virtual void OnCancel();
	int CheckLoginData();

	CString m_user_name;
	CString m_user_password;
	afx_msg void OnPaint();
	CStaticEx m_name_static;
	CStaticEx m_password_static;
};



