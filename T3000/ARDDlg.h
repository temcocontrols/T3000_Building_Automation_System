#pragma once
#include "MainFrm.h"
#include "afxcmn.h"
#include "afxwin.h"

// CARDDlg dialog
 struct Custom_Product_Node{
    int Product_Type;
    CString Product_Name;
 };
class CARDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CARDDlg)

public:
	CARDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CARDDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_REMOTE_CONNECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_ipaddress;
	CEdit m_porteditor;
	vector<CString> m_szComm;
	CComboBox m_add_device_com_port;
	CComboBox m_add_device_baudrate;
	CEdit m_add_device_modbus_id;
	void Enable_Net_UI(bool enable_net);
	afx_msg void OnBnClickedRadioNetDevice();
	afx_msg void OnBnClickedRadio2();
	bool m_is_net_device;
	afx_msg void OnBnClickedButtonLocalMstpDevice();

 
    int m_Serial_Number;
    CString m_type_id;
    CComboBox m_combox_productname;
    vector<Custom_Product_Node> m_Custom_Product;
    afx_msg void OnCbnDropdownComboProductName();
    afx_msg void OnCbnSelchangeComboProductName();
      void OnEnKillfocusEditTypeId();
//    afx_msg void OnEnKillfocusEditAddDeviceModbusId();
    CComboBox m_minipanel_comport;
	int GetNewProductID();
	int GetNewSerialNumber();
	//afx_msg void OnCbnSelchangeComboAddDeviceComport();
//	afx_msg void OnBnClickedCheckOnlineOffline();
//	CButton m_offlineButton;
};
