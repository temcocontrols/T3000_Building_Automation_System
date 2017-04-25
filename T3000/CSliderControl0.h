// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "H:\\T3000_Building_Automation_System\\T3000Controls\\bin\\Release\\T3000Controls.tlb" no_namespace
// CSliderControl0 wrapper class

class CSliderControl0 : public CWnd
{
public:
	CSliderControl0() {} // Calls COleDispatchDriver default constructor
 

	// Attributes
public:

	// Operations
public:


	// ISliderControl methods
public:
	float get_TopValue()
	{
		float result;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_TopValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_BottomValue()
	{
		float result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_BottomValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_CurrentValue()
	{
		float result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_CurrentValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_TopZone()
	{
		BOOL result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TopZone(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_BottomZone()
	{
		BOOL result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_BottomZone(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_TopZoneValue()
	{
		float result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_TopZoneValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_BottomZoneValue()
	{
		float result;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_BottomZoneValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_MiddleZoneValue()
	{
		float result;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_MiddleZoneValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_StepValue()
	{
		float result;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_StepValue(float newValue)
	{
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_TwoSliderMode()
	{
		BOOL result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TwoSliderMode(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_AdditionalText()
	{
		CString result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_AdditionalText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_LinesColor()
	{
		unsigned long result;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_LinesColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BorderColor()
	{
		unsigned long result;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BorderColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_TopZoneColor()
	{
		unsigned long result;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_TopZoneColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BottomZoneColor()
	{
		unsigned long result;
		InvokeHelper(0x6002001c, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BottomZoneColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x6002001c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_CurrentValueColor()
	{
		unsigned long result;
		InvokeHelper(0x6002001e, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_CurrentValueColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x6002001e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BackgroundWidth()
	{
		long result;
		InvokeHelper(0x60020020, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BackgroundWidth(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x60020020, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HandlesHeight()
	{
		long result;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_HandlesHeight(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_HandlesBorderColor()
	{
		unsigned long result;
		InvokeHelper(0x60020024, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_HandlesBorderColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x60020024, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_MiddleHandleColor()
	{
		unsigned long result;
		InvokeHelper(0x60020026, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_MiddleHandleColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x60020026, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_LowEventMode()
	{
		BOOL result;
		InvokeHelper(0x60020028, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_LowEventMode(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020028, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MiddleZoneValueAsAverage()
	{
		BOOL result;
		InvokeHelper(0x6002002a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MiddleZoneValueAsAverage(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x6002002a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_IndicatorColor()
	{
		unsigned long result;
		InvokeHelper(0x6002002c, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_IndicatorColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x6002002c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_IndicatorBorderColor()
	{
		unsigned long result;
		InvokeHelper(0x6002002e, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_IndicatorBorderColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x6002002e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_IndicatorWidth()
	{
		long result;
		InvokeHelper(0x60020030, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_IndicatorWidth(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x60020030, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_IndicatorHeight()
	{
		long result;
		InvokeHelper(0x60020032, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_IndicatorHeight(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x60020032, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_EnableIndicator()
	{
		BOOL result;
		InvokeHelper(0x60020034, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableIndicator(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020034, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IsSimpleIndicator()
	{
		BOOL result;
		InvokeHelper(0x60020036, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IsSimpleIndicator(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020036, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_IndicatorText()
	{
		CString result;
		InvokeHelper(0x60020038, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_IndicatorText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020038, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IsInverse()
	{
		BOOL result;
		InvokeHelper(0x6002003a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_InTopZone()
	{
		BOOL result;
		InvokeHelper(0x6002003b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_InBottomZone()
	{
		BOOL result;
		InvokeHelper(0x6002003c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString ValueToText(float value)
	{
		CString result;
		static BYTE parms[] = VTS_R4;
		InvokeHelper(0x6002003d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, value);
		return result;
	}
	CString get_TopZoneText()
	{
		CString result;
		InvokeHelper(0x6002003e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_BottomZoneText()
	{
		CString result;
		InvokeHelper(0x6002003f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_MiddleZoneText()
	{
		CString result;
		InvokeHelper(0x60020040, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_TopValueText()
	{
		CString result;
		InvokeHelper(0x60020041, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_BottomValueText()
	{
		CString result;
		InvokeHelper(0x60020042, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_CurrentValueText()
	{
		CString result;
		InvokeHelper(0x60020043, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetRange(float TopValue, float BottomValue)
	{
		static BYTE parms[] = VTS_R4 VTS_R4;
		InvokeHelper(0x60020044, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TopValue, BottomValue);
	}
	void SetZoneValues(float TopZoneValue, float BottomZoneValue)
	{
		static BYTE parms[] = VTS_R4 VTS_R4;
		InvokeHelper(0x60020045, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TopZoneValue, BottomZoneValue);
	}
	void SetZoneValues_2(float TopZoneValue, float MiddleZoneValue, float BottomZoneValue)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4;
		InvokeHelper(0x60020046, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TopZoneValue, MiddleZoneValue, BottomZoneValue);
	}
	unsigned long get_ForeColor()
	{
		unsigned long result;
		InvokeHelper(0x60020047, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_ForeColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x60020047, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x60020049, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x60020049, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(0x6002004b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x6002004b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Top()
	{
		long result;
		InvokeHelper(0x6002004d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Top(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x6002004d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Left()
	{
		long result;
		InvokeHelper(0x6002004f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Left(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x6002004f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Width()
	{
		long result;
		InvokeHelper(0x60020051, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Width(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x60020051, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Height()
	{
		long result;
		InvokeHelper(0x60020053, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Height(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x60020053, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Dispose()
	{
		InvokeHelper(0x60020055, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// ISliderControl properties
public:

};
