#pragma once
#include "CM5/CStatic/staticex.h"
#include "afxwin.h"
#include "afxcmn.h"

// BacnetWait dialog

class BacnetWait : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetWait)

public:
	BacnetWait(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetWait();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStaticEx m_wait_detail;
	CProgressCtrl m_wait_progress;
	static DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid);
	
};
