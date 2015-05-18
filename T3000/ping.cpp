/*
Module : PING.CPP
Purpose: Implementation for an MFC wrapper class to encapsulate PING
Created: PJN / 10-06-1998
History: PJN / 23-06-1198 1) Now code can be compiled to use Winsock2 calls
                          instead of using the ICMP.DLL. This gives another of
                          advantages:

                          i)  Your using a API that MS has promised to continue to support.
                          ii) Internally the class calls QueryPerformanceCounter meaning that
                              you will get the highest resolution RTT's possible.

                          2) Also did a general tidy up of the code
                          3) Changed default timeout to 1 second
         PJN / 30-07-1998 1) Can now use both Winsock 2 calls and ICMP style
				                     calls at the same time with the use of 2 preprocessor directives
                          2) sample program now use generic entry point _tmain 
													3) Got rid of a 2 level 4 compiler warning
													4) Fixed a problem with the cleanup of ICMP dll usage
													5) Tidied up and optimized the usage of static variables



Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/


/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "ping.h"


/////////////////////////////////  Definitions ////////////////////////////////

#define MIN_ICMP_PACKET_SIZE 8    //minimum 8 byte icmp packet (just header)
#define MAX_ICMP_PACKET_SIZE 1024 //Maximum icmp packet size

#ifdef CPING_USE_WINSOCK2

#ifndef _WINSOCK2API_
#pragma message("You need to include winsock2.h in your PCH")
#endif

// IP header
typedef struct tagIP_HEADER 
{
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum
	unsigned int sourceIP;
	unsigned int destIP;
} IP_HEADER;
typedef IP_HEADER FAR* LPIP_HEADER;

// ICMP header
typedef struct tagICMP_HEADER 
{
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
} ICMP_HEADER;
typedef ICMP_HEADER FAR* LPICMP_HEADER;

void FillIcmpData(LPICMP_HEADER pIcmp, int nData);
BOOL DecodeResponse(char* pBuf, int nBytes, sockaddr_in* from);
USHORT GenerateIPChecksum(USHORT* pBuffer, int nSize);

#endif //CPING_USE_WINSOCK2




/////////////////////////////////  Macros & Statics ///////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef CPING_USE_WINSOCK2
BOOL CPing::sm_bWinsock2OK = FALSE;
BOOL CPing::sm_bAttemptedWinsock2Initialise = FALSE;
#endif 

#ifdef CPING_USE_ICMP
BOOL CPing::sm_bAttemptedIcmpInitialise = FALSE;
lpIcmpCreateFile CPing::sm_pIcmpCreateFile = NULL;
lpIcmpSendEcho CPing::sm_pIcmpSendEcho = NULL;
lpIcmpCloseHandle CPing::sm_pIcmpCloseHandle = NULL;
#endif

__int64 CPing::sm_TimerFrequency = 0;


//Internal class which is used to ensure that the ICMP
//handle and winsock stack is closed upon exit 
class _CPING
{
public:
  _CPING();
	~_CPING();
protected:
#ifdef CPING_USE_ICMP
  HINSTANCE sm_hIcmp;
#endif

	friend class CPing;
};

_CPING::_CPING()
{
#ifdef CPING_USE_ICMP
  sm_hIcmp = NULL;
#endif
}

_CPING::~_CPING()
{
#ifdef CPING_USE_ICMP
	if (sm_hIcmp)
	{
		FreeLibrary(sm_hIcmp);
		sm_hIcmp = NULL;
  }
#endif
	WSACleanup();
}

static _CPING _cpingData;




///////////////////////////////// Implementation //////////////////////////////

#ifdef CPING_USE_WINSOCK2
BOOL CPing::Initialise2() const
{
	if (!sm_bAttemptedWinsock2Initialise)
	{
		sm_bAttemptedWinsock2Initialise = TRUE;

		//Initialise the winsock 2 stack
		WSADATA wsa;
		sm_bWinsock2OK = (WSAStartup(MAKEWORD(2, 1), &wsa) == 0);

    //Use the High performace counter to get an accurate RTT
    LARGE_INTEGER Frequency;
		Frequency.QuadPart = 0;
    sm_bWinsock2OK = sm_bWinsock2OK && QueryPerformanceFrequency(&Frequency);
    if (sm_bWinsock2OK)
      sm_TimerFrequency = Frequency.QuadPart;
	}

	return sm_bWinsock2OK;
}
#endif //CPING_USE_WINSOCK2

#ifdef CPING_USE_ICMP
BOOL CPing::Initialise1() const
{
	if (!sm_bAttemptedIcmpInitialise)
	{
		sm_bAttemptedIcmpInitialise = TRUE;

		//Initialise the winsock stack
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		{
			TRACE(_T("Could not negotiate a correct version of WinSock\n"));
			return FALSE;
		}

		//Load up the ICMP library
		_cpingData.sm_hIcmp = LoadLibrary(_T("ICMP.DLL"));
		if (_cpingData.sm_hIcmp == NULL)
		{
			TRACE(_T("Could not load up the ICMP DLL\n"));
			return FALSE;
		}

		//Retrieve pointers to the functions in the ICMP dll
		sm_pIcmpCreateFile = (lpIcmpCreateFile) GetProcAddress(_cpingData.sm_hIcmp,"IcmpCreateFile");
		sm_pIcmpSendEcho = (lpIcmpSendEcho) GetProcAddress(_cpingData.sm_hIcmp,"IcmpSendEcho" );
		sm_pIcmpCloseHandle = (lpIcmpCloseHandle) GetProcAddress(_cpingData.sm_hIcmp,"IcmpCloseHandle");

		if (sm_pIcmpCreateFile == NULL || sm_pIcmpSendEcho == NULL ||	sm_pIcmpCloseHandle == NULL)
		  TRACE(_T("Could not find ICMP functions in the ICMP DLL\n"));
	}

	return (sm_pIcmpCreateFile != NULL && sm_pIcmpSendEcho != NULL &&	sm_pIcmpCloseHandle != NULL);
}
#endif //CPING_USE_ICMP

#ifdef CPING_USE_WINSOCK2
BOOL CPing::Ping2(LPCTSTR pszHostName, CPingReply& pr, UCHAR /*nTTL*/, DWORD dwTimeout, UCHAR nPacketSize) const
{
  //Parameter validation
  if (nPacketSize > MAX_ICMP_PACKET_SIZE || nPacketSize < MIN_ICMP_PACKET_SIZE)
  {
    ASSERT(FALSE);
    SetLastError(WSAENOBUFS);
    return FALSE;
  }

	//For correct operation of the T2A macro, see TN059
	USES_CONVERSION;

	//Make sure everything is initialised
	if (!Initialise2())
	  return FALSE;

  //Resolve the address of the host to connect to
  sockaddr_in dest;
  memset(&dest,0,sizeof(dest));
	LPSTR lpszAscii = T2A((LPTSTR) pszHostName);
  unsigned long addr = inet_addr(lpszAscii);
	if (addr == INADDR_NONE)
	{
		//Not a dotted address, then do a lookup of the name
		hostent* hp = gethostbyname(lpszAscii);
		if (hp)
    {
      memcpy(&(dest.sin_addr),hp->h_addr,hp->h_length);
  	  dest.sin_family = hp->h_addrtype;
    }
    else
	  {
		  TRACE(_T("CPing::Ping2, Could not resolve the host name %s\n"), pszHostName);
		  return FALSE;
    }
  }
  else
  {
    dest.sin_addr.s_addr = addr;
    dest.sin_family = AF_INET;
  }

  //Create the raw socket
  SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, 0);
  if (sockRaw == INVALID_SOCKET) 
	{
	  TRACE(_T("CPing::Ping2, Failed to create a raw socket\n"));
	  return FALSE;
  }

  //Allocate the ICMP packet
  int nBufSize = nPacketSize + sizeof(ICMP_HEADER);
  char* pICMP = new char[nBufSize];
  FillIcmpData((LPICMP_HEADER) pICMP, nBufSize);

  //Get the tick count prior to sending the packet
  LARGE_INTEGER TimerTick;
  VERIFY(QueryPerformanceCounter(&TimerTick));
  __int64 nStartTick = TimerTick.QuadPart;

  //Send of the packet
	int nWrote = sendto(sockRaw, pICMP, nBufSize, 0, (sockaddr*)&dest, sizeof(dest));
	if (nWrote == SOCKET_ERROR)
	{
		TRACE(_T("CPing::Ping2, sendto failed\n"));

    delete [] pICMP;

    DWORD dwError = GetLastError();
    closesocket(sockRaw);
    SetLastError(dwError);

    return FALSE;
	}

  //allocate the recv buffer
  char* pRecvBuf = new char[MAX_ICMP_PACKET_SIZE];
  BOOL bReadable;
  sockaddr_in from;
  int nFromlen = sizeof(from);
  int nRead = 0;

  //Allow the specified timeout
  if (IsSocketReadible(sockRaw, dwTimeout, bReadable))
  {
    if (bReadable)
    {
      //Receive the response
	    nRead = recvfrom(sockRaw, pRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&from, &nFromlen);
    }
    else
    {
		  TRACE(_T("CPing::Ping2, timeout occured while awaiting recvfrom\n"));
      closesocket(sockRaw);

      delete [] pICMP;
      delete [] pRecvBuf;

      //set the error to timed out
      SetLastError(WSAETIMEDOUT);

		  return FALSE;
    }
  }
  else
  {
		TRACE(_T("CPing::Ping2, IsReadible call failed\n"));

    delete [] pICMP;
    delete [] pRecvBuf;

    DWORD dwError = GetLastError();
    closesocket(sockRaw);
    SetLastError(dwError);

		return FALSE;
  }

  //Get the current tick count
  VERIFY(QueryPerformanceCounter(&TimerTick));

  //Now check the return response from recvfrom
	if (nRead == SOCKET_ERROR)
	{
		TRACE(_T("CPing::Ping2, recvfrom call failed\n"));

    delete [] pICMP;
    delete [] pRecvBuf;

    DWORD dwError = GetLastError();
    closesocket(sockRaw);
    SetLastError(dwError);

		return FALSE;
	}

  //Decode the response we got back
 	BOOL bSuccess = DecodeResponse(pRecvBuf, nRead, &from);

  //If we successfully decoded the response, then return the
  //values in the CPingReply instance
  if (bSuccess)
  {
    pr.Address = from.sin_addr;
    pr.RTT = (ULONG) ((TimerTick.QuadPart - nStartTick) * 1000 / sm_TimerFrequency);
  }

  //Don't forget to release out socket
  closesocket(sockRaw);

  //Free up the memory we allocated
  delete [] pICMP;
  delete [] pRecvBuf;

  //return the status
	return bSuccess;
}
#endif //CPING_USE_WINSOCK2

BOOL CPing::IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible)
{
  timeval timeout = {dwTimeout/1000, dwTimeout % 1000};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(socket, &fds);
  int nStatus = select(0, &fds, NULL, NULL, &timeout);
  if (nStatus == SOCKET_ERROR)
  {
    return FALSE;
  }
  else
  {
    bReadible = !(nStatus == 0);
    return TRUE;
  }
}

#ifdef CPING_USE_ICMP
BOOL CPing::Ping1(LPCTSTR pszHostName, CPingReply& pr, UCHAR nTTL, DWORD dwTimeout, UCHAR nPacketSize) const
{
	//For correct operation of the T2A macro, see TN059
	USES_CONVERSION;

	//Make sure everything is initialised
	if (!Initialise1())
	  return FALSE;

  ASSERT(_cpingData.sm_hIcmp); //ICMP dll must be open

	LPSTR lpszAscii = T2A((LPTSTR) pszHostName);
	//Convert from dotted notation if required
	unsigned long	addr = inet_addr(lpszAscii);
	if (addr == INADDR_NONE)
	{
		//Not a dotted address, then do a lookup of the name
		hostent* hp = gethostbyname(lpszAscii);
		if (hp)
			memcpy(&addr, hp->h_addr, hp->h_length);
		else
		{
			TRACE(_T("Could not resolve the host name %s\n"), pszHostName);
			return FALSE;
		}
	}

  //Create the ICMP handle
	HANDLE hIP = sm_pIcmpCreateFile();
	if (hIP == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("Could not get a valid ICMP handle\n"));
		return FALSE;
	}

  //Set up the option info structure
	IP_OPTION_INFORMATION OptionInfo;
	ZeroMemory(&OptionInfo, sizeof(IP_OPTION_INFORMATION));
	OptionInfo.Ttl = nTTL;

  //Set up the data which will be sent
  unsigned char* pBuf = new unsigned char[nPacketSize];
  memset(pBuf, 'E', nPacketSize);

	//Do the actual Ping
  int nReplySize = sizeof(ICMP_ECHO_REPLY) + max(MIN_ICMP_PACKET_SIZE, nPacketSize);
  unsigned char* pReply = new unsigned char[nReplySize];
	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*) pReply;
  DWORD nRecvPackets = sm_pIcmpSendEcho(hIP, addr, pBuf, nPacketSize, &OptionInfo, pReply, nReplySize, dwTimeout);

  //Check we got the packet back
  BOOL bSuccess = (nRecvPackets == 1);

  //Check the IP status is OK (O is IP Success)
  if (bSuccess && (pEchoReply->Status != 0))
  {
    bSuccess = FALSE;
    SetLastError(pEchoReply->Status);
  }

  //Check we got the same amount of data back as we sent
  if (bSuccess)
  {
    bSuccess = (pEchoReply->DataSize == nPacketSize);
    if (!bSuccess)
      SetLastError(ERROR_UNEXP_NET_ERR);
  }

  //Check the data we got back is what was sent
  if (bSuccess)
  {
    char* pReplyData = (char*) pEchoReply->Data;
    for (int i=0; i<nPacketSize && bSuccess; i++)
      bSuccess = (pReplyData[i] == 'E');

    if (!bSuccess)
      SetLastError(ERROR_UNEXP_NET_ERR);
  }

  //Close the ICMP handle
	sm_pIcmpCloseHandle(hIP);

	if (bSuccess)
	{
    //Ping was successful, copy over the pertinent info
    //into the return structure
		pr.Address.S_un.S_addr = pEchoReply->Address;
	  pr.RTT = pEchoReply->RoundTripTime;
	}

  //Free up the memory we allocated
  delete [] pBuf;
  delete [] pReply;

  //return the status
	return bSuccess; 
}
#endif //CPING_USE_ICMP


#ifdef CPING_USE_WINSOCK2
//Decode the raw Ip packet we get back
BOOL DecodeResponse(char* pBuf, int nBytes, sockaddr_in* from) 
{
  //Get the current tick count
  LARGE_INTEGER TimerTick;
  VERIFY(QueryPerformanceCounter(&TimerTick));


	LPIP_HEADER pIpHdr = (LPIP_HEADER) pBuf;
	int nIpHdrlen = pIpHdr->h_len * 4; //Number of 32-bit words*4 = bytes

  //Not enough data recieved
	if (nBytes < nIpHdrlen + MIN_ICMP_PACKET_SIZE) 
	{
		TRACE(_T("Received too few bytes from %s\n"), inet_ntoa(from->sin_addr));
    SetLastError(ERROR_UNEXP_NET_ERR);
    return FALSE;
	}

  //Check it is an ICMP_ECHOREPLY packet
	LPICMP_HEADER pIcmpHdr = (LPICMP_HEADER) (pBuf + nIpHdrlen);
	if (pIcmpHdr->i_type != 0) //type ICMP_ECHOREPLY is 0
	{
		TRACE(_T("non-echo type %d recvd\n"), pIcmpHdr->i_type);
    SetLastError(ERROR_UNEXP_NET_ERR);
		return FALSE;
	}

  //Check it is the same id as we sent
	if (pIcmpHdr->i_id != (USHORT)GetCurrentProcessId()) 
	{
		TRACE(_T("Received someone else's packet!\n"));
    SetLastError(ERROR_UNEXP_NET_ERR);
		return FALSE;
	}

  return TRUE;
}

//generate an IP checksum based on a given data buffer
USHORT GenerateIPChecksum(USHORT* pBuffer, int nSize) 
{
  unsigned long cksum = 0;

  while (nSize > 1) 
	{
	  cksum += *pBuffer++;
	  nSize -= sizeof(USHORT);
  }
  
  if (nSize) 
	  cksum += *(UCHAR*)pBuffer;

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >>16);
  return (USHORT)(~cksum);
}

//Fill up the ICMP packet with defined values
void FillIcmpData(LPICMP_HEADER pIcmp, int nData)
{
  pIcmp->i_type    = 8; //ICMP_ECHO type
  pIcmp->i_code    = 0;
  pIcmp->i_id      = (USHORT) GetCurrentProcessId();
  pIcmp->i_seq     = 0;
  pIcmp->i_cksum   = 0;
  pIcmp->timestamp = GetTickCount();
 
  //Set up the data which will be sent
  int nHdrSize = sizeof(ICMP_HEADER);
  char* pData = (char*) (pIcmp + nHdrSize);
  memset(pData, 'E', nData - nHdrSize);

  //Generate the checksum
  pIcmp->i_cksum = GenerateIPChecksum((USHORT*)pIcmp, nData);
}
#endif //CPING_USE_WINSOCK2

