#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
#include "../SQLiteDriver/CppSQLite3.h"
// CManageAccountDlg dialog
typedef struct AccountNode
{
	CString strName;
	CString strpassword;
	//int nLevel;
}Account_Node;

class CManageAccountDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageAccountDlg)

public:
	CManageAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageAccountDlg();

// Dialog Data
	enum { IDD = IDD_USERMANNAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExitbutton();
	CMsflexgrid m_FlexGrid;
	virtual BOOL OnInitDialog();
	void ReloadUserDB();
public:
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	CppSQLite3Table table;
//	CString db_operation_str;
	vector<Account_Node> m_AccountLst;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
	CEdit m_valueEdit;
	int m_nCurRow;
	int m_nCurCol;
	CString m_strUserName;
	CString m_strPassword;
	afx_msg void OnEnKillfocusValueedit();
	void Update_Recorder();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedConfigbutton();
	afx_msg void OnBnClickedDelbutton();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedUseprivilegecheck();
	CButton m_usePrivilegeCheck;
};
