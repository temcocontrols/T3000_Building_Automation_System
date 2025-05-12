#pragma once

#include "stdafx.h"
#include "afxwin.h"
// CLedsDialog 对话框

class CLedsDialog : public CDialog
{
	DECLARE_DYNAMIC(CLedsDialog)

public:
	CLedsDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLedsDialog();

// 对话框数据
	enum { IDD = IDD_LEDS_DIALOG };
	int		m_address;
	CString	m_led1_value;
	CString	m_led2_value;
	CString	m_led3_value;
	CString	m_led4_value;
	CString	m_led5_value;
	CString	m_led6_value;
	CString	m_led7_value;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
protected:	

public:		
	afx_msg void OnRefresh();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetfocusLed1Combo();
	afx_msg void OnSetfocusLed2Combo();
	afx_msg void OnSetfocusLed3Combo();
	afx_msg void OnSetfocusLed4Combo();
	afx_msg void OnSetfocusLed5Combo();
	afx_msg void OnSetfocusLed6Combo();
	afx_msg void OnSetfocusLed7Combo();
	afx_msg void OnSelendokLed1Combo();
	afx_msg void OnSelendokLed2Combo();
	afx_msg void OnSelendokLed3Combo();
	afx_msg void OnSelendokLed4Combo();
	afx_msg void OnSelendokLed5Combo();
	afx_msg void OnSelendokLed6Combo();
	afx_msg void OnSelendokLed7Combo();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	CComboBox m_led1Box;
	CComboBox m_led2Box;
	CComboBox m_led3Box;
	CComboBox m_led4Box;
	CComboBox m_led5Box;
	CComboBox m_led6Box;
	CComboBox m_led7Box;
};
