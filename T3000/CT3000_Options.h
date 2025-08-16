#pragma once
#include "afxdialogex.h"


// CT3000_Options dialog
// CT3000_Options 对话框

class CT3000_Options : public CDialogEx
{
	DECLARE_DYNAMIC(CT3000_Options)

public:
	// Standard constructor
	CT3000_Options(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CT3000_Options();

// Dialog data
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTIONS };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioIpBind();
	afx_msg void OnBnClickedRadioIpauto();
};
