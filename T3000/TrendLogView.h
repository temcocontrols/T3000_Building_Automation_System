#pragma once
#include "TrendLogWnd.h"


// CTrendLogView form view

class CTrendLogView : public CFormView
{
	DECLARE_DYNCREATE(CTrendLogView)

public:
	CTrendLogView();           // protected constructor used by dynamic creation
	virtual ~CTrendLogView();

public:
	enum { IDD = IDD_TRENDLOGFORMVIEW };
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
public:
	void Fresh();
	afx_msg void OnBnClickedGobackbutton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedSavebutton();
	afx_msg void OnBnClickedLoadbutton();
public:
	CTrendLogWnd m_TrendLogWnd;
	CRect m_rcClient;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


