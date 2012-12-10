// TrendLogView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TrendLogView.h"
#include "MainFrm.h"

// CTrendLogView

IMPLEMENT_DYNCREATE(CTrendLogView, CFormView)

CTrendLogView::CTrendLogView()
	: CFormView(CTrendLogView::IDD)
{

}

CTrendLogView::~CTrendLogView()
{
}

void CTrendLogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrendLogView, CFormView)
	ON_BN_CLICKED(IDC_GOBACKBUTTON, &CTrendLogView::OnBnClickedGobackbutton)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SAVEBUTTON, &CTrendLogView::OnBnClickedSavebutton)
	ON_BN_CLICKED(IDC_LOADBUTTON, &CTrendLogView::OnBnClickedLoadbutton)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTrendLogView diagnostics

#ifdef _DEBUG
void CTrendLogView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTrendLogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTrendLogView message handlers

void CTrendLogView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	
	m_TrendLogWnd.Create(_T("STATIC"),_T("TrendLogWnd"),WS_VISIBLE|WS_CHILD,m_rcClient,this,6001);
	// TODO: Add your specialized code here and/or call the base class
}
void CTrendLogView::Fresh()
{

}
void CTrendLogView::OnBnClickedGobackbutton()
{
	((CMainFrame*)(theApp.m_pMainWnd))->SwitchToPruductType(0);
	// TODO: Add your control notification handler code here
}

void CTrendLogView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(m_TrendLogWnd.m_hWnd!=NULL)
	{
		GetClientRect(&m_rcClient);
		m_rcClient.top=m_rcClient.top+70;
		m_rcClient.bottom=m_rcClient.bottom-100;
		m_rcClient.left=m_rcClient.left+10;
		m_rcClient.right=m_rcClient.right-10;

		m_TrendLogWnd.MoveWindow(&m_rcClient);
		Invalidate();
	}
	

	// TODO: Add your message handler code here
}

void CTrendLogView::OnBnClickedSavebutton()
{
	// TODO: Add your control notification handler code here
}

void CTrendLogView::OnBnClickedLoadbutton()
{
	// TODO: Add your control notification handler code here
}

int CTrendLogView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetClientRect(&m_rcClient);
	// TODO:  Add your specialized creation code here

	return 0;
}
