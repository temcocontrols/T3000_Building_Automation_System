#pragma once
#include "..\..\..\T3000\HttpApiDefine.h"
//extern "C" __declspec(dllexport) int https_get(void);
extern "C" __declspec(dllexport) int https_get(int npid, StrGetProductID &sGetPIDInfo);
extern "C" __declspec(dllexport) int https_post(int npid, StrPostProductID &sPostPIDInfo);
extern "C" __declspec(dllexport) int https_get_file(int npid, StrGetFileInfo &sGetFileInfo);
