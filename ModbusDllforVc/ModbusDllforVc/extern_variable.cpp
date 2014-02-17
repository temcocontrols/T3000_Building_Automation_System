#include "stdafx.h"
#include "define.h"

	//extern variable
	TS_US Register_value[MaxRegisterNum];           //got the register value
	TS_UC device=255;                         //Tstat ID,255 is for all
	CString m_com_num;						//com1 or com2
	CMutex mutex;//multithreading lock
 	HANDLE m_hSerial=0;//串口句柄
	OVERLAPPED m_osRead, m_osWrite, m_osMulWrite; // 用于重叠读/写
	TS_UC  gval[13];//the data that get from com   //8
	TS_UC  serinumber_in_dll[4];//only read_one function ,when read 10,
	TS_UC  pval[13];//the data that send from com
	TS_UC  multi_read_val[256];//the register value is put into here,by multi_read function//the number must less 256
	SOCKET m_hSocket=NULL;
	SOCKET m_hSocket_for_list =	NULL;


	