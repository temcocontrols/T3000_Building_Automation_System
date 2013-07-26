

#pragma once

#include "stdafx.h"
#include <string>
#include <vector>
//#include <iostream>
#include <fstream>

typedef struct _load_file_every_step
{
	bool first_step;
	bool second_step;
	bool third_step;
	bool thurth_step;
	bool fifth_step;
	bool sixth_step;
	bool seven_step;
	bool eight_step;
	bool keep_down;
}load_file_every_step;
enum 
{
	FAN,
	VALVE,
	HEATPUMP,
};


void _Twrite_to_file_a_line(wofstream & out,const TCHAR * format, ... );


using namespace std;

bool digital2string(int val,CString & str,unsigned char type);
bool string2digital(CString str,unsigned char & val ,bool & type ); // type false = digital ,true = analog
void WriteSerialNumber(wofstream & out);
void WriteAddress(wofstream & out) ;
void Save2File(const char* fn);
//void Save2File_ForTwoFiles(const char* fn);
void Save2File_ForTwoFiles(TCHAR* fn);

void Save2File_ForTwoFilesTSTAT67(TCHAR* fn);//((LPTSTR)(LPCTSTR)strFilename);
void change_showing_text_variable(CString str);
bool before_load_file_inspect(CString path_name,CString name,int i_tstat_id,bool show_message=true);
int get_real_number_fan_speeds(int fan_number);
void LoadFile2Tstat(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file=NULL);
void LoadFile2Tstat67(load_file_every_step &load_file_one_time,TCHAR* fn,CStdioFile*p_log_file=NULL);
bool find_load_file_error(load_file_every_step temppp);
CString Show_load_file_result_message(load_file_every_step temppp,bool show_message_dialog=true);
void Show_load_file_error_message(load_file_every_step temppp,int section,CFile *p_log_file);
void Show_load_file_message(CString message,CFile *p_log_file);
void weekly_write(CStdioFile &default_file,int schedule_id);
void weekly_routines_insert_write(CStdioFile &default_file,int schedule_id,int weekly_row_number);
void annual_write(CStdioFile &default_file,int schedule_id);
void annual_routines_insert_write(CStdioFile &default_file,int schedule_id,int weekly_row_number);
void configure_write(CStdioFile &default_file,int schedule_id);
void variable_write(CStdioFile &default_file,int schedule_id);

void save_schedule_2_file(TCHAR* fn,int schedule_id);

CString get_left_first_array(CString &a_line);
void weekly_read(CStdioFile & default_file,int schedule_id, CStdioFile& logfile);
void weekly_routines_insert_read(CStdioFile &default_file,int schedule_id,int weekly_row_number);
void annual_read(CStdioFile &default_file,int schedule_id, CStdioFile& logfile);
void annual_routines_insert_read(CStdioFile &default_file,int schedule_id,int weekly_row_number);
void configure_read(CStdioFile &default_file,int schedule_id, CStdioFile& logfile);
void variable_read(CStdioFile &default_file,int schedule_id, CStdioFile& logfile);

void load_file_2_schedule(TCHAR* fn,int schedule_id, CStdioFile& file);

void write_to_file_a_line(wofstream & out,const char * format, ...);
int get_real_fan_address(int fan_speed,int fan_place,int place);
void fan_write(wofstream & out,int m_25_heat_stages,int m_25_cool_stages);
void universal_relay_write(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
void universal_value_setting_write(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);

void universal_relay_write_Tstat67OFF(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
void universal_value_setting_write_Tstat67OFF(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);
void universal_relay_write_Tstat67Auto(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
void universal_value_setting_write_Tstat67Auto(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);

void universal_relay_write(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);//UNIVERSAL RELAY OUTPUTS vlaue
void universal_value_setting_write(wofstream & out,int m_26_heat_stages,int m_26_cool_stages);
void value_setting_write(wofstream & out,int m_25_heat_stages,int m_25_cool_stages);

void turn_order_for_fan(int source[],int order[],int m_heat_stages,int m_cool_stages);//used by get_fan_var function and get_value_setting function
bool find_sub_chars(CString m_array_char,CString sub_array_char);//find sub chars is exist?no up or lower?�����Ӵ��Ƿ���ڣ����ִ�Сд
void get_heat_and_cool_stages_from_file_line(TCHAR *buf,int &m_heat_stages,int & m_cool_stages);
void delay_time_write(wofstream & out);
void delay_time_write_Tstat67(wofstream & out);
void lookup_table_write(wofstream & out);
void lookup_table_write_Tstat67(wofstream & out);
void var_write(wofstream & out);
void var_write_Tstat67(wofstream & out);

bool section_end_find(TCHAR *buf);
int  get_fan_var(wifstream & inf,int fan_value[]);
void get_universal_fan_var(wifstream & inf,int fan_value[]);//UNIVERSAL 
void get_fan_line_value(TCHAR *buf,int array[7]);//used by forget_fan_var function

void get_value_setting(wifstream & inf,int value_setting[7]);//for value

void get_delay_setting(wifstream & inf,int value_setting[10]);//for delay
void get_lookup_table_setting(wifstream & inf,int lookup_table_setting[22]);//for lookup table
void get_write_var_line(TCHAR *buf,float tstat_version,CStdioFile *p_log_file=NULL,load_file_every_step *p_log_file_one_time=NULL);
void get_var_write_var(wifstream & inf,float tstat_version,CStdioFile *p_log_file=NULL,load_file_every_step *p_log_file_one_time=NULL);//for var setting