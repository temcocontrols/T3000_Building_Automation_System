#include "StdAfx.h"
#include "TStatScanner.h"
#include "globle_function.h"
#include "ScanDlg.h"

#include "ScanDbWaitDlg.h"
#include "hangeIDDlg.h"
#include "MainFrm.h"
#include "bip.h"

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

#define SHOW_TX_RX 	g_llTxCount++;g_llRxCount++;if( AfxGetMainWnd()->GetActiveWindow() != NULL ) {CString str;str.Format(_T("Addr:255 [Tx=%d Rx=%d Err=%d]"), g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());}
//#define MAX_FIELDS	10
//CRITICAL_SECTION g_Lock;
#define UPD_BROADCAST_QRY_MSG 100
#define RESPONSE_MSG          UPD_BROADCAST_QRY_MSG+1
const int UDP_BROADCAST_PORT =1234;
const int RECV_RESPONSE_PORT = 4321;

const int TCP_COMM_PORT = 6001;
extern int g_ScnnedNum;

bool is_in_scan_mode = false;

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

UINT _ScanBacnetComThread(LPVOID pParam);
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
	m_pScanTstatOneByOneThread=NULL;
	m_pScanBacnetComThread = NULL;
	
	m_eScanComEnd = new CEvent(false, false);
	m_eScanComOneByOneEnd=new CEvent(false,false);
	m_eScanOldNCEnd = new CEvent(false, false);
	m_eScanNCEnd = new CEvent(false, false);
	m_eScanBacnetComEnd = new CEvent(false, false);
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
	m_com_scan_end = false;
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

	if(m_eScanBacnetComEnd !=NULL)
	{
		delete m_eScanBacnetComEnd;
	}
	if (m_eScanComOneByOneEnd!=NULL)
	{
delete	m_eScanComOneByOneEnd;
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
	if( WaitForSingleObject(m_eScanBacnetComEnd, 0) != WAIT_OBJECT_0 )		
	{
		SetEvent(m_eScanBacnetComEnd);
		TerminateThread(m_pScanBacnetComThread,0);
	}
	if (WaitForSingleObject(m_eScanComOneByOneEnd,0)!=WAIT_OBJECT_0)
	{
	SetEvent(m_eScanComOneByOneEnd);
	TerminateThread(m_pScanTstatOneByOneThread,0);
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
	//m_pScanNCThread = AfxBeginThread(_ScanNCThread,this);

	m_pScanNCThread = AfxBeginThread(_ScanNCByUDPFunc,this);//lsc

	CWinThread * pTempThread= AfxBeginThread(_ScanOldNC, this);//这个是为扫描NC下面的TSTAT

	//m_pWaitScanThread = AfxBeginThread(_WaitScanThread, this);
	return TRUE;
}

BOOL CTStatScanner::ScanComDevice()//02
{
	//background_binarysearch_netcontroller();
	//GetAllComPort();
	m_szComs.clear();
	GetSerialComPortNumber1(m_szComs);

	if (m_szComs.size() <= 0)
	{
		//AfxMessageBox(_T("Can't scan without any com port installed."));
		  WriteLogFile(_T("Can't scan without any com port installed."));
		return FALSE;
	}
    WriteLogFile(_T("scanning your computer com port ...."));
	SetCommunicationType(0);   //设置为串口通信方式

	m_pScanTstatThread = AfxBeginThread(_ScanTstatThread2,this);	
	
	//background_binarysearch();
	//m_ScannedNum=254;
	
	//m_pWaitScanThread = AfxBeginThread(_WaitScanThread, this);

// 	CScanDbWaitDlg waitDlg;
// 	waitDlg.DoModal();
	

	return TRUE;
}
BOOL CTStatScanner::ScanComOneByOneDevice()//02
{
	//background_binarysearch_netcontroller();
	//GetAllComPort();
	m_szComs.clear();
	GetSerialComPortNumber1(m_szComs);

	if (m_szComs.size() <= 0)
	{
		//AfxMessageBox(_T("Can't scan without any com port installed."));
		WriteLogFile(_T("Can't scan without any com port installed."));
		return FALSE;
	}
	WriteLogFile(_T("scanning your computer com port ...."));
	SetCommunicationType(0);   //设置为串口通信方式

	m_pScanTstatThread = AfxBeginThread(_ScanTstatThread,this);	

	//background_binarysearch();
	//m_ScannedNum=254;

	//m_pWaitScanThread = AfxBeginThread(_WaitScanThread, this);

	// 	CScanDbWaitDlg waitDlg;
	// 	waitDlg.DoModal();


	return TRUE;
}

void CTStatScanner::background_binarysearch(int nComPort)
{
	
	  
	binarySearchforComDevice(nComPort, false);
	

}	
 void CTStatScanner::OneByOneSearchforComDevice(int nComPort, bool bForTStat, BYTE devLo, BYTE devHi)
{
   
	if (m_bStopScan)
	{
		 
		 
		CString strlog=_T("Scan Stop Time: ")+Get_NowTime()+_T("\n");;
		NET_WriteLogFile(strlog);
		WriteLogFile(strlog);
		return;
	}
	if (nComPort!=-1)
	{
		CString strlog;
		strlog.Format(_T("Scan ComPort: %d From ID=%d To ID=%d"),nComPort,devLo,devHi);
		WriteLogFile(strlog);
	}
	else
	{
		CString strlog;
		strlog.Format(_T("Scan From ID=%d To ID=%d"),devLo,devHi);
		NET_WriteLogFile(strlog);
	}
	g_strScanInfoPrompt.Format(_T("COM%d"), nComPort);

	for (int ID=devLo;ID<=devHi;ID++)
	{
	    g_nStartID=g_nEndID=ID;
		CString strlog;
		strlog.Format(_T("Scan ComPort: %d From ID=%d To ID=%d"),nComPort,ID,ID);
		WriteLogFile(strlog);

		Sleep(50);	
		int nCount=0;

		int a=read_one(ID,6);

		if (a == -3 || a > 0)
		{
			a=read_one(ID,6);
		}

		//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
		if(binary_search_crc(a))
		{
			return ;	 
		}
		char c_array_temp[5]={'0'};
		CString temp=_T("");
		if(a>0)
		{
			int ntempID=0;
			BOOL bFindSameID=false;
			int nPos=-1;
			//		temp.baudrate=m_baudrate2;
			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			nRet=read_multi2(a,&SerialNum[0],0,9,bForTStat);

			if(nRet>0)
			{
				CTStat_Dev* pTemp = new CTStat_Dev;			
				_ComDeviceInfo* pInfo = new _ComDeviceInfo;
				pInfo->m_pDev = pTemp;
				if(IsRepeatedID(a))
				{
					TRACE(_T("Scan one with Repeated ID = %d\n"), a);
					pInfo->m_bConflict = TRUE;								
					pInfo->m_nSourceID = m_szRepeatedID[a];
					pInfo->m_nTempID = a;
				}
				else
				{
					pInfo->m_bConflict = FALSE;
					pInfo->m_nSourceID = a;
					pInfo->m_nTempID = a;
				}

				pInfo->m_tstatip = m_ip;//scan
				pInfo->m_tstatport = m_port;//scan

				m_szTstatScandRet.push_back(pInfo);
				// 			temp.id=a;
				// 			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
				//int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;//20120424
				unsigned int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;//20120424

				pTemp->SetSerialID(nSerialNumber);
				// 			temp.product_class_id=SerialNum[7];
				// 			temp.hardware_version=SerialNum[8];
				pTemp->SetDevID(a);

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
				if(Read_One2(a, 185, bForTStat)==0)	
					//if(pTemp->ReadOneReg(185)==0)
				{
					//temp.baudrate=9600;
					pTemp->SetBaudRate(9600);//scan

				}
				else
				{
					//temp.baudrate=19200;
					pTemp->SetBaudRate(19200);//scan
				}
				//temp.nEPsize=Read_One2(temp.id,326, bForTStat);
				pTemp->SetEPSize(pTemp->ReadOneReg(326));

				//if(pTemp->GetComPort()>=0)
				pTemp->SetComPort(nComPort);
				// product type
				//pTemp->ReadOneReg(8);
				pTemp->SetProductType(SerialNum[7]);

				// hardware_version
				pTemp->SetHardwareVersion(SerialNum[8]);		

				pTemp->SetBuildingName(m_strBuildingName);
				pTemp->SetSubnetName(m_strSubNet);

				CString strlog ;
				strlog.Format(_T("\nFind one Device << ID=%d,SerialNo=%d >>\n"),a,nSerialNumber);
				WriteLogFile(strlog);
			}
			else
				return;
		}
	}

m_bStopScan=TRUE;
}

void CTStatScanner::binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo, BYTE devHi)
{
   
	if (m_bStopScan)
	{
		 
		 
		CString strlog=_T("Scan Stop Time: ")+Get_NowTime()+_T("\n");;
		NET_WriteLogFile(strlog);
		WriteLogFile(strlog);
		return;
	}
	if (nComPort!=-1)
	{
		CString strlog;
		strlog.Format(_T("Scan ComPort: %d From ID=%d To ID=%d"),nComPort,devLo,devHi);
		WriteLogFile(strlog);
	}
	else
	{
		CString strlog;
		strlog.Format(_T("Scan From ID=%d To ID=%d"),devLo,devHi);
		NET_WriteLogFile(strlog);
	}
	g_strScanInfoPrompt.Format(_T("COM%d"), nComPort);

	
	

	g_nStartID = devLo;
	g_nEndID = devHi;

	Sleep(50);	
	int nCount=0;

	int a=CheckTstatOnline_a(devLo,devHi, bForTStat);
	if(a ==-6)//总线上存在bacnet协议，modbus协议无法扫描;
	{
		CString temp_cs;
		temp_cs.Format(_T("Com%d"),nComPort);
		m_bacnetComs.push_back(temp_cs);
		return ;
	}
	if (a == -3 || a > 0)
	{
        g_llTxCount++;
		g_llRxCount++;
		if( AfxGetMainWnd()->GetActiveWindow() != NULL ) {

			// construct status message string
			CString str;
			str.Format(_T("Addr:255 [Tx=%d Rx=%d Err=%d]"), 
				  g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

			//Display it
			((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

		}

		a=CheckTstatOnline_a(devLo,devHi, bForTStat);
		g_llTxCount++;
		g_llRxCount++;
		if( AfxGetMainWnd()->GetActiveWindow() != NULL ) {

			// construct status message string
			CString str;
			str.Format(_T("Addr:255 [Tx=%d Rx=%d Err=%d]"), 
				g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

			//Display it
			((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

		}

	}

	//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
	{
		
	return ;	 
	}
	char c_array_temp[5]={'0'};
	CString temp=_T("");
	if(a>0)
	{
		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;
//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=read_multi2(a,&SerialNum[0],0,9,bForTStat);
		SHOW_TX_RX
		if(nRet>0)
		{
			CTStat_Dev* pTemp = new CTStat_Dev;			
			_ComDeviceInfo* pInfo = new _ComDeviceInfo;
			pInfo->m_pDev = pTemp;
			if(IsRepeatedID(a))
			{
				TRACE(_T("Scan one with Repeated ID = %d\n"), a);
				pInfo->m_bConflict = TRUE;								
				pInfo->m_nSourceID = m_szRepeatedID[a];
				pInfo->m_nTempID = a;
			}
			else
			{
				pInfo->m_bConflict = FALSE;
				pInfo->m_nSourceID = a;
				pInfo->m_nTempID = a;
			}

			pInfo->m_tstatip = m_ip;//scan
			pInfo->m_tstatport = m_port;//scan
			
			m_szTstatScandRet.push_back(pInfo);
// 			temp.id=a;
// 			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			//int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;//20120424
			unsigned int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;//20120424
			
			pTemp->SetSerialID(nSerialNumber);
// 			temp.product_class_id=SerialNum[7];
// 			temp.hardware_version=SerialNum[8];
			pTemp->SetDevID(a);
			
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
			if(Read_One2(a, 185, bForTStat)==0)	
			//if(pTemp->ReadOneReg(185)==0)
			{SHOW_TX_RX
				//temp.baudrate=9600;
				pTemp->SetBaudRate(9600);//scan

			}
			else
			{
					//temp.baudrate=19200;
				pTemp->SetBaudRate(19200);//scan
			}
			//temp.nEPsize=Read_One2(temp.id,326, bForTStat);
			pTemp->SetEPSize(pTemp->ReadOneReg(326));

			//if(pTemp->GetComPort()>=0)
			pTemp->SetComPort(nComPort);
			// product type
			//pTemp->ReadOneReg(8);
			pTemp->SetProductType(SerialNum[7]);

			// hardware_version
			pTemp->SetHardwareVersion(SerialNum[8]);		
				
			pTemp->SetBuildingName(m_strBuildingName);
			pTemp->SetSubnetName(m_strSubNet);

			CString strlog ;
			strlog.Format(_T("\nFind one Device << ID=%d,SerialNo=%d >>\n"),a,nSerialNumber);
			WriteLogFile(strlog);
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
			binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2);
			binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforComDevice(nComPort, bForTStat, devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2);
			binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi);
		}
		else
		{			
			m_szRepeatedID[devLo] = devLo;
			TRACE(_T("Scan one with same ID = %d\n"), devLo);
			do
			{
				///*
				nCount++;
				if(nCount>=5)
				{
					nCount=0;
					break;
				}
				//*/
				
				//if(Read_One2(devLo,10, bForTStat)==-2)
				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
			//	if(Read_One2(devLo,10, bForTStat)==-2)
			//	{
			//		break;
			//	}
				

			//
				if(Read_One2(devLo,10, bForTStat)!=-2)//one times
				
			//	{

			//	}
			//	else
				{	
					Sleep(100);
					SHOW_TX_RX
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
						//	if(!found_same_net_work_controller_by_mac(a))
							if(1)
							{
								bool find=false;//false==no find;true==find
								for(UINT w=0;w<m_szTstatScandRet.size();w++)
									if(j==(m_szTstatScandRet.at(w))->m_pDev->GetDevID())
									{
										find=true;
										break;
									}
								if(find==false)
								{
									//if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
									if(Write_One2(devLo,10,j, bForTStat)>0)//sometimes write failure ,so inspect,important
									{	SHOW_TX_RX
										m_szRepeatedID[j] = devLo;
										if(j<devLo)
										{
											
												CTStat_Dev* pTemp = new CTStat_Dev;
												_ComDeviceInfo* pInfo = new _ComDeviceInfo;
												pInfo->m_pDev = pTemp;
												pInfo->m_bConflict = TRUE;
												pInfo->m_nSourceID = devLo;
												pInfo->m_nTempID = j;
												TRACE(_T("Scan one with SRC ID = %d, New ID = %d\n"), devLo, j);

											//	temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												//temp.id=j;
												pTemp->SetDevID(j);
												nRet=read_multi2(j,&SerialNum[0],0,9,bForTStat);
												if(nRet>0)
												{		SHOW_TX_RX
													//temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													//temp.product_class_id=SerialNum[7];
													//temp.hardware_version=SerialNum[8];
													int serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													pTemp->SetSerialID(serialnumber);
													pTemp->SetProductType(SerialNum[7]);
													pTemp->SetHardwareVersion(SerialNum[8]);

													float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
													{
														tstat_version2 /=10;
													}
													else 
													{
														tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
														tstat_version2 /=10;
													}//tstat_version
													
													//temp.software_version=tstat_version2;
													pTemp->SetSoftwareVersion(tstat_version2);
													if(Read_One2(j,185, bForTStat)==0)
													//if(pTemp->ReadOneReg(185)==0)
													{
														//temp.baudrate=9600;
														pTemp->SetBaudRate(9600);
													}
													else
													{
														pTemp->SetBaudRate(19200);
													}

													int nEPsize=Read_One2(j,326, bForTStat);
													SHOW_TX_RX
													pTemp->SetEPSize(nEPsize);
											
													pTemp->SetComPort(nComPort);

													pTemp->SetBuildingName(m_strBuildingName);
													pTemp->SetSubnetName(m_strSubNet);
													//if(pTemp->GetComPort()>=0)
													//{														
													m_szTstatScandRet.push_back(pInfo);
													//}												
												}
										}
									
										binarySearchforComDevice(nComPort, bForTStat, devLo,devHi);
									}
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

//---------------------------------------------------------------------------------------------
// 搜索网口设备

// 方法是：向网络发送广播消息，网络设备接受到广播消息后立即自动回复，并告知计算机
//				  该网络设备的信息。计算机由此获得所有网络设备。
//
//步骤：1，计算机向网络发送UDP广播
//           2，建立TCP监听
//			 3，接收数据，判别数据类型
//           4，根据接受到的数据建立网络设备对象，并注册。
//		
UINT _ScanNCThread(LPVOID pParam)
{
	CTStatScanner* pScanner=(CTStatScanner*) pParam;
	if(pScanner->_ScanNCFunc() != 0)
	{
		//pScanner->AddNetToGrid();
	}



// 	if (pScanner->m_eScanNetEnd)
// 	{
// 		pScanner->m_eScanNetEnd->SetEvent();
// 	}
	
	return 1;
}

int CTStatScanner::_ScanNCFunc()
{
	//CTStatScanner* pScan = (CTStatScanner*)pParam;
	SOCKET hBroad=NULL;
	SOCKET sListen=NULL;
	IP_ADAPTER_INFO pAdapterInfo;
	ULONG len = sizeof(pAdapterInfo); 
	if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS) 
	{
		return 0;
	}
//SOCKADDR_IN sockAddress;   // commented by zgq;2010-12-06; unreferenced local variable
	UINT nGatewayIP,nLocalIP,nMaskIP;
	nGatewayIP=inet_addr(pAdapterInfo.GatewayList.IpAddress.String);
	nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);
	nMaskIP=inet_addr(pAdapterInfo.IpAddressList.IpMask.String);
	UINT nBroadCastIP;
	nBroadCastIP=(~nMaskIP)|nLocalIP;
	char* chBroadCast;
	in_addr in;
	in.S_un.S_addr=nBroadCastIP;
	chBroadCast=inet_ntoa(in);
	hBroad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(hBroad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(hBroad,FIONBIO, (u_long FAR*) &iMode);

	SOCKADDR_IN bcast;
	bcast.sin_family=AF_INET;
	//bcast.sin_addr.s_addr=nBroadCastIP;
	bcast.sin_addr.s_addr=INADDR_ANY;
	bcast.sin_port=htons(UDP_BROADCAST_PORT);
	short nmsgType=UPD_BROADCAST_QRY_MSG;
	::sendto(hBroad,(char*)&nmsgType,sizeof(short),0,(sockaddr*)&bcast,sizeof(bcast));

	Sleep(10);
	closesocket(hBroad);
	hBroad=NULL;

	sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sListen == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}
	iMode=1;
	ioctlsocket(sListen,FIONBIO, (u_long FAR*) &iMode);
	int nTimeout=1000;
	BOOL bReuse=TRUE;
	::setsockopt(sListen,SOL_SOCKET,SO_REUSEADDR,(char*)&bReuse,sizeof(BOOL));
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4321);
 	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}
	if(::listen(sListen, 5) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}
	fd_set fdSocket;
	FD_ZERO(&fdSocket);
	FD_SET(sListen, &fdSocket);
	int k=0;
	//while(pScanner->IsComScanRunning())
	while(IsComScanRunning())
	{
		//if (WaitForSingleObject(this->m_eScanNetEnd->m_hObject, 0) == WAIT_OBJECT_0 )
		if(m_bStopScan)
		{
			CString strlog=_T("Scan Stop Time: ")+Get_NowTime()+_T("\n");;
			WriteLogFile(strlog);
			break;
		}

		fd_set fdRead = fdSocket;
		int nRet = ::select(0, &fdRead, NULL, NULL, NULL);
		if(nRet > 0)
		{
			fd_set fdRead = fdSocket;
			int nRet = ::select(0, &fdRead, NULL, NULL, NULL);
			if(nRet > 0)
			{	
				for(int i=0; i<(int)fdSocket.fd_count; i++)
				{
					if(FD_ISSET(fdSocket.fd_array[i], &fdRead))
					{
						if(fdSocket.fd_array[i] == sListen)		// （1）监听套节字接收到新连接
						{
							if(fdSocket.fd_count < FD_SETSIZE)
							{
								sockaddr_in addrRemote;
								int nAddrLen = sizeof(addrRemote);
								SOCKET sNew = ::accept(sListen, (SOCKADDR*)&addrRemote, &nAddrLen);
								FD_SET(sNew, &fdSocket);
							}
							else
							{
								continue;
							}
						}
						else
						{
							unsigned short buffer[256];
							int nRecv = ::recv(fdSocket.fd_array[i], (char*)buffer, 256, 0);
							if(nRecv > 0)						// （2）可读
							{			
								if(buffer[0]==RESPONSE_MSG)
								{
									int nLen=buffer[1];
									if(nLen>=0)
									{
										k++;
										/*
										unsigned short dataPackage[11];
										memcpy(&dataPackage[0],&buffer[2],nLen*sizeof(unsigned short));
										_NCInfo ncInfo;
										ncInfo.nSerial=dataPackage[0]+dataPackage[1]*256+dataPackage[2]*256*256+dataPackage[3]*256*256*256;
										ncInfo.nProductID=dataPackage[4];
										ncInfo.modbusID=dataPackage[5];
										CString strTemp;
										strTemp.Format(_T("%d.%d.%d.%d"),dataPackage[6],dataPackage[7],dataPackage[8],dataPackage[9]);
										ncInfo.strIP=strTemp;
										ncInfo.nPort=(dataPackage[10]);
										::EnterCriticalSection(&g_Lock);
										g_NCList.push_back(ncInfo);
									
										pDlg->m_NCFlexGrid.put_Rows(g_NCList.size()+1);
										strTemp.Format(_T("%d"),ncInfo.nSerial);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,0,strTemp);

										strTemp.Format(_T("%d"),ncInfo.modbusID);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,1,strTemp);
										
										strTemp.Format(_T("%d"),ncInfo.nProductID);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,2,strTemp);

										pDlg->m_NCFlexGrid.put_TextMatrix(k,3,ncInfo.strIP);
										strTemp.Format(_T("%d"),ncInfo.nPort);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,4,strTemp);
										::LeaveCriticalSection(&g_Lock);
										*/

										unsigned short dataPackage[11];
										memcpy(&dataPackage[0],&buffer[2],nLen*sizeof(unsigned short));
										CTStat_Net* pT = new CTStat_Net;
										//
										int nComPort = dataPackage[0]+dataPackage[1]*256+dataPackage[2]*256*256+dataPackage[3]*256*256*256;
										pT->SetComPort(nComPort);
										int nProductID=dataPackage[4];
										int nModbusID=dataPackage[5];
										pT->SetDevID(nModbusID);
										pT->SetProductType(nProductID);

										DWORD dwIPAddr = 0;
										memcpy(((char*)(dwIPAddr)), (char*)(dataPackage[6]), 1);
										memcpy(((char*)(dwIPAddr))+1, (char*)(dataPackage[7]), 1);
										memcpy(((char*)(dwIPAddr))+2, (char*)(dataPackage[8]), 1);
										memcpy(((char*)(dwIPAddr))+3, (char*)(dataPackage[9]), 1);
																														
										pT->SetIPAddr(dwIPAddr);
										int nPort=(dataPackage[10]);
										pT->SetIPPort(nPort);

										//pScanner->m_szNCScanRet.push_back(pT);
										//m_szNCScanRet.push_back(pT);
										
										_NetDeviceInfo* pInfo = new _NetDeviceInfo;																														
										pInfo->m_pNet = pT;										
										m_szNCScanRet.push_back(pInfo);
									}
								}



								SHOW_TX_RX

							}
							else								
							{
								::closesocket(fdSocket.fd_array[i]);
								FD_CLR(fdSocket.fd_array[i], &fdSocket);
							}
						}
					}
				}
			}
		}///end of nret

	}//end of while
	closesocket(sListen);
	sListen=NULL;


	if (m_eScanNCEnd)
	{
		m_eScanNCEnd->SetEvent();
	}

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

	
	CString strScanInfo = _T("Start UDP scan.");
	pScanner->ShowNetScanInfo(strScanInfo); 
	CString strlog;
	strlog=_T("Start Scan By UDP Time:")+Get_NowTime()+_T("\n");;
	 NET_WriteLogFile(strlog);
	g_strScanInfoPrompt = _T("NC by UDP");
	
	TRACE(_T("Start udp scan ! \n"));
	SOCKET soAck =::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	 
	//UINT nGatewayIP,nLocalIP,nMaskIP;
	//nGatewayIP=inet_addr(pAdapterInfo.GatewayList.IpAddress.String);
	//nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);
	//nMaskIP=inet_addr(pAdapterInfo.IpAddressList.IpMask.String);
	//UINT nBroadCastIP;
	//nBroadCastIP=(~nMaskIP)|nLocalIP;
//	char* chBroadCast;
	//in_addr in;
	//in.S_un.S_addr=nBroadCastIP;
	//chBroadCast=inet_ntoa(in);
	hBroad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(hBroad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(hBroad,FIONBIO, (u_long FAR*) &iMode);

	SOCKADDR_IN bcast;
	bcast.sin_family=AF_INET;
	//bcast.sin_addr.s_addr=nBroadCastIP;
	bcast.sin_addr.s_addr=INADDR_BROADCAST;
	bcast.sin_port=htons(UDP_BROADCAST_PORT);

	SOCKADDR_IN siBind;
	siBind.sin_family=AF_INET;
	siBind.sin_addr.s_addr=INADDR_ANY;
	siBind.sin_port=htons(RECV_RESPONSE_PORT);
	int nRet = ::bind(hBroad, (sockaddr*)&siBind,sizeof(siBind));
	if (nRet == SOCKET_ERROR)
	{	   strlog=_T("Binding socket failed. Please check your net work settings.");
	      NET_WriteLogFile(strlog);
		int nError = WSAGetLastError();
		
		AfxMessageBox(strlog);
		goto END_SCAN;
		return 0;
	}
	//############################
	strScanInfo = _T("Initialize UDP network...");
	pScanner->ShowNetScanInfo(strScanInfo); 
	//############################
	NET_WriteLogFile(strScanInfo);
	short nmsgType=UPD_BROADCAST_QRY_MSG;


	//////////////////////////////////////////////////////////////////////////
	const DWORD END_FLAG = 0x00000000;
	TIMEVAL time;
	time.tv_sec =3;
	time.tv_usec = 1000;

	fd_set fdSocket;
// 	FD_ZERO(&fdSocket);
// 	FD_SET(hBroad, &fdSocket);
	//FD_SET(sListen, &fdSocket);

	BYTE buffer[512] = {0};

	BYTE pSendBuf[1024];
	ZeroMemory(pSendBuf, 255);
	pSendBuf[0] = 100;
	//pSendBuf[1] = END_FLAG;
	memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
	int nSendLen = 5;
	
	/*/////////////////////////////////////////////////////////////////////////
	BYTE t4 = 1;
	for (int nnn = 0; nnn < 100; nnn++)
	{
		BYTE t1 = 192;
		BYTE t2 = 168;
		BYTE t3 = 0;

		pSendBuf[nSendLen-4] = t1;
		pSendBuf[nSendLen-3] = t2;
		pSendBuf[nSendLen-2] = t3;
		pSendBuf[nSendLen-1] = t4;
		
		nSendLen +=4;
		t4++;
		memcpy(pSendBuf + nSendLen, (BYTE*)&END_FLAG, 4);
	}
/////////////////////////////////////////////////////////////////////////*/
	int time_out=0;
	BOOL bTimeOut = FALSE;
	while(!bTimeOut)//!pScanner->m_bNetScanFinish)  // 超时结束
	{
		time_out++;
		if(time_out>100)
			 bTimeOut = TRUE;
		if(pScanner->m_bStopScan)
		{
			CString strlog=_T("Scan Stop Time: ")+Get_NowTime()+_T("\n");
			NET_WriteLogFile(strlog);
			break;
		}
		FD_ZERO(&fdSocket);	
		FD_SET(hBroad, &fdSocket);
		//############################
		strScanInfo = _T("Send UDP scan broadcast...");
		pScanner->ShowNetScanInfo(strScanInfo); 
		NET_WriteLogFile(strScanInfo);
		//############################
		nRet = ::sendto(hBroad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&bcast,sizeof(bcast));
		if (nRet == SOCKET_ERROR)
		{
			int  nError = WSAGetLastError();
			strlog=	_T("Sending scan command failed. Please check your net work settings.");
			NET_WriteLogFile(strlog);
			AfxMessageBox(strlog);
			goto END_SCAN;
			return 0;
		}
		int nLen = sizeof(siBind);
		//while(pScanner->IsComScanRunning())

		fd_set fdRead = fdSocket;
		int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);TRACE("recv nc info == %d\n", nSelRet);
		if (nSelRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			strlog=_T("Recving scan infomation failed. Please check your net work settings.");
			NET_WriteLogFile(strlog);
			AfxMessageBox(strlog);
			goto END_SCAN;
			return 0;
		}

		if(nSelRet > 0)
		{
			ZeroMemory(buffer, 512);

			int nRet = ::recvfrom(hBroad,(char*)buffer, 512, 0, (sockaddr*)&siBind, &nLen);
//			int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
			BYTE szIPAddr[4] = {0};
			if(nRet > 0)
			{		
				FD_ZERO(&fdSocket);
				if(buffer[0]==RESPONSE_MSG)
				{	
					nLen=buffer[2]+buffer[3]*256;
					unsigned short dataPackage[32]={0};
					memcpy(dataPackage,buffer+2,nLen*sizeof(unsigned short));
					//int modbusID=dataPackage[6];
					szIPAddr[0]= (BYTE)dataPackage[7];
					szIPAddr[1]= (BYTE)dataPackage[8];
					szIPAddr[2]= (BYTE)dataPackage[9];
					szIPAddr[3]= (BYTE)dataPackage[10];

					int n = 1;
					BOOL bFlag=FALSE;
					//////////////////////////////////////////////////////////////////////////
					// 检测IP重复
					DWORD dwValidIP = 0;
					memcpy((BYTE*)&dwValidIP, pSendBuf+n, 4);
 					while(dwValidIP != END_FLAG)
					{	
						DWORD dwRecvIP=0;
						
						memcpy((BYTE*)&dwRecvIP, szIPAddr, 4);
						memcpy((BYTE*)&dwValidIP, pSendBuf+n, 4);
						
						if(dwRecvIP == dwValidIP)
						{
							bFlag = TRUE;
							break;
						}
						n+=4;
					}
					//////////////////////////////////////////////////////////////////////////
					if (!bFlag)
					{
						pScanner->AddNCToList(buffer, nRet, siBind);
						//############################
						strScanInfo = _T("Find Network device...");
						pScanner->ShowNetScanInfo(strScanInfo); 
						//############################
						//pSendBuf[nSendLen-1] = (BYTE)(modbusID);
						pSendBuf[nSendLen-4] = szIPAddr[0];
						pSendBuf[nSendLen-3] = szIPAddr[1];
						pSendBuf[nSendLen-2] = szIPAddr[2];
						pSendBuf[nSendLen-1] = szIPAddr[3];
						memcpy(pSendBuf + nSendLen, (BYTE*)&END_FLAG, 4);
						//////////////////////////////////////////////////////////////////////////
						
						//pSendBuf[nSendLen+3] = 0xFF;
						nSendLen+=4;
					}

// 					nLen=buffer[2]+buffer[3]*256;
// 					//int n =sizeof(char)+sizeof(unsigned char)+sizeof( unsigned short)*9;
// 					if(nLen>=0)
// 					{
// 						CTStat_Net* pT = new CTStat_Net;
// 						unsigned short dataPackage[13];
// 						memcpy(dataPackage,buffer+2,nLen*sizeof(unsigned short));
// 						
// 						DWORD nSerial=dataPackage[0]+dataPackage[1]*256+dataPackage[2]*256*256+dataPackage[3]*256*256*256;
// 						int nProductID=dataPackage[5];
// 						int modbusID=dataPackage[6];
// 						CString strTemp;
// 						strTemp.Format(_T("%d.%d.%d.%d"),dataPackage[7],dataPackage[8],dataPackage[9],dataPackage[10]);
// 						CString strIP=strTemp;
// 						int nPort=(dataPackage[11]);
// 												
// 						pT->SetSerialID(nSerial);
// 						pT->SetDevID(modbusID);
// 						pT->SetProductType(nProductID);
// 						pT->SetIPPort(ntohs(siBind.sin_port));
// 						LPSTR szIP = inet_ntoa(siBind.sin_addr);
// 						pT->SetIPAddr((char*)szIP);
// 
// 						_NetDeviceInfo* pni = new _NetDeviceInfo;
// 						pni->m_pNet = pT;										
// 						pScanner->m_szNCScanRet.push_back(pni);
				

					//}
				}	
			SHOW_TX_RX
			}
		}	
		else
		{
			//pScanner->ScanTstatFromNCForManual();
			g_ScnnedNum = 0;
			//pScanner->ScanTstatFromNCForAuto();
			//pScanner->m_bNetScanFinish = TRUE; // 

			//new nc//	 pScanner->GetTstatFromNCTable();
			
			bTimeOut = TRUE;
			
		}
	}//end of while
	//closesocket(sListen);

END_SCAN:

	closesocket(hBroad);
	hBroad=NULL;
	//pScanner->m_bNetScanFinish = TRUE; // 超时结束

	g_strScanInfoPrompt = _T("NC by TCP");
	strlog=_T("UDP scan finished,Time: ")+Get_NowTime()+_T("\n");
	NET_WriteLogFile(strlog);
	pScanner->m_eScanNCEnd->SetEvent();
	
	TRACE(_T("UDP End Scan! \n"));

	//############################
	strScanInfo = _T("UDP scan finished!");
	pScanner->ShowNetScanInfo(strScanInfo); 

	//############################

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

		CString strlog;
		strlog.Format(_T("Find a Net Device,modbus ID=%d ,Serial No=%d"),modbusID,nSerial);
		strlog+=_T(" IP=");
		strlog+=strIP;
		NET_WriteLogFile(strlog);

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
	 	

	UINT i = 0;

	for (i = 0; i < pScan->m_szComs.size(); i++)
	{
		

		if(pScan->m_bStopScan)
		{ // 有信号，就结束scan
			//g_ScnnedNum=254;
			//return;
			//WriteLogFile(_T("Scan stop"));
			CString strlog=_T("Scan Stop Time: ")+GetCurrentTime();
			WriteLogFile(strlog);
			break;
		}
		CString strComPort = pScan->m_szComs[i];
		TRACE(_T("Scanning @ ") + strComPort + _T("\n"));
		
		 CString strlog=_T("Scanning @")+strComPort+_T(" ")+Get_NowTime()+_T("\n");
		 WriteLogFile(strlog);

		CString tc = strComPort.Mid(3);

		int n = _wtoi(tc);
// 		char c[2];
// 		_itoa_s(n, c, 10);
// 		c[0] = '7';

		if(pScan->OpenCom(n))
		{
			pScan->SetComPort(n);
			bool bRet = Change_BaudRate(pScan->m_nBaudrate);
			strlog.Empty();
			strlog.Format(_T("Change BaudRate:%d"),pScan->m_nBaudrate);
			WriteLogFile(strlog);
			ASSERT(bRet);
			g_strScanInfoPrompt.Format(_T("COM%d"), n);
			//It's unnecessary to check one device. annulled by Fance. 
			//We encountered such a situation that ,some device scan commond is not works well,if it only exsit on the modbus.
			//it can be scan,but if other device also on modbus line , it will can't scan.
			//So ,in order to check device works well, delete this code can help developer test our product.
		//	BOOL bReadone = pScan->ReadOneCheckOnline(n);
		//	if (!bReadone)//comscan oldcode
			//{
			//	pScan->background_binarysearch(n);				
			//}		
			strlog.Empty();
			strlog.Format(_T("Success to Open the COM%d"),n);
			WriteLogFile(strlog); 

			pScan->background_binarysearch(n);	//lsc comscan new cold
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
			WriteLogFile(str);
			SetPaneString(2, str);
			int n =0 ;
		}

	}
	g_ScnnedNum=254;
	//
	//pScan->AddComDeviceToGrid();

	close_com();
	TRACE(_T("COM Scan finished"));
	CString strlog;
	strlog=_T("Com port Scan Finished.Time: ")+Get_NowTime()+_T("\n");
	WriteLogFile( strlog);
	if (pScan->m_eScanComEnd->m_hObject)
	{
		pScan->m_eScanComEnd->SetEvent();
		pScan->m_com_scan_end = true;
	}

	return 1;
}

//*/

UINT _ScanTstatThread(LPVOID pParam)
{
	CTStatScanner* pScan = (CTStatScanner*)(pParam);
	 	

 	if(pScan->m_bStopScan)
 	{ // 有信号，就结束scan
 		//g_ScnnedNum=254;
 		//return;
		CString strlog=_T("Scan Stop Time: ")+Get_NowTime()+_T("\n");
		WriteLogFile(strlog);
 		return 1;
 	}

	if(pScan->OpenCom(pScan->m_nComPort))
	{
			bool bRet = Change_BaudRate(pScan->m_nBaudrate);
			ASSERT(bRet);
			g_strScanInfoPrompt.Format(_T("COM%d"), pScan->m_nComPort);
			pScan->OneByOneSearchforComDevice(pScan->m_nComPort,FALSE,g_nStartID,g_nEndID);

			close_com();
			Sleep(1000);
			TRACE(_T("Success open the COM%d\n"), pScan->m_nComPort); 
		}
		else
		{
			// 不能打开串口X，提示信息
			TRACE(_T("Cannot open the COM%d\n"), pScan->m_nComPort);
			CString str;
			str.Format(_T("Cannot open the COM%d\n"), pScan->m_nComPort);
			SetPaneString(2, str);
			//int n =0 ;
		}
	
	g_ScnnedNum=254;
	//
	//pScan->AddComDeviceToGrid();
	if (pScan->m_eScanComOneByOneEnd->m_hObject)
	{
		pScan->m_eScanComOneByOneEnd->SetEvent();
	}

	return 1;
}


/*
BOOL CTStatScanner::GetAllComPort()
{
	LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	HKEY   hKey;   
	LONG   lReturnCode=0;   
	lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);   

	if(lReturnCode==ERROR_SUCCESS)   
	{  
		DWORD dwIndex = 0;
		WCHAR lpValueName[MAX_PATH];
		ZeroMemory(lpValueName, MAX_PATH);
		DWORD lpcchValueName = MAX_PATH; 
		DWORD lpReserved = 0; 
		DWORD lpType = REG_SZ; 
		BYTE		lpData[MAX_PATH]; 
		ZeroMemory(lpData, MAX_PATH);
		DWORD lpcbData = MAX_PATH;
		dwIndex = 0;
		while(RegEnumValue(	hKey, dwIndex, lpValueName, &lpcchValueName, 0, &lpType, lpData, &lpcbData ) != ERROR_NO_MORE_ITEMS)
		{   
			//TRACE("Registry's   Read!");   
			dwIndex++;

			lpcchValueName = MAX_PATH; 
			lpValueName[0] = '\0'; 

			WCHAR ch[10];
			ZeroMemory(ch, 10);
			memcpy(ch, lpData, 10);
			CString str = CString(ch);
			m_szComs.push_back(str);
		}   
		::RegCloseKey(hKey);   		   

		return m_szComs.size();
	}

	return -1;   
}
*/

void CTStatScanner::SetParentWnd(CWnd* pParent)
{
	m_pParent = pParent;
}

void CTStatScanner::AddNetToGrid()
{
	//m_pParent->AddNetDeviceToGrid(m_szNCScanRet);
	m_pParent->PostMessage(WM_ADDNETSCAN);
}

void CTStatScanner::AddComDeviceToGrid()
{
	//m_pParent->AddComDeviceToGrid(m_szTsatScandRet);
//	m_pParent->PostMessage(WM_ADDCOMSCAN);
}


void CTStatScanner::SendScanEndMsg()
{
	//m_pParent->PostMessage(WM_SCANFINISH, 0, 0);
	// scan完成，开始冲突检查
		
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
   // below is nc scan handle
	// 找到nc与数据库的冲突
//	else
	//{
// 		FindNetConflict();
// 		ResolveNetConflict();
// 		AddNewNetToDB();
	/*}*/


	((CMainFrame*)m_pParent)->m_bScanALL = FALSE;
	((CMainFrame*)m_pParent)->m_bScanFinished = TRUE;


	if(m_szNCScanRet.size()==0 && m_szTstatScandRet.size() == 0)
	{
		//AfxMessageBox(_T("Can't find any device. Please check configure and connection, then try again."));//scan,在有些机子上，总提示这个，但来会继续进行扫描
		m_pParent->PostMessage(WM_ADDTREENODE);
		return ;
	}

	//AfxMessageBox(_T("Step 6"));

	//closefile();//scan
	
	try
	{

	CScanDlg dlg;
	dlg.SetScanner(this);
	//dlg.AddComDeviceToGrid(m_szTsatScandRet);
	dlg.DoModal();
 	if(IsWindow(dlg.m_hWnd))
 	{
 		WaitForSingleObject(dlg.m_hWnd, 200);  // 为了线程安全。
 	}

	}
	catch (...)
	{

	}

		m_pParent->PostMessage(WM_ADDTREENODE);
		 CString strlog;

		 strlog=_T("Scan Stop Time: ")+Get_NowTime()+_T("\n");;
		 WriteLogFile(strlog);
		 NET_WriteLogFile(strlog);
		  strlog=_T("\n");
		  WriteLogFile(strlog);
		  NET_WriteLogFile(strlog);
		strlog=_T("--------------------------------Scan End--------------------------------");

			WriteLogFile(strlog);
		NET_WriteLogFile(strlog);

		

		CloseLogFile();
		NET_CloseLogFile();
		SetCommunicationType(1);
		close_com();
		SetCommunicationType(0);
		close_com();
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
	try
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
			
			 CString str_product_id = m_pRs->GetCollect("Product_class_ID");
			 if((_wtoi(str_product_id) == 35) ||(_wtoi(str_product_id) ==50))
			 {
				 CString strprotocol = m_pRs->GetCollect("Protocol");
				 ((CTStat_Net*)(pNode))->SetProtocol(_wtoi(strprotocol));
			 }
			 else
			 {
				  ((CTStat_Net*)(pNode))->SetProtocol(1);//1为modebus ip
			 }


			// IP Addr
			CString strIP = m_pRs->GetCollect("Bautrate");

			//char temp_char[50];
			//memset(temp_char,0,255);
			//WideCharToMultiByte( CP_ACP, 0, strIP.GetBuffer(), -1, temp_char, 50, NULL, NULL );

			USES_CONVERSION;
			LPCSTR szIP = W2A(strIP);
			DWORD dwIP = inet_addr(szIP);
			//DWORD dwIP = inet_addr(temp_char);
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

	}
	catch (...)
	{

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
				//加上 buildingname等私货	
				pInfo->m_pDev->SetBuildingName(m_szComNodes[j]->GetBuildingName());
				pInfo->m_pDev->SetFloorName(m_szComNodes[j]->GetFloorName());
				pInfo->m_pDev->SetRoomName(m_szComNodes[j]->GetRoomName());	
				pInfo->m_pDev->SetSubnetName(m_szComNodes[j]->GetSubnetName());	
				if (nAddr != nRtAddr) // 需要矫正
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


// 解决冲突
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
				try
				{

				strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),strID,strSerial);
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


//////////////////////////////////////////////////////////////////////////
// 如果Serial ID相同，而IP不同，那么修改数据库中的ID
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
				try
				{

				//strSql.Format(_T("delete ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),strID,strSerial);
				strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s'"), strSerial);
				t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
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

		//if((m_szTstatScandRet[i]->m_pDev->GetProductType() == 50) || (m_szTstatScandRet[i]->m_pDev->GetProductType() == 35))
		//{
		//	for (UINT j = 0; j < m_szNetNodes.size(); j++)
		//	{
		//		int nNodeSID = m_szNetNodes[j]->GetSerialID();
		//		if (nSID == nNodeSID)
		//		{
		//			bIsNew = FALSE;
		//			break;
		//		}
		//	}

		//}
		if (bIsNew == TRUE)
		{
			WriteOneDevInfoToDB(m_szTstatScandRet[i]);
		}
	}
}


void CTStatScanner::AddNewNetToDB()
{
	BOOL bIsNew = TRUE;
	for(UINT i = 0; i < m_szNCScanRet.size(); i++)
	{
		bIsNew = TRUE;
		_NetDeviceInfo* pInfo = m_szNCScanRet[i];
		pInfo->m_pNet->SetBuildingName(m_strBuildingName);
		pInfo->m_pNet->SetSubnetName(m_strSubNet);
		pInfo->m_pNet->SetFloorName(m_strFloorName);
		pInfo->m_pNet->SetRoomName(m_strRoomName);

		int nSID = pInfo->m_pNet->GetSerialID();
		int nSProtocol = pInfo->m_pNet->GetProtocol();
		for (UINT j = 0; j < m_szNetNodes.size(); j++)
		{
			int nNodeSID = m_szNetNodes[j]->GetSerialID();
			//Comment by Fance
			//if the scan device is CM5 or minipanel, this products has 3 protocol, BacnetIP modbus485 modbus tcp;
			//So when scan bacnet ip and midbus tcp ,the or replay to t3000,
			//So I display the device in two format,judge to 2 decvice;
			if((pInfo->m_pNet->GetProductType() == 50) || (pInfo->m_pNet->GetProductType() == 35))
			{
				int nNodeSID = m_szNetNodes[j]->GetSerialID();
				int nNodeProtocol = m_szNetNodes[j]->GetProtocol();
				if ((nSID == nNodeSID)&&(nSProtocol = nNodeProtocol))
				{
					bIsNew = FALSE;
					break;
				}
			}
			else
			{
				if (nSID == nNodeSID)
				{
					bIsNew = FALSE;
					break;
				}
			}
		}
#if 0
		if((pInfo->m_pNet->GetProductType() == 50) || (pInfo->m_pNet->GetProductType() == 35))
		{
			for (UINT j = 0; j < m_szComNodes.size(); j++)
			{
				int nNodeSID = m_szComNodes[j]->GetSerialID();
				int nNodeProtocol = m_szComNodes[j]->GetProtocol();
				if ((nSID == nNodeSID)&&(nSProtocol = nNodeProtocol))
				{
					bIsNew = FALSE;
					break;
				}
			}

		}
#endif

		if (bIsNew == TRUE)
		{
			WriteOneNetInfoToDB(pInfo);
		}
	}
}

void CTStatScanner::WriteOneNetInfoToDB( _NetDeviceInfo* pInfo)
{
	ASSERT(pInfo);
// 	_ConnectionPtr t_pCon;//for ado connection
//  	t_pCon.CreateInstance(_T("ADODB.Connection"));
//  	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);


	// Modbus ID
	CString strID;
	int nID = pInfo->m_pNet->GetDevID();	
	strID.Format(_T("%d"),  nID);

	CString strProductName = pInfo->m_pNet->GetProductName();

	CString strSerialID;
	strSerialID.Format(_T("%d"), pInfo->m_pNet->GetSerialID());

	int nClassID = pInfo->m_pNet->GetProductType();
	CString strClassID;
	strClassID.Format(_T("%d"), nClassID);
	
	CString strScreenName;
	strScreenName.Format(_T("Screen(S:%d--%d)"), pInfo->m_pNet->GetSerialID(), pInfo->m_pNet->GetDevID() );

	CString strBackground_bmp=_T("Clicking here to add a image...");

	CString strHWV;
	strHWV.Format(_T("%0.1f"), pInfo->m_pNet->GetHardwareVersion());

	CString strSWV;
	strSWV.Format(_T("%0.1f"), pInfo->m_pNet->GetSoftwareVersion());

// 	CString strCom;
// 	strCom.Format(_T("COM%d"), pInfo->m_pNet->GetComPort());

	CString strSql;
	//	CString strSubNetName;

	//CString strEpSize;
	//strEpSize.Format(_T("%d"), pInfo->m_pNet->GetEPSize());

	CString strIP;
	in_addr ia;
	ia.S_un.S_addr = pInfo->m_pNet->GetIPAddr();
	strIP = CString(inet_ntoa(ia));	
	//m_ip = strIP;//scan

	CString strPort;
	strPort.Format(_T("%d"), pInfo->m_pNet->GetIPPort());
	//m_port = strPort;//scan



	CString strSubnetName = m_strSubNet;
	//GetNetDevSubnetName(strIP);
	
	CString strEPSize;
	if(pInfo->m_pNet->GetProtocol()==3)//如果是bacnetip 需要往数据库里保存 协议 3 就是bacnetip;
	{
		CString temp_pro = _T("3");
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol)					  values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"','"+temp_pro+"')"));
	}
	else
	{
		CString temp_pro = _T("1");// protocol type 1 is modbus tcp
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol)					  values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"','"+temp_pro+"')"));
	}
	//strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize)					  values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"')"));
//new nc//  strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize, Mainnet_info) values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEpSize+"','"+strMainnetInfo+"')"));
	try
	{

		t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

void CTStatScanner::WriteOneDevInfoToDB( _ComDeviceInfo* pInfo)
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
try
{

	CString strSql;
	CString strEpSize;
	strEpSize.Format(_T("%d"), pInfo->m_pDev->GetEPSize());


	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"')"));
	//new nc// strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize, Mainnet_info) values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"','"+strMainnetInfo+"')"));
	t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
}
catch (...)
{

}

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

	return (g_ScnnedNum >= 254 && m_bNetScanFinish);

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

	m_bStopScan = TRUE;
}


void CTStatScanner::SetSubnetInfo(vector<Building_info>& szSubnets)
{
	m_szSubnetsInfo = szSubnets;
}

CString CTStatScanner::GetTstatSubnetName(const CString& strComPort)
{
	CString strSubnetName = _T("Subnet_Undefine");
	for (UINT i = 0; i < m_szSubnetsInfo.size(); i++)
	{
		if (strComPort.CompareNoCase(m_szSubnetsInfo[i].strComPort) == 0)
		{
			return m_szSubnetsInfo[i].strBuildingName;
		}			 
	}
	return strSubnetName;
}

CString CTStatScanner::GetTstatMBuildingName(const CString& strComPort)
{
	CString strMBuildingName = _T("Building_Undef");
	for (UINT i = 0; i < m_szSubnetsInfo.size(); i++)
	{
		if (strComPort.CompareNoCase(m_szSubnetsInfo[i].strComPort) == 0)
		{
			return m_szSubnetsInfo[i].strMainBuildingname;
		}			 
	}
	return strMBuildingName;
}



CString CTStatScanner::GetNetDevSubnetName(const CString& strIP)
{
 	CString strSubnetName = _T("Subnet_Undefine");
// 	for (UINT i = 0; i < m_szSubnetsInfo.size(); i++)
// 	{
// 		if (strIP.CompareNoCase(m_szSubnetsInfo[i].strIp) == 0)
// 		{
// 			return m_szSubnetsInfo[i].strBuildingName;
// 		}			 
// 	}
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
		//int ntempID=0;
		//BOOL bFindSameID=false;
		//int nPos=-1;
//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=read_multi2(nTstatID, &SerialNum[0],0,9,true);

		if(nRet>0)
		{
			CTStat_Dev* pTempDev = new CTStat_Dev;
			_ComDeviceInfo* pDevInfo = new _ComDeviceInfo;
			pDevInfo->m_pDev = pTempDev;

			if(IsRepeatedID(nTstatID))
			{
				TRACE(_T("Scan one with Repeated ID = %d\n"), nTstatID);
				pDevInfo->m_bConflict = TRUE;								
				pDevInfo->m_nSourceID = m_szRepeatedID[nTstatID];
				pDevInfo->m_nTempID = nTstatID;
			}
			else
			{
				pDevInfo->m_bConflict = FALSE;
				pDevInfo->m_nSourceID = nTstatID;
				pDevInfo->m_nTempID = nTstatID;
			}


			m_szTstatScandRet.push_back(pDevInfo);
			// 			temp.id=a;
			// 			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			pTempDev->SetSerialID(nSerialNumber);
			// 			temp.product_class_id=SerialNum[7];
			// 			temp.hardware_version=SerialNum[8];
			pTempDev->SetDevID(nTstatID);

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
			pTempDev->SetSoftwareVersion(tstat_version2);
			if(Read_One2(nTstatID, 185, true)==0)	
				//if(pTemp->ReadOneReg(185)==0)
			{
				//temp.baudrate=9600;
				pTempDev->SetBaudRate(9600);
			}
			else
			{
				//temp.baudrate=19200;
				pTempDev->SetBaudRate(19200);
			}
			//temp.nEPsize=Read_One2(temp.id,326);
			pTempDev->SetEPSize(pTempDev->ReadOneReg(326));

			//if(pTemp->GetComPort()>=0)
			// product type
			//pTemp->ReadOneReg(8);
			pTempDev->SetProductType(SerialNum[7]);

			// hardware_version
			pTempDev->SetHardwareVersion(SerialNum[8]);		

		}

}



// 手动搜索，其实是指按ID依次轮询搜索。
void CTStatScanner::ScanTstatFromNCForManual()
{	
	vector<int>  szTStatIDs;

	for(UINT n = 0; n < m_szNCScanRet.size(); n++)
	{
		_NetDeviceInfo* pNCInfo = m_szNCScanRet[n];
		CString strPort;
		int nIPPort=pNCInfo->m_pNet->GetIPPort();

		CString strIP=pNCInfo->m_pNet->GetIPAddrStr();


		g_CommunicationType=1;
		SetCommunicationType(g_CommunicationType);
		bool b=Open_Socket2(strIP, TCP_COMM_PORT);
		CString strInfo;
		//	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
		//	SetPaneString(1,strInfo);
		if(b)
		{	
			strInfo.Format((_T("Connect to IP : %s successful")), strIP);//prompt info;
			SetPaneString(1,strInfo);
		}
		else
		{
			strInfo.Format((_T("Connect to IP : %s failure")), strIP);//prompt info;
			SetPaneString(1,strInfo);
			continue;
		}

		int nIDStart = 245;
		int nIDEnd = 254;

		int nTStatID = 0; 
		int nBraudRate = 0;
		for(int i=nIDStart; i <= nIDEnd; i++)
		{
			g_strScanInfoPrompt.Format(_T("Tstat ID %d"), i);
			nBraudRate=19200;
			if (g_CommunicationType==0)
			{
				nBraudRate=19200;
				Change_BaudRate(19200);
				nTStatID=Read_One2(i,6, true);
				if(nTStatID<=0)
				{
					nBraudRate=9600;
					Change_BaudRate(9600);
					nTStatID=Read_One2(i,6, true);
				}
				szTStatIDs.push_back(nTStatID);
			}
			else if (g_CommunicationType==1)
			{
				nTStatID=Read_One2(i,6, true);
			}

			if(nTStatID>0)
			{
				szTStatIDs.push_back(nTStatID);
				GetTstatInfoFromID(nTStatID);	
			}
		}
	}
		g_ScnnedNum=254;
//*/
}


///*
// 使用485 scan 广播来查找
void CTStatScanner::ScanTstatFromNCForAuto()//scan 分别扫描各个NC中的TSTAT
{	 //Alex_Flag
	//Createfile();
	for(UINT n = 0; n < m_szNCScanRet.size(); n++)
	{
		_NetDeviceInfo* pNCInfo = m_szNCScanRet[n];
		CString strPort;
		int nIPPort=pNCInfo->m_pNet->GetIPPort();

		CString strIP=pNCInfo->m_pNet->GetIPAddrStr();
		//if(strIP.CompareNoCase(_T("192.168.0.145"))!=0)
		//	continue;
		 CString strlog;
		 strlog.Format(_T("Scan Tstat connect to %s IPAdd: %s IPPort: %d"),pNCInfo->m_pNet->GetProductName(),strIP,nIPPort);
         NET_WriteLogFile(strlog);
		//##############################
		CString strInfo;
		strInfo.Format(_T("Scan Tstat connected to %s"), pNCInfo->m_pNet->GetProductName());
		ShowNetScanInfo(strInfo);
		//##############################

		m_port.Format(_T("%d"),nIPPort);

		m_ip = strIP;
		


		g_CommunicationType=1;
		SetCommunicationType(g_CommunicationType);
		bool b=Open_Socket2(strIP, TCP_COMM_PORT);
		if(b)
		{	
			strInfo.Format((_T("Connect to IP : %s successful")), strIP);//prompt info;
			NET_WriteLogFile(strInfo);
			SetPaneString(1,strInfo);
		}
		else
		{
			strInfo.Format((_T("Connect to IP : %s failure")), strIP);//prompt info;
			NET_WriteLogFile(strInfo);
			SetPaneString(1,strInfo);
			continue;
		}

	//	BinaryScanTstatFromNC();
//		SetCommunicationType(1);
		//binarySearchforComDevice(-1, TRUE, 1, 254);
		binarySearchforComDevice(-1, TRUE, 1, 254);	

		//##############################
		   //Alex.....
		strInfo.Format(_T("Scan Tstat connected to %s finished."),pNCInfo->m_pNet->GetProductName() );
		NET_WriteLogFile(strInfo);
		ShowNetScanInfo(strInfo);
		//##############################
	}

	g_ScnnedNum=254;
}


//void CTStatScanner::BinaryScanTstatFromNC(BYTE devLo, BYTE devHi)
//void CTStatScanner::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)

//////////////////////////////////////////////////////////////////////////
// 使用命令字26，做为NC 串口Scan的命令，发送广播Scan NC
// 这个函数模拟TStat的广播协议来分段scan NC
// 注意：是串口scan NC
void CTStatScanner::BinaryScanNCByComPort(BYTE devLo, BYTE devHi)//lsc
{
	g_strScanInfoPrompt.Format(_T("NC by COM"));
	int a=NetController_CheckTstatOnline_a(devLo,devHi, false);

	//int kk=Read_One2(255,7);
	TRACE(_T("L:%d   H:%d  a:%d\n"),devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	CString temp=_T("");

	int nCount = 0;
	if(a>0)
	{
		_NetDeviceInfo* pInfo = new _NetDeviceInfo;	
		CTStat_Net* pTemp = new CTStat_Net;
		pInfo->m_pNet = pTemp;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		int nSourceID=a;
		pInfo->m_pNet->SetDevID(a);
		nRet=read_multi2(a,  &SerialNum[0],0,9,false);
		if(nRet>0)
		{
			if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
			{
				srand((unsigned)time(NULL)); 
				SerialNum[0]=rand()%255; 
				SerialNum[1]=rand()%255; 
				SerialNum[2]=rand()%255; 
				SerialNum[3]=rand()%255; 

				Write_One2(nSourceID,0,SerialNum[0], false);
				Write_One2(nSourceID,1,SerialNum[1], false);
				Write_One2(nSourceID,2,SerialNum[2], false);
				Write_One2(nSourceID,3,SerialNum[3], false);
			}

			DWORD serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			pInfo->m_pNet->SetSerialID(serialnumber);
			pInfo->m_pNet->SetProductType(SerialNum[7]);
			pInfo->m_pNet->SetHardwareVersion(SerialNum[8]); //temp.hardware_version=SerialNum[8];
				
			float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
				tstat_version2 /=10;
			}//tstat_version
				
			pInfo->m_pNet->SetSoftwareVersion(tstat_version2); 				//temp.software_version=tstat_version2;
			int nTstatBaudrate = 19200;
			if(Read_One2(nSourceID, 185, false)==0)
					nTstatBaudrate=9600;
			pInfo->m_pNet->SetBaudRate(nTstatBaudrate);
			pInfo->m_pNet->SetBuildingName(m_strBuildingName);
			pInfo->m_pNet->SetSubnetName(m_strSubNet);
			
			//获得IP地址
			WORD szIP[4] = {0};
			nRet=read_multi2(a,  &szIP[0],107,4,false);
			if(nRet>0)
			{
				in_addr ia;
				ia.S_un.S_un_b.s_b1 = (UCHAR)szIP[0];
				ia.S_un.S_un_b.s_b2 = (UCHAR)szIP[1];
				ia.S_un.S_un_b.s_b3 = (UCHAR)szIP[2];
				ia.S_un.S_un_b.s_b4 = (UCHAR)szIP[3];

				char* szStrIP = inet_ntoa(ia);
				CString strIP(szStrIP);
				pInfo->m_pNet->SetIPAddr(szStrIP);					
			}
			// port
			int nPort=Read_One2(nSourceID,120, false);
			pInfo->m_pNet->SetIPPort(nPort);

			//int nEPsize=Read_One2(nSourceID,326, false);
			//pInfo->m_pNet->SetEPSize(nEPsize);
			
			if(serialnumber>=0)
				m_szNCScanRet.push_back(pInfo);
		}

	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			BinaryScanNCByComPort(devLo,(devLo+devHi)/2);
			BinaryScanNCByComPort((devLo+devHi)/2+1,devHi);
		}
		else
			BinaryScanNCByComPort(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			BinaryScanNCByComPort(devLo,(devLo+devHi)/2);
			BinaryScanNCByComPort((devLo+devHi)/2+1,devHi);
		}
		else
		{//Two Tstat have the same ID,fewness
			do
			{

				nCount++;
				if(nCount>=5)
				{
					nCount=0;
					break;
				}

				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
				if(Read_One(devLo,10)!=-2)//one times
				{
					Sleep(100);
					CString str_temp;
					for(int j=254;j>=1;j--)
					{
						if(j!=devLo)
						{							
						//	if(!found_same_net_work_controller_by_mac(a))
							if(1)
							{
								bool find=false;//false==no find;true==find
								for(UINT w=0;w<m_szNCScanRet.size();w++)
									if(j==m_szNCScanRet.at(w)->m_pNet->GetDevID())
									{
										find=true;
										break;
									}
									if(find==false)
									{
										//************************change the Id
										//	Sleep(20);//////////////////////////////////for running is better
										//if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
										if(Write_One2(devLo,10,j,false)>0)
										{
											if(j<devLo)
											{
												//binary_search_result temp;
												_NetDeviceInfo* pInfo = new _NetDeviceInfo;
												CTStat_Net* pTemp = new CTStat_Net;
												pInfo->m_pNet = pTemp;
											//	temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												int nSourceID = j;
												pInfo->m_pNet->SetDevID(j);
												nRet=read_multi2(nSourceID,&SerialNum[0],0,9,false);

												if(nRet>0)
												{
													if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
													{
														srand((unsigned)time(NULL)); 
														SerialNum[0]=rand()%255; 
														SerialNum[1]=rand()%255; 
														SerialNum[2]=rand()%255; 
														SerialNum[3]=rand()%255; 

														Write_One2(nSourceID,0,SerialNum[0], false);
														Write_One2(nSourceID,1,SerialNum[1], false);
														Write_One2(nSourceID,2,SerialNum[2], false);
														Write_One2(nSourceID,3,SerialNum[3], false);
													}

													DWORD serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													pInfo->m_pNet->SetSerialID(serialnumber);
													pInfo->m_pNet->SetProductType(SerialNum[7]);
													pInfo->m_pNet->SetHardwareVersion(SerialNum[8]); //temp.hardware_version=SerialNum[8];

													float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
													{	tstat_version2 /=10;}
													else 
													{
														tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
														tstat_version2 /=10;
													}//tstat_version

													pInfo->m_pNet->SetSoftwareVersion(tstat_version2); 				//temp.software_version=tstat_version2;
													int nTstatBaudrate = 19200;
													if(Read_One2(nSourceID, 185, false)==0)
													{nTstatBaudrate=9600;}
													pInfo->m_pNet->SetBaudRate(nTstatBaudrate);
													pInfo->m_pNet->SetBuildingName(m_strBuildingName);
													pInfo->m_pNet->SetSubnetName(m_strSubNet);
													//int nEPsize=Read_One2(nSourceID,326, false);
													//pInfo->m_pNet->SetEPSize(nEPsize);

													if(serialnumber>=0)
													{m_szNCScanRet.push_back(pInfo);}
												} // if(nRet>...
											} // if(j<...
											BinaryScanNCByComPort(devLo,devHi);
											return;
									} // wirte_one..
								} // if(find...
							} // if(1)
							else
							{
								return;
							}
						}  // if(j!=devLo)
					} // for(j=254....
				} // if(read_one...
			}while(1);  //  do while
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}



//*/



// 代码改编自void CMainFrame::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
UINT _ScanOldNC(LPVOID pParam)
{
	CString strP = _T("Modbus TCP");
	CString strlog=_T("Scan By Modbus TCP Time: ")+Get_NowTime()+_T("\n");
	WriteLogFile(strlog);
	CTStatScanner *pScan = (CTStatScanner*) pParam;
	Building_info bi = ((CMainFrame*)(pScan->m_pParent))->m_subNetLst.at(((CMainFrame*)(pScan->m_pParent))->m_nCurSubBuildingIndex);
	
	if (bi.strProtocol.CompareNoCase(strP) != 0)
	{
		pScan->m_eScanOldNCEnd->SetEvent();
		return 0;
	}
	WaitForSingleObject(pScan->m_eScanNCEnd, INFINITE);
	

	pScan->ScanOldNC(1,254);

	return 1;
}


// 代码改编自void CMainFrame::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
void CTStatScanner::ScanOldNC(BYTE devLo, BYTE devHi)
{
	TRACE(_T("start TCP scan^^^^^^^^^ \n"));
	 CString strlog=_T("start TCP scan....... \n");
	 WriteLogFile(strlog);
	int a=NetController_CheckTstatOnline_a(1,254, true);
	//int kk=Read_One2(255,7);
	//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);


	if(binary_search_crc(a))
		return ;
	CString temp=_T("");
	if(a>0)
	{
			_NetDeviceInfo* pTemp = new _NetDeviceInfo;
			CTStat_Net* pNet = new CTStat_Net;

			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			pNet->SetDevID(a);
			nRet=read_multi2(a,&SerialNum[0],0,9,true);
			if(nRet>0)
			{
				if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
				{
					srand((unsigned)time(NULL)); 
					SerialNum[0]=rand()%255; 
					SerialNum[1]=rand()%255; 
					SerialNum[2]=rand()%255; 
					SerialNum[3]=rand()%255; 

					Write_One2(a,0,SerialNum[0],true);
					Write_One2(a,1,SerialNum[1],true);
					Write_One2(a,2,SerialNum[2],true);
					Write_One2(a,3,SerialNum[3],true);
				}
				DWORD serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
				pNet->SetSerialID(serialnumber);
				// IP
				nRet=read_multi2(a,&SerialNum[0],107,4,true);
				IN_ADDR ia;
				ia.S_un.S_un_b.s_b1 = (UCHAR)SerialNum[0];
				ia.S_un.S_un_b.s_b2 = (UCHAR)SerialNum[1];
				ia.S_un.S_un_b.s_b3 = (UCHAR)SerialNum[2];
				ia.S_un.S_un_b.s_b4 = (UCHAR)SerialNum[3];
				pNet->SetIPAddr(ia.S_un.S_addr);

				// port
				int nPort=Read_One2(a,120, true);
				pNet->SetIPPort(nPort);
				//
				pNet->SetProductType(SerialNum[7]);
				pNet->SetHardwareVersion(SerialNum[8]);
				float tstat_version2;
				tstat_version2=SerialNum[4];//tstat version			
				if(tstat_version2 >=240 && tstat_version2 <250)
					tstat_version2 /=10;
				else 
				{
					tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
					tstat_version2 /=10;
				}//tstat_version
			
				pNet->SetSoftwareVersion(tstat_version2);
				if(Read_One2(a,185, true)==0)
					pNet->SetBaudRate(9600);
				else
				{
					pNet->SetBaudRate(19200);
				}
				//int nEPsize=Read_One2(a,326, true);
				//pNet->SetEPSize(nEPsize);
				CString strlog;
				strlog.Format(_T("Find one ID=%d,Serial No=%d IPAdd=%s Port=%d"),a,SerialNum,ia.S_un.S_addr,nPort);
				WriteLogFile(strlog);
				if(serialnumber>=0)
				{
					pTemp->m_pNet = pNet;
					m_szNCScanRet.push_back(pTemp);			
				}
		}
	}

	strlog=_T("End TCP Scan..........Time:")+Get_NowTime()+_T("\n");
	 WriteLogFile(strlog);
	m_eScanOldNCEnd->SetEvent();

	TRACE(_T("End TCP scan^^^^^^^^^ \n"));
}




void CTStatScanner::ScanAll()
{	
	
#if 1	
	ScanComDevice();
	
	ScanNetworkDevice();
#endif	
	ScanBacnetComDevice();
	
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
 		if ((WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
 			&&(WaitForSingleObject(pScanner->m_eScanOldNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))
			Flag = TRUE;
		break;
	case 3:

		//if((WaitForSingleObject(pScanner->m_eScanBacnetComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))
		//	Sleep(1);

		#if 1
		if ((WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
			&&(WaitForSingleObject(pScanner->m_eScanOldNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
			&& (WaitForSingleObject(pScanner->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
			&&  (WaitForSingleObject(pScanner->m_eScanBacnetComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))

			Flag = TRUE;
		#endif
		break;
	case 4:

			if (WaitForSingleObject(pScanner->m_eScanComOneByOneEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
				Flag = FALSE;
			break;

	}
		if (Flag)
		{
			//////////////////////////////////////////////////////////////////////////
			// scan tstat
			g_strScanInfoPrompt = _T("TStat connected with NC");
			NET_WriteLogFile(g_strScanInfoPrompt);
			g_nStartID = 1;
			  //Alex_Flag

			pScanner->ScanTstatFromNCForAuto();	

			pScanner->m_bNetScanFinish = TRUE; // at this time, two thread end, all scan end
			pScanner->SendScanEndMsg();
		}
		else
		{
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
	CString* pstrInfo = new CString(strInfo);
	if(((CMainFrame*)(m_pParent))->m_pWaitScanDlg)
		PostMessage(((CMainFrame*)(m_pParent))->m_pWaitScanDlg->m_hWnd, WM_COMSCANINFO, WPARAM(pstrInfo), LPARAM(0));
}

void CTStatScanner::ShowBacnetComScanInfo(const CString& strInfo)
{
	CString* pstrInfo = new CString(strInfo);
	if(((CMainFrame*)(m_pParent))->m_pWaitScanDlg)
		PostMessage(((CMainFrame*)(m_pParent))->m_pWaitScanDlg->m_hWnd, WM_BACNETCOMSCANINFO, WPARAM(pstrInfo), LPARAM(0));
}















//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CTStatScanner::GetTstatFromNCTableByComport()
{
	// 清除已经在udp中搜索过的
	for (UINT i = 0; i < m_szNCScanRet.size(); i++)
	{
		_NetDeviceInfo* pDev = m_szNCScanRet[i];
		for (UINT j = i+1; j < m_szNCScanRet2.size(); j++)
		{
			_NetDeviceInfo* pDev2 = m_szNCScanRet2[j];
			if (pDev->m_pNet->GetSerialID() == pDev2->m_pNet->GetSerialID())
			{
				delete pDev2;
				m_szNCScanRet2.erase(m_szNCScanRet2.begin()+j);
				j--;
			}
		}
	}
	
	GetTstatFromNCTable();

}


// for ethernet nc
void CTStatScanner::GetTstatFromNCTable()
{
	for(UINT n = 0; n < m_szNCScanRet.size(); n++)
	{
		_NetDeviceInfo* pNCInfo = m_szNCScanRet[n];
		CString strPort;
		int nIPPort=pNCInfo->m_pNet->GetIPPort();
		CString strIP=pNCInfo->m_pNet->GetIPAddrStr();
		//##############################
		CString strInfo;strInfo.Format(_T("Scan Tstat connected to %s"), pNCInfo->m_pNet->GetProductName());
		ShowNetScanInfo(strInfo);
		//##############################

		g_CommunicationType=1;
		SetCommunicationType(g_CommunicationType);
		bool b=Open_Socket2(strIP, TCP_COMM_PORT);

		//	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
		//	SetPaneString(1,strInfo);
		if(b)
		{	
			strInfo.Format((_T("Connect to IP : %s successful")), strIP);//prompt info;
			SetPaneString(1,strInfo);
		}
		else
		{
			strInfo.Format((_T("Connect to IP : %s failure")), strIP);//prompt info;
			SetPaneString(1,strInfo);
			continue;
		}

		ReadNCTable(pNCInfo);

	}

	g_ScnnedNum=254;
}


/// for com port nc
void CTStatScanner::GetTstatFromNCTable2()
{
	for(UINT n = 0; n < m_szNCScanRet2.size(); n++)
	{
		close_com();
		_NetDeviceInfo* pNCInfo = m_szNCScanRet2[n];
		CString strPort;
		int nComPort=pNCInfo->m_pNet->GetComPort();
		//##############################
		CString strInfo;strInfo.Format(_T("Scan Tstat connected to %s"), pNCInfo->m_pNet->GetProductName());
		ShowComScanInfo(strInfo);
		//##############################

		g_CommunicationType=0;
		SetCommunicationType(g_CommunicationType);
		bool b=open_com(nComPort);

		//	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
		//	SetPaneString(1,strInfo);
		//CString strCom;
		//strCom.Format(_T("%d"), nComPort);
		if(b)
		{	
			strInfo.Format((_T("Open Com %d successful.")), nComPort);//prompt info;
			SetPaneString(1,strInfo);
		}
		else
		{
			strInfo.Format((_T("Open Com %d failure.")), nComPort);//prompt info;
			SetPaneString(1,strInfo);
			continue;
		}
		
		ReadNCTable(pNCInfo);
	}

	g_ScnnedNum=254;
}


const int TABLE_NODE_NUM_REG = 7000;
const int TABLE_NODE_SIZE = 20;				// 一个node占用20寄存器
void  CTStatScanner::ReadNCTable(_NetDeviceInfo* pNCInfo)
{
	int nAddr = pNCInfo->m_pNet->GetDevID();
	WORD wNumHigh= Read_One(nAddr, TABLE_NODE_NUM_REG);
	if (wNumHigh < 0)
	{
		return;
	}
	int wNum = Read_One(nAddr, TABLE_NODE_NUM_REG+1);
	if (wNum < 0)
	{
		return;
	}
	wNum = (wNumHigh << 8) + wNum;

	unsigned short* szNode = new unsigned short[TABLE_NODE_SIZE];

	for (int i = 0; i < wNum; i++)
	{
		ZeroMemory(szNode, TABLE_NODE_SIZE);
		int nRet = Read_Multi(nAddr, szNode, TABLE_NODE_NUM_REG+2+i*TABLE_NODE_SIZE, TABLE_NODE_SIZE);
		if (nRet >0)
		{
			AddNodeToTable(pNCInfo, szNode);
		}

	}

	delete []szNode;
}


void  CTStatScanner::AddNodeToTable(_NetDeviceInfo* pNCInfo, unsigned short* pNode)
{
	int nSubnet = pNode[0];
	int nAddress = pNode[1];
	long nSerialNo = pNode[2]+pNode[3]*256+pNode[4]*256*256+pNode[5]*256*256*256;

	float fSWVersion =(float) (pNode[6] + pNode[7]*256);
	int nProductType = pNode[9];
	int nHWVersion = pNode[10];

// 	pDev->m_nCoolingSP = pNode[12];
// 	pDev->m_nSP = pNode[13];
// 	pDev->m_nHeatingSP= pNode[14];
// 	pDev->m_nTemprature= pNode[15];
// 	pDev->m_nMode= pNode[16];
// 	pDev->m_nOutPutStatus= pNode[17];
// 	pDev->m_nNightHeatDB= pNode[18];
// 	pDev->m_nNightCoolDB= pNode[19];
// 	pDev->m_nNightHeatSP= pNode[20];	
// 	pDev->m_nNightCoolSP= pNode[21];
// 	pDev->m_nOccupied= pNode[22];
// 
// 
// 	int nStatus = pNode[27];
// 	int nHour = pNode[28];
// 	int nMin = pNode[29];
	int nStatus = pNode[17];
	int nHour = pNode[18];
	int nMin = pNode[19];

	// added for table
// 	int					m_nCoolingSP;
// 	int					m_nSP;
// 	int					m_nHeatingSP;
// 	int					m_nTemprature;
// 	int					m_nMode;
// 	int					m_nOutPutStatus;
// 	int					m_nNightHeatDB;
// 	int					m_nNightCoolDB;
// 	int					m_nOccupied;
	CString strTemp;

	//
	_ComDeviceInfo* pDev = new _ComDeviceInfo;
	pDev->m_nSourceID = nAddress;
	pDev->m_nTempID = nAddress;
	pDev->m_pDev= new CTStat_Dev;

	pDev->m_pDev->m_mainnet_info.m_ProductType = pNCInfo->m_pNet->GetProductType();//PM_NC;
	pDev->m_pDev->m_mainnet_info.m_dwSerialID = pNCInfo->m_pNet->GetSerialID();
	pDev->m_pDev->m_mainnet_info.m_nModbusID = pNCInfo->m_pNet->GetDevID();
	pDev->m_pDev->SetBaudRate(m_nBaudrate);
	pDev->m_pDev->SetSerialID(nSerialNo);
	pDev->m_pDev->SetDevID(nAddress);
	pDev->m_pDev->SetSoftwareVersion(fSWVersion);
	pDev->m_pDev->SetHardwareVersion(float(nHWVersion));
	pDev->m_pDev->SetProductType(nProductType);
	pDev->m_pDev->SetComPort(nSubnet);

	m_szTstatScandRet.push_back(pDev);

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
	if (nDeviceType == PM_LightingController 
		|| nDeviceType == PM_NC
		|| nDeviceType == PM_CO2_NET
		|| nDeviceType == PM_MINIPANEL
		|| nDeviceType == PM_CM5)
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

	if(m_pFile->Open(g_strExePth.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
	{
		for (UINT i = 0; i <10; i++)
		{
			m_pFile->WriteString(_T("dd"));	
		}	

		m_pFile->Close();
	}

}
BOOL CTStatScanner::ScanBacnetComDevice()
{
	m_szComs.clear();
	GetSerialComPortNumber1(m_szComs);

	if (m_szComs.size() <= 0)
	{
		//AfxMessageBox(_T("Can't scan without any com port installed."));
		return FALSE;
	}
	m_pScanBacnetComThread = AfxBeginThread(_ScanBacnetComThread,this);
	return TRUE;
}
UINT _ScanBacnetComThread(LPVOID pParam)
{
	CTStatScanner* pScan = (CTStatScanner*)(pParam);
	m_bac_scan_com_data.clear();	//清空上次扫描的遗留数据;
	m_bac_scan_result_data.clear();

	is_in_scan_mode = true;


		if(!bip_valid())
		{
			bac_net_initial_once = false;
			Initial_bac_com();
			
		}

		for (int i=0;i<3;i++)
		{
			CString strInfo;strInfo.Format(_T("Scan protocol Bacnetip.Send global command"));
			pScan->ShowBacnetComScanInfo(strInfo);
			Send_WhoIs_Global(-1,-1);
			Sleep(1000);
		}

		for (int j=0;j<5;j++)
		{
			int ready_to_read_count =	m_bac_scan_com_data.size();

			CString strInfo;strInfo.Format(_T("Scan  Bacnetip.Found %d device"),ready_to_read_count);
			pScan->ShowBacnetComScanInfo(strInfo);

			if((int)m_bac_scan_result_data.size()>= ready_to_read_count)	//达到返回的个数后就break;
				break;
			TRACE(_T("gloab scan = %d\r\n"),ready_to_read_count);
			for (int i=0;i<ready_to_read_count;i++)
			{
				int	resend_count = 0;
				do 
				{
					resend_count ++;
					if(resend_count>20)
						break;
					g_invoke_id = GetPrivateData(
						m_bac_scan_com_data.at(i).device_id,
						GETSERIALNUMBERINFO,
						0,
						0,
						sizeof(Str_Serial_info));
					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (g_invoke_id<0);
			}
			Sleep(500);
		}

	Sleep(200);
	int ret_3 =	m_bac_scan_result_data.size(); 
	TRACE(_T("serial scan = %d\r\n"),ret_3);
	for (int l=0;l<ret_3;l++)
	{
		CTStat_Net* pTemp = new CTStat_Net;			
		_NetDeviceInfo* pInfo = new _NetDeviceInfo;
		pInfo->m_pNet = pTemp;
		pTemp->SetSerialID(m_bac_scan_result_data.at(l).serialnumber);
		pTemp->SetDevID(m_bac_scan_result_data.at(l).modbus_addr);
		pTemp->SetProductType(m_bac_scan_result_data.at(l).product_type);

		//pTemp->SetProtocol(2);//2就是bacnet;
		pTemp->SetHardwareVersion((float)m_bac_scan_result_data.at(l).device_id);		//用HW Version 代替bacnet的 instance ID
		pTemp->SetSoftwareVersion((float)m_bac_scan_result_data.at(l).panel_number);
		CString temp_ip;
		temp_ip.Format(_T("%u.%u.%u.%u"),(unsigned char)m_bac_scan_result_data.at(l).ipaddress[0],(unsigned char)m_bac_scan_result_data.at(l).ipaddress[1],
										 (unsigned char)m_bac_scan_result_data.at(l).ipaddress[2],(unsigned char)m_bac_scan_result_data.at(l).ipaddress[3]);
		
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, temp_ip.GetBuffer(), -1, cTemp1, 255, NULL, NULL );


		pTemp->SetProtocol(3);//3就是bacnet ip;
		DWORD dwIP = inet_addr(cTemp1); 

		pTemp->SetIPAddr(dwIP);
		pTemp->SetIPPort(47808);
		//pTemp->SetBaudRate(19200);//scan
		//pTemp->SetComPort(5);
		// hardware_version
		//pTemp->SetBuildingName(pScan->m_strBuildingName);
		//pTemp->SetSubnetName(pScan->m_strSubNet);

		pScan->m_szNCScanRet.push_back(pInfo);
	}
	
#if 0
	for (int l=0;l<ret_3;l++)
	{
		CTStat_Dev* pTemp = new CTStat_Dev;			
		_ComDeviceInfo* pInfo = new _ComDeviceInfo;
		pInfo->m_pDev = pTemp;
		pTemp->SetSerialID(m_bac_scan_result_data.at(l).serialnumber);
		pTemp->SetDevID(m_bac_scan_result_data.at(l).modbus_addr);
		pTemp->SetProductType(m_bac_scan_result_data.at(l).product_type);

		pTemp->SetProtocol(2);//2就是bacnet;
		pTemp->SetHardwareVersion(m_bac_scan_result_data.at(l).device_id);		//用HW Version 代替bacnet的 instance ID
		pTemp->SetSoftwareVersion(m_bac_scan_result_data.at(l).panel_number);
		pTemp->SetBaudRate(19200);//scan
		pTemp->SetComPort(5);
		// hardware_version
		pTemp->SetBuildingName(pScan->m_strBuildingName);
		pTemp->SetSubnetName(pScan->m_strSubNet);




		pScan->m_szTstatScandRet.push_back(pInfo);
	}
#endif
endbacnetthread:
	if (pScan->m_eScanBacnetComEnd->m_hObject)
	{
		pScan->m_eScanBacnetComEnd->SetEvent();
	}
	pScan->m_bStopScan = TRUE;
	is_in_scan_mode = false;
	return 1;
}

#if 0
UINT _ScanBacnetComThread(LPVOID pParam)
{
	CTStatScanner* pScan = (CTStatScanner*)(pParam);
	m_bac_scan_com_data.clear();	//清空上次扫描的遗留数据;
	m_bac_scan_result_data.clear();
	while(1)
	{
		if(pScan->m_com_scan_end)
			break;
		Sleep(1);
	}

	if(pScan->m_bacnetComs.size()==0)//根据modbus 扫描的结果判断是否需要进行bacnet 扫描;
	{
		goto	endbacnetthread;
	}
		

	//等Modbus 串口扫描完之后 开始bacnet 的扫描;
	for (int i=0;i<pScan->m_bacnetComs.size();i++)
	{
		int temp_port;
		CString temp_cs;
		temp_cs = pScan->m_bacnetComs.at(i);
		temp_cs = temp_cs.Right(temp_cs.GetLength() - 3);
		temp_port = _wtoi(temp_cs);
		SetCommunicationType(0);//如果没有关闭串口 就先关闭;
		close_com();
		bac_net_initial_once = false;
		Initial_bac_com(temp_port);

		TRACE(_T("Now scan with COM%d\r\n"),temp_port);

		for (int i=0;i<20;i++)
		{
			CString strInfo;strInfo.Format(_T("Scan Com%d,protocol Bacnet MSTP.Send global command time left(%d)"),temp_port,19-i);
			pScan->ShowBacnetComScanInfo(strInfo);
			Send_WhoIs_Global(-1,-1);
			Sleep(1000);
		}

		for (int j=0;j<5;j++)
		{


			int ready_to_read_count =	m_bac_scan_com_data.size();

			CString strInfo;strInfo.Format(_T("Scan Com%d,protocol Bacnet MSTP.Found %d device"),temp_port,ready_to_read_count);
			pScan->ShowBacnetComScanInfo(strInfo);

			if((int)m_bac_scan_result_data.size()>= ready_to_read_count)	//达到返回的个数后就break;
				break;
			TRACE(_T("gloab scan = %d\r\n"),ready_to_read_count);
			for (int i=0;i<ready_to_read_count;i++)
			{
				int	resend_count = 0;
				do 
				{
					resend_count ++;
					if(resend_count>20)
						break;
					g_invoke_id = GetPrivateData(
						m_bac_scan_com_data.at(i).device_id,
						GETSERIALNUMBERINFO,
						0,
						0,
						sizeof(Str_Serial_info));
					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (g_invoke_id<0);
			}
			Sleep(2000);
		}
	}
	Sleep(2000);
	int ret_3 =	m_bac_scan_result_data.size(); 
	TRACE(_T("serial scan = %d\r\n"),ret_3);
	for (int l=0;l<ret_3;l++)
	{
		CTStat_Dev* pTemp = new CTStat_Dev;			
		_ComDeviceInfo* pInfo = new _ComDeviceInfo;
		pInfo->m_pDev = pTemp;
		pTemp->SetSerialID(m_bac_scan_result_data.at(l).serialnumber);
		pTemp->SetDevID(m_bac_scan_result_data.at(l).modbus_addr);
		pTemp->SetProductType(m_bac_scan_result_data.at(l).product_type);

		pTemp->SetProtocol(2);//2就是bacnet;
		pTemp->SetHardwareVersion(m_bac_scan_result_data.at(l).device_id);		//用HW Version 代替bacnet的 instance ID
		pTemp->SetSoftwareVersion(m_bac_scan_result_data.at(l).macaddress);
		pTemp->SetBaudRate(19200);//scan
		pTemp->SetComPort(5);
		// hardware_version
		pTemp->SetBuildingName(pScan->m_strBuildingName);
		pTemp->SetSubnetName(pScan->m_strSubNet);



		
		pScan->m_szTstatScandRet.push_back(pInfo);
	}
endbacnetthread:
	if (pScan->m_eScanBacnetComEnd->m_hObject)
	{
		pScan->m_eScanBacnetComEnd->SetEvent();
	}
	pScan->m_bStopScan = TRUE;
	return 1;
}
#endif
