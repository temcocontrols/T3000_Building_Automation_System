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
void Signal_Tftp_Ack(void);

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
	// External interface.
	 void FlashByEthernet();
	//////////////////////////////////////////////////////////////////////////
	// TFTP functional flow functions
	// TFTP flow function.
	BOOL StartServer();
	//////////////////////////////////////////////////////////////////////////
	// Get local IP
	// Get the local IP.
	DWORD GetLocalIP();
	//void ParseFile(const CString& strFileName);

	int InitSocket();
	// Broadcast notification to NC
	// Broadcast a notice to the NC.
	void BroadCastToClient();

	BOOL RecvBOOTP();
	// Accept Request
	// Receive the request.
	int RecvRequest();
	// Send data
	// Send data.
	int SendData(BYTE* szBuf, int nLen);

	int SendDataNew(BYTE* szBuf, int nLen);
	// Accept ACK
	// Receive the ACK.
	int RecvACK();
	// Send end identifier
	// Send the end marker.

	// Handle request
	// Handle the request.
	void HandleWRRequest(BYTE* szBuf,  int nLen);

	// Print information
	// Print a message.
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace);

	// Flash completed
	// Flash finished.
	void WriteFinish(int nFlashFlag);
    void NewBootWriteFinish(int nFlashFlag);
	// Send data flow
	// Data-send sequence.
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
	BYTE*			m_szDataBuf;			// Data to send.
	int					m_nDataBufLen;
	DWORD			m_dwClientIP;

	// Block number
	int					m_nBlkNum;			// Block number.

	CString			m_strFileName;


	// Receive TFTP request socket, bound to port 69
	SOCKET			m_sock;					// Socket that receives the TFTP request, bound to port 69.
	// Send socket, for sending TFTP data
	SOCKET			m_soSend;				// Socket that sends TFTP data.
	// Receive socket, for receiving TFTP ack
	SOCKET			m_soRecv;				// Socket that receives the TFTP ACK.
	// Send sockaddr_in, not bound to port
	sockaddr_in		m_siSend;				// Send SI without binding a port.
	// Receive TFTP ack
	sockaddr_in		m_siClient;				// Receive the TFTP ACK.
	sockaddr_in		m_siServer;				// client info

	MySocket TCP_Flash_CMD_Socket;
	MySocket SendUDP_Flash_Socket;
	MySocket UDP_Broadcast_Socket;
	TFTP_WRRequest	m_twr;
	CISPDlg*		m_pDlg;

	CWinThread*	m_pThread;

public:

    CString m_StrProductName;
    CString m_StrRev;
    CString m_StrFileProductName;
    void Set_FileProductName(CString Name);
	int total_retry;
    // After clicking Flash, indicates whether TCP socket is connected
    int m_tcp_connect_results; // After Flash is clicked, marks whether the TCP socket is connected.
	int m_FlashTimes;
	unsigned short read_reg[100] ;
	// Continue burning from which package; currently only supports products with product ID 88 ESP32 chip boards
	int continue_flash_count; // Packet number to resume flashing from. Currently only product 88, the ESP32 board.
};
