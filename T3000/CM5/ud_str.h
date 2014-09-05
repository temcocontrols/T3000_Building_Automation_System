#ifndef UD_STR_H
#define UD_STR_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_OUTS	5
#define MAX_INS     5

#define PRIVATE_HEAD_LENGTH 7

//#define PRIVATE_MONITOR_HEAD_LENGTH 18
#define PRIVATE_MONITOR_HEAD_LENGTH 20
#ifndef ALARM_MESSAGE_SIZE 
	#define ALARM_MESSAGE_SIZE    58
#endif
typedef enum
{
	ENUM_OUT=0,ENUM_IN, ENUM_VAR, ENUM_CON, ENUM_WRT, ENUM_AR, ENUM_PRG, ENUM_TBL, ENUM_TZ, 
	ENUM_AMON, ENUM_GRP, ENUM_ARRAY, ENUM_ALARMM,ENUM_UNIT, ENUM_USER_NAME, ENUM_ALARM_SET, ENUM_WR_TIME, ENUM_AR_DATA
}Point_type_equate;
//#if 0
//typedef enum
//{
//OUT=0,IN, VAR, CON, WRT, AR, PRG, TBL, TZ, AMON, GRP, ARRAY, ALARMM,UNIT, USER_NAME, ALARM_SET, WR_TIME, AR_DATA
//}Point_type_equate;




enum
{
	INPUT_OK,
	INPUT_NOUSE,
	INPUT_SHORT
};

typedef enum { 
		 READOUTPUT_T3000          = ENUM_OUT+1,  /* read outputs */
		 READINPUT_T3000           = ENUM_IN+1,   /* read inputs  */
		 READVARIABLE_T3000        = ENUM_VAR+1,        /* read variables*/
		 READCONTROLLER_T3000      = ENUM_CON+1,        /* read controllers*/
		 READWEEKLYROUTINE_T3000   = ENUM_WRT+1,         /* read weekly routines*/
		 READANNUALROUTINE_T3000   = ENUM_AR+1,         /* read annual routines*/
		 READPROGRAM_T3000         = ENUM_PRG+1,        /* read programs       */
		 READTABLE_T3000           = ENUM_TBL+1,        /* read tables         */
         READTOTALIZER_T3000       = ENUM_TZ+1,         /* read totalizers     */
		 READMONITOR_T3000         = ENUM_AMON+1,       /* read monitors       */
		 READSCREEN_T3000          = ENUM_GRP+1,        /* read screens        */
		 READARRAY_T3000           = ENUM_ARRAY+1,      /* read arrays         */
		 READALARM_T3000		   = ENUM_ALARMM + 1, /* read alarm */
		 READUNIT_T3000			   = 14,			/*customer defined units * /
//		 READARRAYVALUE_T3000      = AYVALUE+1,    /* read array elements */
		 READTIMESCHEDULE_T3000    = ENUM_WR_TIME+1,    /* read time schedule  */
		 READANNUALSCHEDULE_T3000  = ENUM_AR_DATA+1,    /* read annual schedule*/
		 READPROGRAMCODE_T3000     = 16,           /* read program code   */
		 READGROUPELEMENTS_T3000   = 19,           /* read group elements */
		 READPOINTINFOTABLE_T3000  = 24,           /* read pointinfo table*/
		 UPDATEMEMMONITOR_T3000    = 23,           /* read monitor updates*/
		 READMONITORDATA_T3000     = 22,           /* read monitor data   */
		 READINDIVIDUALPOINT_T3000 = 20,           /* read individual point*/
		 READGROUPELEMENT_T3000    = 25,           /* read point info      */
		 TIME_COMMAND              = 21,           /* read time            */
		 CLEARPANEL_T3000          = 28,           /* clear panel          */
		 SEND_ALARM_COMMAND        = 32,
		 READTSTAT_T3000		   = 33,

		 WRITEOUTPUT_T3000         = 100+ENUM_OUT+1,  /* write outputs          */
		 WRITEINPUT_T3000          = 100+ENUM_IN+1,   /* write inputs           */
		 WRITEVARIABLE_T3000       = 100+ENUM_VAR+1,        /* write variables  */
		 WRITECONTROLLER_T3000     = 100+ENUM_CON+1,        /* write controllers*/
		 WRITEWEEKLYROUTINE_T3000  = 100+ENUM_WRT+1,         /* write weekly routines*/
		 WRITEANNUALROUTINE_T3000  = 100+ENUM_AR+1,         /* write annual routines*/
		 WRITEPROGRAM_T3000        = 100+ENUM_PRG+1,        /* write programs       */
		 WRITETABLE_T3000          = 100+ENUM_TBL+1,        /* write tables         */
     	 WRITETOTALIZER_T3000      = 100+ENUM_TZ+1,         /* write totalizers     */
		 WRITEMONITOR_T3000        = 100+ENUM_AMON+1,       /* write monitors       */
		 WRITESCREEN_T3000         = 100+ENUM_GRP+1,        /* write screens        */
		 WRITEARRAY_T3000          = 100+ENUM_ARRAY+1,      /* write arrays         */
		  WRITEALARM_T3000		   = 100+ ENUM_ALARMM + 1, /* write alarm */
		  WRITEUNIT_T3000          = 114,                 /* write customer units*/
		 WRITETIMESCHEDULE_T3000   = 100+ENUM_WR_TIME+1,    /* write time schedule  */
		 WRITEANNUALSCHEDULE_T3000 = 100+ENUM_AR_DATA+1,     /* write annual schedule*/
		 WRITEPROGRAMCODE_T3000    = 100+16,           /* write program code    */
		 WRITEINDIVIDUALPOINT_T3000 = 100+READINDIVIDUALPOINT_T3000,  /* write individual point*/
		 WRITETSTAT_T3000			= 100 + READTSTAT_T3000,
		 COMMAND_50                = 50,
		 READ_COMMAND_50           = 50,
		 WRITE_COMMAND_50          = 150,
		 STATION_LIST_COMMAND      = 21,
		 SAVEPROGRAM_COMMAND       = 30,
		 LOADPROGRAM_COMMAND       = 31,
		 DEFAULT_PRG_COMMAND       = 32, 
		 READFILES_COMMAND         = 40,  /* read the files of type define in*/
																					/* a previous SETFILETYPE_COMMAND command*/
																					/* from the current path.                */
																					/* The data returned are an array of type*/
																					/*  S8_T [13];                           */
		 READDIRECTORIES_COMMAND   = 41,  /* read the directories name from            */
																					/* the current path.                     */
																					/* The data returned are of type         */
																					/*  (*S8_T)[13];                         */
		 GETCURRENTPATH_COMMAND    = 42,  /* get the current path                      */
																					/* The command is a read command:        */
																					/*  command  = READ_COMMAND_50           */
																					/*  arg      = GETCURRENTPATH_COMMAND    */
		 SETCURRENTPATH_COMMAND    = 43,  /* set the current path for the subsequent   */
																					/* read directories and read files command*/
																					/* The command is a write command:        */
																					/*  command  = WRITE_COMMAND_50           */
																					/*  arg      = SETCURRENTPATH_COMMAND     */
		 SETFILETYPE_COMMAND       = 44,  /* set the file type (ex. "*.*", "*.prg").    */
																					/* the next read files command will       */
																					/* return only the files of type          */
																					/* set in a SETFILETYPE_COMMAND command.  */
																					/* The command is a write command:        */
																					/*  command  = WRITE_COMMAND_50           */
																					/*  arg      = SETFILETYPE_COMMAND        */
		 ALARM_NOTIFY_COMMAND       = 51,
		 SEND_INFO_COMMAND          = 52,
		 SEND_WANTPOINTS_COMMAND    = 72,
		 SEND_NETWORKPOINTS_COMMAND = 73,


		 TABLEPOINTS_COMMAND       = 75,
		 READ_REMOTE_DEVICE_DB		= 97,
		 READ_SETTING_COMMAND		= 98,
		  GETSERIALNUMBERINFO       = 99,
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
		 RESTARTMINI_COMMAND       = 121,
		 WRITEPRGFLASH_COMMAND     = 122,
		 OPENSCREEN_COMMAND        = 123,
		 WRITE_SETTING_COMMAND		= 198,
		 WRITE_SUB_ID_BY_HAND = 199
} CommandRequest;	  







//const int STR_IN_DESCRIPTION_LENGTH = 21;
//const int STR_IN_LABEL = 9;
//const int STR_OUT_DESCRIPTION_LENGTH = 21;
//const int STR_OUT_LABEL = 9;

#define  STR_IN_DESCRIPTION_LENGTH  21
#define	 STR_IN_LABEL  9
#define  STR_OUT_DESCRIPTION_LENGTH  21
#define  STR_OUT_LABEL  9

#define STR_VARIABLE_DESCRIPTION_LENGTH  21
#define	 STR_VARIABLE_LABEL  9

#define STR_PROGRAM_DESCRIPTION_LENGTH 21
#define  STR_PROGRAM_LABEL_LENGTH 9

#define STR_WEEKLY_DESCRIPTION_LENGTH 21
#define  STR_WEEKLY_LABEL_LENGTH 9

#define STR_ANNUAL_DESCRIPTION_LENGTH 21
#define  STR_ANNUAL_LABEL_LENGTH 9

#define  STR_SCREEN_DESCRIPTION_LENGTH 21
#define  STR_SCREEN_LABLE_LENGTH 9
#define  STR_SCREEN_PIC_FILE_LENGTH 11

#define STR_MONITOR_LABEL_LENGTH 9

#pragma pack(push) //±£´æ¶ÔÆë×´Ì¬ 
#pragma pack(1)
typedef struct
{
	int8_t description[STR_OUT_DESCRIPTION_LENGTH]; 	       /* (21 bytes; string)*/
	int8_t label[STR_OUT_LABEL];		       /* (9 bytes; string)*/

	int32_t value;		       /* (4 bytes; int32_t) */

	int8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog;  /* (1 bit; 0=digital, 1=analog)*/
	int8_t hw_switch_status;  // SW_OFF-0 SW_HAND-2 SW_AUTO-1 
	int8_t control ;  /* (1 bit; 0=off, 1=on)*/
	int8_t digital_control;  /* (1 bit)*/
	int8_t decom;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	int8_t range;	/* (1 Byte ; output_range_equate)*/
#if 0
	unsigned auto_manual : 1;  /* (1 bit; 0=auto, 1=manual)*/
	unsigned digital_analog	: 1;  /* (1 bit; 0=digital, 1=analog)*/
	unsigned hw_switch_status	  : 3;  /* (3 bits; 0-5)*/
	unsigned control       : 1;  /* (1 bit; 0=off, 1=on)*/
	unsigned digital_control: 1;  /* (1 bit)*/
	unsigned decom	     	: 1;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	unsigned range        : 8;	/* (1 Byte ; output_range_equate)*/
	
#endif
//	uint8_t flag1;
//	uint8_t range  ;	/* (1 uint8_t ; output_range_equate)*/

	uint8_t m_del_low;  /* (1 uint8_t ; if analog then low)*/
	uint8_t s_del_high; /* (1 uint8_t ; if analog then high)*/
	uint16_t delay_timer;      /* (2 bytes;  seconds,minutes)*/

} Str_out_point;  /* 21+9+4+2+2+2 = 40 */


typedef  struct
{

	int8_t description[STR_IN_DESCRIPTION_LENGTH]; 	      /* (21 bytes; string)*/
	int8_t label[STR_IN_LABEL];		      	/* (9 bytes; string)*/

	//int8_t value[4]
	int value;		     						/* (4 bytes; int32_t)*/
	//int8_t value[4];
	int8_t  filter;  /* (3 bits; 0=1,1=2,2=4,3=8,4=16,5=32, 6=64,7=128,)*/
	int8_t decom;/* (1 bit; 0=ok, 1=point decommissioned)*/
	int8_t sen_on;/* (1 bit)*/
	int8_t sen_off;  /* (1 bit)*/
	int8_t control; /*  (1 bit; 0=OFF, 1=ON)*/
	int8_t auto_manual; /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog ; /* (1 bit; 1=analog, 0=digital)*/
	int8_t calibration_sign; /* (1 bit; sign 0=positiv 1=negative )*/
	int8_t calibration_increment; /* (1 bit;  0=0.1, 1=1.0)*/
	int8_t unused; /* (5 bits - spare )*/
#if 0
	unsigned  filter:3;  /* (3 bits; 0=1,1=2,2=4,3=8,4=16,5=32, 6=64,7=128,)*/
	unsigned decom	:1;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	unsigned sen_on :1;  /* (1 bit)*/
	unsigned sen_off:1;  /* (1 bit)*/
	unsigned control:1; /*  (1 bit; 0=OFF, 1=ON)*/
	unsigned auto_manual:1; /* (1 bit; 0=auto, 1=manual)*/
	unsigned digital_analog :1; /* (1 bit; 1=analog, 0=digital)*/
	unsigned calibration_sign :1; /* (1 bit; sign 0=positiv 1=negative )*/
	unsigned calibration_increment:1; /* (1 bit;  0=0.1, 1=1.0)*/
	unsigned unused:5; /* (5 bits - spare )*/
#endif
//	uint8_t flag1;
//	uint8_t flag2;

	uint8_t calibration;  /* (8 bits; -25.6 to 25.6 / -256 to 256 )*/

	uint8_t range;	      			/* (1 uint8_t ; input_range_equate)*/

} Str_in_point; /* 21+1+4+1+1+9 = 38 */

//typedef  struct
//{
//	int8_t label[4];		
//}Str_connected_point;

typedef union {
		Str_out_point             *pout;
		Str_in_point 			  *pin;
	 } Str_points_ptr;


typedef struct
{
	 uint16_t  		total_length;        /*	total length to be received or sent	*/
	 uint8_t		command;
	 uint8_t		point_start_instance;
	 uint8_t		point_end_instance;
	 uint16_t		entitysize;

}Str_user_data_header;


typedef enum 
{ 
	not_used,
	KM_10K,
	I_4_20ma,
	V_0_10,
	V_0_5V,
	V_0_24AC,
	TST_Normal
} Analog_input_new_range_equate;

typedef enum 
{ 
	not_used_output, 
	V0_10, 
	P0_100_Open, 
	P0_20psi, 
	P0_100,
	P0_100_Close, 
	I_0_20ma 
} Analog_output_range_equate;

//#endif


typedef enum  
{
	_unused, degC, degF, FPM, Pa, KPa, psi, in_w, Watts, KW, KWH,
	Volts, KV, Amps, ma, CFM, Sec, Min, Hours, Days, time_unit, ohms,
	procent, RH, ppm, counts, Open, CFH, GPM, GPH, GAL, CF, BTU, CMH,
	custom1, custom2, custom3, custom4, custom5, custom6, custom7, custom8
} Analog_units_equate; 


typedef enum 
{   _dUNUSED=100,
	OFF_ON, CLOSED_OPEN, STOP_START, DISABLED_ENABLED,
	NORMAL_ALARM, NORMAL_HIGH, NORMAL_LOW, NO_YES,
	COOL_HEAT, UNOCCUPIED_OCCUPIED, LOW_HIGH,
	ON_OFF , OPEN_CLOSED, START_STOP, ENABLED_DISABLED,
	ALARM_NORMAL, HIGH_NORMAL, LOW_NORMAL, YES_NO,
	HEAT_COOL, OCCUPIED_UNOCCUPIED, HIGH_LOW,
	custom_digital1, custom_digital2, custom_digital3, custom_digital4,
	custom_digital5, custom_digital6, custom_digital7, custom_digital8
} Digital_units_equate;



typedef struct
{
	int8_t description[21];	      /*  (21 bytes; string)*/
	int8_t label[9];		      /*  (9 bytes; string)*/

	int32_t value;		      /*  (4 bytes; float)*/

	uint8_t auto_manual;  /*  (1 bit; 0=auto, 1=manual)*/
	uint8_t digital_analog;  /*  (1 bit; 1=analog, 0=digital)*/
	uint8_t control	;
	uint8_t unused	;
	uint8_t range ; /*  (1 uint8_t ; variable_range_equate)*/


}	Str_variable_point; /* 21+9+4+1+1 = 36*/



typedef struct {
	int32_t	old_err;
	int32_t 	error_area;
	int32_t 	oi;
}	Con_aux;

//class Point_T3000 {
//public:
//	byte number		;
//	byte point_type;
//	byte panel	 ;
//};

typedef struct {
public:
	byte number		;
	byte point_type;
	byte panel	 ;
}Point_T3000;


typedef struct
{
	int8_t description[21];		     /* (21 bytes; string)*/
	int8_t label[9];		      	     /*	(9 bytes; string)*/

	uint8_t value ;  /* (1 bit; 0=off, 1=on)*/
	uint8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
	uint8_t override_1_value;  /* (1 bit; 0=off, 1=on)*/
	uint8_t override_2_value;  /* (1 bit; 0=off, 1=on)*/
	uint8_t off  ;
	uint8_t unused	; /* (11 bits)*/

	Point_T3000 override_1;	     /* (3 bytes; point)*/
	Point_T3000 override_2;	     /* (3 bytes; point)*/

} Str_weekly_routine_point; /* 21+9+2+3+3 = 38*/

typedef struct  
{
	byte time_minutes;
	byte time_hours;
	//byte time_on_minutes1;		// (1 byte ; 0-59)
	//byte time_on_hours1;		// (1 byte ; 0-23)
	//byte time_off_minutes1;	// (1 byte ; 0-59)
	//byte time_off_hours1;		// (1 byte ; 0-23)
	//byte time_on_minutes2;		// (1 byte ; 0-59)
	//byte time_on_hours2;		// (1 byte ; 0-23)
	//byte time_off_minutes2;	// (1 byte ; 0-59)
	//byte time_off_hours2;		// (1 byte ; 0-23)
	//byte time_on_minutes3;		// (1 byte ; 0-59)
	//byte time_on_hours3;		// (1 byte ; 0-23)
	//byte time_off_minutes3;	// (1 byte ; 0-59)
	//byte time_off_hours3;		// (1 byte ; 0-23)
	//byte time_on_minutes4;		// (1 byte ; 0-59)
	//byte time_on_hours4;		// (1 byte ; 0-23)
	//byte time_off_minutes4;	// (1 byte ; 0-59)
	//byte time_off_hours4;		// (1 byte ; 0-23)
}Day_Time;

typedef struct 		// (size = 16 byte s)
{
	Day_Time Schedual_Day_Time[8][9];
} Str_schedual_time_point;

typedef struct
{
	uint8_t	minutes;		/* (1 byte ; 0-59)	*/
	uint8_t	hours; 		/* (1 byte ; 0-23)	*/

} Time_on_off;				/* (size = 2 bytes)	*/




typedef struct
{
	int8_t description[21]; 	    /* (21 bytes; string)*/
	int8_t label[9];		      		/* (9 bytes; string)*/

	uint8_t value		;  /* (1 bit; 0=off, 1=on)*/
	uint8_t auto_manual;/* (1 bit; 0=auto, 1=manual)*/
	//	unsigned unused				: 14; 	/* ( 12 bits)*/
	uint8_t unused;

}	Str_annual_routine_point;   /* 21+9+2=32 bytes*/

typedef struct
{
	byte  ti_sec;         // 0-59
	byte  ti_min;         // 0-59
	byte  ti_hour;           // 0-23
	byte  dayofmonth;   // 1-31
	byte  dayofweek;        // 0-6 ; 0=Sunday
	byte  month;          // 0-11
	byte  year;           // year - 1900
	WORD   dayofyear;    // 0-365 gmtime
	signed char isdst;


} Time_block_mini;

typedef struct
{
	int8_t description[STR_PROGRAM_DESCRIPTION_LENGTH]; 	      	  /* (21 bytes; string)*/
	int8_t label[STR_PROGRAM_LABEL_LENGTH];			  /* (9 bytes; string)*/  
	uint16_t bytes;		/* (2 bytes; size in bytes of program)*/ 
	uint8_t on_off;	//	      : 1; /* (1 bit; 0=off; 1=on)*/
	uint8_t auto_manual;//	  : 1; /* (1 bit; 0=auto; 1=manual)*/
	uint8_t com_prg;	//	    : 1; /* (6 bits; 0=normal use, 1=com program)*/
	uint8_t errcode;	//      : 5; /* (6 bits; 0=normal end, 1=too int32_t in program)*/
	uint8_t unused;   //      : 8;

} Str_program_point;	  /* 21+9+2+2 = 34 bytes*/


typedef struct
{
	Point_T3000 input;	        /* (3 bytes; point)*/
	int32_t input_value; 	        /* (4 bytes; int32_t)*/
	int32_t value;		              /* (4 bytes; int32_t)*/
	Point_T3000 setpoint;	      /* (3 bytes; point)*/
	int32_t setpoint_value;	      /* (4 bytes; float)*/
	uint8_t units;    /* (1 uint8_t ; Analog_units_equate)*/

	uint8_t auto_manual; /* (1 bit; 0=auto, 1=manual)*/
	uint8_t action; /* (1 bit; 0=direct, 1=reverse)*/
	uint8_t repeats_per_min; /* (1 bit; 0=repeats/hour,1=repeats/min)*/
	uint8_t unused; /* (1 bit)*/
	uint8_t prop_high; /* (4 bits; high 4 bits of proportional bad)*/

	uint8_t proportional;

	uint8_t reset;	      /* (1 uint8_t ; 0-255)*/
	uint8_t bias;	      /* (1 uint8_t ; 0-100)*/
	uint8_t rate;	      /* (1 uint8_t ; 0-2.00)*/

}	Str_controller_point; /* 3+4+4+3+4+1+1+4 = 24*/


typedef struct
{
	int8_t description[21];				/* (21 bytes; string)	*/
	int8_t label[9];							/* (9 bytes; string)	*/
	int8_t picture_file[11];			/* (11 bytes; string)	*/

	uint8_t update;                /* refresh time */
	uint8_t  mode     ;// :1;     /* text / graphic */
	uint8_t  xcur_grp	;//:15;

	uint16_t  ycur_grp;

} Control_group_point;				/* (size = 46 bytes)	*/

#ifndef MAX_POINTS_IN_MONITOR
#define MAX_POINTS_IN_MONITOR 14
#endif

#ifndef MAX_ANALOG_SAMPLES_PER_BLOCK
#define MAX_ANALOG_SAMPLES_PER_BLOCK  140
#endif

#ifndef MAX_DIGITAL_SAMPLES_PER_BLOCK
#define MAX_DIGITAL_SAMPLES_PER_BLOCK 112
#endif
/* Point_Net_T3000;*/
typedef struct
{
	byte number		;
	byte point_type;
	byte panel		;
	unsigned short  network;
}Point_Net ;


typedef struct
{
	int8_t label[9];		      	  					/* 9 bytes; string */

	Point_Net 	inputs[MAX_POINTS_IN_MONITOR];	/* 70 bytes; array of Point_Net */
	uint8_t				range[MAX_POINTS_IN_MONITOR]; /* 14 bytes */

	uint8_t second_interval_time; 				/* 1 byte ; 0-59 */
	uint8_t minute_interval_time; 				/* 1 byte ; 0-59 */
	uint8_t hour_interval_time;   				/* 1 byte ; 0-255 */

	uint8_t max_time_length;      /* the length of the monitor in time units */

//	Views views[MAX_VIEWS];			/* 16 x MAX_VIEWS bytes */

	uint8_t num_inputs  ;// :4; 	/* total number of points */
	uint8_t an_inputs ;//   :4; 	/* number of analog points */
	uint8_t unit 		;//		:2; 	/* 2 bits - minutes=0, hours=1, days=2	*/
//	uint8_t ind_views	;//	:2; 	/* number of views */
	uint8_t wrap_flag	;//	:1;		/* (1 bit ; 0=no wrap, 1=data wrapped)*/
	uint8_t status		;//		:1;		/* monitor status 0=OFF / 1=ON */
	uint8_t reset_flag	;//	:1; 	/* 1 bit; 0=no reset, 1=reset	*/
	uint8_t double_flag;//	:1; 	/* 1 bit; 0= 4 bytes data, 1= 1(2) bytes data */

}	Str_monitor_point; 		/* 9+70+14+3+1+48+2 = 133 bytes */

typedef struct              /* 5 bytes */
{
	uint8_t pointno_and_value;    // bit0-bit6 point_no     value bit7
	// uint8_t unused ;
	// uint8_t value;

	int32_t          time;

} Digital_sample;        /* 5 bytes */


typedef struct              /* 645 bytes */
{
	Point_Net     inputs[MAX_POINTS_IN_MONITOR]; /* 70 bytes; array of Point_Net */

	uint8_t monitor;//	       :4; /* monitors' number */
	uint8_t no_points  ;//     :4; /* number of points in block */

	uint8_t second_interval_time; /* 1 uint8_t ; 0-59 */
	uint8_t minute_interval_time; /* 1 uint8_t ; 0-59 */
	uint8_t hour_interval_time;   /* 1 uint8_t ; 0-255 */

	uint8_t priority	 ;//     :2; /* 0-block empty, 1-low, 2-medium, 3-high */
	uint8_t first_block  ;//   :1; /* 1 - this block is the first in the chain */
	uint8_t last_block  ;//    :1; /* 1 - this block is the last in the chain */
	uint8_t analog_digital;//  :1; /* 0 - analog, 1 - digital */
	uint8_t block_state ;//    :1; /* 0 = unused, 1 = used */
	uint8_t fast_sampling;//   :1; /* 0 = normal sampling 1 = fast sampling */
	uint8_t wrap_around ;//    :1; /* 1 - wrapped  */

	uint16_t     send_lenght       ;//    :  2;
	uint8_t     no_used[6];


	uint32_t          start_time;

	uint16_t          index;      /* pointer to the new free location in block */
														/* equal with the number of samples in block */
	uint8_t          next_block; /* pointer to the next block in chain
	                              255 = last block in chain */
	uint8_t          block_no;      /* position of block in chain */
  	uint8_t      last_digital_state ;//: 14;
  	//uint8_t      not_used       ;//    :  2;

	uint8_t     reserved;

  	union {
	int32_t           analog[MAX_ANALOG_SAMPLES_PER_BLOCK];   /* 140*4=560 bytes */
  	Digital_sample digital[MAX_DIGITAL_SAMPLES_PER_BLOCK]; /* 112*5=560 bytes */
  	uint8_t           raw_byte[560];
  	uint16_t           raw_int[280];
  	} dat;

}	Monitor_Block;         /* 645 bytes */



typedef struct              /* 85 bytes */
{
	Point_Net     inputs[MAX_POINTS_IN_MONITOR]; /* 70 bytes; array of Point_Net */

	uint8_t monitor;//	       :4; /* monitors' number */
	uint8_t no_points ;//      :4; /* number of points in block */
/*	unsigned tenths_of_seconds    : 4; /* 4 bits ; 0-15 */
/*	unsigned second_interval_time : 6; /* 6 bits ; 0-59 */
/*	unsigned minute_interval_time : 6; /* 6 bits ; 0-59 */

	uint8_t second_interval_time; /* 1 uint8_t ; 0-59 */
	uint8_t minute_interval_time; /* 1 uint8_t ; 0-59 */
	uint8_t hour_interval_time;   /* 1 uint8_t ; 0-255 */

	uint8_t priority	  ;//     :2; /* 0-block empty, 1-low, 2-medium, 3-high */
	uint8_t first_block  ;//   :1; /* 1 - this block is the first in the chain */
	uint8_t last_block    ;//  :1; /* 1 - this block is the last in the chain */
	uint8_t analog_digital;//  :1; /* 0 - analog, 1 - digital */
	uint8_t block_state ;//    :1; /* 0 = unused, 1 = used */
	uint8_t fast_sampling;//   :1; /* 0 = normal sampling 1 = fast sampling */
	uint8_t wrap_around  ;//   :1; /* 1 - wrapped  */

	uint16_t     send_lenght       ;//    :  2;
	uint8_t     no_used[6];

	int32_t          start_time;

	uint16_t          index;      /* pointer to the new free location in block */
														/* equal with the number of samples in block */
	uint8_t          next_block; /* pointer to the next block in chain
	                              255 = last block in chain */
	uint8_t          block_no;      /* position of block in chain */


  uint8_t      last_digital_state ;//: 14;

  /*  U8_T      index_fast         :  2;*/
  uint8_t     reserved;
 // uint8_t      not_used       ;//    :  2;
/*  unsigned      index_fast         :  2;*/


} Monitor_Block_Header;  /* 100 bytes */


typedef struct {

	long nsize;
	unsigned long oldest_time;
	unsigned long most_recent_time;

} MonitorUpdateData;


typedef struct
{
	uint16_t  		total_length;        /*	total length to be received or sent	*/
	uint8_t		command;
	uint8_t index;
	uint8_t type;
	MonitorUpdateData conm_args;
	uint8_t special;
	uint8_t total_seg;
	uint8_t seg_index;
}Str_Monitor_data_header;

typedef struct 
{
	uint16_t instance;
	uint8_t mac_address[6];
	uint8_t serial_num[4];
	uint8_t modbus_addr;
	uint8_t product_type;
	uint8_t panel_number;
	uint16_t modbus_port;
	uint16_t soft_version;
	uint8_t hardware_version;
	uint8_t noused[1];

}Str_Serial_info;

typedef struct
{
	uint8_t harware_rev;
	uint8_t firmware0_rev_main;
	uint8_t firmware0_rev_sub;
	uint8_t frimware1_rev;    // PIC
	uint8_t frimware2_rev; // C8051
	uint8_t frimware3_rev; // SM5964
	uint8_t bootloader_rev;

	uint8_t no_used[10];
}Str_Pro_Info;




typedef union
{
	uint8_t all[400];
	struct 
	{  
		uint8_t ip_addr[4];
		uint8_t subnet[4];
		uint8_t gate_addr[4];   
		uint8_t mac_addr[6];
		uint8_t tcp_type;   /* 0 -- DHCP, 1-- STATIC */
		uint8_t mini_type;
		uint8_t debug;
		Str_Pro_Info   pro_info;
		uint8_t com0_config;
		uint8_t com1_config;
		uint8_t com2_config;

		uint8_t refresh_flash_timer;
		uint8_t en_plug_n_play;
		uint8_t reset_default;   // write 88
		uint8_t com_baudrate0; 
		uint8_t com_baudrate1; 
		uint8_t com_baudrate2; 
	}reg;
}Str_Setting_Info;

typedef enum
{
	NOUSE,MAIN_MSTP,MAIN_MODBUS,MAIN_PTP,SUB_GSM,MAIN_ZIG,SUB_ZIG,SUB_MODBUS,MAX_COM_TYPE
};

typedef struct
{
	//uint8_t sn[4];
	unsigned int sn;
	uint8_t product_type;
	uint8_t modbus_id;
	uint8_t ip_addr[4];
	uint16_t port;
	uint8_t reserved[10];

}Str_Remote_TstDB;




typedef struct {

	Point_Net point;

	uint8_t  modem  ;//      : 1;
	uint8_t  printer ;//     : 1;
	uint8_t  alarm ;//      : 1;
	uint8_t  restored     ;// : 1;
	uint8_t  acknowledged ;// : 1;
	uint8_t ddelete   ;//   : 1;
	uint8_t  type        ;//  : 2; /* DDDD */
	uint8_t  cond_type   ;//  : 4;
	uint8_t  level       ;//  : 4; /* DDDD */

	int32_t   alarm_time;
	char    alarm_count;
	char     alarm_message[ALARM_MESSAGE_SIZE+1];
	char              none[5];
	uint8_t panel_type   ;//  :4;
	uint8_t dest_panel_type;//:4;
	uint16_t      alarm_id;
	uint8_t              prg;

	uint8_t alarm_panel;   /* (1 byte ; 1-32, panel alarm originated from) */
	uint8_t where1;       /* (1 byte ; panel# to send alarm to, 255 = all) */
	uint8_t where2;       /* (1 byte ; panel# to send alarm to, 0 = none) */
	uint8_t where3;       /* (1 byte ; panel# to send alarm to, 0 = none) */
	uint8_t where4;       /* (1 byte ; panel# to send alarm to, 0 = none) */
	uint8_t where5;       /* (1 byte ; panel# to send alarm to, 0 = none) */

	uint8_t where_state1 ;// :1;       /* (1 byte ; panel# to send alarm to, 255 = all) */
	uint8_t where_state2 ;// :1;       /* (1 byte ; panel# to send alarm to, 255 = all) */
	uint8_t where_state3 ;// :1;       /* (1 byte ; panel# to send alarm to, 255 = all) */
	uint8_t where_state4 ;// :1;       /* (1 byte ; panel# to send alarm to, 255 = all) */
	uint8_t where_state5 ;// :1;       /* (1 byte ; panel# to send alarm to, 255 = all) */
	uint8_t change_flag ;//  :2;
	uint8_t original    ;//  :1;
	uint8_t no         ;//   :8;

} Alarm_point; /* 88 bytes */

typedef struct
{
	Point_T3000 point;
	Point_T3000 point1;
	uint8_t cond1;
	int32_t waylow;
	int32_t low;
	int32_t normal;
	int32_t hi;
	int32_t wayhi;
	uint8_t time;
	uint8_t nrmes;
	uint8_t count;

} Alarm_set_point; /* (size =  bytes); */

typedef struct
{
	uint8_t modem     ;//   :1 ;
	uint8_t printer  ;//    :1 ;
	uint8_t screen   ;//   :1 ;
	uint8_t restored   ;//   :1 ;
	uint8_t acknowledged ;// :1 ;
	int32_t alarm_time ;
	char alarm_count;
	char alarm_message[70];
} Alarm_struct;

typedef struct _SCAN_DATABASE_
{
	uint8_t id;
	int32_t sn;
	uint8_t port;
} SCAN_DB;
typedef  struct
{
	uint8_t product_model;
	uint16_t temperature;
	uint8_t mode;
	uint8_t cool_heat_mode;
	uint16_t setpoint;
	uint16_t cool_setpoint;
	uint16_t heat_setpoint;
	uint8_t occupied; // occupied is 1 bit, 8 BIT is S8_T
	uint8_t output_state;
	uint8_t night_heat_db;
	uint8_t night_cool_db;
	uint8_t night_heat_sp;
	uint8_t night_cool_sp;
	uint8_t over_ride;
	//	uint8_t serial_number[4];
	//	uint8_t address; 
	SCAN_DB tst_db;
	uint8_t type;
}Str_TstatInfo_point;


typedef struct
{
	uint8_t direct;
	char digital_units_off[12];       /*12 bytes; string)*/
	char digital_units_on[12];        /*12 bytes; string)*/

} Str_Units_element;  

typedef struct
{
	unsigned char primitive;
	unsigned short DNET;
	unsigned char DLEN;
	char D_MAC_ADR[6];
	unsigned char dest_LSAP;
	unsigned short SNET;
	unsigned char SLEN;
	char S_MAC_ADR[6];
	unsigned char source_LSAP;

} UNITDATA_PARAMETERS;




#pragma region NewAdd
#define MAXFRAMEBUFFER			      490 

#define MAX_SEND_FRAMES             5

typedef struct {
	uint8_t FrameType;
	uint8_t Destination;
	uint8_t Source;
	unsigned short Length;
	uint8_t HeaderCRC;
	char Buffer[MAXFRAMEBUFFER+2+1];
	unsigned short DataCRC;
	uint8_t being_used ;
	uint8_t locked ;
	uint8_t port ;
	uint8_t reply_flag ;
	uint8_t number ;
	uint8_t delay ;

} FRAME_ENTRY;

typedef struct {

	/* Timers for both protocols */
	unsigned short HeartbeatTimer;
	unsigned short InactivityTimer;
	unsigned short SilenceTimer;
	unsigned short ResponseTimer;     /* only for PTP */
	unsigned short SendFrameTimer;

	/* variables for both protocols */
	uint8_t base_task;
	uint8_t tx_wake_up;
	/*	uint8_t Using_send_frame;*/
	uint8_t Using_send_buffer;

	/* States for PTP */
	uint8_t PTP_reception_state;       /* OS - original station for RS485 */
	uint8_t PTP_transmission_state;    /* NS - next station for RS485     */
	uint8_t PTP_connection_state;      /* sendinfo_flag for RS485         */

	uint8_t Rec_frame_state;

	/* offset 20 */
	uint8_t FrameType;
	uint8_t Destination;
	uint8_t Source;
	unsigned short Length;
	uint8_t HeaderCRC;
	uint8_t PTP_DLE_mask;
	unsigned short DataCRC;
	unsigned short Index;
	uint8_t Rx_work_byte;
	char *DataBuffer;
	//	FRAME_ENTRY *rec_frame;

	/* offset 38 */
	/* PTP related flags */
	uint8_t ack0received;//				    : 1; /* receivedpollformaster for RS485 */
	uint8_t ack1received;//				    : 1; /* newpanelbehind for RS485 */
	uint8_t nack0received;//				  : 1; /* panelOff for RS485 */
	uint8_t nack1received;//				  : 1; /* receivedtoken for RS485 */
	uint8_t transmission_blocked;//		: 1;
	uint8_t TxSequence_number;//			: 1;
	uint8_t RxSequence_number;//			: 1;
	uint8_t reception_blocked;//			: 2; /* wantpointsentry for RS485 */
	/* flags related to both protocols */
	uint8_t rec_frame_ready;//			  : 1;
	uint8_t received_invalid_frame;//	: 1;
	uint8_t received_valid_frame;//		: 1;
	uint8_t receive_frame_free;//		  : 1;
	//	uint8_t send_frame_free;//        : 1;
	//	uint8_t send_frame_locked;//			: 1;
	//	uint8_t sending_frame_now;//			: 1;
	/* MSTP related flags */
	unsigned short     EventCount;

	/* offset 42 */
	uint8_t RetryCount;//             : 3;
	uint8_t FrameCount ;//            : 3;
	uint8_t FirstToken ;//            : 3;
	uint8_t replyflag ;//             : 2;
	uint8_t validint  ;//             : 1;
	uint8_t SoleMaster;//             : 1;
	uint8_t UsedToken ;//             : 1;
	uint8_t action ;//                : 1;
	uint8_t netpointsflag ;//         : 1;

	//	uint8_t ready_for_descriptors;//  : 4;
	uint8_t rec_trigger_sequence ;//  : 4; /* only for PTP */
	/* laststation_connected for RS485 */
	//	uint8_t timeonnet;//              : 3;
	//	uint8_t link_type;//              : 2;
	//	uint8_t port   ;//                : 1;
	//	uint8_t tx_end  ;//               : 1;
	//	uint8_t first_send ;//            : 1;

	//	char     nextpanelisoff;
	//	uint32_t need_info;

}STR_PTP;  


typedef struct
{
	int   network;
	unsigned char  router_address;
	unsigned char  half_router_address;
	unsigned char  status;
}  Reachable_Networks;

typedef struct
{
	unsigned char address;
	int  network;
	Reachable_Networks networks_list;
} Port_Def;

#define   PORT_ACTIVE      0x03
#define   PTP_INSTALLED    0x05
#define   PTP_ACTIVE       0x07


#define MAX_reachable_networks 2
#define MAX_Routing_table      2

/*
// status :   0  - free
//            0x01 Port_installed  ( 0000 0001 )
//            0x03 Port active     ( 0000 0011 )
//            0x05 PTP installed   ( 0000 0101 )
//            0x07 PTP active      ( 0000 0111 )
//            0x11 RS485 installed ( 0001 0001 )
//            0x13 RS485 active    ( 0001 0011 )
//            0x23 ETHERNET        ( 0010 0011 )
*/


typedef enum
{
	DL_INVALID = -1, DL_UNITDATA_REQUEST, DL_UNITDATA_INDICATION,
	DL_UNITDATA_RESPONSE, DL_CONNECT_REQUEST, DL_CONNECT_INDICATION,
	DL_CONNECT_RESPONSE, DL_DISCONNECT_REQUEST, DL_TEST_REQUEST,
	DL_TEST_RESPONSE, DL_CONNECT_ATTEMPT_FAILED
}
DL_Primitives;
typedef enum { N_UNITDATArequest, N_UNITDATAindication } NETWORKPrimitive;


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


//typedef struct {
//	char 		status;
//	struct 	Port Port;
//	void 		*ptr;
//	int      task;
//	PORT_STATUS_variables port_status_vars;
//} ROUTING_TABLE;

typedef struct {
	char 		  status;
	Port_Def  Port;
} Routing_Table;


typedef enum
{
	Who_Is_Router_To_Network, I_Am_Router_To_Network,
	I_Could_Be_Router_To_Network,	Reject_Message_To_Network,
	Router_Busy_To_Network, Router_Available_To_Network,
	Initialize_Routing_Table, Initialize_Routing_Table_Ack,
	Establish_Connection_To_Network, Disconnect_Connection_To_Network,
	I_Am_Router_To_Network_Prop=0x80

} Network_Layer_Message_Type;
typedef enum { Q_BLOCKED, Q_ALMOST_BLOCKED, Q_NOT_BLOCKED } Blocked_States;
typedef enum
{
	HEARTBEAT_XOFF = 0, HEARTBEAT_XON, DATA_0, DATA_1, DATA_ACK_0_XOFF,
	DATA_ACK_1_XOFF, DATA_ACK_0_XON, DATA_ACK_1_XON, DATA_NAK_0_XOFF,
	DATA_NAK_1_XOFF, DATA_NAK_0_XON, DATA_NAK_1_XON, CONNECT_REQUEST,
	CONNECT_RESPONSE, DISCONNECT_REQUEST, DISCONNECT_RESPONSE,
	TEST_REQUEST, TEST_RESPONSE
}	Standard_Frame_Types;
typedef enum
{
	RX_IDLE, RX_PREAMBLE, RX_HEADER, RX_DATA, RX_DATA_CRC
} ReceiveFrameStates;

typedef enum
{
	DISCONNECTED, INBOUND, OUTBOUND, CONNECTED, DISCONNECTING
}
PTP_CON_States;

typedef enum { REC_IDLE, REC_READY, DATA, DATA_ACK, DATA_NAK } PTP_REC_States;

typedef enum { TR_IDLE, TR_PENDING, TR_READY, TR_BLOCKED } PTP_TR_States;

#pragma endregion NewAdd






#pragma pack(pop)//»Ö¸´¶ÔÆë×´Ì¬ 



#endif