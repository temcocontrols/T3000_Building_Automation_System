#pragma once
#include "ConfigFileHandler.h"

class CFlashSN
{
public:
	CFlashSN(void);
	virtual ~CFlashSN(void);

	void SetComport(int nComport);
	void SetIPPort(int nIPPort);
      void SetBrandrate(int nBaudrate);
	void SetIPAddress(CString dwIPAddr);
	void SetFlashTpye(int nFlashType);
    void SetMBID(int ID);
	//void SetFlashParam(const CString	& strProductModel, int nHardWareVersion,int nProductID);
	void SetFlashParam(const CString	& strProductModel, int nHardWareVersion,int nProductID,int nsoftversion,const CString & n_username);
	BOOL StartWriteSN();
	int  GetSerialNumber();
	void SetConfigFileHandler(CConfigFileHandler* pFileHandler);

protected:
	unsigned long GetNewSerialNumber();
	//unsigned long Get_SerialNumber_From_Server();
	bool Connect_To_Server(CString strIPAdress,short nPort);
	unsigned long CFlashSN::Get_SerialNumber_From_Server(CString nipaddress,
		int nipport,
		CString nProductName,
		unsigned char nproductid,
		unsigned short nfirwareversion,
		unsigned short	nhardwareversion, 
		CString nusername);
         int     m_nBraudrate;
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
	CString m_username;
	int m_nsoftversion;

	CConfigFileHandler* m_pFileHandler;
	
};
