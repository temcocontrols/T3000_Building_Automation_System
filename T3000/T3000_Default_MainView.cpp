// T3000_Default_MainView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000_Default_MainView.h"
#include "MainFrm.h"
 

// T3000_Default_MainView

IMPLEMENT_DYNCREATE(T3000_Default_MainView, CFormView)

T3000_Default_MainView::T3000_Default_MainView()
	: CFormView(T3000_Default_MainView::IDD)
{

}

T3000_Default_MainView::~T3000_Default_MainView()
{
}

void T3000_Default_MainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(T3000_Default_MainView, CFormView)
	ON_BN_CLICKED(IDC_EDIT_ID, &T3000_Default_MainView::OnBnClickedEditId)
END_MESSAGE_MAP()


// T3000_Default_MainView diagnostics

#ifdef _DEBUG
void T3000_Default_MainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T3000_Default_MainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T3000_Default_MainView message handlers


void T3000_Default_MainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void T3000_Default_MainView::Fresh(){
    CString strTemp;
	strTemp.Format(_T("%d"),g_tstat_id);
	GetDlgItem(IDC_ID_EDIT)->SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),get_serialnumber());
	GetDlgItem(IDC_SERIALNUM_EDIT)->SetWindowTextW(strTemp);

	strTemp.Format(_T("%0.1f"),get_curtstat_version());
	GetDlgItem(IDC_FIRMWARV_EDIT)->SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),product_register_value[8]);//8
	GetDlgItem(IDC_HARDWAREV_EDIT)->SetWindowTextW(strTemp);
	int nModel=product_register_value[7];
	GetDlgItem(IDC_MODEL_EDIT)->SetWindowTextW(GetProductName(nModel));
	strTemp.Format(_T("%d"),product_register_value[9]);
	GetDlgItem(IDC_PIC_EDIT)->SetWindowText(strTemp);

}
#include "EreaseDlg.h"

void T3000_Default_MainView::OnBnClickedEditId()
{
	 CEreaseDlg dlg;
	 dlg.DoModal();
}
