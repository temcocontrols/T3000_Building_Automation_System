/////////////////////////////////////////////////////////////////////////////
// StaticEx.cpp : implementation file
//
// Copyright (C) 1996 Bill Berry
// All rights reserved.
//
// This class is freely distributable as long as the copyright accompanies 
// the header and implementaion sources.
//
// Modification History:
//
// Bill Berry			November 1996			Created
//                      March    1998           Update
//
// March update now includes Paul DiLascia's code from MSJ which
// allows the control to be used as a web link.
//
// Description:
//
//  Extended CStatic class. Allows easy customization of the following:
//
//      1. COLORREF bkColor( COLORREF crColor )
//         - Sets back ground color of the control
//
//      2. COLORREF textColor( COLORREF crColor )
//         - Sets text or foreground color of the control
//
//      3. COLORREF bkColor() const
//         - Returns back ground color of control
//
//      4. COLORREF textColor() const
//         - Returns text (or foreground) color of control
//
//      5. void setFont( const LOGFONT* lpLogFont )
//         - Sets the font of the static control.
//
//      6. void onClickDoShellCommand( const CString& linkName )
//
//         (Note: the idea for this part of the control
//                came fom Paul DiLascia in the December issue
//                of MSJ )
//
//         - Use control as a link to open a link (this may
//           be an exe if you like...
//
//      7. COLORREF setLinkColorVisited( COLORREF crColor )
//         - Sets the visit color, returns previous setting
//
//      8. COLORREF setLinkColorUnvisited( COLORREF crColor )
//         - Sets the unvisited color, returns previous setting
//
//      *** Set new font for this control ***
//
//      9. void setFont( const LOGFONT* lpLogFont );
//  
//     10. void setFont( LONG fontHeight      = -8, 
//                       LONG fontWeight      = FW_NORMAL,
//                       UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
//                       LPCSTR faceName      = _T("MS Sans Serif"          
//                     );
//
#include "stdafx.h"
#include "LogFont.h"
//#include "..\..\MSVC10API.h"
#include "StaticEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticEx

IMPLEMENT_DYNCREATE( CStaticEx, CStatic )

BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	//{{AFX_MSG_MAP(CStaticEx)
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR_REFLECT()
	//  ON_CONTROL_REFLECT( STN_CLICKED, OnClicked )
END_MESSAGE_MAP()

CStaticEx::CStaticEx()
{
	// Use system colors for defaults
	//
	m_crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBkColor = ::GetSysColor(COLOR_3DFACE);

	m_crOnClickColor = RGB( 0, 128, 192 );

	// The default brush type; SOLID
	//
	CreateBrushType();

	// Uses default font
	//
	m_pCFont = 0;

	// Link was not visited
	//
	m_didClickEvent = FALSE;

	// Set to popup a dialog when this control is clicked
	//
	m_dialog = 0;

	// Show a new window when this control is clicked
	//
	m_showThisCwnd = 0;
	m_sendMsgToThisCwnd = 0;

	// defines a message that may be sent to m_showThisCwnd
	//
	m_msg_flags = 0;
	m_wm_message = 0;

	// Do use the hand cursor
	//
	m_doCustomCursor = false;
}

CStaticEx::~CStaticEx()
{
	if (m_pCFont) delete m_pCFont;
}


// Note: Copy construction is blocked for this class.
//       This is because there would be no defined
//       m_hWnd during the construction of the object.
//
// CStaticEx::CStaticEx( const CStaticEx& o )
//

// Allow = operator to be used for copying basics.
//
CStaticEx& CStaticEx::operator =(const CStaticEx& o)
{
	_ASSERT( o != *this ); // You probably did not mean to do this...

	if (o == *this) return *this; // copying self...

	m_shellCommandArg = o.m_shellCommandArg;
	m_crOnClickColor = o.m_crOnClickColor;

	bkColor(o.m_crBkColor);
	textColor(o.m_crTextColor);

	if (o.m_pCFont)
	{
		CLogFont pLogFont;
		o.m_pCFont->GetLogFont(&pLogFont);
		setFont(&pLogFont);
	}

	m_dialog = o.m_dialog;
	m_showThisCwnd = o.m_showThisCwnd;
	m_didClickEvent = o.m_didClickEvent;
	m_sendMsgToThisCwnd = o.m_sendMsgToThisCwnd;
	m_msg_flags = o.m_msg_flags;
	m_wm_message = o.m_wm_message;
	m_doCustomCursor = o.m_doCustomCursor;

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CStaticEx message handlers

void CStaticEx::setFont(const LOGFONT* lpLogFont)
{
	_ASSERT( lpLogFont ); // logfont is not defined!!!

	if (!lpLogFont) return;

	if (m_pCFont) delete m_pCFont;

	m_pCFont = new CFont;
	m_pCFont->CreateFontIndirect(lpLogFont);

	SetFont(m_pCFont);
}

void CStaticEx::setFont(LONG fontHeight /* = -8                         */,
                        LONG fontWeight /* = FW_NORMAL                  */,
                        UCHAR pitchAndFamily /* = DEFAULT_PITCH | FF_DONTCARE*/,
                        LPCWSTR faceName /*  = "MS Sans Serif"   */)
{
	if (m_pCFont)
	{
		delete m_pCFont;
	}
	m_pCFont = new CFont;
	//	 faceName       = _T("MS Sans Serif") ;
	//const CLogFont lf( fontHeight, 
	//                   FW_NORMAL,
	//                   pitchAndFamily,
	//                   faceName
	//                 );

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = fontHeight;
	lf.lfWeight = FW_NORMAL;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcsncpy_s(lf.lfFaceName,LF_FACESIZE, faceName/*_T("MS Sans Serif")*/, 100);
	//	lf.lfFaceName=_T("MS Sans Serif");

	m_pCFont->CreateFontIndirect(&lf);
	SetFont(m_pCFont);
}

COLORREF CStaticEx::bkColor(COLORREF crColor)
{
	_ASSERT(::IsWindow(m_hWnd));

	COLORREF cr = m_crBkColor;

	m_crBkColor = crColor;

	m_brBkGround.DeleteObject();

	CreateBrushType();

	Invalidate();

	return cr;
}

COLORREF CStaticEx::textColor(COLORREF crColor)
{
	_ASSERT(::IsWindow(m_hWnd));

	COLORREF cr = m_crTextColor;

	m_crTextColor = crColor;

	Invalidate();

	return cr;
}

COLORREF CStaticEx::setLinkColorVisited(COLORREF crColor)
{
	COLORREF c = m_crOnClickColor;
	m_crOnClickColor = crColor;
	return c;
}

COLORREF CStaticEx::setLinkColorUnvisited(COLORREF crColor)
{
	m_crTextColor = crColor;
	return textColor(crColor);
}


BOOL CStaticEx::CreateBrushType()
{
	return m_brBkGround.CreateSolidBrush(m_crBkColor);
}

//void CStaticEx::onClickDoShellCommand( const CString& linkAddress /* = "bberry@javanet.com" */ )
//{
//    m_shellCommandArg  = linkAddress;
//}


HBRUSH CStaticEx::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// Setup for receiving mouse input; we only turn it
	// on if it is needed...
	//
	if (!m_shellCommandArg.IsEmpty())
	{
		doOnClickEvents();
		pDC->SetTextColor(m_didClickEvent ? m_crOnClickColor : m_crTextColor);
	}
	else
		pDC->SetTextColor(m_crTextColor);

	pDC->SetBkColor(m_crBkColor);

	return (HBRUSH)m_brBkGround;
}
#if 0
void CStaticEx::OnClicked()
{
    if ( m_showThisCwnd )
    {
        if ( m_showThisCwnd->IsWindowVisible() )
             m_showThisCwnd->ShowWindow( SW_HIDE );
        else
             m_showThisCwnd->ShowWindow( SW_SHOW );
    }

    if ( m_dialog )
    {
         m_dialog->DoModal();         
    }

    if ( !m_shellCommandArg.IsEmpty() )
    {
// Call ShellExecute to run the link...
//
         HINSTANCE h = ShellExecute( 0, _T("open"), m_shellCommandArg, 0, 0, SW_SHOWNORMAL );

         if ( (UINT)h > 32 ) {
              m_didClickEvent = TRUE;
              Invalidate();
         }
         else {
             MessageBeep(0);
             AfxMessageBox( _T("Unable to open ") + m_shellCommandArg + _T('.'), MB_OK | MB_ICONINFORMATION );
         }
    }

    if ( m_wm_message )
    {
        ::SendMessage( m_sendMsgToThisCwnd->m_hWnd, m_wm_message, m_msg_flags, 0 );
    }
}
#endif
BOOL CStaticEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCursor;

	if (m_doCustomCursor)
	{
		// hCursor = AfxGetApp()->LoadCursor( IDC_CURSOR_HAND );
	}
	else
	{
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	}

	_ASSERT( hCursor );

	::SetCursor(hCursor);

	return TRUE;
}
