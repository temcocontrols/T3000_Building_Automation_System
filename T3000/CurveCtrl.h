/*==============================================================================================================================*
// CurveCtrl.cpp											   
// 
// Author:		  Sunjoy Chen
// Email(MSN):    cfd@dl.cn
// Copyright 2004, Sunjoy Chen.
//
// Permission to use, copy, modify, distribute for any purpose is hereby
// granted without fee, provided that the above copyright notice and
// this permission notice included in all derived versions. 
// I will be glad to know you using it, let me know by email, and 
// your bugs report are also welcome. 
// This file is provided "as is" with no expressed or implied warranty.
//
// Special thanks to Chris Maunder(for his CGridCtrl which I had learned much from).
==============================================================================================================================*/

#if !defined(AFX_CURVECTRL_H__F4C6E288_12F8_42D9_A172_2A56A1839AFC__INCLUDED_)
#define AFX_CURVECTRL_H__F4C6E288_12F8_42D9_A172_2A56A1839AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define INVALID_POINT	CPoint(-1,-1)

// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
// sucn as DDX_Control(pDX, IDC_MYCURVE, m_myCurveCtrl)
#define CURVECTRL_CLASSNAME    _T("MFCCurveCtrl")  // Window class name

// NOTIFY message, send to owner when curve changed by using mouse
#define CVN_MVALUE_ADD			LVN_FIRST-1		// add a point by clicking left key under edit mode
#define CVN_MVALUE_CHANG		LVN_FIRST-2		// when vertical value of a point change
#define CVN_MVALUE_DELETE		LVN_FIRST-3		// delete a point by clicking right key under edit mode

#define CVN_CURVE_SELECTED		LVN_FIRST-4		// curve is selected by clicking left key of mouse
#define CVN_CURVE_CANCELSELECT	LVN_FIRST-5		// curve is not selected
#define CVN_CURVE_SELECTNONE	LVN_FIRST-6		// no curve is selected


// using struct to send message to owner
// NOTE: that do not changed curve object address or point in owner's response function
typedef struct tagNM_CURVECTRL {
    NMHDR	hdr;
    void*   pCurve;		// curve object pointer
    int		iIndex;		// to curve point: index of key point(KP) on curve near cursor, -1 means no KP.
						// to curve: index of curve in CCurveCtrl near cursor, -1 means no curve
	float	fHori;		// horizontal value where cursor is
	float	fVert;		// vertical value where cursor is
} NM_CURVECTRL;

//////////////////////////////////////////////////////////////////////////
// CCurve: 
// Curve data and curve properties but you must according CCurveCtrl to add/delete/edit data 
// each curve in CCurveCtrl has one CCurve
class CCurve : public CObject
{
	friend class CCurveCtrl;
public:
	CCurve();
	virtual ~CCurve();

	void	ShowCurve(BOOL bShow = TRUE)	{ m_bVisible = bShow; };
	BOOL	IsVisible()                     { return m_bVisible;  };

	void	Select(BOOL bSelect = TRUE)  	{ m_bSelected = bSelect; };
	BOOL	IsSelected()					{ return m_bSelected;    };

	BOOL	SetCurveName(CString& strName)  
		{ if (strName.IsEmpty())  return FALSE; m_strName = strName; return TRUE; };
	CString GetCurveName()                  { return m_strName;    };

	void	SetCurveColor(COLORREF color)   { m_crColor = color;  };
	COLORREF GetCurveColor()                { return m_crColor;   };

	// curve line style 
	void	SetCurveStyle(int iStyle)       { m_iStyle = iStyle;  };
	int		GetCurveStyle()                 { return m_iStyle;    };

	// curve line width
	void	SetCurveWidth(int nWidth)       { m_nWidth = nWidth;  };
	int		GetCurveWidth()                 { return m_nWidth;    };

protected:
	BOOL		m_bSelected;	
	BOOL		m_bVisible;		

	CString		m_strName;		// curve name
	COLORREF	m_crColor;		// curve line color
	int			m_iStyle;		// curve line style
	int			m_nWidth;		// curve line style

	CArray< CPoint, CPoint& >	m_ArrPoint;		// key point of curve in pixel
	CArray< float, float >	m_fArrVertValue;	// vertical value
	CArray< float, float >	m_fArrHoriValue;	// horizontal value

	static float	Distance(const CPoint& pt1, const CPoint& pt2);		// distance between two points
	BOOL			IsPointNearCurve(const CPoint& point, int& iIndex);	// whether the point near curve, iIndex is the nearest point's index
};

/////////////////////////////////////////////////////////////////////////////
// CCurveCtrl window

class CCurveCtrl : public CWnd
{
	DECLARE_DYNCREATE(CCurveCtrl)
// Construction / DeConstrution
public:
	CCurveCtrl();
    BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	virtual ~CCurveCtrl();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurveCtrl)
	//}}AFX_VIRTUAL

	// add curve and data
	int		AddCurve(const CString& strName, COLORREF color = RGB(0, 0, 0), int iStyle = PS_SOLID, int nWidth = 1);
	BOOL	AddData(const CString& strName, float fHori, float fVert);
	BOOL	AddCurveData(const CString& strName, const CArray< float, float >& ArrHori, const CArray< float, float >& ArrVert);
	BOOL	AddData(CCurve* pCurve, float fHori, float fVert);

// delete data, not accomplished 
//	BOOL	DeleteDatabyIndex(CString& strName, int iIndex);
//	BOOL	DeleteDatabyIndex(CCurve* pCurve, int iIndex);
//	BOOL	DeleteDatabyVertValue(CString& strName, float fVert);

	// get curve object pointer
	CCurve* GetCurve(int iIndex);
	CCurve* GetCurve(const CString& strName);
	// get curve index in CCurveCtrl
	int		GetIndex(const CCurve* pCurve);

	// delete curve
	BOOL	Remove(int index);
	BOOL	Remove(const CString& strName);
	void	RemoveAll();	

	int		GetCurveCount();
	int		GetSelectedCount();
	
	// move left or right, all curves and coordinate
	void	Move(BOOL bLeft);
	void	MovePage(BOOL bLeft);

	// shift curve (will change horizontal value of the curve)
	void	CurveLeft(CCurve* pCurve, float fLen);
	void	CurveRight(CCurve* pCurve, float fLen);
	
	// mirror in horizontal
	void	MirrorHori(CCurve* pCurve, float fMid);
	// mirror in vertical
	void	MirrorVert(CCurve* pCurve, float fMid);

	// show all points of all curves
	void	Restore();

	// zoom in (bIn: TRUE) or zoom out(bIn: FALSE)
	BOOL	Zoom(BOOL bIn);

	// axis label 
	void	SetHoriLabel(CString& str);
	CString GetHoriLabel();
	void	SetVertLabel(CString& str);
	CString GetVertLabel();

	// cross line when mouse moving
	void	ShowCross(BOOL bShow);
	BOOL	IsShowCross();

	// margin to show title, etc
	void	SetMargin(const CRect& rect);
	CRect	GetMargin();

	// whether can  edit curve data by using mouse  
	void	EnableEdit(BOOL bEdit);
	BOOL	CanEditCurve();
	
	// line style in background
	void	SetGridLineStyle(int iStyle);
	int		GetGridLineStyle();

protected:
	BOOL	RegisterWindowClass();

	void	DrawCurve(CDC *pdc);
	void	DrawCross(CDC *pdc);
	void	DrawGrid(CDC *pdc, const CRect& rect);	
	
	// calculate point in pixel by horizontal and vertical value
	BOOL	CalculatePoint(float fHori, float fVert, CPoint& point);
	void	ReCalcAllPoint();

	// calculate horizontal and vertical value by point in pixel
	BOOL	CalculateValue(const CPoint& point, float& fHori, float& fVert);
	
	// calculate vertical range in max or min
	void	CalculateVertRange(float fValue, BOOL bMax);

	// sort curve data by horizontal value
	void	SortCurveData(CCurve* pCurve);
	
	int	InsertDataToCurve(CCurve* pCurve, float fHori, float fVert, CPoint point = INVALID_POINT);

    LRESULT SendMessageToParent(int nMessage, CCurve* pCurve, int iIndex, float fHori, float fVert) const;

private:
	// max and min data value in horizontal
	float		m_fHoriMax;
	float		m_fHoriMin;
	// max and min value to be shown
	float		m_fHoriBegin;
	float		m_fHoriEnd;
	// max and min value in vertical
	float		m_fVertMax;
	float		m_fVertMin;

	CRect		m_RectCoord;	// rect to draw curve
	CRect		m_Margin;       // used to save margin in four direction

	// font, may be in future...
//	CFont	m_fontAxis;
//	CFont	m_fontTitle;

	COLORREF	m_crBack;		// background color
	COLORREF	m_crGridLine;	// grid line color
	COLORREF	m_crAxis;		// axis color
//	COLORREF	m_crTitle;		// title color

	int			m_iGridLineStyle;	// grid line style
	BOOL		m_bDrawGrid;	// whether draw grid
	BOOL		m_bShowCross;   // whether show cross

	// label string
	CString		m_strHoriLabel;
	CString		m_strVertLabel;

	BOOL		m_bEdit;		// whether in edit mode
	CCurve*		m_pCurveEdit;	// curve be edited  
	int			m_iCurPoint;    // point be edited

	int			m_iZoom;		// zoom 

	// save value of edited point 
	float		m_fOldHori;
	float		m_fOldVert;

	CToolTipCtrl	m_Tooltip;
	
	CArray< CCurve*, CCurve* >  m_ArrCurve;	

	// Generated message map functions
protected:
	//{{AFX_MSG(CCurveCtrl)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVECTRL_H__F4C6E288_12F8_42D9_A172_2A56A1839AFC__INCLUDED_)
