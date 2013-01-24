#pragma once


/*
// CFreeCoolDlg dialog

class CFreeCoolDlg : public CDialog
{
	DECLARE_DYNAMIC(CFreeCoolDlg)

public:
	CFreeCoolDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFreeCoolDlg();

// Dialog Data
	enum { IDD = IDD_FREECOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
*/

// CFreeCoolDlg 对话框

class CFreeCoolDlg : public CDialog
{
	DECLARE_DYNAMIC(CFreeCoolDlg)

public:
	CFreeCoolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFreeCoolDlg();

// 对话框数据
	enum { IDD = IDD_FREECOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnEnKillfocusTimeedit();
	afx_msg void OnEnKillfocusAiredit();
	afx_msg void OnEnKillfocusTrgedit();
	afx_msg void OnEnKillfocusTmpedit();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void Refresh();

	CString m_strStatus;
};

