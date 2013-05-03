// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#include "T3000.h"
#include "MainFrm.h"
#include "WorkspaceBar.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//#include "MainFrm.h"



const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//ON_NOTIFY(TVN_SELCHANGED,ID_SHELLTREE,OnFolderSeletedChanged)
	ON_NOTIFY(TVN_ENDLABELEDIT, ID_SHELLTREE, OnTvnEndlabeleditTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, ID_SHELLTREE, OnTvnBeginlabeleditTree)
	ON_NOTIFY(NM_CLICK, ID_SHELLTREE, OnNMClickTree)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	// TODO: add one-time construction code here

}

CWorkspaceBar::~CWorkspaceBar()
{

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy (0, 0,200, 600);
	const DWORD dwViewStyle =	WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
   | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
   | TVS_DISABLEDRAGDROP | TVS_EDITLABELS;


   m_TreeCtrl.Create(dwViewStyle,CRect(10, 10, 300, 100), this,ID_SHELLTREE);
 //  m_image_list.Create(IDB_TREENODE_BMP,16,1,RGB(255,255,255));
	CBitmap a;
	a.LoadBitmap(IDB_TREENODE_BMP);
	m_image_list.Create(24,24,ILC_COLOR24,1,1);
	m_image_list.Add(&a,RGB(0,0,0));
    m_TreeCtrl.SetImageList(&m_image_list,TVSIL_NORMAL);



// Setup trees content:
//	HTREEITEM hRoot1 = m_TreeCtrl.InsertItem (_T("Root 1"));
//m_TreeCtrl.InsertItem (_T("Item 1"), hRoot1);
//	m_TreeCtrl.InsertItem (_T("Item 2"), hRoot1);
	return 0;
}
void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	if(m_TreeCtrl.m_hWnd!=NULL)
	{
		m_TreeCtrl.SetWindowPos (NULL, 0, 0,
		cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER);
		
		
	//	m_FolderBtm.SetWindowPos (NULL, 10, cy-29,
	//	70, 28,SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if(m_TreeCtrl.m_hWnd!=NULL)
	{
		m_TreeCtrl.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CWorkspaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	CRect rectTree;
	m_TreeCtrl.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,::GetSysColor (COLOR_3DSHADOW),
							::GetSysColor (COLOR_3DSHADOW));

}
void CWorkspaceBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void CWorkspaceBar::OnFolderSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strFolder=_T("");
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeItemSeletedChanged(pNMHDR,pResult);

}

void CWorkspaceBar::OnFolderBrowerBtn()
{

}




LRESULT CWorkspaceBar::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

return 0;   // 使得 dockable pane 无法拖动了。
	return CDockablePane::OnNcHitTest(point);
}

void CWorkspaceBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnLButtonDown(nFlags, point);
}


void CWorkspaceBar::OnTvnEndlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

//	m_TreeCtrl.SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeItemEndlabeledit(pNMHDR,pResult);
}

	
void CWorkspaceBar::OnTvnBeginlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here


	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	*pResult = pMainFrame->OnHTreeItemBeginlabeledit(pNMHDR,pResult);
}


void CWorkspaceBar::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeItemClick(pNMHDR,pResult);

}
