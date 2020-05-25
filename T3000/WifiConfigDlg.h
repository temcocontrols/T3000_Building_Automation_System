#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define UI_TYPE_WIFI_REV1  0   //rev1 是给tstat8 使用的久的 wifi模块
#define UI_TYPE_WIFI_REV2  1   //rev2 是 给Tstat10 使用的  新的wifi 模块
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
    int m_ui_type;
    void SetUI_Type(int ui_type);
    unsigned short wifi_register_value[256] = { 0 };
    CString cs_ssid;
    CString cs_password;
    CString cs_macadd;
    void SetWifiRegStartAddress(int nstart_address = 2000);
    int WIFI_ENABLE ;  //2 disable     1 enable 其他 默认enable
    int IP_AUTO_MANUAL ;
    int IP_WIFI_STATUS ;
    int WIFI_LOAD_DEFAULT; //2003     1 恢复出厂设置
    int WIFI_MODBUS_PORT;
    int WIFI_BACNET_PORT;
    int WIFI_MOUDLE_SOFTWARE_VERSION;
    int SSID_NAME_START_REG ;  // 32个
    int PASSWORD_START_REG ;   //16 个
    int STATIC_IP_START_REG ;  //12个
    void ChangeWifiWindowStatus(bool b_value);
    int m_version;
private:
	BYTE m_address[4];
public:
    afx_msg void OnBnClickedRadioIpAuto();
    afx_msg void OnBnClickedRadioIpStatic();
    afx_msg void OnBnClickedButtonWifiDefault();
};


