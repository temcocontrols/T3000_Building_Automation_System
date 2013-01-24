#pragma once
#include "tstatbase.h"

class CTStat_Net : public CTStatBase
{
public:
	CTStat_Net(void);
	~CTStat_Net(void);

	virtual int ReadOneReg( int nRegAddr, OUT int& nVal);
	virtual int ReadOneReg( int nRegAddr);
	virtual int WriteOneReg( int nRegAddr, IN int& nVal);
	virtual int ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal);
	virtual int WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal);
	// 
	virtual BOOL IsOnLine();	
	//---------------------------------------------------------------
	// 
	// 
	virtual int GetComPort()const; 
	virtual void SetComPort(int nComPort);

	// 
	virtual DWORD GetIPAddr()const; 
	CString GetIPAddrStr()const;
	virtual void SetIPAddr(DWORD dwIDAddr);
	virtual void SetIPAddr(char* szIPAddr);
	//virtual void SetIPAddr(const CString& strIPAddr);
	// 
	virtual int GetIPPort()const; 
	virtual void SetIPPort(int nPort);
	// 
	virtual int GetBaudRate()const; 
	virtual void SetBaudRate(int nBaudRate);
	//----------------------------------------------------------------
	// 以下两个才需要去读写寄存器
	virtual int WriteDevID(int nID)const;	
	virtual int ReadDevID(int& nID);
	CString GetProductName();
	BOOL CTStat_Net::operator==(const CTStat_Net& dev);
protected:

	int				m_nBaudRate;				// 
	int				m_nComPort;				// 
	DWORD		m_dwIPAddr;				// 
	int				m_nPort;					// 

	
	
};
