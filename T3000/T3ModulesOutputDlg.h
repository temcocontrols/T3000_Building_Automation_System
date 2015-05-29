#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"

// CT3ModulesOutputDlg dialog

class CT3ModulesOutputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CT3ModulesOutputDlg)

public:
	CT3ModulesOutputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CT3ModulesOutputDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_T3_OUTPUTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
void Fresh();
afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
afx_msg LRESULT Change_Input_Item(WPARAM wParam,LPARAM lParam);
afx_msg void OnNMClickList_output(NMHDR *pNMHDR, LRESULT *pResult);

public:
    int m_sn;
    ListCtrlEx::CListCtrlEx m_outputlist;
};
