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
//
// History: 
//     09/09/2004 ~ 09/17/2004  basic function finished
//	   09/27/2004  	            select/deselect curve
//	   11/11/2004               bugs: disable edit if no curve selected;
//	                                  send messages if multi curves selected/deselected.
==============================================================================================================================*/
	   

#include "stdafx.h"
#include "CurveCtrl.h"
#include "MemDC.h"
 
#include <FLOAT.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CURVE_EPSILON       FLT_EPSILON

const int CURVE_NEAR_RANGE	= 4;

/*=======================================================================================*/
// CCurve
CCurve::CCurve()
{
	m_crColor = RGB(0, 0, 255);
	m_iStyle  = PS_SOLID;
	m_nWidth  = 1;	
	m_bSelected = FALSE;	
	m_bVisible = TRUE;	
	m_strName.Empty();
}

CCurve::~CCurve()
{
}

// description  : to judge if there is any key point near given point 
//                or if the given point on the line segment between neighbor key points
// in parameter : point  
// out parameter: iIndex -- index of key point if there is
BOOL CCurve::IsPointNearCurve(const CPoint& point, int& iIndex)
{
	iIndex = -1;
	int nCount = m_ArrPoint.GetSize();
	if (nCount < 1)
		return FALSE;
	
	// m_ArrPoint is already sorted
    int iPt = 0;
	for ( iPt = 0; iPt < nCount; iPt++)
	{
		if (point.x < m_ArrPoint[iPt].x)
		{			
			break;
		}
	}

	// to check if point before the first one or after the last one
	if (iPt == 0 || iPt == nCount) 
	{
		if (iPt == nCount)
			iPt--;
		if (Distance(point, m_ArrPoint[iPt]) < CURVE_NEAR_RANGE)
		{
			iIndex = iPt;
			return TRUE;
		}
		else
			return FALSE;		
	}	

	// to check if point near next key point in  m_ArrPoint
	if (Distance(point, m_ArrPoint[iPt]) < CURVE_NEAR_RANGE)
	{
		iIndex = iPt;
		return TRUE;
	}
	// to check if point near previous key point in  m_ArrPoint
	if (Distance(point, m_ArrPoint[iPt - 1]) < CURVE_NEAR_RANGE)
	{
		iIndex = iPt - 1;
		return TRUE;
	}

	// to check if point near the line defined by two key points:m_ArrPoint[iPt] and m_ArrPoint[iPt - 1]
	if (abs(m_ArrPoint[iPt].x - m_ArrPoint[iPt - 1].x) < CURVE_NEAR_RANGE)
	{		
		if ((point.y > m_ArrPoint[iPt].y && point.y < m_ArrPoint[iPt - 1].y)
			|| (point.y < m_ArrPoint[iPt].y && point.y > m_ArrPoint[iPt - 1].y))
		{
			return TRUE;
		}
		else 
			return FALSE;
	}
	else // the line defined by two key points£ºy = k * x + b
	{
		// k = (y2 - y1) / (x2 - x1)
		float k = float(m_ArrPoint[iPt].y - m_ArrPoint[iPt - 1].y) 
			      / float(m_ArrPoint[iPt].x - m_ArrPoint[iPt - 1].x);
		// b = (x2 * y1 - x1 * y2) / (x2 - x1)
		float b = float(m_ArrPoint[iPt].x * m_ArrPoint[iPt - 1].y - m_ArrPoint[iPt - 1].x * m_ArrPoint[iPt].y) 
			      / float(m_ArrPoint[iPt].x - m_ArrPoint[iPt - 1].x);
		
		float y = k * point.x + b;
		if (abs(int(y - point.y)) < CURVE_NEAR_RANGE)
			return TRUE;
	}		

	return FALSE;
}

// description  : static function, to calculate distance between two points
// in paramenter: pt1, pt2
float CCurve::Distance(const CPoint& pt1, const CPoint& pt2)
{
	return (float)sqrt((float)((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y)));
}

/*=======================================================================================*/
// CCurveCtrl

IMPLEMENT_DYNCREATE(CCurveCtrl, CWnd)

// construct 
CCurveCtrl::CCurveCtrl()  
{
    RegisterWindowClass();

    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState->m_bNeedTerm && !AfxOleInit())
	{
		AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
	}

// init:
	m_fHoriMax = m_fVertMax = -FLT_MAX / 2;
	m_fHoriMin = m_fVertMin = FLT_MAX / 2;

	m_Margin = CRect(80, 50, 70, 40);
	m_crBack = RGB(255, 255, 255);		// white
	m_crGridLine = RGB(192, 192, 192);
	m_iGridLineStyle = PS_DOT;			// as: - - - - - 

	m_bShowCross	 = FALSE;
	m_crAxis = RGB(0, 0, 0);			// black

	m_bEdit   = FALSE;
	m_pCurveEdit = NULL;
	m_iCurPoint  = -1;
	m_strHoriLabel = _T("X");
	m_strVertLabel = _T("Y");

	m_iZoom = 0;
}

// deconstruct 
CCurveCtrl::~CCurveCtrl()
{
	// free memory
	RemoveAll();
}

BEGIN_MESSAGE_MAP(CCurveCtrl, CWnd)
	//{{AFX_MSG_MAP(CCurveCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveCtrl message handlers
 
// description :create this window, use like any other window create control
// in parameter: rect       -- window rect
//               pParentWnd -- pointer of parent window 
//               nID        -- resource ID
//               dwStyle    -- style
BOOL CCurveCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
    ASSERT(pParentWnd->GetSafeHwnd());

    if (!CWnd::Create(CURVECTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
        return FALSE;

	//Add ToolTips
	if (!m_Tooltip.Create(this))
		TRACE(_T("Unable to create tip window for CCurveCtrl."));
	else if (!m_Tooltip.AddTool(this, _T("Control ToolTips")))
		TRACE(_T("Unable to add tip for the control window for CCurveCtrl."));
	else
		m_Tooltip.Activate(TRUE);

    return TRUE;
}

BOOL CCurveCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CURVECTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = CURVECTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CCurveCtrl::OnPaint() 
{
	CPaintDC dc(this); 
 	CTMemDC memdc(&dc); // use CMemDC to avoid flicker

	CRect rect;
	GetClientRect(&rect);

	// background and margin    
	CBrush bkBrush(m_crBack);
	CBrush* pOldBrush = memdc.SelectObject(&bkBrush);	
 	memdc.FillRect(rect, &bkBrush);
	rect.InflateRect(1, 1, 0, 0);
	memdc.DrawEdge(&rect, EDGE_ETCHED, BF_TOPLEFT);
	memdc.SelectObject(pOldBrush);

	m_RectCoord = rect;
	m_RectCoord.DeflateRect(m_Margin);

 	DrawGrid(&memdc, m_RectCoord);

	ReCalcAllPoint();	// ! must do this before DrawCurve(...) 
	DrawCurve(&memdc);

	DrawCross(&memdc);
}

// description : draw grid line
// in parameter: pdc  -- pointer of CDC
//               rect -- paint area
void CCurveCtrl::DrawGrid(CDC *pdc, const CRect& rect)
{
	CPen	penStroke;	// pen for drawing grid line
	CPen	penGrid;
	
	// cells in horizontal and vertical, may be as member viariables of CCurveCtrl
	int nGrid = 10;		
	int nVertGrid = 10; 

	CPen penRect;
	penRect.CreatePen(0, 1, m_crAxis);
	CPen *pOldPentmp = pdc->SelectObject(&penRect);

	CBrush bkBrush(m_crBack);
	CBrush* pOldBrush=pdc->SelectObject(&bkBrush);	
		
	// draw rectangle aera contains all
	{
		CRect rctmp = rect;
		rctmp.DeflateRect(-1, -1, -1, -1);
		pdc->Rectangle(&rctmp);
	}
	pdc->SelectObject(pOldBrush);
 	pdc->SelectObject(pOldPentmp);

	penGrid.CreatePen(0, 1, m_crGridLine);
	CPen *pOldPen=pdc->SelectObject(&penGrid);

	penStroke.CreatePen(m_iGridLineStyle, 1, m_crGridLine);// grid line style
	pdc->SelectObject(&penStroke);
	// to draw grid line 
	{
		for(int i = 1; i < nVertGrid; i++)// in horizontal
		{
			pdc->MoveTo(rect.left, rect.top + i * rect.Height() / nVertGrid);
			pdc->LineTo(rect.right, rect.top + i * rect.Height() / nVertGrid);
		}

		for(int k = 1; k < nGrid; k++)// in vertical
		{
			pdc->MoveTo(rect.left + rect.Width() * k / nGrid, rect.bottom);
			pdc->LineTo(rect.left + rect.Width() * k / nGrid, rect.top);
		}
	}
	
	// draw label in horizontal
	pdc->SetTextColor(m_crAxis);	// the same color as axis' color
	pdc->SetBkMode(TRANSPARENT);
	int nTextHei;
	nTextHei = pdc->GetTextExtent(m_strHoriLabel).cy; // length of horizontal label

	CRect rectLabel;
	rectLabel.left   = m_RectCoord.left;
	rectLabel.right  = m_RectCoord.right;
	rectLabel.top	 = m_RectCoord.bottom + nTextHei + 15;
	rectLabel.bottom = rectLabel.top + nTextHei;
	pdc->DrawText(m_strHoriLabel, &rectLabel, DT_CENTER|DT_SINGLELINE);	

	// draw scale
	if (m_ArrCurve.GetSize())
	{
		CString   strScale;
		float	  fDlt = (m_fHoriEnd - m_fHoriBegin) / nGrid;

		if (fabs(fDlt) > CURVE_EPSILON)
		{
			for (int iScale = 0; iScale <= nGrid; iScale++)
			{
				strScale.Format(_T("%.1f"), int(10*(m_fHoriBegin + fDlt * iScale)) / 10.0f );
				CSize szLabel = pdc->GetTextExtent(strScale);
				if (szLabel.cx * (nGrid + 1) > m_RectCoord.Width() && (iScale % 2) == 1 )
					continue;

				pdc->TextOut(m_RectCoord.left + iScale * m_RectCoord.Width() / nGrid - szLabel.cx / 2, m_RectCoord.bottom + szLabel.cy / 2, strScale);
			}
		}
	}

	// draw label in vertical 
	CFont	*curFont=pdc->GetCurrentFont();
	LOGFONT	curLogFont;
	curFont->GetLogFont(&curLogFont);

	curLogFont.lfEscapement = 900; // rotate 90 degree
	CFont	newFont;
	newFont.CreateFontIndirect(&curLogFont);
	CFont* pOldFont = pdc->SelectObject(&newFont);

	CSize szLabel = pdc->GetTextExtent(m_strVertLabel);
	pdc->TextOut(m_RectCoord.left - 60, m_RectCoord.top + (szLabel.cy + m_RectCoord.Height()) / 2, m_strVertLabel);
	pdc->SelectObject(pOldFont);

	// draw scale
	if (m_ArrCurve.GetSize())
	{
		CString   strScale;
		float	  fDlt = (m_fVertMax - m_fVertMin) / nGrid;

		if (fabs(fDlt) > CURVE_EPSILON)
		{
			for (int iScale = 0; iScale <= nVertGrid; iScale++)
			{
				// use int(...) to avoid "-0.0"
				strScale.Format(_T("%.2f"), int(0.5f + 100 * (m_fVertMin + fDlt * iScale)) / 100.0f ); 
				CSize szLabel = pdc->GetTextExtent(strScale);
				if (szLabel.cy * (nVertGrid + 1) > m_RectCoord.Height() && (iScale % 2 == 1))
					continue;

				pdc->TextOut(m_RectCoord.left - szLabel.cx - 2, m_RectCoord.bottom - m_RectCoord.Height() * iScale/ nVertGrid - szLabel.cy / 2, strScale);
			}
		}
	}
	
	pdc->SelectObject(pOldPen);
}

// decription : connect neighbor points by drawing lines
void CCurveCtrl::DrawCurve(CDC *pdc)
{
	CPen		Pen;
	CBrush		brush;
	CPen*		pOldPen;
	CBrush*		pOldBrush;
	CCurve*		pCurve;

	int		iPoint;
	CRect	rect;
	int		nRadius;
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{		
		pCurve = m_ArrCurve[iCurve];
		if ((!pCurve->IsVisible()) || (pCurve->m_ArrPoint.GetSize() < 1))
			continue;

		// create pen using CCurve's member variables
		Pen.CreatePen(pCurve->m_iStyle, pCurve->m_nWidth, pCurve->m_crColor);
		pOldPen = pdc->SelectObject(&Pen);		
		brush.CreateSolidBrush(pCurve->m_crColor);
		pOldBrush = pdc->SelectObject(&brush);	
		nRadius = pCurve->m_nWidth + 1;

		if ((1 == pCurve->m_ArrPoint.GetSize()) && (pCurve->m_ArrPoint[0] != INVALID_POINT))
		{
			// if there is only one point in m_ArrPoint, draw it 
			rect.top	= pCurve->m_ArrPoint[0].y - nRadius / 2;
			rect.bottom = pCurve->m_ArrPoint[0].y + nRadius / 2;
			rect.left	= pCurve->m_ArrPoint[0].x - nRadius / 2;
			rect.right	= pCurve->m_ArrPoint[0].x + nRadius / 2;

			pdc->Rectangle(rect);			
		}
		else   // draw connected line between neighbor key points in m_ArrPoint
		{
			// flag for whether already begin draw line
			BOOL	bStart = FALSE;		

			// make sure that line get to margin line if there are points outside drawing area 
			for (iPoint = 0; iPoint < pCurve->m_ArrPoint.GetSize(); iPoint++)
			{
					// if point in drawing area(INVALID_POINT)
					if (pCurve->m_ArrPoint[iPoint] == INVALID_POINT)
					{
							if (!bStart)  // if not begin drawing£¬then INVALID_PINT is foregoing points, don't draw it
							{
								continue;
							}
							else if (iPoint != pCurve->m_ArrPoint.GetUpperBound())  // if already beginning, then INVALID_PINT isback points
							{
								CPoint ptside;

								// using line equation to calculate value , and then point in pixel
								// k = (y2 - y1) / (x2 - x1)
								float k = float(pCurve->m_fArrVertValue[iPoint] - pCurve->m_fArrVertValue[iPoint + 1]) 
										  / float(pCurve->m_fArrHoriValue[iPoint] - pCurve->m_fArrHoriValue[iPoint + 1]);
								//b = (x2 * y1 - x1 * y2) / (x2 - x1)
								float b = float(pCurve->m_fArrHoriValue[iPoint] * pCurve->m_fArrVertValue[iPoint + 1]
												 - pCurve->m_fArrHoriValue[iPoint + 1] * pCurve->m_fArrVertValue[iPoint]) 
										 / float(pCurve->m_fArrHoriValue[iPoint] - pCurve->m_fArrHoriValue[iPoint + 1]);
								float fVside = k * m_fHoriEnd + b;
								
								CalculatePoint(m_fHoriEnd, fVside, ptside);
								pdc->LineTo(ptside);
							}

							break;   // return if connect to margin line
					}// end INVALID_POINT 

					if (!bStart) // begin drawing after first validate point
					{
						bStart = TRUE;

						// if the first validate point is out of drawing area, draw line to margin line
						if (iPoint > 0)
						{
							CPoint   ptside;
							if (fabs(pCurve->m_fArrHoriValue[iPoint] - pCurve->m_fArrHoriValue[iPoint - 1]) > CURVE_EPSILON)
							{
								// k = (y2 - y1) / (x2 - x1)
								float k = float(pCurve->m_fArrVertValue[iPoint] - pCurve->m_fArrVertValue[iPoint - 1]) 
										  / float(pCurve->m_fArrHoriValue[iPoint] - pCurve->m_fArrHoriValue[iPoint - 1]);
								//b = (x2 * y1 - x1 * y2) / (x2 - x1)
								float b = float(pCurve->m_fArrHoriValue[iPoint] * pCurve->m_fArrVertValue[iPoint - 1]
												 - pCurve->m_fArrHoriValue[iPoint - 1] * pCurve->m_fArrVertValue[iPoint]) 
										 / float(pCurve->m_fArrHoriValue[iPoint] - pCurve->m_fArrHoriValue[iPoint - 1]);
								float fVside = k * m_fHoriBegin + b;

								CalculatePoint(m_fHoriBegin, fVside, ptside);
							}
							else
							{
								ptside = CPoint(m_RectCoord.left, pCurve->m_ArrPoint[iPoint-1].y);
							}
							pdc->MoveTo(ptside);
						}
						else
							pdc->MoveTo(pCurve->m_ArrPoint[iPoint]);
					} // end first validate point
				
					// connect line between neighbor points
					pdc->LineTo(pCurve->m_ArrPoint[iPoint]);
					if (pCurve->m_bSelected)
					{
						rect.top	= pCurve->m_ArrPoint[iPoint].y - nRadius;
						rect.bottom = pCurve->m_ArrPoint[iPoint].y + nRadius;
						rect.left	= pCurve->m_ArrPoint[iPoint].x - nRadius;
						rect.right	= pCurve->m_ArrPoint[iPoint].x + nRadius;
						pdc->Ellipse(rect);
					}
			} //end for pCurve->m_ArrPoint 
		}
		pdc->SelectObject(pOldBrush);
		pdc->SelectObject(pOldPen);
		brush.DeleteObject();
		Pen.DeleteObject();	
	} // end for m_ArrCurve 
}

// decription  : add a curve
// in parameter: strName -- curve name, must not empty
//               color   -- curve line color
//               iStyle  -- curve line style
//               nWidth  -- curve line width
// return value: index of the added curve in CCurveCtrl
int	CCurveCtrl::AddCurve(const CString& strName, COLORREF color, int iStyle, int nWidth)
{
	if (strName.IsEmpty())
		return -1;
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{
		if (strName == m_ArrCurve[iCurve]->m_strName)
			return -1;
	}

	CCurve*	pCurve = new CCurve;
	pCurve->m_strName = strName;
	pCurve->m_crColor = color;
	pCurve->m_iStyle = iStyle;
	pCurve->m_nWidth = nWidth;

	// restore zoom
	m_iZoom = 0;
	
	return m_ArrCurve.Add(pCurve);
}

// description  : add data to an exist curve
// in parameter : strName    -- curve name
//                fHoriValue -- horizontal value
//                fVertValue -- vertical value
// return value : TRUE: data added to curve; FALSE: curve is empty or there are no curve named as strName
BOOL CCurveCtrl::AddData(const CString& strName, float fHoriValue, float fVertValue)
{
	if (strName.IsEmpty()) 
		return FALSE;

	CCurve*		pCurve = NULL;
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{
		if (strName == m_ArrCurve[iCurve]->m_strName)
		{
			pCurve = m_ArrCurve[iCurve];
			break;
		}
	}

	// following AddData(...) will check whether pCurve is NULL
	return AddData(pCurve, fHoriValue, fVertValue);
}

// description  : add data to an exist curve
// in parameter : pCurve     -- curve pointer in this CCurveCtrl
//                fHoriValue -- horizontal value
//                fVertValue -- vertical value
// return value : TRUE: data added to curve; FALSE: pointer of curve invalidate
BOOL CCurveCtrl::AddData(CCurve* pCurve, float fHori, float fVert)
{
	if (!pCurve)
		return FALSE;

	// ad data to array, inserted position decided by horizontal value
	ASSERT(pCurve->m_fArrHoriValue.GetSize() == pCurve->m_fArrVertValue.GetSize());
	InsertDataToCurve(pCurve, fHori, fVert);

	// save max and min values in horizontal
	m_fHoriMax = max(m_fHoriMax, fHori);
	m_fHoriMin = min(m_fHoriMin, fHori);

	CalculateVertRange(fVert, TRUE);
	CalculateVertRange(fVert, FALSE);
	m_fHoriBegin = m_fHoriMin;
	m_fHoriEnd   = m_fHoriMax;

	return TRUE;	
}

// decription  : insert data to data array which already sorted by horizontal value
// in parameter: pCurve -- Curve object pointer
//               fHori  -- horizontal value 
//               fVert  -- vertical value
//               point  -- corresponding point in pixel
// return value: index of the added data in data array
int CCurveCtrl::InsertDataToCurve(CCurve* pCurve, float fHori, float fVert, CPoint point)
{
	if (!pCurve)
		return -1;
	ASSERT(pCurve->m_fArrHoriValue.GetSize() == pCurve->m_fArrVertValue.GetSize());
	ASSERT(pCurve->m_fArrHoriValue.GetSize() == pCurve->m_ArrPoint.GetSize());
	     int iIndex =0;
	for (int iIndex = pCurve->m_fArrHoriValue.GetUpperBound(); iIndex >= 0; iIndex--)
	{
		if (pCurve->m_fArrHoriValue[iIndex] < fHori)
		{
			break;
		}		
	}

	if (iIndex == pCurve->m_fArrHoriValue.GetUpperBound())
	{
		pCurve->m_ArrPoint.Add(point);
		pCurve->m_fArrHoriValue.Add(fHori);
		pCurve->m_fArrVertValue.Add(fVert);
	}
	else
	{
		pCurve->m_ArrPoint.InsertAt(iIndex + 1, point);
		pCurve->m_fArrHoriValue.InsertAt(iIndex + 1, fHori);
		pCurve->m_fArrVertValue.InsertAt(iIndex + 1, fVert);
	}
		
	return iIndex + 1;
}

// decription  : add one curve and copy it's data to data array
// in parameter: strName -- curve name
//               ArrHori -- data array of horizontal value
//               ArrVert -- data array of vertical value
// return value: TRUE if success
BOOL CCurveCtrl::AddCurveData(const CString& strName, const CArray< float, float >& ArrHori, const CArray< float, float >& ArrVert)
{
	int iCurve = AddCurve(strName);
	if (iCurve < 0 || (ArrHori.GetSize() != ArrVert.GetSize()))
		return FALSE;		

	// copy data
	m_ArrCurve[iCurve]->m_fArrHoriValue.Copy(ArrHori);
	m_ArrCurve[iCurve]->m_fArrVertValue.Copy(ArrVert);
	// make sure point array has the same size as the data arrays
	m_ArrCurve[iCurve]->m_ArrPoint.SetSize(ArrHori.GetSize()); 

	// remember the max and min value
	int iIndex;
	for (iIndex = 0; iIndex < ArrHori.GetSize(); iIndex++)
	{
		m_fHoriMax = max(m_fHoriMax, ArrHori[iIndex]);
		m_fHoriMin = min(m_fHoriMin, ArrHori[iIndex]);
	}

	// to adjust max and min value in vertical for mouse editing
	float fVMax = -FLT_MAX / 2;
	float fVMin = FLT_MAX / 2;
	for (iIndex = 0; iIndex < ArrVert.GetSize(); iIndex++)
	{
		fVMax = max(fVMax, ArrVert[iIndex]);
		fVMin = min(fVMin, ArrVert[iIndex]);		
	}
		
	SortCurveData(m_ArrCurve[iCurve]);

	CalculateVertRange(fVMax, TRUE);
	CalculateVertRange(fVMin, FALSE);

	m_fHoriBegin = m_fHoriMin;
	m_fHoriEnd   = m_fHoriMax;
	
	return TRUE;
}

// decription  : sort data to make sure all arrays order by horizontal value
void CCurveCtrl::SortCurveData(CCurve* pCurve)
{
	int nCount = pCurve->m_fArrHoriValue.GetSize();
	
	ASSERT(nCount == pCurve->m_fArrVertValue.GetSize());
	ASSERT(nCount == pCurve->m_ArrPoint.GetSize());

	int		iPos;
	float	fTemp;
	CPoint	PtTemp;
	for (int iPre = 0; iPre < nCount - 1; iPre++)
	{
		iPos = iPre;
		for (int iAft = iPre + 1; iAft < nCount; iAft++)
		{
			if (pCurve->m_fArrHoriValue[iPre] > pCurve->m_fArrHoriValue[iAft])
			{
				iPos = iAft;				
			}
		}

		// exchange
		if (iPos != iPre)
		{
			// horizontal value
			fTemp = pCurve->m_fArrHoriValue[iPre];
			pCurve->m_fArrHoriValue[iPre] = pCurve->m_fArrHoriValue[iPos];
			pCurve->m_fArrHoriValue[iPos] = fTemp;

			// vertical value
			fTemp = pCurve->m_fArrVertValue[iPre];
			pCurve->m_fArrVertValue[iPre] = pCurve->m_fArrVertValue[iPos];
			pCurve->m_fArrVertValue[iPos] = fTemp;

			// point
			PtTemp = pCurve->m_ArrPoint[iPre];
			pCurve->m_ArrPoint[iPre] = pCurve->m_ArrPoint[iPos];
			pCurve->m_ArrPoint[iPos] = PtTemp;
		}
	}
}

// decription  : get CCurve object pointer by its index in this CCurveCtrl 
CCurve* CCurveCtrl::GetCurve(int iIndex)
{
	if (iIndex > m_ArrCurve.GetUpperBound() || iIndex < 0)
		return NULL;

	return m_ArrCurve[iIndex];
}

// decription  : get CCurve object pointer by its name
CCurve* CCurveCtrl::GetCurve(const CString& strName)
{
	CCurve*		pCurve = NULL;
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{
		if (strName == m_ArrCurve[iCurve]->m_strName)
		{
			pCurve = m_ArrCurve[iCurve];
			break;			
		}
	}

	return pCurve;
}

// decription  : get index in this CCurveCtrl by CCurve object pointer
int	CCurveCtrl::GetIndex(const CCurve* pCurve)
{
	int iPos = -1;
	for (int iCur = 0; iCur < m_ArrCurve.GetSize(); iCur++)
	{
		if (pCurve == m_ArrCurve[iCur])
		{
			iPos = iCur;
			break;
		}
	}

	return iPos;
}

// decription  : remove one curve by its index
BOOL CCurveCtrl::Remove(int index)
{
	if (index < 0 || index > m_ArrCurve.GetUpperBound())
		return FALSE;

	delete m_ArrCurve[index];
	m_ArrCurve[index] = NULL;
	m_ArrCurve.RemoveAt(index);

	// disable edit mode if no curve selected
	if (GetSelectedCount() < 1)
		m_bEdit = FALSE;

	return TRUE;
}

// decription  : remove one curve by its name
BOOL CCurveCtrl::Remove(const CString& strName)
{
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{
		if (strName == m_ArrCurve[iCurve]->m_strName)
		{
			delete m_ArrCurve[iCurve];
			m_ArrCurve[iCurve] = NULL;
			m_ArrCurve.RemoveAt(iCurve);

			return TRUE;
		}
	}

	return FALSE;
}

// decription  : remove all curve object in this CCurveCtrl
void CCurveCtrl::RemoveAll()
{
	for (int iC = 0; iC < m_ArrCurve.GetSize(); iC++)
	{
		delete m_ArrCurve[iC];
		m_ArrCurve[iC] = NULL;		
	}
	m_ArrCurve.RemoveAll();

	// there are no curve, so disable edit
	m_bEdit = FALSE;
}

// decription  : get curve count in this CCurveCtrl
int	CCurveCtrl::GetCurveCount() 
{
	return m_ArrCurve.GetSize(); 
}

// decription  : get selected curve count
int	CCurveCtrl::GetSelectedCount()
{
	int nSelected = 0;
	
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{
		if (m_ArrCurve[iCurve]->m_bSelected)
			nSelected++;
	}

	return nSelected;
}

// decription   : calculate point in pixel by horizontal value and vertical value
// in parameter : fHori -- horizontal value
//                fVert -- vertical value
// out parameter: point -- point in pixel, INVALID_POINT if out of drawing area
BOOL CCurveCtrl::CalculatePoint(float fHori, float fVert, CPoint& point)
{
	point = INVALID_POINT;

	float fHoriDiff = m_fHoriEnd - m_fHoriBegin;
	float fHoriDlt = fHori - m_fHoriBegin;
	if ( fHoriDlt < 0 || fHoriDiff <= 0 || (m_fHoriEnd - fHori) < 0)
		return FALSE;

	float fVertDiff = m_fVertMax - m_fVertMin;
	float fVertDlt  = fVert - m_fVertMin;
	if ( fVertDlt < 0 || fVertDiff <= 0 )
		return FALSE;

	float fHoriCof = fHoriDlt / fHoriDiff;	
	float fVertCof = fVertDlt / fVertDiff;		
	
	point.x = m_RectCoord.left + int(m_RectCoord.Width() * fHoriCof + 0.5f);
	point.y = m_RectCoord.bottom - int(m_RectCoord.Height() * fVertCof + 0.5f);


	return TRUE;
}

// decription   : calculate horizontal value and vertical value by point in pixel
// in parameter : point -- point in pixel
// out parameter: fHori -- horizontal value
//                fVert -- vertical value
// return value : TRUE if success, and FALSE if point out of rect area in which drawing curve 
BOOL CCurveCtrl::CalculateValue(const CPoint& point, float& fHori, float& fVert)
{
	if (!m_ArrCurve.GetSize())
		return FALSE;
	
	// if point is out of drawing area
	if (m_RectCoord.PtInRect(point))
	{		
		float fHCof = float(point.x - m_RectCoord.left) / float(m_RectCoord.Width());
		fHori = m_fHoriBegin + fHCof * (m_fHoriEnd - m_fHoriBegin);

		float fVCof = float(point.y - m_RectCoord.top) / float(m_RectCoord.Height());
		fVert = m_fVertMax - fVCof * (m_fVertMax - m_fVertMin); 

		return TRUE;
	}
	return FALSE;
}

void CCurveCtrl::ReCalcAllPoint()
{
	
	for (int iCurve = 0; iCurve < m_ArrCurve.GetSize(); iCurve++)
	{
		CCurve*	pCurve = m_ArrCurve[iCurve];
		ASSERT(pCurve->m_fArrHoriValue.GetSize() == pCurve->m_fArrVertValue.GetSize());

		pCurve->m_ArrPoint.SetSize(pCurve->m_fArrHoriValue.GetSize());
		
		for (int iPoint = 0; iPoint < pCurve->m_ArrPoint.GetSize(); iPoint++)
		{
			CalculatePoint(pCurve->m_fArrHoriValue[iPoint], pCurve->m_fArrVertValue[iPoint], pCurve->m_ArrPoint[iPoint]);
		}
	}
}

// decription   : draw cross lines and output string of corresponding values
void CCurveCtrl::DrawCross(CDC *pdc)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);	

	if(PtInRect(m_RectCoord, point)) 
	{
		// cross lines
		if (/*!m_bEdit &&*/ m_bShowCross)
		{	
			// color: using m_crAxis 
			CPen pentmp(PS_SOLID, 1, m_crAxis);
			CPen *pOldPentmp=pdc->SelectObject(&pentmp);

			pdc->MoveTo(point.x, m_RectCoord.top);
			pdc->LineTo(point.x, m_RectCoord.bottom);
			pdc->MoveTo(m_RectCoord.left, point.y);
			pdc->LineTo(m_RectCoord.right, point.y);

			pdc->SelectObject(pOldPentmp);
		}

		// out put string to top-right outside of the drawing rect
		float		fHori, fVert;
		CString		str;
		CSize		szText;
		if (CalculateValue(point, fHori, fVert))
		{
			str.Format(_T("%.1f, %.1f"), int(10 * fHori) / 10.0f, int(10 * fVert) / 10.0f);
			szText = pdc->GetTextExtent(str);
			pdc->TextOut(m_RectCoord.right - szText.cx, m_RectCoord.top - szText.cy - 2, str);				
		}
	}
}

// decription   : disable base function to avoid flicker
BOOL CCurveCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;	
}

// description : move curves and coordinate scale, data of curves not changed
// in parameter: bLeft -- TRUE: move left; FALSE: move right
void CCurveCtrl::Move(BOOL bLeft)
{
	int nHoriGrid = 10;
	float fPerGird = (m_fHoriEnd - m_fHoriBegin) / nHoriGrid;

	if (bLeft && (m_fHoriBegin + fPerGird < m_fHoriMax))      
	{
		// move left, and make sure there is at least one visible point in drawing area
		m_fHoriBegin += fPerGird;
		m_fHoriEnd   += fPerGird;
	}
	else if (!bLeft && (m_fHoriEnd - fPerGird > m_fHoriMin))  
	{
		// move right
		m_fHoriBegin -= fPerGird;
		m_fHoriEnd   -= fPerGird;
	}
	
	Invalidate();
}

// description : move one page, refer to Move(BOOL bLeft);
void CCurveCtrl::MovePage(BOOL bLeft)
{
	float fPerPage = m_fHoriEnd - m_fHoriBegin;

	if (bLeft && (m_fHoriBegin + fPerPage < m_fHoriMax))     // move left
	{
		m_fHoriBegin += fPerPage;
		m_fHoriEnd   += fPerPage;
	}
	else if (!bLeft && (m_fHoriEnd - fPerPage > m_fHoriMin)) // move right
	{
		m_fHoriBegin -= fPerPage;
		m_fHoriEnd   -= fPerPage;
	}
	Invalidate();
}

// description : shift a curve right, will modify the data of the curve
//               that is, add offset to each element in data array of horizontal value
// in parameter: pCurve -- curve object
//               fLen   -- offset
void CCurveCtrl::CurveRight(CCurve* pCurve, float fLen)
{
	if (pCurve == NULL)
		return;

	for (int iIndex = 0; iIndex < pCurve->m_ArrPoint.GetSize(); iIndex++)
	{
		pCurve->m_fArrHoriValue[iIndex] += fLen;
		// max value 
		m_fHoriMax = max(m_fHoriMax, pCurve->m_fArrHoriValue[iIndex]);
	}
	
	Restore();
}

// description : shift a curve left, will modify the data of the curve
//               that is, subtract offset to each element in data array of horizontal value
// in parameter: pCurve -- curve object
//               fLen   -- offset
void CCurveCtrl::CurveLeft(CCurve* pCurve, float fLen)
{
	if (pCurve == NULL)
		return;

	for (int iIndex = 0; iIndex < pCurve->m_ArrPoint.GetSize(); iIndex++)
	{
		pCurve->m_fArrHoriValue[iIndex] -= fLen;
		// min value
		m_fHoriMin = min(m_fHoriMin, pCurve->m_fArrHoriValue[iIndex]);
	}
	
	Restore();
}

// description : show all points in all visible curves
void CCurveCtrl::Restore()
{
	float fVMin, fVMax;
	m_fHoriMin = FLT_MAX / 2;
	m_fHoriMax = -FLT_MAX / 2;
	m_fVertMin = fVMin = FLT_MAX / 2;
	m_fVertMax = fVMax = -FLT_MAX / 2;

	// max and min value
	for (int iCur = 0; iCur < m_ArrCurve.GetSize(); iCur++)
	{
		for (int iPt = 0; iPt < m_ArrCurve[iCur]->m_fArrHoriValue.GetSize(); iPt++)
		{
			m_fHoriMin = min(m_fHoriMin, m_ArrCurve[iCur]->m_fArrHoriValue[iPt]);
			m_fHoriMax = max(m_fHoriMax, m_ArrCurve[iCur]->m_fArrHoriValue[iPt]);
			
			fVMin = min(fVMin, m_ArrCurve[iCur]->m_fArrVertValue[iPt]);
			fVMax = max(fVMax, m_ArrCurve[iCur]->m_fArrVertValue[iPt]);	
		}
	}

	m_fHoriBegin = m_fHoriMin;
	m_fHoriEnd   = m_fHoriMax;

	// adjust
	CalculateVertRange(fVMin, FALSE);
	CalculateVertRange(fVMax, TRUE);

	// restore zoom
	m_iZoom = 0;

	Invalidate();
}

// description : zoom in or out curves
// in parameter: bIn -- TRUE: out; FALSE: in
BOOL CCurveCtrl::Zoom(BOOL bIn)
{
	bIn ? m_iZoom++ : m_iZoom--;
	if (m_iZoom == 4)
	{
		m_iZoom--;
		return FALSE;
	}
	else if (m_iZoom == -4)
	{
		m_iZoom++;
		return FALSE;
	}

	float fDiff = (m_fHoriMax - m_fHoriMin) / 4.0f;
	
	m_fHoriEnd = m_fHoriMax - m_iZoom * fDiff;
	Invalidate();

	return TRUE;
}

// description : mirror one curve in horizontal
// in parameter: pCurve -- curve object
//               fMid   -- center value 
void CCurveCtrl::MirrorHori(CCurve* pCurve, float fMid)
{
	if (!pCurve)
		return;

	for (int iIndex = 0; iIndex < pCurve->m_fArrHoriValue.GetSize(); iIndex++)
	{
		pCurve->m_fArrHoriValue[iIndex] = 2 * fMid - pCurve->m_fArrHoriValue[iIndex];
	}

	// resort
	SortCurveData(pCurve);	
	
	Restore();
	
	Invalidate();
}

// description : mirror one curve in vertical
// in parameter: pCurve -- curve object
//               fMid   -- center value 
void CCurveCtrl::MirrorVert(CCurve* pCurve, float fMid)
{
	if (!pCurve)
		return;

	for (int iIndex = 0; iIndex < pCurve->m_fArrVertValue.GetSize(); iIndex++)
	{
		pCurve->m_fArrVertValue[iIndex] = 2 * fMid - pCurve->m_fArrVertValue[iIndex];
	}

	// need not resort
	Restore();
	Invalidate();
}

// description : calculate vertical range to show, adjust max and min value to avoid drawing
//               points on margin line
// in parameter: fValue -- vertical value
//               bUp    -- TRUE: max value; FALSE: min value
void CCurveCtrl::CalculateVertRange(float fValue, BOOL bMax)
{
	CString str;
	str.Format(_T("%.0f"), fabs(fValue));
	// get integer digital count
	int nEx   = str.GetLength() - 1;
 	int ntemp = (int)pow((float)10, nEx);

	if (bMax && (fValue + ntemp * 0.1) > m_fVertMax)
	{		
		m_fVertMax = fValue + 0.1f * ntemp;
	}
	else if (!bMax && (fValue - ntemp * 0.1) < m_fVertMin)
	{
		m_fVertMin = fValue - 0.1f * ntemp;
	}	
}

void CCurveCtrl::SetHoriLabel(CString& str)
{
	m_strHoriLabel = str;
}

CString CCurveCtrl::GetHoriLabel()
{
	return m_strHoriLabel;
}

void CCurveCtrl::SetVertLabel(CString& str)
{
	m_strVertLabel = str;
}

CString CCurveCtrl::GetVertLabel()
{
	return m_strVertLabel;
}

void CCurveCtrl::ShowCross(BOOL bShow)
{
	m_bShowCross = bShow;
}

BOOL CCurveCtrl::IsShowCross()
{
	return m_bShowCross;
}

// description : set margin, leave a blank in each direction outside of drawing area 
//               to show title, label, and so on
void CCurveCtrl::SetMargin(const CRect& rect)
{
	m_Margin = rect;
}

CRect CCurveCtrl::GetMargin()
{
	return m_Margin;
}

// enable/disable edit by using mouse key operation
void CCurveCtrl::EnableEdit(BOOL bEdit)
{
	if (GetSelectedCount() > 0)
		m_bEdit = bEdit;
	else
		m_bEdit = FALSE;
}

BOOL CCurveCtrl::CanEditCurve()
{
	return m_bEdit;
}

void CCurveCtrl::SetGridLineStyle(int iStyle)
{
	m_iGridLineStyle = iStyle;
}

int	 CCurveCtrl::GetGridLineStyle()
{
	return m_iGridLineStyle;
}

// description : non-edit mode: select/deselect one or more(by Shift or Ctrl key pressed) curves 
//                              by clicking left button of mouse;
//               edit mode: if parameter "point" is near key point of a curve, select key point and ready to move;
//                          if parameter "point" is not near key point but near to line between two key points, add key point.
//                          note that the edited curve must be selected before editing.
//                
void CCurveCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	// disable tooltip	
	if (m_Tooltip.m_hWnd)
		m_Tooltip.Activate(FALSE);	
	
	int iPt = -1;
	CCurve*	 pCurentCur = NULL;
	
	// has any curve near point
    int iIndex = 0;
	for (  iIndex = 0; iIndex < m_ArrCurve.GetSize(); iIndex++)
	{
		if (m_ArrCurve[iIndex]->IsPointNearCurve(point, iPt))
		{
			pCurentCur = m_ArrCurve[iIndex];
			break;
		}
	}

	// calculate value by point in pixel
	float fHori = 0.0f, fVert = 0.0f;
	if (pCurentCur)
	{
		if (iPt > -1)
		{
			fHori = pCurentCur->m_fArrHoriValue[iPt];
			fVert = pCurentCur->m_fArrVertValue[iPt];
		}
		else 
		{
			CalculateValue(point, fHori, fVert);
		}
	}
	
	// non-edit mode
	if (!m_bEdit)
	{			
		// multi-select if Ctrl or Shift key are pressed
		if ((nFlags & MK_CONTROL) == MK_CONTROL || (nFlags & MK_SHIFT) == MK_SHIFT)
		{
			if (pCurentCur)
			{				
				// save previous status(selected or not) 
				BOOL bOldSel = pCurentCur->m_bSelected;
				// change status of the curve (deselected or not)
				pCurentCur->m_bSelected = !pCurentCur->m_bSelected;
				
				// send message to owner
				int nMsg = pCurentCur->m_bSelected ? CVN_CURVE_SELECTED : CVN_CURVE_CANCELSELECT;					
				BOOL bKeepOld = SendMessageToParent(nMsg,
													pCurentCur,
													iIndex,	// curve index in this CCurveCtrl
													fHori,
													fVert					
													);
				// restore previous status if owner does not permit to change 
				if (bKeepOld)
				{
					pCurentCur->m_bSelected = bOldSel;
				}
			}
		}
		else // single-select
		{			
			// deselect all curve
			for (int iQ = 0; iQ < m_ArrCurve.GetSize(); iQ++)
			{
				m_ArrCurve[iQ]->m_bSelected = FALSE;
			}

			if (pCurentCur != NULL)
			{		
				// save previous status(selected or not) 
				BOOL bOldSel = pCurentCur->m_bSelected;
				// change status of the curve (deselected or not)
				pCurentCur->m_bSelected = !pCurentCur->m_bSelected;

				// send message to owner
				int nMsg = pCurentCur->m_bSelected ? CVN_CURVE_SELECTED : CVN_CURVE_CANCELSELECT;					
				BOOL bKeepOld = SendMessageToParent(nMsg,
													pCurentCur,
													iIndex,	// curve index in this CCurveCtrl
													fHori,
													fVert					
													);
				// restore previous status if owner does not permit to change 
				if (bKeepOld)
				{
					pCurentCur->m_bSelected = bOldSel;
				}
			}
			else 			// send message to owner if no curve is selected 
			{
				SendMessageToParent(CVN_CURVE_SELECTNONE,
							        pCurentCur,
									-1, // no curve selected
									fHori,
									fVert
									);
				
				// disable edit mode if no curve selected
				m_bEdit = FALSE;
			}
		}
	}
	else if(pCurentCur && pCurentCur->IsSelected())// under edit mode, the edited curve must be selected already
	{
		// if index of key point of the curve is valid, move point when mouse move
		if (iPt >= 0)
		{
			m_iCurPoint  = iPt;
			m_pCurveEdit = pCurentCur;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			SetCapture();

			// save value before moving
			m_fOldHori = pCurentCur->m_fArrHoriValue[iPt];
			m_fOldVert = pCurentCur->m_fArrVertValue[iPt];
		}
		else // if index invalidate, then add key point to the curve
		{			
			// calculate value by point
			float fHori, fVert;
			CalculateValue(point, fHori, fVert);
			
			// insert values to sorted data arrays, and get the index of the new key point
			int iH = InsertDataToCurve(pCurentCur, fHori, fVert, point);

			// to notify owner that new key point is added 
			int bDontAdd = SendMessageToParent(CVN_MVALUE_ADD, 
											pCurentCur, 
											iH, 
											fHori,
											fVert
											);			
			// whether to add the new point is determined by owner
			if (bDontAdd)
			{
				pCurentCur->m_fArrHoriValue.RemoveAt(iH);
				pCurentCur->m_fArrVertValue.RemoveAt(iH);
				pCurentCur->m_ArrPoint.RemoveAt(iH);
			}
			
			m_iCurPoint  = -1;
			m_pCurveEdit = NULL;
		}
	}
	
	Invalidate();
	
	CWnd::OnLButtonDown(nFlags, point);
}

// description : non-edit mode: refresh to draw cross(if shown) and tooltip
//               edit mode: change cursor and move the selected key point
void CCurveCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{	
	// change point if has any key point selected
	if (m_bEdit && m_pCurveEdit && m_iCurPoint >= 0)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		
		float fHori, fVert;
		if (CalculateValue(point, fHori, fVert))
		{
			// change vertical value only
			m_pCurveEdit->m_ArrPoint[m_iCurPoint].y = point.y;
			m_pCurveEdit->m_fArrVertValue[m_iCurPoint] = fVert;		
		}
	}
	else
	{		
		// if there is any key point nearby
		int iPoint = -1;
		CCurve* pCurentCur = NULL;
		for (int iIndex = 0; iIndex < m_ArrCurve.GetSize(); iIndex++)
		{
			if (m_ArrCurve[iIndex]->IsPointNearCurve(point, iPoint))
			{
				pCurentCur = m_ArrCurve[iIndex];
				break;
			}
		}

		// if parameter point is near one key point of a selected curve
		if (pCurentCur && iPoint >= 0 && pCurentCur->IsSelected())
		{
			// change cursor
			if (m_bEdit)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			
			// use tooltip to show the values of key point nearby
			CString		strTip;
			strTip.Format(_T("%s[%d]: %.1f,%.1f"), pCurentCur->GetCurveName(), iPoint, pCurentCur->m_fArrHoriValue[iPoint], pCurentCur->m_fArrVertValue[iPoint]);
			
			if(m_Tooltip.m_hWnd)
			{
				m_Tooltip.Activate(TRUE);			
				m_Tooltip.UpdateTipText(strTip, this);

				MSG msg;
				msg.hwnd= m_hWnd;
				msg.message= WM_MOUSEMOVE;
				msg.wParam= nFlags;
				msg.lParam= MAKELPARAM(LOWORD(point.x), LOWORD(point.y));
				msg.time= 0;
				msg.pt.x= point.x;
				msg.pt.y= point.y;
				
				m_Tooltip.RelayEvent(&msg);
			}
		}
	}
	
	Invalidate();

	CWnd::OnMouseMove(nFlags, point);
}

// description : handle left mouse button up, notify owner if data changed
void CCurveCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bEdit && m_pCurveEdit && m_iCurPoint >=0)
	{
		BOOL bKeepOld = SendMessageToParent(CVN_MVALUE_CHANG, 
											m_pCurveEdit, 
											m_iCurPoint, 
											m_pCurveEdit->m_fArrHoriValue[m_iCurPoint],
											m_pCurveEdit->m_fArrVertValue[m_iCurPoint]
											);

		if (bKeepOld) // owner does not permit to change data, restore previous values
		{
			m_pCurveEdit->m_fArrHoriValue[m_iCurPoint] = m_fOldHori;
			m_pCurveEdit->m_fArrVertValue[m_iCurPoint] = m_fOldVert;
		}
		else     
		{			
			CalculateVertRange(m_pCurveEdit->m_fArrVertValue[m_iCurPoint], TRUE);
			CalculateVertRange(m_pCurveEdit->m_fArrVertValue[m_iCurPoint], FALSE);
		}

		// cursor style
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		m_pCurveEdit = NULL;
		m_iCurPoint = -1;
		ReleaseCapture();
		Invalidate();
	}	

	CWnd::OnLButtonUp(nFlags, point);
}

// description : delete key point after right mouse button clicked when enable edit
void CCurveCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bEdit)
	{
		// get the nearby curve
		int			iPt = -1;
		CCurve*		pCurentCur = NULL;
        int iIndex = 0;
		for ( iIndex = 0; iIndex < m_ArrCurve.GetSize(); iIndex++)
		{
			if (m_ArrCurve[iIndex]->IsPointNearCurve(point, iPt))
			{
				pCurentCur = m_ArrCurve[iIndex];
				break;
			}
		}

		// if there is one  and be selected and near point
		if (pCurentCur && pCurentCur->IsSelected() && iPt >= 0)
		{
			int bKeep = SendMessageToParent(CVN_MVALUE_DELETE, 
											pCurentCur, 
											iPt, 
											pCurentCur->m_fArrHoriValue[iPt],
											pCurentCur->m_fArrVertValue[iPt]
											);
			// whether delete the point determined by message handle function
			if (!bKeep)
			{
				pCurentCur->m_ArrPoint.RemoveAt(iPt);
				pCurentCur->m_fArrHoriValue.RemoveAt(iPt);
				pCurentCur->m_fArrVertValue.RemoveAt(iPt);
				
				// remove the curve object if there is no point in it
				if (pCurentCur->m_fArrHoriValue.GetSize() == 0)
				{
					ASSERT(m_ArrCurve[iIndex] == pCurentCur);
					delete pCurentCur;
					m_ArrCurve.RemoveAt(iIndex);
				}
			}
			
			Invalidate();
		}
	}	
	
	CWnd::OnRButtonDown(nFlags, point);
}

// description : notify owner that curve mode or data changed
// in parameter: nMessage -- message id
//               pCurve   -- curve object
//               iIndex   -- index of curve or key point
//               fHori    -- horizontal value
//               fVert    -- vertical value
// return      :  0:permit to change;  else: do not change
LRESULT CCurveCtrl::SendMessageToParent(int nMessage, CCurve* pCurve, int iIndex, float fHori, float fVert) const
{
    if (!IsWindow(m_hWnd))
        return 0;

	// struct to save parameter
    NM_CURVECTRL nmcurve;

	nmcurve.pCurve		 = (void*)pCurve;
	nmcurve.iIndex		 = iIndex;
	nmcurve.fHori		 = fHori;
	nmcurve.fVert		 = fVert;	
    nmcurve.hdr.hwndFrom = m_hWnd;
    nmcurve.hdr.idFrom   = GetDlgCtrlID();
    nmcurve.hdr.code     = nMessage;

	// send message to owner, owner is parent or not
    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmcurve.hdr.idFrom, (LPARAM)&nmcurve);
    else
        return 0;
}