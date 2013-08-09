// T38AI16O.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38AI16O.h"


// T38AI16O

IMPLEMENT_DYNCREATE(T38AI16O, CFormView)

T38AI16O::T38AI16O()
	: CFormView(T38AI16O::IDD)
{

}

T38AI16O::~T38AI16O()
{
}

void T38AI16O::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(T38AI16O, CFormView)
END_MESSAGE_MAP()


// T38AI16O diagnostics

#ifdef _DEBUG
void T38AI16O::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T38AI16O::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T38AI16O message handlers
