
// T3000ProgramEditorTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CT3000ProgramEditorTestApp:
// See T3000ProgramEditorTest.cpp for the implementation of this class
//

class CT3000ProgramEditorTestApp : public CWinApp
{
public:
	CT3000ProgramEditorTestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CT3000ProgramEditorTestApp theApp;