#pragma once
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"

// CBacnetSetting dialog

class CBacnetSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSetting)

public:
	CBacnetSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetSetting();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonBacTest();
	CDateTimeCtrl m_cm5_time_picker;
	CDateTimeCtrl m_cm5_date_picker;
	//afx_msg void OnBnClickedBacEnableEditTime();
	afx_msg void OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult);
	void Get_Time_Edit_By_Control();
	afx_msg void OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnBacWriteTime();
	afx_msg void OnBnClickedBtnBacSYNCTime();
	afx_msg void OnBnClickedBtnBacIPAuto();
	afx_msg void OnBnClickedBtnBacIPStatic();
	afx_msg void OnBnClickedBtnBacIPChange();
	//afx_msg	void OnBnClickedBtnBacIPCancle();
	afx_msg LRESULT Fresh_Setting_UI(WPARAM wParam,LPARAM lParam);
		virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedButtonRebootDevice();
	//afx_msg void OnBnClickedButtonResetTcpip();
	LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnBnClickedButtonEraseFlash();
	afx_msg void OnCbnSelchangeComboBacnetSettingCom0();
	afx_msg void OnCbnSelchangeComboBacnetSettingCom1();
	afx_msg void OnCbnSelchangeComboBacnetSettingCom2();
	CComboBox m_device_com0;
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonSettingLdf();
	afx_msg void OnCbnSelchangeComboBacnetSettingBaudrate0();
	afx_msg void OnCbnSelchangeComboBacnetSettingBaudrate1();
	afx_msg void OnCbnSelchangeComboBacnetSettingBaudrate2();
	afx_msg void OnBnClickedButtonSettingCleanDb();
	afx_msg void OnBnClickedButtonSettingAddIntoDb();
	afx_msg void OnBnClickedButtonSettingUserList();
	afx_msg void OnBnClickedRadioUsbDevice();
	afx_msg void OnBnClickedRadioUsbHost();
	afx_msg void OnBnClickedButtonSettingGsmModual();
	afx_msg void OnClose();

	afx_msg void OnEnKillfocusEditSettingPanel();

	afx_msg void OnEnKillfocusEditSettingNodesLabelSetting();

	_ConnectionPtr				m_pCon;//for ado connection
	_RecordsetPtr				m_pRs;//for ado 

	CStatic m_static_panel_title;
	CEdit m_edit_panel;
	CStatic m_static_nodes_title;
	CEdit m_edit_nodes_label;
	afx_msg void OnCbnSelchangeComboBacnetSettingTimeServer();
	afx_msg void OnBnClickedCheckSettingSyncTime();
	afx_msg void OnEnKillfocusEditDyndnsUserName();
	afx_msg void OnEnKillfocusEditDyndnsPassword();
	afx_msg void OnEnKillfocusEditDyndnsDomain();
	CEdit m_dyndns_user;
	CEdit m_dyndns_password;
	CEdit m_dyndns_domain;
	afx_msg void OnBnClickedCheckSettingDyndns();
	CEdit m_edit_ddns_update_time;
	afx_msg void OnCbnSelchangeComboBacnetSettingDdnsServer();
	afx_msg void OnEnKillfocusEditTimeUpdate();
	afx_msg void OnCbnSelchangeComboBacnetSettingTimeZone();
	afx_msg void OnBnClickedCheckSettingPap();
	CEdit m_edit_port;
	afx_msg void OnEnKillfocusEditSettingPort();
	afx_msg void OnBnClickedButtonHealth();

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F);
};


const int RESET_REGISTER = 33;
const int SPECIAL_COMMAND_REBOOT = 77;
const int SPECIAL_COMMAND_RESET_TCP = 88;
const int SPECIAL_COMMAND_ERASE_FLASH = 111;
