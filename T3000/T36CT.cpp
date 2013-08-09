// T36CT.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T36CT.h"


// T36CT

IMPLEMENT_DYNCREATE(T36CT, CFormView)

T36CT::T36CT()
	: CFormView(T36CT::IDD)
{

}

T36CT::~T36CT()
{
}

void T36CT::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(T36CT, CFormView)
END_MESSAGE_MAP()


// T36CT diagnostics

#ifdef _DEBUG
void T36CT::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T36CT::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T36CT message handlers
