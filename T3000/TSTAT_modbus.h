// *******************modbus.h***********************************
// Header file containing all of the register information for modbus 
// serial communications.
// V.24 first release of modbus.h file.
// V.25

//caution:the tstat response should have a pause between two characrers,but the maximum allowed pause is 1.5 character times or .83 ms * 1.5 or 1.245 ms at 9600 baud.


//  REGISTER ADDRESSES TO BE USED IN MODBUS SERIAL COMMUNICATION
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

	                                        // registers needed for updating status
	MODBUS_UPDATE_STATUS	= 16,			// reg 16 status for update_flash
											// writing 0x7F means jump to ISP routine
											// writing 0x8F means completely erase eeprom
	MODBUS_UPDATE_PTR_HI,					// reg 17 pointer for last attempt upload HI
	MODBUS_UPDATE_PTR_LO,					// reg 18 pointer for last attempt upload LO

	MODBUS_SCRATCHPAD_ADDRESS   ,			// -	-	Internal scratch pad address	
	
	
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
  	MODBUS_CALIBRATION_ANALOG_IN1 , 			// 0	255	External thermistor calibration term 
											//		0 = Temperature - 12.5 degrees, 125 = Temperature + 0 degrees, 255 = Temperature + 13 degrees
  	MODBUS_TEMP_SELECT ,  					// 0	1	Temperature select, 0=internal, 1=external
  	MODBUS_DAC_OFFSET ,						// 0	255	DAC voltage offset
  	MODBUS_DELAY ,							// 0	120	Short cycle prevention delay	(0.1 seconds) - NOT USED IN REV25
  	MODBUS_COOLING_PTERM ,      			// 0	255	Cooling P term (0.1 degree)
  	MODBUS_COOLING_ITERM ,  				// 0	255	Cooling I term (0.1 %deg/min)
  	MODBUS_HEATING_PTERM ,      			// 0	255	Heating P term (0.1 degree) - NOT USED IN REV25
  	MODBUS_HEATING_ITERM ,  				// 0	50	Heating I term (1 %deg/min) - NOT USED IN REV25
  	MODBUS_SEQUENCE ,  						// 0	2	Sequence of operation , tstat behaves differently according to sequence
  	MODBUS_COOLING_DEADBAND ,    			// 1	100	Cooling deadband	(0.1 degree)

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
  	MODBUS_AUTO_ONLY ,						// 0	2	Enable or disable manual modes.  0=manual allowed, 1=auto only
	
	// Address 130 
	MODBUS_OUTPUT_SCALE ,  					// 0	7	Sets the full scale voltage of the outputs - NOT USED IN REV25
				    						//		0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 4=OFF/ON, 5=10-0V, 6=5-0V, 7=10-2V
  	MODBUS_MAX_SETPOINT,            		// 10	99	(50 and 150 for degrees F)	Max cooling setpoint (1degree)
  	MODBUS_MIN_SETPOINT,					// 10	99	(50 and 150 for degrees F)	Min heating setpoint (1degree)
  	MODBUS_SPECIAL_MENU_LOCK,				// 0	3	Special menu lockout via keypad, serial port only 0=unlock, 1=lock	
  	MODBUS_FACTORY_DEFAULTS,     			// 0	1	Factory defaults  0=no default
  	MODBUS_COOLING_SETPOINT ,				// 15	50	(59 and 99 for degrees F)	Cooling setpoint (1degree)
  	MODBUS_HEATING_SETPOINT ,  				// 10	35	(50 and 95 for degrees F)	Heating setpoint (1degree)
  	MODBUS_FAN_SPEED ,  					// 0	4	Fan speed 0=OFF, 1=Low, 2=MED, 3=HI, 4=AUTO

	
	// Binary output state for each of the 8 modes of operation
						// Address 138 
	MODBUS_FAN0_OPER_TABLE_COAST,      	// 
	MODBUS_FAN0_OPER_TABLE_COOL1,		// 
	MODBUS_FAN0_OPER_TABLE_COOL2,      	// 
	MODBUS_FAN0_OPER_TABLE_COOL3,      	// 
	MODBUS_FAN0_OPER_TABLE_HEAT1,      	// 
	MODBUS_FAN0_OPER_TABLE_HEAT2,      	// 
	MODBUS_FAN0_OPER_TABLE_HEAT3,      	// 
	
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
		
	MODBUS_FANAUT_OPER_TABLE_COAST,      	// Address 166
	MODBUS_FANAUT_OPER_TABLE_COOL1,		// 
	MODBUS_FANAUT_OPER_TABLE_COOL2,      	// 
	MODBUS_FANAUT_OPER_TABLE_COOL3,      	// 
	MODBUS_FANAUT_OPER_TABLE_HEAT1,      	// 
	MODBUS_FANAUT_OPER_TABLE_HEAT2,      	// 
	MODBUS_FANAUT_OPER_TABLE_HEAT3,      	// 
	
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
	MODBUS_DISPLAY,							// 0	6	Display setting.  0 = temperature, 1 = setpoint

	// Address 204
	MODBUS_LED1_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED2_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED3_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED4_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED5_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED6_CONTROL,					// 0	22	Determines what controls the state of the LED
	MODBUS_LED7_CONTROL,					// 0	22	Determines what controls the state of the LED
             					//CONTROL FUNCTION		//VALUE
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
	MODBUS_OVERRIDE_TIMER,			// 0	255	Unoccupied Override Timer,Ort 0 = disabled,not 0=number of minutrs manual override is allowed
   	MODBUS_OVERRIDE_TIMER_LEFT,		// 0	255	number of minutes remaining on the timer when unoccupied override timer is in effect
	MODBUS_FILTER,				// 0	100 	weighted average of stored value to new raw value
	MODBUS_HEAT_COOL_CONFIG,		// 0	5	heating cooling mode configuration 
	MODBUS_INTERNAL_TEMP_IC,		// 0	3000	the temperature value read from internal temperature sensor
	MODBUS_INTERNAL_THERMISTOR,		// 0 	3000	the temperature value read from internal thermistor sensor
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR,	// 0	1000	calibration value used on the internal sensor
	MODBUS_CALIBRATION_ANALOG_IN2,		// 0	1000 	calibration value used on the analoginput2
        // Address 219
        MODBUS_TABLE1_ZERO,                     // 0     65535	the range from reg 219 to reg 229.A changable look up table for "custom" sensor 
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
	MODBUS_TABLE2_ZERO,			// 0	65535	the range from reg 230 to reg 240.A changable look up table for "custom" sensor 
	MODBUS_TABLE2_HALFONE,			//              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
	MODBUS_TABLE2_ONE,			//              the value's unit in the table is different according to different sensor
	MODBUS_TABLE2_HALFTWO,
	MODBUS_TABLE2_TWO,
	MODBUS_TABLE2_HALFTHREE,
	MODBUS_TABLE2_THREE,
	MODBUS_TABLE2_HALFFOUR,
	MODBUS_TABLE2_FOUR,
	MODBUS_TABLE2_HALFFIVE,
	MODBUS_TABLE2_FIVE ,
	//Address 241
	MODBUS_INPUT1_SELECT  ,                 // 0    2	universal PID input select
	MODBUS_UNIVERSAL_DB_HI ,		// 0	65535	universal PID upper deadband	
	MODBUS_UNIVERSAL_DB_LO ,		// 0	65535	universal PID lower deadband
        MODBUS_UNIVERSAL_PTERM ,    		// 0	65535	universal PID pterm
	MODBUS_UNIVERSAL_ITERM ,    		// 0	65535	universal PID iterm
	MODBUS_UNIVERSAL_SET ,    		// 0	65535   universal PID setpoint 
	//Address 247	
	MODBUS_PID_OUTPUT1 ,   			// 0	3	be used in the output table 
	MODBUS_PID_OUTPUT2 ,   			//		decide which pid be used to control output
	MODBUS_PID_OUTPUT3 ,   
	MODBUS_PID_OUTPUT4 ,   
	MODBUS_PID_OUTPUT5 ,   
	MODBUS_PID_OUTPUT6 ,   
	MODBUS_PID_OUTPUT7 ,
	//Address 254	
	MODBUS_UNIVERSAL_OUTPUT_BEGIN , 	// 0	255	 Binary output state for each of the 7 modes of operation
	MODBUS_UNIVERSAL_OUTPUT_COOL1 ,
	MODBUS_UNIVERSAL_OUTPUT_COOL2 ,
	MODBUS_UNIVERSAL_OUTPUT_COOL3 ,
	MODBUS_UNIVERSAL_OUTPUT_HEAT1 ,
	MODBUS_UNIVERSAL_OUTPUT_HEAT2 ,
	MODBUS_UNIVERSAL_OUTPUT_HEAT3 ,
	//Address 261
	MODBUS_UNIVERSAL_VALVE_BEGIN ,          // 0	255	 Analog output state for each of the 8 modes of operation
	MODBUS_UNIVERSAL_VALVE_COOL1 ,
	MODBUS_UNIVERSAL_VALVE_COOL2 ,
	MODBUS_UNIVERSAL_VALVE_COOL3 ,
	MODBUS_UNIVERSAL_VALVE_HEAT1 ,
	MODBUS_UNIVERSAL_VALVE_HEAT2 ,
	MODBUS_UNIVERSAL_VALVE_HEAT3 ,
	//Address 268
	MODBUS_HEAT_UNIVERSAL_TABLE,		// 0	6 	number of heating stages in universal table 
	MODBUS_COOL_UNIVERSAL_TABLE,    	// 0	6	number of cooling stages in universal table  	 
	MODBUS_PID_UNIVERSAL ,  		// 0	100	register 270,universal pid value  
	MODBUS_UNITS1_HIGH ,                    // 0	65535   register 271,upper 2 bytes of universal table1 units in ASCII
	MODBUS_UNITS1_LOW  ,  			// 0	65535	register 272,lower 2 bytes of universal table1 units in ASCII
	MODBUS_UNITS2_HIGH , 			// 0	65535   register 273,upper 2 bytes of universal table2 units in ASCII
	MODBUS_UNITS2_LOW  ,                    // 0	65535   register 274,lower 2 bytes of universal table2 units in ASCII
	MODBUS_UNIVERSAL_NIGHTSET ,		// 0	65535   universal pid night setpoint
	MODBUS_HEAT_ORIGINAL_TABLE , 		// 0	6 	register 276,number of heating satges in original table     
	MODBUS_COOL_ORIGINAL_TABLE ,   		// 0	6	register 277,number of coling stages in original table
	MODBUS_PID2_MODE_OPERATION,		// 0	19      register 278,cooling or heating states when universal PID is working
						//		coasting 0  cooling1 1  cooling2 2  cooling3 3  cooling4 14  cooling5 15  cooling6 16
						//		 	    heating1 4  heating2 5  heating3 6  heating4 17  heating5 18  heating6 19
	MODBUS_VALVE_TRAVEL_TIME ,		// 10	255	register 279,the max time that the valve travel from one end to another end,the real value should be divided 10,the units is second
	//Addres 280
	MODBUS_MODE_OUTPUT1,			// 0	0	now output 1 to 3 has only one mode,it is ON/OFF mode
	MODBUS_MODE_OUTPUT2,			// 0 	0
	MODBUS_MODE_OUTPUT3,			// 0	0
	MODBUS_MODE_OUTPUT4,			// 0	1                Outputs 4 and 5 can be assigned as PWM valves (thermal wax or electric heat type valves). 
	MODBUS_MODE_OUTPUT5,			// 0	1 	0:ON/OFF; 1:floating valve
	MODBUS_VALVE_PERCENT,			// 0	100	register 285,the floating valve position in percent
	MODBUS_INTERLOCK_OUTPUT1,		// 0	5                register 286,Interlocks for the outputs. 
	                                                                                                                          	    0, No interlocks, output behaves according to the normal output table settings
              	                                                                                                                                  1, Interlock with Digital input1, in1 must be on before this relay will enable
                                                                                                                                                    2, Interlock with AI1 (when assigned a binary on/off type range)
                                                                                                                                                    3, Interlock with AI2 (when assigned a binary on/off type range)
                                                                                                                                                    4, Interlock with the general timer OR pid1
                                                                                                                                                    5, Interlock with the general timer AND pid1
	MODBUS_INTERLOCK_OUTPUT2,		// 0	5	register 287
	MODBUS_INTERLOCK_OUTPUT3,		// 0	5	register 288
	MODBUS_INTERLOCK_OUTPUT4,		// 0	5	register 289
	MODBUS_INTERLOCK_OUTPUT5,		// 0	5	register 290
	MODBUS_INTERLOCK_OUTPUT6,		// 0	5	register 291
	MODBUS_INTERLOCK_OUTPUT7,		// 0	5	register 292
	MODBUS_SETPOINT_INCREASE,		// 1	10	register 293 Setpoint increment 
                                                                                                                                                                       UNits are x10 DegC or F, 
                                                                                                                                                                        ie: a value of 10 give 1Deg increment. 
	MODBUS_LAST_KEY_TIMER  			// 0	65535	register 294 Number of seconds since the last keypress.
                                                                                                                                                                       Used to see if someone was in the room recently, can trigger occupied mode for example 

	MODBUS_FREEZE_TEMP_SETPOINT,		// -127 +127               register 295, Freeze protection setpoint, if unit goes below this temperature the heating mode will be triggered after the delay period. 
	MODBUS_FREEZE_DELAY_ON,			// 5    255                    register 296  Delay this long before going into freeze protect mode. 
	MODBUS_FREEZE_DELAY_OFF,		// 5    255                    register 297  Freezze Protect period setting, unit will go to heat mode for this long
	MODBUS_ANALOG1_FUNCTION,		// 0	2	298,Function setting for Analog input 1.
                                                                                                                                                               0=normal analog input
                                                                                                                                                               1=freeze protect
                                                                                                                                                               2=occupancy sensor
	MODBUS_ANALOG2_FUNCTION,		// 0	2	299,Function setting for Analog input 2.
                                                                                                                                                               0=normal;
                                                                                                                                                               1=freeze protect
                                                                                                                                                               2=occupancy sensor
	MODBUS_DIGITAL1_FUNCTION,		// 0	2	300,Function setting for the digital input1. 
                                                                                                                                                               0=normal;
                                                                                                                                                               1=freeze protect
                                                                                                                                                               2=occupancy sensor
	MODBUS_TIMER_ON,			// 0	65535	301,General timer, this is the value for the ON time in PWM mode. 
	MODBUS_TIMER_OFF,			// 0	65535	302,General timer, this is the valuve for the OFF time in PWM mode
	MODBUS_TIMER_UNITS,			// 0	2	303,timer units.0,hour;1,minute;2,second
	MODBUS_KEYPAD_VALUE,			// 0	255	304,Keypad capture, user applications can read this register to capture the keypad hits. 
	MODBUS_DISPLAY_HUNDRED,			// 0	255	305,The hundreds digit of the LED display, write to this register when showing custom values on the display. 
	MODBUS_DISPLAY_TEN,			// 0	255	306,Tens digit. 
	MODBUS_DISPLAY_DIGITAL,			// 0	255	307,Ones digit. 
	MODBUS_DISPLAY_STATUS,			// 0	255	308,The status bit of LEDs will show the value which receive from serial port
	MODBUS_INPUT_MANU_ENABLE,		// 0	255	309,manual or auto mode select,0,auo;1,manual.each bit of this byte correspond to an input channel.0,analg1;1analog2;2,digital1
	MODBUS_OUTPUT_MANU_ENABLE,		// 0	255	310,manual or auto mode select,0,auo;1,manual.each bit of this byte correspond to an output .bit0--4, to output1--output5
	MODBUS_DIGITAL_INPUT,			// 0	1	311,digital input1 value,only valid when the correspond bit is set manual
	MODBUS_MANU_RELAY1,			// 0	1	312.open or close relay1 when the relay1 is set to work in MANUAL mode.write 0 to the register close the relay manually,rverse write 1 
 	MODBUS_MANU_RELAY2,			// 0	1	313.open or close relay2 when the relay2 is set to work in MANUAL mode.write 0 to the register close the relay manually,rverse write 1 
	MODBUS_MANU_RELAY3,			// 0	1	314.open or close relay3 when the relay3 is set to work in MANUAL mode.write 0 to the register close the relay manually,rverse write 1 
	MODBUS_MANU_RELAY4,			// 0	1	315.open or close relay4 when the relay4 is set to work in MANUAL mode.write 0 to the register close the relay manually,rverse write 1 
	MODBUS_MANU_RELAY5,			// 0	1	316.open or close relay5 when the relay5 is set to work in MANUAL mode.write 0 to the register close the relay manually,rverse write 1 
	MODBUS_DEAD_MASTER,			// 0	255	317.Dead master parameter. If the Tstat loses communications with a master it will go to occupied mode. 
                                                                                                            //                                           The setting is in minutes, if no serial communication takes place for xx minutes then teh stat will automatically switch to occupied mode. 
                                                                                                                                                          The value of 0 will disable the feature. 
	MODBUS_ROUND_DISPLAY,			// 0	5	318.Rounding for the display.  
                                                                                                                                                            0 means round off the display to the nearest digit. The diplsay will show 22.0, 23.0, 24.0 and so on
                                                                                                                                                            1 means round to the nearest 1/10 unit, this is the default, display will show 22.1,22.2, 22.3 and so on. 
                                                                                                                                                            5 means round to the nearest half unit, display will show 22.0, 22.5, 23.0 and so on. 												 
	MODBUS_MIN_ADDRESS,			// 1	254	319.Set the minimum allowable network address for a particular Tstat. Used if you want to ensure the Tstats are always assigned within a certain block of address ID's
 	MODBUS_MAX_ADDRESS,			// 1	254	320.Maximum allowable network address for a tstat. 
	MODBUS_FAN_GRIDPOINT,			// 0	4	321.Gridpoint specific feature: Fan speed can set to five values.       
	MODBUS_MODE_GRIDPOINT,			// 0	3	322.Gridpoint specific feature: Four modes, 0 = OFF,1 = COOL,2 = HEAT,3 = EMERGENCY HEAT. 		
	MODBUS_HOLD_GRIDPOINT,			// 0	1	323.Gridpoint specific feature: Enable/disable hold mode.
 	MODBUS_CONFIGURATION,			// 0	1	324.For Two config feature, write 0 before loading the first configuration, write 1 before load second configuration.
	MODBUS_UTIL_MODE,			// 0	1	325.Gridpoint specific feature: electrical utility can put stat into load shedding mode. 
	MODBUS_EEPROM_SIZE,			// 0	1	326.Show the size of E2 chip.0 = 24c02,1 = 24c08/24c16.
	MODBUS_TIMER_SELECT,			// 0	3	327.Assign the timer be used for which feature.0 = period timer,1 = rotation timer,2 = interlock,3 = PWM timer.  
	MODBUS_OUTPUT1_FUNCTION,		// 0	2	328.The output1 function,there are three functions for the output1.0 = normal ON/OFF output,1 = rotation , 2 = lighting control. 
 	 				  												 
	MODBUS_ROTATION_OUT2,			// 2	5	329.Show which output table is using for this output when this output function be set rotation 
 	MODBUS_ROTATION_OUT3,			// 2	5	330.Show which output table is using for this output when this output function be set rotation     
	MODBUS_ROTATION_OUT4,			// 2	5	331.Show which output table is using for this output when this output function be set rotation  
	MODBUS_ROTATION_OUT5,			// 2	5	332.Show which output table is using for this output when this output function be set rotation   
	MODBUS_ROTATION_TIME_LEFT,		// 0	65535	333.How much time left before rotation action.  
	MODBUS_OUTPUT2_FUNCTION,		// 0	2	334.The output2 function,there are three functions for the output2.0 = normal ON/OFF output,1 = rotation , 2 = lighting control.  
	MODBUS_OUTPUT3_FUNCTION,		// 0	2	335.The output3 function,there are three functions for the output3.0 = normal ON/OFF output,1 = rotation , 2 = lighting control.  
	MODBUS_OUTPUT4_FUNCTION,		// 0	2	336.The output4 function,there are three functions for the output4.0 = normal ON/OFF output,1 = rotation , 2 = lighting control. 
	MODBUS_OUTPUT5_FUNCTION,		// 0	2	337.The output5 function,there are three functions for the output5.0 = normal ON/OFF output,1 = rotation , 2 = lighting control. 
	MODBUS_DEFAULT_SETPOINT,		// 0	255	338.Use this setpoint instead of normal setpoint when write 2 to  senpoint control  register(the next one).
	MODBUS_SETPOINT_CONTROL,		// 0	2	339.Show which setpoint in using.0 = normal setpoint,and triger an event which use noraml senpoint replace default setpoint, 1 = default setpoint,2 = triger an event which use default sepoint replace noraml setpoint.  
	MODBUS_PIR_STAGE,			// 0	1	340.Enable/disable PIR correspond 1/0 respectively.
	MODBUS_OUTPUT_PWM_AUTO_COAST,		// 0	255	341.PWM output range in COAST mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5
	MODBUS_OUTPUT_PWM_AUTO_COOL1,		// 0	255	342.PWM output range in COOLING1 mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5
	MODBUS_OUTPUT_PWM_AUTO_COOL2,		// 0	255	343.PWM output range in COOLING2 mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5 
	MODBUS_OUTPUT_PWM_AUTO_COOL3,		// 0	255	344.PWM output range in COOLING3 mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5	
	MODBUS_OUTPUT_PWM_AUTO_HEAT1,		// 0	255	345.PWM output range in HEATING1 mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5 
	MODBUS_OUTPUT_PWM_AUTO_HEAT2,		// 0	255	346.PWM output range in HEATING2 mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5 
	MODBUS_OUTPUT_PWM_AUTO_HEAT3,		// 0	255	347.PWM output range in HEATING3 mode.0 = CLOSE,1 = OPEN,2 = 0-100%,3 = 50-100%,4 = 0-50%.MSb 4 bits correspond to output4 and LSB 4 bits correspond to output5
	MODBUS_PWM_OUT4,			// 0	100	348.The ON period take how many percentage for output4
	MODBUS_PWM_OUT5,			// 0	100	349.The ON period take how many percentage for output5
	MODBUS_FREE_COOL_CONFIG,					//		350 Free cool configure register.bit0,free cool enable/disable,0 = disable,1= enable.
//bit1,free cool available decided by local or external master.0 =  local,1 = NC.bit2,free cool available status,0 = npt available,1= available.
//bit4,show the status if NC is OK when the free cool decided by NC.
	MODBUS_VALVE_OFF_TABLE_COAST,		//		351 OFF table for analog output,
	MODBUS_VALVE_OFF_TABLE_COOL1,
	MODBUS_VALVE_OFF_TABLE_COOL2,
	MODBUS_VALVE_OFF_TABLE_COOL3,
	MODBUS_VALVE_OFF_TABLE_HEAT1,
	MODBUS_VALVE_OFF_TABLE_HEAT2,
	MODBUS_VALVE_OFF_TABLE_HEAT3,		//		357  
	MODBUS_LOCK_REGISTER,			//		358	All registers except fan speed and manual input are not writable.0 = lock,1 = no lock.

	MODBUS_FREE_COOL_OUT_TEMP,		//		359	Outside temperature for free cooling,from external sensor or NC.
	MODBUS_NC_MASTER_TIME,			//		360 If outside temp be set from NC.The communication with NC must be set in this time,otherwise will set error status and use external sensor.
	MODBUS_FREE_COOL_TRIGGER,		//		361 If the outside air temp is lower than the room temeprature by this amount, then the free cooling is worthwhile, 350 bit2 = 1
          					//		If the OAT is greater than the room temp, then free cooling mode is not worhtwhile. , 350 bit2 = 0
	MODBUS_OUTPUT_FREE_COOL_COAST,		//		362 Output table in free cooling mode,0 = 0%, 1 = 1-00%,2 = MIN->100%,3 = MIN|100%,4 = MIN
	MODBUS_OUTPUT_FREE_COOL_COOL1,
	MODBUS_OUTPUT_FREE_COOL_COOL2,
	MODBUS_OUTPUT_FREE_COOL_COOL3,
	MODBUS_OUTPUT_FREE_COOL_HEAT1,
	MODBUS_OUTPUT_FREE_COOL_HEAT2,
	MODBUS_OUTPUT_FREE_COOL_HEAT3, 		//		368
	MODBUS_MIN_AIR,				//		369 The minimum ourput for free cooling  
	MODBUS_OAT_HOTTEST,			//		370 Outside air temperature in hottest day
	MODBUS_OAT_COLDEST,			//		371 Outside air temperature in coldest day
	MODBUS_OFFSET_HOTTEST,			//		372 Offset in hottest day
	MODBUS_OFFSET_COLDEST,			//		373 Offset in codest day
	MODBUS_TWO_BYTE_SETPOINT,		//		374 Store setpoint in two bytes ,the resolution is 0.1
	MODBUS_CURRENT_SETPOINT,		//		375 Crrent setpoint  = user setpoint  + offset setpoint
	MODBUS_SETPOINT_OFFSET,			//		376 Setpoint offset
	MODBUS_CHANGEOVER_SENSOR_MODE,		//		377	Change over sensor mode,1 = cooling mode,0 = heating mode.
 
};






 
