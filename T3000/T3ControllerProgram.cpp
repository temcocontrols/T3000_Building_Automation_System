#include "StdAfx.h"
#include "T3ControllerProgram.h"

#include"global_function.h"
CT3ControllerProgram::CT3ControllerProgram()
{
	m_strIpAddress = L"192.168.0.4";
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
int CT3ControllerProgram::SaveProgramData(uint32_t deviceid, uint8_t n_command, uint8_t start_instance, uint8_t end_instance, uint8_t npackage)
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

bool CT3ControllerProgram::Judge_Label_Exsit(LPCTSTR m_new_label)
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

bool CT3ControllerProgram::Judge_FullLabel_Exsit(LPCTSTR m_new_fulllabel)
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

void CT3ControllerProgram::SplitStringToArray(CStringArray &saArray, CString sSource, CString sToken)
{
	CString sTempSource, sTempSplitted;

	sTempSource = sSource;

	int nPos = sTempSource.Find(sToken);

	//--if there are no token in the string, then add itself and return.
	if (nPos == -1)
		saArray.Add(sTempSource);
	else
	{
		while (sTempSource.GetLength() > 0)
		{
			nPos = sTempSource.Find(sToken);
			if (nPos == -1)
			{
				saArray.Add(sTempSource.Trim());
				break;
			}
			else if (nPos == 0)
			{
				sTempSource = sTempSource.Mid(sToken.GetLength(), sTempSource.GetLength());
				continue;
			}
			else
			{
				sTempSplitted = sTempSource.Mid(0, nPos);
				saArray.Add(sTempSplitted.Trim());
				sTempSource = sTempSource.Mid(nPos + sToken.GetLength(), sTempSource.GetLength());
			}
		}
	}

}
void CT3ControllerProgram::Initial_Product()
{
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5A, _T("TStat5A")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5B, _T("TStat5B")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5B2, _T("TStat5B2")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5C, _T("TStat5C")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5D, _T("TStat5D")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5E, _T("TStat5E")));


	product_map.insert(map<int, CString>::value_type(PM_PM5E, _T("PM5E")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5F, _T("TStat5F")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5G, _T("TStat5G")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5H, _T("TStat5H")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT6, _T("TStat6")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5i, _T("TStat5i")));

	product_map.insert(map<int, CString>::value_type(PM_TSTAT8, _T("TStat8")));
	product_map.insert(map<int, CString>::value_type(PM_HUMTEMPSENSOR, _T("HUM Sensor")));
	product_map.insert(map<int, CString>::value_type(STM32_HUM_NET, _T("HUM Sensor")));
	product_map.insert(map<int, CString>::value_type(STM32_HUM_RS485, _T("HUM Sensor")));
	product_map.insert(map<int, CString>::value_type(PM_AirQuality, _T("Air Quality")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT7, _T("TStat7")));

	product_map.insert(map<int, CString>::value_type(PM_NC, _T("NC")));
	product_map.insert(map<int, CString>::value_type(PM_CM5, _T("CM5")));
	product_map.insert(map<int, CString>::value_type(PM_TSTATRUNAR, _T("TStatRunar")));
	product_map.insert(map<int, CString>::value_type(PM_LightingController, _T("LC")));
	product_map.insert(map<int, CString>::value_type(PM_CO2_NET, _T("CO2 Net")));
	product_map.insert(map<int, CString>::value_type(PM_CO2_RS485, _T("CO2")));

	product_map.insert(map<int, CString>::value_type(PM_PRESSURE_SENSOR, _T("Pressure")));
	product_map.insert(map<int, CString>::value_type(PM_CO2_NODE, _T("CO2 Node")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT6_HUM_Chamber, _T("HumChamber")));
	product_map.insert(map<int, CString>::value_type(PM_T3PT10, _T("T3-PT10")));
	product_map.insert(map<int, CString>::value_type(PM_T3IOA, _T("T3-8O")));
	product_map.insert(map<int, CString>::value_type(PM_T332AI, _T("T3-32AI")));

	product_map.insert(map<int, CString>::value_type(PM_T38AI16O, _T("T3-8AI160")));
	product_map.insert(map<int, CString>::value_type(PM_T38I13O, _T("T3-8I13O")));
	product_map.insert(map<int, CString>::value_type(PM_T3PERFORMANCE, _T("T3-Performance")));
	product_map.insert(map<int, CString>::value_type(PM_T34AO, _T("T3-4AO")));
	product_map.insert(map<int, CString>::value_type(PM_T36CT, _T("T3-6CT")));
	product_map.insert(map<int, CString>::value_type(PM_MINIPANEL, _T("T3Controller")));
	product_map.insert(map<int, CString>::value_type(PM_MINIPANEL_ARM, _T("T3Controller(ARM)")));
	product_map.insert(map<int, CString>::value_type(PM_PRESSURE, _T("Pressure Sensor")));
	product_map.insert(map<int, CString>::value_type(PM_HUM_R, _T("HUM-R")));
	product_map.insert(map<int, CString>::value_type(PM_T322AI, _T("T3-22I")));
	product_map.insert(map<int, CString>::value_type(PWM_TRANSDUCER, _T("PWM_Tranducer")));
	product_map.insert(map<int, CString>::value_type(PM_BTU_METER, _T("BTU_Meter")));
	product_map.insert(map<int, CString>::value_type(PM_T3PT12, _T("T3-PT12")));
	product_map.insert(map<int, CString>::value_type(PM_T36CTA, _T("T3-6CTA")));
	product_map.insert(map<int, CString>::value_type(PM_T38AI8AO6DO, _T("T3-8AI8AO6DO")));
	product_map.insert(map<int, CString>::value_type(PM_CS_SM_AC, _T("CS-SM-AC")));
	product_map.insert(map<int, CString>::value_type(PM_CS_SM_DC, _T("CS-SM-DC")));
	product_map.insert(map<int, CString>::value_type(PM_CS_RSM_AC, _T("CS-RSM-AC")));
	product_map.insert(map<int, CString>::value_type(PM_CS_RSM_DC, _T("CS-RSM-DC")));


	product_map.insert(map<int, CString>::value_type(PM_TSTAT8_WIFI, _T("TStat8_Wifi")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT8_OCC, _T("TStat8_Occ")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT7_ARM, _T("TStat7_ARM")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT8_220V, _T("TStat8_220V")));
	product_map.insert(map<int, CString>::value_type(PM_T3_LC, _T("T3_LC")));





}

CString CT3ControllerProgram::ShowT3DefaultName()
{
	return _T("T3Controller:") + m_strIpAddress;
}