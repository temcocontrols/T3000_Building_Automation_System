#pragma once

#include "resource.h"
#include "MainFrm.h"
// CCalibrationHumDlg dialog
#define  CALIBRATION_HUM 3000
class CCalibrationHumDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrationHumDlg)

public:
	CCalibrationHumDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibrationHumDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_CALIBRATION_HUM };
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_hum;
	CString m_temp;
	int m_humid;
	CMainFrame* pMain;
//	afx_msg void OnEnKillfocusHum();
//	afx_msg void OnEnKillfocusTemp();
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnEnSetfocusHum();
//	afx_msg void OnEnSetfocusTemp();
	afx_msg void OnEnKillfocusHumEd();
	afx_msg void OnEnKillfocusTempEd();
};
 
 