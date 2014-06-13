// T3000DefaultView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000DefaultView.h"


// CT3000DefaultView

IMPLEMENT_DYNCREATE(CT3000DefaultView, CFormView)

CT3000DefaultView::CT3000DefaultView()
	: CFormView(CT3000DefaultView::IDD)
{

}

CT3000DefaultView::~CT3000DefaultView()
{
}

void CT3000DefaultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_RichEditor);
}

BEGIN_MESSAGE_MAP(CT3000DefaultView, CFormView)
END_MESSAGE_MAP()


// CT3000DefaultView diagnostics

#ifdef _DEBUG
void CT3000DefaultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3000DefaultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT3000DefaultView message handlers


void CT3000DefaultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
}
void CT3000DefaultView::Fresh(){
	CString Windowtext=_T("\n\nT3000 Building Automation System");
	m_RichEditor.SetWindowText(_T("T3000 Building Automation System"));
	m_RichEditor.SetFontSize(30);
	COLORREF cf=RGB(212,208,200);
	m_RichEditor.SetBackgroundColor(FALSE,cf);
	cf=RGB(84,141,212);
	m_RichEditor.SetSelectTextColor(cf);
	m_RichEditor.SetSel(0,0);
}
