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
};


