#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"

// CProjectManagementDlg dialog

class CProjectManagementDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectManagementDlg)

public:
	CProjectManagementDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProjectManagementDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROJECTMANAGERMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    ListCtrlEx::CListCtrlEx m_project_list;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    void Load_Users();
    void Load_ProductList();
    void Initial_List();
    afx_msg LRESULT Fresh_Lists(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT Change_Item_List(WPARAM wParam,LPARAM lParam);
};
