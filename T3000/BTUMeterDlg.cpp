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
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editor_address);
	DDX_Control(pDX, IDC_EDIT_SN, m_edit_sn);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_edit_model);
	DDX_Control(pDX, IDC_EDIT_FV, m_edit_fv);
	DDX_Control(pDX, IDC_EDIT_HV, m_edit_hv);
	DDX_Control(pDX, IDC_CO2_BRAUDRATECOMBO, m_combox_baudrate);
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


 


void CBTUMeterDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	 
}

void CBTUMeterDlg::Fresh()
{
  
}
