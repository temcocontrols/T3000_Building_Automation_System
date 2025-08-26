/********************************************************************
	created:	2011/07/25
	created:	25:7:2011   16:07
	filename: 	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar\FlexSlideBarDef.h
	file path:	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar
	file base:	FlexSlideBarDef
	file ext:	h
	author:			
	purpose:	 Global variables, constants, macro definitions
	purpose:	 ȫ�ֱ������������궨��
*********************************************************************/
#pragma once


// fsb shape
// fsb����̬
enum FSB_STYLE
{
	FSB_STYLE_SINGLETHUMB = 0x5001,		// Single button pointer
	                                    // ����buttonָ��
	FSB_STYLE_DOUBLETHUMB =	0x5002,		// Two button pointers  
	                                    // ����buttonָ��
	FSB_STYLE_TRIPLETHUMB	= 0x5003,		// 3 button pointers
	                                        // 3��buttonָ��
	FSB_STYLE_DOUBLELINKAGE	= 0x5004,		// 2 button pointers locked linkage
	                                        // 2��buttonָ����������
	FSB_STYLE_TRIPLELINKAGE	= 0x5005,		// 3 button pointers locked linkage
	                                        // 3��buttonָ����������
};

// fsb pointer appearance, shape  
// fsb ָ������ӣ����
enum FSB_THUMBSHAPE
{
	FSB_THUMBSHAPE_BAR = 0x6001,		// Pointer shape - bar
	                                    // ָ����״ �� bar
	FSB_THUMBSHAPE_RECT =  0x6002,		// Pointer shape - rectangle
	                                    // ָ����״ �� rectangle
	FSB_THUMBSHAPE_TRI = 0x6003,		// Pointer shape - triangle
	                                    // ָ����״ �� ����
	FSB_THUMBSHAPE_CIRCLE = 0x6004,		// Pointer shape - circle
	                                    // ָ����״ �� Բ��
};


// fsb scale position
// fsb �̶ȵ�λ��
enum FSB_TICSIDE
{
	FSB_TICSIDE_TOP = 1,	
	FSB_TICSIDE_BOTTOM = 2,	
	FSB_TICSIDE_LEFT = 3,
	FSB_TICSIDE_RIGHT = 4,	
	FSB_TICSIDE_BOTH = 5,	
};


const int THUNMB_RECT_HEIGHT=12;
const int THUNMB_BAR_HEIGHT=2;


// channel partion color 
// const DWORD CLR_COOLING = RGB(96,96,224);
// const DWORD CLR_HEATING = RGB(224,128,128);
// const DWORD CLR_BALANCE = RGB(96,192,0);

// Slider above
const DWORD CLR_COOLING = RGB(0,0,255);//��������
const DWORD CLR_HEATING = RGB(255,0,0);
const DWORD CLR_BALANCE = RGB(96,192,0);