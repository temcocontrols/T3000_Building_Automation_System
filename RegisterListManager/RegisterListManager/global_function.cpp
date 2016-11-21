#include "stdafx.h"
#include "global_function.h"

CString GetExePath(bool bHasSlash)
{
    TCHAR	szBuf[MAX_PATH];
    GetModuleFileName(NULL,szBuf,MAX_PATH);

    CString	strPath(szBuf);
    int idx = strPath.ReverseFind(_T('\\'));
    strPath = strPath.Left(idx);

    if(bHasSlash){ // has '\' at last.
        if(strPath.Right(1)!=_T('\\'))
            strPath+=_T('\\');
        return strPath;
    }else{ // don't need '\'.
        if(strPath.Right(1)==_T('\\'))
            strPath.TrimRight(_T('\\'));
        return strPath;
    }
}