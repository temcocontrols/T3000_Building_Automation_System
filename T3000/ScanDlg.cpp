// ScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ScanDlg.h"
#include "GridButton.h"
#include "TStatScanner.h"
#include "MainFrm.h"
 
#define STATUS_FAILED 0xFFFF 
#define DEF_PACKET_SIZE    32
#define DEF_PACKET_NUMBER  4    /* 发送数据报的个数 */
#define MAX_PACKET 1024 

// scan dialog 中表格的列的定义
#define SCAN_TABLE_TYPE					0
#define SCAN_TABLE_BUILDING				1
#define SCAN_TABLE_FLOOR					2
#define SCAN_TABLE_ROOM					3
#define SCAN_TABLE_SUBNET				4
#define SCAN_TABLE_SERIALID				5
#define SCAN_TABLE_ADDRESS				6
#define SCAN_TABLE_COMPORT			7

// #define SCAN_TABLE_CONFLICT			8
// #define SCAN_TABLE_FIXCONFLICT		9
///#define SCAN_TABLE_BAUDRATE			6
#define SCAN_TABLE_PROTOCOL			8
#define NEW_IPADRESS 9


IMPLEMENT_DYNAMIC(CScanDlg, CDialog)

CScanDlg::CScanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDlg::IDD, pParent)
{
	InitializeCriticalSection(&m_csGrid);
	m_pScanner = NULL;
	m_szGridEditPos.cx = -1;
	m_szGridEditPos.cy = -1;
	m_IsScan=TRUE;
	exsit_ref= RGB(255, 255, 255);
	new_ref = RGB(178, 227, 137);
	exsit_serial = false;
}

CScanDlg::~CScanDlg()
{
	DeleteCriticalSection(&m_csGrid);
}

void CScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_flexGrid);

	DDX_Control(pDX, IDC_EDIT_GRIDEDIT, m_editGrid);
}


BEGIN_MESSAGE_MAP(CScanDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SCANALL, &CScanDlg::OnBnClickedButtonScanall)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CScanDlg::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, &CScanDlg::OnBnClickedButtonManual)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CScanDlg::OnBnClickedButtonExit)
	ON_MESSAGE(WM_SCANFINISH, 	OnScanFinish)
	ON_MESSAGE(WM_ADDCOMSCAN, 	OnAddComScanRet)
	ON_MESSAGE(WM_ADDNETSCAN, 	OnAddNetScanRet)

	
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_GRIDEDIT, &CScanDlg::OnEnKillfocusEditGridedit)

END_MESSAGE_MAP()


// CScanDlg message handlers



void CScanDlg::OnBnClickedButtonExit()
{
	SaveAllNodeToDB();
	Release();
	CDialog::OnOK();
}
//CString &StrIP,CString &StrMask,CString &StrGetway
void CScanDlg::GetIPMaskGetWay(CString &StrIP,CString &StrMask,CString &StrGetway){
	PIP_ADAPTER_INFO pAdapterInfo; 
	PIP_ADAPTER_INFO pAdapter = NULL; 
	DWORD dwRetVal = 0; 
	ULONG ulOutBufLen; 
	pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO)); 
	ulOutBufLen = sizeof(IP_ADAPTER_INFO); 
	 ;
	// 第一次调用GetAdapterInfo获取ulOutBufLen大小 
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{ 
		free(pAdapterInfo); 
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	} 

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) { 
		pAdapter = pAdapterInfo; 
		while (pAdapter) 
		{ 
			 if(pAdapter->Type == MIB_IF_TYPE_ETHERNET){
			 				MultiByteToWideChar( CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1, 
			 					StrIP.GetBuffer(MAX_PATH), MAX_PATH );
			 					StrIP.ReleaseBuffer();
			 				//StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String); 
			 				MultiByteToWideChar( CP_ACP, 0,pAdapter->IpAddressList.IpMask.String, (int)strlen((char *)pAdapter->IpAddressList.IpMask.String)+1, 
			 					StrMask.GetBuffer(MAX_PATH), MAX_PATH );
			 					StrMask.ReleaseBuffer();
			 
			 				//StrMask.Format(_T("%s"), pAdapter->IpAddressList.IpMask.String); 
			 
			 				MultiByteToWideChar( CP_ACP, 0,pAdapter->GatewayList.IpAddress.String, (int)strlen((char *)pAdapter->GatewayList.IpAddress.String)+1, 
			 					StrGetway.GetBuffer(MAX_PATH), MAX_PATH );
			 					StrGetway.ReleaseBuffer();
			 				if (StrMask.CompareNoCase(_T("0.0.0.0"))!=0)
			 				{
			 					break;
			 				}
						}
		else if (pAdapter->Type == IF_TYPE_IEEE80211){
							MultiByteToWideChar( CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1, 
							StrIP.GetBuffer(MAX_PATH), MAX_PATH );
							StrIP.ReleaseBuffer();
			//StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String); 
							MultiByteToWideChar( CP_ACP, 0,pAdapter->IpAddressList.IpMask.String, (int)strlen((char *)pAdapter->IpAddressList.IpMask.String)+1, 
							StrMask.GetBuffer(MAX_PATH), MAX_PATH );
							StrMask.ReleaseBuffer();

			//StrMask.Format(_T("%s"), pAdapter->IpAddressList.IpMask.String); 

							MultiByteToWideChar( CP_ACP, 0,pAdapter->GatewayList.IpAddress.String, (int)strlen((char *)pAdapter->GatewayList.IpAddress.String)+1, 
								StrGetway.GetBuffer(MAX_PATH), MAX_PATH );
							StrGetway.ReleaseBuffer();
							if (StrMask.CompareNoCase(_T("0.0.0.0"))!=0)
							{
								break;
							}
					}

			/*StrGetway.Format(_T("%s"), pAdapter->GatewayList.IpAddress.String); */
						pAdapter = pAdapter->Next; 
		} 
	} 
	else 
	{ 

	} 
}


void CScanDlg::GetIPMaskGetWay(vector<ALL_LOCAL_SUBNET_NODE>& Vector_Subnet){
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

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) { 
		pAdapter = pAdapterInfo; 
		while (pAdapter) 
		{ 
// 			if(pAdapter->Type == MIB_IF_TYPE_ETHERNET)
// 			{
// 				MultiByteToWideChar( CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1, 
// 					StrIP.GetBuffer(MAX_PATH), MAX_PATH );
// 				StrIP.ReleaseBuffer();
// 				//StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String); 
// 				MultiByteToWideChar( CP_ACP, 0,pAdapter->IpAddressList.IpMask.String, (int)strlen((char *)pAdapter->IpAddressList.IpMask.String)+1, 
// 					StrMask.GetBuffer(MAX_PATH), MAX_PATH );
// 				StrMask.ReleaseBuffer();
// 
// 				//StrMask.Format(_T("%s"), pAdapter->IpAddressList.IpMask.String); 
// 
// 				MultiByteToWideChar( CP_ACP, 0,pAdapter->GatewayList.IpAddress.String, (int)strlen((char *)pAdapter->GatewayList.IpAddress.String)+1, 
// 					StrGetway.GetBuffer(MAX_PATH), MAX_PATH );
// 				StrGetway.ReleaseBuffer();
// 				if (StrMask.CompareNoCase(_T("0.0.0.0"))!=0)
// 				{
// 					break;
// 				}
// 
// 			}
// 			else if (pAdapter->Type == IF_TYPE_IEEE80211)
// 			{
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
 
	           Vector_Subnet.push_back(Temp_Node);
			/*StrGetway.Format(_T("%s"), pAdapter->GatewayList.IpAddress.String); */
			pAdapter = pAdapter->Next; 
		} 
	} 
	else 
	{ 

	} 
}
BOOL CScanDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();	
	//CString m_strlocalipaddress;
	//CString m_strlocalsubnet;
	//CString m_strlocalgateway;
	GetIPMaskGetWay(m_strlocalipaddress,m_strlocalsubnet,m_strlocalgateway);
	
	InitScanGrid();	
//	Sleep(3000);
	if (m_IsScan)
	{
		AddComDeviceToGrid(m_pScanner->m_szTstatScandRet);
		//AddNetDeviceToGrid(m_pScanner->m_szNCScanRet);
		AddNetDeviceToGrid();
	}
	else
	{
		CString anewip;
		if (GetNewIP(anewip))
		{
			FLEX_GRID_PUT_COLOR_STR(1,NEW_IPADRESS,anewip); 
		}
	     
		FLEX_GRID_PUT_COLOR_STR(1,SCAN_TABLE_ADDRESS,m_net_product_node.BuildingInfo.strIp); 

		CString strType =GetProductName(m_net_product_node.product_class_id);		
		FLEX_GRID_PUT_COLOR_STR(1,SCAN_TABLE_TYPE,strType); 
		CString strSerailID;
		unsigned int nSID =m_net_product_node.serial_number;
		strSerailID.Format(_T("%u"), nSID);
		FLEX_GRID_PUT_COLOR_STR(1,SCAN_TABLE_SERIALID,strSerailID);
		
		 		 
		FLEX_GRID_PUT_COLOR_STR(1,SCAN_TABLE_COMPORT,m_net_product_node.BuildingInfo.strIpPort); 

// 		CString strBuilding =m_net_product_node.BuildingInfo.strBuildingName;		
// 		FLEX_GRID_PUT_COLOR_STR(1,SCAN_TABLE_BUILDING,strBuilding); 

		/*CString strFloor = m_net_product_node.BuildingInfo.;
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_FLOOR,strFloor); 

		CString strRoom = pNetInfo->m_pNet->GetRoomName();
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ROOM,strRoom); 

		CString strSubnet = pNetInfo->m_pNet->GetSubnetName();
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SUBNET,strSubnet); 

		CString strSerailID;
		int nSID = pNetInfo->m_pNet->GetSerialID();
		strSerailID.Format(_T("%d"), nSID);
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SERIALID,strSerailID); */
		/////

		/////
// 		CString strPort;
// 		int nPort = pNetInfo->m_pNet->GetIPPort();
// 		strPort.Format(_T("%d"), nPort);
// 		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_COMPORT,strPort); 
// 		////
// 		CString strProtocol;		
// 		if(pNetInfo->m_pNet->GetProtocol() == 3)
// 		{
// 			strProtocol.Format(_T("BacnetIP"));
// 		}
// 		else
// 			strProtocol.Format(_T("TCP/IP"));
// 		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_PROTOCOL,strProtocol); 
		 

	}



	return TRUE;
}

BOOL CScanDlg::GetNewIP(CString &newIP){
	USES_CONVERSION;
	IN_ADDR sa;
	//----------------------
	for (int i=0;i<allsubnets.size();i++)
	{
// 		szIP=W2A(allsubnets[i].StrIP);
// 		dwIP=inet_addr(szIP);
// 		sa.S_un.S_addr=dwIP;
// 
// 		if ( ia.S_un.S_un_b.s_b1 == sa.S_un.S_un_b.s_b1 &&
// 			ia.S_un.S_un_b.s_b2 == sa.S_un.S_un_b.s_b2 &&
// 			ia.S_un.S_un_b.s_b3 == sa.S_un.S_un_b.s_b3 
// 			)
// 		{
// 			ret=TRUE;
// 			break;
// 		}
//         if((allsubnets[i].NetworkCardType!=)&&(allsubnets[i].StrIP.Find(_T("0.0."))==-1))
//         {
//         }
        
	}


	 LPCSTR szIP=W2A(m_strlocalipaddress);
	 DWORD dwIP=inet_addr(szIP);
	
	sa.S_un.S_addr=dwIP;


	for (int i=2;i<255;i++)
	{
		CString anewip;
		anewip.Format(_T("%d.%d.%d.%d"), sa.S_un.S_un_b.s_b1,sa.S_un.S_un_b.s_b2,sa.S_un.S_un_b.s_b3,i);

		if (!TestPing(anewip))
		{
			newIP=anewip;
			return TRUE;
		}
		

	}
	return FALSE;
}

BOOL CScanDlg::GetNewIP(CString &newIP,CString BaseIP){
	USES_CONVERSION;
	IN_ADDR sa;
 


	LPCSTR szIP=W2A(BaseIP);
	DWORD dwIP=inet_addr(szIP);

	sa.S_un.S_addr=dwIP;


	for (int i=2;i<255;i++)
	{
		CString anewip;
		anewip.Format(_T("%d.%d.%d.%d"), sa.S_un.S_un_b.s_b1,sa.S_un.S_un_b.s_b2,sa.S_un.S_un_b.s_b3,i);

		if (!TestPing(anewip))
		{
			newIP=anewip;
			return TRUE;
		}


	}
	return FALSE;
}

#include "ping.h"
BOOL CScanDlg::TestPing(const CString& strIP)
{	
 #ifdef CPING_USE_ICMP

    for (int i = 0; i < 3; i++)
    {
        CPing p1;
        CPingReply pr1;
        if (p1.Ping1((LPCTSTR)strIP, pr1))
        {
            return TRUE;
        }
        else
        {
            Sleep(500);
            continue;
        }
    }

    return false;
#endif	

}

int CScanDlg::DecodeResp(char *buf, int bytes,struct sockaddr_in *from)
{ 
	IpHeader *iphdr; 
	IcmpHeader *icmphdr; 
	unsigned short iphdrlen; 
	iphdr = (IpHeader *)buf; 
	iphdrlen = (iphdr->h_len) * 4 ; // number of 32-bit words *4 = bytes 
	if (bytes < iphdrlen + ICMP_MIN) 
	{ 
		//printf("Too few bytes from %s\n",inet_ntoa(from->sin_addr)); 
		CString str;
		str.Format(_T("Too few bytes from %s\n"),inet_ntoa(from->sin_addr));
		 
	} 
	icmphdr = (IcmpHeader*)(buf + iphdrlen); 
	if (icmphdr->i_type != ICMP_ECHOREPLY) 
	{ 
		//fprintf(stderr,"non-echo type %d recvd\n",icmphdr->i_type); 
		CString str;
		str.Format(_T("non-echo type %d recvd\n"),  icmphdr->i_type);
		 
		return 1; 
	} 
	if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
	{ 
		//fprintf(stderr,"someone else's packet!\n"); 
		CString str =(_T("someone else's packet!\n")); 
	 
		return 1; 
	} 

	// 	printf("%d bytes from %s:",bytes, inet_ntoa(from->sin_addr)); 
	 
	// SendEchoMessage(str);
	// 	printf(" icmp_seq = %d. ",icmphdr->i_seq); 
	// 	 CString str1;
	// 	 str1.Format(_T("icmp_seq=%d "), icmphdr->i_seq); 


	// SendEchoMessage(str1);

	// 	printf(" time: %d ms ",GetTickCount()-icmphdr->timestamp); 
 

 

	// 	printf("\n");

	//CString 
	return 0; 
} 
USHORT CScanDlg::Checksum(USHORT *buffer, int size) 
{ 
	unsigned long cksum=0; 
	while(size >1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(USHORT); 
	} 
	if(size) 
	{ 
		cksum += *(UCHAR*)buffer; 
	} 
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (USHORT)(~cksum); 
} 
void CScanDlg::FillIcmpData(char * icmp_data, int datasize)
{ 
	IcmpHeader *icmp_hdr; 
	char *datapart; 
	icmp_hdr = (IcmpHeader*)icmp_data; 
	icmp_hdr->i_type = ICMP_ECHO; 
	icmp_hdr->i_code = 0; 
	icmp_hdr->i_id = (USHORT)GetCurrentProcessId(); 
	icmp_hdr->i_cksum = 0; 
	icmp_hdr->i_seq = 0; 
	datapart = icmp_data + sizeof(IcmpHeader); 

	memset(datapart, 'E', datasize - sizeof(IcmpHeader)); 
} 
BOOL CScanDlg::CheckTheSameSubnet(CString strIP){
    BOOL ret=FALSE;
	USES_CONVERSION;
	LPCSTR szIP = W2A(strIP);
	DWORD dwIP = inet_addr(szIP);
	IN_ADDR ia,sa;
	ia.S_un.S_addr = dwIP;

	allsubnets.clear();
	GetIPMaskGetWay(allsubnets);
	for (int i=0;i<allsubnets.size();i++)
	{
		szIP=W2A(allsubnets[i].StrIP);
		dwIP=inet_addr(szIP);
		sa.S_un.S_addr=dwIP;

		if ( ia.S_un.S_un_b.s_b1 == sa.S_un.S_un_b.s_b1 &&
			ia.S_un.S_un_b.s_b2 == sa.S_un.S_un_b.s_b2 &&
			ia.S_un.S_un_b.s_b3 == sa.S_un.S_un_b.s_b3 
			)
		{
			ret=TRUE;
			break;
		}

	}
	
	return ret;

}	


extern volatile HANDLE Read_Mutex;
void CScanDlg::OnBnClickedButtonScanall()
{
    allsubnets.clear();
	GetIPMaskGetWay(allsubnets);
	
	WaitForSingleObject(Read_Mutex,INFINITE);//Add by Fance .这里要等待Main Scan 那里弄完了 在去扫;
	ReleaseMutex(Read_Mutex);

	SOCKET sListen=NULL;
	CString strlog;
	int nRet = 0;
	short nmsgType=UPD_BROADCAST_QRY_MSG;
	const DWORD END_FLAG = 0x00000000;
	TIMEVAL time;
	time.tv_sec =3;
	time.tv_usec = 1000;
	fd_set fdSocket;
	BYTE buffer[512] = {0};
	BYTE pSendBuf[1024];
	ZeroMemory(pSendBuf, 255);
	pSendBuf[0] = 0x66;
	memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
	int nSendLen = 17;
	int time_out=0;
	int row_flags=m_flexGrid.get_Rows()-1;
	CString stroldipaddress,strnewipadress,strlocalipaddress,strnewsubnet,strnewgateway;
	GetIPMaskGetWay(strlocalipaddress,strnewsubnet,strnewgateway);
	while(row_flags>=1)  // 超时结束
	{
		USES_CONVERSION;
		//strnewipadress=m_flexGrid.get_TextMatrix(row_flags,NEW_IPADRESS);
		stroldipaddress=m_flexGrid.get_TextMatrix(row_flags,SCAN_TABLE_ADDRESS);
		ChangeNetDeviceIP(stroldipaddress,row_flags);
		--row_flags;
// 		if (strnewipadress.GetLength()==0)
// 		{
// 			continue;
// 		}
// 		LPCSTR szIP = W2A(stroldipaddress);
// 		DWORD dwIP = inet_addr(szIP);
// 		IN_ADDR ia;
// 		ia.S_un.S_addr = dwIP;
// 		//////////////////Old IP////////////////////////////////////
// 		pSendBuf[1]=ia.S_un.S_un_b.s_b1;
// 		pSendBuf[2]=ia.S_un.S_un_b.s_b2;
// 		pSendBuf[3]=ia.S_un.S_un_b.s_b3;
// 		pSendBuf[4]=ia.S_un.S_un_b.s_b4;
// 		///////////////////New IP///////////////////////////////////////////
// 		szIP = W2A(strnewipadress);
// 		dwIP = inet_addr(szIP);
// 		ia.S_un.S_addr = dwIP;
// 		///////////////////////////////////////////////////////////
// 		pSendBuf[5]=ia.S_un.S_un_b.s_b1;
// 		pSendBuf[6]=ia.S_un.S_un_b.s_b2;
// 		pSendBuf[7]=ia.S_un.S_un_b.s_b3;
// 		pSendBuf[8]=ia.S_un.S_un_b.s_b4;
// 		////////////////////////////////////////////////////////////////////
// 		szIP = W2A(strnewsubnet);
// 		dwIP = inet_addr(szIP);
// 		ia.S_un.S_addr = dwIP;
// 		pSendBuf[9]=ia.S_un.S_un_b.s_b1;
// 		pSendBuf[10]=ia.S_un.S_un_b.s_b2;
// 		pSendBuf[11]=ia.S_un.S_un_b.s_b3;
// 		pSendBuf[12]=ia.S_un.S_un_b.s_b4;
// 		////////////////////////////////////////////////////////////////////
// 		szIP = W2A(strnewgateway);
// 		dwIP = inet_addr(szIP);
// 		ia.S_un.S_addr = dwIP;
// 		pSendBuf[13]=ia.S_un.S_un_b.s_b1;
// 		pSendBuf[14]=ia.S_un.S_un_b.s_b2;
// 		pSendBuf[15]=ia.S_un.S_un_b.s_b3;
// 		pSendBuf[16]=ia.S_un.S_un_b.s_b4;
// 
// 		FD_ZERO(&fdSocket);	
// 		FD_SET(h_Broad, &fdSocket);
// 		nRet = ::sendto(h_Broad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast,sizeof(h_bcast));
// 		if (nRet == SOCKET_ERROR)
// 		{
// 			int  nError = WSAGetLastError();
// 			goto END_SCAN;
// 			return ;
// 		}
// 		int nLen = sizeof(h_siBind);
// 		//while(pScanner->IsComScanRunning())
// 
// 		fd_set fdRead = fdSocket;
// 		int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);
// 		if (nSelRet == SOCKET_ERROR)
// 		{
// 			int nError = WSAGetLastError();
// 			goto END_SCAN;
// 			return  ;
// 		}
// 
// 		if(nSelRet > 0)
// 		{
// 			ZeroMemory(buffer, 512);
// 
// 			int nRet = ::recvfrom(h_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);
// 			//			int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
// 			BYTE szIPAddr[4] = {0};
// 			if(nRet > 0)
// 			{		
// 				FD_ZERO(&fdSocket);
// 				if(buffer[0]==0x67)//收到正确的回复了
// 				{	
// 					for (int i=0;i<10;i++)
// 					{
// 						COLORREF ref=RGB(255,255,255); 
// 						m_flexGrid.put_Row(row_flags+1);
// 						m_flexGrid.put_Col(i);
// 						m_flexGrid.put_CellBackColor(ref);
// 					}
// 					m_flexGrid.put_TextMatrix(row_flags+1,SCAN_TABLE_ADDRESS,strnewipadress);
// 					SaveNewIPAddress(strnewipadress,stroldipaddress);
// 					AfxMessageBox(_T("Change the ip successfully!"));
// 				}	
// 
// 			}
// 		}	
// 		else
// 		{
// 
// 
// 
// 		}
	}
	if (m_IsScan)
	{
		m_pScanner->m_bCheckSubnetFinish=TRUE;
	}

	// 	OnClose();
// END_SCAN:
// 
// 	closesocket(h_Broad);
// 	h_Broad=NULL;
// 	{
// 
// 		//SOCKET soAck =::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
// 		h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
// 		BOOL bBroadcast=TRUE;
// 		::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
// 		int iMode=1;
// 		ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);
// 
// 		BOOL bDontLinger = FALSE;
// 		setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );
// 
// 		//SOCKADDR_IN bcast;
// 		h_bcast.sin_family=AF_INET;
// 		//bcast.sin_addr.s_addr=nBroadCastIP;
// 		h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
// 		h_bcast.sin_port=htons(UDP_BROADCAST_PORT);
// 
// 		//SOCKADDR_IN siBind;
// 		h_siBind.sin_family=AF_INET;
// 		h_siBind.sin_addr.s_addr=INADDR_ANY;
// 		h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
// 		::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
// 
// 	}



	//############################

	//	return 1;


}


BOOL CScanDlg::ChangeNetDeviceIP(CString strIP,int row_flags ){
BOOL ret=FALSE;
SOCKET sListen=NULL;
CString strlog;
int nRet = 0;
short nmsgType=UPD_BROADCAST_QRY_MSG;
const DWORD END_FLAG = 0x00000000;
TIMEVAL time;
time.tv_sec =3;
time.tv_usec = 1000;
fd_set fdSocket;
BYTE buffer[512] = {0};
BYTE pSendBuf[1024];
ZeroMemory(pSendBuf, 255);
pSendBuf[0] = 0x66;
memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
int nSendLen = 17;
int time_out=0;
USES_CONVERSION;
CString stroldipaddress,strnewipadress,strlocalipaddress,strnewsubnet,strnewgateway;
stroldipaddress=strIP;


	for (int i=0;i<allsubnets.size();i++)
	{
		GetNewIP(strnewipadress,allsubnets[i].StrIP);
		if (strnewipadress.Find(_T("0.0.0"))!=-1)//对0.0.0.0的过滤掉
		{
			continue;
		}

		LPCSTR szIP = W2A(stroldipaddress);
		DWORD dwIP = inet_addr(szIP);
		IN_ADDR ia;
		ia.S_un.S_addr = dwIP;
		//////////////////Old IP////////////////////////////////////
		pSendBuf[1]=ia.S_un.S_un_b.s_b1;
		pSendBuf[2]=ia.S_un.S_un_b.s_b2;
		pSendBuf[3]=ia.S_un.S_un_b.s_b3;
		pSendBuf[4]=ia.S_un.S_un_b.s_b4;
		///////////////////New IP///////////////////////////////////////////
		szIP = W2A(strnewipadress);
		dwIP = inet_addr(szIP);
		ia.S_un.S_addr = dwIP;
		///////////////////////////////////////////////////////////
		pSendBuf[5]=ia.S_un.S_un_b.s_b1;
		pSendBuf[6]=ia.S_un.S_un_b.s_b2;
		pSendBuf[7]=ia.S_un.S_un_b.s_b3;
		pSendBuf[8]=ia.S_un.S_un_b.s_b4;
		////////////////////////////////////////////////////////////////////
		szIP = W2A(strnewsubnet);
		dwIP = inet_addr(szIP);
		ia.S_un.S_addr = dwIP;
		pSendBuf[9]=ia.S_un.S_un_b.s_b1;
		pSendBuf[10]=ia.S_un.S_un_b.s_b2;
		pSendBuf[11]=ia.S_un.S_un_b.s_b3;
		pSendBuf[12]=ia.S_un.S_un_b.s_b4;
		////////////////////////////////////////////////////////////////////
		szIP = W2A(strnewgateway);
		dwIP = inet_addr(szIP);
		ia.S_un.S_addr = dwIP;
		pSendBuf[13]=ia.S_un.S_un_b.s_b1;
		pSendBuf[14]=ia.S_un.S_un_b.s_b2;
		pSendBuf[15]=ia.S_un.S_un_b.s_b3;
		pSendBuf[16]=ia.S_un.S_un_b.s_b4;

		FD_ZERO(&fdSocket);	
		FD_SET(h_Broad, &fdSocket);
		nRet = ::sendto(h_Broad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast,sizeof(h_bcast));
		if (nRet == SOCKET_ERROR)
		{
			int  nError = WSAGetLastError();
			goto END_SCAN;
			return FALSE ;
		}
		int nLen = sizeof(h_siBind);
		//while(pScanner->IsComScanRunning())

		fd_set fdRead = fdSocket;
		int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);
		if (nSelRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			goto END_SCAN;
			return FALSE ;
		}

		if(nSelRet > 0)
		{
			ZeroMemory(buffer, 512);

			int nRet = ::recvfrom(h_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);
			//			int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
			BYTE szIPAddr[4] = {0};
			if(nRet > 0)
			{		
				FD_ZERO(&fdSocket);
				if(buffer[0]==0x67)//收到正确的回复了
				{	

				    Sleep(8000);
					//if (!TestPing(strnewipadress))
					//{
					//   stroldipaddress=strnewipadress;
					//	continue;
					//}
					SetCommunicationType(1);
					 if (!Open_Socket2(strnewipadress,_wtoi(m_net_product_node.BuildingInfo.strIpPort)))
					 {
						 stroldipaddress=strnewipadress;
						 continue;
					 }
					 else
					 {
						 close_com();
					 }

					for (int i=0;i<10;i++)
					{
						COLORREF ref=RGB(255,255,255); 
						m_flexGrid.put_Row(row_flags);
						m_flexGrid.put_Col(i);
						m_flexGrid.put_CellBackColor(ref);
					}
					m_flexGrid.put_TextMatrix(row_flags,NEW_IPADRESS,strnewipadress);
					SaveNewIPAddress(strnewipadress,stroldipaddress);
					//AfxMessageBox(_T("Change the ip successfully!"));
					ret=TRUE;
					break;
				}	

			}
			else
			{
				if (!TestPing(strnewipadress))
				{
					stroldipaddress=strnewipadress;
					continue;
				}
			}
		}	
		else
		{



		}


	}

	return ret;
END_SCAN:

	closesocket(h_Broad);
	h_Broad=NULL;
	{

		//SOCKET soAck =::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		BOOL bBroadcast=TRUE;
		::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
		int iMode=1;
		ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

		BOOL bDontLinger = FALSE;
		setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );

		//SOCKADDR_IN bcast;
		h_bcast.sin_family=AF_INET;
		//bcast.sin_addr.s_addr=nBroadCastIP;
		h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
		h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

		//SOCKADDR_IN siBind;
		h_siBind.sin_family=AF_INET;
		h_siBind.sin_addr.s_addr=INADDR_ANY;
		h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
		::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));

	}
	return ret; 
}			
void CScanDlg::InitScanGrid()
{
	//m_flexGrid.put_TextMatrix(0,0,_T("NO."));
	m_flexGrid.put_TextMatrix(0,0,_T("Model"));
	m_flexGrid.put_TextMatrix(0,1,_T("Building"));
	m_flexGrid.put_TextMatrix(0,2,_T("Floor"));
	m_flexGrid.put_TextMatrix(0,3,_T("Room"));
	m_flexGrid.put_TextMatrix(0,4,_T("Sub_net"));
	m_flexGrid.put_TextMatrix(0,5,_T("Serial#"));
	m_flexGrid.put_TextMatrix(0,6,_T("Address"));
	m_flexGrid.put_TextMatrix(0,7,_T("Port"));
	m_flexGrid.put_TextMatrix(0,8,_T("Protocol"));
	
	if (!m_IsScan)
	{
		m_flexGrid.put_Cols(10);
		m_flexGrid.put_TextMatrix(0,9,_T("Assign New Ip"));
		GetDlgItem(IDC_BUTTON_SCANALL)->ShowWindow(SW_SHOW);


		for(int i = 0; i < 9; i++)
		{
			m_flexGrid.put_ColAlignment(i,4);
		}

		m_flexGrid.put_ColWidth(0,2000);	//type
		m_flexGrid.put_ColWidth(1,0);		//building
		m_flexGrid.put_ColWidth(2,0);		//floor
		m_flexGrid.put_ColWidth(3,0);		//room
		m_flexGrid.put_ColWidth(4,0);		//subnet

		m_flexGrid.put_ColWidth(5,1000);		//serial#
		m_flexGrid.put_ColWidth(6,1400);	//Address
		m_flexGrid.put_ColWidth(7,800);		//Port
		m_flexGrid.put_ColWidth(8,0);	//protocol
		m_flexGrid.put_ColWidth(9,1200);


	}
	else
	{
		ASSERT(m_pScanner);	
		if (!m_pScanner->m_thesamesubnet)
		{
			m_flexGrid.put_Cols(10);
			
			m_flexGrid.put_TextMatrix(0,9,_T("Assign New Ip"));
			GetDlgItem(IDC_BUTTON_SCANALL)->ShowWindow(SW_SHOW);
			
		}
		else
		{
			GetDlgItem(IDC_BUTTON_SCANALL)->ShowWindow(SW_HIDE);
		}

		for(int i = 0; i < 9; i++)
		{
			m_flexGrid.put_ColAlignment(i,4);
		}

		m_flexGrid.put_ColWidth(0,2000);	//type
		m_flexGrid.put_ColWidth(1,800);		//building
		m_flexGrid.put_ColWidth(2,800);		//floor
		m_flexGrid.put_ColWidth(3,800);		//room
		m_flexGrid.put_ColWidth(4,800);		//subnet

		m_flexGrid.put_ColWidth(5,1000);		//serial#
		m_flexGrid.put_ColWidth(6,1400);	//Address
		m_flexGrid.put_ColWidth(7,800);		//Port
		m_flexGrid.put_ColWidth(8,1200);	//protocol

	}
	// 	m_flexGrid.put_TextMatrix(0,8,_T("Confilct"));
	// 	m_flexGrid.put_TextMatrix(0,9,_T("Fix Conflict"));

	

// 	m_flexGrid.put_ColWidth(8,600);		//if conflict
// 	m_flexGrid.put_ColWidth(9,800);		//fix
	
}

void CScanDlg::SetExsit(bool ref_exsit)
{
	exsit_serial = ref_exsit;
}

void CScanDlg::FLEX_GRID_PUT_COLOR_STR(int row,int col,CString str)
{
	COLORREF ref;
	if (exsit_serial)
		ref = exsit_ref;// RGB(178, 227, 137);
	else
		ref = new_ref;
	m_flexGrid.put_TextMatrix(row,col,str);
	m_flexGrid.put_Row(row);
	m_flexGrid.put_Col(col);
	m_flexGrid.put_CellBackColor(ref);
}

void CScanDlg::AddNetDeviceToGrid()
{
	//EnterCriticalSection(&m_csGrid);
	int nRSize = m_flexGrid.get_Rows();
	int nSize = m_refresh_net_device_data.size();// szList.size();

	m_flexGrid.put_Rows(nSize + nRSize);

	for (UINT i = 0; i < m_refresh_net_device_data.size(); i++)
	{
		CString strserialnum;
		strserialnum.Format(_T("%u"), m_refresh_net_device_data.at(i).nSerial);

		//m_pScanner->exsit_serial_array
		bool temp_exsit = false;
		for (int j = 0;j < m_pScanner->serial_array_length;j++)
		{
			if (m_refresh_net_device_data.at(i).nSerial == m_pScanner->exsit_serial_array[j])
			{
				temp_exsit = true;
				break;
			}
		}
		if (temp_exsit)
		{
			exsit_serial = true;
		}
		else
		{
			exsit_serial = false;
		}

		FLEX_GRID_PUT_COLOR_STR(i + nRSize, SCAN_TABLE_SERIALID, strserialnum);

		CString strType = m_refresh_net_device_data.at(i).show_label_name;		
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_TYPE,strType); 

		CString strbuilding = _T("Default_Building");		
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_BUILDING,strbuilding); 

		CString strfloor = _T("Floor1");		
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_FLOOR,strfloor); 

		CString strroom = _T("Room1");		
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ROOM,strroom); 


		CString strsubnet = _T("Sub_net1");		
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SUBNET,strsubnet); 



		CString strip = m_refresh_net_device_data.at(i).ip_address;
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ADDRESS,strip); 
		
		CString strport;
		strport.Format(_T("%u"),m_refresh_net_device_data.at(i).nport);
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_COMPORT,strport); 

		CString strprotocol;
		strprotocol = _T("TCP/IP");
		FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_PROTOCOL,strprotocol); 
	}
#if 0
		_NetDeviceInfo* pNetInfo = szList[i];
		DWORD dwIP =pNetInfo->m_pNet->GetIPAddr();	
		in_addr ad;
		ad.S_un.S_addr = dwIP;
		CString strAddr(inet_ntoa(ad));
		BOOL thesamesubnet=CheckTheSameSubnet(strAddr);
		if (!thesamesubnet)
		{   
			CString anewip;
			if (GetNewIP(anewip))
			{
				FLEX_GRID_PUT_COLOR_STR(i+nRSize,NEW_IPADRESS,anewip); 
			}
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ADDRESS,strAddr); 
			CString strType = pNetInfo->m_pNet->GetProductName();		
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_TYPE,strType); 

			CString strBuilding = pNetInfo->m_pNet->GetBuildingName();		
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_BUILDING,strBuilding); 

			CString strFloor = pNetInfo->m_pNet->GetFloorName();
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_FLOOR,strFloor); 

			CString strRoom = pNetInfo->m_pNet->GetRoomName();
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ROOM,strRoom); 

			CString strSubnet = pNetInfo->m_pNet->GetSubnetName();
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SUBNET,strSubnet); 

			CString strSerailID;
			int nSID = pNetInfo->m_pNet->GetSerialID();
			strSerailID.Format(_T("%d"), nSID);
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SERIALID,strSerailID); 
			/////

			/////
			CString strPort;
			int nPort = pNetInfo->m_pNet->GetIPPort();
			strPort.Format(_T("%d"), nPort);
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_COMPORT,strPort); 
			////
			CString strProtocol;		
			if(pNetInfo->m_pNet->GetProtocol() == 3)
			{
				strProtocol.Format(_T("BacnetIP"));
			}
			else
				strProtocol.Format(_T("TCP/IP"));
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_PROTOCOL,strProtocol); 
			return;
		} 

		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ADDRESS,strAddr); 
		CString strType = pNetInfo->m_pNet->GetProductName();		
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_TYPE,strType); 

		CString strBuilding = pNetInfo->m_pNet->GetBuildingName();		
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_BUILDING,strBuilding); 

		CString strFloor = pNetInfo->m_pNet->GetFloorName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_FLOOR,strFloor); 

		CString strRoom = pNetInfo->m_pNet->GetRoomName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ROOM,strRoom); 

		CString strSubnet = pNetInfo->m_pNet->GetSubnetName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_SUBNET,strSubnet); 

		CString strSerailID;
		int nSID = pNetInfo->m_pNet->GetSerialID();
		strSerailID.Format(_T("%d"), nSID);
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_SERIALID,strSerailID); 
		/////
		dwIP =pNetInfo->m_pNet->GetIPAddr();				

		ad.S_un.S_addr = dwIP;
		CString Addr(inet_ntoa(ad));
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ADDRESS,Addr); 
		/////
		CString strPort;
		int nPort = pNetInfo->m_pNet->GetIPPort();
		strPort.Format(_T("%d"), nPort);
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_COMPORT,strPort); 
		////
		CString strProtocol;		
		if(pNetInfo->m_pNet->GetProtocol() == 3)
		{
			strProtocol.Format(_T("BacnetIP"));
		}
		else
			strProtocol.Format(_T("TCP/IP"));
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_PROTOCOL,strProtocol); 
		////
		// CString strConflict;
	}
#endif
	//LeaveCriticalSection(&m_csGrid);
}

#if 0
void CScanDlg::AddNetDeviceToGrid(vector<_NetDeviceInfo*>& szList)
{
	//EnterCriticalSection(&m_csGrid);
	int nRSize = m_flexGrid.get_Rows();
	int nSize = szList.size();

	m_flexGrid.put_Rows(nSize + nRSize);

	for (UINT i = 0; i < szList.size(); i++)
	{
		_NetDeviceInfo* pNetInfo = szList[i];
		DWORD dwIP =pNetInfo->m_pNet->GetIPAddr();	
		in_addr ad;
		ad.S_un.S_addr = dwIP;
		CString strAddr(inet_ntoa(ad));
		BOOL thesamesubnet=CheckTheSameSubnet(strAddr);
		if (!thesamesubnet)
		{   
			CString anewip;
			if (GetNewIP(anewip))
			{
				FLEX_GRID_PUT_COLOR_STR(i+nRSize,NEW_IPADRESS,anewip); 
			}
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ADDRESS,strAddr); 
			CString strType = pNetInfo->m_pNet->GetProductName();		
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_TYPE,strType); 

			CString strBuilding = pNetInfo->m_pNet->GetBuildingName();		
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_BUILDING,strBuilding); 

			CString strFloor = pNetInfo->m_pNet->GetFloorName();
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_FLOOR,strFloor); 

			CString strRoom = pNetInfo->m_pNet->GetRoomName();
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_ROOM,strRoom); 

			CString strSubnet = pNetInfo->m_pNet->GetSubnetName();
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SUBNET,strSubnet); 

			CString strSerailID;
			int nSID = pNetInfo->m_pNet->GetSerialID();
			strSerailID.Format(_T("%d"), nSID);
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_SERIALID,strSerailID); 
			/////

			/////
			CString strPort;
			int nPort = pNetInfo->m_pNet->GetIPPort();
			strPort.Format(_T("%d"), nPort);
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_COMPORT,strPort); 
			////
			CString strProtocol;		
			if(pNetInfo->m_pNet->GetProtocol() == 3)
			{
				strProtocol.Format(_T("BacnetIP"));
			}
			else
				strProtocol.Format(_T("TCP/IP"));
			FLEX_GRID_PUT_COLOR_STR(i+nRSize,SCAN_TABLE_PROTOCOL,strProtocol); 
			return;
		} 
		 
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ADDRESS,strAddr); 
		CString strType = pNetInfo->m_pNet->GetProductName();		
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_TYPE,strType); 

		CString strBuilding = pNetInfo->m_pNet->GetBuildingName();		
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_BUILDING,strBuilding); 

		CString strFloor = pNetInfo->m_pNet->GetFloorName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_FLOOR,strFloor); 

		CString strRoom = pNetInfo->m_pNet->GetRoomName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ROOM,strRoom); 

		CString strSubnet = pNetInfo->m_pNet->GetSubnetName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_SUBNET,strSubnet); 

		CString strSerailID;
		int nSID = pNetInfo->m_pNet->GetSerialID();
		strSerailID.Format(_T("%d"), nSID);
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_SERIALID,strSerailID); 
		/////
		 dwIP =pNetInfo->m_pNet->GetIPAddr();				
		
		ad.S_un.S_addr = dwIP;
		CString Addr(inet_ntoa(ad));
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ADDRESS,Addr); 
		/////
		CString strPort;
		int nPort = pNetInfo->m_pNet->GetIPPort();
		strPort.Format(_T("%d"), nPort);
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_COMPORT,strPort); 
		////
		CString strProtocol;		
		if(pNetInfo->m_pNet->GetProtocol() == 3)
		{
			strProtocol.Format(_T("BacnetIP"));
		}
		else
		strProtocol.Format(_T("TCP/IP"));
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_PROTOCOL,strProtocol); 
		////
		// CString strConflict;
	}

	//LeaveCriticalSection(&m_csGrid);
}
#endif
void CScanDlg::ChangeIPAddress(CString newip,CString oldip){
	USES_CONVERSION;
for (UINT i=0;i<m_pScanner->m_szNCScanRet.size();i++)
{
	_NetDeviceInfo* pNetInfo = m_pScanner->m_szNCScanRet[i];
	DWORD dwIP =pNetInfo->m_pNet->GetIPAddr();	
	in_addr ad;
	ad.S_un.S_addr = dwIP;
	CString strAddr(inet_ntoa(ad));
	if (strAddr.CompareNoCase(oldip)==0)
	{
		dwIP=inet_addr(W2A(newip));
		m_pScanner->m_szNCScanRet[i]->m_pNet->SetIPAddr(dwIP);
	}
}
}
void CScanDlg::SaveNewIPAddress(CString newip,CString oldip){
 
	CppSQLite3Table table;
	CppSQLite3Query q;

	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	try 
	{
		CString strSql;
		 
		strSql.Format(_T("select * from ALL_NODE where Bautrate='%s' "),oldip);
 
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
		while(!q.eof())
		{
			strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Bautrate='%s'"),newip,oldip);
			 SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			q.nextRow();
		}
		 

		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
	}
	catch(_com_error e)
	{
		 
	}
	SqliteDBBuilding.closedb();
}
int CScanDlg::GetAllNodeFromDataBase()
{
	 
	CppSQLite3Table table;
	CppSQLite3Query q;

	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	

	_variant_t temp_variant;
	CString strTemp;

	CString temp_str=_T("select * from ALL_NODE");
    q = SqliteDBBuilding.execQuery((UTF8MBSTR)temp_str);

	//return m_pRs->get_RecordCount();

	int nTemp = 0;
	while(!q.eof())
	{
		nTemp++;
		int nDefault=0;
		CString strDevType = q.getValuebyName(L"Product_class_ID");

		CTStatBase* pNode = NULL;
		if (strDevType.CompareNoCase(_T("100")) == 0)	 //100 =  NC
		{
			pNode = new CTStat_Net;
			// IP Addr
			CString strIP = q.getValuebyName(L"Product_ID");
			//((CTStat_Net*)(pNode))->SetIPAddr(strIP);
			
			// Port
			CString strPort = q.getValuebyName(L"Bautrate");
			((CTStat_Net*)(pNode))->SetIPAddr(_wtoi(strPort));

			m_szNetNodes.push_back(((CTStat_Net*)(pNode)));
		}
		else
		{
			pNode = new CTStat_Dev;
			// BaudRate
			CString strBaudRate = q.getValuebyName(L"Bautrate");
			((CTStat_Dev*)(pNode))->SetBaudRate(_wtoi(strBaudRate));

			m_szComNodes.push_back(((CTStat_Dev*)(pNode)));

			CString strComPort = q.getValuebyName(L"Com_Port");
			strComPort = strComPort.Mid(3);
			((CTStat_Dev*)(pNode))->SetComPort(_wtoi(strComPort));

			CString strEPSize = q.getValuebyName(L"EPsize");
			((CTStat_Dev*)(pNode))->SetEPSize(int(_wtoi(strEPSize)));

		}
		pNode->SetProductType(_wtoi(strDevType));

		pNode->SetBuildingName(q.getValuebyName(L"MainBuilding_Name"));
		pNode->SetSubnetName(q.getValuebyName(L"Building_Name"));

		pNode->SetFloorName(q.getValuebyName(L"Floor_Name"));
		pNode->SetRoomName(q.getValuebyName(L"Room_Name"));

		CString strID = q.getValuebyName(L"Product_ID");		
		pNode->SetDevID(_wtoi(strID));
		
		CString strHwv = q.getValuebyName(L"Hardware_Ver");
		pNode->SetHardwareVersion(float(_wtof(strHwv)));
		CString strSwv = q.getValuebyName(L"Software_Ver");
		pNode->SetSoftwareVersion(float(_wtof(strSwv)));
	




		CString strSerialID =q.getValuebyName(L"Serial_ID");		
		(pNode)->SetSerialID(_wtoi(strSerialID));
		
		q.nextRow();		
	}
	SqliteDBBuilding.closedb();
	return m_szNetNodes.size() + m_szComNodes.size();
}

void CScanDlg::SetNode(tree_product product_Node){
m_net_product_node=product_Node;
}
void CScanDlg::Set_IsScan(BOOL Is_Scan){
	m_IsScan=Is_Scan;
}

LRESULT CScanDlg::OnAddComScanRet(WPARAM wParam, LPARAM lParam)
{
	//AddComDeviceToGrid(m_pScanner->m_szTsatScandRet);
	return 1;
}

LRESULT CScanDlg::OnAddNetScanRet(WPARAM wParam, LPARAM lParam)
{
	//AddNetDeviceToGrid(m_pScanner->m_szNCScanRet);
	return 1;
}




LRESULT CScanDlg::OnScanFinish(WPARAM wParam, LPARAM lParam)
{

	AddComDeviceToGrid(m_pScanner->m_szTstatScandRet);
	//AddNetDeviceToGrid(m_pScanner->m_szNCScanRet);
	AddNetDeviceToGrid();

	//OpenDefaultCom();
	return 1;
}



// do something clean, call it before scan every time.
void CScanDlg::Release()
{	
	for (UINT i = 0; i < m_szComNodes.size(); i++)
	{
		delete m_szComNodes[i];
	}
	m_szComNodes.clear();
	for (UINT i = 0; i < m_szNetNodes.size(); i++)
	{
		delete m_szNetNodes[i];
	}
	m_szNetNodes.clear();


// 	for (UINT i = 0; i < m_szNCScanRet.size(); i++)
// 	{
// 		delete m_szNCScanRet[i];
// 	}
// 	m_szNCScanRet.clear();
// 
// 	for (UINT i = 0; i < m_szTSScanRet.size(); i++)
// 	{
// 		delete m_szTSScanRet[i];
// 	}
// 	m_szTSScanRet.clear();


	//vector<Conflict_Groups*>	m_szConflictNet;
	//vector<Conflict_Groups*>	m_szConflictCom;
	
// 	if (m_pScanner != NULL)
// 	{
// 		m_pScanner->Release();
// 		delete m_pScanner;
// 		m_pScanner = NULL;
// 	}
}


 

CTStat_Dev* CScanDlg::FindComDeviceBySerialIDInDB(DWORD dwSerialID)
{
	for (UINT i = 0 ; i < m_szComNodes.size(); i++)
	{
		CTStat_Dev* pDev = m_szComNodes[i];
		if (dwSerialID ==  pDev->GetSerialID())
		{
			return pDev;
		}
	}
	return NULL;
}


CTStat_Net* CScanDlg::FindNetDeviceBySerialIDInDB(DWORD dwSerialID)
{
	for (UINT i = 0 ; i < m_szNetNodes.size(); i++)
	{
		CTStat_Net* pNet = m_szNetNodes[i];
		if (dwSerialID ==  pNet->GetSerialID())
		{
			return pNet;
		}
	}

	return NULL;
}


void CScanDlg::SetGridCellColor(int nRow, int nCol, COLORREF clr)
{
	m_flexGrid.put_Row(nRow);
	m_flexGrid.put_Col(nCol);
	m_flexGrid.put_CellBackColor(clr);
}


void CScanDlg::OnClose()
{
	 

	//RecoveryID();
	if (!m_IsScan)
	{
		CDialog::OnClose();
		return;
	}
	if (!m_pScanner->m_bCheckSubnetFinish)
	{
		int ret1 = AfxMessageBox(_T("Are you sure to exit?The devices are exited,which are in the different subnet.please apply all new net ips."),MB_YESNOCANCEL,3); 
		if (ret1 == IDYES)
		{
			Release();

			CDialog::OnClose(); 
		}
	}
	else
	{
		Release();

		CDialog::OnClose();
	}
	
	
}



// 
// // 在指定位置画button
// void CScanDlg::DrawButtonOnGrid(int iRow, int iCol)
// {
// 	CRect rcCell = CalcGridCellRect(iRow, iCol);
//  	CButton*  pBtnTemp = (CButton*)GetDlgItem(IDC_BUTTON_AUTO);
// 
// // 	pBtn->MoveWindow(rcCell,1);
// // 	pBtn->SetFocus();
// // 	pBtn->BringWindowToTop();
// 
// 
// 	CGridButton*  pBtn = new CGridButton;
// 	pBtn->Create(_T("Fix"), BS_CENTER, rcCell, this, 5000);
// 	pBtn->SetFont(pBtnTemp->GetFont(), TRUE);
// 	pBtn->SetPosition(iRow, iCol);
// 	
// 	// 	pBtn->MoveWindow(rcCell,1);
// 	 	pBtn->SetFocus();
// 	 	pBtn->BringWindowToTop();
// 		pBtn->ShowWindow(SW_NORMAL);
// 		m_szGridBtns.push_back(pBtn);
// }


BOOL CScanDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	
return CDialog::OnCommand(wParam, lParam);
}


void CScanDlg::ShowInputEditBox(int iRow, int iCol)
{	
//	int iRow,iCol;
	m_editGrid.SetWindowText(_T(""));
	CRect rc = CalcGridCellRect(iRow, iCol);  // 肯定ID列
	m_editGrid.MoveWindow(rc, TRUE);
	m_editGrid.SetFocus();
	m_editGrid.BringWindowToTop();
	m_editGrid.ShowWindow(SW_NORMAL);
	m_szGridEditPos.cx = iRow;
	m_szGridEditPos.cy = iCol;
	
}


CRect CScanDlg::CalcGridCellRect(int iRow, int iCol )
{
	CRect rect;
	m_flexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC = GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_flexGrid.get_RowPos(iRow)/nTwipsPerDotY;
	long x = m_flexGrid.get_ColPos(iCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_flexGrid.get_ColWidth(iCol)/nTwipsPerDotX+1;
	long height = m_flexGrid.get_RowHeight(iRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+2,rect.top+2);	
	rcCell.InflateRect(-1,-1,-1,-1);
	//CString strValue = m_flexGrid.get_TextMatrix(iRow,iCol);

	//pDC->Draw3dRect(rcCell, RGB(255,0,0), RGB(192,192,192));
	//pDC->DrawText(_T("Fix"), &rcCell, DT_CENTER);

	return rcCell;
	ReleaseDC(pDC);

}


BEGIN_EVENTSINK_MAP(CScanDlg, CDialog)
ON_EVENT(CScanDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CScanDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CScanDlg::ClickMsflexgrid1()
{

#if 1
	long lRow,lCol;
	lRow = m_flexGrid.get_RowSel();//获取点击的行号	
	lCol = m_flexGrid.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));
	m_currow=lRow;
	m_curcol=lCol;
	CRect rect;
	m_flexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_flexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_flexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_flexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_flexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_flexGrid.get_TextMatrix(lRow,lCol);
	if (lCol==9)
	{
		m_editGrid.MoveWindow(&rcCell,1);
		m_editGrid.ShowWindow(SW_SHOW);
		m_editGrid.SetWindowText(strValue);
		m_editGrid.SetFocus();
		m_editGrid.SetCapture();//LSC
		int nLenth=m_editGrid.GetLineCount();//m_editGrid.GetLength();
		m_editGrid.SetSel(nLenth,nLenth); //全选//
		return;
		
	}
#endif
	CSize szTemp;
	CalcClickPos(szTemp);// 计算点中的位置
	if (szTemp == m_szGridEditPos)
	{
		return;	
	}
	
	
	// 3种情况
	// 1，点在其他有效处，显示，记录，数据并移动edit
	// 2，点在无效处，并失去焦点，关掉edit，显示，记录，数据
	// 3，退出，关掉edit，显示，记录，数据，并保存至数据库

	if (IsValidClick(szTemp))  
	{	
		if(m_szGridEditPos.cx != -1) // 不是第一次点击才需要记录上一次的数据
		{
			GetGridEditString();    // 记录edit的数据			
		}
		ShowInputEditBox(szTemp.cx, szTemp.cy);		
		m_szGridEditPos = szTemp;							
	}	
	else// 点在不相干的地方就 destroy
	{	
		GetGridEditString();    // 记录edit的数据
		DestroyFlexEdit();	     
	}
// 	if(GetFocus()->m_hWnd != m_editGrid.m_hWnd)
// 	{
// 		
// 	}



}

void CScanDlg::GetGridEditString() // 记录edit的数据
{
	CString strText ;
	m_editGrid.GetWindowText(strText)	;
	if (strText.GetLength() == 0)  // 没输入，啥也不干
	{
		return;
	}
	CTStatBase* pInfo = FindDeviceByRowNum(m_szGridEditPos.cx);
	if (pInfo == NULL)
	{
		ASSERT(pInfo);
		return;
	}
	switch(m_szGridEditPos.cy)
	{
	case 1:
		pInfo->SetBuildingName(strText);
		break;
	case 2:
		pInfo->SetFloorName(strText);
		break;
	case 3:
		pInfo->SetRoomName(strText);
		break;
	default:
		break;
	}

	m_flexGrid.put_TextMatrix(m_szGridEditPos.cx, m_szGridEditPos.cy, strText);
}


//////////////////////////////////////////////////////////////////////////
// return 1, 正常 
// return 0, 无效点击，就是不应该反应的
// return 3, 在grid之外或者不知道什么地方
int CScanDlg::IsValidClick(CSize szTemp)
{
	if (szTemp.cy  <= 4 && szTemp.cy >0 &&
		szTemp.cx > 0)	
	{
		return 1;
	}
	return 0;

}


void CScanDlg::CalcClickPos(CSize& size)
{
	long lRow,lCol;
	lRow = m_flexGrid.get_RowSel();//获取点击的行号	
	lCol = m_flexGrid.get_ColSel(); //获取点击的列号

	if(lRow<=0)
		return;
	size.cx=lRow;
	size.cy = lCol;
}
void CScanDlg::DestroyFlexEdit()
{
	m_editGrid.ShowWindow(SW_HIDE);	
	CString strText;
	m_editGrid.GetWindowText(strText);		
}
// 将数据库里的值取出放到内存
// 将内存的值将被写入数据库
//
// 只允许写 buildingname，floorname，roomname，address
// 因此，需要向寄存器写的只有address
void CScanDlg::GetDataFromGrid()
{
	int nCount = m_flexGrid.get_Rows();
	for(int i = 1; i < nCount; i++)
	{
// 		if ()
// 		{
// 		}
		// Building
		CString strBuildingName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_BUILDING);
		// Floor
		CString strFloorName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_FLOOR);
		// Room
		CString strRoomName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_ROOM);
		// subnet
		CString strSubnetName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_SUBNET);
		// Serial#
		CString strSerialID = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_SERIALID);
		int nSerialID = _wtoi(strSerialID);
		// Address
		//CString strAddress = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_ADDRESS);
		//int nAddress = _wtoi(strAddress);
		// Com/IP
		//CString strCom = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_COMPORT);
		// BaudRate/Port
		//CString strPort = m_flexGrid.get_TextMatrix(i, 6);
		//int nPort = _wtoi(nPort);
		// Protocol		
		
		CTStatBase* pInfo = FindDeviceByRowNum(i);
		ASSERT(pInfo);
		pInfo->SetBuildingName(strBuildingName);
		pInfo->SetFloorName(strFloorName);
		pInfo->SetRoomName(strRoomName);
		pInfo->SetSubnetName(strSubnetName);
		//pInfo->m_pDev->SetDevID(nAddress);
		//Write_One(pInfo->m_pDev->GetDevID(), 6, nAddress);  // 此时写ID到寄存器	
	}
}

// 将所有节点保存到数据库，AllNodes
void CScanDlg::SaveAllNodeToDB()
{
	GetAllNodeFromDataBase();
	GetDataFromGrid();	
	CombineDBandScanRet();

 

	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	try
	{

	CString strSql;
	strSql.Format(_T("delete   from ALL_NODE"));
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	for (UINT i = 0; i < m_pScanner->m_szTstatScandRet.size(); i++)
	{
		CTStat_Dev* pDevInfo = m_pScanner->m_szTstatScandRet[i]->m_pDev;
		WriteOneDevInfoToDB(pDevInfo);
	}

	for (UINT i = 0; i < m_pScanner->m_szNCScanRet.size(); i++)
	{
		CTStat_Net* pNetInfo = m_pScanner->m_szNCScanRet[i]->m_pNet;
		WriteOneNetInfoToDB(pNetInfo);
	}

	// 数据库的写回去.
	for (UINT i = 0; i < m_szComNodes.size(); i++)
	{
		CTStat_Dev* pDevInfo = m_szComNodes[i];
		WriteOneDevInfoToDB(pDevInfo);
	}

	for (UINT i = 0; i < m_szNetNodes.size(); i++)
	{
		CTStat_Net* pNetInfo = m_szNetNodes[i];
		WriteOneNetInfoToDB(pNetInfo);
	}

	SqliteDBBuilding.closedb();
 
}

// 合并数据库和scan结果，以便存入数据库
// 如果Serial ID相同的，则以grid的数据为准
void CScanDlg::CombineDBandScanRet()
{
	//vector<CTStat_Dev*> szDB;
	BOOL bFind = FALSE;
	for (UINT i = 0; i < m_pScanner->m_szTstatScandRet.size(); i++)
	{
		CTStat_Dev* pDev = m_pScanner->m_szTstatScandRet[i]->m_pDev;
		int nSID = pDev->GetSerialID();
		bFind = FALSE;
		for(UINT j = 0; j < m_szComNodes.size(); j++)
		{
			CTStat_Dev* pDBDev = m_szComNodes[j];
			int nDBSID= pDBDev->GetSerialID();
			if (nDBSID == nSID) // 相等，删掉数据库这项，保留scan 结果
			{
// 				pDBDev->SetBuildingName(pDev->GetBuildingName());
// 				pDBDev->SetFloorName(pDev->GetFloorName());
// 				pDBDev->SetRoomName(pDev->GetRoomName());
// 				pDBDev->SetSubnetName(pDBDev->GetSubnetName());
// 				//pDBDev->SetBuildingName();		
				delete pDBDev;
				pDBDev = NULL;
				m_szComNodes.erase(m_szComNodes.begin() + j);
				j--;
			}			
		}
	}	
	

	for (UINT i = 0; i < m_pScanner->m_szNCScanRet.size(); i++)
	{
		CTStat_Net* pNet = m_pScanner->m_szNCScanRet[i]->m_pNet;
		int nSID = pNet->GetSerialID();
		bFind = FALSE;
		for(UINT j = 0; j < m_szNetNodes.size(); j++)
		{
			CTStat_Net* pDBNet = m_szNetNodes[j];
			int nDBSID= pDBNet->GetSerialID();
			if (nDBSID == nSID) // 相等，赋值
			{
// 				pDBNet->SetBuildingName(pDBNet->GetBuildingName());
// 				pDBNet->SetFloorName(pDBNet->GetFloorName());
// 				pDBNet->SetRoomName(pDBNet->GetRoomName());
// 				pDBNet->SetSubnetName(pDBNet->GetSubnetName());
// 				pDBNet->SetIPAddr(pDBNet->GetIPAddr());

				delete pDBNet;
				pDBNet = NULL;
				m_szNetNodes.erase(m_szNetNodes.begin() + j);
				j--;
				//pDBDev->SetBuildingName();			
			}			
		}
	}	
}


//程序設計
void CScanDlg::WriteOneDevInfoToDB( CTStat_Dev* pDev)
{
	ASSERT(pDev);
	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	CString strBuildingName = pDev->GetBuildingName();

	CString strFloorName = pDev->GetFloorName();

	CString strRoomName = pDev->GetRoomName();	
	
	CString strID;
	int nID = pDev->GetDevID();	
	strID.Format(_T("%d"),  nID);

	CString strProductName = pDev->GetProductName();

	CString strSerialID;
	strSerialID.Format(_T("%u"), pDev->GetSerialID());



	 int nClassID = pDev->GetProductType();
	CString strClassID;
	strClassID.Format(_T("%u"), nClassID);

	int nBaudRate = pDev->GetBaudRate();
	CString strBaudRate;
	strBaudRate.Format(_T("%d"), nBaudRate);

	CString strScreenName;
	strScreenName.Format(_T("Screen(S:%d--%d)"), pDev->GetSerialID(), pDev->GetDevID() );

	CString strBackground_bmp=_T("Modbus_and_Bacnet");

	CString strHWV;
	strHWV.Format(_T("%.1f"), pDev->GetHardwareVersion());

	CString strSWV;
	strSWV.Format(_T("%.1f"), pDev->GetSoftwareVersion());

	CString strCom;
	strCom.Format(_T("COM%d"), pDev->GetComPort());
	
	CString strSql;

	CString strSubNetName=pDev->GetSubnetName();//_T("Sub_net1");

	CString strEpSize;
	strEpSize.Format(_T("%d"), pDev->GetEPSize());

	try
	{

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,Custom,EPsize) values('"
    +strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+_T(",'0'")+"','"+strEpSize+"')"));
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	SqliteDBBuilding.closedb();
}

void CScanDlg::WriteOneNetInfoToDB( CTStat_Net* pNet)
{
 	ASSERT(pNet);
 
	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	CString strBuildingName = pNet->GetBuildingName();

	CString strFloorName = pNet->GetFloorName();

	CString strRoomName = pNet->GetRoomName();	

	// Modbus ID
	CString strID;
	int nID = pNet->GetDevID();	
	strID.Format(_T("%d"),  nID);

	CString strProductName = pNet->GetProductName();

	CString strSerialID;
	strSerialID.Format(_T("%u"), pNet->GetSerialID());

	int nClassID = pNet->GetProductType();
	CString strClassID;
	strClassID.Format(_T("%u"), nClassID);

	CString strScreenName;
	strScreenName.Format(_T("Screen(S:%d--%d)"), pNet->GetSerialID(), pNet->GetDevID() );

	CString strBackground_bmp=_T("Modbus_and_Bacnet");

	CString strHWV;
	strHWV.Format(_T("%0.1f"), pNet->GetHardwareVersion());

	CString strSWV;
	strSWV.Format(_T("%0.1f"), pNet->GetSoftwareVersion());

	// 	CString strCom;
	// 	strCom.Format(_T("COM%d"), pInfo->m_pNet->GetComPort());

	CString strSql;
	CString strSubNetName = pNet->GetSubnetName();
	//CString strSubNetName=_T("Sub_net1");

	//CString strEpSize;
	//strEpSize.Format(_T("%d"), pInfo->m_pNet->GetEPSize());

	CString strIP;
	in_addr ia;
	ia.S_un.S_addr = pNet->GetIPAddr();
	strIP = CString(inet_ntoa(ia));	


	CString strPort;
	strPort.Format(_T("%d"), pNet->GetIPPort());

	CString strEPSize = _T("0");
	

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,Custom,EPsize) values('"
    +strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+_T(",'0'")+strEPSize+"')"));
	//new nc // strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"','"+strMainnetInfo+"')"));
	try
	{

		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
  SqliteDBBuilding.closedb();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


void CScanDlg::SetScanner(CTStatScanner* pScanner)
{
	m_pScanner = pScanner;
}


void CScanDlg::AddComDeviceToGrid(vector<_ComDeviceInfo*>& szList)
{
	int nSize = 1;//m_flexGrid.get_Rows();
	VARIANT vRow; 
	vRow.vt = VT_I4;//指定变量类型 
	vRow.lVal =1;     //赋值 
	//EnterCriticalSection(&m_csGrid);
	TRACE(_T("scan has found node == %d\n"), szList.size());
	m_flexGrid.put_Rows(szList.size() + nSize);
	for (UINT i = 0; i < szList.size(); i++)
	{
		//vRow.lVal = 1+i;
		//m_flexGrid.AddItem(_T(""), vRow);
		_ComDeviceInfo* pDevInfo = szList[i];
		//////////////////////////////////////////////////////////////////////////		
		// 		CString strNO;
		// 		strNO.Format(_T("%d"), i);
		// 		m_flexGrid.put_TextMatrix(i+nSize,0,strNO); 
		CString strType;
		if(pDevInfo->m_pDev->m_cus_name)
		{
			strType= pDevInfo->m_pDev->m_product_cus_name;
		}
		else
		{
			strType = pDevInfo->m_pDev->GetProductName();
			strType = strType.Left(strType.Find(_T(":")));
		}

		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_TYPE,strType); 

		CString strBuilding = pDevInfo->m_pDev->GetBuildingName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_BUILDING,strBuilding); 

		CString strFloor = pDevInfo->m_pDev->GetFloorName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_FLOOR,strFloor); 

		CString strRoom = pDevInfo->m_pDev->GetRoomName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_ROOM,strRoom); 

		CString strSubnet = 	pDevInfo->m_pDev->GetSubnetName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_SUBNET,strSubnet); 


		CString strSerialID;
		int nSID = pDevInfo->m_pDev->GetSerialID();
		strSerialID.Format(_T("%u"), nSID);
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_SERIALID,strSerialID); 
		///// ID
		int nID = 0;

		//// conflict
		CString strConflict;
		if (pDevInfo->m_bConflict)
		{
			
			strConflict = _T("YES");
			//SetGridCellColor(i+nSize, SCAN_TABLE_CONFLICT, RGB(192,0,0));

			TRACE("Draw Button's SrcID = %d\n", nID);
			//DrawButtonOnGrid(nSize + i, SCAN_TABLE_FIXCONFLICT);
			int nID = pDevInfo->m_nSourceID;	
			int nID1 = pDevInfo->m_pDev->GetDevID();	
			CString strID; strID.Format(_T("%d"), nID);
			CString strID1; strID1.Format(_T("%d"), nID1);

			strConflict = _T("More than one TStat on ID XXX, the Serial No strSerial has been changed as XXX. ");

		}
		else
		{
			//nID = pDevInfo->m_pDev->GetDevID();	
			strConflict = _T("NO");
			//DrawButtonOnGrid(nSize + i, 8);
		}
			
		nID = pDevInfo->m_pDev->GetDevID();	
		CString strID;
		strID.Format(_T("%d"), nID);
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_ADDRESS,strID); 
		///// port
		int nPort = pDevInfo->m_pDev->GetComPort();	
		CString strPort;
		if(pDevInfo->m_pDev->m_mainnet_info.m_ProductType == 0)
		{
			strPort.Format(_T("COM%d"), nPort);
		}
		else if(pDevInfo->m_pDev->m_mainnet_info.m_ProductType == 100)
		{
			strPort.Format(_T("NC:COM%d"), nPort);
		}
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_COMPORT,strPort); 
		//// protocol
		CString strProtocol;		
		if(pDevInfo->m_pDev->GetProtocol() == 2)
		strProtocol.Format(_T("Bacnet MSTP"));
			else
		strProtocol.Format(_T("Modbus 485"));
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_PROTOCOL,strProtocol); 
		//// 
		//m_flexGrid.put_TextMatrix(i+nSize,8,strConflict); 
	}
	//LeaveCriticalSection(&m_csGrid);
	//DrawButtonOnGrid(1, 9);
}




CTStatBase* CScanDlg::FindDeviceByRowNum( int nRow )
{
	//ASSERT(nRow < m_flexGrid.get_GridLines());
	int nRows = m_flexGrid.get_Rows();

	CString strSID = m_flexGrid.get_TextMatrix(nRow, SCAN_TABLE_SERIALID);
	unsigned int nSID = _wtoi64(strSID);


	for (UINT i = 0 ; i < m_pScanner->m_szTstatScandRet.size(); i++)
	{
		_ComDeviceInfo* pInfo = m_pScanner->m_szTstatScandRet[i];
		if (pInfo->m_pDev->GetSerialID() == nSID)
		{
			return pInfo->m_pDev;
		}
	}

	for (UINT i = 0 ; i < m_pScanner->m_szNCScanRet.size(); i++)
	{
		_NetDeviceInfo* pInfo = m_pScanner->m_szNCScanRet[i];
		if (pInfo->m_pNet->GetSerialID() == nSID)
		{
			return pInfo->m_pNet;
		}
	}

	return NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// below the code disposed
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



 
void CScanDlg::OnBnClickedButtonAuto()
{
	//AutoFixComConflict();
}

void CScanDlg::OnBnClickedButtonManual()
{
	
//	AfxMessageBox(_T("111"), MB_OK);
}



void CScanDlg::OnEnKillfocusEditGridedit()
{
  if (m_curcol==9)
  {
	  CString strValue;
	  m_editGrid.GetWindowText(strValue);
	  m_flexGrid.put_TextMatrix(m_currow,m_curcol,strValue);
	  m_editGrid.ShowWindow(SW_HIDE);
  }

}
