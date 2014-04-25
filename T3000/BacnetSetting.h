#pragma once


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
	afx_msg void OnBnClickedBacEnableEditTime();
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
	afx_msg	void OnBnClickedBtnBacIPCancle();
	afx_msg LRESULT Fresh_Setting_UI(WPARAM wParam,LPARAM lParam);
		virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRebootDevice();
	afx_msg void OnBnClickedButtonResetTcpip();
	LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonEraseFlash();
};


const int RESET_REGISTER = 33;
const int SPECIAL_COMMAND_REBOOT = 77;
const int SPECIAL_COMMAND_RESET_TCP = 88;
const int SPECIAL_COMMAND_ERASE_FLASH = 111;
