// MailFeedbackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MailFeedbackDlg.h"
#include "afxdialogex.h"


// CMailFeedbackDlg dialog

IMPLEMENT_DYNAMIC(CMailFeedbackDlg, CDialogEx)

CMailFeedbackDlg::CMailFeedbackDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMailFeedbackDlg::IDD, pParent)
{

}

CMailFeedbackDlg::~CMailFeedbackDlg()
{
}

void CMailFeedbackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMailFeedbackDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BUG, &CMailFeedbackDlg::OnBnClickedButtonBug)
	ON_BN_CLICKED(IDC_BUTTON_SUGGEST, &CMailFeedbackDlg::OnBnClickedButtonSuggest)
END_MESSAGE_MAP()


// CMailFeedbackDlg message handlers


void CMailFeedbackDlg::OnBnClickedButtonBug()
{
	CString m_strWebLinker;
	m_strWebLinker.Format(_T("mailto:alex@temcocontrols.com?subject=feedback to temco &body=please add the attachment in the \n%sT3000.log "),g_strExePth);
	try{
		ShellExecute(GetSafeHwnd(), NULL,m_strWebLinker,   NULL, NULL,   SW_SHOWNORMAL);
	}
	catch(...)
	{
		AfxMessageBox(_T("Error:Can't find the email client in your pc!"));
	}
}


void CMailFeedbackDlg::OnBnClickedButtonSuggest()
{
	CString m_strWebLinker;
	m_strWebLinker.Format(_T("mailto:alex@temcocontrols.com?subject=suggest to temco &body=thanks for your suggest to temco"));
	try{
		ShellExecute(GetSafeHwnd(), NULL,m_strWebLinker,   NULL, NULL,   SW_SHOWNORMAL);
	}
	catch(...)
	{
		AfxMessageBox(_T("Error:Can't find the email client in your pc!"));
	}
}
