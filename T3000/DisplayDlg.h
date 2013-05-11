#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDisplayDlg dialog

class CDisplayDlg : public CDialog
{
	DECLARE_DYNAMIC(CDisplayDlg)

public:
	CDisplayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayDlg();
	 BOOL OnInitDialog();
	 void InitDlgData();
	 int GetValueFromID(unsigned short reg);
	  CString GetTextFromReg(unsigned short reg);
	  CString GetTextReg(unsigned short reg);
	  int Get_RegID(int Model_ID,CString Name);
	  CWinThread* pParamBackFresh;
	  void OnDestroy();
// Dialog Data
	enum { IDD = IDD_DISPLAY_CONFIG };
	int m_nCurRow;
	int m_nCurCol;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusLine1();
	void Fresh_Grid();
	CString GetSel(int ID);
	CMsflexgrid m_FlexGrid1;
	afx_msg void OnEnKillfocusLine2();
	afx_msg void OnEnKillfocusInput1();
	afx_msg void OnEnKillfocusInput2();
	afx_msg void OnEnKillfocusInput3();
	afx_msg void OnEnKillfocusInput4();
	afx_msg void OnEnKillfocusInput5();
	afx_msg void OnEnKillfocusInput6();
	afx_msg void OnEnKillfocusInput7();
	afx_msg void OnEnKillfocusInput8();
	afx_msg void OnEnKillfocusInput9();
	int m_display_number;
	DECLARE_EVENTSINK_MAP()
	void ClickInputMsflexgrid();
	CComboBox m_ItemValueCombx;
	afx_msg void OnCbnSelchangeSeqCom();
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_progressctrl;
};
