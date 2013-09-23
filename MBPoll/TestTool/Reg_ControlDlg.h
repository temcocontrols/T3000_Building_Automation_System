
// Reg_ControlDlg.h : header file
//

#pragma once
#include "resource.h"
#include "ModbusDllforVc.h"

#include <vector>
using namespace std;

// CReg_ControlDlg dialog
class CReg_ControlDlg : public CDialogEx
{
// Construction
public:
	CReg_ControlDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CReg_ControlDlg();
// Dialog Data
	enum { IDD = IDD_REG_CONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CComboBox m_port;
//	vector<CString> m_szComm;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void OnDisconnect();
	bool m_port_open;
	bool OnConnect(int comport);

	CEdit m_reg_id;
	CEdit m_reg_on_time;
	CEdit m_reg_cycle;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedBtnDeviceIdWrite();
	afx_msg void OnBnClickedOutputBtnWrite();
	HANDLE hFirstThread;
	bool out_put_bit[64];


	int reg_100;
	int reg_101;

	int reg_102;
	int reg_103;


	afx_msg void OnBnClickedButton4();
	void fresh_output();

		static DWORD WINAPI ReadRegAddress(LPVOID lpVoid);
		bool suspend_thread;
//		afx_msg void OnSetupPolldefinition();
};
