#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "FlashBase.h"

// CTstatFlashDlg dialog
class CConfigFileHandler;

class CComWriter;
class TFTPServer;


class CTstatFlashDlg : public CDialog, public CFlashBase
{
	DECLARE_DYNAMIC(CTstatFlashDlg)

public:
	CTstatFlashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTstatFlashDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TSTAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	afx_msg LRESULT OnAddStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlashFinish(WPARAM wParam, LPARAM lParam);

public:	
	virtual BOOL OnInitDialog();
	void InitCombox();
	void InitSelBox();
	void InitFromConfigFile();
	void UpdateStatusInfo(const CString& strInfo, BOOL bReplace);

	//
	int				GetFlashType();
	int				GetModbusID(vector<int>& szMdbIDs);
	int				GetComPortNo();
	CString		GetIPAddressStr();
	DWORD		GetIPAddress();
	CString		GetIPPort();

	void			SaveToConfigFile(CConfigFileHandler& cfgFileHandler);

	BOOL			Flash();
// 	CString		GetFlashFile();
// 	void			SetFlashFile(const CString& strFlie);


protected:
	BOOL			FileValidation(const CString& strFileName);
	BOOL			ValidMdbIDString();
	void			FlashByCom();

	CString		GetCurSelPageStr();
	//  
	void			FlashSN();

	unsigned long get_file_last_serialnumber();

protected:
	CComboBox		m_cbxCom;

	int						m_nFlashTypeFlag;			// =0 flash by com;  =1 flash by ethernet;
	vector<int>			m_szMdbIDs;

	//CString				m_strHexFileName;

	CComWriter*	m_pComWriter;				// flash
//	TFTPServer*	m_pTFTPServer;				// TFTPflash
	//char*				m_pFileBuf;						// flashdeletenew

	
	// flash sn
	BOOL				m_bFlashSN;					// serial number.
//	BOOL				m_bFlashMacAddr;			// Mac address


};
