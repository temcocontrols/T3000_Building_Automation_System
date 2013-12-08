// RegDetails.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RegDetails.h"
#include "afxdialogex.h"
#include "MbpExterns.h"


// CRegDetails dialog

IMPLEMENT_DYNAMIC(CRegDetails, CDialogEx)

CRegDetails::CRegDetails(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegDetails::IDD, pParent)
{

}

CRegDetails::~CRegDetails()
{
}

void CRegDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ALIAS, editAlias);
	DDX_Control(pDX, IDC_EDIT_VALUE, editValue);
	DDX_Control(pDX, IDC_STATIC_ADDR, staticAddress);
	DDX_Control(pDX, IDC_STATIC_ALIAS, staticAlias2);
}

BEGIN_MESSAGE_MAP(CRegDetails, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegDetails::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRegDetails::OnBnClickedCancel)
END_MESSAGE_MAP()

// CRegDetails message handlers

BOOL CRegDetails::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	editAlias.SetWindowTextW(aliasString);
	editValue.SetWindowTextW(regValueString);
	staticAddress.SetWindowTextW(regAddrString);
	if (editableValueField == 1)
	{
		editValue.EnableWindow(TRUE);
	}
	else
	{
		editValue.EnableWindow(FALSE);
	}

	staticAlias2.ShowWindow(TRUE);
	editAlias.ShowWindow(TRUE);
	if (regDetailsOpenedFrom == 1)
	{
		staticAlias2.ShowWindow(FALSE);
		editAlias.ShowWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRegDetails::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	editAlias.GetWindowTextW(aliasString);
	if (editableValueField == 1)
	{
		editValue.GetWindowTextW(regValueString);
	}
	CDialogEx::OnOK();
}

void CRegDetails::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


