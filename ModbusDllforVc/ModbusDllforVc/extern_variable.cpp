#include "stdafx.h"
#include "define.h" 
    HANDLE m_hSerial = NULL;//串口句柄 - Serial port handle
    HANDLE m_com_h_serial[100] = { 0 }; //用于多线程同时扫描串口 - For multi-threaded concurrent serial port scanning
    OVERLAPPED m_com_osRead[100], m_com_osWrite[100], m_com_osMulWrite[100]; // 用于多线程同时重叠读/写 - For multi-threaded concurrent overlapped read/write
	OVERLAPPED m_osRead, m_osWrite, m_osMulWrite; // 用于重叠读/写 - For overlapped read/write
	TS_UC  gval[128];//the data that get from com   //8
	TS_UC  serinumber_in_dll[4];//only read_one function ,when read 10,
	TS_UC  pval[13];//the data that send from com
	TS_UC  multi_read_val[256];//the register value is put into here,by multi_read function//the number must less 256
	SOCKET m_hSocket=NULL;
    SOCKET m_bip_socket = NULL;
	SOCKET m_hSocket_for_list =	NULL;
    SOCKET m_tcp_hSocket[256];  //用于多线程扫描 - For multi-threaded scanning
    TS_US LATENCY_TIME_COM = 100;
    TS_US LATENCY_TIME_NET = 100;
	HANDLE hDevice=INVALID_HANDLE_VALUE;


	