#pragma once

 
  
#include "afxwin.h"
#include "afxcmn.h"
#include "msflexgrid1.h"
#include "MainFrm.h"
// CCalibrationDlg dialog

class CCalibrationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibrationDlg();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedInsert();
    CString m_hum;
    CMsflexgrid m_msflexgrid;
    UINT m_Start_Calibration;
    unsigned short DataBuffer[24];
    afx_msg void OnBnClickedRt();
    afx_msg void OnBnClickedCancel();
    CMainFrame *pMain;
};
