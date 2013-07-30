#ifndef GLOBLE_FUNCTION_H
#define GLOBLE_FUNCTION_H
#include "stdafx.h"
#include "Global_Struct.h"

int turn_hex_str_to_ten_num(char *str);
unsigned short turn_4_hex_char_to_unsigned_short(char *source);
bool turn_hex_file_line_to_unsigned_char(char *str);
int turn_hex_char_to_int(char c);
void turn_int_to_unsigned_char(char *source,int length_source,TS_UC *aim);
void turn_unsigned_short_to_hex_char(char *order,unsigned short source);
char turn_unsigned_to_char(unsigned short source);

BOOL GetSerialComm(vector<CString>& szComm);
CString GetExePath(bool bHasSlash);

BOOL HexFileValidation(const CString& strFileName);
BOOL BinFileValidation(const CString& strFileName);
int read_one(unsigned char device_var,unsigned short address,int retry_times=3);
//read modbus register value
CString GetProductName(int ModelID); 
//According to ModelID ,getting the product model name.
CString GetSysTime(); 
//�õ�ϵͳ��ʱ�� ��ȷ����
//�ַ����ָ��
//eg��"192.168.0.1" ��.�ָ�ͳ����ĸ��ַ��� 192 168 0 1
void ExtractString(CStringArray& arr, const CString strSrc, const CString sep = _T("\r\n"));
#endif