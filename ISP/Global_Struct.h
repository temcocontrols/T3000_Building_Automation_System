/********************************************************************
	created:	2011/04/07
	created:	7:4:2011   10:23
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\Global_Struct.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	Global_Struct
	file ext:	h
	author:		ZGQ
	
	purpose:	 全局常量，以及结构体的声明
*********************************************************************/
#pragma once
#include "..\t3000\ProductModel.h"

const	 int c_nSplashTime = 2000;	// splash window的显示时间
const TCHAR c_strCfgFileName[] = _T("config.txt");				//	配置文件名称，用于保存用户设置
const TCHAR c_strLogoFileName[] = _T("ISPLogo.jpg");			// logo picture file name

 

//////////////////////////////////////////////////////////////////////////
// for setting file
const TCHAR c_strCfgTstatSection[] = _T("[Tstat]");
const TCHAR c_strCfgDefaultInstalldirItem[] = _T("Installdir : ");
const TCHAR c_strCfgLastFlashFileItem[] = _T("LastFlashFile : ");
const TCHAR c_strCfgLastFlashMethodItem[] = _T("LastFlashMethod : ");
const TCHAR c_strCfgDefaultAddrItem[] = _T("Default address : ");
const TCHAR c_strCfgDefaultComItem[] = _T("Default COM port : ");

const TCHAR c_strCfgNCSection[] = _T("[NC]");
const TCHAR c_strCfgDefaultIPItem[] = _T("Default IP : ");
const TCHAR c_strCfgDefaultIPPortItem[] = _T("Default IP Port : ");
const TCHAR c_strCfgDefaultBaudrateItem[] = _T("Default baudrate : ");//
const TCHAR c_strCfgLastFlashTypePageItem[] = _T("LastFlashPage : ");

const TCHAR c_strCfgLightCtrlSection[] = _T("[Lighting Controller]");
const TCHAR c_strCfgLastFlashLCTypeItem[] = _T("LastFlashType : ");
const TCHAR c_strCfgLastFlashLCTypeMain[] = _T("Main board");
const TCHAR c_strCfgLastFlashLCTypeInput[] = _T("Input board");
const TCHAR c_strCfgLastFlashLCTypeOutput[] = _T("Output board");

const TCHAR c_strCfgLastFlashMDBIDItem[] = _T("Default address : ");

const TCHAR c_strCfgNote[] = _T("Default Note : ");
const TCHAR c_strCfgSubID[] = _T("Default SubID : ");


//
const TCHAR c_strFlashMethodEthernet[] = _T("Ethernet");
const TCHAR c_strFlashMethodCom[] = _T("COM");



const DWORD c_nHexFileBufLen = 0x7FFFF;   // hex 最大64k
const DWORD c_nBinFileBufLen = 0x1FFFFF;  // bin 文件最大512k



#define WM_REPLACE_STATUSINFO		    WM_USER+1000 
#define WM_ADD_STATUSINFO				WM_USER+1001 
#define WM_FLASH_FINISH					WM_USER+1002
#define WM_UPDATA_DEVICE_INFORMATION    WM_USER+1003
#define WM_FLASH_RESTATR_BOOT           WM_USER+1004
#define WM_FLASH_NEW_BOOT_FINISH		WM_USER+1005
//#define WM_TCP_STATUSINFO				WM_USER+1003


#define MaxAddressNum   255
#define MaxRegisterNum  320
typedef unsigned char		TS_UC   ;
typedef char					TS_C    ;
typedef unsigned short		TS_US   ;
typedef short					TS_S    ;
typedef unsigned int			TS_UI   ;
typedef int						TS_I    ;
typedef long					TS_L    ;
typedef bool					TS_BOOL ;



//extern CString g_strExePath;
 enum{
	 NO_HEXFILEPREFIX=0,
	 OK_HEXFILEPREFIX,
	 ERR_HEXFILEPREFIX
 } ;
enum{
	// MODBUS REGISTER						// MIN	MAX	DESCRIPTION
	// Address 0  
	MODBUS_SERIALNUMBER_LOWORD   ,          // -	-	Lower 2 bytes of the serial number
	MODBUS_SERIALNUMBER_HIWORD  = 2  ,		// -	-	Upper 2 bytes of teh serial number
	MODBUS_VERSION_NUMBER_LO   = 4  ,		// -	-	Software version
	MODBUS_VERSION_NUMBER_HI,				// -	-	Software version
	MODBUS_ADDRESS,							// 1	254	Device modbus address
	MODBUS_PRODUCT_MODEL,					// -	-	Temco Product Model	1=Tstat5B, 2=Tstat5A, 4=Tstat5C, 12=Tstat5D, 
	MODBUS_HARDWARE_REV,					// -	-	Hardware Revision	 
	MODBUS_PIC_VERSION,						// -	-	PIC firmware version
	MODBUS_ADDRESS_PLUG_N_PLAY,				// -	-	Temporary address for plug-n-play addressing scheme

	MODBUS_BASE_ADDRESS 	= 15,			// base address select
		                                    // registers needed for updating status
	MODBUS_UPDATE_STATUS	= 16,			// reg 16 status for update_flash
											// writing 0x7F means jump to ISP routine
											// writing 0x8F means completely erase eeprom
	MODBUS_UPDATE_PTR_HI,					// reg 17 pointer for last attempt upload HI
	MODBUS_UPDATE_PTR_LO,					// reg 18 pointer for last attempt upload LO


	MODBUS_SCRATCHPAD_ADDRESS , 			// -	-	Internal scratch pad address	
	//---------------------------------------------------------
	//
	//        BLANK AREA RESERVED FOR FUTURE USE
	//
	//---------------------------------------------------------

	// Address 101  			
	MODBUS_TEMPRATURE_CHIP = 101  ,         // -	-	Calibrated temperature chip reading	(0.1 degrees)
	MODBUS_COOLING_VALVE,                   // -	-	Cooling valve position 0-1000 = 0-10V
	MODBUS_HEATING_VALVE,                   // -	-	Heating valve position 0-1000 = 0-10V
	MODBUS_COOLING_PID,						// -	-	Cooling PI Term	0-100 - PID IN REV25
	MODBUS_HEATING_PID,						// -	-	Heating PI Term	0-1000 - NOT USED IN REV25
	MODBUS_COOL_HEAT_MODE,					// -	-	Heating or cooling mode in effect	0 = coasting, 1 = cooling, 2 = heating
	MODBUS_MODE_OPERATION,					// -	-	Current state of Tstat.  High heat -> coasting -> high cool.
											//		0 = Coasting, 1 = Cooling1, 2 = Cooling2, 3 = Cooling3, 4 = Heating1, 5 = Heating2, 6 = Heating3 
	MODBUS_DIGITAL_OUTPUT_STATUS,			// -	-	Relays 1,2,3 and, if on-off type, 4 and 5
  	MODBUS_CALIBRATION ,  	   				// 0	255	Thermistor calibration term 
											//		0 = Temperature - 12.5 degrees, 125 = Temperature + 0 degrees, 255 = Temperature + 13 degrees

	// Address 110 
  	MODBUS_CALIBRATION_ANALOG_IN1, 			// 0	255	External thermistor calibration term 
											//		0 = Temperature - 12.5 degrees, 125 = Temperature + 0 degrees, 255 = Temperature + 13 degrees
  	MODBUS_TEMP_SELECT,  					// 0	1 = Temperature select, 0 = internal, 1 = external
  	MODBUS_DAC_OFFSET,						// 0	255	DAC voltage offset
  	MODBUS_DELAY,							// 0	120	Short cycle prevention delay	(0.1 seconds) - NOT USED IN REV25
  	MODBUS_COOLING_PTERM ,      			// 0	255	Cooling P term (0.1 degree)
  	MODBUS_COOLING_ITERM ,  				// 0	255	Cooling I term (0.1 %deg/min)
  	MODBUS_HEATING_PTERM ,      			// 0	255	Heating P term (0.1 degree) - NOT USED IN REV25
  	MODBUS_HEATING_ITERM ,  				// 0	50	Heating I term (1 %deg/min) - NOT USED IN REV25
  	MODBUS_SEQUENCE ,  						// 0	2	Sequence of operation , tstat behaves differently according to sequence
  	MODBUS_COOLING_DEADBAND ,    			// 1	100	Cooling deadband	(0.1 degree)
    // 
	// Address 120 
  	MODBUS_HEATING_DEADBAND ,   			// 1	100	Heating deadband	(0.1 degree)
  	MODBUS_DEGC_OR_F ,    					// 0	1	Temperature units  0 = DegC, 1 = DegF 
  	MODBUS_FAN_MODE ,  						// 0	3	Number of fan speeds to show on the display 0 = 0 speeds, 3 = 3 speeds
  	MODBUS_NIGHT_HEATING_DEADBAND ,			// 0	35	(0 and 95 for degrees F)	Night heating setback (1 degree)
  	MODBUS_NIGHT_COOLING_DEADBAND , 		// 0	99	(0 and 95 for degrees F)	Night cooling setback (1 degree)
  	MODBUS_APPLICATION ,       				// 0	1	0=Office, 1=Hotel or Residential
  	MODBUS_POWERUP_SETPOINT , 				// 15	50	(59 and 99 for degrees F)	Power up cooling setpoint (1 degree)
  	MODBUS_POWERUP_MODE , 					// 0	3	Powerup mode.  0=Off, 2=On, 2=Last Value, 3=Auto
  	MODBUS_KEYPAD_SELECT ,					// 0	3	Number of buttons on the keypad - 0=2, 1=4, 2=6A, 3=6B 
  	MODBUS_AUTO_ONLY ,						// 0	1	Enable or disable manual modes.  0=manual allowed, 1=auto only,2 = DDC mode
	
	// Address 130 
	MODBUS_OUTPUT_SCALE ,  					// 0	7	Sets the full scale voltage of the outputs - NOT USED IN REV25
				    						//		0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 4=OFF/ON, 5=10-0V, 6=5-0V, 7=10-2V
  	MODBUS_MAX_SETPOINT,            		// 10	99	(50 and 150 for degrees F)	Max cooling setpoint (1degree)
  	MODBUS_MIN_SETPOINT,					// 10	99	(50 and 150 for degrees F)	Min heating setpoint (1degree)
  	MODBUS_SPECIAL_MENU_LOCK,				// 0	1	Special menu lockout via keypad, serial port only 0=unlock, 1=lock	
  	MODBUS_FACTORY_DEFAULTS,     			// 0	1	Factory defaults  0=no default
  	MODBUS_COOLING_SETPOINT,				// 15	50	(59 and 99 for degrees F)	Cooling setpoint (1degree)
  	MODBUS_HEATING_SETPOINT,  				// 10	35	(50 and 95 for degrees F)	Heating setpoint (1degree)
  	MODBUS_FAN_SPEED ,  					// 0	4	Fan speed 0=OFF, 1=Low, 2=MED, 3=HI, 4=AUTO

	
	// Binary output state for each of the 8 modes of operation  
	// Address 138 
	MODBUS_FAN0_OPER_TABLE_COAST,      	// 1 byte, first 5 lsb shows the relay states, last 3 msb are spares
	MODBUS_FAN0_OPER_TABLE_COOL1,		// 1st byte is always coasting
	MODBUS_FAN0_OPER_TABLE_COOL2,      	// next is cooling stages 
	MODBUS_FAN0_OPER_TABLE_COOL3,      	// then comes heating stages
	MODBUS_FAN0_OPER_TABLE_HEAT1,      	// there are up to 7 stages, unused stages are pushed to the end of the strucuture, spare if not used
	MODBUS_FAN0_OPER_TABLE_HEAT2,      	// any of the stages can be heat or cool depending on the number of heat and cool stages  
	MODBUS_FAN0_OPER_TABLE_HEAT3,      	// defined in registers 276 and 277

	MODBUS_FAN1_OPER_TABLE_COAST,      	// Address 145
	MODBUS_FAN1_OPER_TABLE_COOL1,		// 
	MODBUS_FAN1_OPER_TABLE_COOL2,      	// 
	MODBUS_FAN1_OPER_TABLE_COOL3,      	// 
	MODBUS_FAN1_OPER_TABLE_HEAT1,      	// 
	MODBUS_FAN1_OPER_TABLE_HEAT2,      	// 
	MODBUS_FAN1_OPER_TABLE_HEAT3,      	// 
		
	MODBUS_FAN2_OPER_TABLE_COAST,      	// Address 152
	MODBUS_FAN2_OPER_TABLE_COOL1,		// 
	MODBUS_FAN2_OPER_TABLE_COOL2,      	// 
	MODBUS_FAN2_OPER_TABLE_COOL3,      	// 
	MODBUS_FAN2_OPER_TABLE_HEAT1,      	// 
	MODBUS_FAN2_OPER_TABLE_HEAT2,      	// 
	MODBUS_FAN2_OPER_TABLE_HEAT3,      	// 
			
	MODBUS_FAN3_OPER_TABLE_COAST,      	// Address 159
	MODBUS_FAN3_OPER_TABLE_COOL1,		// 
	MODBUS_FAN3_OPER_TABLE_COOL2,      	// 
	MODBUS_FAN3_OPER_TABLE_COOL3,      	// 
	MODBUS_FAN3_OPER_TABLE_HEAT1,      	// 
	MODBUS_FAN3_OPER_TABLE_HEAT2,      	// 
	MODBUS_FAN3_OPER_TABLE_HEAT3,      	// 
		
	MODBUS_FANAUT_OPER_TABLE_COAST,     // Address 166
	MODBUS_FANAUT_OPER_TABLE_COOL1,		// 
	MODBUS_FANAUT_OPER_TABLE_COOL2,     // 
	MODBUS_FANAUT_OPER_TABLE_COOL3,     // 
	MODBUS_FANAUT_OPER_TABLE_HEAT1,     // 
	MODBUS_FANAUT_OPER_TABLE_HEAT2,     // 
	MODBUS_FANAUT_OPER_TABLE_HEAT3,     // 
	
	// Analog output state for each of the 8 modes of operation	
	MODBUS_VALVE_OPERATION_TABLE_BEGIN,	// Address 173 
	MODBUS_VALVE_OPER_TABLE_COOL1,
	MODBUS_VALVE_OPER_TABLE_COOL2,
	MODBUS_VALVE_OPER_TABLE_COOL3,
	MODBUS_VALVE_OPER_TABLE_HEAT1,
	MODBUS_VALVE_OPER_TABLE_HEAT2,
	MODBUS_VALVE_OPER_TABLE_HEAT3,

	// Address 180
	MODBUS_EXTERNAL_SENSOR_0,				// -	-	External Sensor 0 reading
	MODBUS_EXTERNAL_SENSOR_1,				// -	-	External Sensor 1 reading
	MODBUS_NIGHT_HEATING_SETPOINT,			// 10	99	Night Heating Setpoint	(1 degree)
	MODBUS_NIGHT_COOLING_SETPOINT,			// 10	99	Night Cooling Setpoint	(1 degree)

	MODBUS_INFO_BYTE,						// -	-	Byte that holds info about the tstat
											// 		bit0 = occupancy bit - 1=occupied, 0=unoccupied
											//		bit1 = watchdog reset - 1 indicates a reset due to watchdog
											//		bit2 = serial write reset prevention

	MODBUS_BAUDRATE,						// 0	1	Baudrate 0 = 9.6kb/s, 1 = 19.2kb/s
	MODBUS_OUTPUT1_SCALE,					// 0	3	Sets the full scale voltage of analog output 1
											//		0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 
	MODBUS_OUTPUT2_SCALE,					// 0	3	Sets the full scale voltage of analog output 2
    										//		0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 
  	MODBUS_ANALOG_IN1 ,  					// 0	1	Analog input 1 range. 
	MODBUS_ANALOG_IN2,						// 0	0	Analog input 2 range.  RESERVED FOR FUTURE USE
	// Address 190
	MODBUS_DIGITAL_IN1,						// 0	2	Digital occupied mode trigger 
											//		0 = OFF, 1 = trigger unoccupied on falling edge, 2 = occupied high, unoccupied low

	// Address 191
	MODBUS_OUTPUT1_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT2_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT3_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT4_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT5_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
		
	// Address 196
	MODBUS_OUTPUT1_DELAY_ON_TO_OFF,  		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT2_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT3_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT4_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT5_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	
	// Address 201
	MODBUS_CYCLING_DELAY ,					// 0	20	Delay between going out of heating or cooling and then back in. (1 min)      			
	MODBUS_CHANGOVER_DELAY,					// 0	200	Delay between going from cooling to heating or vice versa (1 min)
	MODBUS_DISPLAY,							// 0	1	Display setting.  0 = temperature, 1 = setpoint

	// Address 204
	MODBUS_LED1_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED2_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED3_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED4_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED5_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED6_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED7_CONTROL,					// 0	22	Determines what controls the state of the LED
             			//CONTROL FUNCTION	//VALUE
						//NONE				//00
						//RELAY1			//01
						//RELAY2			//02
						//RELAY3			//03
						//RELAY4			//04
						//RELAY5			//05
						//COASTING STAGE	//06
						//COOLING1 STAGE	//07
						//COOLING2 STAGE	//08
						//COOLING3 STAGE	//09
						//HEATING1 STAGE	//10
						//HEATING2 STAGE	//11
						//HEATING3 STAGE	//12
						//COOLING MODE		//13
						//HEATING MODE		//14
						//FAN OFF		//15
						//FAN LOW		//16
						//FAN MED		//17
						//FAN HI		//18
						//FAN AUTO		//19
						//HEAT1_OR_COOL1	//20
						//HEAT2_OR_COOL2	//21
						//HEAT3_OR_COOL3	//22
						//COOL1_2_OR_3		//23
						//HEAT1_2_OR_3		//24
	// Address 211
	MODBUS_OVERRIDE_TIMER,			// 0	255	Determines what controls the state of the LED
    MODBUS_OVERRIDE_TIMER_LEFT,
	MODBUS_FILTER,
	MODBUS_HEAT_COOL_CONFIG,
	MODBUS_INTERNAL_TEMP_IC,
	MODBUS_INTERNAL_THERMISTOR,
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR,
	MODBUS_CALIBRATION_ANALOG_IN2,
    // Address 219
    MODBUS_TABLE1_ZERO,                     // 1     254    the range from reg 20 to reg 41.A changable look up table for "custom" sensor 
	MODBUS_TABLE1_HALFONE,                  //              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
	MODBUS_TABLE1_ONE,                      //              the value's unit in the table is different according to different sensor
	MODBUS_TABLE1_HALFTWO,                 
	MODBUS_TABLE1_TWO,
	MODBUS_TABLE1_HALFTHREE,
	MODBUS_TABLE1_THREE,
	MODBUS_TABLE1_HALFFOUR,
	MODBUS_TABLE1_FOUR,
	MODBUS_TABLE1_HALFFIVE,
	MODBUS_TABLE1_FIVE,
	// Address 230
	MODBUS_TABLE2_ZERO,
	MODBUS_TABLE2_HALFONE,
	MODBUS_TABLE2_ONE,
	MODBUS_TABLE2_HALFTWO,
	MODBUS_TABLE2_TWO,
	MODBUS_TABLE2_HALFTHREE,
	MODBUS_TABLE2_THREE,
	MODBUS_TABLE2_HALFFOUR,
	MODBUS_TABLE2_FOUR,
	MODBUS_TABLE2_HALFFIVE,
	MODBUS_TABLE2_FIVE ,
	// Address 241
	MODBUS_INPUT1_SELECT  ,                        //1    254 reg 241 select which way the analog input is
	MODBUS_UNIVERSAL_DB_HI ,
	MODBUS_UNIVERSAL_DB_LO ,
    MODBUS_UNIVERSAL_PTERM ,                       //1    254 reg 244 is the address of P term a universal PID
	MODBUS_UNIVERSAL_ITERM ,                       //1    254 reg 245 is the address of I term a universal PID
	MODBUS_UNIVERSAL_SET ,                         //1    254 reg 246 is the setpoint for universal PID 
		
	MODBUS_PID_OUTPUT1 ,                           //1    254 reg 247 select which type PID to be caculate for control output
	MODBUS_PID_OUTPUT2 ,   
	MODBUS_PID_OUTPUT3 ,   
	MODBUS_PID_OUTPUT4 ,   
	MODBUS_PID_OUTPUT5 ,   
	MODBUS_PID_OUTPUT6 ,   
	MODBUS_PID_OUTPUT7 ,
		
	MODBUS_UNIVERSAL_OUTPUT_BEGIN , //    254 control OUTPUT,there are 7 status
	MODBUS_UNIVERSAL_OUTPUT_COOL1 ,
	MODBUS_UNIVERSAL_OUTPUT_COOL2 ,
	MODBUS_UNIVERSAL_OUTPUT_COOL3 ,
	MODBUS_UNIVERSAL_OUTPUT_HEAT1 , //    258  
	MODBUS_UNIVERSAL_OUTPUT_HEAT2 ,
	MODBUS_UNIVERSAL_OUTPUT_HEAT3 ,
	
	MODBUS_UNIVERSAL_VALVE_BEGIN , //     control the status of valve
	MODBUS_UNIVERSAL_VALVE_COOL1 ,
	MODBUS_UNIVERSAL_VALVE_COOL2 ,
	MODBUS_UNIVERSAL_VALVE_COOL3 ,
	MODBUS_UNIVERSAL_VALVE_HEAT1 ,
	MODBUS_UNIVERSAL_VALVE_HEAT2 ,
	MODBUS_UNIVERSAL_VALVE_HEAT3 ,
	MODBUS_HEAT_UNIVERSAL_TABLE	 ,
	MODBUS_COOL_UNIVERSAL_TABLE  ,              //     269  
	//MODBUS_CALIBRATION_ANALOGIN1 ,            //     266  
	//MODBUS_CALIBRATION_ANALOGIN2 ,            //     267  
	MODBUS_PID_UNIVERSAL ,                      //     270  
	MODBUS_UNITS1_HIGH ,                        //     271
	MODBUS_UNITS1_LOW  ,                        //     272
	MODBUS_UNITS2_HIGH ,                        //     273
	MODBUS_UNITS2_LOW ,                         //     274
	MODBUS_UNIVERSAL_NIGHTSET ,                 //     275
	MODBUS_HEAT_ORIGINAL_TABLE ,                //     276       
	MODBUS_COOL_ORIGINAL_TABLE ,                //     277
	MODBUS_PID2_MODE_OPERATION,
	MODBUS_VALVE_TRAVEL_TIME ,
	MODBUS_MODE_OUTPUT1,
	MODBUS_MODE_OUTPUT2,
	MODBUS_MODE_OUTPUT3,
	MODBUS_MODE_OUTPUT4,						//     	283
	MODBUS_MODE_OUTPUT5,						//	   	284	
	MODBUS_VALVE_PERCENT,						//	   	285
	MODBUS_INTERLOCK_OUTPUT1,					//		286
	MODBUS_INTERLOCK_OUTPUT2,					//		287
	MODBUS_INTERLOCK_OUTPUT3,
	MODBUS_INTERLOCK_OUTPUT4,					//		289
	MODBUS_INTERLOCK_OUTPUT5,
	MODBUS_INTERLOCK_OUTPUT6,					//		291
	MODBUS_INTERLOCK_OUTPUT7,					//		292
	MODBUS_SETPOINT_INCREASE,					//		293
	MODBUS_LAST_KEY_TIMER,						//		294
	MODBUS_FREEZE_TEMP_SETPOINT,				//		295
	MODBUS_FREEZE_DELAY_ON,						//		296
	MODBUS_FREEZE_DELAY_OFF,					//		297
	MODBUS_ANALOG1_FUNCTION,					//		298
	MODBUS_ANALOG2_FUNCTION,					//		299
	MODBUS_DIGITAL1_FUNCTION,					//		300
	MODBUS_TIMER_ON,							//		301
	MODBUS_TIMER_OFF,							//		302
	MODBUS_TIMER_UNITS,							//		303
	MODBUS_KEYPAD_VALUE,						//		304
	MODBUS_DISPLAY_HUNDRED,						//		305
	MODBUS_DISPLAY_TEN,							//		306
	MODBUS_DISPLAY_DIGITAL,						//		307
	MODBUS_DISPLAY_STATUS,						//		308
	MODBUS_INPUT_MANU_ENABLE,					//		309
	MODBUS_OUTPUT_MANU_ENABLE,					//		310
	MODBUS_DIGITAL_INPUT,						//		311
	MODBUS_MANU_RELAY1,							//		312
 	MODBUS_MANU_RELAY2,							//		313
	MODBUS_MANU_RELAY3,							//		314
	MODBUS_MANU_RELAY4,							//		315
	MODBUS_MANU_RELAY5,							//		316
	MODBUS_DEAD_MASTER,							//		317
	MODBUS_ROUND_DISPLAY,						//		318
	MODBUS_MIN_ADDRESS ,						//		319	// The ID of tstat should be in the range MIN and MAX address
	MODBUS_MAX_ADDRESS,							//		320 
	MODBUS_FAN_GRIDPOINT,						//		321
	MODBUS_MODE_GRIDPOINT,						//		322
	MODBUS_HOLD_GRIDPOINT,						//		323
    MODBUS_CONFIGURATION,						//		324
	MODBUS_UTIL_MODE,							//		325
	MODBUS_EEPROM_SIZE,							//		326
	MODBUS_TIMER_SELECT,						//		327	  //
	MODBUS_OUTPUT1_FUNCTION,					//		328		 
	MODBUS_ROTATION_OUT2,						//		329 	 
	MODBUS_ROTATION_OUT3,						//		330		 
	MODBUS_ROTATION_OUT4,						//		331		 
	MODBUS_ROTATION_OUT5,						//		332		 
	MODBUS_ROTATION_TIME_LEFT,					//		333	
	MODBUS_OUTPUT2_FUNCTION,			    	//		334
	MODBUS_OUTPUT3_FUNCTION,					//		335		
	MODBUS_OUTPUT4_FUNCTION,					//		336	
	MODBUS_OUTPUT5_FUNCTION,					//		337	
	MODBUS_DEFAULT_SETPOINT,					//		338
	MODBUS_SETPOINT_CONTROL,					//		339	 
	MODBUS_PIR_STAGE,							//		340	  
 

	MODBUS_OUTPUT_PWM_AUTO_COAST,			    //		341
	MODBUS_OUTPUT_PWM_AUTO_COOL1,			    //		342
	MODBUS_OUTPUT_PWM_AUTO_COOL2,			    //		343
	MODBUS_OUTPUT_PWM_AUTO_COOL3,			    //		344
	MODBUS_OUTPUT_PWM_AUTO_HEAT1,			    //		345
	MODBUS_OUTPUT_PWM_AUTO_HEAT2,			    //		346
	MODBUS_OUTPUT_PWM_AUTO_HEAT3,			    //		347
	MODBUS_PWM_OUT4,
	MODBUS_PWM_OUT5,

	MODBUS_FREE_COOL_CONFIG,					//		350 Free cool configure register.bit0,free cool enable/disable,0 = disable,1= enable.
//bit1,free cool available decided by local or external master.0 =  local,1 = NC.bit2,free cool available status,0 = npt available,1= available.
//bit4,show the status if NC is OK when the free cool decided by NC.
	MODBUS_VALVE_OFF_TABLE_COAST,				//		351 OFF table for analog output,
	MODBUS_VALVE_OFF_TABLE_COOL1,
	MODBUS_VALVE_OFF_TABLE_COOL2,
	MODBUS_VALVE_OFF_TABLE_COOL3,
	MODBUS_VALVE_OFF_TABLE_HEAT1,
	MODBUS_VALVE_OFF_TABLE_HEAT2,
	MODBUS_VALVE_OFF_TABLE_HEAT3,				//		357  
	MODBUS_LOCK_REGISTER,						//		358	All registers except fan speed and manual input are not writable.0 = lock,1 = no lock.

	MODBUS_FREE_COOL_OUT_TEMP,					//		359	Outside temperature for free cooling,from external sensor or NC.
	MODBUS_NC_MASTER_TIME,						//		360 If outside temp be set from NC.The communication with NC must be set in this time,otherwise will set error status and use external sensor.
	MODBUS_FREE_COOL_TRIGGER,					//		361 If the outside air temp is lower than the room temeprature by this amount, then the free cooling is worthwhile, 350 bit2 = 1
          										//		If the OAT is greater than the room temp, then free cooling mode is not worhtwhile. , 350 bit2 = 0
	MODBUS_OUTPUT_FREE_COOL_COAST,				//		362 Output table in free cooling mode,0 = 0%, 1 = 1-00%,2 = MIN->100%,3 = MIN|100%,4 = MIN
	MODBUS_OUTPUT_FREE_COOL_COOL1,
	MODBUS_OUTPUT_FREE_COOL_COOL2,
	MODBUS_OUTPUT_FREE_COOL_COOL3,
	MODBUS_OUTPUT_FREE_COOL_HEAT1,
	MODBUS_OUTPUT_FREE_COOL_HEAT2,
	MODBUS_OUTPUT_FREE_COOL_HEAT3, 				//		368
	MODBUS_MIN_AIR,								//		369 The minimum ourput for free cooling  
	MODBUS_OAT_HOTTEST,							//		370 Outside air temperature in hottest day
	MODBUS_OAT_COLDEST,							//		371 Outside air temperature in coldest day
	MODBUS_OFFSET_HOTTEST,						//		372 Offset in hottest day
	MODBUS_OFFSET_COLDEST,						//		373 Offset in codest day
	MODBUS_TWO_BYTE_SETPOINT,					//		374 Store setpoint in two bytes ,the resolution is 0.1
	MODBUS_CURRENT_SETPOINT,					//		375 Crrent setpoint  = user setpoint  + offset setpoint
	MODBUS_SETPOINT_OFFSET,						//		376 Setpoint offset
	MODBUS_CHANGEOVER_SENSOR_MODE,				//		377	Change over sensor mode,1 = cooling mode,0 = heating mode.

	 	 
//	MODBUS_ADDRESS_END

    /*MODBUS_H1 ,                   			//		
	MODBUS_H2 ,
	MODBUS_H3 ,
	MODBUS_H4 ,
	MODBUS_H5 ,
	MODBUS_H6 ,*/
	MODBUS_TEST1 ,								//		378
	MODBUS_TEST2 ,
	MODBUS_TEST3 
};




///////////////////////////////////////////////////////////////////////////
// for product model file
//////////////////////////////////////////////////////////////////////////
// product model ID table
//int const PM_TSTAT5A = 2;//
//int const PM_TSTAT5B = 1;  //
//int const PM_TSTAT5B2 = 3;//
//int const PM_TSTAT5C = 4; //
//int const PM_TSTAT6 = 6;  //
//int const PM_TSTAT7 = 7;	//
//int const PM_TSTAT5I = 8;  //
//int const PM_TSTAT8 = 9 ;
//
//int const PM_TSTAT5D = 12;//
//const int PM_TSTAT5E = 16;	//
//int const PM_TSTAT5F = 17;	//
//int const PM_TSTAT5G = 18;	 //
//int const PM_TSTAT5H = 19;	 //
//int const PM_T38AIOD= 20;  //
//int const PM_T3IOA = 21;  //
//int const PM_T332AI = 22; //
//int const PM_T38AI16O = 23;
//int const PM_ZIGBEE = 24;
//int const PM_FLEXDRIVER = 25;
//int const PM_T3PT10 = 26;
//int const PM_T3PERFORMANCE = 27;
//int const PM_T34AO = 28;
//int const PM_T36CT = 29;
//int const PM_SOLAR = 30;
//int const PM_FWMTRANSDUCER = 31;
//int const PM_MINIPANEL = 35;
//int const PM_PRESSURE = 40;
//int const PM_CM5 = 50;	  //
//int const PM_NC = 100;	//
//int const PM_AirQuality = 13;
//int const PM_HUMTEMPSENSOR = 14;
//int const PM_LightingController = 103;	//
//int const PM_HUM_R=42;
//int const PM_RUNNAR=15;
//int const PM_CO2_NET = 32;
//int const PM_CO2_RS485 = 33;
//int const PM_CO2_NODE = 34;
//int const PM_PM5E = 41;
//int const PM_T322AI=43;
//int const PM_T38AI8AO6DO=44;
//int const PM_T3PT12 = 46;
//int const PM_BTU_METER=121;
//
//int const PM_PRESSURE_SENSOR=45;
const TCHAR c_strProductModelFileName[] = _T("ProductModel.cfg");				//	Product Model 配置文件名称


const int c_nModelNum = 27;
const CString g_ProductModel[c_nModelNum] = 
{ 
_T("TStat5B=1"),
_T("TStat5A=2"),
_T("TStat5B2=3"),
_T("TStat5C=4"),
_T("TStat5D=12"),
_T("TStat5E=16"),
_T("TStat5F=17"),
_T("TStat5G=18"),
_T("TStat5H=19"),
_T("TStat6=6"),
_T("TStat7=7"),
_T("T3-8AIOD=20"),
_T("T3-8IOA=21"),
_T("T3-32AI=22"),
_T("T3-8AI160=23"),
_T("ZigBee=24"),
_T("FlexDriver=25"),
_T("T3-PT10=26"),
_T("T3-Performance=27"),
_T("T3-4AO=28"),
_T("T3-6CT=29"),
_T("Solar=30"),
_T("MiniPanel=150"),
_T("CM5=50"),
_T("NC=100"),
_T("OR485=102"),
_T("LC=120")
};


const int NO_COMMAND = 0;
const int START_AUTO_FLASH_COMMAND = 1;
const int FLASH_SUCCESS = 2;
const int FAILED_NORESPONSED = 3;
const int FAILED_UNKNOW_ERROR = 4;
//const int

///////////////////////////////////////////////////////////////////////////
// for flash sn settings file

const TCHAR c_strFlashSNCfgFileName[] = _T("Flash_SN.cfg");				//	保存FLASH SN 的用户设定

const TCHAR c_strFlashHWVersion[] = _T("Hardware Verson : ");
const TCHAR c_strFlashSNModel[] = _T("Product Model : ");


 
typedef struct
{
	char company[5];
	char product_name[10];
	unsigned char software_low;
	unsigned char software_high;
	char reserved[3];
}Bin_Info;

enum
{
	READ_SUCCESS = 0,
	FILE_NOT_FIND = 1,
	OPEN_FILE_ERROR = 2,
	BIN_FILE_LENGTH_ERROR = 3,
	BAD_HEX_FILE = 4,
	NO_VERSION_INFO = 5
};
 