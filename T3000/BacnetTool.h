#pragma once
#include "afxcmn.h"

#include "Bacnet_Include.h"
#include "bip.h"
// CBacnetTool dialog

class CBacnetTool : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetTool)

public:
	CBacnetTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetTool();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void InitialListView();
	void InitialBacnetConnection();
	CTreeCtrl m_TreeCtrl;
	afx_msg void OnBnClickedButtonBacToolExit();
	void InitialTemcoLogo();
	afx_msg LRESULT FreshListView(WPARAM wParam, LPARAM lParam);
	HBITMAP hBitmap;
	afx_msg void OnBnClickedButtonBacToolTest2();
};

const CString DEVICE_CS = _T("device: ");
const CString LAST_UPDATE_CS = _T("Last Update : ");
const CString IP_ADDRESS_CS = _T("IP Address: ");
const CString OBJECT_IDENTIFIER_CS = _T("object_identifier: ");
const CString VENDOR_IDENTIFIER_CS = _T("vendor_identifier: ");				

const int TREE_IS_NONE = 0;
const int TREE_NEED_NEW = 1;
const int TREE_ALREADY_EXSIT = 2;
