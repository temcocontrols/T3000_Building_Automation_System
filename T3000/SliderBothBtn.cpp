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
//原代码执行下面代码
//  	if(bHorz)	//LSC 滑块矩形大小
//  	{
//  		rc.right=10;
//  		rc.bottom=18;
//  	}
//  	else
//  	{
//  		rc.right=18;
//  		rc.bottom=10;
//  	}
//LSC 增加
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
	GetClientRect(&rc); //LSC rc指的是当前滑块的区域大小

	iV=192;
	//CBrush brhBtn(RGB(iV,iV,iV));//LSC 原有代码执行这句  设置滑块的yuanse
	CBrush brhBtn(RGB(102,204,255));

	if(b_Horz) //LSC 横向的滑动控件 绘制外形
	{
//下面这两句是原执行
// 		int iX0=0,iX1=rc.right-1;
// 		int iY0=0,iY1=rc.right-1,iY2=rc.bottom-1;

		int iX0=0,iX1=rc.right-9;
		int iY0=0,iY1=rc.right-9,iY2=rc.bottom-1,iX2 = rc.right;//LSC 增加


//下面这两句是原执行
// 		POINT pt0[]={{iX0,iY2},{iX0,iY1},{iX1,iY0},{iX1,iY2}};
// 		POINT pt1[]={{iX0,iY2},{iX0,iY0},{iX1,iY1},{iX1,iY2}};

		POINT pt2[]= {{iX1,iY0},{iX2,iY2},{iX0,iY2}};//LSC 新增加上去的
		
		CRgn rgn;
//原代码执行下面这三行代码
// 		if(b_Fst)	rgn.CreatePolygonRgn(pt0,4,WINDING);//LSC 左边的滑块控件形状 中间的参数4表示pt0有4个数组
// 		else		rgn.CreatePolygonRgn(pt1,4,WINDING);

		rgn.CreatePolygonRgn(pt2,3,WINDING);//LSC 新增加的

//LSC
// 		参数： lpPoints 指向一个POINT结构数组，或指向一个CPoint对象数组。每一个结构指定多边形的一个顶点的x坐标和y坐标。  
// 			   POINT结构具有下面这样的形式：
// 			typedef struct tagPOINT
// 		{
// 			int x;
// 			int y;
// 		} POINT;
// 
// 		nCount 指定由lpPoints指向的数组中的POINT结构或CPoint对象的数目。  
// 		nMode 指定区域的填充模式。这个参数可以是ALTERNATE或WINDING。  


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
