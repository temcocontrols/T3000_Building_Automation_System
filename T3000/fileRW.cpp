#include "stdafx.h"


#include "fileRW.h"
#include "stdlib.h"
#include "malloc.h"
#include "stdarg.h"
#include "schedule.h"
#include "global_variable_extern.h"
#include "globle_function.h"

#include "define.h"
//#include "Weekly_Routines.h"

//#define TSTAT26_VAR_NUM 75
//#define TSTAT26_VAR_NUM 80
//#define TSTAT26_VAR_NUM 102



#define TSTAT25_VAR_NUM 43
#define TSTAT24_VAR_NUM 30
#define NET_WORK_CONTROLLER_NUM 24
#define TSTAT26_VAR_NUM 112

#define NET_WORK_DEFFERENT_TSTAT_FILE "NET WORK\n"
using namespace std;

int tstat24_register_var[TSTAT24_VAR_NUM]={	118,121,128,109,110,	111,112,113,114,115,
											116,117,119,120,122,	123,124,125,126,127,
											186,187,131,132,133,	135,136,137,182,183};
int tstat25_register_var[TSTAT25_VAR_NUM]={	118,121,185,128,111,	112,114,115,119,120,
											122,123,124,125,126,	127,129,186,187,131,
											132,133,135,136,137,	182,183,202,203,201,
											188,189,190,204,205,	206,207,208,209,210,
											211,213,214};
int tstat26_register_var[TSTAT26_VAR_NUM]={	118,121,185,128,111,	112,114,115,119,120,
											122,123,124,125,126,	127,129,186,187,131,
											132,133,135,136,137,	182,183,202,203,201,
											188,189,190,204,205,	206,207,208,209,210,
											211,213,214,
											241,242,243,244,245,	246,247,248,249,250,
											251,252,253,
											268,269,270,271,272,	273,274,275,276,277,
											286,287,288,289,290,	291,292,285,293,
											301,302,303,309,310,    328,329,330,331,332,
											327,333,319,320,321,    322,323,325,334,335,
											336,337,338,339,		327,283,284,
											341,342,343,344,345,346,347,
                                            298,299,300};

int net_work_controller[NET_WORK_CONTROLLER_NUM]={	106,107,108,109,110,	111,112,113,114,115,
													116,117,118,119,120,	121,122,123,124,125,
													126,127,129,130};

/*
int tstat29_register_var[TSTAT29_VAR_NUM]={	118,121,185,128,111,	112,114,115,119,120,
											122,123,124,125,126,	127,129,186,187,131,
											132,133,135,136,137,	182,183,202,203,201,
											188,189,190,204,205,	206,207,208,209,210,
											211,213,214,
											241,242,243,244,245,	246,247,248,249,250,
											251,252,253,
											268,269,270,271,272,	273,274,275,276,277,
											286,287,288,289,290,	291,292,285,293,
											301,302,303,309,310,	314,315,316,317,318,
											319,320,321,322,323,	324,325,326,327,328,
											329,330,331,332,333,     334,335,};
*/

_TCHAR * NET_WORK_CONTROLLER[] = {                  //attention:该数组中的值，不要有完全包含的出现
						_T("ip Mode:"),		//0
						_T("Ip Address:"),		//1
						_T("Subnet Address:"),	//2	
						_T("Gateway Address:"),	//3	
						_T("Host Mode:"),		//4
						_T("Listenning Port:"),		//5
						_T("Inactivity Timeout:"),		//6
						_T("Destination Ip Address:"),//7		
						_T("Destination Port:"),		//8
						_T("Cyclic Connection Interval:"),//9		
						_T("Inter Character Timeout:"),		//10
						_T("Factory Default:")		//11
						};
_TCHAR * temp_heating[]={
	_T("Heating1"),
	_T("Heating2"),
	_T("Heating3"),
	_T("Heating4"),
	_T("Heating5"),
	_T("Heating6")
};

_TCHAR * temp_cooling[]={
	_T("Cooling1"),
	_T("Cooling2"),
	_T("Cooling3"),
	_T("Cooling4"),
	_T("Cooling5"),
	_T("Cooling6")
};
CString m_25,m_26;
_TCHAR * FAN_CONST[] = {
	_T("FAN OFF"),					//0
	_T("FAN ON"),					//1
	_T("FAN LOWER"),				//2
	_T("FAN MIDDLE"),				//3
	_T("FAN HIGH"),					//4
	_T("FAN AUTO")				//5
} ;

_TCHAR * RELAY_CONST[] = {
	_T("RELAY 1 : "),
	_T("RELAY 2 : "),
	_T("RELAY 3 : "),
	_T("RELAY 4 : "),
	_T("RELAY 5 : ")
};

_TCHAR * VALVE_CONST[] = {
	_T("VALVE 1 : "),
	_T("VALVE 2 : ")
};

_TCHAR * DELAY_CONST[] = {
	_T("1->0 : "),
	_T("0->1 : ")
};
_TCHAR * LOOKUPTABLES[] = {
	_T("Lookup Table 1:"),
	_T("Lookup Table 2:")
};

_TCHAR * STATE_DELAY_TIMES_CONST[] = {
	_T("COAST_DELAY_TIMES : "),
	_T("COOL_DELAY_TIMES  : "),
	_T("HEAT_DELAY_TIMES  : "),
};
_TCHAR * TSTATVAR_CONST_24[] = {                  //attention:该数组中的值，不要有完全包含的出现
						_T("SEQUENCE"),		
						_T("DEGC_OR_F"),                   
						_T("KEYPAD_SELECT"),               
						_T("TEMP CHIP CALIBRATION"),        
						_T("EXT I CALIBRATION"),       
						_T("TEMP_SELECT"),                 
						_T("DAC_OFFSET"),     
						_T("DELAY_TIME"),
						_T("COOLING_P_TERM"),		          
						_T("COOLING_I_TERM"),

						_T("HEATING_P_TERM"),
						_T("HEATING_I_TERM"),
						_T("DAY_COOLING_DEADBAND"),        
						_T("DAY_HEATING_DEADBAND"),        
						_T("NUMBER_OF_FAN_SPEEDS"),                    
						_T("NIGHT_HEATING_DEADBAND"),      
						_T("NIGHT_COOLING_DEADBAND"),      
						_T("APPLICATION"),                 
						_T("POWERUP_SETPOINT"),            
						_T("POWERUP_ON_OFF"),              

						_T("OUTPUT1_SCALE"),               
						_T("OUTPUT2_SCALE"),               
						_T("SETPOINT_HI"),                 
						_T("SETPOINT_LO"),                 
						_T("MENU_LOCK"),                   

						_T("DAY_COOLING_SETPOINT"),        
						_T("DAY_HEATING_SETPOINT"),        
						_T("CURRENT_FAN_SPEED"),                   
						_T("NIGHT_HEATING_SETPOINT"),      
						_T("NIGHT_COOLING_SETPOINT")
						};

_TCHAR * TSTATVAR_CONST_25[] = {                  //attention:该数组中的值，不要有完全包含的出现
						_T("SEQUENCE"),		
						_T("DEGC_OR_F"),                   
						_T("baudrate"),                    
						_T("KEYPAD_SELECT"),  //128             
						_T("TEMP_SELECT"),                 

						_T("DAC_OFFSET") ,     
						_T("COOLING_P_TERM"),		          
						_T("COOLING_I_TERM"),
						_T("DAY_COOLING_DEADBAND"),        
						_T("DAY_HEATING_DEADBAND"),        

						_T("NUMBER_OF_FAN_SPEEDS"),                    
						_T("NIGHT_HEATING_DEADBAND"),      
						_T("NIGHT_COOLING_DEADBAND"),      
						_T("APPLICATION"),                 
						_T("POWERUP_SETPOINT"),            

						_T("POWERUP_ON_OFF"),              
						_T("AUTO_ONLY"),                   
						_T("OUTPUT1_SCALE"),               
						_T("OUTPUT2_SCALE"),               
						_T("SETPOINT_HI"),                 

						_T("SETPOINT_LO"),                 
						_T("MENU_LOCK"),                   
						_T("DAY_COOLING_SETPOINT"),        
						_T("DAY_HEATING_SETPOINT"),        
						_T("CURRENT_FAN_SPEED"),                   

						_T("NIGHT_HEATING_SETPOINT"),      
						_T("NIGHT_COOLING_SETPOINT"),      
						_T("Changeover_Delay"),            
						_T("Display"),                     
						_T("Cycling_Delay"),               

						_T("ANALOG_IN1_RANGE"),            
						_T("ANALOG_IN2_RANGE"),            
						_T("DIGITAL_IN1"),                 
						_T("LED1_CONTROL"),                
						_T("LED2_CONTROL"),               

						_T("LED3_CONTROL"),                
						_T("LED4_CONTROL"),                
						_T("LED5_CONTROL"),                
						_T("LED6_CONTROL"),                
						_T("LED7_CONTROL"),

						_T("OVERIDE_TIMER"),//211
						_T("FILTER"),//213
						_T("HEAT_COOL_CONFIG")//214
						};
_TCHAR * TSTATVAR_CONST_26[] = {                  //attention:该数组中的值，不要有完全包含的出现
						_T("SEQUENCE"),		
						_T("DEGC_OR_F"),                   
						_T("baudrate"),                    
						_T("KEYPAD_SELECT"),  //128             
						_T("TEMP_SELECT"),                 

						_T("DAC_OFFSET"),     
						_T("COOLING_P_TERM"),		          
						_T("COOLING_I_TERM"),
						_T("DAY_COOLING_DEADBAND"),        
						_T("DAY_HEATING_DEADBAND"),        

						_T("NUMBER_OF_FAN_SPEEDS"),                    
						_T("NIGHT_HEATING_DEADBAND"),      
						_T("NIGHT_COOLING_DEADBAND"),      
						_T("APPLICATION"),                 
						_T("POWERUP_SETPOINT"),            

						_T("POWERUP_ON_OFF"),              
					//	_T("OUTPUT1_SCALE"),               
					//	_T("OUTPUT1_SCALE"),               
						_T("AUTO_ONLY"),                   
						_T("OUTPUT1_SCALE"),               
						_T("OUTPUT2_SCALE"),               
						_T("SETPOINT_HI"),                 

						_T("SETPOINT_LO"),                 
						_T("MENU_LOCK"),                   
						_T("DAY_COOLING_SETPOINT"),        
						_T("DAY_HEATING_SETPOINT"),        
						_T("CURRENT_FAN_SPEED"),                   

						_T("NIGHT_HEATING_SETPOINT"),      
						_T("NIGHT_COOLING_SETPOINT"),      
						_T("Changeover_Delay"),            
						_T("Display"),                     
						_T("Cycling_Delay"),               

						_T("ANALOG_IN1_RANGE"),            
						_T("ANALOG_IN2_RANGE"),            
						_T("DIGITAL_IN1"),                 
						_T("LED1_CONTROL"),                
						_T("LED2_CONTROL"),               

						_T("LED3_CONTROL"),                
						_T("LED4_CONTROL"),                
						_T("LED5_CONTROL"),                
						_T("LED6_CONTROL"),                
						_T("LED7_CONTROL"),

						_T("OVERIDE_TIMER"),//211
						_T("FILTER"),//213
						_T("HEAT_COOL_CONFIG"),//214

						_T("Universal PID input select"),
						_T("Universal PID upper deadband"),
						_T("Universal PID lower deadband"),
						_T("Universal PID pterm"),
						_T("Universal PID iterm"),
						_T("Universal PID setpoint"),

						_T("Output 1 PID Control"),
						_T("Output 2 PID Control"),
						_T("Output 3 PID Control"),
						_T("Output 4 PID Control"),
						_T("Output 5 PID Control"),
						_T("Output 6 PID Control"),
						_T("Output 7 PID Control"),

						_T("Heating Stages in Universal Table"),
						_T("Cooling Stages in Universal Table"),
						_T("Universal PID"),
						_T("PID1 Units hi"),
						_T("PID1 Units lo"),

						_T("PID2 Units hi"),
						_T("PID2 Units lo"),
						_T("Universal Night Setpoint"),
						_T("Heating Stages in Original Table"),
						_T("Cooling Stages in Original Table"),

						_T("Interlock Output1"),
						_T("Interlock Output2"),
						_T("Interlock Output3"),
						_T("Interlock Output4"),
						_T("Interlock Output5"),

						_T("Interlock Output6"),
						_T("Interlock Output7"),
						_T("Value Percent"),
						_T("Setpoint Increase Step"),
						_T("Time On"),
						_T("Time Off"),
						_T("Units"),
						_T("Input Auto or Manual"),
						_T("Output Auto or Manual"),

						//320:
						_T("Rotation Flag"),
						_T("Rotation out2"),
						_T("Rotation out3"),
						_T("Rotation out4"),
						_T("Rotation out5"),

						_T("Timer Select"),
						_T("Timer Left"),
						_T("Min Address"),
						_T("Max Address"),
						_T("Fan Button"),
						_T( "Mode Button"),
						_T("Hold"),
						_T("Util"),
						_T("Fun1"),
						_T("Fun2"),
						_T("Fun3"),
						_T("Fun4"),
						_T("Default Setpoint "),
						_T("Mode"),
						_T("PWM Timer"),
						_T("PWM Out1"),
						_T("PWM Out2"),
						_T("Coast"),
						_T("Cool1"),
						_T("Cool2"),
						_T("Cool3"),
						_T("Heat1"),
						_T("Heat2"),
						_T("Heat3"),
						_T("Input Fun1"),
						_T("Input Fun2"),
						_T("Input Fun3"),
						};
_TCHAR *STRINGCONST[] ={_T("Off"),_T("On"),_T("Close"),_T("Open"),_T("0-100"),_T("50-100"),_T("0-50")};

bool string2digital(CString str,unsigned char & val ,bool & type )   // type false = digital ,true = analog
{
	unsigned char SN = 255;
	for(int i=0;i<7;i++)
//	for(int i=0;i<5;i++)
	{
		if(!str.CompareNoCase(STRINGCONST[i]))        // equal
		{
			SN = i ;
			break;
		}
	}
	switch(SN)
	{/*
		case 0 :
			val = 0 ;
		break;
		case 1 :
			val = 1 ;
		break;
		case 2 :
			val = 0 ;
		break;
		case 3 :
			val = 3 ;
		break;
		case 4 :
			val = 1 ;
		break;

		case 5:
			val = 2 ;
		break;
		case 6:
			val = 4 ;///bit 7 or 8 and defferent from others
		break;

		default:
		break;
		*/
		case 0 :
			val = 0 ;
		break;
		case 1 :
			val = 1 ;
		break;
		case 2 :
			val = 0 ;
		break;
		case 3 :
			val = 1 ;
		break;
		case 4 :
			val = 2 ;
		break;

		case 5:
			val =3 ;
		break;
		case 6:
			val = 4 ;///bit 7 or 8 and defferent from others
		break;

		default:
		break;

	}
	if(SN == 255)
		return false;
	else
	{
		if(SN <2)
			type = false;
		else
			type = true;
		return true;
	}
}

bool digital2string(int val,CString & str,unsigned char type)
{
	if(type == FAN)
	{
		switch(val)
		{
			case 0 :
				str = STRINGCONST[0];
				return true;
			case 1:
				str = STRINGCONST[1];
				return true;
			default://2.5.0.99
				str = STRINGCONST[0];
				return true;
			break;
		}
	}
	else if(type == VALVE)
	{
		switch(val)
		{
			/*
			case 0 :
				str = STRINGCONST[2];
				return true;
			case 1:
				str = STRINGCONST[4];
				return true;
			case 2:
				str = STRINGCONST[5];
				return true;
			case 3:
				str = STRINGCONST[3];
				return true;
			case 4:
				str = STRINGCONST[6];
				return true;
			default:
			break;	*/

			case 0 :
				str = STRINGCONST[2];
				return true;
			case 1:
				str = STRINGCONST[3];
				return true;
			case 2:
				str = STRINGCONST[4];
				return true;
			case 3:
				str = STRINGCONST[5];
				return true;
			case 4:
				str = STRINGCONST[6];
				return true;
			default:
			break;
		}
	}
	else if(type == HEATPUMP)
	{
		switch(val)
		{
			case 0 :
				str = STRINGCONST[2];
				return true;
			case 1:
				str = STRINGCONST[3];
				return true;
			default:
			break;	

		}
	}
	return false;
}




void WriteSerialNumber(wofstream & out)
{

	
		unsigned long serial = read_one(g_tstat_id,0 ) +
							read_one(g_tstat_id,1) * 256 +
							read_one(g_tstat_id,2) * 65536 +           // 256 * 256 
							read_one(g_tstat_id , 3) * 16777216 ;// 256 *256 *256
		//write_to_file_a_line(out,"%-30s%ld","serialnumber:",serial);
		_Twrite_to_file_a_line(out,_T("%-30s%ld"),_T("serialnumber:"),serial);
		

		
}
void WriteAddress(wofstream & out) 
{				
	//write_to_file_a_line(out,"%-30s%ld","address:",read_one(g_tstat_id,6));	
	_Twrite_to_file_a_line(out,_T("%-30s%ld"),_T("address:"),read_one(g_tstat_id,6));	
}
void write_to_file_a_line(wofstream & out,const char * format, ... )
{
	/*
	va_list marker;
	va_start( marker, format );
	char buf[2048];	
	vsprintf( buf, format, marker );
	va_end(marker);
	out<< buf << endl 
	*/
}
void _Twrite_to_file_a_line(wofstream & out,const TCHAR * format, ... )
{
	va_list marker;
	va_start( marker, format );		
	
	TCHAR buf[2048];	
	vswprintf( buf,2048, format, marker );
	va_end(marker);
	out<< buf << endl ;
}

int get_real_fan_address(int fan_speed,int fan_place,int place)
{
	int return_value=0;
	int fan_register[5]={138,145,152,159,166};//*************************************
	switch(fan_speed)
	{
	case 0: if(fan_place==0)
				return_value=fan_register[0];
			else if(fan_place==1)
				return_value=fan_register[4];
			break;
	case 1:	if(fan_place==0)
			   return_value=fan_register[0];
			else if(fan_place==1)
				return_value=fan_register[1];
			else if(fan_place==2)
				return_value=fan_register[4];
			break;
	case 2:	if(fan_place==0)
			   return_value=fan_register[0];
			else if(fan_place==1)
				return_value=fan_register[1];
			else if(fan_place==2)
				return_value=fan_register[3];
			else if(fan_place==3)
				return_value=fan_register[4];
			break;
	case 3:	if(fan_place==0)
			   return_value=fan_register[0];
			else if(fan_place==1)
				return_value=fan_register[1];
			else if(fan_place==2)
				return_value=fan_register[2];
			else if(fan_place==3)
				return_value=fan_register[3];
			else if(fan_place==4)
				return_value=fan_register[4];
			break;
	default:if(fan_place==0)
			   return_value=fan_register[0];
			else if(fan_place==1)
				return_value=fan_register[1];
			else if(fan_place==2)
				return_value=fan_register[2];
			else if(fan_place==3)
				return_value=fan_register[3];
			else if(fan_place==4)
				return_value=fan_register[4];
			break;
	}
	return_value+=place;
	return return_value;
}
void fan_write(wofstream & out,int m_25_heat_stages,int m_25_cool_stages)
{//write the fan register
	//used by save2file functiojn
	int fan_speed=read_one(g_tstat_id,122);//1,2,3
	if(read_one(g_tstat_id,129)==1)
		fan_speed=0;
	_TCHAR * temp_fan_const[5];
	int fan_number;
	switch(fan_speed)
	{
		case 0:fan_number=2;temp_fan_const[0]=FAN_CONST[0];temp_fan_const[1]=FAN_CONST[5];break;
		case 1:fan_number=3;temp_fan_const[0]=FAN_CONST[0];temp_fan_const[1]=FAN_CONST[1];temp_fan_const[2]=FAN_CONST[5];break;
		case 2:fan_number=4;temp_fan_const[0]=FAN_CONST[0];temp_fan_const[1]=FAN_CONST[2];temp_fan_const[2]=FAN_CONST[4];temp_fan_const[3]=FAN_CONST[5];break;
		case 3:fan_number=5;temp_fan_const[0]=FAN_CONST[0];temp_fan_const[1]=FAN_CONST[2];temp_fan_const[2]=FAN_CONST[3];temp_fan_const[3]=FAN_CONST[4];temp_fan_const[4]=FAN_CONST[5];break;
		default:fan_number=5;temp_fan_const[0]=FAN_CONST[0];temp_fan_const[1]=FAN_CONST[2];temp_fan_const[2]=FAN_CONST[3];temp_fan_const[3]=FAN_CONST[4];temp_fan_const[4]=FAN_CONST[5];break;
	}
	for(int j=0;j<fan_number;j++)
	{
		_Twrite_to_file_a_line(out,_T("//   %s"),temp_fan_const[j]);//j
		//write_to_file_a_line(out,"//        HEATING3 HEATING2 HEATING1 CONAST   COOLING1 COOLING2 COOLING3");
		_Twrite_to_file_a_line(out,m_25.GetString());
		int fan_value[7];
		int pos=0;
		for(int col = 0 ;col <(m_25_heat_stages+m_25_cool_stages+1);col++)
		{//get register value
			if(col < m_25_heat_stages)
				pos = m_25_heat_stages+m_25_cool_stages - col ;
			else
				pos = col - m_25_heat_stages;
			fan_value[col] = read_one(g_tstat_id,get_real_fan_address(fan_speed,j,pos));//j
		}
		CString str,str1;
		for(int row = 0 ;row <5 ;row++)
		{//write the value to file
			str = "";
			str+=RELAY_CONST[row];
			for(int col =0 ;col <(m_25_heat_stages+m_25_cool_stages+1) ;col++)
			{
				if(digital2string((fan_value[col] >> row ) & 0x01,str1,FAN))
				{
					str+= str1;
					for(int i= str1.GetLength();i< 9 ;i++)
					str+= " ";
				}
				else 
					str += "         ";
			}
			_Twrite_to_file_a_line(out,str.GetString());
		}
		_Twrite_to_file_a_line(out,_T("//   FAN END"));	
	}
}
void universal_relay_write(wofstream & out,int m_26_heat_stages,int m_26_cool_stages)//UNIVERSAL RELAY OUTPUTS vlaue
{
		_Twrite_to_file_a_line(out,_T("//   UNIVERSAL RELAY OUTPUTS"));//j
		_Twrite_to_file_a_line(out,m_26.GetString());
		int fan_value[7];
		int pos=0;
		for(int col = 0 ;col <(m_26_heat_stages+m_26_cool_stages+1);col++)
		{//get register value
			if(col < m_26_heat_stages)
				pos = m_26_heat_stages+m_26_cool_stages - col ;
			else
				pos = col - m_26_heat_stages;
			fan_value[col] = read_one(g_tstat_id,254+pos);//j
		}
		CString str,str1;
		for(int row = 0 ;row <5 ;row++)
		{//write the value to file
			str = "";
			str+=RELAY_CONST[row];
			for(int col =0 ;col <(m_26_heat_stages+m_26_cool_stages+1) ;col++)
			{
				if(digital2string((fan_value[col] >> row ) & 0x01,str1,FAN))
				{
					str+= str1;
					for(int i= str1.GetLength();i< 9 ;i++)
					str+= _T(" ");
				}
				else 
					str += _T("         ");
			}
			_Twrite_to_file_a_line(out,str.GetString());
		}
		_Twrite_to_file_a_line(out,_T("//   UNIVERSAL FAN END"));	
}
void value_setting_write(wofstream & out,int m_25_heat_stages,int m_25_cool_stages)
{//write the value register
	//used by save2file functiojn
	int Array[7];
	CString str,str1;
	int pos=0;
	for(int col = 1 ;col <=(m_25_heat_stages+m_25_cool_stages+1);col++)
	{//get register value
		if(col < (m_25_heat_stages+1))
			pos = (m_25_heat_stages+m_25_cool_stages+1) - col ;
		else
			pos = col - (m_25_heat_stages+1);
		Array[col-1] = read_one(g_tstat_id,173+pos);//j
	}
	_Twrite_to_file_a_line(out,_T("//   VALVE SETTING"));
	_Twrite_to_file_a_line(out,m_25.GetString());
	for(int row = 0 ;row <2 ;row++)
	{
		str = "";
		str+=VALVE_CONST[row];
		for(int col =0 ;col <(m_25_heat_stages+m_25_cool_stages+1) ;col++)
		{
			bool t;
			if(Array[col] & 64 && row==0)
			{
				Array[col]-=64;
				t=digital2string(4,str1,VALVE);
			}
			else if(Array[col] & 128 && row==1)
			{
				Array[col]-=128;
				t=digital2string(4,str1,VALVE);
			}
			else 
				t=digital2string((Array[col] >> (row * 2)) & 0x03,str1,VALVE);
			if(t==true)
			{
				str+= str1;
				for(int i= str1.GetLength();i< 9 ;i++)
				str+= _T(" ");
			}
			else 
				str += _T("         ");
		}
		_Twrite_to_file_a_line(out,str.GetString());
	}
	_Twrite_to_file_a_line(out,_T("//   VALVE END"));	



//FANOFF  VALVE SETTING:added 20100505

	for(int col = 1 ;col <=(m_25_heat_stages+m_25_cool_stages+1);col++)
	{//get register value
		if(col < (m_25_heat_stages+1))
			pos = (m_25_heat_stages+m_25_cool_stages+1) - col ;
		else
			pos = col - (m_25_heat_stages+1);
		Array[col-1] = read_one(g_tstat_id,351+pos);//j
	}
	_Twrite_to_file_a_line(out,_T("// FANOFF  VALVE SETTING"));
	_Twrite_to_file_a_line(out,m_25.GetString());
	for(int row = 0 ;row <2 ;row++)
	{
		str = "";
		str+=VALVE_CONST[row];
		for(int col =0 ;col <(m_25_heat_stages+m_25_cool_stages+1) ;col++)
		{
			bool t;
			if(Array[col] & 64 && row==0)
			{
				Array[col]-=64;
				t=digital2string(4,str1,VALVE);
			}
			else if(Array[col] & 128 && row==1)
			{
				Array[col]-=128;
				t=digital2string(4,str1,VALVE);
			}
			else 
				t=digital2string((Array[col] >> (row * 2)) & 0x03,str1,VALVE);
			if(t==true)
			{
				str+= str1;
				for(int i= str1.GetLength();i< 9 ;i++)
				str+= _T(" ");
			}
			else 
				str += _T("         ");
		}
		_Twrite_to_file_a_line(out,str.GetString());
	}
	_Twrite_to_file_a_line(out,_T("// FANOFF VALVE END"));	
	

}
void universal_value_setting_write(wofstream & out,int m_26_heat_stages,int m_26_cool_stages)
{
	int Array[7];
	CString str,str1;
	int pos=0;
	for(int col = 1 ;col <=(m_26_heat_stages+m_26_cool_stages+1);col++)
	{//get register value
		if(col < m_26_heat_stages+1)
			pos = (m_26_heat_stages+m_26_cool_stages+1) - col ;
		else
			pos = col - (m_26_heat_stages+1);
		Array[col-1] = read_one(g_tstat_id,261+pos);//j
	}
	_Twrite_to_file_a_line(out,_T("//   UNIVERSAL VALVE"));
	_Twrite_to_file_a_line(out,m_26.GetString());
	for(int row = 0 ;row <2 ;row++)
	{
		str = _T("");
		str+=VALVE_CONST[row];
		for(int col =0 ;col <(m_26_heat_stages+m_26_cool_stages+1) ;col++)
		{
			if(digital2string((Array[col] >> (row * 2)) & 0x03,str1,VALVE))
			{
				str+= str1;
				for(int i= str1.GetLength();i< 9 ;i++)
				str+= _T(" ");
			}
			else 
				str +=_T("         ");
		}
		_Twrite_to_file_a_line(out,str.GetString());
	}
	_Twrite_to_file_a_line(out,_T("//   VALVE END"));	
}
void delay_time_write(wofstream & out)
{//write the delay
	//used by save2file functiojn
	int Array[10],row,col;
	CString str,str1;
	for(int i = 0 ;i <10;i++)
	{
		Array[i] = read_one(g_tstat_id,191+i);
	}
	_Twrite_to_file_a_line(out,_T("//   DELAY TIMES"));
	_Twrite_to_file_a_line(out,_T("//     OUTPUT1 OUTPUT2 OUTPUT3 OUTPUT4 OUTPUT5"));
	for(row=0;row<2;row++)
	{
		str = _T("");
		str+=(DELAY_CONST[row]);
		for(col =0;col<5;col++)
		{
			//str1.Format("%-8d",Array[2+row*5+col]);//because michel changed ,OUTPUT_SCALE_OFF
			//because the xiao lei,position = OUTPUT_DELAY_DECREASE
			//because Michel need,position = OUTPUT_DELAY_OFF,
			//OUTPUT_DELAY_OFF-OUTPUT_DELAY_DECREASE=4
			str1.Format(_T("%-8d"),Array[row*5+col]);//4
			str+= str1;
		}
		_Twrite_to_file_a_line(out,str.GetString());
	}
	_Twrite_to_file_a_line(out,_T("//   DELAY END"));
}
void lookup_table_write(wofstream & out)
{
	float version=get_tstat_version(g_tstat_id);
	if(version<CUSTOM_TABLE_FLOAT_VERSION)
	{
	int Array[23];
	CString str,str1;
	for(int i = 0 ;i <22;i++)
		Array[i] = read_one(g_tstat_id,219+i);
	_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE"));
	_Twrite_to_file_a_line(out,_T("//			0.0V	0.5V	1.0V	1.5V	2.0V	2.5V	3.0V	3.5V	4.0V	4.5V	5.0V"));
	str.Format(_T("Lookup Table 1:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
		Array[0],Array[1],Array[2],Array[3],Array[4],Array[5],
		Array[6],Array[7],Array[8],Array[9],Array[10]);
	_Twrite_to_file_a_line(out,str);
	str.Format(_T("Lookup Table 2:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
		Array[11],Array[12],Array[13],Array[14],Array[15],Array[16],
		Array[17],Array[18],Array[19],Array[20],Array[21]);
	_Twrite_to_file_a_line(out,str);
	_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE END"));
	}
	if(version>=CUSTOM_TABLE_FLOAT_VERSION)
	{
		{
		short slider1_low=multi_register_value[MODBUS_TABLE1_ZERO];
		short slider1_high=multi_register_value[MODBUS_TABLE1_ZERO_HI];
		int m_slider1_i=slider1_high*65536+slider1_low;
		short slider2_low=multi_register_value[MODBUS_TABLE1_HALFONE];
		short slider2_high=multi_register_value[MODBUS_TABLE1_HALFONE_HI];
		int m_slider2_i=(slider2_high*65536+slider2_low);
		short slider3_low=multi_register_value[MODBUS_TABLE1_ONE];
		short slider3_high=multi_register_value[MODBUS_TABLE1_ONE_HI];
		int m_slider3_i=(slider3_high*65536+slider3_low);	
		short slider4_low=multi_register_value[MODBUS_TABLE1_HALFTWO];
		short slider4_high=multi_register_value[MODBUS_TABLE1_HALFTWO_HI];
		int m_slider4_i=(slider4_high*65536+slider4_low);
		short slider5_low=multi_register_value[MODBUS_TABLE1_TWO];
		short slider5_high=multi_register_value[MODBUS_TABLE1_TWO_HI];
		int m_slider5_i=(slider5_high*65536+slider5_low);
		short slider6_low=multi_register_value[MODBUS_TABLE1_HALFTHREE];
		short slider6_high=multi_register_value[MODBUS_TABLE1_HALFTHREE_HI];
		int m_slider6_i=(slider6_high*65536+slider6_low);
		short slider7_low=multi_register_value[MODBUS_TABLE1_THREE];
		short slider7_high=multi_register_value[MODBUS_TABLE1_THREE_HI];
		int m_slider7_i=(slider7_high*65536+slider7_low);
		short slider8_low=multi_register_value[MODBUS_TABLE1_HALFFOUR];
		short slider8_high=multi_register_value[MODBUS_TABLE1_HALFFOUR_HI];
		int m_slider8_i=(slider8_high*65536+slider8_low);
		short slider9_low=multi_register_value[MODBUS_TABLE1_FOUR];
		short slider9_high=multi_register_value[MODBUS_TABLE1_FOUR_HI];
		int m_slider9_i=(slider9_high*65536+slider9_low);
		short slider10_low=multi_register_value[MODBUS_TABLE1_HALFFIVE];
		short slider10_high=multi_register_value[MODBUS_TABLE1_HALFFIVE_HI];
		int m_slider10_i=(slider10_high*65536+slider10_low);
		short slider11_low=multi_register_value[MODBUS_TABLE1_FIVE];
		short slider11_high=multi_register_value[MODBUS_TABLE1_FIVE_HI];
		int m_slider11_i=(slider11_high*65536+slider11_low);
	_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE"));
	_Twrite_to_file_a_line(out,_T("//			0.0V	0.5V	1.0V	1.5V	2.0V	2.5V	3.0V	3.5V	4.0V	4.5V	5.0V"));
		CString str;
		str.Format(_T("Lookup Table 1:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
		m_slider1_i,m_slider2_i,m_slider3_i,m_slider4_i,m_slider5_i,m_slider6_i,
		m_slider7_i,m_slider8_i,m_slider9_i,m_slider10_i,m_slider11_i);
		_Twrite_to_file_a_line(out,str);
		}
		{
		short slider1_low=multi_register_value[MODBUS_TABLE2_ZERO];
		short slider1_high=multi_register_value[MODBUS_TABLE2_ZERO_HI];
		int m_slider1_i=slider1_high*65536+slider1_low;

		short slider2_low=multi_register_value[MODBUS_TABLE2_HALFONE];
		short slider2_high=multi_register_value[MODBUS_TABLE2_HALFONE_HI];
		int m_slider2_i=(slider2_high*65536+slider2_low);

		short slider3_low=multi_register_value[MODBUS_TABLE2_ONE];
		short slider3_high=multi_register_value[MODBUS_TABLE2_ONE_HI];
		int m_slider3_i=(slider3_high*65536+slider3_low);

		short slider4_low=multi_register_value[MODBUS_TABLE2_HALFTWO];
		short slider4_high=multi_register_value[MODBUS_TABLE2_HALFTWO_HI];
		int m_slider4_i=(slider4_high*65536+slider4_low);

		short slider5_low=multi_register_value[MODBUS_TABLE2_TWO];
		short slider5_high=multi_register_value[MODBUS_TABLE2_TWO_HI];
		int m_slider5_i=(slider5_high*65536+slider5_low);

		short slider6_low=multi_register_value[MODBUS_TABLE2_HALFTHREE];
		short slider6_high=multi_register_value[MODBUS_TABLE2_HALFTHREE_HI];
		int m_slider6_i=(slider6_high*65536+slider6_low);

		short slider7_low=multi_register_value[MODBUS_TABLE2_THREE];
		short slider7_high=multi_register_value[MODBUS_TABLE2_THREE_HI];
		int m_slider7_i=(slider7_high*65536+slider7_low);

		short slider8_low=multi_register_value[MODBUS_TABLE2_HALFFOUR];
		short slider8_high=multi_register_value[MODBUS_TABLE2_HALFFOUR_HI];
		int m_slider8_i=(slider8_high*65536+slider8_low);

		short slider9_low=multi_register_value[MODBUS_TABLE2_FOUR];
		short slider9_high=multi_register_value[MODBUS_TABLE2_FOUR_HI];
		int m_slider9_i=(slider9_high*65536+slider9_low);

		short slider10_low=multi_register_value[MODBUS_TABLE2_HALFFIVE];
		short slider10_high=multi_register_value[MODBUS_TABLE2_HALFFIVE_HI];
		int m_slider10_i=(slider10_high*65536+slider10_low);

		short slider11_low=multi_register_value[MODBUS_TABLE2_FIVE];
		short slider11_high=multi_register_value[MODBUS_TABLE2_FIVE_HI];
		int m_slider11_i=(slider11_high*65536+slider11_low);
		CString str;
		str.Format(_T("Lookup Table 2:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
		m_slider1_i,m_slider2_i,m_slider3_i,m_slider4_i,m_slider5_i,m_slider6_i,
		m_slider7_i,m_slider8_i,m_slider9_i,m_slider10_i,m_slider11_i);
		_Twrite_to_file_a_line(out,str);
		_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE END"));
		}
//
	/*
	int Array[23];
	CString str,str1;
	for(int i = 0 ;i <22;i++)
		Array[i] = read_one(tstat_id,219+i);
	write_to_file_a_line(out,"//   LOOKUP TABLE");
	write_to_file_a_line(out,"//			0.0V	0.5V	1.0V	1.5V	2.0V	2.5V	3.0V	3.5V	4.0V	4.5V	5.0V");
	str.Format("Lookup Table 1:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d",
		Array[0],Array[1],Array[2],Array[3],Array[4],Array[5],
		Array[6],Array[7],Array[8],Array[9],Array[10]);
	write_to_file_a_line(out,LPCSTR(str));
	str.Format("Lookup Table 2:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d",
		Array[11],Array[12],Array[13],Array[14],Array[15],Array[16],
		Array[17],Array[18],Array[19],Array[20],Array[21]);
	write_to_file_a_line(out,LPCSTR(str));
	write_to_file_a_line(out,"//   LOOKUP TABLE END");
	*/
	}
}
void var_write(wofstream & out)
{
	
	//used by save2file functiojn
	CString str1;
	float version=get_tstat_version(g_tstat_id);
		if(version<25)
		{//24
			for(int i=0;i<TSTAT24_VAR_NUM;i++)
			{			
				str1.Format(_T("%d,\t%d,\t%s"),tstat24_register_var[i],read_one(g_tstat_id,tstat24_register_var[i]),TSTATVAR_CONST_24[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
					_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
		}
		else if(version<26)
		{//25
			for(int i=0;i<TSTAT25_VAR_NUM;i++)
			{
				str1.Format(_T("%d,\t%d,\t%s"),tstat25_register_var[i],read_one(g_tstat_id,tstat25_register_var[i]),TSTATVAR_CONST_25[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
					_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
		}
		else //if(version<=29)
		{
			for(int i=0;i<TSTAT26_VAR_NUM;i++)
			{
				/*
				if(version>=28.0&&i==23)//register 135:
				{
				str1.Format("%d,\t%d,\t%s",tstat26_register_var[i],read_one(tstat_id,							tstat26_register_var[i]/10),TSTATVAR_CONST_26[i]);
				write_to_file_a_line(out,str1);
				}
				else
				{
					str1.Format("%d,\t%d,\t%s",tstat26_register_var[i],read_one(tstat_id,							tstat26_register_var[i]),TSTATVAR_CONST_26[i]);
				write_to_file_a_line(out,str1);
				if((i%5)==4)
					write_to_file_a_line(out," ");//a space line
				}
				*/
			    str1.Format(_T("%d,\t%d,\t%s"),tstat26_register_var[i],read_one(g_tstat_id,tstat26_register_var[i]),TSTATVAR_CONST_26[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
					_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
			//135:
		}
}
void Save2File(CString fn)
{
	//
	// save tstat register value to file
	int m_25_heat_stages,m_25_cool_stages,m_26_heat_stages,m_26_cool_stages;
	m_25_heat_stages=read_one(g_tstat_id,276);
	m_25_cool_stages=read_one(g_tstat_id,277);
	m_26_heat_stages=read_one(g_tstat_id,268);
	m_26_cool_stages=read_one(g_tstat_id,269);
	if(m_25_heat_stages==0 && m_25_cool_stages==0)
	{
		m_25_heat_stages=3;
		m_25_cool_stages=3;
	}
	CString str;
	//25
	str=_T("");
	m_25=_T("//	");
	for(int i=0;i<m_25_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_25_heat_stages-i-1]);
		m_25+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_25+=str;
	for(int i=0;i<m_25_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_25+=str;
	}
	//26
	str=_T("");
	m_26=_T("//	");
	for(int i=0;i<m_26_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_26_heat_stages-i-1]);
		m_26+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_26+=str;
	for(int i=0;i<m_26_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_26+=str;
	}	
	float version=get_tstat_version(g_tstat_id);
	TCHAR a[3]={'\0','\0','\0'};
	if(version >= 26)
	{
		a[0]='2';
		a[1]='6';
		a[2]='\0';
	}
	else if(version >= 25)
	{
		a[0]='2';
		a[1]='5';
		a[2]='\0';
	}
	else
	{
		a[0]='2';
		a[1]='4';
		a[2]='\0';
	}
	wofstream out;
	out.open(fn,ios_base::out) ;
	out<<"version:"<<version<<endl;
	fan_write(out,m_25_heat_stages,m_25_cool_stages);//fan value	
	_Twrite_to_file_a_line(out,_T(" "));//space
	value_setting_write(out,m_25_heat_stages,m_25_cool_stages);//value setting
	_Twrite_to_file_a_line(out,_T(" "));//space
	if(version>=25)
		delay_time_write(out);//delay time write
	if(version>=26)
	{
		_Twrite_to_file_a_line(out,_T(" "));//space
		lookup_table_write(out);//lookup table 26 only
		_Twrite_to_file_a_line(out,_T(" "));//space
		universal_relay_write(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		universal_value_setting_write(out,m_26_heat_stages,m_26_cool_stages);
	}
	_Twrite_to_file_a_line(out,_T(" "));//space
	WriteSerialNumber(out);
	WriteAddress(out);
	_Twrite_to_file_a_line(out,_T(" "));//space
	var_write(out);	
	_Twrite_to_file_a_line(out,_T("save config file Success. "));//space
	out.close();

//	if(version>=26)///26 version ,must Store Defaults
	//store as default
	//write_one(tstat_id,16,159);//////
	//close_com();//close the com port 
}
void Save2File_ForTwoFiles(TCHAR* fn)
{
	CString strTips;
	int nValue;
	//nValue=read_one(g_tstat_id,324);
//	write_one(g_tstat_id,324,0);//////
	// save tstat register value to file
	int m_25_heat_stages,m_25_cool_stages,m_26_heat_stages,m_26_cool_stages;
	m_25_heat_stages=read_one(g_tstat_id,276);
	m_25_cool_stages=read_one(g_tstat_id,277);
	m_26_heat_stages=read_one(g_tstat_id,268);
	m_26_cool_stages=read_one(g_tstat_id,269);
	if(m_25_heat_stages==0 && m_25_cool_stages==0)
	{
		m_25_heat_stages=3;
		m_25_cool_stages=3;
	}
	#if 1
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 10%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	CString str;
	//25
	str=_T("");
	m_25=_T("//	");
	for(int i=0;i<m_25_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_25_heat_stages-i-1]);
		m_25+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_25+=str;
	for(int i=0;i<m_25_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_25+=str;
	}
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 15%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	//26
	str=_T("");
	m_26=_T("//	");
	for(int i=0;i<m_26_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_26_heat_stages-i-1]);
		m_26+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_26+=str;
	for(int i=0;i<m_26_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_26+=str;
	}	
	float version=get_tstat_version(g_tstat_id);
	TCHAR a[3]={'\0','\0','\0'};
	if(version >= 26)
	{
		a[0]='2';
		a[1]='6';
		a[2]='\0';
	}
	else if(version >= 25)
	{
		a[0]='2';
		a[1]='5';
		a[2]='\0';
	}
	else
	{
		a[0]='2';
		a[1]='4';
		a[2]='\0';
	}
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 20%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	wofstream out;
	out.open(fn,ios_base::out) ;
	
	_Twrite_to_file_a_line(out,_T("Tstat Config File"));//added the header marker.

	int nModelID = read_one(g_tstat_id, 7);
	CString strProductClassName = get_product_class_name_by_model_ID(nModelID);
	strProductClassName = _T("Model : ") + strProductClassName;
	_Twrite_to_file_a_line(out, strProductClassName);//added the model
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 30%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	out<<"version:"<<version<<endl;
	fan_write(out,m_25_heat_stages,m_25_cool_stages);//fan value	
	_Twrite_to_file_a_line(out,_T(" "));//space
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 40%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	value_setting_write(out,m_25_heat_stages,m_25_cool_stages);//value setting
	write_to_file_a_line(out," ");//space
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 50%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	if(version>=25)
		delay_time_write(out);//delay time write
	if(version>=26)
	{
		_Twrite_to_file_a_line(out,_T(" "));//space
		lookup_table_write(out);//lookup table 26 only
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		strTips = _T("Config file saved 60%...");
		SetPaneString(1, strTips);
		//////////////////////////////////////////////////////////////////////////
		universal_relay_write(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		strTips = _T("Config file saved 70%...");
		SetPaneString(1, strTips);
		//////////////////////////////////////////////////////////////////////////
		universal_value_setting_write(out,m_26_heat_stages,m_26_cool_stages);
	}
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 80%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	_Twrite_to_file_a_line(out,_T(" "));//space
	WriteSerialNumber(out);
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 90%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	WriteAddress(out);
	_Twrite_to_file_a_line(out,_T(" "));//space
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 99%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	var_write(out);	
	//_Twrite_to_file_a_line(out,_T("OK!"));//space
	return;

	_Twrite_to_file_a_line(out,_T("//END CONFIG 1 ********************************// "));//space
	int nvalue=0;

	//nvalue=read_one(g_tstat_id,326);
	/*
	if(nvalue==0)
	{
		if (nValue>=0)
		{
			write_one(g_tstat_id,324,nValue);
		}
		return;
	}
		

	if(nvalue==1)
		write_one(g_tstat_id,324,1);//////	
	else
	*/
		return;

	/*******************************************************************************************************
	
	SetPaneString("Begin to save configuration file 2...");

	*******************************************************************************************************/

	m_25_heat_stages=read_one(g_tstat_id,276);
	m_25_cool_stages=read_one(g_tstat_id,277);
	m_26_heat_stages=read_one(g_tstat_id,268);
	m_26_cool_stages=read_one(g_tstat_id,269);
	if(m_25_heat_stages==0 && m_25_cool_stages==0)
	{
		m_25_heat_stages=3;
		m_25_cool_stages=3;
	}

//	CString str;
	//25
	str=_T("");
	m_25=_T("//	");
	for(int i=0;i<m_25_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_25_heat_stages-i-1]);
		m_25+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_25+=str;
	for(int i=0;i<m_25_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_25+=str;
	}

	//26
	str=_T("");
	m_26=_T("//	");
	for(int i=0;i<m_26_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_26_heat_stages-i-1]);
		m_26+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_26+=str;
	for(int i=0;i<m_26_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_26+=str;
	}	
	version=get_tstat_version(g_tstat_id);
//	a[3]={'\0','\0','\0'};
	if(version >= 26)
	{
		a[0]='2';
		a[1]='6';
		a[2]='\0';
	}
	else if(version >= 25)
	{
		a[0]='2';
		a[1]='5';
		a[2]='\0';
	}
	else
	{
		a[0]='2';
		a[1]='4';
		a[2]='\0';
	}


	fan_write(out,m_25_heat_stages,m_25_cool_stages);//fan value	
	_Twrite_to_file_a_line(out,_T(" "));//space
	value_setting_write(out,m_25_heat_stages,m_25_cool_stages);//value setting
	_Twrite_to_file_a_line(out,_T(" "));//space
	if(version>=25)
		delay_time_write(out);//delay time write
	if(version>=26)
	{
		_Twrite_to_file_a_line(out,_T(" "));//space
		lookup_table_write(out);//lookup table 26 only
		_Twrite_to_file_a_line(out,_T(" "));//space
		universal_relay_write(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		universal_value_setting_write(out,m_26_heat_stages,m_26_cool_stages);
	}
	_Twrite_to_file_a_line(out,_T(" "));//space
	WriteSerialNumber(out);
	WriteAddress(out);
	_Twrite_to_file_a_line(out,_T(" "));//space
	var_write(out);	
	out.close();

	if (nValue>=0)
	{
		write_one(g_tstat_id,324,nValue);
	}


#endif

}
void change_showing_text_variable(CString str)
{
#if 0
	CSingleLock lck3(& showing_text_mutex);
	lck3.Lock(INFINITE);
	if(lck3.IsLocked())
	{
		showing_text=str;
		lck3.Unlock();
	}
#endif
		
	critical_section.Lock();
	showing_text=str;
	critical_section.Unlock();

}

bool before_load_file_inspect(CString path_name,CString name,int i_tstat_id,bool show_message)
{
	//judge the version of hardware is right or no
	//to decide load or no
	CString file_path=path_name;
	CString file=name;
	CStdioFile read_file;
	bool return_value=true;
	float version=get_tstat_version(i_tstat_id);//get_tstat_version
	CString s_first_line=_T("");	
	if(read_file.Open(file_path,CFile::modeRead | CFile::shareDenyNone))
		read_file.ReadString(s_first_line);
	else
	{
		if(show_message==true)
			AfxMessageBox(_T("open file failure!"));
		return_value=false;
	}
	if(version >=27)
	{
		if(s_first_line.Find(_T("27"))==-1)
		{
			if(show_message==true)
			{
				s_first_line=s_first_line.Right(s_first_line.GetLength()-8);
				s_first_line=file+_T(" is for ")+s_first_line+_T(".\n");
				s_first_line=s_first_line+_T("The tstat is 27.\nLoad?");
				if(IDOK!=AfxMessageBox(s_first_line,MB_OKCANCEL))
					return_value = false;
			}
			else
				return_value = false;
		}
	}
	else if(version >=26)
	{
		if(s_first_line.Find(_T("26"))==-1)
		{
			if(show_message==true)
			{
				s_first_line=s_first_line.Right(s_first_line.GetLength()-8);
				s_first_line=file+_T(" is for ")+s_first_line+_T(".\n");
				s_first_line=s_first_line+_T("The tstat is 26.\nLoad?");
				if(IDOK!=AfxMessageBox(s_first_line,MB_OKCANCEL))
					return_value = false;
			}
			else
				return_value = false;
		}
	}
	else if(version >= 25)
	{
		if(s_first_line.Find(_T("25"))==-1)
		{
			if(show_message==true)
			{
				s_first_line=s_first_line.Right(s_first_line.GetLength()-8);
				s_first_line=file+_T(" is for ")+s_first_line+_T(".\n");
				s_first_line=s_first_line+_T("The tstat is 25.\nLoad?");
				if(IDOK!=AfxMessageBox(s_first_line,MB_OKCANCEL))
					return_value = false;
			}
			else
				return_value = false;
		}
	}
	else if(s_first_line.Find(_T("24"))==-1)
		{
			if(show_message==true)
			{
				s_first_line=s_first_line.Right(s_first_line.GetLength()-8);
				s_first_line=file+_T(" is for ")+s_first_line+_T(".\n");
				s_first_line=s_first_line+_T("The tstat is 24.\nLoad?");
				if(IDOK!=AfxMessageBox(s_first_line,MB_OKCANCEL))
					return_value = false;
			}
			else
				return_value = false;
		}
	read_file.Close();	
	return return_value;
}






///////////////////////////////////////load config file////////////////////////////
//int fan_number=0;
int get_real_number_fan_speeds(int fan_number)
{
	int return_value=0;
	switch(fan_number)
	{
		case 2: return_value=-1;break;
		case 3:	return_value=1;break;
		case 4: return_value=2;break;
		case 5:	return_value=3;break;
		default:return_value=3;break;
	}
	return return_value;
}

bool find_load_file_error(load_file_every_step temppp)
{//return value true == found ,false == no found
	int i=0;
	/*
	if(temppp.seven_step==false || temppp.first_step ==false || temppp.second_step ==false || 
		temppp.thurth_step ==false || temppp.fifth_step ==false || temppp.third_step ==false || 
		temppp.sixth_step ==false )
		return false;
	else
		return TRUE;
		*/
	if(temppp.seven_step==false)
		i++;
	if(temppp.first_step ==false)
		i++;
	if(temppp.second_step ==false)
		i++;
	if (temppp.thurth_step ==false) 
		i++;
	if (temppp.fifth_step ==false) 
		i++;
	if ( temppp.third_step ==false) 
		i++;
	if (temppp.sixth_step ==false)
		i++;
	if (i>2) 
	{
		return TRUE;
	}
	else
		return false;
}



int get_fan_var(wifstream & inf,int fan_value[])
{//used by loadfile2Tstat function
	TCHAR buf[1024];
	int fan_array_number=0;
	int m_heat_stages=0,m_cool_stages=0;
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(m_heat_stages==0 && m_cool_stages==0)
			if(find_sub_chars(buf,_T("Heating")) || find_sub_chars(buf,_T("Cooling")))
				get_heat_and_cool_stages_from_file_line(buf,m_heat_stages,m_cool_stages);
		if(section_end_find(buf))//for the value setting start and fan value end
			break;
		if(wcsstr(buf,_T("RELAY 1"))!=NULL)
		{
			int fan_array=fan_array_number++;///////////////fan on ,fan off ,fan lower,fan high,fan auto
			int array[7],order_array[7];
			get_fan_line_value(buf,array);
			turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
			for(int i=0;i<7;i++)				
				fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*1;//
			//*************************************relay 2
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 2"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*2;//				
			}
			else
				continue;
			//*************************************relay 3
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 3"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*4;//				
			}
			else
				continue;
			//*************************************relay 4
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 4"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*8;//				

			}
			else
				continue;
			//*************************************relay 5
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 5"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*16;//				
			}
			else
				continue;
		}
	}
	return fan_array_number;
}

void get_heat_and_cool_stages_from_file_line(TCHAR *buf,int &m_heat_stages,int & m_cool_stages)
{
		if(find_sub_chars(buf,temp_heating[5]))
			m_heat_stages=6;
		else if(find_sub_chars(buf,temp_heating[4]))
			m_heat_stages=5;
		else if(find_sub_chars(buf,temp_heating[3]))
			m_heat_stages=4;
		else if(find_sub_chars(buf,temp_heating[2]))
			m_heat_stages=3;
		else if(find_sub_chars(buf,temp_heating[1]))
			m_heat_stages=2;
		else if(find_sub_chars(buf,temp_heating[0]))
			m_heat_stages=1;

		if(find_sub_chars(buf,temp_cooling[5]))
			m_cool_stages=6;
		else if(find_sub_chars(buf,temp_cooling[4]))
			m_cool_stages=5;
		else if(find_sub_chars(buf,temp_cooling[3]))
			m_cool_stages=4;
		else if(find_sub_chars(buf,temp_cooling[2]))
			m_cool_stages=3;
		else if(find_sub_chars(buf,temp_cooling[1]))
			m_cool_stages=2;
		else if(find_sub_chars(buf,temp_cooling[0]))
			m_cool_stages=1;
}

bool section_end_find(TCHAR *buf)
{
	if(find_sub_chars(buf,_T("//   VALVE SETTING")))
		return true;
	if(find_sub_chars(buf,_T("// FANOFF VALVE END")))
		return true;
	if(find_sub_chars(buf,_T("//   UNIVERSAL FAN END")))
		return true;
	if(find_sub_chars(buf,_T("//   VALVE END")))
		return true;
	if(find_sub_chars(buf,_T("//   VALUE END")))//FOR OLD VERSION
		return true;
	if(find_sub_chars(buf,_T("//   DELAY END")))
		return true;
	if(find_sub_chars(buf,_T("//   LOOKUP TABLE END")))
		return true;
	if(find_sub_chars(buf,_T("//   UNIVERSAL FAN END")))
		return true;
	if(find_sub_chars(buf,_T("serialnumber:")))
		return true;	
	return false;
}
void get_fan_line_value(TCHAR *buf,int array[7])
{//used by get_fan_var function only
	buf=wcsstr(buf,_T(":"));
	buf++;
	TCHAR one_value[5]={'\0'};//the begin value is must
	int i=0;//for array[7]
	int j=0;//for every char
	do{
		if(*buf==' '||*buf=='\0')
		{
			if((*one_value)!='\0')////////////////////////
			{
				if(_wcsicmp(one_value,_T("OFF"))==0)
                    array[i]=0;
				else
					array[i]=1;
				i++;
				*one_value='\0';///////////////////////
			}
			buf++;
			j=0;
		}
		else
		{
			one_value[j]=*buf;
			j++;
			buf++;
			one_value[j]='\0';
		}
	}while(*buf!='\0');	
}
void turn_order_for_fan(int source[],int order[],int m_heat_stages,int m_cool_stages)//used by get_fan_var function and get_value_setting function
{
	for(int i=0;i<(m_heat_stages+m_cool_stages+1);i++)
		if(i < m_heat_stages)
			order[m_heat_stages+m_cool_stages - i]=source[i];
		else
			order[i - m_heat_stages]=source[i];						
}
bool find_sub_chars(CString m_array_char,CString sub_array_char)
{//find sub chars is exist?no up or lower?查找子串是否存在，不分大小写
	CString mother,son;
	mother=m_array_char;
	son=sub_array_char;
	mother.MakeLower();
	son.MakeLower();
	if(mother.Find(son)==-1)
		return false;
	else
        return true;
}


void get_value_setting_line_value(TCHAR *buf,int array[7],int line_number)
{//used by get_value_setting function only
	buf=wcsstr(buf,_T(":"));
	buf++;
	TCHAR one_value[10]={'\0'};//the begin value is must
	int i=0;//for array[7]
	int j=0;//for every char
	do{
		if(*buf==' '||*buf=='\0')
		{
			if((*one_value)!='\0')////////////////////////
			{
				/*
				if(strcmp(one_value,"CLOSE")==0)
                    array[i]+=0;
				else if(strcmp(one_value,"OPEN")==0)
					array[i]+=3;
				else if(strcmp(one_value,"0-100")==0)
					array[i]+=1;
				else if(strcmp(one_value,"50-100")==0)
					array[i]+=2;
				else if(strcmp(one_value,STRINGCONST[6])==0)
				{
					if(line_number==1)
						array[i]+=64;
					else if(line_number==2)
						array[i]+=128;
				}
				*/
				if(wcscmp(one_value,_T("Close"))==0)
					array[i]+=0;
				else if(wcscmp(one_value,_T("Open"))==0)
					array[i]+=1;
				else if(wcscmp(one_value,_T("0-100"))==0)
					array[i]+=2;
				else if(wcscmp(one_value,_T("50-100"))==0)
					array[i]+=3;
				else if(wcscmp(one_value,STRINGCONST[6])==0)
				{
					if(line_number==1)
						array[i]+=64;
					else if(line_number==2)
						array[i]+=128;
				}
				i++;
				*one_value='\0';///////////////////////
			}
			buf++;
			j=0;
		}
		else
		{
			one_value[j]=*buf;
			j++;
			buf++;
			one_value[j]='\0';
		}
	}while(*buf!='\0');	

}
void get_value_setting(wifstream & inf,int value_setting[7])
{//for value setting of file
	TCHAR buf[1024];
	int m_heat_stages=0,m_cool_stages=0;
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(m_heat_stages==0 && m_cool_stages==0)
			if(find_sub_chars(buf,_T("Heating")) || find_sub_chars(buf,_T("Cooling")) )
				get_heat_and_cool_stages_from_file_line(buf,m_heat_stages,m_cool_stages);
		if(section_end_find(buf))//for the value setting start and fan value end
			break;
		if(wcsstr(buf,_T("VALVE 1"))!=NULL)
		{
			int array[7]={0},order_array[7]={0};//must be clear to 0
			get_value_setting_line_value(buf,array,1);
			turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
			for(int i=0;i<7;i++)				
				value_setting[i]=value_setting[i] | order_array[i];//
			//*************************************relay 2			
		}
		else if(wcsstr(buf,_T("VALVE 2"))!=NULL)
		{
			int array[7]={0},order_array[7]={0};//must be clear to 0
			get_value_setting_line_value(buf,array,2);
			turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
			for(int i=0;i<7;i++)	
			{
				if(order_array[i] == 128)
					value_setting[i]=value_setting[i] | order_array[i];//				
				else
                    value_setting[i]=value_setting[i] | order_array[i]*4;//*4 bit 3 and bit 4
			}
		}
	}
}


void Show_load_file_error_message(load_file_every_step temppp,int section,CFile *p_log_file)
{//true show message box ,false no 
	CString for_showing_text=_T("");
	if(section==2)
	{
		if(temppp.first_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_ONE+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}
	if(section==3)
	{
		if(temppp.second_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_TWO+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}
	if(section==4)
	{
		if(temppp.third_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_THREE+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && !for_showing_text.IsEmpty())
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}
	if(section==5)
	{
		if(temppp.thurth_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_FOUR+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}
	if(section==6)
	{
		if(temppp.fifth_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_FIVE+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}
	if(section==7)
	{
		if(temppp.sixth_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_SIX+STEP_FAILURE+_T("*********************";)
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}
}

void get_delay_setting_line_value(TCHAR *buf,int array[5])
{//used by get_value_setting function only
	buf=wcsstr(buf,_T(":"));
	buf++;
	TCHAR one_value[10]={'\0'};//the begin value is must
	int i=0;//for array[7]
	int j=0;//for every char
	do{
		if(*buf==' '||*buf=='\0')
		{
			if((*one_value)!='\0')/////////////////////
			{
				array[i]=_wtoi(one_value);
				i++;
				*one_value='\0';///////////////////////
			}
			buf++;
			j=0;
		}
		else
		{
			one_value[j]=*buf;
			j++;
			buf++;
			one_value[j]='\0';
		}
	}while(*buf!='\0');	
}

void get_delay_setting(wifstream & inf,int value_setting[10])
{
	TCHAR buf[1024];
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(section_end_find(buf))//for the value setting start and fan value end
			break;
		if(wcsstr(buf,_T("1->0"))!=NULL)
		{
			int array[5];
			get_delay_setting_line_value(buf,array);
			for(int i=0;i<5;i++)				
				value_setting[i]=array[i];//
			//*************************************relay 2			
		}
		else if(wcsstr(buf,_T("0->1"))!=NULL)
		{
			int array[5];
			get_delay_setting_line_value(buf,array);
			for(int i=0;i<5;i++)				
				value_setting[5+i]=array[i];//				
		}
	}
}


void get_lookup_table_setting_line_value(TCHAR *buf,int array[11])
{
	buf=wcsstr(buf,_T(":"));
	buf++;
	TCHAR one_value[20]={'\0'};//the begin value is must
	int i=0;//for array[7]
	int j=0;//for every char
	do{
		if(*buf==' '||*buf=='\0')
		{
			if((*one_value)!='\0')////////////////////////
			{
				array[i]=_wtoi(one_value);
				i++;
				*one_value='\0';///////////////////////
			}
			buf++;
			j=0;
		}
		else
		{
			one_value[j]=*buf;
			j++;
			buf++;
			one_value[j]='\0';
		}
	}while(*buf!='\0');	
}

void get_lookup_table_setting(wifstream & inf,int lookup_table_setting[22])
{
	TCHAR buf[1024];
	while(!inf.eof())
	{
		inf.getline(buf,1024);		
		if(section_end_find(buf))//for the value setting start and fan value end
			break;
		if(wcsstr(buf,_T("Lookup Table 1:"))!=NULL)
			get_lookup_table_setting_line_value(buf,lookup_table_setting);
		else if(wcsstr(buf,_T("Lookup Table 2:"))!=NULL)
			get_lookup_table_setting_line_value(buf,&lookup_table_setting[11]);
	}
}

void get_universal_fan_var(wifstream & inf,int fan_value[])
{
	TCHAR buf[1024];
	int fan_array=0;
	int m_heat_stages=0,m_cool_stages=0;
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(m_heat_stages==0 && m_cool_stages==0)
			if(find_sub_chars(buf,_T("Heating")) || find_sub_chars(buf,_T("Cooling")))
				get_heat_and_cool_stages_from_file_line(buf,m_heat_stages,m_cool_stages);
		if(section_end_find(buf))//for the value setting start and fan value end
			break;
		if(wcsstr(buf,_T("RELAY 1"))!=NULL)
		{
			int array[7],order_array[7];
			get_fan_line_value(buf,array);
			turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
			for(int i=0;i<7;i++)				
				fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*1;//
			//*************************************relay 2
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 2"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*2;//				
			}
			else
				continue;
			//*************************************relay 3
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 3"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*4;//				
			}
			else
				continue;
			//*************************************relay 4
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 4"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*8;//				

			}
			else
				continue;
			//*************************************relay 5
			inf.getline(buf,1024);
			if(wcsstr(buf,_T("RELAY 5"))!=NULL)
			{
				get_fan_line_value(buf,array);
				turn_order_for_fan(array,order_array,m_heat_stages,m_cool_stages);
				for(int i=0;i<7;i++)				
					fan_value[7*fan_array+i]=fan_value[7*fan_array+i] | order_array[i]*16;//				
			}
			else
				continue;
		}
	}
}
void get_var_write_var(wifstream & inf,float tstat_version,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{
	TCHAR buf[1024];	
	while(!inf.eof())
	{
		inf.getline(buf,1024);

		if(find_sub_chars(buf,_T("END CONFIG")))
			break;


		get_write_var_line(buf,tstat_version,p_log_file,p_log_file_one_time);//get a line ,one register value,
	}
}

	
int fan_number;

void get_write_var_line(TCHAR *buf,float tstat_version,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{//used by get_value_setting function only
	CString for_showing_text;
	if(_wtoi(buf)==0)
		return ;
	TCHAR *temp=buf;
	temp=wcsstr(temp,_T(","));
	temp++;	
	int register_id=_wtoi(buf);
	int register_value=_wtoi(temp);
	int j;
	int register_122=0;
	if(register_id==121 && register_value>1)
		j=-4;//wrong value
	/*
	else if(register_id==128 && register_value>5)
		j=-4;//wrong value
		//comment on 09-02-03
		*/

	else if(register_id==122 )
	{
		int real_fan_number=get_real_number_fan_speeds(fan_number);
		if(register_value!=real_fan_number && real_fan_number!=-1)
		{
			register_122=-100;
			register_value=real_fan_number;/////////////^-^ 
		}
		j=write_one(now_tstat_id,register_id,register_value);	
	}
	else
		j=write_one(now_tstat_id,register_id,register_value);	
	if(register_id==185)
	{
		Sleep(14000);	
		//if(register_value==1)Change_BaudRate(19200);
		//if(register_value==0)Change_BaudRate(9600);
	}

	if(j==-2 && tstat_version<25 && tstat_version >0)  ////////////////////////if the version is 24.4 ,write_one some register will restart,for example 118,121
	{
		Sleep(14000);	
		j=write_one(now_tstat_id,register_id,register_value);	
	}
	if(p_log_file!=NULL)
	{
		for_showing_text.Format(_T("register ID:%d,VALUE:%d write "),register_id,register_value);
		if(register_122==-100)
		{
			CString t_str=for_showing_text;
			for_showing_text.Format(_T("NUMBER_OF_FAN_SPEEDS IS INCORRECT,try correct value :%d "),register_value);
			for_showing_text=t_str+for_showing_text;
		}
		if(j>0)
			for_showing_text=for_showing_text+_T("OK\r\n");
		else
		{
			p_log_file_one_time->seven_step=false;
			for_showing_text=for_showing_text+_T("Error******************\r\n");
		}
		change_showing_text_variable(for_showing_text);
	//	p_log_file->Write(for_showing_text.GetString(),for_showing_text.GetLength());
		p_log_file->WriteString(for_showing_text);
	}
}

void Save2File_ForTwoFilesTSTAT67( TCHAR* fn )
{
	CString strTips;
	int nValue;
	//nValue=read_one(g_tstat_id,324);
	//	write_one(g_tstat_id,324,0);//////
	// save tstat register value to file

// 		268	330	1	Low byte	W/R	Number of Heating Stages (Max heat+cool = 6)
// 		269	331	1	Low byte	W/R	Number of Cooling Stages (Max heat + Cool = 6) 
// 		276	332	1	Low byte	W/R	Number of Heating Stages in Original Table - (Maximum # of total heating and cooling states is 6)
// 		277	333	1	Low byte	W/R	Number of Cooling Stages in Original Table - (Maximum # of total heating and cooling states is 6)



	int m_25_heat_stages,m_25_cool_stages,m_26_heat_stages,m_26_cool_stages;
// 	m_25_heat_stages=read_one(g_tstat_id,276);
// 	m_25_cool_stages=read_one(g_tstat_id,277);
// 	m_26_heat_stages=read_one(g_tstat_id,268);
// 	m_26_cool_stages=read_one(g_tstat_id,269);


	m_25_heat_stages=read_one(g_tstat_id,332);
	m_25_cool_stages=read_one(g_tstat_id,333);
	m_26_heat_stages=read_one(g_tstat_id,330);
	m_26_cool_stages=read_one(g_tstat_id,331);
	if(m_25_heat_stages==0 && m_25_cool_stages==0)
	{
		m_25_heat_stages=3;
		m_25_cool_stages=3;
	}
#if 1
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 10%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	CString str;
	//25
	str=_T("");
	m_25=_T("//	");
	for(int i=0;i<m_25_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_25_heat_stages-i-1]);
		m_25+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_25+=str;
	for(int i=0;i<m_25_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_25+=str;
	}
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 15%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	//26
	str=_T("");
	m_26=_T("//	");
	for(int i=0;i<m_26_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_26_heat_stages-i-1]);
		m_26+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_26+=str;
	for(int i=0;i<m_26_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_26+=str;
	}	
	float version=get_tstat_version(g_tstat_id);
	TCHAR a[3]={'\0','\0','\0'};
	if(version >= 26)
	{
		a[0]='2';
		a[1]='6';
		a[2]='\0';
	}
	else if(version >= 25)
	{
		a[0]='2';
		a[1]='5';
		a[2]='\0';
	}
	else
	{
		a[0]='2';
		a[1]='4';
		a[2]='\0';
	}
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 20%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	wofstream out;
	out.open(fn,ios_base::out) ;

	_Twrite_to_file_a_line(out,_T("Tstat Config File"));//added the header marker.

	int nModelID = read_one(g_tstat_id, 7);
	CString strProductClassName = get_product_class_name_by_model_ID(nModelID);
	strProductClassName = _T("Model : ") + strProductClassName;
	_Twrite_to_file_a_line(out, strProductClassName);//added the model
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 30%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	out<<"version:"<<version<<endl;
	fan_write(out,m_25_heat_stages,m_25_cool_stages);//fan value	
	_Twrite_to_file_a_line(out,_T(" "));//space
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 40%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	value_setting_write(out,m_25_heat_stages,m_25_cool_stages);//value setting
	write_to_file_a_line(out," ");//space
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 50%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////

	if(version>=25)
		delay_time_write(out);//delay time write
	if(version>=26)
	{
		_Twrite_to_file_a_line(out,_T(" "));//space
		lookup_table_write(out);//lookup table 26 only
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		strTips = _T("Config file saved 60%...");
		SetPaneString(1, strTips);
		//////////////////////////////////////////////////////////////////////////
		universal_relay_write(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		strTips = _T("Config file saved 70%...");
		SetPaneString(1, strTips);
		//////////////////////////////////////////////////////////////////////////
		universal_value_setting_write(out,m_26_heat_stages,m_26_cool_stages);
	}
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 80%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	_Twrite_to_file_a_line(out,_T(" "));//space
	WriteSerialNumber(out);
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 90%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	WriteAddress(out);
	_Twrite_to_file_a_line(out,_T(" "));//space
	//////////////////////////////////////////////////////////////////////////
	strTips = _T("Config file saved 99%...");
	SetPaneString(1, strTips);
	//////////////////////////////////////////////////////////////////////////
	var_write(out);	
	//_Twrite_to_file_a_line(out,_T("OK!"));//space
	return;

	_Twrite_to_file_a_line(out,_T("//END CONFIG 1 ********************************// "));//space
	int nvalue=0;

	//nvalue=read_one(g_tstat_id,326);
	/*
	if(nvalue==0)
	{
	if (nValue>=0)
	{
	write_one(g_tstat_id,324,nValue);
	}
	return;
	}


	if(nvalue==1)
	write_one(g_tstat_id,324,1);//////	
	else
	*/
	return;

	/*******************************************************************************************************

	SetPaneString("Begin to save configuration file 2...");

	*******************************************************************************************************/

	// 		268	330	1	Low byte	W/R	Number of Heating Stages (Max heat+cool = 6)
	// 		269	331	1	Low byte	W/R	Number of Cooling Stages (Max heat + Cool = 6) 
	// 		276	332	1	Low byte	W/R	Number of Heating Stages in Original Table - (Maximum # of total heating and cooling states is 6)
	// 		277	333	1	Low byte	W/R	Number of Cooling Stages in Original Table - (Maximum # of total heating and cooling states is 6)

// 	m_25_heat_stages=read_one(g_tstat_id,276);
// 	m_25_cool_stages=read_one(g_tstat_id,277);
// 	m_26_heat_stages=read_one(g_tstat_id,268);
// 	m_26_cool_stages=read_one(g_tstat_id,269);


	m_25_heat_stages=read_one(g_tstat_id,332);
	m_25_cool_stages=read_one(g_tstat_id,333);
	m_26_heat_stages=read_one(g_tstat_id,330);
	m_26_cool_stages=read_one(g_tstat_id,331);
	if(m_25_heat_stages==0 && m_25_cool_stages==0)
	{
		m_25_heat_stages=3;
		m_25_cool_stages=3;
	}

	//	CString str;
	//25
	str=_T("");
	m_25=_T("//	");
	for(int i=0;i<m_25_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_25_heat_stages-i-1]);
		m_25+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_25+=str;
	for(int i=0;i<m_25_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_25+=str;
	}

	//26
	str=_T("");
	m_26=_T("//	");
	for(int i=0;i<m_26_heat_stages;i++)
	{
		str.Format(_T("%s "),temp_heating[m_26_heat_stages-i-1]);
		m_26+=str;
	}
	str.Format(_T("%s "),_T("Coasting"));
	m_26+=str;
	for(int i=0;i<m_26_cool_stages;i++)
	{
		str.Format(_T("%s "),temp_cooling[i]);
		m_26+=str;
	}	
	version=get_tstat_version(g_tstat_id);
	//	a[3]={'\0','\0','\0'};
	if(version >= 26)
	{
		a[0]='2';
		a[1]='6';
		a[2]='\0';
	}
	else if(version >= 25)
	{
		a[0]='2';
		a[1]='5';
		a[2]='\0';
	}
	else
	{
		a[0]='2';
		a[1]='4';
		a[2]='\0';
	}


	fan_write(out,m_25_heat_stages,m_25_cool_stages);//fan value	
	_Twrite_to_file_a_line(out,_T(" "));//space
	value_setting_write(out,m_25_heat_stages,m_25_cool_stages);//value setting
	_Twrite_to_file_a_line(out,_T(" "));//space
	if(version>=25)
		delay_time_write(out);//delay time write
	if(version>=26)
	{
		_Twrite_to_file_a_line(out,_T(" "));//space
		lookup_table_write(out);//lookup table 26 only
		_Twrite_to_file_a_line(out,_T(" "));//space
		universal_relay_write(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		universal_value_setting_write(out,m_26_heat_stages,m_26_cool_stages);
	}
	_Twrite_to_file_a_line(out,_T(" "));//space
	WriteSerialNumber(out);
	WriteAddress(out);
	_Twrite_to_file_a_line(out,_T(" "));//space
	var_write(out);	
	out.close();

	if (nValue>=0)
	{
		write_one(g_tstat_id,324,nValue);
	}


#endif
}
#if 0
void LoadFile2Tstat(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file)
{
  // int nSpecialValue=read_one(now_tstat_id,326);
//	write_one(now_tstat_id,324,0);
	//every step is false

	CString for_showing_text;
	load_file_one_time.first_step=load_file_one_time.second_step=load_file_one_time.third_step=load_file_one_time.thurth_step=load_file_one_time.fifth_step=load_file_one_time.sixth_step=load_file_one_time.seven_step=load_file_one_time.eight_step=false;
	float tstat_version;
	tstat_version=get_tstat_version(now_tstat_id);///////////////////get version
	wifstream inf;//file
	inf.open(fn,ios_base::in);
	int fan_value[35]={0};//fan_value 
	for(int i=0;i<35;i++)
		fan_value[i]=0;	

	fan_number=get_fan_var(inf,fan_value);//fan value			
	load_file_one_time.first_step=true;
	for(int i=0;i<fan_number;i++)
	{
		for(int t_i=0;t_i<7;t_i++)
			{
				int real_fan_address=get_real_fan_address(fan_number-2,i,t_i);
				if(fan_value[i]>=0)
				{
					int i_i=write_one(now_tstat_id,real_fan_address,fan_value[i*7+t_i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID: %d value:%d write OK")"),real_fan_address,fan_value[i*7+t_i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************"),real_fan_address,fan_value[i*7+t_i]);
							load_file_one_time.first_step=false;
						}
						change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text+_T("\r\n"));
					}
				}
			}
		}
			if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));	
			int value_setting[7]={0};			

			get_value_setting(inf,value_setting);//value setting
			Show_load_file_error_message(load_file_one_time,2,p_log_file);
			load_file_one_time.second_step=true;
			for(int i=0;i<7;i++)
			{
				if(value_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,173+i,value_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK"),173+i,value_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************"),173+i,value_setting[i]);
							load_file_one_time.second_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text+_T("\r\n"));
					}
				}
			}
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
	TCHAR buf[1024];
	
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(find_sub_chars(buf,_T("//   FAN OFF")))
			break;

		if(wcscmp(buf,_T("//   DELAY TIMES"))==0)
		{
			int delay_setting[10]={0};
			get_delay_setting(inf,delay_setting);
			Show_load_file_error_message(load_file_one_time,3,p_log_file);
			load_file_one_time.third_step=true;
			for(int i=0;i<10;i++)
			{
				if(delay_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,191+i,delay_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK"),191+i,delay_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************"),191+i,delay_setting[i]);
							load_file_one_time.third_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text+_T("\r\n"));
					}
				}
			}
			//26
			//lookup table setting
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
		else if(wcscmp(buf,_T("//   LOOKUP TABLE"))==0)
		{
			if(tstat_version<CUSTOM_TABLE_FLOAT_VERSION)
			{
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;
				for(int i=0;i<22;i++)
				{
					if(lookup_table_setting[i]>=0)
					{
						int i_i=write_one(now_tstat_id,219+i,lookup_table_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK"),219+i,lookup_table_setting[i]);
							else
							{							
								for_showing_text.Format(_T("register ID:%d value:%d write Error******************"),219+i,lookup_table_setting[i]);
								load_file_one_time.thurth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text+_T("\r\n"));
						}
					}					
				}
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));
			}
			else if(tstat_version>=CUSTOM_TABLE_FLOAT_VERSION)
			{
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;

				int ntemp=(int)lookup_table_setting[0];

				//table1
				{
				short high=ntemp/65536;
				short low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_ZERO,low);
				write_one(now_tstat_id, MODBUS_TABLE1_ZERO_HI,high);
				//2
				ntemp=lookup_table_setting[1];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFONE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFONE_HI,high);
				//3
				ntemp=lookup_table_setting[2];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_ONE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_ONE_HI,high);
				//4
				ntemp=(int)lookup_table_setting[3];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTWO,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTWO_HI,high);
				//5
				ntemp=lookup_table_setting[4];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_TWO,low);
				write_one(now_tstat_id, MODBUS_TABLE1_TWO_HI,high);
				//6
				ntemp=lookup_table_setting[5];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTHREE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTHREE_HI,high);
				//7
				ntemp=lookup_table_setting[6];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_THREE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_THREE_HI,high);
				//8

				ntemp=lookup_table_setting[7];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFOUR_HI,high);
				//9
				ntemp=lookup_table_setting[8];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_FOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE1_FOUR_HI,high);
				//10
				ntemp=lookup_table_setting[9];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFIVE_HI,high);
				//11
				ntemp=lookup_table_setting[10];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_FIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_FIVE_HI,high);
				}

				//table 2
				{
				int ntemp=(int)lookup_table_setting[11];
				short high=ntemp/65536;
				short low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_ZERO,low);
				write_one(now_tstat_id, MODBUS_TABLE2_ZERO_HI,high);
				//2
				ntemp=lookup_table_setting[12];
				high=ntemp/65536;
				low=ntemp%65536;		
				write_one(now_tstat_id, MODBUS_TABLE2_HALFONE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFONE_HI,high);
				//3
				ntemp=lookup_table_setting[13];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_ONE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_ONE_HI,high);
				//4
				ntemp=(int)lookup_table_setting[14];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTWO,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTWO_HI,high);
				//5
				ntemp=lookup_table_setting[15];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_TWO,low);
				write_one(now_tstat_id, MODBUS_TABLE2_TWO_HI,high);
				//6
				ntemp=lookup_table_setting[16];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTHREE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTHREE_HI,high);
				//7
				ntemp=lookup_table_setting[17];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_THREE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_THREE_HI,high);
				//8

				ntemp=lookup_table_setting[18];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFOUR_HI,high);
				//9
				ntemp=lookup_table_setting[19];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_FOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE2_FOUR_HI,high);
				//10
				ntemp=lookup_table_setting[20];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFIVE_HI,high);
				//11
				ntemp=lookup_table_setting[21];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_FIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_FIVE_HI,high);
				
				}
			}
		}
			else if(wcscmp(buf,_T("//   UNIVERSAL RELAY OUTPUTS"))==0)
			{
				int universal_value[7]={0};
				get_universal_fan_var(inf,universal_value);
				Show_load_file_error_message(load_file_one_time,5,p_log_file);
				load_file_one_time.fifth_step=true;
				for(int i=0;i<7;i++)
				{
					if(universal_value[i]>=0)
					{
						int i_i=write_one(now_tstat_id,254+i,universal_value[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK"),254+i,universal_value[i]);
							else
							{
								for_showing_text.Format(_T("register ID:%d vlaue:%d write Error******************"),254+i,universal_value[i]);
								load_file_one_time.fifth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text+_T("\r\n"));
						}
					}						
				}	
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));
			}

			else if(wcscmp(buf,_T("//   UNIVERSAL VALVE"))==0)
			{
				//universal value setting
				int value_setting[7]={0};
				for(int i=0;i<7;i++)
					value_setting[i]=0;
				get_value_setting(inf,value_setting);//universal value setting
				Show_load_file_error_message(load_file_one_time,6,p_log_file);
				load_file_one_time.sixth_step=true;
				for(int i=0;i<7;i++)
				{
					if(value_setting[i]>=0)

					{
						int i_i=write_one(now_tstat_id,261+i,value_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK"),261+i,value_setting[i]);
							else
							{
								for_showing_text.Format(_T("register ID:%d value:%d write Error******************"),261+i,value_setting[i]);
								load_file_one_time.sixth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text+_T("\r\n"));
						}
					}						
				}
				//26 end 
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));			
		}
		else if(wcsstr(buf,_T("serialnumber"))!=NULL || wcsstr(buf,_T("address:"))!=NULL )
		{
			/////////////////////////////////////var setting
			Show_load_file_error_message(load_file_one_time,7,p_log_file);
			write_one(now_tstat_id,184,5);//184 register,to no restart,when you write the 185,118,121,128 register
			load_file_one_time.seven_step=true;
			get_var_write_var(inf,tstat_version,p_log_file,&load_file_one_time);//////a line //////////// a line
			write_one(now_tstat_id,16,159);//////
			//reset 
			write_one(now_tstat_id,184,0);//184 register,to no restart,when you write the 185,118,121,128 register
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
	}


		
	inf.close();
	return;
	//Sleep(15000);

//
//	LoadFile2Tstat_twofile(load_file_one_time,fn,p_log_file);

}
#endif

void LoadFile2Tstat(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file)
{
   //int nSpecialValue=read_one(now_tstat_id,326);
	//write_one(g_tstat_id,324,0);
	//every step is false

	CString for_showing_text;
	load_file_one_time.first_step=load_file_one_time.second_step=load_file_one_time.third_step=load_file_one_time.thurth_step=load_file_one_time.fifth_step=load_file_one_time.sixth_step=load_file_one_time.seven_step=load_file_one_time.eight_step=false;
	float tstat_version;
	tstat_version=get_tstat_version(now_tstat_id);///////////////////get version
	wifstream inf;//file
	inf.open(fn,ios_base::in);
	int fan_value[35]={0};//fan_value 
	for(int i=0;i<35;i++)
		fan_value[i]=0;	

	fan_number=get_fan_var(inf,fan_value);//fan value			
	load_file_one_time.first_step=true;
	for(int i=0;i<fan_number;i++)
	{
		for(int t_i=0;t_i<7;t_i++)
			{
				int real_fan_address=get_real_fan_address(fan_number-2,i,t_i);
				if(fan_value[i]>=0)
				{
					int i_i=write_one(now_tstat_id,real_fan_address,fan_value[i*7+t_i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID: %d value:%d write OK\r\n"),real_fan_address,fan_value[i*7+t_i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),real_fan_address,fan_value[i*7+t_i]);
							load_file_one_time.first_step=false;
						}
						change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text);
					}
				}
			}
		}
			if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));	
			int value_setting[7]={0};
			memset(value_setting,0,sizeof(value_setting));

			get_value_setting(inf,value_setting);//value setting
			Show_load_file_error_message(load_file_one_time,2,p_log_file);
			load_file_one_time.second_step=true;
			for(int i=0;i<7;i++)
			{
				if(value_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,173+i,value_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),173+i,value_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),173+i,value_setting[i]);
							load_file_one_time.second_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
			/*
			memset(value_setting,0,sizeof(value_setting));
			get_value_setting(inf,value_setting);//value setting			
			Show_load_file_error_message(load_file_one_time,2,p_log_file);
			load_file_one_time.second_step=true;
			for(int i=0;i<7;i++)
			{
				if(value_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,351+i,value_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),351+i,value_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),351+i,value_setting[i]);
							load_file_one_time.second_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
			*/



	if(p_log_file!=NULL)
		p_log_file->WriteString(_T("\r\n"));
	TCHAR buf[1024];
	
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(find_sub_chars(buf,_T("//   FAN OFF")))
			break;

		if(wcscmp(buf,_T("//   DELAY TIMES"))==0)
		{
			int delay_setting[10]={0};
			get_delay_setting(inf,delay_setting);
			Show_load_file_error_message(load_file_one_time,3,p_log_file);
			load_file_one_time.third_step=true;
			for(int i=0;i<10;i++)
			{
				if(delay_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,191+i,delay_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK\r\n"),191+i,delay_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),191+i,delay_setting[i]);
							load_file_one_time.third_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
			//26
			//lookup table setting
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
		else if(wcscmp(buf,_T("//   LOOKUP TABLE"))==0)
		{
			if(tstat_version<CUSTOM_TABLE_FLOAT_VERSION)
			{
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;
				for(int i=0;i<22;i++)
				{
					if(lookup_table_setting[i]>=0)
					{
						int i_i=write_one(now_tstat_id,219+i,lookup_table_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),219+i,lookup_table_setting[i]);
							else
							{							
								for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),219+i,lookup_table_setting[i]);
								load_file_one_time.thurth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text);
						}
					}					
				}
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));
			}
			else if(tstat_version>=CUSTOM_TABLE_FLOAT_VERSION)
			{
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;

				int ntemp=(int)lookup_table_setting[0];

				//table1
				{
				short high=ntemp/65536;
				short low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_ZERO,low);
				write_one(now_tstat_id, MODBUS_TABLE1_ZERO_HI,high);
				//2
				ntemp=lookup_table_setting[1];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFONE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFONE_HI,high);
				//3
				ntemp=lookup_table_setting[2];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_ONE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_ONE_HI,high);
				//4
				ntemp=(int)lookup_table_setting[3];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTWO,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTWO_HI,high);
				//5
				ntemp=lookup_table_setting[4];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_TWO,low);
				write_one(now_tstat_id, MODBUS_TABLE1_TWO_HI,high);
				//6
				ntemp=lookup_table_setting[5];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTHREE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFTHREE_HI,high);
				//7
				ntemp=lookup_table_setting[6];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_THREE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_THREE_HI,high);
				//8

				ntemp=lookup_table_setting[7];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFOUR_HI,high);
				//9
				ntemp=lookup_table_setting[8];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_FOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE1_FOUR_HI,high);
				//10
				ntemp=lookup_table_setting[9];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_HALFFIVE_HI,high);
				//11
				ntemp=lookup_table_setting[10];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE1_FIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE1_FIVE_HI,high);
				}

				//table 2
				{
				int ntemp=(int)lookup_table_setting[11];
				short high=ntemp/65536;
				short low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_ZERO,low);
				write_one(now_tstat_id, MODBUS_TABLE2_ZERO_HI,high);
				//2
				ntemp=lookup_table_setting[12];
				high=ntemp/65536;
				low=ntemp%65536;		
				write_one(now_tstat_id, MODBUS_TABLE2_HALFONE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFONE_HI,high);
				//3
				ntemp=lookup_table_setting[13];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_ONE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_ONE_HI,high);
				//4
				ntemp=(int)lookup_table_setting[14];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTWO,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTWO_HI,high);
				//5
				ntemp=lookup_table_setting[15];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_TWO,low);
				write_one(now_tstat_id, MODBUS_TABLE2_TWO_HI,high);
				//6
				ntemp=lookup_table_setting[16];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTHREE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFTHREE_HI,high);
				//7
				ntemp=lookup_table_setting[17];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_THREE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_THREE_HI,high);
				//8

				ntemp=lookup_table_setting[18];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFOUR_HI,high);
				//9
				ntemp=lookup_table_setting[19];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_FOUR,low);
				write_one(now_tstat_id, MODBUS_TABLE2_FOUR_HI,high);
				//10
				ntemp=lookup_table_setting[20];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_HALFFIVE_HI,high);
				//11
				ntemp=lookup_table_setting[21];
				high=ntemp/65536;
				low=ntemp%65536;
				write_one(now_tstat_id, MODBUS_TABLE2_FIVE,low);
				write_one(now_tstat_id, MODBUS_TABLE2_FIVE_HI,high);
				
				}
			}

		}
		
			else if(wcscmp(buf,_T("//   UNIVERSAL RELAY OUTPUTS"))==0)
			{
				int universal_value[7]={0};
				get_universal_fan_var(inf,universal_value);
				Show_load_file_error_message(load_file_one_time,5,p_log_file);
				load_file_one_time.fifth_step=true;
				for(int i=0;i<7;i++)
				{
					if(universal_value[i]>=0)
					{
						int i_i=write_one(now_tstat_id,254+i,universal_value[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),254+i,universal_value[i]);
							else
							{
								for_showing_text.Format(_T("register ID:%d vlaue:%d write Error******************\r\n"),254+i,universal_value[i]);
								load_file_one_time.fifth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text.GetString());
						}
					}						
				}	
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));
			}
			else if(wcscmp(buf,_T("//   UNIVERSAL VALVE"))==0)
			{
				//universal value setting
				int value_setting[7]={0};
				for(int i=0;i<7;i++)
					value_setting[i]=0;
				get_value_setting(inf,value_setting);//universal value setting
				Show_load_file_error_message(load_file_one_time,6,p_log_file);
				load_file_one_time.sixth_step=true;
				for(int i=0;i<7;i++)
				{
					if(value_setting[i]>=0)
					{
						int i_i=write_one(now_tstat_id,261+i,value_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),261+i,value_setting[i]);
							else
							{
								for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
								load_file_one_time.sixth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text.GetString());
						}
					}						
				}
				//26 end 
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));			
		}
		else if(wcsstr(buf,_T("serialnumber"))!=NULL || wcsstr(buf,_T("address:"))!=NULL )
		{
			/////////////////////////////////////var setting
			Show_load_file_error_message(load_file_one_time,7,p_log_file);
			write_one(now_tstat_id,184,5);//184 register,to no restart,when you write the 185,118,121,128 register
			load_file_one_time.seven_step=true;
			get_var_write_var(inf,tstat_version,p_log_file,&load_file_one_time);//////a line //////////// a line
			write_one(now_tstat_id,16,159);//////
			//reset 
			write_one(now_tstat_id,184,0);//184 register,to no restart,when you write the 185,118,121,128 register
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
	}


		
	inf.close();
	Sleep(5000);
//
	//LoadFile2Tstat_twofile(load_file_one_time,fn,p_log_file);

}



void LoadFile2Tstat_twofile(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile *p_log_file)
{
	return;
#if 0
//	int nSpecialValue=read_one(g_tstat_id,326);
//	if(nSpecialValue!=1)
//		return;
		

	//every step is false
	CString for_showing_text;

	float tstat_version;
	tstat_version=get_tstat_version(g_tstat_id);///////////////////get version
	wifstream inf;//file
	inf.open(fn,ios_base::in);

	//找到第二个文件的起始位置。
	TCHAR bufLine[1024];
	BOOL bFind=false;
	while(!inf.eof())
	{
		inf.getline(bufLine,1024);
		if(find_sub_chars(bufLine,_T("END CONFIG")))
		{
			bFind=true;
			break;
		}
		else//has no second file;
		{
			continue;

		}
	}
	if(!bFind)
		return;
	load_file_one_time.first_step=load_file_one_time.second_step=load_file_one_time.third_step=load_file_one_time.thurth_step=load_file_one_time.fifth_step=load_file_one_time.sixth_step=load_file_one_time.seven_step=load_file_one_time.eight_step=false;
		//	inf.close();
	write_one(g_tstat_id,324,1);

			int fan_value[35]={0};//fan_value 
			for(int i=0;i<35;i++)
				fan_value[i]=0;			
			fan_number=get_fan_var(inf,fan_value);//fan value			
			load_file_one_time.first_step=true;
			for(int i=0;i<fan_number;i++)
			{
				for(int t_i=0;t_i<7;t_i++)
				{
					int real_fan_address=get_real_fan_address(fan_number-2,i,t_i);
					if(fan_value[i]>=0)
					{
						int i_i=write_one(g_tstat_id,real_fan_address,fan_value[i*7+t_i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID: %d value:%d write OK\r\n"),real_fan_address,fan_value[i*7+t_i]);
							else
							{
								for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),real_fan_address,fan_value[i*7+t_i]);
								load_file_one_time.first_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text);
						}
					}
				}
			}
			if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));	
			int value_setting[7]={0};			
			get_value_setting(inf,value_setting);//value setting
			Show_load_file_error_message(load_file_one_time,2,p_log_file);
			load_file_one_time.second_step=true;
			for(int i=0;i<7;i++)
			{
				if(value_setting[i]>=0)
				{
					int i_i=write_one(g_tstat_id,173+i,value_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),173+i,value_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),173+i,value_setting[i]);
							load_file_one_time.second_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
	TCHAR buf[1024];

#if 1
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(wcscmp(buf,_T("//   DELAY TIMES"))==0)
		{
			int delay_setting[10]={0};
			get_delay_setting(inf,delay_setting);
			Show_load_file_error_message(load_file_one_time,3,p_log_file);
			load_file_one_time.third_step=true;
			for(int i=0;i<10;i++)
			{
				if(delay_setting[i]>=0)
				{
					int i_i=write_one(g_tstat_id,191+i,delay_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK\r\n"),191+i,delay_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),191+i,delay_setting[i]);
							load_file_one_time.third_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
			//26
			//lookup table setting
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
		else if(wcscmp(buf,_T("//   LOOKUP TABLE"))==0)
		{
			if(tstat_version<CUSTOM_TABLE_FLOAT_VERSION)
			{
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;
				for(int i=0;i<22;i++)
				{
					if(lookup_table_setting[i]>=0)
					{
						int i_i=write_one(g_tstat_id,219+i,lookup_table_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),219+i,lookup_table_setting[i]);
							else
							{							
								for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),219+i,lookup_table_setting[i]);
								load_file_one_time.thurth_step=false;
							}
							change_showing_text_variable(for_showing_text);
							p_log_file->WriteString(for_showing_text);
						}
					}					
				}
				if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));
			}
			else if(tstat_version>=CUSTOM_TABLE_FLOAT_VERSION)
			{
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;

		{
		int ntemp=(int)lookup_table_setting[0];
		short high=ntemp/65536;
		short low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_ZERO,low);
		write_one(g_tstat_id, MODBUS_TABLE1_ZERO_HI,high);
		//2
		ntemp=lookup_table_setting[1];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_HALFONE,low);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFONE_HI,high);
		//3
		ntemp=lookup_table_setting[2];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_ONE,low);
		write_one(g_tstat_id, MODBUS_TABLE1_ONE_HI,high);
		//4
		ntemp=(int)lookup_table_setting[3];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_HALFTWO,low);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFTWO_HI,high);
		//5
		ntemp=lookup_table_setting[4];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_TWO,low);
		write_one(g_tstat_id, MODBUS_TABLE1_TWO_HI,high);
		//6
		ntemp=lookup_table_setting[5];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_HALFTHREE,low);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFTHREE_HI,high);
		//7
		ntemp=lookup_table_setting[6];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_THREE,low);
		write_one(g_tstat_id, MODBUS_TABLE1_THREE_HI,high);
		//8

		ntemp=lookup_table_setting[7];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_HALFFOUR,low);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFFOUR_HI,high);
		//9
		ntemp=lookup_table_setting[8];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_FOUR,low);
		write_one(g_tstat_id, MODBUS_TABLE1_FOUR_HI,high);
		//10
		ntemp=lookup_table_setting[9];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_HALFFIVE,low);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFFIVE_HI,high);
		//11
		ntemp=lookup_table_setting[10];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE1_FIVE,low);
		write_one(g_tstat_id, MODBUS_TABLE1_FIVE_HI,high);
		}

		//table 2
		{
		int ntemp=(int)lookup_table_setting[11];
		short high=ntemp/65536;
		short low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_ZERO,low);
		write_one(g_tstat_id, MODBUS_TABLE2_ZERO_HI,high);
		//2
		ntemp=lookup_table_setting[12];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_HALFONE,low);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFONE_HI,high);
		//3
		ntemp=lookup_table_setting[13];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_ONE,low);
		write_one(g_tstat_id, MODBUS_TABLE2_ONE_HI,high);
		//4
		ntemp=(int)lookup_table_setting[14];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_HALFTWO,low);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFTWO_HI,high);
		//5
		ntemp=lookup_table_setting[15];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_TWO,low);
		write_one(g_tstat_id, MODBUS_TABLE2_TWO_HI,high);
		//6
		ntemp=lookup_table_setting[16];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_HALFTHREE,low);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFTHREE_HI,high);
		//7
		ntemp=lookup_table_setting[17];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_THREE,low);
		write_one(g_tstat_id, MODBUS_TABLE2_THREE_HI,high);
		//8

		ntemp=lookup_table_setting[18];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_HALFFOUR,low);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFFOUR_HI,high);
		//9
		ntemp=lookup_table_setting[19];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_FOUR,low);
		write_one(g_tstat_id, MODBUS_TABLE2_FOUR_HI,high);
		//10
		ntemp=lookup_table_setting[20];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_HALFFIVE,low);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFFIVE_HI,high);
		//11
		ntemp=lookup_table_setting[21];
		high=ntemp/65536;
		low=ntemp%65536;
		write_one(g_tstat_id, MODBUS_TABLE2_FIVE,low);
		write_one(g_tstat_id, MODBUS_TABLE2_FIVE_HI,high);

		}
			}
		}
		else if(wcscmp(buf,_T("//   UNIVERSAL RELAY OUTPUTS"))==0)
		{
			int universal_value[7]={0};
			get_universal_fan_var(inf,universal_value);
			Show_load_file_error_message(load_file_one_time,5,p_log_file);
			load_file_one_time.fifth_step=true;
			for(int i=0;i<7;i++)
			{
				if(universal_value[i]>=0)
				{
					int i_i=write_one(g_tstat_id,254+i,universal_value[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),254+i,universal_value[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d vlaue:%d write Error******************\r\n"),254+i,universal_value[i]);
							load_file_one_time.fifth_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}						
			}	
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
		else if(wcscmp(buf,_T("//   UNIVERSAL VALVE"))==0)
		{
			//universal value setting
			int value_setting[7]={0};
			for(int i=0;i<7;i++)
				value_setting[i]=0;
			get_value_setting(inf,value_setting);//universal value setting
			Show_load_file_error_message(load_file_one_time,6,p_log_file);
			load_file_one_time.sixth_step=true;
			for(int i=0;i<7;i++)
			{
				if(value_setting[i]>=0)
				{
					int i_i=write_one(g_tstat_id,261+i,value_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),261+i,value_setting[i]);
						else
						{
							for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
							load_file_one_time.sixth_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}						
			}	
			//26 end 
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));			
		}
		else if(wcsstr(buf,_T("serialnumber"))!=NULL || wcsstr(buf,_T("address:"))!=NULL )
		{
			/////////////////////////////////////var setting
			Show_load_file_error_message(load_file_one_time,7,p_log_file);
			write_one(g_tstat_id,184,5);//184 register,to no restart,when you write the 185,118,121,128 register
			load_file_one_time.seven_step=true;
			get_var_write_var(inf,tstat_version,p_log_file,&load_file_one_time);//////a line //////////// a line
			write_one(g_tstat_id,16,159);/////
			write_one(g_tstat_id,184,0);//184 register,to no restart,when you write the 185,118,121,128 register
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
	}
	inf.close();
	Sleep(14000);
//	write_one(tstat_id,16,159);//////
#endif


#endif 
}

void save_schedule_2_file(TCHAR* fn,int schedule_id)
{
	CStdioFile default_file;
	if(default_file.Open(fn,CFile::modeCreate | CFile::modeWrite)!=0)
	{
		//default_file.WriteString(_T("NET WORK\n"));
		default_file.WriteString(_T("NET Controller config file\n"));
		default_file.WriteString(_T("Model : NC \n"));
		int nVersionLo = Read_One(g_tstat_id, 4);
		int	nVersionHi = Read_One(g_tstat_id, 5);
		int firmwareVersion = float(nVersionHi) + float(nVersionLo/100.0);
		CString strFirmware;
		strFirmware.Format(_T("%0.2f"), firmwareVersion);
		default_file.WriteString(_T("version : ")+strFirmware+_T("\n"));
		weekly_write(default_file,schedule_id);
		annual_write(default_file,schedule_id);
		configure_write(default_file,schedule_id);
		variable_write(default_file,schedule_id);

		/////////////////////////////////////////////last work
		default_file.Flush();
		default_file.Close();
	}
	else
		AfxMessageBox(_T("Open file failure!"));
	
}
void variable_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	a_line=_T("VARIABLE LIST\n");
	default_file.WriteString(a_line.GetString());
	unsigned short varliable[NET_WORK_CONTROLLER_NUM+1]={0};
	Read_Multi(schedule_id,varliable,106,NET_WORK_CONTROLLER_NUM+1);
	a_line.Format(_T("\t%d"),varliable[0]);
	a_line=NET_WORK_CONTROLLER[0]+a_line+_T("\n");
	default_file.WriteString(a_line);



	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[5],varliable[6],varliable[7],varliable[8]);
	a_line=NET_WORK_CONTROLLER[2]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[9],varliable[10],varliable[11],varliable[12]);
	a_line=NET_WORK_CONTROLLER[3]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[13]);
	a_line=NET_WORK_CONTROLLER[4]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[14]);
	a_line=NET_WORK_CONTROLLER[5]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[21]);
	a_line=NET_WORK_CONTROLLER[6]+a_line+_T("\n");////////Inactivity Timeout
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[15],varliable[16],varliable[17],varliable[18]);
	a_line=NET_WORK_CONTROLLER[7]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[19]);
	a_line=NET_WORK_CONTROLLER[8]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[20]);
	a_line=NET_WORK_CONTROLLER[9]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[23]);
	a_line=NET_WORK_CONTROLLER[10]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[24]);
	a_line=NET_WORK_CONTROLLER[11]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);	


	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[1],varliable[2],varliable[3],varliable[4]);
	a_line=NET_WORK_CONTROLLER[1]+a_line+_T("\n");
	default_file.WriteString(a_line);

}




void weekly_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("WEEKLY ROUTINES\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tFULL_LABEL\t\tA/M\tOutput\tHoliday1 State1\tHoliday2 State2\tLabel\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[WR_DESCRIPTION_SIZE]={0};
	memset(&temp_buffer[0],0,sizeof(temp_buffer));
	

	description temp_description;	
	//TCHAR* p=new TCHAR;
	//p=(TCHAR *)&temp_description;///////////////*****pointer   attention
	unsigned char* p = reinterpret_cast<unsigned char*>(&temp_description);

	for(i=0;i<MAX_WR;i++)
	{
		a_line=_T("");//clear
		Read_Multi(schedule_id,temp_buffer,MODBUS_WR_DESCRIP_FIRST+i*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<WR_DESCRIPTION_SIZE;itt++)
				temp_buffer[itt]=0;
		}			

		//memset(&temp_description,0,sizeof(description));
		memset(p,0,sizeof(description));

		for(int j=0;j<WR_DESCRIPTION_SIZE;j++)
		{
			p[j] =(unsigned char) temp_buffer[j];
			//(*(p++))=temp_buffer[j];
		}

		CString a_m,output,state1,state2;
		if(temp_description.flag & 128)
			a_m=_T("MAN");
		else
			a_m=_T("AUTO");
		if(temp_description.flag & 64)
			output=_T("ON");
		else
			output=_T("OFF");
		if(temp_description.flag & 32)
			state1=_T("ON");
		else
			state1=_T("OFF");
		if(temp_description.flag & 16)
			state2=_T("ON");
		else
			state2=_T("OFF");			
		if(strlen(temp_description.full_label)<8)
			a_line.Format(_T("\t%s\t\t\t"),(CString)temp_description.full_label);
		else if(strlen(temp_description.full_label)<16)
			a_line.Format(_T("\t%s\t\t"),(CString)temp_description.full_label);
		else if(strlen(temp_description.full_label)<20)
			a_line.Format(_T("\t%s\t"),(CString)temp_description.full_label);
		CString temp;
		a_line=a_line+a_m+_T("\t");
		a_line=a_line+output+_T("\t");
		temp.Format(_T("%d"),temp_description.holiday1);
		a_line=a_line+temp+_T("\t");
		a_line=a_line+state1+_T("\t");
		temp.Format(_T("%d"),temp_description.holiday2);
		a_line=a_line+temp+_T("\t");
		a_line=a_line+state2+_T("\t");
		temp.Format(_T("%s\n"),(CString)temp_description.label);
		a_line=a_line+temp;
		temp.Format(_T("%d:"),i+1);
		a_line=temp+a_line;
		default_file.WriteString(a_line.GetString());////////////////write
		weekly_routines_insert_write(default_file,schedule_id,i);
	}
}






void weekly_routines_insert_write(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	CString a_line;	
	int i=0;
//	a_line="WEEKLY ROUTINES INSERT\n";
//	default_file.WriteString(a_line.GetString());
	a_line=_T("\tMon\tTue\tWed\tThu\tFri\tSat\tSun\tHoliday1Holiday2\n");
	default_file.WriteString(a_line.GetString());
	unsigned short on[72]={0};
	unsigned short off[72]={0};
//	for(i=0;i<MAX_WR;i++)
//	{
		Read_Multi(schedule_id,on,MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE*weekly_row_number,0x48);
		//由上到下，由左到右，from up to lower ,from left to right
		Read_Multi(schedule_id,off,MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE*weekly_row_number,0x48);
		//由上到下，由左到右，from up to lower ,from left to right
		for(int i=0;i<0x48;i++)
		{
			if(on[i]==255)
				on[i]=0;
			if(off[i]==255)
				off[i]=0;
		}
		for(int w=0;w<4;w++)
		{
			int itemp=w*2;
			a_line.Format(_T("ON:\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\n"),on[itemp+0*8],on[itemp+0*8+1],on[itemp+1*8],on[itemp+1*8+1],on[itemp+2*8],on[itemp+2*8+1],on[itemp+3*8],on[itemp+3*8+1],on[itemp+4*8],on[itemp+4*8+1],on[itemp+5*8],on[itemp+5*8+1],on[itemp+6*8],on[itemp+6*8+1],on[itemp+7*8],on[itemp+7*8+1],on[itemp+8*8],on[itemp+8*8+1]);
			default_file.WriteString(a_line.GetString());
			a_line.Format(_T("OFF:\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\n"),off[itemp+0*8],off[itemp+0*8+1],off[itemp+1*8],off[itemp+1*8+1],off[itemp+2*8],off[itemp+2*8+1],off[itemp+3*8],off[itemp+3*8+1],off[itemp+4*8],off[itemp+4*8+1],off[itemp+5*8],off[itemp+5*8+1],off[itemp+6*8],off[itemp+6*8+1],off[itemp+7*8],off[itemp+7*8+1],off[itemp+8*8],off[itemp+8*8+1]);
			default_file.WriteString(a_line.GetString());
		}
		default_file.WriteString(_T("\n"));
//	}
}


void annual_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("ANNUAL ROUTINES\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tFULL_LABEL\t\tA/M\tValue\tLabel\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[AR_DESCRIPTION_SIZE]={0};
	memset(&temp_buffer[0],0,sizeof(temp_buffer));
	for(i=0;i<MAX_AR;i++)
	{
		a_line="";//clear
		Read_Multi(schedule_id,temp_buffer,MODBUS_AR_DESCRIP_FIRST+i*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<AR_DESCRIPTION_SIZE;itt++)
				temp_buffer[itt]=0;
		}
		description2 temp_description;
		memset(&temp_description,0,sizeof(description2));
		char *p;
		p=(char *)&temp_description;///////////////*****pointer   attention
		for(int j=0;j<AR_DESCRIPTION_SIZE;j++)
			(*(p++))=(char)temp_buffer[j];
		CString a_m,value;
		if(temp_description.flag & 128)
			a_m=_T("MAN");
		else
			a_m=_T("AUTO");
		if(temp_description.flag & 64)
			value=_T("ON");
		else
			value=_T("OFF");
		if(strlen(temp_description.full_label)<8)
			a_line.Format(_T("%s\t\t\t"),(CString)(temp_description.full_label));
		else if(strlen(temp_description.full_label)<16)
			a_line.Format(_T("\t%s\t\t"),(CString)(temp_description.full_label));
		else if(strlen(temp_description.full_label)<20)
			a_line.Format(_T("\t%s\t"),(CString)(temp_description.full_label));
		a_line=a_line+a_m+_T("\t");
		a_line=a_line+value+_T("\t");	
		CString temp;
		temp.Format(_T("%s\n"),(CString)(temp_description.label));
		a_line=a_line+temp;
		temp.Format(_T("%d:\t"),i+1);
		a_line=temp+a_line;
		default_file.WriteString(a_line.GetString());////////////////write
		annual_routines_insert_write(default_file,schedule_id,i);
	}
}
void annual_routines_insert_write(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	unsigned short the_days[AR_TIME_SIZE];
	CString a_line=_T("\t"),temp=_T("");	
    Read_Multi(schedule_id,the_days,MODBUS_AR_TIME_FIRST + AR_TIME_SIZE*weekly_row_number,AR_TIME_SIZE);//get from network
	for(int i=0;i<AR_TIME_SIZE;i++)
	{
		int j;
		for(j=0;j<AR_TIME_SIZE;j++)
		{
			if(the_days[j]!=255)
				break;
		}
		if(j==AR_TIME_SIZE)
		{
			for(j=0;j<AR_TIME_SIZE;j++)
				the_days[j]=0;
		}
		temp.Format(_T("%d,"),the_days[i]);
		a_line+=temp;
	}
	a_line+=_T('\n');
	default_file.WriteString(a_line.GetString());////////////////write
}
void configure_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("ID ROUTINES\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tA/M\tValue\tSchedule1\tstate1\tSchedule2\tstate2\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[ID_SIZE]={0};
	for(i=0;i<MAX_ID;i++)
	{
		a_line=_T("");//clear
		Read_Multi(schedule_id,temp_buffer,MODBUS_ID_FIRST+i*ID_SIZE,ID_SIZE);
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<ID_SIZE;itt++)
				temp_buffer[itt]=0;
		}			
		description3 temp_description;
		char *p;
		p=(char *)&temp_description;///////////////*****pointer   attention
		for(int j=0;j<ID_SIZE;j++)
			(*(p++))=(char)temp_buffer[j];
		CString state1,state2,a_m,value;
		if(temp_description.flag & 128)
			a_m=_T("MAN\t");
		else
			a_m=_T("AUTO\t");
		if(temp_description.flag & 64)
			value=_T("ON\t");
		else
			value=_T("OFF\t");
		if(temp_description.flag & 32)
			state1=_T("ON\t");
		else
			state1=_T("OFF\t");
		if(temp_description.flag & 16)
			state2=_T("ON\n");
		else
			state2=_T("OFF\n");
		a_line.Format(_T("%d:\t"),i+1);
		a_line+=a_m;
		a_line+=value;
		CString temp;		
		temp.Format(_T("%d\t\t"),temp_description.schedul1);
		a_line+=temp;
		a_line+=state1;
		temp.Format(_T("%d\t\t"),temp_description.schedul2);
		a_line+=temp;
		a_line+=state2;
		default_file.WriteString(a_line.GetString());
	}
}


#if 0
CString Show_load_file_result_message(load_file_every_step temppp,bool show_message_dialog)
{//true show message box ,false no 
	CString showing_message;
	showing_message= _T("^^^^^^^^^^Summary^^^^^^^^^^\r\n");
	if(temppp.first_step ==true)
		showing_message=showing_message+LOAD_FAILURE_ONE+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_ONE+STEP_FAILURE;
	if(temppp.second_step ==true)
		showing_message=showing_message+LOAD_FAILURE_TWO+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_TWO+STEP_FAILURE;
	if(temppp.third_step ==true)
		showing_message=showing_message+LOAD_FAILURE_THREE+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_THREE+STEP_FAILURE;
	if(temppp.thurth_step ==true)
		showing_message=showing_message+LOAD_FAILURE_FOUR+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_FOUR+STEP_FAILURE;
	if(temppp.fifth_step ==true)
		showing_message=showing_message+LOAD_FAILURE_FIVE+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_FIVE+STEP_FAILURE;
	if(temppp.sixth_step ==true)
		showing_message=showing_message+LOAD_FAILURE_SIX+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_SIX+STEP_FAILURE;
	if(temppp.seven_step==true)
		showing_message=showing_message+LOAD_FAILURE_SEVEN+STEP_FINISH;
	else
		showing_message=showing_message+LOAD_FAILURE_SEVEN+STEP_FAILURE;
	if(show_message_dialog==true)
	{
		if(temppp.seven_step==true)
			message_box_function(showing_message,_T("Finish"));
		else
			AfxMessageBox(showing_message);
	}
	return showing_message;

}

void weekly_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("WEEKLY ROUTINES\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tFULL_LABEL\t\tA/M\tOutput\tHoliday1 State1\tHoliday2 State2\tLabel\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[WR_DESCRIPTION_SIZE]={0};
	memset(&temp_buffer[0],0,sizeof(temp_buffer));
	for(i=0;i<MAX_WR;i++)
	{
		a_line=_T("");//clear
		Read_Multi(schedule_id,temp_buffer,MODBUS_WR_DESCRIP_FIRST+i*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<WR_DESCRIPTION_SIZE;itt++)
				temp_buffer[itt]=0;
		}			
		description temp_description;
		memset(&temp_description,0,sizeof(description));
		TCHAR *p=NULL;
		p=(char *)&temp_description;///////////////*****pointer   attention
		for(int j=0;j<WR_DESCRIPTION_SIZE;j++)
			(*(p++))=temp_buffer[j];
		CString a_m,output,state1,state2;
		if(temp_description.flag & 128)
			a_m=_T("MAN");
		else
			a_m=_T("AUTO");
		if(temp_description.flag & 64)
			output=_T("ON");
		else
			output=_T("OFF");
		if(temp_description.flag & 32)
			state1=_T("ON");
		else
			state1=_T("OFF");
		if(temp_description.flag & 16)
			state2=_T("ON");
		else
			state2=_T("OFF");			
		if(strlen(temp_description.full_label)<8)
			a_line.Format(_T("\t%s\t\t\t"),temp_description.full_label);
		else if(strlen(temp_description.full_label)<16)
			a_line.Format(_T("\t%s\t\t"),temp_description.full_label);
		else if(strlen(temp_description.full_label)<20)
			a_line.Format(_T("\t%s\t"),temp_description.full_label);
		CString temp;
		a_line=a_line+a_m+_T("\t");
		a_line=a_line+output+_T("\t");
		temp.Format(_T("%d"),temp_description.holiday1);
		a_line=a_line+temp+_T("\t");
		a_line=a_line+state1+_T("\t");
		temp.Format(_T("%d"),temp_description.holiday2);
		a_line=a_line+temp+_T("\t");
		a_line=a_line+state2+_T("\t");
		temp.Format(_T("%s\n"),temp_description.label);
		a_line=a_line+temp;
		temp.Format(_T("%d:"),i+1);
		a_line=temp+a_line;
		default_file.WriteString(a_line.GetString());////////////////write
		weekly_routines_insert_write(default_file,schedule_id,i);
	}
}
void weekly_routines_insert_write(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	CString a_line;	
	int i=0;
//	a_line="WEEKLY ROUTINES INSERT\n";
//	default_file.WriteString(a_line.GetString());
	a_line=_T("\tMon\tTue\tWed\tThu\tFri\tSat\tSun\tHoliday1Holiday2\n");
	default_file.WriteString(a_line.GetString());
	unsigned short on[72]={0};
	unsigned short off[72]={0};
//	for(i=0;i<MAX_WR;i++)
//	{
		Read_Multi(schedule_id,on,MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE*weekly_row_number,0x48);
		//由上到下，由左到右，from up to lower ,from left to right
		Read_Multi(schedule_id,off,MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE*weekly_row_number,0x48);
		//由上到下，由左到右，from up to lower ,from left to right
		for(int i=0;i<0x48;i++)
		{
			if(on[i]==255)
				on[i]=0;
			if(off[i]==255)
				off[i]=0;
		}
		for(int w=0;w<4;w++)
		{
			int itemp=w*2;
			a_line.Format(_T("ON:\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\n"),on[itemp+0*8],on[itemp+0*8+1],on[itemp+1*8],on[itemp+1*8+1],on[itemp+2*8],on[itemp+2*8+1],on[itemp+3*8],on[itemp+3*8+1],on[itemp+4*8],on[itemp+4*8+1],on[itemp+5*8],on[itemp+5*8+1],on[itemp+6*8],on[itemp+6*8+1],on[itemp+7*8],on[itemp+7*8+1],on[itemp+8*8],on[itemp+8*8+1]);
			default_file.WriteString(a_line.GetString());
			a_line.Format(_T("OFF:\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\t%d:%d\n"),off[itemp+0*8],off[itemp+0*8+1],off[itemp+1*8],off[itemp+1*8+1],off[itemp+2*8],off[itemp+2*8+1],off[itemp+3*8],off[itemp+3*8+1],off[itemp+4*8],off[itemp+4*8+1],off[itemp+5*8],off[itemp+5*8+1],off[itemp+6*8],off[itemp+6*8+1],off[itemp+7*8],off[itemp+7*8+1],off[itemp+8*8],off[itemp+8*8+1]);
			default_file.WriteString(a_line.GetString());
		}
		default_file.WriteString(_T("\n"));
//	}
}

void annual_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("ANNUAL ROUTINES\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tFULL_LABEL\t\tA/M\tValue\tLabel\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[AR_DESCRIPTION_SIZE]={0};
	memset(&temp_buffer[0],0,sizeof(temp_buffer));
	for(i=0;i<MAX_AR;i++)
	{
		a_line="";//clear
		Read_Multi(schedule_id,temp_buffer,MODBUS_AR_DESCRIP_FIRST+i*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<AR_DESCRIPTION_SIZE;itt++)
				temp_buffer[itt]=0;
		}			
		description2 temp_description;

		memset(&temp_description,0,sizeof(description2));
		char *p;
		p=(char *)&temp_description;///////////////*****pointer   attention
		for(int j=0;j<AR_DESCRIPTION_SIZE;j++)
			(*(p++))=temp_buffer[j];
		CString a_m,value;
		if(temp_description.flag & 128)
			a_m=_T("MAN");
		else
			a_m=_T("AUTO");
		if(temp_description.flag & 64)
			value=_T("ON");
		else
			value=_T("OFF");
		if(strlen(temp_description.full_label)<8)
			a_line.Format(_T("%s\t\t\t"),temp_description.full_label);
		else if(strlen(temp_description.full_label)<16)
			a_line.Format(_T("\t%s\t\t"),temp_description.full_label);
		else if(strlen(temp_description.full_label)<20)
			a_line.Format(_T("\t%s\t"),temp_description.full_label);
		a_line=a_line+a_m+_T("\t");
		a_line=a_line+value+_T("\t");	
		CString temp;
		temp.Format(_T("%s\n"),temp_description.label);
		a_line=a_line+temp;
		temp.Format(_T("%d:\t"),i+1);
		a_line=temp+a_line;
		default_file.WriteString(a_line.GetString());////////////////write
		annual_routines_insert_write(default_file,schedule_id,i);
	}
}
void annual_routines_insert_write(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	unsigned short the_days[AR_TIME_SIZE];
	CString a_line=_T("\t"),temp=_T("");	
    Read_Multi(schedule_id,the_days,MODBUS_AR_TIME_FIRST + AR_TIME_SIZE*weekly_row_number,AR_TIME_SIZE);//get from network
	for(int i=0;i<AR_TIME_SIZE;i++)
	{
		int j;
		for(j=0;j<AR_TIME_SIZE;j++)
		{
			if(the_days[j]!=255)
				break;
		}
		if(j==AR_TIME_SIZE)
		{
			for(j=0;j<AR_TIME_SIZE;j++)
				the_days[j]=0;
		}
		temp.Format(_T("%d,"),the_days[i]);
		a_line+=temp;
	}
	a_line+=_T('\n');
	default_file.WriteString(a_line.GetString());////////////////write
}
void configure_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("ID ROUTINES\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tA/M\tValue\tSchedule1\tstate1\tSchedule2\tstate2\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[ID_SIZE]={0};
	for(i=0;i<MAX_ID;i++)
	{
		a_line=_T("");//clear
		Read_Multi(schedule_id,temp_buffer,MODBUS_ID_FIRST+i*ID_SIZE,ID_SIZE);
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<ID_SIZE;itt++)
				temp_buffer[itt]=0;
		}			
		description3 temp_description;
		char *p;
		p=(char *)&temp_description;///////////////*****pointer   attention
		for(int j=0;j<ID_SIZE;j++)
			(*(p++))=temp_buffer[j];
		CString state1,state2,a_m,value;
		if(temp_description.flag & 128)
			a_m=_T("MAN\t");
		else
			a_m=_T("AUTO\t");
		if(temp_description.flag & 64)
			value=_T("ON\t");
		else
			value=_T("OFF\t");
		if(temp_description.flag & 32)
			state1=_T("ON\t");
		else
			state1=_T("OFF\t");
		if(temp_description.flag & 16)
			state2=_T("ON\n");
		else
			state2=_T("OFF\n");
		a_line.Format(_T("%d:\t"),i+1);
		a_line+=a_m;
		a_line+=value;
		CString temp;		
		temp.Format(_T("%d\t\t"),temp_description.schedul1);
		a_line+=temp;
		a_line+=state1;
		temp.Format(_T("%d\t\t"),temp_description.schedul2);
		a_line+=temp;
		a_line+=state2;
		default_file.WriteString(a_line.GetString());
	}
}
void variable_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	a_line=_T("VARIABLE LIST\n");
	default_file.WriteString(a_line.GetString());
	unsigned short varliable[NET_WORK_CONTROLLER_NUM+1]={0};
	Read_Multi(schedule_id,varliable,106,NET_WORK_CONTROLLER_NUM+1);
	a_line.Format(_T("\t%d"),varliable[0]);
	a_line=NET_WORK_CONTROLLER[0]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[1],varliable[2],varliable[3],varliable[4]);
	a_line=NET_WORK_CONTROLLER[1]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[5],varliable[6],varliable[7],varliable[8]);
	a_line=NET_WORK_CONTROLLER[2]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[9],varliable[10],varliable[11],varliable[12]);
	a_line=NET_WORK_CONTROLLER[3]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[13]);
	a_line=NET_WORK_CONTROLLER[4]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[14]);
	a_line=NET_WORK_CONTROLLER[5]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[21]);
	a_line=NET_WORK_CONTROLLER[6]+a_line+_T("\n");////////Inactivity Timeout
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d:%d:%d:%d"),varliable[15],varliable[16],varliable[17],varliable[18]);
	a_line=NET_WORK_CONTROLLER[7]+a_line+_T("\n");
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[19]);
	a_line=NET_WORK_CONTROLLER[8]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[20]);
	a_line=NET_WORK_CONTROLLER[9]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[23]);
	a_line=NET_WORK_CONTROLLER[10]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);
	a_line.Format(_T("\t%d"),varliable[24]);
	a_line=NET_WORK_CONTROLLER[11]+a_line+_T("\n");////////Listenning Port
	default_file.WriteString(a_line);	
}
void save_schedule_2_file(const char* fn,int schedule_id)
{
	CStdioFile default_file;
	if(default_file.Open(_T(fn),CFile::modeCreate | CFile::modeWrite)!=0)
	{
		default_file.WriteString(NET_WORK_DEFFERENT_TSTAT_FILE);
		weekly_write(default_file,schedule_id);
		annual_write(default_file,schedule_id);
		configure_write(default_file,schedule_id);
		variable_write(default_file,schedule_id);
		/////////////////////////////////////////////last work
		default_file.Flush();
		default_file.Close();
	}
	else
		AfxMessageBox(_T("Open file failure!"));
	
}

#endif
CString get_left_first_array(CString &a_line)
{
	bool found_first_char=false;
	CString return_str=_T("");
	for(int i=0;i<a_line.GetLength();i++)
	{
		if(a_line.GetAt(i) == 9/* '\t'*/ || a_line.GetAt(i) == 0x20/* ' '*/ || a_line.GetAt(i)==',' || a_line.GetAt(i)==':')
		{
			if(found_first_char==true)
				break;
			else
				continue;
		}
		return_str+=a_line.GetAt(i);
		found_first_char=true;		
	}
	a_line=a_line.Right(a_line.GetLength()-a_line.Find(return_str.GetString())-return_str.GetLength());
	return return_str;
}




void weekly_read(CStdioFile & default_file,int schedule_id, CStdioFile& logfile)
{
	CString strShowing;
	CString a_line;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################

		if(a_line.Find(_T("WEEKLY ROUTINES"))!=-1)
			continue;
		if(a_line.Find(_T("A/M"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("ANNUAL ROUTINES"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			CString strNum = a_line.Left(a_line.Find(_T(":")));
			int j=_wtoi(strNum)-1;
			unsigned char flag=0;
			get_left_first_array(a_line);//clear left number
			unsigned char temp_buffer[WR_DESCRIPTION_SIZE]={0};
			CString full_label=get_left_first_array(a_line);
			CString a_m;
			if(full_label=="MAN" || full_label=="AUTO")
			{
				a_m=full_label;
				full_label=_T("");
			}
			else
                a_m=get_left_first_array(a_line);

			CString output=get_left_first_array(a_line);
			CString holiday1=get_left_first_array(a_line);
			CString state1=get_left_first_array(a_line);
			CString holiday2=get_left_first_array(a_line);
			CString state2=get_left_first_array(a_line);
			CString label=get_left_first_array(a_line);
			int i=0;
			for(i=0;i<full_label.GetLength() && i<20;i++)
				temp_buffer[i]=(char)full_label.GetAt(i);
			for(i=20;i<20+label.GetLength() && i<28;i++)
				temp_buffer[i]=(char)label.GetAt(i-20);
			temp_buffer[28]=(char)_wtoi(holiday1);
			temp_buffer[29]=(char)_wtoi(holiday2);
			if(state1==_T("ON"))
				flag+=32;
			if(state2==_T("ON"))
				flag+=16;
			if(output==_T("ON"))
				flag+=64;
			if(a_m==_T("MAN"))
				flag+=128;
			temp_buffer[30]=flag;
			Write_Multi(schedule_id,temp_buffer,MODBUS_WR_DESCRIP_FIRST+j*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
			weekly_routines_insert_read(default_file,schedule_id,j);
		}
	}while(1);	
}

void weekly_routines_insert_read(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	CString a_line;
	do{
		if(default_file.ReadString(a_line)==false)
			return;
		if(a_line.Find(_T("Mon"))!=-1)
		{
			CString on;
			CString off;
			unsigned char on_time[72];
			unsigned char off_time[72];
			for(int w=0;w<4;w++)
			{
				if(default_file.ReadString(on)==false)
					return;
				if(default_file.ReadString(off)==false)
					return;
				get_left_first_array(on);
				get_left_first_array(off);
				int itemp=w*2;
				for(int i=0;i<9;i++)
				{
					on_time[itemp+i*8]=(char)_wtoi(get_left_first_array(on));
					on_time[itemp+i*8+1]=(char)_wtoi(get_left_first_array(on));
					off_time[itemp+i*8]=(char)_wtoi(get_left_first_array(off));
					off_time[itemp+i*8+1]=(char)_wtoi(get_left_first_array(off));
				}			
			}
			Write_Multi(schedule_id,on_time,MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE*weekly_row_number,WR_TIME_SIZE);
			Write_Multi(schedule_id,off_time,MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE*weekly_row_number,WR_TIME_SIZE);
			break;
		}
		else
			return;
	}while(1);	
}


void annual_read(CStdioFile &default_file,int schedule_id, CStdioFile& logfile)
{
	CString a_line, strShowing;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################

		if(a_line.Find(_T("A/M"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("ID ROUTINES"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			int j=((char)_wtoi(a_line))-1;
			unsigned char flag=0;
			get_left_first_array(a_line);//clear left number
			unsigned char temp_buffer[AR_DESCRIPTION_SIZE]={0};
			CString full_label=get_left_first_array(a_line);
			CString a_m;
			if(full_label==_T("MAN") || full_label==_T("AUTO"))
			{
				a_m=full_label;
				full_label=_T("");
			}
			else
                a_m=get_left_first_array(a_line);
			CString value=get_left_first_array(a_line);
			CString label=get_left_first_array(a_line);
			int i=0;
			for(i=0;i<full_label.GetLength() && i<20;i++)
				temp_buffer[i]=(char)(full_label.GetAt(i));
			for(i=20;i<20+label.GetLength() && i<28;i++)
				temp_buffer[i]=(char)(label.GetAt(i-20));
			if(value==_T("ON"))
				flag+=64;
			if(a_m==_T("MAN"))
				flag+=128;
			temp_buffer[28]=flag;
			Write_Multi(schedule_id,temp_buffer,MODBUS_AR_DESCRIP_FIRST+j*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
			annual_routines_insert_read(default_file,schedule_id,j);
		}
	}while(1);	
}
void annual_routines_insert_read(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	CString a_line;
	if(default_file.ReadString(a_line)==false)
		return;
	unsigned char the_days[AR_TIME_SIZE];
	for(int i=0;i<AR_TIME_SIZE;i++)
		the_days[i]=(char)_wtoi(get_left_first_array(a_line));
	Write_Multi(schedule_id,the_days,MODBUS_AR_TIME_FIRST + AR_TIME_SIZE*weekly_row_number,AR_TIME_SIZE);
}
void configure_read(CStdioFile &default_file,int schedule_id, CStdioFile& logfile)
{
	CString a_line, strShowing;
	int i = 0;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################

		if(a_line.Find(_T("Schedule1"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("VARIABLE LIST"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			int j=(_wtoi(a_line))-1;
			unsigned char flag=0;
			get_left_first_array(a_line);//clear left number
			unsigned char temp_buffer[ID_SIZE]={0};
			CString a_m=get_left_first_array(a_line);
			CString value=get_left_first_array(a_line);
			temp_buffer[0]=(char)_wtoi(get_left_first_array(a_line));
			CString state1=get_left_first_array(a_line);
			temp_buffer[1]=(char)_wtoi(get_left_first_array(a_line));
			CString state2=get_left_first_array(a_line);			
			if(a_m==_T("MAN"))
				flag+=128;
			if(value==_T("ON"))
				flag+=64;
			if(state1==_T("ON"))
				flag+=32;
			if(state2==_T("ON"))
				flag+=16;
			temp_buffer[2]=flag;
			Write_Multi(schedule_id,temp_buffer,MODBUS_ID_FIRST+j*ID_SIZE,ID_SIZE);
			i++;
			if (i == 253)
			{
				int n = 0;
			}
		}
	}while(1);		
}
void variable_read(CStdioFile &default_file,int schedule_id, CStdioFile& logfile)
{
	CString a_line, strShowing;
	int j;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################


		if((j=a_line.Find(_T(":")))!=-1)			
		{
			if(a_line.Find(NET_WORK_CONTROLLER[0])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,106,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[1])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,107,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,108,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,109,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,110,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[2])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,111,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,112,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,113,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,114,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[3])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,115,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,116,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,117,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,118,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[4])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,119,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[5])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,120,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[6])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,127,j);				

			}
			else if(a_line.Find(NET_WORK_CONTROLLER[7])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,121,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,122,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,123,j);				
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,124,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[8])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,125,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[9])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,126,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[10])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,129,j);				
			}
			else if(a_line.Find(NET_WORK_CONTROLLER[11])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,130,j);				
			}
		}
	}while(1);	
}


void load_file_2_schedule(TCHAR* fn,int schedule_id, CStdioFile& file)
{
	CStdioFile default_file;
	if(default_file.Open((CString (fn)),CFile::modeRead)!=0)
	{
		CString a_line;
		default_file.ReadString(a_line);
		if(a_line.Find(_T("Network Controller"))==-1)
		{
			AfxMessageBox(_T("Wrong File!"));
			return ;
		}
		write_one(schedule_id,16,0xAA);
		Sleep(5);//here sleep for quick load file
		showing_text = _T("Start read weekly routine...");
		weekly_read(default_file,schedule_id, file);
		showing_text = _T("Start read weekly routine...");
 		annual_read(default_file,schedule_id, file);
 		showing_text = _T("Start read weekly routine...");
		configure_read(default_file,schedule_id, file);
		showing_text = _T("Start read weekly routine...");
		variable_read(default_file,schedule_id, file);
		/////////////////////////////////////////////last work
		default_file.Close();
	}
	else
		AfxMessageBox(_T("Open file failure!"));	
}


void Write_newrigister()
{

}

