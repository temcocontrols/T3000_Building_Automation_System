// T3000UpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000UpdateDlg.h"
#include "afxdialogex.h"
#include "global_function.h"


// CT3000UpdateDlg dialog

IMPLEMENT_DYNAMIC(CT3000UpdateDlg, CDialogEx)

CT3000UpdateDlg::CT3000UpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CT3000UpdateDlg::IDD, pParent)
    , versioninfor(_T(""))
{
m_new=false;
}

CT3000UpdateDlg::~CT3000UpdateDlg()
{
}

void CT3000UpdateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, versioninfor);
}


BEGIN_MESSAGE_MAP(CT3000UpdateDlg, CDialogEx)
    ON_BN_CLICKED(IDOK_DOWNLOAD, &CT3000UpdateDlg::OnBnClickedDownload)
END_MESSAGE_MAP()


// CT3000UpdateDlg message handlers


BOOL CT3000UpdateDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CString version=_T("2014.6.6");
    TCHAR buffer[MAX_PATH];
    CString	m_szAddress=_T("web1232.ixwebhosting.com");
    CString	m_szFilename=_T("/software/T3000_Version.txt");
    CString	m_szPassword=_T("Travel123");
    CString	m_szUsername=_T("temcoftp");
    CString	m_szVersion;
    CString m_CurrentVersion;
    BOOL bUpdate =CheckForUpdate(m_szAddress, m_szUsername, m_szPassword,
        m_szFilename, m_szVersion, buffer);
    if ( bUpdate )
    {
        CString text;

        if ( _tcscmp( buffer, version.GetBuffer() ) != 0 )//当前版本不和服务器保持一致，提醒更新
        {
        //, buffer,version.GetBuffer()
            versioninfor.Format( _T("There is a new version of  T3000 available,Please update your T3000 !\n Update now to get the lastest feastures and improvements."));

            m_new=true;
            //int result = MessageBox( text, _T("New version available"), MB_YESNO|MB_ICONINFORMATION );
            //if ( result == IDYES )
            //{
            //    ShellExecute( GetDesktopWindow()->m_hWnd, _T("open"),
            //        _T("http://temcocontrols.com/ftp/software/9TstatSoftware.zip"), NULL, NULL, SW_SHOWMAXIMIZED );
            //}
        }
        else//保持一致的话，就对了，说明是最新的
        {
            versioninfor.Format( _T("Your T3000 is the lastest version \nT3000 Ver=%s"), version.GetBuffer());
            m_new=false;
             
        }
       
    }
    else
    {
       versioninfor.Format(_T("Can't connect to temco's website \nPlease check your connection of the internet!"));
       m_new=false;
    }
     UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CT3000UpdateDlg::OnBnClickedDownload()
{
    
    if (m_new)
    {
            ShellExecute( GetDesktopWindow()->m_hWnd, _T("open"),
                 _T("http://temcocontrols.com/ftp/software/9TstatSoftware.zip"), NULL, NULL, SW_SHOWMAXIMIZED );
    }
    else
    {
      AfxMessageBox(_T("Your T3000 is the lastest version!"));
    }
}
