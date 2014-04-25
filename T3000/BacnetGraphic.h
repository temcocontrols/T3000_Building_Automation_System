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
#pragma once
#include "afxwin.h"
// BacnetGraphic Code by Fance Du 2013 11 05
#include "CM5/CStatic/staticex.h"
// CBacnetGraphic dialog

class CBacnetGraphic : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetGraphic)

public:
	CBacnetGraphic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetGraphic();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_GRAPHIC };

	enum XASIX_TIME
	{ 
		TIME_ONE_MINUTE = 1 ,
		TIME_TEN_MINUTE = 2 ,
		TIME_ONE_HOUR = 3	,
		TIME_FOUR_HOUR = 4	,
		TIME_TWELVE_HOUR = 5,
		TIME_ONE_DAY = 6	,
		TIME_FOUR_DAY = 7	
	};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CToolBar m_wndToolBar;
	virtual BOOL OnInitDialog();
	void InitialToolBar();
	void MyPaint_Mem(HDC my_hdc);
	void Create_Line_Point();
	void SetXaxisTime(unsigned long nstarttime,unsigned long nendtime);
	void SetXaxisStartTime(unsigned long nstarttime);//只设置开始时间，x的结束时间由 m_time_selected 去计算;
	void SetYaxisValue(int nlowvalue,int nhighvalue);
	void SetXaxisScale(int nxscale);
	void SetYaxisScale(int nyscale);//设置X Y轴有几个刻度;
	void SetAnalogOrignPoint(PointF npoint);
	void SetDigitalOrignPoint(PointF npoint);
	void Set_XAxis_Length(int n_x_with);
	void Set_YAxis_Length(int n_y_high);
	void Set_Time_Scale(int n_time_scale);
	void SetDigital_X_WIDTH(int n_dig_x_width);
	void SetDigital_Y_Hight(int n_dig_hight);

	

	void CalcOnePixelTime();
	void CalcOnePixelValue();

	bool TimeValueToPoint(unsigned long ntime , int nvalue ,PointF &returnpoint);

	void InitialParameter();

	void GetAnalogOrignPoint(PointF &returnpoint);
	void GetDigitalOrignPoint(PointF &returnpoint);
	void GetXaxisTime(unsigned long &returnstarttime,unsigned long &returnendtime);
	void GetYaxisValue(int &returnlowvalue,int &returnhighvalue);
	int	GetDigital_X_WIDTH();
	int GetDigital_Y_Hight();
	int  Get_XAxis_Length();
	int  Get_YAxis_Length();
	int GetXaxisScale();
	int GetYaxisScale();
	int Get_Time_Scale();
public:
	PointF last_used_point;
	bool StaticShow[15];
	long m_starttime;
	long m_endtime;
	int m_lowvalue;
	int m_highvalue;
	int m_xscale;
	int m_yscale;
	PointF m_analogorignpoint;
	PointF m_digitalorignpoint;
	int m_X_ASIX_WIDTH;
	int m_Y_ASIX_HIGHT;
	int m_time_selected;
	float m_onepixeltime;
	float m_onepiexlvalue;
	int m_Digital_X_WIDTH;
	int m_Digital_Y_HIGHT;
protected:
	int x_axis_total_time ;//x轴 总共 目前的时间;
	int y_axis_total_value;//y轴 总共 目前最大值 减最小值;
BOOL InitDC();


//void Init_14_Static();


afx_msg void OnTimer(UINT_PTR nIDEvent);
//afx_msg void OnStnClickedStaticGraphicA();
//afx_msg void OnStnClickedStaticGraphicA();
afx_msg LRESULT Fresh_Static_Function(WPARAM wParam,LPARAM lParam);

CStaticEx m_graphic_st_1;
CStaticEx m_graphic_st_2;
CStaticEx m_graphic_st_3;
CStaticEx m_graphic_st_4;
CStaticEx m_graphic_st_5;
CStaticEx m_graphic_st_6;
CStaticEx m_graphic_st_7;
CStaticEx m_graphic_st_8;
CStaticEx m_graphic_st_9;
CStaticEx m_graphic_st_a;
CStaticEx m_graphic_st_b;
CStaticEx m_graphic_st_c;
CStaticEx m_graphic_st_d;
CStaticEx m_graphic_st_e;
virtual BOOL PreTranslateMessage(MSG* pMsg);
virtual void OnCancel();
virtual void OnOK();
afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
//	afx_msg void OnNcDestroy();
	virtual void PostNcDestroy();
};

//const int _6_min = 1;
//const int _1_hour = 2;
//const int _4_hour = 3;
//const int _12_hour = 4;
//const int _1_day = 5;

#define STATIC_FONT_COLOR  Color(255,0,0,0)

const Color Graphic_Color[15] =
{
	Color(0,0,0,0),
	Color(255,58,49,242),
	Color(255,103,217,19),
	Color(255,68,225,214),
	Color(255,240,88,77),
	Color(255,250,123,228),
	Color(255,220,220,95),
	Color(255,164,100,98),
	Color(255,63,187,140),
	Color(255,29,116,60),
	Color(255,124,0,19),
	Color(255,129,56,139),
	Color(255,98,105,90),
	Color(255,0,0,180),
	Color(255,120,120,120),
};
//#define UNUSE_COLOR  Color(255,222,222,222)

#define UNUSE_COLOR  Color(255,187,187,187)
const COLORREF Static_Color[15] = 
{
	RGB(0,0,0),
	RGB(58,49,242),
	RGB(103,217,19),
	RGB(68,225,214),
	RGB(240,88,77),
	RGB(250,123,228),
	RGB(220,220,95),
	RGB(164,100,98),
	RGB(63,187,140),
	RGB(29,116,60),
	RGB(124,0,19),
	RGB(129,56,139),
	RGB(98,105,90),
	RGB(0,0,180),
	RGB(120,120,120),
};


const int X_ORIGIN = 200;//这4个参数是设置 矩形绘图区域的位置和大小;
const int X_WIDTH  = 1200;

const int Y_ORIGIN = 30;
const int Y_HIGHT = 500;

const int STATIC_INTERVAL = 200;	//14 个Lable 之间的间隔;
const int STATIC_LABLE_WIDTH = 140; //每个Lable 的宽度;
const int STATIC_LABLE_HIGHT = 25; //高度;


const int DIGITAL_X_ORIGIN = 200;	//draw the digital graphic in this range;
const int DIGITAL_Y_ORIGIN = 560;
const int DIGITAL_X_WIDTH  = 1200;
const int DIGITAL_Y_HIGHT  = 150;




	