// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "LoginDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(BOOL &ok_or_no,CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	m_bOk=ok_or_no;
}

CLoginDlg::~CLoginDlg()
{
}
void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USEREDIT, m_userEdit);
	DDX_Control(pDX, IDC_PASSWORDEDIT, m_psswordEdit);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CLoginDlg::OnBnClickedOk()
{
	CString strText;
	m_userEdit.GetWindowText(strText);
	if(strText.IsEmpty())
	{
		AfxMessageBox(_T("Please input user name first!"));
		return;
	}
	CString strPasswordText;
	m_psswordEdit.GetWindowText(strPasswordText);

	CString strSql;
     SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	strSql.Format(_T("select * from users where user_name_login='%s'and password_login='%s'"),strText,strPasswordText);
	 q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	if(!q.eof())
	{//found
		m_bOk=TRUE;
		g_buser_log_in=TRUE;
		if(strText.CompareNoCase(_T("admin"))==0)
			g_UserLevel=0;
		else
			g_UserLevel=1;

	 
	g_strLoginUserName=strText;
	}
	else
	{
		//not find
		AfxMessageBox(_T("Erro account and passworld!"));
		g_buser_log_in=FALSE;
		m_bOk=FALSE;
	 
		return;
	}

	SqliteDBT3000.closedb();
	OnOK();
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnBnClickedCancel()
{
	
	OnCancel();
}

void CLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();
	 
	
}
