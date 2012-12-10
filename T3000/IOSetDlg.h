#pragma once


// CIOSetDlg dialog

class CIOSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CIOSetDlg)

public:
	CIOSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIOSetDlg();
	BOOL CReate();

// Dialog Data
	enum { IDD = ID_VIEW_IOPANE_DLGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
