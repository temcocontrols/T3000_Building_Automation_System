#pragma once
#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
#include "afxcmn.h"

// CShowMessageDlg 对话框
const int EVENT_AUTO_CLOSE = 0;
const int EVENT_IP_AUTO = 1;
const int EVENT_IP_STATIC_CHANGE = 2;

class CShowMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowMessageDlg)

public:
	CShowMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowMessageDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AA_SHOWMESSAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    CString static_text;
    CString static_percent;
    int     m_pos;
    COLORREF static_backcolor; //背景色
    COLORREF static_textcolor; //字体色
    int static_text_length ; // 字体长宽
    int static_text_width ;  // 字体长宽
    bool b_set_backcolor ;
    bool b_show_progress;
    int auto_close_time; // 自动关闭时间;
    
    int auto_close_time_count; // 几次后自动关闭;
    int auto_close_time_count_old; // 用于计算百分比;
    int mevent;

    HWND m_message_hwnd ;  //用于回传消息;
    int m_message ;

    void SetStaticText(LPCTSTR lpszTitleText);
    void SetStaticTextBackgroundColor(COLORREF TitleTextBackColor);
    void SetStaticTextSize(int size_length, int size_width);
    void SetStaticTextColor(COLORREF TitleTextBackColor);
    void SetProgressAutoClose( int mi_seconds, int time_count, int nEvent = EVENT_AUTO_CLOSE);
    void SetHwnd(HWND h_hwnd, int nMessage);
    CStaticEx m_static_title;
    CStaticEx m_static_persent;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CProgressCtrl m_progress_showmessage;
    static DWORD WINAPI ShowMessageThread(LPVOID lPvoid);


    //事件2 静态IP修改
    CString m_string_event_2_static_ip;
    void SetChangedIPaddress(LPCTSTR lp_ipaddr);
    afx_msg void OnClose();
    virtual void OnCancel();
};



