#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"
 
// CUserManagermentDlg dialog

class CUserManagermentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserManagermentDlg)

public:
	CUserManagermentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserManagermentDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USERSMANAGERMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    ListCtrlEx::CListCtrlEx m_user_list;
    virtual BOOL OnInitDialog();
    void Load_Users();
    void Initial_UserList();
  
   afx_msg LRESULT Fresh_Lists(WPARAM wParam,LPARAM lParam);
   afx_msg LRESULT Change_Item_List(WPARAM wParam,LPARAM lParam);
};
