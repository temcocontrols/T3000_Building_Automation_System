


//////////////////// Macros / Defines /////////////////////////////////////////

#pragma once

#ifndef __SCINTILLADOCVIEW_H__
#define __SCINTILLADOCVIEW_H__


#include "ScintillaCtrl.h"

#ifndef __AFXTEMPL_H__
#pragma message("To avoid this message please put afxtempl.h into your pre compiled header (normally stdafx.h)")
#include <afxtempl.h>
#endif //__AFXTEMPL_H__

#ifndef SCINTILLADOCVIEW_EXT_CLASS
#define SCINTILLADOCVIEW_EXT_CLASS
#endif


//////////////////// Classes //////////////////////////////////////////////////

class SCINTILLADOCVIEW_EXT_CLASS CScintillaFindReplaceDlg : public CFindReplaceDialog
{
public:
//Constructors / Destructors
  CScintillaFindReplaceDlg();

//Methods
  BOOL Create(BOOL bFindDialogOnly, LPCTSTR lpszFindWhat, LPCTSTR lpszReplaceWith = NULL, DWORD dwFlags = FR_DOWN, CWnd* pParentWnd = NULL);
  BOOL GetRegularExpression() const { return m_bRegularExpression; };
  void SetRegularExpression(BOOL bRegularExpression) { m_bRegularExpression = bRegularExpression; };

protected:
	virtual BOOL OnInitDialog();

  afx_msg void OnRegularExpression();
  
//Member variables
  BOOL m_bRegularExpression;

	DECLARE_MESSAGE_MAP()
};

class SCINTILLADOCVIEW_EXT_CLASS CScintillaView : public CView
{
public:
//Constructors / Destructors
  CScintillaView();

//Methods
  CScintillaCtrl& GetCtrl();
	void            SetMargins(const CRect& rMargin) { m_rMargin = rMargin; };
	CRect           GetMargins() const { return m_rMargin; };
  BOOL            GetUseROFileAttributeDuringLoading() const { return m_bUseROFileAttributeDuringLoading; };
  void            SetUseROFileAttributeDuringLoading(BOOL bUseROFileAttributeDuringLoading) { m_bUseROFileAttributeDuringLoading = bUseROFileAttributeDuringLoading; };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
//Printing support
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	virtual long PrintPage(CDC* pDC, CPrintInfo* pInfo, long nIndexStart, long nIndexStop);
  virtual void PrintHeader(CDC* pDC, CPrintInfo* pInfo, RangeToFormat& frPrint);
  virtual void PrintFooter(CDC* pDC, CPrintInfo* pInfo, RangeToFormat& frPrint);

//Search and replace support
	virtual void OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression);
	virtual void TextNotFound(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression, BOOL bReplaced);
  virtual BOOL FindText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression);
	virtual void OnEditFindReplace(BOOL bFindOnly);
	virtual BOOL FindTextSimple(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bRegularExpression);
	virtual void OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,	BOOL bWord, BOOL bRegularExpression, LPCTSTR lpszReplace);
	virtual void OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase, BOOL bWord, BOOL bRegularExpression);
	virtual BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL bWord, BOOL bRegularExpression);
	virtual long FindAndSelect(DWORD dwFlags, TextToFind& ft);
  virtual void AdjustFindDialogPosition();
  virtual CScintillaFindReplaceDlg* CreateFindReplaceDialog();

//Misc methods
	virtual void OnDraw(CDC*);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void DeleteContents();
	virtual void Serialize(CArchive& ar);
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
  static BOOL UserWantsMetric();
  virtual void LoadMarginSettings(const CString& sSection = _T("PageSetup"));
  virtual void SaveMarginSettings(const CString& sSection = _T("PageSetup"));

//Notifications
  virtual void OnStyleNeeded(SCNotification* pSCNotification);
  virtual void OnCharAdded(SCNotification* pSCNotification);
  virtual void OnSavePointReached(SCNotification* pSCNotification);
  virtual void OnSavePointLeft(SCNotification* pSCNotification);
  virtual void OnModifyAttemptRO(SCNotification* pSCNotification);
  virtual void OnDoubleClick(SCNotification* pSCNotification);
  virtual void OnUpdateUI(SCNotification* pSCNotification);
  virtual void OnModified(SCNotification* pSCNotification);
  virtual void OnMacroRecord(SCNotification* pSCNotification);
  virtual void OnMarginClick(SCNotification* pSCNotification);
  virtual void OnNeedShown(SCNotification* pSCNotification);
  virtual void OnPainted(SCNotification* pSCNotification);
  virtual void OnUserListSelection(SCNotification* pSCNotification);
  virtual void OnDwellStart(SCNotification* pSCNotification);
  virtual void OnDwellEnd(SCNotification* pSCNotification);
  virtual void OnZoom(SCNotification* pSCNotification);
  virtual void OnHotSpotClick(SCNotification* pSCNotification);
  virtual void OnHotSpotDoubleClick(SCNotification* pSCNotification);
  virtual void OnCallTipClick(SCNotification* pSCNotification);
  virtual void OnAutoCSelection(SCNotification* pSCNotification);
  virtual void OnChange();
  virtual void OnSetFocus();
  virtual void OnKillFocus();

//Member variables
  CScintillaCtrl   m_Edit;                             //The scintilla control
	CArray<int, int> m_aPageStart;                       //array of starting pages
	CRect            m_rMargin;                          //Margin for printing
	BOOL             m_bFirstSearch;                     //Is this the first search
  BOOL             m_bUseROFileAttributeDuringLoading; //Should we check the RO file attribute to see if the file should be opened in read only mode by scintilla
  BOOL             m_bPrintHeader;                     //Should Headers be printed?
  BOOL             m_bPrintFooter;                     //Should Footers be printed?
  BOOL             m_bUsingMetric;                     //TRUE if the margin is specified in Metric units, else FALSE implies imperial
  BOOL             m_bPersistMarginSettings;           //Should we persist the margin settings for the Page Setup dialog

	afx_msg void OnPaint();
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
  afx_msg void OnUpdateNeedPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
  afx_msg void OnUpdateNeedTextAndFollowingText(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();
  afx_msg void OnEditRedo();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnEditRepeat();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePageSetup();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnFindReplaceCmd(WPARAM, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

  DECLARE_DYNCREATE(CScintillaView)

  friend class CScintillaDoc;
};

class SCINTILLADOCVIEW_EXT_CLASS CScintillaDoc : public CDocument
{
protected: // create from serialization only
	CScintillaDoc();
	DECLARE_DYNAMIC(CScintillaDoc)

//Attributes
public:
	virtual CScintillaView* GetView() const;

//Implementation
public:
	virtual void DeleteContents();
	virtual BOOL IsModified();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual void Serialize(CArchive& ar);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif //__SCINTILLADOCVIEW_H__
