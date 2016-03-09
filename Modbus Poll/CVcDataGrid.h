// CVcDataGrid.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVcDataGrid

class CVcDataGrid : public COleDispatchDriver
{
public:
	CVcDataGrid() {}		// Calls COleDispatchDriver default constructor
	CVcDataGrid(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcDataGrid(const CVcDataGrid& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	short get_ColumnCount()
	{
		short result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_ColumnCount(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_ColumnLabelCount()
	{
		short result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_ColumnLabelCount(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_RowLabelCount()
	{
		short result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_RowLabelCount(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_RowCount()
	{
		short result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_RowCount(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void DeleteColumns(short Column, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Column, Count);
	}
	void InsertColumns(short Column, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Column, Count);
	}
	void DeleteColumnLabels(short LabelIndex, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LabelIndex, Count);
	}
	void InsertColumnLabels(short LabelIndex, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LabelIndex, Count);
	}
	void DeleteRows(short Row, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Count);
	}
	void InsertRows(short Row, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Count);
	}
	void DeleteRowLabels(short LabelIndex, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LabelIndex, Count);
	}
	void InsertRowLabels(short LabelIndex, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LabelIndex, Count);
	}
	void RandomDataFill()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetSize(short RowLabelCount, short ColumnLabelCount, short DataRowCount, short DataColumnCount)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, RowLabelCount, ColumnLabelCount, DataRowCount, DataColumnCount);
	}
	void InitializeLabels()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RandomFillColumns(short Column, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Column, Count);
	}
	void RandomFillRows(short Row, short Count)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Count);
	}
	void MoveData(short Top, short Left, short Bottom, short Right, short OverOffset, short DownOffset)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Top, Left, Bottom, Right, OverOffset, DownOffset);
	}
	void GetData(short Row, short Column, double * DataPoint, short * nullFlag)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_PR8 VTS_PI2 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Column, DataPoint, nullFlag);
	}
	void SetData(short Row, short Column, double DataPoint, short nullFlag)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_R8 VTS_I2 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Column, DataPoint, nullFlag);
	}
	CString get_ColumnLabel(short Column, short LabelIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Column, LabelIndex);
		return result;
	}
	void put_ColumnLabel(short Column, short LabelIndex, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_BSTR ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Column, LabelIndex, newValue);
	}
	CString get_CompositeColumnLabel(short Column)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Column);
		return result;
	}
	CString get_CompositeRowLabel(short Row)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Row);
		return result;
	}
	CString get_RowLabel(short Row, short LabelIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Row, LabelIndex);
		return result;
	}
	void put_RowLabel(short Row, short LabelIndex, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, LabelIndex, newValue);
	}


};
