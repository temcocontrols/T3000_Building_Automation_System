#include "StdAfx.h"
#include "TripleLinkageOpt.h"
#include "FlexSlideWnd.h"


CTripleLinkageOpt::CTripleLinkageOpt(void)
{
	m_szThumb.reserve(3);
	for (int i = 0; i < 3; i++)
	{
		CFSBThumb* pThumb = new CFSBThumb;
		m_szThumb.push_back(pThumb);
	}

	m_nFocusThumb = 1;
}

CTripleLinkageOpt::~CTripleLinkageOpt(void)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		delete m_szThumb[i];
	}
}


int CTripleLinkageOpt::GetThumbNum()
{
	return 3;
}

void CTripleLinkageOpt::SetParentWnd(CWnd* pParent)
{
	m_pParent = pParent;
}


void CTripleLinkageOpt::SetThumbPosition(int nFirstPos, int nMidPos, int nLastPos)
{
	CPoint ptFirst, ptMid, ptLast;
	ptFirst= ptMid = ptLast = m_rcChannel.CenterPoint();


	if (m_bHorizontal)
	{
		ptFirst.x = m_szTicMarks->at(nFirstPos);
		ptMid.x = m_szTicMarks->at(nMidPos);
		ptLast.x = m_szTicMarks->at(nLastPos);
	}
	else
	{
		ptFirst.y = m_szTicMarks->at(nFirstPos);
		ptMid.y  = m_szTicMarks->at(nMidPos);
		ptLast.y = m_szTicMarks->at(nLastPos);
	}

	m_szThumb[0]->SetPosition(nFirstPos);
	m_szThumb[0]->MoveThumb(ptFirst);

	m_szThumb[1]->SetPosition(nMidPos);
	m_szThumb[1]->MoveThumb(ptFirst);

	m_szThumb[2]->SetPosition(nLastPos);
	m_szThumb[2]->MoveThumb(ptFirst);

	ReCalcChannelRect();
}


void CTripleLinkageOpt::SetThumbShape(FSB_THUMBSHAPE tbShape)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->SetShape(tbShape);		
	}
	
}

void CTripleLinkageOpt::SetThumbWidth(int nWidth)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->SetWidth(nWidth);
	}
}


BOOL CTripleLinkageOpt::InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)
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

	ASSERT(m_szThumb.size() <= m_szTicMarks->size());

	int nPos = 0;
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		nPos = i;
		if (m_bHorizontal)
		{
			ptCenter.x = m_szTicMarks->at(nPos)+m_rcChannel.left;
		}
		else
		{
			ptCenter.y = m_szTicMarks->at(nPos)+m_rcChannel.top;
		}

		m_szThumb[i]->InitThumb(ptCenter, nPos, nThumbWidth, THUNMB_RECT_HEIGHT, m_bHorizontal);	
	}
	
	ReCalcChannelRect();

	return TRUE;
}

int CTripleLinkageOpt::GetThumbWidth()
{
	ASSERT(m_szThumb.size() > 0);
	return m_szThumb[0]->GetWidth();
}

COLORREF CTripleLinkageOpt::GetThumbColor()
{
	ASSERT(m_szThumb.size() > 0);
	return m_szThumb[0]->GetColor();
}

void CTripleLinkageOpt::SetThumbColor(COLORREF clr)
{
	for (UINT i = 0 ; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->SetColor(clr);
	}

	m_szThumb[1]->SetColor(RGB(0,0,128));
	//GetSysColor(COLOR_BTNFACE)
	//m_szThumb[1]->SetColor(GetSysColor(COLOR_BTNFACE));
}


void CTripleLinkageOpt::Draw(CDC* pDC)
{


		m_szThumb[0]->SetColor(GetSysColor(COLOR_BTNFACE));
		//m_szThumb[0]->SetColor(RGB(255,0,0));
		m_szThumb[0]->Draw(pDC);

		m_szThumb[1]->Draw(pDC);


		m_szThumb[2]->SetColor(GetSysColor(COLOR_BTNFACE));
		//m_szThumb[2]->SetColor(RGB(0,0,255));
		m_szThumb[2]->Draw(pDC);

}


void CTripleLinkageOpt::MovePage(const CPoint& pt)
{	
	ASSERT(m_nFocusThumb != FOCUS_ON_EMPTY);
	//if (m_nFocusThumb==-1)
	//{
//	return;
//	}
	
	if (m_nFocusThumb == FOCUS_ON_MID)
	{
		MovePageForMainThumb(pt);
	}
	else
	{
		MovePageForOtherThumb(pt);
	}

}

// 专门为两个从属thumb movepage使用
void CTripleLinkageOpt::MovePageForOtherThumb(const CPoint& pt)
{

	CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];
	ASSERT(pThumb);

	CPoint ptPos = pThumb->GetPixPosition();
	int nPosition	= pThumb->GetPosition();

	int nMax, nMin;
	GetThumbRange(m_nFocusThumb, nMin, nMax);

	CPoint ptNewPS = ptPos;
	int		nNewPosition = nPosition;

	if (m_bHorizontal)
	{
		if (pt.x > ptPos.x)  // move right
		{
			nNewPosition = nPosition+m_nLengthOfPage;

			if(nNewPosition <= nMax)
			{				
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}
			else
			{
				nNewPosition = nMax;
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}
		}
		else // move left
		{
			nNewPosition = nPosition - m_nLengthOfPage;
			if(nNewPosition >= nMin)
			{				
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}
			else
			{
				nNewPosition = nMin;
				ptNewPS.x = m_szTicMarks->at(nNewPosition)+m_rcChannel.left;
			}
		}
	}
	else
	{
		if (pt.y > ptPos.y)  // move down
		{
			nNewPosition = nPosition-m_nLengthOfPage; // down -

			if(nNewPosition <= nMin)
			{		
				nNewPosition = nMin;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;						
			}
			else
			{
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}

		}
		else // move up
		{
			nNewPosition = nPosition + m_nLengthOfPage;
			if(nNewPosition >= nMax)
			{		
				nNewPosition = nMax;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;		

			}
			else
			{
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}		
		}
	}

	pThumb->SetPosition(nNewPosition);
	pThumb->MoveThumb(ptNewPS);

}


// 专门为主Thumb使用
void CTripleLinkageOpt::MovePageForMainThumb(const CPoint& pt)
{
	CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];
	ASSERT(pThumb);

	CPoint ptPos = pThumb->GetPixPosition();
	CPoint ptltPos = m_szThumb[FOCUS_ON_TOPLEFT]->GetPixPosition();
	CPoint ptbrPos = m_szThumb[FOCUS_ON_RIGHTBTM]->GetPixPosition();
	
	int nPosition	= pThumb->GetPosition();
	int nltPos	  = m_szThumb[FOCUS_ON_TOPLEFT]->GetPosition();
	int nbrPos = m_szThumb[FOCUS_ON_RIGHTBTM]->GetPosition();

	
	int nMax = m_szTicMarks->size()-1;
	int nMin = 0;
	//GetThumbRange(m_nFocusThumb, nMin, nMax);

	CPoint ptNewPS = ptPos;
	CPoint ptNewltPS = ptltPos;
	CPoint ptNewbrPS = ptbrPos;

	int		nNewPos = nPosition;
	int		nNewltPos = nltPos;
	int		nNewbrPos = nbrPos;
	
	if (m_bHorizontal)
	{
		if (pt.x > ptPos.x)  // move right
		{
			nNewbrPos = nbrPos+m_nLengthOfPage;
			if(nNewbrPos <= nMax)
			{				
				nNewPos = nPosition+m_nLengthOfPage;
				nNewltPos = nltPos+m_nLengthOfPage;

				ptNewbrPS.x = m_szTicMarks->at(nNewbrPos)+m_rcChannel.left;
				ptNewltPS.x = m_szTicMarks->at(nNewltPos)+m_rcChannel.left;
				ptNewPS.x = m_szTicMarks->at(nNewPos)+m_rcChannel.left;

			}
			else
			{
				nNewbrPos = nMax;
				int nSteps = nMax - nbrPos;

				nNewPos = nPosition+nSteps;
				nNewltPos = nltPos+nSteps;

				ptNewbrPS.x = m_szTicMarks->at(nNewbrPos)+m_rcChannel.left;
				ptNewltPS.x = m_szTicMarks->at(nNewltPos)+m_rcChannel.left;
				ptNewPS.x = m_szTicMarks->at(nNewPos)+m_rcChannel.left;
			}
		}
		else // move left
		{
			nNewltPos = nPosition - m_nLengthOfPage;
			if(nNewltPos >= nMin)
			{				
				nNewPos = nPosition-m_nLengthOfPage;
				nNewbrPos = nbrPos-m_nLengthOfPage;

				ptNewbrPS.x = m_szTicMarks->at(nNewbrPos)+m_rcChannel.left;
				ptNewltPS.x = m_szTicMarks->at(nNewltPos)+m_rcChannel.left;
				ptNewPS.x = m_szTicMarks->at(nNewPos)+m_rcChannel.left;
			}
			else
			{
				nNewltPos = nMin;
				int nSteps = nltPos - nNewltPos;

				nNewPos = nPosition-nSteps;
				nNewbrPos = nbrPos-nSteps;

				ptNewbrPS.x = m_szTicMarks->at(nNewbrPos)+m_rcChannel.left;
				ptNewltPS.x = m_szTicMarks->at(nNewltPos)+m_rcChannel.left;
				ptNewPS.x = m_szTicMarks->at(nNewPos)+m_rcChannel.left;
			}
		}
		pThumb->SetPosition(nNewPos);
		pThumb->MoveThumb(ptNewPS);

		m_szThumb[FOCUS_ON_TOPLEFT]->SetPosition(nNewltPos);
		m_szThumb[FOCUS_ON_TOPLEFT]->MoveThumb(ptNewltPS);


		m_szThumb[FOCUS_ON_RIGHTBTM]->SetPosition(nNewbrPos);
		m_szThumb[FOCUS_ON_RIGHTBTM]->MoveThumb(ptNewbrPS);
	}
	else
	{
		// 交换
		int nTemp = nltPos;
		nltPos = nbrPos;
		nbrPos = nTemp;
		
		//CPoint ptTemp = 0;

		//
		if (pt.y > ptPos.y)  // move down
		{
			nNewbrPos = nbrPos-m_nLengthOfPage;// 向下是减 

			if(nNewbrPos <= nMin)
			{				
				//ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
				nNewbrPos = nMin;
				int nSteps = abs(nbrPos - nNewbrPos);

				nNewPos = nPosition-nSteps;
				nNewltPos = nltPos-nSteps;

				ptNewbrPS.y = m_szTicMarks->at(nNewbrPos)+m_rcChannel.top;
				ptNewltPS.y = m_szTicMarks->at(nNewltPos)+m_rcChannel.top;
				ptNewPS.y = m_szTicMarks->at(nNewPos)+m_rcChannel.top;

			}
			else
			{
	
				nNewPos = nPosition-m_nLengthOfPage;
				nNewltPos = nltPos-m_nLengthOfPage;

				ptNewbrPS.y = m_szTicMarks->at(nNewbrPos)+m_rcChannel.top;
				ptNewltPS.y = m_szTicMarks->at(nNewltPos)+m_rcChannel.top;
				ptNewPS.y = m_szTicMarks->at(nNewPos)+m_rcChannel.top;
			}

		}
		else // move up
		{
			nNewltPos = nltPos + m_nLengthOfPage;  // up +
			
			if(nNewltPos >= nMax)
			{	
				nNewltPos = nMax;
				int nSteps = abs(nNewltPos - nltPos);

				nNewPos = nPosition+nSteps;
				nNewbrPos = nbrPos+nSteps;

				ptNewbrPS.y = m_szTicMarks->at(nNewbrPos)+m_rcChannel.top;
				ptNewltPS.y = m_szTicMarks->at(nNewltPos)+m_rcChannel.top;
				ptNewPS.y = m_szTicMarks->at(nNewPos)+m_rcChannel.top;
			}
			else
			{
				nNewPos = nPosition+m_nLengthOfPage;
				nNewbrPos = nbrPos+m_nLengthOfPage;

				ptNewbrPS.y = m_szTicMarks->at(nNewbrPos)+m_rcChannel.top;
				ptNewltPS.y = m_szTicMarks->at(nNewltPos)+m_rcChannel.top;
				ptNewPS.y = m_szTicMarks->at(nNewPos)+m_rcChannel.top;


			}		
		}
		pThumb->SetPosition(nNewPos);
		pThumb->MoveThumb(ptNewPS);

		m_szThumb[FOCUS_ON_TOPLEFT]->SetPosition(nNewbrPos);
		m_szThumb[FOCUS_ON_TOPLEFT]->MoveThumb(ptNewbrPS);


		m_szThumb[FOCUS_ON_RIGHTBTM]->SetPosition(nNewltPos);
		m_szThumb[FOCUS_ON_RIGHTBTM]->MoveThumb(ptNewltPS);
	}



}

void CTripleLinkageOpt::OnLButtonDown(const CPoint& point)
{
	int nRet  = GetFocusThumb(point);
	
	
	 //m_nFocusThumb = nRet;
	if(nRet != -1) // begin to drag?
	{	
		m_nFocusThumb = nRet;

 		if( m_nFocusThumb == FOCUS_ON_TOPLEFT || m_nFocusThumb == FOCUS_ON_RIGHTBTM) //single button模式 两个从属的不能拖动
 		{
 			return;
 		}		
		m_bStartDraging = TRUE;
		//HWND hwnd = SetCapture(AfxGetApp()->GetMainWnd()->);
	}
	else
	{
	   
		//if( m_nFocusThumb == FOCUS_ON_TOPLEFT || m_nFocusThumb == FOCUS_ON_RIGHTBTM) //single button模式 两个从属的不能拖动
		//{
		//	return;
		//}
		//MovePage(point);
		//
		//ReCalcChannelRect();
	}
}


// 下一步，大的数值
void CTripleLinkageOpt::MoveNextStep()
{
	/*
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
void CTripleLinkageOpt::MovePreStep()
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

void CTripleLinkageOpt::MoveToPosition(int nIndex, int nPosition)
{
	CFSBThumb* pThumb = m_szThumb[nIndex];

	int nPixPos = m_szTicMarks->at(nPosition);
	CPoint pt = pThumb->GetPixPosition();

	if (m_bHorizontal)
	{
		pt.x = nPixPos+m_rcChannel.left;
	}
	else
	{
		pt.y = nPixPos+m_rcChannel.top;
	}

	pThumb->SetPosition(nPosition);
	pThumb->MoveThumb(pt);
}


void CTripleLinkageOpt::OnMouseMove(const CPoint& point)
{
    if (m_nFocusThumb == FOCUS_ON_EMPTY)
    {
	return ;

    }
    
	ASSERT(m_nFocusThumb != FOCUS_ON_EMPTY);

	if(m_bStartDraging) // begin to drag?
	{
		if (m_nFocusThumb == FOCUS_ON_MID)
		{
			MouseMoveForMainThumb(point);
		}
		else
		{
			MouseMoveForOtherThumb(point);
		}

		ReCalcChannelRect();
	}
	
}

// 专门为两个从属thumb movepage使用
void CTripleLinkageOpt::MouseMoveForMainThumb(const CPoint& point)
{

	if(m_bStartDraging) // begin to drag?
	{
		CFSBThumb* pThumb = m_szThumb[FOCUS_ON_MID];

		CPoint ptPos = pThumb->GetPixPosition();
		CPoint ptltPos = m_szThumb[FOCUS_ON_TOPLEFT]->GetPixPosition();
		CPoint ptrbPos = m_szThumb[FOCUS_ON_RIGHTBTM]->GetPixPosition();

		CPoint ptNewPS = ptPos;
		CPoint ptltNewPS = ptltPos;
		CPoint ptrbNewPS = ptrbPos;
				
		
		int nMax = m_szTicMarks->size()-1;
		int nMin = 0;


		int nPixMin, nPixMax;
		nPixMin = m_szTicMarks->at(nMin);
		nPixMax = m_szTicMarks->at(nMax);

		if (m_bHorizontal)
		{
			nPixMax+=m_rcChannel.left;
			nPixMin+=m_rcChannel.left;
			
			ptltNewPS.x  = ptltPos.x + point.x-ptPos.x;
			ptrbNewPS.x  = ptrbPos.x + point.x-ptPos.x;
			ptNewPS.x  = point.x;


			if(ptrbNewPS.x >= nPixMax)  
			{
				ptrbNewPS.x = nPixMax;
				int nDist = ptrbNewPS.x-ptrbPos.x;

				ptNewPS.x = ptPos.x - nDist;
				ptltNewPS.x = ptltPos.x - nDist;
			}

			if(ptltNewPS.x < nPixMin)
			{
				ptltNewPS.x = nPixMin;
				int nDist = ptltPos.x-ptltNewPS.x;

				ptNewPS.x = ptPos.x - nDist;
				ptrbNewPS.x = ptrbPos.x - nDist;
			}
			pThumb->MoveThumb(ptNewPS);
			m_szThumb[0]->MoveThumb(ptltNewPS);
			m_szThumb[2]->MoveThumb(ptrbNewPS);
		}
		else   // vertical
		{	
			nPixMin = m_szTicMarks->at(nMax);
			nPixMax = m_szTicMarks->at(nMin);
			// 交换
			CPoint ptTemp = ptltPos;
			ptltPos = ptrbPos;
			ptrbPos = ptTemp;
			 ptNewPS = ptPos;
			 ptltNewPS = ptltPos;
			 ptrbNewPS = ptrbPos;
			
			nPixMax+=m_rcChannel.top;
			nPixMin+=m_rcChannel.top;
		
			ptltNewPS.y = ptltPos.y + point.y-ptPos.y;
			ptrbNewPS.y = ptrbPos.y + point.y-ptPos.y; 
			ptNewPS.y = point.y;

			if(ptrbNewPS.y >= nPixMax)   // 超过极大值
			{
				ptrbNewPS.y = nPixMax;
				int nDist = abs(ptrbNewPS.y-ptrbPos.y);

				ptNewPS.y = ptPos.y+nDist;
				ptltNewPS.y = ptltPos.y + nDist;
			}
			if(ptltNewPS.y <= nPixMin)
			{
				ptltNewPS.y = nPixMin;
				int nDist = abs(ptltPos.y-ptltNewPS.y);

				ptNewPS.y = ptPos.y - nDist;
				ptrbNewPS.y = ptrbPos.y - nDist;
			}
			pThumb->MoveThumb(ptNewPS);
			m_szThumb[2]->MoveThumb(ptltNewPS);
			m_szThumb[0]->MoveThumb(ptrbNewPS);

		}
	}

}



// 专门为主Thumb使用
void CTripleLinkageOpt::MouseMoveForOtherThumb(const CPoint& point)
{
	CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];

	if(m_bStartDraging) // begin to drag?
	{

		CPoint ptPos = pThumb->GetPixPosition();
		CPoint ptNewPS = ptPos;
		int nMax, nMin;
		GetThumbRange(pThumb->GetPosition(), nMin, nMax);

		int nPixMin, nPixMax;
		nPixMin = m_szTicMarks->at(nMin);
		nPixMax = m_szTicMarks->at(nMax);


		if (m_bHorizontal)
		{
			nPixMax+=m_rcChannel.left;
			nPixMin+=m_rcChannel.left;
			ptNewPS.x  = point.x;

			if(point.x >= nPixMax)
			{
				ptNewPS.x = nPixMax;
			}
			if(point.x < nPixMin)
			{
				ptNewPS.x = nPixMin;
			}
		}
		else   // vertical
		{	
			nPixMin = m_szTicMarks->at(nMax);
			nPixMax = m_szTicMarks->at(nMin);

			nPixMax+=m_rcChannel.top;
			nPixMin+=m_rcChannel.top;
			ptNewPS.y = point.y;
			if(point.y >= nPixMax)
			{
				ptNewPS.y = nPixMax;
			}
			if(point.y < nPixMin)
			{
				ptNewPS.y = nPixMin;
			}

		}

		pThumb->MoveThumb(ptNewPS);
	}
}






int CTripleLinkageOpt::SearchThumbPosition(const CPoint& pt)
{
	int nPosVal = 0;
	int nMin, nMax;
	int nMinPos, nMaxPos;
	if (m_bHorizontal)
	{
		nPosVal = pt.x - m_rcChannel.left;
		nMinPos = 0;
		nMaxPos = m_szTicMarks->size()-1;

	}
	else
	{
		nPosVal = pt.y - m_rcChannel.top;
		nMaxPos = 0;
		nMinPos = m_szTicMarks->size()-1;
	}
	nMax =  m_szTicMarks->at(nMaxPos);
	nMin =  m_szTicMarks->at(nMinPos);	
	//////////////////////////////////////////////////////////////////////////

	if (nPosVal <= nMin)
	{
		nPosVal = nMinPos;
	}
	else if(nPosVal >= nMax)
	{
		nPosVal = nMaxPos;
	}
	else
	{
		for (UINT i = 0; i < m_szTicMarks->size()-1; i++)
		{
			int nPre, nNext;
			if (m_bHorizontal)
			{	
				nPre = m_szTicMarks->at(i);
				nNext = m_szTicMarks->at(i+1);
			}
			else
			{
				nNext = m_szTicMarks->at(i);
				nPre = m_szTicMarks->at(i+1);
			}

			if (nPosVal >= nPre && nPosVal < nNext)
			{
				nPosVal = i;
				break;
			}
			else if(nPosVal == nNext)
			{
				nPosVal = i+1;
				break;
			}
		}	
	}

	return nPosVal;
}

void CTripleLinkageOpt::OnLButtonUp(const CPoint& point)
{
	if(m_bStartDraging)// begin to drag?
	{
		if (m_nFocusThumb == FOCUS_ON_MID)
		{
				LBtnUpForMainThumb(point);
		}
		else
		{
				LBtnUpForOtherThumb(point);
		}		

		ReCalcChannelRect();


		if (m_nFocusThumb == FOCUS_ON_MID)
		{
			((CFlexSlideWnd*)m_pParent)->SendCallBackMsg();
		}
	}
	
	
//	if (m_nFocusThumb != -1)
//	{
//	((CFlexSlideWnd*)m_pParent)->SendCallBackMsg();
//	}
	//else
	//{
	//((CFlexSlideWnd*)m_pParent)->SendCallBackMsg();
	//}

	
}



// 专门为两个从属thumb movepage使用
void CTripleLinkageOpt::LBtnUpForOtherThumb(const CPoint& point)
{
	if(m_bStartDraging)// begin to drag?
	{
		CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];

		m_bStartDraging = FALSE;
		//ReleaseCapture();

		int nPos = SearchThumbPosition(point);

		int nMin, nMax;
		GetThumbRange(m_nFocusThumb,  nMin, nMax);

		if (nPos >= nMax)
		{
			nPos = nMax;
		}
		if(nPos <= nMin)
		{
			nPos = nMin;
		}
		pThumb->SetPosition(nPos);

		MoveToPosition(m_nFocusThumb, nPos);
	}	
}

// 专门为主Thumb使用
void CTripleLinkageOpt::LBtnUpForMainThumb(const CPoint& point)
{
	if(m_bStartDraging)// begin to drag?
	{
		CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];
	
		int nPos = SearchThumbPosition(point);

		int nltGap, nrbGap;
		int nMin, nMax;

		int nltPos;
		int nrbPos;
		if(m_bHorizontal)
		{
			nltGap=  pThumb->GetPosition()-m_szThumb[0]->GetPosition();
			nrbGap = m_szThumb[2]->GetPosition() - pThumb->GetPosition();

			nMin =  nltGap;
			nMax = m_szTicMarks->size()-1 - nrbGap;

			nltPos = nPos-nltGap;
			nrbPos = nPos+nrbGap;

			//int nMin, nMax;
			//GetThumbRange(m_nFocusThumb,  nMin, nMax);
			if (nPos >= nMax)
			{
				nPos = nMax;
				nltPos = nPos-nltGap;
				nrbPos = nPos + nrbGap;
			}
			if(nPos <= nMin)
			{
				nPos = nMin;
				nltPos = nPos-nltGap;
				nrbPos = nPos + nrbGap;
			}


			pThumb->SetPosition(nPos);
			m_szThumb[0]->SetPosition(nltPos);
			m_szThumb[2]->SetPosition(nrbPos);

			MoveToPosition(0,nltPos);
			MoveToPosition(1,nPos);
			MoveToPosition(2,nrbPos);
		}
		else
		{
			nltGap =  m_szThumb[2]->GetPosition()-pThumb->GetPosition();
			nrbGap = pThumb->GetPosition()-m_szThumb[0]->GetPosition();

			nMin =  nrbGap;
			nMax = m_szTicMarks->size()-1 - nltGap;

			nltPos = nPos+nltGap;
			nrbPos = nPos-nrbGap;

			if (nPos >= nMax)
			{
				nPos = nMax;
			}
			if(nPos <= nMin)
			{
				nPos = nMin;

			}
			nltPos = nPos+nltGap;
			nrbPos = nPos - nrbGap;

			pThumb->SetPosition(nPos);
			m_szThumb[0]->SetPosition(nrbPos);
			m_szThumb[2]->SetPosition(nltPos);

			MoveToPosition(0,nrbPos);
			MoveToPosition(1,nPos);
			MoveToPosition(2,nltPos);
		}




 		m_bStartDraging = FALSE;
// 		//ReleaseCapture();
// 




	}	

}




//////////////////////////////////////////////////////////////////////////

void CTripleLinkageOpt::SetTicsArray(vector<int>& szTics)
{
	m_szTicMarks = &szTics;
	//	m_szTicMarks->clear();
	//	m_szTicMarks->insert(m_szTicMarks->end(), szTics.begin(), szTics.end());
	InitThumb(m_bHorizontal, m_szThumb[0]->GetWidth(), m_rcChannel);
}

int CTripleLinkageOpt::GetThumbPosition()
{
	return m_szThumb[m_nFocusThumb]->GetPosition();
}

void CTripleLinkageOpt::SetThumbPosition(int nIndex, int nPosition)
{
	if (nIndex < 3)
	{
		m_szThumb[nIndex]->SetPosition(nPosition);
		//m_szThumb[nIndex]->MoveThumb(nPosition);
		MoveToPosition(nIndex, nPosition);	
	}
	
//////////////////////////////////////////////////////////////////////////
//	MoveToPosition(nIndex, nPosition);	
	ReCalcChannelRect();
}

void CTripleLinkageOpt::GetThumbPosition(int& nFirstPos, int& nMidPos, int& nLastPos)
{
	nFirstPos = m_szThumb[0]->GetPosition();
	nMidPos = m_szThumb[1]->GetPosition();
	nLastPos = m_szThumb[2]->GetPosition();
}

int CTripleLinkageOpt::GetFocusThumb(const CPoint& point)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		if(m_szThumb[i]->IsPtOnThumb(point))
		{
			return i;
		}
	}
	return -1;
}


// 返回的都是可以到达的位置，而不是不能到达的位置（即重合位置）。
void CTripleLinkageOpt::GetThumbRange(int nIndex, int& nMin, int& nMax)
{			
	if (m_nFocusThumb == 0)
	{
		nMin = 0;
		nMax = m_szThumb[1]->GetPosition()-1; 
	}
	else if (m_nFocusThumb == m_szThumb.size()-1)
	{
		nMin = m_szThumb[m_nFocusThumb-1]->GetPosition()+1;
		nMax = m_szTicMarks->size()-1;
	}
	else
	{
		nMin = m_szThumb[m_nFocusThumb-1]->GetPosition()+1;
		nMax = m_szThumb[m_nFocusThumb+1]->GetPosition()-1;
	}
}

int CTripleLinkageOpt::GetAllThumbPosition(vector<int>& szThumbPos)
{
	szThumbPos.clear();
	for (UINT i = 0 ; i < m_szThumb.size(); i++)
	{
		int nPos = m_szThumb[i]->GetPosition();
		szThumbPos.push_back(nPos);
	}

	return szThumbPos.size();
}

void CTripleLinkageOpt::CalcThumbPosition()
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		SetThumbPosition(i, m_szThumb[i]->GetPosition());		
	}
}

void CTripleLinkageOpt::ReCalcChannelRect()
{
	CPoint ptFirst = m_szThumb[0]->GetPixPosition();
	CPoint ptMid = m_szThumb[1]->GetPixPosition();
	CPoint ptEnd = m_szThumb[2]->GetPixPosition();

	RC_CLR RCCool;
	RC_CLR RCHeat;
	RC_CLR RCGreen;

	RCCool.rc = RCHeat.rc = RCGreen.rc = m_rcChannel;
	
	RCCool.clr = CLR_COOLING;
	RCHeat.clr = CLR_HEATING;
	RCGreen.clr = CLR_BALANCE;
	
	if (m_bHorizontal)
	{
		RCCool.rc.right = ptFirst.x;
		RCHeat.rc.left = ptEnd.x;

		RCGreen.rc.left = ptFirst.x;
		RCGreen.rc.right = ptEnd.x;
	}
	else
	{
		RCCool.rc.bottom = ptFirst.y;
		RCHeat.rc.top = ptEnd.y;

		RCGreen.rc.top = ptFirst.y;
		RCGreen.rc.bottom = ptEnd.y;
	}
	RCCool.rc.DeflateRect(1,1,1,1);
	RCHeat.rc.DeflateRect(1,1,1,1);

	RCGreen.rc.DeflateRect(1,1,1,1);

	
	vector<RC_CLR> szRCs;
	szRCs.push_back(RCCool);
	szRCs.push_back(RCHeat);
	szRCs.push_back(RCGreen);
	
	((CFlexSlideWnd*)m_pParent)->GetFSBChannel()->SetRCArray(szRCs);

}