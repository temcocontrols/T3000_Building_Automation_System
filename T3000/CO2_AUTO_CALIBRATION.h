#pragma once
#include "afxwin.h"

// CCO2_AUTO_CALIBRATION 对话框

class CCO2_AUTO_CALIBRATION : public CDialogEx
{
	DECLARE_DYNAMIC(CCO2_AUTO_CALIBRATION)

public:
	CCO2_AUTO_CALIBRATION(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCO2_AUTO_CALIBRATION();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FIRMWARE_AUTO_CAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboDlgCo2FirmwareAutoCal();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButtonAbcHelp();
};
