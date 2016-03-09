#pragma once


// CDataMonitorDlg dialog

class CDataMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataMonitorDlg)

public:
	CDataMonitorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataMonitorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
