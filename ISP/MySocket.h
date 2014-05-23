//#if !defined(AFX_AKINGSOCKET_H__35E1C928_7A72_4EE2_AE47_1BCC69EAE67A__INCLUDED_)
//#define AFX_AKINGSOCKET_H__35E1C928_7A72_4EE2_AE47_1BCC69EAE67A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//FanceSocket.h : header file
//
#include <WINSOCK2.H>
#include <afxsock.h>


const int ISP_SEND_FLASH_COMMAND = 0;
const int ISP_SEND_DHCP_COMMAND_NO_LANIP = 1;
const int ISP_SEND_DHCP_COMMAND_HAS_LANIP = 2;
const int ISP_Send_TFTP_PAKAGE = 3;
const int ISP_Send_TFTP_OVER = 4;
const int ISP_Flash_Done = 5;

/////////////////////////////////////////////////////////////////////////////
// CFanceSocket command target

class MySocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	MySocket();
	virtual ~MySocket();

// Overrides
public:
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	CString	m_hex_bin_filepath;
protected:
public:
	virtual void OnConnect(int nErrorCode);
private:
	char receive_buf[4096];
	int Receive_data_length;

};


//#endif 
