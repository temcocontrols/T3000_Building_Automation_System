#pragma once
#include "CWebBrowser2.h"

// CBacnetWeb 

class CBacnetWeb : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetWeb)

public:
	CBacnetWeb(CWnd* pParent = NULL);   // 
	virtual ~CBacnetWeb();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WEB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 
    CWebBrowser2 m_browser;
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
