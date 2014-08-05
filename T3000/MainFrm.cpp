 
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "T3000.h"
#include "Scanwaydlg.h"
#include "MainFrm.h"
#include "T3000Doc.h"
#include "T3000TableView.h"
#include "T3000view.h"
#include "WorkspaceBar.h"
#include "NetworkControllView.h"
#include "T3000DefaultView.h"
#include "global_variable_extern.h"
#include "globle_function.h"
#include "AddBuilding.h"
#include "ScanSelectDlg.h"
#include "CalibrationDlg.h"
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
#include "MbpGlobals.h"
#include "Dialog_Progess.h"

#include "excel9.h"
#include "ScanSelectDlg.h"

#include "BacnetTool.h"
#include "BacnetAlarmWindow.h"
//////////////////////////////
#include "isp/CDialogISPTOOL.h"

#include "IONameConfig.h"

#include "DialogCM5_BacNet.h"
#include "TestMultiReadTraffic.h"
#include "T38I13O.h"
#include "T332AI.h"
#include "gloab_define.h"
 
#include "T38AI8AO.h"
 #include "rs485.h" // For call Get_RS485_Handle() function
#include "BacnetWait.h"
#include "BacnetScreenEdit.h"
#include "LanguageLocale.h"
#include "RegisterViewerDlg.h"
#include "DebugWindow.h"
#include "PVDlg.h"
#include "T36CT.h"
#include "T3RTDView.h"
#include "CO2NetView.h"
#include "htmlhelp.h"
#include "T3000UpdateDlg.h"
#include "Dowmloadfile.h"
extern tree_product	m_product_isp_auto_flash;
#pragma region Fance Test
//For Test
// 在没有鼠标和键盘消息的时候 就启用自动刷新 treeview,如果有就 不要刷新，因为如果正在刷新，客户肯能就无法第一时间读到自己想要的数据;

bool start_record_time = true;	//开启计时，如果用户一段时间无键盘和鼠标左键操作就开启自动刷新;
unsigned long time_click = 0;
CTestMultiReadTraffic *g_testmultiReadtraffic_dlg=NULL;
bool first_run_refresh_list_skip_wait = true; //第一次运行时不等待，直接检测状态;
bool enable_show_debug_window = false; 
BacnetWait *WaitWriteDlg=NULL;
HANDLE hwait_write_thread = NULL;
_Refresh_Write_Info Write_Config_Info;
HANDLE hStartEvent; // thread start event
extern int isp_product_id;
vector <MSG> My_Receive_msg;

#define  WM_REFRESH_TREEVIEW_MAP WM_USER + 2008
HANDLE hDeal_thread;
DWORD nThreadID_Do;
extern CBacnetScreenEdit * ScreenEdit_Window;
extern CBacnetAlarmWindow * AlarmWindow_Window;
extern bool m_is_remote_device ;
CCriticalSection MyCriticalSection;
CString SaveConfigFilePath;
bool b_pause_refresh_tree = false;
char local_network_ip[255];
CString local_enthernet_ip;
int m_MainHotKeyID[10] = 
{
	3001,
	3002,
	3003,
	3004,
	3005,
	3006,
	3007,
	3008,
	3009
};

//End for Test
void CALLBACK Listen(SOCKET s, int ServerPort, const char *ClientIP);
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
#define MONITOR_MOUSE_KEYBOARD_TIMER 3

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
	ON_MESSAGE(WM_REFRESH_TREEVIEW_MAP, RefreshTreeViewMap)
	ON_MESSAGE(WM_SCAN_PRODUCT, Message_Scan_Product)
	ON_MESSAGE(MY_BAC_CONFIG_READ_RESULTS, ReadConfigFromDeviceMessageCallBack)
	ON_MESSAGE(MY_RESUME_DATA, AllWriteMessageCallBack)
	ON_MESSAGE(MY_RX_TX_COUNT, Refresh_RX_TX_Count)
	ON_MESSAGE(WM_SHOW_PANNELINFOR,Show_Panel)
	ON_MESSAGE(WM_DELETE_NEW_MESSAGE_DLG,Delete_Write_New_Dlg)	
	ON_MESSAGE(WM_HOTKEY,&CMainFrame::OnHotKey)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	
	//ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, //OnToolbarReset) // 工具栏的按钮添加文字的方法
	ON_COMMAND(ID_BUILDINGBAR,OnCheckBuildingBar)
	ON_UPDATE_COMMAND_UI(ID_BUILDINGBAR, OnUpdateCheckBuildingBar)
	ON_COMMAND(ID_INPUTOUTPUTPANE,OnCheckIOPane)
	ON_UPDATE_COMMAND_UI(ID_INPUTOUTPUTPANE, OnUpdateCheckIOPane)

	ON_COMMAND(ID_FILE_SAVE_CONFIG, OnFileOpen)
	ON_COMMAND(ID_CONFIGFILE_SAVE_AS, SaveConfigFile)
	ON_COMMAND(ID_LOAD_CONFIG,OnLoadConfigFile)

	//ON_COMMAND(ID_TOOL_SCAN,OnScanDevice)
	
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
	ON_MESSAGE(WM_DLG_CLOSE, OnDlgClose)
	ON_COMMAND(ID_DATABASE_MBPOLL, &CMainFrame::OnDatabaseMbpoll)
	ON_MESSAGE(WM_MBPOLL_CLOSED, &CMainFrame::OnMbpollClosed)
	ON_COMMAND(ID_DATABASE_IONAMECONFIG, &CMainFrame::OnDatabaseIonameconfig)
	ON_COMMAND(ID_TOOL_ISPTOOLFORONE, &CMainFrame::OnToolIsptoolforone)
	 
//	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
	ON_COMMAND(ID_VIEW_COMMUNICATETRAFFIC, &CMainFrame::OnViewCommunicatetraffic)
	ON_COMMAND(ID_FUNCTION_HUMCALIBRATION, &CMainFrame::OnFunctionHumcalibration)
	ON_COMMAND(ID_CONTROL_INPUTS, &CMainFrame::OnControlInputs)
	ON_COMMAND(ID_CONTROL_PROGRAMS, &CMainFrame::OnControlPrograms)
	ON_COMMAND(ID_CONTROL_OUTPUTS, &CMainFrame::OnControlOutputs)
	ON_COMMAND(ID_CONTROL_VARIABLES, &CMainFrame::OnControlVariables)
	ON_COMMAND(ID_CONTROL_WEEKLY, &CMainFrame::OnControlWeekly)
	ON_COMMAND(ID_CONTROL_ANNUALROUTINES, &CMainFrame::OnControlAnnualroutines)
	ON_COMMAND(ID_MISCELLANEOUS_LOADDESCRIPTORS, &CMainFrame::OnMiscellaneousLoaddescriptors)
	ON_COMMAND(ID_MISCELLANEOUS_UPDATEMINI, &CMainFrame::OnMiscellaneousUpdatemini)
	ON_COMMAND(ID_CONTROL_CONTROLLERS, &CMainFrame::OnControlControllers)
	ON_COMMAND(ID_CONTROL_SCREENS, &CMainFrame::OnControlScreens)
	ON_COMMAND(ID_CONTROL_MONITORS, &CMainFrame::OnControlMonitors)
	ON_COMMAND(ID_HELP_GETLANGUAGECONFIGFILE, &CMainFrame::OnHelpGetlanguageconfigfile)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, &CMainFrame::OnLanguageEnglish)
	ON_COMMAND(ID_LANGUAGE_34010, &CMainFrame::OnLanguage34010)
	ON_COMMAND(ID_LANGUAGE_34006, &CMainFrame::OnLanguage34006)
	ON_COMMAND(ID_TOOL_REGISTERVIEWER, &CMainFrame::OnToolRegisterviewer)
	ON_WM_SIZING()
	ON_WM_PAINT()
	ON_COMMAND(ID_DATABASE_BACNETTOOL, &CMainFrame::OnDatabaseBacnettool)
	ON_COMMAND(ID_CONTROL_ALARM_LOG, &CMainFrame::OnControlAlarmLog)
		ON_COMMAND(ID_Menu_CHECKUPDATE, &CMainFrame::OnMenuCheckupdate)
	ON_COMMAND(ID_DATABASE_PV, &CMainFrame::OnDatabasePv)
	ON_COMMAND(ID_CONTROL_TSTAT, &CMainFrame::OnControlTstat)
	ON_COMMAND(ID_CALIBRATION_CALIBRATIONHUM, &CMainFrame::OnCalibrationCalibrationhum)

    ON_COMMAND(ID_CONNECT2,OnConnect)
    ON_UPDATE_COMMAND_UI(ID_CONNECT2, &CMainFrame::OnUpdateConnect2)
    ON_COMMAND(ID_DISCONNECT2,OnDisconnect)
    ON_UPDATE_COMMAND_UI(ID_DISCONNECT2, &CMainFrame::OnUpdateDisconnect2)
    ON_COMMAND(ID_SCAN,OnScanDevice)
    ON_UPDATE_COMMAND_UI(ID_SCAN, &CMainFrame::OnUpdateScanDevice)
    ON_COMMAND(ID_BUILDINGCONFIGDB,OnAddBuildingConfig)
    ON_UPDATE_COMMAND_UI(ID_BUILDINGCONFIGDB, &CMainFrame::OnUpdateAddBuildingConfig)
    
	ON_WM_COPYDATA()
	ON_COMMAND(ID_HELP_UPDATEFIRMWARE, &CMainFrame::OnHelpUpdatefirmware)


	ON_UPDATE_COMMAND_UI(IDS_SHOW_RESULTS, &CMainFrame::OnUpdateStatusBar)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILEPOS, &CMainFrame::OnUpdatePaneFilePos)


	ON_COMMAND(ID_HELP_FEEDBACKTOTEMCO, &CMainFrame::OnHelpFeedbacktotemco)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	//ID_SEPARATOR,           // status line indicator
	ID_RW_INFO,
	ID_BUILDING_INFO,
	ID_PROTOCOL_INFO,
	IDS_SHOW_RESULTS,
//  	ID_INDICATOR_CAPS,//老毛说不要
//  	ID_INDICATOR_NUM,
//  	ID_INDICATOR_SCRL,
};

unsigned short tempchange[512];

UINT _ReadMultiRegisters(LPVOID pParam)
{
	CMainFrame* pFrame=(CMainFrame*)(pParam);
	BOOL bFirst=TRUE;
	Read_Mutex=CreateMutex(NULL,TRUE,_T("Read_Multi_Reg"));	//Add by Fance 
	ReleaseMutex(Read_Mutex);//Add by Fance .
	Sleep(30*1000);
	
	//forbid  _ReadMultiRegisters  and _FreshTreeView access com port at the same time.
	
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
        {
            Sleep(10);
            continue;
        }
		if(g_tstat_id_changed)
        {
            Sleep(10);
            continue;
        }
		if(g_tstat_id<0||g_tstat_id>=255)
        {
            Sleep(10);
            continue;
        }

		//Make sure the view is TSTATE VIEW ,if not ,don't read this register.
		CView* pT3000View = pFrame->m_pViews[DLG_T3000_VIEW];
		if (!pT3000View)
        {
            Sleep(10);
            continue;
        }
		CView* pActiveView =pFrame->GetActiveView();
		if(pActiveView != pT3000View)
        {
            Sleep(10);
            continue;
        }

		if (!is_connect())
		{
            Sleep(10);
			continue;
		}

		WaitForSingleObject(Read_Mutex,INFINITE); 
		 
		int nRet =read_one(g_tstat_id,6,2);

		if(nRet<0)	
		{
			ReleaseMutex(Read_Mutex);//Add by Fance .
			continue;
		}
 
		for(int i=0;i<17;i++) //Modify by Fance , tstat 6 has more register than 512;
		 
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
			int multy_ret = 0;
			multy_ret = Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
			register_critical_section.Unlock();
			Sleep(100);
			if(multy_ret<0)		//Fance : 如果出现读失败 就跳出循环体,因为如果是由断开连接 造成的 读失败 会使其他需要用到读的地方一直无法获得资源;
				break;
		}

		ReleaseMutex(Read_Mutex);//Add by Fance .

		//Fance_1
		memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
		if(pFrame->m_pViews[DLG_T3000_VIEW]->m_hWnd!=NULL)
		{
			::PostMessage(pFrame->m_pViews[DLG_T3000_VIEW]->m_hWnd,MsgT3000ViewFresh,0,0);
		}

		//if(pFrame->m_pViews[DLG_CO2_VIEW]->m_hWnd!=NULL)
		//{
		//	::PostMessage(pFrame->m_pViews[DLG_CO2_VIEW]->m_hWnd,MsgT3000ViewFresh,0,0);
		//}
		
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
    m_nStyle=0;
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

	m_bDialogOpen = FALSE;
	mbPollDlgOpen = FALSE;

	mbPoll=NULL;
}


CMainFrame::~CMainFrame()
{
	if (m_pScanner != NULL)
	{
		delete m_pScanner;
		m_pScanner = NULL;
	}
	if (is_connect())
	{
		close_com(); // added by zgq:12-16-2011
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
	m_pViews[DLG_T3000_VIEW] = pActiveView;
	m_pViews[DLG_NETWORKCONTROL_VIEW]=(CView*) new CNetworkControllView();
	m_pViews[DLG_GRAPGIC_VIEW]=(CView*) new CGraphicView();
	m_pViews[DLG_TRENDLOG_VIEW]=(CView*) new CTrendLogView();
 	m_pViews[DLG_DIALOGCM5_VIEW]=(CView*) new CDialogCM5(); //CM5
 	m_pViews[DLG_DIALOGT3_VIEW]=(CView*) new CDialogT3();  //T3
	m_pViews[DLG_DIALOGMINIPANEL_VIEW]=(CView*) new CDialgMiniPanel();//Mini Panel 
 	m_pViews[DLG_AIRQUALITY_VIEW]=(CView*) new CAirQuality;//AirQuality
	m_pViews[DLG_LIGHTINGCONTROLLER_VIEW]=(CView*) new CLightingController;//Lightingcontroller
	m_pViews[DLG_HUMCHAMBER]=(CView*) new CHumChamber;
	m_pViews[DLG_CO2_VIEW]=(CView*) new CCO2_View;
	m_pViews[DLG_BACNET_VIEW]=(CView*) new CDialogCM5_BacNet; //CM5
	m_pViews[DLG_DIALOGT38I13O_VIEW]=(CView*) new T38I13O;
	m_pViews[DLG_DIALOGT332AI_VIEW]=(CView*)new T332AI;
	m_pViews[DLG_DIALOGT38AI8AO]=(CView*)new T38AI8AO;
    m_pViews[DLG_DIALOGT36CT]=(CView*)new T36CT;
    m_pViews[DLG_DIALOGT3PT10]=(CView*)new CT3RTDView;
    m_pViews[DLG_CO2_NET_VIEW]=(CView*)new CCO2NetView;
	m_pViews[DLG_DIALOGDEFAULTVIEW]=(CView*)new CT3000DefaultView;

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
void getLocalIp(void)
{
	WSADATA wsaData;
	char name[155];
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 )
	{
		if(gethostname(name,sizeof(name)) == 0)
		{
			struct hostent* phe = gethostbyname(name);
			if (0 == phe)
			{
				return;
			}
			for (int i = 0; phe->h_addr_list[i] != 0; ++i)
			{
				struct in_addr addr;
				memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
				TRACE("IP %d:%s\n",i+1,inet_ntoa(addr));
			}
		} 
		WSACleanup( );
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
    //theApp.m_bHiColorIcons=FALSE;
	UINT uiMenuID = theApp.m_bHiColorIcons ? IDB_MENUBAROWN_BITMAP : IDB_MENUBAROWN_BITMAP;// 

 
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_TOOLBAR_VER25049 : IDR_TOOLBAR_VER25049,uiToolbarColdID, uiMenuID, FALSE /* Not locked */, 0, 0, uiToolbarHotID))

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

	
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

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
	

// 	int last=theApp.GetLanguage();
// 	theApp.m_locale.SetLanguage(last);
// 	g_language=last;
// 	gSetMenuStrings(m_hWnd);
 
	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	//ID_RW_INFO,
	//ID_BUILDING_INFO,
	//ID_PROTOCOL_INFO,
	
	m_wndStatusBar.SetPaneInfo(0,ID_RW_INFO,SBPS_NOBORDERS,   300);
   // int index = m_wndStatusBar.CommandToIndex(ID_BUILDING_INFO);	
	m_wndStatusBar.SetPaneInfo(1,ID_BUILDING_INFO,SBPS_NOBORDERS, 300);
	m_wndStatusBar.SetPaneInfo(2,ID_PROTOCOL_INFO,SBPS_STRETCH | SBPS_NOBORDERS , 0);
	m_wndStatusBar.SetPaneInfo(3,IDS_SHOW_RESULTS,SBPS_NOBORDERS , 1000);
 	m_wndStatusBar.EnablePaneDoubleClick ();



	m_pTreeViewCrl=&m_wndWorkSpace.m_TreeCtrl;	
	m_pTreeViewCrl->SetExtendedStyle(TVS_EDITLABELS, TVS_EDITLABELS);
	MainFram_hwd = this->m_hWnd;

	//  2011,7,4, 先判断是否第一次运行，是否要导入数据库。
	//ImportDataBaseForFirstRun();

	ScanTstatInDB();
	DeleteConflictInDB();//用于处理数据库中重复的数据，这些数据有相同的序列号;
//20120420	SelectTreeNodeFromRecord();

	//////////////////////////////////////////////////////////////////////////
	// added by zgq;2010-11-30;for update the treectrl;
	//
	//InitTreeNodeConn();   //LSC
	SetTimer(MONITOR_MOUSE_KEYBOARD_TIMER,1000,NULL);
	SetTimer(REFRESH_TIMER, REFRESH_TIMER_VALUE, NULL);
#ifndef Fance_Enable_Test
	  m_pRefreshThread =(CRefreshTreeThread*) AfxBeginThread(RUNTIME_CLASS(CRefreshTreeThread));
	  m_pRefreshThread->SetMainWnd(this);	


	 
#endif
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


	//Register热键   
	#ifdef _DEBUG //debug版本   
	int nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[0],MOD_ALT,'S'); //热键 ctrl + S   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + S failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[1],MOD_ALT,'P'); //热键 ctrl + P  
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + P failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[2],MOD_ALT,'I'); //热键 ctrl + I   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + I failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[3],MOD_ALT,'O'); //热键 ctrl + O   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + O failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[4],MOD_ALT,'V'); //热键 ctrl + V  
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + V failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[5],MOD_ALT,'C'); //热键 ctrl + C   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + C failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[6],MOD_ALT,'W'); //热键 ctrl + W   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + W failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[7],MOD_ALT,'A'); //热键 ctrl + A   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + A failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[8],MOD_ALT,'G'); //热键 ctrl + G   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + G failure")); 
	nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[9],MOD_ALT,'L'); //热键 ctrl + L   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + L failure")); 
	nRet = RegisterHotKey(GetSafeHwnd(),1111,MOD_SHIFT,'D'); //热键 ctrl + L   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey MOD_SHIFT + D failure")); 
	nRet = RegisterHotKey(GetSafeHwnd(),1112,MOD_SHIFT,'F'); //热键 ctrl + L   
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey MOD_SHIFT + F failure")); 
	#else //release版本   
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[0],MOD_ALT,'S'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[1],MOD_ALT,'P'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[2],MOD_ALT,'I'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[3],MOD_ALT,'O'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[4],MOD_ALT,'V'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[5],MOD_ALT,'C'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[6],MOD_ALT,'W'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[7],MOD_ALT,'A'); 
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[8],MOD_ALT,'G');
	RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[9],MOD_ALT,'L');
	RegisterHotKey(GetSafeHwnd(),1111,MOD_SHIFT,'D');
	RegisterHotKey(GetSafeHwnd(),1112,MOD_SHIFT,'F');
	#endif  
	for(int i=0;i<10;i++)
	{
			pDialog[i]=NULL;
	}

	CString AutoFlashConfigPath;
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	AutoFlashConfigPath = ApplicationFolder + _T("//AutoFlashFile.ini");
	CFileFind fFind;
	if(fFind.FindFile(AutoFlashConfigPath))
	{
		DeleteFile(AutoFlashConfigPath);
	}

	MyRegAddress.MatchMoudleAddress();
	bac_net_initial_once = false;

#if 1
		//SOCKET soAck =::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		BOOL bBroadcast=TRUE;
		::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
		int iMode=1;
		ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

	BOOL bDontLinger = FALSE;
	setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
		//SOCKADDR_IN bcast;
		h_bcast.sin_family=AF_INET;
		//bcast.sin_addr.s_addr=nBroadCastIP;
		h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
		h_bcast.sin_port=htons(UDP_BROADCAST_PORT);


		//SOCKADDR_IN siBind;
#if 1
		local_enthernet_ip.Empty();
		GetHostAdaptersInfo(local_enthernet_ip);
		if(!local_enthernet_ip.IsEmpty())
		{
			
			WideCharToMultiByte( CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL );
			h_siBind.sin_family=AF_INET;
			h_siBind.sin_addr.s_addr =  inet_addr(local_network_ip);
			//h_siBind.sin_addr.s_addr=INADDR_ANY;
			//h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
			::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
		}
			
	//	getLocalIp();



#endif
#endif
		// 需要执行线程中的操作时
		m_pFreshMultiRegisters = AfxBeginThread(_ReadMultiRegisters,this);
		m_pFreshTree=AfxBeginThread(_FreshTreeView, this);
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
     
	g_bPauseRefreshTree = TRUE;
	Flexflash = TRUE;
	HTREEITEM hSelItem;//=m_pTreeViewCrl->GetSelectedItem();
 //   int nRet =read_one(g_tstat_id,6,1);

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
	g_bChamber=FALSE;
	m_isMiniPanel=FALSE;

	//m_pFreshMultiRegisters->SuspendThread();
	//m_pFreshTree->SuspendThread();
	g_bPauseMultiRead=TRUE;
#if 1
	for(UINT i=0;i<m_product.size();i++)
	{
		if(hSelItem==m_product.at(i).product_item )
		{	
			g_strT3000LogString.Format(_T("Trying to connect to %s:%d"),GetProductName(m_product.at(i).product_class_id),m_product.at(i).serial_number);
			SetPaneString(3,g_strT3000LogString);
			if(m_product.at(i).product_class_id == PM_CM5)
			{
				g_tstat_id=m_product.at(i).product_id;
				//SetPaneString(2,_T("Connect To CM5"));
				m_isCM5=TRUE;
				
				DoConnectToANode(hSelItem); 
				//SwitchToPruductType(4);			
			}
			else if (m_product.at(i).product_class_id == PM_MINIPANEL)
			{
				g_tstat_id = m_product.at(i).product_id;
				m_isMiniPanel=TRUE;
				DoConnectToANode(hSelItem); 
			}
            else if (m_product.at(i).product_class_id == PM_T3IOA)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                //SwitchToPruductType(DLG_DIALOGT38AI8AO);
            }

            else if (m_product.at(i).product_class_id == PM_T38I13O)
			{
				g_tstat_id = m_product.at(i).product_id;
				/*SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);*/
                DoConnectToANode(hSelItem);
			}
			else if (m_product.at(i).product_class_id == PM_T34AO) //T3
			{
				g_tstat_id = m_product.at(i).product_id;
				/*SwitchToPruductType(DLG_DIALOGT3_VIEW);*/
                DoConnectToANode(hSelItem);

			}
            else if (m_product.at(i).product_class_id == PM_T3PT10)
            {
                g_tstat_id = m_product.at(i).product_id;
               /* SwitchToPruductType(DLG_DIALOGT3PT10);*/
               DoConnectToANode(hSelItem);
            }
			else if (m_product.at(i).product_class_id==PM_T332AI)
			{
			    g_tstat_id = m_product.at(i).product_id;
			/*	SwitchToPruductType(DLG_DIALOGT332AI_VIEW);*/

                 DoConnectToANode(hSelItem);
			}
			else if (m_product.at(i).product_class_id ==PM_AirQuality) //AirQuality
			{
				g_tstat_id = m_product.at(i).product_id;
				SwitchToPruductType(DLG_AIRQUALITY_VIEW);
			}
			else if (m_product.at(i).product_class_id == PM_LightingController)//LightingController
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
				SwitchToPruductType(DLG_LIGHTINGCONTROLLER_VIEW);
			}
			else if (m_product.at(i).product_class_id==PM_TSTAT6_HUM_Chamber)
			{	
			
			g_bChamber=TRUE;
			g_tstat_id = m_product.at(i).product_id;

			SwitchToPruductType(DLG_HUMCHAMBER);
			}
			else if (m_product.at(i).product_class_id==PM_T38I13O)
			{
			g_tstat_id=m_product.at(i).product_id;
			DoConnectToANode(hSelItem); 
			 
			}
            else if (m_product.at(i).product_class_id==PM_T36CT)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem); 
                /*SwitchToPruductType(DLG_DIALOGT36CT);*/
            }
			else 
			{   
			    g_tstat_id = m_product.at(i).product_id;
				DoConnectToANode(hSelItem); 
			}
				
		}
	}
#endif
g_bPauseMultiRead=FALSE;
	//CM5
	//m_pFreshMultiRegisters->ResumeThread();
	//m_pFreshTree->ResumeThread();
	EndWaitCursor();
	g_bPauseRefreshTree = FALSE;
}

//void CMainFrame::OnHTreeItemKeyDownChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{	
//
////	Flexflash = TRUE;
////	HTREEITEM hSelItem,hNextItem;//=m_pTreeViewCrl->GetSelectedItem();
//////int nRet =read_one(g_tstat_id,6,1);
//////
////     hSelItem= m_pTreeViewCrl->GetSelectedItem();
//// 
////	//////////////////////////////////////////////////////////////////////////
////	
////	m_strCurSelNodeName = m_pTreeViewCrl->GetItemText(hSelItem);
//
//   /* CRenameDlg dlg;
//	dlg.m_nodename=m_strCurSelNodeName;
//	if (dlg.DoModal()==IDOK)
//	{
//	   m_strCurSelNodeName=dlg.m_nodename;
//	}*/
//	//m_pTreeViewCrl->SetItemText(hSelItem,m_strCurSelNodeName);
//	/* m_pTreeViewCrl->SelectItem(hSelItem);
//	m_pTreeViewCrl->SetFocus();
//	hNextItem=m_pTreeViewCrl->GetNextItem(hSelItem,TVGN_CARET);
//	m_pTreeViewCrl->SelectItem(hNextItem);
//	m_pTreeViewCrl->SetFocus(); */
//  //  AfxMessageBox(m_strCurSelNodeName);
//}

//void CMainFrame::Write_Config(){
//	 
//
//
//
//
//
//}

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

//删除数据库中 重叠的device.;
void CMainFrame::DeleteConflictInDB()
{
	bool find_conflict = false;
	int temp_item_count = (int)m_product.size();
	for (int i=0;i<temp_item_count;i++)
	{
		for (int j=i+1;j<temp_item_count;j++)
		{
			if(m_product.at(i).serial_number == m_product.at(j).serial_number)
			{
				find_conflict = true;
				m_pCon.CreateInstance(_T("ADODB.Connection"));
				m_pRs.CreateInstance(_T("ADODB.Recordset"));
				m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
				CString mtemp_serial_number;
				CString strSql;
				mtemp_serial_number.Format(_T("%d"),m_product.at(i).serial_number);
				try
				{
					strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s'"), mtemp_serial_number);
					m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
				}
				catch(_com_error *e)
				{
					//AfxMessageBox(e->ErrorMessage());
				}

				if(m_pRs->State)
					m_pRs->Close(); 
				if(m_pCon->State)
					m_pCon->Close();

				break;
			}
		}
	}
	if(find_conflict)
		 PostMessage(WM_MYMSG_REFRESHBUILDING,0,0);
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

	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
	CString strSql;
	//strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName);
	//strSql.Format(_T("select * from Building order by Main_BuildingName where Default_SubBuilding=-1"));
	strSql.Format(_T("select * from Building where Default_SubBuilding=-1"));
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	int count = m_pRs->GetRecordCount();
	_variant_t temp_variant;
	if(count<=0)
	{
		if(m_pRs->State)
		m_pRs->Close(); 
		if(m_pCon->State)
		m_pCon->Close();
	//	PostMessage(ID_NODEFAULTMAINBUILDING_MSG,0,0);
		AfxMessageBox(_T("There is no default building, please select a building Firstly!"));

		return;
	}
	
	m_strCurMainBuildingName=m_pRs->GetCollect("Main_BuildingName");
	if(m_pRs->State)
		m_pRs->Close();
	m_subNetLst.clear();
	strSql.Format(_T("select * from Building where Main_BuildingName ='%s'"),m_strCurMainBuildingName);
	m_pRs->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	


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
		// m_pTreeViewCrl->Expand(hTreeSubbuilding,TVE_EXPAND);//Add

		// Expand the parent, if possible.
		HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hTreeSubbuilding);
		if (hParent != NULL)
			m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);


		if (hTreeSubbuilding!=NULL)
		{
		//	m_subNetLst.at(k).hbuilding_item=hTreeSubbuilding;
		}
	//	m_pRs->Close();
		////begin floor nodes///////////////////////////////////////////////////////////////
		strSql.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '"+strBuilding+"'"));
		HRESULT hR=m_pRs->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
		
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
			
			HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hTreeFloor);
			if (hParent != NULL)
				m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);


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
				HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hTreeRoom);
				if (hParent != NULL)
					m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
				
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
				else if (temp_product_class_id == PM_T3PT10) //T3
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
				else if(temp_product_class_id == PM_TSTAT6||temp_product_class_id == PM_TSTAT5i)
					TVINSERV_TSTAT6
				else if((temp_product_class_id == PM_CO2_NET) || (temp_product_class_id == PM_CO2_RS485))
					TVINSERV_CO2
				else
				
					TVINSERV_TSTAT
#endif



			//	pBuildingInfo->hbuilding_item=							

				//HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertItem(&tvInsert);
				HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertDeviceItem(&tvInsert);
				
				HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hTreeRoom);
				if (hParent != NULL)
					m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);


				tree_product m_product_temp;
				m_product_temp.product_item  =hTreeRoom;
					
				//m_product_temp.serial_number = m_pRs->GetCollect("Serial_ID");
				temp_variant=m_pRs->GetCollect("Serial_ID");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.serial_number=_wtol(strSql);

				//m_product_temp.product_id =m_pRs->GetCollect("Product_ID");
				temp_variant=m_pRs->GetCollect("Product_ID");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.product_id=_wtoi(strSql);

			//	m_product_temp.product_class_id = m_pRs->GetCollect("Product_class_ID");
				temp_variant=m_pRs->GetCollect("Product_class_ID");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.product_class_id=_wtoi(strSql);

			//	m_product_temp.hardware_version= m_pRs->GetCollect("Hardware_Ver");
				temp_variant=m_pRs->GetCollect("Hardware_Ver");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.hardware_version=(float)_wtof(strSql);
				
				//
				temp_variant=m_pRs->GetCollect("Software_Ver");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.software_version=(float)_wtof(strSql);

				// 
				temp_variant=m_pRs->GetCollect("EPsize");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.nEPsize=(int)_wtol(strSql);

				//m_product_temp.baudrate=m_pRs->GetCollect("Bautrate");
				temp_variant=m_pRs->GetCollect("Protocol");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				m_product_temp.protocol = (int)_wtoi(strSql);
			
				temp_variant=m_pRs->GetCollect("Bautrate");//
				if(temp_variant.vt!=VT_NULL)
					strSql=temp_variant;
				else
					strSql=_T("");
				if(_wtoi(strSql) !=0)
					m_product_temp.baudrate = _wtoi(strSql);
				else
					m_product_temp.baudrate = 19200;
				if(m_product_temp.protocol == PROTOCOL_BACNET_IP)
				{
					char cTemp1[255];
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, strSql.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					DWORD dwIP = inet_addr(cTemp1); 

					m_product_temp.baudrate=dwIP;//_wtoi(strTemp);
				}
				else
					m_product_temp.baudrate=_wtoi(strSql);
				if (m_product_temp.product_class_id == PM_NC)
				{m_product_temp.baudrate = 19200;
				}
				////

				//m_product_temp.BuildingInfo.strMainBuildingname=m_pRs->GetCollect("MainBuilding_Name");
				//AfxMessageBox(m_product_temp.BuildingInfo.strMainBuildingname);
				//m_product_temp.BuildingInfo.strBuildingName=m_pRs->GetCollect("Building_Name");
				m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
//20120423	
				if(((strSql.CompareNoCase(_T("19200")) == 0)) || (strSql.CompareNoCase(_T("9600")) == 0))
				{
					m_product_temp.BuildingInfo.strIp.Empty();
					m_product_temp.BuildingInfo.strBaudRate = strSql;
				}
				else
				{
					m_product_temp.BuildingInfo.strIp = strSql;
				}
					
#if 0
				m_product_temp.BuildingInfo.strIp = strSql;
#endif
				temp_variant=m_pRs->GetCollect("Com_Port");//
				if(temp_variant.vt!=VT_NULL)
				{
					strSql=temp_variant;
					m_product_temp.ncomport = _wtoi(strSql);
				}
				else
				{
					strSql=_T("");
					m_product_temp.ncomport = 0;
				}
				
				//m_product_temp.BuildingInfo.strIpPort=strSql;
				m_product_temp.BuildingInfo.strIpPort = _T("10000");
			
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

					m_product_temp.status_last_time[0] = false;
					m_product_temp.status_last_time[1] = false;
					m_product_temp.status_last_time[2] = false;
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
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		MainFram_hwd = this->m_hWnd;
		LoadBacnetConfigFile();
		return;
	}
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

#include "Flash_Multy.h"
void CMainFrame::OnBatchFlashHex()
{

	b_pause_refresh_tree = true;
	bool temp_status = g_bPauseMultiRead;
	g_bPauseMultiRead = true;
	int temp_type = GetCommunicationType();


	BOOL bDontLinger = FALSE;
	setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
	closesocket(h_Broad);

	SetCommunicationType(0);//关闭串口，供ISP 使用;
	close_com();



	CFlash_Multy dlg;
	dlg.DoModal();

	if(temp_type == 0)
	{
		int comport = GetLastOpenedComport();
		open_com(comport);
	}
	else
	{

	}
	SetCommunicationType(temp_type);

	//Fance Add. 在ISP 用完1234 4321 的端口之后，T3000 在重新打开使用，刷新listview 的网络设备要使用;
	h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

	//SOCKADDR_IN bcast;
	h_bcast.sin_family=AF_INET;
	//bcast.sin_addr.s_addr=nBroadCastIP;
	h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
	h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

	//SOCKADDR_IN siBind;
	h_siBind.sin_family=AF_INET;
	h_siBind.sin_addr.s_addr=INADDR_ANY;
	h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
	::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
	b_pause_refresh_tree = false;
	g_bPauseMultiRead = temp_status;

	return;
#if 0
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
#endif
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
				CString strInfo;
				strInfo.Format(_T("Invalidate IP Address,You can click config button to reset it."));
				SetPaneString(1, strInfo);
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
					connectionSuccessful = 1;
                    m_nStyle=1;
					CString	g_configfile_path=g_strExePth+_T("config.ini");
					CFileFind fFind;
					if(!fFind.FindFile(g_configfile_path))
					{
						/*WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("0"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),_T("COM1"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),_T("1"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);*/

						WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),build_info.strIp,g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),build_info.strIpPort,g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
					}
					else
					{
						WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),build_info.strIp,g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),build_info.strIpPort,g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
					}



                    Invalidate();
				}
				else
				{
					CString strInfo;
					strInfo.Format((_T("Open IP:%s failed.")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
					connectionSuccessful = 0;
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
					connectionSuccessful = 0;
					//SetPaneCommunicationPrompt(strInfo);
					SetPaneString(1, strInfo);
				}
				else
				{
					//strInfo=_T("Open ")+build_info.strComPort+_T(" Sucessful!");
					strInfo.Format(_T("COM %d Connected: Yes"), nComPort);	
					connectionSuccessful = 1;
					//SetPaneCommunicationPrompt(strInfo);
					SetPaneString(1, strInfo);
					Change_BaudRate(default_com1_port_baudrate);
                    m_nStyle=1;
					CString	g_configfile_path=g_strExePth+_T("config.ini");
					CFileFind fFind;
					if(!fFind.FindFile(g_configfile_path))
					{
						WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("0"),g_configfile_path);
						strInfo.Format(_T("COM%d"),nComPort);
						WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
						strInfo.Format(_T("%d"),nComPort);
						WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);


					/*	WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),build_info.strIp,g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),build_info.strIpPort,g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);*/
					}
					else
					{
						WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("0"),g_configfile_path);
						strInfo.Format(_T("COM%d"),nComPort);
						WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
						strInfo.Format(_T("%d"),nComPort);
						WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path); 
					}

                    Invalidate();
				}	
			}
		}//endof rs485 checking
	}


	//EnableMyToolBarButton(0, ID_CONNECT2, FALSE);
	//EnableMyToolBarButton(1, ID_DISCONNECT2, TRUE);
}
void CMainFrame::OnDisconnect()
{
	//AfxMessageBox(_T("DisConnect."));
	close_com();
	m_nStyle=2;
    Invalidate();
	//CString strInfo = _T("No Connnection");
	connectionSuccessful = 0;
	//SetPaneString(1,strInfo);
	//strInfo = _T("Offline!");
	//SetPaneString(2,strInfo);


	//EnableMyToolBarButton(1, ID_DISCONNECT2, FALSE);
	//EnableMyToolBarButton(0, ID_CONNECT2, TRUE);

}

BOOL CMainFrame::ConnectDevice(LPCTSTR ip_address,int nport)
{
	CString connect_ip;
	if(m_hCurCommunication!=NULL)	
	{
		CloseHandle(m_hCurCommunication);
		m_hCurCommunication=NULL;
	}

	CString strPort;
	m_nIpPort=nport;

	connect_ip=ip_address;

	g_CommunicationType=1;
	SetCommunicationType(g_CommunicationType);
	bool b=Open_Socket2(connect_ip,m_nIpPort);
	CString strInfo;
	//	strInfo.Format((_T("Open IP:%s successful")),build_info.str3Ip);//prompt info;
	//	SetPaneString(1,strInfo);
	if(b)
	{	strInfo.Format((_T("Open IP:%s successful")),connect_ip);//prompt info;
	SetPaneString(1,strInfo);
	return TRUE;
	}
	else
	{
		strInfo.Format((_T("Open IP:%s failure")),connect_ip);//prompt info;
		SetPaneString(1,strInfo);
		//ValidOpenFailure(build_info.strIp,n1, n2,n3,n4); // 检查失败的原因，并给出详细的提示信息
		return FALSE;
	}
	return true;
}

BOOL CMainFrame::ConnectSubBuilding(Building_info build_info)
{
	CString strInfo;
	if(m_hCurCommunication!=NULL)	
	{
		CloseHandle(m_hCurCommunication);
		m_hCurCommunication=NULL;
	}
	if (!((build_info.strIp.CompareNoCase(_T("9600")) ==0)||(build_info.strIp.CompareNoCase(_T("19200"))==0) ||(build_info.strIp.CompareNoCase(_T(""))) == 0))
	{

	/*}
	if (build_info.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{*/
		g_strT3000LogString.Format(_T("Trying to connect to %s,please waiting..."),build_info.strIp);
		SetPaneString(3,g_strT3000LogString);
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

BOOL CMainFrame::ConnectDevice(tree_product tree_node)
{
	CString strInfo;
	if(m_hCurCommunication!=NULL)	
	{
		CloseHandle(m_hCurCommunication);
		m_hCurCommunication=NULL;
	}
	if (!((tree_node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)||(tree_node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0) ||(tree_node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
	{

	/*}
	if (build_info.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{*/
		UINT n1,n2,n3,n4;
		if (ValidAddress(tree_node.BuildingInfo.strIp,n1,n2,n3,n4)==FALSE)  // 验证NC的IP
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
				if(!GetIPbyHostName(tree_node.BuildingInfo.strIp,StringIP))
				{
					AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
					return false;
				}
				CString strPort;
				m_nIpPort=tree_node.ncomport;//_wtoi(tree_node.ncomport);
				g_CommunicationType=1;
				SetCommunicationType(g_CommunicationType);
				bool b=Open_Socket2(StringIP,m_nIpPort);
				CString strInfo;
				if(b)
				{	strInfo.Format((_T("Open IP:%s:%d successful")),tree_node.BuildingInfo.strIp,m_nIpPort);//prompt info;
					SetPaneString(1,strInfo);
					CString	g_configfile_path=g_strExePth+_T("config.ini");
					CFileFind fFind;
					if(!fFind.FindFile(g_configfile_path))
					{
						CString strInfo;
						strInfo.Format(_T("%d"),g_CommunicationType);
 						WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
//  						strInfo.Format(_T("COM%d"),nCom);
//  						WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
//  						strInfo.Format(_T("%d"),nCom);
//  						WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
//  						strInfo=_T("19200");
//  						WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);


						WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),StringIP,g_configfile_path);
						strInfo.Format(_T("%d"),m_nIpPort);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
					}
					else{
						CString strInfo;
						strInfo.Format(_T("%d"),g_CommunicationType);
						WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
// 						strInfo.Format(_T("COM%d"),nCom);
// 						WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
// 						strInfo.Format(_T("%d"),nCom);
// 						WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
// 						strInfo=_T("19200");
// 						WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),StringIP,g_configfile_path);
						strInfo.Format(_T("%d"),m_nIpPort);
						WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
						WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

						WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
					}

					return TRUE;
				}
				else
				{
					strInfo.Format((_T("Open IP:%s failure")),tree_node.BuildingInfo.strIp);//prompt info;
					SetPaneString(1,strInfo);
					return FALSE;
				}
		}
		else
		{
			CString strPort;
			m_nIpPort=tree_node.ncomport;//_wtoi(tree_node.BuildingInfo.strIpPort);

			m_strIP=tree_node.BuildingInfo.strIp;

			g_CommunicationType=1;
			SetCommunicationType(g_CommunicationType);
			bool b=Open_Socket2(tree_node.BuildingInfo.strIp,m_nIpPort);
			CString strInfo;
		//	strInfo.Format((_T("Open IP:%s successful")),build_info.str3Ip);//prompt info;
		//	SetPaneString(1,strInfo);
			if(b)
			{	strInfo.Format((_T("Open IP:%s:%d successful")),tree_node.BuildingInfo.strIp,m_nIpPort);//prompt info;
				SetPaneString(1,strInfo);
				CString	g_configfile_path=g_strExePth+_T("config.ini");
				CFileFind fFind;
				if(!fFind.FindFile(g_configfile_path))
				{
					WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),tree_node.BuildingInfo.strIp,g_configfile_path);
					strInfo.Format(_T("%d"),m_nIpPort);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
				}
				else
				{
					WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),tree_node.BuildingInfo.strIp,g_configfile_path);
					strInfo.Format(_T("%d"),m_nIpPort);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
				}
				return TRUE;
			}
			else
			{
				strInfo.Format((_T("Open IP:%s failure")),tree_node.BuildingInfo.strIp);//prompt info;
				SetPaneString(1,strInfo);
				//ValidOpenFailure(build_info.strIp,n1, n2,n3,n4); // 检查失败的原因，并给出详细的提示信息
				return FALSE;
			}
		}
	}	


	g_CommunicationType = 0;
	SetCommunicationType(g_CommunicationType);
	CString strComport = tree_node.BuildingInfo.strComPort;
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
	    CString	g_configfile_path=g_strExePth+_T("config.ini");
		CFileFind fFind;
		if(!fFind.FindFile(g_configfile_path))
		{
			CString strInfo;
			strInfo.Format(_T("%d"),g_CommunicationType);
			WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
			strInfo.Format(_T("COM%d"),nCom);
			WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
			strInfo.Format(_T("%d"),nCom);
			WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
			strInfo=_T("19200");
			WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);


 
		}
		else{
			CString strInfo;
			strInfo.Format(_T("%d"),g_CommunicationType);
			WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
			strInfo.Format(_T("COM%d"),nCom);
			WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
			strInfo.Format(_T("%d"),nCom);
			WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
			strInfo=_T("19200");
			WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);


 
		}


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
    m_nStyle=4;
    Invalidate();
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
    m_nStyle=3;
    Invalidate();
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
//				strTemp.Format(_T("Scanning now,please wait,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
// 				CString strTemp;
// 				strTemp.Format(_T("ID %d to %d"),g_nStartID,g_nEndID);
// 				CString strTemp2;
// 				strTemp2.Format(_T("Scanning %s, %s. Please wait."), g_strScanInfoPrompt, strTemp);
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

			g_strT3000LogString=strTip;
			::PostMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,3,0);
			if(!m_bScanALL)
				SetPaneString(1,strTip);
#if 0
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
#endif
		}
	}

	if(MONITOR_MOUSE_KEYBOARD_TIMER == nIDEvent)
	{
		unsigned long time_now = time(NULL);
		if(time_now - time_click > 5)
		{
			no_mouse_keyboard_event_enable_refresh = true;
		}
		CFrameWndEx::OnTimer(nIDEvent);
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

	g_protocol = PROTOCOL_UNKNOW;//USE FOR identify the bacnet ip protocol

here:
	g_bPauseMultiRead = FALSE;//恢复主线程的刷新
	switch(nIndex)
	{
	case DLG_T3000_VIEW:
		{
			m_nCurView=DLG_T3000_VIEW;
			((CT3000View*)m_pViews[m_nCurView])->FreshIOGridTable();
			((CT3000View*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case DLG_NETWORKCONTROL_VIEW:
		{
			m_nCurView=DLG_NETWORKCONTROL_VIEW;    
			((CNetworkControllView*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case DLG_GRAPGIC_VIEW:
		{
			m_nCurView=DLG_GRAPGIC_VIEW;

			int nSerialNum=get_serialnumber();
			((CGraphicView*)m_pViews[m_nCurView])->InitGraphic(nSerialNum,g_tstat_id);


		}
		break;
	case DLG_TRENDLOG_VIEW:
		{
			m_nCurView=DLG_TRENDLOG_VIEW;
		}
		break;
	case DLG_DIALOGCM5_VIEW:		//CM5
		{	
			m_nCurView=DLG_DIALOGCM5_VIEW; 
			((CDialogCM5*)m_pViews[m_nCurView])->Fresh();
		}	
		break;
	case DLG_DIALOGT3_VIEW:	   //T3
		{
			m_nCurView =DLG_DIALOGT3_VIEW;
			((CDialogT3*)m_pViews[m_nCurView])->Fresh();
		}
		break;

	case DLG_DIALOGMINIPANEL_VIEW://Mini Panel
		{
			m_nCurView = DLG_DIALOGMINIPANEL_VIEW;
		((CDialgMiniPanel*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case DLG_AIRQUALITY_VIEW://AirQuality
		{
			m_nCurView = DLG_AIRQUALITY_VIEW;
			((CAirQuality*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case DLG_LIGHTINGCONTROLLER_VIEW:
		{
			m_nCurView = DLG_LIGHTINGCONTROLLER_VIEW;
			((CLightingController*)m_pViews[m_nCurView])->Fresh();
		}
		break;
	case DLG_HUMCHAMBER:
	{
	     m_nCurView=DLG_HUMCHAMBER;
		((CHumChamber*)m_pViews[m_nCurView])->Fresh();
	}
	break;
    case  DLG_CO2_VIEW:
        {
            m_nCurView = DLG_CO2_VIEW;
            ((CCO2_View*)m_pViews[m_nCurView])->Fresh();
        }
        break;
    case  DLG_CO2_NET_VIEW:
        {
            m_nCurView = DLG_CO2_NET_VIEW;
            ((CCO2NetView*)m_pViews[m_nCurView])->Fresh();
        }
        break;

	case  DLG_BACNET_VIEW:
		{
			m_nCurView = DLG_BACNET_VIEW;
			((CDialogCM5_BacNet*)m_pViews[m_nCurView])->Fresh();
			g_protocol = PROTOCOL_BACNET_IP;
		}
		break;
	case DLG_DIALOGT38I13O_VIEW:
	{
	    m_nCurView=DLG_DIALOGT38I13O_VIEW;
		((T38I13O*)m_pViews[m_nCurView])->Fresh();
	}
		break;
	case DLG_DIALOGT332AI_VIEW:
	{
	   m_nCurView=DLG_DIALOGT332AI_VIEW;
	   ((T332AI*)m_pViews[m_nCurView])->Fresh();
	}	break;
	case DLG_DIALOGT38AI8AO :
		{
			m_nCurView=DLG_DIALOGT38AI8AO;
			((T38AI8AO*)m_pViews[m_nCurView])->Fresh();
		}	break;
     case  DLG_DIALOGT36CT:
     {
        m_nCurView=DLG_DIALOGT36CT;
       ((T36CT*)m_pViews[m_nCurView])->Fresh();
     }break;
     case DLG_DIALOGT3PT10:
     {
         m_nCurView=DLG_DIALOGT3PT10;
         ((CT3RTDView*)m_pViews[m_nCurView])->Fresh();
     }
     break;
	 case DLG_DIALOGDEFAULTVIEW:
		 {
			 m_nCurView=DLG_DIALOGDEFAULTVIEW;
			 ((CT3000DefaultView*)m_pViews[m_nCurView])->Fresh();
		 }
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
		

	SwitchToPruductType(DLG_GRAPGIC_VIEW);
}
void CMainFrame::OnAllNodeDatabase()
{
	bool temp_status = g_bPauseRefreshTree;
	g_bPauseRefreshTree = true;
	b_pause_refresh_tree = true;
	if(m_strCurMainBuildingName.IsEmpty())
		AfxMessageBox(_T("Please Select a Building First."));
	CAllNodesDiaolg Dlg;
	Dlg.SetBuildingMainName(m_strCurMainBuildingName);

	Dlg.DoModal();
	if(Dlg.m_bChanged)
	{
		ScanTstatInDB();
	}
	g_bPauseRefreshTree = temp_status;
	b_pause_refresh_tree = temp_status;
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
	CString strTime;
	strTime=Get_NowTime();
	//开始时间
	
	g_strT3000LogString=_T("--------------------------------Scan Begin--------------------------------\n");

	
	//WriteLogFile(g_strT3000LogString);
	//NET_WriteLogFile(g_strT3000LogString);
	 ::PostMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,3,0);
	g_strT3000LogString=_T("Scan begin Time: ");
	g_strT3000LogString+=strTime+_T("\n");;
	//WriteLogFile(g_strT3000LogString); 
	//NET_WriteLogFile(g_strT3000LogString);
	::PostMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,3,0);
	if(m_pRs->State)
	m_pRs->Close(); 
	if(m_pCon->State)
	m_pCon->Close();
	ClearBuilding();

	
	HANDLE temphandle;//如果用户点击Scan，而 bacnet的线程还在继续工作，需要先结束这两个线程;
	if(bac_net_initial_once)
	{
		temphandle = Get_RS485_Handle();
		if(temphandle !=NULL)
		{
			TerminateThread((HANDLE)Get_Thread1(),0);
			TerminateThread((HANDLE)Get_Thread2(),0);

			CloseHandle(temphandle);
			Set_RS485_Handle(NULL);
			bac_net_initial_once = false;
		}
	}
	//g_pauseMultiReadEvent.SetEvent();
	//Sleep(500);

	// oldscan
 	SetTimer(SCAN_TIMER,100,NULL);//lsc


//m_pThreadScan = AfxBeginThread(_ScanproductFun,this);
//* newscan


	g_bEnableRefreshTreeView = FALSE;
	g_bPauseRefreshTree = TRUE;
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
	if (m_pWaitScanDlg!=NULL)
	{
		delete m_pWaitScanDlg;
		m_pWaitScanDlg=NULL;
	}
	m_pWaitScanDlg = new CScanDbWaitDlg;	
	m_pWaitScanDlg->SetTStatScaner(m_pScanner);	
	m_pScanner->ScanAll();
	m_pScanner->m_scantype = 3;
	m_pWaitScanDlg->DoModal();	

	m_bScanALL = TRUE;
//	close_com();
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
	case 7:strClassName=g_strTstat7;break;
	case 6:strClassName=g_strTstat6;break;
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

void CMainFrame::OnMBP()
{
    if (m_bDialogOpen == TRUE) return;
    //m_pDlg->m_nColor = m_nClientColor;   // sets dialog's variables
    //m_pDlg->m_sTitle = m_sMainWindowTitle;
	m_bDialogOpen = TRUE;
    m_pDlg->Create();
}

LONG CMainFrame::OnDlgClose(UINT wParam, LONG lParam)
{
    m_bDialogOpen = FALSE;
    return 1;
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

void CMainFrame::Show_Wait_Dialog_And_SendConfigMessage()
{
	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{

		Write_Config_Info.Write_Input_Info[i].command = 0;
		Write_Config_Info.Write_Input_Info[i].device_id = 0;
		Write_Config_Info.Write_Input_Info[i].start_instance =0;
		Write_Config_Info.Write_Input_Info[i].end_instance =0;
		Write_Config_Info.Write_Input_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Input_Info[i].resend_count = 0;
		Write_Config_Info.Write_Input_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Input_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Input_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{

		Write_Config_Info.Write_Output_Info[i].command = 0;
		Write_Config_Info.Write_Output_Info[i].device_id = 0;
		Write_Config_Info.Write_Output_Info[i].start_instance =0;
		Write_Config_Info.Write_Output_Info[i].end_instance =0;
		Write_Config_Info.Write_Output_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Output_Info[i].resend_count = 0;
		Write_Config_Info.Write_Output_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Output_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Output_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{

		Write_Config_Info.Write_Variable_Info[i].command = 0;
		Write_Config_Info.Write_Variable_Info[i].device_id = 0;
		Write_Config_Info.Write_Variable_Info[i].start_instance =0;
		Write_Config_Info.Write_Variable_Info[i].end_instance =0;
		Write_Config_Info.Write_Variable_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Variable_Info[i].resend_count = 0;
		Write_Config_Info.Write_Variable_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Variable_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Variable_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{

		Write_Config_Info.Write_Program_Info[i].command = 0;
		Write_Config_Info.Write_Program_Info[i].device_id = 0;
		Write_Config_Info.Write_Program_Info[i].start_instance =0;
		Write_Config_Info.Write_Program_Info[i].end_instance =0;
		Write_Config_Info.Write_Program_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Program_Info[i].resend_count = 0;
		Write_Config_Info.Write_Program_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Program_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Program_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
	{

		Write_Config_Info.Write_Programcode_Info[i].command = 0;
		Write_Config_Info.Write_Programcode_Info[i].device_id = 0;
		Write_Config_Info.Write_Programcode_Info[i].start_instance =0;
		Write_Config_Info.Write_Programcode_Info[i].end_instance =0;
		Write_Config_Info.Write_Programcode_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Programcode_Info[i].resend_count = 0;
		Write_Config_Info.Write_Programcode_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Programcode_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Programcode_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
	{

		Write_Config_Info.Write_Controller_Info[i].command = 0;
		Write_Config_Info.Write_Controller_Info[i].device_id = 0;
		Write_Config_Info.Write_Controller_Info[i].start_instance =0;
		Write_Config_Info.Write_Controller_Info[i].end_instance =0;
		Write_Config_Info.Write_Controller_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Controller_Info[i].resend_count = 0;
		Write_Config_Info.Write_Controller_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Controller_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Controller_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_SCREEN_GROUP;i++)
	{

		Write_Config_Info.Write_Screen_Info[i].command = 0;
		Write_Config_Info.Write_Screen_Info[i].device_id = 0;
		Write_Config_Info.Write_Screen_Info[i].start_instance =0;
		Write_Config_Info.Write_Screen_Info[i].end_instance =0;
		Write_Config_Info.Write_Screen_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Screen_Info[i].resend_count = 0;
		Write_Config_Info.Write_Screen_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Screen_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Screen_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_WEEKLY_GROUP;i++)
	{

		Write_Config_Info.Write_Weekly_Info[i].command = 0;
		Write_Config_Info.Write_Weekly_Info[i].device_id = 0;
		Write_Config_Info.Write_Weekly_Info[i].start_instance =0;
		Write_Config_Info.Write_Weekly_Info[i].end_instance =0;
		Write_Config_Info.Write_Weekly_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Weekly_Info[i].resend_count = 0;
		Write_Config_Info.Write_Weekly_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Weekly_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Weekly_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_ANNUAL_GROUP;i++)
	{

		Write_Config_Info.Write_Annual_Info[i].command = 0;
		Write_Config_Info.Write_Annual_Info[i].device_id = 0;
		Write_Config_Info.Write_Annual_Info[i].start_instance =0;
		Write_Config_Info.Write_Annual_Info[i].end_instance =0;
		Write_Config_Info.Write_Annual_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Annual_Info[i].resend_count = 0;
		Write_Config_Info.Write_Annual_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Annual_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Annual_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_MONITOR_GROUP;i++)
	{

		Write_Config_Info.Write_Monitor_Info[i].command = 0;
		Write_Config_Info.Write_Monitor_Info[i].device_id = 0;
		Write_Config_Info.Write_Monitor_Info[i].start_instance =0;
		Write_Config_Info.Write_Monitor_Info[i].end_instance =0;
		Write_Config_Info.Write_Monitor_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Monitor_Info[i].resend_count = 0;
		Write_Config_Info.Write_Monitor_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Monitor_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Monitor_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
	{

		Write_Config_Info.Write_Weeklycode_Info[i].command = 0;
		Write_Config_Info.Write_Weeklycode_Info[i].device_id = 0;
		Write_Config_Info.Write_Weeklycode_Info[i].start_instance =0;
		Write_Config_Info.Write_Weeklycode_Info[i].end_instance =0;
		Write_Config_Info.Write_Weeklycode_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Weeklycode_Info[i].resend_count = 0;
		Write_Config_Info.Write_Weeklycode_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Weeklycode_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Weeklycode_Info[i].timeout_count = 0;
	}

	for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
	{

		Write_Config_Info.Write_Annualcode_Info[i].command = 0;
		Write_Config_Info.Write_Annualcode_Info[i].device_id = 0;
		Write_Config_Info.Write_Annualcode_Info[i].start_instance =0;
		Write_Config_Info.Write_Annualcode_Info[i].end_instance =0;
		Write_Config_Info.Write_Annualcode_Info[i].has_resend_yes_or_no = 0;
		Write_Config_Info.Write_Annualcode_Info[i].resend_count = 0;
		Write_Config_Info.Write_Annualcode_Info[i].task_result = BAC_RESULTS_UNKONW;
		Write_Config_Info.Write_Annualcode_Info[i].invoke_id = -1;
		//bac_input_read_results = 0;
		Write_Config_Info.Write_Annualcode_Info[i].timeout_count = 0;
	}

	WaitWriteDlg = NULL;
	if(WaitWriteDlg==NULL)
	{
		
		WaitWriteDlg = new BacnetWait(BAC_WAIT_READ_CONFIG_WRITE_DEVICE);
		WaitWriteDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
		WaitWriteDlg->ShowWindow(SW_SHOW);

		RECT RECT_SET1;
		if(BacNet_hwd)
		::GetWindowRect(BacNet_hwd,&RECT_SET1);
		else
		GetWindowRect(&RECT_SET1);
	

		ClientToScreen(&RECT_SET1);
		WaitWriteDlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 39,800,20);
	}

	//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
	if(hwait_write_thread==NULL)
	{
		hwait_write_thread =CreateThread(NULL,NULL,Send_Set_Config_Command_Thread,this,NULL, NULL);
	}
}

LRESULT CMainFrame::Delete_Write_New_Dlg(WPARAM wParam,LPARAM lParam)
{
	int message_type = wParam;
	switch(message_type)
	{
	case 0:
		if(WaitWriteDlg!=NULL)
		{
			Sleep(1000);
			delete WaitWriteDlg;
			WaitWriteDlg = NULL;

		}
		break;
	case 1:
		MessageBox(_T("Load config file failed , please try again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		break;
	}

	return 0;
}



DWORD WINAPI  CMainFrame::Send_Set_Config_Command_Thread(LPVOID lpVoid)
{
	//Write_Config_Info
	CMainFrame *pParent = (CMainFrame *)lpVoid;
	



	int resend_count;
	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto mywriteend;
				g_invoke_id =WritePrivateData(g_bac_instance,WRITEINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
				//g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Write_Config_Info.Write_Input_Info[i].command = WRITEINPUT_T3000;
			Write_Config_Info.Write_Input_Info[i].device_id = g_bac_instance;
			Write_Config_Info.Write_Input_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Write_Config_Info.Write_Input_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Write_Config_Info.Write_Input_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Write Input List Item From %d to %d "),g_invoke_id,
				Write_Config_Info.Write_Input_Info[i].start_instance,
				Write_Config_Info.Write_Input_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}


	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEOUTPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			//g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Output_Info[i].command = WRITEOUTPUT_T3000;
		Write_Config_Info.Write_Output_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Output_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Output_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Output_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Output List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Output_Info[i].start_instance,
			Write_Config_Info.Write_Output_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEVARIABLE_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			//g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Variable_Info[i].command = WRITEVARIABLE_T3000;
		Write_Config_Info.Write_Variable_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Variable_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Variable_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Variable_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Variable List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Variable_Info[i].start_instance,
			Write_Config_Info.Write_Variable_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEPROGRAM_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			//g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Program_Info[i].command = WRITEPROGRAM_T3000;
		Write_Config_Info.Write_Program_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Program_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Program_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Program_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Program List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Program_Info[i].start_instance,
			Write_Config_Info.Write_Program_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}



	for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEPROGRAMCODE_T3000,i,i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Programcode_Info[i].command = WRITEPROGRAMCODE_T3000;
		Write_Config_Info.Write_Programcode_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Programcode_Info[i].start_instance = i;
		Write_Config_Info.Write_Programcode_Info[i].end_instance =i;
		Write_Config_Info.Write_Programcode_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Program code List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Programcode_Info[i].start_instance,
			Write_Config_Info.Write_Programcode_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}


	for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITETIMESCHEDULE_T3000,i,i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Weeklycode_Info[i].command = WRITETIMESCHEDULE_T3000;
		Write_Config_Info.Write_Weeklycode_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Weeklycode_Info[i].start_instance = i;
		Write_Config_Info.Write_Weeklycode_Info[i].end_instance =i;
		Write_Config_Info.Write_Weeklycode_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write weekly schedule time From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Weeklycode_Info[i].start_instance,
			Write_Config_Info.Write_Weeklycode_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEANNUALSCHEDULE_T3000,i,i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Annualcode_Info[i].command = WRITEANNUALSCHEDULE_T3000;
		Write_Config_Info.Write_Annualcode_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Annualcode_Info[i].start_instance = i;
		Write_Config_Info.Write_Annualcode_Info[i].end_instance =i;
		Write_Config_Info.Write_Annualcode_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write ANNUAL DAY time From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Annualcode_Info[i].start_instance,
			Write_Config_Info.Write_Annualcode_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITECONTROLLER_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			//g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Controller_Info[i].command = WRITECONTROLLER_T3000;
		Write_Config_Info.Write_Controller_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Controller_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Controller_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Controller_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Controller List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Controller_Info[i].start_instance,
			Write_Config_Info.Write_Controller_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_SCREEN_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITESCREEN_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Screen_Info[i].command = WRITESCREEN_T3000;
		Write_Config_Info.Write_Screen_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Screen_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Screen_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Screen_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Screen List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Screen_Info[i].start_instance,
			Write_Config_Info.Write_Screen_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_ANNUAL_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEANNUALROUTINE_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Annual_Info[i].command = WRITEANNUALROUTINE_T3000;
		Write_Config_Info.Write_Annual_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Annual_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Annual_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Annual_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Annual List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Annual_Info[i].start_instance,
			Write_Config_Info.Write_Annual_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}

	for (int i=0;i<BAC_WEEKLY_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEWEEKLYROUTINE_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Weekly_Info[i].command = WRITEWEEKLYROUTINE_T3000;
		Write_Config_Info.Write_Weekly_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Weekly_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Weekly_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Weekly_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Weekly List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Weekly_Info[i].start_instance,
			Write_Config_Info.Write_Weekly_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}


	for (int i=0;i<BAC_MONITOR_GROUP;i++)
	{
		resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto mywriteend;
			g_invoke_id =WritePrivateData(g_bac_instance,WRITEMONITOR_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Write_Config_Info.Write_Monitor_Info[i].command = WRITEWEEKLYROUTINE_T3000;
		Write_Config_Info.Write_Monitor_Info[i].device_id = g_bac_instance;
		Write_Config_Info.Write_Monitor_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Monitor_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
		Write_Config_Info.Write_Monitor_Info[i].invoke_id = g_invoke_id;


		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Write Monitor List Item From %d to %d "),g_invoke_id,
			Write_Config_Info.Write_Monitor_Info[i].start_instance,
			Write_Config_Info.Write_Monitor_Info[i].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs);

	}



	hwait_write_thread = NULL;
	return 0;
mywriteend:

	AfxMessageBox(_T("Send Command Timeout!Please try it again!"));
	::PostMessage(MainFram_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
	hwait_write_thread = NULL;
	return 0;
}

LRESULT  CMainFrame::ReadConfigFromDeviceMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		SaveBacnetConfigFile();
	}
	return 0;
}
LRESULT CMainFrame::Refresh_RX_TX_Count(WPARAM wParam, LPARAM lParam)
{
	int ret = (int)wParam;
	if(ret == 1)
	{
	Set_Communication_Count(1,g_bac_instance);//成功，计数+1

	m_pTreeViewCrl->turn_item_image(selected_tree_item ,true);
	//SetPaneConnectionPrompt(_T("Online"));
	}
	else
	Set_Communication_Count(0,g_bac_instance);

	return 0;
}
LRESULT CMainFrame::Show_Panel(WPARAM wParam, LPARAM lParam){
	int ret = (int)wParam; 
	SetPaneString(ret,g_strT3000LogString);
	return 0;
}
LRESULT  CMainFrame::AllWriteMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
	
		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Input_Info[i].invoke_id)
				Write_Config_Info.Write_Input_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Output_Info[i].invoke_id)
				Write_Config_Info.Write_Output_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Variable_Info[i].invoke_id)
				Write_Config_Info.Write_Variable_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Program_Info[i].invoke_id)
				Write_Config_Info.Write_Program_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Programcode_Info[i].invoke_id)
				Write_Config_Info.Write_Programcode_Info[i].task_result = true;
		}


		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Controller_Info[i].invoke_id)
				Write_Config_Info.Write_Controller_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Screen_Info[i].invoke_id)
				Write_Config_Info.Write_Screen_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weekly_Info[i].invoke_id)
				Write_Config_Info.Write_Weekly_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weeklycode_Info[i].invoke_id)
				Write_Config_Info.Write_Weeklycode_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annual_Info[i].invoke_id)
				Write_Config_Info.Write_Annual_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annualcode_Info[i].invoke_id)
				Write_Config_Info.Write_Annualcode_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Monitor_Info[i].invoke_id)
				Write_Config_Info.Write_Monitor_Info[i].task_result = true;
		}



		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		TRACE(Show_Results + _T("\r\n"));
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Input_Info[i].invoke_id)
				Write_Config_Info.Write_Input_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Output_Info[i].invoke_id)
				Write_Config_Info.Write_Output_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Variable_Info[i].invoke_id)
				Write_Config_Info.Write_Variable_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Program_Info[i].invoke_id)
				Write_Config_Info.Write_Program_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Programcode_Info[i].invoke_id)
				Write_Config_Info.Write_Programcode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Controller_Info[i].invoke_id)
				Write_Config_Info.Write_Controller_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Screen_Info[i].invoke_id)
				Write_Config_Info.Write_Screen_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weekly_Info[i].invoke_id)
				Write_Config_Info.Write_Weekly_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weeklycode_Info[i].invoke_id)
				Write_Config_Info.Write_Weeklycode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annual_Info[i].invoke_id)
				Write_Config_Info.Write_Annual_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annualcode_Info[i].invoke_id)
				Write_Config_Info.Write_Annualcode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Write_Config_Info.Write_Monitor_Info[i].invoke_id)
				Write_Config_Info.Write_Monitor_Info[i].task_result = false;
		}

		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		TRACE(Show_Results + _T("\r\n"));
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}

	if(pInvoke)
		delete pInvoke;
	return 0;
}

void CMainFrame::LoadBacnetConfigFile()
{
	if((g_mac!=0) &&(g_bac_instance!=0))
	{
		CFileDialog dlg(true,_T("*.prg"),_T(" "),OFN_HIDEREADONLY ,_T("Prg files (*.prg)|*.prg||"),NULL,0);
		if(IDOK==dlg.DoModal())
		{
			CString FilePath;
			FilePath=dlg.GetPathName();
#if 1
			CFile myfile(FilePath,CFile::modeRead);
			char *pBuf;
			DWORD dwFileLen;
			dwFileLen=myfile.GetLength();
			pBuf= new char[dwFileLen+1];
			pBuf[dwFileLen]=0;
			myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
			myfile.Close();
			//MessageBox(pBuf);
			char * temp_buffer = pBuf;
			for (int i=0;i<dwFileLen;i++)
			{
				*temp_buffer = *temp_buffer ^ 1;
				temp_buffer ++;
			}

			CString new_file;
			new_file = FilePath.Left(FilePath.GetLength()-3) + _T("ini");

			HANDLE hFile;
			hFile=CreateFile(new_file,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
			DWORD dWrites;
			WriteFile(hFile,pBuf,dwFileLen,&dWrites,NULL);
			CloseHandle(hFile);  
			if(pBuf)
				delete pBuf;


			FilePath = new_file;
#endif
//			CString FilePath;
	//		FilePath=dlg.GetPathName();
			for (int i=0;i<BAC_INPUT_ITEM_COUNT;i++)
			{
				CString temp_input,temp_des,temp_csc;
				temp_input.Format(_T("Input%d"),i);

				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_input,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Input_data.at(i).description,STR_IN_DESCRIPTION_LENGTH,cTemp1,STR_IN_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_input,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Input_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);

				m_Input_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_input,_T("Auto_Manual"),0,FilePath);
				m_Input_data.at(i).value = GetPrivateProfileInt(temp_input,_T("Value"),0,FilePath);

				m_Input_data.at(i).filter = (unsigned char)GetPrivateProfileInt(temp_input,_T("Filter"),0,FilePath);
				m_Input_data.at(i).decom = (unsigned char)GetPrivateProfileInt(temp_input,_T("Decom"),0,FilePath);
				m_Input_data.at(i).sen_on = (unsigned char)GetPrivateProfileInt(temp_input,_T("Sen_On"),0,FilePath);

				m_Input_data.at(i).sen_off = (unsigned char)GetPrivateProfileInt(temp_input,_T("Sen_Off"),0,FilePath);
				m_Input_data.at(i).control = (unsigned char)GetPrivateProfileInt(temp_input,_T("Control"),0,FilePath);
				m_Input_data.at(i).digital_analog = (unsigned char)GetPrivateProfileInt(temp_input,_T("Digital_Analog"),0,FilePath);

				m_Input_data.at(i).calibration_sign = (unsigned char)GetPrivateProfileInt(temp_input,_T("Calibration_Sign"),0,FilePath);
				m_Input_data.at(i).calibration_increment = (unsigned char)GetPrivateProfileInt(temp_input,_T("Calibration_Increment"),0,FilePath);
				m_Input_data.at(i).unused = (unsigned char)GetPrivateProfileInt(temp_input,_T("Unused"),0,FilePath);

				m_Input_data.at(i).calibration = (unsigned char)GetPrivateProfileInt(temp_input,_T("Calibration"),0,FilePath);
				m_Input_data.at(i).range = (unsigned char)GetPrivateProfileInt(temp_input,_T("Range"),0,FilePath);

			}

			for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Output%d"),i);
				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH,cTemp1,STR_OUT_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Output_data.at(i).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);

				m_Output_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
				m_Output_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);

				m_Output_data.at(i).digital_analog = (unsigned char)GetPrivateProfileInt(temp_section,_T("Digital_Analog"),0,FilePath);
				m_Output_data.at(i).hw_switch_status = (unsigned char)GetPrivateProfileInt(temp_section,_T("hw_switch_status"),0,FilePath);
				m_Output_data.at(i).control = (unsigned char)GetPrivateProfileInt(temp_section,_T("Control"),0,FilePath);
				m_Output_data.at(i).digital_control = (unsigned char)GetPrivateProfileInt(temp_section,_T("Digital_Control"),0,FilePath);
				m_Output_data.at(i).decom = (unsigned char)GetPrivateProfileInt(temp_section,_T("Decom"),0,FilePath);
				m_Output_data.at(i).range = (unsigned char)GetPrivateProfileInt(temp_section,_T("Range"),0,FilePath);
				m_Output_data.at(i).m_del_low = (unsigned char)GetPrivateProfileInt(temp_section,_T("M_Del_Low"),0,FilePath);
				m_Output_data.at(i).s_del_high = (unsigned char)GetPrivateProfileInt(temp_section,_T("S_Del_High"),0,FilePath);
				m_Output_data.at(i).delay_timer = (unsigned char)GetPrivateProfileInt(temp_section,_T("Delay_Timer"),0,FilePath);
			}

			for (int i=0;i<BAC_VARIABLE_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Variable%d"),i);
				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Variable_data.at(i).description,STR_VARIABLE_DESCRIPTION_LENGTH,cTemp1,STR_VARIABLE_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Variable_data.at(i).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);



				m_Variable_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
				m_Variable_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
				m_Variable_data.at(i).digital_analog = (unsigned char)GetPrivateProfileInt(temp_section,_T("Digital_Analog"),0,FilePath);
				m_Variable_data.at(i).control = (unsigned char)GetPrivateProfileInt(temp_section,_T("Control"),0,FilePath);
				m_Variable_data.at(i).unused = (unsigned char)GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);
				m_Variable_data.at(i).range = (unsigned char)GetPrivateProfileInt(temp_section,_T("Range"),0,FilePath);

			}

			for (int i=0;i<BAC_PROGRAM_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Program%d"),i);
				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Program_data.at(i).description,STR_PROGRAM_DESCRIPTION_LENGTH,cTemp1,STR_PROGRAM_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Program_data.at(i).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);

				m_Program_data.at(i).bytes = (unsigned short)GetPrivateProfileInt(temp_section,_T("Bytes"),0,FilePath);
				m_Program_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
				m_Program_data.at(i).on_off = (unsigned char)GetPrivateProfileInt(temp_section,_T("On_Off"),0,FilePath);
				m_Program_data.at(i).com_prg = (unsigned char)GetPrivateProfileInt(temp_section,_T("Com_Prg"),0,FilePath);
				m_Program_data.at(i).errcode = (unsigned char)GetPrivateProfileInt(temp_section,_T("Errcode"),0,FilePath);
				m_Program_data.at(i).unused = (unsigned char)GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);
			}

			for (int i=0;i<BAC_CONTROLLER_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Controller%d"),i);

				m_controller_data.at(i).input.number = (unsigned char)GetPrivateProfileInt(temp_section,_T("Input_Number"),0,FilePath);
				m_controller_data.at(i).input.panel = (unsigned char)GetPrivateProfileInt(temp_section,_T("Input_Panel"),0,FilePath);
				m_controller_data.at(i).input.point_type = (unsigned char)GetPrivateProfileInt(temp_section,_T("Input_Point_Type"),0,FilePath);
				m_controller_data.at(i).input_value = GetPrivateProfileInt(temp_section,_T("Input_Value"),0,FilePath);
				m_controller_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
				m_controller_data.at(i).setpoint.number = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Number"),0,FilePath);
				m_controller_data.at(i).setpoint.panel = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Panel"),0,FilePath);
				m_controller_data.at(i).setpoint.point_type = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Point_Type"),0,FilePath);
				m_controller_data.at(i).setpoint_value = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Value"),0,FilePath);
				m_controller_data.at(i).units = (unsigned char)GetPrivateProfileInt(temp_section,_T("Units"),0,FilePath);
				m_controller_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
				m_controller_data.at(i).action = (unsigned char)GetPrivateProfileInt(temp_section,_T("Action"),0,FilePath);
				m_controller_data.at(i).repeats_per_min = (unsigned char)GetPrivateProfileInt(temp_section,_T("Repeats_Per_Min"),0,FilePath);
				m_controller_data.at(i).unused = (unsigned char)GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);
				m_controller_data.at(i).prop_high = (unsigned char)GetPrivateProfileInt(temp_section,_T("Prop_High"),0,FilePath);
				m_controller_data.at(i).proportional = (unsigned char)GetPrivateProfileInt(temp_section,_T("Proportional"),0,FilePath);
				m_controller_data.at(i).reset = (unsigned char)GetPrivateProfileInt(temp_section,_T("Reset"),0,FilePath);
				m_controller_data.at(i).bias = (unsigned char)GetPrivateProfileInt(temp_section,_T("Bias"),0,FilePath);
				m_controller_data.at(i).rate = (unsigned char)GetPrivateProfileInt(temp_section,_T("Rate"),0,FilePath);
			}


			for (int i=0;i<BAC_WEEKLYCODE_ROUTINES_COUNT;i++)
			{
				CString tempsection,temp_code,temp_csc;
				tempsection.Format(_T("WeeklyRoutinesData_%d"),i);
				CString temp_weeklycode_code;
				GetPrivateProfileStringW(tempsection,_T("Data"),_T(""),temp_weeklycode_code.GetBuffer(300),300,FilePath);
				temp_weeklycode_code.ReleaseBuffer();

				for (int j=0;j<WEEKLY_SCHEDULE_SIZE;j++)
				{
					CString temp_value;
					temp_value = temp_weeklycode_code.Left(2);
					temp_weeklycode_code = temp_weeklycode_code.Right(temp_weeklycode_code.GetLength()-2);
					weeklt_time_schedule[i][j]= Str_to_Byte(temp_value);
				}
				unsigned char * temp_point = NULL;
				temp_point = weeklt_time_schedule[i];
				for (int x=0;x<9;x++)
				{
					for (int y=0;y<8;y++)
					{
						m_Schedual_Time_data.at(i).Schedual_Day_Time[y][x].time_minutes = *(temp_point ++);
						m_Schedual_Time_data.at(i).Schedual_Day_Time[y][x].time_hours = *(temp_point ++);
					}
				}

				//for (int x=0;x<9;x++)
				//{
				//	for (int y=0;y<8;y++)
				//	{
				//		m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[y][x].time_minutes = *(my_temp_point ++);
				//		m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[y][x].time_hours = *(my_temp_point ++);
				//	}
				//}

			}

			for (int i=0;i<BAC_ANNUAL_CODE_COUNT;i++)
			{
				CString tempsection,temp_code,temp_csc;
				tempsection.Format(_T("AnnualRoutinesData_%d"),i);
				CString temp_annualcode_code;
				GetPrivateProfileStringW(tempsection,_T("Data"),_T(""),temp_annualcode_code.GetBuffer(MAX_PATH),MAX_PATH,FilePath);
				temp_annualcode_code.ReleaseBuffer();

				for (int j=0;j<ANNUAL_CODE_SIZE;j++)
				{
					CString temp_value;
					temp_value = temp_annualcode_code.Left(2);
					temp_annualcode_code = temp_annualcode_code.Right(temp_annualcode_code.GetLength()-2);
					g_DayState[i][j]= Str_to_Byte(temp_value);
					//weeklt_time_schedule[i][j]= Str_to_Byte(temp_value);
				}
			}

			for (int i=0;i<BAC_PROGRAMCODE_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				CString temp_code;
				unsigned char * temp_point = NULL;
				temp_point = program_code[i];
				temp_section.Format(_T("Program_Code%d"),i);
				program_code_length[i] = (unsigned int)GetPrivateProfileInt(temp_section,_T("Program_Length"),0,FilePath);

				if(program_code_length[i] >400)
				{
					program_code_length[i]=0;
					memset(program_code[i],0,400);
					continue;
				}
				int save_part = program_code_length[i] /100  + 1;
				for (int m=0;m<save_part;m++)
				{
					CString part_section;
					part_section.Format(_T("Program_Code%d"),m);
					CString temp_program_code1;
					GetPrivateProfileStringW(temp_section,part_section,_T(""),temp_program_code1.GetBuffer(MAX_PATH),400,FilePath);
					temp_program_code1.ReleaseBuffer();
					CString temp_program_code = temp_program_code1;
					int temp_length= temp_program_code.GetLength();


					if(program_code_length[i] > (m+1)*100)
					{
						if(temp_length!=100*2)
						{
							MessageBox(_T("Load config file failed!"));
							return;
						}
						for (int x=m*100;x<(m+1)*100;x++)
						{
							CString temp_value;
							temp_value = temp_program_code.Left(2);
							temp_program_code = temp_program_code.Right(temp_program_code.GetLength()-2);
							program_code[i][x] = Str_to_Byte(temp_value);
						}
					}
					else
					{
						if(temp_length != (program_code_length[i]%100)*2)
						{
							MessageBox(_T("Load config file failed!"));
							return;
						}
						for (int x=m*100;x<program_code_length[i];x++)
						{
							CString temp_value;
							temp_value = temp_program_code.Left(2);
							temp_program_code = temp_program_code.Right(temp_program_code.GetLength()-2);
							program_code[i][x] = Str_to_Byte(temp_value);
						}
					}



				}




				


			
				/*WritePrivateProfileStringW(temp_section,_T("Program_Code"),temp_code,FilePath);*/

			}


			for (int i=0;i<BAC_SCREEN_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Screen%d"),i);

				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_screen_data.at(i).description,STR_SCREEN_DESCRIPTION_LENGTH,cTemp1,STR_SCREEN_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_screen_data.at(i).label,STR_SCREEN_LABLE_LENGTH,cTemp1,STR_SCREEN_LABLE_LENGTH);
				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Picture_file"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_screen_data.at(i).label,STR_SCREEN_PIC_FILE_LENGTH,cTemp1,STR_SCREEN_PIC_FILE_LENGTH);

				m_screen_data.at(i).update = GetPrivateProfileInt(temp_section,_T("Update"),0,FilePath);
				m_screen_data.at(i).mode = GetPrivateProfileInt(temp_section,_T("Mode"),0,FilePath);
				m_screen_data.at(i).xcur_grp = GetPrivateProfileInt(temp_section,_T("Xcur_grp"),0,FilePath);
				m_screen_data.at(i).ycur_grp = GetPrivateProfileInt(temp_section,_T("Ycur_grp"),0,FilePath);
			}

			for (int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Weekly_Routines%d"),i);

				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Weekly_data.at(i).description,STR_WEEKLY_DESCRIPTION_LENGTH,cTemp1,STR_WEEKLY_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Weekly_data.at(i).label,STR_WEEKLY_LABEL_LENGTH,cTemp1,STR_WEEKLY_LABEL_LENGTH);

				m_Weekly_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
				m_Weekly_data.at(i).auto_manual = GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
				m_Weekly_data.at(i).override_1_value = GetPrivateProfileInt(temp_section,_T("Override_1_Value"),0,FilePath);
				m_Weekly_data.at(i).override_2_value = GetPrivateProfileInt(temp_section,_T("Override_2_Value"),0,FilePath);
				m_Weekly_data.at(i).off = GetPrivateProfileInt(temp_section,_T("Off"),0,FilePath);
				m_Weekly_data.at(i).unused = GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);

				m_Weekly_data.at(i).override_1.number = GetPrivateProfileInt(temp_section,_T("Override_1_Number"),0,FilePath);
				m_Weekly_data.at(i).override_1.panel = GetPrivateProfileInt(temp_section,_T("Override_1_Panel"),0,FilePath);
				m_Weekly_data.at(i).override_1.point_type = GetPrivateProfileInt(temp_section,_T("Override_1_Point_Type"),0,FilePath);
				m_Weekly_data.at(i).override_2.number = GetPrivateProfileInt(temp_section,_T("Override_2_number"),0,FilePath);
				m_Weekly_data.at(i).override_2.panel = GetPrivateProfileInt(temp_section,_T("Override_2_Panel"),0,FilePath);
				m_Weekly_data.at(i).override_2.point_type = GetPrivateProfileInt(temp_section,_T("Override_2_Point_Type"),0,FilePath);
			}

			for (int i=0;i<BAC_ANNUAL_ROUTINES_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Annual_Routines%d"),i);

				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Annual_data.at(i).description,STR_ANNUAL_DESCRIPTION_LENGTH,cTemp1,STR_ANNUAL_DESCRIPTION_LENGTH);

				cs_temp.Empty();
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Annual_data.at(i).label,STR_ANNUAL_LABEL_LENGTH,cTemp1,STR_ANNUAL_LABEL_LENGTH);

				m_Annual_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
				m_Annual_data.at(i).auto_manual = GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
				m_Annual_data.at(i).unused = GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);

			}

			for (int i=0;i<BAC_MONITOR_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Monitor%d"),i);

				CString cs_temp;
				char cTemp1[255];
				GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
				cs_temp.ReleaseBuffer();
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_monitor_data.at(i).label,STR_MONITOR_LABEL_LENGTH,cTemp1,STR_MONITOR_LABEL_LENGTH);


				CString temp_input_code1;
				GetPrivateProfileStringW(temp_section,_T("Inputs"),_T(""),temp_input_code1.GetBuffer(MAX_PATH),255,FilePath);
				temp_input_code1.ReleaseBuffer();

				CString temp_input = temp_input_code1;
				if(temp_input.GetLength()!= (sizeof(Point_Net)*MAX_POINTS_IN_MONITOR) )
					continue;	//如果这一个的长度不正确 就继续下一个;忽略这个;
				unsigned char temp_in_array[sizeof(Point_Net)*MAX_POINTS_IN_MONITOR];
				for (int x=0;x<(sizeof(Point_Net)*MAX_POINTS_IN_MONITOR);x++)
				{
					CString temp_value;
					temp_value = temp_input.Left(2);
					temp_input = temp_input.Right(temp_input.GetLength()-2);
					temp_in_array[x] = Str_to_Byte(temp_value);
				}
				memcpy_s(&m_monitor_data.at(i).inputs[0],sizeof(Point_Net)*MAX_POINTS_IN_MONITOR,temp_in_array,sizeof(Point_Net)*MAX_POINTS_IN_MONITOR);//copy 70 

				CString temp_range_code1;
				GetPrivateProfileStringW(temp_section,_T("Range"),_T(""),temp_range_code1.GetBuffer(MAX_PATH),255,FilePath);
				temp_range_code1.ReleaseBuffer();

				CString temp_range = temp_range_code1;
				
				if(temp_range.GetLength() != MAX_POINTS_IN_MONITOR)
					continue;
				unsigned char rang_array[MAX_POINTS_IN_MONITOR];
				for (int r=0;r<MAX_POINTS_IN_MONITOR;r++)
				{
					CString temp_value;
					temp_value = temp_range.Left(2);
					temp_range = temp_range.Right(temp_range.GetLength()-2);
					rang_array[r] = Str_to_Byte(temp_value);
				}
				memcpy_s(&m_monitor_data.at(i).range[0],MAX_POINTS_IN_MONITOR,rang_array,MAX_POINTS_IN_MONITOR);//copy MAX_POINTS_IN_MONITOR 14 

				m_monitor_data.at(i).second_interval_time = GetPrivateProfileInt(temp_section,_T("Second_Interval_Time"),0,FilePath);
				m_monitor_data.at(i).minute_interval_time = GetPrivateProfileInt(temp_section,_T("Minute_Interval_Time"),0,FilePath);
				m_monitor_data.at(i).hour_interval_time = GetPrivateProfileInt(temp_section,_T("Hour_Interval_Time"),0,FilePath);
				m_monitor_data.at(i).max_time_length = GetPrivateProfileInt(temp_section,_T("Max_Time_Length"),0,FilePath);
				m_monitor_data.at(i).num_inputs = GetPrivateProfileInt(temp_section,_T("Num_Inputs"),0,FilePath);
				m_monitor_data.at(i).an_inputs = GetPrivateProfileInt(temp_section,_T("An_Inputs"),0,FilePath);
				m_monitor_data.at(i).unit = GetPrivateProfileInt(temp_section,_T("Unit"),0,FilePath);
				m_monitor_data.at(i).wrap_flag = GetPrivateProfileInt(temp_section,_T("Wrap_flag"),0,FilePath);
				m_monitor_data.at(i).status = GetPrivateProfileInt(temp_section,_T("Status"),0,FilePath);
				m_monitor_data.at(i).reset_flag = GetPrivateProfileInt(temp_section,_T("Reset_Flag"),0,FilePath);
				m_monitor_data.at(i).double_flag = GetPrivateProfileInt(temp_section,_T("Double_flag"),0,FilePath);

			}

				DeleteFile(new_file);

			Show_Wait_Dialog_And_SendConfigMessage();
		}
	}

	
}

void CMainFrame::SaveBacnetConfigFile()
{
	if((g_mac!=0) &&(g_bac_instance!=0))
	{
			CString FilePath;
			CStringArray temp_array1;

			SplitCStringA(temp_array1,SaveConfigFilePath,_T("."));
			int temp_array_size=0;
			temp_array_size = temp_array1.GetSize();
			if(temp_array1.GetSize()<=1)
			{
				MessageBox(_T("Prg file error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				return;
			}

			
			int right_suffix = temp_array1.GetAt(temp_array_size - 1).GetLength();
			int config_file_length = SaveConfigFilePath.GetLength();
			if(config_file_length <= right_suffix)
			{
				MessageBox(_T("Prg file error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				return;
			}
			FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
			FilePath = FilePath + _T("ini");
			for (int i=0;i<BAC_INPUT_ITEM_COUNT;i++)
			{
				CString temp_input,temp_des,temp_csc;
				temp_input.Format(_T("Input%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).description, (int)strlen((char *)m_Input_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				WritePrivateProfileStringW(temp_input,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_input,_T("Label"),temp_label,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_input,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%d"),m_Input_data.at(i).value);
				WritePrivateProfileStringW(temp_input,_T("Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).filter);
				WritePrivateProfileStringW(temp_input,_T("Filter"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).decom);
				WritePrivateProfileStringW(temp_input,_T("Decom"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).sen_on);
				WritePrivateProfileStringW(temp_input,_T("Sen_On"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).sen_off);
				WritePrivateProfileStringW(temp_input,_T("Sen_Off"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).control);
				WritePrivateProfileStringW(temp_input,_T("Control"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).digital_analog);
				WritePrivateProfileStringW(temp_input,_T("Digital_Analog"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).calibration_sign);
				WritePrivateProfileStringW(temp_input,_T("Calibration_Sign"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).calibration_increment);
				WritePrivateProfileStringW(temp_input,_T("Calibration_Increment"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).unused);
				WritePrivateProfileStringW(temp_input,_T("Unused"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).calibration);
				WritePrivateProfileStringW(temp_input,_T("Calibration"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).range);
				WritePrivateProfileStringW(temp_input,_T("Range"),temp_csc,FilePath);
			}

			for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Output%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);
				temp_csc.Format(_T("%d"),m_Output_data.at(i).value);
				WritePrivateProfileStringW(temp_section,_T("Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).digital_analog);
				WritePrivateProfileStringW(temp_section,_T("Digital_Analog"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).hw_switch_status);
				WritePrivateProfileStringW(temp_section,_T("hw_switch_status"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).control);
				WritePrivateProfileStringW(temp_section,_T("Control"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).digital_control);
				WritePrivateProfileStringW(temp_section,_T("Digital_Control"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).decom);
				WritePrivateProfileStringW(temp_section,_T("Decom"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).range);
				WritePrivateProfileStringW(temp_section,_T("Range"),temp_csc,FilePath);

				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).m_del_low);
				WritePrivateProfileStringW(temp_section,_T("M_Del_Low"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).s_del_high);
				WritePrivateProfileStringW(temp_section,_T("S_Del_High"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).delay_timer);
				WritePrivateProfileStringW(temp_section,_T("Delay_Timer"),temp_csc,FilePath);
			}

			for (int i=0;i<BAC_VARIABLE_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Variable%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);

				temp_csc.Format(_T("%d"),m_Variable_data.at(i).value);
				WritePrivateProfileStringW(temp_section,_T("Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).digital_analog);
				WritePrivateProfileStringW(temp_section,_T("Digital_Analog"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).control);
				WritePrivateProfileStringW(temp_section,_T("Control"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).unused);
				WritePrivateProfileStringW(temp_section,_T("Unused"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).range);
				WritePrivateProfileStringW(temp_section,_T("Range"),temp_csc,FilePath);
			}

			for (int i=0;i<BAC_PROGRAM_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Program%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);

				temp_csc.Format(_T("%u"),(unsigned short)m_Program_data.at(i).bytes);
				WritePrivateProfileStringW(temp_section,_T("Bytes"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).on_off);
				WritePrivateProfileStringW(temp_section,_T("On_Off"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).com_prg);
				WritePrivateProfileStringW(temp_section,_T("Com_Prg"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).errcode);
				WritePrivateProfileStringW(temp_section,_T("Errcode"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).unused);
				WritePrivateProfileStringW(temp_section,_T("Unused"),temp_csc,FilePath);

			}

			for (int i=0;i<BAC_CONTROLLER_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Controller%d"),i);

				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).input.number);
				WritePrivateProfileStringW(temp_section,_T("Input_Number"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).input.panel);
				WritePrivateProfileStringW(temp_section,_T("Input_Panel"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).input.point_type);
				WritePrivateProfileStringW(temp_section,_T("Input_Point_Type"),temp_csc,FilePath);
				temp_csc.Format(_T("%d"),m_controller_data.at(i).input_value);
				WritePrivateProfileStringW(temp_section,_T("Input_Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%d"),m_controller_data.at(i).value);
				WritePrivateProfileStringW(temp_section,_T("Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).setpoint.number);
				WritePrivateProfileStringW(temp_section,_T("Setpoint_Number"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).setpoint.panel);
				WritePrivateProfileStringW(temp_section,_T("Setpoint_Panel"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).setpoint.point_type);
				WritePrivateProfileStringW(temp_section,_T("Setpoint_Point_Type"),temp_csc,FilePath);

				temp_csc.Format(_T("%d"),m_controller_data.at(i).setpoint_value);
				WritePrivateProfileStringW(temp_section,_T("Setpoint_Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).units);
				WritePrivateProfileStringW(temp_section,_T("Units"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).action);
				WritePrivateProfileStringW(temp_section,_T("Action"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).repeats_per_min);
				WritePrivateProfileStringW(temp_section,_T("Repeats_Per_Min"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).unused);
				WritePrivateProfileStringW(temp_section,_T("Unused"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).prop_high);
				WritePrivateProfileStringW(temp_section,_T("Prop_High"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).proportional);
				WritePrivateProfileStringW(temp_section,_T("Proportional"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).reset);
				WritePrivateProfileStringW(temp_section,_T("Reset"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).bias);
				WritePrivateProfileStringW(temp_section,_T("Bias"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).rate);
				WritePrivateProfileStringW(temp_section,_T("Rate"),temp_csc,FilePath);

			}

			for (int i=0;i<BAC_SCREEN_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				temp_section.Format(_T("Screen%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).description, (int)strlen((char *)m_screen_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).label, (int)strlen((char *)m_screen_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				CString temp_pic_file;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).picture_file, (int)strlen((char *)m_screen_data.at(i).picture_file)+1, 
					temp_pic_file.GetBuffer(MAX_PATH), MAX_PATH );
				temp_pic_file.ReleaseBuffer();

				WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);
				WritePrivateProfileStringW(temp_section,_T("Picture_file"),temp_pic_file,FilePath);

				temp_csc.Format(_T("%u"),(unsigned short)m_screen_data.at(i).update);
				WritePrivateProfileStringW(temp_section,_T("Update"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_screen_data.at(i).mode);
				WritePrivateProfileStringW(temp_section,_T("Mode"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_screen_data.at(i).xcur_grp);
				WritePrivateProfileStringW(temp_section,_T("Xcur_grp"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned short)m_screen_data.at(i).ycur_grp);
				WritePrivateProfileStringW(temp_section,_T("Ycur_grp"),temp_csc,FilePath);
			}


			for (int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
			{
				CString temp_input,temp_des,temp_csc;
				temp_input.Format(_T("Weekly_Routines%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).description, (int)strlen((char *)m_Weekly_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				WritePrivateProfileStringW(temp_input,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_input,_T("Label"),temp_label,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).value);
				WritePrivateProfileStringW(temp_input,_T("Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_input,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1_value);
				WritePrivateProfileStringW(temp_input,_T("Override_1_Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2_value);
				WritePrivateProfileStringW(temp_input,_T("Override_2_Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).off);
				WritePrivateProfileStringW(temp_input,_T("Off"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).unused);
				WritePrivateProfileStringW(temp_input,_T("Unused"),temp_csc,FilePath);


				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1.number);
				WritePrivateProfileStringW(temp_input,_T("Override_1_Number"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1.panel);
				WritePrivateProfileStringW(temp_input,_T("Override_1_Panel"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1.point_type);
				WritePrivateProfileStringW(temp_input,_T("Override_1_Point_Type"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2.number);
				WritePrivateProfileStringW(temp_input,_T("Override_2_number"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2.panel);
				WritePrivateProfileStringW(temp_input,_T("Override_2_Panel"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2.point_type);
				WritePrivateProfileStringW(temp_input,_T("Override_2_Point_Type"),temp_csc,FilePath);

			}

			for (int i=0;i<BAC_ANNUAL_ROUTINES_COUNT;i++)
			{
				CString temp_input,temp_des,temp_csc;
				temp_input.Format(_T("Annual_Routines%d"),i);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).description, (int)strlen((char *)m_Annual_data.at(i).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).label, (int)strlen((char *)m_Annual_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();

				WritePrivateProfileStringW(temp_input,_T("Description"),temp_des,FilePath);
				WritePrivateProfileStringW(temp_input,_T("Label"),temp_label,FilePath);

				temp_csc.Format(_T("%u"),(unsigned char)m_Annual_data.at(i).value);
				WritePrivateProfileStringW(temp_input,_T("Value"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Annual_data.at(i).auto_manual);
				WritePrivateProfileStringW(temp_input,_T("Auto_Manual"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_Annual_data.at(i).unused);
				WritePrivateProfileStringW(temp_input,_T("Unused"),temp_csc,FilePath);
			}

			for (int i=0;i<BAC_MONITOR_COUNT;i++)
			{
				CString temp_monitor,temp_des,temp_csc;
				temp_monitor.Format(_T("Monitor%d"),i);

				CString temp_label;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_monitor_data.at(i).label, (int)strlen((char *)m_monitor_data.at(i).label)+1, 
					temp_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_label.ReleaseBuffer();
				WritePrivateProfileStringW(temp_monitor,_T("Label"),temp_label,FilePath);
				unsigned char * temp_point = NULL;
				CString temp_inputs;
				for (int j=0;j<MAX_POINTS_IN_MONITOR;j++)
				{
					temp_point = &m_monitor_data.at(i).inputs[j].number;
					for (int k=0;k<(int)sizeof(Point_Net);k++)
					{
						temp_csc.Format(_T("%02x"),*(temp_point + k));
						temp_csc.MakeUpper();
						temp_inputs = temp_inputs + temp_csc;
					}
				}
				WritePrivateProfileStringW(temp_monitor,_T("Inputs"),temp_inputs,FilePath);
				temp_point = NULL;
				CString temp_range;

				for (int x=0;x<MAX_POINTS_IN_MONITOR;x++)
				{
					temp_point = &m_monitor_data.at(i).range[x];
					temp_csc.Format(_T("%02x"),*(temp_point));
					temp_csc.MakeUpper();
					temp_range = temp_range + temp_csc;
				}
				WritePrivateProfileStringW(temp_monitor,_T("Range"),temp_range,FilePath);

				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).second_interval_time);
				WritePrivateProfileStringW(temp_monitor,_T("Second_Interval_Time"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).minute_interval_time);
				WritePrivateProfileStringW(temp_monitor,_T("Minute_Interval_Time"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).hour_interval_time);
				WritePrivateProfileStringW(temp_monitor,_T("Hour_Interval_Time"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).max_time_length);
				WritePrivateProfileStringW(temp_monitor,_T("Max_Time_Length"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).num_inputs);
				WritePrivateProfileStringW(temp_monitor,_T("Num_Inputs"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).an_inputs);
				WritePrivateProfileStringW(temp_monitor,_T("An_Inputs"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).unit);
				WritePrivateProfileStringW(temp_monitor,_T("Unit"),temp_csc,FilePath);

				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).wrap_flag);
				WritePrivateProfileStringW(temp_monitor,_T("Wrap_flag"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).status);
				WritePrivateProfileStringW(temp_monitor,_T("Status"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).reset_flag);
				WritePrivateProfileStringW(temp_monitor,_T("Reset_Flag"),temp_csc,FilePath);
				temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).double_flag);
				WritePrivateProfileStringW(temp_monitor,_T("Double_flag"),temp_csc,FilePath);
				
			}

			for (int i=0;i<BAC_WEEKLYCODE_ROUTINES_COUNT;i++)
			{
				unsigned char * temp_point = NULL;
				temp_point = weeklt_time_schedule[i];
				CString tempsection,temp_code,temp_csc;
				tempsection.Format(_T("WeeklyRoutinesData_%d"),i);
				for (int j=0;j<WEEKLY_SCHEDULE_SIZE;j++)
				{
					temp_csc.Format(_T("%02x"),*(temp_point + j));
					temp_csc.MakeUpper();
					temp_code = temp_code + temp_csc;
				}
				WritePrivateProfileStringW(tempsection,_T("Data"),temp_code,FilePath);
			}

			for (int i=0;i<BAC_ANNUAL_ROUTINES_COUNT;i++)
			{
				unsigned char * temp_point = NULL;
				temp_point = g_DayState[i];
				CString tempsection,temp_code,temp_csc;
				tempsection.Format(_T("AnnualRoutinesData_%d"),i);
				for (int j=0;j<ANNUAL_CODE_SIZE;j++)
				{
					temp_csc.Format(_T("%02x"),*(temp_point + j));
					temp_csc.MakeUpper();
					temp_code = temp_code + temp_csc;
				}
				WritePrivateProfileStringW(tempsection,_T("Data"),temp_code,FilePath);
			}

			for (int i=0;i<BAC_PROGRAMCODE_ITEM_COUNT;i++)
			{
				CString temp_section,temp_des,temp_csc;
				CString temp_code;
				unsigned char * temp_point = NULL;
				temp_point = program_code[i];
				temp_section.Format(_T("Program_Code%d"),i);

				temp_csc.Format(_T("%d"),program_code_length[i]);
				WritePrivateProfileStringW(temp_section,_T("Program_Length"),temp_csc,FilePath);

				//将要保存的代码划分为N快存储，每块 100 ，ini文本一个段最多只有保存260;
				int save_part = program_code_length[i] /100  + 1;
				for (int m=0;m<save_part;m++)
				{
					temp_code.Empty();
					if(program_code_length[i] > (m+1)*100)
					{
						for (int j=m*100;j<m*100+100;j++)
						{
							temp_csc.Format(_T("%02x"),*(temp_point + j));
							temp_csc.MakeUpper();
							temp_code = temp_code + temp_csc;
						}
					}
					else
					{
						for (int j=m*100;j<program_code_length[i];j++)
						{
							temp_csc.Format(_T("%02x"),*(temp_point + j));
							temp_csc.MakeUpper();
							temp_code = temp_code + temp_csc;
						}
					}

					CString part_section;
					part_section.Format(_T("Program_Code%d"),m);
					WritePrivateProfileStringW(temp_section,part_section,temp_code,FilePath);
				}
			}

			CFile myfile(FilePath,CFile::modeRead);
			char *pBuf;
			DWORD dwFileLen;
			dwFileLen=myfile.GetLength();
				pBuf= new char[dwFileLen+1];
			pBuf[dwFileLen]=0;
			myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
			myfile.Close();
			//MessageBox(pBuf);
			char * temp_buffer = pBuf;
			for (int i=0;i<dwFileLen;i++)
			{
				*temp_buffer = *temp_buffer ^ 1;
				temp_buffer ++;
			}
			 
			//CString new_file;
			//new_file = FilePath.Left(FilePath.GetLength()-3) + _T("prg");
			CStringArray temp_array2;
			SplitCStringA(temp_array2,SaveConfigFilePath,_T("."));
			temp_array_size=0;
			temp_array_size = temp_array2.GetSize();
			right_suffix = temp_array2.GetAt(temp_array_size - 1).GetLength();
			CString temp_FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
			SaveConfigFilePath = temp_FilePath + _T("prg");





			HANDLE hFile;
			hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
			DWORD dWrites;
			WriteFile(hFile,pBuf,dwFileLen,&dWrites,NULL);
			CloseHandle(hFile);  
			if(pBuf)
				delete pBuf;
			DeleteFile(FilePath);
		
	}
}

void CMainFrame::SaveConfigFile()
{
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		MainFram_hwd = this->m_hWnd;

		CFileDialog dlg(false,_T("*.prg"),_T(" "),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("prg files (*.prg)|*.prg|All Files (*.*)|*.*||"),NULL,0);
		if(IDOK==dlg.DoModal())
		{

			SaveConfigFilePath=dlg.GetPathName();
			//协议时bacnet ，用户点击 File save时 先调用线程读取所有需要存的资料；在发送消息回来 调用SaveBacnetConfigFile;
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SVAE_CONFIG);
		}
		else
		{
			SaveConfigFilePath.Empty();
		}
		
		



	//	SaveBacnetConfigFile();
		return;
	}
	int nret=0;
	g_bEnableRefreshTreeView = FALSE;
	int IDFlag=read_one(g_tstat_id,7);
	product_register_value[7]=IDFlag;
	if(IDFlag<=0)
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
	//AfxMessageBox(strTips);

	if(product_register_value[7]==100||product_register_value[7]==PM_LightingController)//NC
	{
		strTips.Format(_T("Config file \" %s \" is saving, please waiting..."), strFilename);
		SetPaneString(1, strTips);
		if (product_register_value[7]==100)
		{
		save_schedule_2_file((LPTSTR)(LPCTSTR)strFilename,g_tstat_id);
		}
		else
		{
		save_schedule_2_file_LC((LPTSTR)(LPCTSTR)strFilename,g_tstat_id);
		}
		

		strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
		SetPaneString(1, strTips);
	}
	else if (product_register_value[7]==PM_T3PT10||
	product_register_value[7]==PM_T3IOA||
	product_register_value[7]==PM_T332AI||
	product_register_value[7]==PM_T3AI16O||
	product_register_value[7]==PM_T38I13O||
	product_register_value[7]==PM_T3PERFORMANCE||
	product_register_value[7]==PM_T34AO||
	product_register_value[7]==PM_T36CT 
	        )
	{
		strTips.Format(_T("Config file \" %s \" is saving, please waiting..."), strFilename);
		SetPaneString(1, strTips);

		save_T3Modules_file((LPTSTR)(LPCTSTR)strFilename,g_tstat_id);

		strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
		SetPaneString(1, strTips);
	}
	else if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7)||(newtstat6[7] == PM_TSTAT5i))
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

		SetPaneString(3, strTips);
		//AfxMessageBox(strTips);
	}
	else//save tstat config file:
	{//?????
		nret=write_one(g_tstat_id,MODBUS_FAN_GRIDPOINT,4);
		nret=write_one(g_tstat_id,MODBUS_MODE_GRIDPOINT,0);
		nret=write_one(g_tstat_id,MODBUS_HOLD_GRIDPOINT,0);
		nret=write_one(g_tstat_id,MODBUS_CONFIGURATION,0);
		//int nSpecialValue=read_one(g_tstat_id,326);

		//if(nSpecialValue==1)
		//{
		////	write_one(g_tstat_id,324,0);
		//}
		Save2File_ForTwoFiles((LPTSTR)(LPCTSTR)strFilename);

	
		strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
		
		SetPaneString(1, strTips);
		//AfxMessageBox(strTips);
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
	m_Input_data.clear();
	m_Variable_data.clear();
	m_Output_data.clear();
	m_Program_data.clear();
	m_Weekly_data.clear();
	m_Annual_data.clear();
	m_Schedual_Time_data.clear();
	m_controller_data.clear();
	m_screen_data.clear();
	m_monitor_data.clear();
	m_alarmlog_data.clear();
	m_Tstat_data.clear();

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


	for(int i=0;i<10;i++)
	{
		if(pDialog[i]!=NULL)
		{
			delete pDialog[i];
			pDialog[i]=NULL;
		}
	}

	g_bEnableRefreshTreeView = FALSE;
	HTREEITEM htiSel = m_pTreeViewCrl->GetSelectedItem();
	SaveTreeNodeRecordToReg(htiSel);

	if (mbPoll!=NULL)
	{
	  delete mbPoll;
	  mbPoll=NULL;
	}

	if(DebugWindow !=NULL)
	{
		delete DebugWindow;
		DebugWindow = NULL;
	}
	
	for (int i=0;i<10;i++)
	{
		UnregisterHotKey(GetSafeHwnd(), m_MainHotKeyID[i]);   
	}
	UnregisterHotKey(GetSafeHwnd(), 1111);  

#ifdef Fance_Enable_Test		//For Test Use
	if (is_connect())
	{
		close_com(); 
	}
	return;
#endif

	g_killMultiReadEvent.SetEvent();

	Sleep(500);//wait for the end of the thread.
	if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 1000) != WAIT_OBJECT_0)

	//Sleep(500);//wait for the end of the thread.
	if (m_pFreshMultiRegisters) 

	{
		if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 3000) == WAIT_OBJECT_0)
		{

		}
		else
		{		
			BOOL bRet = TerminateThread(m_pFreshMultiRegisters->m_hThread,0);
			delete m_pFreshMultiRegisters;
			m_pFreshMultiRegisters=NULL;
		}

	}
		if(CM5_hThread!=NULL)
			TerminateThread(CM5_hThread,0);
		if(CM5_UI_Thread!=NULL)
			TerminateThread(CM5_UI_Thread,0);
		if(hDeal_thread!=NULL)
			TerminateThread(hDeal_thread,0);
		if(hThread!=NULL)
			TerminateThread(hThread,0);
		if(m_pFreshTree!=NULL)
			TerminateThread(m_pFreshTree,0);
	// 结束线程
	
	if (m_pRefreshThread) 
	{
		HANDLE hp=m_pRefreshThread->m_hThread;
		g_bEnableRefreshTreeView = FALSE;
		PostThreadMessage(m_pRefreshThread->m_nThreadID,  WM_QUIT,0,0);
		g_bPauseRefreshTree = true;
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

	if (g_testmultiReadtraffic_dlg!=NULL)
	{
		delete g_testmultiReadtraffic_dlg;
		g_testmultiReadtraffic_dlg=NULL;
	}
#endif
	CFrameWndEx::OnDestroy();
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

				g_strInHumName = _T("Humidity Sensor");
				g_strInCO2=_T("CO2 Sensor");
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
				 g_strInCO2=_T("CO2 Sensor");

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
	CString strHelp=g_strExePth+_T("The Instruction of T3000 .txt");
	//::HtmlHelp(NULL, strHelp, HH_DISPLAY_TOPIC, 0);
	ShellExecute(NULL, _T("open"), strHelp, NULL, NULL, SW_SHOWNORMAL);
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
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == m_wndStatusBar.GetSafeHwnd())
	{
		CRect rect;  
		CPoint pt;  
		pt = pMsg->pt;  
		m_wndStatusBar.GetItemRect(3,&rect);  
		m_wndStatusBar.ClientToScreen(&rect);  
		if(PtInRect(&rect,pt))  
		{
			//AfxMessageBox(_T("3"));
			CString strHelp=g_strExePth+_T("T3000.log");
			//::HtmlHelp(NULL, strHelp, HH_DISPLAY_TOPIC, 0);
			ShellExecute(NULL, _T("open"), strHelp, NULL, NULL, SW_SHOWNORMAL);
		}
		m_wndStatusBar.GetItemRect(2,&rect);  
		m_wndStatusBar.ClientToScreen(&rect);  
		if(PtInRect(&rect,pt))  
		{
			//AfxMessageBox(_T("2"));
		}
	}
	if((pMsg->message == WM_KEYDOWN ) || (pMsg->message == WM_LBUTTONDOWN) || (pMsg->message == WM_LBUTTONUP))
	{

		no_mouse_keyboard_event_enable_refresh = false;
		time_click = time(NULL);
	}
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(((GetAsyncKeyState( VK_LCONTROL ) & 0x8000))&&(pMsg->wParam ==VK_F2))
		{
			//OnToolIsptoolforone();
			
			int ret = AfxMessageBox(_T("This will reset the device to the factory defaults\n are you sure'?"),MB_YESNO ,3);
			 if ( ret == IDYES)
			 	{
				if (is_connect())
			 	{
				 write_one(g_tstat_id,16,143);
			 	}
				else
				{
				 AfxMessageBox(_T("Please Connect to the Device!"));
				}
				}
				else
				{
				return TRUE;
				}
				  
				    
			
			return 1;
		}
		if(((GetAsyncKeyState( VK_LCONTROL ) & 0x8000))&&(pMsg->wParam =='R'))
		{
			OnToolIsptoolforone();
			return 1;
		}
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
		/*if(pMsg->wParam == VK_RETURN)
		{
		EnterConnectToANode();
		return 1;
		}*/
			

	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}
void CMainFrame::ReFresh()
{
//	int i;
	if (!is_connect())
	{
		 return;
	}
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
	//RefreshTreeView();
	PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);
	EndWaitCursor();

} 
#include "ScanDlg.h"
void CMainFrame::DoConnectToANode( const HTREEITEM& hTreeItem )
{

	m_pTreeViewCrl->SetSelectItem(hTreeItem);

    MainFram_hwd = this->m_hWnd;
	//20120420
	CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
	//创建对话框窗口
	pDlg->Create(IDD_DIALOG10_Progress, this);

	//居中显示
	//pDlg->CenterWindow();
	//void MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );
	//pDlg->MoveWindow(100,100,500,1000);
	pDlg->ShowProgress(0,0);
	//显示对话框窗口
	pDlg->ShowWindow(SW_SHOW);


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
			g_strT3000LogString.Format(_T("Trying to connect to %s:%d"),GetProductName(m_product.at(i).product_class_id),m_product.at(i).serial_number);
			SetPaneString(3,g_strT3000LogString);

			int nID=-1;
			int nTRet=-1;
			//g_tstat_id=m_product.at(i).product_id;
			product_Node=m_product.at(i);
			selected_product_index = i;//记录目前选中的是哪一个 产品;用于后面自动更新firmware;
			selected_tree_item = hTreeItem;
			if(!m_product.at(i).BuildingInfo.strIp.IsEmpty())
			{
				//OnTestPing(product_Node.BuildingInfo.strIp);
				CScanDlg scandlg;
				if (!scandlg.TestPing(m_product.at(i).BuildingInfo.strIp))
				{
					//去掉进度条
					pDlg->ShowWindow(SW_HIDE);
					if(pDlg)
						delete pDlg;//20120220
					pDlg = NULL;
					//显示 IP部分
					scandlg.Set_IsScan(FALSE);
					scandlg.SetNode(product_Node);
					scandlg.DoModal();
					return;
				}
			}
			 

			if((product_Node.product_class_id == PM_CM5) || (product_Node.product_class_id == PM_MINIPANEL))	//如果是CM5或者MINIPANEL 才有 bacnet协议;
			{
#if 1
				        m_nStyle=1;
						

				CString	g_configfile_path=g_strExePth+_T("config.ini");
				CFileFind fFind;
				if(!fFind.FindFile(g_configfile_path))
				{
					CString strInfo;
					strInfo.Format(_T("%d"),1);
					WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),product_Node.BuildingInfo.strIp,g_configfile_path);
					strInfo.Format(_T("%d"),product_Node.ncomport);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
				}
				else{
					CString strInfo;
					strInfo.Format(_T("%d"),1);
					WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),product_Node.BuildingInfo.strIp,g_configfile_path);
					strInfo.Format(_T("%d"),product_Node.ncomport);
					WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
					WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
				}

#endif
				 
					SetPaneString(1,_T(""));
					if(product_Node.product_class_id == PM_CM5)
						bacnet_device_type = PRODUCT_CM5;
					CString temp_csa;
					temp_csa =  product_Node.BuildingInfo.strComPort;
					temp_csa = temp_csa.Right(temp_csa.GetLength() - 3);
					g_bac_instance = m_product.at(i).hardware_version;
					g_selected_serialnumber = m_product.at(i).serial_number;
					g_mac = m_product.at(i).software_version;
					bac_gloab_panel = g_mac;
					g_gloab_bac_comport =_wtoi(temp_csa);
					
					BOOL is_local = IP_is_Local(product_Node.BuildingInfo.strIp);
					if(is_local == false)	//判断是否是本地IP，不是本地的就要连接到远端的，远端的 Who  is  广播发布过去的;
					{
						m_is_remote_device = true;
						((CDialogCM5_BacNet*)m_pViews[m_nCurView])->Set_remote_device_IP(product_Node.BuildingInfo.strIp);
						((CDialogCM5_BacNet*)m_pViews[m_nCurView])->SetConnected_IP(product_Node.BuildingInfo.strIp);
					}
					else
					{
						m_is_remote_device = false;
						((CDialogCM5_BacNet*)m_pViews[m_nCurView])->SetConnected_IP(product_Node.BuildingInfo.strIp);
					}
					g_serialNum = product_Node.serial_number;
					/*bac_net_initial_once = false;*/
					SwitchToPruductType(DLG_BACNET_VIEW);
					g_protocol = PROTOCOL_BACNET_IP;
					pDlg->ShowWindow(SW_HIDE);
					if(pDlg)
						delete pDlg;//20120220
					pDlg = NULL;
					return;
				//}
			}
			
			HANDLE temphandle;
			if(bac_net_initial_once)
			{
				temphandle = Get_RS485_Handle();
				if(temphandle !=NULL)
				{
					TerminateThread((HANDLE)Get_Thread1(),0);
					TerminateThread((HANDLE)Get_Thread2(),0);

					CloseHandle(temphandle);
					Set_RS485_Handle(NULL);
					bac_net_initial_once = false;
				}
			}

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
			//网络设置
			if (!((product_Node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)||(product_Node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0) ||(product_Node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
			{

				if(product_Node.BuildingInfo.hCommunication==NULL||m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0)
				{
					pDlg->ShowProgress(2,10);//20120220
				BOOL bRet = ConnectDevice(product_Node);//ConnectSubBuilding(product_Node.BuildingInfo);
					 
					if (!bRet)
					{
						CheckConnectFailure(product_Node.BuildingInfo.strIp);
						pDlg->ShowWindow(SW_HIDE);//20120220
						delete pDlg;//20120220
						pDlg = NULL;
						m_nStyle=2;
						m_pTreeViewCrl->turn_item_image(hSelItem ,false);//Can't connect to the device , it will show disconnection;
						return;
					}
					else
					{
						m_nStyle=1;
						m_CurSubBuldingInfo=product_Node.BuildingInfo;
						g_CommunicationType=1;
						SetCommunicationType(1);
					}
				}
				
			}
			//串口设置
			else
			{

				if (product_Node.BuildingInfo.strComPort.CompareNoCase(_T("N/A")) == 0)
				{
					AfxMessageBox(_T("Please select COM port"));//\nDatabase->Building config Database
					CAddBuilding  Dlg;
					if(Dlg.DoModal()==IDOK)
					{
					}

				}
				else
				{
						//close_com();//关闭所有端口
					//int nComPort = _wtoi(product_Node.BuildingInfo.strComPort.Mid(3));
				
					int nComPort = product_Node.ncomport;
					if(nComPort == 0 )
					{
						 nComPort = _wtoi(product_Node.BuildingInfo.strComPort.Mid(3));
					}
					//SetPaneCommunicationPrompt(strInfo);
					SetCommunicationType(0);
					close_com();
					BOOL  bret = open_com(nComPort);
					if (!bret)
					{
						AfxMessageBox(_T("COM port open failure!"));			
						if (pDlg !=NULL)
						{
							pDlg->ShowWindow(SW_HIDE);
							delete pDlg;
							pDlg=NULL;
						}
						m_nStyle=2;
						return;
					}else
					{
						CString strInfo;
						strInfo.Format(_T("COM %d Connected: Yes"), nComPort);	
						SetPaneString(1, strInfo);
						SetCommunicationType(0);
						g_CommunicationType=0;

						m_nStyle=1;


						CString	g_configfile_path=g_strExePth+_T("config.ini");
						CFileFind fFind;
						if(!fFind.FindFile(g_configfile_path))
						{
							CString strInfo;
							strInfo.Format(_T("%d"),g_CommunicationType);
							WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
							strInfo.Format(_T("COM%d"),nComPort);
							WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
							strInfo.Format(_T("%d"),nComPort);
							WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
							strInfo=_T("19200");
							WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);

							 
						/*	WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),StringIP,g_configfile_path);
							strInfo.Format(_T("%d"),m_nIpPort);
							WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);

							WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
							WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

							WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);*/
						}
						else{
							CString strInfo;
							strInfo.Format(_T("%d"),g_CommunicationType);
							WritePrivateProfileStringW(_T("Setting"),_T("Connection Type"),strInfo,g_configfile_path);
							strInfo.Format(_T("COM%d"),nComPort);
							WritePrivateProfileStringW(_T("Setting"),_T("COM Port"),strInfo,g_configfile_path);
							strInfo.Format(_T("%d"),nComPort);
							WritePrivateProfileStringW(_T("Setting"),_T("COM_Port"),strInfo,g_configfile_path);
							strInfo=_T("19200");
							WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);

							/*WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),_T("127.0.0.1"),g_configfile_path);
							strInfo.Format(_T("%d"),m_nIpPort);
							WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),strInfo,g_configfile_path);

							WritePrivateProfileStringW(_T("Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
							WritePrivateProfileStringW(_T("Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

							WritePrivateProfileStringW(_T("Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);*/
						}


					}
				}
			}

			m_strCurSubBuldingName=product_Node.BuildingInfo.strBuildingName;
			BOOL bOnLine=FALSE;
			UINT nSerialNumber=0;
			int Device_Type = 0;
			if (g_CommunicationType==0)
			{
				//m_nbaudrat=_wtoi(product_Node.BuildingInfo.strBaudRate);
				m_nbaudrat= product_Node.baudrate;
				if ((m_nbaudrat !=9600 ) && (m_nbaudrat !=19200))
					m_nbaudrat = 19200;
				Change_BaudRate(m_nbaudrat);

				unsigned short read_data[10];
				register_critical_section.Lock();
				int nmultyRet=Read_Multi(g_tstat_id,&read_data[0],0,10,3);
				register_critical_section.Unlock();
				if(nmultyRet <0)
				{
					AfxMessageBox(_T("Can't read your selected ID or you have changed the serial no of the device"));//\nDatabase->Building config Database			
					if (pDlg !=NULL)
					{
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
						pDlg=NULL;
					}
					bOnLine=FALSE;
					return;
				}
				else
				{
					UINT nSerialNumber=0;

					nSerialNumber=read_data[0]+read_data[1]*256+read_data[2]*256*256+read_data[3]*256*256*256;
					Device_Type = read_data[7];
					if(nSerialNumber>0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
						else
						{
							CString tempcs;
							tempcs.Format(_T("The device serial number is %d,the database saved is %d \r\nPlease delete it and rescan."),nSerialNumber,nSelectSerialNumber);
							MessageBox(tempcs);
						}
					}
				}


#if 0
				register_critical_section.Lock();
				nID=read_one(g_tstat_id,6,5);
				register_critical_section.Unlock();
				if(nID<0)		
				{
					//m_nbaudrat=9600;
					//Change_BaudRate(9600);
					//nID=read_one(g_tstat_id,6,2);
					//if(nID<0)
					//	Change_BaudRate(19200);
					bOnLine=FALSE;
				}
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,6);

					if(nRet>=0)
					{

						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
						m_nStyle=1;
					}
				}
				else
				{
					CString str;
					str.Format(_T("Can't get the information of the device.\n1.You can restart t3000 ,and then try connect to the device\n2.Try to connect to the device,again.3.Check you cable"));
					MessageBox(str,_T("Warning"),MB_OK | MB_ICONWARNING);
					//Reset the COM or check to make sure the product is open
					//	AfxMessageBox(_T("Detect the product model not corresponding\nSelect COM port,try again!"));//\nDatabase->Building config Database	
					//AfxMessageBox(_T("Can't read your selected ID or you have changed the serial no of the device\n"));
					if (pDlg !=NULL)
					{
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
						pDlg=NULL;
					}
					bOnLine=FALSE;
					m_nStyle=2;
					//return;
				}
#endif
			}
			if (g_CommunicationType==1)
			{
				unsigned short read_data[10];
				int nmultyRet=Read_Multi(g_tstat_id,&read_data[0],0,10,3);
				if(nmultyRet <0)
				{
					AfxMessageBox(_T("Can't read your selected ID or you have changed the serial no of the device"));//\nDatabase->Building config Database			
					if (pDlg !=NULL)
					{
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
						pDlg=NULL;
					}
					bOnLine=FALSE;
				}
				else
				{
					UINT nSerialNumber=0;

					nSerialNumber=read_data[0]+read_data[1]*256+read_data[2]*256*256+read_data[3]*256*256*256;
					Device_Type = read_data[7];
					if(nSerialNumber>0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
						else
						{
							CString tempcs;
							tempcs.Format(_T("The device serial number is %d,the database saved is %d \r\nPlease delete it and rescan."),nSerialNumber,nSelectSerialNumber);
							MessageBox(tempcs);
						}
					}
				}





				//nID=read_one(g_tstat_id,6);
				//if(nID<0)
				//	bOnLine=FALSE;
				//return;
#if 0
				int ndevice = read_one(nID,7);
				if(ndevice == 50)	//if it is cm5 .
				{
					int protocol_type = read_one(nID,32);
					if(protocol_type == 2)	//如果是MSTP
					{
						//这里要初始化Bacnet的很多东西;

						bac_net_initial_once =false; //设置为没有初始化，等进去后初始化串口 等等~
						g_protocol = 2;	//设置全局协议变更为mstp;
						//SwitchToPruductType(DLG_BACNET_VIEW);
						if (pDlg !=NULL)
						{
							pDlg->ShowWindow(SW_HIDE);
							delete pDlg;
							pDlg=NULL;
						}
						SwitchToPruductType(DLG_BACNET_VIEW);
						return;
					}
					else if((protocol_type == 1) || (protocol_type == 0))//如果不是MSTP 就切换到以前的 界面;
					{

					}
				}
#endif
#if 0
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//

					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,10);
					UINT nSerialNumber=0;
					if(nRet>0)
					{

						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					else
					{
						MessageBox(_T("Read Serial number failed!"));
					}
					if(nSerialNumber>0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
						else
						{
							CString tempcs;
							tempcs.Format(_T("Can't get the information of the device.\n1.You can restart t3000 ,and then try connect to the device\n2.Try to connect to the device,again.3.Check you cable"));
							MessageBox(tempcs,_T("Warning"),MB_OK | MB_ICONWARNING);
						}
					}
				}
				else
				{
					//AfxMessageBox(_T("Can't read your selected ID or you have changed the serial no of the device"));//\nDatabase->Building config Database			
					 
					CString tempcs;
					tempcs.Format(_T("Can't get the information of the device.\n1.You can restart t3000 ,and then try connect to the device\n2.Try to connect to the device,again.3.Check you cable"));
					MessageBox(tempcs,_T("Warning"),MB_OK | MB_ICONWARNING);
					if (pDlg !=NULL)
					{
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
						pDlg=NULL;
					}
					//return;
				}
#endif
			}

			if(bOnLine)
			{ 
				//SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,true);
				m_nStyle=1;
			}
			else
			{

				//SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,false);	
				memset(&multi_register_value[0],0,sizeof(multi_register_value));
				m_nStyle=2;
				//20120424				
				if (pDlg !=NULL)
				{
					pDlg->ShowWindow(SW_HIDE);
					delete pDlg;
					pDlg=NULL;
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
			//Device_Type=read_one(g_tstat_id,7,10);	
			if (Device_Type==100)
			{

				register_critical_section.Lock();
				int i;
				int it = 0;
				float progress;
				for(i=0;i<12;i++)
				{
					//register_critical_section.Lock();
					//int nStart = GetTickCount();
					int itemp = 0;
					itemp = Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100,3);
					if(itemp < 0 )
					{
						//continue;
						break; //读不到就退出，很多时候 NC在读的过程中断开连接T3000 还一直去读剩余的 就会引起无响应;
					}
					else						
					{
						if (pDlg!=NULL)
						{
							progress=float((it+1)*(100/12));
							pDlg->ShowProgress(int(progress),(int)progress);
						} 
					}							
					it++;
					Sleep(100);
				}


				if (it<12)
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

			else if(Device_Type > 0)
			{
				int Use_zigee = 0;
				int power_value = 1;
				if(Device_Type == PM_TSTAT6)
				{
					//TSTAT6 如果是带zigbee的模块，cherly 说 没有寄存器 确认目前是用哪个在通信，只能读640寄存器大概判断;
					//不靠谱;
					//Use_zigee = read_one(g_tstat_id,REGISTER_USE_ZIGBEE_485,5);	
					//if(Use_zigee == 1)
					//	power_value = 2;
					//else
						power_value = 1;
				}
				register_critical_section.Lock();
				int i;
				int it = 0;
				float progress;
				for(i=0;i<(9*power_value);i++)	//暂定为0 ，因为TSTAT6 目前为600多
				{
					int itemp = 0;
					itemp = Read_Multi(g_tstat_id,&multi_register_value[i*(100/power_value)],i*(100/power_value),100/power_value,5);
					if(itemp < 0)
					{
						break;
					}
					else						
					{
						if (pDlg!=NULL)
						{
							progress=float((it+1)*(100/(9*power_value)));
							pDlg->ShowProgress(int(progress),int(progress));
						} 
					}							
					it++;
					Sleep(20 * power_value);
				}

#if 0
				for(i=0;i<(13*power_value);i++)	//暂定为0 ，因为TSTAT6 目前为600多
				{
					int itemp = 0;
					itemp = Read_Multi(g_tstat_id,&multi_register_value[i*(64/power_value)],i*(64/power_value),64/power_value,5);
					if(itemp < 0)
					{
						break;
					}
					else						
					{
						if (pDlg!=NULL)
						{
							progress=float((it+1)*(100/(13*power_value)));
							pDlg->ShowProgress(int(progress),int(progress));
						} 
					}							
					it++;
					Sleep(50 * power_value);
				}
#endif
				memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));

				if (it<9)
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
#if 0 //Fance recode
			if(product_Node.BuildingInfo.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
			{
#if 0
			    if (m_isCM5)
			    {
					CStdioFile default_file;
					CString recordpath;
					recordpath=g_strExePth+_T("Record.txt");
					if(default_file.Open(recordpath,CFile::modeCreate | CFile::modeWrite)==0)
					{
					return;
					}
					int i;
					int it = 0;
					int failure_count = 0;
					float progress;
					register_critical_section.Lock();
					for(i=0;i<80;i++)
					{
						//register_critical_section.Lock();
						//int nStart = GetTickCount();
						CString temp;
						int itemp = 0;
						
						itemp = Read_Multi(g_tstat_id,&multi_register_value_tcp[i*100],i*100,100);
						
						if(itemp < 0)
						{
							failure_count ++ ;
							if(failure_count > 2)//这里读这么多数据，万一中途断开连接 肯定要设置超市退出;
								break;
						   temp.Format(_T("%d*100=Error\n"),i);
						   default_file.WriteString(temp);
							continue;
						}
						else						
						{
							temp.Format(_T("%d*100=OK\n"),i);
							default_file.WriteString(temp);

							if (pDlg!=NULL)
							{
								progress=float((it+1)*(100/80));
								pDlg->ShowProgress(int(progress),(int)progress);
							} 
							it++;
						}							
						
						
					}
					register_critical_section.Unlock();

				



					//Add by Fance use this product_register_value to unite the register.
					//Fance_2
					//memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value_tcp,1024);
					default_file.Flush();
					default_file.Close();
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
					
			    }
				else if (m_isMiniPanel)
				{
					int i;
					int it = 0;
					int failure_count = 0;
					float progress;
					register_critical_section.Lock();
					for(i=0;i<80;i++)
					{
						//register_critical_section.Lock();
						//int nStart = GetTickCount();
						int itemp = 0;
						 itemp = Read_Multi(g_tstat_id,&multi_register_value_tcp[i*100],i*100,100,5);
					//	Sleep(100);
						itemp>0;
						if(itemp < 0)
						{
							failure_count ++ ;
							if(failure_count > 10)//这里读这么多数据，万一中途断开连接 肯定要设置超市退出;
								break;
							continue;
						}
						else						
						{
							if (pDlg!=NULL)
							{
								progress=float((it+1)*(100/80));
								pDlg->ShowProgress(int(progress),(int)progress);
							} 
							it++;
						}							


					}
					register_critical_section.Unlock();
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
				}
				else  //For Zigbee
				{
#endif
		            int NC=read_one(g_tstat_id,7,20);	
					if (NC==100)
					{
#if 1
						register_critical_section.Lock();
						int i;
						int it = 0;
						float progress;
						for(i=0;i<12;i++)
						{
							//register_critical_section.Lock();
							//int nStart = GetTickCount();
							int itemp = 0;
							itemp = Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100,3);
							if(itemp < 0 )
							{
								//continue;
								break; //读不到就退出，很多时候 NC在读的过程中断开连接T3000 还一直去读剩余的 就会引起无响应;
							}
							else						
							{
								if (pDlg!=NULL)
								{
									progress=float((it+1)*(100/12));
									pDlg->ShowProgress(int(progress),(int)progress);
								} 
							}							
							it++;
							Sleep(100);
						}
						//Add by Fance use this product_register_value to unite the register.
						//Fance_2
						//memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value_tcp,1024);

						if (it<12)
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
#endif
					}
					else
					{
						register_critical_section.Lock();
						int i;
						int it = 0;
						float progress;
						for(i=0;i<20;i++)	//暂定为0 ，因为TSTAT6 目前为600多
						{
							//register_critical_section.Lock();
							//int nStart = GetTickCount();
							int itemp = 0;
							itemp = Read_Multi(g_tstat_id,&multi_register_value[i*30],i*30,30,5);
							if(itemp == -2)
							{
								continue;
							}
							else						
							{
								if (pDlg!=NULL)
								{
									progress=float((it+1)*(100/20));
									pDlg->ShowProgress(int(progress),int(progress));
								} 						
								it++;
								Sleep(1500);
							}							

						}

						memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));

						if (it<20)
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


				//}

			} //COMPort
			
		 
			
			
 
			
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
					itemp = Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64,5);
					if(itemp < 0)
					{
						break;
					}
					else						
					{
						if (pDlg!=NULL)
						{
							progress=float((it+1)*(100/10));
							pDlg->ShowProgress(int(progress),int(progress));
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
#endif
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
			nFlag = Device_Type;//read_one(g_tstat_id,7,6);		
			if(nFlag >65530)	//The return value is -1 -2 -3 -4
			{

				AfxMessageBox(_T("Reading product model abnormal \n Try again!"));
				if (pDlg !=NULL)
				{
					pDlg->ShowWindow(SW_HIDE);
					delete pDlg;
					pDlg=NULL;
				}
				return;
			}
			 g_HumChamberThread=FALSE;
			if(nFlag==PM_NC)	
			{	
				SwitchToPruductType(DLG_NETWORKCONTROL_VIEW);
			}
			else if (nFlag == PM_CM5)//CM5
			{
				SwitchToPruductType(DLG_DIALOGCM5_VIEW);
			}
			//else if (nFlag == PM_T34AO)//T3
			//{
			//	memset(newtstat6,0,sizeof(newtstat6));
			//	SwitchToPruductType(DLG_DIALOGT3_VIEW);
			//}
			//else if (nFlag == PM_T3IOA){
			//SwitchToPruductType(DLG_DIALOGT38AI8AO);
			//}

            else if (nFlag == PM_T3IOA)
            {
              
                SwitchToPruductType(DLG_DIALOGT38AI8AO);
            }

            else if (nFlag== PM_T38I13O)
            {
                 
                SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);
            }
            else if (nFlag == PM_T34AO) //T3
            {
                SwitchToPruductType(DLG_DIALOGT3_VIEW);
            }
            else if (nFlag== PM_T3PT10)
            {
                SwitchToPruductType(DLG_DIALOGT3PT10);
            }
            else if (nFlag==PM_T332AI)
            {
                SwitchToPruductType(DLG_DIALOGT332AI_VIEW);
            }
            else if (m_product.at(i).product_class_id==PM_T36CT)
            {
                SwitchToPruductType(DLG_DIALOGT36CT);
            }
			else if (nFlag == PM_MINIPANEL)
			{
				SwitchToPruductType(DLG_DIALOGMINIPANEL_VIEW);
			}
			else if (nFlag==PM_TSTAT6_HUM_Chamber)
			{
				 g_HumChamberThread=TRUE;
				SwitchToPruductType(DLG_HUMCHAMBER);
			}
			else if(nFlag == PM_CO2_RS485)//(nFlag == PM_CO2_NET)||
			{
				SwitchToPruductType(DLG_CO2_VIEW);
			}
            else if(nFlag == PM_CO2_NET)//(nFlag == PM_CO2_NET)||
            {
                SwitchToPruductType(DLG_CO2_NET_VIEW);
            }
			else if (nFlag==PM_T38I13O)
			{
			SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);
			}
			else if (nFlag==PM_T332AI)
			{
			  SwitchToPruductType(DLG_DIALOGT332AI_VIEW);
			}
			else if (nFlag==PM_T3IOA)
			{
			  SwitchToPruductType(DLG_DIALOGT38AI8AO);
			}
			else if(nFlag<PM_NC)	
			{	

				//Use product_type to change the address map
				//if product module is TSTAT6 or TSTAT7 use T3000_6_ADDRESS. T3000_6_ADDRESS is a register table in t3000.mdb  T3000_Register_Address_By_Name
				//else if product module is PM_TSTAT5E PM_TSTAT5H use T3000_5EH_LCD_ADDRESS
				//else if product module is TSTAT5 ABCDFG USE T3000_5ABCDFG_LED_ADDRESS
				//Fance_
				if((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7)|| (nFlag == PM_TSTAT5i) )
				{
					product_type =T3000_6_ADDRESS;
				}
				else if(  (nFlag == PM_TSTAT5E) || (nFlag == PM_TSTAT5H)||(nFlag==PM_TSTAT5G))
				{
					product_type = T3000_5EH_LCD_ADDRESS;
				}
				else if((nFlag == PM_TSTAT5A) ||(nFlag == PM_TSTAT5B) ||
					(nFlag ==PM_TSTAT5C ) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F))
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
				if ((nFlag == PM_TSTAT6)||(nFlag == PM_TSTAT7)||(nFlag == PM_TSTAT5i))
				{
					//multi_register_value[]列表交换。
					//读取TXT
					if (FistWdb)
					{
						Updata_db_tstat6(nFlag);
						FistWdb = FALSE;
					}
					if ((nFlag == PM_TSTAT6&& flagsoftwareversion >35.5)||(nFlag == PM_TSTAT7)||(nFlag == PM_TSTAT5i))
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


				SwitchToPruductType(DLG_T3000_VIEW);


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


BOOL CMainFrame::CheckDeviceStatus()
{
	bool find_new_device = false;

	for(UINT i=0;i<m_product.size();i++)
	{
		//tree0412if(!g_bEnableRefreshTreeView || g_bPauseRefreshTree || g_bPauseMultiRead) 
		if( g_bPauseRefreshTree || g_bPauseMultiRead)
			return false;
		if(first_run_refresh_list_skip_wait == false)
		{
			if(no_mouse_keyboard_event_enable_refresh == false)	//如果突然有客户操作了，就不要在刷新treeview了;
				return false;
		}


		BOOL bOnLine=FALSE;
		UINT nSerialNumber=0;
		int nID;
		bool temp_online = false;
		tree_product tp = m_product.at(i);
		if(m_strCurSubBuldingName.CompareNoCase(tp.BuildingInfo.strBuildingName)==0)
		{
			int nIDNode=tp.product_id;
			nSerialNumber=tp.serial_number;
			
			//int newnID=read_one(nID,6,2);
			/*
			Get the protocol ,if it is bacnet ip,we compare the device id.
			*/		
			//如果strip不是空的就说明这个设备室挂在NC或LC等等下面的MODBUS  RS485设备
				//if((m_product.at(i).protocol == MODBUS_RS485) && (m_product.at(i).BuildingInfo.strIp.IsEmpty()))
				int temp_port = 0;
				temp_port = m_product.at(i).ncomport;
				if((m_product.at(i).protocol == MODBUS_RS485) && (temp_port > 0 ) && (m_product.at(i).BuildingInfo.strIp.IsEmpty()))
				{
					register_critical_section.Lock();
					int nCom = GetLastOpenedComport();
					int temp_comunicationtype = GetLastCommunicationType();
					int temp_baudrate = GetLastSuccessBaudrate();
					//m_product.at(i).status = false;
					temp_online = false;
					SetCommunicationType(0);
					close_com();
					int nComPort = m_product.at(i).ncomport;
					int n_baudrate = m_product.at(i).baudrate;
					if((n_baudrate != 19200) && (n_baudrate != 9600))
						n_baudrate = 19200;
					if(nComPort == 0)
					{
						temp_online = false;
						//m_product.at(i).status = false;
						bOnLine=FALSE;
						register_critical_section.Unlock();
						goto end_condition;
					}
					BOOL  bret = 0;
					if((nComPort>0) && (nComPort<20))
					{
					//TRACE(_T("Open Com%d "),nComPort);
					 bret = open_com(nComPort);
					}
					else
					{
						bret = 0;
					}
					if (!bret)
					{
						temp_online = false;
						//m_product.at(i).status = false;
						bOnLine=FALSE;
						register_critical_section.Unlock();
						goto end_condition;
					}
					else
					{
						//m_nbaudrat=19200;
						//Change_BaudRate(19200);
						Change_BaudRate(n_baudrate);
						
						// read register offset 6
						//int error = modbus_read_one_value( nID,nIDNode,6,5);
						nID=read_one(nIDNode,6,5);
						TRACE(_T("%d = Read_One(%d)     "),nID,nIDNode);
						/* 
						If an error was returned from the read,
						we previously attempted to try again with a reduced baud rate.
						However, a bug in the code meant this never was done correctly
						and problems were reported when the baud rate was changed.
						See discussion in ticket #14
						Now we simply set the online flag to FALSE and give up.
						*/
#if 0
						if (nID<0)
						{
							m_nbaudrat=9600;
							Change_BaudRate(9600);
							nID=read_one(g_tstat_id,6,5);
							if (nID<0)
							{
								Change_BaudRate(19200);
								m_product.at(i).status = false;
								bOnLine=FALSE;
								//continue;
							}
						} 
#endif

						if( nID <0) 
						{
							temp_online = false;
							//m_product.at(i).status = false;
							bOnLine=FALSE;
						} 
						else 
						{
							// successful read of register offset 6
							unsigned short SerialNum[4];
							memset(SerialNum,0,sizeof(SerialNum));
							int nRet=0;//
							// read first 4 registers 
							//	nRet=modbus_read_multi_value(&SerialNum[0],nID,0,4,5); 
							nRet=Read_Multi(nID,&SerialNum[0],0,4,5);
							int nSerialNumberRead;

							if(nRet>=0)  // 计算串口号
							{
								nSerialNumberRead=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256; 
							}

							if(nSerialNumber>=0)  
							{
								if(nSerialNumberRead==nSerialNumber) // 取到串口号，并相等，说明online;
								{
									temp_online = true;
									//m_product.at(i).status = true;
									bOnLine=TRUE;
								}
							}
						}
					}		
					close_com();
					//TRACE(_T(" CloseCom \r\n"));
#if 0
					if( m_product.at(i).BuildingInfo.strProtocol.CompareNoCase(_T("Modbus 485")) == 0)
					{
						g_CommunicationType = 0;
					}
					else
					{
						g_CommunicationType = 1;
					}

					SetCommunicationType(g_CommunicationType);
#endif
end_condition :

					if(temp_comunicationtype == 0)
					{

						//CString strComport = m_product.at(i).BuildingInfo.strComPort;
						//CString strComNum = strComport.Mid(3);
						
						if(nCom !=65535)
						{
							SetCommunicationType(0);
							close_com();
							open_com(nCom);
							if((temp_baudrate != 19200) && (temp_baudrate != 9600))
								temp_baudrate = 19200;
							Change_BaudRate(temp_baudrate);
							
							//TRACE(_T(" OpenCom  %d \r\n"),nCom);
						}
						
						//int nCom = _wtoi(strComNum);
						
					}
					SetCommunicationType(temp_comunicationtype);
					register_critical_section.Unlock();

				}
				else// if(m_product.at(i).protocol == MODBUS_TCPIP)
				{
					temp_online = false;
					//m_product.at(i).status = false;
					for (int x=0;x<(int)m_refresh_net_device_data.size();x++)
					{
						if(nSerialNumber == m_refresh_net_device_data.at(x).nSerial)
						{
							temp_online = true;
							//m_product.at(i).status = true;
							bOnLine = TRUE;
							break;
						}
					}
				}
		}

			m_product.at(i).status_last_time[2] = m_product.at(i).status_last_time[1] ;
			m_product.at(i).status_last_time[1] = m_product.at(i).status_last_time[0] ;
			m_product.at(i).status_last_time[0] = temp_online ;	
			m_product.at(i).status = m_product.at(i).status_last_time[0] || m_product.at(i).status_last_time[1] || m_product.at(i).status_last_time[2];


	}




	//This function add by Fance
	//If the Database not contain the device wo scaned , then added into the database or update it;
	for (int y=0;y<(int)m_refresh_net_device_data.size();y++)
	{
		bool db_exsit = false;
		int n_index = 0;
		bool db_need_update = false;
		if(m_refresh_net_device_data.at(y).nSerial <=0 )
			continue;
		for (int z=0;z<(int)m_product.size();z++)
		{
			if(m_refresh_net_device_data.at(y).nSerial == m_product.at(z).serial_number)
			{
				n_index = z;
				db_exsit = true;
				break;
			}
		}

		if(db_exsit)	//数据库存在，就查看是否要更新;
		{
				if((m_refresh_net_device_data.at(y).ip_address.CompareNoCase(m_product.at(n_index).BuildingInfo.strIp) != 0) ||
					(m_refresh_net_device_data.at(y).nport != m_product.at(n_index).ncomport) ||
					(m_refresh_net_device_data.at(y).modbusID != m_product.at(n_index).product_id))
				{
					m_pCon.CreateInstance(_T("ADODB.Connection"));
					m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

					CString strSql;
					CString str_ip_address;
					CString str_n_port;
					CString str_serialid;
					CString str_modbus_id;
					str_ip_address = m_refresh_net_device_data.at(y).ip_address;
					str_n_port.Format(_T("%d"),m_refresh_net_device_data.at(y).nport);

					str_serialid.Format(_T("%d"),m_refresh_net_device_data.at(y).nSerial);
					str_modbus_id.Format(_T("%d"),m_refresh_net_device_data.at(y).modbusID);
					try
					{
						m_pCon.CreateInstance(_T("ADODB.Connection"));
						m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
						strSql.Format(_T("update ALL_NODE set Bautrate ='%s' where Serial_ID = '%s'"),str_ip_address,str_serialid);
						m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
						strSql.Format(_T("update ALL_NODE set Com_Port ='%s' where Serial_ID = '%s'"),str_n_port,str_serialid);
						m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
						strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),str_modbus_id,str_serialid);
						m_pCon->Execute(strSql.GetString(),NULL,adCmdText);	
					}
					catch(_com_error *e)
					{
						AfxMessageBox(e->ErrorMessage());
					}
					if(m_pCon->State)
						m_pCon->Close();

					m_product.at(n_index).ncomport = m_refresh_net_device_data.at(y).nport;
					m_product.at(n_index).BuildingInfo.strIp = m_refresh_net_device_data.at(y).ip_address;
					m_product.at(n_index).product_id = m_refresh_net_device_data.at(y).modbusID;
				}
			
		}
		else			//不存在 就插入;
		{
			find_new_device = true;
			CString strSql;
			CString str_ip_address;
			CString str_n_port;
			CString str_serialid;
			CString product_name;
			CString modbusid;
			CString product_class_id;

			m_pCon.CreateInstance(_T("ADODB.Connection"));
			m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

			modbusid.Format(_T("%d"),m_refresh_net_device_data.at(y).modbusID);

			str_ip_address = m_refresh_net_device_data.at(y).ip_address;
			str_n_port.Format(_T("%d"),m_refresh_net_device_data.at(y).nport);

			str_serialid.Format(_T("%d"),m_refresh_net_device_data.at(y).nSerial);
			product_class_id.Format(_T("%d"),m_refresh_net_device_data.at(y).product_id);
			product_name = GetProductName(m_refresh_net_device_data.at(y).product_id);
			product_name = product_name + _T(":") + str_serialid + _T("-") + modbusid + _T("-") + str_ip_address;
			strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('Building_1','Sub_net1','"+str_serialid+"','floor1','room1','"+product_name+"','"+product_class_id+"','"+modbusid+"','""','"+str_ip_address+"','""','0','0','"+str_n_port+"','0')"));
			try
			{

				TRACE(strSql);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			if(m_pCon->State)
				m_pCon->Close();
		}

	}
	if(find_new_device)
		 PostMessage(WM_MYMSG_REFRESHBUILDING,0,0);
	return TRUE;
}


LRESULT  CMainFrame::RefreshTreeViewMap(WPARAM wParam, LPARAM lParam)
{
	for(UINT i=0;i<m_product.size();i++)
	{
 		tree_product tp = m_product.at(i);
		if(tp.status>0)  // 如果online，更新显示图片
		{
			if(selected_product_index == i)
			{
				//SetPaneConnectionPrompt(_T("Online!"));
			}
			//SetPaneConnectionPrompt(_T("Online!"));
			m_pTreeViewCrl->turn_item_image(tp.product_item ,true);

		}
		else  // 替换offline的图片
		{
			//SetPaneConnectionPrompt(_T("Offline!"));
			if(selected_product_index == i)
			{
				//SetPaneConnectionPrompt(_T("Offline!"));
			}
			m_pTreeViewCrl->turn_item_image(tp.product_item ,false);

			// 				if(nID==g_tstat_id)
			// 					memset(&multi_register_value[0],0,sizeof(multi_register_value));
		}
		if((g_bac_instance == m_product.at(i).hardware_version) && (g_selected_serialnumber == m_product.at(i).serial_number))
		{
			if(tp.status>0) 
			bac_select_device_online = true;
			else
			bac_select_device_online = false;
		}
	}
	
	
	return 0;
}


LRESULT CMainFrame::Message_Scan_Product(WPARAM wParam, LPARAM lParam){
	OnScanDevice();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// added by zgq;2010-12-1;从OnToolRefreshLeftTreee函数中抽取出来，
// 这个函数主要是用来更新TreeCtrl的。
#if 0
void CMainFrame::RefreshTreeView()
{
	//TRACE("I'm start refreshing tree !!! \n");

	if(m_subNetLst.size()<=0)
		return;
	RefreshNetWorkDeviceListByUDPFunc();
	//BeginWaitCursor();
	// 对所有节点都检测

	for(UINT i=0;i<m_product.size();i++)
	{
		//tree0412if(!g_bEnableRefreshTreeView || g_bPauseRefreshTree || g_bPauseMultiRead) 
		if( g_bPauseRefreshTree || g_bPauseMultiRead)
			return;


		BOOL bOnLine=FALSE;
		UINT nSerialNumber=0;
		int nID;

		tree_product tp = m_product.at(i);
		if(m_strCurSubBuldingName.CompareNoCase(tp.BuildingInfo.strBuildingName)==0)
		{
			int nIDNode=tp.product_id;
			nSerialNumber=tp.serial_number;
			//int newnID=read_one(nID,6,2);
			/*
			Get the protocol ,if it is bacnet ip,we compare the device id.
			*/
#if 0
			if(m_product.at(i).protocol == PROTOCOL_BACNET_IP)
			{
				int find_exsit = false;
				for (int x=0;x<(int)m_bac_scan_com_data.size();x++)
				{
					if(m_bac_scan_com_data.at(x).device_id == m_product.at(i).hardware_version)	//之前使用hardware 代替 device id的
					{
						find_exsit = true;
						break;
					}
				}
				if(find_exsit)
				{
					bOnLine=TRUE;
					TRACE(_T("Device %d is Online\r\n"),(int)m_product.at(i).hardware_version);
				}
				if(g_bac_instance == m_product.at(i).hardware_version)//判断当前选中的设备是否在线，不在线就不允许操作 读写;
				{
					bac_select_device_online = bOnLine;
				}
			}
			else
			{
#endif
				
				if(m_product.at(i).protocol == MODBUS_RS485)
				{
					register_critical_section.Lock();
					SetCommunicationType(0);
					int nComPort = m_product.at(i).ncomport;
					if(nComPort == 0)
					{
						m_product.at(i).status = false;
						bOnLine=FALSE;
						register_critical_section.Unlock();
						goto end_condition;
					}
						
					BOOL  bret = open_com(nComPort);
					if (!bret)
					{
						m_product.at(i).status = false;
						bOnLine=FALSE;
						register_critical_section.Unlock();
						goto end_condition;
					}
					else
					{
						m_nbaudrat=19200;
						Change_BaudRate(19200);

						// read register offset 6
						//int error = modbus_read_one_value( nID,nIDNode,6,5);
						nID=read_one(nIDNode,6,5);
						/* 
						If an error was returned from the read,
						we previously attempted to try again with a reduced baud rate.
						However, a bug in the code meant this never was done correctly
						and problems were reported when the baud rate was changed.
						See discussion in ticket #14
						Now we simply set the online flag to FALSE and give up.
						*/
						if (nID<0)
						{
							m_nbaudrat=9600;
							Change_BaudRate(9600);
							nID=read_one(g_tstat_id,6,5);
							if (nID<0)
							{
								Change_BaudRate(19200);
								m_product.at(i).status = false;
								bOnLine=FALSE;
								//continue;
							}
						} 

						if( nID <0) 
						{
							m_product.at(i).status = false;
							bOnLine=FALSE;
						} 
						else 
						{
							// successful read of register offset 6
							unsigned short SerialNum[4];
							memset(SerialNum,0,sizeof(SerialNum));
							int nRet=0;//
							// read first 4 registers 
							//	nRet=modbus_read_multi_value(&SerialNum[0],nID,0,4,5); 
							nRet=Read_Multi(nID,&SerialNum[0],0,4,5);
							int nSerialNumberRead;

							if(nRet>=0)  // 计算串口号
							{
								nSerialNumberRead=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256; 
							}

							if(nSerialNumber>=0)  
							{
								if(nSerialNumberRead==nSerialNumber) // 取到串口号，并相等，说明online;
								{
									m_product.at(i).status = true;
									bOnLine=TRUE;
								}
							}
						}
					}		
					close_com();

					if( m_product.at(i).BuildingInfo.strProtocol.CompareNoCase(_T("Modbus 485")) == 0)
					{
						g_CommunicationType = 0;
					}
					else
					{
						g_CommunicationType = 1;
					}

					SetCommunicationType(g_CommunicationType);
					if(g_CommunicationType == 0)
					{

						CString strComport = m_product.at(i).BuildingInfo.strComPort;
						CString strComNum = strComport.Mid(3);
						int nCom = _wtoi(strComNum);
						open_com(nCom);
					}
					register_critical_section.Unlock();

				}
				else// if(m_product.at(i).protocol == MODBUS_TCPIP)
				{
					for (int x=0;x<(int)m_refresh_net_device_data.size();x++)
					{
						if((nSerialNumber == m_refresh_net_device_data.at(x).nSerial) && 
							(nIDNode == m_refresh_net_device_data.at(x).modbusID))
						{
							m_product.at(i).status = true;
							bOnLine = TRUE;
							break;
						}
					}
				}
#if 0 //Fance recode 
				if (g_CommunicationType==0) // 通信类型 0
				{	
					// force baud rate to 19200

					m_nbaudrat=19200;
					Change_BaudRate(19200);

					// read register offset 6

					//int error = modbus_read_one_value( nID,nIDNode,6,5);
					nID=read_one(nIDNode,6,5);
					/* 
					If an error was returned from the read,
					we previously attempted to try again with a reduced baud rate.
					However, a bug in the code meant this never was done correctly
					and problems were reported when the baud rate was changed.
					See discussion in ticket #14

					Now we simply set the online flag to FALSE and give up.
					*/
					if (nID<0)
					{
						m_nbaudrat=9600;
						Change_BaudRate(9600);
						nID=read_one(g_tstat_id,6,5);
						if (nID<0)
						{
							Change_BaudRate(19200);
							bOnLine=FALSE;
							//continue;
						}
					} 

					if( nID <0) {
						bOnLine=FALSE;

					} 

					else {
						// successful read of register offset 6
						unsigned short SerialNum[4];
						memset(SerialNum,0,sizeof(SerialNum));
						int nRet=0;//
						// read first 4 registers 
						//	nRet=modbus_read_multi_value(&SerialNum[0],nID,0,4,5); 
						nRet=Read_Multi(nID,&SerialNum[0],0,4,5);
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
#endif

			//}
end_condition :

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
	m_bac_scan_com_data.clear();
	m_refresh_net_device_data.clear();
}
#endif


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
	PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);
	//RefreshTreeView();
	if (m_nCurView == 0)
	{
		((CT3000View*)m_pViews[m_nCurView])->PostMessage(WM_FRESHVIEW,0,0)	;
	}
	
}


UINT _FreshTreeView(LPVOID pParam )
{
	
	//Sleep(30000);
	
	CMainFrame* pMain = (CMainFrame*)pParam;
	while(1)
	{
		if(first_run_refresh_list_skip_wait)
		{


			if(pMain->m_subNetLst.size()<=0)
			{
				continue;
			}
			RefreshNetWorkDeviceListByUDPFunc();
			pMain->CheckDeviceStatus();
			pMain->DoFreshAll();
			first_run_refresh_list_skip_wait = false;
		}

		Sleep(10000);
		while(1)
		{
			if(no_mouse_keyboard_event_enable_refresh)//判断如果一段时间无人操作，才刷新tree，要不然串口资源会经常冲突;
			{
				g_strT3000LogString=_T("T3000 is free");
				::PostMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,3,0);
				break;
			}
			g_strT3000LogString=_T("T3000 is busy.");
			::PostMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,3,0);
			Sleep(100);
		}
		g_strT3000LogString=_T("T3000 is free,and then Checking the status of devices...");
		::PostMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,3,0);

		m_refresh_net_device_data.clear();
		

		WaitForSingleObject(Read_Mutex,INFINITE);//Add by Fance .

		if(b_pause_refresh_tree)
		{
			ReleaseMutex(Read_Mutex);//Add by Fance .
			continue;
		}

		if(pMain->m_subNetLst.size()<=0)
		{
			ReleaseMutex(Read_Mutex);//Add by Fance .
			continue;
		}

		//这里每一步都要确认客服是不是在操作T3000,如果客户在操作（读写）就不要和客户抢资源;
		if(no_mouse_keyboard_event_enable_refresh == false)	
		{
			ReleaseMutex(Read_Mutex);
			continue;
		}
		RefreshNetWorkDeviceListByUDPFunc();
		if(no_mouse_keyboard_event_enable_refresh == false)
		{
			ReleaseMutex(Read_Mutex);
			continue;
		}
		if(!pMain->CheckDeviceStatus())
		{
			ReleaseMutex(Read_Mutex);
			continue;
		}
		if(no_mouse_keyboard_event_enable_refresh == false)
		{
			ReleaseMutex(Read_Mutex);
			continue;
		}
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
	g_bPauseRefreshTree = FALSE;
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

LRESULT CMainFrame::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	UINT fuModifiers = (UINT) LOWORD(lParam);  // key-modifier flags    
	UINT uVirtKey = (UINT) HIWORD(lParam);     // virtual-key code    

	//判断响应了什么热键   
	if( MOD_ALT == fuModifiers && 'S' == uVirtKey )  //Screen
	{  
		OnControlScreens();
		//AfxMessageBox(_T("你按下了组合键 alt + m"));    
	}  
	else if(MOD_ALT == fuModifiers && 'P' == uVirtKey)//Program
	{
		OnControlPrograms();
	}
	else if(MOD_ALT == fuModifiers && 'I' == uVirtKey)//Input
	{
		OnControlInputs();
	}
	else if(MOD_ALT == fuModifiers && 'O' == uVirtKey)//Output
	{
		OnControlOutputs();
	}
	else if(MOD_ALT == fuModifiers && 'V' == uVirtKey)//Variable
	{
		OnControlVariables();
	}
	else if(MOD_ALT == fuModifiers && 'C' == uVirtKey)//Controllers
	{
		OnControlControllers();
	}
	else if(MOD_ALT == fuModifiers && 'W' == uVirtKey)//Weekly routines
	{
		OnControlWeekly();
	}
	else if(MOD_ALT == fuModifiers && 'A' == uVirtKey)//Annual Routines
	{
		OnControlAnnualroutines();
	}
	else if(MOD_ALT == fuModifiers && 'G' == uVirtKey)//Annual Routines
	{
		OnControlMonitors();
	}
	else if(MOD_ALT == fuModifiers && 'L' == uVirtKey)//Annual Routines
	{
		OnControlAlarmLog();
	}
	else if(MOD_SHIFT == fuModifiers && 'D' == uVirtKey)//Annual Routines
	{
		enable_show_debug_window = true;
	}
	else if(MOD_SHIFT == fuModifiers && 'F' == uVirtKey)//Annual Routines
	{
		if(enable_show_debug_window)
			ShowDebugWindow();
	}
	return 0;
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
	if (imodel == PM_TSTAT6||imodel == PM_TSTAT5i)
		 strtable = _T("Tstat6");
    if (imodel == PM_TSTAT5i)
        strtable = _T("Tstat5i");
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

//Code by Fance
//Used for receive all message from all dlg
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
			case MY_INVOKE_ID:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				break;
			case  MY_BAC_WRITE_LIST:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				break;
			case MY_BAC_REFRESH_LIST:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				break;
			case MY_BAC_REFRESH_ONE:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
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

//Code by Fance
//Use background thread deal all data
DWORD WINAPI  CMainFrame::Translate_My_Message(LPVOID lpVoid)
{
	CMainFrame *pParent = (CMainFrame *)lpVoid;
	MSG msg;
	int resend_count=0;
	int temp_end_value=0;
	_MessageInvokeIDInfo *My_Invoke_Struct=NULL;
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
			case MY_INVOKE_ID:
				MyCriticalSection.Lock();
				msg=My_Receive_msg.at(0);
				My_Invoke_Struct = (_MessageInvokeIDInfo *)msg.wParam;
				My_Receive_msg.erase(My_Receive_msg.begin());
				MyCriticalSection.Unlock();
				for (int i=0;i<60;i++)//3秒钟判断任务是否超时.
				{
					if(tsm_invoke_id_free(My_Invoke_Struct->Invoke_ID))
					{
						::PostMessage(MainFram_hwd,MY_RX_TX_COUNT,1,0);
						::PostMessage(My_Invoke_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_SUCCESS,(LPARAM)My_Invoke_Struct);
						goto loop1;
					}
					else
					{

						//Sleep(30);
						Sleep(5);
						continue;
					}
				}
					::PostMessage(MainFram_hwd,MY_RX_TX_COUNT,0,0);
					::PostMessage(My_Invoke_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_FAIL,(LPARAM)My_Invoke_Struct);
		//			tsm_free_invoke_id (My_Invoke_Struct->Invoke_ID);
loop1:
				break;
			case MY_BAC_WRITE_LIST:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageWriteListInfo *My_WriteList_Struct = (_MessageWriteListInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();
					//temp_end_value = My_WriteList_Struct->end_instance;
					do 
					{
						if(My_WriteList_Struct->end_instance<=(My_WriteList_Struct->start_instance+4))
						{
							temp_end_value  =  My_WriteList_Struct->end_instance;
							//continue;//如果大于这个范围，就continue;
						}
						else
							temp_end_value = My_WriteList_Struct->start_instance + 4;

						resend_count = 0;
						do 
						{
							resend_count ++;
							if(resend_count>10)
								break;
							g_invoke_id =WritePrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,My_WriteList_Struct->start_instance,temp_end_value);
							if(g_invoke_id>=0)
							{
								//if((unsigned char)My_WriteList_Struct->command == CONNECTED_WITH_DEVICE)
								//{
								//	connect_invoke_id = g_invoke_id;
								//	connect_replay = true;
								//}
								CString temp_cs_show;
								temp_cs_show.Format(_T("Task ID = %d. %s "),g_invoke_id,My_WriteList_Struct->Write_Info);
								Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,My_WriteList_Struct->hWnd,temp_cs_show,My_WriteList_Struct->ItemInfo.nRow,My_WriteList_Struct->ItemInfo.nCol);
							}
								//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,My_WriteList_Struct->hWnd);
							Sleep(SEND_COMMAND_DELAY_TIME);
						} while (g_invoke_id<0);

						My_WriteList_Struct->start_instance = temp_end_value + 1;
					} while (temp_end_value<My_WriteList_Struct->end_instance);
					
					if(My_WriteList_Struct)		//删除 new 的东西;
						delete My_WriteList_Struct;

					
				}
				break;
			case MY_BAC_REFRESH_LIST:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageRefreshListInfo *My_WriteList_Struct = (_MessageRefreshListInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();
					//switch(My_WriteList_Struct->command)
					//{
					//case READVARIABLE_T3000:
						for (int i=0;i<My_WriteList_Struct->block_size;i++)
						{
								resend_count = 0;
								do 
								{
									resend_count ++;
									if(resend_count>10)
										break;
									if((unsigned char)My_WriteList_Struct->command == READALARM_T3000)//Because the Alarm list need to read one by one 
									{
									 g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,i,i,My_WriteList_Struct->entitysize);
									}
									else
									g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,My_WriteList_Struct->entitysize);
									Sleep(SEND_COMMAND_DELAY_TIME);
								} while (g_invoke_id<0);

						}

				}
				break;
			case MY_BAC_REFRESH_ONE:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageRefreshListInfo *My_Readone_Struct = (_MessageRefreshListInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();
					resend_count = 0;
					do 
					{
						resend_count ++;
						if(resend_count>10)
							break;

						g_invoke_id = GetPrivateData(My_Readone_Struct->deviceid,My_Readone_Struct->command,My_Readone_Struct->start_instance,My_Readone_Struct->end_instance,My_Readone_Struct->entitysize);
						Sleep(SEND_COMMAND_DELAY_TIME);
					} while (g_invoke_id<0);

				}
				break;
			}
			
		}
		Sleep(10);
	}
	return 0;
}


void CMainFrame::OnDatabaseIonameconfig()
{
	// TODO: Add your command handler code here
	CIONameConfig ionameconfig;
	ionameconfig.DoModal();
}


void CMainFrame::OnDatabaseMbpoll()
{
	//// TODO: Add your command handler code here
	//if (mbPollDlgOpen == TRUE) 
	//	{
	//	mbPoll->ShowWindow(SW_SHOW);
	//	return;
	//	}

	//mbPollDlgOpen = TRUE;

	//mbPoll = new CMbPoll(this);

	////mbPoll->Create(IDD_MBPOLL, GetDesktopWindow());
	//mbPoll->Create(IDD_MBPOLL, this);
	//mbPoll->ShowWindow(SW_SHOW);
    
	if (is_connect())
	{
		if (g_CommunicationType==0)
		{
			OnDisconnect();
		}
		CString strHistotyFile=g_strExePth+_T("Modbus Poll.exe");
		ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
	} 
	else
	{
		AfxMessageBox(_T("Connect to your device ,firstly!"));
	}
	
	
}



LRESULT CMainFrame::OnMbpollClosed(WPARAM wParam, LPARAM lParam)
{  
    m_pFreshMultiRegisters->ResumeThread();
    m_pRefreshThread->ResumeThread();
	m_pFreshTree->ResumeThread(); 
	//mbPollDlgOpen = FALSE;
	if (mbPoll!=NULL)
	{   mbPoll = NULL; 
	delete mbPoll;
	}
	//mbPoll->ShowWindow(FALSE);
	
	mbPollDlgOpen = FALSE;
    return 0;
}


//void CMainFrame::OnAppExit()
//{
//     OnDisconnect();
//	 OnDestroy();
//	// TODO: Add your command handler code here
//}
void CMainFrame::OnToolIsptoolforone()
{



	b_pause_refresh_tree = true;
	bool temp_status = g_bPauseMultiRead;
	g_bPauseMultiRead = true;
	int temp_type = GetCommunicationType();


	BOOL bDontLinger = FALSE;
	setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
	closesocket(h_Broad);


	//	isp_product_id = m_product.at(selected_product_index).product_class_id;
	//	if (!((product_Node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)||(product_Node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0) ||(product_Node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
	SetCommunicationType(0);//关闭串口，供ISP 使用;
	close_com();


	CString ISPtool_path;
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	ISPtool_path = ApplicationFolder + _T("\\ISP.exe");
	WinExecAndWait(ISPtool_path,NULL,NULL,0);

	if(temp_type == 0)
	{
		int comport = GetLastOpenedComport();
		open_com(comport);
	}
	else
	{

	}
	SetCommunicationType(temp_type);

	//Fance Add. 在ISP 用完1234 4321 的端口之后，T3000 在重新打开使用，刷新listview 的网络设备要使用;
	h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

	//SOCKADDR_IN bcast;
	h_bcast.sin_family=AF_INET;
	//bcast.sin_addr.s_addr=nBroadCastIP;
	h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
	h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

	//SOCKADDR_IN siBind;
	h_siBind.sin_family=AF_INET;
	h_siBind.sin_addr.s_addr=INADDR_ANY;
	h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
	::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
	b_pause_refresh_tree = false;
	g_bPauseMultiRead = temp_status;



	//MessageBox(_T("This part of code is recoding,Please use ISP.exe to update now."),_T("Notice"),MB_OK | MB_ICONINFORMATION);
	return;
}


 

void CMainFrame::OnViewCommunicatetraffic()
{

	 if(g_testmultiReadtraffic_dlg==NULL){
	 g_testmultiReadtraffic_dlg=new CTestMultiReadTraffic;
	 g_testmultiReadtraffic_dlg->Create(IDD_TEST_MULTI_READ,this);
	 g_testmultiReadtraffic_dlg->ShowWindow(SW_SHOW);
	 }
	 else
	 {
	  g_testmultiReadtraffic_dlg->ShowWindow(SW_SHOW);
	 }
}
void CMainFrame::OnFunctionHumcalibration()
{
	 CCalibrationDlg dlg;
     dlg.DoModal();
}
void CMainFrame::OnControlInputs()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
	//#endif

	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif

	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);	

	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnControlPrograms()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAM);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif

	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAM);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);	
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnControlOutputs()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif

	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);	
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnControlVariables()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_VARIABLE);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_VARIABLE);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);	

	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnControlWeekly()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);	

	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnControlAnnualroutines()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUAL);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUAL);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);		

	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnMiscellaneousLoaddescriptors()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);		
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}


void CMainFrame::OnMiscellaneousUpdatemini()
{
	// TODO: Add your command handler code here
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,WRITEPRGFLASH_COMMAND);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,WRITEPRGFLASH_COMMAND);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);		

	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}

//#include "BacnetController.h"
void CMainFrame::OnControlControllers()
{
	// TODO: Add your command handler code here
	//BacnetController dlg;
	//dlg.DoModal();
	//#ifdef Fance_Enable_Test
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
	//#endif
	//#ifndef Fance_Enable_Test
	//MessageBox(_T("This function is still in development"));
	//#endif
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);		

	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}

void CMainFrame::OnControlScreens()
{
	
	//bac_cm5_graphic = true;
	// TODO: Add your command handler code here
//	CBacnetScreenEdit Dlg;
//	Dlg.DoModal();
//	return;

	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);		
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}
void CMainFrame::OnHelpGetlanguageconfigfile()
{

}
void CMainFrame::OnLanguageEnglish()
{CString infor;
if (g_language==0)
{
	infor=gLoadString(MESSAGEBOX,_T("1"));
	AfxMessageBox(infor);
} 
else
{
theApp.SetLanguage(0);
infor=gLoadString(MESSAGEBOX,_T("2"));
AfxMessageBox(infor);
}

}
void CMainFrame::OnLanguage34010()
{
// 	CString infor;
// 	if (g_language==1)
// 	 {
// 		 infor=gLoadString(MESSAGEBOX,_T("3"));
// 		 AfxMessageBox(infor);
// 	 } 
// 	 else
// 	 {
// 		 theApp.SetLanguage(1);
// 		 infor=gLoadString(MESSAGEBOX,_T("4"));
// 		 AfxMessageBox(infor);
// 	 }
}


void CMainFrame::OnLanguage34006()
{
	 
}

#include "BacnetMonitor.h"
void CMainFrame::OnControlMonitors()
{
	// TODO: Add your command handler code here
#if 0
#ifdef Fance_Enable_Test
	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_MONITOR);
#endif
#ifndef Fance_Enable_Test
	MessageBox(_T("This function is still in development"));
#endif
#endif

	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_MONITOR);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);		
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}

	//CBacnetMonitor Dlg;
	//Dlg.DoModal();
}
void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFrameWndEx::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
}
void CMainFrame::OnToolRegisterviewer()
{
	// TODO: Add your command handler code here
	CRegisterViewerDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(ScreenEdit_Window)
	{	
		::PostMessage(m_screenedit_dlg_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
	}
	if(AlarmWindow_Window)
	{
		::PostMessage(m_alarmwindow_dlg_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
	}
	// TODO: Add your message handler code here
	// Do not call CFrameWndEx::OnPaint() for painting messages
}


void CMainFrame::OnDatabaseBacnettool()
{
	//CBacnetAlarmWindow dlg;
	//dlg.DoModal();
	CBacnetTool dlg;
	dlg.DoModal();
	// TODO: Add your command handler code here
}

void CMainFrame::OnControlAlarmLog()
{
	// TODO: Add your command handler code here
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
			::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALARMLOG);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}
void CMainFrame::OnMenuCheckupdate()
{

CT3000UpdateDlg dlg;
dlg.DoModal();
}


void CMainFrame::OnDatabasePv()
{
	// TODO: Add your command handler code here
	//AfxMessageBox(_T("Developing....."));
	//return;
    CLoginDlg Dlg(g_buser_log_in);
   if (IDOK== Dlg.DoModal())
   {
       CPVDlg dlg;
       dlg.DoModal();
   } 
   else
   {
     AfxMessageBox(_T("Loging.....fail!"));
   }
    

	
}


//Add by Fance 14/05/21
//When user want to update the firmware ,run this code to  get firmware and call ISP.exe to flash firmware.
void CMainFrame::OnHelpUpdatefirmware()
{
	// TODO: Add your command handler code here
#if 0
	MessageBox(_T("This function is still under develepment!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
	return;
	if(m_product.at(selected_product_index).status == false)//说明不在线;
	{
		if(IDNO == MessageBox(_T("The device you selected is not on line or the communication is bad, are you sure you want to update!"),_T("Notice"),MB_YESNO | MB_ICONINFORMATION))
			return;
	}
#endif
	b_pause_refresh_tree = true;
	bool temp_status = g_bPauseMultiRead;
	g_bPauseMultiRead = true;
	int temp_type = GetCommunicationType();


	BOOL bDontLinger = FALSE;
	setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
	closesocket(h_Broad);

	m_product_isp_auto_flash.baudrate = m_product.at(selected_product_index).baudrate;
	m_product_isp_auto_flash.BuildingInfo.strIp = m_product.at(selected_product_index).BuildingInfo.strIp;
	m_product_isp_auto_flash.ncomport =  m_product.at(selected_product_index).ncomport;

	m_product_isp_auto_flash.product_class_id =  m_product.at(selected_product_index).product_class_id;
	m_product_isp_auto_flash.product_id =  m_product.at(selected_product_index).product_id;

//	isp_product_id = m_product.at(selected_product_index).product_class_id;
//	if (!((product_Node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)||(product_Node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0) ||(product_Node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
	SetCommunicationType(0);//关闭串口，供ISP 使用;
	close_com();
	Dowmloadfile Dlg;
	Dlg.DoModal();
	
	if(temp_type == 0)
	{
		int comport = GetLastOpenedComport();
		open_com(comport);
	}
	else
	{

	}
	SetCommunicationType(temp_type);

	//Fance Add. 在ISP 用完1234 4321 的端口之后，T3000 在重新打开使用，刷新listview 的网络设备要使用;
	h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

	//SOCKADDR_IN bcast;
	h_bcast.sin_family=AF_INET;
	//bcast.sin_addr.s_addr=nBroadCastIP;
	h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
	h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

	//SOCKADDR_IN siBind;
	h_siBind.sin_family=AF_INET;
	h_siBind.sin_addr.s_addr=INADDR_ANY;
	h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
	::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
	b_pause_refresh_tree = false;
	g_bPauseMultiRead = temp_status;
}
void CMainFrame::OnControlTstat()
{
	// TODO: Add your command handler code here
	if(g_protocol == PROTOCOL_BACNET_IP)
	{
		if(bac_select_device_online)
		::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_TSTAT);
		else
			MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("This function only support bacnet protocol!\r\nPlease select a bacnet product first."));
	}
}
void CMainFrame::OnCalibrationCalibrationhum()
{
   CCalibrationDlg dlg;
   dlg.DoModal();
}

void CMainFrame::ShowDebugWindow()
{
	if(DebugWindow == NULL)
	{
		DebugWindow = new CDebugWindow;
		DebugWindow->Create(IDD_DIALOG_DEBUG_TRACE, this);
		DebugWindow->ShowWindow(SW_HIDE);
		g_Print = _T("Debug Time 14-06-26   Debug version 1.8");
		DFTrace(g_Print);
	}
	
	if(DebugWindow->IsWindowVisible())
	{
		enable_show_debug_window = false;
		DebugWindow->ShowWindow(SW_HIDE);
	}
	else
		DebugWindow->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateConnect2(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_nStyle == 1);
}

//TCP Server 的 回调函数 ，当客户端连接上 之后就可以用这个socket去发送数据了;//This function add by Fance.
void CALLBACK Listen(SOCKET s, int ServerPort, const char *ClientIP)
{
	int nRet;
	char buf[1000];

	CMulitithreadSocket wsk;
	wsk = s;
	bool get_mutex_control = false;

	while(1)
	{
		//info.Empty();
		//nRet = wsk.ReadData(buf, 800, 60);
		if(wsk.IsSockConnected(s))
		{
			char sendbuf[] = "Receive connect";
			//memset(sendbuf,0,100);

			wsk.SendData(sendbuf,20,3000);
			TRACE(_T("OK\r\n"));
			Sleep(30000);
		}
		else
		{
			//TRACE(_T("No connection\r\n"));
			//Sleep(2000);
			break;
		}
			
		//memset(buf,0,1000);
		//nRet = wsk.ReadData(buf, 1000, 120);
		//if(nRet==-1)
		//{

		//	break;
		//}

	}



	wsk.Close();

}


UINT _PingThread(LPVOID pParam)
{
	CMainFrame* pDlg = (CMainFrame*)(pParam);
	CString strIP = pDlg->m_strPingIP;
	Ping(strIP, pDlg);
	return 1;
}
//Ping 
void CMainFrame::OnTestPing(const CString& strIP)
{
	m_strPingIP = strIP;
	AfxBeginThread(_PingThread, this);
}



BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	int nSize = pCopyDataStruct->cbData;// = (strMsg.GetLength()+1)*sizeof(TCHAR); //(lstrlen(szBuffer)+1)*sizeof(TCHAR);
	//pCopyDataStruct->lpData =  (LPVOID)(LPCTSTR)strMsg;
	CString strMsg = CString(LPCTSTR(pCopyDataStruct->lpData));

	//CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	//pList->AddString(strMsg);
	CString show_info;
	show_info.Format(_T("Ping - "));
	show_info = show_info + strMsg;
	SetPaneString(BAC_SHOW_MISSION_RESULTS,show_info);
	//UpdateStatusInfo(strMsg, FALSE);
	//UpdateData(TRUE);
	return CFrameWndEx::OnCopyData(pWnd, pCopyDataStruct);
	/*return CDialog::OnCopyData(pWnd, pCopyDataStruct);*/
}


void CMainFrame::OnUpdateDisconnect2(CCmdUI *pCmdUI)
{
   pCmdUI->SetCheck(m_nStyle == 2);
}
void CMainFrame::OnUpdateScanDevice(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_nStyle == 3);
}void CMainFrame::OnUpdateAddBuildingConfig(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_nStyle ==4);
}

void CMainFrame::OnUpdateStatusBar(CCmdUI *pCmdUI){
	pCmdUI->Enable(TRUE);

}
#include "MailFeedbackDlg.h"
void CMainFrame::OnHelpFeedbacktotemco()
{
// 	 CMailFeedbackDlg dlg;
// 	 dlg.DoModal();
	CString m_strWebLinker;
	m_strWebLinker.Format(_T("mailto:alex@temcocontrols.com?subject=feedback to temco &body=please add the attachment in the \n%sT3000.log "),g_strExePth);
	try{
		ShellExecute(GetSafeHwnd(), NULL,m_strWebLinker,   NULL, NULL,   SW_SHOWNORMAL);
	}
	catch(...)
	{
		AfxMessageBox(_T("Error:Can't find the email client in your pc!"));
	}
}
