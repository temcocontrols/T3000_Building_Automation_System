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
	CListCtrl listReg1;
	afx_msg void OnClickedButtonCancel1();
	afx_msg void OnClickedButtonEdit1();
	afx_msg void OnClickedButtonSend1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	void InitListControl1();
//	CMyOwnListCtrl mbPollFuncList;
	CMyOwnListCtrl mbpollFuncList1;
	CRect mbPollFuncListRect;
};
