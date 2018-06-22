// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "..\\ProgramEditor\\bin\\Debug\\ProgramEditor.tlb" no_namespace
// CProgramEditor0 wrapper class

class CProgramEditor0 : public CWnd
{
public:
	CProgramEditor0() {} // Calls COleDispatchDriver default constructor
	 

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

	// IProgramEditor properties
public:

};
