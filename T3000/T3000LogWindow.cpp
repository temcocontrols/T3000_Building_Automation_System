// T3000LogWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000LogWindow.h"
#include "afxdialogex.h"


// CT3000LogWindow 对话框

IMPLEMENT_DYNAMIC(CT3000LogWindow, CDialogEx)

CT3000LogWindow::CT3000LogWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOG_WINDOW, pParent)
{

}

CT3000LogWindow::~CT3000LogWindow()
{
}

void CT3000LogWindow::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_LOG_WINDOW, m_t3000_log_winddow_listbox);
}


BEGIN_MESSAGE_MAP(CT3000LogWindow, CDialogEx)
    ON_MESSAGE(WM_ADD_SHOWLOG_CSTRING, ShowString)
END_MESSAGE_MAP()


// CT3000LogWindow 消息处理程序

extern CString temp_statusbar_log_path;

BOOL CT3000LogWindow::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    ::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    // TODO:  在此添加额外的初始化
    m_t3000_log_window = this->m_hWnd;

    int n_logcount = 0;
    n_logcount = GetPrivateProfileInt(_T("Setting"), _T("LogCount"), 0, temp_statusbar_log_path);

    for (int i = 1; i < n_logcount; i++)
    {
        CString temp_count;
        temp_count.Format(_T("%d"), i);
        CString log_string;
        GetPrivateProfileString(_T("Log"), temp_count, _T(""), log_string.GetBuffer(MAX_PATH), MAX_PATH, temp_statusbar_log_path);
        log_string.ReleaseBuffer();

        CTime print_time = CTime::GetCurrentTime();
        CString str = print_time.Format("%H:%M:%S    ");


        log_string = str + log_string;
        m_t3000_log_winddow_listbox.AddString(log_string);
        m_t3000_log_winddow_listbox.SetTopIndex(m_t3000_log_winddow_listbox.GetCount() - 1);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


BOOL CT3000LogWindow::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN)
    {
        if ((pMsg->wParam == VK_RETURN))
            return 1;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
    return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CT3000LogWindow::ShowString(WPARAM wParam, LPARAM lParam)
{
    char* temp_my_cs = (char *)wParam;
    CString temp_des2;

    CTime print_time = CTime::GetCurrentTime();
    CString str = print_time.Format("%H:%M:%S    ");

    MultiByteToWideChar(CP_ACP, 0, temp_my_cs, (int)strlen((char *)temp_my_cs) + 1,
        temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
    temp_des2.ReleaseBuffer();

    temp_des2 = str + temp_des2;
    m_t3000_log_winddow_listbox.AddString(temp_des2);
    m_t3000_log_winddow_listbox.SetTopIndex(m_t3000_log_winddow_listbox.GetCount() - 1);


    if (temp_my_cs != NULL)
        delete temp_my_cs;
    return 0;
}


