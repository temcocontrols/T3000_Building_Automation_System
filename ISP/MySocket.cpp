// This Documents Add by Fance ,use this socket class communication
// FanceSocket.cpp : implementation file
//
/*
* Copyright (C) 2014 Fance Du <dufanfance@163.com>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*/
#pragma once
#include "stdafx.h"
#include "MySocket.h"
#include "TFTPServer.h"
#include "global_function.h"
#include "Global_Struct.h"
extern unsigned int n_check_temco_firmware;
extern CString Failed_Message;
extern Bin_Info        global_fileInfor;
typedef struct _Product_IP_ID 
{
	CString ISP_Device_IP;
	//unsigned char Byte_ISP_Device_IP[4];
	unsigned char ID;
}Product_IP_ID;

////#include <vector>			//ʸ��ģ�� - Vector template
////using  std::vector;			//�����ռ� - Namespace
////
////
////extern vector <Product_IP_ID> Product_Info;
//#include "CPublic.h"
#define WM_PROCESS_SHOW_RECEIVE 2001
/////////////////////////////////////////////////////////////////////////////
// CAkingSocket

//Used to display some results
extern CString ShowTFTPMessage; //������ʾһЩ���;
//Used to display some results
extern CString ShowTFTPMessage2; //������ʾһЩ���;
extern int ISP_STEP;
extern BYTE Byte_ISP_Device_IP[4];
extern bool device_has_replay_lan_IP;
extern volatile int package_number;
extern volatile int next_package_number;
extern bool device_jump_from_runtime;
extern bool has_enter_dhcp_has_lanip_block;
extern BYTE Product_Name[12];
extern BYTE Rev[4];
 
//extern bool some_device_reply_the_broadcast;
MySocket::MySocket()
{
	receive_not_want_ip_times = 0;
}

MySocket::~MySocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
/////////////////////////////////////////////////////////////////////////////
// Fance Socket member functions
void MySocket::OnAccept(int nErrorCode) 
{
	CAsyncSocket::OnAccept(nErrorCode);
}
extern CString ISP_Device_IP;
//Code by Fance 2013 05 06
void MySocket::OnReceive(int nErrorCode) 
{
	int tem_data=0;
	memset(receive_buf,0,4096);
	Receive_data_length=Receive(receive_buf,4096);
	if(ISP_STEP == ISP_SEND_FLASH_COMMAND)
	{
		//Receive EE 10's 40 byte reply
		if(Receive_data_length == 40)//����EE 10 �� 40 byte �ظ�
		{
			TRACE("ISP_SEND_FLASH_COMMAND Receive 40 byte\n");
			char temp_data[100];
			memset(temp_data,0,sizeof(temp_data));
			memcpy_s(temp_data,sizeof(temp_data),receive_buf,Receive_data_length);
			if((temp_data[0]==0x65)&&(temp_data[1]==0))
			{
				Byte_ISP_Device_IP[0] = temp_data[16];
				Byte_ISP_Device_IP[1] = temp_data[18];
				Byte_ISP_Device_IP[2] = temp_data[20];
				Byte_ISP_Device_IP[3] = temp_data[22];

				device_has_replay_lan_IP = true;
				ISP_STEP =ISP_SEND_DHCP_COMMAND_HAS_LANIP;
				device_jump_from_runtime = true;
			}
		}
		else if(Receive_data_length == 31)
		{
			TRACE("ISP_SEND_FLASH_COMMAND Receive 31 byte\n");
			char temp_data[100];
			memset(temp_data,0,sizeof(temp_data));
			memcpy_s(temp_data,sizeof(temp_data),receive_buf,11);//copy the first 11 byte and check
			if(!strcmp(temp_data,"ReceiveDHCP"))
			{
                memset(Byte_ISP_Device_IP, 0, 4);
                memset(Product_Name, 0, 12);
				memcpy_s(Byte_ISP_Device_IP,sizeof(Byte_ISP_Device_IP),receive_buf+11,4);//copy the first 11 byte and check
                memcpy_s(Product_Name,sizeof(Product_Name),receive_buf+15,11);//copy the first 11 byte and check

				//First check if it's a reply from the target IP address, prioritize processing replies from the target IP address
				//�ȼ���ǲ���Ŀ��IP��ַ�ظ��ģ����ȴ���Ŀ��IP��ַ�ظ���
				CString temp_reply_ip;
				temp_reply_ip.Format(_T("%u.%u.%u.%u"), Byte_ISP_Device_IP[0], Byte_ISP_Device_IP[1], Byte_ISP_Device_IP[2], Byte_ISP_Device_IP[3]);
				if ((temp_reply_ip.CompareNoCase(ISP_Device_IP) != 0) && (receive_not_want_ip_times <3))
				{
					ShowTFTPMessage2.Format(_T("IP:%s responds to the update request (%d)"), temp_reply_ip.GetString(), receive_not_want_ip_times);
					receive_not_want_ip_times++;
					//CAsyncSocket::OnReceive(nErrorCode); //��ʱ���Σ��豸�����ظ� 05 �������������ʧ�� - Temporarily shielded, the device often replies with error command 05, causing handshake failure
					//return;
				}
				

			    CString DeviceProductName ,FileProductName;

                MultiByteToWideChar(CP_ACP, 0, (char *)Product_Name,(int)strlen((char *)Product_Name) + 1, DeviceProductName.GetBuffer(MAX_PATH), MAX_PATH);
                DeviceProductName.ReleaseBuffer();
                DeviceProductName = DeviceProductName.Left(11);

                MultiByteToWideChar(CP_ACP, 0, (char *)global_fileInfor.product_name, (int)strlen((char *)global_fileInfor.product_name) + 1, FileProductName.GetBuffer(MAX_PATH), MAX_PATH);
                FileProductName.ReleaseBuffer();
                FileProductName = FileProductName.Left(11);

				FileProductName.Trim();
				DeviceProductName.Trim();

                FileProductName.MakeUpper();
                DeviceProductName.MakeUpper();
               
                if (FileProductName.CompareNoCase(DeviceProductName) == 0)
                {
                    ShowTFTPMessage = _T("Ready to check the firmware and panel.");
                }
                else
                {
                    ShowTFTPMessage = _T("Firmware: ") +  FileProductName + _T("  ") + _T("Panel: ") + DeviceProductName;
                }
                    
				if (DeviceProductName.CompareNoCase(FileProductName)!=0)
				{
					CString strTip;

					if((FileProductName.CompareNoCase(_T("HUMNET")) == 0) ||
						(FileProductName.CompareNoCase(_T("CO2NET")) == 0) ||
						(FileProductName.CompareNoCase(_T("PSNET")) == 0))
					{

					}
					else if(((DeviceProductName.CompareNoCase(_T("MINI")) == 0) && (FileProductName.CompareNoCase(_T("Minipanel")) == 0)) ||
						((DeviceProductName.CompareNoCase(_T("Minipanel")) == 0) && (FileProductName.CompareNoCase(_T("MINI")) == 0 ))  ||
                        ((DeviceProductName.CompareNoCase(_T("CO2NET")) == 0) && (FileProductName.CompareNoCase(_T("CO2ALL")) == 0)) ||  // 2018 0307 ���˰� CO2NET ��ΪCO2ALL�ˣ�����ƥ�䲻�����޷���д.Ϊ�˼��ݣ�������.
                        ((DeviceProductName.CompareNoCase(_T("UMNET")) == 0) && (FileProductName.CompareNoCase(_T("CO2ALL")) == 0)) ||
                        ((DeviceProductName.CompareNoCase(_T("CO2")) == 0) && (FileProductName.CompareNoCase(_T("CO2 NET")) == 0)) ||
                        ((DeviceProductName.CompareNoCase(_T("PSNET")) == 0) && (FileProductName.CompareNoCase(_T("CO2ALL")) == 0)))
					{

					}
                    else if (n_check_temco_firmware == 0)
                    {
                        Sleep(10);
                    }
					else
					{
						strTip.Format(_T("Your device is %s,but the file is fit for %s"),DeviceProductName.GetBuffer(),FileProductName.GetBuffer());
                        ShowTFTPMessage = strTip;
						CAsyncSocket::OnReceive(nErrorCode);
						return;
					}
				}

				device_has_replay_lan_IP = true;
				ISP_STEP =ISP_SEND_DHCP_COMMAND_HAS_LANIP;
                

			}
		}
		else if (Receive_data_length == 5)
		{
			ShowTFTPMessage = _T("Failed.Reboot or One minute later try again.");
		}
	}
	else if((ISP_STEP ==ISP_SEND_DHCP_COMMAND_HAS_LANIP)&&(has_enter_dhcp_has_lanip_block==true))
	{
		if(Receive_data_length == 31)
		{
			TRACE("ISP_SEND_DHCP_COMMAND_HAS_LANIP Receive 31 byte\n");
			char temp_data[100];
			memset(temp_data,0,sizeof(temp_data));
			memcpy_s(temp_data,sizeof(temp_data),receive_buf,11);//copy the first 11 byte and check
			if(!strcmp(temp_data,"ReceiveDHCP"))
			{
                memset(Byte_ISP_Device_IP, 0, 4);
                memset(Product_Name, 0, 12);

				memcpy_s(Byte_ISP_Device_IP,sizeof(Byte_ISP_Device_IP),receive_buf+11,4);//copy the first 11 byte and check
				memcpy_s(Product_Name,sizeof(Product_Name),receive_buf+15,12);//copy the first 11 byte and check
				CString DeviceProductName ,FileProductName;

                MultiByteToWideChar(CP_ACP, 0, (char *)Product_Name, (int)strlen((char *)Product_Name) + 1, DeviceProductName.GetBuffer(MAX_PATH), MAX_PATH);
                DeviceProductName.ReleaseBuffer();
                DeviceProductName = DeviceProductName.Left(11);

                MultiByteToWideChar(CP_ACP, 0, (char *)global_fileInfor.product_name, (int)strlen((char *)global_fileInfor.product_name) + 1, FileProductName.GetBuffer(MAX_PATH), MAX_PATH);
                FileProductName.ReleaseBuffer();
                FileProductName = FileProductName.Left(11);

                FileProductName.MakeUpper();
                DeviceProductName.MakeUpper();
				FileProductName.Trim();
				DeviceProductName.Trim();

                if (n_check_temco_firmware == 0)
                {
                    ISP_STEP = ISP_Send_TFTP_PAKAGE;
                    CAsyncSocket::OnReceive(nErrorCode);
                    return;
                }

                if (FileProductName.CompareNoCase(DeviceProductName) == 0)
                {
                    ShowTFTPMessage = _T("Check the firmware and panel success.");
                }
                else
                {
                    ShowTFTPMessage = _T("Firmware: ") + FileProductName + _T("  ") + _T("Panel: ") + DeviceProductName;
                }

				if ((DeviceProductName.CompareNoCase(FileProductName)!=0) && (!DeviceProductName.IsEmpty()))
				{
					if((FileProductName.CompareNoCase(_T("HUMNET")) == 0) ||
						(FileProductName.CompareNoCase(_T("CO2NET")) == 0) ||
						(FileProductName.CompareNoCase(_T("PSNET")) == 0))
					{
						device_has_replay_lan_IP = true;
						ISP_STEP =ISP_Send_TFTP_PAKAGE;
						return;
					}
					else if(((DeviceProductName.CompareNoCase(_T("MINI")) == 0) && (FileProductName.CompareNoCase(_T("Minipanel")) == 0)) ||
						((DeviceProductName.CompareNoCase(_T("Minipanel")) == 0) && (FileProductName.CompareNoCase(_T("MINI")) == 0))  ||
                        ((DeviceProductName.CompareNoCase(_T("CO2NET")) == 0) && (FileProductName.CompareNoCase(_T("CO2ALL")) == 0)) ||  //������������.
                        ((DeviceProductName.CompareNoCase(_T("UMNET")) == 0) && (FileProductName.CompareNoCase(_T("CO2ALL")) == 0))  ||
                        ((DeviceProductName.CompareNoCase(_T("CO2")) == 0) && (FileProductName.CompareNoCase(_T("CO2 NET")) == 0)) ||
                        ((DeviceProductName.CompareNoCase(_T("PSNET")) == 0) && (FileProductName.CompareNoCase(_T("CO2ALL")) == 0)))
					{
						ISP_STEP =ISP_Send_TFTP_PAKAGE;
					}
					else
					{
						Failed_Message.Format(_T("Your device is %s,but your bin file is fit for %s"),DeviceProductName.GetBuffer(),FileProductName.GetBuffer());
						ISP_STEP =ISP_Flash_FAILED;
						return;
					}

					CAsyncSocket::OnReceive(nErrorCode);
				}
				else if(!DeviceProductName.IsEmpty())
				{
					ISP_STEP =ISP_Send_TFTP_PAKAGE;
				}
				
			}
		}
		else if (Receive_data_length == 5)
		{
			ShowTFTPMessage = _T("Failed.Reboot or One minute later try again.");
		}
	}
	else if(ISP_STEP ==ISP_Send_TFTP_PAKAGE)
	{
		if(Receive_data_length==4)
		{
			unsigned char temp_data[100];		
			memset(temp_data,0,sizeof(temp_data));
			memcpy_s(temp_data,sizeof(temp_data),receive_buf,11);//copy the first 11 byte and check
			tem_data =  temp_data[2]<<8|temp_data[3];		
			if((temp_data[1]==4)&&(tem_data==package_number))
			{
				next_package_number = package_number +1;
			}
		}
		else if (Receive_data_length == 5)
		{
			ShowTFTPMessage = _T("Failed.Reboot or One minute later try again.");
		}
	}
	else if(ISP_STEP==ISP_Send_TFTP_OVER)
	{
		if(Receive_data_length==4)
		{
			unsigned char temp_data[100];
			memset(temp_data,0,sizeof(temp_data));
			memcpy_s(temp_data,sizeof(temp_data),receive_buf,4);//copy the first 11 byte and check
			if((temp_data[0]==0)&&(temp_data[1]==4)&&(temp_data[2]==0xff)&&(temp_data[3]==0xff))
			{
				ISP_STEP = ISP_Flash_Done;
			}
		}
		else if (Receive_data_length == 5)
		{
			ShowTFTPMessage = _T("Failed.Reboot or One minute later try again.");
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}


void MySocket::OnClose(int nErrorCode) 
{
	CAsyncSocket::OnClose(nErrorCode);
}

void MySocket::OnConnect(int nErrorCode)
{
	CAsyncSocket::OnConnect(nErrorCode);
}



