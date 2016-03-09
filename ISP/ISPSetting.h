#pragma once


// CISPSetting dialog

class CISPSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CISPSetting)

public:
	CISPSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CISPSetting();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingOk();
	afx_msg void OnBnClickedButtonSettingCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
