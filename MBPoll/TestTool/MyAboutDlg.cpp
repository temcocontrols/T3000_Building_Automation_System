// MyAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestTool.h"
#include "MyAboutDlg.h"
#include "afxdialogex.h"


// CMyAboutDlg dialog

IMPLEMENT_DYNAMIC(CMyAboutDlg, CDialogEx)

CMyAboutDlg::CMyAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyAboutDlg::IDD, pParent)
{

}

CMyAboutDlg::~CMyAboutDlg()
{
}

void CMyAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CMyAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyAboutDlg message handlers


void CMyAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CMyAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString cs_tool_version;
	cs_tool_version.LoadString(IDS_TOOL_VERSION);
	CString TEMP_CS = _T("TestTool     ");
	TEMP_CS = TEMP_CS + cs_tool_version;
	SetDlgItemTextW(IDC_STATIC_TOP,TEMP_CS);
	
	// TODO:  Add extra initialization here
	CString release_note;
	CString temp;
	release_note.Format(_T("Rev1.0.3  (2013-06-30)\r\n1.Optimize Write Register\r\n"));
	temp.Format(_T("Rev1.0.4  (2013-07-30)\r\n1.Add Register Monitor read interval.\r\n"));
	release_note = release_note + temp;
	temp.Format(_T("Rev1.0.5  (2013-07-31)\r\n1.Register Monitor save parameter into the config.\r\n"));
	release_note = release_note + temp;
	temp.Format(_T("Rev1.0.6  (2013-08-01)\r\n1.Register Monitor save file use format.\r\n"));
	release_note = release_note + temp;
	SetDlgItemTextW(IDC_STATIC_ABOUT,release_note);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
