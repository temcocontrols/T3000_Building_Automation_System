// SliderBoth.cpp : implementation file
//

#include "stdafx.h"
#include "SliderBoth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define MARGIN_ME	14
#define MARGIN_SLIDE 5

SliderBoth::SliderBoth()
{
	b_Initialized=0;

//	pos.Init(0,200,10); //原代码执行此句
//	pos.Init(0,24,1);
}

SliderBoth::~SliderBoth()
{
}


BEGIN_MESSAGE_MAP(SliderBoth,CWnd)
	//{{AFX_MSG_MAP(SliderBoth)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SliderBoth message handlers

BOOL SliderBoth::Create(CWnd*pMum,CRect rcBd,BOOL bHorz,int iID)
{
	//height is 27 pixels if no text
	i_ID	=iID;
	b_Horz	=bHorz;
	
//	i_SliderY=5+6+8;	//top+tic+gap;//原代码执行此句

	i_SliderY = 5+6+8;
	int iH=i_SliderY+3+5;	//sliderH+bottom; 




	CRect rc=rcBd;
	if(bHorz)	rc.bottom=rc.top+iH;
	else		rc.right=rc.left+iH;

	UINT u=WS_CHILD|WS_VISIBLE;
	BOOL b=CWnd::Create(0,0,u,rc,pMum,iID);
	if(b==0)	return 0;

	tic0.Create(this,bHorz,1);//第三个参数1，是BOOL类型以区别左右两边滑块形状
	tic1.Create(this,bHorz,0);
// 	tic2.Create(this,bHorz,TRUE);//LSC
// 	tic3.Create(this,bHorz,TRUE);//LSC
	
	return b;
}

void SliderBoth::MoveWindow(CRect*pRect,BOOL bRepaint)
{
	CRect rc,rc0=*pRect;
	GetWindowRect(&rc);

	if(b_Horz)	rc0.bottom=rc0.top+rc.Height();
	else		rc0.right=rc0.left+rc.Width();

	int iL,iR;
	if(b_Initialized)	GetPosition(iL,iR);
	CWnd::MoveWindow(&rc0,bRepaint);
	if(b_Initialized)	P_SetPosition(iL,iR,1,1);
}

void SliderBoth::MoveWindow(int iX,int iY,int iW,int iH,BOOL bRepaint)
{
	CRect rc(iX,iY,iX+iW,iY+iH);
	MoveWindow(&rc,bRepaint);
}


void SliderBoth::OnSize(UINT nType,int cx,int cy) 
{
	CWnd::OnSize(nType,cx,cy);
	
	if(::IsWindow(m_hWnd)==0)	return;
	if(::IsWindow(tic0.m_hWnd)==0)	return;
	if(::IsWindow(tic1.m_hWnd)==0)	return;

	if(b_Initialized)
	{
		P_SetPosition(pos.iL,pos.iR,1,1);
	}
}

void SliderBoth::P_SetPosition(int iL,int iR,BOOL bFst,BOOL bSnd)
{
	if(b_Initialized==0)	return;
	if(b_Horz)	P_SetPosition_Horz(iL,iR,bFst,bSnd);
	else		P_SetPosition_Vert(iL,iR,bFst,bSnd);
}

void SliderBoth::P_SetPosition_Horz(int iL,int iR,BOOL bFst,BOOL bSnd)
{
	CRect rcMe;
	GetClientRect(&rcMe);

	const int iXL=MARGIN_ME;
	const int iXR=rcMe.right-MARGIN_ME;

	const double dbOne=(iXR-iXL)*1.0/(pos.iMax-pos.iMin);

	CRect rc;
	tic0.GetWindowRect(&rc);
	const int iW=rc.Width();
	const int iH=rc.Height();

	rc.top=i_SliderY-iW-1;
	rc.bottom=rc.top+iH;

	if(iL<pos.iMin)	iL=pos.iMin;
	if(iR>pos.iMax)	iR=pos.iMax;

	if(bFst)	
	{
		if(iL>iR)	iL=iR;
	}

	if(bSnd)
	{
		if(iR<iL)	iR=iL;
	}

	const int iPixL=iXL+(int)((iL-pos.iMin)*dbOne);
	const int iPixR=iXL+(int)((iR-pos.iMin)*dbOne);

	if(bFst)
	{
		pos.iL=iL;
		rc.right=iPixL+1;
		rc.left=rc.right-iW;
		tic0.MoveWindow(&rc);
	}

	if(bSnd)
	{
		pos.iR=iR;
		rc.left=iPixR;
		rc.right=rc.left+iW;
		tic1.MoveWindow(&rc);
	}
	//LSC 以下新增加的

// 	if(bFst)
// 	{
// 		pos.iL=iL;
// 		rc.right=iPixL+1;
// 		rc.left=rc.right-iW;
// 		tic1.MoveWindow(&rc);
// 	}

}

void SliderBoth::P_SetPosition_Vert(int iL,int iR,BOOL bFst,BOOL bSnd)
{
	CRect rcMe;
	GetClientRect(&rcMe);

	const int iXL=rcMe.bottom-MARGIN_ME;
	const int iXR=rcMe.top+MARGIN_ME;

	const double dbOne=abs(iXL-iXR)*1.0/(pos.iMax-pos.iMin);

	CRect rc;
	tic0.GetWindowRect(&rc);
	const int iW=rc.Width();
	const int iH=rc.Height();

	rc.left=i_SliderY-iH-1;
	rc.right=rc.left+iW;

	if(iL<pos.iMin)	iL=pos.iMin;
	if(iR>pos.iMax)	iR=pos.iMax;

	if(bFst)	
	{
		if(iL>iR)	iL=iR;
	}

	if(bSnd)
	{
		if(iR<iL)	iR=iL;
	}

	const int iPixL=iXL-(int)((iL-pos.iMin)*dbOne);
	const int iPixR=iXL-(int)((iR-pos.iMin)*dbOne);
	if(bFst)
	{
		pos.iL=iL;
		rc.top=iPixL;
		rc.bottom=rc.top+iH;
		tic0.MoveWindow(&rc);
	}

	if(bSnd)
	{
		pos.iR=iR;
		rc.bottom=iPixR;
		rc.top=rc.bottom-iH;
		tic1.MoveWindow(&rc);
	}
}

void SliderBoth::GetPosition(int &iL,int &iR)
{
	iL=pos.iL;
	iR=pos.iR;
}

void SliderBoth::SetPosition(int iL,int iR)
{
	P_SetPosition(iL,iR,1,1);
}

void SliderBoth::OnPaint() 
{
	if(b_Initialized==0)
	{
		PostMessage(WM_USER+100); //确定滑块初始位置状态
	}

	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	rc.bottom = rc.bottom-5;//LSC 增加 Slider控件底部减小
//	COLORREF clrBk=::GetSysColor(COLOR_BTNFACE);//LSC 原执行此句
	int iV=209;
//	COLORREF clrBk=RGB(iV,iV,iV);//LSC 原屏蔽此句
	COLORREF clrBk=RGB(17,74,126); //LSC  修改整个slider控件的背景色
	dc.FillRect(&rc,&CBrush(clrBk));//LSC原代码执行此句

	P_PaintHorz(&dc,rc); //LSC 绘制刻度线
	SetBlockColor();
}

void SliderBoth::P_PaintHorz(CDC*pDC,const CRect &rcMe)
{
	COLORREF clr000=RGB(0,0,0);
	COLORREF clr255=RGB(255,255,255);

	const int iTicT=5;
	const int iTicB=iTicT+3;

	int iNumTic=(pos.iMax-pos.iMin)/pos.iTick;
	int i,iM=iNumTic/2;

	if(b_Horz)
	{
// 		const int iL=MARGIN_ME;//LSC 原代码执行此句 画刻度的
// 		const int iR=rcMe.right-MARGIN_ME;
// 		const double dbStep=(iR-iL)*1.0/iNumTic;


		const int iL=1;//LSC 修改 原代码执行此句 画刻度的 表示刻度线在左边向右移动多少，即起止点离滑块左边多远。
		const int iR=rcMe.right; //LSC 修改 表示刻度在滑块的右边向左移动多少。
		const double dbStep=(iR-iL)*1.0/iNumTic;
 		double db=iL;
		int iX;

		for(i=0;i<=iNumTic;i++)
		{
			iX=(int)db;
			pDC->MoveTo(iX,iTicT);
			if(i%iM==0)	pDC->LineTo(iX,iTicB+3);
			else		pDC->LineTo(iX,iTicB);
			db+=dbStep;
		}

		int iLineT=iTicB+8;
//		CRect rc(iL-5,iLineT,iR+5,iLineT+3);//LSC 原句执行此句 画滑块底下的白色线条
		CRect rc(iL-15,iLineT,iR+15,iLineT+3);//LSC 增加

		pDC->Draw3dRect(&rc,clr000,clr255);//LSC 屏蔽，原代码执行此句
	}
	else
	{
		const int iL=rcMe.bottom-MARGIN_ME;
		const int iR=rcMe.top+MARGIN_ME;
		const double dbStep=(iL-iR)*1.0/iNumTic;
		double db=iL;

		int iY;
		for(i=0;i<=iNumTic;i++)
		{
			iY=(int)db;
			pDC->MoveTo(iTicT,iY);
			if(i%iM==0)	pDC->LineTo(iTicB+3,iY);
			else		pDC->LineTo(iTicB,iY);
			db-=dbStep;
		}

		int iLineT=iTicB+8;
		CRect rc(iLineT,iR-5,iLineT+3,iL+5);
//		pDC->Draw3dRect(&rc,clr000,clr255);//LSC 屏蔽，原代码执行此句
	}

}

LRESULT SliderBoth::WindowProc(UINT m,WPARAM w,LPARAM l) 
{
	if((m==WM_USER)||(m==WM_USER+1)||(m==WM_USER+100))
	{
		CRect rcMe;
		GetClientRect(&rcMe);

		if(b_Horz)
		{
			const int iXL=MARGIN_ME;
			const int iXR=rcMe.right-MARGIN_ME;
			double dbOne=(iXR-iXL)*1.0/(pos.iMax-pos.iMin);

			if(m==WM_USER)
			{
				int iMove=(int)(((int)w)/dbOne);

				int iL,iR;
				GetPosition(iL,iR);
				if(l)	iL+=iMove;
				else	iR+=iMove;
				P_SetPosition(iL,iR,l,!l);
// 				CClientDC dc(this);
// 				
// 				int iV=209;
// 				//	COLORREF clrBk=RGB(iV,iV,iV);//LSC 原屏蔽此句
// 				COLORREF clrBk=RGB(0,0,255); //LSC  修改整个slider控件的背景色
// 				dc.FillRect(&rcMe,&CBrush(clrBk));//LSC原代码执行此句
			}
			else
			if(m==WM_USER+1)
			{
				int iL,iR;
				GetPosition(iL,iR);
				if((pos.iOldL!=iL)||(pos.iOldR!=iR))
				{
					pos.iOldL=iL;
					pos.iOldR=iR;
					//post event
					GetParent()->PostMessage(WM_COMMAND,i_ID);
					SetBlockColor();
#if 0
					CClientDC dc(this);
					CRect rc;
					GetClientRect(&rc);


					int iNumTic=(pos.iMax-pos.iMin)/pos.iTick;
					const int iL=MARGIN_SLIDE;
					const int iR=rc.right-MARGIN_SLIDE;
					const double dbStep=(iR-iL)*1.0/iNumTic;
					double db=iL;



					CBrush brush;             
					//brush.CreateSolidBrush(RGB(200,100,255));  
					brush.CreateSolidBrush(RGB(165,196,211));   
					CBrush * pOldBrush= dc.SelectObject (&brush);


					double lval = pos.iL*dbStep;
					double rval = pos.iR*dbStep;

					dc.Rectangle((int)lval+MARGIN_SLIDE*2,rc.top+1,(int)rval,rc.bottom-MARGIN_SLIDE*2); 
					dc.SelectObject (pOldBrush);
					brush.DeleteObject();
#endif


				}
			}
			if(m==WM_USER+100)
			{
				b_Initialized=1;
				//P_SetPosition(pos.iMin,pos.iMax,1,1);
				P_SetPosition(m_min,m_max,1,1);//注意，后两个参数是1,2,3...的1,不是l,j,k的l
				RedrawWindow();
			}
		}
		else
		{
			const int iXL=rcMe.bottom-MARGIN_ME;
			const int iXR=rcMe.top+MARGIN_ME;
			double dbOne=(iXL-iXR)*1.0/(pos.iMax-pos.iMin);

			if(m==WM_USER)
			{
				int iMove=(int)(((int)w)/dbOne);

				int iL,iR;
				GetPosition(iL,iR);
				if(l)	iL-=iMove;
				else	iR-=iMove;
				P_SetPosition(iL,iR,l,!l);
			}
			else
			if(m==WM_USER+1)
			{
				int iL,iR;
				GetPosition(iL,iR);
				if((pos.iOldL!=iL)||(pos.iOldR!=iR))
				{
					pos.iOldL=iL;
					pos.iOldR=iR;
					//post event;
					GetParent()->PostMessage(WM_COMMAND,i_ID);
//wm.DT(200,i_ID);
				}
			}
			else
			if(m==WM_USER+100)
			{
				b_Initialized=1;
				P_SetPosition(pos.iMin,pos.iMax,1,1);
				RedrawWindow();
			}
		}
	}

	return CWnd::WindowProc(m,w,l);
}

BOOL SliderBoth::SetRange(int iMin,int iMax,int iTick)
{
	int iDis=iMax-iMin; //lsc
	if(iDis<2)		return 0;
	if(iDis%2)		return 0;
	if(iTick<1)		return 0;
	if(iDis%iTick)	return 0;

	pos.Init(iMin,iMax,iTick);
	BOOL ret = RedrawWindow();
	return TRUE;
}
void SliderBoth::GetRange(int&iMin,int&iMax,int&iTick)
{
	iMin=pos.iMin;
	iMax=pos.iMax;
	iTick=pos.iTick;
}

void SliderBoth::SetInstane( int minval,int maxval )
{
	m_min = minval;
	m_max = maxval;
	

}

void SliderBoth::SetBlockColor()
{
#if 1
	CClientDC dc(this);
	//CDC* pdc = this->GetDC();
	//CDC*dc = this->GetDC();
	CRect rc;
	GetClientRect(&rc);
	
	CreateCompatibleDC((HDC)&dc);


	int iNumTic=(pos.iMax-pos.iMin)/pos.iTick;
	const int iL=MARGIN_SLIDE;
	const int iR=rc.right-MARGIN_SLIDE;
	const double dbStep=(iR-iL)*1.0/iNumTic;
	double db=iL;



	CBrush brush;             
	//brush.CreateSolidBrush(RGB(200,100,255));  
	brush.CreateSolidBrush(RGB(165,196,211));   
	//CBrush * pOldBrush= dc.SelectObject (&brush);
	CBrush * pOldBrush= dc.SelectObject (&brush);//LSC


	double lval = pos.iL*dbStep;//左边滑块
	double rval = pos.iR*dbStep;//右边滑块

	int iM=iNumTic/2;
	int RectaParam1,RectaParam2 = rc.top+1,RectaParam3,RectaParam4 = rc.bottom-MARGIN_SLIDE*2;

	if (pos.iL>=iM)
	{
		RectaParam1 = (int)lval;//+MARGIN_SLIDE*2;
	}else 
	{
		RectaParam1 = (int)lval+MARGIN_SLIDE*2;
	}

	if (pos.iR <= iM)
	{
		RectaParam3 = (int)rval+12;

	}else
	{
		RectaParam3 = (int)rval+3;
	}

//	dc.Rectangle((int)lval+MARGIN_SLIDE*2,rc.top+1,(int)rval,rc.bottom-MARGIN_SLIDE*2); 
	dc.Rectangle(RectaParam1,RectaParam2,RectaParam3,RectaParam4);
	dc.SelectObject (pOldBrush);
	brush.DeleteObject();
#endif
}