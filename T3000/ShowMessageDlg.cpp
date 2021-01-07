// ShowMessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "ShowMessageDlg.h"
#include "afxdialogex.h"
#include "ping.h"
#include "global_function.h"
#include "MainFrm.h"
extern tree_product selected_product_Node; // 选中的设备信息;
HANDLE hShowMessageHandle = NULL;
extern HANDLE hwait_read_thread;
// CShowMessageDlg 对话框
extern bool mstp_read_result ; //0  没读到    1  读成功    MSTP 设备 记录 建立连接时，是否为客户手动中断操作;
IMPLEMENT_DYNAMIC(CShowMessageDlg, CDialogEx)
int ok_button_press = 0; //确定按钮
int m_sync_time_auto_close_time;
CShowMessageDlg::CShowMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AA_SHOWMESSAGE, pParent)
{
    b_set_backcolor = false;
    b_show_progress = false;
    auto_close_time = 0;
}

CShowMessageDlg::~CShowMessageDlg()
{
}

void CShowMessageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_SHOW_MESSAGE, m_static_title);
    DDX_Control(pDX, IDC_STATIC_PROGRESS_PERSENT, m_static_persent);
    DDX_Control(pDX, IDC_PROGRESS_SHOW_MESSAGE, m_progress_showmessage);
}


BEGIN_MESSAGE_MAP(CShowMessageDlg, CDialogEx)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDOK, &CShowMessageDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CShowMessageDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CShowMessageDlg 消息处理程序
//设置需要显示的文字
void CShowMessageDlg::SetStaticText(LPCTSTR lpszTitleText)
{
    static_text = lpszTitleText;
    m_exit_by_hands = 0;
}

//设置需要显示的背景色
void CShowMessageDlg::SetStaticTextBackgroundColor(COLORREF TitleTextBackColor)
{
    b_set_backcolor = true;
    static_backcolor = TitleTextBackColor;
}

//设置需要显示的字体色
void CShowMessageDlg::SetStaticTextColor(COLORREF TitleTextBackColor)
{
    static_textcolor = TitleTextBackColor;
}

//设置字体大小
void CShowMessageDlg::SetStaticTextSize(int size_length , int size_width)
{
    static_text_length = size_length;
    static_text_width = size_width;
}


void CShowMessageDlg::SetChangedIPaddress(LPCTSTR lp_ipaddr)
{
    m_string_event_2_static_ip = lp_ipaddr;
}

void CShowMessageDlg::SetProgressAutoClose( int mi_seconds,int time_count, int nEvent)
{
    b_show_progress = true;
    auto_close_time = mi_seconds;
    auto_close_time_count = time_count;
    auto_close_time_count_old = auto_close_time_count;
    mevent = nEvent;

    if (nEvent == EVENT_WARNING_CHANGE_PROTOCOL_BAUDRATE)
    {
        b_show_progress = false;
    }
}

void CShowMessageDlg::SetEvent(int nEvent)
{
    mevent = nEvent;
}

void CShowMessageDlg::SetChangeProtocol(bool modbus_to_bacnet,   // 0 modbus          1  bacnet 
    unsigned char modbus_id,
    unsigned short nreg_address,
    unsigned short nreg_value,
    unsigned char sub_device,         // 如果是子设备  ，数据库中的协议 比较特殊;
    LPCTSTR Dbpath)
{
     cprotocol_modbus_to_bacnet = modbus_to_bacnet;   // 0 modbus          1  bacnet 
     cprotocol_modbus_id = modbus_id;
      cprotocol_nreg_address = nreg_address;
      cprotocol_nreg_value = nreg_value;
      cprotocol_sub_device = sub_device;         // 如果是子设备  ，数据库中的协议 比较特殊;
     cprotocol_Dbpath = Dbpath;

     b_show_progress = true;
     
}


void CShowMessageDlg::SetHwnd(HWND h_hwnd ,int nMessage)
{
    m_message_hwnd = h_hwnd;
    m_message = nMessage;
}

// MSTP 用于确认 whois 是否有回复;
void CShowMessageDlg::SetMstpDeviceInfo(_Bac_Scan_Com_Info deviceinfo)
{
    m_mstp_device_info = deviceinfo;
}


BOOL CShowMessageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    ok_button_press = 2; //初始化状态 未知;
    m_static_title.SetWindowTextW(static_text);
    m_static_title.textColor(static_textcolor);
    if(b_set_backcolor)
        m_static_title.bkColor(static_backcolor);
    m_static_title.setFont(static_text_length, static_text_width, NULL, _T("Arial"));

    if (b_show_progress)
    {
        m_static_persent.textColor(RGB(255,0,0));
        m_static_persent.setFont(18, 15, NULL, _T("Arial"));
    }
    else
    {
        m_static_persent.ShowWindow(false);
        m_progress_showmessage.ShowWindow(false);
    }

    if (mevent == EVENT_SYNC_TIME)
    {
        GetDlgItem(IDC_CHECK_DONT_POP)->ShowWindow(SW_SHOW);
        m_sync_time_auto_close_time = 60;
        SetTimer(2, 1000, NULL);
        Sleep(1);
    }
    else
    {
        GetDlgItem(IDC_CHECK_DONT_POP)->ShowWindow(SW_HIDE);
    }

    ::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetTimer(1, 200, NULL);
    if(hShowMessageHandle == NULL)
        hShowMessageHandle = CreateThread(NULL, NULL, ShowMessageThread, this, NULL, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

//此线程用于每隔5秒读取数据
DWORD WINAPI CShowMessageDlg::ShowMessageThread(LPVOID lPvoid)
{
    CShowMessageDlg * mparent = (CShowMessageDlg *)lPvoid;
    CMainFrame* pFrame = NULL;
        if (mparent->mevent == EVENT_IP_STATIC_CHANGE)
        {
            for (int i = 0; i < 50; i++)
            {
                mparent->m_pos = (mparent->auto_close_time_count_old - mparent->auto_close_time_count) * 100 / mparent->auto_close_time_count_old;
                mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
                Sleep(mparent->auto_close_time);
                mparent->auto_close_time_count--;
            }

            bool ping_ret = false;
            int try_time = 0;
            do
            {
                CPing p1;
                CPingReply pr1;
                ping_ret = p1.Ping1((LPCTSTR)mparent->m_string_event_2_static_ip, pr1);
                if(ping_ret == false)
                {
                    //如果ping 的不通
                    Sleep(2000);
                    try_time++;
                    mparent->m_pos = (mparent->auto_close_time_count_old - mparent->auto_close_time_count) * 100 / mparent->auto_close_time_count_old;
                    mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
                    mparent->auto_close_time_count--;
                }

            } while ((ping_ret == false) && (try_time < 7));

            do
            {
                mparent->m_pos = (mparent->auto_close_time_count_old - mparent->auto_close_time_count) * 100 / mparent->auto_close_time_count_old;
                mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
                Sleep(mparent->auto_close_time);
                mparent->auto_close_time_count--;
            } while (mparent->auto_close_time_count > 0);

            if (mparent->auto_close_time_count <= 0)
            {
                ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL, NULL);
                hShowMessageHandle = NULL;
                return true;
            }
        }
        else if (mparent->mevent == EVENT_IP_AUTO)
        {
            for (int i = 0; i < 60; i++)
            {
                mparent->m_pos = (mparent->auto_close_time_count_old - mparent->auto_close_time_count) * 100 / mparent->auto_close_time_count_old;
                mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
                Sleep(mparent->auto_close_time);
                mparent->auto_close_time_count--;
            }
            refresh_tree_status_immediately = true;
            do
            {
                mparent->m_pos = (mparent->auto_close_time_count_old - mparent->auto_close_time_count) * 100 / mparent->auto_close_time_count_old;
                mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
                Sleep(mparent->auto_close_time);
                mparent->auto_close_time_count--;
            } while (mparent->auto_close_time_count > 0);

            if (mparent->auto_close_time_count <= 0)
            {
                ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL, NULL);
                hShowMessageHandle = NULL;
                return true;
            }
        }
        else if (mparent->mevent == EVENT_MSTP_CONNECTION_ESTABLISH)
        {
            for (int i = 0; i<90; i++)
            {
                if (mparent->m_exit_by_hands)
                    break;
                mstp_port_struct_t Temp_MSTP_Port;
                Get_MSTP_STRUCT(&Temp_MSTP_Port);
                unsigned char mstp_add[255] = {0};
                unsigned char mstp_count = 0;

                Get_MSTP_Nodes(mstp_add, &mstp_count);
                //SetStaticText(_T("Establishing Bacnet MSTP connection , please wait!"));
                mparent->static_text.Format(_T("Establishing Bacnet MSTP connection , please wait!\r\nPolling node %d ,Des node %d\r\n"), Temp_MSTP_Port.Poll_Station, bac_gloab_panel);
                for (int i = 0; i < mstp_count; i++)
                {
                    CString temp_nodes;
                    temp_nodes.Format(_T("MSTP MAC ID : %d\r\n"), mstp_add[i]);
                    mparent->static_text = mparent->static_text + temp_nodes;
                }


                int temp_wait_count = 0;
                while ((Temp_MSTP_Port.SoleMaster == 1) && ( temp_wait_count < 10))
                {
                    Get_MSTP_STRUCT(&Temp_MSTP_Port);
                    Sleep(200);
                    mparent->static_text.Format(_T("Establishing Bacnet MSTP connection , please wait!\r\nPolling node %d ,Des node %d\r\nNo response from device\r\nPlease check the communication protocol and check the RS485 hardware connection\r\nScanning"), Temp_MSTP_Port.Poll_Station, bac_gloab_panel);
                    CString temp_net;
                    for (int i = 0; i < temp_wait_count; i++)
                    {
                        temp_net = temp_net + _T(".");
                    }
                    mparent->static_text = mparent->static_text + temp_net;
                    temp_wait_count++;
                }


                if(i%10 == 0)
                    Send_WhoIs_Global(-1, -1);
                mparent->m_pos = i;
                Sleep(1000);

                for (int j = 0; j < m_bac_handle_Iam_data.size(); j++)
                {
                    if ((mparent->m_mstp_device_info.device_id == m_bac_handle_Iam_data.at(j).device_id) /*&&
                        (mparent->m_mstp_device_info.macaddress == m_bac_handle_Iam_data.at(j).macaddress)*/)   //暂时拿掉 pannal number的 核对，茶洗说不是一一对应了
                    {
                        g_progress_persent = 33;
                        Sleep(200);
                        g_progress_persent = 66;
                        Sleep(200);
                        g_progress_persent = 100;
                        Sleep(200);
                        mstp_read_result = true;  // 读值成功;收到返回I am 设备;
                        ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL, NULL);
                        hShowMessageHandle = NULL;
                        return true;
                    }
                }

            }
            mparent->static_text = mparent->static_text + _T("\r\nDevice not responding.\r\nPlease Check the baudrate and protocol\r\nPlease check the RS485 cable.\r\n");
            Sleep(1500);
            system_connect_info.mstp_status = 0; //没有扫描到对应的 下次点击需要重新初始化;
            ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL, NULL);
        }
        else if (mparent->mevent == EVENT_FIRST_LOAD_PROG)
        {
            Sleep(500);
            while(hwait_read_thread != NULL)
            {
                if (g_progress_persent != 0)
                {
                    mparent->m_pos = g_progress_persent;
                    mparent->static_percent.Format(_T("%d%%"), g_progress_persent);
                }
                Sleep(100);
            }
            mparent->KillTimer(1);
            ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL, NULL);
        }
        else if (mparent->mevent == EVENT_CHANGE_PROTOCOL)
        {
            g_mstp_deviceid = selected_product_Node.object_instance;
            CppSQLite3DB SqliteDBBuilding;
            CppSQLite3Table table;
            CppSQLite3Query q;
            CString SqlText;
            mparent->m_pos = 5;
            mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);

            mparent->static_text.Format(_T("Reading device Bacnet device id!"));
            unsigned int temp_instance = GetDeviceInstance(product_register_value[7]);
            Sleep(1000);
            mparent->m_pos = 25;
            mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);

            if (temp_instance <= 0)
            {
                mparent->static_text.Format(_T("Read Bacnet device id failed!"));
                Sleep(2000);
                goto failed_path;
            }
            int n_ret = 0;

            mparent->static_text.Format(_T("Writing command, please wait!"));
            n_ret = write_one(mparent->cprotocol_modbus_id, mparent->cprotocol_nreg_address, mparent->cprotocol_nreg_value, 6);
            mparent->m_pos = 50;
            mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
            Sleep(1000);


            if (n_ret < 0)
            {
                mparent->static_text.Format(_T("Writing command failed!!"));
                goto failed_path;
            }

            if (mparent->cprotocol_modbus_to_bacnet == 0)  // 0 modbus          1  bacnet 
            {
                if (mparent->cprotocol_sub_device)
                {
                    Sleep(1);
                    //这里要做很多特殊情况处理 ，例如母设备下的   总线上还有没有其他mstp设备在运行;
                    //先把这个子节点的信息更改
                    if (Bacnet_Private_Device(selected_product_Node.product_class_id))
                        SqlText.Format(_T("update ALL_NODE set Protocol = '%d' where Serial_ID='%d'"), PROTOCOL_MB_TCPIP_TO_MB_RS485, g_selected_serialnumber);
                    else
                        SqlText.Format(_T("update ALL_NODE set Protocol = '%d' where Serial_ID='%d'"), MODBUS_TCPIP, g_selected_serialnumber);
                }
                else //
                {
                    SqlText.Format(_T("update ALL_NODE set Protocol = '%d' , Object_Instance = '%d' where Serial_ID='%d'"), MODBUS_RS485, temp_instance, g_selected_serialnumber /*get_serialnumber()*/);

                    //if (Bacnet_Private_Device(selected_product_Node.product_class_id)) //如果是BB类似的
                    //{
                    //    SqlText.Format(_T("update ALL_NODE set Protocol = '%d' , Object_Instance = '%d' where Serial_ID='%d'"), MODBUS_BACNET_MSTP, temp_instance, g_selected_serialnumber /*get_serialnumber()*/);
                    //}
                    //else
                    //    SqlText.Format(_T("update ALL_NODE set Protocol = '%d' , Object_Instance = '%d' where Serial_ID='%d'"), PROTOCOL_MSTP_TO_MODBUS, temp_instance, g_selected_serialnumber /*get_serialnumber()*/);
                }

            }
            else //  转换为 MSTP协议                 0 modbus          1  bacnet 
            {
                if (mparent->cprotocol_sub_device) //BIP下的mstp
                {
                    Sleep(1);
                    //这里要做很多特殊情况处理 ，例如母设备下的   总线上还有没有其他mstp设备在运行;
                    if (selected_product_Node.note_parent_serial_number != 0)
                    {
                        //挂在父节点下面的,更改协议成MSTP ，如果自己本身是T3控制器 就直接用MSTP的协议
                        if(Bacnet_Private_Device(selected_product_Node.product_class_id))
                            SqlText.Format(_T("update ALL_NODE set Protocol = '%d' where Serial_ID='%d'"), MODBUS_BACNET_MSTP, selected_product_Node.serial_number);
                        else
                            SqlText.Format(_T("update ALL_NODE set Protocol = '%d' where Serial_ID='%d'"), PROTOCOL_BIP_T0_MSTP_TO_MODBUS, selected_product_Node.serial_number);
                    }
                    
                }
                else //单纯的 mstp 协议 
                {
                    //SqlText.Format(_T("update ALL_NODE set Protocol = '%d' where Serial_ID='%d'"), MODBUS_RS485, g_selected_serialnumber);
                    if (Bacnet_Private_Device(product_register_value[7])) //如果是BB类似的 全支持的 就改为 全MSTP
                    {
                        //这里还要判断是不是通过网络在改 串口的协议？？？？？？？？？？？？？？？？？？？？？？
                        SqlText.Format(_T("update ALL_NODE set Protocol = '%d',Object_Instance = '%d',Panal_Number = '%d' where Serial_ID='%d'"), MODBUS_BACNET_MSTP, temp_instance, selected_product_Node.product_id, g_selected_serialnumber);
                    }
                    else//否则就要改为鸡肋版本的 MSTP-Modbus寄存器
                    {
                        SqlText.Format(_T("update ALL_NODE set Protocol = '%d',Object_Instance = '%d',Panal_Number = '%d' where Serial_ID='%d'"), PROTOCOL_MSTP_TO_MODBUS, temp_instance, selected_product_Node.product_id, g_selected_serialnumber);
                    }
                }
               
            }

            mparent->static_text.Format(_T("Changing the local configuration file!"));

            SqliteDBBuilding.open((UTF8MBSTR)mparent->cprotocol_Dbpath);

            SqliteDBBuilding.execDML((UTF8MBSTR)SqlText);
            SqliteDBBuilding.closedb();

            mparent->m_pos = 75;
            mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);

            Sleep(1000);
            mparent->static_text.Format(_T("The operation has completed successfully"));

            mparent->m_pos = 100;
            mparent->static_percent.Format(_T("%d%%"), mparent->m_pos);
            Sleep(1000);

            if (selected_product_Node.note_parent_serial_number != 0)
            {
                CString temp_product;
                temp_product = GetProductName(selected_product_Node.product_class_id);
                mparent->static_text.Format(_T("%s protocal and baudrate changed success"), temp_product);
                Sleep(1000);
                mparent->static_text.Format(_T("The controller's protocol and baud rate also need to be changed to communicate properly"));
                Sleep(2000);
            }


failed_path:
            pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
            pFrame->PostMessage(WM_MYMSG_REFRESHBUILDING, 0, 0);
            mparent->PostMessage(WM_CLOSE, NULL, NULL);
            if (selected_product_Node.note_parent_serial_number == 0)
            {
                pFrame->SetTimer(5, 2000, NULL);
            }
        }
        else if (mparent->mevent == EVENT_WARNING_CHANGE_PROTOCOL_BAUDRATE)
        {
            Sleep(mparent->auto_close_time);
            ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL, NULL);
        }
        else if (EVENT_SYNC_TIME == mparent->mevent)
        {
            for (int i = 0; i < 20; i++)
            {
                if ((ok_button_press != 0) && (ok_button_press != 1))
                {
                    Sleep(1000);
                }
                else
                {
                    ::PostMessage(mparent->m_hWnd,WM_CLOSE, NULL, NULL);
                    break;
                }

            }

                
        }
    hShowMessageHandle = NULL;
    return true;
}

BOOL CShowMessageDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CShowMessageDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CString temp_left_close_string;
    switch (nIDEvent)
    {
    case 1:
    {
        m_static_title.SetWindowTextW(static_text);
                m_static_persent.SetWindowTextW(static_percent);
                m_progress_showmessage.SetPos(m_pos);
    }
        break;
    case 2:       //SYNC Time auto close timer
    {
        if (m_sync_time_auto_close_time > 0)
            m_sync_time_auto_close_time--;
        else
        {
            KillTimer(2);
            PostMessage(WM_CLOSE, NULL, NULL);
        }
        temp_left_close_string.Format(_T("Cancel (%d)"), m_sync_time_auto_close_time);
        GetDlgItem(IDCANCEL)->SetWindowTextW(temp_left_close_string);
    }
        break;
    default:
        break;
    }
   
    

    CDialogEx::OnTimer(nIDEvent);
}


void CShowMessageDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    Sleep(1);
    CDialogEx::OnClose();
}


void CShowMessageDlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    //if (hShowMessageHandle != NULL)  //若是手动点击关闭窗口，则 需要关闭 bacnet 的 口;否则一直在哪里 循环去读一个没有 的matp设备;
    //{
    //    mstp_read_result = false;
    //    close_bac_com();
    //}
    //TerminateThread(hShowMessageHandle, 0);
    //hShowMessageHandle = NULL;
    Sleep(1);
    CDialogEx::OnCancel();
}


void CShowMessageDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码

    if (mevent == EVENT_SYNC_TIME)
    {
        if (((CButton *)GetDlgItem(IDC_CHECK_DONT_POP))->GetCheck())
        {
            unsigned long  temp_time_long = time(NULL);
            CString temp_cstring;
            temp_cstring.Format(_T("%u"), temp_time_long);
            WritePrivateProfileString(_T("SYNC_Time"), _T("ignore_pop"), _T("1"), g_cstring_ini_path);
            WritePrivateProfileString(_T("SYNC_Time"), _T("ignore_pop_time"), temp_cstring, g_cstring_ini_path);
        }



        ok_button_press = 1;
    }
    else
    {
        m_exit_by_hands = 1;
        system_connect_info.mstp_status = 0; //没有扫描到对应的 下次点击需要重新初始化;
        PostMessage(WM_CLOSE, NULL);
        Sleep(2000);
    }

    CDialogEx::OnOK();
}


void CShowMessageDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码

    if (mevent == EVENT_SYNC_TIME)
    {
        if (((CButton *)GetDlgItem(IDC_CHECK_DONT_POP))->GetCheck())
        {
            unsigned long  temp_time_long = time(NULL);
            CString temp_cstring;
            temp_cstring.Format(_T("%u"), temp_time_long);
            WritePrivateProfileString(_T("SYNC_Time"), _T("ignore_pop"), _T("1"), g_cstring_ini_path);
            WritePrivateProfileString(_T("SYNC_Time"), _T("ignore_pop_time"), temp_cstring, g_cstring_ini_path);
        }
        Sleep(1);
    }
    else
    {
        m_exit_by_hands = 1;
        //system_connect_info.mstp_status = 0; //没有扫描到对应的 下次点击需要重新初始化;
        PostMessage(WM_CLOSE, NULL);
        Sleep(2000);
    }


    CDialogEx::OnCancel();
}
