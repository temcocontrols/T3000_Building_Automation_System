#pragma once


// CTemcoStandardBacnetToolDlg dialog

class CTemcoStandardBacnetToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTemcoStandardBacnetToolDlg)

public:
	CTemcoStandardBacnetToolDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemcoStandardBacnetToolDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TEMCO_STANDARD_BACNET_TOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
