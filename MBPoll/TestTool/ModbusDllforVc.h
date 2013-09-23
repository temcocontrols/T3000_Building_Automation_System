#pragma once
#pragma  comment(lib,"ModbusDllforVc.lib")
#pragma comment(lib, "WS2_32")
#define INPUT extern "C" __declspec(dllimport)

INPUT int Write_One(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One(unsigned char device_var,unsigned short address);
INPUT int write_multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int write_multi_by_tcp(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);
INPUT bool open_com(unsigned char m_com);
INPUT void close_com();
INPUT bool is_connect();
//INPUT bool open_com_for_Command(unsigned char m_com);
INPUT bool Open_Socket2(CString strIPAdress,short nPort);
INPUT bool Change_BaudRate(unsigned short new_baudrate);
INPUT void SetCommunicationType(int nType);

//INPUT int write_multi_block(unsigned char device_var,unsigned short *to_write,unsigned short  start_address,int length);

