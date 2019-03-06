#pragma once
#include "afxwin.h"



// CBTUMeterDlg form view

class CBTUMeterDlg : public CFormView
{
	DECLARE_DYNCREATE(CBTUMeterDlg)
public:
	CBTUMeterDlg();           // protected constructor used by dynamic creation
	virtual ~CBTUMeterDlg();
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BTUMETER };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh();
	CEdit m_editor_address;
	CEdit m_edit_sn;
	CEdit m_edit_model;
	CEdit m_edit_fv;
	CEdit m_edit_hv;
	CComboBox m_combox_baudrate;
    static DWORD WINAPI Update_BTU_Thread(LPVOID lPvoid);
    afx_msg LRESULT UpdateBTUUI(WPARAM wParam, LPARAM lParam);
    CComboBox m_combo_temperature_unit;
    CComboBox m_combo_flow_unit;
//    afx_msg void OnBnClickedButtonClearUpOffset();
//    afx_msg void OnBnClickedButtonClearUpOffset2();
    afx_msg void OnPaint();
    CString cs_upper_temp;
    CString cs_bottom_temp;
    CString flow_rate;
    CString flow_rate_unit;
    CString Total_heat;
};


const CString BTU_TEMP_UNIT[] =
{
    _T("Deg.F"),
    _T("Deg.C")
};

const CString BTU_FLOW_UNIT[] =
{
    _T("L/sec"),
    _T("L/min"),
    _T("L/hour"),
    _T("m3/sec"),
    _T("m3/min"),
    _T("m3/hour"),  //6
    _T("T/sec"),
    _T("T/min"),
    _T("T/hour"),
    _T("G/sec"),
    _T("G/min"),
    _T("G/hour")
};

const CString BTU_AD_FLOW_UNIT[] =
{
    _T("L/s"),
    _T("L/m"),
    _T("L/h"),
    _T("m3/s"),
    _T("m3/m"),
    _T("m3/h"),  //6
    _T("T/s"),
    _T("T/m"),
    _T("T/h"),
    _T("G/se"),
    _T("G/m"),
    _T("G/h")
};

