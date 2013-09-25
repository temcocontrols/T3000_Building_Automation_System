#include "StdAfx.h"
#include "TStat_Net.h"
#include "TStatBase.h"
#include "define.h"
CTStat_Net::CTStat_Net(void)
:m_nBaudRate(0),				// ������
m_nComPort(0),				// ����	
m_dwIPAddr(0),				// IP Address
m_nPort(0)					// ��ǰ�˿�
//CTStatBase::m_nProductType(100)				// ��ƷID
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


// Ҳ����ͨ����д�Ĵ�����ʵ��,���������Ҫʵʱ�ķ�ӳ����״̬
BOOL CTStat_Net::IsOnLine()
{
	return FALSE;
}


// ���ں�
int CTStat_Net::GetComPort()const
{
	return m_nComPort;
}

void CTStat_Net::SetComPort(int nComPort)
{
	m_nComPort = nComPort;
}

// IP��ַ
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

// void CTStat_Net::SetIPAddr(const CString& strIPAddr)
// {
// 	
// }

// ����˿�
int CTStat_Net::GetIPPort()const
{
	return m_nPort;
}

void CTStat_Net::SetIPPort(int nPort)
{
	m_nPort = nPort;
}

// ������
int CTStat_Net::GetBaudRate()const
{
	return m_nBaudRate;
}
void CTStat_Net::SetBaudRate(int nBaudRate)
{
	m_nBaudRate = nBaudRate;
}


//----------------------------------------------------------------
// ������������Ҫȥ��д�Ĵ���

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
		//&&  m_nPort == dev.m_nProductID					// ��ǰ�˿�
		//&&  m_nProductID == dev.m_nProductID				// ��ƷID
		);	
}

CString CTStat_Net::GetProductName()
{
	CString strProductName;
	switch(m_nProductType)
	{
	case PM_NC:
		strProductName="NC";
		break;
//20120424
	case PM_LightingController:
		strProductName = "LC";
		break;
//20120424
    case PM_CM5:
	    strProductName="CM5_Ethernet";
		break;
	case PM_MINIPANEL:
	    strProductName="MiniPanel";
		break;
	default:
		strProductName="WIFI";

		break;
	}

	CString strID;
	//strID.Format(_T("%ul"), m_dwSerialID);//20120424
	strID.Format(_T("%u"), m_dwSerialID);//20120424  //scan ϵ�к��ܶ��l

	CString strDevID;
	strDevID.Format(_T("%d"), m_nDevID);

	strProductName = strProductName+_T(":")+strID+_T("--")+strDevID;

	return strProductName;

}