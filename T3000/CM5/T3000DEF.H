#ifndef _BURKEDEF_H
#define _BURKEDEF_H				/* avoid recompilation */

#define uint	unsigned int
#define byte	unsigned char
#define ulong	unsigned long
/*
#define TIME_COMMAND          21

#define COMMAND_50            50
#define STATION_LIST_COMMAND  21
#define DEFAULT_PRG_COMMAND   32
#define PANEL_INFO1_COMMAND  110
*/

#define DIGITAL 0
#define ANALOG 1
#define BLANKCHAR  ""
#define  sizeof_mini_var  6
#define  sizeof_mini_con  18
#define  sizeof_mini_wr  4
#define  sizeof_mini_prg  1000

#define NAME_SIZE  17

#ifdef WORKSTATION

#define MAX_OUTPUT 			0
#define MAX_INPUT 			0
#define MAX_VARIABLE 		0
#define MAX_DMONITOR 		0
#define MAX_AMONITOR 		0
#define MAX_GROUP	 		   0
#define MAX_PROGRAM 		   0
#define MAX_TABLE 			0
#define MAX_CONTROLLER 		0
#define MAX_WEEKLY_ROUTINE 0
#define MAX_ANNUAL_ROUTINE	0
#define MAX_STATUS 			0
#define MAX_PASSWORD 		0
#define MAX_SYSTEM         0
#define MAX_UNIT           0
#define MAX_MONITOR        0

#else

/**************************************
**********  William add  *************
********     11.19.1997  *************
**********   code begin  *************
**************************************/
#define MAX_NETSTATS	 32
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********    code end   *************
**************************************/

#define MAX_OUTS_MINI  64
#define MAX_INS_MINI   64

#define MAX_OUTS		 128
#define MAX_INS		   128
#define MAX_VARS		 128
#define MAX_PIDS		  48
#define MAX_SCHS		16
#define MAX_HOLS		8
#define MAX_PRGS		  32
#define MAX_TABS		   5
#define MAX_DIGM		   1
#define MAX_ANALM		  32
#define MAX_GRPS 		  32
#define MAX_ARRAYS	  16
#define MAX_INFO_TYPE 50
#define MAX_STATIONS  32

#endif

#define MAX_TBL_BANK          15

#define MAX_ELEMENT_GROUP     96
#define MAX_ELEM	            MAX_ELEMENT_GROUP
#define MAX_POINTS_IN_MONITOR 14

#define MAX_ICON_NAME_TABLE   16       // 16 is the value for MINI's. If icrease
													// the value for T3000, some code should be change
													// in send_grp() in group.cpp , net_call with
													// ICON_NAME_TABLE_COMMAND command

#define MAX_RANGE             99         /* number of range items */
#define MAX_FILE              43        /* maximum number of PRG files */
#define MAX_BUFFER_SIZE     2000        /* max size for one basic program */

#define SIZE_FULL_LABEL       21
#define SIZE_ABRV              9
#define SIZE_TEXT_RANGE       13         /* 12 characters for range text + null */
													  /* 10 characters for outputs though */
#define SIZE_TEXT_UNITS        6          /* 5 characeters plus a null   */


#define RELEASE_NUMBER		   343
#define MAX_STAND_GROUP_ELEM	64

#define MAX_RETRY		           1
#define YEAR_BYTES		        46
#define byte		 unsigned char
#define time_type	 unsigned long

#define MAX_ALARMS            48
#define MAX_ALARMS_SET        16

#define MAX_UNITS              8
#define MAX_PASSW             10
#define MAX_MONSET             1
//#define MAX_USERS
//#define MAX_DST
#define PROGRAM_SIZE         2000       
//#define PROGRAM_SIZE       14336       // 14K
#define PROGRAM_SIZE_MINI   8000
#define ALARM_MESSAGE_SIZE    58

#define MAX_RECORD            50

#define MAX_ALPHA          	  80
#define MAX_DES	           64000
#define MAX_VIEWS              3

#define OUT 		 0
#define IN  		 1
#define VAR 		 2
#define CON 		 3
#define WR 			 4
#define AR 			 5
#define PRG 		 6
#define TBL 	 	 7
#define DMON 		 8
#define AMON	    9
#define GRP 	   10
#define AY        11                  /* table to hold file names */

#define ALARMM    12
#define UNIT      13
#define USER_NAME 14
#define ALARMS    15
#define WR_TIME   16
#define AR_Y      17

#define COIL_REG  23
#define DIS_INPUT_REG 24
#define INPUT_REG    25
#define MB_REG    26

#define BAC_AV  27
#define BAC_AI  28
#define BAC_AO  29
#define BAC_BO  30

#define PASS    USER_NAME

//#define STAT	11

//#define SYS 	13                    /* no table for sys (yet) */
#define MONSETUP    40

#define SYSTEM_LIST   20         // System List type object
#define GRAPHIC_GRP 22
#define STATION     24
#define CSTATION    25
#define AYVALUE     29

#define ARRAY1       30
#define AMON_INPUTS  33
#define CONNECT_COM  34
#define NET_SEL      35
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********   code begin  *************
**************************************/
#define NETSTAT	     36
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********    code end   *************
**************************************/
#define NETSTATUS   37         

#define DOUT 	0
#define DIN  	1
#define DVAR 	2
#define DWR 	3
#define DAR 	4
#define DPRG 	5
#define DAMON	6
#define DGRP 	7
#define DAY    8


#define MAN    1
#define AUTO   0
#define DIG1 100

#define YES    1
#define NO     0

#define VIRTUAL_ALARM 0
#define TEMPERATURE   1
#define GENERAL_ALARM 2
#define PRINTER_ALARM 10

#define ALARM_VERYLOW 1
#define ALARM_LOW     2
#define ALARM_HI      3
#define ALARM_VERYHI  4

#define PGDN   81
#define PGUP   73
#define RIGHT  77
#define LEFT   75
#define HOME   71
#define ENDKEY 79
#define UP     72
#define DOWN   80
#define INSERT 82
#define DELETE 83
#define ENTER  13

#define F1KEY  59
#define F2KEY  60
#define F3KEY  61
#define F4KEY  62
#define F5KEY  63
#define F6KEY  64
#define F7KEY  65
#define F8KEY  66
#define F9KEY  67
#define F10KEY 68
#define F11KEY 69
#define F12KEY 70

#define CTRL   0x04
#define ALT    0x08

#define MODE_TEXT    1
#define MODE_GRAPHIC 0

#define ASCII_FONT 1
#define EGA_FONT   0

#define INDIVIDUAL -1

#define R_READ_ONLY   0x00
#define R_READ_WRITE  0x01
#define R_NA          0x02
#define R_IND         0x03

#define CNF_BOARD_INFO   0x01
#define CNF_COMM			   0x02
#define CNF_MODEM_INFO   0x04
#define CNF_PATH_INFO    0x08
#define CNF_CONSOLE_INFO 0x10
#define CNF_PASS_INFO    0x20
#define CNF_STATION_INFO 0x40
#define CNF_IORATE			 0x0080

#define CNF_ALL          0x7FFF


#define MAXNETWORKPOINTS  30
#define MAXREMOTEPOINTS   30
#define MAXREMOTEPOINTS82 256    // 82
#define MAXREMOTEPOINTS82_FRAME 82

#define MAX_NEST_GRP 10
#define MAX_FILE_TRANSFER_BUF 10002
#define MAX_BLOCKMON_MINI 160
#define MAX_ANALOG_SAMPLES_PER_BLOCK  140
#define MAX_DIGITAL_SAMPLES_PER_BLOCK 112

#define TIMEOUT_NETCALL 20
#define byte  unsigned char
#define word  unsigned int
#define ulong unsigned long

typedef enum { COMPARE, LENGTH, LENGTH_POINT, DESCRIPTOR_POINT,
													ALL_DESCRIPTORS }		Search_type;

typedef enum { REMOTE_ERROR, REMOTE_OK, WAITING_FOR_HANG_UP } REMOTE_STATUS;

typedef enum {
		 READOUTPUT_T3000          = OUT+1,  // read outputs
		 READINPUT_T3000           = IN+1,   // read inputs
		 READVARIABLE_T3000        = VAR+1,        // read variables
		 READCONTROLLER_T3000      = CON+1,        // read controllers
		 READWEEKLYROUTINE_T3000   = WR+1,         // read weekly routines
		 READANNUALROUTINE_T3000   = AR+1,         // read annual routines
		 READPROGRAM_T3000         = PRG+1,        // read programs
		 READTABLE_T3000           = TBL+1,        // read tables
		 READMONITOR_T3000         = AMON+1,       // read monitors
		 READSCREEN_T3000          = GRP+1,        // read screens
		 READARRAY_T3000           = AY+1,         // read arrays
		 READARRAYVALUE_T3000      = AYVALUE+1,    // read array elements
		 READTIMESCHEDULE_T3000    = WR_TIME+1,    // read time schedule
		 READANNUALSCHEDULE_T3000  = AR_Y+1,       // read annual schedule
		 READPROGRAMCODE_T3000     = 16,           // read program code
		 READGROUPELEMENTS_T3000   = 19,           // read group elements
		 READPOINTINFOTABLE_T3000  = 24,           // read pointinfo table
		 UPDATEMEMMONITOR_T3000    = 23,           // read monitor updates
		 READMONITORDATA_T3000     = 22,           // read monitor data
		 READINDIVIDUALPOINT_T3000 = 20,           // read individual point
		 READGROUPELEMENT_T3000    = 25,           // read point info
		 TIME_COMMAND              = 21,           // read time
		 CLEARPANEL_T3000          = 28,           // clear panel
		 SEND_ALARM_COMMAND        = 32,
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********   code begin  *************
**************************************/
		 READNETSTAT_T3000			   = 40,
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********    code end   *************
**************************************/

		 WRITEOUTPUT_T3000         = 100+OUT+1,  // write outputs
		 WRITEINPUT_T3000          = 100+IN+1,   // write inputs
		 WRITEVARIABLE_T3000       = 100+VAR+1,        // write variables
		 WRITEPID_T3000     = 100+CON+1,        // write controllers
		 WRITESCHEDULE_T3000  = 100+WR+1,         // write weekly routines
		 WRITEHOLIDAY_T3000  = 100+AR+1,         // write annual routines
		 WRITEPROGRAM_T3000        = 100+PRG+1,        // write programs
		 WRITETABLE_T3000          = 100+TBL+1,        // write tables
		 WRITEMONITOR_T3000        = 100+AMON+1,       // write monitors
		 WRITESCREEN_T3000         = 100+GRP+1,        // write screens
		 WRITEARRAY_T3000          = 100+AY+1,         // write arrays
		 WRITETIMESCHEDULE_T3000   = 100+WR_TIME+1,    // write time schedule
		 WRITEANNUALSCHEDULE_T3000 = 100+AR_Y+1,       // write annual schedule
		 WRITEPROGRAMCODE_T3000    = 100+16,           // write program code
		 WRITEINDIVIDUALPOINT_T3000 = 100+READINDIVIDUALPOINT_T3000,  // write individual point
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********   code begin  *************
**************************************/
		 WRITENETSTAT_T3000			   = 100+READNETSTAT_T3000,
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********    code end   *************
**************************************/

//       commands with COMMAND_50
		 COMMAND_50                = 50,
		 READ_COMMAND_50           = 50,
		 WRITE_COMMAND_50          = COMMAND_50 + 100,
		 STATION_LIST_COMMAND      = 21,
		 SAVEPROGRAM_COMMAND       = 30,
		 LOADPROGRAM_COMMAND       = 31,
		 DEFAULT_PRG_COMMAND       = 32,

		 READFILES_COMMAND         = 40,  // read the files of type define in
																					// a previous SETFILETYPE_COMMAND command
																					// from the current path.
																					// The data returned are an array of type
																					//  char [13];
		 READDIRECTORIES_COMMAND   = 41,  // read the directories name from
																					// the current path.
																					// The data returned are of type
																					//  (*char)[13];
		 GETCURRENTPATH_COMMAND    = 42,  // get the current path
																					// The command is a read command:
																					//  command  = READ_COMMAND_50
																					//  arg      = GETCURRENTPATH_COMMAND
		 SETCURRENTPATH_COMMAND    = 43,  // set the current path for the subsequent
																					// read directories and read files command
																					// The command is a write command:
																					//  command  = WRITE_COMMAND_50
																					//  arg      = SETCURRENTPATH_COMMAND
		 SETFILETYPE_COMMAND       = 44,  // set the file type (ex. "*.*", "*.prg").
																					// the next read files command will
																					// return only the files of type
																					// set in a SETFILETYPE_COMMAND command.
																					// The command is a write command:
																					//  command  = WRITE_COMMAND_50
																					//  arg      = SETFILETYPE_COMMAND
		 ALARM_NOTIFY_COMMAND       = 51,
		 SEND_INFO_COMMAND          = 52,
		 SEND_WANTPOINTS_COMMAND    = 72,
		 SEND_NETWORKPOINTS_COMMAND = 73,


		 TABLEPOINTS_COMMAND       = 75,
		 PANEL_INFO1_COMMAND       = 110,
		 PANEL_INFO2_COMMAND       = 111,
		 MINICOMMINFO_COMMAND      = 112,
		 PANELID_COMMAND           = 113,
		 ICON_NAME_TABLE_COMMAND   = 114,
		 WRITEDATAMINI_COMMAND     = 116,
		 SENDCODEMINI_COMMAND      = 117,
		 SENDDATAMINI_COMMAND      = 118,
		 READFLASHSTATUS_COMMAND   = 119,
		 READSTATUSWRITEFLASH_COMMAND = 120,
		 WRITE_TIMECOMMAND       = 121,
		 WRITEPRGFLASH_COMMAND     = 122,
		 OPENSCREEN_COMMAND        = 123,
		 INFODATA_COMMAND          = 124

	  } CommandRequest;

typedef enum {
		property_description=512,
		property_label,
		property_value,
		property_auto_manual,
		property_digital_analog,
		property_range,
		property_control,
		property_hw_switch_status,
		property_decom,
		property_m_del_low,
		property_s_del_high,
		property_delay_timer,
		property_filter,
		property_sen_on,
		property_sen_off,
		property_calibration,
		property_state
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********   code begin  *************
**************************************/
//		property_status,
//		property_set_temp,
//		property_room_temp,
//		property_auto_time,
//		property_sensorid,
//		property_sensor_add
/**************************************
**********  William add  *************
********     11.19.1997  *************
**********    code end   *************
**************************************/
	 } T3000PropertyIdentifier;

#define MAX_COMM_INFO 9
// add new items ONLY after RS485_LINK
typedef enum { SERIAL_LINK=1, MODEM_LINK, RS485_LINK, ASYNCRON_LINK, IPX_LINK,
							 TCPIP_LINK, NETBIOS_LINK, MOUSE_LINK } Media_type;
//typedef enum { SERIAL_LINK=1, MODEM_LINK, RS485_LINK, ASYNCRON_LINK, ETHERNET,
//																			NETBIOS, TCPIP, MOUSE_LINK } Media_type;
#define MOUSE_COMM_ENTRY MAX_COMM_INFO-1

//typedef enum { SERIAL_LINK, MODEM_LINK, RS485_LINK, NETBIOS } Media_type;
//#define MOUSE_COMM RS485_LINK+1

typedef enum { INSTALLED, NOT_INSTALLED } Port_Status;

typedef enum { LOCAL_POINT, LOCAL_NETWORK_POINT, OTHER_NETWORK_POINT } Where_point;

typedef enum { RECEIVE, SEND, INDIRECT, READ_COMMAND, WRITE_COMMAND, TRANSFER_DATA,
					SEND_COMMAND, SEND_DATA, TRANSFER_COMMAND } Command_type;

typedef enum { A='A', B } Connection_port;

typedef enum { T3000, MINI_T3000, STANDARD, MINI_MAIN, SUB_MINI } PanelType;

typedef enum { POINTNET, POINTT3000, POINTLOCAL } DefinePointType;

typedef enum { NOTHING, IN16, OUT16, IN8_OUT8, DIGOUT } CardsType;

typedef enum { SUBA, SUBB } NetType;

typedef enum { _OUTPUT=1, _INPUT, VARIABLE, CONTROLLER, WEEKLY_ROUTINE,
	ANNUAL_ROUTINE, PROGRAM, TABLES, DIGITAL_MONITOR, ANALOG_MONITOR,
	CONTROL_GROUP, STATUS, ALARM, PASSWORD, CODE=16, WR_TIMES,
	AR_DATES, CG_DATA, AMON_DATA=22, DMON_DATA, MINI_DESCRIPTORS,
	UNITS=27, MONITOR=40, USERS=46, DESCRIPTORS, ARRAY
						} Point_type_equate;

typedef enum  {
	unused, degC, degF, FPM, Pa, KPa, psi, in_w, Watts, KW, KWH,
	Volts, KV, Amps, ma, CFM, Sec, Min, Hours, Days, time_unit, ohms,
	procent, RH, ppm, counts,	Open, Close, CFH, GPM, GPH, GAL, CF, BTU, CMH,
	custom1, custom2, custom3, custom4, custom5, custom6, custom7, custom8
	} Analog_units_equate;

typedef enum { UNUSED=100,
	OFF_ON, CLOSED_OPEN, STOP_START, DISABLED_ENABLED,
	NORMAL_ALARM, NORMAL_HIGH, NORMAL_LOW, NO_YES,
	COOL_HEAT, UNOCCUPIED_OCCUPIED, LOW_HIGH,
	ON_OFF , OPEN_CLOSED, START_STOP, ENABLED_DISABLED,
	ALARM_NORMAL, HIGH_NORMAL, LOW_NORMAL, YES_NO,
	HEAT_COOL, OCCUPIED_UNOCCUPIED, HIGH_LOW,
	custom_digital1, custom_digital2, custom_digital3, custom_digital4,
	custom_digital5, custom_digital6, custom_digital7, custom_digital8,
	} Digital_units_equate;

typedef enum { not_used_input, Y3K_40_150DegC, Y3K_40_300DegF, R10K_40_120DegC,
	R10K_40_250DegF, R3K_40_150DegC, R3K_40_300DegF, KM10K_40_120DegC,
	KM10K_40_250DegF, A10K_50_110DegC, A10K_60_200DegF, V0_5, I0_100Amps,
	I0_20ma, I0_20psi, N0_2_32counts, N0_3000FPM_0_5V, P0_100_0_5V,
	P0_100_4_20ma, P0_255p_min, table1, table2, table3, table4,
	table5 } Analog_input_range_equate;

//typedef enum {
//	not_used_input, Y3K_40_150DegC, Y3K_40_300DegF, R10K_40_120DegC,
//	R10K_40_250DegF, R3K_40_150DegC, R3K_40_300DegF, KM10K_40_120DegC,
//	KM10K_40_250DegF, A10K_50_110DegC, A10K_60_200DegF, V0_5, I0_100Amps,
//	I0_20ma, I0_20psi, N0_2_32counts, N0_3000FPM_0_10V, P0_100_0_5V,
//	P0_100_4_20ma/*, P0_255p_min*/, V0_10_IN, table1, table2, table3, table4,
//	table5, HI_spd_count, Frequence
//#if 1 //(defined T36CTA)
//#if 1//(defined T36CTA_REV2)
//	, Humidty, co2, pressureInWc, pressureKpa, pressurePsi, pressureMmHg, pressureInHg,
//	pressureKgcm, pressureAtmos, pressureBar, Reserved1, Reserved2, Reserved3
//#endif
//#endif
//} Analog_input_range_equate;
/*
typedef enum {  off_on = 51, closed_open, stop_start, disabled_enabled,
	normal_alarm, normal_high, normal_low, no_yes, custom_digital_1,
	custom_digital_2, custom_digital_3, custom_digital_4, custom_digital_5,
	custom_digital_6, custom_digital_7, custom_digital_8, cool_heat,
	unoccupied_occupied, low_high, on_off, open_closed, start_stop,
	enabled_disabled, alarm_normal, high_normal, low_normal, yes_no,
	heat_cool, occupied_unoccupied, high_low
	} Digital_input_range_equate;
*/
typedef enum { not_used_output, V0_10, P0_100_Open, P0_20psi, P0_100, P0_100_Close, 
					I_0_20ma } 		Analog_output_range_equate;

typedef enum {
	local_modem_main, subA_modem_main, local_subA_main,
	local_modem_subA, subA_subB_main, subA_modem_subB, local_subA_subB
	} Port_configuration_equate;

typedef enum { SUCCESS, ERROR_UPDATE, ERROR_COMM, ERROR_NET } State;

typedef struct {
	int range;
	char range_text[SIZE_TEXT_RANGE];
	char aunits[SIZE_TEXT_UNITS];
	char dunits[2][12];
	char value;
} range_form;

typedef struct {
	int range;
	char range_text[SIZE_TEXT_RANGE];
	char aunits[SIZE_TEXT_UNITS];
	char value;
} an_range_form;

typedef struct {
	int range;
	char range_text[SIZE_TEXT_RANGE];
	char dunits[2][12];
	char value;
} dig_range_form;

typedef enum { RECEIVE_FRAME_IDLE, RECEIVE_FRAME_HEADER, RECEIVE_FRAME_DATA } ReceiveFrameStatusEnum;

typedef enum { HEADER_FrameType, HEADER_Destination, HEADER_Source,
					HEADER_Length1, HEADER_Length2, HEADER_HeaderCRC
				 } HEADERStateEnum;

typedef struct {
	unsigned ack0received				:1;
	unsigned ack1received				:1;
	unsigned nak0received				:1;
	unsigned nak1received				:1;
	unsigned reception_blocked			:2;
	unsigned transmission_blocked		:1;
	unsigned sending_frame_now			:1;
	unsigned send_done					:1;
	unsigned send_ok						:1;
	unsigned TxSequence_number			:1;
	unsigned RxSequence_number			:1;
	unsigned receive_error				:1;
	}	PTP_COMMUNICATION_flags;

typedef struct {
	  unsigned 					  	SilenceTimer;
	  unsigned 					  	InactivityTimer;   //Timebetweentoken
	  unsigned 					  	HeartbeatTimer;    //timerunMSTP
	  int      					  	EventCount;
	  byte     					  	ReceiveError;
	  PTP_COMMUNICATION_flags 	PTP_comm_status;
	  char                     MSTP_MASTERState;
	  byte     						Preamble1;
	  byte     						Preamble2;
	  byte 	  						HeaderCRC;
	  unsigned 						DataCRC;
	  int      						Length;
	  byte 	  						Destination;
	  byte 	  						Source;
	  byte     						FrameType;
	  byte         	         HEADERState;         //HEADERStateEnum
	  byte  	                  ReceiveFrameStatus;  //ReceiveFrameStatusEnum
	  int      						task;
	  byte                     ring;
	  byte     						connection;
	  byte    			         validint;
	  byte                     mode;
     byte                     physical_connection_state;
     byte                     rings_number;         // answer after number_rings
	  long                     time_modem;
	} PORT_STATUS_variables;

//#include "router.h"

typedef struct {
	unsigned status    :4;
	unsigned pri       :4;
	int  length;
	int  next;
	int  last;
} Header_pool;

class POOL {
 private:
	int length;
	int first;
	int last;
 public:
   char *buf;
   char access;
	POOL(void);
	POOL(char *p, int l);
	void init_pool(char *p, int l);
	char *put(char *p, int l);
	char *next(void);
	void release(void);
	unsigned alloc(int l);
	void free(int offset);
};

typedef struct {
	 long start_address;
	 long size;
	 char logical_number;
	 unsigned char   used:1;
	 unsigned char erased:1;
	 unsigned char  error:1;
	} SectorInfo;

typedef struct {
		unsigned  no_analog;
		unsigned  no_digital;
		unsigned  n_sample[MAX_POINTS_IN_MONITOR];
	} no_sample_monitor;

#endif




