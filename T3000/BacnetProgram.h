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
	afx_msg void OnBnClickedButtonProgramRead();
	afx_msg LRESULT Fresh_Program_List(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonProgramApply();
	afx_msg LRESULT  ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
};

	const int PROGRAM_NUM = 0;
	const int PROGRAM_FULL_LABLE = 1;
	const int PROGRAM_STATUS = 2;
	const int PROGRAM_AUTO_MANUAL = 3;
	const int PROGRAM_SIZE = 4;
	const int PROGRAM_RUN_STATUS = 5;
	const int PROGRAM_LABEL = 6 ;


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


