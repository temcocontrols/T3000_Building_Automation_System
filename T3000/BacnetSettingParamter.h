#pragma once


// CBacnetSettingParamter 对话框

class CBacnetSettingParamter : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingParamter)

public:
	CBacnetSettingParamter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetSettingParamter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_LCD_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    void InitialUI();
    afx_msg void OnCbnSelchangeComboType();
    afx_msg void OnCbnSelchangeComboNumber();
};
