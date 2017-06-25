
// Modbus PollDoc.cpp : implementation of the CModbusPollDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Modbus Poll.h"
#endif

#include "Modbus PollDoc.h"
#include "Modbus PollView.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CModbusPollDoc

IMPLEMENT_DYNCREATE(CModbusPollDoc, CDocument)
BEGIN_MESSAGE_MAP(CModbusPollDoc, CDocument)
 
END_MESSAGE_MAP()


// CModbusPollDoc construction/destruction

CModbusPollDoc::CModbusPollDoc()
{
	 m_Slave_ID=-1;

}

CModbusPollDoc::~CModbusPollDoc()
{
}

BOOL CModbusPollDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	 
 
     m_Slave_ID=-1;

 

	 
	 

	return TRUE;
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CModbusPollDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T(" implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CModbusPollDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CModbusPollDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS
void CModbusPollDoc::Serialize(CArchive& ar)
{
	
	if (ar.IsStoring())
	{
		 ar<<m_Slave_ID;
		 ar<<m_Function;
		 ar<<m_address;
		 ar<<m_Quantity;
		 ar<<m_Scan_Rate;
		 ar<<m_Rows;
		 ar<<m_Display;
		 ar<<m_Hide_Alias_Columns;
		 ar<<m_Address_Cell;
		 ar<<m_PLC_Addresses;
	 
		 //////////////////////////////////////////////////////////////////////
		 ar<<m_MBPoll_Function;
		  //////////////////////////////////////////////////////////////////////
		 ar<<m_data_rows;
		 ar<<m_data_cols;
		 ar<<m_grid_rows;
		 ar<<m_grid_cols;
		 
		 ar<<m_Current_Row;
		 ar<<m_Current_Col;
		 
		 ar<< m_close_dlg;
		 ar<< m_function;
		/////////////////////////DataBuffer/////////////////////////////////////////
		 
		/////////////////////////DataBuffer/////////////////////////////////////////
		for (int i=0;i<127;i++)
		{
			ar<<m_Alias[i];
			ar<<m_DataBuffer[i];
		}
	}
	else
	{
		ar>>m_Slave_ID;
		ar>>m_Function;
		ar>>m_address;
		ar>>m_Quantity;
		ar>>m_Scan_Rate;
		ar>>m_Rows;
		ar>>m_Display;
		ar>>m_Hide_Alias_Columns;
		ar>>m_Address_Cell;
		ar>>m_PLC_Addresses;

		//////////////////////////////////////////////////////////////////////
		ar>>m_MBPoll_Function;
		//////////////////////////////////////////////////////////////////////
		ar>>m_data_rows;
		ar>>m_data_cols;
		ar>>m_grid_rows;
		ar>>m_grid_cols;
		ar>>m_Current_Row;
		ar>>m_Current_Col;
		ar>>m_close_dlg;
		ar>>m_function;
		/////////////////////////DataBuffer/////////////////////////////////////////
		for (int i=0;i<127;i++)
		{
			ar>>m_Alias[i];
			ar>>m_DataBuffer[i];
		}
	}

}



// CModbusPollDoc diagnostics

#ifdef _DEBUG
void CModbusPollDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CModbusPollDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG



 


BOOL CModbusPollDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	  

	return TRUE;
}


BOOL CModbusPollDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	
	CMDIChildWnd * pChild =
		((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

	if ( !pChild )
		return NULL;

	CModbusPollView * pView = (CModbusPollView *)pChild->GetActiveView();

	if ( !pView )
		return NULL;

	// Fail if view is of wrong kind
	if ( ! pView->IsKindOf( RUNTIME_CLASS(CModbusPollView) ) )
		return NULL;

	  m_Slave_ID=pView->m_Slave_ID;
	  m_Function=pView->m_Function;
	  m_address=pView->m_address;
	  m_Quantity=pView->m_Quantity;
	  m_Scan_Rate=pView->m_Scan_Rate;
	  m_Rows=pView->m_Rows;
	  m_Display=pView->m_Display;
	  m_Hide_Alias_Columns=pView->m_Hide_Alias_Columns;
	  m_Address_Cell=pView->m_Address_Cell;
	  m_PLC_Addresses=pView->m_PLC_Addresses;

	//////////////////////////////////////////////////////////////////////////
	  m_MBPoll_Function=pView->m_MBPoll_Function;
	//////////////////////////////////////////////////////////////////////////
	  m_data_rows=pView->m_data_rows;
	  m_data_cols=pView->m_data_cols;
	  m_grid_rows=pView->m_grid_rows;
	  m_grid_cols=pView->m_grid_cols;
	 
	  m_Current_Row=pView->m_Current_Row;
	  m_Current_Col=pView->m_Current_Col;

	//Single Write
	  m_close_dlg=pView->m_close_dlg;
	  m_function=pView->m_function;
	/////////////////////////DataBuffer/////////////////////////////////////////
	  for (int i=0;i<127;i++)
	  {
		  m_Alias[i]=pView->m_Alias[i];
		  m_DataBuffer[i]=pView->m_DataBuffer[i];
	  }
	 
	  


	return CDocument::OnSaveDocument(lpszPathName);
}
