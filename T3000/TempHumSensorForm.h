#pragma once
#include "msflexgrid1.h"

#include "afxwin.h"

// CTempHumSensorForm dialog
union RegisterValue{
	BOOL BOOLRegValue[16];
	unsigned short RegValue;
};

#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
class CTempHumSensorForm : public CFormView
{
	//DECLARE_DYNAMIC(CTempHumSensorForm)
	DECLARE_DYNCREATE(CTempHumSensorForm)
public:
	CTempHumSensorForm();   // standard constructor
	virtual ~CTempHumSensorForm();

// Dialog Data
	enum { IDD = IDD_DIALOG_HUM_TEMP_SENSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
 
public:
void Fresh();
void Changing_Data();
void NoChange_Data();
afx_msg LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
CString m_oldvalue;
CString m_newvalue;

enum {
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

	// Address 100	
	MODBUS_TEMPRATURE_CHIP_F = 100  , 		
	MODBUS_TEMPRATURE_CHIP = 101  ,         // -	-	Calibrated temperature chip reading	(0.1 degrees)
	MODBUS_COOLING_VALVE,                   // -	-	Cooling valve position 0-1000 = 0-10V
	MODBUS_HEATING_VALVE,                   // -	-	Heating valve position 0-1000 = 0-10V
	MODBUS_COOLING_PID,						// -	-	Cooling PI Term	0-100 - PID IN REV25
	MODBUS_HEATING_PID,						// -	-	Heating PI Term	0-1000 - NOT USED IN REV25
	MODBUS_COOL_HEAT_MODE,					// -	-	Heating or cooling mode in effect	0 = coasting, 1 = cooling, 2 = heating
	MODBUS_MODE_OPERATION,					// -	-	Current state of Tstat.  High heat -> coasting -> high cool. //		0 = Coasting, 1 = Cooling1, 2 = Cooling2, 3 = Cooling3, 4 = Heating1, 5 = Heating2, 6 = Heating3 											
	MODBUS_DIGITAL_OUTPUT_STATUS,	//108		// -	-	Relays 1,2,3 and, if on-off type, 4 and 5
	MODBUS_CALIBRATION ,  	   				// 0	255	Thermistor calibration term 		//		0 = Temperature - 12.5 degrees, 125 = Temperature + 0 degrees, 255 = Temperature + 13 degrees
	MODBUS_CALIBRATION_ANALOG_IN1, 			// 0	255	External thermistor calibration term 	  //		0 = Temperature - 12.5 degrees, 125 = Temperature + 0 degrees, 255 = Temperature + 13 degrees											
	MODBUS_TEMP_SELECT,  					// 0	1 = Temperature select, 0 = internal, 1 = external
	MODBUS_DAC_OFFSET,						// 0	255	DAC voltage offset
	MODBUS_DELAY,							// 0	120	Short cycle prevention delay	(0.1 seconds) - NOT USED IN REV25
	MODBUS_COOLING_PTERM ,      			// 0	255	Cooling P term (0.1 degree)
	MODBUS_COOLING_ITERM ,  				// 0	255	Cooling I term (0.1 %deg/min)
	MODBUS_HEATING_PTERM ,      			// 0	255	Heating P term (0.1 degree) - NOT USED IN REV25
	MODBUS_HEATING_ITERM ,  				// 0	50	Heating I term (1 %deg/min) - NOT USED IN REV25
	MODBUS_SEQUENCE ,  						// 0	2	Sequence of operation , tstat behaves differently according to sequence
	MODBUS_COOLING_DEADBAND ,    			// 1	100	Cooling deadband	(0.1 degree)
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
	MODBUS_OUTPUT_SCALE ,  					// 0	7	Sets the full scale voltage of the outputs - NOT USED IN REV25
	MODBUS_MAX_SETPOINT,            		// 10	99	(50 and 150 for degrees F)	Max cooling setpoint (1degree)
	MODBUS_MIN_SETPOINT,					// 10	99	(50 and 150 for degrees F)	Min heating setpoint (1degree)
	MODBUS_SPECIAL_MENU_LOCK,				// 0	1	Special menu lockout via keypad, serial port only 0=unlock, 1=lock	
	MODBUS_FACTORY_DEFAULTS,     			// 0	1	Factory defaults  0=no default
	MODBUS_COOLING_SETPOINT,				// 15	50	(59 and 99 for degrees F)	Cooling setpoint (1degree)
	MODBUS_HEATING_SETPOINT,  				// 10	35	(50 and 95 for degrees F)	Heating setpoint (1degree)
	MODBUS_FAN_SPEED ,  					// 0	4	Fan speed 0=OFF, 1=Low, 2=MED, 3=HI, 4=AUTO 
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
	MODBUS_VALVE_OPERATION_TABLE_BEGIN,	// Address 173 
	MODBUS_VALVE_OPER_TABLE_COOL1,
	MODBUS_VALVE_OPER_TABLE_COOL2,
	MODBUS_VALVE_OPER_TABLE_COOL3,
	MODBUS_VALVE_OPER_TABLE_HEAT1,
	MODBUS_VALVE_OPER_TABLE_HEAT2,
	MODBUS_VALVE_OPER_TABLE_HEAT3,
	MODBUS_EXTERNAL_SENSOR_0,				// -	-	External Sensor 0 reading
	MODBUS_EXTERNAL_SENSOR_1,				// -	-	External Sensor 1 reading
	MODBUS_NIGHT_HEATING_SETPOINT,			// 10	99	Night Heating Setpoint	(1 degree)
	MODBUS_NIGHT_COOLING_SETPOINT,			// 10	99	Night Cooling Setpoint	(1 degree)
	MODBUS_INFO_BYTE,						// -	-	Byte that holds info about the tstat  // 		bit0 = occupancy bit - 1=occupied, 0=unoccupied	//		bit1 = watchdog reset - 1 indicates a reset due to watchdog//		bit2 = serial write reset prevention
	MODBUS_BAUDRATE,						// 0	1	Baudrate 0 = 9.6kb/s, 1 = 19.2kb/s
	MODBUS_OUTPUT1_SCALE,		//186			// 0	3	Sets the full scale voltage of analog output 1											//		0=ON/OFF, 1=0-10V, 2=0-5V, 3=4-20ma, 
	MODBUS_OUTPUT2_SCALE,		//187			// 0	3	Sets the full scale voltage of analog output 2//		0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 
	MODBUS_ANALOG_IN1 ,  					// 0	1	Analog input 1 range. 
	MODBUS_ANALOG_IN2,						// 0	0	Analog input 2 range.  RESERVED FOR FUTURE USE
	MODBUS_DIGITAL_IN1,						// 0	2	Digital occupied mode trigger 	//		0 = OFF, 1 = trigger unoccupied on falling edge, 2 = occupied high, unoccupied low
	MODBUS_OUTPUT1_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT2_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT3_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT4_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT5_DELAY_OFF_TO_ON, 		// 0	255	OFF to ON delay time (sec)
	MODBUS_OUTPUT1_DELAY_ON_TO_OFF,  		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT2_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT3_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT4_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_OUTPUT5_DELAY_ON_TO_OFF, 		// 0	255	ON to OFF delay time (sec)
	MODBUS_CYCLING_DELAY ,					// 0	20	Delay between going out of heating or cooling and then back in. (1 min)      			
	MODBUS_CHANGOVER_DELAY,					// 0	200	Delay between going from cooling to heating or vice versa (1 min)
	MODBUS_DISPLAY,							// 0	1	Display setting.  0 = temperature, 1 = setpoint
	MODBUS_LED1_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED2_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED3_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED4_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED5_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED6_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED7_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_OVERRIDE_TIMER,			// 0	255	Determines what controls the state of the LED
	MODBUS_OVERRIDE_TIMER_LEFT,
	MODBUS_FILTER,
	MODBUS_HEAT_COOL_CONFIG,
	MODBUS_INTERNAL_TEMP_IC,
	MODBUS_INTERNAL_THERMISTOR,
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR,	  //217
	MODBUS_CALIBRATION_ANALOG_IN2,
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
	MODBUS_KEYPAD_VALUE,						//		304	  //INPUT HUMILITY CALIBRATION POINT
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
	MODBUS_DEFAULT_SETPOINT,
	MODBUS_SETPOINT_CONTROL,					//		339	 
	MODBUS_PIR_STAGE,
	MODBUS_ANALOG1_RANGE, 						//		341
	MODBUS_ANALOG2_RANGE,  						//		342
	MODBUS_ANALOG3_RANGE, 						//		343
	MODBUS_ANALOG4_RANGE, 						//		344
	MODBUS_ANALOG5_RANGE, 						//		345
	MODBUS_ANALOG6_RANGE,  						//		346
	MODBUS_ANALOG7_RANGE, 						//		347
	MODBUS_ANALOG8_RANGE, 						//		348
	MODBUS_ANALOG_INPUT1,  						//		349
	MODBUS_ANALOG_INPUT2, 						//		350
	MODBUS_ANALOG_INPUT3, 						//		351
	MODBUS_ANALOG_INPUT4,						//		352
	MODBUS_ANALOG_INPUT5,  						//		353
	MODBUS_ANALOG_INPUT6,						//		354
	MODBUS_ANALOG_INPUT7,  						//		355
	MODBUS_ANALOG_INPUT8,						//		356	
	MODBUS_CALIBRATION_ANALOG1,					//		357
	MODBUS_CALIBRATION_ANALOG2,					//		358
	MODBUS_CALIBRATION_ANALOG3,					//		359
	MODBUS_CALIBRATION_ANALOG4,					//		360
	MODBUS_CALIBRATION_ANALOG5,					//		361
	MODBUS_CALIBRATION_ANALOG6,					//		362
	MODBUS_CALIBRATION_ANALOG7,					//		363
	MODBUS_CALIBRATION_ANALOG8,					//		364
	MODBUS_OUTPUT_VOLT_ADJ_TEM ,					//365
	MODBUS_OUTPUT_VOLT_ADJ_HUM ,
	MODBUS_OUTPUT_CURRENT_ADJ_TEM ,					//
	MODBUS_OUTPUT_CURRENT_ADJ_HUM ,
	MODBUS_OUTPUT_CURRENT_ADJ_STEP,  //369
	MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL ,	// 370 bit 0 : temperature manual value set;  bit 1 :hummidity manual value set	                                        //0: default value  1: user manual        
	MODBUS_OUTPUT_MANUAL_VALUE_TEM,
	MODBUS_OUTPUT_MANUAL_VALUE_HUM,
	MODUBS_HUMIDITY_RH,				//373 relative humidity in percentage
	MODBUS_HUMIDITY_FREQUENCY,		// raw frequency reading
	MODBUS_HUM_PIC_UPDATE,			//
	MODBUS_HUM_CAL_NUM,				//
	MODBUS_HUM_CAL_EREASE,			//
	MODBUS_HUM_CURRENT_DEFAULT,		// //decide which table will run, default tabel or custom table   current=1 default=0
	MODBUS_HUM_PICDATAOK,	        // picdataok
	MODBUS_HUM_OFFSETFLAG,				//380  slope 
	MODBUS_HUMCOUNT1_H ,		    //381  sensor frequency	   //these parameters must be continued,it won't work  if have blank,don't know why
	MODBUS_HUMRH1_H	,				//  calibration point
	MODBUS_HUMCOUNT2_H ,			//
	MODBUS_HUMRH2_H	,				//
	MODBUS_HUMCOUNT3_H ,			//
	MODBUS_HUMRH3_H	,				//
	MODBUS_HUMCOUNT4_H ,			//
	MODBUS_HUMRH4_H	,				//
	MODBUS_HUMCOUNT5_H ,			//
	MODBUS_HUMRH5_H	,				//
	MODBUS_HUMCOUNT6_H ,			//
	MODBUS_HUMRH6_H	,				//
	MODBUS_HUMCOUNT7_H ,			//
	MODBUS_HUMRH7_H	,				//
	MODBUS_HUMCOUNT8_H ,			//
	MODBUS_HUMRH8_H	,				//
	MODBUS_HUMCOUNT9_H ,			//
	MODBUS_HUMRH9_H	,				//
	MODBUS_HUMCOUNT10_H ,			//
	MODBUS_HUMRH10_H,				//400   don't have _L
	MODBUS_TEM_OUTPUT_CALIBRATION1 ,			//401
	MODBUS_TEM_OUTPUT_CALIBRATION2 ,
	MODBUS_TEM_OUTPUT_CALIBRATION3 ,
	MODBUS_TEM_OUTPUT_CALIBRATION4 ,
	MODBUS_TEM_OUTPUT_CALIBRATION5 ,
	MODBUS_TEM_OUTPUT_CALIBRATION6 ,
	MODBUS_TEM_OUTPUT_CALIBRATION7 ,
	MODBUS_TEM_OUTPUT_CALIBRATION8 ,
	MODBUS_TEM_OUTPUT_CALIBRATION9 ,
	MODBUS_TEM_OUTPUT_CALIBRATION10 , //410
	MODBUS_HUM_OUTPUT_CALIBRATION1 ,         //  write teh table to the pic when this is 1, do nothng when =0
	MODBUS_HUM_OUTPUT_CALIBRATION2 ,
	MODBUS_HUM_OUTPUT_CALIBRATION3 ,
	MODBUS_HUM_OUTPUT_CALIBRATION4 ,
	MODBUS_HUM_OUTPUT_CALIBRATION5 ,
	MODBUS_HUM_OUTPUT_CALIBRATION6 ,
	MODBUS_HUM_OUTPUT_CALIBRATION7 ,
	MODBUS_HUM_OUTPUT_CALIBRATION8 ,
	MODBUS_HUM_OUTPUT_CALIBRATION9 ,
	MODBUS_HUM_OUTPUT_CALIBRATION10 ,   // 420
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_1, 
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_2,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_3, 
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_4,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_5,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_6,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_7,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_8,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_9,
	MODBUS_TEM_OUTPUT_CURRENT_CALIBRATION_10,  //430
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_1, 
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_2, 
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_3, 
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_4,
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_5,
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_6,
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_7,
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_8,
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_9,
	MODBUS_HUM_OUTPUT_CURRENT_CALIBRATION_10,  // 440 
	MODBUS_HUM_CALIBRATION_LOCKX,              //
	MODBUS_HUM_CALIBRATION_LOCKY, 
	MODBUS_OUTPUT_TEMP_SENSOR_RANGE, //443
	MODBUS_HUM_SERIE_NUM_L,	 //444 store hum sensor serial nember low byte
	MODBUS_HUM_SERIE_NUM_H,	 //445 store hum sensor serial nember high byte
	MODBUS_TEST1,  //
	MODBUS_TEST2,
	MODBUS_TEST3,
	MODBUS_TEST4,
	MODBUS_TEMP_OFFSET,
	MODBUS_HUM_OFFSET ,
	MODBUS_TEMP_FILTER ,
	MODBUS_HUM_FILTER, 	                                               
};

int m_EDIT_ID;
CMsflexgrid m_sensor_table;
 
//CComboBox m_outrange1;
CComboBox m_combox_range2;
CComboBox m_combox_am1;
CComboBox m_combox_am2;
CComboBox m_combox_am3;
CComboBox m_combox_am4;
CComboBox m_combox_range1;
CComboBox m_combox_unit;

CEdit m_edit_hum;
CEdit m_edit_temp;
CEdit m_edit_hum_filter;
CEdit m_edit_hum_signal;
CEdit m_edit_temp_filter;
CEdit m_edit_temp_signal;
 HANDLE hFirstThread;
virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
afx_msg void OnEnKillfocusEditTempValue();
afx_msg void OnCbnSelchangeComboUnits();
afx_msg void OnEnKillfocusEditHumValue();
afx_msg void OnCbnSelchangeComboAm1();
afx_msg void OnCbnSelchangeComboAm2();
afx_msg void OnCbnSelchangeComboRange1();
afx_msg void OnCbnSelchangeComboRange2();
afx_msg void OnDestroy();
afx_msg void OnEnSetfocusEditTempValue();
afx_msg void OnEnSetfocusEditHumValue();
virtual BOOL PreTranslateMessage(MSG* pMsg);
afx_msg void OnEnKillfocusEditTempFilter();
afx_msg void OnEnKillfocusEditHumFilter();
afx_msg void OnEnSetfocusEditTempFilter();
afx_msg void OnEnSetfocusEditHumFilter();
afx_msg void OnBnClickedButtonHumDefault();
afx_msg void OnBnClickedButtonTempDefault();
CEdit m_Edit_Fre;
afx_msg void OnBnClickedUserTableAdd();
ListCtrlEx::CListCtrlEx m_user_list;
ListCtrlEx::CListCtrlEx m_factory_list;

void Initial_UserList();
void Initial_FactoryList();
afx_msg void OnBnClickedUserTableDel();
void ShowChecks();
 afx_msg LRESULT Fresh_User_Item(WPARAM wParam,LPARAM lParam);
 CButton m_check_user;
 CButton m_check_default;
 afx_msg void OnBnClickedCheckDefault();
 afx_msg void OnBnClickedCheckUser();
};
