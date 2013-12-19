#pragma once

//#include "GraphicView.h"

// CRelayLabel
class CRelayLabel : public CStatic
{
	DECLARE_DYNAMIC(CRelayLabel)
private:
	COLORREF m_bkClr;
	COLORREF m_bTxtClr;
public:
	float m_fFirmwareVersion;
	CString m_strValueText;

public:
	CRelayLabel();
	virtual ~CRelayLabel();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	afx_msg void OnPaint();
	void SetLabelInfo(int TstatID,int input_or_output,int nStatus,COLORREF textClr,COLORREF bkClr);
	void SetLabelInfo_General(int ndeviceid,int type,int nStatus,COLORREF textClr,COLORREF bkClr);
	void DispalyInputValue_General(int nStatus,COLORREF textClr,COLORREF bkClr);
	void DispalyOutputValue_General(int nStatus,COLORREF textClr,COLORREF bkClr);
	void DispalyVariableValue_General(int nStatus,COLORREF textClr,COLORREF bkClr);
	void DispalyInputValue(int nStatus,COLORREF textClr,COLORREF bkClr);
	void DispalyOutputValue(int nStatus,COLORREF textClr,COLORREF bkClr);
	void DispalyRigesterValue(int nStatus,COLORREF textClr,COLORREF bkClr);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};


