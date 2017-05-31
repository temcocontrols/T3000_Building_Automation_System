#include "stdafx.h"
#include "ListCtrlCellEx.h"

using namespace ListCtrlEx;

//////////////////////////////////////////////////////////////////////////
// ------------ for CInPlaceCombo --------------------//
CInPlaceCombo* CInPlaceCombo::m_pInPlaceCombo = NULL; 

CInPlaceCombo::CInPlaceCombo()
{
	m_iRowIndex = -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
}

CInPlaceCombo::~CInPlaceCombo()
{
	if (::IsWindow(m_pInPlaceCombo->m_hWnd))
	{
		SendMessage(WM_CLOSE);
	}
}

BEGIN_MESSAGE_MAP(CInPlaceCombo, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceCombo)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CInPlaceCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);
	SetFocus();
	ResetContent(); 
	CStrList::iterator iter=m_DropDownList.begin();
	for (; iter!=m_DropDownList.end(); ++iter)
	{
		AddString(*iter);
	}

	return 0;
}

BOOL CInPlaceCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;				
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CInPlaceCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : str.GetLength();

	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
	PostMessage(WM_CLOSE);
}

void CInPlaceCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// If the key is "Esc" set focus back to the list control
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
		{
			m_bESC = TRUE;
		}
		GetParent()->SetFocus();
		return;
	}

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CInPlaceCombo::OnCloseup() 
{
	GetParent()->SetFocus();
}

CInPlaceCombo* CInPlaceCombo::GetInstance()
{
	if(m_pInPlaceCombo == NULL)
	{
		m_pInPlaceCombo = new CInPlaceCombo;
	}
	return m_pInPlaceCombo;
}

void CInPlaceCombo::DeleteInstance()
{
	delete m_pInPlaceCombo;
	m_pInPlaceCombo = NULL;
}

BOOL CInPlaceCombo::ShowComboCtrl(DWORD dwStyle, const CRect &rCellRect, CWnd* pParentWnd, UINT uiResourceID,
								  int iRowIndex, int iColumnIndex, CStrList &lstDropDown, CString strCurSel, int iCurSel )
{

	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_bESC = FALSE;

	m_DropDownList.clear();
	m_DropDownList.insert(m_DropDownList.begin(), lstDropDown.begin(), lstDropDown.end());

	BOOL bRetVal = TRUE;

	if (-1 != iCurSel)
	{
		GetLBText(iCurSel, m_strWindowText);
	}
	else if (!strCurSel.IsEmpty()) 
	{
		m_strWindowText = strCurSel;
	}

	if (NULL == m_pInPlaceCombo->m_hWnd) 
	{
		bRetVal = m_pInPlaceCombo->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}

	if (iCurSel >=0 && iCurSel < this->GetCount())
	{
		SetCurSel(iCurSel);
	}
	else
	{
		SelectString(0, strCurSel);
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// --------------- for CInPlaceEdit -------------------//
CInPlaceEdit* CInPlaceEdit::m_pInPlaceEdit=NULL;

CInPlaceEdit::CInPlaceEdit()
{
	m_iRowIndex= -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
	m_strValidChars.Empty();
}

CInPlaceEdit::~CInPlaceEdit()
{
	if (::IsWindow(m_pInPlaceEdit->m_hWnd))
	{
		SendMessage(WM_CLOSE);
	}
}

BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_MESSAGE(WM_PASTE, CInPlaceEdit::OnPaste)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

LRESULT CInPlaceEdit::OnPaste(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!m_strValidChars.IsEmpty())
	{
		CString strFromClipboard;
		// get the text from clipboard
		if(OpenClipboard()) {
			HANDLE l_hData = GetClipboardData(CF_TEXT);
			if(NULL == l_hData) {
				return 1;
			}

			char *l_pBuffer = (char*)GlobalLock(l_hData);
			if(NULL != l_pBuffer) {
				strFromClipboard = l_pBuffer;
			}

			GlobalUnlock(l_hData);
			CloseClipboard();
		}
		// Validate the characters before pasting 
		for(int iCounter_ = 0; iCounter_ < strFromClipboard.GetLength(); iCounter_++)
		{
			if (-1 == m_strValidChars.Find(strFromClipboard.GetAt(iCounter_)))
			{
				return 1;
			}
		}
	}

	//let the individual control handle other processing
	CEdit::Default();	
	return 1;	
}

void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	CString strEdit;
	GetWindowText(strEdit);

	// Send Notification to parent of edit ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex; 
	// escape key is down so use old string
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)strEdit);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : strEdit.GetLength();

	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
	PostMessage(WM_CLOSE);
}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	 
	if ( (m_strValidChars.IsEmpty()) // no valid chars
		|| ((-1 != m_strValidChars.Find(static_cast<TCHAR> (nChar)))
		|| (nChar == VK_BACK) || (nChar == CTRL_C) || (nChar == CTRL_V) || (nChar == CTRL_X)))
	{
		CString temp_cs;
		CInPlaceEdit::GetWindowText(temp_cs);
		TRACE(temp_cs + _T("\r\n"));
		if((temp_cs.GetLength()>= m_charlimit) && (m_charlimit != 0) && (nChar!= 8))
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return;
		}
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}
}

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (WM_KEYDOWN == pMsg->message && (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam))
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			m_bESC = TRUE;
		}

		GetParent()->SetFocus();
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);

	ShowWindow(SW_SHOW);
	SetWindowText(m_strWindowText);
	SetSel(0, -1);
	SetFocus();

	return 0;
}

CInPlaceEdit* CInPlaceEdit::GetInstance()
{
	if(m_pInPlaceEdit == NULL)
	{
		m_pInPlaceEdit = new CInPlaceEdit;
	}
	return m_pInPlaceEdit;
}

void CInPlaceEdit::DeleteInstance()
{
	delete m_pInPlaceEdit;
	m_pInPlaceEdit = NULL;
}
void CInPlaceEdit::SetEditLimit(int ncount)
{
	m_charlimit = ncount;
}
BOOL CInPlaceEdit::ShowEditCtrl(DWORD dwStyle, const RECT &rCellRect, CWnd* pParentWnd, 
								UINT uiResourceID, int iRowIndex, int iColumnIndex,
								CString& strValidChars, CString& rstrCurSelection)
{
	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_strValidChars = strValidChars;
	m_strWindowText = rstrCurSelection;
	m_bESC = FALSE;

	if (NULL == m_pInPlaceEdit->m_hWnd) 
	{
		return m_pInPlaceEdit->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}	

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CListCtrlExSortHead

IMPLEMENT_DYNAMIC(CListCtrlExSortHead, CHeaderCtrl)

CListCtrlExSortHead::CListCtrlExSortHead()
: m_iSortColumn(-1),m_bSortDesc(FALSE)
{

}

CListCtrlExSortHead::~CListCtrlExSortHead()
{

}


BEGIN_MESSAGE_MAP(CListCtrlExSortHead, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlExSortHead::OnNMCustomdraw)
END_MESSAGE_MAP()



// CSortHeadCtrl message handlers
void CListCtrlExSortHead::SetSortArrow( const int nSortColumn, const BOOL bDesc )
{
	if (nSortColumn<0 || nSortColumn>=this->GetItemCount())
	{
		ASSERT(FALSE);
	}
	m_iSortColumn=nSortColumn;
	m_bSortDesc=bDesc;

	// change the item to owner drawn.
	HD_ITEM hditem;

	hditem.mask = HDI_FORMAT;
	VERIFY( GetItem( nSortColumn, &hditem ) );
	hditem.fmt |= HDF_OWNERDRAW;
	VERIFY( SetItem( nSortColumn, &hditem ) );

	// invalidate the header control so it gets redrawn
	Invalidate();
	//UpdateWindow();
}

inline int	 CListCtrlExSortHead::GetCrntSortCol() const
{
	return m_iSortColumn;
}

inline BOOL CListCtrlExSortHead::GetCrntSortDirection() const
{
	return m_bSortDesc;
}

void CListCtrlExSortHead::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// Get the column rect
	CRect rcLabel(lpDrawItemStruct->rcItem);

	// Draw the background
	CBrush br(::GetSysColor(COLOR_3DFACE));
	pDC->FillRect(rcLabel, &br);	

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1).cx*2;

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;	

	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;

	GetItem(lpDrawItemStruct->itemID, &hditem);

	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS ;

	if (hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;
	else if (hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;

	// Adjust the rect if the mouse button is pressed on it
	if (lpDrawItemStruct->itemState == ODS_SELECTED)
	{
		rcLabel.left++;
		rcLabel.top += 2;
		rcLabel.right++;
	}

	// Adjust the rect further if Sort arrow is to be displayed
	if (lpDrawItemStruct->itemID == static_cast<UINT>(m_iSortColumn))
	{
		rcLabel.right -= 3 * offset;
	}

	rcLabel.left += offset;
	rcLabel.right -= offset;

	// Draw column label
	pDC->DrawText(buf, -1, rcLabel, uFormat);

	// Draw the Sort arrow
	if (lpDrawItemStruct->itemID == static_cast<UINT>(m_iSortColumn))
	{
		CRect rcIcon(lpDrawItemStruct->rcItem);

		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
		CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		CPen *pOldPen = pDC->SelectObject(&penLight);

		if (!m_bSortDesc)
		{
			// Draw triangle pointing upwards
			pDC->MoveTo(rcIcon.right - 2*offset, offset - 1);
			pDC->LineTo(rcIcon.right - 3*offset/2, rcIcon.bottom - offset);
			pDC->LineTo(rcIcon.right - 5*offset/2-2, rcIcon.bottom - offset);
			pDC->MoveTo(rcIcon.right - 5*offset/2-1, rcIcon.bottom - offset - 1);

			pDC->SelectObject(&penShadow);
			pDC->LineTo(rcIcon.right - 2*offset, offset-2);
		}
		else	
		{
			// Draw triangle pointing downwards
			pDC->MoveTo(rcIcon.right - 3*offset/2, offset-1);
			pDC->LineTo(rcIcon.right - 2*offset-1, rcIcon.bottom - offset + 1);
			pDC->MoveTo(rcIcon.right - 2*offset-1, rcIcon.bottom - offset);

			pDC->SelectObject(&penShadow);
			pDC->LineTo(rcIcon.right - 5*offset/2-1, offset - 1);
			pDC->LineTo(rcIcon.right - 3*offset/2, offset - 1);
		}

		// Restore the pen
		pDC->SelectObject(pOldPen);
	}

}


void ListCtrlEx::CListCtrlExSortHead::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	switch(pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		*pResult = CDRF_DODEFAULT;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		*pResult = CDRF_DODEFAULT;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:			// recd when CDRF_NOTIFYSUBITEMDRAW is returned in
		*pResult = CDRF_DODEFAULT;
		break;
	default:														// it wasn't a notification that was interesting to us.
		*pResult = CDRF_DODEFAULT;
		break;
	}
	*pResult = 0;
}
