// BTUMeterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BTUMeterDlg.h"


// CBTUMeterDlg

IMPLEMENT_DYNCREATE(CBTUMeterDlg, CFormView)

CBTUMeterDlg::CBTUMeterDlg()
	: CFormView(IDD_DIALOG_BTUMETER)
{

}

CBTUMeterDlg::~CBTUMeterDlg()
{
}

void CBTUMeterDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBTUMeterDlg, CFormView)
END_MESSAGE_MAP()


// CBTUMeterDlg diagnostics

#ifdef _DEBUG
void CBTUMeterDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBTUMeterDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBTUMeterDlg message handlers
