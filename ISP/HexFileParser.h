#pragma once

enum HEXFILE_FORMAT
{
	HEXFILE_DATA = 0,
	HEXFILE_SECADDR = 1,
	HEXFILE_LINERADDR = 2,

};


class CHexFileParser
{
public:
	CHexFileParser(void);
	virtual ~CHexFileParser(void);
    CString Get_HexInfor();
	// Set filename including absolute path
	// 设置文件名，包含绝对路径
	void SetFileName(const CString& strFileName);
	// Get valid data from read hex file, parameters are buffer and buffer length
	// Return the actual number of bytes used in the data buffer, i.e.: return int < nLen
	// 获得读取的Hex文件的有效数据，参数是缓冲区和缓冲区长度。
	// 返回的是数据缓冲区中实际使用的字节数。即：return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);
    int GetFileType();
	// Hex file type: 00 ---- data record, 01 ---- extended segment address record, 02 ---- extended linear address record
	//Hex文件类型，00 ----数据记录01 ----扩展段地址记录02 ----扩展线性地址记录
	HEXFILE_FORMAT GetHexFileFormatType();

	// Get extended hex file section length collection
	// 获得扩展hex 文件的段长度集合。
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
	void Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize);
	// Hex file type: 00 ---- data record, 01 ---- extended segment address record, 02 ---- extended linear address record
	HEXFILE_FORMAT	m_nHexFileType;		//Hex文件类型，00 ----数据记录01 ----扩展段地址记录02 ----扩展线性地址记录
protected:
	// Get hex file type
	// 获得hex文件的类型
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC verification
	// CRC 校验
	BOOL DoCRC( TS_UC* szBuf, int nLen);
	// Helper function, get high address
	// 辅助函数，获得高位地址
	WORD GetHighAddrFromFile(const CString& strLine);
    int GetFileTypeFromLine(const CString& strLine);
	// Read a line from file and put it in buffer
	// 从文件中读取一行，放到缓冲区
	BOOL ReadLineFromFile(CFile& file, char* pBuffer);

	// Read normal mode hex file
	// 读取normal模式的hex文件
	int ReadNormalHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extended mode
	// extend模式
	int ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extended linear mode
	// 扩展线性模式的
	int ReadExtLinearHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
    

private:

	CString				m_strFileName;
	//int					m_nBufLen;

	

	vector<int>				m_szFlags;
	unsigned	char m_DeviceInfor[20];
	UINT m_ProductModel;
	float m_softwareRev;
	CString m_ProductName;
	CString m_ChipName;
	UINT m_ChipSize;
    int m_file_type;
};

#define HEX_TYPE_ASIX     0
#define HEX_TYPE_ARM_32K  1
#define HEX_TYPE_ARM_64K  2
