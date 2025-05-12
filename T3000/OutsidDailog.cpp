// OutsidDailog.cpp : implementation file
//
#include "stdafx.h"
#include "T3000.h"
#include "OutsidDailog.h"
#include "global_variable_extern.h"
#include "global_function.h"

// COutsidDailog 对话框

IMPLEMENT_DYNAMIC(COutsidDailog, CDialog)
COutsidDailog::COutsidDailog(CWnd* pParent /*=NULL*/)
	: CDialog(COutsidDailog::IDD, pParent)
{
}

COutsidDailog::~COutsidDailog()
{
}

void COutsidDailog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETPTEDIT, m_SetPointEdt);
	DDX_Control(pDX, IDC_PTOFFEDIT, m_PointOffpoint);
	DDX_Control(pDX, IDC_USETRSETPT, m_UserSetPtEdit);
}


BEGIN_MESSAGE_MAP(COutsidDailog, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnEnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnEnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnEnKillfocusEdit4)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &COutsidDailog::OnBnClickedButton1)
END_MESSAGE_MAP()


// COutsidDailog 消息处理程序
BOOL COutsidDailog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strUnit=GetTempUnit();

	int nValue =multi_register_value[370];
	CString strTemp;
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT1)->SetWindowText(strTemp);

	nValue =multi_register_value[371];
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT2)->SetWindowText(strTemp);

	nValue =multi_register_value[372];
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT3)->SetWindowText(strTemp);

	nValue =multi_register_value[373];
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT4)->SetWindowText(strTemp);

		
	nValue =multi_register_value[375];
	strTemp.Format(_T("%.1f"),nValue/10.0);
	strTemp+=strUnit;
	m_SetPointEdt.SetWindowText(strTemp);
	m_SetPointEdt.EnableWindow(FALSE);

	nValue =multi_register_value[376];
	strTemp.Format(_T("%.1f"),nValue/10.0);
	strTemp+=strUnit;
	m_PointOffpoint.SetWindowText(strTemp);
	m_PointOffpoint.EnableWindow(FALSE);

	m_version=get_tstat_version(g_tstat_id);
	if(m_version<34.9)
	{
		nValue=multi_register_value[135];
		strTemp.Format(_T("%d"),nValue);
		strTemp+=strUnit;
		m_UserSetPtEdit.SetWindowText(strTemp);
	}
	else
	{
		float fnValue=multi_register_value[374];
		strTemp.Format(_T("%.1f"),fnValue/10.0);
		strTemp+=strUnit;
		m_UserSetPtEdit.SetWindowText(strTemp);
	}

	m_UserSetPtEdit.EnableWindow(FALSE);

	//GetTempUnit

//	SetTimer(1,20000,NULL);


	
	return TRUE;  // return TRUE unless you set the focus to a control
// 异常: OCX 属性页应返回 FALSE

}

void COutsidDailog::OnEnKillfocusEdit1()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strTemp);
	int nValue=_wtoi(strTemp);

	nValue=write_one(g_tstat_id,370,127+nValue);
	Refresh();

}

void COutsidDailog::OnEnKillfocusEdit3()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT3)->GetWindowText(strTemp);
	int nValue=_wtoi(strTemp);

	write_one(g_tstat_id,372,127+nValue);
	Refresh();
}

void COutsidDailog::OnEnKillfocusEdit2()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	int nValue=_wtoi(strTemp);
	write_one(g_tstat_id,371,127+nValue);
	Refresh();
}

void COutsidDailog::OnEnKillfocusEdit4()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT4)->GetWindowText(strTemp);
	int nValue=_wtoi(strTemp);

	write_one(g_tstat_id,373,127+nValue);
	Refresh();
}

void COutsidDailog::OnBnClickedCancel()
{
	
	OnCancel();
}
BOOL COutsidDailog::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN  )
	{		
		if(pMsg->wParam == VK_RETURN )
		{
			CButton * pbutton = (CButton *)GetDlgItem(IDCANCEL);
			pbutton->SetFocus();

			return TRUE;
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}
void COutsidDailog::Refresh()
{
	/*
	int nValue =read_one(g_tstat_id,370);
	CString strTemp;
	strTemp.Format(_T("%d"),nValue-127);
	GetDlgItem(IDC_EDIT1)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,371);
	strTemp.Format(_T("%d"),nValue-127);
	GetDlgItem(IDC_EDIT2)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,372);
	strTemp.Format(_T("%d"),nValue-127);
	GetDlgItem(IDC_EDIT3)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,373);
	strTemp.Format(_T("%d"),nValue-127);
	GetDlgItem(IDC_EDIT4)->SetWindowText(strTemp);

		
	nValue =read_one(g_tstat_id,375);
	strTemp.Format(_T("%.1f"),nValue/10.0);
	m_SetPointEdt.SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,376);
	strTemp.Format(_T("%.1f"),nValue/10.0);
	m_PointOffpoint.SetWindowText(strTemp);

	m_version=get_tstat_version(g_tstat_id);
	if(m_version<34.9)
	{
		nValue=read_one(g_tstat_id,135);
		strTemp.Format(_T("%d"),nValue);
		m_UserSetPtEdit.SetWindowText(strTemp);
	}
	else
	{
		float fnValue=read_one(g_tstat_id,374);
		strTemp.Format(_T("%.1f"),fnValue/10.0);
		m_UserSetPtEdit.SetWindowText(strTemp);
	}
	*/

	CString strUnit=GetTempUnit();
	int nValue =multi_register_value[370];
	CString strTemp;
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT1)->SetWindowText(strTemp);

	nValue =multi_register_value[371];
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT2)->SetWindowText(strTemp);

	nValue =multi_register_value[372];
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT3)->SetWindowText(strTemp);

	nValue =multi_register_value[373];
	strTemp.Format(_T("%d"),nValue-127);
	strTemp+=strUnit;
	GetDlgItem(IDC_EDIT4)->SetWindowText(strTemp);

		
	nValue =multi_register_value[375];
	strTemp.Format(_T("%.1f"),nValue/10.0);
	strTemp+=strUnit;
	m_SetPointEdt.SetWindowText(strTemp);
	m_SetPointEdt.EnableWindow(FALSE);

	nValue =multi_register_value[376];
	strTemp.Format(_T("%.1f"),nValue/10.0);
	strTemp+=strUnit;
	m_PointOffpoint.SetWindowText(strTemp);
	m_PointOffpoint.EnableWindow(FALSE);

	m_version=get_tstat_version(g_tstat_id);
	if(m_version<34.9)
	{
		nValue=multi_register_value[135];
		strTemp.Format(_T("%d"),nValue);
		strTemp+=strUnit;
		m_UserSetPtEdit.SetWindowText(strTemp);
	}
	else
	{
		float fnValue=multi_register_value[374];
		strTemp.Format(_T("%.1f"),fnValue/10.0);
		strTemp+=strUnit;
		m_UserSetPtEdit.SetWindowText(strTemp);
	}

	m_UserSetPtEdit.EnableWindow(FALSE);
}
void COutsidDailog::OnTimer(UINT nIDEvent)
{
	
	if(nIDEvent==1)
	{
	//	Refresh();
	}

	CDialog::OnTimer(nIDEvent);
}

void COutsidDailog::OnBnClickedButton1()
{
	int i;
	for(i=0;i<7;i++)
	{
		register_critical_section.Lock();
		Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
		register_critical_section.Unlock();
	}
	Refresh();
}
