// WifiConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WifiConfigDlg.h"
#include "afxdialogex.h"
#include "global_function.h"


// CWifiConfigDlg dialog

IMPLEMENT_DYNAMIC(CWifiConfigDlg, CDialogEx)

CWifiConfigDlg::CWifiConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WIFI_CONFIG, pParent)
{

}

CWifiConfigDlg::~CWifiConfigDlg()
{
}

void CWifiConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAC_ADDRESS, m_Edit_MacAddress);
	DDX_Control(pDX, IDC_IPADDRESS2, m_ipaddress);
	DDX_Control(pDX, IDC_IPADDRESS1, m_Subnet_Mask);
	DDX_Control(pDX, IDC_IPADDRESS3, m_GatewayAddress);
	DDX_Control(pDX, IDC_EDIT_LISTENPORT, m_ListenPort);
	DDX_Control(pDX, IDC_COMBO_NETWORK_TYPE, m_combox_NetType);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_SSID);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Key);
}


BEGIN_MESSAGE_MAP(CWifiConfigDlg, CDialogEx)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_NETWORK_TYPE, &CWifiConfigDlg::OnCbnSelchangeComboNetworkType)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CWifiConfigDlg::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CWifiConfigDlg::OnEnKillfocusEdit2)
	//ON_WM_NCPAINT()
	ON_BN_CLICKED(IDOK, &CWifiConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWifiConfigDlg message handlers


void CWifiConfigDlg::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
//					   
//					   // Do not call CDialogEx::OnPaint() for painting messages
//
//	CRect   rect;
////	CPaintDC   dc(this);
//	GetClientRect(rect);
//	dc.FillSolidRect(rect, RGB(51, 153, 255));   //设置为绿色背景

}


BOOL CWifiConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strtemp;
	int CO2_NET_MODBUS_MAC_ADDRESS_START = 40;
	strtemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START], product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START + 1], product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START + 2], product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START + 3], product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START + 4], product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START + 5]);
	m_Edit_MacAddress.SetWindowText(strtemp);
	 
	

	int   CO2_NET_MODBUS_IP_ADDRESS_START = 47;
	m_ipaddress.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START],
		(BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START + 1],
		(BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START + 2],
		(BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START + 3]);

	m_address[0] = (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START];
	m_address[1] = (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START + 1];
	m_address[2] = (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START + 2];
	m_address[3] = (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START + 3];

	int   CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START = 51;
	m_ipaddress.ShowWindow(TRUE);
	m_Subnet_Mask.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START],
		(BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START + 1],
		(BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START + 2],
		(BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START + 3]);
	int   CO2_NET_MODBUS_GATEWAY_ADDRESS_START = 55;
	m_Subnet_Mask.ShowWindow(TRUE);

	m_GatewayAddress.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START],
		(BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START + 1],
		(BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START + 2],
		(BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START + 3]);
	int CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START = 60;
	m_GatewayAddress.ShowWindow(TRUE);
	strtemp.Format(_T("%d"), product_register_value[CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START]);
	m_ListenPort.SetWindowText(strtemp);
	m_ListenPort.ShowWindow(TRUE);

	int nettype = product_register_value[59];
	if (nettype == 0)
	{
		m_combox_NetType.SetCurSel(0);
	} 
	else
	{
		m_combox_NetType.SetCurSel(1);
	}

	unsigned short SSIDBuffer[16];
	memset(SSIDBuffer, 0, 16);
	Read_Multi(g_tstat_id, SSIDBuffer, 782,16);
	CString strSSID = Exchange2String(SSIDBuffer, 16);
	m_Edit_SSID.SetWindowTextW(strSSID);
	memset(SSIDBuffer, 0, 16);
	Read_Multi(g_tstat_id, SSIDBuffer, 798, 16);
	strSSID = Exchange2String(SSIDBuffer, 16);
	m_Edit_Key.SetWindowTextW(strSSID);
	



	return TRUE;  
}


BOOL CWifiConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			//CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			////GetDlgItem(IDC_REFRESHBUTTON)->SetFocus();
			//temp_focus->OnKillFocus();
			CWnd *temp_focus = GetFocus();
			//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_EDIT_TEST)->SetFocus();
			temp_focus->SetFocus();

			return 1;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CWifiConfigDlg::OnCbnSelchangeComboNetworkType()
{
    //59
	/*int nettype = product_register_value[59];
	if (nettype == 0)
	{
		m_combox_NetType.SetCurSel(0);
	}
	else
	{
		m_combox_NetType.SetCurSel(1);
	}*/
	int nettype = m_combox_NetType.GetCurSel();
	int ret = write_one(g_tstat_id, 59, nettype);


}
#define THE_CHAR_LENGTH 8
CString CWifiConfigDlg::Exchange2String(unsigned short *Buffer, int reglength)
{
	
	int stringlength = 2 * reglength;
	unsigned char *str = new unsigned char[2 * reglength + 1];
	  


	if (Buffer[0] == 0 || Buffer[0] == 65535)
	{
		return L"";
	}
	unsigned short Hi_Char, Low_Char;

	for (int i = 0; i < reglength; i++)
	{
		Hi_Char = Buffer[i];
		Hi_Char = Hi_Char & 0xff00;
		Hi_Char = Hi_Char >> 8;
		Low_Char = Buffer[i];
		Low_Char = Low_Char & 0x00ff;
		str[2 * i] = Hi_Char;
		str[2 * i + 1] = Low_Char;
	}

	CString str_temp(str);
	delete[] str;
	str = NULL;
	return str_temp;
}

void CWifiConfigDlg::OnEnKillfocusEdit1()
{
	CString str;
	m_Edit_SSID.GetWindowTextW(str);
	if (str.GetLength() <= 0)
	{
		return;
	}
	unsigned char AESPwd[16];
	memset(AESPwd, 0, 16);
	for (int i = 0; i < str.GetLength(); i++)
	{
		AESPwd[i] = str.GetAt(i);
	}
	int ret = Write_Multi_org(g_tstat_id, AESPwd, 782, 16, 5);
 
}


void CWifiConfigDlg::OnEnKillfocusEdit2()
{
	CString str;
	m_Edit_Key.GetWindowTextW(str);
	if (str.GetLength() <= 0)
	{
		return;
	}
	unsigned char AESPwd[16];
	memset(AESPwd, 0, 16);
	for (int i = 0; i < str.GetLength(); i++)
	{
		AESPwd[i] = str.GetAt(i);
	}
	int ret = Write_Multi_org(g_tstat_id, AESPwd, 798, 16, 5);
}


void CWifiConfigDlg::OnBnClickedOk()
{
	int ret[4];
	BYTE address1, address2, address3, address4;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS2))->GetAddress(address1, address2, address3, address4);

	//dufan : 当IP地址 有变化时才写寄存器
	if ((m_address[0] != ret[0]) ||
		(m_address[1] != ret[1]) ||
		(m_address[2] != ret[2]) ||
		(m_address[3] != ret[3]))
	{
		ret[0] = write_one(g_tstat_id, 47, address1);
		ret[3] = write_one(g_tstat_id, 50, address4);
		ret[1] = write_one(g_tstat_id, 48, address2);
		ret[2] = write_one(g_tstat_id, 49, address3);

		if ((ret[0] && ret[1] && ret[2] && ret[3]) == false)
		{
			MessageBox(_T("Warning!"), _T("Change IP Address Failed!"), MB_OK | MB_ICONINFORMATION);
			return;
		}
	}



	CDialogEx::OnOK();
}



  
