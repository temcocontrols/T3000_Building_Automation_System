#ifndef UD_STR_H
#define UD_STR_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_OUTS	5
#define MAX_INS     5

#define PRIVATE_HEAD_LENGTH 6

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
		 WRITETIMESCHEDULE_T3000   = 100+ENUM_WR_TIME+1,    /* write time schedule  */
		 WRITEANNUALSCHEDULE_T3000 = 100+ENUM_AR_DATA+1,     /* write annual schedule*/
		 WRITEPROGRAMCODE_T3000    = 100+16,           /* write program code    */
		 WRITEINDIVIDUALPOINT_T3000 = 100+READINDIVIDUALPOINT_T3000,  /* write individual point*/

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
		 OPENSCREEN_COMMAND        = 123

} CommandRequest;	  







//const int STR_IN_DESCRIPTION_LENGTH = 21;
//const int STR_IN_LABEL = 9;
//const int STR_OUT_DESCRIPTION_LENGTH = 21;
//const int STR_OUT_LABEL = 9;

#define  STR_IN_DESCRIPTION_LENGTH  21
#define	 STR_IN_LABEL  9
#define  STR_OUT_DESCRIPTION_LENGTH  21
#define  STR_OUT_LABEL  9

#define STR_PROGRAM_DESCRIPTION_LENGTH 21
#define  STR_PROGRAM_LABEL_LENGTH 9

#pragma pack(push) //�������״̬ 
#pragma pack(1)
typedef struct
{
	int8_t description[STR_OUT_DESCRIPTION_LENGTH]; 	       /* (21 bytes; string)*/
	int8_t label[STR_OUT_LABEL];		       /* (9 bytes; string)*/

	int32_t value;		       /* (4 bytes; int32_t) */

	int8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog;  /* (1 bit; 0=digital, 1=analog)*/
	int8_t access_level;  /* (3 bits; 0-5)*/
	int8_t control ;  /* (1 bit; 0=off, 1=on)*/
	int8_t digital_control;  /* (1 bit)*/
	int8_t decom;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	int8_t range;	/* (1 Byte ; output_range_equate)*/
#if 0
	unsigned auto_manual : 1;  /* (1 bit; 0=auto, 1=manual)*/
	unsigned digital_analog	: 1;  /* (1 bit; 0=digital, 1=analog)*/
	unsigned access_level	  : 3;  /* (3 bits; 0-5)*/
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
	 uint8_t		entitysize;

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
{ _dUNUSED=100,
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

	//	Point_T3000 override_1;	     /* (3 bytes; point)*/
	//	Point_T3000 override_2;	     /* (3 bytes; point)*/

} Str_weekly_routine_point; /* 21+9+2+3+3 = 38*/


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
	int8_t description[STR_PROGRAM_DESCRIPTION_LENGTH]; 	      	  /* (21 bytes; string)*/
	int8_t label[STR_PROGRAM_LABEL_LENGTH];			  /* (9 bytes; string)*/  
	uint16_t bytes;		/* (2 bytes; size in bytes of program)*/ 
	uint8_t on_off;	//	      : 1; /* (1 bit; 0=off; 1=on)*/
	uint8_t auto_manual;//	  : 1; /* (1 bit; 0=auto; 1=manual)*/
	uint8_t com_prg;	//	    : 1; /* (6 bits; 0=normal use, 1=com program)*/
	uint8_t errcode;	//      : 5; /* (6 bits; 0=normal end, 1=too int32_t in program)*/
	uint8_t unused;   //      : 8;

} Str_program_point;	  /* 21+9+2+2 = 34 bytes*/
#pragma pack(pop)//�ָ�����״̬ 



#endif