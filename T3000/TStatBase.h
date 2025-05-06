#pragma once



class Mainnet_info
{
//////////////////////////////////////////////////////////////////////////
// 
public:
	Mainnet_info();
	~Mainnet_info();
	DWORD m_dwSerialID;	// 
	int m_ProductType;		// NCCM5
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

	// IDYES
	virtual int WriteDevID(int nID)const=0;	
	virtual int ReadDevID(int& nID)=0;

	// 
	virtual BOOL IsOnLine() = 0;	
	
	//---------------------------------------------------------------------------

	virtual int GetDevID()const;	
	virtual void SetDevID(int nID);

	int GetProductType()const;		// 
	void SetProductType(int nProductType); // 
	
	float GetHardwareVersion()const; // 
	void SetHardwareVersion(float fHardwareVersion); // 

	float GetSoftwareVersion()const; // 
	void SetSoftwareVersion(float fSoftwareVersion); 

	void SetBuildingName(const CString& strBuildingName); 
	CString GetBuildingName()const;

	void SetFloorName(const CString& strFloorName); 
	CString GetFloorName()const;
	
	void SetRoomName(const CString& strRoomName); 
	CString GetRoomName()const;

	void SetSerialID(unsigned int dwSerialID); 
	unsigned int  GetSerialID()const;

	CString GetSubnetName() const;
	void SetSubnetName(const CString& strSubnetName);
	
	CString GetProductCusName() const;
	void SetProductCusName(const CString& strProductCusName);
	
	
	Mainnet_info	m_mainnet_info;
public:
	unsigned int	m_dwSerialID;				// ID
	int			m_nDevID;					// IDID
	BOOL		m_bOnLine;					// TRUEFALSE
	int			m_nProductType;			//	5A5BNC
	float		m_fHardware_version;	//  
	float		m_fSoftware_version;	//  
	bool		m_cus_name;				//1 ;
	CString m_product_cus_name; //;
	CString	m_strBuildingName;		// 
	CString	m_strFloorName;			// 
	CString	m_strRoomName;		// 
	CString   m_strSubnetName;       // 

};
