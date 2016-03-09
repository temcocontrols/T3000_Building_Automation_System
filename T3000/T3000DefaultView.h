#pragma once
#include "autoricheditctrl.h"
// CT3000DefaultView form view
class CT3000DefaultView : public CFormView
{
	DECLARE_DYNCREATE(CT3000DefaultView)

public:
	CT3000DefaultView();           // protected constructor used by dynamic creation
	virtual ~CT3000DefaultView();

public:
	enum { IDD = IDD_DIALOG_T3000_DEFAULT };
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
//	CAutoRichEditCtrl m_RichEditor;
	virtual void OnInitialUpdate();
	void Fresh();
	afx_msg void OnPaint();
};


