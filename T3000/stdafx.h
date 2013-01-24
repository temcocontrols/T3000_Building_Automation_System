
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
//#pragma comment(lib,"ISP")
 

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

INPUT void Show(CString CurSubBuldingName);

//INPUT SOCKET GetSocketHandle();

#include "modbus.h"
#include <vector>  // STL vector header. There is no ".h"
#include <map>
#include <afxdhtml.h>
using namespace std;  // Ensure that the namespace is set to std

#define CUSTOM_TABLE_FLOAT_VERSION 50.1
#define SETPOINT_SPECIAL_VERSION	50

typedef struct _STATUSBARINFO
{
	int nIndex;
	CString strInfoText;
}status_info;
#include "fileRW.h"

enum CFGFILE_VALPOS
{
	CV_Tstat_Section = 0,
	CV_Installdir =1,
	CV_TstatLastFlashFile, 
	CV_TstatLastFlashMethod,
	CV_TstatLastFlashPage,
	CV_TstatDeAddr,
	CV_TstatDeCOM,
	CV_TstatDeBaudrate,
	CV_NC_Section,
	CV_NCLastFlashFile,
	CV_NCLastFlashMethod,
	CV_NCDeIP,
	CV_NCDeIPPort,
	CV_LC_Section,
	CV_LCLastFlashFile,
	CV_LCLastFlashType,
	CV_LCDeIP,
	CV_LCDeIPPort,
	CV_LCDMDBID,
	CV_SELDEVICENAME,
};
const TCHAR c_strCfgTstatSection[] = _T("[Tstat]");
const TCHAR c_strCfgDefaultInstalldirItem[] = _T("Installdir : ");
const TCHAR c_strCfgLastFlashFileItem[] = _T("LastFlashFile : ");
const TCHAR c_strCfgLastFlashMethodItem[] = _T("LastFlashMethod : ");
const TCHAR c_strCfgDefaultAddrItem[] = _T("Default address : ");
const TCHAR c_strCfgDefaultComItem[] = _T("Default COM port : ");

const TCHAR c_strCfgNCSection[] = _T("[NC]");
const TCHAR c_strCfgDefaultIPItem[] = _T("Default IP : ");
const TCHAR c_strCfgDefaultIPPortItem[] = _T("Default IP Port : ");
const TCHAR c_strCfgDefaultBaudrateItem[] = _T("Default baudrate : ");//
const TCHAR c_strCfgLastFlashTypePageItem[] = _T("LastFlashPage : ");

const TCHAR c_strCfgLightCtrlSection[] = _T("[Lighting Controller]");
const TCHAR c_strCfgLastFlashLCTypeItem[] = _T("LastFlashType : ");
const TCHAR c_strCfgLastFlashLCTypeMain[] = _T("Main board");
const TCHAR c_strCfgLastFlashLCTypeInput[] = _T("Input board");
const TCHAR c_strCfgLastFlashLCTypeOutput[] = _T("Output board");

const TCHAR c_strCfgLastFlashMDBIDItem[] = _T("Default address : ");
const TCHAR c_strCfgDeviceName[] = _T("Default address : ");
//
const TCHAR c_strFlashMethodEthernet[] = _T("Ethernet");
const TCHAR c_strFlashMethodCom[] = _T("COM");
const TCHAR c_strCfgFileName[] = _T("config.txt");	
//#define _DEBUG
//*********************************link to dll***************************