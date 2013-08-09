// T3RTDView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3RTDView.h"


// CT3RTDView

IMPLEMENT_DYNCREATE(CT3RTDView, CFormView)

CT3RTDView::CT3RTDView()
	: CFormView(CT3RTDView::IDD)
{

}

CT3RTDView::~CT3RTDView()
{
}

void CT3RTDView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CT3RTDView, CFormView)
END_MESSAGE_MAP()


// CT3RTDView diagnostics

#ifdef _DEBUG
void CT3RTDView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3RTDView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT3RTDView message handlers
