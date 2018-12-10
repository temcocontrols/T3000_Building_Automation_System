#include "stdafx.h"
#include "BacnetMstp.h"
#if 0
vector <_Bac_Scan_Com_Info> m_bac_handle_Iam_data;
void LocalIAmHandler(uint8_t * service_request, uint16_t service_len, BACNET_ADDRESS * src)
{

    int len = 0;
    uint32_t device_id = 0;
    unsigned max_apdu = 0;
    int segmentation = 0;
    uint16_t vendor_id = 0;

    (void)src;
    (void)service_len;
    len = iam_decode_service_request(service_request, &device_id, &max_apdu,
        &segmentation, &vendor_id);

    address_add(device_id, max_apdu, src);

    CString bac_cs_mac;
    _Bac_Scan_Com_Info temp_1;
    //g_bac_instance =device_id;
#if 1
    if (src->mac_len == 6)
    {
        bac_cs_mac.Format(_T("%d"), src->mac[3]);
        temp_1.nprotocol = HANDLE_I_AM_BIP;
        memcpy(temp_1.ipaddress, src->mac, 6);
    }
    else if (src->mac_len == 1)
    {
        bac_cs_mac.Format(_T("%d"), src->mac[0]);
        temp_1.nprotocol = 0;
    }
    else
        return;
#endif
    //	bac_cs_mac.Format(_T("%d"),vendor_id);
    CString bac_cs_device_id;
    bac_cs_device_id.Format(_T("%d"), device_id);


    temp_1.device_id = device_id;
    //	temp_1.vendor_id = vendor_id;
    temp_1.macaddress = _wtoi(bac_cs_mac);

    int find_exsit = false;
    for (int i = 0; i<(int)m_bac_handle_Iam_data.size(); i++)
    {
        if ((m_bac_handle_Iam_data.at(i).device_id == temp_1.device_id)
            && (m_bac_handle_Iam_data.at(i).macaddress == temp_1.macaddress))
        {
            find_exsit = true;
        }
    }

    if (!find_exsit)
    {
        m_bac_handle_Iam_data.push_back(temp_1);
    }

    //::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
    return;

}

void Init_Service_Handlers(void)
{
    Device_Init(NULL);

    /* we need to handle who-is to support dynamic device binding */
    //2017 12 07  由杜帆屏蔽 客户不希望T3000 回Who is 的信息。
    //apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, LocalIAmHandler);



    apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_PRIVATE_TRANSFER, local_handler_conf_private_trans_ack);
    //apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,Read_Property_feed_back);


    /* set the handler for all the services we don't implement */
    /* It is required to send the proper reject message... */
    apdu_set_unrecognized_service_handler_handler
    (handler_unrecognized_service);
    /* we must implement read property - it's required! */
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
        handler_read_property);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
        handler_read_property_multiple);
    /* handle the data coming back from confirmed requests */
    //   apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,handler_read_property_ack);
#if defined(BACFILE)
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
        handler_atomic_read_file);
#endif
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
        handler_cov_subscribe);

    //apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY, Localhandler_read_property_ack);

}

bool Initial_bac(int comport, CString bind_local_ip, int n_baudrate)
{

    BACNET_ADDRESS src =
    {
        0
    };  /* address where message came from */
    uint16_t pdu_len = 0;
    unsigned timeout = 100;     /* milliseconds */
    BACNET_ADDRESS my_address, broadcast_address;
    char my_port[50];

    //Device_Set_Object_Instance_Number(4194300);
    srand((unsigned)time(NULL));
    unsigned int temp_value;
    temp_value = rand() % (0x3FFFFF);
    Device_Set_Object_Instance_Number(temp_value);
    address_init();
    Init_Service_Handlers();

       // initial_bip = false;
        set_datalink_protocol(MODBUS_BACNET_MSTP);
        //if ((g_mstp_com.status == 0) ||((g_mstp_com.status == 1) && ((g_mstp_com.ncomport != comport) || (g_mstp_com.nbaudrate != m_nbaudrat))))
        // {
        //    close_bac_com();
        m_bac_handle_Iam_data.clear();
        //     g_mstp_com.status = 1;
        //     g_mstp_com.ncomport = comport;
        //     g_mstp_com.nbaudrate = m_nbaudrat;
        // }
        // else
        // {
        close_bac_com();
        //     m_bac_handle_Iam_data.clear();
        //return true;
        // }
        //HANDLE temphandle;
        //temphandle = Get_RS485_Handle();
        //if(temphandle !=NULL)
        //{
        //    TerminateThread((HANDLE)Get_Thread1(),0);
        //    TerminateThread((HANDLE)Get_Thread2(),0);

        //    CloseHandle(temphandle);
        //    Set_RS485_Handle(NULL);
        //}
        close_com();

        dlmstp_set_baud_rate(n_baudrate);
        //		dlmstp_set_baud_rate(19200);
        dlmstp_set_mac_address(0);
        dlmstp_set_max_info_frames(DEFAULT_MAX_INFO_FRAMES);
        dlmstp_set_max_master(DEFAULT_MAX_MASTER);
        memset(my_port, 0, 50);

        CString temp_cs;
        //temp_cs.Format(_T("COM%d"),g_com);
        temp_cs.Format(_T("COM%d"), comport);
        char cTemp1[255];
        memset(cTemp1, 0, 255);
        WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
        temp_cs.ReleaseBuffer();


        sprintf(my_port, cTemp1);
        dlmstp_init(my_port);

        set_datalink_protocol(MODBUS_BACNET_MSTP);
        datalink_get_broadcast_address(&broadcast_address);
        datalink_get_my_address(&my_address);
        int * comport_parameter = new int;
        *comport_parameter = MODBUS_BACNET_MSTP;
        if (CM5_hThread != NULL)
        {
            TerminateThread(CM5_hThread, 0);
            CM5_hThread = NULL;
        }
        CM5_hThread = CreateThread(NULL, NULL, MSTP_Receive, comport_parameter, NULL, &nThreadID_x);


    if (!bac_net_initial_once)
    {
        bac_net_initial_once = true;
        timesec1970 = (unsigned long)time(NULL);
        timestart = 0;
        init_info_table();
        Init_table_bank();
    }
    return true;
}


//需要先让串口的modbus 扫完，那里会记录有哪些串口存在 bacnet的协议.
//在扫描bacnet的时候 将bacnet ip 扫描完后，要去依次扫描 串口的MS/TP
//Scan bacnet
DWORD WINAPI   _ScanBacnetMSTPThread(LPVOID lpVoid)
{
    vector <_Bac_Scan_results_Info> m_temp_result_data;
    m_temp_result_data.clear();

    CString temp_cs;
    CString temp_cstring;
    //temp_cstring = pScan->m_szComs.at(i).Right(pScan->m_szComs.at(i).GetLength() - 3);

    int temp_port = 3;// current_building_comport;
    int ret_results;
    Initial_bac(temp_port, _T(""), 115200);
    TRACE(_T("Now scan with COM%d\r\n"), temp_port);
    Sleep(5000);//等待几秒让MSTP 的token 运行起来.
    for (int i = 0; i<25; i++)
    {
        CString strInfo;
        strInfo.Format(_T("Send MSTP command time left(%d)"), 25 - i);
        char temp_char[250];
        WideCharToMultiByte(CP_ACP, 0, strInfo.GetBuffer(), -1, temp_char, 250, NULL, NULL);
        Send_WhoIs_Global(-1, -1);
        Sleep(2000);
        if ((m_bac_handle_Iam_data.size() > 0) && (i > 15))
            break;
    }


    int ready_to_read_count = m_bac_handle_Iam_data.size();
    CString strInfo1;
    CString strInfo;


    strInfo1.Format(_T("Scan  Bacnet mstp.Found %d BACNET device relpy who is"), ready_to_read_count);

    for (int i = 0; i<ready_to_read_count; i++)
    {
        unsigned short test_array[1000];
        int ntest_ret = 0;

        memset(test_array, 0, 2000);
        for (int j = 0; j < 50; j++)
        {
            ntest_ret = GetPrivateBacnetToModbusData(m_temp_com_data.at(i).device_id, 0, 100, test_array);
            if (ntest_ret >= 0)
            {
                if ((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
                {
                    CString total_char_test;
                    //total_char_test = _T("Read MSTP To Modbus : ");
                    total_char_test.Format(_T("Read MSTP To Modbus %d , %d:"), 0, 100);
                    unsigned short * temp_print_test = NULL;
                    temp_print_test = test_array;
                    for (int i = 0; i< 100; i++)
                    {
                        CString temp_char_test;
                        temp_char_test.Format(_T("%04x"), (unsigned char)*temp_print_test);
                        temp_char_test.MakeUpper();
                        temp_print_test++;
                        total_char_test = total_char_test + temp_char_test + _T(" ");
                    }
                    DFTrace(total_char_test);

                }

                _Bac_Scan_results_Info  temp_info;
                temp_info.serialnumber = test_array[0] + test_array[1] * 256 + test_array[2] * 256 * 256 + test_array[3] * 256 * 256 * 256;
                temp_info.product_type = test_array[7];
                temp_info.modbus_addr = test_array[6];
                temp_info.panel_number = m_temp_com_data.at(i).macaddress;
                temp_info.software_version = 0;
                temp_info.hardware_version = 0;
                temp_info.m_protocol = PROTOCOL_MSTP_TO_MODBUS;
                temp_info.device_id = m_temp_com_data.at(i).device_id;

                temp_info.ipaddress[0] = test_array[47];
                temp_info.ipaddress[1] = test_array[48];
                temp_info.ipaddress[2] = test_array[49];
                temp_info.ipaddress[3] = test_array[50];

                m_temp_result_data.push_back(temp_info);
                break;
            }
            Sleep(200);
        }
        Sleep(1000);
    }

    int ret_receive_mstp_count = 0;
    if (m_temp_com_data.size() == 0)
    {
        //AfxMessageBox(_T("No device reply who is!"));
        goto end_mstp_thread;
    }
    else
    {
        ret_receive_mstp_count = m_temp_result_data.size();
        if (ret_receive_mstp_count > 0)
        {
            //AfxMessageBox(_T("Receive 99"));
        }
        else
        {
            goto end_mstp_thread;
            //AfxMessageBox(_T("No device reply 99 command!"));
        }
    }

    for (int l = 0; l<ret_receive_mstp_count; l++)
    {
        CString temp_serial_number;
        CString temp_baud;
        CString temp_pname;
        CString temp_modbusid;
        CString temp_view_name;
        CString temp_protocol;
        CString temp_product_id_string;
        CString temp_port_string;
        CString temp_object_instance;
        CString temp_panel_number;
        int count;
        temp_serial_number.Format(_T("%u"), m_temp_result_data.at(l).serialnumber);

        temp_modbusid.Format(_T("%u"), m_temp_result_data.at(l).modbus_addr);
        temp_view_name = temp_pname + _T(":") + temp_serial_number + _T("-") + temp_modbusid;
        temp_protocol.Format(_T("%d"), PROTOCOL_MSTP_TO_MODBUS);
        temp_product_id_string.Format(_T("%d"), m_temp_result_data.at(l).product_type);
        temp_port_string.Format(_T("%d"), 3);
        temp_object_instance.Format(_T("%u"), m_temp_result_data.at(l).device_id);
        temp_panel_number.Format(_T("%u"), m_temp_result_data.at(l).panel_number);
    }


end_mstp_thread:
    m_scan_info.at(scan_bacnet_ip_item).scan_status = SCAN_STATUS_FINISHED;
    HANDLE temphandle;
    if (pScan->m_eScanBacnetIpEnd->m_hObject)
    {
        pScan->m_eScanBacnetIpEnd->SetEvent();
    }
    //m_scan_info.at(scan_bacnet_ip_item).scan_status = SCAN_STATUS_FINISHED;
    //pScan->m_bStopScan = TRUE;

    close_bac_com();

    pScan->m_pScanBacnetIPThread = NULL;
    return 1;
}


void close_bac_com()
{
    HANDLE temphandle;
    temphandle = Get_RS485_Handle();
    if (temphandle != NULL)
    {
        TerminateThread((HANDLE)Get_Thread1(), 0);
        TerminateThread((HANDLE)Get_Thread2(), 0);

        CloseHandle(temphandle);
        Set_RS485_Handle(NULL);
    }
}

#endif

