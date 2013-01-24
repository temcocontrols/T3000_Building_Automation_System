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

#define WM_ADDTREENODE WM_USER + 2000

struct _ComDeviceInfo
{
	CTStat_Dev*	m_pDev;
	BOOL				m_bConflict;		// 
	int					m_nTempID;      //
	int					m_nSourceID;		// 

	// 
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
	BOOL				m_bConflict;		// 
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

	// 
 	BOOL ScanNetworkDevice(); 
 	// 
 	BOOL ScanComDevice();

	void SetComPort(int nCom);
public:
	// 
	void		Release();
	// 
	BOOL		IsComScanRunning();
	// 
	void		background_binarysearch(int nComPort);
	// bForTstat = TRUE : scan tstat, = FALSE : scan NC
	void		binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo=1, BYTE devHi=254);
	// 
	BOOL		binary_search_crc(int a);
	// 
	int _ScanNCFunc();
	
	// 
	//int GetAllComPort();

	// 
	// True = scan com; FALSE = scan net
	void SetScanType( BOOL bScanCom);

	void SetParentWnd(CWnd* pParent);
	
	void AddNetToGrid();	
	void AddComDeviceToGrid();

	void SendScanEndMsg();

	// 
	BOOL OpenCom(int nCom);

	BOOL IsRepeatedID(int nID);
	
	void ResetRepeatedID();
	// 
	int GetAllNodeFromDataBase();
	// 
	void FindComConflict();
	void FindNetConflict();
	// 
	void  ResolveComConflict();
	void  ResolveNetConflict();
	// 
	void AddNewTStatToDB();
	void AddNewNetToDB();
	//  
	void WriteOneDevInfoToDB( _ComDeviceInfo* pInfo);
	void WriteOneNetInfoToDB( _NetDeviceInfo* pInfo);

	void GetBuildingName();

	BOOL IsAllScanFinished();

	void StopScan();

	int AddNCToList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind);
	
	void ScanOldNC(BYTE devLo, BYTE devHi);

	//////////////////////////////////////////////////////////////////////////
	// 
	CString GetNetDevSubnetName(const CString& strIP);
	CString GetTstatSubnetName(const CString& strComPort);
	CString GetTstatMBuildingName(const CString& strComPort);
	void SetSubnetInfo(vector<Building_info>& szSubnets);
	void SetBaudRate(const CString& strBaudrate);
	
	// 
	void ScanTstatFromNCForManual();
	void BinaryScanNCByComPort(BYTE devLo = 1, BYTE devHi = 254);
	void ScanTstatFromNCForAuto();
	void GetTstatInfoFromID(int nTstatID);

	void ScanAll();
	void WaitScan();//scan
	int  m_scantype;
	void CombineScanResult();

	void ShowNetScanInfo(const CString& strInfo);
	void ShowComScanInfo(const CString& strInfo);

	void  ReadNCTable(_NetDeviceInfo* pNCInfo);
	
	void  AddNodeToTable(_NetDeviceInfo* pNCInfo, unsigned short* pNode);
	void GetTstatFromNCTable();
	void GetTstatFromNCTable2();
	void GetTstatFromNCTableByComport();

	BOOL ReadOneCheckOnline(int nCOMPort); // 

	BOOL IsNetDevice(const CString& strDevType);

public:
	//vector<CTStat_Dev*>		m_szTstatScandRet;
	//vector<CTStat_Net*>		m_szNCScanRet;
	vector<_ComDeviceInfo*>		m_szTstatScandRet;
	vector<_NetDeviceInfo*>		m_szNCScanRet;
	vector<_NetDeviceInfo*>		m_szNCScanRet2; // NC find by com port

	vector<CString>				m_szComs;
	CEvent*							m_eScanComEnd;
	CEvent*							m_eScanNCEnd;
	CEvent*							m_eScanOldNCEnd;

	BOOL								m_bStopScan;					// 
	BOOL								m_bNetScanFinish;			// 
	int									m_nBaudrate;
	int									m_nComPort;
	CWnd*							m_pParent;	

protected:
	BOOL								m_bComScanRunning;		// 
	int									m_ScannedNum;

	CWinThread*					m_pScanNCThread;
	CWinThread*					m_pScanTstatThread;
	CWinThread*					m_pWaitScanThread;




	//vector<int>						m_szRepeatedID;
	int									m_szRepeatedID[255];				// 
	//	vector <binary_search_result> m_szNCScanRet;				////background thread search result
	//	vector <binary_search_result> m_szTstatScandRet;			////background thread search result

	vector<CTStat_Net*>		m_szNetNodes;
	vector<CTStat_Dev*>		m_szComNodes;

	vector<_NetDeviceInfo*>		m_szNetConfNodes;			// 冲突列表
	vector<_ComDeviceInfo*>		m_szComConfNodes;
	
	//vector<_ComDeviceInfo*>		m_szTstatForNC;
	


	CString						m_strBuildingName;
	CString						m_strSubNet;
	CString						m_strFloorName;
	CString						m_strRoomName;
	//BOOL							m_bAllScanFinish;	
	_ConnectionPtr					t_pCon;
	
	vector<Building_info>		m_szSubnetsInfo;
	CString m_ip;
	CString m_port;

	void writetxt();
	CStdioFile*					m_pFile;
	CString						m_strFileINI;


};

