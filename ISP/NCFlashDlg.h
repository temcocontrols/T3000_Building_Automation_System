#pragma once
#include "FlashBase.h"
#include "afxcmn.h"
#include "ConfigFileHandler.h"
// CNCFlashDlg dialog


class TFTPServer;
class CNCFlashDlg : public CDialog, public CFlashBase
{
	DECLARE_DYNAMIC(CNCFlashDlg)

public:
	CNCFlashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNCFlashDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NETCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnAddStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlashFinish(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	virtual BOOL FileValidation(const CString& strFileName);

	virtual BOOL Flash();
		
	void SaveToConfigFile(CConfigFileHandler& cfgFileHandler);


protected:
	void FlashByEthernet();
	void InitFromConfigFile();
	void UpdateStatusInfo(const CString& strInfo, BOOL bReplace);
	DWORD GetIPAddress();


protected:
	int m_nFlashTypeFlag;	// 0 =com. 1= TCP
public:
	CIPAddressCtrl m_IPAddr;
	TFTPServer*	m_pTFTPServer;				// 使用网络，TFTP协议flash，使用时实例化
	afx_msg void OnBnClickedButtonPing();
};
