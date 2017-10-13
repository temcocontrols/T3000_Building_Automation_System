////////////////////////////////////////////////////////////////
// CT3000HelpSystem 1997 Microsoft Systems Journal. 
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#include "StdAfx.h"
#include "T3000HelpSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////
// CT3000HelpSystem--does flat tool bar in MFC.
//
IMPLEMENT_DYNAMIC(CT3000HelpSystem, CToolBar)

BEGIN_MESSAGE_MAP(CT3000HelpSystem, CToolBar)
  ON_WM_WINDOWPOSCHANGING()
  ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

////////////////
// Load override modifies the style after loading toolbar.
//
BOOL CT3000HelpSystem::LoadToolBar(LPCTSTR resource_name)
{
  if (!CToolBar::LoadToolBar(resource_name))
    return FALSE;
  ModifyStyle(0, TBSTYLE_FLAT); // make it flat
  return TRUE;
}

//#define ILLUSTRATE_DISPLAY_BUG			 // remove comment to see the bug

//////////////////
// MFC doesn't handle moving a TBSTYLE_FLAT toolbar correctly.
// The simplest way to fix it is to repaint the old rectangle and
// toolbar itself whenever the toolbar moves.
// 
void CT3000HelpSystem::OnWindowPosChanging(LPWINDOWPOS lpwp)
{
  CToolBar::OnWindowPosChanging(lpwp);

#ifndef ILLUSTRATE_DISPLAY_BUG

  if (!(lpwp->flags & SWP_NOMOVE))        // if moved:
  {
    CRect rc;                             //   Fill rectangle with..
    GetWindowRect(&rc);                   //   ..my (toolbar) rectangle.
    CWnd* parent = GetParent();          //   get parent (dock bar/frame) win..
    parent->ScreenToClient(&rc);         //   .. and convert to parent coords

    // Ask parent window to paint the area beneath my old location.
    // Typically, this is just solid grey. The area won't get painted until
    // I send WM_NCPAINT after the move, in OnWindowPosChanged below.
    //
    parent->InvalidateRect(&rc);         // paint old rectangle
  }
#endif
}

//////////////////
// Now toolbar has moved: repaint old area
//
void CT3000HelpSystem::OnWindowPosChanged(LPWINDOWPOS lpwp)
{
  CToolBar::OnWindowPosChanged(lpwp);

#ifndef ILLUSTRATE_DISPLAY_BUG
  if (!(lpwp->flags & SWP_NOMOVE))        // if moved:
  {
    // Now paint my non-client area at the new location.
    // This is the extra bit of border space surrounding the buttons.
    // Without this, you will still have a partial display bug (try it!)
    //
    SendMessage(WM_NCPAINT);
  }
#endif
}

////////////////////////////////////////////////////////////////
// The following stuff is to make the command update UI mechanism
// work properly for flat tool bars. The main idea is to convert
// a "checked" button state into a "pressed" button state. Changed 
// lines marked with "PD"

////////////////
// The following class was copied from BARTOOL.CPP in the MFC source.
// All I changed was SetCheck--PD.
//
class CFlatOrCoolBarCmdUI : public CCmdUI // class private to this file !
{
public: // re-implementations only
  virtual void Enable(BOOL on);
  virtual void SetCheck(int check);
  virtual void SetText(LPCTSTR lpszText);
};


void CFlatOrCoolBarCmdUI::Enable(BOOL on)
{
  m_bEnableChanged = TRUE;
  CToolBar* tool_bar = (CToolBar*)m_pOther;
  ASSERT(tool_bar != NULL);
  ASSERT_KINDOF(CToolBar, tool_bar);
  ASSERT(m_nIndex < m_nIndexMax);

  TBBUTTON btn;
  tool_bar->GetToolBarCtrl().GetButton(m_nIndex, &btn);
  BYTE byNewStyle= on ? btn.fsState | TBSTATE_ENABLED : btn.fsState & ~TBSTATE_ENABLED;
  if (byNewStyle != btn.fsState)
  {
    tool_bar->GetToolBarCtrl().EnableButton(btn.idCommand, on);
    if (!on)
      tool_bar->GetToolBarCtrl().Indeterminate(btn.idCommand, false);
  }
}

// Take your pick:
//#define MYTBBS_CHECKED TBBS_CHECKED			// use "checked" state
//#define MYTBBS_CHECKED TBBS_PRESSED			// use pressed state
//#define MYTBBS_CHECKED (TBBS_PRESSED | TBBS_CHECKED)

//////////////////
// This is the only function that has changed: instead of TBBS_CHECKED,
// I use TBBS_PRESSED--PD
//
void CFlatOrCoolBarCmdUI::SetCheck(int check)
{
  ASSERT(check >= 0 && check <= 2); // 0=>off, 1=>on, 2=>indeterminate
  CToolBar* tool_bar = (CToolBar*)m_pOther;
  ASSERT(tool_bar != NULL);
  ASSERT_KINDOF(CToolBar, tool_bar);
  ASSERT(m_nIndex < m_nIndexMax);

  TBBUTTON btn;
  tool_bar->GetToolBarCtrl().GetButton(m_nIndex, &btn);
  BYTE byNewStyle= btn.fsState;
  switch (check)
  {
  case 0:   // off
    byNewStyle &= ~(TBSTATE_CHECKED);
    break;
  case 1:   // on
    byNewStyle |= TBSTATE_CHECKED;
    break;
  case 2:   // indeterminate
    byNewStyle |= TBSTATE_INDETERMINATE;
    break;
  }
  if (byNewStyle != btn.fsState)
      tool_bar->GetToolBarCtrl().SetState(btn.idCommand, byNewStyle);
/*    if (check != 2)
    {
      btn.fsState &= ~TBSTATE_INDETERMINATE;
      tool_bar->GetToolBarCtrl().SetState(btn.idCommand,
        check == 0 ? btn.fsState & ~(TBSTATE_PRESSED | TBSTATE_CHECKED)
                    : btn.fsState | TBSTATE_PRESSED | TBSTATE_CHECKED);
    }
    else
    {
      if ((btn.fsState & TBSTATE_INDETERMINATE) == 0)
      {
        tool_bar->GetToolBarCtrl().SetState(btn.idCommand,
          (btn.fsState & ~(TBSTATE_PRESSED | TBSTATE_CHECKED)) | TBSTATE_INDETERMINATE);
      }
    } */

/*
  UINT old_style = tool_bar->GetButtonStyle(m_nIndex); // PD
  UINT new_style = old_style &
				~(MYTBBS_CHECKED | TBBS_INDETERMINATE); // PD
  if (check == 1)
    new_style |= MYTBBS_CHECKED; // PD
  else if (check == 2)
    new_style |= TBBS_INDETERMINATE;

  // Following is to fix display bug for TBBS_CHECKED:
  // If new state is unchecked, repaint--but only if style actually changing.
  // (Otherwise will end up with flicker)
  // 
  if (new_style != old_style)
  {
    ASSERT(!(new_style & TBBS_SEPARATOR));
    tool_bar->SetButtonStyle(m_nIndex, new_style);
    tool_bar->Invalidate();
  }
*/
}

void CFlatOrCoolBarCmdUI::SetText(LPCTSTR)
{
	// ignore for now, but you should really set the text
}

//////////////////
// This function is mostly copied from CToolBar/BARTOOL.CPP. The only thing
// that's different is I instantiated a CFlatOrCoolBarCmdUI instead of
// CToolCmdUI.
//

void CT3000HelpSystem::OnUpdateCmdUI(CFrameWnd* target, BOOL disable_if_no_hndler)
{
	CFlatOrCoolBarCmdUI state; // <<<< This is the only line that's different--PD
	state.m_pOther = this;

	state.m_nIndexMax = (UINT)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		// get button state
		TBBUTTON button;
		VERIFY(DefWindowProc(TB_GETBUTTON, state.m_nIndex, (LPARAM)&button));
		// TBSTATE_ENABLED == TBBS_DISABLED so invert it
		button.fsState ^= TBSTATE_ENABLED;

		state.m_nID = button.idCommand;

		// ignore separators
		if (!(button.fsStyle & TBSTYLE_SEP))
		{
			// allow the toolbar itself to have update handlers
			if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;

			// allow the owner to process the update
			state.DoUpdate(target, disable_if_no_hndler);
		}
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(target, disable_if_no_hndler);
}
