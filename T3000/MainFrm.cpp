
// MainFrm.cpp : implementation of the CMainFrame class
//DoConnectToANode

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "T3000Doc.h"

#include "T3000view.h"
#include "WorkspaceBar.h"
#include "NetworkControllView.h"
#include "T3000DefaultView.h"
#include "global_variable_extern.h"
#include "global_function.h"
#include "AddBuilding.h"
#include "StatusbarCtrl.h"
CMyStatusbarCtrl * statusbar = NULL;
#include "TstatAQ.h"
#include "ISPModeSlove.h"
#include "AllNodesDiaolg.h"
#include "GridLoad.h"

#include "ScanDbWaitDlg.h"
#include "ChangePasswordDlg.h"
#include "ManageAccountDlg.h"
#include "LoginDlg.h"
#include "AfxMessageDialog.h"
 
#include "AnnualRout_InsertDia.h"
#include "EreaseDlg.h"

#include "hangeIDDlg.h"
#include "LightingController/LightingController.h"//Lightingcontroller
#include "TStatScheduleDlg.h"
#include "NewTstatSchedulesDlg.h"
#include "CO2_View.h"
//#include "MbpGlobals.h"
#include "Dialog_Progess.h"

#include "excel9.h"
#include "ParameterDlg.h"
#include "Powermeter.h"
#include "BacnetTool.h"
#include "BacnetAlarmWindow.h"
#include "Dialog_Progess.h"
#include "TStatOutputView.h"
extern CDialog_Progess *WaitRead_Data_Dlg;
//////////////////////////////
//#include "isp/CDialogISPTOOL.h"
#include "ShowMessageDlg.h"
#include "IONameConfig.h"
#include "PowerMeterList.h"
#include "DialogCM5_BacNet.h"

#include "T38I13O.h"
#include "T332AI.h"
#include "global_define.h"
#include "ping.h"
#include "BacnetUserlogin.h"
#include "BacnetAddLabel.h"
#include "CustomSource/Splash.h"
extern CBacnetAddLabel * Add_Label_Window;
extern CBacnetUserlogin * User_Login_Window;
extern CString Re_Initial_Bac_Socket_IP;
extern unsigned char already_check_tstat_timesync; //每个TSTAT设备 只有点击的时候在确认时间;
HANDLE hretryThread = NULL;
extern CString AutoFlashConfigPath;
HTREEITEM  hTreeItem_retry =NULL;
HTREEITEM  hLastTreeItem =NULL;
#include "T38AI8AO.h"
#include "rs485.h" // For call Get_RS485_Handle() function
#include "BacnetWait.h"
#include "BacnetScreenEdit.h"
#include "LanguageLocale.h"
#include "DebugWindow.h"

#include "T36CT.h"
#include "T3RTDView.h"
#include "CO2NetView.h"
#include "htmlhelp.h"
#include "T3000UpdateDlg.h"
#include "Dowmloadfile.h"
#include "ptp.h"
#include "datalink.h"
#include "TCP_Server.h"
#include "PressureSensorForm.h"
#include "TroubleShootDlg.h"
#include "TempHumSensorForm.h"
 

#include <algorithm>
#include "BacnetMonitor.h"
#include "T3ModulesView.h"
#include "T3000_Default_MainView.h"
#include "T3ModulesOutputDlg.h"
#include "ProductRegisterListView.h"
#include "TStatInputView.h"
#include "DuplicateIdDetected.h"
#include "BoatMonitorViewer.h"
#include "Class/md5.h"
#include "NewTstatSchedulesDlg.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "BTUMeterDlg.h"
#include "ProductModel.h"
#include "BacnetRegisterListView.h"
#include "CO2_NodeView.h"
#include "ZigbeeRepeater.h"
#include "BacnetThirdPartyMain.h"
bool b_create_status = false;
const TCHAR c_strCfgFileName[] = _T("config.txt");
//	配置文件名称，用于保存用户设置
extern bool b_statusbarthreadflag;
extern tree_product	m_product_isp_auto_flash;
extern void intial_bip_socket();
#pragma region Fance Test
//For Test
// 在没有鼠标和键盘消息的时候 就启用自动刷新 treeview,如果有就 不要刷新，因为如果正在刷新，客户肯能就无法第一时间读到自己想要的数据;
bool mstp_read_result = false; //MSTP 设备 记录 建立连接时，是否为客户手动中断操作;
bool start_record_time = true;	//开启计时，如果用户一段时间无键盘和鼠标左键操作就开启自动刷新;
unsigned long time_click = 0;
tree_product selected_product_Node; // 选中的设备信息;
bool enable_show_debug_window = false;
BacnetWait *WaitWriteDlg=NULL;
HANDLE hwait_write_thread = NULL;
HANDLE hwait_read_thread = NULL;
HANDLE hwait_read_modbus10000 = NULL;
HANDLE hwait_write_modbus10000 = NULL;
HANDLE hwait_write_tstat_cfg = NULL;
HANDLE h_mul_ping_thread = NULL;
int mul_ping_flag = 1;
_Refresh_Write_Info Write_Config_Info;
HANDLE hStartEvent; // thread start event
extern int isp_product_id;
vector <MSG> My_Receive_msg;
#define WM_CREATE_STATUSBAR WM_USER + 638
#define WM_TROUBLESHOOT_MSG  WM_USER + 639

HANDLE hDeal_thread;
DWORD nThreadID_Do;
extern CBacnetScreenEdit * ScreenEdit_Window;
extern CBacnetAlarmWindow * AlarmWindow_Window;
extern bool m_is_remote_device ;
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
extern void  init_info_table(void);
extern void Init_table_bank();
#include "BacnetScreen.h"
#include "BacnetProgram.h"
#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetVariable.h"
#include "BacnetAnnualRoutine.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetController.h"
#include "BacnetMonitor.h"
#include "BacnetAlarmLog.h"
#include "BacnetTstat.h"
#include "BacnetRemotePoint.h"
#include "BacnetSetting.h"
extern BacnetScreen *Screen_Window;
extern CBacnetProgram *Program_Window;
extern CBacnetInput *Input_Window ;
extern CBacnetOutput *Output_Window ;
extern CBacnetVariable *Variable_Window ;
extern BacnetWeeklyRoutine *WeeklyRoutine_Window ;
extern BacnetAnnualRoutine *AnnualRoutine_Window ;
extern BacnetController *Controller_Window ;
extern CBacnetMonitor *Monitor_Window ;
extern CBacnetAlarmLog *AlarmLog_Window ;
extern CBacnetTstat *Tstat_Window ;
extern CBacnetRemotePoint* Remote_Point_Window ;
extern int bacnet_view_number;
extern CBacnetSetting *Setting_Window;

CCriticalSection MyCriticalSection;


char local_network_ip[255];
CString local_enthernet_ip;
int m_MainHotKeyID[14] =
{
    3001,
    3002,
    3003,
    3004,
    3005,
    3006,
    3007,
    3008,
    3009,
    3010,
    3011,
    3012,
    3013
};

UINT __cdecl  _FreshTreeView(LPVOID pParam);
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


#ifdef _DEBUG
    #define new DEBUG_NEW


#endif
// CMainFrame

#define ID_NODEFAULTMAINBUILDING_MSG 8000
#define SCAN_TIMER 2
#define MONITOR_MOUSE_KEYBOARD_TIMER 3
#define FOR_LAST_VIEW_TIMER 5

#define MESSAGE_INFORMATION_DEAL 5

const UINT WM_HTTPDOWNLOAD_THREAD_FINISHED = WM_APP + 100;
CEvent g_killMultiReadEvent;

volatile HANDLE Read_Mutex=NULL;
//CEvent g_pauseMultiReadEvent;

//BOOL g_bPauseMultiRead=FALSE;
BOOL m_active_key_mouse = FALSE;



//tree0412  2017 05 11 fance change
#if 1
#define TVINSERV_BUILDING 		    {tvInsert.item.iImage=0; tvInsert.item.iSelectedImage=0;}
#define TVINSERV_FLOOR	 			{tvInsert.item.iImage=0; tvInsert.item.iSelectedImage=0;}
#define TVINSERV_ROOM				{tvInsert.item.iImage=2; tvInsert.item.iSelectedImage=2;}
#define TVINSERV_TSTAT_DEFAULT 	    {tvInsert.item.iImage=6; tvInsert.item.iSelectedImage=6;}     //默认的产品图标
#define TVINSERV_LED_TSTAT7 		{tvInsert.item.iImage=8; tvInsert.item.iSelectedImage=8;}   //TSTAT7
#define TVINSERV_CMFIVE			    {tvInsert.item.iImage=10;tvInsert.item.iSelectedImage=10;}   //TSTAT7
#define TVINSERV_NET_WORK		    {tvInsert.item.iImage=12;tvInsert.item.iSelectedImage=12;}
#define TVINSERV_MINIPANEL		    {tvInsert.item.iImage=14;tvInsert.item.iSelectedImage=14;} //MiniPanel
#define TVINSERV_LC				    {tvInsert.item.iImage=26;tvInsert.item.iSelectedImage=26;} //Lightingcontroller
#define TVINSERV_TSTAT6			    {tvInsert.item.iImage=16;tvInsert.item.iSelectedImage=16;}//tstat6
#define TVINSERV_CO2			    {tvInsert.item.iImage=18;tvInsert.item.iSelectedImage=18;}//CO2
#define TVINSERV_T3ARM			    {tvInsert.item.iImage=20;tvInsert.item.iSelectedImage=20;}//CO2
#define TVINSERV_CS3000             {tvInsert.item.iImage=22;tvInsert.item.iSelectedImage=22;}//cs3000
#define TVINSERV_TSTAT8			    {tvInsert.item.iImage=24;tvInsert.item.iSelectedImage=24;}//TSTAT8
#define TVINSERV_T3LC			    {tvInsert.item.iImage=26;tvInsert.item.iSelectedImage=26;}//LC
#define TVINSERV_ZIGBEE_REPEATER    {tvInsert.item.iImage=28;tvInsert.item.iSelectedImage=28;}//zigbeerepeater
#define TVINSERV_PM5E               {tvInsert.item.iImage=30;tvInsert.item.iSelectedImage=30;}//PM5E
#define TVINSERV_THIRD_PARTY        {tvInsert.item.iImage=32;tvInsert.item.iSelectedImage=32;}//第三方设备
#endif

#define ITEM_MASK				TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_MESSAGE(WM_REFRESH_TREEVIEW_MAP, RefreshTreeViewMap)
    ON_MESSAGE(WM_HADNLE_DUPLICATE_ID, HandleDuplicateID)
    ON_MESSAGE(WM_HADNLE_ISP_MODE_DEVICE, HandleIspModedivice)
    ON_MESSAGE(WM_SCAN_PRODUCT, Message_Scan_Product)
    ON_MESSAGE(MY_BAC_CONFIG_READ_RESULTS, ReadConfigFromDeviceMessageCallBack)
    ON_MESSAGE(MY_RESUME_DATA, AllWriteMessageCallBack)
    ON_MESSAGE(MY_RX_TX_COUNT, Refresh_RX_TX_Count)
    ON_MESSAGE(MY_RETRY_MESSAGE, Retry_Connect_Message)
    ON_MESSAGE(MY_RETRY_IP_CHANGE_MESSAGE, ReConnect_Message)
    ON_MESSAGE(WM_DELETE_NEW_MESSAGE_DLG, Delete_Write_New_Dlg)
    ON_MESSAGE(WM_HOTKEY, &CMainFrame::OnHotKey)
    ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
    ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
    ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
    ON_COMMAND(ID_BUILDINGBAR, OnCheckBuildingBar)
    ON_UPDATE_COMMAND_UI(ID_BUILDINGBAR, OnUpdateCheckBuildingBar)
    ON_COMMAND(ID_INPUTOUTPUTPANE, OnCheckIOPane)
    ON_UPDATE_COMMAND_UI(ID_INPUTOUTPUTPANE, OnUpdateCheckIOPane)
    ON_COMMAND(ID_FILE_SAVE_CONFIG, OnFileOpen)
    ON_COMMAND(ID_CONFIGFILE_SAVE_AS, SaveConfigFile)
    ON_COMMAND(ID_LOAD_CONFIG, OnLoadConfigFile)
    ON_COMMAND(ID_ALLNODESDATABASE, OnAllNodeDatabase)
    ON_COMMAND(ID_FILE_LOADCONFIGFILE, OnLoadConfigFile)
    ON_COMMAND(ID_FILE_BATCHBURNHEX, OnBatchFlashHex)
    ON_MESSAGE(WM_CREATE_STATUSBAR, OnCreateStatusBarWindow)
    ON_MESSAGE(WM_USER_FRESHSTATUSBAR, OnFreshStatusBar)
    ON_MESSAGE(WM_ADDTREENODE, OnAddTreeNode)
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_COMMAND(ID_USERACCOUNT_CHANGEPASSWORD, OnChangeAdminPassword)
    ON_COMMAND(ID_USERACCOUNT_MANNAGEUSER, OnUserMannageMent)
    ON_UPDATE_COMMAND_UI(ID_USERACCOUNT_MANNAGEUSER, OnUserMannageMentUpdate)
    ON_COMMAND(ID_USERACCOUNT_LOGIN, OnUserLogin)
    ON_COMMAND(ID_HELP, OnHelp)
    ON_COMMAND(ID_TOOL_EREASE_CHANGE_ID, OnToolErease)
    ON_COMMAND(ID_TOOL_FRESH, OnToolFresh)
    ON_COMMAND(ID_TOOL_FRESHLEFTTREEVIEW, OnToolRefreshLeftTreee)
    ON_COMMAND(ID_DATABASE_MBPOLL, &CMainFrame::OnDatabaseMbpoll)
    ON_COMMAND(ID_DATABASE_IONAMECONFIG, &CMainFrame::OnDatabaseIonameconfig)
    ON_COMMAND(ID_TOOL_ISPTOOLFORONE, &CMainFrame::OnToolIsptoolforone)
    ON_COMMAND(ID_CONTROL_MAIN, &CMainFrame::OnControlMain)
    ON_COMMAND(ID_CONTROL_INPUTS, &CMainFrame::OnControlInputs)
    ON_COMMAND(ID_CONTROL_PROGRAMS, &CMainFrame::OnControlPrograms)
    ON_COMMAND(ID_CONTROL_PANEL, &CMainFrame::OnControlPanel)
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
    ON_COMMAND(ID_CONTROL_TSTAT, &CMainFrame::OnControlRemotePoint)
    ON_COMMAND(ID_CONNECT2, OnConnect)
    ON_UPDATE_COMMAND_UI(ID_CONNECT2, &CMainFrame::OnUpdateConnect2)
    ON_COMMAND(ID_DISCONNECT_COM, OnDisconnect)
    ON_UPDATE_COMMAND_UI(ID_DISCONNECT_COM, &CMainFrame::OnUpdateDisconnect2)
    ON_COMMAND(ID_SCAN, OnScanDevice)
    ON_UPDATE_COMMAND_UI(ID_SCAN, &CMainFrame::OnUpdateScanDevice)
    ON_COMMAND(ID_BUILDINGCONFIGDB, OnAddBuildingConfig)
    ON_UPDATE_COMMAND_UI(ID_BUILDINGCONFIGDB, &CMainFrame::OnUpdateAddBuildingConfig)
    ON_WM_COPYDATA()
    ON_UPDATE_COMMAND_UI(IDS_SHOW_RESULTS, &CMainFrame::OnUpdateStatusBar)
    ON_COMMAND(ID_HELP_FEEDBACKTOTEMCO, &CMainFrame::OnHelpFeedbacktotemco)
    ON_COMMAND(ID_CONTROL_CUSTOMERUNITS, &CMainFrame::OnControlCustomerunits)
    ON_COMMAND(ID_MISCELLANEOUS_GSMCONNECTION, &CMainFrame::OnMiscellaneousGsmconnection)
    ON_MESSAGE(WM_HTTPDOWNLOAD_THREAD_FINISHED, OnThreadFinished)
    ON_COMMAND(ID_CONTROL_SETTINGS, &CMainFrame::OnControlSettings)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_MAIN, &CMainFrame::OnUpdateControlMain)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_INPUTS, &CMainFrame::OnUpdateControlInputs)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_OUTPUTS, &CMainFrame::OnUpdateControlOutputs)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_VARIABLES, &CMainFrame::OnUpdateControlVariables)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_PROGRAMS, &CMainFrame::OnUpdateControlPrograms)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_SCREENS, &CMainFrame::OnUpdateControlScreens)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_CONTROLLERS, &CMainFrame::OnUpdateControlControllers)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_WEEKLY, &CMainFrame::OnUpdateControlWeekly)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_ANNUALROUTINES, &CMainFrame::OnUpdateControlAnnualroutines)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_MONITORS, &CMainFrame::OnUpdateControlMonitors)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_ALARM_LOG, &CMainFrame::OnUpdateControlAlarmLog)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_PANEL, &CMainFrame::OnUpdateControlPanel)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_TSTAT, &CMainFrame::OnUpdateControlTstat)
    ON_UPDATE_COMMAND_UI(ID_CONTROL_SETTINGS, &CMainFrame::OnUpdateControlSettings)
    ON_COMMAND(ID_TOOL_DETECTONLINEPRODUCTS, &CMainFrame::OnToolDetectonlineproducts)
    ON_WM_HELPINFO()
    ON_COMMAND(ID_FILE_EXPORTREGISETERSLIST1, &CMainFrame::OnFileExportregiseterslist)
    ON_COMMAND(ID_TOOL_REGISTERSMAINTENANCESYSTEM, &CMainFrame::OnToolRegistersmaintenancesystem)
    ON_COMMAND(ID_TOOL_FLASHSN, &CMainFrame::OnToolFlashsn)
    ON_WM_MOVE()
    ON_COMMAND(ID_TOOL_BOOTLOADER, &CMainFrame::OnToolBootloader)
    ON_COMMAND(ID_TOOLS_PSYCHROMETRY, &CMainFrame::OnToolsPsychrometry)
    ON_COMMAND(ID_TOOLS_OPTION, &CMainFrame::OnToolsOption)
    ON_COMMAND(ID_HELP_USING_UPDATE, &CMainFrame::OnHelpUsingUpdate)
    ON_MESSAGE(WM_PING_MESSAGE, PingDevice)
    ON_WM_SIZE()
    ON_COMMAND(ID_MODE_ONLINEMODE, &CMainFrame::OnModeOnlinemode)
    ON_COMMAND(ID_MODE_OFFLINEMODE, &CMainFrame::OnModeOfflinemode)
    ON_UPDATE_COMMAND_UI(ID_MODE_ONLINEMODE, &CMainFrame::OnUpdateModeOnlinemode)
    ON_UPDATE_COMMAND_UI(ID_MODE_OFFLINEMODE, &CMainFrame::OnUpdateModeOfflinemode)
    ON_COMMAND(ID_TOOLS_PHCHART, &CMainFrame::OnToolsPhchart)
    ON_COMMAND(ID_TOOLS_MODBUSTOBACNETROUTER, &CMainFrame::OnToolsModbustobacnetrouter)
    ON_COMMAND(ID_CONTROL_MODBUSBACNETREGISTERLISTVIEW, &CMainFrame::OnControlModbus)
    ON_COMMAND(ID_CONTROL_IO_NET_CONFIG, &CMainFrame::OnControlIoNetConfig)
        ON_COMMAND(ID_DATABASE_LOGDETAIL, &CMainFrame::OnDatabaseLogdetail)
        ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, &CMainFrame::OnUpdateAppAbout)
        END_MESSAGE_MAP()

static UINT indicators[] =
{
    //ID_SEPARATOR,           // status line indicator
    ID_RW_INFO,
    ID_BUILDING_INFO,
    ID_PROTOCOL_INFO,
    IDS_SHOW_RESULTS,

};

bool sort_by_minipanel_panelnumber(refresh_net_device object1,refresh_net_device object2)
{
	return object1.panal_number < object2.panal_number;
}

bool sort_by_minipanel_pid(refresh_net_device object1,refresh_net_device object2)
{
	return object1.product_id > object2.product_id;
}

unsigned short tempchange[512];
int index_Count=0;
UINT _ReadMultiRegisters(LPVOID pParam)
{

 
    CMainFrame* pFrame=(CMainFrame*)(pParam);
    BOOL bFirst=TRUE;
    Read_Mutex=CreateMutex(NULL,TRUE,_T("Read_Multi_Reg"));
    ReleaseMutex(Read_Mutex);
    Sleep(15*1000);
    CString g_strT3000LogString;
    //forbid  _ReadMultiRegisters  and _FreshTreeView access com port at the same time.

    while(1)
    {
        if(::WaitForSingleObject(g_killMultiReadEvent,0)==WAIT_OBJECT_0)
            break;

        g_bEnableRefreshTreeView = TRUE;
        if (pFrame->m_lastinterface !=19)
        {
            bFirst = FALSE;
        }

		if(g_bPauseMultiRead)
		{
			Sleep(100);
			continue;
		}

        if(!bFirst)
        {
            Sleep(10000);

            g_bEnableRefreshTreeView = FALSE;


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
        }
        if (!is_connect())
        {
            Sleep(10);
            continue;
        }
        //if ((g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_MSTP_TO_MODBUS) || (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
        if (SPECIAL_BAC_TO_MODBUS)
        {
            Sleep(5000);
            continue;
        }

        bFirst=FALSE;
        WaitForSingleObject(Read_Mutex,INFINITE);



        //int nRet =read_one(g_tstat_id,6,2);

        //if(nRet<0)
        //{
        //    ReleaseMutex(Read_Mutex);//Add by Fance .
        //    continue;
        //}

        for(int i=0; i<17; i++) //Modify by Fance , tstat 6 has more register than 512;
        {
            if(g_tstat_id_changed)
            {
                Sleep(10);
                continue;
            }
            if(g_bPauseMultiRead)
            {
                Sleep(10);
                break;
            }
            //register_critical_section.Lock();
            //
            int multy_ret = 0;
            multy_ret = Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100);
            //register_critical_section.Unlock();
            Sleep(SEND_COMMAND_DELAY_TIME);
            if(multy_ret<0)		//Fance : 如果出现读失败 就跳出循环体,因为如果是由断开连接 造成的 读失败 会使其他需要用到读的地方一直无法获得资源;
                break;
        }

        ReleaseMutex(Read_Mutex);//Add by Fance .
		  if(g_bPauseMultiRead)
		  {
			  continue;
		  }
        memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
        CString achive_file_path;
        CString temp_serial;
        temp_serial.Format(_T("%u.prog"),g_selected_serialnumber);
        achive_file_path = g_achive_folder + _T("\\") + temp_serial;

        Save_Product_Value_Cache(achive_file_path);
    }



    return 1;
}


CMainFrame::CMainFrame()
{
    
    theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
    m_nStyle=0;
	m_pFreshTree = NULL;
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
    list_mouse_click = false;
    //////////////////////////////////////////////////////////////////////////
    m_bEnableRefreshTreeView = TRUE;
    m_pRefreshThread = NULL;

    m_pWaitScanDlg = NULL;
    m_pScanner = NULL;
    FistWdb = TRUE;
    m_isCM5=FALSE;
    FlagSerialNumber = 0;
    m_frist_start=TRUE;
    m_bDialogOpen = FALSE;
    mbPollDlgOpen = FALSE;



    m_hInternetSession = NULL;
    m_hHttpConnection = NULL;
    m_hHttpFile = NULL;

    m_bSafeToClose = TRUE;
    m_pDialogInfo = NULL;

	isp_mode_is_cancel = true;
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


}
void CMainFrame::InitViews()
{
    m_nCurView = 0;
    CView* pActiveView = GetActiveView();
    if(pActiveView==NULL)
        return;
    m_pViews[DLG_T3000_VIEW] = (CView*) new CT3000View();  //Tstat user interface;
    //m_pViews[DLG_T3000_VIEW] = pActiveView;
    m_pViews[DLG_NETWORKCONTROL_VIEW]=(CView*) new CNetworkControllView();
    m_pViews[DLG_GRAPGIC_VIEW]=(CView*) new CGraphicView();
    m_pViews[DLG_TRENDLOG_VIEW]=(CView*) new CTrendLogView();
    m_pViews[DLG_DIALOGCM5_VIEW]=  NULL; //(CView*) new CDialogCM5(); //CM5
    m_pViews[DLG_DIALOGT3_VIEW]=(CView*) new CDialogT3();  //T3
    m_pViews[DLG_DIALOGMINIPANEL_VIEW]= NULL;//Mini Panel
    m_pViews[DLG_AIRQUALITY_VIEW]=(CView*) new CAirQuality;//AirQuality
    m_pViews[DLG_LIGHTINGCONTROLLER_VIEW]=(CView*) new CLightingController;//Lightingcontroller
    m_pViews[DLG_HUMCHAMBER] = NULL;
    m_pViews[DLG_CO2_VIEW]=(CView*) new CCO2_View;
    m_pViews[DLG_CO2_NET_VIEW]=(CView*)new CCO2NetView;
    m_pViews[DLG_BACNET_VIEW]=(CView*) new CDialogCM5_BacNet; //CM5
    m_pViews[DLG_DIALOGT38I13O_VIEW]=(CView*) new T38I13O;
    m_pViews[DLG_DIALOGT332AI_VIEW]=(CView*)new T332AI;
    m_pViews[DLG_DIALOGT38AI8AO]=(CView*)new T38AI8AO;
    m_pViews[DLG_DIALOGT36CT]=(CView*)new T36CT;
    m_pViews[DLG_DIALOGT3PT10]=(CView*)new CT3RTDView;
    m_pViews[DLG_DIALOG_PRESSURE_SENSOR]=(CView*)new CPressureSensorForm;
    m_pViews[DLG_DIALOG_DEFAULT_BUILDING]=(CView*)new CT3000DefaultView;
    m_pViews[DLG_DIALOG_TEMP_HUMSENSOR]=(CView*)new CTempHumSensorForm;
    m_pViews[DLG_DIALOG_DEFAULT_T3000_VIEW] = (CView * )new T3000_Default_MainView;
    m_pViews[DLG_DIALOG_T3_INPUTS_VIEW] = (CView * )new CT3ModulesView;
    m_pViews[DLG_DIALOG_T3_OUTPUTS_VIEW] = (CView * )new CT3ModulesOutputDlg;
    m_pViews[DLG_DIALOG_CUSTOM_VIEW]  = (CView * )new CProductRegisterListView;
    m_pViews[DLG_DIALOG_TSTAT_INPUT_VIEW]=(CView *)new CTStatInputView;
    m_pViews[DLG_DIALOG_TSTAT_OUTPUT_VIEW]=(CView *)new CTStatOutputView;
    m_pViews[DLG_DIALOG_BOATMONITOR] = (CView *)new CBoatMonitorViewer;
	m_pViews[DLG_DIALOG_BTUMETER] = (CView *)new CBTUMeterDlg;
    m_pViews[DLG_DIALOG_POWERMETER] = (CView *)new CPowermeter;
    m_pViews[DLG_DIALOG_CO2_NODE] = (CView *)new CCO2_NodeView;
    m_pViews[DLG_DIALOG_ZIGBEE_REPEATER] = NULL /*(CView *)new CZigbeeRepeater;*/;
    m_pViews[DLG_DIALOG_TSTAT_AQ] = NULL /*(CView *)new CTstatAQ;*/;
    m_pViews[DLG_DIALOG_THIRD_PARTY_BAC] = NULL /*(CView *)new CBacnetThirdPartyMain;*/;
    CDocument* pCurrentDoc = GetActiveDocument();
    CCreateContext newContext;
    newContext.m_pNewViewClass = NULL;
    newContext.m_pNewDocTemplate = NULL;
    newContext.m_pLastView = NULL;
    newContext.m_pCurrentFrame = NULL;
    newContext.m_pCurrentDoc = pCurrentDoc;

    CRect rect(0, 0, 0, 0);

    for (int nView =0; nView < NUMVIEWS; nView++)
    {
        if(nView == DLG_DIALOGCM5_VIEW||nView == DLG_DIALOGMINIPANEL_VIEW || nView == DLG_HUMCHAMBER)
            continue;
        if (nView >= DLG_DIALOG_ZIGBEE_REPEATER)  //从31以后得View 不在提前创建，否则浪费太多系统资源;改为何时需要何时创建;
        {
            continue;
        }
        m_pViews[nView]->Create(NULL, NULL,
                                (AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
                                rect, this,
                                AFX_IDW_PANE_FIRST + nView, &newContext);

        m_pViews [nView]->OnInitialUpdate();//?
    }
	((CDialogCM5_BacNet *)m_pViews[DLG_BACNET_VIEW])->InitialBacnetWindow();
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

void CMainFrame::ShowSplashWnd(int nMillisecond)
{
    //m_strLogoFileName = g_strExePth + _T("ResourceFile\\") + _T("SplashScreen.jpg") ;
    m_strLogoFileName = g_strExePth + _T("ResourceFile\\") + _T("CPR_Bestek.jpg");
    CFileFind temp_find;
    CString temp_db_ini_folder;
    temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
    if (temp_find.FindFile(m_strLogoFileName))
    {
        WritePrivateProfileString(_T("SpecialFlag"), _T("Customer_SoftName"), _T("CPR-1000.exe"), temp_db_ini_folder);
        m_special_customer = 1;
        cs_special_name = _T("CPR-1000");
        CSplashWnd::SetSplashPicFilePath(m_strLogoFileName);
        CSplashWnd::EnableSplashScreen();
        CSplashWnd::ShowSplashScreen(this);


        Sleep(nMillisecond);
        CSplashWnd::endSplashScreen();
    }
    else
    {
        WritePrivateProfileString(_T("SpecialFlag"), _T("Customer_SoftName"), _T("T3000.exe"), temp_db_ini_folder);
        cs_special_name = _T("T3000 Building Automation System");
    }
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

    if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;
    CString temp_bacnet_logfile;
    temp_bacnet_logfile = g_achive_folder + _T("\\bacnetlog.txt");
    char temp_path[512] = { 0 };
    WideCharToMultiByte(CP_ACP, 0, temp_bacnet_logfile.GetBuffer(), -1, temp_path, 512, NULL, NULL);
    set_bacnet_log_path(temp_path);
    system_connect_info.mstp_status = 0;
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
    SEND_COMMAND_DELAY_TIME = 100;
    srand(time(NULL));




    CMFCPopupMenu::SetForceMenuFocus(FALSE);
    CMFCToolBar::SetSizes(CSize(32,32),CSize(32,32));
    CMFCToolBar::SetMenuSizes(CSize(32,32),CSize(24,24));

    UINT uiToolbarHotID;
    UINT uiToolbarColdID;
    UINT uiMenuID;

    uiToolbarHotID =  IDB_BITMAP7 ;
    uiToolbarColdID = IDB_BITMAP7;
    uiMenuID =	IDB_BITMAP_BACNET_MENU_BIT ;//
	


    if (!m_testtoolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),IDR_TOOLBAR_BACNET) ||
            !m_testtoolbar.LoadToolBar(IDR_TOOLBAR_BACNET,uiToolbarColdID, uiMenuID, FALSE /* Not locked */, IDB_BITMAP_TOOLBAR_DISABLE, 0, uiToolbarHotID))

    {
        TRACE0("Failed to create toolbar\n");
        return -1;//fail to create
    }


    ///////////////////////////////////////////////////////////////////////////////////

    if (!m_wndWorkSpace.Create (_T("Building View"), this, CRect (0, 0, 300, 800),
                                TRUE, ID_VIEW_WORKSPACE,
                                WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN  | CBRS_LEFT ))//| CBRS_FLOAT_MULTI
    {
        TRACE0("Failed to create Workspace bar\n");

        return FALSE;// fail to create
    }
    Initial_Function();
	Inial_Product_map();
    Inial_Product_Reglist_map();
    Inial_Product_Menu_map();
    Inial_Product_Input_map();
    //////////////////////////////////////////////////////////////////////////////////
    CString strToolBarName;
    bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
    ASSERT(bNameValid);
    //m_wndToolBar.SetWindowText(strToolBarName);


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

    //  Delete these five lines if you don't want the toolbar and menubar to be dockable
    //m_testtoolbar.EnableDocking(CBRS_ALIGN_ANY);

    m_wndWorkSpace.EnableDocking(CBRS_ALIGN_LEFT);


    EnableDocking(CBRS_ALIGN_ANY);

    DockPane(&m_wndMenuBar);
    DockPane (&m_wndWorkSpace);

    DockPane(&m_testtoolbar);


    // enable Visual Studio 2005 style docking window behavior
    CDockingManager::SetDockingMode(DT_SMART);
    // enable Visual Studio 2005 style docking window auto-hide behavior
    EnableAutoHidePanes(CBRS_ALIGN_ANY);


    m_wndStatusBar.SetPaneInfo(0,ID_RW_INFO,SBPS_NOBORDERS,   300);
    m_wndStatusBar.SetPaneInfo(1,ID_BUILDING_INFO,SBPS_NOBORDERS, 300);
    m_wndStatusBar.SetPaneInfo(2,ID_PROTOCOL_INFO,SBPS_STRETCH | SBPS_NOBORDERS , 0);
    m_wndStatusBar.SetPaneInfo(3,IDS_SHOW_RESULTS,SBPS_NOBORDERS , 1000);
    m_wndStatusBar.EnablePaneDoubleClick ();



    m_pTreeViewCrl=&m_wndWorkSpace.m_TreeCtrl;
    m_pTreeViewCrl->SetExtendedStyle(TVS_EDITLABELS, TVS_EDITLABELS);
    MainFram_hwd = this->m_hWnd;

    //  2011,7,4, 先判断是否第一次运行，是否要导入数据库。
    //ImportDataBaseForFirstRun();
    g_configfile_path =g_strExePth + g_strStartInterface_config;
    g_selected_serialnumber=0;//GetPrivateProfileInt(_T("T3000_START"),_T("SerialNumber"),0,g_configfile_path);
    m_lastinterface=19;//GetPrivateProfileInt(_T("T3000_START"),_T("Interface"),19,g_configfile_path);
    if (m_lastinterface != 19 && m_lastinterface!=24)
    {
        CString achive_file_path;
        CString temp_serial;
        temp_serial.Format(_T("%u.prog"),g_selected_serialnumber);
        achive_file_path = g_achive_folder + _T("\\") + temp_serial;
        Load_Product_Value_Cache(achive_file_path);
        g_tstat_id = product_register_value[6];
    }


    ShowSplashWnd(4000);
//	Check_Local_TemcoUpdate();

    ScanTstatInDB();
   // DeleteConflictInDB();//用于处理数据库中重复的数据，这些数据有相同的序列号;
    PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);

    SetTimer(MONITOR_MOUSE_KEYBOARD_TIMER,1000,NULL);
     m_pRefreshThread =(CRefreshTreeThread*) AfxBeginThread(RUNTIME_CLASS(CRefreshTreeThread));
     m_pRefreshThread->SetMainWnd(this);






   
    BuildingComportConfig();//用于 更改 Config里面的 Comport项;


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

    //Register热键
#ifdef _DEBUG //debug版本   
    int nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[0],MOD_ALT,'I');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + I failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[1],MOD_ALT,'O');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + O failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[2],MOD_ALT,'V');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + V failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[3],MOD_ALT,'P');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + P failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[5],MOD_ALT,'L');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + L failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[6],MOD_ALT,'G');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + G failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[7],MOD_ALT,'S');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + S failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[8],MOD_ALT,'H');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + H failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[9],MOD_ALT,'T');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + T failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[10],MOD_ALT,'A');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + A failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[11],MOD_ALT,'C');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + C failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[4],MOD_ALT,'R');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + R failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[12],MOD_ALT,'B');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + R failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[13],MOD_ALT,'D');
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey ALT + R failure"));
#ifdef USE_MOD_SHIFT_DF
    nRet = RegisterHotKey(GetSafeHwnd(),1111,(MOD_SHIFT | MOD_CONTROL | MOD_ALT),'D'); //热键 ctrl + alt + shift + D
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey MOD_SHIFT + D failure"));
    nRet = RegisterHotKey(GetSafeHwnd(),1112,(MOD_SHIFT | MOD_CONTROL | MOD_ALT),'F'); //热键 ctrl + alt + shift + F
    if(!nRet)
        AfxMessageBox(_T("RegisterHotKey MOD_SHIFT + F failure"));
#endif // USE_MOD_SHIFT_DF
#else //release版本   
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[0],MOD_ALT,'I');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[1],MOD_ALT,'O');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[2],MOD_ALT,'V');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[3],MOD_ALT,'P');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[4],MOD_ALT,'R');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[5],MOD_ALT,'L');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[6],MOD_ALT,'G');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[7],MOD_ALT,'S');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[8],MOD_ALT,'H');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[9],MOD_ALT,'T');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[10],MOD_ALT,'A');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[11],MOD_ALT,'C');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[12],MOD_ALT,'B');
    RegisterHotKey(GetSafeHwnd(),m_MainHotKeyID[13],MOD_ALT,'D');
#ifdef USE_MOD_SHIFT_DF
    RegisterHotKey(GetSafeHwnd(),1111,(MOD_SHIFT | MOD_CONTROL | MOD_ALT),'D');
    RegisterHotKey(GetSafeHwnd(),1112,(MOD_SHIFT | MOD_CONTROL | MOD_ALT),'F');
#endif // USE_MOD_SHIFT_DF
#endif
    for(int i=0; i<WINDOW_TAB_COUNT; i++)
    {
        pDialog[i]=NULL;
    }

	CString temp_ApplicationFolder;
    GetModuleFileName(NULL, temp_ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(temp_ApplicationFolder.GetBuffer(MAX_PATH));
    temp_ApplicationFolder.ReleaseBuffer();
    AutoFlashConfigPath = temp_ApplicationFolder + _T("//AutoFlashFile.ini");
    CFileFind fFind;
    if(fFind.FindFile(AutoFlashConfigPath))
    {
        DeleteFile(AutoFlashConfigPath);
    }

    MyRegAddress.MatchMoudleAddress();
    bac_net_initial_once = false;

    if (!bac_net_initial_once)
    {
        bac_net_initial_once = true;
        timesec1970 = (unsigned long)time(NULL);
        timestart = 0;
        init_info_table();
        Init_table_bank();
    }

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
    h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
    h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

#endif

    // 需要执行线程中的操作时
#if 0      //2018 0409 fandu 屏蔽
    if (m_lasttime_tree_node.protocol == Modbus_Serial)
    {
        open_com(m_lasttime_tree_node.ncomport);
        SetCommunicationType(0);
    }
    else  if(m_lasttime_tree_node.protocol == Modbus_TCP)
    {
        Open_Socket2(m_lasttime_tree_node.BuildingInfo.strIp,_wtoi(m_lasttime_tree_node.BuildingInfo.strIpPort));
        SetCommunicationType(1);
    }
    g_protocol =  m_lasttime_tree_node.protocol;

	int ReadID = 0;
	if(g_tstat_id != 0)
		ReadID  = read_one(g_tstat_id,6,10);

    if (ReadID > 0)
    {
        m_pTreeViewCrl->turn_item_image(m_lasttime_tree_node.product_item ,true);
        for (int i = 0; i<(int)m_product.size(); i++)
        {
            if (m_product.at(i).product_item == m_lasttime_tree_node.product_item)
            {

                m_product.at(i).status_last_time[0] = true;
                m_product.at(i).status_last_time[1] = true;
                m_product.at(i).status_last_time[2] = true;
            }

        }

    }
    else
    {
       // m_pTreeViewCrl->turn_item_image(m_lasttime_tree_node.product_item ,false); //2016 03 17 由杜帆屏蔽. 初始化 m_lasttime_tree_node.product_item 的值可能为0xcdcdcdcd. 导致运行错误;
        for (int i = 0; i<(int)m_product.size(); i++)
        {
            if (m_product.at(i).product_item == m_lasttime_tree_node.product_item)
            {

                m_product.at(i).status_last_time[0] = false;
                m_product.at(i).status_last_time[1] = false;
                m_product.at(i).status_last_time[2] = false;
            }

        }
    }
#endif
    if(m_pDialogInfo==NULL)
    {
        m_pDialogInfo = new CDialogInfo;
        m_pDialogInfo->Create(IDD_DIALOG_INFO,this);
        m_pDialogInfo->ShowWindow(SW_HIDE);
        m_pDialogInfo->CenterWindow();
        m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T(""));
    }


    PostMessage(WM_CREATE_STATUSBAR,0,0);
    CString image_fordor;
    image_fordor = temp_ApplicationFolder + _T("\\Database\\Buildings\\") + m_strCurMainBuildingName + _T("\\image");
    CFileFind temp_find;
    if(temp_find.FindFile(image_fordor) == 0)
    {
        CreateDirectory(image_fordor,NULL);
    }
	hThread = CreateThread(NULL, NULL, Get_All_Dlg_Message, this, NULL, &nThreadID);
	hDeal_thread = CreateThread(NULL, NULL, Translate_My_Message, this, NULL, &nThreadID_Do);
	//m_pFreshMultiRegisters = AfxBeginThread(_ReadMultiRegisters,this);
	m_pFreshTree=AfxBeginThread(_FreshTreeView, this);

	SetTimer(FOR_LAST_VIEW_TIMER,2000,NULL);

	CString	configfile_path = g_strExePth + _T("T3000_config.ini");
	int isoffline = GetPrivateProfileInt(_T("ONOFFLINE_MODEL"), _T("ISONLINE"), -1, configfile_path);

	if (isoffline == 1)
	{
		offline_mode = true;
		m_pTreeViewCrl->SetTreeOfflineMode(offline_mode);
		m_pTreeViewCrl->SetTimer(1, 500, NULL);
		//CreateOfflinePrgFile();
		set_offline_mode(offline_mode);
	}
	
    if (h_mul_ping_thread == NULL)
    {
        h_mul_ping_thread = CreateThread(NULL, NULL, Mul_Ping_Thread, this, NULL, NULL);
    }

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWndEx::PreCreateWindow(cs) )
        return FALSE;
    //  Modify the Window class or styles here by modifying
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


     g_SelectChanged = TRUE;
    b_pause_refresh_tree = TRUE;
    Flexflash = TRUE;
    HTREEITEM hSelItem;//=m_pTreeViewCrl->GetSelectedItem();
//   int nRet =read_one(g_tstat_id,6,1);

    CPoint pt;
    GetCursorPos(&pt);
    m_pTreeViewCrl->ScreenToClient(&pt);
    hSelItem = m_pTreeViewCrl->HitTest(pt);
    if(hSelItem == NULL)
        return;
    RECT r;
    if (pt.x <= 104)  //点击展开或者折叠时，不会像以前一样还拼命加载界面;
    {
       
        unsigned char expand_status = 0;
        CString temp_serialnumber = 0;


        for (UINT i = 0; i < m_product.size(); i++)
        {
            if (hSelItem == m_product.at(i).product_item)
            {
                temp_serialnumber.Format(_T("%u"), m_product.at(i).serial_number);
                
                expand_status = (unsigned char)GetPrivateProfileInt(temp_serialnumber, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
                if (expand_status == 2)
                {
                    WritePrivateProfileString(temp_serialnumber, _T("Expand"), _T("1"), g_ext_database_path);
                    m_product.at(i).expand = 1;
                }
                else
                {
                    WritePrivateProfileString(temp_serialnumber, _T("Expand"), _T("2"), g_ext_database_path);
                    m_product.at(i).expand = 2;
                }
                return;
            }
        }
    }
        

    //////////////////////////////////////////////////////////////////////////
    m_strCurSelNodeName = m_pTreeViewCrl->GetItemText(hSelItem);
    BeginWaitCursor();
    //CM5
    g_bChamber=FALSE;
    m_isMiniPanel=FALSE;
    CString g_strT3000LogString;

	g_bPauseMultiRead=FALSE;
#if 1
    for(UINT i=0; i<m_product.size(); i++)
    {
        if(hSelItem==m_product.at(i).product_item )
        {
            /*  int const PM_CS_SM_AC = 36;
              int const PM_CS_SM_DC = 37;
              int const PM_CS_RSM_AC = 38;
              int const PM_CS_RSM_DC = 39;*/
			custom_bacnet_register_listview = false;
            CString Product_Custom = m_product.at(i).Custom;
            m_current_tree_node = m_product.at(i);
            if (Product_Custom.CompareNoCase(_T("1")) == 0)
            {
                g_tstat_id = m_product.at(i).product_id;
                g_selected_serialnumber = m_product.at(i).serial_number;
                SwitchToPruductType(DLG_DIALOG_CUSTOM_VIEW);
                return;
            }

            g_strT3000LogString.Format(_T("Trying to connect to %s:%d"),GetProductName(m_product.at(i).product_class_id),m_product.at(i).serial_number);


            CString* pstrInfo = new CString(g_strT3000LogString);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
            if(m_product.at(i).product_class_id == PM_CM5)
            {
				g_bPauseMultiRead=TRUE;
                g_tstat_id=m_product.at(i).product_id;
                m_isCM5=TRUE;

                DoConnectToANode(hSelItem);
                break;
            }
            else if (m_product.at(i).product_class_id == PM_MINIPANEL|| m_product.at(i).product_class_id == PM_MINIPANEL_ARM)
            {
				g_bPauseMultiRead=TRUE;
                g_tstat_id = m_product.at(i).product_id;
                m_isMiniPanel=TRUE;
                DoConnectToANode(hSelItem);
                break;
            }
            else if (m_product.at(i).product_class_id == PM_T3IOA)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;

            }

            else if (m_product.at(i).product_class_id == PM_T38I13O)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
            }
            else if (m_product.at(i).product_class_id == PM_T34AO) //T3
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;

            }
            else if (m_product.at(i).product_class_id == PM_T322AI) //T3
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;

            }
			else if (m_product.at(i).product_class_id == PM_T3PT12) //T3
			{
				g_tstat_id = m_product.at(i).product_id;
				DoConnectToANode(hSelItem);
                break;

			}
			else if (m_product.at(i).product_class_id == PM_T36CTA
				||m_product.at(i).product_class_id == PM_T3_LC
				) //T3
			{
				g_tstat_id = m_product.at(i).product_id;
				DoConnectToANode(hSelItem);
                break;

			}
            else if (m_product.at(i).product_class_id == PM_T38AI8AO6DO) //T3
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;

            }
            else if (m_product.at(i).product_class_id == PM_T3PT10)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
            }
            else if (m_product.at(i).product_class_id==PM_T332AI)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
            }
            else if (m_product.at(i).product_class_id ==PM_AirQuality) //AirQuality
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
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
                break;
            }
            else if (m_product.at(i).product_class_id==PM_TSTAT6_HUM_Chamber)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
            }
            else if (m_product.at(i).product_class_id==PM_T38I13O)
            {
                g_tstat_id=m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;

            }
            else if (m_product.at(i).product_class_id==PM_T36CT)
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
            }
            else
            {
                g_tstat_id = m_product.at(i).product_id;
                DoConnectToANode(hSelItem);
                break;
            }

        }
    }
#endif
    

    EndWaitCursor();
    b_pause_refresh_tree = FALSE;
	 g_SelectChanged = FALSE;
}


void CMainFrame::OnHTreeItemEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{


}

LRESULT CMainFrame::OnHTreeItemBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{

    return 0;
}

void  CMainFrame::OnHTreeMessageSeletedChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
}

void  CMainFrame::OnHTreeItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    if(list_mouse_click)
    {
        list_mouse_click = true;
    }
    OnHTreeItemSeletedChanged(pNMHDR, pResult);
    list_mouse_click = false;
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
	CppSQLite3Table table;
	CppSQLite3Query q;

	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

    for (int i=0; i<(int)m_product.size(); i++)
    {
        for (int j=i+1; j<(int)m_product.size(); j++)
        {
            if(m_product.at(i).serial_number == m_product.at(j).serial_number)
            {
                find_conflict = true;

                CString mtemp_serial_number;
                CString strSql;
                mtemp_serial_number.Format(_T("%u"),m_product.at(i).serial_number);
                try
                {
                    strSql.Format(_T("delete   from ALL_NODE where Serial_ID ='%s'"), mtemp_serial_number);
                    SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                }
                catch(_com_error *e)
                {
                    //AfxMessageBox(e->ErrorMessage());
                }


                break;
            }
        }
    }
    SqliteDBBuilding.closedb();
    if(find_conflict)
        PostMessage(WM_MYMSG_REFRESHBUILDING,0,0);
}

void CMainFrame::LoadProductFromDB()
{
	ClearBuilding();
	m_pTreeViewCrl->DeleteAllItems();
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

	m_product.clear();
	CString strSql;
	strSql.Format(_T("select * from Building where Default_SubBuilding=1"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	table = SqliteDBT3000.getTable((UTF8MBSTR)strSql);
	int count =table.numRows();

    CString temp_folder;
    temp_folder = g_strExePth + _T("Database\\Buildings\\") + _T("external_data.ini");
    g_ext_database_path = temp_folder;

	_variant_t temp_variant;
	if(count<=0)
	{
		SqliteDBT3000.closedb();
		AfxMessageBox(_T("There is no default building, please select a building Firstly!"));
		return;
	}

	m_strCurMainBuildingName=q.getValuebyName(L"Main_BuildingName");
	CString cs_temp_protocol;
	CString cs_temp_baudrate;
	cs_temp_protocol=q.getValuebyName(L"Protocal");//
	if(!cs_temp_protocol.IsEmpty())
	{
		//   cs_temp_protocol=temp_variant;
	}
	else
	{
		cs_temp_protocol=_T("Auto");
		strSql.Format(_T("update Building set Protocal='Auto' where Main_BuildingName ='%s'"), m_strCurMainBuildingName);
		SqliteDBT3000.execDML((UTF8MBSTR)strSql);
	}
	if(cs_temp_protocol.CompareNoCase(_T("Remote Device")) == 0)
	{
		b_remote_connection = true;
	}
	else
	{
		b_remote_connection = false;
	}

	current_building_protocol = P_AUTO;

	if(cs_temp_protocol.CompareNoCase(_T("Bacnet MSTP")) == 0 )
	{
		current_building_protocol = P_BACNET_MSTP;
	}

	CString StrComport;
	CString StrBaudrate;


	CString StrIp;
	CString StrPort;
	if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP))
	{


		cs_temp_baudrate=q.getValuebyName(L"Braudrate");//

		if(!cs_temp_baudrate.IsEmpty())
		{
			cs_temp_baudrate = temp_variant;
		}
		else
		{
			cs_temp_protocol=_T("19200");
			strSql.Format(_T("update Building set Braudrate='19200' where Main_BuildingName ='%s'"), m_strCurMainBuildingName);
			SqliteDBT3000.execDML((UTF8MBSTR)strSql);
		}

		current_building_baudrate = _wtoi(cs_temp_baudrate);

		StrComport =q.getValuebyName(_T("Com_Port"));
		if(!StrComport.IsEmpty())
		{
			StrComport=temp_variant;
			current_building_comport = _wtoi(StrComport.Mid(3));
		}
		else
		{
			current_building_comport=-1;
		}

	}

	StrIp=q.getValuebyName(L"Ip_Address");//
	if(!StrIp.IsEmpty())
	{
		//StrIp = temp_variant;
		m_str_curBuilding_Domain_IP = StrIp;
	}
	else
	{
		m_str_curBuilding_Domain_IP.Empty();
	}


	StrPort=q.getValuebyName(L"Ip_Port");//
	if(!StrPort.IsEmpty())
	{
		//StrIp = temp_variant;
		m_str_curBuilding_Domain_Port = StrPort;
	}
	else
	{
		m_str_curBuilding_Domain_Port.Empty();
	}

	m_subNetLst.clear();

	strSql.Format(_T("select * from Building where Main_BuildingName ='%s'"),m_strCurMainBuildingName);
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	m_nCurSubBuildingIndex=-1;
	int nTemp=-1;
	while(!q.eof())
	{
		nTemp++;
		int nDefault=0;
		Building_info temBuildingInfo;

		temBuildingInfo.strBuildingName=q.getValuebyName(L"Building_Name");




		temBuildingInfo.strProtocol=q.getValuebyName(L"Protocal");


		temBuildingInfo.strIp=q.getValuebyName(L"Ip_Address");



		temBuildingInfo.strComPort=q.getValuebyName(L"Com_Port");


		temBuildingInfo.strIpPort=q.getValuebyName(L"Ip_Port");




		temBuildingInfo.hCommunication=NULL;


		temBuildingInfo.strMainBuildingname=q.getValuebyName(L"Main_BuildingName");




		temBuildingInfo.strBaudRate=q.getValuebyName(L"Braudrate");



		CString Building_DBpath;
		nDefault=q.getIntField("Default_SubBuilding");
		if(nDefault==1)
		{
			m_nCurSubBuildingIndex=nTemp;
		}
		Building_DBpath=q.getValuebyName(L"Building_Path");


		g_strCurBuildingDatabasefilePath  = g_strExePth+ Building_DBpath;


		q.nextRow();
		m_subNetLst.push_back(temBuildingInfo);
	}
	if(m_subNetLst.size()<=0)
		AfxMessageBox(_T("There is no default building,please select a building First."));



	TV_INSERTSTRUCT tvInsert;////added

	m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;

	CString strBuilding=m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;

	tvInsert.hParent = TVI_ROOT; // 指定父句柄
	tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
	tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strBuilding;
	tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
	TVINSERV_BUILDING
		HTREEITEM hTreeSubbuilding=NULL;
	hTreeSubbuilding=m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入subbuilding。
	// Expand the parent, if possible.
	HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hTreeSubbuilding);
	if (hParent != NULL)
		m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);




	HTREEITEM hlocalnetwork=NULL;
	//要求选择远程连接的时候 不显示 本地的设备.
	//潜在问题是客户一不小心选中remote 后 ，这样改会出现 扫描不到的情况.即使扫描到了 ，要求不显示本地的设备，客户会抱怨 扫不到.
	if(b_remote_connection == false)
	{
		if((current_building_protocol == P_MODBUS_TCP) || (current_building_protocol == P_AUTO))
		{
			CString strNetWrokName= _T("Local Network");
			///*********tree***********************************
			tvInsert.hParent = hTreeSubbuilding; // 指定父句柄
			tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象

			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strNetWrokName;
			tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
			TVINSERV_FLOOR


				hlocalnetwork=m_pTreeViewCrl->InsertItem(&tvInsert);//返回楼层的句柄
		}
	}

	HTREEITEM hrootserialport=NULL;
	if(b_remote_connection == false)
	{
		if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP) || (current_building_protocol == P_AUTO))
		{
			CString strSerialRootName= _T("Serial Port");
			///*********tree***********************************
			tvInsert.hParent = hTreeSubbuilding; // 指定父句柄
			tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象

			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strSerialRootName;
			tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
			TVINSERV_FLOOR


				hrootserialport=m_pTreeViewCrl->InsertItem(&tvInsert);
		}
	}

	HTREEITEM hrootremote=NULL;
	if(b_remote_connection)
	{
		if((current_building_protocol == P_REMOTE_DEVICE)|| (current_building_protocol == P_AUTO))
		{
			CString strRemoteNetworkName= _T("Remote Network Device");
			///*********tree***********************************
			tvInsert.hParent = hTreeSubbuilding; // 指定父句柄
			tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象

			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strRemoteNetworkName;
			tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
			TVINSERV_FLOOR


				hrootremote=m_pTreeViewCrl->InsertItem(&tvInsert);
		}
	}

	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	const int COM_SERIAL_PORT = 0;
	const int LOCAL_NETWORK_PORT = 1;
	const int REMOTE_CONNECTION = 2;
	for (int z=0; z<3; z++)
	{
		int nTemp3;
		CString PortName;
		CString temp_show;
		HTREEITEM hcomport=NULL;
		if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP) || (current_building_protocol == P_AUTO))
		{
			if((z == COM_SERIAL_PORT) && (b_remote_connection == false))
			{
				strSql.Format(_T("select DISTINCT Com_Port from ALL_NODE where Building_Name = '%s' ORDER BY Com_Port ASC"),strBuilding);
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

				while(!q.eof())//所有楼层。
				{
					PortName= q.getValuebyName(L"Com_Port");
					int comport_value = _wtoi(PortName);
					if((comport_value > 50) || (comport_value == 0))
					{
						q.nextRow();
						continue;
					}

					temp_show = _T("Com") + PortName;
					tvInsert.hParent = hrootserialport; // 指定父句柄
					tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象

					tvInsert.item.pszText = (LPTSTR)(LPCTSTR)temp_show;
					tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
					TVINSERV_FLOOR


						hcomport=m_pTreeViewCrl->InsertItem(&tvInsert);

					_com_port_tree m_comport_temp;
					m_comport_temp.building_item=hTreeSubbuilding;
					m_comport_temp.serial_port_item =hrootserialport;
					m_comport_temp.each_port_item = hcomport;
					m_comport_temp.strComPortName=PortName;
					m_comportlist.push_back(m_comport_temp);
					q.nextRow();
				}

			}
			if(hrootserialport != NULL)
				m_pTreeViewCrl->Expand(hrootserialport, TVE_EXPAND);

			int loop_count = 0;
			if((z == COM_SERIAL_PORT) && (b_remote_connection == false))
			{
				loop_count = m_comportlist.size();
			}
			else if((b_remote_connection == false) && (z == LOCAL_NETWORK_PORT) && ((current_building_protocol == P_AUTO) || (current_building_protocol == P_MODBUS_TCP) || (current_building_protocol == P_BACNET_IP)))
			{
				loop_count = 1;
			}
			else if((z == REMOTE_CONNECTION) && (b_remote_connection))
			{
				loop_count = 1;
			}

			for (int j=0; j<loop_count; j++)
			{
				CString temp_cs;
				CString temp_parent_serialnum;
				if(z == COM_SERIAL_PORT)
				{
					temp_cs = m_comportlist.at(j).strComPortName;
					strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Com_Port = '%s'"),strBuilding,temp_cs);
				}
				else if(z == LOCAL_NETWORK_PORT)
				{
					strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Protocol <>'6' and cast(Com_Port as decimal(10, 2)) > '50'"),strBuilding);
				}
				else if(z == REMOTE_CONNECTION)
				{
					temp_cs.Format(_T("%u"),PROTOCOL_REMOTE_IP);
					//strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Protocol = '%s'"),strBuilding,temp_cs);
                    strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Com_Port > '50'"), strBuilding);
				}

				q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

				if(!q.eof())
				{
					while(!q.eof())//次Com port 下面的所有 设备;
					{
						if(z == LOCAL_NETWORK_PORT)
						{
							PortName=q.getValuebyName(L"Com_Port");
							int comport_value = _wtoi(PortName);
							if(comport_value <= 50)
							{
								q.nextRow();
								continue;
							}
						}


						temp_parent_serialnum=q.getValuebyName(L"Parent_SerialNum");
						unsigned int temp_int_serial;
						temp_int_serial = (unsigned int)_wtoi(temp_parent_serialnum);
						if((!temp_parent_serialnum.IsEmpty()) && (temp_int_serial != 0))	//说明有父节点;先插入父节点
						{
							q.nextRow();
							continue;
						}

						CString strProdcut=q.getValuebyName(L"Product_name");
						if(z == COM_SERIAL_PORT)
						{
							tvInsert.hParent = m_comportlist.at(j).each_port_item ; // 指定父句柄
						}
						else if(z == LOCAL_NETWORK_PORT)
						{
							tvInsert.hParent = hlocalnetwork;// 指定父句柄 为本地网络;
						}
						else if(z == REMOTE_CONNECTION)
						{
							tvInsert.hParent = hrootremote;// 指定父句柄 为本地网络;
						}
						tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
						tvInsert.item.pszText =(LPTSTR)(LPCTSTR) strProdcut;
						//TRACE(strProdcut);
						tvInsert.hInsertAfter =TVI_SORT;// TVI_LAST; // 项目插入方式
						int temp_product_class_id=q.getIntField("Product_class_ID");
						if(temp_product_class_id==PM_NC|| temp_product_class_id == PM_SOLAR  )
							TVINSERV_NET_WORK
						else if (temp_product_class_id == PM_CM5 ) //CM5
						TVINSERV_CMFIVE
						else if (temp_product_class_id == PM_T3_LC) //CM5
							TVINSERV_T3LC
						else if (temp_product_class_id == PM_T3PT10||
						temp_product_class_id == PM_T3IOA||
						temp_product_class_id == PM_T332AI||
						temp_product_class_id ==  PM_T38AI16O||
						temp_product_class_id == PM_T38I13O||
						temp_product_class_id == PM_T34AO||
						temp_product_class_id == PM_T36CT) //T3
						TVINSERV_NET_WORK
						else if (
							temp_product_class_id == PM_T322AI ||
							temp_product_class_id == PM_T3PT12 ||
							temp_product_class_id == PM_T36CTA ||
							temp_product_class_id == PM_T3_LC ||
							temp_product_class_id == PM_T38AI8AO6DO
							)
							TVINSERV_T3ARM
						else if (temp_product_class_id ==PM_MINIPANEL|| temp_product_class_id == PM_MINIPANEL_ARM)//Mini Panel
						TVINSERV_MINIPANEL
						else if (temp_product_class_id == PM_AirQuality)//AirQuality
							TVINSERV_TSTAT_DEFAULT
						else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
						TVINSERV_LC          //tree0412
						else if (temp_product_class_id == PM_TSTAT7|| temp_product_class_id == PM_TSTAT7_ARM)//TSTAT7 &TSTAT6 //tree0412
							TVINSERV_LED_TSTAT7 //tree0412
						else if(temp_product_class_id == PM_TSTAT6||temp_product_class_id == PM_TSTAT5i)
						TVINSERV_TSTAT6
						else if(temp_product_class_id == PM_TSTAT10 || 
                                temp_product_class_id == PM_TSTAT8  || 
                                temp_product_class_id == PM_TSTAT9 ||
                                temp_product_class_id == PM_TSTAT8_WIFI || 
                                temp_product_class_id == PM_TSTAT8_OCC || 
                                temp_product_class_id == PM_TSTAT_AQ ||
                                temp_product_class_id == PM_TSTAT8_220V)
						TVINSERV_TSTAT8
                        else if(temp_product_class_id == PM_MULTI_SENSOR )
                        TVINSERV_TSTAT8   //暂且用TSTAT8 的图标
                        else if(temp_product_class_id == PM_ZIGBEE_REPEATER)
                        TVINSERV_ZIGBEE_REPEATER
						else if((temp_product_class_id == PM_CO2_NET) ||
                                (temp_product_class_id == PM_CO2_RS485)||
							    (temp_product_class_id == PM_PRESSURE_SENSOR)|| 
							    (temp_product_class_id == STM32_PRESSURE_NET) ||
							    (temp_product_class_id == STM32_PRESSURE_RS3485) ||
							    (temp_product_class_id == STM32_CO2_NET)||
                                (temp_product_class_id == STM32_PM25) ||
							    (temp_product_class_id == STM32_CO2_RS485) ||
                                (temp_product_class_id == STM32_HUM_NET) ||    //2019 03 28 修复 HUM 使用默认TSTAT图标的bug
                                (temp_product_class_id == STM32_HUM_RS485))
						TVINSERV_CO2
						else if (temp_product_class_id == PM_CS_SM_AC||temp_product_class_id == PM_CS_SM_DC||temp_product_class_id == PM_CS_RSM_AC||temp_product_class_id == PM_CS_RSM_DC)
							TVINSERV_CS3000
                        else if(temp_product_class_id == PM_PM5E || temp_product_class_id == PM_PM5E_ARM)
                            TVINSERV_PM5E
                        else if(temp_product_class_id == PM_THIRD_PARTY_DEVICE)
                            TVINSERV_THIRD_PARTY
						else
							TVINSERV_TSTAT_DEFAULT

							HTREEITEM hProductItem=m_pTreeViewCrl->InsertItem(&tvInsert);

                        tree_product m_product_temp;
                        m_product_temp.product_item = hProductItem;

						HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hProductItem);
                        //if (hParent != NULL)
                        //{
                        //    m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
                        //}




						strSql=q.getValuebyName(L"Serial_ID");
						unsigned int correct_id = (DWORD)(_wtoi64(strSql));
						m_product_temp.serial_number= correct_id;

                        if (hParent != NULL)
                        {
                            CString temp_cs_serial;
                            temp_cs_serial.Format(_T("%u"), m_product_temp.serial_number);
                            //如果父节点 是要求折叠的 就不要展开;
                            m_product_temp.expand = (unsigned char)GetPrivateProfileInt(temp_cs_serial, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
                            if (m_product_temp.expand != 2)
                                m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
                            else
                                m_pTreeViewCrl->Expand(hParent, TVE_COLLAPSE); //2就折叠
                        }



						strSql=q.getValuebyName(L"Product_ID");
						m_product_temp.product_id=_wtoi(strSql);


						strSql=q.getValuebyName(L"NetworkCard_Address");
						m_product_temp.NetworkCard_Address=strSql;

						strSql=q.getValuebyName(L"Product_class_ID");
						m_product_temp.product_class_id=_wtoi(strSql);

						strSql=q.getValuebyName(L"Hardware_Ver");
						m_product_temp.hardware_version=(float)_wtof(strSql);


						strSql=q.getValuebyName(L"Software_Ver");
						m_product_temp.software_version=(float)_wtof(strSql);


						strSql=q.getValuebyName(L"EPsize");
						m_product_temp.nhardware_info =(int)_wtol(strSql);


						strSql=q.getValuebyName(L"Protocol");
						m_product_temp.protocol = (int)_wtoi(strSql);


						strSql=q.getValuebyName(L"Bautrate");
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
						{
							m_product_temp.baudrate = 19200;
						}
						////
						int is_online=q.getIntField("Online_Status");//
						if(is_online == 1)
						{
							m_product_temp.status = is_online;
							m_product_temp.status_last_time[4] = m_product_temp.status;
						}
						else
						{
							m_product_temp.status = false;
							m_product_temp.status_last_time[4] = false;
						}
                        m_product_temp.status_last_time[0] = false;
                        m_product_temp.status_last_time[1] = false;
                        m_product_temp.status_last_time[2] = false;
                        m_product_temp.status_last_time[3] = false;


						m_product_temp.Custom=q.getValuebyName(L"Custom");


						m_product_temp.NameShowOnTree =  q.getValuebyName(L"Product_name");




						m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
						//20120423
						if(((strSql.CompareNoCase(_T("19200")) == 0)) || 
                            (strSql.CompareNoCase(_T("9600")) == 0) || 
                            (strSql.CompareNoCase(_T("38400")) == 0) ||
                            (strSql.CompareNoCase(_T("57600")) == 0) ||
                            (strSql.CompareNoCase(_T("115200")) == 0))
						{
							m_product_temp.BuildingInfo.strIp.Empty();
							m_product_temp.BuildingInfo.strBaudRate = strSql;
						}
						else
						{
							m_product_temp.BuildingInfo.strIp = strSql;
						}

						m_product_temp.ncomport=q.getIntField("Com_Port");//

						//m_product_temp.BuildingInfo.strIpPort = _T("10000");
                        m_product_temp.BuildingInfo.strIpPort.Format(_T("%d"), m_product_temp.ncomport);
						m_product_temp.strImgPathName=q.getValuebyName(L"Background_imgID");

						m_product_temp.note_parent_serial_number=q.getIntField("Parent_SerialNum");

						m_product_temp.panel_number=q.getIntField("Panal_Number");


						m_product_temp.object_instance=q.getIntField("Object_Instance");



						m_product.push_back(m_product_temp);
						q.nextRow();

					}
				}


				if(z == COM_SERIAL_PORT)
				{
					strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Com_Port = '%s' and Parent_SerialNum <> '0' and Parent_SerialNum <> ''"),strBuilding,temp_cs);
				}
				else if((z == LOCAL_NETWORK_PORT) )
				{
					strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Parent_SerialNum <> '0' and Parent_SerialNum <> '' and cast(Com_Port as decimal(10, 2)) > '50'"),strBuilding);
				}
				else if(z == REMOTE_CONNECTION)
				{
                    //下面这句因为远程连接的时候 加上协议 6  会出现不显示 子设备的情况 ，所以去掉 2019 05 22
					//strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Parent_SerialNum <> '0' and Parent_SerialNum <> '' and Protocol ='6'"),strBuilding);
                    strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Parent_SerialNum <> '0' and Parent_SerialNum <> ''"), strBuilding);
				}




				q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);


				if(!q.eof())//就说明这个节点下面有 挂在父节点下面的;
				{
					while(!q.eof())
					{
						_variant_t temp_variant_parent;
						CString cs_parents_serial_number;
						unsigned int uint_p_serial_number;
						cs_parents_serial_number=q.getValuebyName(L"Parent_SerialNum");

						uint_p_serial_number = _wtoi(cs_parents_serial_number);
						bool find_parents = false;
						HTREEITEM parents_item;
						for (int z=0; z<m_product.size(); z++)
						{
							if(uint_p_serial_number == m_product.at(z).serial_number)
							{
								find_parents = true;
								parents_item = m_product.at(z).product_item;
								break;
							}
						}

						if(find_parents)
						{
							CString strProdcut=q.getValuebyName(L"Product_name");
							tvInsert.hParent = parents_item ; // 指定父句柄
							tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
							tvInsert.item.pszText =(LPTSTR)(LPCTSTR) strProdcut;
							//TRACE(strProdcut);
							tvInsert.hInsertAfter =TVI_SORT;// TVI_LAST; // 项目插入方式


							int temp_product_class_id=q.getIntField("Product_class_ID");
                            if (temp_product_class_id == PM_NC || temp_product_class_id == PM_SOLAR )
								TVINSERV_NET_WORK
                            else if (temp_product_class_id == PM_PM5E || temp_product_class_id == PM_PM5E_ARM)
                                TVINSERV_PM5E
							else if (temp_product_class_id == PM_CM5) //CM5
								TVINSERV_CMFIVE
							else if (temp_product_class_id == PM_T3_LC) //CM5
								TVINSERV_T3LC
							else if (temp_product_class_id == PM_T3PT10 ||
								temp_product_class_id == PM_T3IOA ||
								temp_product_class_id == PM_T332AI ||
								temp_product_class_id == PM_T38AI16O ||
								temp_product_class_id == PM_T38I13O ||
								temp_product_class_id == PM_T34AO ||
								temp_product_class_id == PM_T36CT) //T3
								TVINSERV_NET_WORK
							else if (
								temp_product_class_id == PM_T322AI ||
								temp_product_class_id == PM_T3PT12 ||
								temp_product_class_id == PM_T36CTA ||
								
								temp_product_class_id == PM_T38AI8AO6DO)
								TVINSERV_T3ARM
							else if (temp_product_class_id == PM_MINIPANEL|| temp_product_class_id == PM_MINIPANEL_ARM)//Mini Panel
								TVINSERV_MINIPANEL
							else if (temp_product_class_id == PM_AirQuality) //AirQuality
								TVINSERV_TSTAT_DEFAULT
							else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
								TVINSERV_LC          //tree0412
							else if (temp_product_class_id == PM_TSTAT7 || temp_product_class_id == PM_TSTAT7_ARM)//TSTAT7 &TSTAT6 //tree0412
								TVINSERV_LED_TSTAT7 //tree0412
							else if (temp_product_class_id == PM_TSTAT6 || temp_product_class_id == PM_TSTAT5i)
								TVINSERV_TSTAT6
							else if (temp_product_class_id == PM_TSTAT10 || 
                                     temp_product_class_id == PM_TSTAT8 || 
                                     temp_product_class_id == PM_TSTAT9 ||
                                     temp_product_class_id == PM_TSTAT8_WIFI || 
                                     temp_product_class_id == PM_TSTAT8_OCC || 
                                     temp_product_class_id == PM_TSTAT_AQ ||
                                     temp_product_class_id == PM_TSTAT8_220V)
								TVINSERV_TSTAT8
                            else if (temp_product_class_id == PM_MULTI_SENSOR)
                                TVINSERV_TSTAT8   //暂且用TSTAT8 的图标
                            else if (temp_product_class_id == PM_ZIGBEE_REPEATER)
                                TVINSERV_ZIGBEE_REPEATER
							else if ((temp_product_class_id == PM_CO2_NET) || (temp_product_class_id == PM_CO2_RS485) ||
								(temp_product_class_id == PM_PRESSURE_SENSOR) ||
								(temp_product_class_id == STM32_PRESSURE_NET) ||
								(temp_product_class_id == STM32_PRESSURE_RS3485) ||
								(temp_product_class_id == STM32_CO2_NET) ||
                                (temp_product_class_id == STM32_PM25) ||
								(temp_product_class_id == STM32_CO2_RS485) ||
                                (temp_product_class_id == STM32_HUM_NET) ||    //2019 03 28 修复 HUM 使用默认TSTAT图标的bug
                                (temp_product_class_id == STM32_HUM_RS485))
								TVINSERV_CO2
							else if (temp_product_class_id == PM_CS_SM_AC || temp_product_class_id == PM_CS_SM_DC || temp_product_class_id == PM_CS_RSM_AC || temp_product_class_id == PM_CS_RSM_DC)
								TVINSERV_CS3000
                            else if (temp_product_class_id == PM_THIRD_PARTY_DEVICE)
                                TVINSERV_THIRD_PARTY
							else
								TVINSERV_TSTAT_DEFAULT

								HTREEITEM hSubItem=m_pTreeViewCrl->InsertItem(&tvInsert);

							HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hSubItem);
							//if (hParent != NULL)
							//	m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);





							tree_product m_product_temp;
							m_product_temp.product_item  =hSubItem;

                            if (hParent != NULL)
                            {
                                CString temp_cs_serial;
                                temp_cs_serial.Format(_T("%u"), uint_p_serial_number);
                                //如果父节点 是要求折叠的 就不要展开;
                                unsigned char temp_expand = 0;
                                temp_expand = (unsigned char)GetPrivateProfileInt(temp_cs_serial, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
                                if (temp_expand != 2)
                                    m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
                                else
                                    m_pTreeViewCrl->Expand(hParent, TVE_COLLAPSE); //2就折叠
                            }

							strSql=q.getValuebyName(L"Serial_ID");

							long temp_serial_id = (long)(_wtoi64(strSql));
							unsigned int correct_id = (DWORD)(_wtoi64(strSql));
							//用于将以前数据库中的 负的序列号 修改为正的;Add by Fance
							//if(temp_serial_id < 0)
							//{
							//	CString wrong_serial_id;
							//	wrong_serial_id = strSql;
							//	CString correct_serial_id;
							//	CString str_temp;
							//	correct_serial_id.Format(_T("%u"),correct_id);
							//	try
							//	{
							//		str_temp.Format(_T("update ALL_NODE set Serial_ID ='%s' where Serial_ID = '%s'"),correct_serial_id, wrong_serial_id);
							//		SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
							//	}
							//	catch(_com_error *e)
							//	{
							//		AfxMessageBox(e->ErrorMessage());
							//	}
							//}
							m_product_temp.serial_number= correct_id;


							strSql=q.getValuebyName(_T("Product_ID"));
							m_product_temp.product_id=_wtoi(strSql);

							strSql=q.getValuebyName(_T("Custom"));
							m_product_temp.Custom=strSql;

							strSql=q.getValuebyName(_T("NetworkCard_Address"));
							m_product_temp.NetworkCard_Address=strSql;

							strSql=q.getValuebyName(L"Product_class_ID");
							m_product_temp.product_class_id=_wtoi(strSql);

							strSql=q.getValuebyName(L"Hardware_Ver");
							m_product_temp.hardware_version=(float)_wtof(strSql);


							strSql=q.getValuebyName(L"Software_Ver");
							m_product_temp.software_version=(float)_wtof(strSql);


							strSql=q.getValuebyName(L"EPsize");
							m_product_temp.nhardware_info =(int)_wtol(strSql);


							strSql=q.getValuebyName(L"Protocol");
							m_product_temp.protocol = (int)_wtoi(strSql);


							strSql=q.getValuebyName(L"Bautrate");
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
							{
								m_product_temp.baudrate = 19200;
							}
							////
							int is_online=q.getIntField("Online_Status");//
							if(is_online == 1)
							{
								m_product_temp.status = is_online;
								m_product_temp.status_last_time[4] = m_product_temp.status;

							}
							else
							{
								m_product_temp.status = false;
								m_product_temp.status_last_time[4] = false;

							}
                            m_product_temp.status_last_time[0] = false;
                            m_product_temp.status_last_time[1] = false;
                            m_product_temp.status_last_time[2] = false;
                            m_product_temp.status_last_time[3] = false;



							m_product_temp.NameShowOnTree=q.getValuebyName(L"Product_name");//



							m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
							//20120423
							if(((strSql.CompareNoCase(_T("19200")) == 0)) || (strSql.CompareNoCase(_T("9600")) == 0) || ((strSql.CompareNoCase(_T("38400")) == 0)))
							{
								m_product_temp.BuildingInfo.strIp.Empty();
								m_product_temp.BuildingInfo.strBaudRate = strSql;
							}
							else
							{
								m_product_temp.BuildingInfo.strIp = strSql;
							}


							m_product_temp.ncomport = q.getIntField("Com_Port");
							m_product_temp.BuildingInfo.strIpPort = _T("10000");
							m_product_temp.strImgPathName=q.getValuebyName(L"Background_imgID");


							m_product_temp.note_parent_serial_number=q.getIntField("Parent_SerialNum");


							m_product_temp.panel_number=q.getIntField("Panal_Number");


							m_product_temp.object_instance=q.getIntField("Object_Instance");


							m_product.push_back(m_product_temp);

						}
						q.nextRow();
					}
				}

			}
		}
	}

	HTREEITEM hrootvirtualdevice=NULL;
	{
		CString strVirtualDevice= _T("Virtual Device");
		///*********tree***********************************
		tvInsert.hParent = hTreeSubbuilding; // 指定父句柄
		tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象

		tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strVirtualDevice;
		tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
		TVINSERV_FLOOR

			hrootvirtualdevice=m_pTreeViewCrl->InsertItem(&tvInsert);
		m_pTreeViewCrl->SetVirtualTreeItem(hrootvirtualdevice);


		CString temp_cs;
		CString temp_parent_serialnum;

		temp_cs.Format(_T("%u"),PROTOCOL_REMOTE_IP);
		strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' and Floor_name = 'Virtual_Floor1'"),strBuilding);


		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

		if(!q.eof())
		{
			while(!q.eof())//次Com port 下面的所有 设备;
			{
				temp_parent_serialnum=q.getValuebyName(L"Parent_SerialNum");
				unsigned int temp_int_serial;
				temp_int_serial = (unsigned int)_wtoi(temp_parent_serialnum);


				CString strProdcut=q.getValuebyName(L"Product_name");

				tvInsert.hParent = hrootvirtualdevice ; // 指定父句柄

				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText =(LPTSTR)(LPCTSTR) strProdcut;
				//TRACE(strProdcut);
				tvInsert.hInsertAfter =TVI_SORT;// TVI_LAST; // 项目插入方式
				int temp_product_class_id=q.getIntField("Product_class_ID");
                if (temp_product_class_id == PM_NC || temp_product_class_id == PM_SOLAR )
					TVINSERV_NET_WORK
                else if (temp_product_class_id == PM_PM5E || temp_product_class_id == PM_PM5E_ARM)
                    TVINSERV_PM5E
				else if (temp_product_class_id == PM_CM5) //CM5
					TVINSERV_CMFIVE
				else if (temp_product_class_id == PM_T3_LC) //CM5
					TVINSERV_T3LC
				else if (temp_product_class_id == PM_T3PT10 ||
					temp_product_class_id == PM_T3IOA ||
					temp_product_class_id == PM_T332AI ||
					temp_product_class_id == PM_T38AI16O ||
					temp_product_class_id == PM_T38I13O ||
					temp_product_class_id == PM_T34AO ||
					temp_product_class_id == PM_T36CT) //T3
					TVINSERV_NET_WORK
				else if (
					temp_product_class_id == PM_T322AI ||
					temp_product_class_id == PM_T3PT12 ||
					temp_product_class_id == PM_T36CTA ||
					
					temp_product_class_id == PM_T38AI8AO6DO)
					TVINSERV_T3ARM
				else if (temp_product_class_id == PM_MINIPANEL|| temp_product_class_id == PM_MINIPANEL_ARM)//Mini Panel
					TVINSERV_MINIPANEL
				else if (temp_product_class_id == PM_AirQuality) //AirQuality
					TVINSERV_TSTAT_DEFAULT
				else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
					TVINSERV_LC          //tree0412
				else if (temp_product_class_id == PM_TSTAT7 || temp_product_class_id == PM_TSTAT7_ARM)//TSTAT7 &TSTAT6 //tree0412
					TVINSERV_LED_TSTAT7 //tree0412
				else if (temp_product_class_id == PM_TSTAT6 || temp_product_class_id == PM_TSTAT5i)
					TVINSERV_TSTAT6
				else if (temp_product_class_id == PM_TSTAT10 || 
                         temp_product_class_id == PM_TSTAT8 || 
                         temp_product_class_id == PM_TSTAT9 ||
                         temp_product_class_id == PM_TSTAT8_WIFI || 
                         temp_product_class_id == PM_TSTAT8_OCC || 
                         temp_product_class_id == PM_TSTAT_AQ ||
                         temp_product_class_id == PM_TSTAT8_220V)
					TVINSERV_TSTAT8
                else if (temp_product_class_id == PM_MULTI_SENSOR)
                    TVINSERV_TSTAT8   //暂且用TSTAT8 的图标
                else if (temp_product_class_id == PM_ZIGBEE_REPEATER)
                    TVINSERV_ZIGBEE_REPEATER
				else if ((temp_product_class_id == PM_CO2_NET) || (temp_product_class_id == PM_CO2_RS485) ||
					(temp_product_class_id == PM_PRESSURE_SENSOR) ||
					(temp_product_class_id == STM32_PRESSURE_NET) ||
					(temp_product_class_id == STM32_PRESSURE_RS3485) ||
                    (temp_product_class_id == STM32_PM25) ||
					(temp_product_class_id == STM32_CO2_NET) ||
					(temp_product_class_id == STM32_CO2_RS485))
					TVINSERV_CO2
				else if (temp_product_class_id == PM_CS_SM_AC || temp_product_class_id == PM_CS_SM_DC || temp_product_class_id == PM_CS_RSM_AC || temp_product_class_id == PM_CS_RSM_DC)
					TVINSERV_CS3000
                else if (temp_product_class_id == PM_THIRD_PARTY_DEVICE)
                    TVINSERV_THIRD_PARTY
				else
					TVINSERV_TSTAT_DEFAULT

					HTREEITEM hProductItem=m_pTreeViewCrl->InsertItem(&tvInsert);

				HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hProductItem);
				//if (hParent != NULL)
				//	m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);

				tree_product m_product_temp;
				m_product_temp.product_item  =hProductItem;

                if (hParent != NULL)
                {
                    CString temp_cs_serial;
                    temp_cs_serial.Format(_T("%u"), temp_int_serial);
                    //如果父节点 是要求折叠的 就不要展开;
                    unsigned char temp_expand = 0;
                    temp_expand = (unsigned char)GetPrivateProfileInt(temp_cs_serial, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
                    if (temp_expand != 2)
                        m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
                    else
                        m_pTreeViewCrl->Expand(hParent, TVE_COLLAPSE); //2就折叠
                }

				strSql=q.getValuebyName(L"Serial_ID");

				long temp_serial_id = (long)(_wtoi64(strSql));
				unsigned int correct_id = (DWORD)(_wtoi64(strSql));
				//用于将以前数据库中的 负的序列号 修改为正的;Add by Fance
				if(temp_serial_id < 0)
				{
					CString wrong_serial_id;
					wrong_serial_id = strSql;
					CString correct_serial_id;
					CString str_temp;
					correct_serial_id.Format(_T("%u"),correct_id);
					try
					{
						str_temp.Format(_T("update ALL_NODE set Serial_ID ='%s' where Serial_ID = '%s'"),correct_serial_id, wrong_serial_id);
						SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
					}
					catch(_com_error *e)
					{
						AfxMessageBox(e->ErrorMessage());
					}
				}
				m_product_temp.serial_number= correct_id;

				strSql=q.getValuebyName(L"Product_ID");
				m_product_temp.product_id=_wtoi(strSql);


				strSql=q.getValuebyName(L"NetworkCard_Address");
				m_product_temp.NetworkCard_Address=strSql;

				strSql=q.getValuebyName(L"Product_class_ID");
				m_product_temp.product_class_id=_wtoi(strSql);

				strSql=q.getValuebyName(L"Hardware_Ver");
				m_product_temp.hardware_version=(float)_wtof(strSql);


				strSql=q.getValuebyName(L"Software_Ver");
				m_product_temp.software_version=(float)_wtof(strSql);


				strSql=q.getValuebyName(L"EPsize");
				m_product_temp.nhardware_info =(int)_wtol(strSql);


				strSql=q.getValuebyName(L"Protocol");
				m_product_temp.protocol = (int)_wtoi(strSql);


				strSql=q.getValuebyName(L"Bautrate");
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
				{
					m_product_temp.baudrate = 19200;
				}
				////
				int is_online=q.getIntField("Online_Status");//
				if(is_online == 1)
				{
					m_product_temp.status = is_online;
					m_product_temp.status_last_time[4] = m_product_temp.status;
				}
				else
				{
					m_product_temp.status = false;
					m_product_temp.status_last_time[4] = false;
				}
                m_product_temp.status_last_time[0] = false;
                m_product_temp.status_last_time[1] = false;
                m_product_temp.status_last_time[2] = false;
                m_product_temp.status_last_time[3] = false;



				m_product_temp.Custom=q.getValuebyName(L"Custom");


				m_product_temp.NameShowOnTree =  q.getValuebyName(L"Product_name");




				m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
				//20120423
				if(((strSql.CompareNoCase(_T("19200")) == 0)) || (strSql.CompareNoCase(_T("9600")) == 0) || ((strSql.CompareNoCase(_T("38400")) == 0)))
				{
					m_product_temp.BuildingInfo.strIp.Empty();
					m_product_temp.BuildingInfo.strBaudRate = strSql;
				}
				else
				{
					m_product_temp.BuildingInfo.strIp = strSql;
				}

				m_product_temp.ncomport=q.getIntField("Com_Port");//

				m_product_temp.BuildingInfo.strIpPort = _T("10000");
				m_product_temp.strImgPathName=q.getValuebyName(L"Background_imgID");

				m_product_temp.note_parent_serial_number=q.getIntField("Parent_SerialNum");

				m_product_temp.panel_number=q.getIntField("Panal_Number");


				m_product_temp.object_instance=q.getIntField("Object_Instance");

				vector <tree_product>::iterator temp_iterator;

				for (temp_iterator = m_product.begin();temp_iterator!=m_product.end();)
				{
					if(temp_iterator->serial_number == m_product_temp.serial_number)
						temp_iterator = m_product.erase(temp_iterator);
					else
						++temp_iterator; 
				}

				m_product.push_back(m_product_temp);
				q.nextRow();

			}
		}
	}
    m_pTreeViewCrl->Expand(hTreeSubbuilding, TVE_EXPAND);


    for (int i = 0; i < (int)m_product.size(); i++)
    {
        if (m_product.at(i).note_parent_serial_number != 0)
        {
            CString ntemp_serial_number;
            ntemp_serial_number.Format(_T("%u"), m_product.at(i).serial_number);
            m_product.at(i).subnet_port =  GetPrivateProfileInt(ntemp_serial_number, _T("Subnet_port"), 0, g_ext_database_path);

            m_product.at(i).subnet_baudrate = GetPrivateProfileInt(ntemp_serial_number, _T("Subnet_baudrate"), 0, g_ext_database_path);

            //m_product.at(i).expand = (unsigned char)GetPrivateProfileInt(ntemp_serial_number, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
        }
    }
	SqliteDBT3000.closedb();
	SqliteDBBuilding.closedb();
}


void CMainFrame::ScanTstatInDB(void)
{
    if(product_sort_way != SORT_BY_BUILDING_FLOOR)
    {
        LoadProductFromDB();
        PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);
        return;
    }
	CppSQLite3DB SqliteDB;
	CppSQLite3Table table;
	CppSQLite3Query q;
    SqliteDB.open((UTF8MBSTR)g_strDatabasefilepath);
    try
    {

        TRACE(_T("Scan TStat In DB!!  \n"));
#if 1
        ClearBuilding();
        m_product.clear();
		CString strSql;
 
        strSql.Format(_T("select * from Building where Default_SubBuilding=1"));
        //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
        table = SqliteDB.getTable("select * from Building where Default_SubBuilding=1");
        int count =table.numRows(); //ado.m_pRecordset->GetRecordCount();
        _variant_t temp_variant;
        if(count<=0)
        {
            SqliteDB.closedb();
            AfxMessageBox(_T("There is no default building, please select a building Firstly!"));

            return;
        }
        m_strCurMainBuildingName=table.getValuebyName("Main_BuildingName");

        CString cs_temp_protocol;
        CString cs_temp_baudrate;
         
		cs_temp_protocol = table.getValuebyName("Protocal");
        if(cs_temp_protocol.IsEmpty())
        {
            cs_temp_protocol=_T("Auto");
            strSql.Format(_T("update Building set Protocal='Auto' where Main_BuildingName ='%s'"), m_strCurMainBuildingName);
            SqliteDB.execDML((UTF8MBSTR)strSql);
        }
		current_building_protocol = P_AUTO;
		

        CString StrComport;
        CString StrBaudrate;


        CString StrIp;
        if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP))
        {
			cs_temp_baudrate = table.getValuebyName("Braudrate");
            if(cs_temp_baudrate.IsEmpty())
            {
                cs_temp_protocol=_T("19200");
                strSql.Format(_T("update Building set Braudrate='19200' where Main_BuildingName ='%s'"), m_strCurMainBuildingName);
                SqliteDB.execDML((UTF8MBSTR)strSql);
            }

            current_building_baudrate = _wtoi(cs_temp_baudrate);
            if((current_building_baudrate!= 19200) && (current_building_baudrate != 9600) && (current_building_baudrate != 38400))
            {
                current_building_baudrate = 19200;
            }
			current_building_comport = table.getIntField("Com_Port",-1);

        }
        if (current_building_protocol == P_MODBUS_TCP || current_building_protocol == P_BACNET_IP || current_building_protocol == P_REMOTE_DEVICE)
        {
           
			m_str_curBuilding_Domain_IP = table.getValuebyName("Ip_Address");

        }
        else
            m_str_curBuilding_Domain_IP.Empty();
        
        m_subNetLst.clear();


        strSql.Format(_T("select * from Building where Main_BuildingName ='%s'"),m_strCurMainBuildingName);
        q = SqliteDB.execQuery((UTF8MBSTR)strSql);
        m_nCurSubBuildingIndex=-1;
        int nTemp=-1;
        while(!q.eof())
        {
            nTemp++;
            int nDefault=0;
            Building_info temBuildingInfo;
            
			temBuildingInfo.strBuildingName = q.getValuebyName(L"Building_Name");
			temBuildingInfo.strProtocol = q.getValuebyName(L"Protocal");
            temBuildingInfo.strIp  = q.getValuebyName(L"Ip_Address"); 
			temBuildingInfo.strComPort = q.getValuebyName(L"Com_Port");
            temBuildingInfo.strIpPort = q.getValuebyName(L"Ip_Port");
            temBuildingInfo.hCommunication=NULL;
			temBuildingInfo.strMainBuildingname = q.getValuebyName(L"Main_BuildingName");
            temBuildingInfo.strBaudRate = q.getValuebyName(L"Braudrate"); 

            CString Building_DBpath;
            nDefault=q.getIntField("Default_SubBuilding");

            if(nDefault==1)
            {
                m_nCurSubBuildingIndex=nTemp;
            }
            Building_DBpath=q.getValuebyName(L"Building_Path");

            int index= Building_DBpath.Find(_T("Database"));
            Building_DBpath.Delete(0,index);

            g_strCurBuildingDatabasefilePath=GetExePath(true)+  Building_DBpath;
            q.nextRow();
            m_subNetLst.push_back(temBuildingInfo);
        }
		if (m_subNetLst.size() <= 0)
		{
			MessageBox(_T("There is no default building,please select a building First."));
		}

        TV_INSERTSTRUCT tvInsert;////added

        m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;

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
         
			SqliteDB.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
         

            if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP))
            {
                strSql.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '%s' and Com_Port = '%d' and Protocol = '%d'"),strBuilding,current_building_comport,current_building_protocol);
            }
            else if(current_building_protocol == P_MODBUS_TCP || current_building_protocol == P_BACNET_IP)
            {
                strSql.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '%s' and Bautrate = '%s' and Protocol = '%d'"),strBuilding,StrIp,current_building_protocol);
            }
            else if(current_building_protocol == P_REMOTE_DEVICE)
            {
                strSql.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '%s' and Bautrate = '%s' and Protocol = '%d'"),strBuilding,StrIp,current_building_protocol);
            }
            else
            {
                strSql.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '%s'"),strBuilding);
            }
 
			q = SqliteDB.execQuery((UTF8MBSTR)strSql);
             
            vector <tree_floor> tmpfloorLst;//
            tmpfloorLst.empty();
            while(!q.eof())//所有楼层。
            {
                CString strFloorName=q.getValuebyName(L"Floor_name");
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

				q.nextRow();
                 
            }
            vector <tree_room> tmproomLst;
            
            for(unsigned int i=0; i<tmpfloorLst.size(); i++)
            {
                //loop for Room Name f
                //	m_pRs->Close();
                CString strFloorName;
                strFloorName=tmpfloorLst.at(i).strFloorName;
                CString temp_str;

                if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP))
                {
                    temp_str.Format(_T("select DISTINCT Room_name from ALL_NODE where Building_Name = '%s' and Floor_name='%s' and Com_Port = '%d' and Protocol = '%d'"),strBuilding,strFloorName,current_building_comport,current_building_protocol);
                }
                else if(current_building_protocol == P_MODBUS_TCP || current_building_protocol == P_BACNET_IP || current_building_protocol == P_REMOTE_DEVICE)
                {
                    temp_str.Format(_T("select DISTINCT Room_name from ALL_NODE where Building_Name = '%s' and Floor_name='%s' and Bautrate = '%s' and Protocol = '%d'"),strBuilding,strFloorName,StrIp,current_building_protocol);
                }
                else
                {
                    temp_str.Format(_T("select DISTINCT Room_name from ALL_NODE where Building_Name = '"+strBuilding+"' and Floor_name='"+strFloorName+"'"));
                }

                q = SqliteDB.execQuery((UTF8MBSTR)temp_str);
                //插入每个房间到相关的楼层。
                while(!q.eof())
                {
                    CString strRoomName;
                    strRoomName=q.getValuebyName(L"Room_name");
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
                    q.nextRow();
                }
                 
            }
            ///////end room nodes//////////////////////////////////////////////////////////////////////
            //////Begin product node/////////////////////////////////////////////////////////////////
            ///每个房间；
            for(UINT i=0; i<tmproomLst.size(); i++)
            {
                //loop for product Name
                //m_pRs->Close();
                CString str_temp;
                CString StrFloorName;
                CString strRoomName;
                strRoomName=tmproomLst.at(i).strRoomName;
                StrFloorName=tmproomLst.at(i).strFloorName;



                if((current_building_protocol == P_MODBUS_485) || (current_building_protocol == P_BACNET_MSTP))
                {
                    str_temp.Format(_T("select * from ALL_NODE where Floor_name = '%s' and  Room_name = '%s' and Building_Name = '%s' and Com_Port = '%d' and Protocol = '%d'  ORDER BY Product_ID ASC"),StrFloorName,strRoomName,strBuilding,current_building_comport,current_building_protocol);
                }
                else if (current_building_protocol == P_MODBUS_TCP || current_building_protocol == P_BACNET_IP || current_building_protocol == P_REMOTE_DEVICE)
                {
                    str_temp.Format(_T("select * from ALL_NODE where Floor_name = '%s' and  Room_name = '%s' and Building_Name = '%s' and Bautrate = '%s' and Protocol = '%d'  ORDER BY Product_ID ASC"),StrFloorName,strRoomName,strBuilding,StrIp,current_building_protocol);
                }
                else
                {
                    str_temp.Format(_T("select * from ALL_NODE where Floor_name = '"+StrFloorName+"' and\
																							  Room_name = '"+strRoomName+"'and Building_Name = '"+strBuilding+"'  ORDER BY Product_ID ASC "));
                }

                
                q = SqliteDB.execQuery((UTF8MBSTR)str_temp);
                
                while(!q.eof())
                {
                    CString strProdcut=q.getValuebyName(L"Product_name");
                    tvInsert.hParent = tmproomLst.at(i).room_item ; // 指定父句柄
                    tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
                    tvInsert.item.pszText =(LPTSTR)(LPCTSTR) strProdcut;
                    //TRACE(strProdcut);
                    tvInsert.hInsertAfter =TVI_SORT;// TVI_LAST; // 项目插入方式
                    int temp_product_class_id=q.getIntField("Product_class_ID");

#if 1
					if (temp_product_class_id == PM_NC || temp_product_class_id == PM_SOLAR )
						TVINSERV_NET_WORK
                    else if (temp_product_class_id == PM_PM5E || temp_product_class_id == PM_PM5E_ARM)
                        TVINSERV_PM5E
					else if (temp_product_class_id == PM_CM5) //CM5
						TVINSERV_CMFIVE
					else if (temp_product_class_id == PM_T3_LC) //CM5
						TVINSERV_T3LC
					else if (temp_product_class_id == PM_T3PT10 ||
						temp_product_class_id == PM_T3IOA ||
						temp_product_class_id == PM_T332AI ||
						temp_product_class_id == PM_T38AI16O ||
						temp_product_class_id == PM_T38I13O ||
						temp_product_class_id == PM_T34AO ||
						temp_product_class_id == PM_T36CT) //T3
						TVINSERV_NET_WORK
					else if (
						temp_product_class_id == PM_T322AI ||
						temp_product_class_id == PM_T3PT12 ||
						temp_product_class_id == PM_T36CTA ||
						temp_product_class_id == PM_T38AI8AO6DO)
						TVINSERV_T3ARM
					else if (temp_product_class_id == PM_MINIPANEL|| temp_product_class_id == PM_MINIPANEL_ARM)//Mini Panel
						TVINSERV_MINIPANEL
					else if (temp_product_class_id == PM_AirQuality) //AirQuality
						TVINSERV_TSTAT_DEFAULT
					else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
						TVINSERV_LC          //tree0412
					else if (temp_product_class_id == PM_TSTAT7 || temp_product_class_id == PM_TSTAT7_ARM)//TSTAT7 &TSTAT6 //tree0412
						TVINSERV_LED_TSTAT7 //tree0412
					else if (temp_product_class_id == PM_TSTAT6 || temp_product_class_id == PM_TSTAT5i)
						TVINSERV_TSTAT6
					else if (temp_product_class_id == PM_TSTAT8 || 
                             temp_product_class_id == PM_TSTAT9 ||
                             temp_product_class_id == PM_TSTAT8_WIFI || 
                             temp_product_class_id == PM_TSTAT8_OCC || 
                             temp_product_class_id == PM_TSTAT_AQ ||
                             temp_product_class_id == PM_TSTAT8_220V)
						TVINSERV_TSTAT8
                    else if (temp_product_class_id == PM_MULTI_SENSOR)
                        TVINSERV_TSTAT8   //暂且用TSTAT8 的图标
                    else if (temp_product_class_id == PM_ZIGBEE_REPEATER)
                        TVINSERV_ZIGBEE_REPEATER
					else if ((temp_product_class_id == PM_CO2_NET) || (temp_product_class_id == PM_CO2_RS485) ||
						(temp_product_class_id == PM_PRESSURE_SENSOR) ||
						(temp_product_class_id == STM32_PRESSURE_NET) ||
						(temp_product_class_id == STM32_PRESSURE_RS3485) ||
						(temp_product_class_id == STM32_CO2_NET) ||
                        (temp_product_class_id == STM32_PM25) ||
						(temp_product_class_id == STM32_CO2_RS485))
						TVINSERV_CO2
					else if (temp_product_class_id == PM_CS_SM_AC || temp_product_class_id == PM_CS_SM_DC || temp_product_class_id == PM_CS_RSM_AC || temp_product_class_id == PM_CS_RSM_DC)
						TVINSERV_CS3000
                    else if (temp_product_class_id == PM_THIRD_PARTY_DEVICE)
                        TVINSERV_THIRD_PARTY
					else
						TVINSERV_TSTAT_DEFAULT
#endif

                    HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertDeviceItem(&tvInsert);

                    HTREEITEM hParent = m_pTreeViewCrl->GetParentItem(hTreeRoom);
                    if (hParent != NULL)
                        m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);


                    tree_product m_product_temp;
                    m_product_temp.product_item  =hTreeRoom;

                    long temp_serial_id = q.getInt64Field("Serial_ID");
                    unsigned int correct_id = (DWORD)(temp_serial_id);

                    //long temp_serial_id = _wtol(strSql);
                    //unsigned int correct_id = (DWORD)(_wtol(strSql));
                    //用于将以前数据库中的 负的序列号 修改为正的;Add by Fance
                    if(temp_serial_id < 0)
                    {
                        CString wrong_serial_id;
                        wrong_serial_id = strSql;
                        CString correct_serial_id;
                        correct_serial_id.Format(_T("%u"),correct_id);
                        try
                        {
                            str_temp.Format(_T("update ALL_NODE set Serial_ID ='%s' where Serial_ID = '%s'"),correct_serial_id, wrong_serial_id);
                            /*bado.m_pConnection->Execute(str_temp.GetString(),NULL,adCmdText);*/
							SqliteDB.execDML((UTF8MBSTR)str_temp);
                        }
                        catch(_com_error *e)
                        {
                            AfxMessageBox(e->ErrorMessage());
                        }
                    }
                    m_product_temp.serial_number= correct_id;

                    m_product_temp.product_id=q.getIntField("Product_ID");

                    m_product_temp.NetworkCard_Address=q.getValuebyName(L"NetworkCard_Address");
                 
                    m_product_temp.product_class_id=q.getIntField("Product_class_ID");

                    m_product_temp.hardware_version=q.getFloatField("Hardware_Ver");
                    m_product_temp.software_version=q.getFloatField("Software_Ver");
                    m_product_temp.nhardware_info =q.getIntField("EPsize");
                    m_product_temp.protocol = q.getIntField("Protocol");
					 strSql= q.getValuebyName(L"Bautrate");
					
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
                    {
                        m_product_temp.baudrate = 19200;
                    }
                    ////
					int temp_status = q.getIntField("Online_Status");
                    
                    if(temp_status == 1)
                    {
                        m_product_temp.status = temp_status;
                        m_product_temp.status_last_time[0] = m_product_temp.status;
                    }
                    else
                    {
                        m_product_temp.status = false;
                        m_product_temp.status_last_time[0] = false;

                    }
                    m_product_temp.status_last_time[1] = false;
                    m_product_temp.status_last_time[2] = false;
                    m_product_temp.status_last_time[3] = false;
                    m_product_temp.status_last_time[4] = false;

					m_product_temp.NameShowOnTree = q.getValuebyName(L"Product_name");

                    m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
//20120423
					if(((strSql.CompareNoCase(_T("9600")) == 0))
						|| (strSql.CompareNoCase(_T("19200")) == 0)  
						|| (strSql.CompareNoCase(_T("38400")) == 0) 
						|| (strSql.CompareNoCase(_T("57600")) == 0) 
						|| ((strSql.CompareNoCase(_T("115200")) == 0)))
                    {
                        m_product_temp.BuildingInfo.strIp.Empty();
                        m_product_temp.BuildingInfo.strBaudRate = strSql;
                    }
                    else
                    {
                        m_product_temp.BuildingInfo.strIp = strSql;
                    }


                     
					m_product_temp.ncomport = q.getIntField("Com_Port");
                    //m_product_temp.BuildingInfo.strIpPort=strSql;
                    m_product_temp.BuildingInfo.strIpPort = _T("10000");

					m_product_temp.strImgPathName = q.getValuebyName(L"Background_imgID");
                     m_product_temp.Custom =    q.getValuebyName(L"Custom");

                    if (g_selected_serialnumber == m_product_temp.serial_number)
                    {
                        m_lasttime_tree_node  =  m_product_temp;
                        m_pTreeViewCrl->SetSelectItem(m_product_temp.product_item);//在线的时候才将颜色变红;
                        m_pTreeViewCrl->SetSelectSerialNumber(m_product_temp.serial_number);
                        m_current_tree_node =   m_product_temp;
                        /*g_selected_serialnumber = m_product.at(i).serial_number;*/
                    }
					m_product_temp.note_parent_serial_number = q.getIntField("Parent_SerialNum");
                    m_product_temp.panel_number = q.getIntField("Panal_Number");
					m_product_temp.object_instance = q.getIntField("Object_Instance");

                    m_product.push_back(m_product_temp);
                    q.nextRow();
                }
            }
            SqliteDB.closedb();
        }

         
#endif

        PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);

        if(m_nCurSubBuildingIndex>=0)
        {
            m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;
        }

    }
    catch (...)
    {
    }
}

void CMainFrame::OnFileOpen()
{
    
    AfxMessageBox(_T("Open configuration file."));
}

#include "Flash_Multy.h"
void CMainFrame::OnBatchFlashHex()
{

    b_pause_refresh_tree = BATCH_FLASH_HEX;
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
                    //connectionSuccessful = 1;
                    m_nStyle=1;
                    CString	g_configfile_path=g_strExePth+_T("T3000_config.ini");
                    CFileFind fFind;
                    if(!fFind.FindFile(g_configfile_path))
                    {
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),build_info.strIp,g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),build_info.strIpPort,g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
                    }
                    else
                    {
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),build_info.strIp,g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),build_info.strIpPort,g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
                    }
                    Invalidate();
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
                    strInfo.Format(_T("COM %d : Not available "), nComPort);
                    SetPaneString(1, strInfo);
                }
                else
                {
                    strInfo.Format(_T("COM %d Connected: Yes"), nComPort);
                    SetPaneString(1, strInfo);
                    Change_BaudRate(default_com1_port_baudrate);
                    m_nStyle=1;
                    CString	g_configfile_path=g_strExePth+_T("T3000_config.ini");
                    CFileFind fFind;
                    if(!fFind.FindFile(g_configfile_path))
                    {
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("0"),g_configfile_path);
                        strInfo.Format(_T("COM%d"),nComPort);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),strInfo,g_configfile_path);
                        strInfo.Format(_T("%d"),nComPort);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),strInfo,g_configfile_path);
                        strInfo.Format(_T("%d"),default_com1_port_baudrate);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),strInfo,g_configfile_path);
                    }
                    else
                    {
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("0"),g_configfile_path);
                        strInfo.Format(_T("COM%d"),nComPort);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),strInfo,g_configfile_path);
                        strInfo.Format(_T("%d"),nComPort);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),strInfo,g_configfile_path);
                        strInfo.Format(_T("%d"),default_com1_port_baudrate);
                        WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),strInfo,g_configfile_path);
                    }

                    Invalidate();
                    //close_com();
                }
            }
        }//endof rs485 checking
    }
}
void CMainFrame::OnDisconnect()
{
    system_connect_info.mstp_status = 0;
    SetCommunicationType(0);
    close_com();
    close_bac_com();
    m_nStyle=2;
    Invalidate();
    CString strInfo = _T("No Connnection");
    SetPaneString(1,strInfo);
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
    if(b)
    {
        strInfo.Format((_T("Open IP:%s successful")),connect_ip);//prompt info;
        SetPaneString(1,strInfo);
        return TRUE;
    }
    else
    {
        strInfo.Format((_T("Open IP:%s failure")),connect_ip);//prompt info;
        SetPaneString(1,strInfo);
        return FALSE;
    }
    return true;
}

BOOL CMainFrame::ConnectSubBuilding(Building_info build_info)
{
    CString g_strT3000LogString;
    CString strInfo;
    if(m_hCurCommunication!=NULL)
    {
        CloseHandle(m_hCurCommunication);
        m_hCurCommunication=NULL;
    }
    if (!((build_info.strIp.CompareNoCase(_T("9600")) ==0)||(build_info.strIp.CompareNoCase(_T("19200"))==0) ||(build_info.strIp.CompareNoCase(_T(""))) == 0))
    {
        g_strT3000LogString.Format(_T("Trying to connect to %s,please wait..."),build_info.strIp);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        UINT n1,n2,n3,n4;
        if (ValidAddress(build_info.strIp,n1,n2,n3,n4)==FALSE)  // 验证NC的IP
        {
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
            {
                strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
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
            if(b)
            {
                strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
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
    }


    g_CommunicationType = 0;
    SetCommunicationType(g_CommunicationType);
    CString strComport = build_info.strComPort;
    CString strComNum = strComport.Mid(3);
    int nCom = _wtoi(strComNum);

    open_com(nCom);//open*************************************
    if(!is_connect())
    {
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
    BOOL bRet =FALSE;
    CString strInfo;
    if(m_hCurCommunication!=NULL)
    {
        CloseHandle(m_hCurCommunication);
        m_hCurCommunication=NULL;
    }
    if (!((tree_node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)
		||(tree_node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0)
		|| (tree_node.BuildingInfo.strIp.CompareNoCase(_T("38400")) == 0)
		|| (tree_node.BuildingInfo.strIp.CompareNoCase(_T("57600")) == 0)
		|| (tree_node.BuildingInfo.strIp.CompareNoCase(_T("115200")) == 0)
		||(tree_node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
    {
        UINT n1,n2,n3,n4;
        if (ValidAddress(tree_node.BuildingInfo.strIp,n1,n2,n3,n4)==FALSE)  // 验证NC的IP
        {

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
            bRet =b;
            CString strInfo;
            if(b)
            {
                strInfo.Format((_T("Open IP:%s:%d successful")),tree_node.BuildingInfo.strIp,m_nIpPort);//prompt info;
                SetPaneString(1,strInfo);
                CString	g_configfile_path=g_strExePth+_T("T3000_config.ini");
                CFileFind fFind;
                if(!fFind.FindFile(g_configfile_path))
                {
                    CString strInfo;
                    strInfo.Format(_T("%d"),g_CommunicationType);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),strInfo,g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),StringIP,g_configfile_path);
                    strInfo.Format(_T("%d"),m_nIpPort);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),strInfo,g_configfile_path);

                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);

                    CString temp_modbus_id;
                    temp_modbus_id.Format(_T("%d"), tree_node.product_id);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Modbus ID"), temp_modbus_id, g_configfile_path);

                }
                else
                {
                    CString strInfo;
                    strInfo.Format(_T("%d"),g_CommunicationType);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),strInfo,g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),StringIP,g_configfile_path);
                    strInfo.Format(_T("%d"),m_nIpPort);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),strInfo,g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);

                    CString temp_modbus_id;
                    temp_modbus_id.Format(_T("%d"), tree_node.product_id);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Modbus ID"), temp_modbus_id, g_configfile_path);
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
            bool b= Open_Socket_Retry(tree_node.BuildingInfo.strIp,m_nIpPort);
            bRet =b;
            CString strTips;
            strTips.Format(_T("Try to connect %s ,Port:%d"),tree_node.BuildingInfo.strIp,m_nIpPort);
            CString strInfo;
            if(b)
            {
                strInfo.Format((_T("Open IP:%s:%d successful")),tree_node.BuildingInfo.strIp,m_nIpPort);//prompt info;
                SetPaneString(1,strInfo);
                CString	g_configfile_path=g_strExePth+_T("T3000_config.ini");
                CFileFind fFind;
                if(!fFind.FindFile(g_configfile_path))
                {
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),tree_node.BuildingInfo.strIp,g_configfile_path);
                    strInfo.Format(_T("%d"),m_nIpPort);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),strInfo,g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
                    CString temp_modbus_id;
                    temp_modbus_id.Format(_T("%d"), tree_node.product_id);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Modbus ID"), temp_modbus_id, g_configfile_path);
                }
                else
                {
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("1"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),tree_node.BuildingInfo.strIp,g_configfile_path);
                    strInfo.Format(_T("%d"),m_nIpPort);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),strInfo,g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("3000"),g_configfile_path);
                    CString temp_modbus_id;
                    temp_modbus_id.Format(_T("%d"), tree_node.product_id);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Modbus ID"), temp_modbus_id, g_configfile_path);
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
    }


    g_CommunicationType = 0;
    SetCommunicationType(g_CommunicationType);
    int nCom = tree_node.ncomport;
    open_com(nCom);//open*************************************
    if(!is_connect())
    {
        strInfo.Format(_T("COM %d : Not available "), nCom);
        SetPaneString(1,strInfo);
        AfxMessageBox(strInfo);
        bRet =FALSE;
        return 0;
    }
    else
    {
        strInfo.Format(_T("COM %d Connected: Yes"), nCom);
        SetPaneString(1,strInfo);
		 Change_BaudRate(tree_node.baudrate);
        CString	g_configfile_path=g_strExePth+_T("T3000_config.ini");
        CFileFind fFind;
        if(!fFind.FindFile(g_configfile_path))
        {
            CString strInfo;
            strInfo.Format(_T("%d"),g_CommunicationType);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),strInfo,g_configfile_path);
            strInfo.Format(_T("COM%d"),nCom);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),strInfo,g_configfile_path);
            strInfo.Format(_T("%d"),nCom);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),strInfo,g_configfile_path);

            strInfo.Format(_T("%d"),default_com1_port_baudrate);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),strInfo,g_configfile_path);
        }
        else
        {
            CString strInfo;
            strInfo.Format(_T("%d"),g_CommunicationType);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),strInfo,g_configfile_path);
            strInfo.Format(_T("COM%d"),nCom);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),strInfo,g_configfile_path);
            strInfo.Format(_T("%d"),nCom);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),strInfo,g_configfile_path);

            strInfo.Format(_T("%d"),tree_node.baudrate);
            WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),strInfo,g_configfile_path);
        }
        bRet = TRUE;
    }
    bRet = is_connect();


    if (!bRet)
    {
        CheckConnectFailure(tree_node.BuildingInfo.strIp);
        m_nStyle=2;
        m_pTreeViewCrl->turn_item_image(tree_node.product_item ,false);//Can't connect to the device , it will show disconnection;
        tree_node.status_last_time[0] = false;
        tree_node.status_last_time[1] = false;
        tree_node.status_last_time[2] = false;
        tree_node.status_last_time[3] = false;
        tree_node.status_last_time[4] = false;
        return bRet;
    }
    else
    {
        m_nStyle=1;
        m_CurSubBuldingInfo=tree_node.BuildingInfo;
        m_pTreeViewCrl->turn_item_image(tree_node.product_item ,true);

        m_pTreeViewCrl->SetSelectItem(tree_node.product_item );//在线的时候才将颜色变红;
        m_pTreeViewCrl->SetSelectSerialNumber(tree_node.serial_number);
        g_selected_serialnumber = tree_node.serial_number;

    }
    return bRet;
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
    CString strHostIP;
    strHostIP.Format(_T("%d.%d.%d.%d"), sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);

    // 是否是同一子网
    if ( ia.S_un.S_un_b.s_b1 == sa.sin_addr.S_un.S_un_b.s_b1 &&
            ia.S_un.S_un_b.s_b2 == sa.sin_addr.S_un.S_un_b.s_b2 &&
            ia.S_un.S_un_b.s_b3 == sa.sin_addr.S_un.S_un_b.s_b3
       )
    {
        // 是同一子网，但是连接不上，那么提示检查设备连接
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

#include "BuildingConfigration.h"

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
 

    bool temp_value = 	b_pause_refresh_tree;	//如果在Config界面选择building的时候就不要刷新Tree了.
    b_pause_refresh_tree = ADD_BUILDING_CONFIG;
    m_nStyle=4;
    Invalidate();
    CBuildingConfigration Dlg;

     
    Dlg.DoModal();

    if(Dlg.m_bChanged)
    {

    }
    b_pause_refresh_tree = temp_value;

    PostMessage(WM_MYMSG_REFRESHBUILDING,0,0);

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
	if (m_pScanner != NULL)
	{
        
		delete m_pScanner;
		m_pScanner = NULL;
	}
	 

	
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
    if(SCAN_TIMER==nIDEvent)
    {

        CString strTemp;
        if(m_bScanFinished)
        {
            CString strTemp;
            g_bPauseMultiRead=FALSE;
            KillTimer(SCAN_TIMER);
            m_wndWorkSpace.m_TreeCtrl.Invalidate();
            SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Scan finished!"));
        }
        else
        {
            CString strTip;
            strTip.Format(_T("Scanning . Please wait."));
            CString* pstrInfo = new CString(strTip);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
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

	if(FOR_LAST_VIEW_TIMER == nIDEvent)
	{
		KillTimer(FOR_LAST_VIEW_TIMER);
		int temp_pid;
		unsigned int serial_number = 0;
		int first_view_ui = 0 ;
		temp_pid = GetPrivateProfileInt(_T("LastView"),_T("ViewPid"),0,g_cstring_ini_path);
		serial_number = (unsigned int)GetPrivateProfileInt(_T("LastView"),_T("ViewSerialNumber"),0,g_cstring_ini_path);
		first_view_ui = GetPrivateProfileInt(_T("LastView"),_T("FistLevelViewUI"),0,g_cstring_ini_path);

		if((serial_number != 0)/* && (temp_pid == PM_MINIPANEL|| temp_pid == PM_MINIPANEL_ARM)*/)
		{
			bool find_product = false;
			vector <tree_product>::iterator temp_it;
			for (temp_it = m_product.begin();temp_it!= m_product.end();++temp_it)
			{
				if((temp_it->serial_number == serial_number)/* &&
					(temp_it->product_class_id == PM_MINIPANEL|| temp_it->product_class_id == PM_MINIPANEL_ARM)*/)
				{
					DoConnectToANode(temp_it->product_item);
                    break;
				}
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
	if(m_pTreeViewCrl->m_hWnd != NULL)
		m_pTreeViewCrl->DeleteAllItems();
    m_comportlist.clear();
    m_floorLst.clear();
    m_roomLst.clear();
    m_backgroundLst.clear();
}
void CMainFrame::SwitchToPruductType(int nIndex)
{
    HideBacnetWindow();
    CView* pNewView = m_pViews[nIndex];
    if (nIndex >= DLG_DIALOG_ZIGBEE_REPEATER)
    {
        if (!pNewView)
        {
            CDocument* pCurrentDoc = GetActiveDocument();
            CCreateContext newContext;
            CRect rect(0, 0, 0, 0);
            newContext.m_pNewViewClass = NULL;
            newContext.m_pNewDocTemplate = NULL;
            newContext.m_pLastView = NULL;
            newContext.m_pCurrentFrame = NULL;
            newContext.m_pCurrentDoc = pCurrentDoc;
            switch (nIndex)
            {
            case DLG_DIALOG_ZIGBEE_REPEATER:
                m_pViews[DLG_DIALOG_ZIGBEE_REPEATER] = (CView *)new CZigbeeRepeater();
                m_pViews[DLG_DIALOG_ZIGBEE_REPEATER]->Create(NULL, NULL,
                    (AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
                    rect, this,
                    AFX_IDW_PANE_FIRST + DLG_DIALOG_ZIGBEE_REPEATER, &newContext);

                m_pViews[DLG_DIALOG_ZIGBEE_REPEATER]->OnInitialUpdate();//?

                break;
            case DLG_DIALOG_TSTAT_AQ:
                m_pViews[DLG_DIALOG_TSTAT_AQ] = (CView *)new CTstatAQ();
                m_pViews[DLG_DIALOG_TSTAT_AQ]->Create(NULL, NULL,
                    (AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
                    rect, this,
                    AFX_IDW_PANE_FIRST + DLG_DIALOG_TSTAT_AQ, &newContext);

                m_pViews[DLG_DIALOG_TSTAT_AQ]->OnInitialUpdate();//?

                break;
            case DLG_DIALOG_THIRD_PARTY_BAC:
                m_pViews[DLG_DIALOG_THIRD_PARTY_BAC] = (CView *)new CBacnetThirdPartyMain();
                m_pViews[DLG_DIALOG_THIRD_PARTY_BAC]->Create(NULL, NULL,
                    (AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
                    rect, this,
                    AFX_IDW_PANE_FIRST + DLG_DIALOG_THIRD_PARTY_BAC, &newContext);

                m_pViews[DLG_DIALOG_THIRD_PARTY_BAC]->OnInitialUpdate();//?

                break;
            default:
                return;
                    break;
            }
        }
    }
    else
    {
        if (!pNewView)
            return;
    }
   /* CView**/ pNewView = m_pViews[nIndex];
    if (!pNewView)
        return;
    CView* pActiveView =GetActiveView();
    if ( !pActiveView )    // No currently active view
        return;

    if (nIndex < DLG_DIALOG_ZIGBEE_REPEATER)
    {
        if (pNewView == pActiveView)    // Already there
        {
            goto here;
        }
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
    //pNewView->Invalidate();

here:
    g_bPauseMultiRead = FALSE;//恢复主线程的刷新

    switch(nIndex)
    {
    case DLG_T3000_VIEW:
    {
        m_nCurView=DLG_T3000_VIEW;
        ((CT3000View*)m_pViews[m_nCurView])->FreshIOGridTable();
        already_check_tstat_timesync = false;
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
        //m_nCurView=DLG_DIALOGCM5_VIEW;
        //((CDialogCM5*)m_pViews[m_nCurView])->Fresh();
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
        //((CDialgMiniPanel*)m_pViews[m_nCurView])->Fresh();
        //This is old MiniPanel ,basing on the modbus protocol.

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
    //    ((CNewHumChamberView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case  DLG_CO2_VIEW:
    {
        m_nCurView = DLG_CO2_VIEW;
        ((CCO2_View*)m_pViews[m_nCurView])->Fresh();
    }
    break;
	case  DLG_DIALOG_BTUMETER:
	{
		m_nCurView = DLG_DIALOG_BTUMETER;
		((CBTUMeterDlg*)m_pViews[m_nCurView])->Fresh();
	}
	break;
    case  PM_CO2_NODE:
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
        //g_protocol = PROTOCOL_BACNET_IP;
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
    }
    break;
    case DLG_DIALOGT38AI8AO :
    {
        m_nCurView=DLG_DIALOGT38AI8AO;
        ((T38AI8AO*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case  DLG_DIALOGT36CT:
    {
        m_nCurView=DLG_DIALOGT36CT;
        ((T36CT*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOGT3PT10:
    {
        m_nCurView=DLG_DIALOGT3PT10;
        ((CT3RTDView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_PRESSURE_SENSOR:
    {
        m_nCurView=DLG_DIALOG_PRESSURE_SENSOR;
        ((CPressureSensorForm*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case  DLG_DIALOG_TEMP_HUMSENSOR:
    {
        m_nCurView=DLG_DIALOG_TEMP_HUMSENSOR;
        ((CTempHumSensorForm*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case  DLG_DIALOG_DEFAULT_T3000_VIEW:
    {
        m_nCurView=DLG_DIALOG_DEFAULT_T3000_VIEW;
        ((T3000_Default_MainView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_T3_INPUTS_VIEW:
    {
        m_nCurView=DLG_DIALOG_T3_INPUTS_VIEW;
        ((CT3ModulesView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_T3_OUTPUTS_VIEW:
    {
        m_nCurView = DLG_DIALOG_T3_OUTPUTS_VIEW;
        ((CT3ModulesOutputDlg*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_CUSTOM_VIEW:
    {
        m_nCurView = DLG_DIALOG_CUSTOM_VIEW;
        ((CProductRegisterListView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_TSTAT_INPUT_VIEW:
    {
        m_nCurView = DLG_DIALOG_TSTAT_INPUT_VIEW;
        ((CTStatInputView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_TSTAT_OUTPUT_VIEW:
    {
        m_nCurView = DLG_DIALOG_TSTAT_OUTPUT_VIEW;
        ((CTStatOutputView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_BOATMONITOR:
    {
        m_nCurView = DLG_DIALOG_BOATMONITOR;
        ((CBoatMonitorViewer*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_POWERMETER:
    {
        m_nCurView = DLG_DIALOG_POWERMETER;
        ((CPowermeter*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_CO2_NODE:
    {
        m_nCurView = DLG_DIALOG_CO2_NODE;
        ((CCO2_NodeView*)m_pViews[m_nCurView])->Fresh();
    }
    break;
    case DLG_DIALOG_ZIGBEE_REPEATER:
    {
        m_nCurView = DLG_DIALOG_ZIGBEE_REPEATER;
        ((CZigbeeRepeater*)m_pViews[m_nCurView])->Fresh();
        PostMessage(WM_SIZE, 0, 0);
    }
    break;
    case DLG_DIALOG_TSTAT_AQ:
    {
        m_nCurView = DLG_DIALOG_TSTAT_AQ;
        ((CTstatAQ*)m_pViews[m_nCurView])->Fresh();
        PostMessage(WM_SIZE, 0, 0);
    }
    break;
    case DLG_DIALOG_THIRD_PARTY_BAC:
    {
        m_nCurView = DLG_DIALOG_THIRD_PARTY_BAC;
        ((CBacnetThirdPartyMain*)m_pViews[m_nCurView])->Fresh();
        PostMessage(WM_SIZE, 0, 0);
    }
    break;
        //here
    }
}
void CMainFrame::SetPaneConnectionPrompt(CString strInfo)
{
    SetPaneString(2,strInfo);
}
void CMainFrame::SwitchToGraphicView()
{
    SwitchToPruductType(DLG_GRAPGIC_VIEW);
}
void CMainFrame::OnAllNodeDatabase()
{
    bool temp_status = b_pause_refresh_tree;

    b_pause_refresh_tree = All_NODE_DATABASE;
    if(m_strCurMainBuildingName.IsEmpty())
        AfxMessageBox(_T("Please Select a Building First."));
    CAllNodesDiaolg Dlg;
    Dlg.SetBuildingMainName(m_strCurMainBuildingName);

    Dlg.DoModal();
    if(Dlg.m_bChanged)
    {
        ScanTstatInDB();
    }
    b_pause_refresh_tree = temp_status;

}
#include "ScanDlg.h"
void CMainFrame::Scan_Product()
{
    CString strTime;
    strTime=Get_NowTime();
    //开始时间

    CString g_strT3000LogString=_T("--------------------------------Scan Begin--------------------------------\n");
    CString* pstrInfo = new CString(g_strT3000LogString);
    ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));

    g_strT3000LogString=_T("Scan begin Time: ");
    g_strT3000LogString+=strTime+_T("\n");;
    pstrInfo = new CString(g_strT3000LogString);
    ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));

    ClearBuilding();


    HANDLE temphandle;//如果用户点击Scan，而 bacnet的线程还在继续工作，需要先结束这两个线程;
    if(bac_net_initial_once)
    {
        close_bac_com();
    }

    SetTimer(SCAN_TIMER,100,NULL);//lsc

    g_bEnableRefreshTreeView = FALSE;
    b_pause_refresh_tree = SCAN_PRODUCT;
    if (m_pScanner != NULL)
    {
        delete m_pScanner;
        m_pScanner = NULL;
    }
	m_refresh_net_device_data.clear();
	scaning_mode = true;
    m_pScanner = new CTStatScanner;
    g_ScnnedNum=0;

	int size_serial = m_product.size();

	UINT * serial_int = new UINT[size_serial];
	memset(serial_int, 0, size_serial*sizeof(UINT));
	int c_count = 0;
	vector <tree_product>::iterator temp_v;
	for (temp_v = m_product.begin();temp_v != m_product.end();)
	{
		if (temp_v->serial_number != 0)
		{
			serial_int[c_count] = temp_v->serial_number;
			c_count++;
			
		}
		++temp_v;
	}
	
	m_pScanner->SetOldExsitSerial(serial_int, c_count);
	delete[] serial_int;
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

	CScanDlg dlg;
	dlg.SetScanner(m_pScanner);
	dlg.DoModal();

    m_bScanALL = TRUE;
	refresh_tree_status_immediately = true; //立即刷新 树形结构;
    delete m_pWaitScanDlg;
    m_pWaitScanDlg = NULL;
	scaning_mode = false;
	b_pause_refresh_tree = false;
	PostMessage(WM_MYMSG_REFRESHBUILDING,0,0);

}



CString CMainFrame::GetDeviceClassName(int nClassID)
{
    CString strClassName;
    switch(nClassID)
    {
    case 2:
        strClassName=g_strTstat5a;
        break;
    case 1:
        strClassName=g_strTstat5b;
        break;
    case 3:
        strClassName=g_strTstat5b;
        break;
    case 4:
        strClassName=g_strTstat5c;
        break;
    case 12:
        strClassName=g_strTstat5d;
        break;
    case PM_NC:
        strClassName=g_strnetWork;
        break;
    case NET_WORK_OR485_PRODUCT_MODEL:
        strClassName=g_strOR485;
        break;
    case 17:
        strClassName=g_strTstat5f;
        break;
    case 18:
        strClassName=g_strTstat5g;
        break;
    case 16:
        strClassName=g_strTstat5e;
        break;
    case PM_PM5E:
    case PM_PM5E_ARM:
        strClassName=g_strTstat5e;
        break;
    case 19:
        strClassName=g_strTstat5h;
        break;
    case 7:
        strClassName=g_strTstat7;
        break;
    case 6:
        strClassName=g_strTstat6;
        break;
    case 13:
    case 14:
        break;
    default:
        strClassName=g_strTstat5a;
        break;
    }

    return strClassName;
}


void CMainFrame::Show_Wait_Dialog_And_SendConfigMessage()
{    
if(hwait_write_thread==NULL)    
{
        hwait_write_thread =CreateThread(NULL,NULL,Send_Set_Config_Command_Thread,this,NULL, NULL);
    }
}

void CMainFrame::Show_Wait_Dialog_And_ReadBacnet(int ncontrol)
{
    m_read_control = ncontrol;
	if(hwait_read_thread==NULL)
	{
		hwait_read_thread =CreateThread(NULL,NULL,Read_Bacnet_Thread,this,NULL, NULL);
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


DWORD WINAPI  CMainFrame::Write_Modbus_tstat_cfg(LPVOID lpVoid)
{
    CMainFrame *pParent = (CMainFrame *)lpVoid;
    g_bPauseMultiRead = TRUE;
    now_tstat_id = g_tstat_id; //必须要赋值 否则  now_tstat_id 后面全是 0;
    load_file_every_step temppp;
    CString log_file_path = _T("C:\\1.txt");
    //added the header marker.
    CStdioFile log_file;
    if (log_file.Open(log_file_path, CFile::modeCreate | CFile::modeReadWrite) != 0)
    {
        CString ttt = _T("\r\n\r\n");
        log_file.Write(ttt.GetString(), ttt.GetLength());
    }

    //LoadConfigFilePath
    LoadFile2Tstat67(temppp, (LPTSTR)(LPCTSTR)LoadConfigFilePath, &log_file);
    SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Loading config file success!"));
    hwait_write_tstat_cfg = NULL;
    g_bPauseMultiRead = FALSE;
    return 0; 
}

DWORD WINAPI  CMainFrame::Write_Modbus_10000(LPVOID lpVoid)
{
	CMainFrame *pParent = (CMainFrame *)lpVoid;


	if(LoadMiniModbusConfigFile(LoadConfigFilePath)> 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Write prg file OK!"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Write prg file failed!"));
	}
	hwait_write_modbus10000 = NULL;
	return true;
}




DWORD WINAPI  CMainFrame::Read_Modbus_10000(LPVOID lpVoid)
{
	 CMainFrame *pParent = (CMainFrame *)lpVoid;


	 if(SaveModbusConfigFile(SaveConfigFilePath)> 0)
	 {
		 SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Save prg file OK!"));
	 }
	 else
	 {
		  SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Save prg file failed!"));
	 }
	 g_progress_persent = 0;
	 hwait_read_modbus10000 = NULL;
	 return true;
}

//点击 T3TBLBBB TSTAT10  CM5 的时候会通过此函数保存缓存数据至 本地文件;
//点击菜单 Save file  也会 保存所有的配置信息;
DWORD WINAPI  CMainFrame::Read_Bacnet_Thread(LPVOID lpVoid)
{
	 CMainFrame *pParent = (CMainFrame *)lpVoid;
     int  nspecial_mode = pParent->m_read_control;  // 0 默认全读   1 缓存的时候不读 program;
	 int end_temp_instance = 0;
	  CString Mession_ret;
	   read_write_bacnet_config = true;
	  int read_total_count = BAC_INPUT_GROUP +
		  BAC_OUTPUT_GROUP +
		  BAC_PROGRAM_GROUP +
		  BAC_VARIABLE_GROUP +
		  BAC_PID_GROUP +
		  BAC_PROGRAMCODE_GROUP +
		  BAC_SCREEN_GROUP +
		  BAC_MONITOR_GROUP +
		  BAC_SCHEDULE_GROUP +
		  BAC_HOLIDAY_GROUP +
		  BAC_SCHEDULECODE_GOUP +
		  BAC_HOLIDAYCODE_GROUP +
		  BAC_GRPHIC_LABEL_GROUP + 
		  BAC_CUSTOMER_UNIT_GROUP +
		  BAC_USER_LOGIN_GROUP +
          BAC_MSV_GROUP +
		  BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT +
		  BAC_PROGRAM_ITEM_COUNT*5 +   //乘以5 是因为每个program都有5包，共2000个字节要读;
		  1 +		//这个1是Setting
		  1 +
          BAC_SCHEDULE_FLAG_GROUP;		//这个1是Variable Custmer Units.	


	  int read_all_step = 1000 / read_total_count;
	  int read_success_count = 0;
	  int write_pos = 0;


	 for (int i=0; i<BAC_INPUT_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_INPUT_REMAINDER + (BAC_READ_INPUT_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_INPUT_ITEM_COUNT)
			 end_temp_instance = BAC_INPUT_ITEM_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READINPUT_T3000,(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_in_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read input from %d to %d success."),(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read input from %d to %d timeout."),(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }

	 for (int i=0; i<BAC_OUTPUT_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_OUTPUT_REMAINDER + (BAC_READ_OUTPUT_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_OUTPUT_ITEM_COUNT)
			 end_temp_instance = BAC_OUTPUT_ITEM_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READOUTPUT_T3000,(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_out_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read output from %d to %d success."),(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read input from %d to %d timeout."),(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }

     for (int i = 0; i<BAC_SCHEDULECODE_GOUP; i++)
     {
         if (READ_MODE_SIMPLIFICATION == nspecial_mode)
         {
             read_success_count++;
             g_progress_persent = read_success_count * 100 / read_total_count;
             Sleep(SEND_COMMAND_DELAY_TIME);
             continue;
         }
         if (GetPrivateData_Blocking(g_bac_instance, READTIMESCHEDULE_T3000, i, i, WEEKLY_SCHEDULE_SIZE) > 0)
         {
             Mession_ret.Format(_T("Read schedule from %d to %d success."), i, i);
             SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
             read_success_count++;
             Sleep(SEND_COMMAND_DELAY_TIME);
         }
         else
         {
             Mession_ret.Format(_T("Read schedule from %d to %d timeout."), i, i);
             SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
             goto read_end_thread;
         }
         g_progress_persent = read_success_count * 100 / read_total_count;
     }

	 for (int i=0; i<BAC_VARIABLE_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_VARIABLE_REMAINDER + (BAC_READ_VARIABLE_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_VARIABLE_ITEM_COUNT)
			 end_temp_instance = BAC_VARIABLE_ITEM_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READVARIABLE_T3000,(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_variable_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read variable from %d to %d success."),(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read variable from %d to %d timeout."),(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_PROGRAM_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_PROGRAM_REMAINDER + (BAC_READ_PROGRAM_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_PROGRAM_ITEM_COUNT)
			 end_temp_instance = BAC_PROGRAM_ITEM_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READPROGRAM_T3000,(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_program_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read program from %d to %d success."),(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read program from %d to %d timeout."),(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }



	 for (int i=0; i<BAC_HOLIDAYCODE_GROUP; i++)
	 {
         if (READ_MODE_SIMPLIFICATION == nspecial_mode)
         {
             read_success_count++;
             g_progress_persent = read_success_count * 100 / read_total_count;
             Sleep(SEND_COMMAND_DELAY_TIME);
             continue;
         }
		 if(GetPrivateData_Blocking(g_bac_instance,READANNUALSCHEDULE_T3000,i,i, ANNUAL_CODE_SIZE) > 0)
		 {
			 Mession_ret.Format(_T("Read holiday from %d to %d success."),i,i);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read holiday from %d to %d timeout."),i,i);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_PID_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_PID_REMAINDER + (BAC_READ_PID_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_PID_COUNT)
			 end_temp_instance = BAC_PID_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READCONTROLLER_T3000,(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_controller_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read PID from %d to %d success."),(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read PID from %d to %d timeout."),(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_SCREEN_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_SCREEN_REMAINDER + (BAC_READ_SCREEN_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_SCREEN_COUNT)
			 end_temp_instance = BAC_SCREEN_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READSCREEN_T3000,(BAC_READ_SCREEN_GROUP_NUMBER)*i,end_temp_instance,sizeof(Control_group_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read screen from %d to %d success."),(BAC_READ_SCREEN_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read screen from %d to %d timeout."),(BAC_READ_SCREEN_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_HOLIDAY_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_HOLIDAY_REMAINDER + (BAC_READ_HOLIDAY_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_HOLIDAY_COUNT)
			 end_temp_instance = BAC_HOLIDAY_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READANNUALROUTINE_T3000,(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_annual_routine_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read holiday list from %d to %d success."),(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read holiday list from %d to %d timeout."),(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }



	 for (int i=0; i<BAC_SCHEDULE_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_SCHEDULE_REMAINDER + (BAC_READ_SCHEDULE_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_SCHEDULE_COUNT)
			 end_temp_instance = BAC_SCHEDULE_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READWEEKLYROUTINE_T3000,(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_weekly_routine_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read schedule list from %d to %d success."),(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read schedule list from %d to %d timeout."),(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_MONITOR_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_MONOTOR_REMAINDER + (BAC_READ_MONITOR_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_MONITOR_COUNT)
			 end_temp_instance = BAC_MONITOR_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READMONITOR_T3000,(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_monitor_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read monitor list from %d to %d success."),(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read monitor list from %d to %d timeout."),(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_CUSTOMER_UNIT_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_CUSTOMER_UNITS_REMAINDER + (BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_CUSTOMER_UNITS_COUNT)
			 end_temp_instance = BAC_CUSTOMER_UNITS_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READUNIT_T3000,(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_Units_element)) > 0)
		 {
			 Mession_ret.Format(_T("Read custom unit from %d to %d success."),(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read custom unit from %d to %d timeout."),(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }



	 for (int i=0; i<BAC_USER_LOGIN_GROUP; i++)
	 {
		 end_temp_instance = BAC_READ_USER_LOGIN_INFO_REMAINDER + (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_USER_LOGIN_COUNT)
			 end_temp_instance = BAC_USER_LOGIN_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READUSER_T3000,(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_userlogin_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read user login table from %d to %d success."),(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read user login table from %d to %d timeout."),(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 for (int i=0; i<BAC_GRPHIC_LABEL_GROUP; i++)
	 {
         if (READ_MODE_SIMPLIFICATION == nspecial_mode)
         {
             read_success_count++;
             g_progress_persent = read_success_count * 100 / read_total_count;
             Sleep(SEND_COMMAND_DELAY_TIME);
             continue;
         }
		 end_temp_instance = BAC_READ_GRPHIC_LABEL_REMAINDER + (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_GRPHIC_LABEL_COUNT)
			 end_temp_instance = BAC_GRPHIC_LABEL_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READ_GRPHIC_LABEL_COMMAND,(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_label_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read screen data from %d to %d success."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read screen data table from %d to %d timeout."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }


	 //下面是读Setting 结构的部分;
	 if(GetPrivateData_Blocking(g_bac_instance,READ_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info)) > 0)
	 {
		 Mession_ret.Format(_T("Read device information success."));
		 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
		 read_success_count ++ ;
		 Sleep(SEND_COMMAND_DELAY_TIME);
	 }
	 else
	 {
		 Mession_ret.Format(_T("Read device information success."));
		 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
		 goto read_end_thread;
	 }
	  g_progress_persent = read_success_count * 100 /read_total_count;

      for (int z = 0;z<BAC_PROGRAM_ITEM_COUNT;z++)
      {

          memset(program_code[z], 0, 2000);		 //清零;

          for (int i = 0;i < 5;i++)
          {
              if (READ_MODE_SIMPLIFICATION == nspecial_mode)
              {
                  read_success_count++;
                  g_progress_persent = read_success_count * 100 / read_total_count;
                  continue;
              }

              int ret_variable;
              ret_variable = GetProgramData_Blocking(g_bac_instance, z, z, i);
              if (ret_variable < 0)
              {
                  Mession_ret.Format(_T("Read program code %d part %d timeout."), z, i); //如果重试3次都失败就跳转至 失败;
                  SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
                  goto read_end_thread;
              }
              else
              {
                  read_success_count++;
                  g_progress_persent = read_success_count * 100 / read_total_count;
                  Mession_ret.Format(_T("Read program code %d part %d success."), z, i);
                  SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
              }
          }

      }

	  for (int i=0; i<1; i++)
	  {
		  if(GetPrivateData_Blocking(g_bac_instance, READVARUNIT_T3000,0,4,sizeof(Str_variable_uint_point)) > 0)
		  {
			  Mession_ret.Format(_T("Read variable custom units success."));
			  SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			  read_success_count ++ ;
			  Sleep(SEND_COMMAND_DELAY_TIME);
		  }
		  else
		  {
			  Mession_ret.Format(_T("Read variable custom units timeout."));
			  SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			  goto read_end_thread;
		  }
		  g_progress_persent = read_success_count * 100 /read_total_count;
	  }


	 for (int i=0; i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
	 {
		 
		 end_temp_instance = BAC_ALALOG_CUSTMER_RANGE_TABLE_REMAINDER + (BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i ;
		 if(end_temp_instance >= BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT)
			 end_temp_instance = BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT - 1;
		 if(GetPrivateData_Blocking(g_bac_instance,READANALOG_CUS_TABLE_T3000,(BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_table_point)) > 0)
		 {
			 Mession_ret.Format(_T("Read Analog custom table from %d to %d success."),(BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 read_success_count ++ ;
			 Sleep(SEND_COMMAND_DELAY_TIME);
		 }
		 else
		 {
			 Mession_ret.Format(_T("Read Analog custom table from %d to %d timeout."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
			 goto read_end_thread;
		 }
		 g_progress_persent = read_success_count * 100 /read_total_count;
	 }

     //MSV
     for (int i = 0; i<BAC_MSV_GROUP; i++)
     {
         end_temp_instance = BAC_READ_MSV_REMAINDER + (BAC_MSV_GROUP_NUMBER)*i;
         if (end_temp_instance >= BAC_MSV_COUNT)
             end_temp_instance = BAC_MSV_COUNT - 1;
         if (GetPrivateData_Blocking(g_bac_instance, READ_MSV_COMMAND, (BAC_MSV_GROUP_NUMBER)*i, end_temp_instance, sizeof(Str_MSV)) > 0)
         {
             Mession_ret.Format(_T("Read msv range from %d to %d success."), (BAC_MSV_GROUP_NUMBER)*i, end_temp_instance);
             SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
             read_success_count++;
             Sleep(SEND_COMMAND_DELAY_TIME);
         }
         else
         {
             Mession_ret.Format(_T("Read msv range from %d to %d timeout."), (BAC_MSV_GROUP_NUMBER)*i, end_temp_instance);
             SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
             goto read_end_thread;
         }
         g_progress_persent = read_success_count * 100 / read_total_count;
     }

     for (int i = 0; i<BAC_SCHEDULE_FLAG_GROUP; i++)
     {
         end_temp_instance = BAC_READ_SCHEDULE_FLAG_REMAINDER + (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i;
         if (end_temp_instance >= BAC_SCHEDULE_COUNT)
             end_temp_instance = BAC_SCHEDULE_COUNT - 1;
         if (GetPrivateData_Blocking(g_bac_instance, READ_SCHEDUAL_TIME_FLAG, (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i, end_temp_instance, sizeof(Str_schedual_time_flag)) > 0)
         {
             Mession_ret.Format(_T("Read schedule flag list from %d to %d success."), (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i, end_temp_instance);
             SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
             read_success_count++;
             Sleep(SEND_COMMAND_DELAY_TIME);
         }
         else
         {
             Mession_ret.Format(_T("Read schedule flag list from %d to %d timeout."), (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i, end_temp_instance);
             SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
             goto read_end_thread;
         }
         g_progress_persent = read_success_count * 100 / read_total_count;
     }



	 read_write_bacnet_config = false;
	 hwait_read_thread = NULL;
	 SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data success!"));
	 ::PostMessageW(MainFram_hwd,MY_BAC_CONFIG_READ_RESULTS,1,NULL);
	 g_progress_persent = 100;

	 return 2;
read_end_thread:
	 g_progress_persent = 0;
	 read_write_bacnet_config = false;
	 hwait_read_thread = NULL;
	 Mession_ret.Format(_T("Read data Timeout,Please Try again."));
	 SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
	 //AfxMessageBox(_T("Read data Timeout,Please Try again."));
	 return 1;

}



DWORD WINAPI  CMainFrame::Send_Set_Config_Command_Thread(LPVOID lpVoid)
{
    //Write_Config_Info
    CMainFrame *pParent = (CMainFrame *)lpVoid;
	int temp_prg_version = 0;
	temp_prg_version = pParent->m_prg_version;
    read_write_bacnet_config = true;
    int end_temp_instance = 0;

    int resend_count;
    CString Mession_ret;

    int write_total_count;
	if(temp_prg_version == 2)
	{
		write_total_count = BAC_INPUT_GROUP +
			BAC_OUTPUT_GROUP +
			BAC_PROGRAM_GROUP +
			BAC_VARIABLE_GROUP +
			BAC_PID_GROUP +
			BAC_PROGRAMCODE_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP +
			BAC_SCHEDULE_GROUP +
			BAC_HOLIDAY_GROUP +
			BAC_SCHEDULECODE_GOUP +
			BAC_HOLIDAYCODE_GROUP;
	}
	else if(temp_prg_version == 3)
	{
		write_total_count =  BAC_INPUT_GROUP +
			BAC_OUTPUT_GROUP +
			BAC_PROGRAM_GROUP +
			BAC_VARIABLE_GROUP +
			BAC_PID_GROUP +
			BAC_PROGRAMCODE_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP +
			BAC_SCHEDULE_GROUP +
			BAC_HOLIDAY_GROUP +
			BAC_SCHEDULECODE_GOUP +
			BAC_HOLIDAYCODE_GROUP +
			BAC_GRPHIC_LABEL_GROUP + 
			BAC_CUSTOMER_UNIT_GROUP +
			BAC_USER_LOGIN_GROUP +
			BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT ;//+
			//BAC_PROGRAM_ITEM_COUNT*5;//乘以5 是因为每个program都有5包，共2000个字节要读;
	}
	else if((temp_prg_version >= 4) && (temp_prg_version <= 5))
	{
		write_total_count =  BAC_INPUT_GROUP +
			BAC_OUTPUT_GROUP +
			BAC_PROGRAM_GROUP +
			BAC_VARIABLE_GROUP +
			BAC_PID_GROUP +
			BAC_PROGRAMCODE_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP +
			BAC_SCHEDULE_GROUP +
			BAC_HOLIDAY_GROUP +
			BAC_SCHEDULECODE_GOUP +
			BAC_HOLIDAYCODE_GROUP +
			BAC_GRPHIC_LABEL_GROUP + 
			BAC_CUSTOMER_UNIT_GROUP +
			BAC_USER_LOGIN_GROUP +
			BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT //BAC_PROGRAM_ITEM_COUNT*5;//乘以5 是因为每个program都有5包，共2000个字节要读;
			+ 1 ;   //Setting
	}
	else if(temp_prg_version == 6)
	{
		write_total_count =  BAC_INPUT_GROUP +
			BAC_OUTPUT_GROUP +
			BAC_PROGRAM_GROUP +
			BAC_VARIABLE_GROUP +
			BAC_PID_GROUP +
			BAC_PROGRAMCODE_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP +
			BAC_SCHEDULE_GROUP +
			BAC_HOLIDAY_GROUP +
			BAC_SCHEDULECODE_GOUP +
			BAC_HOLIDAYCODE_GROUP +
			BAC_GRPHIC_LABEL_GROUP + 
			BAC_CUSTOMER_UNIT_GROUP +
			BAC_USER_LOGIN_GROUP +
			BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT //BAC_PROGRAM_ITEM_COUNT*5;//乘以5 是因为每个program都有5包，共2000个字节要读;
			+ 1		//Setting
			+ 1;    //Variable_Cus_Units
	}
    else if (temp_prg_version == 7)
    {
        write_total_count = BAC_INPUT_GROUP +
            BAC_OUTPUT_GROUP +
            BAC_PROGRAM_GROUP +
            BAC_VARIABLE_GROUP +
            BAC_PID_GROUP +
            BAC_PROGRAMCODE_GROUP +
            BAC_SCREEN_GROUP +
            BAC_MONITOR_GROUP +
            BAC_SCHEDULE_GROUP +
            BAC_HOLIDAY_GROUP +
            BAC_SCHEDULECODE_GOUP +
            BAC_HOLIDAYCODE_GROUP +
            BAC_GRPHIC_LABEL_GROUP +
            BAC_CUSTOMER_UNIT_GROUP +
            BAC_USER_LOGIN_GROUP +
            BAC_MSV_GROUP +
            BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT //BAC_PROGRAM_ITEM_COUNT*5;//乘以5 是因为每个program都有5包，共2000个字节要读;
            + 1		//Setting
            + 1;    //Variable_Cus_Units
    }
    else if (temp_prg_version == 8)
    {
        write_total_count = BAC_INPUT_GROUP +
            BAC_OUTPUT_GROUP +
            BAC_PROGRAM_GROUP +
            BAC_VARIABLE_GROUP +
            BAC_PID_GROUP +
            BAC_PROGRAMCODE_GROUP +
            BAC_SCREEN_GROUP +
            BAC_MONITOR_GROUP +
            BAC_SCHEDULE_GROUP +
            BAC_HOLIDAY_GROUP +
            BAC_SCHEDULECODE_GOUP +
            BAC_HOLIDAYCODE_GROUP +
            BAC_GRPHIC_LABEL_GROUP +
            BAC_CUSTOMER_UNIT_GROUP +
            BAC_USER_LOGIN_GROUP +
            BAC_MSV_GROUP +
            BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT //BAC_PROGRAM_ITEM_COUNT*5;//乘以5 是因为每个program都有5包，共2000个字节要读;
            + 1		//Setting
            + 1     //Variable_Cus_Units
            + BAC_SCHEDULE_FLAG_GROUP;    // Schedule time flag 需要两包能读取完成;
    }
	else
	{
		Mession_ret.Format(_T("prg is broken."));
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
		goto write_end_thread;
		return false;
	}





    int write_all_step = 1000 / write_total_count;
    int write_success_count = 0;
    int write_pos = 0;
	if(temp_prg_version >= 6)	 // version 6 中才加的这玩意;
	{

			if(Write_Private_Data_Blocking(WRITEVARUNIT_T3000,0,4) > 0)
			{
				Mession_ret.Format(_T("Write variable analog custom units success."));
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				write_success_count ++ ;
				Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write variable analog custom units timeout."),0,4);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				goto write_end_thread;
			}
			g_progress_persent = write_success_count * 100 /write_total_count;

	}

    if (temp_prg_version >= 7)	 // version 7  MSV 中才加的这玩意;
    {
        for (int i = 0; i<BAC_MSV_COUNT; i++)
        {
            if (Write_Private_Data_Blocking(WRITE_MSV_COMMAND, i, i) > 0)
            {
                Mession_ret.Format(_T("Write msv table from %d to %d success."), i, i);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
                write_success_count++;
                Sleep(SEND_COMMAND_DELAY_TIME);
            }
            else
            {
                Mession_ret.Format(_T("Write msv table from %d to %d timeout."), i, i);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
                goto write_end_thread;
            }
            g_progress_persent = write_success_count * 100 / write_total_count;
        }
    }


    for (int i=0; i<BAC_INPUT_GROUP; i++)
    {
        end_temp_instance = BAC_READ_INPUT_REMAINDER + (BAC_READ_INPUT_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_INPUT_ITEM_COUNT)
            end_temp_instance = BAC_INPUT_ITEM_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEINPUT_T3000,(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write input from %d to %d success."),(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write input from %d to %d timeout."),(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }


    for (int i=0; i<BAC_OUTPUT_GROUP; i++)
    {
        end_temp_instance = BAC_READ_OUTPUT_REMAINDER + (BAC_READ_OUTPUT_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_OUTPUT_ITEM_COUNT)
            end_temp_instance = BAC_OUTPUT_ITEM_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEOUTPUT_T3000,(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write output from %d to %d success."),(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write output from %d to %d timeout."),(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_VARIABLE_GROUP; i++)
    {
        end_temp_instance = BAC_READ_VARIABLE_REMAINDER + (BAC_READ_VARIABLE_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_VARIABLE_ITEM_COUNT)
            end_temp_instance = BAC_VARIABLE_ITEM_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEVARIABLE_T3000,(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write variable from %d to %d success."),(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write variable from %d to %d timeout."),(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_PROGRAM_GROUP; i++)
    {
        end_temp_instance = BAC_READ_PROGRAM_REMAINDER + (BAC_READ_PROGRAM_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_PROGRAM_ITEM_COUNT)
            end_temp_instance = BAC_PROGRAM_ITEM_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEPROGRAM_T3000,(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write program from %d to %d success."),(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write program from %d to %d timeout."),(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }


    for (int i=0; i<BAC_SCHEDULECODE_GOUP; i++)
    {
        if(Write_Private_Data_Blocking(WRITETIMESCHEDULE_T3000,i,i) > 0)
        {
            Mession_ret.Format(_T("Write schedule from %d to %d success."),i,i);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write schedule from %d to %d timeout."),i,i);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_HOLIDAYCODE_GROUP; i++)
    {
        if(Write_Private_Data_Blocking(WRITEANNUALSCHEDULE_T3000,i,i) > 0)
        {
            Mession_ret.Format(_T("Write holiday from %d to %d success."),i,i);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write holiday from %d to %d timeout."),i,i);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_PID_GROUP; i++)
    {
        end_temp_instance = BAC_READ_PID_REMAINDER + (BAC_READ_PID_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_PID_COUNT)
            end_temp_instance = BAC_PID_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEPID_T3000,(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write PID from %d to %d success."),(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write PID from %d to %d timeout."),(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_SCREEN_GROUP; i++)
    {
        end_temp_instance = BAC_READ_SCREEN_REMAINDER + (BAC_READ_SCREEN_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_SCREEN_COUNT)
            end_temp_instance = BAC_SCREEN_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITESCREEN_T3000,(BAC_READ_SCREEN_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write screen from %d to %d success."),(BAC_READ_SCREEN_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write screen from %d to %d timeout."),(BAC_READ_SCREEN_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_HOLIDAY_GROUP; i++)
    {
        end_temp_instance = BAC_READ_HOLIDAY_REMAINDER + (BAC_READ_HOLIDAY_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_HOLIDAY_COUNT)
            end_temp_instance = BAC_HOLIDAY_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEHOLIDAY_T3000,(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write holiday from %d to %d success."),(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write holiday from %d to %d timeout."),(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }

    for (int i=0; i<BAC_SCHEDULE_GROUP; i++)
    {
        end_temp_instance = BAC_READ_SCHEDULE_REMAINDER + (BAC_READ_SCHEDULE_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_SCHEDULE_COUNT)
            end_temp_instance = BAC_SCHEDULE_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITESCHEDULE_T3000,(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write schedule from %d to %d success."),(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write schedule from %d to %d timeout."),(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }


    for (int i=0; i<BAC_MONITOR_GROUP; i++)
    {
        end_temp_instance = BAC_READ_MONOTOR_REMAINDER + (BAC_READ_MONITOR_GROUP_NUMBER)*i ;
        if(end_temp_instance >= BAC_MONITOR_COUNT)
            end_temp_instance = BAC_MONITOR_COUNT - 1;
        if(Write_Private_Data_Blocking(WRITEMONITOR_T3000,(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance) > 0)
        {
            Mession_ret.Format(_T("Write monitor from %d to %d success."),(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            write_success_count ++ ;
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        else
        {
            Mession_ret.Format(_T("Write monitor from %d to %d timeout."),(BAC_READ_MONITOR_GROUP_NUMBER)*i,end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
            goto write_end_thread;
        }
        g_progress_persent = write_success_count * 100 /write_total_count;
    }





    for (int i=0; i<BAC_PROGRAMCODE_GROUP; i++)
    {

        int npart = (program_code_length[i] / 401) + 1;



        bool b_program_status = true;
        for (int j=0; j<npart; j++)
        {

            if(WriteProgramData_Blocking(g_bac_instance,WRITEPROGRAMCODE_T3000,i,i,j)> 0)
            {
                Mession_ret.Format(_T("Write program %d part %d success."),i,j+1);
                SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
                Sleep(SEND_COMMAND_DELAY_TIME);
                continue;
            }
            else
            {
                Mession_ret.Format(_T("Write program %d part %d timeout."),i,j+1);
                SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
                goto write_end_thread;
            }
        }
        write_success_count ++ ;
        g_progress_persent = write_success_count * 100 /write_total_count;
    }


	if(temp_prg_version >= 3)
	{
		for (int i=0; i<BAC_CUSTOMER_UNIT_GROUP; i++)
		{
			end_temp_instance = BAC_READ_CUSTOMER_UNITS_REMAINDER + (BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i ;
			if(end_temp_instance >= BAC_CUSTOMER_UNITS_COUNT)
				end_temp_instance = BAC_CUSTOMER_UNITS_COUNT - 1;
			if(Write_Private_Data_Blocking(WRITEUNIT_T3000,(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i,end_temp_instance) > 0)
			{
				Mession_ret.Format(_T("Write Custom unit from %d to %d success."),(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				write_success_count ++ ;
				Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write Custom unit from %d to %d timeout."),(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				goto write_end_thread;
			}
			g_progress_persent = write_success_count * 100 /write_total_count;
		}


	
		for (int i=0; i<BAC_GRPHIC_LABEL_GROUP; i++)
		{
			end_temp_instance = BAC_READ_GRPHIC_LABEL_REMAINDER + (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i ;
			if(end_temp_instance >= BAC_GRPHIC_LABEL_COUNT)
				end_temp_instance = BAC_GRPHIC_LABEL_COUNT - 1;
			if(Write_Private_Data_Blocking(WRITE_GRPHIC_LABEL_COMMAND,(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance) > 0)
			{
				Mession_ret.Format(_T("Write graphic label from %d to %d success."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				write_success_count ++ ;
				Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write graphic label from %d to %d timeout."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				goto write_end_thread;
			}
			g_progress_persent = write_success_count * 100 /write_total_count;
		}


		for (int i=0; i<BAC_USER_LOGIN_GROUP; i++)
		{
			end_temp_instance = BAC_READ_USER_LOGIN_INFO_REMAINDER + (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i ;
			if(end_temp_instance >= BAC_USER_LOGIN_COUNT)
				end_temp_instance = BAC_USER_LOGIN_COUNT - 1;
			if(Write_Private_Data_Blocking(WRITEUSER_T3000,(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,end_temp_instance) > 0)
			{
				Mession_ret.Format(_T("Write user login info from %d to %d success."),(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				write_success_count ++ ;
				Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write user login info from %d to %d timeout."),(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				goto write_end_thread;
			}
			g_progress_persent = write_success_count * 100 /write_total_count;
		}




		for (int i=0; i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
		{
			end_temp_instance = BAC_ALALOG_CUSTMER_RANGE_TABLE_REMAINDER + (BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i ;
			if(end_temp_instance >= BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT)
				end_temp_instance = BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT - 1;
			if(Write_Private_Data_Blocking(WRITEANALOG_CUS_TABLE_T3000,(BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i,end_temp_instance) > 0)
			{
				Mession_ret.Format(_T("Write Analog custom table from %d to %d success."),(BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				write_success_count ++ ;
				Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write Analog custom table from %d to %d timeout."),(BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER)*i,end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				goto write_end_thread;
			}
			g_progress_persent = write_success_count * 100 /write_total_count;
		}

		if(temp_prg_version >= 4)
		{
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) > 0)
			{
				Mession_ret.Format(_T("Write device information success."));
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				write_success_count ++ ;
				Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write device information timeout."));
				SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
				goto write_end_thread;
			}
			g_progress_persent = write_success_count * 100 /write_total_count;
		}

	}



    if (temp_prg_version >= 8)	 // version 8  schedule flag time 中才加的这玩意;
    {
        for (int i = 0; i < BAC_SCHEDULE_FLAG_GROUP; i++)
        {
            end_temp_instance = BAC_READ_SCHEDULE_FLAG_REMAINDER + (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i;
            if (end_temp_instance >= BAC_SCHEDULE_COUNT)
                end_temp_instance = BAC_SCHEDULE_COUNT - 1;
            if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i, end_temp_instance) > 0)
            {
                Mession_ret.Format(_T("Write schedule flag from %d to %d success."), (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i, end_temp_instance);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
                write_success_count++;
                Sleep(SEND_COMMAND_DELAY_TIME);
            }
            else
            {
                Mession_ret.Format(_T("Write schedule flag from %d to %d timeout."), (BAC_READ_SCHEDULE_FLAG_GROUP_NUMBER)*i, end_temp_instance);
                SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
                goto write_end_thread;
            }
            g_progress_persent = write_success_count * 100 / write_total_count;
        }

    }

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,WRITEPRGFLASH_COMMAND);


	Sleep(1000);
	
	copy_data_to_ptrpanel(TYPE_ALL);

	if(Input_Window->IsWindowVisible())
		::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
	else if(Output_Window->IsWindowVisible())
		::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
	else if(Variable_Window->IsWindowVisible())
		::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
	else if(WeeklyRoutine_Window->IsWindowVisible())
		::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	else if(AnnualRoutine_Window->IsWindowVisible())
		::PostMessage(m_annual_dlg_hwnd,WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	else if(Controller_Window->IsWindowVisible())
		::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
	else if(Screen_Window->IsWindowVisible())
	{
		::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
	}
	else if(Monitor_Window->IsWindowVisible())
	{
		::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
	}
	else if(AlarmLog_Window->IsWindowVisible())
		::PostMessage(m_alarmlog_dlg_hwnd, WM_REFRESH_BAC_ALARMLOG_LIST,NULL,NULL);
	else if(Program_Window->IsWindowEnabled())
		::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);


    Mession_ret.Format(_T("Write config file success."));
    SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
    g_progress_persent = 100;

    read_write_bacnet_config = false;
    hwait_write_thread = NULL;
    return 0;
write_end_thread:
    g_progress_persent = 0;
    read_write_bacnet_config = false;
    hwait_write_thread = NULL;
    Mession_ret.Format(_T("Load config file Timeout,Please Try again."));
    SetPaneString(BAC_SHOW_MISSION_RESULTS,Mession_ret);
    AfxMessageBox(_T("Load config file Timeout,Please Try again."));
    return 2;
}

LRESULT  CMainFrame::ReadConfigFromDeviceMessageCallBack(WPARAM wParam, LPARAM lParam)
{
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    if(msg_result)
    {
        CFileFind cfindtempfile;
        if (cfindtempfile.FindFile(SaveConfigFilePath))
        {
            DeleteFile(SaveConfigFilePath);
        }
		SaveBacnetBinaryFile(SaveConfigFilePath);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Save config file success!"));
    }
    return 0;
}
LRESULT CMainFrame::Refresh_RX_TX_Count(WPARAM wParam, LPARAM lParam)
{
    int ret = (int)wParam;
    if(ret == 1)
    {
        Set_Communication_Count(1,g_bac_instance);//成功，计数+1

        //m_pTreeViewCrl->turn_item_image(selected_tree_item ,true); //2016 02 24 MARK Fance
        //SetPaneConnectionPrompt(_T("Online"));
    }
    else
        Set_Communication_Count(0,g_bac_instance);

    return 0;
}
LRESULT CMainFrame::Retry_Connect_Message(WPARAM wParam, LPARAM lParam)
{
	
	DoConnectToANode( hTreeItem_retry );
		hretryThread = NULL;
	return 0;
}

LRESULT CMainFrame::ReConnect_Message(WPARAM wParam, LPARAM lParam)
{
    for (int i = 0; i < m_product.size(); i++)
    {
        if (g_selected_serialnumber == m_product.at(i).serial_number)
        {
            hTreeItem_retry = m_product.at(i).product_item;
            break;
        }
    }


    DoConnectToANode(hTreeItem_retry);
    hretryThread = NULL;
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

        for (int i=0; i<BAC_INPUT_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Input_Info[i].invoke_id)
                Write_Config_Info.Write_Input_Info[i].task_result = true;
        }

        for (int i=0; i<BAC_OUTPUT_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Output_Info[i].invoke_id)
                Write_Config_Info.Write_Output_Info[i].task_result = true;
        }

        for (int i=0; i<BAC_VARIABLE_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Variable_Info[i].invoke_id)
                Write_Config_Info.Write_Variable_Info[i].task_result = true;
        }
        for (int i=0; i<BAC_PROGRAM_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Program_Info[i].invoke_id)
                Write_Config_Info.Write_Program_Info[i].task_result = true;
        }


        for (int i=0; i<BAC_PID_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Controller_Info[i].invoke_id)
                Write_Config_Info.Write_Controller_Info[i].task_result = true;
        }

        for (int i=0; i<BAC_SCREEN_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Screen_Info[i].invoke_id)
                Write_Config_Info.Write_Screen_Info[i].task_result = true;
        }

        for (int i=0; i<BAC_SCHEDULE_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weekly_Info[i].invoke_id)
                Write_Config_Info.Write_Weekly_Info[i].task_result = true;
        }
        for (int i=0; i<BAC_SCHEDULECODE_GOUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weeklycode_Info[i].invoke_id)
                Write_Config_Info.Write_Weeklycode_Info[i].task_result = true;
        }

        for (int i=0; i<BAC_HOLIDAY_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annual_Info[i].invoke_id)
                Write_Config_Info.Write_Annual_Info[i].task_result = true;
        }
        for (int i=0; i<BAC_HOLIDAYCODE_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annualcode_Info[i].invoke_id)
                Write_Config_Info.Write_Annualcode_Info[i].task_result = true;
        }

        for (int i=0; i<BAC_MONITOR_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Monitor_Info[i].invoke_id)
                Write_Config_Info.Write_Monitor_Info[i].task_result = true;
        }
        Show_Results = temp_cs + _T("Success!");
        SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
    }
    else
    {

        for (int i=0; i<BAC_INPUT_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Input_Info[i].invoke_id)
                Write_Config_Info.Write_Input_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_OUTPUT_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Output_Info[i].invoke_id)
                Write_Config_Info.Write_Output_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_VARIABLE_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Variable_Info[i].invoke_id)
                Write_Config_Info.Write_Variable_Info[i].task_result = false;
        }
        for (int i=0; i<BAC_PROGRAM_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Program_Info[i].invoke_id)
                Write_Config_Info.Write_Program_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_PID_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Controller_Info[i].invoke_id)
                Write_Config_Info.Write_Controller_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_SCREEN_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Screen_Info[i].invoke_id)
                Write_Config_Info.Write_Screen_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_SCHEDULE_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weekly_Info[i].invoke_id)
                Write_Config_Info.Write_Weekly_Info[i].task_result = false;
        }
        for (int i=0; i<BAC_SCHEDULECODE_GOUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Weeklycode_Info[i].invoke_id)
                Write_Config_Info.Write_Weeklycode_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_HOLIDAY_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annual_Info[i].invoke_id)
                Write_Config_Info.Write_Annual_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_HOLIDAYCODE_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Annualcode_Info[i].invoke_id)
                Write_Config_Info.Write_Annualcode_Info[i].task_result = false;
        }

        for (int i=0; i<BAC_MONITOR_GROUP; i++)
        {
            if(pInvoke->Invoke_ID==Write_Config_Info.Write_Monitor_Info[i].invoke_id)
                Write_Config_Info.Write_Monitor_Info[i].task_result = false;
        }

        Show_Results = temp_cs + _T("Fail!");
        SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
    }

    if(pInvoke)
        delete pInvoke;
    return 0;
}


DWORD WINAPI  CMainFrame::Mul_Ping_Thread(LPVOID lpVoid)
{
    CMainFrame* pDlg = (CMainFrame*)(lpVoid);

    while (get_ping_ip_network == 0) 
    {
        if (mul_ping_flag == 0)
        {
            h_mul_ping_thread = NULL;
            return 0;
        }
        Sleep(1000);
    }

    CString temp_adaper_ip = g_ipaddress_info.adapter_info.StrIP;

    CStringArray temp_array;
    SplitCStringA(temp_array, temp_adaper_ip, _T("."));
    if (temp_array.GetSize() != 4)
    {
        h_mul_ping_thread = NULL;
        return 0;
    }

    g_ipaddress_info.ip_head.Format(_T("%s.%s.%s."), temp_array.GetAt(0), temp_array.GetAt(1), temp_array.GetAt(2));

    CString temp_head_ip;
    CString temp_ip = g_ipaddress_info.ip_head;

    while (mul_ping_flag)
    {
        for (int j = 1; j < 254; j++)
        {
            if (mul_ping_flag == 0)
            {
                h_mul_ping_thread = NULL;
                return 0;
            }
            CString temp_3;
            temp_3.Format(_T("%s%d"), temp_ip, j);
            CPing p1;
            CPingReply pr1;
            if (p1.Ping1((LPCTSTR)temp_3, pr1))
            {
                TRACE(_T("%s online\r\n"), temp_3);
                g_ipaddress_info.ip_status[j] = 1;
            }
            else
            {
                TRACE(_T("%s offline\r\n"), temp_3);
                g_ipaddress_info.ip_status[j] = 0;
            }
            Sleep(10);
        }
        
    }



    Sleep(1);
    h_mul_ping_thread = NULL;
    return 0;
}

HTREEITEM hItem_for_ping;


UINT _MyPingThread(LPVOID pParam)
{
	CMainFrame* pDlg = (CMainFrame*)(pParam);


	bool find_item = false;
	CString cs_ip_address ;
	for(UINT i=0; i<pDlg->m_product.size(); i++)
	{
		if(hItem_for_ping==pDlg->m_product.at(i).product_item )
		{
			find_item = true;
			cs_ip_address = pDlg->m_product.at(i).BuildingInfo.strIp;
			break;
		}
	}
	CString strIP = cs_ip_address;// pDlg->m_strPingIP;
	Ping(strIP, pDlg);
	return 1;
}

LRESULT CMainFrame::PingDevice(WPARAM wParam, LPARAM lParam)
{
	hItem_for_ping =(HTREEITEM) wParam;
		AfxBeginThread(_MyPingThread, this);
	return true;
}


LRESULT CMainFrame::OnCreateStatusBarWindow(WPARAM wParam, LPARAM lParam)
{

    b_statusbarthreadflag = true; // enable refresh status bar status;
    statusbar =new CMyStatusbarCtrl;
    statusbar->Create(IDD_DIALOG_STATUSBAR,this);
    statusbar->ShowWindow(SW_SHOW);
    CRect temprec;
    m_wndStatusBar.GetWindowRect(&temprec);
    statusbar->SetWindowPos(this,temprec.left,temprec.top,temprec.Width(),temprec.Height(),SWP_NOACTIVATE | SWP_SHOWWINDOW );
    b_create_status = true;
    return 1;
}


LRESULT CMainFrame::OnFreshStatusBar(WPARAM wParam, LPARAM lParam)
{
    status_info*  pinfoStatusbar;
    pinfoStatusbar=(status_info*)wParam;
    m_wndStatusBar.SetPaneText(pinfoStatusbar->nIndex,pinfoStatusbar->strInfoText);
    return 1;
}


void CMainFrame::OnDestroy()
{
    mul_ping_flag = false; //关闭 ping 的命令;
    g_mstp_flag = false;
    b_statusbarthreadflag = FALSE; //close the status bar thread;
    OnDisconnect();


#if 1
	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


    for(int i=0; i<m_product.size(); i++) //用于更新 产品的状态，以便下次打开的时候直接显示上次关闭的时候的状态;
    {
        CString serial_number_temp;
        serial_number_temp.Format(_T("%u"),m_product.at(i).serial_number);
        CString execute_str;
        try
        {

            execute_str.Format(_T("update ALL_NODE set Online_Status = %d where Serial_ID = '%s'"),m_product.at(i).status,serial_number_temp);
             SqliteDBBuilding.execDML((UTF8MBSTR)execute_str);
        }
        catch(_com_error *e)
        {
            //AfxMessageBox(e->ErrorMessage());
        }

    }
    SqliteDBBuilding.closedb();
#endif
#if 1
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
    m_alarmlog_data.clear();
    m_customer_unit_data.clear();
    m_user_login_data.clear();
    m_graphic_label_data.clear();
    m_remote_point_data.clear();
    m_msv_data.clear();
    //CDialogInfo *pDialogInfo = NULL;
    try
    {
        int temp =1;

       // UpdataSlider(temp); //每次结束都会溢出 报错; 2016 - 03 - 17 杜帆 屏蔽;
        
        g_bEnableRefreshTreeView = FALSE;
        for(int i=0; i<WINDOW_TAB_COUNT ; i++)
        {
            if(pDialog[i]!=NULL)
            {
                delete pDialog[i];
                pDialog[i]=NULL;
            }
        }
        if(Add_Label_Window !=NULL)
        {
            delete Add_Label_Window;
            Add_Label_Window = NULL;
        }

        if(statusbar != NULL)
        {
            delete statusbar;
            statusbar = NULL;
        }

        if(DebugWindow !=NULL)
        {
            delete DebugWindow;
            DebugWindow = NULL;
        }

        for (int i=0; i<14; i++)
        {
            UnregisterHotKey(GetSafeHwnd(), m_MainHotKeyID[i]);
        }
#ifdef USE_MOD_SHIFT_DF
        UnregisterHotKey(GetSafeHwnd(), 1111);
        UnregisterHotKey(GetSafeHwnd(), 1112);
#endif


        g_killMultiReadEvent.SetEvent();

        Sleep(500);//wait for the end of the thread.
		if (m_pFreshMultiRegisters != NULL)
		{
			if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 1000) != WAIT_OBJECT_0)

				//Sleep(500);//wait for the end of the thread.

				if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 3000) == WAIT_OBJECT_0)
				{

				}
				else
				{
					BOOL bRet = TerminateThread(m_pFreshMultiRegisters->m_hThread,0);
					m_pFreshMultiRegisters=NULL;
				}

		}
		if (m_pFreshTree != NULL)

		{
			if (WaitForSingleObject(m_pFreshTree->m_hThread, 1000) != WAIT_OBJECT_0)

				if (WaitForSingleObject(m_pFreshTree->m_hThread, 3000) == WAIT_OBJECT_0)
				{

				}
				else
				{
					BOOL bRet = TerminateThread(m_pFreshTree->m_hThread,0);
					m_pFreshTree=NULL;
				}

		}

        //if (CM5_hThread != NULL)
        //{
        //    system_connect_info.mstp_status = 0;
        //    TerminateThread(CM5_hThread, 0);
        //    CM5_hThread = NULL;
        //}
        if (CM5_hThread != NULL)
        {
            Sleep(2000);
        }
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
            b_pause_refresh_tree = true;
            if (WaitForSingleObject(hp, 1000) != WAIT_OBJECT_0)

                if (WaitForSingleObject(m_pRefreshThread->m_hThread, 100) == WAIT_OBJECT_0)
                {
                }
                else
                {
                    TerminateThread(m_pRefreshThread->m_hThread, 0);
                }
        }


        SetCommunicationType(0);
        close_com(); // added by zgq:12-16-2011
        SetCommunicationType(1);
        close_com();

        close_T3000_log_file();


        //if (m_pDialogInfo!=NULL)
        //{
        //    delete m_pDialogInfo;
        //    m_pDialogInfo = NULL;
        //}
#endif
        for (int nView = 1; nView < NUMVIEWS; nView++)
        {
            CView* pActiveView = GetActiveView();
            if ((m_pViews[nView] != NULL) && (pActiveView!= m_pViews[nView]))
            {
                delete []   m_pViews[nView];
                m_pViews[nView] = NULL;
            }
            
        }
        

    }
    catch (...)//这个无效，当pDialogInfo->Create(IDC_STATIC_INFO,this);中的ID写错时，这个函数没有throw抛出错误，所以捕获不到
    {
        Sleep(1);
    }
#ifdef DEBUG
    exit(1);
#endif // DEBUG


    CFrameWndEx::OnDestroy();
    
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    
    if(message==WM_MYMSG_REFRESHBUILDING)
    {
        //Sleep(1000);
        //AfxMessageBox(_T("There is no default building,please select a building First."));
        BOOL bTemp = g_bEnableRefreshTreeView ;
        g_bEnableRefreshTreeView = FALSE;
		if(m_pTreeViewCrl->m_hWnd == NULL)
			return 0;
        ScanTstatInDB();

        int temp_select_serial = m_pTreeViewCrl->GetSelectSerialNumber();
        bool find_serial_number_device = false;	//如果重新加载数据库，没有发现选中的那个设备 序列号，就说明可能被删掉了;需要退出当前界面 切换至初始界面;
        HTREEITEM temp_htree;
        for (int i=0; i<m_product.size(); i++)
        {
            if(temp_select_serial == m_product.at(i).serial_number)
            {
                temp_htree = m_product.at(i).product_item;
                m_pTreeViewCrl->SetSelectItem(temp_htree);
                find_serial_number_device = true;
                selected_product_index = i;//重新加载 产品列表的时候 选中上次选中的设备;
                break;
            }
        }

        if(!find_serial_number_device)
            SwitchToPruductType(DLG_DIALOG_DEFAULT_BUILDING);

        g_bEnableRefreshTreeView |= bTemp;
        return 0;
    }
    else if (message==WM_SHOW_PANNELINFOR)
    {
        CString* pStr = (CString*)(wParam);
        int ret = (int)lParam;
        SetPaneString(ret,*pStr);
        delete pStr;
        return 0;
    }
    else if (message == WM_MAIN_MSG_SCAN_BAC)
    {
        _Bac_Scan_Com_Info *pbac_iam = (_Bac_Scan_Com_Info *)wParam;
        if (pbac_iam == NULL)
            return 0;
        CString temp_cs;
        temp_cs.Format(_T("protocol = %d ,obj = %d, ip = %d.%d.%d.%d, port = %d"), pbac_iam->nprotocol, pbac_iam->device_id, 
                                                                     pbac_iam->ipaddress[0], pbac_iam->ipaddress[1], pbac_iam->ipaddress[2], pbac_iam->ipaddress[3],
            pbac_iam->ipaddress[4]*256 + pbac_iam->ipaddress[5] );
        //SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_cs);

        bool find_exsit = false;
        for (int i = 0;i < (int)m_product.size();i++)
        {
            if (m_product.at(i).object_instance == pbac_iam->device_id)
            {
                find_exsit = true;
                break;
            }
        }
        CString product_name;
        if (find_exsit == false)
        {
            BACNET_APPLICATION_DATA_VALUE temp_value;
            int invoke_id = Bacnet_Read_Properties_Blocking(pbac_iam->device_id, (BACNET_OBJECT_TYPE)OBJECT_DEVICE, pbac_iam->device_id, PROP_OBJECT_NAME, temp_value, 3);
            if (invoke_id >= 0)
            {
                CString n_temp;
                if (temp_value.tag == TPYE_BACAPP_CHARACTER_STRING)
                {
                    Sleep(1);
                    MultiByteToWideChar(CP_ACP, 0, (char *)temp_value.type.Character_String.value,
                        (int)strlen((char *)temp_value.type.Character_String.value) + 1,
                        product_name.GetBuffer(MAX_PATH), MAX_PATH);
                    product_name.ReleaseBuffer();
                }
            }
            product_name = product_name.Trim();
            if (product_name.IsEmpty())
            {
                product_name = _T("Third-party Device");
            }
            
            SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_cs);
            CppSQLite3DB SqliteDBBuilding;
            SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
            CString strSql;
            int serial_id = rand();
            CString str_serialid;
            str_serialid.Format(_T("%u"), serial_id);
            CString product_class_id = _T("254");
            CString str_ip_address;
            CString str_n_port;
            str_ip_address.Format(_T("%u.%u.%u.%u"), pbac_iam->ipaddress[0], pbac_iam->ipaddress[1], pbac_iam->ipaddress[2], pbac_iam->ipaddress[3]);
            str_n_port.Format(_T("%u"), pbac_iam->ipaddress[4] * 256 + pbac_iam->ipaddress[5]);
            CString temp_pro2;
            temp_pro2.Format(_T("%d"), PROTOCOL_THIRD_PARTY_BAC_BIP);
            CString str_panel_number;
            str_panel_number.Format(_T("%d"), pbac_iam->macaddress);
            CString str_object_instance;
            str_object_instance.Format(_T("%u"), pbac_iam->device_id);
            strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Screen_Name,Bautrate,Background_imgID,Com_Port,Protocol,Online_Status,Panal_Number,Object_Instance)   values('" + m_strCurMainBuildingName + "','" + m_strCurSubBuldingName + "','" + str_serialid + "','floor1','room1','" + product_name + "','" + product_class_id + "','""','" + str_ip_address + "','Modbus_and_Bacnet','" + str_n_port + "','" + temp_pro2 + "','1','" + str_panel_number + "' ,'" + str_object_instance + "' )"));
            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            SqliteDBBuilding.closedb();
            PostMessage(WM_MYMSG_REFRESHBUILDING, 0, 0);
        }

        if (pbac_iam)
            delete pbac_iam;
    }
    else if (message == WM_MAIN_MSG_UPDATE_PRODUCT_TREE)
    {
        int temp_status = (int)lParam;
        //Select
        m_pTreeViewCrl->turn_item_image(selected_product_Node.product_item, temp_status);
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
	CppSQLite3DB SqliteDB;
	CppSQLite3Table table;
	CppSQLite3Query q;
    try
    {


        SqliteDB.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

        CString strSerial;
        strSerial.Format(_T("%u"),g_selected_serialnumber);
        strSerial.Trim();
        CString strsql;
        strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
        
        q = SqliteDB.execQuery((UTF8MBSTR)strsql);
        if(!q.eof())
        {
            CString str_temp;
            str_temp.Empty();
            _variant_t temp_variant;

            
            g_strSensorName=q.getValuebyName(L"SENSORNAME",L"Internal Sensor");

             
            g_strInName1=q.getValuebyName(L"INPUT1",L"Input 1");

            
            g_strInName2=q.getValuebyName(L"INPUT2",L"Input 2");

            g_strInName3=q.getValuebyName(L"INPUT3",L"Input 3");

           g_strInName4=q.getValuebyName(L"INPUT4",L"Input 4");
           g_strInName5=q.getValuebyName(L"INPUT5",L"Input 5");
           g_strInName6=q.getValuebyName(L"INPUT6",L"Input 6");  
		   g_strInName7=q.getValuebyName(L"INPUT7",L"Input 7");
           g_strInName8=q.getValuebyName(L"INPUT8",L"Input 8");
           g_strInHumName=q.getValuebyName(L"INPUT9",L"Input 9");


            g_strOutName1=q.getValuebyName(L"OUTPUT1",L"Output 1");
			 


			g_strOutName2=q.getValuebyName(L"OUTPUT2",L"Output 2");

			g_strOutName3=q.getValuebyName(L"OUTPUT3",L"Output 3");

			g_strOutName4=q.getValuebyName(L"OUTPUT4",L"Output 4");
			g_strOutName5=q.getValuebyName(L"OUTPUT5",L"Output 5");
			g_strOutName6=q.getValuebyName(L"OUTPUT6",L"Output 6");  
			g_strOutName7=q.getValuebyName(L"OUTPUT7",L"Output 7");
		 

            g_strInHumName = _T("Humidity Sensor");
            g_strInCO2=_T("CO2 Sensor");
            g_strLightingSensor=_T("Lighting Sensor");
        }
        else
        {
            g_strSensorName =_T("Internal Temp Sensor");
            g_strInName1=_T("Input 1");
            g_strInName2=_T("Input 2");
            g_strInName3=_T("Input 3");
            g_strInName4=_T("Input 4");
            g_strInName5=_T("Input 5");
            g_strInName6=_T("Input 6");
            g_strInName7=_T("Input 7");
            g_strInName8=_T("Input 8");
            g_strLightingSensor=_T("Lighting Sensor");
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
       
     

    }
    catch (...)
    {

    }



}

void CMainFrame::OnHelp()
{
    HWND hWnd;
    hWnd =  ::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
}
 

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{

    if (m_pDialogInfo!=NULL&&m_pDialogInfo->IsWindowVisible())
    {
        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            m_pDialogInfo->ShowWindow(SW_HIDE);
        }
    }


    if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == m_wndStatusBar.GetSafeHwnd())
    {
        CRect rect;
        CPoint pt;
        pt = pMsg->pt;
        m_wndStatusBar.GetItemRect(3,&rect);
        m_wndStatusBar.ClientToScreen(&rect);
        if(PtInRect(&rect,pt))
        {
            CString strHelp=g_strExePth+_T("T3000.log");
            ShellExecute(NULL, _T("open"), strHelp, NULL, NULL, SW_SHOWNORMAL);
        }
        m_wndStatusBar.GetItemRect(2,&rect);
        m_wndStatusBar.ClientToScreen(&rect);
        if(PtInRect(&rect,pt))
        {
        }
    }
    if((pMsg->message == WM_KEYDOWN ) || (pMsg->message == WM_LBUTTONDOWN) || (pMsg->message == WM_LBUTTONUP))
    {

        no_mouse_keyboard_event_enable_refresh = false;
        time_click = time(NULL);
    }

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_F5) )
	{
		refresh_tree_status_immediately = true;
		TRACE(_T(" refresh_tree_status_immediately = true \n"));
	}

    if(pMsg->message == WM_KEYDOWN  )
    {
        if(((GetAsyncKeyState( VK_LCONTROL ) & 0x8000))&&(pMsg->wParam ==VK_F2))
        {
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
            //OnToolErease();
            return 1;
        }
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
    for(int i=0; i<12; i++) //Modify by Fance , tstat 6 has more register than 512;
    {
        Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100);
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
    CString strUpdateSqlOn;
    CppSQLite3DB SqliteDBT3000;
    CppSQLite3DB SqliteDBBuilding;
    CppSQLite3Table table;
    CppSQLite3Query q;
    SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
    m_strCfgFilePath = g_strExePth + c_strCfgFileName;
    //m_cfgFileHandler.CreateConfigFile(m_strCfgFilePath);
    m_cfgFileHandler.CreateConfigFile();
    CString filename;
    CString flashmethod;
    CString id;
    CString comport;
    CString BD;
    CString ip;
    CString ipport;
    BOOL bOnLine = FALSE;
    UINT nSerialNumber = 0;
    int Device_Type = 0;
    MODE_SUPPORT_PTRANSFER = 0;
    check_revert_daxiaoduan = 0; //换设备情况大小端反转为默认不反转
    g_protocol_support_ptp = PROTOCOL_UNKNOW; //默认点击任何设备  ，初始化  为不知道是否支持;
    g_output_support_relinquish = 0;
    int Scan_Product_ID;
    unsigned short read_data[10];
    CString strTemp;
    CString subnote;
    CString subID;
    unsigned char n_show_register_list = 0; //对于没有界面的设备，直接显示寄存器列表;
    m_cfgFileHandler.ReadFromCfgFileForAll(
        filename,
        flashmethod,
        id,
        comport,
        BD,
        ip,
        ipport,
        subnote,
        subID
		);



    CString g_strT3000LogString;
    no_mouse_keyboard_event_enable_refresh = false;

    if (User_Login_Window != NULL)
    {
        if (User_Login_Window->IsWindowVisible())	//如果客户点击其他的 设备，之前还停留在 登入界面就先关掉登入界面，后面再判断要不要登入;
            User_Login_Window->ShowWindow(SW_HIDE);
    }
    if (ScreenEdit_Window != NULL)
    {
        if (ScreenEdit_Window->IsWindowVisible())
        {
            for (int i = 0; i<SCREEN_HOTKEY_COUNT; i++)
            {
                UnregisterHotKey(ScreenEdit_Window->m_hWnd, m_screenHotKeyID[i]);
            }
            UnregisterHotKey(ScreenEdit_Window->m_hWnd, KEY_INSERT);
            ::PostMessage(m_screen_dlg_hwnd, WM_SCREENEDIT_CLOSE, NULL, NULL);
            ::PostMessage(ScreenEdit_Window->m_hWnd, WM_CLOSE, NULL, NULL);
        }
        ScreenEdit_Window->ShowWindow(SW_HIDE);
        ScreenEdit_Window = NULL;
    }

    MainFram_hwd = this->m_hWnd;
    //20120420
    CDialog_Progess* pDlg = new CDialog_Progess(this, 1, 100);
    //创建对话框窗口
    pDlg->Create(IDD_DIALOG10_Progress, this);

    pDlg->ShowProgress(0, 0);
    old_tx_count = persent_array_count = 0;
    RECT RECT_SET1;
    GetClientRect(&RECT_SET1);
    pDlg->MoveWindow(RECT_SET1.left + 400, RECT_SET1.bottom - 19, RECT_SET1.right / 2 + 20, 20, 1);
    pDlg->ShowWindow(SW_HIDE);
    //20120420
    float flagsoftwareversion;
    HTREEITEM hSelItem = hTreeItem;
    int nCounts = m_product.size();

    int nSelectID = -1;
    UINT nSelectSerialNumber;
    bac_gloab_panel = 0;
    CString temp_serial_number;
    for (UINT i = 0; i<m_product.size(); i++)
    {
        if (hSelItem == m_product.at(i).product_item)
        {
            hTreeItem_retry = hTreeItem;
            if (hLastTreeItem != hSelItem)
            {
                g_llerrCount = g_llTxCount = g_llRxCount = 0;//click tree, clear all count;
                hLastTreeItem = hSelItem;
            }

            CString temp_cs_obj;
            temp_cs_obj.Format(_T("%u"), m_product.at(i).object_instance);
            WritePrivateProfileStringW(_T("Yabe"), _T("Auto"), _T("1"), g_configfile_path);
            WritePrivateProfileStringW(_T("Yabe"), _T("ObjInstance"), temp_cs_obj, g_configfile_path);
            WritePrivateProfileStringW(_T("Yabe"), _T("LocalPCIPaddress"), m_product.at(i).NetworkCard_Address, g_configfile_path);

            g_llTxCount = g_llTxCount + 1;
            Scan_Product_ID = m_product.at(i).product_class_id;
            g_strT3000LogString.Format(_T("Trying to connect to %s:%d"), GetProductName(m_product.at(i).product_class_id), m_product.at(i).serial_number);
            //write_T3000_log_file(g_strT3000LogString);
            CString* pstrInfo = new CString(g_strT3000LogString);
            ::SendMessage(MainFram_hwd, WM_SHOW_PANNELINFOR, WPARAM(pstrInfo), LPARAM(3));

            int nID = -1;
            int nTRet = -1;
            //g_tstat_id=m_product.at(i).product_id;
            selected_product_Node = m_product.at(i);

            m_pTreeViewCrl->SetSelectItem(hTreeItem);//在线的时候才将颜色变红;
            m_pTreeViewCrl->SetSelectSerialNumber(selected_product_Node.serial_number);
            g_selected_serialnumber = selected_product_Node.serial_number;
            g_bac_instance = NULL;
            g_selected_product_id = selected_product_Node.product_class_id;
            g_mstp_deviceid = selected_product_Node.object_instance;
            SetCommandDelayTime(g_selected_product_id);


            CString temp_csa;
            temp_csa = selected_product_Node.BuildingInfo.strComPort;
            temp_csa = temp_csa.Right(temp_csa.GetLength() - 3);
            g_bac_instance = m_product.at(i).object_instance;
            g_selected_serialnumber = m_product.at(i).serial_number;
            g_mac = m_product.at(i).panel_number;
            bac_gloab_panel = g_mac;
            g_gloab_bac_comport = m_product.at(i).ncomport;
            g_gloab_bac_baudrate = m_product.at(i).baudrate;
            //if ((selected_product_Node.nhardware_info & 0x02) == 2) //说明是wifi 连接 ，延时要加长，并关闭后台刷新;
            //{
            //    n_wifi_connection = true; //关闭后台刷新;
            //    SEND_COMMAND_DELAY_TIME = 300;
            //    SetCommunicationType(1);
            //    SetResponseTime(SEND_COMMAND_DELAY_TIME);
            //}
            //else
            //{
            //    SEND_COMMAND_DELAY_TIME = 100;
            //    n_wifi_connection = false;
            //}
            selected_product_index = i;//记录目前选中的是哪一个 产品;用于后面自动更新firmware;
            selected_tree_item = hTreeItem;
            Statuspanel.Empty();
            if (selected_product_Node.note_parent_serial_number == 0)
            {
                if ((selected_product_Node.panel_number != 0)&& (selected_product_Node.panel_number != 255))
                {
                    Station_NUM = selected_product_Node.panel_number;
                    Statuspanel.Format(_T("%u"), selected_product_Node.panel_number);
                }
                else
                {
                    Station_NUM = selected_product_Node.product_id;
                    Statuspanel.Format(_T("%u"), selected_product_Node.product_id);
                }
                g_mac = bac_gloab_panel = Station_NUM;
            }
            else
            {
                for (int z = 0; z<m_product.size(); z++)
                {
                    if (selected_product_Node.note_parent_serial_number == m_product.at(z).serial_number)
                    {
                        Statuspanel.Format(_T("%u-%u"), m_product.at(z).panel_number, selected_product_Node.product_id);
                    }
                }
            }
            if (m_product.at(i).protocol == PROTOCOL_VIRTUAL)
            {
                offline_mode = true;
                set_offline_mode(offline_mode);

                CString offline_folder;
                offline_folder = g_strBuildingFolder + m_strCurMainBuildingName;

                offline_folder = offline_folder + _T("\\VirtualDeviceData");

                CString virtual_prg_filename;
                virtual_prg_filename.Format(_T("%d"), m_product.at(i).serial_number);
                offline_prg_path = offline_folder + _T("\\") + virtual_prg_filename + _T(".prog");
            }
            else
            {
                offline_mode = false;
                set_offline_mode(offline_mode);
            }
            //if(1)//GSM  模块

#ifdef USE_THIRD_PARTY_FUNC
            if (m_product.at(i).protocol == PROTOCOL_THIRD_PARTY_BAC_BIP)
            {
                g_protocol = PROTOCOL_THIRD_PARTY_BAC_BIP;

                Initial_bac(0, selected_product_Node.NetworkCard_Address);
                Sleep(50);
                Send_WhoIs_Global(-1, -1);
                Sleep(50);

                g_mstp_deviceid = selected_product_Node.object_instance;
                Device_Type = selected_product_Node.product_class_id;
                g_protocol = PROTOCOL_THIRD_PARTY_BAC_BIP;
                SwitchToPruductType(DLG_DIALOG_THIRD_PARTY_BAC);

                SqliteDBT3000.closedb();

                g_bPauseMultiRead = true;


                goto do_connect_success;
            }
#endif

            if ((m_product.at(i).protocol != MODBUS_BACNET_MSTP) &&
                (m_product.at(i).protocol != PROTOCOL_GSM) &&
                (m_product.at(i).protocol != PROTOCOL_REMOTE_IP) &&
                (m_product.at(i).protocol != MODBUS_RS485))
            {
                if ((!m_product.at(i).BuildingInfo.strIp.IsEmpty()) && (m_product.at(i).protocol != PROTOCOL_MSTP_TO_MODBUS))
                {
                    if (m_product.at(i).BuildingInfo.strProtocol.CompareNoCase(_T("Remote Device")) != 0)
                    {
                        CScanDlg scandlg;
                        if (!offline_mode && (!scandlg.TestPing(m_product.at(i).BuildingInfo.strIp)))
                        {
                            //尝试Ping数据库中保存的IP地址失败 做如下动作     去掉进度条
                            pDlg->ShowWindow(SW_HIDE);
                            if (pDlg)
                                delete pDlg;//20120220
                            pDlg = NULL;
                            BOOL is_OK = FALSE;

                            if (m_product.at(i).status == true)
                            {
                                if (CheckTheSameSubnet(m_product.at(i).NetworkCard_Address, m_product.at(i).BuildingInfo.strIp) == false)
                                {
                                    CTroubleShootDlg dlg;
                                    dlg.SetNode(m_product.at(i));
                                    if (dlg.DoModal() == IDOK)
                                    {
                                        is_OK = dlg.b_changeip_ok;
                                        refresh_tree_status_immediately = true;//在改完IP后立刻在去扫描，更新数据库;
                                    }
                                    return;
                                }
                                else
                                {
                                    g_llTxCount = g_llTxCount + 4;
                                    g_llerrCount = g_llerrCount + 4;
                                    bac_select_device_online = false;
                                    m_pTreeViewCrl->turn_item_image(hSelItem, false);//Can't connect to the device , it will show disconnection;
                                    m_product.at(i).status_last_time[0] = false;
                                    m_product.at(i).status_last_time[1] = false;
                                    m_product.at(i).status_last_time[2] = false;
                                    m_product.at(i).status_last_time[3] = false;
                                    m_product.at(i).status_last_time[4] = false;
                                    m_product.at(i).status = false;

                                    //MessageBox(_T("Device is offline!"));	//Ping 不通 ， 还在一个网段 ， 还显示在线; 其实不在线;

                                    //连不上时，发送ping命令，显示出来.
                                    ::PostMessage(MainFram_hwd, WM_PING_MESSAGE, (WPARAM)hTreeItem, NULL);

#if 0      //T3 无法连接  需要额外处理函数诊断问题
                                    PostMessage(WM_TROUBLESHOOT_MSG, 0, 0);
                                    if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                                    {
                                        m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Device is offline!"));
                                        m_pDialogInfo->ShowWindow(SW_SHOW);
                                    }
                                    /*  while(m_pDialogInfo->IsWindowVisible()){
                                    Sleep(50);
                                    continue;
                                    }*/

                                    goto do_conncet_failed;
#endif
                                    HideBacnetWindow();
                                    return;
                                }

                            }
                            else
                            {
                                g_llTxCount = g_llTxCount + 4;
                                g_llerrCount = g_llerrCount + 4;
                                bac_select_device_online = false;
                                m_pTreeViewCrl->turn_item_image(hSelItem, false);//Can't connect to the device , it will show disconnection;
                                m_product.at(i).status_last_time[0] = false;
                                m_product.at(i).status_last_time[1] = false;
                                m_product.at(i).status_last_time[2] = false;
                                m_product.at(i).status_last_time[3] = false;
                                m_product.at(i).status_last_time[4] = false;
                                m_product.at(i).status = false;

                                ::PostMessage(MainFram_hwd, WM_PING_MESSAGE, (WPARAM)hTreeItem, NULL);
#if 0      //T3 无法连接 需要额外处理函数诊断问题
                                //MessageBox(_T("Device is offline!"));
                                if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                                {
                                    m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Device is offline!"));
                                    m_pDialogInfo->ShowWindow(SW_SHOW);
                                }
                                //                             while(m_pDialogInfo->IsWindowVisible()){
                                //                                 Sleep(50);
                                //                                 continue;
                                //                             }
                                goto do_conncet_failed;
#endif
                                HideBacnetWindow();
                                return;
                            }
                            if (!is_OK)
                            {
                                CString strTitle;
                                strTitle.Format(_T("Can't connect to %s,Ping %s -> Noresponse"), m_product.at(i).BuildingInfo.strIp, m_product.at(i).BuildingInfo.strIp);
                                m_pTreeViewCrl->turn_item_image(hSelItem, false);//Can't connect to the device , it will show disconnection;
                                m_product.at(i).status_last_time[0] = false;
                                m_product.at(i).status_last_time[1] = false;
                                m_product.at(i).status_last_time[2] = false;
                                m_product.at(i).status_last_time[3] = false;
                                m_product.at(i).status_last_time[4] = false;
                                m_product.at(i).status = false;
                                g_llTxCount = g_llTxCount + 4;
                                g_llerrCount = g_llerrCount + 4;
                                bac_select_device_online = false;
                                //MessageBox(strTitle);
                                if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                                {
                                    m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(strTitle);
                                    m_pDialogInfo->ShowWindow(SW_SHOW);
                                }
                                //                             while(m_pDialogInfo->IsWindowVisible()){
                                //                                 Sleep(50);
                                //                                 continue;
                                //                             }
                                goto do_conncet_failed;
                                return;
                            }
                            else
                            {
                                g_llTxCount = g_llTxCount + 4;
                                g_llerrCount = g_llerrCount + 4;
                                bac_select_device_online = true;
                                m_pTreeViewCrl->turn_item_image(hSelItem, TRUE);//Can't connect to the device , it will show disconnection;
                                m_product.at(i).status_last_time[0] = TRUE;
                                m_product.at(i).status_last_time[1] = false;
                                m_product.at(i).status_last_time[2] = false;
                                m_product.at(i).status_last_time[3] = false;
                                m_product.at(i).status_last_time[4] = false;
                                m_product.at(i).status = false;

                            }

                            goto do_conncet_failed;
                            return;
                        }
                        else
                        {
                            ::PostMessage(MainFram_hwd, MY_RX_TX_COUNT, 1, 0);
                            CString IP = m_product.at(i).BuildingInfo.strIp;
                            int Port = m_product.at(i).ncomport;
                            SetCommunicationType(1);
                            if ((selected_product_Node.nhardware_info & 0x02) == 2) //说明是wifi 连接 ，延时要加长，并关闭后台刷新;
                            {
                                if (((selected_product_Node.product_class_id == PM_TSTAT10) && (selected_product_Node.software_version >= 52.3)) ||
                                    ((selected_product_Node.product_class_id == PM_MINIPANEL_ARM) && (selected_product_Node.software_version >= 52.3))||
                                    ((selected_product_Node.product_class_id == PM_TSTAT8) && (selected_product_Node.software_version >= 10.0)) ||
                                    ((selected_product_Node.product_class_id == PM_TSTAT9) && (selected_product_Node.software_version >= 10.0))
                                   )
                                {
                                    n_wifi_connection = false; //新改的 
                                    MODE_SUPPORT_PTRANSFER = 1;//支持bip ptransfer
                                    SEND_COMMAND_DELAY_TIME = 100;
                                    SetResponseTime(SEND_COMMAND_DELAY_TIME);
                                }
                                //else if (
                                //    )
                                //{
                                //    n_wifi_connection = true; //新改的 
                                //    SEND_COMMAND_DELAY_TIME = 350;
                                //    SetResponseTime(SEND_COMMAND_DELAY_TIME);
                                //}
                                else
                                {
                                    n_wifi_connection = true; //关闭后台刷新;
                                    SEND_COMMAND_DELAY_TIME = 350;// DEBUG_DELAY_TIME;
                                    SetResponseTime(SEND_COMMAND_DELAY_TIME);
                                }
                                

                            }
                            else
                            {
                                SEND_COMMAND_DELAY_TIME = 100;
                                n_wifi_connection = false;
                                SetResponseTime(SEND_COMMAND_DELAY_TIME);
                            }
#if 0
                            if ((!offline_mode) && (!Open_Socket_Retry(IP, Port)))
                            {
                                g_llTxCount = g_llTxCount + 4;
                                g_llerrCount = g_llerrCount + 4;
                                bac_select_device_online = false;
                                // MessageBox(_T("Connect failed!Please try again!"));
                                if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                                {
                                    m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Connect failed!Please try again!"));
                                    m_pDialogInfo->ShowWindow(SW_SHOW);
                                }
                                m_pTreeViewCrl->turn_item_image(hSelItem, false);//Can't connect to the device , it will show disconnection;
                                m_product.at(i).status_last_time[0] = false;
                                m_product.at(i).status_last_time[1] = false;
                                m_product.at(i).status_last_time[2] = false;
                                m_product.at(i).status_last_time[3] = false;
                                m_product.at(i).status_last_time[4] = false;
                                m_product.at(i).status = false;
                                goto do_conncet_failed;
                                return;

                            }
#endif
//                            else
//                            {
                                close_com();
//                            }
                        }
                    }


                }
            }
            bacnet_device_type = selected_product_Node.product_class_id;

            product_register_value[7] = selected_product_Node.product_class_id;

            g_serialNum = selected_product_Node.serial_number;
            if ((selected_product_Node.product_class_id == PM_CM5) ||
                (selected_product_Node.product_class_id == PM_MINIPANEL) ||
                (selected_product_Node.product_class_id == PM_TSTAT10) ||
                (selected_product_Node.product_class_id == PM_MINIPANEL_ARM)
                /*|| */
                /*(product_Node.product_class_id == PM_T38AI8AO6DO) */)	//如果是CM5或者MINIPANEL 才有 bacnet协议;
            {

                product_type = selected_product_Node.product_class_id;
#if 1//Modbus Poll Config 
                m_nStyle = 1;
                CString	g_configfile_path = g_strExePth + _T("T3000_config.ini");
                CFileFind fFind;
                if (!fFind.FindFile(g_configfile_path))
                {
                    CString strInfo;
                    strInfo.Format(_T("%d"), 1);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Connection Type"), strInfo, g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("IP Address"), selected_product_Node.BuildingInfo.strIp, g_configfile_path);
                    strInfo.Format(_T("%d"), selected_product_Node.ncomport);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("IP Port"), strInfo, g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Response Timeout"), _T("1000"), g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Delay Between Time"), _T("1000"), g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Connect Timeout"), _T("3000"), g_configfile_path);
                }
                else
                {
                    CString strInfo;
                    strInfo.Format(_T("%d"), 1);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Connection Type"), strInfo, g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("IP Address"), selected_product_Node.BuildingInfo.strIp, g_configfile_path);
                    strInfo.Format(_T("%d"), selected_product_Node.ncomport);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("IP Port"), strInfo, g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Response Timeout"), _T("1000"), g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Delay Between Time"), _T("1000"), g_configfile_path);
                    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Connect Timeout"), _T("3000"), g_configfile_path);
                }

#endif

                SetPaneString(1, _T(""));
                if (selected_product_Node.product_class_id == PM_CM5)
                    bacnet_device_type = PRODUCT_CM5;
                if (selected_product_Node.product_class_id == PM_MINIPANEL_ARM)
                    bacnet_device_type = PM_MINIPANEL_ARM;
                else if (selected_product_Node.product_class_id == PM_T38AI8AO6DO)
                    bacnet_device_type = T38AI8AO6DO;
                else if (selected_product_Node.product_class_id == PM_T322AI)
                    bacnet_device_type = PID_T322AI;
                else if (selected_product_Node.product_class_id == PWM_TRANSDUCER)
                    bacnet_device_type = PWM_TRANSDUCER;
                else if (selected_product_Node.product_class_id == PM_T3PT12)
                    bacnet_device_type = PID_T3PT12;
                else if (selected_product_Node.product_class_id == PM_T36CTA)
                    bacnet_device_type = PID_T36CTA;
                else if (selected_product_Node.product_class_id == PM_T3_LC)
                    bacnet_device_type = PM_T3_LC;

                if (m_product.at(i).protocol == PROTOCOL_GSM)
                {
                    if (!TCP_Server_Running)
                    {
                        if (pDlg)
                            delete pDlg;//20120220
                        pDlg = NULL;
                        MessageBox(_T("Trying to connect a device with GSM module ,Please start the server!\r\nMenu -> Miscellaneous -> Gsmconnection"), _T("Information"), MB_OK | MB_ICONINFORMATION);
                        goto do_conncet_failed;
                        return;
                    }
                    else
                    {
                        if (m_tcp_connect_info.size() == 0)
                        {
                            if (pDlg)
                                delete pDlg;//20120220
                            pDlg = NULL;
                            MessageBox(_T("This device does't connect to T3000 server yet."), _T("Information"), MB_OK | MB_ICONINFORMATION);
                            goto do_conncet_failed;
                            return;
                        }
                        else
                        {
                            int found_device = -1;
                            for (int z = 0; z<m_tcp_connect_info.size(); z++)
                            {
                                if (m_tcp_connect_info.at(z).serialnumber == m_product.at(i).serial_number)
                                {
                                    found_device = z;
                                    break;
                                }
                            }
                            if (found_device >= 0)	//发现 已经连接的设备;
                            {
                                bip_set_socket(m_tcp_connect_info.at(found_device).client_socket);
                                SEND_COMMAND_DELAY_TIME = 1000;
                            }
                            else
                            {
                                if (pDlg)
                                    delete pDlg;//20120220
                                pDlg = NULL;
                                MessageBox(_T("This device does't connect to T3000 server yet."), _T("Information"), MB_OK | MB_ICONINFORMATION);
                                goto do_conncet_failed;
                                return;
                            }

                        }

                    }

                }
                else if (m_product.at(i).protocol == MODBUS_BACNET_MSTP)
                {
                    g_protocol = MODBUS_BACNET_MSTP;
                    SEND_COMMAND_DELAY_TIME = 200;
                    SwitchToPruductType(DLG_BACNET_VIEW);
                    g_gloab_bac_comport = m_product.at(i).ncomport;
                    pDlg->ShowWindow(SW_HIDE);
                    if (pDlg)
                        delete pDlg;//20120220
                    pDlg = NULL;
                    m_pTreeViewCrl->SetSelectItem(hTreeItem);//在线的时候才将颜色变红;
                    m_pTreeViewCrl->SetSelectSerialNumber(selected_product_Node.serial_number);
                    g_selected_serialnumber = m_product.at(i).serial_number;
                    goto do_connect_success;
                    return;

                }
                else if (m_product.at(i).protocol == MODBUS_RS485) //如果客户是将Minipanel的 串口连接至PC;
                {
                    g_protocol = MODBUS_RS485;
                    close_bac_com(); //关闭 bacnet mstp
                    SetCommunicationType(0);
                    g_tstat_id = selected_product_Node.product_id;
                    SEND_COMMAND_DELAY_TIME = 200;
                    SwitchToPruductType(DLG_BACNET_VIEW);

                    pDlg->ShowWindow(SW_HIDE);
                    if (pDlg)
                        delete pDlg;//20120220
                    pDlg = NULL;
                    m_pTreeViewCrl->SetSelectItem(hTreeItem);//在线的时候才将颜色变红;
                    m_pTreeViewCrl->SetSelectSerialNumber(selected_product_Node.serial_number);
                    g_selected_serialnumber = m_product.at(i).serial_number;
                    goto do_connect_success;
                    return;
                }
                else
                {
                    BOOL is_local = true;
                    if (selected_product_Node.protocol == PROTOCOL_REMOTE_IP)
                        is_local = false;
                    //BOOL is_local = IP_is_Local(product_Node.BuildingInfo.strIp);
                    if (is_local == false)	//判断是否是本地IP，不是本地的就要连接到远端的，远端的 Who  is  广播发布过去的;
                    {
                        m_is_remote_device = true;
                        ((CDialogCM5_BacNet*)m_pViews[m_nCurView])->Set_remote_device_IP(selected_product_Node.BuildingInfo.strIp);
                        ((CDialogCM5_BacNet*)m_pViews[m_nCurView])->SetConnected_IP(selected_product_Node.BuildingInfo.strIp);
                    }
                    else
                    {
                        m_is_remote_device = false;
                        ((CDialogCM5_BacNet*)m_pViews[m_nCurView])->SetConnected_IP(selected_product_Node.BuildingInfo.strIp);
                    }
                }



                CString ProductHexBinName, StrTemp;
                CString StrBinHexPath, StrULRPath;
                GetProductFPTAndLocalPath(selected_product_Node.product_class_id, StrULRPath, ProductHexBinName);
                StrBinHexPath = g_strExePth;
                StrBinHexPath += _T("firmware\\");

                StrTemp.Format(_T("%s\\"), GetProductName(selected_product_Node.product_class_id));
                StrBinHexPath += StrTemp;
                StrBinHexPath += ProductHexBinName;

                HANDLE hFind;//
                WIN32_FIND_DATA wfd;//
                hFind = FindFirstFile(StrBinHexPath, &wfd);//
                if ((hFind != INVALID_HANDLE_VALUE) && (!StrULRPath.IsEmpty()))//说明当前目录下无t3000.mdb
                {
                    filename = StrBinHexPath;
                }

                ip = selected_product_Node.BuildingInfo.strIp;
                ipport.Format(_T("%d"), selected_product_Node.ncomport);
                m_cfgFileHandler.WriteToCfgFile(filename,
                    _T("Ethernet"),
                    id,
                    comport,
                    BD,
                    ip,
                    ipport,
                    subnote,
                    subID);


                if (selected_product_Node.protocol == PROTOCOL_BIP_TO_MSTP)
                    g_protocol = PROTOCOL_BIP_TO_MSTP;
                else if ((selected_product_Node.protocol == MODBUS_TCPIP) &&
                    (selected_product_Node.note_parent_serial_number != 0) &&   //如 TSTAT10 用485协议挂在T3BB下
                    ((selected_product_Node.product_class_id == PM_TSTAT10) ||
                    (selected_product_Node.product_class_id == PM_MINIPANEL) ||
                    (selected_product_Node.product_class_id == PM_MINIPANEL_ARM)))
                {
                    g_protocol = PROTOCOL_MB_TCPIP_TO_MB_RS485;
                }
                else if(selected_product_Node.protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
                {
                    if ((selected_product_Node.product_class_id == PM_TSTAT10) ||
                        (selected_product_Node.product_class_id == PM_MINIPANEL) ||
                        (selected_product_Node.product_class_id == PM_MINIPANEL_ARM))
                    {
                        g_protocol = PROTOCOL_BIP_TO_MSTP;
                        selected_product_Node.protocol = PROTOCOL_BIP_TO_MSTP;
                    }
                }
                else
                    g_protocol = PROTOCOL_BACNET_IP;
                SwitchToPruductType(DLG_BACNET_VIEW);

                pDlg->ShowWindow(SW_HIDE);
                if (pDlg)
                    delete pDlg;//20120220
                pDlg = NULL;
                m_pTreeViewCrl->SetSelectItem(hTreeItem);//在线的时候才将颜色变红;
                m_pTreeViewCrl->SetSelectSerialNumber(selected_product_Node.serial_number);
                g_selected_serialnumber = m_product.at(i).serial_number;
                g_bPauseMultiRead = true;
                goto do_connect_success;
                return;
                //}
            }

            HideBacnetWindow();

            if ((m_product.at(i).protocol != PROTOCOL_MSTP_TO_MODBUS) && (m_product.at(i).protocol != MODBUS_BACNET_MSTP))
                close_bac_com();

            //************************************
            strTemp.Format(_T("\ng_tstat_id =%d   product_id =%d\n"), g_tstat_id, selected_product_Node.product_id);
            TRACE(strTemp);
            if (g_tstat_id != selected_product_Node.product_id)
            {
                register_critical_section.Lock();
                g_tstat_id_changed = TRUE;
                strTemp.Format(_T("Lock\n"));
                g_tstat_id = selected_product_Node.product_id;
                register_critical_section.Unlock();
                strTemp.Format(_T("UnLock\n"));
            }


            //***************************************
            strTemp.Format(_T("SelectID=%d\n"), selected_product_Node.product_id);
            nSelectID = selected_product_Node.product_id;
            nSelectSerialNumber = selected_product_Node.serial_number;
            flagsoftwareversion = selected_product_Node.software_version;


            CString strTitle;
            strTitle = selected_product_Node.BuildingInfo.strMainBuildingname;
            strTitle += _T("->");
            strTitle += selected_product_Node.BuildingInfo.strBuildingName;

            m_wndWorkSpace.SetWindowText(strTitle);

            g_strImagePathName = selected_product_Node.strImgPathName;
            if (!(
                (selected_product_Node.BuildingInfo.strIp.CompareNoCase(_T("9600")) == 0) ||
                (selected_product_Node.BuildingInfo.strIp.CompareNoCase(_T("19200")) == 0) ||
                (selected_product_Node.BuildingInfo.strIp.CompareNoCase(_T("38400")) == 0) ||
                (selected_product_Node.BuildingInfo.strIp.CompareNoCase(_T("57600")) == 0) ||
                (selected_product_Node.BuildingInfo.strIp.CompareNoCase(_T("76800")) == 0) ||
                (selected_product_Node.BuildingInfo.strIp.CompareNoCase(_T("115200"))) == 0))
            {

                if (selected_product_Node.BuildingInfo.hCommunication == NULL || m_strCurSubBuldingName.CompareNoCase(selected_product_Node.BuildingInfo.strBuildingName) != 0)
                {
                    strTemp.Format(_T("Connect\n"));
                    TRACE(strTemp);
                    if (MODE_SUPPORT_PTRANSFER)
                    {
                        g_CommunicationType = 1;
                    }
                    else if ((selected_product_Node.protocol == MODBUS_RS485)||
                        (selected_product_Node.protocol == MODBUS_TCPIP))
                        ConnectDevice(selected_product_Node);
                    else
                        g_CommunicationType = 0;

                    //if ((selected_product_Node.protocol != PROTOCOL_MSTP_TO_MODBUS) && (selected_product_Node.protocol != MODBUS_BACNET_MSTP))
                    //    ConnectDevice(selected_product_Node);
                    //else
                    //    g_CommunicationType = 0;
                    // SetLastCommunicationType(1); //不可理解为什么要这么做，屏蔽 by 杜帆 06 03 31;
                    ip = selected_product_Node.BuildingInfo.strIp;
                    ipport.Format(_T("%d"), selected_product_Node.ncomport);

                    //ISPTool Config
#if 1



                    m_cfgFileHandler.WriteToCfgFile(filename,
                        _T("Ethernet"),
                        id,
                        comport,
                        BD,
                        ip,
                        ipport,
                        subnote,
                        subID);

#endif
                }
            }
            //串口设置
            else
            {
                g_protocol = MODBUS_RS485;

                {
                    //close_com();//关闭所有端口
                    //int nComPort = _wtoi(product_Node.BuildingInfo.strComPort.Mid(3));

                    int nComPort = selected_product_Node.ncomport;
                    if (nComPort == 0)
                    {
                        //nComPort = _wtoi(product_Node.BuildingInfo.strComPort.Mid(3));
                        // AfxMessageBox(_T("Comport is 0,Error"));
                        if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                        {
                            m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Comport is 0,Error"));
                            m_pDialogInfo->ShowWindow(SW_SHOW);
                        }
                        //                          while(m_pDialogInfo->IsWindowVisible()){
                        //                              Sleep(50);
                        //                              continue;
                        //                          }
                        if (pDlg != NULL)
                        {
                            pDlg->ShowWindow(SW_HIDE);
                            delete pDlg;
                            pDlg = NULL;
                        }
                        goto do_conncet_failed;
                        return;
                    }
                    //SetPaneCommunicationPrompt(strInfo);
                    SetCommunicationType(0);
                    if (is_connect())
                    {
                        close_com();
                    }
                    Sleep(200);
                    BOOL  bret = open_com(nComPort);
                    if (!bret)	 //Try one more time
                    {
                        bret = open_com(nComPort);
                    }
                    if (!bret)
                    {
                        strTemp.Format(_T("Open COM%d  Failed,Try again!"), nComPort);
                        //AfxMessageBox(strTemp);
                        if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                        {
                            m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(strTemp);
                            m_pDialogInfo->ShowWindow(SW_SHOW);
                        }
                        //                         while(m_pDialogInfo->IsWindowVisible()){
                        //                             Sleep(50);
                        //                             continue;
                        //                         }
                        if (pDlg != NULL)
                        {
                            pDlg->ShowWindow(SW_HIDE);
                            delete pDlg;
                            pDlg = NULL;
                        }
                        m_nStyle = 2;
                        return;
                    }
                    else
                    {
                        CString strInfo;
                        strInfo.Format(_T("COM %d Connected: Yes"), nComPort);
                        SetPaneString(1, strInfo);
                        SetCommunicationType(0);
                        g_CommunicationType = 0;

                        m_nStyle = 1;

                        CString	g_configfile_path = g_strExePth + _T("T3000_config.ini");
                        CFileFind fFind;
                        if (!fFind.FindFile(g_configfile_path))
                        {
                            CString strInfo;
                            strInfo.Format(_T("%d"), g_CommunicationType);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Connection Type"), strInfo, g_configfile_path);
                            strInfo.Format(_T("COM%d"), nComPort);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("COM Port"), strInfo, g_configfile_path);
                            strInfo.Format(_T("%d"), nComPort);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("COM_Port"), strInfo, g_configfile_path);
                            strInfo.Format(_T("%d"), selected_product_Node.baudrate);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Baudrate"), strInfo, g_configfile_path);
                        }
                        else
                        {
                            CString strInfo;
                            strInfo.Format(_T("%d"), g_CommunicationType);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Connection Type"), strInfo, g_configfile_path);
                            strInfo.Format(_T("COM%d"), nComPort);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("COM Port"), strInfo, g_configfile_path);
                            strInfo.Format(_T("%d"), nComPort);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("COM_Port"), strInfo, g_configfile_path);
                            strInfo.Format(_T("%d"), selected_product_Node.baudrate);
                            WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Baudrate"), strInfo, g_configfile_path);
                        }

#if 1

                        id.Format(_T("%d"), g_tstat_id);
                        comport.Format(_T("COM%d"), nComPort);
                        BD.Format(_T("%d"), selected_product_Node.baudrate);
                        ip = selected_product_Node.BuildingInfo.strIp;
                        ipport = selected_product_Node.BuildingInfo.strIpPort;
                        m_cfgFileHandler.WriteToCfgFile(filename,
                            _T("COM"),
                            id,
                            comport,
                            BD,
                            ip,
                            ipport,
                            subnote,
                            subID);
#endif
                    }
                }
            }
            m_strCurSubBuldingName = selected_product_Node.BuildingInfo.strBuildingName;


            if (selected_product_Node.protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
            {
                if (initial_bip == false)
                {

                    bool intial_ret = false;
                    intial_ret = Initial_bac(0, selected_product_Node.NetworkCard_Address);
                    if (intial_ret == false)
                    {
                        MessageBox(_T("Failed to initialize Bacnet MSTP serial port information. Please try again"));
                        return;
                    }
                    
                    initial_bip = true;
                    Sleep(50);
                    Send_WhoIs_Global(-1, -1);
                    Sleep(50);
                }
                g_mstp_deviceid = selected_product_Node.object_instance;
                Device_Type = selected_product_Node.product_class_id;
                g_protocol = PROTOCOL_BIP_T0_MSTP_TO_MODBUS;
                goto start_read_reg_data;
            }



            if (g_CommunicationType == 0)
            {
                //m_nbaudrat=_wtoi(selected_product_Node.BuildingInfo.strBaudRate);
                g_protocol = MODBUS_RS485;
                m_nbaudrat = selected_product_Node.baudrate;
                if ((m_nbaudrat != 9600) && (m_nbaudrat != 19200) && (m_nbaudrat != 38400) && (m_nbaudrat != 57600) && (m_nbaudrat != 76800) && (m_nbaudrat != 115200))
                    m_nbaudrat = 19200;

                if (selected_product_Node.protocol == PROTOCOL_MSTP_TO_MODBUS)
                {
                    g_protocol = PROTOCOL_MSTP_TO_MODBUS;
                    g_mstp_deviceid = selected_product_Node.object_instance;
                    bac_gloab_panel = selected_product_Node.panel_number;
                    if ((system_connect_info.mstp_status == 1) &&
                        (system_connect_info.ncomport == selected_product_Node.ncomport) &&
                        (system_connect_info.nbaudrate == selected_product_Node.baudrate)&&
                        (bac_gloab_panel == g_thread_max_mac_id))
                    {
                        Sleep(100);
                    }
                    else
                    {
                        int init_ret = 0;
                        //m_bac_handle_Iam_data.clear();
                        Set_MSTP_Polling_Node(selected_product_Node.panel_number);
                        init_ret = Initial_bac(selected_product_Node.ncomport, _T(""), selected_product_Node.baudrate);
                        if (init_ret == 0)
                        {
                            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Initial Bacnet MSTP com port failed!"));
                            return;
                        }
                    }
                    mstp_read_result = false;
                    CShowMessageDlg TempDlg;
                    TempDlg.SetStaticText(_T("Establishing Bacnet MSTP connection , please wait!"));
                    //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
                    TempDlg.SetStaticTextColor(RGB(0, 0, 255));
                    TempDlg.SetStaticTextSize(25, 20);
                    TempDlg.SetProgressAutoClose(500, 120, EVENT_MSTP_CONNECTION_ESTABLISH);

                    _Bac_Scan_Com_Info tempinfo;
                    tempinfo.device_id = g_mstp_deviceid;
                    tempinfo.macaddress = selected_product_Node.panel_number;
                    TempDlg.SetMstpDeviceInfo(tempinfo);

                    TempDlg.DoModal();
                    if (!mstp_read_result)
                    {
                        bOnLine = FALSE;
                        m_pTreeViewCrl->turn_item_image(hSelItem, false);
                        m_product.at(i).status_last_time[0] = false;
                        m_product.at(i).status_last_time[1] = false;
                        m_product.at(i).status_last_time[2] = false;
                        m_product.at(i).status_last_time[3] = false;
                        m_product.at(i).status_last_time[4] = false;
                        goto do_conncet_failed;
                        return;
                    }
                }
                else
                {
                    Change_BaudRate(m_nbaudrat);
                }

                register_critical_section.Lock();
                int nmultyRet = Read_Multi(g_tstat_id, &read_data[0], 0, 10, 6);
                register_critical_section.Unlock();
                if (nmultyRet <0)
                {
                    bac_select_device_online = false;
                    if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                    {
                        m_product.at(i).status = false;
                        m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Device is Offline!"));
                        m_pDialogInfo->ShowWindow(SW_SHOW);
                    }

                    if (pDlg != NULL)
                    {
                        pDlg->ShowWindow(SW_HIDE);
                        delete pDlg;
                        pDlg = NULL;
                    }
                    bOnLine = FALSE;
                    m_pTreeViewCrl->turn_item_image(hSelItem, false);
                    m_product.at(i).status_last_time[0] = false;
                    m_product.at(i).status_last_time[1] = false;
                    m_product.at(i).status_last_time[2] = false;
                    m_product.at(i).status_last_time[3] = false;
                    m_product.at(i).status_last_time[4] = false;
                    goto do_conncet_failed;
                    return;
                }
                else
                {
                    SetLastSuccessBaudrate(m_nbaudrat);
                    bac_select_device_online = true;
                    m_product.at(i).status = true;
                    m_pTreeViewCrl->SetSelectItem(hTreeItem);//在线的时候才将颜色变红;
                    m_pTreeViewCrl->SetSelectSerialNumber(selected_product_Node.serial_number);
                    g_selected_serialnumber = m_product.at(i).serial_number;


                    nSerialNumber = read_data[0] + read_data[1] * 256 + read_data[2] * 256 * 256 + read_data[3] * 256 * 256 * 256;
                    Device_Type = read_data[7];
                    if (nSerialNumber >= 0)
                    {
                        //	if(nSerialNumber==nSelectSerialNumber)
                        //{
                        SetLastOpenedComport(selected_product_Node.ncomport);
                        SetLastCommunicationType(0);
                        bOnLine = TRUE;

                        CString temp_value = m_subNetLst.at(0).strComPort;
                        if (temp_value.GetLength() > 3)
                        {
                            CString cs_temp_1;
                            cs_temp_1 = temp_value.Right(temp_value.GetLength() - 3);
                            int i_temp_port = _wtoi(cs_temp_1);
                            if (i_temp_port != selected_product_Node.ncomport)
                            {
                                CString temp_cs_port;
                                temp_cs_port.Format(_T("COM%u"), selected_product_Node.ncomport);
                                try
                                {
                                    CString strSql;
                                    strSql.Format(_T("update Building set Com_Port='%s' where Building_Name='%s' and Main_BuildingName='%s'"), temp_cs_port, m_strCurSubBuldingName, m_strCurMainBuildingName);
                                    SqliteDBT3000.execDML((UTF8MBSTR)strSql);

                                    m_subNetLst.at(0).strComPort = temp_cs_port;
                                }
                                catch (...)
                                {

                                }
                            }
                        }

                    }
                }
            }
            if (g_CommunicationType == 1)
            {
                SetLastCommunicationType(1);
                g_protocol = MODBUS_TCPIP;


                if (MODE_SUPPORT_PTRANSFER)
                {
                    intial_bip_socket();
                    //if (Re_Initial_Bac_Socket_IP.CompareNoCase(selected_product_Node.NetworkCard_Address) != 0)
                    //{
                    //    Initial_bac(0, selected_product_Node.NetworkCard_Address);
                    //}
                    //if (selected_product_Node.NetworkCard_Address.CompareNoCase(Re_Initial_Bac_Socket_IP) != 0)
                    //{
                    //    int temp_add_port = rand() % 10000;
                    //    closesocket(my_sokect);
                    //    int ret_1 = Open_bacnetSocket2(selected_product_Node.NetworkCard_Address, BACNETIP_PORT + temp_add_port, my_sokect);
                    //    if (ret_1 >= 0)
                    //    {
                    //        Re_Initial_Bac_Socket_IP = selected_product_Node.NetworkCard_Address;
                    //    }
                    //}


                    Send_WhoIs_Global(-1, -1);
                    Sleep(SEND_COMMAND_DELAY_TIME);
                    g_protocol = PROTOCOL_BIP_T0_MSTP_TO_MODBUS;
                }

                int nmultyRet = 0;
                // SetResponseTime (2000);   For Kaiyin's TstatHUM
                if(SPECIAL_BAC_TO_MODBUS)
                    nmultyRet = Read_Multi(g_tstat_id, &read_data[0], 0, 10, 10);
                else
                    nmultyRet = Read_Multi(g_tstat_id, &read_data[0], 0, 10, 6);

                //
                if (nmultyRet <0)
                {
                    //AfxMessageBox(_T("Device is offline,Please check the connection!"));//\nDatabase->Building config Database
                    m_product.at(i).status = false;
                    bac_select_device_online = false;
                    if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                    {
                        m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Device is offline,Please check the connection!"));
                        m_pDialogInfo->ShowWindow(SW_SHOW);
                    }

                    if (pDlg != NULL)
                    {
                        pDlg->ShowWindow(SW_HIDE);
                        delete pDlg;
                        pDlg = NULL;
                    }
                    bOnLine = FALSE;
                }
                else
                {
                    if ((selected_product_Node.nhardware_info & 0x02) == 2)
                        Sleep(SEND_COMMAND_DELAY_TIME);
                    UINT nSerialNumber = 0;
                    m_product.at(i).status = true;
                    bac_select_device_online = true;
                    nSerialNumber = read_data[0] + read_data[1] * 256 + read_data[2] * 256 * 256 + read_data[3] * 256 * 256 * 256;
                    Device_Type = read_data[7];

                    CString Temp_product_name = GetProductName(Device_Type);
                    if (!IS_Temco_Product(Device_Type))
                    {
                        CString tempcs;
                        tempcs.Format(_T("Product ID is invalid .Product id is %d \r\n"), Device_Type);
                        if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                        {
                            m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(tempcs);
                            m_pDialogInfo->ShowWindow(SW_SHOW);
                        }

                        if (pDlg != NULL)
                        {
                            pDlg->ShowWindow(SW_HIDE);
                            delete pDlg;
                            pDlg = NULL;
                        }
                        //MessageBox(tempcs);
                        goto do_conncet_failed;
                        return;

                    }
                    if (nSerialNumber >= 0)
                    {
                        bOnLine = TRUE;
                    }
                }
            }

            if (bOnLine)
            {
                //SetPaneConnectionPrompt(_T("Online!"));
                pDlg->ShowWindow(SW_HIDE);
                m_pTreeViewCrl->SetSelectItem(hTreeItem);//在线的时候才将颜色变红;
                m_pTreeViewCrl->SetSelectSerialNumber(selected_product_Node.serial_number);
                g_selected_serialnumber = m_product.at(i).serial_number;
                m_pTreeViewCrl->turn_item_image(hSelItem, true);
                m_nStyle = 1;
            }
            else
            {

                //SetPaneConnectionPrompt(_T("Offline!"));
                m_pTreeViewCrl->turn_item_image(hSelItem, false);
                memset(&multi_register_value[0], 0, sizeof(multi_register_value));
                m_nStyle = 2;
                m_product.at(i).status_last_time[0] = false;
                m_product.at(i).status_last_time[1] = false;
                m_product.at(i).status_last_time[2] = false;
                m_product.at(i).status_last_time[3] = false;
                m_product.at(i).status_last_time[4] = false;
                //20120424
                if (pDlg != NULL)
                {
                    pDlg->ShowWindow(SW_HIDE);
                    delete pDlg;
                    pDlg = NULL;
                }
                return;
            }

        start_read_reg_data:

            CString achive_file_path;
            CString temp_serial;
            temp_serial.Format(_T("%u.prog"), g_selected_serialnumber);
            if (Device_Type == 0)
            {
                Device_Type = Scan_Product_ID;
            }
            achive_file_path = g_achive_folder + _T("\\") + temp_serial;
            if (/*Load_Product_Value_Cache(achive_file_path) < 0*/1)
            {
                if (Device_Type == 100)
                {

                    register_critical_section.Lock();
                    int i;
                    int it = 0;
                    float progress;
                    for (i = 0; i<12; i++)
                    {
                        //register_critical_section.Lock();
                        //int nStart = GetTickCount();
                        int itemp = 0;
                        itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 4);
                        if (itemp < 0)
                        {
                            //continue;
                            break; //读不到就退出，很多时候 NC在读的过程中断开连接T3000 还一直去读剩余的 就会引起无响应;
                        }
                        else
                        {
                            if (pDlg != NULL)
                            {
                                progress = float((it + 1)*(100 / 12));
                                g_progress_persent = progress;
                            }
                        }
                        it++;
                        Sleep(100);
                    }


                    if (it<12)
                    {
                        if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                        {
                            m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Reading abnormal \n Try again!"));
                            m_pDialogInfo->ShowWindow(SW_SHOW);
                        }

                        pDlg->ShowWindow(SW_HIDE);
                        if (pDlg != NULL)
                            delete pDlg;
                        pDlg = NULL;

                    }
                    else
                    {
                        pDlg->ShowProgress(100, 100);
                        pDlg->ShowWindow(SW_HIDE);
                        if (pDlg != NULL)
                            delete pDlg;
                        pDlg = NULL;
                    }
                    g_tstat_id_changed = FALSE;
                    register_critical_section.Unlock();
                }
                else if (Device_Type > 0)
                {
                    int length = 10;
                    nFlag = Device_Type;
                    if (Device_Type == PM_PM5E_ARM)
                    {
                        nFlag = PM_TSTAT8;
                        Device_Type = PM_TSTAT8;
                    }
                    int it = 0;
                    if ((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7) || (nFlag == PM_TSTAT9) || (nFlag == PM_TSTAT5i) || (nFlag == PM_TSTAT8) || (nFlag == PM_TSTAT9)
                        || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
                    {
                        product_type = T3000_6_ADDRESS;
                    }
                    else if ((nFlag == PM_TSTATRUNAR) || (nFlag == PM_TSTAT5E || nFlag == PM_PM5E || nFlag == PM_PM5E_ARM) || (nFlag == PM_TSTAT5H) || (nFlag == PM_TSTAT5G))
                    {
                        product_type = T3000_5EH_LCD_ADDRESS;
                    }
                    else if ((nFlag == PM_TSTAT5A) || (nFlag == PM_TSTAT5B) || (nFlag == PM_TSTAT5B2) ||
                        (nFlag == PM_TSTAT5C) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F))
                    {
                        product_type = T3000_5ABCDFG_LED_ADDRESS;
                    }
                    else if (nFlag == PM_CS_SM_AC || nFlag == PM_CS_SM_DC || nFlag == PM_CS_RSM_AC || nFlag == PM_CS_RSM_DC)
                    {
                        product_type = CS3000;
                    }
                    else if (nFlag == PM_T3PT10 || nFlag == PM_T3IOA || nFlag == PM_T332AI || nFlag == PM_T38AI16O || nFlag == PM_T38I13O ||
                        nFlag == PM_T34AO || nFlag == PM_T36CT || nFlag == PM_T3PERFORMANCE ||
                        nFlag == PM_T322AI ||
                        nFlag == PM_T3PT12 ||
                        nFlag == PM_T36CTA ||
                        nFlag == PM_T3_LC ||
                        nFlag == PM_T38AI8AO6DO)
                    {
                        product_type = T3000_T3_MODULES;
                    }
                    else if (nFlag == PM_PWMETER)
                    {
                        product_type = T3000_T3_MODULES;

                    }
                    else
                    {
                        product_type = nFlag;
                    }




                    memset(&multi_register_value[0], 0, sizeof(multi_register_value));

                    if (product_type == T3000_6_ADDRESS)
                    {
                        register_critical_section.Lock();
                        int i;
                        it = 0;
                        float progress;
                        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Reading data!"));

                        for (i = 0; i<(length); i++)	//暂定为0 ，因为TSTAT6 目前为600多
                        {
                            int itemp = 0;
                            itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 5);
                            if (itemp < 0)
                            {
                                break;
                            }
                            else
                            {
                                if (pDlg != NULL)
                                {
                                    progress = float((it + 1)*(100 / 10));
                                    //pDlg->ShowProgress(int(progress),int(progress));
                                    g_progress_persent = progress;
                                }
                            }
                            it++;

                            Sleep(100);
                        }
                        if (it == length)
                        {
                            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data success!"));
                        }
                        g_tstat_id_changed = FALSE;
                        register_critical_section.Unlock();

                    }
                    else if (product_type == T3000_T3_MODULES)
                    {
                        length = 4;
                        //power_value = 1;
                        register_critical_section.Lock();
                        int i;
                        it = 0;
                        float progress;
                        for (i = 0; i<length; i++)	//暂定为0 ，因为TSTAT6 目前为600多
                        {
                            int itemp = 0;
                            itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 5);
                            if (itemp < 0)
                            {
                                break;
                            }
                            else
                            {
                                if (pDlg != NULL)
                                {
                                    progress = float((it + 1)*(100 / length));
                                    //pDlg->ShowProgress(int(progress),int(progress));
                                    g_progress_persent = progress;
                                }
                            }
                            it++;
                            Sleep(SEND_COMMAND_DELAY_TIME);
                        }
                        g_tstat_id_changed = FALSE;
                        register_critical_section.Unlock();
                    }
                    else if (product_type == PM_PWMETER)
                    {
                        register_critical_section.Lock();
                        int i;
                        it = 0;
                        length = 4;
                        float progress;
                        for (i = 0; i<4; i++)
                        {
                            int itemp = 0;
                            itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 5);
                            if (itemp < 0)
                            {
                                break;
                            }
                            else
                            {
                                if (pDlg != NULL)
                                {
                                    progress = float((it + 1)*(100 / 4));
                                    //pDlg->ShowProgress(int(progress),int(progress));
                                    g_progress_persent = progress;
                                }
                            }
                            it++;
                            Sleep(SEND_COMMAND_DELAY_TIME);
                        }
                        g_tstat_id_changed = FALSE;
                        register_critical_section.Unlock();
                    }
                    else if ((product_type == PM_TSTAT_AQ) || (product_type == PM_MULTI_SENSOR))
                    {

                        length = 11;
                        register_critical_section.Lock();
                        int i;
                        it = 0;
                        float progress;
                        for (i = 0; i<length; i++)
                        {
                            int itemp = 0;
                            if ((i != 0) && (i != 1) && (i != 6) && (i != 7) && (i != 9) && (i != 10))
                            {
                                itemp = 1;
                            }
                            else
                                itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 5);
                            if (itemp < 0)
                            {
                                break;
                            }
                            else
                            {
                                if (pDlg != NULL)
                                {
                                    progress = float((it + 1)*(100 / 11));
                                    g_progress_persent = progress;
                                }
                            }
                            it++;
                            if ((i == 0) || (i == 1) || (i == 6) || (i == 7) || (i == 9) || (i == 10)) //Airlab不需要读这些寄存器
                                Sleep(SEND_COMMAND_DELAY_TIME);
                        }
                        g_tstat_id_changed = FALSE;
                        int read_ret = 0;
                        read_ret = Read_Multi(g_tstat_id, product_register_sensor_flag, 65000, 3, 5);

                        

                        register_critical_section.Unlock();
                    }
                    else
                    {
                        //power_value = 1;
                        length = 9;
                        register_critical_section.Lock();
                        int i;
                        it = 0;
                        float progress;
                        for (i = 0; i<length; i++)
                        {
                            int itemp = 0;
                            itemp = Read_Multi(g_tstat_id, &multi_register_value[i * 100], i * 100, 100, 5);
                            if (itemp < 0)
                            {
                                break;
                            }
                            else
                            {
                                if (pDlg != NULL)
                                {
                                    progress = float((it + 1)*(100 / 9));
                                    //pDlg->ShowProgress(int(progress),int(progress));
                                    g_progress_persent = progress;
                                }
                            }
                            it++;
                            Sleep(200);
                        }
                        g_tstat_id_changed = FALSE;
                        register_critical_section.Unlock();
                    }


                    memcpy_s(product_register_value, sizeof(product_register_value), multi_register_value, sizeof(multi_register_value));


                    if (product_register_value[714] == 0x56)
                    {
                        CString TstatName, TstatDBName;


                        CString strSerial;
                        strSerial.Format(_T("%d"), get_serialnumber());

#if 1 // 点击的时候把新的Tstat6的名字写到Tstat6的寄存器中

                        CString newname;
                        CString strSql;



                        temp_serial_number.Format(_T("%u"), g_selected_serialnumber);

                        int temp_write_flag = GetPrivateProfileInt(temp_serial_number, _T("WriteFlag"), 0, g_achive_device_name_path);
                        if (temp_write_flag == 1)
                        {
                            GetPrivateProfileStringW(temp_serial_number, _T("NewName"), _T(""), newname.GetBuffer(MAX_PATH), MAX_PATH, g_achive_device_name_path);
                            if (newname.GetLength()> 16)	//长度不能大于结构体定义的长度;
                            {
                                newname.Delete(16, newname.GetLength() - 16);
                            }
                            char cTemp1[16];
                            memset(cTemp1, 0, 16);
                            WideCharToMultiByte(CP_ACP, 0, newname.GetBuffer(), -1, cTemp1, 16, NULL, NULL);
                            unsigned char Databuffer[16];
                            memcpy_s(Databuffer, 16, cTemp1, 16);
                            if (Write_Multi(g_tstat_id, Databuffer, 715, 16, 3)>0)
                            {
                                for (int i = 0; i<16; i++)
                                {
                                    product_register_value[715 + i] = Databuffer[i];
                                }




                                strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"), newname, strSerial);
                                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

                                WritePrivateProfileStringW(temp_serial_number, _T("WriteFlag"), L"0", g_achive_device_name_path);
                                PostMessage(WM_MYMSG_REFRESHBUILDING, 0, 0);
                            }
                        }


                        strSql.Format(_T("select * from  ALL_NODE  where Serial_ID='%s'"), strSerial);
                        q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
                        if (!q.eof())
                        {
                            TstatDBName = q.getValuebyName(L"Product_name");
                        }

                        CString StrDefaultName;
                        if (TstatDBName.IsEmpty())
                        {
                            StrDefaultName.Format(_T("%s-%s"), GetProductName(product_register_value[7]), temp_serial_number);
                        }
                        TstatName.Format(_T("%s%s"), GetTextFromReg(715), GetTextFromReg(719));
                        if (TstatName.IsEmpty())
                        {
                            StrDefaultName.Format(_T("%s-%s"), GetProductName(product_register_value[7]), temp_serial_number);
                        }
                        if (TstatName.CompareNoCase(TstatDBName) != 0 && (!TstatName.IsEmpty()))
                        {
                            strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"), TstatName, strSerial);
                            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                            PostMessage(WM_MYMSG_REFRESHBUILDING, 0, 0);
                        }
                        else if (TstatName.IsEmpty() && TstatDBName.IsEmpty())
                        {
                            strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"), StrDefaultName, strSerial);
                            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                            PostMessage(WM_MYMSG_REFRESHBUILDING, 0, 0);
                        }

#endif
                    }

                    //Code and comments by Fance
                    //Use old_product_type to save the last value.
                    //if this value is change ,it will reload the register value.
                    //example:
                    //For tstats 5A : product_register_value[MODBUS_BAUDRATE]  ->  MODBUS_BAUDRATE = 185
                    //For tstats 6	  product_register_value[MODBUS_BAUDRATE]  ->  MODBUS_BAUDRATE = 110
                    if (old_product_type != product_type)
                    {
                        old_product_type = product_type;
                        MyRegAddress.Change_Register_Table();
                    }
                    int nFlag = product_register_value[7];
                    if (product_register_value[7] == PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
                        || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
                    {
                        MODBUS_BAUDRATE = 15;
                    }


                    //    GetIONanme();
                    if (product_type == T3000_6_ADDRESS)
                    {

                        LoadTstat_InputData();
                        LoadTstat_OutputData();
                    }

                    else if (product_type == CS3000)
                    {
                        LoadInputData_CS3000();
                        LoadOutputData_CS3000();
                    }

                    else if (product_type == T3000_T3_MODULES)
                    {


                    }



                    if (it<length)
                    {
                        //AfxMessageBox(_T("Reading abnormal \n Try again!"));
                        if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                        {
                            m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Reading abnormal \n Try again!"));
                            m_pDialogInfo->ShowWindow(SW_SHOW);
                        }
                        pDlg->ShowWindow(SW_HIDE);
                        if (pDlg != NULL)
                            delete pDlg;
                        pDlg = NULL;


                    }
                    else
                    {

                        pDlg->ShowProgress(100, 100);
                        pDlg->ShowWindow(SW_HIDE);
                        if (pDlg != NULL)
                            delete pDlg;
                        pDlg = NULL;
                    }

                }
                Save_Product_Value_Cache(achive_file_path);
            }
            else
            {
                pDlg->ShowProgress(100, 100);
                pDlg->ShowWindow(SW_HIDE);
                if (pDlg != NULL)
                    delete pDlg;
                pDlg = NULL;
            }

            g_serialNum = nSerialNumber;//get_serialnumber();
#if 1
            if (g_bPrivilegeMannage)
            {
                if (g_buser_log_in)
                {
                    if (g_strLoginUserName.CompareNoCase(_T("admin")) == 0)
                    {
                        g_MainScreenLevel = 0;
                        g_ParamLevel = 0;
                        g_OutPutLevel = 0;
                        g_NetWorkLevel = 0;
                        g_GraphicModelevel = 0;
                        g_BurnHexLevel = 0;
                        g_LoadConfigLevel = 0;
                        g_BuildingsetLevel = 0;
                        g_AllscreensetLevel = 0;
                    }
                    else
                    {
                        try
                        {
                            CString strSerial;
                            strSerial.Format(_T("%d"), g_serialNum);
                            strSerial.Trim();
                            CString strsql;
                            strsql.Format(_T("select * from user_level where MainBuilding_Name='%s' and Building_Name='%s' and username = '%s' and serial_number = %d"), m_strCurMainBuildingName, m_strCurSubBuldingName, g_strLoginUserName, g_serialNum);
                            q = SqliteDBT3000.execQuery((UTF8MBSTR)strsql);

                            if (!q.eof())
                            {
                                //获取权限：
                                g_AllscreensetLevel = q.getIntField("allscreen_level");
                                if (g_AllscreensetLevel != 1)
                                {
                                    g_MainScreenLevel = q.getIntField("mainscreen_level");//
                                    g_ParamLevel = q.getIntField("parameter_level");
                                    g_OutPutLevel = q.getIntField("outputtable_level");
                                    g_NetWorkLevel = q.getIntField("networkcontrol_level");//
                                    g_GraphicModelevel = q.getIntField("graphic_level");//
                                    g_BurnHexLevel = q.getIntField("burnhex_level");
                                    g_LoadConfigLevel = q.getIntField("loadconfig_level");
                                    g_BuildingsetLevel = q.getIntField("building_level");
                                }
                                else
                                {
                                    g_MainScreenLevel = 1;
                                    g_ParamLevel = 1;
                                    g_OutPutLevel = 1;
                                    g_NetWorkLevel = 1;
                                    g_GraphicModelevel = 1;
                                    g_BurnHexLevel = 1;
                                    g_LoadConfigLevel = 1;
                                    g_BuildingsetLevel = 1;
                                    g_AllscreensetLevel = 1;

                                }
                            }

                            strSerial.Format(_T("%d"), g_serialNum);
                            strSerial.Trim();

                            strsql.Format(_T("select * from UserLevelSingleSet where MainBuilding_Name='%s' and Building_Name='%s' and username = '%s'"), m_strCurMainBuildingName, m_strCurSubBuldingName, g_strLoginUserName);
                            q = SqliteDBT3000.execQuery((UTF8MBSTR)strsql);
                            if (!q.eof())
                            {
                                //获取权限：
                                g_NetWorkLevel = q.getIntField("networkcontroller");//
                                g_BuildingsetLevel = q.getIntField("database_limition");
                            }
                        }
                        catch (...)
                        {
                        }
                    }

                }

            }




#endif

            CString temp_pid;
            temp_pid.Format(_T("%d"), product_register_value[7]);
            WritePrivateProfileStringW(_T("LastView"), _T("ViewSerialNumber"), temp_serial_number, g_cstring_ini_path);
            WritePrivateProfileStringW(_T("LastView"), _T("ViewPid"), temp_pid, g_cstring_ini_path);

            nFlag = Device_Type;
            if (nFlag >65530)
            {
                if (m_pDialogInfo != NULL && !m_pDialogInfo->IsWindowVisible())
                {
                    m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Reading abnormal \n Try again!"));
                    m_pDialogInfo->ShowWindow(SW_SHOW);
                }

                if (pDlg != NULL)
                {
                    pDlg->ShowWindow(SW_HIDE);
                    delete pDlg;
                    pDlg = NULL;
                }
                goto do_conncet_failed;
                return;
            }
            //------------------------------------------------------------------
            g_HumChamberThread = FALSE;

            m_product.at(i).status = true;
            m_product.at(i).status_last_time[0] = true;
            m_product.at(i).status_last_time[1] = true;
            m_product.at(i).status_last_time[2] = true;
            m_product.at(i).status_last_time[3] = true;
            m_product.at(i).status_last_time[4] = true;

            if (nFlag == PM_NC)
            {
                SwitchToPruductType(DLG_NETWORKCONTROL_VIEW);
            }
            else if (nFlag == PM_CM5)//CM5
            {
                SwitchToPruductType(DLG_DIALOGCM5_VIEW);
            }
            else if (nFlag == PM_T3IOA)
            {

                SwitchToPruductType(DLG_DIALOGT38AI8AO);
            }
            else if (nFlag == PM_T38I13O)
            {

                SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);
            }
            else if (nFlag == PM_T34AO) //T3
            {
                SwitchToPruductType(DLG_DIALOGT3_VIEW);
            }
            else if (nFlag == PM_T3PT10)
            {
                SwitchToPruductType(DLG_DIALOGT3PT10);
            }
            else if (nFlag == PM_T332AI)
            {
                SwitchToPruductType(DLG_DIALOGT332AI_VIEW);
            }
            else if (m_product.at(i).product_class_id == PM_T36CT)
            {
                SwitchToPruductType(DLG_DIALOGT36CT);
            }
            else if (nFlag == PM_T38I13O)
            {
                SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);
            }
            else if (nFlag == PM_T332AI)
            {
                SwitchToPruductType(DLG_DIALOGT332AI_VIEW);
            }
            else if (nFlag == PM_T3IOA)
            {
                SwitchToPruductType(DLG_DIALOGT38AI8AO);
            }
            else if (nFlag == PM_BTU_METER)
            {
                SwitchToPruductType(DLG_DIALOG_BTUMETER);
            }
            else if (nFlag == PM_MINIPANEL || nFlag == PM_MINIPANEL_ARM)
            {
                SwitchToPruductType(DLG_DIALOGMINIPANEL_VIEW);
            }
            else if (nFlag == PM_TSTAT6_HUM_Chamber)
            {
                g_HumChamberThread = TRUE;
                SwitchToPruductType(DLG_HUMCHAMBER);
            }
            else if (nFlag == PM_CO2_RS485 || nFlag == PM_PRESSURE_SENSOR || nFlag == PM_CO2_NODE)//(nFlag == PM_CO2_NET)||
            {
                if (product_register_value[14] == 6)
                {
                    SwitchToPruductType(DLG_AIRQUALITY_VIEW);
                }
                else
                {
                    SwitchToPruductType(DLG_CO2_VIEW);
                }

            }
            else if (nFlag == STM32_CO2_NODE)
            {
                SwitchToPruductType(DLG_DIALOG_CO2_NODE);
            }
            else if (nFlag == PM_CO2_NET || nFlag == STM32_CO2_RS485 || nFlag == STM32_CO2_NET)//(nFlag == PM_CO2_NET)||
            {
                SwitchToPruductType(DLG_CO2_NET_VIEW);
            }
            else if ((product_register_value[7] == PM_CO2_RS485&&product_register_value[14] == 6) ||
                nFlag == PM_AirQuality || nFlag == PM_HUM_R ||
                product_register_value[7] == PM_HUMTEMPSENSOR ||
                product_register_value[7] == STM32_HUM_NET ||
                product_register_value[7] == STM32_PRESSURE_NET ||
                product_register_value[7] == STM32_HUM_RS485
                )
            {
                SwitchToPruductType(DLG_AIRQUALITY_VIEW);
            }
            else if (nFlag == PM_PRESSURE)
            {
                SwitchToPruductType(DLG_DIALOG_PRESSURE_SENSOR);
            }


            else if (nFlag == PM_TSTAT6 || nFlag == PM_TSTAT7 || nFlag == PM_TSTAT5i || nFlag == PM_TSTAT8 || nFlag == PM_TSTAT9
                || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
            {
                CString g_configfile_path = g_strExePth + g_strStartInterface_config;
                int m_lastinterface = GetPrivateProfileInt(_T("T3000_START"), _T("Interface"), 19, g_configfile_path);
                if (DLG_T3000_VIEW != m_lastinterface&&DLG_DIALOG_TSTAT_INPUT_VIEW != m_lastinterface&&DLG_DIALOG_TSTAT_OUTPUT_VIEW != m_lastinterface)
                {
                    m_lastinterface = DLG_T3000_VIEW;
                }
                SwitchToPruductType(m_lastinterface);
            }
            else if (nFlag == PM_CS_SM_AC || nFlag == PM_CS_SM_DC || nFlag == PM_CS_RSM_AC || nFlag == PM_CS_RSM_DC)
            {

                SwitchToPruductType(DLG_DIALOG_TSTAT_INPUT_VIEW);
            }
            else if (nFlag == PM_T322AI || nFlag == CS3000 || nFlag == PWM_TRANSDUCER || nFlag == PM_T38AI8AO6DO || nFlag == PM_T3PT12 || nFlag == PM_T36CTA || nFlag == PM_T3_LC)
            {
                //就说明是加了minipanel 10000以后寄存器的; 否则的话就跳转至以前的界面;
                new_device_support_mini_ui = true;

                bacnet_view_number = TYPE_TSTAT;
                SwitchToPruductType(DLG_DIALOG_DEFAULT_T3000_VIEW);
            }
            else if (nFlag == PM_PWMETER)
            {
                SwitchToPruductType(DLG_DIALOG_DEFAULT_T3000_VIEW);
                //SwitchToPruductType(DLG_DIALOG_POWERMETER);
            }
            else if (nFlag == PM_ZIGBEE_REPEATER)
            {
                SwitchToPruductType(DLG_DIALOG_ZIGBEE_REPEATER);
            }
            else if ((nFlag == PM_TSTAT_AQ) ||
                (nFlag == PM_MULTI_SENSOR))
            {
                SwitchToPruductType(DLG_DIALOG_TSTAT_AQ);
            }
            else if (Scan_Product_ID == 122)
            {
                SwitchToPruductType(DLG_DIALOG_BOATMONITOR);
            }
            else if ((nFlag == PWM_TEMPERATURE_TRANSDUCER) ||
                     (nFlag == STM32_PM25))
            {
                //SwitchToPruductType(DLG_AIRQUALITY_VIEW);
                SwitchToPruductType(DLG_DIALOG_DEFAULT_T3000_VIEW);
                
                //n_show_register_list = 1;
                break; //直接显示寄存器列表;
            }
            else if (nFlag<PM_NC)
            {
                if (nFlag == PM_PM5E_ARM)
                {
                    //nFlag = PM_TSTAT8;
                    //product_register_value[7] = PM_TSTAT8;
                }
                //Use product_type to change the address map
                //if product module is TSTAT6 or TSTAT7 use T3000_6_ADDRESS. T3000_6_ADDRESS is a register table in t3000.mdb  T3000_Register_Address_By_Name
                //else if product module is PM_TSTAT5E PM_TSTAT5H use T3000_5EH_LCD_ADDRESS
                //else if product module is TSTAT5 ABCDFG USE T3000_5ABCDFG_LED_ADDRESS
                if ((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7) || (nFlag == PM_TSTAT5i) || (nFlag == PM_TSTAT8) || (nFlag == PM_TSTAT9)
                    || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
                    )
                {
                    product_type = T3000_6_ADDRESS;
                }
                else if ((nFlag == PM_TSTATRUNAR) || (nFlag == PM_TSTAT5E || nFlag == PM_PM5E || nFlag == PM_PM5E_ARM) || (nFlag == PM_TSTAT5H) || (nFlag == PM_TSTAT5G))
                {
                    product_type = T3000_5EH_LCD_ADDRESS;
                }
                else if ((nFlag == PM_TSTAT5A) || (nFlag == PM_TSTAT5B) || (nFlag == PM_TSTAT5B2) ||
                    (nFlag == PM_TSTAT5C) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F))
                {
                    product_type = T3000_5ABCDFG_LED_ADDRESS;
                }
                //Code and comments by Fance
                //Use old_product_type to save the last value.
                //if this value is change ,it will reload the register value.
                //example:
                //For tstats 5A : product_register_value[MODBUS_BAUDRATE]  ->  MODBUS_BAUDRATE = 185
                //For tstats 6	  product_register_value[MODBUS_BAUDRATE]  ->  MODBUS_BAUDRATE = 110
                if (old_product_type != product_type)
                {
                    old_product_type = product_type;
                    MyRegAddress.Change_Register_Table();
                }
                int nFlag = product_register_value[7];
                if (product_register_value[7] == PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
                    || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
                {
                    MODBUS_BAUDRATE = 15;
                }
                SwitchToPruductType(DLG_T3000_VIEW);

                bacnet_view_number = TYPE_TSTAT;

            }

            break;
        }
    }
    SqliteDBT3000.closedb();

    g_bPauseMultiRead = FALSE;


do_connect_success:

    strUpdateSqlOn.Format(_T("update ALL_NODE set Online_Status = 1 where Serial_ID = %u"), g_selected_serialnumber);
    SqliteDBBuilding.execDML((UTF8MBSTR)strUpdateSqlOn);
    SqliteDBBuilding.closedb();
    //hTreeItem_retry = NULL;
    g_llRxCount = g_llRxCount + 4;

    if (n_show_register_list)  //对于没有界面的设备，直接显示寄存器列表;
    {
        OnToolRegisterviewer();
    }

    return;
do_conncet_failed:

    //Fandu 2017/12/13 设备离线时 更新 数据库设备状态字段。因为有太多地方调用  重新加载数据库的函数，导致如果不更新状态显示不正常.
    CString strUpdateSql;
    strUpdateSql.Format(_T("update ALL_NODE set Online_Status = 0 where Serial_ID = %u"), g_selected_serialnumber);
    SqliteDBBuilding.execDML((UTF8MBSTR)strUpdateSql);
    SqliteDBBuilding.closedb();

    return;
}

DWORD WINAPI   CMainFrame::retry_connect(LPVOID lpVoid)
{
		 CMainFrame *pParent = (CMainFrame *)lpVoid;
	Sleep(20000);
	::PostMessage(pParent->m_hWnd,MY_RETRY_MESSAGE ,NULL,NULL);

	return 1;
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


void CMainFrame::CheckIPDuplicate()
{
    bool find_ip_conflict = false;
    //探索重复IP;  m_refresh_net_device_data
    for (int i = 0; i < m_refresh_net_device_data.size(); i++)
    {
        if (m_refresh_net_device_data.size() <= 1)
            break;
        if (m_refresh_net_device_data.at(i).parent_serial_number != 0)
            continue;
        for (int j = i + 1; j < m_refresh_net_device_data.size(); j++)
        {
            if (m_refresh_net_device_data.at(j).parent_serial_number != 0)
                continue;

            if (m_refresh_net_device_data.at(i).ip_address.CompareNoCase(m_refresh_net_device_data.at(j).ip_address) == 0)
            {
                find_ip_conflict = true;
                /*device_ip_conflict_1*/ device_id_data_1 = m_refresh_net_device_data.at(i);
                /*device_ip_conflict_2*/ device_id_data_2 = m_refresh_net_device_data.at(j);
                edit_confilct_mode = true;
                ::PostMessageW(MainFram_hwd, WM_HADNLE_DUPLICATE_ID, CONFILIC_IPADDRESS, NULL);
                return;
            }
        }
    }
}

void CMainFrame::CheckDuplicate()
{
	sort(m_refresh_net_device_data.begin(),m_refresh_net_device_data.end(),sort_by_minipanel_pid	);

	vector <refresh_net_device> temp_device;
	temp_device.clear();
	for (int y=0;y<m_refresh_net_device_data.size();y++)
	{
		if(m_refresh_net_device_data.at(y).product_id == PM_MINIPANEL|| m_refresh_net_device_data.at(y).product_id == PM_MINIPANEL_ARM)
		{
			temp_device.push_back(m_refresh_net_device_data.at(y));
			continue;
		}
	}
	if(temp_device.size()>0)
	{
		bool find_same_panel = false;
		sort(temp_device.begin(),temp_device.end(),sort_by_minipanel_panelnumber);
		exsit_panel_number.clear();
		for (int i=0;i<temp_device.size();i++)
		{

			exsit_panel_number.push_back((int)temp_device.at(i).panal_number);
			for (int j=i+1;j<temp_device.size();j++)
			{
				if(temp_device.at(i).panal_number == temp_device.at(j).panal_number)
				{
					device_id_data_1 = temp_device.at(i);
					device_id_data_2 = temp_device.at(j);
					find_same_panel = true;
					break;
				}
			}
		}

		if(find_same_panel)
		{
			vector<int>::iterator pos;
			pos = unique(exsit_panel_number.begin(),exsit_panel_number.end());
			exsit_panel_number.erase(pos,exsit_panel_number.end());     

			edit_confilct_mode = true;

			::PostMessageW(MainFram_hwd,WM_HADNLE_DUPLICATE_ID, CONFILIC_PANEL_ID,NULL);
			return;
		}
	}


	if(!edit_confilct_mode)
	{
		int m_datasize = m_refresh_net_device_data.size(); 
		bool find_confilicat_id = false;
		for (int x=0;x<m_datasize;x++)
		{
			if(m_refresh_net_device_data.at(x).parent_serial_number != 0)
			{
				for(int y=x+1;y<m_datasize;y++)
				{
					if(m_refresh_net_device_data.at(y).parent_serial_number != 0)
					{
						if((m_refresh_net_device_data.at(x).parent_serial_number == m_refresh_net_device_data.at(y).parent_serial_number) &&
							(m_refresh_net_device_data.at(x).modbusID == m_refresh_net_device_data.at(y).modbusID))
						{
							device_id_data_1 = m_refresh_net_device_data.at(x);
							device_id_data_2 = m_refresh_net_device_data.at(y);


							find_confilicat_id = true;
							edit_confilct_mode = true;

							::PostMessageW(MainFram_hwd,WM_HADNLE_DUPLICATE_ID, CONFILIC_SUBID,NULL);
							break;
						}
					}
				}

				if(find_confilicat_id)
					break;
			}
		}


	}
}
// refresh_com 只有当这个值为0的时候才刷新串口那部分的状态,因为网络广播的 很快;可以频繁扫描，串口太慢了;
BOOL CMainFrame::CheckDeviceStatus(int refresh_com)
{
    bool find_new_device = false;
	static int rs485_scan_count = 0;
	rs485_scan_count = (++rs485_scan_count) % 5;
    for(UINT i=0; i<m_product.size(); i++)
    {
        if(b_pause_refresh_tree)
            return false;

        BOOL bOnLine=FALSE;
        UINT nSerialNumber=0;

        bool temp_online = false;
        tree_product tp = m_product.at(i);
        if(m_strCurSubBuldingName.CompareNoCase(tp.BuildingInfo.strBuildingName)==0)
        {
            int nIDNode=tp.product_id;
            nSerialNumber=tp.serial_number;

            //int newnID=read_one(nID,6,2);
            /*需要先保存之前的通信协议;
            Get the protocol ,if it is bacnet ip,we compare the device id.
            */
            //如果strip不是空的就说明这个设备室挂在NC或LC等等下面的MODBUS  RS485设备
            //if((m_product.at(i).protocol == MODBUS_RS485) && (m_product.at(i).BuildingInfo.strIp.IsEmpty()))

            int temp_port = 0;
            temp_port = m_product.at(i).ncomport;
			
            if((m_product.at(i).protocol == MODBUS_RS485) && (rs485_scan_count == 0) && (temp_port > 0 ) && (m_product.at(i).BuildingInfo.strIp.IsEmpty()))
            {
                continue;

            }
            else if(m_product.at(i).protocol == PROTOCOL_GSM)
            {
                int found_index = -1;
                for (int m=0; m<m_tcp_connect_info.size(); m++)
                {
                    if(m_tcp_connect_info.at(m).serialnumber == m_product.at(i).serial_number)
                    {
                        found_index = m;
                        bOnLine = TRUE;
                        temp_online = true;
                        break;
                    }
                }

                if(found_index >=0)
                {
                    bOnLine = TRUE;
                    temp_online = true;
                }
                else
                {
                    bOnLine = FALSE;
                    temp_online = false;
                }
                if (temp_online)
                {
                    m_product.at(i).status_last_time[4] = true;
                    m_product.at(i).status_last_time[3] = true;
                    m_product.at(i).status_last_time[2] = true;
                    m_product.at(i).status_last_time[1] = true;
                    m_product.at(i).status_last_time[0] = true;
                }
                else
                {
                    m_product.at(i).status_last_time[4] = m_product.at(i).status_last_time[3];
                    m_product.at(i).status_last_time[3] = m_product.at(i).status_last_time[2];
                    m_product.at(i).status_last_time[2] = m_product.at(i).status_last_time[1];
                    m_product.at(i).status_last_time[1] = m_product.at(i).status_last_time[0];
                    m_product.at(i).status_last_time[0] = temp_online;
                }
				m_product.at(i).status = m_product.at(i).status_last_time[0] || m_product.at(i).status_last_time[1] || m_product.at(i).status_last_time[2] || m_product.at(i).status_last_time[3] || m_product.at(i).status_last_time[4];
				continue;
            }
            else if(m_product.at(i).protocol == PROTOCOL_REMOTE_IP)
            {
                continue;
            }
            else if ((m_product.at(i).protocol == MODBUS_BACNET_MSTP) ||
                (m_product.at(i).protocol == PROTOCOL_MSTP_TO_MODBUS))
            {
                continue;
            }
            else
            {
                temp_online = false;
                //m_product.at(i).status = false;
                for (int x=0; x<(int)m_refresh_net_device_data.size(); x++)
                {
                    if(nSerialNumber == m_refresh_net_device_data.at(x).nSerial)
                    {
                        temp_online = true;
                        bOnLine = TRUE;
                        break;
                    }
                }

                if (temp_online)
                {
                    m_product.at(i).status_last_time[4] = true;
                    m_product.at(i).status_last_time[3] = true;
                    m_product.at(i).status_last_time[2] = true;
                    m_product.at(i).status_last_time[1] = true;
                    m_product.at(i).status_last_time[0] = true;
                }
                else
                {
                    m_product.at(i).status_last_time[4] = m_product.at(i).status_last_time[3];
                    m_product.at(i).status_last_time[3] = m_product.at(i).status_last_time[2];
                    m_product.at(i).status_last_time[2] = m_product.at(i).status_last_time[1];
                    m_product.at(i).status_last_time[1] = m_product.at(i).status_last_time[0];
                    m_product.at(i).status_last_time[0] = temp_online;
                }

                m_product.at(i).status = m_product.at(i).status_last_time[0] || m_product.at(i).status_last_time[1] || m_product.at(i).status_last_time[2] || m_product.at(i).status_last_time[3] || m_product.at(i).status_last_time[4];
				continue;
            }
        }
    }


    unsigned short mstp_array[1000];
    int nret_read_bac = 0;

	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

    //This function add by Fance
    //If the Database not contain the device wo scaned , then added into the database or update it;
    for (int y=0; y<(int)m_refresh_net_device_data.size(); y++)
    {
        bool db_exsit = false;
        int n_index = 0;
        bool db_need_update = false;
        if(m_refresh_net_device_data.at(y).nSerial <=0 )
            continue;
        for (int z=0; z<(int)m_product.size(); z++)
        {
            if(m_refresh_net_device_data.at(y).nSerial == m_product.at(z).serial_number)
            {
                n_index = z;
                db_exsit = true;
				 m_product.at(z).status = true;
                break;
            }
        }
        CString str_hardware_info;
        
        CString str_object_instance;
        CString str_panel_number;
        CString str_hw_version;
        CString str_fw_version;
        str_hw_version.Format(_T("%.1f"),m_refresh_net_device_data.at(y).hw_version);
        str_fw_version.Format(_T("%.1f"),m_refresh_net_device_data.at(y).sw_version/10.0);

        str_object_instance.Format(_T("%u"),m_refresh_net_device_data.at(y).object_instance);
        str_panel_number.Format(_T("%u"),m_refresh_net_device_data.at(y).panal_number);
        str_hardware_info.Format(_T("%u"), m_refresh_net_device_data.at(y).hardware_info);
        if(db_exsit)	//数据库存在，就查看是否要更新;
        {
            //如果是BIP 转MSTP 对已经存在的 不作刷新动作;  //需解决的问题是 已经存在的 子设备 如何刷新？
            //if (m_refresh_net_device_data.at(y).nprotocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
            //{
            //    continue;
            //}
            if((m_refresh_net_device_data.at(y).ip_address.CompareNoCase(m_product.at(n_index).BuildingInfo.strIp) != 0) ||
                    (m_refresh_net_device_data.at(y).NetCard_Address.CompareNoCase(m_product.at(n_index).NetworkCard_Address) != 0) ||
                    (m_refresh_net_device_data.at(y).nport != m_product.at(n_index).ncomport) ||
					(m_refresh_net_device_data.at(y).product_id != m_product.at(n_index).product_class_id) ||
                    (m_refresh_net_device_data.at(y).modbusID != m_product.at(n_index).product_id) ||
                    (m_refresh_net_device_data.at(y).hardware_info != m_product.at(n_index).nhardware_info) ||   //
                    (m_refresh_net_device_data.at(y).parent_serial_number != m_product.at(n_index).note_parent_serial_number) ||
                    //((m_product.at(n_index).protocol != MODBUS_TCPIP) && (m_product.at(n_index).protocol != PROTOCOL_BACNET_IP) && (m_product.at(n_index).protocol != PROTOCOL_BIP_TO_MSTP)) ||
                    (m_refresh_net_device_data.at(y).nprotocol!= m_product.at(n_index).protocol ) ||
                    ((m_refresh_net_device_data.at(y).panal_number != m_product.at(n_index).panel_number) && (m_refresh_net_device_data.at(y).panal_number != 0)) ||
                    ((m_refresh_net_device_data.at(y).object_instance != m_product.at(n_index).object_instance) && (m_refresh_net_device_data.at(y).object_instance != 0)) ||
                    ((m_refresh_net_device_data.at(y).show_label_name.CompareNoCase(m_product.at(n_index).NameShowOnTree) != 0)&& (!m_refresh_net_device_data.at(y).show_label_name.IsEmpty()) && (m_refresh_net_device_data.at(y).nprotocol != PROTOCOL_BIP_T0_MSTP_TO_MODBUS) ) ||
					(  abs((float)(m_refresh_net_device_data.at(y).sw_version - m_product.at(n_index).software_version*10)) >= 1 ) )
            {
                find_new_device = 1;
                CString strSql;
                CString str_ip_address_exist;
                CString str_n_port;
                CString str_serialid;
                CString str_modbus_id;
                CString str_Product_name_view;
                CString NetwordCard_Address;
                CString str_parents_serial;
                str_ip_address_exist = m_refresh_net_device_data.at(y).ip_address;
                str_n_port.Format(_T("%d"),m_refresh_net_device_data.at(y).nport);

                str_serialid.Format(_T("%u"),m_refresh_net_device_data.at(y).nSerial);
                str_modbus_id.Format(_T("%d"),m_refresh_net_device_data.at(y).modbusID);
                NetwordCard_Address=m_refresh_net_device_data.at(y).NetCard_Address;

                if(m_refresh_net_device_data.at(y).parent_serial_number != 0)
                {
                    str_parents_serial.Format(_T("%u"),m_refresh_net_device_data.at(y).parent_serial_number);
                }
                else
                {
                    str_parents_serial = _T("0");
                }

                try
                {
                    CString temp_pname;
                    CString temp_modbusid;
					CString temp_product_class_id;
					temp_product_class_id.Format(_T("%u"),m_refresh_net_device_data.at(y).product_id);
                    temp_modbusid.Format(_T("%d"),m_refresh_net_device_data.at(y).modbusID);
                    temp_pname = GetProductName(m_refresh_net_device_data.at(y).product_id);
                    if(m_refresh_net_device_data.at(y).show_label_name.IsEmpty())
                    {
                        str_Product_name_view = temp_pname + _T(":") + str_serialid + _T("-") + temp_modbusid + _T("-") + str_ip_address_exist;
                    }
                    else
                    {
                        str_Product_name_view = m_refresh_net_device_data.at(y).show_label_name;
						str_Product_name_view.Remove('\'');
						str_Product_name_view.Remove('\%');
                    }

                    bool is_bacnet_device = false;
                    if((m_refresh_net_device_data.at(y).product_id == PM_MINIPANEL)|| (m_refresh_net_device_data.at(y).product_id == PM_MINIPANEL_ARM) || (m_refresh_net_device_data.at(y).product_id == PM_CM5))
                        is_bacnet_device = true;

                    if((m_refresh_net_device_data.at(y).object_instance != 0) && (m_refresh_net_device_data.at(y).panal_number != 0) && is_bacnet_device && (m_refresh_net_device_data.at(y).parent_serial_number != 0))
                    {
                        CString temp_pro;
                        temp_pro.Format(_T("%u"),PROTOCOL_BIP_TO_MSTP);
                        strSql.Format(_T("update ALL_NODE set NetworkCard_Address='%s', Product_class_ID = '%s', Object_Instance = '%s' , Panal_Number = '%s' , Bautrate ='%s',Software_Ver = '%s' , Com_Port ='%s',Product_ID ='%s', Protocol ='%s',Product_name = '%s',Online_Status = 1 ,Parent_SerialNum = '%s' ,EPsize = '%s' where Serial_ID = '%s'"),
                            NetwordCard_Address,temp_product_class_id,str_object_instance,str_panel_number,str_ip_address_exist,str_fw_version,str_n_port,str_modbus_id,temp_pro,str_Product_name_view,str_parents_serial, str_hardware_info,str_serialid);
                    }
                    else if (m_refresh_net_device_data.at(y).nprotocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
                    {
                        CString temp_pro;
                        temp_pro.Format(_T("%u"), PROTOCOL_BIP_T0_MSTP_TO_MODBUS);
                        strSql.Format(_T("update ALL_NODE set NetworkCard_Address='%s', Product_class_ID = '%s', Object_Instance = '%s' , Panal_Number = '%s' , Bautrate ='%s',Software_Ver = '%s' , Com_Port ='%s',Product_ID ='%s', Protocol ='%s',Online_Status = 1 ,Parent_SerialNum = '%s'  ,EPsize = '%s' where Serial_ID = '%s'"), 
                            NetwordCard_Address, temp_product_class_id, str_object_instance, str_panel_number, str_ip_address_exist, str_fw_version, str_n_port, str_modbus_id, temp_pro, str_parents_serial, str_hardware_info, str_serialid);

                    }
                    else
                        strSql.Format(_T("update ALL_NODE set NetworkCard_Address='%s', Product_class_ID = '%s', Object_Instance = '%s' , Panal_Number = '%s' ,  Bautrate ='%s',Software_Ver = '%s' ,Com_Port ='%s',Product_ID ='%s', Protocol ='1',Product_name = '%s',Online_Status = 1,Parent_SerialNum = '%s'  ,EPsize = '%s' where Serial_ID = '%s'"),
                            NetwordCard_Address,temp_product_class_id,str_object_instance,str_panel_number,str_ip_address_exist,str_fw_version,str_n_port,str_modbus_id,str_Product_name_view,str_parents_serial, str_hardware_info , str_serialid);
                    find_new_device = true;
					SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                     
                }
                catch(_com_error *e)
                {
                    Sleep(1);
                    //AfxMessageBox(e->ErrorMessage());
                }


                m_product.at(n_index).ncomport = m_refresh_net_device_data.at(y).nport;
                m_product.at(n_index).BuildingInfo.strIp = m_refresh_net_device_data.at(y).ip_address;
                m_product.at(n_index).product_id = m_refresh_net_device_data.at(y).modbusID;

                if ((m_refresh_net_device_data.at(y).parent_serial_number != 0) && 
                    m_refresh_net_device_data.at(y).command_version >= 10)
                {
                    CString nsubnet_port;
                    nsubnet_port.Format(_T("%u"), m_refresh_net_device_data.at(y).subnet_port);
                    WritePrivateProfileStringW(str_serialid, _T("Subnet_port"), nsubnet_port, g_ext_database_path);
                    CString nsubnet_baudrate;
                    nsubnet_baudrate.Format(_T("%u"), m_refresh_net_device_data.at(y).subnet_baudrate);
                    WritePrivateProfileStringW(str_serialid, _T("Subnet_baudrate"), nsubnet_baudrate, g_ext_database_path);
                }
            }

        }
        else			//不存在 就插入;
        {
            CString strSql;
            CString str_ip_address;
            CString str_n_port;
            CString str_serialid;
            CString product_name;
            CString modbusid;
            CString product_class_id;
            CString NetwordCard_Address;
            CString str_parents_serial;
            CString is_custom;

            modbusid.Format(_T("%d"),m_refresh_net_device_data.at(y).modbusID);

            str_ip_address = m_refresh_net_device_data.at(y).ip_address;
            str_n_port.Format(_T("%d"),m_refresh_net_device_data.at(y).nport);

            str_serialid.Format(_T("%u"),m_refresh_net_device_data.at(y).nSerial);
            product_class_id.Format(_T("%u"),m_refresh_net_device_data.at(y).product_id);

			CppSQLite3Query query_serial;

            //删掉 存在的序列号
            try
            {

                CString strSql;

                strSql.Format(_T("Delete  from ALL_NODE where Serial_ID='%s'"), str_serialid);
                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            }
            catch (...)
            {

            }



			strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"), str_serialid);

			query_serial = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

			if (!query_serial.eof())
			{
				continue; //如果查询到有重复的就不插入
			}



            if (IS_Temco_Product(m_refresh_net_device_data.at(y).product_id))
            {
                is_custom = _T("0");
            }
            else
            {
                is_custom = _T("1");
            }
			if(m_refresh_net_device_data.at(y).show_label_name.IsEmpty())
			{
				 CString temp_default_name;
				 temp_default_name.Format(_T("%s-SN%s"),GetProductName(m_refresh_net_device_data.at(y).product_id),str_serialid);
				 product_name = temp_default_name;
			}
			else
			{
				product_name = m_refresh_net_device_data.at(y).show_label_name;
				product_name.Remove('\'');
				product_name.Remove('\%');
			}
           
            if(product_name.IsEmpty())
            {
                if (m_refresh_net_device_data.at(y).product_id<200)
                {
                    continue;
                }
                product_name = m_refresh_net_device_data.at(y).show_label_name;

				product_name.Remove('\'');
				product_name.Remove('\%');
            }


            if(m_refresh_net_device_data.at(y).parent_serial_number != 0)
            {
                str_parents_serial.Format(_T("%u"),m_refresh_net_device_data.at(y).parent_serial_number);
            }
            else
            {
                str_parents_serial = _T("0");
            }
            find_new_device = true;
            NetwordCard_Address=m_refresh_net_device_data.at(y).NetCard_Address;
            bool is_bacnet_device = false;
			is_bacnet_device = ShowBacnetView(m_refresh_net_device_data.at(y).product_id);
            if((m_refresh_net_device_data.at(y).object_instance != 0) && (m_refresh_net_device_data.at(y).panal_number != 0) && is_bacnet_device && (m_refresh_net_device_data.at(y).parent_serial_number != 0))
            {
                CString temp_pro2;
                temp_pro2.Format(_T("%u"),PROTOCOL_BIP_TO_MSTP);
                strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('"+m_strCurMainBuildingName+"','"+m_strCurSubBuldingName+"','"+NetwordCard_Address+"','"+str_serialid+"','floor1','room1','"+product_name+"','"+product_class_id+"','"+modbusid+"','""','"+str_ip_address+"','Modbus_and_Bacnet','"+str_hw_version+"','"+str_fw_version+"','"+str_n_port+"','"+ str_hardware_info +"','"+temp_pro2+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
            }
			else if((m_refresh_net_device_data.at(y).object_instance != 0) && (m_refresh_net_device_data.at(y).panal_number != 0) && is_bacnet_device)
			{
				CString temp_pro3;
				temp_pro3.Format(_T("%u"),PROTOCOL_BACNET_IP);
				strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('"+m_strCurMainBuildingName+"','"+m_strCurSubBuldingName+"','"+NetwordCard_Address+"','"+str_serialid+"','floor1','room1','"+product_name+"','"+product_class_id+"','"+modbusid+"','""','"+str_ip_address+"','Modbus_and_Bacnet','"+str_hw_version+"','"+str_fw_version+"','"+str_n_port+"','"+ str_hardware_info +"','"+temp_pro3+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
			}
            else if (m_refresh_net_device_data.at(y).nprotocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
            {
                //if (initial_bip == true)
                //{
                //    nret_read_bac = GetPrivateBacnetToModbusData(m_refresh_net_device_data.at(y).object_instance, 0, 100, mstp_array);
                //    if (nret_read_bac == 100)
                //    {
                //        product_class_id.Format(_T("%d"), mstp_array[7]);
                        //序列号这里肯定还有问题 ，因为客户的instance 可能会认为更改;
                        //str_serialid.Format(_T("%u"), mstp_array[0] + mstp_array[1] * 256 + mstp_array[2] * 256 * 256 + mstp_array[3] * 256 * 256 * 256);
                        CString temp_pro4;
                        temp_pro4.Format(_T("%u"), PROTOCOL_BIP_T0_MSTP_TO_MODBUS);
                        strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('" + m_strCurMainBuildingName + "','" + m_strCurSubBuldingName + "','" + NetwordCard_Address + "','" + str_serialid + "','floor1','room1','" + product_name + "','" + product_class_id + "','" + modbusid + "','""','" + str_ip_address + "','Modbus_and_Bacnet','" + str_hw_version + "','" + str_fw_version + "','" + str_n_port + "','"+ str_hardware_info +"','" + temp_pro4 + "','1','" + str_parents_serial + "' ,'" + str_panel_number + "' ,'" + str_object_instance + "' ,'" + is_custom + "' )"));
                //    }
                //    else if (nret_read_bac < 0)
                //    {
                //        g_Print.Format(_T("Read instance %u ,  MSTP TO MODBUS  0 - 100   timeout!"), m_refresh_net_device_data.at(y).object_instance);
                //        DFTrace(g_Print);
                 //   }

                //}

            }
            else
			{
				CString temp_pro4;
				temp_pro4.Format(_T("%u"),MODBUS_TCPIP);

                if (m_refresh_net_device_data.at(y).parent_serial_number != 0)
                {
                    strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('" + m_strCurMainBuildingName + "','" + m_strCurSubBuldingName + "','" + NetwordCard_Address + "','" + str_serialid + "','floor1','room1','" + product_name + "','" + product_class_id + "','" + modbusid + "','""','" + str_ip_address + "','Sub Device RS485','" + str_hw_version + "','" + str_fw_version + "','" + str_n_port + "','" + str_hardware_info + "','" + temp_pro4 + "','1','" + str_parents_serial + "' ,'" + str_panel_number + "' ,'" + str_object_instance + "' ,'" + is_custom + "' )"));
                }
                else 
                    strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('" + m_strCurMainBuildingName + "','" + m_strCurSubBuldingName + "','" + NetwordCard_Address + "','" + str_serialid + "','floor1','room1','" + product_name + "','"+product_class_id+"','"+modbusid+"','""','"+str_ip_address+"','Modbus TCP / BacnetIP','"+str_hw_version+"','"+str_fw_version+"','"+str_n_port+"','"+ str_hardware_info +"','"+temp_pro4+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
			}
            try
            {

                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            } 
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }

            //子设备才存储这些额外的信息;
            if (m_refresh_net_device_data.at(y).parent_serial_number != 0)
            {
                CString nsubnet_port;
                nsubnet_port.Format(_T("%u"), m_refresh_net_device_data.at(y).subnet_port);
                WritePrivateProfileStringW(str_serialid, _T("Subnet_port"), nsubnet_port, g_ext_database_path);
                CString nsubnet_baudrate;
                nsubnet_baudrate.Format(_T("%u"), m_refresh_net_device_data.at(y).subnet_baudrate);
                WritePrivateProfileStringW(str_serialid, _T("Subnet_baudrate"), nsubnet_baudrate, g_ext_database_path);
            }

        }

    }

	//Fandu 2017/12/13 新增批量处理在线状态，修改数据库状态字段.
	CString strUpdateSql;
	if (m_refresh_net_device_data.size() == 0)
	{
		strUpdateSql.Format(_T("update ALL_NODE set Online_Status	= 0 where protocol = 1"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strUpdateSql);
	}
	else
	{
		CString online_composite_serial;
		for (int i = 0;i < m_refresh_net_device_data.size();i++)
		{
			CString temp1;
			if (i != 0)
                online_composite_serial = online_composite_serial + _T(",");
			temp1.Format(_T("%u"), m_refresh_net_device_data.at(i).nSerial);
            online_composite_serial = online_composite_serial + temp1;

            if (m_refresh_net_device_data.at(i).parent_serial_number != 0) //确保 所有的父节点都在线;即使没有回复;
            {
                temp1.Format(_T(",%u"), m_refresh_net_device_data.at(i).parent_serial_number);
                online_composite_serial = online_composite_serial + temp1;
            }
		}
        CString offline_composite_serial;
        int offline_count = 0;
        for (int i = 0;i < (int)m_product.size();i++)
        {
            if (m_product.at(i).status == false)
            {
                CString temp1;
                if (offline_count != 0)
                    offline_composite_serial = offline_composite_serial + _T(",");
                temp1.Format(_T("%u"), m_product.at(i).serial_number);
                offline_composite_serial = offline_composite_serial + temp1;
                offline_count++;
            }
        }
        //将回复 协议不是1 的 网络设备  状态设置为 offline. 
		strUpdateSql.Format(_T("update ALL_NODE set Online_Status = 0 where Serial_ID in (%s) and (protocol = 1  or protocol = %d )"), offline_composite_serial, PROTOCOL_BIP_T0_MSTP_TO_MODBUS);
		SqliteDBBuilding.execDML((UTF8MBSTR)strUpdateSql);

        strUpdateSql.Format(_T("update ALL_NODE set Online_Status = 1 where Serial_ID in (%s) and (protocol = 1 or protocol = %d)"), online_composite_serial, PROTOCOL_BIP_T0_MSTP_TO_MODBUS);
        SqliteDBBuilding.execDML((UTF8MBSTR)strUpdateSql);

        CString subnet_composite_serial;
        //特殊回复处理 0x2f 回复子网设备的命令
        for (int z = 0; z < m_refresh_subnet_status.size(); z++)
        {
            subnet_composite_serial.Empty();
            int n_item_count = 0; // 在线子设备计数;
            for (int y = 0; y < m_refresh_subnet_status.at(z).device_count; y++)
            {
                if (m_refresh_subnet_status.at(z).device_status[y].nstatus == 1)
                {
                    CString temp1;
                    if (n_item_count != 0)
                        subnet_composite_serial = subnet_composite_serial + _T(",");
                    temp1.Format(_T("%u"), m_refresh_subnet_status.at(z).device_status[y].modbusid);
                    subnet_composite_serial = subnet_composite_serial + temp1;
                    n_item_count++;
                }

            }
            strUpdateSql.Format(_T("update ALL_NODE set Online_Status = 1 where Product_ID in (%s) and (Parent_SerialNum = %d)"), subnet_composite_serial, m_refresh_subnet_status.at(z).parent_sn);
            SqliteDBBuilding.execDML((UTF8MBSTR)strUpdateSql);
        }

	}

    SqliteDBBuilding.closedb();
    if(find_new_device)
        PostMessage(WM_MYMSG_REFRESHBUILDING,0,0);
    else
        PostMessage(WM_REFRESH_TREEVIEW_MAP, 0, 0);
    return TRUE;
}


LRESULT  CMainFrame::HandleIspModedivice(WPARAM wParam, LPARAM lParam)
{
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	CString temp_AutoFlashConfigPath = ApplicationFolder + _T("//AutoFlashFile.ini");
	CFileFind fFind;
	if(fFind.FindFile(temp_AutoFlashConfigPath))
	{
		DeleteFile(temp_AutoFlashConfigPath);
	}

	int temp_product_count = m_product.size();



	b_pause_refresh_tree = HANDLE_ISP_MODE;
	bool temp_status = g_bPauseMultiRead;
	g_bPauseMultiRead = true;
	int temp_type = GetCommunicationType();


	BOOL bDontLinger = FALSE;
	setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
	closesocket(h_Broad);
	
	CString temp_ipaddress;
	temp_ipaddress.Format(_T("%u.%u.%u.%u"),need_isp_device.ipaddress[0],need_isp_device.ipaddress[1],need_isp_device.ipaddress[2],need_isp_device.ipaddress[3]);

		m_product_isp_auto_flash.baudrate = 0;
		m_product_isp_auto_flash.BuildingInfo.strIp = temp_ipaddress;
		m_product_isp_auto_flash.ncomport =  502;

		m_product_isp_auto_flash.product_class_id =  need_isp_device.product_id;
		m_product_isp_auto_flash.product_id =  255;
		m_product_isp_auto_flash.note_parent_serial_number = 0;
		m_product_isp_auto_flash.software_version = 0;


		isp_mode_is_cancel = true;
		isp_mode_firmware_auto = true;

		CISPModeSlove dlg;
		dlg.DoModal();
			Dowmloadfile Dlg;
		if(isp_mode_is_cancel)
		{
			goto finished_detect;
		}

	SetCommunicationType(0);//关闭串口，供ISP 使用;
	close_com();

	Dlg.DoModal();


finished_detect:

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

	h_bcast.sin_family=AF_INET;
	h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
	h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

	h_siBind.sin_family=AF_INET;
	h_siBind.sin_addr.s_addr=INADDR_ANY;
	h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
	::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
	b_pause_refresh_tree = false;
	g_bPauseMultiRead = temp_status;

	isp_mode_is_cancel = true;
	isp_mode_firmware_auto = true;
	return TRUE;
}

LRESULT  CMainFrame::HandleDuplicateID(WPARAM wParam, LPARAM lParam)
{
	static int  bool_show_duplicated_window_lim = 0;
	int duplicate_mode = (int)wParam;
	if(bool_show_duplicated_window_lim++ > 5)
	{
		edit_confilct_mode = true;
		return 1;
	}

	int temp_type =  GetCommunicationType();

	b_pause_refresh_tree = HANDLE_DUPLICATE_ID;
//#ifndef DEBUG

    CDuplicateIdDetected Dlg(duplicate_mode, NULL);
    Dlg.DoModal();

//#endif // DEBUG



	edit_confilct_mode = false;
	b_pause_refresh_tree = false;

	SetCommunicationType(temp_type);
	return 1;
}


LRESULT  CMainFrame::RefreshTreeViewMap(WPARAM wParam, LPARAM lParam)
{
    for (UINT i = 0; i < m_product.size(); i++)
    {
        tree_product tp = m_product.at(i);
        if (tp.protocol == P_MODBUS_485)
            continue;

#pragma region hanlde_ping
        CStringArray temp_array;
        SplitCStringA(temp_array, tp.BuildingInfo.strIp, _T("."));
        if (temp_array.GetSize() == 4)
        {
            CString temp_head_ip;
            temp_head_ip.Format(_T("%s.%s.%s."), temp_array.GetAt(0), temp_array.GetAt(1), temp_array.GetAt(2));
            if (temp_head_ip.CompareNoCase(g_ipaddress_info.ip_head) == 0)
            {
                CString temp_last_ip;
                temp_last_ip = temp_array.GetAt(3);
                unsigned char nlastip = (unsigned char)(_wtoi(temp_last_ip));
                if (tp.note_parent_serial_number == 0) //处理 父节点为0的情况;
                {
                    if (tp.status == 0)
                    {
                        if (g_ipaddress_info.ip_status[nlastip] == 1)
                        {
                            tp.status = 1;
                            m_product.at(i).status = 1;
                            TRACE(_T("Change %s.%d status to 1\r\n"), temp_head_ip, nlastip);
                        }
                    }
                }
            }
        }
#pragma endregion


        if (tp.status > 0)    // 如果online，更新显示图片
        {
            m_pTreeViewCrl->turn_item_image(tp.product_item, true);
        }
        else  // 替换offline的图片
        {
            //如果设备不在线了，毛总的意思是Ping一次，确认是否在线;再次确认
            m_pTreeViewCrl->turn_item_image(tp.product_item, false);
        }
        if (g_selected_serialnumber == m_product.at(i).serial_number)
        {
            if (tp.status > 0)
                bac_select_device_online = true;
            else
            {
                //if(tp.product_class_id	!= PM_MINIPANEL)
                bac_select_device_online = false; //对minipanel特殊处理, 被选中的设备不回 64 ，而其他通信又正常，头疼;
            }
        }
    }
    return 0;
}


LRESULT CMainFrame::Message_Scan_Product(WPARAM wParam, LPARAM lParam)
{
    OnScanDevice();
    return 0;
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
    PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);
    //RefreshTreeView();
    if ((m_nCurView == 0) && (g_tstat_id != 0) && (g_tstat_id != 255))
    {
        ((CT3000View*)m_pViews[m_nCurView])->PostMessage(WM_FRESHVIEW,0,0)	;
    }

}

void Send_Key_Value(HWND hwnd,LPCSTR	 lpString)
{
	//lpString->get
	while(1)
	{
		SendMessageA(hwnd,WM_CHAR,*lpString,0);
		lpString++;
		if(*lpString==0)
		{
			break;
		}
	}
}

HWND my_window_hwnd;
static int step_step =0;
BOOL CALLBACK enum3Dinstall_two(HWND   hwnd,   LPARAM   lParam)
{
	step_step ++ ;
	if(step_step == 6)
	{
		my_window_hwnd = hwnd;
		return false;
	}
	return TRUE;
}

UINT _FreshTreeView(LPVOID pParam )
{

    CString g_strT3000LogString;
    CMainFrame* pMain = (CMainFrame*)pParam;
    int int_refresh_com = 0;
    refresh_tree_status_immediately = true;
    while(1)
    {
#if 0
		Sleep(10000);
		HWND temp1_hwmd ;//= 0x000211F0;
		HWND temp2_hwmd;
		temp1_hwmd = FindWindowA(NULL,"十步一杀人，千里不留行 - 群(32984537)");
		::EnumChildWindows(temp1_hwmd,(WNDENUMPROC)enum3Dinstall_two,1);
		int aaaaaaaaaa = 0;
		while(1)
		{		
			unsigned long	my_time = time(NULL);
			aaaaaaaaaa = 1516267800 - my_time;
			if(aaaaaaaaaa < 0)
				return 1;
			//aaaaaaaaaa++;
			CString temp_cs;
			temp_cs.Format(_T("Time %u s"),aaaaaaaaaa);
			char temp_1[20];
			WideCharToMultiByte(CP_ACP,0,temp_cs.GetBuffer(),-1,temp_1,255,NULL,NULL);
			Send_Key_Value(my_window_hwnd,temp_1);
			CRect temp_rect;
			GetWindowRect(my_window_hwnd,temp_rect);
			SetCursorPos(temp_rect.right - 40,temp_rect.bottom + 5);
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

			//Sleep(1);
			//SendMessage(my_window_hwnd,WM_KEYDOWN,VK_RETURN,NULL); 
			//Sleep(1);
			//SendMessage(my_window_hwnd,WM_KEYUP,VK_RETURN,NULL);
			Sleep(1000);
			continue;

		}
#endif



		if(scaning_mode)
		{
			Sleep(1000);
			continue;
		}

        if (!pMain->m_frist_start)
        {
			for (int x=0;x<60;x++)
			{
				if(refresh_tree_status_immediately)  //在设备的IP 或端口改变后 ，其他文件会将此flag设置为true 要求重新刷新;
				{
					refresh_tree_status_immediately = false;
					break;
				}
				if((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
				{
					if(x%3 == 0)
					{
						g_Print.Format(_T("Send scan command after %d(s)"),60 - x);
						DFTrace(g_Print);
					}

				}

				Sleep(1000);
			}
            
        }
		pMain->m_frist_start = false;
		if(b_remote_connection)	//如果是远程连接的设备 就不扫描 , 也无法通过 0x64 判断远程的设备是否在线.  Fance;
		{
			if((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
			{
				g_Print.Format(_T("Remote conncet give up auto background scan."));
				DFTrace(g_Print);
			}
			Sleep(1000);
			continue;
		}


        WaitForSingleObject(Read_Mutex, INFINITE);//Add by Fance .
        if (b_pause_refresh_tree)
        {
            if ((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
            {
                g_Print.Format(_T("The background scan thread has paused."));
                DFTrace(g_Print);
            }
            ReleaseMutex(Read_Mutex);//Add by Fance .
            b_pause_refresh_tree = false;
            continue;
        }


        m_refresh_net_device_data.clear();
        m_refresh_subnet_status.clear();
        RefreshNetWorkDeviceListByUDPFunc();


        if (!pMain->CheckDeviceStatus(int_refresh_com))
        {
            ReleaseMutex(Read_Mutex);
            continue;
        }
        if(edit_confilct_mode == false)
		    pMain->CheckDuplicate();
        if (edit_confilct_mode == false)
            pMain->CheckIPDuplicate();
        pMain->DoFreshAll();
        ReleaseMutex(Read_Mutex);//Add by Fance .
        int_refresh_com = (++int_refresh_com) %4;

        static bool delete_once = false;

        if (delete_once == false)
        {
            delete_once = true;
            try {
                bool find_same_serial_number = false;
                for (int z = 0;z < pMain->m_product.size();z++)
                {
                    for (int y = z + 1;y < pMain->m_product.size();y++)
                    {
                        if (pMain->m_product.at(z).serial_number == pMain->m_product.at(y).serial_number)
                        {
                            find_same_serial_number = true;
                            break;
                        }
                    }
                }

                if (find_same_serial_number)
                {
                    pMain->DeleteConflictInDB();
                }
            }
            catch (_com_error *e)
            {
                Sleep(1);
            }
        }

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
        g_bPauseMultiRead=FALSE;
        KillTimer(SCAN_TIMER);
        m_wndWorkSpace.m_TreeCtrl.Invalidate();

        //SelectTreeNodeFromRecord();//scan 解决scan完后，点击所扫到的项，显示com不对问题。

        //////////////////////////////////////////////////////////////////////////
        delete m_pScanner;
        m_pScanner =NULL;
    }
    g_bEnableRefreshTreeView =TRUE;
    b_pause_refresh_tree = FALSE;
    PostMessage(WM_REFRESH_TREEVIEW_MAP,0,0);//扫描完了之后关闭扫描窗口，还要刷新Treeview;
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



LRESULT CMainFrame::OnHotKey(WPARAM wParam,LPARAM lParam)
{
    UINT fuModifiers = (UINT) LOWORD(lParam);  // key-modifier flags
    UINT uVirtKey = (UINT) HIWORD(lParam);     // virtual-key code

    if(g_protocol == PROTOCOL_BACNET_IP)
    {
        Program_Window->Unreg_Hotkey();
        Screen_Window->Unreg_Hotkey();
        WeeklyRoutine_Window->Unreg_Hotkey();
        AnnualRoutine_Window->Unreg_Hotkey();
        Monitor_Window->Unreg_Hotkey();
    }
    //判断响应了什么热键
    if( MOD_ALT == fuModifiers && 'G' == uVirtKey )  //Screen
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
    else if(MOD_ALT == fuModifiers && 'L' == uVirtKey)
    {
        OnControlControllers();
    }
    else if(MOD_ALT == fuModifiers && 'S' == uVirtKey)
    {
        OnControlWeekly();
    }
    else if(MOD_ALT == fuModifiers && 'H' == uVirtKey)
    {
        OnControlAnnualroutines();
    }
    else if(MOD_ALT == fuModifiers && 'T' == uVirtKey)
    {
        OnControlMonitors();
    }
    else if(MOD_ALT == fuModifiers && 'A' == uVirtKey)
    {
        OnControlAlarmLog();
    }
    else if(MOD_ALT == fuModifiers && 'U' == uVirtKey)
    {
        OnControlCustomerunits();
    }
    else if(MOD_ALT == fuModifiers && 'C' == uVirtKey)
    {
        OnControlSettings();
    }
    else if(MOD_ALT == fuModifiers && 'B' == uVirtKey)
    {
        OnAddBuildingConfig();
    }
    else if(MOD_ALT == fuModifiers && 'D' == uVirtKey)
    {
        OnScanDevice();
    }
    else if((MOD_SHIFT | MOD_CONTROL | MOD_ALT) == fuModifiers && 'D' == uVirtKey)//Annual Routines
    {
        enable_show_debug_window = true;
    }
    else if((MOD_SHIFT | MOD_CONTROL | MOD_ALT) == fuModifiers && 'F' == uVirtKey)//Annual Routines
    {
        if(enable_show_debug_window)
            ShowDebugWindow();
    }
    return 0;
}


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
        ZeroMemory(szRet, 64);
        dwSize=64;
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
        ZeroMemory(szRet, 64);
        dwSize=64;
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
        ZeroMemory(szRet, 64);
        dwSize=64;
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
    struct LANGANDCODEPAGE
    {
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


//Code by Fance
//Used for receive all message from all dlg
DWORD WINAPI   CMainFrame::Get_All_Dlg_Message(LPVOID lpVoid)
{
    _MessageWriteOneInfo *My_Write_Struct=NULL;
    MSG msg;
    PeekMessage(&msg, NULL, MY_WRITE_ONE, MY_CLOSE, PM_REMOVE);
    while(true)
    {
        Sleep(1);
        if(GetMessage(&msg,0,0,0)) //get msg from message queue
        {
            if (g_protocol == P_BACNET_MSTP)
            {
                if (My_Receive_msg.size() > 20)
                {
                    MyCriticalSection.Lock();
                    My_Receive_msg.clear();
                    MyCriticalSection.Unlock();
                    continue;
                }
            }
            else if (My_Receive_msg.size() > 100)
            {
                MyCriticalSection.Lock();
                My_Receive_msg.clear();
                MyCriticalSection.Unlock();
                continue;
            }

            switch(msg.message)
            {
            case MY_WRITE_ONE:
                MyCriticalSection.Lock();
                My_Receive_msg.push_back(msg);
                MyCriticalSection.Unlock();

                My_Write_Struct= (_MessageWriteOneInfo *)msg.wParam;
                product_register_value[My_Write_Struct->address] = My_Write_Struct->new_value;//先变过来，免得后台更新的时候 乱变。
                break;
            case MY_WRITE_ONE_LIST:
                MyCriticalSection.Lock();
                My_Receive_msg.push_back(msg);
                MyCriticalSection.Unlock();
            case MY_WRITE_MULTI_LIST:
                MyCriticalSection.Lock();
                My_Receive_msg.push_back(msg);
                MyCriticalSection.Unlock();
// 				My_Write_Struct= (_MessageWriteOneInfo_List *)msg.wParam;
// 				product_register_value[My_Write_Struct->address] = My_Write_Struct->new_value;//先变过来，免得后台更新的时候 乱变。
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
			case MY_RS485_WRITE_LIST:
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
            //TRACE(_T("My_Receive_msg value : %d\r\n"), My_Receive_msg.size());
  
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
                    if(My_Write_Struct->address < 1024)
                        product_register_value[My_Write_Struct->address] = My_Write_Struct->new_value;
                    ::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_SUCCESS,(LPARAM)My_Write_Struct);
                }
            }
            break;
            case MY_WRITE_ONE_LIST:
            {
                MyCriticalSection.Lock();
                if (My_Receive_msg.size()<=0)
                {
                    return 0;
                }
                msg=My_Receive_msg.at(0);
                vector<MSG>::iterator it=My_Receive_msg.begin();
                //it++;
                //for (;it!=My_Receive_msg.end();)
                while(it!=My_Receive_msg.end())
                {
                    if (msg.hwnd==it->hwnd&&
                            msg.lParam==it->lParam&&
                            msg.message==it->message&&
                            msg.pt.x==it->pt.x&&
                            msg.pt.y==it->pt.y&&
                            msg.time==it->time&&
                            msg.wParam==it->wParam)
                    {

                        My_Receive_msg.erase(it);
                        it=	 My_Receive_msg.begin();
                    }
                    else
                    {
                        it++;
                    }
                }

                _MessageWriteOneInfo_List *My_Write_Struct = (_MessageWriteOneInfo_List *)msg.wParam;
                //	My_Receive_msg.erase(My_Receive_msg.begin());
                MyCriticalSection.Unlock();

                if(write_one(My_Write_Struct->device_id, My_Write_Struct->address,My_Write_Struct->new_value,10)<0)
                {

                    ::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_ONE_FAIL_LIST,(LPARAM)My_Write_Struct);
                }
                else
                {
                    product_register_value[My_Write_Struct->address]=My_Write_Struct->new_value;
                    if (My_Write_Struct->list_type==LIST_TYPE_INPUT_TSTAT)
                    {
                        if (My_Write_Struct->mCol==INPUT_RANGE)
                        {
                            if (My_Write_Struct->mRow>=1&&My_Write_Struct->mRow<=8)
                            {
                                
                            }


                            int ret=Read_Multi(g_tstat_id,&multi_register_value[MODBUS_TEMPRATURE_CHIP],MODBUS_TEMPRATURE_CHIP,20);
                            if (ret>0)
                            {
                                for (int i=0; i<20; i++)
                                {
                                    product_register_value[MODBUS_TEMPRATURE_CHIP+i]=multi_register_value[MODBUS_TEMPRATURE_CHIP+i];
                                }
                            }

                        }

                        if (product_type==T3000_6_ADDRESS)
                        {
                            LoadTstat_InputData();
                        }
                        else if (product_type==CS3000)
                        {
                            LoadInputData_CS3000();
                        }
                    }
                    if (My_Write_Struct->list_type==LIST_TYPE_OUTPUT_TSTAT)
                    {
                        if (product_type==T3000_6_ADDRESS)
                        {
                            LoadTstat_OutputData();
                        }
                        else if (product_type==CS3000)
                        {
                            LoadOutputData_CS3000();
                        }
                    }
                    ::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_ONE_SUCCESS_LIST,(LPARAM)My_Write_Struct);
                }

            }
            break;
            case MY_WRITE_MULTI_LIST:
            {
                MyCriticalSection.Lock();
                msg=My_Receive_msg.at(0);
                _MessageWriteMultiInfo_List *My_Write_Struct = (_MessageWriteMultiInfo_List *)msg.wParam;
                My_Receive_msg.erase(My_Receive_msg.begin());
                MyCriticalSection.Unlock();

                if(Write_Multi(My_Write_Struct->device_id,My_Write_Struct->RegValue,My_Write_Struct->Start_Address,8,10)<0)
                {

                    ::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_MULTI_FAIL_LIST,(LPARAM)My_Write_Struct);
                }
                else
                {
                    UNION_INPUT_NAME inputname;

					inputname.char_name[0]=My_Write_Struct->RegValue[1];
					inputname.char_name[1]=My_Write_Struct->RegValue[0];

					inputname.char_name[2]=My_Write_Struct->RegValue[3];
					inputname.char_name[3]=My_Write_Struct->RegValue[2];

					inputname.char_name[4]=My_Write_Struct->RegValue[5];
					inputname.char_name[5]=My_Write_Struct->RegValue[4];

					inputname.char_name[6]=My_Write_Struct->RegValue[7];
					inputname.char_name[7]=My_Write_Struct->RegValue[6];


                    for (int i=0; i<4; i++)
                    {
                        product_register_value[My_Write_Struct->Start_Address+i]=inputname.reg_value[i];
                    }
                    ::PostMessage(My_Write_Struct->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_MULTI_SUCCESS_LIST,(LPARAM)My_Write_Struct);
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
            {
                MyCriticalSection.Lock();
                msg = My_Receive_msg.at(0);
                My_Invoke_Struct = (_MessageInvokeIDInfo *)msg.wParam;
                My_Receive_msg.erase(My_Receive_msg.begin());
                MyCriticalSection.Unlock();
                for (int i = 0; i < 2000; i++) //10秒钟判断任务是否超时.
                {
                    if (tsm_invoke_id_free(My_Invoke_Struct->Invoke_ID))
                    {
                        ::PostMessage(MainFram_hwd, MY_RX_TX_COUNT, 1, 0);
                        ::PostMessage(My_Invoke_Struct->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)My_Invoke_Struct);
                        goto loop1;
                    }
                    else
                    {

                        //Sleep(30);
                        Sleep(5);
                        continue;
                    }
                }
                ::PostMessage(MainFram_hwd, MY_RX_TX_COUNT, 0, 0);
                ::PostMessage(My_Invoke_Struct->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)My_Invoke_Struct);
                //			tsm_free_invoke_id (My_Invoke_Struct->Invoke_ID);
            }
loop1:
                break;
			case MY_RS485_WRITE_LIST:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageWriteListInfo *My_WriteList_Struct = (_MessageWriteListInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();
					unsigned short write_buffer[200];
					memset(write_buffer,0,400);
					int test_value1 = 0;
					switch( (unsigned char)My_WriteList_Struct->command )
					{
					case WRITEANALOG_CUS_TABLE_T3000:
					{
						memcpy(write_buffer, &m_analog_custmer_range.at(My_WriteList_Struct->start_instance), sizeof(Str_table_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
                            for (int j = 0;j < 200;j++)
                            {
                                write_buffer[j] = htons(write_buffer[j]);
                            }

						test_value1 = Write_Multi_org_short(g_tstat_id, write_buffer, BAC_CUSTOMER_TABLE_START + 53 * My_WriteList_Struct->start_instance, 53, 4);

						_MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
						pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
						pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
						pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
						pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;

						if (test_value1 > 0)
						{
							::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)pMy_Invoke_id);
						}
						else
						{
							::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)pMy_Invoke_id);
						}
					}
					break;
					case WRITEOUTPUT_T3000:
						{
							memcpy( write_buffer,&m_Output_data.at(My_WriteList_Struct->start_instance),sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
							for (int j=0;j<200;j++)
							{
								write_buffer[j] = htons(write_buffer[j]);
							}
							test_value1 = Write_Multi_org_short(g_tstat_id,write_buffer,BAC_OUT_START_REG + 23 * My_WriteList_Struct->start_instance , 23, 4);

							_MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
							pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
							pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
							pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
							pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;

							if(test_value1 > 0)
							{
								 ::PostMessage(pMy_Invoke_id->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_SUCCESS,(LPARAM)pMy_Invoke_id);
							}
							else
							{
								 ::PostMessage(pMy_Invoke_id->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_FAIL,(LPARAM)pMy_Invoke_id);
							}
						}
						break;
					case WRITEINPUT_T3000:
						{
							memcpy( write_buffer,&m_Input_data.at(My_WriteList_Struct->start_instance),sizeof(Str_in_point));//因为IN只有46个字节，两个byte放到1个 modbus的寄存器里面;
							for (int j=0;j<200;j++)
							{
								write_buffer[j] = htons(write_buffer[j]);
							}
							test_value1 = Write_Multi_org_short(g_tstat_id,write_buffer,BAC_IN_START_REG + 23 * My_WriteList_Struct->start_instance , 23, 4);

							_MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
							pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
							pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
							pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
							pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
							if(test_value1 > 0)
							{
								::PostMessage(pMy_Invoke_id->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_SUCCESS,(LPARAM)pMy_Invoke_id);
							}
							else
							{
								::PostMessage(pMy_Invoke_id->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_FAIL,(LPARAM)pMy_Invoke_id);
							}
						}
						break;

                    case WRITEVARIABLE_T3000:
                    {
                        memcpy(write_buffer, &m_Variable_data.at(My_WriteList_Struct->start_instance), sizeof(Str_variable_point));
                        for (int j = 0;j<200;j++)
                        {
                            write_buffer[j] = htons(write_buffer[j]);
                        }
                        test_value1 = Write_Multi_org_short(g_tstat_id, write_buffer, BAC_VAR_START_REG + 20 * My_WriteList_Struct->start_instance, 20, 4); //Variable 是39个字节，占用20个寄存器;

                        _MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
                        pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
                        pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
                        pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
                        pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
                        if (test_value1 > 0)
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)pMy_Invoke_id);
                        }
                        else
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)pMy_Invoke_id);
                        }
                    }
                    break;

					case WRITEPID_T3000:
						{
							memcpy( write_buffer,&m_controller_data.at(My_WriteList_Struct->start_instance),sizeof(Str_controller_point));//因为IN只有46个字节，两个byte放到1个 modbus的寄存器里面;
							for (int j=0;j<200;j++)
							{
								write_buffer[j] = htons(write_buffer[j]);
							}
							test_value1 = Write_Multi_org_short(g_tstat_id,write_buffer,BAC_PID_CONTROL_START_REG + 14 * My_WriteList_Struct->start_instance , 14, 4);

							_MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
							pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
							pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
							pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
							pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
							if(test_value1 > 0)
							{
								::PostMessage(pMy_Invoke_id->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_SUCCESS,(LPARAM)pMy_Invoke_id);
							}
							else
							{
								::PostMessage(pMy_Invoke_id->hwnd,MY_RESUME_DATA,(WPARAM)WRITE_FAIL,(LPARAM)pMy_Invoke_id);
							}
						}
						break;
                    case WRITESCHEDULE_T3000:
                    {
                        memcpy(write_buffer, &m_Weekly_data.at(My_WriteList_Struct->start_instance), sizeof(Str_weekly_routine_point));
                        for (int j = 0;j<200;j++)
                        {
                            write_buffer[j] = htons(write_buffer[j]);
                        }
                        test_value1 = Write_Multi_org_short(g_tstat_id, write_buffer, BAC_SCH_START_REG + 21 * My_WriteList_Struct->start_instance, 21, 4); //Variable 是39个字节，占用20个寄存器;

                        _MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
                        pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
                        pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
                        pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
                        pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
                        if (test_value1 > 0)
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)pMy_Invoke_id);
                        }
                        else
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)pMy_Invoke_id);
                        }
                    }
                    break;
                    case WRITETIMESCHEDULE_T3000:
                    {
                        unsigned short temp_data[72] = { 0 };
                        unsigned short * temp_point = temp_data;
                        memcpy(temp_point, &m_Schedual_Time_data.at(weekly_list_line), 72 * 2);

                        unsigned short * temp_write_point = write_buffer;

                        for (int z = 0; z < 9; z++)
                        {
                            for (int y = 0;y < 8;y++)
                            {
                                *temp_write_point = temp_point[y * 9 + z];
                                temp_write_point++;
                            }
                        }







                        //unsigned short temp_data[72] = { 0 };
                        //memcpy(temp_data, &m_Schedual_Time_data.at(My_WriteList_Struct->start_instance), sizeof(Str_schedual_time_point));
                        //unsigned short * temp_point = temp_data;
                        //for (int z = 0; z < 8; z++)
                        //{
                        //    for (int y = 0;y < 9;y++)
                        //    {
                        //        write_buffer[z * 9 + y] = *temp_point;
                        //        temp_point++;
                        //    }
                        //}

                        //memcpy(write_buffer, &m_Schedual_Time_data.at(My_WriteList_Struct->start_instance), sizeof(Str_schedual_time_point));
                        for (int j = 0;j<200;j++)
                        {
                            write_buffer[j] = htons(write_buffer[j]);
                        }
                        test_value1 = Write_Multi_org_short(g_tstat_id, write_buffer, BAC_WR_TIME_FIRST + 72 * My_WriteList_Struct->start_instance, 72, 4); //Variable 是39个字节，占用20个寄存器;

                        _MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
                        pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
                        pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
                        pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
                        pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
                        if (test_value1 > 0)
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)pMy_Invoke_id);
                        }
                        else
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)pMy_Invoke_id);
                        }
                    }
                    break;
                    case WRITE_SETTING_COMMAND:
                    {
                        test_value1 = Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0);

                        _MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
                        pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
                        pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
                        pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
                        pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
                        if (test_value1 > 0)
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)pMy_Invoke_id);
                        }
                        else
                        {
                            ::PostMessage(pMy_Invoke_id->hwnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)pMy_Invoke_id);
                        }
                    }
                        break;
					default:
						break;
					}	
					if(test_value1 > 0)
					{
						
					}
				}
				break;
            case MY_BAC_WRITE_LIST:
            {
                MyCriticalSection.Lock();
                msg=My_Receive_msg.at(0);
                _MessageWriteListInfo *My_WriteList_Struct = (_MessageWriteListInfo *)msg.wParam;
                My_Receive_msg.erase(My_Receive_msg.begin());
                MyCriticalSection.Unlock();
                do
                {
                    if(My_WriteList_Struct->end_instance<=(My_WriteList_Struct->start_instance+4))
                    {
                        temp_end_value  =  My_WriteList_Struct->end_instance;
                    }
                    else
                        temp_end_value = My_WriteList_Struct->start_instance + 4;

                    resend_count = 0;



                    int ret_cusunits = WritePrivateData_Blocking(My_WriteList_Struct->deviceid, My_WriteList_Struct->command, My_WriteList_Struct->start_instance, temp_end_value);
                    _MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
                    pMy_Invoke_id->Invoke_ID = ret_cusunits;
                    pMy_Invoke_id->hwnd = My_WriteList_Struct->hWnd;
                    pMy_Invoke_id->task_info = My_WriteList_Struct->Write_Info;
                    pMy_Invoke_id->mRow = My_WriteList_Struct->ItemInfo.nRow;
                    pMy_Invoke_id->mCol = My_WriteList_Struct->ItemInfo.nCol;
                    if (ret_cusunits)
                    {
                        ::PostMessage(MainFram_hwd, MY_RX_TX_COUNT, 1, 0);
                        ::PostMessage(My_WriteList_Struct->hWnd, MY_RESUME_DATA, (WPARAM)WRITE_SUCCESS, (LPARAM)pMy_Invoke_id);
                    }
                    else
                    {
                        ::PostMessage(MainFram_hwd, MY_RX_TX_COUNT, 0, 0);
                        ::PostMessage(My_WriteList_Struct->hWnd, MY_RESUME_DATA, (WPARAM)WRITE_FAIL, (LPARAM)pMy_Invoke_id);
                    }
#if 0
                    do
                    {
                        resend_count ++;
                        if(resend_count>10)
                            break;
                        g_invoke_id =WritePrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,My_WriteList_Struct->start_instance,temp_end_value);
                        if(g_invoke_id>=0)
                        {
                            Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,My_WriteList_Struct->hWnd,My_WriteList_Struct->Write_Info,My_WriteList_Struct->ItemInfo.nRow,My_WriteList_Struct->ItemInfo.nCol);
                        }
                        Sleep(SEND_COMMAND_DELAY_TIME);
                    }
                    while (g_invoke_id<0);
#endif
                    My_WriteList_Struct->start_instance = temp_end_value + 1;
                }
                while (temp_end_value<My_WriteList_Struct->end_instance);
            }
            break;
            case MY_BAC_REFRESH_LIST:
            {
                MyCriticalSection.Lock();
                msg=My_Receive_msg.at(0);
                _MessageRefreshListInfo *My_WriteList_Struct = (_MessageRefreshListInfo *)msg.wParam;
                My_Receive_msg.erase(My_Receive_msg.begin());
                MyCriticalSection.Unlock();
                if (n_wifi_connection)
                    break;
                for (int i=0; i<My_WriteList_Struct->block_size; i++)
                {
                    resend_count = 0;
                    do
                    {
                        resend_count ++;
                        if(resend_count>10)
                            break;
                        if((unsigned char)My_WriteList_Struct->command == READALARM_T3000)//Because the Alarm list need to read one by one
                        {
                            int end_temp_instance = 0;
                            end_temp_instance = BAC_READ_ALARMLOG_REMAINDER + (BAC_READ_ALARMLOG_GROUP_NUMBER*i) ;
                            if(end_temp_instance >= BAC_ALARMLOG_COUNT)
                                end_temp_instance = BAC_ALARMLOG_COUNT - 1;

                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_ALARMLOG_GROUP_NUMBER)*i,end_temp_instance,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READ_REMOTE_POINT)
                        {
                            int end_temp_instance = 0;
                            end_temp_instance = BAC_REMOTE_POINT_GROUP_REMAINDER + (BAC_REMOTE_POINT_GROUP_NUMBER*i) ;
                            if(end_temp_instance >= BAC_REMOTE_POINT_COUNT)
                                end_temp_instance = BAC_REMOTE_POINT_COUNT - 1;

                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_REMOTE_POINT_GROUP_NUMBER)*i,end_temp_instance,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READINPUT_T3000)
                        {
                            int end_temp_instance = 0;
                            end_temp_instance = BAC_READ_INPUT_REMAINDER + (BAC_READ_INPUT_GROUP_NUMBER*i) ;
                            if(end_temp_instance >= BAC_INPUT_ITEM_COUNT)
                                end_temp_instance = BAC_INPUT_ITEM_COUNT - 1;
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READOUTPUT_T3000)
                        {
                            int end_temp_instance = 0;
                            end_temp_instance = BAC_READ_OUTPUT_REMAINDER + (BAC_READ_OUTPUT_GROUP_NUMBER*i) ;
                            if(end_temp_instance >= BAC_OUTPUT_ITEM_COUNT)
                                end_temp_instance = BAC_OUTPUT_ITEM_COUNT - 1;
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_OUTPUT_GROUP_NUMBER)*i,end_temp_instance,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READVARIABLE_T3000)
                        {
                            int end_temp_instance = 0;
                            end_temp_instance = BAC_READ_VARIABLE_REMAINDER + (BAC_READ_VARIABLE_GROUP_NUMBER*i) ;
                            if(end_temp_instance >= BAC_VARIABLE_ITEM_COUNT)
                                end_temp_instance = BAC_VARIABLE_ITEM_COUNT - 1;
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READWEEKLYROUTINE_T3000)
                        {
                            int end_temp_instance = 0;
                            end_temp_instance = BAC_READ_SCHEDULE_REMAINDER + (BAC_READ_SCHEDULE_GROUP_NUMBER*i) ;
                            if(end_temp_instance >= BAC_SCHEDULE_COUNT)
                                end_temp_instance = BAC_SCHEDULE_COUNT - 1;
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,end_temp_instance,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READ_MISC)
                        {
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,0,0,My_WriteList_Struct->entitysize);
                        }
                        else if((unsigned char)My_WriteList_Struct->command == READANALOG_CUS_TABLE_T3000)
                        {
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,My_WriteList_Struct->start_instance,My_WriteList_Struct->end_instance,My_WriteList_Struct->entitysize);
                        }
                        else
                            g_invoke_id = GetPrivateData(My_WriteList_Struct->deviceid,My_WriteList_Struct->command,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,My_WriteList_Struct->entitysize);
                        Sleep(SEND_COMMAND_DELAY_TIME * 3);
                    }
                    while (g_invoke_id<0);

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
                }
                while (g_invoke_id<0);

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
    
    CIONameConfig ionameconfig;
    ionameconfig.DoModal();
}


void CMainFrame::OnDatabaseMbpoll()
{
    if (is_connect())
    {
        if (g_CommunicationType==0)
        {
            OnDisconnect();
        }
        CString strHistotyFile=g_strExePth+_T("ModbusPoll.exe");
        ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
    }
    else
    {
        AfxMessageBox(_T("Please config the way of your connection!"));
        OnAddBuildingConfig();
        OnConnect();
    }
}


void CMainFrame::OnToolIsptoolforone()
{



    b_pause_refresh_tree = ISPTOOLFORONE;
    bool temp_status = g_bPauseMultiRead;
    g_bPauseMultiRead = true;
    int temp_type = GetCommunicationType();


    BOOL bDontLinger = FALSE;
    setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
    closesocket(h_Broad);
    SetCommunicationType(0);//关闭串口，供ISP 使用;
    close_com();

    CString ApplicationFolder;
    CString AutoFlashConfigPath;

    CString ISPtool_path;

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

    h_bcast.sin_family=AF_INET;
    h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
    h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

    h_siBind.sin_family=AF_INET;
    h_siBind.sin_addr.s_addr=INADDR_ANY;
    h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
    ::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
    b_pause_refresh_tree = false;
    g_bPauseMultiRead = temp_status;

    return;
}






void CMainFrame::OnControlMain()
{
    CString temp_ui;
    temp_ui.Format(_T("%u"), TYPE_MAIN);
    WritePrivateProfileString(_T("LastView"), _T("FistLevelViewUI"), temp_ui, g_cstring_ini_path);

    if((g_protocol == PROTOCOL_BACNET_IP) || 
        (g_protocol == PROTOCOL_BIP_TO_MSTP) || 
        (g_protocol == MODBUS_BACNET_MSTP) ||
        (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        OnControlSettings();
    }
    else
    {
		HideBacnetWindow();
        if (product_type == T3000_6_ADDRESS)
        {
            //bacnet_view_number = TYPE_TSTAT_MAIN_INFOR;
            bacnet_view_number = TYPE_MAIN;
            SwitchToPruductType(DLG_T3000_VIEW);
        }
        else if (product_type == CS3000||product_register_value[7]==PM_T322AI || product_register_value[7] == PWM_TRANSDUCER ||product_register_value[7]==PM_T38AI8AO6DO || product_register_value[7]==PM_T3PT12
		|| product_register_value[7]==PM_T36CTA||product_register_value[7] == PM_T3_LC)
        {
            bacnet_view_number = TYPE_MAIN;
			global_interface = BAC_MAIN;
            SwitchToPruductType(DLG_DIALOG_DEFAULT_T3000_VIEW);
        }
        else if (product_register_value[7] == PM_T3IOA)
        {

            SwitchToPruductType(DLG_DIALOGT38AI8AO);
        }

        else if (product_register_value[7]== PM_T38I13O)
        {

            SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);
        }
        else if (product_register_value[7] == PM_T34AO) //T3
        {
            SwitchToPruductType(DLG_DIALOGT3_VIEW);
        }
        else if (product_register_value[7]== PM_T3PT10)
        {
            SwitchToPruductType(DLG_DIALOGT3PT10);
        }
        else if (product_register_value[7]==PM_T332AI)
        {
            SwitchToPruductType(DLG_DIALOGT332AI_VIEW);
        }
        else if (product_register_value[7]==PM_T36CT)
        {
            SwitchToPruductType(DLG_DIALOGT36CT);
        }
        else if (product_register_value[7]==PM_T38I13O)
        {
            SwitchToPruductType(DLG_DIALOGT38I13O_VIEW);
        }
        else if (product_register_value[7]==PM_T332AI)
        {
            SwitchToPruductType(DLG_DIALOGT332AI_VIEW);
        }
        else if (product_register_value[7]==PM_T3IOA)
        {
            SwitchToPruductType(DLG_DIALOGT38AI8AO);
        }
		else if(product_register_value[7] == PM_CO2_RS485||product_register_value[7] == PM_PRESSURE_SENSOR||product_register_value[7] == PM_CO2_NODE || product_register_value[7] == STM32_CO2_NODE)//(nFlag == PM_CO2_NET)||
		{
            if (product_register_value[7] == STM32_CO2_NODE)
            {
                SwitchToPruductType(DLG_DIALOG_CO2_NODE);
            }
			else if (product_register_value[14] == 6)
			{
				SwitchToPruductType(DLG_AIRQUALITY_VIEW);
			} 
			else
			{
				SwitchToPruductType(DLG_CO2_VIEW);
			}

		}
		else if(product_register_value[7] == PM_CO2_NET||product_register_value[7] == STM32_CO2_RS485|| product_register_value[7] == STM32_CO2_NET)//(nFlag == PM_CO2_NET)||
		{
			SwitchToPruductType(DLG_CO2_NET_VIEW);
		}
		else if ((product_register_value[7]==PM_CO2_RS485&&product_register_value[14] == 6)
			||product_register_value[7]==PM_AirQuality||
			product_register_value[7]==PM_HUM_R||
			product_register_value[7]==PM_HUMTEMPSENSOR||
			product_register_value[7]==STM32_HUM_NET||
			product_register_value[7] == STM32_PRESSURE_NET ||
			product_register_value[7]==STM32_HUM_RS485)
		{ 
			SwitchToPruductType(DLG_AIRQUALITY_VIEW);
		} 
        else if ((product_register_value[7] == PM_TSTAT_AQ) ||
                 (product_register_value[7] == PM_MULTI_SENSOR))
        {
            SwitchToPruductType(DLG_DIALOG_TSTAT_AQ);
        }
        else if ((product_register_value[7] == PWM_TEMPERATURE_TRANSDUCER) ||
            (product_register_value[7] == STM32_PM25))
        {
            OnToolRegisterviewer();
        }
    }
     bacnet_view_number = TYPE_MAIN		;
 	 global_interface = BAC_MAIN;
}

void CMainFrame::OnControlInputs()
{

#if 0
//    m_testtoolbar.RemoveAllButtons();
   // CMFCToolBar::ResetAllImages();

    CMFCPopupMenu::SetForceMenuFocus(FALSE);
    CMFCToolBar::SetSizes(CSize(32, 32), CSize(32, 32));
    CMFCToolBar::SetMenuSizes(CSize(32, 32), CSize(24, 24));

    UINT uiToolbarHotID;
    UINT uiToolbarColdID;
    UINT uiMenuID;

    uiToolbarHotID = IDB_BITMAP_TOOLBAR_DISABLE;
    uiToolbarColdID = IDB_BITMAP_TOOLBAR_DISABLE;
    uiMenuID = IDB_BITMAP_BACNET_MENU_BIT;//



    if (!m_testtoolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_TOOLBAR_BACNET) ||
        !m_testtoolbar.LoadToolBar(IDR_TOOLBAR_BACNET, uiToolbarColdID, uiMenuID, FALSE /* Not locked */, IDB_BITMAP_TOOLBAR_DISABLE, 0, uiToolbarHotID))

    {
        TRACE0("Failed to create toolbar\n");
        return ;//fail to create
    }


    m_wndToolBar.RedrawWindow();

    return;
#endif
    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;


    if (product_type == CS3000 || 
        product_register_value[7] == PM_TSTAT6 || 
        product_register_value[7] == PM_TSTAT5i || 
        product_register_value[7] == PM_TSTAT7 || 
        product_register_value[7] == PM_TSTAT8 || 
        product_register_value[7] == PM_PM5E_ARM ||
        product_register_value[7] == PM_TSTAT9 || 
        product_register_value[7] == PM_TSTAT8_WIFI || 
        product_register_value[7] == PM_TSTAT8_OCC || 
        product_register_value[7] == PM_TSTAT7_ARM || 
        product_register_value[7] == PM_TSTAT8_220V)
    {

        SwitchToPruductType(DLG_DIALOG_TSTAT_INPUT_VIEW);
        bacnet_view_number = TYPE_INPUT;
        global_interface = BAC_IN;
        return;
    }


    if( (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS) ||
        (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485) ||
        (g_protocol == PROTOCOL_MSTP_TO_MODBUS) ||
        (g_protocol == PROTOCOL_BACNET_IP) || 
		 (g_protocol == MODBUS_BACNET_MSTP) || 
		 (g_protocol == PROTOCOL_BIP_TO_MSTP)||
		 ((g_protocol == MODBUS_RS485 ) && 
		  (
             (product_type == PM_TSTAT10) ||
			 (product_type == PM_MINIPANEL)||
			  (product_type == PM_MINIPANEL_ARM) 
			  ||
			  ( ( (bacnet_device_type == T38AI8AO6DO) 
				  || (bacnet_device_type == PID_T322AI)
			      || (bacnet_device_type == PID_T3PT12) 
				  || (bacnet_device_type == PM_T3_LC)
                  || (bacnet_device_type == PM_TSTAT_AQ)
                  || (bacnet_device_type == PM_T36CTA)
				  || (bacnet_device_type == PWM_TRANSDUCER)) 
                  || (product_type == PM_MULTI_SENSOR)
                  || (product_type == PM_TSTAT_AQ)
                  || (product_type == STM32_CO2_NET)
                  || (product_type == STM32_HUM_NET)
                  || (product_type == STM32_PRESSURE_NET)
				/*&& new_device_support_mini_ui*/ ) ) ) ||
		
		   ((g_protocol == MODBUS_TCPIP ) && 
		( ( (bacnet_device_type == T38AI8AO6DO) 
            || (bacnet_device_type == PID_T322AI) 
			|| (bacnet_device_type == PID_T3PT12) 
			|| (bacnet_device_type == PM_T3_LC)
            || (bacnet_device_type == PM_TSTAT_AQ)
			|| (bacnet_device_type == PID_T36CTA)
            || (product_type == STM32_CO2_NET)
            || (product_type == PWM_TRANSDUCER)
            || (product_type == STM32_HUM_NET)
            || (product_type == STM32_PRESSURE_NET)
			) /*&& new_device_support_mini_ui*/  )  ))
    {

		if(BacNet_hwd == NULL)
		{
			SwitchToPruductType(DLG_BACNET_VIEW);
		}
		else
		{
			CView* pNewView = m_pViews[DLG_BACNET_VIEW];
			if (!pNewView)
				return;
			CView* pActiveView =GetActiveView();
			if ( !pActiveView )    // No currently active view
				return;

			if ( pNewView!= pActiveView )    // Already there
			{
				m_nCurView = DLG_BACNET_VIEW;    // Store the new current view's index


				UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
				::SetWindowLong(pActiveView->m_hWnd, GWL_ID,
				::GetWindowLong(pNewView->m_hWnd, GWL_ID));
				::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
				pActiveView->ShowWindow(SW_HIDE);
				pNewView->ShowWindow(SW_SHOW);
				SetActiveView(pNewView);
				RecalcLayout();
				pNewView->Invalidate();
			}
		}

        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }

        if(pDialog[WINDOW_INPUT] != NULL)
        {
            if(pDialog[WINDOW_INPUT]->IsWindowVisible() == false)
            {
				((CBacnetInput *) pDialog[WINDOW_INPUT])->Reset_Input_Rect();
                pDialog[WINDOW_INPUT]->ShowWindow(SW_SHOW);
            }
            //((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetFocus();
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_INPUT);
            Input_Window->m_input_list.SetFocus();
            bacnet_view_number = TYPE_INPUT;
            g_hwnd_now = m_input_dlg_hwnd;

			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_INPUT);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }

        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }


        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();

        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Input_Window->KillTimer(INPUT_REFRESH_DATA_TIMER);
            Input_Window->SetTimer(INPUT_REFRESH_DATA_TIMER, BAC_LIST_REFRESH_TIME, NULL); //点击按钮手动刷新后，开启计时避免频繁刷新;
            Create_Thread_Read_Item(TYPE_INPUT);
        }
		else if((g_protocol == MODBUS_RS485) || 
                (g_protocol == MODBUS_TCPIP) || 
                (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485) ||  //20200306 TSTAT10或者T3BB  使用MODBUS MODBUS485 接到  T3BB下面 
                (g_protocol == PROTOCOL_MSTP_TO_MODBUS) ||
                (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
		{
            if (product_type == STM32_CO2_NET)
                bacnet_device_type = product_type;
			hide_485_progress = false;
			::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, bacnet_device_type, READINPUT_T3000 /*BAC_IN*/);//第二个参数 In
			::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
            Input_Window->KillTimer(INPUT_REFRESH_DATA_TIMER);
            Input_Window->SetTimer(INPUT_REFRESH_DATA_TIMER, BAC_LIST_REFRESH_TIME, NULL); //点击按钮手动刷新后，开启计时避免频繁刷新;
		}
        else
        {
            Input_Window->KillTimer(INPUT_REFRESH_DATA_TIMER);
            if(n_wifi_connection)
                Input_Window->SetTimer(INPUT_REFRESH_DATA_TIMER, BAC_LIST_REFRESH_TIME, NULL);
            else
                Input_Window->SetTimer(INPUT_REFRESH_DATA_TIMER, BAC_LIST_REFRESH_ETHERNET_TIME, NULL);

            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
        }
    }
    else
    {
        if (!is_connect())
        {
            AfxMessageBox(_T("NO Connection,Please connect your device frist!"));
            return;
        }
   //     if (product_type==CS3000||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT8
			//|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V))
   //     {

   //         SwitchToPruductType(DLG_DIALOG_TSTAT_INPUT_VIEW);
   //     }
        else if ((product_type == T3000_T3_MODULES ) && (product_register_value[7] != PM_PWMETER))
        {
            SwitchToPruductType(DLG_DIALOG_T3_INPUTS_VIEW);
        }
        else
        {
           MessageBox(_T("This device doesn't have a input grid display"));
        }

    }
    bacnet_view_number = TYPE_INPUT;
		global_interface = BAC_IN;
}

void CMainFrame::OnControlPanel()
{
    //bacnet_view_number = TYPE_PANEL;
    //HideBacnetWindow();
}




void CMainFrame::OnControlPrograms()
{
#if 0
    BACNET_APPLICATION_DATA_VALUE temp_value;
    str_bacnet_rp_info temp_test;
    int invoke_id = Bacnet_Read_Properties_Blocking(g_bac_instance, OBJECT_ANALOG_INPUT, 1, PROP_PRESENT_VALUE, temp_value,3);

    temp_value.tag = TPYE_BACAPP_REAL;
    temp_value.context_specific = false;
    temp_value.type.Real = 1111;
   // strcpy(test123.type.Character_String.value, "1123");

     invoke_id = Bacnet_Write_Properties_Blocking(55555, OBJECT_ANALOG_INPUT, 1, PROP_PRESENT_VALUE, &temp_value);
  Sleep(1);
#endif // DEBUG


    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;
    if((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP))
    {
        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }

        if(pDialog[WINDOW_PROGRAM] != NULL)
        {
            Program_Window->Unreg_Hotkey();
            Screen_Window->Unreg_Hotkey();
            WeeklyRoutine_Window->Unreg_Hotkey();
            AnnualRoutine_Window->Unreg_Hotkey();
            Monitor_Window->Unreg_Hotkey();
            if(pDialog[WINDOW_PROGRAM]->IsWindowVisible() == false)
            {
				Program_Window->Reset_Program_Rect();
                pDialog[WINDOW_PROGRAM]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_PROGRAM);
            Program_Window->m_program_list.SetFocus();
            Program_Window->Reg_Hotkey();
            bacnet_view_number = TYPE_PROGRAM;
            g_hwnd_now = m_pragram_dlg_hwnd;

			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_PROGRAM);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }


        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_PROGRAM);
        }
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAM);
        }
    }
    else if ((g_protocol == MODBUS_RS485) || //RS485 下面挂T3 MINIPANEL
            (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485))   //BB网络下面挂 MODBUS485  的   TSTAT10或 BB
    {
        if ((product_type == PM_MINIPANEL) ||
            (product_type == PM_TSTAT10) ||
            (product_type == PM_MINIPANEL_ARM))
        {
            if (BacNet_hwd == NULL)
            {
                SwitchToPruductType(DLG_BACNET_VIEW);
            }
            ::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, PM_MINIPANEL_ARM, READPROGRAM_T3000/*BAC_PRG*/);//第二个参数 In
            bacnet_view_number = TYPE_PROGRAM;
            global_interface = BAC_PRG;

            if (pDialog[WINDOW_PROGRAM]->IsWindowVisible() == false)
            {
                Program_Window->Reset_Program_Rect();
                pDialog[WINDOW_PROGRAM]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_PROGRAM);
            Program_Window->m_program_list.SetFocus();

            CString temp_ui;
            temp_ui.Format(_T("%u"), TYPE_PROGRAM);
            WritePrivateProfileString(_T("LastView"), _T("FistLevelViewUI"), temp_ui, g_cstring_ini_path);
        }
    }
    else
    {
       MessageBox(_T("This device doesn't have a programs grid display"));
    }
}


void CMainFrame::OnControlOutputs()
{
//#ifdef DEBUG
//    ShutDownMstpGlobal(5);
//    return;
//#endif // DEBUG


    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;

    if (product_type == T3000_6_ADDRESS || 
        product_register_value[7] == PM_PM5E_ARM ||
        product_register_value[7] == PM_CS_RSM_AC || 
        product_register_value[7] == PM_CS_RSM_DC)
    {
        SwitchToPruductType(DLG_DIALOG_TSTAT_OUTPUT_VIEW);
        bacnet_view_number = TYPE_OUTPUT;
        global_interface = BAC_OUT;
        return;
    }

	// new_device_support_mini_ui  主要是为了支持 旧版本的T3进入以前的界面;
    if ((g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS) ||
        (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485) ||
          (g_protocol == PROTOCOL_MSTP_TO_MODBUS) ||
          (g_protocol == PROTOCOL_BACNET_IP) || 
		 (g_protocol == MODBUS_BACNET_MSTP) || 
		 (g_protocol == PROTOCOL_BIP_TO_MSTP) || 
		  
			 (g_protocol == MODBUS_RS485 ) &&
		    (
				((product_type == PM_TSTAT10) || product_type == PM_MINIPANEL|| product_type == PM_MINIPANEL_ARM)
				|| (( ( (bacnet_device_type == T38AI8AO6DO)
				|| (bacnet_device_type == PID_T322AI) 
				|| (bacnet_device_type == PM_T3_LC)
                || (bacnet_device_type == PM_T36CTA)
				|| (bacnet_device_type == PWM_TRANSDUCER) 
                || (product_type == STM32_CO2_NET)
                || (product_type == STM32_HUM_NET)
                || (product_type == STM32_PRESSURE_NET)
				|| (bacnet_device_type == PID_T3PT12)  )  
			    /*&& new_device_support_mini_ui*/ ) ) 
		    ) 
			 ||
		 
		   ((g_protocol == MODBUS_TCPIP ) && 
		  ( (bacnet_device_type == T38AI8AO6DO) || 
			(bacnet_device_type == PID_T322AI) || 
			(bacnet_device_type == PID_T3PT12) || 
			(bacnet_device_type == PM_T3_LC)||
            (product_type == STM32_CO2_NET) ||
              (product_type == STM32_HUM_NET) ||
              (product_type == PWM_TRANSDUCER) ||
              (product_type == STM32_PRESSURE_NET) ||
			(bacnet_device_type == PID_T36CTA)
		  ) /*&& new_device_support_mini_ui */
		  )
	 )
	 
	{

		if(BacNet_hwd == NULL)
		{
			SwitchToPruductType(DLG_BACNET_VIEW);
		}
		else
		{
			CView* pNewView = m_pViews[DLG_BACNET_VIEW];
			if (!pNewView)
				return;
			CView* pActiveView =GetActiveView();
			if ( !pActiveView )    // No currently active view
				return;

			if ( pNewView!= pActiveView )    // Already there
			{
				m_nCurView = DLG_BACNET_VIEW;    // Store the new current view's index
				UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
				::SetWindowLong(pActiveView->m_hWnd, GWL_ID,
					::GetWindowLong(pNewView->m_hWnd, GWL_ID));
				::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
				pActiveView->ShowWindow(SW_HIDE);
				pNewView->ShowWindow(SW_SHOW);
				SetActiveView(pNewView);
				RecalcLayout();
				pNewView->Invalidate();
			}
		}
   
        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }

        if(pDialog[WINDOW_OUTPUT] != NULL)
        {
            if(pDialog[WINDOW_OUTPUT]->IsWindowVisible() == false)
            {
				Output_Window->Reset_Output_Rect();
                pDialog[WINDOW_OUTPUT]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_OUTPUT);
            Output_Window->m_output_list.SetFocus();
            bacnet_view_number = TYPE_OUTPUT;
            g_hwnd_now = m_output_dlg_hwnd;

			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_OUTPUT);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }

        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();
        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_OUTPUT);
        }
		else if((g_protocol == MODBUS_RS485) || 
                (g_protocol == MODBUS_TCPIP) || 
                (g_protocol == PROTOCOL_MSTP_TO_MODBUS) || 
                (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485) ||
                (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
		{
			::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			::PostMessage(BacNet_hwd,WM_RS485_MESSAGE,bacnet_device_type, READOUTPUT_T3000/*BAC_OUT*/);//第二个参数 OUT
		}
        else
        {
            if((bac_select_device_online) || offline_mode)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
        }

    }
    else
    {
        if ((product_type == T3000_T3_MODULES) && (product_register_value[7] != PM_PWMETER))
        {
            SwitchToPruductType(DLG_DIALOG_T3_OUTPUTS_VIEW);
            bacnet_view_number = TYPE_OUTPUT;
        }
        else
        {
			MessageBox(_T("This device doesn't have a output grid display"));
        }
    }
		global_interface = BAC_OUT	;
}


void CMainFrame::OnControlVariables()
{
    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;
    if((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP))
    {
        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }

        if(pDialog[WINDOW_VARIABLE] != NULL)
        {
            if(pDialog[WINDOW_VARIABLE]->IsWindowVisible() == false)
            {
				Variable_Window->Reset_Variable_Rect();
                pDialog[WINDOW_VARIABLE]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_VARIABLE);
            Variable_Window->m_variable_list.SetFocus();
            bacnet_view_number = TYPE_VARIABLE;
            g_hwnd_now = m_variable_dlg_hwnd;


			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_VARIABLE);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }

        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();

        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_VARIABLE);
        }
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_VARIABLE);
        }
    }
    else if ((g_protocol == MODBUS_RS485) ||  //RS485 下面挂T3 MINIPANEL
             (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485))   //BB网络下面挂 MODBUS485  的   TSTAT10或 BB
    {
        if ((product_type == PM_MINIPANEL) ||
            (product_type == PM_TSTAT10) ||
            (product_type == PM_MINIPANEL_ARM))
        {
            if (BacNet_hwd == NULL)
            {
                SwitchToPruductType(DLG_BACNET_VIEW);
            }
            ::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, PM_MINIPANEL_ARM, READVARIABLE_T3000/*BAC_VAR*/);//第二个参数 In
            bacnet_view_number = TYPE_VARIABLE;
            global_interface = BAC_VAR;

            if (pDialog[WINDOW_VARIABLE]->IsWindowVisible() == false)
            {
                Variable_Window->Reset_Variable_Rect();
                pDialog[WINDOW_VARIABLE]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_VARIABLE);
            Variable_Window->m_variable_list.SetFocus();

            CString temp_ui;
            temp_ui.Format(_T("%u"), TYPE_VARIABLE);
            WritePrivateProfileString(_T("LastView"), _T("FistLevelViewUI"), temp_ui, g_cstring_ini_path);
        }
    }
    else
    {
       MessageBox(_T("This device doesn't have a variable grid display"));
    }
}


void CMainFrame::OnControlWeekly()
{
    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;
    if((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP))
    {
        if((m_user_level !=	LOGIN_SUCCESS_ROUTINE_MODE) &&
                (m_user_level != LOGIN_SUCCESS_FULL_ACCESS) &&
                (m_user_level != LOGIN_SUCCESS_READ_ONLY))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        if(pDialog[WINDOW_WEEKLY] != NULL)
        {
            Program_Window->Unreg_Hotkey();
            Screen_Window->Unreg_Hotkey();
            WeeklyRoutine_Window->Unreg_Hotkey();
            AnnualRoutine_Window->Unreg_Hotkey();
            Monitor_Window->Unreg_Hotkey();
            if(pDialog[WINDOW_WEEKLY]->IsWindowVisible() == false)
            {
				WeeklyRoutine_Window->Reset_Weekly_Rect();
                pDialog[WINDOW_WEEKLY]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
            WeeklyRoutine_Window->m_weeklyr_list.SetFocus();
            WeeklyRoutine_Window->Reg_Hotkey();
            bacnet_view_number = TYPE_WEEKLY;
            g_hwnd_now = m_weekly_dlg_hwnd;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_WEEKLY);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }

        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_WEEKLY);
        }
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
        }


    }
    else if ((g_protocol == MODBUS_RS485) ||  //RS485 下面挂T3 MINIPANEL
        (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485))   //BB网络下面挂 MODBUS485  的   TSTAT10或 BB
    {
        if(Bacnet_Private_Device(product_type))
        {
            if (BacNet_hwd == NULL)
            {
                SwitchToPruductType(DLG_BACNET_VIEW);
            }
            ::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, PM_MINIPANEL_ARM, READWEEKLYROUTINE_T3000/*BAC_SCH*/);//第二个参数 In
            bacnet_view_number = TYPE_WEEKLY;
            global_interface = BAC_SCH;

            if (pDialog[WINDOW_WEEKLY]->IsWindowVisible() == false)
            {
                WeeklyRoutine_Window->Reset_Weekly_Rect();
                pDialog[WINDOW_WEEKLY]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
            WeeklyRoutine_Window->m_weeklyr_list.SetFocus();

            CString temp_ui;
            temp_ui.Format(_T("%u"), TYPE_WEEKLY);
            WritePrivateProfileString(_T("LastView"), _T("FistLevelViewUI"), temp_ui, g_cstring_ini_path);
        }
    }
    else
    {
		int nFlag = product_register_value[7];
		if (product_register_value[7] == PM_TSTAT8 || product_register_value[7] == PM_TSTAT9 || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
		{
			g_bPauseMultiRead = TRUE;
			CNewTstatSchedulesDlg dlg;
			dlg.DoModal();
			g_bPauseMultiRead = FALSE;
		}
		else if(product_register_value[7] == PM_TSTAT5i || product_register_value[7] == PM_TSTAT6 || product_register_value[7] == PM_TSTAT7)
		{
			g_bPauseMultiRead = TRUE;
			   CTStatScheduleDlg dlg;
			dlg.DoModal(); 
			g_bPauseMultiRead = FALSE;
		}
		else
			MessageBox(_T("The device doesn't have a schedule grid display."));
    }
}



void CMainFrame::OnControlAnnualroutines()
{ 
    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;
    if((g_protocol == PROTOCOL_BACNET_IP) || 
        (g_protocol == MODBUS_BACNET_MSTP) || 
        (g_protocol == PROTOCOL_BIP_TO_MSTP) ||
        (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        if((m_user_level !=	LOGIN_SUCCESS_ROUTINE_MODE) &&
                (m_user_level != LOGIN_SUCCESS_FULL_ACCESS) &&
                (m_user_level != LOGIN_SUCCESS_READ_ONLY)
            )
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        if(pDialog[WINDOW_ANNUAL] != NULL)
        {
            Program_Window->Unreg_Hotkey();
            Screen_Window->Unreg_Hotkey();
            WeeklyRoutine_Window->Unreg_Hotkey();
            AnnualRoutine_Window->Unreg_Hotkey();
            Monitor_Window->Unreg_Hotkey();
            if(pDialog[WINDOW_ANNUAL]->IsWindowVisible() == false)
            {
				AnnualRoutine_Window->Reset_Annual_Rect();
                pDialog[WINDOW_ANNUAL]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_ANNUAL);
            AnnualRoutine_Window->m_annualr_list.SetFocus();
            AnnualRoutine_Window->Reg_Hotkey();
            bacnet_view_number = TYPE_ANNUAL;
            g_hwnd_now = m_annual_dlg_hwnd;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_ANNUAL);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }

        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_ANNUAL);
        }
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUAL);
        }
    }
    else
    {
        if(product_register_value[7]==PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            g_bPauseMultiRead = TRUE;
			AnnualRout_InsertDia dlg;
			dlg.DoModal();
            g_bPauseMultiRead = FALSE;
        }
		 
        else
			MessageBox(_T("This device doesn't have a holidays grid display"));
    }
}

void CMainFrame::OnControlSettings()
{
    CString temp_ui;
    temp_ui.Format(_T("%u"), TYPE_MAIN);
    WritePrivateProfileString(_T("LastView"), _T("FistLevelViewUI"), temp_ui, g_cstring_ini_path);

    g_llTxCount++; //其实毫无意义 ，毛非要不在线点击时 也要能看到TX ++ 了;
    if((g_protocol == PROTOCOL_BACNET_IP) || 
		(g_protocol == PROTOCOL_BIP_TO_MSTP) || 
		(g_protocol == MODBUS_BACNET_MSTP) ||
		(
			((g_protocol == MODBUS_RS485 )  || (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485))
            && 
            (product_type == PM_MINIPANEL  ||
		     product_type == PM_MINIPANEL_ARM  ||
             //product_type == PM_TSTAT_AQ ||
             product_type == PM_TSTAT10)
			
			) )
    {
        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }

        if(pDialog[WINDOW_SETTING] != NULL)
        {
            if(pDialog[WINDOW_SETTING]->IsWindowVisible() == false)
            {
				Setting_Window->Reset_Setting_Rect();
				Setting_Window->ShowWindow(SW_SHOW);
            }
			Setting_Window->SetFocus();
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_SETTING);
            bacnet_view_number = TYPE_SETTING;
            g_hwnd_now = m_setting_dlg_hwnd;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_SETTING);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();
        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_SETTING);
        }
		else if((g_protocol == MODBUS_RS485) || (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485))
        {
            if(g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER)
            {
                ::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_SETTING);
            }
            else
			::PostMessage(BacNet_hwd,WM_RS485_MESSAGE,0,READ_SETTING_COMMAND);//第二个参数 In
		}
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SETTING);
        }
    }
	
	else if (product_type == CS3000||
        product_register_value[7]==PM_T322AI || 
        product_register_value[7] == PWM_TRANSDUCER ||
        product_register_value[7]==PM_T38AI8AO6DO   ||
        product_register_value[7]==PM_T3PT12      ||
        product_register_value[7]==PM_T36CTA|| 
        product_register_value[7] == PM_T3_LC      || 
        product_register_value[7] == PM_TSTAT_AQ   ||
        product_register_value[7] == PM_MULTI_SENSOR ||
        product_register_value[7] == STM32_CO2_NET ||
        product_register_value[7] == STM32_PM25 ||
        product_register_value[7] == STM32_PRESSURE_NET ||
        product_register_value[7] == STM32_CO2_RS485 ||
        product_register_value[7] == STM32_HUM_RS485 ||
        product_register_value[7] == STM32_PRESSURE_RS3485 ||
        product_register_value[7]==  STM32_HUM_NET ||
        product_register_value[7] == STM32_CO2_NODE
        )
	{
		HideBacnetWindow();
		bacnet_view_number = TYPE_SETTING;
		SwitchToPruductType(DLG_DIALOG_DEFAULT_T3000_VIEW);
	}
    else
    {
        if(product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            CParameterDlg dlg(this,GetProductName(product_register_value[7]));
            dlg.DoModal();
        }
        else if (product_register_value[7] == PM_PWMETER)
        {
            //新增power meter 的弹出界面;
            CPowerMeterList dlg;
            dlg.DoModal();
        }
        else
           MessageBox(_T("This device doesn't have a configuration display"));
    }
}


void CMainFrame::OnMiscellaneousLoaddescriptors()
{
    
    if(g_protocol == PROTOCOL_BACNET_IP)
    {
        if(bac_select_device_online)
            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
        else
            MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
    }
    else
    {
		 MessageBox(_T("This device doesn't have a menu item"));
    }
}


void CMainFrame::OnMiscellaneousUpdatemini()
{
    

    if((g_protocol == PROTOCOL_BACNET_IP) ||
        (MODBUS_BACNET_MSTP == g_protocol))
    {
        if(bac_select_device_online)
            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,WRITEPRGFLASH_COMMAND);
        else
            MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);

    }
    else
    {
        MessageBox(_T("This device don't support this feature"));
    }
}

void CMainFrame::OnControlControllers()
{
    if((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP) ||
		(  ((g_protocol == MODBUS_TCPIP ) || (g_protocol == MODBUS_RS485)) &&   (bacnet_device_type == STM32_HUM_NET))  ||
        (  
           ((g_protocol == MODBUS_RS485) || (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485)) &&
            (
              (product_type == PM_MINIPANEL) || 
              (product_type == PM_TSTAT10) || 
              (product_type == PM_MINIPANEL_ARM) 
             )
            )
        )
    {
		if(BacNet_hwd == NULL)
		{
			SwitchToPruductType(DLG_BACNET_VIEW);
		}
		else
		{
			CView* pNewView = m_pViews[DLG_BACNET_VIEW];
			if (!pNewView)
				return;
			CView* pActiveView =GetActiveView();
			if ( !pActiveView )    // No currently active view
				return;

			if ( pNewView!= pActiveView )    // Already there
			{
				m_nCurView = DLG_BACNET_VIEW;    // Store the new current view's index
				UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
				::SetWindowLong(pActiveView->m_hWnd, GWL_ID,
					::GetWindowLong(pNewView->m_hWnd, GWL_ID));
				::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
				pActiveView->ShowWindow(SW_HIDE);
				pNewView->ShowWindow(SW_SHOW);
				SetActiveView(pNewView);
				RecalcLayout();
				pNewView->Invalidate();
			}
		}
        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }

        if(pDialog[WINDOW_CONTROLLER] != NULL)
        {
            if(pDialog[WINDOW_CONTROLLER]->IsWindowVisible() == false)
            {
				Controller_Window->Reset_Controller_Rect();
                pDialog[WINDOW_CONTROLLER]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_CONTROLLER);
            Controller_Window->m_controller_list.SetFocus();
            bacnet_view_number = TYPE_CONTROLLER;
            g_hwnd_now = m_controller_dlg_hwnd;

			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_CONTROLLER);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();

        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_CONTROLLER);
        }
		else if((g_protocol == MODBUS_RS485) || (g_protocol == MODBUS_TCPIP) || (g_protocol == PROTOCOL_MB_TCPIP_TO_MB_RS485))
		{
			hide_485_progress = false;
			::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
			::PostMessage(BacNet_hwd,WM_RS485_MESSAGE,bacnet_device_type, READCONTROLLER_T3000/*BAC_PID*/);//第二个参数 In
		}
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
        }
    }
    else
    {
       MessageBox(_T("This device doesn't have a pid  grid display"));
    }
}

void CMainFrame::OnControlScreens()
{
    if((g_protocol == PROTOCOL_BACNET_IP) || 
        (g_protocol == MODBUS_BACNET_MSTP) || 
        (g_protocol == PROTOCOL_BIP_TO_MSTP) ||
        (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        if((m_user_level !=	LOGIN_SUCCESS_GRAPHIC_MODE) &&
                (m_user_level != LOGIN_SUCCESS_FULL_ACCESS) &&
                (m_user_level != LOGIN_SUCCESS_READ_ONLY))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }

        if(pDialog[WINDOW_SCREEN] != NULL)
        {
            Program_Window->Unreg_Hotkey();
            Screen_Window->Unreg_Hotkey();
            WeeklyRoutine_Window->Unreg_Hotkey();
            AnnualRoutine_Window->Unreg_Hotkey();
            Monitor_Window->Unreg_Hotkey();
            if(pDialog[WINDOW_SCREEN]->IsWindowVisible() == false)
            {
				Screen_Window->Reset_Screen_Rect();
                pDialog[WINDOW_SCREEN]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
            Screen_Window->m_screen_list.SetFocus();
            Screen_Window->Reg_Hotkey();
            bacnet_view_number = TYPE_SCREENS;
            g_hwnd_now = m_screen_dlg_hwnd;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_SCREENS);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        if(g_protocol == PROTOCOL_BIP_TO_MSTP)
        {
            Create_Thread_Read_Item(TYPE_SCREENS);
        }
        else
        {
            if(bac_select_device_online)
                ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);
        }
    }
    else
    {
        if(product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            SwitchToGraphicView();
        }
        else
        {
            MessageBox(_T("This device doesn't have a graphic grid display"));
        }
        //
    }
}
void CMainFrame::OnHelpGetlanguageconfigfile()
{

}
void CMainFrame::OnLanguageEnglish()
{
    CString infor;
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
}


void CMainFrame::OnLanguage34006()
{

}


void CMainFrame::OnControlMonitors()
{
    

    if((g_protocol == PROTOCOL_BACNET_IP) ||
        (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        if((m_user_level ==	LOGIN_SUCCESS_GRAPHIC_MODE) ||
                (m_user_level == LOGIN_SUCCESS_ROUTINE_MODE))
        {
            MessageBox(_T("Please use the administrator to access data!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        if(pDialog[WINDOW_MONITOR] != NULL)
        {
            Program_Window->Unreg_Hotkey();
            Screen_Window->Unreg_Hotkey();
            WeeklyRoutine_Window->Unreg_Hotkey();
            AnnualRoutine_Window->Unreg_Hotkey();
            Monitor_Window->Unreg_Hotkey();
            if(pDialog[WINDOW_MONITOR]->IsWindowVisible() == false)
            {
				((CBacnetMonitor *)pDialog[WINDOW_MONITOR])->Reset_Monitor_Rect();
                pDialog[WINDOW_MONITOR]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_MONITOR);
            Monitor_Window->m_monitor_list.SetFocus();
            Monitor_Window->Reg_Hotkey();
            bacnet_view_number = TYPE_MONITOR;
            g_hwnd_now = m_monitor_dlg_hwnd;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_MONITOR);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }



        if(bac_select_device_online)
        {
            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_MONITOR);
            Post_Refresh_Message(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC),1);
        }
    }
    else
    {
        MessageBox(_T("This device doesn't have a trendlog  grid display"));
    }
}
void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
    CFrameWndEx::OnSizing(fwSide, pRect);
}

extern str_register_db_data * register_dbdata;
void CMainFrame::OnToolRegisterviewer()
{
	HideBacnetWindow();
    CBacnetRegisterListView ReglistViewDlg;
    ReglistViewDlg.DoModal();
    if (register_dbdata != NULL)
    {
        delete[] register_dbdata;
        register_dbdata = NULL;
    }
    //KillProcessFromName(_T("EXCEL.EXE"));
}

#include "CM5/mygdiplus.h"
void CMainFrame::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if(ScreenEdit_Window)
    {
        if(ScreenEdit_Window->IsWindowVisible())
            ::PostMessage(m_screenedit_dlg_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
    }
    if(User_Login_Window)
    {
        if(m_user_login_hwnd != NULL)
        {
            if(User_Login_Window->IsWindowVisible())
                ::PostMessage(m_user_login_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
        }

    }
    if(AlarmWindow_Window)
    {
		 CRect temprec;
		m_testtoolbar.GetWindowRect(&temprec);
		if(AlarmWindow_Window->IsWindowVisible())
			AlarmWindow_Window->SetWindowPos(&wndNoTopMost,temprec.left + 392,temprec.top + 1,32,temprec.Height() - 5,SWP_NOACTIVATE | SWP_SHOWWINDOW );
        ::PostMessage(m_alarmwindow_dlg_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
    }
    if(b_create_status)
    {
        CRect temprec;
        m_wndStatusBar.GetWindowRect(&temprec);
		 statusbar->SetWindowPos(&wndNoTopMost,temprec.left,temprec.top,temprec.Width(),temprec.Height(),SWP_NOACTIVATE | SWP_SHOWWINDOW );
    }
}

#include "TemcoStandardBacnetToolDlg.h"
CTemcoStandardBacnetToolDlg *BacnetTool_Window = NULL;;
void CMainFrame::OnDatabaseBacnettool()
{

//#ifdef DEBUG
    //close_bac_com();
    //CBacnetTool dlg;
    //dlg.DoModal();
    //return;
//#endif // DEBUG

    CString CS_BacnetExplore_Path;
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    CS_BacnetExplore_Path = ApplicationFolder + _T("\\BacnetExplore.exe");

    //ShellExecute(NULL, L"open", CS_BacnetExplore_Path, NULL, NULL, SW_SHOWNORMAL);

    HTREEITEM item = m_pTreeViewCrl->GetSelectedItem();
    CString selecteditemstr = (item != nullptr) ? m_pTreeViewCrl->GetItemText(item) : L"";
    CString selectednetwork = m_current_tree_node.NetworkCard_Address;
    ShellExecute(NULL, L"open", CS_BacnetExplore_Path, selecteditemstr + " " + selectednetwork, NULL, SW_SHOWNORMAL);
    return;
#if 0


    if (BacnetTool_Window != NULL)
    {
        delete BacnetTool_Window;
        BacnetTool_Window = NULL;
    }
    BacnetTool_Window = new CTemcoStandardBacnetToolDlg;
    BacnetTool_Window->Create(IDD_DIALOG_TEMCO_STANDARD_BACNET_TOOL, this);
    BacnetTool_Window->ShowWindow(SW_SHOW);
#endif
}

void CMainFrame::OnControlAlarmLog()
{
    
    if((g_protocol == PROTOCOL_BACNET_IP) ||
        (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        if(pDialog[WINDOW_ALARMLOG] != NULL)
        {
            if(pDialog[WINDOW_ALARMLOG]->IsWindowVisible() == false)
            {
				AlarmLog_Window->Reset_Alarm_Rect();
                pDialog[WINDOW_ALARMLOG]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(WINDOW_ALARMLOG);
            AlarmLog_Window->m_alarmlog_list.SetFocus();
            bacnet_view_number = TYPE_ALARMLOG;
            g_hwnd_now = m_alarmlog_dlg_hwnd;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_ALARMLOG);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }

        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();
        if(bac_select_device_online)
            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALARMLOG);
    }
    else
    {
       MessageBox(_T("This device doesn't have a alarms grid display"));
    }
}


void CMainFrame::OnControlCustomerunits()
{
    if(g_protocol == PROTOCOL_BACNET_IP)
    {
        if(bac_select_device_online)
            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_READ_CUSTOMER_UNIT);
        else
            MessageBox(_T("Device is Offline ,Please Check the connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
    }
    else
    {
       MessageBox(_T("This device doesn't have a custom units  grid display"));
    }
}


void CMainFrame::OnMenuCheckupdate()
{
    CT3000UpdateDlg dlg;
    dlg.DoModal();
}


void CMainFrame::CreateOfflinePrgFile()
{
	CString achive_file_path;
	CString temp_serial;
	CString offline_folder;
	temp_serial.Format(_T("%d.prog"),g_selected_serialnumber);
	achive_file_path = g_achive_folder + _T("\\") + temp_serial;
	offline_mode = true;

	char temp_buffer[200000];
	char * temp_point = NULL;
	memset(temp_buffer ,0,200000);
	offline_folder = g_strBuildingFolder + m_strCurMainBuildingName;
	CreateDirectory(offline_folder,NULL);//
	
	offline_prg_path = offline_folder + _T("\\Offline.prog");
	DWORD dwFileLen;
	HANDLE hFile;
	DWORD dWrites;
	CFileFind temp_file;
	if(temp_file.FindFile(achive_file_path))
	{
		return;
	}
	if(LoadBacnetBinaryFile(false,achive_file_path) < 0 )
	{
		dwFileLen = 200000;	
		hFile=CreateFile(offline_prg_path,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
		WriteFile(hFile,temp_buffer,dwFileLen,&dWrites,NULL);
		CloseHandle(hFile);
		SaveBacnetBinaryFile(offline_prg_path);
		return;
	}
	else
	{
		SaveBacnetBinaryFile(offline_prg_path);
	}
	Sleep(1);

}

void CMainFrame::OnControlRemotePoint()
{
    

    if((g_protocol == PROTOCOL_BACNET_IP) ||
        (g_protocol == MODBUS_BACNET_MSTP) ||
        (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        if(pDialog[WINDOW_REMOTE_POINT] != NULL)
        {
            if(pDialog[WINDOW_REMOTE_POINT]->IsWindowVisible() == false)
            {
				Remote_Point_Window->Reset_RemotePoint_Rect();
                pDialog[WINDOW_REMOTE_POINT]->ShowWindow(SW_SHOW);
            }
            ((CDialogCM5_BacNet*)m_pViews[DLG_BACNET_VIEW])->m_bac_main_tab.SetCurSel(12);	//第12个插入的是远端的点;
            Remote_Point_Window->m_remote_point_list.SetFocus();
            bacnet_view_number = TYPE_READ_REMOTE_POINT_INFO;
			CString temp_ui;
			temp_ui.Format(_T("%u"),TYPE_READ_REMOTE_POINT_INFO);		
			WritePrivateProfileString(_T("LastView"),_T("FistLevelViewUI"),temp_ui,g_cstring_ini_path);
        }
        if(ScreenEdit_Window)
        {
            ::SendMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
        }
        ((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
        ((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
        ((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
        ((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
        ((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();

        if(bac_select_device_online)
            ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_READ_REMOTE_POINT_INFO);
    }
    else
    {
		MessageBox(_T("This device doesn't have a remote point grid display"));
    }

    bacnet_view_number = TYPE_TSTAT ;
}


void CMainFrame::ShowDebugWindow()
{
    if(DebugWindow == NULL)
    {
        DebugWindow = new CDebugWindow;
        DebugWindow->Create(IDD_DIALOG_DEBUG_TRACE, this);
        DebugWindow->ShowWindow(SW_HIDE);
        g_Print = _T("Debug Time 16-12-05   Debug version 2.5");
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


//建立TCP 服务器, GSM 会连接上来;
void CMainFrame::OnMiscellaneousGsmconnection()
{
    if(Tcp_Server_Window == NULL)
    {
        Tcp_Server_Window = new CTCP_Server;
        Tcp_Server_Window->Create(IDD_GSM_SERVER, this);
        Tcp_Server_Window->ShowWindow(SW_SHOW);
    }
    else
    {
        Tcp_Server_Window->ShowWindow(SW_SHOW);
    }

    return;
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

//
void CMainFrame::BuildingComportConfig()
{
   
	CppSQLite3DB SqliteDBT3000;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
 

    bool n_building_port = false;
    if(m_subNetLst.size()<=0)
    {
        n_building_port = false;
    }
    else
    {
        CString comport = m_subNetLst.at(0).strComPort;
        if(comport.GetLength()<=3)
        {
            n_building_port = false;
        }
        else
        {
            if(comport.Left(3).CompareNoCase(_T("COM")) == 0)
            {
                CString temp_i_port;
                temp_i_port = comport.Right(comport.GetLength() - 3);
                m_building_com_port = _wtoi(temp_i_port);


                GetSerialComPortNumber1(m_vector_comport);
                if(m_vector_comport.size()>0)
                {
                    bool find_comport = false;
                    for (int j=0; j<m_vector_comport.size(); j++)
                    {
                        if(comport.CompareNoCase(m_vector_comport.at(j)) == 0)
                        {
                            open_com(m_building_com_port);
                            find_comport = true;
                            break;
                        }
                    }

                    if(find_comport)
                    {
                        SetLastOpenedComport(m_building_com_port);
                        open_com(m_building_com_port);
                    }
                    else	//如果在枚举的 串口里面没有找到就默认第一个;还要改数据库;
                    {
                        CString temp_port;
                        temp_port = m_vector_comport.at(0);
                        temp_port = temp_port.Right(temp_port.GetLength() - 3);
                        m_building_com_port = _wtoi(temp_port);
                        try
                        {
                            CString strSql;
                            strSql.Format(_T("update Building set Com_Port='%s' where Building_Name='%s' and Main_BuildingName='%s'"),m_vector_comport.at(0),m_strCurSubBuldingName,m_strCurMainBuildingName);
                            SqliteDBT3000.execDML((UTF8MBSTR)strSql);
                            m_subNetLst.at(0).strComPort = m_vector_comport.at(0);
                        }
                        catch (...)
                        {

                        }
                    }
                }
            }
        }
    }

    SqliteDBT3000.closedb();
}


BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
     
    int nSize = pCopyDataStruct->cbData;// = (strMsg.GetLength()+1)*sizeof(TCHAR); //(lstrlen(szBuffer)+1)*sizeof(TCHAR);
    //pCopyDataStruct->lpData =  (LPVOID)(LPCTSTR)strMsg;
    CString strMsg = CString(LPCTSTR(pCopyDataStruct->lpData));
    CString show_info;
    show_info.Format(_T("Ping - "));
    show_info = show_info + strMsg;
    SetPaneString(BAC_SHOW_MISSION_RESULTS,show_info);
    return CFrameWndEx::OnCopyData(pWnd, pCopyDataStruct);
}


void CMainFrame::OnUpdateDisconnect2(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_nStyle == 2);
}
void CMainFrame::OnUpdateScanDevice(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_nStyle == 3);
}
void CMainFrame::OnUpdateAddBuildingConfig(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_nStyle ==4);
}

void CMainFrame::OnUpdateStatusBar(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(TRUE);

}

void CMainFrame::OnHelpFeedbacktotemco()
{
    m_product_isp_auto_flash.baudrate = m_product.at(selected_product_index).baudrate;
    m_product_isp_auto_flash.BuildingInfo.strIp = m_product.at(selected_product_index).BuildingInfo.strIp;
    m_product_isp_auto_flash.ncomport =  m_product.at(selected_product_index).ncomport;

    m_product_isp_auto_flash.product_class_id =  m_product.at(selected_product_index).product_class_id;
    m_product_isp_auto_flash.product_id =  m_product.at(selected_product_index).product_id;


    Dowmloadfile Dlg;
    Dlg.DoModal();

}


CString CMainFrame::GetFWVersionFromFTP(CString ProductName)
{
    CString strHtml("");
    CInternetSession sess;
    CHttpFile* pHttpFile = NULL;
    CString ULR("http://temcocontrols.com/ftp/firmware/FV.txt");

    pHttpFile = (CHttpFile*)sess.OpenURL(ULR.GetBuffer());

    char sRecived[1024];
    if(pHttpFile)
    {
        while(pHttpFile->ReadString((LPTSTR)sRecived, 1024)) //解决Cstring乱码
        {
            CString temp(sRecived);
            /*	strHtml += temp;* /*/
            CStringArray  stringarray;
            SplitCStringA(stringarray,temp,_T("="));
            CString scourceProductName=stringarray[0];
            if (scourceProductName.CompareNoCase(ProductName.GetBuffer())==0)
            {
                strHtml=stringarray[1];
                break;
            }
        }
    }
    return strHtml;

}

//-----------------------------------------------------------------------------

LRESULT CMainFrame::OnThreadFinished(WPARAM wParam, LPARAM /*lParam*/)
{



    //If an error occured display the message box

    if (wParam)
    {
        m_bSafeToClose=TRUE;
        return 0L;
    }
    //下载完成之后，打开文件
    else
    {
        //EndDialog(IDOK);
        m_bSafeToClose=TRUE;

        //Free up the internet handles we may be using
        if (m_hHttpFile)
        {
            ::InternetCloseHandle(m_hHttpFile);
            m_hHttpFile = NULL;
        }
        if (m_hHttpConnection)
        {
            ::InternetCloseHandle(m_hHttpConnection);
            m_hHttpConnection = NULL;
        }
        if (m_hInternetSession)
        {
            ::InternetCloseHandle(m_hInternetSession);
            m_hInternetSession = NULL;

        }
        AfxMessageBox(_T("Finished"));
        return 1L;
    }
    return 0L;
}

 

BOOL CMainFrame::DownloadThread()
{
    //Create the Internet session handle
    ASSERT(m_hInternetSession == NULL);
    m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (m_hInternetSession == NULL)
    {
        TRACE(_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
        HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_GENERIC_ERROR);
        m_bSafeToClose=TRUE;
        return FALSE;
    }

    //Should we exit the thread


    //Setup the status callback function
    if (::InternetSetStatusCallback(m_hInternetSession, _OnStatusCallBack) == INTERNET_INVALID_STATUS_CALLBACK)
    {
        TRACE(_T("Failed in call to InternetSetStatusCallback, Error:%d\n"), ::GetLastError());
        HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_GENERIC_ERROR);
        m_bSafeToClose=TRUE;
        return FALSE;;
    }

    //Should we exit the thread


    //Make the connection to the HTTP server
    ASSERT(m_hHttpConnection == NULL);
    if (m_sUserName.GetLength())
        m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort, m_sUserName,
                                              m_sPassword, m_dwServiceType, 0, (DWORD) this);
    else
        m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort, NULL,
                                              NULL, m_dwServiceType, 0, (DWORD) this);
    if (m_hHttpConnection == NULL)
    {
        TRACE(_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
        HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
        m_bSafeToClose=TRUE;
        return FALSE;;
    }

    //Should we exit the thread


    //Start the animation to signify that the download is taking place


    //Issue the request to read the file
    LPCTSTR ppszAcceptTypes[2];
    ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
    ppszAcceptTypes[1] = NULL;
    ASSERT(m_hHttpFile == NULL);
    m_hHttpFile = HttpOpenRequest(m_hHttpConnection, NULL, m_sObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD |
                                  INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, (DWORD) this);
    if (m_hHttpFile == NULL)
    {
        TRACE(_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
        HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
        m_bSafeToClose=TRUE;
        return FALSE;
    }

    //Should we exit the thread


//label used to jump to if we need to resend the request
resend:

    //Issue the request
    BOOL bSend = ::HttpSendRequest(m_hHttpFile, NULL, 0, NULL, 0);
    if (!bSend)
    {
        TRACE(_T("Failed in call to HttpSendRequest, Error:%d\n"), ::GetLastError());
        HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
        m_bSafeToClose=TRUE;
        return FALSE;
    }

    //Check the HTTP status code
    TCHAR szStatusCode[32];
    DWORD dwInfoSize = 32;
    if (!HttpQueryInfo(m_hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
    {
        TRACE(_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
        HandleThreadError(IDS_HTTPDOWNLOAD_INVALID_SERVER_RESPONSE);
        m_bSafeToClose=TRUE;
        return FALSE;
    }
    else
    {
        long nStatusCode = _ttol(szStatusCode);

        //Handle any authentication errors
        if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
        {
            // We have to read all outstanding data on the Internet handle
            // before we can resubmit request. Just discard the data.
            char szData[51];
            DWORD dwSize;
            do
            {
                ::InternetReadFile(m_hHttpFile, (LPVOID)szData, 50, &dwSize);
            }
            while (dwSize != 0);

            //Bring up the standard authentication dialog
            if (::InternetErrorDlg(GetSafeHwnd(), m_hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
                                   FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
                goto resend;
        }
        else if (nStatusCode != HTTP_STATUS_OK)
        {
            TRACE(_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
            HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_INVALID_HTTP_RESPONSE, nStatusCode);
            m_bSafeToClose=TRUE;
            return FALSE;
        }
    }

    //Update the status control to reflect that we are getting the file information
    //SetStatus(IDS_HTTPDOWNLOAD_GETTING_FILE_INFORMATION);

    // Get the length of the file.
    TCHAR szContentLength[32];
    dwInfoSize = 32;
    DWORD dwFileSize = 0;
    BOOL bGotFileSize = FALSE;
    if (::HttpQueryInfo(m_hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
    {
        //Set the progress control range
        bGotFileSize = TRUE;
        dwFileSize = (DWORD) _ttol(szContentLength);
        //SetProgressRange(dwFileSize);
    }

    //Update the status to say that we are now downloading the file
    //SetStatus(IDS_HTTPDOWNLOAD_RETREIVEING_FILE);

    //Now do the actual read of the file
    DWORD dwStartTicks = ::GetTickCount();
    DWORD dwCurrentTicks = dwStartTicks;
    DWORD dwBytesRead = 0;
    char szReadBuf[1024];
    DWORD dwBytesToRead = 1024;
    DWORD dwTotalBytesRead = 0;
    DWORD dwLastTotalBytes = 0;
    DWORD dwLastPercentage = 0;
    do
    {
        if (!::InternetReadFile(m_hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead))
        {
            TRACE(_T("Failed in call to InternetReadFile, Error:%d\n"), ::GetLastError());
            HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_ERROR_READFILE);
            m_bSafeToClose=TRUE;
            return FALSE;
        }
        else if (dwBytesRead)
        {
            //Write the data to file
            TRY
            {
                m_FileToWrite.Write(szReadBuf, dwBytesRead);
            }
            CATCH(CFileException, e);
            {
                TRACE(_T("An exception occured while writing to the download file\n"));
                HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_ERROR_READFILE, e->m_lOsError);
                e->Delete();
                m_bSafeToClose=TRUE;
                return FALSE;
            }
            END_CATCH

            //Increment the total number of bytes read
            dwTotalBytesRead += dwBytesRead;


        }
    }
    while (dwBytesRead);

    //Delete the file being downloaded to if it is present and the download was aborted
    m_FileToWrite.Close();
    //We're finished
    m_bSafeToClose=TRUE;
    return TRUE;
// PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED);
}

#define CLOSE_HANDLE(handle) \
	do \
	 { \
	 CloseHandle(handle); \
	 handle = NULL; \
} while (FALSE)

 
void CMainFrame::HandleThreadErrorWithLastError(UINT nIDError, DWORD dwLastError)
{
    //Form the error string to report
    CString sError;
    if (dwLastError)
        sError.Format(_T("%d"), dwLastError);
    else
        sError.Format(_T("%d"), ::GetLastError());
    AfxFormatString1(m_sError, nIDError, sError);

    //Delete the file being downloaded to if it is present
    m_FileToWrite.Close();
    ::DeleteFile(m_sFileToDownloadInto);

    //PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED, 1);
}

void CMainFrame::HandleThreadError(UINT nIDError)
{
    m_sError.LoadString(nIDError);
    //PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED, 1);
}

void CALLBACK CMainFrame  ::_OnStatusCallBack(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus,
        LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
    //Convert from the SDK C world to the C++ world
    CMainFrame  * pDlg = (CMainFrame  *) dwContext;
    ASSERT(pDlg);
    ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CMainFrame  )));
    //pDlg->OnStatusCallBack(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

void CMainFrame  ::OnStatusCallBack(HINTERNET /*hInternet*/, DWORD dwInternetStatus,
                                    LPVOID lpvStatusInformation, DWORD /*dwStatusInformationLength*/)
{
    switch (dwInternetStatus)
    {
    case INTERNET_STATUS_RESOLVING_NAME:
    {
        //SetStatus(IDS_HTTPDOWNLOAD_RESOLVING_NAME, (LPCTSTR) lpvStatusInformation);
        break;
    }
    case INTERNET_STATUS_NAME_RESOLVED:
    {
        //SetStatus(IDS_HTTPDOWNLOAD_RESOLVED_NAME, (LPCTSTR) lpvStatusInformation);
        break;
    }
    case INTERNET_STATUS_CONNECTING_TO_SERVER:
    {
        //SetStatus(IDS_HTTPDOWNLOAD_CONNECTING, (LPCTSTR) lpvStatusInformation);
        break;
    }
    case INTERNET_STATUS_CONNECTED_TO_SERVER:
    {
        //SetStatus(IDS_HTTPDOWNLOAD_CONNECTED, (LPCTSTR) lpvStatusInformation);
        break;
    }
    case INTERNET_STATUS_REDIRECT:
    {
        //SetStatus(IDS_HTTPDOWNLOAD_REDIRECTING, (LPCTSTR) lpvStatusInformation);
        break;
    }
    default:
    {
        break;
    }
    }
}

BOOL CMainFrame::DownloadFromFTP()
{

    ASSERT(m_sURLToDownload.GetLength()); //Did you forget to specify the file to download
    if (!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort))
    {
        //Try sticking "http://" before it
        m_sURLToDownload = _T("http://") + m_sURLToDownload;
        if (!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort))
        {
            m_bSafeToClose=TRUE;
            return FALSE;

        }
    }
    //Check to see if the file we are downloading to exists and if
    //it does, then ask the user if they were it overwritten
    CFileStatus fs;
    ASSERT(m_sFileToDownloadInto.GetLength());
    if (CFile::GetStatus(m_sFileToDownloadInto, fs))
    {

    }
    //Try and open the file we will download into
    if (!m_FileToWrite.Open(m_sFileToDownloadInto, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
    {
        TRACE(_T("Failed to open the file to download into, Error:%d\n"), GetLastError());
        CString sError;
        sError.Format(_T("%d"), ::GetLastError());
        CString sMsg;
        AfxFormatString1(sMsg, IDS_HTTPDOWNLOAD_FAIL_FILE_OPEN, sError);
        //AfxMessageBox(sMsg);
        //EndDialog(IDCANCEL);
        m_bSafeToClose=TRUE;
        return FALSE;
    }

    //Pull out just the filename component
    int nSlash = m_sObject.ReverseFind(_T('/'));
    if (nSlash == -1)
        nSlash = m_sObject.ReverseFind(_T('\\'));
    if (nSlash != -1 && m_sObject.GetLength() > 1)
        m_sFilename = m_sObject.Right(m_sObject.GetLength() - nSlash - 1);
    else
        m_sFilename = m_sObject;

    //Set the file status text
    CString sFileStatus;
    ASSERT(m_sObject.GetLength());
    ASSERT(m_sServer.GetLength());
    AfxFormatString2(sFileStatus, IDS_HTTPDOWNLOAD_FILESTATUS, m_sFilename, m_sServer);

    return DownloadThread();
}
void CMainFrame::GetProductFPTAndLocalPath(int ProductModel,CString &FtpPath,CString &ProductFileName)
{
    CString strProductName;
    switch(ProductModel)
    {
    case PM_TSTAT5A:
        strProductName="TStat5A";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5B:
        strProductName="TStat5B";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5B2:
        strProductName="TStat5B2";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5C:
        strProductName="TStat5C";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5D:
        strProductName="TStat5D";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5E:
        strProductName="TStat5E";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5F:
        strProductName="TStat5F";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5G:
        strProductName="TStat5G";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }

        break;
    case PM_TSTAT5H:
        strProductName="TStat5H";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }

        break;
    case PM_TSTAT6:
        strProductName="TStat6";
        {
            FtpPath=_T("http://temcocontrols.com/ftp/firmware/Tstat6/128kChip/TSTAT6.HEX");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5i:
        strProductName="TStat5i";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_HUMTEMPSENSOR:
        strProductName="TstatHUM";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_AirQuality:
        strProductName="Air Quality";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT7:
        strProductName="TStat7";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_NC:
        strProductName="NC";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_CM5:
        strProductName ="CM5";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTATRUNAR:
        strProductName="TStatRunar";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    //20120424
    case PM_LightingController:
        strProductName = "LC";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case  PM_CO2_NET:
        strProductName = "CO2 Net";
        {
            FtpPath=_T("http://temcocontrols.com/ftp/firmware/CO2/CO2-W-E/CO2-W-E.bin");
            ProductFileName=_T("CO2-Ethernet.bin");
        }
        break;
    case  PM_CO2_RS485:
        strProductName = "CO2";
        {
            FtpPath=_T("http://temcocontrols.com/ftp/firmware/CO2/CO2-W/CO2-W.hex");
            ProductFileName=_T("CO2-RS485.HEX");
        }
        break;
    case PM_TSTAT6_HUM_Chamber:
        strProductName =g_strHumChamber;
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;

    case PM_T3PT10 :
        strProductName="T3-PT10";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T3IOA :
        strProductName="T3-8O";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T332AI :
        strProductName="T3-32AI";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case  PM_T38AI16O :
        strProductName="T3-8AI160";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T38I13O :
        strProductName="T3-8I13O";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T3PERFORMANCE :
        strProductName="T3-Performance";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T34AO :
        strProductName="T3-4AO";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T36CT :
        strProductName="T3-6CT";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_MINIPANEL:
        strProductName="MiniPanel";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
	case PM_MINIPANEL_ARM:
		strProductName = "MiniPanel";
		{
			FtpPath = _T("");
			ProductFileName = _T("Tstat6.HEX");
		}
		break;

    case PM_PRESSURE:
        strProductName="Pressure Sensor";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_HUM_R:
        strProductName="HUM-R";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    default:
        strProductName="TStat";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    }
}

void CMainFrame::GetProductFirmwareFTPDirectory(int ProductModel,CString &FtpPath,CString &ProductFileName)
{
    CString strProductName;
    switch(ProductModel)
    {
    case PM_TSTAT5A:
        strProductName="TStat5A";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5B:
        strProductName="TStat5B";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5B2:
        strProductName="TStat5B2";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5C:
        strProductName="TStat5C";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5D:
        strProductName="TStat5D";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5E:
        strProductName="TStat5E";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5F:
        strProductName="TStat5F";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT5G:
        strProductName="TStat5G";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }

        break;
    case PM_TSTAT5H:
        strProductName="TStat5H";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }

        break;
    case PM_TSTAT6:
        strProductName="TStat6";
        {
            FtpPath=_T("http://temcocontrols.com/ftp/firmware/Tstat6/128kChip/");
            ProductFileName=_T("TSTAT6.HEX");
        }
        break;
    case PM_TSTAT5i:
        strProductName="TStat5i";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_HUMTEMPSENSOR:
        strProductName="TstatHUM";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_AirQuality:
        strProductName="Air Quality";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTAT7:
        strProductName="TStat7";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_NC:
        strProductName="NC";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_CM5:
        strProductName ="CM5";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_TSTATRUNAR:
        strProductName="TStatRunar";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    //20120424
    case PM_LightingController:
        strProductName = "LC";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case  PM_CO2_NET:
        strProductName = "CO2 Net";
        {
            FtpPath=_T("http://temcocontrols.com/ftp/firmware/CO2/CO2-W-E/");
            ProductFileName=_T("CO2-W-E.bin");
        }
        break;
    case  PM_CO2_RS485:
        strProductName = "CO2";
        {
            FtpPath=_T("http://temcocontrols.com/ftp/firmware/CO2/CO2-W/");
            ProductFileName=_T("CO2-W.hex");
        }
        break;
    case PM_TSTAT6_HUM_Chamber:
        strProductName =g_strHumChamber;
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;

    case PM_T3PT10 :
        strProductName="T3-PT10";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T3IOA :
        strProductName="T3-8O";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T332AI :
        strProductName="T3-32AI";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case  PM_T38AI16O :
        strProductName="T3-8AI160";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T38I13O :
        strProductName="T3-8I13O";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T3PERFORMANCE :
        strProductName="T3-Performance";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T34AO :
        strProductName="T3-4AO";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_T36CT :
        strProductName="T3-6CT";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_MINIPANEL:
        strProductName="MiniPanel";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_PRESSURE:
        strProductName="Pressure Sensor";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    case PM_HUM_R:
        strProductName="HUM-R";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    default:
        strProductName="TStat";
        {
            FtpPath=_T("");
            ProductFileName=_T("Tstat6.HEX");
        }
        break;
    }
}
#include "DetectOnlineDlg.h"
void CMainFrame::OnToolDetectonlineproducts()
{
    CLoginDlg Dlg(g_buser_log_in);
    if (Dlg.DoModal()==IDOK)
    {
        CDetectOnlineDlg dlg;
        dlg.DoModal();
    }



}


void CMainFrame::OnUpdateControlMain(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_MAIN);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_HOME));
}


void CMainFrame::OnUpdateControlInputs(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_INPUT);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_INPUT));

//       if(pCmdUI->m_pMenu != NULL)
//       {
//           pCmdUI->SetText(L"TEST INPUT");
//       }

}

void CMainFrame::OnUpdateControlOutputs(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_OUTPUT);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_OUTPUT));
}


void CMainFrame::OnUpdateControlVariables(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_VARIABLE);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_VARIABLE));
}


void CMainFrame::OnUpdateControlPrograms(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_PROGRAM);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_PROGRAM));
}

void CMainFrame::OnUpdateControlPanel(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_PANEL);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_PANELINFO));
}



void CMainFrame::OnUpdateControlScreens(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_SCREENS);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_SCREEN));
}


void CMainFrame::OnUpdateControlControllers(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_CONTROLLER);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_PID));
}


void CMainFrame::OnUpdateControlWeekly(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_WEEKLY);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_SCHEDUAL));
}


void CMainFrame::OnUpdateControlAnnualroutines(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_ANNUAL);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_HOLIDAY));
}


void CMainFrame::OnUpdateControlMonitors(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_MONITOR);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_TRENDLOG));
}


void CMainFrame::OnUpdateControlAlarmLog(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_ALARMLOG);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_ALARM));
}


void CMainFrame::OnUpdateControlTstat(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_TSTAT);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_NETWORK_POINT));
}


void CMainFrame::OnUpdateControlSettings(CCmdUI *pCmdUI)
{
    //  Add your command update UI handler code here
    pCmdUI->SetCheck(bacnet_view_number == TYPE_SETTING);
    pCmdUI->Enable(Get_Product_Menu_Map(g_selected_product_id, MENU_SETTING));
}

void CMainFrame::Create_Thread_Read_Item(int n_item)
{
    if(WaitRead_Data_Dlg)
    {
        delete WaitRead_Data_Dlg;
        WaitRead_Data_Dlg = 0;
    }
    WaitRead_Data_Dlg = new CDialog_Progess(this,1,100);
    //创建对话框窗口
    WaitRead_Data_Dlg->Create(IDD_DIALOG10_Progress, this);
    WaitRead_Data_Dlg->ShowProgress(0,0);
    RECT RECT_SET1;
    ::GetWindowRect(BacNet_hwd,&RECT_SET1);
    WaitRead_Data_Dlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 19,800,20,1);
    WaitRead_Data_Dlg->ShowWindow(SW_HIDE);
    g_bac_read_type = n_item;
    if(click_read_thread==NULL)
    {
        click_read_thread =CreateThread(NULL,NULL,MSTP_Send_read_Command_Thread,this,NULL, NULL);
    }
}



BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo)
{ 
 
		HWND hWnd;

		if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);

		return (hWnd != NULL);
	 
 
    return CFrameWndEx::OnHelpInfo(pHelpInfo);
}



/// <summary>
/// use the tool ,wo export a register list 
/// </summary>
void CMainFrame::OnFileExportregiseterslist()
{
    CString temp_folder_path;
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    CString Folder_Path;
    Folder_Path = ApplicationFolder + _T("\\ResourceFile");
    temp_folder_path = Folder_Path + _T("\\RegistersListDB.mdb");
    CFileFind cs_temp_find;
    if (!cs_temp_find.FindFile(temp_folder_path))
    {
        CStdioFile*		 m_plogFile;
        m_plogFile = new CStdioFile;
        if (m_plogFile->Open(temp_folder_path.GetString(), CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate))
        {
            Sleep(1);
        }
        m_plogFile->Close();
    }
    CString str = _T("/select, ") + temp_folder_path;
    ShellExecute(NULL, _T("open"), _T("explorer.exe"), str, NULL, SW_SHOWNORMAL);
	//CString strHistotyFile = g_strExePth + _T("ModbusBacnetRegistersList.xls");
	//ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
}



/// <summary>
/// register maintenance stystem
/// </summary>
void CMainFrame::OnToolRegistersmaintenancesystem()
{
    //维护寄存器列表管理的 一个应用程序
    //它能够维护所以产品的寄存器
    // 它的所有的功能列表
    /*
    1>用户管理
    2>密码修改
    3>项目管理
    4>寄存器信息修改
    5>产品的添加
    */
  /*  CString strHistotyFile=g_strExePth+_T("RegisterListManager.exe");
    ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);*/
}

/// <summary>
/// 消息提示 ，根据不同的控件，提示不一样的信息
/// </summary>
/// <param name="id"></param>
/// <param name="Pnmhdr"></param>
/// <param name="pResult"></param>
/// <returns></returns>
BOOL CMainFrame::OnToolTipNotify(UINT id,NMHDR *Pnmhdr,LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *) Pnmhdr;

    UINT_PTR nID = Pnmhdr->idFrom - 1;

    if (Pnmhdr->hwndFrom == m_testtoolbar.Get_Tip_HWND())
    {
        CMFCToolBarButton *pBtn = m_testtoolbar.GetButton(nID);
        if (pBtn)
        {
            if(pBtn->m_nID ==  ID_CONTROL_MAIN)
            {
                pTTT->lpszText = _T("Information \r\nMore information about the device");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_INPUTS)
            {
                pTTT->lpszText = _T("Inputs [ Alt-I ]\r\nSensors and feedback points wired to the controller");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_OUTPUTS)
            {
                pTTT->lpszText = _T("Outputs [ Alt-O ]\r\nValves, relays and actuators wired to the controller");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_VARIABLES)
            {
                pTTT->lpszText = _T("Variables [ Alt-V ]\r\nSetpoints, counters, timers and other system defined items");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_PROGRAMS)
            {
                pTTT->lpszText = _T("Programs [ Alt-P ]\r\nSystem logic and sequencing");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_CONTROLLERS)
            {
                pTTT->lpszText = _T("PID Loops [ Alt-L ]\r\nProportional-Integral-Derivative feedback control loops");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_SCREENS)
            {
                pTTT->lpszText = _T("Graphics [ Alt-G ]\r\nFloor plans, system diagrams, graphical displays");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_WEEKLY)
            {
                pTTT->lpszText = _T("Schedules [ Alt-S ]\r\nDaily on and off events for the week");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_ANNUALROUTINES)
            {
                pTTT->lpszText = _T("Holidays [ Alt-H ]\r\nHolidays and days with special schedules during the year");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_MONITORS)
            {
                pTTT->lpszText = _T("Trend Logs [ Alt-T ]\r\nGraphs and historical data");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_ALARM_LOG)
            {
                pTTT->lpszText = _T("Alarms [ Alt-A ]\r\nCurrent and past alarm events");
            }
            else if (pBtn->m_nID == ID_CONTROL_PANEL)
            {
                pTTT->lpszText = _T("SubPanel [ Alt-E ]\r\nPanel and Sub panel");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_TSTAT)
            {
                pTTT->lpszText = _T("Network Points[ Alt-N ]\r\nNetwork points from other nodes used by the controller");
            }
            else if(pBtn->m_nID ==  ID_CONTROL_SETTINGS)
            {
                pTTT->lpszText = _T("Configuration [ Alt-C ]\r\nSetup and advanced parameters");
            }
            else if(pBtn->m_nID ==  ID_SCAN)
            {
                pTTT->lpszText = _T("Discover [Alt-D]\r\nDiscover new devices on the LAN, com ports and USB");
            }
            else if(pBtn->m_nID ==  ID_BUILDINGCONFIGDB)
            {
                pTTT->lpszText = _T("Buildings [Alt-B]\r\nDatabase of all buildings, connect to a different site or go offline to set up a new one");
            }
			else if (pBtn->m_nID == ID_TOOLS_MODBUSTOBACNETROUTER)
			{
				pTTT->lpszText = _T("Subnetwork Devices [Alt-D]\r\nDevices which connect to this devices subnetwork(s)");
			}
			else if (pBtn->m_nID == ID_CONTROL_IO_NET_CONFIG)
			{
				pTTT->lpszText = _T("Bacnet IO Configuration");
			}
            pTTT->hinst = AfxGetResourceHandle();
        }
    }

    return true;

}







INT_PTR CMainFrame::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
    

    return CFrameWndEx::OnToolHitTest(point, pTI);


    CString adoStr;
    static CString strText;
    INT_PTR hInt = CFrameWndEx::OnToolHitTest(point,pTI);//先调用基类函数,再修改串.   ;
    strText = pTI->lpszText;

    //if(pTI->uId == ID_CONTROL_INPUTS )
    //{
    //	adoStr = _T("test111");

    //}
    strText = adoStr + strText; //重新组合成: New(Ctrl+N)形式.

    //基类中会释放这个指针pTI->lpszText,但是由于我们要修改这个串,
    //所以原来的空间不一定够,所以要释放掉原来的,并重新申请,
    int len = strText.GetLength()*sizeof(TCHAR);
    TCHAR *pBuf =(TCHAR*)malloc(len+10);
    memset(pBuf,0,len+10);
    _tcscpy_s(pBuf,strText.GetLength()+2,strText);
    free(pTI->lpszText);
    pTI->lpszText = pBuf;

    return hInt;

}

#include "BatchSNDlg.h"
void CMainFrame::OnToolFlashsn()
{
    g_bPauseMultiRead =TRUE;
    CBatchSNDlg dlg;
    dlg.DoModal();
    g_bPauseMultiRead =FALSE;
}
#include "BacnetProgramEdit.h"
extern CBacnetProgramEdit *ProgramEdit_Window;
void CMainFrame::Reset_Window_Pos()
{

	if(b_create_status)
	{
		CRect temprec;
		m_wndStatusBar.GetWindowRect(&temprec);
		if((temprec.top >0) && (temprec.bottom >0) && (temprec.left >0) && (temprec.right > 0))
			statusbar->SetWindowPos(&wndNoTopMost,temprec.left,temprec.top,temprec.Width(),temprec.Height(),SWP_NOACTIVATE | SWP_SHOWWINDOW );
		if(((CBacnetInput *)pDialog[WINDOW_INPUT]) != NULL)
		{
			if(((CBacnetInput *)pDialog[WINDOW_INPUT])->IsWindowVisible())
			{
				((CBacnetInput *)pDialog[WINDOW_INPUT])->Reset_Input_Rect();
			}
		}

		if(((CBacnetOutput *)pDialog[WINDOW_OUTPUT]) != NULL)
		{
			if(((CBacnetOutput *)pDialog[WINDOW_OUTPUT])->IsWindowVisible())
			{
				((CBacnetOutput *)pDialog[WINDOW_OUTPUT])->Reset_Output_Rect();
			}
		}

		if(((CBacnetVariable *)pDialog[WINDOW_VARIABLE]) != NULL)
		{
			if(((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->IsWindowVisible())
			{
				((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->Reset_Variable_Rect();
			}
		}

		if(((CBacnetProgram *)pDialog[WINDOW_PROGRAM]) != NULL)
		{
			if(((CBacnetProgram *)pDialog[WINDOW_PROGRAM])->IsWindowVisible())
			{
				((CBacnetProgram *)pDialog[WINDOW_PROGRAM])->Reset_Program_Rect();
			}
		}

		if(((BacnetController *)pDialog[WINDOW_CONTROLLER]) != NULL)
		{
			if(((BacnetController *)pDialog[WINDOW_CONTROLLER])->IsWindowVisible())
			{
				((BacnetController *)pDialog[WINDOW_CONTROLLER])->Reset_Controller_Rect();
			}
		}

		if(((BacnetScreen *)pDialog[WINDOW_SCREEN]) != NULL)
		{
			if(((BacnetScreen *)pDialog[WINDOW_SCREEN])->IsWindowVisible())
			{
				((BacnetScreen *)pDialog[WINDOW_SCREEN])->Reset_Screen_Rect();
			}
		}
		if(((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY]) != NULL)
		{
			if(((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->IsWindowVisible())
			{
				((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->Reset_Weekly_Rect();
			}
		}
		if(((BacnetAnnualRoutine *)pDialog[WINDOW_ANNUAL]) != NULL)
		{
			if(((BacnetAnnualRoutine *)pDialog[WINDOW_ANNUAL])->IsWindowVisible())
			{
				((BacnetAnnualRoutine *)pDialog[WINDOW_ANNUAL])->Reset_Annual_Rect();
			}
		}

		if(((CBacnetMonitor *)pDialog[WINDOW_MONITOR]) != NULL)
		{
			if(((CBacnetMonitor *)pDialog[WINDOW_MONITOR])->IsWindowVisible())
			{
				((CBacnetMonitor *)pDialog[WINDOW_MONITOR])->Reset_Monitor_Rect();
			}
		}

		if(((CBacnetAlarmLog *)pDialog[WINDOW_ALARMLOG]) != NULL)
		{
			if(((CBacnetAlarmLog *)pDialog[WINDOW_ALARMLOG])->IsWindowVisible())
			{
				((CBacnetAlarmLog *)pDialog[WINDOW_ALARMLOG])->Reset_Alarm_Rect();
			}
		}

		if(((CBacnetRemotePoint *)pDialog[WINDOW_REMOTE_POINT]) != NULL)
		{
			if(((CBacnetRemotePoint *)pDialog[WINDOW_REMOTE_POINT])->IsWindowVisible())
			{
				((CBacnetRemotePoint *)pDialog[WINDOW_REMOTE_POINT])->Reset_RemotePoint_Rect();
			}
		}

		if(((CBacnetSetting *)pDialog[WINDOW_SETTING]) != NULL)
		{
			if(((CBacnetSetting *)pDialog[WINDOW_SETTING])->IsWindowVisible())
			{
				((CBacnetSetting *)pDialog[WINDOW_SETTING])->Reset_Setting_Rect();
			}
		}

		switch(bacnet_view_number )
		{
		case TYPE_SETTING:
		case TYPE_MAIN:
			if(((CBacnetSetting *)pDialog[WINDOW_SETTING]) != NULL)
			{
				if(((CBacnetSetting *)pDialog[WINDOW_SETTING])->IsWindowVisible())
				{
					((CBacnetSetting *)pDialog[WINDOW_SETTING])->Reset_Setting_Rect();
				}
			}
			break;
		case TYPE_INPUT:
			if(((CBacnetInput *)pDialog[WINDOW_INPUT]) != NULL)
			{
				if(((CBacnetInput *)pDialog[WINDOW_INPUT])->IsWindowVisible())
				{
					((CBacnetInput *)pDialog[WINDOW_INPUT])->Reset_Input_Rect();
				}
			}
			break;
		case TYPE_OUTPUT:
			if(((CBacnetOutput *)pDialog[WINDOW_OUTPUT]) != NULL)
			{
				if(((CBacnetOutput *)pDialog[WINDOW_OUTPUT])->IsWindowVisible())
				{
					((CBacnetOutput *)pDialog[WINDOW_OUTPUT])->Reset_Output_Rect();
				}
			}
			break;
		case TYPE_PROGRAM:
			if(((CBacnetProgram *)pDialog[WINDOW_PROGRAM]) != NULL)
			{
				if(((CBacnetProgram *)pDialog[WINDOW_PROGRAM])->IsWindowVisible())
				{
					((CBacnetProgram *)pDialog[WINDOW_PROGRAM])->Reset_Program_Rect();
				}
			}
			break;
		case TYPE_VARIABLE:
			if(((CBacnetVariable *)pDialog[WINDOW_VARIABLE]) != NULL)
			{
				if(((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->IsWindowVisible())
				{
					((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->Reset_Variable_Rect();
				}
			}
			break;
		case TYPE_WEEKLY:
			if(((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY]) != NULL)
			{
				if(((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->IsWindowVisible())
				{
					((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->Reset_Weekly_Rect();
				}
			}
			break;
		case TYPE_ANNUAL:
			if(((BacnetAnnualRoutine *)pDialog[WINDOW_ANNUAL]) != NULL)
			{
				if(((BacnetAnnualRoutine *)pDialog[WINDOW_ANNUAL])->IsWindowVisible())
				{
					((BacnetAnnualRoutine *)pDialog[WINDOW_ANNUAL])->Reset_Annual_Rect();
				}
			}
			break;
		case TYPE_CONTROLLER:
			if(((BacnetController *)pDialog[WINDOW_CONTROLLER]) != NULL)
			{
				if(((BacnetController *)pDialog[WINDOW_CONTROLLER])->IsWindowVisible())
				{
					((BacnetController *)pDialog[WINDOW_CONTROLLER])->Reset_Controller_Rect();
				}
			}
			break;
		case TYPE_SCREENS:
			if(((BacnetScreen *)pDialog[WINDOW_SCREEN]) != NULL)
			{
				if(((BacnetScreen *)pDialog[WINDOW_SCREEN])->IsWindowVisible())
				{
					((BacnetScreen *)pDialog[WINDOW_SCREEN])->Reset_Screen_Rect();
				}
			}
			break;
		case TYPE_MONITOR:
			if(((CBacnetMonitor *)pDialog[WINDOW_MONITOR]) != NULL)
			{
				if(((CBacnetMonitor *)pDialog[WINDOW_MONITOR])->IsWindowVisible())
				{
					((CBacnetMonitor *)pDialog[WINDOW_MONITOR])->Reset_Monitor_Rect();
				}
			}
			break;
        case TYPE_ALARMLOG:
            if (((CBacnetAlarmLog *)pDialog[WINDOW_ALARMLOG]) != NULL)
            {
                if (((CBacnetAlarmLog *)pDialog[WINDOW_ALARMLOG])->IsWindowVisible())
                {
                    ((CBacnetAlarmLog *)pDialog[WINDOW_ALARMLOG])->Reset_Alarm_Rect();
                }
            }
            break;
		default:
			break;
		}
		if(ProgramEdit_Window)
		{
			if(ProgramEdit_Window->IsWindowVisible())
				::SetWindowPos(ProgramEdit_Window->m_hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}
}

void CMainFrame::OnMove(int x, int y)
{
    CFrameWndEx::OnMove(x, y);

    
	//TRACE(_T("Move to x = %d , y = %d \r\n"),x,y);
		static int cmove_cx = 0;
	static int cmove_cy = 0;
	if((cmove_cx != x) || (cmove_cy != y))
	{
		Reset_Window_Pos();
		cmove_cx = x;
		cmove_cy = y;
	}
	

	if(AlarmWindow_Window)
	{
		CRect temprec;
		m_testtoolbar.GetWindowRect(&temprec);
		if(AlarmWindow_Window->IsWindowVisible())
			AlarmWindow_Window->SetWindowPos(&wndNoTopMost,temprec.left + 392,temprec.top + 1,32,temprec.Height() - 5,SWP_NOACTIVATE | SWP_SHOWWINDOW );
		::PostMessage(m_alarmwindow_dlg_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
	}
}


void CMainFrame::Check_Local_TemcoUpdate()
{
	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");

	int need_local_update = false;
	need_local_update  = GetPrivateProfileInt(_T("Setting"),_T("LocalUpdateT3000"),0,temp_db_ini_folder);
	if(need_local_update == 0)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("LocalUpdateT3000"),_T("0"),temp_db_ini_folder);
	}
	else if(need_local_update == 1115)
	{

		CFileFind temp_find_bat;
		CString temp_cs_bat_path;
		temp_cs_bat_path = g_strExePth + _T("Update.bat");
		if(temp_find_bat.FindFile(temp_cs_bat_path))
		{
			CPing p2_z_zdriver;
			CPingReply pr1;
			if (p2_z_zdriver.Ping1(_T("192.168.0.2"), pr1))
			{
				// CurrentT3000Version
				CString zdriver_version;
				GetPrivateProfileStringW(_T("Setting"),_T("Version"),_T(""),zdriver_version.GetBuffer(MAX_PATH),MAX_PATH,_T("Z:\\chelsea\\temp\\T3000UpdateVersion.ini"));
				zdriver_version.ReleaseBuffer();

				if(!zdriver_version.IsEmpty())
				{
					zdriver_version.Trim();
					CString temp_cs_1 = CurrentT3000Version;
					temp_cs_1.Trim();
					if(zdriver_version.CompareNoCase(temp_cs_1) != 0 )
					{
						ShellExecute(NULL, _T("open"), temp_cs_bat_path, NULL, NULL, SW_SHOWNORMAL);
						Sleep(2000);
						PostMessage(WM_CLOSE,NULL,NULL);
					}
				}
			}		
		}

	}
}


void CMainFrame::OnHelpUsingUpdate()
{
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	CString temp_AutoFlashConfigPath = ApplicationFolder + _T("//AutoFlashFile.ini");
	CFileFind fFind;
	if(fFind.FindFile(temp_AutoFlashConfigPath))
	{
		DeleteFile(temp_AutoFlashConfigPath);
	}

	int temp_product_count = m_product.size();
	if(((selected_tree_item == NULL) || (selected_product_index == -1)))
	{

		m_product_isp_auto_flash.product_class_id =  199;
	}


	b_pause_refresh_tree = ONHELPUSINGUPDATE;
	bool temp_status = g_bPauseMultiRead;
	g_bPauseMultiRead = true;
	int temp_type = GetCommunicationType();


	BOOL bDontLinger = FALSE;
	setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
	closesocket(h_Broad);

	if((temp_product_count > 0) && (selected_product_index!=-1) && (selected_product_index < temp_product_count))
	{
        m_product_isp_auto_flash = m_product.at(selected_product_index);
	}

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


#include "BootFinderDlg.h"
void CMainFrame::OnToolBootloader()
{
	CBootFinderDlg dlg;
	dlg.DoModal ();
}

void CMainFrame::HideBacnetWindow()
{
	if(Input_Window!=NULL)
	{
		if(Input_Window->IsWindowVisible())
			Input_Window->ShowWindow(SW_HIDE);
	}
	if(Output_Window!=NULL)
	{
		if(Output_Window->IsWindowVisible())
			Output_Window->ShowWindow(SW_HIDE);
	}
	if(Variable_Window!=NULL)
	{
		if(Variable_Window->IsWindowVisible())
			Variable_Window->ShowWindow(SW_HIDE);
	}
	if(Program_Window!=NULL)
	{
		if(Program_Window->IsWindowVisible())
			Program_Window->ShowWindow(SW_HIDE);
	}
	if(Controller_Window!=NULL)
	{
		if(Controller_Window->IsWindowVisible())
			Controller_Window->ShowWindow(SW_HIDE);
	}
	if(Screen_Window!=NULL)
	{
		if(Screen_Window->IsWindowVisible())
			Screen_Window->ShowWindow(SW_HIDE);
	}
	if(WeeklyRoutine_Window!=NULL)
	{
		if(WeeklyRoutine_Window->IsWindowVisible())
			WeeklyRoutine_Window->ShowWindow(SW_HIDE);
	}
	if(AnnualRoutine_Window!=NULL)
	{
		if(AnnualRoutine_Window->IsWindowVisible())
			AnnualRoutine_Window->ShowWindow(SW_HIDE);
	}
	if(Monitor_Window!=NULL)
	{
		if(Monitor_Window->IsWindowVisible())
			Monitor_Window->ShowWindow(SW_HIDE);
	}
	if(AlarmLog_Window!=NULL)
	{
		if(AlarmLog_Window->IsWindowVisible())
			AlarmLog_Window->ShowWindow(SW_HIDE);
	}
	if(Tstat_Window!=NULL)
	{
		if(Tstat_Window->IsWindowVisible())
			Tstat_Window->ShowWindow(SW_HIDE);
	}
	if(Setting_Window!=NULL)
	{
		if(Setting_Window->IsWindowVisible())
			Setting_Window->ShowWindow(SW_HIDE);
	}
	if(User_Login_Window!=NULL)
	{
		if(User_Login_Window->IsWindowVisible())
			User_Login_Window->ShowWindow(SW_HIDE);
	}
	if(Remote_Point_Window!=NULL)
	{
		if(Remote_Point_Window->IsWindowVisible())
			Remote_Point_Window->ShowWindow(SW_HIDE);
	}
}

void CMainFrame::OnToolsPsychrometry()
{
	CString strHistotyFile=g_strExePth+_T("Psychrometry\\WFA_psychometric_chart.exe");
	ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
}

#include "T3000Option.h"

void CMainFrame::OnToolsOption()
{
	 //CT3000Option dlg;
	 //dlg.DoModal();
}


typedef pair<int,string> mydefpair;



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	static int cmainf_cx = 0;
	static int cmainf_cy = 0;
	if((cmainf_cx != cx) || (cmainf_cy != cy))
	{
		Reset_Window_Pos();
		cmainf_cx = cx;
		cmainf_cy = cy;
	}
	
}



void CMainFrame::OnModeOnlinemode()
{
	
	offline_mode = false;
	m_pTreeViewCrl->SetTreeOfflineMode(offline_mode);
	m_pTreeViewCrl->KillTimer(1);
	m_pTreeViewCrl->StopFlashItem();

	set_offline_mode(offline_mode);

	CString	g_configfile_path = g_strExePth + _T("T3000_config.ini");
	CFileFind fFind;
	if (!fFind.FindFile(g_configfile_path))
	{
		WritePrivateProfileStringW(_T("ONOFFLINE_MODEL"), _T("ISONLINE"), _T("0"), g_configfile_path);

	}
	else
	{
		WritePrivateProfileStringW(_T("ONOFFLINE_MODEL"), _T("ISONLINE"), _T("0"), g_configfile_path);
	}

}


void CMainFrame::OnModeOfflinemode()
{
	
	offline_mode = true;
	m_pTreeViewCrl->SetTreeOfflineMode(offline_mode);
	m_pTreeViewCrl->SetTimer(1,500,NULL);
	CreateOfflinePrgFile();
	set_offline_mode(offline_mode);

	CString	g_configfile_path = g_strExePth + _T("T3000_config.ini");
	CFileFind fFind;
	if (!fFind.FindFile(g_configfile_path))
	{
		WritePrivateProfileStringW(_T("ONOFFLINE_MODEL"), _T("ISONLINE"), _T("1"), g_configfile_path);
		 
	}
	else
	{
		WritePrivateProfileStringW(_T("ONOFFLINE_MODEL"), _T("ISONLINE"), _T("1"), g_configfile_path);
	}

}


void CMainFrame::OnUpdateModeOnlinemode(CCmdUI *pCmdUI)
{
	//  Add your command update UI handler code here
	if(offline_mode)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateModeOfflinemode(CCmdUI *pCmdUI)
{
	//  Add your command update UI handler code here
	if(offline_mode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}



void CMainFrame::OnToolsPhchart()
{
	CString strHistotyFile = g_strExePth + _T("PH_Application\\PH_App_By_BBK.exe");
	ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
}


//add comments by NOVAS
//no change source code
#include "ModbusToBacnetRouterDlg.h"

void CMainFrame::OnToolsModbustobacnetrouter()
{
	CModbusToBacnetRouterDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnControlModbus()
{
	HideBacnetWindow();
	custom_bacnet_register_listview = true;
	SwitchToPruductType(DLG_DIALOG_CUSTOM_VIEW);

}

#include "BacnetIOConfig.h"
void CMainFrame::OnControlIoNetConfig()
{
	if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 438)
	{
		MessageBox(_T("This feature need the newest firmware."));
		return;
	}


	if (GetPrivateData_Blocking(g_bac_instance, READEXT_IO_T3000, 0, BAC_EXTIO_COUNT - 1, sizeof(Str_Extio_point)) < 0)
	{
		MessageBox(_T("Read data timeout"));
		return;
	}
	CBacnetIOConfig IOdlg;
	IOdlg.DoModal();
}


void CMainFrame::OnDatabaseLogdetail()
{
    // TODO: 在此添加命令处理程序代码
    ShowDebugWindow();
}

void CMainFrame::OnLoadConfigFile()
{
    LoadConfigFilePath.Empty();
    if ((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        MainFram_hwd = this->m_hWnd;
        LoadBacnetBinaryFile(true, NULL);
        return;
    }
    else if (((g_protocol == MODBUS_RS485) || (g_protocol == MODBUS_TCPIP)) &&
        ((bacnet_device_type == T38AI8AO6DO) || (bacnet_device_type == PID_T322AI) || (bacnet_device_type == PM_T3_LC) || (bacnet_device_type == PID_T3PT12) || (bacnet_device_type == PID_T36CTA) || (bacnet_device_type == PWM_TRANSDUCER)))
    {
        CFileDialog dlg(true, _T("*.prog"), _T(" "), OFN_HIDEREADONLY, _T("Prog files (*.prog)|*.prog||"), NULL, 0);
        if (IDOK != dlg.DoModal())
            return;
        LoadConfigFilePath = dlg.GetPathName();

        if (hwait_write_modbus10000 == NULL)
        {
            hwait_write_modbus10000 = CreateThread(NULL, NULL, Write_Modbus_10000, this, NULL, NULL);
        }
    }
    else if ((product_register_value[7] == PM_TSTAT6) ||
        (product_register_value[7] == PM_PM5E) ||
        (product_register_value[7] == PM_PM5E_ARM) ||
        (product_register_value[7] == PM_TSTAT7) ||
        (product_register_value[7] == PM_TSTAT5i) ||
        (product_register_value[7] == PM_TSTAT5G) ||
        (product_register_value[7] == PM_TSTAT8) ||
        (product_register_value[7] == PM_TSTAT9) ||
        (product_register_value[7] == PM_TSTAT8_WIFI) ||
        (product_register_value[7] == PM_TSTAT8_OCC) ||
        (product_register_value[7] == PM_TSTAT7_ARM) ||
        (product_register_value[7] == PM_TSTAT8_220V))
    {
        CFileDialog dlg(true, _T("*.txt"), _T(" "), OFN_HIDEREADONLY, _T("Txt files (*.txt)|*.txt||"), NULL, 0);
        if (IDOK != dlg.DoModal())
            return;
        LoadConfigFilePath = dlg.GetPathName();

        if (hwait_write_tstat_cfg != NULL)
        {
            TerminateThread(hwait_write_tstat_cfg, 0);
            hwait_write_tstat_cfg = NULL;
        }
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Loading config file , Please wait!"));
        hwait_write_tstat_cfg = CreateThread(NULL, NULL, Write_Modbus_tstat_cfg, this, NULL, NULL);
    }

    //AfxMessageBox(_T("Load configuration file."));
    if (g_LoadConfigLevel == 1)
    {
        CAfxMessageDialog dlg;
        CString strPromption;
        strPromption.LoadString(IDS_STRNOPRIVILEGE2);
        dlg.SetPromtionTxt(strPromption);
        dlg.DoModal();
        return;
    }

}

void CMainFrame::SaveConfigFile()
{
    if ((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == PROTOCOL_BIP_TO_MSTP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
    {
        MainFram_hwd = this->m_hWnd;

        CFileDialog dlg(false, _T("*.prog"), _T(" "), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("prog files (*.prog)|*.prog|All Files (*.*)|*.*||"), NULL, 0);
        if (IDOK == dlg.DoModal())
        {

            SaveConfigFilePath = dlg.GetPathName();
            //CFileFind cfindtempfile;
            //if(cfindtempfile.FindFile(SaveConfigFilePath))
            //{
            //    DeleteFile(SaveConfigFilePath);
            //}

            //协议时bacnet ，用户点击 File save时 先调用线程读取所有需要存的资料；在发送消息回来 调用Save
            //::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SVAE_CONFIG);

            if ((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))
            {
                Show_Wait_Dialog_And_ReadBacnet(0);
            }
            else
            {
                Create_Thread_Read_Item(TYPE_ALL);
            }


        }
        else
        {
            SaveConfigFilePath.Empty();
        }
        return;
    }
    else if (((g_protocol == MODBUS_RS485) || (g_protocol == MODBUS_TCPIP)) &&
        ((bacnet_device_type == T38AI8AO6DO) ||
        (bacnet_device_type == PID_T322AI) ||
            (bacnet_device_type == PM_T3_LC) ||
            (bacnet_device_type == PID_T3PT12) ||
            (bacnet_device_type == PID_T36CTA) ||
            (bacnet_device_type == PWM_TRANSDUCER)))
    {
        //T3的设备支持minipanel的 input output 就读10000以后的寄存器;
        MainFram_hwd = this->m_hWnd;

        CFileDialog dlg(false, _T("*.prog"), _T(" "), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("prog files (*.prog)|*.prog|All Files (*.*)|*.*||"), NULL, 0);
        if (IDOK == dlg.DoModal())
        {

            SaveConfigFilePath = dlg.GetPathName();
            CFileFind cfindtempfile;
            if (cfindtempfile.FindFile(SaveConfigFilePath))
            {
                DeleteFile(SaveConfigFilePath);
            }

            if (hwait_read_modbus10000 == NULL)
            {

                hwait_read_modbus10000 = CreateThread(NULL, NULL, Read_Modbus_10000, this, NULL, NULL);
            }

        }
        return;
    }
    int nret = 0;
    g_bEnableRefreshTreeView = FALSE;
    int IDFlag = read_one(g_tstat_id, 7);
    product_register_value[7] = IDFlag;
    if (IDFlag <= 0)
        return;
    float version = get_curtstat_version();
    CString strFilter;
    CString strFilename;
    strFilter = _T("Text File|*.txt|All File|*.*|");
    CFileDialog dlg(false, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);
    if (dlg.DoModal() != IDOK)
        return;
    strFilename = dlg.GetPathName();

    BeginWaitCursor();
    CString strTips = _T("T3000 is ready to save config file, start now?");

    SetPaneString(1, strTips);
    if (product_register_value[7] == 100 || product_register_value[7] == PM_LightingController)//NC
    {
        strTips.Format(_T("Config file \" %s \" is saving, please wait..."), strFilename);
        SetPaneString(1, strTips);
        if (product_register_value[7] == 100)
        {
            save_schedule_2_file((LPTSTR)(LPCTSTR)strFilename, g_tstat_id);
        }
        else
        {
            save_schedule_2_file_LC((LPTSTR)(LPCTSTR)strFilename, g_tstat_id);
        }


        strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
        SetPaneString(1, strTips);
    }
    else if (product_register_value[7] == PM_T3PT10 ||
        product_register_value[7] == PM_T3IOA ||
        product_register_value[7] == PM_T332AI ||
        product_register_value[7] == PM_T38AI16O ||
        product_register_value[7] == PM_T38I13O ||
        product_register_value[7] == PM_T3PERFORMANCE ||
        product_register_value[7] == PM_T34AO ||
        product_register_value[7] == PM_T36CT ||
        product_register_value[7] == PM_T322AI ||
        product_register_value[7] == PM_T3PT12 ||
        product_register_value[7] == PM_T36CTA ||
        product_register_value[7] == PM_T38AI8AO6DO
        )
    {
        strTips.Format(_T("Config file \" %s \" is saving, please wait..."), strFilename);
        SetPaneString(1, strTips);

        save_T3Modules_file((LPTSTR)(LPCTSTR)strFilename, g_tstat_id);

        strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);
        SetPaneString(1, strTips);
    }
    else if ((product_register_value[7] == PM_TSTAT6) || (product_register_value[7] == PM_TSTAT7) || (product_register_value[7] == PM_TSTAT5i) || (product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
        || (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V))
    {

        int index = 0;


        memcpy(product_register_value, tempchange, sizeof(product_register_value));


        Save2File_ForTwoFilesTSTAT67((LPTSTR)(LPCTSTR)strFilename);


        //strTips.Format(_T("Config file \" %s \" saved successful."), strFilename);

        //SetPaneString(3, strTips);
    }
    else if (product_register_value[7] == PM_CO2_NODE)
    {
        Save2File_ForCO2Node((LPTSTR)(LPCTSTR)strFilename);
    }
    else//save tstat config file:
    {
        Save2File_ForTwoFiles((LPTSTR)(LPCTSTR)strFilename);
        strTips.Format(_T("Config file \" %s \" saved successfully."), strFilename);
        SetPaneString(1, strTips);
        MessageBox(strTips);
    }

    EndWaitCursor();
    g_bEnableRefreshTreeView = TRUE;
}


void CMainFrame::OnUpdateAppAbout(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (pCmdUI->m_pMenu != NULL)
    {
        CString show_cs;
        if(m_special_customer == 0)
            pCmdUI->SetText(_T("Update T3000"));
        else
        {
            pCmdUI->SetText(_T("Update  ") + cs_special_name);
        }
    }
}
