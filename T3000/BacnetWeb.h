#pragma once
#include "CWebBrowser2.h"

// CBacnetWeb 对话框

class CBacnetWeb : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetWeb)

public:
	CBacnetWeb(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetWeb();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WEB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    CWebBrowser2 m_browser;
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};
