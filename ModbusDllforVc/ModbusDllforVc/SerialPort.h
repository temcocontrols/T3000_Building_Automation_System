/*
Module : SerialPort.h
Purpose: Interface for an C++ wrapper class for serial ports

Copyright (c) 1999 - 2016 by PJ Naughter.  

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////// Macros / Structs etc ////////////////////////////////////

#pragma once

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#ifndef CSERIALPORT_EXT_CLASS
#define CSERIALPORT_EXT_CLASS
#endif //#ifndef CSERIALPORT_EXT_CLASS

#ifndef _Out_writes_bytes_
#define _Out_writes_bytes_(X)
#endif //#ifndef _Out_writes_bytes_

#ifndef __out_data_source
#define __out_data_source(X)
#endif //#ifndef __out_data_source

#ifndef _Out_writes_bytes_to_opt_
#define _Out_writes_bytes_to_opt_(X,Y)
#endif //#ifndef _Out_writes_bytes_to_opt_

#ifndef _Out_writes_bytes_opt_
#define _Out_writes_bytes_opt_(X)
#endif //#ifndef _Out_writes_bytes_opt_

#ifndef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_(X)
#endif //#ifndef _In_reads_bytes_opt_

#ifndef _In_
#define _In_
#endif //#ifndef _In_

#ifndef _In_z_
#define _In_z_
#endif //#ifndef _In_z_

#ifndef _Inout_opt_
#define _Inout_opt_
#endif //#ifndef _Inout_opt_

#ifndef _Out_opt_
#define _Out_opt_
#endif //#ifndef _Out_opt_

#ifndef _Out_
#define _Out_
#endif //#ifndef _Out_

#ifndef _Inout_
#define _Inout_
#endif //#ifndef _Inout_

#ifndef _In_opt_
#define _In_opt_
#endif //#ifndef _In_opt_


////////////////////////// Includes ///////////////////////////////////////////

#include <sal.h>

#ifndef CSERIALPORT_MFC_EXTENSIONS
#include <exception>
#include <string>
#endif //#ifndef CSERIALPORT_MFC_EXTENSIONS


/////////////////////////// Classes ///////////////////////////////////////////

#ifdef CSERIALPORT_MFC_EXTENSIONS
class CSERIALPORT_EXT_CLASS CSerialException : public CException
#else
class CSERIALPORT_EXT_CLASS CSerialException : public std::exception
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS
{
public:
//Constructors / Destructors
  CSerialException(DWORD dwError);

//Methods
#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

#if _MSC_VER >= 1700
  virtual BOOL GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError,	_Out_opt_ PUINT pnHelpContext = NULL);
#else	
  virtual BOOL GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpszError, __in UINT nMaxError, __out_opt PUINT pnHelpContext = NULL);
#endif

#ifdef CSERIALPORT_MFC_EXTENSIONS
  CString GetErrorMessage();
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

//Data members
  DWORD m_dwError;
};


class CSERIALPORT_EXT_CLASS CSerialPort
{
public:
//Enums
  enum FlowControl
  {
    NoFlowControl,
    CtsRtsFlowControl,
    CtsDtrFlowControl,
    DsrRtsFlowControl,
    DsrDtrFlowControl,
    XonXoffFlowControl
  };

  enum Parity
  {    
    NoParity = 0,
    OddParity = 1,
    EvenParity = 2,
    MarkParity = 3,
    SpaceParity = 4
  };

  enum StopBits
  {
    OneStopBit,
    OnePointFiveStopBits,
    TwoStopBits
  };

//Constructors / Destructors
  CSerialPort();
  virtual ~CSerialPort();

//General Methods
  void Open(_In_ int nPort, _In_ DWORD dwBaud = 9600, _In_ Parity parity = NoParity, _In_ BYTE DataBits = 8, 
            _In_ StopBits stopBits = OneStopBit, _In_ FlowControl fc = NoFlowControl, _In_ BOOL bOverlapped = FALSE);
  void Open(_In_z_ LPCTSTR pszPort, _In_ DWORD dwBaud = 9600, _In_ Parity parity = NoParity, _In_ BYTE DataBits = 8, 
            _In_ StopBits stopBits = OneStopBit, _In_ FlowControl fc = NoFlowControl, _In_ BOOL bOverlapped = FALSE);
  void Close();
  void Attach(_In_ HANDLE hComm);
  HANDLE Detach();
  operator HANDLE() const { return m_hComm; };
  BOOL IsOpen() const { return m_hComm != INVALID_HANDLE_VALUE; };

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
  void Dump(_In_ CDumpContext& dc) const;
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

//Reading / Writing Methods
  DWORD Read(_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead);
  void  Read(_Out_writes_bytes_to_opt_(dwNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _In_ OVERLAPPED& overlapped, _Inout_opt_ DWORD* lpNumberOfBytesRead = NULL);
  void  ReadEx(_Out_writes_bytes_opt_(dwNumberOfBytesToRead) __out_data_source(FILE) LPVOID lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
  DWORD Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite);
  void  Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _In_ OVERLAPPED& overlapped, _Out_opt_ DWORD* lpNumberOfBytesWritten = NULL);
  void  WriteEx(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) LPCVOID lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
  void  TransmitChar(_In_ char cChar);
  void  GetOverlappedResult(_In_ OVERLAPPED& overlapped, _Out_ DWORD& dwBytesTransferred, _In_ BOOL bWait);
  void  CancelIo();
  DWORD BytesWaiting();

//Configuration Methods
  void GetConfig(_In_ COMMCONFIG& config);
  static void GetDefaultConfig(_In_ int nPort, _Out_ COMMCONFIG& config);
  static void GetDefaultConfig(_In_z_ LPCTSTR pszPort, _Out_ COMMCONFIG& config);
  void SetConfig(_In_ COMMCONFIG& Config);
  static void SetDefaultConfig(_In_ int nPort, _In_ COMMCONFIG& config);
  static void SetDefaultConfig(_In_z_ LPCTSTR pszPort, _In_ COMMCONFIG& config);

//Misc RS232 Methods
  void ClearBreak();
  void SetBreak();
  void ClearError(_Out_ DWORD& dwErrors);
  void GetStatus(_Out_ COMSTAT& stat);
  void GetState(_Out_ DCB& dcb);
  void SetState(_In_ DCB& dcb);
  void Escape(_In_ DWORD dwFunc);
  void ClearDTR();
  void ClearRTS();
  void SetDTR();
  void SetRTS();
  void SetXOFF();
  void SetXON();
  void GetProperties(_Inout_ COMMPROP& properties);
  void GetModemStatus(_Out_ DWORD& dwModemStatus); 

//Timeouts
  void SetTimeouts(_In_ COMMTIMEOUTS& timeouts);
  void GetTimeouts(_Out_ COMMTIMEOUTS& timeouts);
  void Set0Timeout();
  void Set0WriteTimeout();
  void Set0ReadTimeout();

//Event Methods
  void SetMask(_In_ DWORD dwMask);
  void GetMask(_Out_ DWORD& dwMask);
  void WaitEvent(_Inout_ DWORD& dwMask);
  BOOL WaitEvent(_Inout_ DWORD& dwMask, _Inout_ OVERLAPPED& overlapped);
  
//Queue Methods
  void Flush();
  void Purge(_In_ DWORD dwFlags);
  void TerminateOutstandingWrites();
  void TerminateOutstandingReads();
  void ClearWriteBuffer();
  void ClearReadBuffer();
  void Setup(_In_ DWORD dwInQueue, _In_ DWORD dwOutQueue);

//Static methods
  static void ThrowSerialException(_In_ DWORD dwError = 0);

protected:
//Member variables
  HANDLE m_hComm;  //Handle to the comms port
};

#endif //#ifndef __SERIALPORT_H__
