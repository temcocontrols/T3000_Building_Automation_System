#pragma once


// COfficeStPtDlg dialog

class COfficeStPtDlg : public CDialog
{
	DECLARE_DYNAMIC(COfficeStPtDlg)

public:
	COfficeStPtDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COfficeStPtDlg();

// Dialog Data
	enum { IDD = IDD_OIFFICESTPTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
