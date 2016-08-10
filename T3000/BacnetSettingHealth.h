#pragma once


// CBacnetSettingHealth dialog

class CBacnetSettingHealth : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingHealth)

public:
	CBacnetSettingHealth(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetSettingHealth();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_SETTING_HEALTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Refresh_Health_Data();
	afx_msg void OnBnClickedButtonHealthClear();
};
