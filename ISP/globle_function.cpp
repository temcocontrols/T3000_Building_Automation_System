#include "stdafx.h"
#include "globle_function.h"
int turn_hex_str_to_ten_num(char *source)
{//can only turn two char(hex string) to int
	//return -1,the wrong input ,the char number>2;or the input char is not hex string;
	//int a=strlen(source);
/*	if(a!=2)
	{
		AfxMessageBox("wrong input!\nthat the length of the input string !=2!");
		for(a=0;a<3;a++)
			*source='\0';
		return -1;//input wrong 1
	}*/
	int j=0,k=0,l=0;
	for(int i=0;i<2;i++)//***********************************2
		if(j==0)
		{
			switch(source[i])
			{
			case '0':k=0;break;
			case '1':k=1;break;
			case '2':k=2;break;
			case '3':k=3;break;
			case '4':k=4;break;
			case '5':k=5;break;
			case '6':k=6;break;
			case '7':k=7;break;
			case '8':k=8;break;
			case '9':k=9;break;

			case 'a':k=10;break;
			case 'b':k=11;break;
			case 'c':k=12;break;
			case 'd':k=13;break;
			case 'e':k=14;break;
			case 'f':k=15;break;
			case 'A':k=10;break;
			case 'B':k=11;break;
			case 'C':k=12;break;
			case 'D':k=13;break;
			case 'E':k=14;break;
			case 'F':k=15;break;

			default: 
				{
					AfxMessageBox(_T("wrong input!\nin the turn_hex_str_to_ten_num function!"), MB_ICONWARNING);
					return -1;//2
				}
			}
			for( ;j<2-i-1;j++)
				k*=16;
		}
		else
		{
			l+=k;
			j=0;
			i--;
		}
		l+=k;
	return l;
}
unsigned short turn_4_hex_char_to_unsigned_short(char *source)
{//can only turn four char(hex string) to int
	//return -1,the wrong input ,the char number>4;or the input char is not hex string;

	int a=strlen(source);
	if(a>4)
	{
		AfxMessageBox(_T("wrong input!\nthat the length of the input string !=2!"), MB_ICONWARNING);
		for(int i=0;i<a;i++)
			*source='\0';
		return -1;//input wrong 1
	}
	unsigned short k=0,l=0;
	for(int j=0;j<a;j++)
	{
		switch(source[j])
		{
		case '0':k=0;break;
		case '1':k=1;break;
		case '2':k=2;break;
		case '3':k=3;break;
		case '4':k=4;break;
		case '5':k=5;break;
		case '6':k=6;break;
		case '7':k=7;break;
		case '8':k=8;break;
		case '9':k=9;break;

		case 'a':k=10;break;
		case 'b':k=11;break;
		case 'c':k=12;break;
		case 'd':k=13;break;
		case 'e':k=14;break;
		case 'f':k=15;break;
		case 'A':k=10;break;
		case 'B':k=11;break;
		case 'C':k=12;break;
		case 'D':k=13;break;
		case 'E':k=14;break;
		case 'F':k=15;break;

		//default: AfxMessageBox("wrong input!\nthat input is not a hex string!");return -1;//2
		default: return -1;//2
		}
		l*=16;
		l+=k;
	}
	return l;
}
int turn_hex_char_to_int(char c)
{
		int k=0;
		switch(c)
		{
		case '0':k=0;break;
		case '1':k=1;break;
		case '2':k=2;break;
		case '3':k=3;break;
		case '4':k=4;break;
		case '5':k=5;break;
		case '6':k=6;break;
		case '7':k=7;break;
		case '8':k=8;break;
		case '9':k=9;break;

		case 'a':k=10;break;
		case 'b':k=11;break;
		case 'c':k=12;break;
		case 'd':k=13;break;
		case 'e':k=14;break;
		case 'f':k=15;break;
		case 'A':k=10;break;
		case 'B':k=11;break;
		case 'C':k=12;break;
		case 'D':k=13;break;
		case 'E':k=14;break;
		case 'F':k=15;break;

		default: return -1;//2
		}
		return k;
}
bool turn_hex_file_line_to_unsigned_char(char *str)
{
	char *p_temp=str;
	int itemp=strlen(p_temp);
	for(int i=0;i<itemp;i++)
	{
		*(p_temp+i)=turn_hex_char_to_int(*(p_temp+i));	
		if(*(p_temp+i)==-1)
			return false;
	}
	return true;
}
void turn_int_to_unsigned_char(char *source,int length_source,TS_UC *aim)
{
	char *p_c_temp=source;
	TS_UC *p_uc_temp=aim;
	TS_UC uctemp;
	for(int i=0;i <length_source;i++)
		if(i%2==0)
		{
			char ctemp=*(p_c_temp+i);
			uctemp = ctemp*16;
		}
		else
		{
			char ctemp=*(p_c_temp+i);
			uctemp+=ctemp;
			*(p_uc_temp+i/2)=uctemp;
			uctemp=0;
		}
}
void turn_unsigned_short_to_hex_char(char *order,unsigned short source)
{//return value ,1 is success ,0 is failure
	//the number of the order ,should >=5,is good
	//the source must >0 and <65535
	int itemp=0;
	unsigned short ustemp=0;
	do{
		ustemp=source%16;
		order[itemp]=turn_unsigned_to_char(ustemp);
		source/=16;
		itemp++;
	}while(source!=0);
	order[itemp]='\0';	
	if(strlen(order)%2)
	{
		order[itemp]='0';
		order[itemp+1]='\0';
	}
	//order=strrev(order);
	order=_strrev(order);
}
char turn_unsigned_to_char(unsigned short source)
{//return value ,is the char 
	//0 is failure
	if(source>15)
		return 0;
	if(source>=0 && source<=9)
		return source+48;
	else
		if(source>=10 && source<=15)
			return source+55;

	return 0;
}
BOOL GetSerialComm(vector<CString>& szComm)
{
	LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	HKEY   hKey;   
	LONG   lReturnCode=0;   
	lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);   

	if(lReturnCode==ERROR_SUCCESS)   
	{  
		DWORD dwIndex = 0;
		WCHAR lpValueName[MAX_PATH];
		ZeroMemory(lpValueName, MAX_PATH);
		DWORD lpcchValueName = MAX_PATH; 
		DWORD lpReserved = 0; 
		DWORD lpType = REG_SZ; 
		BYTE		lpData[MAX_PATH]; 
		ZeroMemory(lpData, MAX_PATH);
		DWORD lpcbData = MAX_PATH;
		dwIndex = 0;
		while(RegEnumValue(	hKey, dwIndex, lpValueName, &lpcchValueName, 0, &lpType, lpData, &lpcbData ) != ERROR_NO_MORE_ITEMS)
		{   
			dwIndex++;

			lpcchValueName = MAX_PATH; 
			//lpValueName[0] = '\0'; 

			CString strValueName= CString(lpValueName);

			WCHAR ch[MAX_PATH];
			ZeroMemory(ch, MAX_PATH);
			memcpy(ch, lpData, lpcbData);
			CString str = CString(ch);
			szComm.push_back(str);

			ZeroMemory(lpData, MAX_PATH);
			lpcbData = MAX_PATH;
		}   
		::RegCloseKey(hKey);   		   

		return TRUE;
	}

	return FALSE;   
}
CString GetExePath(bool bHasSlash)
{
	TCHAR	szBuf[MAX_PATH];
	GetModuleFileName(NULL,szBuf,MAX_PATH);

	CString	strPath(szBuf);
	int idx = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(idx);

	if(bHasSlash){ // has '\' at last.
		if(strPath.Right(1)!=_T('\\'))
			strPath+=_T('\\');
		return strPath;
	}else{ // don't need '\'.
		if(strPath.Right(1)==_T('\\'))
			strPath.TrimRight(_T('\\'));
		return strPath;
	}
}

BOOL HexFileValidation(const CString& strFileName)
{	
	const CString strConst = _T("hex");
	CString strSuffix = strFileName.Right(3);
	if (strSuffix.CompareNoCase(strConst) != 0)
	{
		return FALSE;
	}

	return TRUE;
}
	
BOOL BinFileValidation(const CString& strFileName)	
{
	const CString strConst = _T("bin");
	CString strSuffix = strFileName.Right(3);
	if (strSuffix.CompareNoCase(strConst) != 0 )
	{
		return FALSE;
	}
	return TRUE;
}
CCriticalSection register_critical_section; 

// 这个是原始的
int read_one(unsigned char device_var,unsigned short address,int retry_times)
{
	//programmer can control retry times,by parameter retry_times	
	// 	CString str;
	// 	str.Format(_T("ID:%d [Tx=%d : Rx=%d]"), device_var, g_llTxCount++, g_llRxCount);
	// 	SetPaneString(0,str);
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		register_critical_section.Lock();
		j=Read_One(device_var,address);
		register_critical_section.Unlock();
	 

		if(j!=-2 && j!=-3)
		{
			CString str;

		 
		 
			if(address==101 && j>32767)
			{//the temperature is below zero;;;;;-23.3
				j=0-65535+j;
			}
			return j;//return reght ,success
		}	 
	}
	/*CString str;*/
	//str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount,g_llTxCount-g_llRxCount );
	//SetPaneString(0,str);
	return j;
}
CString GetProductName(int ModelID)
{
	CString strProductName;
	switch(ModelID)
	{
	case PM_TSTAT5A:
		strProductName=_T("TStat5A");	//
		break;
	case PM_TSTAT5B:
		strProductName=_T("TStat5B");	 //
		break;
	case PM_TSTAT5B2:
		strProductName=_T("TStat5B2");	//
		break;
	case PM_TSTAT5C:
		strProductName=_T("TStat5C");  //
		break;
	case PM_TSTAT5D:
		strProductName=_T("TStat5D");	 //
		break;
	case PM_TSTAT5E:
		strProductName=_T("TStat5E");  //
		break;
	case PM_TSTAT5F:
		strProductName=_T("TStat5F");  //
		break;
	case PM_TSTAT5G:
		strProductName=_T("TStat5G");	//
		break;
	case PM_TSTAT5H:
		strProductName=_T("TStat5H"); //
		break;
	case PM_TSTAT6:
		strProductName=_T("TStat6");   //
		break;
	case PM_TSTAT7:
		strProductName=_T("TStat7");   //
		break;
	case PM_NC:					//
		strProductName=_T("NC");
		break;
	case PM_CM5:
		strProductName =_T("CM5");
		break;					   //
	case PM_LightingController:
		strProductName = _T("LC");	  //
		break;
	case PM_T38AIOD:
		strProductName = _T("T38AIOD");	  //
		break;
	case PM_T3IOA:
		strProductName = _T("T3IOA");	  //
		break;
	case PM_T332AI:
		strProductName = _T("T332AI");	  //
		break;
	case PM_T3AI16O:
		strProductName = _T("T3AI160");	  //
		break;
	case PM_ZIGBEE:
		strProductName = _T("ZigBee");	  //
		break;
	case PM_FLEXDRIVER:
		strProductName = _T("FlexDriver");	  //
		break;
	case PM_T38I13O:
		strProductName = _T("T38I13O");	  //
		break;
	case PM_T3PERFORMANCE:
		strProductName = _T("T3PERFORMANCE");	  //
		break;
	case PM_T34AO:
		strProductName = _T("T34AO");	  //
		break;
	case PM_T36CT:
		strProductName = _T("T36CT");	  //
		break;
	case PM_SOLAR:
		strProductName = _T("Solar");	  //
		break;
	case PM_FWMTRANSDUCER:
		strProductName = _T("FWMTRANSDUCER");	  //
		break;
	case PM_MINIPANEL:
		strProductName = _T("MiniPanel");	  //
		break;
	case PM_PRESSURE:
		strProductName = _T("Pressure");	  //
		break;
	case PM_AirQuality:
		strProductName = _T("AirQuality");	  //
		break;
	default:
		strProductName=_T("ERROR");
		break;
	}
	return strProductName;
}

CString GetSysTime()
{
	CTime   tm   =   CTime::GetCurrentTime(); 
	int year = tm.GetYear();
	int mouth = tm.GetMonth();
	int day = tm.GetDay();
	int hour = tm.GetHour();
	int min = tm.GetMinute();
	int sec = tm.GetSecond();
	CString systime =_T("");
	systime.Format(_T("%d-%d-%d %d:%d:%d"),year,mouth,day,hour,min,sec);
	return systime;
}

void ExtractString(CStringArray& arr, const CString strSrc, const CString sep)
{
	CString str(strSrc);
	str.TrimLeft(); 
	str.TrimRight();

	if(str.IsEmpty())
		return;

	// 开始分解
	int pos = str.Find(sep);
	while (pos != -1)
	{
		//if(!str.Left(pos).IsEmpty()) // 如有必要也可在此Trim后再判断是否为空，为空则舍弃
		arr.Add(str.Left(pos));

		str = str.Mid(pos + sep.GetLength());
		pos = str.Find(sep);
	}

	arr.Add(str); // think
}