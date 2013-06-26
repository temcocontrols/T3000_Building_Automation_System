
#include "stdafx.h"
#include "globle_function.h"
#include "Windows.h"
#include "T3000.h"



#include "T3000RegAddress.h"


#define DELAY_TIME	 10	//MS
#define Modbus_Serial	0
#define	Modbus_TCP	1

#include "modbus_read_write.cpp"

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

*/

int read_one(unsigned char device_var,unsigned short address,int retry_times)
{
	int value;
	int j = modbus_read_one_value( value, device_var, address, retry_times );
	if( j != 0 ) {
		// there was an error, so return the error flag
		return j;
	} else {
		// no error, so return value read
		return value;
	}
}


void SetPaneString(int nIndext,CString str)
{
	/*
//	static CMFCStatusBar * pStatusBar=NULL;
	CMFCStatusBar * pStatusBar=NULL;
	if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
		return;
	if(pStatusBar==NULL)
		pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(nIndext,str.GetString());
	*/
	CMFCStatusBar * pStatusBar=NULL;
	if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
		return;
	pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(nIndext,str.GetString());
	//pStatusBar->SetPaneTextColor (RGB(128,0,0));

// 	CRect rc;
// 	pStatusBar->GetItemRect(0, &rc);
// 	CDC* pDC = pStatusBar->GetWindowDC();
// 	rc.DeflateRect(2,0,2,0);
// 	pDC->Rectangle(rc);


// 	if (nIndext == 0)
// 	{
// 		HBITMAP hBmp = LoadBitmap(AfxGetResourceHandle(),  MAKEINTRESOURCE(IDB_UPBMP));
// 		pStatusBar->SetPaneIcon(nIndext, hBmp, RGB(192,192,192));
// 
// 	}
	/*
	status_info infoStatusbar;
	infoStatusbar.nIndex=nIndext;
	infoStatusbar.strInfoText=str;
	::PostMessage(AfxGetMainWnd()->GetActiveWindow()->m_hWnd,WM_USER_FRESHSTATUSBAR,0,(WPARAM)&infoStatusbar);
*/


}


int Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times)
{
	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = Write_Multi_org(device_var, to_write, start_address, length, retry_times);
	g_bEnableRefreshTreeView |= bTemp;
	return j;
}

int Write_Multi_org(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times)
{
// 	CString str;
// 	str.Format(_T("ID :%d Multi writing start :%d Amount: %d"),device_var,start_address,length);
// 	SetPaneString(2,str);
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		register_critical_section.Lock();
		j=write_multi(device_var,to_write,start_address,length);
		register_critical_section.Unlock();
		if (g_CommunicationType==Modbus_Serial)
		{
			Sleep(DELAY_TIME);//do this for better quickly
		}
		if(j!=-2)
		{
			//SetPaneString(2,_T("Multi-Write successful!"));
			CString str;
			str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d] 1"), device_var, ++g_llTxCount, ++g_llRxCount,g_llTxCount-g_llRxCount);
			SetPaneString(0,str);
			return j;
		}
	}
	//SetPaneString(2,_T("Multi-write failure!"));
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d] 2"), device_var, ++g_llTxCount, g_llRxCount,g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}
/**

  Read multiple values from a modbus device
<<<<<<< HEAD

  @param[out]  put_data_into_here	the values read
  @param[in]   device_var			the modbus device address
  @param[in]   start_address		the offset of thefirt value to be read in the device
  @param[in]   length				number of values to be read
  @param[in]   retry_times			the number of times to retry on read failure before giving up

  @return  0 if there were no errors

  This does NOT lock the register_critical_section

  This is a wrapper for modbus_read_multi_value
  It is provided for compatibility with existing code.
  New code should use modbus_read_multi_value() directly.

  This does NOT lock the critical section.

=======

  @param[out]  put_data_into_here	the values read
  @param[in]   device_var			the modbus device address
  @param[in]   start_address		the offset of thefirt value to be read in the device
  @param[in]   length				number of values to be read
  @param[in]   retry_times			the number of times to retry on read failure before giving up

  @return  0 if there were no errors

  This does NOT lock the register_critical_section

  This is a wrapper for modbus_read_multi_value
  It is provided for compatibility with existing code.
  New code should use modbus_read_multi_value() directly.

  This does NOT lock the critical section.

>>>>>>> 2e9ddba20a1940f319dd2ab81ecc63f9fd73e5d1
  */
int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times)
{
	return modbus_read_multi_value(
		put_data_into_here,
		device_var,
		start_address,
		length,
		retry_times );
}




int turn_hex_str_to_ten_num(char *source)
{
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
			
			default:return -1;
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


void turn_int_to_unsigned_char(char *source,int length_source,unsigned char *aim)
{
	char *p_c_temp=source;
	unsigned char *p_uc_temp=aim;
	unsigned char uctemp;
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






float get_tstat_version(unsigned short tstat_id)
{//get tstat version and judge the tstat is online or no
	//tstat is online ,return >0
	//tstat is not online ,return -2
	
	float tstat_version2=(float)read_one((unsigned char)tstat_id,4);//tstat version			
	if(tstat_version2==-2 || tstat_version2==-3)
		return tstat_version2;
	if(tstat_version2 >=240 && tstat_version2 <250)
		tstat_version2 /=10;
	else 
	{
		tstat_version2 = (float)(read_one((unsigned char)tstat_id,5,1)*256+read_one((unsigned char)tstat_id,4,1));	
		tstat_version2 /=10;
	}//tstat_version
	return tstat_version2;
}

float get_curtstat_version()
{
	float tstat_version2= product_register_value[MODBUS_VERSION_NUMBER_LO];//tstat version			
	if(tstat_version2<=0)
		return tstat_version2;
	if(tstat_version2 >=240 && tstat_version2 <250)
		tstat_version2 /=10;
	else 
	{
		tstat_version2 = (float)(product_register_value[MODBUS_VERSION_NUMBER_HI]*256+product_register_value[MODBUS_VERSION_NUMBER_LO]);	
		tstat_version2 /=10;
	}//tstat_version
	return tstat_version2;

}


int make_sure_isp_mode(int the_tstat_id)
{
	unsigned short isp_unsigned_short[20];
	int i=Read_Multi(the_tstat_id,isp_unsigned_short,100,20);
	if(i==-2 || i==-1)
		return i;//no response
	if(i<0)
		return i;//no in isp mode
	else
	{
		for(int j=0;j<20;j++)
		{
			if(isp_unsigned_short[j]!=1)
				return 0;// no in isp mode
		}
	}
	return 1;//in isp mode 
}

bool get_serialnumber(long & serial,int the_id_of_product)
{

	unsigned short SerialNum[4]={0};
	int nRet=0;
	nRet=Read_Multi(the_id_of_product,&SerialNum[0],0,4);
	serial=0;
	if(nRet>0)
	{
		serial=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
		return TRUE;
	}
		return FALSE;
}


UINT get_serialnumber()
{
	return multi_register_value[MODBUS_SERIALNUMBER_LOWORD]+multi_register_value[MODBUS_SERIALNUMBER_LOWORD+1]*256+multi_register_value[MODBUS_SERIALNUMBER_HIWORD]*256*256+multi_register_value[MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256;
}





bool multi_read_tstat(int id)
{

	bool return_value=true;
	int i;
	for(i=0;i<7;i++)
	{
		//register_critical_section.Lock();
		//int nStart = GetTickCount();
		if(-2==Read_Multi(id,&multi_register_value[i*64],i*64,64,1))
			return_value=false;

		//TRACE(_T("Read_Multi once = %d \n"), GetTickCount()-nStart);
		Sleep(50);
		//register_critical_section.Unlock();
	}
	return return_value;
}


bool can_be_writed_hex_file(int product_model,int hex_file_product_model)
{
	//product model
	// T3-8IO-------------20
	// T3-32I-------------22
	// T3-8i/60-----------23
	// Flexdriver---------25
	//Tstat5A-------------2
	//Tstat5B-------------1
	//Tstat5B2------------3
	//Tstat5C-------------4
	//Tstat5D-------------12
	//Solar---------------30
	//hex_file_product_model parameter is the hex_file_register 0x100 (256)
//	if (product_model==18||product_model==17)
	{
		return true;
	}
	if(hex_file_product_model==255)//////////////old version hex file,before 2005.11.15
		return true;
	if(product_model<=TSTAT_PRODUCT_MODEL && hex_file_product_model<=TSTAT_PRODUCT_MODEL)
		return true;
	if(product_model==LED_PRODUCT_MODEL && hex_file_product_model==LED_PRODUCT_MODEL)
		return true;
	if(product_model==PM_NC && hex_file_product_model==PM_NC)
		return true;
	if(product_model==PM_T3IOA && hex_file_product_model==PM_T3IOA)
		return true;
	if(product_model==PM_T38AIOD && hex_file_product_model==PM_T38AIOD)
		return true;
	if(product_model==T3_32I_PRODUCT_MODEL && hex_file_product_model==T3_32I_PRODUCT_MODEL)
		return true;
	if(product_model==T3_8I_16O_PRODUCT_MODEL && hex_file_product_model==T3_8I_16O_PRODUCT_MODEL)
		return true;
	if(product_model==PM_SOLAR && hex_file_product_model==PM_SOLAR)
		return true;
	if(product_model==PM_ZIGBEE && hex_file_product_model==PM_ZIGBEE)
		return true;	
	return false;
}
CString get_product_name_by_product_model(int product_model)
{
	CString return_str;
	if(product_model>0 && product_model<=TSTAT_PRODUCT_MODEL)
		product_model=TSTAT_PRODUCT_MODEL;
	switch(product_model)
	{
	case 19:return_str=_T("Tstat");break;
	case 20:return_str=_T("T3-8IO");break;
	case 22:return_str=_T("T3-32I");break;
	case 23:return_str=_T("T3-8i/60");break;
	case 25:return_str=_T("Flexdriver");break;
	case 30:return_str=_T("Solar");break;
	case PM_ZIGBEE:return_str=_T("ZigBee");break;
	default:return_str=_T("Unknown");break;
	}
	return return_str;
}

// Function : 获得单位名称，此单位用于Input Grid，Output Grid，Output Set Grid，主界面的Grid等等。
// Param: int nRange: 指示当前的Range的选择值。函数应该根据Range的选择以及TStat的型号，
//					获得单位名称，如摄氏度，华氏度，百分比，自定义的单位等。
//           int nPIDNO: 区分PID1 还是PID2，1＝PID1，2＝PID2
// return ： 单位名称 
CString GetTempUnit(int nRange, int nPIDNO)
{
	CString strTemp=_T("");

	if(nRange<0) // 使用默认的温度单位
	{
		UINT uint_temp=GetOEMCP();//get system is for chinese or english
		if(uint_temp!=936 && uint_temp!=950)
		{
			if(product_register_value[MODBUS_DEGC_OR_F]==0)	//121
			{
				strTemp.Format(_T("%cC"),176);
			}
			else
			{
				strTemp.Format(_T("%cF"),176);
			}
		}
		else
		{
			//Chinese.
			if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
			{
				strTemp=_T("℃");
			}
			else
			{
				strTemp=_T("℉");
			}
		}
		return strTemp;
	}

	if(nRange==0)		// Raw value, no unit
		strTemp=_T("");
	else if(nRange==1)
	{//
		UINT uint_temp=GetOEMCP();//get system is for chinese or english
		if(uint_temp!=936 && uint_temp!=950)
		{
			if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
			{
				strTemp.Format(_T("%cC"),176);
			}
			else
			{
				strTemp.Format(_T("%cF"),176);
			}
		}
		else
		{
			//chinese.
			if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
			{
				strTemp=_T("℃");
			}
			else
			{
				strTemp=_T("℉");
			}
		}
		return strTemp;
	}
	else if(nRange==2)
	{//
		strTemp=_T("%");
	}
	else if(nRange==3)
	{//ON/OFF
		strTemp=_T("");
	}
	else if(nRange==4)
	{//Customer Sersor
		if(nPIDNO==1)
		{
			int m_271=product_register_value[MODBUS_UNITS1_HIGH];//271 390
			int m_272=product_register_value[MODBUS_UNITS1_LOW];//272  391
			if(m_271>>8=='0')
			{
				if((m_271 & 0xFF) =='0')
				{
					if(m_272>>8=='0')
						strTemp.Format(_T("%c"),m_272 & 0xFF);
					else
						strTemp.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
				}
				else
					strTemp.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
			}
			else
				strTemp.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
		}
		else if(nPIDNO==2)
		{
			int m_273=product_register_value[MODBUS_UNITS2_HIGH];//273  392;
			int m_274=product_register_value[MODBUS_UNITS2_LOW];//274 393;
			if(m_273>>8=='0')
			{
				if((m_273 & 0xFF)=='0')
				{
					if(m_274>>8=='0')
						strTemp.Format(_T("%c"),m_274 & 0xFF);
					else
						strTemp.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
				}
				else
					strTemp.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
			}		
			else
				strTemp.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);

		}
	}	
	return strTemp;

	//CString strTemp=_T("");
	//
	//if(nRange<0) // 使用默认的温度单位
	//{
	//	UINT uint_temp=GetOEMCP();//get system is for chinese or english
	//	if(uint_temp!=936 && uint_temp!=950)
	//	{
	//		if(multi_register_value[121]==0)
	//		{
	//			strTemp.Format(_T("%cC"),176);
	//		}
	//		else
	//		{
	//			strTemp.Format(_T("%cF"),176);
	//		}
	//	}
	//	else
	//	{
	//		//Chinese.
	//		if(multi_register_value[121]==0)
	//		{
	//			strTemp=_T("°C");
	//		}
	//		else
	//		{
	//			strTemp=_T("°F");
	//		}
	//	}
	//	return strTemp;
	//}

	//if(nRange==0)		// Raw value, no unit
	//	strTemp=_T("");
	//else if(nRange==1)
	//{//
	//	UINT uint_temp=GetOEMCP();//get system is for chinese or english
	//	if(uint_temp!=936 && uint_temp!=950)
	//	{
	//		if(multi_register_value[121]==0)
	//		{
	//			strTemp.Format(_T("%cC"),176);
	//		}
	//		else
	//		{
	//			strTemp.Format(_T("%cF"),176);
	//		}
	//	}
	//	else
	//	{
	//		//chinese.
	//		if(multi_register_value[121]==0)
	//		{
	//			strTemp=_T("°C");
	//		}
	//		else
	//		{
	//			strTemp=_T("°F");
	//		}
	//	}
	//	return strTemp;
	//}
	//else if(nRange==2)
	//{//
	//	strTemp=_T("%");
	//}
	//else if(nRange==3)
	//{//ON/OFF
	//	strTemp=_T("");
	//}
	//else if(nRange==4)
	//{//Customer Sersor
	//	if(nPIDNO==1)
	//	{
	//		int m_271=multi_register_value[271];
	//		int m_272=multi_register_value[272];
	//		if(m_271>>8=='0')
	//		{
	//			if((m_271 & 0xFF) =='0')
	//			{
	//				if(m_272>>8=='0')
	//					strTemp.Format(_T("%c"),m_272 & 0xFF);
	//				else
	//					strTemp.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
	//			}
	//			else
	//				strTemp.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
	//		}
	//		else
	//			strTemp.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
	//	}
	//	else if(nPIDNO==2)
	//	{
	//		int m_273=multi_register_value[273];
	//		int m_274=multi_register_value[274];
	//		if(m_273>>8=='0')
	//		{
	//			if((m_273 & 0xFF)=='0')
	//			{
	//				if(m_274>>8=='0')
	//					strTemp.Format(_T("%c"),m_274 & 0xFF);
	//				else
	//					strTemp.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
	//			}
	//			else
	//				strTemp.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
	//		}		
	//		else
	//			strTemp.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);

	//	}
	//}	
	//return strTemp;
}

CString get_product_class_name_by_model_ID(int nModelID)
{
	CString strClassName;
	switch(nModelID)
	{
	case 2:strClassName=g_strTstat5a;break;
	case 1:strClassName=g_strTstat5b;break;
	case 3:strClassName=g_strTstat5b;break;
	case 4:strClassName=g_strTstat5c;break;
	case 6:strClassName=g_strTstat6;break;
	case 7:strClassName=g_strTstat7;break;
	case 12:strClassName=g_strTstat5d;break;
	case PM_NC:strClassName=g_strnetWork;break;
	case NET_WORK_OR485_PRODUCT_MODEL:strClassName=g_strOR485;break;
	case 17: strClassName=g_strTstat5f;break;
	case 18:strClassName=g_strTstat5g;break;
	case 16:strClassName=g_strTstat5e;break;
	case 19:strClassName=g_strTstat5h;break;
	case PM_LightingController: strClassName = g_strLightingCtrl;
	
	case 13:
	case 14:break;
	default:strClassName=g_strTstat5a;break;
	}

	return strClassName;
}


BOOL GetSerialComPortNumber1(vector<CString>& szComm)
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
			//TRACE("Registry's   Read!");   
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

//Add 20130516  by Fance
//UINT MsgType
//unsigned char device_id
//unsigned short address
//short new_value
//short old_value


BOOL Post_Thread_Message(UINT MsgType,
	unsigned char device_id,
	unsigned short address,
	short new_value,
	short old_value,
	HWND Dlg_hwnd,
	UINT CTRL_ID,
	CString Changed_Name)
{
	_MessageWriteOneInfo *My_Write_Struct = new _MessageWriteOneInfo;
	My_Write_Struct->device_id = device_id;
	My_Write_Struct->address = address;
	My_Write_Struct->new_value = new_value;
	My_Write_Struct->old_value = old_value;
	My_Write_Struct->hwnd = Dlg_hwnd;
	My_Write_Struct->CTRL_ID = CTRL_ID;
	My_Write_Struct->Changed_Name = Changed_Name;

	//search the id ,if not in the vector, push back into the vector.
	bool find_id=false;
	for (int i=0;i<Change_Color_ID.size();i++)
	{
		if(Change_Color_ID.at(i)!=CTRL_ID)
			continue;
		else
			find_id = true;
	}
	if(!find_id)
		Change_Color_ID.push_back(CTRL_ID);
	else
		return FALSE;
	
	if(!PostThreadMessage(nThreadID,MY_WRITE_ONE,(WPARAM)My_Write_Struct,NULL))//post thread msg
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
<<<<<<< HEAD
=======

BOOL Post_Read_one_Thread_Message(
	unsigned char device_id,
	unsigned short address,
	HWND Dlg_hwnd)
{
	_MessageReadOneInfo *My_Read_Struct = new _MessageReadOneInfo;
	My_Read_Struct->device_id=device_id;
	My_Read_Struct->address=address;
	My_Read_Struct->new_value = -1;
	My_Read_Struct->hwnd = Dlg_hwnd;
	if(!PostThreadMessage(nThreadID,MY_READ_ONE,(WPARAM)My_Read_Struct,NULL))//post thread msg
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


 
>>>>>>> 2e9ddba20a1940f319dd2ab81ecc63f9fd73e5d1
