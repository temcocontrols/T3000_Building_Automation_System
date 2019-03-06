// NewT3000ProgramEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NewT3000ProgramEditorDlg.h"
 
 
#include "Resource.h"


#include "BacnetProgram.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetProgramSetting.h"
#include "BacnetProgramDebug.h"


extern CString program_string;
extern CString AnalysisString;
extern char my_display[10240];
extern HWND mParent_Hwnd;
extern void copy_data_to_ptrpanel(int Data_type);
extern char editbuf[25000];
extern char *desassembler_program();
extern bool show_upper;
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
	//CString ShowProgramText;
	//ShowProgramText.Format(_T("Panel :  %u      Program  :  %u "), Station_NUM, program_list_line + 1);
	////ShowProgramText = ShowProgramText + temp_label;
	//SetWindowText(ShowProgramText);
	CString ShowProgramText;
	CString temp_label;
	MultiByteToWideChar(CP_ACP, 0, (char *)m_Program_data.at(program_list_line).label, (int)strlen((char *)m_Program_data.at(program_list_line).label) + 1,
		temp_label.GetBuffer(MAX_PATH), MAX_PATH);
	temp_label.ReleaseBuffer();
	if (temp_label.IsEmpty())
	{
		temp_label.Format(_T("PRG%d"), program_list_line + 1);
	}

	ShowProgramText.Format(_T("Panel :  %u      Program  :  %u      Name  :  "), Station_NUM, program_list_line + 1);
	ShowProgramText = ShowProgramText + temp_label;
	SetWindowText(ShowProgramText);
//	((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
	 

	
	//my_panel = bac_gloab_panel; //Set the panel number
	AnalysisString.Empty();

	 

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
	mParent_Hwnd = g_hwnd_now;
	//prg_color_change = false;

	m_program_edit_hwnd = this->m_hWnd;
	g_hwnd_now = m_program_edit_hwnd;

	copy_data_to_ptrpanel(TYPE_ALL);
	memset(my_display, 0, sizeof(my_display));
	 
	char * temp_point;
	temp_point = desassembler_program();
	if (temp_point == NULL)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Decode Error!"));
		return 1;
	}
	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Decode success!"));
	show_upper = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Upper Case"), 1, g_cstring_ini_path);
	CString temp;
	int  len = 0;
	len = strlen(my_display); //str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, my_display, -1, NULL, 0);
	::MultiByteToWideChar(CP_ACP, 0, my_display, -1, temp.GetBuffer(unicodeLen), unicodeLen);
	temp.ReleaseBuffer();
	CString temp1 = temp;
	if (show_upper)
	{
		temp1.MakeUpper();
	}
	else
		temp1.MakeLower();

	temp1.Replace(_T("\r\n"), _T(" \r\n"));
	temp1.Replace(_T("\("), _T(" \( "));
	temp1.Replace(_T("\)"), _T(" \) "));
	temp1.Replace(_T("  "), _T(" "));
	temp1.Replace(_T("  "), _T(" "));
	temp1.Replace(_T("  "), _T(" "));


	// TODO:  Add extra initialization here
	 m_programeditor.put_Code(temp1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
