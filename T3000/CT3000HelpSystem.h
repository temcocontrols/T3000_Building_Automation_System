
#ifndef T3000HELPSYSTEM
#define T3000HELPSYSTEM 0x0800	// (in case you don't have the new commctrl.h)
#endif


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
