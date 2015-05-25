// DebugWindow.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DebugWindow.h"
#include "afxdialogex.h"
#include "globle_function.h"

// CDebugWindow dialog

IMPLEMENT_DYNAMIC(CDebugWindow, CDialogEx)

CDebugWindow::CDebugWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugWindow::IDD, pParent)
{
	m_plogFile=new CStdioFile;
}

CDebugWindow::~CDebugWindow()
{
}

void CDebugWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEBUG, m_debug_listbox);
}


BEGIN_MESSAGE_MAP(CDebugWindow, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_ADD_DEBUG_CSTRING,ShowString)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BUTTON_DEBUG_CLEARALL, &CDebugWindow::OnBnClickedButtonDebugClearall)
ON_BN_CLICKED(IDC_BUTTON_DEBUG_PAUSE, &CDebugWindow::OnBnClickedButtonDebugPause)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_BUTTON_DEBUG_SAVE, &CDebugWindow::OnBnClickedButtonDebugSave)
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDebugWindow message handlers
	

BOOL CDebugWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	// TODO:  Add extra initialization here
	Logfile_path.Empty();
	m_is_pause = false;
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);  
	HINSTANCE    hInst    =    LoadLibrary(_T("User32.DLL"));  
	if(hInst)  
	{      
		typedef    BOOL    (WINAPI    *MYFUNC)(HWND,COLORREF,BYTE,DWORD);      
		MYFUNC    fun    =    NULL;      //取¨?得Ì?SetLayeredWindowAttributes函¡¥数ºy指?针?     
		fun=(MYFUNC)GetProcAddress(hInst,    "SetLayeredWindowAttributes");      
		if(fun)
			fun(this->GetSafeHwnd(),0,180,LWA_ALPHA);   //0,1,2,3,4,   
		FreeLibrary(hInst);  
	} 
	h_debug_window = this->m_hWnd;

	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_DEBUG))->SetRange(100,255);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
LRESULT CDebugWindow::ShowString(WPARAM wParam,LPARAM lParam)
{
	if(m_is_pause)
		return 0;
	CString temp_cs;
	temp_cs.Format(_T("%s"),(wchar_t *)wParam);
		m_debug_listbox.AddString(temp_cs);
		m_debug_listbox.SetTopIndex(m_debug_listbox.GetCount() - 1);
	return 0;
}

BOOL CDebugWindow::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if((pMsg->wParam == VK_RETURN))
			return 1;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDebugWindow::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void CDebugWindow::Change_Transparency(int persent)
{
	//int Transparency;
	//Transparency = persent * 255 / 100;
	//if(Transparency < 100)
	//	Transparency = 100;
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);  
	HINSTANCE    hInst    =    LoadLibrary(_T("User32.DLL"));  
	if(hInst)  
	{      
		typedef    BOOL    (WINAPI    *MYFUNC)(HWND,COLORREF,BYTE,DWORD);      
		MYFUNC    fun    =    NULL;      //取¨?得Ì?SetLayeredWindowAttributes函¡¥数ºy指?针?     
		fun=(MYFUNC)GetProcAddress(hInst,    "SetLayeredWindowAttributes");    
		if(fun)
			fun(this->GetSafeHwnd(),0,persent,LWA_ALPHA);   //0,1,2,3,4,   
		FreeLibrary(hInst);  
	} 
}


void CDebugWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
		int persent = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_DEBUG))->GetPos();
		Change_Transparency(persent);
		//Invalidate();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDebugWindow::OnBnClickedButtonDebugClearall()
{
	// TODO: Add your control notification handler code here
	m_debug_listbox.ResetContent();
}


void CDebugWindow::OnBnClickedButtonDebugPause()
{
	// TODO: Add your control notification handler code here
	if(m_is_pause)
	{
		m_is_pause = false;
		SetDlgItemTextW(IDC_BUTTON_DEBUG_PAUSE,_T("Pause"));
	}
	else
	{
		m_is_pause = true;
		SetDlgItemTextW(IDC_BUTTON_DEBUG_PAUSE,_T("Resume"));
	}
	
}





void CDebugWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_debug_listbox.GetSafeHwnd()==NULL)   return;
	CRect rect;
	this->GetClientRect(&rect);
	CPoint pot = rect.TopLeft();
	m_debug_listbox.SetWindowPos(NULL, pot.x+10, pot.y+60, 
		rect.Width()- 45, rect.Height() - 80, NULL);
}


void CDebugWindow::OnBnClickedButtonDebugSave()
{
	// TODO: Add your control notification handler code here
	CTime temp_time = CTime::GetCurrentTime();
	CString strtime = temp_time.Format(_T("%I_%M_%S_%p"));
	Logfile_path = g_achive_folder + _T("\\") + strtime + _T(".txt");
	if (m_debug_listbox.GetCount()!=0)
	{

		if(m_plogFile->Open(Logfile_path.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
		{
			for (int i=0;i<m_debug_listbox.GetCount();i++)
			{
				CString logInfo;
				m_debug_listbox.GetText(i,logInfo);
				m_plogFile->WriteString(logInfo+_T("\n"));
			}


			m_plogFile->Close();
		}
		else
		{
			ASSERT(0);
			m_plogFile->Close();

		}
		ShellExecute(this->m_hWnd, _T("open"), Logfile_path, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDebugWindow::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (m_plogFile)
	{
		delete m_plogFile;
		m_plogFile=NULL;
	}
}
