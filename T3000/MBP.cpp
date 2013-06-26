#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "MBP.h"
#include "AfxMessageDialog.h"

#if 0
#define SwitchMenuCheck(m, i)\
((m)->CheckMenuItem(i, m->GetMenuState(i,\
MF_BYCOMMAND) == MF_CHECKED ? 0 : MF_CHECKED)

#define SwitchMenu(x) do {\
const int nID = x;\
CMenu* menu = GetMenu();\
SwitchMenuCheck(menu, x);\
this->ForceResize();\
while (0)

#define SwitchCheck(x) do {\
const int nID = x;\
CMenu* menu = GetMenu();\
SwitchMenuCheck(menu, x);\
} while (0)
#endif

#if 0
IMPLEMENT_DYNAMIC(CMbp, CDialog)

CMbp::CMbp(CWnd* pParent /*=NULL*/)
	: CDialog(CMbp::IDD, pParent)
{
	
}

CMbp::~CMbp()
{
}

void CMbp::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_MBP1, m_MBP_FlexGrid);
}



BEGIN_MESSAGE_MAP(CMbp, CDialog)
	ON_BN_CLICKED(IDOK, &CMbp::OnBnClickedOk)
END_MESSAGE_MAP()


void CMbp::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//MessageBox(L"Hi", L"Hello", NULL);
	CDialog::OnOK();
}
BEGIN_EVENTSINK_MAP(CMbp, CDialog)
	ON_EVENT(CMbp, IDC_MBP1, DISPID_CLICK, CMbp::ClickMbp1, VTS_NONE)
END_EVENTSINK_MAP()



void CMbp::ClickMbp1()
{
	// TODO: Add your message handler code here
	int row;
	int col;

	row = m_MBP_FlexGrid.get_RowSel();
	col = m_MBP_FlexGrid.get_ColSel();

	//m_MBP_FlexGrid.

	CString rowStr;
	CString colStr;

	rowStr.Format(_T("%d"), row);
	colStr.Format(_T("%d"), col);

	MessageBox(rowStr, colStr, NULL);
}
#endif


CMbp::CMbp(CWnd* pParent/*=NULL*/ )
    : CDialog(CMbp::IDD, pParent)
{
    //{{AFX_DATA_INIT(C_Modeless)
    //}}AFX_DATA_INIT

    m_pMainWindow = pParent;
}

BOOL CMbp::Create()
{
    BOOL ReturnValue = CDialog::Create(CMbp::IDD, m_pMainWindow);
    UpdateData(FALSE);
    ShowWindow(SW_SHOWNA);
    return ReturnValue;
}

void CMbp::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(C_Modeless)
    //DDX_Radio(pDX, IDC_RED, m_nColor);
    //DDX_Text(pDX, IDC_TITLE, m_sTitle);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMbp, CDialog)
    //{{AFX_MSG_MAP(C_Modeless)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDCANCEL, OnClose)
	//ON_UPDATE_COMMAND_UI(ID_DISPLAY_SIGNED, OnUpdateDisplaySigned)
	//ON_COMMAND(ID_DISPLAY_SIGNED, OnCommandDisplaySigned)
    //ON_EN_KILLFOCUS(IDC_TITLE, OnKillfocusTitle)
/*    ON_BN_CLICKED(IDC_GREEN, OnGreen)
    ON_BN_CLICKED(IDC_RED, OnRed)
    ON_BN_CLICKED(IDC_YELLOW, OnYellow)
    ON_MESSAGE(WM_TODLG_RED, OnMainRed)
    ON_MESSAGE(WM_TODLG_GREEN, OnMainGreen)
    ON_MESSAGE(WM_TODLG_YELLOW, OnMainYellow)*/
    //}}AFX_MSG_MAP
	//ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

#if 0
void CMbp::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
    ASSERT(pPopupMenu != NULL);
    // Check the enabled state of various menu items.

    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    HMENU hParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
           // Child windows don't have menus--need to go to the top!
        if (pParent != NULL &&
           (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
           int nIndexMax = ::GetMenuItemCount(hParentMenu);
           for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
           {
            if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
            {
                // When popup is found, m_pParentMenu is containing menu.
                state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
                break;
            }
           }
        }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
      state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
           continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        if (state.m_nID == (UINT)-1)
        {
           // Possibly a popup menu, route to first item of that popup.
           state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
           if (state.m_pSubMenu == NULL ||
            (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
            state.m_nID == (UINT)-1)
           {
            continue;       // First item of popup can't be routed to.
           }
           state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
        }
        else
        {
           // Normal menu item.
           // Auto enable/disable if frame window has m_bAutoMenuEnable
           // set and command is _not_ a system command.
           state.m_pSubMenu = NULL;
           state.DoUpdate(this, FALSE);
        }

        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
           state.m_nIndex -= (state.m_nIndexMax - nCount);
           while (state.m_nIndex < nCount &&
            pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
           {
            state.m_nIndex++;
           }
        }
        state.m_nIndexMax = nCount;
    }
}
#endif			

void CMbp::OnCommandDisplaySigned()
{
//	SwitchMenu(ID_DISPLAY_SIGNED);
/*	static int a = 0;
	if (a == 0)
	{
		m_menu->CheckMenuItem(ID_DISPLAY_SIGNED, MF_CHECKED | MF_BYCOMMAND);
		a = 1;
	}
	else
	{
		m_menu->CheckMenuItem(ID_DISPLAY_SIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		a = 0;
	}*/
}


void CMbp::OnUpdateDisplaySigned(CCmdUI* pCmdUI)
{
	static int a = 0;
	if (a == 0)
	{
		pCmdUI->SetCheck(1);
		a = 1;
	}
	else
	{
		pCmdUI->SetCheck(0);
		a = 0;
	}
}

void CMbp::OnClose()
{
    DestroyWindow();
}

void CMbp::OnOK()
{
    // do not call the base class CDialog::OnOK()
    // providing this OnOK function with no functionality
    // disables the <enter> key, which would otherwise
    // close the modeless dialog box
}

//void CMbp::OnKillfocusTitle()
//{
//    UpdateData(TRUE);
    //m_pMainWindow->PostMessage(WM_FROMDLG_TEXT);
//}

void CMbp::OnDestroy()
{
    CDialog::OnDestroy();
    m_pMainWindow->PostMessage(WM_DLG_CLOSE);
}

BOOL CMbp::OnInitDialog()
{
	//mMbpEditCtrl.ShowWindow(SW_HIDE);
	return TRUE;
}
