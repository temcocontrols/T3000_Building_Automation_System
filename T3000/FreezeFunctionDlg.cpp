// FreezeFunctionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "FreezeFunctionDlg.h"
#include "afxdialogex.h"
#include "globle_function.h"


// CFreezeFunctionDlg dialog

IMPLEMENT_DYNAMIC(CFreezeFunctionDlg, CDialog)

CFreezeFunctionDlg::CFreezeFunctionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFreezeFunctionDlg::IDD, pParent)
	, m_temp(0)
	, m_open(0)
	, m_close(0)
{

}

CFreezeFunctionDlg::~CFreezeFunctionDlg()
{
}

void CFreezeFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEMPTURE, m_temp);
	DDX_Text(pDX, IDC_OPEN, m_open);
	DDX_Text(pDX, IDC_CLOSE, m_close);
}

BOOL CFreezeFunctionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Fresh_Data();
	return TRUE;
}
BEGIN_MESSAGE_MAP(CFreezeFunctionDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_TEMPTURE, &CFreezeFunctionDlg::OnEnKillfocusTempture)
	ON_EN_KILLFOCUS(IDC_OPEN, &CFreezeFunctionDlg::OnEnKillfocusOpen)
	ON_EN_KILLFOCUS(IDC_CLOSE, &CFreezeFunctionDlg::OnEnKillfocusClose)
END_MESSAGE_MAP()


// CFreezeFunctionDlg message handlers
void CFreezeFunctionDlg::Fresh_Data()
{ 
	m_temp=multi_register_value[295];
	m_open=multi_register_value[296];
	m_close=multi_register_value[297];
	UpdateData(FALSE);

}

void CFreezeFunctionDlg::OnEnKillfocusTempture()
{
	UpdateData(TRUE);
	CString str;
	GetDlgItem(IDC_TEMPTURE)->GetWindowText(str);
	if (!str.IsEmpty())
	{
		int var=write_one(g_tstat_id,295,m_temp);
		if (var>0)
		{
			multi_register_value[295]=m_temp;
			AfxMessageBox(_T("OK"));
		} 
		else
		{
			AfxMessageBox(_T("Fail to Write"));
		}

	}
	Fresh_Data();
}


void CFreezeFunctionDlg::OnEnKillfocusOpen()
{
	UpdateData(TRUE);
	CString str;
	GetDlgItem(IDC_OPEN)->GetWindowText(str);
	if (!str.IsEmpty())
	{
		int var=write_one(g_tstat_id,296,m_open);
		if (var>0)
		{
			multi_register_value[296]=m_open;
			AfxMessageBox(_T("OK"));
		} 
		else
		{
			AfxMessageBox(_T("Fail to Write"));
		}
	}
	Fresh_Data();
}


void CFreezeFunctionDlg::OnEnKillfocusClose()
{
	UpdateData(TRUE);
	CString str;
	GetDlgItem(IDC_CLOSE)->GetWindowText(str);
	if (!str.IsEmpty())
	{
		int var=write_one(g_tstat_id,297,m_close);
		if (var>0)
		{
			multi_register_value[297]=m_close;
			AfxMessageBox(_T("OK"));
		} 
		else
		{
			AfxMessageBox(_T("Fail to Write"));
		}
	}
	Fresh_Data();
}
