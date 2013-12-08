#pragma once

#include "MbpExterns.h"
#include "CM5\myownlistctrl.h"

// CMbPollFunctions dialog

class CMbPollFunctions : public CDialogEx
{
	DECLARE_DYNAMIC(CMbPollFunctions)

public:
	CMbPollFunctions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMbPollFunctions();

// Dialog Data
	enum { IDD = IDD_DIALOG_MBPOLL_FUNCS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton btnCancel1;
	CButton btnEdit1;
	CButton btnSend1;
//	CEdit btnAddress1;
//	CEdit btnSize1;
//	CEdit btnSlave1;
	CEdit editAddress1;
	CEdit editSize1;
	CEdit editSlave1;
	CMyOwnListCtrl listReg1;
	afx_msg void OnClickedButtonCancel1();
	afx_msg void OnClickedButtonEdit1();
	afx_msg void OnClickedButtonSend1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	void InitListControl1();
//	CMyOwnListCtrl mbPollFuncList;
//	CMyOwnListCtrl mbpollFuncList1;
	CRect mbPollFuncListRect;
protected:
	afx_msg LRESULT OnMyMbpollWriteRegMsg(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnEnUpdateEditSize1();
	afx_msg void OnEnKillfocusEditSize1();
	afx_msg void OnEnKillfocusEditAddress1();
	afx_msg void OnHdnItemclickListReg1(NMHDR *pNMHDR, LRESULT *pResult);
};
