// CG: This file was added by the Splash Screen component.

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

#include "JPGLoader.h"
// Splash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

class CSplashWnd : public CWnd
{
// Construction
protected:
	CSplashWnd();

// Attributes:
public:
	//CBitmap m_bitmap;

// Operations
public:
	void SetPictureFileName(const CString& strFilePath);
	/** 
	 * manage the splash wnd function
	 */
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);
	static void endSplashScreen();
	static void SetSplashPicFilePath(const CString& strFilePath);
 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	/** 
	 *  create splash window
	 */
	BOOL Create(CWnd* pParentWnd = NULL);
	/** 
	 *  hide splash wnd
	 */
	void HideSplashScreen();

	// show logo pic on splash wnd
	int ShowSplashLogo();

protected:
	static BOOL c_bShowSplashWnd;			///< if show the splash window
	static CSplashWnd* c_pSplashWnd;		///< the splash wnd pointer 

	JPGLoader* m_pPicLoader;
	static CString	s_strPicFilePath;

	CString			m_strVersion;
	// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif
