// T3ModulesOutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3ModulesOutputDlg.h"
#include "afxdialogex.h"


// CT3ModulesOutputDlg dialog

IMPLEMENT_DYNAMIC(CT3ModulesOutputDlg, CDialogEx)

CT3ModulesOutputDlg::CT3ModulesOutputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CT3ModulesOutputDlg::IDD, pParent)
{

}

CT3ModulesOutputDlg::~CT3ModulesOutputDlg()
{
}

void CT3ModulesOutputDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_T3OUTPUTS, m_outputlist);
}


BEGIN_MESSAGE_MAP(CT3ModulesOutputDlg, CDialogEx)
END_MESSAGE_MAP()


// CT3ModulesOutputDlg message handlers
