#include "global_function.h"

pidname_map product_map;

void Inial_Product_map()
{
	product_map.clear();
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5B, _T("TStat5B")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5A, _T("TStat5A")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5B2, _T("TStat5B2")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5C, _T("TStat5C")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT6, _T("TStat6")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT7, _T("TStat7")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5i, _T("TStat5i")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT8, _T("TStat8")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT10, _T("TStat10")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5D, _T("TStat5D")));
	product_map.insert(map<int, CString>::value_type(PM_AirQuality, _T("Air Quality")));
	product_map.insert(map<int, CString>::value_type(PM_HUMTEMPSENSOR, _T("HUM Sensor")));
	product_map.insert(map<int, CString>::value_type(PM_TSTATRUNAR, _T("TStatRunar")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5E, _T("TStat5E")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5F, _T("TStat5F")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5G, _T("TStat5G")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT5H, _T("TStat5H")));
	product_map.insert(map<int, CString>::value_type(PM_CO2_NET, _T("CO2")));
	product_map.insert(map<int, CString>::value_type(PM_CO2_RS485, _T("CO2")));
	product_map.insert(map<int, CString>::value_type(PM_CO2_NODE, _T("CO2 Node")));
	product_map.insert(map<int, CString>::value_type(PM_PM5E, _T("PM5E")));
	product_map.insert(map<int, CString>::value_type(PM_CM5, _T("CM5")));
	product_map.insert(map<int, CString>::value_type(PM_PM5E_ARM, _T("PM5E_ARM")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT9, _T("TStat9")));
	product_map.insert(map<int, CString>::value_type(STM32_HUM_NET, _T("HUM Sensor")));
	product_map.insert(map<int, CString>::value_type(STM32_HUM_RS485, _T("HUM Sensor")));
	product_map.insert(map<int, CString>::value_type(PM_AFS, _T("AirFlow")));



	product_map.insert(map<int, CString>::value_type(PM_NC, _T("NC")));


	product_map.insert(map<int, CString>::value_type(PM_LightingController, _T("LC")));


	product_map.insert(map<int, CString>::value_type(PM_PRESSURE_SENSOR, _T("Pressure")));

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
	product_map.insert(map<int, CString>::value_type(PM_MINIPANEL_ARM, _T("T3Controller")));
	product_map.insert(map<int, CString>::value_type(PM_ESP32_T3_SERIES, _T("T3Controller")));
	product_map.insert(map<int, CString>::value_type(PWM_TEMPERATURE_TRANSDUCER, _T("XDUCER")));

	product_map.insert(map<int, CString>::value_type(PM_PRESSURE, _T("Pressure Sensor")));
	product_map.insert(map<int, CString>::value_type(PM_HUM_R, _T("HUM-R")));
	product_map.insert(map<int, CString>::value_type(PM_T322AI, _T("T3-22I")));
	product_map.insert(map<int, CString>::value_type(PWM_TRANSDUCER, _T("PWM_Tranducer")));
	product_map.insert(map<int, CString>::value_type(PM_BTU_METER, _T("BTU_Meter")));
	product_map.insert(map<int, CString>::value_type(PM_T38AI8AO6DO, _T("T3-8AI8AO6DO")));
	product_map.insert(map<int, CString>::value_type(PM_T3PT12, _T("T3-PT12")));


	product_map.insert(map<int, CString>::value_type(PM_CS_SM_AC, _T("CS-SM-AC")));
	product_map.insert(map<int, CString>::value_type(PM_CS_SM_DC, _T("CS-SM-DC")));
	product_map.insert(map<int, CString>::value_type(PM_CS_RSM_AC, _T("CS-RSM-AC")));
	product_map.insert(map<int, CString>::value_type(PM_CS_RSM_DC, _T("CS-RSM-DC")));

	product_map.insert(map<int, CString>::value_type(PM_TSTAT_AQ, _T("Airlab")));
	product_map.insert(map<int, CString>::value_type(PM_FAN_MODULE, _T("Fan Moudle")));
	product_map.insert(map<int, CString>::value_type(PM_AIRLAB_ESP32, _T("Airlab-E32")));
	product_map.insert(map<int, CString>::value_type(PM_MULTI_SENSOR, _T("Multi Sensor")));
	product_map.insert(map<int, CString>::value_type(STM32_PM25, _T("PM2.5")));
	product_map.insert(map<int, CString>::value_type(PM_PWMETER, _T("Power_Meter")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT8_WIFI, _T("TStat8_Wifi")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT8_OCC, _T("TStat8_Occ")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT7_ARM, _T("TStat7_ARM")));
	product_map.insert(map<int, CString>::value_type(PM_TSTAT8_220V, _T("TStat8_220V")));
	product_map.insert(map<int, CString>::value_type(PM_T3_LC, _T("T3_LC")));
	product_map.insert(map<int, CString>::value_type(PM_T36CTA, _T("T3-6CTA")));
	product_map.insert(map<int, CString>::value_type(STM32_CO2_NET, _T("CO2")));
	product_map.insert(map<int, CString>::value_type(STM32_CO2_RS485, _T("CO2")));
	product_map.insert(map<int, CString>::value_type(STM32_HUM_NET, _T("Hum")));
	product_map.insert(map<int, CString>::value_type(STM32_HUM_RS485, _T("Hum")));
	product_map.insert(map<int, CString>::value_type(STM32_PRESSURE_NET, _T("Pressure")));
	product_map.insert(map<int, CString>::value_type(STM32_PRESSURE_RS3485, _T("Pressure")));
	product_map.insert(map<int, CString>::value_type(STM32_CO2_NODE, _T("CO2 Node")));

}

CString GetProductName(int ModelID)
{

	return product_map[ModelID];

}