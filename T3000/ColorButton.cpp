
#include "stdafx.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
	dx_ = ::GetSystemMetrics(SM_CXEDGE);
	if (dx_ == 0)
		dx_ = 2;
	dy_ = ::GetSystemMetrics(SM_CYEDGE);
	if (dy_ == 0)
		dy_ = 2;
	dx_ += 3;
	dy_ += 3;
}


CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTATE, OnSetState)
END_MESSAGE_MAP()


void CColorButton::SetColorRef(const COLORREF *color)
{
	prgb_color_ = color;

	if (m_hWnd)
		Invalidate();
}


void CColorButton::PaintIt(int offset)
{
	RECT rect;
	GetClientRect(&rect);
	rect.bottom -= dy_-offset;
	rect.right -= dx_-offset;
	rect.top += dy_+offset;
	rect.left += dx_+offset;

	CClientDC dc(this);
	CBrush brush(::GetSysColor(COLOR_BTNTEXT));
	dc.FillSolidRect(&rect,*prgb_color_);
	dc.FrameRect(&rect,&brush);
}


/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::OnPaint() 
{
	//  CPaintDC dc(this); // device context for painting
	CButton::OnPaint();	// wywo³anie Default() - narysowanie guzika
	int x= SendMessage(BM_GETSTATE,0,0);
	PaintIt(x & BST_PUSHED ? 1 : 0);
}


afx_msg LRESULT CColorButton::OnSetState(WPARAM wParam, LPARAM /* lParam */)
{
	CButton::Default();
	PaintIt(wParam ? 1 : 0);
	return 0;
}
