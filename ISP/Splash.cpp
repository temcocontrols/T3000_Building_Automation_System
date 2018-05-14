// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h

#include "Splash.h"  // e.g. splash.h
#include "global_function.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CString CSplashWnd::s_strPicFilePath;
CSplashWnd::CSplashWnd()
{
	m_pPicLoader = new JPGLoader;
	m_strVersion.LoadString(IDS_STR_VIRSIONNUM);
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
	if (m_pPicLoader)
	{
		delete m_pPicLoader;
		m_pPicLoader = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
// 	if (!m_bitmap.LoadBitmap(IDB_SPLASH))
// 		return FALSE;

// 	BITMAP bm;
// 	m_bitmap.GetBitmap(&bm);
	m_pPicLoader->SetJPGFileName(s_strPicFilePath);
	CSize szPic;
	m_pPicLoader->GetPicSize(szPic);
	HDC  hDC = pParentWnd->GetDC()->m_hDC;
	int nWidth = MulDiv(szPic.cx, GetDeviceCaps(hDC, LOGPIXELSX), 2540);
	int nHeight = MulDiv(szPic.cy, GetDeviceCaps(hDC, LOGPIXELSY), 2540);

	return CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, nWidth, nHeight, pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
//	AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1,1000, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	ShowSplashLogo();

	CRect rc;
	GetClientRect(&rc);
	
// 	CPen pen(PS_SOLID, 1, RGB(250,250,250));
// 	CPen* pOldPen = dc.SelectObject(&pen);
	dc.SetTextColor(RGB(250,250,250));
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight =25;                      // request a 12-pixel-height font
	lf.lfItalic = 1;
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, 	_T("Arial"), 7);                    // request a face name "Arial"
	VERIFY(font.CreateFontIndirect(&lf));  // create the font.
	
	// Do something with the font just created...
	CFont* def_font = dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(rc.Width()-150, rc.Height()-50, m_strVersion);
	dc.SelectObject(def_font);
	//dc.SelectObject(pOldPen);


	// Done with the font. Delete the font object.
	font.DeleteObject();
//	pen.DeleteObject();
// 	CDC dcImage;
// 	if (!dcImage.CreateCompatibleDC(&dc))
// 		return;
// 
// 	BITMAP bm;
// 	m_bitmap.GetBitmap(&bm);
// 
// 	// Paint the image.
// 	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
// 	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
// 	dcImage.SelectObject(pOldBitmap);
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}


void CSplashWnd::endSplashScreen()
{
	//ASSERT(c_pSplashWnd != NULL);
	if (c_pSplashWnd)
	{
		c_pSplashWnd->HideSplashScreen();
	}	
}

int CSplashWnd::ShowSplashLogo()
{
	CString strPath = GetExePath(true);
	strPath = strPath + c_strLogoFileName;

	if (!m_pPicLoader)
	{
		m_pPicLoader = new JPGLoader;
	}

	//CWnd* pWnd = (CWnd*)(&m_logoPic);

	//ASSERT(pWnd);
	m_pPicLoader->SetDisplayWnd(this);

	m_pPicLoader->SetJPGFileName(strPath);
	//m_pPicLoader->LoadPic();
	int nRet = m_pPicLoader->ShowJPG();

	//pWnd->Invalidate(TRUE);

	//UpdateData(TRUE);

	return nRet;
}

void CSplashWnd::SetSplashPicFilePath(const CString& strFilePath)
{
	s_strPicFilePath = strFilePath;
}