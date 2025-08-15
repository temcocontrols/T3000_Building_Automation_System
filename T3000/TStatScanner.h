#pragma once
/********************************************************************
	created:	2010/12/23
	created:	23:12:2010   9:25
	filename: 	..\T3000\TStatScanner.h
	file path:	...\T3000
	file base:	TStatScanner
	file ext:	h
	author:		ZGQ
	
	// Purpose: Scan all TStat devices connected to the computer, including serial ports and network ports
	purpose:	ɨ�������ڼ�����ϵ�����TStat�豸���������ں�����
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
	// Whether there is conflict with other devices, TRUE has, FALSE no
	BOOL				m_bConflict;		// �Ƿ��������豸�г�ͻ,TRUE �У�FALSE ��
	// Temporary ID, during scan process, to enable normal read/write to device
	int					m_nTempID;      // ��ʱID����scan�����У�Ϊ����������д�豸
	// Original ID, ....
	int					m_nSourceID;		// ԭʼ��ID, ....

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
	// Whether there is conflict with other devices TRUE has, FALSE no
	BOOL				m_bConflict;		// �Ƿ��������豸�г�ͻ TRUE �У�FALSE ��
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

	// Search network port devices
 	// ���������豸
 	BOOL ScanNetworkDevice(); 

    // Search serial port devices under network ports
    // ���������µĴ����豸
    BOOL ScanTCPtoRS485SubPort();

 	// Search serial port devices
 	// ���������豸
 	BOOL ScanComDevice();

    BOOL ScanDetectComData();
	// Scan remote devices
	BOOL ScanRemoteIPDevice(); //ɨ�� Զ���豸;
	// Scan Bacnet MSTP devices
	BOOL ScanBacnetMSTPDevice();//ɨ��Bacnet MSTP �豸;
	BOOL ScanThirdPartyBACnetDevice();
	void SetComPort(int nCom);
	BOOL CheckTheSameSubnet(CString strIP);
public:
	// Release resources
	// �ͷ���Դ
	void		Release();
	// Whether searching serial port devices
	// �Ƿ��������������豸
	BOOL		IsComScanRunning();
	// Search function
	// ��������
	void		background_binarysearch(int nComPort, int nItem,int nbaudrate);

	void modbusip_to_modbus485(int nComPort,int nBaudrate, LPCTSTR s_ipaddr,int n_tcpport, unsigned int parents_serial, int list_count, BYTE devLo, BYTE devHi, int nindex_value = 0);


	// bForTstat = TRUE : scan tstat, = FALSE : scan NC
	int		binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo=1, BYTE devHi=254, int nItem = 0,int nbaudrate = 19200);
	// bForTstat = TRUE : scan tstat, = FALSE : scan  MINI Pannel
	//void		MINI_binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo=1, BYTE devHi=254,int NET_COM=1);
	void        OneByOneSearchforComDevice(int nComPort, bool bForTStat=FALSE, BYTE devLo=1, BYTE devHi=254);
	// Validation
	// У��
	BOOL		binary_search_crc(int a);

	BOOL m_thesamesubnet;
	// Get all serial ports on computer, return number of serial ports
	// ��ü�����ϵ����д���,���ش�������
	//int GetAllComPort();

	// Before starting search, set current search type, whether scan com or net
	// �ڿ�ʼ����֮ǰ�����õ�ǰ���������ͣ���scan com or net
	// True = scan com; FALSE = scan net
	void SetScanType( BOOL bScanCom);

	void SetParentWnd(CWnd* pParent);
	void SetOldExsitSerial(unsigned int * serialarray, int nsize);
	void AddNetToGrid();	
	void AddComDeviceToGrid();

	void SendScanEndMsg();

	// Open serial port
	// �򿪴���
	BOOL OpenCom(int nCom);

	BOOL IsRepeatedID(int nID);
	
	void ResetRepeatedID();
	// Get all nodes from database, from All_Nodes table
	// �����ݿ���ȡ�����е�node����All_Nodes��
	int GetAllNodeFromDataBase();
	// Find conflicts
	// �ҵ���ͻ
	void FindComConflict();
	void FindNetConflict();
	// Resolve conflicts
	// �����ͻ
	void  ResolveComConflict();
	void  ResolveNetConflict();
	// Write newly found tstat to database All_Nodes table
	// �����ҵ���tstatд�����ݿ�All_Nodes��
	void AddNewTStatToDB();
	void AddNewNetToDB();
	// Write one tstat to database All_Nodes table
	//  ��һ��tstatд�����ݿ��� All_Nodes��
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
	// When writing discovered devices to database, give them subnet name field, useful for tree node display
	// ���������豸	д�����ݿ�ʱ��������д��subnet name��һ���tree node��ʾʱ����
	

	
	void SetSubnetInfo(vector<Building_info>& szSubnets);
	void SetBaudRate(const CString& strBaudrate);
	
	// When minipanel is detected, after ending all serial port network scans, separately scan this type of device to see if there are other connected devices below
	int ScanSubnetFromEthernetDevice(); //����⵽minipanel ��,�ڽ������д��ڵ�����ɨ��󣬵����Դ����豸ɨ�������Ƿ������������豸;

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




	// Read 255, determine if only one Tstat is connected
	BOOL ReadOneCheckOnline(int nCOMPort); // ��255��ȷ���Ƿ�ֻ��һ��Tstat���ӡ�

	BOOL IsNetDevice(const CString& strDevType);
    bool m_saving_data;
    // Record count of com and regular
    int com_count; // ��¼com �ͳ���ĸ���;
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
	CEvent*						m_eScanThirdPartyBacnetIpEnd;

	// Force end SCAN, stop two search threads when user chooses exit
	BOOL								m_bStopScan;					// ǿ�ƽ���SCAN�����û�ѡ��exitʱ��ֹͣ���������߳�
	// Network scan completed
	BOOL								m_bNetScanFinish;			// ����scan���
	BOOL m_isChecksubnet;
	BOOL m_bCheckSubnetFinish;
	int									m_nBaudrate;
	int									m_nComPort;
	CWnd*							m_pParent;	
	//CWinThread*					m_pScanBacnetMstpThread;
	CWinThread*					m_pScanRemoteIPThread;
	HANDLE					m_pScanBacnetIPThread;
	HANDLE					m_pScanThirdPartyBacnetIPThread;
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
	static DWORD WINAPI   _ScanThirdPartyBacnetThread(LPVOID lpVoid);
    unsigned int               scan_com_value;
protected:
	// Whether it's Com scan, TRUE = scan com, FALSE = scan net
	BOOL								m_bComScanRunning;		// �Ƿ���Com scan, TRUE = scan com, FALSE = scan net
	int									m_ScannedNum;


	CppSQLite3DB m_SqliteDBBuilding;
	CppSQLite3Table m_table;
	CppSQLite3Query m_q;



	//vector<int>						m_szRepeatedID;
	// Record repeated IDs, index represents current ID, content represents old ID. If repetition occurs, then the index content is not 0
	int									m_szRepeatedID[255];				// ��¼�ظ�ID���±��ʾ���ڵ�ID�����ݱ�ʾ��ID����������ظ�����ô���±����ݲ�Ϊ0��
	//	vector <binary_search_result> m_szNCScanRet;				////background thread search result
	//	vector <binary_search_result> m_szTstatScandRet;			////background thread search result

	vector<CTStat_Net*>		m_szNetNodes;
	vector<CTStat_Dev*>		m_szComNodes;

	// Conflict list
	vector<_NetDeviceInfo*>		m_szNetConfNodes;			// ��ͻ�б�
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

