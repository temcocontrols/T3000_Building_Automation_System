
// T3000Doc.cpp : implementation of the CT3000Doc class
//

#include "stdafx.h"
#include "T3000.h"

#include "T3000Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CT3000Doc

IMPLEMENT_DYNCREATE(CT3000Doc, CDocument)

BEGIN_MESSAGE_MAP(CT3000Doc, CDocument)
END_MESSAGE_MAP()


// CT3000Doc construction/destruction

CT3000Doc::CT3000Doc()
{
	// 

}

CT3000Doc::~CT3000Doc()
{
}

BOOL CT3000Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	
	// (SDI documents will reuse this document)

	return TRUE;
}




// CT3000Doc serialization

void CT3000Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//  add storing code here
	}
	else
	{
		//  add loading code here
	}
}


// CT3000Doc diagnostics

#ifdef _DEBUG
void CT3000Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CT3000Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CT3000Doc commands
