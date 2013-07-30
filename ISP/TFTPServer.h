#pragma once

#include "TFTP Struct.h"
//#include "ISPDlg.h"
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
	// �ⲿ�ӿ�
	void FlashByEthernet();
	//////////////////////////////////////////////////////////////////////////
	// tftp �������̺���
	BOOL StartServer();
	//////////////////////////////////////////////////////////////////////////
	// ��ñ���IP
	DWORD GetLocalIP();
	//void ParseFile(const CString& strFileName);	

	int InitSocket();
	// �㲥֪ͨNC
	void BroadCastToClient();

	BOOL RecvBOOTP();
	// ����Request
	int RecvRequest();	
	// ��������
	int SendData(BYTE* szBuf, int nLen);

	int SendDataNew(BYTE* szBuf, int nLen);
	// ����ACK
	int RecvACK();
	// ���ͽ�����ʶ

	// ��������
	void HandleWRRequest(BYTE* szBuf,  int nLen);

	// ��ӡ��Ϣ
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace);

	// flash ��ɡ�
	void WriteFinish(int nFlashFlag);

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
protected:
	BYTE*			m_szDataBuf;			// Ҫ���͵�����
	int					m_nDataBufLen;
	DWORD			m_dwClientIP;
	int					m_nClientPort;
	int					m_nBlkNum;			// ���

	CString			m_strFileName;


	SOCKET			m_sock;					// ����tftp request socket, ����69�˿�
	SOCKET			m_soSend;				// ����socket������tftp���ݵ�
	SOCKET			m_soRecv;				// ����socket������tftp ack��
	sockaddr_in		m_siSend;				// ����si�����󶨶˿�
	sockaddr_in		m_siClient;				// ����tftp ack
	sockaddr_in		m_siServer;				// client info

	MySocket TCP_Flash_CMD_Socket;
	MySocket SendUDP_Flash_Socket;
	MySocket UDP_Broadcast_Socket;
	TFTP_WRRequest	m_twr;	
	CISPDlg*			m_pDlg;

	CWinThread*	m_pThread;

public:
	CString ISP_Device_IP;
	int total_retry;
};
