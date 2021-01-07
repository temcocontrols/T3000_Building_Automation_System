#pragma once
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"
#include "afxcmn.h"
#include "BacnetSettingBasicInfo.h"
#include "BacnetSettingTcpip.h"
#include "BacnetSettingTime.h"
#include "BacnetSettingDyndns.h"
#include "BacnetEmailAlarm.h"
#include "BacnetUserConfig.h"
#include "BacnetIOConfig.h"
#include "WifiConfigDlg.h"
// CBacnetSetting dialog

const int SETTING_BASIC = 0;
const int SETTING_TCP = 1;
const int SETTING_TIME = 2;
const int SETTING_DYNDNS = 3;
const int SETTING_EMAIL = 4;
const int SETTING_USER_LOGIN = 5;
const int SETTING_EXPANSION_IO = 6;

//const int SETTING_WIFI = 7;
const int MAX_SETTING = 7;

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


	//afx_msg	void OnBnClickedBtnBacIPCancle();
	afx_msg LRESULT Fresh_Setting_UI(WPARAM wParam,LPARAM lParam);
		virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedButtonRebootDevice();
	//afx_msg void OnBnClickedButtonResetTcpip();
	LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnBnClickedButtonEraseFlash();


	virtual void OnCancel();
	afx_msg void OnBnClickedButtonSettingLdf();

	afx_msg void OnBnClickedButtonSettingCleanDb();
	afx_msg void OnBnClickedButtonSettingAddIntoDb();
	afx_msg void OnBnClickedButtonSettingGsmModual();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckSettingPap();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedButtonBacSettingOk();

	afx_msg void OnBnClickedButtonBacShowZigbee();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void Reset_Setting_Rect();
	bool window_max; // 标记是否已经最大化;


	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonRebootDevice();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void InitScrollbar();
	afx_msg void OnBnClickedButtonZoneSchedule();
    int m_reboot_time_left; //用来记录reboot 的时间;
    char m_tcp_type; //用于记录 IP地址是否是动态的，来判别是否有改变;

    void InitialTab();
    CTabCtrl m_setting_tab;
    int m_CurSelTab;
    CBacnetSettingBasicInfo m_page_basic_info;
    CBacnetSettingTcpip m_page_tcpip;
    CBacnetSettingTime m_page_time;
    CBacnetSettingDyndns m_page_dyndns;
    CBacnetEmailAlarm m_page_email;
    CBacnetUserConfig m_page_user;
    CBacnetIOConfig   m_page_expansion_io;
    CWifiConfigDlg m_page_wifi;
    CDialog* tab_dialog[MAX_SETTING];
    afx_msg void OnTcnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult);
};



const int RESET_REGISTER = 33;
const int SPECIAL_COMMAND_REBOOT = 77;
const int SPECIAL_COMMAND_RESET_TCP = 88;
const int SPECIAL_COMMAND_ERASE_FLASH = 111;
