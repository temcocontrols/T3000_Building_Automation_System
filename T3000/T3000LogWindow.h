#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CT3000LogWindow 对话框

class CT3000LogWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CT3000LogWindow)

public:
	CT3000LogWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CT3000LogWindow();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOG_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLvnItemchangedListT3000Log(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    CListBox m_t3000_log_winddow_listbox;
    afx_msg LRESULT CT3000LogWindow::ShowString(WPARAM wParam, LPARAM lParam);
};
