#pragma once
#include "afxwin.h"


// CImportDatabaseDlg dialog

class CImportDatabaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportDatabaseDlg)

public:
	CImportDatabaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportDatabaseDlg();

// Dialog Data
	enum { IDD = IDD_INPORTDATADIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedBrowsBtn();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CEdit m_sourceDbEditCtrl;
	CMFCButton m_lowerBtn;
	CMFCButton m_upperBtn;
public:
	CString m_strSourceDbFileName;
	_ConnectionPtr m_pCon;//for ado connection
	_RecordsetPtr m_pRs;//for ado 
	
public:
	virtual BOOL OnInitDialog();
	void ImportIONameTable(_ConnectionPtr &  srcConTmp, _RecordsetPtr& srcRsTemp);

	BOOL m_bfromLower;
	

};
