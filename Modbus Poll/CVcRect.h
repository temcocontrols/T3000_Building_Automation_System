// CVcRect.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVcRect

class CVcRect : public COleDispatchDriver
{
public:
	CVcRect() {}		// Calls COleDispatchDriver default constructor
	CVcRect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcRect(const CVcRect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	LPDISPATCH get_Min()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Max()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}


};
