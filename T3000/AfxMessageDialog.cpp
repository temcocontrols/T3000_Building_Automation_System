// AfxMessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "AfxMessageDialog.h"


// CAfxMessageDialog dialog


// CAfxMessageDialog 对话框

IMPLEMENT_DYNAMIC(CAfxMessageDialog, CDialog)
CAfxMessageDialog::CAfxMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAfxMessageDialog::IDD, pParent)
{
	m_bShowOkBtn=TRUE;
	m_nTime=30000;
}

CAfxMessageDialog::~CAfxMessageDialog()
{

}

void CAfxMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAfxMessageDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CAfxMessageDialog 消息处理程序

void CAfxMessageDialog::SetPromtionTxt(CString strText)
{
	m_strPromtionText=strText;

}

void CAfxMessageDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.left=rcClient.left+15;
	rcClient.top=rcClient.top+15;
	rcClient.bottom=rcClient.bottom-30;
	rcClient.right=rcClient.right-15;
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strPromtionText,&rcClient,DT_LEFT|DT_WORDBREAK);
}

void CAfxMessageDialog::OnTimer(UINT nIDEvent)
{
	
	if(nIDEvent==1)
	{
		CAfxMessageDialog::OnCancel();
	}
	if(nIDEvent==2)
	{
		int nSeconds=m_nTime/1000-int((GetTickCount()-m_dwTimeStart)/1000);
		CString strTxt;
		strTxt.Format(_T("%d sec."),nSeconds);
		//GetDlgItem(IDC_TIMERSTATIC)->SetWindowText(strTxt);
		GetDlgItem(IDOK)->SetWindowText(strTxt);
	}
	CDialog::OnTimer(nIDEvent);
}

void CAfxMessageDialog::OnBnClickedOk()
{
	
	OnOK();
}
BOOL CAfxMessageDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetTimer(1,m_nTime,NULL);
	SetTimer(2,200,NULL);
	Invalidate();
	m_dwTimeStart=GetTickCount();
	GotoDlgCtrl(GetDlgItem(IDCANCEL));
	if(!m_bShowOkBtn)
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	return FALSE;
}
void CAfxMessageDialog::OnDestroy()
{
	CDialog::OnDestroy();
	KillTimer(1);
	KillTimer(2);

	
}

void CAfxMessageDialog::OnBnClickedCancel()
{
	
	OnCancel();
}
void CAfxMessageDialog::SetTotalTime(int nTime)
{
	m_nTime=nTime;
}