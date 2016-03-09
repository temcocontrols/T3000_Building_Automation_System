#include "StdAfx.h"
#include "MultiThumbOpt.h"
#include "FSBThumb.h"
#include "FlexSlideWnd.h"

CMultiThumbOpt::CMultiThumbOpt(void)
{
	m_bStartDraging = FALSE;
	m_nFocusThumb = -1;
}

CMultiThumbOpt::~CMultiThumbOpt(void)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		delete m_szThumb[i];
	}
}


// 设置Thumb的个数
void CMultiThumbOpt::SetThumbNum(int nThumbNum)
{
	ASSERT( nThumbNum > 1);
	for (int i = 0; i < nThumbNum; i++)
	{
		CFSBThumb* pThumb = new CFSBThumb;
		m_szThumb.push_back(pThumb);
	}
}


void CMultiThumbOpt::SetThumbShape(FSB_THUMBSHAPE tbShape)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->SetShape(tbShape);		
	}
	
}

int CMultiThumbOpt::GetThumbNum()
{
	return m_szThumb.size();	
}

void CMultiThumbOpt::SetThumbWidth(int nWidth)
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->SetWidth(nWidth);
	}
}



BOOL CMultiThumbOpt::InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)
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
		nPos = i ;
		if (m_bHorizontal)
		{
			ptCenter.x = m_szTicMarks->at(nPos)+m_rcChannel.left;
		}
		else
		{
			ptCenter.y = m_szTicMarks->at(nPos)+m_rcChannel.top;
		}

		m_szThumb[i]->InitThumb(ptCenter, nPos, THUNMB_RECT_HEIGHT, nThumbWidth, m_bHorizontal);
       // m_szThumb[i]->SetMixValue(m_nM)	
	}
	m_nFocusThumb = 0;

	ReCalcChannelRect();
	return TRUE;
}

int CMultiThumbOpt::GetThumbWidth()
{
	ASSERT(m_szThumb.size() > 0);
	return m_szThumb[0]->GetWidth();
}

COLORREF CMultiThumbOpt::GetThumbColor()
{
	ASSERT(m_szThumb.size() > 0);
	return m_szThumb[0]->GetColor();
}

void CMultiThumbOpt::SetThumbColor(COLORREF clr)
{
	for (UINT i = 0 ; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->SetColor(clr);
	}

	//m_szThumb[0]->SetColor(RGB(0,0,128));  //改变下面那个滑块的颜色
	m_szThumb[0]->SetColor(RGB(255,0,0)); 
}

void CMultiThumbOpt::Draw(CDC* pDC)
{
	for (UINT i = 0 ; i < m_szThumb.size(); i++)
	{
		m_szThumb[i]->Draw(pDC);
	}
}


void CMultiThumbOpt::MovePage(const CPoint& pt)
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
		if (pt.y > ptPos.y)  // move down, 向下
		{
			nNewPosition = nPosition-m_nLengthOfPage; // 向下是减 

			if(nNewPosition <= nMin)
			{				
				nNewPosition = nMin;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}
			else
			{
				//nNewPosition = nMax;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}

		}
		else // move up
		{
			nNewPosition = nPosition + m_nLengthOfPage; // up +
			if(nNewPosition >= nMax)
			{				
				nNewPosition = nMax;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}
			else
			{
				//nNewPosition = nMin;
				ptNewPS.y = m_szTicMarks->at(nNewPosition)+m_rcChannel.top;
			}		
		}
	}

	pThumb->SetPosition(nNewPosition);
	pThumb->MoveThumb(ptNewPS);



};

void CMultiThumbOpt::OnLButtonDown(const CPoint& point)
{
	int nRet  = GetFocusThumb(point);

	if(nRet != -1) // begin to drag?
	{	
		m_nFocusThumb = nRet;
		
		m_bStartDraging = TRUE;
		//HWND hwnd = SetCapture(AfxGetApp()->GetMainWnd()->);
	}
	else
	{
// 		MovePage(point);
// 		ReCalcChannelRect();
	}
}


// 下一步，大的数值
void CMultiThumbOpt::MoveNextStep()
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
void CMultiThumbOpt::MovePreStep()
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

void CMultiThumbOpt::MoveToPosition(int nIndex, int nPosition)
{
	ASSERT(nIndex < (int)m_szThumb.size());
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


void CMultiThumbOpt::OnMouseMove(const CPoint& point)
{
	

	if(m_bStartDraging) // begin to drag?
	{    
         CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];
	      ////Check DB
         int nPos = SearchThumbPosition(point);
        int nMax, nMin;
         GetThumbRange(pThumb->GetPosition(), nMin, nMax);


 		CPoint ptPos = pThumb->GetPixPosition();
 		CPoint ptNewPS = ptPos;
// 

		
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
			nPixMax+=m_rcChannel.top;
			nPixMin+=m_rcChannel.top;

			int nTemp = nPixMin;
			nPixMin = nPixMax;
			nPixMax = nTemp;     // 交换一下，

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
                 //if (nPos >= nMax)
                 //{
                 //    nPos = nMax;
                 //}
                 //if(nPos <= nMin)
                 //{
                 //    nPos = nMin;
                 //}
                 //if (m_nFocusThumb == 0)
                 //{
                 //    if (nPos>m_nMidPos)
                 //    {
                 //        return;
                 //    }
                 //}
                 //if (m_nFocusThumb == 1)
                 //{
                 //    if (nPos<m_nMidPos)
                 //    {
                 //        return;
                 //    }
                 //}
		pThumb->MoveThumb(ptNewPS);

		ReCalcChannelRect();
	}
}

int CMultiThumbOpt::SearchThumbPosition(const CPoint& pt)
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

void CMultiThumbOpt::OnLButtonUp(const CPoint& point)
{
	if(m_bStartDraging)// begin to drag?
	{
		CFSBThumb* pThumb = m_szThumb[m_nFocusThumb];
		m_bStartDraging = FALSE;
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

		ReCalcChannelRect();


		((CFlexSlideWnd*)m_pParent)->SendCallBackMsg();
	}
	
}




//////////////////////////////////////////////////////////////////////////

void CMultiThumbOpt::SetTicsArray(vector<int>& szTics)
{
	m_szTicMarks = &szTics;
	//	m_szTicMarks->clear();
	//	m_szTicMarks->insert(m_szTicMarks->end(), szTics.begin(), szTics.end());

// 	for (UINT i = 0; i < m_szThumb.size(); i++)
// 	{
// 		SetThumbPosition(i, m_szThumb[i]->GetPosition());		
// 	}

	
}

void CMultiThumbOpt::CalcThumbPosition()
{
	for (UINT i = 0; i < m_szThumb.size(); i++)
	{
		SetThumbPosition(i, m_szThumb[i]->GetPosition());		
	}
}


int CMultiThumbOpt::GetThumbPosition()
{
	return m_szThumb[m_nFocusThumb]->GetPosition();
}

void CMultiThumbOpt::SetThumbPosition(int nIndex, int nPosition)
{	
	if (nPosition >= (int)m_szTicMarks->size())   // 超过刻度那么裁减掉
	{
		nPosition = m_szTicMarks->size()-1;
	}

	if (nIndex < (int)m_szThumb.size())
	{
		MoveToPosition(nIndex, nPosition);
		ReCalcChannelRect();
	}	
}

int CMultiThumbOpt::GetFocusThumb(const CPoint& point)
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
void CMultiThumbOpt::GetThumbRange(int nIndex, int& nMin, int& nMax)
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

int CMultiThumbOpt::GetAllThumbPosition(vector<int>& szThumbPos)
{
	szThumbPos.clear();
	for (UINT i = 0 ; i < m_szThumb.size(); i++)
	{
		int nPos = m_szThumb[i]->GetPosition();
		szThumbPos.push_back(nPos);
	}

	return szThumbPos.size();
}

	
void CMultiThumbOpt::ReCalcChannelRect()
{
	/*
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
	*/
	//////////////////////////////////////////////////////////////////////////

	DWORD szClr[3] = {CLR_COOLING, CLR_BALANCE, CLR_HEATING};
	DWORD szClr2[3] = {CLR_HEATING, CLR_BALANCE, CLR_COOLING};

	vector<RC_CLR> szRCs;

	CPoint ptS, ptE;
	for (UINT i = 0 ; i <= m_szThumb.size(); i++)
	{
		if (i == 0)
		{
			if (m_bHorizontal)
			{
				ptS = m_rcChannel.TopLeft();
			}
			else
			{
				ptS = m_rcChannel.BottomRight();
			}
			
			ptE = m_szThumb[i]->GetPixPosition();
		}
		else if (i == m_szThumb.size())
		{
			ptS = m_szThumb[i-1]->GetPixPosition();
			if (m_bHorizontal)
			{
				ptE = m_rcChannel.BottomRight();
			}
			else
			{
				ptE = m_rcChannel.TopLeft();
			}
			
		}
		else
		{
			ptS = m_szThumb[i-1]->GetPixPosition();
			ptE = m_szThumb[i]->GetPixPosition();
		}

		RC_CLR RCC;
		RCC.rc = m_rcChannel;	
		if (m_bHorizontal)
		{	
			RCC.rc.right = ptS.x;
			RCC.rc.left = ptE.x;
		}
		else
		{
			RCC.rc.bottom = ptS.y;
			RCC.rc.top = ptE.y;
		}

		if (m_bHorizontal)
		{
			RCC.clr = szClr[i%3];
			RCC.rc.DeflateRect(1,-1,1,-1);
		}
		else
		{
			RCC.clr = szClr2[i%3];
			RCC.rc.DeflateRect(1,-1,1,-1);
		}


		szRCs.push_back(RCC);
	}

	CFSBChannel* pc = ((CFlexSlideWnd*)m_pParent)->GetFSBChannel();
	((CFlexSlideWnd*)m_pParent)->GetFSBChannel()->SetRCArray(szRCs);

}
