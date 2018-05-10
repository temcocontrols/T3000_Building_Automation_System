// T3000TableView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"

#include "T3000Doc.h"
#include "T3000TableView.h"

#include "T3000View.h"
#include "GraphicView.h"
#include "NetworkControllView.h"
// CT3000TableView

IMPLEMENT_DYNCREATE(CT3000TableView, CTabView)

CT3000TableView::CT3000TableView()
{

}

CT3000TableView::~CT3000TableView()
{
}

BEGIN_MESSAGE_MAP(CT3000TableView, CTabView)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CT3000TableView diagnostics

#ifdef _DEBUG
void CT3000TableView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3000TableView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT3000TableView message handlers
CT3000TableView* pTableView=NULL;
int CT3000TableView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	pTableView=this;
	AddView (RUNTIME_CLASS (CT3000View), _T("Tstate"), 100);
	AddView (RUNTIME_CLASS (CGraphicView), _T("Graphic"), 101);
	AddView (RUNTIME_CLASS (CNetworkControllView), _T("Network Ctrl"), 102);
	AddView (RUNTIME_CLASS (CTrendLogView), _T("Trend Log View"), 101);
	
	this->SetActiveView(0);
	

	return 0;
}
LRESULT CT3000TableView::OnChangeActiveTab(WPARAM wp,LPARAM lp)
{

	CString str;
    str.Format (_T("AFX_WM_CHANGE_ACTIVE_TAB: %d"), (int) wp);

	return 1;
}
void CT3000TableView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
