// GridButton.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "GridButton.h"


// CGridButton

IMPLEMENT_DYNAMIC(CGridButton, CButton)

CGridButton::CGridButton()
{

}

CGridButton::~CGridButton()
{
}


BEGIN_MESSAGE_MAP(CGridButton, CButton)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CGridButton message handlers

void CGridButton::SetPosition(int nRow, int nCol)
{
	m_nRow = nRow;
	m_nCol = nCol;
}

void CGridButton::SetPosition(CSize szPos)
{
	m_nRow = szPos.cx;
	m_nCol = szPos.cy;
	
}

CSize CGridButton::GetPosition()const
{
	CSize size;
	size.cx = m_nRow;
	size.cy = m_nCol;
	return size;
}

BOOL CGridButton::OnCommand(WPARAM wParam, LPARAM lParam)
{
	

	return CButton::OnCommand(wParam, lParam);
}

void CGridButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	
}
