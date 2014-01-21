// BacnetAlarmWindow.cpp : implementation file
// Add by Fance ,this window is used for show the ALARM in the right corner of the T3000.
// if some bacnet message pop up, it will blink the "WARNING".

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAlarmWindow.h"
#include "afxdialogex.h"


// CBacnetAlarmWindow dialog
CBacnetAlarmWindow * AlarmWindow_Window = NULL;
CRect my_view_rect;	//用来存储 窗体位置;
CRect warning_window_rect;
IMPLEMENT_DYNAMIC(CBacnetAlarmWindow, CDialogEx)

CBacnetAlarmWindow::CBacnetAlarmWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAlarmWindow::IDD, pParent)
{

}

CBacnetAlarmWindow::~CBacnetAlarmWindow()
{
}

void CBacnetAlarmWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BAC_ALARM, m_static_bac_alarm);
}


BEGIN_MESSAGE_MAP(CBacnetAlarmWindow, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(MY_REDRAW_WINDOW, RedrawScreeneditWindow)
END_MESSAGE_MAP()


// CBacnetAlarmWindow message handlers


BOOL CBacnetAlarmWindow::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN )
	{
		if((pMsg->wParam == VK_RETURN) ||(pMsg->wParam == VK_ESCAPE))
			return 0;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CBacnetAlarmWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	m_static_bac_alarm.SetWindowTextW(_T("ALARM"));
	m_static_bac_alarm.textColor(RGB(255,0,0));
	m_static_bac_alarm.bkColor(RGB(0,0,0));
	m_static_bac_alarm.setFont(32,10,NULL,_T("Arial"));

	SetTimer(1,200,NULL);
	m_alarmwindow_dlg_hwnd = this->m_hWnd;
	::GetWindowRect(BacNet_hwd,&my_view_rect);	//获取 view的窗体大小;
	GetWindowRect(&warning_window_rect);
	//warning_window_rect.Height();
	MoveWindow(my_view_rect.right - warning_window_rect.Width() ,my_view_rect.top/* - warning_window_rect.Height()*/,
		warning_window_rect.Width(),warning_window_rect.Height(),1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT  CBacnetAlarmWindow::RedrawScreeneditWindow(WPARAM wParam, LPARAM lParam)
{

	::GetWindowRect(BacNet_hwd,&my_view_rect);	//获取 view的窗体大小;
	GetWindowRect(&warning_window_rect);
	//warning_window_rect.Height();
	MoveWindow(my_view_rect.right - warning_window_rect.Width() ,my_view_rect.top,
		warning_window_rect.Width(),warning_window_rect.Height(),1);
	Invalidate();
	return 0;
}

void CBacnetAlarmWindow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int n_count = 0;
	switch(nIDEvent)
	{
	case 1:
		{
			n_count = (++n_count)%20;
			if((n_count == 0) || (n_count == 2) || (n_count == 4) || (n_count == 6) || (n_count == 8))
			{
				m_static_bac_alarm.textColor(RGB(255,0,0));
				m_static_bac_alarm.bkColor(RGB(0,0,0));
				m_static_bac_alarm.SetWindowTextW(_T(""));
			}
			else if((n_count == 1) || (n_count == 3) || (n_count == 5) || (n_count == 7) || (n_count == 9))
			{
				m_static_bac_alarm.textColor(RGB(255,0,0));
				m_static_bac_alarm.bkColor(RGB(0,0,0));
				m_static_bac_alarm.SetWindowTextW(_T("ALARM"));
			}
			else if(n_count ==10)
			{
				m_static_bac_alarm.textColor(RGB(0,0,0));
				m_static_bac_alarm.bkColor(RGB(255,0,0));
				m_static_bac_alarm.SetWindowTextW(_T("ALARM"));
			}
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
