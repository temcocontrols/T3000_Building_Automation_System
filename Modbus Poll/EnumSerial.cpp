/************************************************************************* 
* Serial port enumeration routines
*
* The EnumSerialPort function will populate an array of SSerInfo structs,
* each of which contains information about one serial port present in
* the system. Note that this code must be linked with setupapi.lib,
* which is included with the Win32 SDK.
*
* by Zach Gorman <gormanjz@hotmail.com>
*
* Copyright (c) 2002 Archetype Auction Software, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following condition is
* met: Redistributions of source code must retain the above copyright
* notice, this condition and the following disclaimer.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL ARCHETYPE AUCTION SOFTWARE OR ITS
* AFFILIATES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************/

// For MFC
#include "stdafx.h"

// The next 3 includes are needed for serial port enumeration
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>

#include "EnumSerial.h"

// The following define is from ntddser.h in the DDK. It is also
// needed for serial port enumeration.
#ifndef GUID_CLASS_COMPORT
DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, \
			0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
#endif

//---------------------------------------------------------------
// Helpers for enumerating the available serial ports.
// These throw a CString on failure, describing the nature of
// the error that occurred.

void EnumPortsWdm(CArray<SSerInfo,SSerInfo&> &asi);
void EnumPortsWNt4(CArray<SSerInfo,SSerInfo&> &asi);
void EnumPortsW9x(CArray<SSerInfo,SSerInfo&> &asi);
void SearchPnpKeyW9x(HKEY hkPnp, BOOL bUsbDevice,
					 CArray<SSerInfo,SSerInfo&> &asi);


//---------------------------------------------------------------
// Routine for enumerating the available serial ports.
// Throws a CString on failure, describing the error that
// occurred. If bIgnoreBusyPorts is TRUE, ports that can't
// be opened for read/write access are not included.

void EnumSerialPorts(CArray<SSerInfo,SSerInfo&> &asi, BOOL bIgnoreBusyPorts)
{
	// Clear the output array
	asi.RemoveAll();

	// Use different techniques to enumerate the available serial
	// ports, depending on the OS we're using
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (!::GetVersionEx(&vi)) {
		CString str;
		str.Format(L"Could not get OS version. (err=%lx)",
			GetLastError());
		throw str;
	}
	// Handle windows 9x and NT4 specially
	if (vi.dwMajorVersion < 5) {
		if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
			EnumPortsWNt4(asi);
		else
			EnumPortsW9x(asi);
	}
	else {
		// Win2k and later support a standard API for
		// enumerating hardware devices.
		EnumPortsWdm(asi);
	}

	for (int ii=0; ii<asi.GetSize(); ii++)
	{
		SSerInfo& rsi = asi[ii];    
		if (bIgnoreBusyPorts) {
			// Only display ports that can be opened for read/write
			HANDLE hCom = CreateFile(rsi.strDevPath,
				GENERIC_READ | GENERIC_WRITE,
				0,    /* comm devices must be opened w/exclusive-access */
				NULL, /* no security attrs */
				OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
				0,    /* not overlapped I/O */
				NULL  /* hTemplate must be NULL for comm devices */
				);
			if (hCom == INVALID_HANDLE_VALUE) {
				// It can't be opened; remove it.
				asi.RemoveAt(ii);
				ii--;               
				continue;
			}
			else {
				// It can be opened! Close it and add it to the list
				::CloseHandle(hCom);
			}
		}

		// Come up with a name for the device.
		// If there is no friendly name, use the port name.
		if (rsi.strFriendlyName.IsEmpty())
			rsi.strFriendlyName = rsi.strPortName;

		// If there is no description, try to make one up from
		// the friendly name.
		if (rsi.strPortDesc.IsEmpty()) {
			// If the port name is of the form "ACME Port (COM3)"
			// then strip off the " (COM3)"
			rsi.strPortDesc = rsi.strFriendlyName;
			int startdex = rsi.strPortDesc.Find(L" (");
			int enddex = rsi.strPortDesc.Find(L")");
			if (startdex > 0 && enddex == 
				(rsi.strPortDesc.GetLength()-1))
				rsi.strPortDesc = rsi.strPortDesc.Left(startdex);
		}
	}
}

// Helpers for EnumSerialPorts

void EnumPortsWdm(CArray<SSerInfo,SSerInfo&> &asi)
{
	CString strErr;
	// Create a device information set that will be the container for 
	// the device interfaces.
	GUID *guidDev = (GUID*) &GUID_CLASS_COMPORT;

	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;

	try {
		hDevInfo = SetupDiGetClassDevs( guidDev,
			NULL,
			NULL,
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
			);

		if(hDevInfo == INVALID_HANDLE_VALUE) 
		{
			strErr.Format(_T("SetupDiGetClassDevs failed. (err=%lx)"),
				GetLastError());
			throw strErr;
		}

		// Enumerate the serial ports
		BOOL bOk = TRUE;
		SP_DEVICE_INTERFACE_DATA ifcData;
		DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
		pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];
		// This is required, according to the documentation. Yes,
		// it's weird.
		ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		for (DWORD ii=0; bOk; ii++) {
			bOk = SetupDiEnumDeviceInterfaces(hDevInfo,
				NULL, guidDev, ii, &ifcData);
			if (bOk) {
				// Got a device. Get the details.
				SP_DEVINFO_DATA devdata = {sizeof(SP_DEVINFO_DATA)};
				bOk = SetupDiGetDeviceInterfaceDetail(hDevInfo,
					&ifcData, pDetData, dwDetDataSize, NULL, &devdata);
				if (bOk) {
					CString strDevPath(pDetData->DevicePath);
					// Got a path to the device. Try to get some more info.
					TCHAR fname[256];
					TCHAR desc[256];
					BOOL bSuccess = SetupDiGetDeviceRegistryProperty(
						hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL,
						(PBYTE)fname, sizeof(fname), NULL);
					bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(
						hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL,
						(PBYTE)desc, sizeof(desc), NULL);
					BOOL bUsbDevice = FALSE;
					TCHAR locinfo[256];
					if (SetupDiGetDeviceRegistryProperty(
						hDevInfo, &devdata, SPDRP_LOCATION_INFORMATION, NULL,
						(PBYTE)locinfo, sizeof(locinfo), NULL))
					{
						// Just check the first three characters to determine
						// if the port is connected to the USB bus. This isn't
						// an infallible method; it would be better to use the
						// BUS GUID. Currently, Windows doesn't let you query
						// that though (SPDRP_BUSTYPEGUID seems to exist in
						// documentation only).
						 
						bUsbDevice = (strncmp((char *)locinfo, "USB", 3)==0);
					}
					if (bSuccess) {
						// Add an entry to the array
						SSerInfo si;
						si.strDevPath = strDevPath;
						si.strFriendlyName = fname;
						si.strPortDesc = desc;
						si.bUsbDevice = bUsbDevice;
					 
						asi.Add(si);
					}

				}
				else {
					strErr.Format(L"SetupDiGetDeviceInterfaceDetail failed. (err=%lx)",
						GetLastError());
					throw strErr;
				}
			}
			else {
				DWORD err = GetLastError();
				if (err != ERROR_NO_MORE_ITEMS) {
					strErr.Format(L"SetupDiEnumDeviceInterfaces failed. (err=%lx)", err);
					throw strErr;
				}
			}
		}
	}
	catch (CString strCatchErr) {
		strErr = strCatchErr;
	}

	if (pDetData != NULL)
		delete [] (char*)pDetData;
	if (hDevInfo != INVALID_HANDLE_VALUE)
		SetupDiDestroyDeviceInfoList(hDevInfo);

	if (!strErr.IsEmpty())
		throw strErr;
}

void EnumPortsWNt4(CArray<SSerInfo,SSerInfo&> &asi)
{
	// NT4's driver model is totally different, and not that
	// many people use NT4 anymore. Just try all the COM ports
	// between 1 and 16
	SSerInfo si;
	for (int ii=1; ii<=16; ii++) {
		CString strPort;
		strPort.Format(L"COM%d",ii);
		si.strDevPath = CString("\\\\.\\") + strPort;
		si.strPortName = strPort;
		asi.Add(si);
	}
}

void EnumPortsW9x(CArray<SSerInfo,SSerInfo&> &asi)
{
	// Look at all keys in HKLM\Enum, searching for subkeys named
	// *PNP0500 and *PNP0501. Within these subkeys, search for
	// sub-subkeys containing value entries with the name "PORTNAME"
	// Search all subkeys of HKLM\Enum\USBPORTS for PORTNAME entries.

	// First, open HKLM\Enum
	HKEY hkEnum = NULL;
	HKEY hkSubEnum = NULL;
	HKEY hkSubSubEnum = NULL;
	
	try {
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Enum", 0, KEY_READ,
			&hkEnum) != ERROR_SUCCESS)
			throw CString("Could not read from HKLM\\Enum");

		// Enumerate the subkeys of HKLM\Enum
  		char acSubEnum[128];
  		DWORD dwSubEnumIndex = 0;
  		DWORD dwSize = sizeof(acSubEnum);
		CString strTemp_acSubEnum;
	 
	 
		while (RegEnumKeyEx(hkEnum, dwSubEnumIndex++, strTemp_acSubEnum.GetBuffer(), &dwSize,
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			strTemp_acSubEnum.ReleaseBuffer();
			HKEY hkSubEnum = NULL;
			if (RegOpenKeyEx(hkEnum, strTemp_acSubEnum.GetBuffer(), 0, KEY_READ,
				&hkSubEnum) != ERROR_SUCCESS)
			{
				strTemp_acSubEnum.ReleaseBuffer();
				throw CString("Could not read from HKLM\\Enum\\") + strTemp_acSubEnum;
			}
			

			// Enumerate the subkeys of HKLM\Enum\*\, looking for keys
			// named *PNP0500 and *PNP0501 (or anything in USBPORTS)
			BOOL bUsbDevice = (strTemp_acSubEnum.CompareNoCase(L"USBPORTS")==0);
			char acSubSubEnum[128];
			dwSize = sizeof(acSubSubEnum);  // set the buffer size
			DWORD dwSubSubEnumIndex = 0;
			CString strTemp_acSubSubEnum;
			while (RegEnumKeyEx(hkSubEnum, dwSubSubEnumIndex++, strTemp_acSubSubEnum.GetBuffer(),
				&dwSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
			{
				strTemp_acSubSubEnum.ReleaseBuffer();
				BOOL bMatch = (strTemp_acSubSubEnum.CompareNoCase(L"*PNP0500")==0 ||
					strTemp_acSubSubEnum.CompareNoCase(L"*PNP0501")==0 ||
					bUsbDevice);
				if (bMatch) {
					HKEY hkSubSubEnum = NULL;
					if (RegOpenKeyEx(hkSubEnum, strTemp_acSubSubEnum.GetBuffer(), 0, KEY_READ,
						&hkSubSubEnum) != ERROR_SUCCESS)
					{
						strTemp_acSubSubEnum.ReleaseBuffer();
						throw CString("Could not read from HKLM\\Enum\\") +
							strTemp_acSubEnum + L"\\" + strTemp_acSubSubEnum;
					}
						
					SearchPnpKeyW9x(hkSubSubEnum, bUsbDevice, asi);
					RegCloseKey(hkSubSubEnum);
					hkSubSubEnum = NULL;
				}

				dwSize = sizeof(acSubSubEnum);  // restore the buffer size
			}

			RegCloseKey(hkSubEnum);
			hkSubEnum = NULL;
			dwSize = sizeof(acSubEnum); // restore the buffer size
		}
	}
	catch (CString strError) {
		if (hkEnum != NULL)
			RegCloseKey(hkEnum);
		if (hkSubEnum != NULL)
			RegCloseKey(hkSubEnum);
		if (hkSubSubEnum != NULL)
			RegCloseKey(hkSubSubEnum);
		throw strError;
	}

	RegCloseKey(hkEnum);
}

void SearchPnpKeyW9x(HKEY hkPnp, BOOL bUsbDevice,
					 CArray<SSerInfo,SSerInfo&> &asi)
{
	// Enumerate the subkeys of the given PNP key, looking for values with
	// the name "PORTNAME"
	// First, open HKLM\Enum
	HKEY hkSubPnp = NULL;

	try {
		// Enumerate the subkeys of HKLM\Enum\*\PNP050[01]
		char acSubPnp[128];
		DWORD dwSubPnpIndex = 0;
		DWORD dwSize = sizeof(acSubPnp);
		CString strTemp_acSubPnp;
		while (RegEnumKeyEx(hkPnp, dwSubPnpIndex++, strTemp_acSubPnp.GetBuffer(), &dwSize,
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			HKEY hkSubPnp = NULL;
			strTemp_acSubPnp.ReleaseBuffer();
			if (RegOpenKeyEx(hkPnp, strTemp_acSubPnp, 0, KEY_READ,
				&hkSubPnp) != ERROR_SUCCESS)
			{

				throw CString("Could not read from HKLM\\Enum\\...\\")
					+ strTemp_acSubPnp;
			}
				

			// Look for the PORTNAME value
			char acValue[128];
			dwSize = sizeof(acValue);
			if (RegQueryValueEx(hkSubPnp, L"PORTNAME", NULL, NULL, (BYTE*)acValue,
				&dwSize) == ERROR_SUCCESS)
			{
				CString strPortName(acValue);

				// Got the portname value. Look for a friendly name.
				CString strFriendlyName;
				dwSize = sizeof(acValue);
				if (RegQueryValueEx(hkSubPnp, L"FRIENDLYNAME", NULL, NULL, (BYTE*)acValue,
					&dwSize) == ERROR_SUCCESS)
					strFriendlyName = acValue;

				// Prepare an entry for the output array.
				SSerInfo si;
				si.strDevPath = CString("\\\\.\\") + strPortName;
				si.strPortName = strPortName;
				si.strFriendlyName = strFriendlyName;
				si.bUsbDevice = bUsbDevice;

				// Overwrite duplicates.
				BOOL bDup = FALSE;
				for (int ii=0; ii<asi.GetSize() && !bDup; ii++)
				{
					if (asi[ii].strPortName == strPortName) {
						bDup = TRUE;
						asi[ii] = si;
					}
				}
				if (!bDup) {
					// Add an entry to the array
					asi.Add(si);
				}
			}

			RegCloseKey(hkSubPnp);
			hkSubPnp = NULL;
			dwSize = sizeof(acSubPnp);  // restore the buffer size
		}
	}
	catch (CString strError) {
		if (hkSubPnp != NULL)
			RegCloseKey(hkSubPnp);
		throw strError;
	}
}
