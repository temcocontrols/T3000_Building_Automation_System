#pragma once
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

struct _Refresh_Info 
{
	int Input_InvokeID;
	int Output_InvokeID;
	int Variable_InvokeID;
	int Program_InvokeID;
	int Input_result;	//2 UNKONW  1 OK   0 FAIL
	int Output_result;
	int Variable_result;
	int Program_result;
};

#define WM_DELETE_WAIT_DLG WM_USER + 2001