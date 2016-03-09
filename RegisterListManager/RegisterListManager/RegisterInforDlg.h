#pragma once


// CRegisterInforDlg dialog

class CRegisterInforDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterInforDlg)

public:
	CRegisterInforDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterInforDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_REGISTER_INFORMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CString m_strAdd;
    CString m_strComment;
    CString m_strName;
    int m_current_index;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButtonForward();
    afx_msg void OnBnClickedButtonBackward();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
