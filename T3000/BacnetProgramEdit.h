#pragma once
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"
const int KEY_F2 = 1002;
const int KEY_F3 = 1003;
const int KEY_F5 = 1005;
const int KEY_F6 = 1006;
// CBacnetProgramEdit dialog

class CBacnetProgramEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetProgramEdit)

public:
	CBacnetProgramEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetProgramEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_PROGRAM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);//�ֶ�����.
	DECLARE_MESSAGE_MAP()
	bool Run_once_mutex;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	LRESULT  ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	LRESULT  Fresh_Program_RichEdit(WPARAM wParam,LPARAM lParam);
//	afx_msg void OnBnClickedButtonProgramSend();
	afx_msg void OnSend();
	afx_msg void OnClose();
	afx_msg void OnClear();
	afx_msg void OnLoadfile();
	afx_msg void OnSavefile();
	virtual void OnCancel();
	afx_msg void OnEnSetfocusRichedit2Program();
	void Initial_static();
	CListBox m_information_window;
	CStaticEx m_pool_size;
	CStaticEx m_program_size;
	CStaticEx m_free_memory;
};


