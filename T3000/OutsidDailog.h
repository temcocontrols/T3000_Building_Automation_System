/*#pragma once


// COutsidDailog dialog

class COutsidDailog : public CDialog
{
	DECLARE_DYNAMIC(COutsidDailog)

public:
	COutsidDailog(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutsidDailog();

// Dialog Data
	enum { IDD = IDD_OUTSIDEDOORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

*/
#pragma once
#include "afxwin.h"


// COutsidDailog 对话框

class COutsidDailog : public CDialog
{
	DECLARE_DYNAMIC(COutsidDailog)

public:
	COutsidDailog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COutsidDailog();

// 对话框数据
	enum { IDD = IDD_OUTSIDEDOORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnBnClickedCancel();
	void Refresh();

	CEdit m_SetPointEdt;
	CEdit m_PointOffpoint;
	afx_msg void OnTimer(UINT nIDEvent);

	float m_version;
	CEdit m_UserSetPtEdit;
	afx_msg void OnBnClickedButton1();
};