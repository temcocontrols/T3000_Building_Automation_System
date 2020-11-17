// DebugWindow.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DebugWindow.h"
#include "afxdialogex.h"
#include "global_function.h"

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
	DDX_Control(pDX, IDC_DATETIMEPICKER_DEBUG_TIME_DAY, m__day);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DEBUG_TIME_SECOND, m__time);
	DDX_Control(pDX, IDC_COMBO_DEBUG_CHOOSE, m_debug_window_combo_show);
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
ON_BN_CLICKED(IDC_BUTTON_DEBUG_NUM_TO_TIME, &CDebugWindow::OnBnClickedButtonDebugNumToTime)
ON_BN_CLICKED(IDC_BUTTON_DEBUG_TIME_TO_NUM, &CDebugWindow::OnBnClickedButtonDebugTimeToNum)
ON_CBN_SELCHANGE(IDC_COMBO_DEBUG_CHOOSE, &CDebugWindow::OnCbnSelchangeComboDebugChoose)
ON_BN_CLICKED(IDC_BUTTON_DEBUG, &CDebugWindow::OnBnClickedButtonDebug)
ON_BN_CLICKED(IDC_BUTTON_WEB_TEST, &CDebugWindow::OnBnClickedButtonWebTest)
END_MESSAGE_MAP()


// CDebugWindow message handlers
	

BOOL CDebugWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
#ifdef DEBUG
    GetDlgItem(IDC_BUTTON_DEBUG)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_BUTTON_WEB_TEST)->ShowWindow(SW_SHOW);
#endif
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
			fun(this->GetSafeHwnd(),0,255,LWA_ALPHA);   //0,1,2,3,4,   
		 FreeLibrary(hInst); 
	} 
	h_debug_window = this->m_hWnd;

	m_debug_window_combo_show.ResetContent();

	for (int i=0;i<sizeof(Debug_Combo_Choose)/sizeof(Debug_Combo_Choose[0]);i++)
	{
		m_debug_window_combo_show.AddString(Debug_Combo_Choose[i]);
	}
	m_debug_window_combo_show.SetWindowTextW(Debug_Combo_Choose[0]);
	debug_item_show = DEBUG_SHOW_NOTHING;
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
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if((pMsg->wParam == VK_RETURN))
			return 1;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//隐藏调试窗口;
void CDebugWindow::OnClose()
{
	 
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void CDebugWindow::Change_Transparency(int persent)
{
	//int Transparency;

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
	 
		int persent = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_DEBUG))->GetPos();
		Change_Transparency(persent);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDebugWindow::OnBnClickedButtonDebugClearall()
{
	
	m_debug_listbox.ResetContent();
}

//pause or resume the message , post from other window. just for debug .
void CDebugWindow::OnBnClickedButtonDebugPause()
{
	
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

	
	if(m_debug_listbox.GetSafeHwnd()==NULL)   return;
	CRect rect;
	this->GetClientRect(&rect);
	CPoint pot = rect.TopLeft();
	m_debug_listbox.SetWindowPos(NULL, pot.x+10, pot.y+130, 
		rect.Width()- 30, rect.Height() - 130, NULL);
}


void CDebugWindow::OnBnClickedButtonDebugSave()
{
	
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

	
	if (m_plogFile)
	{
		delete m_plogFile;
		m_plogFile=NULL;
	}
}


void CDebugWindow::OnBnClickedButtonDebugNumToTime()
{
	
	CString temp_cs;
	GetDlgItemText(IDC_EDIT_DEBUG_TIME_NUMBER,temp_cs.GetBuffer(MAX_PATH),MAX_PATH);
	temp_cs.ReleaseBuffer();
	unsigned long long_time = _wtoi64(temp_cs);

	CTime time_scaletime;
	time_t scale_time  = long_time;
	time_scaletime = scale_time;

	CTime	TimeTemp;
	int temp_year;
	int temp_mon;
	int temp_day;
	int temp_hour;
	int temp_min;
	int temp_sec;
	temp_year = time_scaletime.GetYear();
	temp_mon = time_scaletime.GetMonth();
	temp_day = time_scaletime.GetDay();
	temp_hour = time_scaletime.GetHour();
	temp_min = time_scaletime.GetMinute();
	temp_sec = time_scaletime.GetSecond();
	TimeTemp = CTime(temp_year,temp_mon,temp_day,temp_hour,temp_min,temp_sec);

	m__day.SetTime(&TimeTemp);
	m__time.SetTime(&TimeTemp);

}


void CDebugWindow::OnBnClickedButtonDebugTimeToNum()
{
	
	CTime temp_start_day;
	CTime temp_start_time;
	m__day.GetTime(temp_start_day);
	m__time.GetTime(temp_start_time);
	int start_year = temp_start_day.GetYear();
	int start_month = temp_start_day.GetMonth();
	int start_day = temp_start_day.GetDay();
	int start_hour = temp_start_time.GetHour();
	int start_minute = temp_start_time.GetMinute();
	int start_sec = temp_start_time.GetSecond();

	CTime temp_start(start_year,start_month,start_day,start_hour,start_minute,start_sec);

	unsigned long end_long_time = temp_start.GetTime();
	CString temp_time_num;
	temp_time_num.Format(_T("%u"),end_long_time);
	SetDlgItemTextW(IDC_EDIT_DEBUG_TIME_NUMBER,temp_time_num);
}


void CDebugWindow::OnCbnSelchangeComboDebugChoose()
{
	
	CString temp_string;
	int nSel = m_debug_window_combo_show.GetCurSel();	
	m_debug_window_combo_show.GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Debug_Combo_Choose)/sizeof(Debug_Combo_Choose[0]);i++)
	{
		if(temp_string.CompareNoCase(Debug_Combo_Choose[i]) == 0 )
		{
			debug_item_show = i;
			break;
		}
	}
}


void CDebugWindow::OnBnClickedButtonDebug()
{
    // TODO: 在此添加控件通知处理程序代码
//#ifdef DEBUG
//    int test_pid = 126;
//    StrPostProductID test2 = { 0 };
//    test2.k_serialNumber.tValue.n_value = 123;
//    test2.k_serialNumber.nStatus = VALUE_TYPE_INT;
//    test2.k_ipAddress.nStatus = VALUE_TYPE_CSTRING;
//    test2.k_ipAddress.tValue.cs_value = _T("192.168.0.33");
//    test2.k_ipPort.nStatus = VALUE_TYPE_INT;
//    test2.k_ipPort.tValue.n_value = 666;
//
//    test2.k_hardwareVer.nStatus = VALUE_TYPE_CSTRING;
//    test2.k_hardwareVer.tValue.cs_value = _T("123");
//
//    test2.k_softwareVer.nStatus = VALUE_TYPE_CSTRING;
//    test2.k_softwareVer.tValue.cs_value = _T("56");
//    https_post(test_pid, test2);
//#endif
//    return;


#ifdef DEBUG
#ifdef ENABLE_HTTP_FUCTION
    StrGetProductID test1 = { 0 };
    int test_pid2 = 74;
    https_get(test_pid2, test1);
    CString cs_temp;
    cs_temp.Format(_T("\
%s : %d\r\n \
%s : %d\r\n \
%s : %s\r\n \
%s : %d\r\n \
%s : %.1f\r\n \
%s : %.1f\r\n \
%s : %s\r\n \
%s : %s\r\n \
%s : %s\r\n "),
test1.k_productID.KeyName, test1.k_productID.tValue.n_value,
test1.k_serialNumber.KeyName, test1.k_serialNumber.tValue.n_value,
test1.k_ipAddress.KeyName, test1.k_ipAddress.tValue.cs_value,
test1.k_ipPort.KeyName, test1.k_ipPort.tValue.n_value,
test1.k_hardwareVer.KeyName, test1.k_hardwareVer.tValue.f_value,
test1.k_softwareVer.KeyName, test1.k_softwareVer.tValue.f_value,
test1.k_lastCommunicationDate.KeyName, test1.k_lastCommunicationDate.tValue.cs_value,
test1.k_createdAt.KeyName, test1.k_createdAt.tValue.cs_value,
test1.k_updatedAt.KeyName, test1.k_updatedAt.tValue.cs_value);
    MessageBox(cs_temp);
    Sleep(1);
#endif

#endif // DEBUG
}

#include "BacnetWeb.h"
void CDebugWindow::OnBnClickedButtonWebTest()
{
    // TODO: 在此添加控件通知处理程序代码
    mul_ping_flag = false; //关闭 ping 的命令;
    for (int j = 1; j < 254; j++)
    {
        g_ipaddress_info.ip_status[j] = 0;
    }
    DFTrace(_T("Close Ping thread success!!"));
#ifdef DEBUG
    CBacnetWeb testweb;
    testweb.DoModal();
    return;
#endif // DEBUG
}
