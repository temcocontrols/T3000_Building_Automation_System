#pragma once


// CBacnetSettingTcpip Dialog
// CBacnetSettingTcpip dialog

class CBacnetSettingTcpip : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingTcpip)

public:
	CBacnetSettingTcpip(CWnd* pParent = NULL);   // Standard constructor
	// Standard constructor.
	virtual ~CBacnetSettingTcpip();

// Dialog Data
// Dialog data.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_TCPIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnBnClickedBtnBacIPAuto();
    afx_msg void OnBnClickedBtnBacIPStatic();
    afx_msg void OnBnClickedBtnBacIPChange();

    char m_tcp_type; // Used to record whether IP address is dynamic to determine if there are changes
    // Records whether the IP address is dynamic, so a change can be detected.
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedButtonWifiConfiguration();

    afx_msg void OnCbnSelchangeComboBacnetSettingSubCom();
    afx_msg void OnCbnSelchangeComboBacnetSettingCom1();
    afx_msg void OnCbnSelchangeComboBacnetSettingMainCom();
    afx_msg void OnCbnSelchangeComboBacnetSettingBaudrate0();
    afx_msg void OnCbnSelchangeComboBacnetSettingBaudrate1();
    afx_msg void OnCbnSelchangeComboBacnetSettingBaudrate2();
    afx_msg void OnBnClickedRadioUsbDevice();
    afx_msg void OnBnClickedRadioUsbHost();
    afx_msg void OnBnClickedButtonHealth();
    afx_msg void OnEnKillfocusEditSettingPort();
    afx_msg void OnBnClickedCheckSettingFixComm();
    afx_msg void OnEnKillfocusEditSettingZeigbeePanid();
    CEdit m_edit_port;
    CComboBox m_device_com0;
    afx_msg void OnCbnSelchangeComboParityBit();
    afx_msg void OnCbnSelchangeComboParityBit3();
    afx_msg void OnCbnSelchangeComboStopBit();
    afx_msg void OnCbnSelchangeComboStopBit3();
    BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
    virtual BOOL OnInitDialog();
    void ClearSubNetInfo();
    afx_msg LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedButtonWireguardConfig();
    afx_msg void OnBnClickedButtonGsmStatus();
    afx_msg void OnBnClickedButtonMqttConfig();
    afx_msg void OnBnClickedButtonSnmpConfig();
};
