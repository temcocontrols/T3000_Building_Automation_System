#pragma once
#include "afxwin.h"


// CWriteRegistersValueDlg dialog

class CWriteRegistersValueDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteRegistersValueDlg)

public:
	CWriteRegistersValueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteRegistersValueDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PRODUCTREGISTER_WRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString m_string_address;
    CString m_string_counts;
    CString m_string_dataformats;
 
    CString m_string_functions;
    CString m_sting_property;
    CString m_string_type;
    CListBox m_list_value_read;
    CListBox m_list_value_set;
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    afx_msg void OnLbnDblclkListValueSet();
    BOOL m_bool_single;
    afx_msg void OnBnClickedRadioSingle();
    afx_msg void OnBnClickedRadioMultiple();
    CButton m_button_single;
    CButton m_button_multi;
    void SetRadioButton();
    unsigned short Get_Data_Unsigned(int index);
    afx_msg void OnBnClickedCwriteone();
};
