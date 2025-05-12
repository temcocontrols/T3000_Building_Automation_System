#include "StdAfx.h"
#include "T3000RegAddress.h"

#include "T3000.h"
#include "MainFrm.h"
#include "afxwin.h"


wchar_t m_ini_path_t3000[255];
wchar_t m_mdb_path_t3000[255];
int Mdb_Adress_Map=0;
int temp_Mdb_Adress_Map=0;



//目前数据库支持这3张表的查询，在切换器件时，一次性载入新的数据库，并不影响效率。;



T3000RegAddress::T3000RegAddress(void)
{
	memset(m_ini_path_t3000,0,sizeof(m_ini_path_t3000));

}


T3000RegAddress::~T3000RegAddress(void)
{
}


//Code by Fance
//The search the character from array which read from t3000.mdb
//Default search from TSTAT5 ABCD register list .
//在运行程序之前加载T3000.MDB 的数据至 数组，在动态运行中，判断 数组的值 后，在取值;
//这样 如果 Frimware 的寄存器列表更改，则只需更改 T3000.mdb的 寄存器表即可;
//但是由于需要修改的 地方实在太多，并且 太多特例情况，目前只能将 看的懂的部分 修改。旧版本的特例情况不做修改。;
int _P(char *str,int mMdb_Adress_Map)
{
	if(product_type==T3000_5ABCDFG_LED_ADDRESS)
	{
		for (int i=0;i<sizeof(TSTAT_5ABCDFG_LED_ADDRESS)/sizeof(TSTAT_5ABCDFG_LED_ADDRESS[0]);i++)
		{
			if(strcmp(TSTAT_5ABCDFG_LED_ADDRESS[i].AddressName,str)==0)
			{
				//#ifdef _DEBUG
				//TRACE("%s = %d\n",str,TSTAT_5ABCDFG_LED_ADDRESS[i].AddressValue);
				//#endif
				return TSTAT_5ABCDFG_LED_ADDRESS[i].AddressValue;
			}
		}
	}
	else if(product_type==T3000_5EH_LCD_ADDRESS)
	{
		for (int i=0;i<sizeof(TSTAT_5EH_LCD_ADDRESS)/sizeof(TSTAT_5EH_LCD_ADDRESS[0]);i++)
		{
			if(strcmp(TSTAT_5EH_LCD_ADDRESS[i].AddressName,str)==0)
			{
				//#ifdef _DEBUG
				//TRACE("%s = %d\n",str,TSTAT_5EH_LCD_ADDRESS[i].AddressValue);
				//#endif
				return TSTAT_5EH_LCD_ADDRESS[i].AddressValue;
			}
		}
	}
	else if(product_type==T3000_6_ADDRESS)
	{
		for (int i=0;i<sizeof(TSTAT_6_ADDRESS)/sizeof(TSTAT_6_ADDRESS[0]);i++)
		{
			if(strcmp(TSTAT_6_ADDRESS[i].AddressName,str)==0)
			{
				//#ifdef _DEBUG
				//TRACE("%s = %d\n",str,TSTAT_6_ADDRESS[i].AddressValue);
				//#endif
				return TSTAT_6_ADDRESS[i].AddressValue;
			}
		}
	}

	return -1;
}



bool T3000RegAddress::MatchMoudleAddress(void)
{

	 
	int temp_variant_item;
	CString temp_variant;
	 
	CString strSql;
	m_SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	strSql.Format(_T("select * from T3000_Register_Address_By_ID order by Register_Address"));
	m_q = m_SqliteDBT3000.execQuery((UTF8MBSTR)strSql); 
	while(!m_q.eof())
	{
		char cTemp[256];
		memset(cTemp,0,256);
		temp_variant_item = m_q.getIntField("Register_Address");

		temp_variant  =  m_q.getValuebyName(_T("TSTAT5_LED_AddressName"));
		CString cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();

		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(TSTAT_5ABCDFG_LED_ADDRESS[temp_variant_item].AddressName,MAX_PATH,cTemp);
		TSTAT_5ABCDFG_LED_ADDRESS[temp_variant_item].AddressValue = temp_variant_item;

		temp_variant=m_q.getValuebyName(_T("TSTAT5_LCD_AddressName"));
		cs_temp.Empty();
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();

		char cTemp_5e[256];
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp_5e, 256, NULL, NULL );
		strcpy_s(TSTAT_5EH_LCD_ADDRESS[temp_variant_item].AddressName,MAX_PATH,cTemp_5e);
		TSTAT_5EH_LCD_ADDRESS[temp_variant_item].AddressValue = temp_variant_item;

		temp_variant=m_q.getValuebyName(_T("TSTAT6_AddressName"));
		cs_temp.Empty();
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();

		char cTemp_6_7[256];
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp_6_7, 256, NULL, NULL );
		strcpy_s(TSTAT_6_ADDRESS[temp_variant_item].AddressName,MAX_PATH,cTemp_6_7);
		TSTAT_6_ADDRESS[temp_variant_item].AddressValue = temp_variant_item;

		m_q.nextRow();
	}
	 
#if 1
	strSql.Format(_T("select * from T3_RegisterList order by RegID"));
	m_q = m_SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	while(!m_q.eof())
	{
		unsigned char nlength;
		char cTemp[256];
		memset(cTemp,0,256);
		CString cs_temp;
		temp_variant_item = m_q.getIntField("RegID");
		T3_8AI8AO[temp_variant_item].AddressValue = temp_variant_item;
		T3_8AI16O[temp_variant_item].AddressValue = temp_variant_item;
		T3_32AI[temp_variant_item].AddressValue = temp_variant_item;
		T3_Performance[temp_variant_item].AddressValue = temp_variant_item;
		T3_4AO[temp_variant_item].AddressValue = temp_variant_item;
		T3_6CT[temp_variant_item].AddressValue = temp_variant_item;
		T3_28IN[temp_variant_item].AddressValue = temp_variant_item;
		T3_RTD[temp_variant_item].AddressValue = temp_variant_item;

		temp_variant=m_q.getValuebyName(_T("T3_8AI8AO"));
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_8AI8AO[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_8AI16O"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_8AI16O[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_32AI"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_32AI[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_Performance"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_Performance[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_4AO"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_4AO[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_6CT"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_6CT[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_28IN"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_28IN[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_RTD"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_RTD[temp_variant_item].AddressName,nlength,cTemp);

		temp_variant=m_q.getValuebyName(_T("T3_8I13O"));
		memset(cTemp,0,256);
		cs_temp=temp_variant;
		cs_temp = cs_temp.Trim();
		nlength = cs_temp.GetLength() + 1;
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 256, NULL, NULL );
		strcpy_s(T3_8I13O[temp_variant_item].AddressName,nlength,cTemp);

		

		m_q.nextRow();
	}
#endif

	m_SqliteDBT3000.closedb();

	return true;
}




 bool T3000RegAddress::Change_Register_Table(void)
 {
    MODBUS_SERIALNUMBER_LOWORD                            =  _P("MODBUS_SERIALNUMBER_LOWORD");              
                                                                                                     
    MODBUS_SERIALNUMBER_HIWORD                            =  _P("MODBUS_SERIALNUMBER_HIWORD");              
                                                                                                         
    MODBUS_VERSION_NUMBER_LO                              =  _P("MODBUS_VERSION_NUMBER_LO");                
    MODBUS_VERSION_NUMBER_HI                              =  _P("MODBUS_VERSION_NUMBER_HI");                
    MODBUS_ADDRESS                                        =  _P("MODBUS_ADDRESS");                          
    MODBUS_PRODUCT_MODEL                                  =  _P("MODBUS_PRODUCT_MODEL");                    
    MODBUS_HARDWARE_REV                                   =  _P("MODBUS_HARDWARE_REV");                     
    MODBUS_PIC_VERSION                                    =  _P("MODBUS_PIC_VERSION");                      
    MODBUS_HUM_PIC_VERSION                                =  _P("MODBUS_HUM_PIC_VERSION");                  
    MODBUS_ADDRESS_PLUG_N_PLAY                            =  _P("MODBUS_ADDRESS_PLUG_N_PLAY");              
                                                                                                          
                                                                                                          
    MODBUS_CUSTOM_ADDRESS                                 =  _P("MODBUS_CUSTOM_ADDRESS");                   
    MODBUS_BASE_ADDRESS                                   =  _P("MODBUS_BASE_ADDRESS");                     
    MODBUS_UPDATE_STATUS                                  =  _P("MODBUS_UPDATE_STATUS");                    
    MODBUS_UPDATE_PTR_HI                                  =  _P("MODBUS_UPDATE_PTR_HI");                    
    MODBUS_UPDATE_PTR_LO                                  =  _P("MODBUS_UPDATE_PTR_LO");                    
    MODBUS_SCRATCHPAD_ADDRESS                             =  _P("MODBUS_SCRATCHPAD_ADDRESS");               
    MODBUS_HARDWARE_INFORMATION                           =  _P("MODBUS_HARDWARE_INFORMATION");             
                                                                                                        
    MODBUS_COOL_HEAT_MODE                                 =  _P("MODBUS_COOL_HEAT_MODE");                   
    MODBUS_MODE_OPERATION                                 =  _P("MODBUS_MODE_OPERATION");                   
    MODBUS_SEQUENCE                                       =  _P("MODBUS_SEQUENCE");                         
    MODBUS_DEGC_OR_F                                      =  _P("MODBUS_DEGC_OR_F");                        
    MODBUS_FAN_MODE                                       =  _P("MODBUS_FAN_MODE");                         
    MODBUS_POWERUP_MODE                                   =  _P("MODBUS_POWERUP_MODE");                     
    MODBUS_AUTO_ONLY                                      =  _P("MODBUS_AUTO_ONLY");                        
    MODBUS_FACTORY_DEFAULTS                               =  _P("MODBUS_FACTORY_DEFAULTS");                 
    MODBUS_INFO_BYTE                                      =  _P("MODBUS_INFO_BYTE");                        
    MODBUS_BAUDRATE                                       =  _P("MODBUS_BAUDRATE");                         
    MODBUS_OVERRIDE_TIMER                                 =  _P("MODBUS_OVERRIDE_TIMER");                   
    MODBUS_OVERRIDE_TIMER_LEFT                            =  _P("MODBUS_OVERRIDE_TIMER_LEFT");              
    MODBUS_HEAT_COOL_CONFIG                               =  _P("MODBUS_HEAT_COOL_CONFIG");                 
    MODBUS_TIMER_ON                                       =  _P("MODBUS_TIMER_ON");                         
    MODBUS_TIMER_OFF                                      =  _P("MODBUS_TIMER_OFF");                        
    MODBUS_TIMER_UNITS                                    =  _P("MODBUS_TIMER_UNITS");                      
    MODBUS_DEAD_MASTER                                    =  _P("MODBUS_DEAD_MASTER");                      
    MODBUS_SYSTEM_TIME_FORMAT                             =  _P("MODBUS_SYSTEM_TIME_FORMAT");               
    MODBUS_FREE_COOL_CONFIG                               =  _P("MODBUS_FREE_COOL_CONFIG");                 
    MODBUS_RS485_MODE                                     =  _P("MODBUS_RS485_MODE");                       
    MODBUS_TEMPRATURE_CHIP                                =  _P("MODBUS_TEMPRATURE_CHIP");                  
    MODBUS_ANALOG1_RANGE                                  =  _P("MODBUS_ANALOG1_RANGE");                    
    MODBUS_ANALOG2_RANGE                                  =  _P("MODBUS_ANALOG2_RANGE");                    
    MODBUS_ANALOG3_RANGE                                  =  _P("MODBUS_ANALOG3_RANGE");                    
    MODBUS_ANALOG4_RANGE                                  =  _P("MODBUS_ANALOG4_RANGE");                    
    MODBUS_ANALOG5_RANGE                                  =  _P("MODBUS_ANALOG5_RANGE");                    
    MODBUS_ANALOG6_RANGE                                  =  _P("MODBUS_ANALOG6_RANGE");                    
    MODBUS_ANALOG7_RANGE                                  =  _P("MODBUS_ANALOG7_RANGE");                    
    MODBUS_ANALOG8_RANGE                                  =  _P("MODBUS_ANALOG8_RANGE");                    
    MODBUS_INTERNAL_THERMISTOR                            =  _P("MODBUS_INTERNAL_THERMISTOR");              
    MODBUS_ANALOG_INPUT1                                  =  _P("MODBUS_ANALOG_INPUT1");                    
    MODBUS_ANALOG_INPUT2                                  =  _P("MODBUS_ANALOG_INPUT2");                    
    MODBUS_ANALOG_INPUT3                                  =  _P("MODBUS_ANALOG_INPUT3");                    
    MODBUS_ANALOG_INPUT4                                  =  _P("MODBUS_ANALOG_INPUT4");                    
    MODBUS_ANALOG_INPUT5                                  =  _P("MODBUS_ANALOG_INPUT5");                    
    MODBUS_ANALOG_INPUT6                                  =  _P("MODBUS_ANALOG_INPUT6");                    
    MODBUS_ANALOG_INPUT7                                  =  _P("MODBUS_ANALOG_INPUT7");                    
    MODBUS_ANALOG_INPUT8                                  =  _P("MODBUS_ANALOG_INPUT8");                    
    MODBUS_EXTERNAL_SENSOR1                               =  _P("MODBUS_EXTERNAL_SENSOR1");                 
    MODBUS_EXTERNAL_SENSOR2                               =  _P("MODBUS_EXTERNAL_SENSOR2");                 
    MODBUS_INPUT_MANU_ENABLE                              =  _P("MODBUS_INPUT_MANU_ENABLE");                
    MODBUS_FILTER                                         =  _P("MODBUS_FILTER");                           
    MODBUS_INPUT1_FILTER                                  =  _P("MODBUS_INPUT1_FILTER");                    
    MODBUS_INPUT2_FILTER                                  =  _P("MODBUS_INPUT2_FILTER");                    
    MODBUS_INPUT3_FILTER                                  =  _P("MODBUS_INPUT3_FILTER");                    
    MODBUS_INPUT4_FILTER                                  =  _P("MODBUS_INPUT4_FILTER");                    
    MODBUS_INPUT5_FILTER                                  =  _P("MODBUS_INPUT5_FILTER");                    
    MODBUS_INPUT6_FILTER                                  =  _P("MODBUS_INPUT6_FILTER");                    
    MODBUS_INPUT7_FILTER                                  =  _P("MODBUS_INPUT7_FILTER");                    
    MODBUS_INPUT8_FILTER                                  =  _P("MODBUS_INPUT8_FILTER");                    
    MODBUS_EX_SENSOR1_FILTER                              =  _P("MODBUS_EX_SENSOR1_FILTER");                
    MODBUS_EX_SENSOR2_FILTER                              =  _P("MODBUS_EX_SENSOR2_FILTER");                
    MODBUS_CALIBRATION_TERM                               =  _P("MODBUS_CALIBRATION_TERM");                 
    MODBUS_CALIBRATION_INTERNAL_THERMISTOR                =  _P("MODBUS_CALIBRATION_INTERNAL_THERMISTOR");  
    MODBUS_CALIBRATION_ANALOG1                            =  _P("MODBUS_CALIBRATION_ANALOG1");              
    MODBUS_CALIBRATION_ANALOG2                            =  _P("MODBUS_CALIBRATION_ANALOG2");              
    MODBUS_CALIBRATION_ANALOG3                            =  _P("MODBUS_CALIBRATION_ANALOG3");              
    MODBUS_CALIBRATION_ANALOG4                            =  _P("MODBUS_CALIBRATION_ANALOG4");              
    MODBUS_CALIBRATION_ANALOG5                            =  _P("MODBUS_CALIBRATION_ANALOG5");              
    MODBUS_CALIBRATION_ANALOG6                            =  _P("MODBUS_CALIBRATION_ANALOG6");              
    MODBUS_CALIBRATION_ANALOG7                            =  _P("MODBUS_CALIBRATION_ANALOG7");              
    MODBUS_CALIBRATION_ANALOG8                            =  _P("MODBUS_CALIBRATION_ANALOG8");              
    MODBUS_HUMCOUNT1_H                                    =  _P("MODBUS_HUMCOUNT1_H");                      
    MODBUS_HUMRH1_H                                       =  _P("MODBUS_HUMRH1_H");                         
    MODBUS_HUMCOUNT2_H                                    =  _P("MODBUS_HUMCOUNT2_H");                      
    MODBUS_HUMRH2_H                                       =  _P("MODBUS_HUMRH2_H");                         
    MODBUS_ANALOG1_FUNCTION                               =  _P("MODBUS_ANALOG1_FUNCTION");                 
    MODBUS_ANALOG2_FUNCTION                               =  _P("MODBUS_ANALOG2_FUNCTION");                 
    MODBUS_ANALOG3_FUNCTION                               =  _P("MODBUS_ANALOG3_FUNCTION");                 
    MODBUS_ANALOG4_FUNCTION                               =  _P("MODBUS_ANALOG4_FUNCTION");                 
    MODBUS_ANALOG5_FUNCTION                               =  _P("MODBUS_ANALOG5_FUNCTION");                 
    MODBUS_ANALOG6_FUNCTION                               =  _P("MODBUS_ANALOG6_FUNCTION");                 
    MODBUS_ANALOG7_FUNCTION                               =  _P("MODBUS_ANALOG7_FUNCTION");                 
    MODBUS_ANALOG8_FUNCTION                               =  _P("MODBUS_ANALOG8_FUNCTION");                 
    MODBUS_TABLE1_ZERO                                    =  _P("MODBUS_TABLE1_ZERO");                      
    MODBUS_TABLE1_HALFONE                                 =  _P("MODBUS_TABLE1_HALFONE");                   
    MODBUS_TABLE1_ONE                                     =  _P("MODBUS_TABLE1_ONE");                       
    MODBUS_TABLE1_HALFTWO                                 =  _P("MODBUS_TABLE1_HALFTWO");                   
    MODBUS_TABLE1_TWO                                     =  _P("MODBUS_TABLE1_TWO");                       
    MODBUS_TABLE1_HALFTHREE                               =  _P("MODBUS_TABLE1_HALFTHREE");                 
    MODBUS_TABLE1_THREE                                   =  _P("MODBUS_TABLE1_THREE");                     
    MODBUS_TABLE1_HALFFOUR                                =  _P("MODBUS_TABLE1_HALFFOUR");                  
    MODBUS_TABLE1_FOUR                                    =  _P("MODBUS_TABLE1_FOUR");                      
    MODBUS_TABLE1_HALFFIVE                                =  _P("MODBUS_TABLE1_HALFFIVE");                  
    MODBUS_TABLE1_FIVE                                    =  _P("MODBUS_TABLE1_FIVE");                      
    MODBUS_TABLE2_ZERO                                    =  _P("MODBUS_TABLE2_ZERO");                      
    MODBUS_TABLE2_HALFONE                                 =  _P("MODBUS_TABLE2_HALFONE");                   
    MODBUS_TABLE2_ONE                                     =  _P("MODBUS_TABLE2_ONE");                       
    MODBUS_TABLE2_HALFTWO                                 =  _P("MODBUS_TABLE2_HALFTWO");                   
    MODBUS_TABLE2_TWO                                     =  _P("MODBUS_TABLE2_TWO");                       
    MODBUS_TABLE2_HALFTHREE                               =  _P("MODBUS_TABLE2_HALFTHREE");                 
    MODBUS_TABLE2_THREE                                   =  _P("MODBUS_TABLE2_THREE");                     
    MODBUS_TABLE2_HALFFOUR                                =  _P("MODBUS_TABLE2_HALFFOUR");                  
    MODBUS_TABLE2_FOUR                                    =  _P("MODBUS_TABLE2_FOUR");                      
    MODBUS_TABLE2_HALFFIVE                                =  _P("MODBUS_TABLE2_HALFFIVE");                  
    MODBUS_TABLE2_FIVE                                    =  _P("MODBUS_TABLE2_FIVE");                     
    MODUBS_HUMIDITY_RH                                    =  _P("MODUBS_HUMIDITY_RH");                      
    MODBUS_HUMIDITY_FREQUENCY                             =  _P("MODBUS_HUMIDITY_FREQUENCY");               
    MODBUS_HUM_PIC_UPDATE                                 =  _P("MODBUS_HUM_PIC_UPDATE");                   
    MODBUS_HUM_CAL_NUM                                    =  _P("MODBUS_HUM_CAL_NUM");                      
    MODBUS_HUM_CURRENT_DEFAULT                            =  _P("MODBUS_HUM_CURRENT_DEFAULT");              
    MODBUS_MODE_OUTPUT1                                   =  _P("MODBUS_MODE_OUTPUT1");                     
    MODBUS_MODE_OUTPUT2                                   =  _P("MODBUS_MODE_OUTPUT2");                     
    MODBUS_MODE_OUTPUT3                                   =  _P("MODBUS_MODE_OUTPUT3");                     
    MODBUS_MODE_OUTPUT4                                   =  _P("MODBUS_MODE_OUTPUT4");                     
    MODBUS_MODE_OUTPUT5                                   =  _P("MODBUS_MODE_OUTPUT5");                     
    MODBUS_OUTPUT1_SCALE                                  =  _P("MODBUS_OUTPUT1_SCALE");                    
    MODBUS_OUTPUT2_SCALE                                  =  _P("MODBUS_OUTPUT2_SCALE");                    
    MODBUS_DIGITAL_OUTPUT_STATUS                          =  _P("MODBUS_DIGITAL_OUTPUT_STATUS");            
    MODBUS_COOLING_VALVE                                  =  _P("MODBUS_COOLING_VALVE");                    
    MODBUS_HEATING_VALVE                                  =  _P("MODBUS_HEATING_VALVE");                    
    MODBUS_DAC_OFFSET                                     =  _P("MODBUS_DAC_OFFSET");                       
    MODBUS_OUTPUT1_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT1_DELAY_OFF_TO_ON");          
    MODBUS_OUTPUT2_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT2_DELAY_OFF_TO_ON");          
    MODBUS_OUTPUT3_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT3_DELAY_OFF_TO_ON");          
    MODBUS_OUTPUT4_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT4_DELAY_OFF_TO_ON");          
    MODBUS_OUTPUT5_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT5_DELAY_OFF_TO_ON");          
    MODBUS_OUTPUT6_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT6_DELAY_OFF_TO_ON");          
    MODBUS_OUTPUT7_DELAY_OFF_TO_ON                        =  _P("MODBUS_OUTPUT7_DELAY_OFF_TO_ON");          
    MODBUS_DELAY1_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY1_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_DELAY2_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY2_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_DELAY3_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY3_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_DELAY4_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY4_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_DELAY5_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY5_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_DELAY6_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY6_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_DELAY7_OFF_TO_ON_TIME_CURRENT                  =  _P("MODBUS_DELAY7_OFF_TO_ON_TIME_CURRENT");    
    MODBUS_OUTPUT1_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT1_DELAY_ON_TO_OFF");          
    MODBUS_OUTPUT2_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT2_DELAY_ON_TO_OFF");          
    MODBUS_OUTPUT3_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT3_DELAY_ON_TO_OFF");          
    MODBUS_OUTPUT4_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT4_DELAY_ON_TO_OFF");          
    MODBUS_OUTPUT5_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT5_DELAY_ON_TO_OFF");          
    MODBUS_OUTPUT6_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT6_DELAY_ON_TO_OFF");          
    MODBUS_OUTPUT7_DELAY_ON_TO_OFF                        =  _P("MODBUS_OUTPUT7_DELAY_ON_TO_OFF");          
    MODBUS_DELAY1_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY1_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_DELAY2_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY2_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_DELAY3_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY3_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_DELAY4_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY4_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_DELAY5_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY5_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_DELAY6_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY6_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_DELAY7_ON_TO_OFF_TIME_CURRENT                  =  _P("MODBUS_DELAY7_ON_TO_OFF_TIME_CURRENT");    
    MODBUS_CYCLING_DELAY                                  =  _P("MODBUS_CYCLING_DELAY");                    
    MODBUS_CHANGOVER_DELAY                                =  _P("MODBUS_CHANGOVER_DELAY");                  
    MODBUS_VALVE_TRAVEL_TIME                              =  _P("MODBUS_VALVE_TRAVEL_TIME");                
    MODBUS_VALVE_PERCENT                                  =  _P("MODBUS_VALVE_PERCENT");                    
    MODBUS_INTERLOCK_OUTPUT1                              =  _P("MODBUS_INTERLOCK_OUTPUT1");                
    MODBUS_INTERLOCK_OUTPUT2                              =  _P("MODBUS_INTERLOCK_OUTPUT2");                
    MODBUS_INTERLOCK_OUTPUT3                              =  _P("MODBUS_INTERLOCK_OUTPUT3");                
    MODBUS_INTERLOCK_OUTPUT4                              =  _P("MODBUS_INTERLOCK_OUTPUT4");                
    MODBUS_INTERLOCK_OUTPUT5                              =  _P("MODBUS_INTERLOCK_OUTPUT5");                
    MODBUS_INTERLOCK_OUTPUT6                              =  _P("MODBUS_INTERLOCK_OUTPUT6");                
    MODBUS_INTERLOCK_OUTPUT7                              =  _P("MODBUS_INTERLOCK_OUTPUT7");                
    MODBUS_FREEZE_DELAY_ON                                =  _P("MODBUS_FREEZE_DELAY_ON");                  
    MODBUS_FREEZE_DELAY_OFF                               =  _P("MODBUS_FREEZE_DELAY_OFF");                 
    MODBUS_OUTPUT_MANU_ENABLE                             =  _P("MODBUS_OUTPUT_MANU_ENABLE");               
    MODBUS_MANU_RELAY1                                    =  _P("MODBUS_MANU_RELAY1");                      
    MODBUS_MANU_RELAY2                                    =  _P("MODBUS_MANU_RELAY2");                      
    MODBUS_MANU_RELAY3                                    =  _P("MODBUS_MANU_RELAY3");                      
    MODBUS_MANU_RELAY4                                    =  _P("MODBUS_MANU_RELAY4");                      
    MODBUS_MANU_RELAY5                                    =  _P("MODBUS_MANU_RELAY5");                      
    MODBUS_MANUAL_AO1                                     =  _P("MODBUS_MANUAL_AO1");                       
    MODBUS_MANUAL_AO2                                     =  _P("MODBUS_MANUAL_AO2");                       
    MODBUS_DEADMASTER_AUTO_MANUAL                         =  _P("MODBUS_DEADMASTER_AUTO_MANUAL");           
    MODBUS_DEADMASTER_OUTPUT_STATE                        =  _P("MODBUS_DEADMASTER_OUTPUT_STATE");          
    MODBUS_DEADMASTER_COOL_OUTPUT                         =  _P("MODBUS_DEADMASTER_COOL_OUTPUT");           
    MODBUS_DEADMASTER_HEAT_OUTPUT                         =  _P("MODBUS_DEADMASTER_HEAT_OUTPUT");           
    MODBUS_OUTPUT1_FUNCTION                               =  _P("MODBUS_OUTPUT1_FUNCTION");                 
    MODBUS_OUTPUT2_FUNCTION                               =  _P("MODBUS_OUTPUT2_FUNCTION");                 
    MODBUS_OUTPUT3_FUNCTION                               =  _P("MODBUS_OUTPUT3_FUNCTION");                 
    MODBUS_OUTPUT4_FUNCTION                               =  _P("MODBUS_OUTPUT4_FUNCTION");                 
    MODBUS_OUTPUT5_FUNCTION                               =  _P("MODBUS_OUTPUT5_FUNCTION");                 
    MODBUS_OUTPUT6_FUNCTION                               =  _P("MODBUS_OUTPUT6_FUNCTION");                 
    MODBUS_OUTPUT7_FUNCTION                               =  _P("MODBUS_OUTPUT7_FUNCTION");                 
    MODBUS_FAN_SPEED                                      =  _P("MODBUS_FAN_SPEED");                        
    MODBUS_PID_OUTPUT1                                    =  _P("MODBUS_PID_OUTPUT1");                      
    MODBUS_PID_OUTPUT2                                    =  _P("MODBUS_PID_OUTPUT2");                      
    MODBUS_PID_OUTPUT3                                    =  _P("MODBUS_PID_OUTPUT3");                      
    MODBUS_PID_OUTPUT4                                    =  _P("MODBUS_PID_OUTPUT4");                      
    MODBUS_PID_OUTPUT5                                    =  _P("MODBUS_PID_OUTPUT5");                      
    MODBUS_PID_OUTPUT6                                    =  _P("MODBUS_PID_OUTPUT6");                      
    MODBUS_PID_OUTPUT7                                    =  _P("MODBUS_PID_OUTPUT7");                      
    MODBUS_UNIVERSAL_OUTPUT_BEGIN                         =  _P("MODBUS_UNIVERSAL_OUTPUT_BEGIN");           
    MODBUS_UNIVERSAL_OUTPUT_COOL1                         =  _P("MODBUS_UNIVERSAL_OUTPUT_COOL1");           
    MODBUS_UNIVERSAL_OUTPUT_COOL2                         =  _P("MODBUS_UNIVERSAL_OUTPUT_COOL2");           
    MODBUS_UNIVERSAL_OUTPUT_COOL3                         =  _P("MODBUS_UNIVERSAL_OUTPUT_COOL3");           
    MODBUS_UNIVERSAL_OUTPUT_HEAT1                         =  _P("MODBUS_UNIVERSAL_OUTPUT_HEAT1");           
    MODBUS_UNIVERSAL_OUTPUT_HEAT2                         =  _P("MODBUS_UNIVERSAL_OUTPUT_HEAT2");           
    MODBUS_UNIVERSAL_OUTPUT_HEAT3                         =  _P("MODBUS_UNIVERSAL_OUTPUT_HEAT3");           
    MODBUS_FAN0_OPER_TABLE_COAST                          =  _P("MODBUS_FAN0_OPER_TABLE_COAST");            
    MODBUS_FAN0_OPER_TABLE_COOL1                          =  _P("MODBUS_FAN0_OPER_TABLE_COOL1");            
    MODBUS_FAN0_OPER_TABLE_COOL2                          =  _P("MODBUS_FAN0_OPER_TABLE_COOL2");            
    MODBUS_FAN0_OPER_TABLE_COOL3                          =  _P("MODBUS_FAN0_OPER_TABLE_COOL3");            
    MODBUS_FAN0_OPER_TABLE_HEAT1                          =  _P("MODBUS_FAN0_OPER_TABLE_HEAT1");            
    MODBUS_FAN0_OPER_TABLE_HEAT2                          =  _P("MODBUS_FAN0_OPER_TABLE_HEAT2");            
    MODBUS_FAN0_OPER_TABLE_HEAT3                          =  _P("MODBUS_FAN0_OPER_TABLE_HEAT3");            
    MODBUS_FAN1_OPER_TABLE_COAST                          =  _P("MODBUS_FAN1_OPER_TABLE_COAST");            
    MODBUS_FAN1_OPER_TABLE_COOL1                          =  _P("MODBUS_FAN1_OPER_TABLE_COOL1");            
    MODBUS_FAN1_OPER_TABLE_COOL2                          =  _P("MODBUS_FAN1_OPER_TABLE_COOL2");            
    MODBUS_FAN1_OPER_TABLE_COOL3                          =  _P("MODBUS_FAN1_OPER_TABLE_COOL3");            
    MODBUS_FAN1_OPER_TABLE_HEAT1                          =  _P("MODBUS_FAN1_OPER_TABLE_HEAT1");            
    MODBUS_FAN1_OPER_TABLE_HEAT2                          =  _P("MODBUS_FAN1_OPER_TABLE_HEAT2");            
    MODBUS_FAN1_OPER_TABLE_HEAT3                          =  _P("MODBUS_FAN1_OPER_TABLE_HEAT3");            
    MODBUS_FAN2_OPER_TABLE_COAST                          =  _P("MODBUS_FAN2_OPER_TABLE_COAST");            
    MODBUS_FAN2_OPER_TABLE_COOL1                          =  _P("MODBUS_FAN2_OPER_TABLE_COOL1");            
    MODBUS_FAN2_OPER_TABLE_COOL2                          =  _P("MODBUS_FAN2_OPER_TABLE_COOL2");            
    MODBUS_FAN2_OPER_TABLE_COOL3                          =  _P("MODBUS_FAN2_OPER_TABLE_COOL3");            
    MODBUS_FAN2_OPER_TABLE_HEAT1                          =  _P("MODBUS_FAN2_OPER_TABLE_HEAT1");            
    MODBUS_FAN2_OPER_TABLE_HEAT2                          =  _P("MODBUS_FAN2_OPER_TABLE_HEAT2");            
    MODBUS_FAN2_OPER_TABLE_HEAT3                          =  _P("MODBUS_FAN2_OPER_TABLE_HEAT3");            
    MODBUS_FAN3_OPER_TABLE_COAST                          =  _P("MODBUS_FAN3_OPER_TABLE_COAST");            
    MODBUS_FAN3_OPER_TABLE_COOL1                          =  _P("MODBUS_FAN3_OPER_TABLE_COOL1");            
    MODBUS_FAN3_OPER_TABLE_COOL2                          =  _P("MODBUS_FAN3_OPER_TABLE_COOL2");            
    MODBUS_FAN3_OPER_TABLE_COOL3                          =  _P("MODBUS_FAN3_OPER_TABLE_COOL3");            
    MODBUS_FAN3_OPER_TABLE_HEAT1                          =  _P("MODBUS_FAN3_OPER_TABLE_HEAT1");            
    MODBUS_FAN3_OPER_TABLE_HEAT2                          =  _P("MODBUS_FAN3_OPER_TABLE_HEAT2");            
    MODBUS_FAN3_OPER_TABLE_HEAT3                          =  _P("MODBUS_FAN3_OPER_TABLE_HEAT3");            
    MODBUS_FANAUT_OPER_TABLE_COAST                        =  _P("MODBUS_FANAUT_OPER_TABLE_COAST");          
    MODBUS_FANAUT_OPER_TABLE_COOL1                        =  _P("MODBUS_FANAUT_OPER_TABLE_COOL1");          
    MODBUS_FANAUT_OPER_TABLE_COOL2                        =  _P("MODBUS_FANAUT_OPER_TABLE_COOL2");          
    MODBUS_FANAUT_OPER_TABLE_COOL3                        =  _P("MODBUS_FANAUT_OPER_TABLE_COOL3");          
    MODBUS_FANAUT_OPER_TABLE_HEAT1                        =  _P("MODBUS_FANAUT_OPER_TABLE_HEAT1");          
    MODBUS_FANAUT_OPER_TABLE_HEAT2                        =  _P("MODBUS_FANAUT_OPER_TABLE_HEAT2");          
    MODBUS_FANAUT_OPER_TABLE_HEAT3                        =  _P("MODBUS_FANAUT_OPER_TABLE_HEAT3");          
    MODBUS_VALVE_OPERATION_TABLE_BEGIN                    =  _P("MODBUS_VALVE_OPERATION_TABLE_BEGIN");      
    MODBUS_VALVE_OPER_TABLE_COOL1                         =  _P("MODBUS_VALVE_OPER_TABLE_COOL1");           
    MODBUS_VALVE_OPER_TABLE_COOL2                         =  _P("MODBUS_VALVE_OPER_TABLE_COOL2");           
    MODBUS_VALVE_OPER_TABLE_COOL3                         =  _P("MODBUS_VALVE_OPER_TABLE_COOL3");           
    MODBUS_VALVE_OPER_TABLE_HEAT1                         =  _P("MODBUS_VALVE_OPER_TABLE_HEAT1");           
    MODBUS_VALVE_OPER_TABLE_HEAT2                         =  _P("MODBUS_VALVE_OPER_TABLE_HEAT2");           
    MODBUS_VALVE_OPER_TABLE_HEAT3                         =  _P("MODBUS_VALVE_OPER_TABLE_HEAT3");           
    MODBUS_HEAT_UNIVERSAL_TABLE                           =  _P("MODBUS_HEAT_UNIVERSAL_TABLE");             
    MODBUS_COOL_UNIVERSAL_TABLE                           =  _P("MODBUS_COOL_UNIVERSAL_TABLE");             
    MODBUS_HEAT_ORIGINAL_TABLE                            =  _P("MODBUS_HEAT_ORIGINAL_TABLE");              
    MODBUS_COOL_ORIGINAL_TABLE                            =  _P("MODBUS_COOL_ORIGINAL_TABLE");              
    MODBUS_VALVE_OFF_TABLE_COAST                          =  _P("MODBUS_VALVE_OFF_TABLE_COAST");            
    MODBUS_VALVE_OFF_TABLE_COOL1                          =  _P("MODBUS_VALVE_OFF_TABLE_COOL1");            
    MODBUS_VALVE_OFF_TABLE_COOL2                          =  _P("MODBUS_VALVE_OFF_TABLE_COOL2");            
    MODBUS_VALVE_OFF_TABLE_COOL3                          =  _P("MODBUS_VALVE_OFF_TABLE_COOL3");            
    MODBUS_VALVE_OFF_TABLE_HEAT1                          =  _P("MODBUS_VALVE_OFF_TABLE_HEAT1");            
    MODBUS_VALVE_OFF_TABLE_HEAT2                          =  _P("MODBUS_VALVE_OFF_TABLE_HEAT2");            
    MODBUS_VALVE_OFF_TABLE_HEAT3                          =  _P("MODBUS_VALVE_OFF_TABLE_HEAT3");            
    MODBUS_DEFAULT_SETPOINT                               =  _P("MODBUS_DEFAULT_SETPOINT");                 
    MODBUS_SETPOINT_CONTROL                               =  _P("MODBUS_SETPOINT_CONTROL");                 
	MODBUS_DAYSETPOINT_OPTION                             =  _P("MODBUS_DAYSETPOINT_OPTION");               
	MODBUS_MIDDLE_SETPOINT                                =  _P("MODBUS_MIDDLE_SETPOINT");                  
	MODBUS_DAY_SETPOINT                                   =  _P("MODBUS_DAY_SETPOINT");                     
	MODBUS_DAY_COOLING_DEADBAND                           =  _P("MODBUS_DAY_COOLING_DEADBAND");             
	MODBUS_DAY_HEATING_DEADBAND                           =  _P("MODBUS_DAY_HEATING_DEADBAND");             
	MODBUS_DAY_COOLING_SETPOINT                           =  _P("MODBUS_DAY_COOLING_SETPOINT");             
	MODBUS_DAY_HEATING_SETPOINT                           =  _P("MODBUS_DAY_HEATING_SETPOINT");             
	MODBUS_NIGHT_SETPOINT                                 =  _P("MODBUS_NIGHT_SETPOINT");                   
	MODBUS_APPLICATION                                    =  _P("MODBUS_APPLICATION");                      
	MODBUS_NIGHT_HEATING_DEADBAND                         =  _P("MODBUS_NIGHT_HEATING_DEADBAND");           
	MODBUS_NIGHT_COOLING_DEADBAND                         =  _P("MODBUS_NIGHT_COOLING_DEADBAND");           
	MODBUS_NIGHT_HEATING_SETPOINT                         =  _P("MODBUS_NIGHT_HEATING_SETPOINT");           
	MODBUS_NIGHT_COOLING_SETPOINT                         =  _P("MODBUS_NIGHT_COOLING_SETPOINT");           
	MODBUS_WINDOW_INTERLOCK_COOLING_SETPOINT              =  _P("MODBUS_WINDOW_INTERLOCK_COOLING_SETPOINT");
    MODBUS_WINDOW_INTERLOCK_HEATING_SETPOINT              =  _P("MODBUS_WINDOW_INTERLOCK_HEATING_SETPOINT");
    MODBUS_UNIVERSAL_NIGHTSET                             =  _P("MODBUS_UNIVERSAL_NIGHTSET");               
    MODBUS_UNIVERSAL_SET                                  =  _P("MODBUS_UNIVERSAL_SET");                    
    MODBUS_UNIVERSAL_DB_HI                                =  _P("MODBUS_UNIVERSAL_DB_HI");                  
    MODBUS_UNIVERSAL_DB_LO                                =  _P("MODBUS_UNIVERSAL_DB_LO");                  
    MODBUS_ECOMONY_COOLING_SETPOINT                       =  _P("MODBUS_ECOMONY_COOLING_SETPOINT");         
    MODBUS_ECOMONY_HEATING_SETPOINT                       =  _P("MODBUS_ECOMONY_HEATING_SETPOINT");         
    MODBUS_POWERUP_SETPOINT                               =  _P("MODBUS_POWERUP_SETPOINT");                 
    MODBUS_MAX_SETPOINT                                   =  _P("MODBUS_MAX_SETPOINT");                     
    MODBUS_MIN_SETPOINT                                   =  _P("MODBUS_MIN_SETPOINT");                     
    MODBUS_MAX_SETPOINT2                                  =  _P("MODBUS_MAX_SETPOINT2");                    
    MODBUS_MIN_SETPOINT2                                  =  _P("MODBUS_MIN_SETPOINT2");                    
    MODBUS_MAX_SETPOINT3                                  =  _P("MODBUS_MAX_SETPOINT3");                    
    MODBUS_MIN_SETPOINT3                                  =  _P("MODBUS_MIN_SETPOINT3");                    
    MODBUS_MAX_SETPOINT4                                  =  _P("MODBUS_MAX_SETPOINT4");                    
    MODBUS_MIN_SETPOINT4                                  =  _P("MODBUS_MIN_SETPOINT4");                    
    MODBUS_SETPOINT_INCREASE                              =  _P("MODBUS_SETPOINT_INCREASE");                
    MODBUS_FREEZE_TEMP_SETPOINT                           =  _P("MODBUS_FREEZE_TEMP_SETPOINT");             
    MODBUS_WALL_SETPOINT                                  =  _P("MODBUS_WALL_SETPOINT");                    
    MODBUS_CEILING_SETPOINT                               =  _P("MODBUS_CEILING_SETPOINT");                 
    MODBUS_AVERAGE_SETPOINT                               =  _P("MODBUS_AVERAGE_SETPOINT");                 
    MODBUS_UNOCCUPIED_HEATING                             =  _P("MODBUS_UNOCCUPIED_HEATING");               
    MODBUS_UNOCCUPIED_COOLING                             =  _P("MODBUS_UNOCCUPIED_COOLING");               
    MODBUS_RH_SETPOINT                                    =  _P("MODBUS_RH_SETPOINT");                      
    MODBUS_CURRENT1_SETPOINT                              =  _P("MODBUS_CURRENT1_SETPOINT");                
    MODBUS_TEMP_SELECT                                    =  _P("MODBUS_TEMP_SELECT");                      
    MODBUS_INPUT1_SELECT                                  =  _P("MODBUS_INPUT1_SELECT");                    
    MODBUS_COOLING_PID                                    =  _P("MODBUS_COOLING_PID");                      
    MODBUS_COOLING_PTERM                                  =  _P("MODBUS_COOLING_PTERM");                    
    MODBUS_COOLING_ITERM                                  =  _P("MODBUS_COOLING_ITERM");                    
    MODBUS_UNIVERSAL_PTERM                                =  _P("MODBUS_UNIVERSAL_PTERM");                  
    MODBUS_UNIVERSAL_ITERM                                =  _P("MODBUS_UNIVERSAL_ITERM");                  
    MODBUS_PID_UNIVERSAL                                  =  _P("MODBUS_PID_UNIVERSAL");                    
    MODBUS_UNITS1_HIGH                                    =  _P("MODBUS_UNITS1_HIGH");                      
    MODBUS_UNITS1_LOW                                     =  _P("MODBUS_UNITS1_LOW");                       
    MODBUS_UNITS2_HIGH                                    =  _P("MODBUS_UNITS2_HIGH");                      
    MODBUS_UNITS2_LOW                                     =  _P("MODBUS_UNITS2_LOW");                       
    MODBUS_PID2_MODE_OPERATION                            =  _P("MODBUS_PID2_MODE_OPERATION");              
    MODBUS_KEYPAD_SELECT                                  =  _P("MODBUS_KEYPAD_SELECT");                    
    MODBUS_SPECIAL_MENU_LOCK                              =  _P("MODBUS_SPECIAL_MENU_LOCK");                
    MODBUS_DISPLAY                                        =  _P("MODBUS_DISPLAY");                          
    MODBUS_ICON                                           =  _P("MODBUS_ICON");                             
    MODBUS_LAST_KEY_TIMER                                 =  _P("MODBUS_LAST_KEY_TIMER");                   
    MODBUS_KEYPAD_VALUE                                   =  _P("MODBUS_KEYPAD_VALUE");                     
    MODBUS_DISPLAY_HUNDRED                                =  _P("MODBUS_DISPLAY_HUNDRED");                  
    MODBUS_DISPLAY_TEN                                    =  _P("MODBUS_DISPLAY_TEN");                      
    MODBUS_DISPLAY_DIGITAL                                =  _P("MODBUS_DISPLAY_DIGITAL");                  
    MODBUS_DISPLAY_STATUS                                 =  _P("MODBUS_DISPLAY_STATUS");                   
    MODBUS_ROUND_DISPLAY                                  =  _P("MODBUS_ROUND_DISPLAY");                    
    MODBUS_MIN_ADDRESS                                    =  _P("MODBUS_MIN_ADDRESS");                      
    MODBUS_MAX_ADDRESS                                    =  _P("MODBUS_MAX_ADDRESS");                      
    MODBUS_EEPROM_SIZE                                    =  _P("MODBUS_EEPROM_SIZE");                      
    MODBUS_TIMER_SELECT                                   =  _P("MODBUS_TIMER_SELECT");                     
    MODBUS_YEAR                                           =  _P("MODBUS_YEAR");                             
    MODBUS_MONTH                                          =  _P("MODBUS_MONTH");                            
    MODBUS_WEEK                                           =  _P("MODBUS_WEEK");                             
    MODBUS_DAY                                            =  _P("MODBUS_DAY");                              
    MODBUS_HOUR                                           =  _P("MODBUS_HOUR");                             
    MODBUS_MINUTE                                         =  _P("MODBUS_MINUTE");                           
    MODBUS_SECOND                                         =  _P("MODBUS_SECOND");                           
    MODBUS_DIAGNOSTIC_ALARM                               =  _P("MODBUS_DIAGNOSTIC_ALARM");                 
    MODBUS_DAY1_EVENT1_HI                                 =  _P("MODBUS_DAY1_EVENT1_HI");                   
    MODBUS_DAY1_EVENT1_LO                                 =  _P("MODBUS_DAY1_EVENT1_LO");                   
    MODBUS_DAY1_EVENT2_HI                                 =  _P("MODBUS_DAY1_EVENT2_HI");                   
    MODBUS_DAY1_EVENT2_LO                                 =  _P("MODBUS_DAY1_EVENT2_LO");                   
    MODBUS_DAY1_EVENT3_HI                                 =  _P("MODBUS_DAY1_EVENT3_HI");                   
    MODBUS_DAY1_EVENT3_LO                                 =  _P("MODBUS_DAY1_EVENT3_LO");                   
    MODBUS_DAY1_EVENT4_HI                                 =  _P("MODBUS_DAY1_EVENT4_HI");                   
    MODBUS_DAY1_EVENT4_LO                                 =  _P("MODBUS_DAY1_EVENT4_LO");                   
    MODBUS_DAY2_EVENT1_HI                                 =  _P("MODBUS_DAY2_EVENT1_HI");                   
    MODBUS_DAY2_EVENT1_LO                                 =  _P("MODBUS_DAY2_EVENT1_LO");                   
    MODBUS_DAY2_EVENT2_HI                                 =  _P("MODBUS_DAY2_EVENT2_HI");                   
    MODBUS_DAY2_EVENT2_LO                                 =  _P("MODBUS_DAY2_EVENT2_LO");                   
    MODBUS_DAY2_EVENT3_HI                                 =  _P("MODBUS_DAY2_EVENT3_HI");                   
    MODBUS_DAY2_EVENT3_LO                                 =  _P("MODBUS_DAY2_EVENT3_LO");                   
    MODBUS_DAY2_EVENT4_HI                                 =  _P("MODBUS_DAY2_EVENT4_HI");                   
    MODBUS_DAY2_EVENT4_LO                                 =  _P("MODBUS_DAY2_EVENT4_LO");                   
    MODBUS_LCD_TURN_OFF                                   =  _P("MODBUS_LCD_TURN_OFF");                     
    MODBUS_LINE1_CHAR1                                    =  _P("MODBUS_LINE1_CHAR1");                      
    MODBUS_LINE1_CHAR2                                    =  _P("MODBUS_LINE1_CHAR2");                      
    MODBUS_LINE1_CHAR3                                    =  _P("MODBUS_LINE1_CHAR3");                      
    MODBUS_LINE1_CHAR4                                    =  _P("MODBUS_LINE1_CHAR4");                      
    MODBUS_LINE2_CHAR1                                    =  _P("MODBUS_LINE2_CHAR1");                      
    MODBUS_LINE2_CHAR2                                    =  _P("MODBUS_LINE2_CHAR2");                      
    MODBUS_LINE2_CHAR3                                    =  _P("MODBUS_LINE2_CHAR3");                      
    MODBUS_LINE2_CHAR4                                    =  _P("MODBUS_LINE2_CHAR4");                      
    MODBUS_INTERNAL_CHAR1                                 =  _P("MODBUS_INTERNAL_CHAR1");                   
    MODBUS_INTERNAL_CHAR2                                 =  _P("MODBUS_INTERNAL_CHAR2");                   
    MODBUS_INTERNAL_CHAR3                                 =  _P("MODBUS_INTERNAL_CHAR3");                   
    MODBUS_INTERNAL_CHAR4                                 =  _P("MODBUS_INTERNAL_CHAR4");                   
    MODBUS_AI1_CHAR1                                      =  _P("MODBUS_AI1_CHAR1");                        
    MODBUS_AI1_CHAR2                                      =  _P("MODBUS_AI1_CHAR2");                        
    MODBUS_AI1_CHAR3                                      =  _P("MODBUS_AI1_CHAR3");                        
    MODBUS_AI1_CHAR4                                      =  _P("MODBUS_AI1_CHAR4");                        
    MODBUS_AI2_CHAR1                                      =  _P("MODBUS_AI2_CHAR1");                        
    MODBUS_AI2_CHAR2                                      =  _P("MODBUS_AI2_CHAR2");                        
    MODBUS_AI2_CHAR3                                      =  _P("MODBUS_AI2_CHAR3");                        
    MODBUS_AI2_CHAR4                                      =  _P("MODBUS_AI2_CHAR4");                        
    MODBUS_AI3_CHAR1                                      =  _P("MODBUS_AI3_CHAR1");                        
    MODBUS_AI3_CHAR2                                      =  _P("MODBUS_AI3_CHAR2");                        
    MODBUS_AI3_CHAR3                                      =  _P("MODBUS_AI3_CHAR3");                        
    MODBUS_AI3_CHAR4                                      =  _P("MODBUS_AI3_CHAR4");                        
    MODBUS_AI4_CHAR1                                      =  _P("MODBUS_AI4_CHAR1");                        
    MODBUS_AI4_CHAR2                                      =  _P("MODBUS_AI4_CHAR2");                        
    MODBUS_AI4_CHAR3                                      =  _P("MODBUS_AI4_CHAR3");                        
    MODBUS_AI4_CHAR4                                      =  _P("MODBUS_AI4_CHAR4");                        
    MODBUS_AI5_CHAR1                                      =  _P("MODBUS_AI5_CHAR1");                        
    MODBUS_AI5_CHAR2                                      =  _P("MODBUS_AI5_CHAR2");                        
    MODBUS_AI5_CHAR3                                      =  _P("MODBUS_AI5_CHAR3");                        
    MODBUS_AI5_CHAR4                                      =  _P("MODBUS_AI5_CHAR4");                        
    MODBUS_AI6_CHAR1                                      =  _P("MODBUS_AI6_CHAR1");                        
    MODBUS_AI6_CHAR2                                      =  _P("MODBUS_AI6_CHAR2");                        
    MODBUS_AI6_CHAR3                                      =  _P("MODBUS_AI6_CHAR3");                        
    MODBUS_AI6_CHAR4                                      =  _P("MODBUS_AI6_CHAR4");                        
    MODBUS_AI7_CHAR1                                      =  _P("MODBUS_AI7_CHAR1");                        
    MODBUS_AI7_CHAR2                                      =  _P("MODBUS_AI7_CHAR2");                        
    MODBUS_AI7_CHAR3                                      =  _P("MODBUS_AI7_CHAR3");                        
    MODBUS_AI7_CHAR4                                      =  _P("MODBUS_AI7_CHAR4");                        
    MODBUS_AI8_CHAR1                                      =  _P("MODBUS_AI8_CHAR1");                        
    MODBUS_AI8_CHAR2                                      =  _P("MODBUS_AI8_CHAR2");                        
    MODBUS_AI8_CHAR3                                      =  _P("MODBUS_AI8_CHAR3");                        
    MODBUS_AI8_CHAR4                                      =  _P("MODBUS_AI8_CHAR4");                        
    MODBUS_OUTPUT1_CHAR1                                  =  _P("MODBUS_OUTPUT1_CHAR1");                    
    MODBUS_OUTPUT1_CHAR2                                  =  _P("MODBUS_OUTPUT1_CHAR2");                    
    MODBUS_OUTPUT1_CHAR3                                  =  _P("MODBUS_OUTPUT1_CHAR3");                    
    MODBUS_OUTPUT1_CHAR4                                  =  _P("MODBUS_OUTPUT1_CHAR4");                    
    MODBUS_OUTPUT2_CHAR1                                  =  _P("MODBUS_OUTPUT2_CHAR1");                    
    MODBUS_OUTPUT2_CHAR2                                  =  _P("MODBUS_OUTPUT2_CHAR2");                    
    MODBUS_OUTPUT2_CHAR3                                  =  _P("MODBUS_OUTPUT2_CHAR3");                    
    MODBUS_OUTPUT2_CHAR4                                  =  _P("MODBUS_OUTPUT2_CHAR4");                    
    MODBUS_OUTPUT3_CHAR1                                  =  _P("MODBUS_OUTPUT3_CHAR1");                    
    MODBUS_OUTPUT3_CHAR2                                  =  _P("MODBUS_OUTPUT3_CHAR2");                    
    MODBUS_OUTPUT3_CHAR3                                  =  _P("MODBUS_OUTPUT3_CHAR3");                    
    MODBUS_OUTPUT3_CHAR4                                  =  _P("MODBUS_OUTPUT3_CHAR4");                    
    MODBUS_OUTPUT4_CHAR1                                  =  _P("MODBUS_OUTPUT4_CHAR1");                    
    MODBUS_OUTPUT4_CHAR2                                  =  _P("MODBUS_OUTPUT4_CHAR2");                    
    MODBUS_OUTPUT4_CHAR3                                  =  _P("MODBUS_OUTPUT4_CHAR3");                    
    MODBUS_OUTPUT4_CHAR4                                  =  _P("MODBUS_OUTPUT4_CHAR4");                    
    MODBUS_OUTPUT5_CHAR1                                  =  _P("MODBUS_OUTPUT5_CHAR1");                    
    MODBUS_OUTPUT5_CHAR2                                  =  _P("MODBUS_OUTPUT5_CHAR2");                    
    MODBUS_OUTPUT5_CHAR3                                  =  _P("MODBUS_OUTPUT5_CHAR3");                    
    MODBUS_OUTPUT5_CHAR4                                  =  _P("MODBUS_OUTPUT5_CHAR4");                    
    MODBUS_OUTPUT6_CHAR1                                  =  _P("MODBUS_OUTPUT6_CHAR1");                    
    MODBUS_OUTPUT6_CHAR2                                  =  _P("MODBUS_OUTPUT6_CHAR2");                    
    MODBUS_OUTPUT6_CHAR3                                  =  _P("MODBUS_OUTPUT6_CHAR3");                    
    MODBUS_OUTPUT6_CHAR4                                  =  _P("MODBUS_OUTPUT6_CHAR4");                    
    MODBUS_OUTPUT7_CHAR1                                  =  _P("MODBUS_OUTPUT7_CHAR1");                    
    MODBUS_OUTPUT7_CHAR2                                  =  _P("MODBUS_OUTPUT7_CHAR2");                    
    MODBUS_OUTPUT7_CHAR3                                  =  _P("MODBUS_OUTPUT7_CHAR3");                    
    MODBUS_OUTPUT7_CHAR4                                  =  _P("MODBUS_OUTPUT7_CHAR4");                    
    MODBUS_SCHEDULEA_CHAR1                                =  _P("MODBUS_SCHEDULEA_CHAR1");                  
    MODBUS_SCHEDULEA_CHAR2                                =  _P("MODBUS_SCHEDULEA_CHAR2");                  
    MODBUS_SCHEDULEA_CHAR3                                =  _P("MODBUS_SCHEDULEA_CHAR3");                  
    MODBUS_SCHEDULEA_CHAR4                                =  _P("MODBUS_SCHEDULEA_CHAR4");                  
    MODBUS_SCHEDULEB_CHAR1                                =  _P("MODBUS_SCHEDULEB_CHAR1");                  
    MODBUS_SCHEDULEB_CHAR2                                =  _P("MODBUS_SCHEDULEB_CHAR2");                  
    MODBUS_SCHEDULEB_CHAR3                                =  _P("MODBUS_SCHEDULEB_CHAR3");                  
    MODBUS_SCHEDULEB_CHAR4                                =  _P("MODBUS_SCHEDULEB_CHAR4");                  
    MODBUS_SCHEDULEC_CHAR1                                =  _P("MODBUS_SCHEDULEC_CHAR1");                  
    MODBUS_SCHEDULEC_CHAR2                                =  _P("MODBUS_SCHEDULEC_CHAR2");                  
    MODBUS_SCHEDULEC_CHAR3                                =  _P("MODBUS_SCHEDULEC_CHAR3");                  
    MODBUS_SCHEDULEC_CHAR4                                =  _P("MODBUS_SCHEDULEC_CHAR4");                  
    MODBUS_SCHEDULED_CHAR1                                =  _P("MODBUS_SCHEDULED_CHAR1");                  
    MODBUS_SCHEDULED_CHAR2                                =  _P("MODBUS_SCHEDULED_CHAR2");                  
    MODBUS_SCHEDULED_CHAR3                                =  _P("MODBUS_SCHEDULED_CHAR3");                  
    MODBUS_SCHEDULED_CHAR4                                =  _P("MODBUS_SCHEDULED_CHAR4");                  
    MODBUS_WALL_CHAR1                                     =  _P("MODBUS_WALL_CHAR1");                       
    MODBUS_WALL_CHAR2                                     =  _P("MODBUS_WALL_CHAR2");                       
    MODBUS_WALL_CHAR3                                     =  _P("MODBUS_WALL_CHAR3");                       
    MODBUS_WALL_CHAR4                                     =  _P("MODBUS_WALL_CHAR4");                       
    MODBUS_CEILING_CHAR1                                  =  _P("MODBUS_CEILING_CHAR1");                    
    MODBUS_CEILING_CHAR2                                  =  _P("MODBUS_CEILING_CHAR2");                    
    MODBUS_CEILING_CHAR3                                  =  _P("MODBUS_CEILING_CHAR3");                    
    MODBUS_CEILING_CHAR4                                  =  _P("MODBUS_CEILING_CHAR4");                    
    MODBUS_OUTDOOR_CHAR1                                  =  _P("MODBUS_OUTDOOR_CHAR1");                    
    MODBUS_OUTDOOR_CHAR2                                  =  _P("MODBUS_OUTDOOR_CHAR2");                    
    MODBUS_OUTDOOR_CHAR3                                  =  _P("MODBUS_OUTDOOR_CHAR3");                    
    MODBUS_OUTDOOR_CHAR4                                  =  _P("MODBUS_OUTDOOR_CHAR4");                    
    MODBUS_AVERAGE_CHAR1                                  =  _P("MODBUS_AVERAGE_CHAR1");                    
    MODBUS_AVERAGE_CHAR2                                  =  _P("MODBUS_AVERAGE_CHAR2");                    
    MODBUS_AVERAGE_CHAR3                                  =  _P("MODBUS_AVERAGE_CHAR3");                    
    MODBUS_AVERAGE_CHAR4                                  =  _P("MODBUS_AVERAGE_CHAR4");                    
    MODBUS_LCD_SCREEN1                                    =  _P("MODBUS_LCD_SCREEN1");                      
    MODBUS_LCD_SCREEN2                                    =  _P("MODBUS_LCD_SCREEN2");                      
    MODBUS_DEMAND_RESPONSE                                =  _P("MODBUS_DEMAND_RESPONSE");                  
    MODBUS_LOCK_REGISTER                                  =  _P("MODBUS_LOCK_REGISTER");                    
    MODBUS_PIR_STAGE                                      =  _P("MODBUS_PIR_STAGE");                        
    MODBUS_FIRST_CAL_FLAG                                 =  _P("MODBUS_FIRST_CAL_FLAG");                   
    MODBUS_HUM_CAL_EREASE                                 =  _P("MODBUS_HUM_CAL_EREASE");                   
    MODBUS_HUMCOUNT3_H                                    =  _P("MODBUS_HUMCOUNT3_H");                      
    MODBUS_HUMRH3_H                                       =  _P("MODBUS_HUMRH3_H");                         
    MODBUS_HUMCOUNT4_H                                    =  _P("MODBUS_HUMCOUNT4_H");                      
    MODBUS_HUMRH4_H                                       =  _P("MODBUS_HUMRH4_H");                         
    MODBUS_HUMCOUNT5_H                                    =  _P("MODBUS_HUMCOUNT5_H");                      
    MODBUS_HUMRH5_H                                       =  _P("MODBUS_HUMRH5_H");                         
    MODBUS_HUMCOUNT6_H                                    =  _P("MODBUS_HUMCOUNT6_H");                      
    MODBUS_HUMRH6_H                                       =  _P("MODBUS_HUMRH6_H");                         
    MODBUS_HUMCOUNT7_H                                    =  _P("MODBUS_HUMCOUNT7_H");                      
    MODBUS_HUMRH7_H                                       =  _P("MODBUS_HUMRH7_H");                         
    MODBUS_HUMCOUNT8_H                                    =  _P("MODBUS_HUMCOUNT8_H");                      
    MODBUS_HUMRH8_H                                       =  _P("MODBUS_HUMRH8_H");                         
    MODBUS_HUMCOUNT9_H                                    =  _P("MODBUS_HUMCOUNT9_H");                      
    MODBUS_HUMRH9_H                                       =  _P("MODBUS_HUMRH9_H");                         
    MODBUS_HUMCOUNT10_H                                   =  _P("MODBUS_HUMCOUNT10_H");                     
    MODBUS_HUMRH10_H                                      =  _P("MODBUS_HUMRH10_H");                        
    MODBUS_HUM_LOCK_A                                     =  _P("MODBUS_HUM_LOCK_A");                       
    MODBUS_HUM_LOCK_B                                     =  _P("MODBUS_HUM_LOCK_B");                       
    MODBUS_LCD_ROTATE_ENABLE                              =  _P("MODBUS_LCD_ROTATE_ENABLE");                
    MODBUS_SCHEDULE_ON_OFF                                =  _P("MODBUS_SCHEDULE_ON_OFF");                  
    MODBUS_DISP_ITEM_TEMPERATURE                          =  _P("MODBUS_DISP_ITEM_TEMPERATURE");            
    MODBUS_DISP_ITEM_SETPOINT                             =  _P("MODBUS_DISP_ITEM_SETPOINT");               
    MODBUS_DISP_ITEM_AI1                                  =  _P("MODBUS_DISP_ITEM_AI1");                    
    MODBUS_DISP_ITEM_AI2                                  =  _P("MODBUS_DISP_ITEM_AI2");                    
    MODBUS_DISP_ITEM_AI3                                  =  _P("MODBUS_DISP_ITEM_AI3");                    
    MODBUS_DISP_ITEM_AI4                                  =  _P("MODBUS_DISP_ITEM_AI4");                    
    MODBUS_DISP_ITEM_AI5                                  =  _P("MODBUS_DISP_ITEM_AI5");                    
    MODBUS_DISP_ITEM_AI6                                  =  _P("MODBUS_DISP_ITEM_AI6");                    
    MODBUS_DISP_ITEM_AI7                                  =  _P("MODBUS_DISP_ITEM_AI7");                    
    MODBUS_DISP_ITEM_AI8                                  =  _P("MODBUS_DISP_ITEM_AI8");                    
    MODBUS_DISP_ITEM_MODE                                 =  _P("MODBUS_DISP_ITEM_MODE");                   
    MODBUS_DISP_ITEM_USER_INFO                            =  _P("MODBUS_DISP_ITEM_USER_INFO");              
    MODBUS_DISP_ITEM_CLOCK_DATE                           =  _P("MODBUS_DISP_ITEM_CLOCK_DATE");             
    MODBUS_DISP_ITEM_CLOCK_TIME                           =  _P("MODBUS_DISP_ITEM_CLOCK_TIME");             
    MODBUS_DISP_ITEM_EXTENAL_SENSOR1                      =  _P("MODBUS_DISP_ITEM_EXTENAL_SENSOR1");        
    MODBUS_DISP_ITEM_EXTENAL_SENSOR2                      =  _P("MODBUS_DISP_ITEM_EXTENAL_SENSOR2");        
    MODBUS_DISP_ITEM_EXTENAL_SENSOR3                      =  _P("MODBUS_DISP_ITEM_EXTENAL_SENSOR3");        
    MODBUS_DISP_ITEM_EXTENAL_SENSOR4                      =  _P("MODBUS_DISP_ITEM_EXTENAL_SENSOR4");        
    MODBUS_DISP_ITEM_OUT1                                 =  _P("MODBUS_DISP_ITEM_OUT1");                   
    MODBUS_DISP_ITEM_OUT2                                 =  _P("MODBUS_DISP_ITEM_OUT2");                   
    MODBUS_DISP_ITEM_OUT3                                 =  _P("MODBUS_DISP_ITEM_OUT3");                   
    MODBUS_DISP_ITEM_OUT4                                 =  _P("MODBUS_DISP_ITEM_OUT4");                   
    MODBUS_DISP_ITEM_OUT5                                 =  _P("MODBUS_DISP_ITEM_OUT5");                   
    MODBUS_DISP_ITEM_OUT6                                 =  _P("MODBUS_DISP_ITEM_OUT6");                   
    MODBUS_DISP_ITEM_OUT7                                 =  _P("MODBUS_DISP_ITEM_OUT7");                   
    MODBUS_OUTPUT_PWM_AUTO_COAST                          =  _P("MODBUS_OUTPUT_PWM_AUTO_COAST");            
    MODBUS_OUTPUT_PWM_AUTO_COOL1                          =  _P("MODBUS_OUTPUT_PWM_AUTO_COOL1");            
    MODBUS_OUTPUT_PWM_AUTO_COOL2                          =  _P("MODBUS_OUTPUT_PWM_AUTO_COOL2");            
    MODBUS_OUTPUT_PWM_AUTO_COOL3                          =  _P("MODBUS_OUTPUT_PWM_AUTO_COOL3");            
    MODBUS_OUTPUT_PWM_AUTO_HEAT1                          =  _P("MODBUS_OUTPUT_PWM_AUTO_HEAT1");            
    MODBUS_OUTPUT_PWM_AUTO_HEAT2                          =  _P("MODBUS_OUTPUT_PWM_AUTO_HEAT2");            
    MODBUS_OUTPUT_PWM_AUTO_HEAT3                          =  _P("MODBUS_OUTPUT_PWM_AUTO_HEAT3");            
    MODBUS_PWM_OUT4                                       =  _P("MODBUS_PWM_OUT4");                         
    MODBUS_PWM_OUT5                                       =  _P("MODBUS_PWM_OUT5");                         
    MODBUS_SUN_ICON_CONTROL                               =  _P("MODBUS_SUN_ICON_CONTROL");                 
    MODBUS_MOON_ICON_CONTROL                              =  _P("MODBUS_MOON_ICON_CONTROL");                
    MODBUS_EXT_SENSOR_TEMP_CAL                            =  _P("MODBUS_EXT_SENSOR_TEMP_CAL");              
    MODBUS_BUTTON_LEFT1                                   =  _P("MODBUS_BUTTON_LEFT1");                     
    MODBUS_BUTTON_LEFT2                                   =  _P("MODBUS_BUTTON_LEFT2");                     
    MODBUS_BUTTON_LEFT3                                   =  _P("MODBUS_BUTTON_LEFT3");                     
    MODBUS_BUTTON_LEFT4                                   =  _P("MODBUS_BUTTON_LEFT4");                     
    

	MODBUS_HUM_HEATER_CONTROL_ENABLE=_P("MODBUS_HUM_HEATER_CONTROL_ENABLE");
	MODBUS_HUM_INPUT_MANUAL_ENABLE=_P("MODBUS_HUM_INPUT_MANUAL_ENABLE");
	MODBUS_HUM_INPUT_MANUAL_VALUE=_P("MODBUS_HUM_INPUT_MANUAL_VALUE");                             
	MODBUS_CO2_INPUT_MANUAL_ENABLE=_P("MODBUS_CO2_INPUT_MANUAL_ENABLE");                            
	MODBUS_CO2_INPUT_VALUE=_P("MODBUS_CO2_INPUT_VALUE");                                    
	MODBUS_CO2_CALIBRATION_DATA=_P("MODBUS_CO2_CALIBRATION_DATA");                              
	MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN");                         
	MODBUS_UNIVERSAL_OFF_OUTPUT_COOL1=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_COOL1");                         
	MODBUS_UNIVERSAL_OFF_OUTPUT_COOL2=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_COOL2");                         
	MODBUS_UNIVERSAL_OFF_OUTPUT_COOL3=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_COOL3");                         
	MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT1=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT1");                         
	MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT2=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT2");                         
	MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT3=_P("MODBUS_UNIVERSAL_OFF_OUTPUT_HEAT3");                        
	MODBUS_UNIVERSAL_OFF_VALVE_BEGIN=_P("MODBUS_UNIVERSAL_OFF_VALVE_BEGIN");                         
	MODBUS_UNIVERSAL_OFF_VALVE_COOL1=_P("MODBUS_UNIVERSAL_OFF_VALVE_COOL1");                          
	MODBUS_UNIVERSAL_OFF_VALVE_COOL2=_P("MODBUS_UNIVERSAL_OFF_VALVE_COOL2");                          
	MODBUS_UNIVERSAL_OFF_VALVE_COOL3=_P("MODBUS_UNIVERSAL_OFF_VALVE_COOL3");                          
	MODBUS_UNIVERSAL_OFF_VALVE_HEAT1=_P("MODBUS_UNIVERSAL_OFF_VALVE_HEAT1");                          
	MODBUS_UNIVERSAL_OFF_VALVE_HEAT2=_P("MODBUS_UNIVERSAL_OFF_VALVE_HEAT2");                          
	MODBUS_UNIVERSAL_OFF_VALVE_HEAT3=_P("MODBUS_UNIVERSAL_OFF_VALVE_HEAT3");                         

	MODBUS_VALUE_SENSOR=_P("MODBUS_VALUE_SENSOR");                                     
	MODBUS_PIR_SELECT_ENABLE =_P("MODBUS_PIR_SELECT_ENABLE");                                 
	MODBUS_PIR_REAL_VALUE=_P("MODBUS_PIR_REAL_VALUE");                                     
	MODBUS_PIR_OCCUPIED=_P("MODBUS_PIR_OCCUPIED");                                       
                         




	MODBUS_HEATING_PID                                    =  _P("MODBUS_HEATING_PID");                      
    MODBUS_CALIBRATION                                    =  _P("MODBUS_CALIBRATION");                      
    MODBUS_CALIBRATION_ANALOG_IN1                         =  _P("MODBUS_CALIBRATION_ANALOG_IN1");           
    MODBUS_DELAY                                          =  _P("MODBUS_DELAY");                            
    MODBUS_HEATING_PTERM                                  =  _P("MODBUS_HEATING_PTERM");                    
    MODBUS_HEATING_ITERM                                  =  _P("MODBUS_HEATING_ITERM");                    
    MODBUS_COOLING_DEADBAND                               =  _P("MODBUS_COOLING_DEADBAND");                 
    MODBUS_HEATING_DEADBAND                               =  _P("MODBUS_HEATING_DEADBAND");                 
    MODBUS_OUTPUT_SCALE                                   =  _P("MODBUS_OUTPUT_SCALE");                     
    MODBUS_COOLING_SETPOINT                               =  _P("MODBUS_COOLING_SETPOINT");                 
    MODBUS_HEATING_SETPOINT                               =  _P("MODBUS_HEATING_SETPOINT");                 
    MODBUS_EXTERNAL_SENSOR_0                              =  _P("MODBUS_EXTERNAL_SENSOR_0");                
    MODBUS_EXTERNAL_SENSOR_1                              =  _P("MODBUS_EXTERNAL_SENSOR_1");                
    MODBUS_ANALOG_IN1                                     =  _P("MODBUS_ANALOG_IN1");                       
    MODBUS_ANALOG_IN2                                     =  _P("MODBUS_ANALOG_IN2");                       
    MODBUS_DIGITAL_IN1                                    =  _P("MODBUS_DIGITAL_IN1");                      
    MODBUS_LED1_CONTROL                                   =  _P("MODBUS_LED1_CONTROL");                     
    MODBUS_LED2_CONTROL                                   =  _P("MODBUS_LED2_CONTROL");                     
    MODBUS_LED3_CONTROL                                   =  _P("MODBUS_LED3_CONTROL");                     
    MODBUS_LED4_CONTROL                                   =  _P("MODBUS_LED4_CONTROL");                     
    MODBUS_LED5_CONTROL                                   =  _P("MODBUS_LED5_CONTROL");                     
    MODBUS_LED6_CONTROL                                   =  _P("MODBUS_LED6_CONTROL");                     
    MODBUS_LED7_CONTROL                                   =  _P("MODBUS_LED7_CONTROL");                     
    MODBUS_INTERNAL_TEMP_IC                               =  _P("MODBUS_INTERNAL_TEMP_IC");                 
    MODBUS_CALIBRATION_ANALOG_IN2                         =  _P("MODBUS_CALIBRATION_ANALOG_IN2");           
    MODBUS_UNIVERSAL_VALVE_BEGIN                          =  _P("MODBUS_UNIVERSAL_VALVE_BEGIN");            
    MODBUS_UNIVERSAL_VALVE_COOL1                          =  _P("MODBUS_UNIVERSAL_VALVE_COOL1");            
    MODBUS_UNIVERSAL_VALVE_COOL2                          =  _P("MODBUS_UNIVERSAL_VALVE_COOL2");            
    MODBUS_UNIVERSAL_VALVE_COOL3                          =  _P("MODBUS_UNIVERSAL_VALVE_COOL3");            
    MODBUS_UNIVERSAL_VALVE_HEAT1                          =  _P("MODBUS_UNIVERSAL_VALVE_HEAT1");            
    MODBUS_UNIVERSAL_VALVE_HEAT2                          =  _P("MODBUS_UNIVERSAL_VALVE_HEAT2");            
    MODBUS_UNIVERSAL_VALVE_HEAT3                          =  _P("MODBUS_UNIVERSAL_VALVE_HEAT3");            
    MODBUS_DIGITAL1_FUNCTION                              =  _P("MODBUS_DIGITAL1_FUNCTION");                
    MODBUS_DIGITAL_INPUT                                  =  _P("MODBUS_DIGITAL_INPUT");                    
    MODBUS_DEBUGTIMER                                     =  _P("MODBUS_DEBUGTIMER");                       
    MODBUS_FAN_GRIDPOINT                                  =  _P("MODBUS_FAN_GRIDPOINT");                    
    MODBUS_MODE_GRIDPOINT                                 =  _P("MODBUS_MODE_GRIDPOINT");                   
    MODBUS_HOLD_GRIDPOINT                                 =  _P("MODBUS_HOLD_GRIDPOINT");                   
    MODBUS_CONFIGURATION                                  =  _P("MODBUS_CONFIGURATION");                    
    MODBUS_UTIL_MODE                                      =  _P("MODBUS_UTIL_MODE");                        
    MODBUS_ROTATION_OUT2                                  =  _P("MODBUS_ROTATION_OUT2");                    
    MODBUS_ROTATION_OUT3                                  =  _P("MODBUS_ROTATION_OUT3");                    
    MODBUS_ROTATION_OUT4                                  =  _P("MODBUS_ROTATION_OUT4");                    
    MODBUS_ROTATION_OUT5                                  =  _P("MODBUS_ROTATION_OUT5");                    
    MODBUS_ROTATION_TIME_LEFT                             =  _P("MODBUS_ROTATION_TIME_LEFT");               
    MODBUS_FREE_COOL_OUT_TEMP                             =  _P("MODBUS_FREE_COOL_OUT_TEMP");               
    MODBUS_NC_MASTER_TIME                                 =  _P("MODBUS_NC_MASTER_TIME");                   
    MODBUS_FREE_COOL_HYSTERISIS                           =  _P("MODBUS_FREE_COOL_HYSTERISIS");             
    MODBUS_OUTPUT_FREE_COOL_COAST                         =  _P("MODBUS_OUTPUT_FREE_COOL_COAST");           
    MODBUS_OUTPUT_FREE_COOL_COOL1                         =  _P("MODBUS_OUTPUT_FREE_COOL_COOL1");           
    MODBUS_OUTPUT_FREE_COOL_COOL2                         =  _P("MODBUS_OUTPUT_FREE_COOL_COOL2");           
    MODBUS_OUTPUT_FREE_COOL_COOL3                         =  _P("MODBUS_OUTPUT_FREE_COOL_COOL3");           
    MODBUS_OUTPUT_FREE_COOL_HEAT1                         =  _P("MODBUS_OUTPUT_FREE_COOL_HEAT1");           
    MODBUS_OUTPUT_FREE_COOL_HEAT2                         =  _P("MODBUS_OUTPUT_FREE_COOL_HEAT2");           
    MODBUS_OUTPUT_FREE_COOL_HEAT3                         =  _P("MODBUS_OUTPUT_FREE_COOL_HEAT3");           
    MODBUS_MIN_AIR                                        =  _P("MODBUS_MIN_AIR");                          
    MODBUS_OAT_HOTTEST                                    =  _P("MODBUS_OAT_HOTTEST");                      
    MODBUS_OAT_COLDEST                                    =  _P("MODBUS_OAT_COLDEST");                      
    MODBUS_OFFSET_HOTTEST                                 =  _P("MODBUS_OFFSET_HOTTEST");                   
    MODBUS_OFFSET_COLDEST                                 =  _P("MODBUS_OFFSET_COLDEST");                   
    MODBUS_TWO_BYTE_SETPOINT                              =  _P("MODBUS_TWO_BYTE_SETPOINT");                
    MODBUS_CURRENT_SETPOINT                               =  _P("MODBUS_CURRENT_SETPOINT");                 
    MODBUS_SETPOINT_OFFSET                                =  _P("MODBUS_SETPOINT_OFFSET");                  
    MODBUS_CHANGEOVER_SENSOR_MODE                         =  _P("MODBUS_CHANGEOVER_SENSOR_MODE");           
    MODBUS_EXTERNAL_SENSOR_INUSE                          =  _P("MODBUS_EXTERNAL_SENSOR_INUSE");            
    MODBUS_EXTERNAL_SENSOR_STATUS                         =  _P("MODBUS_EXTERNAL_SENSOR_STATUS");           
    MODBUS_NEW_COOLING_SETPOINT                           =  _P("MODBUS_NEW_COOLING_SETPOINT");             
    MODBUS_LINE1_CHAR5                                    =  _P("MODBUS_LINE1_CHAR5");                      
    MODBUS_LINE1_CHAR6                                    =  _P("MODBUS_LINE1_CHAR6");                      
    MODBUS_LINE1_CHAR7                                    =  _P("MODBUS_LINE1_CHAR7");                      
    MODBUS_LINE1_CHAR8                                    =  _P("MODBUS_LINE1_CHAR8");                      
    MODBUS_WINDOW_INTERLOCK_DELAY                         =  _P("MODBUS_WINDOW_INTERLOCK_DELAY");           
    MODBUS_LINE2_CHAR5                                    =  _P("MODBUS_LINE2_CHAR5");                      
    MODBUS_LINE2_CHAR6                                    =  _P("MODBUS_LINE2_CHAR6");                      
    MODBUS_LINE2_CHAR7                                    =  _P("MODBUS_LINE2_CHAR7");                      
    MODBUS_LINE2_CHAR8                                    =  _P("MODBUS_LINE2_CHAR8");                      
    MODBUS_DURA_WINTER_SUMMER                             =  _P("MODBUS_DURA_WINTER_SUMMER");               
    MODBUS_FREE_COOL_SETPOINT                             =  _P("MODBUS_FREE_COOL_SETPOINT");               
    MODBUS_FREE_COOL_MAX_SETPOINT                         =  _P("MODBUS_FREE_COOL_MAX_SETPOINT");           
    MODBUS_FREE_COOL_MIN_SETPOINT                         =  _P("MODBUS_FREE_COOL_MIN_SETPOINT");           
    MODBUS_FREE_COOL_MIN_DAMPER                           =  _P("MODBUS_FREE_COOL_MIN_DAMPER");             
    MODBUS_HEAT_COOL                                      =  _P("MODBUS_HEAT_COOL");                        
    MODBUS_TURN_OFF_FAN                                   =  _P("MODBUS_TURN_OFF_FAN");                     
    MODBUS_UI_LINE1_CHAR1                                 =  _P("MODBUS_UI_LINE1_CHAR1");                   
    MODBUS_UI_LINE1_CHAR2                                 =  _P("MODBUS_UI_LINE1_CHAR2");                   
    MODBUS_UI_LINE1_CHAR3                                 =  _P("MODBUS_UI_LINE1_CHAR3");                   
    MODBUS_UI_LINE1_CHAR4                                 =  _P("MODBUS_UI_LINE1_CHAR4");                   
    MODBUS_UI_LINE2_CHAR1                                 =  _P("MODBUS_UI_LINE2_CHAR1");                   
    MODBUS_UI_LINE2_CHAR2                                 =  _P("MODBUS_UI_LINE2_CHAR2");                   
    MODBUS_UI_LINE2_CHAR3                                 =  _P("MODBUS_UI_LINE2_CHAR3");                   
    MODBUS_UI_LINE2_CHAR4                                 =  _P("MODBUS_UI_LINE2_CHAR4");                   
    MODBUS_UI_INTERNAL_CHAR1                              =  _P("MODBUS_UI_INTERNAL_CHAR1");                
    MODBUS_UI_INTERNAL_CHAR2                              =  _P("MODBUS_UI_INTERNAL_CHAR2");                
    MODBUS_UI_INTERNAL_CHAR3                              =  _P("MODBUS_UI_INTERNAL_CHAR3");                
    MODBUS_UI_INTERNAL_CHAR4                              =  _P("MODBUS_UI_INTERNAL_CHAR4");                
    MODBUS_UI_AI1_CHAR1                                   =  _P("MODBUS_UI_AI1_CHAR1");                     
    MODBUS_UI_AI1_CHAR2                                   =  _P("MODBUS_UI_AI1_CHAR2");                     
    MODBUS_UI_AI1_CHAR3                                   =  _P("MODBUS_UI_AI1_CHAR3");                     
    MODBUS_UI_AI1_CHAR4                                   =  _P("MODBUS_UI_AI1_CHAR4");                     
    MODBUS_UI_AI2_CHAR1                                   =  _P("MODBUS_UI_AI2_CHAR1");                     
    MODBUS_UI_AI2_CHAR2                                   =  _P("MODBUS_UI_AI2_CHAR2");                     
    MODBUS_UI_AI2_CHAR3                                   =  _P("MODBUS_UI_AI2_CHAR3");                     
    MODBUS_UI_AI2_CHAR4                                   =  _P("MODBUS_UI_AI2_CHAR4");                     
    MODBUS_UI_AI3_CHAR1                                   =  _P("MODBUS_UI_AI3_CHAR1");                     
    MODBUS_UI_AI3_CHAR2                                   =  _P("MODBUS_UI_AI3_CHAR2");                     
    MODBUS_UI_AI3_CHAR3                                   =  _P("MODBUS_UI_AI3_CHAR3");                     
    MODBUS_UI_AI3_CHAR4                                   =  _P("MODBUS_UI_AI3_CHAR4");                     
    MODBUS_UI_AI4_CHAR1                                   =  _P("MODBUS_UI_AI4_CHAR1");                     
    MODBUS_UI_AI4_CHAR2                                   =  _P("MODBUS_UI_AI4_CHAR2");                     
    MODBUS_UI_AI4_CHAR3                                   =  _P("MODBUS_UI_AI4_CHAR3");                     
    MODBUS_UI_AI4_CHAR4                                   =  _P("MODBUS_UI_AI4_CHAR4");                     
    MODBUS_UI_AI5_CHAR1                                   =  _P("MODBUS_UI_AI5_CHAR1");                     
    MODBUS_UI_AI5_CHAR2                                   =  _P("MODBUS_UI_AI5_CHAR2");                     
    MODBUS_UI_AI5_CHAR3                                   =  _P("MODBUS_UI_AI5_CHAR3");                     
    MODBUS_UI_AI5_CHAR4                                   =  _P("MODBUS_UI_AI5_CHAR4");                     
    MODBUS_UI_AI6_CHAR1                                   =  _P("MODBUS_UI_AI6_CHAR1");                     
    MODBUS_UI_AI6_CHAR2                                   =  _P("MODBUS_UI_AI6_CHAR2");                     
    MODBUS_UI_AI6_CHAR3                                   =  _P("MODBUS_UI_AI6_CHAR3");                     
    MODBUS_UI_AI6_CHAR4                                   =  _P("MODBUS_UI_AI6_CHAR4");                     
    MODBUS_UI_AI7_CHAR1                                   =  _P("MODBUS_UI_AI7_CHAR1");                     
    MODBUS_UI_AI7_CHAR2                                   =  _P("MODBUS_UI_AI7_CHAR2");                     
    MODBUS_UI_AI7_CHAR3                                   =  _P("MODBUS_UI_AI7_CHAR3");                     
    MODBUS_UI_AI7_CHAR4                                   =  _P("MODBUS_UI_AI7_CHAR4");                     
    MODBUS_UI_AI8_CHAR1                                   =  _P("MODBUS_UI_AI8_CHAR1");                     
    MODBUS_UI_AI8_CHAR2                                   =  _P("MODBUS_UI_AI8_CHAR2");                     
    MODBUS_UI_AI8_CHAR3                                   =  _P("MODBUS_UI_AI8_CHAR3");                     
    MODBUS_UI_AI8_CHAR4                                   =  _P("MODBUS_UI_AI8_CHAR4");                     
    MODBUS_WINDOW_HEATING_SETPOINT                        =  _P("MODBUS_WINDOW_HEATING_SETPOINT");          
    MODBUS_WINDOW_COOLING_SETPOINT                        =  _P("MODBUS_WINDOW_COOLING_SETPOINT");          
    MODBUS_RELAY_PWM_ENABLE                               =  _P("MODBUS_RELAY_PWM_ENABLE");                 
    MODBUS_RELAY_PWM_TOTAL_DUTY                           =  _P("MODBUS_RELAY_PWM_TOTAL_DUTY");             
    MODBUS_RELAY_PWM_HIGH_DUTY                            =  _P("MODBUS_RELAY_PWM_HIGH_DUTY");
	MODBUS_SUPPLY_SETPOINT	=	_P("MODBUS_SUPPLY_SETPOINT");
	MODBUS_MAX_SUPPLY_SETPOINT	=	_P("MODBUS_MAX_SUPPLY_SETPOINT");
	MODBUS_MIN_SUPPLY_SETPOINT	=	_P("MODBUS_MIN_SUPPLY_SETPOINT");
	MODBUS_MAX_AIRFLOW_COOLING	=	_P("MODBUS_MAX_AIRFLOW_COOLING");
	MODBUS_MAX_AIRFLOW_HEATING	=	_P("MODBUS_MAX_AIRFLOW_HEATING");
	MODBUS_MIN_AIRFLOW	=	_P("MODBUS_MIN_AIRFLOW");
	MODBUS_AIRFLOW_SETPOINT 	=	_P("MODBUS_AIRFLOW_SETPOINT");
	MODBUS_VAV_CONTROL	=	_P("MODBUS_VAV_CONTROL");
	MODBUS_SUPPLY_TEMP_SELECT	=	_P("MODBUS_SUPPLY_TEMP_SELECT");
	MODBUS_PID3_VALVE_OPERATION_TABLE_BEGIN 	=	_P("MODBUS_PID3_VALVE_OPERATION_TABLE_BEGIN");
	MODBUS_PID3_VALVE_OPER_TABLE_COOL1	=	_P("MODBUS_PID3_VALVE_OPER_TABLE_COOL1");
	MODBUS_PID3_VALVE_OPER_TABLE_COOL2	=	_P("MODBUS_PID3_VALVE_OPER_TABLE_COOL2");
	MODBUS_PID3_VALVE_OPER_TABLE_COOL3	=	_P("MODBUS_PID3_VALVE_OPER_TABLE_COOL3");
	MODBUS_PID3_VALVE_OPER_TABLE_HEAT1	=	_P("MODBUS_PID3_VALVE_OPER_TABLE_HEAT1");
	MODBUS_PID3_VALVE_OPER_TABLE_HEAT2	=	_P("MODBUS_PID3_VALVE_OPER_TABLE_HEAT2");
	MODBUS_PID3_VALVE_OPER_TABLE_HEAT3	=	_P("MODBUS_PID3_VALVE_OPER_TABLE_HEAT3");
	MODBUS_PID3_COOLING_DB	=	_P("MODBUS_PID3_COOLING_DB");
	MODBUS_PID3_HEATING_DB	=	_P("MODBUS_PID3_HEATING_DB");
	MODBUS_PID3_PTERM	=	_P("MODBUS_PID3_PTERM");
	MODBUS_PID3_ITERM	=	_P("MODBUS_PID3_ITERM");
	MODBUS_PID3_HEAT_STAGE	=	_P("MODBUS_PID3_HEAT_STAGE");
	MODBUS_PID3_COOL_STAGE	=	_P("MODBUS_PID3_COOL_STAGE");
	MODBUS_PID3_OUTPUT	=	_P("MODBUS_PID3_OUTPUT");
	MODBUS_PID3_OUTPUT_BEGIN	=	_P("MODBUS_PID3_OUTPUT_BEGIN");
	MODBUS_PID3_OUTPUT_COOL1	=	_P("MODBUS_PID3_OUTPUT_COOL1");
	MODBUS_PID3_OUTPUT_COOL2	=	_P("MODBUS_PID3_OUTPUT_COOL2");
	MODBUS_PID3_OUTPUT_COOL3	=	_P("MODBUS_PID3_OUTPUT_COOL3");
	MODBUS_PID3_OUTPUT_HEAT1	=	_P("MODBUS_PID3_OUTPUT_HEAT1");
	MODBUS_PID3_OUTPUT_HEAT2	=	_P("MODBUS_PID3_OUTPUT_HEAT2");
	MODBUS_PID3_OUTPUT_HEAT3	=	_P("MODBUS_PID3_OUTPUT_HEAT3");
	/*MODBUS_OUTPUT6_FUNCTION		=	_P("MODBUS_OUTPUT6_FUNCTION");
	MODBUS_OUTPUT7_FUNCTION		=	_P("MODBUS_OUTPUT7_FUNCTION");*/
	/*MODBUS_ANALOG3_FUNCTION		=	_P("MODBUS_ANALOG3_FUNCTION");
	MODBUS_ANALOG4_FUNCTION		=	_P("MODBUS_ANALOG4_FUNCTION");
	MODBUS_ANALOG5_FUNCTION		=	_P("MODBUS_ANALOG5_FUNCTION");
	MODBUS_ANALOG6_FUNCTION		=	_P("MODBUS_ANALOG6_FUNCTION");
	MODBUS_ANALOG7_FUNCTION		=	_P("MODBUS_ANALOG7_FUNCTION");*/
	MODBUS_PID3_VALVE_OFF_TABLE_BEGIN=	_P("MODBUS_PID3_VALVE_OFF_TABLE_BEGIN");
	MODBUS_PID3_VALVE_OFF_TABLE_COOL1=	_P("MODBUS_PID3_VALVE_OFF_TABLE_COOL1");
	MODBUS_PID3_VALVE_OFF_TABLE_COOL2=	_P("MODBUS_PID3_VALVE_OFF_TABLE_COOL2");
	MODBUS_PID3_VALVE_OFF_TABLE_COOL3=	_P("MODBUS_PID3_VALVE_OFF_TABLE_COOL3");
	MODBUS_PID3_VALVE_OFF_TABLE_HEAT1=	_P("MODBUS_PID3_VALVE_OFF_TABLE_HEAT1");
	MODBUS_PID3_VALVE_OFF_TABLE_HEAT2=	_P("MODBUS_PID3_VALVE_OFF_TABLE_HEAT2");
	MODBUS_PID3_VALVE_OFF_TABLE_HEAT3=	_P("MODBUS_PID3_VALVE_OFF_TABLE_HEAT3");
	MODBUS_PID3_OFF_OUTPUT_BEGIN	=	_P("MODBUS_PID3_OFF_OUTPUT_BEGIN");
	MODBUS_PID3_OFF_OUTPUT_COOL1	=	_P("MODBUS_PID3_OFF_OUTPUT_COOL1");
	MODBUS_PID3_OFF_OUTPUT_COOL2	=	_P("MODBUS_PID3_OFF_OUTPUT_COOL2");
	MODBUS_PID3_OFF_OUTPUT_COOL3	=	_P("MODBUS_PID3_OFF_OUTPUT_COOL3");
	MODBUS_PID3_OFF_OUTPUT_HEAT1	=	_P("MODBUS_PID3_OFF_OUTPUT_HEAT1");
	MODBUS_PID3_OFF_OUTPUT_HEAT2	=	_P("MODBUS_PID3_OFF_OUTPUT_HEAT2");
	MODBUS_PID3_OFF_OUTPUT_HEAT3	=	_P("MODBUS_PID3_OFF_OUTPUT_HEAT3");
	return 0;
	}