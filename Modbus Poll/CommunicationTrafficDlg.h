#pragma once
#include "afxwin.h"
class CCommunicationTrafficDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCommunicationTrafficDlg)

public:
	CCommunicationTrafficDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommunicationTrafficDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_COMMUNICATION_TRAFFIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListBox m_DataList;
    BOOL m_is_pause;
protected:
	afx_msg LRESULT OnTrafficDataWindow(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedSave();
 
	afx_msg void OnBnClickedCopy();
	afx_msg void OnClose();
    CButton m_btn_copy;
    CButton m_btn_save;
    CButton m_btn_stop;
//    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
