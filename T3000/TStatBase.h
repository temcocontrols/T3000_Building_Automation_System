#pragma once



class Mainnet_info
{
//////////////////////////////////////////////////////////////////////////
// 描述设备从属关系的类
public:
	Mainnet_info();
	~Mainnet_info();
	DWORD m_dwSerialID;	// 
	int m_ProductType;		// 
	int	 m_nModbusID;		// 
	
	CString GetMainnetInfo();
};

class CTStatBase
{
public:
	CTStatBase(void);
	~CTStatBase(void);

	// 
	virtual int ReadOneReg(int nRegAddr, OUT int& nVal) = 0;
	virtual int ReadOneReg(int nRegAddr) = 0;
	virtual int WriteOneReg(int nRegAddr, IN int& nVali) = 0;

	virtual int ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal) = 0;
	virtual int WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal) = 0;

	//
	virtual int WriteDevID(int nID)const=0;	
	virtual int ReadDevID(int& nID)=0;

	// 
	virtual BOOL IsOnLine() = 0;	
	
	//---------------------------------------------------------------------------

	virtual int GetDevID()const;	
	virtual void SetDevID(int nID);

	int GetProductType()const;		// 
	void SetProductType(int nProductType); // 
	
	float GetHardwareVersion()const; // 同上
	void SetHardwareVersion(float fHardwareVersion); // 同上

	float GetSoftwareVersion()const; // 
	void SetSoftwareVersion(float fSoftwareVersion); 

	void SetBuildingName(const CString& strBuildingName); 
	CString GetBuildingName()const;

	void SetFloorName(const CString& strFloorName); 
	CString GetFloorName()const;
	
	void SetRoomName(const CString& strRoomName); 
	CString GetRoomName()const;

	void SetSerialID(DWORD dwSerialID); 
	DWORD  GetSerialID()const;

	CString GetSubnetName() const;
	void SetSubnetName(const CString& strSubnetName);
	
	
	Mainnet_info	m_mainnet_info;
protected:
	DWORD	m_dwSerialID;				// 
	int			m_nDevID;					// 
	BOOL		m_bOnLine;					// 
	int			m_nProductType;			//	
	float		m_fHardware_version;	// 
	float		m_fSoftware_version;	//  
	
	CString	m_strBuildingName;		// 
	CString	m_strFloorName;			// 
	CString	m_strRoomName;		// 
	CString   m_strSubnetName;       // 

};
