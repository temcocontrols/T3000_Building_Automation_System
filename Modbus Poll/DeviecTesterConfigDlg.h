#pragma once
#include "afxcmn.h"


// CDeviecTesterConfigDlg dialog

struct device_node
{
	int modbus_id;
	int from;
	int length;
	int count;
	int repeat;
};

class CDeviecTesterConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviecTesterConfigDlg)

public:
	CDeviecTesterConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviecTesterConfigDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEVICE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_devices_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_list_log;
	vector<device_node> m_vec_device;
};
