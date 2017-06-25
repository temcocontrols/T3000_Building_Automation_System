#pragma once

#include "Class/MulitithreadSocket.h"
#include "CM5/ListCtrlEx.h"
// CTCP_Server dialog
#include "global_function.h"
#include "afxcmn.h"
class CTCP_Server : public CDialogEx
{
	DECLARE_DYNAMIC(CTCP_Server)

public:
	CTCP_Server(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTCP_Server();

// Dialog Data
	enum { IDD = IDD_GSM_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartServer();
	afx_msg void OnBnClickedButtonStopServer();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void Initial_List();
	void Show_List_Data(Client_Info *temp_client);
	afx_msg LRESULT RefreshList(WPARAM wParam,LPARAM lParam);
		BOOL m_bServer;
	CMulitithreadSocket m_wskServer;
	ListCtrlEx::CListCtrlEx m_tcp_server_list;
};

const int TCP_ITEM = 0;
const int TCP_IP_ADDRESS = 1;
const int TCP_IP_PORT = 2;
const int TCP_PRODUCT_NAME = 3;
const int TCP_SERIALNUMBER = 4;

const int ADD_ITEM = 1;
const int DELETE_ITEM = 2;

