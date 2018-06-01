/********************************************************************
	created:	2011/04/11
	created:	11:4:2011   15:26
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\NetWriter.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	NetWriter
	file ext:	h
	author:		zgq
	
	purpose:	 网络flash类，目前使用的是TFTP，也可以使用TCP等。根据需要。这个类
				做为包装类。提供对外一致的方法来调用。
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
