#pragma once
#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"

// BacnetAnnualRoutine dialog

class BacnetAnnualRoutine : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetAnnualRoutine)

public:
	BacnetAnnualRoutine(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetAnnualRoutine();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_ANNUAL_ROUTINES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_annualr_list;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void Initial_List();
	afx_msg void OnClose();
	afx_msg LRESULT AnnualMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickListBacAnnuleList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT Fresh_Annual_Routine_Item(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Annual_Routine_List(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAnnualEdit();
	virtual void OnCancel();
	void Reg_Hotkey();
	void Unreg_Hotkey();
	afx_msg void OnNMDblclkListAnnuleList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	void Reset_Annual_Rect();
	void OnSize(UINT nType, int cx, int cy);
	void OnSysCommand(UINT nID, LPARAM lParam);

	bool window_max; // 标记是否已经最大化;
	//afx_msg LRESULT AnnualResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
};




