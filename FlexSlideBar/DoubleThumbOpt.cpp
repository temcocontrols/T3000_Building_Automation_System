#include "StdAfx.h"
#include "DoubleThumbOpt.h"
#include "FlexSlideWnd.h"

CDoubleThumbOpt::CDoubleThumbOpt(void)
{	
	m_bStartDraging = FALSE;
}

CDoubleThumbOpt::~CDoubleThumbOpt(void)
{
}



void CDoubleThumbOpt::SetThumbShape(FSB_THUMBSHAPE tbShape)
{
	m_ltThumb.SetShape(tbShape);
	m_rbThumb.SetShape(tbShape);
}

void CDoubleThumbOpt::SetThumbWidth(int nWidth)
{
	m_ltThumb.SetWidth(nWidth);
	m_rbThumb.SetWidth(nWidth);
}



BOOL CDoubleThumbOpt::InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)
{	
	SetHorizontal(bHorizontal);	
	SetChannelRect(rcChannel);
	SetThumbWidth(nThumbWidth);

	CPoint ptCenter;
	ptCenter = rcChannel.CenterPoint();	
	if (bHorizontal)
	{
		ptCenter.x =rcChannel.left;
	}
	else
	{
		ptCenter.y = rcChannel.top;
	}
	m_ltThumb.InitThumb(ptCenter, 0, THUNMB_RECT_HEIGHT, nThumbWidth, m_bHorizontal);	
	m_rbThumb.InitThumb(ptCenter, 10, THUNMB_RECT_HEIGHT, nThumbWidth, m_bHorizontal);	

	return TRUE;
}

int CDoubleThumbOpt::GetThumbWidth()
{
	return m_ltThumb.GetWidth();
}

COLORREF CDoubleThumbOpt::GetThumbColor()
{
	return m_ltThumb.GetColor();
}

void CDoubleThumbOpt::SetThumbColor(COLORREF clr)
{
	m_ltThumb.SetColor(clr);	
	m_rbThumb.SetColor(clr);	
}

void CDoubleThumbOpt::Draw(CDC* pDC)
{
	m_ltThumb.Draw(pDC);
	m_rbThumb.Draw(pDC);
}


void CDoubleThumbOpt::MovePage(const CPoint& pt)
{	
	
}

void CDoubleThumbOpt::OnLButtonDown(const CPoint& point)
{

}

// 下一步，大的数值
void CDoubleThumbOpt::MoveNextStep()
{/*
	int nPosition = m_thumb.GetPosition();
	CPoint ptPos = m_thumb.GetPixPosition();

	nPosition += 1;
	if (nPosition >= (int)m_szTicMarks->size())
	{
		nPosition = m_szTicMarks->size()-1;
	}

	m_thumb.SetPosition(nPosition);

	if (m_bHorizontal)
	{
		ptPos.x = m_szTicMarks->at(nPosition);
	}
	else
	{
		ptPos.y = m_szTicMarks->at(nPosition);
	}

	m_thumb.MoveThumb(ptPos);
	*/
}


// 上一步，小的数值
void CDoubleThumbOpt::MovePreStep()
{
	/*
	int nPosition = m_thumb.GetPosition();
	CPoint ptPos = m_thumb.GetPixPosition();

	nPosition -= 1;
	if (nPosition < 0)
	{
		nPosition = 0;
	}

	m_thumb.SetPosition(nPosition);

	if (m_bHorizontal)
	{
		ptPos.x = m_szTicMarks->at(nPosition);
	}
	else
	{
		ptPos.y = m_szTicMarks->at(nPosition);
	}

	m_thumb.MoveThumb(ptPos);
	*/
}

void CDoubleThumbOpt::MoveToPosition(int nPosition)
{

}


void CDoubleThumbOpt::OnMouseMove(const CPoint& point)
{
	
}

int CDoubleThumbOpt::GetThumbPosition(const CPoint& pt)
{
	int nPosVal = 0;
	if (m_bHorizontal)
	{
		nPosVal = pt.x - m_rcChannel.left;
	}
	else
	{
		nPosVal = pt.y - m_rcChannel.top;
	}

	//////////////////////////////////////////////////////////////////////////

	if (nPosVal < m_szTicMarks->at(0))
	{
		nPosVal = 0;
	}
	else if(nPosVal > m_szTicMarks->at(m_szTicMarks->size()-1))
	{
		nPosVal = m_szTicMarks->size()-1;
	}
	else
	{
		for (UINT i = 0; i < m_szTicMarks->size()-1; i++)
		{
			int nPre = m_szTicMarks->at(i)	;
			int nNext = m_szTicMarks->at(i+1);
			if (nPosVal >= nPre && nPosVal < nNext)
			{
				nPosVal = i;
			}
			else if(nPosVal == nNext)
			{
				nPosVal = i+1;
			}
		}	
	}

	return nPosVal;
}

void CDoubleThumbOpt::OnLButtonUp(const CPoint& point)
{

// 	if(m_bStartDraging)// begin to drag?
// 	{
// 		m_bStartDraging = FALSE;
// 		//ReleaseCapture();
// 		int nPos = GetThumbPosition(point);
// 		m_thumb.SetPosition(nPos);
// 
// 		MoveToPosition(nPos);
// 	}

	((CFlexSlideWnd*)m_pParent)->SendCallBackMsg();
}




//////////////////////////////////////////////////////////////////////////

void CDoubleThumbOpt::SetTicsArray(vector<int>& szTics)
{
	m_szTicMarks = &szTics;
	//	m_szTicMarks->clear();
	//	m_szTicMarks->insert(m_szTicMarks->end(), szTics.begin(), szTics.end());
}

int CDoubleThumbOpt::GetThumbPosition()
{
	return m_ltThumb.GetPosition();
}

void CDoubleThumbOpt::SetThumbPosition(int nPosition)
{
	MoveToPosition(nPosition);
}