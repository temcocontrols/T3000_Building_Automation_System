
// T3000View.h : interface of the CT3000View class
//

#pragma once

#include "MainFrm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "msflexgrid1.h"

#include "SetPtDayDlg.h"
#include "OfficeStPtDlg.h"
 

#include "singleslider/MacSliderCtrl.h"
#include "AutoRichEditCtrl.h"
#include "CM5/CStatic/staticex.h"
#include "cslidercontrol0.h"
//#include "TemcoDataGrid.h"
#define WM_FRESHVIEW WM_USER + 1100


//#define  DaySP		0  //address 345
//#define  DcoolDB	1  //address 346
//#define  DheatDB	2  //address 347
//#define  DcoolSP	3  //address 348
//#define  DheatSP	4  //address 349
//#define  NightSP	5  //address 350
//#define  NheatDB	6  //address 352
//#define  NcoolDB	7  //address 353
//#define  NheatSP	8  //address 354
//#define  NcoolSP	9  //address 355
//#define  Max		10  //address 365
//#define  Min		11  //address 366

 
class CT3000View : public CFormView
{
public: // create from serialization only
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
	CWinThread* m_pFreshBackground;
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

	void UpdateDayControls();
	void UpdateNightControls();

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
	void Fresh_T3000View();
	void FreshCtrl();

     int get_real_fan_select();
	 int set_real_fan_select();
	// 这个函数是为了与之前的版本兼容
	// 使用新的Slider，但是寄存器逻辑使用原来的
	void InitSliderBars2();

	void InitFlexSliderBars();
	void Fresh_In();
	void Fresh_Out();
	void Initial_Max_Min();
	void FreshIOGridTable();
    void FreshIOGridTable_Tstat6();
    int GetRoundMM(BOOL is_max,int num1,int num2,float rate);
	int GetRoundNumber(int number);




	void DayFeedBackNewSliderFor5ABCD();
	void NightFeedBackNewSliderFor5ABCD();
	void InitFanSpeed();
	int m_InRanges[10];
	int m_OutRanges[10];
public:
	// scan Tstats in database
	void ScanTstatInDB(void);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCoolRadio();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult); 
	 
 

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

// 	CMacSliderCtrl m_TemperaureSlider;
// 	CMacSliderCtrl m_daySlider;
// 	CMacSliderCtrl m_nightSlider;
// 	CMacSliderCtrl m_nightHeatSlider;

	int m_outRows;
	int m_inRows;
	CWinThread* pMainUIBackFreshThread;

	CMsflexgrid m_Output_Grid;
	CMsflexgrid m_Input_Grid;

	CTabCtrl m_SetPtTabCtrl;
    

 
	/*  
	 *  something about slider
	 *  1.if you want to use the slider 
  
	*/ 
	CSliderControl0  m_nightSlider;
	CSliderControl0  m_daySlider;

 

 
     
	CStatic m_SetptStatic;
	
 
	CEdit m_inNameEdt;
	CEdit m_outNameEdt;


 
 
 

    int m_new_deadband;
    int m_old_deadband;
    int m_slider_Type; 
	afx_msg void OnBnClickedGrapgicbutton();
	afx_msg void OnBnClickedParameterbtn();
	//	afx_msg void OnBnClickedOutputbtn();
	 
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
	 
 
	afx_msg void OnBnClickedOccupacheck();
	afx_msg void OnCbnSelchangeFanspeedcombo();
	LRESULT OnFreshView(WPARAM wParam, LPARAM lParam);
//	static void EnableToolTips(BOOL bEnable);
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


//tstat6
	unsigned short tempchange[512];//for tstat6
	unsigned short temptstat6[512];
//tstat6


	void HandleSliderSetPos(BOOL bRight);
	BOOL				m_bSliderSetPosWarning;
	void InitFlexSliderBars_tstat6();//tsat6
    void SetFlexSliderBars_tstat6();
 
	void TemperatureSlider();
 
    int Round_SetPoint_Max(int spvalue);
	int Round_SetPoint_Min(int spvalue);
	int m_daysetpoint;//0907
	afx_msg void OnEnKillfocusEditCurSp();
	int m_nightpot;//0907
 
	 
	 
	 
	 
	afx_msg void OnCbnSelchangeStaticunint();
	CComboBox m_gUnit;
	CMacSliderCtrl m_currentTemp;

 
   afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnBnClickedHelp();
//	afx_msg void OnBnClickedBtnTopological();
 
	 

	 afx_msg void OnBnClickedHelpHelp();
 
	 afx_msg LRESULT  ResumeMessageCallBackForT3000View(WPARAM wParam, LPARAM lParam);
	 afx_msg LRESULT  ReadDataCallBack(WPARAM wParam, LPARAM lParam);
    // BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	 int DayMax;
	 int DayMin;
    // CStaticEx m_isp;
     CToolTipCtrl m_tooltips;
     afx_msg void OnEnKillfocusDayEdit();
     
	 BOOL m_offline;
	 BOOL m_StartFirst;
     //afx_msg void OnBnClickedTestSlider();
//	 void ValuesChangedDaySlider(const VARIANT& sender, float newValue);
//	 void ValuesChangedNightSlider(const VARIANT& sender, float newValue);
	  
	  
	 void DayHandleMovedSlidercontrol1(const VARIANT& sender, float newValue);
	 void NightHandleMovedSlidercontrol1(const VARIANT& sender, float newValue);
	 /*void ClickSlidercontrol1(const VARIANT& sender, LPDISPATCH e);*/
     afx_msg void OnCbnSelchangeComboSysMode();
};

#ifndef _DEBUG  // debug version in T3000View.cpp
inline CT3000Doc* CT3000View::GetDocument() const
   { return reinterpret_cast<CT3000Doc*>(m_pDocument); }
#endif

