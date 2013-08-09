// T332AI.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T332AI.h"


// T332AI

IMPLEMENT_DYNCREATE(T332AI, CFormView)

T332AI::T332AI()
	: CFormView(T332AI::IDD)
{

}

T332AI::~T332AI()
{
}

void T332AI::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(T332AI, CFormView)
END_MESSAGE_MAP()


// T332AI diagnostics

#ifdef _DEBUG
void T332AI::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T332AI::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T332AI message handlers
