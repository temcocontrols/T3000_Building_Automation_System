// TroubleShootDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TroubleShootDlg.h"
#include "afxdialogex.h"
 #include "ping.h"
#define SHOW_TX_RX_TS 	g_llTxCount++;g_llRxCount++;if( AfxGetMainWnd()->GetActiveWindow() != NULL ) {CString str;str.Format(_T("Addr:255 [Tx=%d Rx=%d Err=%d]"), g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());}

// CTroubleShootDlg dialog




IMPLEMENT_DYNAMIC(CTroubleShootDlg, CDialogEx)

extern	char local_network_ip[255];
extern CString local_enthernet_ip;

CTroubleShootDlg::CTroubleShootDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTroubleShootDlg::IDD, pParent)

{
	  scan_item = -1;
	  scan_udp_item_TS = 0;
	  scan_bacnet_ip_item = -1;
	  scan_baudrate =0;

	  Step1=FALSE;
	  Step2=FALSE;
}

CTroubleShootDlg::~CTroubleShootDlg()
{
}

void CTroubleShootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_newip);
}


BEGIN_MESSAGE_MAP(CTroubleShootDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTroubleShootDlg::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_BUTTON1, &CTroubleShootDlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, &CTroubleShootDlg::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON4, &CTroubleShootDlg::OnBnClickedButton4)
ON_WM_PAINT()
END_MESSAGE_MAP()


// CTroubleShootDlg message handlers


BOOL CTroubleShootDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString StrTemp;
	StrTemp.Format(_T("Change %s's IP Address"),GetProductName(m_net_product_node.product_class_id));



	StrTemp.Format(_T("%s"),m_net_product_node.BuildingInfo.strIp);

	

	

	GetNewIP(StrTemp,m_net_product_node.NetworkCard_Address);

	//
	m_edit_newip.SetWindowTextW(StrTemp);
	m_edit_newip.textColor(RGB(255,0,0));
	m_edit_newip.bkColor(RGB(255,255,255));

	m_edit_newip.setFont(25,10,NULL,_T("Arial"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTroubleShootDlg::GetNewIP(CString &newIP,CString BaseIP){
	USES_CONVERSION;
	IN_ADDR sa;



	LPCSTR szIP=W2A(BaseIP);
	DWORD dwIP=inet_addr(szIP);

	sa.S_un.S_addr=dwIP;


	for (int i=2;i<255;i++)
	{
		CString anewip;
		anewip.Format(_T("%d.%d.%d.%d"), sa.S_un.S_un_b.s_b1,sa.S_un.S_un_b.s_b2,sa.S_un.S_un_b.s_b3,i);

		if (!TestPing(anewip))
		{
			newIP=anewip;
			return TRUE;
		}


	}
	return FALSE;
}
BOOL CTroubleShootDlg::TestPing(const CString& strIP)
{	
#ifdef CPING_USE_ICMP
	CPing p1;
	CPingReply pr1;
	if (p1.Ping1((LPCTSTR)strIP, pr1))
	{
		// 	hostent* phostent = gethostbyaddr((char *)&pr1.Address.S_un.S_addr, 4, PF_INET);
		// 	printf("%d.%d.%d.%d [%s], replied in RTT:%dms\n", 
		// 		pr1.Address.S_un.S_un_b.s_b1, pr1.Address.S_un.S_un_b.s_b2, pr1.Address.S_un.S_un_b.s_b3, 
		// 		pr1.Address.S_un.S_un_b.s_b4, phostent->h_name, pr1.RTT);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#endif	
}

BOOL CTroubleShootDlg::ChangeNetDeviceIP(CString strIP){
	BOOL ret=FALSE;
	 
	CString strlog;
	int nRet = 0;
	short nmsgType=UPD_BROADCAST_QRY_MSG;
	const DWORD END_FLAG = 0x00000000;
	TIMEVAL time;
	time.tv_sec =3;
	time.tv_usec = 1000;
	fd_set fdSocket;
	BYTE buffer[512] = {0};
	BYTE pSendBuf[1024];
	ZeroMemory(pSendBuf, 255);
	pSendBuf[0] = 0x66;
	memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
	int nSendLen = 17;
	int time_out=0;
	USES_CONVERSION;
	CString stroldipaddress,strnewipadress,strlocalipaddress,strnewsubnet,strnewgateway;
	stroldipaddress=strIP;



	SOCKET sListen=NULL;


// 	for (int i=0;i<allsubnets.size();i++)
// 	{
        m_edit_newip.GetWindowTextW(strnewipadress);

         for (int i = 0;i<g_Vector_Subnet.size ();i++)
         {
             if (CheckTheSameSubnet(g_Vector_Subnet.at(i).StrIP,strnewipadress))
             {
                 strnewgateway = g_Vector_Subnet.at(i).StrGetway;
                 strnewsubnet = g_Vector_Subnet.at(i).StrMask;
                 break;
             }
         }

		//GetNewIP(strnewipadress,allsubnets[i].StrIP);
		if (strnewipadress.Find(_T("0.0.0"))!=-1)//对0.0.0.0的过滤掉
		{
			return FALSE;
		}


#pragma region new_socket
		SOCKET h_scan_Broad=NULL;
		SOCKADDR_IN h_scan_bcast;
		SOCKADDR_IN h_scan_siBind;
		h_scan_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		BOOL bBroadcast=TRUE;
		::setsockopt(h_scan_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
		int iMode=1;
		ioctlsocket(h_scan_Broad,FIONBIO, (u_long FAR*) &iMode);

		BOOL bDontLinger = FALSE;
		setsockopt( h_scan_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );


		//SOCKADDR_IN bcast;
		h_scan_bcast.sin_family=AF_INET;
		//bcast.sin_addr.s_addr=nBroadCastIP;
		h_scan_bcast.sin_addr.s_addr=INADDR_BROADCAST;
		h_scan_bcast.sin_port=htons(UDP_BROADCAST_PORT);


		h_scan_siBind.sin_family=AF_INET;
		h_scan_siBind.sin_addr.s_addr =  INADDR_ANY;



#pragma endregion new_socket

		local_enthernet_ip=m_net_product_node.NetworkCard_Address;
		WideCharToMultiByte( CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL );
		h_siBind.sin_family=AF_INET;
		h_siBind.sin_addr.s_addr =  inet_addr(local_network_ip);
		if( -1 == bind(h_scan_Broad,(SOCKADDR*)&h_siBind,sizeof(h_siBind)))//把网卡地址强行绑定到Socket  
		{
			//MessageBox(_T("Network Initial Fail"));
			ret= FALSE;
		}



		LPCSTR szIP = W2A(stroldipaddress);
		DWORD dwIP = inet_addr(szIP);
		IN_ADDR ia;
		ia.S_un.S_addr = dwIP;
		//////////////////Old IP////////////////////////////////////
		pSendBuf[1]=ia.S_un.S_un_b.s_b1;
		pSendBuf[2]=ia.S_un.S_un_b.s_b2;
		pSendBuf[3]=ia.S_un.S_un_b.s_b3;
		pSendBuf[4]=ia.S_un.S_un_b.s_b4;
		///////////////////New IP///////////////////////////////////////////
		szIP = W2A(strnewipadress);
		dwIP = inet_addr(szIP);
		ia.S_un.S_addr = dwIP;
		///////////////////////////////////////////////////////////
		pSendBuf[5]=ia.S_un.S_un_b.s_b1;
		pSendBuf[6]=ia.S_un.S_un_b.s_b2;
		pSendBuf[7]=ia.S_un.S_un_b.s_b3;
		pSendBuf[8]=ia.S_un.S_un_b.s_b4;
		////////////////////////////////////////////////////////////////////
		szIP = W2A(strnewsubnet);
		dwIP = inet_addr(szIP);
		ia.S_un.S_addr = dwIP;
		pSendBuf[9]=ia.S_un.S_un_b.s_b1;
		pSendBuf[10]=ia.S_un.S_un_b.s_b2;
		pSendBuf[11]=ia.S_un.S_un_b.s_b3;
		pSendBuf[12]=ia.S_un.S_un_b.s_b4;
		////////////////////////////////////////////////////////////////////
		szIP = W2A(strnewgateway);
		dwIP = inet_addr(szIP);
		ia.S_un.S_addr = dwIP;
		pSendBuf[13]=ia.S_un.S_un_b.s_b1;
		pSendBuf[14]=ia.S_un.S_un_b.s_b2;
		pSendBuf[15]=ia.S_un.S_un_b.s_b3;
		pSendBuf[16]=ia.S_un.S_un_b.s_b4;

		FD_ZERO(&fdSocket);	
		FD_SET(h_scan_Broad, &fdSocket);
// 		fd_set fdSocket;
// 		FD_ZERO(&fdSocket);	
// 		FD_SET(h_scan_Broad, &fdSocket);

		nRet = ::sendto(h_scan_Broad,(char*)pSendBuf,nSendLen,0,(sockaddr*)&h_bcast,sizeof(h_bcast));
		if (nRet == SOCKET_ERROR)
		{
			int  nError = WSAGetLastError();
			ret= FALSE ;
			goto END_CHANGEIP_SCAN;
			
		}
		int nLen = sizeof(h_siBind);
		//while(pScanner->IsComScanRunning())
	//	Sleep(3000);
		fd_set fdRead = fdSocket;
		int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);
		if (nSelRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			ret= FALSE ;
			goto END_CHANGEIP_SCAN;
			
		}

		if(nSelRet > 0)
		{
			ZeroMemory(buffer, 512);
			//Sleep(3000);
			do 
			{

			int nRet = ::recvfrom(h_scan_Broad,(char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);
			//			int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
			BYTE szIPAddr[4] = {0};
			if(nRet > 0)
			{		
				FD_ZERO(&fdSocket);
				if(buffer[0]==0x67)//收到正确的回复了
				{	
				
				SaveNewIPAddress(strnewipadress,stroldipaddress);
				ret=TRUE;
				MessageBox(_T("Successfull"));
					//Sleep(8000);

				//	SetCommunicationType(1);
// 					if (!Open_Socket2(strnewipadress,_wtoi(m_net_product_node.BuildingInfo.strIpPort)))
// 					{
// 						stroldipaddress=strnewipadress;
// 						MessageBox(_T("Fail!"));
// 						 ret= FALSE;
// 					}
// 					else
// 					{
// 						close_com();
// 					}

					 
					//m_flexGrid.put_TextMatrix(row_flags,NEW_IPADRESS,strnewipadress);
					
					//AfxMessageBox(_T("Change the ip successfully!"));
					//ret=TRUE;
					
					//return TRUE;
				break;
				}

				SHOW_TX_RX_TS

				FD_ZERO(&fdSocket);	
				FD_SET(h_scan_Broad, &fdSocket);
				nLen = sizeof(h_scan_siBind);
				fdRead = fdSocket;
				nSelRet = ::select(0, &fdRead, NULL, NULL, &time);

			}
			
			} while (nSelRet>0);

		}	
		else
		{
			MessageBox(_T("Fail!"));
			 ret=FALSE;
		}
END_CHANGEIP_SCAN:
		closesocket(h_scan_Broad);
		h_scan_Broad=NULL;
	//{

	//	//SOCKET soAck =::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	//	h_scan_Broad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	//	BOOL bBroadcast=TRUE;
	//	::setsockopt(h_scan_Broad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	//	int iMode=1;
	//	ioctlsocket(h_scan_Broad,FIONBIO, (u_long FAR*) &iMode);

	//	BOOL bDontLinger = FALSE;
	//	setsockopt( h_scan_Broad, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( BOOL ) );

	//	//SOCKADDR_IN bcast;
	//	h_bcast.sin_family=AF_INET;
	//	//bcast.sin_addr.s_addr=nBroadCastIP;
	//	h_bcast.sin_addr.s_addr=INADDR_BROADCAST;
	//	h_bcast.sin_port=htons(UDP_BROADCAST_PORT);

	//	//SOCKADDR_IN siBind;
	//	h_siBind.sin_family=AF_INET;
	//	h_siBind.sin_addr.s_addr=INADDR_ANY;
	//	h_siBind.sin_port=htons(RECV_RESPONSE_PORT);
	//	::bind(h_scan_Broad, (sockaddr*)&h_siBind,sizeof(h_siBind));

	//}
	return ret; 
}	

void CTroubleShootDlg::SaveNewIPAddress(CString newip,CString oldip){
	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 

	 
	try 
	{
		CString strSql;
		////////////////////////////////////////////////////////////////////////////////////////////
		//获取数据库名称及路径
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//连接数据库
	    
		strSql.Format(_T("select * from ALL_NODE where Bautrate='%s' "),oldip);
		//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
		bado.m_pRecordset=bado.OpenRecordset(strSql);
		while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
		{
			strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Bautrate='%s'"),newip,oldip);
			bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
			bado.m_pRecordset->MoveNext();
		}
		bado.CloseRecordset();
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
	}
	catch(_com_error e)
	{
		/* AfxMessageBox(e.Description());*/
		//MessageBox(m_name_new+_T("  has been here\n Please change another name!"));


		//m_pCon->Close();
		bado.CloseConn();
	}
	//m_pCon->Close(); 
	bado.CloseConn();
}
void CTroubleShootDlg::OnBnClickedOk()
{
   
is_ok=    ChangeNetDeviceIP(m_net_product_node.BuildingInfo.strIp);
if (is_ok)
{
CDialog::OnOK();
}
 
}
/*void CTroubleShootDlg::FreshUI(){*/
	//CString StrTemp;
	//m_current_product.Format(_T("%s(%s:%s) SN=%d \nThe device is connected to the ip(%s) of the network card.\n there are in different subnet ,so T3000 can't connect to your device. "),GetProductName(m_net_product_node.product_class_id),m_net_product_node.BuildingInfo.strIp,m_net_product_node.BuildingInfo.strIpPort,m_net_product_node.serial_number,m_net_product_node.NetworkCard_Address);
//	m_connection_status=_T("Do you want to fix the ip of your device?\n If Yes ,We will try to change the ip of your device .\n If No ,We will do nothing.");
//	UpdateData(FALSE);

/*}*/


void CTroubleShootDlg::SetNode(tree_product product_Node){
	m_net_product_node=product_Node;
}



void CTroubleShootDlg::OnPaint()
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
	SolidBrush *CharacterBlackBrush;
	
	Graphics *mygraphics;
	mygraphics = new Graphics(memDC.GetDC());
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	BlackBrush =new  SolidBrush(MY_COLOR_RED) ;
	CharacterBlackBrush = new SolidBrush(MY_COLOR_BLACK_CHARACTER);


	mygraphics->FillRectangle(BlackBrush,0,0,rcClient.Width(),40);

	FontFamily  CharacterfontFamily(_T("Arial"));
	PointF     TitlepointF(0, 0);
	SolidBrush  TitleCharacterColor(Color(255,255,255,255));
	Gdiplus::Font  Scroll_font(&CharacterfontFamily, 28, FontStyleBold, UnitPixel);
	TitlepointF.X = 150;
	TitlepointF.Y = 5;
	mygraphics->DrawString(_T("Different Subnets Detected!"), -1, &Scroll_font, TitlepointF,&TitleCharacterColor);

	PointF      WarningMessagePoint(0, 0);
	WarningMessagePoint.X = 25;
	WarningMessagePoint.Y = 45;
	SolidBrush  WarningMessageColor(Color(255,0,0,0));
	Gdiplus::Font  WarningMessageFont(&CharacterfontFamily, 24, FontStyleRegular, UnitPixel);
	mygraphics->DrawString(_T("The subnet of your PC and the device must match."), -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);
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
	StrTempOrgDeviceIP.Format(_T("Device IP address:  %s"),m_net_product_node.BuildingInfo.strIp);
	WarningMessagePoint.X = BLACK_GROUND_START_X + 5;
	WarningMessagePoint.Y = BLACK_GROUND_START_Y + 5;
	mygraphics->DrawString(StrTempOrgDeviceIP, -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);

	mygraphics->FillRectangle(CharacterBlackBrush,BLACK_PC_GROUND_START_X,BLACK_PC_GROUND_START_Y,BLACK_PC_GROUND_WIDTH,BLACK_PC_GROUND_HEIGHT);
	CString StrTempPCIP;
	StrTempPCIP.Format(_T("PC's IP address:     %s"),m_net_product_node.NetworkCard_Address);
	WarningMessagePoint.X = BLACK_PC_GROUND_START_X + 5;
	WarningMessagePoint.Y = BLACK_PC_GROUND_START_Y + 5;
	mygraphics->DrawString(StrTempPCIP, -1, &WarningMessageFont, WarningMessagePoint,&WarningMessageColor);
	//Bitmap bitmap(hBitmap_login,NULL);
	//graphics.DrawImage(&bitmap,0 ,0,test_rect.Width(),test_rect.Height());
}
