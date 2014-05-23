#include "StdAfx.h"
#include "TCPFlasher.h"



UINT _TCPFlashFunc(PVOID pParam);

const BYTE TCPFLASH_CMD[] = {0xee, 0x11};
const BYTE TCPFLASH_INPUT_START[] =	{0x00, 0xee,0x11, 0xff,0xff, 0x00,0x00};
const BYTE TCPFLASH_INPUT_END[] =		{0x00, 0xee,0x11, 0xff,0xff, 0xff,0xff};

const BYTE TCPFLASH_OUTPUT_START[] = {0x00, 0xee,0x12, 0xff,0xff, 0x00,0x00};


CTCPFlasher::CTCPFlasher(void)
{
	m_socket = NULL;


	m_pParent = NULL;
	m_dwIP = 0;
	m_nIPPort =0;

	m_pFileBuf = NULL;

	m_nBufSize = 0;


}

CTCPFlasher::~CTCPFlasher(void)
{
// 	if (m_socket)
// 	{
// 		closeSocket(m_socket);
// 	}
}

void CTCPFlasher::SetIPAddr(DWORD dwIP)
{
	m_dwIP = dwIP;
}

void CTCPFlasher::SetIPPort(int nPort)
{
	m_nIPPort = nPort;	
}

void CTCPFlasher::SetParentWnd(CWnd* pWnd)
{
	m_pParent = pWnd;
}

void CTCPFlasher::SetFileBuffer(char* pFileBuf, int nBufSize)
{
	ASSERT(pFileBuf);
	m_pFileBuf = pFileBuf;	
	m_nBufSize = nBufSize;
}

BOOL CTCPFlasher::StartFlash()
{
	CWinThread* pThread = AfxBeginThread(_TCPFlashFunc, this);
  
	ASSERT(pThread);
	return TRUE;
}




UINT _TCPFlashFunc(PVOID pParam)
{
	CTCPFlasher* pFlasher = (CTCPFlasher*)pParam;

	SOCKET  sockSend = ::socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(pFlasher->m_nIPPort);
	//sin.sin_addr.S_un.S_addr = htonl(pFlasher->m_dwIP);
	sin.sin_addr.s_addr = (INADDR_ANY);
	//siRecv.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(_T("192.168.0.3"))
	
	if(bind(sockSend, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		CString strError;
		strError.Format(_T("New work initialize failed. (Error code : %d)"), dwError);
		pFlasher->ShowFlashInfo(strError, TRUE);
		::closesocket(sockSend);
		goto FINISH;
		return FALSE;
	}

	SOCKADDR_IN sinSend;
	sinSend.sin_family = AF_INET;
	sinSend.sin_port = htons(pFlasher->m_nIPPort);
	sinSend.sin_addr.S_un.S_addr = htonl(pFlasher->m_dwIP);
	
	if (connect(sockSend, (sockaddr*)&sinSend, sizeof(sinSend))  == SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		CString strError;
		strError.Format(_T("Can't connect remote device. Please verify connection.(Error code : %d)") , dwError);
		pFlasher->ShowFlashInfo(strError , TRUE);
		::closesocket(sockSend);
		goto FINISH;
		return FALSE;
	}
	
	char* pRecBuf = new char[16];
	ZeroMemory(pRecBuf, 16);

	int nNetTimeout = 3000;
	//发送时限
	setsockopt(sockSend,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
	//接收时限
	setsockopt(sockSend,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

	int nRet = ::send(sockSend, (char*)(TCPFLASH_INPUT_START), 7, 0);

	nRet = ::recv(sockSend, pRecBuf, 16, 0);
	
	if (nRet == SOCKET_ERROR)  // 如果接受不对
	{
		pFlasher->ShowFlashInfo(_T("Send flash command failed. Please retry."), TRUE);
		::closesocket(sockSend);
		goto FINISH;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	char tempBuf[128] = {0};
	int nCount = 0;
	int nSendSize = 128;

	while(1)
	{		
		memcpy(tempBuf, pFlasher->m_pFileBuf+nCount, nSendSize);
	
		int nRet = ::send(sockSend, tempBuf, nSendSize,0 );

		nCount+= nSendSize;
		if (nCount > pFlasher->m_nBufSize)
		{
			nSendSize =128-(nCount - pFlasher->m_nBufSize);
			if (nSendSize <= 0)
			{

				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 结束标记
	nRet = ::send(sockSend, (char*)(TCPFLASH_INPUT_END), 7, 0);

	//////////////////////////////////////////////////////////////////////////
	closesocket(sockSend);

FINISH:
	pFlasher->NotifyFlashFinish();
	
	return 1;
}



void CTCPFlasher::ShowFlashInfo(const CString& strInfo, BOOL bReplace)
{
	int nCount = strInfo.GetLength();
	WCHAR* strNew = new WCHAR[nCount+1];
	ZeroMemory(strNew, (nCount+1)*sizeof(WCHAR));
	LPCTSTR str = LPCTSTR(strInfo);
	memcpy(strNew, str, nCount*sizeof(WCHAR));

	//g_strFlashInfo = strInfo;

	int nRet = 0;
	if (bReplace)
	{
		nRet =PostMessage(m_pParent->m_hWnd, WM_REPLACE_STATUSINFO, 0,LPARAM(strNew));
	}
	else
	{
		nRet =PostMessage(m_pParent->m_hWnd, WM_ADD_STATUSINFO, 0, LPARAM(strNew));
	}

}


void CTCPFlasher::NotifyFlashFinish()
{
	int	nRet =PostMessage(m_pParent->m_hWnd, WM_FLASH_FINISH, 0, 0);
}