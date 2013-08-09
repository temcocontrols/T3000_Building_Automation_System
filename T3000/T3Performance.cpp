// T3Performance.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3Performance.h"


// CT3Performance

IMPLEMENT_DYNCREATE(CT3Performance, CFormView)

CT3Performance::CT3Performance()
	: CFormView(CT3Performance::IDD)
{

}

CT3Performance::~CT3Performance()
{
}

void CT3Performance::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CT3Performance, CFormView)
END_MESSAGE_MAP()


// CT3Performance diagnostics

#ifdef _DEBUG
void CT3Performance::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3Performance::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT3Performance message handlers
