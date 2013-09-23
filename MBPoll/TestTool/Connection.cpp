// Connection.cpp : implementation file
//

#include "stdafx.h"
#include "TestTool.h"
#include "Connection.h"
#include "afxdialogex.h"

#include "global_variable_extern.h"
#include "globle_function.h"
#include "ModbusDllforVc.h"
// CConnection dialog

IMPLEMENT_DYNAMIC(CConnection, CDialogEx)

CConnection::CConnection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConnection::IDD, pParent)
	, m_connection_port_value(0)
	, m_connection_baudrate_value(0)
{

}

CConnection::~CConnection()
{
}

void CConnection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONNECTION_PORT, m_connection_port);
	DDX_Control(pDX, IDC_CONNECTION_BAUDRATE, m_connection_baudrate);
	DDX_CBIndex(pDX, IDC_CONNECTION_PORT, m_connection_port_value);
	DDX_CBIndex(pDX, IDC_CONNECTION_BAUDRATE, m_connection_baudrate_value);
}


BEGIN_MESSAGE_MAP(CConnection, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConnection::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CONNECTION_PORT, &CConnection::OnCbnSelchangeConnectionPort)
END_MESSAGE_MAP()


// CConnection message handlers


void CConnection::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strInfo;
	UpdateData(TRUE);
	CString temp_cs,temp_ipport,temp_ipaddress,temp_port_des;
	CString temp_baudrate;
	CString temp_communication_type;
	m_connection_port.GetLBText(m_connection_port_value,temp_cs);
	temp_port_des = temp_cs;
	if(temp_cs.CompareNoCase(_T("TCP/IP"))==0)
	{
		GetDlgItemText(IDC_CONNECTION_IP,temp_ipaddress);
		GetDlgItemText(IDC_CONNECTION_TCP_PORT,temp_ipport);
		if((temp_ipaddress.IsEmpty()==1)||(temp_ipport.IsEmpty()==1))
		{
			MessageBox(_T("The IP address and port can't be empty!"));
			return;
		}
		g_IPAddress=temp_ipaddress;
		g_IPPort = _wtoi(temp_ipport);
		SetCommunicationType(1);
		g_CommunicationType=1;
		BOOL ret=Open_Socket2(g_IPAddress,g_IPPort);

		if(ret)
		{	
			strInfo.Format((_T("Connect to IP : %s successful")), g_IPAddress);//prompt info;
			g_bIs_connected = true;
		}
		else
		{
			strInfo.Format((_T("Connect to IP : %s failure")), g_IPAddress);//prompt info;
			g_bIs_connected = false;
			MessageBox(strInfo);
		}
		

	}
	else
	{
		
		m_connection_port.GetLBText(m_connection_port_value,temp_cs);
		temp_cs=temp_cs.Right(temp_cs.GetLength()-3);
		g_PortNum=_wtoi(temp_cs);

		
		m_connection_baudrate.GetLBText(m_connection_baudrate.GetCurSel(),temp_baudrate);
		m_nbaudrat = _wtoi(temp_baudrate);
		SetCommunicationType(0);

		g_CommunicationType=0;

		open_com(g_PortNum);//open*************************************
		if(!is_connect())
		{
			g_bIs_connected = false;
			strInfo.Format(_T("COM %d : Not available "), g_PortNum);
			AfxMessageBox(strInfo);
		}
		else
		{
			g_bIs_connected = true;
			strInfo.Format(_T("COM %d Connected: Yes"), g_PortNum);
			Change_BaudRate(m_nbaudrat);
		}	
	}

	temp_ipport.Format(_T("%d"),g_IPPort);
	temp_baudrate.Format(_T("%d"),m_nbaudrat);
	temp_communication_type.Format(_T("%d"),g_CommunicationType);

	g_PortDescribe = temp_port_des;
	WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),g_IPAddress,Program_ConfigFile_Path);

	WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),temp_ipport,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("Setting"),_T("Port"),temp_port_des,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),temp_baudrate,Program_ConfigFile_Path);
	WritePrivateProfileStringW(_T("Setting"),_T("Communication Type"),temp_communication_type,Program_ConfigFile_Path);

	wchar_t *mychar = new wchar_t[250];
	_tcscpy_s(mychar,250,strInfo);
	//wstrcpy_s(temp_1,250,)
	::PostMessage(main_hwnd,MY_SHOW_MSG,NULL,(LPARAM)mychar);

	CDialogEx::OnOK();
}


BOOL CConnection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	for (int i=0;i<(int)m_szComm.size();i++)
	{
		m_connection_port.AddString(m_szComm.at(i));
	}
	m_connection_port.AddString(_T("TCP/IP"));
	

	GetDlgItem(IDC_CONNECTION_PORT)->SetFocus();
	int temp_index=m_connection_port.FindString(0,g_PortDescribe.GetString());
	if(temp_index>=0)
	{
		m_connection_port.SetCurSel(temp_index);
	}
	else
	{
		m_connection_baudrate.SetCurSel(0);
	}

	SetDlgItemTextW(IDC_CONNECTION_IP,g_IPAddress);
	CString temp_cs;
	temp_cs.Format(_T("%d"),g_IPPort);
	SetDlgItemTextW(IDC_CONNECTION_TCP_PORT,temp_cs);


	if(g_CommunicationType ==0)
	{
		m_connection_baudrate.EnableWindow(TRUE);
		GetDlgItem(IDC_CONNECTION_IP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONNECTION_TCP_PORT)->EnableWindow(FALSE);
	}
	else
	{
		m_connection_baudrate.EnableWindow(FALSE);
		GetDlgItem(IDC_CONNECTION_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CONNECTION_TCP_PORT)->EnableWindow(TRUE);
	}


	

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConnection::OnCbnSelchangeConnectionPort()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int a=	m_connection_port_value;
	CString temp_cs;
	m_connection_port.GetLBText(a,temp_cs);
	if(temp_cs.CompareNoCase(_T("TCP/IP"))==0)
	{
		m_connection_baudrate.EnableWindow(FALSE);
		GetDlgItem(IDC_CONNECTION_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CONNECTION_TCP_PORT)->EnableWindow(TRUE);
	}
	else
	{
		m_connection_baudrate.EnableWindow(TRUE);
		GetDlgItem(IDC_CONNECTION_IP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONNECTION_TCP_PORT)->EnableWindow(FALSE);
	}
}
