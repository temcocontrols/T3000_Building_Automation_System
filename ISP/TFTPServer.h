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
	// �ⲿ�ӿ�
	 void FlashByEthernet();
	//////////////////////////////////////////////////////////////////////////
	// TFTP functional flow functions
	// tftp �������̺���
	BOOL StartServer();
	//////////////////////////////////////////////////////////////////////////
	// Get local IP
	// ��ñ���IP
	DWORD GetLocalIP();
	//void ParseFile(const CString& strFileName);	
	
	int InitSocket();
	// Broadcast notification to NC
	// �㲥֪ͨNC
	void BroadCastToClient();

	BOOL RecvBOOTP();
	// Accept Request
	// ����Request
	int RecvRequest();	
	// Send data
	// ��������
	int SendData(BYTE* szBuf, int nLen);

	int SendDataNew(BYTE* szBuf, int nLen);
	// Accept ACK
	// ����ACK
	int RecvACK();
	// Send end identifier
	// ���ͽ�����ʶ

	// Handle request
	// ��������
	void HandleWRRequest(BYTE* szBuf,  int nLen);
	
	// Print information
	// ��ӡ��Ϣ
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace);

	// Flash completed
	// flash ��ɡ�
	void WriteFinish(int nFlashFlag);
    void NewBootWriteFinish(int nFlashFlag);
	// Send data flow
	// ������������
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
	BYTE*			m_szDataBuf;			// Ҫ���͵�����
	int					m_nDataBufLen;
	DWORD			m_dwClientIP;
	
	// Block number
	int					m_nBlkNum;			// ���
	
	CString			m_strFileName;
   

	// Receive TFTP request socket, bound to port 69
	SOCKET			m_sock;					// ����tftp request socket, ����69�˿�
	// Send socket, for sending TFTP data
	SOCKET			m_soSend;				// ����socket������tftp���ݵ�
	// Receive socket, for receiving TFTP ack
	SOCKET			m_soRecv;				// ����socket������tftp ack��
	// Send sockaddr_in, not bound to port
	sockaddr_in		m_siSend;				// ����si�����󶨶˿�
	// Receive TFTP ack
	sockaddr_in		m_siClient;				// ����tftp ack
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
    int m_tcp_connect_results; // ���Flash֮��  ��ʶTCP �׽����Ƿ�����
	int m_FlashTimes;
	unsigned short read_reg[100] ;
	// Continue burning from which package; currently only supports products with product ID 88 ESP32 chip boards
	int continue_flash_count; // �����Ӷ��ٰ���ʼ����; Ŀǰֻ֧�ֲ�ƷΪ88 �� ESP32 оƬ�İ���;
};
