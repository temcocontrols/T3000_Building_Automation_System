// T38AI8AO.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38AI8AO.h"


// T38AI8AO

IMPLEMENT_DYNCREATE(T38AI8AO, CFormView)

T38AI8AO::T38AI8AO()
	: CFormView(T38AI8AO::IDD)
{

}

T38AI8AO::~T38AI8AO()
{
}

void T38AI8AO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(T38AI8AO, CFormView)
END_MESSAGE_MAP()


// T38AI8AO diagnostics

#ifdef _DEBUG
void T38AI8AO::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T38AI8AO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T38AI8AO message handlers
