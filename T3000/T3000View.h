
// T3000View.h : interface of the CT3000View class
//

#pragma once

#include "MainFrm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "msflexgrid1.h"

#include "SetPtDayDlg.h"
#include "OfficeStPtDlg.h"
#include "../FlexSlideBar/FlexSlideBarDef.h"
//#include "../FlexSlideBar/FlexSlideWnd.h"
#include "../FlexSlideBar/FSBContainer.h"

#include "singleslider/MacSliderCtrl.h"
#include "TemcoDataGrid.h"
#define WM_FRESHVIEW WM_USER + 1100


#define  DaySP		0  //address 345
#define  DcoolDB	1  //address 346
#define  DheatDB	2  //address 347
#define  DcoolSP	3  //address 348
#define  DheatSP	4  //address 349
#define  NightSP	5  //address 350
#define  NheatDB	6  //address 352
#define  NcoolDB	7  //address 353
#define  NheatSP	8  //address 354
#define  NcoolSP	9  //address 355
#define  Max		10  //address 365
#define  Min		11  //address 366

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
	CSetPtDayDlg* m_pSetPtDayDlg;
	CRect m_rcTabCtrl;
	COfficeStPtDlg* m_pSetPtOffceDlg;
	
	//CEdit		m_wnd1;
	//CEdit		m_wnd2;
public:

void Read_SliderData();

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
	
	void Fresh();
	void FreshCtrl();
	
	// 这个函数是为了与之前的版本兼容
	// 使用新的Slider，但是寄存器逻辑使用原来的
	void InitSliderBars2();
	void UpdateSliderBars2();
	void InitFlexSliderBars();
	void Fresh_In();
	void Fresh_Out();

	void FreshIOGridTable();

	void OnFlexSlideCallBackFor5E();
	void OnFlexSlideCallBackFor5ABCD(CFSBContainer* pSlider);

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

	HANDLE m_hSerial;//串口句柄
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

	CMacSliderCtrl m_TemperaureSlider;
	CMacSliderCtrl m_daySlider;
	CMacSliderCtrl m_nightSlider;
	CMacSliderCtrl m_nightHeatSlider;

	int m_outRows;
	int m_inRows;
	CWinThread* pMainUIBackFreshThread;

	CTemcoDataGrid m_Output_Grid;
	CTemcoDataGrid m_Input_Grid;

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

//	CFSBContainer*  m_pTemperSP;

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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
	afx_msg LRESULT OnFlexSlideCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedUnoccupiedMark();


//tstat6
	unsigned short tempchange[512];//for tstat6
	unsigned short temptstat6[512];
//tstat6


	void HandleSliderSetPos(BOOL bRight);
	BOOL				m_bSliderSetPosWarning;
	void InitFlexSliderBars_tstat6();//tsat6
	void OnFlexSlideCallBackFortstat6();
	void TemperatureSlider();
	afx_msg void OnNMReleasedcaptureTempretureSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusDayEdit();
	int m_daysetpoint;//0907
	afx_msg void OnEnKillfocusEditCurSp();
	int m_nightpot;//0907
	//CSliderCtrl m_singlesliderday;
	CMacSliderCtrl m_singlesliderday;
	CMacSliderCtrl m_singleslidernight;
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeStaticunint();
	CComboBox m_gUnit;
	CMacSliderCtrl m_currentTemp;
	afx_msg void OnNMReleasedcaptureSliderDay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderNight(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in T3000View.cpp
inline CT3000Doc* CT3000View::GetDocument() const
   { return reinterpret_cast<CT3000Doc*>(m_pDocument); }
#endif

