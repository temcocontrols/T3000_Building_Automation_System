#pragma  once
#include "stdafx.h"
//#include "define.h"
   HANDLE Enum_USB_Device(CStringArray &EnumUSB);
   void close_testo();
   HANDLE Open_Testo_USB(CString DEVICEINFO);
   int ReadTestoDeviceData(float reveivevalue[]);