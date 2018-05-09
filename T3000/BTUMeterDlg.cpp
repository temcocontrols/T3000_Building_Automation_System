// BTUMeterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BTUMeterDlg.h"
#include "global_function.h"


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
	m_combox_baudrate.ResetContent();
	m_combox_baudrate.AddString(_T("9600"));
	m_combox_baudrate.AddString(_T("19200"));
	m_combox_baudrate.AddString(_T("38400"));
	m_combox_baudrate.AddString(_T("57600"));
	m_combox_baudrate.AddString(_T("115200"));
	CString strTemp;
	strTemp.Format(_T("%d"), product_register_value[6]);
	m_editor_address.SetWindowTextW(strTemp);
	int SN = product_register_value[2 + 1] * 256 * 256 * 256 + product_register_value[2] * 256 * 256 + product_register_value[1] * 256 + product_register_value[0];
	strTemp.Format(_T("%d"), SN);
	m_edit_sn.SetWindowTextW(strTemp);
	strTemp = GetProductName(product_register_value[7]);
 
	m_edit_model.SetWindowTextW(strTemp);
	float fv = ((float)(short)product_register_value[5] * 256 + product_register_value[4]) / 10.0;
	strTemp.Format(_T("%0.1f"), fv);
	m_edit_fv.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"), product_register_value[8]);
	m_edit_hv.SetWindowTextW(strTemp);
	m_combox_baudrate.SetCurSel(product_register_value[15]);

 
	  
}
