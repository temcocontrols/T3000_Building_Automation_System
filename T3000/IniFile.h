
#pragma once

class CIniFile  
{
public:
	CIniFile(CString szFileName=_T(""));
	virtual ~CIniFile();

	void		SetFileName(CString szFileName);
	UINT		GetProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault );
	BOOL		WriteProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue );
	CString		GetProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL );
	BOOL		WriteProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );
private:
	CString		m_szFileName;
};
