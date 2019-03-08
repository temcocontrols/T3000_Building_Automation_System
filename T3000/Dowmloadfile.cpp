// Dowmloadfile.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Dowmloadfile.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "MainFrm.h"
#include "Class/md5.h"
#include "ULRBindCallback.h"
// Dowmloadfile dialog
#define DOWNLOAD_AND_UPDATE  1
#define DOWNLOAD_ONLY        2
int download_and_update = 0;
CString productfolder;
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
unsigned short download_fw_version;
int download_step = SEND_DOWNLOAD_COMMAND;
bool download_thread_flag = true;
int wait_download_and_isp_finished = 0 ;
char receive_md5[20];
HWND downloadfile_hwnd;
CString Folder_Path;
CString HEX_BIN_FILE_PATH;
extern char * receivefile_buffer;
CString new_firmware_ip;
int is_local_temco_net = false;
extern bool update_t3000_only ;
IMPLEMENT_DYNAMIC(Dowmloadfile, CDialogEx)

map <int,CString> product_folder_map;

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
    DDX_Control(pDX, IDC_STATIC_FTP_PERSENT, m_static_persent);
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



LRESULT Dowmloadfile::DownloadFileMessage(WPARAM wParam, LPARAM lParam)
{
    //static bool insert_a_new_line = false;
    int ncommand = (int)wParam;
    if (ncommand == DOWNLOAD_CONNECT_SUCCESS)
    {
        //unsigned char IPAddress[4];
        //((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->GetAddress(IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
        CString retmessage;
        retmessage.Format(_T("Connect to newfirmware.com at IP="));
        retmessage = retmessage + new_firmware_ip + _T(" Success");

        m_download_info.InsertString(m_download_info.GetCount(), retmessage);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        if (Downloadfile_Thread == NULL)
            Downloadfile_Thread = CreateThread(NULL, NULL, DownLoadFileProcess, this, NULL, &download_threadid);
        else
        {
            TerminateThread(Downloadfile_Thread, 0);
            Downloadfile_Thread = CreateThread(NULL, NULL, DownLoadFileProcess, this, NULL, &download_threadid);
        }

    }
    else if (ncommand == DOWNLOAD_DISCONNEC)
    {
        CString retmessage;
        retmessage.Format(_T("Disconnected with server!"));
        m_download_info.InsertString(m_download_info.GetCount(), retmessage);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
        GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
        TCP_File_Socket.Close();
    }
    else if (ncommand == DOWNLOAD_CONNECT_FAILED)
    {

        CString retmessage;
        retmessage.Format(_T("Connect to server failed!Please try again!"));
        m_download_info.InsertString(m_download_info.GetCount(), retmessage);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        TCP_File_Socket.Close();
        GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
        GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);

        if (MessageBox(_T("Do you want download firmware manually from website?"), _T("Download"), MB_YESNO) == IDYES)
        {
            CString excute_path;
            if (!productfolder.IsEmpty())
            {
                excute_path = _T("http://www.temcocontrols.com/ftp/firmware/") + productfolder;
                ShellExecute(NULL, L"open", excute_path, NULL, NULL, SW_SHOWNORMAL);
            }
            return 0;
        }

    }
    else if (ncommand == DOWNLOAD_CLOSE_SOCKET)
    {

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


        //CString mypath = exe_folder + _T("\\Database\\Firmware");
        //mypath = mypath + _T("\\") + download_file_name;
        CString mypath;
        mypath = download_file_name;
        WritePrivateProfileStringW(_T("Data"), _T("FirmwarePath"), mypath, AutoFlashConfigPath);


        temp_isp_info.Format(_T("Firmware has been stored locally to  "));
        temp_isp_info = temp_isp_info + mypath;
        m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        //if (m_download_product_type == 199) //如果下载的是T3000;
        //{
        //    //调用解压;
        //}
        if (download_and_update == DOWNLOAD_ONLY)
        {
            if (flash_multi_auto)
            {
                //保存下载的位置,产品号 等信息;

                for (int z = 0;z<download_info_type.size();z++)
                {
                    if (m_download_product_type == download_info_type.at(z).download_product_type)
                    {
                        WideCharToMultiByte(CP_ACP, 0, mypath.GetBuffer(), -1, download_info_type.at(z).firmware_file_path, 255, NULL, NULL);
                        break;
                    }
                }
                Sleep(1000);
                PostMessage(WM_CLOSE, NULL, NULL);

            }
            GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
            GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
            return 0;
        }
        KillProcessFromName(_T("ISP.exe"));

        bool is_sub_device = false;
        CString temp_deal_ip = m_product_isp_auto_flash.BuildingInfo.strIp;
        if (!temp_deal_ip.IsEmpty())
        {
            CStringArray temparray;
            SplitCStringA(temparray, temp_deal_ip, _T("."));
            if (temparray.GetSize() == 4)
            {
                if (m_product_isp_auto_flash.note_parent_serial_number == 0)
                    is_sub_device = false;
                else
                    is_sub_device = true;
            }
        }


        //bool is_sub_device = (bool)IsNetDevice(m_product_isp_auto_flash.product_class_id);


        WritePrivateProfileStringW(_T("Data"), _T("Command"), _T("1"), AutoFlashConfigPath);
        if (m_product_isp_auto_flash.BuildingInfo.strIp.IsEmpty() ||
            (m_product_isp_auto_flash.baudrate == 19200) ||
            (m_product_isp_auto_flash.baudrate == 115200))//串口
        {
            CString temp_baudrate;
            temp_baudrate.Format(_T("%d"), m_product_isp_auto_flash.baudrate);
            WritePrivateProfileStringW(_T("Data"), _T("COM_OR_NET"), _T("COM"), AutoFlashConfigPath);
            CString cs_comport;
            cs_comport.Format(_T("COM%d"), m_product_isp_auto_flash.ncomport);
            WritePrivateProfileStringW(_T("Data"), _T("COMPORT"), cs_comport, AutoFlashConfigPath);
            //WritePrivateProfileStringW(_T("Data"),_T("Baudrate"),_T("19200"),AutoFlashConfigPath);
            WritePrivateProfileStringW(_T("Data"), _T("Baudrate"), temp_baudrate, AutoFlashConfigPath);

            CString nflash_id;
            nflash_id.Format(_T("%d"), m_product_isp_auto_flash.product_id);
            WritePrivateProfileStringW(_T("Data"), _T("ID"), nflash_id, AutoFlashConfigPath);

            temp_isp_info.Format(_T("Communications port : "));
            temp_isp_info = temp_isp_info + cs_comport;
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
            temp_isp_info.Format(_T("ISP baudrate : %d"), m_product_isp_auto_flash.baudrate);
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
            temp_isp_info.Format(_T("Device ID :"));
            temp_isp_info = temp_isp_info + nflash_id;
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
        }
        else
        {
            WritePrivateProfileStringW(_T("Data"), _T("COM_OR_NET"), _T("NET"), AutoFlashConfigPath);
            WritePrivateProfileStringW(_T("Data"), _T("IPAddress"), m_product_isp_auto_flash.BuildingInfo.strIp, AutoFlashConfigPath);
            if ((m_product_isp_auto_flash.ncomport == 0) || (m_product_isp_auto_flash.ncomport == 47808))
            {
                m_product_isp_auto_flash.ncomport = 10000;
            }

            if (is_sub_device)
            {
                temp_isp_info.Format(_T("Contacting Device at IP address %s , port %d ,and subnet ID %d"),
                    m_product_isp_auto_flash.BuildingInfo.strIp, m_product_isp_auto_flash.ncomport, m_product_isp_auto_flash.product_id);

                WritePrivateProfileStringW(_T("Data"), _T("Subnote"), _T("1"), AutoFlashConfigPath);
                CString nsub_id;
                nsub_id.Format(_T("%d"), m_product_isp_auto_flash.product_id);
                WritePrivateProfileStringW(_T("Data"), _T("SubID"), nsub_id, AutoFlashConfigPath);
            }
            else
            {
                temp_isp_info.Format(_T("Contacting Device at IP address %s , port %d "),
                    m_product_isp_auto_flash.BuildingInfo.strIp, m_product_isp_auto_flash.ncomport);
                WritePrivateProfileStringW(_T("Data"), _T("Subnote"), _T("0"), AutoFlashConfigPath);
            }
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);

            CString n_tcpport;
            n_tcpport.Format(_T("%d"), m_product_isp_auto_flash.ncomport);
            WritePrivateProfileStringW(_T("Data"), _T("IPPort"), n_tcpport, AutoFlashConfigPath);
#if 0
            temp_isp_info.Format(_T("Communications port : network"));
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
            temp_isp_info.Format(_T("IP Address : "));
            temp_isp_info = temp_isp_info + m_product_isp_auto_flash.BuildingInfo.strIp;
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);


            CString n_tcpport;
            n_tcpport.Format(_T("%d"), m_product_isp_auto_flash.ncomport);

            temp_isp_info.Format(_T("Port : "));
            temp_isp_info = temp_isp_info + n_tcpport;
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);

            WritePrivateProfileStringW(_T("Data"), _T("IPPort"), n_tcpport, AutoFlashConfigPath);
            if (is_sub_device)
            {
                WritePrivateProfileStringW(_T("Data"), _T("Subnote"), _T("1"), AutoFlashConfigPath);
                CString nsub_id;
                nsub_id.Format(_T("%d"), m_product_isp_auto_flash.product_id);
                WritePrivateProfileStringW(_T("Data"), _T("SubID"), nsub_id, AutoFlashConfigPath);

                temp_isp_info.Format(_T("Device is subnote."));
                m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
            }
            else
            {
                WritePrivateProfileStringW(_T("Data"), _T("Subnote"), _T("0"), AutoFlashConfigPath);
            }
#endif
            temp_isp_info.Format(_T("Setting device to firmware update mode..."));
            m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
        }

        HANDLE Call_ISP_Application = NULL;
        Call_ISP_Application = CreateThread(NULL, NULL, isp_thread, this, NULL, NULL);


    }
    else if (ncommand == DOWNLOAD_PERSENT)
    {

        int npersent = (int)lParam;
        CString persent_message;
        persent_message.Format(_T("Downloading now:  %d%%"), npersent);
        //bool is_the_first = true;
        //if(!is_the_first)
        m_download_info.DeleteString(m_download_info.GetCount() - 1);

        //m_download_info.AddString(persent_message);
        m_download_info.InsertString(m_download_info.GetCount(), persent_message);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
    }
    else if (ncommand == DOWNLOAD_FINISHED)
    {
        CString complete_message;
        complete_message.Format(_T("File download complete. "));
        //m_download_info.AddString(complete_message);
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);

        complete_message.Format(_T("Local file path : "));
        complete_message = complete_message + Folder_Path;
        //m_download_info.InsertString(m_download_info.GetCount(),complete_message);

        //m_download_info.SetTopIndex(m_download_info.GetCount()-1);
    }
    else if (ncommand == DOWNLOAD_FILE_INFO)
    {
        CString nfile_name;
        CString nfile_size;
        MultiByteToWideChar(CP_ACP, 0, download_filename, (int)strlen((char *)download_filename) + 1, nfile_name.GetBuffer(MAX_PATH), MAX_PATH);
        nfile_name.ReleaseBuffer();
        nfile_size.Format(_T("File size about %d KB "), malloc_download_memory_size / 1024);
        CString show_message;
        show_message = _T("File name  ") + nfile_name + _T("   .") + nfile_size;
        m_download_info.InsertString(m_download_info.GetCount(), show_message);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
    }
    else if (ncommand == DOWNLOAD_T3000_NO_UPDATE)
    {
        CString show_message;
        show_message = _T("Your T3000 is up-to-date");
        m_download_info.InsertString(m_download_info.GetCount(), show_message);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
    }
    else if (ncommand == DOWNLOAD_NOT_FIND_LOCAL)
    {
        CString complete_message;
        complete_message.Format(_T("Local file doesn't exist, downloading from server now."));
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);
        //complete_message.Format(_T("Check MD5 value over!"));
        //m_download_info.InsertString(m_download_info.GetCount(),complete_message);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);



        complete_message.Format(_T("Version Details."));
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);
        complete_message.Format(_T("Device Firmware: %.1f"), m_product_isp_auto_flash.software_version);
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);
        complete_message.Format(_T("Newfirmware.com: %.1f"), ((float)download_fw_version) / 10);
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);
        complete_message.Format(_T("               "));
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);
    }
    else if (ncommand == DOWNLOAD_LOCAL_EXSIT)
    {
        CString complete_message;
        complete_message.Format(_T("Local Firmware  already exist in the Firmware folder.MD5 checksum value matches.  "));
        m_download_info.InsertString(m_download_info.GetCount(), complete_message);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
    }
    else if (ncommand == DOWNLOAD_RESULTS)
    {
        GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
        GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(true);
        KillTimer(1);
        int ret_results = GetPrivateProfileInt(_T("Data"), _T("Command"), 0, AutoFlashConfigPath);
        CString ret_message;
        switch (ret_results)
        {
        case NO_COMMAND:
        case START_AUTO_FLASH_COMMAND:
        case FAILED_NORESPONSED:
            ret_message.Format(_T("No response from device,please try again!"));
            m_download_info.InsertString(m_download_info.GetCount(), ret_message);
            m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
            break;
        case FLASH_SUCCESS:
        {
            ret_message.Format(_T("Sending firmware to device succeed!"));
            m_download_info.InsertString(m_download_info.GetCount(), ret_message);
            m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        }
        break;
        case FAILED_UNKNOW_ERROR:
        {
            ret_message.Format(_T("Sending firmware to device failed! "));
            m_download_info.InsertString(m_download_info.GetCount(), ret_message);
            m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        }
        break;
        }

        CFileFind configfind;
        if (configfind.FindFile(AutoFlashConfigPath))
        {
            DeleteFile(AutoFlashConfigPath);
        }
    }
    else if (ncommand == RETURN_ERROR)
    {
        CString show_error_message;
        int file_error_type = (int)lParam;
        bool download_failed = false;
        switch (file_error_type)
        {
        case NO_MD5_FILE_EXSIT:
        {
            show_error_message.Format(_T("Sorry, the firmware update service is temporarily offline or your file cannot be found."));

            m_download_info.InsertString(m_download_info.GetCount(), show_error_message);
            m_download_info.SetTopIndex(m_download_info.GetCount() - 1);

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
            string temp_md5 = MD5(ifstream(HEX_BIN_FILE_PATH)).toString();
            CString MD5_value;
            MD5_value = temp_md5.c_str();
            CString temp_show;
            temp_show.Format(_T("The File MD5 is :"));
            temp_show = temp_show + MD5_value;
            m_download_info.InsertString(m_download_info.GetCount(), temp_show);
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

        if (download_failed)
        {
            if (flash_multi_auto)
            {
                PostMessage(WM_CLOSE, NULL, NULL);
            }
            else
            {


                if (MessageBox(_T("Do you want download firmware manually from website?"), _T("Download"), MB_YESNO) == IDYES)
                {
                    CString excute_path;
                    if (!productfolder.IsEmpty())
                    {
                        excute_path = _T("http://www.temcocontrols.com/ftp/firmware/") + productfolder;
                        ShellExecute(NULL, L"open", excute_path, NULL, NULL, SW_SHOWNORMAL);
                    }
                    //ShellExecute(NULL, L"open", L"http://www.temcocontrols.com/ftp/firmware/", NULL, NULL, SW_SHOWNORMAL);
                }

            }
        }
        m_download_info.InsertString(m_download_info.GetCount(), show_error_message);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
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
				temp_info.commad =  DOWNLOAD_FILE;
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
		case SHOW_FTP_PATH:
			{
				ShellExecute(NULL, L"open", L"http://www.temcocontrols.com/ftp/software/9TstatSoftware.zip", NULL, NULL, SW_SHOWNORMAL);

				return 0;

			}
			break;
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


					char locoal_char_md5[20];
					memcpy_s(locoal_char_md5,20, MD5(ifstream( temp_file_path )).digest(),20);
					int ret_value = memcmp(locoal_char_md5,receive_md5,16);
					if(ret_value == 0)	//MD5值一样就不用下载 直接结束;
					{
						::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_LOCAL_EXSIT,NULL);
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

void Dowmloadfile::CreateProductFolderMap()
{
	//product_folder_map
	
	product_folder_map.insert(map <int,CString>::value_type(PM_TSTAT5B,_T("Tstat5/Tstat5LED")));
	product_folder_map.insert(map <int,CString>::value_type(PM_MINIPANEL,_T("T3_BB_LB_TB")));
	product_folder_map.insert(map <int, CString>::value_type(PM_MINIPANEL_ARM, _T("T3_BB_LB_TB")));
	product_folder_map.insert(map <int,CString>::value_type(PM_TSTAT6,_T("Tstat6/128kChip")));
	Sleep(1);
}

CString Dowmloadfile::GetProdcutFtpPath(int nproductid)
{
	map <int,CString>::iterator itter;
	itter = product_folder_map.find(nproductid);
	if(itter != product_folder_map.end())
	{
		return itter->second;
	}
	else
		return _T("");
	
}

BOOL Dowmloadfile::OnInitDialog()
{
	CDialogEx::OnInitDialog();


    m_static_persent.SetWindowTextW(_T(""));
    m_static_persent.textColor(RGB(255, 0, 0));
    //m_static_persent.bkColor(RGB(0,255,255));
    m_static_persent.setFont(18, 10, NULL, _T("Arial"));
    m_static_persent.ShowWindow(SW_HIDE);

    GetDlgItem(IDC_PROGRESS_FTP_DOWNLOAD)->ShowWindow(SW_HIDE);
	
	CreateProductFolderMap();

	productfolder = GetProdcutFtpPath(m_product_isp_auto_flash.product_class_id);



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
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_NAME))->SetWindowTextW(temp_name);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_NAME))->EnableWindow(false);
	downloadfile_hwnd = this->m_hWnd;
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
	if(m_download_product_type == 0)
	{
		PostMessage(WM_CLOSE,NULL,NULL);
	}
	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
	
	is_local_temco_net  = GetPrivateProfileInt(_T("Setting"),_T("LocalTemcoNet"),0,temp_db_ini_folder);
	if(is_local_temco_net == false)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("LocalTemcoNet"),_T("0"),temp_db_ini_folder);
		WritePrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_db_ini_folder);
	}


	if((isp_mode_firmware_auto == false) && (isp_mode_is_cancel == false))
	{
		GetDlgItem(IDC_BUTTON_UPDATE_T3000)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(FALSE);
		SetTimer(5,2000,NULL);
		return TRUE;
	}
	else if((isp_mode_firmware_auto == true) && (isp_mode_is_cancel == false))
	{
		SetTimer(6,2000,NULL);
	}
	else if(update_t3000_only)
	{
		SetTimer(7,1000,NULL);
	}

	if(is_local_temco_net == false)
	{
#if 0
		hostent* host = gethostbyname("newfirmware.com");
		if(host == NULL)
		{
			if(!flash_multi_auto)
			{
				MessageBox(_T("Connect firmware server failed.  \r\nPlease check your internet connection!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				if(MessageBox(_T("Do you want download firmware manually from website?"),_T("Download"),MB_YESNO) == IDYES)
				{
					CString excute_path;
					if(!productfolder.IsEmpty())
					{ 
						excute_path = _T("http://www.temcocontrols.com/ftp/firmware/") + productfolder;
						ShellExecute(NULL, L"open", excute_path, NULL, NULL, SW_SHOWNORMAL);
					}

				}
			}

			PostMessage(WM_CLOSE,NULL,NULL);
			return TRUE;
		}
		char* pszIP  = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr)); 
		memcpy_s((char *)IP_ADDRESS_SERVER,20,pszIP,20);

		CString temp_message;
		temp_message.Format(_T("Internet access!"));
		m_download_info.InsertString(m_download_info.GetCount(),temp_message);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
#endif
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

    Update_File();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL Dowmloadfile::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if((pMsg->wParam==VK_RETURN)  || (pMsg->wParam == VK_ESCAPE))
			return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//更新包打补丁的时候，在zip文件中放置 UpdateEng.exe 
//在打开T3000的时候，更新时，替换 原有的Update.exe 达到自动更新引擎的目的
//T3000 更新  Update.exe 更新 自己 .
HANDLE h_updatefile = NULL;
void Dowmloadfile::Update_File()
{
    CString temp_folder_path;
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();

    CString update_exe_file_path = ApplicationFolder + _T("\\UpdateEng.exe");
    CFileFind cs_temp_find;
    if (cs_temp_find.FindFile(update_exe_file_path))
    {
        CString NewUpdateFilePath = ApplicationFolder + _T("\\Update.exe");
        CopyFile(update_exe_file_path, NewUpdateFilePath, false);
        DeleteFile(update_exe_file_path); //在更新完后删除Eng文件;
    }
}

HANDLE hDownloadFtpThread = NULL;
void Dowmloadfile::Start_Download_Ftp()
{
    
    SetTimer(1, 200, NULL);
    wait_download_and_isp_finished = false;
    Add_log_count = 1;
    replace_count = 1;
    m_download_info.ResetContent();

    m_static_persent.ShowWindow(SW_SHOW);
    GetDlgItem(IDC_PROGRESS_FTP_DOWNLOAD)->ShowWindow(SW_SHOW);
    if(hDownloadFtpThread == NULL)
        hDownloadFtpThread = CreateThread(NULL, NULL, FtpDownloadThread, this, NULL, NULL);
}


DWORD WINAPI  Dowmloadfile::FtpDownloadThread(LPVOID lpVoid)
{
    //Write_Config_Info
    Dowmloadfile *pParent = (Dowmloadfile *)lpVoid;
    CString CS_Info;
    CBindCallback cbc;
    cbc.m_pdlg = pParent;
    unsigned int ftp_version_date = 0;
    unsigned int local_version_date = 0;
    CString DesDownloadFilePath;
    CString DownloadIniFilePath;
    CString CheckVersionIniFilePath;
    CString T3000FtpPath;//测试用;
    CString temp_download_path;

    CString revisionFtpPath;//保存revision 的本地路径
    CString temp_revision_path;
    CString DesDownloadRevisionPath;
    CFileFind tempfind;
    CString strFileName;
    CString str_product_section;

    DeleteUrlCacheEntry(_T("https://temcocontrols.com/ftp/firmware/ProductPath.ini")); // 清理缓存
    Sleep(2000);

    HRESULT download_ret = NULL;
    DownloadIniFilePath = Folder_Path + _T("//ProductPath.ini");
    CheckVersionIniFilePath = Folder_Path + _T("//CheckVersionPath.ini");
    download_ret = URLDownloadToFile(NULL, _T("https://temcocontrols.com/ftp/firmware/ProductPath.ini"), DownloadIniFilePath, 0, NULL);
    //T3000_FTP_Version = GetPrivateProfileIntW(_T("Version"), _T("T3000Version"), 0, DownloadIniFilePath);
    if (download_ret != S_OK)
    {
        CS_Info.Format(_T("The network connection is not available,please check the network connection"));
        pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
        pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);

        if (INET_E_DOWNLOAD_FAILURE == download_ret)
        {
            CS_Info.Format(_T("This is an HTTPS(secure) address, click Tools, click Internet Options, click Advanced, and check to be sure the SSL and TLS protocols are enabled under the security section."));
            pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
            pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);
        }

        goto ftp_download_end;
    }
    


    str_product_section.Format(_T("%d"), pParent->m_download_product_type);
    GetPrivateProfileString(_T("ProductPath"), str_product_section, _T(""), temp_download_path.GetBuffer(MAX_PATH), MAX_PATH, DownloadIniFilePath);
    temp_download_path.ReleaseBuffer();

    GetPrivateProfileString(_T("RevisionFilePath"), str_product_section, _T(""), temp_revision_path.GetBuffer(MAX_PATH), MAX_PATH, DownloadIniFilePath);
    temp_revision_path.ReleaseBuffer();

    ftp_version_date = GetPrivateProfileInt(_T("LastUpdateTime"), str_product_section, 0, DownloadIniFilePath);
    local_version_date = GetPrivateProfileInt(_T("LastUpdateTime"), str_product_section, 0, CheckVersionIniFilePath);



    if (temp_download_path.IsEmpty())  //此产品没有找到对应的下载目录;
    {
        CS_Info.Format(_T("Can't find the firmware file on the webside."));
        pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
        pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);
        goto ftp_download_end;
    }

    T3000FtpPath = _T("https://www.temcocontrols.com/ftp/firmware/") + temp_download_path;


    strFileName  = PathFindFileName(T3000FtpPath);  //根据组合的下载路径，获取最后得文件名


    DesDownloadFilePath = Folder_Path + _T("\\") + strFileName;     

    //若存在 版本信息文件路径 就去下载并显示出来;
    if (temp_revision_path.IsEmpty() == false)
    {
        revisionFtpPath = _T("https://www.temcocontrols.com/ftp/firmware/") + temp_revision_path;
        strFileName = PathFindFileName(revisionFtpPath);  //根据组合的下载路径，获取最后得文件名
        DesDownloadRevisionPath = Folder_Path + _T("\\") + strFileName;

        DeleteUrlCacheEntry(revisionFtpPath); // 清理缓存
        Sleep(2000);

        download_ret = URLDownloadToFile(NULL, revisionFtpPath, DesDownloadRevisionPath, 0, &cbc);
        if (download_ret == S_OK)
        {
            CFile myfile(DesDownloadRevisionPath, CFile::modeRead);
            char *pBuf;
            DWORD dwFileLen;
            dwFileLen = myfile.GetLength();
            pBuf = new char[dwFileLen + 1];
            pBuf[dwFileLen] = 0;
            myfile.Read(pBuf, dwFileLen);     //MFC   CFile 类 很方便
            myfile.Close();

            CString temp_revision_notes;
            MultiByteToWideChar(CP_ACP, 0, pBuf, dwFileLen + 1, temp_revision_notes.GetBuffer(dwFileLen + 1), dwFileLen + 1);
            temp_revision_notes.ReleaseBuffer();
            CStringArray temparray;
            SplitCStringA(temparray, temp_revision_notes, _T("\r\n"));

            for (int j = 0; j < temparray.GetSize(); j++)
            {
                if (j > 20) //只展示前20行.
                    break;
                CS_Info.Format(_T("%s"), temparray.GetAt(j));
                pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
                pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);
                Sleep(10);
            }

            delete pBuf;
        }
    }

    if ((local_version_date == ftp_version_date) && (ftp_version_date != 0) && tempfind.FindFile(DesDownloadFilePath))  //与FTP上相等 并且存在;
    {
        pParent->download_file_name = DesDownloadFilePath;
        CS_Info.Format(_T("The file has been downloaded."));
        pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
        pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);
    }
    else
    {
        DeleteUrlCacheEntry(T3000FtpPath); // 清理缓存
        Sleep(2000);

        download_ret = URLDownloadToFile(NULL, T3000FtpPath, DesDownloadFilePath, 0, &cbc); // 根据配置文档配置好的路径去下载.下载到指定目录，并记录目录位置;
        if (download_ret == S_FALSE)
        {
            CS_Info.Format(_T("Download failded!"));
            pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
            pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);
            goto ftp_download_end;
        }
        else
        {
            //GetPrivateProfileInt(_T("LastUpdateTime"), str_product_section, 0, CheckVersionIniFilePath);
            CString temp_version;
            temp_version.Format(_T("%u"), ftp_version_date);
            WritePrivateProfileStringW(_T("LastUpdateTime"), str_product_section, temp_version, CheckVersionIniFilePath);
            pParent->download_file_name = DesDownloadFilePath;
            CS_Info.Format(_T("Download finished."));
            pParent->m_download_info.InsertString(pParent->m_download_info.GetCount(), CS_Info);
            pParent->m_download_info.SetTopIndex(pParent->m_download_info.GetCount() - 1);
        }
    }


    ::PostMessage(downloadfile_hwnd, WM_DOWNLOADFILE_MESSAGE, DOWNLOAD_CLOSE_SOCKET, NULL);

ftp_download_end:

    hDownloadFtpThread = NULL;
    pParent->m_static_persent.ShowWindow(SW_HIDE);
    pParent->GetDlgItem(IDC_PROGRESS_FTP_DOWNLOAD)->ShowWindow(SW_HIDE);

    return true;
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

	//int nRecvBufLen = 32 * 1024; //设置为32K
	//TCP_File_Socket.SetSockOpt(  SO_RCVBUF, ( const char* )&nRecvBufLen, sizeof( int ) ,SOL_SOCKET);

	////发送缓冲区
	//int nSendBufLen = 32*1024; //设置为32K
	//setsockopt( s, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBufLen, sizeof( int ) );



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
	new_firmware_ip = IP_ADDRESS_SERVER;
	CString show_message;
	show_message.Format(_T("Wait connection to newfirmware.com , IP : "));
	show_message = show_message + Connect_ip;
	//m_download_info.AddString(complete_message);
	m_download_info.InsertString(m_download_info.GetCount(),show_message);
	m_download_info.SetTopIndex(m_download_info.GetCount()-1);



	TCP_File_Socket.Connect(Connect_ip,TEMCO_SERVER_PORT);

	const BOOL bReuseaddr=TRUE;
	const BOOL bDontLinger = FALSE; 
	TCP_File_Socket.SetSockOpt(SO_DONTLINGER,&bDontLinger,sizeof(BOOL),SOL_SOCKET);
	TCP_File_Socket.SetSockOpt(SO_REUSEADDR,&bReuseaddr,sizeof(BOOL),SOL_SOCKET);
	TCP_File_Socket.GetLastError();
	TCP_File_Socket.AsyncSelect(FD_READ |FD_CONNECT |FD_CLOSE);

	((CComboBox *)GetDlgItem(IDC_COMBO_UPDATE_TYPE))->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FILE_DOWNLOAD_ONLY)->EnableWindow(false);
}


void Dowmloadfile::OnClose()
{
	 
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
	

	CDialogEx::OnCancel();
}



BOOL Dowmloadfile::IsNetDevice(int DevType)
{
	if (DevType == PM_LightingController 
		|| DevType == PM_NC
		|| DevType == PM_CO2_NET
		|| DevType == PM_MINIPANEL
		|| DevType == PM_MINIPANEL_ARM
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
			KillTimer(2);
			OnBnClickedButtonFileDownloadOnly();
		}
		break;
	case 5:
		{
			KillTimer(5);
			AutoFlashFirmware();
		}
		break;
	case 6:
		{
			KillTimer(6);
			OnBnClickedButtonStartDownload();
		}
		break;
	case 7:
		{
			KillTimer(7);
			OnBnClickedButtonUpdateT3000();
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
	
	download_and_update = DOWNLOAD_AND_UPDATE;
	//Start_Download();
    Start_Download_Ftp();
}

void Dowmloadfile::OnBnClickedButtonFileDownloadOnly()
{
	
	download_and_update = DOWNLOAD_ONLY;
	//Start_Download();
    Start_Download_Ftp();
}

void Dowmloadfile::AutoFlashFirmware()
{
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


	WritePrivateProfileStringW(_T("Data"), _T("FirmwarePath"), isp_mode_detect_firmware_path, AutoFlashConfigPath);


	temp_isp_info.Format(_T("FirmwarePath = "));
	temp_isp_info = temp_isp_info + isp_mode_detect_firmware_path;
	m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
	m_download_info.SetTopIndex(m_download_info.GetCount() - 1);




	bool is_sub_device = false;

	WritePrivateProfileStringW(_T("Data"), _T("Command"), _T("1"), AutoFlashConfigPath);

	WritePrivateProfileStringW(_T("Data"), _T("COM_OR_NET"), _T("NET"), AutoFlashConfigPath);
	WritePrivateProfileStringW(_T("Data"), _T("IPAddress"), m_product_isp_auto_flash.BuildingInfo.strIp, AutoFlashConfigPath);
	if ((m_product_isp_auto_flash.ncomport == 0) || (m_product_isp_auto_flash.ncomport == 47808))
	{
		m_product_isp_auto_flash.ncomport = 502;
	}
	temp_isp_info.Format(_T("Communications port : network"));
	m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
	temp_isp_info.Format(_T("IP Address : "));
	temp_isp_info = temp_isp_info + m_product_isp_auto_flash.BuildingInfo.strIp;
	m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);


	CString n_tcpport;
	n_tcpport.Format(_T("%d"), m_product_isp_auto_flash.ncomport);

	temp_isp_info.Format(_T("Port : "));
	temp_isp_info = temp_isp_info + n_tcpport;
	m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);

	WritePrivateProfileStringW(_T("Data"), _T("IPPort"), n_tcpport, AutoFlashConfigPath);
	if (is_sub_device)
	{
		WritePrivateProfileStringW(_T("Data"), _T("Subnote"), _T("1"), AutoFlashConfigPath);
		CString nsub_id;
		nsub_id.Format(_T("%d"), m_product_isp_auto_flash.product_id);
		WritePrivateProfileStringW(_T("Data"), _T("SubID"), nsub_id, AutoFlashConfigPath);

		temp_isp_info.Format(_T("Device is subnote."));
		m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);
	}
	else
	{
		WritePrivateProfileStringW(_T("Data"), _T("Subnote"), _T("0"), AutoFlashConfigPath);
	}

	temp_isp_info.Format(_T("Please wait!ISP is running!"));
	m_download_info.InsertString(m_download_info.GetCount(), temp_isp_info);

	SetTimer(1, 200, NULL);
	HANDLE Call_ISP_Application = NULL;
	Call_ISP_Application = CreateThread(NULL, NULL, isp_thread, this, NULL, NULL);
}

void Dowmloadfile::OnBnClickedButtonUpdateT3000()
{
    CString DownloadIniFilePath;
    CString CheckVersionIniFilePath;
    bool download_ret = false;
    DownloadIniFilePath = Folder_Path + _T("//ProductPath.ini");
    CheckVersionIniFilePath = Folder_Path + _T("//CheckVersionPath.ini");
    DeleteUrlCacheEntry(_T("https://temcocontrols.com/ftp/firmware/ProductPath.ini")); // 清理缓存
    Sleep(800);

    download_ret = URLDownloadToFile(NULL, _T("https://temcocontrols.com/ftp/firmware/ProductPath.ini"), DownloadIniFilePath, 0, NULL);
    //T3000_FTP_Version = GetPrivateProfileIntW(_T("Version"), _T("T3000Version"), 0, DownloadIniFilePath);
    if ((download_ret != S_OK) && is_local_temco_net == false)  //如果不是本地的temco
    {
        CString CS_Info;
        CS_Info.Format(_T("The network connection is not available,please check the network connection"));
        m_download_info.InsertString(m_download_info.GetCount(), CS_Info);
        m_download_info.SetTopIndex(m_download_info.GetCount() - 1);
        return;
    }


	CString tempApplicationFolder;
	GetModuleFileName(NULL, tempApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(tempApplicationFolder.GetBuffer(MAX_PATH));
	tempApplicationFolder.ReleaseBuffer();


	ShellExecute(NULL, _T("open"), _T("Update.exe"), NULL, tempApplicationFolder, SW_SHOWNORMAL);

	return;
}
