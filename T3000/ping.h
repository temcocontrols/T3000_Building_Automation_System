/*
Module : PING.H
Purpose: Interface for an MFC wrapper class to encapsulate PING
Created: PJN / 10-06-1998
History: None

Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/


/////////////////////////// Macros ///////////////////////////

#ifndef __PING_H__
#define __PING_H__


#ifndef __AFXPRIV_H__
#pragma message("The class CPing requires AFXPRIV.H in your PCH")
#endif





#ifdef CPING_USE_ICMP

//These defines & structure definitions are taken from the "ipexport.h" and
//"icmpapi.h" header files as provided with the Platform SDK and
//are used internally by the CPing class. Including them here allows
//you to compile the CPing code without the need to have the full 
//Platform SDK installed.

typedef unsigned long IPAddr;     // An IP address.

// typedef struct tagIP_OPTION_INFORMATION 
// {
//   unsigned char      Ttl;              // Time To Live
//   unsigned char      Tos;              // Type Of Service
//   unsigned char      Flags;            // IP header flags
//   unsigned char      OptionsSize;      // Size in bytes of options data
//   unsigned char FAR *OptionsData;      // Pointer to options data
// } IP_OPTION_INFORMATION;

// typedef struct tagICMP_ECHO_REPLY 
// {
//   IPAddr                Address;       // Replying address
//   unsigned long         Status;        // Reply IP_STATUS
//   unsigned long         RoundTripTime; // RTT in milliseconds
//   unsigned short        DataSize;      // Reply data size in bytes
//   unsigned short        Reserved;      // Reserved for system use
//   void FAR              *Data;         // Pointer to the reply data
//   IP_OPTION_INFORMATION Options;       // Reply options
// } ICMP_ECHO_REPLY;

typedef IP_OPTION_INFORMATION FAR* LPIP_OPTION_INFORMATION;
typedef ICMP_ECHO_REPLY FAR* LPICMP_ECHO_REPLY;
typedef HANDLE (WINAPI IcmpCreateFile)(VOID);
typedef IcmpCreateFile* lpIcmpCreateFile;
typedef BOOL (WINAPI IcmpCloseHandle)(HANDLE IcmpHandle);
typedef IcmpCloseHandle* lpIcmpCloseHandle;
typedef DWORD (WINAPI IcmpSendEcho)(HANDLE IcmpHandle, IPAddr DestinationAddress,
                                    LPVOID RequestData, WORD RequestSize,
                                    LPIP_OPTION_INFORMATION RequestOptions,
                                    LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout);
typedef IcmpSendEcho* lpIcmpSendEcho;

#endif //CPING_USE_ICMP






/////////////////////////// Classes /////////////////////////////////


struct CPingReply
{
	in_addr	 Address;  //The IP address of the replier
	unsigned long RTT; //Round Trip time in Milliseconds
};

class CPing
{
public:
//Methods
#ifdef CPING_USE_ICMP
	BOOL Ping1(LPCTSTR pszHostName, CPingReply& pr, UCHAR nTTL = 10, DWORD dwTimeout = 5000, UCHAR nPacketSize = 32) const;
#endif
#ifdef CPING_USE_WINSOCK2
	BOOL Ping2(LPCTSTR pszHostName, CPingReply& pr, UCHAR nTTL = 10, DWORD dwTimeout = 5000, UCHAR nPacketSize = 32) const;
#endif

protected:
#ifdef CPING_USE_ICMP
	BOOL Initialise1() const;
	static BOOL sm_bAttemptedIcmpInitialise;
  static lpIcmpCreateFile sm_pIcmpCreateFile;
  static lpIcmpSendEcho sm_pIcmpSendEcho;
  static lpIcmpCloseHandle sm_pIcmpCloseHandle;
#endif

#ifdef CPING_USE_WINSOCK2
	BOOL Initialise2() const;
	static BOOL sm_bAttemptedWinsock2Initialise;
  static BOOL sm_bWinsock2OK;
#endif
  static BOOL IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible);

  static __int64 sm_TimerFrequency;
};



#endif //__PING_H__

