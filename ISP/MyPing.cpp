#include "StdAfx.h"
#include "MyPing.h"

CMyPing::CMyPing(void)
{
}

CMyPing::~CMyPing(void)
{
}


void CMyPing::FillIcmpData(char * icmp_data, int datasize)
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
	// 
	// Place some junk in the buffer. 
	// 
	//memset(datapart,''E'', datasize - sizeof(IcmpHeader)); 
	memset(datapart, 'E', datasize - sizeof(IcmpHeader)); 
} 


USHORT CMyPing::Checksum(USHORT *buffer, int size) 
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


int CMyPing::DecodeResp(char *buf, int bytes,struct sockaddr_in *from)
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
		SendEchoMessage(str);
	} 
	icmphdr = (IcmpHeader*)(buf + iphdrlen); 
	if (icmphdr->i_type != ICMP_ECHOREPLY) 
	{ 
		//fprintf(stderr,"non-echo type %d recvd\n",icmphdr->i_type); 
		CString str;
		str.Format(_T("non-echo type %d recvd\n"),  icmphdr->i_type);
		SendEchoMessage(str);
		return 1; 
	} 
	if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
	{ 
		//fprintf(stderr,"someone else's packet!\n"); 
		CString str =(_T("someone else's packet!\n")); 
		SendEchoMessage(str);
		return 1; 
	} 

// 	printf("%d bytes from %s:",bytes, inet_ntoa(from->sin_addr)); 
	 CString str;
	 str.Format(_T("From %s : bytes=%d "), CString(inet_ntoa(from->sin_addr)),bytes); 
	// SendEchoMessage(str);
// 	printf(" icmp_seq = %d. ",icmphdr->i_seq); 
// 	 CString str1;
// 	 str1.Format(_T("icmp_seq=%d "), icmphdr->i_seq); 


	// SendEchoMessage(str1);

// 	printf(" time: %d ms ",GetTickCount()-icmphdr->timestamp); 
	 CString str2;
	 str2.Format(_T("time=%dms "),GetTickCount()-icmphdr->timestamp); 

	 CString str3;
	 str3.Format(_T("TTL=%d "), iphdr->ttl); 

	 SendEchoMessage(str + str2 + str3);

	 // 	printf("\n");

	//CString 
	return 0; 
} 


int CMyPing::CalcTTLValue(int nValue)
{
	int nTTLValue = 0;
	if ( nValue < 256 && nValue > 128) //UNIX |Solari |BSD
	{
		nTTLValue = 255 - nValue;
	}
	else if ( nValue <= 128 && nValue > 64)  //Win NT,XP,VISTA,Win7
	{
		nTTLValue = 128 - nValue;
	}
	else if ( nValue <= 64 && nValue > 32)   // Compaq|  Win95,98,me| Linux
	{
		nTTLValue = 64 - nValue;
	}
	else if ( nValue <= 32 && nValue >= 0)
	{
		nTTLValue = 32 - nValue;
	}

	return nTTLValue;
}


void CMyPing::SetPingEchoWnd(CWnd* pWndEcho)
{
	ASSERT(pWndEcho);
	m_pWndEcho = pWndEcho;
}


BOOL CMyPing::TestPing(const CString& strIP)
{	
// 	CHAR szIP[256] = {0};//new char[256];
// 	ZeroMemory(szIP, 256);	
	USES_CONVERSION;   
	LPSTR szIP=W2A(strIP); 
//	memcpy(szIP, sIP, strIP.GetLength());

	WSADATA wsaData; 
	SOCKET sockRaw; 
	struct sockaddr_in dest,from; 
	struct hostent * hp; 
	int bread,datasize,times; 
	int fromlen = sizeof(from); 
	int timeout = 300;
	int statistic = 0;  /* 用于统计结果 */  
	char *dest_ip; 
	char *icmp_data; 
	char *recvbuf; 
	unsigned int addr=0; 
	USHORT seq_no = 0; 
	if (WSAStartup(MAKEWORD(2,1),&wsaData) != 0)
	{ 
		//fprintf(stderr,"WSAStartup failed: %d\n",GetLastError()); 
		CString str;
		str.Format(_T("WSAStartup failed: %d\n"),GetLastError()); 
		SendEchoMessage(str);
		//ExitProcess(STATUS_FAILED); 
		return FALSE;
	} 
// 	if (argc <2 ) 
// 	{ 
// 		Usage(argv[0]); 
// 	} 

	sockRaw = WSASocket(AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL, 0,WSA_FLAG_OVERLAPPED);
	//
	//注：为了使用发送接收超时设置(即设置SO_RCVTIMEO, SO_SNDTIMEO)，
	//    必须将标志位设为WSA_FLAG_OVERLAPPED !
	// 
	if (sockRaw == INVALID_SOCKET) 
	{ 
		//fprintf(stderr,"WSASocket() failed: %d\n",WSAGetLastError()); 
		CString str;
		SendEchoMessage(str);
		//ExitProcess(STATUS_FAILED); 
		return FALSE;
	} 
	bread = setsockopt(sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout, sizeof(timeout)); 
	
	if(bread == SOCKET_ERROR) 
	{ 
		//fprintf(stderr,"failed to set recv timeout: %d\n",WSAGetLastError()); 
		CString str;
		str.Format(_T("failed to set recv timeout: %d\n"),WSAGetLastError());
		SendEchoMessage(str);
		//ExitProcess(STATUS_FAILED); 
		return FALSE;
	} 
	
	bread = setsockopt(sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout, sizeof(timeout)); 
	if(bread == SOCKET_ERROR) 
	{ 
		//fprintf(stderr,"failed to set send timeout: %d\n",WSAGetLastError()); 
		//ExitProcess(STATUS_FAILED); 
		CString str;
		str.Format(_T("failed to set send timeout: %d\n"),WSAGetLastError());
		SendEchoMessage(str);
		return FALSE;
	} 

	memset(&dest,0,sizeof(dest)); 
	//hp = gethostbyname(argv[1]); 
	hp = gethostbyname(szIP); 
	if (!hp)
	{ 
		addr = inet_addr(szIP); 
	} 
	if ((!hp) && (addr == INADDR_NONE) ) 
	{ 
		//fprintf(stderr,"Unable to resolve %s\n",argv[1]); 
		//ExitProcess(STATUS_FAILED); 
		CString str;
		str.Format(_T("Unable to resolve %s\n"), strIP);
		SendEchoMessage(str);
		return FALSE;
	} 

	if (hp != NULL) 
		memcpy(&(dest.sin_addr),hp->h_addr,hp->h_length); 
	else 
		dest.sin_addr.s_addr = addr; 
	if (hp) 
		dest.sin_family = hp->h_addrtype; 
	else 
		dest.sin_family = AF_INET; 
	dest_ip = inet_ntoa(dest.sin_addr); 
	// 
	//  atoi函数原型是: int atoi( const char *string );
	//  The return value is 0 if the input cannot be converted to an integer !
	//
// 	if(argc>2)
// 	{
// 		times=atoi(argv[2]);
// 		if(times == 0)
// 			times=DEF_PACKET_NUMBER;
// 	}
// 	else
		times=DEF_PACKET_NUMBER;

// 	if (argc >3) 
// 	{ 
// 		datasize = atoi(argv[3]); 
// 		if (datasize == 0) 
// 			datasize = DEF_PACKET_SIZE;
// 		if (datasize >1024)   /* 用户给出的数据包大小太大 */
// 		{
// 			fprintf(stderr,"WARNING : data_size is too large !\n");
// 			datasize = DEF_PACKET_SIZE; 
// 		}
// 	} 
//	else 
		datasize = DEF_PACKET_SIZE; 

	datasize += sizeof(IcmpHeader); 
	icmp_data = (char*)xmalloc(MAX_PACKET); 
	recvbuf = (char*)xmalloc(MAX_PACKET); 
	if (!icmp_data) 
	{ 
		//fprintf(stderr,"HeapAlloc failed %d\n",GetLastError()); 
		//ExitProcess(STATUS_FAILED); 
		CString str;
		str.Format(_T("HeapAlloc failed %d\n"),GetLastError());
		SendEchoMessage(str);
		return FALSE;
	} 

	memset(icmp_data,0,MAX_PACKET); 

	//fill_icmp_data(icmp_data,datasize); 
	FillIcmpData(icmp_data,datasize);
	//
	//显示提示信息
	//
	//fprintf(stdout,"\nPinging %s ....\n\n",dest_ip);
	CString strTips;
	strTips.Format(_T("\nPinging %s ....\n\n"), CString(dest_ip));
	SendEchoMessage(strTips);
	
	for(int i=0;i < 4; i++)
	{ 
		int bwrote; 
		((IcmpHeader*)icmp_data)->i_cksum = 0; 
		((IcmpHeader*)icmp_data)->timestamp = GetTickCount(); 
		((IcmpHeader*)icmp_data)->i_seq = seq_no++; 
		((IcmpHeader*)icmp_data)->i_cksum = Checksum((USHORT*)icmp_data,datasize);
		bwrote = sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest)); 
		if (bwrote == SOCKET_ERROR)
		{ 
			if (WSAGetLastError() == WSAETIMEDOUT) 
			{ 
				//printf("Request timed out.\n"); 
				CString str;
				str.Format(_T("Request timed out.\n"));
				SendEchoMessage(str);
				continue; 
			} 
			//fprintf(stderr,"sendto failed: %d\n",WSAGetLastError()); 
			//ExitProcess(STATUS_FAILED); 
			CString str;
			str.Format(_T("sendto failed: %d\n"),WSAGetLastError());
			SendEchoMessage(str);
			return FALSE;
		} 

		if (bwrote < datasize ) 
		{ 
			//fprintf(stdout,"Wrote %d bytes\n",bwrote);
			CString str;
			str.Format(_T("Wrote %d bytes\n"),bwrote);
			SendEchoMessage(str);
		} 

		bread = recvfrom(sockRaw,recvbuf,MAX_PACKET,0,(struct sockaddr*)&from,&fromlen); 
		if (bread == SOCKET_ERROR)
		{ 
			if (WSAGetLastError() == WSAETIMEDOUT) 
			{ 
				//printf("Request timed out.\n"); 
				CString str;
				str.Format(_T("Request timed out.\n"));
				SendEchoMessage(str);
				continue; 
			} 

			//fprintf(stderr,"recvfrom failed: %d\n",WSAGetLastError()); 
			//ExitProcess(STATUS_FAILED); 
			CString str;
			str.Format(_T("recvfrom failed: %d\n"),WSAGetLastError());
			SendEchoMessage(str);
			return FALSE;
		} 

		if(!DecodeResp(recvbuf,bread,&from))
			statistic++; /* 成功接收的数目++ */
		
		Sleep(1000); 
	}

/*
Display the statistic result
*/
	//fprintf(stdout,	"\nPing statistics for %s \n",dest_ip);
	//fprintf(stdout,	"Packets: Sent = %d,Received = %d, Lost = %d (%2.0f%% loss)\n",times, statistic,(times-statistic),(float)(times-statistic)/times*100);

	CString str,str1;
	str.Format(_T("\nPing statistics for %s : \n"),  CString(dest_ip));
	SendEchoMessage(str);

	str1.Format(_T("Packets: Sent = %d, Received = %d, Lost = %d (%2.0f%% loss)\n"),times, statistic,(times-statistic),(float)(times-statistic)/times*100);
	SendEchoMessage(str1);

	
	xfree(icmp_data);
	xfree(recvbuf);

// 	delete[] szIP;
// 	szIP = NULL;

	WSACleanup();
	return 0; 
}

BOOL CMyPing::SendEchoMessage(const CString& strMsg)
{
	//使用WM_COPYDATA 
	COPYDATASTRUCT cds = {0};
	//int nSize = (strMsg.GetLength()+1)*sizeof(TCHAR);
	//char* szMsg = new char[]
	//LPCTSTR szMsg = (LPCTSTR)strMsg;
	
	cds.dwData = 0;//(DWORD)m_pWndEcho->m_hWnd;// hWnd;
	cds.cbData = (strMsg.GetLength()+1)*sizeof(TCHAR); //(lstrlen(szBuffer)+1)*sizeof(TCHAR);
	
	cds.lpData =  (LPVOID)(LPCTSTR)strMsg;
	
	
	return SendMessage(m_pWndEcho->m_hWnd, WM_COPYDATA, (WPARAM)0, (LPARAM)&cds); 
//	return PostMessage(m_pWndEcho->m_hWnd, WM_COPYDATA, (WPARAM)0, (LPARAM)&cds); 

//////////////////////////////////////////////////////////////////////////

}