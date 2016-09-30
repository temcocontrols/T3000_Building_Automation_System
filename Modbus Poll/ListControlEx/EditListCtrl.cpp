// EditListCtrl.cpp : implementation file
// Download by http://www.codefans.net

#include "stdafx.h"
#include "EditListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
vector<CPoint> vctPoint;
/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl

CEditListCtrl::CEditListCtrl()
{
	m_iItem = -1;
	m_iSubItem = -1;
	
	m_bHighLight = FALSE;
	m_bFocus = FALSE;

	m_bEditDataChanged = FALSE;
}

CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl message handlers

void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	m_bFocus = TRUE;

	LVHITTESTINFO  lvhit;
	lvhit.pt = point;
	int item = SubItemHitTest(&lvhit);
	
	//if (over a item/subitem)
	if (item != -1 && (lvhit.flags & LVHT_ONITEM))
	{
		CListCtrl::OnLButtonDown(nFlags, point);
		
		if(m_bHighLight && m_iItem == lvhit.iItem && m_iSubItem == lvhit.iSubItem && (m_iSubItem==1 || m_iSubItem==2))//在这里只编辑坐标点
		{
			//第二次单击
			EditLabel(m_iItem);
			return;
		}
		else
		{
			//第一次单击
			m_iItem = lvhit.iItem;
			m_iSubItem = lvhit.iSubItem;
			m_bHighLight = TRUE;
		}
	}
	else
	{
		if(m_edtItemEdit.m_hWnd == NULL)
		{
			//未出现文本编辑框时
			m_bHighLight = FALSE;
		}
		
		CListCtrl::OnLButtonDown(nFlags, point);
	}
	
	Invalidate();
}

//EditLabel() cause this function has been called
void CEditListCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	if (m_iSubItem == 1 || m_iSubItem == 2)
    {
		ASSERT(m_iItem == pDispInfo->item.iItem);
		
		CRect  rcSubItem;
		GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_BOUNDS, rcSubItem);
		
		//get edit control and subclass
		HWND hWnd= (HWND)SendMessage(LVM_GETEDITCONTROL);
		ASSERT(hWnd != NULL);
		VERIFY(m_edtItemEdit.SubclassWindow(hWnd));

		//move edit control text 4 pixel to the right of org label,
		//as Windows does it...
		m_edtItemEdit.m_iXPos = rcSubItem.left + 4;
		preEditString = GetItemText(pDispInfo->item.iItem, m_iSubItem);
		m_edtItemEdit.SetWindowText(GetItemText(pDispInfo->item.iItem, m_iSubItem));
	}

	*pResult = 0;
}


void CEditListCtrl::OnPaint() 
{	
	//CPaintDC dc(this); // device context for painting	
	
	if (m_iSubItem >= 0 && m_edtItemEdit.m_hWnd)	
	{
		CRect	rect;
		CRect	rcEdit;
		m_edtItemEdit.GetWindowRect(rcEdit);
		ScreenToClient(rcEdit);

		GetSubItemRect(m_iItem, m_iSubItem, LVIR_LABEL, rect);

		//当文本编辑框缩小时,擦除露出的项文本高亮部分
		if (rcEdit.right < rect.right)
		{
			rect.left = rcEdit.right;
			CClientDC dc(this);
			dc.FillRect(rect, &CBrush(::GetSysColor(COLOR_WINDOW)));
			ValidateRect(rect);
		}

	}

	CListCtrl::OnPaint();
}


void CEditListCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM	*plvItem = &pDispInfo->item;
	
	if (m_iSubItem == 1 || m_iSubItem == 2)
	{
		if (plvItem->pszText != NULL )
		{
			SetItemText(plvItem->iItem,m_iSubItem, plvItem->pszText);//m_iSubItem为1就是X坐标,2就是Y坐标
			CString  strCmp(_T(""));

			if (GetItemText(plvItem->iItem,1)!=strCmp && GetItemText(plvItem->iItem,2)!=strCmp)
			{
				SetItemText(plvItem->iItem,3,_T(""));
			} 

			else
			{
				SetItemText(plvItem->iItem,3,_T("非法点"));
			}
			
			if (preEditString != plvItem->pszText)
			{
				m_bEditDataChanged = TRUE;//文字改变标志改变		
			}

			if (m_iSubItem == 1)
			{
				if (plvItem->pszText == strCmp)
				{
					vctPoint[plvItem->iItem].x = -9999999;
				} 
				else
				{
					vctPoint[plvItem->iItem].x = _wtoi(plvItem->pszText);
				}
				
			}
			else if (m_iSubItem == 2)
			{
				if (plvItem->pszText == strCmp)
				{
					vctPoint[plvItem->iItem].y = -9999999;
				} 
				else
				{
					vctPoint[plvItem->iItem].y = _wtoi(plvItem->pszText);
				}
				
			}
		}
		

		VERIFY(m_edtItemEdit.UnsubclassWindow()!=NULL);	
		*pResult = 0;
	}

	//编辑文本时对控件父窗口操作(如单击其它控件)引发"OnEndlabeledit"时刷新控件
	CRect rect;
	GetWindowRect(&rect);
	CPoint point;
	::GetCursorPos(&point);
	if(!rect.PtInRect(point))
	{
		m_iItem = -1;
		m_iSubItem = -1;
		m_bFocus = FALSE;
		m_bHighLight = FALSE;
	}
}

void CEditListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//draw each item.set txt color,bkcolor....
	NMLVCUSTOMDRAW* pNMLVCustomDraw = (NMLVCUSTOMDRAW*)pNMHDR;
	
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	
	if (pNMLVCustomDraw->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pNMLVCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pNMLVCustomDraw->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// store the colors back in the NMLVCUSTOMDRAW struct
		// but it's effective only when *pResult = CDRF_DODEFAULT 

		//	pNMLVCustomDraw->clrText = RGB(0, 0, 255);
		//	pNMLVCustomDraw->clrTextBk = RGB(0, 255, 0);
		//	*pResult = CDRF_DODEFAULT;
		
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.
		int iItem = (int)pNMLVCustomDraw->nmcd.dwItemSpec;
		int iSubItem = pNMLVCustomDraw->iSubItem;
		
		CDC* pDC = CDC::FromHandle(pNMLVCustomDraw->nmcd.hdc);
		
		CString strItemText = GetItemText(iItem, iSubItem);
		CRect rcItem, rcText;
		GetSubItemRect(iItem, iSubItem, LVIR_LABEL, rcItem);
		rcText = rcItem;
		
		CSize size = pDC->GetTextExtent(strItemText);
		if(strItemText == _T(""))
		{
			size.cx = 41; 
		}
		
		//设置文本高亮矩形
		rcText.left += 4;
		rcText.right = rcText.left + size.cx + 6;
		if(rcText.right > rcItem.right)
		{
			rcText.right = rcItem.right;
		}
		
		COLORREF crOldTextColor = pDC->GetTextColor();

		//绘制项焦点/高亮效果
		if(m_bFocus)
		{
			
			if((m_iItem == iItem) && (m_iSubItem == iSubItem))
			{	
				if(m_bHighLight)
				{					
					pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
					pDC->FillSolidRect(&rcText, ::GetSysColor(COLOR_HIGHLIGHT));
				}
				pDC->DrawFocusRect(&rcText);
			}		
		}
		
		//绘制项文本
		rcItem.left += 6;
		pDC->DrawText(strItemText, &rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOCLIP);

		pDC->SetTextColor(crOldTextColor);
		*pResult = CDRF_SKIPDEFAULT;// We've painted everything.
	}	
}


void CEditListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl::PreSubclassWindow();
	ModifyStyle(0, LVS_EDITLABELS);
}

void CEditListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetWindowRect(&rect);
	CPoint point;
	::GetCursorPos(&point);
	if(!rect.PtInRect(point) && GetParent()->GetFocus() != NULL)
	{
		m_iItem = -1;
		m_iSubItem = -1;
		m_bFocus = FALSE;
		m_bHighLight = FALSE;
		Invalidate();
	}
	
}


void CEditListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
//	CListCtrl::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CItemEdit

CItemEdit::CItemEdit()
{
}

CItemEdit::~CItemEdit()
{
}


BEGIN_MESSAGE_MAP(CItemEdit, CEdit)
    //{{AFX_MSG_MAP(CItemEdit)
    ON_WM_WINDOWPOSCHANGING()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemEdit message handlers

void CItemEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->x = m_iXPos;
	
	CEdit::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

