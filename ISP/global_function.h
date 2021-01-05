#ifndef GLOBAL_FUNCTION_H
#define GLOBAL_FUNCTION_H
#include "stdafx.h"
#include "Global_Struct.h"

const int MODBUS_RS485 = 0;
const int MODBUS_TCPIP = 1;
const int MODBUS_BACNET_MSTP = 2;
const int PROTOCOL_BACNET_IP = 3;
const int PROTOCOL_GSM = 4;
const int PROTOCOL_REMOTE_IP = 6;
const int PROTOCOL_BIP_TO_MSTP = 10;
const int PROTOCOL_MSTP_TO_MODBUS = 11;
const int PROTOCOL_BIP_T0_MSTP_TO_MODBUS = 12;    //网络下面的设备，子口跑MSTP设备 ，只能通过Ptransfer 转10000以后寄存器读取
const int PROTOCOL_MB_TCPIP_TO_MB_RS485 = 13;     //20200306 TSTAT10或者T3BB  使用MODBUS MODBUS485 接到  T3BB下面 
const int PROTOCOL_MB_PTP_TRANSFER = 14;          //MODBUS485采用ptp 的方式获取 T3私有数据;
const int PROTOCOL_THIRD_PARTY_BAC_BIP = 253;
const int PROTOCOL_VIRTUAL = 254;
const int PROTOCOL_UNKNOW = 255;

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
CString GetFirmwareUpdateName(int ModelID);
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
int read_multi_retry(unsigned char device_var, unsigned short *put_data_into_here, unsigned short start_address, int length, int retry_time = 3);
int Write_One_Retry(unsigned char device_var, unsigned short address, unsigned short value, int retry_time = 3);
bool Open_Socket_Retry(CString strIPAdress, short nPort, int retry_time = 3);
int check_bootloader_and_frimware(int npid, int comport, unsigned short reg_11, unsigned short reg_14,int &update_value,  unsigned char app_already_version = 0);
int mudbus_read_one(unsigned char device_var, unsigned short address, int retry_times = 3);
int mudbus_write_one(unsigned char device_var, unsigned short address, short value, int retry_times = 1);
int mudbus_read_multi(unsigned char device_var, unsigned short *put_data_into_here, unsigned short start_address, int length, int retry_times = 3);
int mudbus_write_multi_short(unsigned char device_var, unsigned short *to_write, unsigned short start_address, int length, int retry_times = 3);
int mudbus_write_single_short(unsigned char device_var, unsigned char *to_write, unsigned short start_address, int length, int retry_times = 3);
bool Initial_bac(int comport, CString bind_local_ip, int n_baudrate);
#endif