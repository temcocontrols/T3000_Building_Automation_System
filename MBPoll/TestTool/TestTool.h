
// TestTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestToolApp:
// See TestTool.cpp for the implementation of this class
//

class CTestToolApp : public CWinApp
{
public:
	CTestToolApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnDisplaySigned();
//	afx_msg void OnDisplayUnsigned();
};

extern CTestToolApp theApp;