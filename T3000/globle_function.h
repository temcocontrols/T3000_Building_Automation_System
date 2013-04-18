#ifndef _GLOBLE_FUNCTION_H
#define _GLOBLE_FUNCTION_H

#include "global_variable_extern.h"

int read_one(unsigned char device_var,unsigned short address,int retry_times=3);
int write_one(unsigned char device_var,unsigned short address,short value,int retry_times=3);
int Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times=3);
int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times=3);
int read_one_org(unsigned char device_var,unsigned short address,int retry_times=3);
int write_one_org(unsigned char device_var,unsigned short address,short value,int retry_times=3);
int Write_Multi_org(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times=3);
int Read_Multi_org(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times=3);

int turn_hex_str_to_ten_num(char *str);
unsigned short turn_4_hex_char_to_unsigned_short(char *source);
bool turn_hex_file_line_to_unsigned_char(char *str);
int turn_hex_char_to_int(char c);
void turn_int_to_unsigned_char(char *source,int length_source,unsigned char *aim);
void turn_unsigned_short_to_hex_char(char *order,unsigned short source);
char turn_unsigned_to_char(unsigned short source);
void delete_folder(CString DirName);
CString get_units_from_now_range(int the_tstat_id,int analog1_or_analog2);//for picturetstatmanager 

bool judgement_file_or_path_exist(CString path);
void clear_a_char_array(char *p,char t='\0');
float get_tstat_version(unsigned short tstat_id);

float get_curtstat_version();

int make_sure_isp_mode(int the_tstat_id);
bool get_serialnumber(long & serial,int the_id_of_product=-1);
bool multi_read_tstat(int id);

//
void SetPaneCommunicationPrompt(CString strComInfo);
void SetPaneString(int nIndext,CString str);

bool Get_SerialNumberA(long & serial,int the_id_of_product);
UINT get_serialnumber();
bool can_be_writed_hex_file(int product_model,int hex_file_product_model);
CString get_product_name_by_product_model(int product_model);

//CString GetTempUnit(int analog1_or_analog2=-1);
CString GetTempUnit(int nRange=-1,int nPIDNO = 0);
CString get_product_class_name_by_model_ID(int nModelID);



BOOL GetSerialComPortNumber1(vector<CString>& szComm);
#endif