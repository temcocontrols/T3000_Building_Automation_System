// T3000Option.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000Option.h"
#include "afxdialogex.h"

#include "../SQLiteDriver/CppSQLite3.h"
#include "global_function.h"
// CT3000Option dialog
 
IMPLEMENT_DYNAMIC(CT3000Option, CDialogEx)

CT3000Option::CT3000Option(CWnd* pParent /*=NULL*/)
	: CDialogEx(CT3000Option::IDD, pParent)
{

}

CT3000Option::~CT3000Option()
{
}

void CT3000Option::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_combox_language);
}


BEGIN_MESSAGE_MAP(CT3000Option, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CT3000Option::OnCbnSelchangeComboLanguage)
END_MESSAGE_MAP()


// CT3000Option message handlers


BOOL CT3000Option::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	 m_combox_language.ResetContent();
	 m_combox_language.AddString(L"English");
	 m_combox_language.AddString(L"Chinese");
	 m_combox_language.AddString(L"Korean");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CT3000Option::OnCbnSelchangeComboLanguage()
{
	int Lan_ID = m_combox_language.GetCurSel();
	
	//CppSQLite3DB SqliteDB;
	//CString m_sqlitepath = GetExePath(true) + L"Psychrometry\\db_psychrometric_project.s3db";
	// 
	//SqliteDB.open((UTF8MBSTR)m_sqlitepath);

	//CString SqlText;
	//
	//SqlText.Format(_T("Update tbl_language_option set language_id = 0  where  language_id = 1"));

	//SqliteDB.execDML((UTF8MBSTR)SqlText);

	//SqlText.Format(_T("Update tbl_language_option set language_id = 1  where  ID = %d"),Lan_ID+1);

	//SqliteDB.execDML((UTF8MBSTR)SqlText);

	//SqliteDB.closedb();

}
