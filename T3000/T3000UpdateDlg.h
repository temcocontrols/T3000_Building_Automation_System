#pragma once


// CT3000UpdateDlg dialog

class CT3000UpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CT3000UpdateDlg)

public:
	CT3000UpdateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CT3000UpdateDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG15 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CString versioninfor;
    bool m_new;
    afx_msg void OnBnClickedDownload();
};
