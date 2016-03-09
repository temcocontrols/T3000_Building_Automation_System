// CVcAxisGrid.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVcAxisGrid

class CVcAxisGrid : public COleDispatchDriver
{
public:
	CVcAxisGrid() {}		// Calls COleDispatchDriver default constructor
	CVcAxisGrid(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcAxisGrid(const CVcAxisGrid& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	LPDISPATCH get_MinorPen()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_MajorPen()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}


};
