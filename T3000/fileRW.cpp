#include "stdafx.h"


#include "fileRW.h"
#include "stdlib.h"
#include "malloc.h"
#include "stdarg.h"
#include "schedule.h"
#include "global_variable_extern.h"
#include "global_function.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "Global_Struct.h"
#include "define.h"
 
//#include "Weekly_Routines.h"
//#define TSTAT26_VAR_NUM 75
//#define TSTAT26_VAR_NUM 80
//#define TSTAT26_VAR_NUM 102

#define TSTAT25_VAR_NUM 90
#define TSTAT24_VAR_NUM 32
#define CO2NODE_VAR_NUM 15
#define NET_WORK_CONTROLLER_NUM 24
#define TSTAT26_VAR_NUM 163
#define TSTAT26_VAR_NUM_TSTAT67  392
#define NET_WORK_DEFFERENT_TSTAT_FILE "NET WORK\n"
#define INPUTCARD_NUM 24
#define GROUP_NUM 40
#define OUTPUTCARD_NUM_PER_INPUTCARD 20
using namespace std;

int tstat24_register_var[TSTAT24_VAR_NUM]={	118,121,128,109,110,	111,112,113,114,115,
											116,117,119,120,122,	123,124,125,126,127,
											186,187,131,132,133,	135,136,137,182,183};
int CO2NODE_register_var[CO2NODE_VAR_NUM]={	109,110,111,112,113,	123,124,125,127,128,
    129,130,131,132,133};

int tstat25_register_var[TSTAT25_VAR_NUM]={	118,121,185,128,111,	112,114,115,119,120,
											122,123,124,125,126,	127,129,186,187,131,
											132,133,135,136,137,	182,183,202,203,201,
											188,189,190,204,205,	206,207,208,209,210,
											211,213,214,570,571,	572,573,574,577,578,	
											579,580,581,582,583,	584,585,586,587,588,
											589,590,592,593,594,	595,596,597,598,599,	
											600,601,602,603,604,	605,606,607,608,609,	
											610,611,612,613,614,    615,616,617,618,619};
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
	298,299,300,
	MODBUS_OUTPUT6_FUNCTION,
	MODBUS_OUTPUT7_FUNCTION,
	570,571,	572,573,574,577,578,	
	579,580,581,582,583,	584,585,586,587,588,
	589,590,592,593,594,	595,596,597,598,599,	
	600,601,602,603,604,	605,606,607,608,609,	
	610,611,612,613,614,    615,616,617,618,619};


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
_TCHAR * LC_VARIABLE_LIST[] = {                  //attention:该数组中的值，不要有完全包含的出现
	_T("Input Filter Time:"),		//0
	_T("Relay Pulse Duration:"),		//1
	_T("Delay Between Relay Pulses:")	//2	
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
	_T("Delay OFF->ON : "),
	_T("Delay ON->OFF : ")
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
	_T("HEAT_COOL_CONFIG"),//214
	_T("VAV_Max_Supply_Setpoint"),
	_T("VAV_Min_Supply_Setpoint"),
	_T("VAV_Max_Airflow_Cooling"),
	_T("VAV_Max_Airflow_Heating"),
	_T("VAV_Min_Airflow"),
	_T("VAV_PID3_Input_Select"),
	_T("VAV_PID3_VALVE_OPERATION_TABLE_BEGIN"),
	_T("VAV_PID3_VALVE_OPER_TABLE_COOL1"),
	_T("VAV_PID3_VALVE_OPER_TABLE_COOL2"),
	_T("VAV_PID3_VALVE_OPER_TABLE_COOL3"),
	_T("VAV_PID3_VALVE_OPER_TABLE_HEAT1"),
	_T("VAV_PID3_VALVE_OPER_TABLE_HEAT2"),
	_T("VAV_PID3_VALVE_OPER_TABLE_HEAT3"),
	_T("VAV_PID3_Cooling_Deadband"),
	_T("VAV_PID3_Heating_Deadband"),
	_T("VAV_PID3_Pterm"),
	_T("VAV_PID3_Iterm"),
	_T("VAV_PID3_Heat_Stage"),
	_T("VAV_PID3_Cool_Stage"),
	_T("VAV_PID3_DIGITAL_OUTPUT_BEGIN"),
	_T("VAV_PID3_DIGITAL_OUTPUT_COOL1"),
	_T("VAV_PID3_DIGITAL_OUTPUT_COOL2"),
	_T("VAV_PID3_DIGITAL_OUTPUT_COOL3"),
	_T("VAV_PID3_DIGITAL_OUTPUT_HEAT1"),
	_T("VAV_PID3_DIGITAL_OUTPUT_HEAT2"),
	_T("VAV_PID3_DIGITAL_OUTPUT_HEAT3"),
	_T("VAV_Output6_Function"),
	_T("VAV_Output7_Function"),
	_T("VAV_Analog_Input3_Function"),
	_T("VAV_Analog_Input4_Function"),
	_T("VAV_Analog_Input5_Function"),
	_T("VAV_Analog_Input6_Function"),
	_T("VAV_Analog_Input7_Function"),
	_T("VAV_PID3_VALVE_OFF_TABLE_BEGIN"),
	_T("VAV_PID3_VALVE_OFF_TABLE_COOL1"),
	_T("VAV_PID3_VALVE_OFF_TABLE_COOL2"),
	_T("VAV_PID3_VALVE_OFF_TABLE_COOL3"),
	_T("VAV_PID3_VALVE_OFF_TABLE_HEAT1"),
	_T("VAV_PID3_VALVE_OFF_TABLE_HEAT2"),
	_T("VAV_PID3_VALVE_OFF_TABLE_HEAT3"),
	_T("VAV_PID3_OFF_OUTPUT_BEGIN"),
	_T("VAV_PID3_OFF_OUTPUT_COOL1"),
	_T("VAV_PID3_OFF_OUTPUT_COOL2"),
	_T("VAV_PID3_OFF_OUTPUT_COOL3"),
	_T("VAV_PID3_OFF_OUTPUT_HEAT1"),
	_T("VAV_PID3_OFF_OUTPUT_HEAT2"),
	_T("VAV_PID3_OFF_OUTPUT_HEAT3"),
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
	_T("Heating Stages  For PID1"),
	_T("Cooling Stages  For PID1"),
	_T("Heating Stages  For PID2"),
	_T("Cooling Stages  For PID2"),

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
	_T("Output Function 6"),
	_T("Output Function 7"),
	_T("VAV_Max_Supply_Setpoint"),
	_T("VAV_Min_Supply_Setpoint"),
	_T("VAV_Max_Airflow_Cooling"),
	_T("VAV_Max_Airflow_Heating"),
	_T("VAV_Min_Airflow"),
	_T("VAV_PID3_Input_Select"),
	_T("VAV_PID3_VALVE_OPERATION_TABLE_BEGIN"),
	_T("VAV_PID3_VALVE_OPER_TABLE_COOL1"),
	_T("VAV_PID3_VALVE_OPER_TABLE_COOL2"),
	_T("VAV_PID3_VALVE_OPER_TABLE_COOL3"),
	_T("VAV_PID3_VALVE_OPER_TABLE_HEAT1"),
	_T("VAV_PID3_VALVE_OPER_TABLE_HEAT2"),
	_T("VAV_PID3_VALVE_OPER_TABLE_HEAT3"),
	_T("VAV_PID3_Cooling_Deadband"),
	_T("VAV_PID3_Heating_Deadband"),
	_T("VAV_PID3_Pterm"),
	_T("VAV_PID3_Iterm"),
	_T("VAV_PID3_Heat_Stage"),
	_T("VAV_PID3_Cool_Stage"),
	_T("VAV_PID3_DIGITAL_OUTPUT_BEGIN"),
	_T("VAV_PID3_DIGITAL_OUTPUT_COOL1"),
	_T("VAV_PID3_DIGITAL_OUTPUT_COOL2"),
	_T("VAV_PID3_DIGITAL_OUTPUT_COOL3"),
	_T("VAV_PID3_DIGITAL_OUTPUT_HEAT1"),
	_T("VAV_PID3_DIGITAL_OUTPUT_HEAT2"),
	_T("VAV_PID3_DIGITAL_OUTPUT_HEAT3"),
	_T("VAV_Output6_Function"),
	_T("VAV_Output7_Function"),
	_T("VAV_Analog_Input3_Function"),
	_T("VAV_Analog_Input4_Function"),
	_T("VAV_Analog_Input5_Function"),
	_T("VAV_Analog_Input6_Function"),
	_T("VAV_Analog_Input7_Function"),
	_T("VAV_PID3_VALVE_OFF_TABLE_BEGIN"),
	_T("VAV_PID3_VALVE_OFF_TABLE_COOL1"),
	_T("VAV_PID3_VALVE_OFF_TABLE_COOL2"),
	_T("VAV_PID3_VALVE_OFF_TABLE_COOL3"),
	_T("VAV_PID3_VALVE_OFF_TABLE_HEAT1"),
	_T("VAV_PID3_VALVE_OFF_TABLE_HEAT2"),
	_T("VAV_PID3_VALVE_OFF_TABLE_HEAT3"),
	_T("VAV_PID3_OFF_OUTPUT_BEGIN"),
	_T("VAV_PID3_OFF_OUTPUT_COOL1"),
	_T("VAV_PID3_OFF_OUTPUT_COOL2"),
	_T("VAV_PID3_OFF_OUTPUT_COOL3"),
	_T("VAV_PID3_OFF_OUTPUT_HEAT1"),
	_T("VAV_PID3_OFF_OUTPUT_HEAT2"),
	_T("VAV_PID3_OFF_OUTPUT_HEAT3"),

};
_TCHAR * TSTATVAR_CONST_67[] = {                  //attention:该数组中的值，不要有完全包含的出现
	_T("COOL HEAT MODE"),																																																																																						
	//_T("MODE OPERATION"),																																																																																						
	_T("SEQUENCE"),																																																																																						
	_T("DEGC OR F"),																																																																																						
	_T("FAN MODE"),																																																																																						
	_T("POWERUP MODE"),																																																																																						
	_T("AUTO ONLY"),																																																																																						
	//_T("FACTORY DEFAULTS"),																																																																																						
	//_T("INFO BYTE"),																																																																																						
	//_T("BAUDRATE"),																																																																																						
	_T("OVERRIDE TIMER"),																																																																																						
	_T("OVERRIDE TIMER LEFT"),																																																																																						
	_T("HEAT COOL CONFIG"),																																																																																						
	_T("TIMER ON"),																																																																																						
	_T("TIMER OFF"),																																																																																						
	_T("TIMER UNITS"),																																																																																						
	_T("DEAD MASTER"),																																																																																						
	_T("SYSTEM TIME FORMAT"),																																																																																						
	_T("FREE COOL CONFIG"),																																																																																						
	_T("RS485 MODE"),																																																																																						
	_T("ANALOG1 RANGE"),																																																																																						
	_T("ANALOG2 RANGE"),																																																																																						
	_T("ANALOG3 RANGE"),																																																																																						
	_T("ANALOG4 RANGE"),																																																																																						
	_T("ANALOG5 RANGE"),																																																																																						
	_T("ANALOG6 RANGE"),																																																																																						
	_T("ANALOG7 RANGE"),																																																																																						
	_T("ANALOG8 RANGE"),																																																																																						
	_T("INPUT MANU ENABLE"),																																																																																						
	_T("FILTER"),																																																																																						
	_T("INPUT1 FILTER"),																																																																																						
	_T("INPUT2 FILTER"),																																																																																						
	_T("INPUT3 FILTER"),																																																																																						
	_T("INPUT4 FILTER"),																																																																																						
	_T("INPUT5 FILTER"),																																																																																						
	_T("INPUT6 FILTER"),																																																																																						
	_T("INPUT7 FILTER"),																																																																																						
	_T("INPUT8 FILTER"),																																																																																						
	_T("EX SENSOR1 FILTER"),																																																																																						
	_T("EX SENSOR2 FILTER"),																																																																																						
	_T("ANALOG1 FUNCTION"),																																																																																						
	_T("ANALOG2 FUNCTION"),																																																																																						
	_T("ANALOG3 FUNCTION"),																																																																																						
	_T("ANALOG4 FUNCTION"),																																																																																						
	_T("ANALOG5 FUNCTION"),																																																																																						
	_T("ANALOG6 FUNCTION"),																																																																																						
	_T("ANALOG7 FUNCTION"),																																																																																						
	_T("ANALOG8 FUNCTION"),																																																																																						
	_T("TABLE1 ZERO"),																																																																																						
	_T("TABLE1 HALFONE"),																																																																																						
	_T("TABLE1 ONE"),																																																																																						
	_T("TABLE1 HALFTWO"),																																																																																						
	_T("TABLE1 TWO"),																																																																																						
	_T("TABLE1 HALFTHREE"),																																																																																						
	_T("TABLE1 THREE"),																																																																																						
	_T("TABLE1 HALFFOUR"),																																																																																						
	_T("TABLE1 FOUR"),																																																																																						
	_T("TABLE1 HALFFIVE"),																																																																																						
	_T("TABLE1 FIVE"),																																																																																						
	_T("TABLE2 ZERO"),																																																																																						
	_T("TABLE2 HALFONE"),																																																																																						
	_T("TABLE2 ONE"),																																																																																						
	_T("TABLE2 HALFTWO"),																																																																																						
	_T("TABLE2 TWO"),																																																																																						
	_T("TABLE2 HALFTHREE"),																																																																																						
	_T("TABLE2 THREE"),																																																																																						
	_T("TABLE2 HALFFOUR"),																																																																																						
	_T("TABLE2 FOUR"),																																																																																						
	_T("TABLE2 HALFFIVE"),																																																																																						
	_T("TABLE2 FIVE"),																																																																																						
	_T("HUM CURRENT DEFAULT"),																																																																																						
	_T("MODE OUTPUT1"),																																																																																						
	_T("MODE OUTPUT2"),																																																																																						
	_T("MODE OUTPUT3"),																																																																																						
	_T("MODE OUTPUT4"),																																																																																						
	_T("MODE OUTPUT5"),																																																																																						
	_T("OUTPUT1 SCALE"),																																																																																						
	_T("OUTPUT2 SCALE"),																																																																																						
	_T("DAC OFFSET"),																																																																																						
	_T("OUTPUT1 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT2 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT3 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT4 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT5 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT6 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT7 DELAY OFF TO ON"),																																																																																						
	_T("OUTPUT1 DELAY ON TO OFF"),																																																																																						
	_T("OUTPUT2 DELAY ON TO OFF"),																																																																																						
	_T("OUTPUT3 DELAY ON TO OFF"),																																																																																						
	_T("OUTPUT4 DELAY ON TO OFF"),																																																																																						
	_T("OUTPUT5 DELAY ON TO OFF"),																																																																																						
	_T("OUTPUT6 DELAY ON TO OFF"),																																																																																						
	_T("OUTPUT7 DELAY ON TO OFF"),																																																																																						
	_T("CYCLING DELAY"),																																																																																						
	_T("CHANGOVER DELAY"),																																																																																						
	_T("VALVE TRAVEL TIME"),																																																																																						
	_T("INTERLOCK OUTPUT1"),																																																																																						
	_T("INTERLOCK OUTPUT2"),																																																																																						
	_T("INTERLOCK OUTPUT3"),																																																																																						
	_T("INTERLOCK OUTPUT4"),																																																																																						
	_T("INTERLOCK OUTPUT5"),																																																																																						
	_T("INTERLOCK OUTPUT6"),																																																																																						
	_T("INTERLOCK OUTPUT7"),																																																																																						
	_T("FREEZE DELAY ON"),																																																																																						
	_T("FREEZE DELAY OFF"),																																																																																						
	_T("OUTPUT MANU ENABLE"),																																																																																						
	_T("MANU RELAY1"),																																																																																						
	_T("MANU RELAY2"),																																																																																						
	_T("MANU RELAY3"),																																																																																						
	_T("MANU RELAY4"),																																																																																						
	_T("MANU RELAY5"),																																																																																						
	_T("MANUAL AO1"),																																																																																						
	_T("MANUAL AO2"),																																																																																						
	_T("DEADMASTER AUTO MANUAL"),																																																																																						
	_T("DEADMASTER OUTPUT STATE"),																																																																																						
	_T("DEADMASTER COOL OUTPUT"),																																																																																						
	_T("DEADMASTER HEAT OUTPUT"),																																																																																						
	_T("OUTPUT1 FUNCTION"),																																																																																						
	_T("OUTPUT2 FUNCTION"),																																																																																						
	_T("OUTPUT3 FUNCTION"),																																																																																						
	_T("OUTPUT4 FUNCTION"),																																																																																						
	_T("OUTPUT5 FUNCTION"),																																																																																						
	_T("OUTPUT6 FUNCTION"),																																																																																						
	_T("OUTPUT7 FUNCTION"),																																																																																						
	_T("FAN SPEED"),																																																																																						
	_T("PID OUTPUT1"),																																																																																						
	_T("PID OUTPUT2"),																																																																																						
	_T("PID OUTPUT3"),																																																																																						
	_T("PID OUTPUT4"),																																																																																						
	_T("PID OUTPUT5"),																																																																																						
	_T("PID OUTPUT6"),																																																																																						
	_T("PID OUTPUT7"),																																																																																						
	_T("UNIVERSAL OUTPUT BEGIN"),																																																																																						
	_T("UNIVERSAL OUTPUT COOL1"),																																																																																						
	_T("UNIVERSAL OUTPUT COOL2"),																																																																																						
	_T("UNIVERSAL OUTPUT COOL3"),																																																																																						
	_T("UNIVERSAL OUTPUT HEAT1"),																																																																																						
	_T("UNIVERSAL OUTPUT HEAT2"),																																																																																						
	_T("UNIVERSAL OUTPUT HEAT3"),																																																																																						
	_T("FAN0 OPER TABLE COAST"),																																																																																						
	_T("FAN0 OPER TABLE COOL1"),																																																																																						
	_T("FAN0 OPER TABLE COOL2"),																																																																																						
	_T("FAN0 OPER TABLE COOL3"),																																																																																						
	_T("FAN0 OPER TABLE HEAT1"),																																																																																						
	_T("FAN0 OPER TABLE HEAT2"),																																																																																						
	_T("FAN0 OPER TABLE HEAT3"),																																																																																						
	_T("FAN1 OPER TABLE COAST"),																																																																																						
	_T("FAN1 OPER TABLE COOL1"),																																																																																						
	_T("FAN1 OPER TABLE COOL2"),																																																																																						
	_T("FAN1 OPER TABLE COOL3"),																																																																																						
	_T("FAN1 OPER TABLE HEAT1"),																																																																																						
	_T("FAN1 OPER TABLE HEAT2"),																																																																																						
	_T("FAN1 OPER TABLE HEAT3"),																																																																																						
	_T("FAN2 OPER TABLE COAST"),																																																																																						
	_T("FAN2 OPER TABLE COOL1"),																																																																																						
	_T("FAN2 OPER TABLE COOL2"),																																																																																						
	_T("FAN2 OPER TABLE COOL3"),																																																																																						
	_T("FAN2 OPER TABLE HEAT1"),																																																																																						
	_T("FAN2 OPER TABLE HEAT2"),																																																																																						
	_T("FAN2 OPER TABLE HEAT3"),																																																																																						
	_T("FAN3 OPER TABLE COAST"),																																																																																						
	_T("FAN3 OPER TABLE COOL1"),																																																																																						
	_T("FAN3 OPER TABLE COOL2"),																																																																																						
	_T("FAN3 OPER TABLE COOL3"),																																																																																						
	_T("FAN3 OPER TABLE HEAT1"),																																																																																						
	_T("FAN3 OPER TABLE HEAT2"),																																																																																						
	_T("FAN3 OPER TABLE HEAT3"),																																																																																						
	_T("FANAUT OPER TABLE COAST"),																																																																																						
	_T("FANAUT OPER TABLE COOL1"),																																																																																						
	_T("FANAUT OPER TABLE COOL2"),																																																																																						
	_T("FANAUT OPER TABLE COOL3"),																																																																																						
	_T("FANAUT OPER TABLE HEAT1"),																																																																																						
	_T("FANAUT OPER TABLE HEAT2"),																																																																																						
	_T("FANAUT OPER TABLE HEAT3"),																																																																																						
	_T("VALVE OPERATION TABLE BEGIN"),																																																																																						
	_T("VALVE OPER TABLE COOL1"),																																																																																						
	_T("VALVE OPER TABLE COOL2"),																																																																																						
	_T("VALVE OPER TABLE COOL3"),																																																																																						
	_T("VALVE OPER TABLE HEAT1"),																																																																																						
	_T("VALVE OPER TABLE HEAT2"),																																																																																						
	_T("VALVE OPER TABLE HEAT3"),																																																																																						
	_T("HEAT UNIVERSAL TABLE"),																																																																																						
	_T("COOL UNIVERSAL TABLE"),																																																																																						
	_T("HEAT ORIGINAL TABLE"),																																																																																						
	_T("COOL ORIGINAL TABLE"),																																																																																						
	_T("VALVE OFF TABLE COAST"),																																																																																						
	_T("VALVE OFF TABLE COOL1"),																																																																																						
	_T("VALVE OFF TABLE COOL2"),																																																																																						
	_T("VALVE OFF TABLE COOL3"),																																																																																						
	_T("VALVE OFF TABLE HEAT1"),																																																																																						
	_T("VALVE OFF TABLE HEAT2"),																																																																																						
	_T("VALVE OFF TABLE HEAT3"),																																																																																						
	_T("DEFAULT SETPOINT"),																																																																																						
	_T("SETPOINT CONTROL"),																																																																																						
	_T("DAYSETPOINT OPTION"),																																																																																						
	_T("MIDDLE SETPOINT"),																																																																																						
	_T("DAY SETPOINT"),																																																																																						
	_T("DAY COOLING DEADBAND"),																																																																																						
	_T("DAY HEATING DEADBAND"),																																																																																						
	_T("DAY COOLING SETPOINT"),																																																																																						
	_T("DAY HEATING SETPOINT"),																																																																																						
	_T("NIGHT SETPOINT"),																																																																																						
	_T("APPLICATION"),																																																																																						
	_T("NIGHT HEATING DEADBAND"),																																																																																						
	_T("NIGHT COOLING DEADBAND"),																																																																																						
	_T("NIGHT HEATING SETPOINT"),																																																																																						
	_T("NIGHT COOLING SETPOINT"),																																																																																						
	_T("WINDOW INTERLOCK COOLING SETPOINT"),																																																																																						
	_T("WINDOW INTERLOCK HEATING SETPOINT"),																																																																																						
	_T("UNIVERSAL NIGHTSET"),																																																																																						
	_T("UNIVERSAL SET"),																																																																																						
	_T("UNIVERSAL DB HI"),																																																																																						
	_T("UNIVERSAL DB LO"),																																																																																						
	_T("ECOMONY COOLING SETPOINT"),																																																																																						
	_T("ECOMONY HEATING SETPOINT"),																																																																																						
	_T("POWERUP SETPOINT"),																																																																																						
	_T("MAX SETPOINT"),																																																																																						
	_T("MIN SETPOINT"),																																																																																						
	_T("MAX SETPOINT2"),																																																																																						
	_T("MIN SETPOINT2"),																																																																																						
	_T("MAX SETPOINT3"),																																																																																						
	_T("MIN SETPOINT3"),																																																																																						
	_T("MAX SETPOINT4"),																																																																																						
	_T("MIN SETPOINT4"),																																																																																						
	_T("SETPOINT INCREASE"),																																																																																						
	_T("FREEZE TEMP SETPOINT"),																																																																																						
	_T("WALL SETPOINT"),																																																																																						
	_T("CEILING SETPOINT"),																																																																																						
	_T("AVERAGE SETPOINT"),																																																																																						
	_T("UNOCCUPIED HEATING"),																																																																																						
	_T("UNOCCUPIED COOLING"),																																																																																						
	_T("RH SETPOINT"),																																																																																						
	_T("CURRENT1 SETPOINT"),																																																																																						
	_T("TEMP SELECT"),																																																																																						
	_T("INPUT1 SELECT"),																																																																																						
	_T("COOLING PTERM"),																																																																																						
	_T("COOLING ITERM"),																																																																																						
	_T("UNIVERSAL PTERM"),
	_T("UNIVERSAL ITERM"),
	_T("UNITS1 HIGH"),																																																																																						
	_T("UNITS1 LOW"),																																																																																						
	_T("UNITS2 HIGH"),																																																																																						
	_T("UNITS2 LOW"),																																																																																						
	_T("PID2 MODE OPERATION"),																																																																																						
	_T("KEYPAD SELECT"),																																																																																						
	_T("SPECIAL MENU LOCK"),																																																																																						
	_T("DISPLAY"),																																																																																						
	_T("ICON"),																																																																																						
	_T("KEYPAD VALUE"),																																																																																						
	_T("DISPLAY DIGITAL"),																																																																																						
	_T("DISPLAY STATUS"),																																																																																						
	_T("ROUND DISPLAY"),																																																																																						
	_T("MIN ADDRESS"),																																																																																						
	_T("MAX ADDRESS"),																																																																																						
	_T("EEPROM SIZE"),																																																																																						
	_T("TIMER SELECT"),																																																																																						
	_T("DIAGNOSTIC ALARM"),																																																																																						
	_T("DAY1 EVENT1 HI"),																																																																																						
	_T("DAY1 EVENT1 LO"),																																																																																						
	_T("DAY1 EVENT2 HI"),																																																																																						
	_T("DAY1 EVENT2 LO"),																																																																																						
	_T("DAY1 EVENT3 HI"),																																																																																						
	_T("DAY1 EVENT3 LO"),																																																																																						
	_T("DAY1 EVENT4 HI"),																																																																																						
	_T("DAY1 EVENT4 LO"),																																																																																						
	_T("DAY2 EVENT1 HI"),																																																																																						
	_T("DAY2 EVENT1 LO"),																																																																																						
	_T("DAY2 EVENT2 HI"),																																																																																						
	_T("DAY2 EVENT2 LO"),																																																																																						
	_T("DAY2 EVENT3 HI"),																																																																																						
	_T("DAY2 EVENT3 LO"),																																																																																						
	_T("DAY2 EVENT4 HI"),																																																																																						
	_T("DAY2 EVENT4 LO"),																																																																																						
	_T("LCD TURN OFF"),																																																																																						
	_T("LCD SCREEN1"),																																																																																						
	_T("LCD SCREEN2"),																																																																																						
	_T("DEMAND RESPONSE"),																																																																																						
	_T("LOCK REGISTER"),																																																																																						
	_T("PIR STAGE"),																																																																																						
	_T("FIRST CAL FLAG"),	
	_T("HUM CAL EREASE"),	
	_T("LCD ROTATE ENABLE"),																																																																																						
	_T("SCHEDULE ON OFF"),																																																																																						
	_T("DISP ITEM TEMPERATURE"),																																																																																						
	_T("DISP ITEM SETPOINT"),																																																																																						
	_T("DISP ITEM AI1"),																																																																																						
	_T("DISP ITEM AI2"),																																																																																						
	_T("DISP ITEM AI3"),																																																																																						
	_T("DISP ITEM AI4"),																																																																																						
	_T("DISP ITEM AI5"),																																																																																						
	_T("DISP ITEM AI6"),																																																																																						
	_T("DISP ITEM AI7"),																																																																																						
	_T("DISP ITEM AI8"),																																																																																						
	_T("DISP ITEM MODE"),																																																																																						
	_T("DISP ITEM USER INFO"),																																																																																						
	_T("DISP ITEM CLOCK DATE"),																																																																																						
	_T("DISP ITEM CLOCK TIME"),																																																																																						
	_T("DISP ITEM EXTENAL SENSOR1"),																																																																																						
	_T("DISP ITEM EXTENAL SENSOR2"),																																																																																						
	_T("DISP ITEM EXTENAL SENSOR3"),																																																																																						
	_T("DISP ITEM EXTENAL SENSOR4"),																																																																																						
	_T("DISP ITEM OUT1"),																																																																																						
	_T("DISP ITEM OUT2"),																																																																																						
	_T("DISP ITEM OUT3"),																																																																																						
	_T("DISP ITEM OUT4"),																																																																																						
	_T("DISP ITEM OUT5"),																																																																																						
	_T("DISP ITEM OUT6"),																																																																																						
	_T("DISP ITEM OUT7"),																																																																																						
	_T("OUTPUT PWM AUTO COAST"),																																																																																						
	_T("OUTPUT PWM AUTO COOL1"),																																																																																						
	_T("OUTPUT PWM AUTO COOL2"),																																																																																						
	_T("OUTPUT PWM AUTO COOL3"),																																																																																						
	_T("OUTPUT PWM AUTO HEAT1"),																																																																																						
	_T("OUTPUT PWM AUTO HEAT2"),																																																																																						
	_T("OUTPUT PWM AUTO HEAT3"),																																																																																						
	_T("SUN ICON CONTROL"),																																																																																						
	_T("MOON ICON CONTROL"),																																																																																						
	_T("EXT SENSOR TEMP CAL"),																																																																																						
	_T("HUM HEATER CONTROL ENABLE"),																																																																																						
	_T("HUM INPUT MANUAL ENABLE"),																																																																																						
	_T("HUM INPUT MANUAL VALUE"),																																																																																						
	_T("CO2 INPUT MANUAL ENABLE"),																																																																																						
	_T("CO2 INPUT VALUE"),																																																																																						
	_T("CO2 CALIBRATION DATA"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT BEGIN"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT COOL1"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT COOL2"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT COOL3"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT HEAT1"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT HEAT2"),																																																																																						
	_T("UNIVERSAL OFF OUTPUT HEAT3"),																																																																																						
	_T("UNIVERSAL OFF VALVE BEGIN"),																																																																																						
	_T("UNIVERSAL OFF VALVE COOL1"),																																																																																						
	_T("UNIVERSAL OFF VALVE COOL2"),																																																																																						
	_T("UNIVERSAL OFF VALVE COOL3"),																																																																																						
	_T("UNIVERSAL OFF VALVE HEAT1"),																																																																																						
	_T("UNIVERSAL OFF VALVE HEAT2"),																																																																																						
	_T("UNIVERSAL OFF VALVE HEAT3"),																																																																																						
	_T("VALUE SENSOR"),																																																																																						
	_T("PIR SELECT ENABLE"),																																																																																						
	_T("PIR REAL VALUE"),																																																																																						
	_T("PIR OCCUPIED"),																																																																																						
	_T("UNIVERSAL VALVE BEGIN"),																																																																																						
	_T("UNIVERSAL VALVE COOL1"),																																																																																						
	_T("UNIVERSAL VALVE COOL2"),																																																																																						
	_T("UNIVERSAL VALVE COOL3"),																																																																																						
	_T("UNIVERSAL VALVE HEAT1"),																																																																																						
	_T("UNIVERSAL VALVE HEAT2"),																																																																																						
	_T("UNIVERSAL VALVE HEAT3"),																																																																																						
	_T("PIR TIMER"),
	_T("	 SUPPLY SETPOINT	")	,
	_T("	 MAX SUPPLY SETPOINT	")	,
	_T("	 MIN SUPPLY SETPOINT	")	,
	_T("	 MAX AIRFLOW COOLING	")	,
	_T("	 MAX AIRFLOW HEATING	")	,
	_T("	 MIN AIRFLOW	")	,
	_T("	 AIRFLOW SETPOINT   	")	,
	_T("	 VAV CONTROL	")	,
	_T("	 PID3 VALVE OPERATION TABLE BEGIN	")	,
	_T("	 PID3 VALVE OPER TABLE COOL1	")	,
	_T("	 PID3 VALVE OPER TABLE COOL2	")	,
	_T("	 PID3 VALVE OPER TABLE COOL3	")	,
	_T("	 PID3 VALVE OPER TABLE HEAT1	")	,
	_T("	 PID3 VALVE OPER TABLE HEAT2	")	,
	_T("	 PID3 VALVE OPER TABLE HEAT3	")	,
	_T("	 PID3 COOLING DB	")	,
	_T("	 PID3 HEATING DB	")	,
	_T("	 PID3 PTERM	")	,
	_T("	 PID3 ITERM	")	,
	_T("	 PID3 HEAT STAGE	")	,
	_T("	 PID3 COOL STAGE	")	,
	_T("	 PID3 OUTPUT 	")	,
	_T("	 PID3 OUTPUT BEGIN	")	,
	_T("	 PID3 OUTPUT COOL1 	")	,
	_T("	 PID3 OUTPUT COOL2 	")	,
	_T("	 PID3 OUTPUT COOL3 	")	,
	_T("	 PID3 OUTPUT HEAT1   	")	,
	_T("	 PID3 OUTPUT HEAT2 	")	,
	_T("	 PID3 OUTPUT HEAT3 	")	,
	_T("	 PID3 VALVE OFF TABLE BEGIN	")	,
	_T("	 PID3 VALVE OFF TABLE COOL1	")	,
	_T("	 PID3 VALVE OFF TABLE COOL2	")	,
	_T("	 PID3 VALVE OFF TABLE COOL3	")	,
	_T("	 PID3 VALVE OFF TABLE HEAT1	")	,
	_T("	 PID3 VALVE OFF TABLE HEAT2	")	,
	_T("	 PID3 VALVE OFF TABLE HEAT3  	")	,
	_T("	 PID3 OFF OUTPUT BEGIN  	")	,
	_T("	 PID3 OFF OUTPUT COOL1 	")	,
	_T("	 PID3 OFF OUTPUT COOL2 	")	,
	_T("	 PID3 OFF OUTPUT COOL3 	")	,
	_T("	 PID3 OFF OUTPUT HEAT1   	")	,
	_T("	 PID3 OFF OUTPUT HEAT2 	")	,
	_T("	 PID3 OFF OUTPUT HEAT3 	")	,
    _T("     MODBUS_HEAT_COOL_MODE  ")  ,
    _T("     MODBUS_PID_D_TERM  "),
    _T("     MODBUS_PID_SAMPLE_TIME  ")
};
_TCHAR * CO2NODEVAR_CONST_25[] = {                  //attention:该数组中的值，不要有完全包含的出现
    _T("CO2 Calibration Offset"),		
    _T("Delta Value"),                   
    _T("Filter Times"),                    
    _T("Fair Alarm PPM Setpoint"),  //128             
    _T("Poor Alarm PPM Setpoint"),                 

    _T("Offset Thermistor Sensor ") ,     
    _T("Select Temperatre Direct To Analog Output"),		          
    _T("Temperature Unit "),
    _T("Analog Output Mode"),        
    _T("The Min Value Temperature Output"),        

    _T("The Max Value Temperature Output"),                    
    _T("The Min Value Humidity Output"),      
    _T("The Max Value Humidity Output"),      
    _T("The Min Value CO2 Output"),                 
    _T("The Max Value CO2 Output")            

}  ;
_TCHAR *STRINGCONST[] ={_T("Off"),_T("On"),_T("Closed"),_T("Open"),_T("0-100"),_T("50-100"),_T("0-50")};

bool string2digital(CString str,unsigned char & val ,bool & type )   // type false = digital ,true = analog
{
	unsigned char SN = 255;
	for(int i=0;i<7;i++)
//	for(int i=0;i<5;i++)
	{
		if(str.CompareNoCase(STRINGCONST[i]) == 0)        // equal
		{
			SN = i ;
			break;
		}
	}
	switch(SN)
	{
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

	
		unsigned long serial = product_register_value[MODBUS_SERIALNUMBER_LOWORD]+
							product_register_value[MODBUS_SERIALNUMBER_LOWORD+1] * 256 +
							product_register_value[MODBUS_SERIALNUMBER_HIWORD] * 65536 +           // 256 * 256 
							product_register_value[MODBUS_SERIALNUMBER_HIWORD+1]* 16777216 ;// 256 *256 *256
		//write_to_file_a_line(out,"%-30s%ld","serialnumber:",serial);
		_Twrite_to_file_a_line(out,_T("%-30s%ld"),_T("serialnumber:"),serial);
		

		
}
void WriteAddress(wofstream & out) 
{				
	//write_to_file_a_line(out,"%-30s%ld","address:",read_one(g_tstat_id,6));	
	_Twrite_to_file_a_line(out,_T("%-30s%ld"),_T("address:"),product_register_value[MODBUS_ADDRESS]);	
}
void write_to_file_a_line(wofstream & out,const char * format, ... )
{
	/*
	va_list marker;
	va_start( marker, format );
	char buf[2048];	
	vsprintf( buf, format, marker );
	va_end(marker);
	out<< buf << endl ;

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
	int fan_register[5]={MODBUS_FAN0_OPER_TABLE_COAST,MODBUS_FAN1_OPER_TABLE_COAST,MODBUS_FAN2_OPER_TABLE_COAST,MODBUS_FAN3_OPER_TABLE_COAST,MODBUS_FANAUT_OPER_TABLE_COAST};//*************************************
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
	int fan_speed=product_register_value[MODBUS_FAN_MODE];//1,2,3
	if(product_register_value[MODBUS_AUTO_ONLY]==1)
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
			fan_value[col] = product_register_value[get_real_fan_address(fan_speed,j,pos)];//j
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
			fan_value[col] = product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN+pos];//j
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
		Array[col-1] = product_register_value[MODBUS_UNIVERSAL_VALVE_BEGIN+pos];//j
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




void universal_relay_write_Tstat67OFF(wofstream & out,int m_26_heat_stages,int m_26_cool_stages)//UNIVERSAL RELAY OUTPUTS vlaue
{
	_Twrite_to_file_a_line(out,_T("//   PID2 OFF TABLE"));//j
	_Twrite_to_file_a_line(out,m_26.GetString());
	int fan_value[7];
	int pos=0;
	for(int col = 0 ;col <(m_26_heat_stages+m_26_cool_stages+1);col++)
	{//get register value
		if(col < m_26_heat_stages)
			pos = m_26_heat_stages+m_26_cool_stages - col ;
		else
			pos = col - m_26_heat_stages;
		fan_value[col] = product_register_value[MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN+pos];//j
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
	_Twrite_to_file_a_line(out,_T("//   PID2 OFF TABLE"));	
}
void universal_value_setting_write_Tstat67OFF(wofstream & out,int m_26_heat_stages,int m_26_cool_stages)
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
		Array[col-1] = product_register_value[MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+pos];//j
	}
	_Twrite_to_file_a_line(out,_T("//   PID2 OFF VALVE TABLE"));
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
	_Twrite_to_file_a_line(out,_T("//   PID2 OFF VALVE TABLE END"));	
}


void universal_relay_write_Tstat67Auto(wofstream & out,int m_26_heat_stages,int m_26_cool_stages)//UNIVERSAL RELAY OUTPUTS vlaue
{
	_Twrite_to_file_a_line(out,_T("//   UNIVERSAL Auto RELAY OUTPUTS"));//j
	_Twrite_to_file_a_line(out,m_26.GetString());
	int fan_value[7];
	int pos=0;
	for(int col = 0 ;col <(m_26_heat_stages+m_26_cool_stages+1);col++)
	{//get register value
		if(col < m_26_heat_stages)
			pos = m_26_heat_stages+m_26_cool_stages - col ;
		else
			pos = col - m_26_heat_stages;
		fan_value[col] = product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN+pos];//j
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
	_Twrite_to_file_a_line(out,_T("//   UNIVERSAL AUTO END"));	
}
void universal_value_setting_write_Tstat67Auto(wofstream & out,int m_26_heat_stages,int m_26_cool_stages)
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
		Array[col-1] = product_register_value[MODBUS_UNIVERSAL_VALVE_BEGIN+pos];//j
	}
	_Twrite_to_file_a_line(out,_T("//   UNIVERSAL AUTO VALVE"));
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
	_Twrite_to_file_a_line(out,_T("//   VALVE AUTO END"));	
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
		Array[col-1] = product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos];//j
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



void delay_time_write(wofstream & out)
{   //write the delay
	//used by save2file functiojn
	int rows;
	int nFlag = product_register_value[7];
	if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)||(product_register_value[7]== PM_TSTAT7)||(product_register_value[7]== PM_TSTAT8)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
	{
	  rows=7;
	}
	else
	{
	 rows=5;
	}
	 
 
	int Array[14],row,col;
	CString str,str1;
	for(int i = 0 ;i <rows;i++)
	{
		Array[i] = product_register_value[MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i];
	}
	for(int i = 0 ;i <rows;i++)
	{
		Array[rows+i] = product_register_value[MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i];
	}
	_Twrite_to_file_a_line(out,_T("//   DELAY TIMES"));
	str=_T("//           ");
	for(col =1;col<=rows;col++)
	{
		//str1.Format("%-8d",Array[2+row*5+col]);//because michel changed ,OUTPUT_SCALE_OFF
		//because the xiao lei,position = OUTPUT_DELAY_DECREASE
		//because Michel need,position = OUTPUT_DELAY_OFF,
		//OUTPUT_DELAY_OFF-OUTPUT_DELAY_DECREASE=4
		str1.Format(_T("OUTPUT%d "),col);//4
		str+= str1;
	}
	 _Twrite_to_file_a_line(out,str);
	for(row=0;row<2;row++)
	{
		str = _T("");
		str+=(DELAY_CONST[row]);
		for(col =0;col<rows;col++)
		{
			 
			str1.Format(_T("%-8d"),Array[row*rows+col]);//4
			str+= str1;
		}
		_Twrite_to_file_a_line(out,str.GetString());
	}
	_Twrite_to_file_a_line(out,_T("//   DELAY END"));
}

void delay_time_write_Tstat67(wofstream & out)
{   //write the delay
	//used by save2file functiojn
	int nFlag = product_register_value[7];
	int rows;
	if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)||(product_register_value[7]== PM_TSTAT7)||(product_register_value[7]== PM_TSTAT8)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
	{
		rows=7;
	}
	else
	{
		rows=5;
	}


	int Array[14],row,col;
	CString str,str1;
	for(int i = 0 ;i <rows;i++)
	{
		Array[i] = product_register_value[MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i];
	}
	for(int i = 0 ;i <rows;i++)
	{
		Array[rows+i] = product_register_value[MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i];
	}
	_Twrite_to_file_a_line(out,_T("//   DELAY TIMES"));
	str=_T("//           ");
	for(col =1;col<=rows;col++)
	{
		//str1.Format("%-8d",Array[2+row*5+col]);//because michel changed ,OUTPUT_SCALE_OFF
		//because the xiao lei,position = OUTPUT_DELAY_DECREASE
		//because Michel need,position = OUTPUT_DELAY_OFF,
		//OUTPUT_DELAY_OFF-OUTPUT_DELAY_DECREASE=4
		str1.Format(_T("OUTPUT%d "),col);//4
		str+= str1;
	}
	_Twrite_to_file_a_line(out,str);
	for(row=0;row<2;row++)
	{
		str = _T("");
		str+=(DELAY_CONST[row]);
		for(col =0;col<rows;col++)
		{

			str1.Format(_T("%-8d"),Array[row*rows+col]);//4
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
		Array[i] = product_register_value[MODBUS_TABLE1_ZERO+i];
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
// 	if(version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		{
// 		short slider1_low=product_register_value[MODBUS_TABLE1_ZERO];
// 		short slider1_high=product_register_value[MODBUS_TABLE1_ZERO_HI];
// 		int m_slider1_i=slider1_high*65536+slider1_low;
// 		short slider2_low=product_register_value[MODBUS_TABLE1_HALFONE];
// 		short slider2_high=product_register_value[MODBUS_TABLE1_HALFONE_HI];
// 		int m_slider2_i=(slider2_high*65536+slider2_low);
// 		short slider3_low=product_register_value[MODBUS_TABLE1_ONE];
// 		short slider3_high=product_register_value[MODBUS_TABLE1_ONE_HI];
// 		int m_slider3_i=(slider3_high*65536+slider3_low);	
// 		short slider4_low=product_register_value[MODBUS_TABLE1_HALFTWO];
// 		short slider4_high=product_register_value[MODBUS_TABLE1_HALFTWO_HI];
// 		int m_slider4_i=(slider4_high*65536+slider4_low);
// 		short slider5_low=product_register_value[MODBUS_TABLE1_TWO];
// 		short slider5_high=product_register_value[MODBUS_TABLE1_TWO_HI];
// 		int m_slider5_i=(slider5_high*65536+slider5_low);
// 		short slider6_low=product_register_value[MODBUS_TABLE1_HALFTHREE];
// 		short slider6_high=product_register_value[MODBUS_TABLE1_HALFTHREE_HI];
// 		int m_slider6_i=(slider6_high*65536+slider6_low);
// 		short slider7_low=product_register_value[MODBUS_TABLE1_THREE];
// 		short slider7_high=product_register_value[MODBUS_TABLE1_THREE_HI];
// 		int m_slider7_i=(slider7_high*65536+slider7_low);
// 		short slider8_low=product_register_value[MODBUS_TABLE1_HALFFOUR];
// 		short slider8_high=product_register_value[MODBUS_TABLE1_HALFFOUR_HI];
// 		int m_slider8_i=(slider8_high*65536+slider8_low);
// 		short slider9_low=product_register_value[MODBUS_TABLE1_FOUR];
// 		short slider9_high=product_register_value[MODBUS_TABLE1_FOUR_HI];
// 		int m_slider9_i=(slider9_high*65536+slider9_low);
// 		short slider10_low=product_register_value[MODBUS_TABLE1_HALFFIVE];
// 		short slider10_high=product_register_value[MODBUS_TABLE1_HALFFIVE_HI];
// 		int m_slider10_i=(slider10_high*65536+slider10_low);
// 		short slider11_low=multi_register_value[MODBUS_TABLE1_FIVE];
// 		short slider11_high=multi_register_value[MODBUS_TABLE1_FIVE_HI];
// 		int m_slider11_i=(slider11_high*65536+slider11_low);
// 	_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE"));
// 	_Twrite_to_file_a_line(out,_T("//			0.0V	0.5V	1.0V	1.5V	2.0V	2.5V	3.0V	3.5V	4.0V	4.5V	5.0V"));
// 		CString str;
// 		str.Format(_T("Lookup Table 1:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
// 		m_slider1_i,m_slider2_i,m_slider3_i,m_slider4_i,m_slider5_i,m_slider6_i,
// 		m_slider7_i,m_slider8_i,m_slider9_i,m_slider10_i,m_slider11_i);
// 		_Twrite_to_file_a_line(out,str);
// 		}
// 		{
// 		short slider1_low=product_register_value[MODBUS_TABLE2_ZERO];
// 		short slider1_high=product_register_value[MODBUS_TABLE2_ZERO_HI];
// 		int m_slider1_i=slider1_high*65536+slider1_low;
// 
// 		short slider2_low=product_register_value[MODBUS_TABLE2_HALFONE];
// 		short slider2_high=product_register_value[MODBUS_TABLE2_HALFONE_HI];
// 		int m_slider2_i=(slider2_high*65536+slider2_low);
// 
// 		short slider3_low=product_register_value[MODBUS_TABLE2_ONE];
// 		short slider3_high=product_register_value[MODBUS_TABLE2_ONE_HI];
// 		int m_slider3_i=(slider3_high*65536+slider3_low);
// 
// 		short slider4_low=product_register_value[MODBUS_TABLE2_HALFTWO];
// 		short slider4_high=product_register_value[MODBUS_TABLE2_HALFTWO_HI];
// 		int m_slider4_i=(slider4_high*65536+slider4_low);
// 
// 		short slider5_low=product_register_value[MODBUS_TABLE2_TWO];
// 		short slider5_high=product_register_value[MODBUS_TABLE2_TWO_HI];
// 		int m_slider5_i=(slider5_high*65536+slider5_low);
// 
// 		short slider6_low=product_register_value[MODBUS_TABLE2_HALFTHREE];
// 		short slider6_high=product_register_value[MODBUS_TABLE2_HALFTHREE_HI];
// 		int m_slider6_i=(slider6_high*65536+slider6_low);
// 
// 		short slider7_low=product_register_value[MODBUS_TABLE2_THREE];
// 		short slider7_high=product_register_value[MODBUS_TABLE2_THREE_HI];
// 		int m_slider7_i=(slider7_high*65536+slider7_low);
// 
// 		short slider8_low=product_register_value[MODBUS_TABLE2_HALFFOUR];
// 		short slider8_high=product_register_value[MODBUS_TABLE2_HALFFOUR_HI];
// 		int m_slider8_i=(slider8_high*65536+slider8_low);
// 
// 		short slider9_low=product_register_value[MODBUS_TABLE2_FOUR];
// 		short slider9_high=product_register_value[MODBUS_TABLE2_FOUR_HI];
// 		int m_slider9_i=(slider9_high*65536+slider9_low);
// 
// 		short slider10_low=product_register_value[MODBUS_TABLE2_HALFFIVE];
// 		short slider10_high=product_register_value[MODBUS_TABLE2_HALFFIVE_HI];
// 		int m_slider10_i=(slider10_high*65536+slider10_low);
// 
// 		short slider11_low=product_register_value[MODBUS_TABLE2_FIVE];
// 		short slider11_high=product_register_value[MODBUS_TABLE2_FIVE_HI];
// 		int m_slider11_i=(slider11_high*65536+slider11_low);
// 		CString str;
// 		str.Format(_T("Lookup Table 2:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
// 		m_slider1_i,m_slider2_i,m_slider3_i,m_slider4_i,m_slider5_i,m_slider6_i,
// 		m_slider7_i,m_slider8_i,m_slider9_i,m_slider10_i,m_slider11_i);
// 		_Twrite_to_file_a_line(out,str);
// 		_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE END"));
// 		}
// //
// 	/*
// 	int Array[23];
// 	CString str,str1;
// 	for(int i = 0 ;i <22;i++)
// 		Array[i] = read_one(tstat_id,219+i);
// 	write_to_file_a_line(out,"//   LOOKUP TABLE");
// 	write_to_file_a_line(out,"//			0.0V	0.5V	1.0V	1.5V	2.0V	2.5V	3.0V	3.5V	4.0V	4.5V	5.0V");
// 	str.Format("Lookup Table 1:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d",
// 		Array[0],Array[1],Array[2],Array[3],Array[4],Array[5],
// 		Array[6],Array[7],Array[8],Array[9],Array[10]);
// 	write_to_file_a_line(out,LPCSTR(str));
// 	str.Format("Lookup Table 2:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d",
// 		Array[11],Array[12],Array[13],Array[14],Array[15],Array[16],
// 		Array[17],Array[18],Array[19],Array[20],Array[21]);
// 	write_to_file_a_line(out,LPCSTR(str));
// 	write_to_file_a_line(out,"//   LOOKUP TABLE END");
// 	*/
// 	}
		 
		 
	 
}
void lookup_table_write_Tstat67(wofstream & out)
{
	float version=get_tstat_version(g_tstat_id);
	if(version<CUSTOM_TABLE_FLOAT_VERSION)
	{
	
	int Array[23];
	CString str,str1;
	for(int i = 0 ;i <22;i++)
		Array[i] = product_register_value[MODBUS_TABLE1_ZERO+i];
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
		 
	/*	short slider1_low=product_register_value[MODBUS_TABLE1_ZERO_M];
		short slider1_high=product_register_value[MODBUS_TABLE1_HALFONE_M];
		int m_slider1_i=slider1_high*65536+slider1_low;
		short slider2_low=product_register_value[MODBUS_TABLE1_HALFONE_M];
		short slider2_high=product_register_value[MODBUS_TABLE1_HALFTWO_M];
		int m_slider2_i=(slider2_high*65536+slider2_low);
		short slider3_low=product_register_value[MODBUS_TABLE1_ONE_M];
		short slider3_high=product_register_value[MODBUS_TABLE1_HALFTHREE_M];
		int m_slider3_i=(slider3_high*65536+slider3_low);	
		short slider4_low=product_register_value[MODBUS_TABLE1_HALFTWO_M];
		short slider4_high=product_register_value[MODBUS_TABLE1_HALFFOUR_M];
		int m_slider4_i=(slider4_high*65536+slider4_low);
		short slider5_low=product_register_value[MODBUS_TABLE1_TWO_M];
		short slider5_high=product_register_value[MODBUS_TABLE1_HALFFIVE_M];
		int m_slider5_i=(slider5_high*65536+slider5_low);
		short slider6_low=product_register_value[MODBUS_TABLE1_HALFTHREE_M];
		short slider6_high=product_register_value[MODBUS_TABLE2_ZERO_M];
		int m_slider6_i=(slider6_high*65536+slider6_low);
		short slider7_low=product_register_value[MODBUS_TABLE1_THREE_M];
		short slider7_high=product_register_value[MODBUS_TABLE2_ONE_M];
		int m_slider7_i=(slider7_high*65536+slider7_low);
		short slider8_low=product_register_value[MODBUS_TABLE1_HALFFOUR_M];
		short slider8_high=product_register_value[MODBUS_TABLE2_TWO_M];
		int m_slider8_i=(slider8_high*65536+slider8_low);
		short slider9_low=product_register_value[MODBUS_TABLE1_FOUR_M];
		short slider9_high=product_register_value[MODBUS_TABLE2_THREE_M];
		int m_slider9_i=(slider9_high*65536+slider9_low);
		short slider10_low=product_register_value[MODBUS_TABLE1_HALFFIVE_M];
		short slider10_high=product_register_value[MODBUS_TABLE2_FOUR_M];
		int m_slider10_i=(slider10_high*65536+slider10_low);
		short slider11_low=multi_register_value[MODBUS_TABLE1_FIVE_M];
		short slider11_high=multi_register_value[MODBUS_TABLE2_FIVE_M];
		int m_slider11_i=(slider11_high*65536+slider11_low);
	_Twrite_to_file_a_line(out,_T("//   LOOKUP TABLE"));
	_Twrite_to_file_a_line(out,_T("//			0.0V	0.5V	1.0V	1.5V	2.0V	2.5V	3.0V	3.5V	4.0V	4.5V	5.0V"));
		CString str;
		str.Format(_T("Lookup Table 1:	\t%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d%-8d"),
		m_slider1_i,m_slider2_i,m_slider3_i,m_slider4_i,m_slider5_i,m_slider6_i,
		m_slider7_i,m_slider8_i,m_slider9_i,m_slider10_i,m_slider11_i);
		_Twrite_to_file_a_line(out,str);*/
		 
		 
	 
}

void var_write(wofstream & out)
{
	
	//used by save2file functiojn
	CString str1;
	float version=get_tstat_version(g_tstat_id);
	if (product_register_value[7]==PM_TSTAT5G)
	{
		if(version<35)
		{//24
			for(int i=0;i<TSTAT24_VAR_NUM;i++)
			{			
				str1.Format(_T("%d,\t%d,\t%s"),tstat24_register_var[i], product_register_value[tstat24_register_var[i]],TSTATVAR_CONST_24[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
					_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
		}
		else 
		{//25
			int tstat26_register_var[TSTAT26_VAR_NUM]={	118,121,185,128,111,	112,114,115,119,120,
				122,123,124,125,126,	127,129,186,187,131,
				132,133,135,136,137,	182,183,202,203,201,
				188,189,190,204,205,	206,207,208,209,210,
				211,213,214,
				241,242,243,244,245,	246,247,248,249,250,
				251,252,253,
				268,269,270,271,272,	273,274,275,MODBUS_HEAT_ORIGINAL_TABLE,MODBUS_COOL_ORIGINAL_TABLE,
				MODBUS_HEAT_UNIVERSAL_TABLE,MODBUS_COOL_UNIVERSAL_TABLE,
				286,287,288,289,290,	291,292,285,293,
				301,302,303,309,310,    328,329,330,331,332,
				327,333,319,320,321,    322,323,325,334,335,
				336,337,338,339,		327,283,284,
				341,342,343,344,345,346,347,
				298,299,300,
				MODBUS_OUTPUT6_FUNCTION,MODBUS_OUTPUT7_FUNCTION,
				570,571,	572,573,574,577,578,	
				579,580,581,582,583,	584,585,586,587,588,
				589,590,592,593,594,	595,596,597,598,599,	
				600,601,602,603,604,	605,606,607,608,609,	
				610,611,612,613,614,    615,616,617,618,619};
			for(int i=0;i<TSTAT26_VAR_NUM;i++)
			{
				str1.Format(_T("%d,\t%d,\t%s"),tstat26_register_var[i],product_register_value[tstat26_register_var[i]]  ,TSTATVAR_CONST_26[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
					_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
		}
	} 
	
    else
	{
		 
		if(version<26)
		{
			for(int i=0;i<TSTAT25_VAR_NUM;i++)
			{
				str1.Format(_T("%d,\t%d,\t%s"),tstat25_register_var[i],product_register_value[tstat25_register_var[i]]  ,TSTATVAR_CONST_25[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
					_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
		}
		else
		{
			for(int i=0;i<TSTAT26_VAR_NUM;i++)
			{
				 
			    str1.Format(_T("%d,\t%d,\t%s"),tstat26_register_var[i],product_register_value[tstat26_register_var[i]],TSTATVAR_CONST_26[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
				_Twrite_to_file_a_line(out,_T(" "));//a space line
			}
			 
		}
	}
		


}
void var_write_CO2_NODE(wofstream & out)
{

    //used by save2file functiojn
    CString str1;
   
  
            for(int i=0;i<CO2NODE_VAR_NUM;i++)
            {			
                str1.Format(_T("%d,\t%d,\t%s"),CO2NODE_register_var[i], product_register_value[CO2NODE_register_var[i]],CO2NODEVAR_CONST_25[i]);
                _Twrite_to_file_a_line(out,str1);
                if((i%5)==4)
                    _Twrite_to_file_a_line(out,_T(" "));//a space line
            }
 



}
void var_write_Tstat67(wofstream & out)
{
	int tstat67_register_var[TSTAT26_VAR_NUM_TSTAT67]={	
		MODBUS_COOL_HEAT_MODE	,
		//MODBUS_MODE_OPERATION	,
		MODBUS_SEQUENCE	,
		MODBUS_DEGC_OR_F	,
		MODBUS_FAN_MODE	,
		MODBUS_POWERUP_MODE	,
		MODBUS_AUTO_ONLY	,
		//MODBUS_FACTORY_DEFAULTS	,
		//MODBUS_INFO_BYTE	,
		//MODBUS_BAUDRATE	,
		MODBUS_OVERRIDE_TIMER	,
		MODBUS_OVERRIDE_TIMER_LEFT	,
		MODBUS_HEAT_COOL_CONFIG	,
		MODBUS_TIMER_ON	,
		MODBUS_TIMER_OFF	,
		MODBUS_TIMER_UNITS	,
		MODBUS_DEAD_MASTER	,
		MODBUS_SYSTEM_TIME_FORMAT	,
		MODBUS_FREE_COOL_CONFIG	,
		MODBUS_RS485_MODE	,
		MODBUS_ANALOG1_RANGE	,
		MODBUS_ANALOG2_RANGE	,
		MODBUS_ANALOG3_RANGE	,
		MODBUS_ANALOG4_RANGE	,
		MODBUS_ANALOG5_RANGE	,
		MODBUS_ANALOG6_RANGE	,
		MODBUS_ANALOG7_RANGE	,
		MODBUS_ANALOG8_RANGE	,
		MODBUS_INPUT_MANU_ENABLE	,
		MODBUS_FILTER	,
		MODBUS_INPUT1_FILTER	,
		MODBUS_INPUT2_FILTER	,
		MODBUS_INPUT3_FILTER	,
		MODBUS_INPUT4_FILTER	,
		MODBUS_INPUT5_FILTER	,
		MODBUS_INPUT6_FILTER	,
		MODBUS_INPUT7_FILTER	,
		MODBUS_INPUT8_FILTER	,
		MODBUS_EX_SENSOR1_FILTER	,
		MODBUS_EX_SENSOR2_FILTER	,
		MODBUS_ANALOG1_FUNCTION	,
		MODBUS_ANALOG2_FUNCTION	,
		MODBUS_ANALOG3_FUNCTION	,
		MODBUS_ANALOG4_FUNCTION	,
		MODBUS_ANALOG5_FUNCTION	,
		MODBUS_ANALOG6_FUNCTION	,
		MODBUS_ANALOG7_FUNCTION	,
		MODBUS_ANALOG8_FUNCTION	,
		MODBUS_TABLE1_ZERO	,
		MODBUS_TABLE1_HALFONE	,
		MODBUS_TABLE1_ONE	,
		MODBUS_TABLE1_HALFTWO	,
		MODBUS_TABLE1_TWO	,
		MODBUS_TABLE1_HALFTHREE	,
		MODBUS_TABLE1_THREE	,
		MODBUS_TABLE1_HALFFOUR	,
		MODBUS_TABLE1_FOUR	,
		MODBUS_TABLE1_HALFFIVE	,
		MODBUS_TABLE1_FIVE	,
		MODBUS_TABLE2_ZERO	,
		MODBUS_TABLE2_HALFONE	,
		MODBUS_TABLE2_ONE	,
		MODBUS_TABLE2_HALFTWO	,
		MODBUS_TABLE2_TWO	,
		MODBUS_TABLE2_HALFTHREE	,
		MODBUS_TABLE2_THREE	,
		MODBUS_TABLE2_HALFFOUR	,
		MODBUS_TABLE2_FOUR	,
		MODBUS_TABLE2_HALFFIVE	,
		MODBUS_TABLE2_FIVE	,
		MODBUS_HUM_CURRENT_DEFAULT	,
		MODBUS_MODE_OUTPUT1	,
		MODBUS_MODE_OUTPUT2	,
		MODBUS_MODE_OUTPUT3	,
		MODBUS_MODE_OUTPUT4	,
		MODBUS_MODE_OUTPUT5	,
		MODBUS_OUTPUT1_SCALE	,
		MODBUS_OUTPUT2_SCALE	,
		MODBUS_DAC_OFFSET	,
		MODBUS_OUTPUT1_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT2_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT3_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT4_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT5_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT6_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT7_DELAY_OFF_TO_ON	,
		MODBUS_OUTPUT1_DELAY_ON_TO_OFF	,
		MODBUS_OUTPUT2_DELAY_ON_TO_OFF	,
		MODBUS_OUTPUT3_DELAY_ON_TO_OFF	,
		MODBUS_OUTPUT4_DELAY_ON_TO_OFF	,
		MODBUS_OUTPUT5_DELAY_ON_TO_OFF	,
		MODBUS_OUTPUT6_DELAY_ON_TO_OFF	,
		MODBUS_OUTPUT7_DELAY_ON_TO_OFF	,
		MODBUS_CYCLING_DELAY	,
		MODBUS_CHANGOVER_DELAY	,
		MODBUS_VALVE_TRAVEL_TIME	,
		MODBUS_INTERLOCK_OUTPUT1	,
		MODBUS_INTERLOCK_OUTPUT2	,
		MODBUS_INTERLOCK_OUTPUT3	,
		MODBUS_INTERLOCK_OUTPUT4	,
		MODBUS_INTERLOCK_OUTPUT5	,
		MODBUS_INTERLOCK_OUTPUT6	,
		MODBUS_INTERLOCK_OUTPUT7	,
		MODBUS_FREEZE_DELAY_ON	,
		MODBUS_FREEZE_DELAY_OFF	,
		MODBUS_OUTPUT_MANU_ENABLE	,
		MODBUS_MANU_RELAY1	,
		MODBUS_MANU_RELAY2	,
		MODBUS_MANU_RELAY3	,
		MODBUS_MANU_RELAY4	,
		MODBUS_MANU_RELAY5	,
		MODBUS_MANUAL_AO1	,
		MODBUS_MANUAL_AO2	,
		MODBUS_DEADMASTER_AUTO_MANUAL	,
		MODBUS_DEADMASTER_OUTPUT_STATE	,
		MODBUS_DEADMASTER_COOL_OUTPUT	,
		MODBUS_DEADMASTER_HEAT_OUTPUT	,
		MODBUS_OUTPUT1_FUNCTION	,
		MODBUS_OUTPUT2_FUNCTION	,
		MODBUS_OUTPUT3_FUNCTION	,
		MODBUS_OUTPUT4_FUNCTION	,
		MODBUS_OUTPUT5_FUNCTION	,
		MODBUS_OUTPUT6_FUNCTION	,
		MODBUS_OUTPUT7_FUNCTION	,
		MODBUS_FAN_SPEED	,
		MODBUS_PID_OUTPUT1	,
		MODBUS_PID_OUTPUT2	,
		MODBUS_PID_OUTPUT3	,
		MODBUS_PID_OUTPUT4	,
		MODBUS_PID_OUTPUT5	,
		MODBUS_PID_OUTPUT6	,
		MODBUS_PID_OUTPUT7	,
		MODBUS_UNIVERSAL_OUTPUT_BEGIN	,
		MODBUS_UNIVERSAL_OUTPUT_COOL1	,
		MODBUS_UNIVERSAL_OUTPUT_COOL2	,
		MODBUS_UNIVERSAL_OUTPUT_COOL3	,
		MODBUS_UNIVERSAL_OUTPUT_HEAT1	,
		MODBUS_UNIVERSAL_OUTPUT_HEAT2	,
		MODBUS_UNIVERSAL_OUTPUT_HEAT3	,
        MODBUS_FAN0_OPER_TABLE_COAST	,
        MODBUS_FAN0_OPER_TABLE_COOL1	,
        MODBUS_FAN0_OPER_TABLE_COOL2	,
        MODBUS_FAN0_OPER_TABLE_COOL3	,
        MODBUS_FAN0_OPER_TABLE_HEAT1	,
        MODBUS_FAN0_OPER_TABLE_HEAT2	,
        MODBUS_FAN0_OPER_TABLE_HEAT3	,
        MODBUS_FAN1_OPER_TABLE_COAST	,
        MODBUS_FAN1_OPER_TABLE_COOL1	,
        MODBUS_FAN1_OPER_TABLE_COOL2	,
        MODBUS_FAN1_OPER_TABLE_COOL3	,
        MODBUS_FAN1_OPER_TABLE_HEAT1	,
        MODBUS_FAN1_OPER_TABLE_HEAT2	,
        MODBUS_FAN1_OPER_TABLE_HEAT3	,
        MODBUS_FAN2_OPER_TABLE_COAST	,
        MODBUS_FAN2_OPER_TABLE_COOL1	,
        MODBUS_FAN2_OPER_TABLE_COOL2	,
        MODBUS_FAN2_OPER_TABLE_COOL3	,
        MODBUS_FAN2_OPER_TABLE_HEAT1	,
        MODBUS_FAN2_OPER_TABLE_HEAT2	,
        MODBUS_FAN2_OPER_TABLE_HEAT3	,
        MODBUS_FAN3_OPER_TABLE_COAST	,
        MODBUS_FAN3_OPER_TABLE_COOL1	,
        MODBUS_FAN3_OPER_TABLE_COOL2	,
        MODBUS_FAN3_OPER_TABLE_COOL3	,
        MODBUS_FAN3_OPER_TABLE_HEAT1	,
        MODBUS_FAN3_OPER_TABLE_HEAT2	,
        MODBUS_FAN3_OPER_TABLE_HEAT3	,
        MODBUS_FANAUT_OPER_TABLE_COAST	,
        MODBUS_FANAUT_OPER_TABLE_COOL1	,
        MODBUS_FANAUT_OPER_TABLE_COOL2	,
        MODBUS_FANAUT_OPER_TABLE_COOL3	,
        MODBUS_FANAUT_OPER_TABLE_HEAT1	,
        MODBUS_FANAUT_OPER_TABLE_HEAT2	,
        MODBUS_FANAUT_OPER_TABLE_HEAT3	,
        MODBUS_VALVE_OPERATION_TABLE_BEGIN	,
        MODBUS_VALVE_OPER_TABLE_COOL1	,
        MODBUS_VALVE_OPER_TABLE_COOL2	,
        MODBUS_VALVE_OPER_TABLE_COOL3	,
        MODBUS_VALVE_OPER_TABLE_HEAT1	,
        MODBUS_VALVE_OPER_TABLE_HEAT2	,
        MODBUS_VALVE_OPER_TABLE_HEAT3	,
		MODBUS_HEAT_UNIVERSAL_TABLE	,
		MODBUS_COOL_UNIVERSAL_TABLE	,
		MODBUS_HEAT_ORIGINAL_TABLE	,
		MODBUS_COOL_ORIGINAL_TABLE	,
		MODBUS_VALVE_OFF_TABLE_COAST	,
		MODBUS_VALVE_OFF_TABLE_COOL1	,
		MODBUS_VALVE_OFF_TABLE_COOL2	,
		MODBUS_VALVE_OFF_TABLE_COOL3	,
		MODBUS_VALVE_OFF_TABLE_HEAT1	,
		MODBUS_VALVE_OFF_TABLE_HEAT2	,
		MODBUS_VALVE_OFF_TABLE_HEAT3	,
		MODBUS_DEFAULT_SETPOINT	,
		MODBUS_SETPOINT_CONTROL	,
		MODBUS_DAYSETPOINT_OPTION	,
		MODBUS_MIDDLE_SETPOINT	,
		MODBUS_DAY_SETPOINT	,
		MODBUS_DAY_COOLING_DEADBAND	,
		MODBUS_DAY_HEATING_DEADBAND	,
		MODBUS_DAY_COOLING_SETPOINT	,
		MODBUS_DAY_HEATING_SETPOINT	,
		MODBUS_NIGHT_SETPOINT	,
		MODBUS_APPLICATION	,
		MODBUS_NIGHT_HEATING_DEADBAND	,
		MODBUS_NIGHT_COOLING_DEADBAND	,
		MODBUS_NIGHT_HEATING_SETPOINT	,
		MODBUS_NIGHT_COOLING_SETPOINT	,
		MODBUS_WINDOW_INTERLOCK_COOLING_SETPOINT  	,
		MODBUS_WINDOW_INTERLOCK_HEATING_SETPOINT	,
		MODBUS_UNIVERSAL_NIGHTSET	,
		MODBUS_UNIVERSAL_SET	,
		MODBUS_UNIVERSAL_DB_HI	,
		MODBUS_UNIVERSAL_DB_LO	,
		MODBUS_ECOMONY_COOLING_SETPOINT	,
		MODBUS_ECOMONY_HEATING_SETPOINT	,
		MODBUS_POWERUP_SETPOINT	,
		MODBUS_MAX_SETPOINT	,
		MODBUS_MIN_SETPOINT	,
		MODBUS_MAX_SETPOINT2	,
		MODBUS_MIN_SETPOINT2	,
		MODBUS_MAX_SETPOINT3	,
		MODBUS_MIN_SETPOINT3	,
		MODBUS_MAX_SETPOINT4	,
		MODBUS_MIN_SETPOINT4	,
		MODBUS_SETPOINT_INCREASE	,
		MODBUS_FREEZE_TEMP_SETPOINT	,
		MODBUS_WALL_SETPOINT	,
		MODBUS_CEILING_SETPOINT	,
		MODBUS_AVERAGE_SETPOINT	,
		MODBUS_UNOCCUPIED_HEATING	,
		MODBUS_UNOCCUPIED_COOLING	,
		MODBUS_RH_SETPOINT	,
		MODBUS_CURRENT1_SETPOINT	,
		MODBUS_TEMP_SELECT	,
		MODBUS_INPUT1_SELECT	,
		MODBUS_COOLING_PTERM	,
		MODBUS_COOLING_ITERM	,
		MODBUS_UNIVERSAL_PTERM	,
		MODBUS_UNIVERSAL_ITERM	,
		MODBUS_UNITS1_HIGH	,
		MODBUS_UNITS1_LOW	,
		MODBUS_UNITS2_HIGH	,
		MODBUS_UNITS2_LOW	,
		MODBUS_PID2_MODE_OPERATION	,
		MODBUS_KEYPAD_SELECT	,
		MODBUS_SPECIAL_MENU_LOCK	,
		MODBUS_DISPLAY	,
		MODBUS_ICON	,
		MODBUS_KEYPAD_VALUE	,
		MODBUS_DISPLAY_DIGITAL	,
		MODBUS_DISPLAY_STATUS	,
		MODBUS_ROUND_DISPLAY	,
		MODBUS_MIN_ADDRESS	,
		MODBUS_MAX_ADDRESS	,
		MODBUS_EEPROM_SIZE	,
		MODBUS_TIMER_SELECT	,
		MODBUS_DIAGNOSTIC_ALARM	,
		MODBUS_DAY1_EVENT1_HI	,
		MODBUS_DAY1_EVENT1_LO	,
		MODBUS_DAY1_EVENT2_HI	,
		MODBUS_DAY1_EVENT2_LO	,
		MODBUS_DAY1_EVENT3_HI	,
		MODBUS_DAY1_EVENT3_LO	,
		MODBUS_DAY1_EVENT4_HI	,
		MODBUS_DAY1_EVENT4_LO	,
		MODBUS_DAY2_EVENT1_HI	,
		MODBUS_DAY2_EVENT1_LO	,
		MODBUS_DAY2_EVENT2_HI	,
		MODBUS_DAY2_EVENT2_LO	,
		MODBUS_DAY2_EVENT3_HI	,
		MODBUS_DAY2_EVENT3_LO	,
		MODBUS_DAY2_EVENT4_HI	,
		MODBUS_DAY2_EVENT4_LO	,
		MODBUS_LCD_TURN_OFF	,
		MODBUS_LCD_SCREEN1	,
		MODBUS_LCD_SCREEN2	,
		MODBUS_DEMAND_RESPONSE	,
		MODBUS_LOCK_REGISTER	,
		MODBUS_PIR_STAGE	,
		MODBUS_FIRST_CAL_FLAG	,
		MODBUS_HUM_CAL_EREASE	,
		MODBUS_LCD_ROTATE_ENABLE	,
		MODBUS_SCHEDULE_ON_OFF	,
		MODBUS_DISP_ITEM_TEMPERATURE	,
		MODBUS_DISP_ITEM_SETPOINT	,
		MODBUS_DISP_ITEM_AI1	,
		MODBUS_DISP_ITEM_AI2	,
		MODBUS_DISP_ITEM_AI3	,
		MODBUS_DISP_ITEM_AI4	,
		MODBUS_DISP_ITEM_AI5	,
		MODBUS_DISP_ITEM_AI6	,
		MODBUS_DISP_ITEM_AI7	,
		MODBUS_DISP_ITEM_AI8	,
		MODBUS_DISP_ITEM_MODE	,
		MODBUS_DISP_ITEM_USER_INFO	,
		MODBUS_DISP_ITEM_CLOCK_DATE	,
		MODBUS_DISP_ITEM_CLOCK_TIME	,
		MODBUS_DISP_ITEM_EXTENAL_SENSOR1	,
		MODBUS_DISP_ITEM_EXTENAL_SENSOR2	,
		MODBUS_DISP_ITEM_EXTENAL_SENSOR3	,
		MODBUS_DISP_ITEM_EXTENAL_SENSOR4	,
		MODBUS_DISP_ITEM_OUT1	,
		MODBUS_DISP_ITEM_OUT2	,
		MODBUS_DISP_ITEM_OUT3	,
		MODBUS_DISP_ITEM_OUT4	,
		MODBUS_DISP_ITEM_OUT5	,
		MODBUS_DISP_ITEM_OUT6	,
		MODBUS_DISP_ITEM_OUT7	,
		MODBUS_OUTPUT_PWM_AUTO_COAST	,
		MODBUS_OUTPUT_PWM_AUTO_COOL1	,
		MODBUS_OUTPUT_PWM_AUTO_COOL2	,
		MODBUS_OUTPUT_PWM_AUTO_COOL3	,
		MODBUS_OUTPUT_PWM_AUTO_HEAT1	,
		MODBUS_OUTPUT_PWM_AUTO_HEAT2	,
		MODBUS_OUTPUT_PWM_AUTO_HEAT3	,
		MODBUS_SUN_ICON_CONTROL	,
		MODBUS_MOON_ICON_CONTROL	,
		MODBUS_EXT_SENSOR_TEMP_CAL	,
		MODBUS_HUM_HEATER_CONTROL_ENABLE	,
		MODBUS_HUM_INPUT_MANUAL_ENABLE	,
		MODBUS_HUM_INPUT_MANUAL_VALUE	,
		MODBUS_CO2_INPUT_MANUAL_ENABLE	,
		MODBUS_CO2_INPUT_VALUE	,
		MODBUS_CO2_CALIBRATION_DATA	,
		MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN	,
		MODBUS_UNIVERSAL_OFF_OUTPUT_COOL1   ,	
		MODBUS_UNIVERSAL_OFF_OUTPUT_COOL2	,
		MODBUS_UNIVERSAL_OFF_OUTPUT_COOL3	,
		MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT1	,
		MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT2	,
		MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT3	,
		MODBUS_UNIVERSAL_OFF_VALVE_BEGIN	,
		MODBUS_UNIVERSAL_OFF_VALVE_COOL1	,
		MODBUS_UNIVERSAL_OFF_VALVE_COOL2	,
		MODBUS_UNIVERSAL_OFF_VALVE_COOL3	,
		MODBUS_UNIVERSAL_OFF_VALVE_HEAT1	,
		MODBUS_UNIVERSAL_OFF_VALVE_HEAT2	,
		MODBUS_UNIVERSAL_OFF_VALVE_HEAT3	,
		MODBUS_VALUE_SENSOR	,
		MODBUS_PIR_SELECT_ENABLE	,
		MODBUS_PIR_REAL_VALUE	,
		MODBUS_PIR_OCCUPIED	,
		MODBUS_UNIVERSAL_VALVE_BEGIN	,
		MODBUS_UNIVERSAL_VALVE_COOL1	,
		MODBUS_UNIVERSAL_VALVE_COOL2	,
		MODBUS_UNIVERSAL_VALVE_COOL3	,
		MODBUS_UNIVERSAL_VALVE_HEAT1	,
		MODBUS_UNIVERSAL_VALVE_HEAT2	,
		MODBUS_UNIVERSAL_VALVE_HEAT3    ,
		641,
		MODBUS_SUPPLY_SETPOINT	,
		MODBUS_MAX_SUPPLY_SETPOINT	,
		MODBUS_MIN_SUPPLY_SETPOINT	,
		MODBUS_MAX_AIRFLOW_COOLING	,
		MODBUS_MAX_AIRFLOW_HEATING	,
		MODBUS_MIN_AIRFLOW	,
		MODBUS_AIRFLOW_SETPOINT   	,
		MODBUS_VAV_CONTROL	,
		MODBUS_PID3_VALVE_OPERATION_TABLE_BEGIN	,
		MODBUS_PID3_VALVE_OPER_TABLE_COOL1	,
		MODBUS_PID3_VALVE_OPER_TABLE_COOL2	,
		MODBUS_PID3_VALVE_OPER_TABLE_COOL3	,
		MODBUS_PID3_VALVE_OPER_TABLE_HEAT1	,
		MODBUS_PID3_VALVE_OPER_TABLE_HEAT2	,
		MODBUS_PID3_VALVE_OPER_TABLE_HEAT3	,
		MODBUS_PID3_COOLING_DB	,
		MODBUS_PID3_HEATING_DB	,
		MODBUS_PID3_PTERM	,
		MODBUS_PID3_ITERM	,
		MODBUS_PID3_HEAT_STAGE	,
		MODBUS_PID3_COOL_STAGE	,
		MODBUS_PID3_OUTPUT 	,
		MODBUS_PID3_OUTPUT_BEGIN	,
		MODBUS_PID3_OUTPUT_COOL1 	,
		MODBUS_PID3_OUTPUT_COOL2 	,
		MODBUS_PID3_OUTPUT_COOL3 	,
		MODBUS_PID3_OUTPUT_HEAT1   	,
		MODBUS_PID3_OUTPUT_HEAT2 	,
		MODBUS_PID3_OUTPUT_HEAT3 	,
		MODBUS_PID3_VALVE_OFF_TABLE_BEGIN	,
		MODBUS_PID3_VALVE_OFF_TABLE_COOL1	,
		MODBUS_PID3_VALVE_OFF_TABLE_COOL2	,
		MODBUS_PID3_VALVE_OFF_TABLE_COOL3	,
		MODBUS_PID3_VALVE_OFF_TABLE_HEAT1	,
		MODBUS_PID3_VALVE_OFF_TABLE_HEAT2	,
		MODBUS_PID3_VALVE_OFF_TABLE_HEAT3  	,
		MODBUS_PID3_OFF_OUTPUT_BEGIN  	,
		MODBUS_PID3_OFF_OUTPUT_COOL1 	,
		MODBUS_PID3_OFF_OUTPUT_COOL2 	,
		MODBUS_PID3_OFF_OUTPUT_COOL3 	,
		MODBUS_PID3_OFF_OUTPUT_HEAT1   ,
		MODBUS_PID3_OFF_OUTPUT_HEAT2 	,
		MODBUS_PID3_OFF_OUTPUT_HEAT3  ,
            MODBUS_HEAT_COOL_MODE ,
            MODBUS_PID_D_TERM,
            MODBUS_PID_SAMPLE_TIME,

	};
	//used by save2file functiojn
	CString str1;
	
			for(int i=0;i<TSTAT26_VAR_NUM_TSTAT67;i++)
			{
			    str1.Format(_T("%d,\t%d,\t%s"),tstat67_register_var[i],product_register_value[tstat67_register_var[i]],TSTATVAR_CONST_67[i]);
				_Twrite_to_file_a_line(out,str1);
				if((i%5)==4)
				_Twrite_to_file_a_line(out,_T(" "));//a space line
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

	int m_25_heat_stages,m_25_cool_stages,m_26_heat_stages,m_26_cool_stages;
	m_25_heat_stages=product_register_value[MODBUS_HEAT_ORIGINAL_TABLE];
	m_25_cool_stages=product_register_value[MODBUS_COOL_ORIGINAL_TABLE];
	m_26_heat_stages=product_register_value[MODBUS_HEAT_UNIVERSAL_TABLE];
	m_26_cool_stages=product_register_value[MODBUS_COOL_UNIVERSAL_TABLE];
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

	//int nModelID = read_one(g_tstat_id, 7);
	CString strProductClassName = GetProductName(product_register_value[7]);
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

	//if(version>=25)
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
	
	//////////////////////////////////////////////////////////////////////////
	var_write(out);	
//   save_write_input_output(out);
	//_Twrite_to_file_a_line(out,_T("OK!"));//space
    _Twrite_to_file_a_line(out,_T("//END CONFIG 1 ********************************// "));//space
	return;

	
	int nvalue=0;


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
	save_write_input_output(out);
	out.close();

	if (nValue>=0)
	{
		write_one(g_tstat_id,324,nValue);
	}


#endif

}
void Save2File_ForCO2Node(TCHAR* fn)
{
	CString strTips;
	 

	 
	 
	#if 1
	 
	//////////////////////////////////////////////////////////////////////////

	wofstream out;
	out.open(fn,ios_base::out) ;
	
	_Twrite_to_file_a_line(out,_T("Config File"));//added the header marker.

	//int nModelID = read_one(g_tstat_id, 7);
	CString strProductClassName = GetProductName(product_register_value[7]);
	strProductClassName = _T("Model : ") + strProductClassName;
	_Twrite_to_file_a_line(out, strProductClassName);//added the model
    _Twrite_to_file_a_line(out,_T("//BEGIN CONFIG 1 ********************************//\n "));//space
    _Twrite_to_file_a_line(out,_T("Reg No,Reg Value,Reg Name\n"));

	var_write_CO2_NODE(out);	

	_Twrite_to_file_a_line(out,_T("//END CONFIG 1 ********************************// "));//space
 
		return;
 
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
	//if(temppp.seven_step==false)
	//	i++;
	//if(temppp.first_step ==false)
	//	i++;
	//if(temppp.second_step ==false)
	//	i++;
	//if (temppp.thurth_step ==false) 
	//	i++;
	//if (temppp.fifth_step ==false) 
	//	i++;
	//if ( temppp.third_step ==false) 
	//	i++;
	//if (temppp.sixth_step ==false)
	//	i++;
	//if (i>2) 
	//{
		//return TRUE;
	//}
	//else
		return false;
}

bool Check_Config_File(wifstream & inf){
	    TCHAR buf[1024];
	    bool ret=false;
 
		inf.getline(buf,1024);
		if (find_sub_chars(buf,_T("Config File")))
		{
			inf.getline(buf,1024);//T3000 版本号
            if (find_sub_chars(buf,GetProductName(product_register_value[7])))
            {
                ret=true; 
            }
			inf.getline(buf,1024);
			if (find_sub_chars(buf,GetProductName(product_register_value[7])))
			{
				ret=true; 
			}
		   
		}


		return ret;
  
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

	if(find_sub_chars(buf,_T("//   PID2 OFF TABLE END")))
		return true;
	if(find_sub_chars(buf,_T("//   PID2 OFF VALVE TABLE END")))
		return true;
	if(find_sub_chars(buf,_T("//   UNIVERSAL AUTO END")))
		return true;
	if(find_sub_chars(buf,_T("//   VALVE AUTO END")))
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
				if(wcscmp(one_value,_T("Closed"))==0)
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
	/*if(section==5)
	{
		if(temppp.thurth_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_FOUR+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}*/
	/*if(section==6)
	{
		if(temppp.fifth_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_FIVE+STEP_FAILURE+_T("*********************");
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}*/
	/*if(section==7)
	{
		if(temppp.sixth_step ==false)
		{
			for_showing_text=for_showing_text+LOAD_FAILURE_SIX+STEP_FAILURE+_T("*********************";)
			change_showing_text_variable(for_showing_text);
		}
		if(p_log_file!=NULL && for_showing_text!=_T(""))
			p_log_file->Write(for_showing_text+_T("\r\n"),for_showing_text.GetLength()+2);
		return ;
	}*/
}
void Show_load_file_message(CString message,CFile *p_log_file){
 change_showing_text_variable(message);
if(p_log_file!=NULL && message!=_T(""))
p_log_file->Write(message+_T("\r\n"),message.GetLength()+2);
}
void get_delay_setting_line_value(TCHAR *buf,int array[7])
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

void get_delay_setting(wifstream & inf,int value_setting[14])
{    int rows=5;
int nFlag = product_register_value[7];
    if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)||(product_register_value[7]== PM_TSTAT7)||(product_register_value[7]== PM_TSTAT8)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
        rows=7;
	TCHAR buf[1024];
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(section_end_find(buf))//for the value setting start and fan value end
			break;
		if(wcsstr(buf,_T("Delay OFF->ON : "))!=NULL)
		{
			int array[7];
			get_delay_setting_line_value(buf,array);
			for(int i=0;i<rows;i++)				
				value_setting[i]=array[i];//
			//*************************************relay 2			
		}
		else if(wcsstr(buf,_T("Delay ON->OFF : "))!=NULL)
		{
			int array[7];
			get_delay_setting_line_value(buf,array);
			for(int i=0;i<rows;i++)				
				value_setting[rows+i]=array[i];//				
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
		if (wcsstr(buf,_T("//Input Name Config"))!=NULL)
		 break;


		get_write_var_line(buf,tstat_version,p_log_file,p_log_file_one_time);//get a line ,one register value,
	}
}

void get_write_var_line_T3(TCHAR *buf,float tstat_version,CStdioFile *p_log_file)
{
	CString for_showing_text;
	Reg_Infor Reg_Infor_Temp;
	if(_wtoi(buf)==0)
		return ;
	TCHAR *temp=buf;
	temp=wcsstr(temp,_T(","));
	temp++;	
	int register_id=_wtoi(buf);
	int register_value=_wtoi(temp);
	int j=-1;
	if (register_id<100)
	{
	return;
	}
	
	j=write_one(now_tstat_id,register_id,register_value);	
	Reg_Infor_Temp.regAddress=register_id;

	Reg_Infor_Temp.RegValue=register_value;
	if(p_log_file!=NULL)
	{
		for_showing_text.Format(_T("register ID:%d,VALUE:%d write "),register_id,register_value);
		if(j>0)
			for_showing_text=for_showing_text+_T("OK\r\n");
		else
		{
			g_Vector_Write_Error.push_back(Reg_Infor_Temp);
			//for_showing_text=for_showing_text+_T("Error******************\r\n");
		}
		change_showing_text_variable(for_showing_text);
		p_log_file->WriteString(for_showing_text);
	}


}

void get_var_write_var_T3(wifstream & inf,float tstat_version,CStdioFile *p_log_file)
{
	TCHAR buf[1024];	
	while(!inf.eof())
	{
		inf.getline(buf,1024);

		if(find_sub_chars(buf,_T("END CONFIG")))
			break;


		get_write_var_line_T3(buf,tstat_version,p_log_file);//get a line ,one register value,
	}

}

//void get_write_var_line_CO2NODE(TCHAR *buf,float tstat_version,CStdioFile *p_log_file)
//{
//    CString for_showing_text;
//    Reg_Infor Reg_Infor_Temp;
//    if(_wtoi(buf)==0)
//        return ;
//    TCHAR *temp=buf;
//    temp=wcsstr(temp,_T(","));
//    temp++;	
//    int register_id=_wtoi(buf);
//    int register_value=_wtoi(temp);
//    int j=-1;
//    if (register_id<100)
//    {
//        return;
//    }
//
//    j=write_one(now_tstat_id,register_id,register_value);	
//    Reg_Infor_Temp.regAddress=register_id;
//
//    Reg_Infor_Temp.RegValue=register_value;
//    if(p_log_file!=NULL)
//    {
//        for_showing_text.Format(_T("register ID:%d,VALUE:%d write "),register_id,register_value);
//        if(j>0)
//            for_showing_text=for_showing_text+_T("OK\r\n");
//        else
//        {
//            g_Vector_Write_Error.push_back(Reg_Infor_Temp);
//            //for_showing_text=for_showing_text+_T("Error******************\r\n");
//        }
//        change_showing_text_variable(for_showing_text);
//        p_log_file->WriteString(for_showing_text);
//    }
//
//
//}
//
//
//void get_var_write_var_CO2NODE(wifstream & inf,float tstat_version,CStdioFile *p_log_file)
//{
//    TCHAR buf[1024];	
//    while(!inf.eof())
//    {
//        inf.getline(buf,1024);
//
//        if(find_sub_chars(buf,_T("END CONFIG")))
//            break;
//
//
//        get_write_var_line_CO2NODE(buf,tstat_version,p_log_file);//get a line ,one register value,
//    }
//
//}


void write_input_output_var(wifstream & inf,float tstat_version,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{
	TCHAR buf[1024];	
	int inputno=0;
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		 if (wcsstr(buf,_T("//Output Name Config"))!=NULL)
			break; 
        

		get_write_var_line_input_output(buf,tstat_version,inputno,p_log_file,p_log_file_one_time);//get a line ,one register value,
	  inputno++;
	}
	int outputno=0;
	while(!inf.eof())
	{
 		inf.getline(buf,1024);
		if (wcsstr(buf,_T("//Input Output Range Config"))!=NULL)
			break; 
		get_write_var_line_output(buf,tstat_version,outputno,p_log_file,p_log_file_one_time);//get a line ,one register value,
		outputno++;
	}
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;

	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if (wcsstr(buf,_T("//END"))!=NULL)
			break; 
		CString strText;
		strText.Format(_T("%s"),buf);
		CStringArray stringarray;
		SplitCStringA(stringarray,strText,_T(":"));
		CString strinput=stringarray[0];
		CString strrange=stringarray[1];
		int inputno=_wtoi(strinput);
		int rangeno=_wtoi(strrange);
		int m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
		CString sql;
		sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),inputno,m_sn);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
		if (!q.eof())//有表但是没有对应序列号的值
		{

			sql.Format(_T("update Value_Range set CRange = %d where CInputNo=%d and SN=%d "),rangeno,inputno,m_sn);
			SqliteDBBuilding.execDML((UTF8MBSTR)sql);
		}
		else
		{
		 
			sql.Format(_T("Insert into Value_Range( SN,CInputNo,CRange) values('%d','%d','%d')"),m_sn,inputno,rangeno);
			SqliteDBBuilding.execDML((UTF8MBSTR)sql);
		}
	}
	SqliteDBBuilding.closedb();

	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if (wcsstr(buf,_T("//Model Name Config"))!=NULL)
			break; 
	}

}
void write_TStatAllLabel(wifstream & inf,float tstat_version,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{
	TCHAR buf[1024];	
	int inputno=0;
	int LabelNumber = 0;
	CString strText;
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		

		if (wcsstr(buf,_T("//Model Name Config END"))!=NULL)
			break; 
		
		TCHAR *temp=buf;
		temp=wcsstr(temp,_T(":"));
		temp++;	
		strText.Format(_T("%s"),temp);


// 		strText.TrimRight();
// 		strText.TrimLeft();
		unsigned char p[8];//input+input1
		for(int i=0;i<8;i++)
		{
			if(i<strText.GetLength())
				p[i]=strText.GetAt(i);
			else
				p[i]=0;
		}

		if (Write_Multi(g_tstat_id,p,737+4*LabelNumber,8)>0)
		{
		 
		} 
		else
		{    int times = 5;
			 while(--times>=0)
			 {
				if (Write_Multi(g_tstat_id,p,737+4*LabelNumber,8)>0)
				break;
			 }
		}
		  LabelNumber ++;
		 
		 
	}
	
}
int fan_number;

void get_write_var_line(TCHAR *buf,float tstat_version,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{//used by get_value_setting function only
Reg_Infor Reg_Infor_Temp;
	CString for_showing_text;
	if(_wtoi(buf)==0)
		return ;
	TCHAR *temp=buf;
	temp=wcsstr(temp,_T(","));
	temp++;	
	int register_id=_wtoi(buf);
    if (register_id == 101)
    {
        return;  //2018 08 22  TSTAT 现在的版本 COOL HEAT MODE ,这个寄存器 不让写.
    }
	int register_value=_wtoi(temp);
	int j;
	int register_122=0;
	if(register_id==MODBUS_DEGC_OR_F && register_value>1)
		j=-4;
		//wrong value
	/*
	else if(register_id==128 && register_value>5)
		j=-4;//wrong value
		//comment on 09-02-03
		*/

	//else if(register_id==MODBUS_FAN_MODE )
	//{
	//	int real_fan_number=get_real_number_fan_speeds(fan_number);
	//	if(register_value!=real_fan_number && real_fan_number!=-1)
	//	{
	//		register_122=-100;
	//		register_value=real_fan_number;/////////////^-^ 
	//	}
	//	j=write_one(now_tstat_id,register_id,register_value);	
	//}
	else
		j=write_one(now_tstat_id,register_id,register_value);	
#if 0   //20180911 杜帆屏蔽 T5需要，但现在不维护;
	if(register_id==185)
	{
		Sleep(14000);	
		//if(register_value==1)Change_BaudRate(19200);
		//if(register_value==0)Change_BaudRate(9600);
	}
#endif 
	if(j==-2 && tstat_version<25 && tstat_version >0)  ////////////////////////if the version is 24.4 ,write_one some register will restart,for example 118,121
	{
		//Sleep(14000);	//20180911 杜帆屏蔽 T5需要，但现在不维护;
		j=write_one(now_tstat_id,register_id,register_value);	
	}
	Reg_Infor_Temp.regAddress=register_id;

	Reg_Infor_Temp.RegValue=register_value;
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
			g_Vector_Write_Error.push_back(Reg_Infor_Temp);
			//for_showing_text=for_showing_text+_T("Error******************\r\n");
		}
		change_showing_text_variable(for_showing_text);
	//	p_log_file->Write(for_showing_text.GetString(),for_showing_text.GetLength());
		p_log_file->WriteString(for_showing_text);
	}

}

void get_write_var_line_input_output(TCHAR *buf,float tstat_version,int inputno,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{
//used by get_value_setting function only
	CString for_showing_text,strText, strInName;;
	//if(_wtoi(buf)==0)
	//	return ;
	TCHAR *temp=buf;
	temp=wcsstr(temp,_T(":"));
	temp++;	
 
	
	
	strText.Format(_T("%s"),temp);
    int lRow=inputno+2;
   
	switch (lRow)
	{
	case 1:
		strInName=g_strSensorName;
		break;
	case 2:
		strInName=g_strInName1;
		break;
	case 3:
		strInName=g_strInName2;
		break;
	case 4:
		strInName=g_strInName3;
		break;
	case 5:
		strInName=g_strInName4;
		break;
	case 6:
		strInName=g_strInName5;
		break;
	case 7:
		strInName=g_strInName6;
		break;
	case 8:
		strInName=g_strInName7;
		break;
	case 9:
		strInName=g_strInName8;
		break;
	}
	if(strText.CompareNoCase(strInName)==0)
		return;
	    
		int Model_ID=read_one(now_tstat_id,7,5);
		int nFlag = Model_ID;
	if ((Model_ID==PM_TSTAT5G)||(Model_ID==PM_TSTAT5E)||(Model_ID==PM_PM5E)||(Model_ID==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT5i)||(Model_ID==PM_TSTAT7)||(Model_ID==PM_TSTAT8)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
	{
		strText.TrimRight();
		strText.TrimLeft();
		unsigned char p[8];//input+input1
		for(int i=0;i<8;i++)
		{
			if(i<strText.GetLength())
				p[i]=strText.GetAt(i);
			else
				p[i]=0;
		}
		 
			if (Write_Multi(g_tstat_id,p,MODBUS_AI1_CHAR1+4*(lRow-2),8)>0)
			{

			} 
			else
			{
				//AfxMessageBox(_T("Error"));
				//return;
			}

		 
	}
	try
	{

		 int ID=read_one(now_tstat_id,6);
		if(ID>0)
		{  unsigned short num[4];  
		   Read_Multi(now_tstat_id,&num[0],0,4);
		   g_serialNum=num[0]+num[1]*256+num[2]*256*256+num[3]*256*256;
		  
		   CppSQLite3DB SqliteDBBuilding;
		   CppSQLite3Table table;
		   CppSQLite3Query q;
		   SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			CString strSerial;
			strSerial.Format(_T("%d"),g_serialNum);

			CString strsql;
			strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
			//m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
			 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
			if(!q.eof())//update
			{			
				CString strField;
				switch (lRow)
				{

				case 2:
					strField=_T("INPUT1");
					break;
				case 3:
					strField=_T("INPUT2");
					break;
				case 4:
					strField=_T("INPUT3");
					break;
				case 5:
					strField=_T("INPUT4");
					break;
				case 6:
					strField=_T("INPUT5");
					break;
				case 7:
					strField=_T("INPUT6");
					break;
				case 8:
					strField=_T("INPUT7");
					break;
				case 9:
					strField=_T("INPUT8");
					break;
				}

				try
				{

					CString str_temp;
					str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
					SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}

			}
			else//inerst
			{
				switch (lRow)
				{
				case 2:
					g_strInName1=strText;
					break;
				case 3:
					g_strInName2=strText;
					break;
				case 4:
					g_strInName3=strText;
					break;
				case 5:
					g_strInName4=strText;
					break;
				case 6:
					g_strInName5=strText;
					break;
				case 7:
					g_strInName6=strText;
					break;
				case 8:
					g_strInName7=strText;
					break;
				case 9:
					g_strInName8=strText;
					break;
				}

				CString g_strInName9;
				//g_strInName9 = _T("input9");Humidity Sensor
				g_strInName9 = _T("Humidity Sensor");
				CString	str_temp;
				str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
					strSerial,
					g_strInName1,
					g_strInName2,
					g_strInName3,
					g_strInName4,
					g_strInName5,
					g_strInName6,
					g_strInName7,
					g_strOutName1,
					g_strOutName2,
					g_strOutName3,
					g_strOutName4,
					g_strOutName5,
					g_strOutName6,
					g_strOutName7,
					g_strInName8,
					g_strInHumName,
					g_strSensorName
					);
				try
				{
				   SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
					 
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

			switch (lRow)
			{
			case 2:
				g_strInName1=strText;
				break;
			case 3:
				g_strInName2=strText;
				break;
			case 4:
				g_strInName3=strText;
				break;
			case 5:
				g_strInName4=strText;
				break;
			case 6:
				g_strInName5=strText;
				break;
			case 7:
				g_strInName6=strText;
				break;
			case 8:
				g_strInName7=strText;
				break;
			case 9:
				g_strInName8=strText;
				break;
			}
			 

		 SqliteDBBuilding.closedb();
		}	
        
	}
	catch (...)
	{


	}
	
    //wrong value
	/*
	else if(register_id==128 && register_value>5)
		j=-4;//wrong value
		//comment on 09-02-03
		*/

	//else if(register_id==MODBUS_FAN_MODE )
	//{
	//	int real_fan_number=get_real_number_fan_speeds(fan_number);
	//	if(register_value!=real_fan_number && real_fan_number!=-1)
	//	{
	//		register_122=-100;
	//		register_value=real_fan_number;/////////////^-^ 
	//	}
	//	j=write_one(now_tstat_id,register_id,register_value);	
	//}
	//else
	//	j=write_one(now_tstat_id,register_id,register_value);	
	//if(register_id==185)
	//{
	//	Sleep(14000);	
	//	//if(register_value==1)Change_BaudRate(19200);
	//	//if(register_value==0)Change_BaudRate(9600);
	//}

	//if(j==-2 && tstat_version<25 && tstat_version >0)  ////////////////////////if the version is 24.4 ,write_one some register will restart,for example 118,121
	//{
	//	Sleep(14000);	
	//	j=write_one(now_tstat_id,register_id,register_value);	
	//}
	//if(p_log_file!=NULL)
	//{
	//	for_showing_text.Format(_T("register ID:%d,VALUE:%d write "),register_id,register_value);
	//	if(register_122==-100)
	//	{
	//		CString t_str=for_showing_text;
	//		for_showing_text.Format(_T("NUMBER_OF_FAN_SPEEDS IS INCORRECT,try correct value :%d "),register_value);
	//		for_showing_text=t_str+for_showing_text;
	//	}
	//	if(j>0)
	//		for_showing_text=for_showing_text+_T("OK\r\n");
	//	else
	//	{
	//		p_log_file_one_time->seven_step=false;
	//		for_showing_text=for_showing_text+_T("Error******************\r\n");
	//	}
	//	change_showing_text_variable(for_showing_text);
	////	p_log_file->Write(for_showing_text.GetString(),for_showing_text.GetLength());
	//	p_log_file->WriteString(for_showing_text);
	//}

}
void get_write_var_line_output(TCHAR *buf,float tstat_version,int outputno,CStdioFile *p_log_file,load_file_every_step *p_log_file_one_time)
{
 
	CString for_showing_text,strText, strInName;;
	 
	TCHAR *temp=buf;
	temp=wcsstr(temp,_T(":"));
	temp++;	
 
	
	
	strText.Format(_T("%s"),temp);
    int lRow=outputno+1;
   
	switch (lRow)
	{
	case 1:
		strInName=g_strOutName1;
		break;
	case 2:
		strInName=g_strOutName2;
		break;
	case 3:
		strInName=g_strOutName3;
		break;
	case 4:
		strInName=g_strOutName4;
		break;
	case 5:
		strInName=g_strOutName5;
		break;
	case 6:
		strInName=g_strOutName6;
		break;
	case 7:
		strInName=g_strOutName7;
		break;
	}
	if(strText.CompareNoCase(strInName)==0)
		return;
	try
	{
		

		 int Model_ID =read_one(now_tstat_id,7,5);
		if(Model_ID>0)
		{  
			if ((Model_ID == PM_TSTAT5G) || (Model_ID == PM_TSTAT5E) || (Model_ID == PM_PM5E) || (Model_ID == PM_TSTAT6) || (product_register_value[7] == PM_TSTAT5i) || (Model_ID == PM_TSTAT7) || (Model_ID == PM_TSTAT8)
				|| (Model_ID == PM_TSTAT8_WIFI) || (Model_ID == PM_TSTAT8_OCC) || (Model_ID == PM_TSTAT7_ARM) || (Model_ID == PM_TSTAT8_220V))
			{
				strText.TrimRight();
				strText.TrimLeft();
				unsigned char p[8];//input+input1
				for (int i = 0; i < 8; i++)
				{
					if (i < strText.GetLength())
						p[i] = strText.GetAt(i);
					else
						p[i] = 0;
				}

				if (Write_Multi(g_tstat_id, p, MODBUS_OUTPUT1_CHAR1 + 4 * (lRow - 1), 8) > 0)
				{

				}
				else
				{
					//AfxMessageBox(_T("Error"));
					//return;
				}


			}

			unsigned short num[4];  
		   Read_Multi(now_tstat_id,&num[0],0,4);
		   g_serialNum=num[0]+num[1]*256+num[2]*256*256+num[3]*256*256;

		   CppSQLite3DB SqliteDBBuilding;
		   CppSQLite3Table table;
		   CppSQLite3Query q;
		   SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			CString strSerial;
			strSerial.Format(_T("%d"),g_serialNum);

			CString strsql;
			strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
			//m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
			if(q.eof())//update
			{			
				CString strField;
				switch (lRow)
				{

				case 1:
					strField=_T("OUTPUT1");
					break;
				case 2:
					strField=_T("OUTPUT2");
					break;
				case 3:
					strField=_T("OUTPUT3");
					break;
				case 4:
					strField=_T("OUTPUT4");
					break;
				case 5:
					strField=_T("OUTPUT5");
					break;
				case 6:
					strField=_T("OUTPUT6");
					break;
				case 7:
					strField=_T("OUTPUT7");
					break;
				}

				try
				{

					CString str_temp;
					str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
					//AfxMessageBox(str_temp );
					SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
					//m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}

			}
			else//inerst
			{
				switch (lRow)
				{
				case 1:
					g_strOutName1=strText;
					break; 
				case 2:
					g_strOutName2=strText;
					break;
				case 3:
					g_strOutName3=strText;
					break;
				case 4:
					g_strOutName4=strText;
					break;
				case 5:
					g_strOutName5=strText;
					break;
				case 6:
					g_strOutName6=strText;
					break;
				case 7:
					g_strOutName7=strText;
					break;

				}
				CString	str_temp;
				str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s')"),
					strSerial,
					g_strOutName1,
					g_strOutName2,
					g_strOutName3,
					g_strOutName4,
					g_strOutName5,
					g_strOutName6,
					g_strOutName7 
					);
				try
				{

					 SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

			switch (lRow)
			{
			case 2:
				g_strOutName1=strText;
				break;
			case 3:
				g_strOutName2=strText;
				break;
			case 4:
				g_strOutName3=strText;
				break;
			case 5:
				g_strOutName4=strText;
				break;
			case 6:
				g_strOutName5=strText;
				break;
			case 7:
				g_strOutName6=strText;
				break;
			case 8:
				g_strOutName7=strText;
				break;
			case 9:
				g_strOutName8=strText;
				break;
			}
		SqliteDBBuilding.closedb();
		}	

	}
	catch (...)
	{


	}
}

void Save2File_ForTwoFilesTSTAT67( TCHAR* fn )
{
	CString strTips;
//	int nValue;
	//nValue=read_one(g_tstat_id,324);
	//	write_one(g_tstat_id,324,0);//////
	// save tstat register value to file

// 		268	330	1	Low byte	W/R	Number of Heating Stages (Max heat+cool = 6)
// 		269	331	1	Low byte	W/R	Number of Cooling Stages (Max heat + Cool = 6) 
// 		276	332	1	Low byte	W/R	Number of Heating Stages in Original Table - (Maximum # of total heating and cooling states is 6)
// 		277	333	1	Low byte	W/R	Number of Cooling Stages in Original Table - (Maximum # of total heating and cooling states is 6)
	int multy_ret = 0;
	for(int i=0;i<17;i++) //Modify by Fance , tstat 6 has more register than 512;

	{
	 
		//register_critical_section.Lock();
		//
		
		multy_ret = Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100);
		//register_critical_section.Unlock();
		Sleep(100);
		if(multy_ret<0)		//Fance : 如果出现读失败 就跳出循环体,因为如果是由断开连接 造成的 读失败 会使其他需要用到读的地方一直无法获得资源;
			break;
	}

     if (multy_ret<0)
     {
	    AfxMessageBox(_T("Saving Error ,Please try again."));
		return ;
     }

	//Fance_1
	memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));


	int m_25_heat_stages,m_25_cool_stages,m_26_heat_stages,m_26_cool_stages;
// 	m_25_heat_stages=read_one(g_tstat_id,276);
// 	m_25_cool_stages=read_one(g_tstat_id,277);
// 	m_26_heat_stages=read_one(g_tstat_id,268);
// 	m_26_cool_stages=read_one(g_tstat_id,269);


	m_25_heat_stages=product_register_value[MODBUS_HEAT_ORIGINAL_TABLE];
	m_25_cool_stages=product_register_value[MODBUS_COOL_ORIGINAL_TABLE];
	m_26_heat_stages=product_register_value[MODBUS_HEAT_UNIVERSAL_TABLE];
	m_26_cool_stages=product_register_value[MODBUS_COOL_UNIVERSAL_TABLE];
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
	CString T3000Version;
	T3000Version.Format(_T("T3000 Version:%s"), CurrentT3000Version);
	_Twrite_to_file_a_line(out,T3000Version);//added the header marker.
	//int nModelID = read_one(g_tstat_id, 7);
	CString strProductClassName = GetProductName(product_register_value[7]);
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

	//if(version>=25)
		delay_time_write_Tstat67(out);//delay time write
	//if(version>=26)
	//{   //Lookup table 取消了这个功能
		//_Twrite_to_file_a_line(out,_T(" "));//space
		//lookup_table_write(out);//lookup table 26 only
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		strTips = _T("Config file saved 60%...");
		SetPaneString(1, strTips);
		//////////////////////////////////////////////////////////////////////////
		//????
		universal_relay_write_Tstat67OFF(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		 
		//////////////////////////////////////////////////////////////////////////
		universal_value_setting_write_Tstat67OFF(out,m_26_heat_stages,m_26_cool_stages);

		universal_relay_write_Tstat67Auto(out,m_26_heat_stages,m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
		_Twrite_to_file_a_line(out,_T(" "));//space
		//////////////////////////////////////////////////////////////////////////
		strTips = _T("Config file saved 70%...");
		SetPaneString(1, strTips);
		//////////////////////////////////////////////////////////////////////////
		universal_value_setting_write_Tstat67Auto(out,m_26_heat_stages,m_26_cool_stages);
	//}
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
	strTips = _T("Finished");
	SetPaneString(3, strTips);
	//////////////////////////////////////////////////////////////////////////
	var_write_Tstat67(out);
	save_write_input_output(out);	
	//_Twrite_to_file_a_line(out,_T("OK!"));//space
	save_write_TStatAllLabel(out);
    save_write_TStatSchedual(out);
	return;

	_Twrite_to_file_a_line(out,_T("//END CONFIG 1 ********************************// "));//space
	int nvalue=0;

	
	return;

#endif
}
 
void LoadFile2Tstat_T3(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file)
{
   //int nSpecialValue=read_one(now_tstat_id,326);
	//write_one(g_tstat_id,324,0);
	//every step is false

	CString for_showing_text;
	float tstat_version;
	tstat_version=get_tstat_version(now_tstat_id);///////////////////get version
	wifstream inf;//file
	inf.open(fn,ios_base::in);
	if (!Check_Config_File(inf))
	{
		AfxMessageBox(_T("It's not a good config file!"));
		return;
	}
	for_showing_text=_T("Begin.....");
	TCHAR buf[1024];

	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(wcscmp(buf,_T("Reg No,Reg Value,Reg Name"))==0)
		{
			get_var_write_var_T3(inf,tstat_version,p_log_file);//////a line //////////// a line

			if(p_log_file!=NULL)
				p_log_file->WriteString(_T("\r\n"));
		}
	}
	
	int Try_Times=5;
	while(g_Vector_Write_Error.size()>0&&Try_Times>0){
		int vecInt_index = 0;
		for (vector <Reg_Infor>::iterator iter_index = g_Vector_Write_Error.begin(); 
			iter_index != g_Vector_Write_Error.end();) 
		{
			int i_i=write_one(now_tstat_id,iter_index->regAddress,iter_index->RegValue);
			if(p_log_file!=NULL)
			{
				if(i_i>0)
				{
					for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),iter_index->regAddress,iter_index->RegValue);
					vecInt_index = iter_index - g_Vector_Write_Error.begin(); 
					g_Vector_Write_Error.erase(iter_index);
					iter_index = g_Vector_Write_Error.begin() + vecInt_index; 
				}

				else
				{
					//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
					//g_Vector_Write_Error.push_back(Reg_Infor_Temp);
					//load_file_one_time.sixth_step=false;
					++iter_index;
				}
				change_showing_text_variable(for_showing_text);
				p_log_file->WriteString(for_showing_text.GetString());
			}

		}
		--Try_Times;
	}


	for (vector <Reg_Infor>::iterator iter_index = g_Vector_Write_Error.begin(); 
		iter_index != g_Vector_Write_Error.end(); ++iter_index) 
	{

		for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),iter_index->regAddress,iter_index->RegValue);

		//load_file_one_time.sixth_step=false;

		change_showing_text_variable(for_showing_text);
		p_log_file->WriteString(for_showing_text.GetString());
	}


	p_log_file->Flush();	
	inf.close();
	Sleep(5000);
}
void LoadFile2Tstat(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file)
{
   //int nSpecialValue=read_one(now_tstat_id,326);
	//write_one(g_tstat_id,324,0);
	//every step is false
	Reg_Infor Reg_Infor_Temp;
	CString for_showing_text;
	load_file_one_time.first_step=load_file_one_time.second_step=load_file_one_time.third_step=load_file_one_time.thurth_step=load_file_one_time.fifth_step=load_file_one_time.sixth_step=load_file_one_time.seven_step=load_file_one_time.eight_step=false;
	float tstat_version;
	tstat_version=get_tstat_version(now_tstat_id);///////////////////get version

	wifstream inf;//file
	inf.open(fn,ios_base::in);
	if (!Check_Config_File(inf))
	{
		CString strTips;
		strTips.Format(_T("Your configuration file is not fit for %s"),GetProductName(product_register_value[7]));

		AfxMessageBox(strTips);
		return;
	}
	int fan_value[35]={0};//fan_value 
	for(int i=0;i<35;i++)
		fan_value[i]=0;	
    for_showing_text=_T("Begin.....");
	fan_number=get_fan_var(inf,fan_value);//fan value	
	 for_showing_text=_T("FAN.....");		
	load_file_one_time.first_step=true;
	for(int i=0;i<fan_number;i++)
	{
		for(int t_i=0;t_i<7;t_i++)
		{
			int real_fan_address=get_real_fan_address(fan_number-2,i,t_i);
			if(fan_value[i*7+t_i]>=0)
			{   
				int i_i=write_one(now_tstat_id,real_fan_address,fan_value[i*7+t_i]);
				if(p_log_file!=NULL)
				{
					if(i_i>0)
						for_showing_text.Format(_T("register ID: %d value:%d write OK\r\n"),real_fan_address,fan_value[i*7+t_i]);
					else
					{
						//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),real_fan_address,fan_value[i*7+t_i]);
						Reg_Infor_Temp.regAddress=real_fan_address;
						Reg_Infor_Temp.RegValue=fan_value[i*7+t_i];
						g_Vector_Write_Error.push_back(Reg_Infor_Temp);
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

				//   VALVE SETTING
			int value_setting[7]={0};
			memset(value_setting,0,sizeof(value_setting));
			get_value_setting(inf,value_setting);//value setting
			Show_load_file_error_message(load_file_one_time,2,p_log_file);
			load_file_one_time.second_step=true;
	        for(int i=0;i<7;i++)
			{
				if(value_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+i,value_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),173+i,value_setting[i]);
						else
						{
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),173+i,value_setting[i]);
							Reg_Infor_Temp.regAddress=MODBUS_VALVE_OPERATION_TABLE_BEGIN+i;
							Reg_Infor_Temp.RegValue=value_setting[i];
							g_Vector_Write_Error.push_back(Reg_Infor_Temp);

							load_file_one_time.second_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
		   // FANOFF  VALVE SETTING
		   memset(value_setting,0,sizeof(value_setting));

		   get_value_setting(inf,value_setting);//value setting
		   Show_load_file_error_message(load_file_one_time,2,p_log_file);
		   load_file_one_time.second_step=true;
		   for(int i=0;i<7;i++)
		   {
			   if(value_setting[i]>=0)
			   {
				   int i_i=write_one(now_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+i,value_setting[i]);
				   if(p_log_file!=NULL)
				   {
					   if(i_i>0)
						   for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),173+i,value_setting[i]);
					   else
					   {
						   //for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),173+i,value_setting[i]);
						   Reg_Infor_Temp.regAddress=MODBUS_VALVE_OFF_TABLE_COAST+i;
						   Reg_Infor_Temp.RegValue=value_setting[i];
						   g_Vector_Write_Error.push_back(Reg_Infor_Temp);

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
	
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(find_sub_chars(buf,_T("//   FAN OFF")))
			break;

		if(wcscmp(buf,_T("//   DELAY TIMES"))==0)
		{
			int nFlag = product_register_value[7];
			int rows;
		    if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)||(product_register_value[7]== PM_TSTAT7)||(product_register_value[7]== PM_TSTAT8)
				|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
		    {
			rows=7;
		    }
			else
			{
			rows=5;
			}
		    
			int delay_setting[14]={0};
			get_delay_setting(inf,delay_setting);
			Show_load_file_error_message(load_file_one_time,3,p_log_file);
			load_file_one_time.third_step=true;
			for(int i=0;i<rows;i++)
			{
				if(delay_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i,delay_setting[i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK\r\n"),MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i,delay_setting[i]);
						else
						{
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i,delay_setting[i]);
							Reg_Infor_Temp.regAddress=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i;
							Reg_Infor_Temp.RegValue=delay_setting[i];
							g_Vector_Write_Error.push_back(Reg_Infor_Temp);
							load_file_one_time.third_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}
			for(int i=0;i<rows;i++)
			{
				if(delay_setting[rows+i]>=0)
				{
					int i_i=write_one(now_tstat_id,MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i,delay_setting[rows+i]);
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK\r\n"),MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i,delay_setting[rows+i]);
						else
						{
							Reg_Infor_Temp.regAddress=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i;
							Reg_Infor_Temp.RegValue=delay_setting[rows+i];
							g_Vector_Write_Error.push_back(Reg_Infor_Temp);
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i,delay_setting[rows+i]);
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
			/*if(tstat_version<CUSTOM_TABLE_FLOAT_VERSION)
			{*/
				int lookup_table_setting[23]={0};
				get_lookup_table_setting(inf,lookup_table_setting);
				Show_load_file_error_message(load_file_one_time,4,p_log_file);
				load_file_one_time.thurth_step=true;
				for(int i=0;i<22;i++)
				{
					if(lookup_table_setting[i]>=0)
					{
						int i_i=write_one(now_tstat_id,MODBUS_TABLE1_ZERO+i,lookup_table_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),MODBUS_TABLE1_ZERO+i,lookup_table_setting[i]);
							else
							{		
								Reg_Infor_Temp.regAddress=MODBUS_TABLE1_ZERO+i;
								Reg_Infor_Temp.RegValue=lookup_table_setting[i];
								g_Vector_Write_Error.push_back(Reg_Infor_Temp);
													
								//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),MODBUS_TABLE1_ZERO+i,lookup_table_setting[i]);
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
						int i_i=write_one(now_tstat_id,MODBUS_UNIVERSAL_OUTPUT_BEGIN+i,universal_value[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),254+i,universal_value[i]);
							else
							{
								//for_showing_text.Format(_T("register ID:%d vlaue:%d write Error******************\r\n"),254+i,universal_value[i]);
								Reg_Infor_Temp.regAddress=MODBUS_UNIVERSAL_OUTPUT_BEGIN+i;
								Reg_Infor_Temp.RegValue=universal_value[i];
								g_Vector_Write_Error.push_back(Reg_Infor_Temp);
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
						int i_i=write_one(now_tstat_id,MODBUS_UNIVERSAL_VALVE_BEGIN+i,value_setting[i]);
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),261+i,value_setting[i]);
							else
							{
								//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
								Reg_Infor_Temp.regAddress=MODBUS_UNIVERSAL_VALVE_BEGIN+i;
								Reg_Infor_Temp.RegValue=value_setting[i];
								g_Vector_Write_Error.push_back(Reg_Infor_Temp);

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
			write_one(now_tstat_id,MODBUS_INFO_BYTE,5);//184 register,to no restart,when you write the 185,118,121,128 register
			load_file_one_time.seven_step=true;
			get_var_write_var(inf,tstat_version,p_log_file,&load_file_one_time);//////a line //////////// a line
		    
		    write_input_output_var(inf,tstat_version,p_log_file,&load_file_one_time);


			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
	  /*  else */
	}
	
	int Try_Times=5;
	while(g_Vector_Write_Error.size()>0&&Try_Times>0){
		int vecInt_index = 0;
		for (vector <Reg_Infor>::iterator iter_index = g_Vector_Write_Error.begin(); 
			iter_index != g_Vector_Write_Error.end();) 
		{
			int i_i=write_one(now_tstat_id,iter_index->regAddress,iter_index->RegValue);
			if(p_log_file!=NULL)
			{
				if(i_i>0)
				{
					for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),iter_index->regAddress,iter_index->RegValue);
					vecInt_index = iter_index - g_Vector_Write_Error.begin(); 
					g_Vector_Write_Error.erase(iter_index);
					iter_index = g_Vector_Write_Error.begin() + vecInt_index; 
				}

				else
				{
					//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
					//g_Vector_Write_Error.push_back(Reg_Infor_Temp);
					//load_file_one_time.sixth_step=false;
					++iter_index;
				}
				change_showing_text_variable(for_showing_text);
				p_log_file->WriteString(for_showing_text.GetString());
			}

		}
		--Try_Times;
	}


	for (vector <Reg_Infor>::iterator iter_index = g_Vector_Write_Error.begin(); 
		iter_index != g_Vector_Write_Error.end(); ++iter_index) 
	{

		for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),iter_index->regAddress,iter_index->RegValue);

		//load_file_one_time.sixth_step=false;

		change_showing_text_variable(for_showing_text);
		p_log_file->WriteString(for_showing_text.GetString());
	}


	p_log_file->Flush();
	
		
	inf.close();
	Sleep(5000);
////LoadFile2Tstat_twofile(load_file_one_time,fn,p_log_file);

}

void LoadFile2Tstat67(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file)
{
   //int nSpecialValue=read_one(now_tstat_id,326);
	//write_one(g_tstat_id,324,0);
	//every step is false
	Reg_Infor Reg_Infor_Temp;
	CString for_showing_text;
	load_file_one_time.first_step=load_file_one_time.second_step=load_file_one_time.third_step=load_file_one_time.thurth_step=load_file_one_time.fifth_step=load_file_one_time.sixth_step=load_file_one_time.seven_step=load_file_one_time.eight_step=false;
	float tstat_version;
	tstat_version=get_tstat_version(now_tstat_id);///////////////////get version
	wifstream inf;//file
	inf.open(fn,ios_base::in);
	if (!Check_Config_File(inf))
	{
		AfxMessageBox(_T("It's not a good config file!"));
		return;
	}
	
	int fan_value[35]={0};//fan_value 
	for(int i=0;i<35;i++)
		fan_value[i]=0;	
    for_showing_text=_T("Begin.....");
	fan_number=get_fan_var(inf,fan_value);//fan value	
	 for_showing_text=_T("FAN.....");		
	load_file_one_time.first_step=true;
	for(int i=0;i<fan_number;i++)
	{
		for(int t_i=0;t_i<7;t_i++)
			{
				int real_fan_address=get_real_fan_address(fan_number-2,i,t_i);
				if(fan_value[i]>=0)
				{
					int i_i=write_one(now_tstat_id,real_fan_address,fan_value[i*7+t_i]);
					Reg_Infor_Temp.regAddress=real_fan_address;
					Reg_Infor_Temp.RegValue=fan_value[i*7+t_i];
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID: %d value:%d write OK\r\n"),real_fan_address,fan_value[i*7+t_i]);
						else
						{
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),real_fan_address,fan_value[i*7+t_i]);
							g_Vector_Write_Error.push_back(Reg_Infor_Temp);
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
					int i_i=write_one(now_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+i,value_setting[i]);
					Reg_Infor_Temp.regAddress=MODBUS_VALVE_OPERATION_TABLE_BEGIN+i;
					Reg_Infor_Temp.RegValue=value_setting[i];
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),173+i,value_setting[i]);
						else
						{
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),173+i,value_setting[i]);
							g_Vector_Write_Error.push_back(Reg_Infor_Temp);
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
	
	while(!inf.eof())
	{
		inf.getline(buf,1024);
		if(find_sub_chars(buf,_T("//   FAN OFF")))
			break;

		if(wcscmp(buf,_T("//   DELAY TIMES"))==0)
		{   int rows;
		   
			rows=7;
		  
		    
			int delay_setting[14]={0};
			get_delay_setting(inf,delay_setting);
			Show_load_file_error_message(load_file_one_time,3,p_log_file);
			load_file_one_time.third_step=true;
			for(int i=0;i<rows;i++)
			{
				if(delay_setting[i]>=0)
				{
					int i_i=write_one(now_tstat_id,MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i,delay_setting[i]);
					Reg_Infor_Temp.regAddress=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i;

					Reg_Infor_Temp.RegValue=delay_setting[i];
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK\r\n"),MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i,delay_setting[i]);
						else
						{
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),MODBUS_OUTPUT1_DELAY_OFF_TO_ON+i,delay_setting[i]);
							g_Vector_Write_Error.push_back(Reg_Infor_Temp);
							load_file_one_time.third_step=false;
						}
						change_showing_text_variable(for_showing_text);
						p_log_file->WriteString(for_showing_text);
					}
				}
			}

			for(int i=0;i<rows;i++)
			{
				if(delay_setting[rows+i]>=0)
				{
					int i_i=write_one(now_tstat_id,MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i,delay_setting[rows+i]);
					Reg_Infor_Temp.regAddress=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i;

					Reg_Infor_Temp.RegValue=delay_setting[rows+i];
					if(p_log_file!=NULL)
					{
						if(i_i>0)
							for_showing_text.Format(_T("register ID:%d vlaue:%d write OK\r\n"),MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i,delay_setting[rows+i]);
						else
						{
						   g_Vector_Write_Error.push_back(Reg_Infor_Temp);
							//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),MODBUS_OUTPUT1_DELAY_ON_TO_OFF+i,delay_setting[rows+i]);
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
		//else if(wcscmp(buf,_T("//   LOOKUP TABLE"))==0)//没有Lookup
		//{
		//	/*if(tstat_version<CUSTOM_TABLE_FLOAT_VERSION)
		//	{*/
		//		/*int lookup_table_setting[23]={0};
		//		get_lookup_table_setting(inf,lookup_table_setting);
		//		Show_load_file_error_message(load_file_one_time,4,p_log_file);
		//		load_file_one_time.thurth_step=true;
		//		for(int i=0;i<22;i++)
		//		{
		//			if(lookup_table_setting[i]>=0)
		//			{
		//				int i_i=write_one(now_tstat_id,MODBUS_TABLE1_ZERO+i,lookup_table_setting[i]);
		//				if(p_log_file!=NULL)
		//				{
		//					if(i_i>0)
		//						for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),MODBUS_TABLE1_ZERO+i,lookup_table_setting[i]);
		//					else
		//					{							
		//						for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),MODBUS_TABLE1_ZERO+i,lookup_table_setting[i]);
		//						load_file_one_time.thurth_step=false;
		//					}
		//					change_showing_text_variable(for_showing_text);
		//					p_log_file->WriteString(for_showing_text);
		//				}
		//			}					
		//		}
		//		if(p_log_file!=NULL)
		//		p_log_file->WriteString(_T("\r\n"));*/
		////	}
		//	//else if(tstat_version>=CUSTOM_TABLE_FLOAT_VERSION)
		//	//{
		//	//	int lookup_table_setting[23]={0};
		//	//	get_lookup_table_setting(inf,lookup_table_setting);
		//	//	Show_load_file_error_message(load_file_one_time,4,p_log_file);
		//	//	load_file_one_time.thurth_step=true;

		//	//	int ntemp=(int)lookup_table_setting[0];

		//	//	//table1
		//	//	{
		//	//	short high=ntemp/65536;
		//	//	short low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_ZERO,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_ZERO_HI,high);
		//	//	//2
		//	//	ntemp=lookup_table_setting[1];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFONE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFONE_HI,high);
		//	//	//3
		//	//	ntemp=lookup_table_setting[2];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_ONE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_ONE_HI,high);
		//	//	//4
		//	//	ntemp=(int)lookup_table_setting[3];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFTWO,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFTWO_HI,high);
		//	//	//5
		//	//	ntemp=lookup_table_setting[4];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_TWO,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_TWO_HI,high);
		//	//	//6
		//	//	ntemp=lookup_table_setting[5];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFTHREE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFTHREE_HI,high);
		//	//	//7
		//	//	ntemp=lookup_table_setting[6];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_THREE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_THREE_HI,high);
		//	//	//8

		//	//	ntemp=lookup_table_setting[7];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFFOUR,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFFOUR_HI,high);
		//	//	//9
		//	//	ntemp=lookup_table_setting[8];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_FOUR,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_FOUR_HI,high);
		//	//	//10
		//	//	ntemp=lookup_table_setting[9];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFFIVE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_HALFFIVE_HI,high);
		//	//	//11
		//	//	ntemp=lookup_table_setting[10];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_FIVE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE1_FIVE_HI,high);
		//	//	}

		//	//	//table 2
		//	//	{
		//	//	int ntemp=(int)lookup_table_setting[11];
		//	//	short high=ntemp/65536;
		//	//	short low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_ZERO,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_ZERO_HI,high);
		//	//	//2
		//	//	ntemp=lookup_table_setting[12];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;		
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFONE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFONE_HI,high);
		//	//	//3
		//	//	ntemp=lookup_table_setting[13];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_ONE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_ONE_HI,high);
		//	//	//4
		//	//	ntemp=(int)lookup_table_setting[14];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFTWO,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFTWO_HI,high);
		//	//	//5
		//	//	ntemp=lookup_table_setting[15];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_TWO,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_TWO_HI,high);
		//	//	//6
		//	//	ntemp=lookup_table_setting[16];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFTHREE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFTHREE_HI,high);
		//	//	//7
		//	//	ntemp=lookup_table_setting[17];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_THREE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_THREE_HI,high);
		//	//	//8

		//	//	ntemp=lookup_table_setting[18];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFFOUR,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFFOUR_HI,high);
		//	//	//9
		//	//	ntemp=lookup_table_setting[19];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_FOUR,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_FOUR_HI,high);
		//	//	//10
		//	//	ntemp=lookup_table_setting[20];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFFIVE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_HALFFIVE_HI,high);
		//	//	//11
		//	//	ntemp=lookup_table_setting[21];
		//	//	high=ntemp/65536;
		//	//	low=ntemp%65536;
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_FIVE,low);
		//	//	write_one(now_tstat_id, MODBUS_TABLE2_FIVE_HI,high);
		//	//	
		//	//	}
		//	//}

		//}
				else if(wcscmp(buf,_T("//   PID2 OFF TABLE"))==0)
				{
					int universal_value[7]={0};
					get_universal_fan_var(inf,universal_value);
					Show_load_file_error_message(load_file_one_time,5,p_log_file);
					load_file_one_time.fifth_step=true;
					for(int i=0;i<7;i++)
					{
						if(universal_value[i]>=0)          
						{
							int i_i=write_one(now_tstat_id,MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN+i,universal_value[i]);
							Reg_Infor_Temp.regAddress=MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN+i;

							Reg_Infor_Temp.RegValue=universal_value[i];
							if(p_log_file!=NULL)
							{
								if(i_i>0)
									for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),254+i,universal_value[i]);
								else
								{
									//for_showing_text.Format(_T("register ID:%d vlaue:%d write Error******************\r\n"),254+i,universal_value[i]);
									g_Vector_Write_Error.push_back(Reg_Infor_Temp);
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
			else if(wcscmp(buf,_T("//   PID2 OFF VALVE TABLE"))==0)
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
							int i_i=write_one(now_tstat_id,MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+i,value_setting[i]);
							Reg_Infor_Temp.regAddress=MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+i;

							Reg_Infor_Temp.RegValue=value_setting[i];
							if(p_log_file!=NULL)
							{
								if(i_i>0)
									for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),261+i,value_setting[i]);
								else
								{
								    g_Vector_Write_Error.push_back(Reg_Infor_Temp);
									//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
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
			else if(wcscmp(buf,_T("//   UNIVERSAL Auto RELAY OUTPUTS"))==0)
			{
				int universal_value[7]={0};
				get_universal_fan_var(inf,universal_value);
				Show_load_file_error_message(load_file_one_time,5,p_log_file);
				load_file_one_time.fifth_step=true;
				for(int i=0;i<7;i++)
				{
					if(universal_value[i]>=0)          
					{
						int i_i=write_one(now_tstat_id,MODBUS_UNIVERSAL_OUTPUT_BEGIN+i,universal_value[i]);
						Reg_Infor_Temp.regAddress=MODBUS_UNIVERSAL_OUTPUT_BEGIN+i;

						Reg_Infor_Temp.RegValue=universal_value[i];
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),254+i,universal_value[i]);
							else
							{g_Vector_Write_Error.push_back(Reg_Infor_Temp);
								//for_showing_text.Format(_T("register ID:%d vlaue:%d write Error******************\r\n"),254+i,universal_value[i]);
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
			else if(wcscmp(buf,_T("//   UNIVERSAL AUTO VALVE"))==0)
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
						int i_i=write_one(now_tstat_id,MODBUS_UNIVERSAL_VALVE_BEGIN+i,value_setting[i]);
						Reg_Infor_Temp.regAddress=MODBUS_UNIVERSAL_VALVE_BEGIN+i;

						Reg_Infor_Temp.RegValue=value_setting[i];
						if(p_log_file!=NULL)
						{
							if(i_i>0)
								for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),261+i,value_setting[i]);
							else
							{
								//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
								g_Vector_Write_Error.push_back(Reg_Infor_Temp);
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
			write_one(now_tstat_id,MODBUS_INFO_BYTE,5);//184 register,to no restart,when you write the 185,118,121,128 register
			load_file_one_time.seven_step=true;
			get_var_write_var(inf,tstat_version,p_log_file,&load_file_one_time);//////a line //////////// a line
			write_input_output_var(inf,tstat_version,p_log_file,&load_file_one_time);
			write_TStatAllLabel(inf,tstat_version,p_log_file,&load_file_one_time);
            write_TStatSchedual(inf, tstat_version, p_log_file, &load_file_one_time);
		//	write_one(now_tstat_id,MODBUS_UPDATE_STATUS,143);//////
			//reset 
			//write_one(now_tstat_id,184,0);//184 register,to no restart,when you write the 185,118,121,128 register
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
	}	
	int Try_Times=5;
	while(g_Vector_Write_Error.size()>0&&Try_Times>0){
	    int vecInt_index = 0;
		 	for (vector <Reg_Infor>::iterator iter_index = g_Vector_Write_Error.begin(); 
		 		iter_index != g_Vector_Write_Error.end();) 
		 	{
				int i_i=write_one(now_tstat_id,iter_index->regAddress,iter_index->RegValue);
				if(p_log_file!=NULL)
				{
					if(i_i>0)
						{
						for_showing_text.Format(_T("register ID:%d value:%d write OK\r\n"),iter_index->regAddress,iter_index->RegValue);
						vecInt_index = iter_index - g_Vector_Write_Error.begin(); 
						g_Vector_Write_Error.erase(iter_index);
						iter_index = g_Vector_Write_Error.begin() + vecInt_index; 
						}

					else
					{
						//for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),261+i,value_setting[i]);
						//g_Vector_Write_Error.push_back(Reg_Infor_Temp);
						//load_file_one_time.sixth_step=false;
						++iter_index;
					}
					change_showing_text_variable(for_showing_text);
					p_log_file->WriteString(for_showing_text.GetString());
				}
			
			}
		--Try_Times;
	}


	for (vector <Reg_Infor>::iterator iter_index = g_Vector_Write_Error.begin(); 
		iter_index != g_Vector_Write_Error.end(); ++iter_index) 
	{
		 
				for_showing_text.Format(_T("register ID:%d value:%d write Error******************\r\n"),iter_index->regAddress,iter_index->RegValue);
		
				//load_file_one_time.sixth_step=false;
		 
			change_showing_text_variable(for_showing_text);
			p_log_file->WriteString(for_showing_text.GetString());
	}


	p_log_file->Flush();
	inf.close();
	Sleep(5000);
////LoadFile2Tstat_twofile(load_file_one_time,fn,p_log_file);

}

void LoadT3Modules(load_file_every_step &load_file_one_time,CHAR* fn,CStdioFile*p_log_file)
{
   //int nSpecialValue=read_one(now_tstat_id,326);
	//write_one(g_tstat_id,324,0);
	//every step is false

	CString for_showing_text;
	float tstat_version;
	tstat_version=get_tstat_version(now_tstat_id);///////////////////get version
	wifstream inf;//file
	inf.open(fn,ios_base::in);
	 
    for_showing_text=_T("Begin.....");
	TCHAR buf[1024];
	
	while(!inf.eof())
	{
		inf.getline(buf,1024);
	 if(wcscmp(buf,_T("Reg No		Reg Value		Reg Name"))==0)
		{
			get_var_write_var_T3(inf,tstat_version,p_log_file);//////a line //////////// a line
	
			if(p_log_file!=NULL)
			p_log_file->WriteString(_T("\r\n"));
		}
	}	
	inf.close();
	Sleep(5000);
////LoadFile2Tstat_twofile(load_file_one_time,fn,p_log_file);

}
void LoadCO2NODE(load_file_every_step &load_file_one_time,CHAR* fn,CStdioFile*p_log_file)
{
    //int nSpecialValue=read_one(now_tstat_id,326);
    //write_one(g_tstat_id,324,0);
    //every step is false

    CString for_showing_text;
    float tstat_version;
    tstat_version=get_tstat_version(now_tstat_id);///////////////////get version
    wifstream inf;//file
    inf.open(fn,ios_base::in);

    for_showing_text=_T("Begin.....");
    TCHAR buf[1024];

    while(!inf.eof())
    {
        inf.getline(buf,1024);
        if(wcscmp(buf,_T("Reg No		Reg Value		Reg Name"))==0)
        {
            get_var_write_var_T3(inf,tstat_version,p_log_file);//////a line //////////// a line

            if(p_log_file!=NULL)
                p_log_file->WriteString(_T("\r\n"));
        }
    }	
    inf.close();
   // Sleep(5000);
    ////LoadFile2Tstat_twofile(load_file_one_time,fn,p_log_file);

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
		int firmwareVersion =int(float(nVersionHi) + float(nVersionLo/100.0));
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

void weekly_write_LC(CStdioFile &default_file,int schedule_id)
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
		Read_Multi(schedule_id,temp_buffer,4276+i*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
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
		 weekly_routines_insert_write_LC(default_file,schedule_id,i);
	}
}
void weekly_routines_insert_write_LC(CStdioFile &default_file,int schedule_id,int weekly_row_number)
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
	Read_Multi(schedule_id,on,6216 + WR_TIME_SIZE*weekly_row_number,0x48);
	//由上到下，由左到右，from up to lower ,from left to right
	Read_Multi(schedule_id,off,7656 + WR_TIME_SIZE*weekly_row_number,0x48);
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
void InputName_OutName_Description_LC(CStdioFile &default_file,int schedule_id){
	CString a_line,temp;	
	int i=0;

	a_line=_T("Input and Output Description\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\t");
	for (i=0;i<OUTPUTCARD_NUM_PER_INPUTCARD;i++)
	{
		temp=_T("");//clear
		temp.Format(_T("Out%02d\t"),i+1);
		a_line+=temp;
	}
	a_line+=_T("\n");
	default_file.WriteString(a_line.GetString());
 
	
	for (i=0;i<INPUTCARD_NUM;i++)
	{   
	    
		 

			OutCardName_Insert_Description_LC(default_file,schedule_id,i);

    }

}
void OutCardName_Insert_Description_LC(CStdioFile &default_file,int schedule_id,int Input_CardNo){
	CString a_line,temp;	
	int i=0;
	unsigned short temp_buffer[INPUT_DESCRIPTION_LENGTH]={0};
	unsigned char temp_charbuffer[INPUT_DESCRIPTION_LENGTH];
	unsigned short temp_buffer_out[INPUT_DESCRIPTION_LENGTH]={0};
	unsigned char temp_charbuffer_out[INPUT_DESCRIPTION_LENGTH];
	a_line=_T("");//clear
	a_line.Format(_T("Input%02d:"),Input_CardNo+1);
	Read_Multi(schedule_id,temp_buffer,INPUT_DESCRIPTION+Input_CardNo*INPUT_DESCRIPTION_LENGTH,INPUT_DESCRIPTION_LENGTH);
	if(temp_buffer[0]==255)
	{
		for(int itt=0;itt<INPUT_DESCRIPTION_LENGTH;itt++)
			temp_buffer[itt]=0;
	}
	for(int itt=0;itt<INPUT_DESCRIPTION_LENGTH;itt++)
	{
	   temp_charbuffer[itt] =(unsigned char) temp_buffer[itt];
	}
	temp=(CString)temp_charbuffer;
	a_line+=temp;
	a_line+=_T("\t");
	for (i=0;i<OUTPUTCARD_NUM_PER_INPUTCARD;i++)
	{
		/*temp=_T("");
		a_line+=temp;*/
		//a_line.Format(_T("Out%2d:"),i+1);
		Read_Multi(schedule_id,temp_buffer_out,OUTPUT_DESCRIPTION+(Input_CardNo*OUTPUTCARD_NUM_PER_INPUTCARD+i)*INPUT_DESCRIPTION_LENGTH,INPUT_DESCRIPTION_LENGTH);
		if(temp_buffer_out[0]==255)
		{
			for(int itt=0;itt<INPUT_DESCRIPTION_LENGTH;itt++)
				temp_buffer_out[itt]=0;
		}
		for(int itt=0;itt<INPUT_DESCRIPTION_LENGTH;itt++)
		{temp_charbuffer_out[itt] =(unsigned char) temp_buffer_out[itt];}
		
		temp=(CString)temp_charbuffer_out;
		temp+=_T("\t");
		a_line+=temp;
	}
	 
		a_line+=_T("\n");
		default_file.WriteString(a_line.GetString());
 
}
//多写到Buffer
void Switch_Config_LC(CStdioFile &default_file,int schedule_id){
	CString a_line,temp,temp1;	
	int i=0;
	 int tempint=0;
	a_line=_T("\nSwitch Config\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\t//Auto/Manaul:A/M\tOT:OverrideTime\tMB:ManualBlock\tDT:DelayTime\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\tType\t\tOT\tA/M\tMB\tDT\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_Type[INPUTCARD_NUM],temp_OT[INPUTCARD_NUM],temp_AM[INPUTCARD_NUM],temp_MB[INPUTCARD_NUM],temp_DT[INPUTCARD_NUM];
	Read_Multi(schedule_id,&temp_Type[0],SWITCH_TYPE,INPUTCARD_NUM);
	Read_Multi(schedule_id,&temp_OT[0],OVER_TIME,INPUTCARD_NUM);
	Read_Multi(schedule_id,&temp_AM[0],INPUT_AM,INPUTCARD_NUM);
	Read_Multi(schedule_id,&temp_MB[0],INPUT_MB,INPUTCARD_NUM);
	Read_Multi(schedule_id,&temp_DT[0],DELAY_TIME_SWITCH,INPUTCARD_NUM);
	for (i=0;i<INPUTCARD_NUM;i++)
	{   a_line=_T("");
		temp.Format(_T("Switch%02d:"),i+1);
		temp+=_T("\t");
		a_line+=temp;
	 
		//temp.Format(_T("%d\t"),temp_Type[i]);
		//a_line+=temp;
		//Type
		if (temp_Type[i]&1)
		{
			temp=_T("Low");
		}
		else if(temp_Type[i]&2)
		{
			temp=_T("High");
		}
		else if(temp_Type[i]&4)
		{
			temp=_T("Edge");
		}
		
		if (temp_Type[i]&16)
		{
			temp+=_T(":OnOnly\t");
		}
		else if (temp_Type[i]&32)
		{
			temp+=_T(":On/Off\t");
		}

		a_line+=temp;

		/////

		temp.Format(_T("%d\t"),temp_OT[i]);
		a_line+=temp;
		tempint=temp_AM[i];
		if (tempint==0)
		{
		temp=_T("AUTO\t");
		} 
		else if(tempint==1)
		{
		temp=_T("MAN\t");
		}
	 
		a_line+=temp;
		tempint=temp_MB[i];
		if (tempint==0)
		{
			temp=_T("OFF\t");
		} 
		else if(tempint==1)
		{
			temp=_T("ON\t");
	    }
		a_line+=temp;
		 
		temp.Format(_T("%d\n"),temp_DT[i]);
		a_line+=temp;
		default_file.WriteString(a_line.GetString());

		
	}

}
//多写到buffer
void Enable_Disable_Manual_Control_LC(CStdioFile &default_file,int schedule_id){
	CString a_line,temp;	
	int i=0;

	a_line=_T("Enable/Disable Manual Control\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\t");
	for (int j=0;j<OUTPUTCARD_NUM_PER_INPUTCARD;j++)
	{

		temp=_T("");//clear
		temp.Format(_T("Out%02d\t\t"),j+1);
		a_line+=temp;
	}
	a_line+=_T("\n");
	default_file.WriteString(a_line.GetString());
	int tempint=0;
	unsigned short DataBuffer[480];
	for (i=0;i<6;i++)
	{
	  Read_Multi(schedule_id,&DataBuffer[i*80],ENABLE_DISABLE_MC+80*i,80);
	}
	for (i=0;i<INPUTCARD_NUM;i++)
	{   
		a_line=_T("");//clear
		a_line.Format(_T("Switch%02d:\t"),i+1);
		  
		 for (int j=0;j<OUTPUTCARD_NUM_PER_INPUTCARD;j++)
		 {  
			 
			 tempint=DataBuffer[OUTPUTCARD_NUM_PER_INPUTCARD*i+j];
			 if (tempint&128)
			 {
			 temp=_T("Enable");
			 } 
			 else
			 {
			 temp=_T("Disable");
			 }
			 if (tempint&1)
			 {
			 temp+=_T(":On\t");
			 } 
			 else
			 {
			 temp+=_T(":Off\t");
			 }
			// temp.Format(_T("%02d\t"),tempint);
			 a_line+=temp;
		 }
		 a_line+=_T("\n");
		 default_file.WriteString(a_line.GetString());

	}
}
//多写到buffer
void Group_Config_LC(CStdioFile &default_file,int schedule_id){
	CString a_line,temp;	
	int i=0;
	int tempint=0;
	a_line=_T("Group Config\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\t//Auto/Manaul:A/M\tMB:ManualBlock\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\tA/M\tMB\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_AM[GROUP_NUM],temp_MB[GROUP_NUM];
	Read_Multi(schedule_id,&temp_AM[0],GROUP_AM,GROUP_NUM);
	Read_Multi(schedule_id,&temp_MB[0],GROUP_MB,GROUP_NUM);
	for (i=0;i<GROUP_NUM;i++)
	{   
	a_line=_T("");
	temp.Format(_T("Group%02d:"),i+1);/////???为什么要两个\t才能对齐呢？？
	temp+=_T("\t");
	a_line+=temp;

	 
	tempint=temp_AM[i];
	if (tempint==0)
	{
		temp=_T("AUTO\t");
	} 
	else if(tempint==1)
	{
		temp=_T("MAN\t");
	}
	/*temp.Format(_T("%d\t"),tempint);*/
	a_line+=temp;
	tempint=temp_MB[i];
	if (tempint==0)
	{
		temp=_T("OFF\n");
	} 
	else if(tempint==1)
	{
		temp=_T("ON\n");
	}
	a_line+=temp;
	default_file.WriteString(a_line.GetString());
	}

}
//多写到buffer
void Input_Output_Mapping_LC(CStdioFile &default_file,int schedule_id){
	CString a_line,temp;	
	int i=0;

	a_line=_T("Input Output Mappings Config\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\t");
	for (int j=0;j<OUTPUTCARD_NUM_PER_INPUTCARD;j++)
	{

		temp=_T("");//clear
		temp.Format(_T("Out%02d:ID%02d\t"),j+1,j+1);
		a_line+=temp;
	}
	a_line+=_T("\n");
	default_file.WriteString(a_line.GetString());
	int tempint=0;
	unsigned short DataBuffer[ONE_SWITCH_LENGTH];
	for (i=0;i<INPUTCARD_NUM;i++)
	{   
		a_line=_T("");//clear
		a_line.Format(_T("Switch%02d:\t"),i+1);
		Read_Multi(schedule_id,&DataBuffer[0],INPUT_OUTPUT_MAPPING+i*ONE_SWITCH_LENGTH,ONE_SWITCH_LENGTH);
		int id,outputvalue;
		for (int j=0;j<OUTPUTCARD_NUM_PER_INPUTCARD;j++)
		{    id=DataBuffer[3*j];
		     outputvalue=DataBuffer[1+3*j]+DataBuffer[2+3*j]*256;
			 temp.Format(_T("%d:%d\t\t"),tempint,id);
			a_line+=temp;
		}
		a_line+=_T("\n");
		default_file.WriteString(a_line.GetString());
	}

}
//多写到buffer
void Group_Output_Mapping_LC(CStdioFile &default_file,int schedule_id){
	CString a_line,temp;	
	int i=0;

	a_line=_T("Group Output Mappings Config\n");
	default_file.WriteString(a_line.GetString());
	a_line=_T("\t\t");
	for (int j=0;j<OUTPUTCARD_NUM_PER_INPUTCARD;j++)
	{

		temp=_T("");//clear
		temp.Format(_T("Out%02d:ID%02d\t"),j+1,j+1);
		a_line+=temp;
	}
	a_line+=_T("\n");
	default_file.WriteString(a_line.GetString());
	int tempint=0;
	unsigned short DataBuffer[ONE_SWITCH_LENGTH];
	for (i=0;i<GROUP_NUM;i++)
	{   
		a_line=_T("");//clear
		a_line.Format(_T("Group%02d:\t"),i+1);
		Read_Multi(schedule_id,&DataBuffer[0],GROUP_OUTPUT_MAPPING+i*ONE_GROUP_LENGTH,ONE_GROUP_LENGTH);
		int id,outputvalue;
		for (int j=0;j<OUTPUTCARD_NUM_PER_INPUTCARD;j++)
		{ 
		id=DataBuffer[3*j];
		outputvalue=DataBuffer[1+3*j]+DataBuffer[2+3*j]*256;
		temp.Format(_T("%d:%d\t\t"),outputvalue,id);
		a_line+=temp;
		}
		a_line+=_T("\n");
		default_file.WriteString(a_line.GetString());
	}

}

void save_T3Modules_file(TCHAR* fn,int schedule_id){
CStdioFile default_file;
CString line;
if(default_file.Open(fn,CFile::modeCreate | CFile::modeWrite)!=0){
	default_file.WriteString(_T("T3 Modules Config File\n"));
	default_file.WriteString(_T("Model :"));
	CString productname=GetProductName(product_register_value[7]);
	default_file.WriteString(productname);
	default_file.WriteString(_T("\n"));

	T3Register temp;
	vector<T3Register>VectorT3Register;
	VectorT3Register.clear();
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
#if 1
	CString SQL = _T("select * from T3_RegisterList");
	q = SqliteDBT3000.execQuery((UTF8MBSTR)SQL);
	_variant_t vartemp;
	while(!q.eof())
	{
		temp.regID=q.getIntField("RegID");
	 
		temp.regName =q.getValuebyName(productname);
		q.nextRow();
		if (temp.regName.CompareNoCase(_T("RESERVED"))==0)
		continue;
		VectorT3Register.push_back(temp);
	}
	SqliteDBT3000.closedb();
#endif 
    default_file.WriteString(_T("Reg No,Reg Value,Reg Name\n"));
	vector<T3Register>::iterator iter;
	for (iter=VectorT3Register.begin();iter!=VectorT3Register.end();iter++)
	{
		  
		 line.Format(_T("%d,	%d,	%s\n"),iter->regID,product_register_value[iter->regID],iter->regName.GetString());
	     default_file.WriteString(line);
	}
	default_file.Flush();
	default_file.Close();
}
else
{
AfxMessageBox(_T("Open file failure!"));
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

void variable_write_LC(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	a_line=_T("VARIABLE LIST\n");
	default_file.WriteString(a_line.GetString());
	a_line.Format(_T("Input Filter Time:%d\n"),read_one(g_tstat_id,INPUT_FILTER_TIME));
	default_file.WriteString(a_line.GetString());
    
	a_line.Format(_T("Relay Pulse Duration:%d\n"),read_one(g_tstat_id,RELAY_PULSE));
	default_file.WriteString(a_line.GetString());

	a_line.Format(_T("Delay Between Relay Pulses:%d\n"),read_one(g_tstat_id,DELAY_RELAY_PULSE));
	default_file.WriteString(a_line.GetString());
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
void annual_write_LC(CStdioFile &default_file,int schedule_id)
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
		Read_Multi(schedule_id,temp_buffer,4896+i*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
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
		annual_routines_insert_write_LC(default_file,schedule_id,i);
	}
}
void annual_routines_insert_write_LC(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	unsigned short the_days[AR_TIME_SIZE];
	CString a_line=_T("\t"),temp=_T("");	
    Read_Multi(schedule_id,the_days,5480 + AR_TIME_SIZE*weekly_row_number,AR_TIME_SIZE);//get from network
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
		temp.Format(_T("%02d,"),the_days[i]);
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
void Group_write(CStdioFile &default_file,int schedule_id)
{
	CString a_line;	
	int i=0;
	a_line=_T("GROUP SCHEDULE\n");
	default_file.WriteString(a_line.GetString());

	a_line=_T("Num\tLable\tA/M\tValue\tSchedule1\tstate1\tSchedule2\tstate2\n");
	default_file.WriteString(a_line.GetString());
	unsigned short temp_buffer[ID_SIZE]={0};
	unsigned short temp_lable[GROUP_LABLE_SIZE]={0};
	for(i=0;i<40;i++)
	{
		a_line=_T("");//clear
		Read_Multi(schedule_id,temp_lable,14136+i*GROUP_LABLE_SIZE,GROUP_LABLE_SIZE);
		Read_Multi(schedule_id,temp_buffer,5360+i*ID_SIZE,ID_SIZE);
		if(temp_lable[0]==255)
		{
			for(int itt=0;itt<GROUP_LABLE_SIZE;itt++)
				temp_lable[itt]=0;

		}
		if(temp_buffer[0]==255)
		{
			for(int itt=0;itt<ID_SIZE;itt++)
				temp_buffer[itt]=0;
			 
		}			
		description3_group temp_description;
		char *p;
		p=(char *)&temp_description;///////////////*****pointer   attention
		
		for(int j=0;j<GROUP_LABLE_SIZE;j++)
				(*(p++))=(char)temp_lable[j];
		for(int j=GROUP_LABLE_SIZE;j<GROUP_LABLE_SIZE+ID_SIZE;j++)
			(*(p++))=(char)temp_buffer[j];

		CString lable,state1,state2,a_m,value;
		lable=(CString)temp_description.full_label+_T("\t");
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
		a_line+=lable;
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
void weekly_read_LC(CStdioFile & default_file,int schedule_id, CStdioFile& logfile)
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
			Write_Multi(schedule_id,temp_buffer,WEEKLY_ROUTINE_DESCRIPTION+j*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
			weekly_routines_insert_read_LC(default_file,schedule_id,j);
		}
	}while(1);	
}
void weekly_routines_insert_read_LC(CStdioFile &default_file,int schedule_id,int weekly_row_number)
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
			Write_Multi(schedule_id,on_time,WEEKLY_ROUTINE_ON_TIME + WR_TIME_SIZE*weekly_row_number,WR_TIME_SIZE);
			Write_Multi(schedule_id,off_time,WEEKLY_ROUTINE_OFF_TIME  + WR_TIME_SIZE*weekly_row_number,WR_TIME_SIZE);
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
void annual_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile)
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

		if(a_line.Find(_T("ANNUAL ROUTINES"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("A/M"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("GROUP SCHEDULE"))!=-1)//annual routines start
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
			Write_Multi(schedule_id,temp_buffer,ANNUAL_ROUTINE_DESCRIPTION+j*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
			annual_routines_insert_read_LC(default_file,schedule_id,j);
		}
	}while(1);	
}
void annual_routines_insert_read_LC(CStdioFile &default_file,int schedule_id,int weekly_row_number)
{
	CString a_line;
	if(default_file.ReadString(a_line)==false)
		return;
	unsigned char the_days[AR_TIME_SIZE];
	for(int i=0;i<AR_TIME_SIZE;i++)
		the_days[i]=(char)_wtoi(get_left_first_array(a_line));
	Write_Multi(schedule_id,the_days,ANNUAL_ROUTINE_TIME + AR_TIME_SIZE*weekly_row_number,AR_TIME_SIZE);
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
void Group_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing;
	 
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################
		unsigned char lable_buffer[GROUP_LABLE_SIZE]={0};
		if(a_line.Find(_T("GROUP SCHEDULE"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("A/M"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Input and Output Description"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			int j=(_wtoi(a_line))-1;
			unsigned char flag=0;
			get_left_first_array(a_line);//clear left number
			CString group_discription=get_left_first_array(a_line);
			//transfer string to char array
			for (int k=0;k<group_discription.GetLength();k++)
			{
			  if (k>=10)
			  break;
			  lable_buffer[k]=(char)group_discription.GetAt(k);
			}
			//write into group description
			Write_Multi(schedule_id,lable_buffer,GROUP_DESCRIPTION_REG+j*GROUP_LABLE_SIZE,GROUP_LABLE_SIZE);

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

			Write_Multi(schedule_id,temp_buffer,GROUP_DESCRIPTION_REG+j*ID_SIZE,ID_SIZE);
			 
			 
		}
	}while(1);		
}
void Input_Output_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing;
	int i = 0;
	int j=-1;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################
		unsigned char lable_buffer[GROUP_LABLE_SIZE]={0};
		if(a_line.Find(_T("Input and Output Description"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("//"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Switch Config"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			++j;
			 
			get_left_first_array(a_line);//clear left number
			CString group_discription=get_left_first_array(a_line);
			//transfer string to char array
			for (int k=0;k<group_discription.GetLength();k++)
			{
				if (k>=10)
					break;
				lable_buffer[k]=(char)group_discription.GetAt(k);
			}
			Write_Multi(schedule_id,lable_buffer,INPUT_DESCRIPTION+j*GROUP_LABLE_SIZE,GROUP_LABLE_SIZE);

			for (i=0;i<OUTPUTCARD_NUM_PER_INPUTCARD;i++)
			{
			 CString out_dis=get_left_first_array(a_line);
			 unsigned char out_lable_buffer[GROUP_LABLE_SIZE]={0};
			 for (int k=0;k<out_dis.GetLength();k++)
			 {
				 if (k>=10)
					 break;
				 out_lable_buffer[k]=(char)out_dis.GetAt(k);
			 }
			 
			  Write_Multi(schedule_id,out_lable_buffer,OUTPUT_DESCRIPTION+(j*OUTPUTCARD_NUM_PER_INPUTCARD+i)*INPUT_DESCRIPTION_LENGTH,INPUT_DESCRIPTION_LENGTH);
			}
		}
	}while(1);

}
void Switch_Config_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing,strTemp;
	int i = 0;
	int j=-1;
	int temp;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################
		unsigned char lable_buffer[GROUP_LABLE_SIZE]={0};
		if(a_line.Find(_T("Switch Config"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("//"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Group Config"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			++j;
			unsigned char flag=0;
			get_left_first_array(a_line);//clear left number
			strTemp=get_left_first_array(a_line);
			temp=0;
			if (strTemp.Find(_T("High"))!=-1)
			{
			temp+=1;
			}
			else if (strTemp.Find(_T("Low"))!=-1)
			{
			temp+=2;
			}
			else if (strTemp.Find(_T("Edge"))!=-1)
			{
			temp+=4;
			}
		
		   strTemp=get_left_first_array(a_line);

			if (strTemp.Find(_T("OnOnly"))!=-1)
			{
				temp+=16;
			}
			else if (strTemp.Find(_T("On/Off"))!=-1)
			{
				temp+=32;
			}
			
			write_one(schedule_id,SWITCH_TYPE+j,temp);
			 
			strTemp=get_left_first_array(a_line);
			temp=_wtoi(strTemp);//Override
			write_one(schedule_id,OVER_TIME+j,temp);

			strTemp=get_left_first_array(a_line);
			
			if (strTemp.Find(_T("AUTO"))!=-1)
			{
			temp=0;
			} 
			else
			{
			temp=1;
			}
		//	temp=_wtoi(strTemp);//Override
			write_one(schedule_id,INPUT_AM+j,temp);

			strTemp=get_left_first_array(a_line);
			if (strTemp.Find(_T("OFF"))!=-1)
			{
				temp=0;
			} 
			else
			{
				temp=1;
			}
			write_one(schedule_id,INPUT_MB+j,temp);

			strTemp=get_left_first_array(a_line);
			temp=_wtoi(strTemp);//DelayTime
			write_one(schedule_id,DELAY_TIME_SWITCH+j,temp);
			
		}
	}while(1);		
}
void Group_Config_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing,strTemp;
	int i = 0;
	int j=-1;
	int temp;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################
		unsigned char lable_buffer[GROUP_LABLE_SIZE]={0};
		if(a_line.Find(_T("Group Config"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("//"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Enable/Disable Manual Control"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
			++j;
			unsigned char flag=0;
			get_left_first_array(a_line);//clear left number

			strTemp=get_left_first_array(a_line);
			if (strTemp.Find(_T("AUTO"))!=-1)
			{
				temp=0;
			} 
			else
			{
				temp=1;
			}
			//	temp=_wtoi(strTemp);//Override
			write_one(schedule_id,GROUP_AM+j,temp);

			strTemp=get_left_first_array(a_line);
			if (strTemp.Find(_T("OFF"))!=-1)
			{
				temp=0;
			} 
			else
			{
				temp=1;
			}
			write_one(schedule_id,GROUP_MB+j,temp);
		}
	}while(1);		
}
void Enable_Disable_MC_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing;
	int i = 0;
	int j=-1;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################
	 
		if(a_line.Find(_T("Enable/Disable Manual Control"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("//"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Input Output Mappings Config"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{
		   
			++j;

			get_left_first_array(a_line);//clear left number
			for (i=0;i<OUTPUTCARD_NUM_PER_INPUTCARD;i++)
			{   unsigned short temp_value=0;
				CString ENDIS=get_left_first_array(a_line);
				CString OnOff=get_left_first_array(a_line);
				if(ENDIS.Find(_T("Disable"))!=-1){
				temp_value+=0;
				}
				else{
				temp_value+=128;
				}

				if (OnOff.Find(_T("Off"))!=-1)
				{
				temp_value+=0;
				} 
				else
				{
				temp_value+=1;
				}
				write_one(schedule_id,ENABLE_DISABLE_MC+j*OUTPUTCARD_NUM_PER_INPUTCARD+i,temp_value);
		   }
		}
	}while(1);

}

void Input_Output_Mapping_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing;
	int i = 0;
	int j=-1;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################

		if(a_line.Find(_T("Input Output Mappings Config"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Out01:ID01"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Group Output Mappings Config"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{

			++j;

			get_left_first_array(a_line);//clear left number
			for (i=0;i<OUTPUTCARD_NUM_PER_INPUTCARD;i++)
			{
			int output_value=0;
			int address=0;   
			unsigned short high,low=0;
			CString outputvalue=get_left_first_array(a_line);
			             get_left_first_array(a_line);//ID不要了
			 output_value=_wtoi(outputvalue);
			 high=output_value/256;
			 low=output_value%256;

			 address=INPUT_OUTPUT_MAPPING+j*ONE_GROUP_LENGTH+3*i;
			 write_one(schedule_id,address+1,low);
			 write_one(schedule_id,address+2,high);
			}
		}
	}while(1);

}

void Group_Output_Mappings_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile){
	CString a_line, strShowing;
	int i = 0;
	int j=-1;
	do{
		if(default_file.ReadString(a_line)==false)
			return;

		//###############################
		strShowing = a_line+_T("\n");
		change_showing_text_variable(strShowing);
		logfile.WriteString(strShowing);			
		//###############################

		if(a_line.Find(_T("Group Output Mappings Config"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("Out01:ID01"))!=-1)//annual routines start
			continue;
		if(a_line.Find(_T("VARIABLE LIST"))!=-1)//annual routines start
			break;
		if(a_line.Find(_T(":"))!=-1)			
		{

			++j;

			get_left_first_array(a_line);//clear left number
			for (i=0;i<OUTPUTCARD_NUM_PER_INPUTCARD;i++)
			{
				int output_value=0;
				int address=0;   
				unsigned short high,low=0;
				CString outputvalue=get_left_first_array(a_line);
				get_left_first_array(a_line);//ID不要了
				output_value=_wtoi(outputvalue);
				high=output_value/256;
				low=output_value%256;

				address=GROUP_OUTPUT_MAPPING+j*ONE_GROUP_LENGTH+3*i;
				write_one(schedule_id,address+1,low);
				write_one(schedule_id,address+2,high);
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

void variable_read_LC(CStdioFile &default_file,int schedule_id, CStdioFile& logfile)
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
			if(a_line.Find(LC_VARIABLE_LIST[0])!=-1)
			{
				a_line=a_line.Right(a_line.GetLength()-j);
				j=_wtoi(get_left_first_array(a_line));
				write_one(schedule_id,INPUT_FILTER_TIME,j);	
			}
		}
		else if(a_line.Find(LC_VARIABLE_LIST[1])!=-1)
		{
			a_line=a_line.Right(a_line.GetLength()-j);
			j=_wtoi(get_left_first_array(a_line));
			write_one(schedule_id,RELAY_PULSE,j);
		}
		else if(a_line.Find(LC_VARIABLE_LIST[2])!=-1)
		{
			a_line=a_line.Right(a_line.GetLength()-j);
			j=_wtoi(get_left_first_array(a_line));
			write_one(schedule_id,DELAY_RELAY_PULSE,j);
		}
	}while(1);	
}

void load_file_2_schedule_NC(TCHAR* fn,int schedule_id, CStdioFile& file)
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
void load_file_2_schedule_LC(TCHAR* fn,int schedule_id, CStdioFile& file)
{
    CStdioFile default_file;
   
	CString filepath_2(fn);
	 
    
	if(default_file.Open(filepath_2,CFile::modeRead)!=0)
	{
		CString a_line;
		default_file.ReadString(a_line);
		
		if(a_line.Find(_T("LC"))==-1)
		{
			AfxMessageBox(_T("Wrong File!"));
			return ;
		}
		//write_one(schedule_id,16,0xAA);
		//Sleep(5);//here sleep for quick load file
		showing_text = _T("Start read weekly routine...");
		default_file.ReadString(a_line);
		default_file.ReadString(a_line);

       weekly_read_LC(default_file,schedule_id, file);
	   showing_text = _T("Start read annual routine...");
	  annual_read_LC(default_file,schedule_id, file);
		 showing_text = _T("Start read  Group_read_LC  ...");
	    Group_read_LC(default_file,schedule_id, file);
	 	showing_text = _T("Start read Input Output Description...");
	   
		/////////////////////////////////////////////last work
		 Input_Output_read_LC(default_file,schedule_id, file);
		 showing_text = _T("Start read Switch config...");
		 Switch_Config_read_LC(default_file,schedule_id, file);
		  showing_text = _T("Start read Group Config...");
		   Group_Config_read_LC(default_file,schedule_id, file);
		    showing_text = _T("Start read Enable Disable Manual Control...");
		  Enable_Disable_MC_read_LC(default_file,schedule_id, file);
		   showing_text = _T("Start read Input Output Mapping config...");
		 Input_Output_Mapping_read_LC(default_file,schedule_id, file);
		  showing_text = _T("Start read Croup Output Mapping  config...");
		 Group_Output_Mappings_read_LC(default_file,schedule_id, file);
		  showing_text = _T("Start read Some Variables config...");
		variable_read_LC(default_file,schedule_id, file);
		default_file.Close();
	}
	else
		AfxMessageBox(_T("Open file failure!"));
}
void save_schedule_2_file_LC(TCHAR* fn,int schedule_id)
{
	CStdioFile default_file;
	if(default_file.Open(fn,CFile::modeCreate | CFile::modeWrite)!=0)
	{
		//default_file.WriteString(_T("NET WORK\n"));
		default_file.WriteString(_T("LC Config File\n"));
		default_file.WriteString(_T("Model : LightingController \n"));
		int nVersionLo = Read_One(g_tstat_id, 4);
		int	nVersionHi = Read_One(g_tstat_id, 5);
		float firmwareVersion =float(nVersionHi) + float(nVersionLo/100.0);
		CString strFirmware;
		strFirmware.Format(_T("%0.2f"), firmwareVersion);
		CString strTips;
		default_file.WriteString(_T("version : ")+strFirmware+_T("\n"));

		strTips=_T("Weekly Routines Saving......");
		SetPaneString(1, strTips);
		weekly_write_LC(default_file,schedule_id);

		strTips=_T("Annual Routines Saving......");
		SetPaneString(1, strTips);
		annual_write_LC(default_file,schedule_id);

		strTips=_T("Group Schedule Saving......");
		SetPaneString(1, strTips);
		Group_write(default_file,schedule_id);

		strTips=_T("Input Output Name Description Saving......");
		SetPaneString(1, strTips);
		InputName_OutName_Description_LC(default_file,schedule_id);

		strTips=_T("Switch Config Saving......");
		SetPaneString(1, strTips);
		Switch_Config_LC(default_file,schedule_id);

		strTips=_T("Group Config Saving......");
		SetPaneString(1, strTips);
		Group_Config_LC(default_file,schedule_id);

		strTips=_T("Enable Disable Manual Control Saving......");
		SetPaneString(1, strTips);
		Enable_Disable_Manual_Control_LC(default_file,schedule_id);

		strTips=_T("Input-Output-Mapping Saving......");
		SetPaneString(1, strTips);
		Input_Output_Mapping_LC(default_file,schedule_id);

		strTips=_T("Group-Output-Mapping Saving......");
		SetPaneString(1, strTips);
		Group_Output_Mapping_LC(default_file,schedule_id);

		strTips=_T("Some important Variable Saving......");
		SetPaneString(1, strTips);
		variable_write_LC(default_file,schedule_id); 

		/////////////////////////////////////////////last work
		default_file.Flush();
		default_file.Close();
	}
	else
		AfxMessageBox(_T("Open file failure!"));

}
void save_write_input_output(wofstream & out){
_Twrite_to_file_a_line(out,_T("//Input Name Config"));//space
	CString str1;
	int m_outRows,m_inRows;
	
	//判断Input output 行数
	#if 1
	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	switch (nModel)
	{
	case 3:
	case 2:
	case 1:
		{
			m_outRows=6;
			m_inRows=3;
		}

		break;
	case 4:
		{
			m_outRows=6;
			m_inRows=5;

		}
		break;	
	case PM_TSTAT7:
	case PM_TSTAT5D:  // 5D 同 TStat7
		{
			m_outRows=8;
			m_inRows=5;

		}break;
    case PM_TSTAT6:
        {
            m_outRows=8;
            m_inRows=12;
        }break;
    case PM_TSTAT5i:
        {
            m_outRows=8;
            m_inRows=12;
        }break;
    case PM_TSTAT8:
        {
            m_outRows=8;
            m_inRows=12;
        }break;
	case PM_TSTAT8_WIFI:
	{
		m_outRows = 8;
		m_inRows = 12;
	}break;
	case PM_TSTAT8_OCC:
	{
		m_outRows = 8;
		m_inRows = 12;
	}break;
	case PM_TSTAT7_ARM:
	{
		m_outRows = 8;
		m_inRows = 12;
	}break;
	case PM_TSTAT8_220V:
	{
		m_outRows = 8;
		m_inRows = 12;
	}break;
    case PM_TSTAT5E:
        {
            m_outRows=8;
            m_inRows=11;
        }break;
    case PM_PM5E:
        {
            m_outRows=8;
            m_inRows=11;
        }break;
	case 17:
		{
			m_outRows=6;
			m_inRows=5;
		}
		break;
	case 18:
		{
			m_outRows=8;
			m_inRows=4;
		}
		break;
	case 19:
		{
			m_outRows=8;
			m_inRows=9;
		}
		break;
	case PM_PRESSURE:
		{
			m_outRows=8;
			m_inRows=2;
		}
		break;

	default:break;
	}
    #endif
	int row=1;
	int rows=m_inRows-1;
	while (row<=rows)
	{
		switch (row)
		{
		case 1:
		{
		str1=_T("Input1_Name: ");
		str1+=m_tstat_input_data.at(0).InputName.StrValue;
		_Twrite_to_file_a_line(out,str1);
		}break;
		case 2:
			{
				str1=_T("Input2_Name: ");
				str1+=m_tstat_input_data.at(1).InputName.StrValue;
				_Twrite_to_file_a_line(out,str1);
			}break;
		case 3:
			{
				str1=_T("Input3_Name: ");
				str1+=m_tstat_input_data.at(2).InputName.StrValue;

				_Twrite_to_file_a_line(out,str1);
			}break;
		case 4:
			{
				str1=_T("Input4_Name: ");
				str1+=m_tstat_input_data.at(3).InputName.StrValue;

				_Twrite_to_file_a_line(out,str1);
			}break;
		case 5:
			{
				str1=_T("Input5_Name: ");
				str1+=m_tstat_input_data.at(4).InputName.StrValue;

				_Twrite_to_file_a_line(out,str1);
			}break;
		case 6:
			{
				str1=_T("Input6_Name: ");
				str1+=m_tstat_input_data.at(5).InputName.StrValue;

				_Twrite_to_file_a_line(out,str1);
			}break;
		case 7:
			{
				str1=_T("Input7_Name: ");
				str1+=m_tstat_input_data.at(6).InputName.StrValue;

				_Twrite_to_file_a_line(out,str1);
			}break;

		case 8:
			{
				str1=_T("Input8_Name: ");
				str1+=m_tstat_input_data.at(7).InputName.StrValue;
				_Twrite_to_file_a_line(out,str1);
			}break;
		 
		 
	}
	   
	   row++;
   }

 _Twrite_to_file_a_line(out,_T("//Output Name Config"));//space
 str1=_T("Output1_Name: ");
 str1+=m_tstat_output_data.at(0).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);
 str1=_T("Output2_Name: ");
 str1+=m_tstat_output_data.at(1).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);
 str1=_T("Output3_Name: ");
 str1+=m_tstat_output_data.at(2).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);
 str1=_T("Output4_Name: ");
 str1+=m_tstat_output_data.at(3).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);
 str1=_T("Output5_Name: ");
 str1+=m_tstat_output_data.at(4).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);
 str1=_T("Output6_Name: ");
 str1+=m_tstat_output_data.at(5).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);
 str1=_T("Output7_Name: ");
 str1+=m_tstat_output_data.at(6).OutputName.StrValue;
 _Twrite_to_file_a_line(out,str1);

 _Twrite_to_file_a_line(out,_T("//Input Output Range Config"));//space

 int m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
int  m_crange,inoutputno;
CppSQLite3DB SqliteDBBuilding;
CppSQLite3Table table;
CppSQLite3Query q;
SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
 if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
 {
	 CString sql;
	 sql.Format(_T("Select * from Value_Range where SN=%d"),m_sn);
	 q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

	 if (!q.eof())//有表但是没有对应序列号的值
	 {    
		 
		 while (!q.eof())
		 {
			 m_crange=q.getIntField("CRange");
			 inoutputno=q.getIntField("CInputNo");
			  
			 str1.Format(_T("%d:%d"),inoutputno,m_crange);
			 
			 
			 _Twrite_to_file_a_line(out,str1);
			 q.nextRow();
		 }
	 } 
	 

 
 }
SqliteDBBuilding.closedb();
 CString strTips;
 _Twrite_to_file_a_line(out,_T("//END"));//space
 strTips = _T("Config file saved 100%...");
 SetPaneString(1, strTips);
 }

 void save_write_TStatAllLabel(wofstream & out)
 {
      CString str1;
     _Twrite_to_file_a_line(out,_T("//Model Name Config"));//space
	 str1=_T("Off Model_Name: ");
	 str1+=GetTextFromReg(737);
	 _Twrite_to_file_a_line(out,str1);
	 str1=_T("Low Model_Name: ");
	 str1+=GetTextFromReg(741);
	 _Twrite_to_file_a_line(out,str1);
	 str1=_T("Mid Model_Name: ");
	 str1+=GetTextFromReg(745);
	 _Twrite_to_file_a_line(out,str1);
	 str1=_T("Hi Model_Name: ");
	 str1+=GetTextFromReg(749);
	 _Twrite_to_file_a_line(out,str1);
	 str1=_T("Auto Model_Name: ");
	 str1+=GetTextFromReg(753);
	 _Twrite_to_file_a_line(out,str1);
	 _Twrite_to_file_a_line(out,_T("//Model Name Config END"));
 }



 //    813 - 824
 //    825 - 836
 //    837 - 848
 //    849 - 859
 //    860 - 872
 //    873 - 884
 //    885 - 896
 //    897 - 908
 //    CHARLY 15:13 : 06
 //    909 - 971单写
 void save_write_TStatSchedual(wofstream & out)
 {
     CString cs_write_string;
     CString temo_cs;
     _Twrite_to_file_a_line(out, _T("//Schedual Config"));//space

     for (int i = 813; i <= 971; i++)
     {
         temo_cs.Format(_T("%d "), product_register_value[i]);
         cs_write_string = cs_write_string + temo_cs;
     }
     _Twrite_to_file_a_line(out, cs_write_string);
     cs_write_string.Empty();
     _Twrite_to_file_a_line(out, _T("//Schedual Config END"));
 }


 void write_TStatSchedual(wifstream & inf, float tstat_version, CStdioFile *p_log_file, load_file_every_step *p_log_file_one_time)
 {
     TCHAR buf[1024];
     int inputno = 0;
     int LabelNumber = 0;
     CString strText;
     int confirm_flag = 0; // 用于判断  Schedual Config 是否存在的 标志位;
     while (!inf.eof())
     {
         inf.getline(buf, 1024);

         if (confirm_flag == 0)
         {
             if (wcsstr(buf, _T("//Schedual Config")) != NULL)
             {
                 confirm_flag = 1;
                 continue;
             }
             return ;
         }
         if (confirm_flag == 2)
         {
             if (wcsstr(buf, _T("//Schedual Config END")) != NULL)
             {
                 break;
             }
             return ;
         }



         TCHAR *temp = buf;
         CString temp_cstring = temp;
         CStringArray temp_array;
         SplitCStringA(temp_array, temp_cstring, _T(" "));
         
         for (int i = 813; i <= 971; i++)
         {
             int temp_value = 0;
             temp_value = _wtoi(temp_array.GetAt(i - 813));
             product_register_value[i] = temp_value;
         }


         //    813 - 824
         //    825 - 836
         //    837 - 848
         //    849 - 859
         //    860 - 872
         //    873 - 884
         //    885 - 896
         //    897 - 908
         //    CHARLY 15:13 : 06
         //    909 - 971单写

         int n_ret[9] = {0};
         n_ret[0]=  Write_Multi_org_short(g_tstat_id, &product_register_value[813], 813, 824 - 813 + 1, 5);
         n_ret[1] = Write_Multi_org_short(g_tstat_id, &product_register_value[825], 825, 836 - 825 + 1, 5);
         n_ret[2] = Write_Multi_org_short(g_tstat_id, &product_register_value[837], 837, 848 - 837 + 1, 5);
         n_ret[3] = Write_Multi_org_short(g_tstat_id, &product_register_value[849], 849, 859 - 849 + 1, 5);
         n_ret[4] = Write_Multi_org_short(g_tstat_id, &product_register_value[860], 860, 872 - 860 + 1, 5);
         n_ret[5] = Write_Multi_org_short(g_tstat_id, &product_register_value[873], 873, 884 - 873 + 1, 5);
         n_ret[6] = Write_Multi_org_short(g_tstat_id, &product_register_value[885], 885, 896 - 885 + 1, 5);
         n_ret[7] = Write_Multi_org_short(g_tstat_id, &product_register_value[897], 897, 908 - 897 + 1, 5);

         for (int i = 909; i <= 971; i++)
         {
             write_one(g_tstat_id, i, product_register_value[i]);
         }

         confirm_flag = 2;
     }

 }

