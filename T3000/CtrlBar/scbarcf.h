
#if !defined(__SCBARCF_H__)
#define __SCBARCF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// scbarcf.h : header file
//
#include "scbarg.h"

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarCF

#ifndef baseCSizingControlBarCF
#define baseCSizingControlBarCF CSizingControlBarG
#endif

class CSizingControlBarCF : public baseCSizingControlBarCF
{
    DECLARE_DYNAMIC(CSizingControlBarCF)

// Construction
public:
    CSizingControlBarCF();

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Implementation
protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);

protected:
    BOOL    m_bActive; // a child has focus
//    CString m_sFontFace;

// Generated message map functions
protected:
    //{{AFX_MSG(CSizingControlBarCF)
    //}}AFX_MSG
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////

#endif // !defined(__SCBARCF_H__)
