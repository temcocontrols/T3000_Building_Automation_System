#pragma once
#include "CM5/ListCtrlEx.h"
#include "afxwin.h"

// CBacnetVariable dialog

class CBacnetVariable : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetVariable)

public:
	CBacnetVariable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetVariable();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_VARIABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonVariableRead();
	//afx_msg LRESULT  VariableResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Variable_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Variable_Item(WPARAM wParam,LPARAM lParam);
	void Initial_List();
	ListCtrlEx::CListCtrlEx m_variable_list;

	afx_msg void OnBnClickedButtonVariableApply();
	afx_msg void OnNMClickListVariable(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


const int VARIABLE_NUM = 0;
const int VARIABLE_FULL_LABLE = 1;
const int VARIABLE_AUTO_MANUAL = 2;
const int VARIABLE_VALUE = 3;
const int VARIABLE_UNITE = 4;
const int VARIABLE_LABLE = 5;
