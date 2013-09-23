#pragma once


// CWrite_Single_Reg dialog

class CWrite_Single_Reg : public CDialogEx
{
	DECLARE_DYNAMIC(CWrite_Single_Reg)

public:
	CWrite_Single_Reg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWrite_Single_Reg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WRITE_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonWriteSend();
	afx_msg LRESULT FunctionCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
