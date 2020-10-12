#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CDebugWindow dialog

class CDebugWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugWindow)

public:
	CDebugWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugWindow();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUG_TRACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Change_Transparency(int persent);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg LRESULT CDebugWindow::ShowString(WPARAM wParam,LPARAM lParam);
	CListBox m_debug_listbox;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDebugClearall();
	afx_msg void OnBnClickedButtonDebugPause();

	bool m_is_pause;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonDebugSave();
	CString Logfile_path;
	CStdioFile*		 m_plogFile;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonDebugNumToTime();
	afx_msg void OnBnClickedButtonDebugTimeToNum();
	CDateTimeCtrl m__day;
	CDateTimeCtrl m__time;
	CComboBox m_debug_window_combo_show;
	afx_msg void OnCbnSelchangeComboDebugChoose();
    afx_msg void OnBnClickedButtonDebug();
    afx_msg void OnBnClickedButtonWebTest();
};


