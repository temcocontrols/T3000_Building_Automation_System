// CTstatAQ_PM25_Parameter.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "afxdialogex.h"
#include "CTstatAQ_PM25_Parameter.h"
#include "TstatAQ.h"
#include "global_function.h"
// CTstatAQ_PM25_Parameter 对话框

IMPLEMENT_DYNAMIC(CTstatAQ_PM25_Parameter, CDialogEx)

CTstatAQ_PM25_Parameter::CTstatAQ_PM25_Parameter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TSTAT_AQ_PM25_PARAMETER, pParent)
{

}

CTstatAQ_PM25_Parameter::~CTstatAQ_PM25_Parameter()
{
}

void CTstatAQ_PM25_Parameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTstatAQ_PM25_Parameter, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM25_VALUE, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Value)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM10_VALUE2, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Value2)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM25_MIN, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Min)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM25_MAX, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Max)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM10_MIN, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Min)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM10_MAX, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Max)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM25_FILTER, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Filter)
	ON_EN_KILLFOCUS(IDC_EDIT_P_PM10_FILTER, &CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Filter)
	ON_MESSAGE(MY_RESUME_DATA, PM25MessageCallBack)
	ON_BN_CLICKED(IDC_RADIO_PM_AUTO, &CTstatAQ_PM25_Parameter::OnBnClickedRadioPmAuto)
	ON_BN_CLICKED(IDC_RADIO_PM_MANUAL, &CTstatAQ_PM25_Parameter::OnBnClickedRadioPmManual)
END_MESSAGE_MAP()


// CTstatAQ_PM25_Parameter 消息处理程序


LRESULT  CTstatAQ_PM25_Parameter::PM25MessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageWriteOneInfo* Write_Struct_feedback = (_MessageWriteOneInfo*)lParam;
	bool msg_result = WRITE_FAIL;
	msg_result = MKBOOL(wParam);

	CString Show_Results;
	CString temp_cs = Write_Struct_feedback->Changed_Name;
	if (msg_result)
	{
		Show_Results = _T("Change ") + temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
	}
	else
	{
		Show_Results = _T("Change ") + temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);

	}

	if (Write_Struct_feedback)
		delete Write_Struct_feedback;
	return 0;
}


BOOL CTstatAQ_PM25_Parameter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString temp_manual_value1;
	temp_manual_value1.Format(_T("%u"), product_register_value[100]);
	GetDlgItem(IDC_EDIT_P_PM25_VALUE)->SetWindowTextW(temp_manual_value1);
	CString temp_manual_value2;
	temp_manual_value2.Format(_T("%u"), product_register_value[101]);
	GetDlgItem(IDC_EDIT_P_PM10_VALUE2)->SetWindowTextW(temp_manual_value2);

	CString temp_25min;
	CString temp_25max;
	CString temp_10min;
	CString temp_10max;
	CString temp_25_filter;
	CString temp_10_filter;
	temp_25min.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_OUTPUT_MIN_RANGE]);
	GetDlgItem(IDC_EDIT_P_PM25_MIN)->SetWindowTextW(temp_25min);
	temp_25max.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_OUTPUT_MAX_RANGE]);
	GetDlgItem(IDC_EDIT_P_PM25_MAX)->SetWindowTextW(temp_25max);
	temp_10min.Format(_T("%u"), product_register_value[PM25_MODBUS_PM10_OUTPUT_MIN_RANGE]);
	GetDlgItem(IDC_EDIT_P_PM10_MIN)->SetWindowTextW(temp_10min);
	temp_10max.Format(_T("%u"), product_register_value[PM25_MODBUS_PM10_OUTPUT_MAX_RANGE]);
	GetDlgItem(IDC_EDIT_P_PM10_MAX)->SetWindowTextW(temp_10max);

	temp_25_filter.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_FILTER]);
	GetDlgItem(IDC_EDIT_P_PM25_FILTER)->SetWindowTextW(temp_25_filter);
	temp_10_filter.Format(_T("%u"), product_register_value[PM25_MODBUS_PM10_FILTER]);
	GetDlgItem(IDC_EDIT_P_PM10_FILTER)->SetWindowTextW(temp_10_filter);

	if (product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL] == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_PM_AUTO))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_PM_MANUAL))->SetCheck(0);
	}
	else if ((product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL] == 1) ||
		(product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL] == 2) ||
		(product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL] == 3))
	{
		((CButton*)GetDlgItem(IDC_RADIO_PM_AUTO))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_PM_MANUAL))->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Value()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_manual_value1;
	GetDlgItem(IDC_EDIT_P_PM25_VALUE)->GetWindowTextW(temp_manual_value1);
	temp_value = _wtoi(temp_manual_value1);
	if(product_register_value[100] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,100,temp_value,product_register_value[100],this->m_hWnd,IDC_EDIT_P_PM25_VALUE,_T(" Manual PM2.5 value "));

}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Value2()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_manual_value2;
	GetDlgItem(IDC_EDIT_P_PM10_VALUE2)->GetWindowTextW(temp_manual_value2);
	temp_value = _wtoi(temp_manual_value2);
	if (product_register_value[101] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 101, temp_value, product_register_value[101], this->m_hWnd, IDC_EDIT_P_PM10_VALUE2, _T(" Manual PM10 value "));

}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Min()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_25min;
	GetDlgItem(IDC_EDIT_P_PM25_MIN)->GetWindowTextW(temp_25min);
	temp_value = _wtoi(temp_25min);
	if (product_register_value[PM25_MODBUS_PM25_OUTPUT_MIN_RANGE] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM25_OUTPUT_MIN_RANGE, temp_value, product_register_value[PM25_MODBUS_PM25_OUTPUT_MIN_RANGE], this->m_hWnd, IDC_EDIT_P_PM25_MIN, _T(" PM2.5 Min Range "));
}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Max()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_25max;
	GetDlgItem(IDC_EDIT_P_PM25_MAX)->GetWindowTextW(temp_25max);
	temp_value = _wtoi(temp_25max);
	if (product_register_value[PM25_MODBUS_PM25_OUTPUT_MAX_RANGE] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM25_OUTPUT_MAX_RANGE, temp_value, product_register_value[PM25_MODBUS_PM25_OUTPUT_MAX_RANGE], this->m_hWnd, IDC_EDIT_P_PM25_MAX, _T(" PM2.5 Max Range "));
}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Min()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_10min;
	GetDlgItem(IDC_EDIT_P_PM10_MIN)->GetWindowTextW(temp_10min);
	temp_value = _wtoi(temp_10min);
	if (product_register_value[PM25_MODBUS_PM10_OUTPUT_MIN_RANGE] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM10_OUTPUT_MIN_RANGE, temp_value, product_register_value[PM25_MODBUS_PM10_OUTPUT_MIN_RANGE], this->m_hWnd, IDC_EDIT_P_PM10_MIN, _T(" PM10 Min Range "));
}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Max()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_10max;
	GetDlgItem(IDC_EDIT_P_PM10_MAX)->GetWindowTextW(temp_10max);
	temp_value = _wtoi(temp_10max);
	if (product_register_value[PM25_MODBUS_PM10_OUTPUT_MAX_RANGE] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM10_OUTPUT_MAX_RANGE, temp_value, product_register_value[PM25_MODBUS_PM10_OUTPUT_MAX_RANGE], this->m_hWnd, IDC_EDIT_P_PM10_MAX, _T(" PM10 Max Range "));
}





void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm25Filter()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_25_filter;
	GetDlgItem(IDC_EDIT_P_PM25_FILTER)->GetWindowTextW(temp_25_filter);
	temp_value = _wtoi(temp_25_filter);
	if (product_register_value[PM25_MODBUS_PM25_FILTER] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM25_FILTER, temp_value, product_register_value[PM25_MODBUS_PM25_FILTER], this->m_hWnd, IDC_EDIT_P_PM25_FILTER, _T(" PM2.5 Filter "));

}


void CTstatAQ_PM25_Parameter::OnEnKillfocusEditPPm10Filter()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short temp_value;
	CString temp_10_filter;
	GetDlgItem(IDC_EDIT_P_PM10_FILTER)->GetWindowTextW(temp_10_filter);
	temp_value = _wtoi(temp_10_filter);
	if (product_register_value[PM25_MODBUS_PM10_FILTER] != temp_value)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM10_FILTER, temp_value, product_register_value[PM25_MODBUS_PM10_FILTER], this->m_hWnd, IDC_EDIT_P_PM10_FILTER, _T(" PM10 Filter "));
}


void CTstatAQ_PM25_Parameter::OnBnClickedRadioPmAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	int nValue = 0;
	if (product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL, nValue,
		product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL], this->m_hWnd, IDC_EDIT_DTERM, _T(" Auto "));
}


void CTstatAQ_PM25_Parameter::OnBnClickedRadioPmManual()
{
	// TODO: 在此添加控件通知处理程序代码
	int nValue = 3;
	if (product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL] == nValue)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL, nValue,
		product_register_value[PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL], this->m_hWnd, IDC_EDIT_DTERM, _T(" Manual "));
}


BOOL CTstatAQ_PM25_Parameter::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
			return 0;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
