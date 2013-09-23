#pragma once
#include "afxwin.h"


// CPolldefinition dialog

class CPolldefinition : public CDialogEx
{
	DECLARE_DYNAMIC(CPolldefinition)

public:
	CPolldefinition(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPolldefinition();

// Dialog Data
	enum { IDD = IDD_POLL_DEFINITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitAllControls();
	bool Save_Parameter();
	UINT m_poll_id;
	UINT m_poll_address;
	UINT m_poll_length;
	int m_poll_scan_interval;
	CComboBox m_poll_function_ctrl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonReadOnce();
};
