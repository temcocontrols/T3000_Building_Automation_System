#pragma once


// CMailFeedbackDlg dialog

class CMailFeedbackDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMailFeedbackDlg)

public:
	CMailFeedbackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMailFeedbackDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FEEDBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBug();
	afx_msg void OnBnClickedButtonSuggest();
};
