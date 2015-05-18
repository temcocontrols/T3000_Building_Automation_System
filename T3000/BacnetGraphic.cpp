/*
* Copyright (C) 2014 Fance Du <dufanfance@163.com>
* BacnetGraphic.cpp : implementation file
* BacnetGraphic Code by Fance Du 2013 11 05
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*/


#include "stdafx.h"
#include "T3000.h"
#include "BacnetGraphic.h"
#include "afxdialogex.h"

#include "CM5/mygdiplus.h"
#include "CM5/PointItem.h"
// CBacnetGraphic dialog
#include <windows.h>
#include "CM5/mygdiplus.h"
#include <conio.h>
using namespace Gdiplus;

#include "BacnetMonitor.h"
#include "BADO/BADO.h"
#define  WM_MONITOR_USER_MESSAGE WM_USER + 902
CString InputLable[15];
CString InputUnit[15];
int total_y_max_value = 0x80000001;
int total_y_min_value = 0x7fffffff;

#define  INPUT_NUMBER  14

CPointItem * pTempItem = NULL;
CPointItem *pPrevItem = NULL;
CPointItem	*m_pFirstItem[INPUT_NUMBER];

//MyPoint temppoint={0,0};
//int nindex=0;
//int Total_count=0;
//const int line_interval = 30;
//static int m_interval = 0;

bool draw_graphic_finished = false;

RECT RectPosition[15];
static bool b_has_create_point = false ;
int start_wait_init = 0;	//用于控制显示刷新的变量;
extern CBacnetGraphic * GraphicWindow;
//CRect Static_Num_Rect[15];
bool flag_continue_thread = true;
HWND myhWnd;
RECT myRect;
HANDLE mythread =NULL;
HDC hMemDC;
HBITMAP hBmp;
HDC gloab_hdc;

int scale_type;
DWORD WINAPI MyThreadPro(LPVOID lPvoid);

IMPLEMENT_DYNAMIC(CBacnetGraphic, CDialogEx)

CBacnetGraphic::CBacnetGraphic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetGraphic::IDD, pParent)
	, m_xscale(6)
	, m_yscale(4)
	, m_lowvalue(0)
	, m_highvalue(100)
	, m_time_selected(TIME_TEN_MINUTE)
	, m_analogorignpoint(200,30)
{

}

CBacnetGraphic::~CBacnetGraphic()
{
}

void CBacnetGraphic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

const int WM_FRESH_STATIC = 1069;
BEGIN_MESSAGE_MAP(CBacnetGraphic, CDialogEx)
	ON_MESSAGE(WM_FRESH_STATIC,Fresh_Static_Function)
	ON_WM_TIMER()
//	ON_STN_CLICKED(IDC_STATIC_GRAPHIC_A, &CBacnetGraphic::OnStnClickedStaticGraphicA)
//ON_STN_CLICKED(IDC_STATIC_GRAPHIC_1, &CBacnetGraphic::OnStnClickedStaticGraphicA)
ON_WM_LBUTTONDOWN()
//ON_WM_NCDESTROY()
ON_WM_HELPINFO()
ON_COMMAND(ID_TIMEBASE_1HOUR, &CBacnetGraphic::OnTimebase1hour)
ON_COMMAND(ID_TIMEBASE_1DAY, &CBacnetGraphic::OnTimebase1day)
ON_COMMAND(ID_TIMEBASE_1MINITES, &CBacnetGraphic::OnTimebase1minites)
ON_COMMAND(ID_TIMEBASE_10MINUTES, &CBacnetGraphic::OnTimebase10minutes)
ON_COMMAND(ID_TIMEBASE_12HOURS, &CBacnetGraphic::OnTimebase12hours)
ON_COMMAND(ID_TIMEBASE_4HOURS, &CBacnetGraphic::OnTimebase4hours)
ON_COMMAND(ID_TIMEBASE_4DAYS, &CBacnetGraphic::OnTimebase4days)
ON_COMMAND(ID_GRAPHIC_LEFT, &CBacnetGraphic::OnGraphicLeft)
ON_COMMAND(ID_GRAPHIC_RIGHT, &CBacnetGraphic::OnGraphicRight)
ON_COMMAND(ID_ZOOMIN, &CBacnetGraphic::OnZoomin)
ON_COMMAND(ID_ZOOMOUT, &CBacnetGraphic::OnZoomout)
END_MESSAGE_MAP()


// CBacnetGraphic message handlers
unsigned long m_time_monitor_now;


int window_width;
int window_hight;
HWND test_hwnd;


void CBacnetGraphic::InitialToolBar()
{
	if(!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS|CBRS_BORDER_TOP) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return ;      // fail to create
	}

	CRect rcClientOld;
	CRect rcClientNew;
	GetClientRect(rcClientOld);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0,reposQuery,rcClientNew);
	CPoint ptOffset(rcClientNew.left -rcClientOld.left,rcClientNew.top-rcClientOld.top);
	CRect rcClild;
	CWnd * pwndChild = GetWindow(GW_CHILD);
	while(pwndChild)
	{
		pwndChild->GetWindowRect(rcClild);
		ScreenToClient(rcClild);
		rcClild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcClild,FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right+=rcClientOld.Width()-rcClientNew.Width();
	rcWindow.bottom +=rcClientOld.Height()-rcClientNew.Height();
	MoveWindow(rcWindow,FALSE);

	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(32,32));
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(54,54));
	m_wndToolBar.SetButtonText(0,_T("Read"));
	m_wndToolBar.SetButtonText(1,_T("Save"));
	m_wndToolBar.SetButtonText(2,_T("Import"));
	m_wndToolBar.SetButtonText(3,_T("Write"));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

}


BOOL CBacnetGraphic::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	srand(NULL);

	//InitialToolBar();
	b_has_create_point = false;
	flag_continue_thread = true;
	scale_type = TIME_ONE_HOUR;
	m_time_monitor_now = time(NULL);

	// TODO:  Add extra initialization here
	MSG msg;
	WNDCLASS wndClass;
	GdiplusStartupInput gdistartupInput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken,&gdistartupInput,NULL);

	myhWnd = this->m_hWnd;
	::GetWindowRect(myhWnd,&myRect);
	myRect.left=0;
	myRect.top=0;
	myRect.right=myRect.right-myRect.left;
	myRect.bottom=myRect.bottom-myRect.top;

	Get_Input_Unit();
	for(int i=0;i<14;i++)
		StaticShow[i] = true;	//初始化时都让显示,显示第一次的时候这些值会被自动的修改;
	InitialParameter(TIME_FOUR_HOUR);
	if(mythread == NULL)
	{
		mythread=CreateThread(NULL,NULL,MyThreadPro,this,NULL,NULL);
		CloseHandle(mythread);
	}
	InitDC();

	test_hwnd = this->m_hWnd;
	//Init_14_Static();

	
	CRect Graphic_Window_Rect;
	GetWindowRect(Graphic_Window_Rect);
	window_width = Graphic_Window_Rect.right - Graphic_Window_Rect.left;
	window_hight = Graphic_Window_Rect.bottom - Graphic_Window_Rect.top;





	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetGraphic::InitDC()
{
	gloab_hdc=::GetDC(myhWnd);
	if(gloab_hdc==NULL)
		return FALSE;
	hMemDC=::CreateCompatibleDC(NULL);
	if(hMemDC==NULL)
		return FALSE;

	hBmp=::CreateCompatibleBitmap(gloab_hdc, myRect.right, myRect.bottom);
	if(hBmp==NULL)
		return FALSE;

	SelectObject(hMemDC, hBmp);
	//hMemDC2=::CreateCompatibleDC(gloab_hdc);

	return TRUE;
}




int CBacnetGraphic::Search_data_from_db()
{
	unsigned long temp_timesec1970 = m_time_monitor_now; 
	unsigned long temp_time_start;
	unsigned long temp_time_end;
	CalcOnePixelTime();
	temp_time_start = temp_timesec1970 - x_axis_total_time;
	temp_time_end = temp_timesec1970;
	m_starttime = temp_time_start;
	m_endtime = temp_time_end;
	unsigned int temp_serial_number = g_selected_serialnumber;
	int temp_monitor_index = monitor_list_line;
	int temp_number_of_inputs = m_monitor_data.at(monitor_list_line).num_inputs;
	int temp_number_of_analog = m_monitor_data.at(monitor_list_line).an_inputs;
	int temp_number_of_digital = m_monitor_data.at(monitor_list_line).num_inputs - temp_number_of_analog;
	get_data_count = temp_number_of_inputs;
	monitor_analog_count = temp_number_of_analog;
	monitor_digital_count = temp_number_of_digital;
	CBADO monitor_bado;
	monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
	monitor_bado.OnInitADOConn(); 

	for (int i=0;i<temp_number_of_inputs;i++)
	{
		CString cs_temp_input_index;
		int temp_record_count = 0;
		int temp_input_number = 0;
		int temp_input_point_type = 0;
		int temp_input_point_panel = 0;
		int temp_input_point_sub_panel = 0;
		int temp_input_network = 1;
		temp_input_number = m_monitor_data.at(monitor_list_line).inputs[i].number;
		temp_input_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type;
		temp_input_point_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		temp_input_point_sub_panel = m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;
		cs_temp_input_index.Format(_T("%u_%u_%u_%u_%u"),temp_input_number,temp_input_point_type,temp_input_point_panel,temp_input_point_sub_panel,temp_input_network);

		if(i<temp_number_of_analog)
		{
			analog_data_max_value[i] = 0x80000001;
			analog_data_min_value[i] = 0x7fffffff;
			CString strSql;
			strSql.Format(_T("select * from MonitorData where Input_Index = '%s' and SerialNumber = %i and Monitor_Index = %i and Analog_Digital = 1 and Record_Time >= %i and Record_Time <= %i ORDER BY Record_Time ASC"),cs_temp_input_index,temp_serial_number,temp_monitor_index,temp_time_start,temp_time_end);	
			monitor_bado.m_pRecordset=monitor_bado.OpenRecordset(strSql);
			temp_record_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
			if(temp_record_count <= 0)
			{
				analog_data_max_value[i] = 100000;
				analog_data_min_value[i] = 0;
				continue;
			}
			analog_data_point[i] = new Data_Time_Match[temp_record_count];
			memset(analog_data_point[i],0,temp_record_count * sizeof(Data_Time_Match));
			int temp_count = 0;
			_variant_t temp_variant;
			unsigned int logging_time = 0;
			int monitor_value = 0;
			while(VARIANT_FALSE==monitor_bado.m_pRecordset->EndOfFile)
			{
				temp_variant=monitor_bado.m_pRecordset->GetCollect("Record_Time");//
				if(temp_variant.vt!=VT_NULL)
					logging_time = temp_variant;
				else
				{
					logging_time = 0;
					monitor_bado.m_pRecordset->MoveNext();
					continue;
				}

				temp_variant=monitor_bado.m_pRecordset->GetCollect("Monitor_Value");//
				if(temp_variant.vt!=VT_NULL)
					monitor_value = temp_variant;
				else
				{
					monitor_value = 0;
					monitor_bado.m_pRecordset->MoveNext();
					continue;
				}

				analog_data_point[i][temp_count].loggingtime = logging_time;
				analog_data_point[i][temp_count].analogdata = monitor_value;
				if(analog_data_point[i][temp_count].analogdata > analog_data_max_value[i])	//取 最大值和最小值;
					analog_data_max_value[i] = analog_data_point[i][temp_count].analogdata;
				if(analog_data_point[i][temp_count].analogdata < analog_data_min_value[i])
					analog_data_min_value[i] = analog_data_point[i][temp_count].analogdata;

				temp_count ++;
				monitor_bado.m_pRecordset->MoveNext();
			}
			analog_data_count[i] = temp_count;
		}
		else if((i >=temp_number_of_analog) && temp_number_of_digital>0)
		{
			CString strSql;
			strSql.Format(_T("select * from MonitorData where Input_Index = '%s' and SerialNumber = %i and Monitor_Index = %i and Analog_Digital = 0 and Record_Time >= %i and Record_Time <= %i ORDER BY Record_Time ASC"),cs_temp_input_index,temp_serial_number,temp_monitor_index,temp_time_start,temp_time_end);	
			monitor_bado.m_pRecordset=monitor_bado.OpenRecordset(strSql);
			temp_record_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
			if(temp_record_count <= 0)
			{
				continue;
			}

			digital_data_point[i] = new Data_Time_Match[temp_record_count];
			memset(digital_data_point[i],0,temp_record_count * sizeof(Data_Time_Match));
			int temp_count = 0;
			_variant_t temp_variant;
			unsigned int logging_time = 0;
			int monitor_value = 0;
			while(VARIANT_FALSE==monitor_bado.m_pRecordset->EndOfFile)
			{
				temp_variant=monitor_bado.m_pRecordset->GetCollect("Record_Time");//
				if(temp_variant.vt!=VT_NULL)
					logging_time = temp_variant;
				else
				{
					logging_time = 0;
					monitor_bado.m_pRecordset->MoveNext();
					continue;
				}

				temp_variant=monitor_bado.m_pRecordset->GetCollect("Monitor_Value");//
				if(temp_variant.vt!=VT_NULL)
					monitor_value = temp_variant;
				else
				{
					monitor_value = 0;
					monitor_bado.m_pRecordset->MoveNext();
					continue;
				}

				digital_data_point[i][temp_count].loggingtime = logging_time;
				digital_data_point[i][temp_count].analogdata = monitor_value;
				if(digital_data_point[i][temp_count].analogdata != 0)
					digital_data_point[i][temp_count].analogdata = 1;

				temp_count ++;
				monitor_bado.m_pRecordset->MoveNext();
			}
			digital_data_count[i] = temp_count;


		}

	}
	monitor_bado.CloseConn();

	return 0;
}


//获取 或者 创建需要绘制的 点;
void CBacnetGraphic::Create_Line_Point()
{
	//TimeValueToPoint
	//analog_data_point[i]

	for (int i =0 ;i<INPUT_NUMBER;i++)	//Initial all the first point;
	{
		m_pFirstItem[i] = NULL;
	}



	for (int x = 0 ;x<get_data_count;x++)
	{
		if(x < monitor_analog_count)
		{
			pTempItem = NULL;
			pPrevItem = NULL;
			if(analog_data_point[x] == NULL)
				continue;
			for (int i=0 ; i<analog_data_count[x];i++)
			{
				pTempItem = new CPointItem();
				PointF mytemppoint(0,0);
				if(analog_data_point[x] == NULL)
					return;
				TimeValueToPoint(analog_data_point[x][i].loggingtime,analog_data_point[x][i].analogdata,mytemppoint);
				//TRACE(_T("X = %d , Y = %d \r\n"),(int)mytemppoint.X,(int)mytemppoint.Y);
				MyPoint Mypoint={0,0};
				Mypoint.x = mytemppoint.X;
				Mypoint.y = mytemppoint.Y;
				pTempItem->SetPoint(Mypoint);
				if(pPrevItem != NULL)
					pPrevItem->SetNext(pTempItem);
				pPrevItem = pTempItem;

				if(m_pFirstItem[x] == NULL)
				{
					m_pFirstItem[x] = pTempItem;
					m_pFirstItem[x]->SetIndex(0);
				}
			}
		}
		else if((x>= monitor_analog_count) && (monitor_digital_count > 0))
		{
			pTempItem = NULL;
			pPrevItem = NULL;
			if(digital_data_point[x] == NULL)
				continue;
			for (int i=0 ; i<digital_data_count[x];i++)
			{
				pTempItem = new CPointItem();
				PointF mytemppoint(0,0);
				if(digital_data_point[x] == NULL)
					return;
				DigitalTimeValueToPoint(digital_data_point[x][i].loggingtime,digital_data_point[x][i].analogdata,mytemppoint);
				//TRACE(_T("X = %d , Y = %d \r\n"),(int)mytemppoint.X,(int)mytemppoint.Y);
				MyPoint Mypoint={0,0};
				Mypoint.x = mytemppoint.X;
				Mypoint.y = mytemppoint.Y;
				pTempItem->SetPoint(Mypoint);
				if(digital_data_point[x][i].analogdata!=0)
					pTempItem->SetPointValue(1);
				else
					pTempItem->SetPointValue(0);
				if(pPrevItem != NULL)
					pPrevItem->SetNext(pTempItem);
				pPrevItem = pTempItem;

				if(m_pFirstItem[x] == NULL)
				{
					m_pFirstItem[x] = pTempItem;
					m_pFirstItem[x]->SetIndex(0);
				}
			}
		}

	}

	return;
}
volatile HANDLE Point_Mutex=NULL;
DWORD WINAPI MyThreadPro(LPVOID lPvoid)
{
	HWND hWnd = (HWND)lPvoid;
	int nCpuPersent =0;
	int point_number = 0;


	CBacnetGraphic * mparent = (CBacnetGraphic *)lPvoid;

	Point_Mutex=CreateMutex(NULL,TRUE,_T("Point_Mutex_Control")); 
	ReleaseMutex(Point_Mutex); 
	while(flag_continue_thread)
	{
		if(start_wait_init < 5)
			start_wait_init ++ ;
		
		if(!b_has_create_point)
		{
			mparent->Search_data_from_db();

			mparent->Reset_X_Y_Parameter();
			mparent->Create_Line_Point();	//新建需要绘制的点;一直到下次开始刷新 或者定时器刷新时在去获取点;
			b_has_create_point = true;
			draw_graphic_finished = true;
		}
		WaitForSingleObject(Point_Mutex,INFINITE); 
		mparent->MyPaint_Mem(hMemDC);
		ReleaseMutex(Point_Mutex);

		BitBlt(gloab_hdc,0,0, myRect.right, myRect.bottom, hMemDC, 0, 0, SRCCOPY);//将绘制完成的内存位图贴到的Picture空间对象中;
		//PostMessage(test_hwnd, WM_FRESH_STATIC,0,0);	//当重绘了之后，控件会被重绘的画布遮盖住，所以发送消息让控件再次刷新;
		if(start_wait_init < 4)
			Sleep(10);
		else
			Sleep(500);
		//flag_continue_thread = false;
	}
	mythread = NULL;
	return 0;
}


LRESULT CBacnetGraphic::Fresh_Static_Function(WPARAM wParam,LPARAM lParam)
{
	return 0;
}



void CBacnetGraphic::MyPaint_Mem(HDC my_hdc)
{
	Graphics *mygraphics;
	mygraphics = new Graphics(my_hdc);
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	Pen *myRectangle_pen;
	Pen * my_inline_pen;
	Pen * static_write_bord;
	Pen * static_black_bord;

	Pen * CurePen;

	SolidBrush *BlackBrush;
	

	myRectangle_pen = new Pen(Color(255,0,255,255));
	my_inline_pen = new Pen(Color(255,220,220,220));

	static_write_bord = new  Pen(Color(255,255,255,255),2.0f);
	static_black_bord = new  Pen(Color(255,0,0,0),2.0f);


	REAL dashValues[2] = {3, 7};
	//Pen blackPen(Color(255, 0, 0, 0), 5);
	my_inline_pen->SetDashPattern(dashValues, 2);


	CurePen = new Pen(Graphic_Color[1],2.0f);
	PointF      pointF(0, 0);

	BlackBrush =new SolidBrush(Color(255,0,0,0));

	
	mygraphics->FillRectangle(BlackBrush,0,0,window_width,window_hight);
	mygraphics->DrawRectangle(myRectangle_pen,(int)m_analogorignpoint.X,(int)m_analogorignpoint.Y,m_X_ASIX_WIDTH,m_Y_ASIX_HIGHT);

	//mygraphics->FillRectangle(BlackBrush,(int)m_digitalorignpoint.X,(int)m_digitalorignpoint.Y,m_Digital_X_WIDTH,m_Digital_Y_HIGHT  );	//Digital region
	mygraphics->DrawRectangle(myRectangle_pen,(int)m_digitalorignpoint.X,(int)m_digitalorignpoint.Y,m_Digital_X_WIDTH,m_Digital_Y_HIGHT  );

	//mygraphics->FillRectangle(BlackBrush,(int)m_analogorignpoint.X,(int)m_analogorignpoint.Y,m_X_ASIX_WIDTH,m_Y_ASIX_HIGHT);
	//mygraphics->DrawRectangle(myRectangle_pen,(int)m_analogorignpoint.X,(int)m_analogorignpoint.Y,m_X_ASIX_WIDTH,m_Y_ASIX_HIGHT);

	//mygraphics->FillRectangle(BlackBrush,(int)m_digitalorignpoint.X,(int)m_digitalorignpoint.Y,m_Digital_X_WIDTH,m_Digital_Y_HIGHT  );	//Digital region
	//mygraphics->DrawRectangle(myRectangle_pen,(int)m_digitalorignpoint.X,(int)m_digitalorignpoint.Y,m_Digital_X_WIDTH,m_Digital_Y_HIGHT  );


	SolidBrush *Static_blackground_Brush;
	Pen *mystaticRectangle_pen;
	mystaticRectangle_pen = new Pen(Color(255,0,0,0),2.0f);
	Static_blackground_Brush =new SolidBrush(Color(255,187,187,187));	//This part is draw the 14 label and it's background;
	//mygraphics->FillRectangle(Static_blackground_Brush,0,window_hight - 120,window_width,120);
	//mygraphics->DrawRectangle(mystaticRectangle_pen,2,window_hight - 110,window_width-15,110 -30);
	mygraphics->FillRectangle(Static_blackground_Brush,0,window_hight - 145,window_width,135);
	mygraphics->DrawRectangle(mystaticRectangle_pen,2,window_hight - 135,window_width-15,135 -55);

	//****************************************************************************************************
	//画下面的 1 到 E 的lable	;
	//mystaticRectangle_pen = new Pen(Graphic_Color[0],2.0f);
	for (int i=0;i<INPUT_NUMBER;i++)
	{
		CString temp_cs;
		temp_cs = InputLable[i];// _T("AAAAAA");
		if((temp_cs.IsEmpty()) || (StaticShow[i] == false))
		{
			Static_blackground_Brush =new SolidBrush(UNUSE_COLOR);	//This part is draw the 14 label and it's background;
			StaticShow[i] = false;	//如果是由 temp_cs.IsEmpty() 进来的 就顺便把 StaticShow 也设置为false;
		}
		else
			Static_blackground_Brush =new SolidBrush(Graphic_Color[i+1]);	//This part is draw the 14 label and it's background;
		if(i<7)
		{
			RectPosition[i].left = 23 + STATIC_INTERVAL*i;
			RectPosition[i].top = window_hight - 128;
			RectPosition[i].right = 23 + STATIC_INTERVAL*i  + STATIC_LABLE_WIDTH;
			RectPosition[i].bottom = window_hight - 128  + STATIC_LABLE_HIGHT;
			mygraphics->FillRectangle(Static_blackground_Brush,23 + STATIC_INTERVAL*i,window_hight - 128,STATIC_LABLE_WIDTH,STATIC_LABLE_HIGHT);
			mygraphics->DrawLine(static_write_bord,23 + STATIC_INTERVAL*i - 2,window_hight - 128 -2,23 + STATIC_INTERVAL*i  + STATIC_LABLE_WIDTH,window_hight - 128 -2);
			mygraphics->DrawLine(static_write_bord,23 + STATIC_INTERVAL*i - 2,window_hight - 128 -2,23 + STATIC_INTERVAL*i - 2,window_hight - 128  + STATIC_LABLE_HIGHT);
			mygraphics->DrawLine(static_black_bord,23 + STATIC_INTERVAL*i ,window_hight - 128  + STATIC_LABLE_HIGHT ,23 + STATIC_INTERVAL*i  + STATIC_LABLE_WIDTH,window_hight - 128  + STATIC_LABLE_HIGHT);
			mygraphics->DrawLine(static_black_bord,23 + STATIC_INTERVAL*i  + STATIC_LABLE_WIDTH ,window_hight - 128,23 + STATIC_INTERVAL*i  + STATIC_LABLE_WIDTH,window_hight - 128  + STATIC_LABLE_HIGHT);
		}
		else
		{
			RectPosition[i].left = 23 + STATIC_INTERVAL*(i-7);
			RectPosition[i].top = window_hight - 128 + 40;
			RectPosition[i].right = 23 + STATIC_INTERVAL*(i-7)  + STATIC_LABLE_WIDTH;
			RectPosition[i].bottom = window_hight - 128  + STATIC_LABLE_HIGHT + 40;
			mygraphics->FillRectangle(Static_blackground_Brush,23 + STATIC_INTERVAL*(i-7),window_hight - 128 + 40,STATIC_LABLE_WIDTH,STATIC_LABLE_HIGHT);
			mygraphics->DrawLine(static_write_bord,23 + STATIC_INTERVAL*(i-7) - 2,window_hight - 128 -2 + 40,23 + STATIC_INTERVAL*(i-7)  + STATIC_LABLE_WIDTH,window_hight - 128 -2 + 40);
			mygraphics->DrawLine(static_write_bord,23 + STATIC_INTERVAL*(i-7) - 2,window_hight - 128 -2 + 40,23 + STATIC_INTERVAL*(i-7) - 2,window_hight - 128  + STATIC_LABLE_HIGHT + 40);
			mygraphics->DrawLine(static_black_bord,23 + STATIC_INTERVAL*(i-7) ,window_hight - 128  + STATIC_LABLE_HIGHT + 40,23 + STATIC_INTERVAL*(i-7)  + STATIC_LABLE_WIDTH,window_hight - 128  + STATIC_LABLE_HIGHT + 40);
			mygraphics->DrawLine(static_black_bord,23 + STATIC_INTERVAL*(i-7)  + STATIC_LABLE_WIDTH ,window_hight - 128 + 40,23 + STATIC_INTERVAL*(i-7)  + STATIC_LABLE_WIDTH,window_hight - 128  + STATIC_LABLE_HIGHT + 40);
		}
		

		SolidBrush  Font_brush(STATIC_FONT_COLOR);
		
		//FontFamily  StaticfontFamily(_T("Times New Roman"));
		FontFamily  StaticfontFamily(_T("Arial"));
		
		Gdiplus::Font        Input_font(&StaticfontFamily, 20, FontStyleRegular, UnitPixel);
		
		PointF      staticpointF(0, 0);
		if(i<7)
		{
		staticpointF.X = STATIC_LABLE_HIGHT + STATIC_INTERVAL*i;
		staticpointF.Y = 742 ;
		}
		else
		{
			staticpointF.X = STATIC_LABLE_HIGHT + STATIC_INTERVAL*(i-7);
			staticpointF.Y = 742 + 40 ;
		}
		mygraphics->DrawString(temp_cs, -1, &Input_font, staticpointF,&Font_brush);




		//************************************************************************************
		//这些是画 Static 后面的 数字;
		CString temp_item;
		temp_item.Format(_T("%x"),i+1);
		temp_item = temp_item.MakeUpper();

		SolidBrush  static_item_brush(Graphic_Color[i+1]);
		FontFamily  UnitfontFamily(_T("Arial"));
		Gdiplus::Font        unitfont(&UnitfontFamily, 22, FontStyleRegular, UnitPixel);
		if((i+1)<8)
		{
			pointF.X = 25 + STATIC_INTERVAL*i + STATIC_LABLE_WIDTH + 5;
			pointF.Y = 737;
		}
		else
		{
			pointF.X = 25 + STATIC_INTERVAL*(i-7) + STATIC_LABLE_WIDTH + 5;
			pointF.Y = 737 + 40;
		}
		mygraphics->DrawString(temp_item, -1, &unitfont, pointF, &static_item_brush);
		//************************************************************************************
		if(Static_blackground_Brush)
			delete Static_blackground_Brush;
		Static_blackground_Brush = NULL;
	}
	//****************************************************************************************************





	SolidBrush  time_brush(Color(255, 225, 225, 225));
	//FontFamily  fontFamily(_T("Times New Roman"));
	FontFamily  fontFamily(_T("Arial"));
	
	Gdiplus::Font        time_font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	for(int i=0;i<m_xscale + 1;i++)				//画网格线
	{
		mygraphics->DrawLine(my_inline_pen,(int)m_analogorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
											(int)m_analogorignpoint.Y,
											 (int)m_analogorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
											 (int)m_analogorignpoint.Y + m_Y_ASIX_HIGHT);

		mygraphics->DrawLine(my_inline_pen,(int)m_digitalorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
			(int)m_digitalorignpoint.Y,
			(int)m_digitalorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
			(int)m_digitalorignpoint.Y + m_Digital_Y_HIGHT);


		CString strTime ;
		wchar_t temp_char[200];
		time_t scale_time ;
		CTime time_scaletime;
		scale_time = m_starttime +  i*(x_axis_total_time / m_xscale);
		time_scaletime = scale_time;
		if((m_time_selected <= TIME_ONE_HOUR) && (m_time_selected >= TIME_ONE_MINUTE))
			strTime = time_scaletime.Format("  %m/%d \r\n%H:%M:%S");
		else
			strTime = time_scaletime.Format("%m/%d %H:%M");

		pointF.X = (int)m_analogorignpoint.X - 30 + i*(m_X_ASIX_WIDTH/m_xscale);
		pointF.Y = (int)m_analogorignpoint.Y + m_Y_ASIX_HIGHT + 5;
		mygraphics->DrawString(strTime, -1, &time_font, pointF, &time_brush);
	}

	SolidBrush  unit_brush(Color(255,255,255,255));
	//FontFamily  UnitfontFamily(_T("Times New Roman"));
	FontFamily  UnitfontFamily(_T("Arial"));
	Gdiplus::Font        unitfont(&UnitfontFamily, 18, FontStyleRegular, UnitPixel);
	for(int i=0;i<=m_yscale;i++)				//画网格线
	{
		CString Unit_value;
		if(i!=m_yscale)
		{
			mygraphics->DrawLine(my_inline_pen,(int)m_analogorignpoint.X,
								(int)m_analogorignpoint.Y+(m_Y_ASIX_HIGHT/m_yscale)*(1+i),
								 (int)m_analogorignpoint.X + m_X_ASIX_WIDTH ,
								 (int)m_analogorignpoint.Y + (m_Y_ASIX_HIGHT/m_yscale)*(1+i));



		}


		if(i!=m_yscale)
			Unit_value.Format(_T("%d"),(y_axis_total_value/m_yscale)*(m_yscale-i) + m_lowvalue);// = timeTest.Format("%Y/%m/%d\r\n %H:%M:%S");
		else
			Unit_value.Format(_T("%d"),m_lowvalue);
		pointF.X =  (int)m_analogorignpoint.X - 35;
		pointF.Y =  (int)m_analogorignpoint.Y+ i*(m_Y_ASIX_HIGHT/m_yscale) - 8;
		mygraphics->DrawString(Unit_value, -1, &unitfont, pointF, &unit_brush);
		//swprintf_s(temp_char,200,L"%d",i*5);
		//mygraphics->DrawString(temp_char, -1, &font, pointF, &brush);

	}
	//****************************************************************
	//画 Digital  的 横着的 两条线内 网格;
	mygraphics->DrawLine(my_inline_pen,(int)m_digitalorignpoint.X,
		(int)m_digitalorignpoint.Y+ 30,
		(int)m_digitalorignpoint.X + m_Digital_X_WIDTH ,
		(int)m_digitalorignpoint.Y + 30);

	mygraphics->DrawLine(my_inline_pen,(int)m_digitalorignpoint.X,
		(int)m_digitalorignpoint.Y+ 110,
		(int)m_digitalorignpoint.X + m_Digital_X_WIDTH ,
		(int)m_digitalorignpoint.Y + 110);
	//****************************************************************

//	CPointItem *first_item=NULL,*second_item=NULL,*third_item=NULL;
	CPointItem *first_item[INPUT_NUMBER];
	CPointItem *second_item[INPUT_NUMBER];
	Pen * DrawLinePen[INPUT_NUMBER];
	for (int i=0;i<INPUT_NUMBER;i++)	//Initial all the PointItems;
	{
		first_item[i] = NULL;
		second_item[i] = NULL;
		DrawLinePen[i] = NULL;
	}

	for (int i=0;i<get_data_count;i++)
	{
		if(!StaticShow[i])
			continue;
		first_item[i] = m_pFirstItem[i];
		DrawLinePen[i] = new Pen(Graphic_Color[i + 1],2.0f);
		if(i<monitor_analog_count)
		{
			if(first_item[i] != NULL)
			{
				do 
				{
					second_item[i]=first_item[i]->GetNext();
					if(second_item[i]==NULL)
						break;
					mygraphics->DrawLine(DrawLinePen[i],first_item[i]->GetPoint().x,first_item[i]->GetPoint().y,second_item[i]->GetPoint().x,second_item[i]->GetPoint().y);

					first_item[i] = second_item[i];
				} while ((second_item[i]->GetNext())!=NULL);
			}
		}
		else if(monitor_digital_count > 0)
		{
			if(first_item[i] != NULL)
			{
				do 
				{
					second_item[i]=first_item[i]->GetNext();
					if(second_item[i]==NULL)
					{
						mygraphics->DrawLine(DrawLinePen[i],first_item[i]->GetPoint().x,first_item[i]->GetPoint().y,(int)m_digitalorignpoint.X + m_Digital_X_WIDTH,first_item[i]->GetPoint().y);
						break;
					}
					//如果前面一个数字点和后面一个数字点的 值不一样 就要画  延变化;
					if(second_item[i]->GetPointValue() == first_item[i]->GetPointValue())
						mygraphics->DrawLine(DrawLinePen[i],first_item[i]->GetPoint().x,first_item[i]->GetPoint().y,second_item[i]->GetPoint().x,second_item[i]->GetPoint().y);
					else
					{
						mygraphics->DrawLine(DrawLinePen[i],first_item[i]->GetPoint().x,first_item[i]->GetPoint().y,second_item[i]->GetPoint().x,first_item[i]->GetPoint().y);
						mygraphics->DrawLine(DrawLinePen[i],second_item[i]->GetPoint().x,first_item[i]->GetPoint().y,second_item[i]->GetPoint().x,second_item[i]->GetPoint().y);
					}
					

					first_item[i] = second_item[i];
				} while (first_item[i]!=NULL);
				//while ((second_item[i]->GetNext())!=NULL);
			}
		}

		
		
		
		mygraphics->DrawLine(DrawLinePen[i],20,50 + i*30,60,50 + i*30);


		SolidBrush *   pen_unit_brush = new SolidBrush(Graphic_Color[i+1]);
		pointF.X =  70;
		pointF.Y =  40 + i*30;

		mygraphics->DrawString(InputUnit[i], -1, &unitfont, pointF, pen_unit_brush);
		delete pen_unit_brush;
		pen_unit_brush = NULL;
	}



	for (int i=0;i<INPUT_NUMBER;i++)
	{
		if(DrawLinePen[i] != NULL)
		{
			delete DrawLinePen[i];
			DrawLinePen[i] = NULL;
		}
	}

	delete CurePen;
	delete mygraphics;
	delete myRectangle_pen;
	delete my_inline_pen;
	delete BlackBrush;

	delete static_write_bord;
	delete static_black_bord;
}






void CBacnetGraphic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnTimer(nIDEvent);
}







BOOL CBacnetGraphic::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetGraphic::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
//	PostMessage(WM_CLOSE,NULL,NULL);
	//GraphicWindow = NULL;
	//::PostMessage(m_monitor_dlg_hwnd,WM_MONITOR_USER_MESSAGE,MONITOR_MESSAGE_DELETE,0);
	//CDialogEx::OnCancel();
	DestroyWindow();//窗口的销毁        //CDialog::OnCancel();//屏蔽基类的OnCancel,因为基类的OnCancel调用了EndDialog函数，这是模态Dialog的。   
}


void CBacnetGraphic::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}

//计算点击的坐标是否在某个  自己绘制的STATIC 里面 ，如果在里面就设置是否变色 以及 显示;
void CBacnetGraphic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int click_item = -1;
	for (int i=0;i<14;i++)
	{
		if((point.x > RectPosition[i].left) &&
			(point.x < RectPosition[i].right) &&
			(point.y > RectPosition[i].top) &&
			(point.y < RectPosition[i].bottom))
		{
			start_wait_init = 0;
			click_item = i;
			if(StaticShow[i] == true)
				StaticShow[i] = false;
			else
				StaticShow[i] = true;
			Delete_Ram_Data();
			break;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CBacnetGraphic::SetXaxisTime(unsigned long nstarttime,unsigned long nendtime)
{
	m_starttime = nstarttime;
	m_endtime = nendtime;
}

void CBacnetGraphic::SetXaxisStartTime(unsigned long nstarttime)
{
	m_starttime = nstarttime;
	m_endtime = nstarttime + x_axis_total_time;
}

void CBacnetGraphic::SetYaxisValue(int nlowvalue,int nhighvalue)
{
	if(nhighvalue <= nlowvalue)
	{
		nhighvalue = 100;
		nlowvalue = 0;
	}
	m_lowvalue = nlowvalue;
	m_highvalue = nhighvalue;
	y_axis_total_value = nhighvalue - nlowvalue;
}

void CBacnetGraphic::GetXaxisTime(unsigned long &returnstarttime,unsigned long &returnendtime)
{
	ASSERT(returnendtime > returnstarttime);
	returnstarttime = m_starttime;
	returnendtime = m_endtime;
}

void CBacnetGraphic::GetYaxisValue(int &returnlowvalue,int &returnhighvalue)
{
	ASSERT(returnhighvalue > returnlowvalue);
	returnlowvalue = m_lowvalue;
	returnhighvalue = m_highvalue;
}

void CBacnetGraphic::SetXaxisScale(int nxscale)
{
	m_xscale = nxscale;
}

void CBacnetGraphic::SetYaxisScale(int nyscale)
{
	m_yscale = nyscale;
}

int CBacnetGraphic::GetXaxisScale()
{
	return m_xscale;
}

int CBacnetGraphic::GetYaxisScale()
{
	return m_yscale;
}

void CBacnetGraphic::SetDigitalOrignPoint(PointF npoint)
{
	m_digitalorignpoint.X = npoint.X;
	m_digitalorignpoint.Y = npoint.Y;
}

void CBacnetGraphic::GetDigitalOrignPoint(PointF &returnpoint)
{
	returnpoint.X = m_digitalorignpoint.X;
	returnpoint.Y = m_digitalorignpoint.Y;
}


void CBacnetGraphic::SetAnalogOrignPoint(PointF npoint)
{
	m_analogorignpoint.X = npoint.X;
	m_analogorignpoint.Y = npoint.Y;
}

void CBacnetGraphic::GetAnalogOrignPoint(PointF &returnpoint)
{
	returnpoint.X = m_analogorignpoint.X;
	returnpoint.Y = m_analogorignpoint.Y;
}

//计算一个像素点 Y轴 所对应 的 值 是多少;
void CBacnetGraphic::CalcOnePixelValue()
{
	m_onepiexlvalue = (float)y_axis_total_value / (float)m_Y_ASIX_HIGHT;
}

void CBacnetGraphic::Set_Time_Scale(int n_time_scale)
{
	m_time_selected = n_time_scale;
}

int CBacnetGraphic::Get_Time_Scale()
{
	return m_time_selected ;
}
void CBacnetGraphic::CalcOnePixelTime()
{
	switch(m_time_selected)
	{
	case TIME_ONE_MINUTE :
		{
			x_axis_total_time = 60;
		}
		break;
	case TIME_TEN_MINUTE:
		{
			x_axis_total_time = 600;
		}
		break;
	case TIME_ONE_HOUR:
		{
			x_axis_total_time = 3600;
		}
		break;
	case TIME_FOUR_HOUR:
		{
			x_axis_total_time = 3600 * 4;
		}
		break;
	case TIME_TWELVE_HOUR:
		{
			x_axis_total_time = 3600 * 12;
		}
		break;
	case TIME_ONE_DAY:
		{
			x_axis_total_time = 3600 * 24;	
		}
		break;
	case TIME_FOUR_DAY:
		{
			x_axis_total_time = 3600 * 24 * 4;
		}
		break;
	default:
		break;
	}
	m_onepixeltime =  (float)(x_axis_total_time) / (float)m_X_ASIX_WIDTH;
}

bool CBacnetGraphic::DigitalTimeValueToPoint(unsigned long ntime , int nvalue ,PointF &returnpoint)
{
	if(nvalue != 0)
	{
		nvalue = 1;
		returnpoint.Y =m_digitalorignpoint.Y + 30;
	}
	else
		returnpoint.Y = m_digitalorignpoint.Y  + 110;

	long delta_time = ntime - m_starttime;
	if((delta_time <0) ||(delta_time > x_axis_total_time))
	{

		returnpoint.X = last_used_point.X;
		returnpoint.Y = last_used_point.Y;
		return false;
	}

	returnpoint.X =m_analogorignpoint.X + (delta_time  / m_onepixeltime);
	//returnpoint.Y = m_analogorignpoint.Y + (m_Y_ASIX_HIGHT - (delta_value / m_onepiexlvalue));
	last_used_point.X = returnpoint.X;
	last_used_point.Y = returnpoint.Y ;
	return true;
}



bool CBacnetGraphic::TimeValueToPoint(unsigned long ntime , int nvalue ,PointF &returnpoint)
{
	//Calculate X axis point
//	ntime = ntime + 10150;//Test

	//ntime =ntime + 3000;//Test

	nvalue = nvalue / 1000;
	//TRACE(_T("value = %d\r\n"),nvalue);
	long delta_time = ntime - m_starttime;
	if((delta_time <0) ||(delta_time > x_axis_total_time))
	{
		
		returnpoint.X = last_used_point.X;
		returnpoint.Y = last_used_point.Y;
		return false;
	}
	
	//Calculate Y axis point
	int delta_value = nvalue - m_lowvalue;
	if((delta_value < 0) ||(delta_value > y_axis_total_value))
	{
		returnpoint.X = last_used_point.X;
		returnpoint.Y = last_used_point.Y;
		return false;
	}

	returnpoint.X =m_analogorignpoint.X + (delta_time  / m_onepixeltime);
	returnpoint.Y = m_analogorignpoint.Y + (m_Y_ASIX_HIGHT - (delta_value / m_onepiexlvalue));
	last_used_point.X = returnpoint.X;
	last_used_point.Y = returnpoint.Y ;
	return true;
}

void CBacnetGraphic::SetDigital_Y_Hight(int n_dig_hight)
{
	m_Digital_Y_HIGHT = n_dig_hight;
}

int CBacnetGraphic::GetDigital_Y_Hight()
{
	return m_Digital_Y_HIGHT;
}

void	CBacnetGraphic::SetDigital_X_WIDTH(int n_dig_x_width)
{
	m_Digital_X_WIDTH = n_dig_x_width;
}

int	CBacnetGraphic::GetDigital_X_WIDTH()
{
	return m_Digital_X_WIDTH ;
}

void CBacnetGraphic::Set_XAxis_Length(int n_x_with)
{
	m_X_ASIX_WIDTH = n_x_with;
}

int CBacnetGraphic::Get_XAxis_Length()
{
	return m_X_ASIX_WIDTH;
	
}

void CBacnetGraphic::Set_YAxis_Length(int n_y_high)
{
	m_Y_ASIX_HIGHT = n_y_high;
}

int CBacnetGraphic::Get_YAxis_Length()
{
	return m_Y_ASIX_HIGHT;
}


void CBacnetGraphic::InitialParameter(int base_time,int y_min_value,int y_max_value)
{
	bool contain_digital = false;
	for (int i=monitor_analog_count;i<get_data_count;i++)
	{
		if(StaticShow[i])
		{
			contain_digital = true;
			break;
		}
	}
	if((monitor_digital_count == 0)  || (contain_digital == false))
	{
		Set_XAxis_Length(1200);
		Set_YAxis_Length(650);
		SetDigital_X_WIDTH(0);
		SetDigital_Y_Hight(0);
	}
	else
	{
		Set_XAxis_Length(1200);
		Set_YAxis_Length(500);
		SetDigital_X_WIDTH(1200);
		SetDigital_Y_Hight(150);
	}
	SetXaxisScale(6);
	SetYaxisScale(4);
	SetAnalogOrignPoint(PointF(200,30));
	SetDigitalOrignPoint(PointF(200,560));
	Set_Time_Scale(base_time);

	//Set_XAxis_Length(1200);
	//Set_YAxis_Length(500);
	//SetDigital_X_WIDTH(1200);
	//SetDigital_Y_Hight(150);
	//SetXaxisScale(6);
	//SetYaxisScale(4);
	//SetAnalogOrignPoint(PointF(200,30));
	//SetDigitalOrignPoint(PointF(200,560));
	//Set_Time_Scale(base_time);


	int temp_min = y_min_value /1000 ;
	int temp_max = y_max_value /1000 ;

	if((temp_min<10) && (temp_min >= 0))
		temp_min = 0;
	else if((temp_min > -10) && (temp_min < 0))
	{
		temp_min = -10;
	}
	else if((temp_min >= -100) && (temp_min <= -10))
	{
		temp_min = temp_min / 10 - 1;
		temp_min = temp_min * 10;
	}
	else if((temp_min >= -1000) && (temp_min <= -100))
	{
		temp_min = temp_min / 100 - 1;
		temp_min = temp_min * 100;
	}
	else if((temp_min >= -10000) && (temp_min <= -1000))
	{
		temp_min = temp_min / 1000 - 1;
		temp_min = temp_min * 1000;
	}
	else if((temp_min >= -100000) && (temp_min <= -10000))
	{
		temp_min = temp_min / 10000 - 1;
		temp_min = temp_min * 10000;
	}
	else if((temp_min>=10) && (temp_min <= 100))
	{
		temp_min = temp_min / 10;
		temp_min = temp_min * 10;
	}
	else if((temp_min>=100) && (temp_min <= 1000))
	{
		temp_min = temp_min / 100;
		temp_min = temp_min * 100;
	}
	else if((temp_min>=1000) && (temp_min <= 10000))
	{
		temp_min = temp_min / 1000;
		temp_min = temp_min * 1000;
	}
	else if((temp_min>=10000) && (temp_min <= 100000))
	{
		temp_min = temp_min / 10000;
		temp_min = temp_min * 10000;
	}
	else if((temp_min>=100000) && (temp_min <= 1000000))
	{
		temp_min = temp_min / 100000;
		temp_min = temp_min * 100000;
	}


	if((temp_max < 10) && (temp_max >=0))
		temp_max = 10;
	else if((temp_max >=10) && (temp_max <= 100))
	{
		temp_max = temp_max / 10 + 1;
		temp_max = temp_max * 10;
	}
	else if((temp_max >=100) && (temp_max <= 1000))
	{
		temp_max = temp_max / 100 + 1;
		temp_max = temp_max * 100;
	}
	else if((temp_max >=1000) && (temp_max <= 10000))
	{
		temp_max = temp_max / 1000 + 1;
		temp_max = temp_max * 1000;
	}
	else if((temp_max >=10000) && (temp_max <= 100000))
	{
		temp_max = temp_max / 10000 + 1;
		temp_max = temp_max * 10000;
	}
	else if((temp_max >=100000) && (temp_max <= 1000000))
	{
		temp_max = temp_max / 100000 + 1;
		temp_max = temp_max * 100000;
	}
	else if((temp_max>=-10) && (temp_max <= - 100))
	{
		temp_max = temp_max / 10 ;
		temp_max = temp_max * 10;
	}
	else if((temp_max>=-100) && (temp_max <= - 1000))
	{
		temp_max = temp_max / 100 ;
		temp_max = temp_max * 100;
	}
	else if((temp_max>=-1000) && (temp_max <= - 10000))
	{
		temp_max = temp_max / 1000 ;
		temp_max = temp_max * 1000;
	}
	else if((temp_max>=-10000) && (temp_max <= - 100000))
	{
		temp_max = temp_max / 10000 ;
		temp_max = temp_max * 10000;
	}
	else if((temp_max>=-100000) && (temp_max <= - 1000000))
	{
		temp_max = temp_max / 100000 ;
		temp_max = temp_max * 100000;
	}
	SetYaxisValue( temp_min ,temp_max );//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SetYaxisValue( (y_min_value* 8)/10000 , (y_max_value * 12)/10000);//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SetYaxisValue( y_min_value/1000 - temp_all*0.2 , y_max_value /1000 + temp_all*0.2);//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CalcOnePixelTime();
	CalcOnePixelValue();
	timestart = m_time_monitor_now - x_axis_total_time;
	SetXaxisStartTime(timestart);

}


//void CBacnetGraphic::OnNcDestroy()
//{
//	CDialogEx::OnNcDestroy();
//
//	
//	// TODO: Add your message handler code here
//}


void CBacnetGraphic::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	flag_continue_thread = false;
	Sleep(200);
	TerminateThread(mythread,2);
	mythread = NULL;
	CDialogEx::PostNcDestroy();
	delete this;//类对象的销毁   ;
	GraphicWindow = NULL;

	Delete_Ram_Data();
}

void CBacnetGraphic::Delete_Ram_Data()
{
	WaitForSingleObject(Point_Mutex,INFINITE);	//防止删除数据的时候  绘图的线程又调用此数据;加锁保护;
	//for (int i=0;i<10;i++)
	//{
	//	Sleep(1000);
	//	TRACE(_T("Sleep %d\r\n"),10-i);
	//}
	for (int i=0;i< INPUT_NUMBER ;i++)
	{
		if(analog_data_point[i] !=NULL)
		{
			delete analog_data_point[i];
			analog_data_point[i] = NULL;
		}
		if(digital_data_point[i] !=NULL)
		{
			delete digital_data_point[i];
			digital_data_point[i] = NULL;
		}
		digital_data_count[i] = 0;
		analog_data_count[i] = 0;
	}
	for (int i=0;i<INPUT_NUMBER;i++)
	{
		if(m_pFirstItem[i] != NULL)
		{

			while(m_pFirstItem[i]->GetNext() !=NULL)
			{
				CPointItem * temp_item = m_pFirstItem[i]->GetNext();
				delete m_pFirstItem[i];
				m_pFirstItem[i] = temp_item;
			}

		}
		m_pFirstItem[i] = NULL ;
	}
	b_has_create_point = false;
	ReleaseMutex(Point_Mutex);

}


void CBacnetGraphic::OnTimebase1hour()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_selected = TIME_ONE_HOUR;
	Delete_Ram_Data();

	
}


void CBacnetGraphic::OnTimebase1day()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	m_time_selected = TIME_ONE_DAY;
	draw_graphic_finished = false;
	
	//InitialParameter(TIME_ONE_DAY);
	Delete_Ram_Data();
}


void CBacnetGraphic::OnTimebase1minites()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_selected = TIME_ONE_MINUTE;
	//InitialParameter(TIME_ONE_MINUTE);
	Delete_Ram_Data();
}


void CBacnetGraphic::OnTimebase10minutes()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_selected = TIME_TEN_MINUTE;
	//InitialParameter(TIME_TEN_MINUTE);
	Delete_Ram_Data();
}


void CBacnetGraphic::OnTimebase12hours()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_selected = TIME_TWELVE_HOUR;
	//InitialParameter(TIME_TWELVE_HOUR);
	Delete_Ram_Data();
}


void CBacnetGraphic::OnTimebase4hours()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_selected = TIME_FOUR_HOUR;
	//InitialParameter(TIME_FOUR_HOUR);
	Delete_Ram_Data();
}


void CBacnetGraphic::OnTimebase4days()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_selected = TIME_FOUR_DAY;
	//InitialParameter(TIME_FOUR_DAY);
	Delete_Ram_Data();
}
void CBacnetGraphic::Reset_X_Y_Parameter()
{
	total_y_max_value = 0x80000001;
	total_y_min_value = 0x7fffffff;
	for (int i=0;i<get_data_count;i++)
	{
		if(i<monitor_analog_count)
		{
			if(StaticShow[i])
			{
				if(total_y_max_value < analog_data_max_value[i])
					total_y_max_value = analog_data_max_value[i];
				if(total_y_min_value > analog_data_min_value[i])
					total_y_min_value = analog_data_min_value[i];
			}
		}

		//int total_y_max_value = 0;
		//int total_y_min_value = 0;
	}
	switch(m_time_selected)
	{
	case TIME_ONE_MINUTE :
		{
			InitialParameter(TIME_ONE_MINUTE,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_TEN_MINUTE:
		{
			InitialParameter(TIME_TEN_MINUTE,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_ONE_HOUR:
		{
			InitialParameter(TIME_ONE_HOUR,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_FOUR_HOUR:
		{
			InitialParameter(TIME_FOUR_HOUR,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_TWELVE_HOUR:
		{
			InitialParameter(TIME_TWELVE_HOUR,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_ONE_DAY:
		{
			InitialParameter(TIME_ONE_DAY,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_FOUR_DAY:
		{
			InitialParameter(TIME_FOUR_DAY,total_y_min_value,total_y_max_value);
		}
		break;
	default:
		InitialParameter(TIME_FOUR_HOUR,0,100);
		break;
	}


}

void CBacnetGraphic::Reset_X_Time_Parameter()
{
	return;
	switch(m_time_selected)
	{
	case TIME_ONE_MINUTE :
		{
			InitialParameter(TIME_ONE_MINUTE);
		}
		break;
	case TIME_TEN_MINUTE:
		{
			InitialParameter(TIME_TEN_MINUTE);
		}
		break;
	case TIME_ONE_HOUR:
		{
			InitialParameter(TIME_ONE_HOUR);
		}
		break;
	case TIME_FOUR_HOUR:
		{
			InitialParameter(TIME_FOUR_HOUR);
		}
		break;
	case TIME_TWELVE_HOUR:
		{
			InitialParameter(TIME_TWELVE_HOUR);
		}
		break;
	case TIME_ONE_DAY:
		{
			InitialParameter(TIME_ONE_DAY);
		}
		break;
	case TIME_FOUR_DAY:
		{
			InitialParameter(TIME_FOUR_DAY);
		}
		break;
	default:
		InitialParameter(TIME_FOUR_HOUR);
		break;
	}
}

void CBacnetGraphic::OnGraphicLeft()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_monitor_now =m_time_monitor_now -  x_axis_total_time;

	//Reset_X_Time_Parameter();
		Delete_Ram_Data();

}


void CBacnetGraphic::OnGraphicRight()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	m_time_monitor_now =m_time_monitor_now +  x_axis_total_time;

//	Reset_X_Time_Parameter();
		Delete_Ram_Data();
}


void CBacnetGraphic::OnZoomin()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	if(m_time_selected > TIME_ONE_MINUTE)
	{
		m_time_selected = m_time_selected - 1;
		//Reset_X_Time_Parameter();
		Delete_Ram_Data();
	}
	else
		m_time_selected = 1;

}


void CBacnetGraphic::OnZoomout()
{
	// TODO: Add your command handler code here
	if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	{
		return;
	}
	draw_graphic_finished = false;
	if(m_time_selected < TIME_FOUR_DAY)
	{
		m_time_selected = m_time_selected + 1;
		//Reset_X_Time_Parameter();
		Delete_Ram_Data();
	}
	else
		m_time_selected = TIME_FOUR_DAY;
}

void CBacnetGraphic::Get_Input_Unit()
{
	int inputs_count = m_monitor_data.at(monitor_list_line).num_inputs;
	int analog_count = m_monitor_data.at(monitor_list_line).an_inputs;
	for (int i=0;i<inputs_count;i++)
	{
		 int temp_range = m_monitor_data.at(monitor_list_line).range[i];
		if(i<analog_count)
		{
			 if( m_monitor_data.at(monitor_list_line).inputs[i].point_type == ENUM_OUT + 1)
			 {
				 if(temp_range < (sizeof(Output_Analog_Units_Show)/sizeof(Output_Analog_Units_Show[0])))
					InputUnit[i] =	Output_Analog_Units_Show[temp_range];
				 else
					InputUnit[i].Empty();
			 }
			 else if(m_monitor_data.at(monitor_list_line).inputs[i].point_type == ENUM_IN + 1)
			 {
				 if(temp_range < (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
					 InputUnit[i] =	Input_List_Analog_Units[temp_range];
				 else
					 InputUnit[i].Empty();
			 }
			 else if(m_monitor_data.at(monitor_list_line).inputs[i].point_type == ENUM_VAR + 1)
			 {
				 if(temp_range < (sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0])))
					 InputUnit[i] =	Variable_Analog_Units_Array[temp_range];
				 else
					 InputUnit[i].Empty();
			 }
			 else
			 {
				 InputUnit[i].Empty();
			 }
		}
		else if((i>= analog_count) && (i<inputs_count))
		{
			if(temp_range < (sizeof(Digital_Units_Array)/sizeof(Digital_Units_Array[0])))
				InputUnit[i] =	Digital_Units_Array[temp_range];
			else
				InputUnit[i].Empty();
		}
		
	}
}


BOOL CBacnetGraphic::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 
			HH_HELP_CONTEXT, IDH_TOPIC_GRAPHIC_SCREENS);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}