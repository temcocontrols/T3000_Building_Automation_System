#pragma once
#include "afxwin.h"
#include "CM5/CStatic/staticex.h"

// CBacnetATCommand dialog

class CBacnetATCommand : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetATCommand)

public:
	CBacnetATCommand(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetATCommand();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_AT_COMMAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonAtSend();
	CStaticEx m_at_command_send;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT Fresh_RX_AT_Command(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
};
