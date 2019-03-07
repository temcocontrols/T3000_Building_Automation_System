// ParameterExtDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "ParameterExtDlg.h"
#include "afxdialogex.h"
#include "global_function.h"

// CParameterExtDlg 对话框

IMPLEMENT_DYNAMIC(CParameterExtDlg, CDialogEx)

CParameterExtDlg::CParameterExtDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARAMETERDIALOG_EXT, pParent)
{

}

CParameterExtDlg::~CParameterExtDlg()
{
}

void CParameterExtDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PARA_EXT_DELAY, m_edit_delay_time);
    DDX_Control(pDX, IDC_EDIT_PARA_EXT_TIME_REMAINING, m_edit_time_remaining);
    DDX_Control(pDX, IDC_EDIT_PARA_EXT_MIN_PWM, m_edit_min_pwm);
    DDX_Control(pDX, IDC_EDIT_ZIGBEE_PAN_ID, m_zigbee_panid);
}


BEGIN_MESSAGE_MAP(CParameterExtDlg, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT_PARA_EXT_DELAY, &CParameterExtDlg::OnEnKillfocusEditParaExtDelay)
    ON_EN_KILLFOCUS(IDC_EDIT_PARA_EXT_MIN_PWM, &CParameterExtDlg::OnEnKillfocusEditParaExtMinPwm)
    ON_BN_CLICKED(IDC_BUTTON_PARA_EXT_OK, &CParameterExtDlg::OnBnClickedButtonParaExtOk)
    ON_EN_KILLFOCUS(IDC_EDIT_PARA_EXT_TIME_REMAINING, &CParameterExtDlg::OnEnKillfocusEditParaExtTimeRemaining)
    ON_EN_KILLFOCUS(IDC_EDIT_ZIGBEE_PAN_ID, &CParameterExtDlg::OnEnKillfocusEditZigbeePanId)
END_MESSAGE_MAP()


// CParameterExtDlg 消息处理程序


BOOL CParameterExtDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CString temp_delay_time;
    CString temp_time_left;
    CString temp_min_pwm;

    temp_delay_time.Format(_T("%d"), product_register_value[804]);
    temp_time_left.Format(_T("%d"),  product_register_value[805]);
    temp_min_pwm.Format(_T("%d"),    product_register_value[806]);

    // TODO:  在此添加额外的初始化
    m_edit_delay_time.SetWindowTextW(temp_delay_time);
    //m_edit_delay_time.textColor(RGB(0, 0, 0));
    //m_edit_delay_time.bkColor(RGB(0,255,255));
    //m_edit_delay_time.setFont(18, 12, NULL, _T("Arial"));

    m_edit_time_remaining.SetWindowTextW(temp_time_left);
    //m_edit_time_remaining.textColor(RGB(0, 0, 0));
    //m_edit_time_remaining.bkColor(RGB(0,255,255));
    //m_edit_time_remaining.setFont(18, 12, NULL, _T("Arial"));

    m_edit_min_pwm.SetWindowTextW(temp_min_pwm);
    //m_edit_min_pwm.textColor(RGB(0, 0, 0));
    //m_edit_min_pwm.bkColor(RGB(0,255,255));
    //m_edit_min_pwm.setFont(18, 12, NULL, _T("Arial"));

    CString zigbeepanid;
    zigbeepanid.Format(_T("%d"), product_register_value[801]);
    m_zigbee_panid.SetWindowTextW(zigbeepanid);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


BOOL CParameterExtDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN)
        {
            CWnd *temp_focus = GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
            GetDlgItem(IDC_BUTTON_PARA_EXT_OK)->SetFocus();
            temp_focus->SetFocus();
            return 1;
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CParameterExtDlg::OnEnKillfocusEditParaExtDelay()
{
    // TODO: 在此添加控件通知处理程序代码
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_delay_time.GetWindowText(strText);
    int nValue = _wtoi(strText);
    if (nValue < 0 || nValue > 65535)
    {
        AfxMessageBox(_T("Relay1 Open Minimum Time (s) Must between 0 and 65535"));
        return;
    }

    if ((short)product_register_value[804] == nValue)	//Add this to judge weather this value need to change.
        return;

    if (write_one(g_tstat_id, 804, short(nValue)) > 0)
    {
        product_register_value[804] = nValue;
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Relay1 Open Minimum Time Success"));
    }
    else
    {
        CString temp_cs;
        temp_cs.Format(_T("%d"), product_register_value[804]);
        m_edit_delay_time.SetWindowTextW(temp_cs);
    }

}





void CParameterExtDlg::OnEnKillfocusEditParaExtMinPwm()
{
    // TODO: 在此添加控件通知处理程序代码
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_min_pwm.GetWindowText(strText);
    int nValue = _wtoi(strText);
    if (nValue < 0 || nValue > 65535)
    {
        AfxMessageBox(_T("Relay2 Open \nMinimum Time (s) Must between 0 and 65535"));
        return;
    }

    if ((short)product_register_value[806] == nValue)	//Add this to judge weather this value need to change.
        return;

    if (write_one(g_tstat_id, 806, short(nValue)) > 0)
    {
        product_register_value[806] = nValue;
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Relay3 Open Minimum Time Success"));
    }
    else
    {
        CString temp_cs;
        temp_cs.Format(_T("%d"), product_register_value[806]);
        m_edit_min_pwm.SetWindowTextW(temp_cs);
    }
}


void CParameterExtDlg::OnBnClickedButtonParaExtOk()
{
    // TODO: 在此添加控件通知处理程序代码
    PostMessage(WM_CLOSE, NULL, NULL);
}


void CParameterExtDlg::OnEnKillfocusEditParaExtTimeRemaining()
{
    // TODO: 在此添加控件通知处理程序代码
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_time_remaining.GetWindowText(strText);
    int nValue = _wtoi(strText);
    if (nValue < 0 || nValue > 65535)
    {
        AfxMessageBox(_T("Relay2 Open \nMinimum Time (s) Must between 0 and 65535"));
        return;
    }

    if ((short)product_register_value[805] == nValue)	//Add this to judge weather this value need to change.
        return;

    if (write_one(g_tstat_id, 805, short(nValue)) > 0)
    {
        product_register_value[805] = nValue;
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Relay2 Open Minimum Time Success"));
    }
    else
    {
        CString temp_cs;
        temp_cs.Format(_T("%d"), product_register_value[805]);
        m_edit_min_pwm.SetWindowTextW(temp_cs);
    }


}


void CParameterExtDlg::OnEnKillfocusEditZigbeePanId()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strText;
    m_edit_time_remaining.GetWindowText(strText);
    int nValue = _wtoi(strText);

    if (write_one(g_tstat_id, 801, short(nValue)) > 0)
    {
        product_register_value[801] = nValue;
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Zigbee Pan ID Success"));
    }
    else
    {
        CString temp_cs;
        temp_cs.Format(_T("%d"), product_register_value[801]);
        m_edit_min_pwm.SetWindowTextW(temp_cs);
    }
}
