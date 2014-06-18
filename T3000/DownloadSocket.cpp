#include "StdAfx.h"
#include "DownloadSocket.h"
#include "gloab_define.h"
extern char receive_buffer[4000];
extern unsigned short totalpackage;
extern unsigned short current_package;
extern char download_filename[20];
extern char receive_md5[20];
extern int download_step ;
int Receive_data_length = 0;

extern int total_file_length;	
int malloc_download_memory_size = 0;
char * receivefile_buffer = NULL;

DownloadSocket::DownloadSocket(void)
{
}


DownloadSocket::~DownloadSocket(void)
{
}


void DownloadSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	memset(receive_buffer,0,4000);
	Receive_data_length=Receive(receive_buffer,4000);

	if(((unsigned char)receive_buffer[0]!= 0x55) || ((unsigned char)receive_buffer[1] !=0xff))
	{
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
		
	char *temp_point = receive_buffer;
	temp_point = temp_point + 4;

	int crc_add = 0;
	unsigned char crc_result = 0;
	for (int i=0;i<Receive_data_length-1;i++)
	{
		crc_add = crc_add + receive_buffer[i];
	}
	crc_result = crc_add % 256;

	unsigned char ncommand  = *(temp_point++);
	switch(ncommand)
	{
	case RETURN_FILE_SIZE:
		{
			if(Receive_data_length !=sizeof(Download_File_Return))
			{
				break;
			}
			totalpackage = ((unsigned char)temp_point[1]<<8) | ((unsigned char)temp_point[0]);
			temp_point = temp_point + 2;
			memcpy(download_filename,temp_point,20);
			download_step = SEND_GET_MD5_VALUE;

			malloc_download_memory_size = totalpackage  * TFTP_SEND_LENGTH;
			receivefile_buffer =  new char[malloc_download_memory_size];
			memset(receivefile_buffer,0,malloc_download_memory_size);
			PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_FILE_INFO,NULL);
		}
		break;
	case  ACK_MD5_VALUE:
		{
			memset(receive_md5,0,20);
			if(Receive_data_length != 26)
			{
				break;
			}
			memcpy_s(receive_md5,20,temp_point,20);
			temp_point = temp_point + 20;
			download_step = CHECK_MD5_VALUE;
		}
		break;
	case  TRANSFER_FILE_PAGE:
		{
			int ntemp_receive_package = ((unsigned char)temp_point[1]<<8) | ((unsigned char)temp_point[0]);
			temp_point = temp_point + 2;
			int ntemptotalpackage = ((unsigned char)temp_point[1]<<8) | ((unsigned char)temp_point[0]);
			temp_point = temp_point + 2;
			if((ntemptotalpackage != totalpackage) || (ntemp_receive_package > totalpackage))
				break;
			if(ntemp_receive_package != current_package)
				break;
			if(ntemp_receive_package < ntemptotalpackage)
			{
				memcpy_s(receivefile_buffer + TFTP_SEND_LENGTH * (ntemp_receive_package - 1),
					TFTP_SEND_LENGTH,temp_point,TFTP_SEND_LENGTH);
				download_step = RECEIVE_WANTED_PACKAGE;
				current_package = ntemp_receive_package + 1;
			}
			else
			{
				temp_point = temp_point - 3 - 4;	//需要回过头去得到 此包的长度;
				
				int nlength;
				nlength = ((unsigned char)temp_point[1]<<8) | ((unsigned char)temp_point[0]);
				nlength = nlength - 4 - 4;
				if(nlength <=0 )
					break;
				total_file_length = (ntemptotalpackage - 1) * TFTP_SEND_LENGTH + nlength ;
				temp_point = temp_point + 3 + 4;
				memcpy_s(receivefile_buffer + TFTP_SEND_LENGTH * (ntemp_receive_package - 1),
					nlength,temp_point,nlength);
				download_step = RECEIVE_COMPLET;
			}

			break;
		}

	case  DOWNLOAD_FILE_ERROR:
		{
			SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
			PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,RETURN_ERROR,DOWNLOAD_FILE_ERROR);
			download_step = THREAD_IDLE;
		}
		break;
	case  HOST_BUSY:
		{
			SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
			PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,RETURN_ERROR,HOST_BUSY);
			download_step = THREAD_IDLE;
		}
		break;
	case NO_MD5_FILE_EXSIT:
		{
			SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
			PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,RETURN_ERROR,NO_MD5_FILE_EXSIT);
			download_step = THREAD_IDLE;
		}
		break;
	default :
		{
			SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
			PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,RETURN_ERROR,NULL);
			download_step = THREAD_IDLE;
		}
		break;
	}

	CAsyncSocket::OnReceive(nErrorCode);
}
void DownloadSocket::SetParentWindow(HWND n_hwnd)
{
	m_parent_hwnd = n_hwnd;
}

void DownloadSocket::SetDownloadResults(int ret_results)
{
	m_results = ret_results;
}

int DownloadSocket::GetDownloadResults()
{
	return	m_results ;
}

void DownloadSocket::OnConnect(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	LPVOID	lpMsgBuf;
	// TODO: Add your specialized code here and/or call the base class
	if(!nErrorCode)
	{
		SetDownloadResults(DOWNLOAD_RESULTS_UNKNOW);
		PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_CONNECT_SUCCESS,NULL);
	}
	else
	{

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,nErrorCode,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);
		//MessageBox((LPCTSTR)lpMsgBuf,"Error",MB_ICONERROR|MB_OK);
		CString strMsg;
		strMsg.Format(_T("Link Failed!\n\n%*sError ID:%d.Error Description:%s"),4,_T(""),nErrorCode,lpMsgBuf);
		AfxMessageBox(strMsg);
		PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_CONNECT_FAILED,NULL);

		//dlg->m_list_receive_box.InsertString(0,strMsg);
	}

	CAsyncSocket::OnConnect(nErrorCode);
}


void DownloadSocket::OnClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	PostMessage(m_parent_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_DISCONNEC,NULL);
	CAsyncSocket::OnClose(nErrorCode);
}
