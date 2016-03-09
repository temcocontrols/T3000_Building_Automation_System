#include "StdAfx.h"
#include "BinFileParser.h"
#include "globle_function.h"
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
CString CBinFileParser::GetBinInfor(){
if (m_strASIX.CompareNoCase(_T("ASIX"))!=-1)
{
return m_strASIX+_T(" ")+m_strProductName;
}
return m_strProductName;
}
int CBinFileParser::GetBinFileBuffer(char* pFileBuf, int nFileBufLen)
{
	int nFileBufSize=0;
	CFile binFile;
	if(binFile.Open(m_strFileName,CFile::modeRead))
	{
		const int BUF_LEN = 1024;
        int linenum=1;
		TS_UC pBuf[BUF_LEN] = {'\0'};
		int nRet = 0;
		int nCount = 0;
		binFile.Seek(0, CFile::begin);
		while( (nRet = binFile.Read(pBuf, BUF_LEN)) != 0 )
		{
            if (linenum==1)
            {
               m_strASIX.Format(_T("%C%C%C%C"),pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
               for (int i=0;i<20;i++)//固定长度20个
               {
                 if (pBuf[256+i]!=0)
                 {
                 CString temp;
                 temp.Format(_T("%C"),pBuf[256+i]);
                 m_strProductName+=temp;
                 }
                 else
                 {
                 break;
                 }
               }
               // m_strProductName.Format(_T("%C%C%C%C%C%C%C%C%C%C%C%C"),pBuf[256+0],pBuf[256+1],pBuf[256+2],pBuf[256+3],pBuf[256+4],pBuf[256+5],pBuf[256+6],pBuf[256+7],pBuf[256+8],pBuf[256+9],pBuf[256+10],pBuf[256+11]);
            }
             
			memcpy(pFileBuf + nCount, pBuf, nRet);
			nCount += nRet;	
			if (nRet < BUF_LEN)
			{  
				break;
			}				
			ZeroMemory(pBuf, BUF_LEN);
              ++linenum;
		}

		nFileBufSize = nCount;
		binFile.Close();
	}
// 	Bin_Info test_info;
// 	Get_Binfile_Information(m_strFileName.GetString(),test_info);
	return nFileBufSize;
}

