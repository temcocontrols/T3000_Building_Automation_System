#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"

// CBacnetIOConfig dialog

class CBacnetIOConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetIOConfig)

public:
	CBacnetIOConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetIOConfig();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_EXT_IO_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_ext_io_config_list;
	void Initial_List();
	BOOL ExtproductIDisValiable(unsigned char temp_id);
	void CaculateIOCount(int nitem);
	afx_msg LRESULT Fresh_Extio_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Extio_Item(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonExtioAdd();
	afx_msg void OnBnClickedButtonExtioDeleteSel();
	afx_msg void OnNMClickListIoconfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonExtOk();
	afx_msg void OnBnClickedButtonExtCancel();
};
