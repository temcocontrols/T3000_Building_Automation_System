#pragma once
#include "afxwin.h"


// CWriteSingle_BinaryDlg dialog

class CWriteSingle_BinaryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteSingle_BinaryDlg)

public:
	CWriteSingle_BinaryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteSingle_BinaryDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_SINGLE_WRITE_BINARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	 
	BOOL m_Check1;
	BOOL m_Check2;
	BOOL m_Check3;
	BOOL m_Check4;
	BOOL m_Check5;
	BOOL m_Check6;
	BOOL m_Check7;
	BOOL m_Check8;
	BOOL m_Check9;
	BOOL m_Check10;
	BOOL m_Check11;
	BOOL m_Check12;
	BOOL m_Check13;
	BOOL m_Check14;
	BOOL m_Check15;
	BOOL m_Check16;
	//BOOL m_close_dlg;
	//UINT m_address;
	//UINT m_slave_id;
	// 
	 
    BOOL	m_Binary_Value[16];


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
	CButton m_closeCheck;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioSingleRegB();
	afx_msg void OnBnClickedRadioMultipleRegB();
	afx_msg void OnBnClickedCheckCloseDlg();
	CEdit m_editor_address;
	afx_msg void OnPaint();
};
