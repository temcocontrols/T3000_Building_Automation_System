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
	// �����ļ�������������·��
	void SetFileName(const CString& strFileName);
	// Get valid data from read hex file, parameters are buffer and buffer length
	// Return the actual number of bytes used in the data buffer, i.e.: return int < nLen
	// ��ö�ȡ��Hex�ļ�����Ч���ݣ������ǻ������ͻ��������ȡ�
	// ���ص������ݻ�������ʵ��ʹ�õ��ֽ���������return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);
    int GetFileType();
	// Hex file type: 00 ---- data record, 01 ---- extended segment address record, 02 ---- extended linear address record
	//Hex�ļ����ͣ�00 ----���ݼ�¼01 ----��չ�ε�ַ��¼02 ----��չ���Ե�ַ��¼
	HEXFILE_FORMAT GetHexFileFormatType();

	// Get extended hex file section length collection
	// �����չhex �ļ��Ķγ��ȼ��ϡ�
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
	void Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize);
	// Hex file type: 00 ---- data record, 01 ---- extended segment address record, 02 ---- extended linear address record
	HEXFILE_FORMAT	m_nHexFileType;		//Hex�ļ����ͣ�00 ----���ݼ�¼01 ----��չ�ε�ַ��¼02 ----��չ���Ե�ַ��¼
protected:
	// Get hex file type
	// ���hex�ļ�������
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC verification
	// CRC У��
	BOOL DoCRC( TS_UC* szBuf, int nLen);
	// Helper function, get high address
	// ������������ø�λ��ַ
	WORD GetHighAddrFromFile(const CString& strLine);
    int GetFileTypeFromLine(const CString& strLine);
	// Read a line from file and put it in buffer
	// ���ļ��ж�ȡһ�У��ŵ�������
	BOOL ReadLineFromFile(CFile& file, char* pBuffer);

	// Read normal mode hex file
	// ��ȡnormalģʽ��hex�ļ�
	int ReadNormalHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extended mode
	// extendģʽ
	int ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// Extended linear mode
	// ��չ����ģʽ��
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
