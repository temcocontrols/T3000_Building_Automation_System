
#if !defined(__SCBARG_H__)
#define __SCBARG_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sizecbar.h"

/////////////////////////////////////////////////////////////////////////
// CSCBButton (button info) helper class

class CSCBButton
{
public:
    CSCBButton();

    void Move(CPoint ptTo) {ptOrg = ptTo; };
    CRect GetRect();
    void Paint(CDC* pDC);

    BOOL    bPushed;
    BOOL    bRaised;

protected:
    CPoint  ptOrg;
};

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar control bar

#ifndef baseCSizingControlBarG
#define baseCSizingControlBarG CSizingControlBar
#endif

class CSizingControlBarG : public baseCSizingControlBarG
{
    DECLARE_DYNAMIC(CSizingControlBarG);

// Construction
public:
    CSizingControlBarG();

// Attributes
public:
    virtual BOOL HasGripper() const;

// Operations
public:

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSizingControlBarG)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSizingControlBarG();
    
protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
    virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

protected:
    int     m_cyGripper;
	LOGFONT small_caption_font_;
    CSCBButton m_biHide;

// Generated message map functions
protected:
    //{{AFX_MSG(CSizingControlBarG)
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SCBARG_H__)

