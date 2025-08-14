// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

// Windows Header Files:
// Windows 头文件: 
//#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
// TODO: reference additional headers your program requires here
//// TODO:  在此处引用程序需要的其他头文件

