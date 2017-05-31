// RegisterValueAnalyzerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "RegisterValueAnalyzerDlg.h"
#include "afxdialogex.h"




#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartPointsSerie.h"
#include "ChartCtrl/ChartSurfaceSerie.h"
#include "ChartCtrl/ChartGrid.h"

 

#include "ChartCtrl/ChartBarSerie.h"
#include "ChartCtrl/ChartLabel.h"

#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartStandardAxis.h"
#include "ChartCtrl/ChartDateTimeAxis.h"

#include "ChartCtrl/ChartCrossHairCursor.h"
#include "ChartCtrl/ChartDragLineCursor.h"

#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include "global_variable_extern.h"

// CRegisterValueAnalyzerDlg dialog

IMPLEMENT_DYNAMIC(CRegisterValueAnalyzerDlg, CDialogEx)

CRegisterValueAnalyzerDlg::CRegisterValueAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterValueAnalyzerDlg::IDD, pParent)
{

}

CRegisterValueAnalyzerDlg::~CRegisterValueAnalyzerDlg()
{
}

void CRegisterValueAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	DDX_Control(pDX, IDC_TITLE_EDIT, m_TitlesEdit);
	 
	DDX_Control(pDX, IDC_AXISMINVAL_EDIT, m_AxisMinValEdit);
	DDX_Control(pDX, IDC_AXISMAXVAL_EDIT, m_AxisMaxValEdit);
	DDX_Control(pDX, IDC_LEGENDVIS_CHECK, m_LegendVisBtn);
}


BEGIN_MESSAGE_MAP(CRegisterValueAnalyzerDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BKGND_COLBTN, &CRegisterValueAnalyzerDlg::OnBnClickedBkgndColbtn)
	ON_BN_CLICKED(IDC_LEGENDVIS_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedLegendvisCheck)
	ON_BN_CLICKED(IDC_PAN_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedPanCheck)
	ON_BN_CLICKED(IDC_ZOOM_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedZoomCheck)
	ON_EN_KILLFOCUS(IDC_TITLE_EDIT, &CRegisterValueAnalyzerDlg::OnEnKillfocusTitleEdit)
	ON_BN_CLICKED(IDC_LEFTAXIS_RADIO, &CRegisterValueAnalyzerDlg::OnBnClickedLeftaxisRadio)
	ON_BN_CLICKED(IDC_BOTTOMAXIS_RADIO, &CRegisterValueAnalyzerDlg::OnBnClickedBottomaxisRadio)
	ON_BN_CLICKED(IDC_RIGHTAXIS_RADIO, &CRegisterValueAnalyzerDlg::OnBnClickedRightaxisRadio)
	ON_BN_CLICKED(IDC_TOPAXIS_RADIO, &CRegisterValueAnalyzerDlg::OnBnClickedTopaxisRadio)
	ON_BN_CLICKED(IDC_AXISVISIBLE_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedAxisvisibleCheck)
	ON_BN_CLICKED(IDC_AXISINVERTED_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedAxisinvertedCheck)
	ON_BN_CLICKED(IDC_AXISAUTOMATIC_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedAxisautomaticCheck)
	ON_BN_CLICKED(IDC_AXISGRIDVIS_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedAxisgridvisCheck)
	ON_BN_CLICKED(IDC_AXISSCROLLBAR_CHECK, &CRegisterValueAnalyzerDlg::OnBnClickedAxisscrollbarCheck)
	ON_EN_KILLFOCUS(IDC_AXISMINVAL_EDIT, &CRegisterValueAnalyzerDlg::OnEnKillfocusAxisminvalEdit)
	ON_EN_KILLFOCUS(IDC_AXISMAXVAL_EDIT, &CRegisterValueAnalyzerDlg::OnEnKillfocusAxismaxvalEdit)
	ON_EN_KILLFOCUS(IDC_AXISLABEL_EDIT, &CRegisterValueAnalyzerDlg::OnEnKillfocusAxislabelEdit)
	ON_WM_TIMER()
	//ON_REGISTERED_MESSAGE(MY_FRESH_DRAW_GRAPHIC, &CRegisterValueAnalyzerDlg::OnMyFreshDrawGraphic)
END_MESSAGE_MAP()


// CRegisterValueAnalyzerDlg message handlers


void CRegisterValueAnalyzerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType==SIZE_RESTORED)
	{

		CRect ViewRect;
		GetClientRect(&ViewRect);
		 
		 
		if (m_ChartCtrl.GetSafeHwnd())
		{
			m_ChartCtrl.MoveWindow(CRect(120,0,ViewRect.Width(),ViewRect.Height()),TRUE);
		}

	}
}


void CRegisterValueAnalyzerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CRegisterValueAnalyzerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CRegisterValueAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CButton*)GetDlgItem(IDC_LEFTAXIS_RADIO))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_PAN_CHECK))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ZOOM_CHECK))->SetCheck(1);
	GetDlgItem(IDC_TITLE_EDIT)->SetWindowText(_T("Register Value Analyzer"));

	int Count = m_TitlesEdit.GetLineCount();
	CChartTitle* pTitle = m_ChartCtrl.GetTitle();
	pTitle->RemoveAll();

	TCHAR szBuff[255];
	for (int i=0;i<Count;i++)
	{
		int Size = m_TitlesEdit.GetLine(i,szBuff);
		szBuff[Size] = '\0';
		pTitle->AddString(szBuff);
	}




	CChartStandardAxis* pBottomAxis = 
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(0, 1000);
	
	CChartStandardAxis* pLeftAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0, 1000);

	CChartStandardAxis* pTopAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::TopAxis);
	pTopAxis->SetMinMax(0, 1000);

	CChartStandardAxis* pRightAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::RightAxis);
	pRightAxis->SetMinMax(0, 1000);

	OnBnClickedLeftaxisRadio();
	m_LegendVisBtn.SetCheck(1);
	if (m_LegendVisBtn.GetCheck() == 1)
		m_ChartCtrl.GetLegend()->SetVisible(true);
	else
		m_ChartCtrl.GetLegend()->SetVisible(false);
	m_ChartCtrl.RefreshCtrl();
	
	CRect ViewRect;
	GetClientRect(&ViewRect);

	
	if (m_ChartCtrl.GetSafeHwnd())
	{
		m_ChartCtrl.MoveWindow(CRect(120,0,ViewRect.Width(),ViewRect.Height()),TRUE);
	}


	DrawerPoint();
	SetTimer(DATATIME_TIMER,2000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CChartAxis* CRegisterValueAnalyzerDlg::GetSelectedAxis()
{
	if ( ((CButton*)GetDlgItem(IDC_LEFTAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetLeftAxis();
	if ( ((CButton*)GetDlgItem(IDC_BOTTOMAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetBottomAxis();
	if ( ((CButton*)GetDlgItem(IDC_RIGHTAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetRightAxis();
	if ( ((CButton*)GetDlgItem(IDC_TOPAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetTopAxis();

	return NULL;
}

void CRegisterValueAnalyzerDlg::OnBnClickedBkgndColbtn()
{
	CColorDialog colorDlg;
	//colorDlg.SetWindowText(_T("Select background color"));
	if(colorDlg.DoModal()){
		m_BackColor=colorDlg.GetColor();
		m_ChartCtrl.SetBackColor(m_BackColor);
		m_ChartCtrl.RefreshCtrl();
	}
}


void CRegisterValueAnalyzerDlg::OnBnClickedLegendvisCheck()
{
	if (m_LegendVisBtn.GetCheck() == 1)
		m_ChartCtrl.GetLegend()->SetVisible(true);
	else
		m_ChartCtrl.GetLegend()->SetVisible(false);
	m_ChartCtrl.RefreshCtrl();
}


void CRegisterValueAnalyzerDlg::OnBnClickedPanCheck()
{
	if ( ((CButton*)GetDlgItem(IDC_PAN_CHECK))->GetCheck() == 1)
		m_ChartCtrl.SetPanEnabled(true);
	else
		m_ChartCtrl.SetPanEnabled(false);
}


void CRegisterValueAnalyzerDlg::OnBnClickedZoomCheck()
{
	if ( ((CButton*)GetDlgItem(IDC_ZOOM_CHECK))->GetCheck() == 1)
		m_ChartCtrl.SetZoomEnabled(true);
	else
		m_ChartCtrl.SetZoomEnabled(false);	
}


void CRegisterValueAnalyzerDlg::OnEnKillfocusTitleEdit()
{
	int Count = m_TitlesEdit.GetLineCount();
	CChartTitle* pTitle = m_ChartCtrl.GetTitle();
	pTitle->RemoveAll();

	TCHAR szBuff[255];
	for (int i=0;i<Count;i++)
	{
		int Size = m_TitlesEdit.GetLine(i,szBuff);
		szBuff[Size] = '\0';
		pTitle->AddString(szBuff);
	}

	m_ChartCtrl.RefreshCtrl();
}


void CRegisterValueAnalyzerDlg::OnBnClickedLeftaxisRadio()
{
	CChartAxis* pAxis = m_ChartCtrl.GetLeftAxis();
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);	
}


void CRegisterValueAnalyzerDlg::OnBnClickedBottomaxisRadio()
{
	CChartAxis* pAxis = m_ChartCtrl.GetBottomAxis();
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);
}


void CRegisterValueAnalyzerDlg::OnBnClickedRightaxisRadio()
{
	CChartAxis* pAxis = m_ChartCtrl.GetRightAxis();
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);	
}


void CRegisterValueAnalyzerDlg::OnBnClickedTopaxisRadio()
{
	CChartAxis* pAxis = m_ChartCtrl.GetTopAxis();
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);	
}


void CRegisterValueAnalyzerDlg::OnBnClickedAxisvisibleCheck()
{
	CChartAxis* pAxis = GetSelectedAxis();
	if ( ((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->GetCheck() == 1)
		pAxis->SetVisible(true);
	else
		pAxis->SetVisible(false);	
	m_ChartCtrl.RefreshCtrl();
}


void CRegisterValueAnalyzerDlg::OnBnClickedAxisinvertedCheck()
{
	CChartAxis* pAxis = GetSelectedAxis();
	if ( ((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->GetCheck() == 1)
		pAxis->SetInverted(true);
	else
		pAxis->SetInverted(false);	
	m_ChartCtrl.RefreshCtrl();	
}


void CRegisterValueAnalyzerDlg::OnBnClickedAxisautomaticCheck()
{

	CChartAxis* pAxis = GetSelectedAxis();
	if ( ((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->GetCheck() == 1)
		pAxis->SetAutomatic(true);
	else
	{
		TCHAR szBuffer[255];
		double MinVal=0, MaxVal=0;
		m_AxisMinValEdit.GetWindowText(szBuffer,254);
		//		MinVal = _tstof(szBuffer);
		MinVal = _tcstod(szBuffer, NULL);
		m_AxisMaxValEdit.GetWindowText(szBuffer,254);
		//		MaxVal = _tstof(szBuffer);
		MaxVal = _tcstod(szBuffer, NULL);

		if (MinVal > MaxVal)
		{
			MessageBox(_T("MinVal > MaxVal"),_T("Error"),MB_OK);
			((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
			return;
		}
		pAxis->SetAutomatic(false);
		pAxis->SetMinMax(MinVal,MaxVal);
	}
	m_ChartCtrl.RefreshCtrl();
}


void CRegisterValueAnalyzerDlg::OnBnClickedAxisgridvisCheck()
{
	CChartAxis* pAxis = GetSelectedAxis();
	if ( ((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->GetCheck() == 1)
		pAxis->GetGrid()->SetVisible(true);
	else
		pAxis->GetGrid()->SetVisible(false);
	m_ChartCtrl.RefreshCtrl();
}


void CRegisterValueAnalyzerDlg::OnBnClickedAxisscrollbarCheck()
{
	CChartAxis* pAxis = GetSelectedAxis();
	bool bShow = ((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->GetCheck() == 1;
	pAxis->EnableScrollBar(bShow);
}


void CRegisterValueAnalyzerDlg::OnEnKillfocusAxisminvalEdit()
{
	CChartAxis* pAxis = GetSelectedAxis();
	TCHAR szBuffer[255];
	double MinVal=0, MaxVal=0;
	m_AxisMinValEdit.GetWindowText(szBuffer,254);
	//	MinVal = _tstof(szBuffer);
	MinVal = _tcstod(szBuffer, NULL);
	m_AxisMaxValEdit.GetWindowText(szBuffer,254);
	//	MaxVal = _tstof(szBuffer);
	MaxVal = _tcstod(szBuffer, NULL);

	if (MinVal > MaxVal)
	{
		MessageBox(_T("MinVal > MaxVal"),_T("Error"),MB_OK);
		return;
	}
	pAxis->SetMinMax(MinVal,MaxVal);

	m_ChartCtrl.RefreshCtrl();	
}


void CRegisterValueAnalyzerDlg::OnEnKillfocusAxismaxvalEdit()
{
	CChartAxis* pAxis = GetSelectedAxis();
	TCHAR szBuffer[255];
	double MinVal=0, MaxVal=0;
	m_AxisMinValEdit.GetWindowText(szBuffer,254);
	//	MinVal = _tstof(szBuffer);
	MinVal = _tcstod(szBuffer, NULL);
	m_AxisMaxValEdit.GetWindowText(szBuffer,254);
	//	MaxVal = _tstof(szBuffer);
	MaxVal = _tcstod(szBuffer, NULL);

	if (MinVal > MaxVal)
	{
		MessageBox(_T("MinVal > MaxVal"),_T("Error"),MB_OK);
		return;
	}
	pAxis->SetMinMax(MinVal,MaxVal);

	m_ChartCtrl.RefreshCtrl();
}


void CRegisterValueAnalyzerDlg::OnEnKillfocusAxislabelEdit()
{
	CChartAxis* pAxis = GetSelectedAxis();
	TCHAR szBuffer[255];
	GetDlgItem(IDC_AXISLABEL_EDIT)->GetWindowText(szBuffer,254);
	pAxis->GetLabel()->SetText(szBuffer);
	m_ChartCtrl.RefreshCtrl();	
}

void CRegisterValueAnalyzerDlg::DrawerPoint(){
	int Type=0;
	CChartXYSerie* pSeries[127] = {NULL};
	for (int i=0;i<g_vectRegisters.size();i++)
	{
		bool bSecondHorizAxis =true;
		bool bSecondVertAxis =true;

		


		switch (Type)
		{
			// Line series
		case 0:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartLineSerie* pLineSeries = m_ChartCtrl.CreateLineSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pLineSeries;

				 
				/*
				实心;下划线;点;下划线-点;下划线-点-点;
				*/
				pLineSeries->SetWidth(3);
				
				pLineSeries->SetPenStyle(3);
			}
			break;

			// Points series
		case 1:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
// 				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
// 				pSeries = pPointsSeries;
// 
// 				CPointsPropDialog PointsPropDlg;
// 				PointsPropDlg.DoModal();
// 
// 				switch (PointsPropDlg.m_iPointsType)
// 				{
// 				case 0:
// 					pPointsSeries->SetPointType(CChartPointsSerie::ptEllipse);
// 					break;
// 
// 				case 1:
// 					pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
// 					break;
// 
// 				case 2:
// 					pPointsSeries->SetPointType(CChartPointsSerie::ptTriangle);
// 					break;
// 				}
// 
// 				pPointsSeries->SetPointSize(PointsPropDlg.m_iPointsWidth,PointsPropDlg.m_iPointsHeight);

				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pPointsSeries;
				pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				pPointsSeries->SetPointSize(5,5);

			}
			break;

			// Surface series
		case 2:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartSurfaceSerie* pSurfSeries = m_ChartCtrl.CreateSurfaceSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pSurfSeries;

// 				CSurfacePropDialog SurfPropDlg;
// 				SurfPropDlg.DoModal();
				int m_FillStyle=0;
				switch (m_FillStyle)
				{
				case 0:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsSolid);
					break;
				case 1:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDownDiag);
					break;
				case 2:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchUpDiag);
					break;
				case 3:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchCross);
					break;
				case 4:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDiagCross);
					break;
				case 5:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchHorizontal);
					break;
				case 6:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchVertical);
					break;
				}

				if (TRUE)
					pSurfSeries->SetHorizontal(true);
				else
					pSurfSeries->SetHorizontal(false);
			}
			break;
		}






		TChartString Name=g_vectRegisters[i].Reg_Name.GetBuffer(0);
		
		pSeries[i]->SetName(Name);
		pSeries[i]->SetColor(g_vectRegisters[i].Point_Color);
		int NumberPoints =g_vectRegisters[i].Reg_Point.size();
		 

		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];

		 

	    

		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_vectRegisters[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset;
		}
		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);


		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();
		//int index = m_SeriesList.AddString(Name.c_str());
		//m_SeriesList.SetItemData(index, pSeries->GetSerieId());

	}

}

void CRegisterValueAnalyzerDlg::Fresh(){
	int Type=1;
	CChartXYSerie* pSeries[127] = {NULL};
	for (int i=0;i<g_vectRegisters.size();i++)
	{
		bool bSecondHorizAxis =true;
		bool bSecondVertAxis =true;

		


		switch (Type)
		{
			// Line series
		case 0:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartLineSerie* pLineSeries = m_ChartCtrl.CreateLineSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pLineSeries;

				/*
				实心;下划线;点;下划线-点;下划线-点-点;
				*/
				pLineSeries->SetWidth(3);
				
				pLineSeries->SetPenStyle(3);
			}
			break;

			// Points series
		case 1:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
// 				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
// 				pSeries = pPointsSeries;
// 
// 				CPointsPropDialog PointsPropDlg;
// 				PointsPropDlg.DoModal();
// 
// 				switch (PointsPropDlg.m_iPointsType)
// 				{
// 				case 0:
// 					pPointsSeries->SetPointType(CChartPointsSerie::ptEllipse);
// 					break;
// 
// 				case 1:
// 					pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
// 					break;
// 
// 				case 2:
// 					pPointsSeries->SetPointType(CChartPointsSerie::ptTriangle);
// 					break;
// 				}
// 
// 				pPointsSeries->SetPointSize(PointsPropDlg.m_iPointsWidth,PointsPropDlg.m_iPointsHeight);

				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pPointsSeries;
				pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				pPointsSeries->SetPointSize(5,5);

			}
			break;

			// Surface series
		case 2:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartSurfaceSerie* pSurfSeries = m_ChartCtrl.CreateSurfaceSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pSurfSeries;

// 				CSurfacePropDialog SurfPropDlg;
// 				SurfPropDlg.DoModal();
				int m_FillStyle=0;
				switch (m_FillStyle)
				{
				case 0:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsSolid);
					break;
				case 1:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDownDiag);
					break;
				case 2:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchUpDiag);
					break;
				case 3:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchCross);
					break;
				case 4:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDiagCross);
					break;
				case 5:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchHorizontal);
					break;
				case 6:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchVertical);
					break;
				}

				if (TRUE)
					pSurfSeries->SetHorizontal(true);
				else
					pSurfSeries->SetHorizontal(false);
			}
			break;
		}
  
		TChartString Name=g_vectRegisters[i].Reg_Name.GetBuffer(0);
		
		//pSeries[i]->SetName(Name);
		pSeries[i]->SetColor(g_vectRegisters[i].Point_Color);
		int NumberPoints =g_vectRegisters[i].Reg_Point.size();
		 

		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];

		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_vectRegisters[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset;
		}
		pSeries[i]->AddPoints(XValues,YValues,NumberPoints);
        g_vectRegisters.at(i).Reg_Point.clear();
        /* vector<RegPoint> temp;
         temp =  g_vectRegisters.at(i).Reg_Point;
         g_vectRegisters.at(i).Reg_Point.swap(temp);*/

        g_vectRegisters.at(i).Reg_Point.shrink_to_fit();
		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();
		 

	}

}
 
void CRegisterValueAnalyzerDlg::OnTimer(UINT_PTR nIDEvent)
{
	 
	if (DATATIME_TIMER == nIDEvent){
	//	Fresh();
	}
	CDialogEx::OnTimer(nIDEvent);
}


// afx_msg LRESULT CRegisterValueAnalyzerDlg::OnMyFreshDrawGraphic(WPARAM wParam, LPARAM lParam)
// {
// 	Fresh();
// 	return 0;
// }


LRESULT CRegisterValueAnalyzerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message==MY_FRESH_DRAW_GRAPHIC)
	{
		Fresh();
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}
