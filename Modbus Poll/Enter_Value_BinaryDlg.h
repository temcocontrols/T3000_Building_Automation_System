#pragma once


// CEnter_Value_BinaryDlg dialog

class CEnter_Value_BinaryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnter_Value_BinaryDlg)

public:
	CEnter_Value_BinaryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnter_Value_BinaryDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_ENTER_BINARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_Check16;
	BOOL m_Check15;
	BOOL m_Check14;
	BOOL m_Check13;
	BOOL m_Check12;
	BOOL m_Check11;
	BOOL m_Check10;
	BOOL m_Check9;
	BOOL m_Check8;
	BOOL m_Check7;
	BOOL m_Check6;
	BOOL m_Check5;
	BOOL m_Check4;
	BOOL m_Check3;
	BOOL m_Check2;
    BOOL m_Check1;
	BOOL m_Binary_Value[16];
	 
	afx_msg void OnBnClickedOk();
};
