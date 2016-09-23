// ConnectRemoteServer.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ConnectRemoteServer.h"
#include "afxdialogex.h"
#include "globle_function.h"
#include "MainFrm.h"
int static_step = 0;
CString ConnectMessage[20];
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
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	CMemDC memDC(dc,this);
	CRect rcClient;
	GetClientRect(&rcClient);
	memDC.GetDC().FillSolidRect(&rcClient,RGB(230,230,230));
	//Graphics graphics(memDC.GetDC());
	SolidBrush *BlackBrush;
	//SolidBrush *CharacterBlackBrush;

	Graphics *mygraphics;
	mygraphics = new Graphics(memDC.GetDC());
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	BlackBrush =new  SolidBrush(MY_COLOR_RED) ;
	//CharacterBlackBrush = new SolidBrush(MY_COLOR_BLACK_CHARACTER);


	mygraphics->FillRectangle(BlackBrush,0,0,rcClient.Width(),40);

	FontFamily  CharacterfontFamily(_T("Arial"));
	PointF     TitlepointF(0, 0);
	SolidBrush  TitleCharacterColor(Color(255,255,255,255));
	Gdiplus::Font  Scroll_font(&CharacterfontFamily, 28, FontStyleBold, UnitPixel);
	TitlepointF.X = 250;
	TitlepointF.Y = 5;
	mygraphics->DrawString(_T("Remote Connect procedure"), -1, &Scroll_font, TitlepointF,&TitleCharacterColor);

	PointF      WarningMessagePoint(0, 0);
	SolidBrush  WarningMessageColor(Color(255,0,0,0));
	Gdiplus::Font  WarningMessageFont(&CharacterfontFamily, 21, FontStyleRegular, UnitPixel);
	SolidBrush  MessageRetColor(Color(255,0,255,255));

	//CString step1_message;

	for (int z=0;z<static_step ;z++)
	{
		WarningMessagePoint.X = 25  ;
		WarningMessagePoint.Y = 45 + z*25;
		//CString temp_cs = ConnectMessage[static_step];
		//step1_message.Format(_T("%d. Connect newfirmware.com "),CONNECT_NEWFIRMWARE_SERVER);
		mygraphics->DrawString(ConnectMessage[z], -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);

	}
end_connect_paint:
	delete mygraphics;
	delete BlackBrush;

	return;

#if 0
	WarningMessagePoint.X = 25;
	WarningMessagePoint.Y = 75;
	mygraphics->DrawString(_T("Would you like to change the IP address of the device"), -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);
	WarningMessagePoint.X = 25;
	WarningMessagePoint.Y = 105;
	mygraphics->DrawString(_T("to match the IP address of your PC?"), -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);

	WarningMessagePoint.X = 220;
	WarningMessagePoint.Y = 160;
	mygraphics->DrawString(_T("Current"), -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);

	WarningMessagePoint.X = 470;
	WarningMessagePoint.Y = 160;
	mygraphics->DrawString(_T("Proposed"), -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);


	mygraphics->FillRectangle(CharacterBlackBrush,BLACK_GROUND_START_X,BLACK_GROUND_START_Y,BLACK_GROUND_WIDTH,BLACK_GROUND_HEIGHT);
	mygraphics->FillRectangle(CharacterBlackBrush,BLACK_2_GROUND_START_X,BLACK_2_GROUND_START_Y,BLACK_2_GROUND_WIDTH,BLACK_2_GROUND_HEIGHT);

	CString StrTempOrgDeviceIP;
	StrTempOrgDeviceIP.Format(_T("Device IP address:  %d"),44444);
	WarningMessagePoint.X = BLACK_GROUND_START_X + 5;
	WarningMessagePoint.Y = BLACK_GROUND_START_Y + 5;
	mygraphics->DrawString(StrTempOrgDeviceIP, -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);

	mygraphics->FillRectangle(CharacterBlackBrush,BLACK_PC_GROUND_START_X,BLACK_PC_GROUND_START_Y,BLACK_PC_GROUND_WIDTH,BLACK_PC_GROUND_HEIGHT);
	CString StrTempPCIP;
	StrTempPCIP.Format(_T("PC's IP address:     %d"),55555);
	WarningMessagePoint.X = BLACK_PC_GROUND_START_X + 5;
	WarningMessagePoint.Y = BLACK_PC_GROUND_START_Y + 5;
	mygraphics->DrawString(StrTempPCIP, -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);
#endif
}
HANDLE tcp_client_thread = NULL; // 连接server 的 TCP状态 线程;

BOOL CConnectRemoteServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	static_step = 0;
	// TODO:  Add extra initialization here
	if(tcp_client_thread == NULL)
		tcp_client_thread = CreateThread(NULL,NULL,TcpClient_Connect_Thread,this,NULL, NULL);
	else
	{
		TerminateThread(tcp_client_thread,0);
		tcp_client_thread = CreateThread(NULL,NULL,TcpClient_Connect_Thread,this,NULL, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

DWORD WINAPI  TcpClient_Connect_Thread(LPVOID lpVoid)
{
	CConnectRemoteServer *pParent = (CConnectRemoteServer *)lpVoid;
	//int resualt=pParent->TCPConnectServerSocket.Create(0,SOCK_STREAM);//SOCK_STREAM
	//if(resualt > 0)
	//{
		CString temp_db_ini_folder;
		temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");

		int is_local_temco_net = false;
		is_local_temco_net  = GetPrivateProfileInt(_T("Setting"),_T("LocalTemcoNet"),0,temp_db_ini_folder);
		if(is_local_temco_net == false)
		{
			WritePrivateProfileStringW(_T("Setting"),_T("LocalTemcoNet"),_T("0"),temp_db_ini_folder);
			WritePrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_db_ini_folder);
		}

		ConnectMessage[static_step].Format(_T("Connect to newfirmware.com , please wait!"));
		static_step ++ ;

		if(is_local_temco_net == false)
		{
			hostent* host = gethostbyname("newfirmware.com");
			if(host == NULL)
			{
				ConnectMessage[static_step].Format(_T("Can't access Firmware server.Please check your internet connection!"));
				static_step ++ ;
				tcp_client_thread = 0;
				return 1;
			}
			char* pszIP  = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr)); 
			memcpy_s((char *)dyndns_ipaddress,20,pszIP,20);
		}
		else
		{
			CString temp_ip;
			GetPrivateProfileStringW(_T("Setting"),_T("TemcoServerIP"),_T("192.168.0.4"),temp_ip.GetBuffer(MAX_PATH),MAX_PATH,temp_db_ini_folder);
			temp_ip.ReleaseBuffer();

			ConnectMessage[static_step].Format(_T("Get local Server IP success!"));
			static_step ++ ;

			char temp_char_ip[40];
			memset(temp_char_ip,0,40);
			WideCharToMultiByte( CP_ACP, 0, temp_ip.GetBuffer(), -1, temp_char_ip, 255, NULL, NULL );
			memcpy_s((char *)dyndns_ipaddress,20,temp_char_ip,20);
		}

		CString Connect_ip;
		Connect_ip = dyndns_ipaddress;


	int nNetTimeout=4000;//1 second.
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);

	SOCKET tcp_client_socket=NULL;

    if(::WSAStartup(sockVersion, &wsaData) != 0)
    {
        //AfxMessageBox(_T("Init Socket failed!"));
        tcp_client_socket=NULL;
        return FALSE;
    }
	int sockfd;
   sockfd = tcp_client_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(tcp_client_socket == INVALID_SOCKET)
    {
        //	AfxMessageBox(_T("Create socket failed!"));
        tcp_client_socket=NULL;
        return FALSE;
    }
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(31234);
    USES_CONVERSION;
    servAddr.sin_addr.S_un.S_addr = (inet_addr(dyndns_ipaddress));
    //发送时限
    setsockopt(tcp_client_socket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
    //接收时限
    setsockopt(tcp_client_socket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

    //****************************************************************************
    // Fance added ,不要用阻塞的模式，如果设备不在线 经常性的 要等10几秒 ，老毛受不了。
    //改为非阻塞的 2.5秒后还没连接上就 算连接失败;
    int error = -1;
    int len;
    len = sizeof(int);
    timeval tm;
    fd_set set;
    unsigned long ul = 1;
    ioctlsocket(tcp_client_socket, FIONBIO, &ul); //设置为非阻塞模式
    bool ret = false;
    if( connect(tcp_client_socket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
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
		ConnectMessage[static_step].Format(_T("Connect newfirmware.com success!"));
		static_step ++ ;
		ConnectMessage[static_step].Format(_T("Reading panel information from server ,please wait!"));
		static_step ++ ;
        //return true;
    }
    else
    {
		ConnectMessage[static_step].Format(_T("Connect newfirmware.com failed!"));
		static_step ++ ;
		tcp_client_thread = 0;
		return 1;
    }

	char send_buffer[200];
	memset(send_buffer,0,200);
	send_buffer[0] = 0xff;
	send_buffer[1] = 0x55;
	send_buffer[2] = GET_MINIPANEL_INFO;
	memcpy_s(&send_buffer[3],4,&try_connect_serial,4);





    int n=::send(tcp_client_socket,send_buffer,7,0);

	char receiver_buffer[1000];
	Sleep(3000);

	int nRecv = ::recv(tcp_client_socket, (char*)receiver_buffer, 1000, 0);
	
	bool device_is_connected = false;
	STR_For_T3000 temp_rm;
	if((receiver_buffer[0]!= 0xff) || (receiver_buffer[1] != 0x55))
	{
		if(receiver_buffer[2] == RECEIVE_MINIPANEL_NOT_CONNECT_YET)
		{
			ConnectMessage[static_step].Format(_T("Device donesn't connect to the server yet!"));
			static_step ++ ;
		}
		else if(receiver_buffer[2] == RETURN_MINI_DATA)
		{
			memcpy_s(&temp_rm,sizeof(STR_For_T3000),&receiver_buffer[3],sizeof(STR_For_T3000));

			ConnectMessage[static_step].Format(_T("Serial Number : %u"),temp_rm.sn);
			static_step ++ ;
			ConnectMessage[static_step].Format(_T("Product Name : %s"),GetProductName(temp_rm.product_id));
			static_step ++ ;
			ConnectMessage[static_step].Format(_T("Bacnet Object Instance : %u"),temp_rm.object_instance);
			static_step ++ ;
			ConnectMessage[static_step].Format(_T("Panel : %u"),temp_rm.panel);
			static_step ++ ;

			CString cs_extern_ip ;
			in_addr temp_add;
			temp_add.S_un.S_addr=temp_rm.external_ip;

			 cs_extern_ip = CString(inet_ntoa(temp_add));


			ConnectMessage[static_step].Format(_T("Remote IP: %s"),cs_extern_ip);
			static_step ++ ;
			ConnectMessage[static_step].Format(_T("TCP Port Forwarding: %u"),temp_rm.modbus_port);
			static_step ++ ;
			ConnectMessage[static_step].Format(_T("UDP Port Forwarding: %u"),temp_rm.bacnet_port);
			static_step ++ ;


			CTime time_scaletime;
			CString strTime;
			time_t scale_time  = temp_rm.update_time;
			time_scaletime = scale_time;
			strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");


			ConnectMessage[static_step].Format(_T("Update Time : %s"),strTime);
			static_step ++ ;
			device_is_connected = true;
		}
	}
	if(device_is_connected)
	{

		CBADO bado;
		bado.SetDBPath(g_strCurBuildingDatabasefilePath);
		bado.OnInitADOConn();

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
		CString is_custom  =  _T("0");;
		str_serialid.Format(_T("%u"),temp_rm.sn);
		product_name.Format(_T("%s"),GetProductName(temp_rm.product_id) + _T("-SN") + str_serialid);
		modbusid.Format(_T("%u"),255);
		product_class_id.Format(_T("%u"),temp_rm.product_id);
		in_addr temp_add;
		temp_add.S_un.S_addr=temp_rm.external_ip;
		str_ip_address = CString(inet_ntoa(temp_add));
		str_n_port.Format(_T("%u"),temp_rm.modbus_port);
		str_panel_number.Format(_T("%u"),temp_rm.panel);
		str_object_instance.Format(_T("%u"),temp_rm.object_instance);
		try
		{

			CString strSql;

			strSql.Format(_T("Delete * From  ALL_NODE Where Serial_ID = '%d' "),temp_rm.sn);
			bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch (...)
		{

		}
		CString strSql;
		CString temp_pro3;
		temp_pro3.Format(_T("%u"),PROTOCOL_REMOTE_IP);
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)   values('"+pFrame->m_strCurMainBuildingName+"','"+pFrame->m_strCurSubBuldingName+"','"+str_serialid+"','floor1','room1','"+product_name+"','"+product_class_id+"','"+modbusid+"','""','"+str_ip_address+"','T3000_Default_Building_PIC.bmp','"+str_hw_version+"','"+str_fw_version+"','"+str_n_port+"','0','"+temp_pro3+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
		 bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
		 bado.CloseConn();
		::PostMessage(pFrame->m_hWnd,   WM_MYMSG_REFRESHBUILDING,0,0);

	}
	pParent->Invalidate(1);
	tcp_client_thread = 0;
	return 1;
}


void CConnectRemoteServer::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnCancel();
}


void CConnectRemoteServer::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}
