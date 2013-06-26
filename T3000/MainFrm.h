
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "WorkspaceBar.h"
#include "IODisplayBar.h"
#include "GraphicView.h"
#include "TrendLogView.h"
#include "RefreshTreeThread.h"
//#include "TStatScanner.h"
/*
#define	ID_BUILDING_INFO	100
#define	ID_RW_INFO			101
#define	ID_PROTOCOL_INFO	102
*/

#include "CM5/DialogCM5.h"  //CM5
#include "T3/DialogT3.h"	//T3
#include "MiniPanel/DialgMiniPanel.h" //Mini Panel
#include "AirQuality/AirQuality.h"//AirQuality

<<<<<<< HEAD
#include "MBP.h"
#include "MbPoll.h"

#define NUMVIEWS 10
=======
#define NUMVIEWS 11
>>>>>>> 2e9ddba20a1940f319dd2ab81ecc63f9fd73e5d1




	

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
	int nEPsize;
	CString strImgPathName;

	
}tree_product;///////////////////////////////////////////////////////////////////////////////



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

class CTStatScanner;
class CScanDbWaitDlg;
class CMainFrame : public CFrameWndEx
<<<<<<< HEAD
{
	
private:
	CMbp* m_pDlg;
	BOOL m_bDialogOpen;
	LONG OnDlgClose(UINT wParam, LONG lParam);

=======
{	
>>>>>>> 2e9ddba20a1940f319dd2ab81ecc63f9fd73e5d1
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	void OnToolErease();
	void OnToolFresh();
	void OnToolRefreshLeftTreee();
	void EnterConnectToANode();
public:

    CView * m_pViews[NUMVIEWS];
    UINT m_nCurView;    
	void InitViews();

public://for scan
	int m_nScanType;
	CString m_strsubNetSelectedScan;
 	HANDLE m_hCurCommunication;
    BOOL m_bScanFinished;
	BOOL m_bScanALL;


// Attributes
public:
	CImageTreeCtrl*	m_pTreeViewCrl;
	int						m_nIpPort;
	CString				m_strIP;
	int						m_nBaudrate;
	CString				g_strIpAdress;
	vector<Building_info>	m_subNetLst;
	vector<CString>			m_buildingLst;
	 vector<CString> m_szComs;
     //vector <HTREEITEM> m_rootLst;//for every building name
	//HTREEITEM treem[20];
	vector <tree_floor>		m_floorLst;//for every building name
	vector <tree_room>		m_roomLst;//for every room name	
	vector <background_infor> m_backgroundLst;
	vector <tree_product>	m_product;//for every product leaf
	_ConnectionPtr				m_pCon;//for ado connection
	_RecordsetPtr				m_pRs;//for ado 
	
	CString m_strCurSubBuldingName;
	CString m_strCurMainBuildingName;
	int m_nCurSubBuildingIndex;
	//
public://scan
	CTStatScanner* m_pScanner;
	CWinThread *m_pThreadScan;
	CWinThread* m_pFreshMultiRegisters;

	CWinThread* m_pFreshTree;

	vector <binary_search_result> m_binary_search_networkcontroller_background_thread;	////background thread search result
	vector <binary_search_result> m_binary_search_product_background_thread;				////background thread search result
	
	
	CScanDbWaitDlg*		m_pWaitScanDlg;	

public:
	void OnLabel();
	void OnLabe2();
	void OnLabe3();

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

	CMFCToolBarImages		m_UserImages;
public: 
	CWorkspaceBar			m_wndWorkSpace;
	CMFCStatusBar			m_wndStatusBar;
		

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnUpdateCheckBuildingBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheckIOPane(CCmdUI* pCmdUI);
	afx_msg void OnScanDevice();
	afx_msg void SaveConfigFile();
	afx_msg void OnHelp();
	afx_msg void OnCaliBrate();
	afx_msg void OnGloabalOverrides();
	afx_msg LRESULT OnFreshStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUserMannageMentUpdate(CCmdUI *pCmdUI);
	afx_msg LRESULT OnAddTreeNode(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	
	void OnMBP();
	void SwitchToPruductType(int nIndex);
	void OnFileOpen();
	void OnLoadConfigFile();
	void OnBatchFlashHex();
	void OnConnect();
	void OnDisconnect();

	void OnAddBuildingConfig();
	void OnAllNodeDatabase();

	void OnCheckBuildingBar();
	void ClearBuilding();
	void Scan_Product();

	void OnCheckIOPane();
	void ScanTstatInDB(void);
	void SwitchToGraphicView();
	void SetPaneConnectionPrompt(CString strInfo);
	void OnHTreeItemSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//void OnHTreeItemKeyDownChanged(NMHDR* pNMHDR, LRESULT* pResult);
	void OnHTreeItemEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnHTreeItemBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	void  OnHTreeItemClick(NMHDR *pNMHDR, LRESULT *pResult);
	
			
			BOOL ValidAddress(CString sAddress);
	BOOL ValidAddress(CString sAddress,UINT& n1,UINT& n2,UINT& n3,UINT& n4);
	 
	void CheckConnectFailure(const CString& strIP);// 检查失败的原因，并给出详细的提示信息
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL ConnectSubBuilding(Building_info build_info);



	//scan funtion:
	void background_binarysearch_netcontroller();
	void binarySearchforview_networkcontroller(BYTE devLo=1, BYTE devHi=254);
	void binarySearchforview(BYTE devLo=1, BYTE devHi=254);
	void background_binarysearch();
	BOOL binary_search_crc(int a);
	void scanForTstat();
	void AddScanedDeviceToDatabase(Building_info buildInfo);
	int find_Address_towrite ();

	afx_msg void OnDestroy();
	void OnChangeAdminPassword();
	void OnUserMannageMent();
	void OnUserLogin();
	void GetIONanme();
	void OnImportDatase();
	void ReFresh();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GetIPbyHostName(CString strHostName,CString& strIP);

	//////////////////////////////////////////////////////////////////////////
	//  below Added by zgq; from 2010-11-29; 
	//

	void DoConnectToANode( const HTREEITEM& hTreeItem );
	void InitTreeNodeConn();
	void GetAllTreeItems( HTREEITEM hItem, vector<HTREEITEM>& szTreeItems  );
	//void EnableRefreshTreeView(BOOL bEnable);
	void DoFreshAll();
	void RefreshTreeView();
	// suspend refresh tree view thread, when flash or other operation need read or write large data by Modbus.
	void SuspendRefreshThread();
	void ContinueRefreshThread();
	CString GetDeviceClassName(int nClassID);
	CString GetScreenName(int nSerialNumber, int nModbusID);
protected:
	void UpdateAllNodesInfo(HTREEITEM& htiEdit);
	void UpdateFloorNode(HTREEITEM& htiEdit);
	void UpdateRoomNode(HTREEITEM& htiEdit);
	void UpdateDeviceNodes(HTREEITEM& htiEdit);
	// for record tree node 
	void SaveTreeNodeRecordToReg(HTREEITEM& htiCurSel);
	BOOL WriteValueToReg(CRegKey& reg, HTREEITEM& htiItem);
	void SelectTreeNodeFromRecord();
	HTREEITEM GetLastSelNodeFromRecord(CRegKey& reg, HTREEITEM& htiRoot);
	HTREEITEM SearchItemByName(HTREEITEM& htiRoot, const CString& strItemName);
	BOOL ImportDataBaseForFirstRun();
	CString GetCurrentFileVersion();
	CString ReadFileVersionFromRegister(CRegKey& reg);
//protected://lsc20111213修改
public:
	BOOL m_bEnableRefreshTreeView;
	CRefreshTreeThread*	m_pRefreshThread;

	CString						m_strCurSelNodeName;  // 记录当前点击树节点的name
	BOOL m_isCM5;
	//HTREEITEM				m_htiCurSel;  // 记录当前点击树节点
	CString						m_strFileVersion;

	void EnableMyToolBarButton(int nIndex, int nID, BOOL bEnable);
public:
//	afx_msg void OnDatabaseTemcoproducts();

	void ReadExcel();

	unsigned short tempchange[512];//for tstat6
	unsigned short temptstat6[512];
	unsigned short nFlag;
	
	void Tstat6_func();
	void JudgeTstat6dbExist(CString strtable,CString strsql);
	void Updata_db_tstat6(unsigned short imodel);
	BOOL FistWdb;
	//afx_msg void OnFileIsptool();
	void JudgeTstat6SliderExist(CString strtable,CString strsql);
	void UpdataSlider(int Flag);
	void ReadSlider();

	UINT FlagSerialNumber;
	void Treestatus();
	static	DWORD WINAPI Get_All_Dlg_Message(LPVOID lpVoid);
	static	DWORD WINAPI Translate_My_Message(LPVOID lpVoid);
<<<<<<< HEAD

	afx_msg void OnDatabaseMbpoll();
	CMbPoll* mbPoll;
	bool mbPollDlgOpen;
protected:
	afx_msg LRESULT OnMbpollClosed(WPARAM wParam, LPARAM lParam);
=======
	afx_msg void OnDatabaseIonameconfig();
>>>>>>> 2e9ddba20a1940f319dd2ab81ecc63f9fd73e5d1
};


