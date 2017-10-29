
/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "CT3000DocView.h"
//#include "resource.h"


//////////////////////////////// Statics / Macros /////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_REGULAR_EXPRESSION	1
#define IDD_CT3000_FINDDLGORD 1
#define IDD_CT3000_REPLACEDLGORD 1
#define IDS_CT3000_DEFAULT_PRINT_HEADER 1
#define IDS_CT3000_DEFAULT_PRINT_FOOTER 1

////////////////////////////////// Implementation /////////////////////////////

class _CT3000_EDIT_STATE
{
public:
//Constructors / Destructors
	_CT3000_EDIT_STATE();

//Member variables
	CCT3000FindReplaceDlg* pFindReplaceDlg;    //find or replace dialog
	BOOL                      bFindOnly;          //Is pFindReplace the find or replace?
	CString                   strFind;            //last find string
	CString                   strReplace;         //last replace string
	BOOL                      bCase;              //TRUE==case sensitive, FALSE==not
	int                       bNext;              //TRUE==search down, FALSE== search up
	BOOL                      bWord;              //TRUE==match whole word, FALSE==not
  BOOL                      bRegularExpression; //TRUE==regular expression search, FALSE==not
};

_CT3000_EDIT_STATE::_CT3000_EDIT_STATE() : bRegularExpression(FALSE),
                                                 pFindReplaceDlg(NULL),
               	                                 bWord(FALSE),
                                                 bFindOnly(TRUE),
	                                               bCase(FALSE),
	                                               bNext(TRUE)
{
}

_CT3000_EDIT_STATE _CT3000EditState;


BEGIN_MESSAGE_MAP(CCT3000FindReplaceDlg, CFindReplaceDialog)
  ON_BN_CLICKED(IDC_REGULAR_EXPRESSION, OnRegularExpression)
END_MESSAGE_MAP()

CCT3000FindReplaceDlg::CCT3000FindReplaceDlg(): m_bRegularExpression(FALSE)
{
}

BOOL CCT3000FindReplaceDlg::Create(BOOL bFindDialogOnly,	LPCTSTR lpszFindWhat, LPCTSTR lpszReplaceWith, DWORD dwFlags, CWnd* pParentWnd)
{
  //Tell Windows to use our dialog instead of the standard one
  m_fr.hInstance = AfxGetResourceHandle();
  m_fr.Flags |= FR_ENABLETEMPLATE;
  if (bFindDialogOnly)
    m_fr.lpTemplateName = MAKEINTRESOURCE(IDD_CT3000_FINDDLGORD);
  else
    m_fr.lpTemplateName = MAKEINTRESOURCE(IDD_CT3000_REPLACEDLGORD);

  //Let the base class do its thing
  return CFindReplaceDialog::Create(bFindDialogOnly,	lpszFindWhat, lpszReplaceWith, dwFlags, pParentWnd);
}

BOOL CCT3000FindReplaceDlg::OnInitDialog() 
{
  //let the base class do its thing
	BOOL bReturn = CFindReplaceDialog::OnInitDialog();

  //Should we check the regular expression check box
  CButton* pCtrl = static_cast<CButton*>(GetDlgItem(IDC_REGULAR_EXPRESSION));
  ASSERT(pCtrl);
  pCtrl->SetCheck(m_bRegularExpression);
	
	return bReturn;
}

void CCT3000FindReplaceDlg::OnRegularExpression() 
{
  //Save the state of the Regular expression checkbox into a member variable
  CButton* pCtrl = static_cast<CButton*>(GetDlgItem(IDC_REGULAR_EXPRESSION));
  ASSERT(pCtrl);
	m_bRegularExpression = (pCtrl->GetCheck() == 1);
}



IMPLEMENT_DYNCREATE(CCT3000View, CView)

const UINT _CT3000MsgFindReplace = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(CCT3000View, CView)
	ON_WM_PAINT()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateNeedPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateNeedFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
  ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateNeedTextAndFollowingText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateNeedText)
	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
 	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_REGISTERED_MESSAGE(_CT3000MsgFindReplace, OnFindReplaceCmd)
END_MESSAGE_MAP()

CCT3000View::CCT3000View() : m_rMargin(0, 0, 0, 0), 
                                   m_bFirstSearch(TRUE),
                                   m_bUseROFileAttributeDuringLoading(TRUE),
                                   m_bPrintHeader(TRUE), 
                                   m_bPrintFooter(TRUE),
                                   m_bPersistMarginSettings(TRUE)
{
  m_bUsingMetric = UserWantsMetric();
}

CCT3000Ctrl& CCT3000View::GetCtrl()
{ 
  CCT3000Ctrl& ctrl = m_Edit;

  return ctrl; 
}

void CCT3000View::LoadMarginSettings(const CString& sSection)
{
  //Get the margin values 
  CWinApp* pApp = AfxGetApp();
  ASSERT(pApp);
	m_rMargin.left = pApp->GetProfileInt(sSection, _T("LeftMargin"), m_rMargin.left);
	m_rMargin.right = pApp->GetProfileInt(sSection, _T("RightMargin"), m_rMargin.right);
	m_rMargin.top = pApp->GetProfileInt(sSection, _T("TopMargin"), m_rMargin.top);
	m_rMargin.bottom = pApp->GetProfileInt(sSection, _T("BottomMargin"), m_rMargin.bottom);
}

void CCT3000View::SaveMarginSettings(const CString& sSection)
{
  //Write out the margin values 
  CWinApp* pApp = AfxGetApp();
  ASSERT(pApp);
	pApp->WriteProfileInt(sSection, _T("LeftMargin"), m_rMargin.left);
	pApp->WriteProfileInt(sSection, _T("RightMargin"), m_rMargin.right);
	pApp->WriteProfileInt(sSection, _T("TopMargin"), m_rMargin.top);
	pApp->WriteProfileInt(sSection, _T("BottomMargin"), m_rMargin.bottom);
}

void CCT3000View::OnDestroy()
{
  //Close Find/Replace dialog if this is the last CCT3000View
	if (_CT3000EditState.pFindReplaceDlg)
	{
		CWinApp* pApp = AfxGetApp();

    //Count up the number of CCT3000View's we have (excluding this one)
    int nCT3000Views = 0;

		//no doc manager - no templates
		if (pApp->m_pDocManager)
		{
			//walk all templates
			CDocTemplate* pTemplate;
			POSITION pos = pApp->m_pDocManager->GetFirstDocTemplatePosition();
			while (pos && (nCT3000Views == 0))
			{
				pTemplate = pApp->m_pDocManager->GetNextDocTemplate(pos);
				ASSERT(pTemplate);

				//walk all documents in the template
				POSITION pos2 = pTemplate->GetFirstDocPosition();
				while (pos2 && (nCT3000Views == 0))
				{
					CDocument* pDoc = pTemplate->GetNextDoc(pos2);
					ASSERT(pDoc);

					//walk all views in the document
					POSITION pos3 = pDoc->GetFirstViewPosition();
					while (pos3 && (nCT3000Views == 0))
					{
						CView* pView = pDoc->GetNextView(pos3);
						ASSERT(pView);
						// if we find another CCT3000View, skip code that closes find dialog
						if (pView->IsKindOf(RUNTIME_CLASS(CCT3000View)) && (pView != this) && ::IsWindow(pView->GetSafeHwnd()))
							++nCT3000Views;
					}
				}
			}
		}

    //Close down the find/replace dialog if we are the last CCT3000View
		if (nCT3000Views == 0)
    {
      if (::IsWindow(_CT3000EditState.pFindReplaceDlg->m_hWnd))
			  _CT3000EditState.pFindReplaceDlg->SendMessage(WM_CLOSE);
		  _CT3000EditState.pFindReplaceDlg = NULL;
    }
	}

  //Let the base class do its thing
  CView::OnDestroy();
}

void CCT3000View::DeleteContents()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

  CCT3000Ctrl& rCtrl = GetCtrl();
	rCtrl.ClearAll();
  rCtrl.EmptyUndoBuffer();
}

void CCT3000View::OnDraw(CDC*)
{
	ASSERT(FALSE);
}

void CCT3000View::OnPaint()
{
	// this is done to avoid CView::OnPaint
	Default();
}

BOOL CCT3000View::OnPreparePrinting(CPrintInfo* pInfo)
{
  //Determine if we should allow selection printing
  CCT3000Ctrl& rCtrl = GetCtrl();

  long nStartChar = rCtrl.GetSelectionStart();
  long nEndChar = rCtrl.GetSelectionEnd();
  if (nStartChar != nEndChar)
  {
    // Enable the Selection button
    pInfo->m_pPD->m_pd.Flags &= ~PD_NOSELECTION;

    // Check the Selection button
    pInfo->m_pPD->m_pd.Flags |= PD_SELECTION;
  }

  //Let the base class do its thing
  return DoPreparePrinting(pInfo);
}

void CCT3000View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);

  CCT3000Ctrl& rCtrl = GetCtrl();

	//initialize page start vector
	ASSERT(m_aPageStart.GetSize() == 0);
  if (pInfo->m_pPD->PrintSelection())
    m_aPageStart.Add(rCtrl.GetSelectionStart());
  else
	  m_aPageStart.Add(0);
	ASSERT(m_aPageStart.GetSize() > 0);

	ASSERT_VALID(this);
}

BOOL CCT3000View::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

  CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= static_cast<UINT>(m_aPageStart.GetSize()));
	VERIFY(pDC->SaveDC() != 0);
	pDC->IntersectClipRect(0, 0, 0, 0);
	pInfo->m_nCurPage = m_aPageStart.GetSize();
	while (pInfo->m_nCurPage < nPageSave)
	{
		ASSERT(pInfo->m_nCurPage == static_cast<UINT>(m_aPageStart.GetSize()));
		OnPrepareDC(pDC, pInfo);
		ASSERT(pInfo->m_bContinuePrinting);
		pInfo->m_rectDraw.SetRect(0, 0,	pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		OnPrint(pDC, pInfo);
		if (pInfo->m_nCurPage == static_cast<UINT>(m_aPageStart.GetSize()))
			break;
		++pInfo->m_nCurPage;
	}
	BOOL bResult = pInfo->m_nCurPage == nPageSave;
	pDC->RestoreDC(-1);
	pInfo->m_nCurPage = nPageSave;
  pInfo->m_rectDraw = rectSave;
	ASSERT_VALID(this);
	return bResult;
}

void CCT3000View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  //Validate our parameters
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);

  if (pInfo->m_nCurPage <= pInfo->GetMaxPage())
  {
	  if ((pInfo->m_nCurPage > static_cast<UINT>(m_aPageStart.GetSize())) &&	!PaginateTo(pDC, pInfo))
	  {
		  //can't paginate to that page, thus cannot print it.
		  pInfo->m_bContinuePrinting = FALSE;
	  }
	  ASSERT_VALID(this);
  }
  else
  {
		//Reached the max page to print
		pInfo->m_bContinuePrinting = FALSE;
  }
}

void CCT3000View::PrintHeader(CDC* pDC, CPrintInfo* /*pInfo*/, RangeToFormat& frPrint)
{
  //By Default we print "Document Name - Printed on Date" as well as a line separator below the text
  //Derived classes are of course free to implement their own version of PrintHeader
  CString sHeader;
  AfxFormatString2(sHeader, IDS_CT3000_DEFAULT_PRINT_HEADER, GetDocument()->GetTitle(), COleDateTime::GetCurrentTime().Format());

  //Setup the DC
  pDC->SetTextColor(RGB(0, 0, 0));
  UINT nAlign = pDC->SetTextAlign(TA_LEFT | TA_TOP);

  //Draw the header
  CSize sizeText = pDC->GetTextExtent(sHeader);
  int nHeaderDepth = 2*sizeText.cy;
	CRect rLine(frPrint.rcPage.left, frPrint.rcPage.top, frPrint.rcPage.right, frPrint.rcPage.top + nHeaderDepth);
  pDC->ExtTextOut(frPrint.rcPage.left, frPrint.rcPage.top + nHeaderDepth/3, 0, &rLine, sHeader, NULL);

  //Draw a line underneath the text
  pDC->MoveTo(frPrint.rcPage.left, frPrint.rcPage.top + nHeaderDepth - 5);
  pDC->LineTo(frPrint.rcPage.right, frPrint.rcPage.top + nHeaderDepth - 5);

  //Restore the DC
  pDC->SetTextAlign(nAlign);

  //Adjust the place where CT3000 will draw the text
  if (frPrint.rc.top < (frPrint.rcPage.top + nHeaderDepth))
    frPrint.rc.top = frPrint.rcPage.top + nHeaderDepth;
}

void CCT3000View::PrintFooter(CDC* pDC, CPrintInfo* pInfo, RangeToFormat& frPrint)
{
  //By Default we print "Page X" as well as a line separator above the text
  //Derived classes are of course free to implement their own version of PrintFooter
  CString sPage;
  sPage.Format(_T("%d"), pInfo->m_nCurPage);
  CString sFooter;
  AfxFormatString1(sFooter, IDS_CT3000_DEFAULT_PRINT_FOOTER, sPage);

  //Setup the DC
  pDC->SetTextColor(RGB(0, 0, 0));
  UINT nAlign = pDC->SetTextAlign(TA_LEFT | TA_TOP);
  
  //Draw the header
  CSize sizeText = pDC->GetTextExtent(sFooter);
  int nFooterDepth = 2*sizeText.cy;
  CRect rLine(frPrint.rcPage.left, frPrint.rcPage.bottom - nFooterDepth, frPrint.rcPage.right, frPrint.rcPage.bottom);
  pDC->ExtTextOut(frPrint.rcPage.left, frPrint.rcPage.bottom - nFooterDepth*2/3, 0, &rLine, sFooter, NULL);

  //Draw a line above the text
  pDC->MoveTo(frPrint.rcPage.left, frPrint.rcPage.bottom - nFooterDepth + 5);
  pDC->LineTo(frPrint.rcPage.right, frPrint.rcPage.bottom - nFooterDepth + 5);

  //Restore the DC
  pDC->SetTextAlign(nAlign);

  //Adjust the place where CT3000 will draw the text
  if (frPrint.rc.bottom > (frPrint.rcPage.bottom - nFooterDepth))
    frPrint.rc.bottom = frPrint.rcPage.bottom - nFooterDepth;
}

BOOL CCT3000View::UserWantsMetric()
{
  TCHAR localeInfo[3];
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, localeInfo, 3);
  return (localeInfo[0] == _T('0')) ;
}

long CCT3000View::PrintPage(CDC* pDC, CPrintInfo* pInfo, long nIndexStart, long nIndexStop)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	RangeToFormat rfPrint;
	rfPrint.hdc = pDC->m_hDC;
	rfPrint.hdcTarget = pDC->m_hAttribDC;

	//Take into account the specified margins
  CRect rMargins;
	if ((m_rMargin.left) != 0 || (m_rMargin.right) != 0 || (m_rMargin.top) != 0 || (m_rMargin.bottom != 0)) 
  {
    //Get printer resolution
    CPoint pDpi;
    pDpi.x = pDC->GetDeviceCaps(LOGPIXELSX);  //DPI in X direction
    pDpi.y = pDC->GetDeviceCaps(LOGPIXELSY);  //DPI in Y direction

		//Convert the hundredths of millimeters or thousandths of inches margin values
		//from the Page Setup dialog to device units.
		int iScale = m_bUsingMetric ? 2540 : 1000;    //scale factor for margin scaling;
		rMargins.left   = MulDiv(m_rMargin.left, pDpi.x, iScale);
		rMargins.top    = MulDiv(m_rMargin.top, pDpi.y, iScale);
		rMargins.right	= MulDiv(m_rMargin.right, pDpi.x, iScale);
		rMargins.bottom	= MulDiv(m_rMargin.bottom, pDpi.y, iScale);
	} 
  else 
		rMargins = m_rMargin;

  //We take the page size from the pInfo member variable (decrement the right and
  //bottom values by 1 to suit CT3000)
	rfPrint.rcPage.left = pInfo->m_rectDraw.left;
	rfPrint.rcPage.top = pInfo->m_rectDraw.top;
	rfPrint.rcPage.right = pInfo->m_rectDraw.right - 1;
	rfPrint.rcPage.bottom = pInfo->m_rectDraw.bottom - 1;

  //Fill in the area to print
  rfPrint.rc.left = rfPrint.rcPage.left + rMargins.left;
  rfPrint.rc.top = rfPrint.rcPage.top + rMargins.top;
  rfPrint.rc.right = rfPrint.rcPage.right - rMargins.right;
  rfPrint.rc.bottom = rfPrint.rcPage.bottom - rMargins.bottom;

  //Fill in the text to print
	rfPrint.chrg.cpMin = nIndexStart;
	rfPrint.chrg.cpMax = nIndexStop;

  //Print the header (if requested to)
  if (m_bPrintHeader)
    PrintHeader(pDC, pInfo, rfPrint);

  //Print the footer (if requested to)
  if (m_bPrintFooter)
    PrintFooter(pDC, pInfo, rfPrint);

  //Finally ask the control to print the text
  return GetCtrl().FormatRange(TRUE, &rfPrint);
}

void CCT3000View::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);

	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage <= (UINT) m_aPageStart.GetSize());
	int nIndex = m_aPageStart[nPage-1];

  //Determine where we should end the printing
  int nEndPrint = 0;
  if (pInfo->m_pPD->PrintSelection())
    nEndPrint = GetCtrl().GetSelectionEnd();
  else
    nEndPrint = GetCtrl().GetTextLength();

	//print as much as possible in the current page.
	nIndex = PrintPage(pDC, pInfo, nIndex, nEndPrint);
	if (nIndex >= nEndPrint)
	{
		TRACE0("End of Document\n");
		pInfo->SetMaxPage(nPage);
	}

	//update pagination information for page just printed
	if (nPage == (UINT) m_aPageStart.GetSize())
	{
		if (nIndex < nEndPrint)
			m_aPageStart.Add(nIndex);
	}
	else
	{
		ASSERT(nPage+1 <= static_cast<UINT>(m_aPageStart.GetSize()));
		ASSERT(nIndex == m_aPageStart[nPage+1-1]);
	}
}

void CCT3000View::OnEndPrinting(CDC*, CPrintInfo*)
{
	ASSERT_VALID(this);
	m_aPageStart.RemoveAll();
}

void CCT3000View::OnUpdateNeedPaste(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetCtrl().CanPaste());
}

void CCT3000View::OnUpdateNeedText(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetCtrl().GetTextLength() != 0);
}

void CCT3000View::OnUpdateNeedTextAndFollowingText(CCmdUI* pCmdUI)
{
  CCT3000Ctrl& rCtrl = GetCtrl();

	ASSERT_VALID(this);

  int nLength = rCtrl.GetTextLength();
  long nStartChar = rCtrl.GetSelectionStart();

	pCmdUI->Enable(nLength && (nStartChar != nLength));
}

void CCT3000View::OnUpdateNeedFind(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetCtrl().GetLength() != 0 && !_CT3000EditState.strFind.IsEmpty());
}

void CCT3000View::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetCtrl().CanUndo());
}

void CCT3000View::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetCtrl().CanRedo());
}

void CCT3000View::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
  CCT3000Ctrl& rCtrl = GetCtrl();
  
	ASSERT_VALID(this);
  long nStartChar = rCtrl.GetSelectionStart();
  long nEndChar = rCtrl.GetSelectionEnd();
	pCmdUI->Enable(nStartChar != nEndChar);
}

void CCT3000View::OnEditCut()
{
	ASSERT_VALID(this);
	GetCtrl().Cut();
}

void CCT3000View::OnEditCopy()
{
	ASSERT_VALID(this);
	GetCtrl().Copy();
}

void CCT3000View::OnEditPaste()
{
	ASSERT_VALID(this);
	GetCtrl().Paste();
}

void CCT3000View::OnEditClear()
{
	ASSERT_VALID(this);
	GetCtrl().Clear();
}

void CCT3000View::OnEditUndo()
{
	ASSERT_VALID(this);
	GetCtrl().Undo();
}

void CCT3000View::OnEditRedo()
{
	ASSERT_VALID(this);
	GetCtrl().Redo();
}

void CCT3000View::OnEditSelectAll()
{
	ASSERT_VALID(this);
	GetCtrl().SelectAll();
}

void CCT3000View::OnEditFind()
{
	ASSERT_VALID(this);
	OnEditFindReplace(TRUE);
}

void CCT3000View::OnEditReplace()
{
	ASSERT_VALID(this);
	OnEditFindReplace(FALSE);
}

void CCT3000View::OnEditRepeat()
{
	ASSERT_VALID(this);
	if (!FindText(_CT3000EditState.strFind, _CT3000EditState.bNext, _CT3000EditState.bCase, _CT3000EditState.bWord, _CT3000EditState.bRegularExpression))
		TextNotFound(_CT3000EditState.strFind, _CT3000EditState.bNext, _CT3000EditState.bCase, _CT3000EditState.bWord, _CT3000EditState.bRegularExpression, FALSE);
}

void CCT3000View::AdjustFindDialogPosition()
{
	ASSERT(_CT3000EditState.pFindReplaceDlg);
  CCT3000Ctrl& rCtrl = GetCtrl();
  int nStart = rCtrl.GetSelectionStart();
	CPoint point;
  point.x = rCtrl.PointXFromPosition(nStart);
  point.y = rCtrl.PointYFromPosition(nStart);
	ClientToScreen(&point);
	CRect rectDlg;
	_CT3000EditState.pFindReplaceDlg->GetWindowRect(&rectDlg);
	if (rectDlg.PtInRect(point))
	{
		if (point.y > rectDlg.Height())
			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
		else
		{
			int nVertExt = GetSystemMetrics(SM_CYSCREEN);
			if (point.y + rectDlg.Height() < nVertExt)
				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
		}
		_CT3000EditState.pFindReplaceDlg->MoveWindow(&rectDlg);
	}
}

CCT3000FindReplaceDlg* CCT3000View::CreateFindReplaceDialog()
{
  return new CCT3000FindReplaceDlg;
}

void CCT3000View::OnEditFindReplace(BOOL bFindOnly)
{
	ASSERT_VALID(this);

	m_bFirstSearch = TRUE;
	if (_CT3000EditState.pFindReplaceDlg != NULL)
	{
		if (_CT3000EditState.bFindOnly == bFindOnly)
		{
			_CT3000EditState.pFindReplaceDlg->SetActiveWindow();
			_CT3000EditState.pFindReplaceDlg->ShowWindow(SW_SHOW);
			return;
		}
		else
		{
			ASSERT(_CT3000EditState.bFindOnly != bFindOnly);
			_CT3000EditState.pFindReplaceDlg->SendMessage(WM_CLOSE);
			ASSERT(_CT3000EditState.pFindReplaceDlg == NULL);
			ASSERT_VALID(this);
		}
	}
  CCT3000Ctrl& rCtrl = GetCtrl();
	CString strFind(rCtrl.GetSelText());
	//if selection is empty or spans multiple lines use old find text
	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))
		strFind = _CT3000EditState.strFind;

	CString strReplace(_CT3000EditState.strReplace);
	_CT3000EditState.pFindReplaceDlg = CreateFindReplaceDialog();
	ASSERT(_CT3000EditState.pFindReplaceDlg != NULL);
	DWORD dwFlags = NULL;
	if (_CT3000EditState.bNext)
		dwFlags |= FR_DOWN;
	if (_CT3000EditState.bCase)
		dwFlags |= FR_MATCHCASE;
	if (_CT3000EditState.bWord)
		dwFlags |= FR_WHOLEWORD;
  if (_CT3000EditState.bRegularExpression)
    _CT3000EditState.pFindReplaceDlg->SetRegularExpression(TRUE);

	if (!_CT3000EditState.pFindReplaceDlg->Create(bFindOnly, strFind, strReplace, dwFlags, this))
	{
		_CT3000EditState.pFindReplaceDlg = NULL;
		ASSERT_VALID(this);
		return;
	}
	ASSERT(_CT3000EditState.pFindReplaceDlg != NULL);
	_CT3000EditState.bFindOnly = bFindOnly;
	_CT3000EditState.pFindReplaceDlg->SetActiveWindow();
	_CT3000EditState.pFindReplaceDlg->ShowWindow(SW_SHOW);
	ASSERT_VALID(this);
}

void CCT3000View::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression)
{
	ASSERT_VALID(this);
	_CT3000EditState.strFind = lpszFind;
	_CT3000EditState.bCase = bCase;
	_CT3000EditState.bWord = bWord;
	_CT3000EditState.bNext = bNext;
  _CT3000EditState.bRegularExpression = bRegularExpression;

	if (!FindText(_CT3000EditState.strFind, bNext, bCase, bWord, bRegularExpression))
		TextNotFound(_CT3000EditState.strFind, bNext, bCase, bWord, bRegularExpression, FALSE);
	else
		AdjustFindDialogPosition();
	ASSERT_VALID(this);
}

void CCT3000View::OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,	BOOL bWord, BOOL bRegularExpression, LPCTSTR lpszReplace)
{
	ASSERT_VALID(this);
	_CT3000EditState.strFind = lpszFind;
	_CT3000EditState.strReplace = lpszReplace;
	_CT3000EditState.bCase = bCase;
	_CT3000EditState.bWord = bWord;
	_CT3000EditState.bNext = bNext;
  _CT3000EditState.bRegularExpression = bRegularExpression;

  CCT3000Ctrl& rCtrl = GetCtrl();

	if (!SameAsSelected(_CT3000EditState.strFind, bCase, bWord, bRegularExpression))
	{
		if (!FindText(_CT3000EditState.strFind, bNext, bCase, bWord, bRegularExpression))
			TextNotFound(_CT3000EditState.strFind, bNext, bCase, bWord, bRegularExpression, TRUE);
		else
			AdjustFindDialogPosition();
		return;
	}

  if (bRegularExpression)
  {
    rCtrl.TargetFromSelection();
    rCtrl.ReplaceTargetRE(_CT3000EditState.strReplace.GetLength(), _CT3000EditState.strReplace);    
  }
  else
  	GetCtrl().ReplaceSel(_CT3000EditState.strReplace);
	if (!FindText(_CT3000EditState.strFind, bNext, bCase, bWord, bRegularExpression))
		TextNotFound(_CT3000EditState.strFind, bNext, bCase, bWord, bRegularExpression, TRUE);
	else
		AdjustFindDialogPosition();
	ASSERT_VALID(this);
}

void CCT3000View::OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase, BOOL bWord, BOOL bRegularExpression)
{
	ASSERT_VALID(this);
	_CT3000EditState.strFind = lpszFind;
	_CT3000EditState.strReplace = lpszReplace;
	_CT3000EditState.bCase = bCase;
	_CT3000EditState.bWord = bWord;
	_CT3000EditState.bNext = TRUE;
  _CT3000EditState.bRegularExpression = bRegularExpression;

	CWaitCursor wait;

  //Set the selection to the begining of the document to ensure all text is replaced in the document
  CCT3000Ctrl& rCtrl = GetCtrl();
  rCtrl.SetSel(0, 0);

  //Do the replacments
	rCtrl.HideSelection(TRUE, FALSE);
  BOOL bFoundSomething = FALSE;
	while (FindTextSimple(_CT3000EditState.strFind, _CT3000EditState.bNext, bCase, bWord, bRegularExpression))
  {
    bFoundSomething = TRUE;
    if (bRegularExpression)
    {
      rCtrl.TargetFromSelection();
      rCtrl.ReplaceTargetRE(_CT3000EditState.strReplace.GetLength(), _CT3000EditState.strReplace);
    }
    else
		  rCtrl.ReplaceSel(_CT3000EditState.strReplace);
  }

  //Restore the old selection
	rCtrl.HideSelection(FALSE, FALSE);

  //Inform the user if we could not find anything
  if (!bFoundSomething)
    TextNotFound(_CT3000EditState.strFind, _CT3000EditState.bNext, bCase, bWord, bRegularExpression, TRUE);

	ASSERT_VALID(this);
}

LRESULT CCT3000View::OnFindReplaceCmd(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT_VALID(this);

	CCT3000FindReplaceDlg* pDialog = static_cast<CCT3000FindReplaceDlg*>(CFindReplaceDialog::GetNotifier(lParam));
	ASSERT(pDialog != NULL);
	ASSERT(pDialog == _CT3000EditState.pFindReplaceDlg);

	if (pDialog->IsTerminating())
		_CT3000EditState.pFindReplaceDlg = NULL;
	else if (pDialog->FindNext())
		OnFindNext(pDialog->GetFindString(), pDialog->SearchDown(), pDialog->MatchCase(), pDialog->MatchWholeWord(), pDialog->GetRegularExpression());
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!_CT3000EditState.bFindOnly);
		OnReplaceSel(pDialog->GetFindString(), pDialog->SearchDown(), pDialog->MatchCase(), pDialog->MatchWholeWord(), pDialog->GetRegularExpression(), pDialog->GetReplaceString());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!_CT3000EditState.bFindOnly);
		OnReplaceAll(pDialog->GetFindString(), pDialog->GetReplaceString(), pDialog->MatchCase(), pDialog->MatchWholeWord(), pDialog->GetRegularExpression());
	}
	ASSERT_VALID(this);

	return 0;
}

BOOL CCT3000View::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL bWord, BOOL bRegularExpression)
{
  CCT3000Ctrl& rCtrl = GetCtrl();

  //check length first
	int nStartChar = rCtrl.GetSelectionStart();                           //get the selection size    
  int nEndChar = rCtrl.GetSelectionEnd();
  size_t nLen = lstrlen(lpszCompare);                                   //get the #chars to search for
  if (!bRegularExpression && (nLen != (size_t)(nEndChar - nStartChar))) //if not a regular expression...
    return FALSE;                                                       //...sizes must match,

  //Now use the advanced search functionality of CT3000 to determine the result
  int iFlags = bCase ? SCFIND_MATCHCASE : 0;
  iFlags |= bWord ? SCFIND_WHOLEWORD : 0;
  iFlags |= bRegularExpression ? SCFIND_REGEXP : 0;
  rCtrl.SetSearchFlags(iFlags);
  rCtrl.TargetFromSelection();                     //set target
  if (rCtrl.SearchInTarget(static_cast<int>(nLen), lpszCompare) < 0) //see what we got
    return FALSE;                                  //no match

  //If we got a match, the target is set to the found text
  return (rCtrl.GetTargetStart() == nStartChar) && (rCtrl.GetTargetEnd() == nEndChar);
}

BOOL CCT3000View::FindText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression)
{
	ASSERT_VALID(this);
	CWaitCursor wait;
	return FindTextSimple(lpszFind, bNext, bCase, bWord, bRegularExpression);
}

BOOL CCT3000View::FindTextSimple(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression)
{
	USES_CONVERSION;

  CCT3000Ctrl& rCtrl = GetCtrl();

	ASSERT(lpszFind != NULL);
	TextToFind ft;
  ft.chrg.cpMin = rCtrl.GetSelectionStart();
  ft.chrg.cpMax = rCtrl.GetSelectionEnd();
	if (m_bFirstSearch)
		m_bFirstSearch = FALSE;

#ifdef _UNICODE
	CCT3000Ctrl::W2UTF8(lpszFind, -1, ft.lpstrText);
#else
	ft.lpstrText = T2A(const_cast<LPTSTR>(lpszFind));
#endif
	if (ft.chrg.cpMin != ft.chrg.cpMax) // i.e. there is a selection
	{
#ifndef _UNICODE
		// If byte at beginning of selection is a DBCS lead byte,
		// increment by one extra byte.
		TEXTRANGE textRange;
		TCHAR ch[2];
		textRange.chrg.cpMin = ft.chrg.cpMin;
		textRange.chrg.cpMax = ft.chrg.cpMin + 1;
		textRange.lpstrText = ch;
		rCtrl.SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&textRange);
		if (_istlead(ch[0]))
		{
			ASSERT(ft.chrg.cpMax - ft.chrg.cpMin >= 2);

      if (bNext)
  			ft.chrg.cpMin++;
      else
        ft.chrg.cpMax = ft.chrg.cpMin - 1;
		}
#endif

    if (bNext)  
		  ft.chrg.cpMin++;
    else
      ft.chrg.cpMax = ft.chrg.cpMin - 1;
	}

  int nLength = rCtrl.GetLength();
  if (bNext)
    ft.chrg.cpMax = nLength;
	else
 	  ft.chrg.cpMin = 0;

	DWORD dwFlags = bCase ? SCFIND_MATCHCASE : 0;
	dwFlags |= bWord ? SCFIND_WHOLEWORD : 0;
  dwFlags |= bRegularExpression ? SCFIND_REGEXP : 0;

  if (!bNext)
  {
    //Swap the start and end positions which CT3000 uses to flag backward searches
    int ncpMinTemp = ft.chrg.cpMin;
    ft.chrg.cpMin = ft.chrg.cpMax;
    ft.chrg.cpMax = ncpMinTemp;
  }

	//if we find the text return TRUE
	BOOL bFound = (FindAndSelect(dwFlags, ft) != -1);

#ifdef _UNICODE
  //Tidy up the heap memory before we return
  delete [] ft.lpstrText;
#endif

  return bFound;
}

long CCT3000View::FindAndSelect(DWORD dwFlags, TextToFind& ft)
{
  CCT3000Ctrl& rCtrl = GetCtrl();
	long index = rCtrl.FindText(dwFlags, &ft);
	if (index != -1) // i.e. we found something
    rCtrl.SetSel(ft.chrgText.cpMin, ft.chrgText.cpMax);
	return index;
}

void CCT3000View::TextNotFound(LPCTSTR /*lpszFind*/, BOOL /*bNext*/, BOOL /*bCase*/, BOOL /*bWord*/, BOOL /*bRegularExpression*/, BOOL /*bReplaced*/)
{
	ASSERT_VALID(this);
	m_bFirstSearch = TRUE;
  MessageBeep(MB_ICONHAND);
}

void CCT3000View::OnSetFocus(CWnd* /*pOldWnd*/) 
{
  //Give the focus to the child control
	m_Edit.SetFocus();
}

void CCT3000View::OnSize(UINT nType, int cx, int cy) 
{
  //Let the base class do its thing
	CView::OnSize(nType, cx, cy);

  //Resize the edit control to be the size of the client area
  CRect r;
  GetClientRect(&r);
  m_Edit.MoveWindow(&r);	
}

BOOL CCT3000View::OnEraseBkgnd(CDC* /*pDC*/) 
{
  //We do nothing here, because the CT3000 control takes up the entire 
  //client area of our view;
	return TRUE;
}

void CCT3000View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	//need to set owner of Find/Replace dialog
	if (_CT3000EditState.pFindReplaceDlg && pActivateView == this)
		_CT3000EditState.pFindReplaceDlg->m_fr.hwndOwner = m_hWnd;

  //let the base class do its thing
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

int CCT3000View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  //let the base class do its thing
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  //Create the CT3000 edit control  
  CRect r(0,0,0,0);
  if (!m_Edit.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, r, this, 0))
    return -1;
	
	return 0;
}

#ifdef _DEBUG
void CCT3000View::AssertValid() const
{
  //Let the base class do its thing
	CView::AssertValid();

	ASSERT_VALID(&m_aPageStart);

	if (_CT3000EditState.pFindReplaceDlg != NULL)
		ASSERT_VALID(_CT3000EditState.pFindReplaceDlg);
}

void CCT3000View::Dump(CDumpContext& dc) const
{
  //Let the base class do its thing
	CView::Dump(dc);

	AFX_DUMP1(dc, "\nm_aPageStart ", &m_aPageStart);
  AFX_DUMP1(dc, "\nbUseROFileAttributeDuringLoading = ", m_bUseROFileAttributeDuringLoading);

	AFX_DUMP0(dc, "\n Static Member Data:");
	if (_CT3000EditState.pFindReplaceDlg != NULL)
	{
		AFX_DUMP1(dc, "\npFindReplaceDlg = ",	static_cast<void*>(_CT3000EditState.pFindReplaceDlg));
		AFX_DUMP1(dc, "\nbFindOnly = ", _CT3000EditState.bFindOnly);
	}
	AFX_DUMP1(dc, "\nstrFind = ", _CT3000EditState.strFind);
	AFX_DUMP1(dc, "\nstrReplace = ", _CT3000EditState.strReplace);
	AFX_DUMP1(dc, "\nbCase = ", _CT3000EditState.bCase);
	AFX_DUMP1(dc, "\nbWord = ", _CT3000EditState.bWord);
	AFX_DUMP1(dc, "\nbNext = ", _CT3000EditState.bNext);
  AFX_DUMP1(dc, "\nbRegularExpression = ", _CT3000EditState.bRegularExpression);
}
#endif //_DEBUG

void CCT3000View::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	
  CCT3000Ctrl& rCtrl = GetCtrl();

  if (ar.IsLoading())
  {
    //Tell the control not to maintain any undo info while we stream the data 
    rCtrl.Cancel();
    rCtrl.SetUndoCollection(FALSE);

    //Read the data in from the file in blocks
    CFile* pFile = ar.GetFile();
    char Buffer[4096];
    int nBytesRead = 0;
    do
    {
      nBytesRead = pFile->Read(Buffer, 4096);
      if (nBytesRead)
        rCtrl.AddText(nBytesRead, Buffer);
    }
    while (nBytesRead);

    //Set the read only state if required
    if (m_bUseROFileAttributeDuringLoading && ((GetFileAttributes(pFile->GetFilePath()) & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY))
      rCtrl.SetReadOnly(TRUE);
    else
      rCtrl.SetReadOnly(FALSE);

    //Reinitialize the control settings
    rCtrl.SetUndoCollection(TRUE);
    rCtrl.EmptyUndoBuffer();
    rCtrl.SetSavePoint();
    rCtrl.GotoPos(0);
  }
  else
  {
    //Get the length of the document
    int nDocLength = rCtrl.GetLength();

    //Write the data in blocks to disk
    CFile* pFile = ar.GetFile();
    for (int i=0; i<nDocLength; i += 4095) //4095 because data will be returned NULL terminated
    {
      int nGrabSize = nDocLength - i;
      if (nGrabSize > 4095)
        nGrabSize = 4095;

      //Get the data from the control
	    TextRange tr;
	    tr.chrg.cpMin = i;
	    tr.chrg.cpMax = i + nGrabSize;
      char Buffer[4096];
	    tr.lpstrText = Buffer;
	    rCtrl.GetTextRange(&tr);

      //Write it to disk
      pFile->Write(Buffer, nGrabSize);
    }
  }
}

void CCT3000View::OnFilePageSetup() 
{
  //Display a standard page setup dialog
	CPageSetupDialog dlg;

  //Allow the margin settings to be tweaked
  dlg.m_psd.Flags |= PSD_MARGINS;

  //Are we using the metric or imperial system	
  if (m_bUsingMetric)
    dlg.m_psd.Flags |= PSD_INHUNDREDTHSOFMILLIMETERS;
  else
    dlg.m_psd.Flags |= PSD_INTHOUSANDTHSOFINCHES;

  if (m_bPersistMarginSettings)
    LoadMarginSettings();

  //Set the current margin settings to the current value from m_rectMargin 
  dlg.m_psd.rtMargin = m_rMargin;

	//get the current device from the app
	PRINTDLG pd;
	pd.hDevNames = NULL;
	pd.hDevMode = NULL;
  CWinApp* pApp = AfxGetApp();
	pApp->GetPrinterDeviceDefaults(&pd);
	dlg.m_psd.hDevNames = pd.hDevNames;
	dlg.m_psd.hDevMode = pd.hDevMode;

	if (dlg.DoModal() == IDOK)
	{
    //Save the new margin value in to the member variable
    m_rMargin = dlg.m_psd.rtMargin;

    if (m_bPersistMarginSettings)
      SaveMarginSettings();

    //Update the printer settings
		pApp->SelectPrinter(dlg.m_psd.hDevNames, dlg.m_psd.hDevMode);
	}
}

void CCT3000View::OnStyleNeeded(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnCharAdded(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnSavePointReached(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnSavePointLeft(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnModifyAttemptRO(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnDoubleClick(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnUpdateUI(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnModified(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnMacroRecord(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnMarginClick(SCNotification* pSCNotification)
{
  //By default get the line where the click occured and toggle its fold state
  CCT3000Ctrl& rCtrl = GetCtrl();
  int nLine = rCtrl.LineFromPosition(pSCNotification->position);
  rCtrl.ToggleFold(nLine);
}

void CCT3000View::OnNeedShown(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnPainted(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnUserListSelection(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnDwellStart(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnDwellEnd(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnZoom(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnHotSpotClick(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnHotSpotDoubleClick(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnCallTipClick(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnAutoCSelection(SCNotification* /*pSCNotification*/)
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnChange()
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnSetFocus()
{
  //By default do nothing, derived classes may want to do something
}

void CCT3000View::OnKillFocus()
{
  //By default do nothing, derived classes may want to do something
}

BOOL CCT3000View::OnCommand(WPARAM wParam, LPARAM lParam)
{
  HWND hWndControl = reinterpret_cast<HWND>(lParam);
  
  if (hWndControl == GetCtrl().GetSafeHwnd())
  {
    WORD wNotification = HIWORD(wParam);
    switch (wNotification)
    {
      case SCEN_CHANGE:
      {
        OnChange();
        break;
      }
      case SCEN_SETFOCUS:
      {
        OnSetFocus();
        break;
      }
      case SCEN_KILLFOCUS:
      {
        OnKillFocus();
        break;
      }
      default:
      {
        break;
      }
    }

    return TRUE;
  }

  //let the base class do its thing
  return CView::OnCommand(wParam, lParam);
}

BOOL CCT3000View::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  NMHDR* pNMHdr = reinterpret_cast<NMHDR*>(lParam);
  ASSERT(pNMHdr);

  //Is it a notification from the embedded control
  CCT3000Ctrl& rCtrl = GetCtrl();
  if (pNMHdr->hwndFrom == rCtrl.GetSafeHwnd())
  {
    SCNotification* pSCNotification = reinterpret_cast<SCNotification*>(lParam);

    switch (pNMHdr->code)
    {
      case SCN_STYLENEEDED:
      {
        OnStyleNeeded(pSCNotification);
        break;
      }
      case SCN_CHARADDED:
      {
        OnCharAdded(pSCNotification);
        break;
      }
      case SCN_SAVEPOINTREACHED:
      {
        OnSavePointReached(pSCNotification);
        break;
      }
      case SCN_SAVEPOINTLEFT:
      {
        OnSavePointLeft(pSCNotification);
        break;
      }
      case SCN_MODIFYATTEMPTRO:
      {
        OnModifyAttemptRO(pSCNotification);
        break;
      }
      case SCN_DOUBLECLICK:
      {
        OnDoubleClick(pSCNotification);
        break;
      }
      case SCN_UPDATEUI:
      {
        OnUpdateUI(pSCNotification);
        break;
      }
      case SCN_MODIFIED:
      {
        OnModified(pSCNotification);
        break;
      }
      case SCN_MACRORECORD:
      {
        OnMacroRecord(pSCNotification);
        break;
      }
      case SCN_MARGINCLICK:
      {
        OnMarginClick(pSCNotification);
        break;
      }
      case SCN_NEEDSHOWN:
      {
        OnNeedShown(pSCNotification);
        break;
      }
      case SCN_PAINTED:
      {
        OnPainted(pSCNotification);
        break;
      }
      case SCN_USERLISTSELECTION:
      {
        OnUserListSelection(pSCNotification);
        break;
      }
      case SCN_DWELLSTART:
      {
        OnDwellStart(pSCNotification);
        break;
      }
      case SCN_DWELLEND:
      {
        OnDwellEnd(pSCNotification);
        break;
      }
      case SCN_ZOOM:
      {
        OnZoom(pSCNotification);
        break;
      }
      case SCN_HOTSPOTCLICK:
      {
        OnHotSpotClick(pSCNotification);
        break;
      }
      case SCN_HOTSPOTDOUBLECLICK:
      {
        OnHotSpotDoubleClick(pSCNotification);
        break;
      }
      case SCN_CALLTIPCLICK:
      {
        OnCallTipClick(pSCNotification);
        break;
      }
      case SCN_AUTOCSELECTION:
      {
        OnAutoCSelection(pSCNotification);
        break;
      }
      default:
      {
        break;
      }
    }
    return TRUE; // we processed the message
  }
  else
  {
    //let the base class do its thing
    return CView::OnNotify(wParam, lParam, pResult);
  }
}





IMPLEMENT_DYNAMIC(CCT3000Doc, CDocument)

CCT3000Doc::CCT3000Doc()
{
	ASSERT_VALID(this);
}

CCT3000View* CCT3000Doc::GetView() const
{
	//find the first view - if there are no views
	//we must return NULL
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL)
		return NULL;

	//find the first view that is a CCT3000View
	CView* pView;
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCT3000View)))
			return static_cast<CCT3000View*>(pView);
	}

	//can't find one--return NULL
	return NULL;
}

void CCT3000Doc::SetModifiedFlag(BOOL bModified)
{
  CCT3000View* pView = GetView();
//  ASSERT(pView);

  if (pView != 0 && bModified == FALSE)
    pView->GetCtrl().SetSavePoint();

  m_bModified = bModified;
}

BOOL CCT3000Doc::IsModified()
{
  CCT3000View* pView = GetView();
	if (pView == 0)
		return m_bModified;
//  ASSERT(pView);

	return m_bModified || pView->GetCtrl().GetModify();
}

void CCT3000Doc::DeleteContents()
{
  //let the base class do its thing
	CDocument::DeleteContents();

  //Ask our accompanying view to delete its contents
	CWaitCursor wait;
	CCT3000View* pView = GetView();
  if (pView)
    pView->DeleteContents();
}

void CCT3000Doc::Serialize(CArchive& ar)
{
	CCT3000View* pView = GetView();
	ASSERT(pView);
	pView->Serialize(ar);
}

BOOL CCT3000Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
  //Let the base class do its thing
	BOOL bSuccess = CDocument::OnSaveDocument(lpszPathName);
  if (bSuccess)
  {
		CCT3000View* pView = GetView();
		ASSERT(pView);
		
		CCT3000Ctrl& rCtrl = pView->GetCtrl();

		//Tell the control that the document has now been saved
		rCtrl.SetSavePoint();
		rCtrl.SetReadOnly(FALSE);
	}

	return bSuccess;
} 


#ifdef _DEBUG
void CCT3000Doc::AssertValid() const
{
  //Let the base class do its thing
	CDocument::AssertValid();
}

void CCT3000Doc::Dump(CDumpContext& dc) const
{
  //let the base class do its thing
	CDocument::Dump(dc);
}
#endif //_DEBUG

