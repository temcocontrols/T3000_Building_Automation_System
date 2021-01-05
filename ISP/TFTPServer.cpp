#include "StdAfx.h"

#include "Resource.h"
#include "TFTPServer.h"
#include "ISPDlg.h"
#include "Iphlpapi.h"
#include "MySocket.h"
#include "global_function.h"
CString local_enthernet_ip;
 Bin_Info        global_fileInfor;
////#include <vector>			//Vector templates
////using  std::vector;			//Namespaces
//#include "define.h"
#pragma  comment(lib,"Iphlpapi.lib")
static bool need_show_device_ip = true;
UINT _StartSeverFunc(LPVOID pParam);
extern bool auto_flash_mode;	//Used for automatic burning;
char sendbuf[45];
extern CString g_strFlashInfo;
extern unsigned int Remote_timeout;
extern unsigned int nflash_receive_to_send_delay_time;
extern int c2_need_update_boot;  //0 不用更新boot   1 需要更新bootload;   C2为bootload  同时要更新就必须更新;
extern CString g_repair_bootloader_file_path;
extern int new_bootload ; //如果等于1 就说明现在烧写的是新的BootLoader;
/*extern*/ CRITICAL_SECTION g_cs;
/*extern*/ CString showing_text;
/*extern*/ int writing_row;
extern int firmware_must_use_new_bootloader ;  //0 不用更新boot   1 需要更新bootload;   C1为hex
const int TFTP_PORT = 69;

const int nLocalDhcp_Port = 67;			// Server Local 67
const int nSendDhcp_Port = 68;			// client Remote 68
CString Failed_Message;

typedef struct _Product_IP_ID
{
    CString ISP_Device_IP;
    //unsigned char Byte_ISP_Device_IP[4];
    unsigned char ID;
} Product_IP_ID;


int ISP_STEP;
//vector <Product_IP_ID> Product_Info;
BYTE Byte_ISP_Device_IP[4];

BYTE Product_Name[12];
BYTE Rev[4];
bool device_has_replay_lan_IP=false;
volatile int package_number=1;
volatile int next_package_number=1;
bool device_jump_from_runtime=false;
bool dhcp_package_is_broadcast=false;
//bool some_device_reply_the_broadcast=false;
//int broadcast_flash_count=0;
//int now_flash_count=0;


bool has_enter_dhcp_has_lanip_block=false;

TFTPServer::TFTPServer(void)
{
    m_szDataBuf = NULL;
    m_nBlkNum = 0;
    m_sock = NULL;
    m_soRecv = NULL;
    m_soSend = NULL;
    m_tcp_connect_results = 0;
    WSADATA wsaData;
    int err;
    WORD wVersionRequested;
    wVersionRequested = MAKEWORD( 1,1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    //if ( err != 0 )
    //{
    //	return TRUE;
    //}
    //some_device_reply_the_broadcast=false;
    device_jump_from_runtime = false;
    dhcp_package_is_broadcast=false;
}

TFTPServer::~TFTPServer(void)
{
    Sleep(1000);
    ReleaseAll();
}

void TFTPServer::SetParentDlg(CDialog* pDlg)
{
    m_pDlg = (CISPDlg *)pDlg;
}

void TFTPServer::SetClientIP(DWORD dwIP)
{
    m_dwClientIP = dwIP;
}

void TFTPServer::SetClientPort(int nPort)
{
    m_nClientPort = nPort;
}



void TFTPServer::SetFileName(const CString& strFileName)
{
    m_strFileName= strFileName;
}
void TFTPServer::Set_FileProductName(CString Name)
{
    m_StrFileProductName=Name;
}

void TFTPServer::SetDataSource(BYTE* pBuf, int nLen)
{
    m_szDataBuf = pBuf;
    m_nDataBufLen = nLen;
}



int TFTPServer::InitSocket()
{
    m_sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (m_sock == INVALID_SOCKET)
    {
        return FALSE;
    }
    //-----------------------------------------------------------------------------------
    // Bind the socket to any address and the specified port.
    m_siServer.sin_family = AF_INET;
    m_siServer.sin_port = htons(TFTP_PORT);

    m_siServer.sin_addr.s_addr = htonl(INADDR_ANY);

    int nRet = bind(m_sock, (SOCKADDR *) &m_siServer, sizeof(m_siServer));
    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }

    //////////////////////////////////////////////////////////////////////////
    m_soSend=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (m_sock == INVALID_SOCKET)
    {
        return FALSE;
    }

    return TRUE;
}

// Broadcast notification 
const int UDP_BROADCAST_PORT = 1234;
void TFTPServer::BroadCastToClient()
{
    ASSERT(m_sock);

    IP_ADAPTER_INFO pAdapterInfo;
    ULONG len = sizeof(pAdapterInfo);
    if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS)
    {
        return;
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
    SOCKET hBroad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    BOOL bBroadcast=TRUE;
    ::setsockopt(hBroad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
    int iMode=1;
    ioctlsocket(hBroad,FIONBIO, (u_long FAR*) &iMode);

    SOCKADDR_IN bcast;
    bcast.sin_family=AF_INET;
    bcast.sin_addr.s_addr=nBroadCastIP;
    bcast.sin_port=htons(UDP_BROADCAST_PORT);
    short nmsgType=0;
    int nRet = ::sendto(hBroad,(char*)&nmsgType,sizeof(short),0,(sockaddr*)&bcast,sizeof(bcast));
}


//  Accept Request
int TFTPServer::RecvRequest()
{
    BYTE szBuf[512];
    ZeroMemory(szBuf, 512);

    SOCKADDR_IN  siRecvRead; // Receive read request，Used to bind
    siRecvRead.sin_family = AF_INET;
    siRecvRead.sin_port = htons(TFTP_PORT);
    siRecvRead.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(_T("192.168.0.3"))
    int nRetB = bind(m_sock, (SOCKADDR *) &siRecvRead, sizeof(siRecvRead));

// 	m_siClient.sin_family = AF_INET;
// 	m_siClient.sin_port = htons(TFTP_PORT);
// 	m_siClient.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(_T("192.168.0.3"))

    int nLen = sizeof(m_siClient);
    //while(1)
    {
        int nRet = ::recvfrom(m_sock,(char*)szBuf,512,0,(sockaddr*)&m_siClient, &nLen);
        //TRACE("RECV by UDP - Num = %d \n", nRet);
        if (nRet == SOCKET_ERROR )
        {
            //out put error message
            return FALSE;
        }
        CString strInfo;
        strInfo.Format(_T("Recv Request Byte Num = %d\n"), nRet);

        // 		if (nRet == 2)
        // 		{
        // 			break;
        // 		}
        HandleWRRequest(szBuf,  nRet);
        return TRUE;
    }

    return TRUE;
}


int TFTPServer::SendProcess()
{
    fd_set          readfds;
    timeval sTimeout;
    sTimeout.tv_sec = 1;
    sTimeout.tv_usec = 500;
    //sTimeout.tv_usec = 0;
    FD_ZERO (& readfds);
    //FD_SET (m_sock, &readfds);
    FD_SET (m_soSend, &readfds);

    //TFTP_DATA_PACK tdp;

    int nCount = 0;
    int nSendNum = m_nDataBufLen;
    BYTE pBuf[512];

    CString strTips;
    strTips = _T("Beginning Programming.");
    OutPutsStatusInfo(strTips, FALSE);

    while(nCount < m_nDataBufLen)
    {
        ZeroMemory(pBuf, 512);
        nSendNum = m_nDataBufLen - nCount;
        nSendNum = nSendNum > 512 ?  512 : nSendNum;
        memcpy(pBuf, m_szDataBuf+nCount, nSendNum);
        m_nBlkNum++;
        //Sleep(75);
        int nRet = SendData(pBuf, nSendNum);
        nCount+= 512;

        CString strTips;
        strTips.Format(_T("Programming finished %d bytes."), nCount);
        OutPutsStatusInfo(strTips, TRUE);

        if (nRet == SOCKET_ERROR)
        {
            return 0;
        }

        int nResend = 5;
        while(nResend > 0)
        {
            nResend--;
            fd_set tempfds = readfds;
            int nSRet = select(1, &tempfds, NULL, NULL, &sTimeout);
            if (nSRet == 0)
            {
                nRet = SendData(pBuf, nSendNum);
                Sleep(500);
                if (nRet == SOCKET_ERROR)
                {
                    return 0;
                }
            }
            else
            {
                if(RecvACK())
                {
                    break;
                }

            }
        }
        if (nResend <= 0)
        {
            // 5 times, over.
            return 0;
        }
    }
    //SendEndDataPack();
    return 1;
}


// Send data
int TFTPServer::SendData(BYTE* szBuf, int nLen)
{
    TFTP_DATA_PACK tdp;
    int nCount = m_nDataBufLen;

    ASSERT(nLen <= 512)	;
    tdp.m_wOPCode = 3;
    tdp.m_wOPCode <<= 8;
    tdp.m_wBlkNum = htons(m_nBlkNum);
    memcpy(tdp.m_szData, szBuf, nLen);

    // 	int nSendPort =3763;
    // 	m_siSend.sin_family = AF_INET;
    // 	m_siSend.sin_port = m_siClient.sin_port;
    // 	m_siSend.sin_addr = m_siClient.sin_addr;
    int nAddrLen = sizeof(m_siClient);
    int nRet = 0;
    if (nLen == 512)
    {
        nRet = sendto(m_soSend,(char*)(&tdp), sizeof(tdp),0, (sockaddr*)&m_siClient, nAddrLen);
    }
    else  // Last package, do not send useless data
    {
        nRet = sendto(m_soSend,(char*)(&tdp), nLen+4, 0, (sockaddr*)&m_siClient, nAddrLen);
    }


    if (nRet == SOCKET_ERROR)
    {
        return 0;
    }

    CString strInfo;
    //strInfo.Format(_T("Send Data Byte Num = %d ********"), nRet);
    //strInfo.Format(_T("Send Block Num = %d\n"), tdp.m_wBlkNum);


    return nRet;
}


// Send data
int TFTPServer::SendDataNew(BYTE* szBuf, int nLen)
{
    TFTP_DATA_PACK tdp;
    int nCount = m_nDataBufLen;

    ASSERT(nLen <= 512)	;
    tdp.m_wOPCode = 3;
    tdp.m_wOPCode <<= 8;
    //if(m_nBlkNum==0x80)
    //{
    //	Sleep(1);
    //}
    tdp.m_wBlkNum = htons(m_nBlkNum);
    memcpy(tdp.m_szData, szBuf, nLen);
    int nAddrLen = sizeof(m_siClient);
    int nRet = 0;
    SendUDP_Flash_Socket.SendTo((char*)(&tdp), nLen+4,FLASH_UDP_PORT,ISP_Device_IP,0);
    return nRet;
}




// Send data
//int TFTPServer::SendData(BYTE* szBuf, int nLen)
//{
//	TFTP_DATA_PACK tdp;
//	int nCount = m_nDataBufLen;
//
//	ASSERT(nLen <= 512)	;
//	tdp.m_wOPCode = 3;
//	tdp.m_wOPCode <<= 8;
//	tdp.m_wBlkNum = htons(m_nBlkNum);
//	memcpy(tdp.m_szData, szBuf, nLen);
//
//// 	int nSendPort =3763;
//// 	m_siSend.sin_family = AF_INET;
//// 	m_siSend.sin_port = m_siClient.sin_port;
//// 	m_siSend.sin_addr = m_siClient.sin_addr;
//	int nAddrLen = sizeof(m_siClient);
//	int nRet = 0;
//	if (nLen == 512)
//	{
//		nRet = sendto(m_soSend,(char*)(&tdp), sizeof(tdp),0, (sockaddr*)&m_siClient, nAddrLen);
//	}
//	else  // Last package, do not send useless data
//	{
//		nRet = sendto(m_soSend,(char*)(&tdp), nLen+4, 0, (sockaddr*)&m_siClient, nAddrLen);
//	}
//
//
//	if (nRet == SOCKET_ERROR)
//	{
//		return 0;
//	}
//
//	CString strInfo;
//	//strInfo.Format(_T("Send Data Byte Num = %d ********"), nRet);
//	//strInfo.Format(_T("Send Block Num = %d\n"), tdp.m_wBlkNum);
//
//
//	return nRet;
//}

const BYTE c_byEndFlag[2] = {0xee, 0x10};
int TFTPServer::SendEndDataPack()
{
    BYTE pBuf[512];
    ZeroMemory(pBuf, 512);

    memcpy(pBuf, c_byEndFlag, 2);

    int nRet = SendData(pBuf, 512);

    Sleep(3000);

    closesocket(m_sock);

    return nRet;
}

// Accept ACK
int TFTPServer::RecvACK()
{
    TFTP_ACK ta;

    //sockaddr_in  siRecv;
    int nLen = sizeof(m_siClient);
    int nRet = ::recvfrom(m_soSend, (char*)&ta, sizeof(ta), 0, (sockaddr*)&m_siClient, &nLen);
    //TRACE("RECV ACK - Num = %d \n", nRet);
    if (nRet == SOCKET_ERROR )
    {
        return FALSE;
    }

    ta.m_wBlkNum = htons(ta.m_wBlkNum);
    if (ta.m_wBlkNum != m_nBlkNum)
    {
        return FALSE;
    }
    CString strInfo;
    strInfo.Format(_T("Recv ACK Byte Num = %d\n"), nRet);

    return TRUE;
}

void TFTPServer::ReleaseAll()
{
    if (m_sock)
    {
        closesocket(m_sock);					// Receive tftp request socket,  Bind to port 69
    }
    if (m_soSend)
    {
        closesocket(m_soSend);				//  Send socket，Send tftp data
    }

    if (m_soRecv)
    {
        closesocket(m_soRecv);				// Receive socket，Receive tftp ack
    }
    SendUDP_Flash_Socket.ShutDown();
    SendUDP_Flash_Socket.Close();
    UDP_Broadcast_Socket.ShutDown();
    UDP_Broadcast_Socket.Close();
    TCP_Flash_CMD_Socket.ShutDown();
    TCP_Flash_CMD_Socket.Close();

    for(UINT i = 0; i < m_twr.m_szItems.size(); i++)
    {
        if( m_twr.m_szItems[i]!=NULL)
            delete m_twr.m_szItems[i];
    }

}

//Fance 2013 0504
//Compare whether the device ip is in local net .
bool TFTPServer::IP_is_Local()
{

    DWORD dwIP = GetLocalIP();
    if(dwIP == 0)
        return 1;
    BYTE byIP[4];
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byIP[i] = (dwIP >> ic*8)&0x000000FF;
    }

    BYTE byISPDeviceIP[4];
    DWORD dwClientIP = m_dwClientIP;
    ZeroMemory(byISPDeviceIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byISPDeviceIP[i] = (dwClientIP >> ic*8)&0x000000FF;
    }
    if(memcmp(byIP,byISPDeviceIP,3)==0)
    {
        return true;
    }

    //memcpy_s(byIP,3,byISPDeviceIP,3)
    return false;
}


//Fance 2013 0504
//Change Device IP to CString Format.
void TFTPServer::GetDeviceIP_String()
{
    BYTE byIP[4];
    DWORD dwClientIP = m_dwClientIP;
    ZeroMemory(byIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {

        byIP[i] = (dwClientIP >> ic*8)&0x000000FF;
    }

    ISP_Device_IP.Format(_T("%d.%d.%d.%d"),byIP[0],byIP[1],byIP[2],byIP[3]);
}


//Fance 2013 0504
//Create DHCP Data to boot loader.
void TFTPServer::SetDHCP_Data()
{
    DWORD dwgateway = 0;
    DWORD dwSubnetMask=0;
    DWORD dwClientIP=0;

    //DHCP_PACKET MYDHCP_PACKET;

    //memcpy_s(MYDHCP_PACKET.Header,sizeof(MYDHCP_PACKET.Header),"Temcocontrols",13);
    memcpy_s(sendbuf,13,"Temcocontrols",13);



    if(IP_is_Local())
    {
        //MYDHCP_PACKET.AssignIp=m_dwClientIP;
        dwClientIP = m_dwClientIP;
    }
    else
    {
        //MYDHCP_PACKET.AssignIp=0;
        dwClientIP = 0;
    }


    BYTE byIP[4];
    ZeroMemory(byIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byIP[i] = (dwClientIP >> ic*8)&0x000000FF;
    }
    memcpy_s(sendbuf+13,4,byIP,4);


    //MYDHCP_PACKET.SubnetMask=0xffffff00;
    BYTE btSubnetMask[4];
    //dwSubnetMask = MYDHCP_PACKET.SubnetMask;
    dwSubnetMask = 0xffffff00;
    ZeroMemory(btSubnetMask,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        btSubnetMask[i] = (dwSubnetMask >> ic*8)&0x000000FF;
    }
    memcpy_s(sendbuf+17,4,btSubnetMask,4);

    //MYDHCP_PACKET.GatewayIp=0;
    BYTE	Gateway[4];
    ZeroMemory(Gateway,4);
    memcpy_s(sendbuf+21,4,Gateway,4);

    if(device_has_replay_lan_IP == true)
        memcpy_s(sendbuf+25,4,Byte_ISP_Device_IP,4);	//ISP_Device_IP is the device ip address and read in runtime.
    else
    {
        //memset(Byte_ISP_Device_IP,0,4);
        memcpy_s(sendbuf+25,4,Byte_ISP_Device_IP,4);
    }

    char reserved[16];
    ZeroMemory(reserved,16);
    memcpy_s(sendbuf+29,16,reserved,16);
}

struct ALL_LOCAL_SUBNET_NODE{
	CString StrIP;
	CString StrMask;
	CString StrGetway;
	int NetworkCardType;
};

vector<ALL_LOCAL_SUBNET_NODE> g_Vector_Subnet;

void TFTPServer::GetIPMaskGetWay()
{
	g_Vector_Subnet.clear();
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	ULONG ulOutBufLen;
	pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	ALL_LOCAL_SUBNET_NODE  Temp_Node;
	// 第一次调用GetAdapterInfo获取ulOutBufLen大小
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			//             CString Name;//=pAdapter->AdapterName;
			// 			MultiByteToWideChar( CP_ACP, 0, pAdapter->AdapterName, (int)strlen((char *)pAdapter->AdapterName)+1,
			// 				Name.GetBuffer(MAX_PATH), MAX_PATH );
			// 			Name.ReleaseBuffer();

			MultiByteToWideChar( CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1,
				Temp_Node.StrIP.GetBuffer(MAX_PATH), MAX_PATH );
			Temp_Node.StrIP.ReleaseBuffer();
			//StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String);
			MultiByteToWideChar( CP_ACP, 0,pAdapter->IpAddressList.IpMask.String, (int)strlen((char *)pAdapter->IpAddressList.IpMask.String)+1,
				Temp_Node.StrMask.GetBuffer(MAX_PATH), MAX_PATH );
			Temp_Node.StrMask.ReleaseBuffer();

			//StrMask.Format(_T("%s"), pAdapter->IpAddressList.IpMask.String);

			MultiByteToWideChar( CP_ACP, 0,pAdapter->GatewayList.IpAddress.String, (int)strlen((char *)pAdapter->GatewayList.IpAddress.String)+1,
				Temp_Node.StrGetway.GetBuffer(MAX_PATH), MAX_PATH );
			Temp_Node.StrGetway.ReleaseBuffer();

			Temp_Node.NetworkCardType=pAdapter->Type;

			g_Vector_Subnet.push_back(Temp_Node);

			/*StrGetway.Format(_T("%s"), pAdapter->GatewayList.IpAddress.String); */
			pAdapter = pAdapter->Next;
		}
	}
	else
	{

	}
	if(pAdapterInfo !=NULL)	// If not released, memory leaks causing crash;
		free(pAdapterInfo);
}


UINT TFTPServer::RefreshNetWorkDeviceListByUDPFunc()
{
			

	int nRet = 0;

	GetIPMaskGetWay();
	short nmsgType= 100;

	//////////////////////////////////////////////////////////////////////////
	const DWORD END_FLAG = 0x00000000;
	TIMEVAL time;
	time.tv_sec =1;
	time.tv_usec = 0;

	fd_set fdSocket;
	BYTE buffer[512] = {0};

	BYTE pSendBuf[1024];
	for (int index=0; index<g_Vector_Subnet.size(); index++)
	{
		if (g_Vector_Subnet[index].StrIP.Find(_T("0.0."))!=-1)
		{
			continue;
		}
		char local_network_ip[255];
		memset(local_network_ip,0,255);
		SOCKADDR_IN h_siBind;
		SOCKET h_Broad=NULL;
		SOCKADDR_IN h_bcast;
		h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		BOOL bBroadcast=TRUE;
		::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
		int iMode=1;
		ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

		BOOL bDontLinger = FALSE;
		setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );


		ZeroMemory(pSendBuf, 255);
		pSendBuf[0] = 100;
		memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
		int nSendLen = 5;


		local_enthernet_ip=g_Vector_Subnet[index].StrIP;
		WideCharToMultiByte( CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL );
		h_siBind.sin_family=AF_INET;
		h_siBind.sin_addr.s_addr =  inet_addr(local_network_ip);
		//h_siBind.sin_addr.s_addr=INADDR_ANY;
		h_siBind.sin_port= htons(11115);

		h_bcast.sin_family=AF_INET;
		//bcast.sin_addr.s_addr=nBroadCastIP;
		h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
		h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

		// h_siBind.sin_port=AF_INET;
		//   ::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
		if( -1 == bind(h_Broad,(SOCKADDR*)&h_siBind,sizeof(h_siBind)))//Bind the network card address forcibly to Socket
		{
			goto END_REFRESH_SCAN;
		}
		int time_out=0;
		BOOL bTimeOut = FALSE;
		//while(!bTimeOut)//!pScanner->m_bNetScanFinish)  // Timeout ended
		//{
			time_out++;
			if(time_out>1)
				bTimeOut = TRUE;

			FD_ZERO(&fdSocket);
			FD_SET(h_Broad, &fdSocket);

			nRet = ::sendto(h_Broad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast,sizeof(h_bcast));

			//DFTrace(local_enthernet_ip);
			if (nRet == SOCKET_ERROR)
			{
				int  nError = WSAGetLastError();
				goto END_REFRESH_SCAN;
				return 0;
			}
			int nLen = sizeof(h_siBind);
			fd_set fdRead = fdSocket;
			int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
			if (nSelRet == SOCKET_ERROR)
			{
				int nError = WSAGetLastError();
				goto END_REFRESH_SCAN;
				return 0;
			}

			if(nSelRet > 0)
			{
				ZeroMemory(buffer, 512);
				int nRet ;
				int nSelRet;
				do
				{
					nRet = ::recvfrom(h_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);

					BYTE szIPAddr[4] = {0};
					if(nRet > 0)
					{
						FD_ZERO(&fdSocket);
						if(buffer[0]==101)
						{

							nLen=buffer[2]+buffer[3]*256;
							unsigned short dataPackage[32]= {0};
							memcpy(dataPackage,buffer+2,nLen*sizeof(unsigned short));
							szIPAddr[0]= buffer[16];// (BYTE)dataPackage[7];
							szIPAddr[1]= buffer[18];//(BYTE)dataPackage[8];
							szIPAddr[2]= buffer[20];// (BYTE)dataPackage[9];
							szIPAddr[3]= buffer[22];// (BYTE)dataPackage[10];
							CString temp_rec_ip;
							temp_rec_ip.Format(_T("%u.%u.%u.%u"),szIPAddr[0],szIPAddr[1],szIPAddr[2],szIPAddr[3]);
							if(temp_rec_ip.CompareNoCase(ISP_Device_IP) == 0)
							{
								unsigned short real_port = AddNetDeviceForRefreshList(buffer, nRet, h_siBind);
								if(real_port == m_nClientPort)
								{
									closesocket(h_Broad);
									return 1;
								}
								else if((real_port != m_nClientPort) && (real_port != 0))
								{
									CString temp_1234;
									CString strTips;
									temp_1234.Format(_T("%u"),real_port);
									closesocket(h_Broad);
									m_nClientPort = real_port;
									CISPDlg* pFrame=(CISPDlg*)(AfxGetApp()->m_pMainWnd);
									pFrame->GetDlgItem(IDC_EDIT_NCPORT)->SetWindowTextW(temp_1234);

									strTips.Format(_T("The Device Port is %u"),real_port);
									OutPutsStatusInfo(strTips, FALSE);
									return 0;
								}
								Sleep(1);	//Find;
							}
							//ISP_Device_IP
							int n = 1;
							BOOL bFlag=FALSE;
							//////////////////////////////////////////////////////////////////////////
							// Detect duplicate IP’s 
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
#if 0
							//////////////////////////////////////////////////////////////////////////
							if (!bFlag)
							{
								AddNetDeviceForRefreshList(buffer, nRet, h_siBind);

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
							else
							{
								AddNetDeviceForRefreshList(buffer, nRet, h_siBind);
							}
#endif
						}
					}
					else
					{

						break;
					}


					FD_ZERO(&fdSocket);
					FD_SET(h_Broad, &fdSocket);
					nLen = sizeof(h_siBind);
					fdRead = fdSocket;
					nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
				}
				while (nSelRet);

				//int nRet = ::recvfrom(h_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);

			}
			else
			{
				bTimeOut = TRUE;
			}
		//}//end of while

END_REFRESH_SCAN:

		closesocket(h_Broad);
	}
	return 1;
}
struct refresh_net_device
{
	DWORD nSerial;
	int modbusID;
	unsigned short product_id;
	CString ip_address;
	int nport;
	float sw_version;
	float hw_version;
	unsigned int object_instance;
	unsigned char panal_number;
	DWORD parent_serial_number;
	CString NetCard_Address;
	CString show_label_name;
};


typedef union
{
	unsigned char all[400];
	struct 
	{  
		UCHAR command;
		UCHAR command_reserve;
		UCHAR length;
		UCHAR length_reserve;
		UCHAR serial_low;
		UCHAR serial_low_reserve;
		UCHAR serial_low_2;
		UCHAR serial_low_2_reserve;
		UCHAR serial_low_3;
		UCHAR serial_low_3_reserve;
		UCHAR serial_low_4;
		UCHAR serial_low_4_reserve;
		UCHAR product_id;
		UCHAR product_id_reserve;
		UCHAR modbus_id;
		UCHAR modbus_id_reserve;
		UCHAR ip_address_1;
		UCHAR ip_address_1_reserve;
		UCHAR ip_address_2;
		UCHAR ip_address_2_reserve;
		UCHAR ip_address_3;
		UCHAR ip_address_3_reserve;
		UCHAR ip_address_4;
		UCHAR ip_address_4_reserve;
		USHORT modbus_port;
		USHORT sw_version;
		USHORT hw_version;
		unsigned int parent_serial_number;
		UCHAR object_instance_2;
		UCHAR object_instance_1;
		UCHAR station_number;
		char panel_name[20];
		UCHAR object_instance_4;
		UCHAR object_instance_3;
		UCHAR isp_mode;  //Non 0 in ISP mode, 0 in the application code; 60th byte
	}reg;
}Str_UPD_SCAN;
unsigned short TFTPServer::AddNetDeviceForRefreshList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind)
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
	temp_data.reg.isp_mode = *(my_temp_point++);	//isp_mode = 0 The expression is in the application code, not 0 in Bootload. TBD: clarify this comment

	DWORD nSerial=temp_data.reg.serial_low + temp_data.reg.serial_low_2 *256+temp_data.reg.serial_low_3*256*256+temp_data.reg.serial_low_4*256*256*256;
	CString nip_address;
	nip_address.Format(_T("%u.%u.%u.%u"),temp_data.reg.ip_address_1,temp_data.reg.ip_address_2,temp_data.reg.ip_address_3,temp_data.reg.ip_address_4);
	CString nproduct_name = GetProductName(temp_data.reg.product_id);
	if(nproduct_name.IsEmpty())	//Exit if the product number is not defined;
	{
		if (temp_data.reg.product_id<220)
		{
			return temp_data.reg.modbus_port;
		}
	}

	temp.nport = temp_data.reg.modbus_port;
	temp.sw_version = temp_data.reg.sw_version;
	temp.hw_version = temp_data.reg.hw_version;
	temp.ip_address = nip_address;
	temp.product_id = temp_data.reg.product_id;
	temp.modbusID = temp_data.reg.modbus_id;
	temp.nSerial = nSerial;
	temp.NetCard_Address=_T("");

	temp.parent_serial_number = temp_data.reg.parent_serial_number ;

	temp.object_instance = temp_data.reg.object_instance_1 + temp_data.reg.object_instance_2 *256+temp_data.reg.object_instance_3*256*256+temp_data.reg.object_instance_4*256*256*256;
	temp.panal_number = temp_data.reg.station_number;




	//if((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
	//{
	//	g_Print.Format(_T("Serial = %u     ID = %d ,ip = %s  , Product name : %s ,obj = %u ,panel = %u,isp_mode = %d"),nSerial,temp_data.reg.modbus_id,nip_address ,nproduct_name,temp.object_instance,temp.panal_number,temp_data.reg.isp_mode);
	//	DFTrace(g_Print);
	//}

	if(temp_data.reg.isp_mode != 0)
	{
		//记录这个的信息,如果短时间多次出现 就判定在bootload下面，只是偶尔出现一次表示只是恰好开机收到的.
             // Record this information, if a short time several times to determine under the Bootload, only occasionally appear only once to the boot received. TBD: Clarify this comment

#if 0
		IspModeInfo temp_info;
		temp_info.ipaddress[0] = temp_data.reg.ip_address_1;
		temp_info.ipaddress[1] = temp_data.reg.ip_address_2;
		temp_info.ipaddress[2] = temp_data.reg.ip_address_3;
		temp_info.ipaddress[3] = temp_data.reg.ip_address_4;
		temp_info.product_id = temp_data.reg.product_id;
		CTime temp_time_now = CTime::GetCurrentTime();
		temp_info.first_time = temp_time_now.GetTime();

		bool find_ip_pid_match = false;
		int temp_index = 0;
		for (int x=0;x<g_isp_device_info.size();x++)
		{
			int ip_cmp_ret =	memcmp(g_isp_device_info.at(x).ipaddress,temp_info.ipaddress,4); 
			if((temp_info.product_id == g_isp_device_info.at(x).product_id ) &&
				(ip_cmp_ret == 0))
			{
				find_ip_pid_match = true;
				temp_index = x;
			}
		}

		if(find_ip_pid_match)
		{
			if(( temp_info.first_time >  g_isp_device_info.at(temp_index).first_time  + 60 ) && 
				(temp_info.first_time <  g_isp_device_info.at(temp_index).first_time  + 120))
			{
				g_isp_device_info.at(temp_index).first_time = temp_info.first_time;
				need_isp_device = g_isp_device_info.at(temp_index);
				if(temp_data.reg.isp_mode == 2) //T3 controller will return some special bootloader fail mode info 
				{
					isp_mode_error_code = 2;
					::PostMessageW(MainFram_hwd,WM_HADNLE_ISP_MODE_DEVICE,2,NULL);
				}
				else
					::PostMessageW(MainFram_hwd,WM_HADNLE_ISP_MODE_DEVICE,NULL,NULL);
			}
			else
			{
				if(temp_info.first_time > g_isp_device_info.at(temp_index).first_time  + 120)
					g_isp_device_info.at(temp_index).first_time = temp_info.first_time;
			}
		}
		else
		{
			g_isp_device_info.push_back(temp_info);
		}
		//TRACE(_T("ip:%s time:%d g_isp:%d\r\n"),nip_address,temp_info.first_time,g_isp_device_info.at(temp_index).first_time);
#endif
		return	 0;
	}


	return temp_data.reg.modbus_port;
}

CString ShowTFTPMessage;
BOOL TFTPServer::StartServer()
{

	   RefreshNetWorkDeviceListByUDPFunc();
	   TCP_Flash_CMD_Socket.Connect(ISP_Device_IP,m_nClientPort);
	   Sleep(2000);


   //  for (int i = 0; i<m_FlashTimes ; i++)
   // { 
		CString strTips;
// 		strTips.Format(_T("FlashTimes = %d"),i);
// 		OutPutsStatusInfo(strTips, FALSE);

		memset(Product_Name,0,12);
        char Flash_Done[10];
        memcpy_s(Flash_Done,10,"FLASH DONE",10);
        m_nBlkNum = 0;
        int nRet  =0;
		ISP_STEP = 0; 
       
        need_show_device_ip = true;


        int nSendNum = m_nDataBufLen;

        BYTE byCommand[64] = {0};
        byCommand[0] = 0xEE;	// Command, 2 bytes, 0xee10, command to start with flash
        byCommand[1] = 0x10;	//

        BYTE szBuf[512];
        ZeroMemory(szBuf, 512);
        //int nLen = sizeof(m_test_siClient);
		 memset(Byte_ISP_Device_IP,0,4);
        GetDeviceIP_String();
        SetDHCP_Data();
        ISP_STEP = ISP_SEND_FLASH_COMMAND;
        int mode_send_flash_try_time=0;
        bool first_time_send_dhcp_package = true;
        int mode_no_lanip_try_time=0;
        int mode_has_lanip_try_time=0;
        int mode_flash_over_try_time=0;
        total_retry=0;
        has_enter_dhcp_has_lanip_block =false;//Used to confirm the state of 2 threads to avoid isp_step jumps. TBD: explain this better
        int has_wait_device_into_bootloader = false;



        int nNetTimeout = 1000;//Send Time Out!
        int nReceiveNetTimeout = 1500;//Send Time Out!

        bool Use_Old_protocol=false;
		
        while(1)
        {
            if (!ShowTFTPMessage.IsEmpty())
            {
                OutPutsStatusInfo(ShowTFTPMessage, FALSE);
                ShowTFTPMessage.Empty();
                strTips.Format(_T(" "));
                OutPutsStatusInfo(strTips, FALSE);
            }
            switch(ISP_STEP)
            {
            case ISP_SEND_FLASH_COMMAND:
				nRet = 0;
                if((mode_send_flash_try_time++)<20)
                {
					int send_ret=TCP_Flash_CMD_Socket.Send(byCommand,sizeof(byCommand),0);
                    Sleep(50);   //WIFI 的 bootloader 需要此延时 ， 设备的响应时间太慢了.不延时他们的设备  会忽略其他的网络包;
                   // int send_ret=TCP_Flash_CMD_Socket.SendTo(byCommand,sizeof(byCommand),m_nClientPort,ISP_Device_IP,0);
                    TRACE(_T("send_ret = %d\r\n"),send_ret);
                    if(send_ret<0)	//Try TCP connection again if send fails
                    {
						//TRACE(_T("Send EEprom data failed!"));
                        //TCP_Flash_CMD_Socket.Connect(ISP_Device_IP,m_nClientPort);
                    }

                    SetDHCP_Data();

                    //if(IP_is_Local())//If it's local, send it as broadcast DHCP
                    //{
                    dhcp_package_is_broadcast = true;
                    BOOL bBroadcast=TRUE;
                    //::sendto(dhcpSock,(char*)pBuffer, nDhcpLen,0,(sockaddr*)&siBroadCast,sizeof(siBroadCast));
                    SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
                    int send_count = SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,_T("255.255.255.255"),0);
                    if(send_count <= 0)
                    {
                        if(!auto_flash_mode)
                            AfxMessageBox(_T("Send command failed!"));
                    }
                    //}
                    //else
                    Sleep(50);
                    SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,ISP_Device_IP,0);



                    strTips.Format(_T("Communication with device.(Remain time:%d)"),20-mode_send_flash_try_time);
                    if(mode_send_flash_try_time == 1)
                        OutPutsStatusInfo(strTips, false);
                    else
                        OutPutsStatusInfo(strTips, true);
                }
                else
                {
                    //nRet=StartServer_Old_Protocol();
                    //if(nRet==0)
                    //{
                    strTips = _T("No Connection! Please check the network connection.");
                    OutPutsStatusInfo(strTips, FALSE);
                    //}


                    goto StopServer;
                    //if(some_device_reply_the_broadcast == true)	//This is used to support multiple broadcasts in bootload mode, and if so print the responses
                    //{
                    //	CString temp_pro_cs;
                    //	temp_pro_cs.Format(_T("Find %d device in ISP mode"),Product_Info.size());
                    //	OutPutsStatusInfo(temp_pro_cs, FALSE);
                    //	for (int i=0;i<((int)Product_Info.size());i++)
                    //	{
                    //		temp_pro_cs.Format(_T("No.%d -----> IP ="),i+1);
                    //		temp_pro_cs = temp_pro_cs + Product_Info.at(i).ISP_Device_IP;
                    //		OutPutsStatusInfo(temp_pro_cs, FALSE);
                    //	}
                    //	ISP_STEP =ISP_SEND_DHCP_COMMAND_HAS_LANIP;
                    //	broadcast_flash_count = Product_Info.size();
                    //	continue;
                    //}

                }
                break;
            case ISP_SEND_DHCP_COMMAND_HAS_LANIP:
                has_enter_dhcp_has_lanip_block =true;
                if(need_show_device_ip)
                {
                    need_show_device_ip = false;
                    strTips.Format(_T("Device IP : %d.%d.%d.%d ."),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
                    OutPutsStatusInfo(strTips, FALSE);
                    OutPutsStatusInfo(_T(""), FALSE);
                    int b_same_subnet_ip = false;
                    CString ipaddr;
                    ipaddr.Format(_T("%d.%d.%d"), Byte_ISP_Device_IP[0], Byte_ISP_Device_IP[1], Byte_ISP_Device_IP[2]);
                    CString PC_IP;
                    for (int i = 0; i < g_Vector_Subnet.size(); i++)
                    {
                        CString temp_ip;
                        temp_ip = g_Vector_Subnet.at(i).StrIP;
                        if (temp_ip.CompareNoCase(_T("0.0.0.0") ) == 0)
                            continue;
                        PC_IP = PC_IP + temp_ip + _T("  ");
                        CStringArray temp_ip_array;
                        SplitCStringA(temp_ip_array, temp_ip, _T("."));
                        CString cs_ret;
                        cs_ret.Format(_T("%s.%s.%s"), temp_ip_array.GetAt(0), temp_ip_array.GetAt(1), temp_ip_array.GetAt(2));
                        if (cs_ret.CompareNoCase(ipaddr) == 0)
                        {
                            b_same_subnet_ip = true;
                            break;
                        }
                    }

                    if (b_same_subnet_ip == false)
                    {
                        strTips.Format(_T("PC IP is "));
                        strTips = strTips + PC_IP ;
                        OutPutsStatusInfo(strTips, FALSE);
                        OutPutsStatusInfo(_T("Please Change your PC's IP address to the same subnet."), FALSE);
                        OutPutsStatusInfo(_T(""), FALSE);
                    }

                }

                if((device_jump_from_runtime == true)&&(has_wait_device_into_bootloader == false))
                {
                    has_wait_device_into_bootloader = true;
                    OutPutsStatusInfo(_T(""), FALSE);
                    for (int i=0; i<7; i++)
                    {
                        strTips.Format(_T(" Setting device to firmware update mode...(%ds)"),7-i);
                        OutPutsStatusInfo(strTips, TRUE);
                        Sleep(1000);
                    }
                    OutPutsStatusInfo(_T(""), FALSE);
                }
#if 1
				if(dhcp_package_is_broadcast == true)//If it's a broadcast, change the device IP TBD: explain this better 
				{
					ISP_Device_IP.Empty();
					ISP_Device_IP.Format(_T("%d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);

					CString Temco_logo;
					MultiByteToWideChar( CP_ACP, 0, (char *)Product_Name, 
						(int)strlen((char *)Product_Name)+1, 
						m_StrProductName.GetBuffer(MAX_PATH), MAX_PATH );
					m_StrProductName.ReleaseBuffer();		

					if(m_StrProductName.GetLength() > 11)
						m_StrProductName = m_StrProductName.Left(10);

					//m_StrProductName.Format(_T("%C%C%C%C%C%C%C%C%C%C"),Product_Name[0],Product_Name[1],Product_Name[2],Product_Name[3],Product_Name[4]
					//,Product_Name[5],Product_Name[6],Product_Name[7],Product_Name[8],Product_Name[9]
					//);
					m_StrProductName.Trim();
					if(!m_StrProductName.IsEmpty())
					{
						m_StrProductName.TrimLeft();
						m_StrProductName.TrimRight();
						m_StrRev.Format(_T("%C%C%C%C"),Rev[0],Rev[1],Rev[2],Rev[3]);
						m_StrRev.TrimLeft();
						m_StrRev.TrimRight();
						CString ProductName(global_fileInfor.product_name);
						ProductName.TrimLeft();
						ProductName.TrimRight();
						m_StrProductName.TrimLeft();
						m_StrProductName.TrimRight();
						if (ProductName.CompareNoCase(m_StrProductName)!=0) 						
						{
							if(((ProductName.CompareNoCase(_T("Minipanel")) == 0) && m_StrProductName.CompareNoCase(_T("MINI")) == 0) ||
								((ProductName.CompareNoCase(_T("MINI")) == 0) && m_StrProductName.CompareNoCase(_T("Minipanel")) == 0))
							{
								Sleep(1);
							}
							else if((m_StrProductName.CompareNoCase(_T("HUMNET")) == 0) ||
								(m_StrProductName.CompareNoCase(_T("CO2NET")) == 0) ||
                                (m_StrProductName.CompareNoCase(_T("CO2")) == 0) ||
								(m_StrProductName.CompareNoCase(_T("PSNET")) == 0))
							{
								Sleep(1);
							}
							else
							{
								nRet=0;
								strTips.Format(_T("Your Device is %s,Your Hex is fit for %s"),m_StrProductName,ProductName);
								OutPutsStatusInfo(strTips, TRUE);

								goto StopServer;
								break;
							}

						}
					}
				}
#endif
                SetDHCP_Data();
                if(mode_has_lanip_try_time++<15)
                {
                    BOOL bBroadcast=false;
                    SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
                    SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,ISP_Device_IP,0);
                    strTips.Format(_T("Send DHCP Package!!(Remain time:%d)"),16-mode_has_lanip_try_time);
                    OutPutsStatusInfo(strTips, TRUE);
                }
                else
                {
                    nRet = 0;
                    goto StopServer;
				   break;
                }
                break;
            case ISP_Send_TFTP_PAKAGE:

                SendUDP_Flash_Socket.SetSockOpt(  SO_SNDTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
                SendUDP_Flash_Socket.SetSockOpt(  SO_RCVTIMEO, ( char * )&nReceiveNetTimeout, sizeof( int ) );

                if (m_tcp_connect_results == 0)
                {
                    //说明在此之前没有检查过bootloader 版本信息,不确定要不要先升级Bootloader;
                    CString temp_real_ip;
                    temp_real_ip.Format(_T("%d.%d.%d.%d"), Byte_ISP_Device_IP[0], Byte_ISP_Device_IP[1], Byte_ISP_Device_IP[2], Byte_ISP_Device_IP[3]);
                    ISP_Device_IP = temp_real_ip;

                    int connect_ret = Open_Socket_Retry(temp_real_ip, m_nClientPort, 1);
                    if (connect_ret)
                    {
                        CString strTips_Ethernet;
                        strTips_Ethernet = _T("Reading bootloader version.");
                        OutPutsStatusInfo(strTips_Ethernet, FALSE);
                        int temp_mu_ret = 0;
                        unsigned short temp_read_reg[100] = { 0 };
                        temp_mu_ret = read_multi_retry(255, temp_read_reg, 0, 100, 3);
                        if (temp_mu_ret < 0)
                        {
                            strTips_Ethernet = _T("Read bootloader version failed.");
                            OutPutsStatusInfo(strTips_Ethernet, FALSE);
                        }
                        else
                        {
                            unsigned short temp_ver_boot = 0;
                            if (temp_read_reg[11] >= temp_read_reg[14])
                                temp_ver_boot = temp_read_reg[11];
                            else
                                temp_ver_boot = temp_read_reg[14];
                            strTips_Ethernet.Format(_T("Bootloader version :%u"), temp_ver_boot);
                            OutPutsStatusInfo(strTips_Ethernet, FALSE);

#pragma region check_boot_version
                            int c2_update_boot = 0;
                            check_bootloader_and_frimware(temp_read_reg[7], 1, temp_read_reg[11], temp_read_reg[14], c2_update_boot, temp_read_reg[4]);

                            if (c2_update_boot == 1)
                            {
                                CString strtips;
                                strtips.Format(_T("New bootloader available ,need update!"));
                                OutPutsStatusInfo(strtips, false);
                                ISP_STEP = ISP_SEND_FLASH_COMMAND;
                                PostMessage(m_pDlg->m_hWnd, WM_FLASH_RESTATR_BOOT, temp_read_reg[7], 0);
                                return 0;
                            }
#pragma endregion

#if 0
                            if ((temp_read_reg[11] >= 62) || (temp_read_reg[14] >= 62))
                            {

                                strTips_Ethernet = _T("Check bootloader version success.");
                                OutPutsStatusInfo(strTips_Ethernet, FALSE);
                            }
                            else
                            {
                                if (temp_mu_ret >= 0)
                                {
                                    strTips_Ethernet.Format(_T("Serial Number: %u"), temp_read_reg[3] * 65536 * 256 + temp_read_reg[2] * 65536 + temp_read_reg[1] * 256 + temp_read_reg[0]);
                                    OutPutsStatusInfo(strTips_Ethernet, FALSE);

                                }

                                if (firmware_must_use_new_bootloader)
                                {
                                    strTips_Ethernet = _T("Bootloader version is too old,need update now.");
                                    OutPutsStatusInfo(strTips_Ethernet, FALSE);
                                    //这里设置标志 需要自动引到ISP来更新 repaire boot 
                                    //HexFileValidation(g_repair_bootloader_file_path);
                                    ISP_STEP = ISP_SEND_FLASH_COMMAND;
                                    PostMessage(m_pDlg->m_hWnd, WM_FLASH_RESTATR_BOOT, temp_read_reg[7], 0);
                                    return 0;
                                }
                            }
#endif
                        }



                    }
                    m_tcp_connect_results = 1;
                }
                strTips.Format(_T("Updating firmware. Device IP : %d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
                OutPutsStatusInfo(strTips, FALSE);
                OutPutsStatusInfo(_T(""), FALSE);
                nRet =Send_Tftp_File();
                if(nRet==0) //break;
                     goto StopServer;
                ISP_STEP = ISP_Send_TFTP_OVER;
                break;
            case  ISP_Send_TFTP_OVER:
                if(mode_flash_over_try_time++<5)
                    SendUDP_Flash_Socket.SendTo(Flash_Done,sizeof(Flash_Done),FLASH_UDP_PORT,ISP_Device_IP,0);
                else
                {
                    nRet = 0;
                     goto StopServer;
                }
                break;
            case ISP_Flash_Done:
                nRet = 1;
                goto StopServer;
                break;
			case ISP_Flash_FAILED:
				OutPutsStatusInfo(Failed_Message, FALSE);
				nRet = 0;
				goto StopServer;
				break;
            default:
                break;
            }


            Sleep(1000);
        }
StopServer:
        if(nRet == 1)
        {
			if(!Use_Old_protocol)
			{
				CString temp_cs;
				temp_cs.Format(_T("Total package(%d).Resend package(%d)"),package_number,total_retry);
				OutPutsStatusInfo(temp_cs, FALSE);
			}

			//CString strTips=_T("Programming successful. ");
			//OutPutsStatusInfo(strTips, FALSE);

            for (int i = 0; i < 4; i++)
            {
                CString temp_cs;
                temp_cs.Format(_T("Wait for the device to reload the firmware.(%d)"), 4 - i);
                if(i==0)
                    OutPutsStatusInfo(temp_cs, FALSE);
                else
                    OutPutsStatusInfo(temp_cs, TRUE);
                Sleep(1000);
            }
			
            if(!auto_flash_mode)
            {
               // AfxMessageBox(strTips);
            }

            //Sleep(5000); //if flash success, wait 5 seconds for LC or NC to start into runtime.
            //if not , the user click flash, it will fail.
        }
        else
        {
            CString strTips=_T("Programming failure.");
            OutPutsStatusInfo(strTips, FALSE);
            if(!auto_flash_mode)
            {
                //AfxMessageBox(strTips);
            }
        }
        if (new_bootload == 1) //目前更新的是新的bootloader 还需要更新客户之前选中的固件;
        {
            if (nRet)
            {
                CString strTips = _T("Update bootloader patch success, please wait.");
                OutPutsStatusInfo(strTips, FALSE);
                strTips = _T("Update firmware, please wait.");
                OutPutsStatusInfo(strTips, FALSE);
                NewBootWriteFinish(nRet);
            }
            else
            {
                CString strTips = _T("Update bootloader patch failed, please try again.");
                OutPutsStatusInfo(strTips, FALSE);
                WriteFinish(nRet);
            }
        }
        else
		    WriteFinish(nRet);
   
    return TRUE;
}

void  TFTPServer::TFTPMessage(LPTSTR sMessage)
{
    CString strTips;
    strTips = _T("TTTTTTTTTTTTTTTTTTTTT");
    OutPutsStatusInfo(strTips, FALSE);
}
 
bool TFTPServer::Send_Tftp_File()
{
    int nCount = 0;
    int nSendNum = m_nDataBufLen;
    CString strTips;
    int nRet  =0;
    BYTE pBuf[512];
    while(nCount < m_nDataBufLen)
    {
        ZeroMemory(pBuf, 512);
        nSendNum = m_nDataBufLen - nCount;
        nSendNum = nSendNum > 512 ?  512 : nSendNum;
        memcpy_s(pBuf,512,m_szDataBuf+nCount,512);
        //memcpy(pBuf, m_szDataBuf+nCount, nSendNum);

        m_nBlkNum++;
        package_number = m_nBlkNum;
        int persent_finished=0;
        int retry =0;
        nCount+= nSendNum;
        persent_finished=(nCount*100)/m_nDataBufLen;


        do
        {
            //if(retry==0)
            //{
            //strTips.Format(_T("Programming finished %d byte.(%d%%)"), nCount,persent_finished);
            //OutPutsStatusInfo(strTips, TRUE);
            //}
            //else
            //{
            if(retry>0)
                total_retry++;
            strTips.Format(_T("Programming finished %d KB.(%d%%).Retry(%d)"), nCount/1024,persent_finished,total_retry);
            OutPutsStatusInfo(strTips, TRUE);
            //}
            nRet = SendDataNew(pBuf, nSendNum);
            if (package_number == 1)
                Sleep(2000);   //发送第一包的时候 设备接收到会做擦除flash的动作，需要等待.
            for (int i=0; i<Remote_timeout; i++)
            {
                //if(IP_is_Local())
                //{
                //	Sleep(1);
                //}
                //else
                //{
                //	Sleep(1);
                //}
                Sleep(1);
                if(next_package_number == package_number +1)
                {
                    if (nflash_receive_to_send_delay_time)
                        Sleep(nflash_receive_to_send_delay_time);
                    goto flash_new_package;
                }
            }
        }
        while (retry++<10);
flash_new_package:
        if(retry>=10)
        {
            CString ErrorMessage;
            ErrorMessage.Format(_T("Flash Package %d Failed.Please try again."),package_number);
			 OutPutsStatusInfo(ErrorMessage, false);
            //if(!auto_flash_mode)
            //    AfxMessageBox(ErrorMessage);

            nRet = 0;
            return 0;
        }
        //else
        //{
        //	total_retry = total_retry +retry;
        //}

    }
    return true;
}



void TFTPServer::HandleWRRequest(BYTE* szBuf,  int nLen)
{
    int nIndex = 0;
    memcpy(&(m_twr.m_wOPCode),szBuf+nIndex, sizeof(WORD));
    nIndex += sizeof(WORD);

    if (nIndex >= nLen)
        return;

    //
    while (szBuf[nIndex] != 0)
    {
        nIndex++;
    }
    if (nIndex >= nLen)
        return;

    int nFileNameLen = nIndex - sizeof(WORD);
    memcpy(m_twr.m_strFileName, szBuf+(nIndex-nFileNameLen), nFileNameLen+1);

    //
    int nIdxTemp = ++nIndex;
    while (szBuf[nIndex] != 0)
    {
        nIndex++;
    }
    if (nIndex >= nLen)
        return;
    int nModeLen =nIndex - nIdxTemp;
    memcpy(m_twr.m_strMode, szBuf+(nIndex-nModeLen), nModeLen+1);

    while(1)
    {
        TFTP_ItemPair* pTip = new TFTP_ItemPair;
        ZeroMemory(pTip, sizeof(TFTP_ItemPair));
        m_twr.m_szItems.push_back(pTip);
        // 选项
        nIdxTemp = ++nIndex;
        while (szBuf[nIndex] != 0)
        {
            nIndex++;
        }
        if (nIndex >= nLen)
            return;
        int nItemLen = nIndex -nIdxTemp;
        memcpy(pTip->m_strItem, szBuf+nIdxTemp, nItemLen+1);

        // 值
        nIdxTemp = ++nIndex;
        while (szBuf[nIndex] != 0)
        {
            nIndex++;
        }
        if (nIndex >= nLen)
            return;
        int nValueLen = nIndex - nIdxTemp;

        memcpy(pTip->m_strValue, szBuf+nIdxTemp, nValueLen+1);
    }

}


void TFTPServer::OutPutsStatusInfo(const CString& strInfo, BOOL bReplace)
{
    int nCount = strInfo.GetLength();
    WCHAR* strNew = new WCHAR[nCount+1];
    ZeroMemory(strNew, (nCount+1)*sizeof(WCHAR));
    LPCTSTR str = LPCTSTR(strInfo);
    memcpy(strNew, str, nCount*sizeof(WCHAR));

    int nRet = 0;
    if (bReplace)
    {
        nRet =PostMessage(m_pDlg->m_hWnd, WM_REPLACE_STATUSINFO, 0, LPARAM(strNew));
    }
    else
    {
        nRet =PostMessage(m_pDlg->m_hWnd, WM_ADD_STATUSINFO, 0, LPARAM(strNew));
    }
}


// 返回 -1， 那么socket error
// 返回 0，  no response
// 1，       ok
int TFTPServer::RecvBOOTP()
{
    BYTE szBuf[512];
    ZeroMemory(szBuf, 512);
    SOCKET sockTemp = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    SOCKADDR_IN siRecv;
    siRecv.sin_family = AF_INET;
    siRecv.sin_port = htons(nLocalDhcp_Port);
    siRecv.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(_T("192.168.0.3"))
    int nRetB = bind(sockTemp, (SOCKADDR *) &siRecv, sizeof(siRecv));

    int nLen = sizeof(siRecv);

    timeval sTimeout;
    sTimeout.tv_sec = 2;
    sTimeout.tv_usec = 0;//0.5s

    fd_set fd;
    FD_ZERO (& fd);
    FD_SET(sockTemp, &fd);


    //while(1)
    {
        int nSRet = select(1, &fd, NULL, NULL, &sTimeout);
        if (nSRet == SOCKET_ERROR )
        {
            //out put error message
            closesocket(sockTemp);
            return -1;
        }
        if (nSRet == 0)
        {
            closesocket(sockTemp);
            return 0;
        }
        int nRet = ::recvfrom(sockTemp,(char*)szBuf,512,0,(sockaddr*)&siRecv, &nLen);
        //Sleep(2);
        //TRACE("RECV by UDP - Num = %d \n", nRet);
        if (nRet == SOCKET_ERROR )
        {
            //out put error message
            closesocket(sockTemp);
            return -1;
        }
        CString strInfo;
        strInfo.Format(_T("Recv BOOTP Byte Num = %d\n"), nRet);
        OutPutsStatusInfo(strInfo, FALSE);

        closesocket(sockTemp);
    }
    return 1;
}



BOOL TFTPServer::SendDHCPPack()
{
    SOCKET dhcpSock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (dhcpSock == INVALID_SOCKET)
    {
        return FALSE;
    }
    //-----------------------------------------------------------------------------------
    // Bind the socket to any address and the specified port.
    SOCKADDR_IN siLocal;
    siLocal.sin_family = AF_INET;
    siLocal.sin_port = htons(nLocalDhcp_Port);
    siLocal.sin_addr.s_addr = htonl(INADDR_ANY);

    int nRet = bind(dhcpSock, (SOCKADDR *) &siLocal, sizeof(siLocal));
    BOOL bBroadcast=TRUE;
    ::setsockopt(dhcpSock,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
    int iMode=1;
    ioctlsocket(dhcpSock,FIONBIO, (u_long FAR*) &iMode);

// 	if (nRet == SOCKET_ERROR)
// 	{
// 		return FALSE;
// 	}

    SOCKADDR_IN siBroadCast;
    siBroadCast.sin_family = AF_INET;
    siBroadCast.sin_port = htons(nSendDhcp_Port);
    siBroadCast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    BYTE pBuffer[512];
    ZeroMemory(pBuffer, 512);
    int nDhcpLen = GetDHCPData(pBuffer, 512);

    int nRetS = ::sendto(dhcpSock,(char*)pBuffer, nDhcpLen,0,(sockaddr*)&siBroadCast,sizeof(siBroadCast));
    //TRACE(_T("DHCP DATA has been send NUM == %d \n"), 512);
    if (nRetS == SOCKET_ERROR)
    {
        int nErrorCode = WSAGetLastError();
        return FALSE;
    }

    closesocket(dhcpSock);
    return TRUE;

}


// 返回的是网络字节顺序
DWORD TFTPServer::GetLocalIP()
{
    IP_ADAPTER_INFO pAdapterInfo;
    ULONG len = sizeof(pAdapterInfo);
    if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS)
    {
        return 0;
    }
    //SOCKADDR_IN sockAddress;   // unreferenced local variable
    long nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);

    return htonl(nLocalIP);
}

int TFTPServer::GetDHCPData(BYTE* pBuf, int nLen)
{
    ASSERT(nLen >= 512);
    ASSERT(pBuf);

    memcpy(pBuf, DHCP_DATAPACK, 304);

    const int nLocalIP1 = 0x3E-0x2A;
    const int nLocalIP2 = 0x11F-0x2A;
    const int nLocalIP3 = 0x12B-0x2A;
    const int nLocalIP4 = 0x137-0x2A;

    DWORD dwIP = GetLocalIP();
    BYTE byIP[4];
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {

        byIP[i] = (dwIP >> ic*8)&0x000000FF;
    }
    memcpy(pBuf+nLocalIP1,byIP , 4);
    memcpy(pBuf+nLocalIP2,byIP , 4);
    memcpy(pBuf+nLocalIP3,byIP , 4);
    memcpy(pBuf+nLocalIP4,byIP , 4);

    //////////////////////////////////////////////////////////////////////////
    // Assign IP address 
    const int nIPPos = 0x10;
    DWORD dwClientIP = m_dwClientIP;
    ZeroMemory(byIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {

        byIP[i] = (dwClientIP >> ic*8)&0x000000FF;
    }

    memcpy(pBuf+nIPPos, (BYTE*)&byIP , 4);

    return 304;
}

int TFTPServer::SendAnyTFTPData()
{
    TFTP_DATA_PACK tdp;

    SOCKET soTest=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (soTest == INVALID_SOCKET)
    {
        return FALSE;
    }
    //-----------------------------------------------------------------------------------
    // Bind the socket to any address and the specified port.
    SOCKADDR_IN siBroadCast;
    siBroadCast.sin_family = AF_INET;
    siBroadCast.sin_port = htons(TFTP_PORT);
    siBroadCast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    int nAddrLen = sizeof(siBroadCast);

    int nRet = bind(soTest, (SOCKADDR *) &siBroadCast, sizeof(siBroadCast));
    BOOL bBroadcast=TRUE;
    ::setsockopt(soTest,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
    int iMode=1;
    ioctlsocket(soTest,FIONBIO, (u_long FAR*) &iMode);


    //ASSERT(nLen <= 512)	;
    tdp.m_wOPCode = 3;
    tdp.m_wOPCode <<= 8;
    tdp.m_wBlkNum = m_nBlkNum++;
    //memcpy(tdp.m_szData, szBuf, nLen);
    ZeroMemory(tdp.m_szData, 0);

    //SOCKET soTest =socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
// 	SOCKADDR_IN siTest;
// 	siTest.sin_family = AF_INET;
// 	siTest.sin_port = htonl(TFTP_PORT);
// 	siTest.sin_addr.S_un.S_addr = ;//inet_addr("192.168.0.3");
//	int nAddrLen = sizeof(m_siClient);
    nRet = sendto(soTest,(char*)(&tdp), sizeof(tdp),0, (sockaddr*)&siBroadCast, nAddrLen);

    if (nRet== SOCKET_ERROR)
    {
        int nError = WSAGetLastError();
        return 0;
    }

    CString strInfo;
    strInfo.Format(_T("Send Data Byte Num = %d ********"), nRet);
    strInfo.Format(_T("Block Num = %d\n"), tdp.m_wBlkNum);
    OutPutsStatusInfo(strInfo, FALSE);

    return nRet;
}



int TFTPServer::SendFlashCommand()
{
    SOCKET soFalsh=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (soFalsh == INVALID_SOCKET)
    {
        return FALSE;
    }

    //////////////////////////////////////////////////////////////////////////
    SOCKADDR_IN siBind;
    siBind.sin_family = AF_INET;
    siBind.sin_port = htons(4321);
    siBind.sin_addr.s_addr = htonl(INADDR_ANY);
    int nRet;
#if 0
    nRet = bind(soFalsh, (SOCKADDR *) &siBind, sizeof(siBind));
    if (nRet == SOCKET_ERROR)
    {
        if(!auto_flash_mode)
            AfxMessageBox(_T("Bind Socket Error,Port 4321 not available"));
        closesocket(soFalsh);
        return FALSE;
    }
#endif

    //-----------------------------------------------------------------------------------
    // Bind the socket to any address and the specified port.
    int nPort = 1234;

    SOCKADDR_IN siSend;
    siSend.sin_family = AF_INET;
    siSend.sin_port = htons(nPort);
    siSend.sin_addr.s_addr= htonl(m_dwClientIP);//(inet_addr(("192.168.0.3")));
    int nAddrLen = sizeof(siSend);

//	int nRet;
    //nRet = bind(soFalsh, (SOCKADDR *) &siSend, sizeof(siSend));

    BYTE byCommand[64] = {0};
    byCommand[0] = 0xEE;	// Command, 2 bytes, 0xee10, command to start flash
    byCommand[1] = 0x10;	//


    nRet = sendto(soFalsh,(char*)(byCommand), 64,0, (sockaddr*)&siSend, nAddrLen);

    if (nRet== SOCKET_ERROR)
    {
        int nError = WSAGetLastError();
        closesocket(soFalsh);
        return 0;
    }

    CString strInfo;
    strInfo.Format(_T("Send FLASH command Data Byte Num = %d ********"), nRet);
    strInfo = _T("Sending flash command...");
//	OutPutsStatusInfo(strInfo, FALSE);
    //TRACE(strInfo);

    closesocket(soFalsh);
    return nRet;
}



//////////////////////////////////////////////////////////////////////////
// Flash finished, notify the parent window
// parameter is the return value of the flash thread
void TFTPServer::WriteFinish(int nFlashFlag)
{
    int	nRet =PostMessage(m_pDlg->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(nFlashFlag));
}


//////////////////////////////////////////////////////////////////////////
// Flash finished, notify the parent window
// parameter is the return value of the flash thread
void TFTPServer::NewBootWriteFinish(int nFlashFlag)
{
    int	nRet = PostMessage(m_pDlg->m_hWnd, WM_FLASH_NEW_BOOT_FINISH, 0, LPARAM(nFlashFlag));
}


void TFTPServer::FlashByEthernet()
{
    device_has_replay_lan_IP =false;
    
    int Udp_resualt=SendUDP_Flash_Socket.Create(LOCAL_UDP_PORT,SOCK_DGRAM);
    if(Udp_resualt == 0)
    {
        DWORD error_msg=GetLastError();
        TCHAR szBuf[250];
        LPVOID lpMsgBuf;
        FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 	FORMAT_MESSAGE_FROM_SYSTEM,	NULL,	error_msg,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	(LPTSTR) &lpMsgBuf,	0, NULL );
        wsprintf(szBuf,_T("failed with error %d: %s"), 	 error_msg, lpMsgBuf);
        if(!auto_flash_mode)
            AfxMessageBox(szBuf);
        LocalFree(lpMsgBuf);

        PostMessage(m_pDlg->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(0));
        return;
    }

    ISP_STEP = 0;
    //int add_port=0;
    //if(m_nClientPort<60000)
    //	add_port= rand() % 30000+ 8000;
    //else
    //	add_port= rand() % 100+ 1;

    GetDeviceIP_String();
    int resualt=TCP_Flash_CMD_Socket.Create(0,SOCK_STREAM);//SOCK_STREAM

    if(resualt == 0)
    {
        DWORD error_msg=GetLastError();
        TCHAR szBuf[250];
        LPVOID lpMsgBuf;
        FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 	FORMAT_MESSAGE_FROM_SYSTEM,	NULL,	error_msg,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	(LPTSTR) &lpMsgBuf,	0, NULL );
        wsprintf(szBuf,_T("failed with error %d: %s"), 	 error_msg, lpMsgBuf);
        if(!auto_flash_mode)
            AfxMessageBox(szBuf);
        LocalFree(lpMsgBuf);

        PostMessage(m_pDlg->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(0));
        return;
    }
  //   TCP_Flash_CMD_Socket.Connect(ISP_Device_IP,m_nClientPort);
	 //Sleep(3000);
   // TCP_Flash_CMD_Socket.m_hex_bin_filepath=m_strFileName;
    if (new_bootload == 0)
    {
        CString strTips_Ethernet;
        strTips_Ethernet = _T("Connecting device.");
        OutPutsStatusInfo(strTips_Ethernet, FALSE);

        int connect_ret = Open_Socket_Retry(ISP_Device_IP, m_nClientPort, 2);
        if (connect_ret)
        {
            m_tcp_connect_results = 1;
            strTips_Ethernet = _T("Reading bootloader version.");
            OutPutsStatusInfo(strTips_Ethernet, FALSE);
            int temp_mu_ret = 0;
            unsigned short temp_read_reg[100] = { 0 };
            temp_mu_ret = read_multi_retry(255, temp_read_reg, 0, 100,3);
            if (temp_mu_ret < 0)
            {
                strTips_Ethernet = _T("Read bootloader version failed.");
                OutPutsStatusInfo(strTips_Ethernet, FALSE);
            }
            else
            {



                unsigned short temp_ver_boot = 0;
                if(temp_read_reg[11] >= temp_read_reg[14])
                    temp_ver_boot = temp_read_reg[11];
                else
                    temp_ver_boot = temp_read_reg[14];
                strTips_Ethernet.Format(_T("Bootloader version :%u"), temp_ver_boot);
                OutPutsStatusInfo(strTips_Ethernet, FALSE);

                int c2_update_boot = 0;
                int Ret_Result = 0;

                Ret_Result = check_bootloader_and_frimware(temp_read_reg[7], 1 , temp_read_reg[11], temp_read_reg[14], c2_update_boot, temp_read_reg[4]);

                if (c2_update_boot == 1)
                {
                    CString strtips;
                    strtips.Format(_T("New bootloader available ,need update!"));
                    OutPutsStatusInfo(strtips, false);
                    PostMessage(m_pDlg->m_hWnd, WM_FLASH_RESTATR_BOOT, temp_read_reg[7], 0);
                    Ret_Result = 2;
                    return;
                }


#if 0
                if ((temp_read_reg[11] >= 62) || (temp_read_reg[14] >= 62))
                {

                    strTips_Ethernet = _T("Check bootloader version success.");
                    OutPutsStatusInfo(strTips_Ethernet, FALSE);
                }
                else
                {
                    if (temp_mu_ret >= 0)
                    {
                        strTips_Ethernet.Format(_T("Serial Number: %u"), temp_read_reg[3] * 65536 * 256 + temp_read_reg[2] * 65536 + temp_read_reg[1] * 256 + temp_read_reg[0]);
                        OutPutsStatusInfo(strTips_Ethernet, FALSE);

                    }

                    if (firmware_must_use_new_bootloader)
                    {

                        strTips_Ethernet = _T("Bootloader version is too old,need update now.");
                        OutPutsStatusInfo(strTips_Ethernet, FALSE);
                        //这里设置标志 需要自动引到ISP来更新 repaire boot 
                        //HexFileValidation(g_repair_bootloader_file_path);
                        PostMessage(m_pDlg->m_hWnd, WM_FLASH_RESTATR_BOOT, temp_read_reg[7], 0);
                        return;
                    }
                }
#endif
            }

            

        }
        else
        {
            m_tcp_connect_results = 0;
            strTips_Ethernet.Format(_T("Can't connect to IP:%s port:%d."), ISP_Device_IP, m_nClientPort);
            OutPutsStatusInfo(strTips_Ethernet, FALSE);
            strTips_Ethernet.Format(_T(" "));
            OutPutsStatusInfo(strTips_Ethernet, FALSE);
        }
    }
    //else
    //{
    //    new_bootload = 0;
    //}

    m_pThread = AfxBeginThread(_StartSeverFunc, this);

    ASSERT(m_pThread);
}


UINT _StartSeverFunc(LPVOID pParam)
{
    TFTPServer* pServer = (TFTPServer*)(pParam);

    if(pServer->StartServer())
    {
        return 1;
    }

    return 0;
}


//Comment by Fance
//This old protocol don't support remote ISP.
//so we change the protocol,but we also need the old protocol to update the old bootload to new bootload
BOOL TFTPServer::StartServer_Old_Protocol()
{
    m_nBlkNum = 0;
    int nRet  =0;


    //else
    //{
    //	CString strTips = _T("Initializing network setting...");
    //	OutPutsStatusInfo(strTips, FALSE);
    //}
    //CString strTips = _T("Use old protocol send flash command");
    //OutPutsStatusInfo(strTips, FALSE);

    if(!SendDHCPPack())
        //if(!SendAnyTFTPData())
    {
        CString strTips = _T("Send dhcp pack broadcast failed.");
        OutPutsStatusInfo(strTips, FALSE);
        //AfxMessageBox(strTips, MB_OK);
        ReleaseAll();
        //goto StopServer;
        return 0;
    }
    else
    {
        CString strTips = _T("ISP has Send a dhcp pack.");
    }

    //BroadCastToClient();


    //阻塞接受
    if(!RecvRequest())
    {
        CString strTips = _T("Recv TFTP read request pack failed.");
        OutPutsStatusInfo(strTips, FALSE);
        //AfxMessageBox(strTips, MB_OK);
        ReleaseAll();
        //goto StopServer;
        return 0;
    }
    else
    {
        CString strTips = _T("Recv TFTP read request pack.");
        //OutPutInfo(strTips);
        OutPutsStatusInfo(strTips, FALSE);
        // fornew m_pDlg->m_sendmul_button.EnableWindow(TRUE);
    }

    nRet = SendProcess();

    //if (nRet == 1)
    //{
    //	CString strTips=_T("Programming successful. ");
    //	OutPutsStatusInfo(strTips, FALSE);
    //	//AfxMessageBox(strTips);
    //}
    //else
    //{
    //	CString strTips=_T("Programming failure.");
    //	OutPutsStatusInfo(strTips, FALSE);
    //	//AfxMessageBox(strTips);
    //}


//StopServer:
//
//	WriteFinish(nRet);
//	//Sleep(3000);
//	((CISPDlg*)m_pDlg)->Show_Flash_DeviceInfor_NET();
    return nRet;
}
