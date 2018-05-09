// CalibartionHT.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CalibartionHT.h"
#include "afxdialogex.h"


// CCalibartionHT dialog

IMPLEMENT_DYNAMIC(CCalibartionHT, CDialogEx)

CCalibartionHT::CCalibartionHT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibartionHT::IDD, pParent)
{

}

CCalibartionHT::~CCalibartionHT()
{
}

void CCalibartionHT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCalibartionHT, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CCalibartionHT::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCalibartionHT message handlers


void CCalibartionHT::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
