#include "StdAfx.h"
#include "NCFinder.h"

CNCFinder::CNCFinder(void)
{
}

CNCFinder::~CNCFinder(void)
{
}

int CNCFinder::GetFindNCIP(vector<DWORD>& szIPs)
{
	szIPs = m_szIPs;
	return szIPs.size();
}


bool CNCFinder::FindNC()
{
	const int nBOOTPPORT = 67;
	BYTE szBuf[512];
	ZeroMemory(szBuf, 512);
	SOCKET soFinder = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	SOCKADDR_IN siRecv;
	siRecv.sin_family = AF_INET;
	siRecv.sin_port = htons(nBOOTPPORT);
	siRecv.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(_T("192.168.0.3"))
	int nRetB = bind(soFinder, (SOCKADDR *) &siRecv, sizeof(siRecv));

	int nLen = sizeof(siRecv);



	//////////////////////////////////////////////////////////////////////////
	timeval sTimeout;
	sTimeout.tv_sec = 10;
	sTimeout.tv_usec = 5000;
	fd_set  fdread;
	FD_ZERO (& fdread);
	//FD_SET (m_sock, &readfds);
	FD_SET (soFinder, &fdread);
	while(1)
	{	
		fd_set tempfds = fdread;
		FD_ZERO(& tempfds);
		int nSRet = select(1, &tempfds, NULL, NULL, &sTimeout);
		if (nSRet > 0)	
		{
			int nRet = ::recvfrom(soFinder,(char*)szBuf,512,0,(sockaddr*)&siRecv, &nLen);
			if (nRet == SOCKET_ERROR )
			{				
				//out put error message
				return FALSE;
			}
			DWORD dwIP = GetIPFromBuf((char*)szBuf, nRet);
			m_szIPs.push_back(dwIP);
		}		
		else
		{
			return TRUE;
		}
		closesocket(soFinder);
	}
	return TRUE;
}


DWORD CNCFinder::GetIPFromBuf(char* pBuf, int nLen)
{
	DWORD dwIP = 0;

	((BYTE*)&dwIP)[0] = pBuf[12];
	((BYTE*)&dwIP)[1] = pBuf[13];
	((BYTE*)&dwIP)[2] = pBuf[14];
	((BYTE*)&dwIP)[3] = pBuf[15];


	return dwIP;	
}