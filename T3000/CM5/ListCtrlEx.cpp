
#include "stdafx.h"
#include "ListCtrlEx.h"

#include "PerfTimer.h"

#include "..\global_variable_extern.h"
using namespace ListCtrlEx;
#define IDC_CELL_EDIT		 1000001
#define IDC_CELL_COMBO  1000002
#define IDC_PROGRESS_START IDC_CELL_COMBO+1

#define COLOR_FRAME RGB(51,102,153)
#define COLOR_MARK RGB(51,255,153)

#define SHOW_PROGRESS_PERCENT 0x00000001
#define SUPPORT_SORT	0x00000002
//////////////////////////////////////////////////////////////////////////
// CListCtrlEx
IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
	m_nImageListType=LVSIL_NORMAL;
	m_dwListCtrlExStyle=0;
	m_dwEditStyle = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_NOHIDESEL;
	m_dwComboStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
		CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	m_need_edit = true;
}

CListCtrlEx::~CListCtrlEx()
{
	CInPlaceCombo::DeleteInstance();
	CInPlaceEdit::DeleteInstance();
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &CListCtrlEx::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CListCtrlEx::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CListCtrlEx::OnLvnColumnclick)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// override CListCtrl interfaces
CImageList* CListCtrlEx::SetImageList( CImageList* pImageList, int nImageListType )
{
	m_nImageListType=nImageListType;
	return CListCtrl::SetImageList(pImageList, nImageListType);
}

int CListCtrlEx::InsertColumn(int nCol, CString strColHead, int nWidth, ColumnType eColType, int nFormat, 
										SortType eSortBy, int nSubItem )
{
	//
	m_mapCol2ColType[nCol]=eColType;
	m_mapCol2Sort[nCol].m_eSortType=eSortBy;
	return CListCtrl::InsertColumn(nCol, strColHead, nFormat, nWidth, nSubItem);
}

BOOL CListCtrlEx::DeleteColumn( int nCol  )
{
	if (!CListCtrl::DeleteColumn(nCol))
	{
		return FALSE;
	}
	m_mapCol2ColType.erase(nCol);
	return TRUE;
}

BOOL	CListCtrlEx::DeleteAllItems( )
{
	if (!CListCtrl::DeleteAllItems())
	{
		return FALSE;
	}
	m_mapCell2Data.clear();
	return TRUE;
}

BOOL CListCtrlEx::DeleteItem( int nItem )
{
	if (!CListCtrl::DeleteItem(nItem))
	{
		return FALSE;
	}
	// erase all of the row
	int nColCnt=GetColumnCount();
	for (int nCol=0; nCol<nColCnt; ++nCol)
	{
		m_mapCell2Data.erase(make_pair(nItem, nCol));
	}
	
	return TRUE;
}
// public interfaces
CListCtrlEx::CellIndex CListCtrlEx::Point2Cell(const CPoint &point)
{
	LVHITTESTINFO lvHitTestInfo;
	CRect rcItem;
	lvHitTestInfo.pt = point;
	if ((HitTest(&lvHitTestInfo) >= 0 || SubItemHitTest(&lvHitTestInfo) >=0 )&& lvHitTestInfo.iItem >= 0)
	{
		int nRow = lvHitTestInfo.iItem;
		int nColCnt=GetColumnCount();
		for (int nCol=0; nCol<nColCnt; ++nCol)
		{
			if (GetCellRect(nRow, nCol, rcItem))
			{
				if (rcItem.PtInRect(point))
				{
					return make_pair(nRow, nCol);
				}
			}
		}
	}
	return make_pair(-1, -1);
}
//////////////////////////////////////////////////////////////////////////
// need to deal with delete column
void CListCtrlEx::SetColumnType(int nColIndex, ColumnType eColType)
{
	ASSERT(0<=nColIndex && nColIndex<GetColumnCount());
	// init if empty
	if (m_mapCol2ColType.empty())
	{
		for (int i=0; i<GetColumnCount(); ++i)
		{
			m_mapCol2ColType.insert(ColMap::value_type(i, Normal));
		}
	}
	// assign
	m_mapCol2ColType[nColIndex]=eColType;
	// maybe should update window here
}

CListCtrlEx::ColumnType CListCtrlEx::GetColumnType(int nColIndex)
{
	// insert new if not found
	ColMap::iterator iter=m_mapCol2ColType.find(nColIndex);
	if (iter==m_mapCol2ColType.end())
	{
		m_mapCol2ColType[nColIndex]=Normal;
	}
	return m_mapCol2ColType[nColIndex];
}

SortType CListCtrlEx::GetColumnSortBy(int nColIndex)
{
	// insert new if not found
	ColSortMap::iterator iter=m_mapCol2Sort.find(nColIndex);
	if (m_mapCol2Sort.end()==iter)
	{
		m_mapCol2Sort[nColIndex]=_ColumnSort_t();
	}
	return m_mapCol2Sort[nColIndex].m_eSortType;
}

void CListCtrlEx::SetColumnSortBy(int nColIndex, SortType eSortBy)
{
	// init sort map
	if (m_mapCol2Sort.empty())
	{
		for (int i=0; i<GetColumnCount(); ++i)
		{
			m_mapCol2Sort[i]=_ColumnSort_t();
		}
	}
	// assign
	m_mapCol2Sort[nColIndex].m_eSortType=eSortBy;
}
//////////////////////////////////////////////////////////////////////////
// need to deal with delete items
void CListCtrlEx::SetCellEnabled(int nRow, int nCol, BOOL bEnabled)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	//FindCellData(nRow, nCol).m_bEnabled=bEnabled;
	FindCellData(nRow, nCol).SetEnable(bEnabled);
	CRect rcCell;
	if(GetCellRect(nRow, nCol, rcCell))
	{
		InvalidateRect(&rcCell);
		UpdateWindow();
		// send message to parent
		CWnd *pWnd=NULL;
		if ((pWnd = GetParent())!=NULL )
		{
			EnableCellMsg msg(nRow, nCol, bEnabled);
			pWnd->SendMessage(WM_ListCtrlEx_ENABLE_CHANGED, (WPARAM)this, (LPARAM)&msg);
		}
	}
}

BOOL CListCtrlEx::GetCellEnabled(int nRow, int nCol)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	return FindCellData(nRow, nCol).GetEnable();
}


void CListCtrlEx::SetCellImage(int nRow, int nCol, int nImage)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	FindCellData(nRow, nCol).m_nImage=nImage;
}

void CListCtrlEx::SetCellChecked(int nRow, int nCol, BOOL bChecked)
{
	int nColCnt=GetColumnCount();
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol<nColCnt );
	ColumnType eColType=m_mapCol2ColType[nCol];
	// only check box and radio box can be checked
	if (CheckBox==eColType)
	{
		FindCellData(nRow, nCol).SetCheck(bChecked);
		CRect rcCell;
		if(GetCellRect(nRow, nCol, rcCell))
		{
			InvalidateRect(&rcCell); // update check box here immediately
			UpdateWindow();
			// send message to parent
			CWnd *pWnd=NULL;
			if ((pWnd = GetParent())!=NULL )
			{
				CheckCellMsg msg(nRow, nCol, bChecked);
				pWnd->SendMessage(WM_ListCtrlEx_CHECKCELL, (WPARAM)this, (LPARAM)&msg);
			}
		}
	}
	else if(RadioBox==eColType)
	{
		int nGrpNo=FindCellData(nRow, nCol).m_nRadioGroup;
		int nRowCnt=GetItemCount();
		CRect rcCell;
		for (int y=0; y<nColCnt; ++y)
		{
			if (RadioBox==m_mapCol2ColType[y])
			{
				for (int x=0; x<nRowCnt; ++x)
				{
					if (nGrpNo==FindCellData(x,y).m_nRadioGroup && FindCellData(x,y).GetCheck()) // only need to update checked one
					{
						FindCellData(x,y).SetCheck(FALSE);
						if(GetCellRect(x, y, rcCell))
						{
							InvalidateRect(&rcCell); // update radio box here immediately
							UpdateWindow();	
						}
					}
				}
			}
		}
		bChecked=TRUE; // check on a radio will always cause to be true
		FindCellData(nRow, nCol).SetCheck(bChecked); 
		if(GetCellRect(nRow, nCol, rcCell))
		{
			InvalidateRect(&rcCell); // update radio box here immediately
			UpdateWindow();	
		}
		// send message to parent
		CWnd *pWnd=NULL;
		if ((pWnd = GetParent())!=NULL )
		{
			CheckCellMsg msg(nRow, nCol, bChecked);
			pWnd->SendMessage(WM_ListCtrlEx_CHECKCELL, (WPARAM)this, (LPARAM)&msg);
		}
	}
	
}

BOOL CListCtrlEx::GetCellChecked(int nRow, int nCol)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	return FindCellData(nRow, nCol).GetCheck();
}

int	CListCtrlEx::GetCellRadioGrp(int nRow, int nCol)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	return FindCellData(nRow, nCol).m_nRadioGroup;
}

void CListCtrlEx::SetCellRadioGrp(int nRow, int nCol, int nGroupNO)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	FindCellData(nRow, nCol).m_nRadioGroup=nGroupNO;
}

void CListCtrlEx::SetCellStringList(int nRow, int nCol, CStrList &lstStrings)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	CStrList &aCellStrList=FindCellData(nRow, nCol).m_lstString;
	aCellStrList.clear();
	aCellStrList.insert(aCellStrList.begin(), lstStrings.begin(), lstStrings.end());
}

void CListCtrlEx::AddCellString(int nRow, int nCol, CString &str)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	CStrList &aCellStrList=FindCellData(nRow, nCol).m_lstString;
	aCellStrList.push_back(str);
}

void CListCtrlEx::SetCellValidChars(int nRow, int nCol, CString &strValidChars)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	FindCellData(nRow, nCol).m_strValidChars=strValidChars;
}

void CListCtrlEx::SetCellProgressMaxValue(int nRow, int nCol, UINT uMaxValue)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	FindCellData(nRow, nCol).m_uProgressMax=uMaxValue;
	CRect rcCell;
	if(GetCellRect(nRow, nCol, rcCell))
	{
		InvalidateRect(&rcCell); // update progress bar here immediately
		UpdateWindow();	
	}
}

void CListCtrlEx::SetCellProgressValue(int nRow, int nCol, UINT uValue)
{
	ASSERT(0<=nRow && nRow<GetItemCount() && nCol>=0 && nCol< GetColumnCount());
	FindCellData(nRow, nCol).m_uProgresVal=uValue;
	CRect rcCell;
	if(GetCellRect(nRow, nCol, rcCell))
	{
		InvalidateRect(&rcCell); // update progress bar here immediately
		UpdateWindow();	
	}
}

BOOL CListCtrlEx::IsShowPogressPercent()
{ 
	return m_dwListCtrlExStyle & SHOW_PROGRESS_PERCENT; 
}
void CListCtrlEx::SetShowProgressPercent(BOOL bShowProgPercent)
{
	if (bShowProgPercent)
		m_dwListCtrlExStyle|=SHOW_PROGRESS_PERCENT;
	else
		m_dwListCtrlExStyle&=(~SHOW_PROGRESS_PERCENT);
}

BOOL CListCtrlEx::IsSupportSort()
{
	return m_dwListCtrlExStyle & SUPPORT_SORT;
}

void CListCtrlEx::SetSupportSort(BOOL bSuptSort)
{
	if (bSuptSort)
		m_dwListCtrlExStyle|=SUPPORT_SORT;
	else
		m_dwListCtrlExStyle&=(~SUPPORT_SORT);
}

//////////////////////////////////////////////////////////////////////////
BOOL CListCtrlEx::GetCellRect(CellIndex ix, CRect &rect)
{
	return GetCellRect(ix.first, ix.second, rect);
}

BOOL CListCtrlEx::GetCellRect(int iRow, int iCol, CRect &rect, int nArea)
{
	ASSERT(0<=iRow && iRow<GetItemCount() && iCol>=0 && iCol< GetColumnCount());
	if(iCol)
		return GetSubItemRect(iRow, iCol, nArea, rect);

	if(GetColumnCount()== 1)
		return GetItemRect(iRow, rect, nArea);

	iCol = 1;
	CRect rCol1;
	if(!GetSubItemRect(iRow, iCol, nArea, rCol1))
		return FALSE;

	if(!GetItemRect(iRow, rect, nArea))
		return FALSE;

	rect.right = rCol1.left;

	return TRUE;
}

void      CListCtrlEx::Set_Edit(bool b_edit)
{
	m_need_edit = b_edit;
}
BOOL		CListCtrlEx::Get_Edit()
{
	return m_need_edit;
}

BOOL	CListCtrlEx::GetCheckRect(int iRow, int iCol, CRect &rect)
{
	// this rect is to do action rect, so it's larger than the draw box
	if (GetCellRect(iRow, iCol, rect))
	{
		//rect.bottom -= 1;
		//rect.top+=1;
		rect.left += 7;        
		rect.right = rect.left + rect.Height();    // width = height
		return TRUE;
	}
	return FALSE;
}



BOOL	CListCtrlEx::GetRowRect(int nRow, CRect &rcRow)
{
	ASSERT(0<=nRow && nRow<GetItemCount());
	return GetItemRect(nRow, rcRow, LVIR_BOUNDS);
}

BOOL	CListCtrlEx::GetColRect(int nCol, CRect &rcCol)
{
	ASSERT(nCol>=0 && nCol< GetColumnCount());
	rcCol =CRect(0,0,0,0);
	CRect rcCellInCol(0,0,0,0);
	int nRowCnt=GetItemCount();
	for (int i=0; i<nRowCnt; ++i)
	{
		if (GetCellRect(i, nCol, rcCellInCol))
		{
			if (0==i)
			{
				rcCol=rcCellInCol;
			}
			else
			{
				rcCol.UnionRect(&rcCol, &rcCellInCol);
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

int CListCtrlEx::GetColumnFmt(int nCol)
{
	ASSERT(0<=nCol && nCol<GetColumnCount());
	LVCOLUMN lvColumn;
	ZeroMemory(&lvColumn, sizeof(lvColumn));
	lvColumn.mask = LVCF_FMT;
	if (GetColumn(nCol, &lvColumn))
	{
		return lvColumn.fmt;
	}
	return LVCFMT_CENTER;
}
//////////////////////////////////////////////////////////////////////////
// customer draw
void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//FuncTime(_T("OnNMCustomdraw"));
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW  lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	int iCol = lplvcd->iSubItem;
	int iRow = (int)lplvcd->nmcd.dwItemSpec;
	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:			// recd when CDRF_NOTIFYSUBITEMDRAW is returned in
		{	
			// response to CDDS_ITEMPREPAINT.
			*pResult = CDRF_DODEFAULT;			
			CString strText = GetItemText(iRow, iCol);
			CRect rcCell;
			if (GetCellRect(iRow, iCol, rcCell))
			{
				bool bSelected=GetItemState(iRow, LVIS_SELECTED) == LVIS_SELECTED;
				// get the device context.
				CDC *pDC= CDC::FromHandle(lplvcd->nmcd.hdc);
				// draw the cell.
				DrawCell(pDC, strText, rcCell, bSelected, iRow, iCol, pResult ); // *pResult will be changed here
			}	

			break;
		}
	default:														// it wasn't a notification that was interesting to us.
		*pResult = CDRF_DODEFAULT;
		break;
	}
	return;
}

void CListCtrlEx::DrawCell(CDC *pDC,  CString &strText, CRect &rcCell, BOOL bSelected, 
										int nRow, int nCol, LRESULT *pResult)
{
	switch(GetColumnType(nCol))
	{
	default:
	case Normal:
		*pResult |= CDRF_SKIPDEFAULT;
		DrawNormal(pDC, strText, rcCell, bSelected, FindCellData(nRow, nCol), GetColumnFmt(nCol));
		return;
	case CheckBox:
		*pResult |= CDRF_SKIPDEFAULT;
		DrawCheckBox(pDC, strText, rcCell, bSelected, FindCellData(nRow, nCol));
		return;
	case RadioBox:
		*pResult |= CDRF_SKIPDEFAULT;
		DrawRadioBox(pDC, strText, rcCell, bSelected, FindCellData(nRow, nCol));
		return;
	case ComboBox:
		*pResult |= CDRF_DODEFAULT;//Fance
		DrawComboBox(pDC, strText, rcCell, bSelected, FindCellData(nRow, nCol));
		return;
	case EditBox:
		*pResult|=CDRF_SKIPDEFAULT;
		DrawEditBox(pDC, strText, rcCell, bSelected, FindCellData(nRow, nCol), GetColumnFmt(nCol));
		return;
	case Progress:
		*pResult|=CDRF_SKIPDEFAULT;
		DrawProgress(pDC, strText, rcCell, bSelected, FindCellData(nRow,nCol));
		return;
	}
}

// draw normal text
void CListCtrlEx::DrawNormal(CDC *pDC, CString &strText, CRect &rcCell, BOOL bSelected, const CellData &cellData, int uLvcFmt)
{
	UNREFERENCED_PARAMETER(cellData);
	COLORREF crText=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crTextBkgrnd=bSelected?GetSysColor(COLOR_HIGHLIGHT):GetSysColor(COLOR_WINDOW);
	// fill background
	CRect rcText(rcCell), rcIcon(rcCell);
	pDC->FillSolidRect(&rcCell, crTextBkgrnd);
	//-- draw icon--//
	CImageList *pImageList=TheImageList();
	if (cellData.m_nImage>=0 && pImageList)
	{
		rcIcon.right=rcIcon.left+rcIcon.Height();
		//UINT fStyle=bSelected?ILD_NORMAL:(ILD_NORMAL|ILD_SELECTED);
		pImageList->DrawIndirect(	pDC, cellData.m_nImage, rcIcon.TopLeft(), rcIcon.Size(), CPoint(0, 0), ILD_NORMAL, 
											SRCCOPY,crTextBkgrnd, crTextBkgrnd );
		//pImageList->DrawIndirect(	pDC, cellData.m_nImage, rcIcon.TopLeft(), rcIcon.Size(), CPoint(0, 0));
	}
	else
	{
		rcIcon.right=rcIcon.left;
	}
	//------------draw text---------------------/
	UINT nFormat=DT_CENTER;
	if(uLvcFmt & LVCFMT_LEFT)
	{
		nFormat=DT_LEFT;
	}
	else if (uLvcFmt & LVCFMT_RIGHT)
	{
		nFormat=DT_RIGHT;
	}

	COLORREF crOldText=pDC->SetTextColor(crText);
	rcText.left=rcIcon.right+3;
	pDC->DrawText(strText, &rcText, nFormat);
	pDC->SetTextColor(crOldText);
}

// draw a check box cell
void CListCtrlEx::DrawCheckBox(CDC *pDC,  CString &strText, CRect &rcCell, BOOL bSelected, const CellData &cellData)
{
	COLORREF crText=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crTextBkgrnd=bSelected?GetSysColor(COLOR_HIGHLIGHT):GetSysColor(COLOR_WINDOW);
	COLORREF crCheckFrame=cellData.GetEnable()?RGB(51,102,153):RGB(192,192,192);
	COLORREF crCheckMark=cellData.GetEnable()?RGB(51,255,153):RGB(192,192,192);
	//------------calculate the check box & Text rect ---------------------/
	CRect rcText(rcCell);
	CRect rcCheck(rcCell);
	rcCheck.bottom -= 2;
	rcCheck.top+=2;
	rcCheck.left += 7;        
	rcCheck.right = rcCheck.left + rcCheck.Height();    // width = height
	rcText.left=rcCheck.right+3;
	//------------draw the inside check box region---------------------/
	pDC->FillSolidRect(&rcText, crTextBkgrnd); 
	pDC->FillSolidRect(&rcCheck, GetSysColor(COLOR_WINDOW)); // refresh the check box
	//------------draw the check box frame border ---------------------/
#ifdef USING_CUSTOM_DRAW
	//CBrush brush(crCheckFrame);
	//pDC->FrameRect(&rcCheck, &brush); //fill frame of the check box
	CPen penFrame(PS_SOLID, 2,crCheckFrame);
	CPen *pOldPen=pDC->SelectObject(&penFrame);
	pDC->Rectangle(&rcCheck);
	//------------draw the check mark---------------------/
	if (cellData.GetCheck() ) // draw the checked Mark
	{
		int deta=rcCheck.Width()/4;
		CPen penMark(PS_SOLID, 1,crCheckMark);
		pDC->SelectObject(&penMark);
		int x = rcCheck.left + deta*3;
		int y = rcCheck.top + deta;
		int i;
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y--;
		}
		
	}
	pDC->SelectObject(pOldPen);
#else
	crCheckMark;crCheckFrame; // only for kill the warnings
	UINT nState=DFCS_BUTTONCHECK;
	if (cellData.GetCheck() ) // draw the checked Mark
	{
		nState|=DFCS_CHECKED;
	}
	if (bSelected)
	{
		nState|=DFCS_HOT;
	}
	if (!cellData.GetEnable())
	{
		nState|=DFCS_INACTIVE;
	}
	pDC->DrawFrameControl(&rcCheck, DFC_BUTTON, nState);
#endif
	//------------draw text---------------------/
	COLORREF crOldText=pDC->SetTextColor(crText);
	pDC->DrawText(strText, &rcText, DT_LEFT);
	pDC->SetTextColor(crOldText);
}
// draw a Radio box cell
void CListCtrlEx::DrawRadioBox(CDC *pDC,  CString &strText, CRect &rcCell, BOOL bSelected, const CellData &cellData)
{
	COLORREF crText=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crTextBkgrnd=bSelected?GetSysColor(COLOR_HIGHLIGHT):GetSysColor(COLOR_WINDOW);
	COLORREF crRadioFrame=cellData.GetEnable()?RGB(51,102,153):RGB(192,192,192);
	COLORREF crRadioFrameBk=GetSysColor(COLOR_WINDOW);
	COLORREF crRadioMark=cellData.GetEnable()?RGB(0,0,0):RGB(192,192,192);
	//------------calculate the check box & Text rect ---------------------/
	CRect rcText(rcCell);
	CRect rcRadio(rcCell);
	rcRadio.bottom -= 1;
	rcRadio.top+=1;
	rcRadio.left += 7;        
	rcRadio.right = rcRadio.left + rcRadio.Height();    // width = height
	rcText.left=rcRadio.right+3;
	//------------draw the inside radio box region---------------------/
	pDC->FillSolidRect(&rcText, crTextBkgrnd); 
	pDC->FillSolidRect(&rcRadio, crRadioFrameBk); // refresh the check box
	//------------draw the radio box frame border ---------------------/
#ifdef USING_CUSTOM_DRAW
	// using three dot to draw the radio box
	CPoint pos=rcRadio.TopLeft();	
	pos +=CSize(rcRadio.Width()/2, rcRadio.Height()/2);
	int nPenWid=rcRadio.Height();
	// ----draw frame----/
	// outer frame
	CPen pen(PS_DOT, nPenWid, crRadioFrame); 
	CPen *pOldPen=pDC->SelectObject(&pen);
	pDC->MoveTo(pos);
	pDC->LineTo(pos);
	// inner frame
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	nPenWid=((nPenWid-4)>0)?(nPenWid-4):4;
	pen.CreatePen(PS_DOT, nPenWid, crRadioFrameBk);
	pDC->SelectObject(&pen);
	pDC->MoveTo(pos);
	pDC->LineTo(pos);
	// draw mark
	if (cellData.GetCheck())
	{
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		nPenWid=((nPenWid-4)>0)?(nPenWid-4):4;
		pen.CreatePen(PS_DOT, nPenWid, crRadioMark);
		pDC->SelectObject(&pen);
		pDC->MoveTo(pos);
		pDC->LineTo(pos);
	}
	pDC->SelectObject(pOldPen);
#else
	crRadioFrame;crRadioMark; // only for kill the warnings
	UINT nState=DFCS_BUTTONRADIO;
	if (cellData.GetCheck() ) // draw the checked Mark
	{
		nState|=DFCS_CHECKED;
	}
	if (!cellData.GetEnable())
	{
		nState|=DFCS_INACTIVE;
	}
	pDC->DrawFrameControl(&rcRadio, DFC_BUTTON, nState);
#endif
	//------------draw text---------------------/
	COLORREF crOldText=pDC->SetTextColor(crText);
	pDC->DrawText(strText, &rcText, DT_LEFT);
	pDC->SetTextColor(crOldText);
	
}
// draw a combo box cell
void CListCtrlEx::DrawComboBox(CDC *pDC,  CString &strText, CRect &rcCell, BOOL bSelected, const CellData &cellData)
{
	COLORREF crText=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crTextBkgrnd=bSelected?GetSysColor(COLOR_HIGHLIGHT):GetSysColor(COLOR_WINDOW);
	COLORREF crComboTL=cellData.GetEnable()?RGB(255,255,255):RGB(192,192,192); //RGB(212,225,252):RGB(192,192,192);
	COLORREF crComboBR=cellData.GetEnable()? RGB(177,203,250):RGB(192,192,192);
	COLORREF crComboMark=cellData.GetEnable()?RGB(77,97,133):RGB(0,0,0);
	//------------calculate the check box & Text rect ---------------------/
	CRect rcText(rcCell);
	CRect rcCombo(rcCell);
	rcCombo.bottom -= 0;
	rcCombo.top+=1;
	rcCombo.right -= 2;        
	rcCombo.left = rcCombo.right - rcCombo.Height();    // width = height
	rcText.right=rcCombo.left-3;
	rcText.left+=2;
	//---------refresh the cell------/
	pDC->FillSolidRect(&rcCell, GetSysColor(COLOR_WINDOW));
	pDC->FillSolidRect(&rcText, crTextBkgrnd); 
	//------------draw the combo box frame border ---------------------/
#ifdef USING_CUSTOM_DRAW
	CBrush brBottomRight(crComboBR);
	CBrush *pOldBrush=pDC->SelectObject(&brBottomRight);
	/*CRect rcInner(rcCombo);
	rcInner.DeflateRect(1,1,1,1);*/
	//pDC->FillSolidRect(&rcInner, crComboBR);
	DrawGradientRect(pDC, rcCombo,  crComboTL, crComboBR);
	//pDC->Draw3dRect(&rcCombo, crComboTL, crComboBR);
	pDC->SelectObject(pOldBrush);
	
	CPen penMark(PS_SOLID, 1, crComboMark);
	CPen *pOldPen=pDC->SelectObject(&penMark);
	// draw the arrow
	{
		int xOrg=rcCombo.Width()/2+rcCombo.left;
		int yOrg=rcCombo.Height()/2+rcCombo.top+2;
		int xOff=max(rcCombo.Width()/4,1);
		int yOff=max(rcCombo.Height()/4,1);
		int y=yOrg-yOff;
		for (int x=(xOrg-xOff); x<xOrg; ++x, ++y)
		{
			pDC->MoveTo(x,y);
			pDC->LineTo(x+2*(xOrg-x), y);
		}
	}

	pDC->SelectObject(pOldPen);

#else
	crComboTL;crComboBR;crComboMark;  // only for kill the warnings
	UINT nState=DFCS_SCROLLCOMBOBOX;
	if (cellData.GetCheck() ) // draw the checked Mark
	{
		nState|=DFCS_CHECKED;
	}
	if (bSelected)
	{
		nState|=DFCS_HOT;
	}
	if (!cellData.GetEnable())
	{
		nState|=DFCS_INACTIVE;
	}
	pDC->DrawFrameControl(&rcCombo, DFC_SCROLL, nState);

#endif
	//------------draw text---------------------/
	COLORREF crOldText=pDC->SetTextColor(crText);
	pDC->DrawText(strText, &rcText, DT_LEFT);
	pDC->SetTextColor(crOldText);
}


// draw a edit box cell
void CListCtrlEx::DrawEditBox(CDC *pDC,  CString &strText, CRect &rcCell, BOOL bSelected, const CellData &cellData, int uLvcFmt)
{
	COLORREF crText=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crTextBkgrnd=bSelected?GetSysColor(COLOR_HIGHLIGHT):GetSysColor(COLOR_WINDOW);
	COLORREF crTextUnabled=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):RGB(192,192,192);

	CRect rcText(rcCell), rcIcon(rcCell);
	// fill background
	pDC->FillSolidRect(&rcCell, crTextBkgrnd);
	//-- draw icon--//
	CImageList *pImageList=TheImageList();
	if (cellData.m_nImage>=0 && pImageList)
	{
		rcIcon.right=rcIcon.left+rcIcon.Height();
		pImageList->DrawIndirect(	pDC, cellData.m_nImage, rcIcon.TopLeft(), rcIcon.Size(), CPoint(0, 0), ILD_NORMAL, 
									SRCCOPY,crTextBkgrnd, crTextBkgrnd );
	}
	else
	{
		rcIcon.right=rcIcon.left;
	}
#ifdef USING_CUSTOM_DRAW
#else
#endif	
	//------------draw text---------------------/
	UINT nFormat=DT_CENTER;
	switch(uLvcFmt)
	{
	default:
	case LVCFMT_CENTER: break;
	case LVCFMT_LEFT: nFormat=DT_LEFT; break;
	case LVCFMT_RIGHT: nFormat=DT_RIGHT; break;
	}
	COLORREF crOldText=pDC->SetTextColor(crText);
	if (!cellData.GetEnable())
	{
		pDC->SetTextColor(crTextUnabled);
	}
	rcText.left=rcIcon.right+3;
	pDC->DrawText(strText, &rcText, nFormat);
	pDC->SetTextColor(crOldText);
}

// draw a progress bar
void CListCtrlEx::DrawProgress(CDC *pDC,  CString &strText, CRect &rcCell, BOOL bSelected, const CellData &cellData)
{
	COLORREF crText=bSelected?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crTextBkgrnd=bSelected?GetSysColor(COLOR_HIGHLIGHT):GetSysColor(COLOR_WINDOW);
	COLORREF crProgBack=bSelected?GetSysColor(COLOR_HOTLIGHT):GetSysColor(COLOR_BTNFACE);
	COLORREF crProgFill=bSelected?COLOR_FRAME:COLOR_FRAME;

	CRect rcProg(rcCell), rcIcon(rcCell);
	// fill background
	pDC->FillSolidRect(&rcCell, crTextBkgrnd);
	//-- draw icon--//
	CImageList *pImageList=TheImageList();
	if (cellData.m_nImage>=0 && pImageList)
	{
		rcIcon.right=rcIcon.left+rcIcon.Height();
		pImageList->DrawIndirect(	pDC, cellData.m_nImage, rcIcon.TopLeft(), rcIcon.Size(), CPoint(0, 0), ILD_NORMAL, 
			SRCCOPY,crTextBkgrnd, crTextBkgrnd );
	}
	else
	{
		rcIcon.right=rcIcon.left;
	}
	rcProg.left=rcIcon.right+3;


	pDC->FillSolidRect(&rcProg, crProgBack);
	pDC->DrawEdge(&rcProg, EDGE_SUNKEN, BF_RECT);
	CRect rcFill(rcProg);
	rcFill.DeflateRect(1,1,1,1);
	float fPersent=0.0f;
	int nFill=GetProgBarSize(cellData, rcProg, &fPersent);
	rcFill.right=min(rcFill.left+nFill, rcFill.right);
	pDC->FillSolidRect(&rcFill, crProgFill);
	COLORREF crOldText=pDC->SetTextColor(crText);

	if (IsShowPogressPercent())
	{
		strText.Format(_T("%.2f%%"), fPersent*100);
	}
	pDC->DrawText(strText, &rcProg, DT_CENTER);
	pDC->SetTextColor(crOldText);
#ifdef USING_CUSTOM_DRAW	
#else
#endif	
	
}
//////////////////////////////////////////////////////////////////////////
// kernel data 
int	CListCtrlEx::GetProgBarSize(const CListCtrlEx::CellData &cellData, CRect &rcProg, float *pPersent)
{
	float fScale=0.0f;
	if (cellData.m_uProgresVal>=cellData.m_uProgressMax)
		fScale=1.0f;
	else if(cellData.m_uProgresVal<=0 || cellData.m_uProgressMax<=0)
		fScale=0.0f;
	else
		fScale=(float)cellData.m_uProgresVal/(float)cellData.m_uProgressMax;

	if (pPersent)
	{
		*pPersent=fScale;
	}
	return (int)(rcProg.Width()*fScale);
}

CImageList* CListCtrlEx::TheImageList()
{
	return GetImageList(m_nImageListType);
}

CListCtrlEx::CellData& CListCtrlEx::FindCellData(CellIndex ix)
{
	return FindCellData(ix.first, ix.second);
}

CListCtrlEx::CellData& CListCtrlEx::FindCellData(int nRow, int nCol)
{
	// init if empty
	if (m_mapCell2Data.empty())
	{
		for (int x=0; x<GetItemCount(); ++x)
		{
			for (int y=0; y< GetColumnCount(); ++y)
			{
				m_mapCell2Data[make_pair(x,y)]=CellData(TRUE, FALSE, y);
			}
		}
	}

	CellDataMap::iterator iter=m_mapCell2Data.find(make_pair(nRow, nCol));
	if (iter==m_mapCell2Data.end())
	{
		m_mapCell2Data[make_pair(nRow, nCol)]=CellData(TRUE, FALSE, nCol);
	}
	return m_mapCell2Data[make_pair(nRow, nCol)];
}

//////////////////////////////////////////////////////////////////////////
// show control
void CListCtrlEx::ShowCellInPlace(CListCtrlEx::CellIndex ix, CellType eCellType)
{
	// roll up make the cell be visible
	if (!EnsureVisible(ix.first, TRUE))
	{
		return;
	}
	switch(eCellType)
	{
	default:
	case Normal:
	case CheckBox:
	case RadioBox:
		return;
	case ComboBox:
		ShowInPlaceCombo(ix);
		break;
	case EditBox:
		ShowInPlaceEdit(ix);
		break;
	}
}

// show combo box in place
void CListCtrlEx::ShowInPlaceCombo(CellIndex ix)
{
	if (ix.first>=0 && ix.first<GetItemCount() && ix.second>=0 && ix.second< GetColumnCount())
	{
		CString strCurSel= GetItemText(ix.first, ix.second);
		CRect rcCell;
		if (GetCellRect(ix, rcCell))
		{	
			GetParent()->SendMessage(WM_SET_ITEMS, (WPARAM)this, (LPARAM)&ix);  
			CInPlaceCombo* pInPlaceComboBox = CInPlaceCombo::GetInstance();
			ASSERT(pInPlaceComboBox); 
			pInPlaceComboBox->ShowComboCtrl(m_dwComboStyle, rcCell, this, IDC_CELL_COMBO, 
												ix.first, ix.second, FindCellData(ix).m_lstString, strCurSel );
			pInPlaceComboBox->SelectString(0, strCurSel);
		}
	}
}

// show edit box in place
void CListCtrlEx::ShowInPlaceEdit(CellIndex ix)
{
	if (ix.first>=0 && ix.first<GetItemCount() && ix.second>=0 && ix.second< GetColumnCount())
	{
		CString strCurSel= GetItemText(ix.first, ix.second);
		CRect rcCell;
		if (GetCellRect(ix, rcCell))
		{	
			GetParent()->SendMessage(WM_SET_ITEMS, (WPARAM)this, (LPARAM)&ix);  
			CInPlaceEdit* pInPlaceEdit = CInPlaceEdit::GetInstance();
			ASSERT(pInPlaceEdit); 
			pInPlaceEdit->ShowEditCtrl(m_dwEditStyle, rcCell, this, IDC_CELL_EDIT, ix.first, ix.second,
													FindCellData(ix).m_strValidChars, strCurSel);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// message handlers
void ListCtrlEx::CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	//FuncTime(_T("OnLButtonDown"));
	CWnd* pWnd=NULL;
	bool bShouldAction=false;
	CellIndex ix=Point2Cell(point);
	
	bShouldAction=(ix.first>=0 && ix.second>=0 && ix.first<GetItemCount() && ix.second<GetColumnCount());
	
	CListCtrl::OnLButtonDown(nFlags, point);
	if(m_need_edit == false)
		return;
	// If the SHIFT or CTRL key is down call the base class
	// Check the high bit of GetKeyState to determine whether SHIFT or CTRL key is down
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		return;
	}

	if (bShouldAction && GetCellEnabled(ix.first, ix.second))
	{
		/*CRect rcItem;
		GetRowRect(ix.first, rcItem);*/
		switch(m_mapCol2ColType[ix.second])
		{
		case CheckBox:
		case RadioBox:
			{ // can check cell 
				CRect rcCheck;
				bool bShouldCheck=(GetCheckRect(ix.first, ix.second, rcCheck) && rcCheck.PtInRect(point));
				if (bShouldCheck)
				{
					SetCellChecked(ix.first, ix.second, !GetCellChecked(ix.first, ix.second));					
					if ((pWnd = GetParent())!=NULL)
					{
						CheckCellMsg msg(ix.first, ix.second, GetCellChecked(ix.first, ix.second));
						pWnd->SendMessage(WM_ListCtrlEx_CHECK_CHANGED, (WPARAM)this, (LPARAM)&msg);
					}
				}		
			}
			break;
		case ComboBox:
		case EditBox:
			{ // can show or edit in place cell
				ShowCellInPlace(ix, m_mapCol2ColType[ix.second]);
			}
		    break;
		case Normal:
		default:			
		    break;
		}
		//InvalidateRect(&rcItem); // normal or default should be update immediately for the focus state
		//UpdateWindow();	
	}

	// notify parent with user messages
	if ((pWnd = GetParent())!=NULL )
	{
			pWnd->SendMessage(WM_ListCtrlEx_LBUTTONDOWN, (WPARAM)this, (LPARAM)&ix);	
	}
	
}

void ListCtrlEx::CListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd* pWnd=NULL;
	bool bShouldAction=false;
	CellIndex ix=Point2Cell(point);
	bShouldAction=(ix.first>=0 && ix.second>=0 && ix.first<GetItemCount() && ix.second<GetColumnCount());
	// actions
	if (bShouldAction && GetCellEnabled(ix.first, ix.second))
	{
		CRect rcItem;
		GetRowRect(ix.first, rcItem);
		CRect rcCheck;
		bool bShouldCheck=(GetCheckRect(ix.first, ix.second, rcCheck) && rcCheck.PtInRect(point));
		if (bShouldCheck)
		{
			//SetCellChecked(ix.first, ix.second, !GetCellChecked(ix.first, ix.second));
			//InvalidateRect(&rcCheck); // should update window here immediately
			//UpdateWindow();
		}	
		InvalidateRect(&rcItem); // normal or default should be update immediately for the focus state
		UpdateWindow();
	}
	// notify parent with user messages
	if ((pWnd = GetParent())!=NULL )
	{
		pWnd->PostMessage(WM_ListCtrlEx_LBUTTONDBLCLK, (WPARAM)this, (LPARAM)&ix);
	}
	// do default action
	CListCtrl::OnLButtonDblClk(nFlags, point);
	
}

void ListCtrlEx::CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pWnd=NULL;
	bool bShouldAction=false;
	CellIndex ix=Point2Cell(point);
	bShouldAction=(ix.first>=0 && ix.second>=0 && ix.first<GetItemCount() && ix.second<GetColumnCount());
	// actions

	// notify parent with user messages
	if ((pWnd = GetParent())!=NULL )
	{
		pWnd->PostMessage(WM_ListCtrlEx_RBUTTONDOWN,  (WPARAM)this, (LPARAM)&ix);
	}
	// do default action
	CListCtrl::OnRButtonDown(nFlags, point);
	
	//////////////////////////////////////////////////////////////////////////
	// for test
#ifdef _DEBUG
	/*int nHot=GetHotItem();
	vector<int> ivec;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		ivec.push_back(GetNextSelectedItem(pos));
	}
	CString str, strTmp;
	str.Format(_T("Hot item: %d \r\n"), nHot);
	vector<int>::iterator iter=ivec.begin();
	for (; iter!=ivec.end(); ++iter)
	{
		strTmp.Format(_T("Selected item: %d\r\n"), *iter);
		str+=strTmp;
	}
	strTmp.Format(_T("Click point cell: row=%d, col=%d\r\n"), ix.first, ix.second);
	str=_T("Testing: \r\n")+str+strTmp;
	AfxMessageBox(str);*/
#endif
}



void ListCtrlEx::CListCtrlEx::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (!GetCellEnabled(pDispInfo->item.iItem, pDispInfo->item.iSubItem))
	{
		*pResult = 1;
		return;
	}
	*pResult = 0;
}
//extern HWND m_input_dlg_hwnd;

void ListCtrlEx::CListCtrlEx::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	// Update the item text with the new text
	int Changed_item=0;
	int Changed_subitem=0;

	SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
	GetParent()->SendMessage(WM_VALIDATE, GetDlgCtrlID(), (LPARAM)pDispInfo); 
	Changed_item = pDispInfo->item.iItem;
	Changed_subitem = pDispInfo->item.iSubItem;
	::PostMessage(g_hwnd_now,WM_LIST_ITEM_CHANGED,Changed_item,Changed_subitem);
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// static members
int CALLBACK CListCtrlEx::CompareFunc(LPARAM lParam1, LPARAM lParam2, 
						 LPARAM lParamSort)
{
	_SortParam_t *pSortParam_t=reinterpret_cast<_SortParam_t*>(lParamSort);
	if (NULL==pSortParam_t)
		return 0;
	CListCtrlEx *pList=pSortParam_t->m_pTheList;
	ASSERT(pList!=NULL && ::IsWindow(pList->m_hWnd));
	CString strCell1 = pList->GetItemText((int)lParam1, pSortParam_t->m_nSortCol);   
	CString strCell2 = pList->GetItemText((int)lParam2, pSortParam_t->m_nSortCol);   
	int nResult=0;
	try
	{
		switch(pSortParam_t->m_ColParam.m_eSortType)
		{
		default:
		case SortByString:
			nResult=StrCompare(strCell1,strCell2);
			break;
		case SortByDigit:
			nResult=IntCompare(strCell1,strCell2);
		    break;
		}
	}
	catch (...)
	{
		nResult=StrCompare(strCell1,strCell2);
	}

	return (pSortParam_t->m_ColParam.m_bIsAsc?nResult:-nResult);
}

#ifdef _DEBUG
#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG

bool CListCtrlEx::IsDigitStr(LPCTSTR str)
{
	int nLen=(int)_tcslen(str);
	int nPointCnt=0;
	TCHAR ch=0;
	for (int i=0; i<nLen; ++i)
	{
		ch=str[i];
		// for negative
		if (0==i && _T('-')==ch) {
			continue;
		}else if ( 0!=i && _T('-')==ch) {
			return false;
		}

		if (_T('.')==ch) {
			++nPointCnt;
		}

		if (nPointCnt>1) {
			return false;
		}

		if ((ch<48 || ch>57) && _T('.')!=ch) {
			return false;
		}
	}
	return true;
}

int CListCtrlEx::IntCompare(LPCTSTR strInt1st,LPCTSTR strInt2nd)
{
	ASSERT_VALID_STRING( strInt1st );
	ASSERT_VALID_STRING( strInt2nd);
	if (!IsDigitStr(strInt1st) || !IsDigitStr(strInt2nd))
	{
		//AfxThrowInvalidArgException();
		return StrCompare(strInt1st, strInt2nd);
	}

	const int num1st=_ttoi(strInt1st);
	const int num2nd=_ttoi(strInt2nd);
	return (num1st-num2nd);

}

int CListCtrlEx::StrCompare(LPCTSTR str1st, LPCTSTR str2nd)
{
	ASSERT_VALID_STRING( str1st );
	ASSERT_VALID_STRING( str2nd);
	return _tcscmp(str1st, str2nd);
}

BOOL CListCtrlEx::DrawGradientRect(CDC *pDC, CRect &rect, COLORREF crTopLeft, COLORREF crBottomRight)
{
	ASSERT(pDC);
	if (0>=rect.Width() || 0>=rect.Height())
	{
		return FALSE;
	}
	// draw a rect with 45 degree slope gradient color from top left to bottom right
	CBrush brColor(crBottomRight);
	pDC->FillRect(&rect, &brColor);
	int nSteps=max(rect.Width(), rect.Height());
	int rBR=GetRValue(crBottomRight), gBR=GetGValue(crBottomRight), bBR=GetBValue(crBottomRight);
	int rTL=GetRValue(crTopLeft), gTL=GetGValue(crTopLeft), bTL=GetBValue(crTopLeft);
	int rDiff=(rBR-rTL)/nSteps;
	int gDiff=(gBR-gTL)/nSteps;
	int bDiff=(bBR-bTL)/nSteps;

	CRgn rgRect, rgRound, rg2Fill;
	rgRect.CreateRectRgnIndirect(&rect);
	rg2Fill.CreateRectRgn(0,0,0,0);
	for (int i=nSteps; i>=0; --i)
	{
		CRect rcRound(rect.left-i, rect.top-i, rect.left+i, rect.top+i);
		rgRound.CreateEllipticRgnIndirect(&rcRound);
		int nCombineResult =rg2Fill.CombineRgn(&rgRect, &rgRound, RGN_AND);
		if( nCombineResult != ERROR && nCombineResult != NULLREGION ){	
			pDC->FillRgn(&rg2Fill, &brColor);
		}
		rgRound.DeleteObject();
		// calculate new color
		brColor.DeleteObject();
		brColor.CreateSolidBrush(RGB(rTL+i*rDiff, gTL+i*gDiff, bTL+i*bDiff));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ListCtrlEx::CListCtrlEx::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(IsSupportSort())
	{
		vector<DWORD_PTR> vecItemDatas(GetItemCount());
		//map<int, int> mapOldRow2NewRow(GetItemCount());
		PreSortItems(vecItemDatas);
		//SortType eSortBy=GetColumnSortBy(pNMLV->iSubItem);
		BOOL bIsAsc=!IsColumnSortAsc(pNMLV->iSubItem);
		SetColumnSortDirection(pNMLV->iSubItem, bIsAsc);
		_SortParam_t aSortParam;
		aSortParam.m_ColParam=m_mapCol2Sort[pNMLV->iSubItem];
		aSortParam.m_nSortCol=pNMLV->iSubItem;
		aSortParam.m_pTheList=this;
		SortItems(CompareFunc, (DWORD_PTR)&aSortParam);
		//m_ctlSortHead.SetSortArrow(pNMLV->iSubItem, !bIsAsc);
		PostSortItems(vecItemDatas);
	}
	
	*pResult = 0;
}

// before sort items, backup old item data here, then set the item data with old row NO.
// vec2StoreData must be initialized as size of "GetItemCount()"
void CListCtrlEx::PreSortItems( vector<DWORD_PTR> &vec2StoreData )
{
	int nRowCnt=GetItemCount();
	for (int i=0; i<nRowCnt; ++i)
	{
		vec2StoreData[i]=GetItemData(i);
		SetItemData(i, (DWORD_PTR)i);
	}
}

// after sort items, re-mapping the cell data here and restore the old item data here
// vec2StoreData must be initialized as size of "GetItemCount()" and assigned with old item data
void CListCtrlEx::PostSortItems( vector<DWORD_PTR> &vec2StoreData )
{
	// adjust the cell data map
	CellDataMap newMap;
	int nRowCnt=GetItemCount();
	int nColCnt=GetColumnCount();
	for (int i=0; i<nRowCnt; ++i)
	{
		int nOldRow=(int)GetItemData(i);
		for (int j=0; j<nColCnt; ++j)
		{
			newMap[make_pair(i,j)]=m_mapCell2Data[make_pair(nOldRow, j)];	
		}
		// restore item data
		SetItemData(i, (DWORD_PTR)vec2StoreData[nOldRow]);
	}
	m_mapCell2Data.swap(newMap);	
}

void ListCtrlEx::CListCtrlEx::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
//	ASSERT( GetStyle() & LVS_REPORT );	//Fance
	CListCtrl::PreSubclassWindow();
//	VERIFY(m_ctlSortHead.SubclassWindow(this->GetHeaderCtrl()->GetSafeHwnd()) );
}
BOOL	CListCtrlEx::IsColumnSortAsc(int nColIndex)
{
	// insert new if not found
	ColSortMap::iterator iter=m_mapCol2Sort.find(nColIndex);
	if (m_mapCol2Sort.end()==iter)
	{
		m_mapCol2Sort[nColIndex]=_ColumnSort_t();
	}
	return m_mapCol2Sort[nColIndex].m_bIsAsc;
}

void CListCtrlEx::SetColumnSortDirection(int nColIndex, BOOL bIsAsc )
{
	// init sort map
	if (m_mapCol2Sort.empty())
	{
		for (int i=0; i<GetColumnCount(); ++i)
		{
			m_mapCol2Sort[i]=_ColumnSort_t();
		}
	}
	// assign
	m_mapCol2Sort[nColIndex].m_bIsAsc=bIsAsc;
}

void ListCtrlEx::CListCtrlEx::OnDestroy()
{
	CListCtrl::OnDestroy();

	if (::IsWindow(CInPlaceCombo::GetInstance()->m_hWnd))
	{
		CInPlaceCombo::GetInstance()->PostMessage(WM_CLOSE);
	}

	if (::IsWindow(CInPlaceEdit::GetInstance()->m_hWnd))
	{
		CInPlaceEdit::GetInstance()->PostMessage(WM_CLOSE);
	}

}




