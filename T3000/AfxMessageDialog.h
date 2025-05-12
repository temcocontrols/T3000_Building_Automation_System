#pragma once


// CAfxMessageDialog dialog

class CAfxMessageDialog : public CDialog
{
	DECLARE_DYNAMIC(CAfxMessageDialog)

private:
	CString m_strPromtionText;
	DWORD m_dwTimeStart;
public:
	CAfxMessageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAfxMessageDialog();

// 对话框数据
	enum { IDD = IDD_AFXDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nTime;//;
public:
	
	void SetPromtionTxt(CString strText);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
	void SetTotalTime(int nTime);
	BOOL m_bShowOkBtn;
};
