#pragma once

#include "TFTP Struct.h"
 
#include "MySocket.h"



typedef struct _DHCP_PACKET
{
	char Header[13];  // Temcocontrols
	DWORD AssignIp;   //
	DWORD SubnetMask;  //
	DWORD GatewayIp;  //
	DWORD LanIp;
	char Reserved[16]; //
} DHCP_PACKET;

#define DHCP_PACKET_SIZE (sizeof(DHCP_PACKET))
const int  FLASH_UDP_PORT=10000; 
const int  LOCAL_UDP_PORT=10001;

class CISPDlg;
class TFTPServer
{
public:
	TFTPServer(void);
	virtual ~TFTPServer(void);

	//////////////////////////////////////////////////////////////////////////
	// Server
	void SetClientIP(DWORD dwIP);
	void SetClientPort(int nPort);

	void SetFileName(const CString& strFileName);
	void SetDataSource(BYTE* pBuf, int nLen);
    
	//////////////////////////////////////////////////////////////////////////
	// External interface
	// 外部接口
	 void FlashByEthernet();
	//////////////////////////////////////////////////////////////////////////
	// TFTP functional flow functions
	// tftp 功能流程函数
	BOOL StartServer();
	//////////////////////////////////////////////////////////////////////////
	// Get local IP
	// 获得本地IP
	DWORD GetLocalIP();
	//void ParseFile(const CString& strFileName);	
	
	int InitSocket();
	// Broadcast notification to NC
	// 广播通知NC
	void BroadCastToClient();

	BOOL RecvBOOTP();
	// Accept Request
	// 接受Request
	int RecvRequest();	
	// Send data
	// 发送数据
	int SendData(BYTE* szBuf, int nLen);

	int SendDataNew(BYTE* szBuf, int nLen);
	// Accept ACK
	// 接受ACK
	int RecvACK();
	// Send end identifier
	// 发送结束标识

	// Handle request
	// 处理请求
	void HandleWRRequest(BYTE* szBuf,  int nLen);
	
	// Print information
	// 打印信息
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace);

	// Flash completed
	// flash 完成。
	void WriteFinish(int nFlashFlag);
    void NewBootWriteFinish(int nFlashFlag);
	// Send data flow
	// 发送数据流程
	int SendProcess();

	int SendEndDataPack();
	
	void SendBOOTPPack();
	int GetDHCPData(BYTE* pBuf, int nLen);
	BOOL SendDHCPPack();


	int SendAnyTFTPData();
	int SendFlashCommand();
	void SetParentDlg(CDialog* pDlg);

	void ReleaseAll();

	void GetDeviceIP_String();
	void SetDHCP_Data();
	bool IP_is_Local();
	bool Send_Tftp_File();
	BOOL StartServer_Old_Protocol();
    void  TFTPMessage(LPTSTR sMessage);

	void GetIPMaskGetWay();
	UINT RefreshNetWorkDeviceListByUDPFunc();
	unsigned short AddNetDeviceForRefreshList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind);
	int					m_nClientPort;
protected:
	// Data to send
	BYTE*			m_szDataBuf;			// 要发送到数据
	int					m_nDataBufLen;
	DWORD			m_dwClientIP;
	
	// Block number
	int					m_nBlkNum;			// 块号
	
	CString			m_strFileName;
   

	// Receive TFTP request socket, bound to port 69
	SOCKET			m_sock;					// 接收tftp request socket, 绑定了69端口
	// Send socket, for sending TFTP data
	SOCKET			m_soSend;				// 发送socket，发送tftp数据的
	// Receive socket, for receiving TFTP ack
	SOCKET			m_soRecv;				// 接收socket，接收tftp ack的
	// Send sockaddr_in, not bound to port
	sockaddr_in		m_siSend;				// 发送si，不绑定端口
	// Receive TFTP ack
	sockaddr_in		m_siClient;				// 接收tftp ack
	sockaddr_in		m_siServer;				// client info

	MySocket TCP_Flash_CMD_Socket;
	MySocket SendUDP_Flash_Socket;
	MySocket UDP_Broadcast_Socket;
	TFTP_WRRequest	m_twr;	
	CISPDlg*			m_pDlg;

	CWinThread*	m_pThread;

public:
	
    CString m_StrProductName;
    CString m_StrRev;
    CString m_StrFileProductName;
    void Set_FileProductName(CString Name);
	int total_retry;
    // After clicking Flash, indicates whether TCP socket is connected
    int m_tcp_connect_results; // 点击Flash之后  标识TCP 套接字是否连上
	int m_FlashTimes;
	unsigned short read_reg[100] ;
	// Continue burning from which package; currently only supports products with product ID 88 ESP32 chip boards
	int continue_flash_count; // 继续从多少包开始烧入; 目前只支持产品为88 的 ESP32 芯片的板子;
};
