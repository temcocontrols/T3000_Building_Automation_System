#include "StdAfx.h"
#include "NetWriter.h"



UINT _TFTPFlashFunc(PVOID pParam);
CNetWriter::CNetWriter(void)
{
	m_pTftpServer = NULL;
	m_pThread = NULL;
}

CNetWriter::~CNetWriter(void)
{
}



BOOL CNetWriter::StartWriteByNet()
{
	m_pThread =  AfxBeginThread(_TFTPFlashFunc, this);
	ASSERT(m_pThread);
	if (m_pThread)
	{
		return TRUE;
	}
	return FALSE;
}



UINT _TFTPFlashFunc(PVOID pParam)
{
	CNetWriter* pWriter = (CNetWriter*)pParam;	

	pWriter->m_pTftpServer = new TFTPServer;
	
	if (pWriter->m_pTftpServer->StartServer())
	{
		return 1;
	}

	return 0;
	
}


BOOL CNetWriter::StopWrite()
{
	ASSERT(m_pTftpServer);
	if (m_pTftpServer)
	{
		//m_pTftpServer->StopFlash();

	}
	return TRUE;
}