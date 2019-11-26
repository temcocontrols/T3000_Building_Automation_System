// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "ControlBasicEditor.tlb" no_namespace
//#import "\ControlBasicEditor\\ProgramEditor.tlb" no_namespace
// CProgramEditor1 wrapper class

class CProgramEditor1 : public CWnd
{
public:
	CProgramEditor1() {} // Calls COleDispatchDriver default constructor
						 /*CProgramEditor1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
						 CProgramEditor1(const CProgramEditor1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}
						 */
						 // Attributes
public:

	// Operations
public:


	// IProgramEditor methods
public:
	CString get_Code()
	{
		CString result;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Code(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_StrAddControlPoint()
	{
		CString result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_StrAddControlPoint(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void AddControlPoint(long type, LPCTSTR Label, LPCTSTR AutoManual, LPCTSTR value, LPCTSTR FullLabel, LPCTSTR Units, long index)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, type, Label, AutoManual, value, FullLabel, Units, index);
	}
	void SendCode_T3000(LPCTSTR Code)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Code);
	}
	void Clear_BASICControl()
	{
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Setting_BASICControl()
	{
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// IProgramEditor properties
public:

};
