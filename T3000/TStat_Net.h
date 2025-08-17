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

	// Ҳ����ͨ����д�Ĵ�����ʵ�� - Must also be implemented through reading and writing registers
	virtual BOOL IsOnLine();	

	//---------------------------------------------------------------
	// ���½����Ƕ������ - The following are object operations only

	// ���ں� - Serial port number
	virtual int GetComPort()const; 
	virtual void SetComPort(int nComPort);

	// IP��ַ - IP address
	virtual DWORD GetIPAddr()const; 
	CString GetIPAddrStr()const;
	virtual void SetIPAddr(DWORD dwIDAddr);
	virtual void SetIPAddr(char* szIPAddr);
	//virtual void SetIPAddr(const CString& strIPAddr);

	// ����˿� - Network port
	virtual int GetIPPort()const; 
	virtual void SetIPPort(int nPort);

	int GetProtocol() const;
	void SetProtocol(int nProtocol);


	// ������ - Baud rate
	virtual int GetBaudRate()const; 
	virtual void SetBaudRate(int nBaudRate);
	//NetworkCard address
	  CString GetNetworkCardAddress(); 
	  void SetNetworkCardAddress(CString networkcardAddress);
	
	
	//----------------------------------------------------------------
	// ������������Ҫȥ��д�Ĵ��� - The following two need to read and write registers
	virtual int WriteDevID(int nID)const;	
	virtual int ReadDevID(int& nID);

	CString GetProductName();




	BOOL CTStat_Net::operator==(const CTStat_Net& dev);

protected:
	int				m_nprotocol;				//Э�� - Protocol
	int				m_nBaudRate;				// ������ - Baud rate
	int				m_nComPort;				// ���� - Serial port	
	DWORD		m_dwIPAddr;				// IP Address
	int				m_nPort;					// ��ǰ�˿� - Current port
	CString NetworkCard_Address;
	
	
};
