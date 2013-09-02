#pragma once
#include "afxwin.h"


// CTestMultiReadTraffic dialog

class CTestMultiReadTraffic : public CDialogEx
{
	DECLARE_DYNAMIC(CTestMultiReadTraffic)

public:
	CTestMultiReadTraffic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestMultiReadTraffic();

// Dialog Data
	enum { IDD = IDD_TEST_MULTI_READ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listbox;
	UINT m_ID;
	UINT m_length;
	afx_msg void OnBnClickedOk();
	short m_startid;
	int m_index;
};
