#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWifiConfigDlg dialog

class CWifiConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWifiConfigDlg)

public:
	CWifiConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWifiConfigDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WIFI_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	CEdit m_Edit_MacAddress;
	CIPAddressCtrl m_ipaddress;
	CIPAddressCtrl m_Subnet_Mask;
	CIPAddressCtrl m_GatewayAddress;
	CEdit m_ListenPort;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboNetworkType();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();
	//afx_msg void OnNcPaint();
	CString Exchange2String(unsigned short *Buffer,int length);
	CComboBox m_combox_NetType;
	CEdit m_Edit_SSID;
	CEdit m_Edit_Key;
	afx_msg void OnBnClickedOk();

private:
	BYTE m_address[4];
};
