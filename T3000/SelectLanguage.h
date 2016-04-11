#pragma once
#include "afxwin.h"
#include <comdef.h>
#include "Resource.h"
#include "mlang.h"

// CSelectLanguage dialog

class CSelectLanguage : public CDialog
{
  DECLARE_DYNAMIC(CSelectLanguage)

public:
  CSelectLanguage(CWnd* pParent = NULL);   // standard constructor
  virtual ~CSelectLanguage();

  void  LoadSettingsAndShow ( bool ForceShow = false ) ;

// Dialog Data
  enum { IDD = IDD_SELECT_LANGUAGE };

private:

  enum enumStartupMode
  {
    ShowLangDialog    = 0,
    UseSavedCulture   = 1,
    UseDefaultCulture = 2
  } ;

  void  LoadSettings() ;
  void  SaveSettings() ;
  void  GetApplicationDirectory() ;
  void  SetLanguageForMFC() ;

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:
  CListBox                m_ListCultures;
  CStatic                 m_NextTime2;
  int                     m_StartupMode ;
  const TCHAR*            m_SelectedIetfTag ;
  CString                 m_ApplicationDirectory ;
  CString                 m_ApplicationName ;
  IMultiLanguage*         m_pMlang ;

  virtual BOOL OnInitDialog();
  afx_msg void OnLbnSelchangeListCultures();
};
