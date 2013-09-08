#pragma once

//#define SHOW_MESSAGEBOX
#define SHOW_ERROR_MESSAGE
const int TYPE_INPUT = 1;
const int TYPE_OUTPUT = 2;
const int TYPE_PROGRAM = 3;
const int TYPE_VARIABLE = 4;

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

const int BAC_READ_INPUT_GROUP_NUMBER = 4;

const int BAC_READ_ALL_LIST = 255;
const int BAC_READ_INPUT_LIST = 1;
const int BAC_READ_OUTPUT_LIST = 2;
const int BAC_READ_VARIABLE_LIST = 3;
const int BAC_READ_PROGRAM_LIST = 4;

const int BAC_SHOW_MISSION_RESULTS = 3;

struct _Refresh_Info 
{
	int Input_InvokeID[BAC_READ_INPUT_GROUP_NUMBER+1];
	int Output_InvokeID[BAC_READ_INPUT_GROUP_NUMBER+1];
	int Variable_InvokeID[BAC_READ_INPUT_GROUP_NUMBER+1];
	int Program_InvokeID[BAC_READ_INPUT_GROUP_NUMBER+1];
	int Input_result[BAC_READ_INPUT_GROUP_NUMBER+1];	//2 UNKONW  1 OK   0 FAIL
	int Output_result[BAC_READ_INPUT_GROUP_NUMBER+1];
	int Variable_result[BAC_READ_INPUT_GROUP_NUMBER+1];
	int Program_result[BAC_READ_INPUT_GROUP_NUMBER+1];
};

typedef struct _MessageWriteListInfo
{
	uint32_t deviceid;
	int8_t command;
	int8_t start_instance;
	int8_t end_instance;
	int8_t entitysize;
	HWND hWnd;
};



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

#define WM_DELETE_WAIT_DLG WM_USER + 2001