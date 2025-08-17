#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CT3000LogWindow dialog
// CT3000LogWindow �Ի���

class CT3000LogWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CT3000LogWindow)

public:
	CT3000LogWindow(CWnd* pParent = NULL);   // Standard constructor - ��׼���캯��
	virtual ~CT3000LogWindow();

// Dialog data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOG_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support - DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLvnItemchangedListT3000Log(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    CListBox m_t3000_log_winddow_listbox;
    afx_msg LRESULT CT3000LogWindow::ShowString(WPARAM wParam, LPARAM lParam);
};
