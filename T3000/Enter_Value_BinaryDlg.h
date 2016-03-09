#pragma once


// CEnter_Value_BinaryDlg dialog

class CEnter_Value_BinaryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnter_Value_BinaryDlg)

public:
	CEnter_Value_BinaryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnter_Value_BinaryDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ENTER_BINARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    BOOL m_bool_check1;
//    BOOL m_m_bool_check2;
    BOOL m_bool_check10;
    BOOL m_bool_check11;
    BOOL m_bool_check12;
    BOOL m_bool_check13;
    BOOL m_bool_check14;
    BOOL m_bool_check15;
    BOOL m_bool_check16;
    BOOL m_bool_check2;
    BOOL m_bool_check3;
    BOOL m_bool_check4;
    BOOL m_bool_check5;
    BOOL m_bool_check6;
    BOOL m_bool_check7;
    BOOL m_bool_check8;
    BOOL m_bool_check9;
    BOOL m_Binary_Value[16];
    CString m_string_value ;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
};
