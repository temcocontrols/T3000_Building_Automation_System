#pragma once
#include <vector>
using namespace std;

#define  TRUE 1
#define  FALSE 0

typedef struct 
{
	int  errorcode;
	char encode_array[2000];
	char encode_message[255];

}encode_str;

typedef struct
{
	int errorcode;
	char decode_text[4000];
	char decode_message[255];

}decode_str;


struct refresh_net_device_dll
{
	unsigned long nSerial;
	int modbusID;
	unsigned short product_id;
	char ip_address[30];
	int nport;
	float sw_version;
	float hw_version;
	unsigned int object_instance;
	unsigned char panal_number;
	unsigned long parent_serial_number;
	char NetCard_Address[30];
	char show_label_name[50];
	unsigned short bacnetip_port;
	int hardware_info;     //bit  0x74 zigbee   bit1 wifi
	int nprotocol;
	unsigned char  command_version; //65����İ汾�ţ��Ժ�ظ���65���� �иĶ���Ҫ+1 ����Ҫ��Ҫ������ǰ�Ļظ�Э��
	unsigned char  subnet_port;  //�豸������һ���˿ڻظ������ġ� 1- MainPort      2-ZigbeePort      3-SubPort
	unsigned char  subnet_baudrate;   //���豸���õĲ�����; ��֮ǰ����Ĳ�������Ŷ�Ӧ
};

#ifdef _WINDOWS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif /*_WINDOWS*/


extern "C" EXPORT encode_str* Encode_function(char* input_text);
extern "C" EXPORT char* Decode_function(char* input_code, decode_str * decodestr);
extern "C" EXPORT  int ScanByUDPFunc(vector<refresh_net_device_dll> &ret_scan_results);
