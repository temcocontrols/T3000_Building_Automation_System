#pragma once
#include "afxwin.h"


// CConnection dialog

class CConnection : public CDialogEx
{
	DECLARE_DYNAMIC(CConnection)

public:
	CConnection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnection();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONNECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_connection_port;
	CComboBox m_connection_baudrate;
	afx_msg void OnCbnSelchangeConnectionPort();
	int m_connection_port_value;
	int m_connection_baudrate_value;


};
