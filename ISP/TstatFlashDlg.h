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
	// 烧 序列号
	void			FlashSN();

	unsigned long get_file_last_serialnumber();

protected:
	CComboBox		m_cbxCom;

	int						m_nFlashTypeFlag;			// =0 flash by com;  =1 flash by ethernet;
	vector<int>			m_szMdbIDs;

	//CString				m_strHexFileName;

	CComWriter*	m_pComWriter;				// 用串口flash的类的指针，在使用时才实例化，用完后立即释放
//	TFTPServer*	m_pTFTPServer;				// 使用网络，TFTP协议flash，使用时实例化
	//char*				m_pFileBuf;						// 文件缓冲区，用来存放读取的文件的内容，flash完后应当delete，使用时再new

	
	// flash sn
	BOOL				m_bFlashSN;					// 是否烧serial number.
//	BOOL				m_bFlashMacAddr;			// 是否烧Mac address


};
