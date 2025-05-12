// T3000DefaultView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000DefaultView.h"
#include "global_function.h"
HBITMAP 	hBitmap_default_bmp;
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
	//  DDX_Control(pDX, IDC_RICHEDIT21, m_RichEditor);
}

BEGIN_MESSAGE_MAP(CT3000DefaultView, CFormView)
	ON_WM_PAINT()
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
        hBitmap_default_bmp =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  
            MAKEINTRESOURCE(IDB_BITMAP_BUILDING_DEFAULT),  
            IMAGE_BITMAP,0,0,  
            LR_LOADMAP3DCOLORS);
   
	CFormView::OnInitialUpdate();
}
void CT3000DefaultView::Fresh()
{
	Sleep(1);
}

void CT3000DefaultView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CFormView::OnPaint() for painting messages
#if 0
	CMemDC memDC(dc,this);
	Graphics graphics(memDC.GetDC());

	CRect test_rect;
	HWND temp_hwnd = this->m_hWnd;
	::GetWindowRect(temp_hwnd,&test_rect);	//获取 view的窗体大小;
	Bitmap bitmap(hBitmap_default_bmp,NULL);
	graphics.DrawImage(&bitmap,0 ,0,test_rect.Width(),test_rect.Height());
#endif
}
