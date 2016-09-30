#pragma once


// CDeviceTestDlg dialog

class CDeviceTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceTestDlg)

public:
	CDeviceTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceTestDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEVICE_TESTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
