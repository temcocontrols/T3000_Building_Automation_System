#pragma once




class CTCPFlasher
{
public:
	CTCPFlasher(void);
	virtual ~CTCPFlasher(void);

	void SetIPAddr(DWORD dwIP);

	void SetIPPort(int nPort);

	void SetFileBuffer(char* pFileBuf, int nBufSize);

	void SetParentWnd(CWnd* pWnd);

	BOOL StartFlash();

	void ShowFlashInfo(const CString& strInfo, BOOL bReplace);

	void NotifyFlashFinish();

protected:





public:
	char*				m_pFileBuf;
	int					m_nBufSize;
	DWORD			m_dwIP;
	int					m_nIPPort;
protected:


	CWnd*			m_pParent;
		
	SOCKET			m_socket;

	


};
