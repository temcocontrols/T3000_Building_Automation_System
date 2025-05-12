// LanguageLocale.h: interface for the CLanguageLocale class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGUAGELOCALE_H__D60F6B28_35A3_4BF7_884C_C1DF1719486D__INCLUDED_)
#define AFX_LANGUAGELOCALE_H__D60F6B28_35A3_4BF7_884C_C1DF1719486D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum _LOADTYPE
{
	LOADSTRING_FROM_RC=0,
	LOADSTRING_FROM_INI,
};

#define BUFF_SIZE 1024
#define SECTION_NAME _T("MENU")
#define MESSAGEBOX _T("Pop Up Message")
#define T3000_VIEW _T("T3000_VIEW")
#define T3000_DLG _T("T3000_DLG")
static CString gStrFileName = _T("");
static TCHAR gBufferString[BUFF_SIZE];
class CLanguageLocale  
{
public:
	void AddCatalogLookupPath(LPCTSTR szPath);//设置文件查找路径
	bool SetLanguage(int nID);
	CLanguageLocale();
	virtual ~CLanguageLocale();
	enum
	{
		LANGUAGE_ENGLISH= 0,//简体中文
		LANGUAGE_PERSONAL  //英文
	};
private:
	CString  m_strPath;
	int      m_nDataSource;//0-rc,1-ini
};

CString gLoadString(LPCTSTR szID);
CString gLoadString(UINT nID);
void InitMenu(HMENU menu,CString KeyName);
void InitMenu(CMenu *menu,CString KeyName=_T("MENU")) ;
bool gSetDialogStrings(HWND hWnd,UINT ID);
bool gSetFormViewStrings(HWND hWnd,UINT ID);
bool gGetFormViewStrings(HWND hWnd,UINT ID);
     
bool gGetDialogStrings(HWND hWnd,UINT uDlgID);
bool gGetMenuStrings(HWND hWnd);
bool gSetMenuStrings(HWND hwnd);
CString gLoadString(CString section,CString keyname);
void ExportMenu(CMenu*menu,CString KeyName=_T("MENU")) ;
#endif // !defined(AFX_LANGUAGELOCALE_H__D60F6B28_35A3_4BF7_884C_C1DF1719486D__INCLUDED_)
