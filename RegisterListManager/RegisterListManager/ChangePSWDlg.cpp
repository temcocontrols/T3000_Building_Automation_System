// ChangePSWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "ChangePSWDlg.h"
#include "afxdialogex.h"


// CChangePSWDlg dialog

IMPLEMENT_DYNAMIC(CChangePSWDlg, CDialogEx)

CChangePSWDlg::CChangePSWDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangePSWDlg::IDD, pParent)
    , m_str_user(g_login_user.Person_Name)
{

    m_str_password = _T("");
}

CChangePSWDlg::~CChangePSWDlg()
{
}

void CChangePSWDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_str_user);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_str_password);
}


BEGIN_MESSAGE_MAP(CChangePSWDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CChangePSWDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangePSWDlg message handlers


void CChangePSWDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //CDialogEx::OnOK();
    CString StrTemp;
    BOOL Can_GO;
    if (g_login_user.Person_Roles.CompareNoCase(L"engineer")==0)
    {

        Can_GO = TRUE;

    }
    else if (g_login_user.Person_Roles.CompareNoCase(L"admin")==0)
    {
        Can_GO = TRUE;
    }
    else
    {
        Can_GO = FALSE;
    }
    if (!Can_GO)
    {
        StrTemp.Format(_T("User Name:'%s' have no permission to change anything!"),g_login_user.Person_Name);
        AfxMessageBox(StrTemp);
        return;
    }
    CString StrSQL;
    UpdateData(TRUE);//get the username and password from UI
    StrSQL.Format(_T("Update users Set password_login = '%s' where user_name_login = '%s' "),m_str_password,m_str_user);
    g_database_operator.m_pConnection->Execute(StrSQL.GetBuffer(),NULL,adCmdText);
    AfxMessageBox(_T("Execute successfully!")) ;
}
