#pragma once

#include "CM5\MyOwnListCtrl.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"

#include "CM5/CStatic/staticex.h"

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
	//HANDLE CM5_hThread;
	//HANDLE CM5_UI_Thread;
	DWORD nThreadID;
	DWORD cm5_nThreadID;
	//static DWORD WINAPI   MSTP_Receive(LPVOID lpVoid);
	//static DWORD WINAPI   CM5_UI(LPVOID lpVoid);
	void Initial_List();
//	CMyOwnListCtrl m_device_list_info;
	afx_msg LRESULT Fresh_UI(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  AllMessageCallBack(WPARAM wParam, LPARAM lParam);
	LRESULT Delete_New_Dlg(WPARAM wParam,LPARAM lParam);

//	afx_msg void OnBnClickedButton1();




	static	DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid);
	void Initial_All_Point();
	void Show_Wait_Dialog_And_SendMessage(int read_list_type);
	void WriteFlash();


	afx_msg void OnBnClickedButtonBacTest();
	CDateTimeCtrl m_cm5_time_picker;
	CDateTimeCtrl m_cm5_date_picker;
	afx_msg void OnBnClickedBacEnableEditTime();
	afx_msg void OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult);
	void Get_Time_Edit_By_Control();
	afx_msg void OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnBacWriteTime();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTabCtrl m_bac_main_tab;
	void Tab_Initial();
	

	afx_msg void OnTcnSelchangeBacMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_cur_tab_sel;
};
//static void Init_Service_Handlers(void);











