#pragma once


// CBacnetSettingBasicInfo Dialog
// CBacnetSettingBasicInfo �Ի���

class CBacnetSettingBasicInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingBasicInfo)

public:
	CBacnetSettingBasicInfo(CWnd* pParent = NULL);   // Standard constructor
	// ��׼���캯��
	virtual ~CBacnetSettingBasicInfo();

// Dialog Data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_BASIC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

    CEdit m_setting_obj_instance;
    CEdit m_edit_modbus_id;

    afx_msg void OnEnKillfocusEditSettingObjInstance();
    afx_msg void OnEnKillfocusEditSettingModbusId();

    afx_msg void OnBnClickedRadioSettingLcdOn();
    afx_msg void OnBnClickedRadioSettingLcdOff();

    afx_msg void OnEnKillfocusEditSettingMaxMaster();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnEnKillfocusEditSettingPanel();
    afx_msg void OnEnKillfocusEditSettingLcdDelayOff();
    afx_msg void OnEnKillfocusEditSettingNodesLabelSetting();

    CStatic m_static_panel_title;
    CEdit m_edit_panel;
    CStatic m_static_nodes_title;
    CEdit m_edit_nodes_label;

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedRadioSettingLcdDelayOff();
    afx_msg LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedButtonLcdSetting();

    CEdit m_edit_zone_name;
    //afx_msg void OnEnKillfocusEditSettingAliasName();
    afx_msg void OnEnKillfocusEditSettingBipNetwork2();
    afx_msg void OnEnKillfocusEditSettingMstpNetwork();
    afx_msg void OnBnClickedButton1();
};
