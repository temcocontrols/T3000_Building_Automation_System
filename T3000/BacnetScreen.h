#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// BacnetScreen dialog

class BacnetScreen : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetScreen)

public:
	BacnetScreen(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetScreen();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_SCREENS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	ListCtrlEx::CListCtrlEx m_screen_list;
	void Initial_List();
	afx_msg LRESULT Fresh_Screen_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Screen_Item(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickListScreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT BacnetScreen::OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT  ScreenCallBack(WPARAM wParam, LPARAM lParam);
	LRESULT Screeenedit_close_handle(WPARAM wParam,LPARAM lParam);
	virtual void OnCancel();
	void Reg_Hotkey();
	void Unreg_Hotkey();
//	afx_msg void OnDestroy();
};


const int SCREEN_NUM			= 0;
const int SCREEN_DESCRIPTION	= 1;
const int SCREEN_LABEL			= 2;
const int SCREEN_PIC_FILE		= 3;
const int SCREEN_MODE			= 4;
const int SCREEN_REFRESH		= 5;

const int SCREEN_COL_NUMBER = 6;

