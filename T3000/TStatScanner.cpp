#include "StdAfx.h"
#include "TStatScanner.h"
#include "globle_function.h"
#include "ScanDlg.h"

#include "ScanDbWaitDlg.h"
#include "hangeIDDlg.h"
#include "MainFrm.h"
//#include "global_variable.h"

//AB means Add Building
#define AB_MAINNAME	1
#define AB_NAME	2
//#define AB_CONNECTION	3
#define	AB_PROTOCOL	3
#define	AB_IPADDRESS	4
#define	AB_IPPORT	5
#define	AB_COMPORT	6
#define	AB_BAUDRAT	7


//#define MAX_FIELDS	10
//CRITICAL_SECTION g_Lock;
#define UPD_BROADCAST_QRY_MSG 100
#define RESPONSE_MSG          UPD_BROADCAST_QRY_MSG+1
const int UDP_BROADCAST_PORT =1234;
const int RECV_RESPONSE_PORT = 4321;

const int TCP_COMM_PORT = 6001;
extern int g_ScnnedNum;


typedef struct infopack
{
	unsigned char msg;//RESPONSE_MSG
	unsigned char lenth;
	unsigned short* pNCInfo;
}NC_Info;

struct _NCInfo
{
	UINT nSerial;
	int nProductID;
	int modbusID;
	CString strIP;
	unsigned short nPort;
};


// vector<_NCInfo> g_NCList;
// 
// SOCKET hBroad=NULL;
// SOCKET sListen=NULL;


UINT _ScanNCThread(LPVOID pParam);
UINT _ScanNCByUDPFunc(LPVOID pParam);
UINT _ScanTstatThread(LPVOID pParam);
UINT _ScanTstatThread2(LPVOID pParam);
UINT _WaitScanThread(LPVOID pParam);

UINT _ScanOldNC(LPVOID pParam);

CTStatScanner::CTStatScanner(void)
:m_bComScanRunning(TRUE)
{
	m_bStopScan = FALSE;
	m_pScanNCThread = NULL;
	m_pScanTstatThread = NULL;
	
	m_eScanComEnd = new CEvent(false, false);
	m_eScanOldNCEnd = new CEvent(false, false);
	m_eScanNCEnd = new CEvent(false, false);

	m_bNetScanFinish = FALSE;
	ResetRepeatedID();

	m_strBuildingName = _T("");
	m_strFloorName = _T("");
	m_strRoomName = _T("");
	

	t_pCon.CreateInstance(_T("ADODB.Connection"));
	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	m_nComPort = 1;
	m_scantype = 0;

	m_pFile = new CStdioFile;//txt

}

CTStatScanner::~CTStatScanner(void)
{
// 	delete		m_pScanNCThread;
// 	delete		m_pScanTstatThread;
// 	delete		m_pWaitScanThread;
	if (m_eScanNCEnd != NULL)
	{		
		delete m_eScanNCEnd;
		//m_eScanComEnd = NULL;

	}
	if (m_eScanOldNCEnd != NULL)
	{		
		delete m_eScanOldNCEnd;
		//m_eScanComEnd = NULL;

	}
	if (m_eScanComEnd != NULL)
	{
		delete m_eScanComEnd;
		//m_eScanNetEnd = NULL;

	}
	
	t_pCon->Close();
	Release();


}


void CTStatScanner::Release() // this function never be used
{
	for(UINT i = 0; i < m_szNCScanRet.size(); i++)
	{
		delete m_szNCScanRet[i]->m_pNet;
		delete m_szNCScanRet[i];
	}
	m_szNCScanRet.clear();

	for(UINT i = 0; i < m_szTstatScandRet.size(); i++)
	{
		delete m_szTstatScandRet[i]->m_pDev;
		delete m_szTstatScandRet[i];
	}
	m_szTstatScandRet.clear();

	for(UINT i = 0; i < m_szComNodes.size(); i++)
	{		
		delete m_szComNodes[i];
	}
	m_szComNodes.clear();

	for(UINT i = 0; i < m_szNetNodes.size(); i++)
	{		
		delete m_szNetNodes[i];
	}
	m_szNetNodes.clear();


	if( WaitForSingleObject(m_eScanComEnd, 0) != WAIT_OBJECT_0 )		
	{
		SetEvent(m_eScanComEnd);
		TerminateThread(m_pScanTstatThread,0);
	}
	
	if(WaitForSingleObject(m_eScanNCEnd, 0) != WAIT_OBJECT_0 )
	{
		SetEvent(m_eScanNCEnd);
		TerminateThread(m_pScanNCThread,0);
	}

	if(WaitForSingleObject(m_eScanOldNCEnd, 0) != WAIT_OBJECT_0 )
	{
		SetEvent(m_eScanOldNCEnd);
		TerminateThread(m_pScanNCThread,0);
	}

	m_bStopScan = FALSE;
	Sleep(200);

}



 void CTStatScanner::SetScanType( BOOL bScanCom)
{
	m_bComScanRunning = bScanCom;
}


 BOOL CTStatScanner::IsComScanRunning()
{
	return m_bComScanRunning;
}

void CTStatScanner::SetComPort(int nCom)
{
	m_nComPort = nCom;
}

BOOL CTStatScanner::ScanNetworkDevice()
{

	return TRUE;
}

BOOL CTStatScanner::ScanComDevice()//02
{
	//background_binarysearch_netcontroller();
	//GetAllComPort();
	GetSerialComPortNumber1(m_szComs);

	if (m_szComs.size() <= 0)
	{
		AfxMessageBox(_T("Can't scan without any com port installed."));
		return FALSE;
	}
	
	SetCommunicationType(0);   //设置为串口通信方式

	m_pScanTstatThread = AfxBeginThread(_ScanTstatThread2,this);	
	
	//background_binarysearch();
	//m_ScannedNum=254;
	
	//m_pWaitScanThread = AfxBeginThread(_WaitScanThread, this);

// 	CScanDbWaitDlg waitDlg;
// 	waitDlg.DoModal();
	

	return TRUE;
}


 void CTStatScanner::background_binarysearch(int nComPort)
{
	//m_szTstatScandRet.clear();////////^0^
	
// 	for(UINT k=0;k<m_szNCScanRet.size();k++)
// 	{
// 		m_szTstatScandRet.push_back(m_szNCScanRet.at(k));
// 	}

	//binarySearchforComDevice(nComPort, FALSE);

	//BinaryScanNCByComPort();//comscan code

	binarySearchforComDevice(nComPort, false);
	
	//new nc//		GetTstatFromNCTableByComport();
}


void CTStatScanner::binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo, BYTE devHi)
{

}
	
 UINT _ScanNCThread(LPVOID pParam)
{

	
	return 1;
}

int CTStatScanner::_ScanNCFunc()
{

	return 1;
}


UINT _ScanNCByUDPFunc(LPVOID pParam)
{
	//g_nStartID = -1; // this is a flag for udp scan.
	CTStatScanner* pScanner = (CTStatScanner*)pParam;
	SOCKET hBroad=NULL;
	SOCKET sListen=NULL;
//	IP_ADAPTER_INFO pAdapterInfo;
//	ULONG len = sizeof(pAdapterInfo); 
// 	if(1) // XP and below
// 	{
// 		if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS) 
// 		{
// 			AfxMessageBox(_T("Can't get local IP address, scan failed!"));
// 			goto END_SCAN;
// 			return 0;
// 		}
// 	}
// 	else  if(0) // VISTA, Win7 or up
// 	{
// 
// 	}

//SOCKADDR_IN sockAddress;   // commented by zgq;2010-12-06; unreferenced local variable

	

	return 1;
}


int CTStatScanner::AddNCToList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind)
{
	int nLen=buffer[2]+buffer[3]*256;
	//int n =sizeof(char)+sizeof(unsigned char)+sizeof( unsigned short)*9;
	unsigned short usDataPackage[16]={0};
	if(nLen>=0)
	{
		CTStat_Net* pT = new CTStat_Net;

		memcpy(usDataPackage,buffer+4,nLen*sizeof(unsigned short));

		DWORD nSerial=usDataPackage[0]+usDataPackage[1]*256+usDataPackage[2]*256*256+usDataPackage[3]*256*256*256;
		int nProductID=usDataPackage[4];
		int modbusID=usDataPackage[5];
		CString strTemp;
		strTemp.Format(_T("%d.%d.%d.%d"),usDataPackage[6],usDataPackage[7],usDataPackage[8],usDataPackage[9]);
		CString strIP=strTemp;
		int nPort=(usDataPackage[10]);

	//	m_ip = strTemp;//scan
	//	m_port.Format(_T("%d"),nPort);//scan



		pT->SetSerialID(nSerial);
		pT->SetDevID(modbusID);
		pT->SetProductType(nProductID);
		pT->SetSubnetName(m_strSubNet);
// 		if (nProductID!= 100)
// 		{
// 			pT->SetProductType(100);
// 		}		
		//pT->SetIPPort(ntohs(siBind.sin_port));
		pT->SetIPPort(nPort);
		LPSTR szIP = inet_ntoa(siBind.sin_addr);
		pT->SetIPAddr((char*)szIP);

		_NetDeviceInfo* pni = new _NetDeviceInfo;
		pni->m_pNet = pT;										
		m_szNCScanRet.push_back(pni);
	}
	

	return m_szNCScanRet.size();
	

}

BOOL CTStatScanner::binary_search_crc(int a)
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


//*
UINT _ScanTstatThread2(LPVOID pParam)
{
	CTStatScanner* pScan = (CTStatScanner*)(pParam);
	close_com();	

	UINT i = 0;

	for (i = 0; i < pScan->m_szComs.size(); i++)
	{
		//if (WaitForSingleObject(pScan->m_eScanComEnd->m_hObject, 0) == WAIT_OBJECT_0 )
		if(pScan->m_bStopScan)
		{ // 有信号，就结束scan
			//g_ScnnedNum=254;
			//return;
			break;
		}
		CString strComPort = pScan->m_szComs[i];
		TRACE(_T("Scanning @ ") + strComPort + _T("\n"));
		
// 		CHAR* lpsz;
// 		USES_CONVERSION;
// 		lpsz = W2A(strComPort.GetString());
		//TCHAR tc = strComPort.GetAt(strComPort.GetLength()-1);
		CString tc = strComPort.Mid(3);

		int n = _wtoi(tc);
// 		char c[2];
// 		_itoa_s(n, c, 10);
// 		c[0] = '7';

		if(pScan->OpenCom(n))
		{
			pScan->SetComPort(n);
			bool bRet = Change_BaudRate(pScan->m_nBaudrate);
			ASSERT(bRet);
			g_strScanInfoPrompt.Format(_T("COM%d"), n);

			BOOL bReadone = pScan->ReadOneCheckOnline(n);
		//	if (!bReadone)//comscan oldcode
			//{
			//	pScan->background_binarysearch(n);				
			//}		
			pScan->background_binarysearch(n);	//lsc comscan new cold  //0
			close_com();
			Sleep(500);
			TRACE(_T("Success open the COM%d\n"), n); 
		}
		else
		{
			// 不能打开串口X，提示信息
			TRACE(_T("Cannot open the COM%d\n"), n);
			CString str;
			str.Format(_T("Cannot open the COM%d\n"), n);
			SetPaneString(2, str);
			int n =0 ;
		}

	}
	g_ScnnedNum=254;
	//
	//pScan->AddComDeviceToGrid();

	close_com();
	TRACE(_T("COM Scan finished"));
	if (pScan->m_eScanComEnd->m_hObject)
	{
		pScan->m_eScanComEnd->SetEvent();
	}

	return 1;
}

//*/





void CTStatScanner::SetParentWnd(CWnd* pParent)
{
	m_pParent = pParent;
}

void CTStatScanner::AddNetToGrid()
{

}

void CTStatScanner::AddComDeviceToGrid()
{

}



void CTStatScanner::SendScanEndMsg()
{
	
	// 合并同类项
	CombineScanResult();
	GetBuildingName();	// 获得当前选择的buildingname
	GetAllNodeFromDataBase();
//	if (m_bComScanRunning)
	{
		FindNetConflict();
		ResolveNetConflict();
		AddNewNetToDB();

		//GetBuildingName();	// 获得当前选择的buildingname
				
		FindComConflict();
		
		ResolveComConflict();
		
		AddNewTStatToDB();//
		
	}




	((CMainFrame*)m_pParent)->m_bScanALL = FALSE;
	((CMainFrame*)m_pParent)->m_bScanFinished = TRUE;


	if(m_szNCScanRet.size()==0 && m_szTstatScandRet.size() == 0)
	{
		m_pParent->PostMessage(WM_ADDTREENODE);
		return ;
	}

	//AfxMessageBox(_T("Step 6"));

//	closefile();//scan
	
	CScanDlg dlg;
	dlg.SetScanner(this);
	dlg.DoModal();
	if(IsWindow(dlg.m_hWnd))
	{
		WaitForSingleObject(dlg.m_hWnd, 200);  
	}
		m_pParent->PostMessage(WM_ADDTREENODE);
		allend = TRUE;
}


BOOL CTStatScanner::OpenCom(int nCom)
{
	for (int i  = 0; i < 3; i++ )
	{
		if(open_com(nCom))
		{
			if(is_connect())
			{			
				return TRUE;
			}		
		}
		Sleep(100);
	}

	return FALSE;
}

BOOL CTStatScanner::IsRepeatedID(int nID)
{
	ASSERT(nID < 255);
	if(m_szRepeatedID[nID] == 0)
	{
		return FALSE;
	}
	return TRUE;

}

void CTStatScanner::ResetRepeatedID()
{
	for (int i = 0; i < 255; i++)
	{
		m_szRepeatedID[i] = 0;
	}

}


// 打开数据库，然后去去比较冲突
int CTStatScanner::GetAllNodeFromDataBase()
{
	_ConnectionPtr m_pCon;		//for ado connection
	_RecordsetPtr m_pRs;			//for ado 

	m_pCon.CreateInstance("ADODB.Connection");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs.CreateInstance("ADODB.Recordset");

	_variant_t temp_variant;
	CString strTemp;

	CString temp_str=_T("select * from ALL_NODE");
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	

	//return m_pRs->get_RecordCount();

	int nTemp = 0;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{
		nTemp++;
		int nDefault=0;
		CString strDevType = m_pRs->GetCollect("Product_class_ID");

		CTStatBase* pNode = NULL;
		//if (strDevType.CompareNoCase(_T("100")) == 0)	 //100 =  NC
		if (IsNetDevice(strDevType))
		{
			pNode = new CTStat_Net;
			// Port
			CString strPort = m_pRs->GetCollect("Com_Port");
			((CTStat_Net*)(pNode))->SetIPPort(_wtoi(strPort));

	
			// IP Addr
			CString strIP = m_pRs->GetCollect("Bautrate");
			USES_CONVERSION;
			LPCSTR szIP = W2A(strIP);
			DWORD dwIP = inet_addr(szIP);
			((CTStat_Net*)(pNode))->SetIPAddr(dwIP);
	
			m_szNetNodes.push_back((CTStat_Net*)pNode);
		}
		else
		{
			pNode = new CTStat_Dev;
			// BaudRate
			CString strBaudRate = m_pRs->GetCollect("Bautrate");
			((CTStat_Dev*)(pNode))->SetBaudRate(_wtoi(strBaudRate));

			m_szComNodes.push_back(((CTStat_Dev*)(pNode)));

			CString strComPort = m_pRs->GetCollect("Com_Port");
			strComPort = strComPort.Mid(3);
			((CTStat_Dev*)(pNode))->SetComPort(_wtoi(strComPort));
		}
		pNode->SetProductType(_wtoi(strDevType));

		pNode->SetBuildingName(m_pRs->GetCollect("MainBuilding_Name"));
		pNode->SetFloorName(m_pRs->GetCollect("Floor_Name"));
		pNode->SetRoomName(m_pRs->GetCollect("Room_Name"));
		pNode->SetSubnetName(m_pRs->GetCollect("Building_Name"));

		CString strID = m_pRs->GetCollect("Product_ID");		
		pNode->SetDevID(_wtoi(strID));

		CString strHwv = m_pRs->GetCollect("Hardware_Ver");
		pNode->SetHardwareVersion(float(_wtof(strHwv)));
		CString strSwv = m_pRs->GetCollect("Software_Ver");
		pNode->SetSoftwareVersion(float(_wtof(strSwv)));

		CString strSerialID = m_pRs->GetCollect("Serial_ID");		
		(pNode)->SetSerialID(_wtoi(strSerialID));

		m_pRs->MoveNext();
	}
	return m_szNetNodes.size() + m_szComNodes.size();
}


// 对比数据库
void CTStatScanner::FindComConflict()
{

	for (UINT i = 0; i < m_szTstatScandRet.size(); i++)
	{
		_ComDeviceInfo* pInfo = m_szTstatScandRet[i];
		int nRtID = pInfo->m_pDev->GetSerialID();
		int nRtAddr = pInfo->m_pDev->GetDevID();
	
		pInfo->m_pDev->SetBuildingName(m_strBuildingName);
		pInfo->m_pDev->SetFloorName(m_strFloorName);
		pInfo->m_pDev->SetRoomName(m_strRoomName);	
		pInfo->m_pDev->SetSubnetName(m_strSubNet);	
		for (UINT j = 0; j < m_szComNodes.size(); j++)
		{
			int nSID = m_szComNodes[j]->GetSerialID();
			int nAddr = m_szComNodes[j]->GetDevID();
			if (nRtID == nSID)
			{	
			
				pInfo->m_pDev->SetBuildingName(m_szComNodes[j]->GetBuildingName());
				pInfo->m_pDev->SetFloorName(m_szComNodes[j]->GetFloorName());
				pInfo->m_pDev->SetRoomName(m_szComNodes[j]->GetRoomName());	
				pInfo->m_pDev->SetSubnetName(m_szComNodes[j]->GetSubnetName());	
				if (nAddr != nRtAddr) // 
				{
					pInfo->m_bConflict = TRUE;
					pInfo->m_nSourceID = nRtAddr;
					pInfo->m_nTempID = nAddr;
					m_szComConfNodes.push_back(pInfo);
				}				
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// 如果Serial ID相同，而IP不同，视为冲突
void CTStatScanner::FindNetConflict()
{
	for (UINT i = 0; i < m_szNCScanRet.size(); i++)
	{
		_NetDeviceInfo* pInfo = m_szNCScanRet[i];
		int nScanID = pInfo->m_pNet->GetSerialID();
		DWORD dwScanIP = pInfo->m_pNet->GetIPAddr();

		pInfo->m_pNet->SetBuildingName(m_strBuildingName);
		pInfo->m_pNet->SetSubnetName(m_strSubNet);	
		for (UINT j = 0; j < m_szNetNodes.size(); j++)
		{
			int nSID = m_szNetNodes[j]->GetSerialID();
			DWORD dwIP = m_szNetNodes[j]->GetIPAddr();
			//加上 buildingname等私货

			if (nScanID == nSID)
			{	
				if (dwScanIP != dwIP) // 需要矫正
				{
					pInfo->m_pNet->SetBuildingName(m_szNetNodes[j]->GetBuildingName());
					pInfo->m_pNet->SetFloorName(m_szNetNodes[j]->GetFloorName());
					pInfo->m_pNet->SetRoomName(m_szNetNodes[j]->GetRoomName());	
					pInfo->m_pNet->SetSubnetName(m_szNetNodes[j]->GetSubnetName());	
					pInfo->m_bConflict = TRUE;
					pInfo->m_dwIPDB = dwIP;
					pInfo->m_dwIPScan = dwScanIP;
					m_szNetConfNodes.push_back(pInfo);
				}				
			}
		}
	}
}



void  CTStatScanner::ResolveComConflict()
{
// 	_ConnectionPtr t_pCon;//for ado connection
// 	//_RecordsetPtr t_pRs;//for ado 
// 	t_pCon.CreateInstance(_T("ADODB.Connection"));
// 	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	for (UINT  i = 0; i  < m_szComConfNodes.size(); i++)
	{
		//changed the id ,
		_ComDeviceInfo* pInfo = m_szComConfNodes[i];
		int nScanID = pInfo->m_nSourceID;
		int nDBID = pInfo->m_nTempID;
		int nSID = pInfo->m_pDev->GetSerialID();
		
		CChangeIDDlg dlg;
// 		dlg.SetPromtionTxt(m_product.at(m).serial_number,
// 								  m_product.at(m).product_id,
// 								  m_binary_search_product_background_thread.at(j).serialnumber,
// 								  m_binary_search_product_background_thread.at(j).id);

		dlg.SetPromtionTxt(  nSID, 
									nDBID, 
									nSID,
									nScanID);

		if(dlg.DoModal()==IDOK)
		{
			if(dlg.m_nChange==0)
			{
				int nRet=-1;
				//Change_BaudRate(19200);
				for(int i=0;i<3;i++)
				{
					register_critical_section.Lock();
					//bRet=Write_One(m_binary_search_product_background_thread.at(j).id,6,m_product.at(m).product_id);
					nRet=Write_One2(nScanID,6,nDBID, false);
					if(nRet>0)
						multi_register_value[6]=nDBID;//mark***********************
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
				CString strSql;
				//strID.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
				strID.Format(_T("%d"),nScanID);
				//strSerial.Format(_T("%d"),m_binary_search_product_background_thread.at(j).serialnumber);
				strSerial.Format(_T("%d"),nSID);

				strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),strID,strSerial);
				t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			if(dlg.m_nChange==3)
			{
				//remove it
				//m_binary_search_product_background_thread.erase(m_binary_search_product_background_thread.begin()+j);
				//EraseScanResult(nSID);
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
		//continue;// this alread inserted to database:
//	}
	
}



void  CTStatScanner::ResolveNetConflict()
{
// 	_ConnectionPtr t_pCon;//for ado connection
// 	//_RecordsetPtr t_pRs;//for ado 
// 	t_pCon.CreateInstance(_T("ADODB.Connection"));
// 	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	for (UINT  i = 0; i  < m_szNetConfNodes.size(); i++)
	{
		//changed the id ,
		_NetDeviceInfo* pInfo = m_szNetConfNodes[i];
		DWORD dwScanIP = pInfo->m_dwIPScan;
		DWORD dwDBIP = pInfo->m_dwIPDB;
		DWORD dwSID = pInfo->m_pNet->GetSerialID();
		//if(dlg.m_nChange==2)
		{
				CString strID;
				CString strSerial;
				CString strSql;
				//strID.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
				strID.Format(_T("%d"),dwScanIP);
				//strSerial.Format(_T("%d"),m_binary_search_product_background_thread.at(j).serialnumber);
				strSerial.Format(_T("%d"),dwSID);

				//strSql.Format(_T("delete ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),strID,strSerial);
				strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s'"), strSerial);
				t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		}
	}
}


void CTStatScanner::AddNewTStatToDB()
{
	BOOL bIsNew = TRUE;
	for(UINT i = 0; i < m_szTstatScandRet.size(); i++)
	{
		bIsNew = TRUE;
		int nSID = m_szTstatScandRet[i]->m_pDev->GetSerialID();
// 		m_szTstatScandRet[i]->m_pDev->SetBuildingName(m_strBuildingName);
// 		m_szTstatScandRet[i]->m_pDev->SetSubnetName(m_strSubNet);
// 		m_szTstatScandRet[i]->m_pDev->SetFloorName(m_strFloorName);
// 		m_szTstatScandRet[i]->m_pDev->SetRoomName(m_strRoomName);
		for (UINT j = 0; j < m_szComNodes.size(); j++)
		{
			int nNodeSID = m_szComNodes[j]->GetSerialID();
			if (nSID == nNodeSID)
			{
				bIsNew = FALSE;
				break;
			}
		}

		if (bIsNew == TRUE)
		{
			WriteOneDevInfoToDB(m_szTstatScandRet[i]);
		}
	}
}


void CTStatScanner::AddNewNetToDB()
{

}

void CTStatScanner::WriteOneNetInfoToDB( _NetDeviceInfo* pInfo)
{

}

 void CTStatScanner::WriteOneDevInfoToDB( _ComDeviceInfo* pInfo)//0
{
	ASSERT(pInfo);
// 	_ConnectionPtr t_pCon;//for ado connection
// 	//_RecordsetPtr t_pRs;//for ado 
// 	t_pCon.CreateInstance(_T("ADODB.Connection"));
// 	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

// 	CString strBuildingName = pInfo->m_pDev->GetBuildingName();
// 	CString strFloorName = pInfo->m_pDev->GetFloorName();
// 	CString strRoomName = pInfo->m_pDev->GetRoomName();	


	CString strID;
	int nID = pInfo->m_pDev->GetDevID();	
	strID.Format(_T("%d"),  nID);

	CString strProductName = pInfo->m_pDev->GetProductName();

	CString strSerialID;
	strSerialID.Format(_T("%d"), pInfo->m_pDev->GetSerialID());


	int nClassID = pInfo->m_pDev->GetProductType();
	CString strClassID;
	strClassID.Format(_T("%d"), nClassID);

// 	int nBaudRate = pInfo->m_pDev->GetBaudRate();//scan
// 	CString strBaudRate;//scan
// 	strBaudRate.Format(_T("%d"), nBaudRate);//scan

	CString strBaudRate;//scan
// 	in_addr ia;
// 	ia.S_un.S_addr = pInfo->m_pNet->GetIPAddr();
// 	strBaudRate = CString(inet_ntoa(ia));	
	strBaudRate = pInfo->m_tstatip;

	CString strScreenName;
	strScreenName.Format(_T("Screen(S:%d--%d)"), pInfo->m_pDev->GetSerialID(), pInfo->m_pDev->GetDevID() );

	CString strBackground_bmp=_T("Clicking here to add a image...");

	CString strHWV;
	strHWV.Format(_T("%0.1f"), pInfo->m_pDev->GetHardwareVersion());

	CString strSWV;
	strSWV.Format(_T("%0.1f"), pInfo->m_pDev->GetSoftwareVersion());

//	CString strCom;//scan
//	strCom.Format(_T("COM%d"), pInfo->m_pDev->GetComPort());//scan

	CString strCom;//scan
	//strCom.Format(_T("%d"), pInfo->m_pNet->GetIPPort());
	strCom = pInfo->m_tstatport;
	
	CString strMainetInfo = pInfo->m_pDev->m_mainnet_info.GetMainnetInfo();







	//strBaudRate
	//strCom

//20120424
// 	CString strIP;
// 	in_addr ia;
// 	ia.S_un.S_addr = pInfo->m_pNet->GetIPAddr();
// 	strIP = CString(inet_ntoa(ia));	
// 
// 	CString strPort;
// 	strPort.Format(_T("%d"), pInfo->m_pNet->GetIPPort());
//20120424

//	CString strSubNetName = GetTstatSubnetName(strCom);
// 	pInfo->m_pDev->SetSubnetName(strSubNetName);
	
 
// 	CString strMBuildingName = GetTstatMBuildingName(strCom);
// 	pInfo->m_pDev->SetBuildingName(strMBuildingName);

	CString strSql;
	CString strEpSize;
	strEpSize.Format(_T("%d"), pInfo->m_pDev->GetEPSize());

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"')"));
	//new nc// strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize, Mainnet_info) values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"','"+strMainnetInfo+"')"));
	t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

}


void CTStatScanner::GetBuildingName()
{
	_RecordsetPtr m_pRs;
	_ConnectionPtr m_pCon;

	m_pCon.CreateInstance("ADODB.Connection");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs.CreateInstance("ADODB.Recordset");

	CString strSql;
	strSql.Format(_T("select * from Building order by Main_BuildingName"));

	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
//	m_AddBuiding_FlexGrid.put_Rows(m_pRs->RecordCount+2);	
	int temp_row=0;
	CString str_temp;
	str_temp.Empty();
	_variant_t temp_variant;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	
		++temp_row;
			
		int bdef=0;
		bdef=m_pRs->GetCollect(_T("Default_SubBuilding"));
		if(bdef == -1)//def building;
		{
			//m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,0,_T("Select"));			
			temp_variant=m_pRs->GetCollect("Main_BuildingName");
			if(temp_variant.vt!=VT_NULL)
				m_strBuildingName = temp_variant;
			else
				m_strBuildingName  = _T("");

			
			temp_variant=m_pRs->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				m_strSubNet = temp_variant;
			else
				m_strSubNet = _T("");

// 			temp_variant=m_pRs->GetCollect("Floor_name");
// 			if(temp_variant.vt!=VT_NULL)
// 				m_strFloorName = temp_variant;
// 			else
				m_strFloorName = _T("floor1");


// 			temp_variant=m_pRs->GetCollect("Room_Name");
// 			if(temp_variant.vt!=VT_NULL)
// 				m_strRoomName = temp_variant;
// 			else
				m_strRoomName = _T("room1");

		}
		
		m_pRs->MoveNext();
	}

	if(m_pRs->State) 
		m_pRs->Close(); 
}

BOOL CTStatScanner::IsAllScanFinished()
{
// 	if (m_bComScanRunning)
// 	{
// 		return (g_ScnnedNum >= 254);
// 	}
// 	else
// 	{
// 		return m_bNetScanFinish;
// 	}

	return ((g_ScnnedNum >= 254 && m_bNetScanFinish)||allend);

	//return TRUE;

}

void CTStatScanner::StopScan()
{
// 	if( WaitForSingleObject(m_eScanComEnd, 0) != WAIT_OBJECT_0 )		
// 	{
// 		//SetEvent(m_eScanComEnd);
// 		m_eScanComEnd->SetEvent(); // 自然结束
// 		//TerminateThread(m_pScanTstatThread,0);
// 	}
// 
// 	if(WaitForSingleObject(m_eScanNetEnd, 0) != WAIT_OBJECT_0 )
// 	{
// 		//SetEvent(m_eScanNetEnd);
// 		m_eScanNetEnd->SetEvent(); // 自然结束
// 		//TerminateThread(m_pScanNCThread,0);
// 	}


}


void CTStatScanner::SetSubnetInfo(vector<Building_info>& szSubnets)
{
	m_szSubnetsInfo = szSubnets;
}

 CString CTStatScanner::GetTstatSubnetName(const CString& strComPort)
{
	CString strSubnetName = _T("Subnet_Undefine");

	return strSubnetName;
}

 CString CTStatScanner::GetTstatMBuildingName(const CString& strComPort)
{
	CString strMBuildingName = _T("Building_Undef");

	return strMBuildingName;
}


// 
 CString CTStatScanner::GetNetDevSubnetName(const CString& strIP)
{
 	CString strSubnetName = _T("Subnet_Undefine");

 	return strSubnetName;
}



void CTStatScanner::SetBaudRate(const CString& strBaudrate)
{
	m_nBaudrate = _wtoi(strBaudrate);
	if (m_nBaudrate != 9600 && m_nBaudrate != 19200)
	{
		m_nBaudrate = 19200;
	}
}


 void CTStatScanner::GetTstatInfoFromID(int nTstatID)
{


}




 void CTStatScanner::ScanTstatFromNCForManual()
{	

}



 void CTStatScanner::ScanTstatFromNCForAuto()
{

}



 void CTStatScanner::BinaryScanNCByComPort(BYTE devLo, BYTE devHi)
{

}



//*/




 UINT _ScanOldNC(LPVOID pParam)
{


	return 1;
}



 void CTStatScanner::ScanOldNC(BYTE devLo, BYTE devHi)
{

}




void CTStatScanner::ScanAll()
{	

	ScanComDevice();
	AfxBeginThread(_WaitScanThread, this);

}




UINT _WaitScanThread(PVOID pParam)
{
	BOOL Flag = FALSE;
	CTStatScanner* pScanner = (CTStatScanner*)(pParam);
	switch(pScanner->m_scantype)
	{
	case 1:
		if (WaitForSingleObject(pScanner->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
			Flag = TRUE;
		break;
	case 2:
 		if (WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
			Flag = TRUE;
		break;
	case 3:
		if ((WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
			&& (WaitForSingleObject(pScanner->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))
			Flag = TRUE;
		break;


	}
		if (Flag)
		{
			//////////////////////////////////////////////////////////////////////////
			// scan tstat
			g_strScanInfoPrompt = _T("TStat connected with NC");
			g_nStartID = 1;

			pScanner->m_bNetScanFinish = TRUE; // at this time, two thread end, all scan end
			pScanner->SendScanEndMsg();
		}
	

	return 1;
}


void CTStatScanner::CombineScanResult()
{
	for (UINT i = 0; i < m_szTstatScandRet.size(); i++)
	{
		_ComDeviceInfo* pDev = m_szTstatScandRet[i];
		for (unsigned int j = i+1; j < m_szTstatScandRet.size(); j++)
		{
			_ComDeviceInfo* pDev2 = m_szTstatScandRet[j];
			if (pDev->m_pDev->GetSerialID() == pDev2->m_pDev->GetSerialID())
			{
// #ifdef _DEBUG
// 				TRACE(pDev2->m_pDev->GetProductName()+_T("-same dev. \n"));
// #endif
				delete pDev2;
				m_szTstatScandRet.erase(m_szTstatScandRet.begin()+j);
				j--;
			}
		}
	}
}


void CTStatScanner::ShowNetScanInfo(const CString& strInfo)
{
	CString* pstrInfo = new CString(strInfo);
	if(((CMainFrame*)(m_pParent))->m_pWaitScanDlg)
		PostMessage(((CMainFrame*)(m_pParent))->m_pWaitScanDlg->m_hWnd, WM_NETSCANINFO, WPARAM(pstrInfo), LPARAM(0));
	
}

 void CTStatScanner::ShowComScanInfo(const CString& strInfo)
{

}















//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
 void CTStatScanner::GetTstatFromNCTableByComport()
{


}


// for ethernet nc
 void CTStatScanner::GetTstatFromNCTable()
{

}


/// for com port nc
 void CTStatScanner::GetTstatFromNCTable2()
{

}


const int TABLE_NODE_NUM_REG = 7000;
const int TABLE_NODE_SIZE = 20;				
 void  CTStatScanner::ReadNCTable(_NetDeviceInfo* pNCInfo)
{

}


 void  CTStatScanner::AddNodeToTable(_NetDeviceInfo* pNCInfo, unsigned short* pNode)
{


}




//*/



BOOL CTStatScanner::ReadOneCheckOnline(int nCOMPort)
{
#ifdef _DEBUG
	if(nCOMPort == 4)
	{
int n = 0;
	}
	
#endif
	BOOL bFlag = TRUE;
	int nID =0;
	for (int i =0; i < 3; i++)//comscan oldcode
	{
		nID =  Read_One(255, 6);
		Sleep(200);
		int nID1 = Read_One(255, 6);
		Sleep(200);
		int nID2 = Read_One(255, 6);

		if ((nID == nID1 && nID == nID2) && nID > 0)
		{
			bFlag = TRUE;		
		}	
		else
		{
			bFlag = FALSE;
		}

	}

	if (bFlag)
	{
		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;
		//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=read_multi2(nID,&SerialNum[0],0,9,false);

		if(nRet>0)
		{
			CTStat_Dev* pTemp = new CTStat_Dev;
			_ComDeviceInfo* pInfo = new _ComDeviceInfo;
			pInfo->m_pDev = pTemp;

			if(IsRepeatedID(nID))
			{
					TRACE(_T("Scan one with Repeated ID = %d\n"), nID);
					pInfo->m_bConflict = TRUE;								
					pInfo->m_nSourceID = m_szRepeatedID[nID];
					pInfo->m_nTempID = nID;
			}
			else
			{
				pInfo->m_bConflict = FALSE;
				pInfo->m_nSourceID = nID;
				pInfo->m_nTempID = nID;
			}

			m_szTstatScandRet.push_back(pInfo);
			// 			temp.id=a;
			// 			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			pTemp->SetSerialID(nSerialNumber);
			// 			temp.product_class_id=SerialNum[7];
			// 			temp.hardware_version=SerialNum[8];
			pTemp->SetDevID(nID);

			float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
				tstat_version2 /=10;
			}//tstat_version

				//temp.software_version=tstat_version2;
			pTemp->SetSoftwareVersion(tstat_version2);
			if(Read_One2(nID, 185, TRUE)==0)	
					//if(pTemp->ReadOneReg(185)==0)
			{
					//temp.baudrate=9600;
				pTemp->SetBaudRate(9600);
			}
			else
			{
					//temp.baudrate=19200;
				pTemp->SetBaudRate(19200);
			}
				//temp.nEPsize=Read_One2(temp.id,326, bForTStat);
				pTemp->SetEPSize(pTemp->ReadOneReg(326));

				//if(pTemp->GetComPort()>=0)
				pTemp->SetComPort(nCOMPort);
				// product type
				//pTemp->ReadOneReg(8);
				pTemp->SetProductType(SerialNum[7]);

				// hardware_version
				pTemp->SetHardwareVersion(SerialNum[8]);		

				pTemp->SetBuildingName(m_strBuildingName);
				pTemp->SetSubnetName(m_strSubNet);
			}
	}

	TRACE(_T("Read one check online finish. \n"));
	return bFlag;

}

BOOL CTStatScanner::IsNetDevice(const CString& strDevType)
{
	int nDeviceType = _wtoi(strDevType);
	if (nDeviceType == PM_LightingController || nDeviceType == PM_NC)
	{
		return TRUE;
	}

	return FALSE;
}

void CTStatScanner::WaitScan()
{
	AfxBeginThread(_WaitScanThread, this);
}

 void CTStatScanner::writetxt()
{


}