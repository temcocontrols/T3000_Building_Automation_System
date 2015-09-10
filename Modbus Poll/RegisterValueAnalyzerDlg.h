#pragma once
 
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "global_struct.h"
// CRegisterValueAnalyzerDlg dialog
#define  DATATIME_TIMER 1
class CRegisterValueAnalyzerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterValueAnalyzerDlg)

public:
	CRegisterValueAnalyzerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterValueAnalyzerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	CChartAxis* GetSelectedAxis();
	 
public:
	CEdit	m_TitlesEdit;
	CListBox	m_SeriesList;
	CEdit	m_AxisMinValEdit;
	CEdit	m_AxisMaxValEdit;
	CButton	m_LegendVisBtn;
	CButton  	m_BackgndColSel;

	COLORREF m_BackColor ;

	

	CChartCtrl m_ChartCtrl;

	afx_msg void OnBnClickedBkgndColbtn();
	afx_msg void OnBnClickedLegendvisCheck();
	afx_msg void OnBnClickedPanCheck();
	afx_msg void OnBnClickedZoomCheck();
	afx_msg void OnEnKillfocusTitleEdit();
	afx_msg void OnBnClickedLeftaxisRadio();
	afx_msg void OnBnClickedBottomaxisRadio();
	afx_msg void OnBnClickedRightaxisRadio();
	afx_msg void OnBnClickedTopaxisRadio();
	afx_msg void OnBnClickedAxisvisibleCheck();
	afx_msg void OnBnClickedAxisinvertedCheck();
	afx_msg void OnBnClickedAxisautomaticCheck();
	afx_msg void OnBnClickedAxisgridvisCheck();
	afx_msg void OnBnClickedAxisscrollbarCheck();
	afx_msg void OnEnKillfocusAxisminvalEdit();
	afx_msg void OnEnKillfocusAxismaxvalEdit();
	afx_msg void OnEnKillfocusAxislabelEdit();


	void DrawerPoint();
	void Fresh();
   
	afx_msg void OnTimer(UINT_PTR nIDEvent);
 
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
