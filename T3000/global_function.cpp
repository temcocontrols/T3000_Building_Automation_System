
#include "stdafx.h"
#include "../ISP/MyPing.h"
#include "global_function.h"
#include "Windows.h"
#include "T3000.h"
 


#include "T3000RegAddress.h"
#include "global_define.h"
#include "DialogCM5_BacNet.h"
#include "CM5\MyOwnListCtrl.h"
#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetProgram.h"
#include "BacnetVariable.h"
#include "global_function.h"


#include "global_define.h"
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
#include "BacnetAlarmLog.h"
#include "BacnetSetting.h"
#include "BacnetUserlogin.h"
#include "BacnetTstat.h"
#include "BacnetRemotePoint.h"
#include "rs485.h"
#include "tlhelp32.h"
#include "ProductModel.h"
#define DELAY_TIME	 10	//MS
#define Modbus_Serial	0
#define	Modbus_TCP	1
#include "MainFrm.h"
#include "modbus_read_write.cpp"
#include "ptp.h"
#include "../SQLiteDriver/CppSQLite3.h"
//#include "CM5\PTP\ptp.h"
#pragma region For_Bacnet_program_Use

extern char mycode[2000];
extern int my_lengthcode;// the length of the edit of code
#pragma endregion


extern uint8_t globle_network_number ;
extern BacnetScreen *Screen_Window;
extern CBacnetProgram *Program_Window;
extern CBacnetInput *Input_Window ;
extern CBacnetOutput *Output_Window ;
extern CBacnetVariable *Variable_Window ;
extern BacnetWeeklyRoutine *WeeklyRoutine_Window ;
extern BacnetAnnualRoutine *AnnualRoutine_Window ;
extern BacnetController *Controller_Window ;
extern CBacnetMonitor *Monitor_Window ;
extern CBacnetAlarmLog *AlarmLog_Window ;
extern CBacnetTstat *Tstat_Window ;
extern CBacnetSetting * Setting_Window ;
extern CBacnetUserlogin* User_Login_Window ;
extern CBacnetRemotePoint* Remote_Point_Window ;
 
int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length)
{
    int retVal;
    retVal =  read_multi_tap(device_var, put_data_into_here, start_address, length);
    return retVal;
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

int read_one(unsigned char device_var,unsigned short address,int retry_times)
{
    CString g_strT3000LogString;
    int value;
    int j = modbus_read_one_value( value, device_var, address, retry_times );
    if (j>0)
    {
        g_strT3000LogString.Format(_T("Read One ID=%d,address=%d,result=%d,Status=OK"),device_var,address,j);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    else
    {
        g_strT3000LogString.Format(_T("Read One ID=%d,address=%d,Status:Fail"),device_var,address);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    //SetPaneString(3,g_strT3000LogString);
    if( j != 0 )
    {
        // there was an error, so return the error flag
        return j;
    }
    else
    {
        // no error, so return value read
        return value;
    }
}
//val         the value that you want to write to the register
//the return value == -1 ,no connecting
//the return value == -2 ,try it again
//the return value == -3,Maybe that have more than 2 Tstat is connecting
//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
//the return value == -5 ,the input have some trouble
//the return value == -6 , the bus has bannet protocol,scan stop;
//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
//清空串口缓冲区
//the return value is the register address
//Sleep(50);       //must use this function to slow computer
int g_CheckTstatOnline_a(unsigned char  devLo,unsigned char devHi, bool bComm_Type)
{

    BOOL EnableRefreshTreeView_original_value = g_bEnableRefreshTreeView;
    g_bEnableRefreshTreeView = false;
    int j=-1;
    // ensure no other threads attempt to access modbus communications
    CSingleLock singleLock(&register_critical_section);
    singleLock.Lock();
    // call the modbus DLL method
    j=CheckTstatOnline_a(devLo,devHi,bComm_Type);
    // free the modbus communications for other threads
    singleLock.Unlock();
    // increment the number of transmissions we have done
    g_llTxCount++;
    // check for other errors
    // increment the number or replies we have received
    if (j!=-1&&j!=-4)
    {
        g_llRxCount++;
    }
    // check for running in the main GUI thread
    if( AfxGetMainWnd()->GetActiveWindow() != NULL )
    {

        // construct status message string
        CString str;
        str.Format(_T("San Command [Tx=%d Rx=%d Err=%d]"),
                   g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

        //Display it
        ((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

    }
    g_bEnableRefreshTreeView |= EnableRefreshTreeView_original_value;
    return j;
}

int g_NetController_CheckTstatOnline_a(unsigned char  devLo,unsigned char devHi, bool bComm_Type)
{

    return -1;
    BOOL EnableRefreshTreeView_original_value = g_bEnableRefreshTreeView;
    g_bEnableRefreshTreeView = false;
    int j=-1;
    // ensure no other threads attempt to access modbus communications
    CSingleLock singleLock(&register_critical_section);
    singleLock.Lock();
    // call the modbus DLL method
    j=g_NetController_CheckTstatOnline_a(devLo,devHi,bComm_Type);
    // free the modbus communications for other threads
    singleLock.Unlock();
    // increment the number of transmissions we have done
    g_llTxCount++;
    // check for other errors
    // increment the number or replies we have received
    if (j!=-1&&j!=-4)
    {
        g_llRxCount++;
    }
    // check for running in the main GUI thread
    if( AfxGetMainWnd()->GetActiveWindow() != NULL )
    {

        // construct status message string
        CString str;
        str.Format(_T("San Command [Tx=%d Rx=%d Err=%d]"),
                   g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

        //Display it
        ((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

    }
    g_bEnableRefreshTreeView |= EnableRefreshTreeView_original_value;
    return j;
}

void SetPaneString(int nIndext,CString str)
{
    if(nIndext != BAC_SHOW_MISSION_RESULTS)
        return;
    if(str.IsEmpty())
        return;

    char * temp_cs = new char[255];

    WideCharToMultiByte( CP_ACP, 0, str.GetBuffer(), -1, temp_cs, 255, NULL, NULL );
    PostMessage(m_statusbar_hwnd,WM_SHOW_STATUS_TEXT,(WPARAM)temp_cs,NULL);

    return;
    CMFCStatusBar * pStatusBar=NULL;
    if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return
        return;
    pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
    pStatusBar->SetPaneText(nIndext,str.GetString());
    pStatusBar->SetPaneTextColor (nIndext, RGB(0,0,0));
    if (nIndext==3)
    {
        pStatusBar->SetPaneTextColor (nIndext, RGB(255,255,255));
        pStatusBar->SetPaneBackgroundColor(nIndext,RGB(42,58,87));
    }
}
int Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times)
{
    BOOL bTemp = g_bEnableRefreshTreeView;
    g_bEnableRefreshTreeView = FALSE;
    int j = Write_Multi_org(device_var, to_write, start_address, length, retry_times);
    g_bEnableRefreshTreeView |= bTemp;
    CString data;
    CString g_strT3000LogString;
    for (int i=0; i<length; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d "),to_write[i]);
        data+=strTemp;
    }
    if (j>0)
    {
        g_strT3000LogString.Format(_T("Multi Write ID=%d,start address=%d,length=%d"),device_var,start_address,length,data.GetBuffer());
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    else
    {
        g_strT3000LogString.Format(_T("Multi Write ID=%d,start address=%d,length=%d,Status:Fail"),device_var,start_address,length);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    // SetPaneString(3,g_strT3000LogString);
    return j;
}
int Write_Multi_short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,int retry_times)
{
    BOOL bTemp = g_bEnableRefreshTreeView;
    g_bEnableRefreshTreeView = FALSE;
    int j = Write_Multi_org_short(device_var, to_write, start_address, length, retry_times);
    g_bEnableRefreshTreeView |= bTemp;
    CString data;
    CString g_strT3000LogString;
    for (int i=0; i<length; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"),to_write[i]);
        data+=strTemp;
    }
    if (j>0)
    {
        g_strT3000LogString.Format(_T("Multi Write ID=%d,start address=%d,length=%d,result=%s"),device_var,start_address,length,data.GetBuffer());
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    else
    {
        g_strT3000LogString.Format(_T("Multi Write ID=%d,start address=%d,length=%d,result:Fail"),device_var,start_address,length);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    return j;
}
int Write_Multi_org(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times)
{
    // 	CString str;
    // 	str.Format(_T("ID :%d Multi writing start :%d Amount: %d"),device_var,start_address,length);
    // 	SetPaneString(2,str);
    int j=0;
    for(int i=0; i<retry_times; i++)
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
    for(int i=0; i<retry_times; i++)
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
    int ret=modbus_read_multi_value(
                put_data_into_here,
                device_var,
                start_address,
                length,
                retry_times );
    CString data;
    CString g_strT3000LogString;
    for (int i=0; i<length; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"),put_data_into_here[i]);
        data+=strTemp;
    }
    if (ret>0)
    {
        g_strT3000LogString.Format(_T("Multi Read ID=%d,start address=%d,length=%d"),device_var,start_address,length,data.GetBuffer());
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));

    }
    else
    {
        g_strT3000LogString.Format(_T("Multi Read ID=%d,start address=%d,length=%d,Status:Fail"),device_var,start_address,length);
        CString* pstrInfo = new CString(g_strT3000LogString);
        ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    }
    // SetPaneString(3,g_strT3000LogString);
    return ret;
}




int turn_hex_str_to_ten_num(char *source)
{
    int j=0,k=0,l=0;
    for(int i=0; i<2; i++) //***********************************2
        if(j==0)
        {
            switch(source[i])
            {
            case '0':
                k=0;
                break;
            case '1':
                k=1;
                break;
            case '2':
                k=2;
                break;
            case '3':
                k=3;
                break;
            case '4':
                k=4;
                break;
            case '5':
                k=5;
                break;
            case '6':
                k=6;
                break;
            case '7':
                k=7;
                break;
            case '8':
                k=8;
                break;
            case '9':
                k=9;
                break;

            case 'a':
                k=10;
                break;
            case 'b':
                k=11;
                break;
            case 'c':
                k=12;
                break;
            case 'd':
                k=13;
                break;
            case 'e':
                k=14;
                break;
            case 'f':
                k=15;
                break;
            case 'A':
                k=10;
                break;
            case 'B':
                k=11;
                break;
            case 'C':
                k=12;
                break;
            case 'D':
                k=13;
                break;
            case 'E':
                k=14;
                break;
            case 'F':
                k=15;
                break;

            default:
                return -1;
            }
            for( ; j<2-i-1; j++)
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
    case '0':
        k=0;
        break;
    case '1':
        k=1;
        break;
    case '2':
        k=2;
        break;
    case '3':
        k=3;
        break;
    case '4':
        k=4;
        break;
    case '5':
        k=5;
        break;
    case '6':
        k=6;
        break;
    case '7':
        k=7;
        break;
    case '8':
        k=8;
        break;
    case '9':
        k=9;
        break;

    case 'a':
        k=10;
        break;
    case 'b':
        k=11;
        break;
    case 'c':
        k=12;
        break;
    case 'd':
        k=13;
        break;
    case 'e':
        k=14;
        break;
    case 'f':
        k=15;
        break;
    case 'A':
        k=10;
        break;
    case 'B':
        k=11;
        break;
    case 'C':
        k=12;
        break;
    case 'D':
        k=13;
        break;
    case 'E':
        k=14;
        break;
    case 'F':
        k=15;
        break;

    default:
        return -1;//2
    }
    return k;
}

bool turn_hex_file_line_to_unsigned_char(char *str)
{
    char *p_temp=str;
    int itemp=strlen(p_temp);
    for(int i=0; i<itemp; i++)
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
    for(int i=0; i <length_source; i++)
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
{
    //get tstat version and judge the tstat is online or no
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
    float tstat_version2= product_register_value[4];//tstat version
    if(tstat_version2<=0)
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





UINT get_serialnumber()
{
    return product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
}







BOOL IS_Temco_Product(int product_model)
{

	switch(product_model)
	{
		    case   PM_TSTAT5B				   :
			case   PM_TSTAT5A				   :
			case   PM_TSTAT5B2				   :
			case   PM_TSTAT5C				   :
			case   PM_TSTAT6				   :
			case   PM_TSTAT7				   :
			case   PM_TSTAT5i				   :
			case   PM_TSTAT8				   :
			case   PM_TSTAT8_WIFI:
			case   PM_TSTAT8_OCC:
			case   PM_TSTAT7_ARM:
			case   PM_TSTAT8_220V:

			case   PM_TSTAT5D				   :
			case   PM_AirQuality			   :
			case   PM_HUMTEMPSENSOR			   :
			case   PM_TSTATRUNAR			   :
			case   PM_TSTAT5E				   :
			case   PM_TSTAT5F				   :
			case   PM_TSTAT5G				   :
			case   PM_TSTAT5H				   :
			case   PM_T3PT10				   :
			case   PM_T3IOA					   :
			case   PM_T332AI				   :
			case   PM_T38AI16O				   :
			case   PM_T38I13O				   :
			case   PM_T34AO					   :
			case   PM_T36CT					   :
			case   PM_ZIGBEE				   :
			case   PM_FLEXDRIVER			   :
			case   PM_T3PERFORMANCE			   :
			case   PM_SOLAR					   :
			case   PM_FWMTRANSDUCER			   :
			case   PM_CO2_NET				   :
			case   PM_CO2_RS485				   :
			case   PM_CO2_NODE				   :
			case   PM_MINIPANEL				   :
			case   PM_CS_SM_AC				   :
			case   PM_CS_SM_DC				   :
			case   PM_CS_RSM_AC				   :
			case   PM_CS_RSM_DC				   :
			case   PM_PRESSURE				   :
			case   PM_PM5E					   :
			case   PM_HUM_R					   :
			case   PM_T322AI				   :
			case   PWM_TRANSDUCER				:
			case   PM_T38AI8AO6DO			   :
			case   PM_PRESSURE_SENSOR		   :
			case   PM_T3PT12				   :
			case   PM_T36CTA                   :
			case   PM_CM5					   :
			case   PM_TSTAT6_HUM_Chamber	   :
			case   PM_BEENY					   :
			case   PM_WATER_SENSOR         	   :
			case   PM_NC					   :
			case   PM_LightingController	   :
			case   PM_BTU_METER 			   :
			case   PM_MINI_TOP				   :
			case   PM_LABBATS				   :
			case   PM_TESTER_JIG			   :
			case   STM32_CO2_NET			   :
			case   STM32_CO2_RS485			   :
			case   STM32_HUM_NET			   :
			case   STM32_HUM_RS485			   :
			case   STM32_PRESSURE_NET		   :
			case   STM32_PRESSURE_RS3485	   :
			case   PM_T3_LC:
			       return TRUE;
			default:
			       return FALSE;
	}
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
    else if(nRange==1||nRange==11)
    {
        //
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
    {
        //
        strTemp=_T("%");
    }
    else if(nRange==3)
    {
        //ON/OFF
        strTemp=_T("");
    }
    else if(nRange==4)
    {
        //Customer Sersor
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

    }
    else if (nRange == 6)
    {
        if(nPIDNO==1)
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
	else if (nRange == 13)
	{
		strTemp = L"V";
	}

    return strTemp;
}


BOOL GetSerialComPortNumber1(vector<CString>& szComm)
{
    LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

    HKEY   hKey;
    LONG   lReturnCode=0;
    lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);
    USB_Serial.Empty();
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

            if(strValueName.Find(_T("USBSER")) >=0)
            {
                DFTrace(_T("Find USB Serial Port!"));
                USB_Serial = str;
            }

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

    if(read_write_bacnet_config)	//在读写Bacnet config 的时候禁止刷新List;
        return FALSE;

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
	if((g_protocol == MODBUS_RS485) || (g_protocol == MODBUS_TCPIP))
	{
		if(!PostThreadMessage(nThreadID,MY_RS485_WRITE_LIST,(WPARAM)pmy_write_info,NULL))//post thread msg
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		if(!PostThreadMessage(nThreadID,MY_BAC_WRITE_LIST,(WPARAM)pmy_write_info,NULL))//post thread msg
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
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
    for (int i=0; i<(int)Change_Color_ID.size(); i++)
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





int WriteBacnetPictureData_Blocking(uint32_t deviceid,uint8_t index , unsigned short transfer_packet, unsigned short total_packet,unsigned char * senddata)
{

	for (int z=0; z<3; z++)
	{
		int temp_invoke_id = -1;
		int send_status = true;
		int	resend_count = 0;

		do
		{
			resend_count ++;
			if(resend_count>5)
			{
				send_status = false;
				break;
			}
			temp_invoke_id = WriteBacnetPictureData(deviceid,index,transfer_packet,total_packet,senddata);

			Sleep(SEND_COMMAND_DELAY_TIME);
		}
		while (temp_invoke_id<0);
		if(send_status)
		{
			for (int i=0; i<4000; i++)
			{
				Sleep(1);
				if(tsm_invoke_id_free(temp_invoke_id))
				{
					return 1;
				}
				else
					continue;
			}


		}
	}
	return -1;

	
}


/***************************************************
**
** Write Bacnet private data to device
** Add by Fance
****************************************************/
int WriteProgramData_Blocking(uint32_t deviceid,uint8_t n_command,uint8_t start_instance,uint8_t end_instance ,uint8_t npackage)
{

    for (int z=0; z<3; z++)
    {
		int temp_invoke_id = -1;
		int send_status = true;
		int	resend_count = 0;
        do
        {
            resend_count ++;
            if(resend_count>5)
            {
                send_status = false;
                break;
            }
            temp_invoke_id = WriteProgramData(deviceid,n_command,start_instance,end_instance,npackage);

            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        while (temp_invoke_id<0);
        if(send_status)
        {
            for (int i=0; i<3000; i++)
            {
                Sleep(1);
                if(tsm_invoke_id_free(temp_invoke_id))
                {
                    return 1;
                }
                else
                    continue;
            }


        }
    }
    return -1;

}


//用于 读取program code 现在每个code 最大能有2000个字节;
//
int WriteProgramData(uint32_t deviceid,uint8_t n_command,uint8_t start_instance,uint8_t end_instance ,uint8_t npackage)
{
	unsigned char command = (unsigned char)n_command;

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
	entitysize = 400;
	entitysize = entitysize | (npackage << 9);	//将entitysize 的 高7位用来给program code ，用来记录是第几包;

	char SendBuffer[1000];
	memset(SendBuffer,0,1000);
	char * temp_buffer = SendBuffer;

	Str_user_data_header private_data_chunk;
	//Str_sub_user_data_header private_sub_data_chunk;
	int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
		unsigned char * n_temp_point = program_code[start_instance];


		HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
		private_data_chunk.total_length = PRIVATE_HEAD_LENGTH + ((unsigned char)end_instance - (unsigned char)start_instance + 1)*400;
		private_data_chunk.command = command;
		private_data_chunk.point_start_instance = start_instance;
		private_data_chunk.point_end_instance = end_instance;
		private_data_chunk.entitysize=entitysize;
		Set_transfer_length(private_data_chunk.total_length);
		memcpy_s(SendBuffer,PRIVATE_HEAD_LENGTH ,&private_data_chunk,PRIVATE_HEAD_LENGTH );
		n_temp_point = n_temp_point + npackage*400;
		memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,400,n_temp_point,400);
		Set_transfer_length(private_data_chunk.total_length);
	

		if(debug_item_show == DEBUG_SHOW_PROGRAM_DATA_ONLY)
		{
			CString temp_char;
			CString n_temp_print;
			char * temp_point;
			temp_point = SendBuffer;
			n_temp_print.Format(_T("prg_%d  pack_%d  write:"),start_instance,npackage);
			for (int i = 0; i< private_data_chunk.total_length ; i++)
			{
				temp_char.Format(_T("%02x"),(unsigned char)*temp_point);
				temp_char.MakeUpper();
				temp_point ++;
				n_temp_print = n_temp_print + temp_char + _T(" ");
			}
			DFTrace(n_temp_print);
		}


	status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&SendBuffer, &data_value);
	//ct_test(pTest, status == true);
	private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
	private_data.serviceParameters = &test_value[0];
	private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };

	if(offline_mode)
	{
		SaveBacnetBinaryFile(offline_prg_path);
		return 1;
	}

    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {
        return Send_ConfirmedPrivateTransfer(&dest,&private_data);
    }
    return -2;
}

/***************************************************
**
** Write Bacnet private data to device
** Add by Fance
****************************************************/
int WritePrivateData(uint32_t deviceid,unsigned char n_command,unsigned char start_instance,unsigned char end_instance)
{
    

    unsigned char command = (unsigned char)n_command;

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
	case WRITEEXT_IO_T3000:
		entitysize = sizeof(Str_Extio_point);
		break;
	case WRITE_GRPHIC_LABEL_COMMAND:
		entitysize = sizeof(Str_label_point);
		break;
	case WRITE_AT_COMMAND:
		entitysize = 100;
		break;
	case READ_AT_COMMAND:
		entitysize = 450;
		break;
	case WRITEUSER_T3000:
		entitysize = sizeof(Str_userlogin_point);
		break;
	case WRITEINPUT_T3000:
		entitysize = sizeof(Str_in_point);
		break;
	case WRITEPROGRAM_T3000:
		entitysize = sizeof(Str_program_point);
		break;
	case WRITEUNIT_T3000:
		entitysize = sizeof(Str_Units_element);
		break;
		//case WRITEPROGRAMCODE_T3000:
		//	entitysize = program_code_length[start_instance];
		//
		//	//m_Program_data.at(program_list_line).bytes = my_lengthcode -7;
		//	//entitysize = my_lengthcode;
		//	if((entitysize<0)||(entitysize>400))
		//		entitysize = 0;
		//	break;
	case WRITEVARIABLE_T3000:
		entitysize = sizeof(Str_variable_point);
		break;
	case  WRITEOUTPUT_T3000:
		entitysize = sizeof(Str_out_point);
		break;
	case WRITESCHEDULE_T3000:
		entitysize = sizeof(Str_weekly_routine_point);
		break;
	case WRITEHOLIDAY_T3000:
		entitysize = sizeof(Str_annual_routine_point);
		break;
	case WRITETIMESCHEDULE_T3000:
		entitysize =WEEKLY_SCHEDULE_SIZE;// sizeof(Str_schedual_time_point);
		break;
	case WRITEANNUALSCHEDULE_T3000:
		entitysize = 48;
		break;
	case RESTARTMINI_COMMAND:
		entitysize = sizeof(Time_block_mini);
		break;
	case WRITE_SETTING_COMMAND:
		entitysize = sizeof(Str_Setting_Info);
		break;
	case WRITEPID_T3000:
		entitysize = sizeof(Str_controller_point);
		break;
	case WRITESCREEN_T3000:
		entitysize = sizeof(Control_group_point);
		break;
	case WRITEMONITOR_T3000:
		entitysize = sizeof(Str_monitor_point);
		break;

	case  WRITEALARM_T3000:
		entitysize = sizeof(Alarm_point);
		break;
	case WRITETSTAT_T3000:
		entitysize = sizeof(Str_TstatInfo_point);
		break;
	case WRITE_SUB_ID_BY_HAND:
		entitysize = 254;
		break;
	case DELETE_MONITOR_DATABASE:
		entitysize = 400;
		break;
	case WRITEANALOG_CUS_TABLE_T3000:
		entitysize = sizeof(Str_table_point);
		break;
	case WRITE_MISC:
		entitysize = sizeof(Str_MISC);
		break;
	case WRITE_SPECIAL_COMMAND:
		entitysize = sizeof(Str_Special);
		break;
	case WRITEVARUNIT_T3000:
		entitysize = sizeof(Str_variable_uint_point);
		break;
	default:
		{
			//AfxMessageBox(_T("Entitysize length error!"));
			TRACE(_T("Entitysize length error!"));
			return 0;
		}

	}
    char SendBuffer[1000];
    memset(SendBuffer,0,1000);
    char * temp_buffer = SendBuffer;

    Str_user_data_header private_data_chunk;
    //Str_sub_user_data_header private_sub_data_chunk;
    int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;

    HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
    private_data_chunk.total_length = PRIVATE_HEAD_LENGTH + ((unsigned char)end_instance - (unsigned char)start_instance + 1)*entitysize;
    private_data_chunk.command = command;
    private_data_chunk.point_start_instance = start_instance;
    private_data_chunk.point_end_instance = end_instance;
    private_data_chunk.entitysize=entitysize;
    Set_transfer_length(private_data_chunk.total_length);
    memcpy_s(SendBuffer,PRIVATE_HEAD_LENGTH ,&private_data_chunk,PRIVATE_HEAD_LENGTH );

    switch(command)
    {
	case WRITEEXT_IO_T3000:
		{
			for (int i=0; i<(end_instance - start_instance + 1); i++)
			{
				memcpy_s(SendBuffer + i*sizeof(Str_Extio_point) +HEADER_LENGTH,sizeof(Str_Extio_point),&m_extio_config_data.at(i + start_instance),sizeof(Str_Extio_point));
			}
		}
		break;
    case  WRITE_GRPHIC_LABEL_COMMAND:
        for (int i=0; i<(end_instance - start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_label_point) +HEADER_LENGTH,sizeof(Str_label_point),&m_graphic_label_data.at(i + start_instance),sizeof(Str_label_point));
        }
        break;
    case WRITEUSER_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_userlogin_point) + HEADER_LENGTH,sizeof(Str_userlogin_point),&m_user_login_data.at(i + start_instance),sizeof(Str_userlogin_point));
        }
        break;
    case WRITEINPUT_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_in_point) + HEADER_LENGTH,sizeof(Str_in_point),&m_Input_data.at(i + start_instance),sizeof(Str_in_point));
        }
        break;
    case WRITEPROGRAM_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_program_point) + HEADER_LENGTH,sizeof(Str_program_point),&m_Program_data.at(i + start_instance),sizeof(Str_program_point));
        }

        break;
    case  WRITEUNIT_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_Units_element) + HEADER_LENGTH,sizeof(Str_Units_element),&m_customer_unit_data.at(i + start_instance),sizeof(Str_Units_element));
        }
        break;
    case WRITE_AT_COMMAND:
    {
        memcpy_s(SendBuffer + HEADER_LENGTH,entitysize,m_at_write_buf,entitysize);
    }
    break;
    case READ_AT_COMMAND:
    {
        memcpy_s(SendBuffer + HEADER_LENGTH,entitysize,m_at_read_buf,entitysize);
    }
    break;
    //case WRITEPROGRAMCODE_T3000:
    //	{
//		//memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,entitysize,mycode,my_lengthcode);
    //	memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH,entitysize,program_code[start_instance],entitysize);
    //
    //	CString n_temp_print;
    //	n_temp_print.Format(_T("Tx : "));
    //	CString temp_char;
    //	char * temp_print = SendBuffer;
    //	for (int i = 0; i< entitysize + 2 ; i++)
    //	{
    //		temp_char.Format(_T("%02x"),(unsigned char)*temp_print);
    //		temp_char.MakeUpper();
    //		temp_print ++;
    //		n_temp_print = n_temp_print + temp_char + _T(" ");
    //	}
    //	DFTrace(n_temp_print);
    //	}
    //	break;
    case WRITEVARIABLE_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_variable_point) + HEADER_LENGTH,sizeof(Str_variable_point),&m_Variable_data.at(i + start_instance),sizeof(Str_variable_point));
        }
        break;
    case  WRITEOUTPUT_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_out_point) + HEADER_LENGTH,sizeof(Str_out_point),&m_Output_data.at(i + start_instance),sizeof(Str_out_point));
        }
        break;
    case WRITESCHEDULE_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_weekly_routine_point) + HEADER_LENGTH,sizeof(Str_weekly_routine_point),&m_Weekly_data.at(i + start_instance),sizeof(Str_weekly_routine_point));
        }
        break;
    case WRITETIMESCHEDULE_T3000:
        temp_buffer = temp_buffer + HEADER_LENGTH;
        for (int j=0; j<9; j++)
        {
            for (int i=0; i<8; i++)
            {
                *(temp_buffer++) = m_Schedual_Time_data.at(start_instance).Schedual_Day_Time[i][j].time_minutes;// = *(my_temp_point ++);
                *(temp_buffer++) = m_Schedual_Time_data.at(start_instance).Schedual_Day_Time[i][j].time_hours;// = *(my_temp_point ++);
            }
        }

        break;
    case  WRITEHOLIDAY_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_annual_routine_point) + HEADER_LENGTH,sizeof(Str_annual_routine_point),&m_Annual_data.at(i + start_instance),sizeof(Str_annual_routine_point));
        }
        break;
    case WRITEANNUALSCHEDULE_T3000:
        memcpy_s(SendBuffer + HEADER_LENGTH,ANNUAL_CODE_SIZE,&g_DayState[start_instance],ANNUAL_CODE_SIZE);

        //memcpy_s(g_DayState[annual_list_line],block_length,my_temp_point,block_length);
        break;
    case RESTARTMINI_COMMAND:
    {
        memcpy_s(SendBuffer + HEADER_LENGTH,sizeof(Time_block_mini),&Device_time,sizeof(Time_block_mini));
    }
    break;
    case WRITE_SETTING_COMMAND:
    {
        memcpy_s(SendBuffer + HEADER_LENGTH,sizeof(Str_Setting_Info),&Device_Basic_Setting,sizeof(Str_Setting_Info));
    }
    break;
    case WRITEPID_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_controller_point) + HEADER_LENGTH,sizeof(Str_controller_point),&m_controller_data.at(i + start_instance),sizeof(Str_controller_point));
        }
        break;
    case WRITESCREEN_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Control_group_point) + HEADER_LENGTH,sizeof(Control_group_point),&m_screen_data.at(i + start_instance),sizeof(Control_group_point));
        }
        break;
    case WRITEMONITOR_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_monitor_point) + HEADER_LENGTH,sizeof(Str_monitor_point),&m_monitor_data.at(i + start_instance),sizeof(Str_monitor_point));
        }
        break;
    case WRITETSTAT_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_TstatInfo_point) + HEADER_LENGTH,sizeof(Str_TstatInfo_point),&m_Tstat_data.at(i + start_instance),sizeof(Str_TstatInfo_point));
        }
        break;
	case WRITEVARUNIT_T3000:
		{
			for (int i=0; i<(end_instance-start_instance + 1); i++)
			{
				memcpy_s(SendBuffer + i*sizeof(Str_variable_uint_point) + HEADER_LENGTH,sizeof(Str_variable_uint_point),&m_variable_analog_unite.at(i + start_instance),sizeof(Str_variable_uint_point));
			}
		}
		break;
    case  WRITEALARM_T3000:
        memcpy_s(SendBuffer + HEADER_LENGTH,sizeof(Alarm_point),&m_alarmlog_data.at(start_instance),sizeof(Alarm_point));
        break;
    case  WRITE_SUB_ID_BY_HAND:
        memcpy_s(SendBuffer + HEADER_LENGTH,254,bacnet_add_id,254);
        break;
    case  DELETE_MONITOR_DATABASE:
        memcpy_s(SendBuffer + HEADER_LENGTH,24,monitor_database_flag,24);
        break;
    case WRITEANALOG_CUS_TABLE_T3000:
        for (int i=0; i<(end_instance-start_instance + 1); i++)
        {
            memcpy_s(SendBuffer + i*sizeof(Str_table_point) + HEADER_LENGTH,sizeof(Str_table_point),&m_analog_custmer_range.at(i + start_instance),sizeof(Str_table_point));
        }
        break;
	case  WRITE_MISC:
		{
			 memcpy_s(SendBuffer +   HEADER_LENGTH,sizeof(Str_MISC),&Device_Misc_Data,sizeof(Str_MISC));
		}
		break;
	case WRITE_SPECIAL_COMMAND:
			memcpy_s(SendBuffer +   HEADER_LENGTH,sizeof(Str_Special),&Device_Special_Data,sizeof(Str_Special));
		break;
    default:
    {
        AfxMessageBox(_T("Command not match!Please Check it!"));
        return -1;
    }
    break;
    }



	if((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
	{
		CString total_char_test;
		total_char_test = _T("Write : ");
		char * temp_print_test = NULL;
		temp_print_test = SendBuffer;
		for (int i = 0; i< private_data_chunk.total_length ; i++)
		{
			CString temp_char_test;
			temp_char_test.Format(_T("%02x"),(unsigned char)*temp_print_test);
			temp_char_test.MakeUpper();
			temp_print_test ++;
			total_char_test = total_char_test + temp_char_test + _T(" ");
		}
		DFTrace(total_char_test);
	}
	if(offline_mode)
	{
		SaveBacnetBinaryFile(offline_prg_path);
		return 1;
	}

    status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&SendBuffer, &data_value);
    //ct_test(pTest, status == true);
    private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
    private_data.serviceParameters = &test_value[0];
    private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };
    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {
        g_llTxCount++;
        return Send_ConfirmedPrivateTransfer(&dest,&private_data);
    }
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Communication failed!"));
	}
    return -2;
}

int GetPrivateData_Blocking(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize)
{

    int send_status = true;
    
    for (int z=0; z<10; z++)
    {
		int temp_invoke_id = -1;
		int	resend_count = 0;
		send_status = true;
        do
        {
            resend_count ++;
            if(resend_count>10)
            {
                send_status = false;
                break;
            }
			if(deviceid == 0)
				temp_invoke_id =  GetPrivateData(g_bac_instance,command,start_instance,end_instance,entitysize);
			else
				temp_invoke_id =  GetPrivateData( deviceid,command, start_instance,end_instance,entitysize);
			if(temp_invoke_id < 0)
				Sleep(500);
			else
				send_status = true;
			//else
			//	Sleep(SEND_COMMAND_DELAY_TIME);
        }
        while (temp_invoke_id<0);
	
        if(send_status)
        {
            for (int i=0; i<300; i++)
            {
                Sleep(10);
                if(tsm_invoke_id_free(temp_invoke_id))
                {
                    return 1;
                }
                else
                    continue;
            }
        }
    }
    return -1;
}


/***************************************************
**
** Write Bacnet private data to device
** Add by Fance
****************************************************/
int Write_Private_Data_Blocking(uint8_t ncommand,uint8_t nstart_index,uint8_t nstop_index,unsigned int write_object_list)
{
	int temp_invoke_id = -1;
	int send_status = true;
	int	resend_count = 0;
	for (int z=0;z<5;z++)
	{
		do 
		{
			resend_count ++;
			if(resend_count>5)
			{
				send_status = false;
				break;
			}
			if(write_object_list == 0)
				temp_invoke_id = WritePrivateData(g_bac_instance,ncommand,nstart_index,nstop_index);
			else
				temp_invoke_id = WritePrivateData(write_object_list,ncommand,nstart_index,nstop_index);

			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (temp_invoke_id<0);
		if(send_status)
		{
			for (int i=0;i<3000;i++)
			{
				Sleep(1);
				if(tsm_invoke_id_free(temp_invoke_id))
				{
					g_llRxCount++;
					return 1;
				}
				else
					continue;
			}
			

        }
    }
    return -1;

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
int GetPrivateData(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize)
{
    

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


    char SendBuffer[1000];
    memset(SendBuffer,0,1000);
    char * temp_buffer = SendBuffer;

    Str_user_data_header private_data_chunk;
    int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;

    HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
    private_data_chunk.total_length=PRIVATE_HEAD_LENGTH;
    private_data_chunk.command = command;
    private_data_chunk.point_start_instance=start_instance;
    private_data_chunk.point_end_instance=end_instance;
    private_data_chunk.entitysize=entitysize;
    Set_transfer_length(PRIVATE_HEAD_LENGTH);
    status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&private_data_chunk, &data_value);

    private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
    private_data.serviceParameters = &test_value[0];
    private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };
	if(offline_mode)
	{
		LoadBacnetConfigFile(false,offline_prg_path);
		return 1;
	}

    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {
        g_llTxCount++;
        return Send_ConfirmedPrivateTransfer(&dest,&private_data);
    }
    else
        return -2;
}




/************************************************************************/
/*
Author: Fance
Get Bacnet Private Data
<param name="deviceid">Bacnet Device ID
<param name="start_instance">start point
<param name="end_instance">end point
<param name="entitysize">Block size of read  包含 第几包
*/
/************************************************************************/
int GetProgramData(uint32_t deviceid,uint8_t start_instance,uint8_t end_instance,uint8_t npackgae)
{
    

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

    unsigned short	entitysize = 400;
    entitysize = entitysize | (npackgae << 9);	//将entitysize 的 高7位用来给program code ，用来记录是第几包;

    Str_user_data_header private_data_chunk;
    //Str_sub_user_data_header private_sub_data_chunk;

    int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;

    private_data_chunk.total_length=PRIVATE_HEAD_LENGTH;
    private_data_chunk.command = READPROGRAMCODE_T3000;
    private_data_chunk.point_start_instance=start_instance;
    private_data_chunk.point_end_instance=end_instance;
    private_data_chunk.entitysize=	entitysize;
    Set_transfer_length(PRIVATE_HEAD_LENGTH);
    status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&private_data_chunk, &data_value);

    private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
    private_data.serviceParameters = &test_value[0];
    private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };

	if(offline_mode)
	{
		LoadBacnetConfigFile(false,offline_prg_path);
		return 1;
	}

    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {

        return Send_ConfirmedPrivateTransfer(&dest,&private_data);
    }
    else
        return -2;
}






int GetProgramData_Blocking(uint32_t deviceid,uint8_t start_instance,uint8_t end_instance,uint8_t npackgae)
{
    int temp_invoke_id = -1;
    int send_status = true;
    int	resend_count = 0;
    for (int z=0; z<5; z++)
    {
        do
        {
            resend_count ++;
            if(resend_count>5)
            {
                send_status = false;
                break;
            }
            temp_invoke_id =  GetProgramData(
                                  deviceid,
                                  start_instance,
                                  end_instance,
                                  npackgae);

            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        while (temp_invoke_id<0);
        if(send_status)
        {
            for (int i=0; i<300; i++)
            {
                Sleep(10);
                if(tsm_invoke_id_free(temp_invoke_id))
                {
                    return 1;
                }
                else
                    continue;
            }
        }
    }
    return -1;
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
int GetMonitorBlockData(uint32_t deviceid,int8_t command,int8_t nIndex,int8_t ntype_ad, uint16_t ntotal_seg,uint16_t nseg_index,MonitorUpdateData* up_data)
{
    

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
    //private_data_chunk.total_length = 0;
    private_data_chunk.total_seg = 0;
    private_data_chunk.command = command;
    private_data_chunk.index = nIndex;
    private_data_chunk.conm_args.nsize = up_data->nsize;
    private_data_chunk.conm_args.oldest_time = up_data->oldest_time;
    private_data_chunk.conm_args.most_recent_time = up_data->most_recent_time;
    private_data_chunk.type = ntype_ad;
    private_data_chunk.special = 0;
    //private_data_chunk.total_seg = ntotal_seg;
    private_data_chunk.seg_index = nseg_index;
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
        g_llTxCount ++;
        return Send_ConfirmedPrivateTransfer(&dest,&private_data);
        //return g_invoke_id;
    }
    else
        return -2;

}



/***************************************************
**
** Compare all the labels , if new label already exist ,return true. Not exist return false.
** Add by Fance
****************************************************/
bool Check_FullLabel_Exsit(LPCTSTR m_new_fulllabel)
{
    CString new_string;
    new_string = m_new_fulllabel;
    if(new_string.IsEmpty())
        return false;
    char cTemp1[255];
    memset(cTemp1,0,255);
    WideCharToMultiByte( CP_ACP, 0, new_string.GetBuffer(), -1, cTemp1, 255, NULL, NULL );

    for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Input_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Output_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Variable_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Program_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_SCREEN_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_screen_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Weekly_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Annual_data.at(i).description) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    return false;
}


/***************************************************
**
** Compare all the labels , if new label already exist ,return true. Not exist return false.
** Add by Fance
****************************************************/
bool Check_Label_Exsit(LPCTSTR m_new_label)
{
    CString new_string;
    new_string = m_new_label;
    if(new_string.IsEmpty())
        return false;
    char cTemp1[255];
    memset(cTemp1,0,255);
    WideCharToMultiByte( CP_ACP, 0, new_string.GetBuffer(), -1, cTemp1, 255, NULL, NULL );

    for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Input_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Output_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Variable_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Program_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_SCREEN_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_screen_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Weekly_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_Annual_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }

    for (int i=0; i<BAC_MONITOR_COUNT; i++)
    {
        if(strcmp(cTemp1,(char *)m_monitor_data.at(i).label) == 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS,new_string + _T(" already exist !"));
            return true;
        }
    }
    return false;
}


/***************************************************
**
** Receive Bacnet private data from device , and handle the data.
** Code by Fance
****************************************************/

int Bacnet_PrivateData_Handle(	BACNET_PRIVATE_TRANSFER_DATA * data,bool &end_flag)
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
    char temp_buf[500];
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

    g_llRxCount++;
//redecode_part:
    command_type = Temp_CS.value[2];


	CString total_char_test;
	if((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
	{
		char * temp_print_test = NULL;
		temp_print_test = (char *)&Temp_CS.value;
		for (int i = 0; i< len_value_type ; i++)
		{
			CString temp_char_test;
			temp_char_test.Format(_T("%02x"),(unsigned char)*temp_print_test);
			temp_char_test.MakeUpper();
			temp_print_test ++;
			total_char_test = total_char_test + temp_char_test + _T(" ");
		}
		DFTrace(total_char_test);
	}



    unsigned int start_instance=0;
    unsigned int end_instance = 0;
    ///////////////////////////////
	switch(command_type)
	{
	case READEXT_IO_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_Extio_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_Extio_point);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(end_instance == (BAC_EXTIO_COUNT - 1))
				end_flag = true;
			for (i=start_instance; i<=end_instance; i++)
			{
				m_extio_config_data.at(i).reg.product_id = *(my_temp_point++);
				m_extio_config_data.at(i).reg.port = *(my_temp_point++);
				m_extio_config_data.at(i).reg.modbus_id = *(my_temp_point++);
				m_extio_config_data.at(i).reg.last_contact_time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
				my_temp_point = my_temp_point + 4;
				m_extio_config_data.at(i).reg.input_start = *(my_temp_point++);
				m_extio_config_data.at(i).reg.input_end = *(my_temp_point++);
				m_extio_config_data.at(i).reg.output_start = *(my_temp_point++);
				m_extio_config_data.at(i).reg.output_end = *(my_temp_point++);
				m_extio_config_data.at(i).reg.serialnumber = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
				my_temp_point = my_temp_point + 4;
				my_temp_point = my_temp_point + 15;
			}
		}
		break;
	case READ_REMOTE_POINT:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_remote_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_remote_point);
			//m_Input_data_length = block_length;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(end_instance == (BAC_REMOTE_POINT_COUNT - 1))
				end_flag = true;
			for (i=start_instance; i<=end_instance; i++)
			{
				m_remote_point_data.at(i).point.number = *(my_temp_point++);
				m_remote_point_data.at(i).point.point_type = *(my_temp_point++);
				m_remote_point_data.at(i).point.panel = *(my_temp_point++);
				m_remote_point_data.at(i).point.sub_panel = *(my_temp_point++);
				m_remote_point_data.at(i).point.network = *(my_temp_point++);

				m_remote_point_data.at(i).point_value = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
				my_temp_point = my_temp_point + 4;

				m_remote_point_data.at(i).auto_manual = *(my_temp_point++);
				m_remote_point_data.at(i).digital_analog = *(my_temp_point++);
				m_remote_point_data.at(i).device_online = *(my_temp_point++);
				m_remote_point_data.at(i).product_id = *(my_temp_point++);
				m_remote_point_data.at(i).count = *(my_temp_point++);
				m_remote_point_data.at(i).read_write = *(my_temp_point++);
				m_remote_point_data.at(i).change = *(my_temp_point++);

			}
		}
		return READ_REMOTE_POINT;
		break;
	case READ_GRPHIC_LABEL_COMMAND:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_label_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_label_point);
			//m_Input_data_length = block_length;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			if(end_instance == (BAC_GRPHIC_LABEL_COUNT - 1))
				end_flag = true;
			if((start_instance > end_instance) || (start_instance >= BAC_GRPHIC_LABEL_COUNT) || (end_instance >= BAC_GRPHIC_LABEL_COUNT))
				return -1;
			for (i=start_instance; i<=end_instance; i++)
			{
				m_graphic_label_data.at(i).reg.label_status = *(my_temp_point++);
				//temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				temp_struct_value = g_serialNum;
				if((temp_struct_value == 0) || (m_graphic_label_data.at(i).reg.label_status == NO_UNSED_LABEL))
				{
					b_stop_read_grp_label = true;
					return -1;
				}
				b_stop_read_grp_label = false;
				m_graphic_label_data.at(i).reg.nSerialNum = temp_struct_value;
				my_temp_point = my_temp_point + 4;
				m_graphic_label_data.at(i).reg.nScreen_index = *(my_temp_point++);
				m_graphic_label_data.at(i).reg.nLabel_index = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point + 2;
				m_graphic_label_data.at(i).reg.nMain_Panel = *(my_temp_point++);
				m_graphic_label_data.at(i).reg.nSub_Panel = *(my_temp_point++);

				//下面的做法不合理，懒得改了，留给后面维护的人;  从一个panel 的prg 导入另一个panel 的prg  他们的 panel number 不同 会出现很多问题;
				if(m_graphic_label_data.at(i).reg.nMain_Panel == m_graphic_label_data.at(i).reg.nSub_Panel)
				{
					if(m_graphic_label_data.at(i).reg.nMain_Panel != Station_NUM)
					{
						m_graphic_label_data.at(i).reg.nMain_Panel = m_graphic_label_data.at(i).reg.nSub_Panel = Station_NUM;
					}
				}
				m_graphic_label_data.at(i).reg.nPoint_type = *(my_temp_point++);
				m_graphic_label_data.at(i).reg.nPoint_number = *(my_temp_point++);
				m_graphic_label_data.at(i).reg.nPoint_x = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point + 2;
				m_graphic_label_data.at(i).reg.nPoint_y = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point + 2;
				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_graphic_label_data.at(i).reg.nclrTxt = temp_struct_value;
				my_temp_point = my_temp_point + 4;
				m_graphic_label_data.at(i).reg.nDisplay_Type = *(my_temp_point++);
				m_graphic_label_data.at(i).reg.nIcon_size = *(my_temp_point++);
				m_graphic_label_data.at(i).reg.nIcon_place = *(my_temp_point++);
				if(strlen(my_temp_point)>=STR_ICON_1_NAME_LENGTH)
					memset(m_graphic_label_data.at(i).reg.icon_name_1,0,STR_ICON_1_NAME_LENGTH);
				else
					memcpy_s( m_graphic_label_data.at(i).reg.icon_name_1,STR_ICON_1_NAME_LENGTH,my_temp_point,STR_ICON_1_NAME_LENGTH);
				my_temp_point=my_temp_point + STR_ICON_1_NAME_LENGTH;
				if(strlen(my_temp_point)>=STR_ICON_2_NAME_LENGTH)
					memset(m_graphic_label_data.at(i).reg.icon_name_2,0,STR_ICON_2_NAME_LENGTH);
				else
					memcpy_s( m_graphic_label_data.at(i).reg.icon_name_2,STR_ICON_2_NAME_LENGTH,my_temp_point,STR_ICON_2_NAME_LENGTH);
				my_temp_point=my_temp_point + STR_ICON_2_NAME_LENGTH;
				my_temp_point = my_temp_point + 7;
			}


		}
		return READ_GRPHIC_LABEL_COMMAND;
		break;
	case READ_AT_COMMAND:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(450)!=0)
				return -1;	//得到的结构长度错误;

			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/450;
			//m_Input_data_length = block_length;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;

			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			memset(m_at_read_buf,0,450);
			memcpy_s(m_at_read_buf,450,my_temp_point,450);



			CString n_temp_print;
			n_temp_print.Format(_T("AT Rx : "));
			CString temp_char;
			char * temp_print = m_at_read_buf;
			len_value_type = strlen(m_at_read_buf);
			//for (int i = 0; i< len_value_type ; i++)
			//{
			//	temp_char.Format(_T("%02x"),(unsigned char)*temp_print);
			//	temp_char.MakeUpper();
			//	temp_print ++;
			//	n_temp_print = n_temp_print + temp_char + _T(" ");
			//}
			//DFTrace(n_temp_print);



			return READ_AT_COMMAND;
		}
		break;
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
			if(end_instance == (BAC_OUTPUT_ITEM_COUNT - 1))
				end_flag = true;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_OUTPUT_ITEM_COUNT)
				return -1;//超过长度了;

			for (i=start_instance; i<=end_instance; i++)
			{
				if(strlen(my_temp_point)>STR_OUT_DESCRIPTION_LENGTH - 2)
				{
					memcpy_s( m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH - 2,my_temp_point,STR_OUT_DESCRIPTION_LENGTH - 2);
					m_Output_data.at(i).description[18] = 0;
				}
				else
					memcpy_s( m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH - 2,my_temp_point,STR_OUT_DESCRIPTION_LENGTH - 2);

				my_temp_point=my_temp_point + STR_OUT_DESCRIPTION_LENGTH - 2;

				m_Output_data.at(i).low_voltage = *(my_temp_point++);
				m_Output_data.at(i).high_voltage = *(my_temp_point++);
				if(m_Output_data.at(i).low_voltage > 120)
					m_Output_data.at(i).low_voltage = 0;
				if(m_Output_data.at(i).high_voltage > 120)
					m_Output_data.at(i).high_voltage = 0;


				if(strlen(my_temp_point)>STR_OUT_LABEL)
					memset(m_Output_data.at(i).label,0,STR_OUT_LABEL);
				else
					memcpy_s(m_Output_data.at(i).label,STR_OUT_LABEL ,my_temp_point,STR_OUT_LABEL );
				my_temp_point=my_temp_point + STR_OUT_LABEL ;


				CString cs_temp;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label)+1,
					cs_temp.GetBuffer(MAX_PATH), MAX_PATH );
				cs_temp.ReleaseBuffer();

				int ret_1 = cs_temp.Replace(_T("-"),_T("_"));
				int ret_2 = cs_temp.Replace(_T("."),_T("_"));
				if((ret_1 !=0 ) || (ret_2 != 0))
				{
					char cTemp1[255];
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Output_data.at(i).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);
				}



				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_Output_data.at(i).value = temp_struct_value;
				//memcpy_s(Private_data[i].value,4,temp_struct_value,4);
				my_temp_point=my_temp_point+4;

				m_Output_data.at(i).auto_manual = *(my_temp_point++);
				m_Output_data.at(i).digital_analog = *(my_temp_point++);
				m_Output_data.at(i).hw_switch_status = *(my_temp_point++);
				m_Output_data.at(i).control = *(my_temp_point++);
				m_Output_data.at(i).digital_control = *(my_temp_point++);
				m_Output_data.at(i).decom	= *(my_temp_point++);
				m_Output_data.at(i).range = *(my_temp_point++);
				m_Output_data.at(i).sub_id = *(my_temp_point++);
				m_Output_data.at(i).sub_product = *(my_temp_point++);
				m_Output_data.at(i).sub_number = *(my_temp_point++);

				//temp_out.delay_timer = *(my_temp_point++);  Output 这个Delay time先不管 清0
				m_Output_data.at(i).pwm_period = *(my_temp_point++);
				CString g_configfile_path = g_strExePth + g_strStartInterface_config;
				int savetodb = GetPrivateProfileInt(_T("SaveToDB"), _T("OUTPUT"), 0, g_configfile_path);
				if (savetodb == 1)
				{
					Save_OutputData_to_db(i);
				}
			}
			return READOUTPUT_T3000;
		}
		break;

	case READINPUT_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_in_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_in_point);
			//m_Input_data_length = block_length;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			if(end_instance == (BAC_INPUT_ITEM_COUNT - 1))
				end_flag = true;

			if(start_instance >= BAC_INPUT_ITEM_COUNT)
				return -1;//超过长度了;
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			//m_Input_data.clear();
			//TRACE(_T("receive input %d-%d\n"),start_instance,end_instance);
			for (i=start_instance; i<=end_instance; i++)
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



				CString cs_temp;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label)+1,
					cs_temp.GetBuffer(MAX_PATH), MAX_PATH );
				cs_temp.ReleaseBuffer();

				int ret_1 = cs_temp.Replace(_T("-"),_T("_"));
				int ret_2 = cs_temp.Replace(_T("."),_T("_"));
				if((ret_1 !=0 ) || (ret_2 != 0))
				{
					char cTemp1[255];
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Input_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
				}
				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_Input_data.at(i).value = temp_struct_value;
			 
				my_temp_point=my_temp_point+4;
				m_Input_data.at(i).filter = *(my_temp_point++);
				m_Input_data.at(i).decom	= *(my_temp_point++);
				m_Input_data.at(i).sub_id	= *(my_temp_point++);
				m_Input_data.at(i).sub_product = *(my_temp_point++);
				m_Input_data.at(i).control = *(my_temp_point++);
				m_Input_data.at(i).auto_manual = *(my_temp_point++);
				m_Input_data.at(i).digital_analog = *(my_temp_point++);
				m_Input_data.at(i).calibration_sign = *(my_temp_point++);
				m_Input_data.at(i).sub_number = *(my_temp_point++);
				m_Input_data.at(i).calibration_h = *(my_temp_point++);
				m_Input_data.at(i).calibration_l = *(my_temp_point++);
				m_Input_data.at(i).range = *(my_temp_point++);
				CString g_configfile_path = g_strExePth + g_strStartInterface_config;
				int savetodb = GetPrivateProfileInt(_T("SaveToDB"), _T("INPUT"), 0, g_configfile_path);
				if (savetodb == 1)
				{
					Save_InputData_to_db(i);
				}
			 
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
			if(end_instance == (BAC_VARIABLE_ITEM_COUNT - 1))
				end_flag = true;
			if(start_instance >= BAC_VARIABLE_ITEM_COUNT)
				return -1;//超过长度了;

			//m_Input_data_length = block_length;
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			//m_Variable_data.clear();
			for (i=start_instance; i<=end_instance; i++)
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


				CString cs_temp;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label)+1,
					cs_temp.GetBuffer(MAX_PATH), MAX_PATH );
				cs_temp.ReleaseBuffer();

				int ret_1 = cs_temp.Replace(_T("-"),_T("_"));
				int ret_2 = cs_temp.Replace(_T("."),_T("_"));
				if((ret_1 !=0 ) || (ret_2 != 0))
				{
					char cTemp1[255];
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Variable_data.at(i).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);
				}



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
	case READANALOG_CUS_TABLE_T3000:
		{
			//CString temp_char2;
			//CString n_temp_print2;
			//char * temp_print2 = (char *)Temp_CS.value;
			//for (int i = 0; i< len_value_type ; i++)
			//{
			//    temp_char2.Format(_T("%02x"),(unsigned char)*temp_print2);
			//    temp_char2.MakeUpper();
			//    temp_print2 ++;
			//    n_temp_print2 = n_temp_print2 + temp_char2 + _T(" ");
			//}
			//CString temp_123;
			//temp_123.Format(_T("Reply length %d"),len_value_type);
			//n_temp_print2 = temp_123 +_T("    ") + n_temp_print2;
			//DFTrace(n_temp_print2);


			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_table_point))!=0)
				return -1;	//得到的结构长度错误;

			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_table_point);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			if(end_instance == (BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT - 1))
				end_flag = true;
			if(start_instance >= BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT)
				return -1;//超过长度了;

			for (int i=start_instance; i<=end_instance; i++)
			{
				if(strlen(my_temp_point) > STR_VARIABLE_DESCRIPTION_LENGTH)
					memset(m_analog_custmer_range.at(i).table_name,0,9);
				else
					memcpy_s( m_analog_custmer_range.at(i).table_name,9,my_temp_point,9);

				MultiByteToWideChar( CP_ACP, 0, (char *)m_analog_custmer_range.at(i).table_name, 
					(int)strlen((char *)m_analog_custmer_range.at(i).table_name)+1, 
					Analog_Customer_Units[i].GetBuffer(MAX_PATH), MAX_PATH );
				Analog_Customer_Units[i].ReleaseBuffer();	

				my_temp_point = my_temp_point + 9;
				for (int j=0; j<16; j++)
				{
					m_analog_custmer_range.at(i).dat[j].value = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
					my_temp_point = my_temp_point + 2;
					m_analog_custmer_range.at(i).dat[j].unit =	((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
					my_temp_point = my_temp_point + 4;
				}

			}
			return READANALOG_CUS_TABLE_T3000;
		}
		break;
	case READVARUNIT_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_variable_uint_point))!=0)
				return -1;	//得到的结构长度错误;

			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_variable_uint_point);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			if(end_instance == (BAC_VARIABLE_CUS_UNIT_COUNT - 1))
				end_flag = true;
			if(start_instance >= BAC_VARIABLE_CUS_UNIT_COUNT)
				return -1;//超过长度了;

			for (int i=start_instance; i<=end_instance; i++)
			{
				if(strlen(my_temp_point) > 20)
					memset(m_variable_analog_unite.at(i).variable_cus_unite,0,20);
				else
					memcpy_s( m_variable_analog_unite.at(i).variable_cus_unite,20,my_temp_point,20);
				my_temp_point = my_temp_point + 20;
				MultiByteToWideChar( CP_ACP, 0, (char *) m_variable_analog_unite.at(i).variable_cus_unite, 
					(int)strlen((char *) m_variable_analog_unite.at(i).variable_cus_unite)+1, 
					Analog_Variable_Units[i].GetBuffer(MAX_PATH), MAX_PATH );
				Analog_Variable_Units[i].ReleaseBuffer();	

			}

			return READVARUNIT_T3000;
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
			if(end_instance == (BAC_SCHEDULE_COUNT - 1))
				end_flag = true;
			if(start_instance >= BAC_SCHEDULE_COUNT)
				return -1;//超过长度了;

			for (i=start_instance; i<=end_instance; i++)
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
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_annual_routine_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_annual_routine_point);

			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			if(end_instance == (BAC_HOLIDAY_COUNT - 1))
				end_flag = true;
			if(start_instance >= BAC_HOLIDAY_COUNT)
				return -1;//超过长度了;

			for (i=start_instance; i<=end_instance; i++)
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
		}
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

			if(start_instance >= BAC_PROGRAM_ITEM_COUNT)
				return -1;//超过长度了;
			if(end_instance == (BAC_PROGRAM_ITEM_COUNT - 1))
				end_flag = true;

			for (i=start_instance; i<=end_instance; i++)
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
	case READUSER_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_userlogin_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_userlogin_point);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_CUSTOMER_UNITS_COUNT)
				return -1;//超过长度了;

			for (i=start_instance; i<=end_instance; i++)
			{
				//Str_program_point temp_in;
				if(strlen(my_temp_point) >= STR_USER_NAME_LENGTH)
					memset(m_user_login_data.at(i).name,0,STR_USER_NAME_LENGTH);
				else
					memcpy_s( m_user_login_data.at(i).name,STR_USER_NAME_LENGTH,my_temp_point,STR_USER_NAME_LENGTH);
				my_temp_point=my_temp_point + STR_USER_NAME_LENGTH;
				if(strlen(my_temp_point) >= STR_USER_PASSWORD_LENGTH)
					memset(m_user_login_data.at(i).password,0,STR_USER_PASSWORD_LENGTH);
				else
					memcpy_s( m_user_login_data.at(i).password,STR_USER_PASSWORD_LENGTH ,my_temp_point,STR_USER_PASSWORD_LENGTH );
				my_temp_point=my_temp_point + STR_USER_PASSWORD_LENGTH ;

				m_user_login_data.at(i).access_level = *(my_temp_point++);
				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_user_login_data.at(i).rights_access = temp_struct_value;
				my_temp_point = my_temp_point + 4;

				m_user_login_data.at(i).default_panel = *(my_temp_point++);
				m_user_login_data.at(i).default_group = *(my_temp_point++);

				memcpy_s( m_user_login_data.at(i).screen_right,8,my_temp_point,8);
				my_temp_point = my_temp_point + 8;
				memcpy_s( m_user_login_data.at(i).program_right,8,my_temp_point,8);
				my_temp_point = my_temp_point + 8;
			}
			return READUSER_T3000;

		}
		break;
	case READPROGRAMCODE_T3000://Fance 将program code 存至Buf 等待发送消息后使用解码函数
		{
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;

			unsigned char package = my_temp_point[1] >> 1;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_PROGRAMCODE_ITEM_COUNT)
				return -1;//超过长度了;

			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
			if(block_length <400)
				return -1;
			int code_length = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
			//my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;

			if(package == 0)
			{
				program_code_length[start_instance] = ((unsigned char)my_temp_point[1])*256 + (unsigned char)my_temp_point[0];
				if(program_code_length[start_instance] > 2000)
					program_code_length[start_instance] = 0;
				//TRACE(_T("program_code_length%d = %d   [1] = %d [0] = %d \r\n"),start_instance,program_code_length[start_instance],(unsigned char)my_temp_point[1],(unsigned char)my_temp_point[0]);
			}
			else if(package == 4)
				end_flag = true;
			memset(mycode + package*400 ,0,400);

			memcpy_s(mycode + package*400 ,400 ,my_temp_point,400);
			unsigned char * temp_point = (program_code[start_instance]) + package*400;
			memcpy_s((program_code[start_instance]) + package*400,400,my_temp_point,400);
			//program_code_length[start_instance] = 400;


			if(debug_item_show == DEBUG_SHOW_PROGRAM_DATA_ONLY)
			{
				CString temp_char;
				CString n_temp_print;
				char * temp_point;
				temp_point = (char *)Temp_CS.value;
				n_temp_print.Format(_T("prg_%d  pack_%d  receive:"),start_instance,package);
				for (int i = 0; i< len_value_type ; i++)
				{
					temp_char.Format(_T("%02x"),(unsigned char)*temp_point);
					temp_char.MakeUpper();
					temp_point ++;
					n_temp_print = n_temp_print + temp_char + _T(" ");
				}
				DFTrace(n_temp_print);
			}





			return READPROGRAMCODE_T3000;
		}
		break;
	case  READTIMESCHEDULE_T3000:
		{
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
			my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			if(block_length!=(WEEKLY_SCHEDULE_SIZE))
				return -1;
			memset(weeklt_time_schedule[start_instance],0,WEEKLY_SCHEDULE_SIZE);
			memcpy_s(weeklt_time_schedule[start_instance],WEEKLY_SCHEDULE_SIZE,my_temp_point,WEEKLY_SCHEDULE_SIZE);

			//copy the schedule day time to my own buffer.
			for (int j=0; j<9; j++)
			{
				for (int i=0; i<8; i++)
				{
					m_Schedual_Time_data.at(start_instance).Schedual_Day_Time[i][j].time_minutes = *(my_temp_point ++);
					m_Schedual_Time_data.at(start_instance).Schedual_Day_Time[i][j].time_hours = *(my_temp_point ++);
				}
			}

			return READTIMESCHEDULE_T3000;
		}
		break;
	case READANNUALSCHEDULE_T3000:
		{
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
			my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			if(block_length!=ANNUAL_CODE_SIZE)
				return -1;
			memset(&g_DayState[start_instance],0,ANNUAL_CODE_SIZE);
			memcpy_s(&g_DayState[start_instance],block_length,my_temp_point,block_length);


			return READANNUALSCHEDULE_T3000;
		}
		break;
	case  TIME_COMMAND:
		{
			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
			my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			if(block_length!=sizeof(Time_block_mini))
				return -1;
			Device_time.ti_sec = *(my_temp_point ++);
			Device_time.ti_min = *(my_temp_point ++);
			Device_time.ti_hour = *(my_temp_point ++);
			Device_time.dayofmonth = *(my_temp_point ++);
			Device_time.dayofweek = *(my_temp_point ++);
			Device_time.month = *(my_temp_point ++);
			Device_time.year = *(my_temp_point ++);



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
		}
		break;
	case READCONTROLLER_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_controller_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_controller_point);

			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_PID_COUNT)
				return -1;//超过长度了;

			if(end_instance == (BAC_PID_COUNT - 1))
				end_flag = true;

			for (i=start_instance; i<=end_instance; i++)
			{
				m_controller_data.at(i).input.number = *(my_temp_point++);
				m_controller_data.at(i).input.point_type = *(my_temp_point++);
				m_controller_data.at(i).input.panel = *(my_temp_point++);

				//这里先加卡关条件，目前暂时不支持 其他panel的Input
				//if(m_controller_data.at(i).input.number>=BAC_INPUT_ITEM_COUNT)
				//	m_controller_data.at(i).input.number = 0;
				//if(m_controller_data.at(i).input.panel != bac_gloab_panel )
				//	m_controller_data.at(i).input.panel = bac_gloab_panel;

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
				m_controller_data.at(i).sample_time = *(my_temp_point++);
				m_controller_data.at(i).prop_high = *(my_temp_point++);
				m_controller_data.at(i).proportional = *(my_temp_point++);
				m_controller_data.at(i).reset = *(my_temp_point++);
				m_controller_data.at(i).bias = *(my_temp_point++);
				m_controller_data.at(i).rate = *(my_temp_point++);
			}



			return READCONTROLLER_T3000;
		}
		break;
	case READSCREEN_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Control_group_point))!=0)
				return -1;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Control_group_point);

			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_SCREEN_COUNT)
				return -1;//超过长度了;
			if(end_instance == (BAC_SCREEN_COUNT - 1))
				end_flag = true;
			for (i=start_instance; i<=end_instance; i++)
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
		}
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
			if(start_instance >= BAC_MONITOR_COUNT)
				return -1;//超过长度了;
			if(end_instance == (BAC_MONITOR_COUNT - 1))
				end_flag = true;

			for (i=start_instance; i<=end_instance; i++)
			{
				if(strlen(my_temp_point) > STR_MONITOR_LABEL_LENGTH)
					memset(m_monitor_data.at(i).label,0,STR_MONITOR_LABEL_LENGTH);
				else
					memcpy_s( m_monitor_data.at(i).label,STR_MONITOR_LABEL_LENGTH,my_temp_point,STR_MONITOR_LABEL_LENGTH);
				my_temp_point=my_temp_point + STR_MONITOR_LABEL_LENGTH;

				for (int j=0; j<MAX_POINTS_IN_MONITOR; j++)
				{
					m_monitor_data.at(i).inputs[j].number = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].point_type = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].panel = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].sub_panel = *(my_temp_point++);
					m_monitor_data.at(i).inputs[j].network = *(my_temp_point++);
				}
				for (int k=0; k<MAX_POINTS_IN_MONITOR; k++)
				{
					m_monitor_data.at(i).range[k] = *(my_temp_point++);
				}
				m_monitor_data.at(i).second_interval_time = *(my_temp_point++);
				m_monitor_data.at(i).minute_interval_time = *(my_temp_point++);
				m_monitor_data.at(i).hour_interval_time   = *(my_temp_point++);
				m_monitor_data.at(i).max_time_length = *(my_temp_point++);
				m_monitor_data.at(i).num_inputs = *(my_temp_point++);
				m_monitor_data.at(i).an_inputs = *(my_temp_point++);
				m_monitor_data.at(i).status= *(my_temp_point++);
				m_monitor_data.at(i).next_sample_time = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point + 4;
			}
		}
		return READMONITOR_T3000;
		break;
	case  READALARM_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Alarm_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Alarm_point);

			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(end_instance == (BAC_ALARMLOG_COUNT - 1))
				end_flag = true;

			if(start_instance >= BAC_ALARMLOG_COUNT)
				return -1;//超过长度了;
			for (int i=start_instance; i<=end_instance; i++)
			{
				m_alarmlog_data.at(i).point.number = *(my_temp_point++);
				m_alarmlog_data.at(i).point.point_type = *(my_temp_point++);
				m_alarmlog_data.at(i).point.panel = *(my_temp_point++);
				temp_struct_value = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
				m_alarmlog_data.at(i).point.network = temp_struct_value;
				my_temp_point = my_temp_point + 2;
				m_alarmlog_data.at(i).modem = *(my_temp_point++);
				m_alarmlog_data.at(i).printer = *(my_temp_point++);
				m_alarmlog_data.at(i).alarm =  *(my_temp_point++);

				//if one of the alarm is not zero ,show the alarm window.
				bac_show_alarm_window = bac_show_alarm_window || m_alarmlog_data.at(start_instance).alarm;

				m_alarmlog_data.at(i).restored =  *(my_temp_point++);
				m_alarmlog_data.at(i).acknowledged =  *(my_temp_point++);
				m_alarmlog_data.at(i).ddelete =  *(my_temp_point++);
				m_alarmlog_data.at(i).type =  *(my_temp_point++);
				m_alarmlog_data.at(i).cond_type =  *(my_temp_point++);
				m_alarmlog_data.at(i).level =  *(my_temp_point++);

				temp_struct_value = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				m_alarmlog_data.at(i).alarm_time =(unsigned int) temp_struct_value;
				my_temp_point = my_temp_point + 4;
				m_alarmlog_data.at(i).alarm_count =  *(my_temp_point++);


				if(strlen(my_temp_point) > ALARM_MESSAGE_SIZE)
					memset(&m_alarmlog_data.at(i).alarm_message,0,ALARM_MESSAGE_SIZE + 1);
				else
					memcpy_s( &m_alarmlog_data.at(i).alarm_message,ALARM_MESSAGE_SIZE + 1,my_temp_point,ALARM_MESSAGE_SIZE + 1);
				my_temp_point=my_temp_point + ALARM_MESSAGE_SIZE + 1;

				my_temp_point = my_temp_point + 5;//ignore char  none[5];

				m_alarmlog_data.at(i).panel_type =  *(my_temp_point++);
				m_alarmlog_data.at(i).dest_panel_type =  *(my_temp_point++);

				temp_struct_value = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
				m_alarmlog_data.at(i).alarm_id = (unsigned short)temp_struct_value;
				my_temp_point = my_temp_point + 2;
				m_alarmlog_data.at(i).prg =  *(my_temp_point++);


				m_alarmlog_data.at(i).alarm_panel =  *(my_temp_point++);
				m_alarmlog_data.at(i).where1 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where2 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where3 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where4 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where5 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where_state1 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where_state2 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where_state3 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where_state4 =  *(my_temp_point++);
				m_alarmlog_data.at(i).where_state5 =  *(my_temp_point++);
				m_alarmlog_data.at(i).change_flag =  *(my_temp_point++);
				m_alarmlog_data.at(i).original =  *(my_temp_point++);
				m_alarmlog_data.at(i).no =  *(my_temp_point++);
			}
			Sleep(1);
		}
		return READALARM_T3000;
		break;
	case READ_MISC:
		{
			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
			my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			if(block_length!=sizeof(Str_MISC))
				return -1;
			Device_Misc_Data.reg.flag[0] = *(my_temp_point++);
			Device_Misc_Data.reg.flag[1] = *(my_temp_point++);
			if((Device_Misc_Data.reg.flag[0]!= 0x55) || (Device_Misc_Data.reg.flag[1] != 0xff))
				return -1;
			for (int z=0; z<12; z++)
			{
				Device_Misc_Data.reg.monitor_analog_block_num[z] = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  4;
				Device_Misc_Data.reg.monitor_digital_block_num[z] = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  4;
			}

			for (int j=0;j<12;j++)
			{
				Device_Misc_Data.reg.operation_time[j] = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  4;
				if((Device_Misc_Data.reg.operation_time[j]< 1450774486) || (Device_Misc_Data.reg.operation_time[j] > 1505939286))
				{
					Device_Misc_Data.reg.operation_time[j] = 0;
				}
			}

			Device_Misc_Data.reg.flag1 = *(my_temp_point++);
			if(Device_Misc_Data.reg.flag1 != 0x55)
			{
				return -1;
			}
			for (int z=0;z<3;z++)
			{
				Device_Misc_Data.reg.com_rx[z] = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  4;
			}

			for (int z=0;z<3;z++)
			{
				Device_Misc_Data.reg.com_tx[z] = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  4;
			}

			for (int z=0;z<3;z++)
			{
				Device_Misc_Data.reg.collision[z] = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  2;
			}

			for (int z=0;z<3;z++)
			{
				Device_Misc_Data.reg.packet_error[z] = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  2;
			}

			for (int z=0;z<3;z++)
			{
				Device_Misc_Data.reg.timeout[z] = ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point  +  2;
			}

		}
		break;
	case READ_SETTING_COMMAND:
		{
			block_length = len_value_type - PRIVATE_HEAD_LENGTH;//Program code length  =  total -  head;
			my_temp_point = (char *)Temp_CS.value + PRIVATE_HEAD_LENGTH;
			if(block_length!=sizeof(Str_Setting_Info))
				return -1;

			memcpy_s(Device_Basic_Setting.reg.ip_addr,4,my_temp_point,4);
			my_temp_point = my_temp_point + 4;
			memcpy_s(Device_Basic_Setting.reg.subnet,4,my_temp_point,4);
			my_temp_point = my_temp_point + 4;
			memcpy_s(Device_Basic_Setting.reg.gate_addr,4,my_temp_point,4);
			my_temp_point = my_temp_point + 4;
			memcpy_s(Device_Basic_Setting.reg.mac_addr,6,my_temp_point,6);
			my_temp_point = my_temp_point + 6;
			Device_Basic_Setting.reg.tcp_type = *(my_temp_point++);
			Device_Basic_Setting.reg.mini_type = *(my_temp_point++);
			if(Device_Basic_Setting.reg.mini_type == BIG_MINIPANEL)
				bacnet_device_type = BIG_MINIPANEL;
			else if(Device_Basic_Setting.reg.mini_type == SMALL_MINIPANEL)
				bacnet_device_type = SMALL_MINIPANEL;
			else if(Device_Basic_Setting.reg.mini_type == TINY_MINIPANEL)
				bacnet_device_type = TINY_MINIPANEL;
			else if (Device_Basic_Setting.reg.mini_type == TINY_EX_MINIPANEL)
				bacnet_device_type = TINY_EX_MINIPANEL;
			else
				bacnet_device_type = PRODUCT_CM5;
			my_temp_point = my_temp_point + 1;	//中间 minitype  和 debug  没什么用;
			Device_Basic_Setting.reg.pro_info.harware_rev = *(my_temp_point++);
			Device_Basic_Setting.reg.pro_info.firmware0_rev_main = *(my_temp_point++);
			Device_Basic_Setting.reg.pro_info.firmware0_rev_sub = *(my_temp_point++);

			Device_Basic_Setting.reg.pro_info.frimware1_rev = *(my_temp_point++);
			Device_Basic_Setting.reg.pro_info.frimware2_rev = *(my_temp_point++);
			Device_Basic_Setting.reg.pro_info.frimware3_rev = *(my_temp_point++);
			Device_Basic_Setting.reg.pro_info.bootloader_rev = *(my_temp_point++);
			my_temp_point = my_temp_point + 10;
			Device_Basic_Setting.reg.com0_config = *(my_temp_point++);
			Device_Basic_Setting.reg.com1_config = *(my_temp_point++);
			Device_Basic_Setting.reg.com2_config = *(my_temp_point++);
			Device_Basic_Setting.reg.refresh_flash_timer =  *(my_temp_point++);
			Device_Basic_Setting.reg.en_plug_n_play =  *(my_temp_point++);
			Device_Basic_Setting.reg.reset_default = *(my_temp_point++);

			Device_Basic_Setting.reg.com_baudrate0 = *(my_temp_point++);
			Device_Basic_Setting.reg.com_baudrate1 = *(my_temp_point++);
			Device_Basic_Setting.reg.com_baudrate2 = *(my_temp_point++);

			Device_Basic_Setting.reg.user_name = *(my_temp_point++);
			Device_Basic_Setting.reg.custmer_unite = *(my_temp_point++);

			Device_Basic_Setting.reg.usb_mode = *(my_temp_point++);
			Device_Basic_Setting.reg.network_number = *(my_temp_point++);
			Device_Basic_Setting.reg.panel_type = *(my_temp_point++);
			memcpy_s(Device_Basic_Setting.reg.panel_name,20,my_temp_point,20);
			my_temp_point = my_temp_point + 20;
			Device_Basic_Setting.reg.en_panel_name = *(my_temp_point++);
			Device_Basic_Setting.reg.panel_number = *(my_temp_point++);


			uint8_t en_dyndns;  // 0 - no  1 - disable 2 - enable
			uint8_t en_sntp;  // 0 - no  1 - disable
			memcpy_s(Device_Basic_Setting.reg.dyndns_user,DYNDNS_MAX_USERNAME_SIZE,my_temp_point,DYNDNS_MAX_USERNAME_SIZE);
			my_temp_point = my_temp_point + DYNDNS_MAX_USERNAME_SIZE;
			memcpy_s(Device_Basic_Setting.reg.dyndns_pass,DYNDNS_MAX_PASSWORD_SIZE,my_temp_point,DYNDNS_MAX_PASSWORD_SIZE);
			my_temp_point = my_temp_point + DYNDNS_MAX_PASSWORD_SIZE;
			memcpy_s(Device_Basic_Setting.reg.dyndns_domain,DYNDNS_MAX_DOMAIN_SIZE,my_temp_point,DYNDNS_MAX_DOMAIN_SIZE);
			my_temp_point = my_temp_point + DYNDNS_MAX_DOMAIN_SIZE;
			Device_Basic_Setting.reg.en_dyndns = *(my_temp_point++);
			Device_Basic_Setting.reg.dyndns_provider = *(my_temp_point++);
			Device_Basic_Setting.reg.dyndns_update_time = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
			my_temp_point = my_temp_point + 2;
			Device_Basic_Setting.reg.en_sntp = *(my_temp_point++);
			Device_Basic_Setting.reg.time_zone = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
			my_temp_point = my_temp_point + 2;
			Device_Basic_Setting.reg.n_serial_number = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			my_temp_point = my_temp_point + 4;

			//CString test_serial_number;
			//test_serial_number.Format(_T("Read Setting %u"),Device_Basic_Setting.reg.n_serial_number);
			//DFTrace(test_serial_number);
			memcpy_s(&Device_Basic_Setting.reg.update_dyndns,UN_TIME_LENGTH,my_temp_point,UN_TIME_LENGTH);
			my_temp_point = my_temp_point + UN_TIME_LENGTH;

			Device_Basic_Setting.reg.mstp_network_number = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
			my_temp_point = my_temp_point + 2;
			Device_Basic_Setting.reg.BBMD_EN = *(my_temp_point++);
			Device_Basic_Setting.reg.sd_exist = *(my_temp_point++);
			Device_Basic_Setting.reg.modbus_port = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
			my_temp_point = my_temp_point + 2;
			Device_Basic_Setting.reg.modbus_id = *(my_temp_point++);
			Device_Basic_Setting.reg.object_instance = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			my_temp_point = my_temp_point + 4;
			Device_Basic_Setting.reg.time_update_since_1970 = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			my_temp_point = my_temp_point + 4;
			Device_Basic_Setting.reg.time_zone_summer_daytime = *(my_temp_point++);
			memcpy_s(Device_Basic_Setting.reg.sntp_server,30,my_temp_point,30);
			my_temp_point = my_temp_point + 30;
			Device_Basic_Setting.reg.zegbee_exsit = *(my_temp_point++);
			return READ_SETTING_COMMAND;
		}
		break;
	case READMONITORDATA_T3000:
		{
			handle_read_monitordata_ex((char *)Temp_CS.value,len_value_type);
			return READMONITORDATA_T3000;
		}
		break;
	case READ_REMOTE_DEVICE_DB:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_Remote_TstDB))!=0)
				return -1;
			m_remote_device_db.clear();

			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_Remote_TstDB);
			if(block_length == 0)
				break;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			for (int x=0; x<block_length; x++)
			{
				Str_Remote_TstDB temp;
				temp.sn = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				my_temp_point = my_temp_point + 4;
				temp.product_type = *(my_temp_point++);
				temp.modbus_id = *(my_temp_point++);
				memcpy_s(temp.ip_addr,4,my_temp_point,4);
				my_temp_point = my_temp_point + 4;
				temp.port =  ((unsigned char)my_temp_point[0]<<8) | ((unsigned char)my_temp_point[1]);
				my_temp_point = my_temp_point + 2;
				memcpy_s(temp.reserved,10,my_temp_point,10);
				my_temp_point = my_temp_point + 10;

				if((temp.product_type == 0) || (temp.modbus_id == 0) || (temp.port == 0) || (temp.sn == 0 ))  //下面挂的不符合规则;
					continue;
				//m_remote_device_db.at(x).sn = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
				//my_temp_point = my_temp_point + 4;
				//m_remote_device_db.at(x).product_type = *(my_temp_point++);
				//m_remote_device_db.at(x).modbus_id = *(my_temp_point++);
				//memcpy_s(m_remote_device_db.at(x).ip_addr,4,my_temp_point,4);
				//my_temp_point = my_temp_point + 4;
				//m_remote_device_db.at(x).port =  ((unsigned char)my_temp_point[0]<<8) | ((unsigned char)my_temp_point[1]);
				//my_temp_point = my_temp_point + 2;
				//memcpy_s(m_remote_device_db.at(x).reserved,10,my_temp_point,10);
				//my_temp_point = my_temp_point + 10;

				m_remote_device_db.push_back(temp);
			}
		}
		break;
	case GETSERIALNUMBERINFO:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_Serial_info))!=0)
				return -1;

			unsigned short device_id_low = 0;
			unsigned short device_id_high = 0;
			_Bac_Scan_results_Info temp_struct;
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;
			device_id_low	= ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);

			my_temp_point = my_temp_point +2;
			memcpy_s(temp_struct.ipaddress,6,my_temp_point,6);

			//  temp_struct.panel_number = *(my_temp_point + 3);//Notice
			// temp_struct.macaddress = *my_temp_point;
			my_temp_point = my_temp_point + 6;
			temp_struct.serialnumber = ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
			my_temp_point = my_temp_point + 4;
			temp_struct.modbus_addr =  *(my_temp_point++);
			temp_struct.product_type =  *(my_temp_point++);
			temp_struct.panel_number =  *(my_temp_point++);
			temp_struct.modbus_port = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
			my_temp_point = my_temp_point + 2;
			temp_struct.software_version = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);
			my_temp_point = my_temp_point + 2;
			temp_struct.hardware_version =  *(my_temp_point++);
			temp_struct.m_protocol = *(my_temp_point++);

			device_id_high = ((unsigned char)my_temp_point[1]<<8) | ((unsigned char)my_temp_point[0]);

			temp_struct.device_id = ((unsigned int)device_id_high) *65536 + device_id_low;
			int find_exsit = false;
			TRACE(_T("serialnumber = %d ,modbus_addr = %d , product_type = %d ,ip = %u.%u.%u.%u , instance = %d\r\n"),temp_struct.serialnumber,
				temp_struct.modbus_addr,temp_struct.product_type,(unsigned char)temp_struct.ipaddress[0],(unsigned char)temp_struct.ipaddress[1] ,
				(unsigned char)temp_struct.ipaddress[2],(unsigned char)temp_struct.ipaddress[3],temp_struct.device_id);
			for (int x=0; x<(int)m_bac_scan_result_data.size(); x++)
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
	case READTSTAT_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_TstatInfo_point))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_TstatInfo_point);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_TSTAT_COUNT)
				return -1;//超过长度了;

			if(end_instance == (BAC_TSTAT_COUNT - 1))
				end_flag = true;
			for (i=start_instance; i<=end_instance; i++)
			{
				m_Tstat_data.at(i).product_model = *(my_temp_point++);
				m_Tstat_data.at(i).temperature = ((unsigned char)my_temp_point[0]<<8) | ((unsigned char)my_temp_point[1]);
				my_temp_point = my_temp_point + 2;
				m_Tstat_data.at(i).mode = *(my_temp_point++);
				m_Tstat_data.at(i).cool_heat_mode = *(my_temp_point++);
				m_Tstat_data.at(i).setpoint =  ((unsigned char)my_temp_point[0]<<8) | ((unsigned char)my_temp_point[1]);
				my_temp_point = my_temp_point + 2;
				m_Tstat_data.at(i).cool_setpoint =  ((unsigned char)my_temp_point[0]<<8) | ((unsigned char)my_temp_point[1]);
				my_temp_point = my_temp_point + 2;
				m_Tstat_data.at(i).heat_setpoint =  ((unsigned char)my_temp_point[0]<<8) | ((unsigned char)my_temp_point[1]);
				my_temp_point = my_temp_point + 2;
				m_Tstat_data.at(i).occupied = *(my_temp_point++);
				m_Tstat_data.at(i).output_state = *(my_temp_point++);

				m_Tstat_data.at(i).night_heat_db = *(my_temp_point++);
				m_Tstat_data.at(i).night_cool_db = *(my_temp_point++);
				m_Tstat_data.at(i).night_heat_sp = *(my_temp_point++);
				m_Tstat_data.at(i).night_cool_sp = *(my_temp_point++);
				m_Tstat_data.at(i).over_ride = *(my_temp_point++);
				m_Tstat_data.at(i).tst_db.id = *(my_temp_point++);
				m_Tstat_data.at(i).tst_db.sn = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
				my_temp_point = my_temp_point + 4;
				m_Tstat_data.at(i).tst_db.port = *(my_temp_point++);
				m_Tstat_data.at(i).type = *(my_temp_point++);
			}

		}
		break;
	case READUNIT_T3000:
		{
			if((len_value_type - PRIVATE_HEAD_LENGTH)%(sizeof(Str_Units_element))!=0)
				return -1;	//得到的结构长度错误;
			block_length=(len_value_type - PRIVATE_HEAD_LENGTH)/sizeof(Str_Units_element);
			my_temp_point = (char *)Temp_CS.value + 3;
			start_instance = *my_temp_point;
			my_temp_point++;
			end_instance = *my_temp_point;
			my_temp_point++;
			my_temp_point = my_temp_point + 2;

			if(start_instance >= BAC_CUSTOMER_UNITS_COUNT)
				return -1;//超过长度了;
			if(end_instance == (BAC_CUSTOMER_UNITS_COUNT - 1))
			{
				end_flag = true;
				receive_customer_unit = true;
			}
			for (i=start_instance; i<=end_instance; i++)
			{
				m_customer_unit_data.at(i).direct = *(my_temp_point++);
				memcpy_s(m_customer_unit_data.at(i).digital_units_off,12,my_temp_point,12);
				my_temp_point = my_temp_point + 12;
				memcpy_s(m_customer_unit_data.at(i).digital_units_on,12,my_temp_point,12);
				my_temp_point = my_temp_point + 12;


				MultiByteToWideChar( CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_off, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_off)+1,
					temp_off[i].GetBuffer(MAX_PATH), MAX_PATH );
				temp_off[i].ReleaseBuffer();
				if(temp_off[i].GetLength() >= 12)
					temp_off[i].Empty();

				MultiByteToWideChar( CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_on, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_on)+1,
					temp_on[i].GetBuffer(MAX_PATH), MAX_PATH );
				temp_on[i].ReleaseBuffer();
				if(temp_on[i].GetLength() >= 12)
					temp_on[i].Empty();

				temp_unit_no_index[i] = temp_off[i] + _T("/") + temp_on[i];

			}

		}
		break;
	case READPIC_T3000:
		{
			handle_read_pic_data_ex((char *)Temp_CS.value,len_value_type);
			return READMONITORDATA_T3000;
		}
		break;
	}
    return 1;
}

int handle_read_pic_data_ex(char *npoint,int nlength)
{



	char * my_temp_point = npoint;
	char * temp_print = npoint;

	m_picture_head.total_seg = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
	my_temp_point = my_temp_point + 3;
	m_picture_head.index = *(my_temp_point++);
	my_temp_point = my_temp_point + 14;
	m_picture_head.seg_index = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
	my_temp_point = my_temp_point + 2;





	if(nlength == 420)
	{
		memcpy(picture_data_buffer,my_temp_point,400);

		if(debug_item_show == DEBUG_SHOW_WRITE_PIC_DATA_ONLY)
		{
			CString temp_char;
			CString n_temp_print;
			char * temp_point;
			temp_point = npoint;
			n_temp_print.Format(_T("picture_%d  pack %d %d  read_:"),m_picture_head.index,m_picture_head.seg_index,m_picture_head.total_seg);
			for (int i = 0; i< 420 ; i++)
			{
				temp_char.Format(_T("%02x"),(unsigned char)*temp_point);
				temp_char.MakeUpper();
				temp_point ++;
				n_temp_print = n_temp_print + temp_char + _T(" ");

				if(i==19)
					n_temp_print = n_temp_print + _T(" Data: ");
			}
			DFTrace(n_temp_print);
		}


		return 1;
	}
	return 0;
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
		return;
#if PRINT_ENABLED
        printf("cpta: Bad Encoding!\n");
#endif
    }
    int receive_data_type;
    bool each_end_flag = false;
    receive_data_type = Bacnet_PrivateData_Handle(&data,each_end_flag);
    if(receive_data_type < 0)
    {
        g_llerrCount ++;
        return;
    }
	bac_select_device_online = true;
    switch(receive_data_type)
    {
	case READEXT_IO_T3000:
		{
			 if(each_end_flag)
			 {
				 ::PostMessage(m_ext_io_dlg_hwmd,WM_REFRESH_BAC_EXTIO_LIST,NULL,NULL);
			 }
		}
		break;
    case READ_REMOTE_POINT:
    {
        if(each_end_flag)
        {
            if(pDialog[WINDOW_REMOTE_POINT]->IsWindowEnabled())
                ::PostMessage(m_remote_point_hwnd,WM_REFRESH_BAC_REMOTE_POINT_LIST,NULL,NULL);
        }
    }
    break;
    case READANALOG_CUS_TABLE_T3000:
    {
        if(analog_cus_range_dlg!=NULL)
            ::PostMessage(analog_cus_range_dlg,WM_REFRESH_BAC_ANALOGCUSRANGE_LIST,NULL,NULL);
    }
    break;
    case READ_AT_COMMAND:
    {
        ::PostMessage(m_at_command_hwnd,WM_REFRESH_BAC_AT_COMMAND,NULL,NULL);
    }
    break;
    case READINPUT_T3000:
        if(each_end_flag)
        {
            if(pDialog[WINDOW_INPUT]->IsWindowVisible())
                ::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
            else
                TRACE(_T("Input window not visiable ,don't refresh\r\n"));
        }
        copy_data_to_ptrpanel(TYPE_INPUT);
        break;
    case READPROGRAM_T3000:
        if(each_end_flag)
        {
            if(pDialog[WINDOW_PROGRAM]->IsWindowVisible())
                ::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
        }
        copy_data_to_ptrpanel(TYPE_ALL);
        break;
    case READPROGRAMCODE_T3000:
        break;
    case READVARIABLE_T3000:
        if(each_end_flag)
        {
            if(pDialog[WINDOW_VARIABLE]->IsWindowVisible())
                ::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
        }
        copy_data_to_ptrpanel(TYPE_VARIABLE);
        break;
    case READOUTPUT_T3000:
        if(each_end_flag)
        {
            if(pDialog[WINDOW_OUTPUT]->IsWindowVisible())
                ::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
        }
        copy_data_to_ptrpanel(TYPE_OUTPUT);
        break;
    case READWEEKLYROUTINE_T3000:
        if(each_end_flag)
            ::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
        copy_data_to_ptrpanel(TYPE_WEEKLY);
        break;
    case READANNUALROUTINE_T3000:
        ::PostMessage(m_annual_dlg_hwnd,WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
        copy_data_to_ptrpanel(TYPE_ANNUAL);
        break;
    case READTIMESCHEDULE_T3000:
        ::PostMessage(m_schedule_time_dlg_hwnd,WM_REFRESH_BAC_SCHEDULE_LIST,NULL,NULL);
        break;
    case TIME_COMMAND:
        ::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,TIME_COMMAND,NULL);
        break;
    case READANNUALSCHEDULE_T3000:
        ::PostMessage(m_schedule_day_dlg_hwnd,WM_REFRESH_BAC_DAY_CAL,NULL,NULL);
        break;
    case READCONTROLLER_T3000:
        if(each_end_flag)
            ::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
        copy_data_to_ptrpanel(TYPE_ALL);
        break;
    case READSCREEN_T3000:
        if(each_end_flag)
            ::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
        copy_data_to_ptrpanel(TYPE_ALL);
        break;
    case READALARM_T3000:
        if(each_end_flag)
            ::PostMessage(m_alarmlog_dlg_hwnd,WM_REFRESH_BAC_ALARMLOG_LIST,NULL,NULL);
        break;
    case READ_SETTING_COMMAND:
        ::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
        break;
    case READTSTAT_T3000:
        //if(each_end_flag)
        //	::PostMessage(m_tstat_dlg_hwnd,WM_REFRESH_BAC_TSTAT_LIST,NULL,NULL);
        break;
    case READMONITOR_T3000:
    {
        if(each_end_flag)
        {
            ::PostMessage(m_monitor_dlg_hwnd,WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
            ::PostMessage(m_monitor_dlg_hwnd,WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
        }
    }
    break;
    default:
        break;
    }

    if((each_end_flag) && (bac_read_which_list != BAC_READ_ALL_LIST) && (bac_read_which_list != BAC_READ_SVAE_CONFIG))
    {
        CString temp_file;
        CString temp_serial;
        temp_serial.Format(_T("%u.prg"),g_selected_serialnumber);
        temp_file = g_achive_folder + _T("\\") + temp_serial;
        SaveBacnetConfigFile_Cache(temp_file);
        //TRACE(_T("Save config file cache\r\n"));
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


void Inial_Product_map()
{
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5A,_T("TStat5A")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5B,_T("TStat5B")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5B2,_T("TStat5B2")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5C,_T("TStat5C")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5D,_T("TStat5D")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5E,_T("TStat5E")));


	product_map.insert(map<int,CString>::value_type(PM_PM5E,_T("PM5E")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5F,_T("TStat5F")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5G,_T("TStat5G")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5H,_T("TStat5H")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT6,_T("TStat6")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT5i,_T("TStat5i")));

	product_map.insert(map<int,CString>::value_type(PM_TSTAT8,_T("TStat8")));
	product_map.insert(map<int,CString>::value_type(PM_HUMTEMPSENSOR,_T("HUM Sensor")));
	product_map.insert(map<int,CString>::value_type(STM32_HUM_NET,_T("HUM Sensor")));
	product_map.insert(map<int,CString>::value_type(STM32_HUM_RS485,_T("HUM Sensor")));
	product_map.insert(map<int,CString>::value_type(PM_AirQuality,_T("Air Quality")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT7,_T("TStat7")));

	product_map.insert(map<int,CString>::value_type(PM_NC,_T("NC")));
	product_map.insert(map<int,CString>::value_type(PM_CM5,_T("CM5")));
	product_map.insert(map<int,CString>::value_type(PM_TSTATRUNAR,_T("TStatRunar")));
	product_map.insert(map<int,CString>::value_type(PM_LightingController,_T("LC")));
	product_map.insert(map<int,CString>::value_type(PM_CO2_NET,_T("CO2 Net")));
	product_map.insert(map<int,CString>::value_type(PM_CO2_RS485,_T("CO2")));

	product_map.insert(map<int,CString>::value_type(PM_PRESSURE_SENSOR,_T("Pressure")));
	product_map.insert(map<int,CString>::value_type(PM_CO2_NODE,_T("CO2 Node")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT6_HUM_Chamber,_T("HumChamber")));
	product_map.insert(map<int,CString>::value_type(PM_T3PT10,_T("T3-PT10")));
	product_map.insert(map<int,CString>::value_type(PM_T3IOA,_T("T3-8O")));
	product_map.insert(map<int,CString>::value_type(PM_T332AI,_T("T3-32AI")));

	product_map.insert(map<int,CString>::value_type(PM_T38AI16O,_T("T3-8AI160")));
	product_map.insert(map<int,CString>::value_type(PM_T38I13O,_T("T3-8I13O")));
	product_map.insert(map<int,CString>::value_type(PM_T3PERFORMANCE,_T("T3-Performance")));
	product_map.insert(map<int,CString>::value_type(PM_T34AO,_T("T3-4AO")));
	product_map.insert(map<int,CString>::value_type(PM_T36CT,_T("T3-6CT")));
	product_map.insert(map<int,CString>::value_type(PM_MINIPANEL,_T("T3-BB/LB/TB")));

	product_map.insert(map<int,CString>::value_type(PM_PRESSURE,_T("Pressure Sensor")));
	product_map.insert(map<int,CString>::value_type(PM_HUM_R,_T("HUM-R")));
	product_map.insert(map<int,CString>::value_type(PM_T322AI,_T("T3-22I")));
	product_map.insert(map<int,CString>::value_type(PWM_TRANSDUCER,_T("PWM_Tranducer")));
	product_map.insert(map<int,CString>::value_type(PM_BTU_METER,_T("BTU_Meter")));
	product_map.insert(map<int,CString>::value_type(PM_T3PT12,_T("T3-PT12")));
	product_map.insert(map<int,CString>::value_type(PM_T36CTA,_T("T3-6CTA")));
	product_map.insert(map<int,CString>::value_type(PM_T38AI8AO6DO,_T("T3-8AI8AO6DO")));
	product_map.insert(map<int,CString>::value_type(PM_CS_SM_AC,_T("CS-SM-AC")));
	product_map.insert(map<int,CString>::value_type(PM_CS_SM_DC,_T("CS-SM-DC")));
	product_map.insert(map<int,CString>::value_type(PM_CS_RSM_AC,_T("CS-RSM-AC")));
	product_map.insert(map<int,CString>::value_type(PM_CS_RSM_DC,_T("CS-RSM-DC")));


	product_map.insert(map<int,CString>::value_type(PM_TSTAT8_WIFI,_T("TStat8_Wifi")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT8_OCC,_T("TStat8_Occ")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT7_ARM,_T("TStat7_ARM")));
	product_map.insert(map<int,CString>::value_type(PM_TSTAT8_220V,_T("TStat8_220V")));
	product_map.insert(map<int, CString>::value_type(PM_T3_LC, _T("T3_LC")));


}


CString GetProductName(int ModelID)
{
	/*map <int,CString>::iterator myiterator;

	myiterator =product_map.find(ModelID);
	if(myiterator != product_map.end())
	{
		return myiterator->second;
	}
	else
	{
		 return _T("");	
	}*/
	return product_map[ModelID];
	  //_T("");	
}


CString Get_Table_Name(int SerialNo,CString Type ,int Row)
{
    
    CString Table_Name;
    CppSQLite3DB SqliteDBBuilding;
    SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	CppSQLite3Table table;
	CppSQLite3Query q;
    if (SqliteDBBuilding.tableExists("IONAME_CONFIG"))//有Version表
    {
        CString sql;
        sql.Format(_T("Select * from IONAME_CONFIG where Type='%s' and  Row=%d and SerialNo=%d"),Type.GetBuffer(),Row,SerialNo);
        q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
        if (!q.eof())//有表但是没有对应序列号的值
        {
            
            while (!q.eof())
            {
                 
				Table_Name=q.getValuebyName(L"InOutName");
                q.nextRow();
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
    SqliteDBBuilding.closedb();
    return Table_Name;
}
void    Insert_Update_Table_Name(int SerialNo,CString Type,int Row,CString TableName)
{
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
    CString sql;
    sql.Format(_T("Select * from IONAME_CONFIG where Type='%s' and  Row=%d and SerialNo=%d"),Type.GetBuffer(),Row,SerialNo);
    q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

    if (!q.eof())//有表但是没有对应序列号的值
    {

        sql.Format(_T("update IONAME_CONFIG set InOutName = '%s' where Type='%s' and  Row=%d and SerialNo=%d "),TableName.GetBuffer(),Type.GetBuffer(),Row,SerialNo);
        SqliteDBBuilding.execDML((UTF8MBSTR)sql);
    }
    else
    {
         
        sql.Format(_T("Insert into IONAME_CONFIG(InOutName,Type,Row,SerialNo) values('%s','%s','%d','%d')"),TableName.GetBuffer(),Type.GetBuffer(),Row,SerialNo);
         SqliteDBBuilding.execDML((UTF8MBSTR)sql);
    }
	SqliteDBBuilding.closedb();
}

int Get_Unit_Process(CString Unit)
{
    int ret_Value=1;
    if (Unit.CompareNoCase(_T("RAW DATA"))==0)
    {
        ret_Value=1;
    }
    else if (Unit.CompareNoCase(_T("TYPE2 10K C"))==0)
    {
        ret_Value=10;
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


BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position)
{

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
    if( hours < 10 )
    {
        strcpy(textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
    }
    itoa(hours,textbuffer,10) ;
    textbuffer += strlen(textbuffer);
    strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/

    if( minutes < 10 )
    {
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
    if( hours < 10 )
    {
        strcpy(textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
    }
    itoa(hours,textbuffer,10) ;
    textbuffer += strlen(textbuffer);
    strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/

    if( minutes < 10 )
    {
        strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:0#:## */
    }
    itoa(minutes,textbuffer,10) ;
    textbuffer += strlen(textbuffer);
    strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/
    if( seconds < 10 )
    {
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
    }
    else
        fprintf(stderr, "!\n");
#endif
    address_add(device_id, max_apdu, src);



    //g_bac_instance =device_id;
#if 1
    if(src->mac_len==6)
    {
        bac_cs_mac.Format(_T("%d"),src->mac[3]);
    }
    else if(src->mac_len==1)
        bac_cs_mac.Format(_T("%d"),src->mac[0]);
    else
        return;
#endif
    //	bac_cs_mac.Format(_T("%d"),vendor_id);

    bac_cs_device_id.Format(_T("%d"),device_id);

	if((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
	{
		g_Print.Format(_T("Find object instance %s , panel %s "),bac_cs_device_id, bac_cs_mac );
		DFTrace(g_Print);
	}

    //TRACE(_T("Find ") + bac_cs_device_id +_T("  ") + bac_cs_mac + _T("\r\n"));

    //g_Print = _T("Globle Who is Find ") + bac_cs_device_id +_T("  ") + bac_cs_mac;
    //DFTrace(g_Print);
    _Bac_Scan_Com_Info temp_1;
    temp_1.device_id = device_id;
    //	temp_1.vendor_id = vendor_id;
    temp_1.macaddress = _wtoi(bac_cs_mac);

    int find_exsit = false;
    for (int i=0; i<(int)m_bac_scan_com_data.size(); i++)
    {
        if((m_bac_scan_com_data.at(i).device_id == temp_1.device_id)
                && (m_bac_scan_com_data.at(i).macaddress == temp_1.macaddress))
        {
            find_exsit = true;
        }
    }

    if(!find_exsit)
    {
        m_bac_scan_com_data.push_back(temp_1);
    }

    ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
    return;

}

SOCKET my_sokect;
extern void  init_info_table( void );
extern void Init_table_bank();
bool Initial_bac(int comport,CString bind_local_ip)
{

    BACNET_ADDRESS src =
    {
        0
    };  /* address where message came from */
    uint16_t pdu_len = 0;
    unsigned timeout = 100;     /* milliseconds */
    BACNET_ADDRESS my_address, broadcast_address;
    char my_port[50];

    bac_program_pool_size = 26624;
    bac_program_size = 0;
    bac_free_memory = 26624;
    //Device_Set_Object_Instance_Number(4194300);
    srand((unsigned)time(NULL));
    unsigned int temp_value;
    temp_value = rand()%(0x3FFFFF);
    g_Print.Format(_T("The initial T3000 Object Instance value is %d"),temp_value);
    //DFTrace(g_Print);
    Device_Set_Object_Instance_Number(temp_value);
    address_init();
    Init_Service_Handlers();



#ifndef test_ptp

#if 1
    if(comport == 0)	//
    {
#endif
        int ret_1 ;
        if(bind_local_ip.IsEmpty())
        {
            ret_1= Open_bacnetSocket2(_T(""),BACNETIP_PORT,my_sokect);
        }
        else
        {
            ret_1= Open_bacnetSocket2(bind_local_ip,BACNETIP_PORT,my_sokect);
        }

        if(ret_1 < 0)
            return false;
        //	Open_Socket2(_T("127.0.0.1"),6002);
        //	 = (int)GetCommunicationHandle();
        bip_set_socket(my_sokect);
        //bip_set_port(49338);
		bip_set_port(htons(47808));
		
        //	int test_port = bip_get_port();
#if 1
        static in_addr BIP_Broadcast_Address;
        BIP_Broadcast_Address.S_un.S_addr =  inet_addr("255.255.255.255");
        //BIP_Broadcast_Address.S_un.S_addr =  inet_addr("192.168.0.177");
        bip_set_broadcast_addr((uint32_t)BIP_Broadcast_Address.S_un.S_addr);
#endif
        PHOSTENT  hostinfo;
        char  name[255];
        CString  cs_myip;
        if(  gethostname  (  name,  sizeof(name))  ==  0)
        {
            if((hostinfo  =  gethostbyname(name))  !=  NULL)
            {
                cs_myip  =  inet_ntoa  (*(struct  in_addr  *)*hostinfo->h_addr_list);
            }
        }
        char cTemp1[255];
        memset(cTemp1,0,255);
        WideCharToMultiByte( CP_ACP, 0, cs_myip.GetBuffer(), -1, cTemp1, 255, NULL, NULL );


        if(bind_local_ip.IsEmpty())
        {
            static in_addr BIP_Address;
            BIP_Address.S_un.S_addr =  inet_addr(cTemp1);
            bip_set_addr((uint32_t)BIP_Address.S_un.S_addr);
        }
        else
        {
            static in_addr BIP_Address;
            char temp_ip_1[100];
            memset(temp_ip_1,0,100);
            WideCharToMultiByte( CP_ACP, 0, bind_local_ip.GetBuffer(), -1, temp_ip_1, 255, NULL, NULL );
            BIP_Address.S_un.S_addr =  inet_addr(temp_ip_1);
            bip_set_addr((uint32_t)BIP_Address.S_un.S_addr);
        }

#if 0
        static in_addr BIP_Address;
        BIP_Address.S_un.S_addr =  inet_addr(cTemp1);
        bip_set_addr((uint32_t)BIP_Address.S_un.S_addr);
#endif

        set_datalink_protocol(PROTOCOL_BACNET_IP);
        datalink_get_broadcast_address(&broadcast_address);
        //    print_address("Broadcast", &broadcast_address);
        datalink_get_my_address(&my_address);
        //		print_address("Address", &my_address);
        //int * comport_parameter = new int;
        //*comport_parameter = PROTOCOL_BACNET_IP;
        if(CM5_hThread!=NULL)
        {
            TerminateThread(CM5_hThread,0);
            CM5_hThread = NULL;
        }
        CM5_hThread =CreateThread(NULL,NULL,MSTP_Receive,NULL,NULL, &nThreadID_x);

#if 1
    }
    else
    {
        HANDLE temphandle;
        temphandle = Get_RS485_Handle();
        if(temphandle !=NULL)
        {
            TerminateThread((HANDLE)Get_Thread1(),0);
            TerminateThread((HANDLE)Get_Thread2(),0);

            CloseHandle(temphandle);
            Set_RS485_Handle(NULL);
        }
        close_com();

        dlmstp_set_baud_rate(38400);
        //		dlmstp_set_baud_rate(19200);
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

        set_datalink_protocol(MODBUS_BACNET_MSTP);
        datalink_get_broadcast_address(&broadcast_address);
        //    print_address("Broadcast", &broadcast_address);
        datalink_get_my_address(&my_address);
        //		print_address("Address", &my_address);
        int * comport_parameter = new int;
        *comport_parameter = MODBUS_BACNET_MSTP;
        if(CM5_hThread!=NULL)
        {
            TerminateThread(CM5_hThread,0);
            CM5_hThread = NULL;
        }
        CM5_hThread =CreateThread(NULL,NULL,MSTP_Receive,comport_parameter,NULL, &nThreadID_x);
    }
#endif

#endif //nodef ptp

    if(!bac_net_initial_once)
    {
        bac_net_initial_once =true;
        timesec1970 = (unsigned long)time(NULL);
        timestart = 0;
        init_info_table();
        Init_table_bank();
    }
    return true;
}
//#include "datalink.h"
DWORD WINAPI   MSTP_Receive(LPVOID lpVoid)
{
    BACNET_ADDRESS src = {0};
    uint16_t pdu_len;
    //int *mparent = (int *)lpVoid;
    //int protocol_new = *mparent;

    uint8_t Rx_Buf[MAX_MPDU] = { 0 };
    //while(mparent->m_MSTP_THREAD)
    g_mstp_flag=true;
    while(g_mstp_flag)
    {
        pdu_len = datalink_receive(&src,&Rx_Buf[0],MAX_MPDU,INFINITE);
        if(pdu_len==0)
        {
            Sleep(1);
            continue;
        }
        npdu_handler(&src, &Rx_Buf[0], pdu_len);
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

    apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,	Localhandler_read_property_ack);

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

#define  PRINT_ENABLED 1
void local_rp_ack_print_data(	BACNET_READ_PROPERTY_DATA * data)
{
    //BACNET_OBJECT_PROPERTY_VALUE object_value;  /* for bacapp printing */
    BACNET_APPLICATION_DATA_VALUE value;        /* for decode value data */

    int len = 0;
    uint8_t *application_data;
    int application_data_len;
    bool first_value = true;
    bool print_brace = false;



    if (data)
    {
        application_data = data->application_data;
        application_data_len = data->application_data_len;
        /* FIXME: what if application_data_len is bigger than 255? */
        /* value? need to loop until all of the len is gone... */
        for (;;)
        {
            //BACnet_Object_Property_Value_Own object_value;  /* for bacapp printing */
            BACNET_APPLICATION_DATA_VALUE value;        /* for decode value data */
            len = bacapp_decode_application_data(application_data,(uint8_t) application_data_len, &value);
            if (first_value && (len < application_data_len))
            {
                first_value = false;
#if PRINT_ENABLED
                fprintf(stdout, "{");
#endif
                print_brace = true;
            }
            receive_object_value.object_type = data->object_type;
            receive_object_value.object_instance = data->object_instance;
            receive_object_value.object_property = data->object_property;
            receive_object_value.array_index = data->array_index;
            receive_object_value.value = &value;

            if (len > 0)
            {
                if (len < application_data_len)
                {
                    application_data += len;
                    application_data_len -= len;
                    /* there's more! */

#if PRINT_ENABLED
                    fprintf(stdout, ",");
#endif
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
#if PRINT_ENABLED
        if (print_brace)
            fprintf(stdout, "}");
        fprintf(stdout, "\r\n");
#endif
    }
}
void Localhandler_read_property_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
//    int len = 0;
//    BACNET_READ_PROPERTY_DATA data;
//
//    (void) src;
//    (void) service_data;        /* we could use these... */
//    len = rp_ack_decode_service_request(service_request, service_len, &data);
//    //char my_pro_name[100];
//    //char * temp = get_prop_name();
//    //strcpy_s(my_pro_name,100,temp);
//
//#if 0
//    fprintf(stderr, "Received Read-Property Ack!\n");
//#endif
//    if (len > 0)
//    {
//        local_rp_ack_print_data(&data);
//        //	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
//    }

	 int len = 0;
	BACNET_READ_PROPERTY_DATA data;

 
	if(service_data->invoke_id == g_invoke_id) {
		len =
			rp_ack_decode_service_request(service_request, service_len, &data);
		if (len > 0) {
			//rp_ack_print_data(&data);
		}

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
    case 0x30:
        the_first=0;
        break;
    case 0x31:
        the_first=1;
        break;
    case 0x32:
        the_first=2;
        break;
    case 0x33:
        the_first=3;
        break;
    case 0x34:
        the_first=4;
        break;
    case 0x35:
        the_first=5;
        break;
    case 0x36:
        the_first=6;
        break;
    case 0x37:
        the_first=7;
        break;
    case 0x38:
        the_first=8;
        break;
    case 0x39:
        the_first=9;
        break;
    case 0x41:
        the_first=10;
        break;
    case 0x42:
        the_first=11;
        break;
    case 0x43:
        the_first=12;
        break;
    case 0x44:
        the_first=13;
        break;
    case 0x45:
        the_first=14;
        break;
    case 0x46:
        the_first=15;
        break;
    default:
        the_first = 0;
        break;
    }
    switch (need_conver.GetAt(1))
    {
    case 0x30:
        the_second=0;
        break;
    case 0x31:
        the_second=1;
        break;
    case 0x32:
        the_second=2;
        break;
    case 0x33:
        the_second=3;
        break;
    case 0x34:
        the_second=4;
        break;
    case 0x35:
        the_second=5;
        break;
    case 0x36:
        the_second=6;
        break;
    case 0x37:
        the_second=7;
        break;
    case 0x38:
        the_second=8;
        break;
    case 0x39:
        the_second=9;
        break;
    case 0x41:
        the_second=10;
        break;
    case 0x42:
        the_second=11;
        break;
    case 0x43:
        the_second=12;
        break;
    case 0x44:
        the_second=13;
        break;
    case 0x45:
        the_second=14;
        break;
    case 0x46:
        the_second=15;
        break;
    default:
        the_second = 0;
        break;
    }
    return (the_first*16+the_second);
}


extern char local_network_ip[255];
extern CString local_enthernet_ip;
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


    if(strIPAdress.IsEmpty())
    {
        servAddr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        char temp_ip_address[100];
        memset(temp_ip_address,0,100);
        WideCharToMultiByte( CP_ACP, 0, strIPAdress.GetBuffer(), -1, temp_ip_address, 255, NULL, NULL );
        servAddr.sin_addr.s_addr =  inet_addr(temp_ip_address);
    }




    //这个地方不加限制后，及时是多网卡广播也没有什么问题.
#if 0
    if(local_enthernet_ip.IsEmpty())
        servAddr.sin_addr.s_addr = INADDR_ANY;
    else
        servAddr.sin_addr.s_addr =  inet_addr(local_network_ip);
#endif
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
    //servAddr.sin_addr.S_un.S_addr = INADDR_ANY;//
    //	servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
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

#define MAX_STRING	100	// should be enough for everone (-:

//********************************************************
//*	FUNCTION: CheckForUpdate
//*
//*	DESCRIPTION:
//*		Connects to the specified Ftp server, and looks
//*		for the specified file (szFtpFilename) and reads
//*		just one string from it. Compares the string with
//*		the szCurrentVersion and if there is a difference,
//*		assumes there is a new app version.
//*
//* PARAMS:
//*		szFtpServer:	FTP server to access
//*		szFtpUsername:	FTP account name
//*		szFtpPassword:	appropriate password
//*		szFtpFilename:	FTP file which holds the
//*						version info
//*		szCurrentVersion:	version of the app calling
//*							this function
//*		szLastVersion:	version retrieved from FTP
//*						valid only if no error occurs
//*
//*	ASSUMES:
//*		Existance of a valid internet connection.
//*		AfxSocketInit() has already been called.
//*		Brains (optional).
//*
//*	RETURNS:
//*		TRUE only if new version is found
//*		FALSE if there was an error OR no new version
//*
//*	AUTHOR: T1TAN <t1tan@cmar-net.org>
//*
//*	COPYRIGHT:	Copyleft (C) T1TAN 2004 - 3827
//*				Copyleft (C) SprdSoft Inc. 2004 - 3827
//*				FREE for (ab)use in any form.
//*				(Leave the headers be)
//*
//*	VERSIONS:
//*		VERSION	AUTHOR	DATE		NOTES
//*		-------	------	----------	------------------
//*		1.0		T1TAN	07/05/2004	initial version
//********************************************************

BOOL CheckForUpdate(
    LPCTSTR szFtpServer,
    LPCTSTR szFtpUsername,
    LPCTSTR szFtpPassword,
    LPCTSTR szFtpFilename,
    LPCTSTR szCurrentVersion,
    LPTSTR szLastVersion )
{
    CWaitCursor wait;
    // zero the last anyway..
    ZeroMemory( szLastVersion, sizeof(szLastVersion) );
    // get a session
    CInternetSession* pFtpSession = new CInternetSession();
    CFtpConnection* pFtpConnection = NULL;

    if ( pFtpSession == NULL )
    {
        // DAMN!
        MessageBox( GetDesktopWindow(),
                    _T("Could not get internet session."),
                    _T("Error"), MB_OK|MB_ICONSTOP );
        return FALSE;
    }

    try
    {
        pFtpConnection = pFtpSession->GetFtpConnection
                         ( szFtpServer, szFtpUsername, szFtpPassword,21);
    }
    catch ( CInternetException *err )
    {
        // no luck today...
        err->ReportError( MB_OK|MB_ICONSTOP );
        err->Delete();
    }

    if ( pFtpConnection == NULL )
    {
        // DAMN AGAIN!!
        // cleanup
        pFtpSession->Close();
        delete pFtpSession;
        return FALSE;
    }

    CFtpFileFind ffind( pFtpConnection );

    BOOL isFound = ffind.FindFile( szFtpFilename );

    if ( isFound == FALSE )
    {
        // CRAP!! WHERE IS OUR FILE?!?!
        ffind.Close();
        pFtpConnection->Close();
        pFtpSession->Close();
        delete pFtpConnection;
        delete pFtpSession;
        MessageBox( GetDesktopWindow(),
                    _T("Could not get version information."),
                    _T("Error"), MB_OK|MB_ICONSTOP );
        return FALSE;
    }
    CString versionfilename=g_strExePth+_T("version.txt");

    BOOL bResult = pFtpConnection->GetFile
                   ( szFtpFilename, versionfilename, FALSE );

    if ( bResult == 0 )
    {
        // DAMN ERRORS
        ffind.Close();
        pFtpConnection->Close();
        pFtpSession->Close();
        delete pFtpConnection;
        delete pFtpSession;
        MessageBox( GetDesktopWindow(),
                    _T("Could not get version information."),
                    _T("Error"), MB_OK|MB_ICONSTOP );
        return FALSE;
    }

    CStdioFile verFile;
    //CFile verFile;
    CFileException error;

    bResult = verFile.Open( versionfilename,
                            CFile::modeRead, &error );

    if ( bResult == 0 )
    {
        // WHATTA HECK?!?
        ffind.Close();
        pFtpConnection->Close();
        pFtpSession->Close();
        delete pFtpConnection;
        delete pFtpSession;
        MessageBox( GetDesktopWindow(),
                    _T("Error opening local file."),
                    _T("Error"), MB_OK|MB_ICONSTOP );
        // just in case...
        DeleteFile(versionfilename);
        return FALSE;
    }
    //verFile.SeekToBegin();
    TCHAR buffer[MAX_STRING];
    ZeroMemory( buffer, sizeof(buffer) );
    //verFile.Read( buffer, MAX_STRING );
    CString The_CurentVersion;
    verFile.ReadString(The_CurentVersion);
    //buffer=The_CurentVersion.GetBuffer()
    //if ( _tcscmp( buffer, szCurrentVersion ) != 0 )
    //{	// new version available!
    //	_tcscpy( szLastVersion, buffer );
    //	// cleanup..
    //	// (i am sometimes impressed with comments
    //	// like this one.. "cleanup.." OH REALLY,
    //	// and i thought it's an airplane!!)
    //	verFile.Close();
    //	ffind.Close();
    //	pFtpConnection->Close();
    //	pFtpSession->Close();
    //	delete pFtpConnection;
    //	delete pFtpSession;

    //	//DeleteFile( versionfilename );
    //	// ok..
    //	return TRUE;
    //}

    // obviously nothing new here...
    // copy the current version to last version
    // so that the caller knows no error occured
    _tcscpy( szLastVersion, The_CurentVersion.GetBuffer() );
    // cleanup.. (again...)
    verFile.Close();
    ffind.Close();
    pFtpConnection->Close();
    pFtpSession->Close();
    delete pFtpConnection;
    delete pFtpSession;
    //DeleteFile( versionfilename );
    return TRUE;
}




#if 1
int AddNetDeviceForRefreshList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind)
{
	refresh_net_device temp;
	Str_UPD_SCAN temp_data;
	memset(&temp_data,0,400);
	unsigned char * my_temp_point = buffer;
	temp_data.reg.command = *(my_temp_point++);
	temp_data.reg.command_reserve = *(my_temp_point++);

	temp_data.reg.length = *(my_temp_point++);
	temp_data.reg.length_reserve = *(my_temp_point++);


	temp_data.reg.serial_low = *(my_temp_point++);
	temp_data.reg.serial_low_reserve = *(my_temp_point++);

	temp_data.reg.serial_low_2 = *(my_temp_point++);
	temp_data.reg.serial_low_2_reserve = *(my_temp_point++);

	temp_data.reg.serial_low_3 = *(my_temp_point++);
	temp_data.reg.serial_low_3_reserve = *(my_temp_point++);

	temp_data.reg.serial_low_4 = *(my_temp_point++);
	temp_data.reg.serial_low_4_reserve = *(my_temp_point++);

	temp_data.reg.product_id =  *(my_temp_point++);
	temp_data.reg.product_id_reserve =  *(my_temp_point++);


	temp_data.reg.modbus_id =  *(my_temp_point++);
	temp_data.reg.modbus_id_reserve =  *(my_temp_point++);

	temp_data.reg.ip_address_1 =  *(my_temp_point++);
	temp_data.reg.ip_address_1_reserve =  *(my_temp_point++);
	temp_data.reg.ip_address_2 =  *(my_temp_point++);
	temp_data.reg.ip_address_2_reserve =  *(my_temp_point++);
	temp_data.reg.ip_address_3 =  *(my_temp_point++);
	temp_data.reg.ip_address_3_reserve =  *(my_temp_point++);
	temp_data.reg.ip_address_4 =  *(my_temp_point++);
	temp_data.reg.ip_address_4_reserve =  *(my_temp_point++);

	temp_data.reg.modbus_port =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;
	temp_data.reg.sw_version =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;
	temp_data.reg.hw_version =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;

	temp_data.reg.parent_serial_number =  ((unsigned char)my_temp_point[3])<<24 | ((unsigned char)my_temp_point[2]<<16) | ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 4;

	temp_data.reg.object_instance_2 = *(my_temp_point++);
	temp_data.reg.object_instance_1 = *(my_temp_point++);
	temp_data.reg.station_number = *(my_temp_point++);
	memcpy(temp_data.reg.panel_name,my_temp_point,20);
	my_temp_point = my_temp_point + 20;
	temp_data.reg.object_instance_4 = *(my_temp_point++);
	temp_data.reg.object_instance_3 = *(my_temp_point++);
	temp_data.reg.isp_mode = *(my_temp_point++);	//isp_mode = 0 表示在应用代码 ，非0 表示在bootload.
	temp_data.reg.bacnetip_port =  ((unsigned char)my_temp_point[1])<<8 | ((unsigned char)my_temp_point[0]);
	my_temp_point= my_temp_point + 2;
	temp_data.reg.zigbee_exsit =  *(my_temp_point++); // 1 代表存在，其他任何代表不存在;

	DWORD nSerial=temp_data.reg.serial_low + temp_data.reg.serial_low_2 *256+temp_data.reg.serial_low_3*256*256+temp_data.reg.serial_low_4*256*256*256;
	CString nip_address;
	nip_address.Format(_T("%u.%u.%u.%u"),temp_data.reg.ip_address_1,temp_data.reg.ip_address_2,temp_data.reg.ip_address_3,temp_data.reg.ip_address_4);
	CString nproduct_name = GetProductName(temp_data.reg.product_id);
	if(nproduct_name.IsEmpty())	//如果产品号 没定义过，不认识这个产品 就exit;
	{
		if (temp_data.reg.product_id<220)
		{
			return m_refresh_net_device_data.size();
		}
	}

	temp.nport = temp_data.reg.modbus_port;
	temp.sw_version = temp_data.reg.sw_version;
	temp.hw_version = temp_data.reg.hw_version;
	temp.ip_address = nip_address;
	temp.product_id = temp_data.reg.product_id;
	temp.modbusID = temp_data.reg.modbus_id;
	temp.nSerial = nSerial;
	temp.NetCard_Address=local_enthernet_ip;

	temp.parent_serial_number = temp_data.reg.parent_serial_number ;

	temp.object_instance = temp_data.reg.object_instance_1 + temp_data.reg.object_instance_2 *256+temp_data.reg.object_instance_3*256*256+temp_data.reg.object_instance_4*256*256*256;
	temp.panal_number = temp_data.reg.station_number;

	temp.bacnetip_port = temp_data.reg.bacnetip_port;


	if((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SCAN_ONLY))
	{
		g_Print.Format(_T("Serial = %u     ID = %d ,ip = %s  , Product name : %s ,obj = %u ,panel = %u,isp_mode = %d"),nSerial,temp_data.reg.modbus_id,nip_address ,nproduct_name,temp.object_instance,temp.panal_number,temp_data.reg.isp_mode);
		DFTrace(g_Print);
	}

	if(temp_data.reg.isp_mode != 0)
	{
		//记录这个的信息,如果短时间多次出现 就判定在bootload下面，只是偶尔出现一次表示只是恰好开机收到的.
		IspModeInfo temp_info;
		temp_info.ipaddress[0] = temp_data.reg.ip_address_1;
		temp_info.ipaddress[1] = temp_data.reg.ip_address_2;
		temp_info.ipaddress[2] = temp_data.reg.ip_address_3;
		temp_info.ipaddress[3] = temp_data.reg.ip_address_4;
		temp_info.product_id = temp_data.reg.product_id;
		CTime temp_time_now = CTime::GetCurrentTime();
		temp_info.first_time = temp_time_now.GetTime();
	
		bool find_ip_pid_match = false;
		int temp_index = 0;
		for (int x=0;x<g_isp_device_info.size();x++)
		{
			int ip_cmp_ret =	memcmp(g_isp_device_info.at(x).ipaddress,temp_info.ipaddress,4); 
			if((temp_info.product_id == g_isp_device_info.at(x).product_id ) &&
				(ip_cmp_ret == 0))
			{
				find_ip_pid_match = true;
				temp_index = x;
			}
		}

		if(find_ip_pid_match)
		{
			if(( temp_info.first_time >  g_isp_device_info.at(temp_index).first_time  + 120 ) && 
				(temp_info.first_time <  g_isp_device_info.at(temp_index).first_time  + 180))
			{
				g_isp_device_info.at(temp_index).first_time = temp_info.first_time;
				need_isp_device = g_isp_device_info.at(temp_index);
				if(temp_data.reg.isp_mode == 2) //Minipanel 会回传特殊的 bootloader 坏掉的信息
				{
					isp_mode_error_code = 2;
					::PostMessageW(MainFram_hwd,WM_HADNLE_ISP_MODE_DEVICE,2,NULL);
				}
				else
					::PostMessageW(MainFram_hwd,WM_HADNLE_ISP_MODE_DEVICE,NULL,NULL);
			}
			else
			{
				if(temp_info.first_time > g_isp_device_info.at(temp_index).first_time  + 180)
					g_isp_device_info.at(temp_index).first_time = temp_info.first_time;
			}
		}
		else
		{
			g_isp_device_info.push_back(temp_info);
		}
		//TRACE(_T("ip:%s time:%d g_isp:%d\r\n"),nip_address,temp_info.first_time,g_isp_device_info.at(temp_index).first_time);

		return	 0;
	}


	bool find_exsit = false;

	for (int i=0; i<(int)m_refresh_net_device_data.size(); i++)
	{
		if(m_refresh_net_device_data.at(i).nSerial == nSerial)
		{
			find_exsit = true;
			break;
		}
	}

	if(!find_exsit)
	{
		m_refresh_net_device_data.push_back(temp);
	}


	char * temp_point = NULL;
	refresh_net_label_info temp_label;
	temp_point = temp_data.reg.panel_name;
	if(( (unsigned char)temp_point[0] != 0xff) && ((unsigned char)temp_point[1] != 0xff) && ((unsigned char)temp_point[0] != 0x00))
	{
		memcpy(temp_label.label_name,&temp_point[0],20);
		temp_point = temp_point + 20;
		CString cs_temp_label;
		MultiByteToWideChar( CP_ACP, 0, (char *)temp_label.label_name, (int)strlen((char *)temp_label.label_name)+1,
			cs_temp_label.GetBuffer(MAX_PATH), MAX_PATH );
		cs_temp_label.ReleaseBuffer();
		if(cs_temp_label.GetLength() > 20)
			cs_temp_label = cs_temp_label.Left(20);
		temp_label.serial_number = (unsigned int)nSerial;
		CString temp_serial_number;
		temp_serial_number.Format(_T("%u"),temp_label.serial_number);
		int need_to_write_into_device = GetPrivateProfileInt(temp_serial_number,_T("WriteFlag"),0,g_achive_device_name_path);
		if(need_to_write_into_device == 0)
		{
			bool found_device = false;
			bool found_device_new_name = false;
			for (int i=0; i<m_refresh_net_device_data.size(); i++)
			{
				if(temp_label.serial_number == m_refresh_net_device_data.at(i).nSerial)
				{
					if(cs_temp_label.CompareNoCase( m_refresh_net_device_data.at(i).show_label_name) == 0)
					{
						found_device_new_name = false;
					}
					else
					{
						m_refresh_net_device_data.at(i).show_label_name = cs_temp_label;
						found_device_new_name = true;
					}
					break;
				}
			}
		}
	}



	return m_refresh_net_device_data.size();
}
#endif

 

int GetHostAdaptersInfo(CString &IP_address_local)
{
    CString szAdaptersInfo;
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    /* variables used to print DHCP time info */

    pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
        if (pAdapterInfo == NULL)
        {
            return -1;
        }
    }
    int i_CntAdapters = 0;
    CString szTmp;
    if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            szTmp.Format(_T("adapter name: %s "),pAdapter->AdapterName);
            szAdaptersInfo += szTmp;
            szTmp.Format(_T("adapter description: %s "),pAdapter->Description);
            szAdaptersInfo += szTmp;
            szTmp.Format(_T("adapter address: "));
            szAdaptersInfo += szTmp;
            for (UINT i = 0; i < pAdapter->AddressLength; i++)
            {
                if (i == (pAdapter->AddressLength - 1))
                {
                    szTmp.Format(_T("%.2X "),(int)pAdapter->Address[i]);
                    szAdaptersInfo += szTmp;
                }
                else
                {
                    szTmp.Format(_T("%.2X-"),(int)pAdapter->Address[i]);
                    szAdaptersInfo += szTmp;
                }
            }
            szTmp.Format(_T("ip address: %s "),pAdapter->IpAddressList.IpAddress.String);
            szAdaptersInfo += szTmp;
            szTmp.Format(_T("ip mask: %s "),pAdapter->IpAddressList.IpMask.String);
            szAdaptersInfo += szTmp;
            szTmp.Format(_T("gateway: %s "),pAdapter->GatewayList.IpAddress.String);
            szAdaptersInfo += szTmp;

            szTmp.Format(_T("type: %d "),pAdapter->Type);
            szAdaptersInfo += szTmp;
            if(pAdapter->Type == 6)
            {
                IP_address_local.Empty();
                MultiByteToWideChar( CP_ACP,0,(char *)pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1,
                                     IP_address_local.GetBuffer(MAX_PATH), MAX_PATH );
                IP_address_local.ReleaseBuffer();
                break;

                //IP_address_local.fo
            }

            szTmp.Format(_T("index: %d "),pAdapter->Index);
            szAdaptersInfo += szTmp;

            pAdapter = pAdapter->Next;
            i_CntAdapters++;
        }
    }
    else
    {
        if (pAdapterInfo)
            free(pAdapterInfo);
        return -1;
    }
    szTmp.ReleaseBuffer();
    return i_CntAdapters;
}



void GetIPMaskGetWay()
{
    g_Vector_Subnet.clear();
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen;
    pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    ALL_LOCAL_SUBNET_NODE  Temp_Node;
    // 第一次调用GetAdapterInfo获取ulOutBufLen大小
    if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
    }

    if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            //             CString Name;//=pAdapter->AdapterName;
            // 			MultiByteToWideChar( CP_ACP, 0, pAdapter->AdapterName, (int)strlen((char *)pAdapter->AdapterName)+1,
            // 				Name.GetBuffer(MAX_PATH), MAX_PATH );
            // 			Name.ReleaseBuffer();

            MultiByteToWideChar( CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1,
                                 Temp_Node.StrIP.GetBuffer(MAX_PATH), MAX_PATH );
            Temp_Node.StrIP.ReleaseBuffer();
            //StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String);
            MultiByteToWideChar( CP_ACP, 0,pAdapter->IpAddressList.IpMask.String, (int)strlen((char *)pAdapter->IpAddressList.IpMask.String)+1,
                                 Temp_Node.StrMask.GetBuffer(MAX_PATH), MAX_PATH );
            Temp_Node.StrMask.ReleaseBuffer();

            //StrMask.Format(_T("%s"), pAdapter->IpAddressList.IpMask.String);

            MultiByteToWideChar( CP_ACP, 0,pAdapter->GatewayList.IpAddress.String, (int)strlen((char *)pAdapter->GatewayList.IpAddress.String)+1,
                                 Temp_Node.StrGetway.GetBuffer(MAX_PATH), MAX_PATH );
            Temp_Node.StrGetway.ReleaseBuffer();

            Temp_Node.NetworkCardType=pAdapter->Type;

            g_Vector_Subnet.push_back(Temp_Node);

            /*StrGetway.Format(_T("%s"), pAdapter->GatewayList.IpAddress.String); */
            pAdapter = pAdapter->Next;
        }
    }
    else
    {

    }
    if(pAdapterInfo !=NULL)	//Add by Fance . 如果不释放，会内存泄露 ，引起程序崩溃; 2015-10-22
        free(pAdapterInfo);
}



BOOL GetIPbyHostName(CString strHostName,CString &strIP)
{
    WSAData   wsdata;
    WORD wVersionRequested=MAKEWORD(2,0);
    int  err=WSAStartup(wVersionRequested,&wsdata);
    hostent     *pHostent=NULL   ;

    pHostent   =   gethostbyname(CW2A(strHostName));
    if(pHostent==NULL)
        return   false;
    if(pHostent->h_addr_list==NULL)
        return   false;
    sockaddr_in   sa;
    memcpy(&sa.sin_addr.s_addr,pHostent->h_addr_list[0],pHostent->h_length);
    strIP.Format(_T("%d.%d.%d.%d"),sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);
    WSACleanup();
    return TRUE;
}

BOOL Is_Dig_Num(CString str)
{
    int n=str.GetLength();
    for(int i=0; i<n; i++)
        if (str[i]<'0'||str[i]>'9')
            return FALSE;
    return TRUE;
}

BOOL ValidAddress(CString sAddress)
{
    int nPos;
    UINT n1,n2,n3,n4;
    CString sTemp=sAddress;
    n1=_wtoi(sTemp);
    nPos=sTemp.Find(_T("."));
    if(nPos==-1) return false;
    sTemp=sTemp.Mid(nPos+1);

    n2=_wtoi(sTemp);
    nPos=sTemp.Find(_T("."));
    if(nPos==-1) return false;
    sTemp=sTemp.Mid(nPos+1);
    n3=_wtoi(sTemp);
    nPos=sTemp.Find(_T("."));
    if(nPos==-1) return false;
    sTemp=sTemp.Mid(nPos+1);
    n4=_wtoi(sTemp);
    if(n1<0 ||n1>255) return false;
    if(n2<0 ||n2>255) return false;
    if(n3<0 ||n3>255) return false;
    if(n4<0 ||n4>255) return false;
    return TRUE;
}


void GetIPMaskGetWayForScan()
{
    g_Scan_Vector_Subnet.clear();
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen;
    pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    ALL_LOCAL_SUBNET_NODE  Temp_Node;
    // 第一次调用GetAdapterInfo获取ulOutBufLen大小
    if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
    }

    if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            //             CString Name;//=pAdapter->AdapterName;
            // 			MultiByteToWideChar( CP_ACP, 0, pAdapter->AdapterName, (int)strlen((char *)pAdapter->AdapterName)+1,
            // 				Name.GetBuffer(MAX_PATH), MAX_PATH );
            // 			Name.ReleaseBuffer();

            MultiByteToWideChar( CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char *)pAdapter->IpAddressList.IpAddress.String)+1,
                                 Temp_Node.StrIP.GetBuffer(MAX_PATH), MAX_PATH );
            Temp_Node.StrIP.ReleaseBuffer();
            //StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String);
            MultiByteToWideChar( CP_ACP, 0,pAdapter->IpAddressList.IpMask.String, (int)strlen((char *)pAdapter->IpAddressList.IpMask.String)+1,
                                 Temp_Node.StrMask.GetBuffer(MAX_PATH), MAX_PATH );
            Temp_Node.StrMask.ReleaseBuffer();

            //StrMask.Format(_T("%s"), pAdapter->IpAddressList.IpMask.String);

            MultiByteToWideChar( CP_ACP, 0,pAdapter->GatewayList.IpAddress.String, (int)strlen((char *)pAdapter->GatewayList.IpAddress.String)+1,
                                 Temp_Node.StrGetway.GetBuffer(MAX_PATH), MAX_PATH );
            Temp_Node.StrGetway.ReleaseBuffer();

            Temp_Node.NetworkCardType=pAdapter->Type;

            g_Scan_Vector_Subnet.push_back(Temp_Node);

            /*StrGetway.Format(_T("%s"), pAdapter->GatewayList.IpAddress.String); */
            pAdapter = pAdapter->Next;
        }
    }
    else
    {

    }
}

UINT RefreshNetWorkDeviceListByUDPFunc()
{

    int nRet = 0;
    //if (nRet == SOCKET_ERROR)
    //{
    //	/*goto END_SCAN;
    //	return 0;*/
    //}

    GetIPMaskGetWay();
    short nmsgType=UPD_BROADCAST_QRY_MSG;

    //////////////////////////////////////////////////////////////////////////
    const DWORD END_FLAG = 0x00000000;
    TIMEVAL time;
    time.tv_sec =3;
    time.tv_usec = 1000;

    fd_set fdSocket;
    BYTE buffer[512] = {0};

    BYTE pSendBuf[1024];
    for (int index=0; index<g_Vector_Subnet.size(); index++)
    {
        if (g_Vector_Subnet[index].StrIP.Find(_T("0.0."))!=-1)
        {
            continue;
        }

        ZeroMemory(pSendBuf, 255);
        pSendBuf[0] = 100;
        memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
        int nSendLen = 5;


        local_enthernet_ip=g_Vector_Subnet[index].StrIP;
        WideCharToMultiByte( CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL );
        h_siBind.sin_family=AF_INET;
        h_siBind.sin_addr.s_addr =  inet_addr(local_network_ip);
        //h_siBind.sin_addr.s_addr=INADDR_ANY;
        h_siBind.sin_port= htons(57619);
        // h_siBind.sin_port=AF_INET;
        //   ::bind(h_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));
        if( -1 == bind(h_Broad,(SOCKADDR*)&h_siBind,sizeof(h_siBind)))//把网卡地址强行绑定到Socket
        {
            goto END_REFRESH_SCAN;
        }
        int time_out=0;
        BOOL bTimeOut = FALSE;
        while(!bTimeOut)//!pScanner->m_bNetScanFinish)  // 超时结束
        {
            time_out++;
            if(time_out>1)
                bTimeOut = TRUE;

            FD_ZERO(&fdSocket);
            FD_SET(h_Broad, &fdSocket);

            nRet = ::sendto(h_Broad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast,sizeof(h_bcast));

            //DFTrace(local_enthernet_ip);
            if (nRet == SOCKET_ERROR)
            {
                int  nError = WSAGetLastError();
                goto END_REFRESH_SCAN;
                return 0;
            }
            int nLen = sizeof(h_siBind);
            fd_set fdRead = fdSocket;
            int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
            if (nSelRet == SOCKET_ERROR)
            {
                int nError = WSAGetLastError();
                goto END_REFRESH_SCAN;
                return 0;
            }

            if(nSelRet > 0)
            {
                ZeroMemory(buffer, 512);
                int nRet ;
                int nSelRet;
                do
                {
                    nRet = ::recvfrom(h_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);

                    BYTE szIPAddr[4] = {0};
                    if(nRet > 0)
                    {
                        FD_ZERO(&fdSocket);
                        if(buffer[0]==RESPONSE_MSG)
                        {
							if(scaning_mode)
							{
								g_llTxCount++;
								g_llRxCount++;
							}

                            nLen=buffer[2]+buffer[3]*256;
                            unsigned short dataPackage[32]= {0};
                            memcpy(dataPackage,buffer+2,nLen*sizeof(unsigned short));
                            szIPAddr[0]=  buffer[16];// (BYTE)dataPackage[7];
                            szIPAddr[1]=   buffer[18];//	(BYTE)dataPackage[8];
                            szIPAddr[2]=  buffer[20];//(BYTE)dataPackage[9];
                            szIPAddr[3]=  buffer[22];//(BYTE)dataPackage[10];

                            int n = 1;
                            BOOL bFlag=FALSE;
                            //////////////////////////////////////////////////////////////////////////
                            // 检测IP重复
                            DWORD dwValidIP = 0;
                            memcpy((BYTE*)&dwValidIP, pSendBuf+n, 4);
                            while(dwValidIP != END_FLAG)
                            {
                                DWORD dwRecvIP=0;
                                memcpy((BYTE*)&dwRecvIP, szIPAddr, 4);
                                memcpy((BYTE*)&dwValidIP, pSendBuf+n, 4);
                                if(dwRecvIP == dwValidIP)
                                {
                                    bFlag = TRUE;
                                    break;
                                }
                                n+=4;
                            }
                            //////////////////////////////////////////////////////////////////////////
                            if (!bFlag)
                            {
                                AddNetDeviceForRefreshList(buffer, nRet, h_siBind);

                                //pSendBuf[nSendLen-1] = (BYTE)(modbusID);
                                pSendBuf[nSendLen-4] = szIPAddr[0];
                                pSendBuf[nSendLen-3] = szIPAddr[1];
                                pSendBuf[nSendLen-2] = szIPAddr[2];
                                pSendBuf[nSendLen-1] = szIPAddr[3];
                                memcpy(pSendBuf + nSendLen, (BYTE*)&END_FLAG, 4);
                                //////////////////////////////////////////////////////////////////////////

                                //pSendBuf[nSendLen+3] = 0xFF;
                                nSendLen+=4;
                            }
                            else
                            {
                                AddNetDeviceForRefreshList(buffer, nRet, h_siBind);
                            }
                        }
#if 0
                        else if(buffer[0]  == RESPONSE_LABEL)
                        {
                            g_llTxCount++;
                            g_llRxCount++;
                            char * temp_point = NULL;
                            refresh_net_label_info temp_label;
                            temp_point = (char *)&buffer[1];
                            if((buffer[1] == 0xff) || (buffer[2] == 0xff) || (buffer[1] == 0x00))
                                continue;
                            if(nRet >= 25)
                            {
                                memcpy(temp_label.label_name,&buffer[1],20);
                                temp_point = temp_point + 20;
                                CString cs_temp_label;
                                MultiByteToWideChar( CP_ACP, 0, (char *)temp_label.label_name, (int)strlen((char *)temp_label.label_name)+1,
                                                     cs_temp_label.GetBuffer(MAX_PATH), MAX_PATH );
                                cs_temp_label.ReleaseBuffer();
                                if(cs_temp_label.GetLength() > 20)
                                    cs_temp_label = cs_temp_label.Left(20);
                                temp_label.serial_number = ((unsigned char)temp_point[3])<<24 | ((unsigned char)temp_point[2]<<16) | ((unsigned char)temp_point[1])<<8 | ((unsigned char)temp_point[0]);
                                CString temp_serial_number;
                                temp_serial_number.Format(_T("%u"),temp_label.serial_number);
                                int need_to_write_into_device = GetPrivateProfileInt(temp_serial_number,_T("WriteFlag"),0,g_achive_device_name_path);
                                if(need_to_write_into_device == 0)
                                {
                                    bool found_device = false;
                                    bool found_device_new_name = false;
                                    for (int i=0; i<m_refresh_net_device_data.size(); i++)
                                    {
                                        if(temp_label.serial_number == m_refresh_net_device_data.at(i).nSerial)
                                        {
                                            if(cs_temp_label.CompareNoCase( m_refresh_net_device_data.at(i).show_label_name) == 0)
                                            {
                                                found_device_new_name = false;
                                            }
                                            else
                                            {
                                                m_refresh_net_device_data.at(i).show_label_name = cs_temp_label;
                                                found_device_new_name = true;
                                            }
                                            break;
                                        }
                                    }
                                }

                            }
                        }
                        //g_llTxCount++;//不合理
#endif
                    }
                    else
                    {

                        break;
                    }


                    FD_ZERO(&fdSocket);
                    FD_SET(h_Broad, &fdSocket);
                    nLen = sizeof(h_siBind);
                    fdRead = fdSocket;
                    nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
                }
                while (nSelRet);

                //int nRet = ::recvfrom(h_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);

            }
            else
            {
                g_ScnnedNum = 0;
                bTimeOut = TRUE;
                //g_llTxCount++;//不合理
            }
        }//end of while
END_REFRESH_SCAN:

        closesocket(h_Broad);
        h_Broad=NULL;
        {

            //SOCKET soAck =::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            h_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            BOOL bBroadcast=TRUE;
            ::setsockopt(h_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
            int iMode=1;
            ioctlsocket(h_Broad,FIONBIO, (u_long FAR*) &iMode);

            BOOL bDontLinger = FALSE;
            setsockopt( h_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );

        }

        if( AfxGetMainWnd()->GetActiveWindow() != NULL )
        {


            CString str;
            str.Format(_T("San Command [Tx=%d Rx=%d Err=%d]"),
                       g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

            //Display it
            ((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

        }

    }
    if (g_Vector_Subnet.size()==0)
    {
        SetPaneString(3,_T("No Network Card is installed in your PC"));
    }
    return 1;
}

extern SOCKET my_sokect;
void Send_WhoIs_remote_ip(CString ipaddress_temp)
{
    CString ipaddress;
    if (ValidAddress(ipaddress_temp)==FALSE)  // 验证NC的IP
    {
        if(!GetIPbyHostName(ipaddress_temp,ipaddress))
        {
            return ;
        }
    }
    else
    {
        ipaddress  = ipaddress_temp;
    }

    //SOCKADDR_IN bcast;.
    char temp_ip_address[20];
    WideCharToMultiByte( CP_ACP, 0, ipaddress.GetBuffer(), -1, temp_ip_address, 255, NULL, NULL );

    SOCKADDR_IN h_bcast_temp;

    UINT temp_ip = inet_addr(temp_ip_address);
    h_bcast_temp.sin_family=AF_INET;
    h_bcast_temp.sin_addr.s_addr=temp_ip;
    //h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
    h_bcast_temp.sin_port=htons(BACNETIP_PORT);


    const DWORD END_FLAG = 0x00000000;
    TIMEVAL time;
    time.tv_sec =3;
    time.tv_usec = 1000;

    fd_set fdSocket;
    BYTE buffer[512] = {0};
    //	char pSendBuf[255];
    //	ZeroMemory(pSendBuf, 255);
    BYTE pSendBuf[1024] = {0x81, 0x0B, 0x00, 0x0C, 0x01, 0x20, 0xFF, 0xFF, 0x00, 0xFF, 0x10, 0x08};

    //pSendBuf[0] = 100;
    //memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
    int nSendLen = 12;

    int time_out=0;
    BOOL bTimeOut = FALSE;
    int nRet = 0;
    Sleep(1);
    Initial_bac(0);
    while(!bTimeOut)//!pScanner->m_bNetScanFinish)  // 超时结束
    {
        time_out++;
        if(time_out>5)
            bTimeOut = TRUE;

        //		FD_ZERO(&fdSocket);
        //		FD_SET(h_temp_sokcet, &fdSocket);
        //my_sokect
        nRet = ::sendto(my_sokect,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast_temp,sizeof(h_bcast_temp));
        //nRet = ::sendto(h_temp_sokcet,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast_temp,sizeof(h_bcast_temp));
        if (nRet == SOCKET_ERROR)
        {
            int  nError = WSAGetLastError();
            //goto END_SCAN;
            return ;
        }
        Sleep(200);
        continue;//Test ///////////////////////////////////////////////////////////////////////////////////

    }//end of while

    //closesocket(h_Broad);


}




 
int LoadBacnetConfigFile(bool write_to_device,LPCTSTR tem_read_path)
{
    if((g_mac!=0) &&(g_bac_instance!=0))
    {
        CString FilePath;
        if(write_to_device)	//如果是客户手动load 就让客户选择路径，不是手动load就说明是读缓存;
        {
            CFileDialog dlg(true,_T("*.prg"),_T(" "),OFN_HIDEREADONLY ,_T("Prg files (*.prg)|*.prg||"),NULL,0);
            if(IDOK!=dlg.DoModal())
                return -1;
            FilePath=dlg.GetPathName();
        }
        else
        {
            //FilePath = tem_read_path;
            FilePath.Format(_T("%s"),tem_read_path);
            CFileFind temp_find;
            if(!temp_find.FindFile(FilePath))
                return -1;
        }



#if 1
        CFile myfile(FilePath,CFile::modeRead);
        char *pBuf;
        DWORD dwFileLen;
        dwFileLen=myfile.GetLength();
        pBuf= new char[dwFileLen+1];
        pBuf[dwFileLen]=0;
        myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
        myfile.Close();
        //MessageBox(pBuf);
        char * temp_buffer = pBuf;
		bool b_new_prg = false;
		int ntemp_version = 0;
		if(( (unsigned char)temp_buffer[0] == 0x55  ) && ((unsigned char)temp_buffer[1] == 0xff))//新版本的prg
		{
			temp_buffer = temp_buffer + 2;
			if(temp_buffer[0] >= 5)
			{
				ntemp_version = temp_buffer[0];
				b_new_prg = true;
			}
			temp_buffer ++ ;
		}

		if(b_new_prg == false)
		{
			for (int i=0; i<dwFileLen; i++)
			{
				*temp_buffer = *temp_buffer ^ 1;
				temp_buffer ++;
			}

			CString new_file;
			new_file = FilePath.Left(FilePath.GetLength()-3) + _T("ini");

			HANDLE hFile;
			hFile=CreateFile(new_file,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
			DWORD dWrites;
			WriteFile(hFile,pBuf,dwFileLen,&dWrites,NULL);
			CloseHandle(hFile);
			if(pBuf)
				delete pBuf;


			FilePath = new_file;
#endif

			ntemp_version = GetPrivateProfileInt(_T("Setting"),_T("Version"),0,FilePath);
			if(ntemp_version < 2)
			{
				DeleteFile(new_file);
				SetPaneString(BAC_SHOW_MISSION_RESULTS ,_T("You config file is the old version."));
				return -1;
			}

			if(ntemp_version < 4)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS ,_T("You config file is the old version.Please save a new one."));
			}

			//Version 3 加入了 BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT    BAC_GRPHIC_LABEL_COUNT    BAC_USER_LOGIN_COUNT    BAC_CUSTOMER_UNITS_COUNT
			//Version 4 加入了 Setting 结构;
			//			CString FilePath;
			//		FilePath=dlg.GetPathName();
			if(ntemp_version >= 2)
			{
				for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
				{
					CString temp_input,temp_des,temp_csc;
					temp_input.Format(_T("Input%d"),i);

					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_input,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Input_data.at(i).description,STR_IN_DESCRIPTION_LENGTH,cTemp1,STR_IN_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_input,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Input_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);

					m_Input_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_input,_T("Auto_Manual"),0,FilePath);
					m_Input_data.at(i).value = GetPrivateProfileInt(temp_input,_T("Value"),0,FilePath);

					m_Input_data.at(i).filter = (unsigned char)GetPrivateProfileInt(temp_input,_T("Filter"),0,FilePath);
					m_Input_data.at(i).decom = (unsigned char)GetPrivateProfileInt(temp_input,_T("Decom"),0,FilePath);
					m_Input_data.at(i).sub_id = (unsigned char)GetPrivateProfileInt(temp_input,_T("Sen_On"),0,FilePath);

					m_Input_data.at(i).sub_product = (unsigned char)GetPrivateProfileInt(temp_input,_T("Sen_Off"),0,FilePath);
					m_Input_data.at(i).control = (unsigned char)GetPrivateProfileInt(temp_input,_T("Control"),0,FilePath);
					m_Input_data.at(i).digital_analog = (unsigned char)GetPrivateProfileInt(temp_input,_T("Digital_Analog"),0,FilePath);

					m_Input_data.at(i).calibration_sign = (unsigned char)GetPrivateProfileInt(temp_input,_T("Calibration_Sign"),0,FilePath);
					m_Input_data.at(i).sub_number = (unsigned char)GetPrivateProfileInt(temp_input,_T("Calibration_Increment"),0,FilePath);
					m_Input_data.at(i).calibration_h = (unsigned char)GetPrivateProfileInt(temp_input,_T("Unused"),0,FilePath);

					m_Input_data.at(i).calibration_l = (unsigned char)GetPrivateProfileInt(temp_input,_T("Calibration"),0,FilePath);
					m_Input_data.at(i).range = (unsigned char)GetPrivateProfileInt(temp_input,_T("Range"),0,FilePath);

				}

				for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Output%d"),i);
					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH,cTemp1,STR_OUT_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Output_data.at(i).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);

					m_Output_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
					m_Output_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);

					m_Output_data.at(i).digital_analog = (unsigned char)GetPrivateProfileInt(temp_section,_T("Digital_Analog"),0,FilePath);
					m_Output_data.at(i).hw_switch_status = (unsigned char)GetPrivateProfileInt(temp_section,_T("hw_switch_status"),0,FilePath);
					m_Output_data.at(i).control = (unsigned char)GetPrivateProfileInt(temp_section,_T("Control"),0,FilePath);
					m_Output_data.at(i).digital_control = (unsigned char)GetPrivateProfileInt(temp_section,_T("Digital_Control"),0,FilePath);
					m_Output_data.at(i).decom = (unsigned char)GetPrivateProfileInt(temp_section,_T("Decom"),0,FilePath);
					m_Output_data.at(i).range = (unsigned char)GetPrivateProfileInt(temp_section,_T("Range"),0,FilePath);
					m_Output_data.at(i).sub_id = (unsigned char)GetPrivateProfileInt(temp_section,_T("M_Del_Low"),0,FilePath);
					m_Output_data.at(i).sub_product = (unsigned char)GetPrivateProfileInt(temp_section,_T("S_Del_High"),0,FilePath);
					m_Output_data.at(i).sub_number = (unsigned char)GetPrivateProfileInt(temp_section,_T("Sub__number"),0,FilePath);
					m_Output_data.at(i).pwm_period = (unsigned char)GetPrivateProfileInt(temp_section,_T("Delay_Timer"),0,FilePath);
				}

				for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Variable%d"),i);
					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Variable_data.at(i).description,STR_VARIABLE_DESCRIPTION_LENGTH,cTemp1,STR_VARIABLE_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Variable_data.at(i).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);



					m_Variable_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
					m_Variable_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
					m_Variable_data.at(i).digital_analog = (unsigned char)GetPrivateProfileInt(temp_section,_T("Digital_Analog"),0,FilePath);
					m_Variable_data.at(i).control = (unsigned char)GetPrivateProfileInt(temp_section,_T("Control"),0,FilePath);
					m_Variable_data.at(i).unused = (unsigned char)GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);
					m_Variable_data.at(i).range = (unsigned char)GetPrivateProfileInt(temp_section,_T("Range"),0,FilePath);

				}

				for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Program%d"),i);
					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Program_data.at(i).description,STR_PROGRAM_DESCRIPTION_LENGTH,cTemp1,STR_PROGRAM_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Program_data.at(i).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);

					m_Program_data.at(i).bytes = (unsigned short)GetPrivateProfileInt(temp_section,_T("Bytes"),0,FilePath);
					m_Program_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
					m_Program_data.at(i).on_off = (unsigned char)GetPrivateProfileInt(temp_section,_T("On_Off"),0,FilePath);
					m_Program_data.at(i).com_prg = (unsigned char)GetPrivateProfileInt(temp_section,_T("Com_Prg"),0,FilePath);
					m_Program_data.at(i).errcode = (unsigned char)GetPrivateProfileInt(temp_section,_T("Errcode"),0,FilePath);
					m_Program_data.at(i).unused = (unsigned char)GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);
				}

				for (int i=0; i<BAC_PID_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Controller%d"),i);

					m_controller_data.at(i).input.number = (unsigned char)GetPrivateProfileInt(temp_section,_T("Input_Number"),0,FilePath);
					m_controller_data.at(i).input.panel = (unsigned char)GetPrivateProfileInt(temp_section,_T("Input_Panel"),0,FilePath);
					if((m_controller_data.at(i).input.panel != Station_NUM) && (m_controller_data.at(i).input.panel != 0))	 //在load prg 的时候 如果加载的panel != 自己的 就变成自己的
						m_controller_data.at(i).input.panel = Station_NUM;
					m_controller_data.at(i).input.point_type = (unsigned char)GetPrivateProfileInt(temp_section,_T("Input_Point_Type"),0,FilePath);
					m_controller_data.at(i).input_value = GetPrivateProfileInt(temp_section,_T("Input_Value"),0,FilePath);
					m_controller_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
					m_controller_data.at(i).setpoint.number = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Number"),0,FilePath);
					m_controller_data.at(i).setpoint.panel = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Panel"),0,FilePath);
					if((m_controller_data.at(i).setpoint.panel != Station_NUM) &&  (m_controller_data.at(i).setpoint.panel != 0) )	 //在load prg 的时候 如果加载的panel != 自己的 就变成自己的
						m_controller_data.at(i).setpoint.panel = Station_NUM;
					m_controller_data.at(i).setpoint.point_type = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Point_Type"),0,FilePath);
					m_controller_data.at(i).setpoint_value = (unsigned char)GetPrivateProfileInt(temp_section,_T("Setpoint_Value"),0,FilePath);
					m_controller_data.at(i).units = (unsigned char)GetPrivateProfileInt(temp_section,_T("Units"),0,FilePath);
					m_controller_data.at(i).auto_manual = (unsigned char)GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
					m_controller_data.at(i).action = (unsigned char)GetPrivateProfileInt(temp_section,_T("Action"),0,FilePath);
					m_controller_data.at(i).repeats_per_min = (unsigned char)GetPrivateProfileInt(temp_section,_T("Repeats_Per_Min"),0,FilePath);
					m_controller_data.at(i).sample_time = (unsigned char)GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);
					m_controller_data.at(i).prop_high = (unsigned char)GetPrivateProfileInt(temp_section,_T("Prop_High"),0,FilePath);
					m_controller_data.at(i).proportional = (unsigned char)GetPrivateProfileInt(temp_section,_T("Proportional"),0,FilePath);
					m_controller_data.at(i).reset = (unsigned char)GetPrivateProfileInt(temp_section,_T("Reset"),0,FilePath);
					m_controller_data.at(i).bias = (unsigned char)GetPrivateProfileInt(temp_section,_T("Bias"),0,FilePath);
					m_controller_data.at(i).rate = (unsigned char)GetPrivateProfileInt(temp_section,_T("Rate"),0,FilePath);
				}


				for (int i=0; i<BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
				{
					CString tempsection,temp_code,temp_csc;
					tempsection.Format(_T("WeeklyRoutinesData_%d"),i);
					CString temp_weeklycode_code;
					GetPrivateProfileStringW(tempsection,_T("Data"),_T(""),temp_weeklycode_code.GetBuffer(300),300,FilePath);
					temp_weeklycode_code.ReleaseBuffer();

					for (int j=0; j<WEEKLY_SCHEDULE_SIZE; j++)
					{
						CString temp_value;
						temp_value = temp_weeklycode_code.Left(2);
						temp_weeklycode_code = temp_weeklycode_code.Right(temp_weeklycode_code.GetLength()-2);
						weeklt_time_schedule[i][j]= Str_to_Byte(temp_value);
					}
					unsigned char * temp_point = NULL;
					temp_point = weeklt_time_schedule[i];
					for (int x=0; x<9; x++)
					{
						for (int y=0; y<8; y++)
						{
							m_Schedual_Time_data.at(i).Schedual_Day_Time[y][x].time_minutes = *(temp_point ++);
							m_Schedual_Time_data.at(i).Schedual_Day_Time[y][x].time_hours = *(temp_point ++);
						}
					}


				}

				for (int i=0; i<BAC_ANNUAL_CODE_COUNT; i++)
				{
					CString tempsection,temp_code,temp_csc;
					tempsection.Format(_T("AnnualRoutinesData_%d"),i);
					CString temp_annualcode_code;
					GetPrivateProfileStringW(tempsection,_T("Data"),_T(""),temp_annualcode_code.GetBuffer(MAX_PATH),MAX_PATH,FilePath);
					temp_annualcode_code.ReleaseBuffer();

					for (int j=0; j<ANNUAL_CODE_SIZE; j++)
					{
						CString temp_value;
						temp_value = temp_annualcode_code.Left(2);
						temp_annualcode_code = temp_annualcode_code.Right(temp_annualcode_code.GetLength()-2);
						g_DayState[i][j]= Str_to_Byte(temp_value);
						//weeklt_time_schedule[i][j]= Str_to_Byte(temp_value);
					}
				}

				for (int i=0; i<BAC_PROGRAMCODE_ITEM_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					CString temp_code;
					unsigned char * temp_point = NULL;
					temp_point = program_code[i];
					temp_section.Format(_T("Program_Code%d"),i);
					program_code_length[i] = (unsigned int)GetPrivateProfileInt(temp_section,_T("Program_Length"),0,FilePath);

					if((program_code_length[i] >2000) || (program_code_length[i] == 0))
					{
						program_code_length[i]=0;
						memset(program_code[i],0,2000);
						continue;
					}

					CString part_section;
					part_section.Format(_T("Program_Code_Text"));


					CString temp_program_code;
					GetPrivateProfileStringW(temp_section,part_section,_T(""),temp_program_code.GetBuffer(4000),4000,FilePath);
					temp_program_code.ReleaseBuffer();

					if(ntemp_version < 2)	//如果是加载的旧版本的 配置档,code 就清零;
					{
						program_code_length[i] = 0;
						memset(program_code[i],0,2000);
					}
					else
					{
						int temp_count = temp_program_code.GetLength()/2;
						for (int x=0; x<temp_count; x++)
						{
							CString temp_value;
							temp_value = temp_program_code.Left(2);
							temp_program_code = temp_program_code.Right(temp_program_code.GetLength()-2);
							program_code[i][x] = Str_to_Byte(temp_value);
						}
#if 0
						if(temp_program_code.GetLength() == 2*program_code_length[i])
						{
							for (int x=0; x<program_code_length[i]; x++)
							{
								CString temp_value;
								temp_value = temp_program_code.Left(2);
								temp_program_code = temp_program_code.Right(temp_program_code.GetLength()-2);
								program_code[i][x] = Str_to_Byte(temp_value);
							}
						}
#endif
					}


				}


				for (int i=0; i<BAC_SCREEN_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Screen%d"),i);

					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_screen_data.at(i).description,STR_SCREEN_DESCRIPTION_LENGTH,cTemp1,STR_SCREEN_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_screen_data.at(i).label,STR_SCREEN_LABLE_LENGTH,cTemp1,STR_SCREEN_LABLE_LENGTH);
					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Picture_file"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_screen_data.at(i).picture_file,STR_SCREEN_PIC_FILE_LENGTH,cTemp1,STR_SCREEN_PIC_FILE_LENGTH);

					m_screen_data.at(i).update = GetPrivateProfileInt(temp_section,_T("Update"),0,FilePath);
					m_screen_data.at(i).mode = GetPrivateProfileInt(temp_section,_T("Mode"),0,FilePath);
					m_screen_data.at(i).xcur_grp = GetPrivateProfileInt(temp_section,_T("Xcur_grp"),0,FilePath);
					m_screen_data.at(i).ycur_grp = GetPrivateProfileInt(temp_section,_T("Ycur_grp"),0,FilePath);
				}

				for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Weekly_Routines%d"),i);

					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Weekly_data.at(i).description,STR_WEEKLY_DESCRIPTION_LENGTH,cTemp1,STR_WEEKLY_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Weekly_data.at(i).label,STR_WEEKLY_LABEL_LENGTH,cTemp1,STR_WEEKLY_LABEL_LENGTH);

					m_Weekly_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
					m_Weekly_data.at(i).auto_manual = GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
					m_Weekly_data.at(i).override_1_value = GetPrivateProfileInt(temp_section,_T("Override_1_Value"),0,FilePath);
					m_Weekly_data.at(i).override_2_value = GetPrivateProfileInt(temp_section,_T("Override_2_Value"),0,FilePath);
					m_Weekly_data.at(i).off = GetPrivateProfileInt(temp_section,_T("Off"),0,FilePath);
					m_Weekly_data.at(i).unused = GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);

					m_Weekly_data.at(i).override_1.number = GetPrivateProfileInt(temp_section,_T("Override_1_Number"),0,FilePath);
					m_Weekly_data.at(i).override_1.panel = GetPrivateProfileInt(temp_section,_T("Override_1_Panel"),0,FilePath);
					m_Weekly_data.at(i).override_1.point_type = GetPrivateProfileInt(temp_section,_T("Override_1_Point_Type"),0,FilePath);
					m_Weekly_data.at(i).override_2.number = GetPrivateProfileInt(temp_section,_T("Override_2_number"),0,FilePath);
					m_Weekly_data.at(i).override_2.panel = GetPrivateProfileInt(temp_section,_T("Override_2_Panel"),0,FilePath);
					m_Weekly_data.at(i).override_2.point_type = GetPrivateProfileInt(temp_section,_T("Override_2_Point_Type"),0,FilePath);
				}

				for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Annual_Routines%d"),i);

					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Description"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Annual_data.at(i).description,STR_ANNUAL_DESCRIPTION_LENGTH,cTemp1,STR_ANNUAL_DESCRIPTION_LENGTH);

					cs_temp.Empty();
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_Annual_data.at(i).label,STR_ANNUAL_LABEL_LENGTH,cTemp1,STR_ANNUAL_LABEL_LENGTH);

					m_Annual_data.at(i).value = GetPrivateProfileInt(temp_section,_T("Value"),0,FilePath);
					m_Annual_data.at(i).auto_manual = GetPrivateProfileInt(temp_section,_T("Auto_Manual"),0,FilePath);
					m_Annual_data.at(i).unused = GetPrivateProfileInt(temp_section,_T("Unused"),0,FilePath);

				}

				for (int i=0; i<BAC_MONITOR_COUNT; i++)
				{
					CString temp_section,temp_des,temp_csc;
					temp_section.Format(_T("Monitor%d"),i);

					CString cs_temp;
					char cTemp1[255];
					GetPrivateProfileStringW(temp_section,_T("Label"),_T(""),cs_temp.GetBuffer(MAX_PATH),255,FilePath);
					cs_temp.ReleaseBuffer();
					memset(cTemp1,0,255);
					WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
					memcpy_s(m_monitor_data.at(i).label,STR_MONITOR_LABEL_LENGTH,cTemp1,STR_MONITOR_LABEL_LENGTH);


					CString temp_input_code1;
					GetPrivateProfileStringW(temp_section,_T("Inputs"),_T(""),temp_input_code1.GetBuffer(MAX_PATH),255,FilePath);
					temp_input_code1.ReleaseBuffer();

					m_monitor_data.at(i).second_interval_time = GetPrivateProfileInt(temp_section,_T("Second_Interval_Time"),0,FilePath);
					m_monitor_data.at(i).minute_interval_time = GetPrivateProfileInt(temp_section,_T("Minute_Interval_Time"),0,FilePath);
					m_monitor_data.at(i).hour_interval_time = GetPrivateProfileInt(temp_section,_T("Hour_Interval_Time"),0,FilePath);
					m_monitor_data.at(i).max_time_length = GetPrivateProfileInt(temp_section,_T("Max_Time_Length"),0,FilePath);
					m_monitor_data.at(i).num_inputs = GetPrivateProfileInt(temp_section,_T("Num_Inputs"),0,FilePath);
					m_monitor_data.at(i).an_inputs = GetPrivateProfileInt(temp_section,_T("An_Inputs"),0,FilePath);
					m_monitor_data.at(i).next_sample_time = GetPrivateProfileInt(temp_section,_T("next_sample_time"),0,FilePath);
					//m_monitor_data.at(i).wrap_flag = GetPrivateProfileInt(temp_section,_T("Wrap_flag"),0,FilePath);
					m_monitor_data.at(i).status = GetPrivateProfileInt(temp_section,_T("Status"),0,FilePath);
					//m_monitor_data.at(i).reset_flag = GetPrivateProfileInt(temp_section,_T("Reset_Flag"),0,FilePath);
					//m_monitor_data.at(i).double_flag = GetPrivateProfileInt(temp_section,_T("Double_flag"),0,FilePath);

					CString temp_input = temp_input_code1;
					int temp_input_test_value;
					int struct_test_value;
					temp_input_test_value = temp_input.GetLength() / 2;
					struct_test_value = sizeof(Point_Net)*MAX_POINTS_IN_MONITOR;
					if(temp_input_test_value != struct_test_value )
					{
						if(IDYES == AfxMessageBox(_T("Prg is too old.Continue") ,MB_YESNO))
							continue;	//如果这一个的长度不正确 就继续下一个;忽略这个;
						else
							return 0;
					}
					unsigned char temp_in_array[sizeof(Point_Net)*MAX_POINTS_IN_MONITOR];
					for (int x=0; x<(sizeof(Point_Net)*MAX_POINTS_IN_MONITOR); x++)
					{
						CString temp_value;
						temp_value = temp_input.Left(2);
						temp_input = temp_input.Right(temp_input.GetLength()-2);
						temp_in_array[x] = Str_to_Byte(temp_value);
					}
					memcpy_s(&m_monitor_data.at(i).inputs[0],sizeof(Point_Net)*MAX_POINTS_IN_MONITOR,temp_in_array,sizeof(Point_Net)*MAX_POINTS_IN_MONITOR);//copy 70

					for (int x=0;x<MAX_POINTS_IN_MONITOR;x++)
					{
						if(m_monitor_data.at(i).inputs[x].panel != Station_NUM)	 //在load prg 的时候 如果加载的panel != 自己的 就变成自己的
						{
							if((m_monitor_data.at(i).inputs[x].panel == m_monitor_data.at(i).inputs[x].sub_panel) && (m_monitor_data.at(i).inputs[x].panel != 0))
							{
								m_monitor_data.at(i).inputs[x].sub_panel = Station_NUM;
								m_monitor_data.at(i).inputs[x].panel = Station_NUM;
							}
						}
					}

					CString temp_range_code1;
					GetPrivateProfileStringW(temp_section,_T("Range"),_T(""),temp_range_code1.GetBuffer(MAX_PATH),255,FilePath);
					temp_range_code1.ReleaseBuffer();

					CString temp_range = temp_range_code1;

					if(temp_range.GetLength() != MAX_POINTS_IN_MONITOR*2)
						continue;
					unsigned char rang_array[MAX_POINTS_IN_MONITOR];
					for (int r=0; r<MAX_POINTS_IN_MONITOR; r++)
					{
						CString temp_value;
						temp_value = temp_range.Left(2);
						temp_range = temp_range.Right(temp_range.GetLength()-2);
						rang_array[r] = Str_to_Byte(temp_value);
					}
					memcpy_s(&m_monitor_data.at(i).range[0],MAX_POINTS_IN_MONITOR,rang_array,MAX_POINTS_IN_MONITOR);//copy MAX_POINTS_IN_MONITOR 14



				}
			}
			if(ntemp_version >= 3) // 第三版中新加入的;
			{
				for (int i=0; i<BAC_GRPHIC_LABEL_COUNT; i++)
				{
					CString temp_section;
					CString temp_code;
					unsigned char * temp_point = NULL;
					char temp_buffer[400];
					memset(temp_buffer,0,400);
					temp_section.Format(_T("LabelData_%d"),i);

					CString temp_grplable_code;
					GetPrivateProfileStringW(_T("GraphicLabel"),temp_section,_T(""),temp_grplable_code.GetBuffer(4000),4000,FilePath);
					temp_grplable_code.ReleaseBuffer();

					int temp_count = temp_grplable_code.GetLength()/2;
					if(temp_count != sizeof(Str_label_point))
					{
						AfxMessageBox(_T("Load prg file error."));
						return 1;
					}
					for (int x=0;x<temp_count;x++)
					{
						CString temp_value;
						temp_value = temp_grplable_code.Left(2);
						temp_grplable_code = temp_grplable_code.Right(temp_grplable_code.GetLength()-2);
						temp_buffer[x] = Str_to_Byte(temp_value);
					}
					memcpy(&m_graphic_label_data.at(i),temp_buffer,sizeof(Str_label_point));
					if((m_graphic_label_data.at(i).reg.nMain_Panel != Station_NUM) && (m_graphic_label_data.at(i).reg.nMain_Panel != 0))
					{
						m_graphic_label_data.at(i).reg.nMain_Panel = Station_NUM;
						m_graphic_label_data.at(i).reg.nSub_Panel = Station_NUM;
					}
				}


				for (int i=0; i<BAC_USER_LOGIN_COUNT; i++)
				{
					CString temp_section;
					CString temp_code;
					unsigned char * temp_point = NULL;
					char temp_buffer[400];
					memset(temp_buffer,0,400);
					temp_section.Format(_T("Userlogin_%d"),i);

					CString temp_login_code;
					GetPrivateProfileStringW(_T("LoginData"),temp_section,_T(""),temp_login_code.GetBuffer(4000),4000,FilePath);
					temp_login_code.ReleaseBuffer();

					int temp_count = temp_login_code.GetLength()/2;
					if(temp_count != sizeof(Str_userlogin_point))
					{
						AfxMessageBox(_T("Load prg file error."));
						return 1;
					}
					for (int x=0;x<temp_count;x++)
					{
						CString temp_value;
						temp_value = temp_login_code.Left(2);
						temp_login_code = temp_login_code.Right(temp_login_code.GetLength()-2);
						temp_buffer[x] = Str_to_Byte(temp_value);
					}
					memcpy(&m_user_login_data.at(i),temp_buffer,sizeof(Str_userlogin_point));
				}


				for (int i=0; i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
				{
					CString temp_section;
					CString temp_code;
					unsigned char * temp_point = NULL;
					char temp_buffer[400];
					memset(temp_buffer,0,400);
					temp_section.Format(_T("AlalogTable_%d"),i);

					CString temp_analog_unit_code;
					GetPrivateProfileStringW(_T("AlalogCusTable"),temp_section,_T(""),temp_analog_unit_code.GetBuffer(4000),4000,FilePath);
					temp_analog_unit_code.ReleaseBuffer();

					int temp_count = temp_analog_unit_code.GetLength()/2;
					if(temp_count != sizeof(Str_table_point))
					{
						AfxMessageBox(_T("Load prg file error."));
						return 1;
					}
					for (int x=0;x<temp_count;x++)
					{
						CString temp_value;
						temp_value = temp_analog_unit_code.Left(2);
						temp_analog_unit_code = temp_analog_unit_code.Right(temp_analog_unit_code.GetLength()-2);
						temp_buffer[x] = Str_to_Byte(temp_value);
					}
					memcpy(&m_analog_custmer_range.at(i),temp_buffer,sizeof(Str_table_point));
				}			

				for (int i=0; i<BAC_CUSTOMER_UNITS_COUNT; i++)
				{
					CString temp_section;
					CString temp_code;
					unsigned char * temp_point = NULL;
					char temp_buffer[400];
					memset(temp_buffer,0,400);
					temp_section.Format(_T("UnitData_%d"),i);

					CString temp_digital_unit_code;
					GetPrivateProfileStringW(_T("Cust_Digital_Unit"),temp_section,_T(""),temp_digital_unit_code.GetBuffer(4000),4000,FilePath);
					temp_digital_unit_code.ReleaseBuffer();

					int temp_count = temp_digital_unit_code.GetLength()/2;
					if(temp_count != sizeof(Str_Units_element))
					{
						AfxMessageBox(_T("Load prg file error."));
						return 1;
					}
					for (int x=0;x<temp_count;x++)
					{
						CString temp_value;
						temp_value = temp_digital_unit_code.Left(2);
						temp_digital_unit_code = temp_digital_unit_code.Right(temp_digital_unit_code.GetLength()-2);
						temp_buffer[x] = Str_to_Byte(temp_value);
					}
					memcpy(&m_customer_unit_data.at(i),temp_buffer,sizeof(Str_Units_element));
				}

			}
			else if(ntemp_version >= 4)
			{
				for (int i=0; i<BAC_BASIC_SETTING_COUNT; i++)
				{
					CString temp_section;
					CString temp_code;
					unsigned char * temp_point = NULL;
					char temp_buffer[400];
					memset(temp_buffer,0,400);
					temp_section.Format(_T("DeviceSetting_%d"),i);

					CString temp_setting_code;
					GetPrivateProfileStringW(_T("DeviceSetting"),temp_section,_T(""),temp_setting_code.GetBuffer(4000),4000,FilePath);
					temp_setting_code.ReleaseBuffer();

					int temp_count = temp_setting_code.GetLength()/2;
					if(temp_count != sizeof(Str_Setting_Info))
					{
						AfxMessageBox(_T("Load prg file error."));
						return 1;
					}
					for (int x=0;x<temp_count;x++)
					{
						CString temp_value;
						temp_value = temp_setting_code.Left(2);
						temp_setting_code = temp_setting_code.Right(temp_setting_code.GetLength()-2);
						temp_buffer[x] = Str_to_Byte(temp_value);
					}
					memcpy(&Device_Basic_Setting.reg.ip_addr[0],temp_buffer,sizeof(Str_Setting_Info));
					for (int a=0;a<4;a++)
					{
						Device_Basic_Setting.reg.ip_addr[a] = 0;
						Device_Basic_Setting.reg.subnet[a] = 0;
						Device_Basic_Setting.reg.gate_addr[a] = 0;
					}
					Device_Basic_Setting.reg.n_serial_number = g_selected_serialnumber;
					Device_Basic_Setting.reg.object_instance = 0;
				}
			}
			DeleteFile(new_file);
		}
		else
		{
			char * temp_point = temp_buffer;
			for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
			{
				memcpy(&m_Input_data.at(i),temp_point ,sizeof(Str_in_point));
				temp_point = temp_point + sizeof(Str_in_point);
			}

			for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
			{
				memcpy(&m_Output_data.at(i),temp_point,sizeof(Str_out_point));
				temp_point = temp_point + sizeof(Str_out_point);
			}

			for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
			{
				memcpy(&m_Variable_data.at(i),temp_point,sizeof(Str_variable_point));
				temp_point = temp_point + sizeof(Str_variable_point);
			}

			for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
			{
				memcpy(&m_Program_data.at(i),temp_point,sizeof(Str_program_point));
				temp_point = temp_point + sizeof(Str_program_point);
			}

			for (int i=0; i<BAC_PID_COUNT; i++)
			{
				memcpy(&m_controller_data.at(i),temp_point,sizeof(Str_controller_point));
				temp_point = temp_point + sizeof(Str_controller_point);
				if((m_controller_data.at(i).input.panel != Station_NUM) && (m_controller_data.at(i).input.panel != 0))	 //在load prg 的时候 如果加载的panel != 自己的 就变成自己的
					m_controller_data.at(i).input.panel = Station_NUM;
				if((m_controller_data.at(i).setpoint.panel != Station_NUM) &&  (m_controller_data.at(i).setpoint.panel != 0) )	 //在load prg 的时候 如果加载的panel != 自己的 就变成自己的
					m_controller_data.at(i).setpoint.panel = Station_NUM;
			}

			for (int i=0; i<BAC_SCREEN_COUNT; i++)
			{
				memcpy(&m_screen_data.at(i),temp_point,sizeof(Control_group_point));
				temp_point = temp_point + sizeof(Control_group_point);
			}

			for (int i=0; i<BAC_GRPHIC_LABEL_COUNT; i++)
			{
				memcpy(&m_graphic_label_data.at(i),temp_point,sizeof(Str_label_point));
				temp_point = temp_point + sizeof(Str_label_point);
				if((m_graphic_label_data.at(i).reg.nMain_Panel != Station_NUM) && (m_graphic_label_data.at(i).reg.nMain_Panel != 0))
				{
					m_graphic_label_data.at(i).reg.nMain_Panel = Station_NUM;
					m_graphic_label_data.at(i).reg.nSub_Panel = Station_NUM;
				}
			}

			for (int i=0; i<BAC_USER_LOGIN_COUNT; i++)
			{
				memcpy(&m_user_login_data.at(i),temp_point,sizeof(Str_userlogin_point));
				temp_point = temp_point + sizeof(Str_userlogin_point);
			}

			for (int i=0; i<BAC_CUSTOMER_UNITS_COUNT; i++)
			{
				memcpy(&m_customer_unit_data.at(i),temp_point,sizeof(Str_Units_element));
				temp_point = temp_point + sizeof(Str_Units_element);
			}

			for (int i=0; i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
			{
				memcpy(&m_analog_custmer_range.at(i),temp_point,sizeof(Str_table_point));
				temp_point = temp_point + sizeof(Str_table_point);
			}

			memcpy(&Device_Basic_Setting,temp_point,sizeof(Str_Setting_Info));
			temp_point = temp_point + sizeof(Str_Setting_Info);


			for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
			{
				memcpy(&m_Weekly_data.at(i),temp_point,sizeof(Str_weekly_routine_point));
				temp_point = temp_point + sizeof(Str_weekly_routine_point);
			}

			for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
			{
				memcpy(&m_Annual_data.at(i),temp_point,sizeof(Str_annual_routine_point));
				temp_point = temp_point + sizeof(Str_annual_routine_point);
			}

			for (int i=0; i<BAC_MONITOR_COUNT; i++)
			{
				memcpy(&m_monitor_data.at(i),temp_point,sizeof(Str_monitor_point));
				temp_point = temp_point + sizeof(Str_monitor_point);
				for (int x=0;x<MAX_POINTS_IN_MONITOR;x++)
				{
					if(m_monitor_data.at(i).inputs[x].panel != Station_NUM)	 //在load prg 的时候 如果加载的panel != 自己的 就变成自己的
					{
						if((m_monitor_data.at(i).inputs[x].panel == m_monitor_data.at(i).inputs[x].sub_panel) && (m_monitor_data.at(i).inputs[x].panel != 0))
						{
							m_monitor_data.at(i).inputs[x].sub_panel = Station_NUM;
							m_monitor_data.at(i).inputs[x].panel = Station_NUM;
						}
					}
				}
			}

			for (int i=0; i<BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
			{
				memcpy(weeklt_time_schedule[i],temp_point,WEEKLY_SCHEDULE_SIZE);
				temp_point = temp_point + WEEKLY_SCHEDULE_SIZE;
			}

			for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
			{
				memcpy(g_DayState[i],temp_point,ANNUAL_CODE_SIZE);
				temp_point = temp_point + ANNUAL_CODE_SIZE;
			}

			for (int i=0; i<BAC_PROGRAMCODE_ITEM_COUNT; i++)
			{
				memcpy(program_code[i],temp_point,2000);
				temp_point = temp_point + 2000;
				program_code_length[i] = 2000;
			}

			if(ntemp_version >= 6)	//第六版中新增的 prg 要读写 
			{
				for (int i=0; i<BAC_VARIABLE_CUS_UNIT_COUNT; i++)
				{
					memcpy(&m_variable_analog_unite.at(i),temp_point,sizeof(Str_variable_uint_point));
					temp_point = temp_point + sizeof(Str_variable_uint_point);
				}
			}
		}






        if(write_to_device)	//如果是客户手动load 就让客户选择路径，不是手动load就说明是读缓存;
        {
            if(g_protocol == PROTOCOL_BIP_TO_MSTP)
            {

            }
            else
            {
                CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
				pFrame->m_prg_version = ntemp_version;
                pFrame->Show_Wait_Dialog_And_SendConfigMessage();
            }

        }
        else
        {
            if(Input_Window->IsWindowVisible())
                ::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
            else if(Output_Window->IsWindowVisible())
                ::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
            else if(Variable_Window->IsWindowVisible())
                ::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			else if(WeeklyRoutine_Window->IsWindowVisible())
				 ::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
			else if(AnnualRoutine_Window->IsWindowVisible())
				::PostMessage(m_annual_dlg_hwnd,WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
			else if(Controller_Window->IsWindowVisible())
				::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
			else if(Screen_Window->IsWindowVisible())
				::PostMessage(m_screen_dlg_hwnd, WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
			else if(Monitor_Window->IsWindowVisible())
			{
				::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
				::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
			}
			else if(AlarmLog_Window->IsWindowVisible())
				::PostMessage(m_alarmlog_dlg_hwnd, WM_REFRESH_BAC_ALARMLOG_LIST,NULL,NULL);
			else if(Setting_Window->IsWindowVisible())
				 ::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
			else if(Program_Window->IsWindowEnabled())
				::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
				

        }

    }


    return 0;
}


int LoadMiniModbusConfigFile(LPCTSTR tem_read_path)
{
	CString FilePath;
	FilePath.Format(_T("%s"),tem_read_path);
	CFileFind temp_find;
	if(!temp_find.FindFile(FilePath))
		return -1;

	CFile myfile(tem_read_path,CFile::modeRead);
	char *pBuf;
	DWORD dwFileLen;
	dwFileLen=myfile.GetLength();
	pBuf= new char[dwFileLen+1];
	memset(pBuf,0,dwFileLen);
	pBuf[dwFileLen]=0;
	myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
	myfile.Close();
	//MessageBox(pBuf);
	char * temp_point = pBuf;
	if(temp_point[0] != 4)
		return -1;
	temp_point = temp_point + 1;

	unsigned long temp_file_time;
	memcpy(&temp_file_time,temp_point,4);

	CTime temp_time_now = CTime::GetCurrentTime();
	unsigned long temp_cur_long_time = temp_time_now.GetTime();

	//如果485 的prg时间 是三天以前的  或者485文件的时间大于现在的时间，这个prg 文件 就不要了;
	//if((temp_cur_long_time - temp_file_time > 259200) || (temp_file_time > temp_cur_long_time))
	//{
	//	return -1;
	//}
	temp_point = temp_point + 4;

	unsigned short temp_buffer[50000];
	memset(temp_buffer ,0,50000*2);

	unsigned short write_buffer[200];

	memcpy(temp_buffer,temp_point,dwFileLen-5);

	memcpy(&Device_Basic_Setting.reg,temp_buffer,400); //Setting 的400个字节;
	//g_progress_persent = (i+1)*100 / 32;	
	int total_count = BAC_OUTPUT_ITEM_COUNT + BAC_INPUT_ITEM_COUNT;
	int resent_count = 0;
	for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		memcpy( &m_Output_data.at(i),temp_buffer + 200 + i*23,sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;

		memset(write_buffer,0,sizeof(write_buffer));
		memcpy( write_buffer,&m_Output_data.at(i),sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
		for (int x=0;x<200;x++)
		{
			write_buffer[x] = htons(write_buffer[x]);
		}
		if(Write_Multi_org_short(g_tstat_id,write_buffer,10000 + 23 * i , 23, 10)> 0)
		{
			resent_count ++ ;
			g_progress_persent = (resent_count*100) / total_count;	
		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Write config file timeout!"));
			g_progress_persent = 0;
			return -1;
		}
		Sleep(10);
	}
	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Write  output OK!"));
	for (int j=0;j<BAC_INPUT_ITEM_COUNT;j++)
	{
		memcpy(&m_Input_data.at(j),temp_buffer + 200 + 23*64 + j*23,sizeof(Str_in_point)); //Input 46 个字节 ;

		memset(write_buffer,0,sizeof(write_buffer));
		memcpy( write_buffer,&m_Input_data.at(j),sizeof(Str_in_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
		for (int Z=0;Z<200;Z++)
		{
			write_buffer[Z] = htons(write_buffer[Z]);
		}
		if(Write_Multi_org_short(g_tstat_id,write_buffer,BAC_IN_START_REG + 23 * j , 23, 10)>0)
		{
			resent_count ++ ;
			g_progress_persent = (resent_count*100) / total_count;	

		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Write config file timeout!"));
			g_progress_persent = 0;
			return -1;
		}
		Sleep(10);
	}
	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Write input OK!"));
	return 1;

}



int LoadModbusConfigFile_Cache(LPCTSTR tem_read_path)
{
	CString FilePath;
	FilePath.Format(_T("%s"),tem_read_path);
	CFileFind temp_find;
	if(!temp_find.FindFile(FilePath))
		return -1;

	CFile myfile(tem_read_path,CFile::modeRead);
	char *pBuf;
	DWORD dwFileLen;
	dwFileLen=myfile.GetLength();
	pBuf= new char[dwFileLen+1];
	memset(pBuf,0,dwFileLen);
	pBuf[dwFileLen]=0;
	myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
	myfile.Close();
	//MessageBox(pBuf);
	char * temp_point = pBuf;
	if(temp_point[0] != 4)
		return -1;
	temp_point = temp_point + 1;

	unsigned long temp_file_time;
	memcpy(&temp_file_time,temp_point,4);

	CTime temp_time_now = CTime::GetCurrentTime();
	unsigned long temp_cur_long_time = temp_time_now.GetTime();

	//如果485 的prg时间 是三天以前的  或者485文件的时间大于现在的时间，这个prg 文件 就不要了;
	if((temp_cur_long_time - temp_file_time > 259200) || (temp_file_time > temp_cur_long_time))
	{
		return -1;
	}
	temp_point = temp_point + 4;

	unsigned short temp_buffer[50000];
	memset(temp_buffer ,0,50000*2);

	memcpy(temp_buffer,temp_point,dwFileLen-5);


	memcpy(&Device_Basic_Setting.reg,temp_buffer,400); //Setting 的400个字节;

	for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		memcpy( &m_Output_data.at(i),temp_buffer + 200 + i*23,sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
	}

	for (int j=0;j<BAC_INPUT_ITEM_COUNT;j++)
	{
		memcpy(&m_Input_data.at(j),temp_buffer + 200 + 23*64 + j*23,sizeof(Str_in_point)); //Input 46 个字节 ;
	}

	return 1;

}

void Copy_Data_From_485_to_Bacnet(unsigned short *start_point)
{
	memcpy(&Device_Basic_Setting.reg,start_point,400); //Setting 的400个字节;

	for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		memcpy( &m_Output_data.at(i),start_point + 200 + i*23,sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
	}

	for (int j=0;j<BAC_INPUT_ITEM_COUNT;j++)
	{
		memcpy(&m_Input_data.at(j),start_point + 200 + 23*64 + j*23,sizeof(Str_in_point)); //Input 46 个字节 ;
	}

	//memcpy(&Device_Basic_Setting.reg,&read_data_buffer[0],400); //Setting 的400个字节;

	//for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	//{
	//	memcpy( &m_Output_data.at(i),&read_data_buffer[200 + i*23],sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
	//}

	//for (int j=0;j<BAC_INPUT_ITEM_COUNT;j++)
	//{
	//	memcpy(&m_Input_data.at(j),&read_data_buffer[200 + 23*64 + j*23],sizeof(Str_in_point)); //Input 46 个字节 ;
	//}
}

int LoadBacnetConfigFile_Cache(LPCTSTR tem_read_path)
{
    if((g_mac!=0) &&(g_bac_instance!=0))
    {

        CString FilePath;
        FilePath.Format(_T("%s"),tem_read_path);
        CFileFind temp_find;
        if(!temp_find.FindFile(FilePath))
            return -1;
#if 1
        CFile myfile(tem_read_path,CFile::modeRead);
        char *pBuf;
        DWORD dwFileLen;
        dwFileLen=myfile.GetLength();
        pBuf= new char[dwFileLen+1];
        memset(pBuf,0,dwFileLen);
        pBuf[dwFileLen]=0;
        myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
        myfile.Close();
        //MessageBox(pBuf);
        char * temp_point = pBuf;

#endif
        if(temp_point[0] != 4)
            return -1;
        temp_point = temp_point + 1;
        for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
        {
            memcpy(&m_Input_data.at(i) ,temp_point,sizeof(Str_in_point));
            temp_point = temp_point + sizeof(Str_in_point);
        }

        for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
        {
            memcpy(&m_Output_data.at(i),temp_point,sizeof(Str_out_point));
            temp_point = temp_point + sizeof(Str_out_point);
        }

        for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
        {
            memcpy(&m_Variable_data.at(i),temp_point,sizeof(Str_variable_point));
            temp_point = temp_point + sizeof(Str_variable_point);
        }

        for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
        {
            memcpy(&m_Program_data.at(i),temp_point,sizeof(Str_program_point));
            temp_point = temp_point + sizeof(Str_program_point);
        }

        for (int i=0; i<BAC_PID_COUNT; i++)
        {
            memcpy(&m_controller_data.at(i),temp_point,sizeof(Str_controller_point));
            temp_point = temp_point + sizeof(Str_controller_point);
        }

        for (int i=0; i<BAC_SCREEN_COUNT; i++)
        {
            memcpy(&m_screen_data.at(i) ,temp_point,sizeof(Control_group_point));
            temp_point = temp_point + sizeof(Control_group_point);
        }


        for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
        {
            memcpy(&m_Weekly_data.at(i) ,temp_point,sizeof(Str_weekly_routine_point));
            temp_point = temp_point + sizeof(Str_weekly_routine_point);
        }

        for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
        {
            memcpy(&m_Annual_data.at(i) ,temp_point,sizeof(Str_annual_routine_point));
            temp_point = temp_point + sizeof(Str_annual_routine_point);
        }

        for (int i=0; i<BAC_MONITOR_COUNT; i++)
        {
            memcpy(&m_monitor_data.at(i) ,temp_point,sizeof(Str_monitor_point));
            temp_point = temp_point + sizeof(Str_monitor_point);
        }

        for (int i=0; i<BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
        {
            memcpy(weeklt_time_schedule[i] ,temp_point,WEEKLY_SCHEDULE_SIZE);
            temp_point = temp_point + WEEKLY_SCHEDULE_SIZE;
        }

        for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
        {
            memcpy(g_DayState[i] ,temp_point,ANNUAL_CODE_SIZE);
            temp_point = temp_point + ANNUAL_CODE_SIZE;
        }

        memcpy(&Device_Basic_Setting,temp_point,sizeof(Str_Setting_Info));
        temp_point = temp_point + sizeof(Str_Setting_Info);

        for (int i=0; i<BAC_GRPHIC_LABEL_COUNT; i++)
        {
            memcpy(&m_graphic_label_data.at(i),temp_point ,sizeof(Str_label_point));
            temp_point = temp_point + sizeof(Str_label_point);
        }
		copy_data_to_ptrpanel(TYPE_ALL);
        //copy_data_to_ptrpanel(TYPE_INPUT);
        //copy_data_to_ptrpanel(TYPE_VARIABLE);
        //copy_data_to_ptrpanel(TYPE_OUTPUT);
        //copy_data_to_ptrpanel(TYPE_WEEKLY);
        //copy_data_to_ptrpanel(TYPE_ANNUAL);
        if (pBuf)
        {
            delete pBuf;
            pBuf = NULL;
        }
        if(Input_Window->IsWindowVisible())
            ::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
        else if(Output_Window->IsWindowVisible())
            ::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
        else if(Variable_Window->IsWindowVisible())
            ::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
        else if(Screen_Window->IsWindowVisible())
            ::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
        else if(Program_Window->IsWindowVisible())
            ::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
        else if(Controller_Window->IsWindowVisible())
            ::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
        else if(Monitor_Window->IsWindowVisible())
        {
            ::PostMessage(m_monitor_dlg_hwnd,WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
            ::PostMessage(m_monitor_dlg_hwnd,WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
        }
        else if(Setting_Window->IsWindowVisible())
        {
            ::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
        }

    }


    return 0;
}



//第一个字节 版本
//然后是 4个字节的 时间 如果时间太久了 也需要重新获取;
int SaveModbusConfigFile(CString &SaveConfigFilePath)
{
	CString FilePath;
	CStringArray temp_array1;


	unsigned char  read_device_id = 0;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	read_device_id =  pFrame->m_product.at(selected_product_index).product_id;
	if(read_device_id != g_tstat_id)
		read_device_id = g_tstat_id;
	SplitCStringA(temp_array1,SaveConfigFilePath,_T("."));
	int temp_array_size=0;
	temp_array_size = temp_array1.GetSize();
	if(temp_array1.GetSize()<=1)
	{
		return -1;
	}


	int right_suffix = temp_array1.GetAt(temp_array_size - 1).GetLength();
	int config_file_length = SaveConfigFilePath.GetLength();
	if(config_file_length <= right_suffix)
	{
		return -1;
	}
	CFileFind tempfind;
	if(tempfind.FindFile(SaveConfigFilePath))
	{
		DeleteFile(SaveConfigFilePath);
	}


	bool read_result = true;
	unsigned int buffer_length = 3200;
	unsigned short read_data_buffer[3200];
	memset(read_data_buffer,0,sizeof(unsigned short)*3200);
	//output 45 按46算  *64  + input 46  *64  需要读2944;
	for(int i=0; i<32; i++)
	{
		//register_critical_section.Lock();
		//int nStart = GetTickCount();
		int itemp = 0;
		itemp = Read_Multi(read_device_id,&read_data_buffer[i*100],BAC_SETTING_START_REG+i*100,100,4);
		if(itemp < 0 )
		{
			read_result = false;
			break; 
		}
		else
		{
			g_progress_persent = (i+1)*100 / 32;	
		}
		Sleep(100);
	}
	if(read_result)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Inputs and Outputs OK!"));
	}

	Copy_Data_From_485_to_Bacnet(&read_data_buffer[0]);





	char *npoint = (char *)read_data_buffer[0];
	unsigned int bufferlength = 3200 * 2;





	char temp_buffer[50000];
	char * temp_point = NULL;
	memset(temp_buffer ,0,50000);
	//FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
	//FilePath = FilePath + _T("ini");
	temp_buffer[0] = 4;
	temp_point = temp_buffer + 1;

#pragma region get_time_area
	CTime temp_save_prg_time = CTime::GetCurrentTime();
	unsigned long prg_temp_long_time = temp_save_prg_time.GetTime();

	memcpy(temp_buffer + 1,&prg_temp_long_time,4);

#pragma endregion get_time_area

	DWORD dwFileLen;
	HANDLE hFile;
	DWORD dWrites;
	if(npoint != NULL)
	{
		memcpy(temp_buffer + 5,npoint,bufferlength);
		temp_point = temp_point + bufferlength;
		dwFileLen = temp_point - temp_buffer;
		hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
		WriteFile(hFile,temp_buffer,dwFileLen,&dWrites,NULL);
		CloseHandle(hFile);
	}
	else //第二个参数 如果是Null 就是update;
	{
		char temp_update_buffer[50000];
		memset(temp_update_buffer,0,50000);

		memcpy(temp_update_buffer,&Device_Basic_Setting.reg,400); //Setting 的400个字节;
		for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
		{
			memcpy(temp_update_buffer + 400 + i*(23*2), &m_Output_data.at(i),sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
		}

		for (int j=0;j<BAC_INPUT_ITEM_COUNT;j++)
		{
			memcpy(temp_update_buffer + 400 + 23*2*64 + j*23*2,&m_Input_data.at(j),sizeof(Str_in_point)); //Input 46 个字节 ;
		}
		memcpy(temp_buffer + 5,temp_update_buffer,bufferlength);
		temp_point = temp_point + bufferlength;
		dwFileLen = bufferlength + 5;//这5个字节是 版本和时间 ;
		hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
		WriteFile(hFile,temp_buffer,dwFileLen,&dWrites,NULL);
		CloseHandle(hFile);
	}


	return true;
}




//第一个字节 版本
//然后是 4个字节的 时间 如果时间太久了 也需要重新获取;
void SaveModbusConfigFile_Cache(CString &SaveConfigFilePath,char *npoint,unsigned int bufferlength)
{
	CString FilePath;
	CStringArray temp_array1;

	SplitCStringA(temp_array1,SaveConfigFilePath,_T("."));
	int temp_array_size=0;
	temp_array_size = temp_array1.GetSize();
	if(temp_array1.GetSize()<=1)
	{
		return;
	}


	int right_suffix = temp_array1.GetAt(temp_array_size - 1).GetLength();
	int config_file_length = SaveConfigFilePath.GetLength();
	if(config_file_length <= right_suffix)
	{
		return;
	}
	CFileFind tempfind;
	if(tempfind.FindFile(SaveConfigFilePath))
	{
		DeleteFile(SaveConfigFilePath);
	}
	char temp_buffer[50000];
	char * temp_point = NULL;
	memset(temp_buffer ,0,50000);
	//FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
	//FilePath = FilePath + _T("ini");
	temp_buffer[0] = 4;
	temp_point = temp_buffer + 1;

#pragma region get_time_area
	CTime temp_save_prg_time = CTime::GetCurrentTime();
	unsigned long prg_temp_long_time = temp_save_prg_time.GetTime();

	memcpy(temp_buffer + 1,&prg_temp_long_time,4);

#pragma endregion get_time_area

	DWORD dwFileLen;
	HANDLE hFile;
	DWORD dWrites;
	if(npoint != NULL)
	{
		memcpy(temp_buffer + 5,npoint,bufferlength);
		temp_point = temp_point + bufferlength;
		dwFileLen = temp_point - temp_buffer;
		hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
		WriteFile(hFile,temp_buffer,dwFileLen,&dWrites,NULL);
		CloseHandle(hFile);
	}
	else //第二个参数 如果是Null 就是update;
	{
		char temp_update_buffer[50000];
		memset(temp_update_buffer,0,50000);

		memcpy(temp_update_buffer,&Device_Basic_Setting.reg,400); //Setting 的400个字节;
		for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
		{
			memcpy(temp_update_buffer + 400 + i*(23*2), &m_Output_data.at(i),sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
		}

		for (int j=0;j<BAC_INPUT_ITEM_COUNT;j++)
		{
			memcpy(temp_update_buffer + 400 + 23*2*64 + j*23*2,&m_Input_data.at(j),sizeof(Str_in_point)); //Input 46 个字节 ;
		}
		memcpy(temp_buffer + 5,temp_update_buffer,bufferlength);
		temp_point = temp_point + bufferlength;
		dwFileLen = bufferlength + 5;//这5个字节是 版本和时间 ;
		hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
		WriteFile(hFile,temp_buffer,dwFileLen,&dWrites,NULL);
		CloseHandle(hFile);
	}



}


void SaveBacnetConfigFile_Cache(CString &SaveConfigFilePath)
{
    if((g_mac!=0) &&(g_bac_instance!=0))
    {
        CString FilePath;
        CStringArray temp_array1;

        SplitCStringA(temp_array1,SaveConfigFilePath,_T("."));
        int temp_array_size=0;
        temp_array_size = temp_array1.GetSize();
        if(temp_array1.GetSize()<=1)
        {
            //MessageBox(_T("Prg file error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }


        int right_suffix = temp_array1.GetAt(temp_array_size - 1).GetLength();
        int config_file_length = SaveConfigFilePath.GetLength();
        if(config_file_length <= right_suffix)
        {
            //MessageBox(_T("Prg file error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }
        CFileFind tempfind;
        if(tempfind.FindFile(SaveConfigFilePath))
        {
            DeleteFile(SaveConfigFilePath);
        }
        char temp_buffer[50000];
        char * temp_point = NULL;
        memset(temp_buffer ,0,50000);
        //FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
        //FilePath = FilePath + _T("ini");
        temp_buffer[0] = 4;
        temp_point = temp_buffer + 1;
        for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
        {
            memcpy(temp_point ,(char *)m_Input_data.at(i).description,sizeof(Str_in_point));
            temp_point = temp_point + sizeof(Str_in_point);
        }

        for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
        {
            memcpy(temp_point,(char *)m_Output_data.at(i).description,sizeof(Str_out_point));
            temp_point = temp_point + sizeof(Str_out_point);
        }

        for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
        {
            memcpy(temp_point,(char *)m_Variable_data.at(i).description,sizeof(Str_variable_point));
            temp_point = temp_point + sizeof(Str_variable_point);
        }

        for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
        {
            memcpy(temp_point,(char *)m_Program_data.at(i).description,sizeof(Str_program_point));
            temp_point = temp_point + sizeof(Str_program_point);
        }

        for (int i=0; i<BAC_PID_COUNT; i++)
        {
            memcpy(temp_point,&m_controller_data.at(i),sizeof(Str_controller_point));
            temp_point = temp_point + sizeof(Str_controller_point);
        }

        for (int i=0; i<BAC_SCREEN_COUNT; i++)
        {
            memcpy(temp_point ,&m_screen_data.at(i),sizeof(Control_group_point));
            temp_point = temp_point + sizeof(Control_group_point);
        }


        for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
        {
            memcpy(temp_point ,&m_Weekly_data.at(i),sizeof(Str_weekly_routine_point));
            temp_point = temp_point + sizeof(Str_weekly_routine_point);
        }

        for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
        {
            memcpy(temp_point ,&m_Annual_data.at(i),sizeof(Str_annual_routine_point));
            temp_point = temp_point + sizeof(Str_annual_routine_point);
        }

        for (int i=0; i<BAC_MONITOR_COUNT; i++)
        {
            memcpy(temp_point ,&m_monitor_data.at(i),sizeof(Str_monitor_point));
            temp_point = temp_point + sizeof(Str_monitor_point);
        }

        for (int i=0; i<BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
        {
            memcpy(temp_point ,weeklt_time_schedule[i],WEEKLY_SCHEDULE_SIZE);
            temp_point = temp_point + WEEKLY_SCHEDULE_SIZE;
        }

        for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
        {
            memcpy(temp_point ,g_DayState[i],ANNUAL_CODE_SIZE);
            temp_point = temp_point + ANNUAL_CODE_SIZE;
        }

        memcpy(temp_point,&Device_Basic_Setting,sizeof(Str_Setting_Info));
        temp_point = temp_point + sizeof(Str_Setting_Info);

        for (int i=0; i<BAC_GRPHIC_LABEL_COUNT; i++)
        {
            memcpy(temp_point ,&m_graphic_label_data.at(i),sizeof(Str_label_point));
            temp_point = temp_point + sizeof(Str_label_point);
        }

        CFileFind temp_file_find;
        if(temp_file_find.FindFile(SaveConfigFilePath))
        {
            DeleteFile(SaveConfigFilePath);
        }

        DWORD dwFileLen;
        dwFileLen = temp_point - temp_buffer;
        HANDLE hFile;
        hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        DWORD dWrites;
        WriteFile(hFile,temp_buffer,dwFileLen,&dWrites,NULL);
        CloseHandle(hFile);

    }
}

void SaveBacnetBinaryFile(CString &SaveConfigFilePath)
{

	char pBuf[200000];
	memset(pBuf,0,200000);
	pBuf[0] = 0x55;
	pBuf[1] = 0xff;
	pBuf[2] = 0x06;//version
	char * original_point = NULL;
	char * temp_point = NULL;
	original_point = pBuf;
	temp_point = pBuf;
	temp_point = temp_point + 3;
	for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
	{
		memcpy(temp_point ,(char *)m_Input_data.at(i).description,sizeof(Str_in_point));
		temp_point = temp_point + sizeof(Str_in_point);
	}

	for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
	{
		memcpy(temp_point,(char *)m_Output_data.at(i).description,sizeof(Str_out_point));
		temp_point = temp_point + sizeof(Str_out_point);
	}

	for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
	{
		memcpy(temp_point,(char *)m_Variable_data.at(i).description,sizeof(Str_variable_point));
		temp_point = temp_point + sizeof(Str_variable_point);
	}

	for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
	{
		memcpy(temp_point,(char *)m_Program_data.at(i).description,sizeof(Str_program_point));
		temp_point = temp_point + sizeof(Str_program_point);
	}

	for (int i=0; i<BAC_PID_COUNT; i++)
	{
		memcpy(temp_point,&m_controller_data.at(i),sizeof(Str_controller_point));
		temp_point = temp_point + sizeof(Str_controller_point);
	}

	for (int i=0; i<BAC_SCREEN_COUNT; i++)
	{
		memcpy(temp_point ,&m_screen_data.at(i),sizeof(Control_group_point));
		temp_point = temp_point + sizeof(Control_group_point);
	}

	for (int i=0; i<BAC_GRPHIC_LABEL_COUNT; i++)
	{
		memcpy(temp_point ,&m_graphic_label_data.at(i),sizeof(Str_label_point));
		temp_point = temp_point + sizeof(Str_label_point);
	}

	for (int i=0; i<BAC_USER_LOGIN_COUNT; i++)
	{
		memcpy(temp_point ,&m_user_login_data.at(i),sizeof(Str_userlogin_point));
		temp_point = temp_point + sizeof(Str_userlogin_point);
	}

	for (int i=0; i<BAC_CUSTOMER_UNITS_COUNT; i++)
	{
		memcpy(temp_point ,&m_customer_unit_data.at(i),sizeof(Str_Units_element));
		temp_point = temp_point + sizeof(Str_Units_element);
	}

	for (int i=0; i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
	{
		memcpy(temp_point ,&m_analog_custmer_range.at(i),sizeof(Str_table_point));
		temp_point = temp_point + sizeof(Str_table_point);
	}

	memcpy(temp_point,&Device_Basic_Setting,sizeof(Str_Setting_Info));
	temp_point = temp_point + sizeof(Str_Setting_Info);


	for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
	{
		memcpy(temp_point ,&m_Weekly_data.at(i),sizeof(Str_weekly_routine_point));
		temp_point = temp_point + sizeof(Str_weekly_routine_point);
	}

	for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
	{
		memcpy(temp_point ,&m_Annual_data.at(i),sizeof(Str_annual_routine_point));
		temp_point = temp_point + sizeof(Str_annual_routine_point);
	}

	for (int i=0; i<BAC_MONITOR_COUNT; i++)
	{
		memcpy(temp_point ,&m_monitor_data.at(i),sizeof(Str_monitor_point));
		temp_point = temp_point + sizeof(Str_monitor_point);
	}

	for (int i=0; i<BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
	{
		memcpy(temp_point ,weeklt_time_schedule[i],WEEKLY_SCHEDULE_SIZE);
		temp_point = temp_point + WEEKLY_SCHEDULE_SIZE;
	}

	for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
	{
		memcpy(temp_point ,g_DayState[i],ANNUAL_CODE_SIZE);
		temp_point = temp_point + ANNUAL_CODE_SIZE;
	}

	for (int i=0; i<BAC_PROGRAMCODE_ITEM_COUNT; i++)
	{
		memcpy(temp_point ,program_code[i],2000);
		temp_point = temp_point + 2000;
	}

	for (int i=0; i<BAC_VARIABLE_CUS_UNIT_COUNT; i++)
	{
		memcpy(temp_point ,&m_variable_analog_unite.at(i),sizeof(Str_variable_uint_point));
		temp_point = temp_point + sizeof(Str_variable_uint_point);
	}

	int write_length = temp_point - original_point;

	CFileFind tempfind;
	if(tempfind.FindFile(SaveConfigFilePath))
	{
		DeleteFile(SaveConfigFilePath);
	}


	HANDLE hFile;
	hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);

	DWORD dWrites;
	WriteFile(hFile,pBuf,write_length,&dWrites,NULL);
	CloseHandle(hFile);
	//if(pBuf)
	//	delete pBuf;
}

void SaveBacnetConfigFile(CString &SaveConfigFilePath)
{
    if((g_mac!=0) &&(g_bac_instance!=0))
    {
        CString FilePath;
        CStringArray temp_array1;

        SplitCStringA(temp_array1,SaveConfigFilePath,_T("."));
        int temp_array_size=0;
        temp_array_size = temp_array1.GetSize();
        if(temp_array1.GetSize()<=1)
        {
            //MessageBox(_T("Prg file error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }


        int right_suffix = temp_array1.GetAt(temp_array_size - 1).GetLength();
        int config_file_length = SaveConfigFilePath.GetLength();
        if(config_file_length <= right_suffix)
        {
            //MessageBox(_T("Prg file error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
            return;
        }
        FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
        FilePath = FilePath + _T("ini");

        WritePrivateProfileStringW(_T("Setting"),_T("Version"),_T("4"),FilePath);
		//Version 3 加入了 BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT    BAC_GRPHIC_LABEL_COUNT    BAC_USER_LOGIN_COUNT    BAC_CUSTOMER_UNITS_COUNT
		//Version 4 加入了setting
        for (int i=0; i<BAC_INPUT_ITEM_COUNT; i++)
        {
            CString temp_input,temp_des,temp_csc;
            temp_input.Format(_T("Input%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).description, (int)strlen((char *)m_Input_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();
            temp_label.Replace(_T("-"),_T("_"));
            WritePrivateProfileStringW(temp_input,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_input,_T("Label"),temp_label,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_input,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%d"),m_Input_data.at(i).value);
            WritePrivateProfileStringW(temp_input,_T("Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).filter);
            WritePrivateProfileStringW(temp_input,_T("Filter"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).decom);
            WritePrivateProfileStringW(temp_input,_T("Decom"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).sub_id);
            WritePrivateProfileStringW(temp_input,_T("Sen_On"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).sub_product);
            WritePrivateProfileStringW(temp_input,_T("Sen_Off"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).control);
            WritePrivateProfileStringW(temp_input,_T("Control"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).digital_analog);
            WritePrivateProfileStringW(temp_input,_T("Digital_Analog"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).calibration_sign);
            WritePrivateProfileStringW(temp_input,_T("Calibration_Sign"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).sub_number);
            WritePrivateProfileStringW(temp_input,_T("Calibration_Increment"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).calibration_h);
            WritePrivateProfileStringW(temp_input,_T("Unused"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).calibration_l);
            WritePrivateProfileStringW(temp_input,_T("Calibration"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Input_data.at(i).range);
            WritePrivateProfileStringW(temp_input,_T("Range"),temp_csc,FilePath);
        }

        for (int i=0; i<BAC_OUTPUT_ITEM_COUNT; i++)
        {
            CString temp_section,temp_des,temp_csc;
            temp_section.Format(_T("Output%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();
            temp_label.Replace(_T("-"),_T("_"));
            WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);
            temp_csc.Format(_T("%d"),m_Output_data.at(i).value);
            WritePrivateProfileStringW(temp_section,_T("Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).digital_analog);
            WritePrivateProfileStringW(temp_section,_T("Digital_Analog"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).hw_switch_status);
            WritePrivateProfileStringW(temp_section,_T("hw_switch_status"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).control);
            WritePrivateProfileStringW(temp_section,_T("Control"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).digital_control);
            WritePrivateProfileStringW(temp_section,_T("Digital_Control"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).decom);
            WritePrivateProfileStringW(temp_section,_T("Decom"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).range);
            WritePrivateProfileStringW(temp_section,_T("Range"),temp_csc,FilePath);

            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).sub_id);
            WritePrivateProfileStringW(temp_section,_T("M_Del_Low"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).sub_product);
            WritePrivateProfileStringW(temp_section,_T("S_Del_High"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).sub_number);
            WritePrivateProfileStringW(temp_section,_T("Sub__number"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Output_data.at(i).pwm_period);
            WritePrivateProfileStringW(temp_section,_T("Delay_Timer"),temp_csc,FilePath);
        }

        for (int i=0; i<BAC_VARIABLE_ITEM_COUNT; i++)
        {
            CString temp_section,temp_des,temp_csc;
            temp_section.Format(_T("Variable%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();
            temp_label.Replace(_T("-"),_T("_"));
            WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);

            temp_csc.Format(_T("%d"),m_Variable_data.at(i).value);
            WritePrivateProfileStringW(temp_section,_T("Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).digital_analog);
            WritePrivateProfileStringW(temp_section,_T("Digital_Analog"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).control);
            WritePrivateProfileStringW(temp_section,_T("Control"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).unused);
            WritePrivateProfileStringW(temp_section,_T("Unused"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Variable_data.at(i).range);
            WritePrivateProfileStringW(temp_section,_T("Range"),temp_csc,FilePath);
        }

        for (int i=0; i<BAC_PROGRAM_ITEM_COUNT; i++)
        {
            CString temp_section,temp_des,temp_csc;
            temp_section.Format(_T("Program%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();

            WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);

            temp_csc.Format(_T("%u"),(unsigned short)m_Program_data.at(i).bytes);
            WritePrivateProfileStringW(temp_section,_T("Bytes"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).on_off);
            WritePrivateProfileStringW(temp_section,_T("On_Off"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).com_prg);
            WritePrivateProfileStringW(temp_section,_T("Com_Prg"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).errcode);
            WritePrivateProfileStringW(temp_section,_T("Errcode"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Program_data.at(i).unused);
            WritePrivateProfileStringW(temp_section,_T("Unused"),temp_csc,FilePath);

        }

        for (int i=0; i<BAC_PID_COUNT; i++)
        {
            CString temp_section,temp_des,temp_csc;
            temp_section.Format(_T("Controller%d"),i);

            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).input.number);
            WritePrivateProfileStringW(temp_section,_T("Input_Number"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).input.panel);
            WritePrivateProfileStringW(temp_section,_T("Input_Panel"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).input.point_type);
            WritePrivateProfileStringW(temp_section,_T("Input_Point_Type"),temp_csc,FilePath);
            temp_csc.Format(_T("%d"),m_controller_data.at(i).input_value);
            WritePrivateProfileStringW(temp_section,_T("Input_Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%d"),m_controller_data.at(i).value);
            WritePrivateProfileStringW(temp_section,_T("Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).setpoint.number);
            WritePrivateProfileStringW(temp_section,_T("Setpoint_Number"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).setpoint.panel);
            WritePrivateProfileStringW(temp_section,_T("Setpoint_Panel"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).setpoint.point_type);
            WritePrivateProfileStringW(temp_section,_T("Setpoint_Point_Type"),temp_csc,FilePath);

            temp_csc.Format(_T("%d"),m_controller_data.at(i).setpoint_value);
            WritePrivateProfileStringW(temp_section,_T("Setpoint_Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).units);
            WritePrivateProfileStringW(temp_section,_T("Units"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_section,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).action);
            WritePrivateProfileStringW(temp_section,_T("Action"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).repeats_per_min);
            WritePrivateProfileStringW(temp_section,_T("Repeats_Per_Min"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).sample_time);
            WritePrivateProfileStringW(temp_section,_T("Unused"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).prop_high);
            WritePrivateProfileStringW(temp_section,_T("Prop_High"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).proportional);
            WritePrivateProfileStringW(temp_section,_T("Proportional"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).reset);
            WritePrivateProfileStringW(temp_section,_T("Reset"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).bias);
            WritePrivateProfileStringW(temp_section,_T("Bias"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_controller_data.at(i).rate);
            WritePrivateProfileStringW(temp_section,_T("Rate"),temp_csc,FilePath);

        }

        for (int i=0; i<BAC_SCREEN_COUNT; i++)
        {
            CString temp_section,temp_des,temp_csc;
            temp_section.Format(_T("Screen%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).description, (int)strlen((char *)m_screen_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).label, (int)strlen((char *)m_screen_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();

            CString temp_pic_file;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).picture_file, (int)strlen((char *)m_screen_data.at(i).picture_file)+1,
                                 temp_pic_file.GetBuffer(MAX_PATH), MAX_PATH );
            temp_pic_file.ReleaseBuffer();

            WritePrivateProfileStringW(temp_section,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_section,_T("Label"),temp_label,FilePath);
            WritePrivateProfileStringW(temp_section,_T("Picture_file"),temp_pic_file,FilePath);

            temp_csc.Format(_T("%u"),(unsigned short)m_screen_data.at(i).update);
            WritePrivateProfileStringW(temp_section,_T("Update"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_screen_data.at(i).mode);
            WritePrivateProfileStringW(temp_section,_T("Mode"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_screen_data.at(i).xcur_grp);
            WritePrivateProfileStringW(temp_section,_T("Xcur_grp"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned short)m_screen_data.at(i).ycur_grp);
            WritePrivateProfileStringW(temp_section,_T("Ycur_grp"),temp_csc,FilePath);
        }

#pragma region Version_3_Add

		for (int i=0; i<BAC_GRPHIC_LABEL_COUNT; i++)
		{
			CString temp_section,temp_des,temp_csc;
			CString temp_label_code;
			temp_section.Format(_T("LabelData_%d"),i);

			char temp_buffer[400];
			memset(temp_buffer,0,400);
			memcpy(temp_buffer,&m_graphic_label_data.at(i),sizeof(Str_label_point));

			int temp_value = sizeof(Str_label_point);
			temp_label_code.Empty();
			for (int j=0; j<temp_value; j++)
			{
				temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_buffer + j)));
				temp_csc.MakeUpper();
				temp_label_code = temp_label_code + temp_csc;
			}

			WritePrivateProfileStringW(_T("GraphicLabel"),temp_section,temp_label_code,FilePath);
		}

		for (int i=0; i<BAC_USER_LOGIN_COUNT; i++)
		{
			CString temp_section,temp_des,temp_csc;
			CString temp_userlogin_code;
			temp_section.Format(_T("Userlogin_%d"),i);

			char login_buffer[400];
			memset(login_buffer,0,400);
			memcpy(login_buffer,&m_user_login_data.at(i),sizeof(Str_userlogin_point));


			temp_userlogin_code.Empty();
			for (int j=0; j<sizeof(Str_userlogin_point); j++)
			{
				temp_csc.Format(_T("%02x"),(unsigned char)(*(login_buffer + j)));
				temp_csc.MakeUpper();
				temp_userlogin_code = temp_userlogin_code + temp_csc;
			}

			WritePrivateProfileStringW(_T("LoginData"),temp_section,temp_userlogin_code,FilePath);
		}



		for (int i=0; i<BAC_CUSTOMER_UNITS_COUNT; i++)
		{
			CString temp_section,temp_des,temp_csc;
			CString temp_units_code;
			temp_section.Format(_T("UnitData_%d"),i);

			char temp_buffer[400];
			memset(temp_buffer,0,400);
			memcpy(temp_buffer,&m_customer_unit_data.at(i),sizeof(Str_Units_element));


			temp_units_code.Empty();
			for (int j=0; j<sizeof(Str_Units_element); j++)
			{
				temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_buffer + j)));
				temp_csc.MakeUpper();
				temp_units_code = temp_units_code + temp_csc;
			}

			WritePrivateProfileStringW(_T("Cust_Digital_Unit"),temp_section,temp_units_code,FilePath);
		}

		for (int i=0; i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
		{
			CString temp_section,temp_des,temp_csc;
			CString temp_alalog_table_code;
			temp_section.Format(_T("AlalogTable_%d"),i);

			char temp_buffer[400];
			memset(temp_buffer,0,400);
			memcpy(temp_buffer,&m_analog_custmer_range.at(i),sizeof(Str_table_point));


			temp_alalog_table_code.Empty();
			for (int j=0; j<sizeof(Str_table_point); j++)
			{
				temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_buffer + j)));
				temp_csc.MakeUpper();
				temp_alalog_table_code = temp_alalog_table_code + temp_csc;
			}

			WritePrivateProfileStringW(_T("AlalogCusTable"),temp_section,temp_alalog_table_code,FilePath);
		}

#pragma endregion Version_3_Add
		
#pragma region Version_4_Add
		for (int i=0; i<BAC_BASIC_SETTING_COUNT; i++)
		{
			CString temp_section,temp_des,temp_csc;
			CString temp_setting_code;
			temp_section.Format(_T("DeviceSetting_%d"),i);

			char devicesetting_buffer[400];
			memset(devicesetting_buffer,0,400);
			memcpy(devicesetting_buffer,&Device_Basic_Setting.reg.ip_addr[0],sizeof(Str_Setting_Info));


			temp_setting_code.Empty();
			for (int j=0; j<sizeof(Str_Setting_Info); j++)
			{
				temp_csc.Format(_T("%02x"),(unsigned char)(*(devicesetting_buffer + j)));
				temp_csc.MakeUpper();
				temp_setting_code = temp_setting_code + temp_csc;
			}

			WritePrivateProfileStringW(_T("DeviceSetting"),temp_section,temp_setting_code,FilePath);
		}
#pragma endregion Version_4_Add

        for (int i=0; i<BAC_SCHEDULE_COUNT; i++)
        {
            CString temp_input,temp_des,temp_csc;
            temp_input.Format(_T("Weekly_Routines%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).description, (int)strlen((char *)m_Weekly_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();

            WritePrivateProfileStringW(temp_input,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_input,_T("Label"),temp_label,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).value);
            WritePrivateProfileStringW(temp_input,_T("Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_input,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1_value);
            WritePrivateProfileStringW(temp_input,_T("Override_1_Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2_value);
            WritePrivateProfileStringW(temp_input,_T("Override_2_Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).off);
            WritePrivateProfileStringW(temp_input,_T("Off"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).unused);
            WritePrivateProfileStringW(temp_input,_T("Unused"),temp_csc,FilePath);


            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1.number);
            WritePrivateProfileStringW(temp_input,_T("Override_1_Number"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1.panel);
            WritePrivateProfileStringW(temp_input,_T("Override_1_Panel"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_1.point_type);
            WritePrivateProfileStringW(temp_input,_T("Override_1_Point_Type"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2.number);
            WritePrivateProfileStringW(temp_input,_T("Override_2_number"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2.panel);
            WritePrivateProfileStringW(temp_input,_T("Override_2_Panel"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Weekly_data.at(i).override_2.point_type);
            WritePrivateProfileStringW(temp_input,_T("Override_2_Point_Type"),temp_csc,FilePath);

        }

        for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
        {
            CString temp_input,temp_des,temp_csc;
            temp_input.Format(_T("Annual_Routines%d"),i);

            MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).description, (int)strlen((char *)m_Annual_data.at(i).description)+1,
                                 temp_des.GetBuffer(MAX_PATH), MAX_PATH );
            temp_des.ReleaseBuffer();
            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).label, (int)strlen((char *)m_Annual_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();

            WritePrivateProfileStringW(temp_input,_T("Description"),temp_des,FilePath);
            WritePrivateProfileStringW(temp_input,_T("Label"),temp_label,FilePath);

            temp_csc.Format(_T("%u"),(unsigned char)m_Annual_data.at(i).value);
            WritePrivateProfileStringW(temp_input,_T("Value"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Annual_data.at(i).auto_manual);
            WritePrivateProfileStringW(temp_input,_T("Auto_Manual"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_Annual_data.at(i).unused);
            WritePrivateProfileStringW(temp_input,_T("Unused"),temp_csc,FilePath);
        }

        for (int i=0; i<BAC_MONITOR_COUNT; i++)
        {
            CString temp_monitor,temp_des,temp_csc;
            temp_monitor.Format(_T("Monitor%d"),i);

            CString temp_label;
            MultiByteToWideChar( CP_ACP, 0, (char *)m_monitor_data.at(i).label, (int)strlen((char *)m_monitor_data.at(i).label)+1,
                                 temp_label.GetBuffer(MAX_PATH), MAX_PATH );
            temp_label.ReleaseBuffer();
            WritePrivateProfileStringW(temp_monitor,_T("Label"),temp_label,FilePath);
            unsigned char * temp_point = NULL;
            CString temp_inputs;
            for (int j=0; j<MAX_POINTS_IN_MONITOR; j++)
            {
                temp_point = &m_monitor_data.at(i).inputs[j].number;
                for (int k=0; k<(int)sizeof(Point_Net); k++)
                {
                    temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_point + k)));
                    temp_csc.MakeUpper();
                    temp_inputs = temp_inputs + temp_csc;
                }
            }
            WritePrivateProfileStringW(temp_monitor,_T("Inputs"),temp_inputs,FilePath);
            temp_point = NULL;
            CString temp_range;

            for (int x=0; x<MAX_POINTS_IN_MONITOR; x++)
            {
                temp_point = &m_monitor_data.at(i).range[x];
                temp_csc.Format(_T("%02x"),*(temp_point));
                temp_csc.MakeUpper();
                temp_range = temp_range + temp_csc;
            }
            WritePrivateProfileStringW(temp_monitor,_T("Range"),temp_range,FilePath);

            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).second_interval_time);
            WritePrivateProfileStringW(temp_monitor,_T("Second_Interval_Time"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).minute_interval_time);
            WritePrivateProfileStringW(temp_monitor,_T("Minute_Interval_Time"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).hour_interval_time);
            WritePrivateProfileStringW(temp_monitor,_T("Hour_Interval_Time"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).max_time_length);
            WritePrivateProfileStringW(temp_monitor,_T("Max_Time_Length"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).num_inputs);
            WritePrivateProfileStringW(temp_monitor,_T("Num_Inputs"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).an_inputs);
            WritePrivateProfileStringW(temp_monitor,_T("An_Inputs"),temp_csc,FilePath);
            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).next_sample_time);
            WritePrivateProfileStringW(temp_monitor,_T("next_sample_time"),temp_csc,FilePath);


            temp_csc.Format(_T("%u"),(unsigned char)m_monitor_data.at(i).status);
            WritePrivateProfileStringW(temp_monitor,_T("Status"),temp_csc,FilePath);

        }

        for (int i=0; i<BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
        {
            unsigned char * temp_point = NULL;
            temp_point = weeklt_time_schedule[i];
            CString tempsection,temp_code,temp_csc;
            tempsection.Format(_T("WeeklyRoutinesData_%d"),i);
            for (int j=0; j<WEEKLY_SCHEDULE_SIZE; j++)
            {
                temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_point + j)));
                temp_csc.MakeUpper();
                temp_code = temp_code + temp_csc;
            }
            WritePrivateProfileStringW(tempsection,_T("Data"),temp_code,FilePath);
        }

        for (int i=0; i<BAC_HOLIDAY_COUNT; i++)
        {
            unsigned char * temp_point = NULL;
            temp_point = g_DayState[i];
            CString tempsection,temp_code,temp_csc;
            tempsection.Format(_T("AnnualRoutinesData_%d"),i);
            for (int j=0; j<ANNUAL_CODE_SIZE; j++)
            {
                temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_point + j)));
                temp_csc.MakeUpper();
                temp_code = temp_code + temp_csc;
            }
            WritePrivateProfileStringW(tempsection,_T("Data"),temp_code,FilePath);
        }

        for (int i=0; i<BAC_PROGRAMCODE_ITEM_COUNT; i++)
        {
            CString temp_section,temp_des,temp_csc;
            CString temp_code;
            unsigned char * temp_point = NULL;
            temp_point = program_code[i];
            temp_section.Format(_T("Program_Code%d"),i);

            if(program_code_length[i] == 0)
                continue;

            program_code_length[i] = ((program_code_length[i] / 400) + 1) * 400;	//保存至 配置档的 都是400 一块

            temp_csc.Format(_T("%d"),program_code_length[i]);
            WritePrivateProfileStringW(temp_section,_T("Program_Length"),temp_csc,FilePath);

            //将要保存的代码划分为N快存储，每块 100 ，ini文本一个段最多只有保存260;
            //int save_part = program_code_length[i] /100  + 1;


            temp_code.Empty();
            for (int j=0; j<program_code_length[i]; j++)
            {
                temp_csc.Format(_T("%02x"),(unsigned char)(*(temp_point + j)));
                temp_csc.MakeUpper();
                temp_code = temp_code + temp_csc;
            }
            CString part_section;
            part_section.Format(_T("Program_Code_Text"));
            WritePrivateProfileStringW(temp_section,part_section,temp_code,FilePath);

        }





        CFile myfile(FilePath,CFile::modeRead);
        char *pBuf;
        DWORD dwFileLen;
        dwFileLen=myfile.GetLength();
        pBuf= new char[dwFileLen+1];
        pBuf[dwFileLen]=0;
        myfile.Read(pBuf,dwFileLen);     //MFC   CFile 类 很方便
        myfile.Close();
        //MessageBox(pBuf);
        char * temp_buffer = pBuf;
        for (int i=0; i<dwFileLen; i++)
        {
            *temp_buffer = *temp_buffer ^ 1;
            temp_buffer ++;
        }

        //CString new_file;
        //new_file = FilePath.Left(FilePath.GetLength()-3) + _T("prg");
        CStringArray temp_array2;
        SplitCStringA(temp_array2,SaveConfigFilePath,_T("."));
        temp_array_size=0;
        temp_array_size = temp_array2.GetSize();
        right_suffix = temp_array2.GetAt(temp_array_size - 1).GetLength();
        CString temp_FilePath = SaveConfigFilePath.Left( config_file_length -  right_suffix);
        SaveConfigFilePath = temp_FilePath + _T("prg");


        CFileFind tempfind;
        if(tempfind.FindFile(SaveConfigFilePath))
        {
            DeleteFile(SaveConfigFilePath);
        }


        HANDLE hFile;
        hFile=CreateFile(SaveConfigFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        DWORD dWrites;
        WriteFile(hFile,pBuf,dwFileLen,&dWrites,NULL);
        CloseHandle(hFile);
        if(pBuf)
            delete pBuf;
        DeleteFile(FilePath);

    }
}


bool Is_Bacnet_Device(unsigned short n_product_class_id)
{
    if((n_product_class_id == PM_CM5) ||
            (n_product_class_id == PM_MINIPANEL))
    {
        return true;
    }

    return false;
}

bool IP_is_Local(LPCTSTR ip_address)
{

    IP_ADAPTER_INFO pAdapterInfo;
    ULONG len = sizeof(pAdapterInfo);
    if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS)
    {
        return 0;
    }
    long nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);
    DWORD dw_ip = htonl(nLocalIP);

    CString temp_ip;
    temp_ip.Format(_T("%s"),ip_address);
    char tempchar[200];
    memset(tempchar,0,200);
    WideCharToMultiByte(CP_ACP,0,temp_ip.GetBuffer(),-1,tempchar,200,NULL,NULL);

    DWORD m_dwClientIP = inet_addr((char *)tempchar);

    BYTE byIP[4];
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byIP[i] = (dw_ip >> ic*8)&0x000000FF;
    }

    BYTE byISPDeviceIP[4];
    DWORD dwClientIP = m_dwClientIP;
    ZeroMemory(byISPDeviceIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byISPDeviceIP[3-i] = (dwClientIP >> ic*8)&0x000000FF;
    }
    if(memcmp(byIP,byISPDeviceIP,3)==0)
    {
        return true;
    }

    //memcpy_s(byIP,3,byISPDeviceIP,3)
    return false;
}

// 执行程序的路径 // 参数  // 执行环境目录   // 最大等待时间, 超过这个时间强行终止;
DWORD WinExecAndWait( LPCTSTR lpszAppPath,LPCTSTR lpParameters,LPCTSTR lpszDirectory, 	DWORD dwMilliseconds)
{
    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize    = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask    = SEE_MASK_NOCLOSEPROCESS;
    //ShExecInfo.fMask    = SEE_MASK_HMONITOR;
    ShExecInfo.hwnd        = NULL;
    ShExecInfo.lpVerb    = NULL;
    ShExecInfo.lpFile    = lpszAppPath;
    ShExecInfo.lpParameters = lpParameters;
    ShExecInfo.lpDirectory    = lpszDirectory;
    ShExecInfo.nShow    = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    if(dwMilliseconds==0)
    {
        WaitForSingleObject(ShExecInfo.hProcess,INFINITE);  //等待进程退出，才继续运行;
    }
    else if (dwMilliseconds!=0 && WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds) == WAIT_TIMEOUT)  	// 指定时间没结束;
    {
        // 强行杀死进程;
        TRACE(_T("TerminateProcess        %s"),lpszAppPath);
        TerminateProcess(ShExecInfo.hProcess, 0);
        return 0;    //强行终止;
    }

    DWORD dwExitCode;
    BOOL bOK = GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
    ASSERT(bOK);

    return dwExitCode;
}




BOOL KillProcessFromName(CString strProcessName)
{
    //创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);



    //PROCESSENTRY32进程快照的结构体

    PROCESSENTRY32 pe;



    //实例化后使用Process32First获取第一个快照的进程前必做的初始化操作

    pe.dwSize = sizeof(PROCESSENTRY32);





    //下面的IF效果同:

    //if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄

    if(!Process32First(hSnapShot,&pe))

    {

        return FALSE;

    }



    //将字符串转换为小写

    strProcessName.MakeLower();



    //如果句柄有效  则一直获取下一个句柄循环下去

    while (Process32Next(hSnapShot,&pe))

    {



        //pe.szExeFile获取当前进程的可执行文件名称

        CString scTmp = pe.szExeFile;





        //将可执行文件名称所有英文字母修改为小写

        scTmp.MakeLower();



        //比较当前进程的可执行文件名称和传递进来的文件名称是否相同

        //相同的话Compare返回0

        if(!scTmp.Compare(strProcessName))

        {



            //从快照进程中获取该进程的PID(即任务管理器中的PID)

            DWORD dwProcessID = pe.th32ProcessID;

            HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);

            ::TerminateProcess(hProcess,0);

            CloseHandle(hProcess);

            return TRUE;

        }

        scTmp.ReleaseBuffer();

    }

    strProcessName.ReleaseBuffer();

    return FALSE;

}




BOOL DirectoryExist(CString Path)
{
    WIN32_FIND_DATA fd;
    BOOL ret = FALSE;
    HANDLE hFind = FindFirstFile(Path, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        //目录存在
        ret = TRUE;

    }
    FindClose(hFind);
    return ret;
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


BOOL CreateDirectory(CString path)
{
    SECURITY_ATTRIBUTES attrib;
    attrib.bInheritHandle = FALSE;
    attrib.lpSecurityDescriptor = NULL;
    attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

    return CreateDirectory( path, &attrib);
}
#if 1
BOOL DeleteDirectory(CString path)
{

    CFileFind finder;
    CString temppath;
    temppath=path;
    //temppath.Format("%s/*.*",path);
    BOOL ret;
    //temppath.Format(_T("%s*.*",path));
    temppath += "\\*.*";

    BOOL bWorking = finder.FindFile(temppath);
    ret=bWorking;
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
        //ret=bWorking;
        if(finder.IsDirectory() && !finder.IsDots()) //处理文件夹
        {
            DeleteDirectory(finder.GetFilePath()); //递归删除文件夹
            RemoveDirectory(finder.GetFilePath());
        }
        else //处理文件
        {
            DeleteFile(finder.GetFilePath());
        }
    }
    RemoveDirectory(path);
    return ret;
}
#endif




void TraverseFolder( const CString& strDir,std::vector<CString>& vecFile)
{
    WIN32_FIND_DATA FindFileData;
    CString strDirTmp;
    strDirTmp = strDir;
    strDirTmp += "\\*.*";
    HANDLE hFind=::FindFirstFile(strDirTmp,&FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
    {

        return;
    }
    while(TRUE)
    {
        if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FindFileData.cFileName[0]!=L'.')
            {
                strDirTmp=strDir;
                //  strDirTmp+="\\";
                strDirTmp+=FindFileData.cFileName;
                TraverseFolder(strDirTmp,vecFile);
            }
            // 				else
            // 				{
            // 					strDirTmp=strDir;
            // 					strDirTmp+="\\";
            // 					strDirTmp+=FindFileData.cFileName;
            // 					TraverseFolder(strDirTmp,vecFile);
            // 				}
        }
        else
        {
            strDirTmp=strDir;
            strDirTmp+=L"\\";
            strDirTmp+=FindFileData.cFileName;
            //TraverseFolder(strDirTmp,vecFile);
            vecFile.push_back(strDirTmp);
        }
        if (!FindNextFile(hFind,&FindFileData))
        {
            break;
        }
    }
    FindClose(hFind);
}

bool GetFileNameFromPath(CString ncstring,CString &cs_return)
{
    CStringArray ntemparray;
    SplitCStringA(ntemparray,ncstring,_T("\\"));
    int find_results = 0;
    find_results = ntemparray.GetSize();
    if(find_results>=2)
    {
        cs_return = ntemparray.GetAt(find_results - 1);
        return true;
    }
    return false;
}


void DFTrace(LPCTSTR lpCString)
{
    CString nCString;
    nCString = lpCString;
    static int count = 0;
    CTime print_time=CTime::GetCurrentTime();
    CString str=print_time.Format("%H:%M:%S    ");

    PrintText[count].Empty();
    PrintText[count] =str + nCString;
    PostMessage(h_debug_window,WM_ADD_DEBUG_CSTRING,(WPARAM)PrintText[count].GetBuffer(),NULL);
    count = (count ++) % 900;

}

CString GetContentFromURL(CString URL)
{
    CString strHtml;
    CInternetSession sess;
    CHttpFile* pHttpFile = NULL;
    try
    {
        pHttpFile = (CHttpFile*)sess.OpenURL(URL.GetBuffer());
        char sRecived[1024];
        if(pHttpFile)
        {
            while(pHttpFile->ReadString((LPTSTR)sRecived, 1024)) //解决Cstring乱码
            {
                CString temp(sRecived);
                strHtml += temp;
            }
        }

    }

    catch (CException* e)
    {
        strHtml=_T("");
    }



    return strHtml;
}
CString GetProductFirmwareTimeFromTemcoWebsite(CString URL,CString HexOrBinName,CString &FileSize)
{
    CString ftp_T3000Version;
    ftp_T3000Version=GetContentFromURL(URL);
    CStringArray HtmlArray;
    CString VersionString;
    if (ftp_T3000Version.GetLength()<=1)
    {
        return VersionString;
    }
    HtmlArray.RemoveAll();
    SplitCStringA(HtmlArray,ftp_T3000Version,_T("<hr>"));
    CString ImageString;
    if (HtmlArray.GetSize()>1)
    {
        ImageString=HtmlArray[1];
        HtmlArray.RemoveAll();
        SplitCStringA(HtmlArray,ImageString,_T("<img"));
        ImageString.Empty();
        for (int i=0; i<HtmlArray.GetSize(); i++)
        {
            if (HtmlArray[i].Find(HexOrBinName)!=-1)
            {
                ImageString=HtmlArray[i];
                break;
            }
        }

        HtmlArray.RemoveAll();

        if(ImageString.GetLength()>1)
        {
            SplitCStringA(HtmlArray,ImageString,_T("</a>"));
        }
        if(HtmlArray.GetSize()>0)
        {
            ImageString=HtmlArray[1];
        }
        //               20-Nov-2014 00:36  152K
        ImageString.TrimLeft();
        ImageString.TrimRight();
        HtmlArray.RemoveAll();
        if(ImageString.GetLength()>1)
        {
            SplitCStringA(HtmlArray,ImageString,_T("  "));
        }
        if (HtmlArray.GetSize()>0)
        {
            VersionString=HtmlArray[0];
            FileSize=HtmlArray[1];
        }

    }

    return VersionString;
}

BOOL CheckTheSameSubnet(CString strIP ,CString strIP2)
{
    BOOL ret=FALSE;
    USES_CONVERSION;
    LPCSTR szIP = W2A(strIP);
    DWORD dwIP = inet_addr(szIP);
    IN_ADDR ia,sa;
    ia.S_un.S_addr = dwIP;



    szIP=W2A(strIP2);
    dwIP=inet_addr(szIP);
    sa.S_un.S_addr=dwIP;

    if ( ia.S_un.S_un_b.s_b1 == sa.S_un.S_un_b.s_b1 &&
            ia.S_un.S_un_b.s_b2 == sa.S_un.S_un_b.s_b2 &&
            ia.S_un.S_un_b.s_b3 == sa.S_un.S_un_b.s_b3
       )
    {
        ret=TRUE;
    }



    return ret;

}


CString GetExePath(bool bHasSlash)
{
    TCHAR	szBuf[MAX_PATH];
    GetModuleFileName(NULL,szBuf,MAX_PATH);

    CString	strPath(szBuf);
    int idx = strPath.ReverseFind(_T('\\'));
    strPath = strPath.Left(idx);

    if(bHasSlash)  // has '\' at last.
    {
        if(strPath.Right(1)!=_T('\\'))
            strPath+=_T('\\');
        return strPath;
    }
    else   // don't need '\'.
    {
        if(strPath.Right(1)==_T('\\'))
            strPath.TrimRight(_T('\\'));
        return strPath;
    }
}


void Save_Product_Value_Cache(CString &SaveFilePath)
{
    CString FilePath;
    CStringArray temp_array1;
    SplitCStringA(temp_array1,SaveFilePath,_T("."));
    int temp_array_size=0;
    temp_array_size = temp_array1.GetSize();
    if(temp_array1.GetSize()<=1)
    {
        return;
    }
    int right_suffix = temp_array1.GetAt(temp_array_size - 1).GetLength();
    int config_file_length = SaveFilePath.GetLength();
    if(config_file_length <= right_suffix)
    {
        return;
    }

    CFileFind tempfind;
    if(tempfind.FindFile(SaveFilePath))
    {
        DeleteFile(SaveFilePath);
    }

    /*HANDLE hFile;
    hFile=CreateFile(SaveFilePath,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD dWrites;
    WriteFile(hFile,product_register_value,1024,&dWrites,NULL);
    CloseHandle(hFile);  */
    CFile savefile(SaveFilePath,CFile::modeCreate|CFile::modeReadWrite);

    CArchive oar(&savefile,CArchive::store);

    for (int i=0; i<1024; i++)
    {
        oar<<product_register_value[i];
    }
    oar.Close();
    savefile.Close();
//     LoadTstat_InputData();
//     LoadTstat_OutputData();
    
}
int Load_Product_Value_Cache(LPCTSTR tem_read_path)
{
    CString FilePath;
    char *pBuf;
    FilePath.Format(_T("%s"),tem_read_path);
    CFileFind temp_find;
    if(!temp_find.FindFile(FilePath))
        return -1;
#if 1
    CFile iFile(tem_read_path,CFile::modeRead);
    CArchive iar(&iFile,CArchive::load);

    for (int i=0; i<1024; i++)
    {
        iar>>product_register_value[i];
        //iar>>multi_register_value[i];
    }
    iar.Close();
    iFile.Close();
    memcpy_s(multi_register_value,sizeof(multi_register_value),product_register_value,sizeof(product_register_value));
#endif
}
//////////////////////////////////////////////////////////////////////////
//这里改成用Minipanel的界面
//LoadTstat_功能Data的函数都是把数据解析到一个Vector容器里面
//在界面下面只要把这些数据填充到对于的表里面即可
//结构中包含了对应的 寄存器地址，寄存器值，寄存器对应意义的字符串值
// Author:Alex
//这要这里解析的正确，后面界面操作，对应的也是正确的
//只要不需要关心寄存器地址等
//
//
//
//////////////////////////////////////////////////////////////////////////
void LoadTstat_InputData()
{
    m_tstat_input_data.clear();
    int m_cvalue=0;
    int m_crange=0;
    long m_sn=0;
    CString strAuto=_T("Auto");
    CString strman=_T("Manual");
    //m_tstat_input_data.clear();
    Tstat_Input_Struct temp_tstat_input;
    CString strTemp;
    m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
    int	m_nModel=product_register_value[MODBUS_PRODUCT_MODEL];
    int Product_Type=product_register_value[7];
    if((Product_Type!=PM_TSTAT6)&&(Product_Type!=PM_TSTAT5i)&&(Product_Type!=PM_TSTAT7)&&(Product_Type!=PM_TSTAT8)
		&& (Product_Type != PM_TSTAT8_WIFI) && (Product_Type != PM_TSTAT8_OCC) && (Product_Type != PM_TSTAT7_ARM) && (Product_Type != PM_TSTAT8_220V))
    {
        return;
    }
    CString strUnit=GetTempUnit();

#if 1//初始化InputName
    strTemp=GetTextFromReg(MODBUS_AI1_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strInName1.Format(_T("AI1:%s"),strTemp);

    }
    else
    {
        g_strInName1=_T("AI1:Input1");
        strTemp=_T("Input1");
    }

    temp_tstat_input.InputName.regAddress=MODBUS_AI1_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);

    strTemp.Empty();

    strTemp=GetTextFromReg(MODBUS_AI2_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();

    if (!strTemp.IsEmpty())
    {
        g_strInName2.Format(_T("AI2:%s"),strTemp);

    }
    else
    {
        g_strInName2=_T("AI2:Input2");
        strTemp =  _T("Input2");
    }

    temp_tstat_input.InputName.regAddress=MODBUS_AI2_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);
    strTemp.Empty();

    strTemp=GetTextFromReg(MODBUS_AI3_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();

    if (!strTemp.IsEmpty())
    {
        g_strInName3.Format(_T("AI3:%s"),strTemp);

    }
    else
    {
        g_strInName3=_T("AI3:Input3");
        strTemp =_T("Input3");
    }

    temp_tstat_input.InputName.regAddress=MODBUS_AI3_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);
    strTemp.Empty();
    strTemp=GetTextFromReg(MODBUS_AI4_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();

    if (!strTemp.IsEmpty())
    {
        g_strInName4.Format(_T("AI4:%s"),strTemp);

    }
    else
    {
        g_strInName4=_T("AI4:Input4");
        strTemp =_T("Input4");
    }

    temp_tstat_input.InputName.regAddress=MODBUS_AI4_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);
    strTemp.Empty();
    strTemp=GetTextFromReg(MODBUS_AI5_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();


    if (!strTemp.IsEmpty())
    {
        g_strInName5.Format(_T("AI5:%s"),strTemp);

    }
    else
    {
        g_strInName5=_T("AI5:Input5");
        strTemp   =_T("Input5");
    }

    temp_tstat_input.InputName.regAddress=MODBUS_AI5_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);
    strTemp.Empty();
    strTemp=GetTextFromReg(MODBUS_AI6_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();


    if (!strTemp.IsEmpty())
    {
        g_strInName6.Format(_T("AI6:%s"),strTemp);

    }
    else
    {
        g_strInName6=_T("AI6:Input6");
        strTemp = _T("Input6");
    }

    temp_tstat_input.InputName.regAddress=MODBUS_AI6_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);
    strTemp.Empty();
    strTemp=GetTextFromReg(MODBUS_AI7_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();

    if (!strTemp.IsEmpty())
    {
        g_strInName7.Format(_T("AI7:%s"),strTemp);

    }
    else
    {
        g_strInName7=_T("AI7:Input7");
        strTemp =_T("Input7");
    }
    temp_tstat_input.InputName.regAddress=MODBUS_AI7_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);
    strTemp.Empty();
    strTemp=GetTextFromReg(MODBUS_AI8_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();

    if (!strTemp.IsEmpty())
    {
        g_strInName8.Format(_T("AI8:%s"),strTemp);

    }
    else
    {
        g_strInName8=_T("AI8:Input8");
        strTemp = _T("Input8");
    }
    temp_tstat_input.InputName.regAddress=MODBUS_AI8_CHAR1;
    temp_tstat_input.InputName.StrValue=strTemp;
    m_tstat_input_data.push_back(temp_tstat_input);

#if 1//  g_strSensorName
    strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216
    temp_tstat_input.InputName.StrValue=g_strSensorName;
    temp_tstat_input.Value.StrValue=strTemp;
    temp_tstat_input.Value.regAddress=MODBUS_INTERNAL_THERMISTOR;
    temp_tstat_input.Value.RegValue=product_register_value[MODBUS_INTERNAL_THERMISTOR];

    temp_tstat_input.Unit.StrValue=strUnit;
    temp_tstat_input.AM.regAddress=695;
    temp_tstat_input.AM.RegValue=product_register_value[695];
    if (product_register_value[695]!=0)
    {
        temp_tstat_input.AM.StrValue=strman;
        //m_FlexGrid.put_TextMatrix(1,AM_FIELD,_T("Manual"));
    }
    else
    {
        temp_tstat_input.AM.StrValue=strAuto;
        //m_FlexGrid.put_TextMatrix(1,AM_FIELD,_T("Auto"));
    }
    strUnit=GetTempUnit();
    temp_tstat_input.Range.StrValue=strUnit;
    temp_tstat_input.Range.regAddress=MODBUS_DEGC_OR_F;
    temp_tstat_input.Range.RegValue=product_register_value[MODBUS_DEGC_OR_F];
    temp_tstat_input.Function.StrValue=NO_APPLICATION;
    //temp_tstat_input.Filter.StrValue=NO_APPLICATION;
    temp_tstat_input.Filter.regAddress=142;
    temp_tstat_input.Filter.RegValue=product_register_value[142];
    temp_tstat_input.Filter.StrValue.Format(_T("%d"),product_register_value[142]);

    temp_tstat_input.CustomTable.StrValue=NO_APPLICATION;
    m_tstat_input_data.push_back(temp_tstat_input);

#endif
    strTemp.Empty();
    temp_tstat_input.InputName.StrValue=g_strInHumName;
    m_tstat_input_data.push_back(temp_tstat_input);

    temp_tstat_input.InputName.StrValue=g_strInCO2;
    m_tstat_input_data.push_back(temp_tstat_input);

    temp_tstat_input.InputName.StrValue=g_strLightingSensor;
    m_tstat_input_data.push_back(temp_tstat_input);


#endif

#if 1   //初始化其他的input的其他值
    int nValue=0;

    float fValue=0;
    for (int i=1; i<=8; i++)
    {

        // m_tstat_input_data.at(i-1).CustomTable.StrValue=NO_APPLICATION;

        //Auto Manual
        nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
        BYTE bFilter=0x01;
        bFilter = bFilter<< (i-1);
        m_tstat_input_data.at(i-1).AM.regAddress=MODBUS_INPUT_MANU_ENABLE;
        m_tstat_input_data.at(i-1).AM.RegValue=nValue;
        if((nValue & bFilter))
        {
            m_tstat_input_data.at(i-1).AM.StrValue=strman;
        }
        else
        {
            m_tstat_input_data.at(i-1).AM.StrValue=strAuto;
        }
        //Filter
        strTemp.Format(_T("%d"),product_register_value[142+i]);
        m_tstat_input_data.at(i-1).Filter.regAddress=142+i;
        m_tstat_input_data.at(i-1).Filter.StrValue=strTemp;

        //Range
        m_crange=0;
		strTemp = L"0";
        nValue=product_register_value[MODBUS_ANALOG1_RANGE+i-1];	//189
        nValue &= 0x7F;//去掉最高位
        
        strTemp=analog_range_TSTAT6[nValue];
        
        m_crange=nValue;

        m_tstat_input_data.at(i-1).Range.StrValue=strTemp;
        m_tstat_input_data.at(i-1).Range.RegValue=product_register_value[MODBUS_ANALOG1_RANGE+i-1];
        m_tstat_input_data.at(i-1).Range.regAddress=MODBUS_ANALOG1_RANGE+i-1;
        // nValue=product_register_value[MODBUS_ANALOG1_RANGE+i-2];
        //Custom
        if(m_crange==4)
        {
            m_tstat_input_data.at(i-1).CustomTable.StrValue=_T("Custom1...");
        }
        else if (m_crange==6)
        {
            m_tstat_input_data.at(i-1).CustomTable.StrValue=_T("Custom2...");
        }
        else
        {
            m_tstat_input_data.at(i-1).CustomTable.StrValue=NO_APPLICATION;
        }
        //Function
        nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-1];		//298   167
        m_tstat_input_data.at(i-1).Function.regAddress=MODBUS_ANALOG1_FUNCTION+i-1;
        m_tstat_input_data.at(i-1).Function.RegValue=nValue;
        strTemp=INPUT_FUNS[0];
        m_tstat_input_data.at(i-1).Function.StrValue=strTemp;
        if (nValue>=0&&nValue<8)//tstat6
        {
            strTemp=INPUT_FUNS[nValue];
            m_tstat_input_data.at(i-1).Function.StrValue=strTemp;
        }

        if(m_crange==1||m_crange == 11)	//359  122
        {
            fValue=float((short)product_register_value[MODBUS_ANALOG_INPUT1+i-1])/10.0;	//367   131
            strTemp.Format(_T("%.1f"),fValue);
        }
        else if (m_crange==3||m_crange==5||m_crange==7||m_crange==8||m_crange==9||m_crange==10)
        {
            if (m_crange==9||m_crange==10)
            {
                int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-1]); //367  131
                if (nValue == 0)
                {
                    strTemp = _T("Closed");
                }
                else
                {
                    strTemp = _T("Open");
                }
            }
            else if (m_crange==7||m_crange==8)
            {

                int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-1]); //367  131
                if (nValue == 0)
                {
                    strTemp = _T("Unoccupied");
                }
                else
                {
                    strTemp = _T("Occupied");
                }

            }
            else
            {

                int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-1]); //367  131
                if (nValue == 0)
                {
                    strTemp = _T("Off");
                }
                else
                {
                    strTemp = _T("On");
                }

            }
        }
        else if (m_crange==4||m_crange==6)  // custom sensor	359 122
        {
            fValue=float((short)product_register_value[MODBUS_ANALOG_INPUT1+i-1])/10.0;	//367  131
            strTemp.Format(_T("%.1f"), (float)fValue);

        }
        else if(m_crange==2)	//359 122
        {
            nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];		//367  131
            strTemp.Format(_T("%0.1f"),  (float)nValue);
        }
        else
        {
            strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+i-1]);
        }


        //Unit
        CString strValueUnit=GetTempUnit(m_crange, 1);
        m_tstat_input_data.at(i-1).Unit.StrValue=strValueUnit;


        m_tstat_input_data.at(i-1).Value.regAddress=MODBUS_ANALOG_INPUT1+i-1;
        m_tstat_input_data.at(i-1).Value.RegValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];
        m_tstat_input_data.at(i-1).Value.StrValue=strTemp;
    }

#endif
    bool m_disable_hum,m_disable_CO2;
    if((product_register_value[20]&2)==2)
    {
        m_disable_hum=TRUE;
    }
    else
    {
        m_disable_hum=FALSE;
    }
    if((product_register_value[MODBUS_TSTAT6_CO2_AVALUE]>=0)&&(product_register_value[MODBUS_TSTAT6_CO2_AVALUE]<=3000))
    {
        m_disable_CO2=TRUE;
    }
    else
    {
        m_disable_CO2=FALSE;
    }
    if (!m_disable_hum)
    {
        m_tstat_input_data.at(9).AM.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(9).CustomTable.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(9).Filter.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(9).Function.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(9).Range.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(9).Unit.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(9).Value.StrValue=NO_APPLICATION;

    }
    else
    {

        CString temp;
        m_tstat_input_data.at(9).AM.regAddress=MODBUS_TSTAT6_HUM_AM;
        m_tstat_input_data.at(9).AM.RegValue=product_register_value[MODBUS_TSTAT6_HUM_AM];

        if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
        {

            temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10.0);
            //m_FlexGrid.put_TextMatrix(10,AM_FIELD,strAuto);
            m_tstat_input_data.at(9).AM.StrValue=strAuto;

            m_tstat_input_data.at(9).Value.regAddress=MODBUS_TSTAT6_HUM_AVALUE;
            m_tstat_input_data.at(9).Value.RegValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE];
            m_tstat_input_data.at(9).Value.StrValue=temp;

        }
        else
        {
            // m_FlexGrid.put_TextMatrix(10,AM_FIELD,strman);
            m_tstat_input_data.at(9).AM.StrValue=strman;
            temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10);

            m_tstat_input_data.at(9).Value.regAddress=MODBUS_TSTAT6_HUM_MVALUE;
            m_tstat_input_data.at(9).Value.RegValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE];
            m_tstat_input_data.at(9).Value.StrValue=temp;


        }
        m_tstat_input_data.at(9).Unit.StrValue=_T("%");

        temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_HUM_FILTER]);
        m_tstat_input_data.at(9).Filter.regAddress=MODBUS_TSTAT6_HUM_FILTER;
        m_tstat_input_data.at(9).Filter.RegValue=product_register_value[MODBUS_TSTAT6_HUM_FILTER];
        m_tstat_input_data.at(9).Filter.StrValue=temp;
        m_tstat_input_data.at(9).Range.StrValue=NO_APPLICATION;
    }


    if (!m_disable_CO2)
    {
        m_tstat_input_data.at(10).AM.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(10).CustomTable.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(10).Filter.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(10).Function.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(10).Range.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(10).Unit.StrValue=NO_APPLICATION;
        m_tstat_input_data.at(10).Value.StrValue=NO_APPLICATION;
    }
    else
    {
        CString temp;
        m_tstat_input_data.at(10).AM.regAddress=MODBUS_TSTAT6_CO2_AM;
        m_tstat_input_data.at(10).AM.RegValue=product_register_value[MODBUS_TSTAT6_CO2_AM];
        strUnit=_T("ppm");
        m_tstat_input_data.at(10).Unit.StrValue=strUnit;
        if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
        {


            m_tstat_input_data.at(10).AM.StrValue=strAuto;
            temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);

            m_tstat_input_data.at(10).Value.regAddress=MODBUS_TSTAT6_CO2_AVALUE;
            m_tstat_input_data.at(10).Value.RegValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE];
            m_tstat_input_data.at(10).Value.StrValue=temp;

        }
        else
        {
            m_tstat_input_data.at(10).AM.StrValue=strman;
            temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
            m_tstat_input_data.at(10).Value.regAddress=MODBUS_TSTAT6_CO2_MVALUE;
            m_tstat_input_data.at(10).Value.RegValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE];
            m_tstat_input_data.at(10).Value.StrValue=temp;

        }

        temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_FILTER]);
        m_tstat_input_data.at(10).Filter.regAddress=MODBUS_TSTAT6_CO2_FILTER;
        m_tstat_input_data.at(10).Filter.RegValue=product_register_value[MODBUS_TSTAT6_CO2_FILTER];
        m_tstat_input_data.at(10).Filter.StrValue=temp;
        m_tstat_input_data.at(10).Range.StrValue=NO_APPLICATION;
    }

    strTemp.Format(_T("%d"),product_register_value[MODBUS_VALUE_SENSOR]);
    // LUX
    m_tstat_input_data.at(11).AM.StrValue=NO_APPLICATION;
    m_tstat_input_data.at(11).CustomTable.StrValue=NO_APPLICATION;
    m_tstat_input_data.at(11).Filter.StrValue=NO_APPLICATION;
    m_tstat_input_data.at(11).Function.StrValue=NO_APPLICATION;
    m_tstat_input_data.at(11).Range.StrValue=L"LUX";
    m_tstat_input_data.at(11).Unit.StrValue=L"LUX";
    m_tstat_input_data.at(11).Value.regAddress=MODBUS_VALUE_SENSOR;
    m_tstat_input_data.at(11).Value.RegValue=product_register_value[MODBUS_VALUE_SENSOR];
    m_tstat_input_data.at(11).Value.StrValue=strTemp;

}

void LoadInputData_CS3000()
{
    m_tstat_input_data.clear();
    Tstat_Input_Struct temp_tstat_input;

    temp_tstat_input.InputName.StrValue=L"Current";
    m_tstat_input_data.push_back(temp_tstat_input);
    temp_tstat_input.InputName.StrValue=L"Voltage";
    m_tstat_input_data.push_back(temp_tstat_input);

    m_tstat_input_data.at(0).Value.Reg_Property=REG_READ_WRITE;
    m_tstat_input_data.at(0).Value.regAddress=100;
    m_tstat_input_data.at(0).Value.RegValue=(short)product_register_value[100];


    m_tstat_input_data.at(0).Range.regAddress = 104;
    m_tstat_input_data.at(0).Range.RegValue = (short)product_register_value[104];

    if (product_register_value[7]==PM_CS_RSM_AC||product_register_value[7]==PM_CS_SM_AC)
    {

        m_tstat_input_data.at(0).Value.StrValue.Format(_T("%0.2f"),(float)(m_tstat_input_data.at(0).Value.RegValue)/100);
        if (m_tstat_input_data.at(0).Range.RegValue>=0&&m_tstat_input_data.at(0).Range.RegValue<=2)
        {
            m_tstat_input_data.at(0).Range.StrValue=CS3000_INPUT_RANGE[m_tstat_input_data.at(0).Range.RegValue];
            m_tstat_input_data.at(0).Range.Reg_Property=REG_READ_WRITE;
        }
    }
    else
    {
        m_tstat_input_data.at(0).Value.StrValue.Format(_T("%0.1f"),(float)(m_tstat_input_data.at(0).Value.RegValue)/10);
        if(m_tstat_input_data.at(0).Range.RegValue==10)
        {
            m_tstat_input_data.at(0).Range.StrValue=CS3000_INPUT_RANGE[3];
        }
    }
    m_tstat_input_data.at(1).Value.Reg_Property=REG_READ_WRITE;
    m_tstat_input_data.at(1).Value.regAddress=101;
    m_tstat_input_data.at(1).Value.RegValue=(short)product_register_value[101];
    m_tstat_input_data.at(1).Value.StrValue.Format(_T("%0.1f"),(float)(m_tstat_input_data.at(1).Value.RegValue)/10);

    m_tstat_input_data.at(0).Unit.StrValue=_T("A");
    m_tstat_input_data.at(1).Unit.StrValue=_T("V");

    m_tstat_input_data.at(0).Filter.Reg_Property=REG_READ_WRITE;
    m_tstat_input_data.at(0).Filter.regAddress=136;
    m_tstat_input_data.at(0).Filter.RegValue=product_register_value[136];
    m_tstat_input_data.at(0).Filter.StrValue.Format(_T("%d"),product_register_value[136]);

    m_tstat_input_data.at(1).Filter.Reg_Property=REG_READ_WRITE;
    m_tstat_input_data.at(1).Filter.regAddress=136;
    m_tstat_input_data.at(1).Filter.RegValue=product_register_value[136];
    m_tstat_input_data.at(1).Filter.StrValue.Format(_T("%d"),product_register_value[136]);

}
#define  THE_CHAR_LENGTH 8
CString GetTextFromReg(unsigned short reg)
{
    CString str_temp=_T("");
    unsigned short temp_buffer[4];
    unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
    unsigned char p[THE_CHAR_LENGTH+1]= {'\0'};

    temp_buffer[0]=product_register_value[reg];
    temp_buffer[1]=product_register_value[reg+1];
    temp_buffer[2]=product_register_value[reg+2];
    temp_buffer[3]=product_register_value[reg+3];


    if (temp_buffer[0]==0||temp_buffer[0]==65535)
    {
        return str_temp;
    }
    unsigned short Hi_Char,Low_Char;

    for (int i=0; i<4; i++)
    {
        Hi_Char=temp_buffer[i];
        Hi_Char=Hi_Char&0xff00;
        Hi_Char=Hi_Char>>8;
        Low_Char=temp_buffer[i];
        Low_Char=Low_Char&0x00ff;
        temp_buffer_Char[2*i]=Hi_Char;
        temp_buffer_Char[2*i+1]=Low_Char;
    }

    for (int i=0; i<THE_CHAR_LENGTH; i++)
    {
        p[i] =(unsigned char)temp_buffer_Char[i];
    }
    str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);


    return str_temp;
}
CString GetTextFromReg_Buffer(unsigned short reg,unsigned short *Buffer)
{
    CString str_temp=_T("");
    unsigned short temp_buffer[4];
    unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
    unsigned char p[THE_CHAR_LENGTH+1]= {'\0'};

    temp_buffer[0]=Buffer[reg];
    temp_buffer[1]=Buffer[reg+1];
    temp_buffer[2]=Buffer[reg+2];
    temp_buffer[3]=Buffer[reg+3];


    if (temp_buffer[0]==0||temp_buffer[0]==65535)
    {
        return str_temp;
    }
    unsigned short Hi_Char,Low_Char;

    for (int i=0; i<4; i++)
    {
        Hi_Char=temp_buffer[i];
        Hi_Char=Hi_Char&0xff00;
        Hi_Char=Hi_Char>>8;
        Low_Char=temp_buffer[i];
        Low_Char=Low_Char&0x00ff;
        temp_buffer_Char[2*i]=Hi_Char;
        temp_buffer_Char[2*i+1]=Low_Char;
    }

    for (int i=0; i<THE_CHAR_LENGTH; i++)
    {
        p[i] =(unsigned char)temp_buffer_Char[i];
    }
    str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);


    return str_temp;
}
#define  OUTPUT_NUMBER 7
void LoadTstat_OutputData()
{

    m_tstat_output_data.clear();
    int nVAlue;
    int nValue=0;
    int m_crange=0;
    int m_nModeType = product_register_value[7];
    if((m_nModeType!=PM_TSTAT6)&&(m_nModeType!=PM_TSTAT5i)&&(m_nModeType!=PM_TSTAT7)&&(m_nModeType!=PM_TSTAT8)
		&& (m_nModeType != PM_TSTAT8_WIFI) && (m_nModeType != PM_TSTAT8_OCC) && (m_nModeType != PM_TSTAT7_ARM) && (m_nModeType != PM_TSTAT8_220V))
    {
        return;
    }
    int m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;

    float m_version = get_curtstat_version();
    CString strTemp;
    //initial output1-output7
    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT1_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName1=strTemp;

    }
    else
    {
        g_strOutName1=_T("Output1");
    }
    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT2_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName2=strTemp;

    }
    else
    {
        g_strOutName2=_T("Output2");
    }
    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT3_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName3=strTemp;
    }
    else
    {
        g_strOutName3=_T("Output3");
    }

    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT4_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName4=strTemp;

    }
    else
    {
        g_strOutName4=_T("Output4");
    }

    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT5_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName5=strTemp;

    }
    else
    {
        g_strOutName5=_T("Output5");
    }

    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT6_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName6=strTemp;

    }
    else
    {
        g_strOutName6=_T("Output6");
    }

    strTemp=_T("");
    strTemp=GetTextFromReg(MODBUS_OUTPUT7_CHAR1);
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (!strTemp.IsEmpty())
    {
        g_strOutName7=strTemp;

    }
    else
    {
        g_strOutName7=_T("Output7");
    }

    Tstat_Output_Struct out_struct_temp;
    out_struct_temp.OutputName.StrValue=g_strOutName1;
    out_struct_temp.OutputName.regAddress=MODBUS_OUTPUT1_CHAR1;
    m_tstat_output_data.push_back(out_struct_temp);
    out_struct_temp.OutputName.StrValue=g_strOutName2;
    out_struct_temp.OutputName.regAddress= MODBUS_OUTPUT2_CHAR1;
    m_tstat_output_data.push_back(out_struct_temp);
    out_struct_temp.OutputName.StrValue=g_strOutName3;
    out_struct_temp.OutputName.regAddress= MODBUS_OUTPUT3_CHAR1 ;
    m_tstat_output_data.push_back(out_struct_temp);
    out_struct_temp.OutputName.StrValue=g_strOutName4;
    out_struct_temp.OutputName.regAddress= MODBUS_OUTPUT4_CHAR1;
    m_tstat_output_data.push_back(out_struct_temp);
    out_struct_temp.OutputName.StrValue=g_strOutName5;
    out_struct_temp.OutputName.regAddress=MODBUS_OUTPUT5_CHAR1;
    m_tstat_output_data.push_back(out_struct_temp);
    out_struct_temp.OutputName.StrValue=g_strOutName6;
    out_struct_temp.OutputName.regAddress= MODBUS_OUTPUT6_CHAR1  ;
    m_tstat_output_data.push_back(out_struct_temp);
    out_struct_temp.OutputName.StrValue=g_strOutName7;
    out_struct_temp.OutputName.regAddress=MODBUS_OUTPUT7_CHAR1  ;
    m_tstat_output_data.push_back(out_struct_temp);
	int nFlag = product_register_value[7];
    if ((product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT8)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
    {
    }
    else
    {
        for (int i=0; i<OUTPUT_NUMBER; i++)
        {
            m_tstat_output_data.at(i).Signal_Type.StrValue=NO_APPLICATION;
            m_tstat_output_data.at(i).Signal_Type.regAddress=NO_ADDRESS;
            m_tstat_output_data.at(i).Signal_Type.RegValue = NO_REGISTER_VALUE;
        }
    }

    //108  209 Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.  Fan.
    nVAlue = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]; //t5=108   t6=209;
    int nRange=0;
    //310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to
    //	output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."
    int nAMVAlue=0;//=product_register_value[310];
    nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
    //Row 1-3 初始化 Value，A/M，Range
    for(int i=1; i<=3; i++)
    {
        //int temp=1<<(i-1);
        //int a = nAMVAlue;
        //int b = nAMVAlue & (1<<(i-1));
        if((int)(nAMVAlue & (1<<(i-1))) == (1<<(i-1)))
        {
            strTemp=_T("Manual");
        }
        else
        {
            strTemp=_T("Auto");
        }
        m_tstat_output_data.at(i-1).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
        m_tstat_output_data.at(i-1).AM.RegValue=nAMVAlue;
        m_tstat_output_data.at(i-1).AM.StrValue=strTemp;
        //strTemp=_T("On/Off");
        nRange=product_register_value[MODBUS_MODE_OUTPUT1+i-1];
		int nFlag = product_register_value[7];
        if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8)
			|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
        {
			CppSQLite3DB SqliteDBT3000;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
            
            if (SqliteDBT3000.tableExists("Value_Range"))//有Version表
            {
                CString sql;
                sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),i,i,m_sn);
                q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
                if (!q.eof())//有表但是没有对应序列号的值
                {
                   
                    while (!q.eof())
                    {
                         
						m_crange = q.getIntField("CRange");
                       q.nextRow();
                    }
                    nRange=m_crange;
                    if(nRange>=0)
                    {
                        strTemp=OUTPUT_RANGE45[nRange];
                    }
                }
                else
                {

                    if(nRange>=0)
                    {
                        strTemp=OUTPUT_RANGE45[nRange];
                    }
                }
                
            }
            else
            {

                if(nRange>=0)
                {
                    strTemp=OUTPUT_RANGE45[nRange];
                }
            }
            SqliteDBT3000.closedb();

            if(nRange>=0&&nRange<7)
            {
                strTemp=OUTPUT_RANGE45[nRange];
            }
        }
        else
        {
            if(nRange>=0&&nRange<3)
            {
                strTemp=OUTPUT_RANGE5[nRange];
            }
        }

        m_tstat_output_data.at(i-1).Range.regAddress=MODBUS_MODE_OUTPUT1+i-1;
        m_tstat_output_data.at(i-1).Range.RegValue=product_register_value[MODBUS_MODE_OUTPUT1+i-1];
        m_tstat_output_data.at(i-1).Range.StrValue=strTemp;

        if (nRange==0||nRange==4||nRange==5||nRange==6)
        {
            m_tstat_output_data.at(i-1).Value.regAddress=MODBUS_DIGITAL_OUTPUT_STATUS;
            m_tstat_output_data.at(i-1).Value.RegValue=nVAlue;

            if(nVAlue&(1<<(i-1)))
            {
                if (nRange==0||nRange==4)
                {
                    strTemp=_T("On");
                }
                else if (nRange==5||nRange==6)
                {
                    strTemp=_T("Open");
                }

            }
            else
            {
                if (nRange==0||nRange==4)
                {
                    strTemp=_T("Off");
                }
                else if (nRange==5||nRange==6)
                {
                    strTemp=_T("Closed");
                }
            }
            m_tstat_output_data.at(i-1).Value.StrValue=strTemp;
        }
        else
        {
            //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
            int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
            strTemp.Format(_T("%d%%"), nValueTemp);
            m_tstat_output_data.at(i-1).Value.regAddress=MODBUS_PWM_OUT4;
            m_tstat_output_data.at(i-1).Value.RegValue=nValueTemp;
            m_tstat_output_data.at(i-1).Value.StrValue=strTemp;

            m_tstat_output_data.at(i-1).Unit.StrValue = _T("%");
        }

    }
    //Function
#if 1
    for(int i=0; i<OUTPUT_NUMBER; i++)
    {
        int nFun=0;//=product_register_value[328];//tstat6找不到对应的
        nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION+i]; //328   266
        strTemp=ONTPUT_FUNS[0];
        if(nFun>=0&&nFun<5)
        {
            strTemp=ONTPUT_FUNS[nFun];
        }
        m_tstat_output_data.at(i).Function.regAddress=MODBUS_OUTPUT1_FUNCTION+i;
        m_tstat_output_data.at(i).Function.RegValue=nFun;
        m_tstat_output_data.at(i).Function.StrValue=strTemp;
    }
#endif

    if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16
            ||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V))//||m_nModeType==17||m_nModeType==18)
    {
        // just for row4 ///////////////////////////////////////////////////////////////
        if((int)(nAMVAlue & 8))
        {
            strTemp=_T("Manual");
        }
        else
        {
            strTemp=_T("Auto");
        }
        m_tstat_output_data.at(3).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
        m_tstat_output_data.at(3).AM.RegValue=nAMVAlue;
        m_tstat_output_data.at(3).AM.StrValue=strTemp;
        strTemp.Empty();
    }

    nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205
    if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V))
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
        if (SqliteDBT3000.tableExists("Value_Range"))//有Version表
        {
            CString sql;
            sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),4,4,m_sn);
             q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
            if (!q.eof())//有表但是没有对应序列号的值
            {
                 
                while (!q.eof())
                {
                    m_crange= q.getIntField("CRange");
                    q.nextRow();
                }
                nRange=m_crange;
                if(nRange>=0)
                {
                    strTemp=OUTPUT_RANGE45[nRange];
                }
            }
            else
            {

                if(nRange>=0)
                {
                    strTemp=OUTPUT_RANGE45[nRange];
                }
            }
             
        }
        else
        {

            if(nRange>=0)
            {
                strTemp=OUTPUT_RANGE45[nRange];
            }
        }
        SqliteDBT3000.closedb();

        if(nRange>=0&&nRange<7)
        {
            strTemp=OUTPUT_RANGE45[nRange];
        }
    }
    else
    {
        if(nRange>=0&&nRange<=2)		//Modify by Fance_0412
        {
            strTemp=OUTPUT_RANGE5[nRange];
        }
    }

    m_tstat_output_data.at(3).Range.regAddress=MODBUS_MODE_OUTPUT4;
    m_tstat_output_data.at(3).Range.RegValue=product_register_value[MODBUS_MODE_OUTPUT4];
    m_tstat_output_data.at(3).Range.StrValue=strTemp;

    if(nRange == 0||nRange==4||nRange==5||nRange==6) // || !(nAMVAlue & 8)AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
    {
        // output is on/off

        m_tstat_output_data.at(3).Value.regAddress=MODBUS_DIGITAL_OUTPUT_STATUS;
        m_tstat_output_data.at(3).Value.RegValue=nVAlue;
        if(nVAlue&8)
        {
            if (nRange==0||nRange==4)
            {
                strTemp=_T("On");
            }
            else if (nRange==5||nRange==6)
            {
                strTemp=_T("Open");
            }
        }
        else
        {
            if (nRange==0||nRange==4)
            {
                strTemp=_T("Off");
            }
            else if (nRange==5||nRange==6)
            {
                strTemp=_T("Closed");
            }
        }
        m_tstat_output_data.at(3).Value.StrValue=strTemp;
    }
    else // output is value
    {
        //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
        int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
        strTemp.Format(_T("%d%%"), nValueTemp);
        m_tstat_output_data.at(3).Value.regAddress=MODBUS_PWM_OUT4;
        m_tstat_output_data.at(3).Value.RegValue=nValueTemp;
        m_tstat_output_data.at(3).Value.StrValue=strTemp;
        m_tstat_output_data.at(3).Unit.StrValue = _T("%");
    }

    //Row=4
    if((int)(nAMVAlue & 16))
    {
        strTemp=_T("Manual");

    }
    else
    {
        strTemp=_T("Auto");

    }

    m_tstat_output_data.at(4).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
    m_tstat_output_data.at(4).AM.RegValue=nAMVAlue;
    m_tstat_output_data.at(4).AM.StrValue=strTemp;

    strTemp.Empty();
    //nRange=product_register_value[284];
    //284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
    nRange = product_register_value[MODBUS_MODE_OUTPUT5];
    if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V))
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
        if (SqliteDBT3000.tableExists("Value_Range"))//有Version表
        {
            CString sql;
            sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),5,5,m_sn);
            q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
            if (!q.eof())//有表但是没有对应序列号的值
            {
				 
				while (!q.eof())
                {
                    m_crange=q.getIntField("CRange");
                    q.nextRow();
                }
                nRange=m_crange;
                if(nRange>=0)
                {
                    strTemp=OUTPUT_RANGE45[nRange];
                }
            }
            else
            {

                if(nRange>=0)
                {
                    strTemp=OUTPUT_RANGE45[nRange];
                }
            }
            
        }
        else
        {

            if(nRange>=0)
            {
                strTemp=OUTPUT_RANGE45[nRange];
            }
        }
        SqliteDBT3000.closedb();

        if(nRange>=0&&nRange<7)
        {
            strTemp=OUTPUT_RANGE45[nRange];
        }
    }
    else
    {
        if(nRange>=0&&nRange<=2)
        {
            strTemp=OUTPUT_RANGE5[nRange];
        }
    }

    m_tstat_output_data.at(4).Range.regAddress=MODBUS_MODE_OUTPUT5;
    m_tstat_output_data.at(4).Range.RegValue=product_register_value[MODBUS_MODE_OUTPUT5];
    m_tstat_output_data.at(4).Range.StrValue=strTemp;
    strTemp.Empty();


    if(nRange == 0||nRange==4||nRange==5||nRange==6)//|| !(nAMVAlue & 16)
    {
        m_tstat_output_data.at(4).Value.regAddress=MODBUS_DIGITAL_OUTPUT_STATUS;
        m_tstat_output_data.at(4).Value.RegValue=nVAlue;

        if(nVAlue&16)
        {
            if (nRange==0||nRange==4)
            {
                strTemp=_T("On");
            }
            else if (nRange==5||nRange==6)
            {
                strTemp=_T("Open");
            }


        }
        else
        {
            if (nRange==0||nRange==4)
            {
                strTemp=_T("Off");
            }
            else if (nRange==5||nRange==6)
            {
                strTemp=_T("Closed");
            }
        }

        m_tstat_output_data.at(4).Value.StrValue=strTemp;
    }
    else
    {
        int nValueTemp=product_register_value[MODBUS_PWM_OUT5];	//tstat6没有找到	349 ,599
        strTemp.Format(_T("%d%%"), nValueTemp);

        m_tstat_output_data.at(4).Value.regAddress=MODBUS_PWM_OUT5;
        m_tstat_output_data.at(4).Value.RegValue=nValueTemp;
        m_tstat_output_data.at(4).Value.StrValue=strTemp;
        m_tstat_output_data.at(4).Unit.StrValue = _T("%");

    }


    if ((m_nModeType==1||m_nModeType==3||m_nModeType==2)||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
            ||m_nModeType==18||m_nModeType==6||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8||m_nModeType==7
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V))//5ADEG
    {
        //186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V
        //102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
        if(m_nModeType==1||m_nModeType==3||m_nModeType==2)
        {
            //186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V
            //102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.

            short nRange;
            //=product_register_value[186];
            int nValue;
            //e=product_register_value[102];
            //if (product_register_value[7] == 6)
            nRange=product_register_value[MODBUS_OUTPUT1_SCALE];
            m_tstat_output_data.at(3).Range.regAddress=MODBUS_OUTPUT1_SCALE;
            m_tstat_output_data.at(3).Range.RegValue=nRange;

            nValue=product_register_value[MODBUS_COOLING_VALVE];
            m_tstat_output_data.at(3).Value.regAddress=MODBUS_COOLING_VALVE;
            m_tstat_output_data.at(3).Value.RegValue=nValue;

            if(nRange==0)
            {
                if(nValue==0)
                    strTemp=_T("Off");
                else
                    strTemp=_T("On");
            }
            else
            {
                //strTemp.Format(_T("%.1f"),nValue/100.0);
                //strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
                float fvalue=0.0;
                //if(nRange==1)//0-10v
                //{
                //nvalue=product_register_value[102]/100 /10.0 * 100%;
                //	nvalue=product_register_value[102]/10.0f;
                fvalue=(float)(nValue)/10.0f;

                strTemp.Format(_T("%.1f%%"),fvalue);
                m_tstat_output_data.at(3).Unit.StrValue = _T("%");
            }
            m_tstat_output_data.at(3).Value.StrValue=strTemp;


            //m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
            if((int)(nAMVAlue & 8))
            {
                strTemp=_T("Manual");
                // 				m_FlexGrid.put_Col(VALUE_OUTFIELD);
                // 				m_FlexGrid.put_Row(4);
                // 				m_FlexGrid.put_CellBackColor(COLOR_CELL);
            }
            else
            {
                strTemp=_T("Auto");
                // 				m_FlexGrid.put_Col(VALUE_OUTFIELD);
                // 				m_FlexGrid.put_Row(4);
                // 				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
            }
            //m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
            m_tstat_output_data.at(3).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
            m_tstat_output_data.at(3).AM.RegValue=nAMVAlue;
            m_tstat_output_data.at(3).AM.StrValue=strTemp;
            if(nRange>=0&&nRange<17)
            {
                strTemp=OUTPUT_ANRANGE[nRange];
            }
            //m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
            m_tstat_output_data.at(3).Range.StrValue=strTemp;


            nRange=product_register_value[MODBUS_OUTPUT2_SCALE];
            m_tstat_output_data.at(4).Range.regAddress=MODBUS_OUTPUT1_SCALE;
            m_tstat_output_data.at(4).Range.RegValue=nRange;
            nValue=product_register_value[MODBUS_HEATING_VALVE];
            m_tstat_output_data.at(4).Value.regAddress=MODBUS_COOLING_VALVE;
            m_tstat_output_data.at(4).Value.RegValue=nValue;

            if(nRange==0)
            {

                if(nValue==0)
                    strTemp=_T("Off");
                if(nValue==1)
                    strTemp=_T("On");
            }
            else
            {
                //strTemp.Format(_T("%.1f"),nValue/100.0);

                float nvalue=0.0;

                nvalue=(float)nValue/10.0f;

                strTemp.Format(_T("%.1f%%"),nvalue);
                m_tstat_output_data.at(4).Unit.StrValue = _T("%");
            }

            m_tstat_output_data.at(4).Value.StrValue=strTemp;



            if((int)(nAMVAlue & 16))
            {
                strTemp=_T("Manual");

            }
            else
            {
                strTemp=_T("Auto");

            }
            //m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
            m_tstat_output_data.at(4).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
            m_tstat_output_data.at(4).AM.RegValue=nAMVAlue;
            m_tstat_output_data.at(4).AM.StrValue=strTemp;
            if(nRange>=0&&nRange<17)
            {
                strTemp=OUTPUT_ANRANGE[nRange];
            }
            m_tstat_output_data.at(4).Range.StrValue=strTemp;


        }
        else
        {
            short nRange;//=product_register_value[186];
            int nValue;//e=product_register_value[102];
            //if (product_register_value[7] == 6)
            nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
            m_tstat_output_data.at(5).Range.regAddress=MODBUS_OUTPUT1_SCALE;
            m_tstat_output_data.at(5).Range.RegValue=nRange;

            nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210
            m_tstat_output_data.at(5).Value.regAddress=MODBUS_COOLING_VALVE;
            m_tstat_output_data.at(5).Value.RegValue=nValue;
			if( m_tstat_output_data.at(5).Value.RegValue > 1000) //Add by Fance ,查理说out6 和7 不可能大于100%;
			{
				m_tstat_output_data.at(5).Value.RegValue = 1000;
			}
            if(nRange==0)
            {
                if(nValue==0)
                    strTemp=_T("Off");
                else
                    strTemp=_T("On");
            }
            else
            {
                float nvalue=0.0;
                //if (product_register_value[7] == 6)
                // 				if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i))
                // 				{
                // 					//AfxMessageBox(_T("It's impossible to enter this place!"));
                // 					if(nRange==1)//0-10v
                // 					{
                nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
                // 					}
                // 					if(nRange==2)//0-5v
                // 					{
                // 						nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
                // 					}
                // 					if(nRange==3)//2-10v
                // 					{
                // 						nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
                // 					}
                // 					if(nRange==3)//10-0v
                // 					{
                // 						nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
                // 					}
                // 					if (nRange==17)
                // 					{
                // 						nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;
                // 					}
                // 				}
                // 				else
                // 				{
                //
                //
                // 					if(nRange==1)//0-10v
                // 					{
                // 						//nvalue=product_register_value[102]/100 /10.0 * 100%;
                // 						nvalue=product_register_value[MODBUS_COOLING_VALVE]/10.0f;//102   210
                // 					}
                // 					if(nRange==2)//0-5v
                // 					{
                // 						nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
                // 					}
                // 					if(nRange==3)//2-10v
                // 					{
                // 						nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
                // 					}
                // 					if(nRange==3)//10-0v
                // 					{
                // 						nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
                // 					}
                // 				}
                strTemp.Format(_T("%.1f%%"),nvalue);
                m_tstat_output_data.at(5).Unit.StrValue = _T("%");
            }
            //m_FlexGrid.put_TextMatrix(6,VALUE_OUTFIELD,strTemp);
            m_tstat_output_data.at(5).Value.StrValue=strTemp;

            {



                if((int)(nAMVAlue & 32))
                {
                    strTemp=_T("Manual");
                    // 					m_FlexGrid.put_Col(VALUE_OUTFIELD);
                    // 					m_FlexGrid.put_Row(6);
                    // 					m_FlexGrid.put_CellBackColor(COLOR_CELL);
                }
                else
                {
                    strTemp=_T("Auto");
                    // 					m_FlexGrid.put_Col(VALUE_OUTFIELD);
                    // 					m_FlexGrid.put_Row(6);
                    // 					m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
                }
                m_tstat_output_data.at(5).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
                m_tstat_output_data.at(5).AM.RegValue=nAMVAlue;
                m_tstat_output_data.at(5).AM.StrValue=strTemp;
                //m_FlexGrid.put_TextMatrix(6,AM_OUTFIELD,strTemp);
                strTemp.Empty();
                if (nRange==17)
                {
                    nRange=4;
                }
                if(nRange>=0&&nRange<5)
                {
                    strTemp=OUTPUT_ANRANGE6[nRange];
                }


                m_tstat_output_data.at(5).Range.StrValue=strTemp;
                //m_FlexGrid.put_TextMatrix(6,RANG_OUTFIELD,strTemp);


                //103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
                //187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V
                //if (product_register_value[7] == 6)



                // 				nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
                // 				nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211

                nRange = product_register_value[MODBUS_OUTPUT2_SCALE]; //186  207
                m_tstat_output_data.at(6).Range.regAddress=MODBUS_OUTPUT2_SCALE;
                m_tstat_output_data.at(6).Range.RegValue=nRange;

                nValue = product_register_value[MODBUS_HEATING_VALVE]; //102  210
                m_tstat_output_data.at(6).Value.regAddress=MODBUS_HEATING_VALVE;
                m_tstat_output_data.at(6).Value.RegValue=nValue;
				if( m_tstat_output_data.at(6).Value.RegValue > 1000) //Add by Fance ,查理说out6 和7 不可能大于100%;
				{
					 m_tstat_output_data.at(6).Value.RegValue = 1000;
				}
                strTemp.Empty();
// 				if(nRange==0)
// 				{
//
// 					if(nValue==0)
// 						strTemp=_T("Off");
// 					else
// 						strTemp=_T("On");
//  				}
//  				else
                {
                    //strTemp.Format(_T("%.1f"),nValue/100.0);
                    float nvalue=0.0;

                    if(nRange==0)
                    {

                        if(nValue==0)
                            strTemp=_T("Off");
                        else
                            strTemp=_T("On");

                        m_tstat_output_data.at(6).Unit.StrValue =NO_APPLICATION;
                    }
                    else if(nRange==1)//0-10v
                    {
                        //nvalue=product_register_value[102]/100 /10.0 * 100%;
                        nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;

                        strTemp.Format(_T("%0.1f%%"),nvalue);
                        m_tstat_output_data.at(6).Unit.StrValue = _T("%");
                    }
                    else
                    {
                        strTemp.Format(_T("%d"),product_register_value[MODBUS_HEATING_VALVE]);
                        m_tstat_output_data.at(6).Unit.StrValue = _T("%");
                    }

                    m_tstat_output_data.at(6).Value.StrValue=strTemp;

                    if((int)(nAMVAlue & 64))
                    {
                        strTemp=_T("Manual");
                    }
                    else
                    {
                        strTemp=_T("Auto");
                    }
                    m_tstat_output_data.at(6).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
                    m_tstat_output_data.at(6).AM.RegValue=nAMVAlue;
                    m_tstat_output_data.at(6).AM.StrValue=strTemp;

                    strTemp.Empty();
                    if (nRange==17)
                    {
                        nRange=4;
                    }
                    if(nRange>=0&&nRange<5)
                    {
                        strTemp=OUTPUT_ANRANGE6[nRange];
                    }
                    m_tstat_output_data.at(6).Range.StrValue=strTemp;

                }


            }



            strTemp.Empty();

        }

        CString strlock;
        int stradd;// = 286;

        stradd =MODBUS_INTERLOCK_OUTPUT1; //245;
        for (int i = 0; i<7; i++)
        {

            int itemp = product_register_value[stradd+i];
            m_tstat_output_data.at(i).Interlock.regAddress=stradd+i;
            m_tstat_output_data.at(i).Interlock.RegValue=itemp;
            if(itemp>=0&&itemp<6)
                //m_FlexGrid.put_TextMatrix(i+1,INTER_LOCK,Interlock[itemp]);
                m_tstat_output_data.at(i).Interlock.StrValue=Interlock[itemp];

        }
        //Delay

        if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V))
        {
            CString strdelay;
            for (int row=0; row<7; row++)
            {
                stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
                int itemp=product_register_value[stradd];
                m_tstat_output_data.at(row).OFFON_Delay.regAddress=stradd;
                m_tstat_output_data.at(row).OFFON_Delay.RegValue=itemp;
                strdelay.Format(_T("%d"),itemp);
                m_tstat_output_data.at(row).OFFON_Delay.StrValue=strdelay;
                //m_FlexGrid.put_TextMatrix(row+1,DELAY_OFFON,strdelay);
            }
            for (int row=0; row<7; row++)
            {
                stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
                int itemp=product_register_value[stradd];
                strdelay.Format(_T("%d"),itemp);
                m_tstat_output_data.at(row).ONOFF_Delay.regAddress=stradd;
                m_tstat_output_data.at(row).ONOFF_Delay.RegValue=itemp;

                m_tstat_output_data.at(row).ONOFF_Delay.StrValue=strdelay;
                //m_FlexGrid.put_TextMatrix(row+1,DELAY_ONOFF,strdelay);
            }
        }
        else
        {
            CString strdelay;
            for (int row=0; row<5; row++)
            {
                stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
                int itemp=product_register_value[stradd];
                strdelay.Format(_T("%d"),itemp);
                m_tstat_output_data.at(row).OFFON_Delay.regAddress=stradd;
                m_tstat_output_data.at(row).OFFON_Delay.RegValue=itemp;
                m_tstat_output_data.at(row).OFFON_Delay.StrValue=strdelay;
                //m_FlexGrid.put_TextMatrix(row+1,DELAY_OFFON,strdelay);
            }
            for (int row=0; row<5; row++)
            {
                stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
                int itemp=product_register_value[stradd];
                strdelay.Format(_T("%d"),itemp);
                m_tstat_output_data.at(row).ONOFF_Delay.regAddress=stradd;
                m_tstat_output_data.at(row).ONOFF_Delay.RegValue=itemp;
                m_tstat_output_data.at(row).ONOFF_Delay.StrValue=strdelay;
                //m_FlexGrid.put_TextMatrix(row+1,DELAY_OFFON,strdelay);
            }
        }
        //AM    MODBUS_OUTPUT_MANU_ENABLE
        //if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)){
        //	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
        //	for (int row=1;row<=5;row++)
        //	{
        //		if (product_register_value[MODBUS_MODE_OUTPUT1+row-1]==3)
        //		{
        //			strTemp=_T("Auto");
        //		}
        //		else
        //		{
        //			if((int)(nAMVAlue & (1<<(row-1))) == (1<<(row-1)))
        //			{
        //				strTemp=_T("Manual");
        //			}
        //			else
        //			{
        //				strTemp=_T("Auto");
        //
        //			}
        //		}
        //		m_tstat_output_data.at(row-1).AM.regAddress=MODBUS_OUTPUT_MANU_ENABLE;
        //		m_tstat_output_data.at(row-1).AM.RegValue=nAMVAlue;
        //		m_tstat_output_data.at(row-1).AM.StrValue=strTemp;
        //		//m_FlexGrid.put_TextMatrix(row,AM_OUTFIELD,strTemp);
        //	}
        //}
        ///////////////////////////////////////Signal Type//////////////////////////
        if ((product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT8||product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT7)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V))
        {
            if (product_register_value[7]==PM_TSTAT6)
            {
                for (int row=1; row<6; row++)
                {
                    m_tstat_output_data.at(row-1).Signal_Type.StrValue=NO_APPLICATION;
                    //m_FlexGrid.put_TextMatrix(row,SIGUAL_TYPE,_T("UNUSED"));
                }
                CString strTemp;
                int ST=product_register_value[207];
                if (ST>=5&&ST<=16)
                {
                    strTemp=OUTPUT_ANRANGE[ST];
                }
                else
                {
                    strTemp=_T("UNUSED");
                }
                // 692 ->207
                m_tstat_output_data.at(5).Signal_Type.regAddress=207;
                m_tstat_output_data.at(5).Signal_Type.RegValue=ST;
                m_tstat_output_data.at(5).Signal_Type.StrValue=strTemp;
                //m_FlexGrid.put_TextMatrix(6,SIGUAL_TYPE,strTemp);
                strTemp=_T("UNUSED");
                ST=product_register_value[208];
                if (ST>=5&&ST<=16)
                {
                    strTemp=OUTPUT_ANRANGE[ST];
                }
                else
                {
                    strTemp=_T("UNUSED");
                }
                // 693 ->208
                m_tstat_output_data.at(6).Signal_Type.regAddress=208;
                m_tstat_output_data.at(6).Signal_Type.RegValue=ST;
                m_tstat_output_data.at(6).Signal_Type.StrValue=strTemp;
            }
            else
            {
                for (int row=1; row<8; row++)
                {
                    m_tstat_output_data.at(row-1).Signal_Type.StrValue=NO_APPLICATION;
                    //m_FlexGrid.put_TextMatrix(row,SIGUAL_TYPE,_T("UNUSED"));
                }
            }
            //m_FlexGrid.put_TextMatrix(7,SIGUAL_TYPE,strTemp);

        }
    }

    bitset<16> Output_AM(nAMVAlue);

    for (int i = 0 ; i<(int)m_tstat_output_data.size(); i++)
    {
        if (Output_AM[i])
        {
            strTemp=_T("Manual");
        }
        else
        {
            strTemp=_T("Auto");
        }
        m_tstat_output_data.at(i).AM.StrValue=strTemp;
        m_tstat_output_data.at(i).AM.RegValue=nAMVAlue;
        m_tstat_output_data.at(i).AM.regAddress =MODBUS_OUTPUT_MANU_ENABLE;
    }

}

void LoadOutputData_CS3000()
{
    m_tstat_output_data.clear();
    if (product_register_value[7]==PM_CS_SM_AC||product_register_value[7]==PM_CS_SM_DC)
    {
        return;
    }

    Tstat_Output_Struct out_struct_temp;
    m_tstat_output_data.push_back(out_struct_temp);
    m_tstat_output_data.at(0).OutputName.StrValue=_T("Output-1");
    m_tstat_output_data.at(0).Signal_Type.regAddress=142;
    m_tstat_output_data.at(0).Signal_Type.Reg_Property=REG_READ_WRITE;
    m_tstat_output_data.at(0).Signal_Type.RegValue=product_register_value[142];
    float signalvalue=-1;
    if (product_register_value[142]==0)
    {
        m_tstat_output_data.at(0).Signal_Type.StrValue=L"Current";
        m_tstat_output_data.at(0).Range.Reg_Property=REG_READ_WRITE;
        m_tstat_output_data.at(0).Range.regAddress=145;
        m_tstat_output_data.at(0).Range.RegValue=product_register_value[145];
        m_tstat_output_data.at(0).Range.StrValue.Format(_T("%d"),product_register_value[145]);

        float fValue=_wtoi(m_tstat_input_data.at(0).Value.StrValue);
        float frange=(float)(product_register_value[145]);
        signalvalue=(fValue/frange)*5;
        m_tstat_output_data.at(0).Value.StrValue.Format(_T("%0.2f V"),signalvalue);
    }
    else if(product_register_value[142]==1)
    {
        m_tstat_output_data.at(0).Signal_Type.StrValue=L"Voltage";
        m_tstat_output_data.at(0).Range.Reg_Property=REG_READ_WRITE;
        m_tstat_output_data.at(0).Range.regAddress=146;
        m_tstat_output_data.at(0).Range.RegValue=product_register_value[146];
        m_tstat_output_data.at(0).Range.StrValue.Format(_T("%d"),product_register_value[146]);


        float fValue=_wtoi(m_tstat_input_data.at(1).Value.StrValue);
        float frange=(float)(product_register_value[146]);
        signalvalue=(fValue/frange)*5;
        m_tstat_output_data.at(0).Value.StrValue.Format(_T("%0.2f V"),signalvalue);
    }

}

void LoadRegistersGraphicMode()
{
    int Product_Mode = product_register_value[7];
    if (Product_Mode == 0 ||Product_Mode == 255)
    {
        return;
    }
	if ((Product_Mode == PM_CO2_RS485&&product_register_value[14] == 6)||Product_Mode == STM32_HUM_NET||Product_Mode == STM32_HUM_RS485)
	{
		Product_Mode = PM_HUMTEMPSENSOR;
	}
    switch (Product_Mode)
    {
    case PM_AirQuality:
    {
        LoadRegistersGraphicMode_AQ();
    }
    break;
    case PM_HUM_R:
    {
        LoadRegistersGraphicMode_AQ();
    }
    break;
    case PM_HUMTEMPSENSOR:
    {
        LoadRegistersGraphicMode_HUMTEMPSENSOR();
    }
    break;
    case PM_CO2_RS485:
    {
        LoadRegistersGraphicMode_CO2485();
    }
    break;
    }
}

void LoadRegistersGraphicMode_AQ()
{
    g_calibration_module_data.Current_Frequency.regAddress = 305;
    g_calibration_module_data.Current_Frequency.StrValue = _T("Frequency");
    g_calibration_module_data.User_Table_Selection.regAddress=309;
    g_calibration_module_data.User_Table_Point_Number.regAddress= 307;
    g_calibration_module_data.User_Offset.regAddress = 106;
    g_calibration_module_data.User_Offset.RegValue = (short)product_register_value[106];
    g_calibration_module_data.User_Fre.regAddress	=312	 ;
    g_calibration_module_data.User_Fre.StrValue=_T("Frequency");
    g_calibration_module_data.User_Hum.regAddress	=313;
    g_calibration_module_data.User_Hum.StrValue =_T("Humidity(%)");
    g_calibration_module_data.Factory_Fre.regAddress  = 2005 ;
    g_calibration_module_data.Factory_Fre.StrValue=_T("Frequency");
    g_calibration_module_data.Factory_Hum.regAddress  =	 2004  ;
    g_calibration_module_data.Factory_Hum.StrValue =_T("Humidity(%)");

    //这个是AQ的Graphic的模块
}
void LoadRegistersGraphicMode_HUMTEMPSENSOR()
{
    g_calibration_module_data.Current_Frequency.regAddress = 374;
    g_calibration_module_data.Current_Frequency.StrValue = _T("Frequency");
    g_calibration_module_data.User_Table_Selection.regAddress=454;
    g_calibration_module_data.User_Table_Point_Number.regAddress=455;
    g_calibration_module_data.User_Offset.regAddress = 451;
    g_calibration_module_data.User_Offset.RegValue = (short)product_register_value[451];
    g_calibration_module_data.User_Fre.regAddress	 = 457;
    g_calibration_module_data.User_Fre.StrValue=_T("Frequency");
    g_calibration_module_data.User_Hum.regAddress	 = 456 ;
    g_calibration_module_data.User_Hum.StrValue =_T("Humidity(%)");
    g_calibration_module_data.Factory_Fre.regAddress  =381;
    g_calibration_module_data.Factory_Fre.StrValue=_T("Frequency");
    g_calibration_module_data.Factory_Hum.regAddress  =382;
    g_calibration_module_data.Factory_Hum.StrValue =_T("Humidity(%)");
}
void LoadRegistersGraphicMode_CO2485()
{
    g_calibration_module_data.Current_Frequency.regAddress = 710;
    g_calibration_module_data.Current_Frequency.StrValue = _T("AD");

    g_calibration_module_data.User_Table_Selection.regAddress=737;
    g_calibration_module_data.User_Table_Point_Number.regAddress=738;
    g_calibration_module_data.User_Offset.regAddress = 712;
    g_calibration_module_data.User_Offset.RegValue = (short)product_register_value[712];
    g_calibration_module_data.User_Fre.regAddress	 = 740;
    g_calibration_module_data.User_Fre.StrValue=_T("AD");
    g_calibration_module_data.User_Hum.regAddress	 = 739 ;
    g_calibration_module_data.User_Hum.StrValue =_T("Pressure");
    g_calibration_module_data.Factory_Fre.regAddress  =718;
    g_calibration_module_data.Factory_Fre.StrValue=_T("AD");
    g_calibration_module_data.Factory_Hum.regAddress  =717;
    g_calibration_module_data.Factory_Hum.StrValue =_T("Pressure");
}
BOOL ReadLineFromHexFile(CFile& file, char* pBuffer)
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
            /*if(!auto_flash_mode)*/
            //AfxMessageBox(_T("The Hex File is broken"));
            return FALSE;
        }

    }
    //TRACE(_T("%s"),pBuffer);
    return FALSE;
}
int Get_HexFile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info)
{
    CFileFind fFind;
    if(!fFind.FindFile(filepath))
        return FILE_NOT_FIND;

    //pBuf = new char[0x20000];

    CString strGetData;
    int nBufCount = 0;
//*****************inspect the file*********************


    DWORD dwHiAddr = 0; // 高位地址
    char readbuffer[256];
    ZeroMemory(readbuffer, 256);
    unsigned	char m_DeviceInfor[20];
    memset(m_DeviceInfor,0,20);
    CFile hexfile;
    if(hexfile.Open(filepath,CFile::modeRead))
    {
        unsigned int nLineNum=0;


        ZeroMemory(readbuffer, 256);
        hexfile.Seek(0, CFile::begin);
        while(ReadLineFromHexFile(hexfile, readbuffer))
        {
            nLineNum++;						//the line number that the wrong hex file;
           
          
            unsigned char get_hex[128]= {0};
            //get hex data,it is get from the line char
            //the number is (i-1)
            //int nLen = strGetData.GetLength();
            for(UINT i=0; i<strlen(readbuffer); i++) // 去掉冒号
            {
                readbuffer[i]=readbuffer[i+1];
            }

            int nLen = strlen(readbuffer)-2; // 不算回车换行的长度
            if(strlen(readbuffer)%2==0)
                turn_hex_file_line_to_unsigned_char(readbuffer);//turn every char to int
            else
            {
                return BAD_HEX_FILE;
            }
            turn_int_to_unsigned_char(readbuffer,nLen,get_hex);//turn to hex
            if(get_hex[3]==1)	//for to seektobegin() function,because to end of the file
                break;
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


            ret_bin_Info.software_low = m_DeviceInfor[15];
            ret_bin_Info.software_high =m_DeviceInfor[16];
            return READ_SUCCESS;



        }
    }
	 return NO_VERSION_INFO;
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
        unsigned char pBuf[BUF_LEN] = {'\0'};
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



int GetPictureBlockData_Blocking(uint32_t deviceid,int8_t nIndex, uint16_t ntotal_seg,uint16_t nseg_index)
{

	int send_status = true;

	for (int z=0; z<10; z++)
	{
		int temp_invoke_id = -1;
		int	resend_count = 0;
		send_status = true;
		do
		{
			resend_count ++;
			if(resend_count>10)
			{
				send_status = false;
				break;
			}
			if(deviceid == 0)
				temp_invoke_id =  GetPictureBlockData(g_bac_instance,	nIndex,	ntotal_seg,	nseg_index);
			else
				temp_invoke_id =  GetPictureBlockData(	deviceid,	nIndex,	ntotal_seg,	nseg_index);
			if(temp_invoke_id < 0)
				Sleep(2000);
			else
				send_status = true;
			//else
			//	Sleep(SEND_COMMAND_DELAY_TIME);
		}
		while (temp_invoke_id<0);
	
		if(send_status)
		{
			for (int i=0; i<300; i++)
			{
				Sleep(10);
				if(tsm_invoke_id_free(temp_invoke_id))
				{
					return 1;
				}
				else
					continue;
			}
		}
	}
	return -1;
}



/************************************************************************/
/*
Author: Fance Du
Get Bacnet picture Private Data
*/
/************************************************************************/
int GetPictureBlockData(uint32_t deviceid,int8_t nIndex, uint16_t ntotal_seg,uint16_t nseg_index)
{
	

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

	Str_picture_header private_data_chunk;
	//private_data_chunk.total_length = 0;
	private_data_chunk.total_seg = ntotal_seg;
	private_data_chunk.command = READPIC_T3000;
	private_data_chunk.index = nIndex;
	memset(private_data_chunk.unused,0,14);
	private_data_chunk.seg_index = nseg_index;



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
		g_llTxCount ++;
		return Send_ConfirmedPrivateTransfer(&dest,&private_data);
		//return g_invoke_id;
	}
	else
		return -2;

}



int WriteBacnetPictureData(uint32_t deviceid,uint8_t index , unsigned short transfer_packet, unsigned short total_packet,unsigned char * senddata)
{
	unsigned char command = WRITEPIC_T3000;

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
	entitysize = 400;

	char SendBuffer[1000];
	memset(SendBuffer,0,1000);
	char * temp_buffer = SendBuffer;

	Str_picture_header private_data_chunk;
	//Str_sub_user_data_header private_sub_data_chunk;

	int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
	unsigned char * n_temp_point = senddata;


	HEADER_LENGTH = 20;
	private_data_chunk.total_seg = total_packet;
	private_data_chunk.command = command;
	private_data_chunk.index = index;

	memset(private_data_chunk.unused,0,14);
	private_data_chunk.seg_index = transfer_packet;

	Set_transfer_length(420); 
	memcpy_s(SendBuffer,20 ,&private_data_chunk,20 );

	memcpy_s(SendBuffer + 20,400,n_temp_point,400);


	
	if(debug_item_show == DEBUG_SHOW_WRITE_PIC_DATA_ONLY)
	{
		CString temp_char;
		CString n_temp_print;
		char * temp_point;
		temp_point = SendBuffer;
		n_temp_print.Format(_T("picture_%d  pack %d %d  write:"),index,transfer_packet,total_packet);
		for (int i = 0; i< 420 ; i++)
		{
			temp_char.Format(_T("%02x"),(unsigned char)*temp_point);
			temp_char.MakeUpper();
			temp_point ++;
			n_temp_print = n_temp_print + temp_char + _T(" ");
		}
		DFTrace(n_temp_print);
	}


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

BOOL ShowBacnetView(unsigned char product_type)
{
	if((product_type == PM_MINIPANEL) || 
		(product_type == PM_CM5) )
		return true;
	else
		return false;
}

//检测字穿是否全是数字;
BOOL AllCharactorIsDigital(LPCTSTR lpszSrc)
{
	CString Src = lpszSrc;
	return (Src ==  Src.SpanIncluding( _T("0123456789" ) ));
}


bool Output_data_to_string(unsigned char  temp_output_index,
							CString &temp_out_panel,
							CString &temp_out_des,
							CString &temp_out_auto_manual,
							CString &temp_out_value,
							CString &temp_out_units,
							CString &temp_out_range,
							CString &temp_out_pwm_period,
							CString &temp_out_decom,
							CString &temp_out_label)
{
	Str_out_point temp_output_data;
	
	if(temp_output_index >= BAC_OUTPUT_ITEM_COUNT)
		return false;
	memcpy_s(&temp_output_data,sizeof(Str_out_point),&m_Output_data.at(temp_output_index),sizeof(Str_out_point));

	MultiByteToWideChar( CP_ACP, 0, (char *)temp_output_data.description, (int)strlen((char *)temp_output_data.description)+1, 
		temp_out_des.GetBuffer(MAX_PATH), MAX_PATH );
	temp_out_des.ReleaseBuffer();


	if(temp_output_data.auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
	{
		temp_out_auto_manual = _T("Auto");
	}
	else
	{
		temp_out_auto_manual = _T("Manual");
	}



	if(temp_output_data.digital_analog == BAC_UNITS_ANALOG)
	{
		if(temp_output_data.range == 0)
		{
			temp_out_range = _T("Unused");
		}
		else if(temp_output_data.range < (sizeof(OutPut_List_Analog_Range)/sizeof(OutPut_List_Analog_Range[0])))
			temp_out_range = OutPut_List_Analog_Range[temp_output_data.range];
		else
			temp_out_range = _T("Out of range");

		if(temp_output_data.range < (sizeof(OutPut_List_Analog_Units)/sizeof(OutPut_List_Analog_Units[0])))
			temp_out_units = OutPut_List_Analog_Units[temp_output_data.range];
		else
			temp_out_units = _T("Unused");

		CString temp_low,temp_high;
		temp_low.Format(_T("%d"),0);
		temp_high.Format(_T("%d"),0);

		temp_out_value.Format(_T("%.2f"),((float)temp_output_data.value) / 1000);
	}
	else if(temp_output_data.digital_analog == BAC_UNITS_DIGITAL)
	{
		temp_out_units = _T("");

		if(temp_output_data.range == 0)
		{
			CString temp_value2;
			temp_out_value.Format(_T("%.2f"),((float)temp_output_data.value) / 1000);
			temp_out_range = Digital_Units_Array[0];
		}
		else if(temp_output_data.range<=22)
			temp_out_range = Digital_Units_Array[temp_output_data.range];
		else if((temp_output_data.range >= 23) && (temp_output_data.range <= 30))
		{
			if(receive_customer_unit)
				temp_out_range = temp_unit_no_index[temp_output_data.range - 23];
			else
				temp_out_range = Digital_Units_Array[0];
		}
		else
			temp_out_range = Digital_Units_Array[0];

		if((temp_output_data.range>30) || (temp_output_data.range == 0))
		{
		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if((temp_output_data.range < 23) &&(temp_output_data.range !=0))
				temp1 = Digital_Units_Array[temp_output_data.range];
			else if((temp_output_data.range >=23) && (temp_output_data.range <= 30))
			{
				if(receive_customer_unit)
					temp1 = temp_unit_no_index[temp_output_data.range - 23];
			}
			else
			{
				temp_out_value.Empty();
			}
			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(temp_output_data.control == 0)
					temp_out_range = temparray.GetAt(0);
				else
					temp_out_range = temparray.GetAt(1);
			}
		}
	}

	temp_out_panel.Format(_T("%d"),(unsigned char)Station_NUM);


	if(temp_output_data.decom==0)
		temp_out_decom.Format(Output_Decom_Array[0]);
	else if(temp_output_data.decom==1)
		temp_out_decom.Format(Output_Decom_Array[1]);
	else
		temp_out_decom.Empty();



	temp_out_pwm_period.Format(_T("%u"),(unsigned char)temp_output_data.pwm_period);


	MultiByteToWideChar( CP_ACP, 0, (char *)temp_output_data.label, (int)strlen((char *)temp_output_data.label)+1, 
		temp_out_label.GetBuffer(MAX_PATH), MAX_PATH );
	temp_out_label.ReleaseBuffer();

	TRACE(temp_out_panel + _T(" ") +temp_out_des + _T(" ") +temp_out_auto_manual+ _T(" ") + temp_out_value + _T(" ")+  temp_out_units + _T(" ")+ temp_out_range+ _T(" ") +  temp_out_pwm_period + _T(" ") +  temp_out_decom+ _T(" ") + temp_out_label+ _T(" ")  + _T("\n"));
}

bool Input_data_to_string(unsigned char  temp_input_index ,
	CString &temp_in_main_panel,
	CString &temp_in_des,
	CString &temp_in_auto_manual,
	CString &temp_in_value,
	CString &temp_in_units,
	CString &temp_in_range,
	CString &temp_in_cal,
	CString &temp_cacl_sign,
	CString &temp_in_filter,
	CString &temp_in_decon,
	CString &temp_in_jumper,
	CString &temp_in_label)
{
	Str_in_point temp_input_data;

	if(temp_input_index >= BAC_INPUT_ITEM_COUNT)
		return false;

	memcpy_s(&temp_input_data,sizeof(Str_in_point),&m_Input_data.at(temp_input_index),sizeof(Str_in_point));

	temp_in_main_panel.Format(_T("%d"),(unsigned char)Station_NUM);


	MultiByteToWideChar( CP_ACP, 0, (char *)temp_input_data.description, (int)strlen((char *)temp_input_data.description)+1, 
		temp_in_des.GetBuffer(MAX_PATH), MAX_PATH );
	temp_in_des.ReleaseBuffer();

	if(temp_input_data.auto_manual==0)
	{
		temp_in_auto_manual.Format(_T("Auto"));
	}
	else
	{
		temp_in_auto_manual.Format(_T("Manual"));
	}

	if(temp_input_data.digital_analog == BAC_UNITS_ANALOG)
	{
		if(temp_input_data.range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
			temp_in_units = Input_List_Analog_Units[temp_input_data.range];
		else if(temp_input_data.range < (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
			temp_in_units.Empty();
		else
			temp_in_range = _T("Unused");

		if(temp_input_data.range == 0)
			temp_in_range = _T("Unused");
		else if(temp_input_data.range <  (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
			temp_in_range = Input_Analog_Units_Array[temp_input_data.range];
		else
			temp_in_range = _T("Out of range");

		CString cstemp_value;
		float temp_float_value;
		temp_float_value = ((float)temp_input_data.value) / 1000;
		temp_in_value.Format(_T("%.2f"),temp_float_value);

		unsigned short temp_cal_value = ((unsigned char)(temp_input_data.calibration_h)) *256 + (unsigned char)temp_input_data.calibration_l;

		temp_in_cal.Format(_T("%.1f"),((float)temp_cal_value)/10);
		if(temp_input_data.calibration_sign == 0)
		{
			temp_cacl_sign = _T("+");
		}
		else
		{
			temp_cacl_sign = _T("-");
		}
	}
	else if(temp_input_data.digital_analog == BAC_UNITS_DIGITAL)
	{
		temp_in_cal.Empty();
		if(temp_input_data.range == 0)
		{
			CString cstemp_value1;
			float temp_float_value1;
			temp_float_value1 = ((float)temp_input_data.value) / 1000;
			temp_in_value.Format(_T("%.2f"),temp_float_value1);
			temp_in_range = Digital_Units_Array[0];
		}
		else if(temp_input_data.range<=22)
		{
			temp_in_range = Digital_Units_Array[temp_input_data.range];
		}
		else if((temp_input_data.range >= 23) && (temp_input_data.range <= 30))
		{
			if(receive_customer_unit)
			{
				temp_in_range = temp_unit_no_index[temp_input_data.range - 23];
			}
			else
				temp_in_range = Digital_Units_Array[0];
		}
		else
		{
			temp_in_range = Digital_Units_Array[0];
		}
		temp_in_units = _T("");

		if((temp_input_data.range>30)  || (temp_input_data.range == 0))
		{

		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if((temp_input_data.range < 23) &&(temp_input_data.range !=0))
				temp1 = Digital_Units_Array[temp_input_data.range];
			else if((temp_input_data.range >=23) && (temp_input_data.range <= 30))
			{
				if(receive_customer_unit)
					temp1 = temp_unit_no_index[temp_input_data.range - 23];
			}

			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(temp_input_data.control == 0)
					temp_in_value = temparray.GetAt(0);
				else
					temp_in_value = temparray.GetAt(1);
			}
		}

	}


	temp_in_filter.Format(_T("%d"),(unsigned char)temp_input_data.filter);
	//m_input_list.SetItemText(i,INPUT_FITLER,temp_filter);

	int temp_decom = 0;
	int temp_jumper = 0;
	temp_decom =temp_input_data.decom & 0x0f;
	temp_jumper = (temp_input_data.decom & 0xf0 ) >> 4;
	CString temp_status;
	//如果range 是0 或者 不在正常范围内，就不要显示 open short 的报警 状态;
	if((temp_decom==0) || (temp_input_data.range == 0) || (temp_input_data.range > 30))
	{
		temp_status.Format(Decom_Array[0]);
	}
	else if(temp_decom==1)
	{
		temp_status.Format(Decom_Array[1]);
	}
	else if(temp_decom==2)
	{
		temp_status.Format(Decom_Array[2]);
	}
	else
	{
		temp_status.Empty();
	}
	temp_in_decon = temp_status;


	if(temp_jumper == 1)
	{
		temp_status.Format(JumperStatus[1]);
	}
	else if(temp_jumper == 2)
	{
		temp_status.Format(JumperStatus[2]);
	}
	else if(temp_jumper == 3)
	{
		temp_status.Format(JumperStatus[3]);
	}
	else
	{
		temp_status.Format(JumperStatus[0]);
	}

	temp_in_jumper = temp_status;


	MultiByteToWideChar( CP_ACP, 0, (char *)temp_input_data.label, (int)strlen((char *)temp_input_data.label)+1, 
		temp_in_label.GetBuffer(MAX_PATH), MAX_PATH );
	temp_in_label.ReleaseBuffer();

	TRACE(temp_in_main_panel + _T(" ") +temp_in_des + _T(" ") +temp_in_auto_manual+ _T(" ") + temp_in_value + _T(" ")+  temp_in_units + _T(" ")+ temp_in_range+ _T(" ") +  temp_in_cal + _T(" ") +  temp_cacl_sign+ _T(" ") + temp_in_filter+ _T(" ") + temp_in_decon + _T(" ")+ temp_in_jumper+ _T(" ") + temp_in_label + _T("\n"));

	return true;
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

extern char *ispoint_ex(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network,unsigned char & sub_panel, byte panel , int *netpresent);



/*
步骤 先读  若返回值> 0 就转换对应的数据;

int GetPrivateData_Blocking(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize);
bool Input_data_to_string(unsigned char  temp_input_index ,CString &temp_main_panel,CString &temp_in_des,
							CString &temp_in_auto_manual,CString &temp_in_value,CString &temp_in_units,
							CString &temp_in_range,CString &temp_in_cal,CString &temp_cacl_sign,
							CString &temp_in_filter,CString &temp_in_decon,CString &temp_in_jumper,CString &temp_in_label)
*/

bool Save_InputData_to_db(unsigned char  temp_input_index )
{
	return true;


	CString temp_in_main_panel;
	CString temp_in_des;
	CString temp_in_auto_manual;
	CString temp_in_value;
	CString temp_in_units;
	CString temp_in_range;
	CString temp_in_cal;
	CString temp_cacl_sign;
	CString temp_in_filter;
	CString temp_in_decon;
	CString temp_in_jumper;
	CString temp_in_label;

	Str_in_point temp_input_data;

	if(temp_input_index >= BAC_INPUT_ITEM_COUNT)
		return false;

	memcpy_s(&temp_input_data,sizeof(Str_in_point),&m_Input_data.at(temp_input_index),sizeof(Str_in_point));

	temp_in_main_panel.Format(_T("%d"),(unsigned char)Station_NUM);


	MultiByteToWideChar( CP_ACP, 0, (char *)temp_input_data.description, (int)strlen((char *)temp_input_data.description)+1, 
		temp_in_des.GetBuffer(MAX_PATH), MAX_PATH );
	temp_in_des.ReleaseBuffer();

	if(temp_input_data.auto_manual==0)
	{
		temp_in_auto_manual.Format(_T("Auto"));
	}
	else
	{
		temp_in_auto_manual.Format(_T("Manual"));
	}

	if(temp_input_data.digital_analog == BAC_UNITS_ANALOG)
	{
		if(temp_input_data.range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
			temp_in_units = Input_List_Analog_Units[temp_input_data.range];
		else if(temp_input_data.range < (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
			temp_in_units.Empty();
		else
			temp_in_range = _T("Unused");

		if(temp_input_data.range == 0)
			temp_in_range = _T("Unused");
		else if(temp_input_data.range <  (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
			temp_in_range = Input_Analog_Units_Array[temp_input_data.range];
		else
			temp_in_range = _T("Out of range");

		CString cstemp_value;
		float temp_float_value;
		temp_float_value = ((float)temp_input_data.value) / 1000;
		temp_in_value.Format(_T("%.2f"),temp_float_value);

		unsigned short temp_cal_value = ((unsigned char)(temp_input_data.calibration_h)) *256 + (unsigned char)temp_input_data.calibration_l;

		temp_in_cal.Format(_T("%.1f"),((float)temp_cal_value)/10);
		if(temp_input_data.calibration_sign == 0)
		{
			temp_cacl_sign = _T("+");
		}
		else
		{
			temp_cacl_sign = _T("-");
		}
	}
	else if(temp_input_data.digital_analog == BAC_UNITS_DIGITAL)
	{
		temp_in_cal.Empty();
		if(temp_input_data.range == 0)
		{
			CString cstemp_value1;
			float temp_float_value1;
			temp_float_value1 = ((float)temp_input_data.value) / 1000;
			temp_in_value.Format(_T("%.2f"),temp_float_value1);
			temp_in_range = Digital_Units_Array[0];
		}
		else if(temp_input_data.range<=22)
		{
			temp_in_range = Digital_Units_Array[temp_input_data.range];
		}
		else if((temp_input_data.range >= 23) && (temp_input_data.range <= 30))
		{
			if(receive_customer_unit)
			{
				temp_in_range = temp_unit_no_index[temp_input_data.range - 23];
			}
			else
				temp_in_range = Digital_Units_Array[0];
		}
		else
		{
			temp_in_range = Digital_Units_Array[0];
		}
		temp_in_units = _T("");

		if((temp_input_data.range>30)  || (temp_input_data.range == 0))
		{

		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if((temp_input_data.range < 23) &&(temp_input_data.range !=0))
				temp1 = Digital_Units_Array[temp_input_data.range];
			else if((temp_input_data.range >=23) && (temp_input_data.range <= 30))
			{
				if(receive_customer_unit)
					temp1 = temp_unit_no_index[temp_input_data.range - 23];
			}

			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(temp_input_data.control == 0)
					temp_in_value = temparray.GetAt(0);
				else
					temp_in_value = temparray.GetAt(1);
			}
		}

	}


	temp_in_filter.Format(_T("%d"),(unsigned char)temp_input_data.filter);
	//m_input_list.SetItemText(i,INPUT_FITLER,temp_filter);

	int temp_decom = 0;
	int temp_jumper = 0;
	temp_decom =temp_input_data.decom & 0x0f;
	temp_jumper = (temp_input_data.decom & 0xf0 ) >> 4;
	CString temp_status;
	//如果range 是0 或者 不在正常范围内，就不要显示 open short 的报警 状态;
	if((temp_decom==0) || (temp_input_data.range == 0) || (temp_input_data.range > 30))
	{
		temp_status.Format(Decom_Array[0]);
	}
	else if(temp_decom==1)
	{
		temp_status.Format(Decom_Array[1]);
	}
	else if(temp_decom==2)
	{
		temp_status.Format(Decom_Array[2]);
	}
	else
	{
		temp_status.Empty();
	}
	temp_in_decon = temp_status;


	if(temp_jumper == 1)
	{
		temp_status.Format(JumperStatus[1]);
	}
	else if(temp_jumper == 2)
	{
		temp_status.Format(JumperStatus[2]);
	}
	else if(temp_jumper == 3)
	{
		temp_status.Format(JumperStatus[3]);
	}
	else
	{
		temp_status.Format(JumperStatus[0]);
	}

	temp_in_jumper = temp_status;


	MultiByteToWideChar( CP_ACP, 0, (char *)temp_input_data.label, (int)strlen((char *)temp_input_data.label)+1, 
		temp_in_label.GetBuffer(MAX_PATH), MAX_PATH );
	temp_in_label.ReleaseBuffer();

	TRACE(temp_in_main_panel + _T(" ") +temp_in_des + _T(" ") +temp_in_auto_manual+ _T(" ") + temp_in_value + _T(" ")+  temp_in_units + _T(" ")+ temp_in_range+ _T(" ") +  temp_in_cal + _T(" ") +  temp_cacl_sign+ _T(" ") + temp_in_filter+ _T(" ") + temp_in_decon + _T(" ")+ temp_in_jumper+ _T(" ") + temp_in_label + _T("\n"));
	CppSQLite3DB SqliteDBBuilding;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	if (!SqliteDBBuilding.tableExists("INPUTable"))
	{

		strSql.Format(_T("CREATE TABLE [INPUTable] ([PanelID] CHAR(255), [InputIndex] CHAR(255), [InputDescription] CHAR(255), [InputAM] CHAR(255), [InputValue] CHAR(255), [InputUnit] CHAR(255), [InputRange] CHAR(255), [InputCalibration] CHAR(255), [InputCalSign] CHAR(255), [InputFilter] CHAR(255), [InputDecon] CHAR(255), [InputJumper] CHAR(255), [InputLabel] CHAR(255));"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

	}
	CString StrTemp;
	StrTemp.Format(_T("%d"),temp_input_index);
	strSql.Format(_T("delete from INPUTable where PanelID = '%s' and InputIndex = '%s' "),temp_in_main_panel,StrTemp);
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);



	strSql.Format(_T("Insert Into INPUTable (PanelID,InputIndex,InputDescription,InputAM,InputValue,InputUnit,InputRange,InputCalibration,InputCalSign,InputFilter,InputDecon,InputJumper,InputLabel) Values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
		temp_in_main_panel,
		StrTemp,
		temp_in_des,
		temp_in_auto_manual,
		temp_in_value,
		temp_in_units,
		temp_in_range,
		temp_in_cal,
		temp_cacl_sign,
		temp_in_filter,
		temp_in_decon,
		temp_in_jumper,
		temp_in_label);

	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	SqliteDBBuilding.closedb();
	return true;
}


bool Save_OutputData_to_db(unsigned char  temp_output_index)
{
	return true;



	CString  temp_out_panel;
	CString  temp_out_des;
	CString  temp_out_auto_manual;
	CString  temp_out_value;
	CString  temp_out_units;
	CString  temp_out_range;
	CString  temp_out_pwm_period;
	CString  temp_out_decom;
	CString  temp_out_label ;
	Str_out_point temp_output_data;

	if(temp_output_index >= BAC_OUTPUT_ITEM_COUNT)
		return false;
	memcpy_s(&temp_output_data,sizeof(Str_out_point),&m_Output_data.at(temp_output_index),sizeof(Str_out_point));

	MultiByteToWideChar( CP_ACP, 0, (char *)temp_output_data.description, (int)strlen((char *)temp_output_data.description)+1, 
		temp_out_des.GetBuffer(MAX_PATH), MAX_PATH );
	temp_out_des.ReleaseBuffer();


	if(temp_output_data.auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
	{
		temp_out_auto_manual = _T("Auto");
	}
	else
	{
		temp_out_auto_manual = _T("Manual");
	}



	if(temp_output_data.digital_analog == BAC_UNITS_ANALOG)
	{
		if(temp_output_data.range == 0)
		{
			temp_out_range = _T("Unused");
		}
		else if(temp_output_data.range < (sizeof(OutPut_List_Analog_Range)/sizeof(OutPut_List_Analog_Range[0])))
			temp_out_range = OutPut_List_Analog_Range[temp_output_data.range];
		else
			temp_out_range = _T("Out of range");

		if(temp_output_data.range < (sizeof(OutPut_List_Analog_Units)/sizeof(OutPut_List_Analog_Units[0])))
			temp_out_units = OutPut_List_Analog_Units[temp_output_data.range];
		else
			temp_out_units = _T("Unused");

		CString temp_low,temp_high;
		temp_low.Format(_T("%d"),0);
		temp_high.Format(_T("%d"),0);

		temp_out_value.Format(_T("%.2f"),((float)temp_output_data.value) / 1000);
	}
	else if(temp_output_data.digital_analog == BAC_UNITS_DIGITAL)
	{
		temp_out_units = _T("");

		if(temp_output_data.range == 0)
		{
			CString temp_value2;
			temp_out_value.Format(_T("%.2f"),((float)temp_output_data.value) / 1000);
			temp_out_range = Digital_Units_Array[0];
		}
		else if(temp_output_data.range<=22)
			temp_out_range = Digital_Units_Array[temp_output_data.range];
		else if((temp_output_data.range >= 23) && (temp_output_data.range <= 30))
		{
			if(receive_customer_unit)
				temp_out_range = temp_unit_no_index[temp_output_data.range - 23];
			else
				temp_out_range = Digital_Units_Array[0];
		}
		else
			temp_out_range = Digital_Units_Array[0];

		if((temp_output_data.range>30) || (temp_output_data.range == 0))
		{
		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if((temp_output_data.range < 23) &&(temp_output_data.range !=0))
				temp1 = Digital_Units_Array[temp_output_data.range];
			else if((temp_output_data.range >=23) && (temp_output_data.range <= 30))
			{
				if(receive_customer_unit)
					temp1 = temp_unit_no_index[temp_output_data.range - 23];
			}
			else
			{
				temp_out_value.Empty();
			}
			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(temp_output_data.control == 0)
					temp_out_range = temparray.GetAt(0);
				else
					temp_out_range = temparray.GetAt(1);
			}
		}
	}

	temp_out_panel.Format(_T("%d"),(unsigned char)Station_NUM);


	if(temp_output_data.decom==0)
		temp_out_decom.Format(Output_Decom_Array[0]);
	else if(temp_output_data.decom==1)
		temp_out_decom.Format(Output_Decom_Array[1]);
	else
		temp_out_decom.Empty();



	temp_out_pwm_period.Format(_T("%u"),(unsigned char)temp_output_data.pwm_period);


	MultiByteToWideChar( CP_ACP, 0, (char *)temp_output_data.label, (int)strlen((char *)temp_output_data.label)+1, 
		temp_out_label.GetBuffer(MAX_PATH), MAX_PATH );
	temp_out_label.ReleaseBuffer();

	TRACE(temp_out_panel + _T(" ") +temp_out_des + _T(" ") +temp_out_auto_manual+ _T(" ") + temp_out_value + _T(" ")+  temp_out_units + _T(" ")+ temp_out_range+ _T(" ") +  temp_out_pwm_period + _T(" ") +  temp_out_decom+ _T(" ") + temp_out_label+ _T(" ")  + _T("\n"));
	CppSQLite3DB SqliteDBBuilding;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	if (!SqliteDBBuilding.tableExists("OUTPUTable"))
	{

		strSql.Format(_T("CREATE TABLE [OUTPUTable] ([out_panel] CHAR(255),[out_index] CHAR(255),[out_am] CHAR(255),[out_value] CHAR(255),[out_units] CHAR(255),[out_range] CHAR(255),[out_pwm_period] CHAR(255),[out_decom] CHAR(255),[out_label] CHAR(255));"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

	}
	CString StrTemp;
	StrTemp.Format(_T("%d"),temp_output_index);
	strSql.Format(_T("delete from OUTPUTable where out_panel = '%s' and out_index = '%s' "),temp_out_panel,StrTemp);
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


	strSql.Format(_T("Insert INTO OUTPUTable(out_panel,out_index,out_am,out_value,out_units,out_range,out_pwm_period,out_decom,out_label) Values('%s','%s','%s','%s','%s','%s','%s','%s','%s' )"),
		temp_out_panel,
		StrTemp,
		temp_out_des,
		temp_out_auto_manual,
		temp_out_value,
		temp_out_units,
		temp_out_range,
		temp_out_pwm_period,
		temp_out_decom,
		temp_out_label);

	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	SqliteDBBuilding.closedb();	
}
bool Save_AVData_to_db()
{
	CString strPanel;
	strPanel.Format(_T("%d"), (unsigned char)Station_NUM);

	CppSQLite3DB SqliteDBBuilding;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	if (!SqliteDBBuilding.tableExists("VariablesTable"))
	{

		strSql.Format(_T("CREATE TABLE [VariablesTable] ([var_panel] CHAR(255),[var_index] CHAR(255),[var_am] CHAR(255),[var_value] CHAR(255),[var_units] CHAR(255),[var_label] CHAR(255));"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

	}
 
	strSql.Format(_T("delete from VariablesTable "));
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	for (int i = 0; i < (int)m_Variable_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des,temp_am;
		CString temp_units;
		CString Strindex;
		Strindex.Format(_T("%d"), i);
		 

		if (i >= variable_item_limit_count)
		{
			return 0;
		}

		MultiByteToWideChar(CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description) + 1,
			temp_des.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des.ReleaseBuffer();
		 
		if (m_Variable_data.at(i).auto_manual == 0)
		{
			temp_am= _T("Auto");
		}
		else
		{
			temp_am = _T("Manual");
		}


		if (m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{

			if ((m_Variable_data.at(i).range == 0) || (m_Variable_data.at(i).range > 30))
			{
				CString cstemp_value2;
				float temp_float_value1;
				temp_float_value1 = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value2.Format(_T("%.3f"), temp_float_value1);
				 
				temp_units= Variable_Analog_Units_Array[0];
			}
			else
			{
			 
				CStringArray temparray;

				if ((m_Variable_data.at(i).range < 23) && (m_Variable_data.at(i).range != 0))
					temp_units = Digital_Units_Array[m_Variable_data.at(i).range];
				else if ((m_Variable_data.at(i).range >= 23) && (m_Variable_data.at(i).range <= 30))
				{
					if (receive_customer_unit)
						temp_units = temp_unit_no_index[m_Variable_data.at(i).range - 23];
				}
				else
				{
					temp_units = Digital_Units_Array[0];
					 
				}

				SplitCStringA(temparray, temp_units, _T("/"));
				if ((temparray.GetSize() == 2))
				{
					if (m_Variable_data.at(i).control == 0)
						temp_value = temparray.GetAt(0);
					else
						temp_value=temparray.GetAt(1);
					 
				}

			}
 
		}
		else
		{
			if (m_Variable_data.at(i).range == 20)	//如果是时间;
			{
				temp_units= Variable_Analog_Units_Array[m_Variable_data.at(i).range];
				char temp_char[50];
				int time_seconds = m_Variable_data.at(i).value / 1000;
				intervaltotextfull(temp_char, time_seconds, 0, 0);
				CString temp_11;
				MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1,
					temp_11.GetBuffer(MAX_PATH), MAX_PATH);
				temp_11.ReleaseBuffer();
				temp_value=temp_11;
 
			}
		 
			else if (m_Variable_data.at(i).range < sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0]))
			{
				temp_units=Variable_Analog_Units_Array[m_Variable_data.at(i).range];

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"), temp_float_value);
				temp_value = cstemp_value;

			 
			}
			else if ((m_Variable_data.at(i).range >= 34) && (m_Variable_data.at(i).range <= 38))
			{
				temp_units=Analog_Variable_Units[m_Variable_data.at(i).range - 34];
				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"), temp_float_value);
				temp_value = cstemp_value;
			}
			else
			{
				temp_units =Variable_Analog_Units_Array[0];

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"), temp_float_value);
				temp_value=cstemp_value;
			}

		}

		CString temp_des2;
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label) + 1,
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des2.ReleaseBuffer();
		temp_des= temp_des2;
		 
 		strSql.Format(_T("Insert INTO VariablesTable(var_panel,var_index,var_am,var_value,var_units,var_label) Values('%s','%s','%s','%s','%s','%s' )"),
 			strPanel,
 			Strindex,
			temp_am,
			temp_value,
			temp_units,
			temp_des
 			);

		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		
		 

	}
	SqliteDBBuilding.closedb();
 
	
}
CString GetGUID()
{
	GUID guid;
	CString strGUID;
	if (CoCreateGuid(&guid))
	{
// 		fprintf(stderr, "create guid error\n");
// 		return -1;
	}

	strGUID.Format(_T("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"), guid.Data1
		, guid.Data2
		, guid.Data3
		, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
		, guid.Data4[6], guid.Data4[7]);
	return strGUID;
}



bool Save_InputData_to_db(unsigned char  temp_input_index, unsigned int nserialnumber)
{

	CString temp_in_main_panel;
	CString temp_in_des;
	CString temp_in_auto_manual;
	CString temp_in_value;
	CString temp_in_units;
	CString temp_in_range;
	CString temp_in_cal;
	CString temp_cacl_sign;
	CString temp_in_filter;
	CString temp_in_decon;
	CString temp_in_jumper;
	CString temp_in_label;

	Str_in_point temp_input_data;

	if (temp_input_index >= BAC_INPUT_ITEM_COUNT)
		return false;

	memcpy_s(&temp_input_data, sizeof(Str_in_point), &m_Input_data.at(temp_input_index), sizeof(Str_in_point));

	temp_in_main_panel.Format(_T("%d"), (unsigned char)Station_NUM);


	MultiByteToWideChar(CP_ACP, 0, (char *)temp_input_data.description, (int)strlen((char *)temp_input_data.description) + 1,
		temp_in_des.GetBuffer(MAX_PATH), MAX_PATH);
	temp_in_des.ReleaseBuffer();

	if (temp_input_data.auto_manual == 0)
	{
		temp_in_auto_manual.Format(_T("Auto"));
	}
	else
	{
		temp_in_auto_manual.Format(_T("Manual"));
	}

	if (temp_input_data.digital_analog == BAC_UNITS_ANALOG)
	{
		if (temp_input_data.range <  (sizeof(Input_List_Analog_Units) / sizeof(Input_List_Analog_Units[0])))
			temp_in_units = Input_List_Analog_Units[temp_input_data.range];
		else if (temp_input_data.range < (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			temp_in_units.Empty();
		else
			temp_in_range = _T("Unused");

		if (temp_input_data.range == 0)
			temp_in_range = _T("Unused");
		else if (temp_input_data.range <  (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			temp_in_range = Input_Analog_Units_Array[temp_input_data.range];
		else
			temp_in_range = _T("Out of range");

		CString cstemp_value;
		float temp_float_value;
		temp_float_value = ((float)temp_input_data.value) / 1000;
		temp_in_value.Format(_T("%.2f"), temp_float_value);

		unsigned short temp_cal_value = ((unsigned char)(temp_input_data.calibration_h)) * 256 + (unsigned char)temp_input_data.calibration_l;

		temp_in_cal.Format(_T("%.1f"), ((float)temp_cal_value) / 10);
		if (temp_input_data.calibration_sign == 0)
		{
			temp_cacl_sign = _T("+");
		}
		else
		{
			temp_cacl_sign = _T("-");
		}
	}
	else if (temp_input_data.digital_analog == BAC_UNITS_DIGITAL)
	{
		temp_in_cal.Empty();
		if (temp_input_data.range == 0)
		{
			CString cstemp_value1;
			float temp_float_value1;
			temp_float_value1 = ((float)temp_input_data.value) / 1000;
			temp_in_value.Format(_T("%.2f"), temp_float_value1);
			temp_in_range = Digital_Units_Array[0];
		}
		else if (temp_input_data.range <= 22)
		{
			temp_in_range = Digital_Units_Array[temp_input_data.range];
		}
		else if ((temp_input_data.range >= 23) && (temp_input_data.range <= 30))
		{
			if (receive_customer_unit)
			{
				temp_in_range = temp_unit_no_index[temp_input_data.range - 23];
			}
			else
				temp_in_range = Digital_Units_Array[0];
		}
		else
		{
			temp_in_range = Digital_Units_Array[0];
		}
		temp_in_units = _T("");

		if ((temp_input_data.range>30) || (temp_input_data.range == 0))
		{

		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if ((temp_input_data.range < 23) && (temp_input_data.range != 0))
				temp1 = Digital_Units_Array[temp_input_data.range];
			else if ((temp_input_data.range >= 23) && (temp_input_data.range <= 30))
			{
				if (receive_customer_unit)
					temp1 = temp_unit_no_index[temp_input_data.range - 23];
			}

			SplitCStringA(temparray, temp1, _T("/"));
			if ((temparray.GetSize() == 2))
			{
				if (temp_input_data.control == 0)
					temp_in_value = temparray.GetAt(0);
				else
					temp_in_value = temparray.GetAt(1);
			}
		}

	}


	temp_in_filter.Format(_T("%d"), (unsigned char)temp_input_data.filter);
	//m_input_list.SetItemText(i,INPUT_FITLER,temp_filter);

	int temp_decom = 0;
	int temp_jumper = 0;
	temp_decom = temp_input_data.decom & 0x0f;
	temp_jumper = (temp_input_data.decom & 0xf0) >> 4;
	CString temp_status;
	//Èç¹ûrange ÊÇ0 »òÕß ²»ÔÚÕý³£·¶Î§ÄÚ£¬¾Í²»ÒªÏÔÊ¾ open short µÄ±¨¾¯ ×´Ì¬;
	if ((temp_decom == 0) || (temp_input_data.range == 0) || (temp_input_data.range > 30))
	{
		temp_status.Format(Decom_Array[0]);
	}
	else if (temp_decom == 1)
	{
		temp_status.Format(Decom_Array[1]);
	}
	else if (temp_decom == 2)
	{
		temp_status.Format(Decom_Array[2]);
	}
	else
	{
		temp_status.Empty();
	}
	temp_in_decon = temp_status;


	if (temp_jumper == 1)
	{
		temp_status.Format(JumperStatus[1]);
	}
	else if (temp_jumper == 2)
	{
		temp_status.Format(JumperStatus[2]);
	}
	else if (temp_jumper == 3)
	{
		temp_status.Format(JumperStatus[3]);
	}
	else
	{
		temp_status.Format(JumperStatus[0]);
	}

	temp_in_jumper = temp_status;


	MultiByteToWideChar(CP_ACP, 0, (char *)temp_input_data.label, (int)strlen((char *)temp_input_data.label) + 1,
		temp_in_label.GetBuffer(MAX_PATH), MAX_PATH);
	temp_in_label.ReleaseBuffer();

	//TRACE(temp_in_main_panel + _T(" ") +temp_in_des + _T(" ") +temp_in_auto_manual+ _T(" ") + temp_in_value + _T(" ")+  temp_in_units + _T(" ")+ temp_in_range+ _T(" ") +  temp_in_cal + _T(" ") +  temp_cacl_sign+ _T(" ") + temp_in_filter+ _T(" ") + temp_in_decon + _T(" ")+ temp_in_jumper+ _T(" ") + temp_in_label + _T("\n"));
	CppSQLite3DB SqliteDBBuilding;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	if (!SqliteDBBuilding.tableExists("INPUTable"))
	{

		strSql.Format(_T("CREATE TABLE [INPUTable] ([SerialNumber] Int, [PanelID] CHAR(255), [InputIndex] CHAR(255), [InputDescription] CHAR(255), [InputAM] CHAR(255), [InputValue] CHAR(255), [InputUnit] CHAR(255), [InputRange] CHAR(255), [InputCalibration] CHAR(255), [InputCalSign] CHAR(255), [InputFilter] CHAR(255), [InputDecon] CHAR(255), [InputJumper] CHAR(255), [InputLabel] CHAR(255));"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

	}
	else
	{
		if (!SqliteDBBuilding.tableColumnExists(_T("INPUTable"), _T("SerialNumber")))
		{
			strSql.Format(_T("Drop table INPUTable"));
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			strSql.Format(_T("CREATE TABLE [INPUTable] ([SerialNumber] Int, [PanelID] CHAR(255), [InputIndex] CHAR(255), [InputDescription] CHAR(255), [InputAM] CHAR(255), [InputValue] CHAR(255), [InputUnit] CHAR(255), [InputRange] CHAR(255), [InputCalibration] CHAR(255), [InputCalSign] CHAR(255), [InputFilter] CHAR(255), [InputDecon] CHAR(255), [InputJumper] CHAR(255), [InputLabel] CHAR(255));"));
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		}
	}


	CString StrTemp;
	StrTemp.Format(_T("%d"), temp_input_index);
	strSql.Format(_T("delete from INPUTable where SerialNumber = '%u' and InputIndex = '%s'"), nserialnumber, StrTemp);
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);



	strSql.Format(_T("Insert Into INPUTable (SerialNumber,PanelID,InputIndex,InputDescription,InputAM,InputValue,InputUnit,InputRange,InputCalibration,InputCalSign,InputFilter,InputDecon,InputJumper,InputLabel) Values(%u,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
		nserialnumber,
		temp_in_main_panel,
		StrTemp,
		temp_in_des,
		temp_in_auto_manual,
		temp_in_value,
		temp_in_units,
		temp_in_range,
		temp_in_cal,
		temp_cacl_sign,
		temp_in_filter,
		temp_in_decon,
		temp_in_jumper,
		temp_in_label);

	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	SqliteDBBuilding.closedb();
	return true;
}


bool Save_OutputData_to_db(unsigned char  temp_output_index, unsigned int nserialnumber)
{

	CString  temp_out_panel;
	CString  temp_out_des;
	CString  temp_out_auto_manual;
	CString  temp_out_value;
	CString  temp_out_units;
	CString  temp_out_range;
	CString  temp_out_pwm_period;
	CString  temp_out_decom;
	CString  temp_out_label;
	Str_out_point temp_output_data;

	if (temp_output_index >= BAC_OUTPUT_ITEM_COUNT)
		return false;
	memcpy_s(&temp_output_data, sizeof(Str_out_point), &m_Output_data.at(temp_output_index), sizeof(Str_out_point));

	MultiByteToWideChar(CP_ACP, 0, (char *)temp_output_data.description, (int)strlen((char *)temp_output_data.description) + 1,
		temp_out_des.GetBuffer(MAX_PATH), MAX_PATH);
	temp_out_des.ReleaseBuffer();


	if (temp_output_data.auto_manual == 0)	//In output table if it is auto ,the value can't be edit by user
	{
		temp_out_auto_manual = _T("Auto");
	}
	else
	{
		temp_out_auto_manual = _T("Manual");
	}



	if (temp_output_data.digital_analog == BAC_UNITS_ANALOG)
	{
		if (temp_output_data.range == 0)
		{
			temp_out_range = _T("Unused");
		}
		else if (temp_output_data.range < (sizeof(OutPut_List_Analog_Range) / sizeof(OutPut_List_Analog_Range[0])))
			temp_out_range = OutPut_List_Analog_Range[temp_output_data.range];
		else
			temp_out_range = _T("Out of range");

		if (temp_output_data.range < (sizeof(OutPut_List_Analog_Units) / sizeof(OutPut_List_Analog_Units[0])))
			temp_out_units = OutPut_List_Analog_Units[temp_output_data.range];
		else
			temp_out_units = _T("Unused");

		CString temp_low, temp_high;
		temp_low.Format(_T("%d"), 0);
		temp_high.Format(_T("%d"), 0);

		temp_out_value.Format(_T("%.2f"), ((float)temp_output_data.value) / 1000);
	}
	else if (temp_output_data.digital_analog == BAC_UNITS_DIGITAL)
	{
		temp_out_units = _T("");

		if (temp_output_data.range == 0)
		{
			CString temp_value2;
			temp_out_value.Format(_T("%.2f"), ((float)temp_output_data.value) / 1000);
			temp_out_range = Digital_Units_Array[0];
		}
		else if (temp_output_data.range <= 22)
			temp_out_range = Digital_Units_Array[temp_output_data.range];
		else if ((temp_output_data.range >= 23) && (temp_output_data.range <= 30))
		{
			if (receive_customer_unit)
				temp_out_range = temp_unit_no_index[temp_output_data.range - 23];
			else
				temp_out_range = Digital_Units_Array[0];
		}
		else
			temp_out_range = Digital_Units_Array[0];

		if ((temp_output_data.range>30) || (temp_output_data.range == 0))
		{
		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if ((temp_output_data.range < 23) && (temp_output_data.range != 0))
				temp1 = Digital_Units_Array[temp_output_data.range];
			else if ((temp_output_data.range >= 23) && (temp_output_data.range <= 30))
			{
				if (receive_customer_unit)
					temp1 = temp_unit_no_index[temp_output_data.range - 23];
			}
			else
			{
				temp_out_value.Empty();
			}
			SplitCStringA(temparray, temp1, _T("/"));
			if ((temparray.GetSize() == 2))
			{
				if (temp_output_data.control == 0)
					temp_out_range = temparray.GetAt(0);
				else
					temp_out_range = temparray.GetAt(1);
			}
		}
	}

	temp_out_panel.Format(_T("%d"), (unsigned char)Station_NUM);


	if (temp_output_data.decom == 0)
		temp_out_decom.Format(Output_Decom_Array[0]);
	else if (temp_output_data.decom == 1)
		temp_out_decom.Format(Output_Decom_Array[1]);
	else
		temp_out_decom.Empty();



	temp_out_pwm_period.Format(_T("%u"), (unsigned char)temp_output_data.pwm_period);


	MultiByteToWideChar(CP_ACP, 0, (char *)temp_output_data.label, (int)strlen((char *)temp_output_data.label) + 1,
		temp_out_label.GetBuffer(MAX_PATH), MAX_PATH);
	temp_out_label.ReleaseBuffer();

	//	TRACE(temp_out_panel + _T(" ") +temp_out_des + _T(" ") +temp_out_auto_manual+ _T(" ") + temp_out_value + _T(" ")+  temp_out_units + _T(" ")+ temp_out_range+ _T(" ") +  temp_out_pwm_period + _T(" ") +  temp_out_decom+ _T(" ") + temp_out_label+ _T(" ")  + _T("\n"));
	CppSQLite3DB SqliteDBBuilding;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	if (!SqliteDBBuilding.tableExists("OUTPUTable"))
	{

		strSql.Format(_T("CREATE TABLE [OUTPUTable] ([SerialNumber] Int,[out_panel] CHAR(255),[out_index] CHAR(255),[out_am] CHAR(255),[out_value] CHAR(255),[out_units] CHAR(255),[out_range] CHAR(255),[out_pwm_period] CHAR(255),[out_decom] CHAR(255),[out_label] CHAR(255));"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

	}
	else
	{
		if (!SqliteDBBuilding.tableColumnExists(_T("OUTPUTable"), _T("SerialNumber")))
		{
			strSql.Format(_T("Drop table OUTPUTable"));
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			strSql.Format(_T("CREATE TABLE [OUTPUTable] ([SerialNumber] Int,[out_panel] CHAR(255),[out_index] CHAR(255),[out_am] CHAR(255),[out_value] CHAR(255),[out_units] CHAR(255),[out_range] CHAR(255),[out_pwm_period] CHAR(255),[out_decom] CHAR(255),[out_label] CHAR(255));"));
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		}
	}

	CString StrTemp;
	StrTemp.Format(_T("%d"), temp_output_index);
	strSql.Format(_T("delete from OUTPUTable where SerialNumber = '%u' and out_index = '%s'"), nserialnumber, StrTemp);
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


	strSql.Format(_T("Insert INTO OUTPUTable(SerialNumber,out_panel,out_index,out_am,out_value,out_units,out_range,out_pwm_period,out_decom,out_label) Values(%u,'%s','%s','%s','%s','%s','%s','%s','%s','%s' )"),
		nserialnumber,
		temp_out_panel,
		StrTemp,
		temp_out_des,
		temp_out_auto_manual,
		temp_out_value,
		temp_out_units,
		temp_out_range,
		temp_out_pwm_period,
		temp_out_decom,
		temp_out_label);

	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	SqliteDBBuilding.closedb();
}
bool Save_VariableData_to_db(unsigned char  temp_output_index, unsigned int nserialnumber)
{

	//return true;


	CString strPanel;
	strPanel.Format(_T("%d"), (unsigned char)Station_NUM);

	CppSQLite3DB SqliteDBBuilding;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	if (!SqliteDBBuilding.tableExists("VariablesTable"))
	{
		strSql.Format(_T("CREATE TABLE [VariablesTable] ([SerialNumber] Int,[var_panel] CHAR(255),[var_index] CHAR(255),[var_am] CHAR(255),[var_value] CHAR(255),[var_units] CHAR(255),[var_label] CHAR(255));"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	}
	else
	{
		if (!SqliteDBBuilding.tableColumnExists(_T("VariablesTable"), _T("SerialNumber")))
		{
			strSql.Format(_T("Drop table VariablesTable"));
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			strSql.Format(_T("CREATE TABLE [VariablesTable] ([SerialNumber] Int,[var_panel] CHAR(255),[var_index] CHAR(255),[var_am] CHAR(255),[var_value] CHAR(255),[var_units] CHAR(255),[var_label] CHAR(255));"));
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		}
	}
	unsigned char i;
	i = temp_output_index;
	CString Strindex;
	Strindex.Format(_T("%u"), i);

	strSql.Format(_T("delete from VariablesTable where SerialNumber = '%u' and var_index = '%s'"), nserialnumber, Strindex);
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


	CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
	CString temp_des, temp_am;
	CString temp_units;



	if (i >= variable_item_limit_count)
	{
		return 0;
	}

	MultiByteToWideChar(CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description) + 1,
		temp_des.GetBuffer(MAX_PATH), MAX_PATH);
	temp_des.ReleaseBuffer();

	if (m_Variable_data.at(i).auto_manual == 0)
	{
		temp_am = _T("Auto");
	}
	else
	{
		temp_am = _T("Manual");
	}


	if (m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{

		if ((m_Variable_data.at(i).range == 0) || (m_Variable_data.at(i).range > 30))
		{
			CString cstemp_value2;
			float temp_float_value1;
			temp_float_value1 = ((float)m_Variable_data.at(i).value) / 1000;
			cstemp_value2.Format(_T("%.3f"), temp_float_value1);

			temp_units = Variable_Analog_Units_Array[0];
		}
		else
		{

			CStringArray temparray;

			if ((m_Variable_data.at(i).range < 23) && (m_Variable_data.at(i).range != 0))
				temp_units = Digital_Units_Array[m_Variable_data.at(i).range];
			else if ((m_Variable_data.at(i).range >= 23) && (m_Variable_data.at(i).range <= 30))
			{
				if (receive_customer_unit)
					temp_units = temp_unit_no_index[m_Variable_data.at(i).range - 23];
			}
			else
			{
				temp_units = Digital_Units_Array[0];

			}

			SplitCStringA(temparray, temp_units, _T("/"));
			if ((temparray.GetSize() == 2))
			{
				if (m_Variable_data.at(i).control == 0)
					temp_value = temparray.GetAt(0);
				else
					temp_value = temparray.GetAt(1);

			}

		}

	}
	else
	{
		if (m_Variable_data.at(i).range == 20)	//Èç¹ûÊÇÊ±¼ä;
		{
			temp_units = Variable_Analog_Units_Array[m_Variable_data.at(i).range];
			char temp_char[50];
			int time_seconds = m_Variable_data.at(i).value / 1000;
			intervaltotextfull(temp_char, time_seconds, 0, 0);
			CString temp_11;
			MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1,
				temp_11.GetBuffer(MAX_PATH), MAX_PATH);
			temp_11.ReleaseBuffer();
			temp_value = temp_11;

		}

		else if (m_Variable_data.at(i).range < sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0]))
		{
			temp_units = Variable_Analog_Units_Array[m_Variable_data.at(i).range];

			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.3f"), temp_float_value);
			temp_value = cstemp_value;


		}
		else if ((m_Variable_data.at(i).range >= 34) && (m_Variable_data.at(i).range <= 38))
		{
			temp_units = Analog_Variable_Units[m_Variable_data.at(i).range - 34];
			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.3f"), temp_float_value);
			temp_value = cstemp_value;
		}
		else
		{
			temp_units = Variable_Analog_Units_Array[0];

			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.3f"), temp_float_value);
			temp_value = cstemp_value;
		}

	}

	CString temp_des2;
	MultiByteToWideChar(CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label) + 1,
		temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
	temp_des2.ReleaseBuffer();
	temp_des = temp_des2;

	strSql.Format(_T("Insert INTO VariablesTable(SerialNumber,var_panel,var_index,var_am,var_value,var_units,var_label) Values(%u,'%s','%s','%s','%s','%s','%s' )"),
		nserialnumber,
		strPanel,
		Strindex,
		temp_am,
		temp_value,
		temp_units,
		temp_des
	);

	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


	SqliteDBBuilding.closedb();


}

