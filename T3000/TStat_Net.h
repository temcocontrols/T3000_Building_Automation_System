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

	// 也必须通过读写寄存器来实现
	virtual BOOL IsOnLine();	

	//---------------------------------------------------------------
	// 以下仅仅是对象操作

	// 串口号
	virtual int GetComPort()const; 
	virtual void SetComPort(int nComPort);

	// IP地址
	virtual DWORD GetIPAddr()const; 
	CString GetIPAddrStr()const;
	virtual void SetIPAddr(DWORD dwIDAddr);
	virtual void SetIPAddr(char* szIPAddr);
	//virtual void SetIPAddr(const CString& strIPAddr);

	// 网络端口
	virtual int GetIPPort()const; 
	virtual void SetIPPort(int nPort);

	int GetProtocol() const;
	void SetProtocol(int nProtocol);


	// 波特率
	virtual int GetBaudRate()const; 
	virtual void SetBaudRate(int nBaudRate);
	//NetworkCard address
	  CString GetNetworkCardAddress(); 
	  void SetNetworkCardAddress(CString networkcardAddress);
	
	
	//----------------------------------------------------------------
	// 以下两个才需要去读写寄存器
	virtual int WriteDevID(int nID)const;	
	virtual int ReadDevID(int& nID);

	CString GetProductName();




	BOOL CTStat_Net::operator==(const CTStat_Net& dev);

protected:
	int				m_nprotocol;				//协议
	int				m_nBaudRate;				// 波特率
	int				m_nComPort;				// 串口	
	DWORD		m_dwIPAddr;				// IP Address
	int				m_nPort;					// 当前端口
	CString NetworkCard_Address;
	
	
};
