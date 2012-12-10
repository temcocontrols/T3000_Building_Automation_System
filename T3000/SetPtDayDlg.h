#pragma once


#include "SetptGraphicBar.h"
// CSetPtDayDlg dialog

class CSetPtDayDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPtDayDlg)

public:
	CSetPtDayDlg(CView* pParent = NULL);   // standard constructor
	virtual ~CSetPtDayDlg();

// Dialog Data
	enum { IDD = IDD_SETPTDAYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
protected:
	Bitmap* m_pBkImg;
	Bitmap* m_pThumbImg;
	CRect m_ClientRC;

public:
	void LoadImage();
	CSetptGraphicBar* m_pSetPtCtrl;
	CSetptGraphicBar* m_pNightSetPt;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
