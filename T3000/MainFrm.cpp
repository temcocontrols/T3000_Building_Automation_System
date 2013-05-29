 
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "T3000.h"

#include "MainFrm.h"
#include "T3000Doc.h"
#include "T3000TableView.h"
#include "T3000view.h"
#include "WorkspaceBar.h"
#include "NetworkControllView.h"

#include "global_variable_extern.h"
#include "globle_function.h"
#include "AddBuilding.h"
#include "ScanSelectDlg.h"

#include "MannageBuidingDlg.h"
#include "AllNodesDiaolg.h"
#include "GridLoad.h"
#include "GridFlashDlg.h"
#include "ScanDbWaitDlg.h"
#include "ChangePasswordDlg.h"
#include "ManageAccountDlg.h"
#include "LoginDlg.h"
#include "AfxMessageDialog.h"
#include "ImportDatabaseDlg.h"
#include "ToolCalibrateDlg.h"
#include "EreaseDlg.h"
#include "ToolCalibrateDlg.h"
#include "hangeIDDlg.h"
#include "LightingController/LightingController.h"//Lightingcontroller
#include "HumChamber.h"
#include "CO2_View.h"

#include "Dialog_Progess.h"

#include "excel9.h"

//////////////////////////////
#include "isp/CDialogISPTOOL.h"

#pragma region Fance Test
//For Test



HANDLE hStartEvent; // thread start event

vector <MSG> My_Receive_msg;


HANDLE hDeal_thread;
DWORD nThreadID_Do;

CCriticalSection MyCriticalSection;

//End for Test
#pragma endregion 
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

// #define _CRTDBG_MAP_ALLOC
// #include "stdlib.h"
// #include "crtdbg.h"
#include "T3000RegAddress.h"
T3000RegAddress MyRegAddress;

#include "ScanDlg.h"
#pragma comment(lib, "Version.lib")

// #include "productDB/ProInterface.h"
// #pragma comment(lib,"productDB/ProductDBdll.lib")


#ifdef _DEBUG
#define new DEBUG_NEW


#endif
// CMainFrame
#define REFRESH_TIMER	1
#define REFRESH_TIMER_VALUE	 35000
#define ID_NODEFAULTMAINBUILDING_MSG 8000
#define SCAN_TIMER 2

CEvent g_killMultiReadEvent;

volatile HANDLE Read_Mutex=NULL;
//CEvent g_pauseMultiReadEvent;
extern CT3000TableView* pTableView;
//BOOL g_bPauseMultiRead=FALSE;

//tree0412
#if 0//原有
#define TVINSERV_BUILDING 		{tvInsert.item.iImage=0;tvInsert.item.iSelectedImage=0;}
#define TVINSERV_FLOOR	 			{tvInsert.item.iImage=0;tvInsert.item.iSelectedImage=0;}
#define TVINSERV_ROOM				{tvInsert.item.iImage=1;tvInsert.item.iSelectedImage=1;}
#define TVINSERV_TSTAT 			{tvInsert.item.iImage=2;tvInsert.item.iSelectedImage=2;}
#define TVINSERV_LED 				{tvInsert.item.iImage=3;tvInsert.item.iSelectedImage=3;}
#define TVINSERV_NET_WORK		{tvInsert.item.iImage=5;tvInsert.item.iSelectedImage=5;}
#define TVINSERV_SOLAR				{tvInsert.item.iImage=5;tvInsert.item.iSelectedImage=5;}
#define TVINSERV_CMFIVE			{tvInsert.item.iImage=4;tvInsert.item.iSelectedImage=4;} //CM5
#define TVINSERV_MINIPANEL		{tvInsert.item.iImage=7;tvInsert.item.iSelectedImage=7;} //MiniPanel
#endif

//tree0412
#if 1
#define TVINSERV_BUILDING 		{tvInsert.item.iImage=0;tvInsert.item.iSelectedImage=0;}
#define TVINSERV_FLOOR	 			{tvInsert.item.iImage=0;tvInsert.item.iSelectedImage=0;}
#define TVINSERV_ROOM				{tvInsert.item.iImage=2;tvInsert.item.iSelectedImage=2;}
#define TVINSERV_TSTAT 			{tvInsert.item.iImage=6;tvInsert.item.iSelectedImage=6;}
#define TVINSERV_LED 				{tvInsert.item.iImage=8;tvInsert.item.iSelectedImage=8;}//TSTAT6&7
#define TVINSERV_NET_WORK		{tvInsert.item.iImage=12;tvInsert.item.iSelectedImage=12;}
#define TVINSERV_SOLAR				{tvInsert.item.iImage=12;tvInsert.item.iSelectedImage=12;}
#define TVINSERV_CMFIVE			{tvInsert.item.iImage=10;tvInsert.item.iSelectedImage=10;} //CM5
#define TVINSERV_MINIPANEL		{tvInsert.item.iImage=14;tvInsert.item.iSelectedImage=14;} //MiniPanel
#define TVINSERV_LC				{tvInsert.item.iImage=16;tvInsert.item.iSelectedImage=16;} //Lightingcontroller
#define TVINSERV_TSTAT6			{tvInsert.item.iImage=18;tvInsert.item.iSelectedImage=18;}//tstat6
#define TVINSERV_CO2			{tvInsert.item.iImage=20;tvInsert.item.iSelectedImage=20;}//CO2
#endif

#define ITEM_MASK				TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset) // 工具栏的按钮添加文字的方法
	ON_COMMAND(ID_BUILDINGBAR,OnCheckBuildingBar)
	ON_UPDATE_COMMAND_UI(ID_BUILDINGBAR, OnUpdateCheckBuildingBar)
	ON_COMMAND(ID_INPUTOUTPUTPANE,OnCheckIOPane)
	ON_UPDATE_COMMAND_UI(ID_INPUTOUTPUTPANE, OnUpdateCheckIOPane)

	ON_COMMAND(ID_FILE_SAVE_CONFIG, OnFileOpen)
	ON_COMMAND(ID_CONFIGFILE_SAVE_AS, SaveConfigFile)
	ON_COMMAND(ID_LOAD_CONFIG,OnLoadConfigFile)
	ON_COMMAND(ID_CONNECT2,OnConnect)
	ON_COMMAND(ID_DISCONNECT2,OnDisconnect)
	ON_COMMAND(ID_SCAN,OnScanDevice)
	//ON_COMMAND(ID_TOOL_SCAN,OnScanDevice)
	ON_COMMAND(ID_BUILDINGCONFIGDB,OnAddBuildingConfig)
	ON_COMMAND(ID_ALLNODESDATABASE,OnAllNodeDatabase)
	ON_COMMAND( ID_FILE_LOADCONFIGFILE,OnLoadConfigFile)
	ON_COMMAND(ID_FILE_BATCHBURNHEX,OnBatchFlashHex)
	ON_COMMAND(ID_FILE_IMPORTDATAFROMDATABASEFILE,OnImportDatase)

	ON_COMMAND(ID_RW_INFO, OnLabel)
	ON_COMMAND(ID_BUILDING_INFO, OnLabe2)
	ON_COMMAND(ID_PROTOCOL_INFO, OnLabe3)
	ON_MESSAGE(WM_USER_FRESHSTATUSBAR,OnFreshStatusBar)
	ON_MESSAGE(WM_ADDTREENODE, OnAddTreeNode)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_USERACCOUNT_CHANGEPASSWORD,OnChangeAdminPassword)
	ON_COMMAND(ID_USERACCOUNT_MANNAGEUSER,OnUserMannageMent)
	ON_UPDATE_COMMAND_UI(ID_USERACCOUNT_MANNAGEUSER,OnUserMannageMentUpdate)
	ON_COMMAND(ID_USERACCOUNT_LOGIN,OnUserLogin)
	ON_COMMAND(ID_HELP,OnHelp)
	ON_COMMAND(ID_TOOL_CALIBRATE,OnCaliBrate)
	ON_COMMAND(ID_TOOL_GLOABALOVERRIDE,OnGloabalOverrides)
	ON_COMMAND(ID_TOOL_EREASE,OnToolErease)
	ON_COMMAND(ID_TOOL_FRESH,OnToolFresh)
	ON_COMMAND(ID_TOOL_FRESHLEFTTREEVIEW,OnToolRefreshLeftTreee)

	//ON_COMMAND(ID_DATABASE_TEMCOPRODUCTS, &CMainFrame::OnDatabaseTemcoproducts)
	//ON_COMMAND(ID_FILE_ISPTOOL, &CMainFrame::OnFileIsptool)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	//ID_SEPARATOR,           // status line indicator
	ID_RW_INFO,
	ID_BUILDING_INFO,
	ID_PROTOCOL_INFO,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

unsigned short tempchange[512];

UINT _ReadMultiRegisters(LPVOID pParam)
{
	CMainFrame* pFrame=(CMainFrame*)(pParam);
	BOOL bFirst=TRUE;
	Read_Mutex=CreateMutex(NULL,TRUE,_T("Read_Multi_Reg"));	//Add by Fance .
	//forbid  _ReadMultiRegisters  and _FreshTreeView access com port at the same time.
	ReleaseMutex(Read_Mutex);//Add by Fance .
	while(1)
	{

		if(::WaitForSingleObject(g_killMultiReadEvent,0)==WAIT_OBJECT_0)
			break;		

		g_bEnableRefreshTreeView = TRUE;
		if(!bFirst)
			Sleep(30*1000);
		bFirst=FALSE;
			g_bEnableRefreshTreeView = FALSE;

		if(g_bPauseMultiRead)
			continue;

		if(g_tstat_id_changed)
			continue;
		if(g_tstat_id<0&&g_tstat_id>255)
			continue;
		
		WaitForSingleObject(Read_Mutex,INFINITE);//Add by Fance .
		//int nRet =read_one(g_tstat_id,6,1);这是原有代码，这样发现，真正调用此函数，成功返回率很低
		int nRet =read_one(g_tstat_id,6,2);

		if(nRet<0)	
		{
			ReleaseMutex(Read_Mutex);//Add by Fance .
			continue;
		}
//		int i;
		for(int i=0;i<10;i++) //Modify by Fance , tstat 6 has more register than 512;
		//for(i=0;i<8;i++)
		{
			if(g_tstat_id_changed)
			{
				continue;
			}
			if(g_bPauseMultiRead)
			{
				continue;
			}
			register_critical_section.Lock();
			//
			Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
			register_critical_section.Unlock();
		}

		ReleaseMutex(Read_Mutex);//Add by Fance .

		//Fance_1
		memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
		if(pFrame->m_pViews[0]->m_hWnd!=NULL)
		{

			::PostMessage(pFrame->m_pViews[0]->m_hWnd,MsgT3000ViewFresh,0,0);
		}
		
	}



	return 1;
}


UINT _ScanproductFun(LPVOID pParam)
{
	CMainFrame* pFrame=(CMainFrame*)(pParam);
	CString strMainBuildingName=pFrame->m_strCurMainBuildingName;
	CString strSubScanNet=pFrame->m_strsubNetSelectedScan;

	if(pFrame->m_strsubNetSelectedScan.CompareNoCase(_T("All subNets..."))==0)
	{
		int nNet=pFrame->m_subNetLst.size();
		Building_info buildingInfo;
		for(int i=0;i<nNet;i++)
		{
			buildingInfo=pFrame->m_subNetLst.at(i);	
			
			pFrame->AddScanedDeviceToDatabase(buildingInfo); // 搜索子网中的设备并加到数据库
		}
		pFrame->ScanTstatInDB();
	}
	else
	{
		int nNet=pFrame->m_subNetLst.size();
		Building_info buildingInfo;
		CString strTemp;
		int k=-1;
		for(int i=0;i<nNet;i++)
		{
			strTemp=pFrame->m_subNetLst.at(i).strBuildingName;
			if(strSubScanNet.CompareNoCase(strTemp)==0)
			{
				k=i;
			}
		}
		if(k!=-1)
		{	
			buildingInfo=pFrame->m_subNetLst.at(k);	
			pFrame->AddScanedDeviceToDatabase(buildingInfo);
			pFrame->ScanTstatInDB();
		}
		//scan the selected building:
	}

	pFrame->m_bScanFinished=TRUE;
	return 1;
}

UINT __cdecl  _FreshTreeView(LPVOID pParam );

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
	m_strCurSubBuldingName.Empty();
	m_strCurMainBuildingName.Empty();
	m_nBaudrate=19200;
	m_pThreadScan=NULL;
	m_pFreshMultiRegisters=NULL;
	m_hCurCommunication=NULL;
	m_bScanFinished=FALSE;
	m_nCurSubBuildingIndex=-1;
	m_bScanALL=FALSE;
	m_strIP=_T("");

	//////////////////////////////////////////////////////////////////////////
	m_bEnableRefreshTreeView = TRUE;
	m_pRefreshThread = NULL;

	m_pWaitScanDlg = NULL;
	m_pScanner = NULL;
	FistWdb = TRUE;
	m_isCM5=FALSE;
	FlagSerialNumber = 0;
}


CMainFrame::~CMainFrame()
{
	if (m_pScanner != NULL)
	{
		delete m_pScanner;
		m_pScanner = NULL;
	}

// 	if (m_pFreshMultiRegisters)
// 	{
// 		//delete m_pFreshMultiRegisters;
// 		//m_pFreshMultiRegisters = NULL;
// 	}


}
void CMainFrame::InitViews()
{
	m_nCurView = 0;
    CView* pActiveView = GetActiveView();
	if(pActiveView==NULL)
		return;
	m_pViews[0] = pActiveView;
	m_pViews[1]=(CView*) new CNetworkControllView();
	m_pViews[2]=(CView*) new CGraphicView();
	m_pViews[3]=(CView*) new CTrendLogView();
 	m_pViews[4]=(CView*) new CDialogCM5(); //CM5
 	m_pViews[5]=(CView*) new CDialogT3();  //T3
	m_pViews[6]=(CView*) new CDialgMiniPanel();//Mini Panel 
 	m_pViews[7]=(CView*) new CAirQuality;//AirQuality
	m_pViews[8]=(CView*) new CLightingController;//Lightingcontroller
	m_pViews[9]=(CView*) new CHumChamber;
	m_pViews[10]=(CView*) new CCO2_View;




	CDocument* pCurrentDoc = GetActiveDocument();
    CCreateContext newContext;
    newContext.m_pNewViewClass = NULL;
    newContext.m_pNewDocTemplate = NULL;
    newContext.m_pLastView = NULL;
    newContext.m_pCurrentFrame = NULL;
    newContext.m_pCurrentDoc = pCurrentDoc;

	CRect rect(0, 0, 0, 0);
    
	for (int nView = 1; nView < NUMVIEWS; nView++)
    {
        m_pViews[nView]->Create(NULL, NULL,
				(AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
		                rect, this,
				AFX_IDW_PANE_FIRST + nView, &newContext);
		m_pViews [nView]->OnInitialUpdate();//?
    }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
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
	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	//CMFCToolBar::SetSizes(CSize(48,48),CSize(48,48));
	CMFCToolBar::SetSizes(CSize(34,34),CSize(34,34));
	CMFCToolBar::SetMenuSizes(CSize(30,30),CSize(24,23));

//  	UINT uiToolbarHotID = theApp.m_bHiColorIcons ? IDB_BITMAP48 : 0;
//  	UINT uiToolbarColdID = theApp.m_bHiColorIcons ? IDB_BITMAP48 : 0;
//  	UINT uiMenuID = theApp.m_bHiColorIcons ? IDB_MENUBAROWN_BITMAP : IDB_MENUBAROWN_BITMAP;

	UINT uiToolbarHotID = theApp.m_bHiColorIcons ? IDB_BITMAP_V25049 : 0;
	UINT uiToolbarColdID = theApp.m_bHiColorIcons ? IDB_BITMAP_V25049 : 0;
	UINT uiMenuID = theApp.m_bHiColorIcons ? IDB_MENUBAROWN_BITMAP : IDB_MENUBAROWN_BITMAP;

// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 	 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MYTOOLBAR : IDR_MYTOOLBAR,uiToolbarColdID, uiMenuID, 
// 	 			FALSE /* Not locked */, 0, 0, uiToolbarHotID))
// 	{
// 		TRACE0("Failed to create toolbar\n");
// 		return -1;//fail to create
// 	}
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_TOOLBAR_VER25049 : IDR_TOOLBAR_VER25049,uiToolbarColdID, uiMenuID, 
	 			FALSE /* Not locked */, 0, 0, uiToolbarHotID))
//	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE ) ||
//	 	!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_TOOLBAR_VER25049 : IDR_TOOLBAR_VER25049,uiToolbarColdID, uiMenuID, FALSE /* Not locked */, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;//fail to create
	}


	//m_wndToolBar.SetWindowText (_T("Standard"));
	//m_wndToolBar.SetBorders ();
	//------------------------------------
	// Remove toolbar gripper and borders:
	//------------------------------------
	//m_wndToolBar.SetPaneStyle (m_wndToolBar.GetPaneStyle() &~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));


	///////////////////////////////////////////////////////////////////////////////////

	if (!m_wndWorkSpace.Create (_T("Building View"), this, CRect (0, 0, 300, 800),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN  | CBRS_LEFT ))//| CBRS_FLOAT_MULTI
	{
		TRACE0("Failed to create Workspace bar\n");
		return FALSE;// fail to create
	}


	//////////////////////////////////////////////////////////////////////////////////
	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	/*
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
*/
	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);


	if (!m_wndStatusBar.Create(this)||!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
		
	
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_LEFT);

	
	EnableDocking(CBRS_ALIGN_ANY);	
	
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane (&m_wndWorkSpace);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	//ID_RW_INFO,
	//ID_BUILDING_INFO,
	//ID_PROTOCOL_INFO,
	
	m_wndStatusBar.SetPaneInfo(0,ID_RW_INFO,SBPS_NOBORDERS,   300);
   // int index = m_wndStatusBar.CommandToIndex(ID_BUILDING_INFO);	
	m_wndStatusBar.SetPaneInfo(1,ID_BUILDING_INFO,SBPS_NOBORDERS, 600);//SBPS_POPOUT | SBPS_NOBORDERS,300);	
//	index = m_wndStatusBar.CommandToIndex(ID_RW_INFO);
//	m_wndStatusBar.SetPaneInfo(index,ID_PROTOCOL_INFO,SBPS_NOBORDERS,250);//SBPS_POPOUT | SBPS_NOBORDERS,300);
//	index = m_wndStatusBar.CommandToIndex(ID_PROTOCOL_INFO);
	m_wndStatusBar.SetPaneInfo(2,ID_PROTOCOL_INFO,SBPS_STRETCH | SBPS_NOBORDERS , 300);

	m_wndStatusBar.SetPaneText(1, _T("No Connection"), TRUE);
	m_wndStatusBar.SetPaneTextColor(0, RGB(224,0,0));

	m_wndStatusBar.EnablePaneDoubleClick ();

	//m_wndStatusBar.SetPaneText(1,_T("Helo"));


	m_pTreeViewCrl=&m_wndWorkSpace.m_TreeCtrl;	
	m_pTreeViewCrl->SetExtendedStyle(TVS_EDITLABELS, TVS_EDITLABELS);


	//  2011,7,4, 先判断是否第一次运行，是否要导入数据库。
	//ImportDataBaseForFirstRun();

	ScanTstatInDB();
//20120420	SelectTreeNodeFromRecord();

	//////////////////////////////////////////////////////////////////////////
	// added by zgq;2010-11-30;for update the treectrl;
	//
	//InitTreeNodeConn();   //LSC

	SetTimer(REFRESH_TIMER, REFRESH_TIMER_VALUE, NULL);

	m_pRefreshThread =(CRefreshTreeThread*) AfxBeginThread(RUNTIME_CLASS(CRefreshTreeThread));
	m_pRefreshThread->SetMainWnd(this);	

	// 需要执行线程中的操作时
	m_pFreshMultiRegisters = AfxBeginThread(_ReadMultiRegisters,this);
	m_pFreshTree=AfxBeginThread(_FreshTreeView, this);
	//tstat6
	Tstat6_func();//为TSTST6新寄存器用的。



	//检测Tstat6数据库中的这个表是否存在
	CString temptable = _T("Tstat6");
	//CString tempsql = _T("create table Tstat6(Bolck TEXT,Address number,Data number,Description TEXT)");
	CString tempsql = _T("create table Tstat6(Block TEXT,Address TEXT,Data TEXT,Description TEXT)");
	JudgeTstat6dbExist(temptable,tempsql);
	temptable = _T("Tstat7");
	tempsql = _T("create table Tstat7(Block TEXT,Address TEXT,Data TEXT,Description TEXT)");
	JudgeTstat6dbExist(temptable,tempsql);

	CString tempslider = _T("Sliderdb");
	CString tempsqlslider = _T("create table Sliderdb(Block Number,Address Number)");
	JudgeTstat6SliderExist(tempslider,tempsqlslider);

	//读取Sliderdb的值。
	ReadSlider();




	
//展开所有的项
//	Treestatus();
	HTREEITEM htiRoot = m_pTreeViewCrl->GetRootItem();//GetRootItem();
	m_pTreeViewCrl->Expand(htiRoot,TVE_EXPAND);
	HTREEITEM hChild = m_pTreeViewCrl->GetChildItem(htiRoot);

	//HTREEITEM GetChildItem(_In_ HTREEITEM hItem) const;
	m_pTreeViewCrl->Expand(hChild,TVE_EXPAND);
	HTREEITEM hChilder = m_pTreeViewCrl->GetChildItem(hChild);
	m_pTreeViewCrl->Expand(hChilder,TVE_EXPAND);

	HTREEITEM hChilder1 = m_pTreeViewCrl->GetChildItem(hChilder);
	m_pTreeViewCrl->Expand(hChilder1,TVE_EXPAND);


	hThread =CreateThread(NULL,NULL,Get_All_Dlg_Message,this,NULL, &nThreadID);
	hDeal_thread =CreateThread(NULL,NULL,Translate_My_Message,this,NULL, &nThreadID_Do);


	MyRegAddress.MatchMoudleAddress();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
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

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
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

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{

	UINT uiToolBarId = (UINT) wp;	

	if (IDR_TOOLBAR_VER25049)
	{	
		//int n;
		m_wndToolBar.EnableTextLabels(true);
		//CMFCToolBarMenuButton btnEditor(ID_FILE_SAVE_CONFIG, NULL,GetCmdMgr ()->GetCmdImage (ID_FILE_SAVE_CONFIG), _T("Save"),FALSE);
		//CMFCToolBarMenuButton btnEditor(0, NULL,NULL, _T(""),FALSE);
		//n=GetCmdMgr ()->GetCmdImage (ID_CONFIGFILE_SAVE_AS);
		//m_wndToolBar.ReplaceButton (ID_FILE_OPEN, btnEditor);
		//m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_CONFIGFILE_SAVE_AS),_T("Save"));

		//CMFCToolBarMenuButton btnLoadConfig(ID_LOAD_CONFIG, NULL,GetCmdMgr ()->GetCmdImage (ID_LOAD_CONFIG), _T("Load"),FALSE);
		//CMFCToolBarMenuButton btnLoadConfig(ID_LOAD_CONFIG, NULL,GetCmdMgr ()->GetCmdImage (ID_LOAD_CONFIG), _T(""),FALSE);
		//n=GetCmdMgr ()->GetCmdImage (ID_LOAD_CONFIG);
		//m_wndToolBar.ReplaceButton (ID_LOAD_CONFIG, btnLoadConfig);
		//m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_LOAD_CONFIG),_T("Load"));

		//CMFCToolBarMenuButton btnBurn(ID_FILE_BATCHBURNHEX, NULL,GetCmdMgr ()->GetCmdImage (ID_FILE_BATCHBURNHEX), _T("Burn"),FALSE);
		//CMFCToolBarMenuButton btnBurn(ID_FILE_BATCHBURNHEX, NULL,GetCmdMgr ()->GetCmdImage (ID_FILE_BATCHBURNHEX), _T(""),FALSE);
		//m_wndToolBar.ReplaceButton (ID_FILE_BATCHBURNHEX, btnBurn);
		//n=m_wndToolBar.CommandToIndex (ID_FILE_BATCHBURNHEX);
		//m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_FILE_BATCHBURNHEX),_T("Burn"));
		
		CMFCToolBarMenuButton btnConnect(ID_CONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_CONNECT2), _T("Connect"),FALSE);
		//CMFCToolBarMenuButton btnConnect(ID_CONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_CONNECT2), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_CONNECT2, btnConnect);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_CONNECT2),_T("Connect"));

		CMFCToolBarMenuButton btndiDisConnect(ID_DISCONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_DISCONNECT2), _T("DisConn"),FALSE);
		//CMFCToolBarMenuButton btndiDisConnect(ID_DISCONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_DISCONNECT2), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_DISCONNECT2, btndiDisConnect);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DISCONNECT2),_T("DisConn"));

		CMFCToolBarMenuButton btnScan(ID_SCAN, NULL,GetCmdMgr ()->GetCmdImage(ID_SCAN), _T("Scan"),FALSE);
		//CMFCToolBarMenuButton btnScan(ID_SCAN, NULL,GetCmdMgr ()->GetCmdImage(ID_SCAN), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_SCAN, btnScan);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_SCAN),_T("Scan"));

		CMFCToolBarMenuButton btnConfig(ID_BUILDINGCONFIGDB, NULL, GetCmdMgr ()->GetCmdImage(ID_BUILDINGCONFIGDB), _T("Config"),FALSE);
		//CMFCToolBarMenuButton btnConnect(ID_CONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_CONNECT2), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_BUILDINGCONFIGDB, btnConfig);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_BUILDINGCONFIGDB),_T("Config"));
	}
	return 0;
}

void CMainFrame::OnCheckBuildingBar()
{
	ShowPane (&m_wndWorkSpace, (m_wndWorkSpace.GetStyle () & WS_VISIBLE) == 0, FALSE, TRUE);

}
void CMainFrame::OnUpdateCheckBuildingBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndWorkSpace.IsWindowVisible ());
}
void CMainFrame::OnCheckIOPane()
{
}
void CMainFrame::OnUpdateCheckIOPane(CCmdUI* pCmdUI)
{

}

void CMainFrame::OnHTreeItemSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{	

	Flexflash = TRUE;
	HTREEITEM hSelItem;//=m_pTreeViewCrl->GetSelectedItem();
int nRet =read_one(g_tstat_id,6,1);

	CPoint pt;
	GetCursorPos(&pt);
	m_pTreeViewCrl->ScreenToClient(&pt);
	hSelItem = m_pTreeViewCrl->HitTest(pt);

	//////////////////////////////////////////////////////////////////////////
	m_strCurSelNodeName = m_pTreeViewCrl->GetItemText(hSelItem);
	//m_pTreeViewCrl->Expand(hSelItem,TVE_EXPAND);


//	BOOL ret  =  m_pTreeViewCrl->Retofline(hSelItem);//tree0412
//	if (ret)//tree0412
//	{

	BeginWaitCursor();
	//CM5
#if 1
	for(UINT i=0;i<m_product.size();i++)
	{
		if(hSelItem==m_product.at(i).product_item )
		{			
			if(m_product.at(i).product_class_id == PM_CM5)
			{
				g_tstat_id=m_product.at(i).product_id;
				//SetPaneString(2,_T("Connect To CM5"));
				m_isCM5=TRUE;
				DoConnectToANode(hSelItem); 
				//SwitchToPruductType(4);			
			}else if (m_product.at(i).product_class_id == PM_MINIPANEL)
			{
				g_tstat_id = m_product.at(i).product_id;
				SwitchToPruductType(6);
			}else if (m_product.at(i).product_class_id == T3_4AO_PRODUCT_MODEL) //T3
			{
				g_tstat_id = m_product.at(i).product_id;
				SwitchToPruductType(5);
			}
			else if (m_product.at(i).product_class_id ==PM_AirQuality) //AirQuality
			{
				g_tstat_id = m_product.at(i).product_id;
				SwitchToPruductType(7);
			}else if (m_product.at(i).product_class_id == PM_LightingController)//LightingController
			{
				g_tstat_id = m_product.at(i).product_id;
				if(m_product.at(i).BuildingInfo.hCommunication==NULL||m_strCurSubBuldingName.CompareNoCase(m_product.at(i).BuildingInfo.strBuildingName)!=0)
				{
					//connect:
					BOOL bRet = ConnectSubBuilding(m_product.at(i).BuildingInfo);
					if (!bRet)
					{
						if(m_product.at(i).BuildingInfo.strProtocol.CompareNoCase(_T("Modbus TCP")) == 0) // net work protocol
						{
							CheckConnectFailure(m_product.at(i).BuildingInfo.strIp);
						}
					}
				}
				SwitchToPruductType(8);
			}
			else 
			{
				DoConnectToANode(hSelItem); 
			}
				
		}
	}
#endif
	//CM5

	EndWaitCursor();
////////////////////////////////////////////////////////////////////////////
// commented by zgq; 2010-11-29;这个是为了
/*
	int nCounts=m_product.size();
	tree_product product_Node;
	int nSelectID=-1;
	UINT nSelectSerialNumber;
	for(int i=0;i<m_product.size();i++)
	{
		if(hSelItem==m_product.at(i).product_item )
		{
			int nID=-1;
			int nTRet=-1;
			//g_tstat_id=m_product.at(i).product_id;
			product_Node=m_product.at(i);

			//************************************
			register_critical_section.Lock();
			g_tstat_id_changed=TRUE;
			g_tstat_id=product_Node.product_id;
			register_critical_section.Unlock();
			//***************************************
			nSelectID=product_Node.product_id;
			nSelectSerialNumber=product_Node.serial_number;


			CString strTitle;
			strTitle=product_Node.BuildingInfo.strMainBuildingname;
			strTitle+=_T("->");
			strTitle+=product_Node.BuildingInfo.strBuildingName;

			m_wndWorkSpace.SetWindowText(strTitle);

			//AfxMessageBox(product_Node.BuildingInfo.strMainBuildingname);
		
			g_strImagePathName=product_Node.strImgPathName;
			if(product_Node.BuildingInfo.hCommunication==NULL||m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0)
			{
				//connect:
				ConnectSubBuilding(product_Node.BuildingInfo);
			}
			
			
// 			if(m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0&&product_Node.BuildingInfo.hCommunication!=NULL)
// 			{
// 				CloseHandle(product_Node.BuildingInfo.hCommunication);
// 				m_product.at(i).BuildingInfo.hCommunication=NULL;
// 			}
			if(m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0&&m_hCurCommunication!=NULL)
			{
				//CloseHandle(m_hCurCommunication);
			}

			m_strCurSubBuldingName=product_Node.BuildingInfo.strBuildingName;
			BOOL bOnLine=FALSE;
			UINT nSerialNumber=0;
			if (g_CommunicationType==0)
			{
				m_nbaudrat=19200;
				Change_BaudRate(19200);
				nID=read_one(g_tstat_id,6,2);
				if(nID<0)
				{
					m_nbaudrat=9600;
					Change_BaudRate(9600);
					nID=read_one(g_tstat_id,6,2);
					bOnLine=FALSE;
				}
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);
					
					if(nRet>=0)
					{
			
						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
			

				}
			}
			if (g_CommunicationType==1)
			{
			  nID=read_one(g_tstat_id,6);
			  if(nID<0)
				  bOnLine=FALSE;
			  if(nID>0)
			  {
				  	unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);
					UINT nSerialNumber=0;
					if(nRet>0)
					{
			
						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
			  }

			}
			if(bOnLine)
			{
				SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,true);
			
			}
			else
			{
				
				SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,false);	
				memset(&multi_register_value[0],0,sizeof(multi_register_value));
				return;
				//CString strtemp;
				//strtemp.Format(_T("Failed to connect the Tstate Serial number=%d,Address ID=%d"),m_product.at(i).serial_number,m_product.at(i).product_id);
				//AfxMessageBox(strtemp);
				//return;
			}
			register_critical_section.Lock();
			multi_read_tstat(g_tstat_id);
			g_tstat_id_changed=FALSE;
			register_critical_section.Unlock();
			g_serialNum=nSerialNumber;//get_serialnumber();

			if(g_bPrivilegeMannage)
			{
				if(g_buser_log_in)
				{
					if(g_strLoginUserName.CompareNoCase(_T("admin"))==0)
					{
						g_MainScreenLevel=0;
						g_ParamLevel=0;
						g_OutPutLevel=0;
						g_NetWorkLevel=0;
						g_GraphicModelevel=0;
						g_BurnHexLevel=0;
						g_LoadConfigLevel=0;
						g_BuildingsetLevel=0;
						g_AllscreensetLevel=0;
					}

					else
				{
					_ConnectionPtr m_pConTmp;
					_RecordsetPtr m_pRsTemp;
					m_pConTmp.CreateInstance("ADODB.Connection");
					m_pRsTemp.CreateInstance("ADODB.Recordset");
					m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

					CString strSerial;
					strSerial.Format(_T("%d"),g_serialNum);
					strSerial.Trim();
					CString strsql;
					strsql.Format(_T("select * from user_level where MainBuilding_Name='%s' and Building_Name='%s' and username = '%s' and serial_number = %d"),m_strCurMainBuildingName,m_strCurSubBuldingName,g_strLoginUserName,g_serialNum);
					m_pRsTemp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	

					if(VARIANT_FALSE==m_pRsTemp->EndOfFile)
					{//获取权限：
						g_AllscreensetLevel=m_pRsTemp->GetCollect("allscreen_level");
						if(g_AllscreensetLevel!=1)
						{
							g_MainScreenLevel=m_pRsTemp->GetCollect("mainscreen_level");//
							g_ParamLevel=m_pRsTemp->GetCollect("parameter_level");
							g_OutPutLevel=m_pRsTemp->GetCollect("outputtable_level");
							g_NetWorkLevel=m_pRsTemp->GetCollect("networkcontrol_level");//
							g_GraphicModelevel=m_pRsTemp->GetCollect("graphic_level");//
							g_BurnHexLevel=m_pRsTemp->GetCollect("burnhex_level");
							g_LoadConfigLevel=m_pRsTemp->GetCollect("loadconfig_level");
							g_BuildingsetLevel=m_pRsTemp->GetCollect("building_level");
						}
						else
						{
							g_MainScreenLevel=1;
							g_ParamLevel=1;
							g_OutPutLevel=1;
							g_NetWorkLevel=1;
							g_GraphicModelevel=1;
							g_BurnHexLevel=1;
							g_LoadConfigLevel=1;
							g_BuildingsetLevel=1;
							g_AllscreensetLevel=1;

						}



					}
					if(m_pRsTemp->State) 
						m_pRsTemp->Close(); 
					if(m_pConTmp->State)
						m_pConTmp->Close();	


					m_pConTmp.CreateInstance("ADODB.Connection");
					m_pRsTemp.CreateInstance("ADODB.Recordset");
					m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

					strSerial.Format(_T("%d"),g_serialNum);
					strSerial.Trim();

					strsql.Format(_T("select * from UserLevelSingleSet where MainBuilding_Name='%s' and Building_Name='%s' and username = '%s'"),m_strCurMainBuildingName,m_strCurSubBuldingName,g_strLoginUserName);
					m_pRsTemp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
					if(VARIANT_FALSE==m_pRsTemp->EndOfFile)
					{//获取权限：
						g_NetWorkLevel=m_pRsTemp->GetCollect("networkcontroller");//
						g_BuildingsetLevel=m_pRsTemp->GetCollect("database_limition");
					}

					if(m_pRsTemp->State) 
						m_pRsTemp->Close(); 
					if(m_pConTmp->State)
						m_pConTmp->Close();	


				}

				}
	
			}
			GetIONanme();

			if(multi_register_value[7]==NET_WORK_CONT_PRODUCT_MODEL)
			{
				SwitchToPruductType(1);
			}
			if(multi_register_value[7]<NET_WORK_CONT_PRODUCT_MODEL)
			{
				SwitchToPruductType(0);
			}
		}
	}
	*/
//	}else
//		AfxMessageBox(_T("Sorry, this node is offline"));//tree0412

}




void CMainFrame::OnHTreeItemEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
#if 0	
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	m_pTreeViewCrl->SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);

	// 更新数据库
	UpdateAllNodesInfo(pTVDispInfo->item.hItem);
#endif

}

LRESULT CMainFrame::OnHTreeItemBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
#if 0
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	int nItemData = m_pTreeViewCrl->GetItemData(pTVDispInfo->item.hItem);
	// TODO: Add your control notification handler code here
	if (nItemData >= 9000)
	{
		//AfxMessageBox(_T("Can not edit Sub net Name."));
		*pResult = 1;
		return 1;
	}

	*pResult = 0;
#endif
	return 0;
}

void  CMainFrame::OnHTreeItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnHTreeItemSeletedChanged(pNMHDR, pResult);
}



BOOL CMainFrame::ValidAddress(CString sAddress)
{
	int nPos;
	UINT n1,n2,n3,n4;
	CString sTemp=sAddress;
	n1=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n2=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n3=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n4=_wtoi(sTemp);
	if(n1<0 ||n1>255) return false;
	if(n2<0 ||n2>255) return false;
	if(n3<0 ||n3>255) return false;
	if(n4<0 ||n4>255) return false;
	return TRUE;
}
BOOL CMainFrame::ValidAddress(CString sAddress,UINT& n1,UINT& n2,UINT& n3,UINT& n4)
{
	int nPos;
	CString sTemp=sAddress;
	n1=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n2=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n3=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n4=_wtoi(sTemp);
	if(n1<0 ||n1>255) return false;
	if(n2<0 ||n2>255) return false;
	if(n3<0 ||n3>255) return false;
	if(n4<0 ||n4>255) return false;
	return true;
}

// scan Tstats in database
void CMainFrame::ScanTstatInDB(void)
{

try
{

	TRACE(_T("Scan TStat In DB!!  \n"));
#if 1
	ClearBuilding();
	m_product.clear();
	m_pCon.CreateInstance("ADODB.Connection");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs.CreateInstance("ADODB.Recordset");


	_variant_t temp_variant;
	CString strTemp;

	CString temp_str=_T("select * from Building where Default_SubBuilding=-1");
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	if(m_pRs->GetRecordCount()<=0)
	{
		if(m_pRs->State)
		m_pRs->Close(); 
		if(m_pCon->State)
		m_pCon->Close();
	//	PostMessage(ID_NODEFAULTMAINBUILDING_MSG,0,0);
		AfxMessageBox(_T("There is no default building, please select a building First！"));

		return;
	}
	
	m_strCurMainBuildingName=m_pRs->GetCollect("Main_BuildingName");
	if(m_pRs->State)
		m_pRs->Close();
	m_subNetLst.clear();
	temp_str.Format(_T("select * from Building where Main_BuildingName ='%s'"),m_strCurMainBuildingName);
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	


	m_nCurSubBuildingIndex=-1;
	int nTemp=-1;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{
		nTemp++;
		int nDefault=0;
		Building_info temBuildingInfo;
		//memset(&temBuildingInfo,0,sizeof(temBuildingInfo));
		temBuildingInfo.strBuildingName=m_pRs->GetCollect("Building_Name");
		temBuildingInfo.strProtocol=m_pRs->GetCollect("Protocal");
		temBuildingInfo.strIp=m_pRs->GetCollect("Ip_Address");
		temBuildingInfo.strComPort=m_pRs->GetCollect("Com_Port");
		temBuildingInfo.strIpPort=m_pRs->GetCollect("Ip_Port");
		temBuildingInfo.hCommunication=NULL;
		temBuildingInfo.strMainBuildingname=m_pRs->GetCollect("Main_BuildingName");
		temBuildingInfo.strBaudRate=m_pRs->GetCollect("Braudrate");

		nDefault=m_pRs->GetCollect("Default_SubBuilding");
		if(nDefault==-1)
		{
			m_nCurSubBuildingIndex=nTemp;
		}

		if(temBuildingInfo.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
		{
			/*
			if(ValidAddress(temBuildingInfo.strIp))
			{
				AfxMessageBox(_T("please check the ip address"));
			}
			//Open_Socket2(temBuildingInfo.strIp,_wtoi(temBuildingInfo.strIp));
			temBuildingInfo.hCommunication=(HANDLE)m_hSocket;
			*/
		}
		m_pRs->MoveNext();
		m_subNetLst.push_back(temBuildingInfo);
	}
	if(m_subNetLst.size()<=0)
		AfxMessageBox(_T("There is no default building,please select a building First！"));
	
	if(m_pRs->State)
		m_pRs->Close(); 


	TV_INSERTSTRUCT tvInsert;////added
	
	m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;
//	for(int k=0;k<m_subNetLst.size();k++)
	{
		CString strBuilding=m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;

		tvInsert.hParent = TVI_ROOT; // 指定父句柄
		tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
		tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strBuilding;		
		tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式	
		TVINSERV_BUILDING
		HTREEITEM hTreeSubbuilding=NULL;
		//hTreeSubbuilding=m_pTreeViewCrl->InsertItem(&tvInsert);//插入subbuilding。
		hTreeSubbuilding=m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入subbuilding。
		if (hTreeSubbuilding!=NULL)
		{
		//	m_subNetLst.at(k).hbuilding_item=hTreeSubbuilding;
		}
	//	m_pRs->Close();
		////begin floor nodes///////////////////////////////////////////////////////////////
		temp_str.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '"+strBuilding+"'"));
		HRESULT hR=m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
		
		int nTemp2 = m_pRs->RecordCount;

		vector <tree_floor> tmpfloorLst;//
		tmpfloorLst.empty();
		while(VARIANT_FALSE==m_pRs->EndOfFile)//所有楼层。 
		{
			CString strFloorName=m_pRs->GetCollect("Floor_name");
			///*********tree***********************************
			tvInsert.hParent = hTreeSubbuilding; // 指定父句柄
			tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
		
			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strFloorName;		
			tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式	
			TVINSERV_FLOOR
			
			HTREEITEM hTreeFloor=NULL;
			//hTreeFloor=m_pTreeViewCrl->InsertItem(&tvInsert);//返回楼层的句柄
			hTreeFloor=m_pTreeViewCrl->InsertFloorItem(&tvInsert);//返回楼层的句柄
			
			tree_floor m_floor_temp;
			m_floor_temp.building_item=hTreeSubbuilding;
			m_floor_temp.floor_item =hTreeFloor;
			m_floor_temp.strFloorName=strFloorName;
			m_floorLst.push_back(m_floor_temp);
			tmpfloorLst.push_back(m_floor_temp);
			m_pRs->MoveNext();
		}

	
	 if(m_pRs->State)
		m_pRs->Close(); 

		vector <tree_room> tmproomLst;
		////end floor nodes/////////////////////////////////////////////////////////////////
		///begin room nodes///////////////////////////////////////////////////////////////////////
		for(unsigned int i=0;i<tmpfloorLst.size();i++)
		{	//loop for Room Name f
		//	m_pRs->Close();
			CString strFloorName;
			strFloorName=tmpfloorLst.at(i).strFloorName;
			CString temp_str;
			temp_str.Format(_T("select DISTINCT Room_name from ALL_NODE where Building_Name = '"+strBuilding+"' and Floor_name='"+strFloorName+"'"));
			m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
			//插入每个房间到相关的楼层。
			while(VARIANT_FALSE==m_pRs->EndOfFile)
			{
				CString strRoomName;
				strRoomName=m_pRs->GetCollect("Room_name");
				tvInsert.hParent = tmpfloorLst.at(i).floor_item ; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strRoomName;		
				tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式	
				TVINSERV_ROOM
				HTREEITEM hTreeRoom=NULL;
				//hTreeRoom=m_pTreeViewCrl->InsertItem(&tvInsert);
				hTreeRoom=m_pTreeViewCrl->InsertRoomItem(&tvInsert);

				
				tree_room m_room_temp;
				
				m_room_temp.building_item=m_floorLst.at(i).building_item ;									
				m_room_temp.floor_item =m_floorLst.at(i).floor_item ;
				m_room_temp.room_item =hTreeRoom;
				m_room_temp.strFloorName=strFloorName;
				m_room_temp.strRoomName=strRoomName;
				m_roomLst.push_back(m_room_temp);	
				tmproomLst.push_back(m_room_temp);	
				m_pRs->MoveNext();
			}		
			 if(m_pRs->State)
			m_pRs->Close(); 
		}
	///////end room nodes//////////////////////////////////////////////////////////////////////
	//////Begin product node/////////////////////////////////////////////////////////////////
	///每个房间；
		for(UINT i=0;i<tmproomLst.size();i++)
		{//loop for product Name
			//m_pRs->Close();	
			CString str_temp;
			CString StrFloorName;
			CString strRoomName;
			strRoomName=tmproomLst.at(i).strRoomName;
			StrFloorName=tmproomLst.at(i).strFloorName;
			str_temp.Format(_T("select * from ALL_NODE where Floor_name = '"+StrFloorName+"' and\
			Room_name = '"+strRoomName+"'and Building_Name = '"+strBuilding+"'  ORDER BY Product_ID ASC "));
			m_pRs->Open(_variant_t(str_temp),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
			//插入每个产品到相关的房间。
			while(VARIANT_FALSE==m_pRs->EndOfFile)
			{
				CString strProdcut=m_pRs->GetCollect("Product_name");
				tvInsert.hParent = tmproomLst.at(i).room_item ; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText =(LPTSTR)(LPCTSTR) strProdcut;	
				TRACE(strProdcut);
				tvInsert.hInsertAfter =TVI_SORT;// TVI_LAST; // 项目插入方式						
				int temp_product_class_id=m_pRs->GetCollect("Product_class_ID");

#if 1			
				if(temp_product_class_id==PM_NC) 
					TVINSERV_NET_WORK
				else if(temp_product_class_id==LED_PRODUCT_MODEL)
					TVINSERV_LED
				else if(temp_product_class_id==PM_SOLAR)
					TVINSERV_SOLAR	
				else if (temp_product_class_id == PM_CM5 ) //CM5		
					TVINSERV_CMFIVE	
				else if (temp_product_class_id == PM_T38AIOD) //T3
					TVINSERV_NET_WORK
				else if (temp_product_class_id ==PM_T3IOA )//T3-8AI8AO
					TVINSERV_NET_WORK
				else if (temp_product_class_id ==PM_MINIPANEL )//Mini Panel
					TVINSERV_MINIPANEL
				else if (temp_product_class_id == PM_AirQuality) //AirQuality
					TVINSERV_NET_WORK
				else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
					//TVINSERV_NET_WORK  //tree0412
					TVINSERV_LC          //tree0412
				else if (temp_product_class_id == PM_TSTAT7)//TSTAT7 &TSTAT6 //tree0412
					TVINSERV_LED //tree0412
				else if(temp_product_class_id == PM_TSTAT6)
					TVINSERV_TSTAT6
				else if(temp_product_class_id == PM_CO2)
					TVINSERV_CO2
				else
				
					TVINSERV_TSTAT
#endif



			//	pBuildingInfo->hbuilding_item=							

				//HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertItem(&tvInsert);
				HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertDeviceItem(&tvInsert);
			
				tree_product m_product_temp;
				m_product_temp.product_item  =hTreeRoom;
					
				//m_product_temp.serial_number = m_pRs->GetCollect("Serial_ID");
				temp_variant=m_pRs->GetCollect("Serial_ID");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.serial_number=_wtol(strTemp);

				//m_product_temp.product_id =m_pRs->GetCollect("Product_ID");
				temp_variant=m_pRs->GetCollect("Product_ID");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.product_id=_wtoi(strTemp);

			//	m_product_temp.product_class_id = m_pRs->GetCollect("Product_class_ID");
				temp_variant=m_pRs->GetCollect("Product_class_ID");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.product_class_id=_wtoi(strTemp);

			//	m_product_temp.hardware_version= m_pRs->GetCollect("Hardware_Ver");
				temp_variant=m_pRs->GetCollect("Hardware_Ver");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.hardware_version=(float)_wtof(strTemp);
				
				//
				temp_variant=m_pRs->GetCollect("Software_Ver");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.software_version=(float)_wtof(strTemp);

				// 
				temp_variant=m_pRs->GetCollect("EPsize");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.nEPsize=(int)_wtol(strTemp);

				//m_product_temp.baudrate=m_pRs->GetCollect("Bautrate");
			
				temp_variant=m_pRs->GetCollect("Bautrate");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.baudrate=_wtoi(strTemp);
				if (m_product_temp.product_class_id == PM_NC)
				{m_product_temp.baudrate = 19200;
				}
				////

				//m_product_temp.BuildingInfo.strMainBuildingname=m_pRs->GetCollect("MainBuilding_Name");
				//AfxMessageBox(m_product_temp.BuildingInfo.strMainBuildingname);
				//m_product_temp.BuildingInfo.strBuildingName=m_pRs->GetCollect("Building_Name");
				m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
//20120423	
				m_product_temp.BuildingInfo.strIp = strTemp;
				temp_variant=m_pRs->GetCollect("Com_Port");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.BuildingInfo.strIpPort=strTemp;
			
//20120423

				//AfxMessageBox(m_product_temp.BuildingInfo.strMainBuildingname);
				//m_product_temp.strSubNetBuildingName=strBuilding;


				/*
				m_product.push_back(m_product_temp);
				background_infor m_background_temp;
				m_background_temp.building_item=hTreeRoom;
				m_background_temp.Screen_name =m_pRs->GetCollect("Screen_Name");
			//	m_background_temp.Background_Bmp =m_pRs->GetCollect("Background_imgID");
				temp_variant=m_pRs->GetCollect("Background_imgID");
				if(temp_variant.vt!=VT_NULL)
					m_background_temp.Background_Bmp=temp_variant;
				else
			    	m_background_temp.Background_Bmp=_T("");
				m_background_temp.building_name=strBuilding;
				m_backgroundLst.push_back(m_background_temp);
				*/

				temp_variant=m_pRs->GetCollect("Background_imgID");
				if(temp_variant.vt!=VT_NULL)
					m_product_temp.strImgPathName=temp_variant;
				else
			    	m_product_temp.strImgPathName=_T("");
				m_product.push_back(m_product_temp);
				m_pRs->MoveNext();
			}

			if(m_pRs->State)
			m_pRs->Close(); 

		}
	}
		if(m_pRs->State)
		m_pRs->Close(); 
		if(m_pCon->State)
		m_pCon->Close();
#endif





	if(m_nCurSubBuildingIndex>=0)
	{
		m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;
	//	ConnectSubBuilding(m_subNetLst.at(m_nCurSubBuildingIndex));
	}

	}
	catch (...)
	{

	}
}

void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("Open configuration file."));
}
void CMainFrame::OnLoadConfigFile()
{
	//AfxMessageBox(_T("Load configuration file."));
	if(g_LoadConfigLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE2);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
		return;
	}

	int nCurID=g_tstat_id;
	g_bPauseMultiRead=TRUE;
	CGridLoad Dlg;
	Dlg.DoModal();
	g_tstat_id=nCurID;
	ReFresh();
	g_bPauseMultiRead=FALSE;
}
void CMainFrame::OnBatchFlashHex()
{
	g_bEnableRefreshTreeView = FALSE;
	if(g_BurnHexLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE3);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
		return;
	}

	g_bPauseMultiRead=TRUE;
	CGridFlashDlg Dlg;
	Dlg.DoModal();
	g_bPauseMultiRead=FALSE;
	g_bEnableRefreshTreeView = TRUE;
}

void CMainFrame::OnConnect()
{
	//	return;
	if(m_strCurMainBuildingName.IsEmpty())
		return;
	if(m_nCurSubBuildingIndex==-1)
		return;
	if(m_subNetLst.size()<=0)
		return;

	Building_info build_info;
	//for(int i=0;i<m_subNetLst.size();i++)
	{
		build_info=m_subNetLst.at(m_nCurSubBuildingIndex);
		if (build_info.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
		{
			UINT n1,n2,n3,n4;
			if (ValidAddress(build_info.strIp,n1,n2,n3,n4)==FALSE) 
			{
				//AfxMessageBox(_T("Invalidate IP Address!!"));
				//goto here;
			}
			else
			{
				CString strPort;
				m_nIpPort=_wtoi(build_info.strIpPort);
				m_strIP=build_info.strIp;
				g_CommunicationType=1;
				SetCommunicationType(g_CommunicationType);
				if(Open_Socket2(build_info.strIp,m_nIpPort))
				{
					CString strInfo;
					strInfo.Format((_T("Open IP:%s successful.")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
				}
				else
				{
					CString strInfo;
					strInfo.Format((_T("Open IP:%s failed.")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
				}

			}
		}

		if (build_info.strProtocol.CompareNoCase(_T("Modbus 485"))==0)
		{
			//rs485
			g_CommunicationType=0;
			SetCommunicationType(g_CommunicationType);		
			CString strInfo;

			if (build_info.strComPort.GetLength()!=0)
			{				
				int nComPort = _wtoi(build_info.strComPort.Mid(3));
				open_com(nComPort);//open*************************************
				CString strInfo;
				if(!is_connect())
				{
					//strInfo=_T("Open ")+build_info.strComPort+_T(" Failure!");
					//strInfo.Format(_T("COM: %d Connected: No"), nComPort);		
					strInfo.Format(_T("COM %d : Not available "), nComPort);
					//SetPaneCommunicationPrompt(strInfo);
					SetPaneString(1, strInfo);
				}
				else
				{
					//strInfo=_T("Open ")+build_info.strComPort+_T(" Sucessful!");
					strInfo.Format(_T("COM %d Connected: Yes"), nComPort);			
					//SetPaneCommunicationPrompt(strInfo);
					SetPaneString(1, strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			/*
			if (build_info.strComPort.CompareNoCase(_T("COM1"))==0)
			{
				open_com(1);//open*************************************
				CString strInfo;
				if(!is_connect())
				{
					strInfo=_T("Open COM1 Failure!");
					SetPaneCommunicationPrompt(strInfo);
				}
				else
				{
					strInfo=_T("Connect COM1 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM2"))==0)
			{
				open_com(2);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM2 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				//	com_port_num=10;
				}
				else
				{
					strInfo=_T("Connect COM2 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM3"))==0)
			{
				open_com(3);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM3 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				//	com_port_num=10;
				}
				else
				{
					strInfo=_T("Connect COM3 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM4"))==0)
			{
				open_com(4);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM4 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				//	com_port_num=10;
				}
				else
				{
					strInfo=_T("Connect COM4 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM5"))==0)
			{
				open_com(5);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM5 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				}
				else
				{
					strInfo=_T("Connect COM5 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
					AfxMessageBox(strInfo);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM6"))==0)
			{
				open_com(6);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM6 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				}
				else
				{
					strInfo=_T("Connect COM6 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM7"))==0)
			{
				open_com(7);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM7 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				//	com_port_num=10;
				}
				else
				{
					strInfo=_T("Connect COM7 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM8"))==0)
			{
				open_com(8);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM8 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				//	com_port_num=10;
				}
				else
				{
					strInfo=_T("Connect COM8 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}
			if (build_info.strComPort.CompareNoCase(_T("COM9"))==0)
			{
				open_com(9);//open ******************************************
				if(!is_connect())
				{
					strInfo=_T("Open COM9 Failure!");
					SetPaneCommunicationPrompt(strInfo);
					AfxMessageBox(strInfo);
				}
				else
				{
					strInfo=_T("Connect COM9 successful.");
					SetPaneCommunicationPrompt(strInfo);
					Change_BaudRate(default_com1_port_baudrate);
				}	
			}

			*/


		}//endof rs485 checking
	}


	//EnableMyToolBarButton(0, ID_CONNECT2, FALSE);
	//EnableMyToolBarButton(1, ID_DISCONNECT2, TRUE);
}
void CMainFrame::OnDisconnect()
{
	//AfxMessageBox(_T("DisConnect."));
	close_com();
	
	CString strInfo = _T("No Connnection");
	SetPaneString(1,strInfo);
	strInfo = _T("Offline!");
	SetPaneString(2,strInfo);


	//EnableMyToolBarButton(1, ID_DISCONNECT2, FALSE);
	//EnableMyToolBarButton(0, ID_CONNECT2, TRUE);

}


BOOL CMainFrame::ConnectSubBuilding(Building_info build_info)
{
	CString strInfo;
	if(m_hCurCommunication!=NULL)	
	{
		CloseHandle(m_hCurCommunication);
		m_hCurCommunication=NULL;
	}
	if (build_info.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{
		UINT n1,n2,n3,n4;
		if (ValidAddress(build_info.strIp,n1,n2,n3,n4)==FALSE)  // 验证NC的IP
		{
				/*
				strInfo=_T("Invalidate IP Address!!");
				AfxMessageBox(strInfo);
				SetPaneString(1,strInfo);
				strInfo=_T("disconnected");
				SetPaneString(1,strInfo);
				return FALSE;
				*/

				CString StringIP;
				if(!GetIPbyHostName(build_info.strIp,StringIP))
				{
					AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
					return false;
				}
				CString strPort;
				m_nIpPort=_wtoi(build_info.strIpPort);
				g_CommunicationType=1;
				SetCommunicationType(g_CommunicationType);
				bool b=Open_Socket2(StringIP,m_nIpPort);
				CString strInfo;
				if(b)
				{	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
					return TRUE;
				}
				else
				{
					strInfo.Format((_T("Open IP:%s failure")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
					return FALSE;
				}
		}
		else
		{
			CString strPort;
			m_nIpPort=_wtoi(build_info.strIpPort);

			m_strIP=build_info.strIp;

			g_CommunicationType=1;
			SetCommunicationType(g_CommunicationType);
			bool b=Open_Socket2(build_info.strIp,m_nIpPort);
			CString strInfo;
		//	strInfo.Format((_T("Open IP:%s successful")),build_info.str3Ip);//prompt info;
		//	SetPaneString(1,strInfo);
			if(b)
			{	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
				SetPaneString(1,strInfo);
				return TRUE;
			}
			else
			{
				strInfo.Format((_T("Open IP:%s failure")),build_info.strIp);//prompt info;
				SetPaneString(1,strInfo);
				//ValidOpenFailure(build_info.strIp,n1, n2,n3,n4); // 检查失败的原因，并给出详细的提示信息
				return FALSE;
			}
		}
	}	


	g_CommunicationType = 0;
	SetCommunicationType(g_CommunicationType);
	CString strComport = build_info.strComPort;
	CString strComNum = strComport.Mid(3);
	int nCom = _wtoi(strComNum);

	open_com(nCom);//open*************************************
	if(!is_connect())
	{

		//strInfo.Format(_T("COM: %d Connected: No"), nCom);
		strInfo.Format(_T("COM %d : Not available "), nCom);
		SetPaneString(1,strInfo);
		AfxMessageBox(strInfo);
		return FALSE;
	}
	else
	{
		strInfo.Format(_T("COM %d Connected: Yes"), nCom);
		SetPaneString(1,strInfo);
		Change_BaudRate(default_com1_port_baudrate);
		return TRUE;
	}	
	return FALSE;
}

void CMainFrame::CheckConnectFailure(const CString& strIP) // 检查失败的原因，并给出详细的提示信息
{
	USES_CONVERSION;
	LPCSTR szIP = W2A(strIP);
	DWORD dwIP = inet_addr(szIP);

	IN_ADDR ia;
	ia.S_un.S_addr = dwIP;

	//////////////////////////////////////////////////////////////////////////
	char hostname[256];
	int res = gethostname(hostname, sizeof(hostname));
	if(res != 0)
		return;

	hostent* pHostent = gethostbyname(hostname); 
	if(pHostent==NULL) 
		return;

	hostent& he = *pHostent;
	sockaddr_in sa; 

	memcpy(&sa.sin_addr.s_addr, he.h_addr_list[0],he.h_length); 

	//return   inet_ntoa(sa.sin_addr); 
	CString strHostIP;
	strHostIP.Format(_T("%d.%d.%d.%d"), sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);
	//AfxMessageBox(strIP);

	// 是否是同一子网
	if ( ia.S_un.S_un_b.s_b1 == sa.sin_addr.S_un.S_un_b.s_b1 &&
		ia.S_un.S_un_b.s_b2 == sa.sin_addr.S_un.S_un_b.s_b2 &&
		ia.S_un.S_un_b.s_b3 == sa.sin_addr.S_un.S_un_b.s_b3 
		)
	{// 是同一子网，但是连接不上，那么提示检查设备连接
		CString strTip;
		strTip.Format(_T("Can not set up the connection with %s, please check its IP address and net cable. "), strIP);
		AfxMessageBox(strTip);
	}
	else
	{
		CString strTip;
		strTip.Format(_T("Your host IP is %s, and NC' IP is %s. They are not in same sub net, please reset your IP address. "),strHostIP, strIP);
		AfxMessageBox(strTip);
	}
	

}



void CMainFrame::OnAddBuildingConfig()
{
	if(g_BuildingsetLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE_BUILDING);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
		return;
	}
	/*
	CMannageBuidingDlg Dlg;
	if(Dlg.DoModal()==IDOK)
	{
	//	CAddBuilding Dlg;
	//	Dlg.DoModal();
	}
	*/
	CAddBuilding Dlg;
	Dlg.DoModal();
//	Treestatus();//展开树列表
}

void CMainFrame::OnScanDevice()
{

	if(m_subNetLst.size()<=0)
	{
		AfxMessageBox(_T("Please Select a main building first."));
		return;
	}
	m_bScanFinished=FALSE;
	m_bScanALL=FALSE;
	g_bPauseMultiRead=TRUE;

	//////////////////////////////////////////////////////////////////////////
	//// oldscan
// 	CScanSelectDlg dlg;
// 	if(dlg.DoModal()==IDOK)
// 	{
// 		this->m_strsubNetSelectedScan=dlg.m_strScanSubNetName;
// 	}
// 	else
// 		return;
// 	if(m_strsubNetSelectedScan.IsEmpty())
// 		return;

	/*
	if(m_hCurCommunication!=NULL)	
	{
		CloseHandle(m_hCurCommunication);
		m_hCurCommunication=NULL;

	}
	close_com();
	*/


	Scan_Product();
	
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(REFRESH_TIMER==nIDEvent)
	{	
		if (g_bEnableRefreshTreeView && m_bEnableRefreshTreeView && !g_bPauseMultiRead )
		{	
// 			g_bEnableRefreshTreeView = FALSE;
// 			

// 			//AfxBeginThread(_FreshTreeView, this);
// 			//RefreshTreeView();
// 			((CT3000View*)m_pViews[0])->Fresh();
// 
// 			g_bEnableRefreshTreeView = TRUE;
			////if ((m_pRefreshThread)&&(g_bEnableRefreshTreeView==TRUE))	//Modify by Fance
			////{
			////	m_pRefreshThread->PostThreadMessage(WM_DOREFRESH,NULL,NULL);
			////}
		}

	//
	}

	if(SCAN_TIMER==nIDEvent)
	{
		CString strTemp;	
		if(m_bScanFinished)
		{
			CString strTemp;
			//strTemp.Format(_T("Scan finised,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
			strTemp.Format(_T("Scan finished,found %d node(s)"),m_pScanner->m_szTstatScandRet.size()+m_pScanner->m_szNCScanRet.size());
			SetPaneString(1,strTemp);
			g_bPauseMultiRead=FALSE;
			KillTimer(SCAN_TIMER);
			m_wndWorkSpace.m_TreeCtrl.Invalidate();			
// 			CString strTemp;
// 			//strTemp.Format(_T("Scan finised,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
// 			strTemp.Format(_T("Scan finished,found %d node(s)"),m_pScanner->m_szTstatScandRet.size()+m_pScanner->m_szNCScanRet.size());
// 			SetPaneString(1,strTemp);
// 			g_bPauseMultiRead=FALSE;
// 			KillTimer(SCAN_TIMER);
// 			m_wndWorkSpace.m_TreeCtrl.Invalidate();			
		}
		else
		{
			//strTemp.Format(_T("Scanning now,please wait,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
			
// 			CString strTemp;
// 			strTemp.Format(_T("ID %d to %d"),g_nStartID,g_nEndID);
// 			CString strTemp2;
// 			strTemp2.Format(_T("Scanning %s, %s. Please wait."), g_strScanInfoPrompt, strTemp);
			CString strTemp;
			if (g_nStartID == -1) // 用这个来区分是scan TStat 还是scan NC
			{
				strTemp = _T("sending scan broadcast");
			}
			else
			{
				strTemp.Format(_T("from address ID %d to %d"),g_nStartID,g_nEndID);
			}

			CString strTip;
			strTip.Format(_T("Scanning %s, %s. Please wait."), g_strScanInfoPrompt, strTemp);


			if(!m_bScanALL)
				SetPaneString(1,strTip);

			if(m_pScanner->m_szTstatScandRet.size() > 0)
			{
				CString strStat = _T("Online.");
				SetPaneString(2, strStat);
			}
			else
			{
				CString strStat = _T("Offline.");
				SetPaneString(2, strStat);
			}			
		}
	}

	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), g_tstat_id, g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
	SetPaneString(0,str);

	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::ClearBuilding()
{
	
	//
	int n=m_buildingLst.size();
	m_pTreeViewCrl->DeleteAllItems();

//	m_subNetLst.clear();
	m_floorLst.clear();
	m_roomLst.clear();

	m_backgroundLst.clear();
//	m_buildingLst.clear();

	
	
}
void CMainFrame::SwitchToPruductType(int nIndex)
{
// 	CString strInfo;
// 	strInfo.Format(_T("CMainFrame::SwitchToPruductType(int nIndex):%d"),nIndex);			
// 	SetPaneString(2, strInfo);

	CView* pNewView = m_pViews[nIndex];
	if (!pNewView)
		return;
	CView* pActiveView =GetActiveView();
	if ( !pActiveView )    // No currently active view
		return;

	if ( pNewView == pActiveView )    // Already there
	{
		goto here;
	}
	m_nCurView = nIndex;    // Store the new current view's index

	// exchange view window ID's so RecalcLayout() works
	UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
	::SetWindowLong(pActiveView->m_hWnd, GWL_ID,
	::GetWindowLong(pNewView->m_hWnd, GWL_ID));
	::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
	// Display and update the new current view - hide the old one    
	pActiveView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_SHOW);
	SetActiveView(pNewView);
	RecalcLayout();
	pNewView->Invalidate();
here:
	switch(nIndex)
	{
	case 0:
		{
//20120427
#if 0
		
			if (nFlag == 6)
			{
				//multi_register_value[]列表交换。
				//读取TXT
				memset(tempchange,0,sizeof(tempchange));
				memset(temptstat6,0,sizeof(temptstat6));
				memcpy(temptstat6,multi_register_value,sizeof(multi_register_value));
				int index = 0;

				for (int i = 0;i<512;i++)
				{
					index = reg_tstat6[i];
					tempchange[index] = temptstat6[i];
				}

				memcpy(multi_register_value,tempchange,sizeof(multi_register_value));

			}
#endif
//20120427

//			theApp.cm5_timer = false;	//CM5
			m_nCurView=0;

// 			strInfo.Format(_T("CMainFrame::SwitchToPruductType(int nIndex):FreshIOGridTable()"));			
// 			SetPaneString(2, strInfo);
			((CT3000View*)m_pViews[m_nCurView])->FreshIOGridTable();
// 			strInfo.Format(_T("CMainFrame::SwitchToPruductType(int nIndex):Fresh"));			
// 			SetPaneString(2, strInfo);
			((CT3000View*)m_pViews[m_nCurView])->Fresh();
// 			strInfo.Format(_T("CMainFrame::SwitchToPruductType(int nIndex):Fresh:END"));			
// 			SetPaneString(2, strInfo);
		}
		break;
	case 1:
		{
//			theApp.cm5_timer = false;	//NetControllView
			m_nCurView=1;    
			((CNetworkControllView*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case 2:
		{
			m_nCurView=2;
			int nSerialNum=get_serialnumber();

			((CGraphicView*)m_pViews[m_nCurView])->InitGraphic(nSerialNum,g_tstat_id);

		}
		break;
	case 3:
		{
			m_nCurView=3;
		}
		break;
	case 4:		//CM5
		{	
//			theApp.cm5_timer = true;
			m_nCurView=4; 
			((CDialogCM5*)m_pViews[m_nCurView])->Fresh();
		}	
		break;
	case 5:	   //T3
		{
//			theApp.cm5_timer = false;
			m_nCurView =5;
			((CDialogT3*)m_pViews[m_nCurView])->Fresh();
		}
		break;

	case 6://Mini Panel
		{
//			theApp.cm5_timer = false;
			m_nCurView = 6;
		((CDialgMiniPanel*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case 7://AirQuality
		{
			m_nCurView = 7;
			((CAirQuality*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case 8:
		{
			m_nCurView = 8;
			((CLightingController*)m_pViews[m_nCurView])->Fresh();

		}
		break;
	case 9:
	{
	     m_nCurView=9;
		((CHumChamber*)m_pViews[m_nCurView])->Fresh();
	}
	break;
	case  10:
		{
			m_nCurView = 10;
			((CCO2_View*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	}
//here
}
void CMainFrame::SetPaneConnectionPrompt(CString strInfo)
{
	SetPaneString(2,strInfo);
}
void CMainFrame::SwitchToGraphicView()
{
	
	//if(((CT3000View*)m_pViews[0])->m_pSetPtDayDlg!=NULL)
	//	((CT3000View*)m_pViews[0])->m_pSetPtDayDlg->ShowWindow(SW_HIDE);
		

	SwitchToPruductType(2);
}
void CMainFrame::OnAllNodeDatabase()
{
	if(m_strCurMainBuildingName.IsEmpty())
		AfxMessageBox(_T("Please Select a Building First."));
	CAllNodesDiaolg Dlg;
	Dlg.SetBuildingMainName(m_strCurMainBuildingName);

	Dlg.DoModal();
	if(Dlg.m_bChanged)
	{
		ScanTstatInDB();
	}
	
}
void CMainFrame::Scan_Product()
{
	/*
	if(m_subNetLst.size()<=0)
	{
		AfxMessageBox(_T("Please add a building first."));
		return;
	}
	*/

	if(m_pRs->State)
	m_pRs->Close(); 
	if(m_pCon->State)
	m_pCon->Close();
	ClearBuilding();

	

	//g_pauseMultiReadEvent.SetEvent();
	//Sleep(500);

	// oldscan
 	SetTimer(SCAN_TIMER,100,NULL);//lsc


//m_pThreadScan = AfxBeginThread(_ScanproductFun,this);
//* newscan


	g_bEnableRefreshTreeView = FALSE;
	g_bScanStart = TRUE;
	if (m_pScanner != NULL)
	{
		delete m_pScanner;
		m_pScanner = NULL;
	}
	m_pScanner = new CTStatScanner;
	g_ScnnedNum=0;
	m_pScanner->SetParentWnd(this);
	m_pScanner->SetSubnetInfo(m_subNetLst);//vector<Building_info>& szSubnets);

	CString strMB485=_T("Modbus 485");
	CString strProtocol = m_subNetLst.at(m_nCurSubBuildingIndex).strProtocol;
	CString strBaudrate = m_subNetLst.at(m_nCurSubBuildingIndex).strBaudRate;
	m_pScanner->SetBaudRate(strBaudrate);
	
	CString strCom = m_subNetLst.at(m_nCurSubBuildingIndex).strComPort;
	CString tc = strCom.Mid(3);
	int nCom = _wtoi(tc);
	m_pScanner->SetComPort(nCom);

	g_nStartID = 1;
	g_nEndID = 254;

	m_pWaitScanDlg = new CScanDbWaitDlg;	
	m_pWaitScanDlg->SetTStatScaner(m_pScanner);	


	//scan 自动com,tcp扫描，如果单一扫描则不会出现多扫（一个产品有两个标识出现）
	if (strProtocol == strMB485)
	{

		int ret = AfxMessageBox(_T("default will scan via 'modbus 485',\n do you also need 'modbus TCP'?"),MB_YESNOCANCEL ,3);
		if ( ret == IDYES)
		{
			m_pScanner->ScanAll();
			m_pScanner->m_scantype = 3;
		}
		else if(ret == IDNO)
		{
		m_pScanner->ScanComDevice();
			m_pScanner->WaitScan();
			m_pScanner->m_scantype = 1;
		}else
		{
			return;
		}
	}
	else
	{

		int ret1 = AfxMessageBox(_T("default will scan via 'modbus TCP',\n do you also need 'modbus 485'?"),MB_YESNOCANCEL,3); 
		if (ret1 == IDYES)
		{
			m_pScanner->ScanAll();
			m_pScanner->m_scantype = 3;
		}
		else if (ret1 == IDNO)
		{
			m_pScanner->ScanNetworkDevice();
			m_pScanner->WaitScan();
			m_pScanner->m_scantype = 2;
		}else
		{
			return;
		}

	}

	m_pWaitScanDlg->DoModal();	

	m_bScanALL = TRUE;
	//m_bScanFinished = FALSE;
	delete m_pWaitScanDlg;
	m_pWaitScanDlg = NULL;
//*/
}


void CMainFrame::background_binarysearch_netcontroller()
{
	m_binary_search_networkcontroller_background_thread.clear();
	if (g_CommunicationType==0)
	{	Change_BaudRate(9600);
		binarySearchforview_networkcontroller();
		Change_BaudRate(19200);
		binarySearchforview_networkcontroller();
	}
	else
	{
		binarySearchforview_networkcontroller();
	}

}

void CMainFrame::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
{
//	had_scaned=true;////////////////////////////////////////////////had scan
////	if(net_work_is_exist_or_not==true)
////		Sleep(200);	    
////	else
////		Sleep(10);//
	int a=NetController_CheckTstatOnline(devLo,devHi);

	//int kk=read_one(255,7);
	//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	CString temp=_T("");
	if(a>0)
	{
			binary_search_result temp;
			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			temp.id=a;
			nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
			if(nRet>0)
			{
				if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
				{
					srand((unsigned)time(NULL)); 
					SerialNum[0]=rand()%255; 
					SerialNum[1]=rand()%255; 
					SerialNum[2]=rand()%255; 
					SerialNum[3]=rand()%255; 

					write_one(temp.id,0,SerialNum[0]);
					write_one(temp.id,1,SerialNum[1]);
					write_one(temp.id,2,SerialNum[2]);
					write_one(temp.id,3,SerialNum[3]);
				}

				temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
				temp.product_class_id=SerialNum[7];
				temp.hardware_version=SerialNum[8];
			float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
				tstat_version2 /=10;
			}//tstat_version
			
			temp.software_version=tstat_version2;
					if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
			temp.nEPsize=read_one(temp.id,326);
			if(temp.serialnumber>=0)
			m_binary_search_networkcontroller_background_thread.push_back(temp);
			}

	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			binarySearchforview_networkcontroller(devLo,(devLo+devHi)/2);
			binarySearchforview_networkcontroller((devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforview_networkcontroller(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforview_networkcontroller(devLo,(devLo+devHi)/2);
			binarySearchforview_networkcontroller((devLo+devHi)/2+1,devHi);
		}
		else
		{//Two Tstat have the same ID,fewness
			do
			{
				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
				if(Read_One(devLo,10)!=-2)//one times
				{
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
						//	if(!found_same_net_work_controller_by_mac(a))
							if(1)
							{
								bool find=false;//false==no find;true==find
								for(UINT w=0;w<m_binary_search_networkcontroller_background_thread.size();w++)
									if(j==m_binary_search_networkcontroller_background_thread.at(w).id)
									{
										find=true;
										break;
									}
									if(find==false)
									{
										//************************change the Id
										//									Sleep(20);//////////////////////////////////for running is better
										if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
											if(j<devLo)
											{
												/*
												#if 1
												binary_search_result temp;
												temp.baudrate=m_baudrate2;
												temp.id=j;
												temp.product_class_id=read_one(j,7);
												get_serialnumber(temp.serialnumber,j);
												temp.hardware_version=read_one(j,8);
												m_binary_search_networkcontroller_background_thread.push_back(temp);
												if(read_one(j,7)==NET_WORK_CONT_PRODUCT_MODEL)//net work controller
													keep_back_mac_address(j);
												#endif
												*/

												binary_search_result temp;
											//	temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												temp.id=j;
												nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
												if(nRet>0)
												{
													//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
													//	+(SerialNum[1]&0xff00>>8)*16777216;

														if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
														{
															srand((unsigned)time(NULL)); 
															SerialNum[0]=rand()%255; 
															SerialNum[1]=rand()%255; 
															SerialNum[2]=rand()%255; 
															SerialNum[3]=rand()%255; 
															write_one(temp.id,0,SerialNum[0]);
															write_one(temp.id,1,SerialNum[1]);
															write_one(temp.id,2,SerialNum[2]);
															write_one(temp.id,3,SerialNum[3]);

														}

													temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													temp.product_class_id=SerialNum[7];
													temp.hardware_version=SerialNum[8];
														float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
														tstat_version2 /=10;
													else 
													{
														tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
														tstat_version2 /=10;
													}//tstat_version
													
													temp.software_version=tstat_version2;
															if(read_one(temp.id,185)==0)
													temp.baudrate=9600;
												else
												{
														temp.baudrate=19200;
												}
													temp.nEPsize=read_one(temp.id,326);
														
													if(temp.serialnumber>=0)
													
													m_binary_search_networkcontroller_background_thread.push_back(temp);
												}
											}
											binarySearchforview_networkcontroller(devLo,devHi);
											return;
									}
							}
							else
							{
								return;
							}
						}
				}
			}while(1);
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}


BOOL CMainFrame::binary_search_crc(int a)
{//use this for binary search ,inspect
	static int for_binary_search_crc=0;//retry times 10;when return value is -2
	if(a==-2)
	{
		if(for_binary_search_crc<10)
			for_binary_search_crc++;
		else
		{
			for_binary_search_crc=0;
			return true;/////////////////more ten time
		}
	}
	else
		for_binary_search_crc=0;
	return false;
}

void CMainFrame::background_binarysearch()
{
	m_binary_search_product_background_thread.clear();////////^0^
	for(UINT k=0;k<m_binary_search_networkcontroller_background_thread.size();k++)
	{
		m_binary_search_product_background_thread.push_back(m_binary_search_networkcontroller_background_thread.at(k));
	}

	scanForTstat();

}
void CMainFrame::scanForTstat()
{
	binarySearchforview();
}
void CMainFrame::binarySearchforview(BYTE devLo, BYTE devHi)
{
	//Sleep(200);	//???
	int nCount=0;
	int a=CheckTstatOnline(devLo,devHi);

	//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	char c_array_temp[5]={'0'};
	CString temp=_T("");
	if(a>0)
	{
		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;

		binary_search_result temp;
//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=Read_Multi(a,&SerialNum[0],0,9,2);
		if(nRet>0)
		{
			temp.id=a;
			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			temp.product_class_id=SerialNum[7];
			temp.hardware_version=SerialNum[8];

				float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
				tstat_version2 /=10;
			}//tstat_version
			
			temp.software_version=tstat_version2;
				if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
					temp.nEPsize=read_one(temp.id,326);
				if(temp.serialnumber>=0)
				m_binary_search_product_background_thread.push_back(temp);
		}
		else
			return;
	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			binarySearchforview(devLo,(devLo+devHi)/2);
			binarySearchforview((devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforview(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforview(devLo,(devLo+devHi)/2);
			binarySearchforview((devLo+devHi)/2+1,devHi);
		}
		else
		{
			do
			{
				/*
				nCount++;
				if(nCount>=2)
				{
					nCount=0;
					break;
				}
				*/
				
				//if(Read_One(devLo,10)==-2)
				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
			//	if(read_one(devLo,10)==-2)
			//	{
			//		break;
			//	}
				

			//
				if(Read_One(devLo,10)!=-2)//one times
			//	{

			//	}
			//	else
				{
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
						//	if(!found_same_net_work_controller_by_mac(a))
							if(1)
							{
								bool find=false;//false==no find;true==find
								for(UINT w=0;w<m_binary_search_product_background_thread.size();w++)
								{
									if(j==m_binary_search_product_background_thread.at(w).id)
									{
										find=true;
										break;
									}
								}
								if(find==false)
								{
									if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
										if(j<devLo)
										{
												binary_search_result temp;
											//	temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												temp.id=j;
												nRet=Read_Multi(j,&SerialNum[0],0,9,2);
												if(nRet>0)
												{		
													temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													temp.product_class_id=SerialNum[7];
													temp.hardware_version=SerialNum[8];

														float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
														tstat_version2 /=10;
													else 
													{
														tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
														tstat_version2 /=10;
													}//tstat_version
													
													temp.software_version=tstat_version2;
															if(read_one(temp.id,185)==0)
														temp.baudrate=9600;
													else
													{
															temp.baudrate=19200;
													}

													temp.nEPsize=read_one(temp.id,326);
											
													if(temp.serialnumber>=0)
													{
															
															m_binary_search_product_background_thread.push_back(temp);
													}
												}

										}
									binarySearchforview(devLo,devHi);
									return;
								}
							}
							else
							{
								return;
							}
						}
				}
			}while(1);
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}
int CMainFrame::find_Address_towrite ()
{
	if (m_product.size()<=0)
	{
		return 1;
	}
	bool find=TRUE;
	for(int j=1;j<=254;j++)
	{

		for(UINT i=0;i<m_product.size();i++)
		{
			if(j==m_product.at(i).product_id)
			{//find same id,because the baudrate is different
				find=true;
				break;
			}
			else
			{
				find=FALSE;
				continue;
			}
		}
		if (find)
		{
			continue;
		}
		if (!find)
		{
			return j;
		}
	}
	return -1;

}

CString CMainFrame::GetDeviceClassName(int nClassID)
{
	CString strClassName;
	switch(nClassID)
	{
	case 2:strClassName=g_strTstat5a;break;
	case 1:strClassName=g_strTstat5b;break;
	case 3:strClassName=g_strTstat5b;break;
	case 4:strClassName=g_strTstat5c;break;
	case 12:strClassName=g_strTstat5d;break;
	case PM_NC:strClassName=g_strnetWork;break;
	case NET_WORK_OR485_PRODUCT_MODEL:strClassName=g_strOR485;break;
	case 17: strClassName=g_strTstat5f;break;
	case 18:strClassName=g_strTstat5g;break;
	case 16:strClassName=g_strTstat5e;break;
	case 19:strClassName=g_strTstat5h;break;
	case 27:strClassName=g_strTstat7;break;
	case 26:strClassName=g_strTstat6;break;
	case 13:
	case 14:break;
	default:strClassName=g_strTstat5a;break;
	}

	return strClassName;
}


void CMainFrame::AddScanedDeviceToDatabase(Building_info buildInfo)
{
	ConnectSubBuilding(buildInfo); // 连接building，根据building table
	CString strTemp;
	strTemp.Format(_T("Scanning the subnet:%s..."),buildInfo.strBuildingName);
	SetPaneString(1,strTemp);
	
	if(g_Scanfully)
	{
		background_binarysearch_netcontroller();   // 专门scan NC
		background_binarysearch();
		g_ScnnedNum=254;
	}

	if(g_ScanSecurity)
	{
		m_binary_search_product_background_thread.clear();
		g_ScnnedNum=g_nStartID;
		for(int i=g_nStartID;i<=g_nEndID;i++)
		{
			if(g_bCancelScan)
				break;
			int nID=-1;
			g_ScnnedNum++;
			binary_search_result temp;
			int nBraudRat=19200;
			if (g_CommunicationType==0)
			{
				nBraudRat=19200;
				Change_BaudRate(19200);
				nID=read_one(i,6);
				if(nID<=0)
				{
					nBraudRat=9600;
					Change_BaudRate(9600);
					nID=read_one(i,6);
				}
				temp.id=nID;
			}
			if (g_CommunicationType==1)
			{
				nID=read_one(i,6);
				temp.id=nID;
			}
			if(temp.id>0)
			{
				temp.baudrate=nBraudRat;
				unsigned short SerialNum[9];
				memset(SerialNum,0,sizeof(SerialNum));
				int nRet=0;//
				float tstat_version2;
				nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
				if(nRet>0)
				{
					//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
					//	+(SerialNum[1]&0xff00>>8)*16777216;

					temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					temp.product_class_id=SerialNum[7];
					temp.hardware_version=SerialNum[8];
					tstat_version2=SerialNum[4];//tstat version			
					if(tstat_version2 >=240 && tstat_version2 <250)
						tstat_version2 /=10;
					else 
					{
						tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
						tstat_version2 /=10;
					}//tstat_version
					temp.software_version=tstat_version2;
					if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
					//nRet=0;
					temp.nEPsize=read_one(temp.id,326);
					if(temp.serialnumber>=0)
					m_binary_search_product_background_thread.push_back(temp);
				}
			}
		}
		g_ScnnedNum=254;
	}


	m_bScanALL=TRUE;
	Sleep(500);
_ConnectionPtr t_pCon;//for ado connection
	try
	{

	
	//_RecordsetPtr t_pRs;//for ado 
	t_pCon.CreateInstance(_T("ADODB.Connection"));
	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	}
	catch (...)
	{

	}

	CString strfloor_name;
	CString strroom_name;
	CString strproduct_name;
	CString strproduct_class_id;
	CString strproduct_id;
	CString strscreen_name;
	CString strbackground_bmp;
	CString strSerialnumber;
	CString strbuildingName;
	CString stemp;


	int lll=m_binary_search_product_background_thread.size();
	
	for(UINT j=0;j<m_binary_search_product_background_thread.size();j++)
	{
		stemp = GetDeviceClassName(m_binary_search_product_background_thread.at(j).product_class_id);
// 		switch(m_binary_search_product_background_thread.at(j).product_class_id)
// 		{
// 		case 2:stemp=g_strTstat5a;break;
// 		case 1:stemp=g_strTstat5b;break;
// 		case 3:stemp=g_strTstat5b;break;
// 		case 4:stemp=g_strTstat5c;break;
// 		case 12:stemp=g_strTstat5d;break;
// 		case NET_WORK_CONT_PRODUCT_MODEL:stemp=g_strnetWork;break;
// 		case NET_WORK_OR485_PRODUCT_MODEL:stemp=g_strOR485;break;
// 		case 17: stemp=g_strTstat5f;break;
// 		case 18:stemp=g_strTstat5g;break;
// 		case 16:stemp=g_strTstat5e;break;
// 		case 19:stemp=g_strTstat5h;break;
// 		case 13:
// 		case 14:break;
// 		default:stemp=g_strTstat5a;break;
// 		}

		BOOL bFind=FALSE;
		int m=-1;
		for (UINT k=0;k<m_product.size();k++)
		{
			if (m_product.at(k).serial_number==m_binary_search_product_background_thread.at(j).serialnumber)
			{
				m=k;
				bFind=TRUE;//查找到是已经加入数据库的 产品。
						  //已经在building数据中的tstate 将不会在newNodes 中显示。
				break;
			}
		}
		if (bFind&&m!=-1)
		{
			//if have same serial number ,but different id,changed the scanned id,
			if(m_product.at(m).product_id!=m_binary_search_product_background_thread.at(j).id)
			{
				//changed the id ,
				CChangeIDDlg dlg;
				dlg.SetPromtionTxt(m_product.at(m).serial_number,m_product.at(m).product_id,m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);
				if(dlg.DoModal()==IDOK)
				{
					if(dlg.m_nChange==0)
					{
						int nRet=-1;
						//Change_BaudRate(19200);
						for(int i=0;i<3;i++)
						{
							register_critical_section.Lock();
							nRet=Write_One(m_binary_search_product_background_thread.at(j).id,6,m_product.at(m).product_id);
							if(nRet>0)
								multi_register_value[6]=m_product.at(m).product_id;//mark***********************
							register_critical_section.Unlock();
							if(nRet!=-2 && nRet!=-3)
							{
								break;
							}
						}
					}
					if(dlg.m_nChange==2)
					{
						CString strID;
						CString strSerial;
						CString strProductName;
						CString strScreenName;

						CString strSql;

					try

					{



						strID.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
						strSerial.Format(_T("%d"),m_binary_search_product_background_thread.at(j).serialnumber);

						strProductName.Format(_T("%s:%u--%u"),stemp,m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);
						strScreenName = _T("Screen(S:") + strSerial + _T("--") + strID + _T(")");

						strSql.Format(_T("update ALL_NODE set Product_ID ='%s', Product_Name = '%s', Screen_Name = '%s' where Serial_ID = '%s'"),strID,strProductName,strScreenName,strSerial);
						t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

					}
					catch(_com_error *e)
					{
						AfxMessageBox(e->ErrorMessage());
					}

					}
					if(dlg.m_nChange==3)
					{
						//remove it
						m_binary_search_product_background_thread.erase(m_binary_search_product_background_thread.begin()+j);
						continue;
					}


					

					/*
					bRet=write_one(m_binary_search_product_background_thread.at(j).id,6,m_product.at(m).product_id);
					if(bRet>=0)
					{
						m_binary_search_product_background_thread.at(j).id=m_product.at(m).product_id;
					}
*/

				}
				
			}
		//	else
		//	{
				continue;// this alread inserted to database:
		//	}
		}
		
		strfloor_name=_T("Floor_xx");
		strroom_name=_T("Room_xx");
		strproduct_name.Format(_T("%s:%u--%u"),stemp,m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);

		strproduct_class_id.Format(_T("%d"),m_binary_search_product_background_thread.at(j).product_class_id);
		strproduct_id.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
		strscreen_name.Format(_T("Sceen(S:%d--%d)"),m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);
		strbackground_bmp=_T("Clicking here to add a image...");
		strSerialnumber.Format(_T("%d"), m_binary_search_product_background_thread.at(j).serialnumber);
		CString temp_str;

		CString strSubNetName;
		CString strmainBuildingName;

		strSubNetName=buildInfo.strBuildingName;//pView->m_Buildinglst.at(0);
		strmainBuildingName=m_strCurMainBuildingName;

		CString strBaudRate;
		CString strHardVersion;
		CString strSoftwareVersion;
		CString strEpSize;
		CString strIspVersion;

		strBaudRate.Format(_T("%d"),m_binary_search_product_background_thread.at(j).baudrate);
		strHardVersion.Format(_T("%.1f"),m_binary_search_product_background_thread.at(j).hardware_version);
		strSoftwareVersion.Format(_T("%.1f"),m_binary_search_product_background_thread.at(j).software_version);
		strEpSize.Format(_T("%d"),m_binary_search_product_background_thread.at(j).nEPsize);

		CString strSql;
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize) values('"+strmainBuildingName+"','"+strSubNetName+"','"+strSerialnumber+"','"+strfloor_name+"','"+strroom_name+"','"+strproduct_name+"','"+strproduct_class_id+"','"+strproduct_id+"','"+strscreen_name+"','"+strBaudRate+"','"+strbackground_bmp+"','"+strHardVersion+"','"+strSoftwareVersion+"','"+strEpSize+"')"));
	
		
		try
		{
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		tree_product pruductNode;
		pruductNode.product_id=m_binary_search_product_background_thread.at(j).serialnumber;
		pruductNode.product_id=m_binary_search_product_background_thread.at(j).id;
		m_product.push_back(pruductNode);


		
/*
		int productID,serialNumber,classID;
		////////////////////////////////////////////////////////////////如果adreess相同，//blog
		for(int i=0;i<m_binary_search_product_background_thread.size();i++)
		{
			for(int k=0;k<m_product.size();k++)
			{
				productID=m_product.at(k).product_id;
				serialNumber=m_product.at(k).serial_number;
				classID=m_product.at(k).product_class_id;
				if(productID==m_binary_search_product_background_thread.at(i).id)
				{
					int ntempID=find_Address_towrite();
					if(serialNumber!=m_binary_search_product_background_thread.at(i).serialnumber)
					{
						BOOL bRet=-1;
						Change_BaudRate(19200);
						bRet=write_one(productID,6,ntempID);
						if(bRet<0)
						{
							Change_BaudRate(9600);
						bRet=write_one(productID,6,ntempID);
						}
						
						if(bRet>0)
						{
							unsigned short SerialNum[9];
							memset(SerialNum,0,sizeof(SerialNum));
							int nRet=0;
							nRet=Read_Multi(ntempID,&SerialNum[0],0,9,3);
							int nSerialNumber=0;
							if(nRet>0)
							{	
								nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
								if(nSerialNumber>0)
								{
									CString strSerialNum;
									CString strID;
									strSerialNum.Format(_T("%d"),nSerialNumber);
									strID.Format(_T("%d"),SerialNum[6]);

									/////根据Serial Number 更新数据库中的记录。
									//	CString execute_str="update Every_product set Product_ID = 0 where Serial_ID = nSerialNumber";
									CString execute_str;
									execute_str.Format(_T("update ALL_NODE set Product_ID = %d where Serial_ID = %d"),SerialNum[6],nSerialNumber);
									t_pCon->Execute(execute_str.GetString(),NULL,adCmdText);

									m_product.at(k).product_id=SerialNum[6];
									m_product.at(k).serial_number=nSerialNumber;
									
									}
					
								}
							}
						}
					}
				}
				

			}*/
		//endof blog;		
	}
	if(t_pCon->State)
			t_pCon->Close();
}

void CMainFrame::OnLabel()
{
}
void CMainFrame::OnLabe2()
{
}
void CMainFrame::OnLabe3()
{
}
void CMainFrame::SaveConfigFile()
{
	int nret=0;
	g_bEnableRefreshTreeView = FALSE;

	if(read_one(g_tstat_id,6)<=0)
		return;
	float version=get_curtstat_version();
	CString strFilter;
	CString strFilename;
	strFilter =_T( "Text File|*.txt|All File|*.*|");
	CFileDialog dlg(false,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	if (dlg.DoModal() != IDOK)
		return;
	strFilename=dlg.GetPathName();

	BeginWaitCursor();
	CString strTips = _T("T3000 is ready to save config file, start now?");

	SetPaneString(1, strTips);
	AfxMessageBox(strTips);

	if(multi_register_value[7]==100)//NC
	{
		strTips.Format(_T("Config file \" %s \" is saving, please waiting..."), strFilename);
		SetPaneString(1, strTips);

		save_schedule_2_file((LPTSTR)(LPCTSTR)strFilename,g_tstat_id);

		strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
		SetPaneString(1, strTips);
	}else if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
	{
// 		nret=write_one(g_tstat_id,321,4);
// 		nret=write_one(g_tstat_id,322,0);
// 		nret=write_one(g_tstat_id,323,0);
// 		nret=write_one(g_tstat_id,325,0);
// 		int nSpecialValue=read_one(g_tstat_id,326);
// 
// 		if(nSpecialValue==1)
// 		{
// 			//	write_one(g_tstat_id,324,0);
// 		}

//0904
		int index = 0;

		for (int i = 0;i<512;i++)
		{
			index = reg_tstat6[i];
			tempchange[index] = newtstat6[i];
		}
		memcpy(multi_register_value,tempchange,sizeof(multi_register_value));
//0904

		Save2File_ForTwoFilesTSTAT67((LPTSTR)(LPCTSTR)strFilename);


		strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);

		SetPaneString(1, strTips);
		AfxMessageBox(strTips);
	}
	else//save tstat config file:
	{
		nret=write_one(g_tstat_id,321,4);
		nret=write_one(g_tstat_id,322,0);
		nret=write_one(g_tstat_id,323,0);
		nret=write_one(g_tstat_id,325,0);
		int nSpecialValue=read_one(g_tstat_id,326);

		if(nSpecialValue==1)
		{
		//	write_one(g_tstat_id,324,0);
		}
		Save2File_ForTwoFiles((LPTSTR)(LPCTSTR)strFilename);

	
		strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
		
		SetPaneString(1, strTips);
		AfxMessageBox(strTips);
// 		CMFCStatusBar * pStatusBar=NULL;
// 		if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
// 			return;
// 		pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		
	}

	EndWaitCursor();
	g_bEnableRefreshTreeView = TRUE;
}

LRESULT CMainFrame::OnFreshStatusBar(WPARAM wParam, LPARAM lParam)
{
//pStatusBar->SetPaneText(nIndext,str.GetString());
	status_info*  pinfoStatusbar;
	pinfoStatusbar=(status_info*)wParam;
	//pinfoStatusbar.nIndex=nIndext;
//	infoStatusbar.strInfoText=str;
	m_wndStatusBar.SetPaneText(pinfoStatusbar->nIndex,pinfoStatusbar->strInfoText);


	return 1;
}


void CMainFrame::OnDestroy()
{
#if 1
	CDialogInfo *pDialogInfo = NULL;

	try
	{
// 		if(pDialogInfo==NULL)
// 		{
			pDialogInfo = new CDialogInfo;
			pDialogInfo->Create(IDD_DIALOG_INFO,this);
			pDialogInfo->ShowWindow(SW_SHOW);
			pDialogInfo->CenterWindow();
			pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Processing Information,please wait..."));
//		}

	}
	catch (...)//这个无效，当pDialogInfo->Create(IDC_STATIC_INFO,this);中的ID写错时，这个函数没有throw抛出错误，所以捕获不到
	{

	}



// 	CCDialogISPTOOL Disptool;
// 	Disptool.DoModal();
	int temp =1;

	UpdataSlider(temp);




	g_bEnableRefreshTreeView = FALSE;
	HTREEITEM htiSel = m_pTreeViewCrl->GetSelectedItem();
	SaveTreeNodeRecordToReg(htiSel);

	CFrameWndEx::OnDestroy();

	g_killMultiReadEvent.SetEvent();

	Sleep(500);//wait for the end of the thread.
	if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 1000) != WAIT_OBJECT_0)

	//Sleep(500);//wait for the end of the thread.
	if (m_pFreshMultiRegisters) 

	{
		if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 3000) == WAIT_OBJECT_0)
		{}
		else
		{		
			BOOL bRet = TerminateThread(m_pFreshMultiRegisters->m_hThread,0);
			delete m_pFreshMultiRegisters;
		}

	}

	// 结束线程
	
	if (m_pRefreshThread) 
	{
		HANDLE hp=m_pRefreshThread->m_hThread;
		g_bEnableRefreshTreeView = FALSE;
		PostThreadMessage(m_pRefreshThread->m_nThreadID,  WM_QUIT,0,0);

		if (WaitForSingleObject(hp, 1000) != WAIT_OBJECT_0)

		if (WaitForSingleObject(m_pRefreshThread->m_hThread, 100) == WAIT_OBJECT_0)

		{
		}
		else
		{
			TerminateThread(m_pRefreshThread->m_hThread, 0);
			 
		}
		//CloseHandle(hp);
		//m_pRefreshThread->Delete();
	}

	if (is_connect())
	{
		close_com(); // added by zgq:12-16-2011
	}

	

	if (pDialogInfo!=NULL)
	{
		delete pDialogInfo;
		pDialogInfo = NULL;
	}
#endif

	// TODO: Add your message handler code here
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message==WM_MYMSG_REFRESHBUILDING)
	{
		//Sleep(1000);
		//AfxMessageBox(_T("There is no default building,please select a building First."));
		BOOL bTemp = g_bEnableRefreshTreeView ;
		g_bEnableRefreshTreeView = FALSE;
		ScanTstatInDB();
		g_bEnableRefreshTreeView |= bTemp;
	}

	return CFrameWndEx::WindowProc(message, wParam, lParam);
}
void CMainFrame::OnChangeAdminPassword()
{


	CChangePasswordDlg dlg;
	//dlg.DoModel();
	dlg.DoModal();
	
}
void CMainFrame::OnUserMannageMent()
{
	CManageAccountDlg Dlg;
	Dlg.DoModal();
}
void CMainFrame::OnUserLogin()
{
	CLoginDlg Dlg(g_buser_log_in);
	Dlg.DoModal();

}
void CMainFrame::OnUserMannageMentUpdate(CCmdUI *pCmdUI)
{
	if(g_UserLevel==0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::GetIONanme()
{
	try
	{

	 
			_ConnectionPtr m_ConTmp;
			_RecordsetPtr m_RsTmp;
			m_ConTmp.CreateInstance("ADODB.Connection");
			m_RsTmp.CreateInstance("ADODB.Recordset");
			m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

			CString strSerial;
			strSerial.Format(_T("%d"),g_serialNum);
			strSerial.Trim();
			CString strsql;
			strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
			m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	

			if(VARIANT_FALSE==m_RsTmp->EndOfFile)
			{	
				CString str_temp;
				str_temp.Empty();
				_variant_t temp_variant;
			
				temp_variant=m_RsTmp->GetCollect("SENSORNAME");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Internal Sensor");
				g_strSensorName=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT1");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 1");
				g_strInName1=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT2");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 2");
				g_strInName2=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT3");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 3");
				g_strInName3=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT4");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 4");
				g_strInName4=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT5");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 5");
				g_strInName5=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT6");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 6");
				g_strInName6=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT7");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 7");
				g_strInName7=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT8");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 8");
				g_strInName8=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT9");
			//	temp_variant=m_RsTmp->GetCollect("Humidity Sensor");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Humidity Sensor");
				g_strInHumName=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT1");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 1");
				g_strOutName1=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT2");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 2");
				g_strOutName2=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT3");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 3");
				g_strOutName3=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT4");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 4");
				g_strOutName4=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT5");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 5");
				g_strOutName5=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT6");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 6");
				g_strOutName6=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT7");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 7");
				g_strOutName7=str_temp;

			}
			else
			{
				g_strSensorName =_T("Internal Sensor");
				g_strInName1=_T("Input 1");
				g_strInName2=_T("Input 2");
				g_strInName3=_T("Input 3");
				g_strInName4=_T("Input 4");
				g_strInName5=_T("Input 5");
				g_strInName6=_T("Input 6");
				g_strInName7=_T("Input 7");
				g_strInName8=_T("Input 8");
				
				g_strInHumName = _T("Humidity Sensor");


				g_strOutName1=_T("Output 1");
				g_strOutName2=_T("Output 2");
				g_strOutName3=_T("Output 3");
				g_strOutName4=_T("Output 4");
				g_strOutName5=_T("Output 5");
				g_strOutName6=_T("Output 6");
				g_strOutName7=_T("Output 7");
				g_strOutName8=_T("Output 8");


			}
			if(m_RsTmp->State) 
				m_RsTmp->Close(); 
			if(m_ConTmp->State)
				m_ConTmp->Close();	

	}
	catch (...)
	{

	}



}
void CMainFrame::OnHelp()
{
	CString strHelp=g_strExePth+_T("T3000help.chm");
	::HtmlHelp(NULL, strHelp, HH_DISPLAY_TOPIC, 0);
}
void CMainFrame::OnImportDatase()
{
	CImportDatabaseDlg Dlg;
	Dlg.DoModal();
	
}
void CMainFrame::OnCaliBrate()
{
	CToolCalibrateDlg Dlg;
	Dlg.DoModal();
}

void CMainFrame::OnGloabalOverrides()
{
	CToolCalibrateDlg Dlg;
	Dlg.DoModal();

}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_F8)
		{
			ReFresh();
			return 1;
		}
		if(pMsg->wParam == VK_F3)
		{
			OnToolRefreshLeftTreee();
		}

		if(pMsg->wParam == VK_F2)
		{
			OnToolErease();
			return 1;
		}
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}
void CMainFrame::ReFresh()
{
//	int i;
	register_critical_section.Lock();
	for(int i=0;i<10;i++) //Modify by Fance , tstat 6 has more register than 512;
	{
		Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);		
	}
	register_critical_section.Unlock();
	//Modify by Fance
	//Use product_register_value  to change all the multi_register_value[] to multi_register_value
	//such as :
	//multi_register_value[101]  ----> product_register_value[MODBUS_TEMPRATURE_CHIP]
	//In order to make sure the program also run correctly , so create a new array to replace.
	//When all of the number like "101" has changed to MODBUS_TEMPRATURE_CHIP.
	//the multi_register_value then will by annul.
	memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));


}
BOOL CMainFrame::GetIPbyHostName(CString strHostName,CString& strIP)
{
	 
	WSAData   wsdata; 
	WORD wVersionRequested=MAKEWORD(2,0);   
    int  err=WSAStartup(wVersionRequested,&wsdata);   
    hostent     *pHostent=NULL   ;   
//    const char   *addr;  
//	char   hostName[255]; 

//pHostent   =   gethostbyname("www.google.com\0");   
 // sockaddr_in   sa;   
 // memcpy(&sa.sin_addr.s_addr,pHostent->h_addr_list[0],pHostent->h_length);   

    pHostent   =   gethostbyname(CW2A(strHostName));   
    if(pHostent==NULL)   
       return   false;  
    if(pHostent->h_addr_list==NULL)   
        return   false;   
    sockaddr_in   sa;   
    memcpy(&sa.sin_addr.s_addr,pHostent->h_addr_list[0],pHostent->h_length);     
	strIP.Format(_T("%d.%d.%d.%d"),sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);  
	WSACleanup();
	return TRUE;

}
void CMainFrame::OnToolErease()
{
	CEreaseDlg Dlg;
	Dlg.DoModal();
	
}
void CMainFrame::OnToolFresh()
{
	ReFresh();
}
void CMainFrame::OnToolRefreshLeftTreee()
{
		BeginWaitCursor();
	RefreshTreeView();
	EndWaitCursor();
	/*
	if(m_subNetLst.size()<=0)	
		return;
	BeginWaitCursor();
	for(UINT i=0;i<m_product.size();i++)
	{
		BOOL bOnLine=FALSE;
		UINT nSerialNumber=0;
		int nID;
		
		if(m_strCurSubBuldingName.CompareNoCase(m_product.at(i).BuildingInfo.strBuildingName)==0)
		{
			int nIDNode=m_product.at(i).product_id;
			nSerialNumber=m_product.at(i).serial_number;
			//int newnID=read_one(nID,6,2);
			if (g_CommunicationType==0)
			{
				m_nbaudrat=19200;
				Change_BaudRate(19200);
				nID=read_one(g_tstat_id,6,2);
				if(nID<0)
				{
					m_nbaudrat=9600;
					Change_BaudRate(9600);
					nID=read_one(nID,6,2);
					bOnLine=FALSE;
				}
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(nID,&SerialNum[0],0,4,3);
					int nSerialNumberRead;
					
					if(nRet>=0)
					{
			
						nSerialNumberRead=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumberRead==nSerialNumber)
							bOnLine=TRUE;
					}
			

				}


			}
			if (g_CommunicationType==1)
			{
			  nID=read_one(nID,6);
			  if(nID<0)
				  bOnLine=FALSE;
			  if(nID>0)
			  {
				  		unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(nID,&SerialNum[0],0,4,3);
					int nSerialNumberRead;
					
					if(nRet>=0)
					{
			
						nSerialNumberRead=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumberRead==nSerialNumber)
							bOnLine=TRUE;
					}
			
			  }

			}
			
			if(bOnLine>0)
			{
				SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(m_product.at(i).product_item ,true);
			
			}
			else
			{
				
				SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(m_product.at(i).product_item ,false);
				if(nID==g_tstat_id)
					memset(&multi_register_value[0],0,sizeof(multi_register_value));
			}
			

		}

	}
	EndWaitCursor();
	*/
} 
///////////////////////////////////////////////
/*
	m_strCurSubBuldingName=product_Node.BuildingInfo.strBuildingName;
			BOOL bOnLine=FALSE;
			UINT nSerialNumber=0;
			if (g_CommunicationType==0)
			{
				m_nbaudrat=19200;
				Change_BaudRate(19200);
				nID=read_one(g_tstat_id,6,2);
				if(nID<0)
				{
					m_nbaudrat=9600;
					Change_BaudRate(9600);
					nID=read_one(g_tstat_id,6,2);
					bOnLine=FALSE;
				}
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);
					
					if(nRet>=0)
					{
			
						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
			

				}
			}
			if (g_CommunicationType==1)
			{
			  nID=read_one(g_tstat_id,6);
			  if(nID<0)
				  bOnLine=FALSE;
			  if(nID>0)
			  {
				  	unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);
					UINT nSerialNumber=0;
					if(nRet>0)
					{
			
						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
			  }

			}
			if(bOnLine)
			{
				SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,true);
			
			}
			else
			{
				
				SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,false);	
				memset(&multi_register_value[0],0,sizeof(multi_register_value));
				return;
				//CString strtemp;
				//strtemp.Format(_T("Failed to connect the Tstate Serial number=%d,Address ID=%d"),m_product.at(i).serial_number,m_product.at(i).product_id);
				//AfxMessageBox(strtemp);
				//return;
			}
			*/


//////////////////////////////////////////////////////////////////////////
// 2010-11-29;Added by zgq;添加启动时更新TreeCtrl的功能，使得TreeCtrl能真实反映连接状况
// 此函数主要用于连接一个选定的节点，并将状态反映到TreeCtrl。
//
void CMainFrame::DoConnectToANode( const HTREEITEM& hTreeItem )
{
	//20120420
	CDialog_Progess* pDlg = new CDialog_Progess(this,1,7);
	//创建对话框窗口
	pDlg->Create(IDD_DIALOG10_Progress, this);

	//居中显示
	//pDlg->CenterWindow();
	//void MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );
	//pDlg->MoveWindow(100,100,500,1000);
	pDlg->ShowProgress(0,0);
	//显示对话框窗口
	pDlg->ShowWindow(SW_SHOW);




	// 	int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	// 	{
	// 		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	// 			return -1;
	// 
	//		CRect rectDummy (0, 0,200, 600);//// from left, top, right, and bottom

	RECT RECT_SET1;
	GetClientRect(&RECT_SET1);
	pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);



	//20120420
	float flagsoftwareversion;



	//HTREEITEM hSelItem=m_pTreeViewCrl->GetSelectedItem();
	HTREEITEM hSelItem=hTreeItem;
	int nCounts=m_product.size();
	tree_product product_Node;
	int nSelectID=-1;
	UINT nSelectSerialNumber;
	for(UINT i=0;i<m_product.size();i++)
	{
		if(hSelItem==m_product.at(i).product_item )
		{
			int nID=-1;
			int nTRet=-1;
			//g_tstat_id=m_product.at(i).product_id;
			product_Node=m_product.at(i);

			//************************************
			register_critical_section.Lock();
			g_tstat_id_changed=TRUE;
			g_tstat_id=product_Node.product_id;
			register_critical_section.Unlock();
			//***************************************
			nSelectID=product_Node.product_id;
			nSelectSerialNumber=product_Node.serial_number;
			flagsoftwareversion = product_Node.software_version;


			CString strTitle;
			strTitle=product_Node.BuildingInfo.strMainBuildingname;
			strTitle+=_T("->");
			strTitle+=product_Node.BuildingInfo.strBuildingName;

			m_wndWorkSpace.SetWindowText(strTitle);

			//AfxMessageBox(product_Node.BuildingInfo.strMainBuildingname);

			g_strImagePathName=product_Node.strImgPathName;
			if (!((product_Node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)||(product_Node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0) ||(product_Node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
			{

				if(product_Node.BuildingInfo.hCommunication==NULL||m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0)
				{
					pDlg->ShowProgress(2,10);//20120220
					BOOL bRet = ConnectSubBuilding(product_Node.BuildingInfo);
					if (!bRet)
					{
						CheckConnectFailure(product_Node.BuildingInfo.strIp);
						pDlg->ShowWindow(SW_HIDE);//20120220
						delete pDlg;//20120220
						pDlg = NULL;
					}
				}
			}else
			{

				if (product_Node.BuildingInfo.strComPort.CompareNoCase(_T("N/A")) == 0)
				{
					AfxMessageBox(_T("Please select COM port"));//\nDatabase->Building config Database
					CAddBuilding  Dlg;
					if(Dlg.DoModal()==IDOK)
					{
					}

				}else
				{

					int nComPort = _wtoi(product_Node.BuildingInfo.strComPort.Mid(3));


					CString strInfo;
					strInfo.Format(_T("COM %d Connected: Yes"), nComPort);			
					//SetPaneCommunicationPrompt(strInfo);
					SetPaneString(1, strInfo);
					BOOL  bret = open_com(nComPort);
					if (!bret)
					{
						AfxMessageBox(_T("COM port open failure!"));			
						if (pDlg !=NULL)
						{
							pDlg->ShowWindow(SW_HIDE);
							delete pDlg;
						}
						return;
					}else
					{
						g_CommunicationType=0;
					}
				}
			}
			if(m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0&&m_hCurCommunication!=NULL)
			{
			}

			m_strCurSubBuldingName=product_Node.BuildingInfo.strBuildingName;
			BOOL bOnLine=FALSE;
			UINT nSerialNumber=0;
			if (g_CommunicationType==0)
			{
				m_nbaudrat=19200;
				Change_BaudRate(19200);
				nID=read_one(g_tstat_id,6,5);
				if(nID<0)		
				{
					m_nbaudrat=9600;
					Change_BaudRate(9600);
					nID=read_one(g_tstat_id,6,5);
					if(nID<0)
						Change_BaudRate(19200);
					bOnLine=FALSE;
				}
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);

					if(nRet>=0)
					{

						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
				}else
				{
					// 					CString str;
					// 					str.Format(_T("communication failure,error code:%d\nor select other com port to try"),nID);
					// 					AfxMessageBox(str);
					//Reset the COM or check to make sure the product is open
					//	AfxMessageBox(_T("Detect the product model not corresponding\nSelect COM port,try again!"));//\nDatabase->Building config Database	
					AfxMessageBox(_T("Detect the product model not corresponding\nReset the com port or check to make sure the product is open,and then try again!"));
					if (pDlg !=NULL)
					{
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
					}
					return;
				}
			}
			if (g_CommunicationType==1)
			{
				nID=read_one(g_tstat_id,6);
				if(nID<0)
					bOnLine=FALSE;
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//

					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);
					UINT nSerialNumber=0;
					if(nRet>0)
					{

						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
				}else
				{
					AfxMessageBox(_T("Detect the product model not corresponding\nSelect COM port,try again!"));//\nDatabase->Building config Database			
					if (pDlg !=NULL)
					{
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
					}
					return;
				}
			}
			nFlag = read_one(g_tstat_id,7,6);		
			if(nFlag >65530)	//The return value is -1 -2 -3 -4
			{

				AfxMessageBox(_T("Reading product model abnormal \n Try again!"));
				if (pDlg !=NULL)
				{
					pDlg->ShowWindow(SW_HIDE);
					delete pDlg;
				}
				return;
			}
			if(bOnLine)
			{ 
				SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,true);
			}
			else
			{

				SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,false);	
				memset(&multi_register_value[0],0,sizeof(multi_register_value));
				//20120424				
				if (pDlg !=NULL)
				{
					pDlg->ShowWindow(SW_HIDE);
					delete pDlg;
				}

				return;
			}
			/*
			Comment by:Alex
			Date:2013/04/10
			Purpose:
			这个的目的的当是通过TCP连接的时候，把读到的寄存器读到
			multi_register_value_tcp
			适用于网络口，大数据量
			*/ 
			if (m_isCM5&&(product_Node.BuildingInfo.strProtocol.CompareNoCase(_T("Modbus TCP"))==0))
			{
				register_critical_section.Lock();
				int i;
				int it = 0;
				float progress;
				for(i=0;i<80;i++)
				{
					//register_critical_section.Lock();
					//int nStart = GetTickCount();
					int itemp = 0;
					itemp = Read_Multi(g_tstat_id,&multi_register_value_tcp[i*100],i*100,100,3);
					if(itemp == -2)
					{
						continue;
					}
					else						
					{
						if (pDlg!=NULL)
						{
							progress=float(it+1);
							pDlg->ShowProgress(int(progress/10),(int)progress);
						} 
					}							
					it++;
					Sleep(100);
				}
				//Add by Fance use this product_register_value to unite the register.
				//Fance_2
				//memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value_tcp,1024);

				if (it<80)
				{	
					AfxMessageBox(_T("Reading abnormal \n Try again!"));
					pDlg->ShowWindow(SW_HIDE);
					if(pDlg!=NULL)
					delete pDlg;
					pDlg=NULL;

				}
				else
				{
					pDlg->ShowProgress(100,100);
					pDlg->ShowWindow(SW_HIDE);
					if(pDlg!=NULL)
					delete pDlg;
					pDlg=NULL;
				}
				g_tstat_id_changed=FALSE;
				register_critical_section.Unlock();
			} 
			else
			{
				register_critical_section.Lock();
				int i;
				int it = 0;
				float progress;
				for(i=0;i<10;i++)	//暂定为0 ，因为TSTAT6 目前为600多
				{
					//register_critical_section.Lock();
					//int nStart = GetTickCount();
					int itemp = 0;
					itemp = Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64,3);
					if(itemp == -2)
					{
						continue;
					}
					else						
					{
						if (pDlg!=NULL)
						{
							progress=float((it+1)*(100/16));
							pDlg->ShowProgress(int(progress/10),int(progress));
						} 
					}							
					it++;
					Sleep(100);
				}
				//Add by Fance use this product_register_value to unite the register.
				//Fance_2
				memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));

				if (it<10)
				{	
					AfxMessageBox(_T("Reading abnormal \n Try again!"));
					pDlg->ShowWindow(SW_HIDE);
					if(pDlg!=NULL)
					delete pDlg;
					pDlg=NULL;

				}
				else
				{
					pDlg->ShowProgress(100,100);
					pDlg->ShowWindow(SW_HIDE);
					if(pDlg!=NULL)
					delete pDlg;
					pDlg=NULL;
				}
				g_tstat_id_changed=FALSE;
				register_critical_section.Unlock();
			}

			if (pDlg!=NULL)
			{
				pDlg->ShowProgress(100,100);
			}
			g_serialNum=nSerialNumber;//get_serialnumber();
#if 1
			if(g_bPrivilegeMannage)
			{
				if(g_buser_log_in)
				{
					if(g_strLoginUserName.CompareNoCase(_T("admin"))==0)
					{
						g_MainScreenLevel=0;
						g_ParamLevel=0;
						g_OutPutLevel=0;
						g_NetWorkLevel=0;
						g_GraphicModelevel=0;
						g_BurnHexLevel=0;
						g_LoadConfigLevel=0;
						g_BuildingsetLevel=0;
						g_AllscreensetLevel=0;
					}
					else
					{
						try
						{

							_ConnectionPtr m_pConTmp;
							_RecordsetPtr m_pRsTemp;
							m_pConTmp.CreateInstance("ADODB.Connection");
							m_pRsTemp.CreateInstance("ADODB.Recordset");
							m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

							CString strSerial;
							strSerial.Format(_T("%d"),g_serialNum);
							strSerial.Trim();
							CString strsql;
							strsql.Format(_T("select * from user_level where MainBuilding_Name='%s' and Building_Name='%s' and username = '%s' and serial_number = %d"),m_strCurMainBuildingName,m_strCurSubBuldingName,g_strLoginUserName,g_serialNum);
							m_pRsTemp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	

							if(VARIANT_FALSE==m_pRsTemp->EndOfFile)
							{//获取权限：
								g_AllscreensetLevel=m_pRsTemp->GetCollect("allscreen_level");
								if(g_AllscreensetLevel!=1)
								{
									g_MainScreenLevel=m_pRsTemp->GetCollect("mainscreen_level");//
									g_ParamLevel=m_pRsTemp->GetCollect("parameter_level");
									g_OutPutLevel=m_pRsTemp->GetCollect("outputtable_level");
									g_NetWorkLevel=m_pRsTemp->GetCollect("networkcontrol_level");//
									g_GraphicModelevel=m_pRsTemp->GetCollect("graphic_level");//
									g_BurnHexLevel=m_pRsTemp->GetCollect("burnhex_level");
									g_LoadConfigLevel=m_pRsTemp->GetCollect("loadconfig_level");
									g_BuildingsetLevel=m_pRsTemp->GetCollect("building_level");
								}
								else
								{
									g_MainScreenLevel=1;
									g_ParamLevel=1;
									g_OutPutLevel=1;
									g_NetWorkLevel=1;
									g_GraphicModelevel=1;
									g_BurnHexLevel=1;
									g_LoadConfigLevel=1;
									g_BuildingsetLevel=1;
									g_AllscreensetLevel=1;

								}



							}
							if(m_pRsTemp->State) 
								m_pRsTemp->Close(); 
							if(m_pConTmp->State)
								m_pConTmp->Close();	


							m_pConTmp.CreateInstance("ADODB.Connection");
							m_pRsTemp.CreateInstance("ADODB.Recordset");
							m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

							strSerial.Format(_T("%d"),g_serialNum);
							strSerial.Trim();

							strsql.Format(_T("select * from UserLevelSingleSet where MainBuilding_Name='%s' and Building_Name='%s' and username = '%s'"),m_strCurMainBuildingName,m_strCurSubBuldingName,g_strLoginUserName);
							m_pRsTemp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
							if(VARIANT_FALSE==m_pRsTemp->EndOfFile)
							{//获取权限：
								g_NetWorkLevel=m_pRsTemp->GetCollect("networkcontroller");//
								g_BuildingsetLevel=m_pRsTemp->GetCollect("database_limition");
							}

							if(m_pRsTemp->State) 
								m_pRsTemp->Close(); 
							if(m_pConTmp->State)
								m_pConTmp->Close();	



						}
						catch (...)
						{
						}
					}

				}

			}



			GetIONanme();
#endif
			// 			strInfo.Format(_T("CMainFrame::DoConnectToANode():read_one(g_tstat_id,7,3)"));			
			// 			SetPaneString(2, strInfo);
		//	SwitchToPruductType(10);


			if(nFlag==PM_NC)	
			{	
				SwitchToPruductType(1);
			}else if (nFlag == PM_CM5)//CM5
			{
				SwitchToPruductType(4);
			}else if (nFlag == PM_T38AIOD ||nFlag == PM_T3IOA  ||nFlag ==PM_T332AI || nFlag == PM_T34AO)//T3
			{
				memset(newtstat6,0,sizeof(newtstat6));
				SwitchToPruductType(5);

			}
			else if (nFlag == PM_MINIPANEL)
			{
				SwitchToPruductType(6);
			}
			else if (nFlag==PM_TSTAT6_HUM_Chamber)
			{

				SwitchToPruductType(9);
			}
			else if(nFlag == PM_CO2)
			{
				SwitchToPruductType(10);
			}
			else if(nFlag<PM_NC)	
			{	

				//Use product_type to change the address map
				//if product module is TSTAT6 or TSTAT7 use T3000_6_ADDRESS. T3000_6_ADDRESS is a register table in t3000.mdb  T3000_Register_Address_By_Name
				//else if product module is PM_TSTAT5E PM_TSTAT5H use T3000_5EH_LCD_ADDRESS
				//else if product module is TSTAT5 ABCDFG USE T3000_5ABCDFG_LED_ADDRESS
				//Fance_
				if((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7) )
				{
					product_type =T3000_6_ADDRESS;
				}
				else if((nFlag == PM_TSTAT5E) || (nFlag == PM_TSTAT5H))
				{
					product_type = T3000_5EH_LCD_ADDRESS;
				}
				else if((nFlag == PM_TSTAT5A) ||(nFlag == PM_TSTAT5B) ||
					(nFlag ==PM_TSTAT5C ) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F) ||
					(nFlag == PM_TSTAT5G))
				{
					product_type =T3000_5ABCDFG_LED_ADDRESS;
				}
				//Code and comments by Fance
				//Use old_product_type to save the last value.
				//if this value is change ,it will reload the register value.
				//example:
				//For tstats 5A : product_register_value[MODBUS_BAUDRATE]  ->  MODBUS_BAUDRATE = 185
				//For tstats 6	  product_register_value[MODBUS_BAUDRATE]  ->  MODBUS_BAUDRATE = 110
				if(old_product_type!=product_type)
				{
					old_product_type = product_type;
					MyRegAddress.Change_Register_Table();
				}
				//20120426
				if ((nFlag == PM_TSTAT6)||(nFlag == PM_TSTAT7))
				{
					//multi_register_value[]列表交换。
					//读取TXT
					if (FistWdb)
					{
						Updata_db_tstat6(nFlag);
						FistWdb = FALSE;
					}
					if ((nFlag == PM_TSTAT6&& flagsoftwareversion >35.5)||(nFlag == PM_TSTAT7))
					{

						memset(tempchange,0,sizeof(tempchange));
						memset(newtstat6,0,sizeof(newtstat6));
						memcpy(newtstat6,multi_register_value,sizeof(newtstat6));

						if (FlagSerialNumber== 0)
						{
							FlagSerialNumber = nSelectSerialNumber;
							int inum = 345;
							for (int i =0;i<6;i++)
							{
								tstat6flex[i]=newtstat6[inum]/10;
								inum++;
							}

							inum = 352;
							for (int i =6;i<10;i++)
							{
								tstat6flex[i]=newtstat6[inum]/10;
								inum++;
							}
							tstat6flex[10] =newtstat6[365];
							tstat6flex[11] =newtstat6[366];
						}
						else if (FlagSerialNumber!=nSelectSerialNumber)
						{
							int inum = 345;
							for (int i =0;i<6;i++)
							{
								tstat6flex[i]=newtstat6[inum];
								inum++;
							}

							inum = 352;
							for (int i =6;i<10;i++)
							{
								tstat6flex[i]=newtstat6[inum];
								inum++;
							}
							tstat6flex[10] =newtstat6[365];
							tstat6flex[11] =newtstat6[366];

							//	 365	1	Low byte	W/R	MAX_SETPOINT, Setpoint high, the highest setpoint a user will be able to set from the keypad.
							//	 366	1	Low byte	W/R	MIN_SETPOINT, Setpoint Low, the lowest setpoint a user will be able to set from the keypad. 

						}


						int index = 0;

						for (int i = 0;i<512;i++)
						{
							index = reg_tstat6[i];
							tempchange[index] = multi_register_value[i];
						}



						memcpy(multi_register_value,tempchange,sizeof(multi_register_value));
					}

				}
				else
				{
					memset(newtstat6,0,sizeof(newtstat6));
				}


				SwitchToPruductType(0);

				//20120426


				//memset(&multi_register_value[10],0,sizeof(multi_register_value)-10);
				//	memset(multi_register_value,0,sizeof(multi_register_value));



			}		

		}
	}

}


///////////////////////////////////////////////////////////////////////////
//  Added by zgq;2010-11-29;添加初始化TreeCtrl，希望在程序初始化完成后
//  TreeCtrl能够反应设备的连接状况。
//  方法：遍历TreeCtrl，分别去连接每一个Node，并根据状态修改其显示
void CMainFrame::InitTreeNodeConn()
{
	// find the currently selected item
	HTREEITEM hCurSel = m_pTreeViewCrl->GetNextItem(TVI_ROOT, TVGN_ROOT);

	// report it to the user
	if (hCurSel == NULL)
	{
		AfxMessageBox(_T("There is no selected item"));
	}
	else
	{
		CString str;
		str.Format(_T("The currently selected item is \"%s\""),(LPCTSTR)m_pTreeViewCrl->GetItemText(hCurSel));
		//AfxMessageBox((LPCTSTR)str);
	}
	
	vector<HTREEITEM> szTreeItems;
	szTreeItems.push_back(hCurSel);
	GetAllTreeItems(hCurSel, szTreeItems );

	for( UINT i = 0 ; i < szTreeItems.size(); i++)
	{
		HTREEITEM hItem = szTreeItems[i];
		DoConnectToANode(hItem);
	}
}


void CMainFrame::GetAllTreeItems( HTREEITEM hItem, vector<HTREEITEM>& szTreeItems  )
{
	HTREEITEM hTemp = m_pTreeViewCrl->GetNextItem(hItem, TVGN_NEXT);
	if (hTemp != NULL)
	{
		szTreeItems.push_back(hTemp);
		GetAllTreeItems(hTemp, szTreeItems);
	}
	
	{
		hTemp = m_pTreeViewCrl->GetNextItem(hItem, TVGN_CHILD);
		if (hTemp != NULL)
		{
			szTreeItems.push_back(hTemp);
			GetAllTreeItems(hTemp, szTreeItems);
		}	
	}	
}


//////////////////////////////////////////////////////////////////////////
// added by zgq;2010-12-1;从OnToolRefreshLeftTreee函数中抽取出来，
// 这个函数主要是用来更新TreeCtrl的。
void CMainFrame::RefreshTreeView()
{
	//TRACE("I'm start refreshing tree !!! \n");

	if(m_subNetLst.size()<=0)
		return;

	//BeginWaitCursor();
	// 对所有节点都检测

	for(UINT i=0;i<m_product.size();i++)
	{
		//tree0412if(!g_bEnableRefreshTreeView || g_bScanStart || g_bPauseMultiRead) 
		if( g_bScanStart || g_bPauseMultiRead)
			return;


#ifdef _DEBUG
		CString str;
		str.Format(_T("Fresh Tree at %d. \n"), i);
		TRACE(str);
		if(i == 8)
		{		
			int n = 0;
		}
#endif
		BOOL bOnLine=FALSE;
		UINT nSerialNumber=0;
		int nID;

		tree_product tp = m_product.at(i);
		if(m_strCurSubBuldingName.CompareNoCase(tp.BuildingInfo.strBuildingName)==0)
		{
			int nIDNode=tp.product_id;
			nSerialNumber=tp.serial_number;
			//int newnID=read_one(nID,6,2);

			if (g_CommunicationType==0) // 通信类型 0
			{	
				// force baud rate to 19200
				m_nbaudrat=19200;
				Change_BaudRate(19200);

				// read register offset 6
				int error = modbus_read_one_value( nID,nIDNode,6,2);

				/* 
				If an error was returned from the read,
				we previously attempted to try again with a reduced baud rate.
				However, a bug in the code meant this never was done correctly
				and problems were reported when the baud rate was changed.
				See discussion in ticket #14

				Now we simply set the online flag to FALSE and give up.
				*/

				if( error ) {
					bOnLine=FALSE;

				} else {

					// successful read of register offset 6

					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//

					// read first 4 registers 
					nRet=modbus_read_multi_value(&SerialNum[0],nID,0,4,2); 

					int nSerialNumberRead;

					if(nRet>=0)  // 计算串口号
					{
						nSerialNumberRead=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256; 
					}

					if(nSerialNumber>=0)  
					{
						if(nSerialNumberRead==nSerialNumber) // 取到串口号，并相等，说明online
							bOnLine=TRUE;
					}
				}
			}

			if (g_CommunicationType==1)  // 通信类型1，重复上面的过程
			{

				int error = modbus_read_one_value( nID, nIDNode, 6 ,2);
				if( error )
					bOnLine=FALSE;

				if( ! error )
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;

					// read first 4 registers
					nRet=modbus_read_multi_value(&SerialNum[0],nIDNode,0,4,2);

					int nSerialNumberRead;

					if(nRet>=0)
					{

						nSerialNumberRead=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumberRead==nSerialNumber)
							bOnLine=TRUE;
					}
				}
			}

			if(bOnLine>0)  // 如果online，更新显示图片
			{
				SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(tp.product_item ,true);

			}
			else  // 替换offline的图片
			{
				SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(tp.product_item ,false);

// 				if(nID==g_tstat_id)
// 					memset(&multi_register_value[0],0,sizeof(multi_register_value));
			}
		}
	}
	//EndWaitCursor();	
	//TRACE("Now End refreshing tree !!! \n");

}



void CMainFrame::SuspendRefreshThread()
{
	m_bEnableRefreshTreeView = FALSE;
}

void CMainFrame::ContinueRefreshThread()
{
	m_bEnableRefreshTreeView = TRUE;
}


void CMainFrame::DoFreshAll()
{		
	RefreshTreeView();
	if (m_nCurView == 0)
	{
		((CT3000View*)m_pViews[m_nCurView])->PostMessage(WM_FRESHVIEW,0,0)	;
	}
	
}


UINT _FreshTreeView(LPVOID pParam )
{
	Sleep(10000);
	CMainFrame* pMain = (CMainFrame*)pParam;
	while(1)
	{
		Sleep(30000);
		WaitForSingleObject(Read_Mutex,INFINITE);//Add by Fance .

		pMain->DoFreshAll();
		ReleaseMutex(Read_Mutex);//Add by Fance .
		/*ReleaseMutex(Read_Mutex);*/
		//pMain->RefreshTreeView();
		//if (pMain->m_pRefreshThread)
		//	pMain->m_pRefreshThread->PostThreadMessage(WM_DOREFRESH,NULL,NULL);
		
	}

	return 1;
}


LRESULT CMainFrame::OnAddTreeNode(WPARAM wParam, LPARAM lParam)
{
	ScanTstatInDB();

	//////////////////////////////////////////////////////////////////////////
	if(m_pScanner)
	{
		//////////////////////////////////////////////////////////////////////////
		CString strTemp;
		//strTemp.Format(_T("Scan finised,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
		strTemp.Format(_T("Scan finished,found %d node(s)"),m_pScanner->m_szTstatScandRet.size()+m_pScanner->m_szNCScanRet.size());
		SetPaneString(1,strTemp);
		g_bPauseMultiRead=FALSE;
		KillTimer(SCAN_TIMER);
		m_wndWorkSpace.m_TreeCtrl.Invalidate();		
		
		SelectTreeNodeFromRecord();//scan 解决scan完后，点击所扫到的项，显示com不对问题。

		//////////////////////////////////////////////////////////////////////////
		delete m_pScanner;
		m_pScanner =NULL;
	}
	g_bEnableRefreshTreeView =TRUE;
	g_bScanStart = FALSE;
	return 1;
}

CString CMainFrame::GetScreenName(int nSerialNumber, int nModbusID)
{
	CString strSerialNum, strMID;
	strSerialNum.Format(_T("%d"), nSerialNumber);
	strMID.Format(_T("%d"), nModbusID);

	CString strScreenName = _T("Screen(S:") + strSerialNum+_T("--")+strMID + _T(")");

	return strScreenName;
}



//////////////////////////////////////////////////////////////////////////
// for edit tree control node name

void CMainFrame::UpdateAllNodesInfo(HTREEITEM& htiEdit)
{
	int nItemData = m_pTreeViewCrl->GetItemData(htiEdit);

	if (nItemData >= 1000 && nItemData < 2000)
	{
		UpdateFloorNode(htiEdit);
	}
	if (nItemData >=2000 && nItemData < 3000)
	{
		UpdateRoomNode(htiEdit);
	}
	if (nItemData >= 3000 && nItemData < 9000)
	{
		UpdateDeviceNodes(htiEdit);
	}
		
}


void CMainFrame::UpdateFloorNode(HTREEITEM& htiEdit)
{
	try
	{

	_ConnectionPtr pCon;
	_RecordsetPtr  pRs;
	pCon.CreateInstance(_T("ADODB.Connection"));
	pRs.CreateInstance(_T("ADODB.Recordset"));
	pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strCurFloorName = m_pTreeViewCrl->GetItemText(htiEdit);
	if(strCurFloorName.GetLength() < 1)
	{
				m_pTreeViewCrl->SetItemText(htiEdit , m_strCurSelNodeName);
		strCurFloorName = m_strCurSelNodeName;
	}
	HTREEITEM htiParent = m_pTreeViewCrl->GetParentItem(htiEdit);
	CString strSubnetName = m_pTreeViewCrl->GetItemText(htiParent);



	CString strSql;
	strSql.Format(_T("update ALL_NODE set Floor_name = '%s' where Floor_name = '%s' and Building_Name = '%s'"), strCurFloorName, m_strCurSelNodeName, strSubnetName);
	pCon->Execute(strSql.GetString(),NULL,adCmdText);	

	if(pRs->State) 
		pRs->Close(); 
	if(pCon->State)
		pCon->Close();	
	}
	catch (...)
	{

	}

	
}


void CMainFrame::UpdateRoomNode(HTREEITEM& htiEdit)
{
	try
	{


	_ConnectionPtr pCon;
	_RecordsetPtr  pRs;
	pCon.CreateInstance(_T("ADODB.Connection"));
	pRs.CreateInstance(_T("ADODB.Recordset"));
	pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strCurRoomName = m_pTreeViewCrl->GetItemText(htiEdit);
	if(strCurRoomName.GetLength() < 1)
	{
				m_pTreeViewCrl->SetItemText(htiEdit , m_strCurSelNodeName);
		strCurRoomName = m_strCurSelNodeName;
	}
	HTREEITEM htiFloor= m_pTreeViewCrl->GetParentItem(htiEdit);
	CString strFloorName = m_pTreeViewCrl->GetItemText(htiFloor);
	HTREEITEM htiSubnet= m_pTreeViewCrl->GetParentItem(htiFloor);
	CString strSubnetName = m_pTreeViewCrl->GetItemText(htiSubnet);


	CString strSql;
	strSql.Format(_T("update ALL_NODE set Room_name = '%s' where Room_name = '%s' and Floor_name = '%s' and Building_Name = '%s'"), strCurRoomName, m_strCurSelNodeName, strFloorName, strSubnetName);
	pCon->Execute(strSql.GetString(),NULL,adCmdText);	

	if(pRs->State) 
		pRs->Close(); 
	if(pCon->State)
		pCon->Close();	


	}
	catch (...)
	{

	}
}

void CMainFrame::UpdateDeviceNodes(HTREEITEM& htiEdit)
{

	try
	{

	 _ConnectionPtr pCon;
	 _RecordsetPtr  pRs;
	 pCon.CreateInstance(_T("ADODB.Connection"));
	pRs.CreateInstance(_T("ADODB.Recordset"));
	pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strCurNodeName = m_pTreeViewCrl->GetItemText(htiEdit);
	if(strCurNodeName.GetLength() < 1)
	{
		m_pTreeViewCrl->SetItemText(htiEdit , m_strCurSelNodeName);
		strCurNodeName = m_strCurSelNodeName;
	}

	CString strSql;
	strSql.Format(_T("update ALL_NODE set Product_name = '%s' where Product_name = '%s'"), strCurNodeName, m_strCurSelNodeName);
	pCon->Execute(strSql.GetString(),NULL,adCmdText);	
	
	if(pRs->State) 
		pRs->Close(); 
	if(pCon->State)
		pCon->Close();	

	}
	catch (...)
	{

	}


}

//////////////////////////////////////////////////////////////////////////
// for record last time tree control node click result

// 

// write current click result to register table of windows.
const CString strRegRoot = _T("Software\\Temco T3000 Application\\T3000\\TreeNodeRecord\\");
		    
const CString strSubnetRegEntry = _T("Subnet");
const CString strSubnetRegEntryValid = _T("SubnetValid");
const CString strFloorRegEntry = _T("Floor");
const CString strFloorRegEntryValid = _T("FloorValid");
const CString strRoomRegEntry = _T("Room");
const CString strRoomRegEntryValid = _T("RoomValid");
const CString strDeviceRegEntry = _T("Device");
const CString strDeviceRegEntryValid = _T("DeviceValid");


void CMainFrame::SaveTreeNodeRecordToReg(HTREEITEM& htiCurSel)
{
	CRegKey reg;
	
	if(reg.Create(HKEY_CURRENT_USER, strRegRoot) == ERROR_SUCCESS)
	{
		HTREEITEM htiParent = m_pTreeViewCrl->GetParentItem(htiCurSel);
		WriteValueToReg(reg, htiCurSel);
		while(htiParent)
		{
			WriteValueToReg(reg, htiParent);
			htiParent = m_pTreeViewCrl->GetParentItem(htiParent);
		}
	}	
	else // 没有这个表项
	{
		ASSERT(0);		
	}	

	reg.Close();
}


// m_nSubnetItemData = 9000;
// m_nFloorItemData = 1000;
// m_nRoomItemData = 2000;
// m_nDeviceItemData = 3000;
BOOL CMainFrame::WriteValueToReg(CRegKey& reg, HTREEITEM& htiItem)
{
	if (!htiItem)
	{
		return FALSE;
	}
	int nData = m_pTreeViewCrl->GetItemData(htiItem);
	CString strText = m_pTreeViewCrl->GetItemText(htiItem);

	if (nData >= 1000 && nData < 2000) // floor
	{
		reg.SetStringValue(strFloorRegEntry, strText);
		reg.SetDWORDValue(strFloorRegEntryValid, 1);
	}
	else if(nData >= 2000 && nData < 3000) // room
	{
		reg.SetStringValue(strRoomRegEntry, strText);
		reg.SetDWORDValue(strRoomRegEntryValid, 1);
	}
	else if(nData >= 3000 && nData < 9000)	// device
	{
		reg.SetStringValue(strDeviceRegEntry, strText);
		reg.SetDWORDValue(strDeviceRegEntryValid, 1);
	}
	else if(nData >= 9000) // subnet
	{
		reg.SetStringValue(strSubnetRegEntry, strText);
		if(reg.SetDWORDValue(strSubnetRegEntryValid, 1) != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("Failed write Registry!"));
		}
	}

	return TRUE;
		
}


// nFlag = 0, read subnet
//         = 1, read floor
//         = 2, read room
//         = 3, read device
void CMainFrame::SelectTreeNodeFromRecord()
{
	CString strSubnetName, strFloorName, strRoomName, strDeviceName;
	//int nSubnet, nFloor, nRoom, nDevice;

	CRegKey reg;
	if(reg.Open(HKEY_CURRENT_USER, strRegRoot) == ERROR_SUCCESS)
	{
		HTREEITEM htiRoot = m_pTreeViewCrl->GetRootItem();
		HTREEITEM htiSel = GetLastSelNodeFromRecord(reg, htiRoot);
		m_pTreeViewCrl->SelectItem(htiSel);
	//	DoConnectToANode(htiSel);//scan 原来有这句的。

	}	
	else // 没有这个表项
	{
		m_pTreeViewCrl->SelectItem(NULL);
	}

	reg.Close();
}

// 
HTREEITEM CMainFrame::GetLastSelNodeFromRecord(CRegKey& reg, HTREEITEM& htiRoot)
{
	HTREEITEM htiSel = NULL;
	CString strSubnetName, strFloorName, strRoomName, strDeviceName;
	DWORD nSubnet, nFloor, nRoom, nDevice;

	DWORD dwSize=64;
	CString strRet;
	LPTSTR  szRet = new TCHAR[64];
	ZeroMemory(szRet, 64);

	//////////////////////////////////////////////////////////////////////////
	// 先获得所有的标志位 和 name
	// subnet
	if(reg.QueryDWORDValue(strSubnetRegEntryValid, nSubnet) == ERROR_SUCCESS)
	{
		if(reg.QueryStringValue(strSubnetRegEntry, szRet, &dwSize) == ERROR_SUCCESS)
		{
			strSubnetName = CString(szRet);
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		nSubnet =0;
	}

	// floor
	if(reg.QueryDWORDValue(strFloorRegEntryValid, nFloor) == ERROR_SUCCESS)
	{
		ZeroMemory(szRet, 64);dwSize=64;
		if(reg.QueryStringValue(strFloorRegEntry, szRet, &dwSize) == ERROR_SUCCESS)
		{
			strFloorName = CString(szRet);
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		nFloor =0;
	}

	// room
	if(reg.QueryDWORDValue(strRoomRegEntryValid, nRoom) == ERROR_SUCCESS)
	{
		ZeroMemory(szRet, 64);dwSize=64;
		if(reg.QueryStringValue(strRoomRegEntry, szRet, &dwSize) == ERROR_SUCCESS)
		{
			strRoomName = CString(szRet);
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		nRoom =0;
	}

	// device
	if(reg.QueryDWORDValue(strDeviceRegEntryValid, nDevice) == ERROR_SUCCESS)
	{
		ZeroMemory(szRet, 64);dwSize=64;
		if(reg.QueryStringValue(strDeviceRegEntry, szRet, &dwSize) == ERROR_SUCCESS)
		{
			strDeviceName = CString(szRet);
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		nDevice =0;
	}

	/////////////////////////////////////////////////////////////////////////
    // 查找节点
	if(nSubnet == 1)
	{
		htiSel = SearchItemByName(htiRoot, strSubnetName);
	}
	else
	{
		return NULL;
	}

	HTREEITEM htiTemp = NULL;
	// check floor
	if (htiSel && nFloor == 1 ) // go on to search
	{
		htiTemp = SearchItemByName(htiSel, strFloorName);		
	}
	
	// check room
	if (htiTemp)
	{
		htiSel = htiTemp;
		if(nRoom == 1)
		{
			htiTemp = SearchItemByName(htiSel, strRoomName);	
		}
	}

	// check device
	if (htiTemp)
	{
		htiSel = htiTemp;
		if(nDevice == 1)
		{
			htiTemp = SearchItemByName(htiSel, strDeviceName);	
			if (htiTemp)
			{
				htiSel = htiTemp;
			}
		}
	}


	delete [] szRet;
	//
	return htiSel;

}



//////////////////////////////////////////////////////////////////////////
// 根据树节点的name搜索一个节点并返回
// 如果有多个节点同名，那么返回第一个找到的
//
HTREEITEM CMainFrame::SearchItemByName(HTREEITEM& htiRoot, const CString& strItemName)
{
	CString strRoot = m_pTreeViewCrl->GetItemText(htiRoot);
	if (strItemName.CompareNoCase( strRoot ) == 0 )
	{
		return htiRoot;
	}


	HTREEITEM htiChild = m_pTreeViewCrl->GetChildItem(htiRoot);

	vector<HTREEITEM>  szTreeItem;

	if (htiChild)
	{

		CString strText = m_pTreeViewCrl->GetItemText(htiChild);
		if (strText.CompareNoCase(strItemName) == 0)
		{
			return htiChild;
		}

		szTreeItem.push_back(htiChild);
		HTREEITEM htiSibling = m_pTreeViewCrl->GetNextSiblingItem(htiChild);
		while( htiSibling )
		{
			CString strText = m_pTreeViewCrl->GetItemText(htiSibling);
			if (strText.CompareNoCase(strItemName) == 0)
			{
				return htiSibling;
			}
			szTreeItem.push_back(htiSibling);
			htiSibling = m_pTreeViewCrl->GetNextSiblingItem(htiSibling);
		}
	}

	for(UINT i = 0; i < szTreeItem.size(); i++)
	{
		HTREEITEM htiRet = SearchItemByName(szTreeItem[i], strItemName);

		if (htiRet)
		{
			return htiRet;
		}
	}

	return NULL;

}


// 为第一次运行导入数据库，如果不是第一次运行则不用提示
// 判断当前版本与注册表中版本是否一致，如一致则不用提示
// 如不一致，那么要提示，并把当前版本写入注册表
BOOL CMainFrame::ImportDataBaseForFirstRun()
{
	CString strFileVer = GetCurrentFileVersion();

	CRegKey reg;
	ReadFileVersionFromRegister(reg);
	
	if (strFileVer.CompareNoCase(m_strFileVersion) == 0) // equal
	{
		// do nothing
	}
	else
	{
		//CImportDatabaseDlg dlg;
		//dlg.DoModal();

		reg.SetStringValue(_T("FileVersion"), strFileVer);
	}	


	return TRUE;
}



const CString c_strFileVersionPath = _T("Software\\Temco T3000 Application\\T3000\\FileVersion\\");


// 从注册表读fileversion
CString CMainFrame::ReadFileVersionFromRegister(CRegKey& reg)
{
	WCHAR szBuf[64] = {0};
	DWORD dwSize = 64;

	if(reg.Create(HKEY_CURRENT_USER, c_strFileVersionPath) == ERROR_SUCCESS)
	{
		if(reg.QueryStringValue(_T("FileVersion"), szBuf, &dwSize) == ERROR_SUCCESS)
		{
			m_strFileVersion = CString(szBuf);
		}
	}	
	else // 没有这个表项
	{
		ASSERT(0);		
	}	

	return m_strFileVersion;
	reg.Close();
}


CString CMainFrame::GetCurrentFileVersion()
{

	WCHAR cPath[200];
	DWORD dwHandle,InfoSize;
	CString strVersion;
	::GetModuleFileName(NULL,cPath,sizeof(cPath)); //首先获得版本信息资源的长度+

	InfoSize = GetFileVersionInfoSize(cPath,&dwHandle); //将版本信息资源读入缓冲区
	if(InfoSize==0) return _T("None Version Supprot");
	char *InfoBuf = new char[InfoSize];
	GetFileVersionInfo(cPath,0,InfoSize,InfoBuf); //获得生成文件使用的代码页及文件版本
	unsigned int  cbTranslate = 0;
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
	VerQueryValue(InfoBuf, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);
	// Read the file description for each language and code page.
	for( UINT i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
	{
		WCHAR  SubBlock[200];
		wsprintf( SubBlock,
			TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
			lpTranslate[i].wLanguage,
			lpTranslate[i].wCodePage);
		void *lpBuffer=NULL;
		unsigned int dwBytes=0;
		VerQueryValue(InfoBuf,
			SubBlock,
			&lpBuffer,
			&dwBytes);
		//CString strTemp=(char *)lpBuffer;
		CString strTemp((WCHAR*)lpBuffer);
		strVersion=strTemp;

	}

	delete []InfoBuf;
	//char *InfoBuf = new char[InfoSize];
	return strVersion;
}


void CMainFrame::EnableMyToolBarButton(int nIndex, int nID, BOOL bEnable)
{
	return;
// 	if (bEnable)
// 	{
// 		m_wndToolBar.SetButtonInfo(nIndex,  nID,   TBBS_BUTTON,   0); 
// 
// 	}
// 	else
// 	{
// 		m_wndToolBar.SetButtonInfo(nIndex,   10000,   TBBS_BUTTON,   0); 
// 	}
	
// 	return;
// 	m_wndToolBar.SetGrayDisabledButtons(TRUE);
// 	//m_wndToolBar.AutoGrayInactiveImages();
// 
// 	DWORD dwID1 = ID_CONNECT2;
// 	DWORD dwID2 = ID_DISCONNECT2;
// 	DWORD dwID = m_wndToolBar.GetItemID(nIndex);
// 
// 	CMFCToolBarButton* pBtn =m_wndToolBar.GetButton(nIndex);
// 	
// 	m_wndToolBar.SetShowTooltips(TRUE);
// 	pBtn->EnableWindow(FALSE);
// 	
// 	pBtn->SetStyle(TBBS_DISABLED);
// 
// 
// 
// 	m_wndToolBar.UpdateButton(nIndex);
// 	//m_wndToolBar.EnableCustomizeButton(FALSE, ID_CONNECT2, _T("Connect"));
// 	//m_wndToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
} 
// 
// void CMainFrame::OnDatabaseTemcoproducts()
// {
// 
// 
// // 	CProInterface dlg;
// // 	dlg.ShowDialog();
// 
// 
// }

void CMainFrame::ReadExcel()
{
#if 0
	CString strPath;
	// 	m_Path.GetWindowText(strPath);


	_Application app;    
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;
	Range range;

	//
// 	if (!app.CreateDispatch(_T("Excel.Application"),NULL)) 
// 	{ 
// 		AfxMessageBox(_T("Create Excel false!")); 
// 		exit(1); 
// 	} 
	books.AttachDispatch(app.GetWorkbooks()); 
	book.AttachDispatch(books.Add(_variant_t(strPath)));
	//μ?μ?Worksheets 
	sheets.AttachDispatch(book.GetWorksheets());	
	sheet.AttachDispatch(sheets.GetItem(_variant_t("Sheet1")));
	//μ?μ?è?2?Cells 
	range.AttachDispatch(sheet.GetCells()); 
//	CString sText[]={_T("编号"),_T("姓名"),_T("所属部门")};//标题
//	int temp = m_Grid.GetItemCount()+1;

// 	for (int setnum=0;setnum<m_Grid.GetItemCount()+1;setnum++)
// 	{
// 		for (int num=0;num<3;num++)
// 		{
// 			if (!setnum)
// 			{
// 				range.SetItem(_variant_t((long)(setnum+1)),_variant_t((long)(num+1)),//在这里写入标题
// 					_variant_t(sText[num]));
// 			}
// 			else
// 			{
// 				range.SetItem(_variant_t((long)(setnum+1)),_variant_t((long)(num+1)),//在这里写入数据
// 					_variant_t(m_Grid.GetItemText(setnum-1,num)));
// 			}
// 		}
// 	}
	app.SetVisible(true); 
	range.ReleaseDispatch(); 
	sheet.ReleaseDispatch(); 
	sheets.ReleaseDispatch(); 
	book.ReleaseDispatch(); 
	books.ReleaseDispatch();
	app.ReleaseDispatch(); 

#endif
}

void CMainFrame::Tstat6_func()
{
	int num =0;
	for (int i =0;i<512;i++)
	{
		num = reg_tstat6[i];
		reg_tststold[num] = i;
	}

}

void CMainFrame::JudgeTstat6dbExist( CString strtable,CString strsql )
{
	CADO m_cado;     
	m_cado.OnInitADOConn();	
	bool ret = m_cado.IsHaveTable(m_cado,strtable);

	//不存在 创建数据库表
	if (!ret) 
	{	
		m_cado.m_pRecordset = m_cado.OpenRecordset(strsql);
		//初始化表
		//如果不初始化，则会有很多问题
		CString sql;
		sql.Format(_T("select * from %s"),strtable);
		m_cado.m_pRecordset = m_cado.OpenRecordset(sql);
		if (m_cado.m_pRecordset->EndOfFile)
		{

			for(int i = 0;i<512;i++)
			{
				try 
				{
					m_cado.m_pRecordset->AddNew();
				

					m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(charr[i][0]));
 					m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(i+1));
 				//	m_cado.m_pRecordset->PutCollect("Data",(_bstr_t)(i+1));
					
 					m_cado.m_pRecordset->PutCollect("Description",(_bstr_t)(charr[i][1]));
						                                                                                                                              
					

					m_cado.m_pRecordset->Update();

				}
			
				catch(...)
				{

				}
			}
		}




	}
	m_cado.CloseConn();

}

void CMainFrame::Updata_db_tstat6( unsigned short imodel )
{
	CString strtable;
	if (imodel == PM_TSTAT6)
		 strtable = _T("Tstat6");
	else if(imodel == PM_TSTAT7)
		strtable = _T("Tstat7");
	//CString strsql = _T("select * from Tstat6");
		//strtemp.Format(_T("update %s set name='%s' where num='%s'"),m_database,m_name,m_num);

	CADO m_cado;     
	m_cado.OnInitADOConn();	
//	bool ret = m_cado.IsHaveTable(m_cado,strtable);

	//不存在 创建数据库表
//	if (!ret) 
//	{	
	//	m_cado.m_pRecordset = m_cado.OpenRecordset(strsql);
		//初始化表
		//如果不初始化，则会有很多问题
		CString sql;
		sql.Format(_T("select * from %s"),strtable);
		m_cado.m_pRecordset = m_cado.OpenRecordset(sql);
	
	//	if (m_cado.m_pRecordset->EndOfFile)
		{

			for(int i = 0;i<512;i++)
			{
				try 
				{
//					m_cado.m_pRecordset->AddNew();


//					m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(charr[i][0]));
//					m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(i+1));
						m_cado.m_pRecordset->PutCollect("Data",(_bstr_t)(multi_register_value[i]));

//					m_cado.m_pRecordset->PutCollect("Description",(_bstr_t)(charr[i][1]));
					m_cado.m_pRecordset->MoveNext();


					m_cado.m_pRecordset->Update();

				}

				catch(...)
				{

				}
			}
		}
#if 0
		else if(m_cado.m_pRecordset->FirstOfFile)
		{
			for(int i = 0;i<512;i++)
 			{
				try 
				{
					//					m_cado.m_pRecordset->AddNew();


					//					m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(charr[i][0]));
					//					m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(i+1));
					m_cado.m_pRecordset->PutCollect("Data",(_bstr_t)(multi_register_value[i]));

					//					m_cado.m_pRecordset->PutCollect("Description",(_bstr_t)(charr[i][1]));


					m_cado.m_pRecordset->Update();

				}

				catch(...)
				{

				}

		}



	}
#endif
	m_cado.CloseConn();

}
// void CMainFrame::OnFileIsptool()
// {
// 	// TODO: Add your command handler code here
// 	CCDialogISPTOOL Disptool;
// 	Disptool.DoModal();
// 
// 
// }

void CMainFrame::JudgeTstat6SliderExist( CString strtable,CString strsql )
{

	CADO m_cado;     
	m_cado.OnInitADOConn();	
	bool ret = m_cado.IsHaveTable(m_cado,strtable);

	//不存在 创建数据库表
	if (!ret) 
	{	
		m_cado.m_pRecordset = m_cado.OpenRecordset(strsql);
		//初始化表
		//如果不初始化，则会有很多问题
		CString sql;
		sql.Format(_T("select * from %s"),strtable);
		m_cado.m_pRecordset = m_cado.OpenRecordset(sql);
		int m_flag =0;
		if (m_cado.m_pRecordset->EndOfFile)
		{

			for(int i = 0;i<12;i++)
			{
				try 
				{
					m_cado.m_pRecordset->AddNew();


					m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(m_flag));
					m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(m_slidertsta6[i]));
					m_cado.m_pRecordset->Update();
				}

				catch(...)
				{

				}
			}
		}




	}
	m_cado.CloseConn();
}

void CMainFrame::UpdataSlider( int Flag )
{
 	CString strtable;
// 	if (imodel == PM_TSTAT6)
// 		strtable = _T("Tstat6");
// 	else if(imodel == PM_TSTAT7)
// 		strtable = _T("Tstat7");
// 	//CString strsql = _T("select * from Tstat6");
// 	//strtemp.Format(_T("update %s set name='%s' where num='%s'"),m_database,m_name,m_num);

	strtable = _T("Sliderdb");
	CADO m_cado;     
	m_cado.OnInitADOConn();	
	//	bool ret = m_cado.IsHaveTable(m_cado,strtable);

	//不存在 创建数据库表
	//	if (!ret) 
	//	{	
	//	m_cado.m_pRecordset = m_cado.OpenRecordset(strsql);
	//初始化表
	//如果不初始化，则会有很多问题
	CString sql;
	sql.Format(_T("select * from %s"),strtable);
	m_cado.m_pRecordset = m_cado.OpenRecordset(sql);
	//if (m_cado.m_pRecordset->EndOfFile)
	{

		for(int i = 0;i<12;i++)
		{
			try 
			{
				m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(Flag));
				m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(tstat6flex[i]));
				m_cado.m_pRecordset->MoveNext();


				m_cado.m_pRecordset->Update();

			}

			catch(...)
			{

	
			}


// 			int tstat6flex[12]={
// 				30,
// 				20,
// 				40,
// 				20,
// 				40,
// 				30,
// 				40,
// 				20,
// 				40,
// 				20,
// 				50,
// 				15
// 			};


// 			for(int i = 6;i<10;i++)
// 			{
// 				try 
// 				{
// 					m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(Flag));
// 					m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(newtstat6[352+i]));
// 					m_cado.m_pRecordset->MoveNext();
// 
// 
// 					m_cado.m_pRecordset->Update();
// 
// 				}
// 
// 				catch(...)
// 				{
// 
// 				}
// 			}
		}
	}

	m_cado.CloseConn();

}

void CMainFrame::ReadSlider()
{
	CString strtable;
	strtable = _T("Sliderdb");
	CADO m_cado;     
	m_cado.OnInitADOConn();	
		CString sql;
	sql.Format(_T("select * from %s"),strtable);
	m_cado.m_pRecordset = m_cado.OpenRecordset(sql);
	{

		for(int i = 0;i<12;i++)
		{
			try 
			{
				m_cado.m_pRecordset->GetCollect("Block");
				m_slidertsta6[i] = m_cado.m_pRecordset->GetCollect("Address");
// 				m_cado.m_pRecordset->PutCollect("Block",(_bstr_t)(Flag));
// 				m_cado.m_pRecordset->PutCollect("Address",(_bstr_t)(newtstat6[345+i]));
				m_cado.m_pRecordset->MoveNext();


//				m_cado.m_pRecordset->Update();

			}

			catch(...)
			{

			}



		}
	}

	m_cado.CloseConn();


}

void CMainFrame::Treestatus()
{


}


DWORD WINAPI   CMainFrame::Get_All_Dlg_Message(LPVOID lpVoid)
{
	_MessageWriteOneInfo *My_Write_Struct=NULL;
	MSG msg;
	PeekMessage(&msg, NULL, MY_WRITE_ONE, MY_CLOSE, PM_REMOVE);
	while(true)
	{
		if(GetMessage(&msg,0,0,0)) //get msg from message queue
		{
			switch(msg.message)
			{
			case MY_WRITE_ONE:	
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();

				My_Write_Struct= (_MessageWriteOneInfo *)msg.wParam;
				product_register_value[My_Write_Struct->address] = My_Write_Struct->new_value;//先变过来，免得后台更新的时候 乱变。
				break;
			case  MY_READ_ONE:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				My_Write_Struct= (_MessageWriteOneInfo *)msg.wParam;
				break;
			case MY_CLOSE:
				goto myend;
				break;
			}
			
		}
	}
myend:
	return 0;
}


DWORD WINAPI  CMainFrame::Translate_My_Message(LPVOID lpVoid)
{
	MSG msg;
	while(1)
	{
		if(My_Receive_msg.size()>0)
		{
			MSG my_temp_msg = My_Receive_msg.at(0);
			switch(my_temp_msg.message)
			{
			case MY_WRITE_ONE:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageWriteOneInfo *My_Write_Struct = (_MessageWriteOneInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();

					if(write_one(My_Write_Struct->device_id, My_Write_Struct->address,My_Write_Struct->new_value,10)<0)
					{
						::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_FAIL,(LPARAM)My_Write_Struct);
					}
					else
					{
						::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_SUCCESS,(LPARAM)My_Write_Struct);
					}
				}
				break;
			case MY_READ_ONE:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageReadOneInfo *My_Read_Struct = (_MessageReadOneInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();
					int ret = read_one(My_Read_Struct->device_id, My_Read_Struct->address,10);
					if(ret<0)
					{
						//::PostMessage(My_Read_Struct->hwnd,MY_READ_DATA_CALLBACK,(WPARAM)WRITE_FAIL,(LPARAM)My_Read_Struct);
						if(My_Read_Struct!=NULL)
							delete	My_Read_Struct;
					}
					else
					{
						My_Read_Struct->new_value = ret;	//refresh the old value; post message to dlg deal the read data;
						::PostMessage(My_Read_Struct->hwnd,MY_READ_DATA_CALLBACK,(WPARAM)WRITE_SUCCESS,(LPARAM)My_Read_Struct);
					}


				}
				break;
			}
			
		}
		Sleep(10);
	}
	return 0;
}
