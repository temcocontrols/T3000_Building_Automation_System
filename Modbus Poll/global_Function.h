#ifndef _GLOBAL_FUNCTION_H
#define _GLOBAL_FUNCTION_H

#pragma region For_Bacnet
 
 
 
 
#include "..\t3000\ProductModel.h"

#pragma endregion For_Bacnet


#include "global_variable_extern.h"
#define FLEXGRID_CELL_COLOR RGB(128,128,128) 
#define GREEN_COLOR RGB(0,255,0)
#define YELLOW_COLOR RGB(255,255,0)
#define ORANGE_COLOR RGB(255,165,0)

//int const PM_TSTAT5B = 1;
//int const PM_TSTAT5A = 2;
//int const PM_TSTAT5B2 = 3;
//int const PM_TSTAT5C = 4;
//int const PM_TSTAT6 = 6;
//int const PM_TSTAT7 = 7;
//int const PM_TSTAT5i=8;
//int const PM_TSTAT5D = 12;
//int const PM_AirQuality = 13;
//int const PM_TSTAT5E = 16;
//int const PM_TSTAT5F = 17;
//int const PM_TSTAT5G = 18;
//int const PM_TSTAT5H = 19;
//
//int const PM_T3PT10= 26;
//int const PM_T3IOA = 21;
//int const PM_T332AI = 22;
//int const  PM_T38AI16O = 23;
//int const PM_T38I13O = 20;
//int const PM_T34AO = 28;
//int const PM_T36CT = 29;
//
//int const PM_ZIGBEE = 24;
//int const PM_FLEXDRIVER = 25;
//int const PM_T3PERFORMANCE = 27;
//int const PM_SOLAR = 30;
//int const PM_FWMTRANSDUCER = 31;
//int const PM_CO2_NET = 32;
//int const PM_CO2_RS485 = 33;
//int const PM_MINIPANEL = 35;
//int const PM_PRESSURE = 40;
//int const PM_CM5 = 50;
//int const PM_TSTAT6_HUM_Chamber=64;
//int const PM_NC = 100;
//int const PM_LightingController = 103;
//int const PM_HUM_R=42;
//int const PM_RUNNAR=15;
//int const PM_CO2_NODE = 34;

#include "global_variable_extern.h"
#include <afxinet.h>
CString GetProductName(int ModelID);
BOOL GetSerialComPortNumber1(vector<CString>& szComm);
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken);
BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position);
#endif

#if MODBUS_POLL
CString get_ExePath();
void Traffic_Data(CString &nCString);
  void SetPaneString(int nIndext,CString str);
#endif