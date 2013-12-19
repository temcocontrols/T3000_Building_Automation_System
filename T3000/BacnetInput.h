#pragma once
#include "afxcmn.h"

//#include "CM5/MyOwnListCtrl.h"
#include "CM5/ListCtrlEx.h"
// CBacnetInput dialog

class CBacnetInput : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetInput)

public:
	CBacnetInput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetInput();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Input_Item(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT InputMessageCallBack(WPARAM wParam, LPARAM lParam);
	void Initial_List();
	ListCtrlEx::CListCtrlEx m_input_list;
	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedButtonCheckInvokeId();
//	afx_msg LRESULT  InputResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonRead();

	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
//int GetPrivateData();
//int WritePrivateData();

const int INPUT_NUM = 0;
const int INPUT_FULL_LABLE = 1;
const int INPUT_AUTO_MANUAL = 2;
const int INPUT_VALUE = 3;
const int INPUT_UNITE = 4;
const int INPUT_RANGE = 5;
const int INPUT_CAL = 6;
const int INPUT_FITLER = 7;
const int INPUT_DECOM = 8;
const int INPUT_LABLE = 9;

const int INPUT_COL_NUMBER = 10;

const CString Input_Range_Array[7] = 
{
	_T("Not Used"),
	_T("10K(-40->120)"),
	_T("I 4->20ma"),
	_T("V 0->10V"),
	_T("V 0->5V"),
	_T("V 0->24AC"),
	_T("TST Normal"),
};

const CString Input_Filter_Array[8] =
{
	_T("1"),
	_T("2"),
	_T("4"),
	_T("8"),
	_T("16"),
	_T("32"),
	_T("64"),
	_T("128")
};




const CString Input_Unit[42]=
{
	_T("Not Used"), 
	_T("¡æ"),
	_T("¨H"), 
	_T("FPM"), 
	_T("Pa"), 
	_T("KPa"), 
	_T("psi"), 
	_T("in_w"), 
	_T("Watts"), 
	_T("KW"), 
	_T("KWH"),
	_T("Volt"), 
	_T("KV"), 
	_T("Amps"), 
	_T("mA"), 
	_T("CFM"), 
	_T("Sec"), 
	_T("Min"), 
	_T("Hours"), 
	_T("Days"), 
	_T("time_unit"), 
	_T("ohms"),
	_T("procent"), 
	_T("RH"), 
	_T("ppm"), 
	_T("counts"), 
	_T("Open"), 
	_T("CFH"), 
	_T("GPM"), 
	_T("GPH"), 
	_T("GAL"), 
	_T("CF"), 
	_T("BTU"), 
	_T("CMH"),
	_T("custom1"), 
	_T("custom2"), 
	_T("custom3"), 
	_T("custom4"), 
	_T("custom5"), 
	_T("custom6"), 
	_T("custom7"), 
	_T("custom8")
};

const CString Input_Unit_Digit[2]=
{ 
	_T("Not Used"),
	_T("ON/OFF")
} ;

const CString Decom_Array[2] =
{
	_T("OK"),
	_T("-"),
};
