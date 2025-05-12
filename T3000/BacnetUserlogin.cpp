// BacnetUserlogin.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetUserlogin.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "global_define.h"
// CBacnetUserlogin dialog
CRect userlogin_rect;	//用来存储 窗体应该有多大;
CString bmp_login_path;
HBITMAP 	hBitmap_login;
IMPLEMENT_DYNAMIC(CBacnetUserlogin, CDialogEx)

CBacnetUserlogin::CBacnetUserlogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetUserlogin::IDD, pParent)
{

}

CBacnetUserlogin::~CBacnetUserlogin()
{
}

void CBacnetUserlogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC__LOGIN_NAME, m_name_static);
	DDX_Control(pDX, IDC_STATIC_LOGIN_PASSWORD, m_password_static);
}


BEGIN_MESSAGE_MAP(CBacnetUserlogin, CDialogEx)
	ON_MESSAGE(MY_REDRAW_WINDOW, RedrawLoginWindow)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBacnetUserlogin::OnBnClickedButtonLogin)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBacnetUserlogin message handlers


BOOL CBacnetUserlogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_user_name.Empty();
	m_user_password.Empty();
	
	m_cxScreen=GetSystemMetrics(SM_CXSCREEN);
	m_cyScreen=GetSystemMetrics(SM_CYSCREEN);
	m_user_login_hwnd = this->m_hWnd;
	::GetWindowRect(BacNet_hwd,&userlogin_rect);	//获取 view的窗体大小;
	MoveWindow(userlogin_rect.left,userlogin_rect.top,userlogin_rect.Width(),userlogin_rect.Height(),1);
	Invalidate();
	GetDlgItem(IDC_EDIT_LOGIN_USERNAME)->SetFocus();


	m_name_static.SetWindowTextW(_T("Name :"));
	m_name_static.textColor(RGB(255,255,0));
	m_name_static.bkColor(RGB(0,68,139));
	m_name_static.setFont(20,10,NULL,_T("Arial"));

	m_password_static.SetWindowTextW(_T("Password  :"));
	m_password_static.textColor(RGB(255,255,0));
	m_password_static.bkColor(RGB(0,68,139));
	m_password_static.setFont(20,10,NULL,_T("Arial"));
	
	 	hBitmap_login =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  
		MAKEINTRESOURCE(IDB_BITMAP_LOGIN),  
		IMAGE_BITMAP,0,0,  
		LR_LOADMAP3DCOLORS);  


	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CBacnetScreenEdit message handlers
LRESULT  CBacnetUserlogin::RedrawLoginWindow(WPARAM wParam, LPARAM lParam)
{
	::GetWindowRect(BacNet_hwd,&userlogin_rect);	//获取 view的窗体大小;
	MoveWindow(userlogin_rect.left,userlogin_rect.top,userlogin_rect.Width(),userlogin_rect.Height(),1);
	Invalidate();
	GetDlgItem(IDC_EDIT_LOGIN_USERNAME)->SetFocus();

	return 0;
}

BOOL CBacnetUserlogin::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN)
		{
			if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_LOGIN_USERNAME)
			{
				GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
				return TRUE;
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_PASSWORD)
			{
				::PostMessage(GetDlgItem(IDC_BUTTON_LOGIN)->m_hWnd,WM_LBUTTONDOWN,NULL,NULL);
				::PostMessage(GetDlgItem(IDC_BUTTON_LOGIN)->m_hWnd,WM_LBUTTONUP,NULL,NULL);
				return TRUE;
			}
			else
				return TRUE;

		}
		else if(pMsg->message==VK_ESCAPE) 
		{		
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




void CBacnetUserlogin::OnBnClickedButtonLogin()
{
	
	bool any_user_valid = false;
	//检测是否里面存在有效的账号密码;
	for (int i=0;i<(int)m_user_login_data.size();i++)
	{
		CString temp_user_name;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(i).name, (int)strlen((char *)m_user_login_data.at(i).name)+1, 
			temp_user_name.GetBuffer(MAX_PATH), MAX_PATH );
		temp_user_name.ReleaseBuffer();

		CString temp_password;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(i).password, (int)strlen((char *)m_user_login_data.at(i).password)+1, 
			temp_password.GetBuffer(MAX_PATH), MAX_PATH );
		temp_password.ReleaseBuffer();
		if(!temp_user_name.IsEmpty())
		{
			if(!temp_password.IsEmpty())
			{
				any_user_valid = true;	//User表里面 存在可用的 账号密码;
				break;
			}
		}		
	}

	if(any_user_valid == false)
	{
		this->ShowWindow(false);
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,PASSWORD_OK_INITIAL_UI,0);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No user name and password in user config list!")); 
		return ;
	}

	int nret = CheckLoginData();
	GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW(_T(""));
	if(nret == LOGIN_SUCCESS_FULL_ACCESS)	//Check password success!
	{
		this->ShowWindow(SW_HIDE);
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,PASSWORD_OK_INITIAL_UI,LOGIN_SUCCESS_FULL_ACCESS);
	}
	else if(nret == LOGIN_SUCCESS_READ_ONLY)
	{
		this->ShowWindow(SW_HIDE);
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,PASSWORD_OK_INITIAL_UI,0);
	}
	else if(nret == LOGIN_SUCCESS_GRAPHIC_MODE)
	{
		this->ShowWindow(SW_HIDE);
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,PASSWORD_OK_INITIAL_UI,LOGIN_SUCCESS_GRAPHIC_MODE);
	}
	else if(nret == LOGIN_SUCCESS_ROUTINE_MODE)
	{
		this->ShowWindow(SW_HIDE);
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,PASSWORD_OK_INITIAL_UI,LOGIN_SUCCESS_ROUTINE_MODE);
	}
	else if(nret == LOGIN_USER_ERROR)
	{
		MessageBox(_T("User Name not exsit!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if(nret == LOGIN_PASSWORD_ERROR)
	{
		MessageBox(_T("User name or password error!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		return;
	}
}

int CBacnetUserlogin::CheckLoginData()
{
	bool user_name_found = false;
	GetDlgItemTextW(IDC_EDIT_LOGIN_USERNAME,m_user_name);
	GetDlgItemTextW(IDC_EDIT_PASSWORD,m_user_password);
	
	if(m_user_name.IsEmpty())
	{
		return LOGIN_USER_ERROR;
	}

	if(m_user_password.IsEmpty())
	{
		return LOGIN_PASSWORD_ERROR;
	}

	
	for (int index=0;index<m_user_login_data.size();index++)
	{
		CString temp_user_name;
		CString temp_password;

		MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(index).name, (int)strlen((char *)m_user_login_data.at(index).name)+1, 
			temp_user_name.GetBuffer(MAX_PATH), MAX_PATH );
		temp_user_name.ReleaseBuffer();


		MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(index).password, (int)strlen((char *)m_user_login_data.at(index).password)+1, 
			temp_password.GetBuffer(MAX_PATH), MAX_PATH );
		temp_password.ReleaseBuffer();

		if(temp_user_name.CompareNoCase(m_user_name) == 0)
		{
			user_name_found = true;
			if(temp_password.CompareNoCase(m_user_password) == 0)
			{
				if(m_user_login_data.at(index).access_level == LOGIN_SUCCESS_READ_ONLY)
					return LOGIN_SUCCESS_READ_ONLY;
				else if(m_user_login_data.at(index).access_level == LOGIN_SUCCESS_FULL_ACCESS)
					return LOGIN_SUCCESS_FULL_ACCESS;
				else if(m_user_login_data.at(index).access_level == LOGIN_SUCCESS_GRAPHIC_MODE)
					return LOGIN_SUCCESS_GRAPHIC_MODE;
				else if(m_user_login_data.at(index).access_level == LOGIN_SUCCESS_ROUTINE_MODE)
					return LOGIN_SUCCESS_ROUTINE_MODE;
				else
					return LOGIN_SUCCESS_FULL_ACCESS;

			}
			else
				return LOGIN_PASSWORD_ERROR;
		}
	}

	if(!user_name_found)
		return LOGIN_USER_ERROR;
	
	return 0;
}


void CBacnetUserlogin::OnCancel()
{

	
	//CDialogEx::OnCancel();
}


void CBacnetUserlogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages

	CRect test_rect;
	::GetWindowRect(BacNet_hwd,&test_rect);	//获取 view的窗体大小;


	
	// Do not call CDialogEx::OnPaint() for painting messages

	CMemDC memDC(dc,this);
	CRect rcClient;
	GetClientRect(&rcClient);

		memDC.GetDC().FillSolidRect(&rcClient,RGB(202,208,216));
		Graphics graphics(memDC.GetDC());


		Bitmap bitmap(hBitmap_login,NULL);
		graphics.DrawImage(&bitmap,0 ,0,test_rect.Width(),test_rect.Height());

}
