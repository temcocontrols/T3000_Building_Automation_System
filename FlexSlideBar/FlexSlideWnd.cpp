// FlexSlideWnd.cpp : implementation file
//

#include "stdafx.h"
#include "FlexSlideWnd.h"
#include "SingleThumbOpt.h"
#include "MultiThumbOpt.h"
#include "TripleThumbOpt.h"
#include "DoubleLinkageOpt.h"
#include "TripleLinkageOpt.h"
#include "CalcTics.h"
#include "FSBContainer.h"
//#include "../../TestFSB/TestFSBDlg.h"

//#include "../T3000/global_variable.h "



// CFlexSlideWnd

IMPLEMENT_DYNAMIC(CFlexSlideWnd, CStatic)
CFlexSlideWnd::CFlexSlideWnd()
{
	m_pThumbOpter = NULL;
	m_pParent = NULL;
		
	m_fsbStyle = FSB_STYLE_SINGLETHUMB;
	m_fsbTicSide = FSB_TICSIDE_LEFT;
	m_nMax=20;
	m_nMin=0;
	m_nPageLength = 10;
	m_pTicMarkLock = new CRITICAL_SECTION;
	::InitializeCriticalSection(m_pTicMarkLock);

}

CFlexSlideWnd::~CFlexSlideWnd()
{
	m_pParent = NULL;
	if (m_pThumbOpter)
	{
		delete m_pThumbOpter;
	}

	::DeleteCriticalSection(m_pTicMarkLock);
	delete m_pTicMarkLock;
}


BEGIN_MESSAGE_MAP(CFlexSlideWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
//	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CFlexSlideWnd::SetParentWnd(CWnd* pParent, CFSBContainer* pContainer)
{
	ASSERT(pParent);
	ASSERT(pContainer);
	m_pParent = pParent;
	m_pContainer = pContainer;
}

void CFlexSlideWnd::SetFSBStyle(FSB_STYLE fsbStyle)
{
	m_fsbStyle = fsbStyle;
	
	if (!m_pThumbOpter)
	{
		switch(fsbStyle)
		{
		case FSB_STYLE_SINGLETHUMB:
 		//	m_pThumbOpter = new CSingleThumbOpt;//因ThumbOperator是抽象类，但在CSingleThumbOpt有一个函数还没实现，	virtual int GetThumbNum()=0;
			//所以CSingleThumbOpt也不能实例化
 		//	m_pThumbOpter->SetParentWnd(this);
			break;
		case FSB_STYLE_DOUBLETHUMB:
			m_pThumbOpter = new CMultiThumbOpt;
			m_pThumbOpter->SetParentWnd(this);
			((CMultiThumbOpt*)m_pThumbOpter)->SetThumbNum(2);
			break;
		case FSB_STYLE_TRIPLELINKAGE:
			m_pThumbOpter = new CTripleLinkageOpt;
			m_pThumbOpter->SetParentWnd(this);
			break;
		   //FSB_STYLE_TRIPLETHUMB
//  		case FSB_STYLE_TRIPLETHUMB://lsc
//  			m_pThumbOpter = new CTripleThumbOpt;
//  			m_pThumbOpter->SetParentWnd(this);
//  			break;
			
		default:
			break;
		}
	}
	m_pThumbOpter->SetFSBStyle(fsbStyle);
}

CFSBChannel* CFlexSlideWnd::GetFSBChannel()
{
	return &m_fsbChannel;
}

FSB_STYLE CFlexSlideWnd::GetFSBStyle()
{
	return m_fsbStyle;
}

void CFlexSlideWnd::SetFSBThumbShape(FSB_THUMBSHAPE fsbTbShape)
{
	//m_fsbTbShape = fsbTbShape;
	m_pThumbOpter->SetThumbShape(fsbTbShape);
}

// 设置控件的位置
void CFlexSlideWnd::SetFlexSlideBarRect(CRect& rc)
{
//	Create(_T(""), SS_SUNKEN, rc, NULL, 0);
//	MoveWindow(&rc);
	m_rc = rc;
}

// 设置channel的宽度，不得大于控件宽度，不小于1个象素
// channel 长度应该依据控件长度来计算获得
// 这里的宽度是指的较短的边长，相对于垂直来说，是width，对于水平来说，是height
void CFlexSlideWnd::SetChannelWidth(int nChannelWidth)
{
	m_fsbChannel.SetWidth(nChannelWidth);

	GetWindowRect(&m_rc);
	ScreenToClient(&m_rc);
	
	int nHeight = m_rc.Height()-10;
	m_fsbChannel.SetHeight(nHeight);		
	
	CRect rcChannel = m_rc;
	int nTemp = (m_rc.Width()-nChannelWidth)/2;
	rcChannel.DeflateRect(nTemp,10,nTemp,10);

	m_fsbChannel.SetRect(rcChannel);	
	ASSERT(m_pThumbOpter);
	m_pThumbOpter->SetChannelRect(rcChannel);

}

int CFlexSlideWnd::GetChannelWidth()
{
	return m_fsbChannel.GetWidth();
}

// 设置Channel的颜色
COLORREF CFlexSlideWnd::GetChannelColor()
{
	return m_fsbChannel.GetColor();	
}


void CFlexSlideWnd::SetChannelColor(COLORREF clrChannel)
{
	m_fsbChannel.SetColor(clrChannel);
}


int CFlexSlideWnd::GetThumbWidth()
{
	return m_pThumbOpter->GetThumbWidth();
}

void CFlexSlideWnd::SetThumbWidth(int nThumbWidth)
{
	m_pThumbOpter->SetThumbWidth(nThumbWidth);
}


// 设置Thumb的颜色
COLORREF CFlexSlideWnd::GetThumbColor()const		
{
	return m_pThumbOpter->GetThumbColor();
}


void CFlexSlideWnd::SetThumbColor(COLORREF clrThumb)
{
	m_pThumbOpter->SetThumbColor(clrThumb);
}

void CFlexSlideWnd::SetRange(int nMax, int nMin)
{
	//ASSERT(nMin <= nMax);
	if (nMin > nMax)
	{
		nMin = nMax;
	}

	if (m_pThumbOpter)
	{
		int nThumbNum = m_pThumbOpter->GetThumbNum();
		if (nMax - nMin < nThumbNum)
		{
			nMax = nMin + nThumbNum;
		}	
	}

	m_nMax = nMax;
	m_nMin = nMin;
	::EnterCriticalSection(m_pTicMarkLock);
	CalcTicMarks(m_fsbChannel.GetRect());
	m_pThumbOpter->SetTicsArray(m_szTics);
	m_pThumbOpter->CalcThumbPosition();

	::EnterCriticalSection(m_pTicMarkLock);
}

void CFlexSlideWnd::GetRange(int& nMin, int& nMax)const
{
	nMin = m_nMin;
	nMax = m_nMax;
}


void	CFlexSlideWnd::SetRangeMax(int nMax)
{
	m_nMax = nMax;
	CalcTicMarks(m_fsbChannel.GetRect());
	m_pThumbOpter->SetTicsArray(m_szTics);
}

void	CFlexSlideWnd::SetRangeMin(int nMin)
{
	m_nMin = nMin;
	CalcTicMarks(m_fsbChannel.GetRect());
	m_pThumbOpter->SetTicsArray(m_szTics);
}

void CFlexSlideWnd::SetHorizontal(BOOL bHorizontal)
{
	m_bHorizontal = bHorizontal;	
}


BOOL CFlexSlideWnd::IsHorizontal()
{
	return m_bHorizontal;
}

void CFlexSlideWnd::SetPageLength(int nPageLength)
{
	m_nPageLength = nPageLength;
	//CalcPixelNumOfTicLength();
	ASSERT(m_pThumbOpter);
	//m_pThumbOpter->SetLengthOfPage(m_nTicLength*nTicNum);
	m_pThumbOpter->SetLengthOfPage(m_nPageLength);
	m_pThumbOpter->SetLengthOfTic(m_nTicLength);


}

void CFlexSlideWnd::CalcTicMarks(CRect& rcChannel)
{
	CCalcTics* pCalTic = NULL;
	if (m_bHorizontal)
	{
		pCalTic = new CCalcTics(rcChannel.Width(),abs(m_nMax - m_nMin));
		m_szTics.clear();
		int nRet = pCalTic->GetTicArray(m_szTics);
		ASSERT(nRet == abs(m_nMax - m_nMin)+1);
		delete pCalTic;
	}
	else
	{
		pCalTic = new CCalcTics(rcChannel.Height(), abs(m_nMax - m_nMin));
		m_szTics.clear();
		int nRet = pCalTic->GetTicArray(m_szTics);
		ASSERT(nRet == abs(m_nMax - m_nMin)+1);
		delete pCalTic;
	}

}


// 创建flex slidebar
BOOL CFlexSlideWnd::CreateFlexSlideBar(
									   BOOL bHorizontal, 
									   FSB_STYLE style,
									   int nChannelWidth, 
									   int nThumbWidth, 				
									   FSB_THUMBSHAPE tbShape									  
									   )
{	
	SetFSBStyle(style);
	SetHorizontal(bHorizontal);
	SetChannelWidth(nChannelWidth);

	SetFSBThumbShape(tbShape);
	//m_pThumbManager->SetChannelRect(m_fsbChannel.GetRect());
//	SetThumbWidth(nThumbWidth);
	if (m_pThumbOpter)
	{
		int nThumbNum = m_pThumbOpter->GetThumbNum();
		if (m_nMax - m_nMin < nThumbNum)
		{
			m_nMax = m_nMin + nThumbNum;
		}	
	}

	CRect rcChannel = m_fsbChannel.GetRect();
	CalcTicMarks(rcChannel);
//	InitTicMarks(rcChannel);
	m_pThumbOpter->SetTicsArray(m_szTics);
	return m_pThumbOpter->InitThumb(bHorizontal, nThumbWidth, rcChannel);
	
	//return TRUE;
}


void CFlexSlideWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CStatic::OnPaint() for painting messages
	
	
//	dc.SetMapMode(MM_LOMETRIC );
	
	CPoint ptW = dc.GetWindowOrg();
	CPoint ptV = dc.GetViewportOrg();



	CRect rc;
 	GetClientRect(&rc);	
 	// SetViewportOrgEx(dc.m_hDC, rc.left, rc.bottom, &ptV);
	//SetWindowOrgEx(dc.m_hDC, rc.left, rc.bottom, &ptW);

	CPoint test(100,500);
	dc.MoveTo(0,0);
	//dc.DPtoLP(&test);
//	dc.LineTo(test);

	Draw(&dc);

}

void CFlexSlideWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	 

	m_pThumbOpter->OnLButtonDown(point);
	//MoveStep(point);
	Invalidate(TRUE);
	//CStatic::OnLButtonDown(nFlags, point);
}



void CFlexSlideWnd::MovePage(const CPoint& pt)
{
	m_pThumbOpter->MovePage(pt);
	Invalidate(TRUE);
}

void CFlexSlideWnd::MoveStep(const CPoint& pt)
{
	//m_pThumbOpter->MoveStep(pt);
	Invalidate(TRUE);
}

int CFlexSlideWnd::CalcPixelNumOfTicLength()
{
// 	CRect rc = m_fsbChannel.GetRect();
// 	if (m_bHorizontal)
// 	{
// 		return m_nTicLength=rc.Width()/(m_nMax-m_nMin);
// 	}
// 	else
// 	{
// 		return m_nTicLength=rc.Height()/(m_nMax-m_nMin);
// 	}
 	return 1;
}
// BOOL CFlexSlideWnd::OnEraseBkgnd(CDC* pDC)
// {
// 	 
// 	
// 	return CStatic::OnEraseBkgnd(pDC);
// }

void CFlexSlideWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	 
	if (nFlags)
	{
	    
		m_pThumbOpter->OnMouseMove(point);
		//Sleep(500);
		Invalidate(TRUE);
	}
	
	
	//CStatic::OnMouseMove(nFlags, point);
}

void CFlexSlideWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	 
	m_pThumbOpter->OnLButtonUp(point);
	Invalidate(TRUE);

//	((CTestFSBDlg*)m_pParent)->CallBackForFlexSlide();
//	((CTestFSBDlg*)m_pParent)->OnBnClickedButtonTest();

	//m_pParent->SendMessage(WM_USER_CALLBACK, WPARAM(this),0)	;
//	CStatic::OnLButtonUp(nFlags, point);
}

void CFlexSlideWnd::SendCallBackMsg()
{
	//m_pParent->SendMessage(WM_USER_CALLBACK, WPARAM(this),0)	;
	m_pContainer->SendCallBackMsg();
}

void CFlexSlideWnd::Draw(CDC* pDC)
{
	//	pDC = GetDC();
	CRect rc;
	GetWindowRect(rc);
	rc.DeflateRect(1,1,1,1);
	
	HBITMAP bmp = CreateCompatibleBitmap(pDC->m_hDC, rc.Width(), rc.Height());

	CDC* pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);
	HGDIOBJ pOldBmp = pMemDC->SelectObject(bmp);
	
	pMemDC->FillSolidRect(m_rc, GetSysColor(COLOR_3DFACE));

	DrawTics(pMemDC);
	m_fsbChannel.Draw(pMemDC);
	m_pThumbOpter->Draw(pMemDC);

	::BitBlt(pDC->m_hDC, 0,0, rc.Width(), rc.Height(), pMemDC->m_hDC, 0,0, SRCCOPY);//没这，就不会有新的控件显示出来

	

	pMemDC->SelectObject(pOldBmp);
	delete pMemDC;

	//pDC->LineTo(100,100);
}

const int TICMARK_LENGTH = 15;
const int TICMARK_LENGTH_MID = 4;
const int TICMARK_DIST = 0;

void CFlexSlideWnd::DrawTics(CDC* pDC)
{	
	//CPen pen(PS_SOLID, 1,RGB(0, 0, 0));
		CPen pen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	HGDIOBJ  pOldPen = pDC->SelectObject(pen);
//GetSysColor(COLOR_3DSHADOW)
	CRect rc = m_fsbChannel.GetRect();

// 	pDC->MoveTo(rc.TopLeft());
// 	pDC->LineTo(rc.TopLeft().x - 10, rc.TopLeft().y);


	if (m_bHorizontal)
	{		
		CPoint ptStart, ptEnd,ptStart2, ptEnd2;
		ptStart.x = rc.left;
		ptStart.y = rc.top - TICMARK_DIST;
		ptEnd.x = ptStart.x;
		ptEnd.y = ptStart.y - TICMARK_LENGTH;

		ptStart2.x = rc.left;
		ptStart2.y = rc.bottom + TICMARK_DIST;
		ptEnd2.x = ptStart2.x;
		ptEnd2.y = ptStart2.y + TICMARK_LENGTH;
		 
		for (int i = 0; i <= abs(m_nMax-m_nMin); i++)
		{		 
			int nX = ptStart.x + m_szTics[i];

 			pDC->MoveTo(nX, ptStart.y);
 			pDC->LineTo(nX,ptEnd.y);

			//////////////////////////////////////////////////////////////////////////
			int nX2 = ptStart2.x + m_szTics[i];
			

 			pDC->MoveTo(nX2, ptStart2.y);
 			pDC->LineTo(nX2, ptEnd2.y);
		}
	


// 		int nWidth = rc.Width();	
// 		int nGap = nWidth/(m_nMax-m_nMin);
// 
// 		CPoint ptStart, ptEnd;
// 		ptStart.x = rc.left;
// 		ptStart.y = rc.top - TICMARK_DIST;
// 		ptEnd.x = ptStart.x;
// 		ptEnd.y = ptStart.y - TICMARK_LENGTH;
// 
// 
// 		for(int i = 0; i <= (m_nMax-m_nMin); i++)
// 		{	
// 			pDC->MoveTo(ptStart);
// 			pDC->LineTo(ptEnd);
// 
// 			ptStart.x += nGap;
// 			ptEnd.x = ptStart.x;		
// 
// 		}
// 
// 		ptStart.x = rc.left;
// 		ptStart.y = rc.bottom + TICMARK_DIST;
// 		ptEnd.x = ptStart.x;
// 		ptEnd.y = ptStart.y + TICMARK_LENGTH;
// 		for(int i = 0; i <= (m_nMax-m_nMin); i++)
// 		{	
// 			pDC->MoveTo(ptStart);
// 			pDC->LineTo(ptEnd);
// 
// 			ptStart.x += nGap;
// 			ptEnd.x = ptStart.x;
// 
// 		}
	}
	else
	{
		CPoint ptStart, ptEnd, ptStart2, ptEnd2;
		CPoint ptStart_mid, ptEnd_mid, ptStart2_mid, ptEnd2_mid;
		ptStart.y = rc.top;
		ptStart.x = rc.left - TICMARK_DIST;
		ptEnd.y = ptStart.y;
		ptEnd.x = ptStart.x - TICMARK_LENGTH;

		ptStart_mid.y = rc.top;
		ptStart_mid.x = rc.left - TICMARK_DIST;
		ptEnd_mid.y = ptStart_mid.y;
		ptEnd_mid.x = ptStart_mid.x - TICMARK_LENGTH_MID;


		ptStart2.x = rc.right + TICMARK_DIST;
		ptStart2.y = rc.top;
		ptEnd2.x = ptStart2.x + TICMARK_LENGTH;
		ptEnd2.y = ptStart2.y;

		ptStart2_mid.x = rc.right + TICMARK_DIST;
		ptStart2_mid.y = rc.top;
		ptEnd2_mid.x = ptStart2_mid.x + TICMARK_LENGTH_MID;
		ptEnd2_mid.y = ptStart2_mid.y;

 
		for (int i = 0; i <= abs(m_nMax-m_nMin); i++)
		{	
			if (i%10==0)
			{
				int nY = ptStart.y + m_szTics[i];


				pDC->MoveTo(ptStart.x, nY);//2.5.0.95显示在滑块两边的刻度条
				pDC->LineTo(ptEnd.x, nY);//2.5.0.95


				//////////////////////////////////////////////////////////////////////////
				int nY2 = ptStart2.y + m_szTics[i];

				pDC->MoveTo(ptStart2.x, nY2);//2.5.0.95
				pDC->LineTo(ptEnd2.x, nY2);//2.5.0.95
			}
 			else if (i%10==5)
 			{
 
 				int nY = ptStart_mid.y + m_szTics[i];
 
 
 				pDC->MoveTo(ptStart_mid.x, nY);//2.5.0.95显示在滑块两边的刻度条
 				pDC->LineTo(ptEnd_mid.x, nY);//2.5.0.95
 
 
 				//////////////////////////////////////////////////////////////////////////
 				int nY2 = ptStart2_mid.y + m_szTics[i];
 
 				pDC->MoveTo(ptStart2_mid.x, nY2);//2.5.0.95
 				pDC->LineTo(ptEnd2_mid.x, nY2);//2.5.0.95
 
 
 			}
			
			
		}

//  		int nHeight= rc.Height();	
//  		int nGap = nHeight/(m_nMax-m_nMin);
//  
//  		//CPoint ptStart, ptEnd;
//  		ptStart.x = rc.left - TICMARK_DIST;
//  		ptStart.y = rc.top;
//  		ptEnd.x = ptStart.x - TICMARK_LENGTH;
//  		ptEnd.y = ptStart.y;
//  		for(int i = 0; i <= (m_nMax-m_nMin); i++)
// 		{	
// 			pDC->MoveTo(ptStart);
// 			pDC->LineTo(ptEnd);
// 			ptStart.y += nGap;
// 			ptEnd.y = ptStart.y;	
// 
//  		}
 
//  		ptStart.x = rc.left + TICMARK_DIST;
//  		ptStart.y = rc.top;
//  		ptEnd.x = ptStart.x + TICMARK_LENGTH;
//  		ptEnd.y = ptStart.y;
//  		for(int i = 0; i <= (m_nMax-m_nMin); i++)
//  		{	
//  			pDC->MoveTo(ptStart);
//  			pDC->LineTo(ptEnd);
//  			ptStart.y += nGap;
//  			ptEnd.y = ptStart.y;
// 		}
	}

	pDC->SelectObject(pOldPen);
	
}




BOOL CFlexSlideWnd::SetPos(int nMinPos, int nMidPos, int nMaxPos)
{
// 	ASSERT(nMinPos < nMidPos && nMidPos < nMaxPos && nMinPos >= 0);//tstat6
// 	ASSERT(m_pThumbOpter);
// 
// 	ASSERT(nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);
 
	if (!((nMinPos < nMidPos) && (nMidPos < nMaxPos)))//tstat6
	{
		//CString strTips = _T("Set Point can't be set properly. Please confirm your set point value!");
		//AfxMessageBox(strTips);
		return FALSE;
		//m_pThumbOpter->SetThumbPosition(0, nMinPos-m_nMin);
		//m_pThumbOpter->SetThumbPosition(1, nMidPos-m_nMin);
		//m_pThumbOpter->SetThumbPosition(2, nMaxPos-m_nMin);
		//return;
	}
	m_pThumbOpter->SetThumbPosition(0, nMinPos-m_nMin);
	m_pThumbOpter->SetThumbPosition(1, nMidPos-m_nMin);
	m_pThumbOpter->SetThumbPosition(2, nMaxPos-m_nMin);
	 Invalidate(TRUE);
	return TRUE;
}

// Retrieves the current position of the slider. 
int	CFlexSlideWnd::GetPos(vector<int>& szPos)
{
	ASSERT(m_pThumbOpter);
	return m_pThumbOpter->GetAllThumbPosition(szPos);		
}

BOOL CFlexSlideWnd::SetPos_tstat6_2pos( int nMinPos, int nMidPos, int nMaxPos )
{
	if (nMinPos<m_nMin)
	{
		nMinPos=m_nMin;
	}
	if (nMaxPos>m_nMax)
	{
		nMaxPos=m_nMax;
	}
	ASSERT(nMinPos <= nMidPos && nMidPos <= nMaxPos );//tstat6
	ASSERT(m_pThumbOpter);

	ASSERT(nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);

	if (!(nMinPos <= nMidPos && nMidPos <= nMaxPos  ))//tstat6
	{
		//CString strTips = _T("Set Point can't be set properly. Please confirm your set point value!");
		//AfxMessageBox(strTips);
		return FALSE;

	}
	m_pThumbOpter->SetPosValue(nMinPos,nMidPos,nMaxPos);
	m_pThumbOpter->SetThumbPosition(0, nMinPos-m_nMin);
 	//m_pThumbOpter->SetThumbPosition(1, nMidPos-m_nMin);
//	m_pThumbOpter->SetThumbPosition(2, nMaxPos-m_nMin);
	m_pThumbOpter->SetThumbPosition(1, nMaxPos-m_nMin);
	Invalidate(TRUE);
	return TRUE;
}

BOOL CFlexSlideWnd::SetPos_tstat6_3pos( int nMinPos, int nMidPos, int nMaxPos )
{
	ASSERT(nMinPos <= nMidPos && nMidPos <= nMaxPos);//tstat6
	ASSERT(m_pThumbOpter);

	ASSERT(nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);

	if (!(nMinPos <= nMidPos && nMidPos <= nMaxPos))//tstat6
	{
		return FALSE;

	}
	m_pThumbOpter->SetThumbPosition(0, nMinPos-m_nMin);
	m_pThumbOpter->SetThumbPosition(1, nMidPos-m_nMin);
	m_pThumbOpter->SetThumbPosition(2, nMaxPos-m_nMin);
	Invalidate(TRUE);
	//UpdateData(F)
	return TRUE;
}