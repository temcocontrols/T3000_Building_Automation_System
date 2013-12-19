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
	CStaticEx m_static_range_units_select;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_analog_select;
	int m_digital_select;
	int m_output_Analog_select;
	int m_input_Analog_select;
	afx_msg void OnEnKillfocusEditRangeSelect();
	afx_msg void OnClose();
};

const CString Range_Type[] =
{
	_T("Analog Ranges"),
	_T("Digital Ranges"),
	_T("Custom Digital Ranges")
};
