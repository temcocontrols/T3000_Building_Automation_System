// StatusbarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "StatusbarCtrl.h"
#include "afxdialogex.h"
#include "T3000LogWindow.h"

HDC status_gloab_hdc;
HDC status_hMemDC;
HBITMAP status_hBmp; //用于双缓冲;
CRect status_myRect;
CString cs_show_status_info;

#define STATUS_CHARACTER_VALUE_OFFSET 35
#define STATUS_HEALTH_VALUE_OFFSET 50


#define STATUS_CHARACTER_WIDTH 30
#define STATUS_HEALTH_CHARACTER_WIDTH 40
#define STATUS_ALARM_ICON_WIDTH 20
unsigned int window_width;
unsigned int window_height;
unsigned int persent_add = 12;
unsigned int persent_tx = 12;
unsigned int persent_rx = 12;
unsigned int persent_err = 12;
unsigned int persent_health = 12;
unsigned int persent_progress = 40;
unsigned int add_start_pos = 0;
unsigned int add_end_pos;
unsigned int add_value_pos;
unsigned int tx_start_pos;
unsigned int tx_end_pos;
unsigned int tx_value_pos;
unsigned int rx_start_pos;
unsigned int rx_end_pos;
unsigned int rx_value_pos;
unsigned int err_start_pos;
unsigned int err_end_pos;
unsigned int err_value_pos;
unsigned int health_start_pos;
unsigned int health_end_pos;
unsigned int health_value_pos;
unsigned int progress_start_pos;
unsigned int progress_end_pos;
unsigned int alarm_start_pos;

unsigned int add_width;
unsigned int tx_width;
unsigned int rx_width;
unsigned int err_width;
unsigned int health_width;
unsigned int progress_width;

bool b_statusbarthreadflag = true;

DWORD WINAPI MyStatusBarThread(LPVOID lPvoid);
// CMyStatusbarCtrl dialog

IMPLEMENT_DYNAMIC(CMyStatusbarCtrl, CDialogEx)

CMyStatusbarCtrl::CMyStatusbarCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyStatusbarCtrl::IDD, pParent)
{
}

CMyStatusbarCtrl::~CMyStatusbarCtrl()
{
}

void CMyStatusbarCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyStatusbarCtrl, CDialogEx)
	ON_MESSAGE(WM_SHOW_STATUS_TEXT,ShowProgressText)
	ON_WM_CLOSE()
	ON_WM_PAINT()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CMyStatusbarCtrl message handlers
CString temp_statusbar_log_path;
int n_count = 0;
HANDLE mystatusbarthread = NULL;

BOOL CMyStatusbarCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_statusbar_hwnd = this->m_hWnd;
	
	if (mystatusbarthread == NULL)
	{
		mystatusbarthread = CreateThread(NULL,NULL, MyStatusBarThread, this,NULL,NULL);
		CloseHandle(mystatusbarthread);
	}



    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    temp_statusbar_log_path = ApplicationFolder + _T("\\Database\\temp\\statusbarlog.ini");

    WritePrivateProfileStringW(_T("Setting"),NULL, NULL, temp_statusbar_log_path);
    WritePrivateProfileStringW(_T("Log"), NULL, NULL, temp_statusbar_log_path);

	InitStatusBarDC();
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMyStatusbarCtrl::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMyStatusbarCtrl::OnClose()
{
	 

	CDialogEx::OnClose();
}


void CMyStatusbarCtrl::OnCancel()
{
	

	CDialogEx::OnCancel();
}

int bitmap_width;
int bitmap_height;

BOOL CMyStatusbarCtrl::InitStatusBarDC()
{
	status_gloab_hdc = ::GetDC(this->m_hWnd);
	if (status_gloab_hdc == NULL)
		return FALSE;
	status_hMemDC = ::CreateCompatibleDC(NULL);
	if (status_hMemDC == NULL)
		return FALSE;
	::GetWindowRect(this->m_hWnd, &status_myRect);

	status_hBmp = ::CreateCompatibleBitmap(status_gloab_hdc, status_myRect.Width(), status_myRect.Height());
	if (status_hBmp == NULL)
		return FALSE;
	else
	{
		bitmap_width = status_myRect.Width();
		bitmap_height = status_myRect.Height();
	}
	SelectObject(status_hMemDC, status_hBmp);
	//hMemDC2=::CreateCompatibleDC(gloab_hdc);

	return TRUE;
}


DWORD WINAPI MyStatusBarThread(LPVOID lPvoid)
{
	CMyStatusbarCtrl* mparent = (CMyStatusbarCtrl *)lPvoid;
	Sleep(2000);
	while (b_statusbarthreadflag)
	{
		::GetWindowRect(mparent->m_hWnd, &status_myRect);


		if ((bitmap_width != status_myRect.Width()) || (bitmap_height != status_myRect.Height()))
		{
			status_hBmp = ::CreateCompatibleBitmap(status_gloab_hdc, status_myRect.Width(), status_myRect.Height());
			if (status_hBmp == NULL)
			{
				Sleep(100);
				continue;
			}
			else
			{
				bitmap_width = status_myRect.Width();
				bitmap_height = status_myRect.Height();
			}
		}


		window_width = status_myRect.Width();
		window_height = status_myRect.Height() - 1;

		if (window_width > 1400)
		{
			persent_add = 6;
			persent_tx = 6;
			persent_rx = 6;
			persent_err = 6;
			persent_health = 6;
			persent_progress = 70;
		}
		else if ((window_width <= 1400) && (window_width > 1000))
		{
			persent_add = 8;
			persent_tx = 8;
			persent_rx = 8;
			persent_err = 8;
			persent_health = 8;
			persent_progress = 60;
		}
		else if ((window_width <= 1000) && (window_width > 800))
		{
			persent_add = 10;
			persent_tx = 10;
			persent_rx = 10;
			persent_err = 10;
			persent_health = 10;
			persent_progress = 50;
		}
		else if ((window_width <= 800) && (window_width > 600))
		{
			persent_add = 12;
			persent_tx = 12;
			persent_rx = 12;
			persent_err = 12;
			persent_health = 12;
			persent_progress = 40;
		}
		else
		{
			Sleep(100);
			continue;
		}

		add_width = window_width * persent_add / 100;
		tx_width = window_width * persent_tx / 100;
		rx_width = window_width * persent_rx / 100;
		err_width = window_width * persent_err / 100;
		health_width = window_width * persent_health / 100;
		progress_width = window_width * persent_progress / 100;
		add_end_pos = add_width;
		tx_start_pos = add_end_pos;
		tx_end_pos = tx_width + add_end_pos;
		rx_start_pos = tx_end_pos;
		rx_end_pos = rx_width + tx_end_pos;
		err_start_pos = rx_end_pos;
		err_end_pos = err_width + rx_end_pos;
		health_start_pos = err_end_pos;
		health_end_pos = health_width + err_end_pos;
		progress_start_pos = health_end_pos;
		progress_end_pos = progress_width + health_end_pos;
		alarm_start_pos = progress_end_pos;

		SelectObject(status_hMemDC, status_hBmp);
		mparent->DrawStatusBar(status_hMemDC);
		BitBlt(status_gloab_hdc, 0, 0, status_myRect.right, status_myRect.bottom, status_hMemDC, 0, 0, SRCCOPY);//将绘制完成的内存位图贴到的Picture空间对象中;
		for (int z = 0; z < 100; z++)
		{
			if (b_statusbarthreadflag)
				Sleep(1);
			else
			{
				mystatusbarthread = NULL;
				return 0;
			}
		}
	}
	mystatusbarthread = NULL;
	return 0;
}

//重绘状态栏 ， 状态栏的大小会根据窗体的大小变动;
void CMyStatusbarCtrl::DrawStatusBar(HDC my_hdc)
{
	Graphics* mygraphics;
	mygraphics = new Graphics(status_hMemDC);
	mygraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	Pen* myRectangle_pen;
	SolidBrush* BlackBrush;
	SolidBrush* CharacterblkBrush;
	SolidBrush* ProgressblkBrush;
	SolidBrush* HealthGreenBrush;
	SolidBrush* HealthRedBrush;

	SolidBrush Font_brush(Color(255, 0, 0, 0));
	FontFamily StaticfontFamily(_T("Arial"));
	Gdiplus::Font character_font(&StaticfontFamily, 12, FontStyleRegular, UnitPixel);
	PointF staticpointF(0, 0);

	HealthGreenBrush = new SolidBrush(Color(255, 38, 182, 69));
	HealthRedBrush = new SolidBrush(Color(255, 255, 0, 0));
	ProgressblkBrush = new SolidBrush(Color(255, 0, 211, 39));
	CharacterblkBrush = new SolidBrush(Color(255, 183, 183, 183));
	BlackBrush = new SolidBrush(Color(255, 228, 228, 228));
	mygraphics->FillRectangle(BlackBrush, 0, 0, status_myRect.right, status_myRect.bottom);


	myRectangle_pen = new Pen(Color(255, 0, 0, 0), 1.0f);
	mygraphics->DrawRectangle(myRectangle_pen, 1, 0, add_width, window_height);
	mygraphics->DrawRectangle(myRectangle_pen, 1, 0,STATUS_CHARACTER_WIDTH, window_height);
	mygraphics->FillRectangle(CharacterblkBrush, 1, 0,STATUS_CHARACTER_WIDTH, window_height);

	mygraphics->DrawRectangle(myRectangle_pen, tx_start_pos, 0, tx_width, window_height);
	mygraphics->DrawRectangle(myRectangle_pen, tx_start_pos, 0,STATUS_CHARACTER_WIDTH, window_height);
	mygraphics->FillRectangle(CharacterblkBrush, tx_start_pos, 0,STATUS_CHARACTER_WIDTH, window_height);

	mygraphics->DrawRectangle(myRectangle_pen, rx_start_pos, 0, rx_width, window_height);
	mygraphics->DrawRectangle(myRectangle_pen, rx_start_pos, 0,STATUS_CHARACTER_WIDTH, window_height);
	mygraphics->FillRectangle(CharacterblkBrush, rx_start_pos, 0,STATUS_CHARACTER_WIDTH, window_height);

	mygraphics->DrawRectangle(myRectangle_pen, err_start_pos, 0, err_width, window_height);
	mygraphics->DrawRectangle(myRectangle_pen, err_start_pos, 0,STATUS_CHARACTER_WIDTH, window_height);
	mygraphics->FillRectangle(CharacterblkBrush, err_start_pos, 0,STATUS_CHARACTER_WIDTH, window_height);

	mygraphics->DrawRectangle(myRectangle_pen, health_start_pos, 0, health_width, window_height);
	mygraphics->DrawRectangle(myRectangle_pen, health_start_pos, 0,STATUS_HEALTH_CHARACTER_WIDTH, window_height);
	mygraphics->FillRectangle(CharacterblkBrush, health_start_pos, 0,STATUS_HEALTH_CHARACTER_WIDTH, window_height);

	mygraphics->DrawRectangle(myRectangle_pen, progress_start_pos, 0, progress_width - STATUS_ALARM_ICON_WIDTH, window_height);

	CString temp_value;


	staticpointF.X = 1;
	staticpointF.Y = 1;
	mygraphics->DrawString(_T("Add"), -1, &character_font, staticpointF, &Font_brush);

	staticpointF.X = add_start_pos + STATUS_CHARACTER_VALUE_OFFSET;
	mygraphics->DrawString(Statuspanel, -1, &character_font, staticpointF, &Font_brush);


	staticpointF.X = tx_start_pos + 1;
	mygraphics->DrawString(_T("Tx"), -1, &character_font, staticpointF, &Font_brush);

	staticpointF.X = tx_start_pos + STATUS_CHARACTER_VALUE_OFFSET;
	temp_value.Format(_T("%u"), g_llTxCount);
	mygraphics->DrawString(temp_value, -1, &character_font, staticpointF, &Font_brush);


	staticpointF.X = rx_start_pos + 1;
	mygraphics->DrawString(_T("Rx"), -1, &character_font, staticpointF, &Font_brush);

	staticpointF.X = rx_start_pos + STATUS_CHARACTER_VALUE_OFFSET;
	temp_value.Format(_T("%u"), g_llRxCount);
	mygraphics->DrawString(temp_value, -1, &character_font, staticpointF, &Font_brush);


	staticpointF.X = err_start_pos + 1;
	mygraphics->DrawString(_T("Err"), -1, &character_font, staticpointF, &Font_brush);

	staticpointF.X = err_start_pos + STATUS_CHARACTER_VALUE_OFFSET;

	//这一段主要是处理有些命令是设备多余回复出来的，例如 发一条，设备回复两条，这样就不太好计算 error 的个数;
	static int n_err_show_count = 0;
	if (++n_err_show_count == 50)
	{
		n_err_show_count = 0;
		if (g_llTxCount - g_llRxCount < 0)
		{
			g_llRxCount = g_llTxCount;
			g_llerrCount = 0;
		}
		else if (g_llTxCount < g_llerrCount + g_llRxCount)
		{
			g_llRxCount = g_llTxCount - g_llerrCount;
		}
		else
		{
			g_llerrCount = g_llTxCount - g_llRxCount;
		}
	}
	if (g_llTxCount > old_tx_count) //每发送一次才进来一次;
	{
		old_tx_count = g_llTxCount;
		if (persent_array_count < 100)
		{
			Tx_array[persent_array_count] = g_llTxCount;
			Rx_array[persent_array_count] = g_llRxCount;
			if (Rx_array[persent_array_count] > Rx_array[0])
			{
                if(Tx_array[persent_array_count] != Tx_array[0])
				    m_health_persent = ((Rx_array[persent_array_count] - Rx_array[0]) * 100) / (Tx_array[persent_array_count] - Tx_array[0]);
				if (m_health_persent > 100)
					m_health_persent = 100;
			}
			else
				m_health_persent = 0;
			persent_array_count ++;
		}
		else
		{
			unsigned int temp_tx_value = 0;
			unsigned int temp_rx_value = 0;
			for (int z = 0; z < 99; z++)
			{
				Tx_array[z] = Tx_array[z + 1];
				Rx_array[z] = Rx_array[z + 1];
			}

			Tx_array[99] = g_llTxCount;
			Rx_array[99] = g_llRxCount;
			if (Rx_array[99] > Rx_array[0])
			{
                if(Tx_array[99] != Tx_array[0])
				    m_health_persent = ((Rx_array[99] - Rx_array[0]) * 100) / (Tx_array[99] - Tx_array[0]);
				if (m_health_persent > 100)
					m_health_persent = 100;
			}
			else
				m_health_persent = 0;
		}
	}


	temp_value.Format(_T("%u"), g_llerrCount);
	mygraphics->DrawString(temp_value, -1, &character_font, staticpointF, &Font_brush);

	CString cs_health_persent;
	cs_health_persent.Format(_T("%d%%"), m_health_persent);

	if (!bac_select_device_online)
	{
		m_health_persent = 0;
		cs_health_persent = _T("0%");
	}
	if (m_health_persent == 0)
	{
		mygraphics->FillRectangle(HealthRedBrush, health_start_pos + STATUS_HEALTH_CHARACTER_WIDTH + 1, 1, health_width - STATUS_HEALTH_CHARACTER_WIDTH - 2, window_height - 2);
		staticpointF.X = health_start_pos + STATUS_HEALTH_VALUE_OFFSET;
		//mygraphics->DrawString(_T("0%"),-1, &character_font,staticpointF,&Font_brush);
		mygraphics->DrawString(cs_health_persent, -1, &character_font, staticpointF, &Font_brush);
	}
	else
	{
		int persent_value = 0;
		persent_value = (health_width - STATUS_HEALTH_CHARACTER_WIDTH - 2) * (((float)m_health_persent) / 100);
		mygraphics->FillRectangle(HealthGreenBrush, health_start_pos + STATUS_HEALTH_CHARACTER_WIDTH + 1, 1, persent_value, window_height - 2);
		staticpointF.X = health_start_pos + STATUS_HEALTH_VALUE_OFFSET;
		mygraphics->DrawString(cs_health_persent, -1, &character_font, staticpointF, &Font_brush);
	}

#if 0
	if(bac_select_device_online)
	{
		mygraphics->FillRectangle(HealthGreenBrush,health_start_pos + STATUS_HEALTH_CHARACTER_WIDTH + 1,1,health_width - STATUS_HEALTH_CHARACTER_WIDTH - 2,window_height - 2);
		staticpointF.X = health_start_pos + STATUS_HEALTH_VALUE_OFFSET;
	//mygraphics->DrawString(_T("100%"),-1, &character_font,staticpointF,&Font_brush);
		mygraphics->DrawString(cs_health_persent,-1, &character_font,staticpointF,&Font_brush);
	}
	else
	{
		mygraphics->FillRectangle(HealthRedBrush,health_start_pos + STATUS_HEALTH_CHARACTER_WIDTH + 1,1,health_width - STATUS_HEALTH_CHARACTER_WIDTH - 2,window_height - 2);
		staticpointF.X = health_start_pos + STATUS_HEALTH_VALUE_OFFSET;
	//mygraphics->DrawString(_T("0%"),-1, &character_font,staticpointF,&Font_brush);
		mygraphics->DrawString(cs_health_persent,-1, &character_font,staticpointF,&Font_brush);
	}
#endif

	staticpointF.X = health_start_pos + 1;
	mygraphics->DrawString(_T("Health"), -1, &character_font, staticpointF, &Font_brush);

	if (g_progress_persent != 0)
	{
		unsigned int temp_width = (progress_width - STATUS_ALARM_ICON_WIDTH) * g_progress_persent / 100;
		CString temp_persent_value;
		temp_persent_value.Format(_T("%d%%"), g_progress_persent);
		mygraphics->FillRectangle(ProgressblkBrush, progress_start_pos, 0, temp_width, window_height);
		staticpointF.X = progress_start_pos + progress_width - 60;
		mygraphics->DrawString(temp_persent_value, -1, &character_font, staticpointF, &Font_brush);
		if (g_progress_persent >= 98)
			g_progress_persent = 0;
		//progress_width
	}
	else
	{
		mygraphics->DrawRectangle(myRectangle_pen, progress_start_pos, 0, progress_width - STATUS_ALARM_ICON_WIDTH, window_height);
	}

	if (!cs_show_status_info.IsEmpty())
	{
		staticpointF.X = progress_start_pos + 1;
		mygraphics->DrawString(cs_show_status_info, -1, &character_font, staticpointF, &Font_brush);
	}


	delete mygraphics;
	delete myRectangle_pen;

	delete BlackBrush;
	delete CharacterblkBrush;
	delete ProgressblkBrush;
    delete HealthGreenBrush;  //2018 03 02 fandu添加  
    delete HealthRedBrush;    //2018 03 02 fandu添加  
}

void CMyStatusbarCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
}

LRESULT CMyStatusbarCtrl::ShowProgressText(WPARAM wParam, LPARAM lParam)
{
	char* temp_my_cs = (char *)wParam;
	CString temp_des2;

	MultiByteToWideChar(CP_ACP, 0, temp_my_cs, (int)strlen((char *)temp_my_cs) + 1,
	                    temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
	temp_des2.ReleaseBuffer();

    CString temp_count;
    temp_count.Format(_T("%d"), n_count);

    WritePrivateProfileStringW(_T("Setting"), _T("LogCount"), temp_count, temp_statusbar_log_path);
    WritePrivateProfileStringW(_T("Log"), temp_count, temp_des2, temp_statusbar_log_path);

    n_count = (n_count++) % 200;

	cs_show_status_info = temp_des2;
	if (temp_my_cs != NULL)
		delete temp_my_cs;
	return 0;
}


void CMyStatusbarCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (T3000LogWindow == NULL)
    {
        T3000LogWindow = new CT3000LogWindow;
        T3000LogWindow->Create(IDD_DIALOG_LOG_WINDOW, this);
        T3000LogWindow->ShowWindow(SW_HIDE);

    }

    if (T3000LogWindow->IsWindowVisible())
    {
    }
    else
        T3000LogWindow->ShowWindow(SW_SHOW);


    CDialogEx::OnLButtonDblClk(nFlags, point);
}
