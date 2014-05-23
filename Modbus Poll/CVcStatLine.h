// CVcStatLine.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVcStatLine

class CVcStatLine : public COleDispatchDriver
{
public:
	CVcStatLine() {}		// Calls COleDispatchDriver default constructor
	CVcStatLine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcStatLine(const CVcStatLine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	short get_Flag()
	{
		short result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Flag(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_VtColor()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	float get_Width()
	{
		float result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Width(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Style(long type)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, type);
		return result;
	}
	void put_Style(long type, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, type, newValue);
	}


};
