
// RegisterMonitorDlg.h : header file
//

#pragma once
#include <vector>
using namespace std;
#include "ModbusDllforVc.h"
// CRegisterMonitorDlg dialog
class CRegisterMonitorDlg : public CDialogEx
{
// Construction
public:
	CRegisterMonitorDlg(CWnd* pParent = NULL);	// standard constructor
		virtual ~CRegisterMonitorDlg();
// Dialog Data
	enum { IDD = IDD_REGISTERMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CComboBox m_port;
	CComboBox m_combo_reg;
	int sel_item;
	static DWORD WINAPI ReadRegAddress(LPVOID lpVoid);
	static DWORD WINAPI WriteRegAddress(LPVOID lpVoid);

	int m_reg_address1;
	int m_reg_address2;
	int m_reg_address3;
	int m_reg_address4;
	int m_reg_address5;
	int m_device_ID;

	bool suspend_thread;

	CTime current_time;

	CString myapp_path;
	CString myData_path;
	CString WriteFileString;


bool have_First_Read;

	bool read_once1;
	bool read_once2;
	bool read_once3;
	bool read_once4;
	bool read_once5;

	DWORD reg_1_value;
	DWORD reg_2_value;
	DWORD reg_3_value;
	DWORD reg_4_value;
	DWORD reg_5_value;
	HANDLE hFirstThread;

	HANDLE hWriteThread;

	CEdit m_reg_edit1;
	CEdit m_reg_edit2;
	CEdit m_reg_edit3;
	CEdit m_reg_edit4;
	CEdit m_reg_edit5;
	CEdit  m_device_edit;


	int write_value;
	int write_register;

//	vector<CString> m_szComm;
	bool m_port_open;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
	void OnDisconnect();
private:


public:
	bool Save_Parameter();
	int read_interval_time;
	bool OnConnect(int comport);
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
	void InitParameters();
};
