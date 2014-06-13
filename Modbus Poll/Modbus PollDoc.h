
// Modbus PollDoc.h : interface of the CModbusPollDoc class
//


#pragma once


class CModbusPollDoc : public CDocument
{
protected: // create from serialization only
	CModbusPollDoc();
	DECLARE_DYNCREATE(CModbusPollDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CModbusPollDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
 
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
public://Data
	int m_Slave_ID;
	int m_Function;
	int m_address;
	int m_Quantity;
	int m_Scan_Rate;
	int m_Rows;
	int m_Display;
	int m_Hide_Alias_Columns;
	int m_Address_Cell;
	int m_PLC_Addresses;

	//////////////////////////////////////////////////////////////////////////
	int m_MBPoll_Function;
	//////////////////////////////////////////////////////////////////////////
	int m_data_rows;
	int m_data_cols;
	int m_grid_rows;
	int m_grid_cols;

	int m_Current_Row;
	int m_Current_Col;

	//Single Write
	BOOL m_close_dlg;
	BOOL m_function;
	/////////////////////////DataBuffer/////////////////////////////////////////
	unsigned short m_DataBuffer[127];
	CString m_Alias[127];

};
