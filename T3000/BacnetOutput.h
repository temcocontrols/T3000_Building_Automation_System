#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetOutput dialog

class CBacnetOutput : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetOutput)

public:
	CBacnetOutput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetOutput();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON	hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOutputRead();
	afx_msg LRESULT OutputMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Output_List(WPARAM wParam,LPARAM lParam);
//	afx_msg LRESULT OutputResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Output_Item(WPARAM wParam,LPARAM lParam);
	ListCtrlEx::CListCtrlEx m_output_list;
	void Initial_List();
	void Reload_Unit_Type();
	afx_msg void OnBnClickedButtonOutputApply();
	afx_msg void OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	virtual void OnCancel();
};

const int OUTPUT_NUM = 0;
const int OUTPUT_FULL_LABLE = 1;
const int OUTPUT_AUTO_MANUAL = 2;
const int OUTPUT_HW_SWITCH = 3;
const int OUTPUT_VALUE = 4;
const int OUTPUT_UNITE = 5;
const int OUTPUT_RANGE = 6;
const int OUTPUT_0_PERSENT = 7;
const int OUTPUT_100_PERSENT = 8;
const int OUTPUT_DECOM = 9;
const int OUTPUT_LABLE = 10;

const int OUTPUT_COL_NUMBER = 11;

const CString Output_Decom_Array[2] =
{
	_T("OK"),
	_T("-"),
};

typedef enum
{
	HW_SW_OFF = 0,
	HW_SW_AUTO = 1,
	HW_SW_HAND = 2
}HW_SWITCH_ENUM;