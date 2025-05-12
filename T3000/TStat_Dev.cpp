#include "StdAfx.h"
#include "TStat_Dev.h"
//#include "global_variable_extern.h"
#include "define.h"


CTStat_Dev::CTStat_Dev(void)
{
}

CTStat_Dev::~CTStat_Dev(void)
{
}


int CTStat_Dev::ReadOneReg(int nRegAddr, OUT int& nVal)
{
	return 0;
}

int CTStat_Dev::ReadOneReg(int nRegAddr)
{
	return 0;
}

int CTStat_Dev::WriteOneReg(int nRegAddr, IN int& nVal)
{
	return 0;
}

int CTStat_Dev::ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal)
{
	return 0;
}

int CTStat_Dev::WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal)
{
	return 0;
}



// 也必须通过读写寄存器来实现
BOOL CTStat_Dev::IsOnLine()
{
	return FALSE;
}


int CTStat_Dev::GetBaudRate()const
{
	return m_nBaudRate;
}

void CTStat_Dev::SetBaudRate(int nBaudRate)
{
	m_nBaudRate = nBaudRate;
}

int CTStat_Dev::GetComPort()const
{
	return m_nComPort;
}

int CTStat_Dev::GetProtocol() const
{
	return m_nprotocol;
}
void CTStat_Dev::SetProtocol(int nProtocol)
{
	m_nprotocol = nProtocol;
}


void CTStat_Dev::SetComPort(int nComPort)
{
	m_nComPort = nComPort;
}

int CTStat_Dev::GetEPSize()const
{
	return m_nEPSize;	
}

void CTStat_Dev::SetEPSize(int nEPSize)
{
	m_nEPSize = nEPSize;
}

// int CTStat_Dev::GetAddrID()const
// {
// 	return m_nAddrID;
// }
// 
// void CTStat_Dev::SetAddrID(int nID)
// {
// 	m_nAddrID = nID;
// }

BOOL CTStat_Dev::operator==(const CTStat_Dev& dev)
{
	return (m_nDevID == dev.m_nDevID 
//		&&  dev.m_bOnLine == dev.m_bOnLine 
		&&  m_nProductType == dev.m_nProductType 
		&&  m_fHardware_version == dev.m_fHardware_version 
		&&  m_fSoftware_version == dev.m_fSoftware_version 
		&&  m_nBaudRate == dev.m_nBaudRate 
		&&  m_nComPort == dev.m_nComPort 
		//&&  m_nAddrID == dev.m_nAddrID 
		);	

}



int CTStat_Dev:: WriteDevID(int nID)const
{
	return 0;
}

int CTStat_Dev::ReadDevID(int& nID)
{
	return 0;
}


CString CTStat_Dev::GetProductName()
{
	CString strProductName = product_map[m_nProductType];

	CString strID;
	strID.Format(_T("%d"), m_dwSerialID);

	CString strDevID;
	strDevID.Format(_T("%d"), m_nDevID);

	strProductName = strProductName+_T(":")+strID+_T("--")+strDevID;

	return strProductName;
}