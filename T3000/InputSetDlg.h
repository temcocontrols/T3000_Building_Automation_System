#pragma once

#include "afxwin.h"
#include "msflexgrid1.h"
#include "T3000.h"
#include "MainFrm.h"

// CInputSetDlg dialog
#define  WM_REFRESH_INPUTDLG WM_USER + 401
class CInputSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputSetDlg)

public:
	CInputSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputSetDlg();

// Dialog Data
	enum { IDD = IDD_IPUTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Init5EGrid();
	void InitGrid5EnoTSTAT6();

	void Init_not_5ABCD_Grid();
	void Fresh_GridForTstat5E();
//	void Init5EGridtstat6();
	
	void SetCurrentCell();
	void OnCbnSelchangeRangComboFor5E();
	void OnBnClickedUpbuttonFor5E();
	void OnBnClickedDownbuttonFor5E();
	void OnCbnSelchangeAmcomboFor5E();
	void OnEnKillfocusInvalueeditFor5E();
	void DealValue_Digit();
	void OnCbnKillfocusValuecomboFor5E();
	void OnClick5EGrid(int nRow, int nCol, CRect rcCell);
	void OnClickTstat6Grid(int nRow, int nCol, CRect rcCell);
	void InitGridtstat6();
	void Fresh_GridForTstat6();
	void add_analog_rang(int i);
	void Fresh_Grid();
	void Fresh_GridForAll();
	void ClickMsflexgrid_Click();
	void ClickMsflexgrid5E(int nRow, int nCol, CRect rcCell);
	void FLEX_GRID1_PUT_COLOR_STR(int ,int,COLORREF grid_color);


	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	DECLARE_EVENTSINK_MAP()

	 void OnCbnSelchangeRangCombo();
	afx_msg void OnCbnKillfocusRangCombo();
	afx_msg void OnBnClickedUpbutton();
	afx_msg void OnBnClickedDownbutton();
	afx_msg void OnCbnKillfocusAmcombo();
 	/*afx_msg*/ void OnCbnSelchangeAmcombo();
	afx_msg void OnCbnSelchangeInputfuncombo();
	afx_msg void OnCbnKillfocusInputfuncombo();
	afx_msg void OnBnClickedCustombutton();
	afx_msg void OnBnKillfocusCustombutton();
	afx_msg void OnBnClickedExit();
	/*afx_msg*/ void OnEnKillfocusInvalueedit();
	/*afx_msg*/ void OnCbnKillfocusValuecombo();
	afx_msg void OnBnClickedRefreshbutton();
	/*afx_msg*/ void OnEnKillfocusInputnameedit();
	afx_msg LRESULT DealMessage(WPARAM wParam,LPARAM lParam);
	static DWORD WINAPI StartRefresh(LPVOID lpVoid);
	afx_msg void OnEnKillfocusFilter();
	void DealWithGridChanged();
	void HideAllControls();
	void ShowRangeCombox();
	HANDLE InputThread;
	HICON hIcon;
	HICON hIcon_Exit;

	int m_cvalue;
	int m_crange;
	long m_sn;

	int m_old_move_row;
	int m_old_move_col;

	COLORREF m_old_Cell_color;
public:
	//int m1_inRows;
	CEdit m_inputNameEdt;
	CComboBox m_valueCombx;
	CEdit m_inValueEdit;
	CButton m_customBtn;	
	CComboBox m_inputFinCombox;

	CComboBox	m_RangCombox;
	CMsflexgrid		m_FlexGrid;
	int					m_nModel;
	float				m_fFirmwareVersion;
	vector<CString> m_strUnitList;
	int m_nCurRow;
	int m_nCurCol;

	CMFCButton m_upButton;
	CMFCButton m_downButton;
	CComboBox m_AmCombox;
	CString		m_strCustomsensorUnit;
	BOOL   m_disable_hum ;
	BOOL   m_disable_CO2;
	//tstat6
	unsigned short tempchange[512];//for tstat6
	//tstat6
	bool b_is_fresh; 
	afx_msg void OnClose();
	CEdit m_Filter;
	UINT m_filterValue;
	afx_msg void OnCbnSelchangeValuecombo();
	afx_msg void OnBnClickedFresh();
	//void EnterCellMsflexgrid1();
	//void KeyDownMsflexgrid1(short* KeyCode, short Shift);
	void DblClickMsflexgrid1();
	void ClickMsflexgrid1();
 
	afx_msg void OnCbnSelendokRangCombo();
	afx_msg void OnCbnSelendokValuecombo();
};
