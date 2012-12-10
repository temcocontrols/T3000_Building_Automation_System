#pragma once


// CTrendLogWnd

class CTrendLogWnd : public CWnd
{
	DECLARE_DYNAMIC(CTrendLogWnd)

public:
	CTrendLogWnd();
	virtual ~CTrendLogWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
public:
	CRect m_rcClient;
};


