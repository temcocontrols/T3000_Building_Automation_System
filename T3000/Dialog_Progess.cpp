// Dialog_Progess.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Dialog_Progess.h"



// CDialog_Progess dialog

IMPLEMENT_DYNAMIC(CDialog_Progess, CDialog)

CDialog_Progess::CDialog_Progess(CWnd* pParent /*=NULL*/,int lower,int upper)
	: CDialog(CDialog_Progess::IDD, pParent)
{

	m_lower = lower;
	m_upper = upper;

}

CDialog_Progess::~CDialog_Progess()
{
}

void CDialog_Progess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(CDialog_Progess, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialog_Progess message handlers

BOOL CDialog_Progess::OnInitDialog()
{
	CDialog::OnInitDialog();

			
	//设置进度条的范围
		m_progress.SetRange(m_lower,m_upper);
	//设置进度条的每一步的增量
	//	m_progress.SetStep(m_setsetup);
	//设置进度条的当前位置
//	SetTimer(1, 1000, NULL);


	
	



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDialog_Progess::PreTranslateMessage(MSG* pMsg)
{


// 	int x;
// 	for (int i = 1;i<101;i++)
// 	{
// 		CString CStemp;
// 		CStemp.Format(_T("%d％"),i);
// 		SetDlgItemText(IDC_STATIC1,CStemp);
// 
// 		m_progress.SetPos(i);
// 	}	
// 	x = 100;



	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}



void CDialog_Progess::OnClose()
{
	 

	CDialog::OnClose();
	
}

void CDialog_Progess::OnOK()
{
	

	CDialog::OnOK();
}

void CDialog_Progess::ShowProgress( int setpos,int percent )
{

	CString CStemp;
	CStemp.Format(_T("%d％"),percent);
	SetDlgItemText(IDC_STATIC1,CStemp);
	m_progress.SetPos(setpos);


}
void CDialog_Progess::OnTimer(UINT_PTR nIDEvent)
{
	 

if(nIDEvent == 1)
{
	static int cont = 1;

	if(m_start)
	{	
		if (m_end)
		{
			ShowProgress(cont,100);
			//this->ShowWindow(SW_HIDE);

		}else
		{
			ShowProgress(cont,cont);
			cont++;
		}

	}
}


	CDialog::OnTimer(nIDEvent);
}

void CDialog_Progess::settimer( BOOL bpram,int lower,int upper,BOOL bend )
{
	//this->ShowWindow(SW_SHOW);
//	this->CenterWindow();
	
// 	m_lower = lower;
// 	m_upper = upper;
	m_start = bpram;
	m_end = bend;



}

int CDialog_Progess::GetPOS()
{
	return m_progress.GetPos();
}