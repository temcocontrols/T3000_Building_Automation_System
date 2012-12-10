// ModbusDllforVc.h : main header file for the ModbusDllforVc DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CModbusDllforVcApp
// See ModbusDllforVc.cpp for the implementation of this class
//

class CModbusDllforVcApp : public CWinApp
{
public:
	CModbusDllforVcApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
