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
    DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_option_baudrate);
}


BEGIN_MESSAGE_MAP(CT3000Option, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CT3000Option::OnCbnSelchangeComboLanguage)
    ON_BN_CLICKED(IDOK, &CT3000Option::OnBnClickedOk)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CT3000Option message handlers
vector<CString>				m_szComs;
extern int n_mstp_comport ;
extern int n_mstp_baudrate ;

BOOL CT3000Option::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    GetSerialComPortNumber1(m_szComs);
    

	 m_combox_language.ResetContent();

     for (int i = 0; i < (int)m_szComs.size(); i++)
     {
         m_combox_language.AddString(m_szComs.at(i));
     }
     ::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

     m_option_baudrate.ResetContent();
     for (int x = 0;x< (sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0]));x++)
     {
         m_option_baudrate.AddString(Baudrate_Array[x]);
     }
     show_time_left = 30;
     SetTimer(1, 1000, NULL);

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


void CT3000Option::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string_com;
    m_combox_language.GetWindowTextW(temp_string_com);
    temp_string_com = temp_string_com.Mid(3);
    n_mstp_comport = _wtoi(temp_string_com);

    CString temp_string_baudrate;
    m_option_baudrate.GetWindowTextW(temp_string_baudrate);
    n_mstp_baudrate = _wtoi(temp_string_baudrate);

    CDialogEx::OnOK();
}


void CT3000Option::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    show_time_left--;
    CString cs_value;
    cs_value.Format(_T("%d"), show_time_left);
    if (show_time_left <= 0)
    {
        KillTimer(1);
        PostMessage(WM_CLOSE, NULL, NULL);
    }
    GetDlgItem(IDC_STATIC_OPTION_TIME)->SetWindowTextW(cs_value);
    CDialogEx::OnTimer(nIDEvent);
}
