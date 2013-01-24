#pragma once

extern	unsigned short multi_register_value[513];
extern unsigned short cm5_register_value[512]; //CM5
extern unsigned short m_buffer[512];//CM5
extern unsigned short reg_tststold[1024];
extern int nCom;
extern	CString program_path;
extern	int g_tstat_id;
extern int g_serialNum;
extern	BOOL g_tstat_id_changed;
extern  BOOL g_bPauseMultiRead;     // for background read register

extern  int now_tstat_id;//for batch load/flash read/write
extern  CString g_strImagePathName;
extern	CString	g_strDatabasefilepath;
extern	CString g_strExePth;
extern  CString g_strImgeFolder;
extern  CString g_strOrigDatabaseFilePath;


extern	BOOL g_Scanfully;
extern	BOOL g_ScanSecurity;
extern	int g_nStartID;
extern	int g_nEndID;
extern BOOL g_bCancelScan;
extern int g_ScnnedNum;
extern CString g_strScanInfoPrompt;

extern int gCommunicationType;



extern CString g_strTstat5a;
extern CString g_strTstat5b;
extern CString g_strTstat5b2;
extern CString g_strTstat5c;
extern CString g_strTstat5d;
extern CString g_strnetWork;
extern CString g_strTstat5f;
extern CString g_strTstat5g;
extern CString g_strTstat5e;
extern CString g_strTstat5h;
extern CString g_strTstat6;
extern CString g_strTstat7;
extern CString g_strOR485;
extern CString g_strPressure;
extern CString g_strLightingCtrl;


extern int g_CommunicationType;
extern int m_nbaudrat;
extern CString showing_text;
extern CCriticalSection critical_section;
extern CCriticalSection register_critical_section;
extern CEvent g_eventPauseMultiRead;


extern BOOL g_buser_log_in;
extern BOOL g_bPrivilegeMannage;
extern CString g_strLoginUserName;
extern int g_UserLevel;

extern CString g_strSensorName;
extern CString g_strInName1;
extern CString g_strInName2;
extern CString g_strInName3;
extern CString g_strInName4;
extern CString g_strInName5;
extern CString g_strInName6;
extern CString g_strInName7;
extern CString g_strInName8;

extern CString g_strInHumName;		// for tstat6 humidity input


extern CString g_strOutName1;
extern CString g_strOutName2;
extern CString g_strOutName3;
extern CString g_strOutName4;
extern CString g_strOutName5;
extern CString g_strOutName6;
extern CString g_strOutName7;
extern CString g_strOutName8;
extern int g_ifanStatus;
extern CString g_strFan;

extern BOOL g_bEnableRefreshTreeView;
extern BOOL g_bScanStart;
extern int g_llTxCount;
extern int g_llRxCount;
extern BOOL g_unint;
extern BOOL allend;


//////////////////////////////////////////////////////////////////////////
// extern int const PM_TSTAT5A;
// extern int const PM_TSTAT5B;
// extern int const PM_TSTAT5B2;
// extern int const PM_TSTAT5C;
// extern int const PM_TSTAT5D;
// extern const int PM_TSTAT5E;
// extern int const PM_TSTAT5F;
// extern int const PM_TSTAT5G;
// extern int const PM_TSTAT5H;
// extern int const PM_TSTAT6;
// extern int const PM_TSTAT7;

//////////////////////////////////////////////////////////////////////////
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

