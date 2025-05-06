#pragma once
#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
#include "afxcmn.h"

// CShowMessageDlg 
const int EVENT_AUTO_CLOSE = 0;
const int EVENT_IP_AUTO = 1;
const int EVENT_IP_STATIC_CHANGE = 2;
const int EVENT_MSTP_CONNECTION_ESTABLISH = 3;
const int EVENT_FIRST_LOAD_PROG = 4;
const int EVENT_CHANGE_PROTOCOL = 5;
const int EVENT_WARNING_CHANGE_PROTOCOL_BAUDRATE = 6;
const int EVENT_SYNC_TIME = 7;
const int EVENT_MESSAGE_ONLY = 8;

class CShowMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowMessageDlg)

public:
	CShowMessageDlg(CWnd* pParent = NULL);   // 
	virtual ~CShowMessageDlg();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AA_SHOWMESSAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    CString static_text;
    CString static_percent;
    int     m_pos;
    COLORREF static_backcolor; //
    COLORREF static_textcolor; //
    int static_text_length ; // 
    int static_text_width ;  // 
    bool b_set_backcolor ;
    bool b_show_progress;
    int auto_close_time; // ;
    
    bool resize_message_window;
    int int_x;  //  static 
    int int_y;
    int int_cx;
    int int_cy;

    int auto_close_time_count; // ;
    int auto_close_time_count_old; // ;
    int mevent;

    HWND m_message_hwnd ;  //;
    int m_message ;

    _Bac_Scan_Com_Info m_mstp_device_info;  // MSTP  whois ;
    void SetMessageWindowSize(int xx, int yy, int c_xx, int c_yy);
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
        unsigned char sub_device,         //    ;
        LPCTSTR Dbpath);
    void SetHwnd(HWND h_hwnd, int nMessage);
    void SetMstpDeviceInfo(_Bac_Scan_Com_Info deviceinfo); // MSTP  whois ;
    CStaticEx m_static_title;
    CStaticEx m_static_persent;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CProgressCtrl m_progress_showmessage;
    static DWORD WINAPI ShowMessageThread(LPVOID lPvoid);


    //2 IP
    CString m_string_event_2_static_ip;
    void SetChangedIPaddress(LPCTSTR lp_ipaddr);
    afx_msg void OnClose();
    virtual void OnCancel();

    //
    bool cprotocol_modbus_to_bacnet;   // 0  modbus to bacnet          1  bacnet to modbus
    unsigned char cprotocol_modbus_id;
    unsigned short cprotocol_nreg_address;
    unsigned short cprotocol_nreg_value;
    unsigned char cprotocol_sub_device;         //    ;
    CString cprotocol_Dbpath;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    int m_exit_by_hands;

};



