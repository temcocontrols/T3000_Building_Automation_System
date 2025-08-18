#pragma once
//#include "BacnetSetting.h"

// CBacnetSettingTime Dialog
// CBacnetSettingTime 对话框

class CBacnetSettingTime : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingTime)

public:
	// Standard constructor
	CBacnetSettingTime(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetSettingTime();

// Dialog data
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_TIME };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButtonRefreshTime();
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
    afx_msg void OnCbnSelchangeComboBacnetSettingTimeZone();
    afx_msg void OnBnClickedCheckSettingZoneDaylightTime();
    afx_msg void OnCbnSelchangeComboBacnetSettingTimeServer();
    afx_msg void OnBnClickedCheckSettingSyncTime();
    afx_msg void OnBnClickedRadioSettingSyncPc();
    afx_msg void OnCbnKillfocusComboBacnetSettingTimeServer();
    afx_msg void OnBnClickedButtonSyncTime();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();
    void UpdateDayNightStartEndUI();
    int MonthType(unsigned char n_month);
    afx_msg void OnCbnSelchangeComboStartMonth();
    afx_msg void OnCbnSelchangeComboStartDay();
    afx_msg void OnCbnSelchangeComboEndMonth();
    afx_msg void OnCbnSelchangeComboEndDay();
};
