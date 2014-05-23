// CVcPlotBase.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVcPlotBase

class CVcPlotBase : public COleDispatchDriver
{
public:
	CVcPlotBase() {}		// Calls COleDispatchDriver default constructor
	CVcPlotBase(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcPlotBase(const CVcPlotBase& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	float get_BaseHeight()
	{
		float result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_BaseHeight(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Brush()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Pen()
	{
		LPDISPATCH result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}


};
