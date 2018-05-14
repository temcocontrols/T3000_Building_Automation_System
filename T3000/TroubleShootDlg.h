#pragma once

#include "global_variable_extern.h"
#include "global_function.h"
// CTroubleShootDlg dialog
#include "MainFrm.h"
#include "ScanDlg.h"
#include "afxwin.h"
#include "CM5/CStatic/staticex.h"
class CTroubleShootDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTroubleShootDlg)

public:
	CTroubleShootDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTroubleShootDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TROUBLESHOOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:


	virtual BOOL OnInitDialog();
	void SetNode(tree_product product_Node);
 
	BOOL TestPing(const CString& strIP);
	BOOL GetNewIP(CString &newIP,CString BaseIP);
	BOOL ChangeNetDeviceIP(CString strIP);
	void SaveNewIPAddress(CString newip,CString oldip);
public:
	int scan_item ;
	int scan_udp_item_TS;
	int scan_bacnet_ip_item ;
	int scan_baudrate ;
	vector <refresh_net_device> m_refresh_net_device_data;
	vector <Scan_Info> m_scan_info;
	vector <Scan_Info> m_scan_info_buffer;
	tree_product m_net_product_node;
	BOOL Step1;
	BOOL Step2;

	 vector<ALL_LOCAL_SUBNET_NODE> Vector_Subnet;
//	afx_msg void OnBnClickedButton2();
	CStaticEx m_connection_status;
//	afx_msg void OnBnClickedButton4();

	CStaticEx m_edit_newip;
	BOOL  b_changeip_ok;
	afx_msg void OnPaint();
    static DWORD WINAPI  thread_change_ip(LPVOID lpVoid);
    afx_msg void OnBnClickedChangeIdOk();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
