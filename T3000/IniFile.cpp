// IniFile.cpp: implementation of the CEzIniFile class.

/* Comment by Jay:
This file is provided to interact with .ini file. 
The ini file acts as an initialization file for the project. 
It is read at the startup when the project is being loaded.
ini file in originally used in 16-bit windows applications. 
Now-a-days, initialization information is stored in various 
registries. 
*/

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
	/* Comment by Jay:
	GetPrivateProfileInt - Retrieves an integer associated with a key in the specified section 
	of an initialization file.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms724345(v=vs.85).aspx
	*/
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

	/* Comment by Jay:
	wsprintf - Writes formatted data to the specified buffer.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms647550(v=vs.85).aspx
	*/
	wsprintf(szT, _T("%d"), nValue);
	/* Comment by Jay:
	WritePrivateProfileString - Copies a string into the specified section of 
	an initialization file.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms725501(v=vs.85).aspx
	*/
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

	/* Comment by Jay:
	GetPrivateProfileString - Retrieves a string from the specified section in 
	an initialization file.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms724353(v=vs.85).aspx
	*/
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
	/* Comment by Jay:
	WritePrivateProfileString - Copies a string into the specified 
	section of an initialization file.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms725501(v=vs.85).aspx
	*/
	return WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, 
		m_szFileName);
}
