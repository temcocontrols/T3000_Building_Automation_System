// BacnetAddLabel.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAddLabel.h"
#include "afxdialogex.h"

POINT Insert_Point;
// CBacnetAddLabel dialog

IMPLEMENT_DYNAMIC(CBacnetAddLabel, CDialogEx)

CBacnetAddLabel::CBacnetAddLabel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAddLabel::IDD, pParent)
{

}

CBacnetAddLabel::~CBacnetAddLabel()
{
}

void CBacnetAddLabel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ADD_LABEL_POINT, m_add_label_point);
	DDX_Control(pDX, IDC_EDIT_ADD_LABEL_POINT, m_add_label_edit);
}


BEGIN_MESSAGE_MAP(CBacnetAddLabel, CDialogEx)
END_MESSAGE_MAP()


// CBacnetAddLabel message handlers


BOOL CBacnetAddLabel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_add_label = this->m_hWnd;
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	//::GetWindowRect(BacNet_hwd,&userlogin_rect);	//获取 view的窗体大小;
	CRect temprect;
	::GetWindowRect(m_screenedit_dlg_hwnd,&temprect);	//获取 view的窗体大小;
	if(lpPoint.x < temprect.left)
	{	
		PostMessage(WM_CLOSE,NULL,NULL);
		//this->ShowWindow(SW_HIDE);
		return 1;
	}
	CRect add_temprect;
	::GetWindowRect(m_add_label,&add_temprect);	
	MoveWindow(lpPoint.x,lpPoint.y,add_temprect.Width(),add_temprect.Height(),1);

	m_add_label_point.SetWindowTextW(_T("Point :"));
	m_add_label_point.textColor(RGB(0,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_add_label_point.setFont(30,15,NULL,_T("Arial"));

	m_add_label_edit.SetWindowTextW(_T(""));
	m_add_label_edit.textColor(RGB(250,120,120));
	//m_static.bkColor(RGB(0,255,255));
	m_add_label_edit.setFont(35,15,NULL,_T("Arial"));



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetAddLabel::PreTranslateMessage(MSG* pMsg)
{
	

	if(pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			m_add_label_edit.SetWindowTextW(_T(""));
			this->ShowWindow(SW_HIDE);
		}
		else if(pMsg->wParam == VK_RETURN)
		{
			if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_ADD_LABEL_POINT)
			{
				//发送消息给父窗体 现在输入的是什么;
				CString temp_point;
				char * temp_char = new char[200];
				m_add_label_edit.GetWindowTextW(temp_point);
				if (temp_point.IsEmpty() || temp_point.GetLength() >= 40)
				{
					return CDialogEx::PreTranslateMessage(pMsg);
				}
				temp_point.MakeUpper();// label only support upper case;
				WideCharToMultiByte(CP_ACP,NULL,temp_point.GetBuffer(),-1,temp_char,200,NULL,NULL);

				::PostMessage(m_screenedit_dlg_hwnd,ADD_LABEL_MESSAGE,(WPARAM)temp_char,(LPARAM)(&Insert_Point));
				m_add_label_edit.SetWindowTextW(_T(""));
			}
			else
				return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBacnetAddLabel::FreshWindow()
{
	this->ShowWindow(SW_SHOW);
	//m_add_label_edit.SetWindowTextW(_T(""));


	POINT lpPoint;
	GetCursorPos(&lpPoint);
	//::GetWindowRect(BacNet_hwd,&userlogin_rect);	//获取 view的窗体大小;
	CRect temprect;
	::GetWindowRect(m_screenedit_dlg_hwnd,&temprect);	//获取 view的窗体大小;
	if((lpPoint.x < temprect.left) || (lpPoint.y < temprect.top))
	{	
		PostMessage(WM_CLOSE,NULL,NULL);
		return ;
	}
	CRect add_temprect;
	::GetWindowRect(m_add_label,&add_temprect);	

	MoveWindow(lpPoint.x,lpPoint.y,add_temprect.Width(),add_temprect.Height(),1);
	GetDlgItem(IDC_EDIT_ADD_LABEL_POINT)->SetFocus();
	Insert_Point.x = lpPoint.x - temprect.left;
	Insert_Point.y = lpPoint.y - temprect.top;
}
