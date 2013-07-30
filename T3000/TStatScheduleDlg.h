#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
// CTStatScheduleDlg dialog

class CTStatScheduleDlg : public CDialog
{
	DECLARE_DYNAMIC(CTStatScheduleDlg)

public:
	CTStatScheduleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTStatScheduleDlg();

// Dialog Data
	enum { IDD = IDD_TSTAT_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid m_fgWorkDay;
	CMsflexgrid m_fgWeekend;
	CEdit m_editInput;
	CSize		m_szEditPosInWork;
	CSize		m_szEditPosInWeekend;
	BOOL		m_bInWorkDay;
	virtual BOOL OnInitDialog();


protected:
	void InitGrid();
	void FillGridByRegister();
	void SaveToRegister();
	void CalcClickPos(CSize& size);
	void ShowInputEditBox(int iRow, int iCol, BOOL bWorkDay);
	CRect CalcGridCellRect(int iRow, int iCol, BOOL bWorkDay );
	void InitRegisterTable();
public:
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridWorking();
	void ClickMsflexgridWeekend();
	afx_msg void OnEnKillfocusEditInput();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnCbnSelchangeComboPmam();
	afx_msg void OnCbnKillfocusComboPmam();



protected:

	vector<int>			m_szRegTable;
public:
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedRadio24h();
	afx_msg void OnBnClickedRadio12h();
	CComboBox m_scheduleonoff;
	afx_msg void OnCbnSelchangeComboOnoff();
};
