#include "StdAfx.h"
#include "MlString.h"

namespace MultiLang
{
  LCID  LocaleId ;
  const TCHAR*  SupportedCultures[] = { _T("zh-CN"), _T("en-US"), _T("en-GB"), _T("ne") } ; //MLHIDE
  const int     NumberOfCultures = sizeof SupportedCultures / sizeof SupportedCultures[0] ;
  const TCHAR*  RootNamespace = _T("T3000") ;       //MLHIDE

  const CString ml_string ( int StringId )
  {
#if 1
    CString   s ;

    // Try to load a resource for the specific locale.
    BOOL      ResourceOK = s.LoadString ( NULL, StringId, (WORD)LocaleId ) ;
    
    // If it fails, then load for the default locale.
    if ( !ResourceOK )
    {
      s.LoadString ( StringId ) ;
    }

    return (const TCHAR *)s ;
#else
    // Read from resources in Exe/Dll
    UINT      Block ;
    UINT      Num ;
    HRSRC     hRC   = NULL ;
    HGLOBAL   hgl   = NULL ;
    LPWSTR    str   = NULL ;
    HMODULE   hInst = NULL ;
    CString   wstrTheString ;

    Block = (StringId >> 4) + 1 ;
    Num   = StringId & 0xf ;

    hRC   = FindResourceEx ( hInst,
                             RT_STRING,
                             MAKEINTRESOURCE(Block),
                             (WORD)LocaleId ) ;

    if ( hRC )
    {
      hgl = LoadResource(hInst, hRC) ;
      if ( hgl )
      {
        str = (LPWSTR)LockResource(hgl) ;
        if ( str )
        {
          // Move up block to string we want
          for ( unsigned int i = 0 ; i < Num ; i++ )
            str += *str + 1 ;

          wstrTheString = CStringW ( str+1, *str ) ;
        }
      }
    }

    if (hgl)
    {
      UnlockResource(hgl) ; // maybe not needed
      FreeResource(hgl) ;   // maybe not needed
    }

    return wstrTheString ;
#endif
  }
}
