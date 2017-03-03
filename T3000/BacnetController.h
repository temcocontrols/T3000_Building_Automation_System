#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"

// BacnetController dialog

class BacnetController : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetController)

public:
	BacnetController(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetController();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_CONTROLLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_controller_list;
	void Initial_List();
	afx_msg LRESULT ControllerMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Controller_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Controller_Item(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickListController(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Reset_Controller_Rect();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	bool window_max; // 标记是否已经最大化;

};



