// BacnetWait.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWait.h"
#include "afxdialogex.h"
#include "gloab_define.h"
#include "globle_function.h"
extern _Refresh_Info Bacnet_Refresh_Info;
// BacnetWait dialog
extern HWND BacNet_hwd;//Used for send a message to father delete the dlg;
IMPLEMENT_DYNAMIC(BacnetWait, CDialogEx)

BacnetWait::BacnetWait(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetWait::IDD, pParent)
{

}

BacnetWait::~BacnetWait()
{
}

void BacnetWait::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WAIT_DETAIL, m_wait_detail);
	DDX_Control(pDX, IDC_PROGRESS1, m_wait_progress);
}


BEGIN_MESSAGE_MAP(BacnetWait, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// BacnetWait message handlers


BOOL BacnetWait::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wait_detail.SetWindowTextW(_T("Send command to device..."));
	m_wait_detail.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_wait_detail.setFont(25,18,NULL,_T("Arial"));
	

	// TODO:  Add extra initialization here
	SetTimer(1,500,NULL);
	m_wait_progress.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void BacnetWait::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void BacnetWait::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialogEx::OnCancel();
}


BOOL BacnetWait::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void BacnetWait::OnTimer(UINT_PTR nIDEvent)
{
	static int static_count=0;
	static_count = (++static_count)%5;
	CString tempcs,tempcs2;
	tempcs.Format(_T("Reading descriptors "));
	for (int i=0;i<=static_count;i++)
	{
		tempcs2 = tempcs2 + _T(".");
	}
	tempcs = tempcs + tempcs2;
	int success_count =0;
	int pos=0;
	if(Bacnet_Refresh_Info.Output_result == BAC_RESULTS_OK)
	{
		success_count ++;
		pos = pos +25;
	}
	if(Bacnet_Refresh_Info.Input_result == BAC_RESULTS_OK)
	{
		success_count ++;
		pos = pos +25;
	}
	if(Bacnet_Refresh_Info.Variable_result == BAC_RESULTS_OK)
	{
		success_count ++;
		pos = pos +25;
	}
	if(Bacnet_Refresh_Info.Program_result == BAC_RESULTS_OK)
	{
		success_count ++;
		pos = pos +25;
	}
	m_wait_progress.SetPos(pos);

	switch(nIDEvent)
	{
	case 1:
		if((Bacnet_Refresh_Info.Input_result == BAC_RESULTS_UNKONW) ||
			(Bacnet_Refresh_Info.Output_result == BAC_RESULTS_UNKONW) ||
			(Bacnet_Refresh_Info.Program_result == BAC_RESULTS_UNKONW) ||
			(Bacnet_Refresh_Info.Variable_result == BAC_RESULTS_UNKONW) )
		{
			m_wait_detail.SetWindowTextW(tempcs);
		}
		else if(Bacnet_Refresh_Info.Input_result == BAC_RESULTS_FAIL)
		{
			m_wait_detail.SetWindowTextW(_T("Read Input Table Time Out!"));
			KillTimer(1);
			SetTimer(2,2000,NULL);
		}
		else if(Bacnet_Refresh_Info.Output_result == BAC_RESULTS_FAIL)
		{
			m_wait_detail.SetWindowTextW(_T("Read Output Table Time Out!"));
			KillTimer(1);
			SetTimer(2,2000,NULL);
		}
		else if(Bacnet_Refresh_Info.Program_result == BAC_RESULTS_FAIL)
		{
			m_wait_detail.SetWindowTextW(_T("Read Program Table Time Out!"));
			KillTimer(1);
			SetTimer(2,2000,NULL);
		}
		else if(Bacnet_Refresh_Info.Variable_result == BAC_RESULTS_FAIL)
		{
			m_wait_detail.SetWindowTextW(_T("Read Variable Table Time Out!"));
			KillTimer(1);
			SetTimer(2,2000,NULL);
		}
		else
		{
			m_wait_detail.SetWindowTextW(_T("Reading descriptors success!"));
			KillTimer(1);
			SetTimer(2,2000,NULL);
		}
		break;
	case 2:
		{
			KillTimer(2);
			SetTimer(3,500,NULL);
			ShowWindow(SW_HIDE);
		}
		break;
	case 3:
		{
			KillTimer(3);
			::PostMessage(BacNet_hwd,WM_DELETE_WAIT_DLG,0,0);
			PostMessage(WM_CLOSE,NULL,NULL);
			//OnCancel();
		}
	}
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}


