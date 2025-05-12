#pragma once
#include "tstatbase.h"

// TStat5 系列设备的基类
class CTStat_Dev :
	public CTStatBase
{
public:
	CTStat_Dev(void);
	~CTStat_Dev(void);

	virtual int ReadOneReg(int nRegAddr, OUT int& nVal);
	virtual int ReadOneReg(int nRegAddr);
	virtual int WriteOneReg(int nRegAddr, IN int& nVal);

	virtual int ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal);
	virtual int WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal);
	// 也必须通过读写寄存器来实现
	virtual BOOL IsOnLine();	

	virtual int WriteDevID(int nID)const;	
	virtual int ReadDevID(int& nID);
	//-------------------------------------------------------------------

	int GetBaudRate()const;
	void  SetBaudRate(int nBaudRate);

	int GetComPort()const;
	void SetComPort(int nComPort);
	
	int GetEPSize()const;
	void SetEPSize(int nEPSize);
	
	int GetProtocol() const;
	void SetProtocol(int nProtocol);

	CString GetProductName();
// 	int GetAddrID()const;
// 	void SetAddrID(int nID);

	
	BOOL CTStat_Dev::operator==(const CTStat_Dev& dev);



public:
	int				m_nprotocol;				//协议
	int				m_nBaudRate;				// 波特率
	int				m_nComPort;				// 串口	
	int				m_nEPSize;					// ??
	//int				m_nAddrID;				// 地址ID，表示在ModBus中的地址==不需要，就是devID
};
