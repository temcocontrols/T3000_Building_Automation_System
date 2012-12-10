// IniFile.cpp: implementation of the CEzIniFile class.
//

#include "stdafx.h"
#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BUFFER_MAXSIZE		4096
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(CString szFileName)
{
	m_szFileName = szFileName;
}

CIniFile::~CIniFile()
{

}

/*****************************************************************
*
*	Function:	SetFileName()
*
*	Purpose:	Set the current file
*
*	Remarks:	
*
******************************************************************/
void CIniFile::SetFileName(CString szFileName)
{
	m_szFileName = szFileName;
}
/*****************************************************************
*
*	Function:	GetProfileInt()
*
*	Purpose:	Get int value from ini file.
*
******************************************************************/
UINT CIniFile::GetProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, 
							   int nDefault )
{
	//
	return GetPrivateProfileInt(lpszSection, lpszEntry, nDefault, m_szFileName);

}

/*****************************************************************
*
*	Function:	WriteProfileInt()
*
*	Purpose:	Write int value to the ini file
*
*	Remarks:	
*
******************************************************************/
BOOL CIniFile::WriteProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, 
								 int nValue )
{
	// Chang int to string
	TCHAR szT[64];
	wsprintf(szT, _T("%d"), nValue);
	return WritePrivateProfileString(lpszSection, lpszEntry, szT, m_szFileName);

}

/*****************************************************************
*
*	Function:	GetProfileString()
*
*	Purpose:	Get String value from file
*
*	Remarks:	
*
******************************************************************/
CString CIniFile::GetProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, 
									 LPCTSTR lpszDefault)
{
	//
	TCHAR buf[BUFFER_MAXSIZE];
	GetPrivateProfileString(lpszSection, lpszEntry, lpszDefault, buf, 
		BUFFER_MAXSIZE, m_szFileName);
	return buf;

}
/*****************************************************************
*
*	Function:	WriteProfileString()
*
*	Purpose:	Write string to file
*
*	Remarks:	
*
******************************************************************/
BOOL CIniFile::WriteProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, 
									LPCTSTR lpszValue )
{
	//
	return WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, 
		m_szFileName);

}
