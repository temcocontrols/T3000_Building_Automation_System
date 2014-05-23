#pragma once
#include "afxsock.h"
#include <WINSOCK2.H>
#include <afxsock.h>

class DownloadSocket :	public CAsyncSocket
{
public:
	DownloadSocket(void);
	~DownloadSocket(void);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	void SetParentWindow(HWND n_hwnd);

	HWND m_parent_hwnd;
	virtual void OnClose(int nErrorCode);
};

