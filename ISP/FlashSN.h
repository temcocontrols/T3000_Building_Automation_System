#pragma once
#include "ConfigFileHandler.h"

class CFlashSN
{
public:
	CFlashSN(void);
	virtual ~CFlashSN(void);

	void SetComport(int nComport);
	void SetIPPort(int nIPPort);

	void SetIPAddress(CString dwIPAddr);
	void SetFlashTpye(int nFlashType);
    void SetMBID(int ID);
	void SetFlashParam(const CString	& strProductModel, int nHardWareVersion,int nProductID);
	BOOL StartWriteSN();
	int  GetSerialNumber();
	void SetConfigFileHandler(CConfigFileHandler* pFileHandler);

protected:
	unsigned long GetNewSerialNumber();
	void FlashTstatSN();
	void FlashNCSN();
public:
	int		m_nComPort;
	int		m_nMBID;
	CString		m_dwIPAddr;
	int		m_nIPPort;
	
	int		m_nFlashType;   // 0= com, 1=ethernet
	int		m_nSerialNumber;
	CString	m_strProductModel;
	int			m_nProductModel;
	int			m_nHardWareVersion;

	CConfigFileHandler* m_pFileHandler;
	
};
