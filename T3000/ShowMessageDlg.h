#pragma once
#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
#include "afxcmn.h"

// CShowMessageDlg 对话框
const int EVENT_AUTO_CLOSE = 0;
const int EVENT_IP_AUTO = 1;
const int EVENT_IP_STATIC_CHANGE = 2;
const int EVENT_MSTP_CONNECTION_ESTABLISH = 3;
const int EVENT_FIRST_LOAD_PROG = 4;
const int EVENT_CHANGE_PROTOCOL = 5;
const int EVENT_WARNING_CHANGE_PROTOCOL_BAUDRATE = 6;
const int EVENT_SYNC_TIME = 7;

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

    _Bac_Scan_Com_Info m_mstp_device_info;  // MSTP 用于确认 whois 是否有回复;

    void SetStaticText(LPCTSTR lpszTitleText);
    void SetStaticTextBackgroundColor(COLORREF TitleTextBackColor);
    void SetStaticTextSize(int size_length, int size_width);
    void SetStaticTextColor(COLORREF TitleTextBackColor);
    void SetProgressAutoClose( int mi_seconds, int time_count, int nEvent = EVENT_AUTO_CLOSE);
    void SetEvent(int nEvent);
    void SetChangeProtocol(bool modbus_to_bacnet,   // 0  modbus to bacnet          1  bacnet to modbus
        unsigned char modbus_id,
        unsigned short nreg_address,
        unsigned short nreg_value,
        unsigned char sub_device,         // 如果是子设备  ，数据库中的协议 比较特殊;
        LPCTSTR Dbpath);
    void SetHwnd(HWND h_hwnd, int nMessage);
    void SetMstpDeviceInfo(_Bac_Scan_Com_Info deviceinfo); // MSTP 用于确认 whois 是否有回复;
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

    //更改协议所用参数
    bool cprotocol_modbus_to_bacnet;   // 0  modbus to bacnet          1  bacnet to modbus
    unsigned char cprotocol_modbus_id;
    unsigned short cprotocol_nreg_address;
    unsigned short cprotocol_nreg_value;
    unsigned char cprotocol_sub_device;         // 如果是子设备  ，数据库中的协议 比较特殊;
    CString cprotocol_Dbpath;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    int m_exit_by_hands;

};



