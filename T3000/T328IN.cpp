// T328IN.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T328IN.h"


// CT328IN

IMPLEMENT_DYNCREATE(CT328IN, CFormView)

CT328IN::CT328IN()
	: CFormView(CT328IN::IDD)
{

}

CT328IN::~CT328IN()
{
}

void CT328IN::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CT328IN, CFormView)
END_MESSAGE_MAP()


// CT328IN diagnostics

#ifdef _DEBUG
void CT328IN::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT328IN::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT328IN message handlers
