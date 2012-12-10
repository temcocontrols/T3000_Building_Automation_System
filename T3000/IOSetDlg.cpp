// IOSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "IOSetDlg.h"


// CIOSetDlg dialog

IMPLEMENT_DYNAMIC(CIOSetDlg, CDialog)

CIOSetDlg::CIOSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIOSetDlg::IDD, pParent)
{

}

CIOSetDlg::~CIOSetDlg()
{
}

void CIOSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIOSetDlg, CDialog)
END_MESSAGE_MAP()

BOOL CIOSetDlg::CReate()
{
	return CDialog::Create(CIOSetDlg::IDD,this);

}

// CIOSetDlg message handlers
