// Dowmloadfile.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Dowmloadfile.h"
#include "afxdialogex.h"
#include "globle_function.h"

// Dowmloadfile dialog
int total_file_length = 0;	
int product_id = 0;
char receive_buffer[4000];
unsigned short totalpackage = 0;
unsigned short current_package = 1;
char download_filename[20];
int download_step = SEND_DOWNLOAD_COMMAND;

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
END_MESSAGE_MAP()

HANDLE Downloadfile_Thread;
DWORD download_threadid;
// Dowmloadfile message handlers
LRESULT Dowmloadfile::DownloadFileMessage(WPARAM wParam,LPARAM lParam)
{
	int ncommand = (int)wParam;
	if(ncommand == 1)
	{
		unsigned char IPAddress[4];
		((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->GetAddress(IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
		CString retmessage;
		retmessage.Format(_T("Connect to IP Address %d.%d.%d.%d success!"),IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
		m_download_info.AddString(retmessage);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);

		Downloadfile_Thread = CreateThread(NULL,NULL,DownLoadFileProcess,this,NULL, &download_threadid);
	}
	else if(ncommand == 2)
	{
		unsigned char IPAddress[4];
		((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->GetAddress(IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
		CString retmessage;
		retmessage.Format(_T("Disconnected with IP Address %d.%d.%d.%d!"),IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
		m_download_info.AddString(retmessage);
		m_download_info.SetTopIndex(m_download_info.GetCount()-1);
	}
	else if(ncommand == 3)
	{
		GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(true);
	}
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

	char sendbuffer[1000];
	while(1)
	{
		
		switch(download_step)
		{
		case SEND_DOWNLOAD_COMMAND:
			{
				Download_Info temp_info;
				temp_info.HEAD_1 = 0x55;
				temp_info.HEAD_2 = 0xff;
				temp_info.length = sizeof(Download_Info) - 2;
				temp_info.commad = DOWNLOAD_FILE;
				temp_info.product_id = product_id;
				temp_info.get_newest = 1;
				temp_info.file_type = 2;
				temp_info.softversion = 0;
				temp_info.CRC = 0xff;
				memset(sendbuffer,0,1000);
				memcpy(sendbuffer,&temp_info,sizeof(Download_Info));
				sendbuffer[sizeof(Download_Info) - 1] = Add_CRC(sendbuffer,sizeof(Download_Info));
				mparent->TCP_File_Socket.Send(sendbuffer,sizeof(Download_Info),0);
				for (int i=0;i<300;i++)
				{
					if(download_step!= SEND_DOWNLOAD_COMMAND)
					{
						current_package = 1;
						break;
					}
					Sleep(1);
				}

				break;
			}
		case START_SEND_WANT_PACKAGE:
			{
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
				for (int i=0;i<300;i++)
				{
					if(download_step!= START_SEND_WANT_PACKAGE)
					{

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
				CString ApplicationFolder;
				CString Folder_Path;
				GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
				PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
				ApplicationFolder.ReleaseBuffer();
				Folder_Path = ApplicationFolder + _T("\\Database\\");

				CString file_name;
				MultiByteToWideChar( CP_ACP, 0, download_filename, (int)strlen((char *)download_filename)+1, file_name.GetBuffer(MAX_PATH), MAX_PATH );
				file_name.ReleaseBuffer();
				Folder_Path = Folder_Path + file_name; 

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

				break;
			}
			
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

	CString temp_id;CString temp_name;
	temp_id.Format(_T("%d"),product_id);
	temp_name = GetProductName(product_id);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_ID))->SetWindowTextW(temp_id);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_ID))->EnableWindow(false);
	//((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->SetAddress(114,93,6,170);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->SetAddress(127,0,0,1);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_NAME))->SetWindowTextW(temp_name);
	((CEdit *)GetDlgItem(IDC_EDIT_PRODUCT_NAME))->EnableWindow(false);
	int resualt=TCP_File_Socket.Create(0,SOCK_STREAM);//SOCK_STREAM
	HWND downloadfile_hwnd = this->m_hWnd;
	TCP_File_Socket.SetParentWindow(downloadfile_hwnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL Dowmloadfile::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


void Dowmloadfile::OnBnClickedButtonStartDownload()
{
	// TODO: Add your control notification handler code here
	
	//TCP_File_Socket.Connect(_T("114.93.6.170"),31234);
	TCP_File_Socket.Connect(_T("127.0.0.1"),31234);

	((CComboBox *)GetDlgItem(IDC_COMBO_UPDATE_TYPE))->EnableWindow(false);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_TEMCO_IP))->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_DOWNLOAD)->EnableWindow(false);
}
