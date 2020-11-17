#include "StdAfx.h"
#include "BinFileParser.h"
#include "global_function.h"
extern Bin_Info        global_fileInfor;
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
CString CBinFileParser::GetBinInfor()
{
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
                CString temp_identify;
                //判断AXIS 0x100位置

                //判断ESP8266 0x200位置

                m_strASIX.Format(_T("%C%C%C%C"),pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
                if (m_strASIX.CompareNoCase(_T("ASIX") ) == 0)
                {
                    for (int i = 0; i<20; i++) //固定长度20个
                    {
                        if (pBuf[256 + i] != 0)
                        {
                            CString temp;
                            temp.Format(_T("%C"), pBuf[256 + i]);
                            m_strProductName += temp;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else //在去0x200位置判断是不是ESP8266的固件;
                {
                    	MultiByteToWideChar( CP_ACP, 0, (char *)(&pBuf[512]),20,temp_identify.GetBuffer(MAX_PATH), MAX_PATH );
                        temp_identify.ReleaseBuffer();
                        if (temp_identify.Find(_T("Temco")) != -1)
                            m_strProductName = temp_identify;
                        else
                        {
                            m_strProductName.Empty();
                            return -1;
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


	int ret_find_in_0x100 = 0;
	for (int i=0;i<2;i++)
	{
		memset(&global_fileInfor,0,sizeof(Bin_Info));
			if(i==0)
				memcpy(&global_fileInfor,&pFileBuf[0x100],sizeof(Bin_Info));
			else if(i == 1)
			    memcpy(&global_fileInfor,&pFileBuf[0x200],sizeof(Bin_Info));
			else
				return -1;

		//if(strlen(global_fileInfor.product_name) > 200)
		//	nFileBufSize = -1;
		//return NO_VERSION_INFO;
		char temocolog[6];
		memcpy_s(temocolog,5,global_fileInfor.company,5);
		temocolog[5] = 0;

		CString Temco_logo;
		MultiByteToWideChar( CP_ACP, 0, (char *)temocolog,
			(int)strlen(temocolog)+1,
			Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
		Temco_logo.ReleaseBuffer();
		Temco_logo.MakeUpper();
		if(Temco_logo.CompareNoCase(_T("TEMCO")) != 0&&Temco_logo.CompareNoCase(L"CO2")!=0)
		{
			//return NO_VERSION_INFO;
			ret_find_in_0x100 = -1;
		}
		else
			return nFileBufSize;
	}





    return nFileBufSize;
}

