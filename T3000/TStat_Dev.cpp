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
	CString strProductName;
	switch(m_nProductType)
	{
	case PM_TSTAT5A:
		strProductName="TStat5A";
		break;
	case PM_TSTAT5B:
		strProductName="TStat5B";
		break;
	case PM_TSTAT5B2:
		strProductName="TStat5B2";
		break;
	case PM_TSTAT5C:
		strProductName="TStat5C";
		break;
	case PM_TSTAT5D:
		strProductName="TStat5D";
		break;
    case PM_TSTAT5E:
        strProductName="TStat5E";
        break;
    case PM_PM5E:
        strProductName="PM5E";
        break;
	case PM_TSTATRUNAR:
		strProductName="TStatRunar";
		break;
	case PM_TSTAT5F:
		strProductName="TStat5F";
		break;
	case PM_TSTAT5G:
		strProductName="TStat5G";
		break;
	case PM_TSTAT5H:
		strProductName="TStat5H";
		break;
	case PM_TSTAT6:
		strProductName="TStat6";
        break;
    case PM_TSTAT5i:
        strProductName="TStat5i";
        break;
    case PM_TSTAT8:
        strProductName="TStat8";
        break;
	case PM_TSTAT7:
		strProductName="TStat7";
		break;
	case PM_NC:
		strProductName="NC";
		break;
	case PM_CM5:
		strProductName ="CM5";
		break;
//20120424
	case PM_LightingController:
		strProductName = "LC";
		break;
	case  PM_CO2_NET:
		strProductName = "CO2 Net";
		break;
	case  PM_CO2_RS485:
		strProductName = "CO2";
		break;
    case  PM_PRESSURE_SENSOR:
        strProductName = "Pressure";
        break;
  
	case  PM_CO2_NODE:
		strProductName = "CO2 Node";
		break;
	case PM_TSTAT6_HUM_Chamber:
		strProductName =g_strHumChamber;
		break;

	case PM_T3PT10 :
	     strProductName="T3-PT10";
		 break;
	case PM_T3IOA :
		strProductName="T3-8O";
			break;
	case PM_T332AI :
		strProductName="T3-32AI";
			break;
	case  PM_T38AI16O :
		strProductName="T3-8AI160";
			break;
	case PM_T38I13O :
		strProductName="T3-8I13O";
			break;
	case PM_T3PERFORMANCE :
		strProductName="T3-Performance";
			break;
	case PM_T34AO :
		strProductName="T3-4AO";
			break;
    case PM_T322AI:
        strProductName="T3-22I";
        break;
    case PM_T38AI8AO6DO:
        strProductName="T3-8AI8AO6DO";
        break;
	case PM_T36CT :
		strProductName="T3-6CT";
			break;
	case PM_MINIPANEL:
		strProductName="MiniPanel";
		break;
    case PM_AirQuality:
        strProductName="Air Quality";
        break;
	case PM_PRESSURE:
	    strProductName="Pressure Sensor";
		break;
	case PM_HUMTEMPSENSOR:
		strProductName="TstatHUM";
		break;
	case PM_CS_SM_AC:
		strProductName="CS-SM-AC";
		break;
	case PM_CS_SM_DC:
		strProductName="CS-SM-DC";
		break;
	case PM_CS_RSM_AC:
		strProductName="CS-RSM-AC";
		break;
	case PM_CS_RSM_DC:
		strProductName="CS-RSM-DC";
		break;
	case PM_HUM_R:
	    strProductName="HUM-R";
		break;
	default:
		strProductName="TStat";
		break;
	}

	CString strID;
	strID.Format(_T("%d"), m_dwSerialID);

	CString strDevID;
	strDevID.Format(_T("%d"), m_nDevID);

	strProductName = strProductName+_T(":")+strID+_T("--")+strDevID;

	return strProductName;
}