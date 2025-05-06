// SliderBothBtn.cpp : implementation file
//

#include "stdafx.h"
#include "SliderBothBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SliderBothBtn

SliderBothBtn::SliderBothBtn()
{
	b_BtnDn=0;
}

SliderBothBtn::~SliderBothBtn()
{
}


BEGIN_MESSAGE_MAP(SliderBothBtn,CWnd)
	//{{AFX_MSG_MAP(SliderBothBtn)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SliderBothBtn message handlers

BOOL SliderBothBtn::Create(CWnd*pMum,BOOL bHorz,BOOL bFst)
{
	b_Fst=bFst;
	b_Horz=bHorz;

	CRect rc(0,0,0,0);
//
//  	if(bHorz)	//LSC 
//  	{
//  		rc.right=10;
//  		rc.bottom=18;
//  	}
//  	else
//  	{
//  		rc.right=18;
//  		rc.bottom=10;
//  	}
//LSC 
	rc.right = 18;
	rc.bottom = 18;

	UINT u=WS_CHILD|WS_VISIBLE;
	return CWnd::Create(0,0,u,rc,pMum,IDC_STATIC);
}

void SliderBothBtn::OnPaint() 
{
	CPaintDC dc(this);
#if 1
	int iV=0;
	CPen pen000(PS_SOLID,1,RGB(iV,iV,iV));
	iV=255;
	CPen pen255(PS_SOLID,1,RGB(iV,iV,iV));
	iV=128;
	CPen pen128(PS_SOLID,1,RGB(iV,iV,iV));
	iV=223;
	CPen pen223(PS_SOLID,1,RGB(iV,iV,iV));

	CPen*pPenOld=dc.SelectObject(&pen000);

	//--------------------------------------
	CRect rc;
	GetClientRect(&rc); //LSC rc

	iV=192;
	//CBrush brhBtn(RGB(iV,iV,iV));//LSC   yuanse
	CBrush brhBtn(RGB(102,204,255));

	if(b_Horz) //LSC  
	{
//
// 		int iX0=0,iX1=rc.right-1;
// 		int iY0=0,iY1=rc.right-1,iY2=rc.bottom-1;

		int iX0=0,iX1=rc.right-9;
		int iY0=0,iY1=rc.right-9,iY2=rc.bottom-1,iX2 = rc.right;//LSC 


//
// 		POINT pt0[]={{iX0,iY2},{iX0,iY1},{iX1,iY0},{iX1,iY2}};
// 		POINT pt1[]={{iX0,iY2},{iX0,iY0},{iX1,iY1},{iX1,iY2}};

		POINT pt2[]= {{iX1,iY0},{iX2,iY2},{iX0,iY2}};//LSC 
		
		CRgn rgn;
//
// 		if(b_Fst)	rgn.CreatePolygonRgn(pt0,4,WINDING);//LSC  4pt04
// 		else		rgn.CreatePolygonRgn(pt1,4,WINDING);

		rgn.CreatePolygonRgn(pt2,3,WINDING);//LSC 

//LSC
// 		 lpPoints POINTCPointxy  
// 			   POINT
// 			typedef struct tagPOINT
// 		{
// 			int x;
// 			int y;
// 		} POINT;
// 
// 		nCount lpPointsPOINTCPoint  
// 		nMode ALTERNATEWINDING  


		dc.FillRgn(&rgn,&brhBtn); 
#if 0
		int i;
		for(i=0;i<2;i++)
		{
			dc.MoveTo(iX0,iY2);
			if(b_Fst)
			{
				if(i==0)	dc.SelectObject(&pen255);
				else		dc.SelectObject(&pen223);
				dc.LineTo(iX0,iY1);
				dc.LineTo(iX1,iY0);
			}
			else
			{
				if(i==0)	dc.SelectObject(&pen255);
				else		dc.SelectObject(&pen223);
				dc.LineTo(iX0,iY0);
			}
			
			if(i==0)	dc.SelectObject(&pen000);
			else		dc.SelectObject(&pen128);

			if(b_Fst==0)	dc.LineTo(iX1,iY1);
			dc.LineTo(iX1,iY2);
			dc.LineTo(iX0,iY2);

			iX0++;
			iY0+=2;
			iX1--;
			iY2--;

//			iY1++;
		}

		//width=10
		iX0=2;
		for(i=0;i<3;i++)
		{
			dc.MoveTo(iX0,iY1);
			dc.LineTo(iX0,rc.bottom-3);
			iX0+=2;
		}
#endif
		rgn.DeleteObject();
	}
	else
	{

		int iX0=0,iX1=rc.bottom-1,iX2=rc.right-1;
		int iY0=0,iY1=rc.bottom-1;

		POINT pt0[]={{iX0,iY0},{iX2,iY0},{iX2,iY1},{iX1,iY1}};
		POINT pt1[]={{iX0,iY1},{iX1,iY0},{iX2,iY0},{iX2,iY1}}; 
		
		CRgn rgn;
		if(b_Fst)	rgn.CreatePolygonRgn(pt0,4,WINDING); 
		else		rgn.CreatePolygonRgn(pt1,4,WINDING);

		dc.FillRgn(&rgn,&brhBtn);

		int i;
		for(i=0;i<2;i++)
		{
			dc.MoveTo(iX2,iY0);
			if(i==0)	dc.SelectObject(&pen000);
			else		dc.SelectObject(&pen128);
			
			dc.LineTo(iX2,iY1);

			if(b_Fst)
			{
				dc.LineTo(iX1,iY1);
				dc.LineTo(iX0,iY0);
				if(i==0)	dc.SelectObject(&pen255);
				else		dc.SelectObject(&pen223);
			}
			else
			{
				dc.LineTo(iX0,iY1);
				if(i==0)	dc.SelectObject(&pen255);
				else		dc.SelectObject(&pen223);
				dc.LineTo(iX1,iY0);
			}
			dc.LineTo(iX2,iY0);

			iX0+=2;
			iY0++;
			iX2--;
			iY1--;

//			iX1++;
		}

		dc.SelectObject(&pen128);  


		//width=10
		iY0=2;
		for(i=0;i<3;i++)
		{
			dc.MoveTo(iX1,			iY0);
			dc.LineTo(rc.right-3,	iY0);
			iY0+=2;
		}
	}

	//--------------------------------------
	dc.SelectObject(pPenOld);
	
	
#endif
}

void SliderBothBtn::OnLButtonDown(UINT nFlags,CPoint pt) 
{
	int iPos;
	if(b_Horz)	iPos=pt.x;
	else		iPos=pt.y;

	i_BtnDnPos=iPos; 
	b_BtnDn=1;
	SetCapture();
	CWnd::OnLButtonDown(nFlags,pt);
}

void SliderBothBtn::OnMouseMove(UINT nFlags,CPoint pt) 
{
	if(b_BtnDn)
	{
		int iPos;
		if(b_Horz)	iPos=pt.x-i_BtnDnPos;
		else		iPos=pt.y-i_BtnDnPos;
		GetParent()->PostMessage(WM_USER,iPos,b_Fst);
	}
	CWnd::OnMouseMove(nFlags,pt);
}

void SliderBothBtn::OnLButtonUp(UINT nFlags,CPoint pt) 
{
	GetParent()->PostMessage(WM_USER+1,b_Fst);
	b_BtnDn=0;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags,pt);
	RedrawWindow();
}
