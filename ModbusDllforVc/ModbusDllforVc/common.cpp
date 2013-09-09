//*********common.cpp**********
#include "stdafx.h"
#include "common.h"
#include "crc.h"
#define TRUE_OR_FALSE	true
#define SLEEP_TIME		50
#define  LATENCY_TIME_COM	60      // �����ӳ٣�����600����Ϊ500���Сʱ��ͨ��NC scan TStat����ʧ��
#define  LATENCY_TIME_NET	100


	//******************************extern variable
	extern TS_US Register_value[MaxRegisterNum];           //got the register value
	extern TS_UC device;                         //Tstat ID,255 is for all
	extern CString m_com_num;						//com1 or com2
	extern CMutex mutex;//multithreading lock
 	extern HANDLE m_hSerial;//���ھ��
	extern OVERLAPPED m_osRead, m_osWrite, m_osMulWrite; // �����ص���/д
	extern TS_UC  gval[13];//the data that get from com
	extern TS_UC  serinumber_in_dll[4];//only read_one function ,when read 10,
    extern TS_UC  pval[13];//the data that send from com
	extern TS_UC  multi_read_val[256];//the register value is put into here,by multi_read function//the number must less 256
	static int baudrate_in_dll=0;
	static int open_com_port_number_in_dll=65535;
	static int old_or_new_scan_protocal_in_dll=1;//1==new protocal;2==old protocal

extern 	SOCKET m_hSocket;	
int g_Commu_type=0;//0:serial modus//

//CMutex scan_mutex;



CStdioFile* g_fileScanLog = NULL;
CStdioFile* g_fileScanNetLog=NULL;




OUTPUT void SetCommunicationType(int nType)
{
	g_Commu_type=nType;
}

/*
OUTPUT bool open_com(TS_UC m_com)
{	
	//open com ,if you want to open "com1",the m_com equal 0;if you want to open "com2",the m_com equal 1
	//you will get the handle to com,m_hSerial,is a extern variable
	//the return value ,true is ok,false is failure
	if(open_com_port_number_in_dll==m_com)
	{
		Change_BaudRate(19200);
		return true;///////////////////////////same com port ,opened by multi times,it's badly.
	}
	if(m_hSerial != NULL)  
	{//�رմ���
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	/////////////////////////////////////////////////////////////////////����Ĵ���ͨ�Ų���
	switch(m_com)
	{
	case 1:					m_hSerial = CreateFile(_T("COM1:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 2:			        m_hSerial = CreateFile(_T("COM2:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 3:					m_hSerial = CreateFile(_T("COM3:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 4:			        m_hSerial = CreateFile(_T("COM4:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 5:					m_hSerial = CreateFile(_T("COM5:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);
							break;
	case 6:			        m_hSerial = CreateFile(_T("COM6:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 7:					m_hSerial = CreateFile(_T("COM7:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 8:			        m_hSerial = CreateFile(_T("COM8:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	case 9:			        m_hSerial = CreateFile(_T("COM9:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
							NULL);break;
	default :				m_hSerial = CreateFile(_T("COM1:"),//���ھ�����򿪴���
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
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
    // Ĭ�ϴ��ڲ���
    if(!GetCommState(m_hSerial, &PortDCB))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
		return false;
	}	
	//not to change the baudate
    PortDCB.BaudRate = 19200; // baud//attention ,if it is wrong,can't write the com
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

*/



OUTPUT void close_com()
{
	if(g_Commu_type==0)
	{
		if(m_hSerial != NULL)  
		{//�رմ���
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

OUTPUT int CheckTstatOnline2(TS_UC devLo,TS_UC devHi)
{
	if(g_Commu_type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			pval,// ���ݻ�������ַ
			6,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			13,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			// added by zgq; find this situation: t3000 can find a comport, 
			//which don't connect a tstat, but write file to the com, will receive the same data with send, 
			// infact the com port don't work fine. But it never give you a wrong data.
			if(gval[0]==pval[0] && gval[1]==pval[1] && gval[2]==pval[2] && gval[3]==pval[3] && gval[4]==pval[4] && gval[5]==pval[5])
			{
				Sleep(SLEEP_TIME);
				return -4;              
			}

			//////////////////////////////////////////////////////////////////////////
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if((gval[0]!=pval[0]) || (gval[1]!=25))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
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
	//	return gval[2];
	}

	if(g_Commu_type==1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer

		TS_UC  pval[10];
	//	TS_US crc;
	//	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			} 
			/*
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			*/

		}
		else
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble

				
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
	//	return gval[2];
	}
	//Modify by Fance , both "if"  and "else" all return gval[2] ,so it can be moved to this ;
	//the warning not all control paths return a value will be disappeared.
	return gval[2];
}

OUTPUT int CheckTstatOnline(TS_UC devLo,TS_UC devHi)
{	
	int the_return_value;
	if(g_Commu_type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if(devLo<1 || devHi>254)
			return -5;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
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
		{//old protocal
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
		//return the_return_value;
	}

	if(g_Commu_type==1)//
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if(devLo<1 || devHi>254)
			return -5;
		if (m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}
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
		{//old protocal
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
		//return the_return_value;
	}
	//Modify by Fance , both "if"  and "else" all return the_return_value ,so it can be moved to this ;
	//the warning not all control paths return a value will be disappeared.
	return the_return_value;
}
OUTPUT bool Change_BaudRate(TS_US new_baudrate)
{

    ///���ô���
	if(new_baudrate!=9600 && new_baudrate!=19200)
		return false;
	if(baudrate_in_dll==new_baudrate)
		return true;
	else
		baudrate_in_dll=new_baudrate;
    DCB  PortDCB;    
    PortDCB.DCBlength = sizeof(DCB); 
    // Ĭ�ϴ��ڲ���
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
    PortDCB.BaudRate = new_baudrate; // baud//attention ,if it is wrong,can't write the com
    PortDCB.ByteSize = 8;     // Number of bits/byte, 4-8 
    PortDCB.Parity = NOPARITY; 
    PortDCB.StopBits = ONESTOPBIT;  
 	successful=false;
	for(i=0;i<10;i++)
	if(SetCommState(m_hSerial, &PortDCB))
	{
			///L"���ô���ʧ��";
		successful=true;
		break;
	}
	return(successful);
}

OUTPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts)
{
	return MKBOOL(SetCommTimeouts(m_hSerial,lpCommTimeouts));
}
//socket dll.
OUTPUT bool Open_Socket(CString strIPAdress)
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
		// ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ
		//servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
		servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
	//	u_long ul=1;
	//	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
		//����ʱ��
		setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
		//����ʱ��
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
OUTPUT bool Open_Socket2(CString strIPAdress,short nPort)
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
	//����ʱ��
	setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
	//����ʱ��
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

//////////////////////////////////////////////////////////////////////////
// Connect ��Ϊ���������
// 1�����ڴ򿪹��������������豸ͨ��ʧ�ܣ��ϵ磬���ߵȵȣ���
// ����Ϊ�Դ��ڲ�������������û��ͨ�����ݡ�
// 2�����ڴ�ʧ�ܻ�����������USB���ڰγ���ռ�õȵȣ���
// ����Ϊ�޷��������ڡ�

OUTPUT bool is_connect()
{
	if (g_Commu_type==0)
	{
		if(m_hSerial==NULL)
			return FALSE;
		else
			return TRUE;
	}
	else if (g_Commu_type==1)
	{
		if(m_hSocket==NULL)
			return false;//disconnected
		else
			return true;//connected
	}
	return false; //add by Fance
}

//device_var:priduct ID,address:a regster;


OUTPUT int write_multi_Short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length)
{	
	if(g_Commu_type==0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600]={0};
		data_to_write[0]=device_var;
		data_to_write[1]=0x10;
		data_to_write[2]=start_address >> 8 & 0xff;
		data_to_write[3]=start_address & 0xff;
		data_to_write[4]=0;
		data_to_write[5]=length*0xff;
		data_to_write[6]=length*2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for(int i=0;i<length;i++)
			{
				data_to_write[7+i*2] =to_write[i]  >> 8 & 0xff;
				data_to_write[7+i*2+1]=to_write[i] & 0xff;
			
			}
 

		TS_US crc=CRC16(data_to_write,length+7);
		data_to_write[length*2+7]=crc>>8 & 0xff;
		data_to_write[length*2+8]=crc & 0xff;

		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			data_to_write,// ���ݻ�������ַ
			length*2+9,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osMulWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		data_to_write[11]=length&0xff;
		data_to_write[12]=length*2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for(int i=0;i<length;i++)
			 {
				 data_to_write[13+i*2] =to_write[i]  >> 8 & 0xff;
				 data_to_write[13+i*2+1]=to_write[i] & 0xff;
			 }
		
			
	//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
	//	TS_US crc=CRC16(data_to_write,i+7);
	//	data_to_write[i+7]=crc>>8 & 0xff;
	//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
//length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}
	
		int n=::send(m_hSocket,(char *)data_to_write,13+2*length,0);
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
	return -1;
}

OUTPUT int Read_One(TS_UC device_var,TS_US address)
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
		//��մ��ڻ�����
		//TS_UC  gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

		BOOL bRet = ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;

		int fState=WriteFile(m_hSerial,// ���
			pval,	// ���ݻ�������ַ
			8,		// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				11,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		else
		{
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				7,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//��մ��ڻ�����
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
		//TS_US crc;		
		//DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���

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
	return -1; //add by Fance
//	singlock.Unlock();
}

OUTPUT int Read_One_log(TS_UC device_var,TS_US address,char *put_senddate_into_here,char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
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
		//��մ��ڻ�����
		//TS_UC  gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

		BOOL bRet = ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
		///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
			return -2; 
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;

		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = pval[i];
		}

		int fState=WriteFile(m_hSerial,// ���
			pval,	// ���ݻ�������ַ
			8,		// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				11,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		else
		{
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				7,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number=0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = gval[i];
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
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//��մ��ڻ�����
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
		//TS_US crc;		
		//DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���

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
		*sendDataLength = nLen;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data[i];
		}

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
		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = rvData[i];
		}

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
	return -1; //add by Fance
//	singlock.Unlock();
}

OUTPUT int Write_One(TS_UC device_var,TS_US address,TS_US val)
{
	if (g_Commu_type==0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����

		//		gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
			fState=WriteFile(m_hSerial,// ���
				pval,// ���ݻ�������ַ
				8,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osWrite);
		}
		else
		{//==10 register
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=WriteFile(m_hSerial,// ���
					pval,// ���ݻ�������ַ
					8,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osWrite);
			}
			else
			{//new protocal
				fState=WriteFile(m_hSerial,// ���
					pval,// ���ݻ�������ַ
					12,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osWrite);
			}
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				8,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=ReadFile(m_hSerial,// ���
					gval,// ���ݻ�������ַ
					8,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osRead);
			}
			else
			{//new protocal
				fState=ReadFile(m_hSerial,// ���
					gval,// ���ݻ�������ַ
					12,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osRead);
			}
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
	
	if(g_Commu_type==1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����
		
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
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	//	TS_US crc;		
	//	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	return -1;//add by Fance 
	///////////////////////////////////////////////////////////
}

OUTPUT int Write_One_log(TS_UC device_var,TS_US address,TS_US val,char *put_senddate_into_here,char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
	if (g_Commu_type==0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����

		//		gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
			*sendDataLength = 8;
			for (int i = 0; i < *sendDataLength; i++)
			{
				*((char*)put_senddate_into_here + i) = pval[i];
			}

			fState=WriteFile(m_hSerial,// ���
				pval,// ���ݻ�������ַ
				8,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osWrite);
		}
		else
		{//==10 register
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				*sendDataLength = 8;
				for (int i = 0; i < *sendDataLength; i++)
				{
					*((char*)put_senddate_into_here + i) = pval[i];
				}
				fState=WriteFile(m_hSerial,// ���
					pval,// ���ݻ�������ַ
					8,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osWrite);
			}
			else
			{//new protocal
				*sendDataLength = 12;
				for (int i = 0; i < *sendDataLength; i++)
				{
					*((char*)put_senddate_into_here + i) = pval[i];
				}

				fState=WriteFile(m_hSerial,// ���
					pval,// ���ݻ�������ַ
					12,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osWrite);
			}
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				8,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=ReadFile(m_hSerial,// ���
					gval,// ���ݻ�������ַ
					8,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osRead);
			}
			else
			{//new protocal
				fState=ReadFile(m_hSerial,// ���
					gval,// ���ݻ�������ַ
					12,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osRead);
			}
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number=0;
		}		

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = gval[i];
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
	
	if(g_Commu_type==1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����
		
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
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	//	TS_US crc;		
	//	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		*/
		if(m_hSocket==INVALID_SOCKET)
		{			
			return -1;
		}

		//::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		*sendDataLength = nSendNum;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data[i];
		}

		int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);

		//Sleep(SLEEP_TIME+10);
		Sleep(LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = rvdata[i];
		}

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
	return -1;//add by Fance 
	///////////////////////////////////////////////////////////
}

OUTPUT int read_multi(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length)
{
	if(g_Commu_type==0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300]={'\0'};
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

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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



		int fState=WriteFile(m_hSerial,// ���
			data_to_send,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osMulWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			to_send_data,// ���ݻ�������ַ
			length*2+5,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		//the return value == -1 ,no connecting
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

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}

		::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

		Sleep(LATENCY_TIME_NET);
		int nn=sizeof(to_Reive_data);

		unsigned long bytes_available;
		ioctlsocket(m_hSocket,FIONREAD,&bytes_available);
		if (bytes_available == 0) return -1;
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
	return -1;
}

OUTPUT int read_multi_log(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length,char *put_senddate_into_here,char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
	if(g_Commu_type==0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300]={'\0'};
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

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		m_osMulWrite.OffsetHigh = 0;

		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}

		///////////////////////////////////////////////////////send the to read message
		int fState=WriteFile(m_hSerial,// ���
			data_to_send,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osMulWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			to_send_data,// ���ݻ�������ַ
			length*2+5,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number=0;
		}	

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_send_data[i];
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


		/*for (int i=0;i<8;i++)
		{
			put_senddate_into_here[i]=data_to_send[i];
		}
		for (int i=0;i<2*length+5;i++)
		{
			put_revdata_into_here[i]=to_send_data[i];
		}*/
		return length;
	}
	if(g_Commu_type==1)//tcp.
	{
		/*
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting
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

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}

		*sendDataLength = sizeof(data_to_send);
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}

		::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

		Sleep(LATENCY_TIME_NET);
		int nn=sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_Reive_data[i];
		}

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
		/*for (int i=6;i<12;i++)
		{
		  put_senddate_into_here[i]=data_to_send[i];
		}
		for (int i=0;i<2*length+3;i++)
		{
		  put_revdata_into_here[i]=to_send_data[i];
		}*/
		return length;
	}
	return -1;
}

OUTPUT int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length)
{	
	if(g_Commu_type==0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600]={'\0'};
		data_to_write[0]=device_var;
		data_to_write[1]=0x10;
		data_to_write[2]=start_address >> 8 & 0xff;
		data_to_write[3]=start_address & 0xff;
		data_to_write[4]=0;
		data_to_write[5]=length;
		data_to_write[6]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
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
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			data_to_write,// ���ݻ�������ַ
			length+9,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osMulWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		data_to_write[12]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for(int i=0;i<length;i++)
			data_to_write[13+i]=to_write[i];
		
			
	//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
	//	TS_US crc=CRC16(data_to_write,i+7);
	//	data_to_write[i+7]=crc>>8 & 0xff;
	//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	return -1;
}

OUTPUT int write_multi_log(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length,char *put_senddate_into_here,char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{	
	if(g_Commu_type==0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600]={'\0'};
		data_to_write[0]=device_var;
		data_to_write[1]=0x10;
		data_to_write[2]=start_address >> 8 & 0xff;
		data_to_write[3]=start_address & 0xff;
		data_to_write[4]=0;
		data_to_write[5]=length;
		data_to_write[6]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
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
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

		*sendDataLength = length + 9;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		///////////////////////////////////////////////////////send the to read message
		int fState=WriteFile(m_hSerial,// ���
			data_to_write,// ���ݻ�������ַ
			length+9,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osMulWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number=0;
		}	

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = gval[i];
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
		data_to_write[12]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for(int i=0;i<length;i++)
			data_to_write[13+i]=to_write[i];
		
			
	//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
	//	TS_US crc=CRC16(data_to_write,i+7);
	//	data_to_write[i+7]=crc>>8 & 0xff;
	//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
		if(m_hSocket==INVALID_SOCKET)
		{
			return -1;
		}
	
		*sendDataLength = 13 + length;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		int n=::send(m_hSocket,(char *)data_to_write,13+length,0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = data_back_write[i];
		}

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
	return -1;
}

OUTPUT int NetController_CheckTstatOnline(TS_UC devLo,TS_UC devHi)
{
	if(g_Commu_type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
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
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
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

OUTPUT int NetController_CheckTstatOnline2(TS_UC devLo,TS_UC devHi)
{
	if(g_Commu_type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			pval,// ���ݻ�������ַ
			6,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)//��֧���ص�
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			13,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
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

	if(g_Commu_type==1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect

		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���

		
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
			//�쳣:
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
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
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
	return -1;
}

//also return the serial communication handle;
OUTPUT HANDLE GetCommunicationHandle()
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

	return NULL;
}

OUTPUT BOOL bTCPDisconnected()
{
	return TRUE;
}

OUTPUT void SetComnicationHandle(int nType,HANDLE hCommunication)
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

OUTPUT bool open_com(int m_com)
{	
	//open com ,if you want to open "com1",the m_com equal 0;if you want to open "com2",the m_com equal 1
	//you will get the handle to com,m_hSerial,is a extern variable
	//the return value ,true is ok,false is failure
	if(open_com_port_number_in_dll==m_com)
	{
		Change_BaudRate(19200);
		return true;///////////////////////////same com port ,opened by multi times,it's badly.
	}
	if(m_hSerial != NULL)  
	{//�رմ���
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	/////////////////////////////////////////////////////////////////////����Ĵ���ͨ�Ų���
// 	LPCSTR lpComNum[6];
// 	ZeroMemory(lpComNum, )

	CString strCom;
	if (m_com <=9 && m_com >0)
	{
		strCom.Format(_T("%d"),m_com);
		strCom = _T("COM")+strCom+_T(":");
	}
	else if(m_com > 9)
	{
		strCom.Format(_T("%d"),m_com);
		strCom = _T("\\\\.\\COM")+strCom;
	}
	
	m_hSerial = CreateFile( strCom, //strCom,//���ھ�����򿪴���
								GENERIC_READ | GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д
								NULL);


	if(m_hSerial == INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;		
		return false;
	}
	//if(!SetupComm(m_hSerial, 1024*32, 1024*9))
	if(!SetupComm(m_hSerial, 1024*64, 1024*32))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	DCB  PortDCB;    
	PortDCB.DCBlength = sizeof(DCB); 
	// Ĭ�ϴ��ڲ���
	if(!GetCommState(m_hSerial, &PortDCB))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
		return false;
	}	
	//not to change the baudate
	PortDCB.BaudRate = 19200; // baud//attention ,if it is wrong,can't write the com
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

#if 0 //lsc 
	CommTimeouts.ReadIntervalTimeout = 1000;
	CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
#endif


	if (!SetCommTimeouts(m_hSerial, &CommTimeouts))
	{
		return false;
	}          
	open_com_port_number_in_dll=m_com;
	baudrate_in_dll=19200;

	return true;
}

OUTPUT int Read_One2(TS_UC device_var,TS_US address, bool bComm_Type)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
	int nTemp=-1;
	if(bComm_Type==0)//serial
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//��մ��ڻ�����
		//TS_UC  gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

		int fState=WriteFile(m_hSerial,// ���
			pval,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)// I/Oδ��ɻ�ʧ��	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				11,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		else
		{
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				7,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
	if(bComm_Type==1)//tcp.
	{  
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//��մ��ڻ�����
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
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���

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
	return -1;
//	singlock.Unlock();
}

OUTPUT int Write_One2(TS_UC device_var,TS_US address,TS_US val, bool bComm_Type)
{
	if (bComm_Type==0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����

		//		gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for(int i=0;i<=11;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_US crc;		
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
			fState=WriteFile(m_hSerial,// ���
				pval,// ���ݻ�������ַ
				8,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osWrite);
		}
		else
		{//==10 register
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=WriteFile(m_hSerial,// ���
					pval,// ���ݻ�������ַ
					8,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osWrite);
			}
			else
			{//new protocal
				fState=WriteFile(m_hSerial,// ���
					pval,// ���ݻ�������ַ
					12,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osWrite);
			}
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
			fState=ReadFile(m_hSerial,// ���
				gval,// ���ݻ�������ַ
				8,// ���ݴ�С
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
				&m_osRead);
		}
		else
		{
			if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
			{//old protocal
				fState=ReadFile(m_hSerial,// ���
					gval,// ���ݻ�������ַ
					8,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osRead);
			}
			else
			{//new protocal
				fState=ReadFile(m_hSerial,// ���
					gval,// ���ݻ�������ַ
					12,// ���ݴ�С
					&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
					&m_osRead);
			}
		}
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����
		
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
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	//	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	return -1;
	///////////////////////////////////////////////////////////
}

OUTPUT int NetController_CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
	if(bComm_Type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
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

	if(bComm_Type==1)//tcp
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
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

OUTPUT int NetController_CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
     CString strlog;
	if(bComm_Type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
			strlog.Format(_T("Scan From ID=%d To ID=%d"),devLo,devHi);
			WriteLogFile(strlog);
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			pval,// ���ݻ�������ַ
			6,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)//��֧���ص�
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			13,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number=0;
		}

		
				CString strCom;
				strCom.Format(_T("Com=%d : "), open_com_port_number_in_dll);
			
				WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
				    CString filelog;
				filelog=_T("Send a scan command to any devices: ");
				//SaveBufferToLogFile(pval, 6);
				 
				   
				for (int i = 0; i < 6; i++)
				{
					int nValue = pval[i];
					CString strValue;
					strValue.Format(_T("%0x, "), nValue);
					//g_fileScanLog->WriteString(strValue);
					filelog+=strValue;
				}
				WriteLogFile(filelog);
				filelog.Empty();
				filelog=_T("Recv Data : ");
				for (int i = 0; i < 13; i++)
				{
					int nValue = gval[i];
					CString strValue;
					strValue.Format(_T("%0x, "), nValue);
					//g_fileScanLog->WriteString(strValue);
					filelog+=strValue;
				}
				WriteLogFile(filelog);
				int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
				if(index==-1)
				{
				 WriteLogFile(_T(">>More than one device answer......"));
				}
				else
				{
				  WriteLogFile(_T(">>No one device answer......"));
				}
				//SaveBufferToLogFile(gval, 13);

				
		



	


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
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
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
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		strlog.Format(_T("Scan From ID=%d To ID=%d"),devLo,devHi);
		NET_WriteLogFile(strlog);
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect

		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���

		
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
			//�쳣:
			memcpy(gval,(void*)&rvdata[6],sizeof(rvdata)-6);
		}


		NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog=_T("Send a scan command to any devices: ");

		for (int i = 0; i < 10; i++)
		{
			int nValue = pval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog+=strValue;
		}
		NET_WriteLogFile(filelog);
		filelog.Empty();
		NET_WriteLogFile(_T("Recv Data : "));
		for (int i = 0; i < 19; i++)
		{
			int nValue = rvdata[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog+=strValue;
		}
		
		NET_WriteLogFile(filelog);
		 
		int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if(index==-1)
		{
			NET_WriteLogFile(_T(">>More than one device answer......"));
		}
		else
		{
			NET_WriteLogFile(_T(">>No one device answer......"));
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
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
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
	return -1;
}

OUTPUT int read_multi2(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length,bool bComm_Type)
{
	if(bComm_Type==0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300]={'\0'};
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

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			data_to_send,// ���ݻ�������ַ
			8,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osMulWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			to_send_data,// ���ݻ�������ַ
			length*2+5,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		//the return value == -1 ,no connecting
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

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
	return -1;
}

OUTPUT int CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{	CString strlog;	
    
    
	if(bComm_Type==0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		strlog.Format(_T("Com Scan:  From ID=%d To ID=%d"),devLo,devHi);
		WriteLogFile(strlog);

		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer
		TS_UC  pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int fState=WriteFile(m_hSerial,// ���
			pval,// ���ݻ�������ַ
			6,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osWrite);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
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
		fState=ReadFile(m_hSerial,// ���
			gval,// ���ݻ�������ַ
			13,// ���ݴ�С
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ���
			&m_osRead);
		if(!fState)// ��֧���ص�	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number=0;
		}
		 
		  

		WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog=_T("Send a scan command to any devices: ");
		
		 

		for (int i = 0; i < 6; i++)
		{
			int nValue = pval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog+=strValue;
		}
		 
		WriteLogFile(filelog);
		filelog.Empty();
		filelog=_T("Recv Data : ");
		for (int i = 0; i < 13; i++)
		{
			int nValue = gval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog+=strValue;
		}
		
		WriteLogFile(filelog);
		int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if(index==-1)
		{
			WriteLogFile(_T(">>More than one device answer......"));
		}
		else
		{
			WriteLogFile(_T(">>No one device answer......"));
		}



		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
		{//old scan protocal
			old_or_new_scan_protocal_in_dll=2;
			if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection

			
			}
			// added by zgq; find this situation: t3000 can find a comport, 
			//which don't connect a tstat, but write file to the com, will receive the same data with send, 
			// infact the com port don't work fine. But it never give you a wrong data.
			if(gval[0]==pval[0] && gval[1]==pval[1] && gval[2]==pval[2] && gval[3]==pval[3] && gval[4]==pval[4] && gval[5]==pval[5])
			{

			
				Sleep(SLEEP_TIME);
				return -4;    
			}

			//////////////////////////////////////////////////////////////////////////
			if(gval[5]!=0 || gval[6]!=0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if((gval[0]!=pval[0]) || (gval[1]!=25))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
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
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		strlog.Format(_T("NET Scan:  From ID=%d To ID=%d"),devLo,devHi);
		NET_WriteLogFile(strlog);
		if(devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for(int i=0;i<13;i++)
			gval[i]=0;/////////////////////////////////////////clear buffer

		TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
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
		int nRet =::send(m_hSocket,(char*)pval,sizeof(pval),0);//scan ɨNC�е�TSTAT

		//Sleep(SLEEP_TIME+8);
		Sleep(LATENCY_TIME_NET+100);
// 		TS_UC  rvData[19];
// 		for(int i=0;i<19;i++)
// 			rvData[i]=0;

		TS_UC  rvData[100];
		for(int i=0;i<100;i++)
			rvData[i]=0;


		int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
		if (nRecv>0)
		{
			memcpy(gval,(void*)&rvData[6],sizeof(rvData));
		}

//		static int num = 0;
		NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog=_T("Send a scan command to any devices: ");
	 

		for (int i = 0; i < 10; i++)
		{
			int nValue = pval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog+=strValue;
		}
		 
		NET_WriteLogFile(filelog);
		filelog.Empty();
		 
		filelog=_T("Recv Data : ");
		for (int i = 0; i <13; i++)
		{
			int nValue = gval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog+=strValue;
		}
		//NET_WriteLogFile(_T("Recv Data:"));
		NET_WriteLogFile(filelog);
		int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if(index==-1)
		{
			WriteLogFile(_T(">>More than one device answer......"));
		}
		else
		{
			WriteLogFile(_T(">>No one device answer......"));
		}

//		if (g_fileScanLog == NULL)
//		{
//			g_fileScanLog = new CStdioFile;
//		}
//
//	if (g_fileScanLog != NULL)
//		{
//			TCHAR exeFullPath[MAX_PATH+1]; //
//			GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
//			(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
//
//			CString g_strExePth=exeFullPath;
//			m_strFileINI = g_strExePth + _T("ScanLog.TXT");
//			if(g_fileScanLog->Open(m_strFileINI.GetString(),CFile::modeReadWrite))
//			{
//#if 1
//                g_fileScanLog->SeekToEnd();
//				CString strsend,strreceive;
//				strsend = _T("Send Data:");
//				strreceive = _T("Receive Data:");
//				g_fileScanLog->WriteString(strsend+_T("\n"));
//
//				CString str;
//				int idate;
//				//str.Format(_T("%"),pval);
//				//m_pFile->WriteString(str+_T("\n"));
//				for (int i =0;i<sizeof(pval);i++)
//				{
//					idate =(int)pval[i];
//					str.Format(_T("%0x"),idate);
//
//					g_fileScanLog->WriteString(str+_T(" "));	
//
//				}
//
//				g_fileScanLog->WriteString(_T("\n"));	
//				g_fileScanLog->WriteString(strreceive+_T("\n"));//��
//				for (int i =0;i<sizeof(rvData);i++)
//				{
//					idate =(int)rvData[i];
//					str.Format(_T("%0x"),idate);
//					g_fileScanLog->WriteString(str+_T(" "));	
//				}
//
//
//
//				g_fileScanLog->WriteString(_T("\n"));
//#endif
//				
//				
//				g_fileScanLog->Close();
//			}
//		    else
//			{
//			 //-----
//			}
//
//
//		}
        

	





		//if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)//// it's old code, modified by zgq:2011-12-14
		if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0 && gval[3]==0 &&gval[4]==0 &&gval[5]==0 &&gval[6]==0)
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
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			} 
			/*
			crc=CRC16(gval,3);
			if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			*/

		}
		else
		{// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			old_or_new_scan_protocal_in_dll=1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble

				
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
	return -1;
}

OUTPUT int CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{	
	if(bComm_Type==0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if(devLo<1 || devHi>254)
			return -5;
		if(m_hSerial==NULL)
		{			
			return -1;
		}
		int the_return_value;
		int the_return_value2=0;
		//CheckTstatOnline2_a ���Լ�¼�շ�������
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
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//��մ��ڻ�����
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
	return -1; 
}

OUTPUT void closefile()
{
		m_pFile->Close();
		delete m_pFile;
}

OUTPUT void writefile( CString strip,CString strport )
{
	m_pFile->WriteString(strip+_T("\t"));
	m_pFile->WriteString(strport+_T("\n\n"));
}

OUTPUT void Createfile()
{
	m_pFile = new CStdioFile;//txt

	TCHAR exeFullPath[MAX_PATH+1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	CString g_strDatabasefilepath=exeFullPath;
	CString g_strExePth=g_strDatabasefilepath;
	m_strFileINI = g_strExePth + _T("SANC.TXT");

	m_pFile->Open(m_strFileINI.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate );
}

OUTPUT void WriteLogFile(CString strlog){
	logfile_section.Lock();
	if (g_fileScanLog == NULL)
	{
		g_fileScanLog = new CStdioFile;
	
		TCHAR exeFullPath[MAX_PATH+1]; //
		GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
		(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
		CString g_strExePth=exeFullPath;
		m_strFileINI = g_strExePth + _T("COM_Log.TXT");
		g_fileScanLog->Open(m_strFileINI.GetString(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone );
		g_fileScanLog->Close();
	}

	if(g_fileScanLog != NULL)
	{	
		TCHAR exeFullPath[MAX_PATH+1]; //
		GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
		(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
		CString g_strExePth=exeFullPath;
		m_strFileINI = g_strExePth + _T("COM_Log.TXT");
		if (g_fileScanLog->Open(m_strFileINI.GetString(),CFile::modeReadWrite | CFile::shareDenyNone ))
		{
			g_fileScanLog->SeekToEnd();
			g_fileScanLog->WriteString(strlog);
			g_fileScanLog->WriteString(_T("\n"));
			g_fileScanLog->Flush();
			g_fileScanLog->Close();
		}
		else
		{
			g_fileScanLog->Close();
			g_fileScanLog->Open(m_strFileINI.GetString(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone );
			g_fileScanLog->SeekToEnd();
			g_fileScanLog->WriteString(strlog);
			g_fileScanLog->WriteString(_T("\n"));
			g_fileScanLog->Flush();
			g_fileScanLog->Close();
		}


	}

	logfile_section.Unlock();
}

OUTPUT void CloseLogFile()
{
	//g_fileScanLog->Close();
	delete g_fileScanLog;
	g_fileScanLog=NULL;

}

OUTPUT CString Get_NowTime()
{
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%2d-%2d"),st.wYear,st.wMonth,st.wDay);
	strTime.Format(_T("%2d:%2d:%2d"), st.wHour,st.wMinute,st.wSecond);
	strDate+=_T(" ");
	strDate+=strTime;


	return strDate;
}

OUTPUT void NET_WriteLogFile(CString strlog){
	NET_logfile_section.Lock();
	if (g_fileScanNetLog == NULL)
	{
		g_fileScanNetLog = new CStdioFile;

		TCHAR exeFullPath[MAX_PATH+1]; //
		GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
		(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
		CString g_strExePth=exeFullPath;
		m_strScanNetfilename = g_strExePth + _T("NET_Log.TXT");
		g_fileScanNetLog->Open(m_strScanNetfilename.GetString(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone );
		g_fileScanNetLog->Close();
	}

	if(g_fileScanNetLog != NULL)
	{	
		TCHAR exeFullPath[MAX_PATH+1]; //
		GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
		(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
		CString g_strExePth=exeFullPath;
		m_strScanNetfilename = g_strExePth + _T("NET_Log.TXT");
		if (g_fileScanNetLog->Open(m_strScanNetfilename.GetString(),CFile::modeReadWrite | CFile::shareDenyNone ))
		{
			g_fileScanNetLog->SeekToEnd();
			g_fileScanNetLog->WriteString(strlog);
			g_fileScanNetLog->WriteString(_T("\n"));
			g_fileScanNetLog->Flush();
			g_fileScanNetLog->Close();
		}
		else
		{
			g_fileScanNetLog->Close();
			g_fileScanNetLog->Open(m_strScanNetfilename.GetString(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone );
			g_fileScanNetLog->SeekToEnd();
			g_fileScanNetLog->WriteString(strlog);
			g_fileScanNetLog->WriteString(_T("\n"));
			g_fileScanNetLog->Flush();
			g_fileScanNetLog->Close();
		}


	}

	NET_logfile_section.Unlock();
}

OUTPUT void NET_CloseLogFile()
{
	//g_fileScanLog->Close();
	delete g_fileScanNetLog;
	g_fileScanNetLog=NULL;

}

