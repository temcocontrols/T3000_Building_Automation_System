#pragma once
#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
#include "afxdtctl.h"

// CBacnetProgramDebug dialog

class CBacnetProgramDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetProgramDebug)

public:
	CBacnetProgramDebug(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetProgramDebug();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_PROGRAM_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx  m_program_debug_list;
	void Initial_List(unsigned int list_type);
	int Fresh_Program_List(unsigned int list_type);
	LRESULT Fresh_Program_Debug_Item(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnNMClickListProgramDebug(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_prg_debug_variable_time_picker;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMKillfocusDatetimepickerPrgVariable(NMHDR *pNMHDR, LRESULT *pResult);

};
