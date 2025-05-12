// GraphicMode.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "GraphicMode.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "global_function.h"




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

// CGraphicMode dialog

IMPLEMENT_DYNAMIC(CGraphicMode, CDialogEx)

CGraphicMode::CGraphicMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphicMode::IDD, pParent)
{
    for (int i=0;i<127;i++)
    {
        pSeries[i]=NULL;
    } 
}

CGraphicMode::~CGraphicMode()
{
}

void CGraphicMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);

	DDX_Control(pDX, IDC_LIST_USER, m_user_list);
	DDX_Control(pDX, IDC_LIST_FACTORY, m_factory_list);
	DDX_Control(pDX, IDC_CHECK_USER, m_check_user);
	DDX_Control(pDX, IDC_CHECK_DEFAULT, m_check_default);
}


BEGIN_MESSAGE_MAP(CGraphicMode, CDialogEx)
	ON_WM_SIZE()
	//ON_BN_CLICKED(IDC_CHECK_USER, &CAirQuality::OnBnClickedUser)
	//ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CAirQuality::OnBnClickedDefault)
     
    ON_BN_CLICKED(IDC_CHECK_USER, &CGraphicMode::OnBnClickedCheckUser)
    ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CGraphicMode::OnBnClickedCheckDefault)
    ON_BN_CLICKED(IDD_USER_TABLE_ADD, &CGraphicMode::OnBnClickedUserTableAdd)
    ON_BN_CLICKED(IDD_USER_TABLE_DEL, &CGraphicMode::OnBnClickedUserTableDel)

    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Input_Item)
END_MESSAGE_MAP()


// CGraphicMode message handlers


void CGraphicMode::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType==SIZE_RESTORED)
	{

		CRect ViewRect;
		GetClientRect(&ViewRect);


		if (m_ChartCtrl.GetSafeHwnd())
		{
			m_ChartCtrl.MoveWindow(CRect(250,0,ViewRect.Width(),ViewRect.Height()),TRUE);
		}

	}
	
}


BOOL CGraphicMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    
	
	CRect ViewRect;
	GetClientRect(&ViewRect);


	if (m_ChartCtrl.GetSafeHwnd())
	{
		m_ChartCtrl.MoveWindow(CRect(250,0,ViewRect.Width(),ViewRect.Height()),TRUE);
	}
     CString StrTemp;
     StrTemp.Format(_T("%d"),product_register_value[g_calibration_module_data.Current_Frequency.regAddress]);
     GetDlgItem(IDC_EDIT_FREQUENCE)->SetWindowText(StrTemp);
     StrTemp = g_calibration_module_data.Current_Frequency.StrValue;
     GetDlgItem(IDC_STATIC_X_NAME)->SetWindowText(StrTemp);
     LoadRegistersGraphicMode();
	 CalculateChartData();
     Initial_UserList();
     Initial_FactoryList();
     ShowChecks();
	 InitChartCtrl();
	 DrawerPoint();
	return TRUE;  
}

void CGraphicMode::CalculateChartData(){
   unsigned short Temp_Value[20];
   int Start_Address = 0;

   int m_Draw_User=3;
   int dataline = 0;
   Registers_Infor temp;
   RegPoint temppoint;
   g_calibration_module_data.Factory_Table.clear();
   g_calibration_module_data.User_Table.clear();
   g_calibration_module_data.Graphic_Data.clear();
   if (g_calibration_module_data.Factory_Fre.regAddress < g_calibration_module_data.Factory_Hum.regAddress)
   {
		m_Factory_Fre_First =TRUE;
		Start_Address =	g_calibration_module_data.Factory_Fre.regAddress;
   }
   else
   {
		m_Factory_Fre_First =FALSE;
	    Start_Address =	g_calibration_module_data.Factory_Hum.regAddress;
   }
   int Ret =  Read_Multi(g_tstat_id,Temp_Value,Start_Address,20);
   if (Ret > 0)
   {
		for (int i = 0;i < 20 ;i++)
		{
			m_Factory_Table_Value[i] = Temp_Value[i];
            product_register_value[Start_Address + i] = Temp_Value[i];
			Temp_Value[i]=-1;
		}
   }

   if (g_calibration_module_data.User_Fre.regAddress < g_calibration_module_data.User_Hum.regAddress)
   {
	   m_User_Fre_First =TRUE;
	   Start_Address =	g_calibration_module_data.User_Fre.regAddress;
   }
   else
   {
	   m_User_Fre_First =FALSE;
	   Start_Address =	g_calibration_module_data.User_Hum.regAddress;
   }
    Ret =  Read_Multi(g_tstat_id,Temp_Value,Start_Address,20);
   if (Ret > 0)
   {
	   for (int i = 0;i < 20 ;i++)
	   {
		   m_User_Table_Value[i] = Temp_Value[i];
           product_register_value[Start_Address + i] = Temp_Value[i];
		   Temp_Value[i]=-1;
	   }
   }

   Start_Address =  g_calibration_module_data.User_Offset.regAddress;
   Ret = Read_Multi (g_tstat_id,Temp_Value,Start_Address,1);
   if (Ret >0 )
   {
        g_calibration_module_data.User_Offset.RegValue = Temp_Value[0];
   }
    
#if 1
   dataline = 0;
   temp.Reg_Name=_T("Factory Table Line");
   temp.Point_Color = RGB(0,100,255);
   for (int row=1;row<=10;row++)
   {

	   if (m_Factory_Fre_First)
	   {
		   if ((m_Factory_Table_Value[2*(row-1)] == 0)||(m_Factory_Table_Value[2*(row-1)] == 65535))
		   {
			   continue;
		   }
		   temppoint.Time_offset=(short)m_Factory_Table_Value[2*(row-1)];
		   temppoint.Value_offset =(short)m_Factory_Table_Value[2*row-1];
	   }
	   else
	   {
		   if ((m_Factory_Table_Value[2*(row-1)+1] == 0)||(m_Factory_Table_Value[2*(row-1)+1] == 65535) )
		   {
			   continue;
		   }
		   temppoint.Time_offset=(short)m_Factory_Table_Value[2*row-1];
		   temppoint.Value_offset =(short)m_Factory_Table_Value[2*(row-1)];
	   }
	   g_calibration_module_data.Factory_Table.push_back(temppoint);
	   temp.Reg_Point.push_back(temppoint);
	   dataline++;
   }

   g_calibration_module_data.Graphic_Data.push_back(temp);

   temp.Reg_Point.clear();

   temp.Reg_Name=_T("User Table Line");
   temp.Point_Color = RGB(255,0,0);
#endif
   int User_Calibration_Point = product_register_value[g_calibration_module_data.User_Table_Point_Number.regAddress];
   if (User_Calibration_Point == 1||(product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] == 1&&User_Calibration_Point == 0))
   {
	   for (int row = 0;row < User_Calibration_Point ; row++)
	   {
		   if(m_User_Fre_First)
		   {
			   temppoint.Time_offset  = m_User_Table_Value[2*row];
			   temppoint.Value_offset = m_User_Table_Value[2*row + 1];
		   }
		   else
		   {
			   temppoint.Time_offset  = m_User_Table_Value[2*row + 1];
			   temppoint.Value_offset = m_User_Table_Value[2*row];
		   }
		  g_calibration_module_data.User_Table.push_back(temppoint);
	   }

	   for (int i = 0; i < (int)g_calibration_module_data.Factory_Table.size();i++)
	   {
            temppoint.Time_offset = g_calibration_module_data.Factory_Table.at(i).Time_offset;
            temppoint.Value_offset = g_calibration_module_data.Factory_Table.at(i).Value_offset + g_calibration_module_data.User_Offset.RegValue;
            temp.Reg_Point.push_back(temppoint);
	   }
        g_calibration_module_data.Graphic_Data.push_back(temp);
   }
   else if (User_Calibration_Point == 0&&product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] == 0)
   {
		
   }
   else 
   {
       for (int row = 0;row < User_Calibration_Point ; row++)
       {
           if(m_User_Fre_First)
           {
               temppoint.Time_offset  = m_User_Table_Value[2*row];
               temppoint.Value_offset = m_User_Table_Value[2*row + 1];
           }
           else
           {
               temppoint.Time_offset  = m_User_Table_Value[2*row + 1];
               temppoint.Value_offset = m_User_Table_Value[2*row];
           }
           g_calibration_module_data.User_Table.push_back(temppoint);
           temp.Reg_Point.push_back(temppoint);
       }
       g_calibration_module_data.Graphic_Data.push_back(temp);
   }
   
   m_xMax =(int) g_calibration_module_data.Graphic_Data.at(0).Reg_Point.at(0).Time_offset;
   m_xMin =(int) g_calibration_module_data.Graphic_Data.at(0).Reg_Point.at(0).Time_offset;
   m_yMax =(int) g_calibration_module_data.Graphic_Data.at(0).Reg_Point.at(0).Value_offset;
   m_yMin =(int) g_calibration_module_data.Graphic_Data.at(0).Reg_Point.at(0).Value_offset;

   for (int i = 0 ;i < (int)g_calibration_module_data.Graphic_Data.size(); i++)
   {
        for (int j = 0; j < (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.size(); j++)
        {
              if (m_xMax < (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Time_offset)
              {
                    m_xMax = (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Time_offset; 
              } 
              if (m_xMin > (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Time_offset)
              {
                  m_xMin = (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Time_offset; 
              }
              
              if (m_yMax < (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Value_offset)
              {
                  m_yMax = (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Value_offset; 
              } 
              if (m_yMin > (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Value_offset)
              {
                  m_yMin = (int)g_calibration_module_data.Graphic_Data.at(i).Reg_Point.at(j).Value_offset; 
              }
               

        }
   }
   
}

void CGraphicMode::ShowChecks(){
	BOOL is_grad = TRUE;
	if (product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] == 1)
	{
		is_grad = FALSE;
	}
	if (is_grad)
	{
		m_check_default.SetCheck(TRUE);
		m_check_user.SetCheck(FALSE);
        GetDlgItem(IDD_USER_TABLE_ADD)->EnableWindow(FALSE);
        GetDlgItem(IDD_USER_TABLE_DEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
	} 
	else
	{
		m_check_default.SetCheck(FALSE);
		m_check_user.SetCheck(TRUE);
        GetDlgItem(IDD_USER_TABLE_ADD)->EnableWindow(TRUE);
        GetDlgItem(IDD_USER_TABLE_DEL)->EnableWindow(TRUE);
        GetDlgItem(IDC_LIST_USER)->EnableWindow(TRUE);
	}

	//Initial_UserList();
     PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
}
void CGraphicMode::OnBnClickedUser(){

	 
		if (product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] == 1)//说明是选中状态
		{

			ShowChecks();
			return;
		}



        int ret=write_one(g_tstat_id,g_calibration_module_data.User_Table_Selection.regAddress,1);
        if (ret>0)
        {
            product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] = 1;
        }
        else
        {
            AfxMessageBox(_T("Setting Fail!"));
        }
        ShowChecks();
	 

}
void CGraphicMode::OnBnClickedDefault(){
 
	int ret=write_one(g_tstat_id,g_calibration_module_data.User_Table_Selection.regAddress,0);
	if (ret>0)
	{
		product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] = 0;

	}
	else
	{
		AfxMessageBox(_T("Setting Fail!"));
	}
	ShowChecks();
	 

}

void CGraphicMode::InitChartCtrl(){
	CChartTitle* pTitle = m_ChartCtrl.GetTitle();
	pTitle->RemoveAll();
	pTitle->AddString(_T("Graphic Mode"));
	pTitle->SetColor(RGB(255,0,0));

	CChartStandardAxis* pBottomAxis = 
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(m_xMin-100, m_xMax+100);
    pBottomAxis->GetLabel ()->SetText (L"Frequency/Hz") ;
    // pBottomAxis->GetLabel ()->SetFont (30,L"Arial") ;
    /*pAxis->GetLabel()->SetText(szBuffer);*/
   

    
         
	CChartStandardAxis* pLeftAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(m_yMin-20, m_yMax+20);
    pLeftAxis->GetLabel ()->SetText (L"Humidity/%");
   // pLeftAxis->GetLabel ()->SetFont (30,L"Arial") ;
	//pLeftAxis->SetMinMax(0, 1000);
 	CChartStandardAxis* pTopAxis =
 		m_ChartCtrl.CreateStandardAxis(CChartCtrl::TopAxis);
 	pTopAxis->SetMinMax(m_xMin-100, m_xMax+100);

 	CChartStandardAxis* pRightAxis =
 		m_ChartCtrl.CreateStandardAxis(CChartCtrl::RightAxis);
 	pRightAxis->SetMinMax(m_yMin-20, m_yMax+20);
	//pRightAxis->SetMinMax(0, 1000);
	m_ChartCtrl.GetLegend()->SetVisible(true);
     m_ChartCtrl.GetTopAxis ()->SetVisible (false);
     m_ChartCtrl.GetRightAxis ()->SetVisible (false);
	m_ChartCtrl.RefreshCtrl();
}

void CGraphicMode::DrawerPoint(){
	int Type=0;
	 
	//
	for (int i=0;i< (int)g_calibration_module_data.Graphic_Data.size();i++)
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

				pLineSeries->SetPenStyle(0);
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


		TChartString Name=g_calibration_module_data.Graphic_Data[i].Reg_Name.GetBuffer(0);

		pSeries[i]->SetName(Name);
		pSeries[i]->SetColor(g_calibration_module_data.Graphic_Data[i].Point_Color);

		int NumberPoints =g_calibration_module_data.Graphic_Data[i].Reg_Point.size();


		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];





		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_calibration_module_data.Graphic_Data[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset ;
			CString str;
			str.Format(_T("\nx=%0.1f y=%0.1f\n"),XValues[j],YValues[j]);
			TRACE(str);
		}

		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);


		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();


	}


	  Type=1;

	//
	for (int i=0;i< (int)g_calibration_module_data.Graphic_Data.size();i++)
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

				pLineSeries->SetPenStyle(0);
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
				pPointsSeries->SetPointSize(8,8);

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


		TChartString Name=g_calibration_module_data.Graphic_Data[i].Reg_Name.GetBuffer(0);

		//pSeries[i]->SetName(Name);
		 pSeries[i]->SetColor(g_calibration_module_data.Graphic_Data[i].Point_Color);

		int NumberPoints =g_calibration_module_data.Graphic_Data[i].Reg_Point.size();


		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];





		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_calibration_module_data.Graphic_Data[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset ;
			CString str;
			str.Format(_T("\nx=%0.1f y=%0.1f\n"),XValues[j],YValues[j]);
			TRACE(str);
		}

		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);


		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();
 
	}
}
void CGraphicMode::Fresh_Graphic(){


    m_ChartCtrl.RemoveAllSeries();
    //m_ChartCtrl.RefreshCtrl();
    m_ChartCtrl.GetLeftAxis()->SetMinMax(m_yMin-20, m_yMax+20);
    m_ChartCtrl.GetRightAxis()->SetMinMax(m_yMin-20, m_yMax+20);
    m_ChartCtrl.GetBottomAxis()->SetMinMax(m_xMin-100, m_xMax+100);
    m_ChartCtrl.GetTopAxis()->SetMinMax(m_xMin-100, m_xMax+100);

    m_ChartCtrl.RefreshCtrl();

  	int Type=0;
	//
	for (int i=0;i< (int)g_calibration_module_data.Graphic_Data.size();i++)
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

				pLineSeries->SetPenStyle(0);
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


		TChartString Name=g_calibration_module_data.Graphic_Data[i].Reg_Name.GetBuffer(0);

 		pSeries[i]->SetName(Name);
 		pSeries[i]->SetColor(g_calibration_module_data.Graphic_Data[i].Point_Color);

		int NumberPoints =g_calibration_module_data.Graphic_Data[i].Reg_Point.size();


		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];





		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_calibration_module_data.Graphic_Data[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset ;
			CString str;
			str.Format(_T("\nx=%0.1f y=%0.1f\n"),XValues[j],YValues[j]);
			TRACE(str);
		}

		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);


		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();


	}
    Type=1;
	//
	for (int i=0;i< (int)g_calibration_module_data.Graphic_Data.size();i++)
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

				pLineSeries->SetPenStyle(0);
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
				pPointsSeries->SetPointSize(8,8);

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


		TChartString Name=g_calibration_module_data.Graphic_Data[i].Reg_Name.GetBuffer(0);

		//pSeries[i]->SetName(Name);
		pSeries[i]->SetColor(g_calibration_module_data.Graphic_Data[i].Point_Color);

		int NumberPoints =g_calibration_module_data.Graphic_Data[i].Reg_Point.size();
               

		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];





		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_calibration_module_data.Graphic_Data[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset ;
			CString str;
			str.Format(_T("\nx=%0.1f y=%0.1f\n"),XValues[j],YValues[j]);
			TRACE(str);
		}

		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);


		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();


	}
}
void CGraphicMode::Initial_UserList(){
	int AddressValue = -1;
	BOOL is_grad = TRUE;
 
	if (product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] == 1)
	{
		is_grad = FALSE;
	}
	int rows =(int)g_calibration_module_data.User_Table.size();

	CString strTemp;
	m_user_list.ShowWindow(SW_HIDE);
	m_user_list.DeleteAllItems();
	while ( m_user_list.DeleteColumn (0)) ;

	m_user_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_user_list.SetExtendedStyle(m_user_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_user_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_user_list.InsertColumn(1, g_calibration_module_data.User_Fre.StrValue, 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(2, g_calibration_module_data.User_Hum.StrValue, 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

	CString index;
	for (int i=0;i< rows;i++)
	{
		index.Format(_T("%d"),i+1);
		m_user_list.InsertItem(i,index);
	}
	for (int row=0;row<rows;row++)
	{
			strTemp.Format(_T("%d"),(int)g_calibration_module_data.User_Table.at(row).Time_offset);

			m_user_list.SetItemText(row,1,strTemp);
			strTemp.Format(_T("%0.1f"),g_calibration_module_data.User_Table.at(row).Value_offset/10.0);
			m_user_list.SetItemText(row,2,strTemp);
            if (is_grad)
            {
                m_user_list.SetCellEnabled(row,0,FALSE);
                m_user_list.SetCellEnabled(row,1,FALSE);
                m_user_list.SetItemBkColor(row,0,DISABLE_COLOR_CELL);
                m_user_list.SetItemBkColor(row,1,DISABLE_COLOR_CELL);
                m_user_list.SetItemBkColor(row,2,DISABLE_COLOR_CELL); 
            }
            else
            {
                m_user_list.SetCellEnabled(row,0,TRUE);
                m_user_list.SetCellEnabled(row,1,TRUE);
                m_user_list.SetItemBkColor(row,0,RGB(255,255,255));
                m_user_list.SetItemBkColor(row,1,RGB(255,255,255));
                m_user_list.SetItemBkColor(row,2,RGB(255,255,255)); 

            }
	}

	m_user_list.ShowWindow(TRUE);
}

void CGraphicMode::Initial_FactoryList(){
	 
	int rows = (int)g_calibration_module_data.Factory_Table.size();
	m_factory_list.ShowWindow(SW_HIDE);
	m_factory_list.DeleteAllItems();
	while ( m_factory_list.DeleteColumn (0)) ;

	m_factory_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_factory_list.SetExtendedStyle(m_factory_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_factory_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_factory_list.InsertColumn(1, g_calibration_module_data.Factory_Fre.StrValue, 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_factory_list.InsertColumn(2, g_calibration_module_data.Factory_Hum.StrValue, 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;


	CString index;
	for (int i=0;i< rows;i++)
	{
		index.Format(_T("%d"),i+1);
		m_factory_list.InsertItem(i,index);
	}

	CString strTemp;
 
		for (int row=0;row<rows;row++)
		{

			strTemp.Format(_T("%d"), (int)g_calibration_module_data.Factory_Table.at(row).Time_offset);
			m_factory_list.SetItemText(row,1,strTemp); 


			strTemp.Format(_T("%0.1f"), g_calibration_module_data.Factory_Table.at(row).Value_offset/10.0);
			m_factory_list.SetItemText(row,2,strTemp);

			m_factory_list.SetCellEnabled(row,0,FALSE);
			m_factory_list.SetCellEnabled(row,1,FALSE);
			m_factory_list.SetItemBkColor(row,0,DISABLE_COLOR_CELL);
			m_factory_list.SetItemBkColor(row,1,DISABLE_COLOR_CELL);
			m_factory_list.SetItemBkColor(row,2,DISABLE_COLOR_CELL);  


		}
 

	m_factory_list.ShowWindow(TRUE);
}

void CGraphicMode::OnBnClickedCheckUser()
{
    OnBnClickedUser();  
    CalculateChartData();
    //DrawerPoint ();
    Fresh_Graphic ();
}

void CGraphicMode::OnBnClickedCheckDefault()
{
    OnBnClickedDefault();
    CalculateChartData();
    Fresh_Graphic ();
   // DrawerPoint ();
}

void CGraphicMode::OnBnClickedUserTableAdd()
{
    int AddressValue = g_calibration_module_data.User_Table_Point_Number.regAddress;

    if (product_register_value[AddressValue] >= 10)
    {
        AfxMessageBox(_T("Can't Add,Current Point can't be more than 10!"));
        return;
    }
    
    int ret = write_one(g_tstat_id,AddressValue,product_register_value[AddressValue]+1);
    if (ret>0)
    {
        ++product_register_value[AddressValue];
    }
    else
    {
        AfxMessageBox(_T("Add Fail,Try again!"));
        return;
    }
    CalculateChartData();
    //Initial_UserList();
    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
    Fresh_Graphic();
}


void CGraphicMode::OnBnClickedUserTableDel()
{
    
    int AddressValue = g_calibration_module_data.User_Table_Point_Number.regAddress;

    if (product_register_value[AddressValue] <= 0)
    {
        AfxMessageBox(_T("Can't Add,Current Point can't be less than 0!"));
        return;
    }
    int ret = write_one(g_tstat_id,AddressValue,product_register_value[AddressValue]-1);
    if (ret>0)
    {
        --product_register_value[AddressValue];

        int RegAddress;
        //int row =m_sensor_calibrate_table.get_Rows();
        int row =m_user_list.GetItemCount();
        row -= 1;//末尾行
         
        RegAddress=g_calibration_module_data.User_Hum.regAddress+2*row;
        int ret=write_one(g_tstat_id,RegAddress,0);
    }
    else
    {
        AfxMessageBox(_T("Add Fail,Try again!"));
        return;
    }
    CalculateChartData();
    
    
    //Initial_UserList();
    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);

    Fresh_Graphic();
}

LRESULT CGraphicMode::Change_Input_Item(WPARAM wParam,LPARAM lParam){

    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam; 
    int AddressValue = -1; 
    CString temp_task_info;
 
    if (Changed_SubItem == 1)
    {
       m_user_list.Set_Edit(false);
       return 0;
    }
    if(Changed_SubItem == 2)
    {
              
                CString New_CString =  m_user_list.GetItemText(Changed_Item,Changed_SubItem);
                int ItemValue = (int)(_wtof(New_CString)*10);
                int RegAddress;
                if(New_CString.IsEmpty())
                {
                    //Initial_UserList(); 
                     PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                    return FALSE;
                }
                RegAddress=g_calibration_module_data.User_Hum.regAddress+2*Changed_Item;
                
                if((int)g_calibration_module_data.User_Table.at(Changed_Item).Value_offset==ItemValue)	//Add this to judge weather this value need to change.
                {
                    //Initial_UserList(); 
                     PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0); 
                    return FALSE;
                }
                int ret=write_one(g_tstat_id,RegAddress,ItemValue);
                if (ret>0)
                {
                   // product_register_value[RegAddress]=ItemValue;		 
                }
                else
                {
                    AfxMessageBox(_T("Write Fail!"));
                    m_user_list.Set_Edit(false);
                    return false;
                }
                CalculateChartData();
                //Initial_UserList();
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0); 
                Fresh_Graphic();
    }
    return 0;
}

LRESULT CGraphicMode::Fresh_Input_List(WPARAM wParam,LPARAM lParam){
    CString strTemp;
 
    int AddressValue = -1;
    BOOL is_grad = TRUE;

    if (product_register_value[g_calibration_module_data.User_Table_Selection.regAddress] == 1)
    {
        is_grad = FALSE;
    }
    int rows =(int)g_calibration_module_data.User_Table.size();

 
    
    m_user_list.DeleteAllItems();
     
    
    CString index;
    for (int i=0;i< rows;i++)
    {
        index.Format(_T("%d"),i+1);
        m_user_list.InsertItem(i,index);
    }
    for (int row=0;row<rows;row++)
    {
        strTemp.Format(_T("%d"),(int)g_calibration_module_data.User_Table.at(row).Time_offset);

        m_user_list.SetItemText(row,1,strTemp);
        strTemp.Format(_T("%0.1f"),g_calibration_module_data.User_Table.at(row).Value_offset/10.0);
        m_user_list.SetItemText(row,2,strTemp);
        if (is_grad)
        {
            m_user_list.SetCellEnabled(row,0,FALSE);
            m_user_list.SetCellEnabled(row,1,FALSE);
            m_user_list.SetItemBkColor(row,0,DISABLE_COLOR_CELL);
            m_user_list.SetItemBkColor(row,1,DISABLE_COLOR_CELL);
            m_user_list.SetItemBkColor(row,2,DISABLE_COLOR_CELL); 
        }
        else
        {
            m_user_list.SetCellEnabled(row,0,TRUE);
            m_user_list.SetCellEnabled(row,1,TRUE);
            m_user_list.SetItemBkColor(row,0,RGB(255,255,255));
            m_user_list.SetItemBkColor(row,1,RGB(255,255,255));
            m_user_list.SetItemBkColor(row,2,RGB(255,255,255)); 

        }
    }
 
    return 0;
}
