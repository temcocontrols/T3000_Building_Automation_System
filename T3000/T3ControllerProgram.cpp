#include "StdAfx.h"
#include "T3ControllerProgram.h"

#include"global_function.h"
CT3ControllerProgram::CT3ControllerProgram()
{
}


CT3ControllerProgram::~CT3ControllerProgram()
{
}


int CT3ControllerProgram::GetProgramData(uint32_t deviceid, uint8_t start_instance, uint8_t end_instance, uint8_t npackgae)
{


	return GetProgramData(deviceid, start_instance, end_instance, npackgae);
}

int CT3ControllerProgram::GetProgramData_Blocking(uint32_t deviceid, uint8_t start_instance, uint8_t end_instance, uint8_t npackgae)
{
	return GetProgramData_Blocking(deviceid, start_instance, end_instance, npackgae);
}
int CT3ControllerProgram::WriteProgramData(uint32_t deviceid, uint8_t n_command, uint8_t start_instance, uint8_t end_instance, uint8_t npackage)
{
	unsigned char command = (unsigned char)n_command;

	unsigned short entitysize = 0;
	uint8_t apdu[480] = { 0 };
	uint8_t test_value[480] = { 0 };
	int private_data_len = 0;
	BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
	BACNET_APPLICATION_DATA_VALUE test_data_value = { 0 };
	BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };
	BACNET_PRIVATE_TRANSFER_DATA test_data = { 0 };
	bool status = false;

	private_data.vendorID = BACNET_VENDOR_ID;
	private_data.serviceNumber = 1;

	unsigned max_apdu = 0;
	entitysize = 400;
	entitysize = entitysize | (npackage << 9);	//将entitysize 的 高7位用来给program code ，用来记录是第几包;

	char SendBuffer[1000];
	memset(SendBuffer, 0, 1000);
	char * temp_buffer = SendBuffer;

	Str_user_data_header private_data_chunk;
	//Str_sub_user_data_header private_sub_data_chunk;
	int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
	unsigned char * n_temp_point = program_code[start_instance];


	HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
	private_data_chunk.total_length = PRIVATE_HEAD_LENGTH + ((unsigned char)end_instance - (unsigned char)start_instance + 1) * 400;
	private_data_chunk.command = command;
	private_data_chunk.point_start_instance = start_instance;
	private_data_chunk.point_end_instance = end_instance;
	private_data_chunk.entitysize = entitysize;
	Set_transfer_length(private_data_chunk.total_length);
	memcpy_s(SendBuffer, PRIVATE_HEAD_LENGTH, &private_data_chunk, PRIVATE_HEAD_LENGTH);
	n_temp_point = n_temp_point + npackage * 400;
	memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH, 400, n_temp_point, 400);
	Set_transfer_length(private_data_chunk.total_length);


	if (debug_item_show == DEBUG_SHOW_PROGRAM_DATA_ONLY)
	{
		CString temp_char;
		CString n_temp_print;
		char * temp_point;
		temp_point = SendBuffer;
		n_temp_print.Format(_T("prg_%d  pack_%d  write:"), start_instance, npackage);
		for (int i = 0; i < private_data_chunk.total_length; i++)
		{
			temp_char.Format(_T("%02x"), (unsigned char)*temp_point);
			temp_char.MakeUpper();
			temp_point++;
			n_temp_print = n_temp_print + temp_char + _T(" ");
		}
		DFTrace(n_temp_print);
	}


	status = bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING, (char *)&SendBuffer, &data_value);
	//ct_test(pTest, status == true);
	private_data_len = bacapp_encode_application_data(&test_value[0], &data_value);
	private_data.serviceParameters = &test_value[0];
	private_data.serviceParametersLen = private_data_len;

	BACNET_ADDRESS dest = { 0 };

	if (offline_mode)
	{
		SaveBacnetBinaryFile(offline_prg_path);
		return 1;
	}

	status = address_get_by_device(deviceid, &max_apdu, &dest);
	if (status)
	{
		return Send_ConfirmedPrivateTransfer(&dest, &private_data);
	}
	return -2;
}

bool CT3ControllerProgram::Check_Label_Exsit(LPCTSTR m_new_label)
{
	CString new_string;
	new_string = m_new_label;
	if (new_string.IsEmpty())
		return false;
	char cTemp1[255];
	memset(cTemp1, 0, 255);
	WideCharToMultiByte(CP_ACP, 0, new_string.GetBuffer(), -1, cTemp1, 255, NULL, NULL);

	for (int i = 0; i < BAC_INPUT_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Input_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_OUTPUT_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Output_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_VARIABLE_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Variable_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Program_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_SCREEN_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_screen_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_SCHEDULE_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Weekly_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_HOLIDAY_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Annual_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_MONITOR_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_monitor_data.at(i).label) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}
	return false;
}

bool CT3ControllerProgram::Check_FullLabel_Exsit(LPCTSTR m_new_fulllabel)
{
	CString new_string;
	new_string = m_new_fulllabel;
	if (new_string.IsEmpty())
		return false;
	char cTemp1[255];
	memset(cTemp1, 0, 255);
	WideCharToMultiByte(CP_ACP, 0, new_string.GetBuffer(), -1, cTemp1, 255, NULL, NULL);

	for (int i = 0; i < BAC_INPUT_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Input_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_OUTPUT_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Output_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_VARIABLE_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Variable_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Program_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_SCREEN_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_screen_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_SCHEDULE_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Weekly_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	for (int i = 0; i < BAC_HOLIDAY_COUNT; i++)
	{
		if (strcmp(cTemp1, (char *)m_Annual_data.at(i).description) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, new_string + _T(" already exist !"));
			return true;
		}
	}

	return false;
}