#pragma once



// CBTUMeterDlg form view

class CBTUMeterDlg : public CFormView
{
	DECLARE_DYNCREATE(CBTUMeterDlg)

protected:
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
};


