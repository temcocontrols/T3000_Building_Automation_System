#pragma once


#include "gloab_define.h"
#include "global_struct.h"
 
#define MY_MBPOLL_REG_DIALOG_MSG (WM_USER + 557)
#define MY_MBPOLL_WRITE_REG_MSG (WM_USER + 558)

extern CString g_strExEPath;
extern CString g_configfile_path;
extern CString	g_strDatabasefilepath;
extern CString  g_StrConnection;
extern CString PrintText[100];
extern  CCriticalSection  register_critical_section;
//extern  CWinThread* //m_pBackground_Read;
extern  BOOL g_online;

extern CDialog* g_data_trafficdlg;
extern CDialog *g_Draw_dlg;
extern HWND h_data_traffic_window;
extern CString g_String_Data_Traffic;
extern BOOL g_is_show_Data_Traffic_Window;
extern   long g_Tx_Rx;
extern vector<Registers_Infor> g_vectRegisters;
extern UINT g_Color_Index;
extern UINT g_Time_Offset;
//extern BOOL g_apply;
//extern BOOL g_rwonce;