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

example:
CMulitithreadSocket m_wskServer;
BOOL m_bServer;

if(m_wskServer.CreateServer(10000, SOMAXCONN))
{
m_bServer = m_wskServer.StartServer(Listen);
}

if(!m_bServer)
{
CString info;
info.Format(_T("Start Server Failed：%d"), m_wskServer.err);
AfxMessageBox(info);
}


//TCP Server 的 回调函数 ，当客户端连接上 之后就可以用这个socket去发送数据了;//This function add by Fance.
void CALLBACK Listen(SOCKET s, int ServerPort, const char *ClientIP)
{
int nRet;
char buf[1000];

CMulitithreadSocket wsk;
wsk = s;
bool get_mutex_control = false;

while(1)
{
//info.Empty();
//nRet = wsk.ReadData(buf, 800, 60);
if(wsk.IsSockConnected(s))
{
char sendbuf[] = "Receive connect";


wsk.SendData(sendbuf,20,3000);
TRACE(_T("OK\r\n"));
Sleep(30000);
}
else
{
break;
}

}

wsk.Close();

}

*/

#if !defined(AFX_MULITITHREADSOCKET_H__F8FC1AC5_A42B_4FD3_8A4F_3FF85879C5A8__INCLUDED_)
#define AFX_MULITITHREADSOCKET_H__F8FC1AC5_A42B_4FD3_8A4F_3FF85879C5A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MulitithreadSocket.h : header file
//
//#include "winsock2.h"
typedef void (CALLBACK* DEALPROC)(SOCKET s, int ServerPort, const char *ClientIP);

/////////////////////////////////////////////////////////////////////////////
// CMulitithreadSocket command target
extern bool m__close;
class CMulitithreadSocket : public CSocket
{
// Attributes
public:
	int		err;
// Operations
public:
	CMulitithreadSocket();
//	CMulitithreadSocket(SOCKET S);

	virtual ~CMulitithreadSocket();
	void    operator=(SOCKET s);

	BOOL StopServer();
	BOOL StartServer(DEALPROC lpDealFunc);

	BOOL CreateServer(int port, int backlog);
	int ReadData(char FAR* buf, int len, int timeout);
	int SendData(const char FAR* buf, int len, int timeout);
	BOOL IsSockConnected(SOCKET s);

	void Close();
	void CloseServer();


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulitithreadSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMulitithreadSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
	
protected:	
	SOCKET  m_sSocket;
	BOOL	m_bConnected;
	BOOL	m_bNeedClose;

	SOCKET  m_sServer;
	BOOL	m_bListened;
	int		m_nServerPort;
	BOOL	m_bNeedCloseServer;

	BOOL	m_bWSAStartup;

	HANDLE	m_hServerThread;

	//function
    static DWORD CALLBACK TimeoutControl(LPVOID lpParm);
	static DWORD CALLBACK ServerProc(LPVOID lpParm);
	static DWORD CALLBACK DealProc(LPVOID lpParm);

 	struct TPARA
	{
		int		OutTime;
		SOCKET	s;
		BOOL	*pbConnected;
		BOOL bExit;
		BOOL IsExit;
	};

	struct SERVERPARA
	{
		SOCKET		s;
		int			port;
		DEALPROC	lpDealFunc;
	};

	struct DEALPARA
	{
		SOCKET		s;
		int			port;
		char		IP[32];
		DEALPROC	lpDealFunc;
		int			SourcePort;
	};

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULITITHREADSOCKET_H__F8FC1AC5_A42B_4FD3_8A4F_3FF85879C5A8__INCLUDED_)
