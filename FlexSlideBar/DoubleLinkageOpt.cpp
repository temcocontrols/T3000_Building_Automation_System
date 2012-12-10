#include "StdAfx.h"
#include "DoubleLinkageOpt.h"

CDoubleLinkageOpt::CDoubleLinkageOpt(void)
{
}

CDoubleLinkageOpt::~CDoubleLinkageOpt(void)
{
}





void CDoubleLinkageOpt::SetThumbShape(FSB_THUMBSHAPE tbShape)
{
	m_thumb.SetShape(tbShape);
}

void CDoubleLinkageOpt::SetThumbWidth(int nWidth)
{
	m_thumb.SetWidth(nWidth);
}


BOOL CDoubleLinkageOpt::initThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)
{	
// 	SetHorizontal(bHorizontal);	
// 	SetChannelRect(rcChannel);
// 	SetThumbWidth(nThumbWidth);
// 
// 	CPoint ptCenter;
// 	ptCenter = rcChannel.CenterPoint();	
// 	m_thumb.InitThumb(ptCenter, THUNMB_RECT_HEIGHT, nThumbWidth);	

	return TRUE;
}

int CDoubleLinkageOpt::GetThumbWidth()
{
	return m_thumb.GetWidth();
}

COLORREF CDoubleLinkageOpt::GetThumbColor()
{
	return m_thumb.GetColor();
}

void CDoubleLinkageOpt::SetThumbColor(COLORREF clr)
{
	m_thumb.SetColor(clr);	
}

void CDoubleLinkageOpt::Draw(CDC* pDC)
{
	m_thumb.Draw(pDC);
}

