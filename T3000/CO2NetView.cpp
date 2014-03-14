// CO2NetView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2NetView.h"


// CCO2NetView

IMPLEMENT_DYNCREATE(CCO2NetView, CFormView)

CCO2NetView::CCO2NetView()
	: CFormView(CCO2NetView::IDD)
{

}

CCO2NetView::~CCO2NetView()
{
}

void CCO2NetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCO2NetView, CFormView)
END_MESSAGE_MAP()


// CCO2NetView diagnostics
void CCO2NetView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

}
#ifdef _DEBUG
void CCO2NetView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCO2NetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CCO2NetView::Fresh(){

}

// CCO2NetView message handlers
