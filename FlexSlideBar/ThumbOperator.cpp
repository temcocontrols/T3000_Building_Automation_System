#include "StdAfx.h"
#include "ThumbOperator.h"

CThumbOperator::CThumbOperator(void)
{
		m_bStartDraging = FALSE;
}

CThumbOperator::~CThumbOperator(void)
{

}


void CThumbOperator::SetParentWnd(CWnd* pParent)
{
	m_pParent = pParent;
}


void CThumbOperator::SetHorizontal(BOOL bHorizontal)
{
	m_bHorizontal = bHorizontal;
}

void CThumbOperator::SetFSBStyle(FSB_STYLE fsbStyle)
{
	m_fbStyle = fsbStyle;
}

void CThumbOperator::SetChannelRect(CRect rc)
{
	m_rcChannel = rc;		
}
	
void CThumbOperator::SetParentRect(CRect rc)
{
	m_rcParent = rc;
}

void CThumbOperator::SetLengthOfPage(int nLengthOfPage)
{
	m_nLengthOfPage = nLengthOfPage;
}

	
void CThumbOperator::SetLengthOfTic(int nLengthOfTic)
{
	m_nLengthOfTic = nLengthOfTic;
}

void CThumbOperator::SetPosValue(int nMinPos, int nMidPos, int nMaxPos){
    m_nMinPos=nMinPos;
    m_nMidPos=nMidPos;
    m_nMaxPos=nMaxPos;
}

