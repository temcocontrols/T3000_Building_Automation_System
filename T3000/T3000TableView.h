#pragma once


// CT3000TableView view

class CT3000TableView : public CTabView
{
	DECLARE_DYNCREATE(CT3000TableView)

protected:
	CT3000TableView();           // protected constructor used by dynamic creation
	virtual ~CT3000TableView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnChangeActiveTab(WPARAM wp,LPARAM lp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



