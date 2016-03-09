#pragma once
#include "stdafx.h"
typedef struct T3_Register
{
	int regID;
	CString regName;
}T3Register;
typedef struct RegPoint{
	unsigned short Time_offset;
	unsigned short Value_offset;
};
typedef struct Registers_Infor
{
    vector<RegPoint> Reg_Point;
    COLORREF Point_Color;
    CString Reg_Name;
    int address;
};


 

#include <string>
#include <sstream>

// #if defined _UNICODE || defined UNICODE
// typedef std::wstring TChartString;
// typedef std::wstringstream TChartStringStream;
// #else
// typedef std::string TChartString;
// typedef std::stringstream TChartStringStream;
// #endif
#if defined _UNICODE || defined UNICODE
typedef std::wstring TChartString;
typedef std::wstringstream TChartStringStream;
#else
typedef std::string TChartString;
typedef std::stringstream TChartStringStream;
#endif