// TCP_Server.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TCP_Server.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "bip.h"
#include "datalink.h"
#include "../SQLiteDriver/CppSQLite3.h"
void CALLBACK Listen(SOCKET s, int ServerPort, const char* ClientIP);
extern void init_info_table(void);
extern void Init_table_bank();
// CTCP_Server dialog
#define  WM_REFRESH_LIST  WM_USER + 7428
IMPLEMENT_DYNAMIC(CTCP_Server, CDialogEx)

CTCP_Server::CTCP_Server(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCP_Server::IDD, pParent)
{
}

CTCP_Server::~CTCP_Server()
{
}

void CTCP_Server::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GSM_CONNECTION, m_tcp_server_list);
}


BEGIN_MESSAGE_MAP(CTCP_Server, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START_SERVER, &CTCP_Server::OnBnClickedButtonStartServer)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SERVER, &CTCP_Server::OnBnClickedButtonStopServer)
	ON_MESSAGE(WM_REFRESH_LIST,&CTCP_Server::RefreshList)
END_MESSAGE_MAP()


// CTCP_Server message handlers


void CTCP_Server::OnBnClickedButtonStartServer()
{
	

	CString temp_cs;
	USHORT nport;
	GetDlgItemTextW(IDC_EDIT_SERVER_PORT, temp_cs.GetBuffer(MAX_PATH),MAX_PATH);
	temp_cs.ReleaseBuffer();
	nport = _wtoi(temp_cs);
	if ((nport == 0) || (nport > 60000))
	{
		MessageBox(_T("Please input an valid port number."));
		SetDlgItemTextW(IDC_EDIT_SERVER_PORT,_T("30000"));
		return;
	}

	if (m_wskServer.CreateServer(nport, SOMAXCONN))
	{
		m_bServer = m_wskServer.StartServer(Listen);
		TCP_Server_Running = true;
	}

	if (!m_bServer)
	{
		CString info;
		info.Format(_T("Start Server Failed：%d"), m_wskServer.err);
		TCP_Server_Running = false;
		AfxMessageBox(info);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SERVER_PORT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_START_SERVER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP_SERVER)->EnableWindow(true);

		BACNET_ADDRESS src = {
			0
		}; /* address where message came from */
		uint16_t pdu_len = 0;
		unsigned timeout = 100; /* milliseconds */
		BACNET_ADDRESS my_address, broadcast_address;
		char my_port[50];

		srand((unsigned)time(NULL));
		unsigned int temp_value;
		temp_value = rand() % (0x3FFFFF);
		g_Print.Format(_T("The initial T3000 Object Instance value is %d"), temp_value);
		DFTrace(g_Print);
		Device_Set_Object_Instance_Number(temp_value);
		address_init();
		Init_Service_Handlers();

		//bip_set_socket(s);
		bip_setgsm(true);
		set_datalink_protocol(PROTOCOL_BACNET_IP);

		if (CM5_hThread != NULL)
		{
            system_connect_info.mstp_status = 0;
			TerminateThread(CM5_hThread, 0);
			CM5_hThread = NULL;
		}
		CM5_hThread = CreateThread(NULL,NULL, MSTP_Receive,NULL,NULL, &nThreadID_x);
        CloseHandle(CM5_hThread);
		if (!bac_net_initial_once)
		{
			bac_net_initial_once = true;
			timesec1970 = (unsigned long)time(NULL);
			timestart = 0;
			init_info_table();
			Init_table_bank();
		}

		//MessageBox(_T("Start server success."));
	}
}


void CTCP_Server::OnBnClickedButtonStopServer()
{
	
	if (m_bServer)
	{
		TCP_Server_Running = false;
		m_bServer = false;
		m_tcp_connect_info.clear();
		m_wskServer.CloseServer();
		GetDlgItem(IDC_EDIT_SERVER_PORT)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_START_SERVER)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_STOP_SERVER)->EnableWindow(false);
	}
}


BOOL CTCP_Server::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CTCP_Server::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	GetDlgItem(IDC_BUTTON_STOP_SERVER)->EnableWindow(false);
	Initial_List();
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTCP_Server::OnCancel()
{
	
	this->ShowWindow(SW_HIDE);
	return;
	CDialogEx::OnCancel();
}

extern SOCKET my_sokect;
//TCP Server 的 回调函数 ，当客户端连接上 之后就可以用这个socket去发送数据了;//This function add by Fance.
void CALLBACK Listen(SOCKET s, int ServerPort, const char* ClientIP)
{
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	int client_object_instance = 0;
	int nRet;
	char buf[1000];

	CMulitithreadSocket wsk;
	wsk = s;
	bool get_mutex_control = false;


	bip_set_socket(s);
    if (CM5_hThread == NULL)
    {
        system_connect_info.mstp_status = 0;
        CM5_hThread = CreateThread(NULL, NULL, MSTP_Receive, NULL, NULL, &nThreadID_x);
        CloseHandle(CM5_hThread);
    }
	char pSendBuf[1024] = {0x81, 0x0B, 0x00, 0x0C, 0x01, 0x20, 0xFF, 0xFF, 0x00, 0xFF, 0x10, 0x08};
	int nSendLen = 12;


	int time_out = 0;
	BOOL bTimeOut = FALSE;
	m_bac_handle_Iam_data.clear();


	while (!bTimeOut)//!pScanner->m_bNetScanFinish)  // 超时结束
	{
		time_out++;
		if (time_out > 5)
			bTimeOut = TRUE;

		//		FD_ZERO(&fdSocket);	
		//		FD_SET(h_temp_sokcet, &fdSocket);
		//my_sokect
		nRet = wsk.SendData(pSendBuf, nSendLen, 6000);
		if (nRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			//goto END_SCAN;
			return;
		}
		Sleep(2000);
		continue;//Test ///////////////////////////////////////////////////////////////////////////////////
	}//end of while

	if (m_bac_handle_Iam_data.size() == 0)
	{
		AfxMessageBox(_T("No device response!"));
		//if (CM5_hThread != NULL)
		//{
  //          system_connect_info.mstp_status = 0;
		//	TerminateThread(CM5_hThread, 0);
		//	CM5_hThread = NULL;
		//	Sleep(1000);
		//}
		wsk.Close();
		CM5_hThread = CreateThread(NULL,NULL, MSTP_Receive,NULL,NULL, &nThreadID_x);
        CloseHandle(CM5_hThread);
		return;
	}
	m_bac_scan_result_data.clear();
#pragma region getserialnumber
	for (int j = 0; j < 5; j++)
	{
		int ready_to_read_count = m_bac_handle_Iam_data.size();

		CString strInfo;
		strInfo.Format(_T("Scan  Bacnetip.Found %d BACNET device"), ready_to_read_count);
		//DFTrace(strInfo);
		//pScan->ShowBacnetComScanInfo(strInfo);

		if ((int)m_bac_scan_result_data.size() >= ready_to_read_count) //达到返回的个数后就break;
			break;
		TRACE(_T("gloab scan = %d\r\n"), ready_to_read_count);
		for (int i = 0; i < ready_to_read_count; i++)
		{
			int resend_count = 0;
			do
			{
				resend_count ++;
				if (resend_count > 50)
					break;
				g_invoke_id = GetPrivateData(
					m_bac_handle_Iam_data.at(i).device_id,
					GETSERIALNUMBERINFO,
					0,
					0,
					sizeof(Str_Serial_info));

				Sleep(3000);
			}
			while (g_invoke_id < 0);
		}
	}
	Sleep(1);
	if (m_bac_scan_result_data.size() == 0)
	{
		AfxMessageBox(_T("No device found in remote area!"));
		//if (CM5_hThread != NULL)
		//{
  //          system_connect_info.mstp_status = 0;
		//	TerminateThread(CM5_hThread, 0);
		//	CM5_hThread = NULL;
		//}
		wsk.Close();
		CM5_hThread = CreateThread(NULL,NULL, MSTP_Receive,NULL,NULL, &nThreadID_x);
        CloseHandle(CM5_hThread);
		return;
	}

	if (m_tcp_connect_info.size() != 0)
	{
		for (int x = 0; x < m_tcp_connect_info.size(); x++)
		{
			if (m_bac_scan_result_data.at(0).serialnumber == m_tcp_connect_info.at(x).serialnumber)
			{
				m_tcp_connect_info.at(x).serialnumber = m_bac_scan_result_data.at(0).serialnumber;
				break;
			}
		}
	}
	else
	{
		Client_Info* temp_info = new Client_Info;
		temp_info->client_socket = s;

		char temp_char[200];
		memset(temp_char, 0, 200);
		strcpy(temp_char, ClientIP);
		CString temp_ip;
		MultiByteToWideChar(CP_ACP,NULL, temp_char, strlen(temp_char) + 1, temp_ip.GetBuffer(MAX_PATH),MAX_PATH);
		temp_ip.ReleaseBuffer();

		temp_info->IPAddress = temp_ip;
		temp_info->object_instance = m_bac_scan_result_data.at(0).device_id;
		client_object_instance = temp_info->object_instance;
		temp_info->ipport = ServerPort;
		temp_info->pannel_number = m_bac_scan_result_data.at(0).panel_number;
		temp_info->Product_name = GetProductName(m_bac_scan_result_data.at(0).product_type);
		temp_info->protocol = m_bac_scan_result_data.at(0).m_protocol;
		temp_info->serialnumber = m_bac_scan_result_data.at(0).serialnumber;
		m_tcp_connect_info.push_back(*temp_info);

		PostMessage(m_tcp_server_hwnd,WM_REFRESH_LIST, ADD_ITEM, (LPARAM)temp_info);
	}

#pragma endregion getserialnumber

#pragma region Save_To_DB
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	int is_found_in_db = false;
	for (int x = 0; x < pFrame->m_product.size(); x++)
	{
		if (m_bac_scan_result_data.at(0).serialnumber == pFrame->m_product.at(x).serial_number)
			is_found_in_db = true;
	}

	if (is_found_in_db) //Found in DB  
	{
		CString strSql;
		//pFrame->m_pCon.CreateInstance(_T("ADODB.Connection"));	//这里要用互斥变量保护起来;
		//pFrame->m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
		CString str_serialid;
		CString str_screen_name;
		CString str_product_name;
		CString str_hinstance;
		CString str_panelnumber;
		str_serialid.Format(_T("%u"), m_bac_scan_result_data.at(0).serialnumber);
		str_product_name = GetProductName(m_bac_scan_result_data.at(0).product_type);
		str_screen_name = _T("GSM - ") + str_product_name + _T(":") + str_serialid;
		str_hinstance.Format(_T("%u"), m_bac_scan_result_data.at(0).device_id);
		str_panelnumber.Format(_T("%u"), m_bac_scan_result_data.at(0).panel_number);
		strSql.Format(_T("update ALL_NODE set Protocol ='5' , Product_name ='%s',Hardware_Ver ='%s',Software_Ver ='%s' where Serial_ID = '%s'"), str_screen_name, str_hinstance, str_panelnumber, str_serialid); //5对应的是GSM协议;
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


		//	 ::PostMessage(MainFram_hwd,WM_MYMSG_REFRESHBUILDING,0,0);
	}
	else //插入;
	{
		CString strSql;

		CString str_serialid;
		CString str_screen_name;
		CString str_product_name;
		CString str_hinstance;
		CString str_panelnumber;
		CString product_class_id;
		CString str_ip_address;
		CString str_n_port;
		CString strmodbusid;
		str_serialid.Format(_T("%u"), m_bac_scan_result_data.at(0).serialnumber);
		str_product_name = GetProductName(m_bac_scan_result_data.at(0).product_type);
		str_screen_name = _T("GSM - ") + str_product_name + _T(":") + str_serialid;
		str_hinstance.Format(_T("%u"), m_bac_scan_result_data.at(0).device_id);
		str_panelnumber.Format(_T("%u"), m_bac_scan_result_data.at(0).panel_number);
		product_class_id.Format(_T("%d"), m_bac_scan_result_data.at(0).product_type);
		strmodbusid.Format(_T("%d"), m_bac_scan_result_data.at(0).modbus_addr);
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Custom)   values('"
			+pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurSubBuldingName+"','"+str_serialid+"','floor1','room1','"
			+str_screen_name+"','"+product_class_id+"','"+strmodbusid+"','""','"+str_ip_address+"','Modbus_and_Bacnet','"
			+str_hinstance+"','"+str_panelnumber+"','"+str_n_port+"','0','5','0')"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


		//
	}
	SqliteDBBuilding.closedb();
	::PostMessage(MainFram_hwd,WM_MYMSG_REFRESHBUILDING, 0, 0);

#pragma endregion Save_To_DB

	while (1)
	{
		//info.Empty();
		if (wsk.IsSockConnected(s))
		{
			Sleep(30000);
		}
		else
		{
			break;
		}
	}


	if (CM5_hThread != NULL)
	{
        system_connect_info.mstp_status = 0;
		TerminateThread(CM5_hThread, 0);
		CM5_hThread = NULL;
	}
	bip_set_socket(my_sokect);
	bip_setgsm(false);
	wsk.Close();
	CM5_hThread = CreateThread(NULL,NULL, MSTP_Receive,NULL,NULL, &nThreadID_x);
    CloseHandle(CM5_hThread);
	Client_Info* temp_info2 = new Client_Info;
	temp_info2->serialnumber = m_bac_scan_result_data.at(0).serialnumber;
	PostMessage(m_tcp_server_hwnd,WM_REFRESH_LIST, DELETE_ITEM, (LPARAM)temp_info2);
}

void CTCP_Server::Initial_List()
{
	m_tcp_server_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	//m_tcp_server_list.SetExtendedStyle(m_tcp_server_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_tcp_server_list.SetExtendedStyle(m_tcp_server_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_tcp_server_list.InsertColumn(TCP_ITEM, _T("Item"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_tcp_server_list.InsertColumn(TCP_IP_ADDRESS, _T("IP Address"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tcp_server_list.InsertColumn(TCP_IP_PORT, _T("Port"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tcp_server_list.InsertColumn(TCP_PRODUCT_NAME, _T("Product Name"), 140, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tcp_server_list.InsertColumn(TCP_SERIALNUMBER, _T("Serial Number"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

	m_tcp_server_hwnd = this->m_hWnd;
	g_hwnd_now = m_tcp_server_hwnd;

	m_tcp_server_list.SetWhetherShowBkCol(false);

	CRect list_rect, win_rect;
	m_tcp_server_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_tcp_server_hwnd, win_rect);
	m_tcp_server_list.Set_My_WindowRect(win_rect);
	m_tcp_server_list.Set_My_ListRect(list_rect);
	m_tcp_server_list.Dont_DT_Left();
	m_tcp_server_list.DeleteAllItems();
}


void CTCP_Server::Show_List_Data(Client_Info* temp_client)
{
	m_tcp_server_list.DeleteAllItems();
	for (int i = 0; i < m_tcp_connect_info.size(); i++)
	{
		CString temp_item;
		CString temp_port;
		CString temp_serialnumber;
		temp_serialnumber.Format(_T("%u"), temp_client->serialnumber);
		temp_port.Format(_T("%d"), temp_client->ipport);
		temp_item.Format(_T("%d"), i + 1);
		m_tcp_server_list.InsertItem(i, temp_item);
		m_tcp_server_list.SetItemText(i, TCP_IP_ADDRESS, temp_client->IPAddress);
		m_tcp_server_list.SetItemText(i, TCP_IP_PORT, temp_port);
		m_tcp_server_list.SetItemText(i, TCP_PRODUCT_NAME, temp_client->Product_name);
		m_tcp_server_list.SetItemText(i, TCP_SERIALNUMBER, temp_serialnumber);
	}
}

LRESULT CTCP_Server::RefreshList(WPARAM wParam, LPARAM lParam)
{
	Client_Info* temp_client = (Client_Info *)lParam;
	int ncommand = (int)wParam;
	int list_count = 0;
	switch (ncommand)
	{
	case ADD_ITEM:
		{
			Show_List_Data(temp_client);
		}
		break;
	case DELETE_ITEM:
		{
			int delete_item = -1;
			for (int i = 0; i < (int)m_tcp_connect_info.size(); i++)
			{
				if (temp_client->serialnumber == m_tcp_connect_info.at(i).serialnumber)
				{
					delete_item = i;
					break;
				}
			}
			if (delete_item != -1)
			{
				m_tcp_connect_info.erase(m_tcp_connect_info.begin() + delete_item);
			}

			Show_List_Data(temp_client);
		}
		break;
	default:
		break;
	}
	delete temp_client;
	return 0;
}
