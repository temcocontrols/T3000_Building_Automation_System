// SelectLanguage.cpp : implementation file
//

#include "stdafx.h"
#include "SelectLanguage.h"
#include "MlString.h"

using namespace MultiLang ;

// CSelectLanguage dialog

const TCHAR*  SupportedCultures[] = { _T("zh-CN"), _T("en-US"), _T("en-GB"), _T("ne") } ;
const int     NumberOfCultures = sizeof SupportedCultures / sizeof SupportedCultures[0] ;

IMPLEMENT_DYNAMIC(CSelectLanguage, CDialog)

CSelectLanguage::CSelectLanguage(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectLanguage::IDD, pParent)
{
  m_SelectedIetfTag = NULL ;
}

CSelectLanguage::~CSelectLanguage()
{
}

void CSelectLanguage::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST_CULTURES, m_ListCultures);
  DDX_Radio(pDX, IDC_RADIO_SHOW, m_StartupMode);
}


BEGIN_MESSAGE_MAP(CSelectLanguage, CDialog)
  ON_LBN_SELCHANGE(IDC_LIST_CULTURES, &CSelectLanguage::OnLbnSelchangeListCultures)
END_MESSAGE_MAP()

void CSelectLanguage::LoadSettingsAndShow ( bool ForceShow )
{
  HRESULT           hr;

  hr = CoInitializeEx ( NULL, COINIT_MULTITHREADED ) ;
  hr = CoCreateInstance ( CLSID_CMultiLanguage, NULL, CLSCTX_ALL, IID_IMultiLanguage, (void**)&m_pMlang ) ;

  GetApplicationDirectory() ;
  LoadSettings() ;

  if ( ForceShow || ( ShowLangDialog == m_StartupMode ) )
  {
    DoModal() ;
    SaveSettings() ;
  }  

  // Load the satellite DLL and pass it to MFC
  SetLanguageForMFC() ;

  m_pMlang->Release() ;
  CoUninitialize() ;
}

void CSelectLanguage::LoadSettings()
{
  HKEY  hkey ;
  DWORD dwDisposition ;
  DWORD dwType ; 
  DWORD dwSize ; 
  DWORD dwValue ;
  LONG  RetVal ;
  
  RetVal = RegCreateKeyEx ( HKEY_CURRENT_USER, _T("Software\\Jollans\\Demo"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hkey, &dwDisposition ) ;
  
  dwType = REG_DWORD;
  dwSize = sizeof(DWORD);
  RetVal = RegQueryValueEx ( hkey, _T("SelectLanguageMode"), NULL, &dwType, (PBYTE)&dwValue, &dwSize ) ;
  
  if ( ERROR_SUCCESS == RetVal )
  {
    m_StartupMode = (enum enumStartupMode)dwValue ;
  }  
  else
  {
    m_StartupMode = ShowLangDialog ;
  }
  
  // Don't fetch the saved language if the mode is 'use default'.
  if ( m_StartupMode == UseDefaultCulture )
  {
    // I'm not sure that this is wise. It might be better to set LocaleId = -1 and 
    // handle it as a special case
    MultiLang::LocaleId = GetUserDefaultLCID() ;
  }
  else
  {
    dwType = REG_DWORD;
    dwSize = sizeof(DWORD);
    RetVal = RegQueryValueEx ( hkey, _T("SelectLanguageLCID"), NULL, &dwType, (PBYTE)&dwValue, &dwSize ) ;
    
    if ( ERROR_SUCCESS == RetVal )
    {
      bool    IsValid = false ;
      HRESULT hr ;

      // Check that it is a valid value
      for ( int i = 0 ; i < NumberOfCultures ; i++ )
      {
        LCID          Id ;
        hr = m_pMlang->GetLcidFromRfc1766 ( &Id, _bstr_t ( SupportedCultures[i] ) ) ;
        if ( SUCCEEDED ( hr ) )
        {
          if ( Id == dwValue )
          {
            // Value stored in registry matches one of our supported languages - use it.
            MultiLang::LocaleId = dwValue ;
            m_SelectedIetfTag = SupportedCultures[i] ;
            IsValid = true ;
            break ;
          }
        }
      }

      if ( !IsValid )
      {
        // Value from registry appears to be invalid. Show the dialog.
        m_StartupMode = ShowLangDialog ;
      }
    }  
  }  
   
  RegCloseKey ( hkey ) ;
}

void CSelectLanguage::SaveSettings()
{
  HKEY  hkey ;
  DWORD dwDisposition ;
  DWORD dwValue ;
  LONG  RetVal ;
  
  RetVal = RegCreateKeyEx ( HKEY_CURRENT_USER, _T("Software\\Jollans\\Demo"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hkey, &dwDisposition ) ;
  
  dwValue = m_StartupMode ;
  RetVal = RegSetValueEx ( hkey, _T("SelectLanguageMode"), NULL, REG_DWORD, (PBYTE)&dwValue, sizeof(DWORD) ) ;
  
  dwValue = MultiLang::LocaleId ;
  RetVal = RegSetValueEx ( hkey, _T("SelectLanguageLCID"), NULL, REG_DWORD, (PBYTE)&dwValue, sizeof(DWORD) ) ;

  RegCloseKey ( hkey ) ;
}

void CSelectLanguage::GetApplicationDirectory()
{
  //-------------------------------------------------------
  // Get the application directory
  //-------------------------------------------------------

  // Get the path of the application
  TCHAR szBuffer[MAX_PATH] ;
  GetModuleFileName ( NULL, szBuffer, MAX_PATH ) ;

  // Prefer to work with CString objects
  CString sBuffer ( szBuffer ) ;

  // Find the last backslash
  int     iLastSlash = sBuffer.ReverseFind ( '\\' ) ;
  int     iLastDot = sBuffer.ReverseFind ( '.' ) ;

  // Extract the directory to a member variable. Leave the trailing backslash
  m_ApplicationDirectory = sBuffer.Left ( iLastSlash + 1 ) ;
  // Also extract the application name. This time leave the leading backslash.
  m_ApplicationName = sBuffer.Mid ( iLastSlash, iLastDot - iLastSlash ) ;
}

void CSelectLanguage::SetLanguageForMFC()
{
  // Build a path for the satellite dll.
  CString sSatellitePath = m_ApplicationDirectory + m_SelectedIetfTag + m_ApplicationName + "_" + m_SelectedIetfTag + ".dll" ;

  // Does this file exist?
  WIN32_FIND_DATA WFD ;
  if ( FindFirstFile ( sSatellitePath, &WFD ) != INVALID_HANDLE_VALUE )
  {
    HMODULE   hDLL = LoadLibrary ( sSatellitePath ) ;
    if ( hDLL )
    {
      AfxSetResourceHandle ( hDLL ) ;
    }
  }
}

// CSelectLanguage message handlers

BOOL CSelectLanguage::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  
  //
  // The array SupportedCultures in MlString.cpp contains a list of
  // cultures as IETF language tags.
  // (See http://en.wikipedia.org/wiki/IETF_language_tag)
  //
  // In unmanaged C++ we can handle these as RTF1766 names and
  // interpret them using the IMultiLanguage interface, defined
  // in the mlang.h header file.
  //
  // This probably requires the Platform SDK.
  // ----------------------------------------
  //
  // Note that these functions don't seem to handle neutral cultures.
  // For "de", GetLcidFromRfc1766() will return LCID=0x0407, equivalent 
  // to "de-DE". If you convert it to 0x0007, then GetRfc1766Info()
  // will return an error.
  //
  // However, FindResourceEx for LCID=0x0407 will nevertheless find
  // resources for LCID=0x0007, so the behaviour if reasonably 
  // consistent, if odd.
  //
  // See also
  // http://blogs.msdn.com/michkap/archive/2004/12/29/343557.aspx
  // http://blogs.msdn.com/michkap/archive/2005/06/13/428570.aspx
  // http://blogs.msdn.com/michkap/archive/2005/08/31/458266.aspx
  //

  HRESULT           hr;
  bool              CurrentLocaleFound = false ;

  //-------------------------------------------------------
  // Initialise the list box based on the SupportedCultures 
  // array, generted by the Add-In.
  //-------------------------------------------------------
  for ( int i = 0 ; i < NumberOfCultures ; i++ )
  {
    // Check whether the satellite dll is actually present.
    CString sSatellitePath = m_ApplicationDirectory + SupportedCultures[i] + m_ApplicationName + "_" + SupportedCultures[i] + ".dll" ;

    // Only add the language to the list if the satellite dll actually exists.
    WIN32_FIND_DATA WFD ;
    if ( FindFirstFile ( sSatellitePath, &WFD ) != INVALID_HANDLE_VALUE )
    {
      LCID          Id ;
      RFC1766INFO   Info ;
      int           ListIndex ;
      hr = m_pMlang->GetLcidFromRfc1766 ( &Id, _bstr_t(SupportedCultures[i]) ) ;
      if ( SUCCEEDED(hr) )
      {
        // mlang.dll knows this IetfTag
        hr = m_pMlang->GetRfc1766Info ( Id, &Info ) ;
        CString sLocaleName ( Info.wszLocaleName ) ;
        ListIndex = m_ListCultures.AddString ( sLocaleName ) ;
      }
      else
      {
        // mlang.dll does not knows this IetfTag
        // Using satellite Dlls, we don't fundamentally need this 
        // anyway. For now, just display the IetfTag in the list.
        // Alternatively, we could get the Add-In to generate a 
        // list of language names.
        Id        = 0 ;
        ListIndex = m_ListCultures.AddString ( SupportedCultures[i] ) ;
      }  
      m_ListCultures.SetItemDataPtr ( ListIndex, (void*)(SupportedCultures[i]) ) ;
    
      if ( Id == MultiLang::LocaleId )
      {
        CurrentLocaleFound = true ;
        m_ListCultures.SetCurSel ( ListIndex ) ;
        m_SelectedIetfTag  = SupportedCultures[i] ;
      }
    }
  }
  
  if ( !CurrentLocaleFound )
  {
    m_ListCultures.SetCurSel ( 0 ) ;
    m_SelectedIetfTag   = (const TCHAR *)m_ListCultures.GetItemDataPtr ( 0 ) ;
    m_pMlang->GetLcidFromRfc1766 ( &MultiLang::LocaleId, _bstr_t(m_SelectedIetfTag) ) ;
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectLanguage::OnLbnSelchangeListCultures()
{
  int   ListIndex = m_ListCultures.GetCurSel() ;
  if ( ListIndex != LB_ERR )
  {
    m_SelectedIetfTag   = (TCHAR*)m_ListCultures.GetItemDataPtr ( ListIndex ) ;
    m_pMlang->GetLcidFromRfc1766 ( &MultiLang::LocaleId, _bstr_t(m_SelectedIetfTag) ) ;
  }
}

