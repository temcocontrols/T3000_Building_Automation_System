#include "StdAfx.h"
#include "TStatScanner.h"
#include "globle_function.h"
#include "ScanDlg.h"

#include "ScanDbWaitDlg.h"
#include "hangeIDDlg.h"
#include "MainFrm.h"
#include "define.h"
#include "bip.h"
#include "rs485.h" // For call Get_RS485_Handle() function
#include "WhichOneToChooseDlg.h"
//#include "global_variable.h"
//#include "gloab_define.h"
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
int scan_item = -1;
int scan_udp_item = -1;
int scan_bacnet_ip_item = -1;
int scan_remote_ip_item = -1;
int scan_baudrate =0;
vector <refresh_net_device> m_tstat_net_device_data;
int reply_count = 0;

const int TCP_COMM_PORT = 6001;
extern int g_ScnnedNum;
extern bool b_pause_refresh_tree ;

extern char local_network_ip[255];
extern CString local_enthernet_ip;
typedef struct infopack
{
    unsigned char msg;//RESPONSE_MSG
    unsigned char lenth;
    unsigned short* pNCInfo;
} NC_Info;

struct _NCInfo
{
    UINT nSerial;
    int nProductID;
    int modbusID;
    CString strIP;
    unsigned short nPort;
};

UINT _ScanRemote_IP_Thread(LPVOID pParam);
UINT _ScanBacnetMSTPThread(LPVOID pParam);
UINT _ScanNCByUDPFunc(LPVOID pParam);
//UINT _ScanTstatThread(LPVOID pParam);
UINT _ScanTstatThread2(LPVOID pParam);
//UINT _WaitScanThread(LPVOID pParam);
DWORD WINAPI  _WaitScanThread(LPVOID lpVoid);

UINT _ScanOldNC(LPVOID pParam);
void Show_Scan_Data(LPCTSTR nstrInfo ,unsigned int nitem);
CTStatScanner::CTStatScanner(void)
    :m_bComScanRunning(TRUE)
{
    m_bStopScan = FALSE;
    m_pScanNCThread = NULL;
    m_pScanTstatThread = NULL;
    //m_pScanTstatOneByOneThread=NULL;
    m_pScanBacnetIPThread = NULL;
//	m_pScanBacnetMstpThread = NULL;
    m_eScanComEnd = new CEvent(false, false);
    //m_eScanComOneByOneEnd=new CEvent(false,false);
    m_eScanOldNCEnd = new CEvent(false, false);
    m_eScanNCEnd = new CEvent(false, false);
    m_eScanBacnetIpEnd = new CEvent(false, false);
    m_eScanRemoteIPEnd =  new CEvent(false, false);
//	m_eScanBacnetMstpEnd = new CEvent(false,false);
    m_bNetScanFinish = FALSE;
    m_bCheckSubnetFinish=TRUE;
    ResetRepeatedID();

    m_strBuildingName = _T("");
    m_strFloorName = _T("");
    m_strRoomName = _T("");

 
	m_SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

    m_nComPort = 1;
    m_scantype = 0;
    m_thesamesubnet=TRUE;
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

    if(m_eScanBacnetIpEnd !=NULL)
    {
        delete m_eScanBacnetIpEnd;
    }

    if(m_eScanRemoteIPEnd !=NULL)
    {
        delete m_eScanRemoteIPEnd;
    }


    //if(m_eScanBacnetMstpEnd != NULL)
    //{
    //	delete m_eScanBacnetMstpEnd;
    //	m_eScanBacnetMstpEnd = NULL;
    //}
// 	if (m_eScanComOneByOneEnd!=NULL)
// 	{
// delete	m_eScanComOneByOneEnd;
// 	}

   m_SqliteDBBuilding.closedb();
    //t_pCon->Close();
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
    if( WaitForSingleObject(m_eScanBacnetIpEnd, 0) != WAIT_OBJECT_0 )
    {
        SetEvent(m_eScanBacnetIpEnd);
        TerminateThread(m_pScanBacnetIPThread,0);
    }
    if( WaitForSingleObject(m_eScanRemoteIPEnd, 0) != WAIT_OBJECT_0 )
    {
        SetEvent(m_eScanRemoteIPEnd);
        TerminateThread(m_pScanRemoteIPThread,0);
    }

    //if( WaitForSingleObject(m_eScanBacnetMstpEnd, 0) != WAIT_OBJECT_0 )
    //{
    //	SetEvent(m_eScanBacnetMstpEnd);
    //	TerminateThread(m_pScanBacnetMstpThread,0);
    //}
// 	if (WaitForSingleObject(m_eScanComOneByOneEnd,0)!=WAIT_OBJECT_0)
// 	{
// 	SetEvent(m_eScanComOneByOneEnd);
// 	TerminateThread(m_pScanTstatOneByOneThread,0);
// 	}
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

// 	if (m_szComs.size() <= 0)
// 	{
//
// 		return FALSE;
// 	}


    SetCommunicationType(0);   //设置为串口通信方式

    m_pScanTstatThread = AfxBeginThread(_ScanTstatThread2,this);
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



        return;
    }
    if (nComPort!=-1)
    {


    }
    else
    {


    }
    g_strScanInfoPrompt.Format(_T("COM%d"), nComPort);

    for (int ID=devLo; ID<=devHi; ID++)
    {
        g_nStartID=g_nEndID=ID;


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
        char c_array_temp[5]= {'0'};
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



            }
            else
                return;
        }
    }

    m_bStopScan=TRUE;
}


void CTStatScanner::modbusip_to_modbus485(int nComPort,int nBaudrate, BYTE devLo, BYTE devHi)
{
	 bool bForTStat = true;
	     if (m_bStopScan)
    {
        return;
    }

    g_nStartID = devLo;
    g_nEndID = devHi;

    Sleep(50);
    int nCount=0;

    CString strlog;
    if (nComPort  ==  0)
    {
        strlog.Format(_T("Scanning IP %s 'RS485 Sub' Baudrate: %d .Scan ID From %d to %d"),m_device_ip_address,nBaudrate,devLo,devHi);
	}
	else if(nComPort  ==  1)
	{
		strlog.Format(_T("Scanning IP %s ZIGB .Scan ID From %d to %d"),m_device_ip_address,devLo,devHi);
	}
	else if(nComPort  ==  2)
	{
		strlog.Format(_T("Scanning IP %s 'RS485 Main' Baudrate: %d . Scan ID From %d to %d"),m_device_ip_address,nBaudrate,devLo,devHi);
	}
	char temp_char[250];
	WideCharToMultiByte( CP_ACP, 0, strlog.GetBuffer(), -1, m_scan_info.at(0).scan_notes, 250, NULL, NULL );
	//memcpy(m_scan_info.at(0).scan_notes,temp_char,250);


    int ret_return=g_CheckTstatOnline_a(devLo,devHi, bForTStat);
    if(ret_return ==-6)
    {
        //CString temp_cs;
        //temp_cs.Format(_T("Com%d"),nComPort);
        //m_bacnetComs.push_back(temp_cs);
        return ;
    }
    if (ret_return == -3 || ret_return > 0)
    {
        strlog.Format(_T("The data is coming,but it is not clear!"));
        g_llTxCount++;
        g_llRxCount++;


        ret_return=g_CheckTstatOnline_a(devLo,devHi, bForTStat);
        strlog.Format(_T("Sending scan broadcast command by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);

        g_llTxCount++; 
        g_llRxCount++;


    }

    //TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
    if(binary_search_crc(ret_return))
    {
        strlog.Format(_T("NO Response com%d to ID From %d to %d"),nComPort,devLo,devHi);
        return ;
    }
    char c_array_temp[5]= {'0'};
    CString temp=_T("");
    if(ret_return>0)
    {


        int ntempID=0;
        BOOL bFindSameID=false;
        int nPos=-1;
//		temp.baudrate=m_baudrate2;

        unsigned short SerialNum[10];
        memset(SerialNum,0,sizeof(SerialNum));
        int nRet=0;
        nRet=read_multi2(ret_return,&SerialNum[0],0,10,bForTStat);
        SHOW_TX_RX
        if(nRet>0)
        {
            CTStat_Dev* pTemp = new CTStat_Dev;
            _ComDeviceInfo* pInfo = new _ComDeviceInfo;
            pInfo->m_pDev = pTemp;
            if(IsRepeatedID(ret_return))
            {
                TRACE(_T("Scan one with Repeated ID = %d\n"), ret_return);
                pInfo->m_bConflict = TRUE;
                pInfo->m_nSourceID = m_szRepeatedID[ret_return];
                pInfo->m_nTempID = ret_return;
            }
            else
            {
                pInfo->m_bConflict = FALSE;
                pInfo->m_nSourceID = ret_return;
                pInfo->m_nTempID = ret_return;
            }

            pInfo->m_tstatip = m_ip;//scan
            pInfo->m_tstatport = m_port;//scan

            //m_szTstatScandRet.push_back(pInfo);
			refresh_net_device temp_data;
			

            unsigned int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;//20120424

            pTemp->SetSerialID(nSerialNumber);

            pTemp->SetDevID(ret_return);

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


 			if (nComPort==-1)
 			{
 				//pTemp->SetBaudRate(dwIP);//scan
 				pTemp->SetComPort(_wtoi(m_port));
 			}
 			else
            {
                pTemp->SetBaudRate(m_nBaudrate);//scan
                pTemp->SetComPort(nComPort);
            }


#if 0
            if(Read_One2(ret_return, 185, bForTStat)==0)
                //if(pTemp->ReadOneReg(185)==0)
            {
                SHOW_TX_RX
                //temp.baudrate=9600;
                pTemp->SetBaudRate(9600);//scan

            }
            else
            {
                //temp.baudrate=19200;
                pTemp->SetBaudRate(19200);//scan
            }
#endif
            //temp.nEPsize=Read_One2(temp.id,326, bForTStat);
            pTemp->SetEPSize(pTemp->ReadOneReg(326));

            //if(pTemp->GetComPort()>=0)

            // product type
            //pTemp->ReadOneReg(8);
            //
//              if (SerialNum[7] == PM_CO2_RS485 && )
//              {
//              }
            pTemp->SetProductType(SerialNum[7]);

            // hardware_version
            pTemp->SetHardwareVersion(SerialNum[8]);

            pTemp->SetBuildingName(m_strBuildingName);
            pTemp->SetSubnetName(m_strSubNet);

			unsigned short device_name[10];
			memset(device_name,0 ,10);

			 nRet=read_multi2(ret_return,&device_name[0],714,10,bForTStat);
			 if(device_name[0] != 0x56)
			 {
				 temp_data.show_label_name = GetProductName(SerialNum[7]);
			 }
			 else
			 {
				  unsigned short Hi_Char,Low_Char;

				 for (int i=0; i<8; i++)
				 {
					 Hi_Char=device_name[1+i];
					 Hi_Char=Hi_Char&0xff00;
					 Hi_Char=Hi_Char>>8;
					 Low_Char=device_name[i+1];
					 Low_Char=Low_Char&0x00ff;


					 device_name[i+1] = Low_Char*256 + Hi_Char;
				 }

				 //for (int i=0; i<17; i++)
				 //{
					// p[i] =(unsigned char)temp_buffer_Char[i];
				 //}
				 //str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);


				 CString temp_cs;
				 MultiByteToWideChar( CP_ACP, 0, (char *)&device_name[1], (int)strlen((char *)&device_name[1])+1, 
					 temp_cs.GetBuffer(MAX_PATH), MAX_PATH );
				temp_cs.ReleaseBuffer();
				temp_data.show_label_name = temp_cs;
				 if(temp_data.show_label_name.GetLength() > 16)
					 temp_data.show_label_name = temp_data.show_label_name.Left(16);

				 CString temp123 ;
				 temp123 = temp_data.show_label_name.Trim();
				 if(temp123.IsEmpty())
				 {
					  temp_data.show_label_name = GetProductName(SerialNum[7]);
				 }
			 }

			temp_data.ip_address = m_device_ip_address;
			temp_data.hw_version = SerialNum[8];
			temp_data.modbusID = ret_return;
			temp_data.NetCard_Address = _T("");
			temp_data.nport = m_device_ip_port;
			temp_data.nSerial = nSerialNumber;
			temp_data.object_instance = 0;
			temp_data.panal_number = ret_return;
			temp_data.product_id = SerialNum[7];
			temp_data.sw_version = tstat_version2;
			temp_data.parent_serial_number = m_parent_serialnum;

			m_tstat_net_device_data.push_back(temp_data);

            m_scan_info.at(scan_item).scan_found ++;
        }
        else
            return;
    }

    switch(ret_return)
    {

    case -2:
        //crc error
        strlog.Format(_T("CRC ERROR by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
        if(devLo!=devHi)
        {
            modbusip_to_modbus485(nComPort,nBaudrate, devLo,(devLo+devHi)/2);
            modbusip_to_modbus485(nComPort,nBaudrate, (devLo+devHi)/2+1,devHi);
        }
        else
            modbusip_to_modbus485(nComPort,nBaudrate, devLo,devHi);
        break;
    case -3:
        //more than 2 Tstat is connect
        strlog.Format(_T("More than two tstats by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
        if(devLo!=devHi)
        {
            modbusip_to_modbus485(nComPort,nBaudrate, devLo,(devLo+devHi)/2);
            modbusip_to_modbus485(nComPort,nBaudrate, (devLo+devHi)/2+1,devHi);
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
                Sleep(20);
                //////////////////////////////////for running is better
                char c_temp_arr[100]= {'\0'};
                if(Read_One2(devLo,10, bForTStat)!=-2)//one times
                {
                    Sleep(100);
                    SHOW_TX_RX
                    CString str_temp;
                    for(int j=254; j>=1; j--)
                        if(j!=devLo)
                        {
                            //	if(!found_same_net_work_controller_by_mac(a))
                            if(1)
                            {
                                bool find=false;//false==no find;true==find
                                for(UINT w=0; w<m_szTstatScandRet.size(); w++)
                                    if(j==(m_szTstatScandRet.at(w))->m_pDev->GetDevID())
                                    {
                                        find=true;
                                        break;
                                    }
                                if(find==false)
                                {
                                    //if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
                                    if(Write_One2(devLo,10,j, bForTStat)>0)//sometimes write failure ,so inspect,important
                                    {
                                        SHOW_TX_RX
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
                                            {
                                                SHOW_TX_RX
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
                                              //  m_szTstatScandRet.push_back(pInfo);
                                                //}
                                            }
                                        }

                                       modbusip_to_modbus485(nComPort,nBaudrate, devLo,devHi);
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
            }
            while(1);
        }
        break;
    case -4:
        break;
        strlog.Format(_T("No Response by com%d to ID From %d to %d"),nComPort,devLo,devHi);
    //write_T3000_log_file(strlog);
    //no connection
    case -5:
        strlog.Format(_T("No Response by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
        break;

        //the input error
    }



}

void CTStatScanner::binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo, BYTE devHi)
{

    if (m_bStopScan)
    {


        return;
    }

    g_strScanInfoPrompt.Format(_T("COM%d"), nComPort);


    //write_T3000_log_file(g_strScanInfoPrompt);

    g_nStartID = devLo;
    g_nEndID = devHi;

    Sleep(50);
    int nCount=0;

    CString strlog;
    if (nComPort==-1)
    {
        strlog.Format(_T("Sending scan broadcast command by TCP to ID From %d to %d"),devLo,devHi);
        //write_T3000_log_file(strlog);
    }
    else
    {
        strlog.Format(_T("Sending scan broadcast command by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
        CString temp_str;
        temp_str.Format(_T("Sending scan broadcast command From %d to %d"),devLo,devHi);
        char temp_char[250];
        WideCharToMultiByte( CP_ACP, 0, temp_str.GetBuffer(), -1, temp_char, 250, NULL, NULL );
        memcpy(m_scan_info.at(scan_item).scan_notes,temp_char,250);
    }


    int a=g_CheckTstatOnline_a(devLo,devHi, bForTStat);
    if(a ==-6)//总线上存在bacnet协议，modbus协议无法扫描;
    {
        CString temp_cs;
        temp_cs.Format(_T("Com%d"),nComPort);
        m_bacnetComs.push_back(temp_cs);
        return ;
    }
    if (a == -3 || a > 0)
    {
        strlog.Format(_T("The data is coming,but it is not clear!"));
        g_llTxCount++;
        g_llRxCount++;
        if( AfxGetMainWnd()->GetActiveWindow() != NULL )
        {

            // construct status message string
            CString str;
            str.Format(_T("Addr:255 [Tx=%d Rx=%d Err=%d]"),
                       g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

            //Display it
            ((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());
        }

        a=g_CheckTstatOnline_a(devLo,devHi, bForTStat);
        strlog.Format(_T("Sending scan broadcast command by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);

        g_llTxCount++;
        g_llRxCount++;
        if( AfxGetMainWnd()->GetActiveWindow() != NULL )
        {

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
        strlog.Format(_T("NO Response com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
        return ;
    }
    char c_array_temp[5]= {'0'};
    CString temp=_T("");
    if(a>0)
    {


        int ntempID=0;
        BOOL bFindSameID=false;
        int nPos=-1;
//		temp.baudrate=m_baudrate2;
//      兼容小叶的Pressure
        unsigned short SerialNum[10];
        memset(SerialNum,0,sizeof(SerialNum));
        int nRet=0;
        nRet=read_multi2(a,&SerialNum[0],0,10,bForTStat);
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

            unsigned int nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;//20120424

            pTemp->SetSerialID(nSerialNumber);

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


 			if (nComPort==-1)
 			{
 				//pTemp->SetBaudRate(dwIP);//scan
 				pTemp->SetComPort(_wtoi(m_port));
 			}
 			else
            {
                pTemp->SetBaudRate(m_nBaudrate);//scan
                pTemp->SetComPort(nComPort);
            }


#if 0
            if(Read_One2(a, 185, bForTStat)==0)
                //if(pTemp->ReadOneReg(185)==0)
            {
                SHOW_TX_RX
                //temp.baudrate=9600;
                pTemp->SetBaudRate(9600);//scan

            }
            else
            {
                //temp.baudrate=19200;
                pTemp->SetBaudRate(19200);//scan
            }
#endif
            //temp.nEPsize=Read_One2(temp.id,326, bForTStat);
            pTemp->SetEPSize(pTemp->ReadOneReg(326));

            //if(pTemp->GetComPort()>=0)

            // product type
            //pTemp->ReadOneReg(8);
            //
//              if (SerialNum[7] == PM_CO2_RS485 && )
//              {
//              }
            pTemp->SetProductType(SerialNum[7]);

            // hardware_version
            pTemp->SetHardwareVersion(SerialNum[8]);

            pTemp->SetBuildingName(m_strBuildingName);
            pTemp->SetSubnetName(m_strSubNet);


            m_scan_info.at(scan_item).scan_found ++;
        }
        else
            return;
    }

    switch(a)
    {

    case -2:
        //crc error
        strlog.Format(_T("CRC ERROR by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
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
        strlog.Format(_T("More than two tstats by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
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
                Sleep(20);
                //////////////////////////////////for running is better
                char c_temp_arr[100]= {'\0'};
                if(Read_One2(devLo,10, bForTStat)!=-2)//one times
                {
                    Sleep(100);
                    SHOW_TX_RX
                    CString str_temp;
                    for(int j=254; j>=1; j--)
                        if(j!=devLo)
                        {
                            //	if(!found_same_net_work_controller_by_mac(a))
                            if(1)
                            {
                                bool find=false;//false==no find;true==find
                                for(UINT w=0; w<m_szTstatScandRet.size(); w++)
                                    if(j==(m_szTstatScandRet.at(w))->m_pDev->GetDevID())
                                    {
                                        find=true;
                                        break;
                                    }
                                if(find==false)
                                {
                                    //if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
                                    if(Write_One2(devLo,10,j, bForTStat)>0)//sometimes write failure ,so inspect,important
                                    {
                                        SHOW_TX_RX
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
                                            {
                                                SHOW_TX_RX
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
            }
            while(1);
        }
        break;
    case -4:
        break;
        strlog.Format(_T("No Response by com%d to ID From %d to %d"),nComPort,devLo,devHi);
    //write_T3000_log_file(strlog);
    //no connection
    case -5:
        strlog.Format(_T("No Response by com%d to ID From %d to %d"),nComPort,devLo,devHi);
        //write_T3000_log_file(strlog);
        break;

        //the input error
    }



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
        if (m_eScanNCEnd)
        {
            m_eScanNCEnd->SetEvent();
        }
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
        if (m_eScanNCEnd)
        {
            m_eScanNCEnd->SetEvent();
        }
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
        if (m_eScanNCEnd)
        {
            m_eScanNCEnd->SetEvent();
        }
        return 0;
    }
    if(::listen(sListen, 5) == SOCKET_ERROR)
    {
        printf("Failed listen() \n");
        if (m_eScanNCEnd)
        {
            m_eScanNCEnd->SetEvent();
        }
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
BOOL CTStatScanner::CheckTheSameSubnet(CString strIP)
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
        return FALSE;

    hostent* pHostent = gethostbyname(hostname);
    if(pHostent==NULL)
        return FALSE;

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
    {
        // 是同一子网，但是连接不上，那么提示检查设备连接
        // 		CString strTip;
        // 		strTip.Format(_T("Can not set up the connection with %s, please check its IP address and net cable. "), strIP);
        // 		AfxMessageBox(strTip);
        return TRUE;
    }
    else
    {
        // 		CString strTip;
        // 		strTip.Format(_T("Your host IP is %s, and NC' IP is %s. They are not in same sub net, please reset your IP address. "),strHostIP, strIP);
        // 		AfxMessageBox(strTip);
        return FALSE;
    }
}

UINT _ScanNCByUDPFunc(LPVOID pParam)
{
    //g_nStartID = -1; // this is a flag for udp scan.
    GetIPMaskGetWayForScan();
    CTStatScanner* pScanner = (CTStatScanner*)pParam;
	                   reply_count = 0;
    if(m_scan_info.at(scan_udp_item).scan_skip == false)
    {
        for (int index=0; index<g_Scan_Vector_Subnet.size(); index++)
        {
            if (g_Scan_Vector_Subnet[index].StrIP.Find(_T("0.0."))!=-1)
            {
                continue;
            }
            SOCKET sListen=NULL;
            #pragma region new_socket
            SOCKET h_scan_Broad=NULL;
            SOCKADDR_IN h_scan_bcast;
            SOCKADDR_IN h_scan_siBind;
            h_scan_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            BOOL bBroadcast=TRUE;
            ::setsockopt(h_scan_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
            int iMode=1;
            ioctlsocket(h_scan_Broad,FIONBIO, (u_long FAR*) &iMode);

            BOOL bDontLinger = FALSE;
            setsockopt( h_scan_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );


            //SOCKADDR_IN bcast;
            h_scan_bcast.sin_family=AF_INET;
            //bcast.sin_addr.s_addr=nBroadCastIP;
            h_scan_bcast.sin_addr.s_addr=INADDR_BROADCAST;
            h_scan_bcast.sin_port=htons(UDP_BROADCAST_PORT);


            h_scan_siBind.sin_family=AF_INET;
            h_scan_siBind.sin_addr.s_addr =  INADDR_ANY;



            #pragma endregion new_socket


            local_enthernet_ip=g_Scan_Vector_Subnet[index].StrIP;
            WideCharToMultiByte( CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL );
            h_siBind.sin_family=AF_INET;
            h_siBind.sin_addr.s_addr =  inet_addr(local_network_ip);
            //	h_siBind.sin_port=
            h_siBind.sin_port= htons(57629);
            if( -1 == bind(h_scan_Broad,(SOCKADDR*)&h_siBind,sizeof(h_siBind)))//把网卡地址强行绑定到Socket
            {
                continue;
            }

            m_scan_info.at(scan_udp_item).scan_status = SCAN_STATUS_RUNNING;
            memset(m_scan_info.at(scan_udp_item).scan_notes,0,250);
            CString temp_str;
            temp_str.Format(_T("Scan Network device.."));
            char temp_char[250];
            WideCharToMultiByte( CP_ACP, 0, temp_str.GetBuffer(), -1, temp_char, 250, NULL, NULL );
            memcpy(m_scan_info.at(scan_udp_item).scan_notes,temp_char,250);

            CString strScanInfo = _T("Start UDP scan.");
            pScanner->ShowNetScanInfo(strScanInfo);


            g_strScanInfoPrompt = _T("NC by UDP");
            TRACE(_T("Start udp scan ! \n"));


            int nRet = 0;
            //############################



            short nmsgType=UPD_BROADCAST_QRY_MSG;


            //////////////////////////////////////////////////////////////////////////
            const DWORD END_FLAG = 0x00000000;
            TIMEVAL time;
            time.tv_sec =3;
            time.tv_usec = 1000;

            fd_set fdSocket;
            FD_ZERO(&fdSocket);
            FD_SET(h_scan_Broad, &fdSocket);

            BYTE buffer[512] = {0};

            BYTE pSendBuf[1024];
            ZeroMemory(pSendBuf, 255);
            pSendBuf[0] = 100;
            //pSendBuf[1] = END_FLAG;
            memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
            int nSendLen = 5;

			  BYTE ptempSendBuf[100];
			  memset(ptempSendBuf,0,100);
			  ptempSendBuf[0] = 0xff;
			  ptempSendBuf[1] = 0x55;
			  ptempSendBuf[2] = 0xff;
			  ptempSendBuf[3] = 0x55;
			  int ntempSendLen = 4;
			 nRet = ::sendto(h_scan_Broad,(char*)ptempSendBuf,ntempSendLen,0,(sockaddr*)&h_scan_bcast,sizeof(h_scan_bcast));

            /////////////////////////////////////////////////////////////////////////*/
            int time_out=0;
            BOOL bTimeOut = FALSE;
            while(!bTimeOut)//!pScanner->m_bNetScanFinish)  // 超时结束
            {
                time_out++;
                if(time_out>5)
                    bTimeOut = TRUE;
                if(pScanner->m_bStopScan)
                {

                    break;
                }

                //############################

                //############################
                nRet = ::sendto(h_scan_Broad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_scan_bcast,sizeof(h_scan_bcast));
                if (nRet == SOCKET_ERROR)
                {
                    int  nError = WSAGetLastError();

                    goto END_SCAN;
                    return 0;
                }
				g_llTxCount ++ ;
                memset(m_scan_info.at(scan_udp_item).scan_notes,0,250);
                temp_str.Format(_T("Send UDP broadcast package to device.."));
                char temp_char[250];
                WideCharToMultiByte( CP_ACP, 0, temp_str.GetBuffer(), -1, temp_char, 250, NULL, NULL );
                memcpy(m_scan_info.at(scan_udp_item).scan_notes,temp_char,250);

                int nLen = sizeof(h_scan_siBind);
                //while(pScanner->IsComScanRunning())

                fd_set fdRead = fdSocket;
                int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
                if (nSelRet == SOCKET_ERROR)
                {
                    int nError = WSAGetLastError();

                    goto END_SCAN;
                    return 0;
                }

                if(nSelRet > 0)
                {
                    ZeroMemory(buffer, 512);
 
                    do
                    {
                        int nRet = ::recvfrom(h_scan_Broad,(char*)buffer, 512, 0,(sockaddr*)&h_scan_siBind, &nLen);
                        //int nRet = ::recvfrom(h_scan_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);
                        //			int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
                        BYTE szIPAddr[4] = {0};
                        if(nRet > 0)
                        {
                            FD_ZERO(&fdSocket);
                            if(buffer[0]==RESPONSE_MSG)
                            {
								g_llRxCount ++ ;
                                nLen=buffer[2]+buffer[3]*256;
                                unsigned short dataPackage[32]= {0};
                                memcpy(dataPackage,buffer+2,nLen*sizeof(unsigned short));
                                //int modbusID=dataPackage[6];
                                szIPAddr[0]= (BYTE)dataPackage[7];
                                szIPAddr[1]= (BYTE)dataPackage[8];
                                szIPAddr[2]= (BYTE)dataPackage[9];
                                szIPAddr[3]= (BYTE)dataPackage[10];
                                CString StrIp;
                                StrIp.Format(_T("%d.%d.%d.%d"),szIPAddr[0],szIPAddr[1],szIPAddr[2],szIPAddr[3]);
                                if (StrIp.GetLength()<=16)
                                {
                                    // 							if (pScanner->m_thesamesubnet)
                                    // 							{
                                    // 								pScanner->m_thesamesubnet=pScanner->CheckTheSameSubnet(StrIp);
                                    // 								pScanner->m_bCheckSubnetFinish=pScanner->m_thesamesubnet;
                                    // 							}
                                }

                                memset(m_scan_info.at(scan_udp_item).scan_notes,0,250);
                                temp_str.Format(_T("Receive reply :%u"),reply_count);
                                char temp_char[250];
                                WideCharToMultiByte( CP_ACP, 0, temp_str.GetBuffer(), -1, temp_char, 250, NULL, NULL );
                                memcpy(m_scan_info.at(scan_udp_item).scan_notes,temp_char,250);


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
                                bFlag = FALSE;
                                if (!bFlag)	//不判断 Ip是否重复，因为 Minipanel能挂载TSTAT的设备 会将底下的设备也回复过来;
                                {
                                    
                                    pScanner->AddNCToList(buffer, nRet, h_scan_siBind);
									m_scan_info.at(scan_udp_item).scan_found  = reply_count;

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

                                //}
                            }
                            else
                            {
                                break;
                            }
                            SHOW_TX_RX

                            FD_ZERO(&fdSocket);
                            FD_SET(h_scan_Broad, &fdSocket);
                            nLen = sizeof(h_scan_siBind);
                            fdRead = fdSocket;
                            nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
                        }
                    }
                    while (nSelRet);
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

            //}//end of while
            //closesocket(sListen);

END_SCAN:

            closesocket(h_scan_Broad);
            m_scan_info.at(scan_udp_item).scan_status = SCAN_STATUS_FINISHED;
            memset(m_scan_info.at(scan_udp_item).scan_notes,0,250);
            temp_str.Format(_T("Network scan finished."));
            memset(temp_char,0,250);
            WideCharToMultiByte( CP_ACP, 0, temp_str.GetBuffer(), -1, temp_char, 250, NULL, NULL );
            memcpy(m_scan_info.at(scan_udp_item).scan_notes,temp_char,250);

        }
        //SOCKET hBroad=NULL;

    }












    pScanner->m_eScanNCEnd->SetEvent();

    TRACE(_T("UDP End Scan! \n"));

    //############################
    CString strScanInfo = _T("UDP scan finished!");

    pScanner->ShowNetScanInfo(strScanInfo);

    //############################

    return 1;
}




int CTStatScanner::AddNCToList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind)
{

	refresh_net_device temp;
	Str_UPD_SCAN temp_data;
	memset(&temp_data,0,400);
	unsigned char * my_temp_point = buffer;
	temp_data.reg.command = *(my_temp_point++);
	temp_data.reg.command_reserve = *(my_temp_point++);

	temp_data.reg.length = *(my_temp_point++);
	temp_data.reg.length_reserve = *(my_temp_point++);


	temp_data.reg.serial_low = *(my_temp_point++);
	temp_data.reg.serial_low_reserve = *(my_temp_point++);

	temp_data.reg.serial_low_2 = *(my_temp_point++);
	temp_data.reg.serial_low_2_reserve = *(my_temp_point++);

	temp_data.reg.serial_low_3 = *(my_temp_point++);
	temp_data.reg.serial_low_3_reserve = *(my_temp_point++);

	temp_data.reg.serial_low_4 = *(my_temp_point++);
	temp_data.reg.serial_low_4_reserve = *(my_temp_point++);

	temp_data.reg.product_id =  *(my_temp_point++);
	temp_data.reg.product_id_reserve =  *(my_temp_point++);


	temp_data.reg.modbus_id =  *(my_temp_point++);
	temp_data.reg.modbus_id_reserve =  *(my_temp_point++);

	temp_data.reg.ip_address_1 =  *(my_temp_point++);
	temp_data.reg.ip_address_1_reserve =  *(my_temp_point++);
	temp_data.reg.ip_address_2 =  *(my_temp_point++);
	temp_data.reg.ip_address_2_reserve =  *(my_temp_point++);
	temp_data.reg.ip_address_3 =  *(my_temp_point++);
	temp_data.reg.ip_address_3_reserve =  *(my_temp_point++);
	temp_data.reg.ip_address_4 =  *(my_temp_point++);
	temp_data.reg.ip_address_4_reserve =  *(my_temp_point++);

	temp_data.reg.modbus_port =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;
	temp_data.reg.sw_version =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;
	temp_data.reg.hw_version =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;

	temp_data.reg.parent_serial_number =  ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 4;

	temp_data.reg.object_instance_2 = *(my_temp_point++);
	temp_data.reg.object_instance_1 = *(my_temp_point++);
	temp_data.reg.station_number = *(my_temp_point++);
	memcpy(temp_data.reg.panel_name,my_temp_point,20);
	my_temp_point = my_temp_point + 20;
	temp_data.reg.object_instance_4 = *(my_temp_point++);
	temp_data.reg.object_instance_3 = *(my_temp_point++);
	temp_data.reg.isp_mode = *(my_temp_point++);	//isp_mode = 0 表示在应用代码 ，非0 表示在bootload.
	if(temp_data.reg.isp_mode != 0)
	{
		//记录这个的信息,如果短时间多次出现 就判定在bootload下面，只是偶尔出现一次表示只是恰好开机收到的.
		return	 0;
	}
	DWORD nSerial=temp_data.reg.serial_low + temp_data.reg.serial_low_2 *256+temp_data.reg.serial_low_3*256*256+temp_data.reg.serial_low_4*256*256*256;
	CString nip_address;
	nip_address.Format(_T("%u.%u.%u.%u"),temp_data.reg.ip_address_1,temp_data.reg.ip_address_2,temp_data.reg.ip_address_3,temp_data.reg.ip_address_4);
	CString nproduct_name = GetProductName(temp_data.reg.product_id);
	if(nproduct_name.IsEmpty())	//如果产品号 没定义过，不认识这个产品 就exit;
	{
		if (temp_data.reg.product_id<220)
		{
			return 0;
		}
	}

	temp.nport = temp_data.reg.modbus_port;
	temp.sw_version = temp_data.reg.sw_version;
	temp.hw_version = temp_data.reg.hw_version;
	temp.ip_address = nip_address;
	temp.product_id = temp_data.reg.product_id;
	temp.modbusID = temp_data.reg.modbus_id;
	temp.nSerial = nSerial;
	temp.NetCard_Address=local_enthernet_ip;

	temp.parent_serial_number = temp_data.reg.parent_serial_number ;

	temp.object_instance = temp_data.reg.object_instance_1 + temp_data.reg.object_instance_2 *256+temp_data.reg.object_instance_3*256*256+temp_data.reg.object_instance_4*256*256*256;
	temp.panal_number = temp_data.reg.station_number;




	if((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
	{
		g_Print.Format(_T("Serial = %u     ID = %d ,ip = %s  , Product name : %s ,obj = %u ,panel = %u"),nSerial,temp_data.reg.modbus_id,nip_address ,nproduct_name,temp.object_instance,temp.panal_number);
		DFTrace(g_Print);
	}




	bool find_exsit = false;

	for (int i=0; i<(int)m_refresh_net_device_data.size(); i++)
	{
		if(m_refresh_net_device_data.at(i).nSerial == nSerial)
		{
			find_exsit = true;
			break;
		}
	}

	if(!find_exsit)
	{
		reply_count ++;
		m_refresh_net_device_data.push_back(temp);
	}


	char * temp_point = NULL;
	refresh_net_label_info temp_label;
	temp_point = temp_data.reg.panel_name;
	if(( (unsigned char)temp_point[0] != 0xff) && ((unsigned char)temp_point[1] != 0xff) && ((unsigned char)temp_point[0] != 0x00))
	{
		memcpy(temp_label.label_name,&temp_point[0],20);
		temp_point = temp_point + 20;
		CString cs_temp_label;
		MultiByteToWideChar( CP_ACP, 0, (char *)temp_label.label_name, (int)strlen((char *)temp_label.label_name)+1,
			cs_temp_label.GetBuffer(MAX_PATH), MAX_PATH );
		cs_temp_label.ReleaseBuffer();
		if(cs_temp_label.GetLength() > 20)
			cs_temp_label = cs_temp_label.Left(20);
		temp_label.serial_number = (unsigned int)nSerial;
		CString temp_serial_number;
		temp_serial_number.Format(_T("%u"),temp_label.serial_number);
		int need_to_write_into_device = GetPrivateProfileInt(temp_serial_number,_T("WriteFlag"),0,g_achive_device_name_path);
		if(need_to_write_into_device == 0)
		{
			bool found_device = false;
			bool found_device_new_name = false;
			for (int i=0; i<m_refresh_net_device_data.size(); i++)
			{
				if(temp_label.serial_number == m_refresh_net_device_data.at(i).nSerial)
				{
					if(cs_temp_label.CompareNoCase( m_refresh_net_device_data.at(i).show_label_name) == 0)
					{
						found_device_new_name = false;
					}
					else
					{
						m_refresh_net_device_data.at(i).show_label_name = cs_temp_label;
						found_device_new_name = true;
					}
					break;
				}
			}
		}
	}



	 CTStat_Net* pT = new CTStat_Net;
	 pT->SetSerialID(temp.nSerial);
	 pT->SetDevID(temp.modbusID);
	 pT->SetProductType(temp.product_id);
	 pT->SetSubnetName(temp.show_label_name);

	 pT->SetIPPort(temp.nport);
	 LPSTR szIP = inet_ntoa(siBind.sin_addr);
	 pT->SetIPAddr((char*)szIP);
	 pT->SetNetworkCardAddress(temp.NetCard_Address);


	return m_refresh_net_device_data.size();



	//return 0;



#if 0
    int nLen=buffer[2]+buffer[3]*256;
    //int n =sizeof(char)+sizeof(unsigned char)+sizeof( unsigned short)*9;
    unsigned short usDataPackage[30]= {0};
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

        /*  BOOL net_open = Open_Socket2(strIP,nPort);
           SetCommunicationType(1);
           if (net_open)
           {
              unsigned short DataBuffer[10];
              int ret = Read_Multi(modbusID,DataBuffer,0,10);
              if (ret>0)
              {
                  float softwareversion = ((float)(DataBuffer[4]+DataBuffer[5]*256))/10;
                  pT->SetSoftwareVersion(softwareversion);
                  pT->SetHardwareVersion(DataBuffer[8]);
              }

           }
           close_com();*/

        pT->SetSerialID(nSerial);
        pT->SetDevID(modbusID);
        pT->SetProductType(nProductID);
        pT->SetSubnetName(m_strSubNet);

        pT->SetIPPort(nPort);
        LPSTR szIP = inet_ntoa(siBind.sin_addr);
        pT->SetIPAddr((char*)szIP);
        pT->SetNetworkCardAddress(local_enthernet_ip);

        _NetDeviceInfo* pni = new _NetDeviceInfo;
        pni->m_pNet = pT;
        bool found_exsit = false;
        for (int z=0; z<m_szNCScanRet.size(); z++)
        {
            unsigned int temp_serial_id;
            temp_serial_id = ((CTStatBase *)(m_szNCScanRet.at(z)->m_pNet))->m_dwSerialID;
            if(temp_serial_id == nSerial)
            {
                found_exsit = true;
            }
        }
        if(!found_exsit)
            m_szNCScanRet.push_back(pni);
    }


    return m_szNCScanRet.size();

#endif
}

BOOL CTStatScanner::binary_search_crc(int a)
{
    //use this for binary search ,inspect
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
        {
            break;
        }
        CString strComPort = pScan->m_szComs[i];

        CString tc = strComPort.Mid(3);

        int n = _wtoi(tc);



        for (int j=0; j<m_scan_info.size(); j++)
        {
            scan_item = -1;
            if(n == m_scan_info.at(j).scan_com_port)
            {
                //&& (19200 ==  m_scan_info.at(j).scan_baudrate)
                scan_item = j;
                //break;
            }

            if(scan_item != -1)
            {
                if(!m_scan_info.at(scan_item).scan_skip)
                {
                    if(pScan->OpenCom(n))
                    {


                        pScan->SetComPort(n);
                        bool bRet = Change_BaudRate(m_scan_info.at(j).scan_baudrate);
                        CString strBraudrate;
                        strBraudrate.Format (_T("%d"),m_scan_info.at(j).scan_baudrate);
                        pScan->SetBaudRate(strBraudrate);
                        scan_baudrate = m_scan_info.at(j).scan_baudrate;

                        ASSERT(bRet);


                        m_scan_info.at(scan_item).scan_status = SCAN_STATUS_RUNNING;
                        pScan->background_binarysearch(n);	//lsc comscan new cold
                        close_com();
                        m_scan_info.at(scan_item).scan_status = SCAN_STATUS_FINISHED;
                        memcpy(m_scan_info.at(scan_item).scan_notes,"Scan finished",strlen("Scan finished"));
                        memset(m_scan_info.at(scan_item).scan_notes,0,250);
                        memcpy(m_scan_info.at(scan_item).scan_notes,"Scan finished",strlen("Scan finished"));
                        Sleep(500);
                        TRACE(_T("Success open the COM%d\n"), n);

                    }
                    else
                    {
                        m_scan_info.at(scan_item).scan_status = SCAN_STATUS_FAILED;
                        memset(m_scan_info.at(scan_item).scan_notes,0,250);
                        memcpy(m_scan_info.at(scan_item).scan_notes,"Cannot open the COM Port",strlen("Cannot open the COM Port"));
                        // 不能打开串口X，提示信息
                        TRACE(_T("Cannot open the COM%d\n"), n);
                        CString str;
                        str.Format(_T("Cannot open the COM%d\n"), n);

                    }
                }
            }
        }


        close_com();

    }
    g_ScnnedNum=254;
    //
    //pScan->AddComDeviceToGrid();

    close_com();
    TRACE(_T("COM Scan finished"));


    if (pScan->m_eScanComEnd->m_hObject)
    {
        pScan->m_eScanComEnd->SetEvent();
        pScan->m_com_scan_end = true;
    }




    return 1;
}


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

extern HWND scan_wait_dlg;
void CTStatScanner::SendScanEndMsg()
{
    //m_pParent->PostMessage(WM_SCANFINISH, 0, 0);
    // scan完成，开始冲突检查

    // 合并同类项
    //if (!m_isChecksubnet)
    {
        CombineScanResult();
        GetBuildingName();	// 获得当前选择的buildingname
        GetAllNodeFromDataBase();
        FindNetConflict();
        ResolveNetConflict();


        //GetBuildingName();	// 获得当前选择的buildingname

        FindComConflict();

        ResolveComConflict();

        CompareNetToComConflict();

        AddNewTStatToDB();
        AddNewNetToDB();









        ((CMainFrame*)m_pParent)->m_bScanALL = FALSE;
        ((CMainFrame*)m_pParent)->m_bScanFinished = TRUE;


        if( m_refresh_net_device_data.size() == 0 && m_szNCScanRet.size()==0 && m_szTstatScandRet.size() == 0)
        {
            //AfxMessageBox(_T("Can't find any device. Please check configure and connection, then try again."));//scan,在有些机子上，总提示这个，但来会继续进行扫描

            m_pParent->PostMessage(WM_ADDTREENODE);
            return ;
        }

        try
        {

			//::PostMessage(scan_wait_dlg,WM_SHOW_SCANN_RESULTS,NULL,NULL);
			return;
            //CScanDlg dlg;
            //dlg.SetScanner(this);
            Sleep(1000000);

            
//            dlg.DoModal();
            //if(IsWindow(dlg.m_hWnd))
            //{
            //    WaitForSingleObject(dlg.m_hWnd, 200);  // 为了线程安全。
            //}

        }
        catch (...)
        {
            TRACE(_T("error int sendscanendMsg \r\n"));
        }


        m_pParent->PostMessage(WM_ADDTREENODE);

        SetCommunicationType(1);
        close_com();
        SetCommunicationType(0);
        close_com();

    }




}


BOOL CTStatScanner::OpenCom(int nCom)
{
    if (!m_bStopScan)
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



        _variant_t temp_variant;
        CString strTemp;

        CString temp_str=_T("select * from ALL_NODE");
        //m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
        m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)temp_str);
        //return m_pRs->get_RecordCount();
        /*   m_szComNodes.clear();
           m_szNetNodes.clear();*/
        int nTemp = 0;
        while(!m_q.eof())
        {
            nTemp++;
            int nDefault=0;
            CString strDevType = m_q.getValuebyName(L"Product_class_ID");

            CTStatBase* pNode = NULL;
            //if (strDevType.CompareNoCase(_T("100")) == 0)	 //100 =  NC
            if (IsNetDevice(strDevType))
            {
                pNode = new CTStat_Net;
                // Port
                CString strPort;
                strPort=m_q.getValuebyName(L"Com_Port");


                ((CTStat_Net*)(pNode))->SetIPPort(_wtoi(strPort));

                CString str_product_id ;
                
                    str_product_id=m_q.getValuebyName(L"Product_class_ID");

                if((_wtoi(str_product_id) == PM_MINIPANEL) ||(_wtoi(str_product_id) == PM_CM5))
                {
                    CString strprotocol;

                   
                        strprotocol=m_q.getValuebyName(L"Protocol");

                    ((CTStat_Net*)(pNode))->SetProtocol(_wtoi(strprotocol));
                }
                else
                {
                    ((CTStat_Net*)(pNode))->SetProtocol(1);//1为modebus ip
                }


                // IP Addr
                CString strIP ;
              
                    strIP=m_q.getValuebyName(L"Bautrate");
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
                CString strBaudRate ;
                
                    strBaudRate=  m_q.getValuebyName(L"Bautrate");

                ((CTStat_Dev*)(pNode))->SetBaudRate(_wtoi(strBaudRate));

                m_szComNodes.push_back(((CTStat_Dev*)(pNode)));

                CString strComPort ;
               
                    strComPort=m_q.getValuebyName(L"Com_Port");
                strComPort = strComPort.Mid(3);
                ((CTStat_Dev*)(pNode))->SetComPort(_wtoi(strComPort));
            }
            pNode->SetProductType(_wtoi(strDevType));

			pNode->SetBuildingName(m_q.getValuebyName(L"MainBuilding_Name"));
			pNode->SetFloorName(m_q.getValuebyName(L"Floor_Name"));
			pNode->SetRoomName(m_q.getValuebyName(L"Room_Name"));
			pNode->SetSubnetName(m_q.getValuebyName(L"Building_Name"));

			CString strID ;

			strID=m_q.getValuebyName(L"Product_ID");
			pNode->SetDevID(_wtoi(strID));

            CString strHwv ;
         
                strHwv=m_q.getValuebyName(L"Hardware_Ver");
            pNode->SetHardwareVersion(float(_wtof(strHwv)));

            CString strSwv;
         
                strSwv=m_q.getValuebyName(L"Software_Ver");
            pNode->SetSoftwareVersion(float(_wtof(strSwv)));

            CString strSerialID;
         
                strSerialID=m_q.getValuebyName(L"Serial_ID");
            unsigned int temp_serial = _wtoi64(strSerialID);
            (pNode)->SetSerialID(temp_serial);

           m_q.nextRow();
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

// 解决冲突
void  CTStatScanner::ResolveComConflict()
{ 
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
                for(int i=0; i<3; i++)
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
                     m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
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
// 如果Serial ID相同，而IP不同，视为冲突
void CTStatScanner::FindNetConflict()
{
    for (UINT i = 0; i < m_szNCScanRet.size(); i++)
    {
        _NetDeviceInfo* pInfo = m_szNCScanRet[i];
        unsigned int nScanID = pInfo->m_pNet->GetSerialID();
        DWORD dwScanIP = pInfo->m_pNet->GetIPAddr();
        float hw_version = pInfo->m_pNet->GetHardwareVersion();
        float sw_version = pInfo->m_pNet->GetSoftwareVersion();
        int   ip_port =  pInfo->m_pNet->GetIPPort();
        int  modbus_id = pInfo->m_pNet->GetDevID();
        pInfo->m_pNet->SetBuildingName(m_strBuildingName);
        pInfo->m_pNet->SetSubnetName(m_strSubNet);

        for (UINT j = 0; j < m_szNetNodes.size(); j++)
        {
            unsigned int nSID = m_szNetNodes[j]->GetSerialID();
            DWORD dwIP = m_szNetNodes[j]->GetIPAddr();
            float db_hw_version = m_szNetNodes[j]->GetHardwareVersion();
            float db_sw_version = m_szNetNodes[j]->GetSoftwareVersion();
            int db_ip_port = m_szNetNodes[j]->GetIPPort();
            int db_modbus_id = m_szNetNodes[j]->GetDevID();
            //加上 buildingname等私货

            if (nScanID == nSID)
            {
                if ((dwScanIP != dwIP) ||(hw_version != db_hw_version) || (sw_version != db_sw_version) || (ip_port != db_ip_port) || (modbus_id != db_modbus_id) ) // 需要矫正
                {
                    pInfo->m_pNet->SetBuildingName(m_szNetNodes[j]->GetBuildingName());
                    pInfo->m_pNet->SetFloorName(m_szNetNodes[j]->GetFloorName());
                    pInfo->m_pNet->SetRoomName(m_szNetNodes[j]->GetRoomName());
                    pInfo->m_pNet->SetSubnetName(m_szNetNodes[j]->GetSubnetName());
                    pInfo->m_pNet->SetHardwareVersion(hw_version);
                    pInfo->m_pNet->SetSoftwareVersion(sw_version);
                    pInfo->m_pNet->SetIPPort(ip_port);
                    pInfo->m_pNet->SetDevID(modbus_id);
                    pInfo->m_bConflict = TRUE;
                    pInfo->m_dwIPDB = dwIP;
                    pInfo->m_dwIPScan = dwScanIP;
                    m_szNetConfNodes.push_back(pInfo);
                }
            }
        }
    }

}

void CTStatScanner::CompareNetToComConflict()
{
    CString NetInfor,ComInfor;
    for (UINT i = 0; i < m_szNCScanRet.size(); i++)
    {
        _NetDeviceInfo* pNetInfo = m_szNCScanRet[i];
        DWORD nNetSerialNo = pNetInfo->m_pNet->GetSerialID();
        int nNetModel=pNetInfo->m_pNet->GetProductType();
        int nNetID=pNetInfo->m_pNet->GetDevID();
        CString NetDeivceName=pNetInfo->m_pNet->GetProductName();
        CString ip=pNetInfo->m_pNet->GetIPAddrStr();
        for (UINT i = 0; i < m_szTstatScandRet.size(); i++)
        {
            _ComDeviceInfo* pComInfo = m_szTstatScandRet[i];
            DWORD nComSerialNo= pComInfo->m_pDev->GetSerialID();
            int nComModelNo = pComInfo->m_pDev->GetProductType();
            int nComID=pComInfo->m_pDev->GetDevID();
            CString ComDeviceName=pComInfo->m_pDev->GetProductName();
            if ((nNetSerialNo==nComSerialNo)&&(nNetModel=nComModelNo)&&(nNetID==nComID))
            {

                CString StrCom;
                StrCom.Format(_T("COM%d"),pComInfo->m_pDev->GetComPort());
                NetInfor.Format(_T("Net Device:IP Address=%s Device Name=%s ID=%d Serial No=%d Model NO=%d"),ip.GetBuffer(),NetDeivceName.GetBuffer(),
                                nNetID,nNetSerialNo,nNetModel);
                ComInfor.Format(_T("Com Device:Com Port=%s Device Name=%s ID=%d Serial No=%d Model NO=%d"),StrCom.GetBuffer(),ComDeviceName.GetBuffer(),
                                nComID,nComSerialNo,nComModelNo);
// 				 CWhichOneToChooseDlg dlg;
// 				 dlg.m_Bool_Check_Net=1;
// 				 dlg.m_Bool_Check_Com=0;
// 				 dlg.m_StrNet=NetInfor;
// 				 dlg.m_StrCom=ComInfor;
// 				 if (dlg.DoModal()==IDOK)
// 				 {
//  					 if (!dlg.m_Bool_Check_Net)//不添加Net
//  					 {
                /*  for (vector<_NetDeviceInfo*>::iterator it=m_szNCScanRet.begin();it!=m_szNCScanRet.end();++it)
                  {

                	  if ((nNetSerialNo==(*it)->m_pNet->GetSerialID())&&(nNetModel=(*it)->m_pNet->GetProductType())&&(nNetID==(*it)->m_pNet->GetDevID()))
                	  {
                		  m_szNCScanRet.erase(it);
                		  break;
                	  }
                  }*/
// 						  if (!dlg.m_Bool_Check_Com)
// 						  {
                for (vector<_ComDeviceInfo*>::iterator it=m_szTstatScandRet.begin(); it!=m_szTstatScandRet.end(); ++it)
                {
                    if ((nComSerialNo==(*it)->m_pDev->GetSerialID())&&(nComModelNo=(*it)->m_pDev->GetProductType())&&(nComID==(*it)->m_pDev->GetDevID()))
                    {
                        m_szTstatScandRet.erase(it);
                        break;
                    }
                }
                /* }*/
                /// }
                /*else if (!dlg.m_Bool_Check_Com)
                {
                 for (vector<_ComDeviceInfo*>::iterator it=m_szTstatScandRet.begin();it!=m_szTstatScandRet.end();++it)
                 {
                	 if ((nComSerialNo==(*it)->m_pDev->GetSerialID())&&(nComModelNo=(*it)->m_pDev->GetProductType())&&(nComID==(*it)->m_pDev->GetDevID()))
                	 {
                		 m_szTstatScandRet.erase(it);
                		 break;
                	 }
                 }
                }*/

                // }


            }


        }
        //DWORD dwScanIP = pInfo->m_pNet->GetIPAddr();

    }
}


//////////////////////////////////////////////////////////////////////////
// 如果Serial ID相同，而IP不同，那么修改数据库中的ID
void  CTStatScanner::ResolveNetConflict()
{
 

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
            CString str_IPaddr;
            CString str_port;
            CString str_modbus_id;
            //strID.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
            strID.Format(_T("%d"),dwScanIP);
            //strSerial.Format(_T("%d"),m_binary_search_product_background_thread.at(j).serialnumber);
            strSerial.Format(_T("%u"),dwSID);


            CString str_baudrate;
            CString hw_instance;
            CString sw_panelnamber;
            hw_instance.Format(_T("%u"),(unsigned int)pInfo->m_pNet->GetHardwareVersion());
            sw_panelnamber.Format(_T("%u"),(unsigned int)pInfo->m_pNet->GetSoftwareVersion());
            str_IPaddr = pInfo->m_pNet->GetIPAddrStr();
            str_baudrate =pInfo->m_pNet->GetIPAddrStr();
            //str_port = pInfo->m_pNet->GetIPPort();
            int int_port = pInfo->m_pNet->GetIPPort();
            int int_id = pInfo->m_pNet->GetDevID();
            str_modbus_id.Format(_T("%d"),int_id);
            str_port.Format(_T("%d"),int_port);
            try
            {

                strSql.Format(_T("update ALL_NODE set Hardware_Ver ='%s' where Serial_ID = '%s'"),hw_instance,strSerial);
                
				m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                strSql.Format(_T("update ALL_NODE set Software_Ver ='%s' where Serial_ID = '%s'"),sw_panelnamber,strSerial);
                m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

                strSql.Format(_T("update ALL_NODE set Bautrate ='%s' where Serial_ID = '%s'"),str_IPaddr,strSerial);
                m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

                strSql.Format(_T("update ALL_NODE set Com_Port ='%s' where Serial_ID = '%s'"),str_port,strSerial);
                m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

                strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),str_modbus_id,strSerial);
               m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


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
        //删掉 存在的序列号
        try
        {

            CString strSql;

            strSql.Format(_T("Delete   From  ALL_NODE Where Serial_ID = '%d' "),nSID);
            m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
        }
        catch (...)
        {

        }



        WriteOneDevInfoToDB(m_szTstatScandRet[i]);

    }
}


void CTStatScanner::AddNewNetToDB()
{
 

 
 
	

	BOOL bIsNew = TRUE;
	for(UINT i = 0; i < m_refresh_net_device_data.size(); i++)
	{
		CString strSql;
		strSql.Format(_T("Select * From  ALL_NODE Where Serial_ID = '%u' "),m_refresh_net_device_data.at(i).nSerial);

		m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
		m_table = m_SqliteDBBuilding.getTable((UTF8MBSTR)strSql);
		//bado.m_pRecordset=bado.OpenRecordset(strSql);
		int nTemp3 =m_table.numRows();


		CString str_ip_address_exist;
		CString str_n_port;
		CString str_serialid;
		CString str_modbus_id;
		CString str_Product_name_view;
		CString NetwordCard_Address;
		CString str_parents_serial;
		CString str_object_instance;
		CString str_panel_number;
		CString str_fw_version;
		CString str_hw_version;
		str_hw_version.Format(_T("%.1f"),m_refresh_net_device_data.at(i).hw_version);
		  CString is_custom;
		str_fw_version.Format(_T("%.1f"),m_refresh_net_device_data.at(i).sw_version);
		str_panel_number.Format(_T("%u"),m_refresh_net_device_data.at(i).panal_number);
		str_object_instance.Format(_T("%u"),m_refresh_net_device_data.at(i).object_instance);
		str_ip_address_exist = m_refresh_net_device_data.at(i).ip_address;
		str_n_port.Format(_T("%d"),m_refresh_net_device_data.at(i).nport);

		str_serialid.Format(_T("%u"),m_refresh_net_device_data.at(i).nSerial);
		str_modbus_id.Format(_T("%d"),m_refresh_net_device_data.at(i).modbusID);
		NetwordCard_Address=m_refresh_net_device_data.at(i).NetCard_Address;

		if(m_refresh_net_device_data.at(i).parent_serial_number != 0)
		{
			str_parents_serial.Format(_T("%u"),m_refresh_net_device_data.at(i).parent_serial_number);
		}
		else
		{
			str_parents_serial = _T("0");
		}


		CString temp_pname;
		CString temp_modbusid;
		CString temp_product_class_id;
		temp_product_class_id.Format(_T("%u"),m_refresh_net_device_data.at(i).product_id);
		temp_modbusid.Format(_T("%d"),m_refresh_net_device_data.at(i).modbusID);
		temp_pname = GetProductName(m_refresh_net_device_data.at(i).product_id);
		if(m_refresh_net_device_data.at(i).show_label_name.IsEmpty())
		{
			str_Product_name_view = temp_pname + _T(":") + str_serialid + _T("-") + temp_modbusid + _T("-") + str_ip_address_exist;
		}
		else
		{
			str_Product_name_view = m_refresh_net_device_data.at(i).show_label_name;
			str_Product_name_view.Remove('\'');
			str_Product_name_view.Remove('\%');
		}


		if(nTemp3 >= 1)
		{

			strSql.Format(_T("update ALL_NODE set NetworkCard_Address='%s', Product_class_ID = '%s', Object_Instance = '%s' , Panal_Number = '%s' ,  Bautrate ='%s',Software_Ver = '%s' ,Com_Port ='%s',Product_ID ='%s', Protocol ='1',Product_name = '%s',Online_Status = 1,Parent_SerialNum = '%s' where Serial_ID = '%s'"),NetwordCard_Address,temp_product_class_id,str_object_instance,str_panel_number,str_ip_address_exist,str_fw_version,str_n_port,str_modbus_id,str_Product_name_view,str_parents_serial,str_serialid);

			try
			{
				 
				m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}

		}
		else
		{
			//插入
			CString temp_pro4;
			bool is_bacnet_device = false;
			if((m_refresh_net_device_data.at(i).product_id == PM_MINIPANEL) || (m_refresh_net_device_data.at(i).product_id == PM_CM5))
				is_bacnet_device = true;
			if(is_bacnet_device)
				temp_pro4.Format(_T("%d"),PROTOCOL_BACNET_IP);
			else
				temp_pro4.Format(_T("%d"),MODBUS_TCPIP);
				
			 is_custom = _T("0");
			  CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
				
			strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('"+pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurSubBuldingName+"','"+NetwordCard_Address+"','"+str_serialid+"','floor1','room1','"+str_Product_name_view+"','"+temp_product_class_id+"','"+str_modbus_id+"','""','"+str_ip_address_exist+"','T3000_Default_Building_PIC.bmp','"+str_hw_version+"','"+str_fw_version+"','"+str_n_port+"','0','"+temp_pro4+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
		/*	 bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);*/
             m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		}

	}


   
#if 0
    BOOL bIsNew = TRUE;
    for(UINT i = 0; i < m_szNCScanRet.size(); i++)
    {
        bIsNew = TRUE;
        _NetDeviceInfo* pInfo = m_szNCScanRet[i];
        pInfo->m_pNet->SetBuildingName(m_strBuildingName);
        pInfo->m_pNet->SetSubnetName(m_strSubNet);
        pInfo->m_pNet->SetFloorName(m_strFloorName);
        pInfo->m_pNet->SetRoomName(m_strRoomName);

        unsigned int nSID = pInfo->m_pNet->GetSerialID();
        int nSProtocol = pInfo->m_pNet->GetProtocol();
        if((nSProtocol <0) || (nSProtocol >3))
            nSProtocol = MODBUS_TCPIP;//default it will be modbus protocol;

        CString temp_cs;
        temp_cs.Format(_T("%d"),pInfo->m_pNet->GetProductType());
        if(!IsNetDevice(temp_cs))
            continue;

//         for (UINT j = 0; j < m_szNetNodes.size(); j++)
//         {
//             unsigned int nNodeSID = m_szNetNodes[j]->GetSerialID();
//             //Comment by Fance
//             //if the scan device is CM5 or minipanel, this products has 3 protocol, BacnetIP modbus485 modbus tcp;
//             //So when scan bacnet ip and midbus tcp ,the or replay to t3000,
//             //So I display the device in two format,judge to 2 decvice;
//             if((pInfo->m_pNet->GetProductType() == PM_CM5) || (pInfo->m_pNet->GetProductType() == PM_MINIPANEL))
//             {
//                 unsigned int nNodeSID = m_szNetNodes[j]->GetSerialID();
//                 int nNodeProtocol = m_szNetNodes[j]->GetProtocol();
//                 if (nSID == nNodeSID)
//                 {
//                     if(nSProtocol == nNodeProtocol)
//                     {
//                         bIsNew = FALSE;
//                         break;
//                     }
//                     else
//                     {
//                         CString strSql;
//                         CString strText;
//                         strText.Format(_T("%u"),nNodeSID);
//                         strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s'"),strText);
//                         CString strTemp;
//                         strTemp.Format(_T("Are you sure to delete thise item"));
//                         //if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
//                         //{
//                         try
//                         {
//                             bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
//                         }
//                         catch(_com_error *e)
//                         {
//                             AfxMessageBox(e->ErrorMessage());
//                         }
//                         //}
//                         bIsNew = TRUE;
//                         break;
//                     }
//                 }
//             }
//             else
//             {
//                 if (nSID == nNodeSID)
//                 {
//                     bIsNew = FALSE;
//                     break;
//                 }
//             }
//         }

		//不加这句话能解决扫描后 名字会变掉
		//但是加了 会出现 DB是空的时候  明明扫描到了 设备，却要等很长时间 靠后台扫描 才显示出来;
#if 1



        try
        {

            CString strSql;
			 strSql.Format(_T("Select * From  ALL_NODE Where Serial_ID = '%d' "),nSID);
			  
			  
			   m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			   m_table = m_SqliteDBBuilding.getTable((UTF8MBSTR)strSql);
			  int nTemp3 = m_table.numRows();
			  if(nTemp3 >= 1)
			  {
				   				   continue;
			  }
        }
        catch (...)
        {

        }


        WriteOneNetInfoToDB(pInfo);
#endif
    }



#endif
}

void CTStatScanner::WriteOneNetInfoToDB( _NetDeviceInfo* pInfo)
{
    ASSERT(pInfo);
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

    CString strBackground_bmp=_T("T3000_Default_Building_PIC.bmp");



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
    float softrev = 0.0;
    float hardrev = 0.0;
    //if (nClassID == PM_TSTAT6||nClassID == PM_TSTAT7||nClassID == PM_HUMTEMPSENSOR||nClassID ==PM_AirQuality||nClassID ==PM_HUM_R)
    //{
    //    BOOL Is_Open=Open_Socket2(strIP,pInfo->m_pNet->GetIPPort());
    //    if (Is_Open)
    //    {
    //        SetCommunicationType(1);
    //        unsigned short DataBuffer[10] ;
    //        int ret = Read_Multi(nID,DataBuffer,0,10);
    //        if (ret>0)
    //        {
    //
    //                softrev = ((float)(DataBuffer[5]*256+ DataBuffer[4]))/10;
    //
    //                hardrev =   DataBuffer[8];

    //               int ret=Read_Multi(nID,DataBuffer,714,10);
    //
    //             if (DataBuffer[0]==0x56)
    //             {
    //               /*  strProductName.Format(_T("%s%s"),GetTextFromReg(715),GetTextFromReg(719));*/
    //                 strProductName.Format(_T("%s%s"),GetTextFromReg_Buffer(1,DataBuffer),GetTextFromReg_Buffer(5,DataBuffer));
    //             }

    //        }
    //        close_com();
    //    }
    //}



    CString strHWV;
    strHWV.Format(_T("%0.1f"), hardrev);

    CString strSWV;
    strSWV.Format(_T("%0.1f"), softrev);
    CString NetwordCard_Address;
    NetwordCard_Address=pInfo->m_pNet->GetNetworkCardAddress();

    CString strSubnetName = m_strSubNet;
    //GetNetDevSubnetName(strIP);

    CString strEPSize;
    if(pInfo->m_pNet->GetProtocol()== PROTOCOL_BACNET_IP)//如果是bacnetip 需要往数据库里保存 协议 3 就是bacnetip;
    {
        CString temp_pro = _T("3");
        strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Custom,Online_Status)					  values('"
                         +m_strBuildingName+"','"+m_strSubNet+"','"+NetwordCard_Address+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"','"+temp_pro+"','0',1)"));
    }
    else
    {
        CString temp_pro = _T("1");// protocol type 1 is modbus tcp
        strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Custom,Online_Status)					  values('"
                         +m_strBuildingName+"','"+m_strSubNet+"','"+NetwordCard_Address+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"','"+temp_pro+"','0',1)"));
    }

    try
    {

        m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
    }
    catch(_com_error *e)
    {
        AfxMessageBox(e->ErrorMessage());
    }
}

void CTStatScanner::WriteOneDevInfoToDB( _ComDeviceInfo* pInfo)
{
    ASSERT(pInfo);
 


    CString strID;
    int nID = pInfo->m_pDev->GetDevID();
    strID.Format(_T("%d"),  nID);

    CString strProductName = pInfo->m_pDev->GetProductName();

    CString strSerialID;
    strSerialID.Format(_T("%d"), pInfo->m_pDev->GetSerialID());


    int nClassID = pInfo->m_pDev->GetProductType();
    CString strClassID;
    strClassID.Format(_T("%d"), nClassID);


// 	in_addr ia;
// 	ia.S_un.S_addr = pInfo->m_pNet->GetIPAddr();
// 	strBaudRate = CString(inet_ntoa(ia));
    //strBaudRate = pInfo->m_tstatip;

    CString strScreenName;
    strScreenName.Format(_T("Screen(S:%d--%d)"), pInfo->m_pDev->GetSerialID(), pInfo->m_pDev->GetDevID() );

    CString strBackground_bmp=_T("T3000_Default_Building_PIC.bmp");

    CString strHWV;
    strHWV.Format(_T("%0.1f"), pInfo->m_pDev->GetHardwareVersion());

    CString strSWV;
    strSWV.Format(_T("%0.1f"), pInfo->m_pDev->GetSoftwareVersion());



//	CString strCom;//scan
//	strCom.Format(_T("COM%d"), pInfo->m_pDev->GetComPort());//scan

    int nBaudRate = pInfo->m_pDev->GetBaudRate();//scan
    // 	CString strBaudRate;//scan

    CString strBaudRate;//scan
    strBaudRate.Format(_T("%d"), nBaudRate);//scan
    CString strCom;//scan
    //strCom.Format(_T("%d"), pInfo->m_pNet->GetIPPort());
    //strCom = pInfo->m_tstatport;strBaudRate
    strCom.Format(_T("%d"),pInfo->m_pDev->m_nComPort);
    if (pInfo->m_pDev->m_nComPort==-1)
    {
        strBaudRate=pInfo->m_tstatip;
        strCom=pInfo->m_tstatport;
    }
    if (nClassID == PM_TSTAT6||nClassID == PM_TSTAT7||nClassID == PM_TSTAT8||nClassID == PM_TSTAT5i
            ||nClassID == PM_HUMTEMPSENSOR||nClassID ==PM_AirQuality||nClassID ==PM_HUM_R||nClassID == PM_CO2_RS485||nClassID == PM_CO2_NODE)
    {
        BOOL Is_Open=FALSE;
        if (pInfo->m_pDev->m_nComPort==-1)
        {
            /*  strBaudRate=pInfo->m_tstatip;
              strCom=pInfo->m_tstatport;*/
            Is_Open=Open_Socket2(pInfo->m_tstatip,_wtoi(pInfo->m_tstatport));
            SetCommunicationType(1);
        }
        else
        {
            Is_Open = open_com(pInfo->m_pDev->m_nComPort) ;
            Change_BaudRate(nBaudRate) ;
            SetCommunicationType(0);
        }

        if (Is_Open)
        {

            unsigned short DataBuffer[20] ;




            int ret=Read_Multi(nID,DataBuffer,714,20);
            if (ret>0)
            {
                if (DataBuffer[0]==0x56)
                {
                    strProductName.Format(_T("%s%s"),GetTextFromReg_Buffer(1,DataBuffer),GetTextFromReg_Buffer(5,DataBuffer));

                }
            }
			 ret = Read_Multi(nID,DataBuffer,0,20);
			 if (ret > 0)
			 {

				// if ((DataBuffer[7]==PM_CO2_RS485&&DataBuffer[14] == 6)||DataBuffer[7]==PM_HUMTEMPSENSOR)
				 if ((product_register_value[7]==PM_CO2_RS485&&product_register_value[14] == 6)
				 ||product_register_value[7]==PM_HUMTEMPSENSOR
				 ||product_register_value[7]==STM32_HUM_NET
				 ||product_register_value[7]==STM32_HUM_RS485)
				 {
					 strProductName = L"Hum Sensor";
				 }

			 }
		

            close_com();
        }
    }

    CString strMainetInfo = pInfo->m_pDev->m_mainnet_info.GetMainnetInfo();


    try
    {

        CString strSql;
        CString strEpSize;
        strEpSize.Format(_T("%d"), pInfo->m_pDev->GetEPSize());


        strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Online_Status,Protocol,Custom) values('"
                         +m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"',1,0,'0')"));
        //new nc// strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize, Mainnet_info) values('"+m_strBuildingName+"','"+m_strSubNet+"','"+strSerialID+"','"+m_strFloorName+"','"+m_strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"','"+strMainnetInfo+"')"));
        m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
    }
    catch (...)
    {

    }

}


void CTStatScanner::GetBuildingName()
{
    CppSQLite3DB SqliteDBT3000;

	CString strSql;
	strSql.Format(_T("select * from Building order by Main_BuildingName"));
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath); 
	m_q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	 
	int temp_row=0;
	CString str_temp;
	str_temp.Empty();
	_variant_t temp_variant;
	while(!m_q.eof())
	{
		++temp_row;

		int bdef=0;
		bdef=m_q.getIntField("Default_SubBuilding");
		if(bdef == 1)//def building;
		{
			 
			 
				m_strBuildingName  = m_q.getValuebyName(L"Main_BuildingName");


		 
				m_strSubNet = m_q.getValuebyName(L"Building_Name");

			 
			m_strFloorName = _T("floor1");

 
			m_strRoomName = _T("room1");

		}

		m_q.nextRow();
	}
 
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

    return (m_bNetScanFinish);

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
    m_nBaudrate = 0;//_wtoi(strBaudrate);
    /*if (m_nBaudrate != 9600 && m_nBaudrate != 19200 && m_nBaudrate != 38400)
    {
    	m_nBaudrate = 19200;
    }*/
    for (int i=0; i<5; i++)
    {
        if (strBaudrate.CompareNoCase(Building_Baudrate[i])==0)
        {
            m_nBaudrate = _wtoi(strBaudrate);
        }
    }
    if (m_nBaudrate == 0)
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







void CTStatScanner::ScanSubnetFromEthernetDevice()//scan 
{
	m_tstat_net_device_data.clear();
	int controller_counter = 0;
	for (int i=0;i<m_refresh_net_device_data.size();i++)
	{
		if((m_refresh_net_device_data.at(i).product_id != PM_MINIPANEL) && (m_refresh_net_device_data.at(i).product_id != PM_CM5))
		{
			continue;
		}

		controller_counter ++ ;
	}

	if(controller_counter > 0)
	{
		CString show_cstring;
		show_cstring.Format(_T("Find %d T3 Panel(s) ,Do you want to scan the subnet?\r\n'RS485 Main' 9600\r\n'RS485 Main' 19200\r\n'RS485 Main' 384000\r\n'RS485 Main' 76800\r\n'RS485 Main' 115200\r\n'RS485 Sub' 9600\r\n'RS485 Sub' 19200\r\n"),controller_counter);
		if(AfxMessageBox(show_cstring,MB_YESNO)==IDNO)
		{
			return;
		}
	}

	for (int i=0;i<m_refresh_net_device_data.size();i++)
	{
		if((m_refresh_net_device_data.at(i).product_id != PM_MINIPANEL) && (m_refresh_net_device_data.at(i).product_id != PM_CM5))
		{
			continue;
		}




		 CString strIP;
		 int net_port;
		 strIP = m_refresh_net_device_data.at(i).ip_address;
		 net_port = m_refresh_net_device_data.at(i).nport;
		 
		 //CString temp_str;
		 //temp_str.Format(_T("Scanning Panel %d ,Main subnet"));
		 //char temp_char[250];
		 //WideCharToMultiByte( CP_ACP, 0, temp_str.GetBuffer(), -1, temp_char, 250, NULL, NULL );
		 //memcpy(m_scan_info.at(0).scan_notes,temp_char,250);

		 g_CommunicationType=1;
		 SetCommunicationType(g_CommunicationType);
		 bool connect_ret=Open_Socket2(strIP, net_port);
		 if(connect_ret)
		 {
			 unsigned short temp_data[2];
			 int read_version_ret = 0;
			 read_version_ret = Read_Multi(255,temp_data,4,2);
			 if(read_version_ret < 0)
				 continue;
			 int tempversion = temp_data[1] * 10 + temp_data[0];
			 if(tempversion < 433)
				 continue;
			 for (int sub_com=0;sub_com<3;sub_com++ )
			 {
				 int reset_sub_port = 0;
				 int reset_sub_baudrate = 0;
				 m_device_ip_address = strIP;
				 m_device_ip_port = net_port;
				 m_parent_serialnum = m_refresh_net_device_data.at(i).nSerial;

				 for (int sub_baudrate = UART_9600;sub_baudrate <UART_115200 + 1;sub_baudrate++)
				 {
					 if(sub_com == 1)
					 {
						 m_device_baudrate = _wtoi(Baudrate_Array[UART_38400]);
						 m_device_com_port = sub_com;
					 }
					 else
					 {
						 m_device_baudrate = _wtoi(Baudrate_Array[sub_baudrate]);
						 m_device_com_port = sub_com;
					 }


					 reset_sub_port = write_one(255,96,sub_com);
					 reset_sub_baudrate = write_one(255,97,sub_baudrate);
					 if((reset_sub_port > 0) && (reset_sub_baudrate > 0))
						 modbusip_to_modbus485(m_device_com_port,m_device_baudrate,  1, 254);
					 else
					 {
						DFTrace(_T("write 96 97 failed"));
						continue;
					 }
					  if(sub_com == 1)
					  {
						  break;
					  }

					  if((sub_com == 0) && (sub_baudrate == UART_19200))
					  {
						  break;
					  }
				 }

			 }

		 }
		 else
		 {
			 DFTrace(_T("Connect modbus ip failed"));
		 }
	}

	for (int j=0;j<m_tstat_net_device_data.size();j++)
	{
		int find_exsit = false;
		for (int z=0;z<m_refresh_net_device_data.size();z++)
		{
			if(m_tstat_net_device_data.at(j).nSerial == m_refresh_net_device_data.at(z).nSerial)
			{
				find_exsit = true;
			}
		}
		if(find_exsit == false)
			m_refresh_net_device_data.push_back(m_tstat_net_device_data.at(j));
	}
	

#if 0
    CString g_strT3000LogString;
    for(UINT n = 0; n < m_szNCScanRet.size(); n++)
    {
        _NetDeviceInfo* pNCInfo = m_szNCScanRet[n];
        CString strPort;

        int temp_product_type = pNCInfo->m_pNet->GetProductType();
        if((temp_product_type == PM_MINIPANEL) || (temp_product_type == PM_CM5))	//如果是Minipanel或者CM5就不要扫描下面的,这两设备不会回复的;
        {
            continue;
        }

        if((temp_product_type != PM_CO2_NET) && (temp_product_type != PM_NC) && (temp_product_type != PM_LightingController))
        {
            continue;
        }

        int nIPPort=pNCInfo->m_pNet->GetIPPort();

        CString strIP=pNCInfo->m_pNet->GetIPAddrStr();
        //if(strIP.CompareNoCase(_T("192.168.0.145"))!=0)
        //	continue;

        g_strT3000LogString.Format(_T("Scan Tstat connect to %s IPAdd: %s IPPort: %d"),pNCInfo->m_pNet->GetProductName(),strIP,nIPPort);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        //##############################
        CString Product_Name_Mini=pNCInfo->m_pNet->GetProductName();
        CString strInfo;
        strInfo.Format(_T("Scan Tstat connected to %s "),Product_Name_Mini);
        ShowNetScanInfo(strInfo);
        //##############################

        m_port.Format(_T("%d"),nIPPort);

        m_ip = strIP;

        int net_port=pNCInfo->m_pNet->GetIPPort();

        g_CommunicationType=1;
        SetCommunicationType(g_CommunicationType);
        bool b=Open_Socket2(strIP, net_port);
        if(b)
        {
            strInfo.Format((_T("Connect to IP : %s successful")), strIP);//prompt info;
            CString* pstrInfo = new CString(strInfo);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        }
        else
        {
            strInfo.Format((_T("Connect to IP : %s failure")), strIP);//prompt info;
            CString* pstrInfo = new CString(strInfo);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
            continue;
        }

        binarySearchforComDevice(-1, TRUE, 1, 254);

    }
#endif
}



///*
// 使用485 scan 广播来查找
void CTStatScanner::ScanTstatFromNCForAuto()//scan 分别扫描各个NC中的TSTAT
{
    //Alex_Flag
    //Createfile();
    CString g_strT3000LogString;
    for(UINT n = 0; n < m_szNCScanRet.size(); n++)
    {
        _NetDeviceInfo* pNCInfo = m_szNCScanRet[n];
        CString strPort;

        int temp_product_type = pNCInfo->m_pNet->GetProductType();
        if((temp_product_type == PM_MINIPANEL) || (temp_product_type == PM_CM5))	//如果是Minipanel或者CM5就不要扫描下面的,这两设备不会回复的;
        {
            continue;
        }

        if((temp_product_type != PM_CO2_NET) && (temp_product_type != PM_NC) && (temp_product_type != PM_LightingController))
        {
            continue;
        }

        int nIPPort=pNCInfo->m_pNet->GetIPPort();

        CString strIP=pNCInfo->m_pNet->GetIPAddrStr();
        //if(strIP.CompareNoCase(_T("192.168.0.145"))!=0)
        //	continue;

        g_strT3000LogString.Format(_T("Scan Tstat connect to %s IPAdd: %s IPPort: %d"),pNCInfo->m_pNet->GetProductName(),strIP,nIPPort);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        //##############################
        CString Product_Name_Mini=pNCInfo->m_pNet->GetProductName();
        CString strInfo;
        strInfo.Format(_T("Scan Tstat connected to %s "),Product_Name_Mini);
        ShowNetScanInfo(strInfo);
        //##############################

        m_port.Format(_T("%d"),nIPPort);

        m_ip = strIP;

        int net_port=pNCInfo->m_pNet->GetIPPort();

        g_CommunicationType=1;
        SetCommunicationType(g_CommunicationType);
        bool b=Open_Socket2(strIP, net_port);
        if(b)
        {
            strInfo.Format((_T("Connect to IP : %s successful")), strIP);//prompt info;
            CString* pstrInfo = new CString(strInfo);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        }
        else
        {
            strInfo.Format((_T("Connect to IP : %s failure")), strIP);//prompt info;
            CString* pstrInfo = new CString(strInfo);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
            continue;
        }

        binarySearchforComDevice(-1, TRUE, 1, 254);




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
    int a=g_NetController_CheckTstatOnline_a(devLo,devHi, false);

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
        {
            //Two Tstat have the same ID,fewness
            do
            {

                nCount++;
                if(nCount>=5)
                {
                    nCount=0;
                    break;
                }

                Sleep(20);//////////////////////////////////for running is better
                char c_temp_arr[100]= {'\0'};
                if(Read_One(devLo,10)!=-2)//one times
                {
                    Sleep(100);
                    CString str_temp;
                    for(int j=254; j>=1; j--)
                    {
                        if(j!=devLo)
                        {
                            //	if(!found_same_net_work_controller_by_mac(a))
                            if(1)
                            {
                                bool find=false;//false==no find;true==find
                                for(UINT w=0; w<m_szNCScanRet.size(); w++)
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
                                                {
                                                    tstat_version2 /=10;
                                                }
                                                else
                                                {
                                                    tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);
                                                    tstat_version2 /=10;
                                                }//tstat_version

                                                pInfo->m_pNet->SetSoftwareVersion(tstat_version2); 				//temp.software_version=tstat_version2;
                                                int nTstatBaudrate = 19200;
                                                if(Read_One2(nSourceID, 185, false)==0)
                                                {
                                                    nTstatBaudrate=9600;
                                                }
                                                pInfo->m_pNet->SetBaudRate(nTstatBaudrate);
                                                pInfo->m_pNet->SetBuildingName(m_strBuildingName);
                                                pInfo->m_pNet->SetSubnetName(m_strSubNet);
                                                //int nEPsize=Read_One2(nSourceID,326, false);
                                                //pInfo->m_pNet->SetEPSize(nEPsize);

                                                if(serialnumber>=0)
                                                {
                                                    m_szNCScanRet.push_back(pInfo);
                                                }
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
            }
            while(1);   //  do while
        }
        break;
    case -4:
        break;
    //no connection
    case -5:
        break;
        //the input error
    }
}



//*/



// 代码改编自void CMainFrame::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
UINT _ScanOldNC(LPVOID pParam)
{
    CTStatScanner *pScan = (CTStatScanner*) pParam;
    pScan->m_eScanOldNCEnd->SetEvent();
    return 0;
    CString strP = _T("Modbus TCP");
    CString g_strT3000LogString;
// 	g_strT3000LogString=_T("Scan By Modbus TCP Time: ")+Get_NowTime()+_T("\n");
// 	//WriteLogFile(g_strT3000LogString);
    CString* pstrInfo = new CString(g_strT3000LogString);
    ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));

    Building_info bi = ((CMainFrame*)(pScan->m_pParent))->m_subNetLst.at(((CMainFrame*)(pScan->m_pParent))->m_nCurSubBuildingIndex);

    //if (bi.strProtocol.CompareNoCase(strP) != 0)
    //{

    //}
    WaitForSingleObject(pScan->m_eScanNCEnd, INFINITE);


    pScan->ScanOldNC(1,254);

    return 1;
}


// 代码改编自void CMainFrame::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
void CTStatScanner::ScanOldNC(BYTE devLo, BYTE devHi)
{
    TRACE(_T("start TCP scan^^^^^^^^^ \n"));

    int a=g_NetController_CheckTstatOnline_a(1,254, true);



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


            if(serialnumber>=0)
            {
                pTemp->m_pNet = pNet;
                m_szNCScanRet.push_back(pTemp);
            }
        }
    }


    m_eScanOldNCEnd->SetEvent();

    TRACE(_T("End TCP scan^^^^^^^^^ \n"));
}


void CTStatScanner::Initial_Scan_Info()
{

    m_scan_info.clear();
    vector <CString> temp_serialport;
    GetSerialComPortNumber1(temp_serialport);
    int ncount = temp_serialport.size();

    Scan_Info temp_scan_info;

    temp_scan_info.scan_list_item = 0;
    temp_scan_info.scan_baudrate = 0;
    temp_scan_info.scan_com_port = 0;
    temp_scan_info.scan_mode = SCAN_BY_UDP;
    if((current_building_protocol == P_AUTO) || (current_building_protocol == P_BACNET_IP) || (current_building_protocol == P_MODBUS_TCP))
    {
        temp_scan_info.scan_skip = false;
        temp_scan_info.scan_status = SCAN_STATUS_WAIT;
    }
    else
    {
        temp_scan_info.scan_skip = true;
        temp_scan_info.scan_status = SCAN_STATUS_SKIP;
    }
    temp_scan_info.scan_found = 0;
    memset(temp_scan_info.scan_notes,0,250);
    m_scan_info.push_back(temp_scan_info);
    m_scan_info_buffer.push_back(temp_scan_info);
    scan_udp_item = 0;


 
        for (int i=0; i<ncount; i++)
        {
            CString temp_cs;
            int temp_port;
            temp_cs = temp_serialport.at(i).Right(temp_serialport.at(i).GetLength() - 3);
            temp_port = _wtoi(temp_cs);

            int  intBaudate;

            for(int baudrate = 0; baudrate<NUMBER_BAUDRATE; baudrate++)
            {

                intBaudate = _wtoi (c_strBaudate[baudrate]);

                temp_scan_info.scan_list_item = i*2 + baudrate+1;
                temp_scan_info.scan_baudrate = intBaudate;
                temp_scan_info.scan_com_port = temp_port;
                temp_scan_info.scan_mode = SCAN_BY_SERIAL_PORT;
                if((current_building_protocol == P_AUTO) || ((current_building_protocol == P_MODBUS_485)&& (current_building_baudrate == intBaudate)&&(current_building_comport == temp_port) ))
                {
                    temp_scan_info.scan_skip = false;
                    temp_scan_info.scan_status = SCAN_STATUS_WAIT;
                }
                else
                {
                    temp_scan_info.scan_skip = true;
                    temp_scan_info.scan_status = SCAN_STATUS_SKIP;
                }
                temp_scan_info.scan_found = 0;
                memset(temp_scan_info.scan_notes,0,250);
                m_scan_info.push_back(temp_scan_info);
                m_scan_info_buffer.push_back(temp_scan_info);

            }

        }
  
#if 1
    scan_bacnet_ip_item = ncount*NUMBER_BAUDRATE + 1;
    temp_scan_info.scan_list_item = ncount*NUMBER_BAUDRATE + 1;

    temp_scan_info.scan_com_port = 0;
    temp_scan_info.scan_mode = SCAN_BY_MSTP;
    if(current_building_protocol == P_BACNET_MSTP)
    {
        temp_scan_info.scan_skip = false;
        temp_scan_info.scan_status = SCAN_STATUS_WAIT;
        temp_scan_info.scan_baudrate = current_building_baudrate;
    }
    else
    {
        temp_scan_info.scan_skip = true;
        temp_scan_info.scan_status = SCAN_STATUS_SKIP;
        temp_scan_info.scan_baudrate = 38400;
    }

    temp_scan_info.scan_found = 0;
    memset(temp_scan_info.scan_notes,0,250);
    m_scan_info.push_back(temp_scan_info);
    m_scan_info_buffer.push_back(temp_scan_info);


    scan_remote_ip_item =  ncount*NUMBER_BAUDRATE + 2;
    temp_scan_info.scan_list_item = ncount*NUMBER_BAUDRATE + 2;

    temp_scan_info.scan_com_port = 0;
    temp_scan_info.scan_mode = SCAN_BY_REMOTE_IP;
    if(current_building_protocol == P_REMOTE_DEVICE)
    {
        temp_scan_info.scan_skip = false;
        temp_scan_info.scan_status = SCAN_STATUS_WAIT;
        temp_scan_info.scan_baudrate = current_building_baudrate;
    }
    else
    {
        temp_scan_info.scan_skip = true;
       temp_scan_info.scan_status = SCAN_STATUS_SKIP;
        temp_scan_info.scan_baudrate = 0;
    }

    temp_scan_info.scan_found = 0;

    memset(temp_scan_info.scan_notes,0,250);
    m_scan_info.push_back(temp_scan_info);
    m_scan_info_buffer.push_back(temp_scan_info);
#endif
}
void CTStatScanner::ScanAll()
{
    m_szNCScanRet.clear();	//Clear all the old information ,when we start a new scan;
    GetBuildingName();


    Initial_Scan_Info();

    b_pause_refresh_tree = true ;
    ScanComDevice();


    ScanNetworkDevice();



    ScanBacnetIPDevice();

    ScanRemoteIPDevice();

	hwait_scan_thread =CreateThread(NULL,NULL,_WaitScanThread,this,NULL, NULL);

    //AfxBeginThread(_WaitScanThread, this);




}



DWORD WINAPI  _WaitScanThread(LPVOID lpVoid)
//UINT _WaitScanThread(PVOID pParam)
{
    Sleep(1000);
    BOOL Flag = FALSE;
    //CTStatScanner* pScanner = (CTStatScanner*)(pParam);
	 CTStatScanner* pScanner = (CTStatScanner*)(lpVoid);
    switch(pScanner->m_scantype)
    {
    case 1:
        if (WaitForSingleObject(pScanner->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
            Flag = TRUE;
        break;
    case 2:
        if ((WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
                &&(WaitForSingleObject(pScanner->m_eScanOldNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))
            Flag = FALSE;
        break;
    case 3:
        //if ((WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
        //	&&(WaitForSingleObject(pScanner->m_eScanOldNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
        //	&& (WaitForSingleObject(pScanner->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
        //	&&  (WaitForSingleObject(pScanner->m_eScanBacnetIpEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))
        //	Flag = TRUE;

        if (WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 )
        {
            if(WaitForSingleObject(pScanner->m_eScanOldNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
            {
                if(WaitForSingleObject(pScanner->m_eScanBacnetIpEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
                {
                    if(WaitForSingleObject(pScanner->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
                    {
                        if((WaitForSingleObject(pScanner->m_eScanRemoteIPEnd->m_hObject, INFINITE) == WAIT_OBJECT_0))
                        {
                            Flag = TRUE;
                            break;
                        }
                    }
                }

            }

        }
        Flag = FALSE;

        break;
    case 4:

// 		if (WaitForSingleObject(pScanner->m_eScanComOneByOneEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
// 			Flag = FALSE;
        break;
    case 5:
        if ((WaitForSingleObject(pScanner->m_eScanNCEnd->m_hObject, INFINITE) == WAIT_OBJECT_0 ))
            Flag = FALSE;
        break;
    default:
    {
       // b_pause_refresh_tree = false;
        pScanner->m_bNetScanFinish = TRUE; // at this time, two thread end, all scan end
        return 0;
    }

    }

    if (Flag)
    {
		scan_item = 0;

		m_scan_info.at(0).scan_status = SCAN_STATUS_RUNNING;
		pScanner->ScanSubnetFromEthernetDevice();



#if 0

		Sleep(1000000);
        //////////////////////////////////////////////////////////////////////////
        g_nStartID = 1;
        //Alex_Flag
        if (!pScanner->m_bStopScan)
        {
            if((current_building_protocol == P_AUTO) || (current_building_protocol == P_MODBUS_TCP))
            {
                g_strScanInfoPrompt = _T("TStat connected with NC");
                CString* pstrInfo = new CString(g_strScanInfoPrompt);
                ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
                //pScanner->ScanTstatFromNCForAuto();
            }
        }
#endif

        pScanner->m_bNetScanFinish = TRUE; // at this time, two thread end, all scan end

        pScanner->SendScanEndMsg();

    }
    else
    {
        g_ScnnedNum=254;
        pScanner->m_bNetScanFinish = TRUE;
        if (!pScanner->m_thesamesubnet)
        {

            pScanner->SendScanEndMsg();
        }
        else
        {
            pScanner->m_bCheckSubnetFinish=TRUE;
        }


    }
    pScanner->m_bNetScanFinish = TRUE;
    //b_pause_refresh_tree = false;

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
    return;
    CString* pstrInfo = new CString(strInfo);
    if(((CMainFrame*)(m_pParent))->m_pWaitScanDlg)
        PostMessage(((CMainFrame*)(m_pParent))->m_pWaitScanDlg->m_hWnd, WM_NETSCANINFO, WPARAM(pstrInfo), LPARAM(0));
}

void CTStatScanner::ShowComScanInfo(const CString& strInfo)
{
    return;
    CString* pstrInfo = new CString(strInfo);
    if(((CMainFrame*)(m_pParent))->m_pWaitScanDlg)
        PostMessage(((CMainFrame*)(m_pParent))->m_pWaitScanDlg->m_hWnd, WM_COMSCANINFO, WPARAM(pstrInfo), LPARAM(0));
}

void CTStatScanner::ShowBacnetComScanInfo(const CString& strInfo)
{
    return;
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
    CString g_strT3000LogString;
    for(UINT n = 0; n < m_szNCScanRet.size(); n++)
    {
        _NetDeviceInfo* pNCInfo = m_szNCScanRet[n];
        CString strPort;
        int nIPPort=pNCInfo->m_pNet->GetIPPort();
        CString strIP=pNCInfo->m_pNet->GetIPAddrStr();
        //##############################
        CString strInfo;
        strInfo.Format(_T("Scan Tstat connected to %s"), pNCInfo->m_pNet->GetProductName());
        ShowNetScanInfo(strInfo);
        //##############################
        int net_port=pNCInfo->m_pNet->GetIPPort();
        g_CommunicationType=1;
        SetCommunicationType(g_CommunicationType);
        bool b=Open_Socket2(strIP, net_port);

        //	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
        //	SetPaneString(1,strInfo);
        if(b)
        {
            g_strT3000LogString.Format((_T("Connect to IP : %s successful")), strIP);//prompt info;

            CString* pstrInfo = new CString(g_strT3000LogString);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        }
        else
        {
            g_strT3000LogString.Format((_T("Connect to IP : %s failure")), strIP);//prompt info;

            CString* pstrInfo = new CString(g_strT3000LogString);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
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
        CString strInfo;
        strInfo.Format(_T("Scan Tstat connected to %s"), pNCInfo->m_pNet->GetProductName());
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
            CString* pstrInfo = new CString(strInfo);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
        }
        else
        {
            strInfo.Format((_T("Open Com %d failure.")), nComPort);//prompt info;

            CString* pstrInfo = new CString(strInfo);
            ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
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
            || nDeviceType == PM_CM5
			|| nDeviceType == STM32_CO2_NET)
    {
        return TRUE;
    }

    return TRUE;
}

void CTStatScanner::WaitScan()
{
    //AfxBeginThread(_WaitScanThread, this);
	hwait_scan_thread =CreateThread(NULL,NULL,_WaitScanThread,this,NULL, NULL);
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
BOOL CTStatScanner::ScanBacnetIPDevice()
{
    m_szComs.clear();
    GetSerialComPortNumber1(m_szComs);

    //if (m_szComs.size() <= 0)
    //{
    //	//AfxMessageBox(_T("Can't scan without any com port installed."));
    //	return FALSE;
    //}
    m_pScanBacnetIPThread = AfxBeginThread(_ScanBacnetMSTPThread,this);
    return TRUE;
}

BOOL CTStatScanner::ScanRemoteIPDevice()
{


    m_pScanRemoteIPThread = AfxBeginThread(_ScanRemote_IP_Thread,this);
    return 1;
}




UINT _ScanRemote_IP_Thread(LPVOID pParam)
{
    CTStatScanner* pScan = (CTStatScanner*)(pParam);

    if((m_scan_info.at(scan_remote_ip_item).scan_skip) || ((m_str_curBuilding_Domain_IP.IsEmpty())))
    {
        if (pScan->m_eScanRemoteIPEnd->m_hObject)
        {
            pScan->m_eScanRemoteIPEnd->SetEvent();
        }

        return 1;
    }
    m_scan_info.at(scan_remote_ip_item).scan_status = SCAN_STATUS_RUNNING;
    CString Remote_IP_Address;
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


    m_bac_scan_result_data.clear();
    m_bac_scan_com_data.clear();


    CString strIP;
    bool is_domain = false;
    strIP = m_str_curBuilding_Domain_IP;

    CStringArray temparray;
    SplitCStringA(temparray,strIP,_T("."));
    if((temparray.GetSize()==4))	//有3个  . 4段
    {
        CString temp_0;
        CString temp_1;
        CString temp_2;
        CString temp_3;
        temp_0 = temparray.GetAt(0);
        temp_1 = temparray.GetAt(1);
        temp_2 = temparray.GetAt(2);
        temp_3 = temparray.GetAt(3);

        if((Is_Dig_Num(temp_0)) && (Is_Dig_Num(temp_1)) && (Is_Dig_Num(temp_2)) && (Is_Dig_Num(temp_3)))
        {
            if(ValidAddress(strIP) == false)
            {
                //MessageBox(_T("Warning!IP address error!"));
                if (pScan->m_eScanRemoteIPEnd->m_hObject)
                {
                    pScan->m_eScanRemoteIPEnd->SetEvent();
                }
                return 0;
            }

        }
        else	//否则判断为 域名;
        {
            is_domain = true;
        }
    }
    else	//判断为 域名;
    {
        is_domain = true;
    }

    CString strInfo;
    strInfo.Format(_T("Try to send command to "));
    strInfo = strInfo + m_str_curBuilding_Domain_IP;
    Show_Scan_Data(strInfo,scan_remote_ip_item);

    if(is_domain)
    {
        CString temp_host_ip;
        if(!GetIPbyHostName(m_str_curBuilding_Domain_IP,temp_host_ip))
        {
            if (pScan->m_eScanRemoteIPEnd->m_hObject)
            {
                pScan->m_eScanRemoteIPEnd->SetEvent();
            }
            //AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
            return 0;
        }
        Remote_IP_Address = temp_host_ip;
    }
    else
    {
        Remote_IP_Address = m_str_curBuilding_Domain_IP;
    }

    int ready_to_read_count = 0;
    for (int j=0; j<5; j++)
    {
        Send_WhoIs_remote_ip(Remote_IP_Address);
        Sleep(2000);
        ready_to_read_count =	m_bac_scan_com_data.size();


        strInfo.Format(_T("Scan  Remote device.Found device"));
        Show_Scan_Data(strInfo,scan_remote_ip_item);
        if(ready_to_read_count == 0)
        {
            strInfo.Format(_T("No remote device response!"));
            Show_Scan_Data(strInfo,scan_remote_ip_item);
            continue;
        }
        //DFTrace(strInfo);
        //pScan->ShowBacnetComScanInfo(strInfo);

        if((int)m_bac_scan_result_data.size()>= ready_to_read_count)	//达到返回的个数后就break;
            break;
        TRACE(_T("gloab scan = %d\r\n"),ready_to_read_count);
        for (int i=0; i<ready_to_read_count; i++)
        {
            int	resend_count = 0;
            do
            {
                resend_count ++;
                if(resend_count>50)
                    break;
                g_invoke_id = GetPrivateData(
                                  m_bac_scan_com_data.at(i).device_id,
                                  GETSERIALNUMBERINFO,
                                  0,
                                  0,
                                  sizeof(Str_Serial_info));

                Sleep(SEND_COMMAND_DELAY_TIME);
            }
            while (g_invoke_id<0);

            bool temp_replay = false;
            for (int i=0; i<3000; i++)
            {
                Sleep(1);
                if(tsm_invoke_id_free(g_invoke_id))
                {
                    temp_replay = true;
                    break;
                }
                else
                    continue;
            }
            if(temp_replay)
                break;
        }
    }
    Sleep(1);
    if(m_bac_scan_result_data.size() == 0)
    {

        Show_Scan_Data(_T("No remote device response."),scan_remote_ip_item);

        //MessageBox(_T("No device found in remote area!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
        if (pScan->m_eScanRemoteIPEnd->m_hObject)
        {
            pScan->m_eScanRemoteIPEnd->SetEvent();
        }
        return 0;
    }

    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    //unsigned int temp_serial_number = pFrame->m_product.at(selected_product_index).serial_number;//以序列号的文件名保存;
    for (int i=0; i<(int)m_bac_scan_result_data.size(); i++)
    {
        //int temp1 = m_FlexGrid.get_Rows();
        CString device_instance_id;
        CString device_ip_address;
        CString device_panel_number;
        CString device_serial_number;
        CString device_name;
        CString priduct_name;
        CString device_modbus_address;
        CString device_type;
        CString device_modbus_port;
        unsigned int temp_serial_number;
        device_modbus_port.Format(_T("%d"),m_bac_scan_result_data.at(i).modbus_port);
        device_modbus_address.Format(_T("%d"),m_bac_scan_result_data.at(i).modbus_addr);
        device_instance_id.Format(_T("%d"),m_bac_scan_result_data.at(i).device_id);
        device_ip_address = m_str_curBuilding_Domain_IP;

        device_panel_number.Format(_T("%d"),m_bac_scan_result_data.at(i).panel_number);
        device_serial_number.Format(_T("%u"),m_bac_scan_result_data.at(i).serialnumber);
        temp_serial_number = m_bac_scan_result_data.at(i).serialnumber;
        device_type.Format(_T("%d"),m_bac_scan_result_data.at(i).product_type);
        device_name = GetProductName(m_bac_scan_result_data.at(i).product_type);
        priduct_name = device_name + _T(":") + device_serial_number;
        bool find_exsit = false;
        int nitems = -1;

        CString strSql;
        strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"),device_serial_number);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
        table = SqliteDBBuilding.getTable((UTF8MBSTR)strSql);
        int count =table.numRows(); //ado.m_pRecordset->GetRecordCount();
      
        if(count >= 1)
            find_exsit = true;

        if(!find_exsit)//没有发现相同的序列号就插入;
        {
            //changed_items = temp1 -1;
            CString m_protocol_temp;
            m_protocol_temp.Format(_T("%u"),P_REMOTE_DEVICE);


            strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Custom) values('"
                             +pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurMainBuildingName+"','"+device_serial_number+"','floor1','room1','"+priduct_name+"','"+device_type+"','"+device_modbus_address+"','""','"+device_ip_address+"','""','"+device_instance_id+"','"+device_panel_number+"','"+device_modbus_port+"','0','"+m_protocol_temp+"','1','0')"));
            //new nc//strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Mainnet_info) values('"+strMainBuildName+"','"+strSubBuildingName +"','"+strSID+"','"+strFloorName+"','"+strRoomName+"','"+strProName+"','"+strProType+"','"+strProID+"','"+strScreenID+"','"+strBaudrate+"','"+strGraphicID+"','"+strHdVersion+"','"+strStVersion+"','"+strCom+"','"+strEPSize+"','"+strMainnetInfo+"')"));
            try
            {

                TRACE(strSql);
                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            }
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }
        }
        else
        {
            CString m_protocol_temp;
            m_protocol_temp.Format(_T("%u"),P_REMOTE_DEVICE);
            CString strSql;
            try
            {
                strSql.Format(_T("update ALL_NODE set MainBuilding_Name = '%s' , Building_Name = '%s' , Protocol = '6' , Product_name ='%s' , Product_class_ID = '%s' , Product_ID = '%s' , Bautrate = '%s' , Hardware_Ver = '%s' , Software_Ver = '%s' ,Online_Status = '1'  where Serial_ID = '%s'"),pFrame->m_strCurMainBuildingName,pFrame->m_strCurMainBuildingName,priduct_name,device_type,device_modbus_address,device_ip_address,device_instance_id,device_panel_number,device_serial_number);
                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            }
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }

        }

        CString nmessage;
        nmessage.Format(_T("Find SerialNumber is %s ,and Product is %s "),device_serial_number,priduct_name);
        //MessageBox(nmessage,_T("Notice"),MB_OK | MB_ICONINFORMATION);

        int	resend_count = 0;
        do
        {
            resend_count ++;
            if(resend_count>50)
                break;
            g_invoke_id = GetPrivateData(
                              m_bac_scan_result_data.at(i).device_id,
                              READ_REMOTE_DEVICE_DB,
                              0,
                              0,
                              sizeof(Str_Remote_TstDB));

            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        while (g_invoke_id<0);

    }
    Sleep(500);


    for (int i=0; i<(int)m_remote_device_db.size(); i++)
    {

        CString device_instance_id;
        CString device_ip_address;
        CString device_panel_number;
        CString device_serial_number;
        CString device_name;
        CString priduct_name;
        CString device_modbus_address;
        CString device_type;
        CString device_port;
        device_modbus_address.Format(_T("%d"),m_remote_device_db.at(i).modbus_id);
        device_ip_address = Remote_IP_Address;

        device_serial_number.Format(_T("%d"),m_remote_device_db.at(i).sn);
        device_type.Format(_T("%d"),m_remote_device_db.at(i).product_type);
        device_name = GetProductName(m_remote_device_db.at(i).product_type);
        priduct_name = device_name + _T(":") + device_serial_number + _T("--") + device_modbus_address;
        device_port.Format(_T("%d"), m_remote_device_db.at(i).port);
        bool find_exsit = false;

        CString strSql;
        strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"),device_serial_number);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
		table = SqliteDBBuilding.getTable((UTF8MBSTR)strSql);
        int count =table.numRows(); //ado.m_pRecordset->GetRecordCount();
        
        if(count >= 1)
            find_exsit = true;


        if(!find_exsit)//没有发现相同的序列号就插入;
        {
            CString m_protocol_temp;
            m_protocol_temp.Format(_T("%u"),P_REMOTE_DEVICE);
            CString strSql;
            strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Custom) values('"
                             +pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurMainBuildingName+"','"+device_serial_number+"','floor1','room1','"+priduct_name+"','"+device_type+"','"+device_modbus_address+"','""','"+device_ip_address+"','""','""','""','"+device_port+"','0','"+m_protocol_temp+"','1','0')"));
            //new nc//strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Mainnet_info) values('"+strMainBuildName+"','"+strSubBuildingName +"','"+strSID+"','"+strFloorName+"','"+strRoomName+"','"+strProName+"','"+strProType+"','"+strProID+"','"+strScreenID+"','"+strBaudrate+"','"+strGraphicID+"','"+strHdVersion+"','"+strStVersion+"','"+strCom+"','"+strEPSize+"','"+strMainnetInfo+"')"));
            try
            {

                TRACE(strSql);
                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            }
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }
        }
        else
        {
            CString m_protocol_temp;
            m_protocol_temp.Format(_T("%u"),P_REMOTE_DEVICE);
            CString strSql;
            try
            {
                strSql.Format(_T("update ALL_NODE set MainBuilding_Name = '%s' , Building_Name = '%s' , Protocol = '%s' , Product_name ='%s' , Product_class_ID = '%s' , Product_ID = '%s' , Bautrate = '%s' , Hardware_Ver = '%s' , Software_Ver = '%s' , Com_Port = '%s' ,Online_Status = '1'  where Serial_ID = '%s'"),pFrame->m_strCurMainBuildingName,pFrame->m_strCurMainBuildingName,m_protocol_temp,priduct_name,device_type,device_modbus_address,device_ip_address,device_instance_id,device_panel_number,device_port,device_serial_number);
               SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            }
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }

        }


    }
    SqliteDBBuilding.closedb();
    m_scan_info.at(scan_remote_ip_item).scan_status = SCAN_STATUS_FINISHED;




    if (pScan->m_eScanRemoteIPEnd->m_hObject)
    {
        pScan->m_eScanRemoteIPEnd->SetEvent();
    }

    return 1;
}

//需要先让串口的modbus 扫完，那里会记录有哪些串口存在 bacnet的协议.
//在扫描bacnet的时候 将bacnet ip 扫描完后，要去依次扫描 串口的MS/TP
//Scan bacnet
UINT _ScanBacnetMSTPThread(LPVOID pParam)
{
    CTStatScanner* pScan = (CTStatScanner*)(pParam);


    //if (pScan->m_eScanBacnetIpEnd->m_hObject)
    //{
    //	pScan->m_eScanBacnetIpEnd->SetEvent();
    //}
    //is_in_scan_mode = false;
    //return 1;
    if(m_scan_info.at(scan_bacnet_ip_item).scan_skip)
    {
        if (pScan->m_eScanBacnetIpEnd->m_hObject)
        {
            pScan->m_eScanBacnetIpEnd->SetEvent();
        }
        //is_in_scan_mode = false;
        return 1;
    }

    m_bac_scan_com_data.clear();	//清空上次扫描的遗留数据;
    m_bac_scan_result_data.clear();

    //scaning_mode = true;
    m_scan_info.at(scan_bacnet_ip_item).scan_status = SCAN_STATUS_RUNNING;


	//等串口的modbus 的 完全释放了之后在开始 扫描 bacnet 的mstp ， 不然的话 其他地方的close_COM 容易出现 异常报错;
	 if(WaitForSingleObject(pScan->m_eScanComEnd->m_hObject, INFINITE) == WAIT_OBJECT_0)
	 {

	 }
	   pScan->m_eScanComEnd->SetEvent();
#if 1 // bacnet mstp
    //for (int i=0;i<pScan->m_bacnetComs.size();i++)
    //{

    CString temp_cs;
    int temp_port = current_building_comport;
    Initial_bac(temp_port);
    TRACE(_T("Now scan with COM%d\r\n"),temp_port);

    for (int i=0; i<100; i++)
    {
        CString strInfo;
        strInfo.Format(_T("Send MSTP command time left(%d)"),100-i);
        memset(m_scan_info.at(scan_bacnet_ip_item).scan_notes,0,250);
        char temp_char[250];
        WideCharToMultiByte( CP_ACP, 0, strInfo.GetBuffer(), -1, temp_char, 250, NULL, NULL );
        memcpy(m_scan_info.at(scan_bacnet_ip_item).scan_notes,temp_char,250);

        //pScan->ShowBacnetComScanInfo(strInfo);
        Send_WhoIs_Global(-1,-1);
        Sleep(1000);
        if((m_bac_scan_com_data.size() > 0) && (i > 20))
            break;
    }




    for (int j=0; j<5; j++)
    {
        int ready_to_read_count =	m_bac_scan_com_data.size();
        CString strInfo1;
        CString strInfo;


        strInfo1.Format(_T("Scan  Bacnet mstp.Found %d BACNET device relpy who is"),ready_to_read_count);
        memset(m_scan_info.at(scan_bacnet_ip_item).scan_notes,0,250);
        char temp_char[250];
        WideCharToMultiByte( CP_ACP, 0, strInfo1.GetBuffer(), -1, temp_char, 250, NULL, NULL );
        memcpy(m_scan_info.at(scan_bacnet_ip_item).scan_notes,temp_char,250);


        if((int)m_bac_scan_result_data.size()>= ready_to_read_count)	//达到返回的个数后就break;
            break;
        TRACE(_T("gloab scan = %d\r\n"),ready_to_read_count);
        for (int i=0; i<ready_to_read_count; i++)
        {
            int	resend_count = 0;
            do
            {
                resend_count ++;
                if(resend_count>50)
                    break;
                g_invoke_id = GetPrivateData(
                                  m_bac_scan_com_data.at(i).device_id,
                                  GETSERIALNUMBERINFO,
                                  0,
                                  0,
                                  sizeof(Str_Serial_info));

                Sleep(SEND_COMMAND_DELAY_TIME);
            }
            while (g_invoke_id<0);
            Sleep(2000);
            //} while (!tsm_invoke_id_free(g_invoke_id));

        }
    }


    //}
#endif
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


    int ret_3 = 0;
    if(m_bac_scan_com_data.size()==0)
    {
        AfxMessageBox(_T("No device reply who is!"));
        goto end_mstp_thread;
    }
    else
    {
        ret_3 =	m_bac_scan_result_data.size();
        if(ret_3 > 0)
        {
            //AfxMessageBox(_T("Receive 99"));
        }
        else
        {
            goto end_mstp_thread;
            //AfxMessageBox(_T("No device reply 99 command!"));
        }
    }

    //m_scan_info.at(scan_bacnet_ip_item).scan_found = ret_3;
    //CString bac_strInfo;
    //bac_strInfo.Format(_T("Scan  Bacnetip.Found %d recognizable Bacnet device"),ret_3);
    //pScan->ShowBacnetComScanInfo(bac_strInfo);


    //memset(m_scan_info.at(scan_bacnet_ip_item).scan_notes,0,250);
    //char temp_char[250];
    //WideCharToMultiByte( CP_ACP, 0, bac_strInfo.GetBuffer(), -1, temp_char, 250, NULL, NULL );
    //memcpy(m_scan_info.at(scan_bacnet_ip_item).scan_notes,temp_char,250);

    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    //TRACE(_T("serial scan = %d\r\n"),ret_3);
    for (int l=0; l<ret_3; l++)
    {
        CString temp_serial_number;
        temp_serial_number.Format(_T("%u"),m_bac_scan_result_data.at(l).serialnumber);
        CString str_temp;
        str_temp.Format(_T("select * from ALL_NODE where Serial_ID = '%s'"),temp_serial_number);

        CString temp_baud;
        temp_baud.Format(_T("%u"),current_building_baudrate);


		q = SqliteDBBuilding.execQuery((UTF8MBSTR)str_temp);
		table = SqliteDBBuilding.getTable((UTF8MBSTR)str_temp);
        int count =table.numRows();
         

        CString temp_pname;
        temp_pname = GetProductName(m_bac_scan_result_data.at(l).product_type);
        CString temp_modbusid;
        temp_modbusid.Format(_T("%u"),m_bac_scan_result_data.at(l).modbus_addr);
        CString temp_view_name;
        temp_view_name = temp_pname + _T(":") + temp_serial_number + _T("-") + temp_modbusid;
        CString temp_protocol;
        temp_protocol.Format(_T("%d"),P_BACNET_MSTP);

        CString temp_product_id_string;
        temp_product_id_string.Format(_T("%d"),m_bac_scan_result_data.at(l).product_type);

        CString temp_port_string;
        temp_port_string.Format(_T("%d"),current_building_comport);

        CString temp_object_instance;
        CString temp_panel_number;
        temp_object_instance.Format(_T("%u"),m_bac_scan_result_data.at(l).device_id);
        temp_panel_number.Format(_T("%u"),m_bac_scan_result_data.at(l).panel_number);

        if(count>= 1)
        {
            str_temp.Format(_T("update ALL_NODE set Bautrate ='%s',Com_Port ='%s',Product_ID ='%s', Protocol ='%s',Product_name = '%s',Online_Status = 1,Hardware_Ver = '%s',Software_Ver = ' %s' where Serial_ID = '%s'"),temp_baud,temp_port_string,temp_product_id_string,temp_protocol,
                            temp_view_name,temp_object_instance,temp_panel_number,temp_serial_number);
        }
        else
        {
            str_temp.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Custom)   values('"
                               +pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurSubBuldingName+"','""','"+temp_serial_number+"','floor1','room1','"+temp_view_name+"','"+temp_product_id_string+"','"+temp_modbusid+"','""','"+temp_baud+"','Default_Building_PIC.bmp','"+temp_object_instance+"','"+temp_panel_number+"','"+temp_port_string+"','0','"+temp_protocol+"','1','0')"));

        }
         SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
    }

#if 0
    for (int l=0; l<ret_3; l++)
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
    /*bac_strInfo.Format(_T("Scan finished"));
    memset(m_scan_info.at(scan_bacnet_ip_item).scan_notes,0,250);
    memset(temp_char,0,250);
    WideCharToMultiByte( CP_ACP, 0, bac_strInfo.GetBuffer(), -1, temp_char, 250, NULL, NULL );
    memcpy(m_scan_info.at(scan_bacnet_ip_item).scan_notes,temp_char,250);*/

 

end_mstp_thread:
    if (pScan->m_eScanBacnetIpEnd->m_hObject)
    {
        pScan->m_eScanBacnetIpEnd->SetEvent();
    }
    //m_scan_info.at(scan_bacnet_ip_item).scan_status = SCAN_STATUS_FINISHED;
    //pScan->m_bStopScan = TRUE;
    HANDLE temphandle;
    temphandle = Get_RS485_Handle();
    if(temphandle !=NULL)
    {
        TerminateThread((HANDLE)Get_Thread1(),0);
        TerminateThread((HANDLE)Get_Thread2(),0);

        CloseHandle(temphandle);
        Set_RS485_Handle(NULL);
    }

    //is_in_scan_mode = false;
    return 1;
}

void Show_Scan_Data(LPCTSTR nstrInfo ,unsigned int nitem)
{
    if((nitem < 0) || (nitem >= m_scan_info.size()))
        return;
    if(_tcslen(nstrInfo) == 0)
        return;
    CString strInfo;
    strInfo = nstrInfo;
    memset(m_scan_info.at(scan_remote_ip_item).scan_notes,0,250);
    char temp_char[250];
    WideCharToMultiByte( CP_ACP, 0, strInfo.GetBuffer(), -1, temp_char, 250, NULL, NULL );
    memcpy(m_scan_info.at(nitem).scan_notes,temp_char,250);
    //strInfo.Format(_T("Try to send command to "));
    //strInfo = strInfo + m_str_curBuilding_Domain_IP;
    //memset(m_scan_info.at(scan_remote_ip_item).scan_notes,0,250);
    //char temp_char[250];
    //WideCharToMultiByte( CP_ACP, 0, strInfo.GetBuffer(), -1, temp_char, 250, NULL, NULL );
    //memcpy(m_scan_info.at(scan_remote_ip_item).scan_notes,temp_char,250);
}
