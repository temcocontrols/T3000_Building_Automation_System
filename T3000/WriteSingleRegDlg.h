#pragma once
#include "afxwin.h"


// CWriteSingleRegDlg dialog

class CWriteSingleRegDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteSingleRegDlg)

public:
	//CWriteSingleRegDlg(int ID,int Value,CWnd* pParent = NULL);   // standard constructor
	CWriteSingleRegDlg(CWnd* pParent = NULL);
	virtual ~CWriteSingleRegDlg();
	virtual BOOL OnInitDialog();
	void Set(int Add,int Value);
// Dialog Data
	enum { IDD = IDD_WRITEREGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_slave;
	UINT m_address;
	int m_value;
	afx_msg void OnBnClickedOk();
	CEdit m_valueedit;
	afx_msg void OnPaint();
};
