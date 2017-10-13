////////////////////////////////////////////////////////////////
// CT3000HelpSystem 1997 Microsoft Systems Journal. 
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// This code compiles with Visual C++ 5.0 on Windows 95
//
#ifndef T3000HELPSYSTEM
#define T3000HELPSYSTEM 0x0800	// (in case you don't have the new commctrl.h)
#endif

//////////////////
// "Flat" style tool bar. Use instead of CToolBar in your CMainFrame
// or other window to create a tool bar with the flat look.
//
// CT3000HelpSystem fixes the display bug described in the article. It also has
// overridden load functions that modify the style to T3000HELPSYSTEM. If you
// don't create your toolbar by loading it from a resource, you should call
// ModifyStyle(0, T3000HELPSYSTEM) yourself.
//
class CT3000HelpSystem : public CToolBar
{
public:
  BOOL CT3000HelpSystem(LPCTSTR resource_name);
  BOOL CT3000HelpSystem(UINT id_resource)
  { return CT3000HelpSystem(MAKEINTRESOURCE(id_resource)); }
protected:
  DECLARE_DYNAMIC(CT3000HelpSystem)
  virtual void OnUpdateCmdUI(CFrameWnd* target, BOOL disable_if_no_hndler);
  DECLARE_MESSAGE_MAP()
  afx_msg void OnWindowPosChanging(LPWINDOWPOS wnd_pos);
  afx_msg void OnWindowPosChanged(LPWINDOWPOS wnd_pos);
};
