// MFC16API.h : main header file for the MFC16API DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <afxinet.h>
#include <string>

#define Fance_API __declspec(dllexport)
// CMFC16APIApp
// See MFC16API.cpp for the implementation of this class
//

extern "C"
{
	class CMFC16APIApp : public CWinApp
	{
	public:
		CMFC16APIApp();

		// Overrides
	public:
		virtual BOOL InitInstance();

		DECLARE_MESSAGE_MAP()
	};

	 int		Fance_API	Test_Add(int test1, int test2);
	 bool		Fance_API	ConnectToFtp(CString FTP_Address , CString sUserName , CString sPassWord);
	 bool		Fance_API	DisconnectFtp();
	 bool       Fance_API   DownloadFileFromFtp(CString FileFtpPath,CString DesFolder);
	 bool		Fance_API   GetFtpTransferPersent(unsigned int *total_byte,unsigned int *finish_byte,int *m_persent);
	  bool		Fance_API    UnzipItem(CString SourceFilePath,CString DesFolder);
}

