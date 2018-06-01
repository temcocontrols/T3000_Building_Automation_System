#pragma once

//#include "IOSetDlg.h"
// CIODisplayBar

class CIODisplayBar : public CDockablePane
{
	DECLARE_DYNAMIC(CIODisplayBar)

public:
	CIODisplayBar();
	virtual ~CIODisplayBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	//CIOSetDlg* m_pSetDlg;
	
};