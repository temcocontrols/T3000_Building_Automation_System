// LoginMode.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "LoginMode.h"
#include "afxdialogex.h"


// CLoginMode dialog

IMPLEMENT_DYNAMIC(CLoginMode, CDialogEx)

CLoginMode::CLoginMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginMode::IDD, pParent)
    , m_user_name(_T("guest"))
    , m_password(_T("guest"))
{

}

CLoginMode::~CLoginMode()
{
}

void CLoginMode::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_user_name);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
}


BEGIN_MESSAGE_MAP(CLoginMode, CDialogEx)
    ON_BN_CLICKED(IDOK, &CLoginMode::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginMode message handlers


void CLoginMode::OnBnClickedOk()
{
    

     CString StrSQL;
     UpdateData(TRUE);//get the username and password from UI
     StrSQL.Format(_T("Select * from users where user_name_login = '%s' and password_login = '%s'"),m_user_name,m_password);
     g_database_operator.m_pRecordset = g_database_operator.OpenRecordset(StrSQL);
    int recordcount = g_database_operator.GetRecordCount(g_database_operator.m_pRecordset);
    if (recordcount > 0)
    {
        g_login_user.Person_Name = m_user_name;
        g_login_user.Person_Roles = g_database_operator.m_pRecordset->GetCollect("roles");
        g_database_operator.CloseRecordset();
    }
    else
    {
        g_database_operator.CloseRecordset();
        AfxMessageBox(_T("Your user name or password is wrong!"));
        return;
    }
     CDialogEx::OnOK();
}
