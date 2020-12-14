// ConnectRemoteServer.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ConnectRemoteServer.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "MainFrm.h"
#include "../SQLiteDriver/CppSQLite3.h"
int static_step = 0;
CString ConnectMessage[100];
char dyndns_ipaddress[20];
// CConnectRemoteServer dialog
unsigned int try_connect_serial = 0;
IMPLEMENT_DYNAMIC(CConnectRemoteServer, CDialogEx)

CConnectRemoteServer::CConnectRemoteServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConnectRemoteServer::IDD, pParent)
{
}

CConnectRemoteServer::~CConnectRemoteServer()
{
}

void CConnectRemoteServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConnectRemoteServer, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CConnectRemoteServer message handlers

#define CONNECT_NEWFIRMWARE_SERVER				1
#define CONNECT_NEWFIRMWARE_SERVER_RET			2

void CConnectRemoteServer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
	CMemDC memDC(dc, this);
	CRect rcClient;
	GetClientRect(&rcClient);
	memDC.GetDC().FillSolidRect(&rcClient,RGB(230,230,230));
	//Graphics graphics(memDC.GetDC());
	SolidBrush* BlackBrush;
	//SolidBrush *CharacterBlackBrush;

	Graphics* mygraphics;
	mygraphics = new Graphics(memDC.GetDC());
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	BlackBrush = new SolidBrush(MY_COLOR_RED);
	//CharacterBlackBrush = new SolidBrush(MY_COLOR_BLACK_CHARACTER);


	mygraphics->FillRectangle(BlackBrush, 0, 0, rcClient.Width(), 40);

	FontFamily CharacterfontFamily(_T("Arial"));
	PointF TitlepointF(0, 0);
	SolidBrush TitleCharacterColor(Color(255, 255, 255, 255));
	Gdiplus::Font Scroll_font(&CharacterfontFamily, 28, FontStyleBold, UnitPixel);
	TitlepointF.X = 250;
	TitlepointF.Y = 5;
	mygraphics->DrawString(_T("Remote Connect procedure"), -1, &Scroll_font, TitlepointF, &TitleCharacterColor);

	PointF WarningMessagePoint(0, 0);
	SolidBrush WarningMessageColor(Color(255, 0, 0, 0));
	Gdiplus::Font WarningMessageFont(&CharacterfontFamily, 21, FontStyleRegular, UnitPixel);
	SolidBrush MessageRetColor(Color(255, 0, 255, 255));

	//CString step1_message;

	if (static_step >= 15)
	{
		CString temp_cs;

		for (int j = 0; j < static_step; j++)
		{
			ConnectMessage[j] = ConnectMessage[j + 1];
		}

		static_step = 14;
	}

	for (int z = 0; z < static_step; z++)
	{
		WarningMessagePoint.X = 25;
		WarningMessagePoint.Y = 45 + z * 25;
		mygraphics->DrawString(ConnectMessage[z], -1, &WarningMessageFont, WarningMessagePoint, &WarningMessageColor);
	}
end_connect_paint:
	delete mygraphics;
	delete BlackBrush;

	return;
}

HANDLE tcp_client_thread = NULL; // 连接server 的 TCP状态 线程;
HANDLE udp_ptp_client_thread = NULL; // UDP 打洞 线程;
BOOL CConnectRemoteServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	static_step = 0;
	


	if (udp_ptp_client_thread == NULL)
		udp_ptp_client_thread = CreateThread(NULL,NULL, UDP_ptp_Thread, this,NULL, NULL);
	else
	{
		TerminateThread(udp_ptp_client_thread,NULL);
		udp_ptp_client_thread = CreateThread(NULL,NULL, UDP_ptp_Thread, this,NULL, NULL);
	}
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

DWORD WINAPI UDP_ptp_Thread(LPVOID lpVoid)
{
	CConnectRemoteServer* pParent = (CConnectRemoteServer *)lpVoid;

#if 1
	hostent* host = gethostbyname("newfirmware.com");
	if (host == NULL)
	{
		ConnectMessage[static_step].Format(_T("Can't access Firmware server.Please check your internet connection!"));
		static_step ++;
		tcp_client_thread = 0;
		pParent->Invalidate(1);
		return 1;
	}
	char* pszIP = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr));
	memset(dyndns_ipaddress, 0, 20);
	memcpy_s((char *)dyndns_ipaddress, 20, pszIP, 20);
#endif


	ConnectMessage[static_step].Format(_T("Connect to Temco Sever!"));
	static_step ++;
	pParent->Invalidate(1);

	SOCKET sockClient = socket(AF_INET,SOCK_DGRAM, 0);
	sockaddr_in addrServer;
	addrServer.sin_addr.S_un.S_addr = inet_addr(dyndns_ipaddress);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(SERVER_MINI_PORT);

	char send_buffer[1000];
	memset(send_buffer, 0, 1000);


	send_buffer[0] = COMMAND_FROM_T3000;
	send_buffer[1] = 0x55;
	send_buffer[2] = 0xff;
	send_buffer[3] = COMMAND_T3000_REQUEST;

	Str_T3000_Connect temp_data;
	temp_data.reg_date.m_serial_number = remote_connect_serial_number;
	memcpy(temp_data.reg_date.login_message.userName, ptpLoginName, 30);
	memcpy(temp_data.reg_date.login_message.password, ptpLoginPassword, 20);

	memcpy(&send_buffer[4], &temp_data,T3000_CONNECT_LENGTH);


	char receive_buffer[1000];

	int temp_len = sizeof(sockaddr);
	int ret_rec = 0;

	//while(1)
	//{

	ConnectMessage[static_step].Format(_T("Request connection to the server!"));
	static_step ++;
	pParent->Invalidate(1);

	sendto(sockClient, send_buffer,T3000_CONNECT_DATA_LENGTH, 0, (sockaddr*)&addrServer, sizeof(sockaddr));
	memset(receive_buffer, 0, 1000);
	ret_rec = recvfrom(sockClient, receive_buffer, 1000, 0, (sockaddr *)&addrServer, &temp_len);

	if (ret_rec <= 0)
	{
		Sleep(1000); //接收出错 延时等待;
		ConnectMessage[static_step].Format(_T("Recvfrom server error!"));
		static_step ++;
		pParent->Invalidate(1);
		goto end_udp_hole_thread;
		//continue;
	}
	//接收到服务器发来的 minipanel 的 信息; 209 个字节
	if (((unsigned char)receive_buffer[0] == 0x55) && ((unsigned char)receive_buffer[1] == 0xff))
	{
		if ((unsigned char)receive_buffer[2] == COMMAND_COMMAND_UNKNOWN)
		{
			//收到的命令格式不正确;
			ConnectMessage[static_step].Format(_T("Command type error!"));
			static_step ++;
			goto end_udp_hole_thread;
		}
		else if ((unsigned char)receive_buffer[2] == COMMAND_PASSWORD_ERROR)
		{
			//账号密码错误;
			ConnectMessage[static_step].Format(_T("Username and password error!"));
			static_step ++;
			goto end_udp_hole_thread;
		}
		else if ((unsigned char)receive_buffer[2] == COMMAND_DEVICE_NOT_CONNECT_ERROR)
		{
			//账号密码错误;
			ConnectMessage[static_step].Format(_T("Device not connect to the server yet!"));
			static_step ++;
			goto end_udp_hole_thread;
		}
		else if ((unsigned char)receive_buffer[2] == COMMAND_DEVICE_NO_HEARTBEAT_ERROR)
		{
			//账号密码错误;
			ConnectMessage[static_step].Format(_T("No heartbeat package from device!"));
			static_step ++;
			goto end_udp_hole_thread;
		}
	}

	if (ret_rec != T3000_MINI_HEARTBEAT_LENGTH_WITH_MINI_PORT)
	{
		//不是收到的minipanel 的端口信息 + minipanel 的 所有信息 ;
		ConnectMessage[static_step].Format(_T("Error comand receive!"));
		static_step ++;
		goto end_udp_hole_thread;
	}
	ConnectMessage[static_step].Format(_T("Get device ip address information from server success!"));
	static_step ++;
	pParent->Invalidate(1);
	//break;
	//}


	char c_ip[16];
	unsigned long temp_ip = 0;
	memcpy(&temp_ip, &receive_buffer[3], 4);
	sockaddr_in addr_device_address;
	addr_device_address.sin_addr.S_un.S_addr = temp_ip;//  inet_addr(c_ip);
	addr_device_address.sin_family = AF_INET;
	addr_device_address.sin_port = ((unsigned char)receive_buffer[8]) * 256 + (unsigned char)receive_buffer[7];
	int test_miniport = ((unsigned char)receive_buffer[7]) * 256 + (unsigned char)receive_buffer[8];
	char test_ip[4];
	test_ip[0] = receive_buffer[3];
	test_ip[1] = receive_buffer[4];
	test_ip[2] = receive_buffer[5];
	test_ip[3] = receive_buffer[6];
	memset(send_buffer, 0, 1000);

	send_buffer[0] = 0x55;
	send_buffer[1] = 0xff;
	send_buffer[2] = COMMAND_T3000_SEND_TO_DEVICE_MAKEHOLE;

	//int nNetTimeout = 5000; //1秒
	//setsockopt( sockClient, SOL_SOCKET, SO_RCVTIMEO, ( char * )&nNetTimeout, sizeof( int ) );

	memset(receive_buffer, 0, 1000);
	ret_rec = recvfrom(sockClient, receive_buffer, 1000, 0, (sockaddr *)&addrServer, &temp_len);
	if (ret_rec <= 0)
	{
		Sleep(1000); //接收出错 延时等待;
		ConnectMessage[static_step].Format(_T("Recvfrom server error , No suuccess COMMAND_MINI_SEDN_T3000_DONE 10  receive!"));
		static_step ++;
		pParent->Invalidate(1);
		goto end_udp_hole_thread;
	}

	if (((unsigned char)receive_buffer[0] != 0x55) || ((unsigned char)receive_buffer[1] != 0xff) || ((unsigned char)receive_buffer[2] != COMMAND_T3000_KEYI_FA_MESSAGE))
	{
		Sleep(1000); //接收出错 延时等待;
		ConnectMessage[static_step].Format(_T("Not 0x10  received!"));
		static_step ++;
		pParent->Invalidate(1);
		goto end_udp_hole_thread;
	}


	Sleep(2000);


	send_buffer[0] = 0x55;
	send_buffer[1] = 0xFF;
	send_buffer[2] = COMMAND_T3000_HOLE_SUCCESS_OR_NOT;
	sendto(sockClient, send_buffer, 3, 0, (sockaddr*)&addr_device_address, sizeof(sockaddr));


	int nNetTimeout = 5000; //1秒
	setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof( int));

	memset(receive_buffer, 0, 1000);
	ret_rec = recvfrom(sockClient, receive_buffer, 1000, 0, (sockaddr *)&addr_device_address, &temp_len);
	if (ret_rec <= 0)
	{
		ConnectMessage[static_step].Format(_T("No device reply . Make UDP Hole failed!"));
		static_step ++;
		pParent->Invalidate(1);
		////取消接收时限
		nNetTimeout = 0;
		setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof( int));
		ret_rec = recvfrom(sockClient, receive_buffer, 1000, 0, (sockaddr *)&addr_device_address, &temp_len);
		goto end_udp_hole_thread;
	}

	if (((unsigned char)receive_buffer[0] != 0x55) || ((unsigned char)receive_buffer[1] != 0xff) || ((unsigned char)receive_buffer[2] != COMMAND_REPLY_MK_HOLE_SUCCESS))
	{
		ConnectMessage[static_step].Format(_T("Receive Command Error,Make UDP Hole failed!"));
		static_step ++;
		pParent->Invalidate(1);


		goto end_udp_hole_thread;
		//goto end_udp_hole_thread;
	}


	while (1)
	{
		send_buffer[0] = 0x31;
		send_buffer[1] = 0x32;
		send_buffer[2] = 0x33;
		send_buffer[3] = 0;

		sendto(sockClient, send_buffer, 4, 0, (sockaddr*)&addr_device_address, sizeof(sockaddr));
		ConnectMessage[static_step].Format(_T("Send message 123 to minipanel"), (unsigned char)test_ip[0], (unsigned char)test_ip[1], (unsigned char)test_ip[2], (unsigned char)test_ip[3], test_miniport);
		static_step ++;
		pParent->Invalidate(1);
		Sleep(2000);

#if 0
		memset(receive_buffer,0,1000);

		sockaddr_in temp_receive_addr;
		ret_rec = recvfrom(sockClient,receive_buffer,1000,0,(sockaddr *)&temp_receive_addr,&temp_len);
		if(ret_rec > 0)
		{
			if(addr_device_address.sin_port != temp_receive_addr.sin_port)
				continue;
			if( memcmp(&addr_device_address.sin_addr,&temp_receive_addr.sin_addr,sizeof(IN_ADDR)) != 0 )
				continue;

			ConnectMessage[static_step].Format(_T("Receive message from device. PTP connection OK!"));
			static_step ++ ;
			pParent->Invalidate(1);
			CString total_char_test;
			total_char_test = _T("Receive from minipanel: ");
			char * temp_print_test = NULL;
			temp_print_test = receive_buffer;
			for (int i = 0; i< ret_rec ; i++)
			{
				CString temp_char_test;
				temp_char_test.Format(_T("%02x"),(unsigned char)*temp_print_test);
				temp_char_test.MakeUpper();
				temp_print_test ++;
				total_char_test = total_char_test + temp_char_test + _T(" ");
			}
			DFTrace(total_char_test);
			break;
		}
#endif
	}


	////取消接收时限
	//nNetTimeout = 0;
	//setsockopt( sockClient, SOL_SOCKET, SO_RCVTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
	//ret_rec = recvfrom(sockClient,receive_buffer,1000,0,(sockaddr *)&addr_device_address,&temp_len);


	Sleep(1);
	udp_ptp_client_thread = NULL;
	return 1;

end_udp_hole_thread:
	ConnectMessage[static_step].Format(_T("PTP connection Failed!"));
	static_step ++;
	pParent->Invalidate(1);

	udp_ptp_client_thread = NULL;
	return 2;
}

DWORD WINAPI TcpClient_Connect_Thread(LPVOID lpVoid)
{
	CConnectRemoteServer* pParent = (CConnectRemoteServer *)lpVoid;
	//int resualt=pParent->TCPConnectServerSocket.Create(0,SOCK_STREAM);//SOCK_STREAM
	//if(resualt > 0)
	//{
	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
	int is_local_temco_net = false;
	is_local_temco_net = GetPrivateProfileInt(_T("Setting"),_T("LocalTemcoNet"), 0, temp_db_ini_folder);
	if (is_local_temco_net == false)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("LocalTemcoNet"),_T("0"), temp_db_ini_folder);
		WritePrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"), temp_db_ini_folder);
	}

	ConnectMessage[static_step].Format(_T("Connect to newfirmware.com , please wait!"));
	static_step ++;

	if (is_local_temco_net == false)
	{
		hostent* host = gethostbyname("newfirmware.com");
		if (host == NULL)
		{
			ConnectMessage[static_step].Format(_T("Can't access Firmware server.Please check your internet connection!"));
			static_step ++;
			tcp_client_thread = 0;
			pParent->Invalidate(1);
			return 1;
		}
		char* pszIP = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr));
		memcpy_s((char *)dyndns_ipaddress, 20, pszIP, 20);
	}
	else
	{
		CString temp_ip;
		GetPrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"), temp_ip.GetBuffer(MAX_PATH),MAX_PATH, temp_db_ini_folder);
		temp_ip.ReleaseBuffer();

		ConnectMessage[static_step].Format(_T("Get local Server IP success!"));
		static_step ++;

		char temp_char_ip[40];
		memset(temp_char_ip, 0, 40);
		WideCharToMultiByte(CP_ACP, 0, temp_ip.GetBuffer(), -1, temp_char_ip, 255, NULL, NULL);
		memcpy_s((char *)dyndns_ipaddress, 20, temp_char_ip, 20);
	}

	CString Connect_ip;
	Connect_ip = dyndns_ipaddress;


	int nNetTimeout = 4000;//1 second.
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);

	SOCKET tcp_client_socket = NULL;

	if (::WSAStartup(sockVersion, &wsaData) != 0)
	{
		//AfxMessageBox(_T("Init Socket failed!"));
		tcp_client_socket = NULL;
		return FALSE;
	}
	int sockfd;
	sockfd = tcp_client_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (tcp_client_socket == INVALID_SOCKET)
	{
		//	AfxMessageBox(_T("Create socket failed!"));
		tcp_client_socket = NULL;
		return FALSE;
	}
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(TEMCO_SERVER_PORT);
	USES_CONVERSION;
	servAddr.sin_addr.S_un.S_addr = (inet_addr(dyndns_ipaddress));
	//发送时限
	setsockopt(tcp_client_socket,SOL_SOCKET,SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	//接收时限
	setsockopt(tcp_client_socket,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//****************************************************************************
	// Fance added ,不要用阻塞的模式，如果设备不在线 经常性的 要等10几秒 。
	//改为非阻塞的 2.5秒后还没连接上就 算连接失败;
	int error = -1;
	int len;
	len = sizeof(int);
	timeval tm;
	fd_set set;
	unsigned long ul = 1;
	ioctlsocket(tcp_client_socket, FIONBIO, &ul); //设置为非阻塞模式
	bool ret = false;
	if (connect(tcp_client_socket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
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
		ConnectMessage[static_step].Format(_T("Connect newfirmware.com success!"));
		static_step ++;
		ConnectMessage[static_step].Format(_T("Reading panel information from server ,please wait!"));
		static_step ++;
		//return true;
	}
	else
	{
		ConnectMessage[static_step].Format(_T("Connect newfirmware.com failed!"));
		static_step ++;
		tcp_client_thread = 0;
		pParent->Invalidate(1);
		return 1;
	}

	char send_buffer[200];
	memset(send_buffer, 0, 200);
	send_buffer[0] = 0xff;
	send_buffer[1] = 0x55;
	send_buffer[2] = GET_MINIPANEL_INFO;
	memcpy_s(&send_buffer[3], 4, &try_connect_serial, 4);


	int n = ::send(tcp_client_socket, send_buffer, 7, 0);

	char receiver_buffer[1000];
	Sleep(3000);

	int nRecv = ::recv(tcp_client_socket, (char*)receiver_buffer, 1000, 0);

	bool device_is_connected = false;
	STR_For_T3000 temp_rm;
	if ((receiver_buffer[0] != 0xff) || (receiver_buffer[1] != 0x55))
	{
		if (receiver_buffer[2] == RECEIVE_MINIPANEL_NOT_CONNECT_YET)
		{
			ConnectMessage[static_step].Format(_T("Device donesn't connect to the server yet!"));
			static_step ++;
			pParent->Invalidate(1);
		}
		else if (receiver_buffer[2] == RETURN_MINI_DATA)
		{
			memcpy_s(&temp_rm, sizeof(STR_For_T3000), &receiver_buffer[3], sizeof(STR_For_T3000));

			ConnectMessage[static_step].Format(_T("Serial Number : %u"), temp_rm.sn);
			static_step ++;
			ConnectMessage[static_step].Format(_T("Product Name : %s"), GetProductName(temp_rm.product_id));
			static_step ++;
			ConnectMessage[static_step].Format(_T("Bacnet Object Instance : %u"), temp_rm.object_instance);
			static_step ++;
			ConnectMessage[static_step].Format(_T("Panel : %u"), temp_rm.panel);
			static_step ++;

			CString cs_extern_ip;
			in_addr temp_add;
			temp_add.S_un.S_addr = temp_rm.external_ip;

			cs_extern_ip = CString(inet_ntoa(temp_add));


			ConnectMessage[static_step].Format(_T("Remote IP: %s"), cs_extern_ip);
			static_step ++;
			ConnectMessage[static_step].Format(_T("TCP Port Forwarding: %u"), temp_rm.modbus_port);
			static_step ++;
			ConnectMessage[static_step].Format(_T("UDP Port Forwarding: %u"), temp_rm.bacnet_port);
			static_step ++;


			CTime time_scaletime;
			CString strTime;
			time_t scale_time = temp_rm.update_time;
			time_scaletime = scale_time;
			strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");


			ConnectMessage[static_step].Format(_T("Update Time : %s"), strTime);
			static_step ++;
			device_is_connected = true;
			pParent->Invalidate(1);
		}
	}
	if (device_is_connected)
	{
		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


		CString str_serialid;
		CString product_name;
		CString modbusid;
		CString product_class_id;
		CString str_ip_address;
		CString str_hw_version = _T("0");
		CString str_fw_version = _T("0");
		CString str_n_port;
		CString str_parents_serial = _T("0");
		CString str_panel_number;
		CString str_object_instance;
		CString is_custom = _T("0");;
		str_serialid.Format(_T("%u"), temp_rm.sn);
		product_name.Format(_T("%s"), GetProductName(temp_rm.product_id) + _T("-SN") + str_serialid);
		modbusid.Format(_T("%u"), 255);
		product_class_id.Format(_T("%u"), temp_rm.product_id);
		in_addr temp_add;
		temp_add.S_un.S_addr = temp_rm.external_ip;
		str_ip_address = CString(inet_ntoa(temp_add));
		str_n_port.Format(_T("%u"), temp_rm.modbus_port);
		str_panel_number.Format(_T("%u"), temp_rm.panel);
		str_object_instance.Format(_T("%u"), temp_rm.object_instance);
		try
		{
			CString strSql;

			strSql.Format(_T("Delete  From  ALL_NODE Where Serial_ID = '%d' "), temp_rm.sn);
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		}
		catch (...)
		{
		}
		CString strSql;
		CString temp_pro3;
		temp_pro3.Format(_T("%u"), PROTOCOL_REMOTE_IP);
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('"+pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurSubBuldingName+"','"+str_serialid+"','floor1','room1','"+product_name+"','"+product_class_id+"','"+modbusid+"','""','"+str_ip_address+"','Modbus_and_Bacnet','"+str_hw_version+"','"+str_fw_version+"','"+str_n_port+"','0','"+temp_pro3+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
		SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		SqliteDBBuilding.closedb();
		::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
	}
	pParent->Invalidate(1);
	tcp_client_thread = 0;
	return 1;
}


void CConnectRemoteServer::OnCancel()
{
	

	CDialogEx::OnCancel();
}


void CConnectRemoteServer::OnClose()
{
	 

	CDialogEx::OnClose();
}
