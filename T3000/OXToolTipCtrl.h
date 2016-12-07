// ==========================================================================
//                    Class Specification : COXToolTipCtrl
// ==========================================================================
// //////////////////////////////////////////////////////////////////////////
// Header file : OXToolTipCtrl.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is ?1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////


/*
DESCRIPTION         

COXToolTipCtrl is an extended tooltip control that allows multiline tooltips,
plus extended tooltip text. Extended tooltip text is extra text that is 
displayed if the user clicks on the tooltip window. If the tooltip contains
extended text (as well as a standard tooltip string) then the info window
will contain a small arrow that prompts the user to click on the window. Once
the window is clicked, the extended text is shown. If the window is clicked 
again then the window reduces to show just the standard text.

The maximum width of the tooltips can be specified, and if the info text is
too big to fit within these bounds then the text will be wrapped over multiple
lines. The control also allows you to specify a different text and background 
colors for the tooltips, and the display font can also be changed.

Using ToolTipCtrlEx :

This class is a direct replacement for the CToolTipCtrl class. It incorporates
the entire API of the standard CToolTipCtrl, and introduces new features not
found in the standard tooltip.

The control is used just like any other tooltip control. To use the tool simply
call Create(...) and specify the parent window of the tool, then add tools to
the control using the AddTool(...) member function. 

eg. (In a formview or dialog)

     tooltip.Create(this)
     tooltip.AddTool(GetDlgItem(IDC_CONTROL), 
			_T("Tooltip text\rThis is the extended\ntooltip text"));

where ID_CONTROL is the ID of a control.

To specify extended text for a tooltip, simply append a '\r' after your tooltip 
text, and then append the extended tooltip info.

As with the standard tooltip control you can specify the actual text for the tool 
at creation time (as shown above), or you can specify the LPSTR_TEXTCALLBACK value 
and provide a TTN_NEEDTEXT handler to return the text dynamically at runtime.

To handle the TTN_NEEDTEXT message, you will need to add a message handler in the
parent window, and an entry in the message map;

eg. In your view or form

   BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
   ...
   ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)
   END_MESSAGE_MAP()

   BOOL CMyDlg::OnInitDialog()
   {
	      CDialog::OnInitDialog();

       tooltip.Create(this)
       tooltip.AddTool(GetDlgItem(IDC_CONTROL), LPSTR_TEXTCALLBACK);
       ...
    }

    BOOL CMyDlg::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
    {    
        TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;    
        UINT nID = pNMHDR->idFrom;
        
        if (nID == IDC_CONTROL) Fill in the text buffer
        {
            _tcscpy(pTTT->szText, _T("Tooltip text\rExtended tooltip text"));
            return TRUE;
        }
        
        return FALSE;
    }

You can also supply text two alternate ways, either by supplying a string 
resource

            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst    = AfxGetResourceHandle();            
            return TRUE;

or by supplying a pointer to the text:
        
            pTTT->lpszText = _T("Tooltip text\rExtended tooltip text");
            return TRUE;

Newline characters ('\n') can be embedded anywhere within the text or extended
text to produce a multiline tooltip. If the width of the tooltip window is
specified using SetMaxTipWidth() then the tooltip text will be wrapped to this
length, and if necessary displayed on more than one line.

To change the font of the tooltips simply use the SetFont member function. 

The GetToolInfo/SetToolInfo functions, and the HitTest functions are very
similar to the CToolTipCtrl versions except that they use a OXTOOLINFO
structure instead of a TOOLINFO structure. This structure is defined as

    struct OXTOOLINFO : public TOOLINFO
    {
    #if (_WIN32_IE < 0x0300)
       LPARAM    lParam;       Application defined value that is associated with the tool
    #endif
       int       nWidth;       Width of box, or 0 for default
       COLORREF  clrTextColor; text color
       COLORREF  clrBackColor; background color
    }

and is very similar to the standard TOOLINFO.  It is used in the same way,
with the exception that the uFlags member is not (yet) used.

To change the color of an individual tip, use the GetToolInfo/SetToolInfo
functions

   OXTOOLINFO ToolInfo;
   if (m_toolTip.GetToolInfo(ToolInfo, GetDlgItem(IDC_CONTROL)))
   {
       ToolInfo.clrBackColor = RGB(255, 255, 255);
       ToolInfo.clrTextColor = RGB(  0,   0, 255);
       m_toolTip.SetToolInfo(&ToolInfo);
   }



The ToolTipEx sample that demonstrates the functionality of the COXToolTipCtrl class 
can be found in the .\Samples\gui\ToolTipEx subdirectory of your Ultimate Toolbox 
directory.



Dependencies:

#include "OXToolTipCtrl.h"


Source code files:

"OXToolTipCtrl.cpp"		-	COXToolTipCtrl implementation


*/
//
//
//////////////////////////////////////////////////////////////////////////


#if !defined(_OXTOOLTIPCTRL_H__)
#define _OXTOOLTIPCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

class COXToolTipInfo;

struct OXTOOLINFO : public TOOLINFO
{
 #if (_WIN32_IE < 0x0300)
	// Application defined value that is associated with the tool
    LPARAM    lParam;       
#endif
	// Width of box, or 0 for default
    int       nWidth;       
	// Text color
    COLORREF  clrTextColor; 
	// Background color
    COLORREF  clrBackColor; 


    OXTOOLINFO::OXTOOLINFO()
    {
        cbSize = sizeof(OXTOOLINFO);
    }

    void operator=(COXToolTipInfo& RHS);
};

// This structure holds all of the tooltip information internally
class  COXToolTipInfo : public CObject
{
public:
    UINT      uFlags;       // Not used
    HWND      hWnd;         // Window handle of the control
    UINT      nIDTool;      // ID of tool   
    CRect     rectBounds;   // Bounding rect for toolinfo to be displayed
    LPCTSTR   szText;       // Either LPSTR_TEXTCALLBACK or NULL
    CString   strText;      // Tooltip text if szText is NULL or empty.
    int       nWidth;       // Width of box, or 0 for default
    COLORREF  clrTextColor; // Text color
    COLORREF  clrBackColor; // Background color
    LPARAM    lParam;       // Application defined value that is associated 
							// with the tool


    void operator=(OXTOOLINFO& RHS)
    {
        uFlags     = RHS.uFlags;
        hWnd       = RHS.hwnd;
        nIDTool    = (UINT)RHS.uId;
        rectBounds = RHS.rect;
        if (RHS.lpszText == LPSTR_TEXTCALLBACK)
        {
            szText = RHS.lpszText;
            strText.Empty();
        }
        else
        {
            szText = NULL;
            strText = RHS.lpszText;
        }
        nWidth       = RHS.nWidth;
        clrTextColor = RHS.clrTextColor;
        clrBackColor = RHS.clrBackColor;
        lParam       = RHS.lParam;
    }

    void operator=(COXToolTipInfo& RHS)
    {
        uFlags		 = RHS.uFlags;
        hWnd         = RHS.hWnd;
        nIDTool      = RHS.nIDTool;
        rectBounds   = RHS.rectBounds;
        strText      = RHS.strText;
        nWidth       = RHS.nWidth;
        clrTextColor = RHS.clrTextColor;
        clrBackColor = RHS.clrBackColor;
        lParam       = RHS.lParam;
    }
};

inline void OXTOOLINFO::operator=(COXToolTipInfo& RHS)
{
    cbSize = sizeof(OXTOOLINFO);
    uFlags = RHS.uFlags;
    hwnd   = RHS.hWnd;
    uId    = RHS.nIDTool;
    rect   = RHS.rectBounds;
    hinst  = AfxGetResourceHandle(); 
    if (RHS.szText == LPSTR_TEXTCALLBACK)
        lpszText = LPSTR_TEXTCALLBACK;
    else
        lpszText = (LPTSTR)(LPCTSTR) RHS.strText;
    lParam       = RHS.lParam;
    nWidth       = RHS.nWidth;
    clrTextColor = RHS.clrTextColor;
    clrBackColor = RHS.clrBackColor;
}


/////////////////////////////////////////////////////////////////////////////
// COXToolTipCtrl window

class  COXToolTipCtrl : public CWnd
{
// Construction
public:
	// --- In?:
	// --- Out :
	// --- Returns:
	// --- Effect : The constructor - all relevant variables initialized
	COXToolTipCtrl();

// Attributes
public:
	// --- In:		ToolInfo	-	Reference to a TOOLINFO object that receives the 
	//								tool’s text.
	//				pWnd		-	Pointer to the window that contains the tool.
	//				nIDTool		-	ID of the tool.
	// --- Out :
	// --- Returns:	TRUE on success, FALSE otherwise
	// --- Effect :	Call this function to retrieve the information that a tool tip 
	//				control maintains about a tool. If the control has information 
	//				on the tool identified by the window and ID, then the 
	//				COXToolTipInfo structure is filled with that information.
    BOOL GetToolInfo(OXTOOLINFO& ToolInfo, CWnd* pWnd, UINT nIDTool = 0);

	// --- In:		pToolInfo	-	A pointer to an OXTOOLINFO structure that 
	//								specifies the information to be set. 
	// --- Out :
	// --- Returns:
	// --- Effect:	Sets the information that a tooltip maintains for a tool
    void SetToolInfo(OXTOOLINFO* pToolInfo);


	// --- In: 
	// --- Out:		lprc	-	Address of a RECT structure that will receive the 
	//							margin information
	// --- Returns: 
	// --- Effect:	Retrieves the margins used for drawing the text in the tooltip. 
	//				The rectangle does not specify a bounding rect, but rather the 
	//				top, bottom, left and right distances (in pixels) between the 
	//				text and the the edge of the toltip window.
    void GetMargin(LPRECT lprc) const;


	// --- In:		pWnd	-	Pointer to the window that contains the tool.
	//				nIDTool	-	ID of the tool.
	// --- Out:		str		-	Reference to a CString object that receives the 
	//							tool’s text
	// --- Returns : 
	// --- Effect : Retrieves the text that a tooltip control maintains for a tool.
	//              If pWnd and nIDTool specify a valid tool that has been previously
	//              registered, then str is filled with the tooltip's text.
    void GetText(CString& str, CWnd* pWnd, UINT nIDTool = 0);


	// --- In:		nTime		-	Specifies the new delay time, in milliseconds
	//				dwDuration	-	Flag that specifies which duration value will 
	//								be set. This is either:
	//
	//					TTDT_AUTOPOP	-	The length of time the tooltip window 
	//										remains visible if the pointer is 
	//										stationary within a tool's bounding 
	//										rectangle. 
	//					TTDT_INITIAL	-	The length of time the pointer must 
	//										remain stationary within a tool's 
	//										bounding rectangle before the tooltip 
	//										window appears.
	//				nDelay		-	Specifies the new delay time, in milliseconds
	// --- Out : 
	// --- Returns:
	// --- Effect:	Sets the delay times for the tooltip in milliseconds
    void SetDelayTime(DWORD dwDuration, int nTime);
	inline void SetDelayTime(UINT nDelay) { m_nDisplayDelay = nDelay; }

	// --- In:		dwDuration	-	Flag that specifies which duration value will 
	//								be retrieved. This is either:
	//
	//					TTDT_AUTOPOP	-	The length of time the tooltip window 
	//										remains visible if the pointer is 
	//										stationary within a tool's bounding 
	//										rectangle. 
	//					TTDT_INITIAL	-	The length of time the pointer must 
	//										remain stationary within a tool's 
	//										bounding rectangle before the tooltip 
	//										window appears.
	// --- Out :
	// --- Returns:	The delay times for the tooltip in milliseconds
	// --- Effect:	Retrieves the initial, pop-up, and reshow durations currently 
	//				set for a tooltip control
    int  GetDelayTime(DWORD dwDuration) const;


	// --- In?:
	// --- Out :
	// --- Returns:	The maximum width for a tooltip window, or 0 if this width is 
	//				calculated automatically
	// --- Effect:	Retrieves the maximum width of the tool tip window 
    int  GetMaxTipWidth() const;

	// --- In:		nWidth	-	The maximum width for a tooltip window, or 0 if this 
	//							width is calculated automatically. The maximum 
	//							tooltip width value does not indicate a tooltip 
	//							window's actual width. Rather, if a tooltip string 
	//							exceeds the maximum width, the control breaks the 
	//							text into multiple lines, using spaces to determine 
	//							line breaks. If the text cannot be segmented into 
	//							multiple lines, it will be displayed on a single line. 
	//							The length of this line may exceed the maximum 
	//							tooltip width. 
	// --- Out :
	// --- Returns:	The previous maximum tooltip window width.
	// --- Effect:	Sets the maximum tooltip window width
    int  SetMaxTipWidth(int nWidth);

	// --- In?: 
	// --- Out :
	// --- Returns:	A COLORREF value that represents the background color of the
	//				tooltip window
	// --- Effect:	Retrieves the background color
    COLORREF GetTipBkColor() const;

	// --- In:		clr - The new background color
	// --- Out :
	// --- Returns:
	// --- Effect:	Sets the background colour for all tools maintained by this 
	//				control. If the value is CLR_DEFAULT then the default system 
	//				colour is used
    void     SetTipBkColor(COLORREF clr);

    
	// --- In?: 
	// --- Out :
	// --- Returns: A COLORREF value that represents the text color
	// --- Effect:	Retrieves the text color 
	COLORREF GetTipTextColor() const;

	// --- In:		clr	-	The new text color
	// --- Out :
	// --- Returns:
	// --- Effect:	Sets the text colour for all tools maintained by this control.
	//              If the value is CLR_DEFAULT then the default system colour is 
	//				used
    void     SetTipTextColor(COLORREF clr);

// Attributes - inline
public:

	// --- In:		bActivate	-	Specifies whether the tooltip control is to be 
	//								activated or deactivated.
	// --- Out :
	// --- Returns: 
	// --- Effect:	Call this function to activate or deactivate a tooltip control. 
	//				If bActivate is TRUE, the control is activated; if FALSE, it is 
	//				deactivated. When a tooltip control is active, the tooltip 
	//				information appears when the cursor is on a tool that is 
	//				registered with the control; when it is inactive, the tooltip 
	//				information does not appear, even when the cursor is on a tool.
    inline void Activate(BOOL bActivate) { m_bActivated = bActivate; }

    
	// --- In?: 
	// --- Out :
	// --- Returns: A count of tools registered with the tooltip control
	// --- Effect:	Retrieves a count of the tools registered with the 
	//				tool tip control
    inline int  GetToolCount() const { return (int)m_arrTools.GetSize(); }

    
	// --- In?:	lprc	-	Address of a RECT structure that contains the margin 
	//							information to be set. The members of the RECT 
	//							structure do not define a bounding rectangle, 
	//							but rather the top, bottom, left and right distances 
	//							(in pixels) between the text and the the edge of the 
	//							toltip window.
	// --- Out :
	// --- Returns: 
	// --- Effect:	Sets the top, left, bottom, and right margins for a tooltip 
	//				window. A margin is the distance, in pixels, between the tooltip 
	//				window border and the text contained within the tooltip window. 
    inline void SetMargin(LPRECT lprc) { m_rectMargin = lprc; }


// Operations
public:
	
	// --- In:		pParentWnd	-	A pointer to the tooltip control parent
	// --- Out :
	// --- Returns:	TRUE on success, FALSE otherwise
	// --- Effect:	Creates the tooltip window, initially invisible
    BOOL Create(CWnd* pParentWnd);


	// --- In?:	pWnd		-	Pointer to the window that contains the tool.
	//				nIDText		-	ID of the string resource that contains the text 
	//								for the tool. If the text contains a '\r' 
	//								character, then all text before the \r is the 
	//								standard tooltip text, and all text after the \r 
	//								will be displayed as extended test if the use 
	//								clicks on the tooltip
	//				lpszText	-	Pointer to the text for the tool. If the text 
	//								contains a '\r' character, then all text before 
	//								the \r is the standard tooltip text, and all text 
	//								after the \r will be displayed as extended test if 
	//								the use clicks on the tooltip. If the text is 
	//								LPSTR_TEXTCALLBACK then the control will send the 
	//								TTN_NEEDTEXT notification message to the parent 
	//								window to retrieve the text.
	//				lpRectTool	-	Pointer to a RECT structure containing coordinates 
	//								of the tool's bounding rectangle, using client 
	//								coordinates relative to the window identified 
	//								by pWnd.
	//				nIDTool		-	ID of the tool.
	// --- Out :
	// --- Returns:	TRUE on success, FALSE otherwise
	// --- Effect : Registers a tool with the tooltip control, so that the 
	//				information stored in the tooltip is displayed when the cursor 
	//				is on the tool.
    BOOL AddTool(CWnd* pWnd, UINT nIDText, 
                 LPCRECT lpRectTool = NULL, UINT nIDTool = 0);
    BOOL AddTool(CWnd* pWnd, LPCTSTR lpszText, 
                 LPCRECT lpRectTool = NULL, UINT nIDTool = 0);
    
	// --- In:		pWnd	-	Pointer to the window that contains the tool
	//				nIDTool	-	ID of the tool
	// --- Out :
	// --- Returns:
	// --- Effect:	Removes the tool specified by pWnd and nIDTool from the 
	//              collection of tools supported by a tooltip control.
	void DelTool(CWnd* pWnd, UINT nIDTool = 0);
    
	// --- In?:	pWnd		-	Pointer to the window that contains the tool.
	//				lpRectTool	-	Pointer to a RECT structure containing coordinates 
	//								of the tool's bounding rectangle, using client 
	//								coordinates relative to the window identified 
	//								by pWnd.
	//				nIDTool		-	ID of the tool.
	// --- Out :
	// --- Returns:	TRUE on success, FALSE otherwise
	// --- Effect : Updates the bounding rectangle of the existing tooltip.
	BOOL UpdateToolRect(CWnd* pWnd, LPCRECT lpRectTool = NULL, UINT nIDTool = 0);
	
	// --- In:		pMsg	-	Pointer to a MSG structure that contains the 
	//							message to relay
	// --- Out :
	// --- Returns:
	// --- Effect:	Call this function to pass a mouse message to a tooltip 
	//				control for processing. 
	void RelayEvent(MSG* pMsg);
    
	
	// --- In:		pWnd		-	Pointer to the window that contains the tool.
	//				pt			-	Pointer to a CPoint object containing the 
	//								coordinates of the point to be tested.
	//				lpToolInfo	-	Pointer to a OXTOOLINFO structure that contains 
	//								information about the tool.
	// --- Out :
	// --- Returns:	TRUE if the point specified by the hit-test information is 
	//				within the tool’s bounding rectangle; otherwise FALSE.
	// --- Effect:	Returns TRUE if the given pt is in the windows toolinfo bounding 
	//				rectangle (pt is in client coords relative to the parent window). 
	//				If this function returns a TRUE, the structure pointed to by 
	//				pToolInfo is filled with information on the corresponding tool.
	BOOL HitTest(HWND pWnd, POINT pt, OXTOOLINFO* pToolInfo) const;
    
	
	// --- In?:
	// --- Out :
	// --- Returns:
	// --- Effect:	Hides the tooltip.
	void Pop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXToolTipCtrl)
	//}}AFX_VIRTUAL

protected:

	// --- In:		pt				-	The topleft corner of the region
	//				pToolTip		-	Information on the tooltip
	//				rectTextBounds	-	The minimum rectangle needed to contain the 
	//									text
	// --- Out : .
	// --- Returns:	A rectangle containing the bounds of the tooltip.
	// --- Effect:	Given the bounding rect of some text, this returns the rectangle 
	//              (in screen coords) that is best suited to displaying the tooltip 
	//              info (uses current mouse position).
    virtual CRect CalculateInfoBoxRect(CPoint& pt, COXToolTipInfo* pToolTip, 
                                       CRect& rectTextBounds) const;

	// --- In:		strText	-	The text to be displayed (may be multiline)
	//				nWidth	-	The desired width. If this is 0, then the width 
	//							will be calculated.
	// --- Out :
	// --- Returns: The bounding rect for the text (with top left corner at 0,0)
	// --- Effect : Returns the smallest possible rectangle that will contain 
	//              the text (including margins)
    virtual CRect GetBoundsRect(CString strText, int nWidth) const;

// Implementation
public:
	// --- In?:
	// --- Out :
	// --- Returns:
	// --- Effect:	The destructor - cleanup and memory release.
	virtual ~COXToolTipCtrl();

protected:
    inline BOOL IsVisible() const { return ((GetStyle() & WS_VISIBLE) == WS_VISIBLE); }

	// --- In:		pToolTip	-	A pointer to a COXToolTipInfo structure 
	//								describing the tooltip.
	// --- Out :
	// --- Returns: A CString containing the tooltip text.
	// --- Effect:	Returns the tooltip text for the tool. If the tooltip is
	//              currently displaying extended text, then the extended
	//              text, if available, will be returned. Otherwise the standard
	//              tooltip text will be returned.
    CString GetTooltipText(COXToolTipInfo *pToolTip);

    
	// --- In:		pWnd	-	A window containing a tool.
	//				nIDTool	-	The ID of the tool.
	// --- Out :
	// --- Returns:	A pointer to the internal storage of the tooltip for the given
	//				window and ID.
	// --- Effect:	Retrieves the internal info about a tool. 
	COXToolTipInfo*	GetToolInfoPtr(CWnd* pWnd, UINT_PTR nIDTool /*=0*/);


	// --- In:		point	-	A point (in parent window client coords) to test.
	// --- Out :
	// --- Returns:	The child window (of the parent window) from the supplied point.
	// --- Effect : Retrieves pointer to a child window of the tooltip control parent 
	//				window that contains the specified point.
    CWnd* GetChildWindowFromPoint(POINT& point) const;


	// --- In:		pToolInfo	-	A tooltip.
	// --- Out:
	// --- Returns:
	// --- Effect:	Initiates the display of a tooltip. The tip will not actually
	//              be displayed until the preset delay (m_nDisplayDelay) is up.
    void StartNewTool(COXToolTipInfo* pToolInfo);

	// --- In:		pt			-	The topleft corner where the tooltip should be 
	//								displayed.
	//				bExtended	-	If TRUE then the extended text is displayed.
	// --- Out :
	// --- Returns :
	// --- Effect : Displays the current tooltip, either as a normal tooltip or as an
	//              extended tooltip.
    void DisplayToolTip(CPoint& pt, BOOL bExtended = FALSE);


	// --- In:		pt	-	A point (in parent window client coords) to test.
	// --- Out :
	// --- Returns:	Either a pointer to the COXToolTipInfo, or NULL if it failed.
	// --- Effect : Searches through the list of tools looking for the first tool 
	//				whose bounding rect contains pt.
    COXToolTipInfo* FindToolFromPoint(POINT& pt);


	// --- In:		pToolInfo	-	A tool tip info.
	// --- Out :
	// --- Returns:	Returns TRUE if the cursor is in the tool given by pToolInfo
	// --- Effect:	Retrieves the flag that specifies if the cursor is over the 
	//				specified tool.
    BOOL IsCursorInTool(COXToolTipInfo* pToolInfo) const;

	// --- In?:
	// --- Out :
	// --- Returns:	Returns TRUE if the cursor is in the current tooltip window.
	// --- Effect:	Retrieves the flag that specifies if the cursor is over the 
	//				current tooltip window.
    BOOL IsCursorInToolTip() const;

    
	// --- In:		ref		-	The string to parse.
	//				nIndex	-	The field number to return (0 based).
	//				ch		-	The separator character.
	// --- Out :
	// --- Returns:	The substring of ref delimted by ch
	// --- Effect:	This routine breaks a supplied string into substrings, 
	//				each delimited by 'ch', and returns the substring number given 
	//				by nIndex.
	CString GetFieldFromString(CString ref, int nIndex, TCHAR ch) const;

    
	// --- In:		lpLogFont	-	A pointer to a LOGFONT structure (the new font).
	//				bRedraw		-	If TRUE then the tooltip should be immediately 
	//								redrawn
	// --- Out :.
	// --- Returns:	TRUE on success, FALSE otherwise.
	// --- Effect:	Sets the font to be used in the tooltip.
	BOOL SetLogFont(LPLOGFONT lpLogFont, BOOL bRedraw = TRUE);

	// --- In?:
	// --- Out :
	// --- Returns: A pointer to a LOGFONT structure that contains the default font 
	//				used in tooltips.
	// --- Effect : Returns the system tooltip font.
    LPLOGFONT GetSystemToolTipFont() const;

protected:
    CWnd*   m_pParentWnd;               // Parent window of all the tools
    CRect   m_rectMargin;               // Margin between text and tooltip window edge
    int     m_nMaxWidth;                // Maximum tooltip width
    CPoint  m_ptOffset;                 // Offset from the cursor 
    CFont   m_Font;
    LOGFONT m_LogFont;                  // Current font in use
    DWORD   m_dwTextStyle;              // The style in which to draw the text
    BOOL    m_bUsingSystemFont;         // Use system tooltip font?
    COXToolTipInfo *m_pCurrentToolTip;	// Current tooltip information
    UINT   m_nCheckInterval;            // Time in mS between check of the tip
    UINT   m_nDisplayTime;              // Time in mS to display the tip
    UINT   m_nElapsedTime;              // Elapsed display time of the tip
    UINT   m_nDisplayDelay;             // Delay in mS before the tip is displayed
    BOOL   m_bActivated;                // Are tips activated?
    BOOL   m_bTipCancelled;             // Has the current tooltip been cancelled?
    BOOL   m_bHasExtendedText;          // TRUE if the tooltip contains extended text
    BOOL   m_bExtended;                 // Is the tip displaying extended info?

    COLORREF m_crBackColor;             // Default background color of tips
    COLORREF m_crTextColor;             // Default foreground color of tips

    HWND   m_hOldFocusWnd;              // Window that had focus before the tooltip
                                        // was clicked on

    static LPCTSTR m_szArrowSpace;      // Spaces (' ') added to allow room for arrow in text

protected:
    enum { eIDDisplayToolEvent = 1, eIDCheckToolEvent = 2};

private:
    CObArray m_arrTools;                  // Collection of all tools

	// Generated message map functions
protected:
	//{{AFX_MSG(COXToolTipCtrl)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
    afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
    afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXTOOLTIPCTRL_H__)
