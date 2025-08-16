// BacnetSettingAdvParameter.cpp: Implementation file
// BacnetSettingAdvParameter.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "afxdialogex.h"
#include "BacnetSettingAdvParameter.h"
#include "global_function.h"

// BacnetSettingAdvParameter Dialog
// BacnetSettingAdvParameter 对话框

IMPLEMENT_DYNAMIC(BacnetSettingAdvParameter, CDialogEx)

BacnetSettingAdvParameter::BacnetSettingAdvParameter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADVANCED_SETTINGS, pParent)
{

}

BacnetSettingAdvParameter::~BacnetSettingAdvParameter()
{
}

void BacnetSettingAdvParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BacnetSettingAdvParameter, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_APPLY, &BacnetSettingAdvParameter::OnBnClickedButtonAdvancedApply)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_OK, &BacnetSettingAdvParameter::OnBnClickedButtonAdvancedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_Q1, &BacnetSettingAdvParameter::OnBnClickedButtonAdvancedQ1)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_Q2, &BacnetSettingAdvParameter::OnBnClickedButtonAdvancedQ2)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_CANCEL, &BacnetSettingAdvParameter::OnBnClickedButtonAdvancedCancel)
END_MESSAGE_MAP()


// BacnetSettingAdvParameter Message handlers
// BacnetSettingAdvParameter 消息处理程序


BOOL BacnetSettingAdvParameter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// TODO:  在此添加额外的初始化
	if(Device_Basic_Setting.reg.panel_type == PM_ESP32_T3_SERIES)
	{
		if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 655)//655
		{
			((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ADVANCED_TIMESAVE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q2)->EnableWindow(FALSE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->EnableWindow(true);
			GetDlgItem(IDC_EDIT_ADVANCED_TIMESAVE)->EnableWindow(1);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q1)->EnableWindow(1);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q2)->EnableWindow(1);
			CString temp;
			temp.Format(_T("%u"), Device_Basic_Setting.reg.write_flash);
			GetDlgItem(IDC_EDIT_ADVANCED_TIMESAVE)->SetWindowTextW(temp);
			if(Device_Basic_Setting.reg.fix_com_config == 0)
				((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->SetCheck(0);
			else
				((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->SetCheck(1);
		}
	}
	else if (Device_Basic_Setting.reg.panel_type == PM_MINIPANEL_ARM)
	{
		if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 670)//670
		{
			((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ADVANCED_TIMESAVE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q2)->EnableWindow(FALSE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->EnableWindow(true);
			GetDlgItem(IDC_EDIT_ADVANCED_TIMESAVE)->EnableWindow(1);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q1)->EnableWindow(1);
			GetDlgItem(IDC_BUTTON_ADVANCED_Q2)->EnableWindow(1);

			CString temp;
			temp.Format(_T("%u"), Device_Basic_Setting.reg.write_flash);
			GetDlgItem(IDC_EDIT_ADVANCED_TIMESAVE)->SetWindowTextW(temp);
			if (Device_Basic_Setting.reg.fix_com_config == 0)
				((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->SetCheck(0);
			else
				((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->SetCheck(1);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// Exception: OCX property pages should return FALSE
	// 异常: OCX 属性页应返回 FALSE
}


void BacnetSettingAdvParameter::OnBnClickedButtonAdvancedApply()
{
	// TODO: Add control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	int temp_flag = 0;
	if (Device_Basic_Setting.reg.panel_type == PM_ESP32_T3_SERIES)
	{
		if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 655)//655
		{
			CString temp_cstring;
			GetDlgItemTextW(IDC_EDIT_ADVANCED_TIMESAVE, temp_cstring);
			unsigned char temp_int = unsigned char(_wtoi(temp_cstring));
			if((temp_int != 0) && (temp_int <5))
			{
				MessageBox(_T("The value of save parameter must be greater than 5!"), _T("Advanced Settings"), MB_OK | MB_ICONINFORMATION);
				return;
			}
			Device_Basic_Setting.reg.write_flash = temp_int;

			// IDC_CHECK_FIX_RS485
			if(((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->GetCheck() == 1)
			{
				Device_Basic_Setting.reg.fix_com_config = 1;
			}
			else
			{
				Device_Basic_Setting.reg.fix_com_config = 0;
			}
			temp_flag = 1;
		}
	}
	else if (Device_Basic_Setting.reg.panel_type == PM_MINIPANEL_ARM)
	{
		if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 670)//670
		{
			CString temp_cstring;
			GetDlgItemTextW(IDC_EDIT_ADVANCED_TIMESAVE, temp_cstring);
			unsigned char temp_int = unsigned char(_wtoi(temp_cstring));
			if ((temp_int != 0) && (temp_int < 5))
			{
				MessageBox(_T("The value of save parameter must be greater than 5!"), _T("Advanced Settings"), MB_OK | MB_ICONINFORMATION);
				return;
			}
			Device_Basic_Setting.reg.write_flash = temp_int;

			// IDC_CHECK_FIX_RS485
			if (((CButton*)GetDlgItem(IDC_CHECK_FIX_RS485))->GetCheck() == 1)
			{
				Device_Basic_Setting.reg.fix_com_config = 1;
			}
			else
			{
				Device_Basic_Setting.reg.fix_com_config = 0;
			}
			temp_flag = 1;
		}
	}


	if (temp_flag == 1)
	{
		if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Update advanced setting timeout!"));
		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Update advanced setting success!"));
		}
	}




}


void BacnetSettingAdvParameter::OnBnClickedButtonAdvancedOk()
{
	// TODO: Add control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonAdvancedApply();
	PostMessage(WM_CLOSE, 0, 0); // Close the dialog
}


void BacnetSettingAdvParameter::OnBnClickedButtonAdvancedQ1()
{
	// TODO: Add control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("After enabling this item, the Setting-Communication-Device Serial Port Config will be fixed."),_T("Advanced Settings"), MB_OK | MB_ICONINFORMATION);
}


void BacnetSettingAdvParameter::OnBnClickedButtonAdvancedQ2()
{
	// TODO: Add control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("All parameters, input, output, variable, and the values run in the program are saved at regular intervals within the set time. Prevent data loss after an unexpected power outage.You can also modify this value through register 92 of modbus. When this value is set to 0, it means that this function is disabled."), _T("Advanced Settings"), MB_OK | MB_ICONINFORMATION);
}


void BacnetSettingAdvParameter::OnBnClickedButtonAdvancedCancel()
{
	// TODO: Add control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_CLOSE, 0, 0); // Close the dialog
}
