// ==========================================================================
//                     Class Implementation : COXToolTipCtrl
// ==========================================================================

// Source file : OXToolTipCtrl.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is ?1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
              
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXToolTipCtrl.h"
#include "multimon.h"
#include "UTB64Bit.h"

 
#define COMPILE_MULTIMON_STUBS
 

#pragma warning(disable : 4706)

#include <multimon.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//  Add message handlers (eg TTM_SETTIPBKCOLOR) and point them 
//       to the correct functions.

LPCTSTR COXToolTipCtrl::m_szArrowSpace = _T("  ");

/////////////////////////////////////////////////////////////////////////////
// COXToolTipCtrl construction

COXToolTipCtrl::COXToolTipCtrl() :
    m_pParentWnd(NULL),
    m_rectMargin(2,2,3,3),
    m_nMaxWidth(0),
    m_ptOffset(0,20),
    m_pCurrentToolTip(NULL),
    m_nCheckInterval(500),          // Time between checks of the tooltip - allows 
                                    // user to move the mouse to the tooltip before 
                                    // it disappears.  
    m_nDisplayDelay(500),           // Delay in milliseconds until the tooltip is 
                                    // displayed.
    m_nDisplayTime(5000),           // Length of time the tooltip is displayed (mS).
    m_nElapsedTime(0),
    m_bActivated(TRUE),
    m_bTipCancelled(FALSE),
    m_bHasExtendedText(FALSE),
    m_hOldFocusWnd(NULL),
    m_crBackColor(CLR_DEFAULT),
    m_crTextColor(CLR_DEFAULT),
    m_bUsingSystemFont(TRUE),
    m_dwTextStyle(DT_EXPANDTABS|DT_EXTERNALLEADING|DT_NOPREFIX|DT_WORDBREAK)
{
    m_arrTools.RemoveAll();
}

COXToolTipCtrl::~COXToolTipCtrl()
{
    m_Font.DeleteObject();

    COXToolTipInfo *pInfo = NULL;
    int nSize = PtrToInt(m_arrTools.GetSize());
    for (int nIndex = 0; nIndex < nSize; nIndex++)
    {
        pInfo = (COXToolTipInfo* )m_arrTools.GetAt(nIndex);
        delete pInfo;
    }

    m_arrTools.RemoveAll();

    if (IsWindow(m_hWnd))
        DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// COXToolTipCtrl message handlers

BEGIN_MESSAGE_MAP(COXToolTipCtrl, CWnd)
    //{{AFX_MSG_MAP(COXToolTipCtrl)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETFOCUS()
    ON_WM_DESTROY()
    ON_WM_SETTINGCHANGE()
    ON_WM_MOUSEACTIVATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
END_MESSAGE_MAP()

// --- In?:
// --- Out :
// --- Returns :
// --- Effect : Draws the tooltip - called in response to a WM_PAINT message
void COXToolTipCtrl::OnPaint() 
{
    if (!m_pCurrentToolTip)
        return;

    CString str = GetTooltipText(m_pCurrentToolTip);
    if (str.IsEmpty())
        return;

    CPaintDC dc(this); // Device context for painting

    CRect rect;
    GetClientRect(rect);

    CBrush  Brush, *pOldBrush;
    if (m_pCurrentToolTip->clrBackColor == CLR_DEFAULT)
        Brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));
    else
        Brush.CreateSolidBrush(m_pCurrentToolTip->clrBackColor);
    pOldBrush = dc.SelectObject(&Brush);

    if (m_pCurrentToolTip->clrTextColor == CLR_DEFAULT)
        dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
    else
        dc.SetTextColor(m_pCurrentToolTip->clrTextColor);

    CFont *pOldFont = dc.SelectObject(&m_Font);
    
    // Draw Border
    dc.FillRect(&rect, &Brush);
    dc.SelectStockObject(NULL_BRUSH);
    dc.SelectStockObject(BLACK_PEN);
    dc.Rectangle(rect);

    // Draw Text
    dc.SetBkMode(TRANSPARENT);
    rect.DeflateRect(m_rectMargin);

    dc.DrawText(str, rect, m_dwTextStyle);

    if (m_bHasExtendedText)
    {
        TEXTMETRIC tm;
        dc.GetTextMetrics(&tm);
        int nYMargin = max(tm.tmExternalLeading + tm.tmInternalLeading, tm.tmDescent);
        int nXMargin = tm.tmAveCharWidth / 2;

        CSize size = dc.GetTextExtent(m_szArrowSpace);
        if ((size.cy & 1))
            size.cy--;

        CPoint pt[4];
        if (m_bExtended)
        {
            pt[0] = CPoint(rect.left + size.cx - nXMargin, rect.top + nYMargin);
            pt[1] = CPoint(rect.left, rect.top + size.cy/2);
            pt[2] = CPoint(rect.left + size.cx - nXMargin, rect.top + size.cy - nYMargin);
            pt[3] = pt[0];
        }
        else
        {
            pt[0] = CPoint(rect.right - size.cx + nXMargin, rect.top + nYMargin);
            pt[1] = CPoint(rect.right, rect.top + size.cy/2);
            pt[2] = CPoint(rect.right - size.cx + nXMargin, rect.top + size.cy - nYMargin);
            pt[3] = pt[0];
        }
        dc.SelectStockObject(BLACK_BRUSH);
        dc.Polygon(pt, 4);
    }

    // Cleanup
    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldFont);
}

// --- In?: nIDEvent - The timer event
// --- Out :
// --- Returns :
// --- Effect : Timer events are used to either Activate the tooltip after 
//              the mouse has been stationary for the specified time, remove
//              the tip after a specified display time, or periodically check
//              that the mouse is still within the bounds of the tool.
void COXToolTipCtrl::OnTimer(UINT nIDEvent) 
{
    CPoint pt;
    COXToolTipInfo *pToolTip = NULL;

    if (!m_pCurrentToolTip || m_bTipCancelled)
    {
        KillTimer(nIDEvent);
        Pop();
        return;
    }

    switch (nIDEvent) 
    {
        // The mouse has been still for sufficient time. If it's still in the
        // initial tool, then show the tooltip for that tool.
        case eIDDisplayToolEvent:
            KillTimer(eIDDisplayToolEvent);
            if (IsCursorInTool(m_pCurrentToolTip))
            {
                CPoint pt;
                GetCursorPos(&pt);
                pt += m_ptOffset;
                DisplayToolTip(pt);
                m_nElapsedTime = 0;
                SetTimer(eIDCheckToolEvent, m_nCheckInterval, NULL);
            }
			else
			{
				m_pCurrentToolTip = NULL;
			}
            break;

        // The tooltip is visible, check to see if: (a) it has been visible too long, and 
        // (b) is still in the bounding rect (or tooltip window). If all is well then
        // reset the egg timer and check again later.
        case eIDCheckToolEvent:
            m_nElapsedTime += m_nCheckInterval;

            GetCursorPos(&pt);
            m_pParentWnd->ScreenToClient(&pt);
            pToolTip = FindToolFromPoint(pt);

            // Check that the cursor isn't over a new tool
            if (pToolTip && pToolTip != m_pCurrentToolTip)
            {
                KillTimer(eIDCheckToolEvent);
                Pop();
                return;
            }

            if (!IsCursorInTool(m_pCurrentToolTip) || (m_nElapsedTime >= m_nDisplayTime))
            {
                if (IsCursorInToolTip())
                {
                    //TRACE0("Cursor in tooltip - will check later\n");
                    //SetTimer(eIDCheckToolEvent, m_nCheckInterval, NULL);
                }
                else
                {
                    //TRACE0("Not in tool or tooltip anymore, or expired. Destroying\n");
                    KillTimer(eIDCheckToolEvent);
                    Pop();
                    //m_pCurrentToolTip = NULL;
                }
            }
            else
            {
                //TRACE0("Everything's OK - will check later\n");
                //SetTimer(eIDCheckToolEvent, m_nCheckInterval, NULL);
            }
            
            break;

        default:
            ASSERT(FALSE);
    }
}

// --- In?: nFlags - Unused
//           point -  Unused
// --- Out :
// --- Returns :
// --- Effect : Causes a switch between standard and extended info viewing
void COXToolTipCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
    // Sometimes a click comes through the pipeline after the tool
    // has already been removed
    if (!m_pCurrentToolTip || m_bTipCancelled)
        return;

    if (m_bHasExtendedText)
    {
        CRect rect;
        GetWindowRect(rect);
        DisplayToolTip(rect.TopLeft(), !m_bExtended);
    }

    if (m_hOldFocusWnd)
        ::SetFocus(m_hOldFocusWnd);

    //CWnd::OnLButtonDown(nFlags, point);
}

// --- In?: pOldWnd - Contains the CWnd object that loses the input focus 
//                    (may be NULL). 
// --- Out :
// --- Returns :
// --- Effect : Restores the focus back to the previous window (the tooltip does 
//              not need the focus at all)
void COXToolTipCtrl::OnSetFocus(CWnd* pOldWnd) 
{
    CWnd::OnSetFocus(pOldWnd);

    m_hOldFocusWnd = pOldWnd->GetSafeHwnd();
}

int COXToolTipCtrl::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, 
                                    UINT /*message*/)
{
    return MA_NOACTIVATE;
}

// --- In?:
// --- Out :
// --- Returns :
// --- Effect : Kills off any remaining timers
void COXToolTipCtrl::OnDestroy() 
{
    KillTimer(eIDDisplayToolEvent);
    KillTimer(eIDCheckToolEvent);

    CWnd::OnDestroy();
}

// --- In?: wFlag - System-wide parameter flag (see WM_SETTINGCHANGE).
//           lpszSection - Name of changed section or registry changes.
// --- Out :
// --- Returns :
// --- Effect : If the tooltip font has not been overriden, this updates the
//              font with the new system tooltip font
void COXToolTipCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    CWnd::OnSettingChange(uFlags, lpszSection);
    
    if (m_bUsingSystemFont)
        SetLogFont(GetSystemToolTipFont(), TRUE);
}

// --- In?: hFont - Specifies a handle to the new font.
// --- Out :
// --- Returns : No return value
// --- Effect : Sets the new tooltip font
LRESULT COXToolTipCtrl::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    LRESULT result = Default();

    // Get the logical font from the supplied handle
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
    {
        SetLogFont(GetSystemToolTipFont());
        return result;
    }
    
    SetLogFont(&lf, TRUE);

    return result;
}

// --- In?: The parameters are not used
// --- Out :
// --- Returns : The return value is a handle to the font used by the control
// --- Effect : 
LRESULT COXToolTipCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return (LRESULT) (HFONT) m_Font;
}

/////////////////////////////////////////////////////////////////////////////
// COXToolTipCtrl operations

BOOL COXToolTipCtrl::Create(CWnd* pParentWnd)
{
    m_pParentWnd = pParentWnd;

    // Get the class name and create the window
    CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS, 
                                              LoadCursor(NULL, IDC_ARROW));

    // Create the window - just don't show it yet.
    if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
                        WS_POPUP, 
                        0, 0, 10, 10, // Size & position updated when needed
                        pParentWnd->GetSafeHwnd(), 0, NULL))
    {
        return FALSE;
    }
   
    SetLogFont(GetSystemToolTipFont(), FALSE);

    return TRUE;
}

void COXToolTipCtrl::RelayEvent(MSG* pMsg)
{
    ASSERT(m_pParentWnd);

    if (pMsg->message == WM_MOUSEMOVE)
    {
        CPoint pt = pMsg->pt;
        m_pParentWnd->ScreenToClient(&pt);

        // Get the tool under the cursor
        COXToolTipInfo *pToolTip = FindToolFromPoint(pt);

        // If the tip has been cancelled and the mouse has moved into a "no tool"
        // area, then we can reset the current tool in order for a new tool to be
        // set as soon as it goes back into tool territory. 
		if (!pToolTip && ( !m_bHasExtendedText || m_bTipCancelled ))
		{
			m_pCurrentToolTip = NULL;
		}
		
        // If no tooltip (we are in a no tooltip fly zone), but we have a potential
        // tooltip, then just relax and let the timer do it's thing.
        if (!pToolTip || (m_pCurrentToolTip && (pToolTip == m_pCurrentToolTip)))
            return;

        // First see if we have a current tooltip, then see that it is
        // still in the current tool or tooltip (if not, dismiss it)
        if (m_pCurrentToolTip != NULL)
        {
            // Cursor over tooltip?
            if (IsCursorInToolTip())
                return;

            if (pToolTip && (pToolTip != m_pCurrentToolTip))
            {
                StartNewTool(pToolTip);
                return;
            }

            // Cursor over tool?
            //if (!IsCursorInTool(m_pCurrentToolTip))
            //    return; // Allow the timer to deal with this instead of killing now
        }
        else // m_pCurrentToolTip == NULL
        {
            // If we have a tool under the cursor then prepare to display
            if (pToolTip)
            {
                StartNewTool(pToolTip);
                return;
            }
        }
    }
    else if (pMsg->message == WM_LBUTTONDOWN)
    {
        if (!IsCursorInToolTip())
            Pop();
    }
    else if ((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) ||
             //(pMsg->message >= WM_SYSKEYFIRST && pMsg->message <= WM_SYSKEYLAST) ||
             (/*pMsg->message == WM_LBUTTONDOWN ||*/ pMsg->message == WM_LBUTTONDBLCLK) ||
             (pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_RBUTTONDBLCLK) ||
             (pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_MBUTTONDBLCLK) ||
             (pMsg->message == WM_NCLBUTTONDOWN || pMsg->message == WM_NCLBUTTONDBLCLK) ||
             (pMsg->message == WM_NCRBUTTONDOWN || pMsg->message == WM_NCRBUTTONDBLCLK) ||
             (pMsg->message == WM_NCMBUTTONDOWN || pMsg->message == WM_NCMBUTTONDBLCLK))
    {
        // The user has interrupted the current tool - dismiss it
        Pop();
    }
}

BOOL COXToolTipCtrl::AddTool(CWnd* pWnd, UINT nIDText, LPCRECT lpRectTool /*=NULL*/, 
                             UINT nIDTool /*=0*/)
{
    CString str;
    str.LoadString(nIDText);
    return AddTool(pWnd, (LPCTSTR) str, lpRectTool, nIDTool);
}

BOOL COXToolTipCtrl::AddTool(CWnd* pWnd, LPCTSTR lpszText, 
                             LPCRECT lpRectTool /*=NULL*/,
                             UINT nIDTool /*=0*/)
{
    ASSERT(pWnd);

    if (!pWnd)
        return FALSE;

    COXToolTipInfo *pToolTip = new COXToolTipInfo;
    ASSERT( pToolTip != NULL );

    if (pToolTip == NULL)
        return FALSE;

    // Fill the tooltip structure
    pToolTip->hWnd         = pWnd->GetSafeHwnd();
    pToolTip->nIDTool      = nIDTool;
    if (lpszText == LPSTR_TEXTCALLBACK)
    {
        pToolTip->szText = lpszText;
        pToolTip->strText.Empty();
    }
    else
    {
        pToolTip->szText = NULL;
        pToolTip->strText = lpszText;
    }
    pToolTip->clrBackColor = m_crBackColor;
    pToolTip->clrTextColor = m_crTextColor;
    pToolTip->nWidth       = m_nMaxWidth;
    pToolTip->lParam       = 0;

    // Get bounding region for tooltip info
    CRect rect;
    if (lpRectTool)
    {
        rect = lpRectTool;
        pWnd->ClientToScreen(rect);
    }
    else
    {
        pWnd->GetWindowRect(rect);
    }
    m_pParentWnd->ScreenToClient(rect);
    pToolTip->rectBounds = rect;
        
    // Add to the list
    m_arrTools.Add(pToolTip);

    return TRUE;
}

void COXToolTipCtrl::DelTool(CWnd* pWnd, UINT nIDTool /*=0*/)
{
    for (int i = 0; i < m_arrTools.GetSize(); i++)
    {
        COXToolTipInfo* pToolInfo = (COXToolTipInfo*) m_arrTools.GetAt(i);
        if (!pToolInfo)
            continue;

        if (pWnd->GetSafeHwnd() == pToolInfo->hWnd && 
            nIDTool == pToolInfo->nIDTool)
        {
            if (pToolInfo == m_pCurrentToolTip)
            {
                Pop();
                m_pCurrentToolTip = NULL;
            }
            delete pToolInfo;
            m_arrTools.RemoveAt(i, 1);
        }
    }
}

BOOL COXToolTipCtrl::UpdateToolRect(CWnd* pWnd, LPCRECT lpRectTool /*=NULL*/, UINT nIDTool /*=0*/)
{
    for (int i = 0; i < m_arrTools.GetSize(); i++)
    {
        COXToolTipInfo* pToolInfo = (COXToolTipInfo*) m_arrTools.GetAt(i);
        if (!pToolInfo)
            continue;

        if (pWnd->GetSafeHwnd() == pToolInfo->hWnd && 
            nIDTool == pToolInfo->nIDTool)
        {
            if (pToolInfo == m_pCurrentToolTip)
            {
                Pop();
                m_pCurrentToolTip = NULL;
            }

			// Get bounding region for tooltip info
			CRect rect;
			if (lpRectTool)
			{
				rect = lpRectTool;
				pWnd->ClientToScreen(rect);
			}
			else
			{
				pWnd->GetWindowRect(rect);
			}
			m_pParentWnd->ScreenToClient(rect);
			pToolInfo->rectBounds = rect;
			return TRUE;
        }
    }

	return FALSE;
}

void COXToolTipCtrl::GetText(CString& str, CWnd* pWnd, UINT nIDTool /*=0*/)
{
    COXToolTipInfo* pToolInfo = GetToolInfoPtr(pWnd, nIDTool);
    if (pToolInfo)
        str = GetTooltipText(pToolInfo);
}

void COXToolTipCtrl::GetMargin( LPRECT lprc) const
{
    ASSERT(lprc);
    lprc->top    = m_rectMargin.top;
    lprc->left   = m_rectMargin.left;
    lprc->bottom = m_rectMargin.bottom;
    lprc->right  = m_rectMargin.right;
}

void COXToolTipCtrl::SetDelayTime(DWORD dwDuration, int nTime)
{
    switch (dwDuration)
    {
        case TTDT_AUTOPOP: 
            m_nDisplayTime = nTime;
            break;
        case TTDT_INITIAL:
            m_nDisplayDelay = nTime;
            break;
        case TTDT_RESHOW:
        default:
            ASSERT(FALSE);
    }
}

int COXToolTipCtrl::GetDelayTime(DWORD dwDuration) const
{
    switch (dwDuration)
    {
        case TTDT_AUTOPOP: 
            return m_nDisplayTime;
        case TTDT_INITIAL: 
            return m_nDisplayDelay;
        case TTDT_RESHOW:
        default:
            ASSERT(FALSE);
            return 0;
    }
}

int COXToolTipCtrl::GetMaxTipWidth() const
{
    return m_nMaxWidth;
}

int COXToolTipCtrl::SetMaxTipWidth(int nWidth)
{
    int nOldWidth = m_nMaxWidth;
    m_nMaxWidth = nWidth;

    // Update the width for all tooltips whose width is standard.
    // Non-standard widths have obviously been modified outside
    // of this function, so leave them be.
    int nSize = PtrToInt(m_arrTools.GetSize());
    for (int i = 0; i < nSize; i++)
    {
        COXToolTipInfo* pTool = (COXToolTipInfo*) m_arrTools.GetAt(i);
        if (!pTool)
            continue;

        if (pTool->nWidth == nOldWidth)
            pTool->nWidth = nWidth;
    }

    return nOldWidth;
}

COLORREF COXToolTipCtrl::GetTipBkColor() const
{
    return m_crBackColor;
}

void COXToolTipCtrl::SetTipBkColor(COLORREF clr)
{
    // Update the width for all tooltips whose width is standard.
    // Non-standard widths have obviously been modified outside
    // of this function, so leave them be.
    int nSize = PtrToInt(m_arrTools.GetSize());
    for (int i = 0; i < nSize; i++)
    {
        COXToolTipInfo* pTool = (COXToolTipInfo*) m_arrTools.GetAt(i);
        if (!pTool)
            continue;

        if (pTool->clrBackColor == m_crBackColor)
            pTool->clrBackColor = clr;
    }
    m_crBackColor = clr;
}

COLORREF COXToolTipCtrl::GetTipTextColor() const
{
    return m_crTextColor;
}

void COXToolTipCtrl::SetTipTextColor(COLORREF clr)
{
    // Update the width for all tooltips whose width is standard.
    // Non-standard widths have obviously been modified outside
    // of this function, so leave them be.
    int nSize = PtrToInt(m_arrTools.GetSize());
    for (int i = 0; i < nSize; i++)
    {
        COXToolTipInfo* pTool = (COXToolTipInfo*) m_arrTools.GetAt(i);
        if (!pTool)
            continue;

        if (pTool->clrTextColor == m_crTextColor)
            pTool->clrTextColor = clr;
    }
    m_crTextColor = clr;
}

BOOL COXToolTipCtrl::HitTest(HWND pWnd, POINT pt, OXTOOLINFO* pToolInfo) const
{
    if (!pWnd)
        return FALSE;

    COXToolTipInfo* pTool = NULL;

    // Loop through all registered tool tips to check if we should display one.
	// In order to support multiple tool tips per window we will check
	// all registered tool tips to make sure if there is a need for a tool tip.
    int nSize = PtrToInt(m_arrTools.GetSize());

    for (int tipIndex = 0; tipIndex < nSize; tipIndex++)
    {
        pTool = (COXToolTipInfo*) m_arrTools.GetAt( tipIndex );
        if (!pTool)
			// the tool tip found is not valid, continue to the next
            continue;

		if( pWnd == pTool->hWnd )
		{	// The window has a tool - but is the cursor currently over it?
			if ( pTool->rectBounds.PtInRect(pt) != FALSE )
			{
				if (pToolInfo)
					*pToolInfo = *pTool;

				return TRUE;
			}
		}
		else if ( ::IsChild(pWnd, pTool->hWnd ) && pTool->rectBounds.PtInRect(pt) != FALSE )
		{	// The user's mouse is over a child window, make a recursive call to HitTest
			// function in order to find the proper window that the user has the mouse over.
			return HitTest( pTool->hWnd, pt, pToolInfo );
		}
    }

    // No tool in window - return 
    return FALSE;
}

void COXToolTipCtrl::Pop()
{
    ShowWindow(SW_HIDE);
    m_bTipCancelled = TRUE;
	m_pCurrentToolTip = NULL;
}

BOOL COXToolTipCtrl::GetToolInfo(OXTOOLINFO& ToolInfo, CWnd* pWnd, 
                                 UINT nIDTool /*=0*/)
{
    COXToolTipInfo *pToolInfo = GetToolInfoPtr(pWnd, nIDTool);
    if (pToolInfo == NULL)
        return FALSE;

    ToolInfo = *pToolInfo;

    return TRUE;
}

void COXToolTipCtrl::SetToolInfo(OXTOOLINFO* pToolInfo)
{
    COXToolTipInfo *pInfo = GetToolInfoPtr(CWnd::FromHandle(pToolInfo->hwnd), 
                                        pToolInfo->uId);

    if (pToolInfo != NULL)
        *pInfo = *pToolInfo;
}

/////////////////////////////////////////////////////////////////////////////
// COXToolTipCtrl implementation

CString COXToolTipCtrl::GetTooltipText(COXToolTipInfo *pToolTip)
{
    CString strTooltipText(_T(""));

    if (!pToolTip)
        return strTooltipText;

    if (pToolTip->szText == LPSTR_TEXTCALLBACK)
    {
        NMTTDISPINFO nmtt;
        nmtt.hdr.hwndFrom = m_hWnd;
        nmtt.hdr.idFrom   = ::GetDlgCtrlID(pToolTip->hWnd);
        nmtt.hdr.code     = TTN_NEEDTEXT;
        nmtt.lpszText     = NULL;
        nmtt.hinst        = NULL;
        nmtt.uFlags       = 0;

        m_pParentWnd->SendMessage(WM_NOTIFY, (WPARAM) pToolTip->nIDTool, (LPARAM) &nmtt);

        if (nmtt.hinst != NULL)
        {
            TCHAR sz[512];

            UINT nLen = PtrToUint(::LoadString(nmtt.hinst, 
				PtrToUint((UINT_PTR)nmtt.lpszText), 
				sz, 
				512));
            ASSERT(nLen < 511);
            if (nLen > 0)
                strTooltipText = sz;
            else
                strTooltipText.Empty();
        }
        else if (nmtt.lpszText)
            strTooltipText = nmtt.lpszText;
        else
            strTooltipText = nmtt.szText;
    }
    else
        strTooltipText = pToolTip->strText;

    m_bHasExtendedText = (strTooltipText.Find(_T('\r')) != -1);

    if (m_bHasExtendedText)
    {
        if (m_bExtended)
            strTooltipText = m_szArrowSpace + GetFieldFromString(strTooltipText, 1, _T('\r'));
        else
            strTooltipText = GetFieldFromString(strTooltipText, 0, _T('\r')) + m_szArrowSpace;
    }

    return strTooltipText;
}

LPLOGFONT COXToolTipCtrl::GetSystemToolTipFont() const
{
    static LOGFONT LogFont;

    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0))
        return FALSE;

    memcpy(&LogFont, &(ncm.lfStatusFont), sizeof(LOGFONT));

    return &LogFont; 
}

BOOL COXToolTipCtrl::SetLogFont(LPLOGFONT lpLogFont, BOOL bRedraw /*=TRUE*/)
{
    ASSERT(lpLogFont);
    if (!lpLogFont)
        return FALSE;

    // Store font as the global default
    memcpy(&m_LogFont, lpLogFont, sizeof(LOGFONT));

    // If the new font is not the same as the system font, then we will no
    // longer update the font in the event of a System Settings Change.
    LPLOGFONT lpSysFont = GetSystemToolTipFont();

    // Not reliable
    //m_bUsingSystemFont = (memcmp(&m_LogFont, lpSysFont, sizeof(LOGFONT)) == 0);
    m_bUsingSystemFont = (m_LogFont.lfWeight == lpSysFont->lfWeight &&
                          m_LogFont.lfItalic == lpSysFont->lfItalic &&
                          m_LogFont.lfHeight == lpSysFont->lfHeight &&
                          m_LogFont.lfCharSet == lpSysFont->lfCharSet &&
                          !_tcscmp(m_LogFont.lfFaceName, lpSysFont->lfFaceName));
    
    // Create the actual font object
    m_Font.DeleteObject();
    m_Font.CreateFontIndirect(&m_LogFont);

    if (bRedraw && ::IsWindow(GetSafeHwnd())) 
        Invalidate();

    return TRUE;
}

CRect COXToolTipCtrl::GetBoundsRect(CString strText, int nWidth) const
{
    CWindowDC dc(NULL);

    CFont *pOldFont = (CFont*) dc.SelectObject((CFont*)&m_Font); 

    int nLineWidth = nWidth;
   
    if (nLineWidth == 0)
    {
        // Count the number of lines of text
        int nStart = 0, nNumLines = 0;
        CString strTextCopy=strText;
        do {
            nStart = strTextCopy.Find(_T("\n"));

            // Skip found character 
            if (nStart >= 0)
                strTextCopy=strTextCopy.Mid(nStart+1);

            nNumLines++;
        } while (nStart > 0);

        // Find the widest line
        for (int i = 0; i < nNumLines; i++)
        {
            CString strLine = GetFieldFromString(strText, i, _T('\n')) + _T("  ");
            nLineWidth = max(nLineWidth, dc.GetTextExtent(strLine).cx);
        }
    }

    CRect rect(0,0, max(0,nLineWidth), 0);
    dc.DrawText(strText, rect, DT_CALCRECT | m_dwTextStyle);

    rect.bottom += m_rectMargin.top + m_rectMargin.bottom;
    rect.right += m_rectMargin.left + m_rectMargin.right;

    dc.SelectObject(pOldFont);

    return rect;
}

CRect COXToolTipCtrl::CalculateInfoBoxRect(CPoint& pt, COXToolTipInfo* pToolTip, 
                                           CRect& rectTextBounds) const
{  
    CRect InfoRect = rectTextBounds;
    InfoRect.OffsetRect(-InfoRect.TopLeft());

    InfoRect.OffsetRect(pt);
    // Need to check if it will fit on the monitor
	// Get the rectangle of the closest monitor
	HMONITOR hMonitor = ::MonitorFromRect(InfoRect,MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitor, &mi);
	// calculate the monitor's range
	const int iMixScreenX = mi.rcMonitor.left;
	const int iMaxScreenX = mi.rcMonitor.right;
	const int iMaxScreenY = mi.rcMonitor.bottom;

    // Too far right?
    if (InfoRect.right > iMaxScreenX)
        InfoRect.OffsetRect(-(InfoRect.right - iMaxScreenX), 0);

    // Too far left?
    if (InfoRect.left < iMixScreenX)
        InfoRect.OffsetRect( -InfoRect.left, iMixScreenX);

    // Bottom falling out of screen?
    if (InfoRect.bottom > iMaxScreenY)
    {
        CRect ParentRect;
        ::GetWindowRect(pToolTip->hWnd, ParentRect);
        int nHeight = InfoRect.Height();
        InfoRect.top = ParentRect.top - nHeight;
        InfoRect.bottom = InfoRect.top + nHeight;
    }
    
    return InfoRect;
}

void COXToolTipCtrl::StartNewTool(COXToolTipInfo* pToolInfo)
{
    KillTimer(eIDDisplayToolEvent);
    KillTimer(eIDCheckToolEvent);

    Pop();
    m_bTipCancelled = FALSE;
    m_pCurrentToolTip = pToolInfo;

    SetTimer(eIDDisplayToolEvent, m_nDisplayDelay, NULL);
}

BOOL COXToolTipCtrl::IsCursorInTool(COXToolTipInfo* pToolInfo) const
{
    ASSERT(m_pParentWnd);

    CPoint pt;
    GetCursorPos(&pt);
    m_pParentWnd->ScreenToClient(&pt);

    return HitTest( FromHandle(pToolInfo->hWnd)->GetSafeHwnd(), pt, NULL );
}

BOOL COXToolTipCtrl::IsCursorInToolTip() const
{
    ASSERT(m_pParentWnd);

    // Is tooltip visible?
    if (m_bTipCancelled || !IsVisible() || !IsWindow(m_hWnd))
        return FALSE;

    CPoint pt;
    GetCursorPos(&pt);
    CRect rect;
    GetWindowRect(rect);

    return  rect.PtInRect(pt);
}

COXToolTipInfo* COXToolTipCtrl::FindToolFromPoint(POINT& pt)
{
    ASSERT(m_pParentWnd);

    CWnd* pWnd = GetChildWindowFromPoint(pt);
    if (!pWnd /*|| 
         pWnd->GetSafeHwnd() == m_pParentWnd->GetSafeHwnd()*/)
        return NULL;

    OXTOOLINFO ti;
    BOOL bHitTest = HitTest(pWnd->GetSafeHwnd(), pt, &ti);

    if (bHitTest)
        return GetToolInfoPtr(CWnd::FromHandle(ti.hwnd), ti.uId);
    else
        return NULL;
}

COXToolTipInfo* COXToolTipCtrl::GetToolInfoPtr(CWnd* pWnd, UINT_PTR nIDTool /*=0*/)
{
    int nSize = PtrToInt(m_arrTools.GetSize());
    for (int i = 0; i < nSize; i++)
    {
        COXToolTipInfo* pToolInfo = (COXToolTipInfo*) m_arrTools.GetAt(i);
        if (!pToolInfo)
            continue;

        if (pWnd->GetSafeHwnd() == pToolInfo->hWnd && 
            nIDTool == pToolInfo->nIDTool)
        {
            return pToolInfo;
        }
    }

    return NULL;
}

CWnd* COXToolTipCtrl::GetChildWindowFromPoint(POINT& point) const
{
    ASSERT(m_pParentWnd);

    CPoint pt = point;

    // Find the window under the cursor
    m_pParentWnd->ClientToScreen(&pt);
    HWND hWnd = ::WindowFromPoint(pt);

    // WindowFromPoint misses disabled windows and such - go for a more
    // comprehensive search in this case.
    if (hWnd == m_pParentWnd->GetSafeHwnd())
        hWnd = m_pParentWnd->ChildWindowFromPoint(point, CWP_ALL)->GetSafeHwnd();

    // Check that we aren't over the parent or out of client area
    if (!hWnd /*|| hWnd == m_pParentWnd->GetSafeHwnd()*/)
        return NULL;

    // If it's not part of the main parent window hierarchy, then we are
    // not interested
    if (!::IsChild(m_pParentWnd->GetSafeHwnd(), hWnd)
		&& m_pParentWnd->GetSafeHwnd()!=hWnd)
        return NULL;

    return CWnd::FromHandle(hWnd);
}

void COXToolTipCtrl::DisplayToolTip(CPoint& pt, BOOL bExtended /*= FALSE*/)
{
    ASSERT(::IsWindow(m_hWnd));

    if (!m_bActivated || !m_pCurrentToolTip)
        return;

    m_bExtended = bExtended;

    CString str = GetTooltipText(m_pCurrentToolTip);
    if (str.IsEmpty())
        return;

    //Calculate the width and height of the box dynamically
    CRect rect = GetBoundsRect(str, m_pCurrentToolTip->nWidth);
    rect = CalculateInfoBoxRect(pt, m_pCurrentToolTip, rect);

    ShowWindow(SW_HIDE);
    SetWindowPos(NULL, 
                 rect.left, rect.top,
                 rect.Width(), rect.Height(),
                 SWP_SHOWWINDOW|SWP_NOCOPYBITS|SWP_NOACTIVATE|SWP_NOZORDER);

//  ModifyStyle(0, WS_VISIBLE);
//  ShowWindow(SW_SHOWNA);
}    

CString COXToolTipCtrl::GetFieldFromString(CString ref, int nIndex, TCHAR ch) const
{
    CString strReturn;
    LPCTSTR pstrStart = ref.LockBuffer();
    LPCTSTR pstrBuffer = pstrStart;
    int nCurrent = 0;
    int nStart = 0;
    int nEnd = 0;
    int nOldStart = 0;

    while (nCurrent <= nIndex && *pstrBuffer != _T('\0'))
    {
        if (*pstrBuffer == ch)
        {
            nOldStart = nStart;
            nStart = nEnd+1;
            nCurrent++;
        }
        nEnd++;
        pstrBuffer++;
    }

    // May have reached the end of the string
    if (*pstrBuffer == _T('\0'))
    {
        nOldStart = nStart;
        nEnd++;
    }

    ref.UnlockBuffer();

    if (nCurrent < nIndex) 
    {
        //TRACE1("Warning: GetStringField - Couldn't find field %d.\n", nIndex);
        return strReturn;
    }
    return ref.Mid(nOldStart, nEnd-nOldStart-1);
}
