#include "StdAfx.h"
#include "SingleThumbOpt.h"
#include "FlexSlideWnd.h"


CSingleThumbOpt::CSingleThumbOpt(void)
{
	m_bStartDraging = FALSE;
}

CSingleThumbOpt::~CSingleThumbOpt(void)
{
	
}


void CSingleThumbOpt::SetThumbShape(FSB_THUMBSHAPE tbShape)
{
	m_thumb.SetShape(tbShape);
}

void CSingleThumbOpt::SetThumbWidth(int nWidth)
{
	m_thumb.SetWidth(nWidth);
}


BOOL CSingleThumbOpt::InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)
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
	m_thumb.InitThumb(ptCenter, 0, THUNMB_RECT_HEIGHT, nThumbWidth,m_bHorizontal);	

	return TRUE;
}

int CSingleThumbOpt::GetThumbWidth()
{
	return m_thumb.GetWidth();
}

COLORREF CSingleThumbOpt::GetThumbColor()
{
	return m_thumb.GetColor();
}

void CSingleThumbOpt::SetThumbColor(COLORREF clr)
{
	m_thumb.SetColor(clr);	
}

void CSingleThumbOpt::Draw(CDC* pDC)
{
	m_thumb.Draw(pDC);
}


void CSingleThumbOpt::MovePage(const CPoint& pt)
{	
	CPoint ptPos = m_thumb.GetPixPosition();
	int nPosition	= m_thumb.GetPosition();
	
	CPoint ptNewPS = ptPos;
	int		nNewPosition = nPosition;

	if (m_bHorizontal)
	{

		if (pt.x > ptPos.x)  // move right
		{
			nNewPosition = nPosition+m_nLengthOfPage;

			if(nNewPosition < (int)m_szTicMarks->size())
			{				
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}
			else
 			{
				nNewPosition = m_szTicMarks->size()-1;
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
 			}
		}
		else // move left
		{
			nNewPosition = nPosition - m_nLengthOfPage;
			if(nNewPosition >= 0)
			{				
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}
			else
			{
				nNewPosition = 0;
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}

		}
	}
	else
	{
		if (pt.y > ptPos.y)  // move down
		{
			nNewPosition = nPosition+m_nLengthOfPage;

			if(nNewPosition < (int)m_szTicMarks->size())
			{				
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}
			else
			{
				nNewPosition = m_szTicMarks->size()-1;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}

		}
		else // move up
		{
			nNewPosition = nPosition - m_nLengthOfPage;
			if(nNewPosition >= 0)
			{				
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}
			else
			{
				nNewPosition = 0;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}		
		}
	}

	m_thumb.SetPosition(nNewPosition);
	m_thumb.MoveThumb(ptNewPS);



};

void CSingleThumbOpt::OnLButtonDown(const CPoint& point)
{
	if(m_thumb.IsPtOnThumb(point)) // begin to drag?//lsc
	{	
		m_bStartDraging = TRUE;
		//HWND hwnd = SetCapture(AfxGetApp()->GetMainWnd()->);
	}
	else
	{
		MovePage(point);
	}
}


// 下一步，大的数值
void CSingleThumbOpt::MoveNextStep()
{
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
}


// 上一步，小的数值
void CSingleThumbOpt::MovePreStep()
{
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
}

void CSingleThumbOpt::MoveToPosition(int nPosition)
{
	int nPixPos = m_szTicMarks->at(nPosition);
	CPoint pt = m_thumb.GetPixPosition();
	if (m_bHorizontal)
	{
		pt.x = nPixPos+m_rcChannel.left;
	}
	else
	{
		pt.y = nPixPos+m_rcChannel.top;
	}

	m_thumb.SetPosition(nPosition);
	m_thumb.MoveThumb(pt);
}


void CSingleThumbOpt::OnMouseMove(const CPoint& point)
{
	if(m_bStartDraging) // begin to drag?
	{
		CPoint ptPos = m_thumb.GetPixPosition();
		CPoint ptNewPS = ptPos;

		if (m_bHorizontal)
		{
			ptNewPS.x  = point.x;
				
			if(point.x > m_rcChannel.right)
			{
				ptNewPS.x = m_rcChannel.right;
			}
			if(point.x < m_rcChannel.left)
			{
				ptNewPS.x = m_rcChannel.left;
			}
		}
		else   // vertical
		{	
			ptNewPS.y = point.y;
			if(point.y > m_rcChannel.bottom)
			{
					ptNewPS.y = m_rcChannel.bottom;
			}
			if(point.y < m_rcChannel.top)
			{
				ptNewPS.y = m_rcChannel.top;
			}
	
		}

		m_thumb.MoveThumb(ptNewPS);
	}
}

int CSingleThumbOpt::GetThumbPosition(const CPoint& pt)
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

	if (nPosVal < m_szTicMarks->at(0))//lsc
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

void CSingleThumbOpt::OnLButtonUp(const CPoint& point)
{
	if(m_bStartDraging)// begin to drag?
	{
		m_bStartDraging = FALSE;
		//ReleaseCapture();
		int nPos = GetThumbPosition(point);
		m_thumb.SetPosition(nPos);

		MoveToPosition(nPos);
	}
	((CFlexSlideWnd*)m_pParent)->SendCallBackMsg();
}




//////////////////////////////////////////////////////////////////////////

void CSingleThumbOpt::SetTicsArray(vector<int>& szTics)
{
	m_szTicMarks = &szTics;
//	m_szTicMarks->clear();
//	m_szTicMarks->insert(m_szTicMarks->end(), szTics.begin(), szTics.end());
}

int CSingleThumbOpt::GetThumbPosition()
{
	return m_thumb.GetPosition();
}

void CSingleThumbOpt::SetThumbPosition(int nIndex, int nPosition)
{
	MoveToPosition(nPosition);
}

int CSingleThumbOpt::GetAllThumbPosition(vector<int>& szThumbPos)
{
	szThumbPos.push_back(GetThumbPosition());

	return szThumbPos.size();
}

void  CSingleThumbOpt::CalcThumbPosition()
{


}

