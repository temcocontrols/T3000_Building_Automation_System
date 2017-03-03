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
	afx_msg void OnBnClickedInsert();
	static	DWORD WINAPI  ReadScreenThreadfun(LPVOID lpVoid);
	static DWORD WINAPI  WritePictureThread(LPVOID lpVoid);
	static DWORD WINAPI  GetPictureThread(LPVOID lpVoid);
	bool read_screen_label();
	afx_msg void BacnetScreen::OnNMDblclkListScreen(NMHDR *pNMHDR, LRESULT *pResult);
	//int WritePicFileFunction(CString ChooseFilePath,unsigned char screen_index);
	int   WritePicFileFunction(CString ChooseFilePath,unsigned char screen_index,CString pic_filename);
	int GetPicFileFunction(unsigned char screen_index ,CString temp_image_folder);
//	afx_msg void OnDestroy();
	void Reset_Screen_Rect();
	CString pic_filename;
	void OnSize(UINT nType, int cx, int cy);
	void OnSysCommand(UINT nID, LPARAM lParam);
	bool window_max; // 标记是否已经最大化;
};


const int SCREEN_NUM			= 0;
const int SCREEN_DESCRIPTION	= 1;
const int SCREEN_LABEL			= 2;
const int SCREEN_PIC_FILE		= 3;
//const int SCREEN_MODE			= 4;
const int SCREEN_ELEMENT_COUNT		= 4;

const int SCREEN_COL_NUMBER = 5;

