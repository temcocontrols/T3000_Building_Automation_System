
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _BIND_TO_CURRENT_VCLIBS_VERSION 1


#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



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

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","EndOfFile") rename("BOF","FirstOfFile")
//**********************************link to dll*********************
#define INPUT extern "C" __declspec(dllimport)

#pragma comment(lib, "WS2_32")
#pragma comment(lib,"ModbusDllforVc")
#pragma comment(lib,"FlexSlideBar")

INPUT int Write_One(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One(unsigned char device_var,unsigned short address);
INPUT int write_multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);
//INPUT bool open_com(unsigned char m_com);
INPUT bool open_com(int m_com);
INPUT void close_com();
INPUT bool is_connect();
INPUT int CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Change_BaudRate(unsigned short new_baurate);
INPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);
INPUT void SetComnicationHandle(int nType,HANDLE hCommunication);
INPUT bool Open_Socket(CString strIPAdress);
INPUT void SetCommunicationType(int nType);
INPUT int NetController_CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Open_Socket2(CString strIPAdress,short nPort);
INPUT HANDLE GetCommunicationHandle();
//////////////////////////////////////////////////////////////////////////
INPUT int NetController_CheckTstatOnline_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
INPUT int NetController_CheckTstatOnline2_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
INPUT int CheckTstatOnline_a(unsigned char  devLo,unsigned char devHi, bool bComm_Type);
INPUT int CheckTstatOnline2_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);

INPUT int Read_One2(unsigned char  device_var,unsigned short  address, bool bComm_Type);
INPUT int Write_One2(unsigned char  device_var,unsigned short  address,unsigned short  value, bool bComm_Type);
//OUTPUT int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length);
INPUT int read_multi2(unsigned char device_var,unsigned short  *put_data_into_here,unsigned short  start_address,int length, bool bComm_Type);
INPUT void closefile();//scan
INPUT void writefile( CString strip,CString strport);//scan
INPUT void Createfile();//scan

INPUT void WriteLogFile(CString strlog);
INPUT void CloseLogFile();
INPUT CString Get_NowTime();


INPUT void NET_WriteLogFile(CString strlog);
INPUT void NET_CloseLogFile();
//INPUT SOCKET GetSocketHandle();

//#include "modbus.h"
#include <vector>  // STL vector header. There is no ".h"
#include <afxdhtml.h>
using namespace std;  // Ensure that the namespace is set to std

#define CUSTOM_TABLE_FLOAT_VERSION 50.1
#define SETPOINT_SPECIAL_VERSION	50



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


#define MY_WRITE_ONE WM_USER+100
#define MY_READ_ONE  WM_USER+101
#define MY_CLOSE WM_USER+110
#define MY_RESUME_DATA  WM_USER+200
#define MY_READ_DATA_CALLBACK WM_USER+201
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


typedef struct _MessageReadOneInfo
{
	unsigned char device_id;
	unsigned short address;
	short new_value;
	HWND hwnd;
}MessageReadOneInfo;

//#define _DEBUG
//*********************************link to dll***************************