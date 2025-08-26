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
	// Flash serial number
	// �� ���к�
	void			FlashSN();

	unsigned long get_file_last_serialnumber();

protected:
	CComboBox		m_cbxCom;

	int						m_nFlashTypeFlag;			// =0 flash by com;  =1 flash by ethernet;
	vector<int>			m_szMdbIDs;

	//CString				m_strHexFileName;

	// Pointer to COM flash class, instantiated only when in use, released immediately after use
	CComWriter*	m_pComWriter;				// �ô���flash�����ָ�룬��ʹ��ʱ��ʵ����������������ͷ�
	// Use network, TFTP protocol flash, instantiated when in use
//	TFTPServer*	m_pTFTPServer;				// ʹ�����磬TFTPЭ��flash��ʹ��ʱʵ����
	// File buffer, used to store the content of read files, should be deleted after flash, new when in use
	//char*				m_pFileBuf;						// �ļ���������������Ŷ�ȡ���ļ������ݣ�flash���Ӧ��delete��ʹ��ʱ��new

	
	// flash sn
	// Whether to flash serial number
	BOOL				m_bFlashSN;					// �Ƿ���serial number.
	// Whether to flash Mac address
//	BOOL				m_bFlashMacAddr;			// �Ƿ���Mac address


};
