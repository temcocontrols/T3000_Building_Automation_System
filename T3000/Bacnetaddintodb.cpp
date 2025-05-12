// Bacnetaddintodb.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Bacnetaddintodb.h"
#include "afxdialogex.h"


bool cancle_send = false;
// CBacnetaddintodb dialog

IMPLEMENT_DYNAMIC(CBacnetaddintodb, CDialogEx)

CBacnetaddintodb::CBacnetaddintodb(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetaddintodb::IDD, pParent)
{

}

CBacnetaddintodb::~CBacnetaddintodb()
{
}

void CBacnetaddintodb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetaddintodb, CDialogEx)
	ON_BN_CLICKED(IDC_BACNET_SETTING_ADD_BTN, &CBacnetaddintodb::OnBnClickedBacnetSettingAddBtn)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADD_OK, &CBacnetaddintodb::OnBnClickedButtonSettingAddOk)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADD_CANCEL, &CBacnetaddintodb::OnBnClickedButtonSettingAddCancel)
	ON_BN_CLICKED(IDC_BACNET_SETTING_CLEAR_BTN, &CBacnetaddintodb::OnBnClickedBacnetSettingClearBtn)
END_MESSAGE_MAP()


// CBacnetaddintodb message handlers


void CBacnetaddintodb::OnBnClickedBacnetSettingAddBtn()
{
	
	CString temp_add_id;
	temp_add_id.Empty();
	GetDlgItem(IDC_BACNET_ADDED_EDIT)->GetWindowTextW(temp_add_id);
	if(temp_add_id.IsEmpty())
	{
		MessageBox(_T("Please input an ID.Click 'add' button and 'OK' button ,it will add this id into device database"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_BACNET_ADDED_EDIT)->SetWindowTextW(_T(""));
		return;
	}

	long nidvalue = _wtoi64(temp_add_id);
	if((nidvalue <= 0) || (nidvalue >= 255))
	{
		MessageBox(_T("Please input an effective ID."),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_BACNET_ADDED_EDIT)->SetWindowTextW(_T(""));
		return;
	}

	for (int x=0;x<254;x++)
	{
		if(bacnet_add_id[x] == 0)	//为0 就说明后面没有了;
		{
			bacnet_add_id[x] = nidvalue;
			CString showvalue;
			CString temp_id;
			temp_id.Format(_T("%02u"),nidvalue);
			GetDlgItem(IDC_STATIC_ADDED_SHOW)->GetWindowTextW(showvalue);
			showvalue = showvalue + _T("  ") + temp_id;
			GetDlgItem(IDC_STATIC_ADDED_SHOW)->SetWindowTextW(showvalue);
			GetDlgItem(IDC_BACNET_ADDED_EDIT)->SetWindowTextW(_T(""));
			return;
		}
		if(nidvalue == bacnet_add_id[x])
		{
			MessageBox(_T("This ID already exsit in the below window."),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			GetDlgItem(IDC_BACNET_ADDED_EDIT)->SetWindowTextW(_T(""));
			return;
		}
	}
}


BOOL CBacnetaddintodb::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	memset(bacnet_add_id,0,254);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetaddintodb::PreTranslateMessage(MSG* pMsg)
{
	
	if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN))
	{
		if(bacnet_add_id[0] == 0)
			cancle_send = true;
		else
			cancle_send = false;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetaddintodb::OnCancel()
{
	

	CDialogEx::OnCancel();
}


void CBacnetaddintodb::OnBnClickedButtonSettingAddOk()
{
	
	if(bacnet_add_id[0] == 0)
		cancle_send = true;
	else
		cancle_send = false;
	PostMessage(WM_CLOSE,0,0);
}


void CBacnetaddintodb::OnBnClickedButtonSettingAddCancel()
{
	
	cancle_send = false;
	PostMessage(WM_CLOSE,0,0);
}


void CBacnetaddintodb::OnBnClickedBacnetSettingClearBtn()
{
	
	GetDlgItem(IDC_STATIC_ADDED_SHOW)->SetWindowTextW(_T(""));
	memset(bacnet_add_id,0,254);
}
