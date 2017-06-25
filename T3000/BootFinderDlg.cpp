// BootFinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BootFinderDlg.h"
#include "afxdialogex.h"
#include "global_function.h"


   // CBootFinderDlg dialog
   char sendbuf[45];
   int ISP_STEP;
   //vector <Product_IP_ID> Product_Info;
   BYTE Byte_ISP_Device_IP[4];
   BYTE Product_Name[12];
   BYTE Rev[4];
   bool device_has_replay_lan_IP=false;
   volatile int package_number=1;
   volatile int next_package_number=1;
   bool device_jump_from_runtime=false;
   bool dhcp_package_is_broadcast=false;
   bool has_enter_dhcp_has_lanip_block=false;

   CString global_message =L"" ;

   DWORD WINAPI _DetectInBootloader(LPVOID pParam )
   {

	   CBootFinderDlg *pdlg = (CBootFinderDlg *)pParam;
     
     CString strTips;
     int mode_send_flash_try_time = 0  ;
     ISP_STEP = ISP_SEND_FLASH_COMMAND;

     while(/*((mode_send_flash_try_time++)<10)&& */ISP_STEP == ISP_SEND_FLASH_COMMAND)
     {
         ++mode_send_flash_try_time  ;
         pdlg->SetDHCP_Data();
         dhcp_package_is_broadcast = true;
         BOOL bBroadcast=TRUE;
         //::sendto(dhcpSock,(char*)pBuffer, nDhcpLen,0,(sockaddr*)&siBroadCast,sizeof(siBroadCast));
         pdlg->SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
         int send_count = pdlg->SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,_T("255.255.255.255"),0);
         global_message.Format (_T("Send Boot Command Time = %d"),mode_send_flash_try_time);
         ::PostMessage(pdlg->m_hWnd,WM_BOOTMESSAGE,0,0);
         if(send_count <= 0)
         {
             //pdlg->m_message_list.AddString(_T("Send command failed!"));
             global_message= _T("Send command failed!");
             ::PostMessage(pdlg->m_hWnd,WM_BOOTMESSAGE,0,0);
         } 
         Sleep (200);
           
     }

     pdlg->ISP_Device_IP.Empty();
     pdlg->ISP_Device_IP.Format(_T("%d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
     pdlg->m_StrProductName.Format(_T("%C%C%C%C%C%C%C%C%C%C%C%C%C"),Product_Name[0],Product_Name[1],Product_Name[2],Product_Name[3],Product_Name[4]
     ,Product_Name[5],Product_Name[6],Product_Name[7],Product_Name[8],Product_Name[9]
     ,Product_Name[10],Product_Name[11]);
     pdlg->m_StrProductName.TrimLeft();
     pdlg->m_StrProductName.TrimRight();
     pdlg->m_StrRev.Format(_T("%C%C%C%C"),Rev[0],Rev[1],Rev[2],Rev[3]);
     pdlg->m_StrRev.TrimLeft();
     pdlg->m_StrRev.TrimRight();
     global_message.Format (_T("Bootloader Model Device IP and Name :%s  %s"),pdlg->ISP_Device_IP,pdlg->m_StrProductName);// =   +L""
     ::PostMessage(pdlg->m_hWnd,WM_BOOTMESSAGE,0,0);
     Sleep(20000);//休息三秒钟
     RefreshNetWorkDeviceListByUDPFunc();
     Sleep (5000);
     BOOL FindDevice = FALSE;
     for (int i = 0 ;i < m_refresh_net_device_data.size ();i++)
     {
          if (m_refresh_net_device_data.at (i).ip_address.CompareNoCase(pdlg->ISP_Device_IP) == 0)  //找到了 
          {
             FindDevice = TRUE;
             break;   
          }
          
     }
     if (FindDevice)
     {
         global_message.Format (_T("Scan Find Device IP and Name :%s  %s is running..."),pdlg->ISP_Device_IP,pdlg->m_StrProductName);// =   +L""
         ::PostMessage(pdlg->m_hWnd,WM_BOOTMESSAGE,0,0); 
     }
     else
     {
         global_message.Format (_T("Scan Command can't find Device IP and Name :%s  %s"),pdlg->ISP_Device_IP,pdlg->m_StrProductName);// =   +L""
         ::PostMessage(pdlg->m_hWnd,WM_BOOTMESSAGE,0,0);

         Sleep (2000);
         global_message.Format (_T("Please update its firmware of Device IP :%s"),pdlg->ISP_Device_IP);// =   +L""
         ::PostMessage(pdlg->m_hWnd,WM_BOOTMESSAGE,0,0);

     }
    return 1;
}

IMPLEMENT_DYNAMIC(CBootFinderDlg, CDialogEx)

CBootFinderDlg::CBootFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBootFinderDlg::IDD, pParent)
{
       m_dwClientIP = 0;
       hFirstThread = NULL;
       m_line= 0;

}

CBootFinderDlg::~CBootFinderDlg()
{
}

void CBootFinderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_message_list);
}


BEGIN_MESSAGE_MAP(CBootFinderDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CBootFinderDlg::OnBnClickedOk)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

DWORD CBootFinderDlg::GetLocalIP()
{
    IP_ADAPTER_INFO pAdapterInfo;
    ULONG len = sizeof(pAdapterInfo); 
    if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS) 
    {
        return 0;
    }
    //SOCKADDR_IN sockAddress;   // commented by zgq;2010-12-06; unreferenced local variable
    long nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);
    return htonl(nLocalIP);
}
// CBootFinderDlg message handlers
bool CBootFinderDlg::IP_is_Local()
{

    DWORD dwIP = GetLocalIP();
    BYTE byIP[4];
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byIP[i] = (dwIP >> ic*8)&0x000000FF;
    }

    BYTE byISPDeviceIP[4];
    DWORD dwClientIP = m_dwClientIP;
    ZeroMemory(byISPDeviceIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byISPDeviceIP[i] = (dwClientIP >> ic*8)&0x000000FF;
    }
    if(memcmp(byIP,byISPDeviceIP,3)==0)
    {
        return true;
    }

    //memcpy_s(byIP,3,byISPDeviceIP,3)
    return false;
}
void CBootFinderDlg::SetDHCP_Data()
{
    DWORD dwgateway = 0;
    DWORD dwSubnetMask=0;
    DWORD dwClientIP=0;

    //DHCP_PACKET MYDHCP_PACKET;

    //memcpy_s(MYDHCP_PACKET.Header,sizeof(MYDHCP_PACKET.Header),"Temcocontrols",13);
    memcpy_s(sendbuf,13,"Temcocontrols",13);



    if(IP_is_Local())
    {
        //MYDHCP_PACKET.AssignIp=m_dwClientIP;
        dwClientIP = m_dwClientIP;
    }
    else
    {
        //MYDHCP_PACKET.AssignIp=0;
        dwClientIP = 0;
    }


    BYTE byIP[4];
    ZeroMemory(byIP,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        byIP[i] = (dwClientIP >> ic*8)&0x000000FF;
    }
    memcpy_s(sendbuf+13,4,byIP,4);


    //MYDHCP_PACKET.SubnetMask=0xffffff00;
    BYTE btSubnetMask[4];
    //dwSubnetMask = MYDHCP_PACKET.SubnetMask;
    dwSubnetMask = 0xffffff00;
    ZeroMemory(btSubnetMask,4);
    for (int i = 0, ic = 3; i < 4; i++,ic--)
    {
        btSubnetMask[i] = (dwSubnetMask >> ic*8)&0x000000FF;
    }
    memcpy_s(sendbuf+17,4,btSubnetMask,4);

    //MYDHCP_PACKET.GatewayIp=0;
    BYTE	Gateway[4];
    ZeroMemory(Gateway,4);
    memcpy_s(sendbuf+21,4,Gateway,4);

    if(device_has_replay_lan_IP == true)
        memcpy_s(sendbuf+25,4,Byte_ISP_Device_IP,4);	//ISP_Device_IP is the device ip address and read in runtime.
    else
    {
        memset(Byte_ISP_Device_IP,0,4);
        memcpy_s(sendbuf+25,4,Byte_ISP_Device_IP,4);	
    }

    char reserved[16];
    ZeroMemory(reserved,16);
    memcpy_s(sendbuf+29,16,reserved,16);
}
void CBootFinderDlg::OnBnClickedOk()
{
    
    //CString strTips;
    //int mode_send_flash_try_time = 0  ;
    //ISP_STEP = ISP_SEND_FLASH_COMMAND;

    //while(((mode_send_flash_try_time++)<10)&& ISP_STEP == ISP_SEND_FLASH_COMMAND)
    //{
    //        SetDHCP_Data();
    //        dhcp_package_is_broadcast = true;
    //        BOOL bBroadcast=TRUE;
    //        //::sendto(dhcpSock,(char*)pBuffer, nDhcpLen,0,(sockaddr*)&siBroadCast,sizeof(siBroadCast));
    //        SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
    //        int send_count = SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,_T("255.255.255.255"),0);
    //        if(send_count <= 0)
    //        {
    //              m_message_list.AddString(_T("Send command failed!"));
    //        }   
    //}

    //ISP_Device_IP.Empty();
    //ISP_Device_IP.Format(_T("%d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
    //m_StrProductName.Format(_T("%C%C%C%C%C%C%C%C%C%C%C%C%C"),Product_Name[0],Product_Name[1],Product_Name[2],Product_Name[3],Product_Name[4]
    //,Product_Name[5],Product_Name[6],Product_Name[7],Product_Name[8],Product_Name[9]
    //,Product_Name[10],Product_Name[11]);
    //m_StrProductName.TrimLeft();
    //m_StrProductName.TrimRight();
    //m_StrRev.Format(_T("%C%C%C%C"),Rev[0],Rev[1],Rev[2],Rev[3]);
    //m_StrRev.TrimLeft();
    //m_StrRev.TrimRight();

    //m_message_list.AddString(ISP_Device_IP);
    //m_message_list.AddString(m_StrProductName);

  
    //while(1)
    //{
    //    switch(ISP_STEP)
    //    {
    //    case ISP_SEND_FLASH_COMMAND:
    //        if((mode_send_flash_try_time++)<10)
    //        {

                //	SendFlashCommand();


                //int send_ret=TCP_Flash_CMD_Socket.SendTo(byCommand,sizeof(byCommand),m_nClientPort,ISP_Device_IP,0);
                ////TRACE(_T("%d"),send_ret);
                //if(send_ret<0)	//如果发送失败 就尝试 再次进行TCP连接
                //{
                //	TCP_Flash_CMD_Socket.Connect(ISP_Device_IP,m_nClientPort);
                //}
                //SetDHCP_Data();


                ////if(IP_is_Local())//如果是本地的 就用广播的 方式 发送 DHCP
                ////{
                //dhcp_package_is_broadcast = true;
                //BOOL bBroadcast=TRUE;
                ////::sendto(dhcpSock,(char*)pBuffer, nDhcpLen,0,(sockaddr*)&siBroadCast,sizeof(siBroadCast));
                //SendUDP_Flash_Socket.SetSockOpt(SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL),SOL_SOCKET);
                //int send_count = SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,_T("255.255.255.255"),0);
                //if(send_count <= 0)
                //    AfxMessageBox(_T("Send command failed!"));
                //}
                //else
                //SendUDP_Flash_Socket.SendTo(sendbuf,sizeof(sendbuf),FLASH_UDP_PORT,ISP_Device_IP,0);



            /*     strTips.Format(_T("Communication with device.(Remain time:%d)\n"),10-mode_send_flash_try_time);
                 TRACE (strTips);*/
                // m_message_list.AddString(strTips);
                // Sleep (500);
                /*OutPutsStatusInfo(strTips, TRUE);*/
      /*      }
            else
            {*/
                //nRet=StartServer_Old_Protocol();
                //if(nRet==0)
                //{
               /* strTips = _T("No Connection!Please check the network connection.");
                OutPutsStatusInfo(strTips, FALSE);	*/			
                //}


           //     goto StopServer;
                //if(some_device_reply_the_broadcast == true)	//这个是用来支持 多个 在bootload中回复的 广播，如果有 则 先打印其中有几个回复
                //{
                //	CString temp_pro_cs;
                //	temp_pro_cs.Format(_T("Find %d device in ISP mode"),Product_Info.size());
                //	OutPutsStatusInfo(temp_pro_cs, FALSE);
                //	for (int i=0;i<((int)Product_Info.size());i++)
                //	{
                //		temp_pro_cs.Format(_T("No.%d -----> IP ="),i+1);
                //		temp_pro_cs = temp_pro_cs + Product_Info.at(i).ISP_Device_IP;
                //		OutPutsStatusInfo(temp_pro_cs, FALSE);
                //	}
                //	ISP_STEP =ISP_SEND_DHCP_COMMAND_HAS_LANIP;
                //	broadcast_flash_count = Product_Info.size();
                //	continue;
                //}
          /*        ISP_STEP = -1;
            }
           
              break;

        }
    }*/
   
  /*  ISP_Device_IP.Empty();
    ISP_Device_IP.Format(_T("%d.%d.%d.%d"),Byte_ISP_Device_IP[0],Byte_ISP_Device_IP[1],Byte_ISP_Device_IP[2],Byte_ISP_Device_IP[3]);
    m_StrProductName.Format(_T("%C%C%C%C%C%C%C%C%C%C%C%C%C"),Product_Name[0],Product_Name[1],Product_Name[2],Product_Name[3],Product_Name[4]
    ,Product_Name[5],Product_Name[6],Product_Name[7],Product_Name[8],Product_Name[9]
    ,Product_Name[10],Product_Name[11]);
    m_StrProductName.TrimLeft();
    m_StrProductName.TrimRight();
    m_StrRev.Format(_T("%C%C%C%C"),Rev[0],Rev[1],Rev[2],Rev[3]);
    m_StrRev.TrimLeft();
    m_StrRev.TrimRight();*/
  //  AfxMessageBox (L"OK");
   // CDialogEx::OnOK();
}


BOOL CBootFinderDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    int Udp_resualt=SendUDP_Flash_Socket.Create(LOCAL_UDP_PORT,SOCK_DGRAM);
    if(Udp_resualt == 0)
    {
        DWORD error_msg=GetLastError();
        //         TCHAR szBuf[250]; 
        //         LPVOID lpMsgBuf;
        //         FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 	FORMAT_MESSAGE_FROM_SYSTEM,	NULL,	error_msg,
        //             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	(LPTSTR) &lpMsgBuf,	0, NULL );
        //         wsprintf(szBuf,_T("failed with error %d: %s"), 	 error_msg, lpMsgBuf); 
        // 
        //         AfxMessageBox(szBuf);
        //         LocalFree(lpMsgBuf);
        // 
        //         PostMessage(m_pDlg->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(0));
        return FALSE;
    }
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    hFirstThread=NULL;
    if (!hFirstThread)
    {
        hFirstThread = CreateThread(NULL,NULL,_DetectInBootloader,this,NULL,0);
    } 

    return TRUE;  
    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CBootFinderDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_BOOTMESSAGE)
    {
         m_line = m_message_list.GetCount ();
         m_message_list.InsertString (m_line,global_message);
         m_message_list.SetCurSel (m_line - 1);
         
    }

    return CDialogEx::WindowProc(message, wParam, lParam);
}


void CBootFinderDlg::OnDestroy()
{


    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    hFirstThread=NULL;


    CDialogEx::OnDestroy();

    
}
