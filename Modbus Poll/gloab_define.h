#pragma once
#define  MODBUS_POLL 1
//#define SHOW_MESSAGEBOX
#define	 MY_FRESH_MBPOLLVIEW		WM_USER + 1231
#define  MY_FRESH_DRAW_GRAPHIC      WM_USER + 1230
#define  MY_READ_ONCE               WM_USER + 1240

#define WM_FRESH_CM_LIST WM_USER + 975
#define WM_DELETE_NEW_MESSAGE_DLG WM_USER + 2001
#define MY_BAC_CONFIG_READ_RESULTS  WM_USER + 2002
#define SHOW_ERROR_MESSAGE

#define  WM_LIST_MONITOR_CHANGED WM_USER+ 976
#define  WM_LIST_MONITOR_INPUT_CHANGED WM_USER+ 977
#define WM_SCREENEDIT_CLOSE WM_USER + 1232

const int BACNETIP_PORT =  47808;
const int UDP_BROADCAST_PORT =1234;
const int RECV_RESPONSE_PORT = 4321;
#define UPD_BROADCAST_QRY_MSG 100
#define RESPONSE_MSG          UPD_BROADCAST_QRY_MSG+1

const int MODBUS_RS485 = 0;
const int MODBUS_TCPIP = 1;
const int MODBUS_BACNET_MSTP = 2;
const int PROTOCOL_BACNET_IP   = 3;
const int PROTOCOL_UNKNOW = 255;

#define WM_COMMAND_WHO_IS  1
#define MENU_CLICK			2

const COLORREF LIST_ITEM_CHANGED_BKCOLOR = RGB(255,0,0);
const COLORREF LIST_ITEM_DEFAULT_BKCOLOR = GetSysColor(COLOR_WINDOW);
const COLORREF LIST_ITEM_DEFAULT_BKCOLOR_GRAY = RGB(235,235,235);
const COLORREF LIST_ITEM_SELECTED = RGB(150,150,200);

const bool REFRESH_ON_ITEM = TRUE;

const int SEND_COMMAND_DELAY_TIME = 100;
const int RESEND_COUNT = 100;
const int FAIL_RESEND_COUNT = 4;

const int TYPE_SVAE_CONFIG = 254;
const int TYPE_ALL = 255;
const int TYPE_CONNECT_WITH_DEVICE = 253;
const int TYPE_INPUT = 1;
const int TYPE_OUTPUT = 2;
const int TYPE_PROGRAM = 3;
const int TYPE_VARIABLE = 4;
const int TYPE_WEEKLY =5;
const int TYPE_ANNUAL = 6;
const int TYPE_CONTROLLER = 8;
const int TYPE_SCREENS = 9;
const int TYPE_MONITOR = 10;
const int TYPE_PROGRAMCODE = 11;
const int TYPE_WEEKLYCODE = 12;
const int TYPE_ANNUALCODE = 13;
const int TYPE_ALARMLOG = 14;
const int TYPE_TSTAT = 15;

const int DELETE_WINDOW_MSG = 200;


const int BAC_READ_ALL_LIST = 255;
const int BAC_READ_SVAE_CONFIG = 254;
const int BAC_READ_CONNECT_WITH_DEVICE_LIST = 253;
const int BAC_READ_INPUT_LIST = 1;
const int BAC_READ_OUTPUT_LIST = 2;
const int BAC_READ_PROGRAM_LIST = 3;
const int BAC_READ_VARIABLE_LIST = 4;
const int BAC_READ_WEEKLY_LIST = 5;
const int BAC_READ_ANNUAL_LIST = 6;
const int BAC_READ_TIME_COMMAND = 7;
const int BAC_READ_CONTROLLER_LIST = 8;
const int BAC_READ_SCREEN_LIST = 9;
const int BAC_READ_MONITOR_LIST = 10;
const int BAC_READ_PROGRAMCODE_LIST = 11;
const int BAC_READ_WEEKLTCODE_LIST = 12;
const int BAC_READ_ANNUALCODE_LIST = 13;
const int BAC_READ_ALARMLOG_LIST = 14;
const int BAC_READ_TSTAT_LIST = 15;
const int BAC_READ_BASIC_SETTING_COMMAND = 16;




const int VARIABLE_RANGE_ANALOG_TYPE = 1;
const int VARIABLE_RANGE_DIGITAL_TYPE = 2;
const int VARIABLE_RANGE_CUSTOM_DIG_TYPE = 3;

const int INPUT_RANGE_ANALOG_TYPE = 4;
const int INPUT_RANGE_DIGITAL_TYPE = 5;
const int INPUT_RANGE_CUSTOM_DIG_TYPE = 6;

const int OUTPUT_RANGE_ANALOG_TYPE = 7;
const int OUTPUT_RANGE_DIGITAL_TYPE = 8;
const int OUTPUT_RANGE_CUSTOM_DIG_TYPE = 9;

const int BAC_RESULTS_UNKONW = 2;
const int BAC_RESULTS_OK = 1;
const int BAC_RESULTS_FAIL = 0;

const int BAC_UNITS_DIGITAL = 0;
const int BAC_UNITS_ANALOG  = 1;

const int BAC_AUTO = 0;
const int BAC_MANUAL = 1;

const int BAC_DECOM_YES = 0;
const int BAC_DECOM_NO  = 1;

const int BAC_READ_GROUP_NUMBER = 4;

const int BAC_INPUT_ITEM_COUNT = 20;
const int BAC_OUTPUT_ITEM_COUNT = 20;
const int BAC_PROGRAM_ITEM_COUNT = 16;
const int BAC_PROGRAMCODE_ITEM_COUNT = 16;
const int BAC_VARIABLE_ITEM_COUNT = 64;
const int BAC_WEEKLY_ROUTINES_COUNT = 16;
const int BAC_WEEKLYCODE_ROUTINES_COUNT = 16;
const int BAC_ANNUAL_ROUTINES_COUNT = 8;
const int BAC_SCHEDULE_TIME_COUNT = 8;
const int BAC_TIME_COMMAND_COUNT = 1;
const int BAC_BASIC_SETTING_COUNT = 1;
const int BAC_CONTROLLER_COUNT = 48;
const int BAC_SCREEN_COUNT = 32;
const int BAC_MONITOR_COUNT = 12;
const int BAC_ANNUAL_CODE_COUNT = 8;
const int BAC_CONNECT_WITH_DEVICE_COUNT = 1;
const int BAC_ALARMLOG_COUNT = 16;
const int BAC_TSTAT_COUNT = 8;



const int BAC_INPUT_GROUP = BAC_INPUT_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_OUTPUT_GROUP = BAC_OUTPUT_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_PROGRAM_GROUP = BAC_PROGRAM_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_PROGRAMCODE_GROUP = BAC_PROGRAMCODE_ITEM_COUNT;
const int BAC_VARIABLE_GROUP = BAC_VARIABLE_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_WEEKLY_GROUP = BAC_WEEKLY_ROUTINES_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_WEEKLYCODE_GOUP = BAC_WEEKLYCODE_ROUTINES_COUNT;
const int BAC_ANNUAL_GROUP = BAC_ANNUAL_ROUTINES_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_TIME_COMMAND_GROUP = 1;
const int BAC_BASIC_SETTING_GROUP = 1;
const int BAC_ANNUALCODE_GROUP = BAC_ANNUAL_CODE_COUNT;
const int BAC_CONTROLLER_GROUP = BAC_CONTROLLER_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_SCREEN_GROUP = BAC_SCREEN_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_MONITOR_GROUP = BAC_MONITOR_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_CONNECT_WITH_DEVICE_GROUP = 1;
const int BAC_ALARMLOG_GROUP = BAC_ALARMLOG_COUNT;
const int BAC_TSTAT_GROUP = BAC_TSTAT_COUNT / BAC_READ_GROUP_NUMBER;




const int BAC_SHOW_CONNECT_RESULTS = 1;
const int BAC_SHOW_MISSION_RESULTS = 3;

const int BAC_LIST_REFRESH_TIME = 10000;//ms


const int SCHEDULE_TIME_NUM = 0;
const int SCHEDULE_TIME_MONDAY = 1;
const int SCHEDULE_TIME_TUESDAY = 2;
const int SCHEDULE_TIME_WEDNESDAY = 3;
const int SCHEDULE_TIME_THURSDAY = 4;
const int SCHEDULE_TIME_FRIDAY = 5;
const int SCHEDULE_TIME_SATURDAY = 6;
const int SCHEDULE_TIME_SUNDAY = 7;
const int SCHEDULE_TIME_HOLIDAY1 = 8;
const int SCHEDULE_TIME_HOLIDAY2 = 9;


const int WEEKLY_SCHEDULE_SIZE = 144;
const int ANNUAL_CODE_SIZE = 48;
struct _Bac_Scan_Com_Info
{
	int device_id;
	int macaddress;
};
struct _Bac_Scan_results_Info
{
	int device_id;
	char ipaddress[6];
	unsigned char modbus_addr;
	unsigned char product_type;
	unsigned int serialnumber;
	unsigned char panel_number;
};


struct _Resend_Read_Info
{
	int device_id;
	int command;
	int start_instance;
	int end_instance;
	int resend_count;
	int task_result;
	int invoke_id;
	int has_resend_yes_or_no;
	int timeout_count;//多少次还没收到回复，就算 失败;
};

struct _Com_Scan_Read_Info
{
	int device_id;
	int command;
	int start_instance;
	int end_instance;
	int resend_count;
	int invoke_id;
	int retry_time;
};

struct refresh_net_device
{
	DWORD nSerial;
	int modbusID;
};

struct _Refresh_Info 
{
	_Resend_Read_Info Read_Variable_Info[BAC_VARIABLE_GROUP];
	_Resend_Read_Info Read_Input_Info[BAC_INPUT_GROUP];
	_Resend_Read_Info Read_Output_Info[BAC_OUTPUT_GROUP];
	_Resend_Read_Info Read_Program_Info[BAC_PROGRAM_GROUP];
	_Resend_Read_Info Read_Programcode_Info[BAC_PROGRAMCODE_GROUP];
	_Resend_Read_Info Read_Weekly_Info[BAC_WEEKLY_GROUP];
	_Resend_Read_Info Read_Weeklycode_Info[BAC_WEEKLYCODE_GOUP];
	_Resend_Read_Info Read_Annual_Info[BAC_ANNUAL_GROUP];
	_Resend_Read_Info Read_Annualcode_Info[BAC_ANNUALCODE_GROUP];
	_Resend_Read_Info Read_Time_Command[BAC_TIME_COMMAND_GROUP];
	_Resend_Read_Info Read_Controller_Info[BAC_CONTROLLER_GROUP];
	_Resend_Read_Info Read_Screen_Info[BAC_SCREEN_GROUP];
	_Resend_Read_Info Read_Monitor_Info[BAC_MONITOR_GROUP];
	_Resend_Read_Info Read_Connect_With_Device[BAC_CONNECT_WITH_DEVICE_GROUP];
	_Resend_Read_Info Read_AlarmLog_Info[BAC_ALARMLOG_GROUP];
	_Resend_Read_Info Read_Tstat_Info[BAC_TSTAT_GROUP];
	_Resend_Read_Info Read_BasicSetting_Info[BAC_BASIC_SETTING_GROUP];
};

struct _Refresh_Write_Info 
{
	_Resend_Read_Info Write_Variable_Info[BAC_VARIABLE_GROUP];
	_Resend_Read_Info Write_Input_Info[BAC_INPUT_GROUP];
	_Resend_Read_Info Write_Output_Info[BAC_OUTPUT_GROUP];
	_Resend_Read_Info Write_Program_Info[BAC_PROGRAM_GROUP];
	_Resend_Read_Info Write_Programcode_Info[BAC_PROGRAMCODE_GROUP];
	_Resend_Read_Info Write_Weekly_Info[BAC_WEEKLY_GROUP];
	_Resend_Read_Info Write_Weeklycode_Info[BAC_WEEKLYCODE_GOUP];
	_Resend_Read_Info Write_Annual_Info[BAC_ANNUAL_GROUP];
	_Resend_Read_Info Write_Annualcode_Info[BAC_ANNUALCODE_GROUP];
	_Resend_Read_Info Write_Time_Command[BAC_TIME_COMMAND_GROUP];
	_Resend_Read_Info Write_Controller_Info[BAC_CONTROLLER_GROUP];
	_Resend_Read_Info Write_Screen_Info[BAC_SCREEN_GROUP];
	_Resend_Read_Info Write_Monitor_Info[BAC_MONITOR_GROUP];
};


struct Monitor_Input_Info
{
	int Max_Value;
	int Min_Value;
	bool be_record;//是否记录了这条;
	bool use_own_scale;//是否单独用自己的刻度;
	bool show_graphic; //是否显示图像;
};

struct Changed_Item_Info
{
	int nRow;
	int nCol;
};





const int UNITS_TYPE_ANALOG = 0;
const int UNITS_TYPE_DIGITAL = 1;
const int UNITS_TYPE_CUSTOM = 2;

const int INPUT_ANOLAG_UNITE_COUNT = 19;
const int VARIABLE_ANALOG_UNITE_COUNT = 34;

const CString Units_Type[]=
{
	_T("Analog"),
	_T("Digital")
	//_T("Custom dig")
};

const CString Yes_No[] = 
{
	_T("NO"),
	_T("YES")
};

const CString Digital_Units_Array[] = 
{
	_T("Unused"),
	_T("Off/On"),
	_T("Close/Open"),
	_T("Stop/Start"),
	_T("Disable/Enable"),
	_T("Normal/Alarm"),
	_T("Normal/High"),
	_T("Normal/Low"),
	_T("No/Yes"),
	_T("Cool/Heat"),
	_T("Unoccupy/Occupy"),
	_T("Low/High"),
	_T("On/Off"),
	_T("Open/Close"),
	_T("Start/Stop"),
	_T("Enable/Disable"),
	_T("Alarm/Normal"),
	_T("High/Normal"),
	_T("Low/Normal"),
	_T("Yes/No"),
	_T("Heat/Cool"),
	_T("Occupy/Unoccupy"),
	_T("High/Low")
};

//const CString Input_Analog_Units_Show[] =
//{
//	_T("")
//};

const CString Output_Analog_Units_Show[] = 
{
	_T("Unused"),
	_T("Volts"),
	_T("%Open"),
	_T("psi"),
	_T("%"),
	_T("%Cls"),
	_T("ma"),	
};

const CString OutPut_List_Analog_Range[] =
{
	_T("Unused"),
	_T("0.0 -> 10"),
	_T("0.0 -> 100"),
	_T("0.0 -> 20"),
	_T("0.0 -> 100"),
	_T("0.0 -> 100"),
	_T("0.0 -> 20")
};

const CString OutPut_List_Analog_Units[] =
{
	_T("Unused"),
	_T("Volts"),
	_T("%Open"),
	_T("psi"),
	_T("%"),
	_T("%Cls"),
	_T("ma")
};

const CString Input_List_Analog_Units[] =
{
	_T("Unused"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Volts"),
	 _T("Amps"),
	 _T("ma"),
	 _T("psi"),
	 _T("counts"),
	 _T("FPM"),
	 _T("%(0-5V)"),
	 _T("%(4-20ma)"),
};

const CString Input_Analog_Units_Array[] =
{
	_T("Unused"),
    _T("PT100 -40 to 1000"),
    _T("PT100 -40 to 1800"),
	 _T("10K -40 to 120"),
	 _T("10K -40 to 250"),
     _T("PT1000 -40 to 450"),
     _T("PT1000 -40 to 800"),
	 _T("KM10K -40 to 120"),
	 _T("KM10K -40 to 250"),
     _T("PT 1K -200 to 300"),
     _T("PT 1K -200 to 570"),
	 _T("0.0 to 5.0"),
	 _T("0.0 to 100"),
	 _T("0.0 to 20"),
	 _T("0.0 to 20"),
	 _T("0.0 to 2^22"),
	 _T("0.0 to 3000"),
	 _T("0 to 100"),
	 _T("0 to 100"),
};

const CString Output_Analog_Units_Array[] = 
{
	_T("Unused"),
	_T("0.0 -> 10   Volts"),
	_T("0.0 -> 100  %Open"),
	_T("0.0 -> 20   psi"),
	_T("0.0 -> 100  %"),
	_T("0.0 -> 100  %Cls"),
	_T("0.0 -> 20   ma")
};

const CString Variable_Analog_Units_Array[] = 
{
	_T("Unused"),
	_T("deg.Celsius"),
	_T("deg.Fahrenheit"),
	_T("Feet per Min"),
	_T("Pascals"),
	_T("KPascals"),
	_T("lbs/sqr.inch"),
	_T("inches of WC"),
	_T("Watts"),
	_T("KWatts"),
	_T("KWH"),
	_T("Volts"),
	_T("KV"),
	_T("Amps"),//13
	_T("ma"),
	_T("CFM"),
	_T("Seconds"),
	_T("Minutes"),
	_T("Hours"),
	_T("Days"),
	_T("Time"),
	_T("Ohms"),
	_T("%"),
	_T("%RH"),
	_T("p/min"),
	_T("Counts"),
	_T("%Open"),
	_T("CFH"),
	_T("GPM"),
	_T("GPH"),
	_T("GAL"),
	_T("CF"),
	_T("BTU"),
	_T("CMH"),
};






const int WINDOW_TAB_COUNT = 11; //多少个Window 嵌入在TAB里面;
const int WINDOW_INPUT = 0;
const int WINDOW_OUTPUT = 1;
const int WINDOW_VARIABLE = 2;
const int WINDOW_PROGRAM = 3;
const int WINDOW_CONTROLLER = 4;
const int WINDOW_SCREEN = 5;
const int WINDOW_WEEKLY = 6;
const int WINDOW_ANNUAL = 7;
const int WINDOW_MONITOR = 8;
const int WINDOW_ALARMLOG = 9;
const int WINDOW_TSTAT	= 10;



const int KEY_INSERT = 1020;