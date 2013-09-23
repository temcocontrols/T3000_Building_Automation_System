
// TestToolDlg.h : header file
//

#pragma once
#include "resource.h"
#include "ModbusDllforVc.h"
#include "afxcmn.h"
#include "MyOwnListCtrl.h"


const int   MENU_FILE				=		0;
const int	MENU_CONNECTION			=		1;
const int	MENU_SETUP				=		2;
const int	MENU_FUNCTION			=		3;
const int	MENU_DISPLAY			=		4;

const int	TYPE_SIGNED=0;
const int	TYPE_UNSIGNED=1;
const int	TYPE_HEX=2;
const int	TYPE_BINARY=3;


const int   FUNCTION_REGISTER_MONITOR		=		0;
const int	FUNCTION_LC_TEST_JAT			=		1;
const int	FUNCTION_MODBUS_POLL			=		2;

const int	ENUM_RESET_COUNTER	= 0;
const int   ENUM_POLL_DEFINITION		= 2;
const int	ENUM_DISCONNECTION	= 4;
const int	ENUM_CONNECTION= 5;
const int	ENUM_READ_ONCE= 7;
const int	ENUM_WRITE_SINGLE_REGISTER= 8;


// CTestToolDlg dialog
class CTestToolDlg : public CDialogEx
{
// Construction
public:
	CTestToolDlg(CWnd* pParent = NULL);	// standard constructor
	virtual	~CTestToolDlg();
// Dialog Data
	enum { IDD = IDD_TESTTOOL_DIALOG };
	CToolBar m_wndToolBar;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedRegMonitor();
	//afx_msg void OnBnClickedLCTestJat();
	void InitStateBar();
	CStatusBarCtrl* m_StatusBar;
	afx_msg void OnConnectionConnect();
//	afx_msg void OnUpdateConnectionConnect(CCmdUI *pCmdUI);
	afx_msg void OnConnectionQuickconnect();
	

public:
	void Refresh_Menu_And_ToolBar();
	void Fresh_TX_ERR_ID(int nTX,int nErr,int nID,int nSR,int is_it_auto_read);
	void InitListCtrl();
	void InitParameters();
	CString DecToBin(CString strDec);
	afx_msg void OnConnectionDisconnect();
	CMyOwnListCtrl m_list_control;

	static	DWORD WINAPI Get_All_Dlg_Message(LPVOID lpVoid);
	static	DWORD WINAPI Translate_My_Message(LPVOID lpVoid);

	int m_Show_Data_Type;
	int m_function_type;
	CRect ListRect;
	afx_msg void OnSetupPolldefinition();
	afx_msg void OnDisplaySigned();
	afx_msg void OnDisplayUnsigned();
	afx_msg void OnDisplayHex();
	afx_msg void OnDisplayBinary();
	afx_msg BOOL  OnToolTipNotify(UINT id,NMHDR *Pnmhdr,LRESULT *pResult);
	afx_msg LRESULT OpenWriteDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT FreshListControl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ShowMSG(WPARAM wParam, LPARAM lParam);

	void OnHdnBegintrackList1(NMHDR *pNMHDR, LRESULT *pResult) ;
	HANDLE Main_Read_Thread;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetupResetcounters();
	afx_msg void OnFunctionReadonce();
	afx_msg void OnFunction06();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnFileExit();
	afx_msg void OnFunctionRegistermonitor();
	afx_msg void OnFunctionLctestjat();
	afx_msg void OnFunction3();
	afx_msg void OnAboutAbout();
	afx_msg void OnDestroy();
};
#pragma once


