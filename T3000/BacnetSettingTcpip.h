#pragma once


// CBacnetSettingTcpip 对话框

class CBacnetSettingTcpip : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingTcpip)

public:
	CBacnetSettingTcpip(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetSettingTcpip();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_TCPIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnBnClickedBtnBacIPAuto();
    afx_msg void OnBnClickedBtnBacIPStatic();
    afx_msg void OnBnClickedBtnBacIPChange();

    char m_tcp_type; //用于记录 IP地址是否是动态的，来判别是否有改变;
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
};
