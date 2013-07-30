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

	// �����ļ�������������·��
	void SetFileName(const CString& strFileName);
	// ��ö�ȡ��Hex�ļ�����Ч���ݣ������ǻ������ͻ��������ȡ�
	// ���ص������ݻ�������ʵ��ʹ�õ��ֽ���������return int < nLen
	int GetHexFileBuffer(IN char* pBuf, IN int nLen);

	//Hex�ļ����ͣ�00 ----���ݼ�¼01 ----��չ�ε�ַ��¼02 ----��չ���Ե�ַ��¼
	HEXFILE_FORMAT GetHexFileFormatType();

	// �����չhex �ļ��Ķγ��ȼ��ϡ�
	int	GetExtendHexFileSectionFlag(vector<int>& szFlags);
protected:
	// ���hex�ļ�������
	HEXFILE_FORMAT	GetHexFileType(CFile& hexFile);
	// CRC У��
	BOOL DoCRC( TS_UC* szBuf, int nLen);
	// ������������ø�λ��ַ
	WORD GetHighAddrFromFile(const CString& strLine);
	// ���ļ��ж�ȡһ�У��ŵ�������
	BOOL ReadLineFromFile(CFile& file, char* pBuffer);

	// ��ȡnormalģʽ��hex�ļ�
	int ReadNormalHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// extendģʽ
	int ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
	// ��չ����ģʽ��
	int ReadExtLinearHexFile(CFile& hexFile, char* pBuf, int nBufLen);	
private:

	CString				m_strFileName;
	//int					m_nBufLen;

	HEXFILE_FORMAT	m_nHexFileType;		//Hex�ļ����ͣ�00 ----���ݼ�¼01 ----��չ�ε�ַ��¼02 ----��չ���Ե�ַ��¼

	vector<int>				m_szFlags;

};
