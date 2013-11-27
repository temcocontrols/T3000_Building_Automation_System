#ifndef __SCHEDULE_H
#define __SCHEDULE_H
//***************************************************************************

#define REV1
//#define TEST
#define GOAL_CPU
#define DEFAULT_TIMEOUT  			0

#define INIT_HEARTBEAT_COUNT			70   //timer for 100ms
#define CR					0x0d // enter character ,use in communication between goal chip and NEMO10

#define MAX_ID					254
#define MAX_WR      				20
#define MAX_AR					16

#define	MAX_INTERVALS_PER_DAY			4
typedef enum
{
	WR_DESCRIPTION ,			 
 	AR_DESCRIPTION ,				 
  	ID ,							 
  	AR_TIME ,	 	 
 	INITIALIZE ,
 	BLOCK_COUNT       			
 };					 
 #define GROUP_LABLE_SIZE 10
#define WR_DESCRIPTION_SIZE			31
#define AR_DESCRIPTION_SIZE			29
#define ID_SIZE					3
#define AR_TIME_SIZE				46
#define WR_TIME_SIZE				72
#define INPUT_DESCRIPTION_LENGTH 10
#define ONE_SWITCH_LENGTH 60
#define ONE_GROUP_LENGTH 60
#define OUTPUT_DESCRIPTION_LENGTH 10
#define WR_BLOCK_SIZE				1024
#define AR_BLOCK_SIZE				1024
#define ID_BLOCK_SIZE				1536
#define AR_TIME_BLOCK_SIZE			1024
#define INITIALIZE_BLOCK_SIZE			512

#define TIME_START_ADDRESS			0X14
#define WR_START_ADDRESS			0X15
#define WR_ON_TIME_START_ADDRESS		0X16
#define WR_OFF_TIME_START_ADDRESS		0X17
#define AR_START_ADDRESS			0X18
#define AR_TIME_START_ADDRESS			0X19
#define ID_START_ADDRESS			0X1a

#define READ_MAIN_VARIABLES      		3
#define WRITE_MAIN_VARIABLES     		6
#define WRITE_MULTIPLE_MAIN_VARIABLES  		16 
#define CHECKONLINE				25			 

#define IT_IS_RELAY				1
#define SWITCH_MODE_DATA			2
#define LOCAL_DATA_PACKET			3
#define IT_IS_GARBAGE				4

#define SUB_TO_MAIN				1

#define SEND					1
#define RECEIVE					0

#define FALSE 					0
#define TRUE  					1

#define OFFSET_MODBUS_ADDRESS	100 			// Offset for 2nd block of registers
#define SCHEDUAL_MODBUS_ADDRESS 200

enum {
	 
	MODBUS_TIMER_ADDRESS		= SCHEDUAL_MODBUS_ADDRESS ,
	
	MODBUS_WR_DESCRIP_FIRST		= MODBUS_TIMER_ADDRESS + 8 ,
	MODBUS_WR_DESCRIP_LAST		= MODBUS_WR_DESCRIP_FIRST + WR_DESCRIPTION_SIZE * MAX_WR ,
	
	MODBUS_AR_DESCRIP_FIRST		= MODBUS_WR_DESCRIP_LAST  ,
	MODBUS_AR_DESCRIP_LAST		= MODBUS_AR_DESCRIP_FIRST + AR_DESCRIPTION_SIZE * MAX_AR ,
	
	MODBUS_ID_FIRST				= MODBUS_AR_DESCRIP_LAST  ,
	MODBUS_ID_LAST				= MODBUS_ID_FIRST + ID_SIZE * MAX_ID ,
	
	MODBUS_AR_TIME_FIRST		= MODBUS_ID_LAST  ,
	MODBUS_AR_TIME_LAST			= MODBUS_AR_TIME_FIRST + AR_TIME_SIZE * MAX_AR ,
	
	MODBUS_WR_ONTIME_FIRST		= MODBUS_AR_TIME_LAST ,
	MODBUS_WR_ONTIME_LAST		= MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE * MAX_WR ,
	
	MODBUS_WR_OFFTIME_FIRST		= MODBUS_WR_ONTIME_LAST  ,
	MODBUS_WR_OFFTIME_LAST		= MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE * MAX_WR ,
	MODBUS_TOTAL_PARAMETERS		= MODBUS_WR_OFFTIME_LAST 

} ;

#define INPUT_FILTER_TIME 200
#define SWITCH_TYPE 201
#define OVER_TIME 252
#define INPUT_OUTPUT_MAPPING 300
#define GROUP_OUTPUT_MAPPING 1740
#define INPUT_AM 4148
#define INPUT_MB 4172
#define GROUP_AM 4196
#define GROUP_MB 4236
#define WEEKLY_ROUTINE_DESCRIPTION 4276
#define ANNUAL_ROUTINE_DESCRIPTION 4896
#define GROUP_SCHEDULE 5360
#define ANNUAL_ROUTINE_TIME 5480
#define WEEKLY_ROUTINE_ON_TIME 6216
#define WEEKLY_ROUTINE_OFF_TIME 7656
#define INPUT_DESCRIPTION 9096
#define OUTPUT_DESCRIPTION 9336
#define GROUP_DESCRIPTION_REG 14136
#define DELAY_TIME_SWITCH 14536
#define ENABLE_DISABLE_MC 14602
#define RELAY_PULSE 15083
#define DELAY_RELAY_PULSE 15084
typedef struct WEEKLY_ROUTINES_DESCRIPTION{
		char  full_label[20];
		char  label[8];
		unsigned char  holiday1;
		unsigned char  holiday2;
		unsigned char  flag;
      }description;//weekly routines
typedef struct ANNUAL_ROUTINES_DESCRIPTION{
		char  full_label[20];
		char  label[8];
		unsigned char  flag;
      }description2;//annual routines
typedef struct CONFIG_ROUTINES_DESCRIPTION{
        unsigned char  schedul1;
 		unsigned char  schedul2;	 
		unsigned char  flag;
      }description3;//id routines
typedef struct GROUP_DESCRIPTION{
    char  full_label[10];
	unsigned char  schedul1;
	unsigned char  schedul2;	 
	unsigned char  flag;
}description3_group;
//***************************************************************************



const int SCHED_WORK_AWAKE_HOUR = 0;
const int SCHED_WORK_AWAKE_MIN = 1;
const int SCHED_WORK_AWAY_HOUR = 2;
const int SCHED_WORK_AWAY_MIN = 3;
const int SCHED_WORK_BACK_HOUR = 4;
const int SCHED_WORK_BACK_MIN = 5;
const int SCHED_WORK_SLEEP_HOUR = 6;
const int SCHED_WORK_SLEEP_MIN = 7;

const int SCHED_WEEKEN_AWAKE_HOUR = 8;
const int SCHED_WEEKEN_AWAKE_MIN = 9;
const int SCHED_WEEKEN_AWAY_HOUR = 10;
const int SCHED_WEEKEN_AWAY_MIN = 11;
const int SCHED_WEEKEN_BACK_HOUR = 12;
const int SCHED_WEEKEN_BACK_MIN = 13;
const int SCHED_WEEKEN_SLEEP_HOUR = 14;
const int SCHED_WEEKEN_SLEEP_MIN = 15;











#endif