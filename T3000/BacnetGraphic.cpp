// BacnetGraphic.cpp : implementation file
// BacnetGraphic Code by Fance Du 2013 11 05

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

CRect Static_Num_Rect[15];
bool flag_continue_thread = true;
HWND myhWnd;
RECT myRect;
HANDLE mythread;
HDC hMemDC;
HBITMAP hBmp;
HDC gloab_hdc;

int scale_type;
DWORD WINAPI MyThreadPro(LPVOID lPvoid);

IMPLEMENT_DYNAMIC(CBacnetGraphic, CDialogEx)

CBacnetGraphic::CBacnetGraphic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetGraphic::IDD, pParent)
{

}

CBacnetGraphic::~CBacnetGraphic()
{
}

void CBacnetGraphic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_1, m_graphic_st_1);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_2, m_graphic_st_2);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_3, m_graphic_st_3);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_4, m_graphic_st_4);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_5, m_graphic_st_5);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_6, m_graphic_st_6);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_7, m_graphic_st_7);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_8, m_graphic_st_8);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_9, m_graphic_st_9);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_A, m_graphic_st_a);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_B, m_graphic_st_b);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_C, m_graphic_st_c);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_D, m_graphic_st_d);
	DDX_Control(pDX, IDC_STATIC_GRAPHIC_E, m_graphic_st_e);
}

const int WM_FRESH_STATIC = 1069;
BEGIN_MESSAGE_MAP(CBacnetGraphic, CDialogEx)
	ON_MESSAGE(WM_FRESH_STATIC,Fresh_Static_Function)
	ON_WM_TIMER()
//	ON_STN_CLICKED(IDC_STATIC_GRAPHIC_A, &CBacnetGraphic::OnStnClickedStaticGraphicA)
ON_STN_CLICKED(IDC_STATIC_GRAPHIC_1, &CBacnetGraphic::OnStnClickedStaticGraphicA)
END_MESSAGE_MAP()


// CBacnetGraphic message handlers
unsigned long time_now;
unsigned long old_early_time;

int window_width;
int window_hight;
HWND test_hwnd;
BOOL CBacnetGraphic::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	srand(NULL);

	scale_type = _1_hour;
	time_now = time(NULL);
	old_early_time = time_now - 3600;
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
	mythread=CreateThread(NULL,NULL,MyThreadPro,this->m_hWnd,NULL,NULL);
	CloseHandle(mythread);
	InitDC();

	test_hwnd = this->m_hWnd;
	Init_14_Static();

	
	CRect Graphic_Window_Rect;
	GetWindowRect(Graphic_Window_Rect);
	window_width = Graphic_Window_Rect.right - Graphic_Window_Rect.left;
	window_hight = Graphic_Window_Rect.bottom - Graphic_Window_Rect.top;

	CRect Temp_Rect[15];
	m_graphic_st_1.GetWindowRect(Temp_Rect[1]);	ScreenToClient(Temp_Rect[1]);
	m_graphic_st_2.GetWindowRect(Temp_Rect[2]);	ScreenToClient(Temp_Rect[2]);
	m_graphic_st_3.GetWindowRect(Temp_Rect[3]);	ScreenToClient(Temp_Rect[3]);
	m_graphic_st_4.GetWindowRect(Temp_Rect[4]);	ScreenToClient(Temp_Rect[4]);
	m_graphic_st_5.GetWindowRect(Temp_Rect[5]);	ScreenToClient(Temp_Rect[5]);
	m_graphic_st_6.GetWindowRect(Temp_Rect[6]);	ScreenToClient(Temp_Rect[6]);
	m_graphic_st_7.GetWindowRect(Temp_Rect[7]);	ScreenToClient(Temp_Rect[7]);
	m_graphic_st_8.GetWindowRect(Temp_Rect[8]);	ScreenToClient(Temp_Rect[8]);
	m_graphic_st_9.GetWindowRect(Temp_Rect[9]);	ScreenToClient(Temp_Rect[9]);
	m_graphic_st_a.GetWindowRect(Temp_Rect[10]);ScreenToClient(Temp_Rect[10]);
	m_graphic_st_b.GetWindowRect(Temp_Rect[11]);ScreenToClient(Temp_Rect[11]);
	m_graphic_st_c.GetWindowRect(Temp_Rect[12]);ScreenToClient(Temp_Rect[12]);
	m_graphic_st_d.GetWindowRect(Temp_Rect[13]);ScreenToClient(Temp_Rect[13]);
	m_graphic_st_e.GetWindowRect(Temp_Rect[14]);ScreenToClient(Temp_Rect[14]);

	for(int i=1;i<=14;i++)
	{
		Static_Num_Rect[i].left = Temp_Rect[i].right + 5;
		Static_Num_Rect[i].top = Temp_Rect[i].top + 1;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetGraphic::Init_14_Static()
{
	m_graphic_st_1.SetWindowTextW(_T("AHU1HSP"));
	m_graphic_st_1.textColor(RGB(0,0,0));
	m_graphic_st_1.bkColor(Static_Color[1]);
	m_graphic_st_1.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_2.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_2.textColor(RGB(0,0,0));
	m_graphic_st_2.bkColor(Static_Color[2]);
	m_graphic_st_2.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_3.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_3.textColor(RGB(0,0,0));
	m_graphic_st_3.bkColor(Static_Color[3]);
	m_graphic_st_3.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_4.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_4.textColor(RGB(0,0,0));
	m_graphic_st_4.bkColor(Static_Color[4]);
	m_graphic_st_4.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_5.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_5.textColor(RGB(0,0,0));
	m_graphic_st_5.bkColor(Static_Color[5]);
	m_graphic_st_5.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_6.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_6.textColor(RGB(0,0,0));
	m_graphic_st_6.bkColor(Static_Color[6]);
	m_graphic_st_6.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_7.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_7.textColor(RGB(0,0,0));
	m_graphic_st_7.bkColor(Static_Color[7]);
	m_graphic_st_7.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_8.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_8.textColor(RGB(0,0,0));
	m_graphic_st_8.bkColor(Static_Color[8]);
	m_graphic_st_8.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_9.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_9.textColor(RGB(0,0,0));
	m_graphic_st_9.bkColor(Static_Color[9]);
	m_graphic_st_9.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_a.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_a.textColor(RGB(0,0,0));
	m_graphic_st_a.bkColor(Static_Color[10]);
	m_graphic_st_a.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_b.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_b.textColor(RGB(0,0,0));
	m_graphic_st_b.bkColor(Static_Color[11]);
	m_graphic_st_b.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_c.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_c.textColor(RGB(0,0,0));
	m_graphic_st_c.bkColor(Static_Color[12]);
	m_graphic_st_c.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_d.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_d.textColor(RGB(0,0,0));
	m_graphic_st_d.bkColor(Static_Color[13]);
	m_graphic_st_d.setFont(22,15,NULL,_T("Arial"));

	m_graphic_st_e.SetWindowTextW(_T("AHU1SAT"));
	m_graphic_st_e.textColor(RGB(0,0,0));
	m_graphic_st_e.bkColor(Static_Color[14]);
	m_graphic_st_e.setFont(22,15,NULL,_T("Arial"));


//	SetTimer(1,1,NULL);
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



void MyPaint_Mem(HDC my_hdc);
CPointItem *pTempItem = NULL;
CPointItem *pPrevItem = NULL;
CPointItem	*m_pFirstItem=NULL;
CPointItem  *p_temp_item=NULL;
MyPoint temppoint={0,0};
int nindex=0;
int Total_count=0;
const int line_interval = 30;
static int m_interval = 0;
static int runonce=0;

static int y_line_scale = 0;	//y轴有几个刻度;
static int x_line_scale = 0;	//x轴有几个刻度;


void Get_Line_Point()
{

	for (int i=0;i<60;i++)	//tEST
	{
		//Monitor_Input__Data[monitor_list_line][]
		for (int j=0;j<14;j++)
		{
			if(my_input_info[j].be_record)	//只有要求记录的时候才去比较这些数据;
			{
				my_input_info[j].use_own_scale = false;
				if(my_input_info[j].Max_Value<Monitor_Input__Data[j][i])
					my_input_info[j].Max_Value = Monitor_Input__Data[j][i];
				if(my_input_info[j].Min_Value > Monitor_Input__Data[j][i])
					my_input_info[j].Min_Value = Monitor_Input__Data[j][i];

				if(Max_Scale_value < my_input_info[j].Max_Value)
					Max_Scale_value = my_input_info[j].Max_Value;
				if(Min_Scale_value > my_input_info[j].Min_Value)
					Min_Scale_value = my_input_info[j].Min_Value;
			}
		}
		
	}
	Min_Scale_value = 12;
	Max_Scale_value = 123;
	if(Min_Scale_value>0)
		Min_Scale_value = 0;
	int temp = abs(Min_Scale_value);
	Min_Scale_value =0 - (abs(Min_Scale_value) / 5) *5;

	Max_Scale_value = (Max_Scale_value/5)*5;

	Total_SCALE = Max_Scale_value - Min_Scale_value;

	for (int i=0;i<60;i++)	//tEST
	//for (int i=0;i<m_monitor_block.index;i++)
	{

		

		int interval_time;
		interval_time = m_monitor_block.hour_interval_time*3600 + m_monitor_block.minute_interval_time*60+ m_monitor_block.second_interval_time;
		pTempItem = new CPointItem();
		MyPoint mytemppoint={0,0};
		// old_early_time	

		//For debug
#if 1
		m_monitor_block.start_time = time_now - 3600;
		interval_time = 60;
#endif
	//	float aaaa = (float(m_monitor_block.start_time + i*(interval_time) - old_early_time) /3600);
		mytemppoint.x = (float(m_monitor_block.start_time + i*(interval_time) - old_early_time) /3600) *X_WIDTH + X_ORIGIN;

	//	Monitor_Input__Data[0][0][i]
	//	mytemppoint.y = Y_ORIGIN + Y_HIGHT - ((rand()%2)*10 + 100);
			mytemppoint.y = Y_ORIGIN + Y_HIGHT - (float(rand()%Max_Scale_value))/Total_SCALE *Y_HIGHT ;
			if(mytemppoint.y > Y_ORIGIN + Y_HIGHT)
				mytemppoint.y = Y_ORIGIN + Y_HIGHT;
		pTempItem->SetPoint(mytemppoint);

		if(pPrevItem != NULL)
			pPrevItem->SetNext(pTempItem);
		pPrevItem = pTempItem;

		if(m_pFirstItem == NULL)
		{
			m_pFirstItem = pTempItem;
			m_pFirstItem->SetIndex(0);
		}
	}
}

DWORD WINAPI MyThreadPro(LPVOID lPvoid)
{
	HWND hWnd = (HWND)lPvoid;
	int nCpuPersent =0;
	int point_number = 0;
	y_line_scale = 5;
	x_line_scale = 6;

	CBacnetGraphic * mparent = (CBacnetGraphic *)lPvoid;

	static bool test___ = false ;
	while(flag_continue_thread)
	{
		if(!test___)
		{
			test___ = true;
			Get_Line_Point();
		}
		



		MyPaint_Mem(hMemDC);
		BitBlt(gloab_hdc,0,0, myRect.right, myRect.bottom, hMemDC, 0, 0, SRCCOPY);//将绘制完成的内存位图贴到的Picture空间对象中;
		PostMessage(test_hwnd, WM_FRESH_STATIC,0,0);	//当重绘了之后，控件会被重绘的画布遮盖住，所以发送消息让控件再次刷新;
		Sleep(3000);
	}
	return 0;
}


LRESULT CBacnetGraphic::Fresh_Static_Function(WPARAM wParam,LPARAM lParam)
{
	m_graphic_st_1.Invalidate(TRUE);
	m_graphic_st_2.Invalidate(TRUE);
	m_graphic_st_3.Invalidate(TRUE);
	m_graphic_st_4.Invalidate(TRUE);
	m_graphic_st_5.Invalidate(TRUE);
	m_graphic_st_6.Invalidate(TRUE);
	m_graphic_st_7.Invalidate(TRUE);
	m_graphic_st_8.Invalidate(TRUE);
	m_graphic_st_9.Invalidate(TRUE);
	m_graphic_st_a.Invalidate(TRUE);
	m_graphic_st_b.Invalidate(TRUE);
	m_graphic_st_c.Invalidate(TRUE);
	m_graphic_st_d.Invalidate(TRUE);
	m_graphic_st_e.Invalidate(TRUE);
	return 0;
}

void MyPaint_Mem(HDC my_hdc)
{
	Graphics *mygraphics;
	mygraphics = new Graphics(my_hdc);
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	Pen *myRectangle_pen;
	Pen * my_inline_pen;

	Pen * CurePen;

	SolidBrush *BlackBrush;
	

	myRectangle_pen = new Pen(Color(255,0,255,255));
	my_inline_pen = new Pen(Color(255,220,220,220));

	REAL dashValues[2] = {5, 5};
	//Pen blackPen(Color(255, 0, 0, 0), 5);
	my_inline_pen->SetDashPattern(dashValues, 2);


	CurePen = new Pen(Graphic_Color[1],3.0f);
	PointF      pointF(0, 0);

	BlackBrush =new SolidBrush(Color(255,0,0,0));
	mygraphics->FillRectangle(BlackBrush,X_ORIGIN,Y_ORIGIN,X_WIDTH,Y_HIGHT);
	mygraphics->DrawRectangle(myRectangle_pen,X_ORIGIN,Y_ORIGIN,X_WIDTH,Y_HIGHT);

	SolidBrush *Static_blackground_Brush;
	Pen *mystaticRectangle_pen;
	mystaticRectangle_pen = new Pen(Color(255,0,0,0),2.0f);
	Static_blackground_Brush =new SolidBrush(Color(255,187,187,187));
	mygraphics->FillRectangle(Static_blackground_Brush,0,window_hight - 120,window_width,120);
	mygraphics->DrawRectangle(mystaticRectangle_pen,2,window_hight - 110,window_width-15,110 -30);

	SolidBrush  time_brush(Color(255, 225, 225, 225));
	FontFamily  fontFamily(_T("Times New Roman"));
	Gdiplus::Font        time_font(&fontFamily, 18, FontStyleRegular, UnitPixel);
	for(int i=0;i<x_line_scale;i++)				//画网格线
	{
		mygraphics->DrawLine(my_inline_pen,X_ORIGIN+(X_WIDTH/x_line_scale)*(i+1),Y_ORIGIN,X_ORIGIN+(X_WIDTH/x_line_scale)*(i+1),Y_ORIGIN + Y_HIGHT);
		CString strTime ;
		wchar_t temp_char[200];
		//time_t test = old_early_time;
		//CTime timeTest(test);
		time_t test ;
		CTime timeTest;

		switch(scale_type)
		{
		case _6_min:
			break;
		case _1_hour:
			test = old_early_time + i*600;

			timeTest = test ;

			strTime = timeTest.Format("%Y/%m/%d\r\n %H:%M:%S");
			pointF.X = X_ORIGIN - 40 + i*(X_WIDTH/x_line_scale);
			pointF.Y = Y_ORIGIN+Y_HIGHT + 10;
			mygraphics->DrawString(strTime, -1, &time_font, pointF, &time_brush);
			//old_early_time
			break;
		case _4_hour:
			break;
		case _12_hour:
			break;
		case _1_day:
			break;
		}

	}
	SolidBrush  unit_brush(Graphic_Color[1]);
	FontFamily  UnitfontFamily(_T("Times New Roman"));
	Gdiplus::Font        unitfont(&UnitfontFamily, 18, FontStyleRegular, UnitPixel);
	for(int i=0;i<=y_line_scale;i++)				//画网格线
	{
		CString Unit_value;
		if(i!=y_line_scale)
		mygraphics->DrawLine(my_inline_pen,X_ORIGIN,Y_ORIGIN+(Y_HIGHT/y_line_scale)*(1+i),X_WIDTH + X_ORIGIN,Y_ORIGIN+(Y_HIGHT/y_line_scale)*(1+i));

		if(i!=y_line_scale)
		Unit_value.Format(_T("%d"),(Total_SCALE/y_line_scale)*(y_line_scale-i));// = timeTest.Format("%Y/%m/%d\r\n %H:%M:%S");
		else
		Unit_value.Format(_T("%d"),Min_Scale_value);
		pointF.X = X_ORIGIN - 30;
		pointF.Y = Y_ORIGIN+ i*(Y_HIGHT/y_line_scale);
		mygraphics->DrawString(Unit_value, -1, &unitfont, pointF, &unit_brush);
		//swprintf_s(temp_char,200,L"%d",i*5);
		//mygraphics->DrawString(temp_char, -1, &font, pointF, &brush);

	}
	for (int i=1;i<=14;i++)
	{
		CString temp_item;
		temp_item.Format(_T("%x"),i);
		temp_item = temp_item.MakeUpper();

		SolidBrush  static_item_brush(Graphic_Color[i]);
		FontFamily  UnitfontFamily(_T("Arial Black"));
		Gdiplus::Font        unitfont(&UnitfontFamily, 22, FontStyleRegular, UnitPixel);
		pointF.X = Static_Num_Rect[i].left;
		pointF.Y = Static_Num_Rect[i].top;
		mygraphics->DrawString(temp_item, -1, &unitfont, pointF, &static_item_brush);
	}
	

	CPointItem *first_item=NULL,*second_item=NULL,*third_item=NULL;
	first_item = m_pFirstItem;
	//********************************************
	//直线
	for (int i=0;i<60;i=i+1)
	{
		second_item=first_item->GetNext();
		if(second_item==NULL)
			break;
		CurePen->SetStartCap(LineCapArrowAnchor);
		CurePen->SetEndCap(LineCapRoundAnchor);
		mygraphics->DrawLine(CurePen,first_item->GetPoint().x,first_item->GetPoint().y,second_item->GetPoint().x,second_item->GetPoint().y);
		first_item = second_item;

	}
	//********************************************

#if 0
	for (int i=0;i<60;i=i+2)
	//for (int i=0;i<m_monitor_block.index - 1;i=i+2)	//画贝塞尔曲线，使得线条平滑;3个点一画;		TEST
	//for (int i=0;i<Total_count-1;i=i+2)	//画贝塞尔曲线，使得线条平滑;3个点一画;
	{
		second_item=first_item->GetNext();
		third_item = second_item->GetNext();
		if(second_item==NULL)
			break;
		if(third_item == NULL)
		{
			first_item = second_item;
			break;
		}
		//mygraphics->DrawCurve(CurePen,, 3, 1.5f);
		//取3个点给画曲线的GDI函数;
		Point myPointArray[] =
		{Point(first_item->GetPoint().x,first_item->GetPoint().y),
		Point(second_item->GetPoint().x,second_item->GetPoint().y),
		Point(third_item->GetPoint().x,third_item->GetPoint().y)};

		CurePen->SetStartCap(LineCapArrowAnchor);
		CurePen->SetEndCap(LineCapRoundAnchor);


			mygraphics->DrawCurve(CurePen,myPointArray, 3, 1.0f);
			first_item = third_item;

		
	}
#endif

	delete CurePen;
	delete mygraphics;
	delete myRectangle_pen;
	delete my_inline_pen;
	delete BlackBrush;
}
#if 0
DWORD WINAPI MyThreadPro(LPVOID lPvoid)
{
	HWND hWnd = (HWND)lPvoid;
	int nCpuPersent =0;

	int point_number = 0;

	while(flag_continue_thread)
	{
		pTempItem = new CPointItem();

		temppoint.x=1300;

		nCpuPersent=rand()%20+20;//GetCPUpercent();
		if(nCpuPersent<0)
			nCpuPersent=0;
		temppoint.y = 610 - nCpuPersent*6;
		pTempItem->SetPoint(temppoint);
		pTempItem->SetCPUPersent(nCpuPersent);
		//pTempItem->SetIndex(0);



	/*	if(temppoint.y>610)
			MessageBox(myhWnd,L"111111111",L"ASD",NULL);*/


		if(pPrevItem != NULL)
			pPrevItem->SetNext(pTempItem);
		pPrevItem = pTempItem;

		if(m_pFirstItem == NULL)
		{
			m_pFirstItem = pTempItem;
			m_pFirstItem->SetIndex(0);
		}
		p_temp_item = m_pFirstItem;
		nindex=0;
		Total_count=0;
		while(p_temp_item->GetNext()!=NULL)	//每次重绘都将各个点的 X值向左平移 6个像素;
		{
			nindex ++;
			if(nindex>Total_count)
			{
				Total_count = nindex;
			}
			MyPoint temp1;
			temp1 =	p_temp_item->GetPoint();
			temp1.x = temp1.x - 6;

			if(temp1.x>=100)			//如果超出左边界 则删除此节点;
			{
				p_temp_item->SetPoint(temp1);
				p_temp_item =p_temp_item->GetNext();
				p_temp_item->SetIndex(nindex);
			}
			else
			{
				m_pFirstItem=p_temp_item->GetNext();
				delete p_temp_item;
				p_temp_item = m_pFirstItem;

				m_pFirstItem=p_temp_item->GetNext();//这里要删除2个节点，否则那个曲线会一直在变化;
				delete p_temp_item;
				p_temp_item = m_pFirstItem;
			}
		}

		
		MyPaint_Mem(hMemDC);	
	
		//PostMessage(hWnd,WM_PAINT,0,0);
		if(point_number++>100)
		{
			
			BitBlt(gloab_hdc,0,0, myRect.right, myRect.bottom, hMemDC, 0, 0, SRCCOPY);//将绘制完成的内存位图贴到的Picture空间对象中;
			while(1)
			{
				Sleep(1000);
			}
		}

	}

	return 0;
}
void MyPaint_Mem(HDC my_hdc)
{

	Graphics *mygraphics;
	mygraphics = new Graphics(my_hdc);
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	Pen *myRectangle_pen;
	Pen * my_inline_pen;

	Pen * CurePen;

	SolidBrush *BlackBrush;


	myRectangle_pen = new Pen(Color(255,0,255,255));
	my_inline_pen = new Pen(Color(255,0,128,64));

	CurePen = new Pen(Color(255,0,255,0));

	m_interval = (m_interval++)%5;	//让其向左平移;


	if(runonce < 2)				//初始化时画背景图案;
	{
		runonce ++;
		//Image myImage(L"texture.BMP");
		//TextureBrush myTextureBrush(&myImage);
		//RECT newrect_now;
		//GetWindowRect(myhWnd,&newrect_now);
		//mygraphics->FillRectangle(&myTextureBrush, 0, 0, newrect_now.right, newrect_now.bottom);


		SolidBrush  brush(Color(255, 0, 0, 255));
		FontFamily  fontFamily(_T("Times New Roman"));
		Gdiplus::Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
		PointF      pointF(40.0f, 600.0f);
		for(int i=0;i<=20;i=i+2)				//增加文字 %;
		{
			pointF.Y = 600;
			pointF.Y = pointF.Y - i*30;
			wchar_t temp_char[200];
			//swprintf_s(temp_char,200,L"%d%%",i*5);
			swprintf_s(temp_char,200,L"%d",i*5);
			mygraphics->DrawString(temp_char, -1, &font, pointF, &brush);
		}
		pointF.X = 200;
		pointF.Y = 650;
		//mygraphics->DrawString(L"CPU 使用率", -1, &font, pointF, &brush);

	}


	//BlackBrush =new SolidBrush(Color(255,0,0,0));
	BlackBrush =new SolidBrush(Color(255,0,0,0));
	mygraphics->FillRectangle(BlackBrush,100,10,1200,600);
	mygraphics->DrawRectangle(myRectangle_pen,100,10,1200,600);



	for(int i=0;i<=39;i++)				//画网格线
		mygraphics->DrawLine(my_inline_pen,100+line_interval*(i+1)-m_interval*6,10,100+line_interval*(i+1)-m_interval*6,610);

	for(int i=0;i<19;i++)				//画网格线
		mygraphics->DrawLine(my_inline_pen,100,10+line_interval*(1+i),1300,10+line_interval*(1+i));
#if 0
	CPointItem *first_item=NULL,*second_item=NULL;
	first_item = m_pFirstItem;
	for (int i=0;i<Total_count-1;i++)
	{
		second_item=first_item->GetNext();
		if(second_item==NULL)
			break;
		//mygraphics->DrawCurve(CurePen,, 3, 1.5f);
		//Point myPointArray[] =
		//{Point(first_item->GetPoint().x,first_item->GetPoint().y),Point(second_item->GetPoint().x,second_item->GetPoint().y)};
		//mygraphics->DrawCurve(CurePen,myPointArray, 2, 1.0f);
		//mygraphics->DrawLine(CurePen,first_item->GetPoint().x,first_item->GetPoint().y,second_item->GetPoint().x,second_item->GetPoint().y);
		first_item = second_item;
	}
#endif
#if 1
	CPointItem *first_item=NULL,*second_item=NULL,*third_item=NULL;
	first_item = m_pFirstItem;

	for (int i=0;i<Total_count-1;i=i+2)	//画贝塞尔曲线，使得线条平滑;3个点一画;
	{
		second_item=first_item->GetNext();
		third_item = second_item->GetNext();
		if(second_item==NULL)
			break;
		if(third_item == NULL)
		{
			first_item = second_item;
			break;
		}
		//mygraphics->DrawCurve(CurePen,, 3, 1.5f);
		//取3个点给画曲线的GDI函数;
		Point myPointArray[] =
		{Point(first_item->GetPoint().x,first_item->GetPoint().y),
		Point(second_item->GetPoint().x,second_item->GetPoint().y),
		Point(third_item->GetPoint().x,third_item->GetPoint().y)};
		mygraphics->DrawCurve(CurePen,myPointArray, 3, 1.0f);
		//mygraphics->DrawLine(CurePen,first_item->GetPoint().x,first_item->GetPoint().y,second_item->GetPoint().x,second_item->GetPoint().y);

		first_item = third_item;
	}
#endif
	//	mygraphics->DrawLine(CurePen,)


	delete CurePen;
	delete mygraphics;
	delete myRectangle_pen;
	delete my_inline_pen;
	delete BlackBrush;
}
#endif


void CBacnetGraphic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_graphic_st_1.Invalidate(TRUE);
	m_graphic_st_2.Invalidate(TRUE);
	m_graphic_st_3.Invalidate(TRUE);
	m_graphic_st_4.Invalidate(TRUE);
	m_graphic_st_5.Invalidate(TRUE);
	m_graphic_st_6.Invalidate(TRUE);
	m_graphic_st_7.Invalidate(TRUE);
	m_graphic_st_8.Invalidate(TRUE);
	m_graphic_st_9.Invalidate(TRUE);
	m_graphic_st_a.Invalidate(TRUE);
	m_graphic_st_b.Invalidate(TRUE);
	m_graphic_st_c.Invalidate(TRUE);
	m_graphic_st_d.Invalidate(TRUE);
	m_graphic_st_e.Invalidate(TRUE);
	CDialogEx::OnTimer(nIDEvent);
}


//void CBacnetGraphic::OnStnClickedStaticGraphicA()
//{
//	// TODO: Add your control notification handler code here
//	static bool test_1 = false;
//	if(test_1)
//	{
//		m_graphic_st_1.bkColor(Static_Color[1]);
//		test_1 = false;
//	}
//	else
//	{
//		m_graphic_st_1.bkColor(RGB(187,187,187));
//		test_1 = true;
//	}
//}


void CBacnetGraphic::OnStnClickedStaticGraphicA()
{
	// TODO: Add your control notification handler code here

	if(my_input_info[0].show_graphic)
	{
		my_input_info[0].show_graphic = false;
		m_graphic_st_1.bkColor(Static_Color[1]);
	}
	else
	{
		my_input_info[0].show_graphic = true;
		m_graphic_st_1.bkColor(RGB(222,222,222));
	}

}



