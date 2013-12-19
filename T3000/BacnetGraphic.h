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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


BOOL InitDC();


void Init_14_Static();


afx_msg void OnTimer(UINT_PTR nIDEvent);
//afx_msg void OnStnClickedStaticGraphicA();
afx_msg void OnStnClickedStaticGraphicA();
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
};

const int _6_min = 1;
const int _1_hour = 2;
const int _4_hour = 3;
const int _12_hour = 4;
const int _1_day = 5;

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


const int X_ORIGIN = 250;
const int X_WIDTH  = 1000;

const int Y_ORIGIN = 30;
const int Y_HIGHT = 500;


	