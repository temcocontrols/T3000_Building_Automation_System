#include "StdAfx.h"
#include "HexFileParser.h"
#include "globle_function.h"



CHexFileParser::CHexFileParser(void)
{
	m_nHexFileType = HEXFILE_DATA;
}

CHexFileParser::~CHexFileParser(void)
{

}


void  CHexFileParser::SetFileName(const CString& strFileName)
{
	m_strFileName = strFileName;
}


//////////////////////////////////////////////////////////////////////////
// ����int��ʵ�ʵ���Ч�ֽ�
int  CHexFileParser::GetHexFileBuffer(char* pBuf, int nLen)
{
	CFile hexFile; //the hex file
	
	int nBufLen = 0;
	if(hexFile.Open(m_strFileName, CFile::modeRead))
	{
		m_nHexFileType = GetHexFileType(hexFile);
		BOOL bRet = TRUE;

		if (m_nHexFileType == 0)
		{			
			CString strTips = _T("Hex file verified okay.");//_T("The Hex File is Normal.");
			//AddStringToOutPuts(strTips);

			nBufLen = ReadNormalHexFile(hexFile, pBuf, nLen);	

		}
		else if(m_nHexFileType == 1)
		{
			CString strTips = _T("The hex file has a Extend Section Address Record(HEX86).");
			//AddStringToOutPuts(strTips);

			nBufLen = ReadExtendHexFile(hexFile, pBuf, nLen);	
		}
		else if (m_nHexFileType == 2)
		{
			CString strTips = _T("The hex file has a Extend Linear Addree Record(HEX86).");
			//AddStringToOutPuts(strTips);


			nBufLen = ReadExtLinearHexFile(hexFile, pBuf, nLen);	
		}		
	}

	
	return nBufLen;

}



// ����7��8���ַ���00ΪNormal��02Ϊ��չ��04Ϊ��չ����
HEXFILE_FORMAT	CHexFileParser::GetHexFileType(CFile& hexFile)
{
	int nRet = 0;
	char chBuf[64] = {0};
	hexFile.Read(chBuf, 12);
	if (chBuf[8] == '0')
	{		
	}
	else if (chBuf[8] == '2')
	{
		nRet = 1;
	}
	else if (chBuf[8] == '4')
	{
		nRet = 2;
	}

	// reset file pointer
	hexFile.SeekToBegin();
	return (HEXFILE_FORMAT)nRet;		
}


////////////////////////////////////////////////////////////////////////////
// Parameter:
// 1, CFile&  : hex file preference, to be read
// 2, char*  : a memory pointer, has been allocate in heep, it's a valid memory space.
// 3, int      :  parameter 2, memory space's size.
// return:
// int			: it's the truely length of the used buffer, also useful byte number after hex file been parsed.
int CHexFileParser::ReadNormalHexFile( CFile& hexFile,  char* pBuf, int nBufLen)
{
	ASSERT(nBufLen >= 0xFFFF);
	//CString m_get_data_from_file;
	int nBufCount = 0;
//*****************inspect the file*********************
	unsigned int nLineNum=0;

	char a[256];
	ZeroMemory(a, 256);
	
	//while(NULL!=ar.ReadString(m_get_data_from_file))//ѭ����ȡ�ļ���ֱ���ļ�����
	while(ReadLineFromFile(hexFile, a))
	{//get a line from the file,check "crc" for total file
	
#ifdef _DEBUG
		if (a[8] != '0')
		{
			int n = 0;
		}
#endif

		nLineNum++;						//the line number that the wrong hex file;
		TS_UC get_hex[128]={0};		//get hex data,it is get from the line char
	
		UINT i = 0;
		for( i=0;i<strlen(a); i++)		//get a line
		{
			a[i] = a[i+1];
		}
		int nLen = strlen(a)-2;			// ��ȥ�س����з���
		if(strlen(a)%2==0)
			turn_hex_file_line_to_unsigned_char(a);		// turn every char to int 
		else
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			AfxMessageBox(p_c_temp);
			//close_com();
			return 0;
		}
		turn_int_to_unsigned_char(a,nLen,get_hex);	//turn to hex 
		if(get_hex[3]==1)	//for to seektobegin() function,because to end of the file
			break;

		TS_UC uctemp2=0;
		for(int j=0;j<(nLen/2);j++)
			uctemp2+=get_hex[j];
		if(uctemp2%256!=0)
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			AfxMessageBox(p_c_temp,MB_OK);
			//close_com();
			return 0;
		}
		if(get_hex[1]==0 && get_hex[2]==0)
			get_hex[4]=255;	//for the 0000 register to write 255
		unsigned int ltemp;
		ltemp=get_hex[1]*256+get_hex[2];

		for(int j=0;j<get_hex[0];j++)
		{
			pBuf[ltemp+j]=get_hex[4+j];//get the data
		}

		if((UINT)nBufCount<(ltemp+get_hex[0]))
		{
			nBufCount=ltemp+get_hex[0]-1;
		}

		ZeroMemory(a, 256);
	}//end while


	hexFile.Close();//close the file		
	
	return nBufCount;
}



BOOL CHexFileParser::DoCRC( TS_UC* szBuf, int nLen)
{
	TS_UC uctemp=0;
	for(int j=0; j < nLen; j++)
		uctemp+=szBuf[j];

	if(uctemp%256!=0)
	{		
		return FALSE;
	}

	return TRUE;
}

WORD CHexFileParser::GetHighAddrFromFile(const CString& strLine)
{	
	WORD dwTemp = 0;
	char ch[128] = {0};
	for (int i = 1; i < strLine.GetLength(); i++)
	{
		ch[i-1] = (char)strLine.GetAt(i);
	}
	turn_hex_file_line_to_unsigned_char(ch);
	TS_UC szBuf[64] = {0};
	turn_int_to_unsigned_char(ch, strLine.GetLength()-1, szBuf);
	dwTemp = szBuf[4]*0x100 + szBuf[5];

	return dwTemp;
}



BOOL CHexFileParser::ReadLineFromFile(CFile& file, char* pBuffer)
{
	char c;
	int nRet = file.Read(&c, 1);

	while(nRet != 0)
	{
		*pBuffer++ = c;

		if (c == 0x0d) // �س�
		{
			file.Read(&c, 1);  // ��һ������
			*pBuffer++ = c;
			return TRUE;
		}
		file.Read(&c, 1);
	}

	return FALSE;
}



////////////////////////////////////////////////////////////////////////////
// Parameter:
// 1, CFile&  : hex file preference, to be read
// 2, char*  : a memory pointer, has been allocate in heep, it's a valid memory space.
// 3, int      :  parameter 2, memory space's size.
// return:
// int			: it's the truely length of the used buffer, also useful byte number after hex file been parsed.
// ����չ��ַ��¼��hex file�����512k��ַ�ռ�
int CHexFileParser::ReadExtendHexFile(CFile& hexFile, char* pBuf, int nBufLen)
{
	//MessageBox(NULL, _T("Sorry, don't suppot Extend Hex File. Please contact software developer."), _T("ISP"), MB_OK);

	ASSERT(nBufLen >= 0xFFFF);
	//CString m_get_data_from_file;
	int nBufCount = 0;
//*****************inspect the file*********************
	unsigned int nLineNum=0;

	char a[256];
	ZeroMemory(a, 256);
	
	//while(NULL!=ar.ReadString(m_get_data_from_file))//ѭ����ȡ�ļ���ֱ���ļ�����
	while(ReadLineFromFile(hexFile, a))
	{//get a line from the file,check "crc" for total file
	
#ifdef _DEBUG
		if (a[8] != '0')
		{
			int n = 0;
		}
#endif

		nLineNum++;						//the line number that the wrong hex file;
		TS_UC get_hex[128]={0};		//get hex data,it is get from the line char
	
		UINT i = 0;
		for( i=0;i<strlen(a); i++)		//get a line
		{
			a[i] = a[i+1];
		}
		int nLen = strlen(a)-2;			// ��ȥ�س����з���
		if(strlen(a)%2==0)
			turn_hex_file_line_to_unsigned_char(a);		// turn every char to int 
		else
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			AfxMessageBox(p_c_temp);
			//close_com();
			return 0;
		}
		turn_int_to_unsigned_char(a,nLen,get_hex);	//turn to hex 
		if(get_hex[3]==1)	//for to seektobegin() function,because to end of the file
			break;

		TS_UC uctemp2=0;
		for(int j=0;j<(nLen/2);j++)
			uctemp2+=get_hex[j];
		if(uctemp2%256!=0)
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			AfxMessageBox(p_c_temp,MB_OK);
			//close_com();
			return 0;
		}
		if(get_hex[1]==0 && get_hex[2]==0)
			get_hex[4]=255;	//for the 0000 register to write 255
		unsigned int ltemp;
		ltemp=get_hex[1]*256+get_hex[2];

		for(int j=0;j<get_hex[0];j++)
		{
			pBuf[ltemp+j]=get_hex[4+j];//get the data
		}

		if((UINT)nBufCount<(ltemp+get_hex[0]))
		{
			nBufCount=ltemp+get_hex[0]-1;
		}

		ZeroMemory(a, 256);
	}//end while


	hexFile.Close();//close the file		
	
	return nBufCount;
}


////////////////////////////////////////////////////////////////////////////
// Parameter:
// 1, CFile&  : hex file preference, to be read
// 2, char*  : a memory pointer, has been allocate in heep, it's a valid memory space.
// 3, int      :  specify parameter 2, memory space's size.
// return:
// int			: it's the truely length of the used buffer, also useful byte number after hex file been parsed.
// ����չ���Ե�ַ��¼��hex file�����32λ�ĵ�ַ�ռ�
BOOL CHexFileParser::ReadExtLinearHexFile(CFile& hexfile, char* pBuf, int nBufLen)
{
	//CArchive ar(&m_hex_file,CArchive::load);
	//ASSERT(nBufLen >= 0xFFFF);
	if (pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}

	pBuf = new char[0x20000];
		
	CString strGetData;
	int nBufCount = 0;
//*****************inspect the file*********************
	unsigned int nLineNumErr=0;

	DWORD dwHiAddr = 0; // ��λ��ַ
	char a[256];
	ZeroMemory(a, 256);

	//while(NULL!=ar.ReadString(strGetData))	//ѭ����ȡ�ļ���ֱ���ļ�����
	while(ReadLineFromFile(hexfile, a))
	{
		// ȡ�ø�λ��ַ�����ܲ�ֹһ����չ
		if( a[8] == '4')
		{
			CString strTemp(a);
			dwHiAddr = GetHighAddrFromFile(strTemp);
			dwHiAddr <<= 16;

			if( nBufCount != 0)
			{
				m_szFlags.push_back(nBufCount);
			}
			// do CRC
			continue;
		}

		//get a line from the file,check "crc" for total file
		nLineNumErr++;	//the line number that the wrong hex file;
		TS_UC get_hex[128]={0};//get hex data,it is get from the line char
		//the number is (i-1)
		//int nLen = strGetData.GetLength();
		for(UINT i=0; i<strlen(a); i++) // ȥ��ð��
		{
			a[i]=a[i+1];
		}
		int nLen = strlen(a)-2; // ����س����еĳ���
		if(strlen(a)%2==0)
			turn_hex_file_line_to_unsigned_char(a);//turn every char to int 
		else
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNumErr);
			
			//AddStringToOutPuts(p_c_temp);
			AfxMessageBox(p_c_temp);
			//close_com();
			goto END;
			return 0;
		}
		turn_int_to_unsigned_char(a,nLen,get_hex);//turn to hex 
		if(get_hex[3]==1)	//for to seektobegin() function,because to end of the file
			break;


		if(!DoCRC( get_hex, nLen/2))
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNumErr);
			//AddStringToOutPuts(p_c_temp);		
			AfxMessageBox(p_c_temp, MB_OK);
			
			//close_com();
			goto END;
			return 0;
		}

	//	if(get_hex[1]==0 && get_hex[2]==0)
	//		get_hex[4]=255;//for the 0000 register to write 255
		unsigned int ltemp;
		ltemp=get_hex[1]*256+get_hex[2] + dwHiAddr;
		for(int j=0;j<get_hex[0];j++)
			pBuf[ltemp+j]=get_hex[4+j];//get the data
		if((UINT)nBufCount<(ltemp+get_hex[0]))
			nBufCount=ltemp+get_hex[0];


		ZeroMemory(a, 256);
	}//end while
	
	m_szFlags.push_back(nBufCount);
	

END:
	hexfile.Close();//close the file

	return nBufCount;
	//return 0 ;
}


HEXFILE_FORMAT	CHexFileParser::GetHexFileFormatType()
{
	return m_nHexFileType;
}

int	CHexFileParser::GetExtendHexFileSectionFlag(vector<int>& szFlags)
{
	ASSERT(m_szFlags.size() > 0);
	szFlags.clear();
	szFlags = m_szFlags;

	return szFlags.size();
}