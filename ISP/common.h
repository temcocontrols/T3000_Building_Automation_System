   /*  define.h */
#ifndef COMMON_H
#define COMMON_H

#include "define.h"
 
 int Write_One(TS_UC device_var,TS_US address,TS_US value);
 int Read_One(TS_UC device_var,TS_US address);
 int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length);
int read_multi(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length);
 bool open_com(TS_UC m_com);
 bool open_com(int m_com);
 void close_com();
 bool is_connect();
 int CheckTstatOnline(TS_UC devLo=1,TS_UC devHi=254);
 bool Change_BaudRate(TS_US new_baudrate);
 bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);

 int CheckTstatOnline2(TS_UC devLo,TS_UC devHi);


//Socket
 bool Open_Socket(CString strIPAdress);
 void SetCommunicationType(int nType);
 int GetCommunicationType(void);
 bool Open_Socket2(CString strIPAdress,short nPort);

//for scan network controller
 int NetController_CheckTstatOnline(TS_UC devLo=1,TS_UC devHi=254);
 int NetController_CheckTstatOnline2(TS_UC devLo,TS_UC devHi);

//read multible bytes to check is there more than one tstates have the same id. 

 HANDLE GetCommunicationHandle();
 BOOL bTCPDisconnected();
 void SetComnicationHandle(int nType,HANDLE hCommunication);


//////////////////////////////////////////////////////////////////////////
 int NetController_CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
 int NetController_CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
 int CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);
  int CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type);

  int Read_One2(TS_UC device_var,TS_US address, bool bComm_Type);
  int Write_One2(TS_UC device_var,TS_US address,TS_US value, bool bComm_Type);
  int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length);
  int read_multi2(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length, bool bComm_Type);

 BOOL Ping(const CString& strIP, CWnd* pWndEcho);
 


#ifndef _DEBUG
void SaveBufferToLogFile(TS_UC* pBuffer, int nSize);
#endif


#endif
