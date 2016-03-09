#pragma once


// TestCenter dialog

class TestCenter : public CDialogEx
{
	DECLARE_DYNAMIC(TestCenter)

public:
	TestCenter(CWnd* pParent = NULL);   // standard constructor
	virtual ~TestCenter();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEST_CENTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
    BOOL CheckInputData(CString strText);
    unsigned short CStringToHex(CString Text);
    unsigned short HexCharToInt(char ch);
	CButton m_addlist_btn;
	CButton m_copy_btn;
	CButton m_openlist_btn;
	CButton m_savelist_btn;
	CButton m_add_check;
	CListBox m_list;
	CComboBox m_combox_send;
	 
};
