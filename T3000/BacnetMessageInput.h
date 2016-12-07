#pragma once

#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
// CBacnetMessageInput dialog

class CBacnetMessageInput : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetMessageInput)

public:
	CBacnetMessageInput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetMessageInput();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_MESSAGE_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CStaticEx m_static_title;
	CStaticEx m_edit_return;
};
