// BacnetAddRemoteDevice.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAddRemoteDevice.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "MainFrm.h"
#include "ScanDlg.h"
// CBacnetAddRemoteDevice 对话框

vector <CString> local_ip_address;

CString Compare_CS;
CString static_info;
CString static_info1;
CString static_info2;
CString static_info3;
CString static_info4;

int ConnectToRemoteDevice(vector <CString> nlocal_ip_address/*LPCTSTR local_ip*/, LPCTSTR remote_ip, int udp_port, int tcp_port);
IMPLEMENT_DYNAMIC(CBacnetAddRemoteDevice, CDialogEx)

CBacnetAddRemoteDevice::CBacnetAddRemoteDevice(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_ADD_REMOTE_DEVICE, pParent)
{

}

CBacnetAddRemoteDevice::~CBacnetAddRemoteDevice()
{
}

void CBacnetAddRemoteDevice::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_ADD_REMOTE1, m_static_addinfo1);
    DDX_Control(pDX, IDC_STATIC_ADD_REMOTE2, m_static_addinfo2);
    DDX_Control(pDX, IDC_STATIC_ADD_REMOTE3, m_static_addinfo3);
    DDX_Control(pDX, IDC_STATIC_ADD_REMOTE4, m_static_addinfo4);
}


BEGIN_MESSAGE_MAP(CBacnetAddRemoteDevice, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_REMOTEDLG_SCAN, &CBacnetAddRemoteDevice::OnBnClickedButtonRemotedlgScan)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetAddRemoteDevice 消息处理程序

HANDLE h_add_remote_thread = NULL;
void CBacnetAddRemoteDevice::OnBnClickedButtonRemotedlgScan()
{
    // TODO: 在此添加控件通知处理程序代码
    BYTE address1, address2, address3, address4;
    //GetDlgItemText(IDC_COMBO_REMOTEDLG_LOCAL_IP, cs_local_ip);
    ((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_REMOTEDLG_REMOTE_IP))->GetAddress(address1, address2, address3, address4);
    cs_remote_ip.Format(_T("%u.%u.%u.%u"), address1, address2, address3, address4);

    CString temp_udp_port;
    GetDlgItemText(IDC_EDIT_REMOTEDLG_REMOTE_UDP_PORT, temp_udp_port);
    m_bacnet_udp_port = _wtoi(temp_udp_port);

    CString temp_tcp_port;
    GetDlgItemText(IDC_EDIT_REMOTEDLG_REMOTE_TCP_PORT, temp_tcp_port);
    m_modbus_tcp_port = _wtoi(temp_tcp_port);

    //if (cs_local_ip.IsEmpty())
    //{
    //    MessageBox(_T("Local Endpoint is not available!"));
    //    return;
    //}


    if (cs_remote_ip.CompareNoCase(_T("0.0.0.0")) == 0)
    {
        MessageBox(_T("Remote IP address is not available!"));
        return;
    }

    if ((m_bacnet_udp_port == 0) || (m_bacnet_udp_port >= 65535))
    {
        MessageBox(_T("Remote Bacnet UDP port is not available!"));
        return;
    }

    if ((m_modbus_tcp_port == 0) || (m_modbus_tcp_port >= 65535))
    {
        MessageBox(_T("Remote Modbus TCP Port is not available!"));
        return;
    }
    Sleep(1);

    if (h_add_remote_thread == NULL)
    {
        h_add_remote_thread = CreateThread(NULL, NULL, AddRemoteThreadPro, this, NULL, NULL);
        CloseHandle(h_add_remote_thread);
    }
}

DWORD WINAPI CBacnetAddRemoteDevice::AddRemoteThreadPro(LPVOID lPvoid)
{
    CBacnetAddRemoteDevice * mparent = (CBacnetAddRemoteDevice *)lPvoid;

    

    ConnectToRemoteDevice(local_ip_address/*mparent->cs_local_ip*/, mparent->cs_remote_ip, mparent->m_bacnet_udp_port, mparent->m_modbus_tcp_port);

    h_add_remote_thread = NULL;
    return true;
}

int ConnectToRemoteDevice(vector <CString> nlocal_ip_address/*LPCTSTR local_ip*/, LPCTSTR remote_ip, int udp_port, int tcp_port)
{


    for (int x = 0; x < nlocal_ip_address.size(); x++)
    {
        m_bac_handle_Iam_data.clear();
        CString local_ip;
        local_ip = nlocal_ip_address.at(x);

        Initial_bac(0, local_ip);
        bool n_connect_type = 0; // 0 bacnet   1 modbus;
        int ready_to_read_count = 0;
        for (int j = 0; j < 5; j++)
        {
            static_info.Format(_T("Scanning using network adapter:%s , please wait! (%d)"), local_ip,j);
            Send_WhoIs_remote_ip(remote_ip);
            Sleep(1500);
            ready_to_read_count = m_bac_handle_Iam_data.size();

            if (ready_to_read_count == 0)
            {
                continue;
            }
            Sleep(500);
        }


#if 0
        if (ready_to_read_count == 0)
        {
            static_info.Format(_T("No device response using Bacnet protocol (port:%d)!"), udp_port);
            Sleep(1000);
            static_info.Format(_T("Trying using Modbus (port:%d)!"), tcp_port);
            bool b_ret = Open_Socket_Retry(remote_ip, tcp_port, 6);
            if (b_ret)
            {
                n_connect_type = 1;
            }
            else
            {

            }
        }
#endif

        TRACE(_T("gloab scan = %d\r\n"), ready_to_read_count);
        for (int i = 0; i < ready_to_read_count; i++)
        {
            g_mstp_deviceid = m_bac_handle_Iam_data.at(i).device_id;
            if (n_connect_type != 1)
                g_protocol = PROTOCOL_BIP_T0_MSTP_TO_MODBUS;
            else
                g_protocol = MODBUS_TCPIP;
            unsigned short read_data[100] = { 0 };
            static_info.Format(_T("Reading device %d information!"), i + 1);
            int nmultyRet = Read_Multi(g_tstat_id, &read_data[0], 0, 50, 2);
            if (nmultyRet > 0)
            {

                int panel_name_start_add = PanelName_Map(read_data[7]);
                unsigned short read_panel_name[20] = { 0 };
                char char_name[40] = { 0 };
                static_info.Format(_T("Reading device %d name!"), i + 1);
                nmultyRet = Read_Multi(g_tstat_id, &read_panel_name[0], panel_name_start_add, 8, 6);

                CString cs_temp_name;
                if (nmultyRet >= 0)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        read_panel_name[j] = htons(read_panel_name[j]);
                    }
                    memcpy(char_name, read_panel_name, 16);
                    MultiByteToWideChar(CP_ACP, 0, (char *)char_name,
                        (int)strlen((char *)char_name) + 1,
                        cs_temp_name.GetBuffer(20), 20);
                    cs_temp_name.ReleaseBuffer();
                    cs_temp_name.Trim();
                }


                _Bac_Scan_results_Info temp_struct = { 0 };
                temp_struct.serialnumber = ((unsigned char)read_data[3]) << 24 | ((unsigned char)read_data[2] << 16) | ((unsigned char)read_data[1]) << 8 | ((unsigned char)read_data[0]);
                temp_struct.modbus_addr = read_data[6];
                temp_struct.product_type = read_data[7];
                temp_struct.modbus_port = tcp_port;
                if ((temp_struct.product_type == PM_MINIPANEL_ARM) || (temp_struct.product_type == PM_MINIPANEL))
                {
                    temp_struct.m_protocol = PROTOCOL_BACNET_IP;
                }
                else
                {
                    temp_struct.m_protocol = PROTOCOL_BIP_T0_MSTP_TO_MODBUS;
                }

                CppSQLite3DB SqliteDBBuilding;
                SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

                CString strSql;
                CString str_ip_address;
                CString str_n_port;
                CString str_serialid;
                CString product_name;
                CString modbusid;
                CString product_class_id;
                CString NetwordCard_Address;
                CString str_parents_serial;
                CString is_custom;
                CString str_object_instance;
                modbusid.Format(_T("%d"), read_data[6]);

                str_ip_address = remote_ip;
                str_n_port.Format(_T("%d"), tcp_port);

                str_serialid.Format(_T("%u"), temp_struct.serialnumber);
                product_class_id.Format(_T("%u"), read_data[7]);

                CppSQLite3Query query_serial;
                strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"), str_serialid);

                query_serial = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

                if (!query_serial.eof())
                {
                    strSql.Format(_T("Delete  From  ALL_NODE Where Serial_ID = '%d' "), temp_struct.serialnumber);
                    SqliteDBBuilding.execDML((UTF8MBSTR)strSql);   //如果查询到有重复的就删除后在插入
                }

                if (IS_Temco_Product(read_data[7]))
                {
                    is_custom = _T("0");
                }
                else
                {
                    is_custom = _T("1");
                }
                if (cs_temp_name.IsEmpty())
                    product_name = _T("RemoteDevice");
                else
                {
                    product_name = cs_temp_name;
                    static_info.Format(_T("Device %d , name : %s!"), i + 1, product_name);
                    Sleep(1000);
                }
                str_parents_serial = _T("0");
                str_object_instance.Format(_T("%u"), m_bac_handle_Iam_data.at(i).device_id);

                NetwordCard_Address = local_ip;

                CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
                pFrame->m_strCurSubBuldingName;
                CString str_hw_version;
                CString str_fw_version;
                CString str_panel_number = _T("0");
                if ((read_data[7] == PM_MINIPANEL) || (read_data[7] == PM_MINIPANEL_ARM))
                {
                    CString temp_pro3;
                    temp_pro3.Format(_T("%u"), PROTOCOL_REMOTE_IP /*PROTOCOL_BACNET_IP*/);
                    strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)  \
                        values('" + pFrame->m_strCurMainBuildingName + "','" + pFrame->m_strCurSubBuldingName + "','" + NetwordCard_Address + "','" + str_serialid + "','floor1','room1','" + product_name + "','" + product_class_id + "','" + modbusid + "','""','" + str_ip_address + "','T3000_Default_Building_PIC.bmp','" + str_hw_version + "','" + str_fw_version + "','" + str_n_port + "','0','" + temp_pro3 + "','1','" + str_parents_serial + "' ,'" + str_panel_number + "' ,'" + str_object_instance + "' ,'" + is_custom + "' )"));
                }
                else
                {
                    CString temp_pro4;
                    temp_pro4.Format(_T("%u"), PROTOCOL_BIP_T0_MSTP_TO_MODBUS /*PROTOCOL_BIP_T0_MSTP_TO_MODBUS*/);
                    strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,NetworkCard_Address,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)  \
                        values('" + pFrame->m_strCurMainBuildingName + "','" + pFrame->m_strCurSubBuldingName + "', '" + NetwordCard_Address + "','" + str_serialid + "','floor1','room1','" + product_name + "','" + product_class_id + "','" + modbusid + "','""','" + str_ip_address + "','T3000_Default_Building_PIC.bmp','" + str_hw_version + "','" + str_fw_version + "','" + str_n_port + "','0','" + temp_pro4 + "','1','" + str_parents_serial + "' ,'" + str_panel_number + "' ,'" + str_object_instance + "' ,'" + is_custom + "' )"));

                }

                SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                static_info.Format(_T("Add device %d into database success!"), i + 1);
            }
            else
            {
                static_info.Format(_T("Read device %d information failed!"), i + 1);
            }
            Sleep(500);
        }
        static_info.Format(_T("Network adapter:%s Scan finished!Found %d device"), local_ip, ready_to_read_count);
        Sleep(2000);
    }
    return 0;
}

BOOL CBacnetAddRemoteDevice::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    InitialStatic();
    InitialUI();
    SetTimer(1, 200, NULL);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CBacnetAddRemoteDevice::InitialStatic()
{
    m_static_addinfo1.SetWindowTextW(_T(""));
    m_static_addinfo1.textColor(RGB(0, 0, 0));
    //m_static_addinfo1.bkColor(RGB(80, 80, 255));
    m_static_addinfo1.setFont(24, 15, NULL, _T("Arial"));

    m_static_addinfo2.SetWindowTextW(_T(""));
    m_static_addinfo2.textColor(RGB(0, 0, 0));
    //m_static_addinfo2.bkColor(RGB(80, 80, 255));
    m_static_addinfo2.setFont(24, 15, NULL, _T("Arial"));

    m_static_addinfo3.SetWindowTextW(_T(""));
    m_static_addinfo3.textColor(RGB(0, 0, 0));
    //m_static_addinfo3.bkColor(RGB(80, 80, 255));
    m_static_addinfo3.setFont(24, 15, NULL, _T("Arial"));

    m_static_addinfo4.SetWindowTextW(_T(""));
    m_static_addinfo4.textColor(RGB(0, 0, 0));
    //m_static_addinfo4.bkColor(RGB(80, 80, 255));
    m_static_addinfo4.setFont(24, 15, NULL, _T("Arial"));

    Compare_CS.Empty();
    static_info1.Empty();
    static_info2.Empty();
    static_info3.Empty();
    static_info4.Empty();
}

void CBacnetAddRemoteDevice::InitialUI()
{
    local_ip_address.clear();
    GetIPMaskGetWay();

    ((CComboBox *)GetDlgItem(IDC_COMBO_REMOTEDLG_LOCAL_IP))->ResetContent();
    for (int i = 0; i < g_Vector_Subnet.size(); i++)
    {
        CString Local_IP_Add;
        Local_IP_Add = g_Vector_Subnet.at(i).StrIP;
        if (Local_IP_Add.IsEmpty())
            continue;
        if (Local_IP_Add.CompareNoCase(_T("0.0.0.0")) == 0)
            continue;
        ((CComboBox *)GetDlgItem(IDC_COMBO_REMOTEDLG_LOCAL_IP))->AddString(Local_IP_Add);
        local_ip_address.push_back(Local_IP_Add);
    }

    GetDlgItem(IDC_EDIT_REMOTEDLG_REMOTE_UDP_PORT)->SetWindowTextW(_T("47808"));
    GetDlgItem(IDC_EDIT_REMOTEDLG_REMOTE_TCP_PORT)->SetWindowTextW(_T("502"));

}

BOOL CBacnetAddRemoteDevice::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}



void CBacnetAddRemoteDevice::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    static int addinfo_count = 1;
    if (Compare_CS.CompareNoCase(static_info) == 0)
    {

        m_static_addinfo1.SetWindowTextW(static_info1);
        m_static_addinfo2.SetWindowTextW(static_info2);
        m_static_addinfo3.SetWindowTextW(static_info3);
        m_static_addinfo4.SetWindowTextW(static_info4);
    }
    else
    {
        CString cs_temp;
        cs_temp.Format(_T("%d.  "), addinfo_count);
        addinfo_count++;
        static_info = cs_temp + static_info;
        //static_info4 = static_info3;
        //static_info3 = static_info2;
        //static_info2 = static_info1;
        //static_info1 = static_info;

        static_info1 = static_info2;
        static_info2 = static_info3;
        static_info3 = static_info4;
        static_info4 = static_info;

        Compare_CS = static_info;



        m_static_addinfo1.SetWindowTextW(static_info1);
        m_static_addinfo2.SetWindowTextW(static_info2);
        m_static_addinfo3.SetWindowTextW(static_info3);
        m_static_addinfo4.SetWindowTextW(static_info4);
    }

    CDialogEx::OnTimer(nIDEvent);
}
