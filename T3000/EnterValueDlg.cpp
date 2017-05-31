// EnterValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "EnterValueDlg.h"
#include "afxdialogex.h"


// CEnterValueDlg dialog

IMPLEMENT_DYNAMIC(CEnterValueDlg, CDialogEx)

CEnterValueDlg::CEnterValueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterValueDlg::IDD, pParent)
    , m_string_value(_T(""))
{

}

CEnterValueDlg::~CEnterValueDlg()
{
}

void CEnterValueDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_string_value);
    DDX_Control(pDX, IDC_EDIT1, m_edit_value);
}


BEGIN_MESSAGE_MAP(CEnterValueDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnterValueDlg message handlers


BOOL CEnterValueDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    
    m_edit_value.SetSel(0,m_string_value.GetLength());
    m_edit_value.SetFocus();
    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
