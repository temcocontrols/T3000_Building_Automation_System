#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetProgram dialog

class CBacnetProgram : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetProgram)

public:
	CBacnetProgram(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetProgram();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_PROGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	ListCtrlEx::CListCtrlEx m_program_list;
	virtual BOOL OnInitDialog();

	void Initial_List();
	afx_msg LRESULT Fresh_Program_List(WPARAM wParam,LPARAM lParam);
	LRESULT ProgramMessageCallBack(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT  ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonProgramEdit();
//	afx_msg void OnLvnItemchangedListProgram(NMHDR *pNMHDR, LRESULT *pResult);
	void UnCheckOtherItem(ListCtrlEx::CListCtrlEx& listCtrl, int index);
//	afx_msg void OnHdnItemclickListProgram(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnItemActivateListProgram(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListProgram(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT Fresh_Program_Item(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	void Reg_Hotkey();
	void Unreg_Hotkey();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnNMDblclkListProgram(NMHDR *pNMHDR, LRESULT *pResult);
	void Reset_Program_Rect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	bool window_max; // 标记是否已经最大化;
	afx_msg void OnBnClickedButton1();
};

	const int PROGRAM_NUM = 0;
	const int PROGRAM_FULL_LABLE = 1;
	const int PROGRAM_STATUS = 2;
	const int PROGRAM_AUTO_MANUAL = 3;
	const int PROGRAM_SIZE_LIST = 4;
	const int PROGRAM_RUN_STATUS = 5;
	const int PROGRAM_LABEL = 6 ;

	const int PROGRAM_COL_NUMBER = 7;
	


	const CString Auto_Manual[2]=
	{ 
		_T("Auto"),
		_T("Manual")
	} ;

	const CString ProgramStatus[2]=
	{
		_T("ON"),
		_T("OFF")
	};

//	const int KEY_INSERT = 1020;


