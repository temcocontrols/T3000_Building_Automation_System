#pragma once
#include "stdafx.h"

#define MODBUS_RS485  0
#define MODBUS_TCPIP  1
#define MODBUS_BACNET_MSTP  2
#define PROTOCOL_BACNET_IP  3
#define PROTOCOL_GSM  4
#define PROTOCOL_REMOTE_IP  6
#define PROTOCOL_BIP_TO_MSTP  10
#define PROTOCOL_MSTP_TO_MODBUS  11
#define PROTOCOL_BIP_T0_MSTP_TO_MODBUS  12
#define PROTOCOL_VIRTUAL  254
#define PROTOCOL_UNKNOW  255

#define HANDLE_I_AM_MSTP  0
#define HANDLE_I_AM_BIP  1

struct _Bac_Scan_results_Info
{
    unsigned int device_id;
    unsigned char ipaddress[6];
    unsigned char modbus_addr;
    unsigned char product_type;
    unsigned int serialnumber;
    unsigned char panel_number;
    unsigned short modbus_port;
    unsigned short software_version;
    unsigned char hardware_version;
    unsigned char m_protocol;
};

struct _Bac_Scan_Com_Info
{
    int nprotocol;  // 0 MSTP     1 BIP
    unsigned char ipaddress[6];   //前四位位IP地址  后两位位端口号
    int device_id;
    int macaddress;
};
int Bacnet_PrivateData_Deal(char * bacnet_apud_point, uint32_t len_value_type, bool &end_flag);
int Bacnet_PrivateData_Handle(BACNET_PRIVATE_TRANSFER_DATA * data, bool &end_flag);
void local_handler_conf_private_trans_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
int GetPrivateBacnetToModbusData(uint32_t deviceid, uint16_t start_reg, int16_t readlength, unsigned short *data_out);
int WritePrivateBacnetToModbusData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned short *data_in);
int WritePrivateBacnetToModbusCharData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned char *data_in);


