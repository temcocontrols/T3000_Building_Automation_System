
#pragma once
#include "afxmt.h"
#include "gloab_define.h"
#include "global_struct.h"

CString	g_strDatabasefilepath=_T("");
CString g_strExEPath=_T("");
CString g_configfile_path=_T("");

CCriticalSection  register_critical_section;
//CWinThread* m_pBackground_Read;
CString PrintText[100];
BOOL g_online=FALSE;
CString g_StrConnection;
CDialog *g_data_trafficdlg=NULL;

CDialog *g_Draw_dlg=NULL;
HWND h_data_traffic_window;

CString g_String_Data_Traffic;

BOOL g_is_show_Data_Traffic_Window=FALSE;
long g_Tx_Rx;
vector<Registers_Infor> g_vectRegisters;
UINT g_Color_Index=0;
UINT g_Time_Offset = 0;
//BOOL g_apply=TRUE;
//BOOL g_rwonce=FALSE;