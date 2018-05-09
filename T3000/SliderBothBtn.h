#if !defined(AFX_SLIDERBOTHTICK_H__BB5A7841_264C_11DB_ACFE_0050BA012D9B__INCLUDED_)
#define AFX_SLIDERBOTHTICK_H__BB5A7841_264C_11DB_ACFE_0050BA012D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SliderBothBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SliderBothBtn window

class SliderBothBtn : public CWnd
{
// Construction
public:
	SliderBothBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SliderBothBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd*pMum,BOOL bHorz,BOOL bFst);
	virtual ~SliderBothBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(SliderBothBtn)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BYTE b_Fst,b_BtnDn,b_Horz;
	int i_BtnDnPos;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERBOTHTICK_H__BB5A7841_264C_11DB_ACFE_0050BA012D9B__INCLUDED_)
