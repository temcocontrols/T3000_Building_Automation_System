#include "StdAfx.h"
#include "Modbus.h"
#include "crc.h"
#include <bitset>
using namespace std;

CModbus::CModbus()
{
	m_hSerial = 0;
	m_LATENCY_TIME_COM = 100;
	m_LATENCY_TIME_NET = 100;
	m_hSocket_for_list = NULL;
	m_hSocket = NULL;
	g_data_to_send[0] = 0;
	g_data_to_send[1] = 1;
	g_data_to_send[2] = 0;
	g_data_to_send[3] = 0;
	g_data_to_send[4] = 0;
	g_data_to_send[5] = 6;

	m_baudrate_in_dll = 0;
	m_open_com_port_number_in_dll = 65535;
	m_old_or_new_scan_protocal_in_dll = 1;//1==new protocal;2==old protocal

	  m_Commu_type = 0;
	 
	  m_last_connected_port = 0;
	  m_last_communication_type = 0;
	  m_successful_baudrate = 19200;
	  m_successful_com_port = 0;
	  m_ArrayBaudate[0] = 9600;
	  m_ArrayBaudate[1] = 19200;
	  m_ArrayBaudate[2] = 38400;
	  m_ArrayBaudate[3] = 57600;
	  m_ArrayBaudate[4] = 115200;
	    g_fileScanLog = NULL;
	  g_fileScanNetLog = NULL;
	  
}


CModbus::~CModbus()
{
}





 void CModbus::SetLastSuccessBaudrate(int nbaudrate)
{
	m_successful_baudrate = nbaudrate;
}

 void CModbus::SetLastOpenedComport(int ncom_port)
{
	m_successful_com_port = ncom_port;
}

 void CModbus::SetLastCommunicationType(int n_commnication_type)
{
	m_last_communication_type = n_commnication_type;
}


 int CModbus::GetLastSuccessBaudrate()
{
	return m_successful_baudrate;
}

 int CModbus::GetLastOpenedComport()
{
	return m_successful_com_port;
}

 int CModbus::GetLastCommunicationType()
{
	return m_last_communication_type;
}

  void CModbus::SetCommunicationType(int nType)
 {
 	m_Commu_type = nType;
 }

 int CModbus::GetCommunicationType(void)
{
	return m_Commu_type;
}






 void CModbus::close_com()
{
	if (m_Commu_type == 0)
	{
		if (m_hSerial != NULL)
		{
			//关闭串口
			CloseHandle(m_hSerial);
			m_hSerial = NULL;
		}
		m_baudrate_in_dll = 0;
		m_open_com_port_number_in_dll = 65535;
	}
	if (m_Commu_type == 1)
	{
		if (m_hSocket != INVALID_SOCKET)
		{
			::closesocket(m_hSocket);
			m_hSocket = NULL;
		}
	}

}

 int CModbus::CheckTstatOnline2(TS_UC devLo, TS_UC devHi)
{
	if (m_Commu_type == 0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_UC  m_pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = 255;
		m_pval[1] = 25;  //put comments here,
		m_pval[2] = devHi;
		m_pval[3] = devLo;
		crc = CRC16(m_pval, 4);
		m_pval[4] = (crc >> 8) & 0xff;
		m_pval[5] = crc & 0xff;
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear buffer
		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,// 数据缓冲区地址
			6,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}

		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,m_pval[0],m_pval[1],m_pval[2],m_pval[3],m_pval[4],m_pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(m_LATENCY_TIME_COM);//because that scan have a delay lower 75ms
								/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			13,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		//CloseHandle(m_osRead.hEvent);
		/*
		CStdioFile default_file;
		CString saved_path="C:\\modbus_scan_data.txt";
		CString a_line;
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("T:%x %x %x %x %x %x",m_pval[0],m_pval[1],m_pval[2],m_pval[3],m_pval[4],m_pval[5]);
		default_file.WriteString(a_line+"\n");
		a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}
		*/
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);



		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			// added by zgq; find this situation: t3000 can find a comport,
			//which don't connect a tstat, but write file to the com, will receive the same data with send,
			// infact the com port don't work fine. But it never give you a wrong data.
			if (m_gval[0] == m_pval[0] && m_gval[1] == m_pval[1] && m_gval[2] == m_pval[2] && m_gval[3] == m_pval[3] && m_gval[4] == m_pval[4] && m_gval[5] == m_pval[5])
			{
				Sleep(SLEEP_TIME);
				return -4;
			}

			//////////////////////////////////////////////////////////////////////////
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 25))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc = CRC16(m_gval, 3);
			if ((m_gval[3] != ((crc >> 8) & 0xff)) || (m_gval[4] != (crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect
				return -3;
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 25))
				return -2;
			crc = CRC16(m_gval, 7);
			if (m_gval[7] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[8] != (crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,m_gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",m_gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		//	return m_gval[2];
	}

	if (m_Commu_type == 1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address

		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer

		TS_UC  m_pval[10];
		//	TS_US crc;
		//	DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = 1;
		m_pval[1] = 2;
		m_pval[2] = 3;
		m_pval[3] = 4;
		m_pval[4] = 5;
		m_pval[5] = 6;

		m_pval[6] = 255;
		m_pval[7] = 25;
		m_pval[8] = devHi;
		m_pval[9] = devLo;

		/*
		crc = CRC16(m_pval,4);
		m_pval[4] = (crc >>8) & 0xff;
		m_pval[5] = crc & 0xff;
		*/

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		::send(m_hSocket, (char*)m_pval, sizeof(m_pval), 0);

		//Sleep(SLEEP_TIME+8);
		Sleep(m_LATENCY_TIME_NET);
		TS_UC  rvData[19];
		for (int i = 0; i<19; i++)
			rvData[i] = 0;
		int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
		if (nRecv>0)
		{
			memcpy(m_gval, (void*)&rvData[6], sizeof(rvData));
		}
		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != 255) || (m_gval[1] != 25))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			/*
			crc=CRC16(m_gval,3);
			if( (m_gval[3]!=((crc>>8) & 0xff)) || (m_gval[4]!=(crc & 0xff)))
			{
			//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			return -2;
			}
			*/

		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble


							  //if(m_gval[7]!=0 || m_gval[8]!=0 || m_gval[9]!=0 || m_gval[10]!=0)//to inspect
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect  //zgq: same with com port scan
			{
				//Sleep(SLEEP_TIME+8);
				return -3;

			}

			if ((m_gval[0] != 255) || (m_gval[1] != 25))
				return -2;

			/*
			if (m_gval[0]==0&&m_gval[0]==0&&m_gval[0]==0)
			{
			return -3;
			}
			*/

			/*
			crc=CRC16(m_gval,7);
			if( m_gval[7]!=((crc>>8) & 0xff) )
			return -2;
			if(m_gval[8]!=(crc & 0xff))
			return -2;
			*/
		}
		//	return m_gval[2];
	}
	//Modify by Fance , both "if"  and "else" all return m_gval[2] ,so it can be moved to this ;
	//the warning not all control paths return a value will be disappeared.
	return m_gval[2];
}

 int CModbus::CheckTstatOnline(TS_UC devLo, TS_UC devHi)
{
	int the_return_value;
	if (m_Commu_type == 0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSerial == NULL)
		{
			return -1;
		}
		int the_return_value2 = 0;
		the_return_value = CheckTstatOnline2(devLo, devHi);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = CheckTstatOnline2(devLo, devHi);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = CheckTstatOnline2(devLo, devHi);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
			if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
				return the_return_value;
			int i = 0;
			do
			{
				the_return_value = CheckTstatOnline2(devLo, devHi);
				if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
					return the_return_value;
				else if (the_return_value>0)
					i++;
			} while (i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		//return the_return_value;
	}

	if (m_Commu_type == 1)//
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		int the_return_value2 = 0;
		the_return_value = CheckTstatOnline2(devLo, devHi);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = CheckTstatOnline2(devLo, devHi);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = CheckTstatOnline2(devLo, devHi);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
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
			if (the_return_value>0)
			{
				the_return_value2 = CheckTstatOnline2(devLo, devHi);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
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

 bool CModbus::Change_BaudRate(int new_baudrate)
{

	///配置串口
	//if(new_baudrate!=9600 && new_baudrate!=19200)
	//	return false;
	BOOL iscorrect = FALSE;
	for (int i = 0; i< BAUDRATENUMBER; i++)
	{
		if (new_baudrate == m_ArrayBaudate[i])
		{
			iscorrect = TRUE;
			break;
		}
	}
	if (!iscorrect)
	{
		return FALSE;
	}
	if (m_baudrate_in_dll == new_baudrate)
		return true;
	else
		m_baudrate_in_dll = new_baudrate;
	DCB  PortDCB;
	PortDCB.DCBlength = sizeof(DCB);
	// 默认串口参数
	int i = 0;
	bool successful = false;//true==do it success;false==do it failure
	for (i = 0; i<10; i++)
		if (GetCommState(m_hSerial, &PortDCB))
		{
			successful = true;
			break;
		}
	if (successful == false)
		return false;
	//not to change the baudate
	PortDCB.BaudRate = new_baudrate; // baud//attention ,if it is wrong,can't write the com
	PortDCB.ByteSize = 8;     // Number of bits/byte, 4-8 
	PortDCB.Parity = NOPARITY;
	PortDCB.StopBits = ONESTOPBIT;
	successful = false;
	for (i = 0; i<10; i++)
		if (SetCommState(m_hSerial, &PortDCB))
		{
			///L"配置串口失败";
			successful = true;
			break;
		}
	return(successful);
}
 bool CModbus::SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts)
{
	return MKBOOL(SetCommTimeouts(m_hSerial, lpCommTimeouts));
}
//socket dll.
 bool CModbus::Open_Socket(CString strIPAdress)
{
	if (m_Commu_type == 0)
		return false;

	int nNetTimeout = 3000;//1 second.
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (::WSAStartup(sockVersion, &wsaData) != 0)
	{
		//	AfxMessageBox(_T("Init Socket failed!"));
		return FALSE;
	}
	if (m_hSocket != INVALID_SOCKET)
	{

		::closesocket(m_hSocket);
	}
	if (m_hSocket != NULL)
	{
		::closesocket(m_hSocket);
		m_hSocket = NULL;
	}

	m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hSocket == INVALID_SOCKET)
	{
		//	AfxMessageBox(_T("Create socket failed!"));
		return FALSE;
	}
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(6001);
	// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
	//servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
	servAddr.sin_addr.S_un.S_addr = inet_addr((LPSTR)(LPCTSTR)strIPAdress);
	//	u_long ul=1;
	//	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
	//发送时限
	setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	//接收时限
	setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
	if (::connect(m_hSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		//	AfxMessageBox(_T(" Failed connect() \n"));
		::closesocket(m_hSocket);
		m_hSocket = NULL;
		return FALSE;
	}
	return TRUE;
}

//socket dll.
 bool CModbus::Open_Socket2(CString strIPAdress, short nPort)
{
	//if(m_Commu_type==0)
	//	return false;

	int nNetTimeout = 3000;//1 second.
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);

	if (m_hSocket != INVALID_SOCKET)
	{
		::closesocket(m_hSocket);
		m_hSocket = NULL;
	}

	if (::WSAStartup(sockVersion, &wsaData) != 0)
	{
		//AfxMessageBox(_T("Init Socket failed!"));
		m_hSocket = NULL;
		return FALSE;
	}
	int sockfd;
	sockfd = m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hSocket == INVALID_SOCKET)
	{
		//	AfxMessageBox(_T("Create socket failed!"));
		m_hSocket = NULL;
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
	//发送时限
	setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	//接收时限
	setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//****************************************************************************
	// Fance added ,不要用阻塞的模式，如果设备不在线 经常性的 要等10几秒 ，老毛受不了。
	//改为非阻塞的 2.5秒后还没连接上就 算连接失败;
	int error = -1;
	int len;
	len = sizeof(int);
	timeval tm;
	fd_set set;
	unsigned long ul = 1;
	ioctlsocket(m_hSocket, FIONBIO, &ul); //设置为非阻塞模式
	bool ret = false;
	if (connect(m_hSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		tm.tv_sec = 3;//4.5s 如果连接不上就 算失败 ，不要重新retry了;
		tm.tv_usec = 500;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
			if (error == 0)
			{
				ret = true;
			}
			else
				ret = false;
		}
		else
		{
			ret = false;
		}
	}
	else ret = true;
	ul = 0;
	ioctlsocket(sockfd, FIONBIO, &ul); //设置为阻塞模式
									   //****************************************************************************

	if (ret)
	{
		m_last_connected_ip = strIPAdress;
		m_last_connected_port = nPort;
		return true;
	}
	else
	{
		return false;
	}

	if (::connect(m_hSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		DWORD dwErr = WSAGetLastError();
		//AfxMessageBox(_T(" Failed connect() \n"));
		::closesocket(m_hSocket);
		m_hSocket = NULL;
		return FALSE;
	}
	m_last_connected_ip = strIPAdress;
	m_last_connected_port = nPort;
	return TRUE;
}


//  bool Open_Socket2(CString strIPAdress,short nPort)
// {
// 	if(m_Commu_type==0)
// 		return false;
//
// 	int nNetTimeout=3000;//1 second.
// 	WSADATA wsaData;
// 	WORD sockVersion = MAKEWORD(2, 2);
//
// 	if (m_hSocket!=INVALID_SOCKET)
// 	{
// 		::closesocket(m_hSocket);
// 		m_hSocket=NULL;
// 	}
//
// 	if(::WSAStartup(sockVersion, &wsaData) != 0)
// 	{
// 		//AfxMessageBox(_T("Init Socket failed!"));
// 		m_hSocket=NULL;
// 		return FALSE;
// 	}
//
// 	m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 	if(m_hSocket == INVALID_SOCKET)
// 	{
// 		//	AfxMessageBox(_T("Create socket failed!"));
// 		m_hSocket=NULL;
// 		return FALSE;
// 	}
// 	sockaddr_in servAddr;
// 	servAddr.sin_family = AF_INET;
// 	servAddr.sin_port = htons(nPort);
// 	USES_CONVERSION;
// 	//char pTemp[20];
// 	//pTemp=W2A(strIPAdress);
//
//
// 	//servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
// 	//	servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
// 	servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
// 	//	u_long ul=1;
// 	//	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
// 	//发送时限
// 	setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
// 	//接收时限
// 	setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
// 	if(::connect(m_hSocket,(sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
// 	{
// 		DWORD dwErr = WSAGetLastError();
// 		//AfxMessageBox(_T(" Failed connect() \n"));
// 		::closesocket(m_hSocket);
// 		m_hSocket=NULL;
// 		return FALSE;
// 	}
// 	return TRUE;
////////////////////////////////////////////////////////////////////////////
// Connect 分为两种情况：
// 1，串口打开工作正常，但是设备通信失败（断电，掉线等等）。
// 表现为对串口操作正常，但是没有通信数据。
// 2，串口打开失败或工作不正常（USB串口拔出，占用等等）。
// 表现为无法操作串口。

 bool CModbus::is_connect()
{
	if (m_Commu_type == 0)
	{
		if (m_hSerial == NULL)
			return FALSE;
		else
			return TRUE;
	}
	else if (m_Commu_type == 1)
	{
		if (m_hSocket == NULL)
			return false;//disconnected
		else
			return true;//connected
	}
	return false; //add by Fance
}

//device_var:priduct ID,address:a regster;


 int CModbus::write_multi_Short(unsigned char device_var, unsigned short *to_write, unsigned short start_address, int length)
{
	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { 0 };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x10;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = 0;
		data_to_write[5] = length & 0xff;
		data_to_write[6] = length * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
		{
			data_to_write[7 + i * 2] = to_write[i] >> 8 & 0xff;
			data_to_write[7 + i * 2 + 1] = to_write[i] & 0xff;

		}


		TS_US crc = CRC16(data_to_write, 2 * length + 7);
		data_to_write[length * 2 + 7] = crc >> 8 & 0xff;
		data_to_write[length * 2 + 8] = crc & 0xff;

		CString traceStr = _T("Send:");
		CString temp;
		for (int i = 0; i<2 * length + 9; i++)
		{
			temp.Format(_T("%02X "), data_to_write[i]);
			traceStr += temp;
		}
		TRACE(_T("%s"), traceStr.GetBuffer());

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length * 2 + 9,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		traceStr = _T("Rev:");
		for (int i = 0; i<8; i++)
		{
			temp.Format(_T("%02X "), m_gval[i]);
			traceStr += temp;
		}
		TRACE(_T("%s"), traceStr);
		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
			if (m_gval[i] != *(data_to_write + i))
				return -2;
		crc = CRC16(m_gval, 6);
		if (m_gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (m_gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };

		data_to_write[0] = 1;
		data_to_write[1] = 2;
		data_to_write[2] = 3;
		data_to_write[3] = 4;
		data_to_write[4] = 5;
		data_to_write[5] = 6;

		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = 0;
		data_to_write[11] = length & 0xff;
		data_to_write[12] = length * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
		{
			data_to_write[13 + i * 2] = to_write[i] >> 8 & 0xff;
			data_to_write[13 + i * 2 + 1] = to_write[i] & 0xff;
		}


		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + 2 * length, 0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);
		if (nRecv<0)
		{
			return -2;
		}
		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}
 int CModbus::write_multi_Short_log(TS_UC device_var, TS_US *to_write, TS_US start_address, TS_US length,//参数命令部分
	unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, //发送和接受的原始数据
	int* sendDataLength, int* recvDataLength) //发送和接受数据长度
{
	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { 0 };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x10;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = length >> 8 & 0xff;
		data_to_write[5] = length & 0xff;
		data_to_write[6] = length * 2;
		//128 is better ,if you send more than 128, 
		//the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
		{
			//                 unsigned char UC_Value=to_write[i];
			//                 data_to_write[7+i*2] =UC_Value>>8&0xff;
			// 				data_to_write[7+i*2+1]=UC_Value& 0xff;


			data_to_write[7 + i * 2] = to_write[i] >> 8 & 0xff;
			data_to_write[7 + i * 2 + 1] = to_write[i] & 0xff;


		}


		TS_US crc = CRC16(data_to_write, 2 * length + 7);
		data_to_write[length * 2 + 7] = crc >> 8 & 0xff;
		data_to_write[length * 2 + 8] = crc & 0xff;

		//CString traceStr=_T("Send:");
		//CString temp;
		//for (int i=0;i<2*length+9;i++)
		//{
		//	temp.Format(_T("%02X "),data_to_write[i]);
		//	traceStr+=temp;
		//}
		*sendDataLength = 2 * length + 9;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		//	TRACE(_T("%s"),traceStr.GetBuffer());

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length * 2 + 9,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		//traceStr=_T("Rev:");
		//for(int i=0;i<8;i++){
		//	temp.Format(_T("%02X "),m_gval[i]);
		//	traceStr+=temp;
		//}
		//TRACE(_T("%s"),traceStr);
		*recvDataLength = 8;
		for (int i = 0; i<8; i++)
		{
			*((char*)put_revdata_into_here + i) = m_gval[i];
		}
		//  *((char*)put_senddate_into_here + i) = data_to_write[i];
		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
		{
			if (m_gval[i] != *(data_to_write + i))
				return -2;
		}
		crc = CRC16(m_gval, 6);
		if (m_gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (m_gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };

		data_to_write[0] = 1;
		data_to_write[1] = 2;
		data_to_write[2] = 3;
		data_to_write[3] = 4;
		data_to_write[4] = 5;
		data_to_write[5] = 6;

		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = 0;
		data_to_write[11] = length & 0xff;
		data_to_write[12] = length * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
		{
			data_to_write[13 + i * 2] = to_write[i] >> 8 & 0xff;
			data_to_write[13 + i * 2 + 1] = to_write[i] & 0xff;
		}

		*sendDataLength = 2 * length + 13;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}
		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + 2 * length, 0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);

		*recvDataLength = 13;
		for (int i = 0; i<13; i++)
		{
			*((char*)recvDataLength + i) = data_back_write[i];
		}
		if (nRecv<0)
		{
			return -2;
		}

		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}
 int CModbus::write_multi_Coil_log(TS_UC device_var, TS_BOOL *to_write, TS_US start_address, TS_US length,//参数命令部分
	unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, //发送和接受的原始数据
	int* sendDataLength, int* recvDataLength) //发送和接受数据长度
{
	unsigned short ToSendData[16];
	int DataLength = 0;
	if (length % 8 == 0)
	{
		DataLength = length / 8;

	}
	else
	{
		DataLength = length / 8 + 1;
	}
	bitset<8> BitReg;
	for (int index = 0; index< DataLength; index++)
	{
		//初始化BitReg
		for (int i = 0; i<8; i++)
		{
			BitReg.at(i) = false;
		}

		for (int i = 0; i < 8; i++)
		{
			if (index * 8 + i < length)
			{
				BitReg.at(i) = to_write[index * 8 + i];
			}

		}
		ToSendData[index] = BitReg.to_ulong();

	}

	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { 0 };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x0F;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = length >> 8 & 0xff;
		data_to_write[5] = length & 0xff;

		data_to_write[6] = DataLength * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time

		for (int i = 0; i<DataLength; i++)
		{



			data_to_write[7 + i * 2] = ToSendData[i] >> 8 & 0xff;
			data_to_write[7 + i * 2 + 1] = ToSendData[i] & 0xff;


		}

		TS_US crc = CRC16(data_to_write, 2 * DataLength + 7);
		data_to_write[DataLength * 2 + 7] = crc >> 8 & 0xff;
		data_to_write[DataLength * 2 + 8] = crc & 0xff;

		//CString traceStr=_T("Send:");
		//CString temp;
		//for (int i=0;i<2*length+9;i++)
		//{
		//	temp.Format(_T("%02X "),data_to_write[i]);
		//	traceStr+=temp;
		//}
		*sendDataLength = 2 * DataLength + 9;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		//	TRACE(_T("%s"),traceStr.GetBuffer());

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length * 2 + 9,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		//traceStr=_T("Rev:");
		//for(int i=0;i<8;i++){
		//	temp.Format(_T("%02X "),m_gval[i]);
		//	traceStr+=temp;
		//}
		//TRACE(_T("%s"),traceStr);
		*recvDataLength = 8;
		for (int i = 0; i<8; i++)
		{
			*((char*)put_revdata_into_here + i) = m_gval[i];
		}
		//  *((char*)put_senddate_into_here + i) = data_to_write[i];
		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
		{
			if (m_gval[i] != *(data_to_write + i))
				return -2;
		}
		crc = CRC16(m_gval, 6);
		if (m_gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (m_gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };

		data_to_write[0] = 1;
		data_to_write[1] = 2;
		data_to_write[2] = 3;
		data_to_write[3] = 4;
		data_to_write[4] = 5;
		data_to_write[5] = 6;

		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = length >> 8 & 0xff;;
		data_to_write[11] = length & 0xff;
		data_to_write[12] = DataLength * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time

		for (int i = 0; i < DataLength; i++)
		{
			data_to_write[13 + i * 2] = ToSendData[i] >> 8 & 0xff;
			data_to_write[13 + i * 2 + 1] = ToSendData[i] & 0xff;
		}
		*sendDataLength = 2 * DataLength + 13;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}
		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + 2 * length, 0);
		//Sleep(m_LATENCY_TIME_NET);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);

		*recvDataLength = 13;
		for (int i = 0; i<13; i++)
		{
			*((char*)recvDataLength + i) = data_back_write[i];
		}
		if (nRecv<0)
		{
			return -2;
		}

		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}

 int CModbus::Read_One(TS_UC device_var, TS_US address)
{

	//	CSingleLock singlock(&scan_mutex);
	//	singlock.Lock();
	int nTemp = -1;
	if (m_Commu_type == 0)//serial
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
		crc = CRC16(m_pval, 6);
		m_pval[6] = (crc >> 8) & 0xff;
		m_pval[7] = crc & 0xff;
		if (m_hSerial == NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		BOOL bRet = ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;

		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,	// 数据缓冲区地址
			8,		// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_PENDING)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		//////////////////////////////////////////the message had send ,now to read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;

		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				11,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				7,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;
			crc = CRC16(m_gval, 5);
			if (m_gval[5] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[6] != (crc & 0xff))
				return -2;
		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				crc = CRC16(m_gval, 9);
				if (m_gval[9] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[10] != (crc & 0xff))
					return -2;
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];
				//stay serialnumber
				//			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)//2
					return -2;
				crc = CRC16(m_gval, 5);
				if (m_gval[5] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[6] != (crc & 0xff))
					return -2;
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (nTemp * 256 + m_gval[4]);
	}
	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];

		/*
		struct DataPack
		{
		TS_UC  empty[6];
		TS_UC  m_pval[13];

		};
		*/

		TS_UC data[12];

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;
		data[6] = device_var;
		data[7] = 3;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		data[10] = 0;
		data[11] = 1;

		//		DataPack dataInfo;
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//TS_US crc;
						//DWORD m_had_send_data_number;//已经发送的数据的字节数

		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
					//	crc = CRC16(m_pval,6);
					//	m_pval[6] = (crc >>8) & 0xff;
					//	m_pval[7] = crc & 0xff;

					//memcpy((char*)&data[6],(char*)&m_pval[0],sizeof(m_pval));

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int nLen = sizeof(data);
		//int xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		int xx = ::send(m_hSocket, (char*)&data, sizeof(data), 0);

		if (xx < 0)
		{
			if (m_last_connected_port != 0)
				Open_Socket2(m_last_connected_ip, m_last_connected_port);
		}
		////////////////////////////////////////////////clear com error
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
		}

		Sleep(m_LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
								//DataPack rvData;
								//Sleep(SLEEP_TIME+10);

		TS_UC rvData[17];
		//int nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);
		int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
		//m_gval=rvData.m_pval;
		memcpy((void*)m_gval, (void*)&rvData[6], 13/*sizeof(m_gval)*/);

		/*
		xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);*/

		int nerro = WSAGetLastError();

		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0/* && m_gval[5]==0 && m_gval[6]==0*/)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;

			/*;
			crc=CRC16(m_gval,5);
			if(m_gval[5]!=((crc>>8)&0xff))
			return -2;
			if(m_gval[6]!=(crc & 0xff))
			return -2;
			*/

		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				/*
				crc=CRC16(m_gval,9);
				if(m_gval[9]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[10]!=(crc & 0xff))
				return -2;
				*/
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_gval[1] || m_gval[2] != 2)//2
					return -2;
				/*
				crc=CRC16(m_gval,5);
				if(m_gval[5]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[6]!=(crc & 0xff))
				return -2;
				*/
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (m_gval[3] * 256 + m_gval[4]);
	}
	return -1; //add by Fance
			   //	singlock.Unlock();
}

 int CModbus::Read_One_log(TS_UC device_var, TS_US address, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{

	//	CSingleLock singlock(&scan_mutex);
	//	singlock.Lock();
	int nTemp = -1;
	if (m_Commu_type == 0)//serial
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
		crc = CRC16(m_pval, 6);
		m_pval[6] = (crc >> 8) & 0xff;
		m_pval[7] = crc & 0xff;
		if (m_hSerial == NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		BOOL bRet = ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;

		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = m_pval[i];
		}

		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,	// 数据缓冲区地址
			8,		// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_PENDING)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		//////////////////////////////////////////the message had send ,now to read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;

		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				11,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				7,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = m_gval[i];
		}

		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;
			crc = CRC16(m_gval, 5);
			if (m_gval[5] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[6] != (crc & 0xff))
				return -2;
		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				crc = CRC16(m_gval, 9);
				if (m_gval[9] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[10] != (crc & 0xff))
					return -2;
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)//2
					return -2;
				crc = CRC16(m_gval, 5);
				if (m_gval[5] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[6] != (crc & 0xff))
					return -2;
			}
		}

		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (nTemp * 256 + m_gval[4]);
	}
	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];

		/*
		struct DataPack
		{
		TS_UC  empty[6];
		TS_UC  m_pval[13];

		};
		*/

		TS_UC data[12];

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;



		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}



		data[0] = g_data_to_send[0];
		data[1] = g_data_to_send[1];
		data[2] = g_data_to_send[2];
		data[3] = g_data_to_send[3];
		data[4] = g_data_to_send[4];
		data[5] = g_data_to_send[5];


		data[6] = device_var;
		data[7] = 3;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		data[10] = 0;
		data[11] = 1;

		//		DataPack dataInfo;
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//TS_US crc;
						//DWORD m_had_send_data_number;//已经发送的数据的字节数

		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
					//	crc = CRC16(m_pval,6);
					//	m_pval[6] = (crc >>8) & 0xff;
					//	m_pval[7] = crc & 0xff;

					//memcpy((char*)&data[6],(char*)&m_pval[0],sizeof(m_pval));

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int nLen = sizeof(data);
		//int xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		*sendDataLength = nLen;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data[i];
		}

		int xx = ::send(m_hSocket, (char*)&data, sizeof(data), 0);


		////////////////////////////////////////////////clear com error
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
		}

		Sleep(m_LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
								//DataPack rvData;
								//Sleep(SLEEP_TIME+10);

		TS_UC rvData[17];
		//int nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);
		int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
		//m_gval=rvData.m_pval;
		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = rvData[i];
		}

		memcpy((void*)m_gval, (void*)&rvData[6], 13/*sizeof(m_gval)*/);

		/*
		xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);*/

		int nerro = WSAGetLastError();

		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0/* && m_gval[5]==0 && m_gval[6]==0*/)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;

			/*;
			crc=CRC16(m_gval,5);
			if(m_gval[5]!=((crc>>8)&0xff))
			return -2;
			if(m_gval[6]!=(crc & 0xff))
			return -2;
			*/

		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				/*
				crc=CRC16(m_gval,9);
				if(m_gval[9]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[10]!=(crc & 0xff))
				return -2;
				*/
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_gval[1] || m_gval[2] != 2)//2
					return -2;
				/*
				crc=CRC16(m_gval,5);
				if(m_gval[5]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[6]!=(crc & 0xff))
				return -2;
				*/
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (m_gval[3] * 256 + m_gval[4]);
	}
	return -1; //add by Fance
			   //	singlock.Unlock();
}

 int CModbus::Write_One(TS_UC device_var, TS_US address, TS_US val)
{
	if (m_Commu_type == 0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo
		if (address != 10)
		{
			crc = CRC16(m_pval, 6);
			m_pval[6] = (crc >> 8) & 0xff;
			m_pval[7] = crc & 0xff;
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				crc = CRC16(m_pval, 6);
				m_pval[6] = (crc >> 8) & 0xff;
				m_pval[7] = crc & 0xff;
			}
			else
			{
				m_pval[6] = m_serinumber_in_dll[0];
				m_pval[7] = m_serinumber_in_dll[1];
				m_pval[8] = m_serinumber_in_dll[2];
				m_pval[9] = m_serinumber_in_dll[3];
				crc = CRC16(m_pval, 10);
				m_pval[10] = (crc >> 8) & 0xff;
				m_pval[11] = crc & 0xff;
			}
		}

		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		if (address != 10)
		{
			fState = WriteFile(m_hSerial,// 句柄
				m_pval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osWrite);
		}
		else
		{
			//==10 register
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
			else
			{
				//new protocal
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address != 10)
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
			else
			{
				//new protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			for (int i = 0; i<8; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<8; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0 && m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0)
					return -3;
				for (int i = 0; i<12; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}
		return 1;
	}

	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//TS_UC data[12];
		TS_UC data[16];
		// 		TS_UC* data=NULL;
		int nSendNum = 12;
		if (address == 10)
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

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		data[6] = device_var;
		data[7] = 6;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			data[10] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			data[10] = (val >> 8) & 0xff;//number hi
		data[11] = val & 0xff;//number lo

		if (address == 10)
		{

			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
			}
			else
			{
				//* uncommend by zgq; we use net work scan tstat through NC
				data[12] = m_serinumber_in_dll[0];
				data[13] = m_serinumber_in_dll[1];
				data[14] = m_serinumber_in_dll[2];
				data[15] = m_serinumber_in_dll[3];
				//*/
			}
		}


		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//	TS_US crc;
						//	DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo

							 /*
							 for(int i=0;i<=11;i++)
							 m_gval[i]=0;/////////////////////////////////////////clear buffer
							 TS_US crc;
							 DWORD m_had_send_data_number;//已经发送的数据的字节数
							 m_pval[0] = device_var;
							 m_pval[1] = 6;
							 m_pval[2] = address>>8 & 0xFF ;
							 m_pval[3] = address & 0xFF;
							 if(address==10 && (m_serinumber_in_dll[0]!=0 || m_serinumber_in_dll[1]!=0 || m_serinumber_in_dll[2]!=0 || m_serinumber_in_dll[3]!=0))
							 m_pval[4]=0x55;/////////////////////////////new protocal or write_one 10
							 else
							 m_pval[4] = (val>>8) & 0xff;//number hi
							 m_pval[5] = val & 0xff;//number lo
							 if(address!=10)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 if(m_serinumber_in_dll[0]==0 && m_serinumber_in_dll[1]==0 && m_serinumber_in_dll[2]==0 && m_serinumber_in_dll[3]==0)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 m_pval[6]=m_serinumber_in_dll[0];
							 m_pval[7]=m_serinumber_in_dll[1];
							 m_pval[8]=m_serinumber_in_dll[2];
							 m_pval[9]=m_serinumber_in_dll[3];
							 crc = CRC16(m_pval,10);
							 m_pval[10] = (crc >>8) & 0xff;
							 m_pval[11] = crc & 0xff;
							 }
							 }
							 */
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		//::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);
		if (nRet < 0)	//Add by Fance ,if device is dosconnected , we need to connect the device again;
		{
			if (m_last_connected_port != 0)
				Open_Socket2(m_last_connected_ip, m_last_connected_port);
		}
		//Sleep(SLEEP_TIME+10);
		Sleep(m_LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		int nErr = WSAGetLastError();

		memcpy((void*)&m_gval[0], (void*)&rvdata[6], 13/*sizeof(m_gval)*/);
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			for (int i = 0; i<5; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0/* && m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0 && m_gval[8]==0 && m_gval[9]==0 && m_gval[10]==0 && m_gval[11]==0*/)
					return -3;
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}

		//delete []data;
		return 1;
	}
	return -1;//add by Fance
			  ///////////////////////////////////////////////////////////
}

 int CModbus::Write_One_log(TS_UC device_var, TS_US address, TS_US val, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
	if (m_Commu_type == 0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo
		if (address != 10)
		{
			crc = CRC16(m_pval, 6);
			m_pval[6] = (crc >> 8) & 0xff;
			m_pval[7] = crc & 0xff;
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				crc = CRC16(m_pval, 6);
				m_pval[6] = (crc >> 8) & 0xff;
				m_pval[7] = crc & 0xff;
			}
			else
			{
				m_pval[6] = m_serinumber_in_dll[0];
				m_pval[7] = m_serinumber_in_dll[1];
				m_pval[8] = m_serinumber_in_dll[2];
				m_pval[9] = m_serinumber_in_dll[3];
				crc = CRC16(m_pval, 10);
				m_pval[10] = (crc >> 8) & 0xff;
				m_pval[11] = crc & 0xff;
			}
		}

		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		if (address != 10)
		{
			*sendDataLength = 8;
			for (int i = 0; i < *sendDataLength; i++)
			{
				*((char*)put_senddate_into_here + i) = m_pval[i];
			}

			fState = WriteFile(m_hSerial,// 句柄
				m_pval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osWrite);
		}
		else
		{
			//==10 register
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				*sendDataLength = 8;
				for (int i = 0; i < *sendDataLength; i++)
				{
					*((char*)put_senddate_into_here + i) = m_pval[i];
				}
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
			else
			{
				//new protocal
				*sendDataLength = 12;
				for (int i = 0; i < *sendDataLength; i++)
				{
					*((char*)put_senddate_into_here + i) = m_pval[i];
				}

				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}

		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address != 10)
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
			else
			{
				//new protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = m_gval[i];
		}

		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			for (int i = 0; i<8; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<8; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0 && m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0)
					return -3;
				for (int i = 0; i<12; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}
		return 1;
	}

	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//TS_UC data[12];
		TS_UC data[16];
		// 		TS_UC* data=NULL;
		int nSendNum = 12;
		if (address == 10)
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


		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}



		data[0] = g_data_to_send[0];
		data[1] = g_data_to_send[1];
		data[2] = g_data_to_send[2];
		data[3] = g_data_to_send[3];
		data[4] = g_data_to_send[4];
		data[5] = g_data_to_send[5];
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		data[6] = device_var;
		data[7] = 6;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			data[10] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			data[10] = (val >> 8) & 0xff;//number hi
		data[11] = val & 0xff;//number lo

		if (address == 10)
		{

			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
			}
			else
			{
				//* uncommend by zgq; we use net work scan tstat through NC
				data[12] = m_serinumber_in_dll[0];
				data[13] = m_serinumber_in_dll[1];
				data[14] = m_serinumber_in_dll[2];
				data[15] = m_serinumber_in_dll[3];
				//*/
			}
		}


		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//	TS_US crc;
						//	DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo

							 /*
							 for(int i=0;i<=11;i++)
							 m_gval[i]=0;/////////////////////////////////////////clear buffer
							 TS_US crc;
							 DWORD m_had_send_data_number;//已经发送的数据的字节数
							 m_pval[0] = device_var;
							 m_pval[1] = 6;
							 m_pval[2] = address>>8 & 0xFF ;
							 m_pval[3] = address & 0xFF;
							 if(address==10 && (m_serinumber_in_dll[0]!=0 || m_serinumber_in_dll[1]!=0 || m_serinumber_in_dll[2]!=0 || m_serinumber_in_dll[3]!=0))
							 m_pval[4]=0x55;/////////////////////////////new protocal or write_one 10
							 else
							 m_pval[4] = (val>>8) & 0xff;//number hi
							 m_pval[5] = val & 0xff;//number lo
							 if(address!=10)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 if(m_serinumber_in_dll[0]==0 && m_serinumber_in_dll[1]==0 && m_serinumber_in_dll[2]==0 && m_serinumber_in_dll[3]==0)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 m_pval[6]=m_serinumber_in_dll[0];
							 m_pval[7]=m_serinumber_in_dll[1];
							 m_pval[8]=m_serinumber_in_dll[2];
							 m_pval[9]=m_serinumber_in_dll[3];
							 crc = CRC16(m_pval,10);
							 m_pval[10] = (crc >>8) & 0xff;
							 m_pval[11] = crc & 0xff;
							 }
							 }
							 */
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		//::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		*sendDataLength = nSendNum;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data[i];
		}

		int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);

		//Sleep(SLEEP_TIME+10);
		Sleep(m_LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = rvdata[i];
		}

		int nErr = WSAGetLastError();

		memcpy((void*)&m_gval[0], (void*)&rvdata[6], 13/*sizeof(m_gval)*/);
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			for (int i = 0; i<5; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0/* && m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0 && m_gval[8]==0 && m_gval[9]==0 && m_gval[10]==0 && m_gval[11]==0*/)
					return -3;
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}

		//delete []data;
		return 1;
	}
	return -1;//add by Fance
			  ///////////////////////////////////////////////////////////
}
//MF 0x05 Write Coil
 int CModbus::Write_Coil_log(TS_UC device_var, TS_US address, TS_BOOL val, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
	if (m_Commu_type == 0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 5;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (val)
		{
			m_pval[4] = 0xFF;
		}
		else
		{
			m_pval[4] = 0x00;
		}
		m_pval[5] = 0x00;

		crc = CRC16(m_pval, 6);
		m_pval[6] = (crc >> 8) & 0xff;
		m_pval[7] = crc & 0xff;


		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);

		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = m_pval[i];
		}

		fState = WriteFile(m_hSerial,// 句柄
			m_pval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);


		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}

		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);

		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);


		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = m_gval[i];
		}


		if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
			return -3;
		for (int i = 0; i<8; i++)
			if (m_gval[i] != m_pval[i])
				return -2;


		return 1;
	}

	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//TS_UC data[12];
		TS_UC data[16];
		// 		TS_UC* data=NULL;
		int nSendNum = 12;
		if (address == 10)
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


		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}



		data[0] = g_data_to_send[0];
		data[1] = g_data_to_send[1];
		data[2] = g_data_to_send[2];
		data[3] = g_data_to_send[3];
		data[4] = g_data_to_send[4];
		data[5] = g_data_to_send[5];
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		data[6] = device_var;
		data[7] = 5;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;


		if (val)
		{
			data[10] = 0xFF;
		}
		else
		{
			data[10] = 0x00;
		}
		data[11] = 0x00;

		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;
		/////////////////////////////////////////clear buffer
		//	TS_US crc;
		//	DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 5;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (val)
		{
			m_pval[4] = 0xFF;
		}
		else
		{
			m_pval[4] = 0x00;
		}
		m_pval[5] = 0x00;

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		//::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		*sendDataLength = nSendNum;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data[i];
		}

		int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);

		//Sleep(SLEEP_TIME+10);
		Sleep(m_LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = rvdata[i];
		}

		int nErr = WSAGetLastError();

		memcpy((void*)&m_gval[0], (void*)&rvdata[6], 13/*sizeof(m_gval)*/);

		if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0)
			return -3;
		for (int i = 0; i<5; i++)
			if (m_gval[i] != m_pval[i])
				return -2;




		return 1;
	}
	return -1;//add by Fance
			  ///////////////////////////////////////////////////////////
}

#if 1
 int CModbus::read_multi(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length)
{
	if (m_Commu_type == 0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = device_var;//slave address
		data_to_send[1] = 3;//function multiple
		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[3] = start_address & 0xff;//starting address lo
		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5] = length & 0xff;//quantity of registers lo
		TS_US crc = CRC16(data_to_send, 6);
		data_to_send[6] = (crc >> 8) & 0xff;
		data_to_send[7] = crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message



		int fState = WriteFile(m_hSerial,// 句柄
			data_to_send,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(m_LATENCY_TIME_COM);
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			to_send_data,// 数据缓冲区地址
			length * 2 + 5,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		crc = CRC16(to_send_data, length * 2 + 3);
		if (to_send_data[length * 2 + 3] != ((crc >> 8) & 0xff))
			return -2;
		if (to_send_data[length * 2 + 4] != (crc & 0xff))
			return -2;
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
		return length;
	}
	if (m_Commu_type == 1)//tcp.
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

		DWORD m_had_send_data_number;//已经发送的数据的字节数
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

		TS_UC to_send_data[700] = { '\0' };
		TS_UC to_Reive_data[700] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = 1;
		data_to_send[1] = 2;
		data_to_send[2] = 3;
		data_to_send[3] = 4;
		data_to_send[4] = 5;
		data_to_send[5] = 6;



		data_to_send[6] = device_var;//slave address
		data_to_send[7] = 3;//function multiple
		data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[9] = start_address & 0xff;//starting address lo
		data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11] = length & 0xff;//quantity of registers lo

										 //TS_US crc=CRC16(data_to_send,6);
										 //data_to_send[6]=(crc>>8) & 0xff;
										 //data_to_send[7]=crc & 0xff;/

										 //		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

		Sleep(m_LATENCY_TIME_NET);
		int nn = sizeof(to_Reive_data);

		unsigned long bytes_available;
		ioctlsocket(m_hSocket, FIONREAD, &bytes_available);
		if (bytes_available == 0) return -1;
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

		memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		/*
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
		return -2;

		if(to_send_data[length*2+4]!=(crc & 0xff))
		return -2;
		*/
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
		return length;
	}
	return -1;
}

#endif

 int CModbus::read_multi_log(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, TS_US length,
	unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here,
	int* sendDataLength, int* recvDataLength)
{
	if (m_Commu_type == 0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = device_var;//slave address
		data_to_send[1] = 3;//function multiple
		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[3] = start_address & 0xff;//starting address lo
		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5] = length & 0xff;//quantity of registers lo
		TS_US crc = CRC16(data_to_send, 6);
		data_to_send[6] = (crc >> 8) & 0xff;
		data_to_send[7] = crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;

		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}

		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_send,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(m_LATENCY_TIME_COM);
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -3;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			to_send_data,// 数据缓冲区地址
			length * 2 + 5,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_send_data[i];
		}

		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -4;
		crc = CRC16(to_send_data, length * 2 + 3);
		if (to_send_data[length * 2 + 3] != ((crc >> 8) & 0xff))
			return -5;
		if (to_send_data[length * 2 + 4] != (crc & 0xff))
			return -5;
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];


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
	if (m_Commu_type == 1)//tcp.
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

		DWORD m_had_send_data_number;//已经发送的数据的字节数
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

		TS_UC to_send_data[700] = { '\0' };
		TS_UC to_Reive_data[700] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}



		data_to_send[0] = g_data_to_send[0];
		data_to_send[1] = g_data_to_send[1];

		data_to_send[2] = g_data_to_send[2];
		data_to_send[3] = g_data_to_send[3];

		data_to_send[4] = g_data_to_send[4];
		data_to_send[5] = g_data_to_send[5];



		data_to_send[6] = device_var;//slave address
		data_to_send[7] = 3;//function multiple
		data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[9] = start_address & 0xff;//starting address lo
		data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11] = length & 0xff;//quantity of registers lo

										 //TS_US crc=CRC16(data_to_send,6);
										 //data_to_send[6]=(crc>>8) & 0xff;
										 //data_to_send[7]=crc & 0xff;/

										 //		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		*sendDataLength = sizeof(data_to_send);
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}

		::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

		Sleep(m_LATENCY_TIME_NET);
		int nn = sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_Reive_data[i];
		}

		memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		/*
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
		return -2;

		if(to_send_data[length*2+4]!=(crc & 0xff))
		return -2;
		*/
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
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

 int CModbus::write_multi(TS_UC device_var, TS_UC *to_write, TS_US start_address, TS_US length)
{
	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x10;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = 0;
		data_to_write[5] = length;
		data_to_write[6] = length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[7 + i] = to_write[i];
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;

		TS_US crc = CRC16(data_to_write, length + 7);
		data_to_write[length + 7] = crc >> 8 & 0xff;
		data_to_write[length + 8] = crc & 0xff;

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length + 9,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
			if (m_gval[i] != *(data_to_write + i))
				return -2;
		crc = CRC16(m_gval, 6);
		if (m_gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (m_gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };

		data_to_write[0] = 1;
		data_to_write[1] = 2;
		data_to_write[2] = 3;
		data_to_write[3] = 4;
		data_to_write[4] = 5;
		data_to_write[5] = 6;

		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = 0;
		data_to_write[11] = length;
		data_to_write[12] = length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[13 + i] = to_write[i];


		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + length, 0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);
		if (nRecv<0)
		{
			return -2;
		}
		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}

 int CModbus::write_multi_log(TS_UC device_var, TS_UC *to_write, TS_US start_address, TS_US length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x10;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = 0;
		data_to_write[5] = length;
		data_to_write[6] = length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[7 + i] = to_write[i];
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;

		TS_US crc = CRC16(data_to_write, length + 7);
		data_to_write[length + 7] = crc >> 8 & 0xff;
		data_to_write[length + 8] = crc & 0xff;

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;

		*sendDataLength = length + 9;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length + 9,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = m_gval[i];
		}

		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
			if (m_gval[i] != *(data_to_write + i))
				return -2;
		crc = CRC16(m_gval, 6);
		if (m_gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (m_gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };


		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}



		data_to_write[0] = g_data_to_send[0];
		data_to_write[1] = g_data_to_send[1];
		data_to_write[2] = g_data_to_send[2];
		data_to_write[3] = g_data_to_send[3];
		data_to_write[4] = g_data_to_send[4];
		data_to_write[5] = g_data_to_send[5];



		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = 0;
		data_to_write[11] = length;
		data_to_write[12] = length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[13 + i] = to_write[i];


		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		*sendDataLength = 13 + length;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + length, 0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = data_back_write[i];
		}

		if (nRecv<0)
		{
			return -2;
		}
		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}

 int CModbus::NetController_CheckTstatOnline(TS_UC devLo, TS_UC devHi)
{
	if (m_Commu_type == 0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address

		//Sleep(150);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSerial == NULL)
		{
			return -1;
		}
		int the_return_value;
		int the_return_value2 = 0;
		the_return_value = NetController_CheckTstatOnline2(devLo, devHi);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = NetController_CheckTstatOnline2(devLo, devHi);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = NetController_CheckTstatOnline2(devLo, devHi);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
			if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
				return the_return_value;
			int i = 0;
			do
			{
				the_return_value = NetController_CheckTstatOnline2(devLo, devHi);
				if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
					return the_return_value;
				else if (the_return_value>0)
					i++;
			} while (i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if (m_Commu_type == 1)//tcp
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		int the_return_value;
		int the_return_value2 = 0;
		the_return_value = NetController_CheckTstatOnline2(devLo, devHi);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = NetController_CheckTstatOnline2(devLo, devHi);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = NetController_CheckTstatOnline2(devLo, devHi);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
			if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
				return the_return_value;
			int i = 0;
			do
			{
				the_return_value = NetController_CheckTstatOnline2(devLo, devHi);
				if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
					return the_return_value;
				else if (the_return_value>0)
					i++;
			} while (i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	return -1;
}

 int CModbus::NetController_CheckTstatOnline2(TS_UC devLo, TS_UC devHi)
{
	if (m_Commu_type == 0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address

		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_UC  m_pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = 255;
		m_pval[1] = 26;  //put comments here,
		m_pval[2] = devHi;
		m_pval[3] = devLo;
		crc = CRC16(m_pval, 4);
		m_pval[4] = (crc >> 8) & 0xff;
		m_pval[5] = crc & 0xff;

		if (m_hSerial == NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear buffer
		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,// 数据缓冲区地址
			6,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)//不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,m_pval[0],m_pval[1],m_pval[2],m_pval[3],m_pval[4],m_pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(m_LATENCY_TIME_COM);//SLEEP_TIME);//because that scan have a delay lower 75ms
								//	Sleep(300);
								/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			13,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		//CloseHandle(m_osRead.hEvent);
		/*
		CStdioFile default_file;
		CString saved_path="C:\\modbus_scan_data.txt";
		CString a_line;
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("T:%x %x %x %x %x %x",m_pval[0],m_pval[1],m_pval[2],m_pval[3],m_pval[4],m_pval[5]);
		default_file.WriteString(a_line+"\n");
		a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}
		*/
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 26))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc = CRC16(m_gval, 3);
			if ((m_gval[3] != ((crc >> 8) & 0xff)) || (m_gval[4] != (crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect
				return -3;
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 25))
				return -2;
			crc = CRC16(m_gval, 7);
			if (m_gval[7] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[8] != (crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,m_gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",m_gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		return m_gval[2];
	}

	if (m_Commu_type == 1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect

		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_UC  m_pval[10];
		//		TS_US crc;
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数


		m_pval[0] = 1;
		m_pval[1] = 2;
		m_pval[2] = 3;
		m_pval[3] = 4;
		m_pval[4] = 5;
		m_pval[5] = 6;
		m_pval[6] = 255;
		m_pval[7] = 26;
		m_pval[8] = devHi;
		m_pval[9] = devLo;
		/*
		crc = CRC16(m_pval,4);
		m_pval[4] = (crc >>8) & 0xff;
		m_pval[5] = crc & 0xff;
		*/
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		::send(m_hSocket, (char*)m_pval, sizeof(m_pval), 0);
		Sleep(m_LATENCY_TIME_NET);//Sleep(SLEEP_TIME+8);

		TS_UC  rvdata[19];
		for (int i = 0; i<19; i++)
			rvdata[i] = 0;
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
		if (nRecv>0)
		{
			//异常:
			memcpy(m_gval, (void*)&rvdata[6], sizeof(rvdata) - 6);
		}
		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != 255) || (m_gval[1] != 26))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect
				return -3;
			if ((m_gval[0] != 255) || (m_gval[1] != 26))
				return -2;
		}

		/*
		m_old_or_new_scan_protocal_in_dll=1;

		if((m_gval[0]!=m_pval[6]) || (m_gval[1]!=26))
		return -2;
		if(m_gval[0]==0 && m_gval[1]==0 && m_gval[2]==0 && m_gval[3]==0)
		{
		//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
		return -4;              //no response ,no connection
		}
		*/
		return m_gval[2];
	}
	return -1;
}

//also return the serial communication handle;
 HANDLE CModbus::GetCommunicationHandle()
{

	/*
	if(m_Commu_type==1&&m_hSocket!=NULL)
	return m_hSocket;
	else
	return NULL;
	*/
	if (m_Commu_type == 1)
		return (HANDLE)m_hSocket;
	if (m_Commu_type == 0)
		return m_hSerial;

	return NULL;
}

 BOOL CModbus::bTCPDisconnected()
{
	return TRUE;
}

 void CModbus::SetComnicationHandle(int nType, HANDLE hCommunication)
{
	close_com();
	if (m_Commu_type == 0)
	{

	}
	if (m_Commu_type == 1)
	{
		m_hSocket = (SOCKET)hCommunication;
	}

}

 bool CModbus::open_com(int m_com)
{
	//open com ,if you want to open "com1",the m_com equal 0;if you want to open "com2",the m_com equal 1
	//you will get the handle to com,m_hSerial,is a extern variable
	//the return value ,true is ok,false is failure
	if (m_open_com_port_number_in_dll == m_com)
	{
		Change_BaudRate(19200);
		return true;///////////////////////////same com port ,opened by multi times,it's badly.
	}
	if (m_hSerial != NULL)
	{
		//关闭串口
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	/////////////////////////////////////////////////////////////////////加入的串口通信部分
	// 	LPCSTR lpComNum[6];
	// 	ZeroMemory(lpComNum, )

	CString strCom;
	if (m_com <= 9 && m_com >0)
	{
		strCom.Format(_T("%d"), m_com);
		strCom = _T("COM") + strCom + _T(":");
	}
	else if (m_com > 9)
	{
		strCom.Format(_T("%d"), m_com);
		strCom = _T("\\\\.\\COM") + strCom;
	}

	m_hSerial = CreateFile(strCom, //strCom,//串口句柄，打开串口
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,是另外的形式，表示的是异步通信，能同时读写;0为同步读写
		NULL);


	if (m_hSerial == INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
		return false;
	}
	//if(!SetupComm(m_hSerial, 1024*32, 1024*9))
	if (!SetupComm(m_hSerial, 1024 * 64, 1024 * 32))
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
	DCB  PortDCB;
	PortDCB.DCBlength = sizeof(DCB);
	// 默认串口参数
	if (!GetCommState(m_hSerial, &PortDCB))
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
	if (!SetCommState(m_hSerial, &PortDCB))
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
	m_open_com_port_number_in_dll = m_com;
	m_baudrate_in_dll = 19200;

	return true;
}

 int CModbus::Read_One2(TS_UC device_var, TS_US address, bool bComm_Type)
{

	//	CSingleLock singlock(&scan_mutex);
	//	singlock.Lock();
	int nTemp = -1;
	if (bComm_Type == 0)//serial
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
		crc = CRC16(m_pval, 6);
		m_pval[6] = (crc >> 8) & 0xff;
		m_pval[7] = crc & 0xff;
		if (m_hSerial == NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;

		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)// I/O未完成或失败
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_PENDING)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		//////////////////////////////////////////the message had send ,now to read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;

		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				11,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				7,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;
			crc = CRC16(m_gval, 5);
			if (m_gval[5] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[6] != (crc & 0xff))
				return -2;
		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				crc = CRC16(m_gval, 9);
				if (m_gval[9] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[10] != (crc & 0xff))
					return -2;
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)//2
					return -2;
				crc = CRC16(m_gval, 5);
				if (m_gval[5] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[6] != (crc & 0xff))
					return -2;
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (nTemp * 256 + m_gval[4]);
	}
	if (bComm_Type == 1)//tcp.
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];

		/*
		struct DataPack
		{
		TS_UC  empty[6];
		TS_UC  m_pval[13];

		};
		*/

		TS_UC data[12];

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;
		data[6] = device_var;
		data[7] = 3;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		data[10] = 0;
		data[11] = 1;

		//		DataPack dataInfo;
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//		TS_US crc;
						//		DWORD m_had_send_data_number;//已经发送的数据的字节数

		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
					//	crc = CRC16(m_pval,6);
					//	m_pval[6] = (crc >>8) & 0xff;
					//	m_pval[7] = crc & 0xff;

					//memcpy((char*)&data[6],(char*)&m_pval[0],sizeof(m_pval));

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int nLen = sizeof(data);
		//int xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		int xx = ::send(m_hSocket, (char*)&data, sizeof(data), 0);


		////////////////////////////////////////////////clear com error
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
		}

		Sleep(m_LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
								//DataPack rvData;
								//Sleep(SLEEP_TIME+10);

		TS_UC rvData[17];
		//int nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);
		int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
		//m_gval=rvData.m_pval;
		memcpy((void*)m_gval, (void*)&rvData[6], 13/*sizeof(m_gval)*/);

		/*
		xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);*/

		int nerro = WSAGetLastError();

		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0/* && m_gval[5]==0 && m_gval[6]==0*/)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;

			/*;
			crc=CRC16(m_gval,5);
			if(m_gval[5]!=((crc>>8)&0xff))
			return -2;
			if(m_gval[6]!=(crc & 0xff))
			return -2;
			*/

		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				/*
				crc=CRC16(m_gval,9);
				if(m_gval[9]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[10]!=(crc & 0xff))
				return -2;
				*/
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_gval[1] || m_gval[2] != 2)//2
					return -2;
				/*
				crc=CRC16(m_gval,5);
				if(m_gval[5]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[6]!=(crc & 0xff))
				return -2;
				*/
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (m_gval[3] * 256 + m_gval[4]);
	}
	return -1;
	//	singlock.Unlock();
}

 int CModbus::Write_One2(TS_UC device_var, TS_US address, TS_US val, bool bComm_Type)
{
	if (bComm_Type == 0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo
		if (address != 10)
		{
			crc = CRC16(m_pval, 6);
			m_pval[6] = (crc >> 8) & 0xff;
			m_pval[7] = crc & 0xff;
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				crc = CRC16(m_pval, 6);
				m_pval[6] = (crc >> 8) & 0xff;
				m_pval[7] = crc & 0xff;
			}
			else
			{
				m_pval[6] = m_serinumber_in_dll[0];
				m_pval[7] = m_serinumber_in_dll[1];
				m_pval[8] = m_serinumber_in_dll[2];
				m_pval[9] = m_serinumber_in_dll[3];
				crc = CRC16(m_pval, 10);
				m_pval[10] = (crc >> 8) & 0xff;
				m_pval[11] = crc & 0xff;
			}
		}

		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		if (address != 10)
		{
			fState = WriteFile(m_hSerial,// 句柄
				m_pval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osWrite);
		}
		else
		{
			//==10 register
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
			else
			{
				//new protocal
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address != 10)
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
			else
			{
				//new protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			for (int i = 0; i<8; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<8; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0 && m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0)
					return -3;
				for (int i = 0; i<12; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}
		return 1;
	}

	if (bComm_Type == 1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//TS_UC data[12];
		TS_UC data[16];
		// 		TS_UC* data=NULL;
		int nSendNum = 12;
		if (address == 10)
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

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		data[6] = device_var;
		data[7] = 6;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			data[10] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			data[10] = (val >> 8) & 0xff;//number hi
		data[11] = val & 0xff;//number lo

		if (address == 10)
		{

			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
			}
			else
			{
				//* uncommend by zgq; we use net work scan tstat through NC
				data[12] = m_serinumber_in_dll[0];
				data[13] = m_serinumber_in_dll[1];
				data[14] = m_serinumber_in_dll[2];
				data[15] = m_serinumber_in_dll[3];
				//*/
			}
		}


		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//		TS_US crc;
						//	DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo

							 /*
							 for(int i=0;i<=11;i++)
							 m_gval[i]=0;/////////////////////////////////////////clear buffer
							 TS_US crc;
							 DWORD m_had_send_data_number;//已经发送的数据的字节数
							 m_pval[0] = device_var;
							 m_pval[1] = 6;
							 m_pval[2] = address>>8 & 0xFF ;
							 m_pval[3] = address & 0xFF;
							 if(address==10 && (m_serinumber_in_dll[0]!=0 || m_serinumber_in_dll[1]!=0 || m_serinumber_in_dll[2]!=0 || m_serinumber_in_dll[3]!=0))
							 m_pval[4]=0x55;/////////////////////////////new protocal or write_one 10
							 else
							 m_pval[4] = (val>>8) & 0xff;//number hi
							 m_pval[5] = val & 0xff;//number lo
							 if(address!=10)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 if(m_serinumber_in_dll[0]==0 && m_serinumber_in_dll[1]==0 && m_serinumber_in_dll[2]==0 && m_serinumber_in_dll[3]==0)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 m_pval[6]=m_serinumber_in_dll[0];
							 m_pval[7]=m_serinumber_in_dll[1];
							 m_pval[8]=m_serinumber_in_dll[2];
							 m_pval[9]=m_serinumber_in_dll[3];
							 crc = CRC16(m_pval,10);
							 m_pval[10] = (crc >>8) & 0xff;
							 m_pval[11] = crc & 0xff;
							 }
							 }
							 */
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		//::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);

		//Sleep(SLEEP_TIME+10);
		Sleep(m_LATENCY_TIME_NET + 100);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		int nErr = WSAGetLastError();

		memcpy((void*)&m_gval[0], (void*)&rvdata[6], 13/*sizeof(m_gval)*/);
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			for (int i = 0; i<5; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0/* && m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0 && m_gval[8]==0 && m_gval[9]==0 && m_gval[10]==0 && m_gval[11]==0*/)
					return -3;
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}

		//delete []data;
		return 1;
	}
	return -1;
	///////////////////////////////////////////////////////////
}

 int CModbus::NetController_CheckTstatOnline_a(TS_UC devLo, TS_UC devHi, bool bComm_Type)
{
	if (bComm_Type == 0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address

		//Sleep(150);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSerial == NULL)
		{
			return -1;
		}
		int the_return_value;
		int the_return_value2 = 0;
		the_return_value = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
			if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
				return the_return_value;
			int i = 0;
			do
			{
				the_return_value = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
					return the_return_value;
				else if (the_return_value>0)
					i++;
			} while (i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if (bComm_Type == 1)//tcp
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		int the_return_value;
		int the_return_value2 = 0;
		the_return_value = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
			if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
				return the_return_value;
			int i = 0;
			do
			{
				the_return_value = NetController_CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
					return the_return_value;
				else if (the_return_value>0)
					i++;
			} while (i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	return -1;
}

 int CModbus::NetController_CheckTstatOnline2_a(TS_UC devLo, TS_UC devHi, bool bComm_Type)
{
	CString strlog;
	if (bComm_Type == 0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		strlog.Format(_T("Scan From ID=%d To ID=%d"), devLo, devHi);
		//WriteLogFile(strlog);
		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_UC  m_pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = 255;
		m_pval[1] = 26;  //put comments here,
		m_pval[2] = devHi;
		m_pval[3] = devLo;
		crc = CRC16(m_pval, 4);
		m_pval[4] = (crc >> 8) & 0xff;
		m_pval[5] = crc & 0xff;

		if (m_hSerial == NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear buffer
		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,// 数据缓冲区地址
			6,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)//不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,m_pval[0],m_pval[1],m_pval[2],m_pval[3],m_pval[4],m_pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(m_LATENCY_TIME_COM);//SLEEP_TIME);//because that scan have a delay lower 75ms
								//	Sleep(300);
								/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			13,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}


		CString strCom;
		strCom.Format(_T("Com=%d : "), m_open_com_port_number_in_dll);

		//WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog = _T("Send a scan command to any devices: ");
		//SaveBufferToLogFile(m_pval, 6);


		for (int i = 0; i < 6; i++)
		{
			int nValue = m_pval[i];
			CString strValue;
			strValue.Format(_T("%0x, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}
		//WriteLogFile(filelog);
		filelog.Empty();
		filelog = _T("Recv Data : ");
		for (int i = 0; i < 13; i++)
		{
			int nValue = m_gval[i];
			CString strValue;
			strValue.Format(_T("%0x, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}
		//WriteLogFile(filelog);
		int index = filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if (index == -1)
		{

		}
		else
		{

		}
		//SaveBufferToLogFile(m_gval, 13);









		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 26))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc = CRC16(m_gval, 3);
			if ((m_gval[3] != ((crc >> 8) & 0xff)) || (m_gval[4] != (crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect
				return -3;
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 25))
				return -2;
			crc = CRC16(m_gval, 7);
			if (m_gval[7] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[8] != (crc & 0xff))
				return -2;
		}
		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,m_gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",m_gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/

		return m_gval[2];
	}

	if (bComm_Type == 1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		strlog.Format(_T("Scan From ID=%d To ID=%d"), devLo, devHi);
		//NET_WriteLogFile(strlog);
		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect

		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_UC  m_pval[10];
		//		TS_US crc;
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数


		m_pval[0] = 1;
		m_pval[1] = 2;
		m_pval[2] = 3;
		m_pval[3] = 4;
		m_pval[4] = 5;
		m_pval[5] = 6;
		m_pval[6] = 255;
		m_pval[7] = 26;
		m_pval[8] = devHi;
		m_pval[9] = devLo;
		/*
		crc = CRC16(m_pval,4);
		m_pval[4] = (crc >>8) & 0xff;
		m_pval[5] = crc & 0xff;
		*/
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		::send(m_hSocket, (char*)m_pval, sizeof(m_pval), 0);
		Sleep(m_LATENCY_TIME_NET);//Sleep(SLEEP_TIME+8);

		TS_UC  rvdata[19];
		for (int i = 0; i<19; i++)
			rvdata[i] = 0;
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
		if (nRecv>0)
		{
			//异常:
			memcpy(m_gval, (void*)&rvdata[6], sizeof(rvdata) - 6);
		}


		//NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog = _T("Send a scan command to any devices: ");

		for (int i = 0; i < 10; i++)
		{
			int nValue = m_pval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}
		//NET_WriteLogFile(filelog);
		filelog.Empty();
		//NET_WriteLogFile(_T("Recv Data : "));
		for (int i = 0; i < 19; i++)
		{
			int nValue = rvdata[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}

		//NET_WriteLogFile(filelog);

		int index = filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if (index == -1)
		{
			//NET_WriteLogFile(_T(">>More than one device answer......"));
		}
		else
		{
			//NET_WriteLogFile(_T(">>No one device answer......"));
		}

		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != 255) || (m_gval[1] != 26))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect
				return -3;
			if ((m_gval[0] != 255) || (m_gval[1] != 26))
				return -2;
		}

		/*
		m_old_or_new_scan_protocal_in_dll=1;

		if((m_gval[0]!=m_pval[6]) || (m_gval[1]!=26))
		return -2;
		if(m_gval[0]==0 && m_gval[1]==0 && m_gval[2]==0 && m_gval[3]==0)
		{
		//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
		return -4;              //no response ,no connection
		}
		*/
		return m_gval[2];
	}
	return -1;
}

 int CModbus::read_multi2(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length, bool bComm_Type)
{
	if (bComm_Type == 0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = device_var;//slave address
		data_to_send[1] = 3;//function multiple
		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[3] = start_address & 0xff;//starting address lo
		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5] = length & 0xff;//quantity of registers lo
		TS_US crc = CRC16(data_to_send, 6);
		data_to_send[6] = (crc >> 8) & 0xff;
		data_to_send[7] = crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_send,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(m_LATENCY_TIME_COM);
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			to_send_data,// 数据缓冲区地址
			length * 2 + 5,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		crc = CRC16(to_send_data, length * 2 + 3);
		if (to_send_data[length * 2 + 3] != ((crc >> 8) & 0xff))
			return -2;
		if (to_send_data[length * 2 + 4] != (crc & 0xff))
			return -2;
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
		return length;
	}
	if (bComm_Type == 1)//tcp.
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

		DWORD m_had_send_data_number;//已经发送的数据的字节数
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

		TS_UC to_send_data[700] = { '\0' };
		TS_UC to_Reive_data[700] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = 1;
		data_to_send[1] = 2;
		data_to_send[2] = 3;
		data_to_send[3] = 4;
		data_to_send[4] = 5;
		data_to_send[5] = 6;



		data_to_send[6] = device_var;//slave address
		data_to_send[7] = 3;//function multiple
		data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[9] = start_address & 0xff;//starting address lo
		data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11] = length & 0xff;//quantity of registers lo

										 //TS_US crc=CRC16(data_to_send,6);
										 //data_to_send[6]=(crc>>8) & 0xff;
										 //data_to_send[7]=crc & 0xff;/

										 //		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

		Sleep(m_LATENCY_TIME_NET);
		int nn = sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

		memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		/*
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
		return -2;

		if(to_send_data[length*2+4]!=(crc & 0xff))
		return -2;
		*/
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
		return length;
	}
	return -1;
}

 int CModbus::CheckTstatOnline2_a(TS_UC devLo, TS_UC devHi, bool bComm_Type)
{
	CString strlog;


	if (bComm_Type == 0)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value == -6 , the bus has bannet protocol,scan stop;
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		strlog.Format(_T("Com Scan:  From ID=%d To ID=%d"), devLo, devHi);
		//WriteLogFile(strlog);

		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_UC  m_pval[6];
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = 255;
		m_pval[1] = 25;  //put comments here,
		m_pval[2] = devHi;
		m_pval[3] = devLo;
		crc = CRC16(m_pval, 4);
		m_pval[4] = (crc >> 8) & 0xff;
		m_pval[5] = crc & 0xff;
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear buffer
		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,// 数据缓冲区地址
			6,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}

		//	TRACE("%d T:%x %x %x %x %x %x\n",ddd,m_pval[0],m_pval[1],m_pval[2],m_pval[3],m_pval[4],m_pval[5]);
		//CloseHandle(m_osWrite.hEvent);
		///////////////////////////up is write
		Sleep(m_LATENCY_TIME_COM);//because that scan have a delay lower 75ms
								/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			13,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}



		//WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog = _T("Send a scan command to any devices: ");



		for (int i = 0; i < 6; i++)
		{
			int nValue = m_pval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}

		//WriteLogFile(filelog);
		filelog.Empty();
		filelog = _T("Recv Data : ");
		for (int i = 0; i < 13; i++)
		{
			int nValue = m_gval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
			if ((nValue == 0x55) && ((i + 1)<13))	//为了防止误判，检测2个周期的 55 ff  。。。。55 ff;
			{
				if ((m_gval[i + 1] == 0xff) && (i + 8)<13)
				{
					if ((m_gval[i + 8] == 0x55) && (i + 9)<13)
					{
						if (m_gval[i + 9] == 0xff)
							return -6;//总线上有bacnet协议，结束扫描;
					}

				}
			}
		}

		//WriteLogFile(filelog);
		int index = filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if (index == -1)
		{
			//WriteLogFile(_T(">>More than one device answer......"));
		}
		else
		{
			//WriteLogFile(_T(">>No one device answer......"));
		}

		if (m_gval[7] == 0 && m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0)  //modify by fance .解决扫描校验刚好为0而扫不到的 bug;
																											  //if(m_gval[8]==0 && m_gval[9]==0 && m_gval[10]==0 && m_gval[11]==0 && m_gval[12]==0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection


			}
			// added by zgq; find this situation: t3000 can find a comport,
			//which don't connect a tstat, but write file to the com, will receive the same data with send,
			// infact the com port don't work fine. But it never give you a wrong data.
			if (m_gval[0] == m_pval[0] && m_gval[1] == m_pval[1] && m_gval[2] == m_pval[2] && m_gval[3] == m_pval[3] && m_gval[4] == m_pval[4] && m_gval[5] == m_pval[5])
			{


				Sleep(SLEEP_TIME);
				return -4;
			}

			//////////////////////////////////////////////////////////////////////////
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 25))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			crc = CRC16(m_gval, 3);
			if ((m_gval[3] != ((crc >> 8) & 0xff)) || (m_gval[4] != (crc & 0xff)))
			{
				Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
		}
		else
		{

			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect
				return -3;
			if ((m_gval[0] != m_pval[0]) || (m_gval[1] != 25))
				return -2;
			crc = CRC16(m_gval, 7);
			if (m_gval[7] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[8] != (crc & 0xff))
				return -2;

		}


		//here is different with CheckTstatOnline() function
		//	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,m_gval[0],m_gval[1],m_gval[2],m_gval[3],m_gval[4],m_gval[5],m_gval[6],m_gval[7],m_gval[8],m_gval[9],m_gval[10],m_gval[11],m_gval[12]);
		//	TRACE("%d ^-^ ^-^ %d^\n",ddd,m_gval[2]);
		/*
		if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
		{
		default_file.SeekToEnd();
		a_line.Format("exist:%d",m_gval[2]);
		default_file.WriteString(a_line+"\n");
		default_file.Flush();
		default_file.Close();
		}*/
		return m_gval[2];
	}

	if (bComm_Type == 1)
	{
		//the second time
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		strlog.Format(_T("NET Scan:  From ID=%d To ID=%d"), devLo, devHi);
		//NET_WriteLogFile(strlog);
		if (devLo<1 || devHi>254)
			return -5;
		//the input inspect
		for (int i = 0; i<13; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer

		TS_UC  m_pval[10];
		//		TS_US crc;
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = 1;
		m_pval[1] = 2;
		m_pval[2] = 3;
		m_pval[3] = 4;
		m_pval[4] = 5;
		m_pval[5] = 6;

		m_pval[6] = 255;
		m_pval[7] = 25;
		m_pval[8] = devHi;
		m_pval[9] = devLo;

		/*
		crc = CRC16(m_pval,4);
		m_pval[4] = (crc >>8) & 0xff;
		m_pval[5] = crc & 0xff;
		*/

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		int nRet = ::send(m_hSocket, (char*)m_pval, sizeof(m_pval), 0);//scan 扫NC中的TSTAT

																   //Sleep(SLEEP_TIME+8);
		Sleep(m_LATENCY_TIME_NET + 100);
		// 		TS_UC  rvData[19];
		// 		for(int i=0;i<19;i++)
		// 			rvData[i]=0;

		TS_UC  rvData[100];
		for (int i = 0; i<100; i++)
			rvData[i] = 0;


		int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
		if (nRecv>0)
		{
			memcpy(m_gval, (void*)&rvData[6], sizeof(rvData));
		}

		//		static int num = 0;
		//NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
		CString filelog;
		filelog = _T("Send a scan command to any devices: ");


		for (int i = 0; i < 10; i++)
		{
			int nValue = m_pval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}

		//NET_WriteLogFile(filelog);
		filelog.Empty();

		filelog = _T("Recv Data : ");
		for (int i = 0; i <13; i++)
		{
			int nValue = m_gval[i];
			CString strValue;
			strValue.Format(_T("%0X, "), nValue);
			//g_fileScanLog->WriteString(strValue);
			filelog += strValue;
		}
		////NET_WriteLogFile(_T("Recv Data:"));
		//NET_WriteLogFile(filelog);
		int index = filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
		if (index == -1)
		{
			//WriteLogFile(_T(">>More than one device answer......"));
		}
		else
		{
			//WriteLogFile(_T(">>No one device answer......"));
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
		//				//str.Format(_T("%"),m_pval);
		//				//m_pFile->WriteString(str+_T("\n"));
		//				for (int i =0;i<sizeof(m_pval);i++)
		//				{
		//					idate =(int)m_pval[i];
		//					str.Format(_T("%0x"),idate);
		//
		//					g_fileScanLog->WriteString(str+_T(" "));
		//
		//				}
		//
		//				g_fileScanLog->WriteString(_T("\n"));
		//				g_fileScanLog->WriteString(strreceive+_T("\n"));//↑
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








		//if(m_gval[8]==0 && m_gval[9]==0 && m_gval[10]==0 && m_gval[11]==0 && m_gval[12]==0)//// it's old code, modified by zgq:2011-12-14
		if (m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0 && m_gval[12] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0)
		{
			//old scan protocal
			m_old_or_new_scan_protocal_in_dll = 2;
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0)
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -4;              //no response ,no connection
			}
			if (m_gval[5] != 0 || m_gval[6] != 0)//to inspect
			{
				Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
				return -3;
			}
			if ((m_gval[0] != 255) || (m_gval[1] != 25))
			{
				//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
				return -2;
			}
			/*
			crc=CRC16(m_gval,3);
			if( (m_gval[3]!=((crc>>8) & 0xff)) || (m_gval[4]!=(crc & 0xff)))
			{
			//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
			return -2;
			}
			*/

		}
		else
		{
			// new scan protocal,if many old tstat ,get into here ,scan result is oK too.
			m_old_or_new_scan_protocal_in_dll = 1;
			Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble


							  //if(m_gval[7]!=0 || m_gval[8]!=0 || m_gval[9]!=0 || m_gval[10]!=0)//to inspect
			if (m_gval[9] != 0 || m_gval[10] != 0 || m_gval[11] != 0 || m_gval[12] != 0)//to inspect  //zgq: same with com port scan
			{
				//Sleep(SLEEP_TIME+8);
				return -3;

			}

			if ((m_gval[0] != 255) || (m_gval[1] != 25))
				return -2;

			/*
			if (m_gval[0]==0&&m_gval[0]==0&&m_gval[0]==0)
			{
			return -3;
			}
			*/

			/*
			crc=CRC16(m_gval,7);
			if( m_gval[7]!=((crc>>8) & 0xff) )
			return -2;
			if(m_gval[8]!=(crc & 0xff))
			return -2;
			*/
		}
		return m_gval[2];
	}
	return -1;
}

 int CModbus::CheckTstatOnline_a(TS_UC devLo, TS_UC devHi, bool bComm_Type)
{
	if (bComm_Type == 0)
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value == -6 , the bus has bannet protocol,scan stop;
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSerial == NULL)
		{
			return -1;
		}
		int the_return_value;
		int the_return_value2 = 0;
		//CheckTstatOnline2_a 可以记录收发的数据
		the_return_value = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
		if (the_return_value == -6)
			return -6;
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
			if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
				return the_return_value;
			int i = 0;
			do
			{
				the_return_value = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
					return the_return_value;
				else if (the_return_value>0)
					i++;
			} while (i<3);
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}

	if (bComm_Type == 1)//
	{
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -2 ,try it again
		//the return value == -3,Maybe that have more than 2 Tstat is connecting
		//the return value == -4 ,between devLo and devHi,no Tstat is connected ,
		//the return value == -5 ,the input have some trouble
		//the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
		//清空串口缓冲区
		//the return value is the register address
		//Sleep(50);       //must use this function to slow computer
		if (devLo<1 || devHi>254)
			return -5;
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		int the_return_value;
		int the_return_value2 = 0;
		the_return_value = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
		//down is inspect result first scan
		if (the_return_value == -4)
		{
			the_return_value = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
			return the_return_value;
		}
		if (m_old_or_new_scan_protocal_in_dll == 1)
		{
			//new protocal
			if (the_return_value>0)
			{
				the_return_value2 = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		else if (m_old_or_new_scan_protocal_in_dll == 2)
		{
			//old protocal
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
			if (the_return_value>0)
			{
				the_return_value2 = CheckTstatOnline2_a(devLo, devHi, bComm_Type);
				if (the_return_value2 != -4)
					the_return_value = the_return_value2;
			}
			return the_return_value;
		}
		//	if(the_return_value>0)
		//		TRACE("^-^ ^-^ %d\n",the_return_value);
		return the_return_value;
	}
	return -1;
}

 void CModbus::close_T3000_log_file()
{
	m_pFile->Close();
	delete m_pFile;
}

 void CModbus::write_T3000_log_file(CString StrTips)
{



	CCriticalSection  m_logfile_section;
	m_logfile_section.Lock();



	CTime nowtime = CTime::GetCurrentTime();
	CString time;
	time.Format(_T("%4d-%.2d-%.2d %.2d:%.2d:%.2d  >>"), nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
		nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());
	CString logstr = time + StrTips;


	m_pFile->SeekToEnd();
	m_pFile->WriteString(logstr.GetBuffer());
	m_pFile->WriteString(_T("\n"));


	m_pFile->Flush();



	m_logfile_section.Unlock();

}

 void CModbus::Create_T3000_log_file()
{
	m_pFile = new CStdioFile;//txt

	TCHAR exeFullPath[MAX_PATH + 1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	CString g_strDatabasefilepath = exeFullPath;
	CString g_strExePth = g_strDatabasefilepath;
	m_strFileINI = g_strExePth + _T("T3000.log");
	m_pFile->Open(m_strFileINI.GetString(), CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate);

}



 CString CModbus::Get_NowTime()
{
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%2d-%2d"), st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("%2d:%2d:%2d"), st.wHour, st.wMinute, st.wSecond);
	strDate += _T(" ");
	strDate += strTime;


	return strDate;
}



 int CModbus::Read_One_tap(TS_UC device_var, TS_US address)
{

	//	CSingleLock singlock(&scan_mutex);
	//	singlock.Lock();
	int nTemp = -1;
	if (m_Commu_type == 0)//serial
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
		crc = CRC16(m_pval, 6);
		m_pval[6] = (crc >> 8) & 0xff;
		m_pval[7] = crc & 0xff;
		if (m_hSerial == NULL)
		{
			return -1;
		}

		////////////////////////////////////////////////////////////overlapped declare
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		BOOL bRet = ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							///////////////////////////////////////////////////////send the to read message
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;

		int fState = WriteFile(m_hSerial,// 句柄
			m_pval,	// 数据缓冲区地址
			8,		// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_PENDING)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		//////////////////////////////////////////the message had send ,now to read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;

		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				11,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				7,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;
			crc = CRC16(m_gval, 5);
			if (m_gval[5] != ((crc >> 8) & 0xff))
				return -2;
			if (m_gval[6] != (crc & 0xff))
				return -2;
		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				crc = CRC16(m_gval, 9);
				if (m_gval[9] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[10] != (crc & 0xff))
					return -2;
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)//2
					return -2;
				crc = CRC16(m_gval, 5);
				if (m_gval[5] != ((crc >> 8) & 0xff))
					return -2;
				if (m_gval[6] != (crc & 0xff))
					return -2;
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (nTemp * 256 + m_gval[4]);
	}
	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		//return value == -3 ,no response
		//清空串口缓冲区
		//TS_UC  m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];

		/*
		struct DataPack
		{
		TS_UC  empty[6];
		TS_UC  m_pval[13];

		};
		*/

		TS_UC data[12];

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;
		data[6] = device_var;
		data[7] = 3;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		data[10] = 0;
		data[11] = 1;

		//		DataPack dataInfo;
		for (int i = 0; i<11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//TS_US crc;
						//DWORD m_had_send_data_number;//已经发送的数据的字节数

		m_pval[0] = device_var;
		m_pval[1] = 3;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		m_pval[4] = 0;//(val>>8) & 0xff;//number hi
		m_pval[5] = 1;//val & 0xff;//number lo
					//	crc = CRC16(m_pval,6);
					//	m_pval[6] = (crc >>8) & 0xff;
					//	m_pval[7] = crc & 0xff;

					//memcpy((char*)&data[6],(char*)&m_pval[0],sizeof(m_pval));

		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int nLen = sizeof(data);
		//int xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		int xx = ::send(m_hSocket, (char*)&data, sizeof(data), 0);


		////////////////////////////////////////////////clear com error
		if (address == 10)
		{
			m_serinumber_in_dll[0] = m_serinumber_in_dll[1] = m_serinumber_in_dll[2] = m_serinumber_in_dll[3] = 0;//this line is for new protocal
		}

		Sleep(m_LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
								//DataPack rvData;
								//Sleep(SLEEP_TIME+10);

		TS_UC rvData[17];
		//int nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);
		int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
		//m_gval=rvData.m_pval;
		memcpy((void*)m_gval, (void*)&rvData[6], 13/*sizeof(m_gval)*/);

		/*
		xx=::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		nRecv = ::recv(m_hSocket, (char*)m_gval, sizeof(m_gval), 0);*/

		int nerro = WSAGetLastError();

		if (address != 10)
		{
			//old protocal
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0/* && m_gval[5]==0 && m_gval[6]==0*/)
				return -3;
			if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 2)
				return -2;

			/*;
			crc=CRC16(m_gval,5);
			if(m_gval[5]!=((crc>>8)&0xff))
			return -2;
			if(m_gval[6]!=(crc & 0xff))
			return -2;
			*/

		}
		else
		{
			if (m_gval[7] != 0 || m_gval[8] != 0 || m_gval[9] != 0 || m_gval[10] != 0)
			{
				//new protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_pval[1] || m_gval[2] != 6)//6
					return -2;
				/*
				crc=CRC16(m_gval,9);
				if(m_gval[9]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[10]!=(crc & 0xff))
				return -2;
				*/
				m_serinumber_in_dll[0] = m_gval[5];
				m_serinumber_in_dll[1] = m_gval[6];
				m_serinumber_in_dll[2] = m_gval[7];
				m_serinumber_in_dll[3] = m_gval[8];//stay serialnumber
											   //			TRACE("R:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
			}
			else
			{
				//old protocal
				if (m_gval[0] != m_pval[0] || m_gval[1] != m_gval[1] || m_gval[2] != 2)//2
					return -2;
				/*
				crc=CRC16(m_gval,5);
				if(m_gval[5]!=((crc>>8)&0xff))
				return -2;
				if(m_gval[6]!=(crc & 0xff))
				return -2;
				*/
			}
		}
		nTemp = m_gval[3];
		if (nTemp == 255)
			nTemp = -1;
		return (m_gval[3] * 256 + m_gval[4]);
	}
	return -1; //add by Fance
			   //	singlock.Unlock();
}

 int CModbus::Write_One_tap(TS_UC device_var, TS_US address, TS_US val)
{
	if (m_Commu_type == 0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//		m_gval[8]={'\0'};//the data that get
		//      TS_UC  m_pval[9];
		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo
		if (address != 10)
		{
			crc = CRC16(m_pval, 6);
			m_pval[6] = (crc >> 8) & 0xff;
			m_pval[7] = crc & 0xff;
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				crc = CRC16(m_pval, 6);
				m_pval[6] = (crc >> 8) & 0xff;
				m_pval[7] = crc & 0xff;
			}
			else
			{
				m_pval[6] = m_serinumber_in_dll[0];
				m_pval[7] = m_serinumber_in_dll[1];
				m_pval[8] = m_serinumber_in_dll[2];
				m_pval[9] = m_serinumber_in_dll[3];
				crc = CRC16(m_pval, 10);
				m_pval[10] = (crc >> 8) & 0xff;
				m_pval[11] = crc & 0xff;
			}
		}

		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		if (address != 10)
		{
			fState = WriteFile(m_hSerial,// 句柄
				m_pval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osWrite);
		}
		else
		{
			//==10 register
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
			else
			{
				//new protocal
				fState = WriteFile(m_hSerial,// 句柄
					m_pval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osWrite);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		Sleep(m_LATENCY_TIME_COM);
		if (address != 10)
		{
			fState = ReadFile(m_hSerial,// 句柄
				m_gval,// 数据缓冲区地址
				8,// 数据大小
				&m_had_send_data_number,// 返回发送出去的字节数
				&m_osRead);
		}
		else
		{
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					8,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
			else
			{
				//new protocal
				fState = ReadFile(m_hSerial,// 句柄
					m_gval,// 数据缓冲区地址
					12,// 数据大小
					&m_had_send_data_number,// 返回发送出去的字节数
					&m_osRead);
			}
		}
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			for (int i = 0; i<8; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<8; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 && m_gval[6] == 0 && m_gval[7] == 0 && m_gval[8] == 0 && m_gval[9] == 0 && m_gval[10] == 0 && m_gval[11] == 0)
					return -3;
				for (int i = 0; i<12; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}
		return 1;
	}

	if (m_Commu_type == 1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//清空串口缓冲区

		//TS_UC data[12];
		TS_UC data[16];
		// 		TS_UC* data=NULL;
		int nSendNum = 12;
		if (address == 10)
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

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;
		data[3] = 4;
		data[4] = 5;
		data[5] = 6;
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		data[6] = device_var;
		data[7] = 6;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			data[10] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			data[10] = (val >> 8) & 0xff;//number hi
		data[11] = val & 0xff;//number lo

		if (address == 10)
		{

			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
			}
			else
			{
				//* uncommend by zgq; we use net work scan tstat through NC
				data[12] = m_serinumber_in_dll[0];
				data[13] = m_serinumber_in_dll[1];
				data[14] = m_serinumber_in_dll[2];
				data[15] = m_serinumber_in_dll[3];
				//*/
			}
		}


		for (int i = 0; i <= 11; i++)
			m_gval[i] = 0;/////////////////////////////////////////clear buffer
						//	TS_US crc;
						//	DWORD m_had_send_data_number;//已经发送的数据的字节数
		m_pval[0] = device_var;
		m_pval[1] = 6;
		m_pval[2] = address >> 8 & 0xFF;
		m_pval[3] = address & 0xFF;
		if (address == 10 && (m_serinumber_in_dll[0] != 0 || m_serinumber_in_dll[1] != 0 || m_serinumber_in_dll[2] != 0 || m_serinumber_in_dll[3] != 0))
			m_pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
		else
			m_pval[4] = (val >> 8) & 0xff;//number hi
		m_pval[5] = val & 0xff;//number lo

							 /*
							 for(int i=0;i<=11;i++)
							 m_gval[i]=0;/////////////////////////////////////////clear buffer
							 TS_US crc;
							 DWORD m_had_send_data_number;//已经发送的数据的字节数
							 m_pval[0] = device_var;
							 m_pval[1] = 6;
							 m_pval[2] = address>>8 & 0xFF ;
							 m_pval[3] = address & 0xFF;
							 if(address==10 && (m_serinumber_in_dll[0]!=0 || m_serinumber_in_dll[1]!=0 || m_serinumber_in_dll[2]!=0 || m_serinumber_in_dll[3]!=0))
							 m_pval[4]=0x55;/////////////////////////////new protocal or write_one 10
							 else
							 m_pval[4] = (val>>8) & 0xff;//number hi
							 m_pval[5] = val & 0xff;//number lo
							 if(address!=10)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 if(m_serinumber_in_dll[0]==0 && m_serinumber_in_dll[1]==0 && m_serinumber_in_dll[2]==0 && m_serinumber_in_dll[3]==0)
							 {
							 crc = CRC16(m_pval,6);
							 m_pval[6] = (crc >>8) & 0xff;
							 m_pval[7] = crc & 0xff;
							 }
							 else
							 {
							 m_pval[6]=m_serinumber_in_dll[0];
							 m_pval[7]=m_serinumber_in_dll[1];
							 m_pval[8]=m_serinumber_in_dll[2];
							 m_pval[9]=m_serinumber_in_dll[3];
							 crc = CRC16(m_pval,10);
							 m_pval[10] = (crc >>8) & 0xff;
							 m_pval[11] = crc & 0xff;
							 }
							 }
							 */
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		//::send(m_hSocket,(char*)m_pval,sizeof(m_pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);

		//Sleep(SLEEP_TIME+10);
		Sleep(m_LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		int nErr = WSAGetLastError();

		memcpy((void*)&m_gval[0], (void*)&rvdata[6], 13/*sizeof(m_gval)*/);
		if (address != 10)
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			for (int i = 0; i<5; i++)
				if (m_gval[i] != m_pval[i])
					return -2;
		}
		else
		{
			if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0/* && m_gval[6]==0 && m_gval[7]==0*/)
				return -3;
			if (m_serinumber_in_dll[0] == 0 && m_serinumber_in_dll[1] == 0 && m_serinumber_in_dll[2] == 0 && m_serinumber_in_dll[3] == 0)
			{
				//old protocal
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
			else
			{
				//new protocal
				//			crc=CRC16(m_gval,10);
				//			if(m_gval[6]!=((crc>>8)&0xff))
				//				return -2;
				//			if(m_gval[7]!=(crc & 0xff))
				//				return -2;
				//			TRACE("W:%x %x %x %x\n",m_serinumber_in_dll[0],m_serinumber_in_dll[1],m_serinumber_in_dll[2],m_serinumber_in_dll[3]);
				if (m_gval[0] == 0 && m_gval[1] == 0 && m_gval[2] == 0 && m_gval[3] == 0 && m_gval[4] == 0 && m_gval[5] == 0 /*&& m_gval[6]==0 && m_gval[7]==0 && m_gval[8]==0 && m_gval[9]==0 && m_gval[10]==0 && m_gval[11]==0*/)
					return -3;
				for (int i = 0; i<5; i++)
					if (m_gval[i] != m_pval[i])
						return -2;
			}
		}

		//delete []data;
		return 1;
	}
	return -1;//add by Fance
			  ///////////////////////////////////////////////////////////
}

 int CModbus::read_multi_tap(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length)
{
	if (m_Commu_type == 0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = device_var;//slave address
		data_to_send[1] = 3;//function multiple
		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[3] = start_address & 0xff;//starting address lo
		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5] = length & 0xff;//quantity of registers lo
		TS_US crc = CRC16(data_to_send, 6);
		data_to_send[6] = (crc >> 8) & 0xff;
		data_to_send[7] = crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message



		int fState = WriteFile(m_hSerial,// 句柄
			data_to_send,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(m_LATENCY_TIME_COM);
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error

		fState = ReadFile(m_hSerial,// 句柄
			to_send_data,// 数据缓冲区地址
			length * 2 + 5,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		crc = CRC16(to_send_data, length * 2 + 3);
		if (to_send_data[length * 2 + 3] != ((crc >> 8) & 0xff))
			return -2;
		if (to_send_data[length * 2 + 4] != (crc & 0xff))
			return -2;
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
		return length;
	}
	if (m_Commu_type == 1)//tcp.
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

		DWORD m_had_send_data_number;//已经发送的数据的字节数
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

		TS_UC to_send_data[700] = { '\0' };
		TS_UC to_Reive_data[700] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
		data_to_send[0] = 1;
		data_to_send[1] = 2;
		data_to_send[2] = 3;
		data_to_send[3] = 4;
		data_to_send[4] = 5;
		data_to_send[5] = 6;



		data_to_send[6] = device_var;//slave address
		data_to_send[7] = 3;//function multiple
		data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[9] = start_address & 0xff;//starting address lo
		data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11] = length & 0xff;//quantity of registers lo

										 //TS_US crc=CRC16(data_to_send,6);
										 //data_to_send[6]=(crc>>8) & 0xff;
										 //data_to_send[7]=crc & 0xff;/

										 //		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

		Sleep(m_LATENCY_TIME_NET);
		int nn = sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

		memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
			return -2;
		/*
		crc=CRC16(to_send_data,length*2+3);
		if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
		return -2;

		if(to_send_data[length*2+4]!=(crc & 0xff))
		return -2;
		*/
		for (int i = 0; i<length; i++)
			put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
		return length;
	}
	return -1;
}

 int CModbus::write_multi_tap(TS_UC device_var, TS_UC *to_write, TS_US start_address, int length)
{
	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x10;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = 0;
		data_to_write[5] = length;
		data_to_write[6] = length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[7 + i] = to_write[i];
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;

		TS_US crc = CRC16(data_to_write, length + 7);
		data_to_write[length + 7] = crc >> 8 & 0xff;
		data_to_write[length + 8] = crc & 0xff;

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length + 9,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
			if (m_gval[i] != *(data_to_write + i))
				return -2;
		crc = CRC16(m_gval, 6);
		if (m_gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (m_gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };

		data_to_write[0] = 1;
		data_to_write[1] = 2;
		data_to_write[2] = 3;
		data_to_write[3] = 4;
		data_to_write[4] = 5;
		data_to_write[5] = 6;

		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = 0;
		data_to_write[11] = length;
		data_to_write[12] = length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[13 + i] = to_write[i];


		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + length, 0);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);
		if (nRecv<0)
		{
			return -2;
		}
		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}

 void CModbus::SetResponseTime(TS_US Time)
{
	if (m_Commu_type == 0)
	{
		m_LATENCY_TIME_COM = Time;
	}
	else
	{
		m_LATENCY_TIME_NET = Time;
	}
}

 int CModbus::SendData(TS_US *to_write, TS_US length, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength, BOOL CHECK_CRC)
{
	if (m_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' }; //128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[i] = to_write[i];
		TS_US crc;
		if (CHECK_CRC)
		{
			crc = CRC16(data_to_write, length);
			data_to_write[length] = crc >> 8 & 0xff;
			data_to_write[length + 1] = crc & 0xff;
		}


		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;

		if (CHECK_CRC)
		{
			length += 2;
		}

		*sendDataLength = length;


		for (int i = 0; i < *sendDataLength; i++)
		{
			//   *((char*)put_senddate_into_here + i) = data_to_write[i];
			put_senddate_into_here[i] = data_to_write[i];
		}

		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// 句柄
			data_to_write,// 数据缓冲区地址
			length,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(m_LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// 句柄
			m_gval,    // 数据缓冲区地址
			128,      // 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			//*((char*)put_revdata_into_here + i) = m_gval[i];
			put_revdata_into_here[i] = m_gval[i];
		}

		///////////////////////////////////////////////////////////


		return 1;
	}
	if (m_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };





		//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		for (int i = 0; i<length; i++)
			data_to_write[i] = to_write[i];



		TS_US crc;
		if (CHECK_CRC)
		{
			crc = CRC16(data_to_write, length);
			data_to_write[length] = crc >> 8 & 0xff;
			data_to_write[length + 1] = crc & 0xff;
		}



		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		if (CHECK_CRC)
		{
			length += 2;
		}
		*sendDataLength = length;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		int n = ::send(m_hSocket, (char *)data_to_write, length, 0);
		Sleep(m_LATENCY_TIME_NET);
		int nn = sizeof(data_back_write);
		int nRecv = ::recv(m_hSocket, (char*)data_back_write, nn, 0);



		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = data_back_write[i];
		}

		if (nRecv<0)
		{
			return -2;
		}


		return 1;

	}
	return -1;

}


 int CModbus::Modbus_Standard_Read(TS_UC device_var, TS_US *put_data_into_here, int function_code, TS_US start_address, int length,
	unsigned char *put_senddate_into_here,
	unsigned char *put_revdata_into_here,
	int* sendDataLength, int* recvDataLength
)
{
	int responseLength = -2;



	if (m_Commu_type == 0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting

		TS_UC to_send_data[300] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register

		TS_UC data_to_send[8] = { '\0' };    //the array to used in writefile()
		data_to_send[0] = device_var;      //slave address
		data_to_send[1] = function_code;  //function multiple
		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[3] = start_address & 0xff;//starting address lo
		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5] = length & 0xff;//quantity of registers lo
		TS_US crc = CRC16(data_to_send, 6);
		data_to_send[6] = (crc >> 8) & 0xff;
		data_to_send[7] = crc & 0xff;

		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("MulWrite"))) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}


		int fState = WriteFile(m_hSerial,// 句柄
			data_to_send,// 数据缓冲区地址
			8,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osMulWrite);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(m_LATENCY_TIME_COM);
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, _T("Read"))) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error

		fState = ReadFile(m_hSerial,// 句柄
			to_send_data,// 数据缓冲区地址
			length * 2 + 5,// 数据大小
			&m_had_send_data_number,// 返回发送出去的字节数
			&m_osRead);
		if (!fState)// 不支持重叠
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// 等待
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_send_data[i];
		}


		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != function_code)
		{
			return -2;
		}
		responseLength = length;
		if (function_code == 1 || function_code == 2)
		{

			if (length % 8 == 0)
			{
				responseLength = length / 8;
			}
			else
			{

				responseLength = length / 8 + 1;
			}
			if (to_send_data[2] != responseLength)
			{
				return -2;
			}
			crc = CRC16(to_send_data, responseLength + 3);
			if (to_send_data[responseLength + 3] != ((crc >> 8) & 0xff))
				return -2;
			if (to_send_data[responseLength + 4] != (crc & 0xff))
				return -2;
			int count = 0;
			for (int i = 0; i < responseLength; i++)
			{
				bitset<8> BitReg(to_send_data[3 + i]);
				for (int j = 8 * i; j < 8 * (i + 1); j++)
				{
					if (count<length)
					{
						put_data_into_here[j] = BitReg[j - 8 * i];
					}
					count++;
				}

			}

		}
		else
		{
			if (to_send_data[2] != responseLength * 2)
			{
				return -2;
			}
			crc = CRC16(to_send_data, responseLength * 2 + 3);
			if (to_send_data[responseLength * 2 + 3] != ((crc >> 8) & 0xff))
				return -2;
			if (to_send_data[responseLength * 2 + 4] != (crc & 0xff))
				return -2;
			for (int i = 0; i < responseLength; i++)
				put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];

		}




		return length;
	}
	if (m_Commu_type == 1)//tcp.
	{
		TS_UC to_send_data[700] = { '\0' };
		TS_UC to_Reive_data[700] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register

		TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}


		//data_to_send[0] = 1;
		//data_to_send[1] = 2;
		//data_to_send[2] = 3;
		//data_to_send[3] = 4;
		//data_to_send[4] = 5;
		//data_to_send[5] = 6;

		// 		TS_UC data_to_send[8] = { '\0' };    //the array to used in writefile()
		// 		data_to_send[0] = device_var;      //slave address
		// 		data_to_send[1] = function_code;  //function multiple
		// 		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		// 		data_to_send[3] = start_address & 0xff;//starting address lo
		// 		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		// 		data_to_send[5] = length & 0xff;//quantity of registers lo
		// 		TS_US crc = CRC16(data_to_send, 6);
		// 		data_to_send[6] = (crc >> 8) & 0xff;
		// 		data_to_send[7] = crc & 0xff;

		data_to_send[6] = device_var;//slave address
		data_to_send[7] = function_code;
		data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[9] = start_address & 0xff;//starting address lo
		data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11] = length & 0xff;//quantity of registers lo

										 //TS_US crc=CRC16(data_to_send,6);
										 //data_to_send[6]=(crc>>8) & 0xff;
										 //data_to_send[7]=crc & 0xff;/

										 //		DWORD m_had_send_data_number;//已经发送的数据的字节数
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		*sendDataLength = sizeof(data_to_send);
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}
		::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

		Sleep(m_LATENCY_TIME_NET);
		int nn = sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_Reive_data[i];
		}

		memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != function_code)
			return -2;

		responseLength = length;
		if (function_code == 1 || function_code == 2)
		{

			if (length % 8 == 0)
			{
				responseLength = length / 8;
			}
			else
			{

				responseLength = length / 8 + 1;
			}
			if (to_send_data[2] != responseLength)
			{
				return -2;
			}

			int count = 0;
			for (int i = 0; i < responseLength; i++)
			{
				bitset<8> BitReg(to_send_data[3 + i]);
				for (int j = 8 * i; j < 8 * (i + 1); j++)
				{
					if (count < length)
					{
						put_data_into_here[j] = BitReg[j - 8 * i];
					}
					count++;
				}

			}

		}
		else
		{
			if (to_send_data[2] != responseLength * 2)
			{
				return -2;
			}

			for (int i = 0; i < responseLength; i++)
				put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];

		}




		return length;

	}
	return -1;
}


