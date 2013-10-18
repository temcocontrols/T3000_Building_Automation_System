#pragma once


// CRegDetails dialog

class CRegDetails : public CDialogEx
{
	DECLARE_DYNAMIC(CRegDetails)

public:
	CRegDetails(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegDetails();

// Dialog Data
	enum { IDD = IDD_MBPOLL_REG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit editAlias;
	CEdit editValue;
	CStatic staticAddress;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

