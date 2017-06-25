// BacnetSettingHealth.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingHealth.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetSettingHealth dialog

IMPLEMENT_DYNAMIC(CBacnetSettingHealth, CDialogEx)

CBacnetSettingHealth::CBacnetSettingHealth(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetSettingHealth::IDD, pParent)
{

}

CBacnetSettingHealth::~CBacnetSettingHealth()
{
}

void CBacnetSettingHealth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingHealth, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_HEALTH_CLEAR, &CBacnetSettingHealth::OnBnClickedButtonHealthClear)
END_MESSAGE_MAP()


// CBacnetSettingHealth message handlers


BOOL CBacnetSettingHealth::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Refresh_Health_Data();
	SetTimer(1,4000,NULL);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetSettingHealth::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetSettingHealth::OnTimer(UINT_PTR nIDEvent)
{
	 
	//Post_Refresh_Message(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC),1);
	GetPrivateData(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC));

	Refresh_Health_Data();
	CDialogEx::OnTimer(nIDEvent);
}

void CBacnetSettingHealth::Refresh_Health_Data()
{
	CString CS_COM_RX_1;	CString CS_COM_RX_2;	CString CS_COM_RX_3;
	CString CS_COM_TX_1;	CString CS_COM_TX_2;	CString CS_COM_TX_3;
	CString CS_COM_COLLISION_1;	CString CS_COM_COLLISION_2;	CString CS_COM_COLLISION_3;
	CString CS_PACHET_ERROR_1;	CString CS_PACHET_ERROR_2;	CString CS_PACHET_ERROR_3;
	CString CS_TIMEOUT_1;	CString CS_TIMEOUT_2;	CString CS_TIMEOUT_3;
	CS_COM_RX_1.Format(_T("%u"),Device_Misc_Data.reg.com_rx[0]);
	CS_COM_RX_2.Format(_T("%u"),Device_Misc_Data.reg.com_rx[1]);
	CS_COM_RX_3.Format(_T("%u"),Device_Misc_Data.reg.com_rx[2]);

	CS_COM_TX_1.Format(_T("%u"),Device_Misc_Data.reg.com_tx[0]);
	CS_COM_TX_2.Format(_T("%u"),Device_Misc_Data.reg.com_tx[1]);
	CS_COM_TX_3.Format(_T("%u"),Device_Misc_Data.reg.com_tx[2]);

	CS_COM_COLLISION_1.Format(_T("%u"),Device_Misc_Data.reg.collision[0]);
	CS_COM_COLLISION_2.Format(_T("%u"),Device_Misc_Data.reg.collision[1]);
	CS_COM_COLLISION_3.Format(_T("%u"),Device_Misc_Data.reg.collision[2]);

	CS_PACHET_ERROR_1.Format(_T("%u"),Device_Misc_Data.reg.packet_error[0]);
	CS_PACHET_ERROR_2.Format(_T("%u"),Device_Misc_Data.reg.packet_error[1]);
	CS_PACHET_ERROR_3.Format(_T("%u"),Device_Misc_Data.reg.packet_error[2]);

	CS_TIMEOUT_1.Format(_T("%u"),Device_Misc_Data.reg.timeout[0]);
	CS_TIMEOUT_2.Format(_T("%u"),Device_Misc_Data.reg.timeout[1]);
	CS_TIMEOUT_3.Format(_T("%u"),Device_Misc_Data.reg.timeout[2]);

	GetDlgItem(IDC_EDIT_RX_1)->SetWindowTextW(CS_COM_RX_1);
	GetDlgItem(IDC_EDIT_RX_2)->SetWindowTextW(CS_COM_RX_2);
	GetDlgItem(IDC_EDIT_RX_3)->SetWindowTextW(CS_COM_RX_3);

	GetDlgItem(IDC_EDIT_TX_1)->SetWindowTextW(CS_COM_TX_1);
	GetDlgItem(IDC_EDIT_TX_2)->SetWindowTextW(CS_COM_TX_2);
	GetDlgItem(IDC_EDIT_TX_3)->SetWindowTextW(CS_COM_TX_3);

	GetDlgItem(IDC_EDIT_ID_COLLISION_1)->SetWindowTextW(CS_COM_COLLISION_1);
	GetDlgItem(IDC_EDIT_ID_COLLISION_2)->SetWindowTextW(CS_COM_COLLISION_2);
	GetDlgItem(IDC_EDIT_ID_COLLISION_3)->SetWindowTextW(CS_COM_COLLISION_3);

	GetDlgItem(IDC_EDIT_PACKET_ERROR_1)->SetWindowTextW(CS_PACHET_ERROR_1);
	GetDlgItem(IDC_EDIT_PACKET_ERROR_2)->SetWindowTextW(CS_PACHET_ERROR_2);
	GetDlgItem(IDC_EDIT_PACKET_ERROR_3)->SetWindowTextW(CS_PACHET_ERROR_3);

	GetDlgItem(IDC_EDIT_TIMEOUT_1)->SetWindowTextW(CS_TIMEOUT_1);
	GetDlgItem(IDC_EDIT_TIMEOUT_2)->SetWindowTextW(CS_TIMEOUT_2);
	GetDlgItem(IDC_EDIT_TIMEOUT_3)->SetWindowTextW(CS_TIMEOUT_3);

}


void CBacnetSettingHealth::OnBnClickedButtonHealthClear()
{
	
	memset(&Device_Special_Data,0,sizeof(Str_Special));
	Device_Special_Data.reg.clear_health_rx_tx = 0x11;

	if(Write_Private_Data_Blocking(WRITE_SPECIAL_COMMAND,0,0) > 0 )
	{
		PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		MessageBox(_T("Clear Count : OK !"));
	}
	else
	{
		MessageBox(_T("Clear Count : Timeout !"));
	}
}
