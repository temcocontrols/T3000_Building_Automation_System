#pragma once


// CBacnetSettingDyndns 对话框

class CBacnetSettingDyndns : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingDyndns)

public:
	CBacnetSettingDyndns(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetSettingDyndns();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_DYNDNS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_dyndns_user;
    CEdit m_dyndns_password;
    CEdit m_dyndns_domain;
    CEdit m_edit_ddns_update_time;

    afx_msg void OnEnKillfocusEditDyndnsUserName();
    afx_msg void OnEnKillfocusEditDyndnsPassword();
    afx_msg void OnEnKillfocusEditDyndnsDomain();

    afx_msg void OnBnClickedCheckSettingDyndns();

    afx_msg void OnCbnSelchangeComboBacnetSettingDdnsServer();
    afx_msg void OnEnKillfocusEditTimeUpdate();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
