
#include "stdafx.h"
#include "globle_function.h"
#include "Windows.h"
#include "T3000.h"
#include "ado/ADO.h"


#include "T3000RegAddress.h"
#include "gloab_define.h"
#include "DialogCM5_BacNet.h"
#include "CM5\MyOwnListCtrl.h"
#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetProgram.h"
#include "BacnetVariable.h"
#include "globle_function.h"

#include "gloab_define.h"
#include "datalink.h"
#include "BacnetWait.h"
#include "Bacnet_Include.h"
#include "CM5\ud_str.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetAnnualRoutine.h"
#include "AnnualRout_InsertDia.h"
#include "BacnetController.h"
#include "BacnetScreen.h"
#include "BacnetMonitor.h"
#include "rs485.h"

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
//extern int modbus_read_multi_value( 
//	unsigned short *put_data_into_here,
//	unsigned char device_var,
//	unsigned short start_address,
//	int length,
//	int retry_times );
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
	return product_register_value[MODBUS_SERIALNUMBER_LOWORD]+product_register_value[MODBUS_SERIALNUMBER_LOWORD+1]*256+product_register_value[MODBUS_SERIALNUMBER_HIWORD]*256*256+product_register_value[MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256;
}





bool multi_read_tstat(int id)
{

	bool return_value=true;
	int i;
	for(i=0;i<7;i++)
	{
		//register_critical_section.Lock();
		//int nStart = GetTickCount();
		if(-2==Read_Multi(id,&product_register_value[i*64],i*64,64,1))
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
BOOL Post_Refresh_One_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize)
{
	_MessageRefreshListInfo *pmy_refresh_info = new _MessageRefreshListInfo;
	pmy_refresh_info->deviceid = deviceid;
	pmy_refresh_info->command = command;
	pmy_refresh_info->start_instance = start_instance;
	pmy_refresh_info->end_instance = end_instance;
	pmy_refresh_info->entitysize = entitysize;
	if(!PostThreadMessage(nThreadID,MY_BAC_REFRESH_ONE,(WPARAM)pmy_refresh_info,NULL))//post thread msg
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
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
BOOL Post_Write_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize,HWND hWnd ,CString Task_Info ,int nRow,int nCol)
{
	_MessageWriteListInfo *pmy_write_info = new _MessageWriteListInfo;
	pmy_write_info->deviceid = deviceid;
	pmy_write_info->command = command;
	pmy_write_info->start_instance = start_instance;
	pmy_write_info->end_instance = end_instance;
	pmy_write_info->Write_Info = Task_Info;
	pmy_write_info->entitysize = entitysize;
	pmy_write_info->hWnd = hWnd;
	pmy_write_info->ItemInfo.nRow = nRow;
	pmy_write_info->ItemInfo.nCol = nCol;
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
	CString Show_Detail ,
	int nRow,
	int nCol
	)
{
	_MessageInvokeIDInfo *pMy_Invoke_id = new _MessageInvokeIDInfo;
	pMy_Invoke_id->Invoke_ID = Invoke_ID;
	pMy_Invoke_id->hwnd = hwnd;
	pMy_Invoke_id->task_info = Show_Detail;
	pMy_Invoke_id->mRow = nRow;
	pMy_Invoke_id->mCol = nCol;
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
	for (int i=0;i<(int)Change_Color_ID.size();i++)
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
		entitysize = program_code_length[start_instance];
		
		//m_Program_data.at(program_list_line).bytes = my_lengthcode -7;
		//entitysize = my_lengthcode;
		if((entitysize<0)||(entitysize>400))
			entitysize = 0;
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
	case WRITECONTROLLER_T3000:
		entitysize = sizeof(Str_controller_point);
		break;
	case WRITESCREEN_T3000:
		entitysize = sizeof(Control_group_point);
		break;
	case WRITEMONITOR_T3000:
		entitysize = sizeof(Str_monitor_point);
		break;
	default:
		{
			//AfxMessageBox(_T("Entitysize length error!"));
			TRACE(_T("Entitysize length error!"));
			break;
		}
		
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
 		//memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,entitysize,mycode,my_lengthcode);
		memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,entitysize,program_code[start_instance],entitysize);
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

		}
		break;
	case WRITECONTROLLER_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_controller_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_controller_point),&m_controller_data.at(i + start_instance),sizeof(Str_controller_point));
		}
		break;
	case WRITESCREEN_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Control_group_point) + PRIVATE_HEAD_LENGTH,sizeof(Control_group_point),&m_screen_data.at(i + start_instance),sizeof(Control_group_point));
		}
		break;
	case WRITEMONITOR_T3000:
		for (int i=0;i<(end_instance-start_instance + 1);i++)
		{
			memcpy_s(SendBuffer + i*sizeof(Str_monitor_point) + PRIVATE_HEAD_LENGTH,sizeof(Str_monitor_point),&m_monitor_data.at(i + start_instance),sizeof(Str_monitor_point));
		}
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
	//if (1) 
	{
		
		return Send_ConfirmedPrivateTransfer(&dest,&private_data);
		//return g_invoke_id;
	}
	else
		return -2;

}



/************************************************************************/
/*
Author: Fance Du
Get Bacnet Monitor Private Data
<param name="deviceid">Bacnet Device ID
<param name="command">Bacnet command
<param name="index"> read which the item of the monitor list
<param name="nspecial">if the first read, the special should be zero ,otherwise it must be 1;
<param name="MonitorUpdateData"> this structure means the data size ,data start time and end time;
<param name="ntype" > Analog data or digital data
*/
/************************************************************************/
int GetMonitorBlockData(uint32_t deviceid,int8_t command,int8_t index,int8_t ntype,int8_t nspecial,MonitorUpdateData* up_data)
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

	Str_Monitor_data_header private_data_chunk;
	private_data_chunk.total_length = 0;
	private_data_chunk.command = command;
	private_data_chunk.index = index;
	private_data_chunk.conm_args.nsize = up_data->nsize;
	private_data_chunk.conm_args.oldest_time = up_data->oldest_time;
	private_data_chunk.conm_args.most_recent_time = up_data->most_recent_time;
	private_data_chunk.type = ntype;
	private_data_chunk.special = nspecial;
	Set_transfer_length(PRIVATE_MONITOR_HEAD_LENGTH);


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
//	uint32_t uiErrorCode;
//	char cBlockNumber;
//	uint32_t ulTemp;
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
				if(strlen(my_temp_point)>STR_OUT_DESCRIPTION_LENGTH)
					memset(m_Output_data.at(i).description,0,STR_OUT_DESCRIPTION_LENGTH);
				else
				memcpy_s( m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH,my_temp_point,STR_OUT_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_OUT_DESCRIPTION_LENGTH;
				if(strlen(my_temp_point)>STR_OUT_LABEL)
					memset(m_Output_data.at(i).label,0,STR_OUT_LABEL);
				else
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
				if(strlen(my_temp_point) > STR_IN_DESCRIPTION_LENGTH)
					memset(m_Input_data.at(i).description,0,STR_IN_DESCRIPTION_LENGTH);
				else
					memcpy_s( m_Input_data.at(i).description,STR_IN_DESCRIPTION_LENGTH,my_temp_point,STR_IN_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_IN_DESCRIPTION_LENGTH;
				if(strlen(my_temp_point) > STR_IN_LABEL)
					memset(m_Input_data.at(i).label,0,STR_IN_LABEL);
				else
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
				if(strlen(my_temp_point) > STR_VARIABLE_DESCRIPTION_LENGTH)
					memset(m_Variable_data.at(i).description,0,STR_VARIABLE_DESCRIPTION_LENGTH);
				else
					memcpy_s( m_Variable_data.at(i).description,STR_VARIABLE_DESCRIPTION_LENGTH,my_temp_point,STR_VARIABLE_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_VARIABLE_DESCRIPTION_LENGTH;
				if(strlen(my_temp_point) > STR_VARIABLE_LABEL)
					memset(m_Variable_data.at(i).label,0,STR_VARIABLE_LABEL);
				else
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
		{
			int aaaa = sizeof(Str_weekly_routine_point);
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_weekly_routine_point))!=0)
				return -1;	
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
				if(strlen(my_temp_point) > STR_WEEKLY_DESCRIPTION_LENGTH)
					memset(m_Weekly_data.at(i).description,0,STR_WEEKLY_DESCRIPTION_LENGTH);
				else
					memcpy_s( m_Weekly_data.at(i).description,STR_WEEKLY_DESCRIPTION_LENGTH,my_temp_point,STR_WEEKLY_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_WEEKLY_DESCRIPTION_LENGTH;

				if(strlen(my_temp_point) > STR_WEEKLY_LABEL_LENGTH)
					memset(m_Weekly_data.at(i).label,0,STR_WEEKLY_LABEL_LENGTH);
				else
					memcpy_s( m_Weekly_data.at(i).label,STR_WEEKLY_LABEL_LENGTH ,my_temp_point,STR_WEEKLY_LABEL_LENGTH );
				my_temp_point=my_temp_point + STR_WEEKLY_LABEL_LENGTH ;


				m_Weekly_data.at(i).value = (unsigned char)(*(my_temp_point++));
				m_Weekly_data.at(i).auto_manual = (unsigned char)(*(my_temp_point++));
				m_Weekly_data.at(i).override_1_value =  (unsigned char)(*(my_temp_point++));
				m_Weekly_data.at(i).override_2_value =  (unsigned char)(*(my_temp_point++));
				m_Weekly_data.at(i).off =  (unsigned char)(*(my_temp_point++));
				m_Weekly_data.at(i).unused = (unsigned char)(*(my_temp_point++));

				my_temp_point = my_temp_point + 2*sizeof(Point_T3000);
			}
			return READWEEKLYROUTINE_T3000;
		}
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
			if(strlen(my_temp_point) > STR_ANNUAL_DESCRIPTION_LENGTH)
				memset(m_Annual_data.at(i).description,0,STR_ANNUAL_DESCRIPTION_LENGTH);
			else
				memcpy_s( m_Annual_data.at(i).description,STR_ANNUAL_DESCRIPTION_LENGTH,my_temp_point,STR_ANNUAL_DESCRIPTION_LENGTH);
			my_temp_point=my_temp_point + STR_ANNUAL_DESCRIPTION_LENGTH;

			if(strlen(my_temp_point) > STR_ANNUAL_DESCRIPTION_LENGTH)
				memset(m_Annual_data.at(i).label,0,STR_ANNUAL_DESCRIPTION_LENGTH);
			else
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
				if(strlen(my_temp_point) > STR_PROGRAM_DESCRIPTION_LENGTH)
					memset(m_Program_data.at(i).description,0,STR_PROGRAM_DESCRIPTION_LENGTH);
				else
					memcpy_s( m_Program_data.at(i).description,STR_PROGRAM_DESCRIPTION_LENGTH,my_temp_point,STR_PROGRAM_DESCRIPTION_LENGTH);
				my_temp_point=my_temp_point + STR_PROGRAM_DESCRIPTION_LENGTH;
				if(strlen(my_temp_point) > STR_PROGRAM_LABEL_LENGTH)
					memset(m_Program_data.at(i).label,0,STR_PROGRAM_LABEL_LENGTH);
				else
					memcpy_s( m_Program_data.at(i).label,STR_PROGRAM_LABEL_LENGTH ,my_temp_point,STR_PROGRAM_LABEL_LENGTH );
				my_temp_point=my_temp_point + STR_PROGRAM_LABEL_LENGTH ;
				if(i==14)
					Sleep(1);
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
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
		//	int code_length = (unsigned char)(my_temp_point[1]*256) + (unsigned char)my_temp_point[0];
			int code_length = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			memset(mycode,0,1024);
			if((code_length!=0) && (code_length <=400) && (code_length >0))
			{
				memcpy_s(mycode,code_length+ 10,my_temp_point,code_length+ 10);
				memcpy_s(program_code[start_instance],code_length + 10,my_temp_point,code_length + 10);
				program_code_length[start_instance] = code_length + 10;
			}
			else
			{
				memset(program_code[start_instance],0,400);
				program_code_length[start_instance] = 0;
			}

			
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
	case READSCREEN_T3000:
		if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Control_group_point))!=0)
			return -1;	
		block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Control_group_point);

		my_temp_point = (char *)Temp_CS.value + 3;
		start_instance = *my_temp_point;
		my_temp_point++;
		end_instance = *my_temp_point;
		my_temp_point++;
		my_temp_point = my_temp_point + 2;

		for (i=start_instance;i<=end_instance;i++)
		{
			if(strlen(my_temp_point) > STR_SCREEN_DESCRIPTION_LENGTH)
				memset(m_screen_data.at(i).description,0,STR_SCREEN_DESCRIPTION_LENGTH);
			else
				memcpy_s( m_screen_data.at(i).description,STR_SCREEN_DESCRIPTION_LENGTH,my_temp_point,STR_SCREEN_DESCRIPTION_LENGTH);
			my_temp_point=my_temp_point + STR_SCREEN_DESCRIPTION_LENGTH;

			if(strlen(my_temp_point) > STR_SCREEN_LABLE_LENGTH)
				memset(m_screen_data.at(i).label,0,STR_SCREEN_LABLE_LENGTH);
			else
				memcpy_s( m_screen_data.at(i).label,STR_SCREEN_LABLE_LENGTH ,my_temp_point,STR_SCREEN_LABLE_LENGTH );
			my_temp_point=my_temp_point + STR_SCREEN_LABLE_LENGTH ;

			if(strlen(my_temp_point) > STR_SCREEN_PIC_FILE_LENGTH)
				memset(m_screen_data.at(i).picture_file,0,STR_SCREEN_PIC_FILE_LENGTH);
			else
				memcpy_s( m_screen_data.at(i).picture_file,STR_SCREEN_PIC_FILE_LENGTH ,my_temp_point,STR_SCREEN_PIC_FILE_LENGTH );
			my_temp_point=my_temp_point + STR_SCREEN_PIC_FILE_LENGTH ;

			m_screen_data.at(i).update = *(my_temp_point++);
			m_screen_data.at(i).mode = *(my_temp_point++);
			m_screen_data.at(i).xcur_grp = *(my_temp_point++);
			unsigned short temp_ycur_grp;
			temp_ycur_grp = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			m_screen_data.at(i).ycur_grp = temp_ycur_grp;
			my_temp_point = my_temp_point + 2;
		}
		return READSCREEN_T3000;
		break;
	case READMONITOR_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_monitor_point))!=0)
				return -1;	
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_monitor_point);

			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			for (i=start_instance;i<=end_instance;i++)
			{
				if(strlen(my_temp_point) > STR_MONITOR_LABEL_LENGTH)
					memset(m_monitor_data.at(i).label,0,STR_MONITOR_LABEL_LENGTH);
				else
					memcpy_s( m_monitor_data.at(i).label,STR_MONITOR_LABEL_LENGTH,my_temp_point,STR_MONITOR_LABEL_LENGTH);
				my_temp_point=my_temp_point + STR_MONITOR_LABEL_LENGTH;

				for (int j=0;j<MAX_POINTS_IN_MONITOR;j++)
				{
					m_monitor_data.at(i).inputs[j].number = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].point_type = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].panel = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].network = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
					my_temp_point = my_temp_point + 2;
				}
				for (int k=0;k<MAX_POINTS_IN_MONITOR;k++)
				{
					m_monitor_data.at(i).range[k] = *(my_temp_point++);
				}
				m_monitor_data.at(i).second_interval_time = *(my_temp_point++);
				m_monitor_data.at(i).minute_interval_time = *(my_temp_point++);
				m_monitor_data.at(i).hour_interval_time   = *(my_temp_point++);
				m_monitor_data.at(i).max_time_length = *(my_temp_point++);
				m_monitor_data.at(i).num_inputs = *(my_temp_point++);
				m_monitor_data.at(i).an_inputs = *(my_temp_point++);
				m_monitor_data.at(i).unit = *(my_temp_point++);
				m_monitor_data.at(i).wrap_flag = *(my_temp_point++);
				m_monitor_data.at(i).status= *(my_temp_point++);
				m_monitor_data.at(i).reset_flag= *(my_temp_point++);
				m_monitor_data.at(i).double_flag= *(my_temp_point++);
			}
		}
		return READMONITOR_T3000;
		break;
	case READMONITORDATA_T3000:
	

		my_temp_point = (char *)Temp_CS.value;
		my_temp_point = my_temp_point + PRIVATE_MONITOR_HEAD_LENGTH;
		my_temp_point = my_temp_point + MAX_POINTS_IN_MONITOR * 5 ;
		m_monitor_block.monitor =  *(my_temp_point++);
		m_monitor_block.no_points =  *(my_temp_point++);
		m_monitor_block.second_interval_time =  *(my_temp_point++);
		m_monitor_block.minute_interval_time =  *(my_temp_point++);
		m_monitor_block.hour_interval_time =  *(my_temp_point++);
		m_monitor_block.priority =  *(my_temp_point++);
		m_monitor_block.first_block =  *(my_temp_point++);
		m_monitor_block.last_block =  *(my_temp_point++);
		m_monitor_block.analog_digital =  *(my_temp_point++);
		m_monitor_block.block_state =  *(my_temp_point++);
		m_monitor_block.fast_sampling =  *(my_temp_point++);
		m_monitor_block.wrap_around =  *(my_temp_point++);
		
		temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
		m_monitor_block.start_time =temp_struct_value; 
		my_temp_point = my_temp_point + 4;

		temp_struct_value = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
		m_monitor_block.index =temp_struct_value; 
		my_temp_point = my_temp_point + 2;

		m_monitor_block.next_block =  *(my_temp_point++);
		m_monitor_block.block_no =  *(my_temp_point++);
		m_monitor_block.last_digital_state =  *(my_temp_point++);
		m_monitor_block.not_used =  *(my_temp_point++);


		char *data_temp_point;
		data_temp_point = my_temp_point;
		if(m_monitor_block.analog_digital == 0)
		{		
			int data_index_check = (len_value_type -PRIVATE_MONITOR_HEAD_LENGTH - (MAX_POINTS_IN_MONITOR * 5) - 22)/4;
			if(m_monitor_block.index != data_index_check)
			{
				//return 1;
			}
			int data_group = 0;
			if((m_monitor_block.index % m_monitor_data.at(monitor_list_line).an_inputs) == 0)
			{
				data_group = m_monitor_block.index / m_monitor_data.at(monitor_list_line).an_inputs;
			}
			else
			{
				return 1;
			}
			
			for (int j=0;j<data_group;j++)
			{
				for (int k=0;k< MAX_POINTS_IN_MONITOR; k++)
				{
					if(my_input_info[k].be_record)
					{
						temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
						Monitor_Input__Data[k][j] = temp_struct_value;
						my_temp_point = my_temp_point + 4;
					}
					else
					{
						continue;
					}
				}
				
			}

#if 0	
			for (int i=0;i<m_monitor_data.at(monitor_list_line).an_inputs;i++)
			{
				for (int j=0;j<m_monitor_block.index;j++)
				{

					Monitor_Input__Data[i][j] = 


				}
			}
#endif
			//memcpy_s(m_monitor_block.dat.analog,20,my_temp_point,20);
		}
		else if(m_monitor_block.analog_digital == 1)
		{

		}
		else
		{
			Sleep(1);
		}

		break;
	case GETSERIALNUMBERINFO:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_Serial_info))!=0)
				return -1;

			_Bac_Scan_results_Info temp_struct;


			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			 temp_struct.device_id	= ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
			 my_temp_point = my_temp_point +2;
			 memcpy_s(temp_struct.ipaddress,6,my_temp_point,6);
			//  temp_struct.panel_number = *(my_temp_point + 3);//Notice
			// temp_struct.macaddress = *my_temp_point;
			 my_temp_point = my_temp_point + 6;
			 temp_struct.serialnumber = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			 my_temp_point = my_temp_point + 4;
			 temp_struct.modbus_addr =  *(my_temp_point++);
			 temp_struct.product_type =  *(my_temp_point++);
			// my_temp_point++;
			  temp_struct.panel_number =  *(my_temp_point++);
			 int find_exsit = false;
			 TRACE(_T("serialnumber = %d ,modbus_addr = %d , product_type = %d ,ip = %u.%u.%u.%u , instance = %d\r\n"),temp_struct.serialnumber,
				 temp_struct.modbus_addr,temp_struct.product_type,temp_struct.ipaddress[0],temp_struct.ipaddress[1] ,
				 temp_struct.ipaddress[2],temp_struct.ipaddress[3],temp_struct.device_id);
			 for (int x=0;x<(int)m_bac_scan_result_data.size();x++)
			 {
				 if(temp_struct.serialnumber == m_bac_scan_result_data.at(x).serialnumber)
					 find_exsit = true;
			 }
			 if(!find_exsit)
			 {
				 m_bac_scan_result_data.push_back(temp_struct);
				 //CTStat_Dev* pTemp = new CTStat_Dev;			
				 //_ComDeviceInfo* pInfo = new _ComDeviceInfo;
				 //pInfo->m_pDev = pTemp;

				 //pTemp->SetSerialID(temp_struct.serialnumber);
				 //pTemp->SetDevID(temp_struct.modbus_addr);
				 //pTemp->SetProductType(temp_struct.product_type);
			 }
		}
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
		copy_data_to_ptrpanel(TYPE_ALL);
		break;
	case READSCREEN_T3000:
		copy_data_to_ptrpanel(TYPE_ALL);
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


CString GetProductName(int ModelID)
{
	CString strProductName;
	switch(ModelID)
	{
	case PM_TSTAT5A:
		strProductName="TStat5A";
		break;
	case PM_TSTAT5B:
		strProductName="TStat5B";
		break;
	case PM_TSTAT5B2:
		strProductName="TStat5B2";
		break;
	case PM_TSTAT5C:
		strProductName="TStat5C";
		break;
	case PM_TSTAT5D:
		strProductName="TStat5D";
		break;
	case PM_TSTAT5E:
		strProductName="TStat5E";
		break;
	case PM_TSTAT5F:
		strProductName="TStat5F";
		break;
	case PM_TSTAT5G:
		strProductName="TStat5G";
		break;
	case PM_TSTAT5H:
		strProductName="TStat5H";
		break;
	case PM_TSTAT6:
		strProductName="TStat6";
		break;
	case PM_TSTAT7:
		strProductName="TStat7";
		break;
	case PM_NC:
		strProductName="NC";
		break;
	case PM_CM5:
		strProductName ="CM5";
		break;
		//20120424
	case PM_LightingController:
		strProductName = "LC";
		break;
	case  PM_CO2_NET:
		strProductName = "CO2 Net";
		break;
	case  PM_CO2_RS485:
		strProductName = "CO2";
		break;
	case PM_TSTAT6_HUM_Chamber:
		strProductName =g_strHumChamber;
		break;

	case PM_T38AIOD :
		strProductName="T3-8I13O";
		break;
	case PM_T3IOA :
		strProductName="T3-8IOA";
		break;
	case PM_T332AI :
		strProductName="T3-32AI";
		break;
	case PM_T3AI16O :
		strProductName="T3-8AI160";
		break;
	case PM_T38I13O :
		strProductName="T3-8I13O";
		break;
	case PM_T3PERFORMANCE :
		strProductName="T3-Performance";
		break;
	case PM_T34AO :
		strProductName="T3-4AO";
		break;
	case PM_T36CT :
		strProductName="T3-6CT";
		break;
	default:
		strProductName="TStat";
		break;
	}
	return strProductName;
}


CString Get_Table_Name(int SerialNo,CString Type ,int Row){
	CADO ado;
	CString Table_Name;
	ado.OnInitADOConn();
	if (ado.IsHaveTable(ado,_T("IONAME_CONFIG")))//有Version表
	{
		CString sql;
		sql.Format(_T("Select * from IONAME_CONFIG where Type='%s' and  Row=%d and SerialNo=%d"),Type.GetBuffer(),Row,SerialNo);
		ado.m_pRecordset=ado.OpenRecordset(sql);
		if (!ado.m_pRecordset->EndOfFile)//有表但是没有对应序列号的值
		{
			ado.m_pRecordset->MoveFirst();
			while (!ado.m_pRecordset->EndOfFile)
			{
				Table_Name=ado.m_pRecordset->GetCollect(_T("InOutName"));
				ado.m_pRecordset->MoveNext();
			}
		}
		else
		{
		  Table_Name.Format(_T("%s%d"),Type.GetBuffer(),Row);
		}
	}
	else
	{
	    Table_Name.Format(_T("%s%d"),Type.GetBuffer(),Row);
	}
	ado.CloseRecordset();
	ado.CloseConn();
	return Table_Name;
}
void    Insert_Update_Table_Name(int SerialNo,CString Type,int Row,CString TableName){
	CADO ado;
	ado.OnInitADOConn();
	CString sql;
	sql.Format(_T("Select * from IONAME_CONFIG where Type='%s' and  Row=%d and SerialNo=%d"),Type.GetBuffer(),Row,SerialNo);
	ado.m_pRecordset=ado.OpenRecordset(sql);

	if (!ado.m_pRecordset->EndOfFile)//有表但是没有对应序列号的值
	{

		sql.Format(_T("update IONAME_CONFIG set InOutName = '%s' where Type='%s' and  Row=%d and SerialNo=%d "),TableName.GetBuffer(),Type.GetBuffer(),Row,SerialNo);
		ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
	}
	else
	{
			ado.CloseRecordset();
		sql.Format(_T("Insert into IONAME_CONFIG(InOutName,Type,Row,SerialNo) values('%s','%s','%d','%d')"),TableName.GetBuffer(),Type.GetBuffer(),Row,SerialNo);
		ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
	}

	ado.CloseConn();
}

int Get_Unit_Process(CString Unit){
int ret_Value=1;
if (Unit.CompareNoCase(_T("RAW DATA"))==0)
{
ret_Value=1;
}
else if (Unit.CompareNoCase(_T("TYPE2 10K C"))==0)
{ret_Value=10;
}
else if (Unit.CompareNoCase(_T("TYPE2 10K F"))==0)
{
ret_Value=10;
}
else if (Unit.CompareNoCase(_T("0-100%"))==0)
{
ret_Value=1;
}
else if (Unit.CompareNoCase(_T("ON/OFF"))==0)
{
ret_Value=1;
}

else if (Unit.CompareNoCase(_T("OFF/ON"))==0)
{
ret_Value=1;
}
else if (Unit.CompareNoCase(_T("Pulse Input"))==0)
{
ret_Value=1;
}
else if (Unit.CompareNoCase(_T("Lighting Control"))==0)
{
ret_Value=1;
}
else if (Unit.CompareNoCase(_T("TYPE3 10K C"))==0)
{
ret_Value=10;
}
else if (Unit.CompareNoCase(_T("TYPE3 10K F"))==0)
{
ret_Value=10;
}
else if (Unit.CompareNoCase(_T("NO USE"))==0)
{
ret_Value=1;
}
else if (Unit.CompareNoCase(_T("0-5V"))==0)
{
ret_Value=1000;
}
else if (Unit.CompareNoCase(_T("0-10V"))==0)
{
ret_Value=1000;
}
else if (Unit.CompareNoCase(_T("0-20ma"))==0)
{
ret_Value=1000;
}



return ret_Value;
}


BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position){
 
int postionvalue=1;
  postionvalue=postionvalue<<(Position-1);
  postionvalue= RegisterValue&postionvalue;
  BOOL ret=postionvalue>>(Position-1);
  return ret;
}



char * intervaltotext(char *textbuf, long seconds , unsigned minutes , unsigned hours, char *c)
{
	char buf[12], *textbuffer;
	char *separator = c ;
	textbuffer = buf;
	if( seconds < 0 )
	{
		seconds = -seconds;
		strcpy(textbuffer++, "-" ) ;        /* add the '-' */
	}
	if(*c!='-')
	{
		hours += seconds/3600;
		minutes += (unsigned)(seconds%3600)/60;
		seconds = (unsigned)(seconds%3600)%60;
	}
	if( hours < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
	}
	itoa(hours,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/

	if( minutes < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:0#:## */
	}
	itoa(minutes,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	//strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/
	//if( seconds < 10 ) {
	//	strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:##:0# */
	//}
	//itoa(seconds,textbuffer,10)  ;

	if(textbuf) strcpy(textbuf, buf);
	//return( buf ) ;
	return NULL;
}

char * intervaltotextfull(char *textbuf, long seconds , unsigned minutes , unsigned hours, char *c)
{
	char buf[12], *textbuffer;
	char *separator = c ;
	textbuffer = buf;
	if( seconds < 0 )
	{
		seconds = -seconds;
		strcpy(textbuffer++, "-" ) ;        /* add the '-' */
	}
	if(*c!='-')
	{
		hours += seconds/3600;
		minutes += (unsigned)(seconds%3600)/60;
		seconds = (unsigned)(seconds%3600)%60;
	}
	if( hours < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
	}
	itoa(hours,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/

	if( minutes < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:0#:## */
	}
	itoa(minutes,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/
	if( seconds < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:##:0# */
	}
	itoa(seconds,textbuffer,10)  ;

	if(textbuf) strcpy(textbuf, buf);
	return( buf ) ;
}

void LocalIAmHandler(	uint8_t * service_request,	uint16_t service_len,	BACNET_ADDRESS * src)
{

	int len = 0;
	uint32_t device_id = 0;
	unsigned max_apdu = 0;
	int segmentation = 0;
	uint16_t vendor_id = 0;

	(void) src;
	(void) service_len;
	len =  iam_decode_service_request(service_request, &device_id, &max_apdu,
		&segmentation, &vendor_id);



#if 0
	fprintf(stderr, "Received I-Am Request");
	if (len != -1) 
	{
		fprintf(stderr, " from %u!\n", device_id);
		address_add(device_id, max_apdu, src);
	} else
		fprintf(stderr, "!\n");
#endif
	address_add(device_id, max_apdu, src);



	//g_bac_instance =device_id;
#if 1
	if(src->mac_len==6)
		bac_cs_mac.Format(_T("%d"),src->mac[3]);
	else
		return;
#endif
//	bac_cs_mac.Format(_T("%d"),vendor_id);

	bac_cs_device_id.Format(_T("%d"),device_id);


//	bac_cs_vendor_id.Format(_T("%d"),vendor_id);
	//g_bac_instance =device_id;
	//g_mac = _wtoi(bac_cs_mac);

	TRACE(_T("Find ") + bac_cs_device_id +_T("  ") + bac_cs_mac + _T("\r\n"));

	_Bac_Scan_Com_Info temp_1;
	temp_1.device_id = device_id;
//	temp_1.vendor_id = vendor_id;
	temp_1.macaddress = _wtoi(bac_cs_mac);

	int find_exsit = false;
	for (int i=0;i<(int)m_bac_scan_com_data.size();i++)
	{
		if((m_bac_scan_com_data.at(i).device_id == temp_1.device_id)
			|| (m_bac_scan_com_data.at(i).macaddress == temp_1.macaddress))
			find_exsit = true;
	}
	if(!find_exsit)
	{
		m_bac_scan_com_data.push_back(temp_1);
#if 0
		CString Program_Path,Program_ConfigFile_Path;
		int g_com=0;
		GetModuleFileName(NULL,Program_Path.GetBuffer(MAX_PATH),MAX_PATH);  
		PathRemoveFileSpec(Program_Path.GetBuffer(MAX_PATH) );            
		Program_Path.ReleaseBuffer();
		Program_ConfigFile_Path = Program_Path + _T("\\BacnetDB.ini");

		CString Section;
		Section.Format(_T("Device_%d"),temp_1.device_id);

		CString temp_csc;
		CString temp_code;
		unsigned char src_data[20];
		memcpy_s(src_data,18,src,18);
		unsigned char * temp_point = NULL;
		temp_point = src_data;
		for (int i=0;i<18;i++)
		{
			temp_csc.Format(_T("%02x"),*(temp_point + i));
			temp_csc.MakeUpper();
			temp_code = temp_code + temp_csc;
		}
		CString temp_apdu;
		temp_apdu.Format(_T("%d"),max_apdu);
		WritePrivateProfileStringW(Section,_T("SRC"),temp_code,Program_ConfigFile_Path);
		WritePrivateProfileStringW(Section,_T("MAX_APDU"),temp_apdu,Program_ConfigFile_Path);
#endif
	}

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	return;

}
SOCKET my_sokect;
extern void  init_info_table( void );
extern void Init_table_bank();
void Initial_bac_com(int comport)
{

	BACNET_ADDRESS src = {
		0
	};  /* address where message came from */
	uint16_t pdu_len = 0;
	unsigned timeout = 100;     /* milliseconds */
	BACNET_ADDRESS my_address, broadcast_address;
//	char my_port[50];

	//if(bac_net_initial_once)
	//{
	//	BacNet_hwd = this->m_hWnd;
	//}

	if(!bac_net_initial_once)
	{
		bac_net_initial_once =true;
		bac_program_pool_size = 26624;
		bac_program_size = 0;
		bac_free_memory = 26624;
		//hwait_thread = NULL;
		HANDLE temphandle;
		temphandle = Get_RS485_Handle();
		if(temphandle !=NULL)
		{
			TerminateThread((HANDLE)Get_Thread1(),0);
			TerminateThread((HANDLE)Get_Thread2(),0);

			CloseHandle(temphandle);
			Set_RS485_Handle(NULL);
		}

		//SetCommunicationType(0);	
		//close_com();
		//SetCommunicationType(1);
		//close_com();
		Device_Set_Object_Instance_Number(4194300);
		address_init();
		Init_Service_Handlers();

		SetCommunicationType(1);
		close_com();
		//Open_Socket2(_T("192.168.0.177"),6001);
		
		//Open_bacnetSocket2(_T("127.0.0.1"),6002,my_sokect);
		//int ret_1 = Open_bacnetSocket2(_T("192.168.0.177"),47808,my_sokect);
		int ret_1 = Open_bacnetSocket2(_T("192.168.0.130"),47808,my_sokect);
	//	Open_Socket2(_T("127.0.0.1"),6002);
	//	 = (int)GetCommunicationHandle();
		bip_set_socket(my_sokect);
		bip_set_port(49338);
	//	int test_port = bip_get_port();
#if 1
		static in_addr BIP_Broadcast_Address;
		BIP_Broadcast_Address.S_un.S_addr =  inet_addr("255.255.255.255");
		//BIP_Broadcast_Address.S_un.S_addr =  inet_addr("192.168.0.177");
		bip_set_broadcast_addr((uint32_t)BIP_Broadcast_Address.S_un.S_addr);
#endif


#if 1
		static in_addr BIP_Address;
		BIP_Address.S_un.S_addr =  inet_addr("192.168.0.158");
		bip_set_addr((uint32_t)BIP_Address.S_un.S_addr);
#endif	
#if 0
		dlmstp_set_baud_rate(19200);
		dlmstp_set_mac_address(0);
		dlmstp_set_max_info_frames(DEFAULT_MAX_INFO_FRAMES);
		dlmstp_set_max_master(DEFAULT_MAX_MASTER);
		memset(my_port,0,50);

		//CString Program_Path,Program_ConfigFile_Path;
		//int g_com=0;
		//GetModuleFileName(NULL,Program_Path.GetBuffer(MAX_PATH),MAX_PATH);  
		//PathRemoveFileSpec(Program_Path.GetBuffer(MAX_PATH) );           
		//Program_Path.ReleaseBuffer();
		//Program_ConfigFile_Path = Program_Path + _T("\\MyConfig.ini");

		/*CFileFind fFind;
		if(!fFind.FindFile(Program_ConfigFile_Path))
		{
			WritePrivateProfileStringW(_T("Setting"),_T("ComPort"),_T("1"),Program_ConfigFile_Path);
		}
		g_com = GetPrivateProfileInt(_T("Setting"),_T("ComPort"),1,Program_ConfigFile_Path);*/
		CString temp_cs;
		//temp_cs.Format(_T("COM%d"),g_com);
		temp_cs.Format(_T("COM%d"),comport);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		temp_cs.ReleaseBuffer();


		sprintf(my_port,cTemp1);
		dlmstp_init(my_port);
#endif

		//Initial_List();
		//		BacNet_hwd = this->m_hWnd;
		//    dlenv_init();
		datalink_get_broadcast_address(&broadcast_address);
		//    print_address("Broadcast", &broadcast_address);
		datalink_get_my_address(&my_address);
		//		print_address("Address", &my_address);

		CM5_hThread =CreateThread(NULL,NULL,MSTP_Receive,NULL,NULL, &nThreadID_x);

		//CM5_UI_Thread = CreateThread(NULL,NULL,CM5_UI,this,NULL, &cm5_nThreadID);

		//Send_WhoIs_Global(g_bac_instance, g_bac_instance);


		//m_bac_static_status.SetWindowTextW(_T("Disconnected"));
		//m_bac_static_status.textColor(RGB(255,255,255));
		//m_bac_static_status.bkColor(RGB(255,0,0));
		//m_bac_static_status.setFont(26,18,NULL,_T("Cambria"));
		timesec1970 = (unsigned long)time(NULL);
		timestart = 0;
		init_info_table();
		Init_table_bank();
	}
}

DWORD WINAPI   MSTP_Receive(LPVOID lpVoid)
{
	BACNET_ADDRESS src = {0};
	uint16_t pdu_len;
	CDialogCM5_BacNet *mparent = (CDialogCM5_BacNet *)lpVoid;
	uint8_t Rx_Buf[MAX_MPDU] = { 0 };
	//while(mparent->m_MSTP_THREAD)
	while(1)
	{
		pdu_len =  bip_receive(&src,&Rx_Buf[0],MAX_MPDU, INFINITE);
		//pdu_len = dlmstp_receive(&src, &Rx_Buf[0], MAX_MPDU, INFINITE);
		if(pdu_len==0)
			continue;
		npdu_handler(&src, &Rx_Buf[0], pdu_len);
		//CString TEMP1;
		//TEMP1.Format("%s",Rx_Buf);
		//	AfxMessageBox(TEMP1);
	}
	return 0;
}

void Init_Service_Handlers(	void)
{
	Device_Init(NULL);

	/* we need to handle who-is to support dynamic device binding */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, LocalIAmHandler);



	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_PRIVATE_TRANSFER,local_handler_conf_private_trans_ack);
	//apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,Read_Property_feed_back);

	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,	Localhandler_read_property_ack);
	/* set the handler for all the services we don't implement */
	/* It is required to send the proper reject message... */
	apdu_set_unrecognized_service_handler_handler
		(handler_unrecognized_service);
	/* we must implement read property - it's required! */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
		handler_read_property);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
		handler_read_property_multiple);
	/* handle the data coming back from confirmed requests */
	//   apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,handler_read_property_ack);
#if defined(BACFILE)
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
		handler_atomic_read_file);
#endif
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
		handler_cov_subscribe);

	////#if 0
	////	/* Adding these handlers require the project(s) to change. */
	////#if defined(BACFILE)
	////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_WRITE_FILE,
	////		handler_atomic_write_file);
	////#endif
	////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_RANGE,
	////		handler_read_range);
	////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
	////		handler_reinitialize_device);
	////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
	////		handler_timesync_utc);
	////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
	////		handler_timesync);
	////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
	////		handler_ucov_notification);
	////	/* handle communication so we can shutup when asked */
	////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
	////		handler_device_communication_control);
	////#endif
}

void Localhandler_read_property_ack(
	uint8_t * service_request,
	uint16_t service_len,
	BACNET_ADDRESS * src,
	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
	int len = 0;
	BACNET_READ_PROPERTY_DATA data;

	(void) src;
	(void) service_data;        /* we could use these... */
	len = rp_ack_decode_service_request(service_request, service_len, &data);
#if 0
	fprintf(stderr, "Received Read-Property Ack!\n");
#endif
	if (len > 0)
	{
	//	local_rp_ack_print_data(&data);
	//	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	}
}


//This function add by Fance Du, used for changed the CString to hex
//2013 12 02
//Ex: "0F" -> 15
unsigned char Str_to_Byte(CString need_conver)
{
	int the_first=0;
	int the_second=0;
	switch (need_conver.GetAt(0))
	{
	case 0x30:	the_first=0;		break;
	case 0x31:	the_first=1;		break;
	case 0x32:	the_first=2;		break;
	case 0x33:	the_first=3;		break;
	case 0x34:	the_first=4;		break;
	case 0x35:	the_first=5;		break;
	case 0x36:	the_first=6;		break;
	case 0x37:  the_first=7;		break;
	case 0x38:  the_first=8;		break;
	case 0x39:  the_first=9;		break;
	case 0x41:  the_first=10;		break;
	case 0x42:  the_first=11;		break;
	case 0x43:  the_first=12;		break;
	case 0x44:  the_first=13;		break;
	case 0x45:  the_first=14;		break;
	case 0x46:  the_first=15;		break;
	}
	switch (need_conver.GetAt(1))
	{
	case 0x30:	the_second=0;		break;
	case 0x31:	the_second=1;		break;
	case 0x32:	the_second=2;		break;
	case 0x33:	the_second=3;		break;
	case 0x34:	the_second=4;		break;
	case 0x35:	the_second=5;		break;
	case 0x36:	the_second=6;		break;
	case 0x37:  the_second=7;		break;
	case 0x38:  the_second=8;		break;
	case 0x39:  the_second=9;		break;
	case 0x41:  the_second=10;		break;
	case 0x42:  the_second=11;		break;
	case 0x43:  the_second=12;		break;
	case 0x44:  the_second=13;		break;
	case 0x45:  the_second=14;		break;
	case 0x46:  the_second=15;		break;
	}
	return (the_first*16+the_second);
}

//socket dll.
bool Open_bacnetSocket2(CString strIPAdress,short nPort,SOCKET &mysocket)
{

	int nNetTimeout=3000;//1 second.
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);

	//if (m_hSocket!=INVALID_SOCKET)
	//{
	//	::closesocket(m_hSocket);
	//	m_hSocket=NULL;
	//}

	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		//AfxMessageBox(_T("Init Socket failed!"));
	//	m_hSocket=NULL;
		return FALSE;
	}

//	mysocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	mysocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(mysocket == INVALID_SOCKET)
	{
		//	AfxMessageBox(_T("Create socket failed!"));
		mysocket=NULL;
		return FALSE;
	}
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(nPort);
	servAddr.sin_addr.s_addr = INADDR_ANY; 

	USES_CONVERSION;   


	int bind_ret =	bind(mysocket, (struct sockaddr*)&servAddr, sizeof(servAddr));
	//if(bind_ret<0)
	//{
	//	//AfxMessageBox(_T("Locol port 47808 is not valiable"));

	//}


	//char pTemp[20];
	//pTemp=W2A(strIPAdress);     


	//servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
	//	servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
	servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
	//	u_long ul=1;
	//	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);

	setsockopt(mysocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));

	setsockopt(mysocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));


	BOOL bBroadcast=TRUE;
	setsockopt(mysocket,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));


	//char ABC[10];
	//ABC[0]=0X11;
	//ABC[1]=0X22;
	//sendto(mysocket,ABC,2,NULL,(struct sockaddr *) &servAddr,sizeof(sockaddr));
	//if(::connect(mysocket,(sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	//{
	//	DWORD dwErr = WSAGetLastError();
	//	//AfxMessageBox(_T(" Failed connect() \n"));
	//	::closesocket(mysocket);
	//	mysocket=NULL;
	//	return FALSE;
	//}
	return TRUE;
}
