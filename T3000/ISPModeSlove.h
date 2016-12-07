#pragma once

#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
// CISPModeSlove dialog

class CISPModeSlove : public CDialogEx
{
	DECLARE_DYNAMIC(CISPModeSlove)

public:
	CISPModeSlove(CWnd* pParent = NULL);   // standard constructor
	virtual ~CISPModeSlove();

// Dialog Data
	enum { IDD = IDD_DIALOG_ISP_MODE_SOLVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Initial_static();
	CStatic m_device_ip_title;
	CStatic m_product_name_title;
	CStatic m_device_ip_edit;
	CStatic m_device_name_edit;
	CStatic m_isp_mode_title;

	afx_msg void OnBnClickedButtonIspChooseFirmware();
	afx_msg void OnBnClickedButtonUpdateFirmware();
	afx_msg void OnBnClickedButtonIspCancel();
	afx_msg void OnBnClickedRadioFromServer();
	afx_msg void OnBnClickedRadioFromHardisk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
