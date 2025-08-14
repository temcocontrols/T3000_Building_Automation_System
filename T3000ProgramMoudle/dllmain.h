#pragma once
#include <vector>
using namespace std;
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
	// Version number of command 65, future replies to command 65 need to increment by 1 when modified, mainly for compatibility with previous reply protocols
	unsigned char  command_version; //65����İ汾�ţ��Ժ�ظ���65���� �иĶ���Ҫ+1 ����Ҫ��Ҫ������ǰ�Ļظ�Э��
	// Which port the device belongs to for reply. 1- MainPort 2-ZigbeePort 3-SubPort
	unsigned char  subnet_port;  //�豸������һ���˿ڻظ������ġ� 1- MainPort      2-ZigbeePort      3-SubPort
	// Baud rate used by sub-devices; corresponds to previously defined baud rate sequence numbers
	unsigned char  subnet_baudrate;   //���豸���õĲ�����; ��֮ǰ����Ĳ�������Ŷ�Ӧ
};


extern "C" __declspec(dllexport) encode_str* Encode_function(char* input_text);
extern "C" __declspec(dllexport) char* Decode_function(char* input_code, decode_str * decodestr);
extern "C" __declspec(dllexport)  int ScanByUDPFunc(vector<refresh_net_device_dll> &ret_scan_results);
