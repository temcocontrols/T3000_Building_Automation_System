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
	// 
	void SetFileName(const CString& strFileName);
	// Hex
	// return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);
    BOOL Is_RAM_HEXType();
	//Hex00 ----01 ----02 ----
	HEXFILE_FORMAT GetHexFileFormatType();

	// hex 
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
	void Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize);
	Bin_Info        global_fileInfor;
protected:
	// hex
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC 
	BOOL DoCRC( unsigned char* szBuf, int nLen);
	// 
	WORD GetHighAddrFromFile(const CString& strLine);
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

	HEXFILE_FORMAT	m_nHexFileType;		//Hex00 ----01 ----02 ----

	vector<int>				m_szFlags;
	unsigned	char m_DeviceInfor[20];
	UINT m_ProductModel;
	float m_softwareRev;
	CString m_ProductName;
	CString m_ChipName;
	UINT m_ChipSize;
    BOOL m_IsRAM;
};
