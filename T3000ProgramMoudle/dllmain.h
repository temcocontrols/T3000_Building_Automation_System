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
	unsigned char  command_version; //65命令的版本号，以后回复的65命令 有改动就要+1 ，主要是要兼容以前的回复协议
	unsigned char  subnet_port;  //设备属于哪一个端口回复出来的。 1- MainPort      2-ZigbeePort      3-SubPort
	unsigned char  subnet_baudrate;   //子设备所用的波特率; 和之前定义的波特率序号对应
};


extern "C" __declspec(dllexport) encode_str* Encode_function(char* input_text);
extern "C" __declspec(dllexport) char* Decode_function(char* input_code, decode_str * decodestr);
extern "C" __declspec(dllexport)  int ScanByUDPFunc(vector<refresh_net_device_dll> &ret_scan_results);
