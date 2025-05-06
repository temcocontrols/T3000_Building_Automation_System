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
	// 
	void SetFileName(const CString& strFileName);
	// Hex
	// return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);
    int GetFileType();
	//Hex00 ----01 ----02 ----
	HEXFILE_FORMAT GetHexFileFormatType();

	// hex 
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
	void Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize);
	HEXFILE_FORMAT	m_nHexFileType;		//Hex00 ----01 ----02 ----
protected:
	// hex
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC 
	BOOL DoCRC( TS_UC* szBuf, int nLen);
	// 
	WORD GetHighAddrFromFile(const CString& strLine);
    int GetFileTypeFromLine(const CString& strLine);
	// 
	BOOL ReadLineFromFile(CFile& file, char* pBuffer);

	// normalhex
	int ReadNormalHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// extend
	int ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// 
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
