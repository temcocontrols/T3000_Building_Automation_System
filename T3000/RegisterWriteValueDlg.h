#pragma once
#include "afxwin.h"


// CRegisterWriteValueDlg dialog

class CRegisterWriteValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterWriteValueDlg)

public:
	CRegisterWriteValueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterWriteValueDlg();
	virtual BOOL OnInitDialog();
	void Set(int Col,CString Type,CString Name,CString No,CString Value,CString description,CString operation);
	BOOL m_CanWrite;
// Dialog Data
	enum { IDD = IDD_DIALOG_WRITE_REGISTERVIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	CStatic m_reg_name;
//	CStatic m_reg_no;
	CEdit m_valueEdit;
	CEdit m_descriptionEdit;
	CString m_caption;
	CString m_name;
	CString m_no;
	CString m_value;
	CString m_description;
	CString m_operation;
	CString m_type;
	int m_col;//0:是Value 1：Description
	CEdit m_reg_name;
	CEdit m_reg_no;
	int m_color;//0:默认值，1：红色
	CButton m_sendBtn;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
