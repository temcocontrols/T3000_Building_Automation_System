#pragma once


// CBacnetIODefine 

class CBacnetIODefine : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetIODefine)

public:
	CBacnetIODefine(CWnd* pParent = nullptr);   // 
	virtual ~CBacnetIODefine();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REDEFINE_IO_COUNT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
