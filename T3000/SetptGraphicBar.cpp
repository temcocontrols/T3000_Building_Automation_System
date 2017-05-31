#include "stdAfx.h"
#include "SetptGraphicBar.h"
#include "global_function.h"
#include "global_variable_extern.h"

int xBase=123;
int yBase=490;
int nThumLeftBase=46;
int RulerBeginPix=500;
CSetptGraphicBar::CSetptGraphicBar(void)
{
	//m_rcClient.CopyRect(CRect(0,0,0,0));
	m_rcClient.CopyRect(CRect(0,0,24,120));
	m_nType=0;
	m_pBkImg=NULL;
	m_nUnit=8;
	m_nbkImgWidth=0;
	m_nbkImgHeight=0;
	m_nThumbWidth=0;
	m_nThumbHeight=0;
	m_rcTank.SetRectEmpty();
	m_nRulerHeight=464;
	m_fFirmwareVersion=0.0;

	m_hParentWnd=NULL;
	m_strSrtPt=_T("Setpt");
	m_bSetPtSelected=FALSE;
	m_bMaxSelected=FALSE;
	m_bMinSelected=FALSE;
	m_strMax=_T("Max");
	m_strMin=_T("Min");
	m_strHeat=_T("Heat");
	m_strCool=_T("Cool");

	m_bHeatSelected=FALSE;
	m_bCoolSelected=FALSE;
}

CSetptGraphicBar::~CSetptGraphicBar(void)
{
	delete this;
}
void CSetptGraphicBar::CreatSliderBar(CRect rcSliderBar,int nStyle,HWND hParent)
{
	InitialBar();
	m_rcClient.CopyRect(rcSliderBar);
	m_nType=nStyle;

	m_rcTank.CopyRect(CRect(60,26,86,511));
	m_rcTank.OffsetRect(m_rcClient.left,m_rcClient.top);
	m_hParentWnd=hParent;

}
void CSetptGraphicBar::DrawGraphicBar(CDC* pDC)
{	
	m_fFirmwareVersion =get_curtstat_version();
	if(m_pBkImg!=NULL)
	{
		Graphics graphics(pDC->m_hDC);
		graphics.DrawImage(m_pBkImg,m_rcClient.left,m_rcClient.top,m_pBkImg->GetWidth(),m_pBkImg->GetHeight());
		DrawRuler(pDC);
	
		
		if(multi_register_value[7]<=0)
			return;
		//draw thumb:
		if(m_nType==0)
		{
			CString strTect;
			CRect rc;

			//Dragging
			if(m_bSetPtSelected)
			{
					graphics.DrawImage(m_SetPtImg.pImg,m_SetPtImg.rcImg.left,m_SetPtImg.rcImg.top,m_SetPtImg.rcImg.Width(),m_SetPtImg.rcImg.Height());
					
					CString strText;
					strText.Format(_T("%d"),m_nDragTempvalue);
					pDC->DrawText(strText,&m_SetPtImg.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			
					CRect rcCaption(m_SetPtImg.rcImg.left-44,m_SetPtImg.rcImg.top,m_SetPtImg.rcImg.right,m_SetPtImg.rcImg.bottom);
					pDC->DrawText(m_strSrtPt,&rcCaption,DT_LEFT|DT_SINGLELINE);
			
			}
			else
			{
				if(m_fFirmwareVersion>0)
				{
					int nValue;
					if(m_fFirmwareVersion<34.9)
					{
						nValue=multi_register_value[135];
					}
					else
					{
						nValue=(int)(multi_register_value[374]/10.0);
					}
					float bYpos=m_rcClient.top+RulerBeginPix-(float)nValue/(m_nRulerEnd-m_nRulerBegin)*m_nRulerHeight;
					m_SetPtImg.rcImg=CRect(nThumLeftBase+m_rcClient.left,(int)(bYpos-m_nThumbHeight/2),nThumLeftBase+m_rcClient.left+m_nThumbWidth,(int)(bYpos+m_nThumbHeight/2));
					m_SetPtImg.pImg=m_pThumbImg;
					graphics.DrawImage(m_SetPtImg.pImg,m_SetPtImg.rcImg.left,m_SetPtImg.rcImg.top,m_SetPtImg.rcImg.Width(),m_SetPtImg.rcImg.Height());
					
					CString strText;
					strText.Format(_T("%d"),nValue);
					pDC->DrawText(strText,&m_SetPtImg.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
					CRect rcCaption(m_SetPtImg.rcImg.left-44,m_SetPtImg.rcImg.top,m_SetPtImg.rcImg.right,m_SetPtImg.rcImg.bottom);
					pDC->DrawText(m_strSrtPt,&rcCaption,DT_LEFT|DT_SINGLELINE);
				}

			}

			if(m_bMaxSelected)
			{
			
					graphics.DrawImage(m_MaxImg.pImg,m_MaxImg.rcImg.left,m_MaxImg.rcImg.top,m_MaxImg.rcImg.Width(),m_MaxImg.rcImg.Height());
					
					CString strText;
					strText.Format(_T("%d"),m_nDragTempvalue);
					pDC->DrawText(strText,&m_MaxImg.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			
					CRect rcCaption(m_MaxImg.rcImg.left-44,m_MaxImg.rcImg.top,m_MaxImg.rcImg.right,m_MaxImg.rcImg.bottom);
					pDC->DrawText(m_strMax,&rcCaption,DT_LEFT|DT_SINGLELINE);
		

			}
			else
			{
				int nValue;
				nValue=multi_register_value[131];
				float bYpos=m_rcClient.top+RulerBeginPix-(float)nValue/(m_nRulerEnd-m_nRulerBegin)*m_nRulerHeight;
				m_MaxImg.rcImg=CRect(nThumLeftBase+m_rcClient.left,(int)(bYpos-m_nThumbHeight/2),nThumLeftBase+m_rcClient.left+m_nThumbWidth,(int)(bYpos+m_nThumbHeight/2));
				m_MaxImg.pImg=m_pThumbImg;
				graphics.DrawImage(m_MaxImg.pImg,m_MaxImg.rcImg.left,m_MaxImg.rcImg.top,m_MaxImg.rcImg.Width(),m_MaxImg.rcImg.Height());
			
				CString strText;
				strText.Format(_T("%d"),nValue);

				pDC->DrawText(strText,&m_MaxImg.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				CRect rcCaption(m_MaxImg.rcImg.left-44,m_MaxImg.rcImg.top,m_MaxImg.rcImg.right,m_MaxImg.rcImg.bottom);
				pDC->DrawText(m_strMax,&rcCaption,DT_LEFT|DT_SINGLELINE);
			
			}


			if(m_bMinSelected)
			{
				
					graphics.DrawImage(m_MinImg.pImg,m_MinImg.rcImg.left,m_MinImg.rcImg.top,m_MinImg.rcImg.Width(),m_MinImg.rcImg.Height());
					
 					CString strText;
					strText.Format(_T("%d"),m_nDragTempvalue);
					pDC->DrawText(strText,&m_MinImg.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			
					CRect rcCaption(m_MinImg.rcImg.left-44,m_MinImg.rcImg.top,m_MinImg.rcImg.right,m_MinImg.rcImg.bottom);
					pDC->DrawText(m_strMin,&rcCaption,DT_LEFT|DT_SINGLELINE);

			}
			else
			{
				int nValue;
				nValue=multi_register_value[132];
				float bYpos=m_rcClient.top+RulerBeginPix-(float)nValue/(m_nRulerEnd-m_nRulerBegin)*m_nRulerHeight;
				m_MinImg.rcImg=CRect(nThumLeftBase+m_rcClient.left,(int)(bYpos-m_nThumbHeight/2),nThumLeftBase+m_rcClient.left+m_nThumbWidth,(int)(bYpos+m_nThumbHeight/2));
				m_MinImg.pImg=m_pThumbImg;
				graphics.DrawImage(m_MinImg.pImg,m_MaxImg.rcImg.left,m_MinImg.rcImg.top,m_MinImg.rcImg.Width(),m_MinImg.rcImg.Height());
			
				CString strText;
				strText.Format(_T("%d"),nValue);
				pDC->DrawText(strText,&m_MinImg.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				CRect rcCaption(m_MinImg.rcImg.left-44,m_MinImg.rcImg.top,m_MinImg.rcImg.right,m_MinImg.rcImg.bottom);
				pDC->DrawText(m_strMin,&rcCaption,DT_LEFT|DT_SINGLELINE);

			}

			CRect rcUnderMin(m_rcTank.left,m_MinImg.rcImg.bottom,m_rcTank.right,m_rcTank.bottom);
			pDC->FillSolidRect(&rcUnderMin,RGB(238,66,91));
			CRect rcSetpt1(m_rcTank.left,m_SetPtImg.rcImg.bottom,m_rcTank.right,m_MinImg.rcImg.top);
			pDC->FillSolidRect(&rcSetpt1,RGB(244,171,71));
			CRect rcSetpt2(m_rcTank.left,m_MaxImg.rcImg.bottom,m_rcTank.right,m_SetPtImg.rcImg.top);
			pDC->FillSolidRect(&rcSetpt2,RGB(244,171,71));

			CRect rcMax(m_rcTank.left,m_rcTank.top,m_rcTank.right,m_MaxImg.rcImg.top);
			pDC->FillSolidRect(&rcMax,RGB(34,34,255));
			//	graphics.DrawImage(m_pThumbImg,76,80,m_pThumbImg->GetWidth(),m_pThumbImg->GetHeight());
			
		}




		else
		{
			int nMode;
			int nValueHeat=-1;
			int nValueCool=-1;

			nMode=multi_register_value[125];
			if(nMode>=0)
			{
				if(nMode==0)
				{		
					nValueHeat=multi_register_value[182];
					nValueCool=multi_register_value[183];
				}
				else
				{
					nValueHeat=multi_register_value[123];
					nValueCool=multi_register_value[124];
				}

			}
			if(nValueHeat!=-1&&nValueHeat!=-1)
			{
				
					if(m_bHeatSelected)
					{
						
						graphics.DrawImage(m_HeatSetPt.pImg,m_HeatSetPt.rcImg.left,m_HeatSetPt.rcImg.top,m_HeatSetPt.rcImg.Width(),m_HeatSetPt.rcImg.Height());
						
						CString strText;
						strText.Format(_T("%d"),m_nDragTempvalue);
						pDC->DrawText(strText,&m_HeatSetPt.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				
						CRect rcCaption(m_HeatSetPt.rcImg.left-44,m_HeatSetPt.rcImg.top,m_HeatSetPt.rcImg.right,m_HeatSetPt.rcImg.bottom);
						pDC->DrawText(m_strHeat,&rcCaption,DT_LEFT|DT_SINGLELINE);

					}
					else
					{
					
						float bYpos=m_rcClient.top+RulerBeginPix-(float)nValueHeat/(m_nRulerEnd-m_nRulerBegin)*m_nRulerHeight;
						m_HeatSetPt.rcImg=CRect(nThumLeftBase+m_rcClient.left,(int)(bYpos-m_nThumbHeight/2),nThumLeftBase+m_rcClient.left+m_nThumbWidth,(int)(bYpos+m_nThumbHeight/2));
						m_HeatSetPt.pImg=m_pThumbImg;
						graphics.DrawImage(m_HeatSetPt.pImg,m_HeatSetPt.rcImg.left,m_HeatSetPt.rcImg.top,m_HeatSetPt.rcImg.Width(),m_HeatSetPt.rcImg.Height());
						CString strText;
						strText.Format(_T("%d"),nValueHeat);
						pDC->DrawText(strText,&m_HeatSetPt.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						CRect rcCaption(m_HeatSetPt.rcImg.left-44,m_HeatSetPt.rcImg.top,m_HeatSetPt.rcImg.right,m_HeatSetPt.rcImg.bottom);
						pDC->DrawText(m_strHeat,&rcCaption,DT_LEFT|DT_SINGLELINE);

					}
				
					if(m_bCoolSelected)
					{
						graphics.DrawImage(m_CoolSetPt.pImg,m_CoolSetPt.rcImg.left,m_CoolSetPt.rcImg.top,m_CoolSetPt.rcImg.Width(),m_CoolSetPt.rcImg.Height());
						
						CString strText;
						strText.Format(_T("%d"),m_nDragTempvalue);
						pDC->DrawText(strText,&m_CoolSetPt.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				
						CRect rcCaption(m_CoolSetPt.rcImg.left-44,m_CoolSetPt.rcImg.top,m_CoolSetPt.rcImg.right,m_CoolSetPt.rcImg.bottom);
						pDC->DrawText(m_strCool,&rcCaption,DT_LEFT|DT_SINGLELINE);
					}
					else
					{
						
						float bYpos=m_rcClient.top+RulerBeginPix-(float)nValueCool/(m_nRulerEnd-m_nRulerBegin)*m_nRulerHeight;
						m_CoolSetPt.rcImg=CRect(nThumLeftBase+m_rcClient.left,(int)(bYpos-m_nThumbHeight/2),nThumLeftBase+m_rcClient.left+m_nThumbWidth,(int)(bYpos+m_nThumbHeight/2));
						m_CoolSetPt.pImg=m_pThumbImg;
						graphics.DrawImage(m_CoolSetPt.pImg,m_CoolSetPt.rcImg.left,m_CoolSetPt.rcImg.top,m_CoolSetPt.rcImg.Width(),m_CoolSetPt.rcImg.Height());
			
						CString strText;
						strText.Format(_T("%d"),nValueCool);
						pDC->DrawText(strText,&m_CoolSetPt.rcImg,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						CRect rcCaptionCool(m_CoolSetPt.rcImg.left-44,m_CoolSetPt.rcImg.top,m_CoolSetPt.rcImg.right,m_CoolSetPt.rcImg.bottom);
						pDC->DrawText(m_strCool,&rcCaptionCool,DT_LEFT|DT_SINGLELINE);


					}

								
					CRect rcBottom(m_rcTank.left,m_HeatSetPt.rcImg.bottom,m_rcTank.right,m_rcTank.bottom);
					pDC->FillSolidRect(&rcBottom,RGB(238,66,91));

					CRect rcSetpt1(m_rcTank.left,m_CoolSetPt.rcImg.bottom,m_rcTank.right,m_HeatSetPt.rcImg.top);
					pDC->FillSolidRect(&rcSetpt1,RGB(244,171,71));

					CRect rcSetpt2(m_rcTank.left,m_rcTank.top,m_rcTank.right,m_CoolSetPt.rcImg.top);
					pDC->FillSolidRect(&rcSetpt2,RGB(34,34,255));

				}

			
		}
		
	}

}
void CSetptGraphicBar::InitialBar()
{
}
void CSetptGraphicBar::SetImg(Bitmap* pBkImg,Bitmap* pThumb)
{
	m_pBkImg=pBkImg;
	m_nbkImgWidth=m_pBkImg->GetWidth();
	m_nbkImgHeight=m_pBkImg->GetHeight();
	m_pThumbImg=pThumb;
	m_nThumbWidth=m_pThumbImg->GetWidth();
	m_nThumbHeight=m_pThumbImg->GetHeight();
}
void CSetptGraphicBar:: DrawRuler(CDC* pDC)
{
	int nX=m_rcClient.left+123;
	int nY=m_rcClient.top+490;
	CString strTemp;
	for(int i=0;i<=10;i++)
	{
		strTemp.Format(_T("%d"),i*m_nUnit);
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(nX,nY-46*i,strTemp,strTemp.GetLength());
	}
	m_nRulerBegin=0;
	m_nRulerEnd=m_nUnit*10;
}


void CSetptGraphicBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nType==0)
	{
	
		if(m_SetPtImg.rcImg.PtInRect(point)&&!m_bMaxSelected&&!m_bMinSelected)
		{
			m_bSetPtSelected=TRUE;
			::SetCapture(m_hParentWnd);
		}
		if(m_MaxImg.rcImg.PtInRect(point)&&!m_bMinSelected&&!m_bSetPtSelected)
		{
			m_bMaxSelected=TRUE;
			::SetCapture(m_hParentWnd);
		}
		if(m_MinImg.rcImg.PtInRect(point)&&!m_bMaxSelected&&!m_bSetPtSelected)
		{
			m_bMinSelected=TRUE;
			::SetCapture(m_hParentWnd);
		}
	}
	else
	{
		if(m_HeatSetPt.rcImg.PtInRect(point)&&!m_bCoolSelected)
		{
			m_bHeatSelected=TRUE;
			::SetCapture(m_hParentWnd);
		}
		if(m_CoolSetPt.rcImg.PtInRect(point)&&!m_bHeatSelected)
		{
			m_bCoolSelected=TRUE;
			::SetCapture(m_hParentWnd);
		}

	}
	 
}

void CSetptGraphicBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if(point.y<m_rcTank.top+8)
		return;
	if(point.y>m_rcTank.bottom-8)
		return;

	CWnd *pWnd=CWnd::FromHandle(m_hParentWnd);
	if(m_nType==0)
	{
		if(m_bSetPtSelected)
		{
			int nValue=PosTOValue(point.y);

			if(m_fFirmwareVersion>0)
			{
				int nreg;
				if(m_fFirmwareVersion<34.9)
				{
					nreg=135;//multi_register_value[135];
					
				}
				else
				{
					nreg=374;//multi_register_value[374]/10.0;
					nValue=nValue*10;
				}
					write_one(g_tstat_id,nreg,nValue);
			}
		

			m_bSetPtSelected=FALSE;
			::ReleaseCapture();
			pWnd->Invalidate();
		}

		if(m_bMaxSelected)
		{
			int nValue=PosTOValue(point.y);
			if(multi_register_value[7]>0)
				write_one(g_tstat_id,131,nValue);


			m_bMaxSelected=FALSE;
			::ReleaseCapture();
			pWnd->Invalidate();
		}

		if(m_bMinSelected)
		{
			int nValue=PosTOValue(point.y);
			if(multi_register_value[7]>0)
				write_one(g_tstat_id,132,nValue);
			m_bMinSelected=FALSE;
			::ReleaseCapture();
			pWnd->Invalidate();
		}
	}
	else
	{
		/*
		
			nMode=multi_register_value[125];
			if(nMode>=0)
			{
				if(nMode==0)
				{		
					nValueHeat=multi_register_value[182];
					nValueCool=multi_register_value[183];
				}
				else
				{
					nValueHeat=multi_register_value[123];
					nValueCool=multi_register_value[124];
				}

			}
		*/

		if(m_bHeatSelected)
		{
			if(multi_register_value[7]>0)
			{
				
				int nValue=PosTOValue(point.y);
				int nMode=-1;
				nMode=multi_register_value[125];
				int nReg;
				if(nMode>=0)
				{
					if(nMode==0)
					{
						nReg=182;
					}
					if(nMode==1)
					{
						nReg=123;
					}
				
				}
				write_one(g_tstat_id,nReg,nValue);

			}

			m_bHeatSelected=FALSE;
			::ReleaseCapture();
			pWnd->Invalidate();
		}
		if(m_bCoolSelected)
		{
			if(multi_register_value[7]>0)
			{
				
				int nValue=PosTOValue(point.y);
				int nMode=-1;
				nMode=multi_register_value[125];
				int nReg;
				if(nMode>=0)
				{
					if(nMode==0)
					{
						nReg=183;
					}
					if(nMode==1)
					{
						nReg=124;
					}
				
				}
				write_one(g_tstat_id,nReg,nValue);
			}
			m_bCoolSelected=FALSE;
			::ReleaseCapture();
			pWnd->Invalidate();
		}

	}
	
}

void CSetptGraphicBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if(point.y<m_rcTank.top+8)
		return;
	if(point.y>m_rcTank.bottom-8)
		return;
	CWnd *pWnd=CWnd::FromHandle(m_hParentWnd);
	if(m_nType==0)
	{
		if(m_bSetPtSelected)
		{
			//float newpos=(RulerBeginPix-point.y)/(m_nRulerBegin-m_nRulerEnd)
			m_nDragTempvalue=PosTOValue(point.y);
			m_SetPtImg.rcImg.top=int(point.y-m_nThumbHeight/2+0.5);
			m_SetPtImg.rcImg.bottom=int(point.y+m_nThumbHeight/2+0.5);
			pWnd->Invalidate();
		}
		if(m_bMaxSelected)
		{
			
			m_nDragTempvalue=PosTOValue(point.y);
			m_MaxImg.rcImg.top=int(point.y-m_nThumbHeight/2+0.5);
			m_MaxImg.rcImg.bottom=int(point.y+m_nThumbHeight/2+0.5);
			pWnd->Invalidate();

		}
		if(m_bMinSelected)
		{
			
			m_nDragTempvalue=PosTOValue(point.y);
			m_MinImg.rcImg.top=int(point.y-m_nThumbHeight/2+0.5);
			m_MinImg.rcImg.bottom=int(point.y+m_nThumbHeight/2+0.5);
			pWnd->Invalidate();
		}

	}
	else
	{
		if(m_bHeatSelected)
		{
			//float newpos=(RulerBeginPix-point.y)/(m_nRulerBegin-m_nRulerEnd)
			m_nDragTempvalue=PosTOValue(point.y);
			m_HeatSetPt.rcImg.top=int(point.y-m_nThumbHeight/2+0.5);
			m_HeatSetPt.rcImg.bottom=int(point.y+m_nThumbHeight/2+0.5);
			pWnd->Invalidate();
		}
		if(m_bCoolSelected)
		{
			//float newpos=(RulerBeginPix-point.y)/(m_nRulerBegin-m_nRulerEnd)
			m_nDragTempvalue=PosTOValue(point.y);
			m_CoolSetPt.rcImg.top=int(point.y-m_nThumbHeight/2+0.5);
			m_CoolSetPt.rcImg.bottom=int(point.y+m_nThumbHeight/2+0.5);
			pWnd->Invalidate();
		}
	}
	
	 

}
int CSetptGraphicBar::PosTOValue(int nPix)
{
	int nValue=(int)((m_rcClient.top+RulerBeginPix-nPix)/(float)m_nRulerHeight*(m_nRulerEnd-m_nRulerBegin)+m_nRulerBegin);
	return nValue;
}
