#pragma once


// CFreezeFunctionDlg dialog

class CFreezeFunctionDlg : public CDialog
{
	DECLARE_DYNAMIC(CFreezeFunctionDlg)

public:
	CFreezeFunctionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFreezeFunctionDlg();
	 void Fresh_Data();
	 virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_FREEZEFUNDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusTempture();
	int m_temp;
	int m_open;
	int m_close;
	afx_msg void OnEnKillfocusOpen();
	afx_msg void OnEnKillfocusClose();
};
