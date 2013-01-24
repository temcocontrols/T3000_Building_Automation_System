
#pragma once
#include "afxmt.h"

//#include "stdafx.h"
unsigned short multi_register_value[513]={-1};

unsigned short cm5_register_value[512]; //CM5
unsigned short m_buffer[512];//CM5

int nCom;
//CString program_path=_T("");
int g_tstat_id=255;
int g_serialNum=0;
BOOL g_tstat_id_changed=FALSE;
BOOL g_bPauseMultiRead=FALSE;


CString g_strImagePathName=_T("");
int now_tstat_id =0;//for batch load /flash.

CString g_strOrigDatabaseFilePath=_T("");
CString	g_strDatabasefilepath=_T("");
CString g_strExePth=_T("");
CString g_strImgeFolder=_T("");

BOOL g_Scanfully=FALSE;
BOOL g_ScanSecurity=TRUE;
int g_nStartID=1;
int g_nEndID=254;
BOOL g_bCancelScan=FALSE;
int g_ScnnedNum=0;
CString g_strScanInfoPrompt;

CString g_strTstat5a;
CString g_strTstat5b;
CString g_strTstat5b2;
CString g_strTstat5c;
CString g_strTstat5d;
CString g_strnetWork;
CString g_strTstat5f;
CString g_strTstat5g;
CString g_strTstat5e;
CString g_strTstat5h;
CString g_strTstat6;
CString g_strTstat7;
CString g_strPressure;
CString g_strOR485;
CString g_strcm5;  //CM5
CString g_strLightingCtrl;

int g_CommunicationType;
int m_nbaudrat=19200;
CString showing_text;
CString flash_process;
CCriticalSection critical_section;
CCriticalSection register_critical_section;
CEvent g_eventPauseMultiRead;

BOOL g_buser_log_in=FALSE;
BOOL g_bPrivilegeMannage=FALSE;

CString g_strLoginUserName;
int g_MainScreenLevel=0;
int g_ParamLevel=0;
int g_OutPutLevel=0;
int g_NetWorkLevel=0;
int g_GraphicModelevel=0;
int g_BurnHexLevel=0;
int g_LoadConfigLevel=0;
int g_BuildingsetLevel=0;
int g_AllscreensetLevel=0;

int g_UserLevel=1;

CString g_strSensorName;
CString g_strInName1;
CString g_strInName2;
CString g_strInName3;
CString g_strInName4;
CString g_strInName5;
CString g_strInName6;
CString g_strInName7;
CString g_strInName8;

CString g_strInHumName;		// for tstat6 humidity input

CString g_strOutName1;
CString g_strOutName2;
CString g_strOutName3;
CString g_strOutName4;
CString g_strOutName5;
CString g_strOutName6;
CString g_strOutName7;
CString g_strOutName8;
int g_ifanStatus;
CString g_strFan=_T("");



BOOL g_bEnableRefreshTreeView = TRUE;
BOOL g_bScanStart = FALSE;
int g_llTxCount = 0;
int g_llRxCount = 0;
BOOL g_unint = TRUE;//TREE = °„C;FALSE = F;

//int g_nIpPort=6001;
//CString	g_strIpAdress;
BOOL allend = FALSE;


//tstat6_new
unsigned short reg_tststold[1024];





//  char CharTstat6[512][] = {
//  	{"COMMON"},{"Serial Number - 4 byte value.  Read-only"}
//  };
