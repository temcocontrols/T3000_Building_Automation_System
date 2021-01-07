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
	void InitialBacnetWindow();
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
	LRESULT BacnetView_Message_Handle(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Show_Progress(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Show_Panel_String(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Change_Next_Panel(WPARAM wParam,LPARAM lParam);
//	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT RS485_Read_Fun(WPARAM wParam,LPARAM lParam);



	//static	DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid);
	void Initial_All_Point();
	void Show_Wait_Dialog_And_SendMessage(int read_list_type);
	void WriteFlash();
	void SetConnected_IP(LPCTSTR myip);
	void Set_Device_Type(bool is_remote_device);
	void Set_remote_device_IP(LPCTSTR ipaddress);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTabCtrl m_bac_main_tab;
	void Tab_Initial();
	void Set_Tab_Loaded_Parameter(int ntab);
	void Inital_Tab_Loaded_Parameter();
	void Initial_Some_UI(int ntype);

	afx_msg void OnTcnSelchangeBacMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_cur_tab_sel;
	int m_bac_obtain_ip_mode;

	bool tab_loaded[WINDOW_TAB_COUNT];
	unsigned int last_serial_number;
};
//static void Init_Service_Handlers(void);
DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid);
DWORD WINAPI  MSTP_Send_read_Command_Thread(LPVOID lpVoid);
DWORD WINAPI  MSTP_Write_Command_Thread(LPVOID lpVoid);
DWORD WINAPI  Mstp_Connect_Thread(LPVOID lpVoid);
DWORD WINAPI RS485_Connect_Thread(LPVOID lpvoid);
DWORD WINAPI RS485_Read_Each_List_Thread(LPVOID lpvoid);
DWORD WINAPI Handle_Bip_whois_Thread(LPVOID lpvoid);










