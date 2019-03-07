// NewT3000ProgramEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NewT3000ProgramEditorDlg.h"
#include "afxdialogex.h"
#include "NewT3000ProgramEditorDlg.h"
#include "Resource.h"
// CNewT3000ProgramEditorDlg dialog

IMPLEMENT_DYNAMIC(CNewT3000ProgramEditorDlg, CDialogEx)

CNewT3000ProgramEditorDlg::CNewT3000ProgramEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_NEW_T3000_PRG_EDIT, pParent)
{

}

CNewT3000ProgramEditorDlg::~CNewT3000ProgramEditorDlg()
{
}

void CNewT3000ProgramEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FORMS1, m_programeditor);
}


BEGIN_MESSAGE_MAP(CNewT3000ProgramEditorDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewT3000ProgramEditorDlg message handlers


BOOL CNewT3000ProgramEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString ShowProgramText;
	ShowProgramText.Format(_T("Panel :  %u      Program  :  %u "), Station_NUM, program_list_line + 1);
	//ShowProgramText = ShowProgramText + temp_label;
	SetWindowText(ShowProgramText);

	CString strcode;

#if 0
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
 
	CString strSql;
	strSql.Format(_T("SELECT COUNT(*) FROM sqlite_master where type = 'table' and name = 'PRG_CODE'"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	if(!q.eof()) {
		strSql.Format(_T("Select * From PRG_CODE where Station_NUM=%d AND program_list_line=%d"), Station_NUM, program_list_line);
		q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
		if (!q.eof()) {
			strcode = q.getValuebyName(L"CODE");
		}
	}
	
	SqliteDBT3000.closedb();
#endif

	// TODO:  Add extra initialization here
	m_programeditor.put_Code(strcode);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
