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
//	str_temp="select * from users where user_name_login = '"+m_user_name
//		+"' and password_login = '"+m_password+"'";
	strSql.Format(_T("select * from users where user_name_login='%s'and password_login='%s'"),strText,strPasswordText);
	m_pRs->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	
	if(VARIANT_FALSE==m_pRs->EndOfFile)
	{//found
		m_bOk=TRUE;
		g_buser_log_in=TRUE;
		if(strText.CompareNoCase(_T("admin"))==0)
			g_UserLevel=0;
		else
			g_UserLevel=1;

	if(m_pRs->State) 
		m_pRs->Close();
	g_strLoginUserName=strText;
	}
	else
	{
		//not find
		AfxMessageBox(_T("Erro account and passworld!"));
		g_buser_log_in=FALSE;
		m_bOk=FALSE;
		if(m_pRs->State) 
			m_pRs->Close();
		return;
	}


	OnOK();
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pCon.CreateInstance("ADODB.Connection");
	m_pRs.CreateInstance("ADODB.Recordset");//
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();
	if(m_pRs->State) 
		m_pRs->Close(); 
	if(m_pCon->State) 
		m_pCon->Close();
	// TODO: Add your message handler code here
}
