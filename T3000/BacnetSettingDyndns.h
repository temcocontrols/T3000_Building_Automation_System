#pragma once


// CBacnetSettingDyndns Dialog
// CBacnetSettingDyndns �Ի���

class CBacnetSettingDyndns : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingDyndns)

public:
	CBacnetSettingDyndns(CWnd* pParent = NULL);   // Standard constructor
	// ��׼���캯��
	virtual ~CBacnetSettingDyndns();

// Dialog Data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_DYNDNS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// DDX/DDV ֧��

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
