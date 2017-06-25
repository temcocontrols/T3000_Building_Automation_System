// BacnetATCommand.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetATCommand.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetATCommand dialog

IMPLEMENT_DYNAMIC(CBacnetATCommand, CDialogEx)

CBacnetATCommand::CBacnetATCommand(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetATCommand::IDD, pParent)
{

}

CBacnetATCommand::~CBacnetATCommand()
{
}

void CBacnetATCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEND, m_at_command_send);
}


BEGIN_MESSAGE_MAP(CBacnetATCommand, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AT_SEND, &CBacnetATCommand::OnBnClickedButtonAtSend)
	ON_MESSAGE(WM_REFRESH_BAC_AT_COMMAND,Fresh_RX_AT_Command)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CBacnetATCommand message handlers


BOOL CBacnetATCommand::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_at_command_hwnd = this->m_hWnd;
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask|=CFM_BOLD;

	cf.dwEffects&=~CFE_BOLD;
	//cf.dwEffects|=~CFE_BOLD; //粗体，取消用cf.dwEffects&=~CFE_BOLD;
	cf.dwMask|=CFM_ITALIC;
	cf.dwEffects&=~CFE_ITALIC;
	//cf.dwEffects|=~CFE_ITALIC; //斜体，取消用cf.dwEffects&=~CFE_ITALIC;
	cf.dwMask|=CFM_UNDERLINE;
	cf.dwEffects&=~CFE_UNDERLINE;
	//cf.dwEffects|=~CFE_UNDERLINE; //斜体，取消用cf.dwEffects&=~CFE_UNDERLINE;
	cf.dwMask|=CFM_COLOR;
	cf.crTextColor = RGB(0,0,255); //设置颜色;
	cf.dwMask|=CFM_SIZE;
	cf.yHeight =200; //设置高度;
	cf.dwMask|=CFM_FACE;
	_tcscpy(cf.szFaceName ,_T("Arial"));

	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->SetSelectionCharFormat(cf);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->SetDefaultCharFormat(cf); 
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	m_at_command_send.SetWindowTextW(_T(""));
	m_at_command_send.textColor(RGB(255,0,0));
	m_at_command_send.bkColor(RGB(255,255,255));
	m_at_command_send.setFont(26,20,NULL,_T("Arial"));


	memset(m_at_read_buf,0,450);
	memset(m_at_write_buf,0,100);

	m_at_write_buf[0] = AT_OPEN;
	Write_Private_Data_Blocking(WRITE_AT_COMMAND,0,0);

	SetTimer(1,4000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CBacnetATCommand::Fresh_RX_AT_Command(WPARAM wParam,LPARAM lParam)
{
	CString temp_recv;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_at_read_buf, 
		(int)strlen((char *)m_at_read_buf)+1, 
		temp_recv.GetBuffer(MAX_PATH), MAX_PATH );
	temp_recv.ReleaseBuffer();	
	if(temp_recv.IsEmpty())
	{
		DFTrace(_T("Receive AT 90 Command , it's empty!"));
		return 0;
	}

	temp_recv = _T("\r\n") + temp_recv;
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->SetSel(-1, -1);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->ReplaceSel( (LPCTSTR)temp_recv.GetBuffer() );
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	memset(m_at_write_buf,0,100);
	m_at_write_buf[0] = AT_CLEAR;
	if(Write_Private_Data_Blocking(WRITE_AT_COMMAND,0,0) > 0)
	{
		DFTrace(_T("AT_CLEAR"));
	}
	return 0;
}

BOOL CBacnetATCommand::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetATCommand::OnBnClickedButtonAtSend()
{
	
	CString cs_send_tx;
	GetDlgItemTextW(IDC_EDIT_SEND,cs_send_tx);
	if(cs_send_tx.GetLength()>=98)
	{
		MessageBox(_T("Command too long."));
		return;
	}
	else if(cs_send_tx.IsEmpty())
	{
		return;
	}
	memset(m_at_write_buf,0,100);
	m_at_write_buf[0] = AT_SEND;
	int send_length = cs_send_tx.GetLength();

	WideCharToMultiByte( CP_ACP, 0, cs_send_tx.GetBuffer(), -1, m_at_write_buf + 1, 255, NULL, NULL );
	m_at_write_buf[send_length + 1] = 0x0d;
	m_at_write_buf[send_length + 2] = 0x0a;

	if(Write_Private_Data_Blocking(WRITE_AT_COMMAND,0,0) > 0)
	{
		//GetDlgItem(IDC_EDIT_SEND)->SetWindowTextW(_T(""));
		//CString temp2;
		//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->GetWindowTextW(temp2);

		//CString temp1;
		//temp1 = temp2 + _T("\r\n") + cs_send_tx;
		//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->SetWindowTextW(temp1);
		cs_send_tx = _T("\r\n") + cs_send_tx;
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->SetSel(-1, -1);
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->ReplaceSel( (LPCTSTR)cs_send_tx.GetBuffer() );
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT21_AT_COMMAND_RECEIVE))->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
		//GetDlgItem(IDC_EDIT_SEND)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_SEND)->SetFocus();

	}
}


void CBacnetATCommand::OnTimer(UINT_PTR nIDEvent)
{
	 
	int temp_invoke_id = -1;
	int send_status = true;
	int	resend_count = 0;
	for (int z=0;z<3;z++)
	{
		do 
		{
			resend_count ++;
			if(resend_count>5)
			{
				send_status = false;
				break;
			}
			temp_invoke_id =  GetPrivateData(
				g_bac_instance,
				READ_AT_COMMAND,
				0,
				0,
				450);		

			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (temp_invoke_id<0);
		if(send_status)
		{
			break;
		}
	}
	//GetPrivateData_Blocking(g_bac_instance,READ_AT_COMMAND,0,0,450);
	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetATCommand::OnClose()
{
	 
	memset(m_at_write_buf,0,100);

	m_at_write_buf[0] = AT_CLOSE;
	int send_ret = -1;
	for (int i=0;i<3;i++)
	{
		send_ret = Write_Private_Data_Blocking(WRITE_AT_COMMAND,0,0);
		if(send_ret>0)
			break;
	}
	if(send_ret <0)
	{
		if(IDYES == MessageBox(_T("Send Command Timeout!Do you want to exit!"),_T("Warning"),MB_YESNO))
		{
			CDialogEx::OnClose();
		}
		else
			return;
	}

	CDialogEx::OnClose();
}
