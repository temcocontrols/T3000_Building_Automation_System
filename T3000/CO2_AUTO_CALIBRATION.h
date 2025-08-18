#pragma once
#include "afxwin.h"

// CCO2_AUTO_CALIBRATION dialog
// CCO2_AUTO_CALIBRATION �Ի���

class CCO2_AUTO_CALIBRATION : public CDialogEx
{
	DECLARE_DYNAMIC(CCO2_AUTO_CALIBRATION)

public:
	// Standard constructor
	CCO2_AUTO_CALIBRATION(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCO2_AUTO_CALIBRATION();

// Dialog data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FIRMWARE_AUTO_CAL };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboDlgCo2FirmwareAutoCal();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButtonAbcHelp();
	afx_msg void OnBnClickedButtonCo2ScdHelp();
	afx_msg void OnBnClickedButtonReCalibrationScdDone();
};
