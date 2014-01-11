#pragma once
#include "afxwin.h"
#include "MainFrm.h"

// CStaticDlg dialog

class CStaticDlg : public CDialog
{
	DECLARE_DYNAMIC(CStaticDlg)

public:
	CStaticDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStaticDlg();
	virtual BOOL OnInitDialog();
	void SetText(CString lab);
// Dialog Data
	enum { IDD = IDD_GRID_LAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_name;
	CString m_lab;
};
