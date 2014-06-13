#pragma once
#include "afxwin.h"


// CWriteSingleRegisterDlg dialog
 
class CWriteSingleRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteSingleRegisterDlg)

public:
	CWriteSingleRegisterDlg(UINT Slave_ID=255,UINT Address=0,BOOL Close_Dlg=FALSE,BOOL Single_Function=FALSE,UINT DataFormat=0,BOOL Base_0=FALSE,CString value=L"",CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteSingleRegisterDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SINGLE_WRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	UINT m_slave_id;
	UINT m_address;
	CString m_StrValue;
	CString m_static_StrValue;
	BOOL m_close_dlg;
	BOOL m_function;
	BOOL m_base_0;
	CButton m_radio_single;
	CButton m_radio_mulitple;
	//CButton m_closeCheck;
	UINT m_data_format;
	afx_msg void OnBnClickedRadioSingleReg();
	afx_msg void OnBnClickedRadioMultipleReg();
	CButton m_closeCheck;
	afx_msg void OnPaint();
	CEdit m_editor_value;
};
