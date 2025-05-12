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
	BacnetWait(int wait_type = 0);
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
	void Set_Read_Write(bool read_0_write_1);
	CStaticEx m_wait_detail;
	CProgressCtrl m_wait_progress;
	//static DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid);
	
	CStaticEx m_wait_persent;
	int m_wait_type;//0 µã»÷£»  1 Ð´config  £»2  ¶Áconfig;
	CString PosString;
	int m_pos;
public:
	void Set_Show_String(CString temp);
	void Set_Pos(int temp_pos);
	virtual void PostNcDestroy();
};

const int READ_MONITORDATA_TIME = 6 ;
