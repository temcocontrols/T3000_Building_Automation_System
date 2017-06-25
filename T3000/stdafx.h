
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once
#define _SECURE_SCL 0

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _BIND_TO_CURRENT_VCLIBS_VERSION 1


#define CUSTOM_TABLE_FLOAT_VERSION 50.1
#define SETPOINT_SPECIAL_VERSION	50
#define  WM_REFRESH_BAC_INPUT_LIST WM_USER + 201
#define  WM_REFRESH_BAC_PROGRAM_LIST WM_USER + 203

#define  WM_LIST_ITEM_CHANGED WM_USER + 202
#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxinet.h>    // MFC Internet support
#include <afxmt.h>      // MFC multithreading support
#include <bitset>
//#define  test_ptp

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <afxsock.h>            // MFC socket extensions
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma  comment(lib,"GdiPlus.lib")

#pragma  comment(lib,"HtmlHelp.lib")
#pragma  comment(lib,"Iphlpapi.lib")

 
#include "HtmlHelp.h"

#include "T3000_Help_Map.h"

//#include <CppUnitTest.h>
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//#pragma  comment(lib,"ISP.lib")
//#pragma  comment(lib,"RegisterMonitor.lib")
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","EndOfFile") rename("BOF","FirstOfFile")
//**********************************link to dll*********************
#define INPUT extern "C" __declspec(dllimport)
#pragma comment(lib, "WS2_32")
#pragma comment(lib,"ModbusDllforVc")
#pragma comment(lib,"FlexSlideBar")
//#pragma comment(lib,"ISP")
#pragma comment(lib,"BACnet_Stack_Library" )
//INPUT void  show_ISPDlg();
//INPUT void  show_RegisterMonitorDlg(); 
INPUT int write_multi_Short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length);
//INPUT bool open_com(unsigned char m_com);


INPUT int Write_One_tap(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One_tap(unsigned char device_var,unsigned short address);
INPUT int write_multi_tap(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int read_multi_tap(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);
INPUT int Modbus_Standard_Read(unsigned char device_var, unsigned short *put_data_into_here, int function_code, unsigned short start_address, int length,
	unsigned char *put_senddate_into_here,
	unsigned char *put_revdata_into_here,
	int* sendDataLength, int* recvDataLength
);
INPUT bool open_com(int m_com);
INPUT void close_com();
INPUT bool is_connect();
INPUT int CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Change_BaudRate(int new_baurate);
INPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);
INPUT void SetComnicationHandle(int nType,HANDLE hCommunication);
INPUT void SetCommunicationType(int nType);
INPUT int NetController_CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Open_Socket2(CString strIPAdress,short nPort);

INPUT void SetLastSuccessBaudrate(int nbaudrate);
INPUT void SetLastOpenedComport(int ncom_port);
INPUT void SetLastCommunicationType(int n_commnication_type);
INPUT int GetLastSuccessBaudrate();
INPUT int GetLastOpenedComport();
INPUT int GetLastCommunicationType();
INPUT int GetCommunicationType();
INPUT HANDLE GetCommunicationHandle();
//////////////////////////////////////////////////////////////////////////
INPUT int NetController_CheckTstatOnline_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
//INPUT int NetController_CheckTstatOnline2_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
INPUT int CheckTstatOnline_a(unsigned char  devLo,unsigned char devHi, bool bComm_Type);
//INPUT int CheckTstatOnline2_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
INPUT int MINI_CheckTstatOnline_a(unsigned char devLo,unsigned char devHi, bool bComm_Type,int NET_COM=1);
//INPUT int MINI_CheckTstatOnline2_a(unsigned char devLo,unsigned char devHi, bool bComm_Type,int NET_COM=1);

INPUT int Read_One2(unsigned char  device_var,unsigned short  address, bool bComm_Type);
INPUT int Write_One2(unsigned char  device_var,unsigned short  address,unsigned short  value, bool bComm_Type);
//OUTPUT int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length);
INPUT int read_multi2(unsigned char device_var,unsigned short  *put_data_into_here,unsigned short  start_address,int length, bool bComm_Type);

INPUT void close_T3000_log_file();//scan
INPUT void write_T3000_log_file( CString StrTips);//scan
INPUT void Create_T3000_log_file();//scan

INPUT CString Get_NowTime();
INPUT int Write_One(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One(unsigned char device_var,unsigned short address);
INPUT int write_multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
//INPUT int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);

INPUT int Read_One_log(unsigned char device_var,unsigned short address,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int Write_One_log(unsigned char device_var,unsigned short address,unsigned short val,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int read_multi_log(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int write_multi_log(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT void SetResponseTime(unsigned short Time);

#include <stdint.h>
//#include "modbus.h"
#include <vector>  // STL vector header. There is no ".h"
#include <afxdhtml.h>
using namespace std;  // Ensure that the namespace is set to std

#define		CUSTOM_TABLE_FLOAT_VERSION					50.1
#define		SETPOINT_SPECIAL_VERSION					50
#define		WM_REFRESH_BAC_INPUT_LIST					WM_USER + 201

#define		WM_REFRESH_BAC_PROGRAM_LIST					WM_USER + 203
#define		WM_REFRESH_BAC_PROGRAM_RICHEDIT				WM_USER + 204
#define		WM_REFRESH_BAC_VARIABLE_LIST				WM_USER + 205
#define		WM_REFRESH_BAC_OUTPUT_LIST					WM_USER + 206
#define		WM_REFRESH_BAC_WEEKLY_LIST					WM_USER + 207
#define		WM_REFRESH_BAC_ANNUAL_LIST					WM_USER + 208
#define		WM_REFRESH_BAC_SCHEDULE_LIST				WM_USER + 209
#define		WM_REFRESH_BAC_DAY_CAL						WM_USER + 210
#define		WM_REFRESH_BAC_CONTROLLER_LIST				WM_USER + 211
#define		WM_REFRESH_BAC_SCREEN_LIST					WM_USER + 212
#define		WM_REFRESH_BAC_MONITOR_LIST					WM_USER + 213
#define		WM_REFRESH_BAC_MONITOR_INPUT_LIST			WM_USER + 214
#define		WM_REFRESH_BAC_ALARMLOG_LIST				WM_USER + 215
#define		WM_REFRESH_BAC_TSTAT_LIST					WM_USER + 216
#define     WM_DOWNLOADFILE_MESSAGE						WM_USER + 217
#define     WM_MULTY_FLASH_MESSAGE						WM_USER + 218
#define		WM_REFRESH_BAC_CUSTOMER_DIGITAL_RANGE_LIST	WM_USER + 219
#define		WM_REFRESH_BAC_USER_NAME_LIST				WM_USER + 220
#define		WM_REFRESH_BAC_AT_COMMAND					WM_USER + 221
#define		WM_REFRESH_BAC_REMOTE_POINT_LIST					WM_USER + 222
#define		WM_REFRESH_BAC_ANALOGCUSRANGE_LIST					WM_USER + 223
#define		WM_REFRESH_BAC_EXTIO_LIST					WM_USER + 224
#define WM_SHOW_SCANN_RESULTS WM_USER + 2691
//#pragma warning(disable:4244)
//#pragma warning(disable:4018)
//#pragma warning(disable:4800)
//#pragma warning(disable:4101)
//#pragma warning(disable:4554)
//#pragma warning(disable:4305)
//#pragma warning(disable:4005)


#pragma warning(disable:4146)	//Add by Fance
//warning C4146: unary minus operator applied to unsigned type, result still unsigned
//The MSDN suggested that we can ignore it;

typedef struct _STATUSBARINFO
{
	int nIndex;
	CString strInfoText;
}status_info;
#include "fileRW.h"

#include "T3000RegAddress.h"


#define MKBOOL(_VALUE) ((_VALUE) != 0)		//Add by Fance .Use this macro to solve the warning warning C4800: 'BOOL' : forcing value to bool 'true' or 'false'


#define MY_WRITE_ONE				WM_USER	+ 100
#define MY_READ_ONE					WM_USER	+ 101
#define MY_INVOKE_ID				WM_USER + 102
#define MY_BAC_WRITE_LIST			WM_USER + 103
#define MY_BAC_REFRESH_LIST			WM_USER + 104
#define MY_BAC_REFRESH_ONE			WM_USER + 105

#define MY_WRITE_ONE_LIST				WM_USER	+ 106
#define MY_WRITE_MULTI_LIST				WM_USER	+ 107
#define MY_RS485_WRITE_LIST          WM_USER	+ 108
#define MY_CLOSE WM_USER+110

#define MY_RESUME_DATA  WM_USER + 300
#define MY_READ_DATA_CALLBACK WM_USER+301
#define MY_RX_TX_COUNT WM_USER + 302
#define WM_ADD_DEBUG_CSTRING WM_USER + 303
#define MY_RETRY_MESSAGE WM_USER + 304
#define  WM_SCAN_PRODUCT WM_USER+2014
#define  WM_SHOW_PANNELINFOR WM_USER + 2222

typedef struct _MessageWriteOneInfo
{
	unsigned char device_id;
	unsigned short address;
	short new_value;
	short old_value;
	HWND hwnd;
	UINT CTRL_ID;
	CString Changed_Name;

}MessageWriteOneInfo;

typedef struct _MessageWriteOneInfo_List
{
    int list_type;
	unsigned char device_id;
	unsigned short address;
	short new_value;
	short db_value;
	HWND hwnd;
	int mRow;
	int mCol;
	CString Changed_Name;
}MessageWriteOneInfo_List;

typedef struct _MessageWriteMultiInfo_List
{
	unsigned char device_id;
	unsigned short Start_Address;
	unsigned char RegValue[8];
	HWND hwnd;
	int mRow;
	int mCol;
	CString Changed_Name;
}MessageWriteMultiInfo_List;


typedef struct _MessageReadOneInfo
{
	unsigned char device_id;
	unsigned short address;
	short new_value;
	HWND hwnd;
}MessageReadOneInfo;

typedef struct _MessageInvokeIDInfo
{
	int Invoke_ID;
	HWND hwnd;
	CString task_info;
	int mRow;
	int mCol;
}MessageInvokeODInfo;

#define  CPING_USE_ICMP 1
//#include "Bacnet_Include.h"
//#define _DEBUG
//*********************************link to dll***************************