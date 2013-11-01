
#include "stdafx.h"
#include "globle_function.h"
#include "Windows.h"
#include "T3000.h"



#include "T3000RegAddress.h"
#include "gloab_define.h"

#define DELAY_TIME	 10	//MS
#define Modbus_Serial	0
#define	Modbus_TCP	1

#include "modbus_read_write.cpp"

#pragma region For_Bacnet_program_Use

extern char mycode[1024];
extern int my_lengthcode;// the length of the edit of code
#pragma endregion
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
int Write_Multi_short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,int retry_times)
{
	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = Write_Multi_org_short(device_var, to_write, start_address, length, retry_times);
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
			str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount,g_llTxCount-g_llRxCount);
			SetPaneString(0,str);
			return j;
		}
	}
	//SetPaneString(2,_T("Multi-write failure!"));
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount,g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}


int Write_Multi_org_short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,int retry_times)
{
	// 	CString str;
	// 	str.Format(_T("ID :%d Multi writing start :%d Amount: %d"),device_var,start_address,length);
	// 	SetPaneString(2,str);
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		register_critical_section.Lock();
		j=write_multi_Short(device_var,to_write,start_address,length);
		register_critical_section.Unlock();
		if (g_CommunicationType==Modbus_Serial)
		{
			Sleep(DELAY_TIME);//do this for better quickly
		}
		if(j!=-2)
		{
			//SetPaneString(2,_T("Multi-Write successful!"));
			CString str;
			str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount,g_llTxCount-g_llRxCount);
			SetPaneString(0,str);
			return j;
		}
	}
	//SetPaneString(2,_T("Multi-write failure!"));
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount,g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}
/**

  Read multiple values from a modbus device

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
	
	float tstat_version2=(float)product_register_value[4];//tstat version			
	if(tstat_version2==-2 || tstat_version2==-3)
		return tstat_version2;
	if(tstat_version2 >=240 && tstat_version2 <250)
		tstat_version2 /=10;
	else 
	{
		tstat_version2 = (float)(product_register_value[5]*256+product_register_value[4]);	
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
BOOL Post_Refresh_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize,int block_size)
{
	_MessageRefreshListInfo *pmy_refresh_info = new _MessageRefreshListInfo;
	pmy_refresh_info->deviceid = deviceid;
	pmy_refresh_info->command = command;
	pmy_refresh_info->start_instance = start_instance;
	pmy_refresh_info->end_instance = end_instance;
	pmy_refresh_info->entitysize = entitysize;
	pmy_refresh_info->block_size = block_size;
	if(!PostThreadMessage(nThreadID,MY_BAC_REFRESH_LIST,(WPARAM)pmy_refresh_info,NULL))//post thread msg
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
BOOL Post_Write_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize,HWND hWnd ,CString Task_Info)
{
	_MessageWriteListInfo *pmy_write_info = new _MessageWriteListInfo;
	pmy_write_info->deviceid = deviceid;
	pmy_write_info->command = command;
	pmy_write_info->start_instance = start_instance;
	pmy_write_info->end_instance = end_instance;
	pmy_write_info->Write_Info = Task_Info;
	pmy_write_info->entitysize = entitysize;
	pmy_write_info->hWnd = hWnd;
	if(!PostThreadMessage(nThreadID,MY_BAC_WRITE_LIST,(WPARAM)pmy_write_info,NULL))//post thread msg
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//Add 20130516  by Fance
//UINT MsgType
//unsigned char device_id
//unsigned short address
//short new_value
//short old_value
BOOL Post_Invoke_ID_Monitor_Thread(UINT MsgType,
	int Invoke_ID,
	HWND hwnd,
	CString Show_Detail 
	)
{
	_MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
	pMy_Invoke_id->Invoke_ID = Invoke_ID;
	pMy_Invoke_id->hwnd = hwnd;
	pMy_Invoke_id->task_info = Show_Detail;
	if(!PostThreadMessage(nThreadID,MY_INVOKE_ID,(WPARAM)pMy_Invoke_id,NULL))//post thread msg
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

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
extern int my_lengthcode;
/***************************************************
**
** Write Bacnet private data to device
** Add by Fance
****************************************************/
int WritePrivateData(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance )
{
	// TODO: Add your control notification handler code here
	unsigned short entitysize=0;
	uint8_t apdu[480] = { 0 };
	uint8_t test_value[480] = { 0 };
	int private_data_len = 0;	
	BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
	BACNET_APPLICATION_DATA_VALUE test_data_value = { 0 };
	BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };
	BACNET_PRIVATE_TRANSFER_DATA test_data = { 0 };
	bool status = false;

	private_data.vendorID = BACNET_VENDOR_ID;
	private_data.serviceNumber = 1;

	unsigned max_apdu = 0;
	switch(command)
	{
	case WRITEINPUT_T3000:
		entitysize = sizeof(Str_in_point);
		break;
	case WRITEPROGRAM_T3000:
		entitysize = sizeof(Str_variable_point);
		break;
	case WRITEPROGRAMCODE_T3000:
		entitysize = my_lengthcode;
		break;
	case WRITEVARIABLE_T3000:
		entitysize = sizeof(Str_variable_point);
		break;
	case  WRITEOUTPUT_T3000:
		entitysize = sizeof(Str_out_point);
		break;
	case WRITEWEEKLYROUTINE_T3000:
		entitysize = sizeof(Str_weekly_routine_point);
		break;
	case WRITEANNUALROUTINE_T3000:
		entitysize = sizeof(Str_annual_routine_point);
		break;
	case WRITETIMESCHEDULE_T3000:
		entitysize =9*16;// sizeof(Str_schedual_time_point);
		break;
	case WRITEANNUALSCHEDULE_T3000:
		entitysize = 48;
		break;
	case RESTARTMINI_COMMAND:
		entitysize = sizeof(Time_block_mini);
		break;
	default:
		{
			AfxMessageBox(_T("Entitysize length error!"));
		}
		break;
	}
	Str_user_data_header private_data_chunk;
	private_data_chunk.total_length = PRIVATE_HEAD_LENGTH + (end_instance - start_instance + 1)*entitysize;
	private_data_chunk.command = command;
	private_data_chunk.point_start_instance = start_instance;
	private_data_chunk.point_end_instance = end_instance;
	private_data_chunk.entitysize=entitysize;

	char SendBuffer[1000];
	memset(SendBuffer,0,1000);
	char * temp_buffer = SendBuffer;
	memcpy_s(SendBuffer,PRIVATE_HEAD_LENGTH ,&private_data_chunk,PRIVATE_HEAD_LENGTH );

	switch(command)
	{
	case WRITEINPUT_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_in_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_in_point),&m_Input_data.at(i + start_instance),sizeof(Str_in_point));
		}
		break;
	case WRITEPROGRAM_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_program_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_program_point),&m_Program_data.at(i + start_instance),sizeof(Str_program_point));
		}

		break;
	case WRITEPROGRAMCODE_T3000:
		memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,entitysize,mycode,my_lengthcode);
		break;
	case WRITEVARIABLE_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_variable_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_variable_point),&m_Variable_data.at(i + start_instance),sizeof(Str_variable_point));
		}
		break;
	case  WRITEOUTPUT_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_out_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_out_point),&m_Output_data.at(i + start_instance),sizeof(Str_out_point));
		}
		break;
	case WRITEWEEKLYROUTINE_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_weekly_routine_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_weekly_routine_point),&m_Weekly_data.at(i + start_instance),sizeof(Str_weekly_routine_point));
		}
		break;
	case WRITETIMESCHEDULE_T3000:
		temp_buffer = temp_buffer + PRIVATE_HEAD_LENGTH;
		for (int j=0;j<9;j++)
		{
			for (int i=0;i<8;i++)
			{
				*(temp_buffer++) = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes;// = *(my_temp_point ++);
				*(temp_buffer++) = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours;// = *(my_temp_point ++);
			}
		}
		
		break;
	case  WRITEANNUALROUTINE_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_annual_routine_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_annual_routine_point),&m_Annual_data.at(i + start_instance),sizeof(Str_annual_routine_point));
		}
		break;
	case WRITEANNUALSCHEDULE_T3000:
		memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,48,&g_DayState[annual_list_line],48);

		//memcpy_s(g_DayState[annual_list_line],block_length,my_temp_point,block_length);
		break;
	case RESTARTMINI_COMMAND:
		{
			SendBuffer[7] = Device_time.ti_sec;
			SendBuffer[8] = Device_time.ti_min;
			SendBuffer[9] = Device_time.ti_hour;
			SendBuffer[10] = Device_time.dayofmonth;
			SendBuffer[11] = Device_time.month;
			SendBuffer[12] = Device_time.year;
			SendBuffer[13] = Device_time.dayofweek;
			SendBuffer[14] =	Device_time.dayofyear && 0x00ff;
			SendBuffer[15] =	(Device_time.dayofyear && 0xff00)>>8;
			SendBuffer[16] = Device_time.isdst;

			//temp_struct_value = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			//Device_time.dayofyear = temp_struct_value;
			//my_temp_point = my_temp_point + 2;

			//Device_time.isdst = *(my_temp_point ++);

		}

		//Device_time
		break;
	default:
		{
			AfxMessageBox(_T("Command not match!Please Check it!"));
			return -1;
		}
		break;
	}


	Set_transfer_length(private_data_chunk.total_length);
	//transfer_len=6;

	status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&SendBuffer, &data_value);
	//ct_test(pTest, status == true);
	private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
	private_data.serviceParameters = &test_value[0];
	private_data.serviceParametersLen = private_data_len;

	BACNET_ADDRESS dest = { 0 };
	status = address_get_by_device(deviceid, &max_apdu, &dest);
	if (status) 
	{
		return Send_ConfirmedPrivateTransfer(&dest,&private_data);
	}
	return -2;
}



/************************************************************************/
/*
Author: Fance
Get Bacnet Private Data
<param name="deviceid">Bacnet Device ID
<param name="command">Bacnet command
<param name="start_instance">start point
<param name="end_instance">end point
<param name="entitysize">Block size of read
*/
/************************************************************************/
int GetPrivateData(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,int16_t entitysize)
{
	// TODO: Add your control notification handler code here

	uint8_t apdu[480] = { 0 };
	uint8_t test_value[480] = { 0 };
	int apdu_len = 0;
	int private_data_len = 0;	
	unsigned max_apdu = 0;
	BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
//	BACNET_APPLICATION_DATA_VALUE test_data_value = { 0 };
	BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };
//	BACNET_PRIVATE_TRANSFER_DATA test_data = { 0 };
	bool status = false;

	private_data.vendorID = BACNET_VENDOR_ID;
	private_data.serviceNumber = 1;



	Str_user_data_header private_data_chunk;
	private_data_chunk.total_length=PRIVATE_HEAD_LENGTH;
	private_data_chunk.command = command;
	private_data_chunk.point_start_instance=start_instance;
	private_data_chunk.point_end_instance=end_instance;
	private_data_chunk.entitysize=entitysize;
	// char private_data_chunk[33] = { "3031323334353637383940" };
	Set_transfer_length(PRIVATE_HEAD_LENGTH);
	//transfer_len=6;

	status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&private_data_chunk, &data_value);
	//ct_test(pTest, status == true);
	private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
	private_data.serviceParameters = &test_value[0];
	private_data.serviceParametersLen = private_data_len;

	BACNET_ADDRESS dest = { 0 };
	status = address_get_by_device(deviceid, &max_apdu, &dest);
	if (status) 
	{
		return Send_ConfirmedPrivateTransfer(&dest,&private_data);
		//return g_invoke_id;
	}
	else
		return -2;

}

/***************************************************
**
** Receive Bacnet private data from device , and handle the data.
** Add by Fance
****************************************************/

int CM5ProcessPTA(	BACNET_PRIVATE_TRANSFER_DATA * data)
{
	int i;
	int block_length;
	char *my_temp_point;
	int temp_struct_value;


	int iLen;   /* Index to current location in data */
	uint32_t uiErrorCode;
	char cBlockNumber;
	uint32_t ulTemp;
	int tag_len;
	uint8_t tag_number;
	uint32_t len_value_type;
	BACNET_OCTET_STRING Temp_CS;
	iLen = 0;
	int command_type;

	/* Error code is returned for read and write operations */

	tag_len =  decode_tag_number_and_value(&data->serviceParameters[iLen],   &tag_number, &len_value_type);
	iLen += tag_len;
	if (tag_number != BACNET_APPLICATION_TAG_OCTET_STRING) 
	{
		/* if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {*/
#if PRINT_ENABLED
		printf("CPTA: Bad Encoding!\n");
#endif
		return 0;
	}
	//iLen +=
	//    decode_unsigned(&data->serviceParameters[iLen], len_value_type,
	//    &uiErrorCode);
	decode_octet_string(&data->serviceParameters[iLen], len_value_type,&Temp_CS);
	command_type = Temp_CS.value[2];



	int start_instance=0;
	int end_instance = 0;
	///////////////////////////////
	switch(command_type)
	{
	case READOUTPUT_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_out_point))!=0)
				return -1;	//得到的结构长度错误;

			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_out_point);
			//m_Input_data_length = block_length;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			//m_Output_data.clear();
			for (i=start_instance;i<=end_instance;i++)
			{
				//Str_out_point temp_out;
				memcpy_s( m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH,my_temp_point,STR_OUT_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_OUT_DESCRIPTION_LENGTH;
				memcpy_s(m_Output_data.at(i).label,STR_OUT_LABEL ,my_temp_point,STR_OUT_LABEL );
				my_temp_point=my_temp_point + STR_OUT_LABEL ;

				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_Output_data.at(i).value = temp_struct_value;
				//memcpy_s(Private_data[i].value,4,temp_struct_value,4);
				my_temp_point=my_temp_point+4;

				m_Output_data.at(i).auto_manual = *(my_temp_point++);
				m_Output_data.at(i).digital_analog = *(my_temp_point++);
				m_Output_data.at(i).access_level = *(my_temp_point++);
				m_Output_data.at(i).control = *(my_temp_point++);
				m_Output_data.at(i).digital_control = *(my_temp_point++);
				m_Output_data.at(i).decom	= *(my_temp_point++);
				m_Output_data.at(i).range = *(my_temp_point++);
				m_Output_data.at(i).m_del_low = *(my_temp_point++);
				m_Output_data.at(i).s_del_high = *(my_temp_point++);
				//temp_out.delay_timer = *(my_temp_point++);  Output 这个Delay time先不管 清0
				m_Output_data.at(i).delay_timer = 0;
				my_temp_point = my_temp_point + 2;

				//m_Output_data.push_back(temp_out);
			}
			return READOUTPUT_T3000;
		}
		break;

	case READINPUT_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_in_point))!=0)
				return -1;	//得到的结构长度错误;
			int test =  sizeof(Str_in_point);
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_in_point);
			//m_Input_data_length = block_length;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			//m_Input_data.clear();
			for (i=start_instance;i<=end_instance;i++)
			{
			//	Str_in_point temp_in;
				memcpy_s( m_Input_data.at(i).description,STR_IN_DESCRIPTION_LENGTH,my_temp_point,STR_IN_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_IN_DESCRIPTION_LENGTH;
				memcpy_s(m_Input_data.at(i).label,STR_IN_LABEL ,my_temp_point,STR_IN_LABEL );
				my_temp_point=my_temp_point + STR_IN_LABEL ;

				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_Input_data.at(i).value = temp_struct_value;
				//memcpy_s(Private_data[i].value,4,temp_struct_value,4);
				my_temp_point=my_temp_point+4;
				m_Input_data.at(i).filter = *(my_temp_point++);
				m_Input_data.at(i).decom	= *(my_temp_point++);
				m_Input_data.at(i).sen_on	= *(my_temp_point++);;
				m_Input_data.at(i).sen_off = *(my_temp_point++);
				m_Input_data.at(i).control = *(my_temp_point++);
				m_Input_data.at(i).auto_manual = *(my_temp_point++);
				m_Input_data.at(i).digital_analog = *(my_temp_point++);
				m_Input_data.at(i).calibration_sign = *(my_temp_point++);
				m_Input_data.at(i).calibration_increment = *(my_temp_point++);
				m_Input_data.at(i).unused = *(my_temp_point++);
				m_Input_data.at(i).calibration = *(my_temp_point++);
				m_Input_data.at(i).range = *(my_temp_point++);
				//m_Input_data.push_back(temp_in);
			}
			return READINPUT_T3000;
		}
		break;
	case READVARIABLE_T3000   :
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_variable_point))!=0)
				return -1;	//得到的结构长度错误;

			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_variable_point);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			//m_Input_data_length = block_length;
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			//m_Variable_data.clear();
			for (i=start_instance;i<=end_instance;i++)
			{
				//Str_variable_point temp_variable;
				memcpy_s( m_Variable_data.at(i).description,STR_VARIABLE_DESCRIPTION_LENGTH,my_temp_point,STR_VARIABLE_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_VARIABLE_DESCRIPTION_LENGTH;
				memcpy_s(m_Variable_data.at(i).label,STR_VARIABLE_LABEL ,my_temp_point,STR_VARIABLE_LABEL );
				my_temp_point=my_temp_point + STR_VARIABLE_LABEL ;

				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_Variable_data.at(i).value = temp_struct_value;
				//memcpy_s(Private_data[i].value,4,temp_struct_value,4);
				my_temp_point=my_temp_point+4;
				
				
				m_Variable_data.at(i).auto_manual = *(my_temp_point++);
				m_Variable_data.at(i).digital_analog = *(my_temp_point++);
				m_Variable_data.at(i).control = *(my_temp_point++);
				m_Variable_data.at(i).unused = *(my_temp_point++);
				m_Variable_data.at(i).range = *(my_temp_point++);

				//m_Variable_data.push_back(temp_variable);
			}
			return READVARIABLE_T3000;
		}
		break;
	case READWEEKLYROUTINE_T3000  :
		if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_weekly_routine_point))!=0)
			return -1;	//得到的结构长度错误;
		block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_weekly_routine_point);

		my_temp_point = (char *)Temp_CS.value + 3;
		start_instance = *my_temp_point;
		my_temp_point++;
		end_instance = *my_temp_point;
		my_temp_point++;
		my_temp_point = my_temp_point + 2;

		for (i=start_instance;i<=end_instance;i++)
		{
			//Str_program_point temp_in;
			memcpy_s( m_Weekly_data.at(i).description,STR_WEEKLY_DESCRIPTION_LENGTH,my_temp_point,STR_WEEKLY_DESCRIPTION_LENGTH);
			my_temp_point=my_temp_point + STR_WEEKLY_DESCRIPTION_LENGTH;
			memcpy_s( m_Weekly_data.at(i).label,STR_WEEKLY_LABEL_LENGTH ,my_temp_point,STR_WEEKLY_LABEL_LENGTH );
			my_temp_point=my_temp_point + STR_WEEKLY_LABEL_LENGTH ;


			m_Weekly_data.at(i).value = (unsigned char)(*(my_temp_point++));
			m_Weekly_data.at(i).auto_manual = (unsigned char)(*(my_temp_point++));
			m_Weekly_data.at(i).override_1_value =  (unsigned char)(*(my_temp_point++));
			m_Weekly_data.at(i).override_2_value =  (unsigned char)(*(my_temp_point++));
			m_Weekly_data.at(i).off =  (unsigned char)(*(my_temp_point++));
			m_Weekly_data.at(i).unused = (unsigned char)(*(my_temp_point++));
		}
		return READWEEKLYROUTINE_T3000;
		break;
	case READANNUALROUTINE_T3000  :
		if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_annual_routine_point))!=0)
			return -1;	//得到的结构长度错误;
		block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_annual_routine_point);

		my_temp_point = (char *)Temp_CS.value + 3;
		start_instance = *my_temp_point;
		my_temp_point++;
		end_instance = *my_temp_point;
		my_temp_point++;
		my_temp_point = my_temp_point + 2;

		for (i=start_instance;i<=end_instance;i++)
		{
			//Str_program_point temp_in;
			memcpy_s( m_Annual_data.at(i).description,STR_ANNUAL_DESCRIPTION_LENGTH,my_temp_point,STR_ANNUAL_DESCRIPTION_LENGTH);
			my_temp_point=my_temp_point + STR_ANNUAL_DESCRIPTION_LENGTH;
			memcpy_s( m_Annual_data.at(i).label,STR_ANNUAL_LABEL_LENGTH ,my_temp_point,STR_ANNUAL_LABEL_LENGTH );
			my_temp_point=my_temp_point + STR_ANNUAL_LABEL_LENGTH ;


			m_Annual_data.at(i).value = (unsigned char)(*(my_temp_point++));
			m_Annual_data.at(i).auto_manual = (unsigned char)(*(my_temp_point++));
			my_temp_point++;
		}
		return READANNUALROUTINE_T3000;
		break;
	case READPROGRAM_T3000:
		{

			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_program_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_program_point);
			//m_Input_data_length = block_length;
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			//m_Program_data.clear();
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			for (i=start_instance;i<=end_instance;i++)
			{
				//Str_program_point temp_in;
				memcpy_s( m_Program_data.at(i).description,STR_PROGRAM_DESCRIPTION_LENGTH,my_temp_point,STR_PROGRAM_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_PROGRAM_DESCRIPTION_LENGTH;
				memcpy_s( m_Program_data.at(i).label,STR_PROGRAM_LABEL_LENGTH ,my_temp_point,STR_PROGRAM_LABEL_LENGTH );
				my_temp_point=my_temp_point + STR_PROGRAM_LABEL_LENGTH ;
				 m_Program_data.at(i).bytes	= ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
				 my_temp_point = my_temp_point + 2;
				 m_Program_data.at(i).on_off = *(my_temp_point++);
				 m_Program_data.at(i).auto_manual = *(my_temp_point++);
				 m_Program_data.at(i).com_prg = *(my_temp_point++);
				 m_Program_data.at(i).errcode = *(my_temp_point++);
				 m_Program_data.at(i).unused = *(my_temp_point++);
				//m_Program_data.push_back(temp_in);
			}
			return READPROGRAM_T3000;
		}
		break;
	case READPROGRAMCODE_T3000://Fance 将program code 存至Buf 等待发送消息后使用解码函数
		{
		block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
		my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
		memset(mycode,0,1024);
		memcpy_s(mycode,block_length,my_temp_point,block_length);
		return READPROGRAMCODE_T3000;
		}
		break;
	case  READTIMESCHEDULE_T3000:
		block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
		my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
		if(block_length!=(16*9))
			return -1;
		//copy the schedule day time to my own buffer.
		for (int j=0;j<9;j++)
		{
			for (int i=0;i<8;i++)
			{
				m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes = *(my_temp_point ++);
				m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours = *(my_temp_point ++);
			}
		}
	
		return READTIMESCHEDULE_T3000;
		break;
	case READANNUALSCHEDULE_T3000:
		block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
		my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
		if(block_length!=48)
			return -1;
		memset(&g_DayState[annual_list_line],0,48);
		memcpy_s(&g_DayState[annual_list_line],block_length,my_temp_point,block_length);
		

		return READANNUALSCHEDULE_T3000;
		break;
	case  TIME_COMMAND:
		block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
		my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
		if(block_length!=sizeof(Time_block_mini))
			return -1;
		Device_time.ti_sec = *(my_temp_point ++);
		Device_time.ti_min = *(my_temp_point ++);
		Device_time.ti_hour = *(my_temp_point ++);
		Device_time.dayofmonth = *(my_temp_point ++);
		Device_time.month = *(my_temp_point ++);
		Device_time.year = *(my_temp_point ++);
		Device_time.dayofweek = *(my_temp_point ++);

		//temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
		//Device_time.dayofyear = temp_struct_value;
		//my_temp_point = my_temp_point + 4;

		temp_struct_value = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
		Device_time.dayofyear = temp_struct_value;
		my_temp_point = my_temp_point + 2;

		Device_time.isdst = *(my_temp_point ++);

		if(Device_time.ti_sec>=60)
			Device_time.ti_sec=0;
		if(Device_time.ti_min>=60)
			Device_time.ti_min=0;
		if(Device_time.ti_hour>=24)
			Device_time.ti_hour=0;
		if((Device_time.dayofmonth>=32)||(Device_time.dayofmonth==0))
			Device_time.dayofmonth=1;
		if((Device_time.month>12) || (Device_time.month == 0))
			Device_time.month = 1;
		if((Device_time.year>50))
			Device_time.year = 13;
		if((Device_time.dayofweek >7) || (Device_time.dayofweek == 0))
			Device_time.dayofweek = 1;
		if((Device_time.dayofyear >366) || (Device_time.dayofyear == 0))
			Device_time.dayofyear = 1;
		//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,NULL,NULL);
		//byte  ti_min;         // 0-59
		//byte  ti_hour;           // 0-23
		//byte  dayofmonth;   // 1-31
		//byte  month;          // 0-11
		//byte  year;           // year - 1900
		//byte  dayofweek;        // 0-6 ; 0=Sunday
		//int   dayofyear;    // 0-365 gmtime
		//signed char isdst;


		return TIME_COMMAND;
		break;
	case READCONTROLLER_T3000:
		if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_controller_point))!=0)
			return -1;	//得到的结构长度错误;
		block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_controller_point);

		my_temp_point = (char *)Temp_CS.value + 3;
		start_instance = *my_temp_point;
		my_temp_point++;
		end_instance = *my_temp_point;
		my_temp_point++;
		my_temp_point = my_temp_point + 2;

		for (i=start_instance;i<=end_instance;i++)
		{
			m_controller_data.at(i).input.number = *(my_temp_point++);
			m_controller_data.at(i).input.point_type = *(my_temp_point++);
			m_controller_data.at(i).input.panel = *(my_temp_point++);

			//这里先加卡关条件，目前暂时不支持 其他panel的Input
			//if(m_controller_data.at(i).input.number>=BAC_INPUT_ITEM_COUNT)
			//	m_controller_data.at(i).input.number = 0;
			if(m_controller_data.at(i).input.panel != bac_gloab_panel )
				m_controller_data.at(i).input.panel = bac_gloab_panel;

			temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			m_controller_data.at(i).input_value = temp_struct_value;

			my_temp_point=my_temp_point+4;
			temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			m_controller_data.at(i).value = temp_struct_value;
			my_temp_point=my_temp_point+4;

			m_controller_data.at(i).setpoint.number = *(my_temp_point++);
			m_controller_data.at(i).setpoint.point_type = *(my_temp_point++);
			m_controller_data.at(i).setpoint.panel = *(my_temp_point++);

			temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			m_controller_data.at(i).setpoint_value = temp_struct_value;
			my_temp_point=my_temp_point+4;

			m_controller_data.at(i).units = *(my_temp_point++);
			m_controller_data.at(i).auto_manual = *(my_temp_point++);
			m_controller_data.at(i).action = *(my_temp_point++);
			m_controller_data.at(i).repeats_per_min = *(my_temp_point++);
			m_controller_data.at(i).unused = *(my_temp_point++);
			m_controller_data.at(i).prop_high = *(my_temp_point++);
			m_controller_data.at(i).proportional = *(my_temp_point++);
			m_controller_data.at(i).reset = *(my_temp_point++);
			m_controller_data.at(i).bias = *(my_temp_point++);
			m_controller_data.at(i).rate = *(my_temp_point++);
		}



		return READCONTROLLER_T3000;
		break;
	}
	return 1;
}


extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
void local_handler_conf_private_trans_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
    BACNET_PRIVATE_TRANSFER_DATA data;
    int len;

/*
 * Note:
 * We currently don't look at the source address and service data
 * but we probably should to verify that the ack is oneit is what
 * we were expecting. But this is just to silence some compiler
 * warnings from Borland.
 */
    src = src;
    service_data = service_data;

    len = 0;
#if PRINT_ENABLED
    printf("Received Confirmed Private Transfer Ack!\n");
#endif

    len = ptransfer_decode_service_request(service_request, service_len, &data);        /* Same decode for ack as for service request! */
    if (len < 0) {
#if PRINT_ENABLED
        printf("cpta: Bad Encoding!\n");
#endif
    }
	int receive_data_type;

	receive_data_type = CM5ProcessPTA(&data);
	switch(receive_data_type)
	{
	case READINPUT_T3000:
	//	::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
		copy_data_to_ptrpanel(TYPE_INPUT);
		break;
	case READPROGRAM_T3000:
	//	::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
		copy_data_to_ptrpanel(TYPE_ALL);
		break;
	case READPROGRAMCODE_T3000:
		::PostMessage(m_program_edit_hwnd,WM_REFRESH_BAC_PROGRAM_RICHEDIT,NULL,NULL);
		break;
	case READVARIABLE_T3000:
	//	::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
		copy_data_to_ptrpanel(TYPE_VARIABLE);
		break;
	case READOUTPUT_T3000:
	//	::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
		copy_data_to_ptrpanel(TYPE_OUTPUT);
		break;
	case READWEEKLYROUTINE_T3000:
		copy_data_to_ptrpanel(TYPE_WEEKLY);
		break;
	case READANNUALROUTINE_T3000:
		copy_data_to_ptrpanel(TYPE_ANNUAL);
		break;
	case READTIMESCHEDULE_T3000:
		::PostMessage(m_schedule_time_dlg_hwnd,WM_REFRESH_BAC_SCHEDULE_LIST,NULL,NULL);
		break;
	case TIME_COMMAND:
		::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,TIME_COMMAND,NULL);
		break;
	case READANNUALSCHEDULE_T3000:
		::PostMessage(m_schedule_day_dlg_hwnd,WM_REFRESH_BAC_DAY_CAL,NULL,NULL);
		break;
	case READCONTROLLER_T3000:
		//::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
		break;
	default:
		break;
	}

    return;
}

//This function coded by Fance,used to split the cstring to each part.
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken)
{
	CString sTempSource, sTempSplitted;

	sTempSource = sSource;

	int nPos = sTempSource.Find(sToken);

	//--if there are no token in the string, then add itself and return.
	if(nPos == -1)
		saArray.Add(sTempSource);
	else
	{
		while(sTempSource.GetLength() > 0)
		{
			nPos = sTempSource.Find(sToken);
			if(nPos == -1)
			{
				saArray.Add(sTempSource.Trim());
				break;
			}
			else if(nPos == 0)
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


 
