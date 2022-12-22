
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "WorkspaceBar.h"
#include "GraphicView.h"
#include "TrendLogView.h"
#include "RefreshTreeThread.h"
//#include "TStatScanner.h"
/*
#define	ID_BUILDING_INFO	100
#define	ID_RW_INFO			101
#define	ID_PROTOCOL_INFO	102
*/
#include "global_function.h"
#include "T3/DialogT3.h"	//T3

#include "AirQuality/AirQuality.h"//AirQuality


#include "ConfigFileHandler.h"
#include "DialogInfo.h"


const int DLG_T3000_VIEW = 0;
const int DLG_NETWORKCONTROL_VIEW = 1;
const int DLG_GRAPGIC_VIEW = 2;
const int DLG_TRENDLOG_VIEW =3;
const int DLG_DIALOGCM5_VIEW = 4;

const int DLG_DIALOGMINIPANEL_VIEW = 6;
const int DLG_AIRQUALITY_VIEW = 7;
const int DLG_LIGHTINGCONTROLLER_VIEW = 8;
const int DLG_HUMCHAMBER = 9;
const int DLG_CO2_VIEW = 10;
const int DLG_CO2_NET_VIEW=11;
const int DLG_BACNET_VIEW = 12;
//T3-Serials
const int DLG_DIALOGT3_VIEW = 5;
const int DLG_DIALOGT38I13O_VIEW=13;
const int DLG_DIALOGT332AI_VIEW=14;
const int DLG_DIALOGT38AI8AO=15;
const int DLG_DIALOGT36CT=16;
const int DLG_DIALOGT3PT10=17;
const int DLG_DIALOG_PRESSURE_SENSOR=18;
const int DLG_DIALOG_DEFAULT_BUILDING = 19;
const int DLG_DIALOG_TEMP_HUMSENSOR = 20;
const int DLG_DIALOG_DEFAULT_T3000_VIEW = 21;
const int DLG_DIALOG_T3_INPUTS_VIEW =22;
const int DLG_DIALOG_T3_OUTPUTS_VIEW =23;
const int DLG_DIALOG_CUSTOM_VIEW = 24;
const int DLG_DIALOG_TSTAT_INPUT_VIEW=25;
const int DLG_DIALOG_TSTAT_OUTPUT_VIEW = 26;
const int DLG_DIALOG_BOATMONITOR = 27;
const int DLG_DIALOG_BTUMETER = 28;
const int DLG_DIALOG_POWERMETER = 29;
const int DLG_DIALOG_CO2_NODE = 30;
const int DLG_DIALOG_ZIGBEE_REPEATER = 31;
const int DLG_DIALOG_TSTAT_AQ = 32;
const int DLG_DIALOG_THIRD_PARTY_BAC = 33;
const int DLG_DIALOG_BUILDING_MANAGEMENT = 34;
const int DLG_DIALOG_AIRFLOW = 35;
const int DLG_DIALOG_TRANSDUCER = 36;
const int NUMVIEWS = 37;


#define  WM_REFRESH_TREEVIEW_MAP WM_USER + 2008
extern int g_gloab_bac_comport;
extern int g_gloab_bac_baudrate;
const int REGISTER_USE_ZIGBEE_485 = 640;
union UNION_INPUT_NAME{
	unsigned short reg_value[4];
	unsigned char char_name[8];
};

typedef struct _com_port_tree
{//for vector
	HTREEITEM building_item;//building name
	HTREEITEM serial_port_item;	//floor name
	HTREEITEM each_port_item;	//floor name
	CString strComPortName;
}com_port_tree;


typedef struct _tree_floor///////////////////////////////////////////////////////////////////////////////
{//for vector
	HTREEITEM building_item;//building name
	HTREEITEM floor_item;	//floor name
	CString strFloorName;
}tree_floor;///////////////////////////////////////////////////////////////////////////////
typedef struct _tree_room///////////////////////////////////////////////////////////////////////////////
{//for vector
	HTREEITEM building_item;//building name
	HTREEITEM floor_item;	//floor name
	HTREEITEM room_item;	//Room name
	CString strFloorName;
	CString strRoomName;
}tree_room;///////////////////////////////////////////////////////////////////////////////

typedef struct _background_infor_TAG
{
	HTREEITEM building_item;//building name
	CString Screen_name;
	CString Background_Bmp;
	CString building_name;
}background_infor;

typedef struct _BUILDING_TAG
{
	HTREEITEM hbuilding_item;
	HANDLE hCommunication;
	CString strBuildingName;//sub building name
	CString strProtocol;
	CString strIp;
	CString strComPort;
	CString strIpPort;
	CString strBaudRate;
	CString strMainBuildingname;
	CString strConnectionType;
}Building_info;

typedef struct ext_info
{
	int virtual_device;  // 0  default    1  verture device
	int mini_type;       //��Ӧ Setting �����  mini_type
};

typedef struct tree_io_info
{
	HTREEITEM h_tree_item; //�ӽڵ������
	HTREEITEM h_parent_item; // ���ڵ�ľ��
	int h_item_type; // input or output or ......
	int capacity;   //������
	int already_use; //�Ѿ�ʹ��;
}tree_sub_io;

typedef struct _tree_product//////////////////////
{//for vector 
 //every product leaf of the leaf
    Building_info  BuildingInfo;
    HTREEITEM product_item;//product name 
    unsigned int serial_number;//the id of product ;Modbus protocol need
    int product_class_id;//maybe 1 is Tstat;2 is led;3 is minipanel;and others
    int baudrate;
    int product_id;
    float software_version;
    float hardware_version;
    int nhardware_info;  ////bit0 zigbee   bit1 wifi
    CString strImgPathName;
    int protocol;
    unsigned int ncomport;
    bool status;
    bool status_last_time[5];
    CString NetworkCard_Address;
    CString NameShowOnTree;
    CString Custom;
    unsigned int note_parent_serial_number;
    unsigned char panel_number;
    unsigned int object_instance;
    UCHAR  subnet_port;  //�豸������һ���˿ڻظ������ġ� 1- MainPort      2-ZigbeePort      3-SubPort
    UCHAR  subnet_baudrate;   //���豸���õĲ�����; ��֮ǰ����Ĳ�������Ŷ�Ӧ
    UCHAR  expand; //�Ƿ����νṹչ��; 1ΪĬ��չ�� ���� ��2 Ϊչ��       2 Ϊ�۵�
	ext_info m_ext_info;
	tree_sub_io sub_io_info[TREE_MAX_TYPE];
	
}tree_product;///////////////////////////////////////////////////////////////////////////////
//

typedef struct _binary_search_result
{
	int id;//the id of product ;Modbus protocol need
	int product_class_id;//7 product model
	int baudrate;
	unsigned int serialnumber;//0,1,2,3
	float hardware_version;//8
	float software_version;
	int nEPsize;
	
}binary_search_result;



class CViewClientToolBar : public CMFCToolBar
{
public:
    HWND Get_Tip_HWND(){
    return  m_pToolTip->m_hWnd;
    }
};



class CTStatScanner;
class CScanDbWaitDlg;
class CMainFrame : public CFrameWndEx
{	
private:
	 
	BOOL m_bDialogOpen;
//	LONG OnDlgClose(UINT wParam, LONG lParam);
public: // create from serialization only
	void OnLoadConfigFile(); //Load File
    void SaveConfigFile();
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	void OnToolErease();
	void OnToolFresh();
	void OnToolRefreshLeftTreee();
	void BuildingComportConfig();
//	CMulitithreadSocket m_wskServer;
	BOOL m_bServer;
public:

    CView * m_pViews[NUMVIEWS];
    UINT m_nCurView;    
	void InitViews();
	CConfigFileHandler		m_cfgFileHandler;
	CString					m_strCfgFilePath;
public://for scan
	int m_nScanType;
	CString m_strsubNetSelectedScan;
 	HANDLE m_hCurCommunication;
    BOOL m_bScanFinished;
	BOOL m_bScanALL;
	vector <CString> m_vector_comport;
	BOOL m_frist_start;
// Attributes
public:
	CImageTreeCtrl*	m_pTreeViewCrl;
	int						m_nIpPort;
	CString				m_strIP;
	int						m_nBaudrate;
	int					m_building_com_port;
	CString				g_strIpAdress;
	vector<Building_info>	m_subNetLst;
	vector<CString>			m_buildingLst;
	 vector<CString> m_szComs;
     //vector <HTREEITEM> m_rootLst;//for every building name
	//HTREEITEM treem[20];
	vector <com_port_tree> m_comportlist;
	vector <tree_floor>		m_floorLst;//for every building name
	vector <tree_room>		m_roomLst;//for every room name	
	vector <background_infor> m_backgroundLst;
	vector <tree_product>	m_product;//for every product leaf
// 	_ConnectionPtr				m_pCon;//for ado connection
// 	_RecordsetPtr				m_pRs;//for ado 
	 
    tree_product m_current_tree_node ;
    tree_product m_lasttime_tree_node;
	CString m_strCurSubBuldingName;
	Building_info m_CurSubBuldingInfo;
	CString m_strCurMainBuildingName;
	int m_nCurSubBuildingIndex;
	//
public://scan
	CTStatScanner* m_pScanner;
	CWinThread *m_pThreadScan;
	CWinThread* m_pFreshMultiRegisters;

	CWinThread* m_pFreshTree;
	CWinThread* m_pCheck_net_device_online;


	
	
	CScanDbWaitDlg*		m_pWaitScanDlg;	


// Operations
public:
	CMFCStatusBar& GetStatusBar ()
	{
	return m_wndStatusBar;
	}
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	 CMFCMenuBar				m_wndMenuBar;
	CMFCToolBar				m_wndToolBar;

	CViewClientToolBar		m_testtoolbar;
	//CMFCToolBarImages		m_UserImages;
public: 
	CWorkspaceBar			m_wndWorkSpace;
	CMFCStatusBar			m_wndStatusBar;
		

// Generated message map functions
protected:
    void ShowSplashWnd(int nMillisecond);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	 
	afx_msg void OnUpdateCheckBuildingBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheckIOPane(CCmdUI* pCmdUI);
	afx_msg void OnScanDevice();

	afx_msg void OnHelp();
	
	afx_msg LRESULT OnCreateStatusBarWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFreshStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUserMannageMentUpdate(CCmdUI *pCmdUI);
	afx_msg LRESULT OnAddTreeNode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  AllWriteMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HandleWriteNewDevice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Refresh_RX_TX_Count(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Retry_Connect_Message(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT ReConnect_Message(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Delete_Write_New_Dlg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  ReadConfigFromDeviceMessageCallBack(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	BOOL  CheckDeviceStatus(int refresh_com);
	void  CheckDuplicate();
    void  CheckIPDuplicate();
	void  Show_Wait_Dialog_And_SendConfigMessage();
    void  Show_Wait_Dialog_And_ReadBacnet(int ncontrol);
	static DWORD WINAPI  Send_Set_Config_Command_Thread(LPVOID lpVoid);
    static DWORD WINAPI  Mul_Ping_Thread(LPVOID lpVoid);
    int m_read_control;   // 0 Ĭ��ȫ����ȡ     1  �������ʱ��ʹ�� ;
	static DWORD WINAPI  Read_Bacnet_Thread(LPVOID lpVoid);
	static DWORD WINAPI  Read_Modbus_10000(LPVOID lpVoid);
	static DWORD WINAPI  Write_Modbus_10000(LPVOID lpVoid);
	static DWORD WINAPI  DetectYabbeThread(LPVOID lpVoid);
    static DWORD WINAPI  Write_Modbus_tstat_cfg(LPVOID lpVoid);
	static DWORD WINAPI  Write_Modbus_ConfigFile(LPVOID lpVoid);
	void SwitchToPruductType(int nIndex);
	void OnFileOpen();
    
	
	void OnBatchFlashHex();
	void OnConnect();
	void OnDisconnect();

	void OnAddBuildingConfig();
	void OnAllNodeDatabase();

	void OnCheckBuildingBar();
	void ClearBuilding();
	void Scan_Product();
	void LoadProductFromDB();
	void OnCheckIOPane();
	void GetExtProductInfo(tree_product& m_product_temp, CString temp_ext);
	void ScanTstatInDB(void);
	void DeleteConflictInDB();
	void SwitchToGraphicView();
	void SetPaneConnectionPrompt(CString strInfo);
	void OnHTreeItemSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//void OnHTreeItemKeyDownChanged(NMHDR* pNMHDR, LRESULT* pResult);
	void OnHTreeItemEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnHTreeItemBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	void  OnHTreeItemClick(NMHDR *pNMHDR, LRESULT *pResult);
	void  OnHTreeMessageSeletedChanged(NMHDR *pNMHDR, LRESULT *pResult);
			
			//BOOL ValidAddress(CString sAddress);
	BOOL ValidAddress(CString sAddress,UINT& n1,UINT& n2,UINT& n3,UINT& n4);
	 
	void CheckConnectFailure(const CString& strIP);// ���ʧ�ܵ�ԭ�򣬲�������ϸ����ʾ��Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL ConnectSubBuilding(Building_info build_info);
	BOOL ConnectDevice(LPCTSTR ip_address,int nport);
	BOOL ConnectDevice(tree_product tree_node);

	//scan funtion:


	afx_msg void OnDestroy();
	void OnChangeAdminPassword();
	void OnUserMannageMent();
	void OnUserLogin();
	void GetIONanme();
	 
	void ReFresh();

	CString GetFWVersionFromFTP(CString ProductName);
 
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GetIPbyHostName(CString strHostName,CString& strIP);

	//////////////////////////////////////////////////////////////////////////
	//  below Added by zgq; from 2010-11-29; 
	//
	void DoConnectToANode( const HTREEITEM& hTreeItem );
	//void InitTreeNodeConn();
	void GetAllTreeItems( HTREEITEM hItem, vector<HTREEITEM>& szTreeItems  );
	//void EnableRefreshTreeView(BOOL bEnable);
	void DoFreshAll();
	//void RefreshTreeView();
	LRESULT  CMainFrame::HandleIspModedivice(WPARAM wParam, LPARAM lParam);
	LRESULT  CMainFrame::HandleDuplicateID(WPARAM wParam, LPARAM lParam);
	LRESULT  CMainFrame::RefreshTreeViewMap(WPARAM wParam, LPARAM lParam);
	LRESULT  CMainFrame::Message_Scan_Product(WPARAM wParam,LPARAM lParam);
	// suspend refresh tree view thread, when flash or other operation need read or write large data by Modbus.
	void SuspendRefreshThread();
	void ContinueRefreshThread();
	CString GetDeviceClassName(int nClassID);
	CString GetScreenName(int nSerialNumber, int nModbusID);
protected:


	// for record tree node 
	//void SaveTreeNodeRecordToReg(HTREEITEM& htiCurSel);
	//BOOL WriteValueToReg(CRegKey& reg, HTREEITEM& htiItem);
	//void SelectTreeNodeFromRecord();
	HTREEITEM GetLastSelNodeFromRecord(CRegKey& reg, HTREEITEM& htiRoot);
	HTREEITEM SearchItemByName(HTREEITEM& htiRoot, const CString& strItemName);
	BOOL ImportDataBaseForFirstRun();
	CString GetCurrentFileVersion();
	CString ReadFileVersionFromRegister(CRegKey& reg);
//protected://lsc20111213�޸�
public:
	BOOL m_bEnableRefreshTreeView;
	CRefreshTreeThread*	m_pRefreshThread;

	CString						m_strCurSelNodeName;  // ��¼��ǰ������ڵ��name
	BOOL m_isCM5;
	BOOL m_isMiniPanel;

	//HTREEITEM				m_htiCurSel;  // ��¼��ǰ������ڵ�
	CString						m_strFileVersion;

public:
	unsigned short tempchange[512];//for tstat6
	unsigned short temptstat6[512];
	unsigned short nFlag;
	
    CString m_strLogoFileName;
	
	BOOL FistWdb;
	//afx_msg void OnFileIsptool();

	

	UINT FlagSerialNumber;

	static	DWORD WINAPI Get_All_Dlg_Message(LPVOID lpVoid);
	static	DWORD WINAPI Translate_My_Message(LPVOID lpVoid);

	static DWORD WINAPI   retry_connect(LPVOID lpVoid);
 //   static DWORD WINAPI ConnectToTreeNode(LPVOID);
	afx_msg void OnDatabaseIonameconfig();
	afx_msg void OnDatabaseMbpoll();
	afx_msg void OnToolIsptoolforone();
	 
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	 
	bool mbPollDlgOpen;

public:
	CString					m_strPingIP;	
	void ShowDebugWindow();
	afx_msg void OnControlMain();
	afx_msg void OnControlInputs();
	afx_msg void OnControlPrograms();
	afx_msg void OnControlOutputs();
	afx_msg void OnControlVariables();
	afx_msg void OnControlWeekly();
	afx_msg void OnControlAnnualroutines();
	afx_msg void OnMiscellaneousLoaddescriptors();
	afx_msg void OnMiscellaneousUpdatemini();
	afx_msg void OnControlControllers();
	afx_msg void OnControlScreens();
	afx_msg void OnControlMonitors();
    afx_msg void OnControlPanel();
	afx_msg void OnHelpGetlanguageconfigfile();
	afx_msg void OnLanguageEnglish();
	afx_msg void OnLanguage34010();
	afx_msg void OnLanguage34006();
	afx_msg void OnToolRegisterviewer();
	afx_msg void OnDatabaseBacnettool();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnPaint();
	afx_msg void OnControlAlarmLog();
	afx_msg void OnMenuCheckupdate();
	afx_msg void OnControlRemotePoint();

    afx_msg void OnUpdateConnect2(CCmdUI *pCmdUI);
    afx_msg void OnUpdateDisconnect2(CCmdUI *pCmdUI);
    afx_msg void OnUpdateScanDevice(CCmdUI *pCmdUI);
    afx_msg void OnUpdateAddBuildingConfig(CCmdUI *pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg LRESULT CMainFrame::PingDevice(WPARAM wParam, LPARAM lParam);
	void OnTestPing(const CString& strIP);
    UINT m_nStyle;
	afx_msg void OnUpdateStatusBar(CCmdUI *pCmdUI);
	afx_msg void OnHelpFeedbacktotemco();
	afx_msg void OnControlCustomerunits();
	
	afx_msg void OnMiscellaneousGsmconnection();
public:
	CString m_sURLToDownload;
	CString m_sFileToDownloadInto;
	CString m_sUserName;
	CString m_sPassword;
	CString       m_sError;
	CString       m_sServer; 
	CString       m_sObject; 
	CString       m_sFilename;
	INTERNET_PORT m_nPort;
	DWORD         m_dwServiceType;
	HINTERNET     m_hInternetSession;
	HINTERNET     m_hHttpConnection;
	HINTERNET     m_hHttpFile;
	int           m_prg_version;
	BOOL          m_bSafeToClose;
	CFile         m_FileToWrite;
	//CWinThread*   m_pThread;
     int  m_lastinterface;
	CString      m_file_firmware_Time;
	CDialogInfo *m_pDialogInfo;
public:
	void Create_Thread_Read_Item(int n_item);
	void Check_Local_TemcoUpdate();
	void Reset_Window_Pos();
    BOOL DownloadFromFTP();
	static void CALLBACK _OnStatusCallBack(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, 
		LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	void OnStatusCallBack(HINTERNET hInternet, DWORD dwInternetStatus, 
		LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	 
	void HandleThreadErrorWithLastError(UINT nIDError, DWORD dwLastError=0);
	void HandleThreadError(UINT nIDError);
	BOOL DownloadThread();
	 
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
	void GetProductFPTAndLocalPath(int ProductModel,CString &FtpPath,CString &ProductFileName); 
	void GetProductFirmwareFTPDirectory(int ProductModel,CString &FtpPath,CString &ProductFileName);
	afx_msg void OnToolDetectonlineproducts();
	afx_msg void OnControlSettings();
	afx_msg void OnUpdateControlMain(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlInputs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlOutputs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlVariables(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlPrograms(CCmdUI *pCmdUI);
    afx_msg void OnUpdateControlPanel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlScreens(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlControllers(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlWeekly(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlAnnualroutines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlMonitors(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlAlarmLog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlTstat(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlSettings(CCmdUI *pCmdUI);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnFileExportregiseterslist();
    afx_msg void OnToolRegistersmaintenancesystem();
    afx_msg void OnToolFlashsn();
 	//virtual void GetMessageString(UINT nID, CString& rMessage) const;
		afx_msg BOOL  OnToolTipNotify(UINT id,NMHDR *Pnmhdr,LRESULT *pResult);
		virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
        afx_msg void OnMove(int x, int y);

    //    BOOL CheckNameFromDevice(CString NameFromDevice);
  // virtual void GetMessageString(UINT nID, CString& rMessage);
        afx_msg void OnToolBootloader();
		afx_msg void OnHelpUsingUpdate();
		afx_msg void OnToolsPsychrometry();
		afx_msg void OnToolsOption();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		void HideBacnetWindow();
		void VirtualDeviceMode();
		void CreateOfflinePrgFile();
		afx_msg void OnModeOnlinemode();
		afx_msg void OnModeOfflinemode();
		afx_msg void OnUpdateModeOnlinemode(CCmdUI *pCmdUI);
		afx_msg void OnUpdateModeOfflinemode(CCmdUI *pCmdUI);
		afx_msg void OnToolsPhchart();
		afx_msg void OnToolsModbustobacnetrouter();
		afx_msg void OnControlModbus();
		afx_msg void OnControlIoNetConfig();
        afx_msg void OnDatabaseLogdetail();
        afx_msg void OnUpdateAppAbout(CCmdUI *pCmdUI);
		void OnDatabaseBuildingManagement();
		afx_msg void OnViewRefresh();
		afx_msg void OnFileNewproject();
#ifndef LOCAL_DB_FUNCTION
		afx_msg void OnUpdateFileNewproject(CCmdUI* pCmdUI);
		
#else
		int    DoConnectDB_TreeNode(const HTREEITEM& hTreeItem);
#endif
};
//DWORD WINAPI  Bacnet_ReadWrite_Message(LPVOID lpVoid);
//tree0412  2017 05 11 fance change
extern vector <bacnet_background_struct> m_backbround_data; // ����ȫ�̴�����Ҫ�����ȡ��һЩ��̨bacnet panel����
#define TVINSERV_BUILDING 		    {tvInsert.item.iImage=0; tvInsert.item.iSelectedImage=0;}
#define TVINSERV_FLOOR	 			{tvInsert.item.iImage=0; tvInsert.item.iSelectedImage=0;}
#define TVINSERV_ROOM				{tvInsert.item.iImage=2; tvInsert.item.iSelectedImage=2;}
#define TVINSERV_TSTAT_DEFAULT 	    {tvInsert.item.iImage=6; tvInsert.item.iSelectedImage=6;}     //Ĭ�ϵĲ�Ʒͼ��
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
#define TVINSERV_T3_NANO    {tvInsert.item.iImage=28;tvInsert.item.iSelectedImage=28;}//zigbeerepeater
#define TVINSERV_PM5E               {tvInsert.item.iImage=30;tvInsert.item.iSelectedImage=30;}//PM5E
#define TVINSERV_THIRD_PARTY        {tvInsert.item.iImage=32;tvInsert.item.iSelectedImage=32;}//�������豸
#define ITEM_MASK				TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT

