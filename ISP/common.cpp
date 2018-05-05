//*********common.cpp**********
#include "stdafx.h"
#include "common.h"
#include "crc.h"
#include "MyPing.h"
#define TRUE_OR_FALSE	true
#define SLEEP_TIME		50
#define  LATENCY_TIME_COM	60      // 串口延迟，必须600，设为500或更小时，通过NC scan TStat容易失败
                                   // Serial port delay, must be 60 or more milliseconds TBD: Clarify this comment

#define  LATENCY_TIME_NET	100


	//******************************extern variable
	extern TS_US Register_value[MaxRegisterNum];  //Get the register value
	extern TS_UC device;                          //Device ID,255 is for global
	extern CString m_com_num;				//com1 or com2
	extern CMutex mutex;					//multithreading lock
 	extern HANDLE m_hSerial;				// Serial Handle
	extern OVERLAPPED m_osRead, m_osWrite, m_osMulWrite; // 用于重叠读/写, for overlapping read/write
	extern TS_UC  gval[13];			//the data buffer for the received com port data
	extern TS_UC  serinumber_in_dll[4];	//only read_one function ,when read 10,
      extern TS_UC  pval[13];			//the data sent from the com port
	extern TS_UC  multi_read_val[256];		//the register value is put here by the multi_read function
//the number must less than 256
	static int baudrate_in_dll=0;
	static int open_com_port_number_in_dll=65535;
	static int old_or_new_scan_protocal_in_dll=1;	//1==new protocol;2==old protocol

extern 	SOCKET m_hSocket;	
int g_Commu_type=0;						//0:serial modbus//

//CMutex scan_mutex;

#ifndef _DEBUG
const CString g_strLogFile = _T("C:\\T3000Scan_log.txt");
CStdioFile* g_fileScanLog = NULL;
#endif




/*OUTPUT*/ void SetCommunicationType(int nType)
{
	g_Commu_type=nType;
}

/*OUTPUT*/ int GetCommunicationType(void)
{
	return g_Commu_type;
}


/*
OUTPUT bool open_com(TS_UC m_com)
{	
	//open com port, M_com = 0 = com1, 1 = com2 and so on
	//will return the handle to com, m_hSerial,is an external variable
	//the return value ,true is ok,false is failure
	if(open_com_port_number_in_dll==m_com)
	{
		Change_BaudRate(19200);
		return true;///////////////////////////only open a com port once 
	}
	if(m_hSerial != NULL)  
	{//close serial port
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	/////////////////////////////////////////////////////////////////////加入的串口通信部分 , Joined Serial Comms Part
	switch(m_com)
	{
	case 1:					m_hSerial = CreateFile(_T("COM1:"),//Serial handle, open serial port
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
                                                                                                                                      //is a different form, the expression is asynchronous communication, can read and write at the same time, 0 for synchronous Read and write
 
							NULL);break;
	case 2:			        m_hSerial = CreateFile(_T("COM2:"),//Serial handle, open serial port
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
                                                             // same comment as above, 0 for synchronous R/W
							NULL);break;
	case 3:					m_hSerial = CreateFile(_T("COM3:"),//Serial handle, open serial port
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写, same  comment as above
							NULL);break;
	case 4:			        m_hSerial = CreateFile(_T("COM4:"),//Serial handle, open serial port
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写, same comment as above
							NULL);break;
	case 5:					m_hSerial = CreateFile(_T("COM5:"),//Serial handle, open serial port
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
							NULL);
							break;
	case 6:			        m_hSerial = CreateFile(_T("COM6:"),//串口句柄，打开串口
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
							NULL);break;
	case 7:					m_hSerial = CreateFile(_T("COM7:"),//串口句柄，打开串口
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
							NULL);break;
	case 8:			        m_hSerial = CreateFile(_T("COM8:"),//串口句柄，打开串口
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
							NULL);break;
	case 9:			        m_hSerial = CreateFile(_T("COM9:"),//串口句柄，打开串口
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
							NULL);break;
	default :				m_hSerial = CreateFile(_T("COM1:"),//串口句柄，打开串口, serial handle, open serial port
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
							NULL);break;
	}
    if(m_hSerial == INVALID_HANDLE_VALUE)
    {
		CloseHandle(m_hSerial);
		m_hSerial = NULL;		
		return false;
    }
	if(!SetupComm(m_hSerial, 1024*32, 1024*9))
    {
        CloseHandle(m_hSerial);
		m_hSerial = NULL;
    }
    DCB  PortDCB;    
    PortDCB.DCBlength = sizeof(DCB); 
    // 默认串口参数, default serial port parameters
    if(!GetCommState(m_hSerial, &PortDCB))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
		return false;
	}	
	//not to change the baudate
    PortDCB.BaudRate = 19200; // Caution, if baud rate is wrong you can't write to the com port
    PortDCB.ByteSize = 8;     // Number of bytes, 4-8 
    PortDCB.Parity = NOPARITY; 
    PortDCB.StopBits = ONESTOPBIT;  
    if (! SetCommState(m_hSerial, &PortDCB))
    {
        return false;
    }
    COMMTIMEOUTS CommTimeouts;
    GetCommTimeouts(m_hSerial, &CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 160;  
    CommTimeouts.ReadTotalTimeoutMultiplier = 20;  
    CommTimeouts.ReadTotalTimeoutConstant = 360;    
    CommTimeouts.WriteTotalTimeoutMultiplier = 20;  
    CommTimeouts.WriteTotalTimeoutConstant = 200;    
    if (!SetCommTimeouts(m_hSerial, &CommTimeouts))
    {
        return false;
    }          
	open_com_port_number_in_dll=m_com;
	baudrate_in_dll=19200;
	
	return true;
}

*/



/*OUTPUT*/ void close_com()
{
	if(g_Commu_type==0)
	{
		if(m_hSerial != NULL)  
		{//关闭串口
			CloseHandle(m_hSerial);
			m_hSerial = NULL;
		}
		baudrate_in_dll=0;
		open_com_port_number_in_dll=65535;
	}
	if (g_Commu_type==1)
	{
		if (m_hSocket!=INVALID_SOCKET)
		{
			::closesocket(m_hSocket);
			m_hSocket=NULL;
		}
	}
	
}


/*OUTPUT*/ int CheckTstatOnline2(TS_UC devLo,TS_UC devHi)
{
	if(g_Commu_type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,retry
		//the return value == -3, may have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device is detected
		//the return value == -5 ,the input buffer is bad 
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//empty the serial port buffer
		//the return value is the register address
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//Number of bytes sent
		pval[0] = 255;											
		pval[1] = 25;  //TBD: need comments here,
		pval[2] = devHi;
		pval[3] = devLo;
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
		int fState=WriteFile(m_hSerial,//Handle
			pval,// Data buffer address
			6,// Data size
			&m_had_send_data_number,// Returns the number of bytes sent
			&m_osWrite);
		if(!fState)// Overlflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
				//			if(GetLastError()==ERROR_IO_INCOMPLETE)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}

		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(LATENCY_TIME_COM);//Scan has a delay lower than 75ms
		/////////////**************below is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// Handle
			gval,// Data buffer Address
			13,// Data Size
			&m_had_send_data_number,// Returns the number of bytes sent
			&m_osRead);
		if(!fState)// Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}
		//CloseHandle(m_osRead.hEvent);
		/*
		CStdioFile default_file;
		CString saved_path="C:\\modbus_scan_data.txt";
		CString a_line;
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("T:%x %x %x %x %x %x",pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		default_file.WriteString(a_line+"\n");
		a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}
		*/
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);



		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0) 
		{//old scan protocal
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME);	//Need a delay here or this can fail. 
				return -4;             //No response, no connection
			}
			// TBD: Explain this better: found this situation: t3000 can find a comport 
			//which didn’t connect to a device, but write file to the com, will receive the same data with send, 
			// infact the com port is bad but it gave OK data
			if(gval[0]==pval[0] && gval[1]==pval[1] && gval[2]==pval[2] && gval[3]==pval[3] && gval[4]==pval[4] && gval[5]==pval[5])
			{
				Sleep(SLEEP_TIME);
				return -4;              
			}

			//////////////////////////////////////////////////////////////////////////
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME);//Need a delay here
				return -3;
			}
			if((gval[0]!=pval[0]) || (gval[1]!=25))
			{
				Sleep(SLEEP_TIME);//Need a delay here
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//Need a delay here
				return -2;
			}
		}
		else
		{// TBD: Explain this better
             //New scan protocol,if many old devices scan result is oK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);//Need a delay here
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
				return -3;
			if((gval[0]!=pval[0]) || (gval[1]!=25))
				return -2;
			crc=CRC16(gval,7);
			if( gval[7]!=((crc>>8) & 0xff) )
				return -2;
			if(gval[8]!=(crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		return gval[2];
	}

	if(g_Commu_type==1)
	{
		//the second time
		//val         the value that will be written to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try it again
		//the return value == -3, May have more than 2 devices
		//the return value == -4 ,between devLo and devHi, no device was detected ,
		//the return value == -5 ,the input buffer is bad
		//the return value >=1 ,the devLo!=devHi, May have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer

		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//Number of bytes sent
		pval[0]=1;
		pval[1]=2;
		pval[2]=3;
		pval[3]=4;
		pval[4]=5;
		pval[5]=6;

		pval[6] = 255;
		pval[7] = 25;
		pval[8] = devHi;
		pval[9] = devLo;
		
		/*
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;
		*/

		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		::send(m_hSocket,(char*)pval,sizeof(pval),0);

		//Sleep(SLEEP_TIME+8);
		Sleep(LATENCY_TIME_NET);
		TS_UC  rvData[19];
		for(int i=0;i<19;i++)
			rvData[i]=0;
		int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
		if (nRecv>0)
		{
			memcpy(gval,(void*)&rvData[6],sizeof(rvData));
		}
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocal
 			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if((gval[0]!=255) || (gval[1]!=25))
			{
				//		Sleep(SLEEP_TIME);//Need a delay here
				return -2;
			} 
			/*
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				//		Sleep(SLEEP_TIME);// Need a delay here
				return -2;
			}
			*/

		}
		else
		{// new scan protocol,if many old devices scan result is OK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);// Need a delay here

				
			//if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)//to inspect
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect  //zgq: same with com port scan
			{
				//Sleep(SLEEP_TIME+8);
				return -3;

			}
				
			if((gval[0]!=255) || (gval[1]!=25))
				return -2;

			/*
			if (gval[0]==0&&gval[0]==0&&gval[0]==0)
			{
				return -3;
			}
			*/

			/*
			crc=CRC16(gval,7);
			if( gval[7]!=((crc>>8) & 0xff) )
				return -2;
			if(gval[8]!=(crc & 0xff))
				return -2;
				*/
		}
		return gval[2];
	}

	return 0;
}

  int CheckTstatOnline(TS_UC devLo,TS_UC devHi)
{	
	if(g_Commu_type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,retry
		//the return value == -3, may have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device detected
		//the return value == -5 ,the input buffer had some trouble
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		// empty the serial port buffer
		//the return value is the register address
		//Sleep(50);       //Need a delay here
		if(devLo<1 || devHi>254)
			return -5;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=CheckTstatOnline2(devLo,devHi);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=CheckTstatOnline2(devLo,devHi);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0)
			{
				the_return_value2=CheckTstatOnline2(devLo,devHi);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}  
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocol
			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
				return the_return_value;
			int i=0;
			do{
				the_return_value=CheckTstatOnline2(devLo,devHi);
				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
					return the_return_value;
				else if(the_return_value>0)
					i++;
			}while(i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if(g_Commu_type==1)//
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,retry
		//the return value == -3, may have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no devices detected
		//the return value == -5 ,the input buffer is bad
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address
		//Sleep(50);       //Need a delay here
		if(devLo<1 || devHi>254)
			return -5;
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=CheckTstatOnline2(devLo,devHi);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=CheckTstatOnline2(devLo,devHi);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0)
			{
				the_return_value2=CheckTstatOnline2(devLo,devHi);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocol
// 			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
// 				return the_return_value;
// 			int i=0;
// 			do{
// 				the_return_value=CheckTstatOnline2(devLo,devHi);
// 				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
// 					return the_return_value;
// 				else if(the_return_value>0)
// 					i++;
// 			}while(i<3);
			if(the_return_value>0)
			{
				the_return_value2=CheckTstatOnline2(devLo,devHi);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	return 0;
}
  bool Change_BaudRate(TS_US new_baudrate)
{

    //Configure the serial port
	if(new_baudrate!=9600 && new_baudrate!=19200)
		return false;
	if(baudrate_in_dll==new_baudrate)
		return true;
	else
		baudrate_in_dll=new_baudrate;
    DCB  PortDCB;    
    PortDCB.DCBlength = sizeof(DCB); 
    // Default serial port parameters
	int i=0;
	bool successful=false;//true==do it success;false==do it failure
	for(i=0;i<10;i++)
		if(GetCommState(m_hSerial, &PortDCB))
		{
			successful=true;
			break;
		}
	if(successful==false)
		return false;
	//not to change the baudate
     PortDCB.BaudRate = new_baudrate; // baud,if it is wrong then the write will fail. 
    PortDCB.ByteSize = 8;     // Number of bits/byte, 4-8 
    PortDCB.Parity = NOPARITY; 
    PortDCB.StopBits = ONESTOPBIT;  
 	successful=false;
	for(i=0;i<10;i++)
	if(SetCommState(m_hSerial, &PortDCB))
	{
			///Configure Serial port failed
		successful=true;
		break;
	}
	return(successful);
}
  bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts)
{
	return SetCommTimeouts(m_hSerial,lpCommTimeouts) == TRUE;
}
//socket dll.
  bool Open_Socket(CString strIPAdress)//打开Open Socket
{
	if(g_Commu_type==0)
		return false;

		int nNetTimeout=3000;//1 second.
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		if(::WSAStartup(sockVersion, &wsaData) != 0)
		{
		//	AfxMessageBox(_T("Init Socket failed!"));
			return FALSE;
		}
		if (m_hSocket!=INVALID_SOCKET)
		{
		
			::closesocket(m_hSocket);
		}
		if (m_hSocket!=NULL)
		{
			::closesocket(m_hSocket);
			m_hSocket=NULL;
		}
			
		m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(m_hSocket == INVALID_SOCKET)
		{
		//	AfxMessageBox(_T("Create socket failed!"));
			return FALSE;
		}
		sockaddr_in servAddr; 
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(6001);
		// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
          //TBD: explain this better
                         //Note that the IP address of the machine where the server program (TCPServer) is to be filled in
		//servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
		servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
	//	u_long ul=1;
	//	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
		//发送时限
		setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
		//Receive time limit
		setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
		if(::connect(m_hSocket,(sockaddr*)&servAddr, sizeof(servAddr)) == -1)
		{
		//	AfxMessageBox(_T(" Failed connect() \n"));
			::closesocket(m_hSocket);
			m_hSocket=NULL;
			return FALSE;
		}
		return TRUE;
}

//socket dll.
  bool Open_Socket2(CString strIPAdress,short nPort)
{
	if(g_Commu_type==0)
		return false;

	int nNetTimeout=3000;//1 second.
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	
	if (m_hSocket!=INVALID_SOCKET)
	{
		::closesocket(m_hSocket);
		m_hSocket=NULL;
	}

	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		//AfxMessageBox(_T("Init Socket failed!"));
		m_hSocket=NULL;
		return FALSE;
	}

	m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_hSocket == INVALID_SOCKET)
	{
	//	AfxMessageBox(_T("Create socket failed!"));
		m_hSocket=NULL;
		return FALSE;
	}
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(nPort);
	USES_CONVERSION;   
	//char pTemp[20];
	//pTemp=W2A(strIPAdress);     
    

	//servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
//	servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
		servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
	//	u_long ul=1;
	//	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
	// Send time
	setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
	//Receive time limit
	setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
	if(::connect(m_hSocket,(sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		DWORD dwErr = WSAGetLastError();
		//AfxMessageBox(_T(" Failed connect() \n"));
		::closesocket(m_hSocket);
		m_hSocket=NULL;
		return FALSE;
	}
	return TRUE;
}


  bool is_connect()
{
	if (g_Commu_type==0)
	{
		if(m_hSerial==NULL)
			return FALSE;
		else
			return TRUE;
	}
	if (g_Commu_type==1)
	{
		if(m_hSocket==NULL)
			return false;//disconnected
		else
			return true;//connected
	}

	return false;
}

//device_var:product ID,address:a register;
  int Read_One(TS_UC device_var,TS_US address)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
	int nTemp=-1;
	if(g_Commu_type==0)//serial
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//Empty serial port buffer
		//TS_UC  gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;// Number of bytes sent
		pval[0] = device_var;
		pval[1] = 3;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		pval[4] = 0;//(val>>8) & 0xff;//number hi
		pval[5] = 1;//val & 0xff;//number lo
		crc = CRC16(pval,6);
		pval[6] = (crc >>8) & 0xff;
		pval[7] = crc & 0xff;
		if(m_hSerial==NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;

		int fState=WriteFile(m_hSerial,// Handle
			pval,// Data buffer address
			8,// Data size
			&m_had_send_data_number,//Returns the number of bytes sent
			&m_osWrite);
		if(!fState)// Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
				//			if(GetLastError()==ERROR_IO_PENDING)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}
		//////////////////////////////////////////the message had send ,now to read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;

		////////////////////////////////////////////////clear com error
		Sleep(LATENCY_TIME_COM);
		if(address==10)
		{
			serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal			
			fState=ReadFile(m_hSerial,//Handle
				gval,//Data buffer Address
				11,// Data size
				&m_had_send_data_number,//Returns the number of bytes sent
				&m_osRead);
		}
		else
		{
			fState=ReadFile(m_hSerial,// 句柄 , Handle
				gval,//Data buffer Address
				7,//Data size
				&m_had_send_data_number,//returns the number of bytes sent
				&m_osRead);
		}
		if(!fState)//Overlap not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}
		if(address!=10)
		{//old protocal
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 )
				return -3;
			if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
				return -2;
			crc=CRC16(gval,5);
			if(gval[5]!=((crc>>8)&0xff))
				return -2;
			if(gval[6]!=(crc & 0xff))
				return -2;	
		}
		else
		{
			if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
			{//new protocal
				if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
					return -2;
				crc=CRC16(gval,9);
				if(gval[9]!=((crc>>8)&0xff))
					return -2;
				if(gval[10]!=(crc & 0xff))
					return -2;		
				serinumber_in_dll[0]=gval[5];
				serinumber_in_dll[1]=gval[6];
				serinumber_in_dll[2]=gval[7];
				serinumber_in_dll[3]=gval[8];//stay serialnumber	
				//			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
			}
			else
			{//old protocal
				if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )//2
					return -2;
				crc=CRC16(gval,5);
				if(gval[5]!=((crc>>8)&0xff))
					return -2;
				if(gval[6]!=(crc & 0xff))
					return -2;	
			}
		}
		nTemp=gval[3];
		if(nTemp==255)
			nTemp=-1;
		return (nTemp*256+gval[4]);
	}
	if(g_Commu_type==1)//tcp.
	{  
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,not connecting
		//return value == -3 ,no response
		//Empty the serial port buffer
		//TS_UC  gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];

		/*
		struct DataPack
		{
			TS_UC  empty[6];
			TS_UC  pval[13];

		};
		*/

		TS_UC data[12];

		data[0]=1;
		data[1]=2;
		data[2]=3;
		data[3]=4;
		data[4]=5;
		data[5]=6;
		data[6]=device_var;
		data[7]=3;
		data[8]=address>>8 & 0xFF ;
		data[9] = address & 0xFF;
		data[10]=0;
		data[11]=1;

//		DataPack dataInfo;
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
//		TS_US crc;		
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent

		pval[0] = device_var;
		pval[1] = 3;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		pval[4] = 0;//(val>>8) & 0xff;//number hi
		pval[5] = 1;//val & 0xff;//number lo
	//	crc = CRC16(pval,6);
	//	pval[6] = (crc >>8) & 0xff;
	//	pval[7] = crc & 0xff;

		//memcpy((char*)&data[6],(char*)&pval[0],sizeof(pval));

		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}

		int nLen = sizeof(data);
		//int xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		int xx=::send(m_hSocket,(char*)&data,sizeof(data),0);


		////////////////////////////////////////////////clear com error
		if(address==10)
		{
			serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
		}

		Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
		//DataPack rvData;
		//Sleep(SLEEP_TIME+10);

		TS_UC rvData[17];
		//int nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);
		int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
		//gval=rvData.pval;
		memcpy((void*)gval,(void*)&rvData[6],13/*sizeof(gval)*/);
		
		/*
		xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);*/

		int nerro= WSAGetLastError();

		if(address!=10)
		{//old protocal
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0/* && gval[5]==0 && gval[6]==0*/ )
				return -3;
			if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
				return -2;
			
			/*;
			crc=CRC16(gval,5);
			if(gval[5]!=((crc>>8)&0xff))
				return -2;
			if(gval[6]!=(crc & 0xff))
				return -2;	
			*/

		}
		else
		{
			if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
			{//new protocal
				if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
					return -2;
				/*
				crc=CRC16(gval,9);
				if(gval[9]!=((crc>>8)&0xff))
					return -2;
				if(gval[10]!=(crc & 0xff))
					return -2;	
					*/
				serinumber_in_dll[0]=gval[5];
				serinumber_in_dll[1]=gval[6];
				serinumber_in_dll[2]=gval[7];
				serinumber_in_dll[3]=gval[8];//stay serialnumber	
				//			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
			}
			else
			{//old protocal
				if(gval[0]!=pval[0] || gval[1]!=gval[1] || gval[2]!=2 )//2
					return -2;
				/*
				crc=CRC16(gval,5);
				if(gval[5]!=((crc>>8)&0xff))
					return -2;
				if(gval[6]!=(crc & 0xff))
					return -2;	
					*/
			}
		}
		nTemp=gval[3];
		if(nTemp==255)
			nTemp=-1;
		return (gval[3]*256+gval[4]);
	}
//	singlock.Unlock();

	return 0;
}

  int Write_One(TS_UC device_var,TS_US address,TS_US val)
{
	if (g_Commu_type==0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		pval[0] = device_var;
		pval[1] = 6;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			pval[4]=0x55;/////////////////////////////new protocal or write_one 10
		else
			pval[4] = (val>>8) & 0xff;//number hi
		pval[5] = val & 0xff;//number lo
		if(address!=10)
		{
			crc = CRC16(pval,6);
			pval[6] = (crc >>8) & 0xff;
			pval[7] = crc & 0xff;
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{
				crc = CRC16(pval,6);
				pval[6] = (crc >>8) & 0xff;
				pval[7] = crc & 0xff;				
			}
			else
			{
				pval[6]=serinumber_in_dll[0];
				pval[7]=serinumber_in_dll[1];
				pval[8]=serinumber_in_dll[2];
				pval[9]=serinumber_in_dll[3];
				crc = CRC16(pval,10);
				pval[10] = (crc >>8) & 0xff;
				pval[11] = crc & 0xff;				
			}
		}

		if(m_hSerial==NULL)
		{			
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		if(address!=10)
		{
			fState=WriteFile(m_hSerial,// 句柄, Handle
				pval,//Data buffer address
				8,//Data size
				&m_had_send_data_number,//Returns the number of bytes sent
				&m_osWrite);
		}
		else
		{//==10 register
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=WriteFile(m_hSerial,// 句柄, Handle
					pval,//Databuffer address
					8,//Data size
					&m_had_send_data_number,//Bytes sent
					&m_osWrite);
			}
			else
			{//new protocol
				fState=WriteFile(m_hSerial,// 句柄 , handle
					pval,//Data buffer Address
					12,//Data size
					&m_had_send_data_number,//Returns # bytes sent 
					&m_osWrite);
			}
		}
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
				//			if(GetLastError()==ERROR_IO_INCOMPLETE)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		Sleep(LATENCY_TIME_COM);
		if(address!=10)
		{
			fState=ReadFile(m_hSerial,// 句柄, Handle
				gval,//Data buffer address
				8,//Data size
				&m_had_send_data_number,//Returns the number of bytes sent
				&m_osRead);
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=ReadFile(m_hSerial,		//Handle
					gval,					//Data buffer address
					8,					//Data size
					&m_had_send_data_number, 	//Returns # of bytes sent
					&m_osRead);
			}
			else
			{//new protocal
				fState=ReadFile(m_hSerial,		//Handle
					gval,					//Data buffer address
					12,					//Data size
					&m_had_send_data_number,	//Returns # of bytes sent 
					&m_osRead);
			}
		}
		if(!fState)			//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}		
		if(address!=10)
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
				return -3;
			for(int i=0;i<8;i++)
				if(gval[i]!=pval[i])
					return -2;
		}
		else
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
				return -3;
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocol
				for(int i=0;i<8;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
			else
			{//new protocol
				//			crc=CRC16(gval,10);
				//			if(gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(gval[7]!=(crc & 0xff))
				//				return -2;	
				//			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
				if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0)
					return -3;
				for(int i=0;i<12;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
		}
		return 1;
	}
	
	if(g_Commu_type==1)//tcp.
	{
		//address of the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -3 , no response
		//Empty the serial port buffer
		
		//TS_UC data[12];
		TS_UC data[16];
// 		TS_UC* data=NULL;
 		int nSendNum = 12;
		if (address==10)
		{
			//data = new TS_UC[16];
			nSendNum = 16;
		}
		else
		{
			//data = new TS_UC[12];
			nSendNum = 12;
		}
		ZeroMemory(data, nSendNum);

		data[0]=1;
		data[1]=2;
		data[2]=3;
		data[3]=4;
		data[4]=5;
		data[5]=6;
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, # of bytes sent
		data[6] = device_var;
		data[7] = 6;
		data[8] = address>>8 & 0xFF ;
		data[9] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			data[10]=0x55;/////////////////////////////new protocal or write_one 10
		else
			data[10] = (val>>8) & 0xff;//number hi
		data[11] = val & 0xff;//number lo

		if(address==10)
		{

			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{			
			}
			else
			{
				//* uncommend by zgq; we use net work scan tstat through NC
				data[12]=serinumber_in_dll[0];
				data[13]=serinumber_in_dll[1];
				data[14]=serinumber_in_dll[2];
				data[15]=serinumber_in_dll[3];	
				//*/
			}
		}


		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
//		TS_US crc;		
	//	DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent
		pval[0] = device_var;
		pval[1] = 6;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			pval[4]=0x55;/////////////////////////////new protocol or write_one 10
		else
			pval[4] = (val>>8) & 0xff;//number hi
		pval[5] = val & 0xff;//number lo
	
		/*
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		pval[0] = device_var;
		pval[1] = 6;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			pval[4]=0x55;/////////////////////////////new protocol or write_one 10
		else
			pval[4] = (val>>8) & 0xff;//number hi
		pval[5] = val & 0xff;//number lo
		if(address!=10)
		{
			crc = CRC16(pval,6);
			pval[6] = (crc >>8) & 0xff;
			pval[7] = crc & 0xff;
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{
				crc = CRC16(pval,6);
				pval[6] = (crc >>8) & 0xff;
				pval[7] = crc & 0xff;				
			}
			else
			{
				pval[6]=serinumber_in_dll[0];
				pval[7]=serinumber_in_dll[1];
				pval[8]=serinumber_in_dll[2];
				pval[9]=serinumber_in_dll[3];
				crc = CRC16(pval,10);
				pval[10] = (crc >>8) & 0xff;
				pval[11] = crc & 0xff;				
			}
		}
		*/
		if(m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}

		//::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);

		//Sleep(SLEEP_TIME+10);
		Sleep(LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		int nErr = WSAGetLastError();

		memcpy((void*)&gval[0],(void*)&rvdata[6],13/*sizeof(gval)*/);
		if(address!=10)
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0*/)
				return -3;
			for(int i=0;i<5;i++)
				if(gval[i]!=pval[i])
					return -2;
		}
		else
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0/* && gval[6]==0 && gval[7]==0*/)
				return -3;
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				for(int i=0;i<5;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
			else
			{//new protocol
				//			crc=CRC16(gval,10);
				//			if(gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(gval[7]!=(crc & 0xff))
				//				return -2;	
				//			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
				if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
					return -3;
				for(int i=0;i<5;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
		}

		//delete []data;
		return 1;
	}

	return 0;
	///////////////////////////////////////////////////////////
}

  int read_multi(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length)
{
	if(g_Commu_type==0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connection

		TS_UC to_send_data[300]={'\0'};
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//send_data is the array to store the register data
		//length is the number of registers that will be read
		//start_address is the start register
		TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
		data_to_send[0]=device_var;//slave address
		data_to_send[1]=3;//function multiple
		data_to_send[2]=start_address>>8 & 0xff;//starting address hi
		data_to_send[3]=start_address & 0xff;//starting address lo
		data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5]=length & 0xff;//quantity of registers lo
		TS_US crc=CRC16(data_to_send,6);
		data_to_send[6]=(crc>>8) & 0xff;
		data_to_send[7]=crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		if(m_hSerial==NULL)
		{		
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false,_T("MulWrite")))==NULL)
			return -2; 
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0 ;
		///////////////////////////////////////////////////////send the read message
		int fState=WriteFile(m_hSerial,// 句柄, Handle
			data_to_send,//Data buffer address
			8,//Data size
			&m_had_send_data_number,//Returns the # of bytes sent
			&m_osMulWrite);
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(LATENCY_TIME_COM);
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// 句柄, Handle
			to_send_data,//Buffer Addres
			length*2+5,//Data size
			&m_had_send_data_number,//Returns the number of bytes sent
			&m_osRead);
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}	
		///////////////////////////////////////////////////////////
		if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
			return -2;
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
			return -2;
		if(to_send_data[length*2+4]!=(crc & 0xff))
			return -2;
		for(int i=0;i<length;i++)
			put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
		return length;
	}
	if(g_Commu_type==1)//tcp.
	{
		/*
		//device_var is the Tstat ID
		//the return value == -1 ,no connection
		TS_UC to_send_data[700]={'\0'};
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
		data_to_send[0]=device_var;//slave address
		data_to_send[1]=3;//function multiple
		data_to_send[2]=start_address>>8 & 0xff;//starting address hi
		data_to_send[3]=start_address & 0xff;//starting address lo
		data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5]=length & 0xff;//quantity of registers lo
		TS_US crc=CRC16(data_to_send,6);
		data_to_send[6]=(crc>>8) & 0xff;
		data_to_send[7]=crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent
		if(m_hSocket==INVALID_SOCKET)
		{		
			return -1;
		}
		::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

		int nn=sizeof(to_send_data);
		int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

		///////////////////////////////////////////////////////////
		if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
			return -2;
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
			return -2;
		if(to_send_data[length*2+4]!=(crc & 0xff))
			return -2;
		for(int i=0;i<length;i++)
			put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
		return length;
		*/
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[700]={'\0'};
		TS_UC to_Reive_data[700]={'\0'};
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[12]={'\0'};//the array to used in writefile()
		data_to_send[0]=1;
		data_to_send[1]=2;
		data_to_send[2]=3;
		data_to_send[3]=4;
		data_to_send[4]=5;
		data_to_send[5]=6;



		data_to_send[6]=device_var;//slave address
		data_to_send[7]=3;//function multiple
		data_to_send[8]=start_address>>8 & 0xff;//starting address hi
		data_to_send[9]=start_address & 0xff;//starting address lo
		data_to_send[10]=length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11]=length & 0xff;//quantity of registers lo

		//TS_US crc=CRC16(data_to_send,6);
		//data_to_send[6]=(crc>>8) & 0xff;
		//data_to_send[7]=crc & 0xff;/

//		DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}

		::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

		Sleep(LATENCY_TIME_NET);
		int nn=sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

		memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
			return -2;
		/*
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
			return -2;

		if(to_send_data[length*2+4]!=(crc & 0xff))
			return -2;
			*/
		for(int i=0;i<length;i++)
			put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
		return length;
	}

	return 0;
}

int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length)
{	
	if(g_Commu_type==0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connection
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600]={'\0'};
		data_to_write[0]=device_var;
		data_to_write[1]=0x10;
		data_to_write[2]=start_address >> 8 & 0xff;
		data_to_write[3]=start_address & 0xff;
		data_to_write[4]=0;
		data_to_write[5]=length;
		data_to_write[6]=length;//if you send more than 128 bytes
                                  //the command cannot finish on time -> problem 
		for(int i=0;i<length;i++)
			data_to_write[7+i]=to_write[i];
	//	TS_US crc=CRC16(data_to_write,i+7);
	//	data_to_write[i+7]=crc>>8 & 0xff;
	//	data_to_write[i+8]=crc & 0xff;

		TS_US crc=CRC16(data_to_write,length+7);
		data_to_write[length+7]=crc>>8 & 0xff;
		data_to_write[length+8]=crc & 0xff;

		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent
		if(m_hSerial==NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false,_T("MulWrite")))==NULL)
			return -2; 
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0 ;
		///////////////////////////////////////////////////////send the to read message
		int fState=WriteFile(m_hSerial,// 句柄, Handle
			data_to_write,//Data buffer address
			length+9,//Data size
			&m_had_send_data_number,//Returns the number of bytes sent
			&m_osMulWrite);
		if(!fState)//Doesn’t support overflow. 
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, wait
			}
			else
				m_had_send_data_number=0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;
		Sleep(LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// 句柄, handle
			gval,//Address of buffer
			8,//Data size
			&m_had_send_data_number,//Number of bytes sent
			&m_osRead);
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, wait
			}
			else
				m_had_send_data_number=0;
		}	
		///////////////////////////////////////////////////////////
		for(int i=0;i<6;i++)
			if(gval[i]!=*(data_to_write+i))
				return -2;
		crc=CRC16(gval,6);
		if(gval[6]!=((crc>>8)&0xff))
			return -2;
		if(gval[7]!=(crc & 0xff))
			return -2;
		return 1;
	}
	if(g_Commu_type==1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600]={'\0'};
		TS_UC data_back_write[600]={'\0'};
		
		data_to_write[0]=1;
		data_to_write[1]=2;
		data_to_write[2]=3;
		data_to_write[3]=4;
		data_to_write[4]=5;
		data_to_write[5]=6;

		data_to_write[6]=device_var;
		data_to_write[7]=0x10;
		data_to_write[8]=start_address >> 8 & 0xff;
		data_to_write[9]=start_address & 0xff;
		data_to_write[10]=0;
		data_to_write[11]=length;
		data_to_write[12]=length;//128 is max, longer and the routine cannot finish in time. 
		for(int i=0;i<length;i++)
			data_to_write[13+i]=to_write[i];
		
			
	//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
	//	TS_US crc=CRC16(data_to_write,i+7);
	//	data_to_write[i+7]=crc>>8 & 0xff;
	//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length, for 128 bytes, the length == 128
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, # of bytes sent
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}
	
		int n=::send(m_hSocket,(char *)data_to_write,13+length,0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);
		if(nRecv<0)
		{
			return -2;
		}
	//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for(int i=0;i<6;i++)
			if(data_back_write[i+6]!=*(data_to_write+i+6))
				return -2;
		return 1;

	}

	return 0;
}

 
  int NetController_CheckTstatOnline(TS_UC devLo,TS_UC devHi)
{
	if(g_Commu_type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try it again
		//the return value == -3, May have more than 2 Devices connecting at this address
		//the return value == -4 ,between devLo and devHi,no Device is connected in this range
		//the return value == -5 ,the receive buffer had some trouble TBD: explain this
		//the return value >=1 ,the devLo!=devHi, may have 2 devices is connecting at this address
		//empty the serial port buffer
		//the return value is the register address

		//Sleep(150);       //must use this function to slow computer
		if(devLo<1 || devHi>254)
			return -5;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0) 
			{
				the_return_value2=NetController_CheckTstatOnline2(devLo,devHi);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocal
			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
				return the_return_value;
			int i=0;
			do{
				the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
					return the_return_value;
				else if(the_return_value>0)
					i++;
			}while(i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if(g_Commu_type==1)//tcp
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try again
		//the return value == -3,May have more than 2 devices is connecting at this address
		//the return value == -4 ,between devLo and devHi, no device is detected in this range
		//the return value == -5 ,the receive buffer may have some trouble
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if(devLo<1 || devHi>254)
			return -5;
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0)
			{
				the_return_value2=NetController_CheckTstatOnline2(devLo,devHi);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocal
			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
				return the_return_value;
			int i=0;
			do{
				the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
					return the_return_value;
				else if(the_return_value>0)
					i++;
			}while(i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	return -1;
}




  int NetController_CheckTstatOnline2(TS_UC devLo,TS_UC devHi)
{
	if(g_Commu_type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try it again
		//the return value == -3, may have more than 2 devices connecting
		//the return value == -4 , between devLo and devHi,no device detected
		//the return value == -5 ,the input buffer may have some problem
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//清空串口缓冲区
		//the return value is the register address

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数, # of Bytes sent
		pval[0] = 255;											
		pval[1] = 26;  //put comments here,
		pval[2] = devHi;
		pval[3] = devLo;
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;

		if(m_hSerial==NULL)
		{			
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
		int fState=WriteFile(m_hSerial,// 句柄, Handle
			pval,//Data buffer address
			6,//Data size
			&m_had_send_data_number,//Returns # of bytes sent
			&m_osWrite);
		if(!fState)//不支持重叠, Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, Wait
				//			if(GetLastError()==ERROR_IO_INCOMPLETE)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}
		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(LATENCY_TIME_COM);//SLEEP_TIME);//because that scan have a delay lower 75ms
	//	Sleep(300);
		/////////////**************down is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// 句柄, Handle
			gval,//Buffer address
			13,//size
			&m_had_send_data_number,//bytese sent
			&m_osRead);
		if(!fState)		//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, Wait
			}
			else
				m_had_send_data_number=0;
		}
		//CloseHandle(m_osRead.hEvent);
		/*
		CStdioFile default_file;
		CString saved_path="C:\\modbus_scan_data.txt";
		CString a_line;
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("T:%x %x %x %x %x %x",pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		default_file.WriteString(a_line+"\n");
		a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}
		*/
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocal
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME);//Delay required here
				return -4;              //no response ,no connection
			}
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME);// Delay required here
				return -3;
			}
			if((gval[0]!=pval[0]) || (gval[1]!=26))
			{
				Sleep(SLEEP_TIME);// Delay required here 
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);// Delay required here 
				return -2;
			}
		}
		else
		{// new scan protocol,if many old devices, get into here and scan result is OK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);//Need a delay here
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
				return -3;
			if((gval[0]!=pval[0]) || (gval[1]!=25))
				return -2;
			crc=CRC16(gval,7);
			if( gval[7]!=((crc>>8) & 0xff) )
				return -2;
			if(gval[8]!=(crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		return gval[2];
	}

	if(g_Commu_type==1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try again
		//the return value == -3, maybe have more than 2 devices connecting
		//the return value == -4 , between devLo and devHi,no device is detected
		//the return value == -5 ,the buffer has some problem
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect

		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent

		
		pval[0]=1;
		pval[1]=2;
		pval[2]=3;
		pval[3]=4;
		pval[4]=5;
		pval[5]=6;
		pval[6] = 255;
		pval[7] = 26;
		pval[8] = devHi;
		pval[9] = devLo;
		/*
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;
*/
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		::send(m_hSocket,(char*)pval,sizeof(pval),0);
		Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME+8);

		TS_UC  rvdata[19];
		for(int i=0;i<19;i++)
			rvdata[i]=0;
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
		if (nRecv>0)
		{
			//异常:
			memcpy(gval,(void*)&rvdata[6],sizeof(rvdata)-6);
		}
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocal
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
					Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
					Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if((gval[0]!=255) || (gval[1]!=26))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{// new scan protocol,if many old tstat ,get into here ,scan result is oK too.
			old_or_new_scan_protocal_in_dll=1;
			//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
				return -3;
			if((gval[0]!=255) || (gval[1]!=26))
				return -2;
		}

		/*
		old_or_new_scan_protocal_in_dll=1;

		if((gval[0]!=pval[6]) || (gval[1]!=26))
			return -2;
		if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0)
		{
			//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			return -4;              //no response ,no connection
		}
		*/
		return gval[2];
	}
	return 0;
}

///also return the serial communication handle;
  HANDLE GetCommunicationHandle()
{
	
	/*
	if(g_Commu_type==1&&m_hSocket!=NULL)
		return m_hSocket;
	else 
		return NULL;
		*/
	if(g_Commu_type==1)
		return (HANDLE)m_hSocket;
	if(g_Commu_type==0) 
		return m_hSerial;

	return 0;
}
  BOOL bTCPDisconnected()
{
	return TRUE;
}
  void SetComnicationHandle(int nType,HANDLE hCommunication)
{
	close_com();
	if(g_Commu_type==0)
	{

	}
	if(g_Commu_type==1)
	{
		m_hSocket=(SOCKET)hCommunication;
	}

}


  bool open_com(int m_com)
{	
	//open com , for com1 use m_com = 0, for "com2", m_com = 1 and so on. 
	//you will get the handle to com,m_hSerial,is an extern variable
	//the return value ,true is ok,false is failure
	if(open_com_port_number_in_dll==m_com)
	{
		Change_BaudRate(19200);
		return true;///////////////////////////Don’t open the same port multiple times.
	}
	if(m_hSerial != NULL)  
	{//关闭串口, Close the serial port
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	/////////////////////////////////////////////////////////////////////加入的串口通信部分, Joined serial communications part
// 	LPCSTR lpComNum[6];
// 	ZeroMemory(lpComNum, )

	CString strCom;
	if (m_com < 9 && m_com >0)
	{
		strCom.Format(_T("%d"),m_com);
		strCom = _T("COM")+strCom+_T(":");
	}
	else if(m_com > 9)
	{
		strCom.Format(_T("%d"),m_com);
		strCom = _T("\\\\.\\COM")+strCom;
	}
	
	m_hSerial = CreateFile( strCom, //strCom,//串口句柄，打开串口, Serial Handle, open serial port
								GENERIC_READ | GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
// Different form, the expression is asynchronous communication, R/W at the same time, 0 for synchronous Read and write TBD: Explain this a little better please. 

								NULL);


	if(m_hSerial == INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;		
		return false;
	}
	if(!SetupComm(m_hSerial, 1024*32, 1024*9))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	DCB  PortDCB;    
	PortDCB.DCBlength = sizeof(DCB); 
	// 默认串口参数, Default serial port paramters
	if(!GetCommState(m_hSerial, &PortDCB))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
		return false;
	}	
	//don’t change the baudate
	PortDCB.BaudRate = 19200; // baud//If it’s the wrong baud rate you can’t write
	PortDCB.ByteSize = 8;     // Number of bits/byte, 4-8 
	PortDCB.Parity = NOPARITY; 
	PortDCB.StopBits = ONESTOPBIT;  
	if (! SetCommState(m_hSerial, &PortDCB))
	{
		return false;
	}
	COMMTIMEOUTS CommTimeouts;
	GetCommTimeouts(m_hSerial, &CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = 160;  
	CommTimeouts.ReadTotalTimeoutMultiplier = 20;  
	CommTimeouts.ReadTotalTimeoutConstant = 360;    
	CommTimeouts.WriteTotalTimeoutMultiplier = 20;  
	CommTimeouts.WriteTotalTimeoutConstant = 200;    
	if (!SetCommTimeouts(m_hSerial, &CommTimeouts))
	{
		return false;
	}          
	open_com_port_number_in_dll=m_com;
	baudrate_in_dll=19200;

	return true;
}



  int Read_One2(TS_UC device_var,TS_US address, bool bComm_Type)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
	int nTemp=-1;
	if(bComm_Type==0)//serial
	{
		//address        the register
		//the return value ,-2 bad comms
		//the return value == -1 ,no connection
		//return value == -3 ,no response
		//Empty the serial port buffer
		//TS_UC  gval[8]={'\0'};//the data which was read
		//      TS_UC  pval[9];
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		pval[0] = device_var;
		pval[1] = 3;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		pval[4] = 0;//(val>>8) & 0xff;//number hi
		pval[5] = 1;//val & 0xff;//number lo
		crc = CRC16(pval,6);
		pval[6] = (crc >>8) & 0xff;
		pval[7] = crc & 0xff;
		if(m_hSerial==NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;

		int fState=WriteFile(m_hSerial,// 句柄, Handle
			pval,//Address of buffer
			8,//length
			&m_had_send_data_number,//returns # of bytes sent
			&m_osWrite);
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, wait
				//			if(GetLastError()==ERROR_IO_PENDING)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}
		//////////////////////////////////////////the message had send ,now to read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;

		////////////////////////////////////////////////clear com error
		Sleep(LATENCY_TIME_COM);
		if(address==10)
		{
			serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal			
			fState=ReadFile(m_hSerial,// 句柄, Handle
				gval,// 数据缓冲区地址 , Adderss 
				11,//length
				&m_had_send_data_number,//num of bytes sent
				&m_osRead);
		}
		else
		{
			fState=ReadFile(m_hSerial,// 句柄 , Handle
				gval,//Address of buffer
				7,//length
				&m_had_send_data_number,//Num of bytes sent. 
				&m_osRead);
		}
		if(!fState)//Oveerflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}
		if(address!=10)
		{//old protocal
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 )
				return -3;
			if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
				return -2;
			crc=CRC16(gval,5);
			if(gval[5]!=((crc>>8)&0xff))
				return -2;
			if(gval[6]!=(crc & 0xff))
				return -2;	
		}
		else
		{
			if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
			{//new protocol
				if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
					return -2;
				crc=CRC16(gval,9);
				if(gval[9]!=((crc>>8)&0xff))
					return -2;
				if(gval[10]!=(crc & 0xff))
					return -2;		
				serinumber_in_dll[0]=gval[5];
				serinumber_in_dll[1]=gval[6];
				serinumber_in_dll[2]=gval[7];
				serinumber_in_dll[3]=gval[8];//stay serialnumber	
				//			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
			}
			else
			{//old protocal
				if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )//2
					return -2;
				crc=CRC16(gval,5);
				if(gval[5]!=((crc>>8)&0xff))
					return -2;
				if(gval[6]!=(crc & 0xff))
					return -2;	
			}
		}
		nTemp=gval[3];
		if(nTemp==255)
			nTemp=-1;
		return (nTemp*256+gval[4]);
	}
	if(bComm_Type==1)//tcp.
	{  
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connection
		//return value == -3 ,no response
		//Empty the serial port buffer
		//TS_UC  gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];

		/*
		struct DataPack
		{
			TS_UC  empty[6];
			TS_UC  pval[13];

		};
		*/

		TS_UC data[12];

		data[0]=1;
		data[1]=2;
		data[2]=3;
		data[3]=4;
		data[4]=5;
		data[5]=6;
		data[6]=device_var;
		data[7]=3;
		data[8]=address>>8 & 0xFF ;
		data[9] = address & 0xFF;
		data[10]=0;
		data[11]=1;

//		DataPack dataInfo;
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
//		TS_US crc;		
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, # of bytes sent

		pval[0] = device_var;
		pval[1] = 3;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		pval[4] = 0;//(val>>8) & 0xff;//number hi
		pval[5] = 1;//val & 0xff;//number lo
	//	crc = CRC16(pval,6);
	//	pval[6] = (crc >>8) & 0xff;
	//	pval[7] = crc & 0xff;

		//memcpy((char*)&data[6],(char*)&pval[0],sizeof(pval));

		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}

		int nLen = sizeof(data);
		//int xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		int xx=::send(m_hSocket,(char*)&data,sizeof(data),0);


		////////////////////////////////////////////////clear com error
		if(address==10)
		{
			serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
		}

		Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
		//DataPack rvData;
		//Sleep(SLEEP_TIME+10);

		TS_UC rvData[17];
		//int nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);
		int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
		//gval=rvData.pval;
		memcpy((void*)gval,(void*)&rvData[6],13/*sizeof(gval)*/);
		
		/*
		xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);*/

		int nerro= WSAGetLastError();

		if(address!=10)
		{//old protocal
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0/* && gval[5]==0 && gval[6]==0*/ )
				return -3;
			if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
				return -2;
			
			/*;
			crc=CRC16(gval,5);
			if(gval[5]!=((crc>>8)&0xff))
				return -2;
			if(gval[6]!=(crc & 0xff))
				return -2;	
			*/

		}
		else
		{
			if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
			{//new protocal
				if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
					return -2;
				/*
				crc=CRC16(gval,9);
				if(gval[9]!=((crc>>8)&0xff))
					return -2;
				if(gval[10]!=(crc & 0xff))
					return -2;	
					*/
				serinumber_in_dll[0]=gval[5];
				serinumber_in_dll[1]=gval[6];
				serinumber_in_dll[2]=gval[7];
				serinumber_in_dll[3]=gval[8];//stay serialnumber	
				//			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
			}
			else
			{//old protocal
				if(gval[0]!=pval[0] || gval[1]!=gval[1] || gval[2]!=2 )//2
					return -2;
				/*
				crc=CRC16(gval,5);
				if(gval[5]!=((crc>>8)&0xff))
					return -2;
				if(gval[6]!=(crc & 0xff))
					return -2;	
					*/
			}
		}
		nTemp=gval[3];
		if(nTemp==255)
			nTemp=-1;
		return (gval[3]*256+gval[4]);
	}
//	singlock.Unlock();
	return 0;
}


  int Write_One2(TS_UC device_var,TS_US address,TS_US val, bool bComm_Type)
{
	if (bComm_Type==0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		pval[0] = device_var;
		pval[1] = 6;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			pval[4]=0x55;/////////////////////////////new protocal or write_one 10
		else
			pval[4] = (val>>8) & 0xff;//number hi
		pval[5] = val & 0xff;//number lo
		if(address!=10)
		{
			crc = CRC16(pval,6);
			pval[6] = (crc >>8) & 0xff;
			pval[7] = crc & 0xff;
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{
				crc = CRC16(pval,6);
				pval[6] = (crc >>8) & 0xff;
				pval[7] = crc & 0xff;				
			}
			else
			{
				pval[6]=serinumber_in_dll[0];
				pval[7]=serinumber_in_dll[1];
				pval[8]=serinumber_in_dll[2];
				pval[9]=serinumber_in_dll[3];
				crc = CRC16(pval,10);
				pval[10] = (crc >>8) & 0xff;
				pval[11] = crc & 0xff;				
			}
		}

		if(m_hSerial==NULL)
		{			
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		if(address!=10)
		{
			fState=WriteFile(m_hSerial,// 句柄, Handle
				pval,//Address of buffer
				8,//length
				&m_had_send_data_number,//return number of bytes sent
				&m_osWrite);
		}
		else
		{//==10 register
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=WriteFile(m_hSerial,// 句柄 handle
					pval,//address
					8,//len
					&m_had_send_data_number,// 返回发送出去的字节数 , bytes sent
					&m_osWrite);
			}
			else
			{//new protocol
				fState=WriteFile(m_hSerial,// 句柄, handle
					pval,//address
					12,//len
					&m_had_send_data_number,//returns the number of bytes sent
					&m_osWrite);
			}
		}
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, wait
				//			if(GetLastError()==ERROR_IO_INCOMPLETE)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		Sleep(LATENCY_TIME_COM);
		if(address!=10)
		{
			fState=ReadFile(m_hSerial,// 句柄, handle
				gval,//buffer address
				8,//len
				&m_had_send_data_number,//returns the number of bytes sent
				&m_osRead);
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=ReadFile(m_hSerial,// 句柄, Handle
					gval,//address of buffer
					8,//len
					&m_had_send_data_number,//return # of bytes sent
					&m_osRead);
			}
			else
			{//new protocol
				fState=ReadFile(m_hSerial,// 句柄 , handle
					gval,//address of buffer
					12,//len
					&m_had_send_data_number,//returns # bytes sent
					&m_osRead);
			}
		}
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}		
		if(address!=10)
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
				return -3;
			for(int i=0;i<8;i++)
				if(gval[i]!=pval[i])
					return -2;
		}
		else
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
				return -3;
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				for(int i=0;i<8;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
			else
			{//new protocal
				//			crc=CRC16(gval,10);
				//			if(gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(gval[7]!=(crc & 0xff))
				//				return -2;	
				//			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
				if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0)
					return -3;
				for(int i=0;i<12;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
		}
		return 1;
	}
	
	if(bComm_Type==1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -3 , no response
		//清空串口缓冲区
		
		//TS_UC data[12];
		TS_UC data[16];
// 		TS_UC* data=NULL;
 		int nSendNum = 12;
		if (address==10)
		{
			//data = new TS_UC[16];
			nSendNum = 16;
		}
		else
		{
			//data = new TS_UC[12];
			nSendNum = 12;
		}
		ZeroMemory(data, nSendNum);

		data[0]=1;
		data[1]=2;
		data[2]=3;
		data[3]=4;
		data[4]=5;
		data[5]=6;
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, # of bytes sent
		data[6] = device_var;
		data[7] = 6;
		data[8] = address>>8 & 0xFF ;
		data[9] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			data[10]=0x55;/////////////////////////////new protocol or write_one 10
		else
			data[10] = (val>>8) & 0xff;//number hi
		data[11] = val & 0xff;//number lo

		if(address==10)
		{

			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{			
			}
			else
			{
				//* uncommented by zgq; we use net work scan devices through gateway
				data[12]=serinumber_in_dll[0];
				data[13]=serinumber_in_dll[1];
				data[14]=serinumber_in_dll[2];
				data[15]=serinumber_in_dll[3];	
				//*/
			}
		}


		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
//		TS_US crc;		
	//	DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		pval[0] = device_var;
		pval[1] = 6;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			pval[4]=0x55;/////////////////////////////new protocal or write_one 10
		else
			pval[4] = (val>>8) & 0xff;//number hi
		pval[5] = val & 0xff;//number lo
	
		/*
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		pval[0] = device_var;
		pval[1] = 6;
		pval[2] = address>>8 & 0xFF ;
		pval[3] = address & 0xFF;
		if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
			pval[4]=0x55;/////////////////////////////new protocol or write_one 10
		else
			pval[4] = (val>>8) & 0xff;//number hi
		pval[5] = val & 0xff;//number lo
		if(address!=10)
		{
			crc = CRC16(pval,6);
			pval[6] = (crc >>8) & 0xff;
			pval[7] = crc & 0xff;
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{
				crc = CRC16(pval,6);
				pval[6] = (crc >>8) & 0xff;
				pval[7] = crc & 0xff;				
			}
			else
			{
				pval[6]=serinumber_in_dll[0];
				pval[7]=serinumber_in_dll[1];
				pval[8]=serinumber_in_dll[2];
				pval[9]=serinumber_in_dll[3];
				crc = CRC16(pval,10);
				pval[10] = (crc >>8) & 0xff;
				pval[11] = crc & 0xff;				
			}
		}
		*/
		if(m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}

		//::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);

		//Sleep(SLEEP_TIME+10);
		Sleep(LATENCY_TIME_NET+100);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		int nErr = WSAGetLastError();

		memcpy((void*)&gval[0],(void*)&rvdata[6],13/*sizeof(gval)*/);
		if(address!=10)
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0*/)
				return -3;
			for(int i=0;i<5;i++)
				if(gval[i]!=pval[i])
					return -2;
		}
		else
		{
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0/* && gval[6]==0 && gval[7]==0*/)
				return -3;
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				for(int i=0;i<5;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
			else
			{//new protocal
				//			crc=CRC16(gval,10);
				//			if(gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(gval[7]!=(crc & 0xff))
				//				return -2;	
				//			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
				if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
					return -3;
				for(int i=0;i<5;i++)
					if(gval[i]!=pval[i])
						return -2;
			}
		}

		//delete []data;
		return 1;
	}

	return 0;
	///////////////////////////////////////////////////////////
}


  int NetController_CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
	if(bComm_Type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try again
		//the return value == -3, may have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device is detected
		//the return value == -5 ,the input buffer has some trouble
		//the return value >=1 ,the devLo!=devHi, maybe have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address

		//Sleep(150);       //Delay, allow time for device to switch to reply mode
		if(devLo<1 || devHi>254)
			return -5;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0) 
			{
				the_return_value2=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocol
			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
				return the_return_value;
			int i=0;
			do{
				the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
					return the_return_value;
				else if(the_return_value>0)
					i++;
			}while(i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if(bComm_Type==1)//tcp
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try it again
		//the return value == -3,May have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device is detected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if(devLo<1 || devHi>254)
			return -5;
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0)
			{
				the_return_value2=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocal
			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
				return the_return_value;
			int i=0;
			do{
				the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
					return the_return_value;
				else if(the_return_value>0)
					i++;
			}while(i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	return -1;
}




  int NetController_CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
	if(bComm_Type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,try it again
		//the return value == -3, maybe have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device is detected ,
		//the return value == -5 ,the buffer has some trouble
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数, Number of bytes sent
		pval[0] = 255;											
		pval[1] = 26;  //put comments here,
		pval[2] = devHi;
		pval[3] = devLo;
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;

		if(m_hSerial==NULL)
		{			
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
		int fState=WriteFile(m_hSerial,// 句柄 , Handle
			pval,//Buffer Address
			6,//len
			&m_had_send_data_number,//Return the number of bytes
			&m_osWrite);
		if(!fState)//不支持重叠 , Overflow not supported. 
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
				//			if(GetLastError()==ERROR_IO_INCOMPLETE)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}
		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(LATENCY_TIME_COM);//SLEEP_TIME);//Scan has a delay less than 75ms
	//	Sleep(300);
		/////////////**************down is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// 句柄, Handle
			gval,//Buffer address
			13,//len
			&m_had_send_data_number,//returns the num of bytes sent
			&m_osRead);
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, Wait
			}
			else
				m_had_send_data_number=0;
		}
		//CloseHandle(m_osRead.hEvent);
		/*
		CStdioFile default_file;
		CString saved_path="C:\\modbus_scan_data.txt";
		CString a_line;
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("T:%x %x %x %x %x %x",pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		default_file.WriteString(a_line+"\n");
		a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}
		*/
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocal
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if((gval[0]!=pval[0]) || (gval[1]!=26))
			{
				Sleep(SLEEP_TIME);//Delay required here
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//Delay required here
				return -2;
			}
		}
		else
		{// new scan protocol,if many old devices, scan result is OK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);//Delay required here
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
				return -3;
			if((gval[0]!=pval[0]) || (gval[1]!=25))
				return -2;
			crc=CRC16(gval,7);
			if( gval[7]!=((crc>>8) & 0xff) )
				return -2;
			if(gval[8]!=(crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		return gval[2];
	}

	if(bComm_Type==1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 , Retry
		//the return value == -3, May have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device detected
		//the return value == -5 ,the input buffer had trouble
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the buffer
		//the return value is the register address
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect

		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent

		
		pval[0]=1;
		pval[1]=2;
		pval[2]=3;
		pval[3]=4;
		pval[4]=5;
		pval[5]=6;
		pval[6] = 255;
		pval[7] = 26;
		pval[8] = devHi;
		pval[9] = devLo;
		/*
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;
*/
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		::send(m_hSocket,(char*)pval,sizeof(pval),0);
		Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME+8);

		TS_UC  rvdata[19];
		for(int i=0;i<19;i++)
			rvdata[i]=0;
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
		if (nRecv>0)
		{
			//异常:
			memcpy(gval,(void*)&rvdata[6],sizeof(rvdata)-6);
		}
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocol
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
					Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
					Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if((gval[0]!=255) || (gval[1]!=26))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{// new scan protocol,if many old devices scan result is oK too.
			old_or_new_scan_protocal_in_dll=1;
			//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
				return -3;
			if((gval[0]!=255) || (gval[1]!=26))
				return -2;
		}

		/*
		old_or_new_scan_protocal_in_dll=1;

		if((gval[0]!=pval[6]) || (gval[1]!=26))
			return -2;
		if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0)
		{
			//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			return -4;              //no response ,no connection
		}
		*/
		return gval[2];
	}

	return 0;
}

  int read_multi2(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length,bool bComm_Type)
{
	if(bComm_Type==0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300]={'\0'};
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array where the data to be sent is buffered
		//length is the number of register that you will read
		//start_address is the start register
		TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
		data_to_send[0]=device_var;//slave address
		data_to_send[1]=3;//function multiple
		data_to_send[2]=start_address>>8 & 0xff;//starting address hi
		data_to_send[3]=start_address & 0xff;//starting address lo
		data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5]=length & 0xff;//quantity of registers lo
		TS_US crc=CRC16(data_to_send,6);
		data_to_send[6]=(crc>>8) & 0xff;
		data_to_send[7]=crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		if(m_hSerial==NULL)
		{		
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false,_T("MulWrite")))==NULL)
			return -2; 
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0 ;
		///////////////////////////////////////////////////////send the to read message
		int fState=WriteFile(m_hSerial,// 句柄, Handle
			data_to_send,//Address of buffer
			8,//len
			&m_had_send_data_number,//Returns num of bytes sent. 
			&m_osMulWrite);
		if(!fState)//Overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, wait
			}
			else
				m_had_send_data_number=0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(LATENCY_TIME_COM);
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// 句柄, handle
			to_send_data,//buffer address
			length*2+5,//len
			&m_had_send_data_number,//Number of bytes sent
			&m_osRead);
		if(!fState)//overflow not supported
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待, wait
			}
			else
				m_had_send_data_number=0;
		}	
		///////////////////////////////////////////////////////////
		if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
			return -2;
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
			return -2;
		if(to_send_data[length*2+4]!=(crc & 0xff))
			return -2;
		for(int i=0;i<length;i++)
			put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
		return length;
	}
	if(bComm_Type==1)//tcp.
	{
		/*
		//device_var is the Tstat ID
		//the return value == -1 ,no connection
		TS_UC to_send_data[700]={'\0'};
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array where the send data is buffered. 
		//length is the number of registers that will be read 
		//start_address is the start register
		TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
		data_to_send[0]=device_var;//slave address
		data_to_send[1]=3;//function multiple
		data_to_send[2]=start_address>>8 & 0xff;//starting address hi
		data_to_send[3]=start_address & 0xff;//starting address lo
		data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5]=length & 0xff;//quantity of registers lo
		TS_US crc=CRC16(data_to_send,6);
		data_to_send[6]=(crc>>8) & 0xff;
		data_to_send[7]=crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数, Num of bytes sent
		if(m_hSocket==INVALID_SOCKET)
		{		
			return -1;
		}
		::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

		int nn=sizeof(to_send_data);
		int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

		///////////////////////////////////////////////////////////
		if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
			return -2;
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
			return -2;
		if(to_send_data[length*2+4]!=(crc & 0xff))
			return -2;
		for(int i=0;i<length;i++)
			put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
		return length;
		*/
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[700]={'\0'};
		TS_UC to_Reive_data[700]={'\0'};
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of registers that will be read
		//start_address is the start register
		TS_UC data_to_send[12]={'\0'};//the array to used in writefile()
		data_to_send[0]=1;
		data_to_send[1]=2;
		data_to_send[2]=3;
		data_to_send[3]=4;
		data_to_send[4]=5;
		data_to_send[5]=6;



		data_to_send[6]=device_var;//slave address
		data_to_send[7]=3;//function multiple
		data_to_send[8]=start_address>>8 & 0xff;//starting address hi
		data_to_send[9]=start_address & 0xff;//starting address lo
		data_to_send[10]=length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11]=length & 0xff;//quantity of registers lo

		//TS_US crc=CRC16(data_to_send,6);
		//data_to_send[6]=(crc>>8) & 0xff;
		//data_to_send[7]=crc & 0xff;/

//		DWORD m_had_send_data_number;//已经发送的数据的字节数, # of bytes sent
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}

		::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

		Sleep(LATENCY_TIME_NET);
		int nn=sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

		memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
			return -2;
		/*
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
			return -2;

		if(to_send_data[length*2+4]!=(crc & 0xff))
			return -2;
			*/
		for(int i=0;i<length;i++)
			put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
		return length;
	}

	return 0;
}


  int CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
	if(bComm_Type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 , retry
		//the return value == -3, May have more than 2 devices connecting
		//the return value == -4 ,between devLo and devHi,no device is detected,
		//the return value == -5 ,the input buffer has some sort of trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 devices connecting
		// Empty the serial port
		//the return value is the register address
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;// Number of bytes sent
		pval[0] = 255;											
		pval[1] = 25;  //put comments here,
		pval[2] = devHi;
		pval[3] = devLo;
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0 ;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
		int fState=WriteFile(m_hSerial,// Handle
			pval,// Data Buffer Address
			6,// Data size
			&m_had_send_data_number,// Returns the number of bytes sent
			&m_osWrite);
		if(!fState)// Overlflow Not Supported	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// Wait
				//			if(GetLastError()==ERROR_IO_INCOMPLETE)
				//				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number=0;
		}

		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(LATENCY_TIME_COM);//because that scan have a delay lower 75ms
		/////////////**************down is read
		ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if((m_osRead.hEvent = CreateEvent(NULL,true,false,_T("Read")))==NULL)
			return -2; 
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState=ReadFile(m_hSerial,// Handle
			gval,// Data buffer Address
			13,// Data size
			&m_had_send_data_number,// Returns the number of bytes sent
			&m_osRead);
		if(!fState)// Overlflow Not Supported	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number=0;
		}
		//CloseHandle(m_osRead.hEvent);
		/*
		CStdioFile default_file;
		CString saved_path="C:\\modbus_scan_data.txt";
		CString a_line;
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("T:%x %x %x %x %x %x",pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
		default_file.WriteString(a_line+"\n");
		a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}
		*/
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);


#ifndef _DEBUG

		if (g_fileScanLog == NULL)
		{
			g_fileScanLog = new CStdioFile;
			g_fileScanLog->Open(g_strLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone );
			
		}

		if(g_fileScanLog != NULL)
		{
			CString strCom;
			strCom.Format(_T("Com=%d : "), open_com_port_number_in_dll);
			g_fileScanLog->WriteString(strCom);
			g_fileScanLog->WriteString(_T("Send Data : "));
				SaveBufferToLogFile(pval, 6);
			
			g_fileScanLog->WriteString(_T("Recv Data : "));
				SaveBufferToLogFile(gval, 13);
			
		}
		

#endif



		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocal
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME);//Delay required here
				return -4;              //no response ,no connection
			}
			// TBD: Clarify this comment. 
                 //T3000 finds a com port 
			//which doesnt connect to a devicew, but write file to the com, will receive the same data with send, 
			// infact the com port is bad. But it never give you a wrong data.
                    
			if(gval[0]==pval[0] && gval[1]==pval[1] && gval[2]==pval[2] && gval[3]==pval[3] && gval[4]==pval[4] && gval[5]==pval[5])
			{
				Sleep(SLEEP_TIME);
				return -4;              
			}

			//////////////////////////////////////////////////////////////////////////
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME);//Need a delay here
				return -3;
			}
			if((gval[0]!=pval[0]) || (gval[1]!=25))
			{
				Sleep(SLEEP_TIME);//Need a delay here
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//Need a delay here
				return -2;
			}
		}
		else
		{// new scan protocol,if many old devices ,get into here and scan result is OK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
				return -3;
			if((gval[0]!=pval[0]) || (gval[1]!=25))
				return -2;
			crc=CRC16(gval,7);
			if( gval[7]!=((crc>>8) & 0xff) )
				return -2;
			if(gval[8]!=(crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		return gval[2];
	}

	if(bComm_Type==1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,retry
		//the return value == -3, may have more than 2 devices is connecting
		//the return value == -4 , between devLo and devHi, no device is detected
		//the return value == -5 ,the input buffer has trouble. 
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//Empty the serial port buffer
		//the return value is the register address

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer

		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//Number of bytes of data sent
		pval[0]=1;
		pval[1]=2;
		pval[2]=3;
		pval[3]=4;
		pval[4]=5;
		pval[5]=6;

		pval[6] = 255;
		pval[7] = 25;
		pval[8] = devHi;
		pval[9] = devLo;
		
		/*
		crc = CRC16(pval,4);
		pval[4] = (crc >>8) & 0xff;
		pval[5] = crc & 0xff;
		*/

		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		::send(m_hSocket,(char*)pval,sizeof(pval),0);

		//Sleep(SLEEP_TIME+8);
		Sleep(LATENCY_TIME_NET+100);
		TS_UC  rvData[19];
		for(int i=0;i<19;i++)
			rvData[i]=0;
		int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
		if (nRecv>0)
		{
			memcpy(gval,(void*)&rvData[6],sizeof(rvData));
		}
		//if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)//// it's old code, modified by zgq:2011-12-14
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0 && gval[3]==0 &&gval[4]==0 &&gval[5]==0 &&gval[6]==0)
		{//old scan protocal
 			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME+8);//Need a delay here
				return -4;              //no response ,no connection
			}
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME+8);// Need a delay here
				return -3;
			}
			if((gval[0]!=255) || (gval[1]!=25))
			{
				//		Sleep(SLEEP_TIME);// Need a delay here
				return -2;
			} 
			/*
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				//		Sleep(SLEEP_TIME);// Need a delay here 
				return -2;
			}
			*/

		}
		else
		{// new scan protocol,if many old devices then scan result is OK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);// Need a delay here 

				
			//if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)//to inspect
			if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect  //zgq: same with com port scan
			{
				//Sleep(SLEEP_TIME+8);
				return -3;

			}
				
			if((gval[0]!=255) || (gval[1]!=25))
				return -2;

			/*
			if (gval[0]==0&&gval[0]==0&&gval[0]==0)
			{
				return -3;
			}
			*/

			/*
			crc=CRC16(gval,7);
			if( gval[7]!=((crc>>8) & 0xff) )
				return -2;
			if(gval[8]!=(crc & 0xff))
				return -2;
				*/
		}
		return gval[2];
	}
	
	return 0;
}

  int CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{	
	if(bComm_Type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,retry
		//the return value == -3, may have more than 2 devices is connecting
		//the return value == -4 ,between devLo and devHi,no devices detected
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,May have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address
		//Sleep(50);       // Need a delay here 
		if(devLo<1 || devHi>254)
			return -5;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0)
			{
				the_return_value2=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}  
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocal
			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
				return the_return_value;
			int i=0;
			do{
				the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
					return the_return_value;
				else if(the_return_value>0)
					i++;
			}while(i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if(bComm_Type==1)//
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connection
		//the return value == -2 ,retry
		//the return value == -3, Maybe have more than 2 devices connecting
		//the return value == -4 , between devLo and devHi,no device is detected
		//the return value == -5 ,the input buffer has some problem
		//the return value >=1 ,the devLo!=devHi, may have 2 devices connecting
		//Empty the serial port buffer
		//the return value is the register address
		//Sleep(50);       //Need a delay here (before anyway)
		if(devLo<1 || devHi>254)
			return -5;
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
		//down is inspect result first scan
		if(the_return_value==-4)
		{
			the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
			return the_return_value;
		}
		if(old_or_new_scan_protocal_in_dll==1)
		{//new protocal		
			if(the_return_value>0)
			{
				the_return_value2=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		else if(old_or_new_scan_protocal_in_dll==2)
		{//old protocal
// 			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
// 				return the_return_value;
// 			int i=0;
// 			do{
// 				the_return_value=CheckTstatOnline2(devLo,devHi, bComm_Type);
// 				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
// 					return the_return_value;
// 				else if(the_return_value>0)
// 					i++;
// 			}while(i<3);
			if(the_return_value>0)
			{
				the_return_value2=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
				if(the_return_value2!=-4)
					the_return_value=the_return_value2;
			}
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	return 0;
}






  BOOL Ping(const CString& strIP, CWnd* pWndEcho)
{
	CMyPing* pPing = new CMyPing;

	pPing->SetPingEchoWnd(pWndEcho);
	pPing->TestPing(strIP);


	delete pPing;
	pPing = NULL;
	return FALSE;
}


#ifndef _DEBUG
void SaveBufferToLogFile(TS_UC* pBuffer, int nSize)
{
	ASSERT(g_fileScanLog);
	for (int i = 0; i < nSize; i++)
	{
		int nValue = pBuffer[i];
		CString strValue;
		strValue.Format(_T("%d, "), nValue);
		g_fileScanLog->WriteString(strValue);
	}

	g_fileScanLog->WriteString(_T("\n"));	

}
#endif


