// WifiConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WifiConfigDlg.h"
#include "afxdialogex.h"
#include "global_function.h"

str_wifi_point wifi_info;
// CWifiConfigDlg dialog

IMPLEMENT_DYNAMIC(CWifiConfigDlg, CDialogEx)

CWifiConfigDlg::CWifiConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WIFI_CONFIG, pParent)
{
    m_ui_type = UI_TYPE_WIFI_REV1;

    WIFI_ENABLE = 2000 ;
    IP_AUTO_MANUAL = 2001 ;
    IP_WIFI_STATUS = 2002 ;

    WIFI_LOAD_DEFAULT = 2003;
    WIFI_MODBUS_PORT = 2004;
    WIFI_BACNET_PORT = 2005;
    WIFI_MOUDLE_SOFTWARE_VERSION = 2006;
    SSID_NAME_START_REG = 2010;  // 32个
    PASSWORD_START_REG = 2042 ;   //16 个
    STATIC_IP_START_REG = 2058 ;  //12个
}

CWifiConfigDlg::~CWifiConfigDlg()
{
}

void CWifiConfigDlg::SetWifiRegStartAddress(int nstart_address)
{
    int offset = nstart_address - 2000;

    WIFI_ENABLE = 2000 + offset;
    IP_AUTO_MANUAL = 2001 + offset;
    IP_WIFI_STATUS = 2002 + offset;
    WIFI_LOAD_DEFAULT = 2003 + offset;
    WIFI_MODBUS_PORT = 2004 + offset;
    WIFI_BACNET_PORT = 2005 + offset;
    WIFI_MOUDLE_SOFTWARE_VERSION = 2006 + offset;
    SSID_NAME_START_REG = 2010 + offset;  // 32个
    PASSWORD_START_REG = 2042 + offset;   //16 个
    STATIC_IP_START_REG = 2058 + offset;  //12个

    m_version = 1;
}

void CWifiConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAC_ADDRESS, m_Edit_MacAddress);
	DDX_Control(pDX, IDC_IPADDRESS2, m_ipaddress);
	DDX_Control(pDX, IDC_IPADDRESS1, m_Subnet_Mask);
	DDX_Control(pDX, IDC_IPADDRESS3, m_GatewayAddress);
	//DDX_Control(pDX, IDC_EDIT_LISTENPORT, m_ListenPort);
	DDX_Control(pDX, IDC_COMBO_NETWORK_TYPE, m_combox_NetType);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_SSID);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Key);
}


BEGIN_MESSAGE_MAP(CWifiConfigDlg, CDialogEx)

	ON_CBN_SELCHANGE(IDC_COMBO_NETWORK_TYPE, &CWifiConfigDlg::OnCbnSelchangeComboNetworkType)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CWifiConfigDlg::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CWifiConfigDlg::OnEnKillfocusEdit2)
	//ON_WM_NCPAINT()
	ON_BN_CLICKED(IDOK, &CWifiConfigDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_RADIO_IP_AUTO, &CWifiConfigDlg::OnBnClickedRadioIpAuto)
    ON_BN_CLICKED(IDC_RADIO_IP_STATIC, &CWifiConfigDlg::OnBnClickedRadioIpStatic)
    ON_BN_CLICKED(IDC_BUTTON_WIFI_DEFAULT, &CWifiConfigDlg::OnBnClickedButtonWifiDefault)
END_MESSAGE_MAP()


// CWifiConfigDlg message handlers
void CWifiConfigDlg::SetUI_Type(int ui_type)
{
    m_ui_type = ui_type;
}

void CWifiConfigDlg::ChangeWifiWindowStatus(bool b_value)
{
    GetDlgItem(IDC_RADIO_IP_AUTO)->EnableWindow(b_value);
    GetDlgItem(IDC_RADIO_IP_STATIC)->EnableWindow(b_value);
    GetDlgItem(IDC_EDIT1)->EnableWindow(b_value);
    GetDlgItem(IDC_EDIT2)->EnableWindow(b_value);
    GetDlgItem(IDOK)->EnableWindow(b_value);
    GetDlgItem(IDC_IPADDRESS1)->EnableWindow(b_value);
    GetDlgItem(IDC_IPADDRESS2)->EnableWindow(b_value);
    GetDlgItem(IDC_IPADDRESS3)->EnableWindow(b_value);
    GetDlgItem(IDC_RADIO_WIFI_ENABLE)->EnableWindow(b_value);
    GetDlgItem(IDC_RADIO_WIFI_DISABLE)->EnableWindow(b_value);
    GetDlgItem(IDC_EDIT_WIFI_MODBUS_PORT)->EnableWindow(b_value);
    
    //GetDlgItem(IDCANCEL)->EnableWindow(b_value);
}


BOOL CWifiConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    GetDlgItem(IDC_BUTTON_WIFI_DEFAULT)->EnableWindow(0);
    GetDlgItem(IDC_EDIT_WIFI_MODBUS_PORT)->EnableWindow(1);
    GetDlgItem(IDC_EDIT_WIFI_BACNET_PORT)->EnableWindow(1);
    if (m_ui_type == UI_TYPE_WIFI_REV1)
    {

        GetDlgItem(IDC_RADIO_IP_AUTO)->EnableWindow(false);
        GetDlgItem(IDC_RADIO_IP_STATIC)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_WIFI_MODBUS_PORT)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_WIFI_BACNET_PORT)->EnableWindow(false);
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
        //m_ListenPort.SetWindowText(strtemp);
        //m_ListenPort.ShowWindow(TRUE);

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
        Read_Multi(g_tstat_id, SSIDBuffer, 782, 16);
        CString strSSID = Exchange2String(SSIDBuffer, 16);
        m_Edit_SSID.SetWindowTextW(strSSID);
        memset(SSIDBuffer, 0, 16);
        Read_Multi(g_tstat_id, SSIDBuffer, 798, 16);
        strSSID = Exchange2String(SSIDBuffer, 16);
        m_Edit_Key.SetWindowTextW(strSSID);
    }
    else if (m_ui_type == UI_TYPE_WIFI_REV2)
    {
        GetDlgItem(IDC_RADIO_IP_AUTO)->EnableWindow(true);
        GetDlgItem(IDC_RADIO_IP_STATIC)->EnableWindow(true);

        

        m_combox_NetType.ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_NET_WORK_TYPE)->ShowWindow(SW_HIDE);

        int itemp = Read_Multi(g_tstat_id, wifi_register_value, WIFI_ENABLE, 100, 6);
        if (itemp < 0)
        {
            AfxMessageBox(_T("Read wifi information timeout"));
            PostMessage(WM_CLOSE, NULL, NULL);
        }
        wifi_info.reg.IP_Auto_Manual = wifi_register_value[IP_AUTO_MANUAL - WIFI_ENABLE];
        if (wifi_register_value[IP_WIFI_STATUS - WIFI_ENABLE] < (sizeof(Wifi_Module_Status) / sizeof(Wifi_Module_Status[0])))
        {
            GetDlgItem(IDC_EDIT_WIFI_STATUS)->SetWindowTextW(Wifi_Module_Status[wifi_register_value[IP_WIFI_STATUS - WIFI_ENABLE]]);
            
        }

        if (wifi_register_value[IP_WIFI_STATUS - WIFI_ENABLE] == WIFI_NO_WIFI)
        {
            ChangeWifiWindowStatus(0);
            return TRUE;
        }
        else
        {
            ChangeWifiWindowStatus(1);
        }

        GetDlgItem(IDC_EDIT_WIFI_BACNET_PORT)->EnableWindow(false);
        if (wifi_register_value[WIFI_MOUDLE_SOFTWARE_VERSION - WIFI_ENABLE] >= 2)
        {
            CString temp_cs_modbus_port;
            temp_cs_modbus_port.Format(_T("%u"), wifi_register_value[WIFI_MODBUS_PORT - WIFI_ENABLE]);
            CString temp_cs_bacnet_port;
            temp_cs_bacnet_port.Format(_T("%u"), wifi_register_value[WIFI_BACNET_PORT - WIFI_ENABLE]);

            GetDlgItem(IDC_EDIT_WIFI_MODBUS_PORT)->SetWindowTextW(temp_cs_modbus_port);
            GetDlgItem(IDC_EDIT_WIFI_BACNET_PORT)->SetWindowTextW(temp_cs_bacnet_port);

            m_version = wifi_register_value[WIFI_MOUDLE_SOFTWARE_VERSION - WIFI_ENABLE];

            for (int i = 0; i < 6; i++)
            {
                wifi_info.reg.wifi_mac[i] = wifi_register_value[STATIC_IP_START_REG - WIFI_ENABLE + 12 + i];
            }

            cs_macadd.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), (BYTE)wifi_info.reg.wifi_mac[0],
                (BYTE)wifi_info.reg.wifi_mac[1],
                (BYTE)wifi_info.reg.wifi_mac[2],
                (BYTE)wifi_info.reg.wifi_mac[3],
                (BYTE)wifi_info.reg.wifi_mac[4],
                (BYTE)wifi_info.reg.wifi_mac[5]);
            m_Edit_MacAddress.SetWindowText(cs_macadd);
        }
        else
        {
            GetDlgItem(IDC_EDIT_WIFI_MODBUS_PORT)->EnableWindow(false);

            m_version = 1;
        }

        if (wifi_register_value[WIFI_MOUDLE_SOFTWARE_VERSION - WIFI_ENABLE] >= 3)
        {
            GetDlgItem(IDC_RADIO_WIFI_ENABLE)->EnableWindow(1);
            GetDlgItem(IDC_RADIO_WIFI_DISABLE)->EnableWindow(1);
            if (wifi_register_value[WIFI_ENABLE] == 2)
            {
                ((CButton *)GetDlgItem(IDC_RADIO_WIFI_DISABLE))->SetCheck(true);
                ((CButton *)GetDlgItem(IDC_RADIO_WIFI_ENABLE))->SetCheck(false);
            }
            else
            {
                ((CButton *)GetDlgItem(IDC_RADIO_WIFI_ENABLE))->SetCheck(true);
                ((CButton *)GetDlgItem(IDC_RADIO_WIFI_DISABLE))->SetCheck(false);
            }

            GetDlgItem(IDC_BUTTON_WIFI_DEFAULT)->EnableWindow(true);
        }
        else
        {
            GetDlgItem(IDC_RADIO_WIFI_ENABLE)->EnableWindow(0);
            GetDlgItem(IDC_RADIO_WIFI_DISABLE)->EnableWindow(0);
            GetDlgItem(IDC_BUTTON_WIFI_DEFAULT)->EnableWindow(0);
        }


        memcpy(wifi_info.reg.username, &wifi_register_value[10], sizeof(str_wifi_point) - 10);

        MultiByteToWideChar(CP_ACP, 0, (char *)wifi_info.reg.username, (int)strlen((char *)wifi_info.reg.username) + 1, cs_ssid.GetBuffer(MAX_PATH), MAX_PATH);
        cs_ssid.ReleaseBuffer();

        MultiByteToWideChar(CP_ACP, 0, (char *)wifi_info.reg.password, (int)strlen((char *)wifi_info.reg.password) + 1, cs_password.GetBuffer(MAX_PATH), MAX_PATH);
        cs_password.ReleaseBuffer();

        m_Edit_SSID.SetWindowTextW(cs_ssid);
        m_Edit_Key.SetWindowTextW(cs_password);
        for (int i = 0; i < 4; i++)
        {
            wifi_info.reg.ip_addr[i] = wifi_register_value[STATIC_IP_START_REG - WIFI_ENABLE + i];
        }
        
        for (int i = 0; i < 4; i++)
        {
            wifi_info.reg.net_mask[i] = wifi_register_value[STATIC_IP_START_REG - WIFI_ENABLE + 4 + i];
        }

        for (int i = 0; i < 4; i++)
        {
            wifi_info.reg.getway[i] = wifi_register_value[STATIC_IP_START_REG - WIFI_ENABLE + 8 + i];
        }




        m_ipaddress.SetAddress((BYTE)wifi_info.reg.ip_addr[0],
            (BYTE)wifi_info.reg.ip_addr[1],
            (BYTE)wifi_info.reg.ip_addr[2],
            (BYTE)wifi_info.reg.ip_addr[3]);

        m_Subnet_Mask.SetAddress((BYTE)wifi_info.reg.net_mask[0],
            (BYTE)wifi_info.reg.net_mask[1],
            (BYTE)wifi_info.reg.net_mask[2],
            (BYTE)wifi_info.reg.net_mask[3]);

        m_GatewayAddress.SetAddress((BYTE)wifi_info.reg.getway[0],
            (BYTE)wifi_info.reg.getway[1],
            (BYTE)wifi_info.reg.getway[2],
            (BYTE)wifi_info.reg.getway[3]);



        if (wifi_info.reg.IP_Auto_Manual == 1)
        {
            ((CButton *)GetDlgItem(IDC_RADIO_IP_AUTO))->SetCheck(false);
            ((CButton *)GetDlgItem(IDC_RADIO_IP_STATIC))->SetCheck(true);
            GetDlgItem(IDC_IPADDRESS1)->ShowWindow(true);
            GetDlgItem(IDC_IPADDRESS3)->ShowWindow(true);
            GetDlgItem(IDC_IPADDRESS2)->EnableWindow(true);
            GetDlgItem(IDC_STATIC_MASK)->ShowWindow(true);
            GetDlgItem(IDC_STATIC_GATEWAY)->ShowWindow(true);
            
        }
        else
        {
            ((CButton *)GetDlgItem(IDC_RADIO_IP_AUTO))->SetCheck(true);
            ((CButton *)GetDlgItem(IDC_RADIO_IP_STATIC))->SetCheck(false);
            GetDlgItem(IDC_IPADDRESS1)->ShowWindow(false);
            GetDlgItem(IDC_IPADDRESS3)->ShowWindow(false);
            GetDlgItem(IDC_IPADDRESS2)->EnableWindow(false);
            GetDlgItem(IDC_STATIC_MASK)->ShowWindow(false);
            GetDlgItem(IDC_STATIC_GATEWAY)->ShowWindow(false);
        }
    }



	return TRUE;  
}




BOOL CWifiConfigDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_ui_type == UI_TYPE_WIFI_REV1)
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
    if (m_ui_type == UI_TYPE_WIFI_REV1)
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
}


void CWifiConfigDlg::OnEnKillfocusEdit2()
{
    if (m_ui_type == UI_TYPE_WIFI_REV1)
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
}


void CWifiConfigDlg::OnBnClickedOk()
{
    if (m_ui_type == UI_TYPE_WIFI_REV1)
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
    }
    else if (m_ui_type == UI_TYPE_WIFI_REV2)
    {
        if (m_version >= 3)
        {
            bool isstatic_enablewifi = ((CButton *)GetDlgItem(IDC_RADIO_WIFI_ENABLE))->GetCheck(); //返回1表示选上，0表示没选上;
            bool isstatic_disablewifi = ((CButton *)GetDlgItem(IDC_RADIO_WIFI_DISABLE))->GetCheck(); //返回1表示选上，0表示没选上;
            if (isstatic_disablewifi)
            {
                int ret2 = write_one(g_tstat_id, WIFI_ENABLE, 2, 10);
                if (ret2 >= 0)
                {
                    MessageBox(_T("Wifi disabled!"));
                }
            }

        }

        CString str_ssid;
        m_Edit_SSID.GetWindowTextW(str_ssid);
        if (str_ssid.GetLength() <= 0)
        {
            return;
        }

        CString str_password;
        m_Edit_Key.GetWindowTextW(str_password);
        if (str_password.GetLength() <= 0)
        {
            return;
        }
        CString modbus_port; int n_modbus_port;
        CString bacnet_port; int n_bacnet_port;

        
        if (m_version >= 2)
        {
            GetDlgItem(IDC_EDIT_WIFI_BACNET_PORT)->GetWindowTextW(bacnet_port);
            GetDlgItem(IDC_EDIT_WIFI_MODBUS_PORT)->GetWindowTextW(modbus_port);
            n_modbus_port = _wtoi(modbus_port);
            n_bacnet_port = _wtoi(bacnet_port);
            if (n_modbus_port == 0)
                return;
            if (n_bacnet_port == 0)
                return;
        }

        BYTE address1, address2, address3, address4;
        BYTE subnet1, subnet2, subnet3, subnet4;
        BYTE gatway1, gatway2, gatway3, gatway4;
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS2))->GetAddress(address1, address2, address3, address4);
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS1))->GetAddress(subnet1, subnet2, subnet3, subnet4);
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS3))->GetAddress(gatway1, gatway2, gatway3, gatway4);

        wifi_info.reg.ip_addr[0] = address1;
        wifi_info.reg.ip_addr[1] = address2;
        wifi_info.reg.ip_addr[2] = address3;
        wifi_info.reg.ip_addr[3] = address4;
        wifi_info.reg.net_mask[0] = subnet1;
        wifi_info.reg.net_mask[1] = subnet2;
        wifi_info.reg.net_mask[2] = subnet3;
        wifi_info.reg.net_mask[3] = subnet4;
        wifi_info.reg.getway[0] = gatway1;
        wifi_info.reg.getway[1] = gatway2;
        wifi_info.reg.getway[2] = gatway3;
        wifi_info.reg.getway[3] = gatway4;

        bool isstatic = ((CButton *)GetDlgItem(IDC_RADIO_IP_AUTO))->GetCheck(); //返回1表示选上，0表示没选上;
        if (isstatic == false)
            wifi_info.reg.IP_Auto_Manual = 1;
        else
            wifi_info.reg.IP_Auto_Manual = 0;
        wifi_info.reg.Wifi_Enable = 1;


       // memcpy(&wifi_register_value[10] ,wifi_info.reg.username,  sizeof(str_wifi_point) - 10);



        int ret2 = write_one(g_tstat_id, IP_AUTO_MANUAL, wifi_info.reg.IP_Auto_Manual,10);
        if (ret2 < 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration auto manual change failed "));
            return;
        }
        Sleep(SEND_COMMAND_DELAY_TIME*2);

        if (m_version >= 2)
        {
            ret2 = write_one(g_tstat_id, WIFI_MODBUS_PORT, n_modbus_port, 10);
            if (ret2 < 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration modbus port failed "));
                return;
            }
            Sleep(SEND_COMMAND_DELAY_TIME * 2);

#if 0  //暂时不让通过 界面去修改47808 的端口号 ，实在需要凯爷手动 modbus去改
            ret2 = write_one(g_tstat_id, WIFI_BACNET_PORT, n_bacnet_port, 10);
            if (ret2 < 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration bacnet port failed "));
                return;
            }
            Sleep(SEND_COMMAND_DELAY_TIME * 2);
#endif
        }



        WideCharToMultiByte(CP_ACP, 0, str_ssid.GetBuffer(), -1, wifi_info.reg.username, 64, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, str_password.GetBuffer(), -1, wifi_info.reg.password, 32, NULL, NULL);
        unsigned short temp_data[128] = { 0 };
        memset(temp_data, 0, 128);
        memcpy(temp_data, wifi_info.reg.username, 64);
        for (int i = 0; i < 128; i++)
        {
            temp_data[i] = htons(temp_data[i]);
        }
        int ret3 = Write_Multi_org_short(g_tstat_id, temp_data, SSID_NAME_START_REG, 32, 10);
        if (ret3 < 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration SSID change failed "));
            return;
        }
        Sleep(SEND_COMMAND_DELAY_TIME * 2);
        memset(temp_data, 0, 128);
        memcpy(temp_data, wifi_info.reg.password, 32);
        for (int i = 0; i < 128; i++)
        {
            temp_data[i] = htons(temp_data[i]);
        }
        int ret4 = Write_Multi_org_short(g_tstat_id, temp_data, PASSWORD_START_REG, 16, 10);
        if (ret4 < 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration password change failed "));
            return;
        }
        Sleep(SEND_COMMAND_DELAY_TIME * 2);
        memset(temp_data, 0, 128);
        for (int i = 0; i < 4; i++)
        {
            temp_data[i] = wifi_info.reg.ip_addr[i];
        }
        for (int i = 0; i < 4; i++)
        {
            temp_data[i+4] = wifi_info.reg.net_mask[i];
        }
        for (int i = 0; i < 4; i++)
        {
            temp_data[i+8] = wifi_info.reg.getway[i];
        }
        int ret5 = Write_Multi_org_short(g_tstat_id, temp_data, STATIC_IP_START_REG, 12, 10);
        if (ret5 < 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration IP information change failed "));
            return;
        }
        Sleep(SEND_COMMAND_DELAY_TIME * 2);
        int ret1 = write_one(g_tstat_id, WIFI_ENABLE, wifi_info.reg.Wifi_Enable,5);
        //if (ret1 < 0)
        //{
        //    SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration change enable flag failed "));
        //    return;
        //}
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Configuration change success "));

    }



	CDialogEx::OnOK();
}



  


void CWifiConfigDlg::OnBnClickedRadioIpAuto()
{
    // TODO: 在此添加控件通知处理程序代码
    GetDlgItem(IDC_IPADDRESS1)->ShowWindow(false);
    GetDlgItem(IDC_IPADDRESS3)->ShowWindow(false);
    GetDlgItem(IDC_IPADDRESS2)->EnableWindow(false);
    GetDlgItem(IDC_STATIC_MASK)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_GATEWAY)->ShowWindow(false);
}


void CWifiConfigDlg::OnBnClickedRadioIpStatic()
{
    // TODO: 在此添加控件通知处理程序代码
    GetDlgItem(IDC_IPADDRESS1)->ShowWindow(true);
    GetDlgItem(IDC_IPADDRESS3)->ShowWindow(true);
    GetDlgItem(IDC_IPADDRESS2)->EnableWindow(true);
    GetDlgItem(IDC_STATIC_MASK)->ShowWindow(true);
    GetDlgItem(IDC_STATIC_GATEWAY)->ShowWindow(true);

    //如果客户改为静态IP ，而子网掩码又是 0.0.0.0  就默认给客户一个较为规范的255.255.255.0
    BYTE subnet1, subnet2, subnet3, subnet4;
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS1))->GetAddress(subnet1, subnet2, subnet3, subnet4);
    if ((subnet1 == 0) && (subnet1 == 0) && (subnet1 == 0) && (subnet1 == 0))
    {
        ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS1))->SetAddress(255, 255, 255, subnet4);
    }
}


void CWifiConfigDlg::OnBnClickedButtonWifiDefault()
{
    // TODO: 在此添加控件通知处理程序代码
    write_one(g_tstat_id, WIFI_LOAD_DEFAULT, 1, 10);
}
