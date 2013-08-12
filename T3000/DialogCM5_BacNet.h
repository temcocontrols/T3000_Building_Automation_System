#pragma once

#include "CM5\MyOwnListCtrl.h"
#include "afxcmn.h"

// CDialogCM5_BacNet form view

class CDialogCM5_BacNet : public CFormView
{
	DECLARE_DYNCREATE(CDialogCM5_BacNet)

protected:
	
	virtual ~CDialogCM5_BacNet();

public:
	CDialogCM5_BacNet();           // protected constructor used by dynamic creation
	enum { IDD = IDD_DIALOG_CM5_BACNET };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	virtual void OnInitialUpdate();
	void Fresh();

	bool CM5_Thread;
	bool m_MSTP_THREAD;
	HANDLE CM5_hThread;
	DWORD nThreadID;
	static DWORD WINAPI   MSTP_Receive(LPVOID lpVoid);
	void Initial_List();
//	CMyOwnListCtrl m_device_list_info;
	afx_msg LRESULT Fresh_UI(WPARAM wParam,LPARAM lParam);
	
	afx_msg void OnBnClickedButtonCm5Advance();
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonCm5Programing();
	afx_msg void OnBnClickedButtonCm5Output();
};
static void Init_Service_Handlers(void);





