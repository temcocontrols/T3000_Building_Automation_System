#pragma once
#include "afxwin.h"

#include "CM5\CStatic\staticex.h"
// CBacnetAddRemoteDevice �Ի��� (dialog box)

class CBacnetAddRemoteDevice : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetAddRemoteDevice)

public:
    // Standard constructor
	CBacnetAddRemoteDevice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBacnetAddRemoteDevice();

// �Ի�������
// Dialog data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_ADD_REMOTE_DEVICE };
#endif

protected:
    // DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonRemotedlgScan();
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void InitialUI();
    void InitialStatic();
    static DWORD WINAPI AddRemoteThreadPro(LPVOID lPvoid);
    //CString cs_local_ip;
    CString cs_remote_ip;
    int m_bacnet_udp_port;
    int m_modbus_tcp_port;
    CStaticEx m_static_addinfo1;
    CStaticEx m_static_addinfo2;
    CStaticEx m_static_addinfo3;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CStaticEx m_static_addinfo4;
};
