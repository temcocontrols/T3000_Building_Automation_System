//******************************************************************************
/*
		This function.cpp added by Fance
		 Open_Testo_USB() function can open the Testo-435
		 ReadTestoDeviceData can read it's data ;

*/
//******************************************************************************
#include "stdafx.h"
#include "DllFunction.h"
#include <stdlib.h>
#include <windows.h>
#include <winioctl.h>
//#include "devintf.h"	// DriverWorks中的一个头文件

#define INITGUID
#include <guiddef.h>
//#include "initguid.h"
#include "setupapi.h"
#include "devintf.h"	// DriverWorks中的一个头文件

#pragma comment(lib,"setupapi.lib")

#define UsbTestDevice_CLASS_GUID \
 { 0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } }
GUID ClassGuid = UsbTestDevice_CLASS_GUID;

extern HANDLE hDevice;
OVERLAPPED m_TESTORead;
//if error  return INVALID_HANDLE_VALUE;

//HANDLE Open_Testo_USB(GUID *pClassGuid, DWORD instance, PDWORD pError)

HANDLE Open_Testo_USB()
{

	HANDLE hDev;
	GUID * pClassGuid = &ClassGuid;
	DWORD	Error;
	PDWORD pError = &Error;

	for (int i=0;i<10;i++)
	{

		CDeviceInterfaceClass DevClass(pClassGuid, pError);

		if (*pError != ERROR_SUCCESS)
			return INVALID_HANDLE_VALUE;

		CDeviceInterface DevInterface(&DevClass,i, pError);

		if (*pError != ERROR_SUCCESS)
			return INVALID_HANDLE_VALUE;

		hDev = CreateFile(
			DevInterface.DevicePath(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		TRACE(_T("%d    "),i);
		TRACE(DevInterface.DevicePath());
		TRACE(_T("\r\n"));
		CString VID_GUID;
		VID_GUID = DevInterface.DevicePath();
		if(VID_GUID.CompareNoCase(_T("\\\\?\\usb#vid_128d&pid_0003#5&23573013&0&1#{a5dcbf10-6530-11d2-901f-00c04fb951ed}")) == 0)
		{
			hDevice = hDev;
			return hDev; 
		}
		else
		{
			continue;
		}
		//+		Sdn	"\\?\usb#vid_128d&pid_0003#5&23573013&0&1#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"	ATL::CStringT<char,StrTraitMFC_DLL<char,ATL::ChTraitsCRT<char> > >

		if (hDev == INVALID_HANDLE_VALUE)
		{
			*pError = GetLastError();
			return hDev; 
		}
	}
	hDevice = INVALID_HANDLE_VALUE;
	return INVALID_HANDLE_VALUE; 

}


int ReadTestoDeviceData(float reveivevalue[]) 
{
	char vbtSend[] = {0x12,0,0,0,1,1,0x55,0xd1,0xb7};
	ULONG	nWritten;
	WriteFile(hDevice, vbtSend, 9, &nWritten, NULL);
	Sleep(100);
	
	char	*buf = NULL;
	ULONG	nRead;
	int	    n = 65;

	CString p;
	buf = new char[n+1];
	memset(buf,0,sizeof(char)*n);

	if (buf == NULL)
	{
		return -1;
	}

	if (hDevice == INVALID_HANDLE_VALUE)
		return -2;
	else
	{
		memset(&m_TESTORead, 0, sizeof(OVERLAPPED));
		if((m_TESTORead.hEvent = CreateEvent(NULL,true,false,_T("TestoRead")))==NULL)
			return -2; 
		m_TESTORead.Offset = 0;
		m_TESTORead.OffsetHigh = 0;

		DWORD receive_length;
		int fState = ReadFile(hDevice, buf, n, &nRead, &m_TESTORead);
		if(!fState)// 不支持重叠	
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(hDevice,&m_TESTORead,&receive_length,1);// 等待
			}
			else
				receive_length=0;
		}
	}

	int dwNumChannels;
	dwNumChannels = buf[6];
	for (DWORD dwChannelIndex = 0; dwChannelIndex < dwNumChannels; dwChannelIndex++)
	{
		DWORD dwIndex = 7+dwChannelIndex*7;
		float fVal = *reinterpret_cast<float*>(&buf[dwIndex]);
		reveivevalue[dwChannelIndex] = fVal;
		printf("Val %d %.2f Unit %u\n",dwChannelIndex,fVal,buf[dwIndex+4]);
	}
	if(buf)
		delete[] buf;
	buf = NULL;

	return 1;
}
