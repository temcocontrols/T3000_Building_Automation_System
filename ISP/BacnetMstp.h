#pragma once
#include "stdafx.h"

const int MODBUS_RS485 = 0;
const int MODBUS_TCPIP = 1;
const int MODBUS_BACNET_MSTP = 2;
const int PROTOCOL_BACNET_IP = 3;
const int PROTOCOL_GSM = 4;
const int PROTOCOL_REMOTE_IP = 6;
const int PROTOCOL_BIP_TO_MSTP = 10;
const int PROTOCOL_MSTP_TO_MODBUS = 11;
const int PROTOCOL_BIP_T0_MSTP_TO_MODBUS = 12;
const int PROTOCOL_VIRTUAL = 254;
const int PROTOCOL_UNKNOW = 255;

const int HANDLE_I_AM_MSTP = 0;
const int HANDLE_I_AM_BIP = 1;

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

//struct _Bac_Scan_Com_Info
//{
//    int device_id;
//    int macaddress;
//};
void LocalIAmHandler(uint8_t * service_request, uint16_t service_len, BACNET_ADDRESS * src);
void Init_Service_Handlers(void);
bool Initial_bac(int comport, CString bind_local_ip, int n_baudrate);
static DWORD WINAPI   _ScanBacnetMSTPThread(LPVOID lpVoid);
void close_bac_com();

