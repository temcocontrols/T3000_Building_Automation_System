#pragma once


// CConnectRemoteServer dialog

class CConnectRemoteServer : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectRemoteServer)

public:
	CConnectRemoteServer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectRemoteServer();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONNECT_NEWFIRMWARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClose();
};
DWORD WINAPI  TcpClient_Connect_Thread(LPVOID lpVoid);
DWORD WINAPI UDP_ptp_Thread(LPVOID lpVoid);
