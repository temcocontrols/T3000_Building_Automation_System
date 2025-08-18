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
	// �����ļ�������������·��
	void SetFileName(const CString& strFileName);
	// Get valid data from the read Hex file, parameters are buffer and buffer length.
	// ��ö�ȡ��Hex�ļ�����Ч���ݣ������ǻ������ͻ��������ȡ�
	// The return value is the actual bytes used in the data buffer. i.e.: return int < nLen
	// ���ص������ݻ�������ʵ��ʹ�õ��ֽ���������return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);
    BOOL Is_RAM_HEXType();
	//Hex file type, 00----data record, 01----extended segment address record, 02----extended linear address record
	//Hex�ļ����ͣ�00 ----���ݼ�¼01 ----��չ�ε�ַ��¼02 ----��չ���Ե�ַ��¼
	HEXFILE_FORMAT GetHexFileFormatType();

	// Get the segment length collection of extended hex files.
	// �����չhex �ļ��Ķγ��ȼ��ϡ�
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
	void Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize);
	Bin_Info        global_fileInfor;
protected:
	// Get the type of hex file
	// ���hex�ļ�������
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC checksum
	// CRC У��
	BOOL DoCRC( unsigned char* szBuf, int nLen);
	// Helper function to get high address
	// ������������ø�λ��ַ
	WORD GetHighAddrFromFile(const CString& strLine);
	// Read a line from the file and put it in the buffer
	// ���ļ��ж�ȡһ�У��ŵ�������
	BOOL ReadLineFromFile(CFile& file, char* pBuffer);

	// Read hex file in normal mode
	// ��ȡnormalģʽ��hex�ļ�
	int ReadNormalHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extend mode
	// extendģʽ
	int ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extended linear mode
	// ��չ����ģʽ��
	int ReadExtLinearHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
    
	
private:

	CString				m_strFileName;
	//int					m_nBufLen;

	//Hex file type, 00----data record, 01----extended segment address record, 02----extended linear address record
	HEXFILE_FORMAT	m_nHexFileType;		//Hex�ļ����ͣ�00 ----���ݼ�¼01 ----��չ�ε�ַ��¼02 ----��չ���Ե�ַ��¼

	vector<int>				m_szFlags;
	unsigned	char m_DeviceInfor[20];
	UINT m_ProductModel;
	float m_softwareRev;
	CString m_ProductName;
	CString m_ChipName;
	UINT m_ChipSize;
    BOOL m_IsRAM;
};
