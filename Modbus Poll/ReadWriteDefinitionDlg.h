#pragma once
#include "afxwin.h"

#include "ChildFrm.h"
// CReadWriteDefinitionDlg dialog

class CReadWriteDefinitionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReadWriteDefinitionDlg)

public:
	CReadWriteDefinitionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReadWriteDefinitionDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_RW_DEFINITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
 
public:
	UINT m_slave_id;
	UINT m_scan_rate;
	UINT m_quantity;
	UINT m_address;
	int m_function;
	int m_data_formate;
	int m_rows;
	 
	int m_hide_alias_col;
	int m_address_in_cell;

	CComboBox m_combox_function;
	CComboBox m_combox_dataformate;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio20();
	afx_msg void OnBnClickedRadio50();
	afx_msg void OnBnClickedRadio100();
	afx_msg void OnBnClickedRadiofit();
	BOOL m_plc_addresses;

    CButton m_Radio_10;
	CButton m_Radio_20;
	CButton m_Radio_50;
	CButton m_Radio_100;
	CButton m_Radio_Fit;
	CEdit m_editor_slave_id;


	BOOL m_apply;
	BOOL m_wronce;
	afx_msg void OnPaint();
//	BOOL m_is_read_write_once;
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonRwOnce();
	HWND m_pWnd;
	 
};
