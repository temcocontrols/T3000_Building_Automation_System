/*  define.h */
#ifndef COMMON_H
#define COMMON_H

struct baudrate_def
{
    int ncomport;
    int baudrate;
    int test_ret;
};

#include "define.h"
OUTPUT int write_multi_Short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length);
OUTPUT int Read_One(TS_UC device_var,TS_US address);
OUTPUT bool open_com(int m_com);
OUTPUT bool open_com_nocretical(int m_com);
OUTPUT void close_com();
OUTPUT void close_com_nocritical(int ncomport);
OUTPUT bool is_connect();
OUTPUT int CheckTstatOnline(TS_UC devLo=1,TS_UC devHi=254);
OUTPUT bool Change_BaudRate(int new_baudrate);
OUTPUT bool Change_BaudRate_NoCretical(int new_baudrate, int ncomport);
OUTPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);
OUTPUT int CheckTstatOnline2(TS_UC devLo,TS_UC devHi);
//Socket
OUTPUT bool Open_Socket(CString strIPAdress);
OUTPUT void SetCommunicationType(int nType);
OUTPUT bool Open_Socket2(CString strIPAdress,short nPort);
OUTPUT bool Open_Socket2_multy_thread(CString strIPAdress, short nPort, int nindex);

OUTPUT void SetLastSuccessBaudrate(int nbaudrate);
OUTPUT void SetLastOpenedComport(int ncom_port);
OUTPUT void SetLastCommunicationType(int n_commnication_type);


OUTPUT int GetLastSuccessBaudrate();
OUTPUT int GetLastOpenedComport();
OUTPUT int GetLastCommunicationType();
//for scan network controller
OUTPUT int NetController_CheckTstatOnline(TS_UC devLo=1,TS_UC devHi=254);
OUTPUT int NetController_CheckTstatOnline2(TS_UC devLo,TS_UC devHi);

//read multible bytes to check is there more than one tstates have the same id. 

OUTPUT HANDLE GetCommunicationHandle();
OUTPUT BOOL bTCPDisconnected();
OUTPUT void SetComnicationHandle(int nType,HANDLE hCommunication);


//////////////////////////////////////////////////////////////////////////
OUTPUT int NetController_CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
OUTPUT int NetController_CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
OUTPUT int CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
OUTPUT int CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
OUTPUT int CheckTstatOnline_nocretical(TS_UC devLo, TS_UC devHi, bool bComm_Type, int ncomport);
OUTPUT int CheckTstatOnline2_a_nocretical(TS_UC devLo, TS_UC devHi, bool bComm_Type, int ncomport);

OUTPUT int Read_One2(TS_UC device_var,TS_US address, bool bComm_Type);
OUTPUT int Write_One2(TS_UC device_var,TS_US address,TS_US value, bool bComm_Type);
OUTPUT int Write_One2_nocretical(TS_UC device_var, TS_US address, TS_US val, bool bComm_Type, int ncomport);
//OUTPUT int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length);
//OUTPUT int read_multi(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length);
OUTPUT int read_multi2(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length, bool bComm_Type);
OUTPUT int read_multi2_nocretical(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length, bool bComm_Type,int ncomport);
OUTPUT int read_ptp_data(unsigned char device_var, unsigned char *put_data_into_here, TS_UC command, TS_UC start_instance, TS_UC end_instance, TS_US entitysize);
OUTPUT int write_ptp_data(unsigned char device_var, char *to_write, unsigned short nlength);
OUTPUT int Read_One_log(TS_UC device_var,TS_US address,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
OUTPUT int Write_One_log(TS_UC device_var,TS_US address,TS_US val,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
OUTPUT int read_multi_log(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,TS_US length,
	                       unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,
	                        int* sendDataLength, int* recvDataLength);
OUTPUT int write_multi_log(TS_UC device_var,TS_UC *to_write,TS_US start_address,TS_US length,
	                       unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,
	                         int* sendDataLength, int* recvDataLength);
OUTPUT int write_multi_Short_log(TS_UC device_var,TS_US *to_write,TS_US start_address,TS_US length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,int* sendDataLength, int* recvDataLength);

OUTPUT int Write_Coil_log(TS_UC device_var, TS_US address, TS_BOOL val, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
OUTPUT int write_multi_Coil_log(TS_UC device_var, TS_BOOL *to_write, TS_US start_address, TS_US length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);

CStdioFile*					m_pFile;
CString						m_strFileINI;
CString m_strScanNetfilename;
unsigned char g_data_to_send[6]={0,1,0,0,0,6};
//CStdioFile* g_fileScanLog;
CCriticalSection  logfile_section;
CCriticalSection  NET_logfile_section;
OUTPUT void close_T3000_log_file();
OUTPUT void write_T3000_log_file(CString StrTips);
OUTPUT void Create_T3000_log_file();
OUTPUT CString Get_NowTime();
void SaveBufferToLogFile(TS_UC* pBuffer, int nSize);

//--------------------------------
OUTPUT void SetCommunicationType(int nType);
OUTPUT int GetCommunicationType(void);
OUTPUT void SetResponseTime(TS_US Time);
OUTPUT int Modbus_Standard_Read(TS_UC device_var, TS_US *put_data_into_here,int function_code, TS_US start_address, int length,
	unsigned char *put_senddate_into_here,
	unsigned char *put_revdata_into_here,
	int* sendDataLength, int* recvDataLength);
OUTPUT int SendData(TS_US *to_write,TS_US length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,int* sendDataLength, int* recvDataLength,BOOL CHECK_CRC) ;
OUTPUT int Test_Comport(int comport, baudrate_def * ntest_ret);
#endif
