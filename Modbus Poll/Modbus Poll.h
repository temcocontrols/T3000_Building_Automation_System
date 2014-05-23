
// Modbus Poll.h : main header file for the Modbus Poll application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CModbusPollApp:
// See Modbus Poll.cpp for the implementation of this class
//

class CModbusPollApp : public CWinAppEx
{
public:
	CModbusPollApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CModbusPollApp theApp;
