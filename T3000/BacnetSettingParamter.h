#pragma once


// CBacnetSettingParamter 

class CBacnetSettingParamter : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingParamter)

public:
	CBacnetSettingParamter(CWnd* pParent = NULL);   // 
	virtual ~CBacnetSettingParamter();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_LCD_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    void InitialUI();
//    afx_msg void OnCbnSelchangeComboType();
//    afx_msg void OnCbnSelchangeComboNumber();
	afx_msg void OnBnClickedButtonLcdOk();
};
