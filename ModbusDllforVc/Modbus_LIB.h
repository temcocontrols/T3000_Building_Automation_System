#pragma once
#include "define.h"
#define TRUE_OR_FALSE	true
#define SLEEP_TIME		50
 
#define DLL_CMODBUS __declspec(dllimport)
 

DLL_CMODBUS class  CModbus
{
public:
	CModbus();
	~CModbus();
public:
	 int write_multi_Short(unsigned char device_var, unsigned short *to_write, unsigned short start_address, int length);
	 int Read_One(TS_UC device_var, TS_US address);
	 bool open_com(int m_com);
	 void close_com();
	 bool is_connect();
	 int CheckTstatOnline(TS_UC devLo = 1, TS_UC devHi = 254);
	 bool Change_BaudRate(int new_baudrate);
	 bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);
	 int CheckTstatOnline2(TS_UC devLo, TS_UC devHi);
	//Socket
	 bool Open_Socket(CString strIPAdress);
 
	 bool Open_Socket2(CString strIPAdress, short nPort);

	 void SetLastSuccessBaudrate(int nbaudrate);
	 void SetLastOpenedComport(int ncom_port);
	 void SetLastCommunicationType(int n_commnication_type);


	 int GetLastSuccessBaudrate();
	 int GetLastOpenedComport();
	 int GetLastCommunicationType();
	 //for scan network controller
	  int NetController_CheckTstatOnline(TS_UC devLo = 1, TS_UC devHi = 254);
	  int NetController_CheckTstatOnline2(TS_UC devLo, TS_UC devHi);

	 //read multible bytes to check is there more than one tstates have the same id. 

	  HANDLE GetCommunicationHandle();
	  BOOL bTCPDisconnected();
	  void SetComnicationHandle(int nType, HANDLE hCommunication);


	 //////////////////////////////////////////////////////////////////////////
	  int NetController_CheckTstatOnline_a(TS_UC devLo, TS_UC devHi, bool bComm_Type);
	  int NetController_CheckTstatOnline2_a(TS_UC devLo, TS_UC devHi, bool bComm_Type);
	  int CheckTstatOnline_a(TS_UC devLo, TS_UC devHi, bool bComm_Type);
	  int CheckTstatOnline2_a(TS_UC devLo, TS_UC devHi, bool bComm_Type);

	  int Read_One2(TS_UC device_var, TS_US address, bool bComm_Type);
	  int Write_One2(TS_UC device_var, TS_US address, TS_US value, bool bComm_Type);
	  int read_multi(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length);
	  int read_multi2(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length, bool bComm_Type);
	  int write_multi(TS_UC device_var, TS_UC *to_write, TS_US start_address, TS_US length);
	  int Read_One_log(TS_UC device_var, TS_US address, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
	  int Write_One(TS_UC device_var, TS_US address, TS_US val);
	  int Write_One_log(TS_UC device_var, TS_US address, TS_US val, 
		  unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here,
		  int* sendDataLength, int* recvDataLength);
// 	  int CModbus::Write_One_log(TS_UC device_var, TS_US address, TS_US val, unsigned char *put_senddate_into_here,
// 		  unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
	  int read_multi_log(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, TS_US length,
		 unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here,
		 int* sendDataLength, int* recvDataLength);
	  int write_multi_log(TS_UC device_var, TS_UC *to_write, TS_US start_address, TS_US length,
		 unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here,
		 int* sendDataLength, int* recvDataLength);
	  int write_multi_Short_log(TS_UC device_var, TS_US *to_write, TS_US start_address, TS_US length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
	  int Read_One_tap(TS_UC device_var, TS_US address);
	  int Write_One_tap(TS_UC device_var, TS_US address, TS_US val);
	  int read_multi_tap(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length);
	  int write_multi_tap(TS_UC device_var, TS_UC *to_write, TS_US start_address, int length);
	  int Write_Coil_log(TS_UC device_var, TS_US address, TS_BOOL val, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
	  int write_multi_Coil_log(TS_UC device_var, TS_BOOL *to_write, TS_US start_address, TS_US length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength);
	   void close_T3000_log_file();
	   void write_T3000_log_file(CString StrTips);
	   void Create_T3000_log_file();
	   CString Get_NowTime();
	  void SaveBufferToLogFile(TS_UC* pBuffer, int nSize);


	  //--------------------------------
	   void SetCommunicationType(int nType);
	   int GetCommunicationType(void);
	   void SetResponseTime(TS_US Time);
	   int Modbus_Standard_Read(TS_UC device_var, TS_US *put_data_into_here, int function_code, TS_US start_address, int length,
		  unsigned char *put_senddate_into_here,
		  unsigned char *put_revdata_into_here,
		  int* sendDataLength, int* recvDataLength);
	   int SendData(TS_US *to_write, TS_US length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength, BOOL CHECK_CRC);

private:
	HANDLE m_hSerial;//串口句柄
	OVERLAPPED m_osRead, m_osWrite, m_osMulWrite; // 用于重叠读/写
	TS_UC  m_gval[128];//the data that get from com   //8
	TS_UC  m_serinumber_in_dll[4];//only read_one function ,when read 10,
	TS_UC  m_pval[13];//the data that send from com
	SOCKET m_hSocket ;
	SOCKET m_hSocket_for_list ;
	TS_US m_LATENCY_TIME_COM ;
	TS_US m_LATENCY_TIME_NET ;

	CStdioFile*					m_pFile;
	CString						m_strFileINI;
	CString m_strScanNetfilename;
	unsigned char g_data_to_send[6] ;
	 
	CCriticalSection  m_logfile_section;
	 
	  int m_baudrate_in_dll  ;
	  int m_open_com_port_number_in_dll  ;
	  int m_old_or_new_scan_protocal_in_dll  ;//1==new protocal;2==old protocal
 
	int m_Commu_type    ; 
	CString m_last_connected_ip;
	int m_last_connected_port    ;
	int m_last_communication_type    ;
	int m_successful_baudrate    ;
	int m_successful_com_port    ;
	//CMutex scan_mutex;
	int m_ArrayBaudate[BAUDRATENUMBER];


	CStdioFile* g_fileScanLog ;
	CStdioFile* g_fileScanNetLog;
};

