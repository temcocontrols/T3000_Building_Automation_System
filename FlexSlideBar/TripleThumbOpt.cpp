#include "StdAfx.h"
#include "TripleThumbOpt.h"

CTripleThumbOpt::CTripleThumbOpt(void)
{
}

CTripleThumbOpt::~CTripleThumbOpt(void)
{
}



void CTripleThumbOpt::SetThumbShape(FSB_THUMBSHAPE tbShape)
{
	m_thumb.SetShape(tbShape);
}

void CTripleThumbOpt::SetThumbWidth(int nWidth)
{
	m_thumb.SetWidth(nWidth);
}


BOOL CTripleThumbOpt::initThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)
{	
	SetHorizontal(bHorizontal);	
	SetChannelRect(rcChannel);
	SetThumbWidth(nThumbWidth);

	CPoint ptCenter;
	ptCenter = rcChannel.CenterPoint();	
//	m_thumb.InitThumb(ptCenter, THUNMB_RECT_HEIGHT, nThumbWidth);	

	return TRUE;
}

int CTripleThumbOpt::GetThumbWidth()
{
	return m_thumb.GetWidth();
}

COLORREF CTripleThumbOpt::GetThumbColor()
{
	return m_thumb.GetColor();
}

void CTripleThumbOpt::SetThumbColor(COLORREF clr)
{
	m_thumb.SetColor(clr);	
}

void CTripleThumbOpt::Draw(CDC* pDC)
{
	m_thumb.Draw(pDC);
}

