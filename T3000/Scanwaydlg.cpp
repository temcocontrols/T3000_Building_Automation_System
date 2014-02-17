// Scanwaydlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Scanwaydlg.h"
#include "afxdialogex.h"


// CScanwaydlg dialog

IMPLEMENT_DYNAMIC(CScanwaydlg, CDialogEx)

CScanwaydlg::CScanwaydlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScanwaydlg::IDD, pParent)
{
Is_Deep=0;
}

CScanwaydlg::~CScanwaydlg()
{
}

void CScanwaydlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScanwaydlg, CDialogEx)
//	ON_BN_CLICKED(IDC_QUICK, &CScanwaydlg::OnBnClickedQuick)
    ON_BN_CLICKED(IDC_QUICK, &CScanwaydlg::OnBnClickedQuick)
    ON_BN_CLICKED(IDOK, &CScanwaydlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CScanwaydlg message handlers


//void CScanwaydlg::OnBnClickedQuick()
//{
//	// TODO: Add your control notification handler code here
//	//CDialog::OnOK();
//}


void CScanwaydlg::OnBnClickedQuick()
{
     Is_Deep=2;
    CDialog::OnOK();
}


void CScanwaydlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    Is_Deep=1;
    CDialogEx::OnOK();
}
