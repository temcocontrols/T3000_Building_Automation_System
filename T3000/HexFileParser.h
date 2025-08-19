#pragma once
#include "global_struct.h"
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
	// Set filename, including absolute path
	// 设置文件名，包含绝对路径
	void SetFileName(const CString& strFileName);
	// Get valid data from the read Hex file, parameters are buffer and buffer length.
	// 获得读取的Hex文件的有效数据，参数是缓冲区和缓冲区长度。
	// The return value is the actual bytes used in the data buffer. i.e.: return int < nLen
	// 返回的是数据缓冲区中实际使用的字节数。即：return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);
    BOOL Is_RAM_HEXType();
	//Hex file type, 00----data record, 01----extended segment address record, 02----extended linear address record
	//Hex文件类型，00 ----数据记录01 ----扩展段地址记录02 ----扩展线性地址记录
	HEXFILE_FORMAT GetHexFileFormatType();

	// Get the segment length collection of extended hex files.
	// 获得扩展hex 文件的段长度集合。
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
	void Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize);
	Bin_Info        global_fileInfor;
protected:
	// Get the type of hex file
	// 获得hex文件的类型
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC checksum
	// CRC 校验
	BOOL DoCRC( unsigned char* szBuf, int nLen);
	// Helper function to get high address
	// 辅助函数，获得高位地址
	WORD GetHighAddrFromFile(const CString& strLine);
	// Read a line from the file and put it in the buffer
	// 从文件中读取一行，放到缓冲区
	BOOL ReadLineFromFile(CFile& file, char* pBuffer);

	// Read hex file in normal mode
	// 读取normal模式的hex文件
	int ReadNormalHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extend mode
	// extend模式
	int ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extended linear mode
	// 扩展线性模式的
	int ReadExtLinearHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
    
	
private:

	CString				m_strFileName;
	//int					m_nBufLen;

	//Hex file type, 00----data record, 01----extended segment address record, 02----extended linear address record
	HEXFILE_FORMAT	m_nHexFileType;		//Hex文件类型，00 ----数据记录01 ----扩展段地址记录02 ----扩展线性地址记录

	vector<int>				m_szFlags;
	unsigned	char m_DeviceInfor[20];
	UINT m_ProductModel;
	float m_softwareRev;
	CString m_ProductName;
	CString m_ChipName;
	UINT m_ChipSize;
    BOOL m_IsRAM;
};
