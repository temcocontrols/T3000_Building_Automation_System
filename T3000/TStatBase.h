#pragma once



class Mainnet_info
{
//////////////////////////////////////////////////////////////////////////
// 描述设备从属关系的类
public:
	Mainnet_info();
	~Mainnet_info();
	DWORD m_dwSerialID;	// 主设备，主网的序列号
	int m_ProductType;		// 主设备，主网的产品类型，NC，CM5等等
	int	 m_nModbusID;		// 主设备，主网的设备地址
	
	CString GetMainnetInfo();
};

class CTStatBase
{
public:
	CTStatBase(void);
	~CTStatBase(void);

	// 最好返回值和返回信息分开
	virtual int ReadOneReg(int nRegAddr, OUT int& nVal) = 0;
	virtual int ReadOneReg(int nRegAddr) = 0;
	virtual int WriteOneReg(int nRegAddr, IN int& nVali) = 0;

	virtual int ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal) = 0;
	virtual int WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal) = 0;

	// 写ID必须写寄存器吗？YES
	virtual int WriteDevID(int nID)const=0;	
	virtual int ReadDevID(int& nID)=0;

	// 也必须通过读写寄存器来实现
	virtual BOOL IsOnLine() = 0;	
	
	//---------------------------------------------------------------------------

	virtual int GetDevID()const;	
	virtual void SetDevID(int nID);

	int GetProductType()const;		// 直接返回变量，通常只有第一次取值时才需要读寄存器，那么不再设置专门的读函数
	void SetProductType(int nProductType); // 类型出厂时都已经确定不再更改。
	
	float GetHardwareVersion()const; // 同上
	void SetHardwareVersion(float fHardwareVersion); // 同上

	float GetSoftwareVersion()const; // 软件可以更新，因此。。。视具体情况而定
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
	unsigned int	m_dwSerialID;				// 每个产品的序列号ID，出厂时设定
	int			m_nDevID;					// 设备ID，当设备联网工作时的ID，这个内容必须与寄存器同步
	BOOL		m_bOnLine;					// 连接状态，TRUE＝在线，FALSE＝掉线
	int			m_nProductType;			//	产品型号，由此判断5A，5B。。。等等，或者是否是NC
	float		m_fHardware_version;	//  硬件版本号
	float		m_fSoftware_version;	//  软件版本号
	bool		m_cus_name;				//1 代表有客户自己的名字;
	CString m_product_cus_name; //客户自定义的名字;
	CString	m_strBuildingName;		// 所属建筑
	CString	m_strFloorName;			// 所属楼层
	CString	m_strRoomName;		// 所属房间
	CString   m_strSubnetName;       // 子网名

};
