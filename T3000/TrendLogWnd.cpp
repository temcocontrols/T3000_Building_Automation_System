// TrendLogWnd.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TrendLogWnd.h"


// CTrendLogWnd

IMPLEMENT_DYNAMIC(CTrendLogWnd, CWnd)

CTrendLogWnd::CTrendLogWnd()
{

}

CTrendLogWnd::~CTrendLogWnd()
{
}


BEGIN_MESSAGE_MAP(CTrendLogWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTrendLogWnd message handlers



int CTrendLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetClientRect(&m_rcClient);

	

	return 0;
}

void CTrendLogWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	GetClientRect(&m_rcClient);
	dc.FillSolidRect(&m_rcClient,RGB(0,0,0));
	
	// Do not call CWnd::OnPaint() for painting messages
}
