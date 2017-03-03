/*
Module : SerialPort.cpp
Purpose: Implementation for an C++ wrapper class for serial ports
Created: PJN / 31-05-1999
History: PJN / 03-06-1999 1. Fixed problem with code using CancelIo which does not exist on 95.
                          2. Fixed leaks which can occur in sample app when an exception is thrown
         PJN / 16-06-1999 1. Fixed a bug whereby CString::ReleaseBuffer was not being called in 
                             CSerialException::GetErrorMessage
         PJN / 29-09-1999 1. Fixed a simple copy and paste bug in CSerialPort::SetDTR
         PJN / 08-05-2000 1. Fixed an unreferrenced variable in CSerialPort::GetOverlappedResult in VC 6
         PJN / 10-12-2000 1. Made class destructor virtual
         PJN / 15-01-2001 1. Attach method now also allows you to specify whether the serial port
                          is being attached to in overlapped mode
                          2. Removed some ASSERTs which were unnecessary in some of the functions
                          3. Updated the Read method which uses OVERLAPPED IO to also return the bytes
                          read. This allows calls to WriteFile with a zeroed overlapped structure (This
                          is required when dealing with TAPI and serial communications)
                          4. Now includes copyright message in the source code and documentation.
         PJN / 24-03-2001 1. Added a BytesWaiting method
         PJN / 04-04-2001 1. Provided an overriden version of BytesWaiting which specifies a timeout
         PJN / 23-04-2001 1. Fixed a memory leak in DataWaiting method
         PJN / 01-05-2002 1. Fixed a problem in Open method which was failing to initialize the DCB 
                          structure incorrectly, when calling GetState. Thanks to Ben Newson for this fix.
         PJN / 29-05-2002 1. Fixed an problem where the GetProcAddress for CancelIO was using the
                          wrong calling convention
         PJN / 07-08-2002 1. Changed the declaration of CSerialPort::WaitEvent to be consistent with the
                          rest of the methods in CSerialPort which can operate in "OVERLAPPED" mode. A note
                          about the usage of this: If the method succeeds then the overlapped operation
                          has completed synchronously and there is no need to do a WaitForSingle/MultipleObjects.
                          If any other unexpected error occurs then a CSerialException will be thrown. See
                          the implementation of the CSerialPort::DataWaiting which has been rewritten to use
                          this new design pattern. Thanks to Serhiy Pavlov for spotting this inconsistency.
         PJN / 20-09-2002 1. Addition of an additional ASSERT in the internal _OnCompletion function.
                          2. Addition of an optional out parameter to the Write method which operates in 
                          overlapped mode. Thanks to Kevin Pinkerton for this addition.
         PJN / 10-04-2006 1. Updated copyright details.
                          2. Addition of a CSERIALPORT_EXT_CLASS and CSERIALPORT_EXT_API macros which makes 
                          the class easier to use in an extension dll.
                          3. Removed derivation of CSerialPort from CObject as it was not really needed.
                          4. Fixed a number of level 4 warnings in the sample app.
                          5. Reworked the overlapped IO methods to expose the LPOVERLAPPED structure to client 
                          code.
                          6. Updated the documentation to use the same style as the web site.
                          7. Did a spell check of the HTML documentation.
                          8. Updated the documentation on possible blocking in Read/Ex function. Thanks to 
                          D Kerrison for reporting this issue.
                          9. Fixed a minor issue in the sample app when the code is compiled using /Wp64
         PJN / 02-06-2006 1. Removed the bOverlapped as a member variable from the class. There was no 
                          real need for this setting, since the SDK functions will perform their own 
                          checking of how overlapped operations should
                          2. Fixed a bug in GetOverlappedResult where the code incorrectly checking against
                          the error ERROR_IO_PENDING instead of ERROR_IO_INCOMPLETE. Thanks to Sasho Darmonski
                          for reporting this bug.
                          3. Reviewed all TRACE statements for correctness.
         PJN / 05-06-2006 1. Fixed an issue with the creation of the internal event object. It was incorrectly
                          being created as an auto-reset event object instead of a manual reset event object.
                          Thanks to Sasho Darmonski for reporting this issue.
         PJN / 24-06-2006 1. Fixed some typos in the history list. Thanks to Simon Wong for reporting this.
                          2. Made the class which handles the construction of function pointers at runtime a
                          member variable of CSerialPort
                          3. Made AfxThrowSerialPortException part of the CSerialPort class. Thanks to Simon Wong
                          for reporting this.
                          4. Removed the unnecessary CSerialException destructor. Thanks to Simon Wong for 
                          reporting this.
                          5. Fixed a minor error in the TRACE text in CSerialPort::SetDefaultConfig. Again thanks
                          to Simon Wong for reporting this. 
                          6. Code now uses new C++ style casts rather than old style C casts where necessary. 
                          Again thanks to Simon Wong for reporting this.
                          7. CSerialException::GetErrorMessage now uses the strsafe functions. This does mean 
                          that the code now requires the Platform SDK if compiled using VC 6.
         PJN / 25-06-2006 1. Combined the functionality of the CSerialPortData class into the main CSerialPort class.
                          2. Renamed AfxThrowSerialPortException to ThrowSerialPortException and made the method
                          public.
         PJN / 05-11-2006 1. Minor update to stdafx.h of sample app to avoid compiler warnings in VC 2005.
                          2. Reverted the use of the strsafe.h header file. Instead now the code uses the VC 2005
                          Safe CRT and if this is not available, then we fail back to the standard CRT.
         PJN / 25-01-2007 1. Minor update to remove strsafe.h from stdafx.h of the sample app.
                          2. Updated copyright details.
         PJN / 24-12-2007 1. CSerialException::GetErrorMessage now uses the FORMAT_MESSAGE_IGNORE_INSERTS flag. For more information please see Raymond
                          Chen's blog at http://blogs.msdn.com/oldnewthing/archive/2007/11/28/6564257.aspx. Thanks to Alexey Kuznetsov for reporting this
                          issue.
                          2. Simplified the code in CSerialException::GetErrorMessage somewhat.
                          3. Optimized the CSerialException constructor code.
                          4. Code now uses newer C++ style casts instead of C style casts.
                          5. Reviewed and updated all the TRACE logging in the module
                          6. Replaced all calls to ZeroMemory with memset
         PJN / 30-12-2007 1. Updated the sample app to clean compile on VC 2008
                          2. CSerialException::GetErrorMessage now uses Checked::tcsncpy_s if compiled using VC 2005 or later.
         PJN / 18-05-2008 1. Updated copyright details.
                          2. Changed the actual values for Parity enum so that they are consistent with the Parity define values in the Windows SDK 
                          header file WinBase.h. This avoids the potential issue where you use the CSerialPort enum parity values in a call to the
                          raw Win32 API calls. Thanks to Robert Krueger for reporting this issue.
         PJN / 21-06-2008 1. Code now compiles cleanly using Code Analysis (/analyze)
                          2. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          3. The code now only supports VC 2005 or later. 
                          4. CSerialPort::Read, Write, GetOverlappedResult & WaitEvent now throw an exception irrespective of whether the last error
                          is ERROR_IO_PENDING or not
                          5. Replaced all calls to ZeroMemory with memset
         PJN / 04-07-2008 1. Provided a version of the Open method which takes a string instead of a numeric port number value. This allows the code
                          to support some virtual serial port packages which do not use device names of the form "COM%d". Thanks to David Balazic for 
                          suggesting this addition.
         PJN / 25-01-2012 1. Updated copyright details.
                          2. Updated sample app and class to compile cleanly on VC 2010 and later.
         PJN / 28-02-2015 1. Updated sample project settings to more modern default values.
                          2. Updated copyright details.
                          3. Reworked the CSerialPort and CSerialPortException classes to optionally compile without MFC. By default 
                          the classes now use STL classes and idioms but if you define CSERIALPORT_MFC_EXTENSIONS the classes will 
                          revert back to the MFC behaviour.
                          4. Remove logic to use GetProcAddress to access CancelIO functionality.
                          5. Updated the code to clean compile on VC 2013
                          6. Added SAL annotations to all the code
                          7. Addition of a GetDefaultConfig method which takes a string
                          8. Addition of a SetDefaultConfig method which takes a string
         PJN / 26-04-2015 1. Removed unnecessary inclusion of WinError.h
                          2. Removed the CSerialPort::DataWaiting method as it depends on the port being open in overlapped mode. Instead client code
                          can simply call CSerialPort::WaitEvent directly themselves. Removing this method also means that the CSerialPort::m_hEvent
                          handle has not also been removed.
                          3. The CSerialPort::WriteEx method has been reworked to expose all the parameters of the underlying WriteFileEx API. This
                          rework also fixes a memory leak in WriteEx which can sometimes occur. This reworks also means that the 
                          CSerialPort::_OnCompletion and CSerialPort::_OnCompletion methods have been removed. Thanks to Yufeng Huang for reporting 
                          this issue.
                          4. The CSerialPort::ReadEx method has been reworked to expose all the parameters of the underlying ReadFileEx API. This
                          rework also fixes a memory leak in ReadEx which can sometimes occur. This reworks also means that the 
                          CSerialPort::_OnCompletion and CSerialPort::_OnCompletion methods have been removed. Thanks to Yufeng Huang for reporting 
                          this issue.
         PJN / 17-12-2015 1. Verified the code compiles cleanly on VC 2015.
         PJN / 22-05-2016 1. Updated copyright details.
                          2. Fixed some typos in SerialPort.h, SerialPort.cpp and SerialPort.htm where CSERIALPORT_MFC_EXTENSTIONS was being used 
                          instead of CSERIALPORT_MFC_EXTENSIONS. Thanks to Nicholas Buse for reporting this issue.

Copyright (c) 1996 - 2016 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////////////// Includes ////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

#ifndef __ATLBASE_H__
#pragma message("To avoid this message, please put atlbase.h in your pre compiled header (normally stdafx.h)")
#include <atlbase.h>
#endif //#ifndef __ATLBASE_H__


///////////////////////////////// Defines /////////////////////////////////////

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
#define new DEBUG_NEW
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS


//////////////////////////////// Implementation ///////////////////////////////

#if _MSC_VER >= 1700
BOOL CSerialException::GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext)
#else	
BOOL CSerialException::GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpszError, __in UINT nMaxError, __out_opt PUINT pnHelpContext)
#endif
{
  //Validate our parameters
  ATLASSERT(lpszError != NULL);
    
  if (pnHelpContext != NULL)
    *pnHelpContext = 0;
    
  //What will be the return value from this function (assume the worst)
  BOOL bSuccess = FALSE;

  LPTSTR lpBuffer;
  DWORD dwReturn = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL,  m_dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
                                 reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

  if (dwReturn == 0)
    *lpszError = _T('\0');
  else
  {
    bSuccess = TRUE;
    Checked::tcsncpy_s(lpszError, nMaxError, lpBuffer, _TRUNCATE);  
    LocalFree(lpBuffer);
  }

  return bSuccess;
}

#ifdef CSERIALPORT_MFC_EXTENSIONS
CString CSerialException::GetErrorMessage()
{
  CString rVal;
  LPTSTR pstrError = rVal.GetBuffer(4096);
  GetErrorMessage(pstrError, 4096, NULL);
  rVal.ReleaseBuffer();
  return rVal;
}
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

CSerialException::CSerialException(DWORD dwError) : m_dwError(dwError)
{
}

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
void CSerialException::Dump(_In_ CDumpContext& dc) const
{
  CObject::Dump(dc);

  dc << _T("m_dwError = ") << m_dwError << _T("\n");
}
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS


CSerialPort::CSerialPort() : m_hComm(INVALID_HANDLE_VALUE)
{
}

CSerialPort::~CSerialPort()
{
  Close();
}

void CSerialPort::ThrowSerialException(_In_ DWORD dwError)
{
  if (dwError == 0)
    dwError = ::GetLastError();

  ATLTRACE(_T("Warning: throwing CSerialException for error %d\n"), dwError);
#ifdef CSERIALPORT_MFC_EXTENSIONS
  CSerialException* pException = new CSerialException (dwError);
  THROW(pException);
 #else
  CSerialException e(dwError);
  throw e;
 #endif //#ifdef CSERIALPORT_MFC_EXTENSIONS
}

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
void CSerialPort::Dump(CDumpContext& dc) const
{
  dc << _T("m_hComm = ") << m_hComm << _T("\n");
}
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

void CSerialPort::Open(_In_z_ LPCTSTR pszPort, _In_ DWORD dwBaud, _In_ Parity parity, _In_ BYTE DataBits, _In_ StopBits stopBits, _In_ FlowControl fc, _In_ BOOL bOverlapped)
{
  Close(); //In case we are already open

  //Call CreateFile to open the comms port
  m_hComm = CreateFile(pszPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, bOverlapped ? FILE_FLAG_OVERLAPPED : 0, NULL);
  if (m_hComm == INVALID_HANDLE_VALUE)
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Open, Failed to open the comms port, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  //Get the current state prior to changing it
  DCB dcb;
  dcb.DCBlength = sizeof(DCB);
  GetState(dcb);

  //Setup the baud rate
  dcb.BaudRate = dwBaud; 

  //Setup the Parity
  switch (parity)
  {
    case EvenParity:  
    {
      dcb.Parity = EVENPARITY;
      break;
    }
    case MarkParity:  
    {
      dcb.Parity = MARKPARITY;
      break;
    }
    case NoParity:    
    {
      dcb.Parity = NOPARITY;
      break;
    }
    case OddParity:   
    {
      dcb.Parity = ODDPARITY;
      break;
    }
    case SpaceParity: 
    {
      dcb.Parity = SPACEPARITY;
      break;
    }
    default:          
    {
      ATLASSERT(FALSE);            
      break;
    }
  }

  //Setup the data bits
  dcb.ByteSize = DataBits;

  //Setup the stop bits
  switch (stopBits)
  {
    case OneStopBit:           
    {
      dcb.StopBits = ONESTOPBIT; 
      break;
    }
    case OnePointFiveStopBits: 
    {
      dcb.StopBits = ONE5STOPBITS;
      break;
    }
    case TwoStopBits:          
    {
      dcb.StopBits = TWOSTOPBITS;
      break;
    }
    default:                   
    {
      ATLASSERT(FALSE);
      break;
    }
  }

  //Setup the flow control 
  dcb.fDsrSensitivity = FALSE;
  switch (fc)
  {
    case NoFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case CtsRtsFlowControl:
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case CtsDtrFlowControl:
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case DsrRtsFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = TRUE;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case DsrDtrFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = TRUE;
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case XonXoffFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fOutX = TRUE;
      dcb.fInX = TRUE;
      dcb.XonChar = 0x11;
      dcb.XoffChar = 0x13;
      dcb.XoffLim = 100;
      dcb.XonLim = 100;
      break;
    }
    default:
    {
      ATLASSERT(FALSE);
      break;
    }
  }
  
  //Now that we have all the settings in place, make the changes
  SetState(dcb);
}

void CSerialPort::Open(_In_ int nPort, _In_ DWORD dwBaud, _In_ Parity parity, _In_ BYTE DataBits, _In_ StopBits stopBits, _In_ FlowControl fc, _In_ BOOL bOverlapped)
{
  //Form the string version of the port number
  TCHAR szPort[12];
  _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("\\\\.\\COM%d"), nPort);

  //Delegate the work to the other version of Open
  Open(szPort, dwBaud, parity, DataBits, stopBits, fc, bOverlapped);
}

void CSerialPort::Close()
{
  if (IsOpen())
  {
    //Close down the comms port
    CloseHandle(m_hComm);
    m_hComm = INVALID_HANDLE_VALUE;
  }
}

void CSerialPort::Attach(_In_ HANDLE hComm)
{
  Close();

  //Validate our parameters, now that the port has been closed
  ATLASSERT(m_hComm == INVALID_HANDLE_VALUE);

  m_hComm = hComm;  
}

HANDLE CSerialPort::Detach()
{
  //What will be the return value from this function
  HANDLE hComm = m_hComm;

  m_hComm = INVALID_HANDLE_VALUE;

  return hComm;
}

DWORD CSerialPort::Read(_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwBytesRead = 0;
  if (!ReadFile(m_hComm, lpBuffer, dwNumberOfBytesToRead, &dwBytesRead, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Read, Failed in call to ReadFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  return dwBytesRead;
}

void CSerialPort::Read(_Out_writes_bytes_to_opt_(dwNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _In_ OVERLAPPED& overlapped, _Inout_opt_ DWORD* lpNumberOfBytesRead)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ReadFile(m_hComm, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead, &overlapped))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Read, Failed in call to ReadFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ReadEx(_Out_writes_bytes_opt_(dwNumberOfBytesToRead) __out_data_source(FILE) LPVOID lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ReadFileEx(m_hComm, lpBuffer, dwNumberOfBytesToRead, lpOverlapped, lpCompletionRoutine))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::ReadEx, Failed in call to ReadFileEx, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

DWORD CSerialPort::Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwBytesWritten = 0;
  if (!WriteFile(m_hComm, lpBuffer, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Write, Failed in call to WriteFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  return dwBytesWritten;
}

void CSerialPort::Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _In_ OVERLAPPED& overlapped, _Out_opt_ DWORD* lpNumberOfBytesWritten)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!WriteFile(m_hComm, lpBuffer, dwNumberOfBytesToWrite, lpNumberOfBytesWritten, &overlapped))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Write, Failed in call to WriteFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::WriteEx(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) LPCVOID lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!WriteFileEx(m_hComm, lpBuffer, dwNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::WriteEx, Failed in call to WriteFileEx, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetOverlappedResult(_In_ OVERLAPPED& overlapped, _Out_ DWORD& dwBytesTransferred, _In_ BOOL bWait)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!::GetOverlappedResult(m_hComm, &overlapped, &dwBytesTransferred, bWait))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetOverlappedResult, Failed in call to GetOverlappedResult, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::CancelIo()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!::CancelIo(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("Failed in call to CancelIO, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

DWORD CSerialPort::BytesWaiting()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  //Check to see how many characters are unread
  COMSTAT stat;
  GetStatus(stat);
  return stat.cbInQue;
}

void CSerialPort::TransmitChar(_In_ char cChar)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!TransmitCommChar(m_hComm, cChar))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::TransmitChar, Failed in call to TransmitCommChar, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetConfig(_In_ COMMCONFIG& config)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!GetCommConfig(m_hComm, &config, &dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetConfig, Failed in call to GetCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetConfig(_In_ COMMCONFIG& config)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!SetCommConfig(m_hComm, &config, dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetConfig, Failed in call to SetCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetBreak()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommBreak(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetBreak, Failed in call to SetCommBreak, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ClearBreak()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ClearCommBreak(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::ClearBreak, Failed in call to SetCommBreak, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ClearError(_Out_ DWORD& dwErrors)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ClearCommError(m_hComm, &dwErrors, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::ClearError, Failed in call to ClearCommError, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetDefaultConfig(_In_ int nPort, _Out_ COMMCONFIG& config)
{
  //Create the device name as a string
  TCHAR szPort[12];
  _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("COM%d"), nPort);

  return GetDefaultConfig(szPort, config);
}

void CSerialPort::GetDefaultConfig(_In_z_ LPCTSTR pszPort, _Out_ COMMCONFIG& config)
{
  DWORD dwSize = sizeof(COMMCONFIG);
  if (!GetDefaultCommConfig(pszPort, &config, &dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetDefaultConfig, Failed in call to GetDefaultCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetDefaultConfig(_In_ int nPort, _In_ COMMCONFIG& config)
{
  //Create the device name as a string
  TCHAR szPort[12];
  _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("COM%d"), nPort);

  return SetDefaultConfig(szPort, config);
}

void CSerialPort::SetDefaultConfig(_In_z_ LPCTSTR pszPort, _In_ COMMCONFIG& config)
{
  DWORD dwSize = sizeof(COMMCONFIG);
  if (!SetDefaultCommConfig(pszPort, &config, dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetDefaultConfig, Failed in call to SetDefaultCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetStatus(_Out_ COMSTAT& stat)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwErrors;
  if (!ClearCommError(m_hComm, &dwErrors, &stat))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetStatus, Failed in call to ClearCommError, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetState(_Out_ DCB& dcb)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommState(m_hComm, &dcb))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetState, Failed in call to GetCommState, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetState(_In_ DCB& dcb)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommState(m_hComm, &dcb))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetState, Failed in call to SetCommState, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Escape(_In_ DWORD dwFunc)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!EscapeCommFunction(m_hComm, dwFunc))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Escape, Failed in call to EscapeCommFunction, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ClearDTR()
{
  Escape(CLRDTR);
}

void CSerialPort::ClearRTS()
{
  Escape(CLRRTS);
}

void CSerialPort::SetDTR()
{
  Escape(SETDTR);
}

void CSerialPort::SetRTS()
{
  Escape(SETRTS);
}

void CSerialPort::SetXOFF()
{
  Escape(SETXOFF);
}

void CSerialPort::SetXON()
{
  Escape(SETXON);
}

void CSerialPort::GetProperties(_Inout_ COMMPROP& properties)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommProperties(m_hComm, &properties))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetProperties, Failed in call to GetCommProperties, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetModemStatus(_Out_ DWORD& dwModemStatus)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommModemStatus(m_hComm, &dwModemStatus))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetModemStatus, Failed in call to GetCommModemStatus, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetMask(_In_ DWORD dwMask)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommMask(m_hComm, dwMask))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetMask, Failed in call to SetCommMask, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetMask(_Out_ DWORD& dwMask)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommMask(m_hComm, &dwMask))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetMask, Failed in call to GetCommMask, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Flush()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!FlushFileBuffers(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Flush, Failed in call to FlushFileBuffers, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Purge(_In_ DWORD dwFlags)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!PurgeComm(m_hComm, dwFlags))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Purge, Failed in call to PurgeComm, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::TerminateOutstandingWrites()
{
  Purge(PURGE_TXABORT);
}

void CSerialPort::TerminateOutstandingReads()
{
  Purge(PURGE_RXABORT);
}

void CSerialPort::ClearWriteBuffer()
{
  Purge(PURGE_TXCLEAR);
}

void CSerialPort::ClearReadBuffer()
{
  Purge(PURGE_RXCLEAR);
}

void CSerialPort::Setup(_In_ DWORD dwInQueue, _In_ DWORD dwOutQueue)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetupComm(m_hComm, dwInQueue, dwOutQueue))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Setup, Failed in call to SetupComm, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetTimeouts(_In_ COMMTIMEOUTS& timeouts)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommTimeouts(m_hComm, &timeouts))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetTimeouts, Failed in call to SetCommTimeouts, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetTimeouts(_Out_ COMMTIMEOUTS& timeouts)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommTimeouts(m_hComm, &timeouts))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetTimeouts, Failed in call to GetCommTimeouts, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Set0Timeout()
{
  COMMTIMEOUTS Timeouts;
  memset(&Timeouts, 0, sizeof(Timeouts));
  Timeouts.ReadIntervalTimeout = MAXDWORD;
  SetTimeouts(Timeouts);
}

void CSerialPort::Set0WriteTimeout()
{
  COMMTIMEOUTS Timeouts;
  GetTimeouts(Timeouts);
  Timeouts.WriteTotalTimeoutMultiplier = 0;
  Timeouts.WriteTotalTimeoutConstant = 0;
  SetTimeouts(Timeouts);
}

void CSerialPort::Set0ReadTimeout()
{
  COMMTIMEOUTS Timeouts;
  GetTimeouts(Timeouts);
  Timeouts.ReadIntervalTimeout = MAXDWORD;
  Timeouts.ReadTotalTimeoutMultiplier = 0;
  Timeouts.ReadTotalTimeoutConstant = 0;
  SetTimeouts(Timeouts);
}

void CSerialPort::WaitEvent(_Inout_ DWORD& dwMask)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!WaitCommEvent(m_hComm, &dwMask, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::WaitEvent, Failed in call to WaitCommEvent, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

BOOL CSerialPort::WaitEvent(_Inout_ DWORD& dwMask, _Inout_ OVERLAPPED& overlapped)
{
  //Validate our parameters
  ATLASSERT(IsOpen());
  ATLASSERT(overlapped.hEvent != NULL);

  BOOL bSuccess = WaitCommEvent(m_hComm, &dwMask, &overlapped);
  if (!bSuccess)
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::WaitEvent, Failed in call to WaitCommEvent, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  return bSuccess;
}
