
#pragma once
#include "afxmt.h"

//#include "stdafx.h"
unsigned short multi_register_value[20000]={-1};

HANDLE m_hSerial;//串口句柄
SOCKET m_hSocket;

CString cs_tool_version;


bool g_bIs_connected=false;

int g_CommunicationType;

CString g_PortDescribe;

int g_PortNum=0;
int m_nbaudrat=19200;
CString g_IPAddress;
short	g_IPPort;

//-----------------------------------------
//For poll definition
UINT g_read_ID;
UINT g_read_function;
UINT g_read_start_address;
UINT g_read_length;
int  g_read_interval;
bool g_auto_read;
//-----------------------------------------
//Wirte reg
UINT g_ready_write_ID;
UINT g_ready_write_Address;
UINT g_ready_write_Value;


//-----------------------------------------
_MessageReadMultyInfo *my_readmult;	//需要读取的结构体;

CString Program_Path;
CString Program_ConfigFile_Path;


CCriticalSection critical_section;
CCriticalSection register_critical_section;

BOOL g_bEnableRefreshTreeView = TRUE;
BOOL g_bScanStart = FALSE;
int g_llTxCount = 0;
int g_llRxCount = 0;

vector<CString> m_szComm;



_Tx_Struct g_Tx_Struct;
_Err_Struct g_Err_Struct;
_ID_Struct g_ID_Struct;
_SR_Struct g_SR_Struct;

HANDLE hThread;
HANDLE hDeal_thread;

DWORD n_Main_ThreadID;
DWORD n_Main_ThreadID_Do;

CCriticalSection MyCriticalSection;

vector <MSG> My_Receive_msg;

unsigned short product_register_value[10000];


int g_Tx_count;
int g_Error_count;

bool g_is_reading_now;

HWND main_hwnd;

