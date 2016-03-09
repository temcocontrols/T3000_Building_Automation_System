#pragma once


// CNewImportDBDlg dialog

class CNewImportDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewImportDBDlg)

public:
	CNewImportDBDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewImportDBDlg();

// Dialog Data
	enum { IDD = IDD_DLG_NEWIMPORTDB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedCreate();
};
