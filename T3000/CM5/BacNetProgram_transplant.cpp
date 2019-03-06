// TryProgram.cpp : Defines the entry point for the console application.
// Add by Fance for bacnet programming.

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include <stdint.h>
#include <stdbool.h>

#include "T3000DEF.H"
#include "ROUTER.H"

#include "..\global_define.h"

#pragma warning(disable:4309)
#pragma warning(disable:4305)
#pragma warning(disable:4244)


extern DWORD prg_text_color;
extern DWORD prg_label_color;
extern DWORD prg_command_color;
extern DWORD prg_local_var_color;
extern DWORD prg_function_color;

extern int program_code_length[BAC_PROGRAM_ITEM_COUNT];
extern int program_list_line ;

char *index_stack;
char stack[300];

char local_var_new_name[40][40] = { 0 };

extern char editbuf[25000];//For Bacnet Program use
 static int time_count = 0; //Fance add for wait function.
#define  TRUE 1
#define  FALSE 0

#define min(a,b)    (((a) < (b)) ? (a) : (b))
#include "parser.h"
#define LONGTIMETEST -123456789L
#define MAX_GOTO 100
void parse_exp( float *value );
void parse_exp0( float *value );
void eval_exp(float *value);
int define_var(char *tok, int t,int l,int c, int lstr);
int createlocaltable(int cur);
void putback ( void );
void sntx_err(int err, int err_true );
void parse_exp1( float *value );

void parse_exp2( float *value );
void parse_exp3( float *value );
void parse_exp4( float *value );
void parse_exp5( float *value );
int checklocalvar(char *tok);
int look_up_func( char *s );
int isarray(char *tok);
int varoffsetlast(int cur);
int checkonlyarray(char *tok);
int Encode_Program ( /*GEdit *ppedit*/);
void put_line_num( int line_value );

unsigned int line_array[200][2];
int ind_line_array;

extern int mode_text;
extern int ipxport, rs485port;
int my_network = 0;
char my_panel = 0;
#pragma pack(push) //保存对齐状态 
#pragma pack(1)
typedef struct {
	char *address;
	int str_size;
	int max_points;
	char *name;
} Info_Table;

Info_Table my_info_panel[18];

#define  STR_IN_DESCRIPTION_LENGTH  21
#define	 STR_IN_LABEL  9
#define  STR_OUT_DESCRIPTION_LENGTH  21
#define  STR_OUT_LABEL  9

#define STR_VARIABLE_DESCRIPTION_LENGTH  21
#define	 STR_VARIABLE_LABEL  9

#define STR_PROGRAM_DESCRIPTION_LENGTH 21
#define  STR_PROGRAM_LABEL_LENGTH 9

#if 1
#pragma region Panel

class MyPoint{
public:
	byte number		;
	byte point_type;
};



/* Point_T3000;*/
#if 0
class Point_T3000 {
public:
	byte number		;
	byte point_type;
	byte panel	 ;
public:
	void setzero(void);
	int zero(void);
	int cmp(byte num, byte p_type, byte p);
	void putpoint( byte num, byte p_type, byte p);
	void getpoint( byte *num, byte *p_type, byte *p);
	int operator==(Point_T3000 a);
	void operator=(const Point_T3000 &copy);
};

#endif
typedef struct {
public:
	byte number		;
	byte point_type;
	byte panel	 ;
}Point_T3000;

#if 0
/* Point_Net_T3000;*/
class Point_Net {
public:
	byte number		;
	byte point_type;
	byte panel		;
	int  network;
public:
	void setzero(void);
	int zero(void);
	int cmp(	byte num, byte p_type, byte p, int  net);
	void putpoint(	byte num, byte p_type, byte p, int  net);
	void getpoint(	byte *num, byte *p_type, byte *p, int *net);
	int operator==(Point_Net compare);
	void operator=(const Point_Net &copy);
};

void Point_Net::putpoint( byte num, byte p_type, byte p, int  net)
{
	number = num;
	point_type = p_type;
	panel = p;
	network = net;
}
#endif

typedef  struct
{

	char description[STR_IN_DESCRIPTION_LENGTH]; 	      /* (21 bytes; string)*/
	char label[STR_IN_LABEL];		      	/* (9 bytes; string)*/

	//int8_t value[4]
	int value;		     						/* (4 bytes; int32_t)*/
	//int8_t value[4];
	int8_t  filter;  /* (3 bits; 0=1,1=2,2=4,3=8,4=16,5=32, 6=64,7=128,)*/
	int8_t decom;/* (1 bit; 0=ok, 1=point decommissioned)*/
	int8_t sub_id;/* (1 bit)*/
	int8_t sub_product;  /* (1 bit)*/
	int8_t control; /*  (1 bit; 0=OFF, 1=ON)*/
	int8_t auto_manual; /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog ; /* (1 bit; 1=analog, 0=digital)*/
	int8_t calibration_sign; /* (1 bit; sign 0=positiv 1=negative )*/
	int8_t sub_number; /* (1 bit;  0=0.1, 1=1.0)*/
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


#if 0
typedef struct
{
	char description[21]; 	      /* (21 bytes; string)*/
	char label[9];		      	/* (9 bytes; string)*/

	long value;		     						/* (4 bytes; long)*/

	unsigned filter		      : 3;  /* (3 bits; 0=1,1=2,2=4,3=8,4=16,5=32,6=64,7=128)*/
	unsigned decom		      : 1;  /*	(1 bit; 0=ok, 1=point decommissioned)*/
	unsigned sen_on		      : 1;  /* (1 bit)*/
	unsigned sen_off	      : 1;  /* (1 bit)*/
	unsigned control 	      : 1; /*  (1 bit; 0=OFF, 1=ON)*/
	unsigned auto_manual		: 1;  /* (1 bit; 0=auto, 1=manual)*/
	unsigned digital_analog	: 1;  /* (1 bit; 0=digital, 1=analog)*/
	unsigned calibration_sign         : 1;  /* (0 positive; 1 negativ)*/
	unsigned calibration_increment    : 1;  /* (1 bit;  0=0.1, 1=1.0)*/
	unsigned unused         	       : 5;


	byte  calibration;  /* (8 bits; -256.0 to 256.0 / -25.6 to 25.6 (msb is sign))*/

	byte        range;	      			/* (1 Byte ; input_range_equate)*/

} Str_in_point; /* 21+1+4+1+1+9+1 = 38 */

typedef struct
{
	//fance Point_T3000 input;	      /* (2 bytes; point)*/
	long input_value; 	      /* (4 bytes; float)*/
	long value;		      /* (4 bytes; float)*/
	//fance Point_T3000 setpoint;	      /* (2 bytes; point)*/
	long setpoint_value;	      /* (4 bytes; float)*/
	byte units;    /* (1 Byte ; Analog_units_equate)*/

	unsigned auto_manual	    : 1; /* (1 bit; 0=auto, 1=manual)*/
	unsigned action		    : 1; /* (1 bit; 0=direct, 1=reverse)*/
	unsigned repeats_per_min : 1; /* (1 bit; 0=repeats/hour,1=repeats/min)*/
	unsigned unused		    : 1; /* (1 bit)*/
	unsigned prop_high	    : 4; /* (4 bits; high 4 bits of proportional bad)*/

	unsigned proportional    : 8;   /* (1 Byte ; 0-2000 with prop_high)*/
	byte reset;	      /* (1 Byte ; 0-255)*/
	byte bias;	      /* (1 Byte ; 0-100)*/
	byte rate;	      /* (1 Byte ; 0-2.00)*/
}	Str_controller_point; /* 2+4+4+2+4+1+1+4 = 24*/
#endif

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
#if 0
typedef struct
{
	char label[9];		      	  					/* 9 bytes; string */

	//fancePoint_Net    inputs[MAX_POINTS_IN_MONITOR];	/* 28 bytes; array of Point_T3000 */
	byte			 range[MAX_POINTS_IN_MONITOR]; /* 14 bytes */

	byte second_interval_time; 				/* 1 byte ; 0-59 */
	byte minute_interval_time; 				/* 1 byte ; 0-59 */
	byte hour_interval_time;   				/* 1 byte ; 0-255 */

	byte max_time_length;      /* the length of the monitor in time units */

	//fance Views views[MAX_VIEWS];			/* 16 x MAX_VIEWS bytes */

	unsigned num_inputs     :4; 	/* total number of points */
	unsigned anum_inputs    :4; 	/* number of analog points */
	unsigned unit 				:2; 	/* 2 bits - minutes=0, hours=1, days=2	*/
	unsigned ind_views		:2; 	/* number of views */
	unsigned wrap_flag		:1;		/* (1 bit ; 0=no wrap, 1=data wrapped)*/
	unsigned status			:1;		/* monitor status 0=OFF / 1=ON */
	unsigned reset_flag		:1; 	/* 1 bit; 0=no reset, 1=reset	*/
	unsigned double_flag	   :1; 	/* 1 bit; 0= 4 bytes data, 1= 2 bytes data */

}	Str_monitor_point; 		/* 9+28+14+3+1+48+2 = 147 bytes */
#endif

//typedef struct	//changed by Fance
//{
//	byte number		;
//	byte point_type;
//	byte panel		;
//	unsigned short  network;
//}Point_Net ;
typedef struct
{
	byte number		;
	byte point_type;
	byte panel		;
	byte sub_panel  ;
	byte  network;
}Point_Net ;


typedef struct
{
	char label[9];		      	  					/* 9 bytes; string */

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

typedef struct
{
	char  *data_segment;
	unsigned start   :1;    	          // (1 bit)
	unsigned saved	  :1;    	          // (1 bit)
	unsigned unused  :6;    	          // (6 bits)
	unsigned long next_sample_time;
	unsigned int  head_index;	    // (2 byte s; index to next data entry)
	unsigned long last_sample_time;	    // (4 byte s; time)
	unsigned long last_sample_saved_time;	    // (4 byte s; time)
	unsigned int  start_index_dig[MAX_POINTS_IN_MONITOR];
	unsigned int  end_index_dig[MAX_POINTS_IN_MONITOR];

}	Str_monitor_work_data;

typedef struct
{
	char description[21]; 	    /* (21 bytes; string)*/
	char label[9];		      		/* (9 bytes; string)*/

	uint8_t value		;  /* (1 bit; 0=off, 1=on)*/
	uint8_t auto_manual;/* (1 bit; 0=auto, 1=manual)*/
	//	unsigned unused				: 14; 	/* ( 12 bits)*/
	uint8_t unused;

}	Str_annual_routine_point;   /* 21+9+2=32 bytes*/

typedef struct
{
	char description[21];		     /* (21 bytes; string)*/
	char label[9];		      	     /*	(9 bytes; string)*/

	uint8_t value ;  /* (1 bit; 0=off, 1=on)*/
	uint8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
	uint8_t override_1_value;  /* (1 bit; 0=off, 1=on)*/
	uint8_t override_2_value;  /* (1 bit; 0=off, 1=on)*/
	uint8_t off  ;
	uint8_t unused	; /* (11 bits)*/

	//	Point_T3000 override_1;	     /* (3 bytes; point)*/
	//	Point_T3000 override_2;	     /* (3 bytes; point)*/

} Str_weekly_routine_point; /* 21+9+2+3+3 = 38*/

#if 0
typedef struct
{
	char description[21];		     /* (21 bytes; string)*/
	char label[9];		      	     /*	(9 bytes; string)*/

	unsigned value 			   :1;  /* (1 bit; 0=off, 1=on)*/
	unsigned auto_manual		   :1;  /* (1 bit; 0=auto, 1=manual)*/
	unsigned override_1_value	:1;  /* (1 bit; 0=off, 1=on)*/
	unsigned override_2_value	:1;  /* (1 bit; 0=off, 1=on)*/
	unsigned off               :1;
	unsigned unused			   :11; /* (11 bits)*/

	//fance Point_T3000 override_1;	     /* (2 bytes; point)*/
	//fance Point_T3000 override_2;	     /* (2 bytes; point)*/

}	Str_weekly_routine_point; /* 21+2+2+2+10 = 38*/
#endif
typedef struct 		// (size = 16 byte s)
{
	union {
		struct {
			byte time_on_minutes1;		// (1 byte ; 0-59)
			byte time_on_hours1;		// (1 byte ; 0-23)
			byte time_off_minutes1;	// (1 byte ; 0-59)
			byte time_off_hours1;		// (1 byte ; 0-23)
			byte time_on_minutes2;		// (1 byte ; 0-59)
			byte time_on_hours2;		// (1 byte ; 0-23)
			byte time_off_minutes2;	// (1 byte ; 0-59)
			byte time_off_hours2;		// (1 byte ; 0-23)
			byte time_on_minutes3;		// (1 byte ; 0-59)
			byte time_on_hours3;		// (1 byte ; 0-23)
			byte time_off_minutes3;	// (1 byte ; 0-59)
			byte time_off_hours3;		// (1 byte ; 0-23)
			byte time_on_minutes4;		// (1 byte ; 0-59)
			byte time_on_hours4;		// (1 byte ; 0-23)
			byte time_off_minutes4;	// (1 byte ; 0-59)
			byte time_off_hours4;		// (1 byte ; 0-23)
		} time1;
		char time2[16];
	};
} Wr_one_day;
#if 0
typedef struct
{
	char description[21]; 	    /* (21 bytes; string)*/
	char label[9];		      		/* (9 bytes; string)*/
	unsigned value				   : 1;  /* (1 bit; 0=off, 1=on)*/
	unsigned auto_manual	      : 1;  /* (1 bit; 0=auto, 1=manual)*/
	unsigned unused				: 14; 	/* ( 12 bits)*/
}	Str_annual_routine_point;   /* 21+9+2=32 bytes*/
#endif
typedef struct
{
	char description[STR_PROGRAM_DESCRIPTION_LENGTH]; 	      	  /* (21 bytes; string)*/
	char label[STR_PROGRAM_LABEL_LENGTH];			  /* (9 bytes; string)*/  
	uint16_t bytes;		/* (2 bytes; size in bytes of program)*/ 
	uint8_t on_off;	//	      : 1; /* (1 bit; 0=off; 1=on)*/
	uint8_t auto_manual;//	  : 1; /* (1 bit; 0=auto; 1=manual)*/
	uint8_t com_prg;	//	    : 1; /* (6 bits; 0=normal use, 1=com program)*/
	uint8_t errcode;	//      : 5; /* (6 bits; 0=normal end, 1=too int32_t in program)*/
	uint8_t unused;   //      : 8;

} Str_program_point;	  /* 21+9+2+2 = 34 bytes*/
#if 0
typedef struct
{
	char description[21]; 	        // (21 bytes; string)*/
	char label[9];			           // (9 bytes; string)*/

	unsigned int bytes;		        // (2 bytes; size in bytes of program)*/
	unsigned     on_off      : 1;    // (1 bit; 0=off; 1=on)*/
	unsigned     auto_manual : 1;    // (1 bit; 0=auto; 1=manual)*/
	unsigned     com_prg	    : 1;	  // (1 bit; 0=normal , 1=com program)*/
	unsigned     errcode	    : 5;	  // (1 bit; 0=normal end, 1=too long in program)*/
	byte         unused;                // because of mini's

}	Str_program_point;	  /* 21+9+2+1+1 = 34 bytes*/

typedef struct
{
	char description[21];				/* (21 bytes; string)	*/
	char label[9];							/* (9 bytes; string)	*/
	char picture_file[11];			/* (11 bytes; string)	*/

	byte                 update_time;       /* refresh time */
	unsigned mode			   :1;				/* (1 bit ; 0=text, 1=graphic)	*/
	unsigned state       :1;        // 1 group displayed on screen 
	unsigned xcur_grp    :14;
	int    	             ycur_grp;
} Control_group_point;				/* (size = 46 bytes)	*/
#endif

typedef struct
{
	char description[21];				/* (21 bytes; string)	*/
	char label[9];							/* (9 bytes; string)	*/
	char picture_file[11];			/* (11 bytes; string)	*/

	uint8_t update;                /* refresh time */
	uint8_t  mode     ;// :1;     /* text / graphic */
	uint8_t  xcur_grp	;//:15;

	uint16_t  ycur_grp;

} Control_group_point;				/* (size = 46 bytes)	*/

typedef struct
{
//fance	Point_info 	point_info; 				/* 11 bytes*/

	unsigned 	show_point		: 1;
	unsigned 	icon_name_index: 7;
	unsigned 	nr_element     : 8;

	long        high_limit;
	long        low_limit;

	unsigned 	graphic_y_coordinate	: 10;
	unsigned 	off_low_color			: 4;
	unsigned 	type_icon	  	      : 2;

	unsigned 	graphic_x_coordinate	: 10;
	unsigned 	on_high_color			: 4;
	unsigned 	display_point_name	: 1;
	unsigned 	default_icon			: 1;

	unsigned 	text_x_coordinate    : 7;
	unsigned 	modify               : 1;
	unsigned 	point_absent         : 1;    // 0 - point present, 1 - point absent
	unsigned 	where_point          : 2;
	unsigned 	text_y_coordinate    : 5;

	signed char bkgnd_icon;

	unsigned		xicon                : 10;
	unsigned 	text_place				: 4;
	unsigned 	text_present			: 1;
	unsigned 	icon_present			: 1;

	unsigned		yicon                : 10;
	unsigned 	text_size		      : 2;
	unsigned 	normal_color	      : 4;

}	Str_grp_element; /*8+2+4+4+2+2+1+1+2+2 = 32*/

typedef struct
{
	Str_grp_element	    *ptrgrp;
	int 						nr_elements;
} Control_group_elements;

typedef struct
{
	char hard_name[NAME_SIZE];         // (17 bytes; string)
	char name[NAME_SIZE]; 		       // (17 bytes; string)
	byte number;
	unsigned des_length;
	unsigned descksum;
	byte state;
	byte panel_type;
	int  version;
	byte tbl_bank[MAX_TBL_BANK];
} Station_point;

typedef struct {
	int ind_passwords;
	//fance Password_point	passwords[MAX_PASSW];
} Password_struct;

typedef struct {

	//fance Point_Net point;

	unsigned 	modem     	  : 1;
	unsigned 	printer		    : 1;
	unsigned 	alarm			    : 1;
	unsigned 	restored      : 1;
	unsigned 	acknowledged  : 1;
	unsigned	ddelete		    : 1;
	unsigned  type          : 2; /* DDDD */
	unsigned  cond_type     : 4;
	unsigned  level         : 4; /* DDDD */

	unsigned long 		alarm_time;
	char 			        alarm_count;
	char 			        alarm_message[ALARM_MESSAGE_SIZE+1];
	char              none[5];
	unsigned char panel_type     :4;
	unsigned char dest_panel_type:4;
	unsigned int      alarm_id;
	byte              prg;

	byte alarm_panel;   /* (1 byte ; 1-32, panel alarm originated from)	*/
	byte where1;	      /* (1 byte ; panel# to send alarm to, 255 = all)	*/
	byte where2;	      /* (1 byte ; panel# to send alarm to, 0 = none)	*/
	byte where3;	      /* (1 byte ; panel# to send alarm to, 0 = none)	*/
	byte where4;	      /* (1 byte ; panel# to send alarm to, 0 = none)	*/
	byte where5;	      /* (1 byte ; panel# to send alarm to, 0 = none)	*/
	unsigned where_state1  :1;	      /* (1 byte ; panel# to send alarm to, 255 = all)	*/
	unsigned where_state2  :1;	      /* (1 byte ; panel# to send alarm to, 255 = all)	*/
	unsigned where_state3  :1;	      /* (1 byte ; panel# to send alarm to, 255 = all)	*/
	unsigned where_state4  :1;	      /* (1 byte ; panel# to send alarm to, 255 = all)	*/
	unsigned where_state5  :1;	      /* (1 byte ; panel# to send alarm to, 255 = all)	*/
	unsigned change_flag   :2;
	unsigned original      :1;
	unsigned no            :8;
	//	unsigned line          :11;
	//	byte     no;
} Alarm_point;

typedef struct
{
//fance 	Point_T3000 point;
//fance	Point_T3000 point1;
	unsigned cond1;
	long waylow;
	long low;
	long normal;
	long hi;
	long wayhi;
	unsigned time;
	signed char nrmes;
	unsigned count;
} Alarm_set_point;

typedef struct 		// (size = 11 byte s)
{
	char label[9];		     // (9 byte s; string)
	int  length;  	 // (1 byte ; 0-255)
} Str_array_point;

typedef struct {
	char direct;
	char digital_units_off[12];       //12 bytes; string)
	char digital_units_on[12];        //12 bytes; string)
} Units_element;             // 30 bytes;

// structure for single netstat
typedef struct{
	char description[21];//modify by dina long, Feb.3,1998
	int  sensorid;        // 2 bytes , for hardware number
	byte sensoradd;      // 1 byte: max sensor address 32
//	byte valve;        // 1 byte: 0% - 100%
//	byte cov;          // cov define by user , range 0.1 -- 0.5
	byte version;        // netstat version
	long room_temp;
	long set_temp;
	byte auto_manual;
/*
	unsigned room_temp		:6;
	unsigned auto_manual	:1;
	unsigned occupied		  :1;
	unsigned set_temp		  :5;
	unsigned fanspeed     :2;
	unsigned status			  :1; // 1-heat 0-cool
	unsigned acting_type	:4; // set netstat to act as a dirrerent type
	unsigned change_flag	:1; // 0 - no change, slaver report IM_OK
							            	// 1 - change,  slaver report change
*/
} Str_netstat_point; // 2+ 1+1+1+1+ 1+1+ 1=9
#if 0
typedef struct
{
	char description[21]; 	       /* (21 bytes; string)*/
	char label[9];		       /* (9 bytes; string)*/

	long value;		       /* (4 bytes; long) */

	unsigned auto_manual	   : 1;  /* (1 bit; 0=auto, 1=manual)*/
	unsigned digital_analog : 1;  /* (1 bit; 0=digital, 1=analog)*/
	unsigned hw_switch_status	: 3;  /* (3 bits; 0-5)*/
	unsigned control        : 1;  /* (1 bit; 0=off, 1=on)*/
	unsigned digital_control: 1;  /* (1 bit)*/
	unsigned decom	       	: 1;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	unsigned range          : 8;	/* (1 Byte ; output_range_equate)*/

	byte m_del_low;  /* (1 Byte ; if analog then low)*/
	byte s_del_high; /* (1 Byte ; if analog then high)*/
	unsigned int delay_timer;      /* (2 bytes;  seconds,minutes)*/

} Str_out_point;  /* 21+4+2+2+9 = 40 */
#endif

typedef struct
{
	char description[STR_OUT_DESCRIPTION_LENGTH]; 	       /* (21 bytes; string)*/
	char label[STR_OUT_LABEL];		       /* (9 bytes; string)*/

	int32_t value;		       /* (4 bytes; int32_t) */

	int8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog;  /* (1 bit; 0=digital, 1=analog)*/
	int8_t hw_switch_status;  /* (3 bits; 0-5)*/
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

	uint8_t sub_id;  /* (1 uint8_t ; if analog then low)*/
	uint8_t sub_product; /* (1 uint8_t ; if analog then high)*/
	uint16_t delay_timer;      /* (2 bytes;  seconds,minutes)*/

} Str_out_point;  /* 21+9+4+2+2+2 = 40 */


typedef struct
{
	char description[21];	      /*  (21 bytes; string)*/
	char label[9];		      /*  (9 bytes; string)*/

	int32_t value;		      /*  (4 bytes; float)*/

	uint8_t auto_manual;  /*  (1 bit; 0=auto, 1=manual)*/
	uint8_t digital_analog;  /*  (1 bit; 1=analog, 0=digital)*/
	uint8_t control	;
	uint8_t unused	;
	uint8_t range ; /*  (1 uint8_t ; variable_range_equate)*/


}	Str_variable_point; /* 21+9+4+1+1 = 36*/

#if 0
typedef struct
{
	char description[21];	      /*  (21 bytes; string)*/
	char label[9];		      /*  (9 bytes; string)*/

	long value;		      /*  (4 bytes; float)*/

	unsigned auto_manual	   : 1;  /*  (1 bit; 0=auto, 1=manual)*/
	unsigned digital_analog	: 1;  /*  (1 bit; 0=digital, 1=analog)*/
	unsigned control	      : 1;
	unsigned unused		   : 5;
	unsigned range          : 8; /*  (1 Byte ; variable_range_equate)*/

}	Str_variable_point; /* 21+9+4+2 = 36*/
#endif
class Aio
{
public:
	byte GlPanel, GlPanel_type;
	int  GlNetwork;
	//	Sub_a_b GlNet_type;
	//fance Card_Type						       card_type;
	byte 								       number_of_boards;
	byte								       panel_number;
	byte 								       user_number;
	int								         time;  // scan interval in seconds
	char 								       system_name[22];
	//	char 								master_password[20];
	byte								       table_bank[MAX_TBL_BANK];
	byte								       lock[MAX_INFO_TYPE];
	//fance In_aux							       in_aux[MAX_INS];
	//fance Con_aux							       con_aux[MAX_PIDS];
	Info_Table					       info[MAX_FUNCTION_COUNT+1];
	//	Str_grp_element				group_elements[MAX_ELEM];
	Str_out_point   				   outputs[MAX_OUTS];
	Str_in_point    				   inputs[MAX_INS];
	//	char                       in_control[MAX_INS];
	Str_variable_point			   vars[MAX_VARS];
	//	char                       var_control[MAX_VARS];
	Str_controller_point 		   controllers[MAX_PIDS];
	Str_monitor_point				   analog_mon[MAX_ANALM];
	Str_monitor_work_data      monitor_work_data[MAX_ANALM];
	Str_weekly_routine_point   weekly_routines[MAX_SCHS];
	Wr_one_day                 wr_times[MAX_SCHS][9];
	Str_annual_routine_point	 annual_routines[MAX_HOLS];
	byte                       ar_dates[MAX_HOLS][46];
	Str_program_point	 			   programs[MAX_PRGS];
	byte 			     				     *program_codes[MAX_PRGS];   //  pointer to code
	Control_group_point	 		   control_groups[MAX_GRPS];
	Control_group_elements     control_group_elements[MAX_GRPS];
	Station_point					     local_stations[32];
	Password_struct            passwords;
	Alarm_point 				       alarms[MAX_ALARMS];
	int                        ind_alarms;
	Alarm_set_point 		       alarms_set[MAX_ALARMS_SET];
	int                        ind_alarms_set;
	Str_array_point 				   arrays[MAX_ARRAYS];
	long 							       	 *arrays_data[MAX_ARRAYS];
	char 		            		   *program_code;
	unsigned int        		   code_length;
	Units_element					     units[MAX_UNITS];
	int						             in_data[MAX_OUTS];
	int						             out_data[MAX_INS];
	//	char                       Icon_name_table[MAX_ICON_NAME_TABLE][14];
	Str_netstat_point          netstats[MAX_NETSTATS];


////public:
////	void clearpanel(void);
////	Aio( byte nr_boards);
////	~Aio();
////	//fance Aio_Error Aio_Control( Order comm, byte *data_buffer, byte p_number );
////	void Aio::Config_dacqd_card( void );
////	void	Controller( byte p_number );
////	//	void Aio::setcodeprg(int routine_num);
////	void convert_in();
////	void convert_out();
////	long convert_in_point( byte, byte );
////private:
////	long calibration( byte point );
////	long test_match( Analog_input_range_equate range, uint raw );
////	long test_match_custom( Analog_input_range_equate  range, long raw );
////	uint filter( uint sample, byte point );
////	void init_info_table( void );
	//	void Aio::setcodeprg(int routine_num);
};

class GWDialog;

class Panel:public Aio
{
public:
	char Program_Path[64]   ;
	char Descriptor_Path[64];
	char Basic_Path[64]     ;
	char Monitor_Path[64]   ;
	char Graphics_Path[64]  ;
	char DataBase_Path[64]  ;
	char Default_Program[64];
	//			char panel_name[32][20];

	int qt;
public:


	//			Panel(PanelType type, RS232PortName name, unsigned int baud_rate,
	//	char parity, byte length, byte stop, Connection_port port_conn );
	////Panel();
	////Panel(Panel &copy);
	////void operator=(const Panel &copy);

	////byte	point_type;
	////void select_panel(int);
	////void select_subpanel();
	////void subselect(GWDialog *D);
	//////			void Select(int man,char **argv, char **message);
	////void Select(int man);
	////int  connect_panel_proc(int t=0);
	////void Path();
	////void go_graphic(int t=0);
	////void go_text(void);
	//////			void showpoints(int man, int arg=0);
	//////fance int loadprg(char *file, Panel_info1 *panel_info, unsigned char *tblbank, int local=1, int control=1);
	////int file(int man=-1);
	//////fance int savefile(char *, Panel_info1 *panel_info, unsigned char *tblbank, int local=1);

	////friend void file_path_o1(GWDialog *D);
	////friend void file_path_o2(GWDialog *D);
	////friend void file_path_o3(GWDialog *D);
	////friend void file_path_o4(GWDialog *D);
	////friend void file_path_o5(GWDialog *D);
	////friend void pselect(GWDialog *D);

};





#pragma endregion Panel
#endif

 Panel ptr_panel;
 extern vector <Str_in_point>  m_Input_data;
  extern vector <Str_out_point>  m_Output_data;
  extern vector <Str_program_point>  m_Program_data;
  extern vector <Str_variable_point>  m_Variable_data;
   extern vector <Str_weekly_routine_point>  m_Weekly_data;
    extern vector <Str_annual_routine_point>  m_Annual_data;
	extern vector <Control_group_point> m_screen_data;
	extern vector <Str_controller_point> m_controller_data;

//extern Panel *ptr_panel;
//extern void creategauge(GWindow **gauge, char *buf);
//extern void deletegauge(GWindow **gauge);
//extern void showgauge(GWindow *w, long coef, long ntot);
extern int local_request(int panel, int network);

extern int networkaddress;
//fance extern int Station_NUM;
 int Station_NUM;
//extern unsigned long t;
int DORU_SYSTEM=1;
extern int GetPrgFileNames(void) ;    	/* get the prg names */

//void cod_putint(char *ptr, int j, int type);
int find_var_def(char *var_name);
char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network=0, byte panel=0, int *netpresent=0);
char *ispoint_ex(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network,unsigned char &sub_panel , byte panel=0, int *netpresent=0);
char *ltrim(char *text);
char *rtrim(char *text);
void get_label(void);

char *islabel(char *token,int *num_point,byte *var_type,byte *point_type, int *num_panel);
void prescan2(void) ; // find the location of all functions and global
int prescan1(void) ; // find the location of all functions and global
int define_var(char *token, int t=FLOAT_TYPE,int l=0,int c=0, int lstr=LENGTHSTRING);
void assig_var(char *var_name, int value);
void write_cod(int type,int n_var,int v1, char *var1, float fvar1,
	int v2, char *var2, float fvar2,
	int v3, char *var3, float fvar3);
void get_comment(char *);
int pcodvar(int cod,int v,char *var,float fvar,char *op,int Byte);
char *look_label(int panel, int point_type, int num, int network);
char *look_label_ex(int panel,int sub_panel, int point_type, int num, int network);
void eval_exp(float *) ;
extern int checkpointpresence(int num_point,int point_type,int num_panel,int num_net, int panel, int network);
extern int checkmaxpoints(int num_point,int point_type,int num_panel,int num_net);
extern float atofproc(char *avalue);
extern void parse_atom( float  *value );
extern void parse_exp5( float *value );
extern void parse_exp4( float *value );
extern void parse_exp3( float *value );
extern void parse_exp2( float *value );
extern void parse_exp1( float *value );
extern void parse_exp( float *value );
int parse_then_else();
void parse2_then_else();
bool iswhite(char c);
bool isdelim(char c);
int look_up( char *s );
void write_cod_d(int type,int n_var, int v1, char *var1, float fvar1,
	int v2, char *var2, float fvar2,
	int v3, char *var3, float fvar3);
extern struct remote_local_list  remote_local_list[100];
/*extern*/ int ind_remote_local_list;


//extern int  station_num;
/*extern*/ int local_panel=0;
extern unsigned char tbl_bank[100];

/* This array is used by eval_exp1() */
char relops[8] = { LT , LE , GT , GE, EQ , NE , 0 } ;
//extern char relops[8]  ;
extern char logops[7]  ;
//extern char huge *lin;

/* these were declared as external in the parser */
float ttime;
unsigned long ttimed;
char *prog ;
char *p_buf;
//jmp_buf e_buf ;

FILE *fp;
//char null_buf[10],cod_line[150],label[9]; //Fance
char null_buf[10],cod_line[450],label[9];
//int n_local,n_global;
long pos,vpos;
unsigned line_value;
long long_line_value;
int int_value,cur_scope,cur_index,len_cod_linie;
int ncod;
int error;
//int panel,panel_sub;
int n_var,n_var1,n_var2,for_count,then_else,ind_cod_line,next_then_else,next_else_else;
char eoi=0;
char eol;
char argcall[40];





struct line_table_str {
	 unsigned go_to;
	 int go_to_pos;
	 int for_count;
	 int for_pos;
	 int next_pos;
	 unsigned line;
	 int pos;
	 };
struct line_table_str *line_table;
//int index_line_table;

struct buf_str *buf_v;      // buf[MAX_VAR_LINE];
struct buf_str /*huge*/ buf2[7];

struct  dalarm {
			int line;
			int pos;
			} /*huge*/ dalarm[25];//fance
int  wait[15][2];
int index_wait=0;

struct  go_to_str {
			unsigned line;
			unsigned go_to;
			int pos;
         };

struct  go_to_str *go_to;

int index_go_to;
int index_dalarm;

int index_buf,index_op,type_eval;
int id;
float res;

unsigned *renum;
int ind_renum;
int renumvar;

struct variable_table *vars_table;
//struct variable_table vars_table[MAX_Var];

//int position_table[40][30];
//int index_position_table;

int index_vars_table;
//int lline;	//Fance modify
unsigned short lline;

char outdir[65];
char indir[65];
char infile[65];

struct func_table {
 char func_name[15] ;
 char tok ;
} func_table[] = {
 "ABS",ABS,
 "AVG",AVG,
 "COM1",COM_1,
 "CONPROP", CONPROP,
 "CONRATE", CONRATE,
 "CONRESET", CONRESET,
 "CLEARPORT", CLEARPORT,
 "DOW",DOW,
 "DOM",DOM,
 "DOY",DOY,
 "MOY",MOY,
 "INPUT",INKEYD,
 "INT",_INT,
 "INTERVAL",INTERVAL,
 "LN",LN,
 "LN-1",LN_1 ,
 "LN_1",LN_1 ,
 "MAX",MAX,
 "MIN",MIN,
 "OUTPUT",OUTPUTD,
 "POWER-LOSS",POWER_LOSS,
 "POWER_LOSS",POWER_LOSS,
 "SCANS",SCANS,
 "SQR",SQR,
 "STATUS",_Status,
 "RUNTIME",RUNTIME,
 "TBL",Tbl,
 "TIME",TIME,
 "TIME_FORMAT",TIME_FORMAT,
 "TIME-ON",TIME_ON,
 "TIME_ON",TIME_ON,
 "TIME-OFF",TIME_OFF,
 "TIME_OFF",TIME_OFF,
 "WR-ON",WR_ON,
 "WR_ON",WR_ON,
 "WR-OFF",WR_OFF,
 "WR_OFF",WR_OFF,
 "SENSOR-ON",	SENSOR_ON,
 "SENSOR_ON",	SENSOR_ON,
 "SENSOR-OFF", SENSOR_OFF,
 "SENSOR_OFF", SENSOR_OFF,
 "UNACK",UNACK,
 "USER-A",USER_A,
 "USER_A",USER_A,
 "USER-B",USER_B,
 "USER_B",USER_B,
 "NOT", NOT,
 "SUNDAY", SUN,
 "MONDAY", MON,
 "TUESDAY", TUE,
 "WEDNESDAY", WED,
 "THURSDAY", THU,
 "FRIDAY", FRI,
 "SATURDAY", SAT,
 "JANUARY", JAN,
 "FEBRUARY", FEB,
 "MARCH", MAR,
 "APRIL", APR,
 "MAY", MAY,
 "JUNE", JUN,
 "JULY", JUL,
 "AUGUST", AUG,
 "SEPTEMBER", SEP,
 "OCTOBER", OCT,
 "NOVEMBER", NOV,
 "DECEMBER", DEC,
 "",ENDPRG
 };

struct commands {
 char command[15] ;
 char tok ;
} table[] = { /* Commands must be enetered lower case */
 "ALARM",   Alarm,
 "ALARM-AT",ALARM_AT,
 "ALARM_AT",ALARM_AT,
 "CALL",    CALLB,
 "CLEAR",   CLEARX,
 "CLOSE",   CLOSE,
 "DALARM",  DALARM,
 "DECLARE", DECLARE,
 "DISABLE",	DISABLEX,
 "ENABLE",  ENABLEX,
 "END",     ENDPRG,
 "FOR",     FOR,
 "TO",      TO,
 "STEP",    STEP,
 "GOSUB",	  GOSUB,
 "GOTO",    GOTO,
 "HANGUP",  HANGUP,
 "IF",      IF,
 "IF+",     IFP,
 "IF-",     IFM,
 "THEN",    THEN,
 "ELSE",    ELSE,
 "LET",	    LET,
 "MOD",     MOD,
 "NEXT",    NEXT,
 "ON",      ON,
 "ON-ALARM",    ON_ALARM,
 "ON_ALARM",    ON_ALARM,
 "ON-ERROR",    ON_ERROR,
 "ON_ERROR",    ON_ERROR,
 "OPEN",        OPEN,
 "PHONE",       PHONE,
 "PRINT",       PRINT,
 "PRINT-AT",    PRINT_AT,
 "PRINT_AT",    PRINT_AT,
 "REM",         REM,
 "REMOTE-GET",  REMOTE_GET,
 "REMOTE_GET",  REMOTE_GET,
 "REMOTE-SET",  REMOTE_SET,
 "REMOTE_SET",  REMOTE_SET,
 "RETURN",      RETURN,
 "RUN-MACRO",   RUN_MACRO,
 "RUN_MACRO",   RUN_MACRO,
 "SET-PRINTER", SET_PRINTER,
 "SET_PRINTER", SET_PRINTER,
 "START",       STARTPRG,
 "STOP",        STOP,
 "WAIT",        WAIT,
 "AND",         AND,
 "OR",          OR,
 "XOR",         XOR,
 "DIM",         DIM,
 "INTEGER",     INTEGER_TYPE,
 "STRING",      STRING_TYPE,
 "BYTE",        BYTE_TYPE, 
 "FLOAT",       FLOAT_TYPE,
 "LONG",        LONG_TYPE,
 "" ,           ENDPRG /* marks the end of the table */
 };
/* "if" , IF ,
 "else" , ELSE ,
 "for" , FOR ,
 "do" , DO,
 "while" , WHILE ,
 "char" , CHAR ,
 "int" , _INT ,
 "return" , RETURN ,
 "REM" ,REM,
 "end" , END ,
 "" , END  marks the end of the table */


long coef;
char token[80]  ;
char token_type  ;
unsigned char tok ;

static char *time_table;
int ind_time_table;
char *local_table;
int ind_local_table;

char *code;
int ind_code;

int ret_value ; /* function return value */

extern void get_nl(void);
extern void putback(void) ;
extern void sntx_err(int err, int err_true = 1 );
int load_program(char *p , char *fname) , find_var(char *s) ;
extern int get_token(void) ;

char *look_instr( char cod );
int desvar(void);
int pointtotext(char *buf,Point_Net *point);
int pointtotext(char *buf,Point_T3000 *point);
int	desexpr(void);
void ftoa(float f, int length, int ndec, char *buf);
char *desassembler_program();
int local_request_ex(int panel, int sub_panel, int network);
int local_request(int panel, int network);
int findroutingentry(int port, int network, int &j, int t=1);
int localnetwork(int net);
void  init_info_table( void );
void Init_table_bank();
#pragma pack(pop)//恢复对齐状态 
#pragma warning(disable:4996)


#ifdef Fance_Enable
int _tmain(int argc, _TCHAR* argv[])
{
	init_info_table();
	Init_table_bank();


	//For Test
	_tcscpy_s(ptr_panel.vars[3].label,"VARIAB");
	//ptr_panel.vars[3].label = "VARIAB";


	renumvar = 1;
	Encode_Program();
	desassembler_program();
	return 0;
}
#endif

char *pmes;
//char mycode[1024];
char mycode[2000];
//char editbuf[1024] = {"10 REM IF DAYTIMER > 06:00:00 THEN START COOLMODE\r\n20 REM IF+ AHU10CC THEN START OCCUPIED\r\n21 IF- AHU10CC THEN START OCCUPIED\r\n30 IF TIME-ON( OCCUPIED ) > 00:00:15 THEN STOP OCCUPIED"};

//char editbuf[1024] = {"10 REM Test123456789\r\n20 IF VARIAB > 1 THEN VARIAB = 3"};//Pass
//char editbuf[1024] = {"10 REM 123\r\n20 ENABLE OUT1\r\n30 DISABLE IN2"};//Pass

//char editbuf[1024] = {"10 REM TEST\r\n20 IF VAR1 > 1 THEN VAR2 = 2 ELSE VAR3 = 3\r\n30 VAR4 = VAR5 - 2\r\n40 VAR6 = VAR7 + 2"};
//char editbuf[1024] = {"10 REM TEST\r\n20 IF 1-VAR1 > 1 THEN 1-VAR2 = 2 ELSE 1-VAR3 = 3\r\n30 1-VAR4 = 1-VAR5 - 2\r\n40 1-VAR6 = 1-VAR7 + 2"};

//char editbuf[1024] = {"10 REM Test123456789\r\n20 ENABLE OUT8"};//Pass
//extern char editbuf[25000];
//char editbuf[1024] = {"10 REM TestEnable\r\n20 ENABLE 0-OUT1\r\n30 DISABLE 0-OUT2"};
char * intervaltotext_old(char *textbuf, long seconds , unsigned minutes , unsigned hours, char *c=":");
char mesbuf[1024];
char mycomment[1024] = {"AAAAAAAAAAAA"};
int my_lengthcode = 0;
int Encode_Program ( /*GEdit *ppedit*/)
{
	int /*n,*/i,j,k/*,m*/;

//	char fname[65];//,*p;
	unsigned int Byte;
//	int lx,ly,rx,ry;
	eoi=NL;
	eol=0;
//	t=0;
//	n_local=n_global=0;
	error = -1;
	ncod/*=error*/=n_var=n_var1=for_count=then_else=ind_cod_line=next_then_else=0;//Fance marked error
	next_else_else=ret_value=index_vars_table=lline=index_buf=index_op=type_eval=0;
	index_wait=index_go_to=index_dalarm=ind_renum=0;
	ind_line_array = 0;
//	index_position_table=ind_renum=0;
	id=res=0;
	ind_code=0;

	memset(cod_line,0,sizeof(cod_line));//Fance
	time_count = 0;

	//  local_table[4000] , time_table[300]
	local_table = new char[4000+300+sizeof(struct variable_table)*MAX_Var+
								  sizeof(struct line_table_str)*MAX_LINE+sizeof(struct buf_str)*MAX_VAR_LINE+
								  sizeof(go_to_str)*MAX_GOTO];
	time_table = local_table+4000;
	vars_table = (struct variable_table *)(time_table+300);
	line_table = (struct line_table_str *)(((char *)vars_table)+sizeof(struct variable_table)*MAX_Var);
	buf_v = (struct buf_str *)(((char *)line_table)+sizeof(struct line_table_str)*MAX_LINE);
	go_to = (struct go_to_str *)(((char *)buf_v)+sizeof(struct buf_str)*MAX_VAR_LINE);
	 
	memset(local_table, 0, 4000+300+sizeof(struct variable_table)*MAX_Var+
								  sizeof(struct line_table_str)*MAX_LINE+sizeof(struct buf_str)*MAX_VAR_LINE+
								  sizeof(go_to_str)*MAX_GOTO);
	if( !local_table )
		  return 2;

//	memset(go_to, 0, 20 * sizeof(struct  go_to));
//	memset(time_table, 0, 200);
//	memset(local_table,0,sizeof(local_table));
	ind_local_table = ind_time_table = 0;

	memset(mycode,0,sizeof(mycode));
	code = mycode;
	memset(mesbuf,0,1024);
	pmes = mesbuf;
	p_buf = editbuf; 
	*pmes = 0;

//	 

//	 set the program pointer to the beginning of program buffer
	prog = p_buf;

	for_count = 0;
	if (renumvar)
	{
	  
	 renum=new unsigned [MAX_LINE];
	  
	 if( renum==NULL )
	 {
	  if( local_table )
		{
			 
			delete local_table;
			 
		}
		*pmes = 0;
		return 0;
	 }
	 get_token() ;
	 while( (unsigned char)tok!=FINISHED)  //Fance
	 {
		if(token_type==NL) 	get_token() ;
		if((token_type==NL) || ((unsigned char)tok==FINISHED)) continue;//fance
		renum[ind_renum++] = atoi(token);
		get_nl();
		get_token() ;
	 }
	}
	prog = p_buf;
	long length=1;
	while(  *prog != 0  )
	{
	 if (*prog++ == 13) length++;
	}
//	coef = (float)length / 20.;
	coef = (length*1000L/20000L)*1000L + ((length*1000L)%20000L)*1000L/20000L ;
//fance	//if (renumvar)
	//	creategauge(&gaugep,"Renum");
	//else
	//	creategauge(&gaugep,"Parsing");


	memset(local_table, 0, 4000+300+sizeof(struct variable_table)*MAX_Var+
									sizeof(struct line_table_str)*MAX_LINE+sizeof(struct buf_str)*MAX_VAR_LINE+
									sizeof(go_to_str)*MAX_GOTO);

	prog = p_buf;
int ret =	prescan1() ; // find the location of all functions and global
if(error!=-1)
	return error;
//Fance	//deletegauge(&gaugep);

#if 0
	if (renumvar)
	{
		  
		 delete renum;
		 if( local_table )
			delete local_table;
		  
		 *pmes = 0;
		 return 0;
	}
#endif
	for(i=0;i<lline;i++)
		 if(line_table[i].go_to!=0)
		 {
			 for(k=0;k<index_go_to;k++)
			 {
				for(j=0;j<lline;j++)
					 if(line_table[j].line==go_to[k].go_to)
							break;
				if (j==lline)
				{
						sntx_err(NOT_LINE) ;
						break;
				}
			 }
		 }
#if 0
	if (error || for_count!=0)
	{
		  
		 if( local_table )
			delete local_table;
		  
		 *pmes = 0;
		 return 2;
	}
#endif
	prog = p_buf;
	lline = 0;
	memcpy(code,null_buf,2);
	code += 2;
	for_count=0;
	index_go_to = 0;
//	creategauge(&gaugep,"Generating code");
 	prescan2() ; // find the location of all functions and global
	if(error!=-1)
		return error;
//	deletegauge(&gaugep);
//	cod_putint(pedit->code, ncod, pedit->ptrprg->type);
//fance	memcpy(pedit->code,&ncod,2);
	memcpy(mycode,&ncod,2);

		for(i=0;i<lline;i++)
		{ 
		 if(line_table[i].go_to!=0)
		 {
				for(k=0;k<index_go_to;k++)
				{
					for(j=0;j<lline;j++)
						if((go_to[k].line==i+1) && line_table[j].line==go_to[k].go_to)
							break;
					if (j!=lline)
					{
							Byte=line_table[j].pos-2+2;
//							cod_putint(pedit->code+go_to[k].pos, Byte, pedit->ptrprg->type);
							//FANCE memcpy(pedit->code+go_to[k].pos,&Byte,2);
							memcpy(mycode+go_to[k].pos,&Byte,2);
					}
				}
		 }
		 if(line_table[i].for_pos!=0)
		 {
				for(j=i+1;j<lline;j++)
					 if(line_table[j].for_count==line_table[i].for_count)
							break;
				if (j==lline-1)
					 Byte=ncod+2;
				else
					 Byte=line_table[j+1].pos-2+2;

//				cod_putint(pedit->code+line_table[i].for_pos, Byte, pedit->ptrprg->type);
				//FANCE memcpy(pedit->code+line_table[i].for_pos,&Byte,2);
				memcpy(mycode+line_table[i].for_pos,&Byte,2);
				if(!DORU_SYSTEM)
					Byte=line_table[i].pos-2+5;
				else
					Byte=line_table[i].pos ;
//				cod_putint(pedit->code+line_table[j].next_pos, Byte, pedit->ptrprg->type);
				//fance memcpy(pedit->code+line_table[j].next_pos,&Byte,2);
				memcpy(mycode+line_table[j].next_pos,&Byte,2);
		 }
		}

		if(DORU_SYSTEM)
			Byte=0xfe;
		else
			Byte=0xff;

		if( ncod+2+1 >= PROGRAM_SIZE - 100)
		{
		  
		 if( local_table )
			delete local_table;
		  
		 *pmes = 0;
		 sntx_err(TOTAL_2000_BREAK);	// 2017 - 01 17  杜帆 fix 
		 error = 1;
		 return 3;
		}
		memcpy(code,&Byte,1);
		code += 1;

		if (DORU_SYSTEM)
		{
		 if( (ncod+2)+2+2+ind_local_table+2+ind_time_table >= PROGRAM_SIZE - 100 )
		 {
		   
			if( local_table )
			delete local_table;
			 
		  *pmes = 0;
		  sntx_err(TOTAL_2000_BREAK);	// 2017 - 01 17  杜帆 fix 
		  error = 1;
			return 3;
		 }
		 Byte=0;

		 memcpy(code,&Byte,2);
		 code += 2;
		 Byte =  ind_local_table;

		 memcpy(code,&Byte,2);
		 code += 2;
		 memcpy(code, local_table, Byte);
		 code += Byte;

		 memcpy(code,&ind_time_table,2);
		 memcpy(code+2,time_table,ind_time_table);
		 code = code + 2 + ind_time_table;
		}



		my_lengthcode = code - mycode;
		if(my_lengthcode >= 2000)
		{
			sntx_err(TOTAL_2000_BREAK);   // 2017 - 01 17  杜帆 fix 
			error = 1;
			return 1;
		}
		//program_code_length[program_list_line] = my_lengthcode;
		//program_code_length[program_list_line] = 400;

	//fance pedit->length_code=code-pedit->code;

	for(i=0;i<index_vars_table;i++)
	{
	   if(vars_table[i].panel!=my_panel && vars_table[i].type!=LOCAL_VAR)
	 //fance if(vars_table[i].panel!=pedit->panel && vars_table[i].type!=LOCAL_VAR)
	 {
		for(j=0;j<ind_remote_local_list;j++)
		{
	//Fance		//if(remote_local_list[j].point.network==vars_table[i].network &&
			//	remote_local_list[j].point.panel==vars_table[i].panel-1 &&
			//	remote_local_list[j].point.point_type==vars_table[i].point_type+1 &&
			//	remote_local_list[j].point.number==vars_table[i].num_point-1 &&
			//	remote_local_list[j].index<1000)
			//			{
			//			 remote_local_list[j].index += 1000;
			//			 break;
			//			}
		}
		if(j==ind_remote_local_list)
		{
		//Fance	 //remote_local_list[j].point.network=vars_table[i].network;
			 //remote_local_list[j].point.panel=vars_table[i].panel-1;
			 //remote_local_list[j].point.point_type=vars_table[i].point_type+1;
			 //remote_local_list[j].point.number=vars_table[i].num_point-1;
			 //remote_local_list[j].index=-1;
			 ind_remote_local_list++;
			 if( ind_remote_local_list >= MAXREMOTEPOINTS )
			 {
		     
				if( local_table )
				 delete local_table;
				 
				*pmes = 0;
				return 4;
			 }
		}
	 }
	}

/*
	memcpy(pmes,"Program compiled, 0 errors",26);
	pmes += 26;
	*pmes++ = 0x0d;
*/
	*pmes = 0;

	 
	if( local_table )
			delete local_table;
	 
  *pmes = 0;

	return 0;
}	/* end of main */


char * intervaltotext_old(char *textbuf, long seconds , unsigned minutes , unsigned hours, char *c)
{
	char buf[12], *textbuffer;
	char *separator = c ;
	textbuffer = buf;
	if( seconds < 0 )
	{
		seconds = -seconds;
		strcpy(textbuffer++, "-" ) ;        /* add the '-' */
	}
	if(*c!='-')
	{
		hours += seconds/3600;
		minutes += (unsigned)(seconds%3600)/60;
		seconds = (unsigned)(seconds%3600)%60;
	}
	if( hours < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
	}
	itoa(hours,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/

	if( minutes < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:0#:## */
	}
	itoa(minutes,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/
	if( seconds < 10 ) {
		strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:##:0# */
	}
	itoa(seconds,textbuffer,10)  ;

	if(textbuf) strcpy(textbuf, buf);
	return( buf ) ;
}
/*******************intervaltotext ********************/


int prescan1 (void)
{
	char *p;
	char temp[32] ;
	float value;
	int i,l,c,t; /* when this is zero, we are outside all functions */

	p = prog ;


	get_token() ;
	while( tok!=FINISHED)  
	{
		if (token_type == FINISHED)
				break;
		if (token_type == NL)
			 if ((then_else!=0) && eol==NL )
				 return 1;
			 else
			 {
			  while(token_type==NL) get_token();
			 }
		if ((unsigned char)tok==FINISHED) 
			return 1;

		if (!then_else)
		{
		if( token_type != NUMBER )
				{
				get_nl();
				sntx_err(NOT_LINE);
				get_token() ;
				continue;
				}
//		putback() ; /* restore input stream for further processing
//									 by eval_exp() */
//		eval_exp(&value) ; /* process the expression */
		line_value=atoi(token);       //(int)value;
		line_table[lline].line=(int)line_value;
		lline++;
		if( lline >= MAX_LINE )
			{sntx_err(TOOMANYLINES); break;}
		long_line_value=atol(token);       //(int)value;
		if(long_line_value > 64000)
			{sntx_err(TOOBIG);}
//		showgauge(gaugep, coef, lline);
		if(renumvar)
		{
		 itoa(lline*10,temp,10);
		 putback();
		 memmove(prog+strlen(temp), prog+strlen(token), strlen(prog));
		 memcpy(prog, temp, strlen(temp));
		 prog += strlen(temp);
		}
		get_token() ;
		}
		else
		 then_else++;

		switch ( token_type ) 
		{
		case IDENTIFIER:
						if( *prog == '(' )
						{
							if( !checklocalvar(token) )
							{sntx_err(SYNTAX);get_nl();break;}
						}
						else
						 if (!define_var(token))
							{get_nl();break;}
						get_token();
						if (*token=='[')
						{
						 type_eval=0;
						 parse_exp(&res);
						 get_token();
						 if (*token!=']')
							{sntx_err(SYNTAX);get_nl();break;}
						 get_token();
						}
						else
						{
						 if (*token=='(')
						 {
						  type_eval=0;
						  parse_exp(&res);
						  if(!res)
								{sntx_err(SYNTAX);get_nl();break;}
						  get_token();
						  if ( *token!=')')
							 if (*token!=',')
								{sntx_err(SYNTAX);get_nl();break;}
						  if (*token==',')
						  {
							type_eval=0;
							parse_exp(&res);
							if(!res)
								{sntx_err(SYNTAX);get_nl();break;}
							get_token();
							if (*token!=')')
								{sntx_err(SYNTAX);get_nl();break;}
						  }
						  get_token();
						 }
						}
						if (*token!=EQ)
							{sntx_err(SYNTAX);get_nl();break;}
						type_eval=0;
						parse_exp(&res);
						break;

		case KEYWORD:
			 switch(tok) 
			 {
					case DIM:
						get_token();
						t = FLOAT_TYPE;
					case INTEGER_TYPE:
					case STRING_TYPE:
					case BYTE_TYPE:
					case FLOAT_TYPE:
					case LONG_TYPE:
						l=0;c=0;
						if ( ( token_type==KEYWORD && (tok==INTEGER_TYPE || tok==STRING_TYPE ||
							  tok==BYTE_TYPE || tok==FLOAT_TYPE || tok==LONG_TYPE) )
							  || token_type==IDENTIFIER)
						{
						 if ( token_type!=IDENTIFIER )
						 {
							 t=tok;
							 get_token();
							 if ( token_type!=IDENTIFIER )
								{sntx_err(SYNTAX);get_nl();break;};
						 }
						 else
						 {
							if( token[strlen(token)-1]=='%' ) 
								t = INTEGER_TYPE;
							if( token[strlen(token)-1]=='$' ) 
								t = STRING_TYPE;
						 }
						 strcpy(temp, token);
	//Fance					 //if(t!=STRING_TYPE)
						 //{
						 // if(!isarray(l,c))
							//	{sntx_err(SYNTAX);get_nl();break;};
						 //}
						 //else
						 //{
						 // i = isarray(l,c);
						 // if( i==1 || i==0 )
							//	{sntx_err(SYNTAX);get_nl();break;}
						 // else
						 // { l=0; c=128; }
						 //}

						 if (!define_var(temp,t,l,c))    // define array
							{get_nl();break;}
						}
						else
							 {sntx_err(SYNTAX);get_nl();break;};
						break;
					case PHONE:
					case REM:
						while( (*prog!=13) && *prog)
							++prog ;
						break;
					case PRINT:
						get_token();
						if (*token==';')
							 {sntx_err(SYNTAX);get_nl();break;};

						while (token_type!=NL && tok!=FINISHED && token_type!=KEYWORD )
						{
						if (token_type!=STRING)
							 if (strcmp(strupr(token),"DATE"))
								 if (strcmp(strupr(token),"PTIME"))
									 if (strcmp(strupr(token),"USER_A"))
										 if (strcmp(strupr(token),"USER_B"))
											 if (strcmp(strupr(token),"BEEP"))
													if(!define_var(token))
														 {get_nl();get_token();break;};

						get_token();
						if (*token!=';' && token_type!=NL && then_else==0 )
							 {sntx_err(SYNTAX);get_nl();get_token();break;};
						if (*token!=';' && token_type!=NL && token_type!=',' && token_type!=KEYWORD && then_else!=0 )
							 {sntx_err(SYNTAX);get_nl();get_token();break;};
						if (*token==';')
							get_token();
						}

						if ( token_type!=KEYWORD )
							if (eol==NL ) {--prog;--prog;}
							else --prog;
						else
							putback();
						break;
					case ENDPRG:
//						get_token();
//						if (token_type!=NL && tok!=FINISHED)
//									{sntx_err(SYNTAX);get_nl();break;};
						break;
					case HANGUP:
					case CLEARX:
					case RETURN:
						get_token();
						if (token_type!=NL && tok!=FINISHED)
									{sntx_err(SYNTAX);get_nl();break;};
						break;
					case SET_PRINTER:
						get_token();
						if (*token != 'A' && *token != 'B' && *token != '0'
								&& *token != 'a' && *token != 'b' )
											{sntx_err(SYNTAX);get_nl();break;};
						break;
					case RUN_MACRO:
						get_token();
						if ((token_type == NUMBER) && tok!=FINISHED)
									 {
									 putback();
									 eval_exp(&value);
									 if ((int)value<1 || (int)value>16)
											{sntx_err(SYNTAX);get_nl();break;};
									 }
						else
											{sntx_err(SYNTAX);get_nl();break;};
						break;
					case STARTPRG:
					case STOP:
					case OPEN:
					case CLOSE:
					case DISABLEX:
					case ENABLEX:
						get_token();
						if ((token_type == IDENTIFIER) && tok!=FINISHED)
									{
									 if(!define_var(token))
											{get_nl();break;}
									}
						else
									{sntx_err(SYNTAX);get_nl();break;};
						break;
					case ON:
						type_eval=0;
						parse_exp(&res);
						if (!res)
								break;
						get_token();
						if((token_type==NL) || tok==FINISHED)	{sntx_err(SYNTAX);break;}
						if((tok!=GOTO) && tok!=GOSUB)	{sntx_err(SYNTAX);get_nl();break;}
						get_token();
						if((token_type==NL) || tok==FINISHED)	{sntx_err(SYNTAX);break;}
						while ((token_type != NL) && tok!=FINISHED) {
							if(token_type!=NUMBER) {sntx_err(SYNTAX);get_nl();break;} ;
							if(renumvar)
									{
									 for(int i=0;i<ind_renum;i++)
										 if(renum[i]==(unsigned)atoi(token)) break;
									 itoa((i+1)*10,temp,10);
									 putback();
									 memmove(prog+strlen(temp), prog+strlen(token), strlen(prog));
									 memcpy(prog, temp, strlen(temp));
									 prog += strlen(temp);
									}
							get_token();
							if (token_type == NL || tok==FINISHED) break;
							if(*token!=',') {sntx_err(SYNTAX);get_nl();break;};
							get_token();
						}
						break;
					case ON_ALARM:
					case ON_ERROR:
					case GOSUB:
					case GOTO:
						get_token();
						if(token_type==NL) {sntx_err(NOT_LINE);break;}
						if(token_type!=NUMBER) {sntx_err(NOT_LINE),get_nl();break;}
						else {
									putback() ;
									eval_exp(&value) ;
									if(renumvar)
									{
									 for(i=0;i<ind_renum;i++)
										 if(renum[i]==(unsigned)value) break;
									 itoa((i+1)*10,temp,10);
									 putback();
									 memmove(prog+strlen(temp), prog+strlen(token), strlen(prog));
									 memcpy(prog, temp, strlen(temp));
									 prog += strlen(temp);
									}
									if(index_go_to+1 < MAX_GOTO)
									{
									 line_table[lline-1].go_to=1;
									 go_to[index_go_to].line=lline;
									 go_to[index_go_to++].go_to=value;
									}
									else
									  {sntx_err(TOOMANYGOTO),get_nl();break;}
								 }
						break;
					case Alarm:
						type_eval=0;
						parse_exp(&res);
						if (!res)
								break;
						get_token();
						if (*token!=',')
							{sntx_err(SYNTAX);get_nl();break;}
						parse_exp(&res);
						if (!res)
								break;
						get_token();
						if (*token!=',')
							{sntx_err(SYNTAX);get_nl();break;}
						get_nl();
						break;
					case PRINT_AT:
					case ALARM_AT:
						get_token();
						if(((token_type!=NUMBER) && (token_type!=IDENTIFIER)) || tok==FINISHED)
											 {sntx_err(SYNTAX);get_nl(); break;}
						if((token_type==IDENTIFIER) && (strcmp(strupr(token),"ALL")))
											 {sntx_err(SYNTAX);get_nl();break;} ;
						if(token_type==NUMBER)
						{
							get_token();
							if (token_type!=NL)
								 if(token_type!=NUMBER){sntx_err(SYNTAX);get_nl();break;}
								 else
								 {
									 get_token();
									 if (token_type!=NL)
									 {
										 if(token_type!=NUMBER){sntx_err(SYNTAX);get_nl();break;};
									 }
								 }
						}
						break;
					case CALLB:
						get_token();
						if ((token_type == IDENTIFIER) && tok!=FINISHED)
								 {
								 if(!define_var(token))
										{get_nl();break;}
								 //Fance if (vars_table[cur_index].point_type!=PRG)
								 if(0)
											 {sntx_err(SYNTAX);get_nl();break;} ;
								 }
						else
											 {sntx_err(SYNTAX);get_nl();break;} ;
						get_token();
						if((token_type==NL) || (tok==FINISHED)) break;
						if( *token != EQ )  {sntx_err(SYNTAX);get_nl();break;} ;
							res=0;
							while ((token_type != NL) && tok!=FINISHED)
										 {
											parse_exp(&res);
											if (!res)
													break;
											get_token();
											if((token_type==NL) || (tok==FINISHED)) continue;
											if(*token!=',') {sntx_err(SYNTAX);get_nl();break;} ;
										 }
						break;
					case DALARM:
						type_eval=0;
						parse_exp(&res);
						if (!res)
							break;
						get_token();
						if((token_type==NL) || (tok==FINISHED)) {sntx_err(SYNTAX);break;}
						if( *token != ',' )  {sntx_err(SYNTAX);get_nl();break;} ;
						get_token();
						if((token_type==NL) || (tok==FINISHED)) {sntx_err(SYNTAX);break;}
						if( token_type!=NUMBER ) {sntx_err(SYNTAX);get_nl();break;} ;
						get_token();
						if((token_type==NL) || (tok==FINISHED)) {sntx_err(SYNTAX);break;}
						if( *token != ',' )	 {sntx_err(SYNTAX);get_nl();break;} ;
						get_nl();
						break;
					case DECLARE:
						if (lline != 1) 
						{
//									 fprintf(pmes,"error line : %d  DECLARE must be first line\n",(int)line_value);
										memcpy(pmes,"error line : ",13);
										pmes += 13;
										itoa((int)line_value,pmes,10);
										pmes += strlen(pmes);
										memcpy(pmes," DECLARE must be first line",27);
										pmes += 27;
										*pmes++ = 0x0d;
									 break;
						} ;

						get_token();
						while ((token_type != NL) && tok!=FINISHED)
									 {
											if(token_type!=IDENTIFIER)
													 {sntx_err(SYNTAX);get_nl();break;} ;
											if(!define_var(token))
														{get_nl();break;}
											get_token();
									 }
						break;
					case REMOTE_SET:
					case REMOTE_GET:
						get_token();
						if (tok==FINISHED || token_type==NL)  {sntx_err(SYNTAX);break;}
						if ((token_type != IDENTIFIER)) {sntx_err(SYNTAX);get_nl();break;} ;
						if(!define_var(token))
							{get_nl();break;}
						get_token();
						if (*token!=EQ)	{sntx_err(SYNTAX);get_nl();break;}
						get_token();
						if ((token_type != IDENTIFIER) || tok==FINISHED || token_type==NL)
									 {sntx_err(SYNTAX);get_nl();break;} ;
						if(!define_var(token))
							{get_nl();break;}
						break;

					case FOR:
						get_token();
						if ((token_type != IDENTIFIER) || tok==FINISHED || token_type==NL)
									 {sntx_err(SYNTAX);get_nl();break;} ;
						if(!define_var(token))
									 {get_nl();break;} ;
						if (vars_table[cur_index].type!=LOCAL_VAR)
						{
												memcpy(pmes,"not a local variable in line ",31);
												pmes += 31;
												itoa((int)line_value,pmes,10);
												pmes += strlen(pmes);
												*pmes++ = 0x0d;
//											 fprintf(pmes,"not a local variable in line %d \n",line_value);
												get_nl();break;
						}
						get_token();
						if (*token!=EQ)
							{sntx_err(SYNTAX);get_nl();break;}
						type_eval=0;
						parse_exp(&res);
						if (!res)
							break;
						get_token();
						if (tok!=TO)
							{sntx_err(SYNTAX);get_nl();break;}
						type_eval=0;
						parse_exp(&res);
						if (!res)
							break;
						get_token();
						if (tok==STEP)
						 {
							type_eval=0;
							parse_exp(&res);
							if (!res)
								break;
							get_token();
						 }
						if (token_type!=NL)
										{sntx_err(SYNTAX);get_nl();break;}
						for_count++;
						if (prescan1()!=NEXT)
							 {
								memcpy(pmes,"missing NEXT ",13);
								pmes += 13;
								*pmes++ = 0x0d;
//							 fprintf(pmes," missing NEXT  \n");
							 break;
							 }
						if( error != -1) 	break;
						get_token();
						if ((token_type != IDENTIFIER) || tok==FINISHED || token_type == NL)
								{sntx_err(SYNTAX);get_nl();break;}
						if(!define_var(token))
									 {get_nl();break;}
						break;
					case NEXT:
						if (for_count--<0)
								 {
									error =1;// TRUE;
									memcpy(pmes,"missing FOR ",12);
									pmes += 12;
									*pmes++ = 0x0d;
//									fprintf(pmes," missing FOR \n");
								 }
						return NEXT;
					case ELSE:
						return 1;
					case IF:
					case IFP:
					case IFM:
/*
						if (*prog=='+')
						{
						prog++;
						}
						if (*prog=='-')
						{
						prog++;
						}
*/
						type_eval=0;
						parse_exp(&res);
						if (!res)
							break;
						get_token();
						if (tok!=THEN)
							{sntx_err(SYNTAX);get_nl();break;}
						if (parse_then_else())
								{sntx_err(SYNTAX);get_nl();break;}
						if (tok==ELSE)
							if (parse_then_else())
								{sntx_err(SYNTAX);get_nl();break;}
						break;
					case WAIT:
						get_token();
						if(token_type==NL || tok==FINISHED) {sntx_err(SYNTAX); break;}
						if(token_type == TTIME)
							 {
								get_token();
								if(token_type!=NL && tok!=FINISHED) {sntx_err(SYNTAX);get_nl(); break;}
								break;
							 }
						putback();
						type_eval=0;
						parse_exp(&res);
						break;
				 }
             break;
			 default:
					{sntx_err(SYNTAX);get_nl();break;}
		}
//	if(token_type!=NL)
		get_token();
	}
	return 0;
}

void get_nl()
{
	while( (*prog!=13) && *prog)
		++prog ;
	token[0]='\0';
/*	if (*prog)
		{
		++prog;
		token_type = NL;
		}
	else
		{
		 token_type = DELIMITER;
		 tok = FINISHED;
		}
*/
}

int get_token(void)
{
	int i;
	float time1;
	char *temp ;
	token_type = 0 ; tok = 0 ;
	temp = token ;
	*temp = '\0' ;
	/* skip over white space */
	/* 	fprintf(pmes, "*****GetToken****\n" ) ; */
	while( iswhite( *prog) && *prog) 
	{
		++prog ;
		/* 	fprintf(pmes, "Prog %c\n", *prog ) ; */
	}
	/* this seems to push us beyond the end of the file */
	if (eoi == NL)
	{
		if (*prog== 13  ) 
		{            /* carriage return \r */
			++prog ;
			if (*prog== 10  ) 
			{            /* new line \n */
				++prog ;
				eol = NL;
				return (token_type=NL);
			}
			/* skip over white space */
			while( iswhite( *prog) && *prog ){ ++prog ;}
		}    /* while \r encountered */
	}
	else
		if (*prog == 13 || *prog == eoi ) 
		{            /* carriage return \r */
			++prog ;
			eol=',';
			if (*prog== 10  )             /* new line \n */
			{
				++prog ;
				eol=NL;
			}
			return (token_type=NL);
		}

		/* 	if ( *prog=='\0') { /* end of the file */
		if ( *prog== 0  ) 
		{      /* EOF doesn't work here */
			*token = '\0' ;
			tok = FINISHED ;
			/* 		print_vars();  /* ( "End of file \n" ) ; */
			return ( token_type= DELIMITER ) ;
		}
		if( strchr("{}" , *prog )) { /* block delimiters */
			*temp = *prog ;
			temp++;
			*temp = '\0' ;
			prog++ ;
			/*		print_vars();  /*fprintf(pmes, "Block \n" ) ;*/
			return( token_type = BLOCKX ) ;
		}

		/* look for comments */
		if ( *prog=='/')
			if( *(prog+1) == '*') 
			{ /* we're in a comment */
				prog += 2 ;
				do { /* find the end of the comment */
					while (*prog!='*' && *prog) 
						prog++;
					prog++ ;
				} while ( *prog!='/' && *prog) ;
				prog++;
				/*			print_vars();  /*		fprintf(pmes, "End of a comment \n" ) ; */
			}    /* end of comments */


			if( strchr("!<>=" , *prog)) { /* is or might be a relation operator */
				switch (*prog) 
				{
				case '=':
					prog++ ;
					*temp = EQ ;
					temp++ ;
					*temp = '\0' ;

					break ;
					/*
					case '!': if (*(prog+1) =='=') {
					prog++ ; prog++ ;
					*temp = NE ;
					temp++ ; *temp = NE ; temp++;
					*temp = '\0' ;
					}
					break ;
					*/
				case '<':
					if ((unsigned char)*(prog+1) =='=') {
						prog += 2;
						*temp = LE ;
						temp++ ;
					}
					else
					{
						if ((unsigned char)*(prog+1) =='>')
						{
							prog += 2;
							*temp = NE ;
							temp++ ;
						}
						else
						{
							prog++ ;
							*temp = LT ;
						}
					}
					temp++ ;
					temp = '\0' ;
					break ;

				case '>': if ((unsigned char)*(prog+1) =='=') {
					prog += 2;
					*temp = GE ;
					temp++ ;
						  }
						  else {
							  prog++ ;
							  *temp = GT ;
						  }
						  temp++ ;
						  temp = '\0' ;
						  break ;
				} 		 /* end of the switch statement */
				if (*token) {
					/* 	 print_vars(); */
					return( token_type = DELIMITER) ;
				}
			}        /* end of relaional statement check */


			if ( strchr("+-*^/%&|=;()[]'," , *prog))
			{    /* delimiter*/
                ////if ((strstr(prog, "(")) &&
                ////    (strstr(prog, "(")) &&
                ////    (//判断包含AV AI
                ////    (strstr(prog, "AV") != NULL) ||
                ////        (strstr(prog, "AI") != NULL) ||
                ////        (strstr(prog, "AO") != NULL) ||
                ////        (strstr(prog, "BO") != NULL)
                ////        ))
                ////{
                ////    *temp = *prog;
                ////    prog++;
                ////    temp++;
                ////    *temp = '\0';

                ////}

                    *temp = *prog;
                    prog++;
                    temp++;
                    *temp = '\0';
                    /*		print_vars();  /*		fprintf(pmes, "DELIMITER \n" ) ; */
                    return (token_type = DELIMITER);

			}

			if ( *prog == '"') 
			{    /* quoted string */
				prog++ ;
				while ( *prog!='"' && *prog != '\r' && *prog) 
					*temp++ = *prog++ ;
				if (*prog=='\r') sntx_err(SYNTAX) ;
				prog++;
				*temp = '\0' ;
				/*		print_vars();  /*		fprintf(pmes, "String \n" ) ; */
				return (token_type = STRING ) ;
			}

			if (isdigit(*prog) || (*prog=='.')) 
			{ /* a number */
				while( !isdelim(*prog)) *temp++ = *prog++ ;
				if (*prog==':')
				{
					prog++;
					*temp = '\0';
					ttime = atof(token) * 100;
					ttimed = atol(token) * 3600;
					temp = token;
					*temp = '\0';
					while( !isdelim(*prog)) *temp++ = *prog++ ;
					*temp = '\0';
					time1 = atof(token) * 60;
					ttimed += atol(token) * 60;
					if (*prog==':')
					{
						prog++;
						temp = token;
						*temp = '\0';
						while( !isdelim(*prog)) *temp++ = *prog++ ;
						*temp = '\0';
						time1 += atof(token);
						ttimed += atol(token);
					}
					ttime += time1 / 36;
					return (token_type = TTIME );

				}
				if (*(prog-1)=='e' || *(prog-1)=='E')
				{
					*temp++ = *prog++ ;
					while( !isdelim(*prog)) *temp++ = *prog++ ;
					*temp = '\0' ;
					return (token_type = NUMBER );
				}
				if (*prog=='-')
				{
					*temp++ = *prog++ ;
					while( !isdelim(*prog)) *temp++ = *prog++ ;
					token_type = TEMP ;
				}
				else
				{
					*temp = '\0' ;
					for(i=0;i<(int)strlen(token);i++)
						if((token[i]<'0' || token[i]>'9') && (token[i]!='.' && token[i]!='E' && token[i]!='e'))
							break;
					if (i==strlen(token))
					{
						/*		print_vars();  /*		fprintf(pmes, "NUMBER \n" ) ; */
						return (token_type = NUMBER );
					}
					else
						token_type = TEMP ;
				}
			}

			if( isalpha(*prog)) 
			{ /* var or a command */
				while( !isdelim(*prog)) *temp++ = *prog++ ;
				token_type = TEMP ;
			}


			*temp = '\0' ;

			/* see if string is a command or a variable */
			if( token_type == TEMP ) 
			{
				tok = look_up(token) ; /* convert to internal representation */
				if( tok){ token_type = KEYWORD ;
				/* 		fprintf(pmes, "KEYWORD \n" ) ; */
				}
				else{
					token_type = IDENTIFIER ;
					/* 	 fprintf(pmes, "IDENTIFIER \n" ) ; */
				}
			}     /* end of is string a command a number or a variable */
			{ /* print_vars(); */
				return token_type ;
			}

} 		 /* end of the get_token function  */

#define Srt '\`'
/* return true if c is a space or a tab*/
bool iswhite(char c)
{ if( c == ' ' || c == '\t' || c == Srt)
{  /* fprintf(pmes, "White:%c", c ) ; */

	return 1;}

else
{
	/* fprintf(pmes, "Not white:%c", c ) ; */
	return 0;}
}

/* return true if c is a delimiter */
bool isdelim(char c)
//{ if (strchr( " :!;,+-<>'/*%^=()" , c) || c == 9 || c == '\r' || c == 0 )
//  if (strchr( " :!;,<>'*%^=()[]" , c) || c == 9 || c == '\r' || c == 0 )
{
	if(c==Srt)
	{
	prog++;
	c = *prog;
	}
	if (strchr( " :!;,<>'*^=()[]" , c) || c == 9 || c == '\r' || c == 0 )
			return 1;
	else
			return 0;
}


/* lookup atoken's internal representation in th etoken table */
int look_up( char *s )
{ register int i ;
char *p ;
/* convert to lower case */
p = s ;
//	 while (*p) { * p = toupper(*p) ; p++ ; }

/* see if token is in table */
for( i = 0 ; *table[i].command ; i++ )
	if (!strcmp(table[i].command , s)) return table[i].tok;
return 0 ; /* unkown command */
}

int parse_then_else()
{
	char temp[32] ;
	float value;
	then_else = 1;
	eoi=',';
	//t++;
	get_token();
	if (token_type==NUMBER)
	{
		putback() ;
		eval_exp(&value) ;
		if(renumvar)
		{
	//Fance		//for(int i=0;i<ind_renum;i++)
			//	if(renum[i]==(unsigned)value) break;
			//itoa((i+1)*10,temp,10);
			for(int i=0;i<ind_renum;i++)
			{
				if(renum[i]==(unsigned)value) break;
			itoa((i+1)*10,temp,10);
			}
			putback();
			memmove(prog+strlen(temp), prog+strlen(token), strlen(prog));
			memcpy(prog, temp, strlen(temp));
			prog += strlen(temp);
		}
		if(index_go_to+1 < MAX_GOTO)
		{
			line_table[lline-1].go_to=1;
			go_to[index_go_to].line=lline;
			go_to[index_go_to++].go_to=value;
		}
		get_token();
		//t--;
		then_else=0;
		eoi=NL;
		return 0;
	}
	putback() ;
	prescan1();
	eoi=NL;
	//t--;
	if (then_else==1)
	{
		then_else = 0;
		error = 1;
		return 1;
	}
	then_else = 0;
	return 0;
}

void parse_exp( float *value )
{
	get_token() ;
	if (!*token || *token==',')
	{
		sntx_err(NO_EXP) ;
		return ;
	}
	parse_exp0(value);
	putback() ;
}

char logops[7] = { AND , OR , XOR, 0 } ;
/* Process logical operators */
void parse_exp0( float *value )
{
//	float partial_value ;
	register char op ;

	parse_exp1(value) ;
	op = tok;
	if( strchr(logops, op ) && op!=0) 
	{
		get_token() ;
		parse_exp0(value) ;
		if(type_eval)
			buf_v[index_buf-1].op[index_op++]=op;
	}
} /* end of function eval_exp(1) */

/* Process relational operators */
void parse_exp1( float *value )
{
//	float partial_value ;
	register char op ;

	parse_exp2(value) ;
	op = *token;
	if( strchr(relops, op ) && op!=0) 
	{
		get_token() ;
		parse_exp2(value) ;
		if(type_eval)
			buf_v[index_buf-1].op[index_op++]=op;
	}
} /* end of function eval_exp(1) */


void eval_exp(float *value)
{
 char *p,*q;
 signed char semn;
 get_token();
// get_token();
 if((p=strchr(token,'e'))==0)
	 p=strchr(token,'E');
 if(p)
 {
	*p = 0;
#ifndef NOMATH
	*value = (float)atof(token) ;
#else
	*value = atofproc(token) ;
#endif
	semn = 1;
	p++;
 if((q=strchr(p,'-'))!=0)
	{
	 semn=-1;
	 q++;
	}
 else
 if((q=strchr(p,'+'))!=0)
	 q++;
 if(!q)
	q=p;
#ifndef NOMATH
 *value *= (float)pow(10,semn*atof(q));
#else
 *value *= (float)pow(10,semn*atofproc(q));
#endif
 }
 else
 {
#ifndef NOMATH
		*value = (float)atof(token) ;
#else
		*value = (float)atofproc(token) ;
#endif
 }

 return ;
}


int define_var(char *tok, int t,int l,int c, int lstr)
{
	unsigned char sub_panel_number = 0;
 char *label/*,test*/;
  byte point_type,var_type;
 int i,ret_network,num_net,num_point,num_panel;
 var_type = num_point = point_type = 0;
 ret_network =0;
 /*label=ispoint(tok,&num_point,&var_type,&point_type,&num_panel,&num_net,pedit->network,pedit->panel,&k);*/
 my_panel = Station_NUM;
// label=ispoint(tok,&num_point,&var_type,&point_type,&num_panel,&num_net,my_network,my_panel,&k);
  label=ispoint_ex(tok,&num_point,&var_type,&point_type,&num_panel,&num_net,my_network,sub_panel_number,my_panel,&ret_network);
 if(!label)
 {             // local var
 // if ((strlen(token) == 1 ) && (token[0]>='A' && token[0]<='Z'))
  {
      //2018 07 20 dufan
	 //sntx_err(NOT_VAR); 
	 //error = 1;
	 //return 0;
	 var_type = LOCAL_VAR;
	 label = tok;
	 while( *label )
	 {
		if (strchr( " :!;,+-/<>'*^=()[]" , *label) )
		 {sntx_err(NOT_VAR); return 0;}
		*label++;
	 }
  }
 }


 if (!var_type)
				{sntx_err(NOT_VAR); return 0;}

 for(i=0;i<index_vars_table;i++)
		 if (!strcmp(vars_table[i].name,tok))
				break;
 if ( i>= MAX_Var )
	{sntx_err(TOOMANYVARS); return 0;}

 cur_index=i;

 if(i>=index_vars_table)
 {
  strcpy(vars_table[i].name,tok) ;

  vars_table[i].type = var_type ;

	if (var_type==POINT_VAR || var_type==LABEL_VAR)
	{
					vars_table[i].point_type= point_type;// & 0x1F;
					vars_table[i].num_point = num_point;
					vars_table[i].panel = num_panel;
					vars_table[i].sub_panel = sub_panel_number;
					//if( local_request(pedit->panel,pedit->network) )
					//if( local_request(my_panel,my_network) )
					//{
					//	if ( !k )
					//		if( rs485port!=-1 && ipxport!=-1 )
					//		{
					//			num_net=(( class ConnectionData *)Routing_table[rs485port].ptr)->panel_info1.network;
					//		}
					//}

					//if ( !k ) num_net = 0xFFFF;
					//vars_table[i].network = num_net;
					//if ( !ret_network) num_net = 1;
					vars_table[i].network = ret_network;

					if (var_type==LABEL_VAR)
					{
						strcpy(vars_table[i].label,label);
					}
	}
    if (var_type == LOCAL_VAR)
    {
        if (c && t != STRING_TYPE)
        {
            vars_table[i].var_type = t + FLOAT_TYPE_ARRAY - FLOAT_TYPE;
        }
        else
        {
            if (tok[strlen(tok) - 1] == '%') t = INTEGER_TYPE;
            if (tok[strlen(tok) - 1] == '$') { t = STRING_TYPE; l = 0; c = lstr; }
            vars_table[i].var_type = t;
        }
	vars_table[i].l = l;
	vars_table[i].c = c;

	createlocaltable(i);
	}
//n_global++;
	if (i>=index_vars_table) index_vars_table++;
 }
return 1;
}




int createlocaltable(int cur)
{
	int t=0,j=0,k=0;
	for(j=0;j<ind_local_table; )
	{
		switch((unsigned char)local_table[j])
		{
		case FLOAT_TYPE:
		case LONG_TYPE:
			k = 4;
			break;
		case INTEGER_TYPE:
			k = 2;
			break;
		case BYTE_TYPE:
			k = 1;
			break;
		default:
			{
				switch((unsigned char)local_table[j])
				{
				case FLOAT_TYPE_ARRAY:
				case LONG_TYPE_ARRAY:
					k = 4;
					break;
				case INTEGER_TYPE_ARRAY:
					k = 2;
					break;
				case BYTE_TYPE_ARRAY:
				case STRING_TYPE:
					//						case STRING_TYPE_ARRAY:
					k = 1;
					break;
				}
				if( *((int *)&local_table[j+1]) )
					k = 4+(*((int *)&local_table[j+1])) * (*((int *)&local_table[j+3])) * k;
				else
					k = 4+(*((int *)&local_table[j+3])) * k;
			}
			break;
		}
		j += 1 + k;
		if( !strcmp(&local_table[j], vars_table[cur].name) ) break;
		j += 1+strlen(&local_table[j]);
	}
	if (j == ind_local_table)
	{
		//		local_table[j]=vars_table[cur].var_type;
		//		j++;
		switch(vars_table[cur].var_type)
		{
		case FLOAT_TYPE:
		case LONG_TYPE:
		case FLOAT_TYPE_ARRAY:
		case LONG_TYPE_ARRAY:
			k = 4;
			break;
		case INTEGER_TYPE:
		case INTEGER_TYPE_ARRAY:
			k = 2;
			break;
		case BYTE_TYPE:
		case STRING_TYPE:
		case BYTE_TYPE_ARRAY:
			//			case STRING_TYPE_ARRAY:
			k = 1;
			break;
		}
		local_table[j++]=vars_table[cur].var_type;
		if( vars_table[cur].c)
		{
			//		 local_table[j++]=vars_table[cur].var_type+FLOAT_TYPE_ARRAY-FLOAT_TYPE;
			memcpy(&local_table[j],&vars_table[cur].l,2);
			j += 2;
			memcpy(&local_table[j],&vars_table[cur].c,2);
			j += 2;
			if( vars_table[cur].l )
				k *= (vars_table[cur].l*vars_table[cur].c);
			else
				k *= vars_table[cur].c;

		}
		//		else
		//		{
		//		 local_table[j++]=vars_table[cur].var_type;
		//		}
		memset( &local_table[j], 0, k);
		j += k;
		strcpy(&local_table[j], vars_table[cur].name);
		ind_local_table = j + 1 + strlen(vars_table[cur].name);
	}
	return 0;
}





/* Return a token to the input stream */
void putback ( void )
{ char *t ;
t = token ;
for ( ; *t ; t++ )
{
	prog-- ;
	if( *prog == Srt ) prog--;
}
}


/* Display an error message pg 26 */
void sntx_err(int err, int err_true )
{
//	char *p , *temp ;
	int linecount = 0 ;
//	register int i ;
	static char *e[] = 
	{
	 "syntax error" ,
	 "unbalanced parentheses" ,
	 "no expression present" ,
	 "equals sign expected" ,
	 "not a variable" ,
	 "parameter error" ,
	 "semicolon expected" ,
	 "unbalanced braces" ,
	 "function undefined" ,
	 "type specifier expected" ,
	 "too many nested function calls" ,
	 "return without call" ,
	 "parentheses expected" ,
	 "while expected" ,
	 "closing quote expected" ,
	 "not a string" ,
	 "too many local variables",
	 "wrong or missing line number",
	 "point number out of range",
	 "line number too big",
	 "too many lines",
	 "number of points and variables exceeds 500",
	 "too many goto",
	 "warning: panel out of service",
	 "warning: point out of service",
	 "Output number is too large",
	 "Input number is too large",
	 "Variable number is too large",
	 "Program code larger than 2000",
     "Use instance only support AV AI AO DO"      // 29 
	} ;
	if(!pmes) return;
	if(pmes < mesbuf + ( 1024 - 100 ) )
	{
			strcpy(pmes,e[err]);
			pmes += strlen(pmes);

	memcpy(pmes," in line ",9 );
	pmes += strlen(pmes);
	if(err == TOOBIG)
		ltoa(long_line_value,pmes,10);
	else
		itoa(line_value,pmes,10);
	pmes += strlen(pmes);
	if(err == NOT_VAR)
	{
	 strcpy(pmes," : ");
	 pmes += 3;
	 strcpy(pmes, token);
	 pmes += strlen(pmes);
	}
	*pmes++ = 0x0d;
	*pmes++ = 0x0a;
	error=err_true;
 }
}



/* Process addition or subtraction */
void parse_exp2( float *value )
{
//	float partial_value ;
	register char op ;

	parse_exp3(value) ;
	while((unsigned char)(op = *token ) == '+' || op == '-') 
	{
		get_token() ;
		parse_exp3(value) ;
		if(type_eval)
			buf_v[index_buf-1].op[index_op++]=op;
	}   /* while statement */
}     /* end of function */


/* Process multiplication or division  */
void parse_exp3( float *value )
{
//	float partial_value ;
	register char op ;

	parse_exp4(value) ;
	while((op = *token ) == '*' || op == '/' || op == '%' || op == '&' || op == '|') 
	{
		get_token() ;
		parse_exp4(value) ;
		if(type_eval)
			buf_v[index_buf-1].op[index_op++]=op;
	}
}     /* end of function */



/*

// Process unary operations ie ++ and --
void parse_exp4( float *value )
{
	register char op ;
	op = '\0' ;

	if ((*token == '+' || *token == '-') && token_type!=NL && tok!=FINISHED) {
		op = *token;
		get_token() ;
	} // if statement
	parse_exp5(value) ;
	if (op)
		 if(op=='-') *value = -(*value);
} // end of function

*/
/* Process power */
void parse_exp4( float *value )
{
//	float partial_value ;
	register char op ;

	parse_exp5(value) ;
	while((op = *token ) == '^' ) 
	{
		get_token() ;
		parse_exp5(value) ;
		if(type_eval)
			buf_v[index_buf-1].op[index_op++]=op;
	}
}     /* end of function */



/* Process unparenthesized expression  */
void parse_exp5( float *value )
{
 if ((*token == '(' )) 
 {
	get_token() ;
	parse_exp0(value) ;
	if (*token != ')') sntx_err(PAREN_EXPECTED) ;
	get_token() ;
	if (token_type==NL)
	 if (eol==NL) {--prog;--prog;}
		else --prog;
 }
 else
	parse_atom(value) ;
}     /* end of function */



char address_item;
/* Find the value of a number, variable or function */
void parse_atom( float  *value )
{
	float res;
	int i/*,t*/ ;
	int ftok;
	char pt[14];
	switch((unsigned char)token_type)
	{
	case IDENTIFIER :
	 if((ftok=look_up_func(token))==NOT)
	 {
		 get_token();
		 if ((*token == '(' ))
		 {
			 get_token() ;
			 parse_exp0(value) ;
			 if (*token != ')')
			 {
				sntx_err(PAREN_EXPECTED) ;
				error = TRUE;
				*value = FALSE ;
				get_nl();
				return;
			 }
			 get_token() ;
		 }
		 else
			parse_atom(value) ;
		 if (type_eval)
			 buf_v[index_buf-1].op[index_op++]=ftok;
		 return;
	 }

	 //Fance Add if there are some more white text , ignore it.
	 while( iswhite( *prog) && *prog) 
	 {
		 ++prog ;
		 /* 	fprintf(pmes, "Prog %c\n", *prog ) ; */
	 }

	 if(*prog=='(')
	 {
		 if((ftok=look_up_func(token))==0)
		 {
			memcpy(pt,token,13);
			if (!type_eval)
			{
//			 if((ftok=isarray(pt))==1 )
//			 {
//			  ftok=checklocalvar(pt);
//			 }
			 if( isarray(pt) )
//			 if(ftok)
			 {
				*value = TRUE ;
				get_token() ;
				if (token_type==NL)
				if (eol==NL) {--prog;--prog;}
				else --prog;
			 }
			 else
			 {
			  error = TRUE;
			  *value = FALSE ;
			  sntx_err(SYNTAX);
			  get_nl();
			 }
			}
			else
			{
/*				 ftok=checkarray(pt);
				 if( !vars_table[ftok-1].l )
				 {
					buf[index_buf].fvar = 1;
					strcpy(buf[index_buf].var,"");
					buf[index_buf].v=0;
					index_op=0;
					for(i=0;i<MAX_OP;i++)
						 buf[index_buf].op[i]=0;
					index_buf++;
					n_var++;
				 }
*/
				 prog++;
				 get_token();
				 parse_exp0(value) ;
				 if (*token==',')
				 {
				  get_token();
				  parse_exp0(value) ;
				  buf_v[index_buf-1].op[index_op++]=ASSIGNARRAY_2;
				 }
				 else
				  buf_v[index_buf-1].op[index_op++]=ASSIGNARRAY_1;
//
				 buf_v[index_buf].v=1;
				 index_op=0;
				 for(i=0;i<MAX_OP;i++)
					 buf_v[index_buf].op[i]=0;
				 memcpy(buf_v[index_buf].var,pt,13);
				 buf_v[index_buf].fvar=0;
				 if( address_item )
					buf_v[index_buf].op[index_op++]=ASSIGNARRAY;
				 index_buf++;
				 n_var++;
//
				 get_token() ;
					if (token_type==NL)
						if (eol==NL) {--prog;--prog;}
						else --prog;
			}
			return;
		 }
		 else
		 {
			 prog++ ;
			 get_token();
			 switch (ftok) 
			 {
				 case ABS:
				 case _INT:
				 case LN:
				 case LN_1:
				 case SQR:
				 case _Status:
				 case RUNTIME:
				 case SENSOR_ON:
				 case SENSOR_OFF:
								 parse_exp0(value) ;
								 if (*token != ')') 
								 {	error = TRUE; *value = FALSE ; sntx_err(SYNTAX); get_nl();return;}
								 if(type_eval)
									 buf_v[index_buf-1].op[index_op++]=ftok;
								 break;
				 case TIME_ON:
				 case TIME_OFF:
				 case INTERVAL:
								 parse_exp0(value) ;
								 if (*token != ')') 
								 {	error = TRUE; *value = FALSE ;get_nl(); sntx_err(SYNTAX);return;}
								 if(type_eval)
								 {
									buf_v[index_buf-1].op[index_op++]=ftok;
									if (ftok == INTERVAL)
									{
									 buf_v[index_buf-1].op[index_op++]=0;
									 buf_v[index_buf-1].op[index_op++]=0;
									 buf_v[index_buf-1].op[index_op++]=0;
									 buf_v[index_buf-1].op[index_op++]=0;
									}
								 }
								 break;
				 case AVG:
				 case MAX:
				 case MIN:
				 case COM_1:
							  {
								 char eoiold = eoi;
								 eoi = NL;
								 parse_exp0(value) ;
								 i=1;
								 while(*token == ',')
											{
											 get_token();
											 parse_exp0(value) ;
											 i++;
											}
								 eoi = eoiold;
								 if (*token != ')') 
								 {	
									 error = TRUE; 
									 *value = FALSE ; 
									 sntx_err(SYNTAX);
									 get_nl();
									 return;
								 }
								 if(type_eval)
								 {
									buf_v[index_buf-1].op[index_op++]=ftok;
									buf_v[index_buf-1].op[index_op++]=i;
								 }
								}
								 break;
				 case INKEYD:
				 case OUTPUTD:
								 address_item = 1;
				 case CONPROP:
				 case CONRATE:
				 case CONRESET:
				 case Tbl:
				 case WR_ON:
				 case WR_OFF:
								 if(ftok==INKEYD || ftok==OUTPUTD)
								 {
									memcpy(pt,token,13);
								 }
								 parse_exp0(value) ;
								 if(ftok==INKEYD || ftok==OUTPUTD)
									 address_item = 0;
								 if (*token != ',') 
								 {	error = TRUE; *value = FALSE ; sntx_err(SYNTAX);get_nl();return;}
								 get_token();
								 parse_exp0(value) ;
								 if (*token != ')') 
								 {	error = TRUE; *value = FALSE ; sntx_err(SYNTAX);get_nl();return;}
								 if(ftok==INKEYD || ftok==OUTPUTD)
								 {
									i=checklocalvar(pt);
									if(i)
									{
									  if(type_eval && ftok==INKEYD)
									  {
										 buf_v[index_buf].fvar = varoffsetlast(i-1);
										 strcpy(buf_v[index_buf].var,"");
										 buf_v[index_buf].v=0;
										 index_op=0;
										 for(i=0;i<MAX_OP;i++)
											 buf_v[index_buf].op[i]=0;
										 index_buf++;
										 n_var++;
									  }
									}
									else
									 {	error = TRUE; *value = FALSE ; sntx_err(SYNTAX);get_nl();return;}
								 }
								 if(type_eval)
									 buf_v[index_buf-1].op[index_op++]=ftok;
								 break;
				 default:
								{	error = TRUE; *value = FALSE ; sntx_err(SYNTAX);get_nl();return;}
			 }
			 get_token();
			 if (token_type==NL)
				if (eol==NL) {--prog;--prog;}
				else --prog;
		 }
		 return;
	 }

	 if((ftok=look_up_func(token))!=0)
	 {
		 switch (ftok) {
				case SUN:
				case MON:
				case TUE:
				case WED:
				case THU:
				case FRI:
				case SAT:
				case JAN:
				case FEB:
				case MAR:
				case APR:
				case MAY:
				case JUN:
				case JUL:
				case AUG:
				case SEP:
				case OCT:
				case NOV:
				case DEC:
				case DOM:
				case DOW:
				case DOY:
				case MOY:
				case POWER_LOSS:
				case SCANS:
				case TIME:
				case USER_A:
				case USER_B:
				case UNACK:
				 if (type_eval)
				 {
					buf_v[index_buf].var[0]=ftok;
					buf_v[index_buf].v=-1;
					index_op=0;
					for(i=0;i<MAX_OP;i++)
						 buf_v[index_buf].op[i]=0;
					index_buf++;
					n_var++;
				 }
				else
					*value = TRUE ;
				get_token() ;
				if (token_type==NL)
						if (eol==NL) {--prog;--prog;}
						else --prog;
				return ;
				break;
			 }
	 }

	 if (type_eval)
	 {
		 if(*prog=='[')
		 {
				 buf_v[index_buf].v=1;
				 index_op=0;
				 for(i=0;i<MAX_OP;i++)
					 buf_v[index_buf].op[i]=0;
				 memcpy(buf_v[index_buf].var,token,13);
				 buf_v[index_buf].fvar=0;
				 index_buf++;
				 n_var++;

				 prog++;
				 get_token();
				 parse_exp0(value) ;
				 buf_v[index_buf-1].op[index_op++]=ASSIGNAR;
		}
		else
		{
		 buf_v[index_buf].v=1;
		 index_op=0;
		 for(i=0;i<MAX_OP;i++)
				 buf_v[index_buf].op[i]=0;
		 strcpy(buf_v[index_buf].var,token);
		 buf_v[index_buf].fvar=0;

		 if( checkonlyarray(token) )
			buf_v[index_buf].op[index_op++]=ASSIGNARRAY;

		 index_buf++;
		 n_var++;
		}
		get_token() ;
		if (token_type==NL)
			if (eol==NL) {--prog;--prog;}
				else --prog;
	 }
	 else
		if (define_var(token))
		{
			*value = TRUE ;
			get_token() ;
			if (*token=='[')
			{
//				 type_eval=0;
				 parse_exp(&res);
				 if(!res)
				 {
					error = TRUE;
					*value = FALSE ;
					sntx_err(SYNTAX);
					get_nl();
				 }
				 get_token();
				 if (*token!=']')
				 {
					error = TRUE;
					*value = FALSE ;
					sntx_err(SYNTAX);
					get_nl();
				 }
				 get_token();
			}
			if (token_type==NL)
				if (eol==NL) {--prog;--prog;}
				else --prog;
		}
		else
		{
//			error = TRUE;
			*value = FALSE ;
//			sntx_err(SYNTAX);
			get_nl();
		}
		return ;

	case TTIME:
	case NUMBER:
		if (type_eval)
		{
			if (token_type==TTIME)
				if (DORU_SYSTEM)
					buf_v[index_buf].fvar=ttimed;
				else
					buf_v[index_buf].fvar=ttime;
			else
			{
			 putback();
			 eval_exp(&buf_v[index_buf].fvar) ;
			}
			strcpy(buf_v[index_buf].var,"");
			buf_v[index_buf].v=0;
			index_op=0;
			for(i=0;i<MAX_OP;i++)
				 buf_v[index_buf].op[i]=0;
			if(token_type == TTIME)
				 buf_v[index_buf].op[index_op++]=TIME_FORMAT;
			index_buf++;
			n_var++;
		}
		else
		{
			*value = TRUE ;
/*			get_token();
			if(!strcmp(token,"e") || !strcmp(token,"E"))
			{
			 get_token();
			 if(token[0]=='-' || token[0]=='+')
				 get_token();
			 if(token_type!=NUMBER)
			 {
				error = TRUE;
				*value = FALSE ;
				get_nl();
				return;
			 }
			}
			else
			 putback();
*/
		}
		get_token() ;
		if (token_type==NL)
				if (eol==NL) {--prog;--prog;}
				else --prog;
		return ;
	case DELIMITER :
		if(*token=='\'') 
		{
			*value = *prog ;
			prog++ ;
			if(*prog != '\'')
			{	error = TRUE; *value = FALSE ; sntx_err(QUOTE_EXPECTED) ;get_nl();return;}
			prog++;

		 if (type_eval)
		 {
			buf_v[index_buf].fvar=*value ;
			strcpy(buf_v[index_buf].var,"");
			buf_v[index_buf].v=0;
			index_op=0;
			for(i=0;i<MAX_OP;i++)
				 buf_v[index_buf].op[i]=0;
			index_buf++;
			n_var++;
		 }
		 *value = TRUE ;
		 get_token() ;
		}
		else
		if(*token=='-') 
		{
		 get_token() ;
		 parse_exp0(value) ;
		 if (type_eval)
			 buf_v[index_buf-1].op[index_op++]=MINUSUNAR;
		 else
			 *value = TRUE ;
		}
		else
			{
			*value = FALSE ;
			error = TRUE;
			get_nl();
			sntx_err(SYNTAX) ;
			}
		return ;
	default :
	 if (*token==')') return; /* process an empty expression */
	 else
		{
			*value = FALSE ;
			error = TRUE;
			get_nl();
			sntx_err(SYNTAX) ;
		}
	}   /* end of switch statement */
}     /* end of atom() */



char *ispoint_ex(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network,unsigned char & sub_panel, byte panel , int *netpresent)
{

	int /*i,j*/k,l;
	char pc[30],*p,*q,buf[30],*tok;
	char fance_sub_pc[30];
	char *fance_tok;
	memset(fance_sub_pc,0,30);
	memset(pc,0,30);
	memset(buf,0,30);
	tok = token;
	if(netpresent) *netpresent = 0;
	*num_net = network;
	if(strlen(token)==0) return 0;
#if 0   //Mark by Fance  点暂时用来当main 和sub的分隔符，不用在network;
	if ((q=strchr(token,'.'))!=NULL)
	{
		memcpy(pc,token,min( (int)(q-token),10));
		pc[min((int)(q-token),10)]=0;
		q++;
		*num_net = atoi(pc);
		token = q;
		if(netpresent)
			*netpresent = *num_net;
	}
#endif
    int b_is_instance = false;
    char * temp_next = token + 3;
    int token_length = strlen(token);
    char * temp_last = token + token_length - 1;
    if (((q = strstr(token, "INS")) != NULL) && ((*(temp_next) >= '0') && (*(temp_next) <= '9'))     && ((*(temp_last) >= '0') && (*(temp_last) <= '9')))
    {
        b_is_instance = true;
        Sleep(1);
    }
	
    if (((q=strchr(token,'-'))!=NULL) || ((q=strchr(token,'.'))!=NULL))
	{
		memcpy(pc,token,min( (int)(q-token),10));
		pc[min((int)(q-token),10)]=0;

#if 1
		if((*(q + 1) >= '0') && (*(q + 1) <= '9'))	//说明存在sub panel;
		{
			q++;
			fance_tok = q;
			if ((q=strchr(fance_tok,'-'))!=NULL)  //妈的，老毛不要第二个 杠 ， 又得改;
			{
				memcpy(fance_sub_pc,fance_tok,min( (int)(q-fance_tok),10));
				fance_sub_pc[min((int)(q-fance_tok),10)]=0;

				q++;
				*num_panel = atoi(pc);

				sub_panel = atoi(fance_sub_pc);
			}
			else if ((q=strchr(fance_tok,'.'))!=NULL)  //妈的，要改成用冒号来当分隔符，想一出是一出;
			{
				memcpy(fance_sub_pc,fance_tok,min( (int)(q-fance_tok),10));
				fance_sub_pc[min((int)(q-fance_tok),10)]=0;

				q++;
				*num_panel = atoi(pc);

				sub_panel = atoi(fance_sub_pc);
			}
			else
			{
				return 0;
			}

		}		
		else
		{
			q++;
			*num_panel = atoi(pc);
			sub_panel = *num_panel;
			itoa((int)(*num_panel),(char *)fance_sub_pc,10);

			// = ;
		}
#endif
		//Fance		recode;		
#if 0
		memcpy(pc,token,min( (int)(q-token),10));
		pc[min((int)(q-token),10)]=0;
		q++;
		*num_panel = atoi(pc);
		//if ( *num_panel>32 || *num_panel==0 )
		//	q=token;
#endif
	}
	else	//如果是直接var1 就代表 使用的是自己的 main和sub都是一个;Fance
	{
        //因为要支持instance 所以数比较大，不局限于以前的 255那么大
		//for(l=0;l<3 && token[l]!=0;l++)
		//	if (token[l]<'0' || token[l]>'9')
		//		break;
        if (b_is_instance)
            token = token + 3; //因为前面加了M

        for (l = 0;l<10 && token[l] != 0;l++)
            if (token[l]<'0' || token[l]>'9')
                break;


		    q=token+l;
		memcpy(pc,token,l);
		pc[l]=0;
		*num_panel = atoi(pc);
		if (*num_panel==0)
		{
			*num_panel = panel;
			//sub_panel = *num_panel; //填VAR1 之类的 subpanel 改为0 
            sub_panel = 0;
			itoa((int)(*num_panel),(char *)fance_sub_pc,10);
		}
#if 0   //需要支持 bacnet 的 instance 所以 屏蔽下面的255 主panel的限制.
		if ( *num_panel>255 && strlen(pc) )
			return(islabel(token,num_point,var_type,point_type,num_panel));
#endif
	}
	//	strcpy_s(my_info_panel[0].name,4,"VAR");

	if ((p=strpbrk(q, "0123456789"))!=NULL)
	{
		memcpy(pc,q,p-q);
		pc[p-q]=0;
		for(k=OUT;k<= MAX_FUNCTION_COUNT;k++)
		{
			if(k!=DMON)
			{
				if (!strcmp(pc,ptr_panel.info[k].name))
					break;
				else if(strcmp(pc,"REG") == 0)
				{
					k = VAR;
					break;
				}
			}
		}
			

		if (k<= MAX_FUNCTION_COUNT)
		{
			if (p==NULL) 
			{
				memcpy(pmes,"error line : ",13);
				pmes += 13;
				itoa(lline,pmes,10);
				pmes += strlen(pmes);
				*pmes++ = 0x0d;
				//												fprintf(pmes,"error line %d\n",line);
				error=1;return 0;
			}
			else  if (((strlen(p)==1) && (*p=='0')) && 
				((k!= COIL_REG) && (k!= DIS_INPUT_REG) && (k!= INPUT_REG) && (k!= MB_REG) &&
				(k != BAC_AV) && (k != BAC_AI) && (k != BAC_AO) && (k != BAC_DO) && (k != BAC_BV) && (k != BAC_BI)))
			{
				memcpy(pmes,"error line : ",13);
				pmes += 13;
				itoa(lline,pmes,10);
				pmes += strlen(pmes);
				*pmes++ = 0x0d;
				//								fprintf(pmes,"error line %d\n",line);
				error=1;return 0;
			}
			else 
			{
				for(l=0;l<(int)strlen(p);l++)//括号 Fance 加
				{
					if (p[l]<'0' || p[l]>'9')
						break;
				}
				if (l<(int)strlen(p))
					return(islabel(token,num_point,var_type,point_type,num_panel));
				else
				{
					//														itoa(panel,buf,10);
					buf[0] = 0;
#if 0   //2018 07 10   network point 用来复用 给 object instance .
					if(netpresent)
						if(*netpresent)
						{
							itoa(*num_net,buf,10);
							strcat(buf,".");
						}
#endif
#if 0
						itoa(*num_panel,&buf[strlen(buf)],10);
						*num_point=atoi(p);
						*point_type = k;
						if(*num_panel<10 || *num_point<100)
							strcat(buf,"-");
						strcat(buf,pc);
						strcat(buf,p);
						strcpy(tok,buf);

#endif
#if 1
						itoa(*num_panel,&buf[strlen(buf)],10);
						*num_point=atoi(p);
						unsigned char high_3bit =  0;
						if(*num_point % 0x100 == 0)
						{
							if(*num_point == 0x100)
								high_3bit = 0;
							else if(*num_point == 0x200)
								high_3bit = 0x20;
							else if(*num_point == 0x300)
								high_3bit = 0x40;
							else if(*num_point == 0x400)
								high_3bit = 0x80;
						}
							
						else
							high_3bit = ((*num_point) & 0xff00) >> 3;

                        if ((k == MB_REG) && (*num_point >=2000))
                        {
                            int temp_num_point = *num_point;
                            temp_num_point = temp_num_point >> 11;
                            temp_num_point = temp_num_point | 0x0080;
                            *netpresent = temp_num_point; // 去Number 的 最高5位;

                        }

						*point_type = k;
						*point_type = *point_type | high_3bit;
						//if(*num_panel<10 || *num_point<100)
						//	strcat(buf,"-");
                        if ((*num_panel >= 256) || (b_is_instance == true))
                        {
                            unsigned int temp_value = 0;
                            temp_value = *num_panel;
                            *netpresent = temp_value >> 16;
                            *netpresent = *netpresent | 0x00000080;
                            
                            *num_panel = temp_value >> 8;
                            *num_panel = *num_panel & 0x000000FF;

                            sub_panel = (unsigned char)(temp_value & 0x000000FF);
                            Sleep(1);
                        }
                        else
                        {
                            //if (*num_panel<10 || *num_point< (8 * 256))
                                strcat(buf, ".");
                            strcat(buf, fance_sub_pc);
                            strcat(buf, ".");
                            strcat(buf, pc);
                            strcat(buf, p);
                        }



                        //FANDU  2018 01 26 现在开始要慢慢支持访问其他panel 的数据，所以 判断不等于本地panel的需要拿掉;
						//if((*num_panel !=Station_NUM ) || (sub_panel != Station_NUM))
						//{
						//	*num_point = *num_point & 0x000000ff;
						//	strcpy(tok,buf);
						//}

#endif
						//		return tok;
						if((p=look_label_ex(*num_panel,sub_panel,k,*num_point,*num_net)) != NULL)
						{
							*var_type = LABEL_VAR;
							return p;
						}
						else
						{
							*var_type = POINT_VAR;
							return tok;
						}
				}
			}
		}
	}


	//return 0;
	char * temp_ret = NULL;
	temp_ret = islabel(token,num_point,var_type,point_type,num_panel);
	if(temp_ret != NULL)
	{
		sub_panel = panel;
		return temp_ret;
	}

    //// 例如 编程   123456IN4   会报错 ，因为这种利用object instance 去编程 只支持 AV AI AO DO
    if ((*num_panel >= 256) || b_is_instance)
    {
        if ((k != BAC_AV) && (k != BAC_AI) && (k != BAC_AO) && (k != BAC_DO) && (k != BAC_BI) && (k != BAC_BV))
        {
            sntx_err(INSTANCE_NOT_SUPPORT); //instance 不支持 此 type. 
            error = INSTANCE_NOT_SUPPORT;
            return 0;
        }
    }
	return(islabel(token,num_point,var_type,point_type,num_panel));//fance
}




char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent)
{
	int /*i,j*/k,l;
	char pc[11],*p,*q,buf[21],*tok;
	tok = token;
	if(netpresent) *netpresent = 0;
	*num_net = network;
	if(strlen(token)==0) return 0;
	if ((q=strchr(token,'.'))!=NULL)
	{
		memcpy(pc,token,min( (int)(q-token),10));
		pc[min((int)(q-token),10)]=0;
		q++;
		*num_net = atoi(pc);
		token = q;
		if(netpresent)
			*netpresent = *num_net;
	}
	if ((q=strchr(token,'-'))!=NULL)
	{
		memcpy(pc,token,min( (int)(q-token),10));
		pc[min((int)(q-token),10)]=0;
		q++;
		*num_panel = atoi(pc);
		//Fance //if ( *num_panel>32 || *num_panel==0 )
		//	q=token;
	}
	else
	{
		for(l=0;l<3 && token[l]!=0;l++)
			if (token[l]<'0' || token[l]>'9')
				break;
		q=token+l;
		memcpy(pc,token,l);
		pc[l]=0;
		*num_panel = atoi(pc);
		if (*num_panel==0)
			*num_panel = panel;
		//if ( *num_panel>32 && strlen(pc) )
		if ( *num_panel>255 && strlen(pc) )
			//return 0;
			return(islabel(token,num_point,var_type,point_type,num_panel));
	}
	//	strcpy_s(my_info_panel[0].name,4,"VAR");
#if 0
	my_info_panel[0].name = "OUT" ;
	my_info_panel[1].name = "IN";
	my_info_panel[2].name = "VAR";
#endif
	if ((p=strpbrk(q, "123456789"))!=NULL)
	{
		memcpy(pc,q,p-q);
		pc[p-q]=0;
		for(k=OUT;k<=AY;k++)
			if(k!=DMON)
				//if (!strcmp(pc,my_info_panel[k].name))
				//	break;
				if (!strcmp(pc,ptr_panel.info[k].name))
					break;
		if (k<=AY)
		{
			if (p==NULL) 
			{
				memcpy(pmes,"error line : ",13);
				pmes += 13;
				itoa(lline,pmes,10);
				pmes += strlen(pmes);
				*pmes++ = 0x0d;
				//												fprintf(pmes,"error line %d\n",line);
				error=1;return 0;
			}
			else  if ((strlen(p)==1) && (*p=='0'))
			{
				memcpy(pmes,"error line : ",13);
				pmes += 13;
				itoa(lline,pmes,10);
				pmes += strlen(pmes);
				*pmes++ = 0x0d;
				//								fprintf(pmes,"error line %d\n",line);
				error=1;return 0;
			}
			else 
			{
				for(l=0;l<(int)strlen(p);l++)//括号 Fance 加
				{
					if (p[l]<'0' || p[l]>'9')
						break;
				}
				if (l<(int)strlen(p))
					return(islabel(token,num_point,var_type,point_type,num_panel));
				else
				{
					//														itoa(panel,buf,10);
					buf[0] = 0;
					if(netpresent)
						if(*netpresent)
						{
							itoa(*num_net,buf,10);
							strcat(buf,".");
						}
						itoa(*num_panel,&buf[strlen(buf)],10);
						*num_point=atoi(p);
						*point_type = k;
						if(*num_panel<10 || *num_point<100)
							strcat(buf,"-");
						strcat(buf,pc);
						strcat(buf,p);
						strcpy(tok,buf);


						//		return tok;
						if((p=look_label(*num_panel,k,*num_point,*num_net)) != NULL)
						{
							*var_type = LABEL_VAR;
							return p;
						}
						else
						{
							*var_type = POINT_VAR;
							return tok;
						}
				}
			}
		}
	}
	//return 0;
	return(islabel(token,num_point,var_type,point_type,num_panel));//fance
}


Str_out_point My_outputs[10];
Str_variable_point  My_vars[10];


char *islabel(char *token,int *num_point,byte *var_type,byte *point_type, int *num_panel)
{
	char *p;
	int i,j;

	if(!local_panel)
	{
		#ifdef Fance_enable
		p=0;
		if(Des)
		{
			p=Des->islabel(token,num_point,var_type,point_type,num_panel);
		}
		if( p )
			return(p);
		#endif
	}

	for(i=OUT;i<=AY;i++)
		//		for(j=0;j<ptr_panel->info[i].max_points;j++)
		for(j=0;j<ptr_panel.table_bank[i];j++)
		{
			p = 0;
			if(i!=CON)
			{
				switch (i) {
				case OUT:
					p = ptr_panel.outputs[j].label;
					break;
				case IN:
					p = ptr_panel.inputs[j].label;
					break;
				case VAR:
					p = ptr_panel.vars[j].label;
					break;
				case CON:
					//p = ptr_panel.controllers[j].;
					break;
				case WR:
					p = ptr_panel.weekly_routines[j].label;
					break;
				case AR:
					p = ptr_panel.annual_routines[j].label;
					break;
				case PRG:
					p = ptr_panel.programs[j].label;
					break;
				case GRP:
					p = ptr_panel.control_groups[j].label;
					break;
				case AMON:
					p = ptr_panel.analog_mon[j].label;
					break;
				case AY:
					p = ptr_panel.arrays[j].label;
					break;
				default:
					break;
				}
				if(p)
				{
					char * temp_p = p;
					char * temp_token = token;
					for (int i=0;i<strlen(p);i++)
					{
						*temp_p = toupper(*temp_p);
						temp_p ++;
					}

					for (int i=0;i<strlen(token);i++)
					{
						*temp_token = toupper(*temp_token);
						temp_token ++;
					}

					if(!strcmp(rtrim(p),rtrim(token)))  // de verificat duplicate
					{
						*var_type = LABEL_VAR;
						itoa(Station_NUM,token,10);
						//Fance strcat(token,lin);
						strcat(token,"-");
						char temp[20];
						memset(temp ,0 , 20);
						itoa(Station_NUM,temp,10);
						strcat(token,temp);
						
						strcat(token,"-");
						strcat(token,ptr_panel.info[i].name);
						itoa(j+1,&token[strlen(token)],10);
						*num_point = j+1;
						*point_type = i;
						//						 *num_panel = ptr_panel->GlPanel;
						*num_panel = Station_NUM;
						return p;
					}
				}
			}
		}
		p=0;
		if(local_panel)
		{
			#ifdef Fance_enable
			if(Des)
			{
				p=Des->islabel(token,num_point,var_type,point_type,num_panel);
			}
			#endif

		}
		return(p);
}


int local_request(int panel, int network)
{
	if( panel==Station_NUM && localnetwork(network) )
		return 1;
	else
		return 0;
}

int local_request_ex(int panel, int sub_panel, int network)
{
    //2018 01 26   subpanel 都变为0 
	//if((panel == Station_NUM) && (sub_panel == Station_NUM) && localnetwork(network) )
    if ((panel == Station_NUM) && localnetwork(network))
		return 1;
	else
		return 0;
}



//  input:  t=1  check all posible networks
//          t=0  check only direct connected networks

//  ret: -1 network number  not found
//       >=0 and j=-1  direct connected network
//       >=0 and j>=0  reachable network
//
int findroutingentry(int port, int network, int &j, int t)
{
	int i;
	j=-1;
	for(i=0; i<MAX_Routing_table; i++)
	{
		if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE )
			if( Routing_table[i].Port.network==network )
			{
				break;
			}
			else
			{
				for(j=0; j<MAX_reachable_networks; j++)
					if( (t && ((Routing_table[i].Port.networks_list[j].status&REACHABLE) == REACHABLE)) &&
						Routing_table[i].Port.networks_list[j].network==network)
					{
						//					 destination=Routing_table[i].Port.networks_list[j].router_address;
						break;
					}
					if(j<MAX_reachable_networks) break;
					j = -1;
			}
	}
	if (i<MAX_Routing_table)
	{
		return i;
	}
	else
		return -1;
}



// ret: 1 direct connected network
//      0 reachable network
int localnetwork(int net)
{
	 int i,j;
	 i=findroutingentry(0, net, j);
	 if(i!=-1)
	 {
		if( (Routing_table[i].status&PTP_ACTIVE)==PTP_ACTIVE)
		{
			return 0;
		}
	 }
	 if(j!=-1) return 0;
/*
			for(int j=0; j<MAX_Routing_table ; j++)
			{
				if( ((Routing_table[j].status&RS485_ACTIVE)==RS485_ACTIVE) || ((Routing_table[j].status&IPX_ACTIVE)==IPX_ACTIVE))
				{
					if ( Routing_table[j].Port.network == net ) return 1;
				}
			}
			return 0;
*/
	 return 1;
}

char *look_label_ex(int panel,int sub_panel, int point_type, int num, int network)
{
	char *p=NULL;
	// if(local_panel)
	if( local_request_ex(panel,sub_panel, network) )
		//if((panel == Station_NUM) && (sub_panel == Station_NUM))
		{
			switch (point_type) 
			{
			//case 1:
			//	p = ptr_panel.outputs[num-1].label;
			//	break;
			//case 0:
			//	p = ptr_panel.inputs[num-1].label;
			//	break;
			case OUT:
				p = ptr_panel.outputs[num-1].label;
				break;
			case IN:
				p = ptr_panel.inputs[num-1].label;
				break;
			case VAR:
				//strcpy(ptr_panel.vars[num-1].label,"VAR");
				p = ptr_panel.vars[num-1].label;
				break;
			case CON:
				return 0;
				//									p = ptr_panel->controllers[num].label;
				break;
			case WR:
				p = ptr_panel.weekly_routines[num-1].label;
				break;
			case AR:
				p = ptr_panel.annual_routines[num-1].label;
				break;
			case PRG:
				p = ptr_panel.programs[num-1].label;
				break;
			case AMON:
				p = ptr_panel.analog_mon[num-1].label;
				break;
			case AY:
				p = ptr_panel.arrays[num-1].label;
				break;
			default:
				break;
			}
			if(p)
			{
				p=rtrim(p);
				if (strlen(p) == 0)
					return 0;
				else
					return p;
			}
			else
				return 0;

		}
		p=0;
		if(point_type!=CON && point_type!=TBL)
		{
			//fance if(Des)
			//fance 	p = Des->look_label(panel,point_type,num);
			//fance     look_label(panel,point_type,num,0);
		}
		return p;
}

char *look_label(int panel, int point_type, int num, int network)
{
	char *p=NULL;
	// if(local_panel)
	if( local_request(panel, network) )
		if(panel == Station_NUM)
		{
			switch (point_type) {
			case OUT:
				//strcpy(ptr_panel.outputs[num-1].label,"OUT");
				p = ptr_panel.outputs[num-1].label;
				break;
			case IN:
				//strcpy(ptr_panel.inputs[num-1].label,"IN");
				p = ptr_panel.inputs[num-1].label;
				break;
			case VAR:
				//strcpy(ptr_panel.vars[num-1].label,"VAR");
				p = ptr_panel.vars[num-1].label;
				break;
			case CON:
				return 0;
				//									p = ptr_panel->controllers[num].label;
				break;
			case WR:
				p = ptr_panel.weekly_routines[num-1].label;
				break;
			case AR:
				p = ptr_panel.annual_routines[num-1].label;
				break;
			case PRG:
				p = ptr_panel.programs[num-1].label;
				break;
			case AMON:
				p = ptr_panel.analog_mon[num-1].label;
				break;
			case AY:
				p = ptr_panel.arrays[num-1].label;
				break;
			case GRP:
				p = ptr_panel.control_groups[num-1].label;
				break;
			default:
				break;
			}
			if(p)
			{
				p=rtrim(p);
				if (strlen(p) == 0)
					return 0;
				else
					return p;
			}
			else
				return 0;

		}
		p=0;
		if(point_type!=CON && point_type!=TBL)
		{
			//fance if(Des)
			//fance 	p = Des->look_label(panel,point_type,num);
			//fance     look_label(panel,point_type,num,0);
		}
		return p;
}


int checklocalvar(char *tok)
{
	int i;
	for(i=0;i<index_vars_table;i++)
		if (!strcmp(vars_table[i].name,tok))
			break;
	if(i>=index_vars_table) return 0;
	if(vars_table[i].type!=LOCAL_VAR) return 0;
	// if( (l>=1 && l<=vars_table[i].l) && (c>=1 && c<=vars_table[i].c) )
	// {
	//	t=vars_table[i].var_type;
	return i+1;
	// }
	// return 0;
}



int look_up_func( char *s )
{ register int i ;
char *p ;
/* convert to lower case */
p = s ;
//	 while (*p) { * p = toupper(*p) ; p++ ; }

/* see if token is in table */
for( i = 0 ; *func_table[i].func_name ; i++ )
	if (!strcmp(func_table[i].func_name , s)) 
		return func_table[i].tok;
return 0 ; /* unkown command */
}


int isarray(char *tok)
{
//	float value;
	int i;
	i=checklocalvar(tok);
	if( !i ) return 0;
	get_token();
	if (*token=='(')
	{
		parse_exp(&res);
		if(!res)
			return 0;
		get_token();
		if (*token!=')')
		{
			if( !vars_table[i-1].l ) 
				return 0;
			if (*token!=',')
				return 0;
			parse_exp(&res);
			if(!res)
				return 0;
			get_token();
			if (*token!=')')
				return 0;
			return 1;
		}
		if( vars_table[i-1].l && vars_table[i-1].c ) 
			return 0;
		return 1;
	}
	else
	{
		return 2;
	}
}

int varoffsetlast(int cur)
{
	int j,k;
	for(j=0;j<ind_local_table; )
	{
		switch((unsigned char)local_table[j]){
		case FLOAT_TYPE:
		case LONG_TYPE:
			k = 4;
			break;
		case INTEGER_TYPE:
			k = 2;
			break;
		case BYTE_TYPE:
			k = 1;
			break;
		default:
			{
				switch((unsigned char)local_table[j]){
				case FLOAT_TYPE_ARRAY:
				case LONG_TYPE_ARRAY:
					k = 4;
					break;
				case INTEGER_TYPE_ARRAY:
					k = 2;
					break;
				case BYTE_TYPE_ARRAY:
				case STRING_TYPE:
					//					case STRING_TYPE_ARRAY:
					k = 1;
					break;
				}
				if( (*((int *)&local_table[j+1])) )
					k = 4+(*((int *)&local_table[j+1])) * (*((int *)&local_table[j+3])) * k;
				else
					k = 4 + (*((int *)&local_table[j+3])) * k;
			}
			break;
		}
		//	t = j+1;
		j += 1 + k;
		if( !strcmp(&local_table[j], vars_table[cur].name) )
		{
			return j;;
		}
		j += 1+strlen(&local_table[j]);
	}
	return 0;
}

int checkonlyarray(char *tok)
{
	int i;
	for(i=0;i<index_vars_table;i++)
		if (!strcmp(vars_table[i].name,tok))
			break;
	if(i>=index_vars_table) return 0;
	if(vars_table[i].type!=LOCAL_VAR) return 0;
	if( !vars_table[i].l && !vars_table[i].c ) return 0;
	return i+1;
}


void prescan2(void)
{
	int v1,v2,v3 ;
	float value,fvar1,fvar2,fvar3;
	int i,command ;
	char *p;
	char temp[31];//,temp1[11];
	char block = 0 ;
	char var1[12],var2[12],var3[12];
	int nrlin[10];
	p = prog ;

	get_token() ;
	while( (unsigned char)tok!=FINISHED)  
	{
		if(error != -1)
			break;
//	for(i=0;i<100;i++)
//				cod_line[i]=0;
		if (token_type == FINISHED)
				break;
		if (token_type == NL)
			 if ((then_else!=0) && eol==NL )
				 return;
			 else
			 {
			  while(token_type==NL) get_token();
			 }
		if (tok==FINISHED) return;

		if (!then_else)
		{
		if( token_type != NUMBER )
				{
				get_nl();
				memcpy(pmes,"error: missing line number",27);
				pmes += 27;
				*pmes++ = 0x0d;
//				fprintf(pmes,"error: missing line number\n");
				get_token() ;
				continue;
				}

		line_value=atoi(token);       //(int)value;
		line_table[lline].line=(int)line_value;
		/*line_table[lline].pos=code-pedit->code;*///fance
			line_table[lline].pos=code-mycode;
		lline++;
	//	showgauge(gaugep, coef, lline);

		get_token() ;
		}
		else
		 then_else++;

		switch (token_type) {

		case IDENTIFIER:
						v2=v3=0;
						strcpy(temp,token);


						get_token() ;    //  override '='
						if( *token=='[' )
							command = ASSIGNAR;
						else
						{
						 if( *token=='(' )
							command = ASSIGNARRAY_2;
						 else
							command = ASSIGN;
						}
						if (*token=='[' || *token=='(')
						{
						  if (*token=='(')
						  {
							i=checklocalvar(temp);
							if( !vars_table[i-1].l )    // one index
							{
								command = ASSIGNARRAY_1;
/*
								for(i=0;i<7;i++)
								{
								  memset(&buf2[i],0,sizeof(buf2[i]));
								}
								buf2[0].fvar = 1;
								strcpy(buf2[0].var,"");
								index_buf++;
								v2 = 1;
*/
							}
						  }
							index_buf=0;
							index_op=0;
							n_var=0;
							type_eval=1;
							memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
							parse_exp((float *)buf_v);
							for(i=v2;i<7;i++)
							{
							 buf2[i] = buf_v[i-v2];
							}
//							v2 = n_var;
//							v3=0;
//							v3 = n_var;
							v2 = n_var;

							get_token();  //']' or ',' or ')'
							if (*token==',')
							{
//							 v2 = v3;
//							 v3 = 0;
							 index_buf=0;
							 index_op=0;
							 n_var=0;
							 type_eval=1;
							 memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
							 parse_exp((float *)buf_v);
							 for(i=v2;i<7;i++)
							 {
							  buf2[i] = buf_v[i-v2];
							 }
							 v3 = n_var;
							 get_token();  //')'
							}
							get_token();  //'='
						}

										index_buf=0;
										index_op=0;
										n_var=0;
										type_eval=1;
										memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
										parse_exp((float *)buf_v);
						write_cod(command,n_var,1,temp,0,v2,(char *)buf_v,0,v3,(char *)buf2,0);
						break;
		case KEYWORD:
					switch (tok) {

						case DIM:
						case INTEGER_TYPE:
						case BYTE_TYPE:
						case STRING_TYPE:
						case LONG_TYPE:
						case FLOAT_TYPE:
						case REM:
						case PHONE:
										prog++;
										 get_comment(mycomment);
										//fance get_comment(pedit->comment);
										write_cod(tok,0,0,"",0,0,"",0,0,"",0);
										break;
						case PRINT:
									  n_var=0;
									  i=0;
									  get_token();
									  if (token_type==NL)
									  {
												buf_v[i++].v=NL;
												n_var++;
									  }
									  v1=0;
									  while (token_type!=NL && tok!=FINISHED && token_type!=KEYWORD)
									  {
										if (token_type==STRING)
										{
													buf_v[i].v=STRING;
													strcpy(&mycomment[v1],token);
													//fance strcpy(&pedit->comment[v1],token);
													v1 += strlen(token)+1;
										}
                                        else
                                        {
                                            if (!strcmp(strupr(token), "DATE"))
                                                buf_v[i].v = _DATE;
                                            else if (!strcmp(strupr(token), "PTIME"))
                                                buf_v[i].v = PTIME;
                                            else	if (!strcmp(strupr(token), "USER_A"))
                                                buf_v[i].v = USER_A;
                                            else if (!strcmp(strupr(token), "USER_B"))
                                                buf_v[i].v = USER_B;
                                            else	if (!strcmp(strupr(token), "BEEP"))
                                                buf_v[i].v = BEEP;
                                            else if (token_type == IDENTIFIER)
                                            {
                                                buf_v[i].v = IDENTIFIER;
                                                strcpy(buf_v[i].var, token);
                                            }
                                        }
										n_var++;
										i++;
										get_token();
										if (*token==';')
										 {
											get_token();
											if (token_type==NL)
												break;
											if (token_type==KEYWORD)
												putback();
										 }

										if (token_type==NL)
												{
												buf_v[i++].v=NL;
												n_var++;
												if (eol==NL) 
												{--prog;--prog;}
												else 
													--prog;
												}
									}
										write_cod(PRINT,n_var,0,"",0,1,(char *)buf_v,0,0,(char *)"",0);
										break;
						case CLEARX:
						case ENDPRG:
						case RETURN:
						case HANGUP:
										write_cod(tok,0,0,"",0,0,"",0,0,"",0);
										break;
					case ELSE:
										return;
						case SET_PRINTER:
						case STARTPRG:
						case STOP:
						case OPEN:
						case CLOSE:
						case DISABLEX:
						case ENABLEX:
										id = tok;
										get_token() ;
										write_cod(id,1,1,token,0,0,"",0,0,"",0);
										break;
						case RUN_MACRO:
										eval_exp(&value);
										write_cod(RUN_MACRO,0,0,"",value,0,"",0,0,"",0);
										break;
						case ON:
									 index_buf=0;
									 index_op=0;
									 n_var=0;
									 type_eval=1;
									 memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
									 i=0;
									 parse_exp((float *)buf_v);
									 get_token();
									 if (tok == GOTO) fvar1 = GOTO;
									 else fvar1=GOSUB;
									 i=0;
									 while ((token_type != NL) && tok!=FINISHED) 
									 {
											eval_exp(&value);
											nrlin[i++]=(int)value;
											get_token();
										}
										write_cod(ON,n_var,i,(char *)nrlin,fvar1,1,(char*)buf_v,0,0,"",0);

										break;
						case GOTO:
						case ON_ERROR:
						case ON_ALARM:
						case GOSUB:
										id = tok;
										eval_exp(&value) ;
										line_table[lline-1].go_to=1;
										go_to[index_go_to].line=lline;
										go_to[index_go_to++].go_to=value;
										write_cod(id,1,0,"",value,0,"",0,0,"",0);
										break;
						case Alarm:
										char c;
										index_buf=0;
										index_op=0;
										n_var=0;
										n_var1=0;
										type_eval=1;
										memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));

									if ((p=(char *)strchr(prog,'<'))!=NULL)
										c=LT;
									else
										if ((p=(char *)strchr(prog,'>'))!=NULL)
											c=GT;
									*p=',';


										parse_exp((float *)buf_v);
										n_var1=n_var;
										get_token();
										parse_exp((float *)buf_v);
										n_var2=n_var;
										get_token();
										parse_exp((float *)&buf_v[index_buf]);
										get_token();
//  treb rez probl spatiului
										prog++;
										get_comment(mycomment);
										//fance get_comment(pedit->comment);
										write_cod(Alarm,n_var,c,"",0,1,(char*)buf_v,0,0,"",0);

										break;

						case PRINT_AT:
						case ALARM_AT:
										id = tok;
										get_token();
										if(token_type==IDENTIFIER)
												write_cod(PRINT_ALARM_AT_ALL,0,0,"",0,0,"",0,0,"",0);
										n_var=0;
										if(token_type==NUMBER)
										{
//											putback();
//											eval_exp(&fvar1) ;
#ifndef NOMATH
											fvar1=atof(token);
#else
											fvar1=atofproc(token);
#endif
											v1=0;
											*var1=0;
											n_var++;
											get_token();
											if (token_type!=NL)
												{
//												 putback();
//												 eval_exp(&fvar2) ;
#ifndef NOMATH
												 fvar2=atof(token);
#else
												 fvar2=atofproc(token);
#endif
												 v2=0;
												 *var2=0;
												 n_var++;
												 get_token();
												 if (token_type!=NL)
														{
//														 putback();
//														 eval_exp(&fvar3) ;
#ifndef NOMATH
														 fvar3=atof(token);
#else
														 fvar3=atofproc(token);
#endif
														 v3=0;
														 *var3=0;
														 n_var++;
														}
												}
										write_cod(PRINT_ALARM_AT,n_var,v1,var1,fvar1,v2,var2,fvar2,v3,var3,fvar3);
									 }
									 break;
						 case CALLB:
									 get_token();
									 strcpy(var1,token);
									 index_buf=0;
									 index_op=0;
									 n_var=0;
									 type_eval=1;
									 memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
									 i=0;
									 get_token();
									 while ((token_type != NL) && tok!=FINISHED)
										 {
											parse_exp((float *)&buf_v[index_buf]);
											argcall[i++]=n_var;
											get_token();
										 }
									 argcall[i]=0;
									 write_cod(CALLB,n_var,1,var1,0,i,(char*)buf_v,0,0,"",0);
									 break;
						 case DALARM:
									index_buf=0;
									index_op=0;
									n_var=0;
									type_eval=1;
									memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
									parse_exp((float *)buf_v);
									get_token();
									eval_exp(&fvar1) ;
									get_token();
									prog++;
									 get_comment(mycomment);
									//fance get_comment(pedit->comment);
									write_cod(DALARM,n_var,0,"",fvar1,1,(char*)buf_v,0,0,"",0);
									break;
							case DECLARE:
									n_var=0;
									i=0;
									get_token();
									while ((token_type != NL) && tok!=FINISHED)
									 {
											buf_v[i].v=1;
											strcpy(buf_v[i].var,token);
											i++;
											n_var++;
											get_token();
									 }
									 write_cod(DECLARE,n_var,0,"",0,1,(char*)buf_v,0,0,"",0);
									 break;
							case REMOTE_SET:
							case REMOTE_GET:
									id = tok;
									get_token();
									v1=1;
									strcpy(var1,token);
									get_token();
									get_token();
									v2=1;
									strcpy(var2,token);
									write_cod(id,0,1,var1,0,1,var2,0,0,"",0);
									break;
							case FOR:
										for_count++;
										n_var=0;
										get_token();
										v1=1;
										strcpy(var1,token);
										index_buf=0;
										index_op=0;
										n_var=0;
										type_eval=1;
										memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
										i=0;
										get_token();
										parse_exp((float *)buf_v);
										argcall[i++]=n_var;
										get_token();
										parse_exp((float *)&buf_v[index_buf]);
										argcall[i++]=n_var;
										get_token();
										if (tok == STEP)
											{
											 parse_exp((float *)&buf_v[index_buf]);
											 argcall[i++]=n_var;
											}
										else
											 {
												buf_v[index_buf].v=0;
												buf_v[index_buf++].fvar=1;
												n_var++;
												argcall[i++]=n_var;
											 }
									 argcall[i]=0;
										write_cod(FOR,n_var,1,var1,0,i,(char*)buf_v,0,0,"",0);
										break;
								case NEXT:
										get_token() ;
										write_cod(NEXT,0,0,"",0,0,"",0,0,"",0);
										for_count--;
										break;
					case IF:
					case IFP:
					case IFM:
						fvar1 = 0;
						if (tok==IFP)
						{
						fvar1 = 1;
						}
						if (tok==IFM)
						{
						fvar1 = 2;
						}

									index_buf=0;
									index_op=0;
									n_var=0;
									ind_cod_line=0;
									type_eval=1;
									memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
									parse_exp((float *)buf_v);
									write_cod(IF,n_var,0,"",fvar1,1,(char*)buf_v,0,0,"",0);
									get_token();
									parse2_then_else();

						if (tok==ELSE)
						{
							cod_line[ind_cod_line++]=0x10;
							if(DORU_SYSTEM)
								cod_line[ind_cod_line++]=0xFF;
							next_else_else = ind_cod_line;
							ind_cod_line += 2;
							i=ncod+ind_cod_line+2;
//							cod_putint(next_then_else, i, pedit->ptrprg->type);
							memcpy(&cod_line[next_then_else],&i,2);
							parse2_then_else();
							i=ncod+ind_cod_line+2;
//							cod_putint(next_else_else, i, pedit->ptrprg->type);
							memcpy(&cod_line[next_else_else],&i,2);
						}
						else
						{
							i=ncod+ind_cod_line+2;
//							cod_putint(next_then_else, i, pedit->ptrprg->type);
							memcpy(&cod_line[next_then_else],&i,2);
							if (token_type==NL)
							  if (eol==NL) {--prog;--prog;}
							  else --prog;
						}

					if( ncod + ind_cod_line + 2 < PROGRAM_SIZE )
					{
					 ncod = ncod + ind_cod_line ;
					 memcpy(code,cod_line,ind_cod_line);
					 code += ind_cod_line;
					}
					else
					  ncod = PROGRAM_SIZE ;
					break;

					case WAIT:
									get_token();
									if(token_type == TTIME)
									 {
											write_cod(WAIT,0,1,"",ttime,0,"",0,0,"",0);
											break;
									 }
									putback();
									index_buf=0;
									index_op=0;
									n_var=0;
									type_eval=1;
									memset(buf_v, 0, MAX_VAR_LINE * sizeof(struct buf_str));
									parse_exp((float *)buf_v);
									write_cod(WAIT,n_var,0,"",0,1,(char*)buf_v,0,0,"",0);

						break;
				 }
			}
		get_token() ;
	}
} /* end of function interp_block */


void parse2_then_else()
{
	float value;
	then_else=1;
	eoi=',';
	//fance t++;
	get_token();
	if (token_type==NUMBER)
	{
		putback() ;
		eval_exp(&value) ;
		line_table[lline-1].go_to=1;
		go_to[index_go_to].line=lline;
		go_to[index_go_to++].go_to=value;
		write_cod(GOTOIF,1,0,"",value,0,"",0,0,"",0);
		get_token();
		//fance t--;
		then_else=0;
		eoi=NL;
		return;
	}
	putback() ;
	prescan2();
	//fance t--;
	then_else=0;
	eoi=NL;
}


void  get_comment(char *comment)
{
	int i=0;
	while( i++ < 79 && *prog!=13 && *prog)
	{
		*(comment++)=*(prog++);
	}
	while( *prog!=13 && *prog )
	{
		prog++;
	}
	//	if (*prog)
	//		prog=prog+2;
	*comment=0x0;
}


void write_cod(int type,int n_var, int v1, char *var1, float fvar1,
																	 int v2, char *var2, float fvar2,
																	 int v3, char *var3, float fvar3)
{
//char *p;
int *buf1;
struct buf_str *buf,*buf2;
int /*int_var1,*/n,i,j;
long lval;
switch (type) 
{
case ASSIGN:
case ASSIGNAR:
case ASSIGNARRAY_1:
case ASSIGNARRAY_2:
		 buf=(struct buf_str*)var2;
		 buf2=(struct buf_str*)var3;
		 if (!then_else)
		 {
			put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
		 {
			 cod_line[ind_cod_line++]=type;
/*
			 if(v3)
				cod_line[ind_cod_line++]=ASSIGNAR;
			 else
				cod_line[ind_cod_line++]=ASSIGN;
*/
			 ind_cod_line=pcodvar(0,v1,var1,fvar1,NULL,ind_cod_line);
			 if(ind_cod_line == -2)
			 {
				 error = 1;
				 return;
			 }
			 if(v2)
			 {
				for(i=0;i<v2;i++)
				{
				 ind_cod_line=pcodvar(0,buf2[i].v,buf2[i].var,buf2[i].fvar,buf2[i].op,ind_cod_line);
				}
				cod_line[ind_cod_line++]=0xFF;
				if(v3)
				{
				 for(i=v2;i<v2+v3;i++)
				 {
				  ind_cod_line=pcodvar(0,buf2[i].v,buf2[i].var,buf2[i].fvar,buf2[i].op,ind_cod_line);
				 }
				 cod_line[ind_cod_line++]=0xFF;
				}
//				cod_line[ind_cod_line++]=0xFF;
			 }

/*
			for(i=0;i<v3;i++)
			{
				 ind_cod_line=pcodvar(0,buf2[i].v,buf2[i].var,buf2[i].fvar,buf2[i].op,ind_cod_line);
			}
*/
		 }
		 for(i=0;i<n_var;i++)
		 {
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
		 }
/*
		 if (!DORU_SYSTEM)
			 cod_line[ind_cod_line++]='\x09';
*/
		 if (then_else)
			 {
				cod_line[ind_cod_line++]=0xFF;
				return;
			 }
		 len_cod_linie = ind_cod_line ;
		 break;
 case DIM:
 case INTEGER_TYPE:
 case BYTE_TYPE:
 case STRING_TYPE:
 case LONG_TYPE:
 case FLOAT_TYPE:
 case PHONE:
 case REM:
		 if (!then_else)
		 {
			put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=type;
		 //cod_line[ind_cod_line++]=strlen(pedit->comment);
		 //strcpy(&cod_line[ind_cod_line],pedit->comment);
		 //ind_cod_line += strlen(pedit->comment);	Fance
		 cod_line[ind_cod_line++]=strlen(mycomment);
		 strcpy(&cod_line[ind_cod_line],mycomment);
		 ind_cod_line += strlen(mycomment);
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line ;
		 break;
 case PRINT:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=PRINT;
		 cod_line[ind_cod_line++]=(char)n_var;
		 j=0;
		 for(i=0;i<n_var;i++)
		 switch (buf[i].v) 
		 {
		 case STRING:
				 cod_line[ind_cod_line++]=STRING;
				  n = strlen(&mycomment[j]);
				 //n = strlen(&pedit->comment[j]);fance
				 cod_line[ind_cod_line++]=n;
				/* strcpy(&cod_line[ind_cod_line],&pedit->comment[j]);*/
				 strcpy(&cod_line[ind_cod_line],&mycomment[j]);
				 ind_cod_line += n;
				 j += n+1;
//				 strcpy(&cod_line[ind_cod_line],buf[i].var);
//				 ind_cod_line +=  strlen(buf[i].var)+1;
				 break;
		 case _DATE:
				 cod_line[ind_cod_line++]=_DATE;
				 break;
		 case PTIME:
				 cod_line[ind_cod_line++]=PTIME;
				 break;
		 case USER_A:
				 cod_line[ind_cod_line++]=USER_A;
				 break;
		 case USER_B:
				 cod_line[ind_cod_line++]=USER_B;
				 break;
		 case BEEP:
				 cod_line[ind_cod_line++]=BEEP;
				 break;
		 case IDENTIFIER:
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,NULL,ind_cod_line);
				 break;
		 case NL:
				 cod_line[ind_cod_line++]=STRING;
				 cod_line[ind_cod_line++]=2;
				 cod_line[ind_cod_line++]=0x0D;
				 cod_line[ind_cod_line++]=0x0A;
//				 cod_line[ind_cod_line++]='\x00';
				 break;
		 }
		 if (then_else)
			{
			 cod_line[ind_cod_line++]=0xFF;
			 return;
			}
		 len_cod_linie = ind_cod_line;
		 break;
 case CLEARX:
 case ENDPRG:
 case RETURN:
 case HANGUP:
		 if (!then_else)
		 {
				put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=type;
		 if (then_else)
			{
			 cod_line[ind_cod_line++]=0xFF;
			 return;
			}
		 len_cod_linie = ind_cod_line;
		 break;
 case OPEN:
 case CLOSE:
 case DISABLEX:
 case ENABLEX:
 case STARTPRG:
 case STOP:
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=type;
		 ind_cod_line=pcodvar(0,v1,var1,fvar1,NULL,ind_cod_line);
		 if (then_else)
			{
//			 cod_line[ind_cod_line++]='\xFF';
			 return;
			}
		 len_cod_linie = ind_cod_line;
		 break;
 case RUN_MACRO:
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=0x23;
		 cod_line[ind_cod_line++]=(char)fvar1;
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case SET_PRINTER:
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=0x28;
		 cod_line[ind_cod_line++]=*var1;
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case ON:
		 buf=(struct buf_str*)var2;
		 buf1 = (int *)var1;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=ON;
		 for(i=0;i<n_var;i++)
		 {
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
		 }
		 if (DORU_SYSTEM)
				 cod_line[ind_cod_line++]=0xFF;
		 if (fvar1==GOTO)
//			cod_line[ind_cod_line++]=0x16;
			cod_line[ind_cod_line++]=GOTO;
		 else
//			cod_line[ind_cod_line++]=0x0C;
			cod_line[ind_cod_line++]=GOSUB;
		 cod_line[ind_cod_line++]=v1;
		 line_table[lline-1].go_to=1;
		 for(i=0;i<v1;i++)
			{
			go_to[index_go_to].line=lline;
			go_to[index_go_to].go_to=buf1[i];
			//go_to[index_go_to++].pos = code-pedit->code+ind_cod_line; //Fance
			go_to[index_go_to++].pos = code-mycode+ind_cod_line;
			ind_cod_line += 2;
			}
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line ;
		 break;
 case ON_ALARM:
		 if (then_else)
		 {
		 cod_line[ind_cod_line++]=0x2B;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+ind_cod_line;//fance
		 go_to[index_go_to-1].pos = code-mycode+ind_cod_line;
		 ind_cod_line += 2;
		 return;
		 }
		 put_line_num( line_value );
		 cod_line[ind_cod_line++]=0x2B;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		/* go_to[index_go_to-1].pos = code-pedit->code+4;*///fance
		  go_to[index_go_to-1].pos = code-mycode+4;
		 cod_line[ind_cod_line++]=0;
		 cod_line[ind_cod_line++]=0;
		 len_cod_linie = ind_cod_line;
		 break;
 case ON_ERROR:
		 if (then_else)
		 {
		 cod_line[ind_cod_line++]=0x27;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		//go_to[index_go_to-1].pos = code-pedit->code+ind_cod_line;fance
		 go_to[index_go_to-1].pos = code-mycode+ind_cod_line;
		 ind_cod_line += 2;
		 return;
		 }
		 put_line_num( line_value );
		 cod_line[ind_cod_line++]=0x27;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+4;//Fance
		 go_to[index_go_to-1].pos = code-mycode+4;
		 cod_line[ind_cod_line++]=0;
		 cod_line[ind_cod_line++]=0;
		 len_cod_linie = ind_cod_line;
		 break;
 case GOTO:
		 if (then_else)
		 {
		 cod_line[ind_cod_line++]=GOTO;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+ind_cod_line;//fance
		  go_to[index_go_to-1].pos = code-mycode+ind_cod_line;
		 ind_cod_line += 2;
		 return;
		 }
		 put_line_num( line_value );
		 cod_line[ind_cod_line++]=GOTO;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+4;fance
		  go_to[index_go_to-1].pos = code-mycode+4;
		 cod_line[ind_cod_line++]=0;
		 cod_line[ind_cod_line++]=0;
		 len_cod_linie = ind_cod_line;
		 break;
 case GOTOIF:
		 if (then_else)
		 {
		 cod_line[ind_cod_line++]=0x2A;
//		 line_table[line-1].go_to_pos=(int)ftell(fp)+ind_cod_line;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+ind_cod_line;fance
		  go_to[index_go_to-1].pos = code-mycode+ind_cod_line;
		 ind_cod_line += 2;
		 cod_line[ind_cod_line++]=0xFF;
		 return;
		 }
		 break;
 case GOSUB:
		 if (then_else)
		 {
		 cod_line[ind_cod_line++]=GOSUB;
 //		 line_table[line-1].go_to_pos=(int)ftell(fp)+ind_cod_line;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+ind_cod_line;fance
		  go_to[index_go_to-1].pos = code-mycode+ind_cod_line;
		 ind_cod_line += 2;
		 return;
		 }
		 put_line_num( line_value );
		 cod_line[ind_cod_line++]=GOSUB;
//		 line_table[line-1].go_to_pos=1;
		 go_to[index_go_to-1].line=lline;
		// go_to[index_go_to-1].pos = code-pedit->code+4;fance
		 go_to[index_go_to-1].pos = code-mycode+4;
		 cod_line[ind_cod_line++]=0;
		 cod_line[ind_cod_line++]=0;
		 len_cod_linie = ind_cod_line;
		 break;
 case Alarm:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=0x41;
		 for(i=0;i<n_var1;i++)
		 {
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
		 }
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=(char)v1;
		 for(i=n_var1;i<n_var2;i++)
				{
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
				}
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=0xFF;
		 for(i=n_var2;i<n_var;i++)
		 {
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
		 }

		 if (DORU_SYSTEM)
		 {
			 cod_line[ind_cod_line++]=0xFF;
		 }

		 //cod_line[ind_cod_line++]=strlen(pedit->comment);
		 //strcpy(&cod_line[ind_cod_line],pedit->comment);
		 //ind_cod_line += strlen(pedit->comment);//fance

		 cod_line[ind_cod_line++]=strlen(mycomment);
		 strcpy(&cod_line[ind_cod_line],mycomment);
		 ind_cod_line += strlen(mycomment);

		 if (DORU_SYSTEM)
		 {
			 cod_line[ind_cod_line++] = 0;
		 }
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case PRINT_ALARM_AT_ALL:
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (id == ALARM_AT)
			 cod_line[ind_cod_line++]=0x21;
		 else
			 cod_line[ind_cod_line++]=0x1B;

		 cod_line[ind_cod_line++]=0xFF;
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case PRINT_ALARM_AT:
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (id == ALARM_AT)
			 cod_line[ind_cod_line++]=0x21;
		 else
			 cod_line[ind_cod_line++]=0x1B;
		 cod_line[ind_cod_line++]=(int)fvar1;
		 if (n_var>=2)
			 cod_line[ind_cod_line++]=(int)fvar2;
		 if (n_var==3)
			 cod_line[ind_cod_line++]=(int)fvar3;
		 cod_line[ind_cod_line++]=0;
		 if(n_var==3)
			 ind_cod_line = 7;
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case CALLB:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
		 {
			 cod_line[ind_cod_line++]=0x2F;
			 find_var_def(var1);
			 cod_line[ind_cod_line++]=(char)(vars_table[cur_index].num_point-1);
			 cod_line[ind_cod_line++]=v2;
		 }
		 j=0;
		 for(i=0;i<n_var;i++)
		 {
				 ind_cod_line=pcodvar(NAME,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
				 if(DORU_SYSTEM)
					 if (argcall[j]==i+1)
							{
							 j++;
							 cod_line[ind_cod_line++]=0xFF;
							}
//				 n = n + 3 + (buf[i].v^'\x01')*2;
		 }


		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case DALARM:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=0x43;
		 for(i=0;i<n_var;i++)
		 {
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
		 }
		 if (DORU_SYSTEM)
			cod_line[ind_cod_line++]=0xFF;

		 lval = (long) (fvar1*1000);
//		 cod_putlong(ind_cod_line, lval, pedit->ptrprg->type);
		 memcpy(&cod_line[ind_cod_line],&lval,4);
		 ind_cod_line += 4;
		 dalarm[index_dalarm].line=lline;


		 //cod_line[ind_cod_line++]=strlen(pedit->comment);
		 //strcpy(&cod_line[ind_cod_line],pedit->comment);
		 //ind_cod_line += strlen(pedit->comment);//Fance
		 cod_line[ind_cod_line++]=strlen(mycomment);
		 strcpy(&cod_line[ind_cod_line],mycomment);
		 ind_cod_line += strlen(mycomment);

		 if (DORU_SYSTEM)
		 {
//			 cod_putlong(ind_cod_line, lval, pedit->ptrprg->type);
			 memcpy(&cod_line[ind_cod_line],&lval,4);
			 ind_cod_line += 4;
		 }

		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case DECLARE:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=0x30;
		 cod_line[ind_cod_line++]=(char)n_var;

		 for(i=0;i<n_var;i++)
				{
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,NULL,ind_cod_line);
				}
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case REMOTE_GET:
 case REMOTE_SET:
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (type == REMOTE_SET)
				cod_line[ind_cod_line++]=0x22;
		 else
				cod_line[ind_cod_line++]=0x24;
		 if (type == REMOTE_SET)
				 ind_cod_line=pcodvar(0,v1,var1,fvar1,NULL,ind_cod_line);
		 else
				 ind_cod_line=pcodvar(NAME,v1,var1,fvar1,NULL,ind_cod_line);

		 ind_cod_line=pcodvar(0,v2,var2,fvar2,NULL,ind_cod_line);
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line;
		 break;
 case FOR:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=FOR;
		 ind_cod_line=pcodvar(NAME,v1,var1,fvar1,NULL,ind_cod_line);
		 j=0;
		 for(i=0;i<n_var;i++)
				{
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
				 if(DORU_SYSTEM)
					 if (argcall[j]==i+1)
							{
							 j++;
							 cod_line[ind_cod_line++]=0xFF;
							}
//				 n = n + 3 + (buf[i].v^'\x01')*2;
				}
/*
		 if (!DORU_SYSTEM)
			 cod_line[ind_cod_line++]='\x0B';
*/
		 line_table[lline-1].for_count=for_count;
		// line_table[lline-1].for_pos=code-pedit->code+ind_cod_line;fance
		 line_table[lline-1].for_pos=code-mycode+ind_cod_line;
		 ind_cod_line += 2;
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line ;
		 break;
 case NEXT:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 cod_line[ind_cod_line++]=NEXT;
		 line_table[lline-1].for_count=for_count;

		 line_table[lline-1].next_pos=code-mycode+ind_cod_line;
		 // line_table[lline-1].next_pos=code-pedit->code+ind_cod_line;fance
		 ind_cod_line += 2;
		 if (then_else)
				 return;
		 len_cod_linie = ind_cod_line ;
		 break;
 case IF:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
		 {
			 if (fvar1==0)
				 cod_line[ind_cod_line++]=0x0E;
			 if (fvar1==1)
				 cod_line[ind_cod_line++]=0x11;
			 if (fvar1==2)
				 cod_line[ind_cod_line++]=0x12;
		 }
		 for(i=0;i<n_var;i++)
		 {
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
		 }
		 if (DORU_SYSTEM)
				 cod_line[ind_cod_line++]=0xFF;

		 if (fvar1==0)
		 {
/*
			if (!DORU_SYSTEM)
				 cod_line[ind_cod_line++]='\x0E';
*/
			next_then_else = ind_cod_line;
			cod_line[ind_cod_line++]=0;
			cod_line[ind_cod_line++]=0;
			return;
		 }
		 if (fvar1==1)
		 {
			if (DORU_SYSTEM)
				cod_line[ind_cod_line++]=0;   //    ifP
/*
			else
				cod_line[ind_cod_line++]='\x11';
*/
			next_then_else = ind_cod_line;
			cod_line[ind_cod_line++]=0;
			cod_line[ind_cod_line++]=0;
/*
			if (!DORU_SYSTEM)
			{
				cod_line[ind_cod_line++]='\x01';
				cod_line[ind_cod_line++]='\x0';
			}
*/
			return;
		 }
		 if (fvar1==2)
		 {
			if (DORU_SYSTEM)
					cod_line[ind_cod_line++]=0;   //    ifm
/*
			else
					cod_line[ind_cod_line++]='\x12';
*/
			next_then_else = ind_cod_line;
			cod_line[ind_cod_line++]=0;
			cod_line[ind_cod_line++]=0;
/*
			if (!DORU_SYSTEM)
			{
				cod_line[ind_cod_line++]='\x0';
				cod_line[ind_cod_line++]='\x01';
			}
*/
			return;
			}

 case WAIT:
		 buf=(struct buf_str*)var2;
		 if (!then_else)
		 {
			 put_line_num( line_value );
		 }
		 if (DORU_SYSTEM)
			 cod_line[ind_cod_line++]=0x1E;
		 if(v1)
		 {
				cod_line[ind_cod_line++]=0xA1;
				if (DORU_SYSTEM)
				{
//					cod_putlong(ind_cod_line, ttimed, pedit->ptrprg->type);
					memcpy(&cod_line[ind_cod_line], &ttimed, 4);
				}
/*
				else
					memcpy(&cod_line[ind_cod_line], &fvar1, 4);
*/
				ind_cod_line += 4;
		 }
		 else
		 {
			for(i=0;i<n_var;i++)
			{
				 ind_cod_line=pcodvar(0,buf[i].v,buf[i].var,buf[i].fvar,buf[i].op,ind_cod_line);
			}
			if (DORU_SYSTEM)
			  cod_line[ind_cod_line++]=0xFF;
		 }
		 if (DORU_SYSTEM)
		 {
			
			 time_count ++;
			 memcpy(&cod_line[ind_cod_line],&time_count,4);
			 ind_cod_line = ind_cod_line + 4;
			 //cod_line[ind_cod_line++]=0;
			 //cod_line[ind_cod_line++]=0;
			 //cod_line[ind_cod_line++]=0;
			 //cod_line[ind_cod_line++]=0;
		 }

/*
		 if (!DORU_SYSTEM)
		 {
			cod_line[ind_cod_line++]='\x1E';
			wait[index_wait][0] = code-pedit->code+ind_cod_line;
			ind_cod_line += 2;
			wait[index_wait++][1]=code-pedit->code+ind_cod_line+2-2;
		 }
*/
		 if (then_else)
		 {
			 cod_line[ind_cod_line++]=0xFF;
			 return;
		 }
		 len_cod_linie = ind_cod_line ;
		 break;
 }
 if( ncod + len_cod_linie + 2 < PROGRAM_SIZE )
 {
  ncod = ncod + len_cod_linie ;
  memcpy(code,cod_line,len_cod_linie);
  code += len_cod_linie;
 }
 else
 {
	 sntx_err(TOTAL_2000_BREAK); //编码长度超过2000个字节 返回提示信息;
	 error = TOTAL_2000_BREAK;
	 return;
  ncod = PROGRAM_SIZE;
 }
}

void put_line_num( int line_value )
{
	cod_line[0]=0x01;
	memcpy(&cod_line[1],&line_value,2) ;
	ind_cod_line = 3;
}

int find_var_def(char *var_name)
{
 int i,/*j,*/num_net,num_panel,num_point,k;
 byte point_type,var_type;
 var_type = 0;
 k = 0;
 unsigned char sub_panel_number = 0;
 /*ispoint(var_name,&num_point,&var_type, &point_type,&num_panel,&num_net,pedit->network,pedit->panel, &k);*///fance
 //ispoint(var_name,&num_point,&var_type, &point_type,&num_panel,&num_net,my_network,my_panel, &k);
 ispoint_ex(var_name,&num_point,&var_type,&point_type,&num_panel,&num_net,my_network,sub_panel_number,my_panel,&k);
 for (i=0; i<index_vars_table; i++)
		if(!strcmp(vars_table[i].name , var_name))
		{
			cur_index = i;
		}
return 0;
}

int pcodvar(int cod,int v,char *var,float fvar,char *op,int Byte)
{
 long lvar;
 int i,ind;
 Point_Net point;
		 ind = Byte;
		 if (v>0)
		 {
				find_var_def(var);
				if (vars_table[cur_index].type == LOCAL_VAR)
				{
					 if (DORU_SYSTEM)
					 {
						 cod_line[Byte++]=vars_table[cur_index].var_type;
						 int t=0,j,k=0;
						 i=0;

						 for(j=0;j<ind_local_table; )
						 {
							switch((unsigned char)local_table[j])
							{
								case FLOAT_TYPE:
								case LONG_TYPE:
									k = 4;
									break;
								case INTEGER_TYPE:
									k = 2;
									break;
								case BYTE_TYPE:
									k = 1;
									break;
								default:
								  {
									switch((unsigned char)local_table[j])
									{
									case FLOAT_TYPE_ARRAY:
									case LONG_TYPE_ARRAY:
										k = 4;
										break;
									case INTEGER_TYPE_ARRAY:
										k = 2;
										break;
									case BYTE_TYPE_ARRAY:
									case STRING_TYPE:
//									case STRING_TYPE_ARRAY:
										k = 1;
										break;
								 }
								 if( (*((int *)&local_table[j+1])) )
									 k = 4+(*((int *)&local_table[j+1])) * (*((int *)&local_table[j+3])) * k;
								 else
									 k = 4 + (*((int *)&local_table[j+3])) * k;
								}
								break;
							}
							t = j+1;
							j += 1 + k;

							if(j>(4000+300+sizeof(struct variable_table)*MAX_Var+
								sizeof(struct line_table_str)*MAX_LINE+sizeof(struct buf_str)*MAX_VAR_LINE+
								sizeof(go_to_str)*MAX_GOTO))
							{
								error = 2;
								return 0;
							}

							if( !strcmp(&local_table[j], vars_table[cur_index].name) )
							{
								if( vars_table[cur_index].c)
								{
								 t += 4;
								}
								memcpy(&cod_line[Byte], &t, 2);
								Byte += 2;
								break;
							}
							j += 1+strlen(&local_table[j]);
						 }
						 if (j == ind_local_table)
						 {
							 return -2;
							//exit(1);//Fance
						 }
					 }

				}
				else
					if ((vars_table[cur_index].type == POINT_VAR) || (vars_table[cur_index].type == LABEL_VAR))
					{
                        if (((unsigned char)vars_table[cur_index].network >= 128) && (( ((unsigned char)vars_table[cur_index].point_type) & 0x1F) != MB_REG)) //network 的最高位用来标识  是否是新的数据格式
                        {
                            unsigned char high_3bit;
                            cod_line[Byte++] = REMOTE_POINT_PRG;
                            if (((unsigned char)vars_table[cur_index].point_type == COIL_REG) ||
                                ((unsigned char)vars_table[cur_index].point_type == DIS_INPUT_REG) ||
                                ((unsigned char)vars_table[cur_index].point_type == INPUT_REG) ||
                                ((unsigned char)vars_table[cur_index].point_type == MB_REG) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_BV) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_BI) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_AV) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_AI) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_AO) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_DO))
                            {
                                point.number = (unsigned char)((vars_table[cur_index].num_point) & 0x00ff);
                                high_3bit = ((vars_table[cur_index].num_point) & 0xff00) >> 3;
                            }
                            else
                            {
                                point.number = (unsigned char)((vars_table[cur_index].num_point - 1) & 0x00ff);
                                high_3bit = ((vars_table[cur_index].num_point - 1) & 0xff00) >> 3;

                            }

                            point.point_type = (vars_table[cur_index].point_type + 1) | high_3bit;
                            //*point_type = *point_type | high_3bit;



                            point.panel = vars_table[cur_index].panel;
                            point.sub_panel = vars_table[cur_index].sub_panel;
                            point.network = vars_table[cur_index].network;
                            memcpy(&cod_line[Byte], &point, sizeof(Point_Net));
                            Byte += sizeof(Point_Net);
                        }
						else if( ( ( (unsigned char)vars_table[cur_index].panel == Station_NUM ) && (  (unsigned char)vars_table[cur_index].sub_panel == Station_NUM  )  )  ||
                            (   (unsigned char)vars_table[cur_index].panel == Station_NUM ) && (  (unsigned char)vars_table[cur_index].sub_panel == 0 ) )
						{
							if((vars_table[cur_index].point_type & 0x1F) == 2/*ENUM_VAR*/ )
							{
								if(vars_table[cur_index].num_point > BAC_VARIABLE_ITEM_COUNT)
								{
									error = 1;
									sntx_err(VARIABLE_BREAK);
									return -2;
								}
							}
							else if((vars_table[cur_index].point_type & 0x1F) == 0/*ENUM_OUT*/ )
							{
								if(vars_table[cur_index].num_point > BAC_OUTPUT_ITEM_COUNT)
								{
									error = 1;
									sntx_err(OUTPUT_BREAK);
									return -2;
								}
							}
							else if((vars_table[cur_index].point_type & 0x1F) == 1/*ENUM_IN*/ )
							{
								if(vars_table[cur_index].num_point > BAC_INPUT_ITEM_COUNT)
								{
									error = 1;
									sntx_err(INPUT_BREAK);
									return -2;
								}
							}

							cod_line[Byte++]=LOCAL_POINT_PRG;
							point.number     = vars_table[cur_index].num_point-1;
							point.point_type = vars_table[cur_index].point_type+1;
							memcpy(&cod_line[Byte],&point,sizeof(MyPoint));
							Byte += sizeof(MyPoint);
						}
						else if(( (unsigned char)vars_table[cur_index].panel == Station_NUM ) && ((unsigned char)vars_table[cur_index].sub_panel != Station_NUM )  && 
                                 ((unsigned char)vars_table[cur_index].sub_panel != 0 ))	//Minipanel 下面的TSTAT
						{
							unsigned char high_3bit = 0;
							cod_line[Byte++]=REMOTE_POINT_PRG;
                            unsigned char temp_point_type = (unsigned char) (vars_table[cur_index].point_type & 0x1F);
							if((temp_point_type == COIL_REG) ||
								(temp_point_type == DIS_INPUT_REG) ||
								(temp_point_type == INPUT_REG) ||
								(temp_point_type == MB_REG) ||
                                (temp_point_type == BAC_BV) ||
                                (temp_point_type == BAC_BI) ||
								(temp_point_type == BAC_AV) ||
								(temp_point_type == BAC_AI) ||
								(temp_point_type == BAC_AO) ||
								(temp_point_type == BAC_DO))
							{
								point.number     = (unsigned char)((vars_table[cur_index].num_point) & 0x00ff);
								high_3bit = ((vars_table[cur_index].num_point) & 0xff00) >> 3;
							}
							else
							{
                                point.number = (unsigned char)((vars_table[cur_index].num_point-1) & 0x00ff);
								if(vars_table[cur_index].num_point > 0)
									high_3bit = ((vars_table[cur_index].num_point-1) & 0xff00) >> 3;
								else
								{

								}
								
							}
							point.point_type = (vars_table[cur_index].point_type+1) | high_3bit ;
							//*point_type = *point_type | high_3bit;
							point.panel = vars_table[cur_index].panel;
							point.sub_panel = vars_table[cur_index].sub_panel ;
							point.network = vars_table[cur_index].network;
							memcpy(&cod_line[Byte],&point,sizeof(Point_Net));
							Byte += sizeof(Point_Net);
						}
						else
						{
							unsigned char high_3bit;
							cod_line[Byte++]=REMOTE_POINT_PRG;
							if(((unsigned char)vars_table[cur_index].point_type == COIL_REG) ||
								((unsigned char)vars_table[cur_index].point_type == DIS_INPUT_REG) ||
								((unsigned char)vars_table[cur_index].point_type == INPUT_REG) ||
								((unsigned char)vars_table[cur_index].point_type == MB_REG) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_BV) ||
                                ((unsigned char)vars_table[cur_index].point_type == BAC_BI) ||
								((unsigned char)vars_table[cur_index].point_type == BAC_AV) ||
								((unsigned char)vars_table[cur_index].point_type == BAC_AI) ||
								((unsigned char)vars_table[cur_index].point_type == BAC_AO) ||
								((unsigned char)vars_table[cur_index].point_type == BAC_DO))
							{
								point.number     = (unsigned char)((vars_table[cur_index].num_point) & 0x00ff);
								high_3bit = ((vars_table[cur_index].num_point) & 0xff00) >> 3;
							}
							else
							{
								point.number     = (unsigned char)((vars_table[cur_index].num_point-1) & 0x00ff);
								high_3bit = ((vars_table[cur_index].num_point-1) & 0xff00) >> 3;
								
							}
						
							point.point_type = (vars_table[cur_index].point_type+1) | high_3bit ;
							//*point_type = *point_type | high_3bit;



							point.panel = vars_table[cur_index].panel;
							point.sub_panel = vars_table[cur_index].sub_panel ;
							point.network = vars_table[cur_index].network;
							memcpy(&cod_line[Byte],&point,sizeof(Point_Net));
							Byte += sizeof(Point_Net);
						}
						#if 0
					 if( vars_table[cur_index].panel == pedit->panel )
					 {
						cod_line[Byte++]=LOCAL_POINT_PRG;
						point.number     = vars_table[cur_index].num_point-1;
						point.point_type = vars_table[cur_index].point_type+1;
						memcpy(&cod_line[Byte],&point,sizeof(MyPoint));
						Byte += sizeof(MyPoint);
					 }
					 else
					 {
						cod_line[Byte++]=REMOTE_POINT_PRG;
						point.putpoint( vars_table[cur_index].num_point-1, vars_table[cur_index].point_type+1, vars_table[cur_index].panel-1, vars_table[cur_index].network);
						memcpy(&cod_line[Byte],&point,sizeof(Point_Net));
						Byte += sizeof(Point_Net);
					 }
						#endif
					}
		 }
		 else
			if (v<0)
			 cod_line[Byte++]=var[0];
			else
			{
			 cod_line[Byte++]=0x9D;
			 lvar = (long)( fvar * 1000 + 0.1 );
//			 cod_putlong(Byte, lvar, pedit->ptrprg->type);
			 memcpy(&cod_line[Byte],&lvar,4);
			 Byte += 4;
			}
		i=0;
		if (op!=NULL)
			while(op[i] && i<MAX_OP)
			{
			  switch (op[i])
			  {
				 case '+':	 cod_line[Byte++]=PLUS;break;
				 case '-':	 cod_line[Byte++]=MINUS;break;
				 case '*':	 cod_line[Byte++]=MUL;break;
				 case '/':	 cod_line[Byte++]=DIV;break;
				 case '%':	 cod_line[Byte++]=MOD;break;
				 case '^':	 cod_line[Byte++]=POW;break;
				 case '&':   cod_line[Byte++]=BIT_AND;break;
				 case '|':   cod_line[Byte++]=BIT_OR;break;
				 case LT:
				 case LE:
				 case GT:
				 case GE:
				 case NE:
				 case EQ:
				 case OR:
				 case XOR:
				 case AND:
				 case NOT:
				 case MINUSUNAR:
				 case ABS:
				 case INKEYD:
				 case OUTPUTD:
				 case _INT:
				 case LN:
				 case LN_1:
				 case SQR:
				 case _Status:
         case RUNTIME:
				 case CONPROP:
				 case CONRATE:
				 case CONRESET:
				 case Tbl:
				 case WR_ON:
				 case WR_OFF:
				 case SENSOR_ON:
				 case SENSOR_OFF:
				 case TIME_FORMAT:
				 case ASSIGNARRAY_1:
				 case ASSIGNARRAY_2:
							cod_line[Byte++]=op[i];break;
				 case ASSIGNARRAY:
						 memmove(&cod_line[Byte-2], &cod_line[Byte-3], 3);
						 cod_line[Byte-3]=op[i];
						 Byte++;
						 break;
				 case ASSIGNAR:
						 cod_line[Byte++]=0xFF;
						 break;
				 case TIME_ON:
				 case TIME_OFF:
							  {
								int j=0,t=0;
								char *p;
								while (j<ind_time_table)
								{
								 if(!memcmp(&time_table[j], &cod_line[ind], Byte-ind))
								 {
									t = 1;
									break;
								 }
								 if(( p = (char *)memchr(&time_table[j], 0xff, ind_time_table - j)) )
									 j += (p - &time_table[j])+1+5;
								 else
									break;
								}

								if (!t)
								{
									memcpy(&time_table[ind_time_table], &cod_line[ind], Byte-ind);
									ind_time_table += Byte-ind;
									time_table[ind_time_table++] = 0xff;
									ind_time_table++;
								}
								Byte = ind;
								cod_line[Byte++]=op[i++];
								if (!t)
								{
//									cod_putint(Byte, ind_time_table, pedit->ptrprg->type);
									memcpy(&cod_line[Byte], &ind_time_table, 2);
								}
								else
								 {
									 p = (char *)memchr(&time_table[j], 0xff, ind_time_table - j);
									 p++;
									 j = (p - &time_table[0])+1;
//									 cod_putint(Byte, j, pedit->ptrprg->type);
									 memcpy(&cod_line[Byte], &j, 2);
								 }

								Byte += 2;
								if (!t)
									ind_time_table += 4;
/*
								cod_line[Byte++]=op[i++];
								cod_line[Byte++]=op[i++];
								cod_line[Byte++]=op[i++];
								cod_line[Byte++]=op[i];
*/
								}
								break;
				 case INTERVAL:
								cod_line[Byte++]=op[i++];
								lvar = LONGTIMETEST;
//								cod_putlong(Byte, lvar, pedit->ptrprg->type);
								memcpy(&cod_line[Byte], &lvar, 4);
								Byte += 4;
								i += 3;
								break;
				 case AVG:
				 case MAX:
				 case MIN:
				 case COM_1:
								cod_line[Byte++]=op[i++];
								cod_line[Byte++]=op[i];
								break;
			  }
			  i++;
			}
			return Byte;
}

char *rtrim(char *text)
{
	int n,i;
	n=strlen(text);
	for (i=n-1;i>=0;i--)
		if (text[i]!=' ')
			break;
	text[i+1]='\0';
	return text;
}



char * buf;
char my_display[10240];

char my_input_code[1024];
char *pcode, *ptimebuf;
char *local;
extern int bac_program_size;
extern int bac_free_memory;
char *desassembler_program()
{
unsigned char cod;//,xtemp[15];
 int /*point,*//*type_var,*//*ind,*/i;
 int len,nitem;//lvar;
 long lval;
 eoi=0;
 ind_line_array = 0;
 index_wait=0;
 code = mycode;
//Edit by Fance
 buf = my_display;
 memset(my_display,0,10240);
 index_stack = stack;
 int n;
 buf[0]=0;
 int bytes = 0;

  int code_length = ((unsigned char)code[1])*256 + (unsigned char)code[0];
 if((code_length == 0) || (code_length > 2000)) //如果传过来的是无效的 大于500的编码 就说明是错的 直接清空;
	 return my_display;
 bac_program_size = code_length;
 bac_free_memory = 2000 - bac_program_size;
 pcode=code+2;
 memcpy(&bytes,code,2);
 //adjustint(&bytes, 0);
 
 bytes += 2+3;
 code += bytes;
#ifndef Fance
 memcpy(&ind_local_table,code,2);    //local var
#endif
 local = code+2;  
 
  #ifndef Fance
// adjustint(&bytes, ptrprg->type);
 code += ind_local_table + 2;
  ptimebuf = code + 2;      //timer	Fance
 memcpy(&bytes,code,2);
// adjustint(&bytes, ptrprg->type);
 code += bytes + 2;

 memcpy(&ind_remote_local_list,code,2);
 #endif
// adjustint(&ind_remote_local_list, ptrprg->type);
#ifdef Fance
 memcpy(remote_local_list,code+2,ind_remote_local_list*sizeof(/*struct*/ remote_local_list));
#endif
 code = pcode;
 int then_else = 0;
 while(((unsigned char)*code)!=0xFE)
 {
	 if(!then_else)
	 {
		 if ((unsigned char)*code!=0x01)
		 {
			//printf("ERROR!!!!Desassambler!!!!!!!!!!!!!\n");
			return NULL;
		 }
		memcpy(&lline,++code,2);
//		adjustint(&lline, ptrprg->type);
		itoa(lline, buf,10);
		code += 2;
		buf += strlen(buf);
		*buf++=' ';
	 }
	 else
		 if ((unsigned char)*code==0x01)
				{
				 then_else = 0;
				 buf -= 3;
				 *buf++=0x0d;
				 *buf++=0x0a;
				 continue;
				}
//				 return;          // return when else is finished



	if (*code == ELSE) buf -= 2;
	if (*code != ASSIGN && *code != ASSIGNAR || *code != ASSIGNARRAY_1 || *code != ASSIGNARRAY_2)
	{
			strcpy(buf,look_instr(*code));
			buf += strlen(buf);
			*buf++=' ';
	}

	switch ((unsigned char)*code++) {
			case ASSIGNARRAY_1:
			case ASSIGNARRAY_2:
			case ASSIGNAR:
			case ASSIGN:
									i = *(code-1);
									n=desvar();
									buf += strlen(buf);
/*
									if(i == ASSIGNAR)
									{
									 *buf++ = '[';
									 desexpr();
									 buf += strlen(buf);
									 *buf++ = ']';
									}
									else
*/
									{
									 if(i == ASSIGNARRAY_1 || i == ASSIGNARRAY_2)
									 {
									  *buf++ = '(';
									  desexpr();
									  if(n>1)
									  {
										buf += strlen(buf);
										strcpy(buf," , ");
										buf += strlen(buf);
									   desexpr();
									  }
//									  desexpr();
									  buf += strlen(buf);
									  *buf++ = ')';
									 }
									}
									*buf++=' ';
									*buf++='=';
									*buf++=' ';
									desexpr();
									buf += strlen(buf);
									break;

			case STARTPRG:
			case OPEN:
			case ENABLEX:
			case STOP:
			case CLOSE:
			case DISABLEX:
//									desexpr();
									desvar();
									buf += strlen(buf);
									break;
			case PHONE:
			case REM:
			case DIM:
			case INTEGER_TYPE:
			case BYTE_TYPE:
			case STRING_TYPE:
			case LONG_TYPE:
			case FLOAT_TYPE:
									len = *code++;
									memcpy(buf,code,len);
									buf += len;
									code += len;
									break;
			case PRINT:
								  {
									nitem = *code++;
									i=0;
									for(int j=0;j<nitem;j++)
									 {
										switch ((unsigned char)*code++) {
											case _DATE:
																strcpy(buf,"DATE;");
																buf += 5;
																break;
											case PTIME:
																strcpy(buf,"TIME;");
																buf += 5;
																break;
											case BEEP:
																strcpy(buf,"BEEP;");
																buf += 5;
																break;
											case USER_A:
																strcpy(buf,"USER_A;");
																buf += strlen("USER_A;");
																break;
											case USER_B:
																strcpy(buf,"USER_B;");
																buf += strlen("USER_B;");
																break;
											case STRING:
																len = *code++;
																if ((unsigned char)*code=='\r')
																{
																 buf--;
																}
																else
																{
																	*buf++='"';
																	memcpy(buf,code,len);
																	buf += len;
																	*buf++='"';
																	*buf++=';';
																}
																code += len;
																break;

											default:
																code--;
																desexpr();
																buf += strlen(buf);
																*buf++=';';
																break;
									 }
									}
								  }
								  break;
			case CLEARX:              // clear all local variables to zero
			case ENDPRG:
			case RETURN:
			case HANGUP:
									break;
			case SET_PRINTER:
									*buf++ = *code;
									code++;
									break;
			case RUN_MACRO:
									itoa((unsigned char)*code,buf,10);
									buf += strlen(buf);
									code++;
									break;
			case ON:
									desexpr();
									buf += strlen(buf);
									*buf++ = ' ';
//									if (*code++==0x0c)   //gosub
									if ((unsigned char)*code++==GOSUB)   //gosub
									{
											strcat(buf,"GOSUB");
											buf += 5;
									}
									else
									{
											strcat(buf,"GOTO");
											buf += 4;
									}
									*buf++ = ' ';
									nitem = *code++;
									for(i=0; i<nitem;i++)
										{
										 memcpy(&n,code,2);
//										 adjustint(&n, ptrprg->type);
//									 itoa(*((int *)&pcode[n-2+1]),buf,10);
										 itoa(*((int *)&pcode[n+1]),buf,10);
										 buf += strlen(buf);
										 *buf++ = ',';
										 code += 2;
										}
									buf--;
									break;
			case GOSUB:
			case ON_ALARM:
			case ON_ERROR:
			case GOTO:
			case GOTOIF:
									 cod = *(code-1);
									 unsigned short temp_n;
									 memcpy(&temp_n,code,2);
									 itoa(*((unsigned short *)&pcode[temp_n-2+1]),buf,10);
									 buf += strlen(buf);
									 code += 2;
									 if(cod == GOTOIF)
										 code++;    //FF
									 //memcpy(&n,code,2);
//									 adjustint(&n, ptrprg->type);
#if fance_debug
									 itoa(*((int *)&pcode[n-2+1]),buf,10);
									 buf += strlen(buf);
									 code += 2;
									 if(cod == GOTOIF)
											code++;    //FF
#endif
									break;
			case Alarm:
/*									if ( ind_line_array && line_array[ind_line_array-1][0]!=line)
										 {
											line_array[ind_line_array][0] = line;
											line_array[ind_line_array++][1] = code-pcode+2;
										 }
*/
									char c,*p;
									if ((p=(char *)memchr(code,LT,30))!=NULL)
										c=LT;
									else
										if ((p=(char *)memchr(code,GT,30))!=NULL)
											c=GT;
									if(p == NULL)
									{
										error = 1;
										return NULL;
									}
									*p='\xFF';
									desexpr();
									buf += strlen(buf);
									if (c==LT)
										strcpy(buf," < ");
									else
										strcpy(buf," > ");
									buf += strlen(buf);
									desexpr();
									buf += strlen(buf);
									strcpy(buf," , ");
									buf += strlen(buf);
									desexpr();
									buf += strlen(buf);
									strcpy(buf," , ");
									buf += strlen(buf);
									len = *code++;
									memcpy(buf,code,len);
									buf += len;
									code += len;
									code++;
									*p=c;
									break;
			case DALARM:
	/*								if ( ind_line_array && line_array[ind_line_array-1][0]!=line)
										 {
											line_array[ind_line_array][0] = line;
											line_array[ind_line_array++][1] = code-pcode+2;
										 }
	*/
									desexpr();
									buf += strlen(buf);
									strcpy(buf," , ");
									buf += strlen(buf);
									memcpy(&lval,code,4);
//									adjustlong((unsigned long *)&lval, ptrprg->type);
									code += 4;
/*									desexpr();
									buf += strlen(buf);
*/
									itoa(lval/1000,buf,10);
									buf += strlen(buf);
									strcpy(buf," , ");
									buf += strlen(buf);
									len = *code++;
									memcpy(buf,code,len);
									buf += len;
									code += len;
									code += 4;
									break;
			case ALARM_AT:
			case PRINT_AT:
									if ((unsigned char)*code=='\xFF')
										 {
											strcpy(buf,"ALL");
											buf += 3;
											code++;
										 }
									else {
												i=1;
												while(((unsigned char)*code) && i++<=3)
													{
													 itoa(*code,buf,10);
													 buf += strlen(buf);
													 *buf++=' ';
													 code++;
													}
												if(*code==0) code++;
											 }
									break;
			case CALLB:
									strcpy(buf,"PRG");
									buf += strlen("PRG");
									itoa(((unsigned char)*code++)+1,buf,10);
									buf += strlen(buf);
									strcpy(buf," = ");
									buf += strlen(buf);
									nitem = (unsigned char)*code++;
									for(i=0;i<nitem;i++)
									 {
										desexpr();
										buf += strlen(buf);
										if (i!=nitem-1)
											{
											 strcpy(buf," , ");
											 buf += strlen(buf);
											}
									 }
									break;
			case DECLARE:
									nitem = (unsigned char)*code++;
									for(i=0; i<nitem; i++)
									 {
										desvar();
										buf += strlen(buf);
										*buf++=' ';
									 }
									break;
			case REMOTE_GET:
			case REMOTE_SET:
										desvar();
										buf += strlen(buf);
										*buf++='=';
										desvar();
										buf += strlen(buf);
									break;
			case FOR:
										desvar();
										buf += strlen(buf);
										strcpy(buf," = ");
										buf += strlen(buf);
										desexpr();
										buf += strlen(buf);
										strcpy(buf," TO ");
										buf += 4;
										desexpr();
										buf += strlen(buf);
										strcpy(buf," STEP ");
										buf += 6;
										desexpr();
										buf += strlen(buf);
										code += 2;
									break;
			case NEXT:
									p=code;
									memcpy(&n,code,2);
//									adjustint(&n, ptrprg->type);
									code=pcode+n-2+4;
									desvar();
									buf += strlen(buf);
									code = p + 2;
									break;
			case IF:
			case IFP:
			case IFM:
									cod = *(code-1);
									then_else = 0;
									desexpr();
									if( cod == IFP || cod == IFM)
											code++;
									buf += strlen(buf);
									strcpy(buf," THEN ");
									buf += strlen(" THEN ");
									code += 2;

									then_else = 1;
									continue;

			case ELSE:
									code += 3;
									continue;
			case WAIT:
/*									if ( ind_line_array && line_array[ind_line_array-1][0]!=line)
										 {
											line_array[ind_line_array][0] = line;
											line_array[ind_line_array++][1] = code-pcode+2;
										 }
*/
									if ((unsigned char)*code==0xA1)
										 {
											unsigned long k;
											memcpy(&k,++code,4);
//									      adjustlong(&k, ptrprg->type);
											intervaltotext_old(buf, k , 0 , 0 );
#ifdef Fance
											intervaltotext(buf, k , 0 , 0 );
#endif
											buf += strlen(buf);
											code += 4;
											}
									else
										 {
											desexpr();
											buf += strlen(buf);
										 }
									code += 4;
									break;
	 }
	if(then_else)
		 {
			 strcpy(buf," , ");
			 buf += strlen(buf);
		 }
	else
		 {
			*buf++=0x0d;
			*buf++=0x0a;
		 }
	}
if (then_else)
	 {
		buf -= 3;
		*buf++=0x0d;
		*buf++=0x0a;
	 }

*buf=0;

//mxyputs(1,1,editbuf);
//mgets(buf+1,1, Black, White);

return buf;
}

char *look_instr( char cod )
{ int i ;
/* convert to lower case */
/* see if token is in table */
for( i = 0 ; *table[i].command ; i++ )
	if (table[i].tok==cod ) return table[i].command;
return "" ; /* unkown command */
}



void ftoa(float f, int length, int ndec, char *buf)
{
	int i,n;
	char xxxtmp[50],*p,c;
	for(i=0;i<length-ndec;i++)
		buf[i]=' ';
	buf[length-ndec-1]='.';
	for(i=length-ndec;i<length;i++)
		buf[i]='0';
	if(f>1000000.00 || f<-1000000.00 || (f>-1.e-6 && f<1.e-6) )
		gcvt(f, 5, xxxtmp);
	else
		gcvt(f, length, xxxtmp);
	if ((p=strchr(xxxtmp, 'e'))!=NULL  ||  (p=strchr(xxxtmp, 'E'))!=NULL)
		strcpy(buf+length-strlen(xxxtmp),xxxtmp);
	else
	{
		c='.';
		if ((p=strchr(xxxtmp, c))!=NULL)
		{
			memcpy(&buf[length-ndec],p+1,min(ndec,(int)strlen(p+1)));
		}
		else
			p=&xxxtmp[strlen(xxxtmp)];
		n=min(p-&xxxtmp[0],length-ndec-1);
		memcpy(&buf[length-ndec-1]-n,p-n,n);
	}
	buf[length]=0;
}


int desvar(void)
{
 char *b,q[30], *p/*, *r*/;
 Point_T3000 point;
 byte point_type,var_type;
 int num_point,num_panel,num_net,k;
 unsigned char sub_panel = 0;
 p=buf;
 if ((unsigned char)*code == ASSIGNARRAY )
 {
	code++;
 }
 if ((unsigned char)*code >= 0x82 && (unsigned char)*code <= 0x9B)
 {
	 long n = 0 ;
    n = *((unsigned short *)(code + 1));
  //n = *((int *)(code+1));  //杜帆屏蔽  实际只有两个字节  我把它改为 short
  int t=0,j,k = 0,l,c,m = 0;//lc,cc;

  l = c = 0;
  for(j=0;j<ind_local_table; )
  {
      switch ((unsigned char)local[j])
	  //switch(local[j])  //dufan 屏蔽
	  {
			case FLOAT_TYPE:
			case LONG_TYPE:
					k = 4;
					break;
			case INTEGER_TYPE:
					k = 2;
					break;
			case BYTE_TYPE:
					k = 1;
					break;
			default:
				  {
					switch((unsigned char)local[j])
					{
						case FLOAT_TYPE_ARRAY:
						case LONG_TYPE_ARRAY:
								k = 4;
								break;
						case INTEGER_TYPE_ARRAY:
								k = 2;
								break;
						case BYTE_TYPE_ARRAY:
						case STRING_TYPE:
//						case STRING_TYPE_ARRAY:
								k = 1;
								break;
					 }
					 memcpy(&l, &local[j+1], 2);
					 memcpy(&c, &local[j+3], 2);
					 if(l)
						k *= l*c;
					 else
					  k *= c;
					 j += 4;
					}
					break;
		}
		m = j + 1;
		j += 1 + k;


#if 1
        //此功能用来保存 Local var  变色的功能。
        char temp_local_value[40];
        memset(temp_local_value, 0, 40);
        strcpy(temp_local_value, &local[j]);
        bool find_exsit = false;
        int next_add_count = 0;
        for (int xy = 0; xy < 40; xy++)
        {
            if (strlen(local_var_new_name[xy]) <= 0)
            {
                next_add_count = xy;
                break;
            }
            int compare_ret = strcmp(local_var_new_name[xy], temp_local_value);
            if (compare_ret == 0)
            {
                find_exsit = true;
                next_add_count = xy;
                break;
            }
        }

        if (find_exsit == false)
        {
            strcpy(local_var_new_name[next_add_count], temp_local_value);
        }
#endif

		if( n < j )
		{
		 strcpy(p, &local[j]);
		 p += strlen(p);
		 break;
		}

		j += 1+strlen(&local[j]);
	}
  code += 3;
  return l+1;
 }
 else 
	 if (((unsigned char)*code) == 0x9c/*(byte)*//*LOCAL_POINT_PRG*/)
	{
//#ifdef Fance_recode
//				*((MyPoint *)&point) = *((MyPoint *)++code);
//				point.panel = panel-1;
//				pointtotext(q, &point);
//				code += sizeof(MyPoint);
//				k=0;
//				strcpy(buf,ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel, &k));
//				if( (point.point_type-1) == AY )
//				{
//				  b = buf;
//				  strcat(buf, "[");
//				  buf += strlen(buf);
//				  desexpr();
//				  strcat(buf, "]");
//				  buf = b;
//				}
//#endif

				*((MyPoint *)&point) = *((MyPoint *)++code);
				//point.panel = 0;
				point.panel = Station_NUM;
				pointtotext(q, &point);
				code += sizeof(MyPoint);
				k=0;
				//strcpy(buf,ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel, &k));
				char *temp_label =NULL;
				temp_label = ispoint_ex(q,&num_point,&var_type, &point_type, &num_panel, &num_net, my_network,sub_panel, point.panel, &k);
				if(temp_label != NULL)
					strcpy(buf,temp_label);

				//strcpy(buf,ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, my_network, point.panel, &k));
				if( (point.point_type-1) == AY )
				{
					b = buf;
					strcat(buf, "[");
					buf += strlen(buf);
					desexpr();
					strcat(buf, "]");
					buf = b;
				}
  }
else
  if (((unsigned char)*code) == (byte)REMOTE_POINT_PRG)
	 {
				++code;
				Point_Net temp_point;
				memcpy(&temp_point,code,sizeof(Point_Net));
				b = code;
				memset(q,0,17);
				if(temp_point.point_type>0)
					temp_point.point_type = temp_point.point_type - 1;
				pointtotext(q,&temp_point);
				code += sizeof(Point_Net);
				k=0;
			//	strcpy(buf,ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel, &k));//Fance
			
				char *temp_p =NULL;
				temp_p = ispoint_ex(q,&num_point,&var_type, &point_type, &num_panel, &num_net, my_network,sub_panel, temp_point.panel, &k);	
				if(temp_p == NULL)	//没有获取到，就算失败
				{
					return 0;
				}
			//	char *temp_p = ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, my_network, my_panel, &k);
				//if(temp_p == NULL)	//没有获取到，就算失败
				//{
				//	return 0;
				//}
				strcpy(buf,temp_p);
				if( (((Point_Net *)b)->point_type-1) == AY )
				{
					b = buf;
				  strcat(buf, "[");
				  buf += strlen(buf);
				  desexpr();
				  strcat(buf, "]");
				  buf = b;
				}
	 }
	 else 
		 if (((unsigned char)*code) == CONST_VALUE_PRG)
		 {
						float f;
						long l;
						memcpy(&l,++code,4);
//						adjustlong((unsigned long *)&l, ptrprg->type);
						f = (float)l / 1000.;
						code += 4;
						if((unsigned char)*code == TIME_FORMAT)
						{
							code++;
							intervaltotext_old(buf, (long)f, 0, 0);
							//#ifdef Fance
							//intervaltotext(buf, (long)f, 0, 0);
							//#endif

						}
						else
						{
							if((long)f==f && (f<1000000 && f>-1000000 )) //原始的;  但是 有问题; （long）f 一直不等于f
							{
								ltoa((long)f,buf,10);
							}
							else
							{
								//ftoa(f, 15, 2, buf);//original
								sprintf(buf,"%.3f",f);
							//	ftoa(f, 35, 2, buf);//Fance modify
								ltrim(buf);
							}
						}
		 }
		 else
			return 0;
 return 1;
}


int pointtotext_for_controller(char *buf,Point_T3000 *point)
{
	char x[4];
	byte num,panel,point_type;
	num=point->number;
	panel=point->panel;
	point_type=point->point_type;
	if (point_type==0)
	{
		buf[0]=0;
		return 1;
	}
	if(point_type > 16)
	{
		buf[0]=0;
		return 1;
	}
	strcpy(buf,"");
#ifdef Fance_Enable
	strcpy(buf,itoa(panel+1,x,10));
	if(panel+1<10 || num+1 < 100)
		strcat(buf,"-");
#endif
	//strcat(buf,lin);
	if(point_type>27)
		return 0;

	//	ptr_panel.info[point_type-1].name = "VAR";
	strcat(buf,ptr_panel.info[point_type].name);	
	//	strcat(buf,ptr_panel->info[point_type-1].name);
	strcat(buf,itoa(num,x,10));
	return 0;
}

int pointtotext(char *buf,Point_T3000 *point)
{
	char x[4];
	byte num,panel,point_type;
	num=point->number;
	panel=point->panel;
	point_type=point->point_type;
	if (point_type==0)
	{
		buf[0]=0;
		return 1;
	}

	if(point_type > BAC_DO)
	{
		buf[0]=0;
		return 1;
	}

	strcpy(buf,"");
#ifdef Fance_Enable
	strcpy(buf,itoa(panel+1,x,10));
	if(panel+1<10 || num+1 < 100)
		strcat(buf,"-");
#endif
		//strcat(buf,lin);


//	ptr_panel.info[point_type-1].name = "VAR";
	strcat(buf,ptr_panel.info[point_type-1].name);	
	//	strcat(buf,ptr_panel->info[point_type-1].name);
	strcat(buf,itoa(num+1,x,10));
	return 0;
}



int pointtotext(char *buf,Point_Net *point)
{
	char x[6];
	byte panel,point_type;
	unsigned short num;
	unsigned char sub_panel = 0;
	int net;
	num=point->number;

	int high_3_bit = (point->point_type & 0xe0) >> 5;

	num= (point->number) + high_3_bit*256  ;

    //if ((point->point_type == COIL_REG) ||
    //    (point->point_type == DIS_INPUT_REG) ||
    //    (point->point_type == INPUT_REG) ||
    //    (point->point_type == MB_REG) ||
    //    (point->point_type == BAC_AV) ||
    //    (point->point_type == BAC_AI) ||
    //    (point->point_type == BAC_AO) ||
    //    (point->point_type == BAC_DO))
    //{
    //    num_point = num_point;
    //}
    //else
    //{
    //    if (num_point > 0)
    //        num_point = num_point - 1;
    //}


	panel=point->panel;
	point_type= (point->point_type ) & 0x1F;
    if ((point_type == MB_REG) && (point->network >=128))
    {
        num = (point->number)  + ((point->network -128)*8 + high_3_bit)*256;
    }
	sub_panel = point->sub_panel;
	if(point_type > BAC_MAX)
	{
		error = -6;
		return -6;
	}
	net = point->network;
	//if (point_type==0)
	//{
	//	buf[0]=0;
	//	return 1;
	//}

	//Fance  先不考虑 net
	//if((net!=0xFFFF) && (net != 0xff))
	//{
	//	strcpy(buf,itoa(net,x,10));
	//	strcat(buf,".");
	//}
	//else
	//{
	//	buf[0] = 0;
	//}
	ptr_panel.info[0].name = "OUT";
	ptr_panel.info[1].name = "IN";
	ptr_panel.info[2].name = "VAR";

	//if((point->panel == point->sub_panel) && (point->panel == Station_NUM))
	//{
	//	//不愿意 看到minipanel 显示为1-1-var10   如果panel是1的情况 直接显示var10 或者它的label;
	//	strcat(buf,ptr_panel.info[point_type].name);//Fance
	//	strcat(buf,itoa(num+1,x,10));
	//	return 0;
	//}

    if (((point->panel != 0) && (point->sub_panel == 0)) ||
        ((point->panel == point->sub_panel) && (point->panel == Station_NUM)))
    {
        //不愿意 看到minipanel 显示为1-1-var10   如果panel是1的情况 直接显示var10 或者它的label;
        strcat(buf, itoa(point->panel, x, 10));
        strcat(buf, ptr_panel.info[point_type].name);//Fance
        if ((point_type == BAC_AV) ||
            (point_type == BAC_AI) ||
            (point_type == BAC_AO) ||
            (point_type == BAC_DO) ||
            (point_type == BAC_BV) ||
            (point_type == BAC_BI) ||
            (point_type == COIL_REG) ||
            (point_type == DIS_INPUT_REG) ||
            (point_type == INPUT_REG) ||
            (point_type == MB_REG) )
        {
            strcat(buf, itoa(num, x, 10));
        }
        else
            strcat(buf, itoa(num + 1, x, 10));
        return 0;
    }

    if ((point->network >= 128) && (point_type != MB_REG)) // 说明是新的格式，最高位用来标识.
    {
        unsigned int temp_value;
        temp_value = ((point->network & 0x7F) << 16) + (point->panel << 8) + (point->sub_panel);
        char temp_object_instance[10];
        memset(temp_object_instance, 0, 10);
        if (temp_value < 256)
        {
            strcpy(buf, "INS");
            strcat(buf + 1, itoa(temp_value, temp_object_instance, 10));
        }
        else
            strcat(buf, itoa(temp_value, temp_object_instance, 10));
        if ((point->panel != point->sub_panel) && (point_type == VAR))
            strcat(buf, "REG");//Fance	曰了狗了 ，3-25-VAR100  非要支持  3-25-REG100.
        else
            strcat(buf, ptr_panel.info[point_type].name);//Fance

        point->point_type = (point->point_type) & 0x1F;
        if ((point->point_type == COIL_REG) ||
            (point->point_type == DIS_INPUT_REG) ||
            (point->point_type == INPUT_REG) ||
            (point->point_type == MB_REG) ||
            (point->point_type == BAC_BV) ||
            (point->point_type == BAC_BI) ||
            (point->point_type == BAC_AV) ||
            (point->point_type == BAC_AI) ||
            (point->point_type == BAC_AO) ||
            (point->point_type == BAC_DO))
        {
            strcat(buf, itoa(num, x, 10));
        }
        else
            strcat(buf, itoa(num + 1, x, 10));
        return 0;

    }

	strcat(buf,itoa(panel,x,10));
	//strcat(buf,itoa(panel+1,x,10));//2015-03-03修改，从此panel number 不在减一;
	//if(panel+1<10 || num+1 < 100)
	//	strcat(buf,"-");
	//if(panel+1<10 || num+1 < (8*256))
		strcat(buf,".");
	strcat(buf,itoa(sub_panel,x,10));
	strcat(buf,".");
		//strcat(token,ptr_panel.info[point_type-1].name);	//Fance
	if((point->panel != point->sub_panel) && (point_type == VAR))
		strcat(buf,"REG");//Fance	曰了狗了 ，3-25-VAR100  非要支持  3-25-REG100.
	else
		strcat(buf,ptr_panel.info[point_type].name);//Fance
	//这4个比较特别  是0基址;
    point->point_type = (point->point_type) & 0x1F;
	if((point->point_type == COIL_REG) ||
		(point->point_type == DIS_INPUT_REG) ||
		(point->point_type == INPUT_REG) ||
		(point->point_type == MB_REG) ||
        (point->point_type == BAC_BV) ||
        (point->point_type == BAC_BI) ||
		(point->point_type == BAC_AV) ||
		(point->point_type == BAC_AI) ||
		(point->point_type == BAC_AO) ||
		(point->point_type == BAC_DO) )
	{
		strcat(buf,itoa(num,x,10));
	}
	else
		strcat(buf,itoa(num+1,x,10));
	return 0;
}


void push(char *buf)
{
	memcpy(index_stack,buf,strlen(buf)+1);
	index_stack += strlen(buf)+1;
}

char *pop(void)
{
	--index_stack;
	--index_stack;
	while (*index_stack && index_stack >= stack) index_stack--;
	index_stack++;
	return(index_stack);
}

int isdelimit(char c)
{
	if (strchr( "\x1\xFF\xFE" , c) )
		return 1;
	else
		return 0;
}

char *look_func( char cod )
{ int i ;
/* convert to lower case */
/* see if token is in table */
for( i = 0 ; *func_table[i].func_name ; i++ )
	if (func_table[i].tok==cod ) return func_table[i].func_name;
return "" ; /* unkown command */
}



int	desexpr(void)
{
 char *op1,*op2,*op;
 char oper[10],last_oper,par,opar;
 int /*point,*/i;
 char n;
 char stack_par[300];
 char ind_par;
// index_stack = stack;
// set_semaphore_dos();Fance
 op1 = new char [200];
 op2 = new char [200];
 op = new char [200];
 memset(op1 ,0,200);
 memset(op2 ,0,200);
 memset(op1 ,0,200);
// clear_semaphore_dos();Fance

 ind_par=0;
 desvar();
 push(buf);
 stack_par[ind_par++]=0;
 last_oper=0;
 while( !isdelimit(*code))         // && code < )
 {
	switch ((unsigned char)*code++) {
		case PLUS:
							 strcpy(oper," + ");
							 par=1;
							 break;
		case MINUS:
							 strcpy(oper," - ");
							 par=1;
							 break;
		case POW:
							 strcpy(oper," ^ ");
							 par=0;
							 break;
		case MUL:
							 strcpy(oper," * ");
							 par=0;
							 break;
		case DIV:
							 strcpy(oper," / ");
							 par=0;
							 break;
		case MOD:
							strcpy(oper," % ");
							par=0;
							break;
		case BIT_AND:
							strcpy(oper," & ");
							par=0;
							break;
		case BIT_OR:
							strcpy(oper," | ");
							par=0;
							break;
		case XOR:
							 strcpy(oper," XOR ");
							 par=0;
							 break;
		case OR:
							 strcpy(oper," OR ");
							 par=0;
							 break;
		case AND:
							 strcpy(oper," AND ");
							 par=0;
							 break;
		case MINUSUNAR:
		case NOT:
							 par=0;
							 if(*(code-1)==NOT)
								{
								 strcpy(op1," NOT ");
								 strcpy(op2,pop());
								 if( strchr(op2,' ') )
								 {
									strcat(op1,"( ");
									strcat(op1,op2);
									strcat(op1," )");
								 }
								 else
									strcat(op1,op2);
								}
							 else
								{
								 strcpy(op1," -");
								 strcat(op1,pop());
								}
							 ind_par--;
							 push(op1);
							 stack_par[ind_par++]=0;
							 break;
		case GT:
							 strcpy(oper," > ");
							 par=0;
							 break;
		case GE:
							 strcpy(oper," >= ");
							 par=0;
							 break;
		case LT:
							 strcpy(oper," < ");
							 par=0;
							 break;
		case LE:
							 strcpy(oper," <= ");
							 par=0;
							 break;
		case EQ:
							 strcpy(oper," = ");
							 par=0;
							 break;
		case NE:
							 strcpy(oper," <> ");
							 par=0;
							 break;
				 case ABS:
				 case SENSOR_ON:
				 case SENSOR_OFF:
				 case _INT:
				 case INTERVAL:
				 case LN:
				 case LN_1:
				 case SQR:
				 case _Status:
				 case RUNTIME:
							 par=0;
								strcpy(op1,look_func(*(code-1)));
								strcat(op1,"( ");
								strcat(op1,pop());
								ind_par--;
								strcat(op1," )");
								push(op1);
								stack_par[ind_par++]=0;
							//	#ifdef Fance
								if (*(code-1) == INTERVAL || *(code-1) == TIME_ON || *(code-1) == TIME_OFF )
								{
									if (*(code-1) == TIME_ON || *(code-1) == TIME_OFF )
										//											if ( ind_line_array && line_array[ind_line_array-1][0]!=line)
									{
									//	line_array[ind_line_array][0] = (unsigned int)((long)FP_SEG(code)*16+(long)FP_OFF(code)-(long)(FP_SEG(pcode)*16+(long)FP_OFF(pcode)) + 2);
									//	memcpy(&line_array[ind_line_array++][1],code-3,2);
									}
									code += 4;
								}
							//	#endif
								break;
				 case TIME_ON:
				 case TIME_OFF:
					// #ifdef Fance
							 char *oldcode, *oldbuf;
							 par=0;
							 strcpy(op1,look_func(*(code-1)));
							 strcat(op1,"( ");
							 oldcode = code;
							 oldbuf = buf;
							// code = ptimebuf+ *((int *)code);
							 code = ptimebuf+ (unsigned char)*((char *)code);//Fance Chagned
							 code -= 3;
							 
							 while( ((unsigned char)*code) != 0xff && code >= ptimebuf)
							 {
								 code--;
							 }
							 if (code < ptimebuf) code = ptimebuf;
							 else code += 6;
							 buf = op2;
							 desexpr();
							 buf = oldbuf;
							 code = oldcode;
							 strcat(op1,op2);
							 strcat(op1," )");
							 push(op1);
							 stack_par[ind_par++]=0;
							 if (*(code-1) == INTERVAL || *(code-1) == TIME_ON || *(code-1) == TIME_OFF )
									 {
										if (*(code-1) == TIME_ON || *(code-1) == TIME_OFF )
//											if ( ind_line_array && line_array[ind_line_array-1][0]!=line)
											 {
												//line_array[ind_line_array][0] = (unsigned int)((long)FP_SEG(code)*16+(long)FP_OFF(code)-(long)(FP_SEG(pcode)*16+(long)FP_OFF(pcode)) + 2);
												line_array[ind_line_array][0] = (unsigned int)((long)((unsigned char)*code)*16+(long)((unsigned char)*code)-(long)(((unsigned char)*pcode)*16+(long)((unsigned char)*pcode)) + 2);
												memcpy(&line_array[ind_line_array++][1],code-3,2);
											  }
										code += 2;
									 }
					//		 #endif
								break;
				 case COM_1:
				 case AVG:
				 case MAX:
				 case MIN:
				 case INKEYD:
				 case	OUTPUTD:
				 case CONPROP:
				 case CONRATE:
				 case CONRESET:
				 case Tbl:
				 case WR_ON:
				 case WR_OFF:
				         {
								par=0;
								if (*(code-1)==AVG || *(code-1)==MIN || *(code-1)==MAX  || *(code-1)==COM_1)
								{
										 i = *(code-1);
										 n = *code++;
								}
								else
								{
										 i = *(code-1);
										 n = 2;
								}
								strcpy(op1,look_func(i));
								strcat(op1,"( ");
								op2[0]=0;
								if( *(code-1)==INKEYD )
								{
								  strcpy(op,pop());
								}
								for(int i=0;i<n;i++)
								 {
									strcpy(op,op2);
									strcpy(op2,pop());
									ind_par--;
									if(ind_par < 0)
										ind_par = 0;
									strcat(op2," , ");
									strcat(op2,op);
									if (i==n-1)
										{
										 strcpy(&op2[strlen(op2)-2],")");
										}
									}
								 strcat(op1,op2);
								 push(op1);
								 stack_par[ind_par++]=0;
								}
								break;
				case SUN:
				case MON:
				case TUE:
				case WED:
				case THU:
				case FRI:
				case SAT:
				case JAN:
				case FEB:
				case MAR:
				case APR:
				case MAY:
				case JUN:
				case JUL:
				case AUG:
				case SEP:
				case OCT:
				case NOV:
				case DEC:
				case DOM:
				case DOW:
				case DOY:
				case MOY:
				case POWER_LOSS:
				case SCANS:
				case TIME:
				case USER_A:
				case USER_B:
				case UNACK:
						 par=0;
						strcpy(op1,look_func(*(code-1)));
						push(op1);
						stack_par[ind_par++]=0;
						break;
				case ASSIGNARRAY:
						desexpr();
						buf += strlen(buf);
                  break;
				case ASSIGNARRAY_1:
				case ASSIGNARRAY_2:
						i=desvar();
						strcpy(op1,buf);
						strcat(op1,"( ");
						if(i>1)
						{
						 strcpy(op2,pop());
						 strcat(op1,pop());
						 strcat(op1," , ");
						 strcat(op1,op2);
						}
						else
						{
						 strcat(op1,pop());
//						 strcat(op2,pop());
						}
						strcat(op1," )");
						push(op1);
						break;
		default:
							par=0;
							code--;
							if(!desvar())
								{
								 strcpy(buf,pop());


								 return 1;
								}
							push(buf);
							stack_par[ind_par++]=0;
							last_oper=0;
							continue;
	}
	switch (*(code-1)) {
		case PLUS:
		case MINUS:
		case MUL:
		case DIV:
		case MOD:
		case OR:
		case AND:
		case GT:
		case GE:
		case LT:
		case LE:
		case EQ:
		case NE:
		case BIT_AND:
		case BIT_OR:
							{
							 opar=stack_par[--ind_par];
							 if ((*(code-1)==MUL || *(code-1)==DIV || *(code-1)==MOD || *(code-1)==MINUS) && opar)
								 {
									 strcpy(op2,"(");
									 strcat(op2,pop());
									 strcat(op2,")");
								 }
							 else
									 strcpy(op2,pop());
							 opar=stack_par[--ind_par];
							 if ((*(code-1)==MUL || *(code-1)==DIV || *(code-1)==MOD) && opar)
								 {
									 strcpy(op1,"(");
									 strcat(op1,pop());
									 strcat(op1,")");
								 }
							 else
									 strcpy(op1,pop());

							 strcat(op1,oper);
							 strcat(op1,op2);

							 push(op1);
							 stack_par[ind_par++]=par;
							 last_oper=1;
							}
							 break;
		default: break;
		}

 }
 if (((unsigned char)*code)==0xFF) code++;
 strcpy(buf,pop());
// set_semaphore_dos();fance
 delete op1;
 delete op2;
 delete op;
// clear_semaphore_dos();fance
// pop();//Fance
 return 0;
// return 0;
}

char *ltrim(char *text)
{
	int n,i;
	n=strlen(text);
	for (i=0;i<n;i++)
		if (text[i]!=' ')
			break;
	strcpy(text,text+i);
	return text;
}

void Init_table_bank()
{
//	strcpy(ptr_panel.outputs[1].label,"OOO");


	ptr_panel.table_bank[OUT] = BAC_OUTPUT_ITEM_COUNT; // set to 0 at the end
	ptr_panel.table_bank[IN] = BAC_INPUT_ITEM_COUNT; //     - || -


	ptr_panel.table_bank[VAR]       = BAC_VARIABLE_ITEM_COUNT;
	ptr_panel.table_bank[CON]       = BAC_PID_COUNT;
	ptr_panel.table_bank[WR]        = BAC_SCHEDULE_COUNT;
	ptr_panel.table_bank[AR]        = BAC_HOLIDAY_COUNT;
	ptr_panel.table_bank[PRG]       = BAC_PROGRAM_ITEM_COUNT;
	ptr_panel.table_bank[TBL]       = MAX_TABS;
	ptr_panel.table_bank[DMON]      = MAX_DIGM;
	ptr_panel.table_bank[AMON]      = MAX_ANALM;
	ptr_panel.table_bank[GRP]       = BAC_SCREEN_COUNT;
	ptr_panel.table_bank[AY]        = MAX_ARRAYS;
	ptr_panel.table_bank[ALARMM]    = MAX_ALARMS;
	ptr_panel.table_bank[UNIT]      = MAX_UNITS;
	ptr_panel.table_bank[USER_NAME] = MAX_PASSW;

	ptr_panel.info[1].name = "IN";
	ptr_panel.info[0].name = "OUT";
}


void init_info_table( void )
{
	int i;
	memset( ptr_panel.info, 0, MAX_FUNCTION_COUNT *sizeof( Info_Table ) );
	for( i=0; i<MAX_FUNCTION_COUNT + 1; i++ )
	//for( i=0; i<18; i++ )
	{
		switch( i )
		{
			case OUT:
				ptr_panel.info[i].address = (char *)ptr_panel.outputs;
				ptr_panel.info[i].str_size = sizeof( Str_out_point );
				ptr_panel.info[i].name = "OUT";
				ptr_panel.info[i].max_points = MAX_OUTS;
				break;
			case IN:
				ptr_panel.info[i].address = (char *)ptr_panel.inputs;
				ptr_panel.info[i].str_size = sizeof( Str_in_point );
				ptr_panel.info[i].name = "IN";
				ptr_panel.info[i].max_points = MAX_INS;
				break;
			case VAR:
				ptr_panel.info[i].address = (char *)ptr_panel.vars;
				ptr_panel.info[i].str_size = sizeof( Str_variable_point );
				ptr_panel.info[i].name = "VAR";
				ptr_panel.info[i].max_points = MAX_VARS;
				break;
			case CON:
				ptr_panel.info[i].address = (char *)ptr_panel.controllers;
				ptr_panel.info[i].str_size = sizeof( Str_controller_point );
				ptr_panel.info[i].name = "PID";
				ptr_panel.info[i].max_points = MAX_PIDS;
				break;
			case WR:
				ptr_panel.info[i].address = (char *)ptr_panel.weekly_routines;
				ptr_panel.info[i].str_size = sizeof( Str_weekly_routine_point );
				ptr_panel.info[i].name = "SCH";
				ptr_panel.info[i].max_points = MAX_SCHS;
				break;
			case AR:
				ptr_panel.info[i].address = (char *)ptr_panel.annual_routines;
				ptr_panel.info[i].str_size = sizeof( Str_annual_routine_point );
				ptr_panel.info[i].name = "HOL";
				ptr_panel.info[i].max_points = MAX_HOLS;
				break;
			case PRG:
				ptr_panel.info[i].address = (char *)ptr_panel.programs;
				ptr_panel.info[i].str_size = sizeof( Str_program_point );
				ptr_panel.info[i].name = "PRG";
				ptr_panel.info[i].max_points = MAX_PRGS;
				break;
			case TBL:
				ptr_panel.info[i].address = (char *)ptr_panel.analog_mon;
				ptr_panel.info[i].str_size = sizeof( Str_monitor_point );
				ptr_panel.info[i].name = "TBL";
				ptr_panel.info[i].max_points = MAX_TABS;

				#ifdef Fance_enable
				info[i].address = (char *)custom_tab;
				info[i].str_size = sizeof( Str_tbl_point );
				info[i].name = "TBL";
				info[i].max_points = MAX_TABS;
				#endif
				break;
			case DMON:
				ptr_panel.info[i].address = (char *)ptr_panel.analog_mon;
				ptr_panel.info[i].str_size = sizeof( Str_monitor_point );
				ptr_panel.info[i].name = "DMON";
				ptr_panel.info[i].max_points = MAX_TABS;
				break;
			case AMON:
				ptr_panel.info[i].address = (char *)ptr_panel.analog_mon;
				ptr_panel.info[i].str_size = sizeof( Str_monitor_point );
				ptr_panel.info[i].name = "AMON";
				ptr_panel.info[i].max_points = MAX_ANALM;
				break;
			case GRP:
				ptr_panel.info[i].address = (char *)ptr_panel.control_groups;
				ptr_panel.info[i].str_size = sizeof( Control_group_point );
				ptr_panel.info[i].name = "GRP";
				ptr_panel.info[i].max_points = MAX_GRPS;
				break;
			case AY:
				ptr_panel.info[i].address = (char *)ptr_panel.arrays;
				ptr_panel.info[i].str_size = sizeof( Str_array_point );
				ptr_panel.info[i].name = "AY";
				ptr_panel.info[i].max_points = MAX_ARRAYS;
				break;
			case ALARMM:          //12
				ptr_panel.info[i].address = (char *)ptr_panel.alarms;
				ptr_panel.info[i].str_size = sizeof( Alarm_point );
				ptr_panel.info[i].name = "ALR";
				ptr_panel.info[i].max_points = MAX_ALARMS;
				break;
			case UNIT:
				ptr_panel.info[i].address = (char *)ptr_panel.units;
				ptr_panel.info[i].str_size = sizeof( Units_element );
				ptr_panel.info[i].name = "UNITS";
				ptr_panel.info[i].max_points = MAX_UNITS;
				break;
			case USER_NAME:
				ptr_panel.info[i].address = (char *)&::ptr_panel.passwords;
				//info[i].str_size = sizeof( Password_point );
				ptr_panel.info[i].name = "PASS";
				ptr_panel.info[i].max_points = MAX_PASSW;
				break;
			case ALARMS:          //12
				ptr_panel.info[i].address = (char *)ptr_panel.alarms_set;
				ptr_panel.info[i].str_size = sizeof( Alarm_set_point );
				ptr_panel.info[i].name = "ALRS";
				ptr_panel.info[i].max_points = MAX_ALARMS_SET;
				break;
/*	case 15: {
//		  &system_info[i].name = data;
			System_Name(  ctype );
				info[i].name = "";
			break;
		}
	case 16: {
			program_code = ( byte * ) data;
			result = Program_Code( ctype, code_length, subscript );
				info[i].name = "";
			break;
		}************************/
			case WR_TIME:
				ptr_panel.info[i].address = (char *)ptr_panel.wr_times;
				ptr_panel.info[i].str_size = 9*sizeof( Wr_one_day );
				ptr_panel.info[i].name = "WR_T";
				break;
			case AR_Y:                //17
				ptr_panel.info[i].address = (char *)ptr_panel.ar_dates;
				ptr_panel.info[i].str_size = 46;
				ptr_panel.info[i].name = "AR_D";
				break;
			//case REG:
			//	//ptr_panel.info[i].address = (char *)ptr_panel.;
			//	//ptr_panel.info[i].str_size = 46;
			//	ptr_panel.info[i].name = "REG";
			//	break;
            case BAC_BI:
                ptr_panel.info[i].name = "BI";
                break;
			case COIL_REG:
				ptr_panel.info[i].name = "MB_COIL";
				break;
			case DIS_INPUT_REG:
				ptr_panel.info[i].name = "MB_DISINPUT";
				break;
			case INPUT_REG:
				ptr_panel.info[i].name = "MB_INPUTREG";
				break;
			case MB_REG:
				ptr_panel.info[i].name = "MB_REG";
				break;

			case BAC_AV:
				ptr_panel.info[i].name = "AV";
				break;
			case BAC_AI:
				ptr_panel.info[i].name = "AI";
				break;
			case BAC_AO:
				ptr_panel.info[i].name = "AO";
				break;
			case BAC_DO:
				ptr_panel.info[i].name = "BO";  //change DO to BO.
				break;
            case BAC_BV:
                ptr_panel.info[i].name = "BV";
                break;
			default:
				{
					ptr_panel.info[i].name = "DEFAULT_NAME";
				}
				break;
		}
	}
}

void copy_data_to_ptrpanel(int Data_type)
{
	if(Data_type ==  TYPE_ALL)
	{
		for (int i=0;i<(int)m_Input_data.size();i++)
		{
			memcpy(&ptr_panel.inputs[i], &m_Input_data.at(i),sizeof(Str_in_point));
		}
		for (int i=0;i<(int)m_Output_data.size();i++)
		{
			memcpy(&ptr_panel.outputs[i], &m_Output_data.at(i),sizeof(Str_out_point));
		}
		for (int i=0;i<(int)m_Program_data.size();i++)
		{
			memcpy(&ptr_panel.programs[i], &m_Program_data.at(i),sizeof(Str_program_point));
		}
		for (int i=0;i<(int)m_Variable_data.size();i++)
		{
			memcpy(&ptr_panel.vars[i], &m_Variable_data.at(i),sizeof(Str_variable_point));
		}
		for (int i=0;i<(int)m_Weekly_data.size();i++)
		{
			memcpy(&ptr_panel.weekly_routines[i], &m_Weekly_data.at(i),sizeof(Str_weekly_routine_point));
		}
		for (int i=0;i<(int)m_Annual_data.size();i++)
		{
			memcpy(&ptr_panel.annual_routines[i], &m_Annual_data.at(i),sizeof(Str_annual_routine_point));
		}
		for (int i=0;i<(int)m_screen_data.size();i++)
		{
			memcpy(&ptr_panel.control_groups[i],&m_screen_data.at(i),sizeof(Control_group_point));
		}
		for (int i=0;i<(int)m_controller_data.size();i++)
		{
			memcpy(&ptr_panel.controllers[i],&m_controller_data.at(i),sizeof(Str_controller_point));
		}
		return;
	}
	switch(Data_type)
	{
	case TYPE_INPUT:
		{
			for (int i=0;i<(int)m_Input_data.size();i++)
			{
				memcpy(&ptr_panel.inputs[i], &m_Input_data.at(i),sizeof(Str_in_point));
			}
		}
		break;
	case TYPE_OUTPUT:
		{
			for (int i=0;i<(int)m_Output_data.size();i++)
			{
				memcpy(&ptr_panel.outputs[i], &m_Output_data.at(i),sizeof(Str_out_point));
			}
		}
		break;
	case  TYPE_PROGRAM:
		{
			for (int i=0;i<(int)m_Program_data.size();i++)
			{
				memcpy(&ptr_panel.programs[i], &m_Program_data.at(i),sizeof(Str_program_point));
			}
		}
		break;
	case TYPE_VARIABLE:
		{
			for (int i=0;i<(int)m_Variable_data.size();i++)
			{
				memcpy(&ptr_panel.vars[i], &m_Variable_data.at(i),sizeof(Str_variable_point));
			}
		}
		break;
	case TYPE_WEEKLY:
		for (int i=0;i<(int)m_Weekly_data.size();i++)
		{
			memcpy(&ptr_panel.weekly_routines[i], &m_Weekly_data.at(i),sizeof(Str_weekly_routine_point));
		}
		break;
	case TYPE_ANNUAL:
		for (int i=0;i<(int)m_Annual_data.size();i++)
		{
			memcpy(&ptr_panel.annual_routines[i], &m_Annual_data.at(i),sizeof(Str_annual_routine_point));
		}
		break;
	case TYPE_SCREENS:
		for (int i=0;i<(int)m_screen_data.size();i++)
		{
			memcpy(&ptr_panel.control_groups[i],&m_screen_data.at(i),sizeof(Control_group_point));
		}
		break;
	case TYPE_CONTROLLER:
		for (int i=0;i<(int)m_controller_data.size();i++)
		{
			memcpy(&ptr_panel.controllers[i],&m_controller_data.at(i),sizeof(Str_controller_point));
		}
		break;
	}

}
#define  FUNCTION_TABEL  0
#define  COMMAND_TABEL 1
#define  LOCAL_VAR_TABEL 2
void check_each_point(char *richeditchar,int item_count ,int ntype);
void check_function_table(char *richeditchar ,int ntype);
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken);
extern vector <Str_char_pos_color> m_prg_char_color;	//用于highlight 关键字用;
extern CString high_light_string;
void check_high_light()
{
	char temp_char[4000];
	memset(temp_char,0,4000);

	if(high_light_string.IsEmpty())
		return;

	high_light_string.Replace(_T("\r\n"),_T("\n"));
	//CStringArray mytemp_array;
	//SplitCStringA(mytemp_array,high_light_string,_T("\r\n"));
	//CString new_high_light_str;
	//new_high_light_str.Empty();
	//for (int i=0;i<mytemp_array.GetSize();i++)
	//{
	//	new_high_light_str = new_high_light_str + mytemp_array.GetAt(i);
	//}

	WideCharToMultiByte( CP_ACP, 0, high_light_string.GetBuffer(), -1, temp_char, 4000, NULL, NULL );
	m_prg_char_color.clear();

	check_each_point(temp_char,BAC_INPUT_ITEM_COUNT,IN);
	check_each_point(temp_char,BAC_OUTPUT_ITEM_COUNT,OUT);
	check_each_point(temp_char,BAC_VARIABLE_ITEM_COUNT,VAR);
	check_each_point(temp_char,BAC_PID_COUNT,CON);
	check_each_point(temp_char,BAC_PROGRAM_ITEM_COUNT,PRG);
	check_each_point(temp_char,BAC_SCREEN_COUNT,GRP);
	check_each_point(temp_char,BAC_SCHEDULE_COUNT,WR);
	check_each_point(temp_char,BAC_HOLIDAY_COUNT,AR);
	check_function_table(temp_char,FUNCTION_TABEL);
	check_function_table(temp_char,COMMAND_TABEL);
    check_function_table(temp_char, LOCAL_VAR_TABEL);
}

void check_function_table(char *richeditchar ,int ntype)
{
	Str_char_pos_color temp_pos_color;
	char * char_source = NULL;
	char temp_label[20];

	int loop_size = 0;
	if(ntype == FUNCTION_TABEL)
	{
		for (int i=0; strlen(func_table[i].func_name)!= 0 ; i++ )
		{
			loop_size = i;
		}
	}
	else if(ntype == COMMAND_TABEL)
	{
		for (int i=0; strlen(table[i].command)!= 0; i++ )
		{
			loop_size = i;
		}
	}
    else if (ntype == LOCAL_VAR_TABEL)
    {
        //extern char local_var_new_name[40][40];
        for (int i = 0; i < 40; i++)
        {
            if (strlen(local_var_new_name[i]) == 0)
            {
                loop_size = i - 1;
                break;
            }
        }
    }
	else
		return;


    //LOCAL_VAR_TABEL
	for(int i = 0 ; i<loop_size+1 ; i++ )
	{
		char_source = richeditchar;
		long start_pos = 0;
		long end_pos = 0;
		DWORD char_color;// = RGB(0,255,255);
		char* find_char_pos = NULL;
		unsigned short look_str_length = 0;
		memset(temp_label,0,20);
		temp_label[0] = 0x20;
		if(ntype == FUNCTION_TABEL)
		{
			strcat_s(temp_label,20,func_table[i].func_name);
			char_color =prg_function_color;//  RGB(0,255,255);
		}
		else if(ntype == COMMAND_TABEL)
		{
			strcat_s(temp_label,20,table[i].command);
			char_color =prg_command_color;// RGB(255,0,255);
		}
        else if (ntype == LOCAL_VAR_TABEL)
        {
            strcat_s(temp_label, 20, local_var_new_name[i]);
            char_color = prg_local_var_color;//RGB(111, 111, 111);
        }
		else
			return;
			
		//strcat_s(temp_label,20," ");

		look_str_length = strlen(temp_label);
		if(look_str_length == 1)
		{
			continue;
		}
		do 
		{
			find_char_pos = strstr(char_source,temp_label);
			if(find_char_pos != NULL)
			{
				//char * temp_point_start = find_char_pos;
				//char * temp_point_end = NULL;

				//while( *temp_point_start == " ")
				//{
				//	temp_point_start ++ ;
				//}
				//temp_point_start = temp_point_end;

				//while((*(temp_point_end + 1) != " ") && (*(temp_point_end + 1) != 0x0d))
				//{
				//	temp_point_end ++ ;
				//}

				//int temp_length = 0;
				//temp_length = temp_point_end - temp_point_start;

				//if((temp_length > 0) && (temp_length <255))
				//{

				//	char temp_buffer[255];
				//	memset(temp_buffer ,0 , 255);
				//	memcpy_s(temp_buffer,temp_length,temp_point_start,temp_length);
				//	ispoint_ex()
				//}




				temp_pos_color.startpos = find_char_pos - richeditchar ;
				temp_pos_color.endpos = temp_pos_color.startpos + look_str_length ;
				if(ntype == FUNCTION_TABEL)
				{
					temp_pos_color.key_type = KEY_FUNCTION;
				}
				else if(ntype == COMMAND_TABEL)
				{
					temp_pos_color.key_type = KEY_COMMAND;
				}

				temp_pos_color.ncolor = char_color;
				m_prg_char_color.push_back(temp_pos_color);
				char_source = find_char_pos + look_str_length;
			}
		} while (find_char_pos != NULL);

	}

}

void check_each_point(char *richeditchar,int item_count ,int ntype)
{
	Str_char_pos_color temp_pos_color;
	char * char_source = NULL;
	char temp_label[20];
	char item_value[10];
	for (int i=1; i< item_count + 1;i++)
	{
		char_source = richeditchar;
		long start_pos = 0;
		long end_pos = 0;
		DWORD char_color = prg_label_color;//RGB(255,0,0);


		char* find_char_pos = NULL;
		unsigned short look_str_length = 0;
		memset(item_value,0,10);
		memset(temp_label,0,20);
		temp_label[0] = 0x20;
		strcat_s(temp_label,20,ptr_panel.info[ntype].name);
		itoa(i,item_value,10);
		strcat_s(temp_label,20,item_value);
		strcat_s(temp_label,20," ");


		//CString temp_cs;
		//temp_cs.Format(_T(" %s%d "),ptr_panel.info[IN].name,i);
		look_str_length = strlen(temp_label);
		do 
		{
			find_char_pos = strstr(char_source,temp_label);
			if(find_char_pos != NULL)
			{
				temp_pos_color.startpos = find_char_pos - richeditchar ;
				temp_pos_color.endpos = temp_pos_color.startpos + look_str_length ;
				temp_pos_color.key_type = KEY_INPUT;
				temp_pos_color.ncolor = char_color;
				m_prg_char_color.push_back(temp_pos_color);
				char_source = find_char_pos + look_str_length;
			}
		} while (find_char_pos != NULL);

		unsigned char length_temp = 0;
		char temp_label_char[50];
		char temp_get_label[50];
		memset(temp_get_label,0,50);
		switch(ntype)
		{
		case IN:
			strcpy_s(temp_get_label,50,m_Input_data.at(i-1).label);
			break;
		case OUT:
			strcpy_s(temp_get_label,50,m_Output_data.at(i-1).label);
			break;
		case VAR:
			strcpy_s(temp_get_label,50,m_Variable_data.at(i-1).label);
			break;
		case CON:
			continue;
			break;
		case WR:
			strcpy_s(temp_get_label,50,m_Weekly_data.at(i-1).label);
			break;
		case AR:
			strcpy_s(temp_get_label,50,m_Annual_data.at(i-1).label);
			break;
		case PRG:
			strcpy_s(temp_get_label,50,m_Program_data.at(i-1).label);
			break;
		case GRP:
			strcpy_s(temp_get_label,50,m_screen_data.at(i-1).label);
			break;
		default:
			return;
		}
		length_temp = strlen(temp_get_label);
		if((length_temp == 0 ) || (length_temp >= STR_IN_LABEL))
		{
			continue;
		}
		else
		{
			memset(temp_label_char,0,50);
			char_source = richeditchar;
			temp_label_char[0] = 0x20;
			strcat_s(temp_label_char,50,temp_get_label);
			strcat_s(temp_label_char,50," ");
			look_str_length = strlen(temp_label_char);
			do 
			{
				find_char_pos = strstr(char_source,temp_label_char);
				if(find_char_pos != NULL)
				{
					temp_pos_color.startpos = find_char_pos - richeditchar ;
					temp_pos_color.endpos = temp_pos_color.startpos + look_str_length ;
					temp_pos_color.key_type = KEY_INPUT;
					temp_pos_color.ncolor = char_color;
					m_prg_char_color.push_back(temp_pos_color);
					char_source = find_char_pos + look_str_length;
				}
			} while (find_char_pos != NULL);

		}


	}
}



















