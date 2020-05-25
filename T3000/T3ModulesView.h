#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"

// CT3ModulesView form view

class CT3ModulesView : public CFormView
{
	DECLARE_DYNCREATE(CT3ModulesView)

public:
	CT3ModulesView();           // protected constructor used by dynamic creation
	virtual ~CT3ModulesView();

public:
	enum { IDD = IDD_DIALOG_T3_INPUTS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_T3_Input_List;
	virtual void OnInitialUpdate(); 
	void Fresh();
    CString GetValue(int regValue,int RangeValue);
    afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT Change_Input_Item(WPARAM wParam,LPARAM lParam);
    afx_msg void OnNMClickList_Input(NMHDR *pNMHDR, LRESULT *pResult);
private:
int m_sn;

public:
BOOL m_ThreadStop;
HANDLE hFirstThread;
    afx_msg void OnDestroy();
};


