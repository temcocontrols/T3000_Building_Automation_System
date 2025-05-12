// BacnetAlarmWindow.cpp : implementation file
// Add by Fance ,this window is used for show the ALARM in the right corner of the T3000.
// if some bacnet message pop up, it will blink the "WARNING".

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAlarmWindow.h"
#include "afxdialogex.h"
HBITMAP     show_bitmap;
HBITMAP 	hBitmap_red_bmp;
HBITMAP 	hBitmap_brown_bmp;
HBITMAP 	hBitmap_green_bmp;
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
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CBacnetAlarmWindow message handlers


BOOL CBacnetAlarmWindow::PreTranslateMessage(MSG* pMsg)
{
	
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
	this->ShowWindow(SW_HIDE);
	hBitmap_red_bmp =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  
		MAKEINTRESOURCE(IDB_BITMAP_ALARM_RED),  
		IMAGE_BITMAP,0,0,  
		LR_LOADMAP3DCOLORS);
	
	hBitmap_brown_bmp =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  
		MAKEINTRESOURCE(IDB_BITMAP_ALARM_BROWN),  
		IMAGE_BITMAP,0,0,  
		LR_LOADMAP3DCOLORS);

	hBitmap_green_bmp =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  
		MAKEINTRESOURCE(IDB_BITMAP_ALARM_GREEN),  
		IMAGE_BITMAP,0,0,  
		LR_LOADMAP3DCOLORS);

	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,32,24,SWP_NOMOVE|SWP_SHOWWINDOW);

	m_static_bac_alarm.SetWindowTextW(_T("ALARM"));
	m_static_bac_alarm.textColor(RGB(255,0,0));
	m_static_bac_alarm.bkColor(RGB(0,0,0));
	m_static_bac_alarm.setFont(32,10,NULL,_T("Arial"));
	m_static_bac_alarm.ShowWindow(SW_HIDE);
	SetTimer(1,500,NULL);
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





	::GetWindowRect(MainFram_hwd,&my_view_rect);	//获取 view的窗体大小;
	GetWindowRect(&warning_window_rect);
	//warning_window_rect.Height();
	//MoveWindow(my_view_rect.right - warning_window_rect.Width() ,my_view_rect.top,
	//	warning_window_rect.Width(),warning_window_rect.Height(),1);
	CRect temp_rect;
	temp_rect.left =my_view_rect.left + 390;
	temp_rect.top  =my_view_rect.top + 64;

//	MoveWindow(temp_rect.left ,temp_rect.top,warning_window_rect.Width(),warning_window_rect.Height(),1);
	Invalidate();
	return 0;
}

void CBacnetAlarmWindow::OnTimer(UINT_PTR nIDEvent)
{
	 
	static int n_count = 0;
	switch(nIDEvent)
	{
	case 1:
		{
			n_count = (++n_count)%3;
			if(n_count == 0 )
			{
				show_bitmap = hBitmap_red_bmp;
			}
			else if(n_count == 1)
			{
				show_bitmap = hBitmap_brown_bmp;
			}
			else if(n_count == 2)
			{
				show_bitmap = hBitmap_green_bmp;
			}
			Invalidate(false);
			//if((n_count == 0) || (n_count == 2) || (n_count == 4) || (n_count == 6) || (n_count == 8))
			//{
			//	m_static_bac_alarm.textColor(RGB(255,0,0));
			//	m_static_bac_alarm.bkColor(RGB(0,0,0));
			//	m_static_bac_alarm.SetWindowTextW(_T(""));
			//}
			//else if((n_count == 1) || (n_count == 3) || (n_count == 5) || (n_count == 7) || (n_count == 9))
			//{
			//	m_static_bac_alarm.textColor(RGB(255,0,0));
			//	m_static_bac_alarm.bkColor(RGB(0,0,0));
			//	m_static_bac_alarm.SetWindowTextW(_T("ALARM"));
			//}
			//else if(n_count ==10)
			//{
			//	m_static_bac_alarm.textColor(RGB(0,0,0));
			//	m_static_bac_alarm.bkColor(RGB(255,0,0));
			//	m_static_bac_alarm.SetWindowTextW(_T("ALARM"));
			//}
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetAlarmWindow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages

	CMemDC memDC(dc,this);
	CRect rcClient;
	GetClientRect(&rcClient);
	memDC.GetDC().FillSolidRect(&rcClient,RGB(230,230,230));
	Graphics *mygraphics;
	mygraphics = new Graphics(memDC.GetDC());
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);


	CRect test_rect;
	HWND temp_hwnd = this->m_hWnd;
	::GetWindowRect(temp_hwnd,&test_rect);	//获取 view的窗体大小;
	Bitmap bitmap(show_bitmap,NULL);
	mygraphics->DrawImage(&bitmap,0 ,0,test_rect.Width(),test_rect.Height());


	delete mygraphics;

	//Graphics graphics(memDC.GetDC());
	//SolidBrush *BlackBrush;
	//SolidBrush *CharacterBlackBrush;

	//Graphics *mygraphics;
	//mygraphics = new Graphics(memDC.GetDC());
	//mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	//BlackBrush =new  SolidBrush(MY_COLOR_RED) ;
	//CharacterBlackBrush = new SolidBrush(MY_COLOR_BLACK_CHARACTER);


	//mygraphics->FillRectangle(BlackBrush,0,0,rcClient.Width(),40);

	//FontFamily  CharacterfontFamily(_T("Arial"));
	//PointF     TitlepointF(0, 0);
	//SolidBrush  TitleCharacterColor(Color(255,255,255,255));
	//Gdiplus::Font  Scroll_font(&CharacterfontFamily, 28, FontStyleBold, UnitPixel);
	//TitlepointF.X = 250;
	//TitlepointF.Y = 5;
	//mygraphics->DrawString(_T("Remote Connect procedure"), -1, &Scroll_font, TitlepointF,&TitleCharacterColor);

	//PointF      WarningMessagePoint(0, 0);
	//SolidBrush  WarningMessageColor(Color(255,0,0,0));
	//Gdiplus::Font  WarningMessageFont(&CharacterfontFamily, 21, FontStyleRegular, UnitPixel);
	//SolidBrush  MessageRetColor(Color(255,0,255,255));

	//CString step1_message;

end_connect_paint:

	return;

}

#include "MainFrm.h"
void CBacnetAlarmWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->OnControlAlarmLog();
	CDialogEx::OnLButtonDown(nFlags, point);
}
