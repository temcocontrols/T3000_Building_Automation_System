#pragma once


// CBacnetIODefine 对话框

class CBacnetIODefine : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetIODefine)

public:
	CBacnetIODefine(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetIODefine();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REDEFINE_IO_COUNT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
