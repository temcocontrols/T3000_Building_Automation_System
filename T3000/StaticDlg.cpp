// StaticDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "StaticDlg.h"
#include "afxdialogex.h"


// CStaticDlg dialog

IMPLEMENT_DYNAMIC(CStaticDlg, CDialog)

CStaticDlg::CStaticDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStaticDlg::IDD, pParent)
{

}

CStaticDlg::~CStaticDlg()
{
}

void CStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAB, m_name);
}


BEGIN_MESSAGE_MAP(CStaticDlg, CDialog)
END_MESSAGE_MAP()
void CStaticDlg::SetText(CString lab){
m_lab=lab;
}
BOOL CStaticDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		CRect rect;
     ScreenToClient(rect);
	 m_name.MoveWindow(rect);
     m_name.SetWindowText(m_lab);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// CStaticDlg message handlers
