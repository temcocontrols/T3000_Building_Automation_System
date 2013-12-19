/////////////////////////////////////////////////////////////////////////////
// StaticEx.hxx : header file - implementation file is StaticEx.cxx
//
// Copyright (C) 1996 Bill Berry
// All rights reserved.
//
// This class is freely distributable as long as the copyright accompanies 
// the header and implementaion sources.
//
// Modification History:
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
//      1. COLORREF CStaticEx::bkColor( COLORREF crColor )
//         - Sets back ground color of the control
//
//      2. COLORREF CStaticEx::textColor( COLORREF crColor )
//         - Sets text or foreground color of the control
//
//      3. COLORREF bkColor() const
//         - Returns back ground color of control
//
//      4. COLORREF textColor() const
//         - Returns text (or foreground) color of control
//
//      5. void onClickDoShellCommand( const CString& linkName )
//
//         (Note: the idea for this part of the control
//                came fom Paul DiLascia in the December issue
//                of MSJ )
//
//         - Use control as a link to open a link (this may
//           be an exe if you like...
//
//         COLORREF setLinkColorVisited( COLORREF crColor )
//         - Sets the visit color, returns previous setting
//
//         COLORREF setLinkColorUnvisited( COLORREF crColor )
//         - Sets the unvisited color, returns previous setting
//
//
//
#if !defined(AFX_STATICEX_H__CC617FC0_4FE2_11D1_9E2E_40E806C10000__INCLUDED_)
#define AFX_STATICEX_H__CC617FC0_4FE2_11D1_9E2E_40E806C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef void (*CWND_INIT_FUNCTION)();

/////////////////////////////////////////////////////////////////////////////
// CStaticEx window

class CStaticEx : public CStatic
{
// Construction
public:
	CStaticEx();

// Copy Semantics

   // Block copy construction...
   //
   // No m_hWnd defined for object. 
   //
private:
   CStaticEx( const CStaticEx& ); 

public:
   // Allow basics to be copied...
   CStaticEx& operator = ( const CStaticEx& );

public:
    DECLARE_DYNCREATE( CStaticEx )

// Attributes

    // Turn off and on showing off the default cursor
    //
    void doCustomCursor( bool custom = true ) 
    {  
        m_doCustomCursor = custom; 
    }

    // Font control
    //
    void setFont( const LOGFONT* lpLogFont );

    void setFont( LONG fontHeight      = -8, 
                  LONG fontWeight      = FW_NORMAL,
                  UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
                  LPCWSTR faceName      = _T("MS Sans Serif") );
	//   LPCSTR faceName      = _T("MS Sans Serif" ) );
    
  //  void onClickDoShellCommand( const CString& linkAddress = "1" );

    // This method will send a windows message if onClickSendMessage() has
    // defined a message to send
    //
    void onClickShowModalDialog( CDialog* dlg = 0 )
    {   m_dialog = dlg;
    }

    // On a windows click event show this window object
    //
    void onClickShowWindow( CWnd* cwnd = 0 )
    {   
        ASSERT( cwnd && ::IsWindow(cwnd->m_hWnd) ); 
        m_showThisCwnd = cwnd;
    }

    // Send a windows message when a OnClick event occurs
    //
    void onClickSendMessage( CWnd* cwnd = 0, UINT wm_message = 0, WPARAM flags = 0 )
    {
        ASSERT( cwnd&& ::IsWindow(cwnd->m_hWnd) ); 
        m_sendMsgToThisCwnd = cwnd;
        m_msg_flags  = flags;
        m_wm_message = wm_message;
    }

    // Turn click events on or off
    //
    void doOnClickEvents( bool doEvents = true )
    {
        DWORD dwStyle = GetStyle();

        if ( doEvents )
        {
             if ( !(dwStyle & SS_NOTIFY) ) 
                  ::SetWindowLong( m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY );
        }
        else
        {
             if ( (dwStyle & SS_NOTIFY) ) {
                  dwStyle ^= SS_NOTIFY;
                  ::SetWindowLong( m_hWnd, GWL_STYLE, dwStyle );
             }
        }
    }


    // Control coloring of the static object
    //
    COLORREF bkColor  ( COLORREF );    
    COLORREF bkColor()   const { return m_crBkColor;   }   
    
    COLORREF textColor( COLORREF );
    COLORREF textColor() const { return m_crTextColor; }

    // Set up shell link colors
    //
    COLORREF setLinkColorVisited( COLORREF );
    COLORREF setLinkColorUnvisited( COLORREF );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticEx)
	//}}AFX_VIRTUAL

// Implementation
	virtual ~CStaticEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticEx)
	//}}AFX_MSG

  //  afx_msg void OnClicked();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message );
    
	DECLARE_MESSAGE_MAP()

    // Customize your brush
    //
    virtual BOOL CreateBrushType();

private:
    CWnd*    m_showThisCwnd;        // a window to show when a click event
                                    // occurs

    CWnd*    m_sendMsgToThisCwnd;   // the window to send messages to when
                                    // a click event occurs

    UINT     m_wm_message;          // the windows message event
                                    // initialize the m_showThisCwnd object

    WPARAM   m_msg_flags;           // data to send on click events

    CDialog* m_dialog;              // a dialog to show on show on a click event

    bool m_doCustomCursor;          // don't show the hand cursor

    CBrush   m_brBkGround;          // the back ground brush

    BOOL     m_didClickEvent;       // use to flag if a click event ever happened
    CString  m_shellCommandArg;     // The actual link

    COLORREF m_crBkColor;
    COLORREF m_crTextColor;
    COLORREF m_crOnClickColor;
    //
    // COLORREF m_crUnvisited = m_crTextColor;
    //
    CFont*   m_pCFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICEX_H__CC617FC0_4FE2_11D1_9E2E_40E806C10000__INCLUDED_)
