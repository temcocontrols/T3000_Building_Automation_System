// MulitithreadSocket.cpp : implementation file
//
/*
* Copyright (C) 2014 Fance Du <dufanfance@163.com>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*/
#include "stdafx.h"
//#include "mulitithread.h"
#include "MulitithreadSocket.h"
//#include "..\global_define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02
/////////////////////////////////////////////////////////////////////////////
// CMulitithreadSocket
/*extern*/ bool m__close;
void CMulitithreadSocket::Close()
{
    if(!m_bConnected) return;
	if(m_sSocket == NULL) return;

	//BOOL  bDontLinger  =  FALSE;  

	//::setsockopt(m_sSocket,SOL_SOCKET,SO_DONTLINGER,(LPCSTR)&bDontLinger,sizeof(BOOL));  

	//linger m_sLinger;
	//m_sLinger.l_onoff = 0; 
	//m_sLinger.l_linger = 0; 
	//setsockopt( m_sSocket, SOL_SOCKET, SO_LINGER, ( const char* )&m_sLinger, sizeof( linger ) );

	shutdown(m_sSocket, SD_BOTH);
	Sleep(50);
	::closesocket(m_sSocket);
	m_sSocket = NULL;
	m_bConnected = FALSE;
}
void CMulitithreadSocket::CloseServer()
{
    if(!m_bListened) return;
	if(m_sServer == NULL) return;


	//BOOL  bDontLinger  =  FALSE;  

	//::setsockopt(m_sServer,SOL_SOCKET,SO_DONTLINGER,(LPCSTR)&bDontLinger,sizeof(BOOL));  

	shutdown(m_sServer, SD_BOTH);
	//shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	m_bListened = FALSE;
}

CMulitithreadSocket::CMulitithreadSocket()
{
    WSADATA wsaData;//存储SOCKET初始信息

	m_sSocket = NULL;
	m_bConnected = FALSE;
	m_sServer = NULL;
	m_bListened = FALSE;
	m_nServerPort = -1;
	m_hServerThread = NULL;

	m_bNeedClose = FALSE;
	m_bNeedCloseServer = FALSE;
	
	if(WSAStartup(0x202, &wsaData) == 0)
	{
		m_bWSAStartup = TRUE;
		err = 0;
	}
	else
	{
		m_bWSAStartup = FALSE;
		err = WSAGetLastError();
	}
}

CMulitithreadSocket::~CMulitithreadSocket()
{
	if(m_bNeedClose) Close();
	if(m_bNeedCloseServer) CloseServer();
	if(m_bWSAStartup) WSACleanup();
}
void CMulitithreadSocket::operator=(SOCKET s)
{	
	if(IsSockConnected(s))
	{
		if(m_bConnected) Close();
		m_sSocket = s;
		m_bConnected = TRUE;
		m_bNeedClose = FALSE;
	}

}

BOOL CMulitithreadSocket::CreateServer(int port, int backlog)
{
	if(m_bListened == TRUE) CloseServer();

	struct	sockaddr_in local;

	//创建监听socket
    m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(m_sServer == SOCKET_ERROR)
    {
        err = WSAGetLastError();
        return FALSE;
    }

	//添参数	
	local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons(port);

	if(bind(m_sServer, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
		err = WSAGetLastError();
		closesocket(m_sServer);
        return FALSE;
    }

	//开始侦听
    if(listen(m_sServer, backlog) != 0)
	{
		err = WSAGetLastError();
		closesocket(m_sServer);
        return FALSE;
    }

	m_nServerPort = port;
	m_bListened = TRUE;
	m_bNeedCloseServer = TRUE;

	return TRUE;
}
//extern HWND my_dlg_hwnd;
DWORD CALLBACK CMulitithreadSocket::DealProc(LPVOID lpParm)
{ 
    DEALPARA *para = (DEALPARA*)lpParm;
	
	if(para == NULL) return -1;

	SOCKET		s = para->s;
	int			port = para->port;
	DEALPROC	lpDealFunc = para->lpDealFunc;
	char		IP[32];
	int			source_port = para->SourcePort;
	memcpy(IP, para->IP, sizeof(IP));
	delete para;

//	Client_Info *temp_client = new Client_Info;
//	memcpy(temp_client->ip_address, IP, sizeof(IP));
//	temp_client->port = source_port;
//	::PostMessage(my_dlg_hwnd,WM_REFRESH_LIST,(WPARAM)temp_client,1);
	try
	{
		lpDealFunc(s, port, IP);
	}
	catch(...)
	{
		//::PostMessage(my_dlg_hwnd,WM_REFRESH_LIST,(WPARAM)temp_client,2);
		return -1;
	}
//	::PostMessage(my_dlg_hwnd,WM_REFRESH_LIST,(WPARAM)temp_client,2);
	//if(temp_client !=NULL)
	//{
	//	delete temp_client;
	//	temp_client = NULL;
	//}
	return 0;
}

int CMulitithreadSocket::ReadData(char FAR* buf, int len, int timeout)
{   
	
	if(!m_bConnected || m_sSocket == NULL)
	{
		err = -1;
		return -1;
	}
   
   
	HANDLE hThread;
	DWORD dwThreadId;
	TPARA para;

	para.OutTime = timeout;
	para.s = m_sSocket;
	para.bExit = FALSE;
	para.IsExit = FALSE;
	para.pbConnected = &m_bConnected;
	hThread = CreateThread(NULL, NULL, TimeoutControl, (LPVOID)(&para), 0, &dwThreadId);
	if (hThread == NULL) return -1;
	int nRet = recv(m_sSocket, buf, len, 0);
	if(m__close==true)
   
		return -1;
   
	if(nRet == SOCKET_ERROR) err = WSAGetLastError();

	para.bExit = TRUE;
	while(!para.IsExit) Sleep(1);

	return nRet;
}

int CMulitithreadSocket::SendData(const char FAR *buf, int len, int timeout)
{
	if(!m_bConnected || m_sSocket == NULL)
	{
		err = -1;
		return -1;
	}

	if(!IsSockConnected(m_sSocket))
	{
		m_bConnected = FALSE;
		err = -1;
		return -1;
	}

	int nRet = send(m_sSocket, buf, len, 0);

	if(nRet == SOCKET_ERROR) err = WSAGetLastError();

	return nRet;
}

BOOL CMulitithreadSocket::IsSockConnected(SOCKET s)
{	
	int nRet = 0;
	struct fd_set Fd_Recv;
    struct timeval Time_Recv;

	memset(&Fd_Recv, 0, sizeof(struct fd_set));
    FD_CLR(s, &Fd_Recv); 
	FD_ZERO(&Fd_Recv);	
    FD_SET(s, &Fd_Recv); 
    Time_Recv.tv_sec = 0;
    Time_Recv.tv_usec = 0;

	nRet = select(s, &Fd_Recv, NULL, NULL, &Time_Recv);
	if(nRet == 1)
	{
		char buf[2000];
		int nRet = recv(s, buf, 2000, 0);
		if(nRet<=0)
			return false;
		else
			return true;
	}
	if(nRet == 0)
		return true;
	else
		return false;
    //return (nRet == 0);
}


BOOL CMulitithreadSocket::StartServer(DEALPROC lpDealFunc)
{
	if(!m_bListened) return -1;
	if(m_sServer == NULL) return -1;

	DWORD dwThreadId;
	SERVERPARA *para;

	para = new SERVERPARA;
	para->s = m_sServer;
	para->port = m_nServerPort;
	para->lpDealFunc = lpDealFunc;

	m_hServerThread = CreateThread(NULL, 0, ServerProc, (LPVOID)(para), 0, &dwThreadId);

	if(m_hServerThread == NULL)
	{
		delete para;
		err = WSAGetLastError();
        return FALSE;
	}

	return TRUE;
}
DWORD CALLBACK CMulitithreadSocket::ServerProc(LPVOID lpParm)
{
	SERVERPARA *para = (SERVERPARA*)lpParm;
	
	if(para == NULL) return -1;

	SOCKET		s = para->s;
	int			port = para->port;
	DEALPROC	lpDealFunc = para->lpDealFunc;
	delete para;

	SOCKET	sClient;	
	int		iAddrSize;
	struct	sockaddr_in addr;
	char	IP[32];
	HANDLE	hThread;
    DWORD	dwThreadId;
	DEALPARA *parac;

	iAddrSize = sizeof(addr);

	while(1)
	{
		sClient = accept(s, (struct sockaddr *)&addr, &iAddrSize);
		
		if(sClient == SOCKET_ERROR) break;

		sprintf(IP, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, 
									   addr.sin_addr.S_un.S_un_b.s_b2,
									   addr.sin_addr.S_un.S_un_b.s_b3,
									   addr.sin_addr.S_un.S_un_b.s_b4);

		parac = new DEALPARA;
		memset(parac->IP, 0, sizeof(parac->IP));
		parac->s = sClient;
		parac->port = port;
		parac->lpDealFunc = lpDealFunc;
		memcpy(parac->IP, IP, strlen(IP));	
		parac->SourcePort =  ntohs(addr.sin_port);
		//侦听到连接，开一个线程		
        hThread = CreateThread(NULL, 0, DealProc, (LPVOID)(parac), 0, &dwThreadId);

		if(hThread == NULL) delete parac;
	}

	return 0;
}
BOOL CMulitithreadSocket::StopServer()
{
    CloseServer();

	return TRUE;
}
DWORD CALLBACK CMulitithreadSocket::TimeoutControl(LPVOID lpParm)
{
	TPARA* para = (TPARA*)lpParm;
	time_t stime = time(NULL);
	BOOL bTimeover = FALSE;
	Sleep(1);
	while(!bTimeover)
	{
		if(para->bExit)
		{
			para->IsExit = TRUE;
			return 0;
		}

		Sleep(1);

		time_t ntime = time(NULL);
		if((ntime - stime) > para->OutTime) 
			bTimeover = TRUE;
		//if((ntime - stime) > para->OutTime) bTimeover = FALSE;
	}

	if(para->bExit)
	{
		para->IsExit = TRUE;
		return 0;
	}

	if(para->s != NULL)
	{
		para->pbConnected[0] = FALSE;
		shutdown(para->s, SD_RECEIVE);
		Sleep(5);
		closesocket(para->s);
	}

	para->IsExit = TRUE;

	return 0;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMulitithreadSocket, CSocket)
	//{{AFX_MSG_MAP(CMulitithreadSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMulitithreadSocket member functions
