#include "StdAfx.h"
#include "TStatBase.h"

CTStatBase::CTStatBase(void)
:m_dwSerialID(0),						// 每个产品的序列号ID，出厂时设定
m_nDevID(-1),						// 设备ID，当设备联网工作时的ID，这个内容必须与寄存器同步
m_bOnLine(FALSE),					// 连接状态，TRUE＝在线，FALSE＝掉线
m_nProductType(-1),				//	产品型号，由此判断5A，5B。。。等等，或者是否是NC
m_fHardware_version(0.0),		//  硬件版本号
m_fSoftware_version(0.0),			//  软件版本号

m_strBuildingName(_T("")),		// 所属建筑
m_strFloorName(_T("")),			// 所属楼层
m_strRoomName(_T(""))		// 所属房间
{
}

CTStatBase::~CTStatBase(void)
{
}

int CTStatBase::GetDevID()const
{
	return m_nDevID;
}

void CTStatBase::SetDevID(int nID)
{
	m_nDevID = nID;
}



int CTStatBase::GetProductType()const
{
	return m_nProductType;
}

void CTStatBase::SetProductType(int nProductType)
{
	m_nProductType = nProductType;
}

float CTStatBase::GetHardwareVersion()const
{
	return m_fHardware_version;
}
void CTStatBase::SetHardwareVersion(float fHardwareVersion)
{
	m_fHardware_version = fHardwareVersion;
}

float CTStatBase::GetSoftwareVersion()const
{
	return m_fSoftware_version;
}

void CTStatBase::SetSoftwareVersion(float fSoftwareVersion)
{
	m_fSoftware_version = fSoftwareVersion;
}



void CTStatBase::SetBuildingName(const CString& strBuildingName)
{
	m_strBuildingName = strBuildingName;
}

CString CTStatBase::GetBuildingName()const
{
	return m_strBuildingName;
}

void CTStatBase::SetFloorName(const CString& strFloorName)
{
	m_strFloorName = strFloorName;
}

CString CTStatBase::GetFloorName()const
{
	return m_strFloorName;
}

void CTStatBase::SetRoomName(const CString& strRoomName)
{
	m_strRoomName = strRoomName;

}

CString CTStatBase::GetRoomName()const
{
	return m_strRoomName;
}

void CTStatBase::SetSerialID(unsigned int dwSerialID)
{
	m_dwSerialID = dwSerialID;
}


unsigned int  CTStatBase::GetSerialID()const
{
	return m_dwSerialID;
}

CString CTStatBase::GetSubnetName() const
{
	return m_strSubnetName;
}

void CTStatBase::SetSubnetName(const CString& strSubnetName)
{
	m_strSubnetName = strSubnetName;
}

CString CTStatBase::GetProductCusName() const
{
	return m_product_cus_name;
}

void CTStatBase::SetProductCusName(const CString& strProductCusName)
{
	m_product_cus_name = strProductCusName;
}



Mainnet_info::Mainnet_info()
{
	m_dwSerialID = 0;
	m_ProductType = 0;		
	m_nModbusID = 0;		
}
Mainnet_info::~Mainnet_info()
{

}

CString Mainnet_info::GetMainnetInfo()
{
	CString strInfo;
	strInfo.Format(_T("%d:%d:%d"), m_ProductType, m_dwSerialID, m_nModbusID);

	return strInfo;
}