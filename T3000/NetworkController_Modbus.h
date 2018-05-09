#define OFFSET_MODBUS_ADDRESS	100 			// Offset for 2nd block of registers
#define SCHEDUAL_MODBUS_ADDRESS 200
enum {
	// MODBUS REGISTER				// MIN	MAX	DESCRIPTION
	// Address 0  
	MODBUS_SERIALNUMBER_LOWORD   ,          	// -	-	Lower 2 bytes of the serial number
	MODBUS_SERIALNUMBER_HIWORD  = 2  ,		// -	-	Upper 2 bytes of teh serial number
	MODBUS_VERSION_NUMBER_LO    = 4  ,		// -	-	Software version
	MODBUS_VERSION_NUMBER_HI,			// -	-	Software version
	MODBUS_ADDRESS,					// 1	254	Device modbus address
	MODBUS_PRODUCT_MODEL,				// -	-	Temco Product Model	100,network controller 
	MODBUS_HARDWARE_REV,				// -	-	Hardware Revision	 
	MODBUS_ADDRESS_PLUG_N_PLAY=10,			// -	-	Temporary address for plug-n-play addressing scheme
	
	MODBUS_TIME_ZONE,				//0	255	Time zone
	MODBUS_BAUDRATE,				//0	1	network controller baudrate 0,9600 1,19200
	MODBUS_DAYLIGHT_ENABLE,                         //0	1	daylighting switch,0 disable daylighting feature,1 enable
	MODBUS_UPDATE_STATUS	= 16,			// reg 16 status for update_flash
	MODBUS_REFRESH_STATUS   = 90,
	
	MODBUS_SERINALNUMBER_WRITE_FLAG		= EEP_SERINALNUMBER_WRITE_FLAG ,//for debug	

	MODBUS_MAC_ADDRESS			= OFFSET_MODBUS_ADDRESS , 	//reg100,MAC address ,read only
	MODBUS_IP_MODE         		= MODBUS_MAC_ADDRESS + 6 ,		//0	1	,reg106,IP mode. 0,static ip; 1,dhcp 
	MODBUS_IPADDRESS_LOWORD ,						//0	255	,reg107,right two bytes of IP address										//107
	MODBUS_IPADDRESS_HIWORD 	= MODBUS_IPADDRESS_LOWORD + 2 ,		//0	255	,reg109,left two bytes of IP address
	MODBUS_SUBNET_LOWORD    	= MODBUS_IPADDRESS_HIWORD + 2 ,		//0	255	,reg111,right two bytes of SUBNET MASK address		
	MODBUS_SUBNET_HIWORD    	= MODBUS_SUBNET_LOWORD + 2 ,		//0	255	,reg113,left two bytes of SUBNET MASK address
	MODBUS_GATEWAY_LOWORD   	= MODBUS_SUBNET_HIWORD + 2 ,		//0	255	,reg115,right two bytes of GATEWAY address	
	MODBUS_GATEWAY_HIWORD   	= MODBUS_GATEWAY_LOWORD + 2 ,		//0	255	,reg117,left two bytes of GATEWAY address
	MODBUS_HOST_MODE   		= MODBUS_GATEWAY_HIWORD + 2 ,		//0	0	,reg119,0,tcp server;  
	MODBUS_LISTENING_PORT ,							//0	32767	,reg120,listen port at TCP server mode
	MODBUS_DESTINATION_ADDRESS_LOWORD ,					//0	255	,reg121,right two bytes of destination address
	MODBUS_DESTINATION_ADDRESS_HIWORD	= MODBUS_DESTINATION_ADDRESS_LOWORD + 2 ,//0	255	,reg123,left two bytes of destination address
	MODBUS_DESTINATION_PORT    	= MODBUS_DESTINATION_ADDRESS_HIWORD + 2 ,//0	32767	,reg125,destination port
	MODBUS_CONNECTION_INTERVAL ,		//0	32767	,reg126,the internal try to connect host
	MODBUS_INACTIVITY_TIMEOUT  ,		//0	32767	,reg127,inactivity timeout	
	MODBUS_SERIAL_BAUDRATE ,		//0	1	,reg128, tcp/ip module baudrate. 0,9600; 1,19200    
	MODBUS_SERIAL_INTERCHAR_TIMEOUT , 	//10	5000	,reg129, serial port interchar timeout
	MODBUS_FACTORY_DEFAULT ,		//0	1	,reg130,0,tcp/ip module all parameters are default ; 1, tcp/ip module all parameters except ip are default
	MODBUS_NEMO_SAVE , 			//0	1	,reg131,0,normal work;1,save the nemo10 copfigure data
	MODBUS_NEMO_GET , 			//0	1	,reg132,0,normal work;1,get the nemo10 copfigure data
	MODBUS_NEMO_REBOOT ,			//0	1	,reg133,0,NORMAL WORK; 1, save the configure data and disconnect the communication between NEMO10 and PC
	
 //The following registers are using for scheduals.There are several type data and set up the scheduals must use Write multiple.
 //Register 200 to 207 use for clock.The data length in Write multiple is 8.
 // MODBUS REGISTER			MIN	MAX	DESCRIPTION
 // 200 				0	255	higher byte of the yaer,for example,here should be 19 in the year 1998	
 // 201					0	255 	lower byte of the year,for example,here should be 98 in year 1998
 // 202					1	12	month	
 // 203					0	6	week	,0,Sunday;1,Monday;and so on.
 // 204					1	31	day	
 // 205					0	23	hour	
 // 206					0	59	minute	
 // 207					0	59	second		
	MODBUS_TIMER_ADDRESS		= SCHEDUAL_MODBUS_ADDRESS ,//reg200 network controller date and time
	
//Register 208 to 827 use for weekly routine description.The data length is 31 in Write Multiple and the maximum weekly routine is 20.
	MODBUS_WR_DESCRIP_FIRST		= MODBUS_TIMER_ADDRESS + 8 ,//reg208 weekly routine descripton
	MODBUS_WR_DESCRIP_LAST		= MODBUS_WR_DESCRIP_FIRST + WR_DESCRIPTION_SIZE * MAX_WR ,
	
//Register 828 to 1291 use for annual routine description.The data length is 29 in Write Multiple and the annual routine is 16.	
	MODBUS_AR_DESCRIP_FIRST		= MODBUS_WR_DESCRIP_LAST ,//reg828,annual routine description
	MODBUS_AR_DESCRIP_LAST		= MODBUS_AR_DESCRIP_FIRST + AR_DESCRIPTION_SIZE * MAX_AR ,

//Register 1292 to 2053 use for ID schedule.The data length is 3 in Write Multiple and the maximum ID routine is 254.
	MODBUS_ID_FIRST			= MODBUS_AR_DESCRIP_LAST ,//reg1292 ID schedual
	MODBUS_ID_LAST			= MODBUS_ID_FIRST + ID_SIZE * MAX_ID ,

//Register 2054 to 2789 use for annual routine time.The data length is 46 in Write Multiple and the maximum annual routine is 16.	
	MODBUS_AR_TIME_FIRST		= MODBUS_ID_LAST ,//reg2054 annual routine time
	MODBUS_AR_TIME_LAST		= MODBUS_AR_TIME_FIRST + AR_TIME_SIZE * MAX_AR ,

//Register 2790 to 4229 use for weekly routine ON time.The data length is 72 in Write Multiple and the maximum weekly routine is 20.	
	MODBUS_WR_ONTIME_FIRST		= MODBUS_AR_TIME_LAST ,//reg2790,weekly routine on time
	MODBUS_WR_ONTIME_LAST		= MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE * MAX_WR ,

//Register 208 to 827 use for weekly routine OFF time.The data length is 72 in Write Multiple and the maximum weekly routine is 20.	
	MODBUS_WR_OFFTIME_FIRST		= MODBUS_WR_ONTIME_LAST ,//reg4230 weekly routine off time
	MODBUS_WR_OFFTIME_LAST		= MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE * MAX_WR ,

	MODBUS_TOTAL_PARAMETERS		= MODBUS_WR_OFFTIME_LAST
	
	
} ;

 

 