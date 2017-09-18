 #pragma once
#include "afxcmn.h"


// CBacnetUserConfig dialog
#include "CM5/ListCtrlEx.h"

class CBacnetUserConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetUserConfig)

public:
	CBacnetUserConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetUserConfig();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_USER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_user_config_list;
	void Initial_List();
	afx_msg LRESULT Fresh_User_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_User_Item(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickUserList(NMHDR *pNMHDR, LRESULT *pResult);
	void Refresh_parameter(UINT index);
	afx_msg void OnBnClickedCheckUserlistInfo();
	void Enable_Window_Stage(int nstage);
	afx_msg void OnBnClickedButtonUserOk();
	int m_slected_item;
	int ok_button_stage;
	afx_msg void OnBnClickedButtonUserDelete();
};

const int stage_enter_original_password = 1;
const int stage_enter_new_password = 2;
const int stage_retype_password = 3;


const int USERLIST_NAME = 0;

const int HIDE_ENTER_ORIGINAL_PASSWORD = 0;
const int ENTER_ORIGINAL_PASSWORD = 1;
const int ENTER_NEW_PASSWORD = 2;
const int RETYPE_NEW_PASSWORD = 3;

const int HIDE_ALL = 255;
