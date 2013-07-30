#pragma once
#include "ConfigFileHandler.h"

class CFlashSN
{
public:
	CFlashSN(void);
	virtual ~CFlashSN(void);

	void SetComport(int nComport);
	void SetIPPort(int nIPPort);
	void SetIPAddress(DWORD dwIPAddr);
	void SetFlashTpye(int nFlashType);

	void SetFlashParam(const CString	& strProductModel, int nHardWareVersion);
	BOOL StartWriteSN();

	void SetConfigFileHandler(CConfigFileHandler* pFileHandler);

protected:
	unsigned long GetNewSerialNumber();
	void FlashTstatSN();

protected:
	int		m_nComPort;
	int		m_nMBID;
	int		m_dwIPAddr;
	int		m_nIPPort;

	int		m_nFlashType;   // 0= com, 1=ethernet
	int		m_nSerialNumber;
	CString	m_strProductModel;
	int			m_nProductModel;
	int			m_nHardWareVersion;

	CConfigFileHandler* m_pFileHandler;

};
