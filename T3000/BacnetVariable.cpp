// BacnetVariable.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetVariable.h"
#include "afxdialogex.h"


// CBacnetVariable dialog

IMPLEMENT_DYNAMIC(CBacnetVariable, CDialogEx)

CBacnetVariable::CBacnetVariable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetVariable::IDD, pParent)
{

}

CBacnetVariable::~CBacnetVariable()
{
}

void CBacnetVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetVariable, CDialogEx)
END_MESSAGE_MAP()


// CBacnetVariable message handlers


BOOL CBacnetVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
