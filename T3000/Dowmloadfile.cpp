// Dowmloadfile.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Dowmloadfile.h"
#include "afxdialogex.h"
#include "globle_function.h"
#include "MainFrm.h"
#include "Class/md5.h"
// Dowmloadfile dialog
#define DOWNLOAD_AND_UPDATE  1
#define DOWNLOAD_ONLY        2
int download_and_update = 0;

extern vector <Str_download_firmware_info> download_info_type;

BYTE IP_ADDRESS_SERVER[20];
int total_file_length = 0;	
int Add_log_count = 1;	//用于读取改读多少count了;
int replace_count = 1;
CString AutoFlashConfigPath;
CString ISPtool_path;
tree_product	m_product_isp_auto_flash;
//int isp_product_id = 0;
char receive_buffer[4000];
unsigned short totalpackage = 0;
unsigned short current_package = 1;
char download_filename[40];
int download_step = SEND_DOWNLOAD_COMMAND;
bool download_thread_flag = true;
int wait_download_and_isp_finished = 0 ;
char receive_md5[20];
HWND downloadfile_hwnd;
CString Folder_Path;
CString HEX_BIN_FILE_PATH;
extern char * receivefile_buffer;

IMPLEMENT_DYNAMIC(Dowmloadfile, CDialogEx)

Dowmloadfile::Dowmloadfile(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dowmloadfile::IDD, pParent)
{

}

Dowmloadfile::~Dowmloadfile()
{
}

void Dowmloadfile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DOWNLOAD_INFO, m_download_info);
}


BEGIN_MESSAGE_MAP(Dowmloadfile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START_DOWNLOAD, &Dowmloadfile::OnBnClickedButtonStartDownload)
	ON_MESSAGE(WM_DOWNLOADFILE_MESSAGE,DownloadFileMessage)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &Dowmloadfile::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_FILE_DOWNLOAD_ONLY, &Dowmloadfile::OnBnClickedButtonFileDownloadOnly)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_T3000, &Dowmloadfile::OnBnClickedButtonUpdateT3000)
END_MESSAGE_MAP()

HANDLE Downloadfile_Thread;
DWORD download_threadid;
extern int malloc_download_memory_size;
// Dowmloadfile message handlers
LRESULT Dowmloadfile::DownloadFileMessage(WPARAM wParam,LPARAM lParam)
{
	//static bool insert_a_new_line = false;
	int ncommand = (int)wParam;
	if(ncommand == DOWNLOAD_CONNECT_SUCCESS)
	{
		//unsigned char IPAddress[4];
		//((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->GetAddress(IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
		CString retmessage;
		retmessage.Format(_T("Connect to server success!"));
		//m_download_info.AddString(retmessage);
		m_download_info.InsertString(m_download_info.GetCount(),retmessage);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
		if(Downloadfile_Thread == NULL)
			Downloadfile_Thread = CreateThread(NULL,NULL,DownLoadFileProcess,this,NULL, &download_threadid);

	}
	else if(ncommand == DOWNLOAD_DISCONNEC)
	{
		CString retmessage;
		retmessage.Format(_T("Disconnected with server!"));
		//m_download_info.AddString(retmessage);
		m_download_info.InsertString(m_download_info.GetCount(),retmessage);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
		GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
		TCP_File_Socket.Close();
	}
	else if(ncommand == DOWNLOAD_CONNECT_FAILED)
	{

		CString retmessage;
		retmessage.Format(_T("Connect to server failed!Please try again!"));
		//m_download_info.AddString(retmessage);
		m_download_info.InsertString(m_download_info.GetCount(),retmessage);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
		TCP_File_Socket.Close();
		GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
	}
	else if(ncommand == DOWNLOAD_CLOSE_SOCKET)
	{
		TCP_File_Socket.Close();
		
		if(TCP_File_Socket.GetDownloadResults() == DOWNLOAD_RESULTS_FAILED)
		{
			GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
			return 0;
		}

		CString ApplicationFolder;
		GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
		PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
		ApplicationFolder.ReleaseBuffer();
		ISPtool_path = ApplicationFolder + _T("\\ISP.exe");

		AutoFlashConfigPath = ApplicationFolder + _T("//AutoFlashFile.ini");

		CString temp_isp_info;
		CString exe_folder;
		GetModuleFileName(NULL, exe_folder.GetBuffer(MAX_PATH), MAX_PATH);
		PathRemoveFileSpec(exe_folder.GetBuffer(MAX_PATH));
		exe_folder.ReleaseBuffer();

		CString file_name;
		MultiByteToWideChar( CP_ACP, 0, download_filename, (int)strlen((char *)download_filename)+1, file_name.GetBuffer(MAX_PATH), MAX_PATH );
		file_name.ReleaseBuffer();
		CString mypath =  exe_folder + _T("\\Database\\Firmware");
		mypath = mypath + _T("\\") + file_name;
		WritePrivateProfileStringW(_T("Data"),_T("FirmwarePath"),mypath,AutoFlashConfigPath);


		temp_isp_info.Format(_T("FirmwarePath = "));
		temp_isp_info = temp_isp_info + mypath;
		m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
		if(m_download_product_type == 199) //如果下载的是T3000;
		{
			//调用解压;
		}
		if(download_and_update == DOWNLOAD_ONLY)
		{
			if(flash_multi_auto)
			{
				//保存下载的位置,产品号 等信息;

				for (int z=0;z<download_info_type.size();z++)
				{
					if(m_download_product_type == download_info_type.at(z).download_product_type)
					{
						WideCharToMultiByte( CP_ACP, 0, mypath.GetBuffer(), -1, download_info_type.at(z).firmware_file_path, 255, NULL, NULL );
						break;
					}
				}
				Sleep(1000);
				PostMessage(WM_CLOSE,NULL,NULL);

			}
			GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
			return 0;
		}
		

		bool is_sub_device = false;
		CString temp_deal_ip =	m_product_isp_auto_flash.BuildingInfo.strIp;
		if(!temp_deal_ip.IsEmpty())
		{
			CStringArray temparray;
			SplitCStringA(temparray,temp_deal_ip,_T("."));
			if(temparray.GetSize()==4)
			{
				if(m_product_isp_auto_flash.note_parent_serial_number == 0)
					is_sub_device  = false;
				else
					is_sub_device = true;
			}
		}

		
		//bool is_sub_device = (bool)IsNetDevice(m_product_isp_auto_flash.product_class_id);


		WritePrivateProfileStringW(_T("Data"),_T("Command"),_T("1"),AutoFlashConfigPath);
		if(m_product_isp_auto_flash.BuildingInfo.strIp.IsEmpty())//串口
		{
			
			 WritePrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("COM"),AutoFlashConfigPath);
			 CString cs_comport;
			 cs_comport.Format(_T("COM%d"), m_product_isp_auto_flash.ncomport);
			 WritePrivateProfileStringW(_T("Data"),_T("COMPORT"),cs_comport,AutoFlashConfigPath);
			 WritePrivateProfileStringW(_T("Data"),_T("Baudrate"),_T("19200"),AutoFlashConfigPath);

			 CString nflash_id;
			 nflash_id.Format(_T("%d"),m_product_isp_auto_flash.product_id);
			 WritePrivateProfileStringW(_T("Data"),_T("ID"),nflash_id,AutoFlashConfigPath);

			 temp_isp_info.Format(_T("ISP via : "));
			 temp_isp_info = temp_isp_info + cs_comport;
			 m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
			 temp_isp_info.Format(_T("ISP baudrate : 19200"));
			 m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
			 temp_isp_info.Format(_T("Device ID :"));
			 temp_isp_info = temp_isp_info + nflash_id;
			 m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
		}
		else
		{
			 WritePrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("NET"),AutoFlashConfigPath);
			 WritePrivateProfileStringW(_T("Data"),_T("IPAddress"),m_product_isp_auto_flash.BuildingInfo.strIp,AutoFlashConfigPath);
			 if((m_product_isp_auto_flash.ncomport == 0) || (m_product_isp_auto_flash.ncomport == 47808))
			 {
				 m_product_isp_auto_flash.ncomport = 10000;
			 }
			 temp_isp_info.Format(_T("ISP via : network"));
			 m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
			  temp_isp_info.Format(_T("IP Address : "));
			  temp_isp_info = temp_isp_info + m_product_isp_auto_flash.BuildingInfo.strIp;
			   m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
			   

			 CString n_tcpport;
			 n_tcpport.Format(_T("%d"),m_product_isp_auto_flash.ncomport);
			 
			  temp_isp_info.Format(_T("Port : "));
			  temp_isp_info = temp_isp_info + n_tcpport;
			  m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);

			 WritePrivateProfileStringW(_T("Data"),_T("IPPort"),n_tcpport,AutoFlashConfigPath);
			 if(is_sub_device)
			 {
				   WritePrivateProfileStringW(_T("Data"),_T("Subnote"),_T("1"),AutoFlashConfigPath);
				   CString nsub_id;
				   nsub_id.Format(_T("%d"),m_product_isp_auto_flash.product_id);
				   WritePrivateProfileStringW(_T("Data"),_T("SubID"),nsub_id,AutoFlashConfigPath);

				   temp_isp_info.Format(_T("Device is subnote."));
				   m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
			 }
			 else
			 {
				  WritePrivateProfileStringW(_T("Data"),_T("Subnote"),_T("0"),AutoFlashConfigPath);
			 }

			 temp_isp_info.Format(_T("Please wait!ISP is running!"));
			 m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
		}
		//CString exe_folder;
		//GetModuleFileName(NULL, exe_folder.GetBuffer(MAX_PATH), MAX_PATH);
		//PathRemoveFileSpec(exe_folder.GetBuffer(MAX_PATH));
		//exe_folder.ReleaseBuffer();

		//CString file_name;
		//MultiByteToWideChar( CP_ACP, 0, download_filename, (int)strlen((char *)download_filename)+1, file_name.GetBuffer(MAX_PATH), MAX_PATH );
		//file_name.ReleaseBuffer();
		//CString mypath =  exe_folder + _T("\\Database\\Firmware");
		//mypath = mypath + _T("\\") + file_name;
		//WritePrivateProfileStringW(_T("Data"),_T("FirmwarePath"),mypath,AutoFlashConfigPath);
		//

		//temp_isp_info.Format(_T("FirmwarePath = "));
		//temp_isp_info = temp_isp_info + mypath;
		//m_download_info.InsertString(m_download_info.GetCount(),temp_isp_info);
		//m_download_info.SetTopIndex(m_download_info.GetCount()-1);

		HANDLE Call_ISP_Application = NULL;
		Call_ISP_Application =CreateThread(NULL,NULL,isp_thread,this,NULL, NULL);
		
		
	}
	else if(ncommand == DOWNLOAD_PERSENT)
	{

		int npersent = (int)lParam;
		CString persent_message;
		persent_message.Format(_T("File download finished %d%%"),npersent);
		//bool is_the_first = true;
		//if(!is_the_first)
		m_download_info.DeleteString(m_download_info.GetCount() - 1);

		//m_download_info.AddString(persent_message);
		m_download_info.InsertString(m_download_info.GetCount(),persent_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == DOWNLOAD_FINISHED)
	{
		CString complete_message;
		complete_message.Format(_T("File download complete. "));
		//m_download_info.AddString(complete_message);
		m_download_info.InsertString(m_download_info.GetCount(),complete_message);

		complete_message.Format(_T("Local file path : "));
		complete_message = complete_message + Folder_Path;
		//m_download_info.InsertString(m_download_info.GetCount(),complete_message);

		//m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == DOWNLOAD_FILE_INFO)
	{
		CString nfile_name;
		CString nfile_size;
		MultiByteToWideChar( CP_ACP, 0, download_filename, (int)strlen((char *)download_filename)+1, nfile_name.GetBuffer(MAX_PATH), MAX_PATH );
		nfile_name.ReleaseBuffer();
		nfile_size.Format(_T("File size about %d Bytes "),malloc_download_memory_size);
		CString show_message;
		show_message = _T("File name  ") + nfile_name + _T("   .") + nfile_size;
		m_download_info.InsertString(m_download_info.GetCount(),show_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == DOWNLOAD_T3000_NO_UPDATE)
	{
		CString show_message;
		show_message = _T("You T3000 is up-to-date") ;
		m_download_info.InsertString(m_download_info.GetCount(),show_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == DOWNLOAD_NOT_FIND_LOCAL)
	{
		CString complete_message;
		complete_message.Format(_T("Local firmware file doesn’t exist, downloading from server now."));
		//m_download_info.AddString(complete_message);
		m_download_info.InsertString(m_download_info.GetCount(),complete_message);
		//m_download_info.SetTopIndex(m_download_info.GetCount()-1);


		complete_message.Format(_T("Check MD5 value over!"));
		//m_download_info.AddString(complete_message);
		m_download_info.InsertString(m_download_info.GetCount(),complete_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == DOWNLOAD_LOCAL_EXSIT)
	{



		CString complete_message;
		complete_message.Format(_T("Local Firmware  already exsit in the Firmware folder.The MD5 value is match "));
		//m_download_info.AddString(complete_message);
		m_download_info.InsertString(m_download_info.GetCount(),complete_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == DOWNLOAD_RESULTS/*DOWNLOAD_RESULTS*/)
	{
		GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
		KillTimer(1);
		int ret_results =  GetPrivateProfileInt(_T("Data"),_T("Command"),0,AutoFlashConfigPath);
		CString ret_message;
		switch(ret_results)
		{
		case NO_COMMAND:
		case START_AUTO_FLASH_COMMAND:
		case FAILED_NORESPONSED:
			ret_message.Format(_T("The device not response ,please try again!"));
			//m_download_info.AddString(complete_message);
			m_download_info.InsertString(m_download_info.GetCount(),ret_message);
			m_download_info.SetTopIndex(m_download_info.GetCount()-1);
			break;
		case FLASH_SUCCESS:
			{
				ret_message.Format(_T("ISP succeed!"));
				//m_download_info.AddString(complete_message);
				m_download_info.InsertString(m_download_info.GetCount(),ret_message);
				m_download_info.SetTopIndex(m_download_info.GetCount()-1);
			}
			break;
		case FAILED_UNKNOW_ERROR:
			{
				ret_message.Format(_T("Unknown error! More details please reference Log_info.txt !"));
				//m_download_info.AddString(complete_message);
				m_download_info.InsertString(m_download_info.GetCount(),ret_message);
				m_download_info.SetTopIndex(m_download_info.GetCount()-1);
			}
			break;
		}

		CFileFind configfind;
		if(configfind.FindFile(AutoFlashConfigPath))
		{
			DeleteFile(AutoFlashConfigPath);
		}
	}
	else if(ncommand == RETURN_ERROR)
	{
		CString show_error_message;
		int file_error_type = (int)lParam;
		bool download_failed = false;
		switch(file_error_type)
		{
		case NO_MD5_FILE_EXSIT:
			{
				show_error_message.Format(_T("Sorry, the firmware update service is temporarily offline or your file cannot be found."));

				m_download_info.InsertString(m_download_info.GetCount(),show_error_message);
				m_download_info.SetTopIndex(m_download_info.GetCount()-1);

				show_error_message.Format(_T(" Please navigate to this link and manually download the firmware:http://www.temcocontrols.com/ftp/firmware/"));
				download_failed = true;
			}
			break;
		case DOWNLOAD_FILE_ERROR:
			{
				show_error_message.Format(_T("Download file from server failed!"));
				download_failed = true;
			}

			break;
		case HOST_BUSY:
			{
				show_error_message.Format(_T("Host busy,please try again later!"));
				download_failed = true;
			}
			break;
		case DOWNLOAD_MD5_FAILED:
			{
				show_error_message.Format(_T("Check MD5 value failed!Please retry again!"));
				download_failed = true;
			}
			break;
		case DOWNLOAD_MD5_CHECK_PASS:
			{
				 string temp_md5 = MD5(ifstream( HEX_BIN_FILE_PATH )).toString();
				 CString MD5_value;
				 MD5_value = temp_md5.c_str();
				 CString temp_show;
				 temp_show.Format(_T("The File MD5 is :"));
				 temp_show = temp_show + MD5_value;
				 m_download_info.InsertString(m_download_info.GetCount(),temp_show);
				 show_error_message.Format(_T("Check MD5 value with server Pass."));
				 download_failed = false;
			}
			break;
		default:
			{
				show_error_message.Format(_T("Unknown error,please try again later!"));
				download_failed = true;
			}
			break;

		}

		if(download_failed)
		{
			if(flash_multi_auto)
			{
				PostMessage(WM_CLOSE,NULL,NULL);
			}
			else
			{
				if(MessageBox(_T("Do you want download firmware manually from website?"),_T("Download"),MB_YESNO) == IDYES)
				{
					ShellExecute(NULL, L"open", L"http://www.temcocontrols.com/ftp/firmware/", NULL, NULL, SW_SHOWNORMAL);
				}

			}
		}
		m_download_info.InsertString(m_download_info.GetCount(),show_error_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	return 0;
}

DWORD WINAPI  Dowmloadfile::isp_thread(LPVOID lpVoid)
{
	//Write_Config_Info
	Dowmloadfile *pParent = (Dowmloadfile *)lpVoid;
	WinExecAndWait(ISPtool_path,NULL,NULL,0);

	::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_RESULTS,NULL);
	return 0;
}


unsigned char Add_CRC(char *point , int nlength)
{
	int add_value = 0;
	for (int i=0;i<nlength;i++)
	{
		add_value = add_value + point[i];
	}
	return (add_value % 256);
}

DWORD WINAPI   Dowmloadfile::DownLoadFileProcess(LPVOID lpVoid)
{
	Dowmloadfile *mparent = (Dowmloadfile *)lpVoid;
	int retry_time = 0;
	//int retry_send_
	char sendbuffer[1000];
	while(download_thread_flag)
	{
		
		switch(download_step)
		{
		case SEND_DOWNLOAD_COMMAND:
			{
				if(retry_time>10)
				{
					mparent->TCP_File_Socket.SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
					download_step = THREAD_IDLE;
					continue;
				}
				retry_time ++;

				Download_Info temp_info;
				temp_info.HEAD_1 = 0x55;
				temp_info.HEAD_2 = 0xff;
				temp_info.length = sizeof(Download_Info) - 2;
				temp_info.commad = DOWNLOAD_FILE;
				temp_info.product_id = m_product_isp_auto_flash.product_class_id;
				temp_info.get_newest = 1;
				temp_info.file_type = 2;
				temp_info.softversion = 0;
				temp_info.CRC = 0xff;
				memset(sendbuffer,0,1000);
				memcpy(sendbuffer,&temp_info,sizeof(Download_Info));
				sendbuffer[sizeof(Download_Info) - 1] = Add_CRC(sendbuffer,sizeof(Download_Info));
				mparent->TCP_File_Socket.Send(sendbuffer,sizeof(Download_Info),0);
				for (int i=0;i<1000;i++)
				{
					if(download_step!= SEND_DOWNLOAD_COMMAND)
					{
						retry_time = 0;
						current_package = 1;
						break;
					}
					Sleep(1);
				}

				break;
			}
		case SEND_GET_MD5_VALUE:
			{
				if(retry_time>10)
				{
					mparent->TCP_File_Socket.SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
					download_step = THREAD_IDLE;
					continue;
				}
				retry_time ++;

				Black_head_struct temp_info;
				temp_info.HEAD_1 = 0x55;
				temp_info.HEAD_2 = 0xff;
				temp_info.length = 4;
				temp_info.commad = GET_MD5_VALUE;
				memset(sendbuffer,0,1000);
				memcpy(sendbuffer,&temp_info,sizeof(temp_info));
				sendbuffer[5] = Add_CRC(sendbuffer,5);
				mparent->TCP_File_Socket.Send(sendbuffer,6,0);
				for (int i=0;i<1000;i++)
				{
					if(download_step!= SEND_GET_MD5_VALUE)
					{
						current_package = 1;
						retry_time = 0;
						break;
					}
					Sleep(1);
				}
			}
			break;
		case CHECK_MD5_VALUE:
			{
				CString ApplicationFolder;
				CString temp_file_path;
				CString file_name;
				GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
				PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
				ApplicationFolder.ReleaseBuffer();
				temp_file_path = ApplicationFolder + _T("\\Database\\Firmware");
				MultiByteToWideChar( CP_ACP, 0, download_filename, (int)strlen((char *)download_filename)+1, file_name.GetBuffer(MAX_PATH), MAX_PATH );
				file_name.ReleaseBuffer();
				temp_file_path = temp_file_path + _T("\\") + file_name; 
				CFileFind findfirmwarefile;
				if(!findfirmwarefile.FindFile(temp_file_path))	//没有发现文件 就要服务器传过来;
				{
					::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_NOT_FIND_LOCAL,NULL);
					download_step = START_SEND_WANT_PACKAGE;
				}
				else	//发现有一样的了就要比对MD5是否一致;
				{
					//CString remote_md5;
					//CString local_md5;
					//MultiByteToWideChar( CP_ACP, 0, receive_md5, (int)strlen((char *)receive_md5)+1, remote_md5.GetBuffer(MAX_PATH), MAX_PATH );
					//remote_md5.ReleaseBuffer();

					char locoal_char_md5[20];
					memcpy_s(locoal_char_md5,20, MD5(ifstream( temp_file_path )).digest(),20);
					int ret_value = memcmp(locoal_char_md5,receive_md5,16);
					//string local_string_md5 = MD5(ifstream( temp_file_path )).toString();
					//local_md5 = local_string_md5.c_str();
					if(ret_value == 0)	//MD5值一样就不用下载 直接结束;
					{
						::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_LOCAL_EXSIT,NULL);
						//::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_CLOSE_SOCKET,NULL);
						download_step = THREAD_IDLE;
					}
					else	// 不一致就要再次下载;
					{
						::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_NOT_FIND_LOCAL,NULL);
						download_step = START_SEND_WANT_PACKAGE;
					}
				}
				
			}
			break;
		case START_SEND_WANT_PACKAGE:
			{
				if(retry_time>10)
				{
					mparent->TCP_File_Socket.SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
					download_step = THREAD_IDLE;
					continue;
				}
				retry_time ++;

				File_Package_Struct temp_struct;
				temp_struct.HEAD_1 = 0x55;
				temp_struct.HEAD_2 = 0xff;
				temp_struct.length = sizeof(File_Package_Struct) - 2;
				temp_struct.commad = ACK_GET_FILE_PAGE;
				temp_struct.package_index = current_package;
				temp_struct.total_package = totalpackage;
				temp_struct.CRC = 0;
				memset(sendbuffer,0,1000);
				memcpy(sendbuffer,&temp_struct,sizeof(File_Package_Struct));
				sendbuffer[sizeof(File_Package_Struct) - 1] = Add_CRC(sendbuffer,sizeof(File_Package_Struct));
				mparent->TCP_File_Socket.Send(sendbuffer,sizeof(File_Package_Struct),0);
				for (int i=0;i<1000;i++)
				{
					if(download_step!= START_SEND_WANT_PACKAGE)
					{
						int npersent = (current_package * 100) / totalpackage;
						retry_time = 0;
						::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_PERSENT,npersent);
						break;
					}
					Sleep(1);
				}
				break;
			}
		case RECEIVE_WANTED_PACKAGE:
			{
				download_step = START_SEND_WANT_PACKAGE;

				break;
			}
		case RECEIVE_COMPLET:
			{


				CString file_name;
				MultiByteToWideChar( CP_ACP, 0, download_filename, (int)strlen((char *)download_filename)+1, file_name.GetBuffer(MAX_PATH), MAX_PATH );
				file_name.ReleaseBuffer();
				CString ApplicationFolder;
				download_thread_flag = true;
				GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
				PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
				ApplicationFolder.ReleaseBuffer();
				Folder_Path = ApplicationFolder + _T("\\Database\\Firmware");

				Folder_Path = Folder_Path + _T("\\") + file_name; 
				HEX_BIN_FILE_PATH = Folder_Path;
				HANDLE hFile;
				hFile=CreateFile(Folder_Path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				DWORD dWrites;
				WriteFile(hFile,receivefile_buffer,total_file_length,&dWrites,NULL);
				CloseHandle(hFile);  




				if(receivefile_buffer)
				{
					delete receivefile_buffer;
					receivefile_buffer = NULL;
					total_file_length = 0;	//开辟的内存空间大小;
				}
				download_step = THREAD_IDLE;

				char locoal_char_md5[20];
				memcpy_s(locoal_char_md5,20, MD5(ifstream( HEX_BIN_FILE_PATH )).digest(),20);
				int ret_value = memcmp(locoal_char_md5,receive_md5,16);
				if(ret_value != 0)
				{
					::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,RETURN_ERROR,DOWNLOAD_MD5_FAILED);
					mparent->TCP_File_Socket.SetDownloadResults(DOWNLOAD_RESULTS_FAILED);
				}
				else
				{
					::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,RETURN_ERROR,DOWNLOAD_MD5_CHECK_PASS);
					mparent->TCP_File_Socket.SetDownloadResults(DOWNLOAD_RESULTS_PASS);
					
				}

				::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_FINISHED,NULL);
				break;
			}
		case THREAD_IDLE:
			{
				//mparent->TCP_File_Socket.ShutDown();
				//mparent->TCP_File_Socket.Close();
				::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_CLOSE_SOCKET,NULL);
				Downloadfile_Thread = NULL;
				retry_time = 0;
				Sleep(1000);
				return 0;
			}
			break;
		}
		Sleep(1);
	}
	return 0;
}


BOOL Dowmloadfile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	((CComboBox *)GetDlgItem(IDC_COMBO_UPDATE_TYPE))->AddString(_T("Bootloader"));
	((CComboBox *)GetDlgItem(IDC_COMBO_UPDATE_TYPE))->AddString(_T("Main Firmware"));
	((CComboBox *)GetDlgItem(IDC_COMBO_UPDATE_TYPE))->SetCurSel(1);
	((CEdit *)GetDlgItem(IDC_EDIT_SERVER_DOMAIN))->SetWindowText(_T("Firmware Server"));
		
	wait_download_and_isp_finished = false;
	CString temp_id;CString temp_name;
	temp_id.Format(_T("%d"),m_product_isp_auto_flash.product_class_id);
	temp_name = GetProductName(m_product_isp_auto_flash.product_class_id);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_ID))->SetWindowTextW(temp_id);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_ID))->EnableWindow(false);
	//((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->SetAddress(114,93,27,13);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_NAME))->SetWindowTextW(temp_name);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_NAME))->EnableWindow(false);
	//int resualt=TCP_File_Socket.Create(0,SOCK_STREAM);//SOCK_STREAM
	downloadfile_hwnd = this->m_hWnd;
	//TCP_File_Socket.SetParentWindow(downloadfile_hwnd);
	Downloadfile_Thread = NULL;
	CString ApplicationFolder;
	download_thread_flag = true;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	Folder_Path = ApplicationFolder + _T("\\Database\\Firmware");
	if(!DirectoryExist(Folder_Path))
	{
		CreateDirectory(Folder_Path);
	}

	m_download_product_type = (unsigned char)m_product_isp_auto_flash.product_class_id;
	if((m_download_product_type == 0) || (m_download_product_type >= 200))
	{
		PostMessage(WM_CLOSE,NULL,NULL);
	}
	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");

	int is_local_temco_net = false;
	is_local_temco_net  = GetPrivateProfileInt(_T("Setting"),_T("LocalTemcoNet"),0,temp_db_ini_folder);
	if(is_local_temco_net == false)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("LocalTemcoNet"),_T("0"),temp_db_ini_folder);
		WritePrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_db_ini_folder);
	}

	//hostent* host = gethostbyname("www.temcocontrols.poweredbyclear.com");  //这是以前的域名 
	//hostent* host = gethostbyname("14i5f38013.iok.la");

	if(is_local_temco_net == false)
	{

		hostent* host = gethostbyname("newfirmware.com");
		if(host == NULL)
		{
			if(!flash_multi_auto)
				MessageBox(_T("Connect firmware server failed.  \r\nPlease check your internet connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_CLOSE,NULL,NULL);
			return TRUE;
		}
		char* pszIP  = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr)); 
		memcpy_s((char *)IP_ADDRESS_SERVER,20,pszIP,20);

		CString temp_message;
		temp_message.Format(_T("Internet access!"));
		m_download_info.InsertString(m_download_info.GetCount(),temp_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else
	{
		CString temp_ip;
		GetPrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_ip.GetBuffer(MAX_PATH),MAX_PATH,temp_db_ini_folder);
		temp_ip.ReleaseBuffer();

		char temp_char_ip[40];
		memset(temp_char_ip,0,40);
		WideCharToMultiByte( CP_ACP, 0, temp_ip.GetBuffer(), -1, temp_char_ip, 255, NULL, NULL );
		memcpy_s((char *)IP_ADDRESS_SERVER,20,temp_char_ip,20);
	}
//#ifdef _DEBUG
//	memcpy_s((char *)IP_ADDRESS_SERVER,20,"127.0.0.1",20);
//#endif
	if(flash_multi_auto)
		SetTimer(2,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL Dowmloadfile::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if((pMsg->wParam==VK_RETURN)  || (pMsg->wParam == VK_ESCAPE))
			return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}




void Dowmloadfile::Start_Download()
{
	KillProcessFromName(_T("ISP.exe"));
	SetTimer(1,200,NULL);
	wait_download_and_isp_finished = false;
	Add_log_count = 1;
	replace_count = 1;
	m_download_info.ResetContent();
	download_step = SEND_DOWNLOAD_COMMAND;
	int resualt=TCP_File_Socket.Create(0,SOCK_STREAM);//SOCK_STREAM
	TCP_File_Socket.SetParentWindow(downloadfile_hwnd);


	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");

	int is_local_temco_net = false;
	is_local_temco_net  = GetPrivateProfileInt(_T("Setting"),_T("LocalTemcoNet"),0,temp_db_ini_folder);
	if(is_local_temco_net == false)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("LocalTemcoNet"),_T("0"),temp_db_ini_folder);
		WritePrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_db_ini_folder);
	}
	//TCP_File_Socket.Connect(_T("114.93.6.170"),31234);

	//hostent* host = gethostbyname("14i5f38013.iok.la");

	if(is_local_temco_net == false)
	{
		hostent* host = gethostbyname("newfirmware.com");
		if(host == NULL)
		{
			MessageBox(_T("Can't access Firmware server.  \r\nPlease check your internet connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_CLOSE,NULL,NULL);
			return ;
		}
		char* pszIP  = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr)); 
		memcpy_s((char *)IP_ADDRESS_SERVER,20,pszIP,20);
	}
	else
	{
		CString temp_ip;
		GetPrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_ip.GetBuffer(MAX_PATH),MAX_PATH,temp_db_ini_folder);
		temp_ip.ReleaseBuffer();

		char temp_char_ip[40];
		memset(temp_char_ip,0,40);
		WideCharToMultiByte( CP_ACP, 0, temp_ip.GetBuffer(), -1, temp_char_ip, 255, NULL, NULL );
		memcpy_s((char *)IP_ADDRESS_SERVER,20,temp_char_ip,20);
	}

	//#ifdef _DEBUG
	//	memcpy_s((char *)IP_ADDRESS_SERVER,20,"127.0.0.1",20);
	//#endif



	CString Connect_ip;
	//Connect_ip.Format(_T("%d.%d.%d.%d"),IP_ADDRESS_SERVER[0],IP_ADDRESS_SERVER[1],IP_ADDRESS_SERVER[2],IP_ADDRESS_SERVER[3]);
	Connect_ip = IP_ADDRESS_SERVER;

	CString show_message;
	show_message.Format(_T("Wait connection to the IP "));
	show_message = show_message + Connect_ip;
	//m_download_info.AddString(complete_message);
	m_download_info.InsertString(m_download_info.GetCount(),show_message);
	m_download_info.SetTopIndex(m_download_info.GetCount()-1);



	TCP_File_Socket.Connect(Connect_ip,TEMCO_SERVER_PORT);

	const BOOL bReuseaddr=TRUE;
	const BOOL bDontLinger = FALSE; 
	TCP_File_Socket.SetSockOpt(SO_DONTLINGER,&bDontLinger,sizeof(BOOL),SOL_SOCKET);

	//sockClient.Create(m_my_port,SOCK_STREAM);
	TCP_File_Socket.SetSockOpt(SO_REUSEADDR,&bReuseaddr,sizeof(BOOL),SOL_SOCKET);
	TCP_File_Socket.GetLastError();
	TCP_File_Socket.AsyncSelect(FD_READ |FD_CONNECT |FD_CLOSE);

	((CComboBox *)GetDlgItem(IDC_COMBO_UPDATE_TYPE))->EnableWindow(false);
	//((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->SetAddress(IP_ADDRESS_SERVER);
	//((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(false);
}


void Dowmloadfile::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(wait_download_and_isp_finished)
	{
		MessageBox(_T("Can't exit, please wait!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		return;
	}
	download_thread_flag = false;
	Sleep(200);
	CDialogEx::OnClose();
}


void Dowmloadfile::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnCancel();
}



BOOL Dowmloadfile::IsNetDevice(int DevType)
{
	if (DevType == PM_LightingController 
		|| DevType == PM_NC
		|| DevType == PM_CO2_NET
		|| DevType == PM_MINIPANEL
		|| DevType == PM_CM5
		|| DevType == PM_T322AI
		|| DevType == PM_T38AI8AO6DO)
	{
		return TRUE;
	}

	return FALSE;
}

void Dowmloadfile::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int temp_count = 0;
	switch(nIDEvent)
	{
	case 1:
		{
			temp_count = GetPrivateProfileInt(_T("LogInfo"),_T("AddCount"),0,AutoFlashConfigPath);
			if(Add_log_count <= temp_count)
			{
				CString temp_string;
				CString section;
				section.Format(_T("AddText%d"),Add_log_count);
				GetPrivateProfileStringW(_T("LogInfo"),section,_T(""),temp_string.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
				temp_string.ReleaseBuffer();
				Add_log_count ++;
				if(!temp_string.IsEmpty())
				{
					m_download_info.InsertString(m_download_info.GetCount(),temp_string);
					m_download_info.SetTopIndex(m_download_info.GetCount()-1);
				}
			}

			int is_replace_refresh = GetPrivateProfileInt(_T("LogInfo"),_T("Replace_Refresh"),0,AutoFlashConfigPath);
			if(is_replace_refresh)
			{
				int temp_replace_count = GetPrivateProfileInt(_T("LogInfo"),_T("ReplaceCount"),0,AutoFlashConfigPath);	
				CString temp_section;
				temp_section.Format(_T("ReplaceText%d"),temp_replace_count);
				CString replace_string;
				GetPrivateProfileStringW(_T("LogInfo"),temp_section,_T(""),replace_string.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
				replace_string.ReleaseBuffer();
				int nLineCount = m_download_info.GetCount();
				m_download_info.DeleteString(nLineCount - 1);

				m_download_info.InsertString(m_download_info.GetCount(),replace_string);
				m_download_info.SetTopIndex(m_download_info.GetCount()-1);
				WritePrivateProfileStringW(_T("LogInfo"),_T("Replace_Refresh"),_T("0"),AutoFlashConfigPath);	

			}
		}
		break;
	case 2:
		{
			OnBnClickedButtonFileDownloadOnly();
			KillTimer(2);
		}
		break;
	default:
		{

		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void Dowmloadfile::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here


	CString temp_folder_path;
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();

	Folder_Path = ApplicationFolder + _T("\\Database\\Firmware");
	temp_folder_path = Folder_Path + _T("\\DefaultFile.txt");
	CFileFind cs_temp_find;
	if(!cs_temp_find.FindFile(temp_folder_path))
	{
		CStdioFile*		 m_plogFile;
		m_plogFile=new CStdioFile;
		if(m_plogFile->Open(temp_folder_path.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
		{
			Sleep(1);
		}
		m_plogFile->Close();
	}
	CString str = _T("/select, ") + temp_folder_path;     
	ShellExecute( NULL, _T("open"), _T("explorer.exe"), str, NULL, SW_SHOWNORMAL );  
}

void Dowmloadfile::OnBnClickedButtonStartDownload()
{
	// TODO: Add your control notification handler code here
	download_and_update = DOWNLOAD_AND_UPDATE;
	Start_Download();
}

void Dowmloadfile::OnBnClickedButtonFileDownloadOnly()
{
	// TODO: Add your control notification handler code here
	download_and_update = DOWNLOAD_ONLY;
	Start_Download();
}


void Dowmloadfile::OnBnClickedButtonUpdateT3000()
{
	// TODO: Add your control notification handler code here
	m_product_isp_auto_flash.product_class_id =  199;
	download_and_update = DOWNLOAD_ONLY;
	Start_Download();
	return;
}
