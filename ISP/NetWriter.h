/********************************************************************
	created:	2011/04/11
	created:	11:4:2011   15:26
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\NetWriter.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	NetWriter
	file ext:	h
	author:		zgq
	
	purpose:	 ����flash�࣬Ŀǰʹ�õ���TFTP��Ҳ����ʹ��TCP�ȡ�������Ҫ�������
				��Ϊ��װ�ࡣ�ṩ����һ�µķ��������á�
*********************************************************************/
#include "TFTPServer.h"

#pragma once



class CNetWriter
{
public:
	CNetWriter(void);
	virtual ~CNetWriter(void);

	BOOL StartWriteByNet();

	BOOL StopWrite();
	

public:
	TFTPServer*		m_pTftpServer;
	CWinThread*		m_pThread;

};
