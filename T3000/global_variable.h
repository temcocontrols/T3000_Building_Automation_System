
#pragma once
#include "afxmt.h"
#include "Bacnet_Include.h"
#include "CM5\ud_str.h"
#include "global_define.h"
#include "Global_Struct.h"
 
CString USB_Serial;
vector <int> Change_Color_ID;
const bool WRITE_SUCCESS = true;
const bool WRITE_FAIL    = false;

const int WRITE_ONE_SUCCESS_LIST = 1;
const int WRITE_ONE_FAIL_LIST =2 ;
const int WRITE_MULTI_SUCCESS_LIST =3 ;
const int WRITE_MULTI_FAIL_LIST =4 ;

int g_invoke_id;
HANDLE hThread;
DWORD nThreadID;
DWORD nThreadID_mstp;
HWND hMbpollWnd;
HWND hMbpollWritePopWnd;
int regDetailsOpenedFrom = 0;	// 0 = MbPoll.cpp; 1 = mbpollFunctions.cpp
bool list_mouse_click = false;
//#include "stdafx.h"
unsigned short multi_register_value[1024]={-1};
unsigned short multi_register_value_tcp[10000]={-1};
unsigned short product_register_value[20000]={-1};

int product_type = 0;
int old_product_type = 0;

bool g_HumChamberThread=FALSE;


unsigned short cm5_register_value[512]; //CM5
unsigned short m_buffer[512];//CM5
bool no_mouse_keyboard_event_enable_refresh = true;
bool g_fresh_T3000_background = false;
CString CurrentT3000Version ;
int nCom;
//CString program_path=_T("");
int g_tstat_id=255;
int g_mstp_deviceid; //用于全局根据Device id 访问 MSTP 。 
unsigned int g_serialNum=0;
BOOL g_tstat_id_changed=FALSE;
BOOL g_bPauseMultiRead=FALSE;
BOOL g_bChamber=FALSE;
int g_language=0;
CString g_strImagePathName=_T("");
int now_tstat_id =0;//for batch load /flash.


CString	g_strDatabasefilepath=_T("");
CString g_strExePth=_T("");
CString g_strImgeFolder=_T("");
CString g_strBuildingFolder=_T("");
CString g_achive_folder=_T("");
CString g_achive_folder_temp_txt = _T("");
CString g_achive_folder_temp_db = _T("");
CString g_achive_device_name_path = _T("");
CString g_achive_monitor_datatbase_path = _T("");

BOOL g_Scanfully=FALSE;
BOOL g_ScanSecurity=TRUE;
int g_nStartID=1;
int g_nEndID=254;
BOOL g_bCancelScan=FALSE;
int g_ScnnedNum=0;
CString g_strScanInfoPrompt;
 
CString g_strTstat5a;
CString g_strTstat5b;
CString g_strTstat5b2;
CString g_strTstat5c;
CString g_strTstat5d;
CString g_strnetWork;
CString g_strTstat5f;
CString g_strTstat5g;
CString g_strTstat5e;
CString g_strTstat5h;
CString g_strTstat6;
CString g_strTstat7;
CString g_strPressure;
CString g_strOR485;
CString g_strcm5;  //CM5
CString g_strLightingCtrl;
CString g_strHumChamber;
int g_CommunicationType;
int m_nbaudrat=19200;
CString showing_text;
 CCriticalSection  critical_section;
 CCriticalSection  register_critical_section;





BOOL g_buser_log_in=FALSE;
BOOL g_bPrivilegeMannage=FALSE;

CString g_strLoginUserName;
int g_MainScreenLevel=0;
int g_ParamLevel=0;
int g_OutPutLevel=0;
int g_NetWorkLevel=0;
int g_GraphicModelevel=0;
int g_BurnHexLevel=0;
int g_LoadConfigLevel=0;
int g_BuildingsetLevel=0;
int g_AllscreensetLevel=0;

int g_UserLevel=1;

CString g_strSensorName;
CString g_strInName1;
CString g_strInName2;
CString g_strInName3;
CString g_strInName4;
CString g_strInName5;
CString g_strInName6;
CString g_strInName7;
CString g_strInName8;
 CString g_strInCO2;
CString g_strInHumName;		// for tstat6 humidity input
CString g_strLightingSensor;
CString g_strOutName1;
CString g_strOutName2;
CString g_strOutName3;
CString g_strOutName4;
CString g_strOutName5;
CString g_strOutName6;
CString g_strOutName7;
CString g_strOutName8;
int g_ifanStatus;
CString g_strFan=_T("");

CString g_strCurBuildingDatabasefilePath=L"";
	CString m_str_curBuilding_Domain_IP;
	CString m_str_curBuilding_Domain_Port;

BOOL g_bEnableRefreshTreeView = TRUE;
BOOL g_bPauseRefreshTree = FALSE;
BOOL g_SelectChanged = TRUE;
unsigned int g_llTxCount = 0;
unsigned int g_llRxCount = 0;
int g_llerrCount = 0;
BOOL g_unint = TRUE;//TREE = °C;FALSE = F;



//int g_nIpPort=6001;
//CString	g_strIpAdress;
int MDAY=1,MNIGHT=1;








// 345	1	Low byte	W/R	(Day)Occupied   setpoint 
// 346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
// 347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
// 348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
// 349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)
// 350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
// 351	1	Low byte		spare
// 352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
// 353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
// 354	2	Full	W/R	(Night)Unoccupied heating setpoint
// 355	2	Full	W/R	(Night)Unoccupied cooling setpoint

BOOL Flexflash = FALSE;
int  FlexSP = 0;
int FlexSPN = 0;


//tstat6_new
unsigned short reg_tststold[1024];
//unsigned short newtstat6[512];
unsigned short newtstat6[1024]; //this should be more large than before;
unsigned short reg_tstat6[1024] ={
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	515,
	10,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	106,
	107,
	118,
	121,
	122,
	127,
	129,
	134,
	184,
	185,
	211,
	212,
	214,
	301,
	302,
	303,
	317,
	348,
	350,
	118,
	101,
	359,
	360,
	361,
	362,
	363,
	364,
	365,
	366,
	216,
	367,
	368,
	369,
	370,
	371,
	372,
	373,
	374,
	600,
	601,
	309,
	213,
	602,
	603,
	604,
	605,
	606,
	607,
	608,
	609,
	610,
	611,
	109,
	217,
	375,
	376,
	377,
	378,
	379,
	380,//135
	381,
	382,
	523,
	524,
	525,
	526,
	298,
	299,
	612,
	613,
	614,
	615,
	616,
	617,
	219,
	220,
	221,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	230,
	231,
	232,
	233,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	516,
	422,
	518,
	519,
	520,
	280,
	281,
	282,
	283,
	284,
	186,
	187,
	108,
	102,
	103,
	112,
	191,
	192,
	193,
	194,
	195,
	618,
	619,
	620,
	621,
	622,
	623,
	624,
	625,
	626,
	196,
	197,
	198,
	199,
	200,
	627,
	628,
	629,
	630,
	631,
	632,
	633,
	634,
	635,
	201,
	202,
	279,
	285,
	286,
	287,
	288,
	289,
	290,
	291,
	292,
	296,
	297,
	310,
	312,
	313,
	314,
	315,
	316,
	636,
	637,
	544,
	545,
	546,
	547,
	328,
	334,
	335,
	336,
	337,
	638,
	639,
	137,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
	255,
	256,
	257,
	258,
	259,
	260,
	138,
	139,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	268,
	269,
	276,
	277,
	640,
	641,
	642,
	643,
	644,
	645,
	646,
	338,
	339,
	454,//417,
	647,
	119,
	120,
	135,//380
	136,
	648,
	125,
	123,
	124,
	182,
	183,
	275,
	246,
	242,
	243,
	649,
	650,
	126,
	131,
	132,
	725,
	726,
	727,
	728,
	729,
	730,
	293,
	295,
	731,
	732,
	733,
	734,
	735,
	736,
	737,
	111,
	241,
	104,
	114,
	115,
	244,
	245,
	270,
	271,
	272,
	273,
	274,
	278,
	128,
	133,
	203,
	651,
	294,
	304,
	305,
	692,
	307,
	308,
	318,
	319,
	320,
	326,
	327,
	700,
	701,
	702,
	703,
	704,
	705,
	706,
	707,
	708,
	709,
	710,
	711,
	712,
	713,
	714,
	715,
	716,
	717,
	718,
	719,
	720,
	721,
	722,
	723,
	724,
	425,
	426,
	427,
	428,
	429,
	430,
	431,
	432,
	433,
	434,
	435,
	436,
	437,
	438,
	439,
	440,
	441,
	442,
	443,
	444,
	445,
	446,
	447,
	448,
	449,
	450,
	451,
	452,
	453,
	417,//454,
	455,
	456,
	457,
	458,
	459,
	460,
	461,
	462,
	463,
	464,
	465,
	466,
	467,
	468,
	469,
	470,
	471,
	472,
	473,
	474,
	475,
	476,
	477,
	478,
	479,
	480,
	481,
	482,
	483,
	484,
	485,
	486,
	487,
	488,
	489,
	490,
	491,
	492,
	493,
	494,
	495,
	496,
	655,
	656,
	657,
	658,
	659,
	660,
	661,
	662,
	663,
	664,
	665,
	666,
	667,
	668,
	669,
	670,
	671,
	672,
	673,
	674,
	675,
	676,
	677,
	678,
	679,
	680,
	681,
	682,
	683,
	684,
	685,
	686,
	687,
	688,
	543,
	539,
	340,
	521,
	522,
	527,
	528,
	529,
	530,
	531,
	532,
	533,
	534,
	535,
	536,
	537,
	538,
	539,
	540,
	541,
	542,
	689,
	690,
	691

};


//  char CharTstat6[512][] = {
//  	{"COMMON"},{"Serial Number - 4 byte value.  Read-only"}
//  };

char charr[600][2][600] ={
//0-3
{"COMMON","Serial Number - 4 byte value.  Read-only"},{"COMMON","Serial Number - 4 byte value.  Read-only"},
{"COMMON","Serial Number - 4 byte value.  Read-only"},{"COMMON","Serial Number - 4 byte value.  Read-only"},
//4-5
{"COMMON","Software Version – 2 byte value.  Read-only"},{"COMMON","Software Version – 2 byte value.  Read-only"},
//6,7
{"COMMON","ADDRESS. Modbus device address"},{"COMMON","Product Model.  This is a read-only register that is used by the microcontroller to determine the product"},
//8-11
{"COMMON","Hardware Revision.  This is a read-only register that is used by the microcontroller to determine the hardware rev"},{"COMMON","PIC firmware version"},
{"COMMON","PIC version of Humidity module"},{"COMMON","PLUG_N_PLAY_ADDRESS,   ‘plug n play’ address, used by the network master to resolve address conflicts. See VC code for algorithms"},
//12-13
{"COMMON",""},{"COMMON",""},
//14-17
{"COMMON",""},{"COMMON","Base address selection.0 = Protocol address,1 = PLC address."},
{"COMMON","Firmware Update Register, used to show the status of firmware updates.Writing 143 to register 16 - resets the unit to latest factory defaults.Writing 159 to register 16 - makes the current configuration the new factory default."},{"COMMON","Spare"},
//18-21
{"COMMON","Spare"},{"COMMON","Spare"},
{"COMMON","Hardware Options Register, starting with LSB: Bit0=Clock present or not, Bit1 = Humidity present or not, Bit2 = C02 Sensor, Bit3=CO sensor, Bit4 = Motion Sensor"},{"COMMON","Blank, for future use "},
//22-41
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},

//42-61
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},

//62-81
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},

//82-100
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},
{"COMMON","Blank, for future use "},{"COMMON","Blank, for future use "},

//101-120
{"MODE","COOL_HEAT_MODE, heating or cooling mode. 0=none, 1=cooling, 2=heating."},{"MODE","MODE_OPERATION, heating or cooling state: 0-7 =  coasting, cooling 1,2,3, heating 1,2,3"},
{"MODE","SEQUENCE , 0 = internal test sequence, ooutputs slowly cycle on/off or ramp up & down. 1 = normal, operation according to the output tables."},{"MODE","DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1"},
{"MODE","FAN MODE, number of fan speeds. Single speed = 1 up to three speed fan = 3"},{"MODE","POWERUP_MODE, mode of operation on power up. 0 = power off, 1 = power up in on mode, 2 = last value (default), 3 =  auto mode."},
{"MODE","AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed (depending on R122 value, 1 = Auto Mode Only, 2 = DDC mode,the user can not change setpoint and fan speed from keypad."},{"MODE","Write 1 to register 108 - resets the unit to latest factory defaults. (same as writing Writing 143 to register 16)"},
{"MODE","Info Byte, this register contains info about the state of the tstat."},{"MODE","Bau - Baudrate, 0=9600, 1=19.2kbaud"},
{"MODE","Unoccupied Override Timer, Ort. 0=disabled, >0=number of minutes manual override is allowed"},{"MODE","OVERRIDE_TIMER_DOWN_COUNT - Number of minutes remaining on the timer when unoccupied override timer is in effect."},
{"MODE","Heating cooling mode configuration, HC, 0=PID, 1=Keypad, 2=Digital_in1(not used), 3=Digital_in1,(not used) 4=Analog_in1, 5=Analog_in2"},{"MODE","Period timer ON time."},
{"MODE","Period timer OFF time."},{"MODE","Period timer units. 0, second; 1, minute; 2, hour."},
{"MODE","Dead master. The Tstat will go to occupied mode automatically if there is no serial communications for a user defined period of time, for example if the register is set to 10 the thermostat will go to ocupied mode if there are no communications for a period of 10 minutes. Set the register to 0 to disable the feature"},{"MODE","Time format. 0 = 12hours,1 = 24hours, in TSTAT6, it always 24h"},
{"MODE","free cool cobfigure byte"},{"MODE","RS485 mode"},

//121-201
{"INPUT","TEMPERATURE  reading in DegC or F from the sensor used in the control loop PI1 which is configured in register 111. This can be the internal sensor, external, or an average of the two. Writing a temperature value to this register will calibrate the currently selected sensor by adjusting the associated calibration register. If average is selected then you cannot write to this register. "},{"INPUT","ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},
{"INPUT","ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},{"INPUT","ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},
{"INPUT","ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},{"INPUT","ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},
{"INPUT","ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},{"INPUT","ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},
{"INPUT","ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON"},{"INPUT","Internal Thermistor Sensor - Shows the filtered, calibrated value of the internal thermistor sensor"},
{"INPUT","Analog input1 value"},{"INPUT","Analog input2 value"},
{"INPUT","Analog input3 value"},{"INPUT","Analog input4 value"},
{"INPUT","Analog input5 value"},{"INPUT","Analog input6 value"},
{"INPUT","Analog input7 value"},{"INPUT","Analog input8 value"},
{"INPUT","co2 ppm(optional)"},{"INPUT","humidity % (optional)"},

{"INPUT","Input auto/ manual enable."},{"INPUT","Temperature sensor filter, FIL, weighted average of stored value to new raw value"},
{"INPUT","(future)Analog input1 filer"},{"INPUT","(future)Analog input2 filer"},
{"INPUT","(future)Analog input3 filer"},{"INPUT","(future)Analog input4 filer"},
{"INPUT","(future)Analog input5 filer"},{"INPUT","(future)Analog input6 filer"},
{"INPUT","(future)Analog input7 filer"},{"INPUT","(future)Analog input8 filer"},
{"INPUT","(future)co2 filer"},{"INPUT","(future)hum filer"},
{"INPUT","CALIBRATION, this is the calibration factor for the internal sensor, normally maintained by the tstat."},{"INPUT","Calibration for the internal thermistor - internally managed offset for the internal temp sensor value"},
{"INPUT","Calibration for analog input1"},{"INPUT","Calibration for analog input2"},
{"INPUT","Calibration for analog input3"},{"INPUT","Calibration for analog input4"},
{"INPUT","Calibration for analog input5"},{"INPUT","Calibration for analog input6"},

{"INPUT","Calibration for analog input7"},{"INPUT","Calibration for analog input8"},
{"INPUT","hum 1st calibration capacitance value"},{"INPUT","hum 1st calibration humidity value"},
{"INPUT","hum 2nd calibration capacitance value"},{"INPUT","hum 2nd calibration humidity value"},
{"INPUT","Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},{"INPUT","Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},
{"INPUT","(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},{"INPUT","(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},
{"INPUT","(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},{"INPUT","(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},
{"INPUT","(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},{"INPUT","(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode."},
{"INPUT","Lookup Table 1 - 0.0V value   Sensor value that corresponds to 0.0V"},{"INPUT","Lookup Table 1 - 0.5V value   Sensor value that corresponds to 0.5V"},
{"INPUT","Lookup Table 1 - 1.0V value   Sensor value that corresponds to 1.0V"},{"INPUT","Lookup Table 1 - 1.5V value   Sensor value that corresponds to 1.5V"},
{"INPUT","Lookup Table 1 - 2.0V value   Sensor value that corresponds to 2.0V"},{"INPUT","Lookup Table 1 - 2.5V value   Sensor value that corresponds to 2.5V"},

{"INPUT","Lookup Table 1 - 3.0V value   Sensor value that corresponds to 3.0V"},{"INPUT","Lookup Table 1 - 3.5V value   Sensor value that corresponds to 3.5V"},
{"INPUT","Lookup Table 1 - 4.0V value   Sensor value that corresponds to 4.0V"},{"INPUT","Lookup Table 1 - 4.5V value   Sensor value that corresponds to 4.5V"},
{"INPUT","Lookup Table 1 - 5.0V value   Sensor value that corresponds to 5.0V"},{"INPUT","Lookup Table 2 - 0.0V value   Sensor value that corresponds to 0.0V"},
{"INPUT","Lookup Table 2 - 0.5V value   Sensor value that corresponds to 0.5V"},{"INPUT","Lookup Table 2 - 1.0V value   Sensor value that corresponds to 1.0V"},
{"INPUT","Lookup Table 2 - 1.5V value   Sensor value that corresponds to 1.5V"},{"INPUT","Lookup Table 2 - 2.0V value   Sensor value that corresponds to 2.0V"},
{"INPUT","Lookup Table 2 - 2.5V value   Sensor value that corresponds to 2.5V"},{"INPUT","Lookup Table 2 - 3.0V value   Sensor value that corresponds to 3.0V"},
{"INPUT","Lookup Table 2 - 3.5V value   Sensor value that corresponds to 3.5V"},{"INPUT","Lookup Table 2 - 4.0V value   Sensor value that corresponds to 4.0V"},
{"INPUT","Lookup Table 2 - 4.5V value   Sensor value that corresponds to 4.5V"},{"INPUT","Lookup Table 2 - 5.0V value   Sensor value that corresponds to 5.0V"},
{"INPUT","Humidity Control Register, 0 = defautl, write 1 here to set to fac calib, "},{"INPUT","Current humidity sensor value"},
{"INPUT","Update calibration data,  when set to 1, tstat will update the calibration data to PIC"},{"INPUT","Calibration points number, value can be single or two point calibration for the Tstat6"},
{"INPUT","Decide which calibration table will be used. 0 = default table 1 = custom table"},

//202-272
{"OUTPUT","Determine the output1 mode. Output1 always is ON/OFF mode"},{"OUTPUT","Determine the output2 mode. Output2 always is ON/OFF mode"},
{"OUTPUT","Determine the output3 mode. Output3 always is ON/OFF mode"},{"OUTPUT","Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM "},
{"OUTPUT","Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM"},{"OUTPUT","Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V "},
{"OUTPUT","Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V "},{"OUTPUT","Output1 tot 5, bit 0 thru 4 = relay 1 thru 5."},
{"OUTPUT","Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0."},{"OUTPUT","Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0."},
{"OUTPUT","DAC_OFFSET , Calibration data for the 0-10VDC signal, internal variable maintained by tstat"},
{"OUTPUT","Output1 Relay1_delay_OFF_TO_ON – delay time for output1 going from OFF to ON (sec)"},{"OUTPUT","Output2 Relay2_delay_OFF_TO_ON – delay time for output2 going from OFF to ON (sec)"},
{"OUTPUT","Output3 Relay3_delay_OFF_TO_ON – delay time for output3 going from OFF to ON (sec)"},{"OUTPUT","Output4 Relay4_delay_OFF_TO_ON – delay time for output4 going from OFF to ON (sec)"},
{"OUTPUT","Output5 Relay5_delay_OFF_TO_ON – delay time for output5 going from OFF to ON (sec)"},{"OUTPUT","(future)Output6 delay_OFF_TO_ON – delay time for output4 going from OFF to ON (sec)"},
{"OUTPUT","(future)Output7 delay_OFF_TO_ON – delay time for output5 going from OFF to ON (sec)"},{"OUTPUT","output 1 current time left from OFF to ON"},
{"OUTPUT","output 2 current time left from OFF to ON"},{"OUTPUT","output 3 current time left from OFF to ON"},

 {"OUTPUT","output 4 current time left from OFF to ON"},{"OUTPUT","output 5 current time left from OFF to ON"},
 {"OUTPUT","(reserved)output 6 current time left from OFF to ON"},{"OUTPUT","(reserved)output 7 current time left from OFF to ON"},
 {"OUTPUT","Output1 Relay1_delay_ON_TO_OFF – delay time for output1 going from ON to OFF (sec)"},{"OUTPUT","Output2 Relay2_delay_ON_TO_OFF – delay time for output2 going from ON to OFF (sec)"},
 {"OUTPUT","Output3 Relay3_delay_ON_TO_OFF – delay time for output3 going from ON to OFF (sec)"},{"OUTPUT","Output4 Relay4_delay_ON_TO_OFF – delay time for output4 going from ON to OFF(sec)"},
 {"OUTPUT","Output5 Relay5_delay_ON_TO_OFF – delay time for output5 going from ON to OFF(sec)"},{"OUTPUT","(future)Output6_delay_ON_TO_OFF – delay time for output4 going from ON to OFF(sec)"},
 {"OUTPUT","(future)Output7_delay_ON_TO_OFF – delay time for output5 going from ON to OFF (sec)"},{"OUTPUT","output 1 current time left from ON to OFF"},
 {"OUTPUT","output 2 current time left from ON to OFF"},{"OUTPUT","output 3 current time left from ON to OFF"},
 {"OUTPUT","output 4 current time left from ON to OFF"},{"OUTPUT","output 5 current time left from ON to OFF"},
 {"OUTPUT","(reserved)output 6 current time left from ON to OFF"},{"OUTPUT","(reserved)output 7 current time left from ON to OFF"},
 {"OUTPUT","MODBUS_CYCLING_DELAY – delay time (in minutes) for switching out of heating or cooling and then back in."},{"OUTPUT","MODBUS_CHANGOVER_DELAY – delay time (in minutes) for switching from cooling into heating or vice versa."},
 
 {"OUTPUT","Valve travel time. The time of the valve travel  from one end to another end. The units is second."},{"OUTPUT","Valve percent. Show the valve opened how much percent.  READ ONLY"},
 {"OUTPUT","Interlock for  output1"},{"OUTPUT","Interlock for  output2"},
 {"OUTPUT","Interlock for  output3"},{"OUTPUT","Interlock for  output4"},
 {"OUTPUT","Interlock for  output5"},{"OUTPUT","Interlock for  output6"},
 {"OUTPUT","Interlock for  output7"},{"OUTPUT","Delay to open. The heating valve will open if the ambient temp less than the Freeze temp setpoint last the time this register set. The units is second."},
 {"OUTPUT","Delay to close. The duration the heating valve open. The units is minute."},{"OUTPUT","Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."},
 {"OUTPUT","Relay1 manual output value"},{"OUTPUT","Relay2 manual output value"},
 {"OUTPUT","Relay3 manual output value"},{"OUTPUT","Relay4 manual output value"},
 {"OUTPUT","Relay5 manual output value"},{"OUTPUT","(future) Analog1 manual  output value"},
 {"OUTPUT","(future) Analog2 manual  output value"},{"OUTPUT","DEADMASTER_MODE = 0, the default, outputs will not change when deadmaster is triggered DEADMASTER_MODE = 1, the output will be trigger to “AUTO” mode if they were previously in manual DEADMASTER_MODE = 2, the outputs will go to manual on or off as defined in the following registers."},

 {"OUTPUT","Define output states when DEADMASTER_MODE = 2 (active)Bit0 is for relay 1, bit1 for relay 2 and so on up to output 5"},{"OUTPUT","Analog Output1 goes to this value when DEADMASTER_MODE = 2 (active), 0-1000"},
 {"OUTPUT","Analog Output2 goes to this value when DEADMASTER_MODE = 2 (active), 0-1000"},{"OUTPUT","Output1 Function setting:0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "},
 {"OUTPUT","Output2 function setting (see above)"},{"OUTPUT","Output3 function setting (see above)"},
 {"OUTPUT","Output4 function setting (see above)"},{"OUTPUT","Output5 function setting (see above)"},
 {"OUTPUT","(future)Output6 function setting (see above)"},{"OUTPUT","(future)Output7 function setting (see above)"},


//273-340
{"OUTTABLE","FAN_SPEED, current operating fan speed Relay Output Tables (bit0 = relay1, bit1 = relay2, bit2 = relay3, bit3 = relay4, bit4 = relay5)'Fan0 table is for the off state. Fan1, Fan2, and Fan3 are for the manual states. Fan4 is for the Auto state.  These states are controlled by the user.'The mode of operation (coasting, cooling, heating) is determined by the PID parameter."},{"OUTTABLE","Output 1 PID Interlock   0 = PID1, can assign each output to either PID1 or 2, the max or the min of the two PIDS"},
{"OUTTABLE","Output 2 PID Interlock                    1 = PID2"},{"OUTTABLE","Output 3 PID Interlock                    2 = Maximum of PID1 and PID2"},
{"OUTTABLE","Output 4 PID Interlock                    3 = Minimum of PID1 and PID2"},{"OUTTABLE","Output 5 PID Interlock"},
{"OUTTABLE","Output 6 PID Interlock"},{"OUTTABLE","Output 7 PID Interlock"},
{"OUTTABLE","PID2 Output table- Coasting"},{"OUTTABLE","PID2 Output table- Cooling1"},
{"OUTTABLE","PID2 Output table- Cooling2"},{"OUTTABLE","PID2 Output table- Cooling3"},
{"OUTTABLE","PID2 Output - Heating1"},{"OUTTABLE","PID2 Output - Heating"},
{"OUTTABLE","PID2 Output - Heating3"},{"OUTTABLE","FAN0_OPERATION_TABLE_COAST"},
{"OUTTABLE","FAN0_OPERATION_TABLE_COOL1"},{"OUTTABLE","FAN0_OPERATION_TABLE_COOL2"},
{"OUTTABLE","FAN0_OPERATION_TABLE_COOL3  "},{"OUTTABLE","FAN0_OPERATION_TABLE_HEAT1"},

{"OUTTABLE","FAN0_OPERATION_TABLE_HEAT2 "},{"OUTTABLE","FAN0_OPERATION_TABLE_HEAT3"},
{"OUTTABLE","FAN1_OPERATION_TABLE_COAST"},{"OUTTABLE","FAN1_OPERATION_TABLE_COOL1 "},
{"OUTTABLE","FAN1_OPERATION_TABLE_COOL2"},{"OUTTABLE","FAN1_OPERATION_TABLE_COOL3 "},
{"OUTTABLE","FAN1_OPERATION_TABLE_HEAT1"},{"OUTTABLE","FAN1_OPERATION_TABLE_HEAT2"},
{"OUTTABLE","FAN1_OPERATION_TABLE_HEAT3 "},{"OUTTABLE","FAN2_OPERATION_TABLE_COAST "},
{"OUTTABLE","FAN2_OPERATION_TABLE_COOL1"},{"OUTTABLE","FAN2_OPERATION_TABLE_COOL2 "},
{"OUTTABLE","FAN2_OPERATION_TABLE_COOL3 "},{"OUTTABLE","FAN2_OPERATION_TABLE_HEAT1 "},
{"OUTTABLE","FAN2_OPERATION_TABLE_HEAT2 "},{"OUTTABLE","FAN2_OPERATION_TABLE_HEAT3 "},
{"OUTTABLE","FAN3_OPERATION_TABLE_COAST"},{"OUTTABLE","FAN3_OPERATION_TABLE_COOL1 "},
{"OUTTABLE","FAN3_OPERATION_TABLE_COOL2 "},{"OUTTABLE","FAN3_OPERATION_TABLE_COOL3"},

{"OUTTABLE","FAN3_OPERATION_TABLE_HEAT1 "},{"OUTTABLE","FAN3_OPERATION_TABLE_HEAT2 "},
{"OUTTABLE","FAN3_OPERATION_TABLE_HEAT3"},{"OUTTABLE","FANAUT_OPERATION_TABLE_COAST"},
{"OUTTABLE","FANAUT_OPERATION_TABLE_COOL1 "},{"OUTTABLE","FANAUT_OPERATION_TABLE_COOL2 "},
{"OUTTABLE","FANAUT_OPERATION_TABLE_COOL3"},{"OUTTABLE","FANAUT_OPERATION_TABLE_HEAT1 "},
{"OUTTABLE","FANAUT_OPERATION_TABLE_HEAT2 "},{"OUTTABLE","FANAUT_OPERATION_TABLE_HEAT3"},
{"OUTTABLE","VALVE_OPER_TABLE_COAST, Analog output state for each of the 7 modes of operation"},{"OUTTABLE","VALVE_OPER_TABLE_COOLING1"},
{"OUTTABLE","VALVE_OPER_TABLE_COOLING2"},{"OUTTABLE","VALVE_OPER_TABLE_COOLING3"},
{"OUTTABLE","VALVE_OPER_TABLE_HEATING1"},{"OUTTABLE","VALVE_OPER_TABLE_HEATING2"},
{"OUTTABLE","VALVE_OPER_TABLE_HEATING3"},{"OUTTABLE","Number of Heating Stages (Max heat+cool = 6)"},
{"OUTTABLE","Number of Cooling Stages (Max heat + Cool = 6) "},{"OUTTABLE","Number of Heating Stages in Original Table - (Maximum # of total heating and cooling states is 6)"},

{"OUTTABLE","Number of Cooling Stages in Original Table - (Maximum # of total heating and cooling states is 6)"},{"OUTTABLE","analog output in OFF table, coating mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},
{"OUTTABLE","analog output in OFF table, cooling1 mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},{"OUTTABLE","analog output in OFF table, cooling2 mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},
{"OUTTABLE","analog output in OFF table, cooling3 mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},{"OUTTABLE","analog output in OFF table, heating1 mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},
{"OUTTABLE","analog output in OFF table, heating2 mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},{"OUTTABLE","analog output in OFF table, heating3 mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off"},



//341-378
{"SETPOINT","Default occupied setpoint. Works in concert with the 'occupied setpoint control register', register 339"},{"SETPOINT","Occupied Setpoint Control Register: 0 = normal, setpoint is managed by the serial port and keypad, the stat will remember the last occupied setpoint and use that during the next occupied period. 1 = Default mode, the last occupied setpoint if forgotten and the occupied setpoint gets reset to the default. 2 = trigger an event, when a master controller writes 2 to this register, the default setpoint will be copied to the occupied setpoint after which the Tstat will set the value back to 1 to show the event has been serviced. "},
{"SETPOINT","Occupied setpoint option,0 = office, 1 = hotel mode"},{"SETPOINT","(hotel)Occupied cooling setpoin."},
{"SETPOINT","(hotel)Occupied cooling setpoint dead band , offset from setpoint for cooling to begin. Units are 0.1 deg."},{"SETPOINT","(hotel)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg."},
{"SETPOINT","(office)Occupied cooling setpoint - limits are set by the max and min setpoints"},{"SETPOINT","(office)Occupied heating setpoint,current heating setpoint"},
{"SETPOINT","(office)Occupied cooling setpoint + cooling deadband(through this register can adjust cooling dead band), use for tiny adjustment "},{"SETPOINT","Unoccupied setpoint option,   0 = office (absolute setpoints, 1 = Hotel or Residential, deadband type offsets"},
{"SETPOINT","(hotel)Unoccupied cooling setpoint dead band , heating deadband in the night time or OFF mode.  Units of 1 deg."},{"SETPOINT","(hotel)Unoccupied heating setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg."},
{"SETPOINT","(office)Unoccupied heating setpoint"},{"SETPOINT","(office)Unoccupied cooling setpoint"},
{"SETPOINT","PID2 Unoccupied (OFF) Setpoint"},{"SETPOINT","PID2 Occupied Setpoint"},
{"SETPOINT","PID2 Cooling Deadband  TBD: can write to either deadband or to setpoint"},{"SETPOINT","PID2 Heating Deadband,  TBD: can write to either deadband or to setpoint"},
{"SETPOINT","economy mode cooling setpoint"},{"SETPOINT","economy mode heating setpoint"},

{"SETPOINT","POWERUP_SETPOINT , setpoint on power up"},{"SETPOINT","MAX_SETPOINT, Setpoint high, the highest setpoint a user will be able to set from the keypad."},
{"SETPOINT","MIN_SETPOINT, Setpoint Low, the lowest setpoint a user will be able to set from the keypad. "},{"SETPOINT","reserved,reserved,MAX_SETPOINT2"},
{"SETPOINT","reserved,MIN_SETPOINT2"},{"SETPOINT","reserved,MAX_SETPOINT3"},
{"SETPOINT","reserved,MIN_SETPOINT3"},{"SETPOINT","reserved,MAX_SETPOINT4"},
{"SETPOINT","reserved,MIN_SETPOINT4"},{"SETPOINT","Setpoint increment on the display each time the user hits the up/down buttons. Units are 0.1Deg, 10 = 1Deg and so on. "},
{"SETPOINT","Freeze protect setpoint. If the ambient temperature less than the setpoint,the heating valve will open some time the Delay to off register set ."},{"SETPOINT","reserved, wall setpoint"},
{"SETPOINT","reserved,ceiling setpoint"},{"SETPOINT","reserved,average setpoint"},
{"SETPOINT","reserved,unoccupied heating"},{"SETPOINT","reserved,unoccupied cooling"},
{"SETPOINT","reserved,humidity setpoint"},{"SETPOINT","reserved,cuurent1 setpoint"},


//379-391
{"PID","Sensor to be used for the PID calculations,  1= external sensor analog input 1 , 2 = internal thermistor, 3 = average the internal thermistor and analog input1"},{"PID","Universal PID input select, 0=none, 1=analog_in1, 2=analog_in2, 3= humidity  4= co2"},
{"PID","PID1, current PI calculation for PID number 1  (PID2 is in register 240)"},{"PID","COOLING PTERM , proportional term for PI calculation"},
{"PID","COOLING ITEM"},{"PID","PID2 P term"},
{"PID","PID2 I term"},{"PID","PID2, the current value of PID number 2"},
{"PID","PID1 Units High byte - Upper 2 bytes of the PID1 units in ASCII"},{"PID","PID1 Units Low byte - Lower 2 bytes of the PID1 units in ASCII"},
{"PID","PID2 Units High byte - Upper 2 bytes of the PID2 units in ASCII"},{"PID","PID2 Units Low byte - Lower 2 bytes of the PID2 units in ASCII"},
{"PID","PID2 heating or cooling state derived from PID2 register 240, 0=coasting, 1=cooling1, 2=cooling2, 3=cooling3, 4=heating1, 5=heating2, 6=heating3, 14=cooling4, 15=cooling5, 16=cooling6, 17=heating4, 18=heating5, 19=heating6."},

//392-512
{"UI","KEYPAD_SELECT , variable to select various keypad arrangements.  Refer to PAd description in Table 1: Advanced Menu Items"},{"UI","SPECIAL_MENU_LOCK, Special menu lockout via keypad, serial port only, "},
{"UI","dIS – Display.  This sets the display to either room temperature or setpoint.  0 = room temp,  1 = setpoint, 2 = Blank Display,3 = PID2 value,4 = PID2 setpoint, 5 = set segment code by manually, 6 = Display sleep"},{"UI","ICON TABLE Set when certain icons are on/off"},
{"UI","Last key  pressed counter. Minutes since the used last pressed a key. "},{"UI","Keypad encoded value. Last keypress, READ ONLY"},
{"UI","LED hundred’s digit, , can drive the LEDs manually when the display register 381 is set to manual (5) "},{"UI",""},
{"UI","LED one’s digit code, can drive the LEDs manually when the display register 381 is set to manual (5) "},{"UI","LED discrete status symbols, can drive the LEDs manually when the display register 381 is set to manual (5) "},
 {"UI","Rouding display. 0 = round the display to the nearest digit; 1 = round the display to the nearest 1/10 unit;  5 = round the display to the nearest 1/2 unit. 2,3,4 reserved."},{"UI","The minimum address which the device can accept, use this to limit addresses to a certain defined band. "},
{"UI","The maximum device address can  be set. The device address should between min and max address, default is 254"},{"UI","Show the size of E2 chip. 0 = 24c02, 1 = 24c08/24c16."},
 {"UI","Assign the timer to be used for which feature. 0 = period timer, 1 = rotation timer, 2 = interlock, 3 = PWM timer."},{"UI","Clock, year"},
{"UI","Clock,month"},{"UI","Clock,week"},
{"UI","Clock,day"},{"UI","clock, hours"},

{"UI","clock,minutes"},{"UI","clock,seconds"},
{"UI","alarm,not used now"},{"UI","work day,wake time hour"},
{"UI","work day,wake time minutes"},{"UI","work day,away time,hour"},
{"UI","work day,away time,minute"},{"UI","work day,home time,hour"},
{"UI","work day,home time,minute"},{"UI","work day,sleep time,hour"},
{"UI","work day,sleep time,minute"},{"UI","weekend day,wake time hour"},
{"UI","weekend day,wake time minutes"},{"UI","weekend,away time,hour"},
{"UI","weekend,away time,minute"},{"UI","weekend,home time,hour"},
{"UI","weekend,home time,minute"},{"UI","weekend,sleep time,hour"},
{"UI","weekend,sleep time,minute"},{"UI","LCD turn off, 0 = turn off, 1 = normal"},

{"UI","User name. line 1:characters 1 & 2"},{"UI","User name. line 1:characters 3 & 4"},
{"UI","User name. line 1:characters 5 & 6"},{"UI","User name. line 1:characters 7 & 8"},
{"UI","User name. line 2:characters 1 & 2"},{"UI","User name. line 2:characters 3 & 4"},
{"UI","User name. line 2:characters 5 & 6"},{"UI","User name. line 2:characters 7 & 8"},
{"UI","Internal sensor name. line 1:characters 1 & 2"},{"UI","Internal sensor name. line 1:characters 3 & 4"},
{"UI","Internal sensor name. line 1:characters 5 & 6"},{"UI","Internal sensor name. line 1:characters 7 & 8"},
{"UI","Input 1 user name. line 1:characters 1 & 2"},{"UI","Input 1 user name. line 1:characters 3 & 4"},
{"UI","Input 1 user name. line 1:characters 5 & 6"},{"UI","Input 1 user name. line 1:characters 7 & 8"},
{"UI","Input 2 user name. line 1:characters 1 & 2"},{"UI","Input 2 user name. line 1:characters 3 & 4"},
{"UI","Input 2 user name. line 1:characters 5 & 6"},{"UI","Input 2 user name. line 1:characters 7 & 8"},


{"UI","Input 3 user name. line 1:characters 1 & 2"},{"UI","Input 3 user name. line 1:characters 3 & 4"},
{"UI","Input 3 user name. line 1:characters 5 & 6"},{"UI","Input 3 user name. line 1:characters 7 & 8"},
{"UI","Input 4 user name. line 1:characters 1 & 2"},{"UI","Input 4 user name. line 1:characters 3 & 4"},
{"UI","Input 4 user name. line 1:characters 5 & 6"},{"UI","Input 4 user name. line 1:characters 7 & 8"},
{"UI","Input 5 user name. line 1:characters 1 & 2"},{"UI","Input 5 user name. line 1:characters 3 & 4"},
{"UI","Input 5 user name. line 1:characters 5 & 6"},{"UI","Input 5 user name. line 1:characters 7 & 8"},
{"UI","Input 6 user name. line 1:characters 1 & 2"},{"UI","Input 6 user name. line 1:characters 3 & 4"},
{"UI","Input 6 user name. line 1:characters 5 & 6"},{"UI","Input 6 user name. line 1:characters 7 & 8"},
{"UI","Input 7 user name. line 1:characters 1 & 2"},{"UI","Input 7 user name. line 1:characters 3 & 4"},
{"UI","Input 7 user name. line 1:characters 5 & 6"},{"UI","Input 7 user name. line 1:characters 7 & 8"},

{"UI","Input 8 user name. line 1:characters 1 & 2"},{"UI","Input 8 user name. line 1:characters 3 & 4"},
{"UI","Input 8 user name. line 1:characters 5 & 6"},{"UI","Input 8 user name. line 1:characters 7 & 8"},
{"UI","Output 1 user name. line 1:characters 1 & 2"},{"UI","Output 1 user name. line 1:characters 3 & 4"},
{"UI","Output 1 user name. line 1:characters 5 & 6"},{"UI","Output 1 user name. line 1:characters 7 & 8"},
{"UI","Output 2 user name. line 1:characters 1 & 2"},{"UI","Output 2 user name. line 1:characters 3 & 4"},
{"UI","Output 2 user name. line 1:characters 5 & 6"},{"UI","Output 2 user name. line 1:characters 7 & 8"},
{"UI","Output 3 user name. line 1:characters 1 & 2"},{"UI","Output 3 user name. line 1:characters 3 & 4"},
{"UI","Output 3 user name. line 1:characters 5 & 6"},{"UI","Output 3 user name. line 1:characters 7 & 8"},
{"UI","Output 4 user name. line 1:characters 1 & 2"},{"UI","Output 4 user name. line 1:characters 3 & 4"},
{"UI","Output 4 user name. line 1:characters 5 & 6"},{"UI","Output 4 user name. line 1:characters 7 & 8"},

{"UI","Output 5 user name. line 1:characters 1 & 2"},{"UI","Output 5 user name. line 1:characters 3 & 4"},
{"UI","Output 5 user name. line 1:characters 5 & 6"},{"UI","Output 5 user name. line 1:characters 7 & 8"},
{"UI","Output 6 user name. line 1:characters 1 & 2"},{"UI","Output 6 user name. line 1:characters 3 & 4"},
{"UI","Output 6 user name. line 1:characters 5 & 6"},{"UI","Output 6 user name. line 1:characters 7 & 8"},
{"UI","Output 7 user name. line 1:characters 1 & 2"},{"UI","Output 7 user name. line 1:characters 3 & 4"},
{"UI","Output 7 user name. line 1:characters 5 & 6"},{"UI","Output 7 user name. line 1:characters 7 & 8"},
{"UI","Output 8 user name. line 1:characters 1 & 2"},{"UI","Output 8 user name. line 1:characters 3 & 4"},
{"UI","Output 8 user name. line 1:characters 5 & 6"},{"UI","Output 8 user name. line 1:characters 7 & 8"},
{"UI","Schedule_B  character 1"},{"UI","Schedule_B  character 2"},
{"UI","Schedule_B  character 3"},{"UI","Schedule_B  character 4"},
{"UI","Schedule_C  character 1"},

//512-539
//{"UI",""},{"UI",""},



}; 
//  Register Name                                         TSTAT6 7   		TSTAT5ABCDFG			TSTAT5EH
int MODBUS_SERIALNUMBER_LOWORD                               =0   ;//              0                   0 
//int RESERVED                                                 =1   ;//              1                   1 
int MODBUS_SERIALNUMBER_HIWORD                               =2   ;//              2                   2 
//int RESERVED                                                 =3   ;//              3                   3 
int MODBUS_VERSION_NUMBER_LO                                 =4   ;//              4                   4 
int MODBUS_VERSION_NUMBER_HI                                 =5   ;//              5                   5 
int MODBUS_ADDRESS                                           =6   ;//              6                   6 
int MODBUS_PRODUCT_MODEL                                     =7   ;//              7                   7 
int MODBUS_HARDWARE_REV                                      =8   ;//              8                   8 
int MODBUS_PIC_VERSION                                       =9   ;//              9                   9 
int MODBUS_HUM_PIC_VERSION                                   =10  ;//              10                  10
int MODBUS_ADDRESS_PLUG_N_PLAY                               =11  ;//              11                  11
//int RESERVED                                                 =12  ;//              12                  12
//int RESERVED                                                 =13  ;//              13                  13
int MODBUS_CUSTOM_ADDRESS                                    =14  ;//              14                  14
int MODBUS_BASE_ADDRESS                                      =15  ;//              15                  15
int MODBUS_UPDATE_STATUS                                     =16  ;//              16                  16
int MODBUS_UPDATE_PTR_HI                                     =17  ;//              17                  17
int MODBUS_UPDATE_PTR_LO                                     =18  ;//              18                  18
int MODBUS_SCRATCHPAD_ADDRESS                                =19  ;//              19                  19
int MODBUS_HARDWARE_INFORMATION                              =20  ;//              -1                  -1

int MODBUS_COOL_HEAT_MODE                                    =101 ;//              106                 106
int MODBUS_MODE_OPERATION                                    =102 ;//              107                 107
int MODBUS_SEQUENCE                                          =103 ;//              118                 118
int MODBUS_DEGC_OR_F                                         =104 ;//              121                 121
int MODBUS_FAN_MODE                                          =105 ;//              122                 122
int MODBUS_POWERUP_MODE                                      =106 ;//              127                 127
int MODBUS_AUTO_ONLY                                         =107 ;//              129                 129
int MODBUS_FACTORY_DEFAULTS                                  =108 ;//              134                 134
int MODBUS_INFO_BYTE                                         =109 ;//              184                 184
int MODBUS_BAUDRATE                                          =110 ;//              185                 185
int MODBUS_OVERRIDE_TIMER                                    =111 ;//              211                 211
int MODBUS_OVERRIDE_TIMER_LEFT                               =112 ;//              212                 212
int MODBUS_HEAT_COOL_CONFIG                                  =113 ;//              214                 214
int MODBUS_TIMER_ON                                          =114 ;//              301                 301
int MODBUS_TIMER_OFF                                         =115 ;//              302                 302
int MODBUS_TIMER_UNITS                                       =116 ;//              303                 303
int MODBUS_DEAD_MASTER                                       =117 ;//              317                 317
int MODBUS_SYSTEM_TIME_FORMAT                                =118 ;//              -1                  -1 
int MODBUS_FREE_COOL_CONFIG                                  =119 ;//              350                 350
int MODBUS_RS485_MODE                                        =120 ;//              -1                  -1 
int MODBUS_TEMPRATURE_CHIP                                   =121 ;//              101                 101
int MODBUS_ANALOG1_RANGE                                     =122 ;//              -1                  359
int MODBUS_ANALOG2_RANGE                                     =123 ;//              -1                  360
int MODBUS_ANALOG3_RANGE                                     =124 ;//              -1                  361
int MODBUS_ANALOG4_RANGE                                     =125 ;//              -1                  362
int MODBUS_ANALOG5_RANGE                                     =126 ;//              -1                  363
int MODBUS_ANALOG6_RANGE                                     =127 ;//              -1                  364
int MODBUS_ANALOG7_RANGE                                     =128 ;//              -1                  365
int MODBUS_ANALOG8_RANGE                                     =129 ;//              -1                  366
int MODBUS_INTERNAL_THERMISTOR                               =130 ;//              216                 216
int MODBUS_ANALOG_INPUT1                                     =131 ;//              -1                  367
int MODBUS_ANALOG_INPUT2                                     =132 ;//              -1                  368
int MODBUS_ANALOG_INPUT3                                     =133 ;//              -1                  369
int MODBUS_ANALOG_INPUT4                                     =134 ;//              -1                  370
int MODBUS_ANALOG_INPUT5                                     =135 ;//              -1                  371
int MODBUS_ANALOG_INPUT6                                     =136 ;//              -1                  372
int MODBUS_ANALOG_INPUT7                                     =137 ;//              -1                  373
int MODBUS_ANALOG_INPUT8                                     =138 ;//              -1                  374
int MODBUS_EXTERNAL_SENSOR1                                  =139 ;//              -1                  -1 
int MODBUS_EXTERNAL_SENSOR2                                  =140 ;//              -1                  -1 
int MODBUS_INPUT_MANU_ENABLE                                 =141 ;//              309                 309
int MODBUS_FILTER                                            =142 ;//              213                 213
int MODBUS_INPUT1_FILTER                                     =143 ;//              -1                  -1 
int MODBUS_INPUT2_FILTER                                     =144 ;//              -1                  -1 
int MODBUS_INPUT3_FILTER                                     =145 ;//              -1                  -1 
int MODBUS_INPUT4_FILTER                                     =146 ;//              -1                  -1 
int MODBUS_INPUT5_FILTER                                     =147 ;//              -1                  -1 
int MODBUS_INPUT6_FILTER                                     =148 ;//              -1                  -1 
int MODBUS_INPUT7_FILTER                                     =149 ;//              -1                  -1 
int MODBUS_INPUT8_FILTER                                     =150 ;//              -1                  -1 
int MODBUS_EX_SENSOR1_FILTER                                 =151 ;//              -1                  -1 
int MODBUS_EX_SENSOR2_FILTER                                 =152 ;//              -1                  -1 
int MODBUS_CALIBRATION_TERM                                  =153 ;//              -1                  -1 
int MODBUS_CALIBRATION_INTERNAL_THERMISTOR                   =154 ;//              217                 217
int MODBUS_CALIBRATION_ANALOG1                               =155 ;//              -1                  375
int MODBUS_CALIBRATION_ANALOG2                               =156 ;//              -1                  376
int MODBUS_CALIBRATION_ANALOG3                               =157 ;//              -1                  377
int MODBUS_CALIBRATION_ANALOG4                               =158 ;//              -1                  378
int MODBUS_CALIBRATION_ANALOG5                               =159 ;//              -1                  379
int MODBUS_CALIBRATION_ANALOG6                               =160 ;//              -1                  380
int MODBUS_CALIBRATION_ANALOG7                               =161 ;//              -1                  381
int MODBUS_CALIBRATION_ANALOG8                               =162 ;//              -1                  382
int MODBUS_HUMCOUNT1_H                                       =163 ;//              -1                  -1 
int MODBUS_HUMRH1_H                                          =164 ;//              -1                  -1 
int MODBUS_HUMCOUNT2_H                                       =165 ;//              -1                  -1 
int MODBUS_HUMRH2_H                                          =166 ;//              -1                  -1 
int MODBUS_ANALOG1_FUNCTION                                  =167 ;//              298                 298
int MODBUS_ANALOG2_FUNCTION                                  =168 ;//              299                 299
int MODBUS_ANALOG3_FUNCTION                                  =169 ;//              -1                  -1 
int MODBUS_ANALOG4_FUNCTION                                  =170 ;//              -1                  -1 
int MODBUS_ANALOG5_FUNCTION                                  =171 ;//              -1                  -1 
int MODBUS_ANALOG6_FUNCTION                                  =172 ;//              -1                  -1 
int MODBUS_ANALOG7_FUNCTION                                  =173 ;//              -1                  -1 
int MODBUS_ANALOG8_FUNCTION                                  =174 ;//              -1                  -1 
int MODBUS_TABLE1_ZERO                                       =175 ;//              219                 219
int MODBUS_TABLE1_HALFONE                                    =176 ;//              220                 220
int MODBUS_TABLE1_ONE                                        =177 ;//              221                 221
int MODBUS_TABLE1_HALFTWO                                    =178 ;//              222                 222
int MODBUS_TABLE1_TWO                                        =179 ;//              223                 223
int MODBUS_TABLE1_HALFTHREE                                  =180 ;//              224                 224
int MODBUS_TABLE1_THREE                                      =181 ;//              225                 225
int MODBUS_TABLE1_HALFFOUR                                   =182 ;//              226                 226
int MODBUS_TABLE1_FOUR                                       =183 ;//              227                 227
int MODBUS_TABLE1_HALFFIVE                                   =184 ;//              228                 228
int MODBUS_TABLE1_FIVE                                       =185 ;//              229                 229
int MODBUS_TABLE2_ZERO                                       =186 ;//              230                 230
int MODBUS_TABLE2_HALFONE                                    =187 ;//              231                 231
int MODBUS_TABLE2_ONE                                        =188 ;//              232                 232
int MODBUS_TABLE2_HALFTWO                                    =189 ;//              233                 233
int MODBUS_TABLE2_TWO                                        =190 ;//              234                 234
int MODBUS_TABLE2_HALFTHREE                                  =191 ;//              235                 235
int MODBUS_TABLE2_THREE                                      =192 ;//              236                 236
int MODBUS_TABLE2_HALFFOUR                                   =193 ;//              237                 237
int MODBUS_TABLE2_FOUR                                       =194 ;//              238                 238
int MODBUS_TABLE2_HALFFIVE                                   =195 ;//              239                 239
int MODBUS_TABLE2_FIVE                                       =196 ;//              240                 240
int MODUBS_HUMIDITY_RH                                       =197 ;//              -1                  -1 
int MODBUS_HUMIDITY_FREQUENCY                                =198 ;//              -1                  -1 
int MODBUS_HUM_PIC_UPDATE                                    =199 ;//              -1                  -1 
int MODBUS_HUM_CAL_NUM                                       =200 ;//              -1                  -1 
int MODBUS_HUM_CURRENT_DEFAULT                               =201 ;//              -1                  -1 
int MODBUS_MODE_OUTPUT1                                      =202 ;//              280                 280
int MODBUS_MODE_OUTPUT2                                      =203 ;//              281                 281
int MODBUS_MODE_OUTPUT3                                      =204 ;//              282                 282
int MODBUS_MODE_OUTPUT4                                      =205 ;//              283                 283
int MODBUS_MODE_OUTPUT5                                      =206 ;//              284                 284
int MODBUS_OUTPUT1_SCALE                                     =207 ;//              186                 186
int MODBUS_OUTPUT2_SCALE                                     =208 ;//              187                 187
int MODBUS_DIGITAL_OUTPUT_STATUS                             =209 ;//              108                 108
int MODBUS_COOLING_VALVE                                     =210 ;//              102                 102
int MODBUS_HEATING_VALVE                                     =211 ;//              103                 103
int MODBUS_DAC_OFFSET                                        =212 ;//              112                 112
int MODBUS_OUTPUT1_DELAY_OFF_TO_ON                           =213 ;//              191                 191
int MODBUS_OUTPUT2_DELAY_OFF_TO_ON                           =214 ;//              192                 192
int MODBUS_OUTPUT3_DELAY_OFF_TO_ON                           =215 ;//              193                 193
int MODBUS_OUTPUT4_DELAY_OFF_TO_ON                           =216 ;//              194                 194
int MODBUS_OUTPUT5_DELAY_OFF_TO_ON                           =217 ;//              195                 195
int MODBUS_OUTPUT6_DELAY_OFF_TO_ON                           =218 ;//              -1                  -1 
int MODBUS_OUTPUT7_DELAY_OFF_TO_ON                           =219 ;//              -1                  -1 
int MODBUS_DELAY1_OFF_TO_ON_TIME_CURRENT                     =220 ;//              387                 410
int MODBUS_DELAY2_OFF_TO_ON_TIME_CURRENT                     =221 ;//              388                 411
int MODBUS_DELAY3_OFF_TO_ON_TIME_CURRENT                     =222 ;//              389                 412
int MODBUS_DELAY4_OFF_TO_ON_TIME_CURRENT                     =223 ;//              390                 413
int MODBUS_DELAY5_OFF_TO_ON_TIME_CURRENT                     =224 ;//              391                 414
int MODBUS_DELAY6_OFF_TO_ON_TIME_CURRENT                     =225 ;//              -1                  -1 
int MODBUS_DELAY7_OFF_TO_ON_TIME_CURRENT                     =226 ;//              -1                  -1 
int MODBUS_OUTPUT1_DELAY_ON_TO_OFF                           =227 ;//              196                 196
int MODBUS_OUTPUT2_DELAY_ON_TO_OFF                           =228 ;//              197                 197
int MODBUS_OUTPUT3_DELAY_ON_TO_OFF                           =229 ;//              198                 198
int MODBUS_OUTPUT4_DELAY_ON_TO_OFF                           =230 ;//              199                 199
int MODBUS_OUTPUT5_DELAY_ON_TO_OFF                           =231 ;//              200                 200
int MODBUS_OUTPUT6_DELAY_ON_TO_OFF                           =232 ;//              -1                  -1 
int MODBUS_OUTPUT7_DELAY_ON_TO_OFF                           =233 ;//              -1                  -1 
int MODBUS_DELAY1_ON_TO_OFF_TIME_CURRENT                     =234 ;//              382                 405
int MODBUS_DELAY2_ON_TO_OFF_TIME_CURRENT                     =235 ;//              383                 406
int MODBUS_DELAY3_ON_TO_OFF_TIME_CURRENT                     =236 ;//              384                 407
int MODBUS_DELAY4_ON_TO_OFF_TIME_CURRENT                     =237 ;//              385                 408
int MODBUS_DELAY5_ON_TO_OFF_TIME_CURRENT                     =238 ;//              386                 409
int MODBUS_DELAY6_ON_TO_OFF_TIME_CURRENT                     =239 ;//              -1                  -1 
int MODBUS_DELAY7_ON_TO_OFF_TIME_CURRENT                     =240 ;//              -1                  -1 
int MODBUS_CYCLING_DELAY                                     =241 ;//              201                 201
int MODBUS_CHANGOVER_DELAY                                   =242 ;//              202                 202
int MODBUS_VALVE_TRAVEL_TIME                                 =243 ;//              279                 279
int MODBUS_VALVE_PERCENT                                     =244 ;//              285                 285
int MODBUS_INTERLOCK_OUTPUT1                                 =245 ;//              286                 286
int MODBUS_INTERLOCK_OUTPUT2                                 =246 ;//              287                 287
int MODBUS_INTERLOCK_OUTPUT3                                 =247 ;//              288                 288
int MODBUS_INTERLOCK_OUTPUT4                                 =248 ;//              289                 289
int MODBUS_INTERLOCK_OUTPUT5                                 =249 ;//              290                 290
int MODBUS_INTERLOCK_OUTPUT6                                 =250 ;//              291                 291
int MODBUS_INTERLOCK_OUTPUT7                                 =251 ;//              292                 292
int MODBUS_FREEZE_DELAY_ON                                   =252 ;//              296                 296
int MODBUS_FREEZE_DELAY_OFF                                  =253 ;//              297                 297
int MODBUS_OUTPUT_MANU_ENABLE                                =254 ;//              310                 310
int MODBUS_MANU_RELAY1                                       =255 ;//              -1                  -1 
int MODBUS_MANU_RELAY2                                       =256 ;//              -1                  -1 
int MODBUS_MANU_RELAY3                                       =257 ;//              -1                  -1 
int MODBUS_MANU_RELAY4                                       =258 ;//              -1                  -1 
int MODBUS_MANU_RELAY5                                       =259 ;//              -1                  -1 
int MODBUS_MANUAL_AO1                                        =260 ;//              -1                  -1 
int MODBUS_MANUAL_AO2                                        =261 ;//              -1                  -1 
int MODBUS_DEADMASTER_AUTO_MANUAL                            =262 ;//              312                 312
int MODBUS_DEADMASTER_OUTPUT_STATE                           =263 ;//              313                 313
int MODBUS_DEADMASTER_COOL_OUTPUT                            =264 ;//              314                 314
int MODBUS_DEADMASTER_HEAT_OUTPUT                            =265 ;//              315                 315
int MODBUS_OUTPUT1_FUNCTION                                  =266 ;//              328                 328
int MODBUS_OUTPUT2_FUNCTION                                  =267 ;//              334                 334
int MODBUS_OUTPUT3_FUNCTION                                  =268 ;//              335                 335
int MODBUS_OUTPUT4_FUNCTION                                  =269 ;//              336                 336
int MODBUS_OUTPUT5_FUNCTION                                  =270 ;//              337                 337
int MODBUS_OUTPUT6_FUNCTION                                  =271 ;//              -1                  -1 
int MODBUS_OUTPUT7_FUNCTION                                  =272 ;//              -1                  -1 
int MODBUS_FAN_SPEED                                         =273 ;//              137                 137
int MODBUS_PID_OUTPUT1                                       =274 ;//              247                 247
int MODBUS_PID_OUTPUT2                                       =275 ;//              248                 248
int MODBUS_PID_OUTPUT3                                       =276 ;//              249                 249
int MODBUS_PID_OUTPUT4                                       =277 ;//              250                 250
int MODBUS_PID_OUTPUT5                                       =278 ;//              251                 251
int MODBUS_PID_OUTPUT6                                       =279 ;//              252                 252
int MODBUS_PID_OUTPUT7                                       =280 ;//              253                 253
int MODBUS_UNIVERSAL_OUTPUT_BEGIN                            =281 ;//              254                 254
int MODBUS_UNIVERSAL_OUTPUT_COOL1                            =282 ;//              255                 255
int MODBUS_UNIVERSAL_OUTPUT_COOL2                            =283 ;//              256                 256
int MODBUS_UNIVERSAL_OUTPUT_COOL3                            =284 ;//              257                 257
int MODBUS_UNIVERSAL_OUTPUT_HEAT1                            =285 ;//              258                 258
int MODBUS_UNIVERSAL_OUTPUT_HEAT2                            =286 ;//              259                 259
int MODBUS_UNIVERSAL_OUTPUT_HEAT3                            =287 ;//              260                 260
int MODBUS_FAN0_OPER_TABLE_COAST                             =288 ;//              138                 138
int MODBUS_FAN0_OPER_TABLE_COOL1                             =289 ;//              139                 139
int MODBUS_FAN0_OPER_TABLE_COOL2                             =290 ;//              140                 140
int MODBUS_FAN0_OPER_TABLE_COOL3                             =291 ;//              141                 141
int MODBUS_FAN0_OPER_TABLE_HEAT1                             =292 ;//              142                 142
int MODBUS_FAN0_OPER_TABLE_HEAT2                             =293 ;//              143                 143
int MODBUS_FAN0_OPER_TABLE_HEAT3                             =294 ;//              144                 144
int MODBUS_FAN1_OPER_TABLE_COAST                             =295 ;//              145                 145
int MODBUS_FAN1_OPER_TABLE_COOL1                             =296 ;//              146                 146
int MODBUS_FAN1_OPER_TABLE_COOL2                             =297 ;//              147                 147
int MODBUS_FAN1_OPER_TABLE_COOL3                             =298 ;//              148                 148
int MODBUS_FAN1_OPER_TABLE_HEAT1                             =299 ;//              149                 149
int MODBUS_FAN1_OPER_TABLE_HEAT2                             =300 ;//              150                 150
int MODBUS_FAN1_OPER_TABLE_HEAT3                             =301 ;//              151                 151
int MODBUS_FAN2_OPER_TABLE_COAST                             =302 ;//              152                 152
int MODBUS_FAN2_OPER_TABLE_COOL1                             =303 ;//              153                 153
int MODBUS_FAN2_OPER_TABLE_COOL2                             =304 ;//              154                 154
int MODBUS_FAN2_OPER_TABLE_COOL3                             =305 ;//              155                 155
int MODBUS_FAN2_OPER_TABLE_HEAT1                             =306 ;//              156                 156
int MODBUS_FAN2_OPER_TABLE_HEAT2                             =307 ;//              157                 157
int MODBUS_FAN2_OPER_TABLE_HEAT3                             =308 ;//              158                 158
int MODBUS_FAN3_OPER_TABLE_COAST                             =309 ;//              159                 159
int MODBUS_FAN3_OPER_TABLE_COOL1                             =310 ;//              160                 160
int MODBUS_FAN3_OPER_TABLE_COOL2                             =311 ;//              161                 161
int MODBUS_FAN3_OPER_TABLE_COOL3                             =312 ;//              162                 162
int MODBUS_FAN3_OPER_TABLE_HEAT1                             =313 ;//              163                 163
int MODBUS_FAN3_OPER_TABLE_HEAT2                             =314 ;//              164                 164
int MODBUS_FAN3_OPER_TABLE_HEAT3                             =315 ;//              165                 165
int MODBUS_FANAUT_OPER_TABLE_COAST                           =316 ;//              166                 166
int MODBUS_FANAUT_OPER_TABLE_COOL1                           =317 ;//              167                 167
int MODBUS_FANAUT_OPER_TABLE_COOL2                           =318 ;//              168                 168
int MODBUS_FANAUT_OPER_TABLE_COOL3                           =319 ;//              169                 169
int MODBUS_FANAUT_OPER_TABLE_HEAT1                           =320 ;//              170                 170
int MODBUS_FANAUT_OPER_TABLE_HEAT2                           =321 ;//              171                 171
int MODBUS_FANAUT_OPER_TABLE_HEAT3                           =322 ;//              172                 172
int MODBUS_VALVE_OPERATION_TABLE_BEGIN                       =323 ;//              173                 173
int MODBUS_VALVE_OPER_TABLE_COOL1                            =324 ;//              174                 174
int MODBUS_VALVE_OPER_TABLE_COOL2                            =325 ;//              175                 175
int MODBUS_VALVE_OPER_TABLE_COOL3                            =326 ;//              176                 176
int MODBUS_VALVE_OPER_TABLE_HEAT1                            =327 ;//              177                 177
int MODBUS_VALVE_OPER_TABLE_HEAT2                            =328 ;//              178                 178
int MODBUS_VALVE_OPER_TABLE_HEAT3                            =329 ;//              179                 179
int MODBUS_HEAT_UNIVERSAL_TABLE                              =330 ;//              268                 268
int MODBUS_COOL_UNIVERSAL_TABLE                              =331 ;//              269                 269
int MODBUS_HEAT_ORIGINAL_TABLE                               =332 ;//              276                 276
int MODBUS_COOL_ORIGINAL_TABLE                               =333 ;//              277                 277
int MODBUS_VALVE_OFF_TABLE_COAST                             =334 ;//              351                 351
int MODBUS_VALVE_OFF_TABLE_COOL1                             =335 ;//              352                 352
int MODBUS_VALVE_OFF_TABLE_COOL2                             =336 ;//              353                 353
int MODBUS_VALVE_OFF_TABLE_COOL3                             =337 ;//              354                 354
int MODBUS_VALVE_OFF_TABLE_HEAT1                             =338 ;//              355                 355
int MODBUS_VALVE_OFF_TABLE_HEAT2                             =339 ;//              356                 356
int MODBUS_VALVE_OFF_TABLE_HEAT3                             =340 ;//              357                 357
int MODBUS_DEFAULT_SETPOINT                                  =341 ;//              338                 338
int MODBUS_SETPOINT_CONTROL                                  =342 ;//              339                 339
int MODBUS_DAYSETPOINT_OPTION                                =343 ;//              381                 423
int MODBUS_MIDDLE_SETPOINT                                   =344 ;//              -1                  -1 
int MODBUS_DAY_SETPOINT                                      =345 ;//              -1                  -1 
int MODBUS_DAY_COOLING_DEADBAND                              =346 ;//              -1                  -1 
int MODBUS_DAY_HEATING_DEADBAND                              =347 ;//              -1                  -1 
int MODBUS_DAY_COOLING_SETPOINT                              =348 ;//              -1                  -1 
int MODBUS_DAY_HEATING_SETPOINT                              =349 ;//              -1                  -1 
int MODBUS_NIGHT_SETPOINT                                    =350 ;//              -1                  -1 
int MODBUS_APPLICATION                                       =351 ;//              125                 125
int MODBUS_NIGHT_HEATING_DEADBAND                            =352 ;//              123                 123
int MODBUS_NIGHT_COOLING_DEADBAND                            =353 ;//              124                 124
int MODBUS_NIGHT_HEATING_SETPOINT                            =354 ;//              182                 182
int MODBUS_NIGHT_COOLING_SETPOINT                            =355 ;//              183                 183
int MODBUS_WINDOW_INTERLOCK_COOLING_SETPOINT                 =356 ;//              -1                  -1 
int MODBUS_WINDOW_INTERLOCK_HEATING_SETPOINT                 =357 ;//              -1                  -1 
int MODBUS_UNIVERSAL_NIGHTSET                                =358 ;//              275                 275
int MODBUS_UNIVERSAL_SET                                     =359 ;//              246                 246
int MODBUS_UNIVERSAL_DB_HI                                   =360 ;//              242                 242
int MODBUS_UNIVERSAL_DB_LO                                   =361 ;//              243                 243
int MODBUS_ECOMONY_COOLING_SETPOINT                          =362 ;//              -1                  -1 
int MODBUS_ECOMONY_HEATING_SETPOINT                          =363 ;//              -1                  -1 
int MODBUS_POWERUP_SETPOINT                                  =364 ;//              126                 126
int MODBUS_MAX_SETPOINT                                      =365 ;//              131                 131
int MODBUS_MIN_SETPOINT                                      =366 ;//              132                 132
int MODBUS_MAX_SETPOINT2                                     =367 ;//              -1                  -1 
int MODBUS_MIN_SETPOINT2                                     =368 ;//              -1                  -1 
int MODBUS_MAX_SETPOINT3                                     =369 ;//              -1                  -1 
int MODBUS_MIN_SETPOINT3                                     =370 ;//              -1                  -1 
int MODBUS_MAX_SETPOINT4                                     =371 ;//              -1                  -1 
int MODBUS_MIN_SETPOINT4                                     =372 ;//              -1                  -1 
int MODBUS_SETPOINT_INCREASE                                 =373 ;//              293                 293
int MODBUS_FREEZE_TEMP_SETPOINT                              =374 ;//              295                 295
int MODBUS_WALL_SETPOINT                                     =375 ;//              -1                  -1 
int MODBUS_CEILING_SETPOINT                                  =376 ;//              -1                  -1 
int MODBUS_AVERAGE_SETPOINT                                  =377 ;//              -1                  -1 
int MODBUS_UNOCCUPIED_HEATING                                =378 ;//              -1                  -1 
int MODBUS_UNOCCUPIED_COOLING                                =379 ;//              -1                  -1 
int MODBUS_RH_SETPOINT                                       =380 ;//              -1                  -1 
int MODBUS_CURRENT1_SETPOINT                                 =381 ;//              -1                  -1 
int MODBUS_TEMP_SELECT                                       =382 ;//              111                 111
int MODBUS_INPUT1_SELECT                                     =383 ;//              241                 241
int MODBUS_COOLING_PID                                       =384 ;//              104                 104
int MODBUS_COOLING_PTERM                                     =385 ;//              114                 114
int MODBUS_COOLING_ITERM                                     =386 ;//              115                 115
int MODBUS_UNIVERSAL_PTERM                                   =387 ;//              244                 244
int MODBUS_UNIVERSAL_ITERM                                   =388 ;//              245                 245
int MODBUS_PID_UNIVERSAL                                     =389 ;//              270                 270
int MODBUS_UNITS1_HIGH                                       =390 ;//              271                 271
int MODBUS_UNITS1_LOW                                        =391 ;//              272                 272
int MODBUS_UNITS2_HIGH                                       =392 ;//              273                 273
int MODBUS_UNITS2_LOW                                        =393 ;//              274                 274
int MODBUS_PID2_MODE_OPERATION                               =394 ;//              278                 278
int MODBUS_KEYPAD_SELECT                                     =395 ;//              128                 128
int MODBUS_SPECIAL_MENU_LOCK                                 =396 ;//              133                 133
int MODBUS_DISPLAY                                           =397 ;//              203                 203
int MODBUS_ICON                                              =398 ;//              -1                  -1 
int MODBUS_LAST_KEY_TIMER                                    =399 ;//              294                 294
int MODBUS_KEYPAD_VALUE                                      =400 ;//              304                 304
int MODBUS_DISPLAY_HUNDRED                                   =401 ;//              305                 305
int MODBUS_DISPLAY_TEN                                       =402 ;//              306                 306
int MODBUS_DISPLAY_DIGITAL                                   =403 ;//              307                 307
int MODBUS_DISPLAY_STATUS                                    =404 ;//              308                 308
int MODBUS_ROUND_DISPLAY                                     =405 ;//              318                 318
int MODBUS_MIN_ADDRESS                                       =406 ;//              319                 319
int MODBUS_MAX_ADDRESS                                       =407 ;//              320                 320
int MODBUS_EEPROM_SIZE                                       =408 ;//              326                 326
int MODBUS_TIMER_SELECT                                      =409 ;//              327                 327
int MODBUS_YEAR                                              =410 ;//              -1                  519
int MODBUS_MONTH                                             =411 ;//              -1                  520
int MODBUS_WEEK                                              =412 ;//              -1                  521
int MODBUS_DAY                                               =413 ;//              -1                  522
int MODBUS_HOUR                                              =414 ;//              -1                  523
int MODBUS_MINUTE                                            =415 ;//              -1                  524
int MODBUS_SECOND                                            =416 ;//              -1                  525
int MODBUS_DIAGNOSTIC_ALARM                                  =417 ;//              -1                  -1 
int MODBUS_DAY1_EVENT1_HI                                    =418 ;//              -1                  -1 
int MODBUS_DAY1_EVENT1_LO                                    =419 ;//              -1                  -1 
int MODBUS_DAY1_EVENT2_HI                                    =420 ;//              -1                  -1 
int MODBUS_DAY1_EVENT2_LO                                    =421 ;//              -1                  -1 
int MODBUS_DAY1_EVENT3_HI                                    =422 ;//              -1                  -1 
int MODBUS_DAY1_EVENT3_LO                                    =423 ;//              -1                  -1 
int MODBUS_DAY1_EVENT4_HI                                    =424 ;//              -1                  -1 
int MODBUS_DAY1_EVENT4_LO                                    =425 ;//              -1                  -1 
int MODBUS_DAY2_EVENT1_HI                                    =426 ;//              -1                  -1 
int MODBUS_DAY2_EVENT1_LO                                    =427 ;//              -1                  -1 
int MODBUS_DAY2_EVENT2_HI                                    =428 ;//              -1                  -1 
int MODBUS_DAY2_EVENT2_LO                                    =429 ;//              -1                  -1 
int MODBUS_DAY2_EVENT3_HI                                    =430 ;//              -1                  -1 
int MODBUS_DAY2_EVENT3_LO                                    =431 ;//              -1                  -1 
int MODBUS_DAY2_EVENT4_HI                                    =432 ;//              -1                  -1 
int MODBUS_DAY2_EVENT4_LO                                    =433 ;//              -1                  -1 
int MODBUS_LCD_TURN_OFF                                      =434 ;//              -1                  399
int MODBUS_LINE1_CHAR1                                       =435 ;//              -1                  383
int MODBUS_LINE1_CHAR2                                       =436 ;//              -1                  384
int MODBUS_LINE1_CHAR3                                       =437 ;//              -1                  385
int MODBUS_LINE1_CHAR4                                       =438 ;//              -1                  386
int MODBUS_LINE2_CHAR1                                       =439 ;//              -1                  391
int MODBUS_LINE2_CHAR2                                       =440 ;//              -1                  392
int MODBUS_LINE2_CHAR3                                       =441 ;//              -1                  393
int MODBUS_LINE2_CHAR4                                       =442 ;//              -1                  394
int MODBUS_INTERNAL_CHAR1                                    =443 ;//              -1                  -1 
int MODBUS_INTERNAL_CHAR2                                    =444 ;//              -1                  -1 
int MODBUS_INTERNAL_CHAR3                                    =445 ;//              -1                  -1 
int MODBUS_INTERNAL_CHAR4                                    =446 ;//              -1                  -1 
int MODBUS_AI1_CHAR1                                         =447 ;//              -1                  -1 
int MODBUS_AI1_CHAR2                                         =448 ;//              -1                  -1 
int MODBUS_AI1_CHAR3                                         =449 ;//              -1                  -1 
int MODBUS_AI1_CHAR4                                         =450 ;//              -1                  -1 
int MODBUS_AI2_CHAR1                                         =451 ;//              -1                  -1 
int MODBUS_AI2_CHAR2                                         =452 ;//              -1                  -1 
int MODBUS_AI2_CHAR3                                         =453 ;//              -1                  -1 
int MODBUS_AI2_CHAR4                                         =454 ;//              -1                  -1 
int MODBUS_AI3_CHAR1                                         =455 ;//              -1                  -1 
int MODBUS_AI3_CHAR2                                         =456 ;//              -1                  -1 
int MODBUS_AI3_CHAR3                                         =457 ;//              -1                  -1 
int MODBUS_AI3_CHAR4                                         =458 ;//              -1                  -1 
int MODBUS_AI4_CHAR1                                         =459 ;//              -1                  -1 
int MODBUS_AI4_CHAR2                                         =460 ;//              -1                  -1 
int MODBUS_AI4_CHAR3                                         =461 ;//              -1                  -1 
int MODBUS_AI4_CHAR4                                         =462 ;//              -1                  -1 
int MODBUS_AI5_CHAR1                                         =463 ;//              -1                  -1 
int MODBUS_AI5_CHAR2                                         =464 ;//              -1                  -1 
int MODBUS_AI5_CHAR3                                         =465 ;//              -1                  -1 
int MODBUS_AI5_CHAR4                                         =466 ;//              -1                  -1 
int MODBUS_AI6_CHAR1                                         =467 ;//              -1                  -1 
int MODBUS_AI6_CHAR2                                         =468 ;//              -1                  -1 
int MODBUS_AI6_CHAR3                                         =469 ;//              -1                  -1 
int MODBUS_AI6_CHAR4                                         =470 ;//              -1                  -1 
int MODBUS_AI7_CHAR1                                         =471 ;//              -1                  -1 
int MODBUS_AI7_CHAR2                                         =472 ;//              -1                  -1 
int MODBUS_AI7_CHAR3                                         =473 ;//              -1                  -1 
int MODBUS_AI7_CHAR4                                         =474 ;//              -1                  -1 
int MODBUS_AI8_CHAR1                                         =475 ;//              -1                  -1 
int MODBUS_AI8_CHAR2                                         =476 ;//              -1                  -1 
int MODBUS_AI8_CHAR3                                         =477 ;//              -1                  -1 
int MODBUS_AI8_CHAR4                                         =478 ;//              -1                  -1 
int MODBUS_OUTPUT1_CHAR1                                     =479 ;//              -1                  469
int MODBUS_OUTPUT1_CHAR2                                     =480 ;//              -1                  470
int MODBUS_OUTPUT1_CHAR3                                     =481 ;//              -1                  471
int MODBUS_OUTPUT1_CHAR4                                     =482 ;//              -1                  472
int MODBUS_OUTPUT2_CHAR1                                     =483 ;//              -1                  473
int MODBUS_OUTPUT2_CHAR2                                     =484 ;//              -1                  474
int MODBUS_OUTPUT2_CHAR3                                     =485 ;//              -1                  475
int MODBUS_OUTPUT2_CHAR4                                     =486 ;//              -1                  476
int MODBUS_OUTPUT3_CHAR1                                     =487 ;//              -1                  477
int MODBUS_OUTPUT3_CHAR2                                     =488 ;//              -1                  478
int MODBUS_OUTPUT3_CHAR3                                     =489 ;//              -1                  479
int MODBUS_OUTPUT3_CHAR4                                     =490 ;//              -1                  480
int MODBUS_OUTPUT4_CHAR1                                     =491 ;//              -1                  481
int MODBUS_OUTPUT4_CHAR2                                     =492 ;//              -1                  482
int MODBUS_OUTPUT4_CHAR3                                     =493 ;//              -1                  483
int MODBUS_OUTPUT4_CHAR4                                     =494 ;//              -1                  484
int MODBUS_OUTPUT5_CHAR1                                     =495 ;//              -1                  485
int MODBUS_OUTPUT5_CHAR2                                     =496 ;//              -1                  486
int MODBUS_OUTPUT5_CHAR3                                     =497 ;//              -1                  487
int MODBUS_OUTPUT5_CHAR4                                     =498 ;//              -1                  488
int MODBUS_OUTPUT6_CHAR1                                     =499 ;//              -1                  489
int MODBUS_OUTPUT6_CHAR2                                     =500 ;//              -1                  490
int MODBUS_OUTPUT6_CHAR3                                     =501 ;//              -1                  491
int MODBUS_OUTPUT6_CHAR4                                     =502 ;//              -1                  492
int MODBUS_OUTPUT7_CHAR1                                     =503 ;//              -1                  493
int MODBUS_OUTPUT7_CHAR2                                     =504 ;//              -1                  494
int MODBUS_OUTPUT7_CHAR3                                     =505 ;//              -1                  495
int MODBUS_OUTPUT7_CHAR4                                     =506 ;//              -1                  496
int MODBUS_SCHEDULEA_CHAR1                                   =507 ;//              -1                  -1 
int MODBUS_SCHEDULEA_CHAR2                                   =508 ;//              -1                  -1 
int MODBUS_SCHEDULEA_CHAR3                                   =509 ;//              -1                  -1 
int MODBUS_SCHEDULEA_CHAR4                                   =510 ;//              -1                  -1 
int MODBUS_SCHEDULEB_CHAR1                                   =511 ;//              -1                  -1 
int MODBUS_SCHEDULEB_CHAR2                                   =512 ;//              -1                  -1 
int MODBUS_SCHEDULEB_CHAR3                                   =513 ;//              -1                  -1 
int MODBUS_SCHEDULEB_CHAR4                                   =514 ;//              -1                  -1 
int MODBUS_SCHEDULEC_CHAR1                                   =515 ;//              -1                  -1 
int MODBUS_SCHEDULEC_CHAR2                                   =516 ;//              -1                  -1 
int MODBUS_SCHEDULEC_CHAR3                                   =517 ;//              -1                  -1 
int MODBUS_SCHEDULEC_CHAR4                                   =518 ;//              -1                  -1 
int MODBUS_SCHEDULED_CHAR1                                   =519 ;//              -1                  -1 
int MODBUS_SCHEDULED_CHAR2                                   =520 ;//              -1                  -1 
int MODBUS_SCHEDULED_CHAR3                                   =521 ;//              -1                  -1 
int MODBUS_SCHEDULED_CHAR4                                   =522 ;//              -1                  -1 
int MODBUS_WALL_CHAR1                                        =523 ;//              -1                  -1 
int MODBUS_WALL_CHAR2                                        =524 ;//              -1                  -1 
int MODBUS_WALL_CHAR3                                        =525 ;//              -1                  -1 
int MODBUS_WALL_CHAR4                                        =526 ;//              -1                  -1 
int MODBUS_CEILING_CHAR1                                     =527 ;//              -1                  -1 
int MODBUS_CEILING_CHAR2                                     =528 ;//              -1                  -1 
int MODBUS_CEILING_CHAR3                                     =529 ;//              -1                  -1 
int MODBUS_CEILING_CHAR4                                     =530 ;//              -1                  -1 
int MODBUS_OUTDOOR_CHAR1                                     =531 ;//              -1                  -1 
int MODBUS_OUTDOOR_CHAR2                                     =532 ;//              -1                  -1 
int MODBUS_OUTDOOR_CHAR3                                     =533 ;//              -1                  -1 
int MODBUS_OUTDOOR_CHAR4                                     =534 ;//              -1                  -1 
int MODBUS_AVERAGE_CHAR1                                     =535 ;//              -1                  -1 
int MODBUS_AVERAGE_CHAR2                                     =536 ;//              -1                  -1 
int MODBUS_AVERAGE_CHAR3                                     =537 ;//              -1                  -1 
int MODBUS_AVERAGE_CHAR4                                     =538 ;//              -1                  -1 
int MODBUS_LCD_SCREEN1                                       =539 ;//              -1                  400
int MODBUS_LCD_SCREEN2                                       =540 ;//              -1                  401
int MODBUS_DEMAND_RESPONSE                                   =541 ;//              -1                  -1 
int MODBUS_LOCK_REGISTER                                     =542 ;//              358                 358
int MODBUS_PIR_STAGE                                         =543 ;//              340                 340
int MODBUS_FIRST_CAL_FLAG                                    =544 ;//              -1                  -1 
int MODBUS_HUM_CAL_EREASE                                    =545 ;//              -1                  -1 
int MODBUS_HUMCOUNT3_H                                       =546 ;//              -1                  -1 
int MODBUS_HUMRH3_H                                          =547 ;//              -1                  -1 
int MODBUS_HUMCOUNT4_H                                       =548 ;//              -1                  -1 
int MODBUS_HUMRH4_H                                          =549 ;//              -1                  -1 
int MODBUS_HUMCOUNT5_H                                       =550 ;//              -1                  -1 
int MODBUS_HUMRH5_H                                          =551 ;//              -1                  -1 
int MODBUS_HUMCOUNT6_H                                       =552 ;//              -1                  -1 
int MODBUS_HUMRH6_H                                          =553 ;//              -1                  -1 
int MODBUS_HUMCOUNT7_H                                       =554 ;//              -1                  -1 
int MODBUS_HUMRH7_H                                          =555 ;//              -1                  -1 
int MODBUS_HUMCOUNT8_H                                       =556 ;//              -1                  -1 
int MODBUS_HUMRH8_H                                          =557 ;//              -1                  -1 
int MODBUS_HUMCOUNT9_H                                       =558 ;//              -1                  -1 
int MODBUS_HUMRH9_H                                          =559 ;//              -1                  -1 
int MODBUS_HUMCOUNT10_H                                      =560 ;//              -1                  -1 
int MODBUS_HUMRH10_H                                         =561 ;//              -1                  -1 
int MODBUS_HUM_LOCK_A                                        =562 ;//              -1                  -1 
int MODBUS_HUM_LOCK_B                                        =563 ;//              -1                  -1 
int MODBUS_LCD_ROTATE_ENABLE                                 =564 ;//              -1                  497
int MODBUS_SCHEDULE_ON_OFF                                   =565 ;//              -1                  -1 
int MODBUS_DISP_ITEM_TEMPERATURE                             =566 ;//              -1                  498
int MODBUS_DISP_ITEM_SETPOINT                                =567 ;//              -1                  499
int MODBUS_DISP_ITEM_AI1                                     =568 ;//              -1                  500
int MODBUS_DISP_ITEM_AI2                                     =569 ;//              -1                  501
int MODBUS_DISP_ITEM_AI3                                     =570 ;//              -1                  502
int MODBUS_DISP_ITEM_AI4                                     =571 ;//              -1                  503
int MODBUS_DISP_ITEM_AI5                                     =572 ;//              -1                  504
int MODBUS_DISP_ITEM_AI6                                     =573 ;//              -1                  505
int MODBUS_DISP_ITEM_AI7                                     =574 ;//              -1                  506
int MODBUS_DISP_ITEM_AI8                                     =575 ;//              -1                  507
int MODBUS_DISP_ITEM_MODE                                    =576 ;//              -1                  515
int MODBUS_DISP_ITEM_USER_INFO                               =577 ;//              -1                  516
int MODBUS_DISP_ITEM_CLOCK_DATE                              =578 ;//              -1                  517
int MODBUS_DISP_ITEM_CLOCK_TIME                              =579 ;//              -1                  518
int MODBUS_DISP_ITEM_EXTENAL_SENSOR1                         =580 ;//              -1                  -1 
int MODBUS_DISP_ITEM_EXTENAL_SENSOR2                         =581 ;//              -1                  -1 
int MODBUS_DISP_ITEM_EXTENAL_SENSOR3                         =582 ;//              -1                  -1 
int MODBUS_DISP_ITEM_EXTENAL_SENSOR4                         =583 ;//              -1                  -1 
int MODBUS_DISP_ITEM_OUT1                                    =584 ;//              -1                  508
int MODBUS_DISP_ITEM_OUT2                                    =585 ;//              -1                  509
int MODBUS_DISP_ITEM_OUT3                                    =586 ;//              -1                  510
int MODBUS_DISP_ITEM_OUT4                                    =587 ;//              -1                  511
int MODBUS_DISP_ITEM_OUT5                                    =588 ;//              -1                  512
int MODBUS_DISP_ITEM_OUT6                                    =589 ;//              -1                  513
int MODBUS_DISP_ITEM_OUT7                                    =590 ;//              -1                  514
int MODBUS_OUTPUT_PWM_AUTO_COAST                             =591 ;//              341                 341
int MODBUS_OUTPUT_PWM_AUTO_COOL1                             =592 ;//              342                 342
int MODBUS_OUTPUT_PWM_AUTO_COOL2                             =593 ;//              343                 343
int MODBUS_OUTPUT_PWM_AUTO_COOL3                             =594 ;//              344                 344
int MODBUS_OUTPUT_PWM_AUTO_HEAT1                             =595 ;//              345                 345
int MODBUS_OUTPUT_PWM_AUTO_HEAT2                             =596 ;//              346                 346
int MODBUS_OUTPUT_PWM_AUTO_HEAT3                             =597 ;//              347                 347
int MODBUS_PWM_OUT4                                          =598 ;//              348                 348
int MODBUS_PWM_OUT5                                          =599 ;//              349                 349
int MODBUS_SUN_ICON_CONTROL                                  =600 ;//              -1                  402
int MODBUS_MOON_ICON_CONTROL                                 =601 ;//              -1                  403
int MODBUS_EXT_SENSOR_TEMP_CAL                               =602 ;//              -1                  -1 
int MODBUS_BUTTON_LEFT1                                      =603 ;//              -1                  -1 
int MODBUS_BUTTON_LEFT2                                      =604 ;//              -1                  -1 
int MODBUS_BUTTON_LEFT3                                      =605 ;//              -1                  -1 
int MODBUS_BUTTON_LEFT4                                      =606 ;//              -1                  -1 
int MODBUS_HUM_HEATER_CONTROL_ENABLE                         =607 ;
int MODBUS_HUM_INPUT_MANUAL_ENABLE                           =608 ;//              -1                  -1 
int MODBUS_HUM_INPUT_MANUAL_VALUE                            =609 ;//              -1                  -1 
int MODBUS_CO2_INPUT_MANUAL_ENABLE                           =610;
int MODBUS_CO2_INPUT_VALUE                                   =611;
int MODBUS_CO2_CALIBRATION_DATA                              =612;
int MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN                        =613 ;//              393                 526
int MODBUS_UNIVERSAL_OFF_OUTPUT_COOL1                        =614 ;//              394                 527
int MODBUS_UNIVERSAL_OFF_OUTPUT_COOL2                        =615 ;//              395                 528
int MODBUS_UNIVERSAL_OFF_OUTPUT_COOL3                        =616 ;//              396                 529
int MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT1                        =617 ;//              397                 530
int MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT2                        =618 ;//              398                 531
int MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT3                        =619 ;//              399                 532
int MODBUS_UNIVERSAL_OFF_VALVE_BEGIN                         =620 ;//              400                 533
int MODBUS_UNIVERSAL_OFF_VALVE_COOL1                         =621 ;//              401                 534
int MODBUS_UNIVERSAL_OFF_VALVE_COOL2                         =622 ;//              402                 535
int MODBUS_UNIVERSAL_OFF_VALVE_COOL3                         =623 ;//              403                 536
int MODBUS_UNIVERSAL_OFF_VALVE_HEAT1                         =624 ;//              404                 537
int MODBUS_UNIVERSAL_OFF_VALVE_HEAT2                         =625 ;//              405                 538
int MODBUS_UNIVERSAL_OFF_VALVE_HEAT3                         =626 ;//              406                 539

int MODBUS_VALUE_SENSOR                                      =628;
int MODBUS_PIR_SELECT_ENABLE                                 =629;
int MODBUS_PIR_REAL_VALUE                                    =630;
int MODBUS_PIR_OCCUPIED                                      =631;
int MODBUS_UNIVERSAL_VALVE_BEGIN                          =632;
int MODBUS_UNIVERSAL_VALVE_COOL1                         =633;
int MODBUS_UNIVERSAL_VALVE_COOL2                         =634;
int MODBUS_UNIVERSAL_VALVE_COOL3                         =635;
int MODBUS_UNIVERSAL_VALVE_HEAT1                            =636;
int MODBUS_UNIVERSAL_VALVE_HEAT2                            =637;
int MODBUS_UNIVERSAL_VALVE_HEAT3                            =638;


int MODBUS_HEATING_PID                                       =-1  ;//              105                 105
int MODBUS_CALIBRATION                                       =-1  ;//              109                 109
int MODBUS_CALIBRATION_ANALOG_IN1                            =-1  ;//              110                 110
int MODBUS_DELAY                                             =-1  ;//              113                 113
int MODBUS_HEATING_PTERM                                     =-1  ;//              116                 116
int MODBUS_HEATING_ITERM                                     =-1  ;//              117                 117
int MODBUS_COOLING_DEADBAND                                  =-1  ;//              119                 119
int MODBUS_HEATING_DEADBAND                                  =-1  ;//              120                 120
int MODBUS_OUTPUT_SCALE                                      =-1  ;//              130                 130
int MODBUS_COOLING_SETPOINT                                  =-1  ;//              135                 135
int MODBUS_HEATING_SETPOINT                                  =-1  ;//              136                 136
int MODBUS_EXTERNAL_SENSOR_0                                 =-1  ;//              180                 180
int MODBUS_EXTERNAL_SENSOR_1                                 =-1  ;//              181                 181
int MODBUS_ANALOG_IN1                                        =-1  ;//              188                 188
int MODBUS_ANALOG_IN2                                        =-1  ;//              189                 189
int MODBUS_DIGITAL_IN1                                       =-1  ;//              190                 190
int MODBUS_LED1_CONTROL                                      =-1  ;//              204                 204
int MODBUS_LED2_CONTROL                                      =-1  ;//              205                 205
int MODBUS_LED3_CONTROL                                      =-1  ;//              206                 206
int MODBUS_LED4_CONTROL                                      =-1  ;//              207                 207
int MODBUS_LED5_CONTROL                                      =-1  ;//              208                 208
int MODBUS_LED6_CONTROL                                      =-1  ;//              209                 209
int MODBUS_LED7_CONTROL                                      =-1  ;//              210                 210
int MODBUS_INTERNAL_TEMP_IC                                  =-1  ;//              215                 215
int MODBUS_CALIBRATION_ANALOG_IN2                            =-1  ;//              218                 218
//int MODBUS_UNIVERSAL_VALVE_BEGIN                             =-1  ;//              261                 261
//int MODBUS_UNIVERSAL_VALVE_COOL1                             =-1  ;//              262                 262
//int MODBUS_UNIVERSAL_VALVE_COOL2                             =-1  ;//              263                 263
//int MODBUS_UNIVERSAL_VALVE_COOL3                             =-1  ;//              264                 264
//// MODBUS_UNIVERSAL_VALVE_HEAT1                             =-1  ;//              265                 265
//int MODBUS_UNIVERSAL_VALVE_HEAT2                             =-1  ;//              266                 266
//int MODBUS_UNIVERSAL_VALVE_HEAT3                             =-1  ;//              267                 267
int MODBUS_DIGITAL1_FUNCTION                                 =-1  ;//              300                 300
int MODBUS_DIGITAL_INPUT                                     =-1  ;//              311                 311
int MODBUS_DEBUGTIMER                                        =-1  ;//              316                 316
int MODBUS_FAN_GRIDPOINT                                     =-1  ;//              321                 321
int MODBUS_MODE_GRIDPOINT                                    =-1  ;//              322                 322
int MODBUS_HOLD_GRIDPOINT                                    =-1  ;//              323                 323
int MODBUS_CONFIGURATION                                     =-1  ;//              324                 324
int MODBUS_UTIL_MODE                                         =-1  ;//              325                 325
int MODBUS_ROTATION_OUT2                                     =-1  ;//              329                 329
int MODBUS_ROTATION_OUT3                                     =-1  ;//              330                 330
int MODBUS_ROTATION_OUT4                                     =-1  ;//              331                 331
int MODBUS_ROTATION_OUT5                                     =-1  ;//              332                 332
int MODBUS_ROTATION_TIME_LEFT                                =-1  ;//              333                 333
int MODBUS_FREE_COOL_OUT_TEMP                                =-1  ;//              359                 -1 
int MODBUS_NC_MASTER_TIME                                    =-1  ;//              360                 -1 
int MODBUS_FREE_COOL_HYSTERISIS                              =-1  ;//              361                 -1 
int MODBUS_OUTPUT_FREE_COOL_COAST                            =-1  ;//              362                 -1 
int MODBUS_OUTPUT_FREE_COOL_COOL1                            =-1  ;//              363                 -1 
int MODBUS_OUTPUT_FREE_COOL_COOL2                            =-1  ;//              364                 -1 
int MODBUS_OUTPUT_FREE_COOL_COOL3                            =-1  ;//              365                 -1 
int MODBUS_OUTPUT_FREE_COOL_HEAT1                            =-1  ;//              366                 -1 
int MODBUS_OUTPUT_FREE_COOL_HEAT2                            =-1  ;//              367                 -1 
int MODBUS_OUTPUT_FREE_COOL_HEAT3                            =-1  ;//              368                 -1 
int MODBUS_MIN_AIR                                           =-1  ;//              369                 -1 
int MODBUS_OAT_HOTTEST                                       =-1  ;//              370                 415
int MODBUS_OAT_COLDEST                                       =-1  ;//              371                 416
int MODBUS_OFFSET_HOTTEST                                    =-1  ;//              372                 417
int MODBUS_OFFSET_COLDEST                                    =-1  ;//              373                 418
int MODBUS_TWO_BYTE_SETPOINT                                 =-1  ;//              374                 -1 
int MODBUS_CURRENT_SETPOINT                                  =-1  ;//              375                 419
int MODBUS_SETPOINT_OFFSET                                   =-1  ;//              376                 420
int MODBUS_CHANGEOVER_SENSOR_MODE                            =-1  ;//              377                 -1 
int MODBUS_EXTERNAL_SENSOR_INUSE                             =-1  ;//              378                 -1 
int MODBUS_EXTERNAL_SENSOR_STATUS                            =-1  ;//              379                 -1 
int MODBUS_NEW_COOLING_SETPOINT                              =-1  ;//              380                 422
int MODBUS_LINE1_CHAR5                                       =-1  ;//              -1                  387
int MODBUS_LINE1_CHAR6                                       =-1  ;//              -1                  388
int MODBUS_LINE1_CHAR7                                       =-1  ;//              -1                  389
int MODBUS_LINE1_CHAR8                                       =-1  ;//              -1                  390
int MODBUS_WINDOW_INTERLOCK_DELAY                            =-1  ;//              392                 424
int MODBUS_LINE2_CHAR5                                       =-1  ;//              -1                  395
int MODBUS_LINE2_CHAR6                                       =-1  ;//              -1                  396
int MODBUS_LINE2_CHAR7                                       =-1  ;//              -1                  397
int MODBUS_LINE2_CHAR8                                       =-1  ;//              -1                  398
int MODBUS_DURA_WINTER_SUMMER                                =-1  ;//              407                 -1 
int MODBUS_FREE_COOL_SETPOINT                                =-1  ;//              408                 -1 
int MODBUS_FREE_COOL_MAX_SETPOINT                            =-1  ;//              409                 -1 
int MODBUS_FREE_COOL_MIN_SETPOINT                            =-1  ;//              410                 -1 
int MODBUS_FREE_COOL_MIN_DAMPER                              =-1  ;//              411                 -1 
int MODBUS_HEAT_COOL                                         =-1  ;//              -1                  404
int MODBUS_TURN_OFF_FAN                                      =-1  ;//              -1                  421
int MODBUS_UI_LINE1_CHAR1                                    =-1  ;//              -1                  425
int MODBUS_UI_LINE1_CHAR2                                    =-1  ;//              -1                  426
int MODBUS_UI_LINE1_CHAR3                                    =-1  ;//              -1                  427
int MODBUS_UI_LINE1_CHAR4                                    =-1  ;//              -1                  428
int MODBUS_UI_LINE2_CHAR1                                    =-1  ;//              -1                  429
int MODBUS_UI_LINE2_CHAR2                                    =-1  ;//              -1                  430
int MODBUS_UI_LINE2_CHAR3                                    =-1  ;//              -1                  431
int MODBUS_UI_LINE2_CHAR4                                    =-1  ;//              -1                  432
int MODBUS_UI_INTERNAL_CHAR1                                 =-1  ;//              -1                  433
int MODBUS_UI_INTERNAL_CHAR2                                 =-1  ;//              -1                  434
int MODBUS_UI_INTERNAL_CHAR3                                 =-1  ;//              -1                  435
int MODBUS_UI_INTERNAL_CHAR4                                 =-1  ;//              -1                  436
int MODBUS_UI_AI1_CHAR1                                      =-1  ;//              -1                  437
int MODBUS_UI_AI1_CHAR2                                      =-1  ;//              -1                  438
int MODBUS_UI_AI1_CHAR3                                      =-1  ;//              -1                  439
int MODBUS_UI_AI1_CHAR4                                      =-1  ;//              -1                  440
int MODBUS_UI_AI2_CHAR1                                      =-1  ;//              -1                  441
int MODBUS_UI_AI2_CHAR2                                      =-1  ;//              -1                  442
int MODBUS_UI_AI2_CHAR3                                      =-1  ;//              -1                  443
int MODBUS_UI_AI2_CHAR4                                      =-1  ;//              -1                  444
int MODBUS_UI_AI3_CHAR1                                      =-1  ;//              -1                  445
int MODBUS_UI_AI3_CHAR2                                      =-1  ;//              -1                  446
int MODBUS_UI_AI3_CHAR3                                      =-1  ;//              -1                  447
int MODBUS_UI_AI3_CHAR4                                      =-1  ;//              -1                  448
int MODBUS_UI_AI4_CHAR1                                      =-1  ;//              -1                  449
int MODBUS_UI_AI4_CHAR2                                      =-1  ;//              -1                  450
int MODBUS_UI_AI4_CHAR3                                      =-1  ;//              -1                  451
int MODBUS_UI_AI4_CHAR4                                      =-1  ;//              -1                  452
int MODBUS_UI_AI5_CHAR1                                      =-1  ;//              -1                  453
int MODBUS_UI_AI5_CHAR2                                      =-1  ;//              -1                  454
int MODBUS_UI_AI5_CHAR3                                      =-1  ;//              -1                  455
int MODBUS_UI_AI5_CHAR4                                      =-1  ;//              -1                  456
int MODBUS_UI_AI6_CHAR1                                      =-1  ;//              -1                  457
int MODBUS_UI_AI6_CHAR2                                      =-1  ;//              -1                  458
int MODBUS_UI_AI6_CHAR3                                      =-1  ;//              -1                  459
int MODBUS_UI_AI6_CHAR4                                      =-1  ;//              -1                  460
int MODBUS_UI_AI7_CHAR1                                      =-1  ;//              -1                  461
int MODBUS_UI_AI7_CHAR2                                      =-1  ;//              -1                  462
int MODBUS_UI_AI7_CHAR3                                      =-1  ;//              -1                  463
int MODBUS_UI_AI7_CHAR4                                      =-1  ;//              -1                  464
int MODBUS_UI_AI8_CHAR1                                      =-1  ;//              -1                  465
int MODBUS_UI_AI8_CHAR2                                      =-1  ;//              -1                  466
int MODBUS_UI_AI8_CHAR3                                      =-1  ;//              -1                  467
int MODBUS_UI_AI8_CHAR4                                      =-1  ;//              -1                  468
int MODBUS_WINDOW_HEATING_SETPOINT                           =-1  ;//              -1                  540
int MODBUS_WINDOW_COOLING_SETPOINT                           =-1  ;//              -1                  541
int MODBUS_RELAY_PWM_ENABLE                                  =-1  ;//              -1                  542
int MODBUS_RELAY_PWM_TOTAL_DUTY                              =-1  ;//              -1                  543
int MODBUS_RELAY_PWM_HIGH_DUTY                               =-1  ;//              -1                  544
int	MODBUS_SUPPLY_SETPOINT	=	-1	;
int	MODBUS_MAX_SUPPLY_SETPOINT	=	-1	;
int	MODBUS_MIN_SUPPLY_SETPOINT	=	-1	;
int	MODBUS_MAX_AIRFLOW_COOLING	=	-1	;
int	MODBUS_MAX_AIRFLOW_HEATING	=	-1	;
int	MODBUS_MIN_AIRFLOW	=	-1	;
int	MODBUS_AIRFLOW_SETPOINT 	=	-1	;
int	MODBUS_VAV_CONTROL	=	-1	;
int	MODBUS_SUPPLY_TEMP_SELECT	=	-1	;
int	MODBUS_PID3_VALVE_OPERATION_TABLE_BEGIN 	=	-1	;
int	MODBUS_PID3_VALVE_OPER_TABLE_COOL1	=	-1	;
int	MODBUS_PID3_VALVE_OPER_TABLE_COOL2	=	-1	;
int	MODBUS_PID3_VALVE_OPER_TABLE_COOL3	=	-1	;
int	MODBUS_PID3_VALVE_OPER_TABLE_HEAT1	=	-1	;
int	MODBUS_PID3_VALVE_OPER_TABLE_HEAT2	=	-1	;
int	MODBUS_PID3_VALVE_OPER_TABLE_HEAT3	=	-1	;
int	MODBUS_PID3_COOLING_DB	=	-1	;
int	MODBUS_PID3_HEATING_DB	=	-1	;
int	MODBUS_PID3_PTERM	=	-1	;
int	MODBUS_PID3_ITERM	=	-1	;
int	MODBUS_PID3_HEAT_STAGE	=	-1	;
int	MODBUS_PID3_COOL_STAGE	=	-1	;
int	MODBUS_PID3_OUTPUT	=	-1	;
int	MODBUS_PID3_OUTPUT_BEGIN	=	-1	;
int	MODBUS_PID3_OUTPUT_COOL1	=	-1	;
int	MODBUS_PID3_OUTPUT_COOL2	=	-1	;
int	MODBUS_PID3_OUTPUT_COOL3	=	-1	;
int	MODBUS_PID3_OUTPUT_HEAT1	=	-1	;
int	MODBUS_PID3_OUTPUT_HEAT2	=	-1	;
int	MODBUS_PID3_OUTPUT_HEAT3	=	-1	;
//int	MODBUS_OUTPUT6_FUNCTION	=	-1	;
//int	MODBUS_OUTPUT7_FUNCTION	=	-1	;
//int	MODBUS_ANALOG3_FUNCTION	=	-1	;
//int	MODBUS_ANALOG4_FUNCTION	=	-1	;
//int	MODBUS_ANALOG5_FUNCTION	=	-1	;
//int	MODBUS_ANALOG6_FUNCTION	=	-1	;
//int	MODBUS_ANALOG7_FUNCTION	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_BEGIN  	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_COOL1  	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_COOL2  	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_COOL3  	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_HEAT1  	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_HEAT2  	=	-1	;
int	MODBUS_PID3_VALVE_OFF_TABLE_HEAT3  	=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_BEGIN		=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_COOL1		=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_COOL2		=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_COOL3		=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_HEAT1		=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_HEAT2		=	-1	;
int	MODBUS_PID3_OFF_OUTPUT_HEAT3		=	-1	;

int MODBUS_HEAT_COOL_MODE   =                691;

int MODBUS_PID3_DAY_SETPOINT = 705;
int MODBUS_PID3_NIGHT_SETPOINT = 706;

int MODBUS_PID_D_TERM       =                799;
int MODBUS_PID_SAMPLE_TIME  =                800;
//used for record the selected product , if user want to update this product ,we need to check whether it is on line or not.
int selected_product_index = -1;
HTREEITEM selected_tree_item = NULL;
#pragma region For_bacnet


CString temp_off[BAC_CUSTOMER_UNITS_COUNT];		//用于 保存 客户自定义的 单位;
CString temp_on[BAC_CUSTOMER_UNITS_COUNT];
CString temp_unit[BAC_CUSTOMER_UNITS_COUNT];
CString Custom_Digital_Range[BAC_CUSTOMER_UNITS_COUNT];
bool read_customer_unit;	//如果这个设备没有读过 customer unit这一项,就要尝试去读，以前老版本的没有;
bool receive_customer_unit; //收到回复，flag就置 true;
bool read_analog_customer_unit;  // 这个是模拟的cus tabel ;
bool read_var_analog_cus_units;          //Var Cus units 自定义
CString Analog_Customer_Units[BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT];
CString Analog_Variable_Units[BAC_VARIABLE_CUS_UNIT_COUNT];

unsigned char bacnet_add_id[254];
int bacnet_device_type;
int g_bac_instance;
unsigned int g_sub_instace;
unsigned int g_selected_serialnumber;
unsigned int g_selected_product_id;
unsigned short g_mac;
HWND MainFram_hwd;
HWND BacNet_hwd;
int bac_ranges_type;
int bac_range_number_choose;

int bac_program_pool_size;
int bac_program_size;
int bac_free_memory;
int bac_read_which_list;
bool bac_read_all_results;
bool bac_input_read_results;
bool bac_output_read_results;
bool bac_variable_read_results;
bool bac_program_read_results;
bool bac_weekly_read_results;
bool bac_annual_read_results;
bool bac_time_command_read_results;
bool bac_controller_read_results;
bool bac_screen_read_results;
bool bac_monitor_read_results;
bool bac_programcode_read_results;
bool bac_weeklycode_read_results;
bool bac_annualcode_read_results;
bool bac_alarmlog_read_results;
bool bac_tstat_read_results;
bool bac_basic_setting_read_results;
bool bac_customer_unit_read_results;
bool bac_user_login_read_results;
bool bac_graphic_label_read_results;
bool bac_remote_point_read_results;

bool bac_cm5_graphic;
int bac_gloab_panel;
//int g_bac_instance;
int input_list_line;
int output_list_line;
int program_list_line ;
int weekly_list_line ;
int annual_list_line ;
int screen_list_line ;
int monitor_list_line;
int analog_range_tbl_line;
int ext_io_list_line;

HWND      g_hwnd_now;
HWND      m_input_dlg_hwnd;
HWND      m_output_dlg_hwnd;
HWND      m_pragram_dlg_hwnd;
HWND      m_program_edit_hwnd;
HWND      m_variable_dlg_hwnd;
HWND      m_weekly_dlg_hwnd;
HWND      m_annual_dlg_hwnd;
HWND      m_schedule_time_dlg_hwnd;
HWND      m_schedule_day_dlg_hwnd;
HWND      m_controller_dlg_hwnd;
HWND      m_screen_dlg_hwnd;
HWND	  m_screenedit_dlg_hwnd;
HWND	  m_monitor_dlg_hwnd;
HWND	  m_alarmlog_dlg_hwnd;
HWND      m_alarmwindow_dlg_hwnd;
HWND      m_tstat_dlg_hwnd;
HWND      m_setting_dlg_hwnd;
HWND      m_flash_multy_hwnd;
HWND      m_customer_digital_range_dlg_hwnd;
HWND      m_building_config_hwnd;
HWND      m_scan_dlg_hwnd;
HWND	  m_tcp_server_hwnd;
HWND      m_user_login_hwnd;
HWND      m_user_config_hwnd;
HWND      m_add_label = NULL;
HWND      m_edit_label = NULL;
HWND      m_at_command_hwnd = NULL;
HWND      m_remote_point_hwnd = NULL;
HWND	  m_program_debug_list_hwnd = NULL;
HWND	  m_ext_io_dlg_hwmd = NULL;
HWND      m_tstat_schedule_dlg_hwnd = NULL;

HWND analog_cus_range_dlg=NULL;

HWND	  m_statusbar_hwnd = NULL;
HWND      m_t3000_log_window = NULL;
vector <Str_out_point> m_Output_data;
vector <Str_in_point>  m_Input_data;
vector <Str_program_point>  m_Program_data;
vector <Str_variable_point>  m_Variable_data;
vector <Str_weekly_routine_point> m_Weekly_data;
vector <Str_annual_routine_point> m_Annual_data;
vector <Str_schedual_time_point> m_Schedual_Time_data;
vector <Str_controller_point> m_controller_data;
vector <Control_group_point> m_screen_data;
vector <Str_tstat_schedule> m_tatat_schedule_data;
vector <Str_monitor_point> m_monitor_data;
vector <_Bac_Scan_Com_Info> m_bac_handle_Iam_data;
vector <_Bac_Scan_results_Info> m_bac_scan_result_data;
vector <Alarm_point> m_alarmlog_data;
vector <Str_TstatInfo_point> m_Tstat_data;
vector <Str_Remote_TstDB> m_remote_device_db;
vector <Str_Units_element> m_customer_unit_data;
vector <Str_userlogin_point> m_user_login_data;
vector <Client_Info> m_tcp_connect_info;
vector <Str_label_point> m_graphic_label_data;	//图片里面的Label的信息要存在设备里面;
vector <Str_remote_point> m_remote_point_data;  //Mini panel 里面Tstat 远端点的 值;
vector <Str_table_point> m_analog_custmer_range;
vector <Str_variable_uint_point> m_variable_analog_unite;
vector <Str_Extio_point> m_extio_config_data;

Time_block_mini Device_time;
Str_Setting_Info Device_Basic_Setting;
Str_MISC Device_Misc_Data;
Str_Special Device_Special_Data;
char m_at_write_buf[100];
char m_at_read_buf[450];

vector <int> exsit_panel_number;
vector <refresh_net_device> m_T3BB_device_data;
vector <refresh_net_device> m_refresh_net_device_data;
vector <GSM_connection_info> m_gsm_connect_info;
vector <Scan_Info> m_scan_info;
vector <Scan_Info> m_scan_info_buffer;
Monitor_Block m_monitor_block;
Str_Monitor_data_header m_monitor_head;//用来接收monitor 的头，里面有需要接收的总包和目前是第几包;
char picture_data_buffer[400]; //用来存每包 pic 的数据.
Str_picture_header m_picture_head; //传图片的 头，结构其实是和 monitor same.
int Monitor_Input__Data[14][1000];	

int controller_counter = 0; //记录扫描的子节点




vector <_Graphic_Value_Info> m_graphic_refresh_data;

byte	g_DayState[8][ANNUAL_CODE_SIZE];
unsigned char weeklt_time_schedule[BAC_SCHEDULE_COUNT][WEEKLY_SCHEDULE_SIZE + 1];
unsigned char program_code[BAC_PROGRAM_ITEM_COUNT][2000];//暂定2000;
int program_code_length[BAC_PROGRAM_ITEM_COUNT];

BOOL g_mstp_flag;

unsigned long timesec1970; 
unsigned long timestart;   

CString bac_cs_mac;
CString bac_cs_device_id;
//CString bac_cs_vendor_id;

HANDLE CM5_hThread = NULL;
HANDLE CM5_UI_Thread;
DWORD nThreadID_x;
DWORD cm5_nThreadID;
CDialog *pDialog[14];
CDialog *DebugWindow;
CDialog *T3000LogWindow = NULL;
CDialog *Tcp_Server_Window =NULL;
HWND h_debug_window;
CString PrintText[1000];
CString g_Print;
bool range_cancel;//用于监测Range 对话框是否正常修改，如果正常修改就为0，否则就为1;
int g_protocol=PROTOCOL_UNKNOW;
int g_bac_read_type;	//用于记录将要读取哪一个，input 还是output,给线程使用;
bool g_bac_need_read_setting;  //如果是第一次点击 需要读Setting里面的 数据;判断是否需要更改Label之类的;
HANDLE click_read_thread;
bool bac_net_initial_once;
unsigned char my_ip[4];
int connect_invoke_id = -1;
//bool connect_replay = false;
BACNET_OBJECT_PROPERTY_VALUE receive_object_value;  /* for bacapp printing */
bool bac_show_alarm_window = false;	//used for judge whether need to show the alarm window.
bool bac_select_device_online = false; //判断选中的bacnet device 是否在线;
SOCKET h_Broad=NULL;
SOCKADDR_IN h_siBind;
SOCKADDR_IN h_bcast;

Point_Data_str digital_last_data[MAX_POINTS_IN_MONITOR];//用来存 数字量 没变化的情况下又 不在时间轴范围内的值;
Data_Time_Match * digital_data_point[MAX_POINTS_IN_MONITOR];
Data_Time_Match * analog_data_point[MAX_POINTS_IN_MONITOR];
int analog_data_max_value[MAX_POINTS_IN_MONITOR];
int analog_data_min_value[MAX_POINTS_IN_MONITOR];
int digital_data_count[MAX_POINTS_IN_MONITOR];	//用于记录digital data 这个指针到底存了多少数据;
int analog_data_count[MAX_POINTS_IN_MONITOR];	//用于记录analog data 这个指针到底存了多少数据;
int get_data_count = 0;
int monitor_analog_count = 0;	//暂存monitor 的模拟和数字的个数;
int monitor_digital_count = 0;
Data_Time_Match * temp_analog_data[MAX_POINTS_IN_MONITOR];	//临时存放新建的块;
int SEND_COMMAND_DELAY_TIME = 100;
bool TCP_Server_Running = false; //开启TCP 服务器就将标志位置1;
bool Gsm_communication = false;	//如果是通过GSM 读远程设备 许多地方要特殊处理;不能刷新太频繁;
CString SaveConfigFilePath;
CString LoadConfigFilePath;
vector<ALL_LOCAL_SUBNET_NODE> g_Vector_Subnet;
vector<ALL_LOCAL_SUBNET_NODE> g_Scan_Vector_Subnet;

CString g_strStartInterface_config = _T("T3000_config.ini");
int g_SleepTimeForConfig = 3;

vector<Reg_Infor> g_Vector_Write_Error;
bool need_read_bacnet_graphic_label_flag = true;	//用于防止重复读取 graphic label ;只有切换设备的时候才需要再次读取;
bool read_write_bacnet_config = false;	//读写Bacnet config 的时候禁止刷新 List;

 vector <Tstat_Input_Struct> m_tstat_input_data;
 vector <Tstat_Output_Struct> m_tstat_output_data;

 // CString analog_range[ANALOG_RANG_NUMBER]={_T("Raw"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
 // CString analog_range[ANALOG_RANG_NUMBER]={_T("Raw"),_T("10KF Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
 CString analog_range[11]={_T("UNUSED"),_T("10K Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor1"),_T("Off/On"),_T("Custom Sensor2"),_T("Occupied/Unoccupied"),_T("Unoccupied/Occupied"),_T("Open/Close"),_T("Close/Open")};
 CString analog_range_TSTAT6[15]={_T("-"),_T("10K Thermistor Type2"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor1"),_T("Off/On"),_T("Custom Sensor2"),_T("Occupied/Unoccupied"),_T("Unoccupied/Occupied"),_T("Open/Close"),_T("Close/Open"),_T("10K Thermistor Type3"),_T("4-20ma"), _T("50K Thermistor"), _T("Voltage")};

 CString INPUT_FUNS[13]=
 {_T("Normal"),
  _T("Freeze Protect"),
  _T("Occupancy Sensor"),
  _T("Sweep Off"),
  _T("Clock"),
  _T("Changeover Mode"),
  _T("Outside Temp"),
  _T("Airflow"),
  _T("Heat"),
  _T("Cool"), 
  _T("Potentionmeter"), 
  _T("Temperature"), 
  _T("Fan")};


 CString Interlock[6]={_T("ON"),_T("DI1"),_T("AI1") ,_T("AI2"),_T("TIMER OR"),_T("TIMER AND")};
 CString ONTPUT_FUNS[5]={_T("Normal"),_T("Rotation Timer"),_T("Lighting Control") ,_T("VAV Control"),_T("Transducer")};

 CString OUTPUT_RANGE5[3]={_T("On/Off"),_T("Float(0-100%)"),_T("PWM(0-100%)")};

 CString OUTPUT_RANGE45[4]={_T("On/Off"),_T("Float(Cooling)"),_T("Float(Heating)"),_T("PWM(0-100%)")};//2.5.0.98

 CString OUTPUT_ANRANGE6[5]={_T("On/Off"),_T("0-10V(100%)"),_T("0-5V(100%)"),_T("2-10V(100%)"),_T("PWM(0-100%)")};

 CString OUTPUT_ANRANGE[18]={_T("On/Off"),_T("0-10V(100%)"),_T("0-5V(100%)"),_T("2-10V(100%)"),_T("10-0V(100%)"),_T("Internal Sensor"),_T("Setpoint"),
 	 _T("AI1"),_T("AI2"),_T("AI3"),_T("AI4"),_T("AI5"),_T("AI6"),_T("AI7"),_T("AI8"),_T("Hum Sensor"),_T("CO2 Sensor"),_T("PWM(0-100%)")};



AddressMap TSTAT_6_ADDRESS[2000];
AddressMap TSTAT_5EH_LCD_ADDRESS[2000];
AddressMap TSTAT_5ABCDFG_LED_ADDRESS[2000];


AddressMap T3_8AI8AO[T3_REG_TOTAL_COUNT];
AddressMap T3_8AI16O[T3_REG_TOTAL_COUNT];
AddressMap T3_32AI[T3_REG_TOTAL_COUNT];
AddressMap T3_Performance[T3_REG_TOTAL_COUNT];
AddressMap T3_4AO[T3_REG_TOTAL_COUNT];
AddressMap T3_6CT[T3_REG_TOTAL_COUNT];
AddressMap T3_28IN[T3_REG_TOTAL_COUNT];
AddressMap T3_RTD[T3_REG_TOTAL_COUNT];
AddressMap T3_8I13O[T3_REG_TOTAL_COUNT];



int current_building_protocol;
int current_building_comport;
int current_building_baudrate;
int current_building_ip;
int current_building_ipport;


// 需要刷新--在MainFrm.cpp中的线程中需要判断这个变量
//主要是两个地方
//1>T3000View-Fresh()--进入到这里说明是需要刷新的因为这个是Tstat

BOOL g_NEED_MULTI_READ = FALSE;

//AC.0:0~10A,1:0~20A.2:0~50A. DC.10:0-100A
CString CS3000_INPUT_RANGE[4]={L"0-10A",L"0-20A",L"0-50A",L"0-100A"};
CString STRING_SWITCH_STATUS[3]={_T("Off"),_T("Hand"),_T("Auto")};
int m_user_level = 2;

vector <IspModeInfo> g_isp_device_info;
IspModeInfo need_isp_device;

vector<Registers_Infor> g_vectRegisters;

BOOL g_fresh_Graphic = FALSE;

Global_Calibration_Module   g_calibration_module_data;

CDialog *g_Draw_dlg=NULL;
unsigned int g_progress_persent = 0;	//用来记录全局状态栏该显示的 进度 百分比;
bool b_stop_read_grp_label = false;		//如果读到空的grp label 就不要继续读下一个了;
bool b_stop_read_tstat_schedule = false;	//如果读到空的tstat 就不要继续读下一个了;

CString Statuspanel;   //在状态栏显示panel 2-3;

CString g_cstring_ini_path;
unsigned char product_sort_way;		//0 default   1 by connection      2 bu floor
int input_item_limit_count = BAC_INPUT_ITEM_COUNT;	//input list 要显示多少个input 的个数， 不是根据vector 的size 来判断大小;
int output_item_limit_count = BAC_OUTPUT_ITEM_COUNT;	//output list 要显示多少个output 的个数， 不是根据vector 的size 来判断大小;
int variable_item_limit_count = BAC_VARIABLE_ITEM_COUNT;	//variable list 要显示多少个variable 的个数， 不是根据vector 的size 来判断大小;
int program_item_limit_count = BAC_PROGRAM_ITEM_COUNT;
int controller_item_limit_count = BAC_PID_COUNT;
int screen_item_limit_count = BAC_SCREEN_COUNT;

Str_out_point m_temp_output_data[BAC_OUTPUT_ITEM_COUNT];
Str_in_point m_temp_Input_data[BAC_INPUT_ITEM_COUNT];
Str_variable_point m_temp_variable_data[BAC_VARIABLE_ITEM_COUNT];
Str_program_point m_temp_program_data[BAC_PROGRAM_ITEM_COUNT];
Str_controller_point m_temp_controller_data[BAC_PID_COUNT];
Control_group_point m_temp_screen_data[BAC_SCREEN_COUNT];
Str_weekly_routine_point m_temp_weekly_data[BAC_SCHEDULE_COUNT];
Str_annual_routine_point m_temp_annual_data[BAC_HOLIDAY_COUNT];
Str_monitor_point m_temp_monitor_data[BAC_MONITOR_COUNT];
Alarm_point	 m_temp_alarmlog_data[BAC_ALARMLOG_COUNT];

char monitor_database_flag[24];   //用于标记哪些Database需要删除的 ，1 为删除;
int debug_item_show = 0;
bool monitor_ignore_enable = false;
int monitor_ignore_max_value = 0;
int monitor_ignore_min_value = 0;
int persent_array_count = 0;
unsigned int old_tx_count = 0;
HANDLE read_each_485_fun_thread = NULL;
bool hide_485_progress = false;
bool new_device_support_mini_ui = false;

int flash_multi_auto = false; //用于控制 多烧的  自动控制;
unsigned int T3000_Version ; //T3000的版本号.

int flag_auto_scroll = 0; // 如果客户有指定显示X轴的某些事件 此 变量会为 1;否则显示最新数据;
int graphic_last_scale_type = 0 ; //用于存储上次 有效的 X轴的 刻度;
unsigned int graphic_last_time_value =0 ; //x轴上次查看的时间; 
const CString c_strBaudate[NUMBER_BAUDRATE] =
{
    L"9600",
    L"19200",
    L"38400",
    L"57600",
    L"115200"
};
#pragma endregion For_bacnet

int global_interface = 1; //用来标识全局的界面在哪？主要是 Input 和 output;

bool b_remote_connection = false;  //全局的 用来判断 对远程设备的 特殊处理;
bool refresh_tree_status_immediately = false;	//如果置为treu了，就会立即调用 刷新 ，不用等待一个周期，用于 某些IP或状态变了，立即刷新;
unsigned int remote_connect_serial_number = 0; // 远程连接的序列号;
char ptpLoginName[30];	//远程连接的账号和密码;
char ptpLoginPassword[20];
refresh_net_device device_id_data_1;
refresh_net_device device_id_data_2;

refresh_net_device device_same_panel_1;
refresh_net_device device_same_panel_2;

refresh_net_device device_ip_conflict_1;
refresh_net_device device_ip_conflict_2;

bool edit_confilct_mode = false;

bool scaning_mode = false;

bool isp_mode_is_cancel = true;
bool isp_mode_firmware_auto;

int isp_mode_error_code = 0;

CString isp_mode_detect_firmware_path;

	HANDLE hwait_scan_thread;

unsigned char nDefaultDisplayType;
unsigned char nDefaultTextPlace;
unsigned char nDefaultIconSize;
COLORREF	  nDefaultclrTxt;

int bacnet_view_number = TYPE_INPUT;
int input_item_select_for_range = 0;

bitset<65536*3> read_analog_package; //用于记录trendlog 已经记录了哪些模拟包
bitset<65536*3> read_dig_package;    //用于记录trendlog 已经记录了哪些数字包

int graphic_view_index ;
CString grapgic_view_name[3];
bool graphic_view_visible[14];
CString bacnet_message_input_title;	//输入的提示信息
CString bacnet_message_return_string;  //得到的输入字符串
pidname_map product_map;
pid_reglist_map product_reglist_map;
bool offline_mode = false; //全局离线模式判断;
CString offline_prg_path;   //离线模式得prg 保存路径;

bac_mstp_com g_mstp_com; // 全局mstp com 口 连接状态

bool custom_bacnet_register_listview = true;
bool initial_bip = false;
Str_modbus_reg bacnet_to_modbus_struct;  //用于bacnet 协议转换为modbus 协议的结构


panelname_map g_panelname_map;

CString HolLable[BAC_HOLIDAY_COUNT] =   //用于动态加载List中的下拉框
{
    _T("AR1"),
    _T("AR2"),
    _T("AR3"),
    _T("AR4")
};
