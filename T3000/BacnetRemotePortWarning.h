#pragma once
#include "CM5\CStatic\staticex.h"
#include "afxwin.h"

// CBacnetRemotePortWarning dialog

class CBacnetRemotePortWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetRemotePortWarning)

public:
	CBacnetRemotePortWarning(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetRemotePortWarning();

// Dialog Data
	enum { IDD = IDD_DIALOG_REMOTE_PORT_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_static_remote_warning_info;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStaticEx m_static_ignore_text;
	afx_msg void OnBnClickedCheckRemotePortWarning();
};
