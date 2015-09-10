#include "StdAfx.h"
#include "FSBThumb.h"

CFSBThumb::CFSBThumb(void)
{
	m_clr = GetSysColor(COLOR_3DFACE);

	m_nWidth=12;
	m_nHeight=6;

	m_bIsDragging = FALSE;
	m_bHorizontal = FALSE;
}



CFSBThumb::~CFSBThumb(void)
{
}

void CFSBThumb::SetHorizontal(BOOL bHorizontal)
{
	m_bHorizontal = bHorizontal;
}


void CFSBThumb::SetWidth(int nWidth)
{
	m_nWidth= nWidth;

	if (m_bHorizontal) // 
	{
		int nInflate = (nWidth - m_rc.Height())/2;
		m_rc.InflateRect(0, nInflate);
	}
	else
	{	
		int nInflate = (nWidth - m_rc.Width())/2;
		m_rc.InflateRect(nInflate,0);
	}
}

int CFSBThumb::GetWidth()
{
	return m_nWidth;
}


COLORREF CFSBThumb::GetColor()
{
	return m_clr;
}

void CFSBThumb::SetColor(COLORREF clr)
{
	m_clr = clr;
}

void CFSBThumb::SetShape(FSB_THUMBSHAPE fsbShape)
{
	m_shape = fsbShape;
}

void CFSBThumb::SetPixPosition(CPoint pt)
{
	m_nPosX = pt.x;
	m_nPosY = pt.y;

	m_rc = CRect(pt.x-m_nWidth/2,pt.y-m_nHeight/2,pt.x+m_nWidth/2,pt.y+m_nHeight/2);
	//	InitThumb(CPoint(m_nPosX, m_nPosY),m_nWidth, m_nHeight);
}


CPoint CFSBThumb::GetPixPosition()
{
	CPoint ptRet;
	ptRet.x = m_nPosX;
	ptRet.y = m_nPosY;

	return ptRet;	
}

void CFSBThumb::SetPosition(int nPos)
{
	m_nPosition = nPos;
}


int CFSBThumb::GetPosition()
{
	return m_nPosition;
}



void CFSBThumb::InitThumb(CPoint ptCenter, int nPosition, int nWidth, int nHeight, BOOL bHorizontal)
{
	SetPixPosition(ptCenter);
	SetWidth(nWidth);
	m_nHeight = nHeight;
	m_nPosition = nPosition;
	m_rc = CRect(ptCenter.x-m_nWidth/2,ptCenter.y-m_nHeight/2,ptCenter.x+m_nWidth/2,ptCenter.y+m_nHeight/2);	
}


CRect CFSBThumb::GetThumbRect()
{
	return m_rc;
}


BOOL CFSBThumb::IsPtOnThumb(const CPoint& pt)
{
	return m_rc.PtInRect(pt);
}

void CFSBThumb::MoveThumb(const CPoint& ptNewPosition)
{
	m_nPosX = ptNewPosition.x;
	m_nPosY = ptNewPosition.y;
	m_rc = CRect(ptNewPosition.x-m_nWidth/2,ptNewPosition.y-m_nHeight/2,ptNewPosition.x+m_nWidth/2,ptNewPosition.y+m_nHeight/2);	
}




void CFSBThumb::Draw(CDC* pDC)
{
	ASSERT(pDC);

	switch(m_shape)
	{
	case FSB_THUMBSHAPE_BAR:
		DrawBarThumb(pDC);
		break;
	case FSB_THUMBSHAPE_RECT:
		DrawRectThumb(pDC);
		break;
	case FSB_THUMBSHAPE_CIRCLE:
		DrawCircleThumb(pDC);
		break;
	case FSB_THUMBSHAPE_TRI:
		DrawTriangleThumb(pDC);		
		break;
	default:
		break;
	}

}


void CFSBThumb::DrawBarThumb(CDC* pDC)
{

}
 
void CFSBThumb::SetMixValue(int min){
    m_min = min;
}
void CFSBThumb::DrawRectThumb(CDC* pDC)
{
	CBrush brush(m_clr);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(m_rc);
	//pDC->Draw3dRect(m_rc, RGB(255, 0, 0), RGB(0, 255, 0));
	pDC->Draw3dRect(m_rc, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DSHADOW));
	pDC->SelectObject(pOldBrush);
  
    //CString StrValue;
    //StrValue.Format(_T("%0.1f"),((float)m_nPosition/10));
    //pDC->DrawText(StrValue,m_rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CFSBThumb::DrawCircleThumb(CDC* pDC)
{

}

void CFSBThumb::DrawTriangleThumb(CDC* pDC)
{

}

