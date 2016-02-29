#pragma once
#include "afxwin.h"
#include "MySocket.h"

// CBootFinderDlg dialog
const int  FLASH_UDP_PORT=10000; 
const int  LOCAL_UDP_PORT=10001;

#define WM_BOOTMESSAGE WM_USER  +   10000

class CBootFinderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBootFinderDlg)

public:
	CBootFinderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBootFinderDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BOOTFINDER};

protected:                        
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CListBox m_message_list;
    afx_msg void OnBnClickedOk();
    void SetDHCP_Data();
    bool IP_is_Local();
    DWORD GetLocalIP();
    MySocket SendUDP_Flash_Socket;
    DWORD			m_dwClientIP;
    CString ISP_Device_IP;
    CString m_StrProductName;
    CString m_StrRev;
    HANDLE hFirstThread;
    virtual BOOL OnInitDialog();
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    afx_msg void OnDestroy();
    long m_line;
};
