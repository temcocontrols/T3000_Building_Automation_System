#ifndef _GLOBLE_FUNCTION_H
#define _GLOBLE_FUNCTION_H

#pragma region For_Bacnet
 
 
 
#include "msflexgrid1.h"

#pragma endregion For_Bacnet


#include "global_variable_extern.h"
#define FLEXGRID_CELL_COLOR RGB(128,128,128) 
#define GREEN_COLOR RGB(0,255,0)
#define YELLOW_COLOR RGB(255,255,0)
#define ORANGE_COLOR RGB(255,165,0)

#include "global_variable_extern.h"
#include <afxinet.h>

BOOL GetSerialComPortNumber1(vector<CString>& szComm);
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken);
BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position);
#endif

#if MODBUS_POLL
CString get_ExePath();
void Traffic_Data(CString &nCString);
  void SetPaneString(int nIndext,CString str);
#endif