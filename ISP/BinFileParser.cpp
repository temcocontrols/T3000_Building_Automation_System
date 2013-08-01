#include "StdAfx.h"
#include "BinFileParser.h"

CBinFileParser::CBinFileParser(void)
{
}

CBinFileParser::~CBinFileParser(void)
{
}



void CBinFileParser::SetBinFileName(const CString& strFileName)
{
	m_strFileName = strFileName;
}

int CBinFileParser::GetBinFileBuffer(char* pFileBuf, int nFileBufLen)
{
	int nFileBufSize=0;
	CFile binFile;
	if(binFile.Open(m_strFileName,CFile::modeRead))
	{
		const int BUF_LEN = 1024;

		TS_UC pBuf[BUF_LEN] = {'\0'};
		int nRet = 0;
		int nCount = 0;
		binFile.Seek(0, CFile::begin);
		while( (nRet = binFile.Read(pBuf, BUF_LEN)) != 0 )
		{
			memcpy(pFileBuf + nCount, pBuf, nRet);
			nCount += nRet;	
			if (nRet < BUF_LEN)
			{ // 说明读完了
				break;
			}				
			ZeroMemory(pBuf, BUF_LEN);
		}

		nFileBufSize = nCount;
		binFile.Close();
	}

	return nFileBufSize;
}

