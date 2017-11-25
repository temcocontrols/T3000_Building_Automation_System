#include "StdAfx.h"
#include "TStat_Net.h"
#include "TStatBase.h"
#include "define.h"
CTStat_Net::CTStat_Net(void)
:m_nBaudRate(0),				// 波特率
m_nComPort(0),				// 串口	
m_dwIPAddr(0),				// IP Address
m_nPort(0)					// 当前端口
//CTStatBase::m_nProductType(100)				// 产品ID
{

}



CTStat_Net::~CTStat_Net(void)
{

}


int CTStat_Net::ReadOneReg(int nRegAddr, OUT int& nVal)
{

	return 0;
}

int CTStat_Net::ReadOneReg(int nRegAddr)
{
	return 0;
}

int CTStat_Net::WriteOneReg(int nRegAddr, IN int& nVal)
{
	return 0;
}

int CTStat_Net::ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal)
{

	return 0;
}

int CTStat_Net::WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal)
{

	return 0;
}


// 也必须通过读写寄存器来实现,这个函数需要实时的反映在线状态
BOOL CTStat_Net::IsOnLine()
{
	return FALSE;
}


// 串口号
int CTStat_Net::GetComPort()const
{
	return m_nComPort;
}

void CTStat_Net::SetComPort(int nComPort)
{
	m_nComPort = nComPort;
}

// IP地址
DWORD CTStat_Net::GetIPAddr()const
{
	return m_dwIPAddr;
}

CString CTStat_Net::GetIPAddrStr()const
{
	m_dwIPAddr;
	in_addr ia;
	ia.S_un.S_addr = m_dwIPAddr;
	char* sz = inet_ntoa(ia);
	CString strIP = CString(sz);
	
	return strIP;
}

void CTStat_Net::SetIPAddr(DWORD dwIDAddr)
{
	m_dwIPAddr = dwIDAddr;
}

void CTStat_Net::SetIPAddr(char* szIPAddr)
{
	m_dwIPAddr = inet_addr(szIPAddr);
}

int CTStat_Net::GetProtocol() const
{
	return m_nprotocol;
}
void CTStat_Net::SetProtocol(int nProtocol)
{
	m_nprotocol = nProtocol;
}


// void CTStat_Net::SetIPAddr(const CString& strIPAddr)
// {
// 	
// }

// 网络端口
int CTStat_Net::GetIPPort()const
{
	return m_nPort;
}

void CTStat_Net::SetIPPort(int nPort)
{
	m_nPort = nPort;
}

// 波特率
int CTStat_Net::GetBaudRate()const
{
	return m_nBaudRate;
}
void CTStat_Net::SetBaudRate(int nBaudRate)
{
	m_nBaudRate = nBaudRate;
}

void CTStat_Net::SetNetworkCardAddress(CString networkcardAddress){
NetworkCard_Address=networkcardAddress;
}
CString CTStat_Net::GetNetworkCardAddress(){
return NetworkCard_Address;
}
//----------------------------------------------------------------
// 以下两个才需要去读写寄存器

int CTStat_Net:: WriteDevID(int nID)const
{
	return 0;
}

int CTStat_Net::ReadDevID(int& nID)
{
	return 0;
}


BOOL CTStat_Net::operator==(const CTStat_Net& dev)
{
	return (m_nDevID == dev.m_nDevID 
		//		&&  dev.m_bOnLine == dev.m_bOnLine 
		&&  m_nProductType == dev.m_nProductType 
		&&  m_fHardware_version == dev.m_fHardware_version 
		&&  m_fSoftware_version == dev.m_fSoftware_version 

		&&  m_nBaudRate == dev.m_nBaudRate 
		&&  m_nComPort == dev.m_nComPort
		//&&  m_dwIPAddr == dev.m_nProductID				// IP Address
		//&&  m_nPort == dev.m_nProductID					// 当前端口
		//&&  m_nProductID == dev.m_nProductID				// 产品ID
		);	
}

CString CTStat_Net::GetProductName()
{
	CString strProductName=product_map[m_nProductType];
	 

	CString strID;
	//strID.Format(_T("%ul"), m_dwSerialID);//20120424
	strID.Format(_T("%u"), m_dwSerialID);//20120424  //scan 系列号总多出l

	CString strDevID;
	//if(m_nprotocol == PROTOCOL_BACNET_IP)//如果是bacnet ip
	//{
	//	strDevID.Format(_T("%d"), (int)m_fHardware_version);
	//}
	//else
		strDevID.Format(_T("%d"), m_nDevID);

	CString temp_ipaddr;
	temp_ipaddr = GetIPAddrStr();



	strProductName = strProductName+_T(":")+strID+_T("-")+strDevID + _T("-") + temp_ipaddr  ;


	//product_name = GetProductName(m_refresh_net_device_data.at(y).product_id);
	//product_name = product_name + _T(":") + str_serialid + _T("-") + modbusid + _T("-") + str_ip_address;

	return strProductName;

}