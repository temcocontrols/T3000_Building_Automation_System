#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
#include "afxwin.h"
// CFlash_Multy dialog
typedef struct
{
	int nitem;
	CString nID;
	CString devicename;
	CString strSN;
	CString ncomport;
	CString nIPaddress;
	CString nipport;
	bool	b_is_sub;
	CString file_position;
	int	nresult;
	CString config_file_position;

}Str_flash_device;

class CFlash_Multy : public CDialogEx
{
	DECLARE_DYNAMIC(CFlash_Multy)

public:
	CFlash_Multy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFlash_Multy();

// Dialog Data
	enum { IDD = IDD_DIALOG_MULTY_FLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_flash_multy_list;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	virtual void OnCancel();
	void Initial_List();
	BOOL IsOurNetDevice(int DevType);
	void Initial_Parameter();
	afx_msg void OnBnClickedButtonApplyWoAllSelect();
	afx_msg void OnBnClickedButtonMultySel();
	afx_msg void OnBnClickedButtonStatrt();
	void SetAutoConfig(Str_flash_device ndevice_info);
	static DWORD WINAPI  multy_isp_thread(LPVOID lpVoid);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT MultyFlashMessage(WPARAM wParam,LPARAM lParam);
	CListBox m_multy_flash_listbox;
	BOOL			m_bTstatLoadFinished;
	afx_msg void OnNMClickListFlashMulty(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonMultySel2();
	CEdit m_configure_edit;
	CButton m_AllCheckBtn;
	CString m_configure_path;
	CString		m_strLogFilePath;
	CString m_StrTips_old;
	CString m_StrTips_new;
	afx_msg void OnBnClickedAllcheck();
 
};

const int FLASH_ITEM = 0;
const int FLASH_SERIAL_NUMBER = 1;
const int FLASH_ID = 2;
const int FLASH_PRODUCT_NAME = 3;
const int FLASH_COM_PORT = 4;
const int FLASH_IPADDRESS = 5;
const int FLASH_IPPORT = 6;
const int FLASH_SUB_NOTE = 7;
const int FLASH_FILE_POSITION = 8;
const int FLASH_CONFIG_FILE_POSITION = 9;
const int FLASH_RESULTS = 10;
const int FLASH_CONFIG_RESULTS = 11;

const int FLASH_MAX_COUNT = 12;



