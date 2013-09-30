#pragma once

//#define SHOW_MESSAGEBOX
#define WM_FRESH_CM_LIST WM_USER + 975
#define WM_DELETE_WAIT_DLG WM_USER + 2001
#define SHOW_ERROR_MESSAGE

#define WM_COMMAND_WHO_IS  1
#define MENU_CLICK			2



const int TYPE_ALL = 255;
const int TYPE_INPUT = 1;
const int TYPE_OUTPUT = 2;
const int TYPE_PROGRAM = 3;
const int TYPE_VARIABLE = 4;
const int TYPE_WEEKLY =5;
const int TYPE_ANNUAL = 6;
const int TYPE_CONTROLLER = 8;

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
const int BAC_VARIABLE_ITEM_COUNT = 64;
const int BAC_WEEKLY_ROUTINES_COUNT = 16;
const int BAC_ANNUAL_ROUTINES_COUNT = 8;
const int BAC_SCHEDULE_TIME_COUNT = 8;
const int BAC_TIME_COMMAND_COUNT = 1;
const int BAC_CONTROLLER_COUNT = 48;

const int BAC_INPUT_GROUP = BAC_INPUT_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_OUTPUT_GROUP = BAC_OUTPUT_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_PROGRAM_GROUP = BAC_PROGRAM_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_VARIABLE_GROUP = BAC_VARIABLE_ITEM_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_WEEKLY_GROUP = BAC_WEEKLY_ROUTINES_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_ANNUAL_GROUP = BAC_ANNUAL_ROUTINES_COUNT / BAC_READ_GROUP_NUMBER;
const int BAC_TIME_COMMAND_GROUP = 1;
const int BAC_CONTROLLER_GROUP = BAC_CONTROLLER_COUNT / BAC_READ_GROUP_NUMBER;


const int BAC_READ_ALL_LIST = 255;
const int BAC_READ_INPUT_LIST = 1;
const int BAC_READ_OUTPUT_LIST = 2;
const int BAC_READ_PROGRAM_LIST = 3;
const int BAC_READ_VARIABLE_LIST = 4;
const int BAC_READ_WEEKLY_LIST = 5;
const int BAC_READ_ANNUAL_LIST = 6;
const int BAC_READ_TIME_COMMAND = 7;
const int BAC_READ_CONTROLLER_LIST = 8;


const int BAC_SHOW_MISSION_RESULTS = 3;

const int BAC_LIST_REFRESH_TIME = 30000;


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

struct _Resend_Read_Info
{
	int device_id;
	int command;
	int start_instance;
	int end_instance;
	int resend_count;
	int task_result;
	int invoke_id;
	bool has_resend_yes_or_no;
};

struct _Refresh_Info 
{
	_Resend_Read_Info Read_Variable_Info[BAC_VARIABLE_GROUP];
	_Resend_Read_Info Read_Input_Info[BAC_INPUT_GROUP];
	_Resend_Read_Info Read_Output_Info[BAC_OUTPUT_GROUP];
	_Resend_Read_Info Read_Program_Info[BAC_PROGRAM_GROUP];
	_Resend_Read_Info Read_Weekly_Info[BAC_WEEKLY_GROUP];
	_Resend_Read_Info Read_Annual_Info[BAC_ANNUAL_GROUP];
	_Resend_Read_Info Read_Time_Command[BAC_TIME_COMMAND_GROUP];
	_Resend_Read_Info Read_Controller_Info[BAC_CONTROLLER_GROUP];
};




/*typedef*/ struct _MessageWriteListInfo
{
	uint32_t deviceid;
	int8_t command;
	int8_t start_instance;
	int8_t end_instance;
	unsigned short entitysize;
	CString Write_Info;
	HWND hWnd;
};

/*typedef*/ struct _MessageRefreshListInfo
{
	uint32_t deviceid;
	int8_t command;
	int8_t start_instance;
	int8_t end_instance;
	unsigned short entitysize;
	int8_t block_size;
	HWND hWnd;
};

const int UNITS_TYPE_ANALOG = 0;
const int UNITS_TYPE_DIGITAL = 1;
const int UNITS_TYPE_CUSTOM = 2;

const int INPUT_ANOLAG_UNITE_COUNT = 19;
const int VARIABLE_ANALOG_UNITE_COUNT = 34;

const CString Units_Type[]=
{
	_T("Analog"),
	_T("Digital"),
	_T("Custom dig")
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
	_T("Low/High")
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
	 _T("Y3K -40 to 150"),
	 _T("Y3K -40 to 300"),
	 _T("10K -40 to 120"),
	 _T("10K -40 to 250"),
	 _T("G3K -40 to 120"),
	 _T("G3K -40 to 250"),
	 _T("KM10K -40 to 120"),
	 _T("KM10K -40 to 250"),
	 _T("A10K -50 to 110"),
	 _T("A10K -60 to 200"),
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

