#pragma once

#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
// BacnetRange dialog

class BacnetRange : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetRange)

public:
	BacnetRange(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetRange();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_RANGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CStaticEx m_static_range_title;
//	CStaticEx m_static_range_detail;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	void Initial_static();
	void Update_Custom_Units();
    void SetAllRadioButton(int button_index = 2);
	CStaticEx m_static_range_units_select;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_analog_select;
	int m_digital_select;
	int m_output_Analog_select;
	int m_input_Analog_select;
	afx_msg void OnEnKillfocusEditRangeSelect();
	afx_msg void OnClose();
	CStaticEx m_show_unit;
	afx_msg void OnBnClickedCancel();
	CStatic m_rang_pic;
	void Timer2_handle();
	afx_msg void OnBnClickedBtnEditCustomerRange();
	afx_msg void OnBnClickedBtnEditCustomerVarRange();
	afx_msg void OnBnClickedRadio73();
	afx_msg void OnBnClickedRadio74();
	afx_msg void OnBnClickedRadio75();
	afx_msg void OnBnClickedRadio76();
	afx_msg void OnBnClickedRadio77();
	afx_msg void OnBnClickedRadio78();
	afx_msg void OnBnClickedRadio79();
	afx_msg void OnBnClickedRadio80();
    void UpdateCustomerRangeText();
    afx_msg void OnBnClickedRadio82();
    afx_msg void OnBnClickedRadio83();
    afx_msg void OnBnClickedRadio84();
    afx_msg void OnBnClickedRadio85();
    afx_msg void OnBnClickedRadio86();
    void ShowAnalogCusRange();
    int m_device_type;
    afx_msg void OnBnClickedBtnEditMsvRange();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnBnClickedRadioMsv1();
    afx_msg void OnBnClickedRadioMsv2();
    afx_msg void OnBnClickedRadioMsv3();
    void DisableAnalogVarRadio();
    afx_msg void OnBnClickedRadio57();
    afx_msg void OnBnClickedRadio58();
    afx_msg void OnBnClickedRadio87();
    afx_msg void OnBnClickedRadio103();
	void RadioDegC_DegF(bool n_enable, bool n_deg_c= 1);
	void HandleSpecialRange();
	afx_msg void OnBnClickedRadioMsv4();
};

const CString Range_Type[] =
{
	_T("Analog Ranges"),
	_T("Digital Ranges"),
	_T("Custom Digital Ranges")
};

#define RANGE_RADIO_KEEP    0
#define RANGE_RADIO_ENABLE  1
#define RANGE_RADIO_DISABLE 2
