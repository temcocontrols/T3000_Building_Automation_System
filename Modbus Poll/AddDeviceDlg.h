#pragma once


// CAddDeviceDlg dialog

class CAddDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDeviceDlg)

public:
	CAddDeviceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDeviceDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEVICE_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_from;
	int m_modbus_id;
	int m_repeat;
//	CEdit m_to;
	int m_length;
	afx_msg void OnBnClickedOk();
	int m_count;
};
