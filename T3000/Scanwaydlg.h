#pragma once


// CScanwaydlg dialog

class CScanwaydlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScanwaydlg)

public:
	CScanwaydlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanwaydlg();

// Dialog Data
	enum { IDD = IDD_SCANWAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedQuick();
    afx_msg void OnBnClickedQuick();
    int Is_Deep;
    afx_msg void OnBnClickedOk();
};
