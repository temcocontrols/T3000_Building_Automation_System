#pragma once
#include "afxdialogex.h"


// BacnetSettingAdvParameter 对话框

class BacnetSettingAdvParameter : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetSettingAdvParameter)

public:
	BacnetSettingAdvParameter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~BacnetSettingAdvParameter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADVANCED_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdvancedApply();
	afx_msg void OnBnClickedButtonAdvancedOk();
	afx_msg void OnBnClickedButtonAdvancedQ1();
	afx_msg void OnBnClickedButtonAdvancedQ2();
	afx_msg void OnBnClickedButtonAdvancedCancel();
};
