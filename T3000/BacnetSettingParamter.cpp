// BacnetSettingParamter.cpp : Implementation file
// BacnetSettingParamter.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSettingParamter.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetSettingParamter dialog box
// CBacnetSettingParamter 对话框

IMPLEMENT_DYNAMIC(CBacnetSettingParamter, CDialogEx)

CBacnetSettingParamter::CBacnetSettingParamter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_SETTING_LCD_PARAMETER, pParent)
{

}

CBacnetSettingParamter::~CBacnetSettingParamter()
{
}

void CBacnetSettingParamter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetSettingParamter, CDialogEx)
    //ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CBacnetSettingParamter::OnCbnSelchangeComboType)
    //ON_CBN_SELCHANGE(IDC_COMBO_NUMBER, &CBacnetSettingParamter::OnCbnSelchangeComboNumber)
    ON_BN_CLICKED(IDC_BUTTON_LCD_OK, &CBacnetSettingParamter::OnBnClickedButtonLcdOk)
END_MESSAGE_MAP()


// CBacnetSettingParamter message handlers
// CBacnetSettingParamter 消息处理程序


BOOL CBacnetSettingParamter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: Add additional initialization here
    // TODO:  在此添加额外的初始化
    InitialUI();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // Exception: OCX property pages should return FALSE
                  // 异常: OCX 属性页应返回 FALSE
}

void CBacnetSettingParamter::InitialUI()
{
    ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->EnableWindow(1);
    ((CComboBox *)GetDlgItem(IDC_COMBO_NUMBER))->EnableWindow(1);
    ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->ResetContent();
    ((CComboBox *)GetDlgItem(IDC_COMBO_NUMBER))->ResetContent();
    for (int i = 0; i < sizeof(DisplayType) / sizeof(DisplayType[0]); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->AddString(DisplayType[i]);
    }

    for (int i = 0; i < 128; i++)
    {
        CString temp_cs;
        temp_cs.Format(_T("%u"), i + 1);
        ((CComboBox *)GetDlgItem(IDC_COMBO_NUMBER))->AddString(temp_cs);
    }

        int dis_type = 0;
        dis_type = Device_Basic_Setting.reg.display_lcd.lcd_mod_reg.npoint.point_type;
        if (dis_type == BAC_OUT)
            ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->SetWindowTextW(DisplayType[BAC_OUT]);
        else if (dis_type == BAC_IN)
            ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->SetWindowTextW(DisplayType[BAC_IN]);
        else if (dis_type == BAC_VAR)
            ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->SetWindowTextW(DisplayType[BAC_VAR]);

        int temp_number = 0;
        temp_number = Device_Basic_Setting.reg.display_lcd.lcd_mod_reg.npoint.number;
        CString temp_cs;
        temp_cs.Format(_T("%u"), temp_number);
        ((CComboBox *)GetDlgItem(IDC_COMBO_NUMBER))->SetWindowTextW(temp_cs);

  

        if ((Device_Basic_Setting.reg.mini_type == T3_TSTAT10) ||
            (Device_Basic_Setting.reg.mini_type == T3_OEM) ||
            (Device_Basic_Setting.reg.mini_type == T3_OEM_12I))
        {
            if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 539)
            {
                ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->EnableWindow(1);
                ((CComboBox*)GetDlgItem(IDC_COMBO_NUMBER))->EnableWindow(1);
            }
            else
            {
                ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->EnableWindow(0);
                ((CComboBox*)GetDlgItem(IDC_COMBO_NUMBER))->EnableWindow(0);
            }
        }
        else
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->EnableWindow(0);
            ((CComboBox*)GetDlgItem(IDC_COMBO_NUMBER))->EnableWindow(0);
        }

}





//void CBacnetSettingParamter::OnCbnSelchangeComboType()
//{
//    // TODO: Add control notification handler code here
//    // TODO: 在此添加控件通知处理程序代码
//    CString temp_string;
//    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
//    ((CComboBox *)GetDlgItem(IDC_COMBO_TYPE))->GetLBText(nSel, temp_string);
//    for (int i = 0;i<sizeof(DisplayType) / sizeof(DisplayType[0]);i++)
//    {
//        if (temp_string.CompareNoCase(DisplayType[i]) == 0)
//        {
//            Device_Basic_Setting.reg.display_lcd.lcd_mod_reg.npoint.point_type = i;
//            break;
//        }
//    }
//    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
//    {
//        CString temp_task_info;
//        temp_task_info.Format(_T("Change LCD point type timeout!"));
//        SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
//        return;
//    }
//}


//void CBacnetSettingParamter::OnCbnSelchangeComboNumber()
//{
//    // TODO: Add control notification handler code here
//    // TODO: 在此添加控件通知处理程序代码
//    CString temp_string;
//    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_NUMBER))->GetCurSel();
//    ((CComboBox *)GetDlgItem(IDC_COMBO_NUMBER))->GetLBText(nSel, temp_string);
//    int temp_value;
//    temp_value = (unsigned char)_wtoi(temp_string);
//    Device_Basic_Setting.reg.display_lcd.lcd_mod_reg.npoint.number = temp_value;
//    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
//    {
//        CString temp_task_info;
//        temp_task_info.Format(_T("Change LCD point number timeout!"));
//        SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
//        return;
//    }
//}


void CBacnetSettingParamter::OnBnClickedButtonLcdOk()
{
    // TODO: 在此添加控件通知处理程序代码
    if ((Device_Basic_Setting.reg.mini_type == T3_TSTAT10) ||
        (Device_Basic_Setting.reg.mini_type == T3_OEM) ||
        (Device_Basic_Setting.reg.mini_type == T3_OEM_12I))
    {
        if (((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 539)
        {

            CString temp_type; CString temp_number;
            GetDlgItemText(IDC_COMBO_TYPE, temp_type);
            GetDlgItemText(IDC_COMBO_NUMBER, temp_number);

            for (int i = 0; i < sizeof(DisplayType) / sizeof(DisplayType[0]); i++)
            {
                if (temp_type.CompareNoCase(DisplayType[i]) == 0)
                {
                    Device_Basic_Setting.reg.display_lcd.lcd_mod_reg.npoint.point_type = i;
                    break;
                }
            }
            int temp_value;
            temp_value = (unsigned char)_wtoi(temp_number);
            Device_Basic_Setting.reg.display_lcd.lcd_mod_reg.npoint.number = temp_value;
        }
    }

    if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
    {
        CString temp_task_info;
        temp_task_info.Format(_T("Change LCD point type timeout!"));
        SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
        return;
    }
    else
    {
        MessageBox(_T("success!"));
        PostMessage(WM_CLOSE, NULL, NULL);
    }

}
