// RegisterMonitor.h : main header file for the RegisterMonitor DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
//HINSTANCE RResOld; // �ɵ���Դ���
//HINSTANCE RDll; // DLL��Դ���

// CRegisterMonitorApp
// See RegisterMonitor.cpp for the implementation of this class
//

class CRegisterMonitorApp : public CWinApp
{
public:
	CRegisterMonitorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
