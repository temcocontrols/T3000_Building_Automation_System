#include "StdAfx.h"

#include "Resource.h"
#include "TFTPServer.h"
#include "ISPDlg.h"
#include "Iphlpapi.h"
#include "MySocket.h"

////#include <vector>			//矢量模板
////using  std::vector;			//命名空间
//#include "define.h"
#pragma  comment(lib,"Iphlpapi.lib")
static bool need_show_device_ip = true;
UINT _StartSeverFunc(LPVOID pParam);
extern bool auto_flash_mode;	//用于自动烧写，不要弹框;
char sendbuf[45];
extern CString g_strFlashInfo;
extern unsigned int Remote_timeout;
/*extern*/ CRITICAL_SECTION g_cs;
/*extern*/ CString showing_text;
/*extern*/ int writing_row;

const int TFTP_PORT = 69;

const int nLocalDhcp_Port = 67;			// Server本地67
const int nSendDhcp_Port = 68;			// client目的68


typedef struct _Product_IP_ID 
{
	CString ISP_Device_IP;
	//unsigned char Byte_ISP_Device_IP[4];
	unsigned char ID;
}Product_IP_ID;


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
void TFTPServer::Set_FileProductName(CString Name){
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

// 广播通知NC
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


// 接受Request
int TFTPServer::RecvRequest()
{
	BYTE szBuf[512];
	ZeroMemory(szBuf, 512);

	SOCKADDR_IN  siRecvRead; // 接收read request，用来绑定
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
		strTips.Format(_T("Programming finished %d byte."), nCount);
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
		{ // 重发了5次，结束
			return 0;
		}
	}
	//SendEndDataPack();
	return 1;
}


// 发送数据
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
	else  // 最后一个包，不要发无用数据
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


// 发送数据
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




// 发送数据
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
//	else  // 最后一个包，不要发无用数据
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

// 接受ACK
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
		closesocket(m_sock);					// 接收tftp request socket, 绑定了69端口
	}
	if (m_soSend)
	{
		closesocket(m_soSend);				// 发送socket，发送tftp数据的
	}
	
	if (m_soRecv)
	{
		closesocket(m_soRecv);				// 接收socket，接收tftp ack的
	}
	

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
		memset(Byte_ISP_Device_IP,0,4);
		memcpy_s(sendbuf+25,4,Byte_ISP_Device_IP,4);	
	}

	char reserved[16];
	ZeroMemory(reserved,16);
	memcpy_s(sendbuf+29,16,reserved,16);
}


BOOL TFTPServer::StartServer()
{
	char Flash_Done[10];
	memcpy_s(Flash_Done,10,"FLASH DONE",10);
	m_nBlkNum = 0;	
	int nRet  =0;

	CString strTips;
	need_show_device_ip = true;
	
	
	int nSendNum = m_nDataBufLen;

	BYTE byCommand[64] = {0};
	byCommand[0] = 0xEE;	// 命令，2字节，0xEE10，作为flash开始的命令
	byCommand[1] = 0x10;	// 

	BYTE szBuf[512];
	ZeroMemory(szBuf, 512);
	//int nLen = sizeof(m_test_siClient);

	GetDeviceIP_String();
	SetDHCP_Data();
	ISP_STEP = ISP_SEND_FLASH_COMMAND;
	int mode_send_flash_try_time=0;
	bool first_time_send_dhcp_package = true;
	int mode_no_lanip_try_time=0;
	int mode_has_lanip_try_time=0;
	int mode_flash_over_try_time=0;
	total_retry=0;
	has_enter_dhcp_has_lanip_block =false;//用于确认 2个线程的状态，避免ISP_STEP跳动。
	int has_wait_device_into_bootloader = false;



	int nNetTimeout = 1000;//Send Time Out!
	int nReceiveNetTimeout = 1500;//Send Time Out!

	bool Use_Old_protocol=false;

	if(!InitSocket())
	{
		CString strTips = _T("Initializing network failed...");
		OutPutsStatusInfo(strTips, FALSE);
		AfxMessageBox(strTips, MB_OK);
		ReleaseAll();
		//goto StopServer;
		return 0;
	}


	//bool hassendonce=false;//这个需要 EVAN改 现在只是零时的 只发一遍广播，发多遍广播 会回复5个byte的 错误。
	while(1)
	{
		switch(ISP_STEP)
		{
		case ISP_SEND_FLASH_COMMAND:
			if((mode_send_flash_try_time++)<10)
			{

					SendFlashCommand();

					#if 0
					nRet = RecvBOOTP();
					if (nRet==-1)
					{
						CString strTips = _T("Network connection interrupted. Please try again!");
						OutPutsStatusInfo(strTips, FALSE);
						AfxMessageBox(strTips, MB_OK);
						ReleaseAll();
						//goto StopServer;
						return 0;
					}
					else if(nRet ==  1)
					{
						CString strTips = _T("Recv the BOOTP Request pack.");
						nRet=StartServer_Old_Protocol();
						Use_Old_protocol = true;
						goto StopServer;
					}
					else
					{
						int n = 0;
						strTips.Format(_T("Use old protocol send flash command.Remain(%d)"),5-mode_send_flash_try_time);
						OutPutsStatusInfo(strTips, TRUE);
					}
					#endif
				int send_ret=TCP_Flash_CMD_Socket.SendTo(byCommand,sizeof(byCommand),m_nClientPort,ISP_Device_IP,0);
				//TRACE(_T("%d"),send_ret);
				if(send_ret<0)	//如果发送失败 就尝试 再次进行TCP连接
				{
					TCP_Flash_CMD_Socket.Connect(ISP_Device_IP,m_nClientPort);
				}
				SetDHCP_Data();

				
				//if(IP_is_Local())//如果是本地的 就用广播的 方式 发送 DHCP
				//{
					dhcp_package_is_broadcast = true;
					BOOL bBroadcast=TRUE;
					//::sendto(dhcpSock,(char*)pBuffer, nDhcpLen,0,(sockaddr*)&siBroadCast,sizeof(siBroadCast));
					SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
					int send_count = SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,_T("255.255.255.255"),0);
					if(send_count <= 0)
						AfxMessageBox(_T("Send command failed!"));
				//}
				//else
				SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,ISP_Device_IP,0);

				

				strTips.Format(_T("Communication with device.(Remain time:%d)"),10-mode_send_flash_try_time);
				OutPutsStatusInfo(strTips, TRUE);
			}
			else
			{
				//nRet=StartServer_Old_Protocol();
				//if(nRet==0)
				//{
					strTips = _T("No Connection!Please check the network connection.");
					OutPutsStatusInfo(strTips, FALSE);				
				//}


					goto StopServer;
				//if(some_device_reply_the_broadcast == true)	//这个是用来支持 多个 在bootload中回复的 广播，如果有 则 先打印其中有几个回复
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
				strTips.Format(_T("The Device IP is %d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
				OutPutsStatusInfo(strTips, FALSE);
				OutPutsStatusInfo(_T(""), FALSE);
			}

			if((device_jump_from_runtime == true)&&(has_wait_device_into_bootloader == false))
			{
				has_wait_device_into_bootloader = true;


				OutPutsStatusInfo(_T(""), FALSE);
				for (int i=0;i<7;i++)
				{
					strTips.Format(_T("Wait the device jump to bootloader.(%ds)"),7-i);
					OutPutsStatusInfo(strTips, TRUE);
					Sleep(1000);
				}		
				OutPutsStatusInfo(_T(""), FALSE);
			}

			if(dhcp_package_is_broadcast == true)//如果是用广播的方式 要把界面上的 IP地址改为 NC/LC回复的 IP；
			{
				ISP_Device_IP.Empty();
				ISP_Device_IP.Format(_T("%d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
				m_StrProductName.Format(_T("%C%C%C%C%C%C%C%C%C%C%C%C%C"),Product_Name[0],Product_Name[1],Product_Name[2],Product_Name[3],Product_Name[4]
                                                                         ,Product_Name[5],Product_Name[6],Product_Name[7],Product_Name[8],Product_Name[9]
                                                                         ,Product_Name[10],Product_Name[11]);
                m_StrProductName.TrimLeft();
                m_StrProductName.TrimRight();
                m_StrRev.Format(_T("%C%C%C%C"),Rev[0],Rev[1],Rev[2],Rev[3]);
                m_StrRev.TrimLeft();
                m_StrRev.TrimRight();
 				/*if (m_StrFileProductName.Find(m_StrProductName)==-1)
 				{
                    nRet=0;
                    goto StopServer;
 				}*/


			}
			SetDHCP_Data();
			if(mode_has_lanip_try_time++<5)
			{
				BOOL bBroadcast=false;
				SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
				SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,ISP_Device_IP,0);
				strTips.Format(_T("Send DHCP Package!!(Remain time:%d)"),6-mode_has_lanip_try_time);
				OutPutsStatusInfo(strTips, TRUE);
			}
			else
			{
				nRet = 0;
				goto StopServer;
			}
			break;
		case ISP_Send_TFTP_PAKAGE:
			
			SendUDP_Flash_Socket.SetSockOpt(  SO_SNDTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
			SendUDP_Flash_Socket.SetSockOpt(  SO_RCVTIMEO, ( char * )&nReceiveNetTimeout, sizeof( int ) );

			strTips.Format(_T("The Device IP is %d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
			OutPutsStatusInfo(strTips, FALSE);
			OutPutsStatusInfo(_T(""), FALSE);
			nRet =Send_Tftp_File();
			if(nRet==0)
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
		default:
			break;
		}

		Sleep(1000);
	}
StopServer:
	WriteFinish(nRet);
	if(nRet == 1)
	{
		if(!Use_Old_protocol)
		{
		CString temp_cs;
		temp_cs.Format(_T("Total package(%d).Resend package(%d)"),package_number,total_retry);
		OutPutsStatusInfo(temp_cs, FALSE);
		}

		CString strTips=_T("Programming successful. ");
		OutPutsStatusInfo(strTips, FALSE);
		if(!auto_flash_mode)
		{
			AfxMessageBox(strTips);
		}

		//Sleep(5000); //if flash success, wait 5 seconds for LC or NC to start into runtime.
		//if not , the user click flash, it will be not success.
	}
	else
	{
		CString strTips=_T("Programming failure.");
		OutPutsStatusInfo(strTips, FALSE);
		if(!auto_flash_mode)
		{
			AfxMessageBox(strTips);
		}
	}

	

	return TRUE;
}
void TFTPServer::Set_bininfor(Bin_Info temp){
	
	int i;
	for (  i=0;i<5;i++)
	{
		m_binInfor.company[i]=temp.company[i];
	}
	for (  i=0;i<10;i++)
	{
		m_binInfor.product_name[i]=temp.product_name[i];
	}
	for (  i=0;i<3;i++)
	{
		m_binInfor.reserved[i]=temp.reserved[i];
	}
	m_binInfor.software_high=temp.software_high;
	m_binInfor.software_low=temp.software_low;
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
			strTips.Format(_T("Programming finished %d byte.(%d%%).Retry(%d)"), nCount,persent_finished,total_retry);
			OutPutsStatusInfo(strTips, TRUE);
			//}
			nRet = SendDataNew(pBuf, nSendNum);
			for (int i=0;i<Remote_timeout;i++)
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
			if(!auto_flash_mode)
				AfxMessageBox(ErrorMessage);

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
	//SOCKADDR_IN sockAddress;   // commented by zgq;2010-12-06; unreferenced local variable
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
	// 分配界面设置的IP
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
	byCommand[0] = 0xEE;	// 命令，2字节，0xEE10，作为flash开始的命令
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
// flash 完了，不论成功还是失败，都通知父窗口
// 参数就是flash线程的返回值
void TFTPServer::WriteFinish(int nFlashFlag)
{
	int	nRet =PostMessage(m_pDlg->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(nFlashFlag));
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
		AfxMessageBox(szBuf);
		LocalFree(lpMsgBuf);

		PostMessage(m_pDlg->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(0));
		return;
	}
	TCP_Flash_CMD_Socket.Connect(ISP_Device_IP,m_nClientPort);
	TCP_Flash_CMD_Socket.m_hex_bin_filepath=m_strFileName;
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