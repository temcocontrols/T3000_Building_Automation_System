/********************************************************************
	created:	2011/07/25
	created:	25:7:2011   16:07
	filename: 	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar\FlexSlideBarDef.h
	file path:	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar
	file base:	FlexSlideBarDef
	file ext:	h
	author:			
	purpose:	 
*********************************************************************/
#pragma once


// fsb
enum FSB_STYLE
{
	FSB_STYLE_SINGLETHUMB = 0x5001,		// button
	FSB_STYLE_DOUBLETHUMB =	0x5002,		// button
	FSB_STYLE_TRIPLETHUMB	= 0x5003,		// 3button
	FSB_STYLE_DOUBLELINKAGE	= 0x5004,		// 2button
	FSB_STYLE_TRIPLELINKAGE	= 0x5005,		// 3button
};

// fsb 
enum FSB_THUMBSHAPE
{
	FSB_THUMBSHAPE_BAR = 0x6001,		//   bar
	FSB_THUMBSHAPE_RECT =  0x6002,		//   rectangle
	FSB_THUMBSHAPE_TRI = 0x6003,		//   
	FSB_THUMBSHAPE_CIRCLE = 0x6004,		//   
};


// fsb 
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

const DWORD CLR_COOLING = RGB(0,0,255);//
const DWORD CLR_HEATING = RGB(255,0,0);
const DWORD CLR_BALANCE = RGB(96,192,0);