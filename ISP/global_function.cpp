#include "stdafx.h"
#include "global_function.h"
#include "MyPing.h"
#include "HexFileParser.h"
extern bool auto_flash_mode;
int SPECIAL_BAC_TO_MODBUS = 0;
unsigned int g_mstp_deviceid = 0;

int GetPrivateBacnetToModbusData(uint32_t deviceid, uint16_t start_reg, int16_t readlength, unsigned short *data_out);
int WritePrivateBacnetToModbusData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned short *data_in);
int WritePrivateBacnetToModbusCharData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned char *data_in);
int turn_hex_str_to_ten_num(char *source)
{//can only turn two char(hex string) to int
	//return -1,invalid input ,the char number>2;or the input char is not hex string;
	//int a=strlen(source);
/*	if(a!=2)
	{
		AfxMessageBox("Invalid Data!\nLength is not 2!");
		for(a=0;a<3;a++)
			*source='\0';
		return -1;//invalid data 1
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
					if(!auto_flash_mode)
						AfxMessageBox(_T("Invalid Data!\nin the routine turn_hex_str_to_baseten_function!"), MB_ICONWARNING);
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
	//return -1,invalid data ,the char number>4;or the input char is not hex string;

	int a=strlen(source);
	if(a>4)
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("Invalid Data!\nthe length of the input string is not = 2!"), MB_ICONWARNING);
		for(int i=0;i<a;i++)
			*source='\0';
		return -1;//invaid data 1
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

		//default: AfxMessageBox("invalid data!\nthe input is not a hex string!");return -1;//2
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
BOOL GetSerialComm_ISP(vector<CString>& szComm)
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
CCriticalSection register_critical_section1; 

// This is the original.
int read_one(unsigned char device_var,unsigned short address,int retry_times)
{
	//programmer can control retry times,by parameter retry_times	
	// 	CString str;
	// 	str.Format(_T("ID:%d [Tx=%d : Rx=%d]"), device_var, g_llTxCount++, g_llRxCount);
	// 	SetPaneString(0,str);
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		register_critical_section1.Lock();
		j=Read_One(device_var,address);
		register_critical_section1.Unlock();
	 

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


CString GetFirmwareUpdateName(int ModelID) //TBD: Change this to an array
{
    CString strProductName;
    switch (ModelID)
    {
    case PM_TSTAT5A:
        strProductName = _T("TStat5A");	//
        break;
    case PM_TSTAT5B:
        strProductName = _T("TStat5B");	 //
        break;
    case PM_TSTAT5B2:
        strProductName = _T("TStat5B2");	//
        break;
    case PM_TSTAT5C:
        strProductName = _T("TStat5C");  //
        break;
    case PM_TSTAT10:
        strProductName = _T("PID10");
        break;
    case PM_TSTAT5D:
        strProductName = _T("TStat5D");	 //
        break;
    case PM_TSTAT5E:
        strProductName = _T("TStat5E");  //
        break;
    case PM_PM5E_ARM:
        strProductName = _T("PM5EARM");  //
        break;
    case PM_PM5E:
        strProductName = _T("PM5E");  //
        break;
    case PM_TSTAT5F:
        strProductName = _T("TStat5F");  //
        break;
    case PM_TSTAT5G:
        strProductName = _T("TStat5G");	//
        break;
    case PM_TSTAT5H:
        strProductName = _T("TStat5H"); //
        break;
    case PM_TSTAT6:
        strProductName = _T("TStat6");   //
        break;
    case PM_TSTAT5i:
        strProductName = _T("TStat5I");   //
        break;
    case PM_TSTAT7:
        strProductName = _T("TStat7");   //
        break;
    case PM_TSTAT8:
        strProductName = L"TStat8";
        break;
    case PM_TSTAT8_WIFI:
        strProductName = "TStat8Wifi";
        break;
    case PM_TSTAT8_OCC:
        strProductName = "TStat8Occ";
        break;
    case PM_TSTAT7_ARM:
        strProductName = "TStat7ARM";
        break;
    case PM_TSTAT8_220V:
        strProductName = "TStat8220V";
        break;
    case PM_NC:					//
        strProductName = _T("NC");
        break;
    case PM_CM5:
        strProductName = _T("CM5");
        break;					   //
    case PM_LightingController:
        strProductName = _T("LC");	  //
        break;
    case PM_T38I13O:
        strProductName = _T("T3-8I13O");	  //
        break;
    case PM_T3IOA:
        strProductName = _T("T3-8IOA");	  //
        break;
    case PM_T332AI:
        strProductName = _T("T3-32AI");	  //
        break;
    case  PM_T38AI16O:
        strProductName = _T("T3-8AI160");	  //
        break;
    case PM_ZIGBEE:
        strProductName = _T("ZigBee");	  //
        break;
    case PM_FLEXDRIVER:
        strProductName = _T("FlexDriver");	  //
        break;
    case PM_T3PT10:
        strProductName = _T("T3-PT10");	  //
        break;
    case PM_T3PERFORMANCE:
        strProductName = _T("T3-PERFORMANCE");	  //
        break;
    case PM_T34AO:
        strProductName = _T("T3-4AO");	  //
        break;
    case PM_T36CT:
        strProductName = _T("T3-6CT");	  //
        break;
    case PM_SOLAR:
        strProductName = _T("Solar");	  //
        break;
    case PM_FWMTRANSDUCER:
        strProductName = _T("FWMTRANSDUCER");	  //
        break;
    case PM_MINIPANEL:
    case PM_MINIPANEL_ARM:
        strProductName = _T("MiniPanel");	  //
        break;
    case PM_PRESSURE:
        strProductName = _T("Pressure");	  //
        break;
    case PM_AirQuality:
        strProductName = _T("AirQuality");	  //
        break;
    case PM_HUMTEMPSENSOR:
        strProductName = _T("TstatHUM");	  //
        break;
    case PM_HUM_R:
        strProductName = _T("HUM-R");
        break;
    case PM_TSTATRUNAR:
        strProductName = "TStatRunar";
        break;
    case  PM_CO2_NET:
        strProductName = "CO2 Net";
        break;
    case  PM_CO2_RS485:
        strProductName = "CO2";
        break;
    case  PM_PRESSURE_SENSOR:
        strProductName = "Pressure";
        break;
    case  PM_T3PT12:
        strProductName = "T3PT12";
        break;
    case  PM_T36CTA:
        strProductName = "T36CTA";
        break;
    case  PM_CO2_NODE:
        strProductName = "CO2 Node";
        break;
    case PM_T322AI:
        strProductName = "T322I";
        break;
    case PM_T38AI8AO6DO:
        strProductName = "T38IO";
        break;
    case PM_BTU_METER:
        strProductName = "BTU METER";
        break;
    case PM_T322AIVG:
        strProductName = "T322AIVG";
        break;
    case PM_T38IOVG:
        strProductName = "T38IOVG";
        break;
    case PM_T3PTVG:
        strProductName = "T3PTVG";
        break;
    case STM32_CO2_NET:
        strProductName = "CO2NET";
        break;
    case STM32_CO2_RS485:
        strProductName = "CO2RS485";
        break;
    case STM32_HUM_NET:
        strProductName = "HUMNET";
        break;
    case STM32_HUM_RS485:
        strProductName = "HUMRS485";
        break;
    case PWM_TRANSDUCER:
        strProductName = _T("PWMTRANX");	  //
        break;
    case STM32_PRESSURE_NET:
        strProductName = "PSNET";
        break;
    case STM32_PRESSURE_RS485:
        strProductName = "PSRS485";
        break;
    case STM32_CO2_NODE:
        strProductName = "CO2 NODE";
        break;
    case PM_PWMETER:
        strProductName = "PWMETER";
        break;
    case PM_WEATHER_STATION:
        strProductName = "WS";
        break;
    case STM32_PM25:
        strProductName = "PM2.5";
        break;
    default:
    {
        strProductName.Format(_T("PID%d"), ModelID);
    }
    //strProductName.Format(_T("Model ID:%d is not valid"),ModelID);
    break;
    }
    return strProductName;
}



CString GetProductName(int ModelID) //TBD: Change this to an array
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
    case PM_TSTAT10:
        strProductName = _T("TStat10");
        break;
	case PM_TSTAT5D:
		strProductName=_T("TStat5D");	 //
		break;
	case PM_TSTAT5E:
		strProductName=_T("TStat5E");  //
		break;
    case PM_PM5E_ARM:
        strProductName = _T("PM5EARM");  //
        break;
    case PM_PM5E:
        strProductName=_T("PM5E");  //
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
	case PM_TSTAT5i:
		strProductName=_T("TStat5I");   //
		break;
	case PM_TSTAT7:
		strProductName=_T("TStat7");   //
		break;
	case PM_TSTAT8:
		strProductName=L"TStat8";
		break;
	case PM_TSTAT8_WIFI:
		strProductName = "TStat8Wifi";
		break;
	case PM_TSTAT8_OCC:
		strProductName = "TStat8Occ";
		break;
	case PM_TSTAT7_ARM:
		strProductName = "TStat7ARM";
		break;
	case PM_TSTAT8_220V:
		strProductName = "TStat8220V";
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
	case PM_T38I13O:
		strProductName = _T("T3-8I13O");	  //
		break;
	case PM_T3IOA:
		strProductName = _T("T3-8IOA");	  //
		break;
	case PM_T332AI:
		strProductName = _T("T3-32AI");	  //
		break;
	case  PM_T38AI16O:
		strProductName = _T("T3-8AI160");	  //
		break;
	case PM_ZIGBEE:
		strProductName = _T("ZigBee");	  //
		break;
	case PM_FLEXDRIVER:
		strProductName = _T("FlexDriver");	  //
		break;
	case PM_T3PT10:
		strProductName = _T("T3-PT10");	  //
		break;
	case PM_T3PERFORMANCE:
		strProductName = _T("T3-PERFORMANCE");	  //
		break;
	case PM_T34AO:
		strProductName = _T("T3-4AO");	  //
		break;
	case PM_T36CT:
		strProductName = _T("T3-6CT");	  //
		break;
	case PM_SOLAR:
		strProductName = _T("Solar");	  //
		break;
	case PM_FWMTRANSDUCER:
		strProductName = _T("FWMTRANSDUCER");	  //
		break;
	case PM_MINIPANEL:
    case PM_MINIPANEL_ARM :
		strProductName = _T("MiniPanel");	  //
		break;
	case PM_PRESSURE:
		strProductName = _T("Pressure");	  //
		break;
	case PM_AirQuality:
		strProductName = _T("AirQuality");	  //
		break;
	case PM_HUMTEMPSENSOR:
		strProductName = _T("TstatHUM");	  //
		break;
	case PM_HUM_R:
		strProductName=_T("HUM-R");
		break;
	case PM_TSTATRUNAR:
		strProductName="TStatRunar";
		break;
	case  PM_CO2_NET:
		strProductName = "CO2 Net";
		break;
	case  PM_CO2_RS485:
		strProductName = "CO2";
		break;
    case  PM_PRESSURE_SENSOR:
        strProductName = "Pressure";
        break;
	case  PM_T3PT12:
        strProductName = "T3PT12";
		break;
	case  PM_T36CTA:
		strProductName = "T36CTA";
		break;
	case  PM_CO2_NODE:
		strProductName = "CO2 Node";
		break;
	case PM_T322AI:
		strProductName="T322I";
		break;
	case PM_T38AI8AO6DO:
		strProductName="T38IO";
		break;
	case PM_BTU_METER:
		strProductName="BTU METER";
		break;
	case PM_T322AIVG:
		strProductName="T322AIVG";
		break;
	case PM_T38IOVG:
		strProductName="T38IOVG";
		break;
	case PM_T3PTVG:
		strProductName="T3PTVG";
		break;
	case STM32_CO2_NET:
		strProductName="CO2NET";
		break;
	case STM32_CO2_RS485:
		strProductName="CO2RS485";
		break;
	case STM32_HUM_NET:
		strProductName="HUMNET";
		break;
	case STM32_HUM_RS485:
		strProductName="HUMRS485";
		break;
	case PWM_TRANSDUCER:
		strProductName = _T("PWMTRANX");	  //
		break;
	case STM32_PRESSURE_NET:
		strProductName="PSNET";
		break;
	case STM32_PRESSURE_RS485:
		strProductName="PSRS485";
		break;
	case STM32_CO2_NODE:
		strProductName="CO2 NODE";
		break;
	case PM_PWMETER:
		strProductName = "PWMETER";
		break;
    case PM_WEATHER_STATION:
        strProductName = "WS";
        break;
    case STM32_PM25:
        strProductName = "PM2.5";
        break;
	default:
    {
        strProductName.Format(_T("PID%d"), ModelID);
    }
		//strProductName.Format(_T("Model ID:%d is not valid"),ModelID);
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

	// Start decomposition
	int pos = str.Find(sep);
	while (pos != -1)
	{
		//if(!str.Left(pos).IsEmpty()) // Look for empty string, then discard
		arr.Add(str.Left(pos));

		str = str.Mid(pos + sep.GetLength());
		pos = str.Find(sep);
	}

	arr.Add(str); // 
}

BOOL DoHEXCRC( TS_UC* szBuf, int nLen)
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

BOOL ReadLineFromHexFile(CFile& file, char* pBuffer)
{
	//If the row has more than 256 chars there is a problem with the hex file. 
	int linecharnum=0;
	char c;
	int nRet = file.Read(&c, 1);

	while(nRet != 0)
	{
		++linecharnum;
		*pBuffer++ = c;
		//TRACE(_T("\n%c"),c);
		if (c == 0x0d) // Enter
		{
			file.Read(&c, 1);  // Read a newline
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

int Get_HexFile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info,int Address)
{


char*			pFileBuffer;
CHexFileParser* pHexFile = new CHexFileParser;
pHexFile->SetFileName(filepath);
pFileBuffer = new char[c_nHexFileBufLen];
memset(pFileBuffer, 0xFF, c_nHexFileBufLen);
// Get file buffer
int nDataSize = pHexFile->GetHexFileBuffer(pFileBuffer, c_nHexFileBufLen);//��ȡ�ļ���buffer - Get file buffer

memcpy(&ret_bin_Info,&pFileBuffer[Address],sizeof(Bin_Info));

if (pFileBuffer)
{
	delete []pFileBuffer;
	pFileBuffer = NULL;
}
delete pFileBuffer;



if(strlen(ret_bin_Info.product_name) > 200)
	return NO_VERSION_INFO;
char temocolog[6];
memcpy_s(temocolog,5,ret_bin_Info.company,5);
temocolog[5] = 0;

CString Temco_logo;
MultiByteToWideChar( CP_ACP, 0, (char *)temocolog, 
	(int)strlen(temocolog)+1, 
	Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
Temco_logo.ReleaseBuffer();		
Temco_logo.MakeUpper();
if(Temco_logo.CompareNoCase(_T("TEMCO")) != 0)
{
	return NO_VERSION_INFO;
}


return READ_SUCCESS;

}

int Get_Binfile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info)
{
	CFileFind fFind;
	if(!fFind.FindFile(filepath))
		return FILE_NOT_FIND;


	CFile binFile;
	if(binFile.Open(filepath,CFile::modeRead))
	{
		const int BUF_LEN = 1024;
		int linenum=1;
		TS_UC pBuf[BUF_LEN] = {'\0'};
		int nRet = 0;
		int nCount = 0;
		binFile.Seek(0, CFile::begin);
		while( (nRet = binFile.Read(pBuf, BUF_LEN)) != 0 )
		{
			if(nRet<(0x100 + sizeof(Bin_Info)))
				return BIN_FILE_LENGTH_ERROR;
			memcpy(&ret_bin_Info,&pBuf[0x100],sizeof(Bin_Info));
			if(strlen(ret_bin_Info.product_name) > 200)
				return NO_VERSION_INFO;
			char temocolog[6];
			memcpy_s(temocolog,5,ret_bin_Info.company,5);
			temocolog[5] = 0;

			CString Temco_logo;
			MultiByteToWideChar( CP_ACP, 0, (char *)temocolog, 
				(int)strlen(temocolog)+1, 
				Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
			Temco_logo.ReleaseBuffer();		
			Temco_logo.MakeUpper();
			if(Temco_logo.CompareNoCase(_T("TEMCO")) != 0)
			{
				return NO_VERSION_INFO;
			}


			return READ_SUCCESS;
		}
	}
	return OPEN_FILE_ERROR;

}

BOOL Ping(const CString& strIP, CWnd* pWndEcho)
{
	CMyPing* pPing = new CMyPing;
	pPing->SetPingEchoWnd(pWndEcho);
	pPing->TestPing(strIP);
	delete pPing;
	pPing = NULL;
	return FALSE;
}

int read_multi_retry(unsigned char device_var, unsigned short *put_data_into_here, unsigned short start_address, int length , int retry_time)
{
    int nret = 0;
    for (int i = 0; i < retry_time; i++)
    {
        nret = read_multi_tap(device_var, put_data_into_here, start_address, length);
        if (nret < 0)
        {
            Sleep(300);
            continue;
        }
        else
            return nret;
    }
    return nret;
   
}

int Write_One_Retry(unsigned char device_var, unsigned short address, unsigned short value,int retry_time)
{
    int nret = 0;
    for (int i = 0; i < retry_time; i++)
    {
        nret = Write_One(device_var, address, value);
        if (nret < 0)
        {
            Sleep(300);
            continue;
        }
        else
            return nret;
    }
    return nret;
    
}

bool Open_Socket_Retry(CString strIPAdress, short nPort, int retry_time)
{
    for (int i = 0; i < retry_time; i++)
    {
        bool nreslts = false;
        nreslts = Open_Socket2(strIPAdress, nPort);
        if (nreslts)
            return true;
    }
    return false;
}

//Split the cstring to each of its components.
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken)
{
    CString sTempSource, sTempSplitted;

    sTempSource = sSource;

    int nPos = sTempSource.Find(sToken);

    //--if there are no tokens in the string then add itself and return.
    if (nPos == -1)
        saArray.Add(sTempSource);
    else
    {
        while (sTempSource.GetLength() > 0)
        {
            nPos = sTempSource.Find(sToken);
            if (nPos == -1)
            {
                saArray.Add(sTempSource.Trim());
                break;
            }
            else if (nPos == 0)
            {
                sTempSource = sTempSource.Mid(sToken.GetLength(), sTempSource.GetLength());
                continue;
            }
            else
            {
                sTempSplitted = sTempSource.Mid(0, nPos);
                saArray.Add(sTempSplitted.Trim());
                sTempSource = sTempSource.Mid(nPos + sToken.GetLength(), sTempSource.GetLength());
            }
        }
    }

}

// 0 normal mode, 1 flash boot mode
extern int com_port_flash_status;  // 0 ����ģʽ   1 ��дbootģʽ
// 0 no need to update boot, 1 need to update bootloader; C1 is hex
extern int firmware_must_use_new_bootloader;  //0 ���ø���boot   1 ��Ҫ����bootload;   C1Ϊhex
int check_bootloader_and_frimware(int npid ,int comport , unsigned short reg_11 , unsigned short reg_14, int &update_value, unsigned char app_already_version)
{
    unsigned short Device_infor[18] = {0};
    Device_infor[7] = npid;
    Device_infor[11] = reg_11;
    Device_infor[14] = reg_14;
	int c2_update_boot = false;
	int device_version = 0;
	int Ret_Result = 1;
	if ((((npid >= STM32_CO2_NET) && (npid <= STM32_PRESSURE_RS485)) ||
		(npid == STM32_PM25)) &&
		app_already_version >= 59) //���ܶ���û���� boot version ������ �ٴθ���boot - Regardless of whether the boot version is read or not, it does not need to be updated again
	{
		c2_update_boot = false;
		return Ret_Result;
	}


    if (com_port_flash_status == 0)
    {
        if ((Device_infor[7] == PM_TSTAT8) ||
            (Device_infor[7] == PM_TSTAT9) ||
            (Device_infor[7] == PM_MINIPANEL_ARM) ||
            (Device_infor[7] == PM_MINIPANEL) ||
            (Device_infor[7] == PM_TSTAT10) ||
			(Device_infor[7] == STM32_PM25) ||
			(Device_infor[7] == STM32_CO2_NET) ||
			(Device_infor[7] == STM32_CO2_RS485) ||
			(Device_infor[7] == STM32_HUM_NET) ||
			(Device_infor[7] == STM32_HUM_RS485) ||
			(Device_infor[7] == STM32_PRESSURE_NET) ||
			(Device_infor[7] == STM32_PRESSURE_RS485))
        {



            if (firmware_must_use_new_bootloader == 0)
                c2_update_boot = false;
            else
            {
                int temp_bootloader_version = 0;
                temp_bootloader_version = isp_max(Device_infor[11], Device_infor[14]);
                if ((Device_infor[7] == PM_TSTAT8) && ((temp_bootloader_version <= 48) && (temp_bootloader_version != 0)))
                {
                    c2_update_boot = true;
                }
                else if ((Device_infor[7] == PM_MINIPANEL_ARM) && (temp_bootloader_version < 62))
                {
                    if (comport == 0)
                    {
                        c2_update_boot = false; //��֧�ִ��ڸ��� - Do not support serial port update
                        Ret_Result = -1;
                    }
                    else
                        c2_update_boot = true;

                }
                else if ((Device_infor[7] == PM_MINIPANEL) && (temp_bootloader_version < 62))
                {
                    if (comport == 0)
                    {
                        c2_update_boot = false; //��֧�ִ��ڸ��� - Do not support serial port update
                        Ret_Result = -1;
                    }
                    else
                        c2_update_boot = true;
                }
                else if ((Device_infor[7] == PM_TSTAT10) && (temp_bootloader_version < 54))
                {
                    c2_update_boot = true;
                }
				else if ((((Device_infor[7] >= STM32_CO2_NET) && 
					      (Device_infor[7] <= STM32_PRESSURE_RS485)) || (Device_infor[7] == STM32_PM25))
					&& (temp_bootloader_version < 56))
				{
					c2_update_boot = true;
				}
                else
                    c2_update_boot = false;
            }
            update_value = c2_update_boot;
            //if (c2_update_boot == 1)
            //{
            //    CString strtips;
            //    strtips.Format(_T("New bootloader available ,need update!"), resend_count);
            //    OutPutsStatusInfo(strtips, false);
            //    PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_RESTATR_BOOT, Device_infor[7], 0);
            //    Ret_Result = 2;
            //}
        }
    }
    return Ret_Result;
}


/**

A wrapper for modbus_read_one_value which returns BOTH read value and error flag

@param[in]   device_var	the modbus device address
@param[in]   address		the offset of the value to be read in the device
@param[in]   retry_times	the number of times to retry on read failure before giving up

@return -1, -2, -3 on error, otherwise value read cast to integer

This interface is provided for compatibility with existing code.
New code should use modbus_read_one_value() directly,
since it returns a separate error flag and read value -
allowing simpler, more easily understood calling code design.
CString* pstrInfo = new CString(strInfo);
*/


int mudbus_read_one(unsigned char device_var, unsigned short address, int retry_times)
{
    if (SPECIAL_BAC_TO_MODBUS)
    {
        unsigned short ret_value = 0;
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {
            n_ret = GetPrivateBacnetToModbusData(g_mstp_deviceid, address, 1, &ret_value);
            if (n_ret >= 0)
            {
                return ret_value;
            }
            Sleep(1000);
        }
        return -2;
    }
    int value = 0;

    for (int i = 0; i < 3; i++)
    {
        value = Read_One(device_var, address);
        if (value < 0)
        {
            // there was an error, so return the error flag
            Sleep(200);
            continue;
        }
        else
        {
            // no error, so return value read
            return value;
        }
    }
}



int mudbus_write_one(unsigned char device_var, unsigned short address, short value, int retry_times)
{
    //2018 0606 �ڵײ㹫����д�������ӶԲ�ͬЭ��Ĵ��� - Add support for different protocols in the underlying read/write functions
    if (SPECIAL_BAC_TO_MODBUS)
    {
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {

            n_ret = WritePrivateBacnetToModbusData(g_mstp_deviceid, address, 1, (unsigned short *)(&value));
            if (n_ret >= 0)
            {

                return n_ret;
            }
            Sleep(1000);
        }
        return n_ret;
    }
    int  j = 0;
    for (int i = 0; i < retry_times; i++)
    {
        j = Write_One(device_var, address, value);
        if (j < 0)
        {
            Sleep(300);
            continue;
        }
        else
        {
            break;
        }
    }
    return j;
}



int modbus_read_multi(unsigned char device_var, unsigned short *put_data_into_here, unsigned short start_address, int length, int retry_times)
{
    CString data;
    CString g_strT3000LogString;
    //if ((g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_MSTP_TO_MODBUS) || (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
    if (SPECIAL_BAC_TO_MODBUS)
    {
		retry_times = 10;
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {
            n_ret = GetPrivateBacnetToModbusData(g_mstp_deviceid, start_address, length, put_data_into_here);
            if (n_ret >= 0)
            {
                Sleep(100);
                return n_ret;
            }
            Sleep(100);

        }
        return n_ret;
    }

    int ret = 0;
    for (int i = 0; i < retry_times; i++)
    {
        ret = read_multi_tap(
            
            device_var,
            put_data_into_here,
            start_address,
            length);
        if (ret < 0)
        {
            Sleep(200);
            continue;
        }
        else
        {
            break;
        }
    }


    return ret;
}





int mudbus_write_single_short(unsigned char device_var, unsigned char *to_write, unsigned short start_address, int length, int retry_times)
{
    //2018 0525 �ڵײ㹫����д�������ӶԲ�ͬЭ��Ĵ��� - Add support for different protocols in the underlying read/write functions
    //if ((g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_MSTP_TO_MODBUS) || (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
    if (SPECIAL_BAC_TO_MODBUS)
    {
		retry_times = 10;
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {
            n_ret = WritePrivateBacnetToModbusCharData(g_mstp_deviceid, start_address, length, to_write);
            if (n_ret >= 0)
            {
                return n_ret;
            }
            Sleep(1000);
        }
        return n_ret;
    }

    int j = 0;
    for (int i = 0; i<retry_times; i++)
    {

        j = write_multi(device_var, to_write, start_address, length);

        if (j < 0)
        {
            Sleep(300);
            return j;
        }
        else
        {
            break;
        }

    }
    return j;
}



int mudbus_write_multi_short(unsigned char device_var, unsigned short *to_write, unsigned short start_address, int length, int retry_times)
{
    //2018 0525 �ڵײ㹫����д�������ӶԲ�ͬЭ��Ĵ��� - Add support for different protocols in the underlying read/write functions
    //if ((g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_MSTP_TO_MODBUS) || (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
    if (SPECIAL_BAC_TO_MODBUS)
    {
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {
            n_ret = WritePrivateBacnetToModbusData(g_mstp_deviceid, start_address, length, to_write);
            if (n_ret >= 0)
            {
                return n_ret;
            }
            Sleep(1000);
        }
        return n_ret;
    }

    int j = 0;
    for (int i = 0; i<retry_times; i++)
    {

        j = write_multi_Short(device_var, to_write, start_address, length);

        if (j < 0 )
        {
            Sleep(500);
            return j;
        }
        else
        {
            break;
        }
    }
    return j;
}
