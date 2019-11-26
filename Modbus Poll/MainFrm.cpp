
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Modbus Poll.h"

#include "MainFrm.h"
#include "ConnectionSetup.h"
#include "global_variable_extern.h"
#include "ModbusDllForVC.h"
#include "RegisterValueAnalyzerDlg.h"
#include "CommunicationTrafficDlg.h"
#include "Modbus PollView.h"
#include "DeviecTesterConfigDlg.h"


 UINT _ReadMultiRegisters(LPVOID pParam)
 {
	  CMainFrame* pFrame=(CMainFrame*)(pParam);
	  pFrame->OnConnectionQuickconnectf5();
	  return 1;
 }

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 void Update_ViewData(CView* MBPollView);
 
	 DWORD WINAPI _Multi_Read_Fun03_MF(LPVOID pParam);
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_CONNECTION_CONNECT32776, &CMainFrame::OnConnectionConnect32776)
	ON_WM_DESTROY()
	ON_COMMAND(ID_CONNECTION_QUICKCONNECTF5, &CMainFrame::OnConnectionQuickconnectf5)
	ON_COMMAND(ID_CONNECTION_DISCONNECT, &CMainFrame::OnConnectionDisconnect)
	ON_UPDATE_COMMAND_UI(ID_CONNECTION_QUICKCONNECTF5, &CMainFrame::OnUpdateConnectionQuickconnectf5)
	ON_UPDATE_COMMAND_UI(ID_CONNECTION_DISCONNECT, &CMainFrame::OnUpdateConnectionDisconnect)
	ON_UPDATE_COMMAND_UI(ID_CONNECTION_CONNECT32776, &CMainFrame::OnUpdateConnectionConnect32776)
//	ON_COMMAND(ID_SETUP_READ32783, &CMainFrame::OnSetupRead32783)
ON_COMMAND(ID_DISPALY_COMMUNICATION, &CMainFrame::OnDispalyCommunication)
ON_UPDATE_COMMAND_UI(ID_DISPALY_COMMUNICATION, &CMainFrame::OnUpdateDispalyCommunication)
 
ON_COMMAND(ID_VIEW_REGISTERVALUEANALYZER, &CMainFrame::OnViewRegistervalueanalyzer)
ON_UPDATE_COMMAND_UI(IDS_CONNECTION, &CMainFrame::OnUpdateStatusBar)
ON_COMMAND(ID_FUNCTIONS_TESTCENTER, &CMainFrame::OnFunctionsTestcenter)
 
 
ON_COMMAND(ID_TOOLS_DEVICETESTER, &CMainFrame::OnToolsDevicetester)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_CONNECTION,
	//IDS_ADDINFOR,
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	 
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndMenuBar.SetPaneStyle()
	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	
	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//  Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
// 	CMFCToolBar::EnableQuickCustomization();
// 
// 	if (CMFCToolBar::GetUserImages() == NULL)
// 	{
// 		// load user-defined toolbar images
// 		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
// 		{
// 			CMFCToolBar::SetUserImages(&m_UserImages);
// 		}
// 	}

	// enable menu personalization (most-recently used commands)
	//  define your own basic commands, ensuring that each pulldown menu has at least one basic command.
// 	CList<UINT, UINT> lstBasicCommands;
// 
// 	lstBasicCommands.AddTail(ID_FILE_NEW);
// 	lstBasicCommands.AddTail(ID_FILE_OPEN);
// 	lstBasicCommands.AddTail(ID_FILE_SAVE);
// 	lstBasicCommands.AddTail(ID_FILE_PRINT);
// 	lstBasicCommands.AddTail(ID_APP_EXIT);
// 	lstBasicCommands.AddTail(ID_EDIT_CUT);
// 	lstBasicCommands.AddTail(ID_EDIT_PASTE);
// 	lstBasicCommands.AddTail(ID_EDIT_UNDO);
// 	lstBasicCommands.AddTail(ID_APP_ABOUT);
// 	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
// 	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
// 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
// 
// 	CMFCToolBar::SetBasicCommands(lstBasicCommands);

  	  //m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_NOBORDERS,   300);
  	    m_wndStatusBar.SetPaneInfo(1,IDS_CONNECTION,SBPS_NOBORDERS,   300);
// m_wndStatusBar.SetPaneInfo(1,ID_BUILDING_INFO,SBPS_NOBORDERS, 300);
//OnConnectionQuickconnectf5();

// 		if (!CreateDockingWindows())
// 		{
// 			TRACE0("Failed to create docking windows\n");
// 			return -1;
// 		}
// 
// 		m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
// 		DockPane(&m_wndFileView);
	    AfxBeginThread(_ReadMultiRegisters,this);



		if (m_MultiRead_handle != NULL)
			TerminateThread(m_MultiRead_handle, 0);
		m_MultiRead_handle = NULL;
		if (!m_MultiRead_handle)
		{
			m_MultiRead_handle = CreateThread(NULL, NULL, _Multi_Read_Fun03_MF, this, NULL, 0);
		}


		m_wndToolBar.ResetAll();
               
                 
	 

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	//  Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnConnectionConnect32776()
{   
	//if (m_communication_type==0)
	//{
	//	SetCommunicationType(0);
	//	close_com();
	//}
	//else{
	//	SetCommunicationType(1);
	//	close_com();
	//}
	CString strpannel;
	CConnectionSetup connectionsetup;
	if (IDOK==connectionsetup.DoModal())
	{
		m_communication_type=connectionsetup.m_Commication_Type;
		m_comport=_wtoi(connectionsetup.m_comport.Mid(3));
		m_bradrate=connectionsetup.m_bradrate;

		m_ipaddress=connectionsetup.m_ipaddress;
		m_port=connectionsetup.m_ipport;
		m_responsetimeout=connectionsetup.m_response_timeout;
        m_connecttimeout = connectionsetup.m_connect_timeout;
        m_between_time = connectionsetup.m_between_time;
		SetCommunicationType(m_communication_type);
		if (m_communication_type==0)
		{
			if (!open_com(m_comport))
			{
				
				strpannel.Format(_T("Port%d Can't open"),m_comport);
				AfxMessageBox(strpannel);
				m_isconnect=FALSE;
				g_online=FALSE;
				
				 
				SetPaneString(1,strpannel);
				 
				return;
			} 
			m_isconnect=TRUE;
			g_online=TRUE;
			Change_BaudRate(m_bradrate);
			  strpannel.Format(_T("Port%d :%d"),m_comport,m_bradrate);
			 SetPaneString(1,strpannel);
		} 
		else
		{
			if (!Open_Socket2(m_ipaddress,m_port))
			{
				CString temp;
				temp.Format(_T("%s:%d can't be connected"),m_ipaddress.GetBuffer(),m_port);
				AfxMessageBox(temp);
				strpannel=temp;
				SetPaneString(1,strpannel);
				m_isconnect=FALSE;
				g_online=FALSE;
				return;
			}
			m_isconnect=TRUE;
			g_online=TRUE;
			strpannel.Format(_T("IP%s :%d"),m_ipaddress,m_port);
			SetPaneString(1,strpannel);
		}
        
        SetResponseTime (m_responsetimeout);
	}
	else
	{
	   OnConnectionQuickconnectf5();
	}
}


void CMainFrame::OnDestroy()
{
	

 
	if(g_data_trafficdlg !=NULL)
	{
		delete g_data_trafficdlg;
		g_data_trafficdlg = NULL;
	}

	if (g_Draw_dlg!=NULL)
	{
		delete g_Draw_dlg;
		g_Draw_dlg=NULL;
	}

	if (GetLastCommunicationType()!=0)
	{
		SetCommunicationType(1);
		close_com();
	} 
	else
	{
		SetCommunicationType(0);
		close_com();
	}
	 
    if(m_MultiRead_handle != NULL)
        TerminateThread(m_MultiRead_handle, 0);
    m_MultiRead_handle=NULL;
	CMDIFrameWndEx::OnDestroy();
}

void CMainFrame::Read_Config(){
	CFileFind fFind;
	if(!fFind.FindFile(g_configfile_path))
	{
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("0"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),_T("COM1"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),_T("1"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);


		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),_T("127.0.0.1"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),_T("502"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
	}

	CString comport;
	m_bradrate=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Baudrate"),19200,g_configfile_path);
	m_communication_type=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Connection Type"),2,g_configfile_path);
	m_comport=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("COM_Port"),1,g_configfile_path);
	GetPrivateProfileString(_T("MBPOLL_Setting"),_T("IP Address"),_T("127.0.0.1"),m_ipaddress.GetBuffer(MAX_PATH),MAX_PATH,g_configfile_path);
    m_ipaddress.ReleaseBuffer();
	m_port=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("IP Port"),502,g_configfile_path);
	m_connecttimeout=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Connect Timeout"),1000,g_configfile_path);
    m_responsetimeout =GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Response Timeout"),3000,g_configfile_path);

}
void CMainFrame::OnConnectionQuickconnectf5()
{
	if (g_online)
	{
		return;
	}
	Read_Config();
	SetCommunicationType(m_communication_type);
	CString temp;
	if (m_communication_type==0)
	{
		
		if (!open_com(m_comport))
		{
			temp.Format(_T("COM%d Can't Open"),m_comport);
			AfxMessageBox(temp);

			CString strpannel;
			strpannel=temp;
			SetPaneString(1,strpannel);
			g_StrConnection=strpannel;
			m_isconnect=FALSE;
			g_online=FALSE;
			return;
		} 
		m_isconnect=TRUE;
		g_online=TRUE;
		Change_BaudRate(m_bradrate);

		CString strpannel;
		strpannel.Format(_T("ComPort %d: Brandrate %d"),m_comport,m_bradrate);
		g_StrConnection=strpannel;
		SetPaneString(1,strpannel);
	} 
	else
	{
		if (!Open_Socket2(m_ipaddress,m_port))
		{
			temp.Format(_T("%s can't be connected"),m_ipaddress.GetBuffer());
            m_ipaddress.ReleaseBuffer();
			AfxMessageBox(temp);
			CString strpannel;
			strpannel=temp;
			g_StrConnection=strpannel;
			SetPaneString(1,strpannel);
			m_isconnect=FALSE;
			g_online=FALSE;
			return;
		}
		CString strpannel;
		strpannel.Format(_T("IP: %s-Port:%d "),m_ipaddress,m_port);
		g_StrConnection=strpannel;
		SetPaneString(1,strpannel);
		m_isconnect=TRUE;
		g_online=TRUE;
	}

    SetResponseTime (m_responsetimeout);
}


void CMainFrame::OnConnectionDisconnect()
{
	  if (!g_online)
	  {
	  	return;
	  }
	  SetCommunicationType(0);
	  close_com();
	  SetCommunicationType(1);
	  close_com();
	  g_online=FALSE;
	  m_isconnect=FALSE;
	  CString strpannel;
	  strpannel.Format(_T("Disconnected"));
	  SetPaneString(1,strpannel);
}


void CMainFrame::OnUpdateConnectionQuickconnectf5(CCmdUI *pCmdUI)
{
	 pCmdUI->SetCheck(m_isconnect==TRUE);
}


void CMainFrame::OnUpdateConnectionDisconnect(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_isconnect==FALSE);
}

void CMainFrame::OnUpdateConnectionConnect32776(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_isconnect==TRUE);
}



void CMainFrame::Show_Data_Traffic_Window(){

	if (g_data_trafficdlg==NULL)
	{
		g_data_trafficdlg=new CCommunicationTrafficDlg;
		g_data_trafficdlg->Create(IDD_DIALOG_COMMUNICATION_TRAFFIC, this);
		g_data_trafficdlg->ShowWindow(SW_SHOW);
		g_is_show_Data_Traffic_Window=TRUE;
	} 
	 
		if(g_data_trafficdlg->IsWindowVisible())
		{

		}

		else
		{
			g_is_show_Data_Traffic_Window=TRUE;
			g_data_trafficdlg->ShowWindow(SW_SHOW);
		}
	 

		
}

void CMainFrame::OnDispalyCommunication()
{
	 Show_Data_Traffic_Window();
}


void CMainFrame::OnUpdateDispalyCommunication(CCmdUI *pCmdUI)
{
	//  Add your command update UI handler code here
	//pCmdUI->SetCheck(g_is_show_Data_Traffic_Window==TRUE);
	if (g_is_show_Data_Traffic_Window)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{

		pCmdUI->Enable(TRUE);
	}
}

volatile HANDLE Read_Mutex=NULL;
DWORD WINAPI _Multi_Read_Fun03_MF(LPVOID pParam){
	while(TRUE){
	//	Sleep(500);
		CObList lstFrames;
		CDocManager *pDocManger=AfxGetApp()->m_pDocManager;
		POSITION pos=pDocManger->GetFirstDocTemplatePosition();
		while(pos){
			CDocTemplate *pTemplate=(CDocTemplate*)pDocManger->GetNextDocTemplate(pos);
			{
				POSITION pos=pTemplate->GetFirstDocPosition();
				while(pos){
					CDocument *pDoc=pTemplate->GetNextDoc(pos);
					if (pDoc!=NULL)
					{
						 
						  
						 POSITION pos = pDoc->GetFirstViewPosition();
						  
						  while (pos) {
							CView *pMBPollView = pDoc->GetNextView(pos);
							if (pMBPollView != NULL)
							{
  								Update_ViewData(pMBPollView);
  								TRACE("Update_ViewData\n");
							}

						   }
						
						
					}
				}
			}
		}
	}


	return 0;
}

void Update_ViewData(CView* MBPollView){
	CModbusPollView* pMBPollView=(CModbusPollView*)(MBPollView);

	int ID;
	unsigned short DataBuffer[127];
	unsigned short startAdd;
	unsigned short quantity;
	int sleep;
	int functioncode;

	int Send_length;
	int Rev_length;

	CString temp;

	unsigned char rev_back_rawData[300],send_data[100];

 
	 
		if (pMBPollView->m_hWnd==NULL)
		{
			Sleep(30);
			return;
		}


		if (!g_online)
		{
			Sleep(30);
			if (pMBPollView->m_hWnd!=NULL)
			{
				::SendMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
			}
			return;
		}
		//DataBuffer=pMBPollView->m_DataBuffer;
        static bool run_loadid_once = false;  //解决Modbus ID 一直固定从ini读取的问题.只有开始第一次从ini读取
        if (!run_loadid_once)
        {
            pMBPollView->m_Slave_ID = GetPrivateProfileInt(_T("MBPOLL_Setting"), _T("Modbus ID"), 255, g_configfile_path);
            run_loadid_once = true;
        }
        
		ID=pMBPollView->m_Slave_ID;
		startAdd=pMBPollView->m_address;
		quantity=pMBPollView->m_Quantity;
		sleep=pMBPollView->m_Scan_Rate;
		functioncode = pMBPollView->m_MBPoll_Function;

		 
		//read_multi_log(ID,&pMBPollView->m_DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		//read_multi_tap(ID,&DataBuffer[0],startAdd,quantity);
		CString m_Tx,m_Rx;
		int ret=0;
		if (pMBPollView->m_apply)
		{
			memset(DataBuffer, 0, 127);
			register_critical_section.Lock();
			if (pMBPollView->m_PLC_Addresses==1)
			{
				//ret=read_multi_log(ID,&DataBuffer[0],startAdd-1,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
                for (int j = 0; j < 3; j++)
                {
                    ret = Modbus_Standard_Read(ID, &DataBuffer[0], functioncode, startAdd - 1, quantity, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);
                    if (ret >= 0)
                        break;
                    else
                    {
                        Sleep(100);
                    }
                }
			}
			else
			{
				//ret = read_multi_log(ID,&DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
                for (int  j = 0; j < 3; j++)
                {
                    ret = Modbus_Standard_Read(ID, &DataBuffer[0], functioncode, startAdd, quantity, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);
                    if (ret >= 0)
                        break;
                    else
                    {
                        Sleep(100);
                    }
                }
				

			}
			register_critical_section.Unlock();
		} 
		else
		{

		  if (pMBPollView->m_wronce)
		  {
			  memset(DataBuffer, 0, 127);
			  register_critical_section.Lock();
			  if (pMBPollView->m_PLC_Addresses==1)
			  {
				  ret=read_multi_log(ID,&DataBuffer[0],startAdd-1,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
			  }
			  else
			  {
				  ret=read_multi_log(ID,&DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
			  }
			  register_critical_section.Unlock();
			  pMBPollView->m_wronce=FALSE;
		  }
		  else
		  {
		     return;
		  }
		  
		}
		
		/*
		-1:no connection
		-2:create write error
		-3:create read error
		-4:time out error
		-5:crc error
		*/
		
 
	
	    pMBPollView->m_MultiReadReturnType=ret;
			
	
		++g_Tx_Rx;
		temp.Format(_T("Tx:%06d--"),g_Tx_Rx);
		m_Tx+=temp;

		for (int i=0;i<Send_length;i++)
		{
			temp.Format(_T("%02X "),send_data[i]);
			m_Tx+=temp;
		}
 	    Traffic_Data(m_Tx);


		++g_Tx_Rx;
		temp.Format(_T("Rx:%06d--"),g_Tx_Rx);
		m_Rx+=temp;

		for(int i=0;i<Rev_length;i++){
			temp.Format(_T("%02X "),rev_back_rawData[i]);
			m_Rx+=temp;
		}

		Traffic_Data(m_Rx);

		 register_critical_section.Lock();
		int tt=read_multi_log(ID,&pMBPollView->m_modeldata[0],6,2,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		if (tt>0)
		{
			pMBPollView->m_isgetmodel=TRUE;
		}
		else
		{
			pMBPollView->m_modeldata[0] = 0;
			pMBPollView->m_modeldata[1] = 0;
			pMBPollView->m_isgetmodel=FALSE;
		}
		register_critical_section.Unlock();

		if (ret>0)//读的正确之后，我们才把值传给view显示
		{
			memcpy_s(pMBPollView->m_DataBuffer,sizeof(pMBPollView->m_DataBuffer),DataBuffer,sizeof(DataBuffer));
			++pMBPollView->m_Tx;
		} 
		else
		{
			memset(DataBuffer, 0, 127);
			memcpy_s(pMBPollView->m_DataBuffer, sizeof(pMBPollView->m_DataBuffer), DataBuffer, sizeof(DataBuffer));
			++pMBPollView->m_Tx;
			++pMBPollView->m_Err;

		}
		Sleep(sleep);
		
		if (pMBPollView->m_hWnd!=NULL)
		{
			::SendMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
		}

	 
}
void CMainFrame::OnViewRegistervalueanalyzer()
{

	//OnConnectionConnect32776();
    if (g_Draw_dlg==NULL)
    {

        g_Draw_dlg=new CRegisterValueAnalyzerDlg;
        g_Draw_dlg->Create(IDD_DIALOG_CHART,this);
        g_Draw_dlg->ShowWindow(SW_SHOW);
       //::PostMessage(g_Draw_dlg->m_hWnd,MY_FRESH_DRAW_GRAPHIC,0,0);
    } 
    else{

        if(g_Draw_dlg->IsWindowVisible())
        {

        }

        else
        {
            g_is_show_Data_Traffic_Window=TRUE;
            g_Draw_dlg->ShowWindow(SW_SHOW);
            //::PostMessage(g_Draw_dlg->m_hWnd,MY_FRESH_DRAW_GRAPHIC,0,0);
        }
    }
}
void CMainFrame::OnUpdateStatusBar(CCmdUI *pCmdUI){
	pCmdUI->Enable(TRUE);

}
 #include "TestCenter.h"
void CMainFrame::OnFunctionsTestcenter()
{
	  TestCenter dlg;
      dlg.DoModal ();
}


 




void CMainFrame::OnToolsDevicetester()
{
	 CDeviecTesterConfigDlg dlg;
	 dlg.DoModal();
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	 
	 

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

 

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	 

}

