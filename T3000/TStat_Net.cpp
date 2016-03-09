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
    case PM_CM5:
	    strProductName="CM5_Ethernet";
		break;
	case PM_MINIPANEL:
	    strProductName="MiniPanel";
		break;
    case PM_CO2_NET:
        strProductName="CO2 Net";
        break;
	case PM_TSTATRUNAR:
		strProductName="TStatRunar";
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
   
    case PM_AirQuality:
        strProductName="Air Quality";
        break;
    case PM_PRESSURE:
        strProductName="Pressure Sensor";
        break;
    case PM_HUMTEMPSENSOR:
        strProductName="TstatHUM";
        break;
    case PM_HUM_R:
        strProductName="HUM-R";
        break;
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
 
 
        //20120424
  
   
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

   
 
   
   
 
 
 
 
 
  
  
 
    
	default:
		strProductName="WIFI";

		break;
	}

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