#pragma once
/********************************************************************
	created:	2010/12/23
	created:	23:12:2010   9:25
	filename: 	..\T3000\TStatScanner.h
	file path:	...\T3000
	file base:	TStatScanner
	file ext:	h
	author:		ZGQ
	
	purpose:	扫描连接在计算机上的所有TStat设备，包括串口和网口
*********************************************************************/
#include "TStatBase.h"
#include "TStat_Dev.h"
#include "TStat_Net.h"
#include "MainFrm.h"
#include "../SQLiteDriver/CppSQLite3.h"

#define WM_ADDTREENODE WM_USER + 2000

struct _ComDeviceInfo
{
	CTStat_Dev*	m_pDev;
	BOOL				m_bConflict;		// 是否与其他设备有冲突,TRUE 有，FALSE 无
	int					m_nTempID;      // 临时ID，在scan过程中，为了能正常读写设备
	int					m_nSourceID;		// 原始的ID, ....

	// added for NC table
	int					m_nCoolingSP;
	int					m_nSP;
	int					m_nHeatingSP;
	int					m_nTemprature;
	int					m_nMode;
	int					m_nOutPutStatus;
	int					m_nNightHeatDB;
	int					m_nNightCoolDB;
	
	int					m_nNightHeatSP;
	int					m_nNightCoolSP;
	int					m_nOccupied;

	CString				m_tstatip;//scan
	CString				m_tstatport;//scan

	_ComDeviceInfo()
	{
		m_pDev = NULL;
		m_bConflict = FALSE;
		m_nTempID = -1;
		m_nSourceID = 1;

		m_nCoolingSP=0;
		m_nSP=0;
		m_nHeatingSP=0;
		m_nTemprature=0;
		m_nMode=0;
		m_nOutPutStatus=0;
		m_nNightHeatDB=0;
		m_nNightCoolDB=0;
		m_nOccupied=0;
	}
	
};

struct _NetDeviceInfo
{	
	CTStat_Net*	m_pNet;
	BOOL				m_bConflict;		// 是否与其他设备有冲突 TRUE 有，FALSE 无
	DWORD			m_dwIPDB;			// 
	DWORD			m_dwIPScan;		// 
	_NetDeviceInfo()
	{
		m_pNet = NULL;
		m_bConflict = FALSE;
		m_dwIPDB = 0; 
		m_dwIPScan = 0; 
	}
};



class CScanDlg;
class CTStatScanner
{
public:
	CTStatScanner(void);
	~CTStatScanner(void);

	// 搜索网口设备
 	BOOL ScanNetworkDevice(); 

    // 搜索网口下的串口设备
    BOOL ScanTCPtoRS485SubPort();

 	// 搜索串口设备
 	BOOL ScanComDevice();

    BOOL ScanDetectComData();
	BOOL ScanRemoteIPDevice(); //扫描 远程设备;
	BOOL ScanBacnetMSTPDevice();//扫描Bacnet MSTP 设备;
	void SetComPort(int nCom);
	BOOL CheckTheSameSubnet(CString strIP);
public:
	// 释放资源
	void		Release();
	// 是否正在搜索串口设备
	BOOL		IsComScanRunning();
	// 搜索函数
	void		background_binarysearch(int nComPort, int nItem,int nbaudrate);

	void modbusip_to_modbus485(int nComPort,int nBaudrate, LPCTSTR s_ipaddr,int n_tcpport, unsigned int parents_serial, int list_count, BYTE devLo, BYTE devHi, int nindex_value = 0);


	// bForTstat = TRUE : scan tstat, = FALSE : scan NC
	void		binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo=1, BYTE devHi=254, int nItem = 0,int nbaudrate = 19200);
	// bForTstat = TRUE : scan tstat, = FALSE : scan  MINI Pannel
	//void		MINI_binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo=1, BYTE devHi=254,int NET_COM=1);
	void        OneByOneSearchforComDevice(int nComPort, bool bForTStat=FALSE, BYTE devLo=1, BYTE devHi=254);
	// 校验
	BOOL		binary_search_crc(int a);

	BOOL m_thesamesubnet;
	// 获得计算机上的所有串口,返回串口数量
	//int GetAllComPort();

	// 在开始搜索之前，设置当前的搜索类型，是scan com or net
	// True = scan com; FALSE = scan net
	void SetScanType( BOOL bScanCom);

	void SetParentWnd(CWnd* pParent);
	void SetOldExsitSerial(unsigned int * serialarray, int nsize);
	void AddNetToGrid();	
	void AddComDeviceToGrid();

	void SendScanEndMsg();

	// 打开串口
	BOOL OpenCom(int nCom);

	BOOL IsRepeatedID(int nID);
	
	void ResetRepeatedID();
	// 从数据库中取出所有的node，从All_Nodes表
	int GetAllNodeFromDataBase();
	// 找到冲突
	void FindComConflict();
	void FindNetConflict();
	// 解决冲突
	void  ResolveComConflict();
	void  ResolveNetConflict();
	// 把新找到的tstat写到数据库All_Nodes表
	void AddNewTStatToDB();
	void AddNewNetToDB();
	//  把一个tstat写到数据库里 All_Nodes表
	void WriteOneDevInfoToDB( _ComDeviceInfo* pInfo);
	void WriteOneNetInfoToDB( _NetDeviceInfo* pInfo);
	/////////////////////////////////////////////////////////////////////////
	void CompareNetToComConflict();
	/////////////////////////////////////////////////////////////////////////
	void GetBuildingName();

	BOOL IsAllScanFinished();

	void StopScan();

	int AddNCToList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind);
	
	void ScanOldNC(BYTE devLo, BYTE devHi);

	//////////////////////////////////////////////////////////////////////////
	// 搜索到的设备	写入数据库时，给他们写上subnet name这一项，在tree node显示时有用
	

	
	void SetSubnetInfo(vector<Building_info>& szSubnets);
	void SetBaudRate(const CString& strBaudrate);
	
	int ScanSubnetFromEthernetDevice(); //当检测到minipanel 后,在结束所有串口的网络扫描后，单独对此类设备扫描下面是否有连接其他设备;

	void Initial_Scan_Info();
    void NetWork_Sub_Scan_Info();
	void ScanAll();
	void WaitScan();//scan
	int  m_scantype;
	void CombineScanResult();

	void ShowNetScanInfo(const CString& strInfo);
	void ShowComScanInfo(const CString& strInfo);
	void ShowBacnetComScanInfo(const CString& strInfo);

	void  ReadNCTable(_NetDeviceInfo* pNCInfo);
	
	void  AddNodeToTable(_NetDeviceInfo* pNCInfo, unsigned short* pNode);




	BOOL ReadOneCheckOnline(int nCOMPort); // 读255，确定是否只有一个Tstat连接。

	BOOL IsNetDevice(const CString& strDevType);
    bool m_saving_data;
    int com_count; // 记录com 和常规的个数;
public:
    bool is_delete_tstat_scanner ;
	unsigned int * exsit_serial_array;
	int serial_array_length;
	//vector<CTStat_Dev*>		m_szTstatScandRet;
	//vector<CTStat_Net*>		m_szNCScanRet;
	vector<_ComDeviceInfo*>		m_szTstatScandRet;
	vector<_NetDeviceInfo*>		m_szNCScanRet;
	vector<_NetDeviceInfo*>		m_szNCScanRet2; // NC find by com port
	vector<CString>				m_bacnetComs;
	vector<CString>				m_szComs;
	CEvent*							m_eScanComEnd;
	//CEvent*							m_eScanComOneByOneEnd;
	CEvent*							m_eScanNCEnd;
    CEvent*                     m_eScan_tcp_to_485_End;

	CEvent*							m_eScanOldNCEnd;
	CEvent*						m_eScanBacnetIpEnd;
	CEvent*						m_eScanRemoteIPEnd;
	//CEvent*						m_eScanBacnetMstpEnd;

	BOOL								m_bStopScan;					// 强制结束SCAN，由用户选择exit时，停止两个搜索线程
	BOOL								m_bNetScanFinish;			// 网络scan完成
	BOOL m_isChecksubnet;
	BOOL m_bCheckSubnetFinish;
	int									m_nBaudrate;
	int									m_nComPort;
	CWnd*							m_pParent;	
	//CWinThread*					m_pScanBacnetMstpThread;
	CWinThread*					m_pScanRemoteIPThread;
	HANDLE					m_pScanBacnetIPThread;
	CWinThread*					m_pScanNCThread;
	CWinThread*                  m_pCheckSubnetThread;
	CWinThread*					m_pScanTstatThread;

    CWinThread*					m_pScanTCP_to_485Thread;
	//CWinThread*					m_pScanTstatOneByOneThread;
	//CWinThread*					m_pWaitScanThread;
	bool						m_com_scan_end;

	CString						m_strBuildingName;
	CString						m_strSubNet;
	CString						m_strFloorName;
	CString						m_strRoomName;


	CString                     m_device_ip_address;
	int							m_device_ip_port;
	int							m_device_baudrate;
	int							m_device_com_port;
	unsigned int				m_parent_serialnum;
    static DWORD WINAPI   _ScanBacnetMSTPThread(LPVOID lpVoid);
    static	DWORD WINAPI   ScanComThreadNoCritical(LPVOID lpVoid);
    static	DWORD WINAPI   ScanTCPSubPortThreadNoCritical(LPVOID lpVoid);
    unsigned int               scan_com_value;
protected:
	BOOL								m_bComScanRunning;		// 是否是Com scan, TRUE = scan com, FALSE = scan net
	int									m_ScannedNum;


	CppSQLite3DB m_SqliteDBBuilding;
	CppSQLite3Table m_table;
	CppSQLite3Query m_q;



	//vector<int>						m_szRepeatedID;
	int									m_szRepeatedID[255];				// 记录重复ID，下标表示现在的ID，内容表示老ID。如果出现重复，那么该下标内容不为0。
	//	vector <binary_search_result> m_szNCScanRet;				////background thread search result
	//	vector <binary_search_result> m_szTstatScandRet;			////background thread search result

	vector<CTStat_Net*>		m_szNetNodes;
	vector<CTStat_Dev*>		m_szComNodes;

	vector<_NetDeviceInfo*>		m_szNetConfNodes;			// 冲突列表
	vector<_ComDeviceInfo*>		m_szComConfNodes;
	
	//vector<_ComDeviceInfo*>		m_szTstatForNC;
	



	//BOOL							m_bAllScanFinish;	
 
 
	vector<Building_info>		m_szSubnetsInfo;
	CString m_ip;
	CString m_port;

	void writetxt();
	CStdioFile*					m_pFile;
	CString						m_strFileINI;


};

