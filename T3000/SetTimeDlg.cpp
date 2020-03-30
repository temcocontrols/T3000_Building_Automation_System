// SetTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "SetTimeDlg.h"
#include "afxdialogex.h"


// CSetTimeDlg dialog

IMPLEMENT_DYNAMIC(CSetTimeDlg, CDialogEx)

CSetTimeDlg::CSetTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SETTIME, pParent)
	, m_strTime(_T("9:30"))
{
	 
}

CSetTimeDlg::~CSetTimeDlg()
{
}

void CSetTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1_SCHEDUAL, m_strTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER1_SCHEDUAL, m_TimePicker);
}


BEGIN_MESSAGE_MAP(CSetTimeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetTimeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetTimeDlg message handlers


void CSetTimeDlg::OnBnClickedOk()
{
	 
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


BOOL CSetTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    m_TimePicker.SetFormat(_T("HH:mm"));
	
	m_TimePicker.GetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
