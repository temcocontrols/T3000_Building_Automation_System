
#pragma once
#include "afxmt.h"

//#include "stdafx.h"
unsigned short multi_register_value[513]={-1};

unsigned short cm5_register_value[512]; //CM5
unsigned short m_buffer[512];//CM5

int nCom;
//CString program_path=_T("");
int g_tstat_id=255;
int g_serialNum=0;
BOOL g_tstat_id_changed=FALSE;
BOOL g_bPauseMultiRead=FALSE;


CString g_strImagePathName=_T("");
int now_tstat_id =0;//for batch load /flash.

CString g_strOrigDatabaseFilePath=_T("");
CString	g_strDatabasefilepath=_T("");
CString g_strExePth=_T("");
CString g_strImgeFolder=_T("");

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

int g_CommunicationType;
int m_nbaudrat=19200;
CString showing_text;
CCriticalSection critical_section;
CCriticalSection register_critical_section;
CEvent g_eventPauseMultiRead;

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

CString g_strInHumName;		// for tstat6 humidity input

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



BOOL g_bEnableRefreshTreeView = TRUE;
BOOL g_bScanStart = FALSE;
int g_llTxCount = 0;
int g_llRxCount = 0;
BOOL g_unint = TRUE;//TREE = °C;FALSE = F;

int parameterSet = 0;//用于初始化SLIDER。

//int g_nIpPort=6001;
//CString	g_strIpAdress;
int MDAY=0,MNIGHT=0;
int tstat6flex[12]={
	30,
	20,
	40,
	40,//20,
	20,//40,
	30,
	40,
	20,
	20,//40,
	40,//20,
	50,
	15
};





int m_slidertsta6[12]={
	30,
	20,
	40,
	40,//20,
	20,//40,
	30,
	40,
	20,
	20,//40,
	40,//20,
	50,
	15
};

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
unsigned short newtstat6[512];
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
{"INPUT","Decide which calibration table will be used. 0 = default table 1 = customer table"},

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
