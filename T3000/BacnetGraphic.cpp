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
#include "global_function.h"
#include "BacnetMonitor.h"
#include "BADO/BADO.h"
#include "BacnetMessageInput.h"
#include "BacnetGraphicSetting.h"
#define  WM_MONITOR_USER_MESSAGE WM_USER + 902

void BitToString(int digtal_or_analog, int nIndex);
CString InputLable[15];
CString InputUnit[15];
bool StaticShow[15];
int total_y_max_value = 0x80000001;
int total_y_min_value = 0x7fffffff;

#define  INPUT_NUMBER  14

CPointItem * pTempItem = NULL;
CPointItem *pPrevItem = NULL;
CPointItem	*m_pFirstItem[INPUT_NUMBER];

unsigned int time_interval_point = 0;
unsigned int customer_define_x_time = 3600;	// 客户定义的 x 的 长度; 
unsigned long customer_start_time = 0;
unsigned long customer_end_time = 3600;
bool use_customer_time = false;
int no_space_auto_close = 0;
bool contain_digital = false; //用于判断是否包含数字量;
bool draw_graphic_finished = false;
//extern unsigned char read_monitor_sd_ret;

RECT RectPosition[15];
static bool b_has_create_point = false ;
int start_wait_init = 0;	//用于控制显示刷新的变量;
extern CBacnetGraphic * GraphicWindow;
//CRect Static_Num_Rect[15];
bool flag_continue_thread = true;
bool b_show_value_line = false;

HWND myhWnd;
RECT myRect;
HANDLE mythread =NULL;
HANDLE updatedatathread = NULL;
HANDLE refreshthread = NULL;
HDC hMemDC;
HBITMAP hBmp;
HDC gloab_hdc;

int scale_type;
DWORD WINAPI MyThreadPro(LPVOID lPvoid);
DWORD WINAPI UpdateDataThreadPro(LPVOID lPvoid);
DWORD WINAPI RefreshThreadPro(LPVOID lPvoid);
int point_error_2 = 0;
PointF      RclickValueTime(0, 0);
extern int ncontinue_read_data ; //如果变更了刻度或进度条，就退出之前正在读的循环,需要读新的刻度;

//#define  MY_COLOR_BACKGRAND SolidBrush(Color(255,192,192,192))
//#define  MY_COLOR_AUTOSCROLL			Color(255,255,255,0)
//#define  MY_COLOR_ON_OFF				Color(255,255,0,0)
//#define  MY_COLOR_14LABLE_BGD			Color(255,255,255,255)
//#define  MY_COLOR_14BGD					Color(255,187,187,187)
//#define  MY_COLOR_14_WRITE_BORD  Color(255,255,255,255)
//#define  MY_COLOR_14_BLACK_BORD  Color(255,0,0,0)
//#define  MY_COLOR_PEN					Color(255,0,0,0)
//#define  MY_COLOR_TIME_PEN				Color(255, 225, 225, 225)
//#define  MY_COLOR_PEN_RECTANGLE_BORD    Color(255,0,255,255)
//#define  MY_COLOR_PEN_INLINE_PEN        Color(255,220,220,220)
//#define  MY_COLOR_UNIT_PEN              Color(255,255,255,255)
#define  MY_COLOR_VIEW123				Color(255,0,0,0)
#define  MY_COLOR_BACKGRAND				Color(255,192,192,192)
#define  MY_COLOR_AUTOSCROLL			Color(255,255,255,0)
#define  MY_COLOR_ON_OFF				Color(255,255,0,0)
#define  MY_COLOR_14LABLE_BGD			Color(255,255,255,255)
#define  MY_COLOR_14BGD					Color(255,187,187,187)
#define  MY_COLOR_14_WRITE_BORD			Color(255,255,255,255)
#define  MY_COLOR_14_BLACK_BORD			Color(255,0,0,0)
#define  MY_COLOR_PEN					Color(255,0,0,0)
#define  MY_COLOR_TIME_PEN				Color(255, 25, 25, 25)
#define  MY_COLOR_PEN_RECTANGLE_BORD    Color(255,0,0,0)
#define  MY_COLOR_PEN_INLINE_PEN        Color(255,20,20,20)
#define  MY_COLOR_UNIT_PEN              Color(255,0,0,0)
#define  MY_COLOR_RIGHTCLICK_VALUE_COLOR		Color(255,255,0,255)

#define  MY_COLOR_VIEW_SELECT			Color(255,255,255,255)
#define  MY_COLOR_VIEW_UNSELECT			Color(255,192,192,192)


#define  MY_COLOR_PEN_VALUE_PEN        Color(255,255,255,255)

IMPLEMENT_DYNAMIC(CBacnetGraphic, CDialogEx)


CBacnetGraphic::CBacnetGraphic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetGraphic::IDD, pParent)
	, m_xscale(6)
	, m_yscale(4)
	, m_lowvalue(0)
	, m_highvalue(100)
	, m_time_selected(TIME_TEN_MINUTE)
	, m_analogorignpoint(200,30)
	, mdigital_count_intervel(30)
	, mdigital_height(20)
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
	ON_WM_LBUTTONDOWN()
	ON_WM_HELPINFO()
	ON_COMMAND(ID_TIMEBASE_1HOUR, &CBacnetGraphic::OnTimebase1hour)
	ON_COMMAND(ID_TIMEBASE_1DAY, &CBacnetGraphic::OnTimebase1day)
	ON_COMMAND(ID_TIMEBASE_10MINUTES, &CBacnetGraphic::OnTimebase10minutes)
	ON_COMMAND(ID_TIMEBASE_12HOURS, &CBacnetGraphic::OnTimebase12hours)
	ON_COMMAND(ID_TIMEBASE_4HOURS, &CBacnetGraphic::OnTimebase4hours)
	ON_COMMAND(ID_TIMEBASE_4DAYS, &CBacnetGraphic::OnTimebase4days)
	ON_COMMAND(ID_GRAPHIC_LEFT, &CBacnetGraphic::OnGraphicLeft)
	ON_COMMAND(ID_GRAPHIC_RIGHT, &CBacnetGraphic::OnGraphicRight)
	ON_COMMAND(ID_ZOOMIN, &CBacnetGraphic::OnZoomin)
	ON_COMMAND(ID_ZOOMOUT, &CBacnetGraphic::OnZoomout)
	ON_COMMAND(ID_TIMEBASE_CUSTOMERDEFINE, &CBacnetGraphic::OnTimebaseCustomerdefine)
	ON_COMMAND(ID_TIMEBASE_5MINUTES, &CBacnetGraphic::OnTimebase5minutes)
	ON_COMMAND(ID_TIMEBASE_30MINUTES, &CBacnetGraphic::OnTimebase30minutes)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CBacnetGraphic message handlers
unsigned long m_time_monitor_now; //X轴最右边的时间;
unsigned long m_time_left_time;


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
HICON Loading_Icon1;
HICON Loading_Icon2;
HICON Loading_Icon3;
HICON Loading_Icon4;
HICON Loading_Icon5;
HICON Loading_Icon6;


void CBacnetGraphic::Initial_Scale_Time()
{

    //从配置文件中获得上次所选的刻度;
    graphic_last_scale_type = GetPrivateProfileInt(_T("Setting"), _T("GraphicScaleType"), -1, g_cstring_ini_path);
    if (graphic_last_scale_type == -1)
    {
        CString temp_cs1;
        temp_cs1.Format(_T("%d"), TIME_TEN_MINUTE);
        WritePrivateProfileString(_T("Setting"), _T("GraphicScaleType"), temp_cs1, g_cstring_ini_path);

    }
    scale_type = graphic_last_scale_type;

    if (flag_auto_scroll)	//如果不是自动滚动，就要获取上次的 时间;
    {
        m_time_monitor_now = time(NULL);
    }
    else
    {
#if 0
        graphic_last_time_value = GetPrivateProfileInt(_T("Setting"), _T("Graphic_last_time_value"), -1, g_cstring_ini_path);
        if (graphic_last_time_value == -1)
        {
            m_time_monitor_now = time(NULL);
        }
        else
            m_time_monitor_now = graphic_last_time_value;
#endif
    }

    if (graphic_last_scale_type == TIME_FIVE_MINUTE)
        m_time_left_time = m_time_monitor_now - 300;
    else if (graphic_last_scale_type == TIME_TEN_MINUTE)
        m_time_left_time = m_time_monitor_now - 600;
    else if (graphic_last_scale_type == TIME_THIRTY_MINUTE)
        m_time_left_time = m_time_monitor_now - 1800;
    else if (graphic_last_scale_type == TIME_ONE_HOUR)
        m_time_left_time = m_time_monitor_now - 3600;
    else if (graphic_last_scale_type == TIME_FOUR_HOUR)
        m_time_left_time = m_time_monitor_now - 14400;
    else if (graphic_last_scale_type == TIME_TWELVE_HOUR)
        m_time_left_time = m_time_monitor_now - 43200;
    else if (graphic_last_scale_type == TIME_ONE_DAY)
        m_time_left_time = m_time_monitor_now - 86400;
    else if (graphic_last_scale_type == TIME_FOUR_DAY)
        m_time_left_time = m_time_monitor_now - 345600;

}

BOOL CBacnetGraphic::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	srand(NULL);
	Loading_Icon1 = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LOADING_1),IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  
	Loading_Icon2 = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LOADING_2),IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  
	Loading_Icon3 = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LOADING_3),IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  
	Loading_Icon4 = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LOADING_4),IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  
	Loading_Icon5 = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LOADING_5),IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  
	Loading_Icon6 = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LOADING_6),IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  

    m_analog_count = m_monitor_data.at(monitor_list_line).an_inputs;
    m_digital_count = m_monitor_data.at(monitor_list_line).num_inputs - m_monitor_data.at(monitor_list_line).an_inputs;
    

	b_has_create_point = false;
	flag_continue_thread = true;
	scale_type = TIME_THIRTY_MINUTE;

	flag_auto_scroll  = 1;//  Default auto scroll.// GetPrivateProfileInt(_T("Setting"),_T("GraphicAutoScroll"),0,g_cstring_ini_path);
	

    Initial_Scale_Time();
	

#ifndef _DEBUG
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
#endif
	
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

	time_interval_point = m_monitor_data.at(monitor_list_line).hour_interval_time *3600 + m_monitor_data.at(monitor_list_line).minute_interval_time * 60 + m_monitor_data.at(monitor_list_line).second_interval_time;
	if(monitor_list_line != 11)
		Get_Input_Unit();
	else
	{
		for(int m=0;m< (sizeof(System_Unit) / sizeof(System_Unit[0]));m++)
			InputUnit[m]= System_Unit[m];
	}
#if 0
	for(int i=0;i<14;i++)
		StaticShow[i] = true;	//初始化时都让显示,显示第一次的时候这些值会被自动的修改;
#endif
	InitialParameter(scale_type);
	if(mythread == NULL)
	{
		mythread=CreateThread(NULL,NULL,MyThreadPro,this,NULL,NULL);
		CloseHandle(mythread);
	}

	//if(read_monitor_sd_ret == MONITOR_READ_TIMEOUT)
	//{
	//	//MessageBox(_T("Read Monitor Data Timeout!"));
	//	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read monitor data timeout!"));
	//}
	//else if(read_monitor_sd_ret == MONITOR_READ_NO_DATA)
	//{
	//	//MessageBox(_T("No data in this period of time!"));
	//	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No data in this period of time!"));
	//}


	if(updatedatathread == NULL)
	{
		updatedatathread = CreateThread(NULL,NULL,UpdateDataThreadPro,this,NULL,NULL);
		CloseHandle(updatedatathread);
	}

    if (refreshthread == NULL)
    {
        refreshthread = CreateThread(NULL, NULL, RefreshThreadPro, this, NULL, NULL);
        CloseHandle(refreshthread);
    }


	InitDC();

	test_hwnd = this->m_hWnd;


	CRect Graphic_Window_Rect;
	GetWindowRect(Graphic_Window_Rect);
	window_width = Graphic_Window_Rect.right - Graphic_Window_Rect.left;
	window_hight = Graphic_Window_Rect.bottom - Graphic_Window_Rect.top;


	
	draw_graphic_finished = false;
	Delete_Ram_Data();
	b_has_create_point = false;


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
	m_time_monitor_now = (m_time_monitor_now / 60)*60;
	unsigned long temp_timesec1970 = m_time_monitor_now; 
	unsigned long temp_time_start;
	unsigned long temp_time_end;
	CalcOnePixelTime();
	//temp_time_start = temp_timesec1970 - x_axis_total_time;
	//temp_time_end = temp_timesec1970;
	//m_starttime = temp_time_start;
	//m_endtime = temp_time_end;

	//m_starttime = m_time_monitor_now -  x_axis_total_time;
	//m_endtime = m_time_monitor_now ;


	unsigned int temp_serial_number = g_selected_serialnumber;
	int temp_monitor_index = monitor_list_line;
	int temp_number_of_inputs = m_monitor_data.at(monitor_list_line).num_inputs;
	int temp_number_of_analog = m_monitor_data.at(monitor_list_line).an_inputs;
	int temp_number_of_digital = m_monitor_data.at(monitor_list_line).num_inputs - temp_number_of_analog;
	get_data_count = temp_number_of_inputs;
	monitor_analog_count = temp_number_of_analog;
	monitor_digital_count = temp_number_of_digital;
	if(monitor_digital_count >= 6)
	{
		mdigital_count_intervel = 30;
		mdigital_height = 20;
	}
	else if(monitor_digital_count >= 4)
	{
		mdigital_count_intervel = 40;
		mdigital_height = 30;
	}
	else if(monitor_digital_count == 3)
	{
		mdigital_count_intervel = 60;
		mdigital_height = 45;
	}
	else if(monitor_digital_count == 2)
	{
		mdigital_count_intervel = 80;
		mdigital_height = 60;
	}
	else if(monitor_digital_count == 1)
	{
		mdigital_count_intervel = 150;
		mdigital_height = 120;
	}

	for (int i=0;i< monitor_digital_count;i++)
	{
		SetDigitalYLabelPos(i);
		//mDigitalYLabelPoint[i].X = m_digitalorignpoint.X - 30;
		//mDigitalYLabelPoint[i].Y = m_digitalorignpoint.Y + mdigital_count_intervel*(i+1)  -(mdigital_height);
	}

	CBADO monitor_bado;
	monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
	monitor_bado.OnInitADOConn(); 

	for (int i=0;i<temp_number_of_inputs;i++)
	{
		if(StaticShow[i] == false)	//如果没有选中，就不要检索这部分的数据库;
			continue;
		CString cs_temp_input_index;
		unsigned int temp_record_count = 0;
		int temp_input_number = 0;
		int temp_input_point_type = 0;
		int temp_input_point_panel = 0;
		int temp_input_point_sub_panel = 0;
		int temp_input_network = 0;
		temp_input_number = m_monitor_data.at(monitor_list_line).inputs[i].number;
		temp_input_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type;
		temp_input_point_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		temp_input_point_sub_panel = m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;
        temp_input_network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		cs_temp_input_index.Format(_T("%u_%u_%u_%u_%u"),temp_input_number,temp_input_point_type,temp_input_point_panel,temp_input_point_sub_panel,temp_input_network);

		if(i<temp_number_of_analog)
		{
			analog_data_max_value[i] = 0x80000001;
			analog_data_min_value[i] = 0x7fffffff;
			CString strSql;
			//strSql.Format(_T("select * from MonitorData where Input_Index = '%s' and SerialNumber = %i and Monitor_Index = %i and Analog_Digital = 1 and Record_Time >= %i and Record_Time <= %i ORDER BY Record_Time ASC"),cs_temp_input_index,temp_serial_number,temp_monitor_index,temp_time_start,temp_time_end);	
			strSql.Format(_T("select * from MonitorData where Index_ = '%s'   and Time_Since1970 >= %i and Time_Since1970 <= %i and Analog_Digital = -1 ORDER BY Time_Since1970 ASC"),cs_temp_input_index,m_starttime,m_endtime);	
			monitor_bado.m_pRecordset=monitor_bado.OpenRecordset(strSql);
			temp_record_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
			if(temp_record_count <= 0)
			{
				analog_data_max_value[i] = 100000;
				analog_data_min_value[i] = 0;
				monitor_bado.CloseRecordset();//Ffff add
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
				temp_variant=monitor_bado.m_pRecordset->GetCollect("Time_Since1970");//
				if(temp_variant.vt!=VT_NULL)
					logging_time = temp_variant;
				else
				{
					logging_time = 0;
					monitor_bado.m_pRecordset->MoveNext();
					continue;
				}

				temp_variant=monitor_bado.m_pRecordset->GetCollect("Value_");//
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

				if(monitor_ignore_enable)
				{
					if((analog_data_point[i][temp_count].analogdata < monitor_ignore_min_value) || (analog_data_point[i][temp_count].analogdata > monitor_ignore_max_value))
					{
						monitor_bado.m_pRecordset->MoveNext();
						continue;
					}
				}

				if(analog_data_point[i][temp_count].analogdata > analog_data_max_value[i])	//取 最大值和最小值;
					analog_data_max_value[i] = analog_data_point[i][temp_count].analogdata;
				if(analog_data_point[i][temp_count].analogdata < analog_data_min_value[i])
					analog_data_min_value[i] = analog_data_point[i][temp_count].analogdata;

				temp_count ++;
				monitor_bado.m_pRecordset->MoveNext();
			}
			monitor_bado.CloseRecordset();//Ffff add
			analog_data_count[i] = temp_count;
		}
		else if((i >=temp_number_of_analog) && temp_number_of_digital>0)
		{
			CString strSql;
			//strSql.Format(_T("select * from MonitorData where Index_ = '%s'  and Analog_Digital = 0 and Time_Since1970 >= %i and Time_Since1970 <= %i ORDER BY Time_Since1970 ASC"),cs_temp_input_index,m_starttime,m_endtime);	
			strSql.Format(_T("select * from MonitorData where Index_ = '%s'  and Analog_Digital = 0 and Time_Since1970 <= %i  ORDER BY Time_Since1970 ASC"),cs_temp_input_index,m_endtime);	
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
				temp_variant=monitor_bado.m_pRecordset->GetCollect("Time_Since1970");//
				if(temp_variant.vt!=VT_NULL)
					logging_time = temp_variant;
				else
				{
					logging_time = 0;
					monitor_bado.m_pRecordset->MoveNext();
					continue;
				}

				temp_variant=monitor_bado.m_pRecordset->GetCollect("Value_");//
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


	for (int i =0 ;i<INPUT_NUMBER;i++)	//Initial all the first point;
	{
		m_pFirstItem[i] = NULL;
	}

	int digital_index = 0;
	point_error_2 = 0;
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
				if(x<14)
				{
					if(StaticShow[x] == false)
						break;
				}
				else
					return;

				if(TimeValueToPoint(analog_data_point[x][i].loggingtime,analog_data_point[x][i].analogdata,mytemppoint) == false)
				{
					point_error_2 = 1;
					break;
				}

				//CString strTime;
				//time_t scale_time ;
				//CTime time_scaletime;
				//scale_time = analog_data_point[x][i].loggingtime ;
				//time_scaletime = scale_time;
				//strTime = time_scaletime.Format("  %m/%d %H:%M:%S");

				//TRACE(_T("X = %d , Y = %d Time = %s\r\n"),(int)mytemppoint.X,(int)mytemppoint.Y,strTime);

				MyPoint Mypoint={0,0};
				Mypoint.x = mytemppoint.X;
				Mypoint.y = mytemppoint.Y;
				pTempItem->SetPoint(Mypoint);
				//if(i == (analog_data_count[x]-1))
				//{
				//	pTempItem->m_link_to_next = false;
				//}
				if(pPrevItem != NULL)
				{
					pPrevItem->SetNext(pTempItem);
					//切尔西说 模拟量的 值 也要改为 有变化的时候记录， 所以所有的点都需要自动连接至下一个点;  2016 02 25
					if(analog_data_point[x][i].loggingtime - analog_data_point[x][i - 1].loggingtime > 10*time_interval_point)
					{
						pPrevItem->m_link_to_next = false;
					}
				}
				pPrevItem = pTempItem;

				if(m_pFirstItem[x] == NULL)
				{
					m_pFirstItem[x] = pTempItem;
					m_pFirstItem[x]->SetIndex(0);
					pTempItem->m_link_to_next = true;
				}
			}
		}
		else if((x>= monitor_analog_count) && (monitor_digital_count > 0))
		{
			pTempItem = NULL;
			pPrevItem = NULL;
			if(digital_data_point[x] == NULL)
				continue;
			digital_index ++;
			for (int i=0 ; i<digital_data_count[x];i++)
			{
				pTempItem = new CPointItem();
				PointF mytemppoint(0,0);
				if(digital_data_point[x] == NULL)
					continue;


				bool b_ret = false;
				b_ret = DigitalTimeValueToPoint(digital_data_point[x][i].loggingtime,digital_data_point[x][i].analogdata,mytemppoint,digital_index);
				if(digital_data_point[x][i].loggingtime    < m_starttime)
				{
					digital_last_data[x].analogdata = digital_data_point[x][i].analogdata;
					digital_last_data[x].data_point.X = m_analogorignpoint.X;

					if(digital_last_data[x].analogdata == 1)
					{
						mytemppoint.Y =m_digitalorignpoint.Y + 30*digital_index  - 20;
					}
					else
						mytemppoint.Y = m_digitalorignpoint.Y  + 30*digital_index +20 - 20;

					continue;
				}
				SetDigitalYLabelPos(digital_index);


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

	if(point_error_2 == 1)
	{
		draw_graphic_finished = false;
		b_has_create_point = false;
	}
	else
	{
		b_has_create_point = true;
		draw_graphic_finished = true;
	}

	return;
}


DWORD WINAPI RefreshThreadPro(LPVOID lPvoid)
{
    CBacnetGraphic * mparent = (CBacnetGraphic *)lPvoid;
    while (flag_continue_thread)
    {
        if (flag_auto_scroll == false)
        {
            Sleep(10000);
            draw_graphic_finished = false;
            b_has_create_point = false;
            continue;
        }

        if (draw_graphic_finished == false)	//避免客户 频繁切换数据;
        {
            Sleep(5000);
            continue;
        }
        unsigned long tempstarttime;
        unsigned long tempendtime;
        mparent->GetXaxisTime(tempstarttime, tempendtime);
        unsigned long temp_x_total_time = tempendtime - tempstarttime;
        unsigned long temp_timestart = time(NULL);
        m_time_monitor_now = temp_timestart;
        temp_timestart = temp_timestart - temp_x_total_time;

        mparent->SetXaxisStartTime(temp_timestart);
        draw_graphic_finished = false;
        b_has_create_point = false;

        for (int i = 0;i<1000;i++)
        {
            Sleep(10);
            if (!flag_continue_thread)
            {
                refreshthread = NULL;
                return 0;
            }
        }
    }
    refreshthread = NULL;
    return 0;
}

//此线程用于每隔5秒读取数据
DWORD WINAPI UpdateDataThreadPro(LPVOID lPvoid)
{
    CBacnetGraphic * mparent = (CBacnetGraphic *)lPvoid;

    CString strSql;
    CBADO monitor_bado;
    monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
    TRACE(g_achive_monitor_datatbase_path);
    monitor_bado.OnInitADOConn();
    CString temp_time;
    temp_time.Format(_T(" and Time_Since1970 < %u "), m_time_left_time - 36000); //删除10个小时之前的 临时数据;
    
    strSql = _T("delete * from MonitorData where Flag <> 0") + temp_time;
    monitor_bado.m_pConnection->Execute(strSql.GetString(), NULL, adCmdText);
    monitor_bado.CloseConn();
    int n_update_shou_1_once = true;
    while (flag_continue_thread)
    {
        mparent->Initial_Scale_Time();
        bool refresh_ret = false;
        ncontinue_read_data = true;
        int read_analog_ret = 0;
        int read_dig_ret = 0;
        if (mparent->m_analog_count != 0)  //Fan如果不包含模拟量，就没有必要刷新读的数据;
        {
            if(n_update_shou_1_once == true)
                g_progress_persent = 1;
            TRACE(_T("Start read BAC_UNITS_ANALOG \r\n"));
            read_analog_ret = read_monitordata(BAC_UNITS_ANALOG, m_time_left_time, m_time_monitor_now);
            if (read_analog_ret < 0)
            {
                if (read_analog_ret == -5)
                {
                    no_space_auto_close = 20;
                    g_progress_persent = 100;
                    updatedatathread = NULL;
                    return 0;
                }
                BitToString(BAC_UNITS_ANALOG, monitor_list_line);
                continue;
            }
            if (n_update_shou_1_once == true)
                g_progress_persent = 100;
            BitToString(BAC_UNITS_ANALOG, monitor_list_line);
        }
        
        if (mparent->m_digital_count != 0)  //Fan 如果不包含数字量，就没有必要刷新读的数据;
        {
            if (n_update_shou_1_once == true)
                g_progress_persent = 1;
            read_dig_ret = read_monitordata(BAC_UNITS_DIGITAL, m_time_left_time, m_time_monitor_now);
            if (read_dig_ret < 0)
            {
                BitToString(BAC_UNITS_DIGITAL, monitor_list_line);
                continue;
            }
            if (n_update_shou_1_once == true)
                g_progress_persent = 100;
            BitToString(BAC_UNITS_DIGITAL, monitor_list_line);
        }
        n_update_shou_1_once = false;


        //if (!refresh_ret)
        //{
        //    Sleep(5000);
        //    continue;
        //}

        draw_graphic_finished = false;
        b_has_create_point = false;


        for (int i = 0;i<1000;i++)
        {
            Sleep(5);
            if (ncontinue_read_data == false)
            {
                break;      //如果有按键触发 ，更新线程要立刻启动区刷新数据;
            }
            if (!flag_continue_thread)
            {
                updatedatathread = NULL;
                return 0;
            }
        }
    }
    updatedatathread = NULL;
    return 0;
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
				digital_last_data[i].analogdata = 0;
				digital_last_data[i].data_point.X = 0;
				digital_last_data[i].data_point.Y = 0;
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

			mparent->Reset_X_Y_Parameter();
			//long t1=GetTickCount();//程序段开始前取得系统运行时间(ms);
			mparent->Search_data_from_db();
			//long t2=GetTickCount();//程序段结束后取得系统运行时间(ms);
			//CString str_temp;
			//str_temp.Format(_T("time:%dms\r\n"),t2-t1);//前后之差即 程序运行时间;
			//TRACE(str_temp);

			mparent->Create_Line_Point();	//新建需要绘制的点;一直到下次开始刷新 或者定时器刷新时在去获取点;
			//b_has_create_point = true;
			//draw_graphic_finished = true;
		}
		WaitForSingleObject(Point_Mutex,INFINITE); 
		mparent->Draw_Graphic(hMemDC);
		ReleaseMutex(Point_Mutex);

		BitBlt(gloab_hdc,0,0, myRect.right, myRect.bottom, hMemDC, 0, 0, SRCCOPY);//将绘制完成的内存位图贴到的Picture空间对象中;
		//PostMessage(test_hwnd, WM_FRESH_STATIC,0,0);	//当重绘了之后，控件会被重绘的画布遮盖住，所以发送消息让控件再次刷新;
		if(start_wait_init < 4)
			Sleep(10);
		else
			Sleep(500);

        if (no_space_auto_close)
        {
            no_space_auto_close--;
            if(no_space_auto_close == 0)
                ::PostMessage(myhWnd, WM_CLOSE, NULL, NULL);
        }
		//flag_continue_thread = false;
	}
	mythread = NULL;
	return 0;
}


LRESULT CBacnetGraphic::Fresh_Static_Function(WPARAM wParam,LPARAM lParam)
{
	return 0;
}



void CBacnetGraphic::Draw_Graphic(HDC my_hdc)
{
	Graphics *mygraphics;
	mygraphics = new Graphics(my_hdc);
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	Pen *myRectangle_pen;
	Pen * my_inline_pen;
	Pen * m_value_move_pen;
	Pen * static_write_bord;
	Pen * static_black_bord;
	Pen * static_auto_scroll_pen;

	Pen * CurePen;

	Pen * persent_pen1;Pen * persent_pen2;Pen * persent_pen3;
	persent_pen1 = new Pen(Color(255,255,0,0),2.0);	persent_pen2 = new Pen(Color(255,0,255,0),2.0);	persent_pen3 = new Pen(Color(255,0,0,255),2.0);

	SolidBrush *BlackBrush;
	
	
	myRectangle_pen = new Pen(MY_COLOR_PEN_RECTANGLE_BORD);
	my_inline_pen = new Pen(MY_COLOR_PEN_INLINE_PEN);
	m_value_move_pen = new Pen(MY_COLOR_PEN_VALUE_PEN);
	static_write_bord = new  Pen(MY_COLOR_14_WRITE_BORD,2.0f);
	static_black_bord = new  Pen(MY_COLOR_14_BLACK_BORD,2.0f);
	static_auto_scroll_pen = new  Pen(Color(255,255,255,255),4.0f);

	REAL dashValues[2] = {3, 7};
	//Pen blackPen(Color(255, 0, 0, 0), 5);
	my_inline_pen->SetDashPattern(dashValues, 2);
	REAL valuedashValues[2] = {4, 6};
	m_value_move_pen->SetDashPattern(valuedashValues, 2);

	CurePen = new Pen(Graphic_Color[1],2.0f);
	PointF      pointF(0, 0);

	//BlackBrush =new SolidBrush(Color(255,0,0,0));老毛希望背景色改为灰色.
	BlackBrush =new  SolidBrush(MY_COLOR_BACKGRAND) ;

	
	mygraphics->FillRectangle(BlackBrush,0,0,window_width,window_hight);
	mygraphics->DrawRectangle(myRectangle_pen,(int)m_analogorignpoint.X,(int)m_analogorignpoint.Y,m_X_ASIX_WIDTH,m_Y_ASIX_HIGHT);
	mygraphics->DrawRectangle(myRectangle_pen,(int)m_digitalorignpoint.X,(int)m_digitalorignpoint.Y,m_Digital_X_WIDTH,m_Digital_Y_HIGHT  );

	SolidBrush *Static_blackground_Brush;
	Pen *mystaticRectangle_pen;
	mystaticRectangle_pen = new Pen(MY_COLOR_PEN,2.0f);
	Static_blackground_Brush =new SolidBrush(MY_COLOR_14BGD);	//This part is draw the 14 label and it's background;

	//mygraphics->FillRectangle(Static_blackground_Brush,0,window_hight - 200,window_width,200);
	//mygraphics->DrawRectangle(mystaticRectangle_pen,2,window_hight - 200,window_width-15,200 -55);
	mygraphics->FillRectangle(Static_blackground_Brush,4,4,9 + STATIC_LABLE_WIDTH + 40,740);
	mygraphics->DrawRectangle(mystaticRectangle_pen,6,4,9 + STATIC_LABLE_WIDTH + 40,740);


	FontFamily  ScrollfontFamily(_T("Arial"));

	SolidBrush  Font_brush_temp(MY_COLOR_AUTOSCROLL);
	SolidBrush  Font_brush_on_off(MY_COLOR_ON_OFF);
	Gdiplus::Font  Scroll_font(&ScrollfontFamily, 18, FontStyleRegular, UnitPixel);


	SolidBrush *Static_scroll_blackground_Brush;
	Static_scroll_blackground_Brush =new SolidBrush(MY_COLOR_14LABLE_BGD);	//This part is draw the 14 label and it's background;
	mygraphics->FillRectangle(Static_scroll_blackground_Brush,120,714,50,20);


	PointF      scrollpointF(0, 0);
	scrollpointF.X = 25;
	scrollpointF.Y = 715;
	mygraphics->DrawString(_T("Auto Scroll"), -1, &Scroll_font, scrollpointF,&Font_brush_temp);
	scrollpointF.X = 122;
	scrollpointF.Y = 714;
	if(flag_auto_scroll)
		mygraphics->DrawString(_T("ON"), -1, &Scroll_font, scrollpointF,&Font_brush_on_off);
	else
		mygraphics->DrawString(_T("OFF"), -1, &Scroll_font, scrollpointF,&Font_brush_on_off);

    if (no_space_auto_close)
    {
        scrollpointF.X = 500;
        scrollpointF.Y = 200;
        mygraphics->DrawString(_T("There is not enough space to store new data (selected trend log)."), -1, &Scroll_font, scrollpointF, &Font_brush_on_off);

    }



	delete Static_scroll_blackground_Brush;

#pragma region Show_Three_View
//#define  MY_COLOR_VIEW_SELECT			Color(255,255,255,255)
//#define  MY_COLOR_VIEW_UNSELECT			Color(255,192,192,192)

	SolidBrush  View_Font_brush_temp(MY_COLOR_VIEW123);
	SolidBrush *Static_view_Brush;
	SolidBrush *Static_view_unselect_Brush;
	Static_view_Brush =new SolidBrush(MY_COLOR_VIEW_SELECT);	
	Static_view_unselect_Brush =new SolidBrush(MY_COLOR_VIEW_UNSELECT);

	//绘制 屏幕上方的 三个View.
	if(graphic_view_index == 0)
		mygraphics->FillRectangle(Static_view_Brush,850,6,120,20);
	else
		mygraphics->FillRectangle(Static_view_unselect_Brush,850,6,120,20);

	mygraphics->DrawRectangle(mystaticRectangle_pen,850,6,120,20);
	scrollpointF.X = 850;
	scrollpointF.Y = 6;
	mygraphics->DrawString(grapgic_view_name[0], -1, &Scroll_font, scrollpointF,&View_Font_brush_temp);

	if(graphic_view_index == 1)
		mygraphics->FillRectangle(Static_view_Brush,1000,6,120,20);
	else
		mygraphics->FillRectangle(Static_view_unselect_Brush,1000,6,120,20);
	mygraphics->DrawRectangle(mystaticRectangle_pen,1000,6,120,20);
	scrollpointF.X = 1000;
	scrollpointF.Y = 6;
	mygraphics->DrawString(grapgic_view_name[1], -1, &Scroll_font, scrollpointF,&View_Font_brush_temp);

	if(graphic_view_index == 2)
		mygraphics->FillRectangle(Static_view_Brush,1150,6,120,20);
	else
		mygraphics->FillRectangle(Static_view_unselect_Brush,1150,6,120,20);
	mygraphics->DrawRectangle(mystaticRectangle_pen,1150,6,120,20);
	scrollpointF.X = 1150;
	scrollpointF.Y = 6;
	mygraphics->DrawString(grapgic_view_name[2], -1, &Scroll_font, scrollpointF,&View_Font_brush_temp);

	delete Static_view_unselect_Brush;
	delete Static_view_Brush;
	Static_view_Brush = NULL;
	Static_view_unselect_Brush = NULL;
#pragma endregion Show_Three_View

	Gdiplus::Font  TimeTopShow_font(&ScrollfontFamily, 24, FontStyleRegular, UnitPixel);
	scrollpointF.X = 700;
	scrollpointF.Y = 2;
	CString top_string;
	switch(m_time_selected)
	{
	case TIME_FIVE_MINUTE:
			top_string.Format(_T("5 Minutes"));
	
		break;
	case TIME_TEN_MINUTE :
			top_string.Format(_T("10 Minutes"));
		break;
	case TIME_ONE_HOUR:
		top_string.Format(_T("1 Hour"));
		break;
	case TIME_THIRTY_MINUTE:
		top_string.Format(_T("30 Minutes"));
		break;
	case TIME_FOUR_HOUR:
		top_string.Format(_T("4 Hours"));
		break;
	case TIME_TWELVE_HOUR:
		top_string.Format(_T("12 Hours"));
		break;
	case TIME_ONE_DAY:
		top_string.Format(_T("1 Day"));
		break;
	case TIME_FOUR_DAY:
		top_string.Format(_T("4 Days"));
		break;
	default:
		break;
	}
	
	mygraphics->DrawString(top_string, -1, &TimeTopShow_font, scrollpointF,&Font_brush_on_off);

	//右键点击 在绘图区域显示当前的值;
	if((RclickValueTime.X != 0) && (RclickValueTime.Y != 0))
	{
		CString cs_value ;
		float temp_nvalue;
		temp_nvalue =	 ( m_analogorignpoint.Y + m_Y_ASIX_HIGHT - RclickValueTime.Y ) * m_onepiexlvalue  + m_lowvalue ;
		cs_value.Format(_T("%.1f"),temp_nvalue);
		SolidBrush  RClickBrush(MY_COLOR_RIGHTCLICK_VALUE_COLOR);
		mygraphics->DrawString(cs_value, -1, &TimeTopShow_font, RclickValueTime,&RClickBrush);
		mygraphics->DrawLine(m_value_move_pen,(INT)250, (INT) RclickValueTime.Y   ,(INT)  1250, (INT) RclickValueTime.Y);
		if(contain_digital )
		mygraphics->DrawLine(m_value_move_pen,(INT)RclickValueTime.X, (INT) (30)   ,(INT)RclickValueTime.X, (INT) (30 + 500));
		else
		{
			mygraphics->DrawLine(m_value_move_pen,(INT)RclickValueTime.X, (INT) (30)   ,(INT)RclickValueTime.X, (INT) (30 + 650));
		}
	}

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

		RectPosition[i].left = 15 ;
		RectPosition[i].top = 15 + i* 50;
		RectPosition[i].right = 15   + STATIC_LABLE_WIDTH;
		RectPosition[i].bottom = 15 + i* 50  + STATIC_LABLE_HIGHT;
		mygraphics->FillRectangle(Static_blackground_Brush,RectPosition[i].left,RectPosition[i].top,STATIC_LABLE_WIDTH,STATIC_LABLE_HIGHT);
		mygraphics->DrawLine(static_write_bord,RectPosition[i].left - 2,  RectPosition[i].top -2   ,RectPosition[i].right     ,RectPosition[i].top -2);
		mygraphics->DrawLine(static_write_bord,RectPosition[i].left - 2,  RectPosition[i].top -2   ,RectPosition[i].left - 2  ,RectPosition[i].bottom);
		mygraphics->DrawLine(static_black_bord,RectPosition[i].left ,     RectPosition[i].bottom   ,RectPosition[i].right     ,RectPosition[i].bottom);
		mygraphics->DrawLine(static_black_bord,RectPosition[i].right ,    RectPosition[i].top      ,RectPosition[i].right     ,RectPosition[i].bottom);



		SolidBrush  Font_brush(STATIC_FONT_COLOR);
		FontFamily  UnitfontFamily(_T("Arial"));
		//FontFamily  StaticfontFamily(_T("Times New Roman"));
		FontFamily  StaticfontFamily(_T("Arial"));
		
		Gdiplus::Font        Input_font(&StaticfontFamily, 14, FontStyleRegular, UnitPixel);
		
		PointF      staticpointF(0, 0);
		SolidBrush *   pen_unit_brush = new SolidBrush(Graphic_Color[i+1]);
		PointF      UnitpointF(0, 0);

		staticpointF.X = RectPosition[i].left + 4;
		staticpointF.Y = RectPosition[i].top + 4 ;
		UnitpointF.X = RectPosition[i].left + 4;
		UnitpointF.Y =  RectPosition[i].top + 4  + 18;

		mygraphics->DrawString(temp_cs, -1, &Input_font, staticpointF,&Font_brush);

		CString temp_cs_label_unit;
		temp_cs_label_unit =  InputUnit[i];// _T("AAAAAA");
		if(i<get_data_count)
			mygraphics->DrawString(temp_cs_label_unit, -1, &Input_font, UnitpointF, &Font_brush);
		delete pen_unit_brush;
		pen_unit_brush = NULL;




		//************************************************************************************
		//这些是画 Static 后面的 数字;
		CString temp_item;
		temp_item.Format(_T("%x"),i+1);
		temp_item = temp_item.MakeUpper();

		SolidBrush  static_item_brush(Graphic_Color[i+1]);
		
		Gdiplus::Font        unitfont(&UnitfontFamily, 22, FontStyleRegular, UnitPixel);
		pointF.X = RectPosition[i].left + STATIC_LABLE_WIDTH + 5;
		pointF.Y = RectPosition[i].top + 4;

		mygraphics->DrawString(temp_item, -1, &unitfont, pointF, &static_item_brush);
		//************************************************************************************
		if(Static_blackground_Brush)
			delete Static_blackground_Brush;
		Static_blackground_Brush = NULL;
	}
	//****************************************************************************************************


	SolidBrush  time_brush(MY_COLOR_TIME_PEN);
	//FontFamily  fontFamily(_T("Times New Roman"));
	FontFamily  fontFamily(_T("Arial"));
	
	Gdiplus::Font        time_font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	for(int i=0;i<m_xscale + 1;i++)				//画网格线
	{
		//mygraphics->DrawLine(my_inline_pen,(int)m_analogorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
		//									(int)m_analogorignpoint.Y,
		//									 (int)m_analogorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
		//									 (int)m_analogorignpoint.Y + m_Y_ASIX_HIGHT);

		//mygraphics->DrawLine(my_inline_pen,(int)m_digitalorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
		//	(int)m_digitalorignpoint.Y,
		//	(int)m_digitalorignpoint.X+(m_X_ASIX_WIDTH/m_xscale)*(i+1),
		//	(int)m_digitalorignpoint.Y + m_Digital_Y_HIGHT);

		mygraphics->DrawLine(my_inline_pen,(int)m_analogorignpoint.X+(m_X_ASIX_WIDTH*i)/m_xscale,
			(int)m_analogorignpoint.Y,
			(int)m_analogorignpoint.X+(m_X_ASIX_WIDTH*i)/m_xscale,
			(int)m_analogorignpoint.Y + m_Y_ASIX_HIGHT);

		mygraphics->DrawLine(my_inline_pen,(int)m_digitalorignpoint.X+(m_X_ASIX_WIDTH*i)/m_xscale,
			(int)m_digitalorignpoint.Y,
			(int)m_digitalorignpoint.X+(m_X_ASIX_WIDTH*i)/m_xscale,
			(int)m_digitalorignpoint.Y + m_Digital_Y_HIGHT);


		CString strTime ;
		wchar_t temp_char[200];
		time_t scale_time ;
		CTime time_scaletime;
		scale_time = m_starttime +  i*(x_axis_total_time / m_xscale);
		time_scaletime = scale_time;
		if((m_time_selected <= TIME_ONE_HOUR) && (m_time_selected >= TIME_FIVE_MINUTE))
			strTime = time_scaletime.Format("  %m/%d \r\n%H:%M:%S");
		else
			strTime = time_scaletime.Format("%m/%d %H:%M");

		pointF.X = (int)m_analogorignpoint.X - 30 + i*(m_X_ASIX_WIDTH/m_xscale);
		pointF.Y = (int)m_analogorignpoint.Y + m_Y_ASIX_HIGHT + 5;
		mygraphics->DrawString(strTime, -1, &time_font, pointF, &time_brush);
	}

	SolidBrush  unit_brush(MY_COLOR_UNIT_PEN);
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
		{
			Unit_value.Format(_T("%.0f"),((float)y_axis_total_value/m_yscale)*(m_yscale-i) + m_lowvalue);
		}
		else
		{
			Unit_value.Format(_T("%.0f"),m_lowvalue);
		}

		int value_temp_length = Unit_value.GetLength();
		pointF.X = (int)m_analogorignpoint.X - 38  - (value_temp_length-3)*4;	//动态调整 Y轴 显示的值 根据所带小数点的位数不同 调整位置;
		
		if(i == m_yscale)	//如果是显示靠近原点的 Y值 因为与X轴的时间显示 有轻微重叠，所以特殊处理，稍微向上一点;
		{
			pointF.Y =  (int)m_analogorignpoint.Y+ i*(m_Y_ASIX_HIGHT/m_yscale) - 16;
			mygraphics->DrawString(Unit_value, -1, &unitfont, pointF, &unit_brush);
		}
		else
		{
			pointF.Y =  (int)m_analogorignpoint.Y+ i*(m_Y_ASIX_HIGHT/m_yscale) - 8;
			mygraphics->DrawString(Unit_value, -1, &unitfont, pointF, &unit_brush);
		}
	}


#if 0
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
#endif
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
		DrawLinePen[i] = new Pen(Graphic_Color[i + 1],3.0f);
		if(i<monitor_analog_count)
		{
			if(first_item[i] != NULL)
			{
				do 
				{
					second_item[i]=first_item[i]->GetNext();
					if(second_item[i]==NULL)
						break;
					if(first_item[i]->m_link_to_next)
						mygraphics->DrawLine(DrawLinePen[i],first_item[i]->GetPoint().x,first_item[i]->GetPoint().y,second_item[i]->GetPoint().x,second_item[i]->GetPoint().y);

					first_item[i] = second_item[i];
				} while ((second_item[i]->GetNext())!=NULL);
			}
		}
		else if(monitor_digital_count > 0)
		{
			CString temp_cs_label;
			temp_cs_label = InputLable[i];

			SolidBrush  static_item_brush(Graphic_Color[i+1]);
			//SolidBrush  Font_brush(STATIC_FONT_COLOR);
			//FontFamily  UnitfontFamily(_T("Arial"));
			//FontFamily  StaticfontFamily(_T("Times New Roman"));
			FontFamily  StaticfontFamily(_T("Arial"));

			Gdiplus::Font        DiInputFont(&StaticfontFamily, 10, FontStyleRegular, UnitPixel);

			PointF      staticpointF(0, 0);
			//SolidBrush *   pen_unit_brush = new SolidBrush(Graphic_Color[i+1]);//
			//PointF      UnitpointF(0, 0);

			//staticpointF.X = m_digitalorignpoint.X - 30;
			staticpointF = (PointF)GetDigitalYLabelPos(i-monitor_analog_count);// m_digitalorignpoint.Y + 30*(i-monitor_analog_count)  ;


			
			if(first_item[i] != NULL)
			{
				staticpointF.Y =  first_item[i]->GetPoint().y;
				mygraphics->DrawString(temp_cs_label, -1, &DiInputFont, staticpointF,&static_item_brush);
			}

			if(first_item[i] != NULL)
			{
				if((digital_last_data[i].data_point.X != 0) && (digital_last_data[i].data_point.Y != 0))	//有可能确实出现前面所有一个点都没有，这种情况就不要从前面拉线了;
				{
					if(digital_last_data[i].data_point.X < first_item[i]->GetPoint().x)
					{
						//下面一部分是处理 digital  前半段没有数据，需要参考 xtime 之前的最近的状态来划线的做法;

						if(digital_last_data[i].analogdata == first_item[i]->GetPointValue())
							mygraphics->DrawLine( DrawLinePen[i],
							(int)digital_last_data[i].data_point.X,
							(int)digital_last_data[i].data_point.Y,
							first_item[i]->GetPoint().x,
							first_item[i]->GetPoint().y);
						else
						{
							mygraphics->DrawLine(DrawLinePen[i],
								(int)digital_last_data[i].data_point.X, (int)digital_last_data[i].data_point.Y,
								first_item[i]->GetPoint().x,  (int)digital_last_data[i].data_point.Y);
							mygraphics->DrawLine(DrawLinePen[i],
								first_item[i]->GetPoint().x,(int)digital_last_data[i].data_point.Y,
								first_item[i]->GetPoint().x,first_item[i]->GetPoint().y);
						}
					}
				}







				do 
				{
					second_item[i]=first_item[i]->GetNext();
					if(second_item[i]==NULL)
					{
						int temp_x = 0 ;
						
						//if(first_item[i]->GetPoint().x + 20 < m_digitalorignpoint.X + m_Digital_X_WIDTH)
						//	temp_x = first_item[i]->GetPoint().x + 20;
						//else
							temp_x = m_digitalorignpoint.X + m_Digital_X_WIDTH;
						mygraphics->DrawLine(DrawLinePen[i],first_item[i]->GetPoint().x,first_item[i]->GetPoint().y,temp_x,first_item[i]->GetPoint().y);
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
			}
			else	//目标区域一个点都没有，需要参考很久以前的值;
			{
				if((digital_last_data[i].data_point.X != 0) && (digital_last_data[i].data_point.Y != 0))	//有可能确实出现前面所有一个点都没有，这种情况就不要从前面拉线了;
					mygraphics->DrawLine(DrawLinePen[i],digital_last_data[i].data_point.X,digital_last_data[i].data_point.Y,digital_last_data[i].data_point.X + m_Digital_X_WIDTH,digital_last_data[i].data_point.Y);
			}
		}

	}

#if 1 //中间绘制提示进度条
	int reg_x;
	int reg_y;
	reg_x = 710;
	reg_y = 310;
	if((g_progress_persent != 0 )  && (g_progress_persent != 100))
	{

		static int temp_cycle = 0;
		temp_cycle  = (++temp_cycle) % 6;

		if(temp_cycle==0)
		{
			Bitmap icon_bitmap(Loading_Icon1);		
			
			mygraphics->DrawImage(&icon_bitmap,(int)reg_x ,(int)reg_y,(int)90,(int)90);
		}
		else if(temp_cycle==1)
		{
			Bitmap icon_bitmap(Loading_Icon2);		
			mygraphics->DrawImage(&icon_bitmap,(int)reg_x ,(int)reg_y,(int)90,(int)90);
		}
		else if(temp_cycle==2)
		{
			Bitmap icon_bitmap(Loading_Icon3);		
			mygraphics->DrawImage(&icon_bitmap,(int)reg_x ,(int)reg_y,(int)90,(int)90);
		}
		else if(temp_cycle==3)
		{
			Bitmap icon_bitmap(Loading_Icon4);		
			mygraphics->DrawImage(&icon_bitmap,(int)reg_x ,(int)reg_y,(int)90,(int)90);
		}
		else if(temp_cycle==4)
		{
			Bitmap icon_bitmap(Loading_Icon5);		
			mygraphics->DrawImage(&icon_bitmap,(int)reg_x ,(int)reg_y,(int)90,(int)90);
		}
		else if(temp_cycle==5)
		{
			Bitmap icon_bitmap(Loading_Icon6);		
			mygraphics->DrawImage(&icon_bitmap,(int)reg_x ,(int)reg_y,(int)90,(int)90);
		}




		//if((temp_cycle >= 0) && (temp_cycle<3))
		//	mygraphics->DrawEllipse(persent_pen3,(INT)reg_x,(INT)reg_y,(INT)80,(INT)80);
		//else if((temp_cycle >= 3) && (temp_cycle<6)) 
		//	mygraphics->DrawEllipse(persent_pen2,(INT)reg_x,(INT)reg_y,(INT)80,(INT)80);
		//else if((temp_cycle >= 6) && (temp_cycle <9))
		//	mygraphics->DrawEllipse(persent_pen3,(INT)reg_x,(INT)reg_y,(INT)80,(INT)80);
		CString temp_cs_draw_persent;
		temp_cs_draw_persent.Format(_T("%u%%"),g_progress_persent);
		PointF      ReadingpointF(0, 0);
		Gdiplus::Font  PersentShow_font(&ScrollfontFamily, 18, FontStyleRegular, UnitPixel);
		SolidBrush  Persentbrush( Color(255,255,255,255));
		ReadingpointF.X = reg_x + 8;
		ReadingpointF.Y = reg_y + 25;;
		mygraphics->DrawString(_T("Reading"), -1, &PersentShow_font, ReadingpointF,&Font_brush_on_off);

		ReadingpointF.X = reg_x + 23;
		ReadingpointF.Y = reg_y + 45;
		mygraphics->DrawString(temp_cs_draw_persent, -1, &PersentShow_font, ReadingpointF,&Font_brush_on_off);



	}

#endif


	

	for (int i=0;i<INPUT_NUMBER;i++)
	{
		if(DrawLinePen[i] != NULL)
		{
			delete DrawLinePen[i];
			DrawLinePen[i] = NULL;
		}
	}


	delete persent_pen1;delete persent_pen2;delete persent_pen3;
	delete m_value_move_pen;
	delete CurePen;
	delete mygraphics;
	delete myRectangle_pen;
	delete my_inline_pen;
	delete BlackBrush;

	delete static_write_bord;
	delete static_black_bord;
	delete static_auto_scroll_pen;
	delete mystaticRectangle_pen;
}






void CBacnetGraphic::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 2:
		{
			KillTimer(2);
			b_has_create_point = false;
			draw_graphic_finished = false;
		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}







BOOL CBacnetGraphic::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_LEFT)
		{
			OnGraphicLeft();
			return 0;
		}
		else if(pMsg->wParam == VK_RIGHT)
		{
			OnGraphicRight();
			return 0;
		}
        else if ((pMsg->wParam == VK_OEM_MINUS) || (pMsg->wParam == VK_SUBTRACT))   //Add by Fandu 2018 0104  支持快捷键加减用来缩进
        {
            OnZoomout();
            return 0;
        }
        else if (pMsg->wParam == VK_OEM_PLUS || (pMsg->wParam == VK_ADD))           //Add by Fandu 2018 0104  支持快捷键加减用来缩进
        {
            OnZoomin();
            return 0;
        }
        else if ((pMsg->wParam >= 0x31) && (pMsg->wParam <= 0x39))                   //Add by Fandu 2018 0104  按下数字键 1-9后  对应的Label 显示和不显示
        {
            unsigned char ntemp = pMsg->wParam - 0x31;
            if (StaticShow[ntemp] == true)
                StaticShow[ntemp] = false;
            else
                StaticShow[ntemp] = true;
            SetTimer(2, 5000, NULL);       
            WriteViewInfoIntoConfig();
        }
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetGraphic::OnCancel()
{
	
//	PostMessage(WM_CLOSE,NULL,NULL);
	//GraphicWindow = NULL;
	//::PostMessage(m_monitor_dlg_hwnd,WM_MONITOR_USER_MESSAGE,MONITOR_MESSAGE_DELETE,0);
	//CDialogEx::OnCancel();
	DestroyWindow();//窗口的销毁        //CDialog::OnCancel();//屏蔽基类的OnCancel,因为基类的OnCancel调用了EndDialog函数，这是模态Dialog的。   
}


void CBacnetGraphic::OnOK()
{
	

	CDialogEx::OnOK();
}

//计算点击的坐标是否在某个  自己绘制的STATIC 里面 ，如果在里面就设置是否变色 以及 显示;
void CBacnetGraphic::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	RclickValueTime.X = 0;
	RclickValueTime.Y = 0;
	int click_item = -1;
	b_show_value_line = false;
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

			SetTimer(2,5000,NULL);
			WriteViewInfoIntoConfig();
			break;
		}
	}

	//点击时间轴是否 自动 卷动的 自绘按钮;
	if((point.x > 122) &&
		(point.x <172) &&
		(point.y > 710) &&
		(point.y < 710+ 20))
	{
		if(flag_auto_scroll)
		{
			WritePrivateProfileStringW(_T("Setting"),_T("GraphicAutoScroll"),_T("0"),g_cstring_ini_path);
			flag_auto_scroll = false;
			CString cs_temp_value;
			cs_temp_value.Format(_T("%d"),m_time_monitor_now);
			WritePrivateProfileStringW(_T("Setting"),_T("Graphic_last_time_value"),cs_temp_value,g_cstring_ini_path);

		}
		else
		{
			WritePrivateProfileStringW(_T("Setting"),_T("GraphicAutoScroll"),_T("1"),g_cstring_ini_path);
			flag_auto_scroll = true;
		}
	}
	else if((point.x > 850) &&
		(point.x <970) &&
		(point.y > 6) &&
		(point.y < 26))
	{
		//按下的是第一个View 的按钮;
		CString temp_cs;
		graphic_view_index = 0;
		CString temp_serial;
		temp_serial.Format(_T("%u"),g_selected_serialnumber);
		WritePrivateProfileStringW(temp_serial,_T("GraphicView"),_T("0"),g_cstring_ini_path);

		int ret_time_scal = 0;
		ret_time_scal = GetPrivateProfileInt(temp_serial,_T("GraphicView0Time"),TIME_THIRTY_MINUTE,g_cstring_ini_path);
		switch(ret_time_scal)
		{
		case TIME_FIVE_MINUTE:
			OnTimebase5minutes();
			break;
		case  TIME_TEN_MINUTE:
			OnTimebase10minutes();
			break;
		case  TIME_THIRTY_MINUTE:
			OnTimebase30minutes();
			break;
		case TIME_ONE_HOUR:
			OnTimebase1hour();
			break;
		case TIME_FOUR_HOUR:
			OnTimebase4hours();
			break;
		case TIME_TWELVE_HOUR:
			OnTimebase12hours();
			break;
		case TIME_ONE_DAY:
			OnTimebase1day();
			break;
		case TIME_FOUR_DAY:
			OnTimebase4days();
			break;
		default:
			break;
		}
		for (int i=0;i<14;i++)
		{
			temp_cs.Format(_T("Static%d_label%d"),graphic_view_index,i+1);
			StaticShow[i] = GetPrivateProfileInt(temp_serial,temp_cs,1,g_cstring_ini_path);
		}
	}
	else if((point.x > 1000) &&
		(point.x <1120) &&
		(point.y > 6) &&
		(point.y < 26))
	{
		//按下的是第二个View 的按钮;
		CString temp_cs;
		graphic_view_index = 1;
		CString temp_serial;
		temp_serial.Format(_T("%u"),g_selected_serialnumber);
		WritePrivateProfileStringW(temp_serial,_T("GraphicView"),_T("1"),g_cstring_ini_path);

		int ret_time_scal = 0;
		ret_time_scal = GetPrivateProfileInt(temp_serial,_T("GraphicView1Time"),TIME_THIRTY_MINUTE,g_cstring_ini_path);
		switch(ret_time_scal)
		{
		case TIME_FIVE_MINUTE:
			OnTimebase5minutes();
			break;
		case  TIME_TEN_MINUTE:
			OnTimebase10minutes();
			break;
		case  TIME_THIRTY_MINUTE:
			OnTimebase30minutes();
			break;
		case TIME_ONE_HOUR:
			OnTimebase1hour();
			break;
		case TIME_FOUR_HOUR:
			OnTimebase4hours();
			break;
		case TIME_TWELVE_HOUR:
			OnTimebase12hours();
			break;
		case TIME_ONE_DAY:
			OnTimebase1day();
			break;
		case TIME_FOUR_DAY:
			OnTimebase4days();
			break;
		default:
			break;
		}
		for (int i=0;i<14;i++)
		{
			temp_cs.Format(_T("Static%d_label%d"),graphic_view_index,i+1);
			StaticShow[i] = GetPrivateProfileInt(temp_serial,temp_cs,1,g_cstring_ini_path);
		}
	}
	else if((point.x > 1150) &&
		(point.x <1270) &&
		(point.y > 6) &&
		(point.y < 26))
	{
		//按下的是第三个View 的按钮;
		CString temp_cs;
		graphic_view_index = 2;
		CString temp_serial;
		temp_serial.Format(_T("%u"),g_selected_serialnumber);
		WritePrivateProfileStringW(temp_serial,_T("GraphicView"),_T("2"),g_cstring_ini_path);

		int ret_time_scal = 0;
		ret_time_scal = GetPrivateProfileInt(temp_serial,_T("GraphicView2Time"),TIME_THIRTY_MINUTE,g_cstring_ini_path);
		switch(ret_time_scal)
		{
		case TIME_FIVE_MINUTE:
			OnTimebase5minutes();
			break;
		case  TIME_TEN_MINUTE:
			OnTimebase10minutes();
			break;
		case  TIME_THIRTY_MINUTE:
			OnTimebase30minutes();
			break;
		case TIME_ONE_HOUR:
			OnTimebase1hour();
			break;
		case TIME_FOUR_HOUR:
			OnTimebase4hours();
			break;
		case TIME_TWELVE_HOUR:
			OnTimebase12hours();
			break;
		case TIME_ONE_DAY:
			OnTimebase1day();
			break;
		case TIME_FOUR_DAY:
			OnTimebase4days();
			break;
		default:
			break;
		}

		for (int i=0;i<14;i++)
		{
			temp_cs.Format(_T("Static%d_label%d"),graphic_view_index,i+1);
			StaticShow[i] = GetPrivateProfileInt(temp_serial,temp_cs,1,g_cstring_ini_path);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}



void CBacnetGraphic::WriteViewInfoIntoConfig()
{
	CString temp_serial;
	CString temp_cs;
	CString view_name_index;
	temp_serial.Format(_T("%u"),g_selected_serialnumber);

	view_name_index.Format(_T("GraphicView%d_Name"),graphic_view_index);
	WritePrivateProfileStringW(temp_serial,view_name_index,grapgic_view_name[graphic_view_index],g_cstring_ini_path);
	for (int i=0;i<14;i++)
	{
		temp_cs.Format(_T("Static%d_label%d"),graphic_view_index,i+1);
		if(StaticShow[i] || (InputLable[i].IsEmpty()))
		{
			WritePrivateProfileStringW(temp_serial,temp_cs,_T("1"),g_cstring_ini_path);
		}
		else
		{
			WritePrivateProfileStringW(temp_serial,temp_cs,_T("0"),g_cstring_ini_path);
		}
	}
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

void CBacnetGraphic::SetYaxisValue(float nlowvalue,float nhighvalue)
{
	if(nhighvalue < nlowvalue)
	{
		nhighvalue = 100;
		nlowvalue = 0;
	}
	else if(nhighvalue == nlowvalue)
	{
		nhighvalue = nhighvalue + 1;
		nlowvalue = nlowvalue - 1;
	}
	m_lowvalue = nlowvalue;
	m_highvalue = nhighvalue;
	y_axis_total_value = nhighvalue - nlowvalue;
}

void CBacnetGraphic::GetXaxisTime(unsigned long &returnstarttime,unsigned long &returnendtime)
{
	//ASSERT(m_endtime > m_starttime);
	returnstarttime = m_starttime;
	returnendtime = m_endtime;
}

void CBacnetGraphic::GetYaxisValue(int &returnlowvalue,int &returnhighvalue)
{
	//ASSERT(m_highvalue > m_lowvalue);
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
	case TIME_FIVE_MINUTE:
		{
			x_axis_total_time = 300;
		}
		break;
	case TIME_TEN_MINUTE:
		{
			x_axis_total_time = 600;
		}
		break;
	case TIME_THIRTY_MINUTE:
		{
			x_axis_total_time = 1800;
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
	case TIME_USER_DEFINE:
		{
		    x_axis_total_time = customer_define_x_time;
		}
		break;
	default:
		break;
	}
	m_onepixeltime =  (float)(x_axis_total_time) / (float)m_X_ASIX_WIDTH;
}

bool CBacnetGraphic::SetDigitalYLabelPos(int ndigital_index)
{
	mDigitalYLabelPoint[ndigital_index].X = m_digitalorignpoint.X - 30;
	mDigitalYLabelPoint[ndigital_index].Y = m_digitalorignpoint.Y + mdigital_count_intervel*ndigital_index  -(mdigital_height);
	return 1;
}

PointF CBacnetGraphic::GetDigitalYLabelPos(int ndigital_index)
{

	return mDigitalYLabelPoint[ndigital_index];
}

bool CBacnetGraphic::DigitalTimeValueToPoint(unsigned long ntime , int nvalue ,PointF &returnpoint,int n_index)
{
	//if(nvalue != 0)
	//{
	//	nvalue = 1;
	//	returnpoint.Y =m_digitalorignpoint.Y + 30;
	//}
	//else
	//	returnpoint.Y = m_digitalorignpoint.Y  + 110;
	if(nvalue != 0)
	{
		nvalue = 1;
		returnpoint.Y =m_digitalorignpoint.Y + mdigital_count_intervel*n_index - (mdigital_height);
	}
	else
		returnpoint.Y = m_digitalorignpoint.Y  + mdigital_count_intervel*n_index ;

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
	float temp_nvalue = nvalue / 1000.00;
//	nvalue = nvalue / 1000;
	//TRACE(_T("value = %d\r\n"),nvalue);
	long delta_time = ntime - m_starttime;


	if((delta_time <0) ||(delta_time > x_axis_total_time))
	{
		
		//returnpoint.X = last_used_point.X;
		//returnpoint.Y = last_used_point.Y;
		return false;
	}
	
	//Calculate Y axis point
	//int delta_value = nvalue - m_lowvalue;
	float delta_value = temp_nvalue - m_lowvalue;
	if((delta_value < 0) ||(delta_value > y_axis_total_value))
	{
		//returnpoint.X = last_used_point.X;
		//returnpoint.Y = last_used_point.Y;
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


void CBacnetGraphic::InitialParameter(int base_time,float y_min_value,float y_max_value)
{


	//if(flag_auto_scroll == false)
	//{
		CString cs_temp_value;
		cs_temp_value.Format(_T("%d"),m_time_monitor_now);
		WritePrivateProfileStringW(_T("Setting"),_T("Graphic_last_time_value"),cs_temp_value,g_cstring_ini_path);
	//}

	if(m_monitor_data.at(monitor_list_line).num_inputs > m_monitor_data.at(monitor_list_line).an_inputs )
	{
		contain_digital = true;
	}
	int temp_get_data_count = m_monitor_data.at(monitor_list_line).num_inputs;
	int temp_monitor_analog_count = m_monitor_data.at(monitor_list_line).an_inputs;

	bool temp_digi = false;
	for (int i=monitor_analog_count;i<temp_get_data_count;i++)
	{
		if(StaticShow[i])
		{
			temp_digi = true;
			break;
		}
	}
	if(temp_digi)
		contain_digital = true;
	else
		contain_digital = false;
	//if((monitor_digital_count == 0)  || (contain_digital == false))
	//if(contain_digital == false)
	//{
	//	Set_XAxis_Length(1200);
	//	Set_YAxis_Length(650);
	//	SetDigital_X_WIDTH(0);
	//	SetDigital_Y_Hight(0);
	//}
	//else
	//{
	//	Set_XAxis_Length(1200);
	//	Set_YAxis_Length(500);
	//	SetDigital_X_WIDTH(1200);
	//	SetDigital_Y_Hight(150);
	//}

	if(contain_digital == false)
	{
		Set_XAxis_Length(1000);
		Set_YAxis_Length(650);
		SetDigital_X_WIDTH(0);
		SetDigital_Y_Hight(0);
	}
	else
	{
		Set_XAxis_Length(1000);
		Set_YAxis_Length(500);
		SetDigital_X_WIDTH(1000);
		//SetDigital_Y_Hight(150);
		SetDigital_Y_Hight(180);
	}


	SetXaxisScale(5);
//	SetYaxisScale(5);
	SetYaxisScale(2);
	SetAnalogOrignPoint(PointF(250,30));
	SetDigitalOrignPoint(PointF(250,560));
	Set_Time_Scale(base_time);





	if((int)(y_max_value) % 5 != 0)
	{
		y_max_value = y_max_value + (5 -(int)y_max_value % 5);
	}



	if((int)(y_min_value) % 5 != 0)
	{
		y_min_value = y_min_value - (int)y_min_value % 5;
	}

	float temp_min = y_min_value /1000 ;
	float temp_max = y_max_value /1000 ;
		if(y_min_value == 0)
			temp_min = 0;



	SetYaxisValue( temp_min ,temp_max );//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SetYaxisValue( (y_min_value* 8)/10000 , (y_max_value * 12)/10000);//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SetYaxisValue( y_min_value/1000 - temp_all*0.2 , y_max_value /1000 + temp_all*0.2);//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CalcOnePixelTime();
	CalcOnePixelValue();
	timestart = m_time_monitor_now - x_axis_total_time;
	TRACE(_T("time from %u"),timestart);

    if (m_time_selected <= TIME_ONE_HOUR)
    {
        timestart = (timestart / 60 + 1) * 60;
    }
    else if (m_time_selected == TIME_FOUR_HOUR)
    {
        timestart = (timestart / 3600 + 1) * 3600;
    }
    else if (m_time_selected == TIME_TWELVE_HOUR)
    {
        timestart = (timestart / 3600 + 1) * 3600;
    }
    else if (m_time_selected == TIME_ONE_DAY)
    {
        timestart = (timestart / 3600 + 1) * 3600;
    }
    else if (m_time_selected == TIME_FOUR_DAY)
    {
        timestart = (timestart / (3600*12)  + 1) * (3600*12);
    }
    
	//TRACE(_T(" to %u\n"),timestart);
	SetXaxisStartTime(timestart);


}


void CBacnetGraphic::PostNcDestroy()
{
	
	flag_continue_thread = false;
	Sleep(400);
	TerminateThread(mythread,2);
	mythread = NULL;
	TerminateThread(updatedatathread,2);
	updatedatathread = NULL;

	CDialogEx::PostNcDestroy();
	delete this;//类对象的销毁   ;
	GraphicWindow = NULL;

	Delete_Ram_Data();
}

//void CBacnetGraphic::Delete_Ram_Data()
void Delete_Ram_Data()
{
	WaitForSingleObject(Point_Mutex,INFINITE);	//防止删除数据的时候  绘图的线程又调用此数据;加锁保护;

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
	

	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_ONE_HOUR);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);


	flag_auto_scroll = true;
	m_time_selected = TIME_ONE_HOUR;
	draw_graphic_finished = false;
    ncontinue_read_data = false;
	//Delete_Ram_Data();
	b_has_create_point = false;

		Save_time_scale(TIME_ONE_HOUR);
}


void CBacnetGraphic::OnTimebase1day()
{
	
	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_ONE_DAY);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);

		Save_time_scale(TIME_ONE_DAY);
	flag_auto_scroll = false;
	m_time_selected = TIME_ONE_DAY;
	draw_graphic_finished = false;
    ncontinue_read_data = false;
	//Delete_Ram_Data();
	b_has_create_point = false;

}



void CBacnetGraphic::OnTimebase10minutes()
{
	

	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_TEN_MINUTE);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);
	m_time_selected = TIME_TEN_MINUTE;
	draw_graphic_finished = false;
	b_has_create_point = false;
    ncontinue_read_data = false;
	Save_time_scale(TIME_TEN_MINUTE);
}

void CBacnetGraphic::Save_time_scale(unsigned int ntime)
{

	CString temp_serial;
	CString temp_index;
	CString temp_cs1;
	temp_cs1.Format(_T("%d"),ntime);
	temp_serial.Format(_T("%u"),g_selected_serialnumber);
	temp_index.Format(_T("GraphicView%dTime"),graphic_view_index);
	WritePrivateProfileStringW(temp_serial,temp_index,temp_cs1,g_cstring_ini_path);
}

void CBacnetGraphic::OnTimebase12hours()
{
	
	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_TWELVE_HOUR);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);


	//flag_auto_scroll = false;
	m_time_selected = TIME_TWELVE_HOUR;
	draw_graphic_finished = false;
	b_has_create_point = false;
    ncontinue_read_data = false;
	Save_time_scale(TIME_TWELVE_HOUR);
}


void CBacnetGraphic::OnTimebase4hours()
{
	
	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_FOUR_HOUR);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);

	//flag_auto_scroll = true;
	m_time_selected = TIME_FOUR_HOUR;
	draw_graphic_finished = false;
	b_has_create_point = false;
    ncontinue_read_data = false;
	Save_time_scale(TIME_FOUR_HOUR);
}


void CBacnetGraphic::OnTimebase4days()
{
	
	//if(draw_graphic_finished == false)	//避免客户 频繁切换数据;
	//{
	//	return;
	//}
	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_FOUR_DAY);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);

	flag_auto_scroll = false;
	m_time_selected = TIME_FOUR_DAY;
	draw_graphic_finished = false;
	//Delete_Ram_Data();
	b_has_create_point = false;
    ncontinue_read_data = false;
	Save_time_scale(TIME_FOUR_DAY);
}



//向大方向规整
int CBacnetGraphic::MaxMinRound(int Data,bool UP)
{
	while (true)
	{
		if (UP)
		{
			if ((Data % 5 == 0) || (Data % 10 == 0))
			{
				return Data;
			}
			else
			{
				Data++;
			}
		}
		else
		{
			if ((Data % 5 == 0) || (Data % 10 == 0))
			{
				return Data;
			}
			else
			{
				Data--;
			}
		}

	}
}

bool CBacnetGraphic::re_calc_max_and_min(int y_max,int y_min,int &ret_y_max,int &ret_y_min,int scale)
{
	y_min = MaxMinRound(y_min, false);
	y_max = MaxMinRound(y_max, true);
	bool temp_time = true;
	while (true)
	{
        if((y_max - y_min) %2 == 0)
		//int temp_range = (y_max - y_min) / 2;
		//if ( temp_range % scale == 0)
		{
			break;
		}
		else
		{	
			if (temp_time)
			{
				y_max += 5;
				temp_time = false;
			}
			else
			{
				y_min -= 5;
				temp_time = true;
			}
		}
	}
	ret_y_max = y_max;
	ret_y_min = y_min;
	return 1;
}



void CBacnetGraphic::Reset_X_Y_Parameter()
{
	int temp_number_of_inputs = m_monitor_data.at(monitor_list_line).num_inputs;
	int temp_number_of_analog = m_monitor_data.at(monitor_list_line).an_inputs;
	int temp_number_of_digital = m_monitor_data.at(monitor_list_line).num_inputs - temp_number_of_analog;
	get_data_count = temp_number_of_inputs;
	monitor_analog_count = temp_number_of_analog;
	monitor_digital_count = temp_number_of_digital;

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
	int bef_max;
	int bef_min;
	//total_y_max_value = 189000;
	//total_y_min_value = 123000;
	//bef_max = total_y_max_value / 1000 + 1;
	//bef_min = total_y_min_value /1000  - 1;	
    bef_max = total_y_max_value / 1000 ;
    bef_min = total_y_min_value / 1000 ;

	int temp_max;
	int temp_min;

	re_calc_max_and_min(bef_max,bef_min,temp_max,temp_min);
	total_y_max_value = temp_max * 1000;

	if((total_y_min_value >= 0) && (total_y_min_value < 5000))
		total_y_min_value = 0;
	else
		total_y_min_value = temp_min * 1000;


#if 0
	//针对 0 - 100 范围的Y轴 做出特殊处理;
	unsigned int delta_value = 0;
	delta_value = total_y_max_value - total_y_min_value;
	 temp_max = total_y_max_value;
	 temp_min = total_y_min_value;
	if(delta_value == 0)
	{
		total_y_max_value = total_y_min_value + 10;
	}
	else if((total_y_min_value >= 0) && (total_y_min_value <= 100000) && (total_y_max_value >= 0) && (total_y_max_value <= 100000))
	{
		if(delta_value < 1000)
		{
			//差值小于1;
			total_y_min_value = total_y_min_value / 1000;
			total_y_min_value = total_y_min_value * 1000;

			total_y_max_value = total_y_min_value + 2000;

			if((total_y_max_value - 1000) >= temp_max)
				total_y_max_value = total_y_max_value - 1000;

		}
		else if(delta_value < 10000)
		{
			//差值小于10;
			total_y_min_value = total_y_min_value / 10000;
			total_y_min_value = total_y_min_value * 10000;
			total_y_max_value = total_y_min_value + 10000;
			while(total_y_max_value < temp_max)
			{
				total_y_max_value = total_y_max_value + 1000;
			}
		}
		else if(delta_value < 20000)
		{
			//差值小于20;
			total_y_min_value = total_y_min_value / 20000;
			total_y_min_value = total_y_min_value * 20000;

			total_y_max_value = total_y_min_value + 20000;
			while(total_y_max_value < temp_max)
			{
				total_y_max_value = total_y_max_value + 1000;
			}
		}
		else if(delta_value < 40000)
		{
			//差值小于40;
			total_y_min_value = total_y_min_value / 40000;
			total_y_min_value = total_y_min_value * 40000;

			total_y_max_value = total_y_min_value + 40000;
			while(total_y_max_value < temp_max)
			{
				total_y_max_value = total_y_max_value + 1000;
			}
		}
		else
		{
			total_y_min_value = 0;
			total_y_max_value = 100000;
		}

	}
	//暂时给郭少用
	if((total_y_max_value > 100000) && (total_y_max_value < 150000))
	{
		if(total_y_min_value > 0)
			total_y_min_value = 0;
		total_y_max_value = 150000;

	}
#endif
	switch(m_time_selected)
	{
	case TIME_ONE_MINUTE :
		{
			InitialParameter(TIME_ONE_MINUTE,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_FIVE_MINUTE :
		{
			InitialParameter(TIME_FIVE_MINUTE,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_TEN_MINUTE:
		{
			InitialParameter(TIME_TEN_MINUTE,total_y_min_value,total_y_max_value);
		}
		break;
	case TIME_THIRTY_MINUTE:
		{
			InitialParameter(TIME_THIRTY_MINUTE,total_y_min_value,total_y_max_value);
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
	case TIME_USER_DEFINE:
		{
			InitialParameter(TIME_USER_DEFINE,total_y_min_value,total_y_max_value);
		}
		break;
	default:
		InitialParameter(TIME_FOUR_HOUR,0,100);
		break;
	}


}


void CBacnetGraphic::OnGraphicLeft()
{
	

	draw_graphic_finished = false;

	if(m_time_selected == TIME_USER_DEFINE)
	{
		//客户自定义的数据段时 ，左移 ;
		customer_end_time = customer_end_time - x_axis_total_time;
		customer_start_time = customer_start_time - x_axis_total_time;
		m_time_monitor_now = customer_end_time;
	}
	else
	{
		m_time_monitor_now =m_time_monitor_now -  x_axis_total_time/2;
	}
    flag_auto_scroll = false;
    ncontinue_read_data = false;
	draw_graphic_finished = false;
	//Delete_Ram_Data();
	b_has_create_point = false;

}


void CBacnetGraphic::OnGraphicRight()
{
	

	draw_graphic_finished = false;

	if(m_time_selected == TIME_USER_DEFINE)
	{
		//客户自定义的数据段时 ，右移 ;
		customer_end_time = customer_end_time + x_axis_total_time;
		customer_start_time = customer_start_time + x_axis_total_time;
		m_time_monitor_now = customer_end_time;
        ncontinue_read_data = false;
	}
	else
	{
		CTime temp_time_now = CTime::GetCurrentTime();
		unsigned long temp_cur_long_time = temp_time_now.GetTime();
        if (temp_cur_long_time > (m_time_monitor_now + x_axis_total_time/2))
        {
            ncontinue_read_data = false;
            m_time_monitor_now = m_time_monitor_now + x_axis_total_time/2;
        }
		else
		{
			m_time_monitor_now = temp_cur_long_time;
			//MessageBox(_T("No more data!"));
            flag_auto_scroll = true;
		}
	}

		draw_graphic_finished = false;
		//Delete_Ram_Data();
		b_has_create_point = false;
}


void CBacnetGraphic::OnZoomin()
{
	
	while(draw_graphic_finished == false)
	{
		Sleep(1);
	}

	draw_graphic_finished = false;
	if(m_time_selected > TIME_FIVE_MINUTE)
	{
        CString temp_cs1;
		m_time_selected = m_time_selected - 1;
        temp_cs1.Format(_T("%d"), m_time_selected);
        WritePrivateProfileString(_T("Setting"), _T("GraphicScaleType"), temp_cs1, g_cstring_ini_path); //在变更后记录时间刻度,下次打开还原为此刻度.
		Delete_Ram_Data();
		draw_graphic_finished = false;
		b_has_create_point = false;
        ncontinue_read_data = false;
	}
	else
	{
		MessageBox(_T("The smallest time scale is five minutes"));
		m_time_selected = TIME_FIVE_MINUTE;
		draw_graphic_finished = false;
		Delete_Ram_Data();
		b_has_create_point = false;
	}

}


void CBacnetGraphic::OnZoomout()
{
	
	while(draw_graphic_finished == false)
	{
		Sleep(1);
	}

	draw_graphic_finished = false;
	if(m_time_selected < TIME_FOUR_DAY)
	{
        CString temp_cs1;
        m_time_selected = m_time_selected + 1;
        temp_cs1.Format(_T("%d"), m_time_selected);
        WritePrivateProfileString(_T("Setting"), _T("GraphicScaleType"), temp_cs1, g_cstring_ini_path);//在变更后记录时间刻度,下次打开还原为此刻度.
		draw_graphic_finished = false;
        ncontinue_read_data = false;
		Delete_Ram_Data();
		b_has_create_point = false;
	}
	else
	{
		MessageBox(_T("Max is four days"));
		m_time_selected = TIME_FOUR_DAY;
		draw_graphic_finished = false;
		Delete_Ram_Data();
		b_has_create_point = false;
	}
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
					if(temp_range == 0)
					{
						if(m_monitor_data.at(monitor_list_line).inputs[i].number < BAC_OUTPUT_ITEM_COUNT)
						{
							temp_range = m_Output_data.at(m_monitor_data.at(monitor_list_line).inputs[i].number).range;
							if((temp_range < (sizeof(Output_Analog_Units_Show)/sizeof(Output_Analog_Units_Show[0]))) && (temp_range !=0))
							{
								InputUnit[i] =	Output_Analog_Units_Show[temp_range];
							}
							else
							{
								InputUnit[i].Empty();
							}
						}
						else
							InputUnit[i].Empty();
					}
					else if(temp_range < (sizeof(Output_Analog_Units_Show)/sizeof(Output_Analog_Units_Show[0])))
						InputUnit[i] =	Output_Analog_Units_Show[temp_range];
					else
						InputUnit[i].Empty();
				}
				else if(m_monitor_data.at(monitor_list_line).inputs[i].point_type == ENUM_IN + 1)
				{
					if(temp_range == 0)
					{
						if(m_monitor_data.at(monitor_list_line).inputs[i].number < BAC_INPUT_ITEM_COUNT)
						{
							temp_range = m_Input_data.at(m_monitor_data.at(monitor_list_line).inputs[i].number).range;
							if((temp_range < (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0]))) && (temp_range !=0))
							{
								InputUnit[i] =	Input_List_Analog_Units[temp_range];
							}
							else
							{
								InputUnit[i].Empty();
							}
						}
						else
							InputUnit[i].Empty();
					}
					else if(temp_range < (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
						InputUnit[i] =	Input_List_Analog_Units[temp_range];
					else
						InputUnit[i].Empty();
				}
				else if(m_monitor_data.at(monitor_list_line).inputs[i].point_type == ENUM_VAR + 1)
				{
					if(temp_range == 0)
					{
						if(m_monitor_data.at(monitor_list_line).inputs[i].number < BAC_VARIABLE_ITEM_COUNT)
						{
							temp_range = m_Variable_data.at(m_monitor_data.at(monitor_list_line).inputs[i].number).range;
							if((temp_range < (sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))) && (temp_range !=0))
							{
								InputUnit[i] =	Variable_Analog_Units_Array[temp_range];
							}
							else
							{
								InputUnit[i].Empty();
							}
						}
						else
							InputUnit[i].Empty();
					}
					else if(temp_range < (sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0])))
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
			{
				if(temp_range != 0)
					InputUnit[i] =	Digital_Units_Array[temp_range];
				else
					InputUnit[i].Empty();
			}
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
			HH_HELP_CONTEXT, IDH_TOPIC_6_1_GRAPHIC_SCREENS);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}

void CBacnetGraphic::OnTimebaseCustomerdefine()
{
	
	CBacnetGraphicSetting dlg;
	dlg.DoModal();
	if(use_customer_time)
	{
        flag_auto_scroll = false;
        ncontinue_read_data = false;
		//int timeout_count =0;
		while(draw_graphic_finished == false)
		{
			//timeout_count ++ ;
			Sleep(10);
			//if(timeout_count > 1000)
			//	return;
		}



		draw_graphic_finished = false;
		m_time_selected = TIME_USER_DEFINE;
		m_time_monitor_now = customer_end_time;
		draw_graphic_finished = false;
		Delete_Ram_Data();
		b_has_create_point = false;
	}
}


void CBacnetGraphic::OnTimebase5minutes()
{
	
	m_time_selected = TIME_FIVE_MINUTE;
	draw_graphic_finished = false;
	b_has_create_point = false;
    ncontinue_read_data = false;
		Save_time_scale(TIME_FIVE_MINUTE);
}


void CBacnetGraphic::OnTimebase30minutes()
{
	
	CString temp_cs1;
	temp_cs1.Format(_T("%d"),TIME_THIRTY_MINUTE);
	WritePrivateProfileString(_T("Setting"),_T("GraphicScaleType"),temp_cs1,g_cstring_ini_path);

	m_time_selected = TIME_THIRTY_MINUTE;
	draw_graphic_finished = false;
	b_has_create_point = false;
    ncontinue_read_data = false;
		Save_time_scale(TIME_THIRTY_MINUTE);
}


void CBacnetGraphic::OnRButtonDown(UINT nFlags, CPoint point)
{
	 
	int temp_y_value = 0;
	if(contain_digital )
		temp_y_value = 500;
	else
		temp_y_value = 650;
	//if((point.x > 252) &&
	//	(point.x < 1445) &&
	//	(point.y > 32) &&
	//	(point.y <679))
	if((point.x > 250) &&
		(point.x < 250 + 1000) &&
		(point.y > 30) &&
		(point.y < 30 + temp_y_value))
	{
		b_show_value_line = true;
		RclickValueTime.X = point.x;
		RclickValueTime.Y = point.y;
	}
	else
	{
		b_show_value_line = false;
		RclickValueTime.X = 0;
		RclickValueTime.Y = 0;
	}

	if((point.x > 850) &&
		(point.x <970) &&
		(point.y > 6) &&
		(point.y < 26))
	{
		//按下的是第一个View 的按钮;
		bacnet_message_input_title.Format(_T("Please input a new name "));
		CBacnetMessageInput temp_dlg;
		temp_dlg.DoModal();
		if(!bacnet_message_return_string.IsEmpty())
		{
			grapgic_view_name[0] = bacnet_message_return_string;
			WriteViewInfoIntoConfig();
		}
	}
	else if((point.x > 1000) &&
		(point.x <1120) &&
		(point.y > 6) &&
		(point.y < 26))
	{
		//按下的是第二个View 的按钮;
		bacnet_message_input_title.Format(_T("Please input a new name "));
		CBacnetMessageInput temp_dlg;
		temp_dlg.DoModal();
		if(!bacnet_message_return_string.IsEmpty())
		{
			grapgic_view_name[1] = bacnet_message_return_string;
			WriteViewInfoIntoConfig();
		}
		
	}
	else if((point.x > 1150) &&
		(point.x <1270) &&
		(point.y > 6) &&
		(point.y < 26))
	{
		//按下的是第三个View 的按钮;
		bacnet_message_input_title.Format(_T("Please input a new name "));
		CBacnetMessageInput temp_dlg;
		temp_dlg.DoModal();
		if(!bacnet_message_return_string.IsEmpty())
		{
			grapgic_view_name[2] = bacnet_message_return_string;
			WriteViewInfoIntoConfig();
		}
	}


	CDialogEx::OnRButtonDown(nFlags, point);
}


void CBacnetGraphic::OnMouseMove(UINT nFlags, CPoint point)
{
	 
	if(b_show_value_line)
	{
		int temp_y_value = 0;
		if(contain_digital )
			temp_y_value = 500;
		else
			temp_y_value = 650;

		if((point.x > 250) &&
			(point.x < 250 + 1000) &&
			(point.y > 30) &&
			(point.y < 30 + temp_y_value))
		{

			RclickValueTime.X = point.x;
			RclickValueTime.Y = point.y;
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
