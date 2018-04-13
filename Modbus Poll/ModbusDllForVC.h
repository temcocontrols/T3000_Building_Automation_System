#pragma once
#pragma comment(lib,"ModbusDllforVc")
#pragma comment(lib, "WS2_32")
#define INPUT extern "C" __declspec(dllimport)
INPUT int write_multi_Short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length);
//INPUT bool open_com(unsigned char m_com);


INPUT int Write_One_tap(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One_tap(unsigned char device_var,unsigned short address);
INPUT int write_multi_tap(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int read_multi_tap(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);

INPUT bool open_com(int m_com);
INPUT bool open_com_nocretical(int m_com);
INPUT void close_com();
INPUT bool is_connect();
INPUT int CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Change_BaudRate(int new_baurate);
INPUT bool Change_BaudRate_NoCretical(int new_baudrate, int ncomport);
INPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);
INPUT void SetComnicationHandle(int nType,HANDLE hCommunication);
INPUT bool Open_Socket(CString strIPAdress);
INPUT void SetCommunicationType(int nType);
INPUT int NetController_CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Open_Socket2(CString strIPAdress,short nPort);

INPUT int GetLastOpenedComport();
INPUT int GetLastCommunicationType();
INPUT HANDLE GetCommunicationHandle();



INPUT int Write_One(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One(unsigned char device_var,unsigned short address);
INPUT int write_multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);

INPUT int Read_One_log(unsigned char device_var,unsigned short address,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int Write_One_log(unsigned char device_var,unsigned short address,unsigned short val,
                        unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int read_multi_log(unsigned char device_var,unsigned short *put_data_into_here,
	                      unsigned short start_address,int length,
	                       unsigned char *put_senddate_into_here,
	                       unsigned char *put_revdata_into_here, 
	                       int* sendDataLength, int* recvDataLength);
INPUT int write_multi_log(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,
                          unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int write_multi_Short_log(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,//参数命令部分
                                unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, //发送和接受的原始数据
                                int* sendDataLength, int* recvDataLength);
INPUT int Modbus_Standard_Read(unsigned char device_var, unsigned short *put_data_into_here, int function_code, unsigned short start_address, int length,
	unsigned char *put_senddate_into_here,
	unsigned char *put_revdata_into_here,
	int* sendDataLength, int* recvDataLength
      );
INPUT int Write_Coil_log(unsigned char device_var, unsigned short address, bool val, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
INPUT int write_multi_Coil_log(unsigned char device_var, bool *to_write, unsigned short start_address, unsigned short length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);

INPUT void SetResponseTime(unsigned short Time);

INPUT int SendData(unsigned short *to_write,unsigned short length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,int* sendDataLength, int* recvDataLength,BOOL CHECK_CRC) ;
