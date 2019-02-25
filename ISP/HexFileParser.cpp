#include "StdAfx.h"
#include "HexFileParser.h"
#include "global_function.h"

extern unsigned int n_check_temco_firmware;
extern bool auto_flash_mode;
 extern Bin_Info        global_fileInfor;
CHexFileParser::CHexFileParser(void)
{
    m_nHexFileType = HEXFILE_DATA;
    m_IsRAM=FALSE;
}

CHexFileParser::~CHexFileParser(void)
{

}


void  CHexFileParser::SetFileName(const CString& strFileName)
{
	m_strFileName = strFileName;
}


//////////////////////////////////////////////////////////////////////////
// 返回int：实际的有效字节
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
			nBufLen = ReadNormalHexFile(hexFile, pBuf, nLen);	

		}
		else if(m_nHexFileType == 1)
		{
			CString strTips = _T("The hex file has a Extend Section Address Record(HEX86).");
			nBufLen = ReadExtendHexFile(hexFile, pBuf, nLen);	
		}
		else if (m_nHexFileType == 2)
		{
			CString strTips = _T("The hex file has a Extend Linear Addree Record(HEX86).");
			nBufLen = ReadExtLinearHexFile(hexFile, pBuf, nLen);	
		}		
	}

	if (m_IsRAM)
	{
		memcpy(&global_fileInfor,&pBuf[0x8200],sizeof(Bin_Info));
	} 
	else
	{
		memcpy(&global_fileInfor,&pBuf[0x100],sizeof(Bin_Info));
	}
	if(strlen(global_fileInfor.product_name) > 200)
	nBufLen = -1;
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
    if (n_check_temco_firmware == 0)
        return nBufLen;
	if(Temco_logo.CompareNoCase(_T("TEMCO")) != 0&&Temco_logo.Find(L"CO2")==-1)
	{
		//return NO_VERSION_INFO;
		nBufLen = -1;
	}


	//return READ_SUCCESS;

	return nBufLen;

}

BOOL CHexFileParser::Is_RAM_HEXType(){
    return m_IsRAM;
}

// 读第7，8个字符，00为Normal，02为扩展，04为扩展线性
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
	
	//while(NULL!=ar.ReadString(m_get_data_from_file))//循环读取文件，直到文件结束
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
		for( i=0;i<strlen(a); i++)		//get a line//去掉第一个
		{
			a[i] = a[i+1];
		}
		int nLen = strlen(a)-2;			// 减去回车换行符号
		if(strlen(a)%2==0)
			turn_hex_file_line_to_unsigned_char(a);		// turn every char to int 
		else
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			nBufCount = 0;
			if(!auto_flash_mode)
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
			nBufCount = 0;
			if(!auto_flash_mode)
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
	//dwTemp = szBuf[4]*0x100 + szBuf[5];  杜帆屏蔽
    dwTemp = szBuf[4] * 256 + szBuf[5];
    if (dwTemp>=800)
    {
        dwTemp-=0x800;
        m_IsRAM=TRUE;
    }
	return dwTemp;
}


BOOL CHexFileParser::ReadLineFromFile(CFile& file, char* pBuffer)
{
	//当hex文件中每一行的文件超过了256个字符的时候，我们就认为这个hex文件出现了问题
	int linecharnum=0;
	char c;
	int nRet = file.Read(&c, 1);

	while(nRet != 0)
	{
		++linecharnum;
		*pBuffer++ = c;
		//TRACE(_T("\n%c"),c);
		if (c == 0x0d) // 回车
		{
			file.Read(&c, 1);  // 读一个换行
			*pBuffer++ = c;
			TRACE(_T("%s"),pBuffer);
			return TRUE;
		}
		 if (linecharnum<256)
		 {
			 file.Read(&c, 1);
		 }
		 else
		 {
			 if(!auto_flash_mode)
				 AfxMessageBox(_T("The Hex File is broken"));
			 return FALSE;
		 }
		
	}
	//TRACE(_T("%s"),pBuffer);
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////
// Parameter:
// 1, CFile&  : hex file preference, to be read
// 2, char*  : a memory pointer, has been allocate in heep, it's a valid memory space.
// 3, int      :  parameter 2, memory space's size.
// return:
// int			: it's the truely length of the used buffer, also useful byte number after hex file been parsed.
// 读扩展地址记录的hex file－最大512k地址空间
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
	
	//while(NULL!=ar.ReadString(m_get_data_from_file))//循环读取文件，直到文件结束
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
		int nLen = strlen(a)-2;			// 减去回车换行符号
		if(strlen(a)%2==0)
			turn_hex_file_line_to_unsigned_char(a);		// turn every char to int 
		else
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			nBufCount = 0;
			if(!auto_flash_mode)
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
			nBufCount = 0;
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNum);
			if(!auto_flash_mode)
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
// 读扩展线性地址记录的hex file－最大32位的地址空间
BOOL CHexFileParser::ReadExtLinearHexFile(CFile& hexfile, char* pBuf, int nBufLen)
{
	//CArchive ar(&m_hex_file,CArchive::load);
	//ASSERT(nBufLen >= 0xFFFF);
	/*if (pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}

	pBuf = new char[c_nHexFileBufLen];*/
		
	CString strGetData;
	int nBufCount = 0;
//*****************inspect the file*********************
	unsigned int nLineNumErr=0;

	DWORD dwHiAddr = 0; // 高位地址
	char a[256];
	ZeroMemory(a, 256);
	
	//while(NULL!=ar.ReadString(strGetData))	//循环读取文件，直到文件结束
	while(ReadLineFromFile(hexfile, a))
	{

		// 取得高位地址，可能不止一处扩展
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
        else if ((a[8] == '5'))
        {
            continue;
        }


		//get a line from the file,check "crc" for total file
		nLineNumErr++;	//the line number that the wrong hex file;
		TS_UC get_hex[128]={0};//get hex data,it is get from the line char
		//the number is (i-1)
		//int nLen = strGetData.GetLength();
		for(UINT i=0; i<strlen(a); i++) // 去掉冒号
		{
			a[i]=a[i+1];
		}

		int nLen = strlen(a)-2; // 不算回车换行的长度
		if(strlen(a)%2==0)
			turn_hex_file_line_to_unsigned_char(a);//turn every char to int 
		else
		{
			wchar_t p_c_temp[74]={'\0'};
			swprintf_s(p_c_temp,_T("Error: the hex file had error at %d line!"),nLineNumErr);
			
			//AddStringToOutPuts(p_c_temp);
			nBufCount = 0;
			if(!auto_flash_mode)
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
			nBufCount = 0;	
			if(!auto_flash_mode)
				AfxMessageBox(p_c_temp, MB_OK);
			
			//close_com();
			goto END;
			return 0;
		}

	//	if(get_hex[1]==0 && get_hex[2]==0)
	//		get_hex[4]=255;//for the 0000 register to write 255
	    int temp;
		char temp_buf[32];
		
		if (nLineNumErr==9)
		{

		    for (int i=0;i<32;i++)
		    {
			  temp_buf[i]=a[i+8];
		    }
			for (int i=0;i<20;i++)
			{   
			    temp=temp_buf[2*i]*16+temp_buf[2*i+1];
				m_DeviceInfor[i]=temp;
			}
			
			//m_softwareRev = (temp_buf[30] + temp_buf[31]*256)/10.0;


			//m_ProductModel=m_DeviceInfor[0];
			m_softwareRev= ((a[38]*16+a[39]) + (a[40]*16 + a[41])*256)/10.0;
			m_ProductName.Empty();
			for (int i=0;i<10;i++)
			{
				CString temp1;
				temp1.Format(_T("%C"),m_DeviceInfor[5+i]);
				m_ProductName = m_ProductName + temp1;
			}
		}



		unsigned int ltemp;
		ltemp=get_hex[1]*256+get_hex[2] + dwHiAddr;
//         if (m_IsRAM)
//         {
//             ltemp-=0x00008000; 
//         }
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

CString CHexFileParser::Get_HexInfor(){
 
 return m_ProductName+_T(" ")+m_ChipName;
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
void CHexFileParser::Get_DeviceInfor(UINT &ProductModel,float &SoftwareVersion,CString &ProductName,CString &ChipName,UINT &Chipsize){
ProductModel=m_ProductModel;
SoftwareVersion=m_softwareRev;
ProductName=m_ProductName;
ChipName=m_ChipName;
Chipsize=m_ChipSize;

}