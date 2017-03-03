#pragma once
#include <vector>
#include "afxwin.h"
using namespace std;

// CConnectionSetup dialog

class CConnectionSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectionSetup)

public:
	CConnectionSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectionSetup();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_CONNECTION_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//Comment Function
public:
	void OnInitUI();
	void Fresh_UI();
	void Read_Config();
	void Write_Config();
public:
	vector<CString> m_szComm;
	CComboBox m_combox_bradrate;
	CComboBox m_combox_connection_type;
	CComboBox  m_combox_comport;
	CIPAddressCtrl m_combox_ipaddress;
	CEdit m_edit_between_time;
	CEdit m_edit_connect_timeout;
	CEdit m_edit_port;
	CEdit m_edit_response_timeout;

	int m_bradrate;
	int m_Commication_Type;
	CString m_comport;
	CString m_ipaddress;
	int m_between_time;
	int m_connect_timeout;
	int m_ipport;
	int m_response_timeout;
 
	 
public:
	afx_msg void OnCbnSelchangeComboboxConnectionType();
	afx_msg void OnCbnSelchangeComboboxComport();
	afx_msg void OnBnClickedOk();
 
	CComboBox m_combox_databits;
	CComboBox m_combox_parity;
	CComboBox m_combox_stopbits;
};
