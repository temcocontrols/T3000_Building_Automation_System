// CVBDataObject.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVBDataObject

class CVBDataObject : public COleDispatchDriver
{
public:
	CVBDataObject() {}		// Calls COleDispatchDriver default constructor
	CVBDataObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVBDataObject(const CVBDataObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	void Clear()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	VARIANT GetData(short sFormat)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, sFormat);
		return result;
	}
	BOOL GetFormat(short sFormat)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sFormat);
		return result;
	}
	void SetData(VARIANT vValue, VARIANT vFormat)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &vValue, &vFormat);
	}
	LPDISPATCH get_Files()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}


};
