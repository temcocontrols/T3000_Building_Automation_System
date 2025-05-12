#include "StdAfx.h"
#include "FlashSN.h"
#include "Global_Struct.h"
#include <fstream>
extern bool auto_flash_mode;
extern CString Auto_flash_SN_connect_IP;
extern CString    c_strSNRecordFileName;
CFlashSN::CFlashSN(void)
{
	m_nComPort = 1;
	m_nMBID = 255; 
//	m_dwIPAddr = 0;
	m_nIPPort = 0;

	m_nFlashType = 0;   // 0= com, 1=ethernet
	m_nSerialNumber = 0;
	
	m_nProductModel = 1;
	m_nHardWareVersion = 0;
}
CFlashSN::~CFlashSN(void)
{

}
void CFlashSN::SetComport(int nComport)
{
	m_nComPort = nComport;
}
void CFlashSN::SetIPPort(int nIPPort)
{
	m_nIPPort = nIPPort;
}
void CFlashSN::SetIPAddress(CString dwIPAddr)
{
	m_dwIPAddr = dwIPAddr;
}
void CFlashSN::SetFlashTpye(int nFlashType)
{
	m_nFlashType = nFlashType;
}
BOOL CFlashSN::StartWriteSN()
{
	if (m_nFlashType == 0)
	{
		FlashTstatSN();
	}
    else
    {
        FlashNCSN();
    }
	

	return TRUE;
}

unsigned char Add_CRC(char *point , int nlength)
{
	int add_value = 0;
	for (int i=0;i<=nlength;i++)
	{
		add_value = add_value + point[i];
	}
	return (add_value % 256);
}

// BOOL CFlashSN::NeedNewSN()
// {
// 	return FALSE;
// }

void CFlashSN::SetFlashParam(const CString	& strProductModel, int nHardWareVersion,int nProductID,int nsoftversion,const CString & n_username)
{
	m_strProductModel = strProductModel;
 
	m_nHardWareVersion = nHardWareVersion;
	m_nProductModel=nProductID;
	m_username = n_username;
	m_nsoftversion = nsoftversion;
}
SOCKET m_hSocket=NULL;
unsigned long CFlashSN::Get_SerialNumber_From_Server(CString nipaddress,
													 int nipport,
													 CString nProductName,
													 unsigned char nproductid,
													 unsigned short nfirwareversion,
													 unsigned short	nhardwareversion, 
													 CString nusername)
{
	bool ret_connect = Connect_To_Server(nipaddress,nipport);
	if(!ret_connect)
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("Connect to the Server failed!"));
		return -1;
	}
	int rec_index =0;
	int index = 0;
	char send_buffer[200];
	memset(send_buffer,0,200);
	send_buffer[index] = 0x55; index ++;
	send_buffer[index] = 0xff; index ++;
	send_buffer[index] = 0x33; index ++;
	send_buffer[index] = 0; index ++;
	send_buffer[index] = 1;  index ++;//GET_SERIAL_NUMBER = 1,

	if(nProductName.GetLength()> 19)
	{
		nProductName = nProductName.Left(19);
	}
	char ntemp_name[20];
	memset(ntemp_name,0,20);
	WideCharToMultiByte( CP_ACP, 0, nProductName.GetBuffer(), -1, ntemp_name, 20, NULL, NULL );
	nProductName.ReleaseBuffer();

	memcpy(&send_buffer[index],ntemp_name,20);
	index = index + 20;
	send_buffer[index] = nproductid; index ++;

	send_buffer[index] = (char)(nfirwareversion & 0x00ff); index ++;
	send_buffer[index] = (char)(nfirwareversion >> 8); index ++;
	index = index + 4; //序列号等服务器传过来;

	send_buffer[index] = (char)(nhardwareversion & 0x00ff); index ++;
	send_buffer[index] = (char)(nhardwareversion >> 8); index ++;

	if(nusername.GetLength()> 17)
	{
		nusername = nusername.Left(17);
	}
	char ntemp_user_name[18];
	memset(ntemp_user_name,0,18);
	WideCharToMultiByte( CP_ACP, 0, nusername.GetBuffer(), -1, ntemp_user_name, 18, NULL, NULL );
	nusername.ReleaseBuffer();
	memcpy(&send_buffer[index],ntemp_user_name,18);
	index = index + 18;
	send_buffer[index] =	Add_CRC(send_buffer,index - 1);

	TIMEVAL time;
	time.tv_sec =3;
	time.tv_usec = 1000;
	fd_set fdSocket;

	for (int i=0;i<3;i++)
	{
		FD_ZERO(&fdSocket);	
		FD_SET(m_hSocket, &fdSocket);

		int nRet=::send(m_hSocket,(char*)&send_buffer,53,0);

		if (nRet == SOCKET_ERROR)
		{
			int  nError = WSAGetLastError();
			return -1;	//Socket error;
		}
		int nLen = sizeof(m_hSocket);
		fd_set fdRead = fdSocket;
		int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
		if (nSelRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			return -1;
		}

		if(nSelRet > 0)
		{
			int nRet ;
			int nSelRet;
			do 
			{
				unsigned char rvData[200];
				memset(rvData,0,200);
				int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
				if(nRecv > 0)
				{
					if(nRecv == 53)
					{
						rec_index = 0;
						if((rvData[rec_index] != 0x55) ||(rvData[rec_index + 1] != 0xff ))
						{
							goto data_error;
						}
						rec_index = rec_index + 2;
						if((rvData[rec_index] != 0x33) ||(rvData[rec_index + 1] != 0 ))
						{
							goto data_error;
						}
						rec_index = rec_index + 2;
						if((rvData[rec_index] != 101))	//RETURN_SERIAL_NUMBER = 101,
						{
							goto data_error;
						}
						rec_index =rec_index + 1;
						rec_index = rec_index + 20 + 1 + 2;
						unsigned long temp_serial = (rvData[rec_index + 3])<<24 | (rvData[rec_index + 2]<<16) | (rvData[rec_index + 1])<<8 | (rvData[rec_index]);
						
						char reply_buffer[100];
						reply_buffer[0]= 0x55;
						reply_buffer[1]= 0xff;
						reply_buffer[2]= 4;
						reply_buffer[3]= 0;
						reply_buffer[4]= 0xc9;	//ACK_SERIAL_NUMBER
						reply_buffer[5]= 0x21;

						int nreply_Ret=::send(m_hSocket,(char*)&reply_buffer,6,0);
						if(nreply_Ret == 6)
						{
							::closesocket(m_hSocket);
							m_hSocket = NULL;
							return temp_serial;
						}
						else
						{
							::closesocket(m_hSocket);
							m_hSocket = NULL;
						}
					}
				}
				else
				{
					break;
				}
				
data_error:
				FD_ZERO(&fdSocket);	
				FD_SET(m_hSocket, &fdSocket);
				//nLen = sizeof(m_hSocket);
				fdRead = fdSocket;
				nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);

				
			} while (nSelRet);	
		}
	}

	closesocket(m_hSocket);
	m_hSocket = NULL;
	//m_strProductModel = strProductModel;

	//m_nHardWareVersion = nHardWareVersion;
	//m_nProductModel=nProductID;

}




bool CFlashSN::Connect_To_Server(CString strIPAdress,short nPort)
{

	int nNetTimeout=2000;//1 second.
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
	int sockfd;
	sockfd = m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	//发送时限
	setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
	//接收时限
	setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

	//****************************************************************************
	// Fance added ,不要用阻塞的模式，如果设备不在线 经常性的 要等10几秒
	//改为非阻塞的 2.5秒后还没连接上就 算连接失败;
	int error = -1;int len;
	len = sizeof(int);
	timeval tm;
	fd_set set;
	unsigned long ul = 1;
	ioctlsocket(m_hSocket, FIONBIO, &ul); //设置为非阻塞模式
	bool ret = false;
	if( connect(m_hSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		tm.tv_sec = 3;//4.5s 如果连接不上就 算失败 ，不要重新retry了;
		tm.tv_usec = 500;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		if( select(sockfd+1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
			if(error == 0) 
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

	if(ret)
	{
		return true;
	}
	else
	{
		return false;
	}

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


unsigned long CFlashSN::GetNewSerialNumber()
{
	//Get_SerialNumber_From_Server(_T("192.168.0.222"),31234,_T("TSTAT_Dufan"),6,555,666,_T("Dufan for Test"));
	//return 0;
	int nSerialNumber;		
	CStdioFile file;
			
	if(!file.Open(c_strSNRecordFileName, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("Error: Cannot find serial records file.\n Check Z:\Serial_Records\serial_records.txt"));
		return -1;
	}

	//----------------------------get the serialnumber--------------------------------------
	///////////////////// read a line from file to the lastest line
	CString strLine,strTemp;	


	while(file.ReadString(strTemp))
	{
		strLine = strTemp;		
	}	
	strTemp = strLine.Mid(0, strLine.Find(_T(',')));
	nSerialNumber = _wtoi(strTemp);//the serialnumber
	file.Close();
	return nSerialNumber+1;
}
void CFlashSN::SetMBID(int ID){
m_nMBID=ID;
}
void CFlashSN::SetBrandrate (int nBraudrate)
{
    m_nBraudrate = nBraudrate;
}

void CFlashSN::FlashTstatSN()
{
	//m_nSerialNumber=Get_SerialNumber_From_Server(Auto_flash_SN_connect_IP,31234,m_strProductModel,m_nProductModel,m_nsoftversion,m_nHardWareVersion,m_username);

	m_nSerialNumber=GetNewSerialNumber();// get last serialnumber of the file
	if(m_nSerialNumber<=0)//can't fine the serialnumber file on z drive
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("WARNING : Get serial number from server failed."));
		return;
	}
// 	CString serial_id;
// 	serial_id.Format(_T("Get Serial number from server : %u"),m_nSerialNumber);
// 	AfxMessageBox(serial_id);

	unsigned int loword,hiword;
	loword=m_nSerialNumber & 0xffff;
	hiword=(m_nSerialNumber >> 16) & 0xffff;
	if (open_com(m_nComPort))
	{
      Change_BaudRate (m_nBraudrate);
        if (m_nProductModel!=102)
        {
            Write_One(m_nMBID,16,142);
            Sleep(10*1000);
        }

     
		
	if(Write_One(m_nMBID,0,loword)>0 && Write_One(m_nMBID,2,hiword)>0)
	{		
		int nSWVersion=Read_One(m_nMBID, 4);//tstat version			
		if(nSWVersion >= 240 && nSWVersion < 250)
			nSWVersion /=10;
		else 
		{
			nSWVersion = Read_One(m_nMBID,5)*256+Read_One(m_nMBID,4);	
			nSWVersion /=10;
		}//tstat_version		

		//if(nSWVersion >= 25)
		{
			Write_One(m_nMBID,7,m_nProductModel);//write product_model register
			Write_One(m_nMBID,8,m_nHardWareVersion);//write hardware_rev register
		}
		int year,month,day;
		char software_version[8]={'\0'};
		time_t curtime;
		struct tm* today = NULL;
		curtime = time(NULL);
		today = localtime(&curtime);
	//	localtime_s(today, &curtime);
		year = today->tm_year + 1900;
		month = today->tm_mon + 1;
		day = today->tm_mday;
		//		getTime(&year,&month,&day);//get the month,day,year of now							
		float fl_temp=(float)nSWVersion;
		CString f_l_temp;
		f_l_temp.Format(_T("%.1f"),fl_temp);
		for(int j=0;j<f_l_temp.GetLength();j++)
			software_version[j]=(char)f_l_temp.GetAt(j);

		
		CString stringlog;
		stringlog.Format(_T("%d,%s,%d,%s,%d-%d-%d"),
			m_nSerialNumber,
			m_strProductModel,
			m_nHardWareVersion,
			f_l_temp,
			month,
			day,
			year);
		CStdioFile seriallogfile;
		if(seriallogfile.Open(c_strSNRecordFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone))
		{

			seriallogfile.SeekToEnd();

			seriallogfile.WriteString(_T("\n")+stringlog);



			seriallogfile.Close();
		}
		else
		{
			ASSERT(0);
			seriallogfile.Close();

		}
		CString index;
		index.Format(_T("SN=%d has been written sucessfully."),m_nSerialNumber);
		if(!auto_flash_mode)
			AfxMessageBox(index);
	}
	else
	{
		m_nSerialNumber = 
			Read_One(m_nMBID,0 ) +
			Read_One(m_nMBID,1) * 256 +
			Read_One(m_nMBID,2) * 65536 +           // 256 * 256 
			Read_One(m_nMBID , 3) * 16777216 ;// 256 *256 *256
		CString index;
		index.Format(_T("SN:%d has not been written."),m_nSerialNumber);
		if(!auto_flash_mode)
			AfxMessageBox(index);
	}
	
	close_com();
	}
	else
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("Cannot open COM Port"));
	}

	 
}


void CFlashSN::SetConfigFileHandler(CConfigFileHandler* pFileHandler)
{
	m_pFileHandler = pFileHandler;
}
void CFlashSN::FlashNCSN()
{
	m_nSerialNumber=Get_SerialNumber_From_Server(Auto_flash_SN_connect_IP,31234,m_strProductModel,m_nProductModel,m_nsoftversion,m_nHardWareVersion,m_username);
	if(m_nSerialNumber<=0)//can't fine the serialnumber file on z drive
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("WARNING : Get serial number from the server failed."));
		return;
	}
	CString serial_id;
	serial_id.Format(_T("Got Serial number from server : %u"),m_nSerialNumber);
	if(!auto_flash_mode)
		AfxMessageBox(serial_id);
	//m_nSerialNumber=GetNewSerialNumber();// get last serialnumber of the file
	unsigned int loword,hiword,loword0,loword1,hiword2,hiword3;
	loword=m_nSerialNumber & 0xffff;

	hiword=(m_nSerialNumber >> 16) & 0xffff;

	loword0=loword&0xff;
	loword1=(loword>>8)&0xff;

	hiword2=hiword&0xff;
	hiword3=(hiword>>8)&0xff;
	if(m_nSerialNumber==-1)//can't fine the serialnumber file on z drive
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("WARNING : Can't find serial number file on Z Drive."));
		return;
	}
	
	if (!Open_Socket2(m_dwIPAddr,m_nIPPort))
	{
	  CString strmessage;
	  strmessage.Format(_T("Can't Open %s:%d"),m_dwIPAddr,m_nIPPort);
	  if(!auto_flash_mode)
		AfxMessageBox(strmessage);
	} 
	
	else
	{

		if((Write_One(255,0,loword0)>0) && (Write_One(255,1,loword1)>0)&&(Write_One(255,2,hiword2)>0) && (Write_One(255,3,hiword3)>0))
		{		
			int nSWVersion=Read_One(255, 4);//tstat version			
			if(nSWVersion >= 240 && nSWVersion < 250)
				nSWVersion /=10;
			else 
			{
				nSWVersion = Read_One(255,5)*256+Read_One(255,4);	
				nSWVersion /=10;
			}//tstat_version		

			if(nSWVersion >= 25)
			{
				Write_One(255,7,m_nProductModel);//write product_model register
				Write_One(255,8,m_nHardWareVersion);//write hardware_rev register
			}
			int year,month,day;
			char software_version[8]={'\0'};
			time_t curtime;
			struct tm* today = NULL;
			curtime = time(NULL);
			today = localtime(&curtime);
			//	localtime_s(today, &curtime);
			year = today->tm_year + 1900;
			month = today->tm_mon + 1;
			day = today->tm_mday;
			//		getTime(&year,&month,&day);//get the month,day,year of now							
			float fl_temp=(float)nSWVersion;
			CString f_l_temp;
			f_l_temp.Format(_T("%.1f"),fl_temp);
			for(int j=0;j<f_l_temp.GetLength();j++)
				software_version[j]=(char)f_l_temp.GetAt(j);

			

#pragma region Save_SerialNumber
			CString stringlog;
			stringlog.Format(_T("%d,%s,%d,%s,%d-%d-%d"),
				m_nSerialNumber,
				m_strProductModel,
				m_nHardWareVersion,
				f_l_temp,
				month,
				day,
				year);
			CStdioFile seriallogfile;
			if(seriallogfile.Open(c_strSNRecordFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone))
			{

				seriallogfile.SeekToEnd();

				seriallogfile.WriteString(_T("\n")+stringlog);



				seriallogfile.Close();
			}
			else
			{
				ASSERT(0);
				seriallogfile.Close();

			}
			CString index;
			index.Format(_T("SN=%d have been written,sucessfully."),m_nSerialNumber);
			if(!auto_flash_mode)
				AfxMessageBox(index);
		}
		else
		{
			m_nSerialNumber = 
				Read_One(255,0 ) +
				Read_One(255,1) * 256 +
				Read_One(255,2) * 65536 +           // 256 * 256 
				Read_One(255 , 3) * 16777216 ;// 256 *256 *256
			CString index;
			index.Format(_T("SN:%d has not been written."),m_nSerialNumber);
			if(!auto_flash_mode)
				AfxMessageBox(index);
		}
#pragma endregion Save_SerialNumber
			
	   close_com;

	}

}
 
int CFlashSN::GetSerialNumber()
{

 return m_nSerialNumber;
}