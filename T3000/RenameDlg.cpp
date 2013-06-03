
// RenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RenameDlg.h"
#include "afxdialogex.h"


// CRenameDlg dialog

IMPLEMENT_DYNAMIC(CRenameDlg, CDialog)

	CRenameDlg::CRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameDlg::IDD, pParent)
	, m_nodename(_T(""))
{

}

CRenameDlg::~CRenameDlg()
{
}

void CRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME_EDIT, m_nodename);
}


BEGIN_MESSAGE_MAP(CRenameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRenameDlg message handlers

BOOL CRenameDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;
}
void CRenameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CDialog::OnOK();
}
