// EreaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "EreaseDlg.h"
#include "globle_function.h"

// CEreaseDlg dialog

IMPLEMENT_DYNAMIC(CEreaseDlg, CDialog)

CEreaseDlg::CEreaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEreaseDlg::IDD, pParent)
{

}

CEreaseDlg::~CEreaseDlg()
{

}

void CEreaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_tstatIDEdit);
}


BEGIN_MESSAGE_MAP(CEreaseDlg, CDialog)
	ON_BN_CLICKED(ID_MYOK, &CEreaseDlg::OnBnClickedMyOk)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CEreaseDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CEreaseDlg message handlers

void CEreaseDlg::OnBnClickedMyOk()
{
	// TODO: Add your control notification handler code here

	OnOK();
}

BOOL CEreaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(ID_MYOK)->SetFocus();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void CEreaseDlg::OnEnKillfocusEdit1()
{

}

BOOL CEreaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nTstatID=g_tstat_id;
	CString strText;
	strText.Format(_T("%d"),m_nTstatID);
	m_tstatIDEdit.SetWindowText(strText);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
