#include "pch.h"

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include "global_define.h"
#include "global_function.h"
#pragma warning(disable : 4996)
//#include <WS2tcpip.h>
//#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#include "dllmain.h"
using namespace std;  // Ensure that the namespace is set to std

vector <refresh_net_device_dll> m_refresh_net_device_data;

int AddNCToList(BYTE* buffer, int nBufLen, sockaddr_in& siBind);
struct ALL_LOCAL_SUBNET_NODE {
    CString StrIP;
    CString StrMask;
    CString StrGetway;
    int NetworkCardType;
};

vector <ALL_LOCAL_SUBNET_NODE> g_Scan_Vector_Subnet;
void GetIPMaskGetWayForScan()
{
    g_Scan_Vector_Subnet.clear();
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen;
    pAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    ALL_LOCAL_SUBNET_NODE  Temp_Node;
    // GetAdapterInfoulOutBufLen
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            MultiByteToWideChar(CP_ACP, 0, pAdapter->IpAddressList.IpAddress.String, (int)strlen((char*)pAdapter->IpAddressList.IpAddress.String) + 1,
                Temp_Node.StrIP.GetBuffer(MAX_PATH), MAX_PATH);
            Temp_Node.StrIP.ReleaseBuffer();
            //StrIP.Format(_T("%s"),pAdapter->IpAddressList.IpAddress.String);
            MultiByteToWideChar(CP_ACP, 0, pAdapter->IpAddressList.IpMask.String, (int)strlen((char*)pAdapter->IpAddressList.IpMask.String) + 1,
                Temp_Node.StrMask.GetBuffer(MAX_PATH), MAX_PATH);
            Temp_Node.StrMask.ReleaseBuffer();


            MultiByteToWideChar(CP_ACP, 0, pAdapter->GatewayList.IpAddress.String, (int)strlen((char*)pAdapter->GatewayList.IpAddress.String) + 1,
                Temp_Node.StrGetway.GetBuffer(MAX_PATH), MAX_PATH);
            Temp_Node.StrGetway.ReleaseBuffer();

            Temp_Node.NetworkCardType = pAdapter->Type;

            g_Scan_Vector_Subnet.push_back(Temp_Node);

            pAdapter = pAdapter->Next;
        }
    }
    else
    {

    }
}
#include <winsock.h>
#include <winsock2.h>
unsigned int g_llTxCount = 0;
unsigned int g_llRxCount = 0;
CString local_enthernet_ip;
char local_network_ip[255];
SOCKADDR_IN h_siBind;

int ScanByUDPFunc(vector<refresh_net_device_dll>& ret_scan_results)
{
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);

    if (::WSAStartup(sockVersion, &wsaData) != 0)
    {
        //AfxMessageBox(_T("Init Socket failed!"));
        return FALSE;
    }
    Inial_Product_map();
    m_refresh_net_device_data.clear();
	GetIPMaskGetWayForScan();

    for (int index = 0; index < g_Scan_Vector_Subnet.size(); index++)
    {
        if (g_Scan_Vector_Subnet[index].StrIP.Find(_T("0.0.0.0")) != -1)
        {
            continue;
        }
        SOCKET sListen = NULL;
#pragma region new_socket
        SOCKET h_scan_Broad = NULL;
        SOCKADDR_IN h_scan_bcast;
        SOCKADDR_IN h_scan_siBind;
        h_scan_Broad = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        BOOL bBroadcast = TRUE;
        ::setsockopt(h_scan_Broad, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL));
        int iMode = 1;
        ioctlsocket(h_scan_Broad, FIONBIO, (u_long FAR*) & iMode);

        BOOL bDontLinger = FALSE;
        setsockopt(h_scan_Broad, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));


        //SOCKADDR_IN bcast;
        h_scan_bcast.sin_family = AF_INET;
        //bcast.sin_addr.s_addr=nBroadCastIP;
        h_scan_bcast.sin_addr.s_addr = INADDR_BROADCAST;
        h_scan_bcast.sin_port = htons(UDP_BROADCAST_PORT);


        h_scan_siBind.sin_family = AF_INET;
        h_scan_siBind.sin_addr.s_addr = INADDR_ANY;



#pragma endregion new_socket


        local_enthernet_ip = g_Scan_Vector_Subnet[index].StrIP;
        WideCharToMultiByte(CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL);
        h_siBind.sin_family = AF_INET;
        h_siBind.sin_addr.s_addr = inet_addr(local_network_ip);
        //	h_siBind.sin_port=
        h_siBind.sin_port = htons(57629);
        if (-1 == bind(h_scan_Broad, (SOCKADDR*)&h_siBind, sizeof(h_siBind)))//Socket
        {
            continue;
        }


        int nRet = 0;
        short nmsgType = UPD_BROADCAST_QRY_MSG;


        const DWORD END_FLAG = 0x00000000;
        TIMEVAL time;
        time.tv_sec = 3;
        time.tv_usec = 1000;

        fd_set fdSocket;
        FD_ZERO(&fdSocket);
        FD_SET(h_scan_Broad, &fdSocket);

        BYTE buffer[512] = { 0 };

        BYTE pSendBuf[1024];
        ZeroMemory(pSendBuf, 255);
        pSendBuf[0] = 100;
        //pSendBuf[1] = END_FLAG;
        memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
        int nSendLen = 5;

        int time_out = 0;
        BOOL bTimeOut = FALSE;
        while (!bTimeOut)//!pScanner->m_bNetScanFinish)  // 
        {
            time_out++;
            if (time_out > 3)
                bTimeOut = TRUE;

            nRet = ::sendto(h_scan_Broad, (char*)pSendBuf, nSendLen, 0, (sockaddr*)&h_scan_bcast, sizeof(h_scan_bcast));
            if (nRet == SOCKET_ERROR)
            {
                int  nError = WSAGetLastError();

                goto END_SCAN;
                //return 0;
            }


            int nLen = sizeof(h_scan_siBind);


            fd_set fdRead = fdSocket;
            int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
            if (nSelRet == SOCKET_ERROR)
            {
                int nError = WSAGetLastError();

                goto END_SCAN;
                return 0;
            }

            if (nSelRet > 0)
            {
                ZeroMemory(buffer, 512);

                do
                {
                    int nRet = ::recvfrom(h_scan_Broad, (char*)buffer, 512, 0, (sockaddr*)&h_scan_siBind, &nLen);
                    BYTE szIPAddr[4] = { 0 };
                    if (nRet > 0)
                    {
                        FD_ZERO(&fdSocket);
                        if (buffer[0] == RESPONSE_MSG)
                        {
                            g_llRxCount++;
                            nLen = buffer[2] + buffer[3] * 256;
                            unsigned short dataPackage[32] = { 0 };
                            memcpy(dataPackage, buffer + 2, nLen * sizeof(unsigned short));
                            szIPAddr[0] = buffer[16];
                            szIPAddr[1] = buffer[18];
                            szIPAddr[2] = buffer[20];
                            szIPAddr[3] = buffer[22];
                            CString StrIp;
                            StrIp.Format(_T("%d.%d.%d.%d"), szIPAddr[0], szIPAddr[1], szIPAddr[2], szIPAddr[3]);
                            if (StrIp.GetLength() <= 16)
                            {

                            }


                            int n = 1;
                            BOOL bFlag = FALSE;
                            //////////////////////////////////////////////////////////////////////////
                            // IP
                            DWORD dwValidIP = 0;
                            memcpy((BYTE*)&dwValidIP, pSendBuf + n, 4);
                            while (dwValidIP != END_FLAG)
                            {
                                DWORD dwRecvIP = 0;

                                memcpy((BYTE*)&dwRecvIP, szIPAddr, 4);
                                memcpy((BYTE*)&dwValidIP, pSendBuf + n, 4);

                                if (dwRecvIP == dwValidIP)
                                {
                                    bFlag = TRUE;
                                    break;
                                }
                                n += 4;
                            }
                            //////////////////////////////////////////////////////////////////////////
                            bFlag = FALSE;
                            if (!bFlag)	// Ip MinipanelTSTAT ;
                            {

                                AddNCToList(buffer, nRet, h_scan_siBind);

                                pSendBuf[nSendLen - 4] = szIPAddr[0];
                                pSendBuf[nSendLen - 3] = szIPAddr[1];
                                pSendBuf[nSendLen - 2] = szIPAddr[2];
                                pSendBuf[nSendLen - 1] = szIPAddr[3];
                                memcpy(pSendBuf + nSendLen, (BYTE*)&END_FLAG, 4);
                                //////////////////////////////////////////////////////////////////////////

                                //pSendBuf[nSendLen+3] = 0xFF;
                                nSendLen += 4;
                            }

                            //}
                        }
                        else
                        {
                            break;
                        }
                        //SHOW_TX_RX

                            FD_ZERO(&fdSocket);
                        FD_SET(h_scan_Broad, &fdSocket);
                        nLen = sizeof(h_scan_siBind);
                        fdRead = fdSocket;
                        nSelRet = ::select(0, &fdRead, NULL, NULL, &time);//TRACE("recv nc info == %d\n", nSelRet);
                    }
                } while (nSelRet);
            }
            else
            {
                bTimeOut = TRUE;

            }

        }//end of while

        //}//end of while
        //closesocket(sListen);

    END_SCAN:

        closesocket(h_scan_Broad);

    }
    ret_scan_results = m_refresh_net_device_data;
    return ret_scan_results.size();
}

int AddNCToList(BYTE* buffer, int nBufLen, sockaddr_in& siBind)
{

    refresh_net_device_dll temp = {0};
    Str_UPD_SCAN temp_data = {0};
    memset(&temp_data, 0, 400);
    unsigned char* my_temp_point = buffer;
    temp_data.reg.command = *(my_temp_point++);
    temp_data.reg.command_reserve = *(my_temp_point++);

    temp_data.reg.length = *(my_temp_point++);
    temp_data.reg.length_reserve = *(my_temp_point++);


    temp_data.reg.serial_low = *(my_temp_point++);
    temp_data.reg.serial_low_reserve = *(my_temp_point++);

    temp_data.reg.serial_low_2 = *(my_temp_point++);
    temp_data.reg.serial_low_2_reserve = *(my_temp_point++);

    temp_data.reg.serial_low_3 = *(my_temp_point++);
    temp_data.reg.serial_low_3_reserve = *(my_temp_point++);

    temp_data.reg.serial_low_4 = *(my_temp_point++);
    temp_data.reg.serial_low_4_reserve = *(my_temp_point++);

    temp_data.reg.product_id = *(my_temp_point++);
    temp_data.reg.product_id_reserve = *(my_temp_point++);


    temp_data.reg.modbus_id = *(my_temp_point++);
    temp_data.reg.modbus_id_reserve = *(my_temp_point++);

    temp_data.reg.ip_address_1 = *(my_temp_point++);
    temp_data.reg.ip_address_1_reserve = *(my_temp_point++);
    temp_data.reg.ip_address_2 = *(my_temp_point++);
    temp_data.reg.ip_address_2_reserve = *(my_temp_point++);
    temp_data.reg.ip_address_3 = *(my_temp_point++);
    temp_data.reg.ip_address_3_reserve = *(my_temp_point++);
    temp_data.reg.ip_address_4 = *(my_temp_point++);
    temp_data.reg.ip_address_4_reserve = *(my_temp_point++);

    temp_data.reg.modbus_port = ((unsigned char)my_temp_point[1]) << 8 | ((unsigned char)my_temp_point[0]);
    my_temp_point = my_temp_point + 2;
    temp_data.reg.sw_version = ((unsigned char)my_temp_point[1]) << 8 | ((unsigned char)my_temp_point[0]);
    my_temp_point = my_temp_point + 2;
    temp_data.reg.hw_version = ((unsigned char)my_temp_point[1]) << 8 | ((unsigned char)my_temp_point[0]);
    my_temp_point = my_temp_point + 2;

    if ((my_temp_point[0] == my_temp_point[1]) &&
        (my_temp_point[0] == my_temp_point[2]) &&
        (my_temp_point[0] == my_temp_point[3]) &&
        (my_temp_point[0] != 0))
    {
        // 4Airlab Bug ,Tree;
        my_temp_point[0] = 0; my_temp_point[1] = 0; my_temp_point[2] = 0; my_temp_point[3] = 0;
    }

    temp_data.reg.parent_serial_number = ((unsigned char)my_temp_point[3]) << 24 | ((unsigned char)my_temp_point[2] << 16) | ((unsigned char)my_temp_point[1]) << 8 | ((unsigned char)my_temp_point[0]);
    my_temp_point = my_temp_point + 4;

    temp_data.reg.object_instance_2 = *(my_temp_point++);
    temp_data.reg.object_instance_1 = *(my_temp_point++);
    temp_data.reg.station_number = *(my_temp_point++);
    memcpy(temp_data.reg.panel_name, my_temp_point, 20);
    my_temp_point = my_temp_point + 20;
    temp_data.reg.object_instance_4 = *(my_temp_point++);
    temp_data.reg.object_instance_3 = *(my_temp_point++);
    temp_data.reg.isp_mode = *(my_temp_point++);	//isp_mode = 0  0 bootload.
    temp_data.reg.bacnetip_port = ((unsigned char)my_temp_point[1]) << 8 | ((unsigned char)my_temp_point[0]);
    my_temp_point = my_temp_point + 2;
    temp_data.reg.hardware_info = *(my_temp_point++);
    temp_data.reg.subnet_protocol = *(my_temp_point++);

    if (temp_data.reg.subnet_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
    {
        //BIP MSTP ;
        return	 0;
    }

    if (temp_data.reg.isp_mode != 0)
    {
        //, bootload.
        return	 0;
    }
    DWORD nSerial = temp_data.reg.serial_low + temp_data.reg.serial_low_2 * 256 + temp_data.reg.serial_low_3 * 256 * 256 + temp_data.reg.serial_low_4 * 256 * 256 * 256;
    CString nip_address;
    nip_address.Format(_T("%u.%u.%u.%u"), temp_data.reg.ip_address_1, temp_data.reg.ip_address_2, temp_data.reg.ip_address_3, temp_data.reg.ip_address_4);
    CString nproduct_name = GetProductName(temp_data.reg.product_id);
    if (nproduct_name.IsEmpty())	//  exit;
    {
        if (temp_data.reg.product_id < 220)
        {
            return 0;
        }
    }

    temp.nport = temp_data.reg.modbus_port;
    temp.sw_version = temp_data.reg.sw_version;
    temp.hw_version = temp_data.reg.hw_version;
    //temp.ip_address = nip_address;
    WideCharToMultiByte(CP_ACP, 0, nip_address.GetBuffer(), -1, temp.ip_address, 30, NULL, NULL);
    //memcpy_s(temp.ip_address, 30, nip_address, 30);
    temp.product_id = temp_data.reg.product_id;
    temp.modbusID = temp_data.reg.modbus_id;
    temp.nSerial = nSerial;
    WideCharToMultiByte(CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, temp.NetCard_Address, 30, NULL, NULL);

    memcpy_s(temp.show_label_name, 50, temp_data.reg.panel_name, 20);

    temp.parent_serial_number = temp_data.reg.parent_serial_number;

    temp.object_instance = temp_data.reg.object_instance_1 + temp_data.reg.object_instance_2 * 256 + temp_data.reg.object_instance_3 * 256 * 256 + temp_data.reg.object_instance_4 * 256 * 256 * 256;
    temp.panal_number = temp_data.reg.station_number;
    temp.bacnetip_port = temp_data.reg.bacnetip_port;
    temp.hardware_info = temp_data.reg.hardware_info;


    bool find_exsit = false;

    for (int i = 0; i < (int)m_refresh_net_device_data.size(); i++)
    {
        if (m_refresh_net_device_data.at(i).nSerial == nSerial)
        {
            find_exsit = true;
            break;
        }
    }

    if (!find_exsit)
    {
        m_refresh_net_device_data.push_back(temp);
    }


    char* temp_point = NULL;
    refresh_net_label_info temp_label;
    temp_point = temp_data.reg.panel_name;
    if (((unsigned char)temp_point[0] != 0xff) && ((unsigned char)temp_point[1] != 0xff) && ((unsigned char)temp_point[0] != 0x00))
    {
        memcpy(temp_label.label_name, &temp_point[0], 20);
        temp_point = temp_point + 20;
        CString cs_temp_label;
        MultiByteToWideChar(CP_ACP, 0, (char*)temp_label.label_name, (int)strlen((char*)temp_label.label_name) + 1,
            cs_temp_label.GetBuffer(MAX_PATH), MAX_PATH);
        cs_temp_label.ReleaseBuffer();
        if (cs_temp_label.GetLength() > 20)
            cs_temp_label = cs_temp_label.Left(20);
        temp_label.serial_number = (unsigned int)nSerial;
    }


    Sleep(1000);
    return m_refresh_net_device_data.size();

}