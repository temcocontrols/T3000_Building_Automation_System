#pragma once
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"

// CT3RangeDlg dialog
CString Range_Units[14] = {
	_T("RAW DATA"),
	_T("10K C"),
	_T("10K F"),
	_T("0-100%"),
	_T("ON/OFF"),
	_T("OFF/ON"),
	_T("Pulse Input"),
	_T("Lighting Control"),
	_T("TYPE3 10K C"),
	_T("TYPE3 10K F"),
	_T("NO USE"),
	_T("0-5V"),
	_T("0-10V"),
	_T("0-20 ma")
};
class CT3RangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CT3RangeDlg)

public:
	CT3RangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CT3RangeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_T3_RANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_show_unit;
	CStatic m_rang_pic;
	CStaticEx m_static_range_units_select;
	int m_current_range;
	int m_input_Analog_select; 
	virtual BOOL OnInitDialog();
	void Initial_window();
};
