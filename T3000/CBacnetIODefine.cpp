// CBacnetIODefine.cpp: Implementation file
// CBacnetIODefine.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetIODefine.h"
#include "afxdialogex.h"
#include "global_function.h"


// CBacnetIODefine dialog
// CBacnetIODefine 对话框

IMPLEMENT_DYNAMIC(CBacnetIODefine, CDialogEx)

CBacnetIODefine::CBacnetIODefine(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REDEFINE_IO_COUNT, pParent)
{

}

CBacnetIODefine::~CBacnetIODefine()
{
}

void CBacnetIODefine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetIODefine, CDialogEx)
    ON_BN_CLICKED(IDOK, &CBacnetIODefine::OnBnClickedOk)
END_MESSAGE_MAP()


// CBacnetIODefine message handlers
// CBacnetIODefine 消息处理程序


BOOL CBacnetIODefine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// TODO:  在此添加额外的初始化
    if ((Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 636) &&
        (g_selected_product_id == PM_ESP32_T3_SERIES))
    {
        CString temp_input;
        CString temp_output;
        CString temp_variable;
        temp_input.Format(_T("%d"), Device_Basic_Setting.reg.max_in);
        temp_output.Format(_T("%d"), Device_Basic_Setting.reg.max_out);
        temp_variable.Format(_T("%d"), Device_Basic_Setting.reg.max_var);
        GetDlgItem(IDC_EDIT_INPUT_COUNT)->SetWindowTextW(temp_input);
        GetDlgItem(IDC_EDIT_OUTPUT_COUNT)->SetWindowTextW(temp_output);
        GetDlgItem(IDC_EDIT_VARIABLE_COUNT)->SetWindowTextW(temp_variable);
    }
    else
    {
        GetDlgItem(IDC_EDIT_INPUT_COUNT)->SetWindowTextW(_T(""));
        GetDlgItem(IDC_EDIT_OUTPUT_COUNT)->SetWindowTextW(_T(""));
        GetDlgItem(IDC_EDIT_VARIABLE_COUNT)->SetWindowTextW(_T(""));
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // Exception: OCX property pages should return FALSE
				  // 异常: OCX 属性页应返回 FALSE
}


void CBacnetIODefine::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    // TODO: 在此添加控件通知处理程序代码
    CString temp_input_count;
    CString temp_output_count;
    CString temp_var_count;
    GetDlgItemTextW(IDC_EDIT_INPUT_COUNT, temp_input_count);
    GetDlgItemTextW(IDC_EDIT_OUTPUT_COUNT, temp_output_count);
    GetDlgItemTextW(IDC_EDIT_VARIABLE_COUNT, temp_var_count);

    unsigned short temp_int_input = unsigned short(_wtoi(temp_input_count));
    unsigned short temp_int_output = unsigned short(_wtoi(temp_output_count));
    unsigned short temp_int_variable = unsigned short(_wtoi(temp_var_count));
    if ((temp_int_input > 256) && (temp_int_input == 0))
    {
        MessageBox(_T("The number of input cannot be greater than 256"));
        return;
    }
    if ((temp_int_output > 256) && (temp_int_output == 0))
    {
        MessageBox(_T("The number of output cannot be greater than 256"));
        return;
    }
    if ((temp_int_variable > 256) && (temp_int_variable == 0))
    {
        MessageBox(_T("The number of variable cannot be greater than 256"));
        return;
    }
    if (g_selected_product_id == PM_ESP32_T3_SERIES)
    {
        Device_Basic_Setting.reg.max_in = temp_int_input;
        Device_Basic_Setting.reg.max_out = temp_int_output;
        Device_Basic_Setting.reg.max_var = temp_int_variable;
    }
    else
    {
        Device_Basic_Setting.reg.max_in = 64;
        Device_Basic_Setting.reg.max_out = 64;
        Device_Basic_Setting.reg.max_var = 128;
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

    CDialogEx::OnOK();
}
