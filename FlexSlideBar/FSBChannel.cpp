#include "StdAfx.h"
#include "FSBChannel.h"

CFSBChannel::CFSBChannel(void)
{
	m_szRC.resize(1);
}

CFSBChannel::~CFSBChannel(void)
{
	for (UINT i = 0 ; i < m_szRC.size(); i++)
	{
		delete m_szRC[i];
	}
	m_szRC.clear();
}



void CFSBChannel::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

int CFSBChannel::GetWidth()
{
	return m_nWidth;
}


void CFSBChannel::SetHeight(int nHeight)
{
	m_nHeight = nHeight;	
}

int CFSBChannel::GetHeight()
{
	return m_nHeight;
}

void  CFSBChannel::SetColor(COLORREF clr)
{
	m_clr = clr;
}

COLORREF CFSBChannel::GetColor()
{
	return m_clr;
}

void CFSBChannel::SetRect(CRect rc)
{
	m_rc = rc;
}

CRect CFSBChannel::GetRect()
{
	return m_rc;
}


int CFSBChannel::SetRCArray(vector<RC_CLR>& szRcs)
{
	UINT nSize = m_szRC.size();
	for (UINT i = 0 ; i < nSize; i++)
	{
		delete m_szRC[i];
	}
	m_szRC.clear();
	//m_szRC.insert(m_szRC.end(), szRcs.begin(), szRcs.end());

	for (UINT i = 0 ; i < szRcs.size(); i++)
	{
		RC_CLR* rc = new RC_CLR;
		rc->rc = szRcs[i].rc;
		rc->clr = szRcs[i].clr;
		m_szRC.push_back(rc);
	}
	return m_szRC.size();
}


void CFSBChannel::Draw(CDC* pDC)
{
	ASSERT(pDC);
		
	//CBrush brush(COLOR_3DFACE);
	//CBrush* pOldBrush = pDC->SelectObject(&brush);

	// create and select a thick, black pen
	//CPen penBlack;
	//penBlack.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	//CPen* pOldPen = pDC->SelectObject(&penBlack);

	//pDC->Rectangle(m_rc);
	//pDC->Draw3dRect(m_rc, RGB(255, 0, 0), RGB(0, 255, 0));
	pDC->Draw3dRect(m_rc, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHIGHLIGHT));

		
	// put back the old objects
	//pDC->SelectObject(pOldBrush);
	//pDC->SelectObject(pOldPen);
//if (m_szRC.size()!=1)//lsc加的，当增加一人滑块后在这里出错m_szRC.size()没有数据
//{
	//////////////////////////////////////////////////////////////////////////
 	for (UINT i = 0; i < m_szRC.size(); i++)
 	{
 	 	pDC->FillSolidRect(m_szRC[i]->rc,  m_szRC[i]->clr);
// 		if (i==0)
// 		{
// 			int left=m_szRC[i]->rc.left+1;
// 			int right=m_szRC[i]->rc.right-1;
// 			for (UINT j=m_szRC[i]->rc.top+1;j<m_szRC[i]->rc.bottom;j+=5)
// 			{
// 
// 				CPoint PointA(left,j),PointB(left+16,j);
// 
// 				pDC->LineTo(PointA);
// 				pDC->MoveTo(PointB);
// 
// 			}
// 		}
		
 	}
//}



}

