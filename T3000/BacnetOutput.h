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
	afx_msg LRESULT Fresh_Output_List(WPARAM wParam,LPARAM lParam);
//	afx_msg LRESULT OutputResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Output_Item(WPARAM wParam,LPARAM lParam);
	ListCtrlEx::CListCtrlEx m_output_list;
	void Initial_List();

	afx_msg void OnBnClickedButtonOutputApply();
	afx_msg void OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

const int OUTPUT_NUM = 0;
const int OUTPUT_FULL_LABLE = 1;
const int OUTPUT_AUTO_MANUAL = 2;
const int OUTPUT_VALUE = 3;
const int OUTPUT_UNITE = 4;
const int OUTPUT_RANGE = 5;
const int OUTPUT_0_PERSENT = 6;
const int OUTPUT_100_PERSENT = 7;
const int OUTPUT_DECOM = 8;
const int OUTPUT_LABLE = 9;

const CString Output_Decom_Array[2] =
{
	_T("OK"),
	_T("-"),
};