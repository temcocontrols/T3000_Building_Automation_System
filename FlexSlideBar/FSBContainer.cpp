#include "StdAfx.h"
#include "FSBContainer.h"

CFSBContainer::CFSBContainer(void)
{
	m_pFSWnd = new CFlexSlideWnd;

    
}


CFSBContainer::~CFSBContainer(void)
{
	if( m_pFSWnd )
	{
		delete m_pFSWnd;
		m_pFSWnd = NULL;
	}
}


void CFSBContainer::SetFSBStyle(FSB_STYLE fsbStyle)
{
	m_pFSWnd->SetFSBStyle(fsbStyle);
}

FSB_STYLE CFSBContainer::GetFSBStyle()
{
	return m_pFSWnd->GetFSBStyle();
}

void CFSBContainer::SetFSBThumbShape(FSB_THUMBSHAPE fsbTbShape)
{
	m_pFSWnd->SetFSBThumbShape(fsbTbShape);
}

// 设置控件的位置
void CFSBContainer::SetFlexSlideBarRect(CRect& rc)
{
	m_pFSWnd->SetFlexSlideBarRect(rc);
}


// CRect& CFSBContainer::GetFlexSlideBarRect()
// {
// 	return m_pFSWnd->GetFlexSlideBarRect();
// }

// 设置channel的宽度，不得大于控件宽度，不小于1个象素
// channel 长度应该依据控件长度来计算获得
void CFSBContainer::SetChannelWidth(int nChannelWidth)
{
	m_pFSWnd->SetChannelWidth(nChannelWidth);
}


int CFSBContainer::GetChannelWidth()
{
	return m_pFSWnd->GetChannelWidth();
}

// 设置Channel的颜色
COLORREF CFSBContainer::GetChannelColor()
{
	return m_pFSWnd->GetChannelColor();
}

void CFSBContainer::SetChannelColor(COLORREF clrChannel)
{
	m_pFSWnd->SetChannelColor(clrChannel);
}

// 设置Thumb的长度，不小于channel的宽度，不大于控件的宽度，thumb的宽度为固定值
int CFSBContainer::GetThumbWidth()
{
	return m_pFSWnd->GetThumbWidth();
}

void CFSBContainer::SetThumbWidth(int nThumbLen)
{
	m_pFSWnd->SetThumbWidth(nThumbLen);
}

// 设置Thumb的颜色
COLORREF CFSBContainer::GetThumbColor()const
{
	return m_pFSWnd->GetThumbColor();
}

void CFSBContainer::SetThumbColor(COLORREF clrThumb)
{
	m_pFSWnd->SetThumbColor(clrThumb);
}

//Sets the maximum position for a slider. 
void	CFSBContainer::SetRangeMax(int nMax)
{
	m_pFSWnd->SetRangeMax(nMax);
}


void	CFSBContainer::SetRangeMin(int nMin)
{
	m_pFSWnd->SetRangeMin(nMin);
}

// 控件是否水平放置
void CFSBContainer::SetHorizontal(BOOL bHorizontal)
{
	m_pFSWnd->SetHorizontal(bHorizontal);
}


BOOL CFSBContainer::IsHorizontal()
{
	return m_pFSWnd->IsHorizontal();
}


void CFSBContainer::SetPageLength(int nPageLength)
{
	m_pFSWnd->SetPageLength(nPageLength);
}

BOOL CFSBContainer::CreateFlexSlideBar(
						BOOL bHorizontal,	
						FSB_STYLE style, 
						int nChannelWidth, 
						int nThumbWidth, 
						FSB_THUMBSHAPE tbShape
						)
{
	return m_pFSWnd->CreateFlexSlideBar(bHorizontal,	style,	nChannelWidth, nThumbWidth, tbShape );
} 
void CFSBContainer::SetRange(int nMax, int nMin)
{
	m_pFSWnd->SetRange(nMax, nMin);
}

//Retrieves the minimum and maximum positions for a slider. 
void CFSBContainer::GetRange(int& nMin, int& nMax)const
{
	m_pFSWnd->GetRange(nMin, nMax);
}




// Retrieves the current position of the slider. 
int	CFSBContainer::GetPos(vector<int>& szPos)
{
	return m_pFSWnd->GetPos(szPos);
}


void CFSBContainer::SetParentWnd(CWnd* pParent)
{
	m_pFSWnd->SetParentWnd(pParent, this);
}

void CFSBContainer::SendCallBackMsg()
{
	//::SendMessage(m_pFSWnd->GetParent()->m_hWnd, WM_USER_CALLBACK, WPARAM(this),0)	;
	::SendMessage(m_pFSWnd->GetParent()->m_hWnd, WM_USER_CALLBACK, WPARAM(m_sort),0)	;//m_hWnd
}

BOOL CFSBContainer::CreateInstance(CWnd* pParentWnd, DWORD dwStyle, CRect rc)
{
	ASSERT(m_pFSWnd);
	//return m_pFSWnd->Create(_T("FSB"), SS_SUNKEN, rc, pParentWnd);
	m_pFSWnd->SubclassWindow(pParentWnd->GetSafeHwnd());
	return TRUE;
// 	return m_pFSWnd->CreateEx(
// 		0,
// 		_T("FlexSlideWnd"),
// 		_T(""),
// 		dwStyle | WS_CHILD,
// 		rc.left, 
// 		rc.top,
// 		rc.right - rc.left, 
// 		rc.bottom - rc.top,
// 		pParentWnd->GetSafeHwnd(),
// 		(HMENU)(UINT_PTR)(30001), 
// 		NULL);








}


void CFSBContainer::ShowWindow(int nFlag)
{
	m_pFSWnd->ShowWindow(nFlag);
}

BOOL CFSBContainer::SetPos_tstat6_2pos( int nMinPos, int nMidPos, int nMaxPos )
{
  
	return m_pFSWnd->SetPos_tstat6_2pos(nMinPos, nMidPos, nMaxPos);
}
// Sets the current position of the slider. 
// nMinPos 是最小的值，MidPos是中等值，Max是最大的值
BOOL CFSBContainer::SetPos(int nMinPos, int nMidPos, int nMaxPos)
{
    return m_pFSWnd->SetPos(nMinPos, nMidPos, nMaxPos);
}
BOOL CFSBContainer::SetPos_tstat6_3pos( int nMinPos, int nMidPos, int nMaxPos )
{
	return m_pFSWnd->SetPos_tstat6_3pos(nMinPos, nMidPos, nMaxPos);
}
