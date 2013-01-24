// IODisplayBar.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "IODisplayBar.h"


// CIODisplayBar

IMPLEMENT_DYNAMIC(CIODisplayBar, CDockablePane)

CIODisplayBar::CIODisplayBar()
{

	//m_pSetDlg= new CIOSetDlg();

}

CIODisplayBar::~CIODisplayBar()
{
	//delete m_pSetDlg;
}


BEGIN_MESSAGE_MAP(CIODisplayBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CIODisplayBar message handlers



int CIODisplayBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	//m_pSetDlg->CReate();
	return 0;
}

void CIODisplayBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting


}

void CIODisplayBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(&rcClient);
	//if(m_pSetDlg->m_hWnd!=NULL)
	{
	//	m_pSetDlg->MoveWindow(rcClient,1);
	}

//	Invalidate();
	// TODO: Add your message handler code here
}
