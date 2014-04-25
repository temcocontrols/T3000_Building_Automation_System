// WhichOneToChooseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WhichOneToChooseDlg.h"
#include "afxdialogex.h"


// CWhichOneToChooseDlg dialog

IMPLEMENT_DYNAMIC(CWhichOneToChooseDlg, CDialogEx)

CWhichOneToChooseDlg::CWhichOneToChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWhichOneToChooseDlg::IDD, pParent)
{

	m_StrCom = _T("");
	m_StrNet = _T("");
}

CWhichOneToChooseDlg::~CWhichOneToChooseDlg()
{
}

void CWhichOneToChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_COM, m_Bool_Check_Com);
	DDX_Check(pDX, IDC_CHECK_NET, m_Bool_Check_Net);
	DDX_Text(pDX, IDC_STATIC_COM, m_StrCom);
	DDX_Text(pDX, IDC_STATIC_NET, m_StrNet);
}


BEGIN_MESSAGE_MAP(CWhichOneToChooseDlg, CDialogEx)
END_MESSAGE_MAP()


// CWhichOneToChooseDlg message handlers
