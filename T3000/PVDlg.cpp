// PVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "PVDlg.h"
#include "afxdialogex.h"

// CPVDlg dialog
 

IMPLEMENT_DYNAMIC(CPVDlg, CDialog)

CPVDlg::CPVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPVDlg::IDD, pParent)
{

}

CPVDlg::~CPVDlg()
{
}

void CPVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CPVDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CPVDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CPVDlg message handlers

BOOL CPVDlg::OnInitDialog(){


CDialog::OnInitDialog();
 InitTab();

return TRUE;
}
void CPVDlg::InitTab(){
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("Item Master");
	int nIdx = m_Tab.InsertItem(0, &tcItem);

	tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("Vendor");
	m_Tab.InsertItem(1, &tcItem);
}
void CPVDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	 
	*pResult = 0;
}
