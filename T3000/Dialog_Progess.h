#pragma once
#include "afxcmn.h"
#include "LightingController/LightingController.h"


// CDialog_Progess dialog

class CDialog_Progess : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Progess)

public:
	CDialog_Progess(CWnd* pParent = NULL,int lower = 1,int upper =100);   // standard constructor
	virtual ~CDialog_Progess();

// Dialog Data
	enum { IDD = IDD_DIALOG10_Progress };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	virtual BOOL OnInitDialog();
	int m_lower;
	int m_upper;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_percent;
	int m_setsetup;
	int m_setpos;
	afx_msg void OnClose();
protected:
	virtual void OnOK();
//	CLightingController dlg;
public:
	void ShowProgress(int setpos,int percent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void settimer(BOOL bpram,int lower,int upper,BOOL bend);
	int GetPOS();

	BOOL m_start;
	BOOL m_end;
};
