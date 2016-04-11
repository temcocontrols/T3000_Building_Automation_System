#pragma once

#include <atlstr.h>

#define ML_STRING(id,text) MultiLang::ml_string(id)
#define ML_RESOURCE(id) MultiLang::ml_string(id)

namespace MultiLang
{
  extern LCID  LocaleId ;
  extern const CString ml_string ( int StringId ) ;
  //extern const TCHAR*  SupportedCultures[] ;
  //extern const int     NumberOfCultures ;
  extern const TCHAR*  RootNamespace ;
}

