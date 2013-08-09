// T38I13O.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38I13O.h"


// T38I13O

IMPLEMENT_DYNCREATE(T38I13O, CFormView)

T38I13O::T38I13O()
	: CFormView(T38I13O::IDD)
{

}

T38I13O::~T38I13O()
{
}

void T38I13O::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(T38I13O, CFormView)
END_MESSAGE_MAP()


// T38I13O diagnostics

#ifdef _DEBUG
void T38I13O::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T38I13O::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T38I13O message handlers
