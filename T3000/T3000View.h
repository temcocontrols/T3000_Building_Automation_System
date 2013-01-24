
// T3000View.h : interface of the CT3000View class
//

#pragma once

#include "MainFrm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "msflexgrid1.h"

#include "OfficeStPtDlg.h"
#include "../FlexSlideBar/FlexSlideBarDef.h"
//#include "../FlexSlideBar/FlexSlideWnd.h"
#include "../FlexSlideBar/FSBContainer.h"

#define WM_FRESHVIEW WM_USER + 1100

class CFSBContainer;
class CT3000View : public CFormView
{
protected: // create from serialization only
	CT3000View();
	DECLARE_DYNCREATE(CT3000View)

public:
	enum{ IDD = IDD_T3000_FORM };

// Attributes
public:
	CT3000Doc* GetDocument() const;

// Operations
public:

	CString	g_strIpAdress;
	CMFCTabCtrl	m_wndTab;
	CRect m_rcTabCtrl;
	COfficeStPtDlg* m_pSetPtOffceDlg;
	
	//CEdit		m_wnd1;
	//CEdit		m_wnd2;
public:



public:
	
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	void CreateFlexSilde();
// Implementation
public:
	virtual ~CT3000View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:
////	CMFCStatusBar& GetStatusBar () const
//	{
//		return ((CMainFrame*) AfxGetMainWnd ())->GetStatusBar ();
//	}
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void OnFileOpen();

	void OnConnect();
	void OnDisconnect();
	void OnAddBuildingConfig();
	//void OnHTreeItemSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	void Fresh();

	void InitSliderBars();

	void InitSliderBars2();
	
	
	void Fresh_In();
	void Fresh_Out();

	void FreshIOGridTable();

	void InitFanSpeed();
public:
	// scan Tstats in database
	void ScanTstatInDB(void);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCoolRadio();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMReleasedcaptureDaySlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureNightSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureNightheatSlider(NMHDR *pNMHDR, LRESULT *pResult);

	HANDLE m_hSerial;//
	SOCKET m_hSocket;

	int m_nID;
	float m_fFirmwareVersion;
	UINT m_nSerialNumber;
	int m_nHardwareVersion;
	CString m_strModelName;
	float m_fTemperature;
	CComboBox m_FanComBox;
	int m_iCurFanSpeed;
	int m_nOverrideTime;
	int m_nTimeLeft;
	CString m_nstrPID1;
	CString m_nstrPID2;

	CButton m_CoolCtrl;
	CButton m_HeatCtrl;
	CButton m_OcupaCtrl;
	CButton	m_UnOcupaCtrl;
	BOOL m_bOccupied;
	CButton m_OcupiedBtn;
	CString m_strFreeCool;
	CString m_strFreeCoolFN;

	CSliderCtrl m_TemperaureSlider;
	CSliderCtrl m_daySlider;
	CSliderCtrl m_nightSlider;
	CSliderCtrl m_nightHeatSlider;

	int m_outRows;
	int m_inRows;
	CWinThread* pMainUIBackFreshThread;

	CMsflexgrid m_Output_Grid;
	CMsflexgrid m_Input_Grid;

	CTabCtrl m_SetPtTabCtrl;


// slider
	//CFlexSlideWnd m_pFswOffice;
	//CFlexSlideWnd m_pFswHotel;
	//CFSBContainer*  m_pFswOffice;
	//CFSBContainer*  m_pFswHotel;
	CFSBContainer*  m_pNightTwoSP;
	CFSBContainer*  m_pNightSingleSP;

	CFSBContainer*  m_pDayTwoSP;
	CFSBContainer*  m_pDaySingleSP;

	CEdit m_TempInfoEdit;
	CEdit m_dayInfoEdit;
	CEdit m_nightInfoEdit;
	CEdit m_nightHeatInfoEdit;

	CStatic m_SetptStatic;
	CStatic m_NightCoolStatic;
	CStatic m_NightHeatStatic;
	CEdit m_inNameEdt;
	CEdit m_outNameEdt;

	CEdit m_DayCoolEdit;
	CEdit m_DayHeatEdit;
	CStatic m_DayCoolStatic;
	CStatic m_DaySPStatic;
	CStatic m_DayHeatStatic;


	afx_msg void OnBnClickedGrapgicbutton();
	afx_msg void OnBnClickedParameterbtn();
	//	afx_msg void OnBnClickedOutputbtn();
	afx_msg void OnBnClicked5ebutton();
	DECLARE_EVENTSINK_MAP()
	void ClickInputMsflexgrid();
	void ClickOutputMsflexgrid();
	afx_msg void OnEnKillfocusInputnameedit();
	afx_msg void OnEnKillfocusOutputnameedit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedTrendlogview();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedLedbutton();
	afx_msg void OnBnClickedOccupacheck();
	afx_msg void OnCbnSelchangeFanspeedcombo();
	LRESULT OnFreshView(WPARAM wParam, LPARAM lParam);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedBtnSynctime();
	afx_msg void OnBnClickedButtonSchedule();

	afx_msg void OnBnClickedUnoccupiedMark();





	void HandleSliderSetPos(BOOL bRight);
	BOOL				m_bSliderSetPosWarning;

	afx_msg void OnBnClickedFlash();
};

#ifndef _DEBUG  // debug version in T3000View.cpp
inline CT3000Doc* CT3000View::GetDocument() const
   { return reinterpret_cast<CT3000Doc*>(m_pDocument); }
#endif

