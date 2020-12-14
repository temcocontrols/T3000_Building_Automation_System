#include "stdafx.h"
#include "BacnetMstp.h"
Str_modbus_reg bacnet_to_modbus_struct;  //用于bacnet 协议转换为modbus 协议的结构
vector <_Bac_Scan_Com_Info> m_bac_handle_Iam_data;

HANDLE CM5_hThread = NULL;
BOOL g_mstp_flag;
DWORD nThreadID_x;
uint8_t Rx_Buf[MAX_MPDU] = { 0 };
DWORD WINAPI   MSTP_Receive(LPVOID lpVoid)
{
    BACNET_ADDRESS src = { 0 };
    uint16_t pdu_len;


    g_mstp_flag = true;
    while (g_mstp_flag)
    {
        pdu_len = datalink_receive(&src, &Rx_Buf[0], MAX_MPDU, 3000);
        if (pdu_len == 0)
        {
            Sleep(1);
            continue;
        }
        npdu_handler(&src, &Rx_Buf[0], pdu_len);
    }
    CM5_hThread = NULL;
    return 0;
}

void close_bac_com()
{
    HANDLE temphandle;
    temphandle = Get_RS485_Handle();
    if (temphandle != NULL)
    {
        Set_Thread1_Status(0);
        Set_Thread2_Status(0);
        Sleep(500);
        //TerminateThread((HANDLE)Get_Thread1(), 0);
        //TerminateThread((HANDLE)Get_Thread2(), 0);

        CloseHandle(temphandle);
        Set_RS485_Handle(NULL);
        m_bac_handle_Iam_data.clear();
        //g_mstp_com.status = 0;
    }

}

void LocalIAmHandler(uint8_t * service_request, uint16_t service_len, BACNET_ADDRESS * src)
{
    CString bac_cs_mac;
    int len = 0;
    uint32_t device_id = 0;
    unsigned max_apdu = 0;
    int segmentation = 0;
    uint16_t vendor_id = 0;

    (void)src;
    (void)service_len;
    len = iam_decode_service_request(service_request, &device_id, &max_apdu,
        &segmentation, &vendor_id);



#if 0
    fprintf(stderr, "Received I-Am Request");
    if (len != -1)
    {
        fprintf(stderr, " from %u!\n", device_id);
        address_add(device_id, max_apdu, src);
    }
    else
        fprintf(stderr, "!\n");
#endif
    address_add(device_id, max_apdu, src);


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
        temp_1.nprotocol = HANDLE_I_AM_MSTP;
    }
    else
        return;
#endif



    temp_1.device_id = device_id;
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
#ifdef USE_THIRD_PARTY_FUNC
        if (vendor_id != 148) //如果不是Temco的ID 才当作第三方设备
        {
            _Bac_Scan_Com_Info *temp = new _Bac_Scan_Com_Info;
            memcpy(temp, &temp_1, sizeof(_Bac_Scan_Com_Info));
            ::PostMessage(MainFram_hwd, WM_MAIN_MSG_SCAN_BAC, (WPARAM)temp, 0);
        }


#endif // DEBUG

    }

    return;

}

void Init_Service_Handlers(void)
{
    Device_Init(NULL);
#if 1
    /* we need to handle who-is to support dynamic device binding */
    //2017 12 07  由杜帆屏蔽 客户不希望T3000 回Who is 的信息。
    //apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, LocalIAmHandler);



    apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_PRIVATE_TRANSFER, local_handler_conf_private_trans_ack);
    //apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,Read_Property_feed_back);
//    apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY, localhandler_read_property_ack);

//    apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE, local_handler_read_property_multiple_ack);


    /* set the handler for all the services we don't implement */
    /* It is required to send the proper reject message... */
    apdu_set_unrecognized_service_handler_handler
    (handler_unrecognized_service);
    /* we must implement read property - it's required! */
    //apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
    //                           handler_read_property);
    //apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
    //                           handler_read_property_multiple);
    /* handle the data coming back from confirmed requests */
    //   apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,handler_read_property_ack);
#if defined(BACFILE)
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
        handler_atomic_read_file);
#endif
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
        handler_cov_subscribe);


    ////#if 0
    ////	/* Adding these handlers require the project(s) to change. */
    ////#if defined(BACFILE)
    ////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_WRITE_FILE,
    ////		handler_atomic_write_file);
    ////#endif
    ////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_RANGE,
    ////		handler_read_range);
    ////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
    ////		handler_reinitialize_device);
    ////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
    ////		handler_timesync_utc);
    ////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
    ////		handler_timesync);
    ////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
    ////		handler_ucov_notification);
    ////	/* handle communication so we can shutup when asked */
    ////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
    ////		handler_device_communication_control);
    ////#endif

#ifdef DEBUG
    //apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
    //    handler_read_property_multiple);
#endif // DEBUG
#endif
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

    srand((unsigned)time(NULL));
    unsigned int temp_value;
    temp_value = rand() % (0x3FFFFF);


    Device_Set_Object_Instance_Number(temp_value);

    address_init();
    Init_Service_Handlers();

    if (0)	//
    {

    }
    else
    {
        int * comport_parameter = new int;
        *comport_parameter = MODBUS_BACNET_MSTP;
        set_datalink_protocol(MODBUS_BACNET_MSTP);
        SetCommunicationType(0);
        close_bac_com();
        close_com();
        Sleep(2000);
        dlmstp_set_baud_rate(n_baudrate);
        //		dlmstp_set_baud_rate(19200);
        dlmstp_set_mac_address(0);
        dlmstp_set_max_info_frames(DEFAULT_MAX_INFO_FRAMES);
        //dlmstp_set_max_master(DEFAULT_MAX_MASTER);
        dlmstp_set_max_master(254);
        //g_thread_max_mac_id = 254;
        memset(my_port, 0, 50);

        CString temp_cs;
        //temp_cs.Format(_T("COM%d"),g_com);
        temp_cs.Format(_T("COM%d"), comport);
        char cTemp1[255];
        memset(cTemp1, 0, 255);
        WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
        temp_cs.ReleaseBuffer();


        sprintf(my_port, cTemp1);
        int mstp_init_ret = dlmstp_init(my_port);
        if (mstp_init_ret == false)
        {
            //system_connect_info.mstp_status = 0;
            return false;
        }

        set_datalink_protocol(MODBUS_BACNET_MSTP);
        datalink_get_broadcast_address(&broadcast_address);
        datalink_get_my_address(&my_address);
        if (CM5_hThread == NULL)
        {
            CM5_hThread = CreateThread(NULL, NULL, MSTP_Receive, comport_parameter, NULL, &nThreadID_x);
            CloseHandle(CM5_hThread);
        }
        else
        {
            g_mstp_flag = false;
            Sleep(3500);
            CM5_hThread = CreateThread(NULL, NULL, MSTP_Receive, comport_parameter, NULL, &nThreadID_x);
            CloseHandle(CM5_hThread);
        }
    }


    return true;
}




void local_handler_conf_private_trans_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
    BACNET_PRIVATE_TRANSFER_DATA data;
    int len;

    /*
    * Note:
    * We currently don't look at the source address and service data
    * but we probably should to verify that the ack is oneit is what
    * we were expecting. But this is just to silence some compiler
    * warnings from Borland.
    */
    src = src;
    service_data = service_data;

    len = 0;


    len = ptransfer_decode_service_request(service_request, service_len, &data);        /* Same decode for ack as for service request! */
    if (len < 0) {
        return;
    }
    int receive_data_type;
    bool each_end_flag = false;
    receive_data_type = Bacnet_PrivateData_Handle(&data, each_end_flag);
    if (receive_data_type < 0)
    {
        return;
    }

//    local_handler_update_bacnet_ui(receive_data_type, each_end_flag);

    return;
}



/***************************************************
**
** Receive Bacnet private data from device , and handle the data.
** Code by Fance
****************************************************/

int Bacnet_PrivateData_Handle(BACNET_PRIVATE_TRANSFER_DATA * data, bool &end_flag)
{
    int i;
    int iLen;   /* Index to current location in data */
    int tag_len;
    uint8_t tag_number;
    uint32_t len_value_type;
    BACNET_OCTET_STRING Temp_CS;
    char temp_buf[500];
    iLen = 0;
    int command_type;

    /* Error code is returned for read and write operations */

    tag_len = decode_tag_number_and_value(&data->serviceParameters[iLen], &tag_number, &len_value_type);
    iLen += tag_len;
    if (tag_number != BACNET_APPLICATION_TAG_OCTET_STRING)
    {
        /* if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {*/
#if PRINT_ENABLED
        printf("CPTA: Bad Encoding!\n");
#endif
        return 0;
    }
    //iLen +=
    //    decode_unsigned(&data->serviceParameters[iLen], len_value_type,
    //    &uiErrorCode);
    decode_octet_string(&data->serviceParameters[iLen], len_value_type, &Temp_CS);

    command_type = Temp_CS.value[2];

    char * bacnet_apud_point = (char *)Temp_CS.value;
    CString total_char_test;

    int ret_value = 0;
    ret_value = Bacnet_PrivateData_Deal(bacnet_apud_point, len_value_type, end_flag);
    return ret_value;

}

//This function code by Fance du for handle bacnet to modbus data;
int handle_bacnet_to_modbus_data(char *npoint, int nlength)
{
    Str_bacnet_to_modbus_header private_data_receive;
    char * my_temp_point = npoint;
    char * temp_print = my_temp_point;
    temp_print = my_temp_point;

    private_data_receive.total_length = (unsigned char)my_temp_point[1] << 8 | (unsigned char)my_temp_point[0];
    my_temp_point = my_temp_point + 2;
    private_data_receive.command = *(my_temp_point++);
    private_data_receive.start_reg = (unsigned char)my_temp_point[1] << 8 | (unsigned char)my_temp_point[0];
    my_temp_point = my_temp_point + 2;
    private_data_receive.nlength = (unsigned char)my_temp_point[1] << 8 | (unsigned char)my_temp_point[0];
    my_temp_point = my_temp_point + 2;

    bacnet_to_modbus_struct.nlength = private_data_receive.nlength;
    bacnet_to_modbus_struct.start_add = bacnet_to_modbus_struct.org_start_add;
    if (bacnet_to_modbus_struct.nlength == 0)
        return 0;
    if (bacnet_to_modbus_struct.nlength != bacnet_to_modbus_struct.org_nlength)
        return 0;
    if (bacnet_to_modbus_struct.start_add != bacnet_to_modbus_struct.org_start_add)
        return 0;

    if (private_data_receive.total_length != (2 * private_data_receive.nlength))
        return 0;

    if (bacnet_to_modbus_struct.nlength <= 200)
    {
        memcpy(bacnet_to_modbus_struct.ndata, my_temp_point, 2 * bacnet_to_modbus_struct.nlength);
        //因为下位机回传的大小端与电脑相反,需要转换为以下位机的为准
        for (int i = 0; i < nlength; i++)
        {
            bacnet_to_modbus_struct.ndata[i] = htons(bacnet_to_modbus_struct.ndata[i]);
        }
        my_temp_point = my_temp_point + 2 * bacnet_to_modbus_struct.nlength;
    }
    //bacnet_to_modbus_struct
    return 0;
}

int Bacnet_PrivateData_Deal(char * bacnet_apud_point, uint32_t len_value_type, bool &end_flag)
{
    int temp_struct_value;
    int command_type;
    int i = 0;
    int block_length;
    unsigned char start_instance = 0;
    unsigned char end_instance = 0;
    char *my_temp_point;
    command_type = *(bacnet_apud_point + 2);
    ///////////////////////////////
    switch (command_type)
    {

    case READ_BACNET_TO_MODBUS_COMMAND: //case READ_BACNET_TO_MODBUS_COMMAND:
    {
        handle_bacnet_to_modbus_data(bacnet_apud_point, len_value_type);
        return READ_BACNET_TO_MODBUS_COMMAND;
    }
    break;
    default:
        break;
    }
}


/************************************************************************/
/*
Author: Fance
Get  Private  Bacnet  To  ModbusData
//这个函数用来通过bacnet命令读取modbus 的相关寄存器
*/
/************************************************************************/
int GetPrivateBacnetToModbusData(uint32_t deviceid, uint16_t start_reg, int16_t readlength, unsigned short *data_out)
{
    //int retry_count = 1000;
    //if (MODE_SUPPORT_PTRANSFER)
    int   retry_count = 1000;

    int n_ret = 0;
    bacnet_to_modbus_struct.org_nlength = readlength;
    bacnet_to_modbus_struct.org_start_add = start_reg;
    uint8_t apdu[480] = { 0 };
    uint8_t test_value[480] = { 0 };
    int apdu_len = 0;
    int private_data_len = 0;
    unsigned max_apdu = 0;
    BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
    BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };
    bool status = false;
    private_data.vendorID = BACNET_VENDOR_ID;
    private_data.serviceNumber = 1;

    char SendBuffer[1000];
    memset(SendBuffer, 0, 1000);
    char * temp_buffer = SendBuffer;
    Str_bacnet_to_modbus_header private_data_chunk;

    int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;

    HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
    private_data_chunk.total_length = PRIVATE_HEAD_LENGTH;
    private_data_chunk.command = READ_BACNET_TO_MODBUS_COMMAND;
    private_data_chunk.start_reg = start_reg; // 测试
    private_data_chunk.nlength = readlength;
    Set_transfer_length(PRIVATE_HEAD_LENGTH);
    status = bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING, (char *)&private_data_chunk, &data_value);

    private_data_len = bacapp_encode_application_data(&test_value[0], &data_value);
    private_data.serviceParameters = &test_value[0];
    private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };

    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {
        n_ret = Send_ConfirmedPrivateTransfer(&dest, &private_data);

        if (n_ret >= 0)
        {
            for (int i = 0; i<retry_count; i++)
            {
                Sleep(10);
                if (tsm_invoke_id_free(n_ret))
                {
                    if (bacnet_to_modbus_struct.org_nlength != bacnet_to_modbus_struct.nlength)
                        return -3;
                    if (bacnet_to_modbus_struct.org_start_add != bacnet_to_modbus_struct.start_add)
                        return -4;
                    if (readlength != bacnet_to_modbus_struct.nlength)
                        return -5;
                    memcpy(data_out, bacnet_to_modbus_struct.ndata, 2 * bacnet_to_modbus_struct.nlength);
                    return readlength;
                }
                else
                    continue;
            }
        }
        else
        {
            Sleep(retry_count * 3);
            return -6;
        }
    }
    else
    {
        Send_WhoIs_Global(-1, -1);
        //test_string.Format(_T("test_count = %d , status = %d failed ,Sleep3000\r\n"), test_count, status);
        //TRACE(test_string);
        //if (MODE_SUPPORT_PTRANSFER)
            Sleep(300);
        //else
        //    Sleep(1000);  //address 

        return -2;
    }

    return -7;
}

int WritePrivateBacnetToModbusData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned short *data_in)
{
    unsigned short temp_data[400];
    int n_ret = 0;

    memset(temp_data, 0, 800);
    uint8_t apdu[480] = { 0 };
    uint8_t test_value[480] = { 0 };
    int apdu_len = 0;
    int private_data_len = 0;
    unsigned max_apdu = 0;
    BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
    BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };


    if ((writelength == 0) || (writelength > 128))
        return -4; //长度有误;

    bool status = false;

    private_data.vendorID = BACNET_VENDOR_ID;
    private_data.serviceNumber = 1;


    char SendBuffer[1000];
    memset(SendBuffer, 0, 1000);
    char * temp_buffer = SendBuffer;
    Str_bacnet_to_modbus_header private_data_chunk;

    int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;

    HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
    private_data_chunk.total_length = PRIVATE_HEAD_LENGTH + writelength * 2;
    private_data_chunk.command = WRITE_BACNET_TO_MODBUS_COMMAND;
    private_data_chunk.start_reg = start_reg; // 测试
    private_data_chunk.nlength = writelength;

    Set_transfer_length(private_data_chunk.total_length);
    memcpy_s(SendBuffer, PRIVATE_HEAD_LENGTH, &private_data_chunk, PRIVATE_HEAD_LENGTH);

    memcpy(temp_data, data_in, writelength * 2);
    //电脑和设备大小端不一致，这里以设备为准.
    for (int i = 0; i < writelength; i++)
    {
        temp_data[i] = htons(temp_data[i]);
    }

    memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH, writelength * 2, temp_data, writelength * 2);
    status = bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING, (char *)&SendBuffer, &data_value);

    private_data_len = bacapp_encode_application_data(&test_value[0], &data_value);
    private_data.serviceParameters = &test_value[0];
    private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };

    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {
        n_ret = Send_ConfirmedPrivateTransfer(&dest, &private_data);

        if (n_ret >= 0)
        {
            for (int i = 0; i<300; i++)
            {
                Sleep(10);
                if (tsm_invoke_id_free(n_ret))
                {

                    return 1;
                }
                else
                    continue;
            }
        }
    }
    else
        return -2;
}



int WritePrivateBacnetToModbusCharData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned char *data_in)
{
    unsigned short temp_data[400];
    int n_ret = 0;

    memset(temp_data, 0, 800);
    uint8_t apdu[480] = { 0 };
    uint8_t test_value[480] = { 0 };
    int apdu_len = 0;
    int private_data_len = 0;
    unsigned max_apdu = 0;
    BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
    BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };


    if ((writelength == 0) || (writelength > 128))
        return -4; //长度有误;

    bool status = false;

    private_data.vendorID = BACNET_VENDOR_ID;
    private_data.serviceNumber = 1;


    char SendBuffer[1000];
    memset(SendBuffer, 0, 1000);
    char * temp_buffer = SendBuffer;
    Str_bacnet_to_modbus_header private_data_chunk;

    int HEADER_LENGTH = PRIVATE_HEAD_LENGTH;

    HEADER_LENGTH = PRIVATE_HEAD_LENGTH;
    private_data_chunk.total_length = PRIVATE_HEAD_LENGTH + writelength * 2;
    private_data_chunk.command = WRITE_BACNET_TO_MODBUS_COMMAND;
    private_data_chunk.start_reg = start_reg; // 测试
    private_data_chunk.nlength = writelength;

    Set_transfer_length(private_data_chunk.total_length);
    memcpy_s(SendBuffer, PRIVATE_HEAD_LENGTH, &private_data_chunk, PRIVATE_HEAD_LENGTH);

    memcpy(temp_data, data_in, writelength );
    //电脑和设备大小端不一致，这里以设备为准.
    for (int i = 0; i < writelength; i++)
    {
        temp_data[i] = htons(temp_data[i]);
    }

    memcpy_s(SendBuffer + PRIVATE_HEAD_LENGTH, writelength * 2, temp_data, writelength * 2);
    status = bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING, (char *)&SendBuffer, &data_value);

    private_data_len = bacapp_encode_application_data(&test_value[0], &data_value);
    private_data.serviceParameters = &test_value[0];
    private_data.serviceParametersLen = private_data_len;

    BACNET_ADDRESS dest = { 0 };

    status = address_get_by_device(deviceid, &max_apdu, &dest);
    if (status)
    {
        n_ret = Send_ConfirmedPrivateTransfer(&dest, &private_data);

        if (n_ret >= 0)
        {
            for (int i = 0; i<300; i++)
            {
                Sleep(10);
                if (tsm_invoke_id_free(n_ret))
                {

                    return 1;
                }
                else
                    continue;
            }
        }
    }
    else
        return -2;
}





