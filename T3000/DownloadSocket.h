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
	void SetDownloadResults(int ret_results);
	int GetDownloadResults();
	HWND m_parent_hwnd;
	int m_results;
	virtual void OnClose(int nErrorCode);
	bool is_connect;
};

#define DOWNLOAD_RESULTS_UNKNOW 1
#define DOWNLOAD_RESULTS_PASS	2
#define DOWNLOAD_RESULTS_FAILED 3



