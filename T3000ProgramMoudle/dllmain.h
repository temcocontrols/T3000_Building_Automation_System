#pragma once
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

extern "C" __declspec(dllexport) encode_str* Encode_function(char* input_text);
extern "C" __declspec(dllexport) char* Decode_function(char* input_code, decode_str * decodestr);
