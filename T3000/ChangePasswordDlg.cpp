// ChangePasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ChangePasswordDlg.h"


// CChangePasswordDlg dialog

IMPLEMENT_DYNAMIC(CChangePasswordDlg, CDialog)

CChangePasswordDlg::CChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePasswordDlg::IDD, pParent)
{

}

CChangePasswordDlg::~CChangePasswordDlg()
{
}

void CChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_userEdt);
	DDX_Control(pDX, IDC_OLDPASSWORDEDT, m_userOldPwEdt);
	DDX_Control(pDX, IDC_EDIT2, m_newPwEdt);
	DDX_Control(pDX, IDC_EDIT3, m_verifyPwEdt);
}


BEGIN_MESSAGE_MAP(CChangePasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CChangePasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()
// CChangePasswordDlg message handlers

BOOL CChangePasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChangePasswordDlg::OnBnClickedOk()
{
	
	OnOK();
}
