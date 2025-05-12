#pragma once
#include "global_variable_extern.h"

// #ifdef _DEBUG
// #ifndef _WIN64

#define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")

// #else
// #pragma comment(lib,"json/json_mtd_x64.lib")
// #define FOR_DATABASE_CONNECT					_T("Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider;Data Source=")
// #endif
// #else
// #ifndef _WIN64 
// #define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")
// #else
// #define FOR_DATABASE_CONNECT					_T("Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider;Data Source=")
// #endif
// #endif

#define USER_DATABASE_PASSWORD					_T(";Jet OLEDB:Database Password=alancat")
//Persist Security Info=False;

#define NO_APPLICATION _T("N/A")
#define NO_ADDRESS			-1
#define NO_REGISTER_VALUE	-1

#define REG_READ_ONLY TRUE
#define REG_READ_WRITE FALSE

#define Global_String_AUTO    _T("Auto")
#define Global_String_MANUAL  _T("Manual")
#define Global_String_NO     _T("On")
#define Global_String_OFF   _T("Off")

#define Global_String_C _T("¡ãC")
#define Glocal_String_F _T("¡ãF")

#define Modbus_Serial	0
#define Modbus_TCP		1
#define	default_com1_port_baudrate	19200

#define LIST_TYPE_INPUT_TSTAT 1
#define LIST_TYPE_OUTPUT_TSTAT 2
#define COLOR_CELL RGB(226,238,251)
#define DISABLE_COLOR_CELL RGB(140,140,140)
#define SETPOINT_SPECIAL_VERSION	50
#define MsgT3000ViewFresh 5000
#define  MESSAGE_SLIDER 5001
//the product model of product declare
//#define NET_WORK_CONT_PRODUCT_MODEL					100
#define	LED_PRODUCT_MODEL							25
#define TSTAT_PRODUCT_MODEL							19
//#define SOLAR_PRODUCT_MODEL							30		//SOLOR
#define PWM_TRANSDUCER_PRODUCT_MODEL				31
//#define T3_8IOD_PRODUCT_MODEL						20		//T3-8IOD
//#define T3_8IOA_PRODUCT_MODEL						21		//T3-8O
#define	T3_32I_PRODUCT_MODEL						22		//T3-32I
#define T3_8I_16O_PRODUCT_MODEL						23		//T3-8I-16O
//#define ZIGBEE_PRODUCT_MODEL						24		
#define NET_WORK_OR485_PRODUCT_MODEL				102
// #define CMFIVE_PRODUCT_MODEL                        50      //CM5
// #define MINIPANEL_MODEL								35		//Mini Panel
#define T3_4AO_PRODUCT_MODEL						28		//T3-4AO


#define WM_USER_FRESHSTATUSBAR						10000
#define WM_MYMSG_REFRESHBUILDING					80001

#define WM_MAIN_MSG_SCAN_BAC                        10010
#define WM_MAIN_MSG_UPDATE_PRODUCT_TREE             10011
//scan


//Database operation

#define DB_SELECT_FOUND							VARIANT_FALSE==m_pRs->EndOfFile
#define DB_SELECT_NO_FOUND						VARIANT_FALSE!=m_pRs->EndOfFile
#define CLOSE_DATABASE								if(m_pRs->State) \
														m_pRs->Close(); \
													if(m_pCon->State) \
														m_pCon->Close(); 



#define LOAD_FAILURE_ONE							_T("Section1:FAN VALVE ")
#define LOAD_FAILURE_TWO							_T("Section2:VALVE SETTING ")
#define LOAD_FAILURE_THREE							_T("Section3:DELAY TIMES ")
#define LOAD_FAILURE_FOUR							_T("Section4:LOOKUP TABLE ")
#define LOAD_FAILURE_FIVE							_T("Section5:UNIVERSAL RELAY OUTPUTS ")
#define LOAD_FAILURE_SIX							_T("Section6:UNIVERSAL VALVE ")
#define LOAD_FAILURE_SEVEN							_T("Section7:VARIABLE ")
#define STEP_FAILURE								_T(" Error\r\n")
#define STEP_FINISH									_T(" Success\r\n")
 





// CBuildTable1 ¶Ô»°¿ò
    // Address 219
	/*
#define MODBUS_TABLE1_ZERO  219
#define	MODBUS_TABLE1_ZERO_HI  220              // 1     254    the range from reg 20 to reg 41.A changable look up table for "custom" sensor 
#define	MODBUS_TABLE1_HALFONE  221   
#define	MODBUS_TABLE1_HALFONE_HI 222               //              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
#define	MODBUS_TABLE1_ONE       223
#define	MODBUS_TABLE1_ONE_HI    224             //              the value's unit in the table is different according to different sensor
#define	MODBUS_TABLE1_HALFTWO   225
#define	MODBUS_TABLE1_HALFTWO_HI 226             
#define	MODBUS_TABLE1_TWO       227
#define	MODBUS_TABLE1_TWO_HI    228
#define	MODBUS_TABLE1_HALFTHREE 229
#define	MODBUS_TABLE1_HALFTHREE_HI 230
#define	MODBUS_TABLE1_THREE     231
#define	MODBUS_TABLE1_THREE_HI 232
#define	MODBUS_TABLE1_HALFFOUR 233
#define	MODBUS_TABLE1_HALFFOUR_HI 234
#define	MODBUS_TABLE1_FOUR     235
#define	MODBUS_TABLE1_FOUR_HI  236
#define	MODBUS_TABLE1_HALFFIVE 237
#define	MODBUS_TABLE1_HALFFIVE_HI 238
#define	MODBUS_TABLE1_FIVE 239
#define	MODBUS_TABLE1_FIVE_HI 240

#define MODBUS_TABLE2_ZERO        314  
#define	MODBUS_TABLE2_ZERO_HI     315                   // 1     254    the range from reg 20 to reg 41.A changable look up table for "custom" sensor 
#define	MODBUS_TABLE2_HALFONE     316
#define	MODBUS_TABLE2_HALFONE_HI  317             //              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
#define	MODBUS_TABLE2_ONE         318
#define	MODBUS_TABLE2_ONE_HI      319                   //              the value's unit in the table is different according to different sensor
#define	MODBUS_TABLE2_HALFTWO     320
#define	MODBUS_TABLE2_HALFTWO_HI  321          
#define	MODBUS_TABLE2_TWO         322 
#define	MODBUS_TABLE2_TWO_HI      323
#define	MODBUS_TABLE2_HALFTHREE   324
#define	MODBUS_TABLE2_HALFTHREE_HI 325
#define	MODBUS_TABLE2_THREE        326
#define	MODBUS_TABLE2_THREE_HI     327
#define	MODBUS_TABLE2_HALFFOUR     328
#define	MODBUS_TABLE2_HALFFOUR_HI  329
#define	MODBUS_TABLE2_FOUR         330
#define	MODBUS_TABLE2_FOUR_HI      331
#define	MODBUS_TABLE2_HALFFIVE     332
#define	MODBUS_TABLE2_HALFFIVE_HI  333
#define	MODBUS_TABLE2_FIVE         334
#define	MODBUS_TABLE2_FIVE_HI      335*/
//
//
//
#define MODBUS_TSTAT6_HUM_AM  608  //605
#define MODBUS_TSTAT6_HUM_MVALUE 609   //606
#define MODBUS_TSTAT6_HUM_AVALUE 140
#define MODBUS_TSTAT6_CO2_AM  610		 //607
#define MODBUS_TSTAT6_CO2_MVALUE 611    //608
#define MODBUS_TSTAT6_CO2_AVALUE 139
#define MODBUS_TSTAT6_HUM_FILTER 152
#define MODBUS_TSTAT6_CO2_FILTER 151

#define MODBUS_TSTAT_LUX_FILTER 627

#define MaxAddressNum   255
#define MaxRegisterNum  320
#define BAUDRATENUMBER  5
typedef unsigned char	TS_UC;
typedef char			TS_C;
typedef unsigned short	TS_US;
typedef short			TS_S;
typedef unsigned int	TS_UI;
typedef int				TS_I;
typedef long			TS_L;
typedef bool			TS_BOOL;





