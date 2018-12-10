#ifndef GLOBAL_FUNCTION_H
#define GLOBAL_FUNCTION_H
#include "stdafx.h"
#include "Global_Struct.h"

int turn_hex_str_to_ten_num(char *str);
unsigned short turn_4_hex_char_to_unsigned_short(char *source);
bool turn_hex_file_line_to_unsigned_char(char *str);
int turn_hex_char_to_int(char c);
void turn_int_to_unsigned_char(char *source,int length_source,TS_UC *aim);
void turn_unsigned_short_to_hex_char(char *order,unsigned short source);
char turn_unsigned_to_char(unsigned short source);

BOOL GetSerialComm_ISP(vector<CString>& szComm);
CString GetExePath(bool bHasSlash);

BOOL HexFileValidation(const CString& strFileName);
BOOL BinFileValidation(const CString& strFileName);
int read_one(unsigned char device_var,unsigned short address,int retry_times=3);
//read modbus register value
CString GetProductName(int ModelID); 
//According to ModelID ,getting the product model name.
CString GetSysTime(); 
//得到系统的时间 精确到秒
 //字符串分割函数
//eg："192.168.0.1" 用.分割，就成了四个字符串 192 168 0 1
void ExtractString(CStringArray& arr, const CString strSrc, const CString sep = _T("\r\n"));
int Get_Binfile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info);
int Get_HexFile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info,int Address);

BOOL ReadLineFromHexFile(CFile& file, char* pBuffer);
BOOL DoHEXCRC( TS_UC* szBuf, int nLen);
BOOL Ping(const CString& strIP, CWnd* pWndEcho);
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken);
int Write_One_Retry(unsigned char device_var, unsigned short address, unsigned short value, int retry_time = 3);
bool Open_Socket_Retry(CString strIPAdress, short nPort, int retry_time = 3);
#endif