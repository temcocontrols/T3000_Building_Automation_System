#pragma once

//-----------------------------------------
//For poll definition
extern UINT g_read_ID;
extern UINT g_read_function;
extern UINT g_read_start_address;
extern UINT g_read_length;
extern int  g_read_interval;
extern bool g_auto_read;
//-----------------------------------------
//Wirte reg
extern UINT g_ready_write_ID;
extern UINT g_ready_write_Address;
extern UINT g_ready_write_Value;


//-----------------------------------------
extern _MessageReadMultyInfo *my_readmult;	//需要读取的结构体;
extern vector <CString> m_szComm;

extern	unsigned short multi_register_value[20000];
extern bool g_bIs_connected;

extern int g_PortNum;

extern int g_CommunicationType;
extern int m_nbaudrat;
extern CString g_IPAddress;
extern short	g_IPPort;


extern CString Program_Path;
extern CString Program_ConfigFile_Path;

extern CString g_PortDescribe;

extern CCriticalSection critical_section;
extern CCriticalSection register_critical_section;

extern CString cs_tool_version;


extern BOOL g_bEnableRefreshTreeView;
extern BOOL g_bScanStart;
extern int g_llTxCount;
extern int g_llRxCount;


extern int g_Tx_count;
extern int g_Error_count;

extern bool g_is_reading_now;

extern HWND main_hwnd;

// product model ID table
int const PM_TSTAT5A = 2;
int const PM_TSTAT5B = 1;
int const PM_TSTAT5B2 = 3;
int const PM_TSTAT5C = 4;
int const PM_TSTAT6 = 6;
int const PM_TSTAT7 = 7;
int const PM_TSTAT5D = 12;
const int PM_TSTAT5E = 16;
int const PM_TSTAT5F = 17;
int const PM_TSTAT5G = 18;
int const PM_TSTAT5H = 19;
int const PM_T38AIOD= 20;
int const PM_T3IOA = 21;
int const PM_T332AI = 22;
int const PM_T3AI16O = 23;
int const PM_ZIGBEE = 24;
int const PM_FLEXDRIVER = 25;
int const PM_T38I13O = 26;
int const PM_T3PERFORMANCE = 27;
int const PM_T34AO = 28;
int const PM_T36CT = 29;
int const PM_SOLAR = 30;
int const PM_FWMTRANSDUCER = 31;
int const PM_MINIPANEL = 35;
int const PM_PRESSURE = 40;
int const PM_CM5 = 50;
int const PM_NC = 100;
int const PM_AirQuality = 13;
int const PM_LightingController = 103;


extern HANDLE hThread;
extern HANDLE hDeal_thread;

extern DWORD n_Main_ThreadID;
extern DWORD n_Main_ThreadID_Do;

extern CCriticalSection MyCriticalSection;
extern vector <MSG> My_Receive_msg;

extern unsigned short product_register_value[10000];

