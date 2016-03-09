// CVBDataObjectFiles.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVBDataObjectFiles

class CVBDataObjectFiles : public COleDispatchDriver
{
public:
	CVBDataObjectFiles() {}		// Calls COleDispatchDriver default constructor
	CVBDataObjectFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVBDataObjectFiles(const CVBDataObjectFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	CString get_Item(long lIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, lIndex);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Add(LPCTSTR bstrFilename, VARIANT vIndex)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrFilename, &vIndex);
	}
	void Clear()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Remove(VARIANT vIndex)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &vIndex);
	}
	LPUNKNOWN _NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}


};
