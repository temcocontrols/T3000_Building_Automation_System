#pragma once
#include "afxwin.h"

//

#if 1
// CScanSelectDlg dialog

class CScanSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CScanSelectDlg)

public:
	CScanSelectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanSelectDlg();

// Dialog Data
	enum { IDD = IDD_SCANSELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();	
	afx_msg void OnEnChangeEdit2();
	void ShowSafeScanControl(BOOL bShow);
	int m_FromID;
	int m_nDestID;
	afx_msg void OnEnKillfocusMinedit();
	afx_msg void OnEnKillfocusMaxedit();
	CComboBox m_strScanSubNetNameCobox;
public:
	CString m_strScanSubNetName;
};
#endif