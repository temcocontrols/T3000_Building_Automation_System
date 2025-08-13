#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// rev1 is the old wifi module used for tstat8
#define UI_TYPE_WIFI_REV1  0   //rev1 �Ǹ�tstat8 ʹ�õľõ� wifiģ��
// rev2 is the new wifi module used for Tstat10
#define UI_TYPE_WIFI_REV2  1   //rev2 �� ��Tstat10 ʹ�õ�  �µ�wifi ģ��
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
    // 2 disable, 1 enable, others default enable
    int WIFI_ENABLE ;  //2 disable     1 enable ���� Ĭ��enable
    int IP_AUTO_MANUAL ;
    int IP_WIFI_STATUS ;
    // 2003, 1 restore factory settings
    int WIFI_LOAD_DEFAULT; //2003     1 �ָ���������
    int WIFI_MODBUS_PORT;
    int WIFI_BACNET_PORT;
    int WIFI_MOUDLE_SOFTWARE_VERSION;
    // 32 registers
    int SSID_NAME_START_REG ;  // 32��
    // 16 registers
    int PASSWORD_START_REG ;   //16 ��
    // 12 registers
    int STATIC_IP_START_REG ;  //12��
    void ChangeWifiWindowStatus(bool b_value);
    int m_version;
private:
	BYTE m_address[4];
public:
    afx_msg void OnBnClickedRadioIpAuto();
    afx_msg void OnBnClickedRadioIpStatic();
    afx_msg void OnBnClickedButtonWifiDefault();
	afx_msg void OnBnClickedRadioWifiDisable();
};


