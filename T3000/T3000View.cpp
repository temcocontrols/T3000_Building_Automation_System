
// T3000View.cpp : implementation of the CT3000View class
#include "stdafx.h"
#include "T3000.h"

#include "T3000Doc.h"
#include "T3000View.h"
#include "globle_function.h"
//#include "global_variable.h"
#include "global_variable_extern.h"
#include "ParameterDlg.h"

#include "OutPutDlg.h"
#include "InputSetDlg.h"
#include "OutputSetDlg.h"
#include "AfxMessageDialog.h"
#include "5EInputDialog.h"
#include "LedsDialog.h"
#include "TDelayForm.h"


#include "../FlexSlideBar/FlexSlideWnd.h"
#include "../FlexSlideBar/FSBContainer.h"


// #define _CRTDBG_MAP_ALLOC
// #include "stdlib.h"
// #include "crtdbg.h"


// #include <vector>
// using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMainFrame;

// CT3000View
//extern BOOL g_bPauseMultiRead;

IMPLEMENT_DYNCREATE(CT3000View, CFormView)
BEGIN_MESSAGE_MAP(CT3000View, CFormView)
	ON_BN_CLICKED(IDC_COOL_RADIO, &CT3000View::OnBnClickedCoolRadio)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_DAY_SLIDER, &CT3000View::OnNMReleasedcaptureDaySlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_NIGHT_SLIDER, &CT3000View::OnNMReleasedcaptureNightSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_NIGHTHEAT_SLIDER, &CT3000View::OnNMReleasedcaptureNightheatSlider)
	ON_BN_CLICKED(IDC_GRAPGICBUTTON, &CT3000View::OnBnClickedGrapgicbutton)
	ON_BN_CLICKED(IDC_PARAMETERBTN, &CT3000View::OnBnClickedParameterbtn)
//	ON_BN_CLICKED(IDC_OUTPUTBTN, &CT3000View::OnBnClickedOutputbtn)
	ON_BN_CLICKED(IDC_5EBUTTON, &CT3000View::OnBnClicked5ebutton)
	ON_EN_KILLFOCUS(IDC_INPUTNAMEEDIT, &CT3000View::OnEnKillfocusInputnameedit)
	ON_EN_KILLFOCUS(IDC_OUTPUTNAMEEDIT, &CT3000View::OnEnKillfocusOutputnameedit)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TRENDLOGVIEW, &CT3000View::OnBnClickedTrendlogview)
	ON_BN_CLICKED(IDC_BUTTON1, &CT3000View::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_LEDBUTTON, &CT3000View::OnBnClickedLedbutton)
	ON_BN_CLICKED(IDC_OCCUPACHECK, &CT3000View::OnBnClickedOccupacheck)
	ON_CBN_SELCHANGE(IDC_FANSPEEDCOMBO, &CT3000View::OnCbnSelchangeFanspeedcombo)

	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_MOVING()
	ON_MESSAGE(WM_FRESHVIEW, OnFreshView)
	ON_BN_CLICKED(IDC_BTN_SYNCTIME, &CT3000View::OnBnClickedBtnSynctime)
	ON_BN_CLICKED(IDC_BUTTON_SCHEDULE, &CT3000View::OnBnClickedButtonSchedule)

	ON_BN_CLICKED(IDC_UNOCCUPIED_MARK, &CT3000View::OnBnClickedUnoccupiedMark)
	ON_BN_CLICKED(IDC_FLASH, &CT3000View::OnBnClickedFlash)
END_MESSAGE_MAP()

// CT3000View construction/destruction
UINT BackMainUIFresh(LPVOID pParam)
{
	CT3000View* pdlg = (CT3000View*)pParam;
	while(1)
	{
		Sleep(15*1000);
		if(pdlg->IsWindowVisible())
		{
			pdlg->Fresh();
		}
	}
	return 0;
}

CT3000View::CT3000View()
	: CFormView(CT3000View::IDD)
	, m_nID(0)
	, m_fFirmwareVersion(0)
	, m_nSerialNumber(0)
	, m_nHardwareVersion(0)
	, m_strModelName(_T(""))
	, m_fTemperature(0)
	, m_iCurFanSpeed(0)
	, m_nOverrideTime(0)
	, m_nTimeLeft(0)
	, m_bOccupied(FALSE)
	, m_strFreeCool(_T(""))
	, m_strFreeCoolFN(_T(""))
{
	// TODO: add construction code here
	m_hSerial=NULL;
	m_hSocket=NULL;
	m_outRows=1;
	m_inRows=1;
	m_pSetPtOffceDlg=NULL;
	pMainUIBackFreshThread=NULL;

	m_pDaySingleSP = NULL;
	m_pDayTwoSP= NULL;
	m_pNightSingleSP = NULL;
	m_pNightTwoSP= NULL;

	m_rcTabCtrl.CopyRect(CRect(620,500,850,800));//runningtime
	m_bSliderSetPosWarning = TRUE; // permit warning
}

CT3000View::~CT3000View()
{


	if (m_pNightSingleSP)
	{
		delete m_pNightSingleSP;
		m_pNightSingleSP = NULL;
	}

	if (m_pNightTwoSP)
	{
		delete m_pNightTwoSP;
		m_pNightTwoSP = NULL;
	}

	if (m_pDaySingleSP)
	{
		delete m_pDaySingleSP;
		m_pDaySingleSP = NULL;
	}

	if (m_pDayTwoSP)
	{
		delete m_pDayTwoSP;
		m_pDayTwoSP = NULL;
	}
		
}

void CT3000View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//	DDX_Text(pDX, IDC_ID_EDIT, m_strIDAddress);
	DDX_Text(pDX, IDC_ID_EDIT, m_nID);
	DDX_Text(pDX, IDC_FIRMWARV_EDIT, m_fFirmwareVersion);
	DDX_Text(pDX, IDC_SERIALNUM_EDIT, m_nSerialNumber);
	DDX_Text(pDX, IDC_HARDWAREV_EDIT, m_nHardwareVersion);
	DDX_Text(pDX, IDC_MODEL_EDIT, m_strModelName);
	DDX_Text(pDX, IDC_TEMPERATURE_EDIT, m_fTemperature);
	DDX_Control(pDX, IDC_FANSPEEDCOMBO, m_FanComBox);
	DDX_CBIndex(pDX, IDC_FANSPEEDCOMBO, m_iCurFanSpeed);
	DDX_Text(pDX, IDC_OVERIDETIME_EDIT, m_nOverrideTime);
	DDX_Text(pDX, IDC_TIMELEFT_EDIT, m_nTimeLeft);
	DDX_Text(pDX, IDC_PID1_EDIT, m_nstrPID1);
	DDX_Text(pDX, IDC_PID2_EDIT, m_nstrPID2);
	DDX_Control(pDX, IDC_COOL_RADIO, m_CoolCtrl);
	DDX_Control(pDX, IDC_HEAT_RADIO, m_HeatCtrl);

 
	DDX_Check(pDX, IDC_OCCUPACHECK, m_bOccupied);
	DDX_Control(pDX, IDC_OCCUPACHECK, m_OcupiedBtn);
	DDX_Text(pDX, IDC_FREECOOLA_EDIT, m_strFreeCool);
	DDX_Text(pDX, IDC_FREECOOLF_EDIT, m_strFreeCoolFN);

	DDX_Control(pDX, IDC_TEMPRETURE_SLIDER, m_TemperaureSlider);
	DDX_Control(pDX, IDC_DAY_SLIDER, m_daySlider);
	DDX_Control(pDX, IDC_NIGHT_SLIDER, m_nightSlider);
	DDX_Control(pDX, IDC_NIGHTHEAT_SLIDER, m_nightHeatSlider);

	DDX_Control(pDX, IDC_TEMPINFO_EDIT, m_TempInfoEdit);
	DDX_Control(pDX, IDC_DAY_EDIT, m_dayInfoEdit);
	DDX_Control(pDX, IDC_NIGHT_EDIT, m_nightInfoEdit);
	DDX_Control(pDX, IDC_NIGHTHEAT_EDIT, m_nightHeatInfoEdit);
	DDX_Control(pDX, IDC_OUTPUT_MSFLEXGRID, m_Output_Grid);
	DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_Input_Grid);
	DDX_Control(pDX, IDC_INPUTNAMEEDIT, m_inNameEdt);
	DDX_Control(pDX, IDC_OUTPUTNAMEEDIT, m_outNameEdt);
	DDX_Control(pDX, IDC_SETPTSTATIC, m_SetptStatic);
	DDX_Control(pDX, IDC_NCSTATIC, m_NightCoolStatic);
	DDX_Control(pDX, IDC_NHSTATIC, m_NightHeatStatic);

	//DDX_Control(pDX, IDC_NCSTATIC, m_NightCoolStatic);
	//DDX_Control(pDX, IDC_NHSTATIC, m_NightHeatStatic);

	DDX_Control(pDX, IDC_EDIT_DAYCOOL, m_DayCoolEdit);
	DDX_Control(pDX, IDC_EDIT_DAYHEAT, m_DayHeatEdit);
	DDX_Control(pDX, IDC_STATIC_DAYCOOL, m_DayCoolStatic);
	DDX_Control(pDX, IDC_STATIC_DATSP, m_DaySPStatic);
	DDX_Control(pDX, IDC_STATIC_DAYHEAT, m_DayHeatStatic);
}


BOOL CT3000View::PreCreateWindow(CREATESTRUCT& cs)
{																	
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void CT3000View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDlgItem(IDC_5EBUTTON)->ShowWindow(SW_HIDE);
	m_inNameEdt.ShowWindow(SW_HIDE); 
	m_outNameEdt.ShowWindow(SW_HIDE);  

	if(g_MainScreenLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
	}

//	GetDlgItem(IDC_OVERIDETIME_EDIT)->EnableWindow(FALSE);
	m_Output_Grid.put_Cols(3);
	m_Input_Grid.put_Cols(3);

	m_Input_Grid.put_ColWidth(0,400);
	m_Input_Grid.put_ColWidth(1,1300);
	m_Input_Grid.put_ColWidth(2,1200);
	m_Input_Grid.put_TextMatrix(0,1,_T("Input Name"));	
	m_Input_Grid.put_TextMatrix(0,2,_T("Input Value"));
		
	m_Output_Grid.put_ColWidth(0,400);
	m_Output_Grid.put_ColWidth(1,1300);
	m_Output_Grid.put_ColWidth(2,1200);
	m_Output_Grid.put_TextMatrix(0,1,_T("Output Name"));
	m_Output_Grid.put_TextMatrix(0,2,_T("Output Value"));




	SetTimer(1,1000,NULL);

	// Create flex slide bar
	//return;
	CreateFlexSilde();

}

void CT3000View::CreateFlexSilde()
{
	if(m_pNightTwoSP == NULL)
	{
		CStatic *s = (CStatic*)GetDlgItem(IDC_FSB_OFFICE);
		CRect rc;
		s->GetWindowRect(&rc);
		ScreenToClient(&rc);

		m_pNightTwoSP=new CFSBContainer;
		m_pNightTwoSP->SetParentWnd(this);


		//////////////////////////////////////////////////////////////////////////

		
		m_pNightTwoSP->CreateInstance(s, s->GetStyle()|s->GetExStyle(), rc);

		m_pNightTwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB);
		m_pNightTwoSP->SetFlexSlideBarRect(rc);
		m_pNightTwoSP->SetHorizontal(FALSE);
		m_pNightTwoSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
		m_pNightTwoSP->SetChannelColor(RGB(192,192,192));
		m_pNightTwoSP->SetThumbColor(RGB(255,128,128));
		m_pNightTwoSP->SetPageLength(5);
		m_pNightTwoSP->SetChannelWidth(10);
		m_pNightTwoSP->SetThumbWidth(24);

		m_pNightTwoSP->SetRange(10, 0);
		m_pNightTwoSP->SetPos(0, 1, 2);	
		// init two thumb slider

//*
		CStatic *s1 = (CStatic*)GetDlgItem(IDC_FSB_HOTEL);
		m_pNightSingleSP= new CFSBContainer;
		//m_pFswHotel->SetParentWnd(this);
		//nRet = m_pNightSingleSP->SubclassWindow(s1->GetSafeHwnd());
		//nRet = m_pNightSingleSP->SubclassDlgItem(IDC_FSB_HOTEL, this);

		s1->GetWindowRect(&rc);
		ScreenToClient(&rc);

		//m_pFswHotel->Create( NULL, s1->GetStyle()|s1->GetExStyle()|SS_SUNKEN, rc, this, 10001);
		m_pNightSingleSP->CreateInstance(s1, s1->GetStyle()|s1->GetExStyle()|SS_SUNKEN, rc);
		m_pNightSingleSP->SetParentWnd(this);

		m_pNightSingleSP->CreateFlexSlideBar(FALSE, FSB_STYLE_TRIPLELINKAGE );	
		m_pNightSingleSP->SetFlexSlideBarRect(rc);
		m_pNightSingleSP->SetHorizontal(FALSE);
		m_pNightSingleSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
		m_pNightSingleSP->SetChannelColor(RGB(192,192,192));
		m_pNightSingleSP->SetThumbColor(RGB(255,128,128));
		m_pNightSingleSP->SetPageLength(5);
		m_pNightSingleSP->SetChannelWidth(10);
		m_pNightSingleSP->SetThumbWidth(24);
		
		m_pNightSingleSP->SetRange(10, 0);
		m_pNightSingleSP->SetPos(0, 1, 2);	
	
		//////////////////////////////////////////////////////////////////////////
		// Day

		s = (CStatic*)GetDlgItem(IDC_FSB_DAYOFFICE);
		rc;
		s->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_pDayTwoSP=new CFSBContainer;
		m_pDayTwoSP->SetParentWnd(this);

		m_pDayTwoSP->CreateInstance(s, s->GetStyle()|s->GetExStyle(), rc);
	
		m_pDayTwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB);
		m_pDayTwoSP->SetFlexSlideBarRect(rc);
		m_pDayTwoSP->SetHorizontal(FALSE);
		m_pDayTwoSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
		m_pDayTwoSP->SetChannelColor(RGB(192,192,192));
		m_pDayTwoSP->SetThumbColor(RGB(255,128,128));
		m_pDayTwoSP->SetPageLength(5);
		m_pDayTwoSP->SetChannelWidth(10);
		m_pDayTwoSP->SetThumbWidth(24);

		m_pDayTwoSP->SetRange(10, 0);
		m_pDayTwoSP->SetPos(0, 1, 2);	
		
		//--------------------------------Day hotel------------------------------------

		s1 = (CStatic*)GetDlgItem(IDC_FSB_DAYHOTEL);
		m_pDaySingleSP= new CFSBContainer;
		s1->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_pDaySingleSP->CreateInstance(s1, s1->GetStyle()|s1->GetExStyle()|SS_SUNKEN, rc);
		m_pDaySingleSP->SetParentWnd(this);

		m_pDaySingleSP->CreateFlexSlideBar(FALSE, FSB_STYLE_TRIPLELINKAGE );	
		m_pDaySingleSP->SetFlexSlideBarRect(rc);
		m_pDaySingleSP->SetHorizontal(FALSE);
		m_pDaySingleSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
		m_pDaySingleSP->SetChannelColor(RGB(192,192,192));
		m_pDaySingleSP->SetThumbColor(RGB(255,128,128));
		m_pDaySingleSP->SetPageLength(5);
		m_pDaySingleSP->SetChannelWidth(10);
		m_pDaySingleSP->SetThumbWidth(24);

		m_pDaySingleSP->SetRange(10, 0);
		m_pDaySingleSP->SetPos(0, 1, 2);	
	}
}

void CT3000View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CT3000View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}
// CT3000View diagnostics
#ifdef _DEBUG
void CT3000View::AssertValid() const
{
	CFormView::AssertValid();
}

void CT3000View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CT3000Doc* CT3000View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CT3000Doc)));
	return (CT3000Doc*)m_pDocument;
}
#endif //_DEBUG


void CT3000View::OnFileOpen()
{
	// TODO: Add your command handler code here
	
}

void CT3000View::Fresh()
{

//	121	1	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1

	if (multi_register_value[121] == 1)
	{
		g_unint = FALSE;
		GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("°F"));
	}else
	{
		g_unint = TRUE;
		GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("°C"));
	}
		
	m_nID=g_tstat_id;
	m_fFirmwareVersion=get_curtstat_version();
	m_nSerialNumber=get_serialnumber();
	m_nHardwareVersion=multi_register_value[MODBUS_HARDWARE_REV];
	int nModel=multi_register_value[MODBUS_PRODUCT_MODEL];


	switch (nModel)
	{
		case 2:m_strModelName=g_strTstat5a;break;
		case 1:m_strModelName=g_strTstat5b;break;
		case 3:m_strModelName=g_strTstat5b;break;
		case 4:m_strModelName=g_strTstat5c;break;
		case 12:m_strModelName=g_strTstat5d;break;
		case PM_NC:m_strModelName=g_strnetWork;break;
	
		case PM_TSTAT5E:m_strModelName=g_strTstat5e;break;
		case 17: m_strModelName=g_strTstat5f;break;
		case 18:m_strModelName=g_strTstat5g;break;
		case 19:m_strModelName=g_strTstat5h;break;

		case PM_TSTAT6:m_strModelName=g_strTstat6;break;
		case PM_TSTAT7:m_strModelName=g_strTstat7;break;
		case PM_PRESSURE:m_strModelName=g_strPressure;break;

		default:m_strModelName=g_strTstat5a;break;

	}
	m_fTemperature=(float)multi_register_value[101]/10;
	m_iCurFanSpeed=multi_register_value[137];
	m_nOverrideTime=multi_register_value[211];
	m_nTimeLeft=multi_register_value[212];

	switch(multi_register_value[107])
	{
			case 0:	m_nstrPID1=_T("COASTING");break;
			case 1: m_nstrPID1=_T("COOLING 1");break;
			case 2: m_nstrPID1=_T("COOLING 2");break;
			case 3: m_nstrPID1=_T("COOLING 3");break;
			case 4: m_nstrPID1=_T("HEATING 1");break;
			case 5: m_nstrPID1=_T("HEATING 2");break;
			case 6: m_nstrPID1=_T("HEATING 3");break;

			case 14: m_nstrPID1=_T("COOLING 4");break;
			case 15: m_nstrPID1=_T("COOLING 5");break;
			case 16: m_nstrPID1=_T("COOLING 6");break;
			case 17: m_nstrPID1=_T("HEATING 4");break;
			case 18: m_nstrPID1=_T("HEATING 5");break;
			case 19: m_nstrPID1=_T("HEATING 6");break;
			default : m_nstrPID1=_T("COASTING");break;
		}
	switch(multi_register_value[278])
		{
			case 0:	m_nstrPID2=_T("COASTING");break;
			case 1: m_nstrPID2=_T("COOLING 1");break;
			case 2: m_nstrPID2=_T("COOLING 2");break;
			case 3: m_nstrPID2=_T("COOLING 3");break;
			case 4: m_nstrPID2=_T("HEATING 1");break;
			case 5: m_nstrPID2=_T("HEATING 2");break;
			case 6: m_nstrPID2=_T("HEATING 3");break;

			case 14: m_nstrPID2=_T("COOLING 4");break;
			case 15: m_nstrPID2=_T("COOLING 5");break;
			case 16: m_nstrPID2=_T("COOLING 6");break;
			case 17: m_nstrPID2=_T("HEATING 4");break;
			case 18: m_nstrPID2=_T("HEATING 5");break;
			case 19: m_nstrPID2=_T("HEATING 6");break;
			default : m_nstrPID2=_T("COASTING");break;
		}

		if (multi_register_value[106]==1)
		{
			//m_bcooling=TRUE;
			m_CoolCtrl.SetCheck(BST_CHECKED);
			m_HeatCtrl.SetCheck(BST_UNCHECKED);
		}
		if(multi_register_value[106]==2)
		{
		//	m_bcooling=FALSE;
			m_CoolCtrl.SetCheck(BST_UNCHECKED);
			m_HeatCtrl.SetCheck(BST_CHECKED);
		}
		switch(multi_register_value[128])
		{
			case 2:	m_CoolCtrl.EnableWindow();m_HeatCtrl.EnableWindow();break;
			case 4:	m_CoolCtrl.EnableWindow();m_HeatCtrl.EnableWindow();break;
			case 5:	m_CoolCtrl.EnableWindow();m_HeatCtrl.EnableWindow();break;
			default:m_CoolCtrl.EnableWindow(false);m_HeatCtrl.EnableWindow(false);break;
		}	
		if(multi_register_value[184] & 1)
		{
			m_bOccupied=TRUE;
			CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);
			pBtn->SetCheck(0);
		}
		else
		{
			m_bOccupied=FALSE;
			CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);
			pBtn->SetCheck(1);
		}


//Free Cool available

		if(multi_register_value[350]&0x4)//Free Cool available,yes or on
		{
			m_strFreeCool=_T("Yes");
		}
		else
		{
			m_strFreeCool=_T("No");
		}
// for fan speed
		InitFanSpeed();

//
		if(multi_register_value[350]&0x10)//outside air temperature from NC, if the NC has an active OAT then OAT=OK, else OAT=FAIL .
		{
			m_strFreeCoolFN=_T("OK");//shoud also show the oat on the user's display

		}
		else
		{
			m_strFreeCoolFN=_T("FAIL"); //show a grey box for the OAT
		}

		if(multi_register_value[350]&0x2)
		{
		}
		else
		{
			m_strFreeCoolFN=_T("");
		}

	//////////////////////////////////////////////////////////////////////////
	// just for tstat6

	if(multi_register_value[7] == PM_TSTAT6)
	{	
		// humidity
		WORD wHum = multi_register_value[422];
		CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HUMIDITY);
		CString str;
		str.Format(_T("%0.1f%%"), wHum/10.0);
		pEdit->SetWindowText(str);
	}	
	else
	{GetDlgItem(IDC_EDIT_HUMIDITY)->SetWindowText(_T("N/A"));	}
	
	//time
	CTime time = CTime::GetCurrentTime();

	CString strtime = time.Format(_T("%I:%M:%S %p"));
	//CString strtime = time.Format(_T("%m/%d/%Y %H:%M:%S %a"));
	CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC_TIME);
	pEdit->SetWindowText(strtime);
	CString strDate = time.Format(_T("%A, %b %d, %Y"));
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC);
	pEdit->SetWindowText(strDate);
	
	


			InitSliderBars2();//



	
			
	UpdateData(FALSE);

	FreshIOGridTable();
}

void CT3000View::OnBnClickedCoolRadio()
{
	// TODO: Add your control notification handler code here
}

void CT3000View::InitSliderBars()
{
#if 1
	
	CString strTemp;
	int min=multi_register_value[132];
	int max=multi_register_value[131];
	m_TemperaureSlider.SetRange(min,max);
	m_daySlider.SetRange(min,max);
	m_nightSlider.SetRange(min,max);
	m_nightHeatSlider.SetRange(min,max);

	m_TemperaureSlider.SetPos((int)(max-m_fTemperature+min));
	if (g_unint)
		strTemp.Format(_T("%.1f°C"),m_fTemperature);
	else
		strTemp.Format(_T("%.1f°F"),m_fTemperature);

	m_TempInfoEdit.SetWindowText(strTemp);

	float itemp;
	if( multi_register_value[7] == PM_TSTAT7)
	{
		itemp=(float)(multi_register_value[380]/10.0);
	}
	else if(m_fFirmwareVersion<34.9 || multi_register_value[7] == PM_TSTAT5E || multi_register_value[7] == PM_TSTAT6)
	{
		itemp=(float)(multi_register_value[135]);
	}
	else
	{
		itemp=(float)(multi_register_value[374]/10.0);
	}
	m_daySlider.SetPos(int(max-itemp+min));

	strTemp.Format(_T("%.1f"),itemp);
	m_dayInfoEdit.SetWindowText(strTemp);

	int m_hotel_or_office=multi_register_value[125];///////hotel or office
	int the_setpoint_ha_ha=(int)itemp;// copy from old code vc2003.
	/*
	//m_TemperaureSlider.SetRange(20,100);
//	m_TemperaureSlider.SetPos(30);
//	m_nightSlider.SetPos(max-(itemp-min)+0.5);
//	m_nightHeatSlider.SetPos(max-(itemp-min)+0.5);
*/


	if(m_hotel_or_office==0)  // office
	{
		itemp=multi_register_value[183];
		m_nightSlider.SetPos((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),itemp);
		m_nightInfoEdit.SetWindowText(strTemp);

		itemp=multi_register_value[182];//heat
		m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),itemp);
		m_nightHeatInfoEdit.SetWindowText(strTemp);
		m_NightCoolStatic.SetWindowText(_T("Cooling"));
		m_NightHeatStatic.SetWindowText(_T("Heating"));
		//;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));

	}
	
	if(m_hotel_or_office==1)
	{
		itemp=(float)(multi_register_value[124]+the_setpoint_ha_ha);//cool
		m_nightSlider.SetPos(int(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),(float)multi_register_value[124]);
		m_nightInfoEdit.SetWindowText(strTemp);

		itemp=(float)(the_setpoint_ha_ha-multi_register_value[123]);//heat
		m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),(float)multi_register_value[123]);
		m_nightHeatInfoEdit.SetWindowText(strTemp);

		m_NightCoolStatic.SetWindowText(_T("CoolDB"));
		m_NightHeatStatic.SetWindowText(_T("HeatDB"));
		//GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Hotel"));

	}

	/*
	if(m_pSetPtDayDlg!=NULL)
	{
		m_pSetPtDayDlg->Invalidate();
	}
	*/

#endif
}



void CT3000View::InitSliderBars2()
{





#if 1
	CString strTemp;
	int min=multi_register_value[132];
	int max=multi_register_value[131];
	m_TemperaureSlider.SetRange(min,max);



	//m_nightSlider.SetRange(min,max);
	//m_nightHeatSlider.SetRange(min,max);
	m_pNightSingleSP->SetRange(max, min);


	m_TemperaureSlider.SetPos((int)(max-m_fTemperature+min));
	if (g_unint)
		strTemp.Format(_T("%.1f°C"),m_fTemperature);
	else
		strTemp.Format(_T("%.1f°F"),m_fTemperature);
	m_TempInfoEdit.SetWindowText(strTemp);

	float itemp;
	if(multi_register_value[7] == PM_TSTAT5E)
	{
		itemp=(float)(multi_register_value[135]);
	}
	else
	{
		itemp=(float)(multi_register_value[374]/10.0);
	}

	strTemp.Format(_T("%.1f"),itemp);
	m_dayInfoEdit.SetWindowText(strTemp);
	m_daySlider.SetRange(min,max);
	m_daySlider.SetPos(int(max-itemp+min));
	//m_daySlider.ShowWindow(SW_NORMAL);
	int nSP =(int)itemp;//(max-itemp+min);

	//////////////////////////////////////////////////////////////////////////
	// 	strTemp = _T("Single SetPoint Mode");
	// 	CWnd *pWnd = GetDlgItem(IDC_STATIC_DAYSPMODE);
	// 	pWnd->SetWindowText(strTemp);
	// 	
	// 	int nCoolSP = multi_register_value[135];
	// 	strTemp.Format(_T("%.1f"),nCoolSP);
	// 	m_DayCoolEdit.SetWindowText(strTemp);
	// 
	// 	int nHeatSP = multi_register_value[136];
	// 	strTemp.Format(_T("%.1f"),nHeatSP);
	// 	m_DayHeatEdit.SetWindowText(strTemp);
	// 
	// 	m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
	// 	m_DayHeatStatic.SetWindowText(_T("Heating SP"));
	// 	
	// 	if (nCoolSP == nHeatSP && nCoolSP== nSP )
	// 	{
	// 		nSP = nHeatSP + 1;
	// 		nCoolSP = nSP+1;				
	// 	}
	// 
	// 	int nRangeMax = nCoolSP > max ? nCoolSP : max;
	// 	int nRangeMin = nHeatSP < min ? nHeatSP : min;
	// 
	// 	m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);
	// 	m_pDaySingleSP->SetPos(nHeatSP, nSP, nCoolSP);		


	//////////////////////////////////////////////////////////////////////////
	//m_pDaySingleSP->ShowWindow(SW_HIDE);
	//m_pDayTwoSP->ShowWindow(SW_HIDE);

	//////////////////////////////////////////////////////////////////////////
	// 	CString strTemp1 = _T("Single SetPoint Mode");
	// 	CWnd *pWndMode1 = GetDlgItem(IDC_STATIC_SPMODE2);
	// 	pWndMode1->SetWindowText(strTemp1);
	//////////////////////////////////////////////////////////////////////////

	short nOccupied = multi_register_value[184];  // Day setpoint option  
	BOOL bOccupied = nOccupied & 0x0001;

	int nOfficeSelReg=454;
	if (bOccupied)
	{	
		if(multi_register_value[7]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
		{
			nOfficeSelReg = 423;
		}
	}
	else
	{
		nOfficeSelReg = 125;
	}
	int m_hotel_or_office=multi_register_value[nOfficeSelReg];///////hotel or office
	int the_setpoint_ha_ha=(int)itemp;		// c

	//////////////////////////////////////////////////////////////////////////
	if(m_hotel_or_office==0  && bOccupied)  //
	{  // 2sp
		CString strTemp1 = _T("Two SetPoint Mode");
		CWnd *pWndMode1 = GetDlgItem(IDC_STATIC_SPMODE2);
		pWndMode1->SetWindowText(strTemp1);

		//nSP = multi_register_value[374];//(nCoolSP-nHeatSP)/2 + nHeatSP;
		itemp=multi_register_value[119];
		//m_nightSlider.SetPos((int)(max-(itemp-min)));
		int nCoolDB = (int)(itemp/10);//(int)(max-(itemp-min));

		itemp=multi_register_value[120];//heat
		//m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		int nHeatDB = (int)(itemp/10);//((int)(max-(itemp-min)));


		m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
		m_DayHeatStatic.SetWindowText(_T("Heating SP"));
		//;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));

		int nCoolSP = (nCoolDB + nSP); 
		int nHeatSP =(nSP - nHeatDB);
		strTemp.Format(_T("%0.1f"), nSP);
		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
		pSPWnd->SetWindowText(strTemp);

		strTemp.Format(_T("%.1f"),(float)nCoolSP);
		m_DayCoolEdit.SetWindowText(strTemp);
		strTemp.Format(_T("%.1f"),(float)nHeatSP);
		m_DayHeatEdit.SetWindowText(strTemp);

		//nSP= int(nSP/10.0);
		if (nCoolSP == nHeatSP && nCoolSP== nSP )
		{
			nSP = nHeatSP + 1;
			nCoolSP = nSP+1;				
		}

		int nRangeMax = nCoolSP > max ? nCoolSP : max;
		int nRangeMin = nHeatSP < min ? nHeatSP : min;

		 nRangeMax = 100;
		 nRangeMin = 0;
		m_pDayTwoSP->SetRange(nRangeMax, nRangeMin);
		BOOL bRetSP = m_pDayTwoSP->SetPos(10, 30, 50); 
		HandleSliderSetPos(bRetSP);

		m_pDayTwoSP->ShowWindow(SW_NORMAL);
		m_pDaySingleSP->ShowWindow(SW_HIDE);
	}

	if(m_hotel_or_office==1 && bOccupied)  // hotel - occupied
	{ // 1SP
		CString strTemp1 = _T("Single SetPoint Mode");
		CWnd *pWndMode1 = GetDlgItem(IDC_STATIC_SPMODE2);
		pWndMode1->SetWindowText(strTemp1);

		itemp=multi_register_value[422];
		//m_nightSlider.SetPos((int)(max-(itemp-min)));
		int nCoolSP = itemp/10.0;//(int)(max-(itemp-min));
		strTemp.Format(_T("%.1f"),float(nCoolSP-nSP));
		m_DayCoolEdit.SetWindowText(strTemp);

		itemp=multi_register_value[136];//heat
		//m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		int nHeatSP = itemp;//((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),float(nSP-nHeatSP));
		m_DayHeatEdit.SetWindowText(strTemp);

		m_DayCoolStatic.SetWindowText(_T("Cooling DB"));
		m_DayHeatStatic.SetWindowText(_T("Heating DB"));
		//;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));
		//nSP = multi_register_value[135];//(nCoolSP-nHeatSP)/2 + nHeatSP;
		strTemp.Format(_T("%d"), multi_register_value[135]/10.0);
		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
		pSPWnd->SetWindowText(strTemp);

		if (nCoolSP == nHeatSP && nCoolSP== nSP )
		{
			nSP = nHeatSP + 1;
			nCoolSP = nSP+1;				
		}

		int nRangeMax = nCoolSP > max ? nCoolSP : max;
		int nRangeMin = nHeatSP < min ? nHeatSP : min;

		 nRangeMax = 100;
		 nRangeMin = 0;

		m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);
		BOOL bRetSP = m_pDaySingleSP->SetPos(10, 20, 80);		
		HandleSliderSetPos(bRetSP);
		m_pDayTwoSP->ShowWindow(SW_HIDE);
		m_pDaySingleSP->ShowWindow(SW_NORMAL);	
	}


	//////////////////////////////////////////////////////////////////////////
	if(m_hotel_or_office==0  && !bOccupied)  // office - unoccupied ,2sp
	{ // 2sp
		strTemp = _T("Two SetPoint Mode");
		CWnd *pModeWnd = GetDlgItem(IDC_STATIC_SPMODE);
		pModeWnd->SetWindowText(strTemp);

		itemp=multi_register_value[183];
		//m_nightSlider.SetPos((int)(max-(itemp-min)));
		int nCoolSP = (int)itemp;//(int)(max-(itemp-min));
		strTemp.Format(_T("%.1f"),itemp);
		m_nightInfoEdit.SetWindowText(strTemp);

		itemp=multi_register_value[182];//heat
		//m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		int nHeatSP = (int)itemp;//((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),itemp);
		m_nightHeatInfoEdit.SetWindowText(strTemp);
		m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
		m_NightHeatStatic.SetWindowText(_T("Heating SP"));
		//;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));
		nSP = (nCoolSP-nHeatSP)/2 + nHeatSP;
		strTemp.Format(_T("%d"), nSP);
		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
		pSPWnd->SetWindowText(strTemp);


		if (nCoolSP == nHeatSP && nCoolSP== nSP )
		{
			nSP = nHeatSP + 1;
			nCoolSP = nSP+1;				
		}

		int nRangeMax = nCoolSP > max ? nCoolSP : max;
		int nRangeMin = nHeatSP < min ? nHeatSP : min;


		 nRangeMax =100;
		 nRangeMin = 0;
		m_pNightTwoSP->SetRange(nRangeMax, nRangeMin);
		BOOL bSetPos = m_pNightTwoSP->SetPos(10, 30, 50);		
		HandleSliderSetPos(bSetPos);

		m_pNightTwoSP->ShowWindow(SW_NORMAL);
		m_pNightSingleSP->ShowWindow(SW_HIDE);

	}

	if(m_hotel_or_office==1  && !bOccupied)  // unoccupied-hotel
	{ // 1sp
		//strTemp = _T("Hotel");
		strTemp = _T("Single SetPoint Mode");
		CWnd *pModeWnd = GetDlgItem(IDC_STATIC_SPMODE);
		pModeWnd->SetWindowText(strTemp);
		nSP = multi_register_value[135]; // 135
		//int nCoolSP, nHeatSP;
		//itemp=multi_register_value[124]+the_setpoint_ha_ha;//cool
		//m_nightSlider.SetPos(max-(itemp-min));
		int nCoolDB = multi_register_value[124]/10;//(max-(itemp-min));
		strTemp.Format(_T("%.1f"),(float)multi_register_value[124]/10);
		m_nightInfoEdit.SetWindowText(strTemp);



		//itemp=the_setpoint_ha_ha-multi_register_value[123];//heat
		//m_nightHeatSlider.SetPos(max-(itemp-min));
		int nHeatDB = multi_register_value[123]/10;//(max-(itemp-min));
		strTemp.Format(_T("%.1f"),(float)multi_register_value[123]/10);
		m_nightHeatInfoEdit.SetWindowText(strTemp);

		strTemp.Format(_T("%d"), nSP);
		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
		pSPWnd->SetWindowText(strTemp);

		m_NightCoolStatic.SetWindowText(_T("Cooling DB"));
		m_NightHeatStatic.SetWindowText(_T("Heating DB"));
		//GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Hotel"));
		int nCoolSP = nSP + nCoolDB;
		int nHeatSP = nSP - nHeatDB;		
		if (nCoolSP == nHeatSP && nHeatSP == 0)
		{
			nHeatSP = nSP - 1;
			nCoolSP = nSP+1;	
		}

		int nRangeMax = nCoolSP > max ? nCoolSP : max;
		int nRangeMin = nHeatSP < min ? nHeatSP : min;

		 nRangeMax = 100;
		 nRangeMin = 0;

		m_pNightSingleSP->SetRange(nRangeMax, nRangeMin);
		BOOL bRetSP = m_pNightSingleSP->SetPos(10, 30, 50);		
		HandleSliderSetPos(bRetSP);
		m_pNightSingleSP->ShowWindow(SW_NORMAL);
		m_pNightTwoSP->ShowWindow(SW_HIDE);

	}

	/*
	if(m_pSetPtDayDlg!=NULL)
	{
	m_pSetPtDayDlg->Invalidate();
	}
	*/


#endif

}



void CT3000View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CT3000View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CT3000View::OnNMReleasedcaptureDaySlider(NMHDR *pNMHDR, LRESULT *pResult)
{
#if 1
	// TODO: Add your control notification handler code here
	int min,max;
	m_daySlider.GetRange(min,max);
	int nCurDay=max-m_daySlider.GetPos()+min;



	m_daySlider.SetPos(max-nCurDay+min);
		
	CString strTemp;
	strTemp.Format(_T("%d"),nCurDay);
	m_dayInfoEdit.SetWindowText(strTemp);

// 	if(m_fFirmwareVersion<34.9 || )
// 		write_one(g_tstat_id,135,nCurDay);	//write setpoint ,for TStat5E
// 	else
// 	{
// 		write_one(g_tstat_id,374,nCurDay*10);	//write setpoint
// 	}

	//////////////////////////////////////////////////////////////////////////
	if (multi_register_value[7] == PM_TSTAT7)
	{
		write_one(g_tstat_id,380,nCurDay*10);
	}
	else if(m_fFirmwareVersion<34.9 || multi_register_value[7] == PM_TSTAT5E)
	{
		write_one(g_tstat_id,135,nCurDay);
	}
	else 
	{
		write_one(g_tstat_id,374,nCurDay*10);
	}

	InitSliderBars();


	*pResult = 0;
#endif
}

void CT3000View::OnNMReleasedcaptureNightSlider(NMHDR *pNMHDR, LRESULT *pResult)
{

#if 1
	int min,max;
	m_nightSlider.GetRange(min,max);
	int nCurDay=max-m_daySlider.GetPos()+min;
	int nCurNight=max-m_nightSlider.GetPos()+min;
	int hotel_or_office=multi_register_value[125];



	m_nightSlider.SetPos(max-nCurNight+min);
	CString strTemp;
	strTemp.Format(_T("%d"),nCurNight);
	m_nightInfoEdit.SetWindowText(strTemp);
	
	if(hotel_or_office==0)
	{//setpoint
		write_one(g_tstat_id,183,nCurNight);
	}
	else
	{//deadband
		write_one(g_tstat_id,124,nCurNight-nCurDay);
	}
	// TODO: Add your control notification handler code here
	*pResult = 0;
#endif
}

void CT3000View::OnNMReleasedcaptureNightheatSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
#if 1
	int min,max;
	m_daySlider.GetRange(min,max);
	int nCurDay=max-m_daySlider.GetPos()+min;
	int nCurNightHeat=max-m_nightHeatSlider.GetPos()+min;

	CString strTemp;
	strTemp.Format(_T("%d"),nCurNightHeat);
	m_nightHeatInfoEdit.SetWindowText(strTemp);
	int hotel_or_office=multi_register_value[125];
	if(hotel_or_office==0)
	{
		//setpoint
		write_one(g_tstat_id,182,nCurNightHeat);
	}
	else
	{//deadband
		write_one(g_tstat_id,123,nCurDay-nCurNightHeat);
	}
	*pResult = 0;
#endif
}



void CT3000View::Fresh_In()
{
	int nModel=multi_register_value[MODBUS_PRODUCT_MODEL];
	CString strUnit=GetTempUnit();
	CString strTemp;
	strTemp.Empty();
	if( nModel==2||nModel==1||nModel==4||nModel==PM_TSTAT5D||nModel==17||nModel==18
		|| nModel==3||nModel==PM_TSTAT6||nModel==PM_TSTAT7)//A,B,C,D ,F,G,6,7
	{
		//strTemp.Format(_T("Input %d"),1);
		m_Input_Grid.put_TextMatrix(1,1,g_strSensorName);
		strTemp.Format(_T("%.1f"),multi_register_value[216]/10.0);
		strTemp=strTemp+strUnit;
		m_Input_Grid.put_TextMatrix(1,2,strTemp);
		TRACE(_T("INPUT1-")+strTemp+_T("\n"));

		//strTemp.Format(_T("Input %d"),2);
		m_Input_Grid.put_TextMatrix(2,1,g_strInName1);
		strTemp.Empty();

		strUnit=GetTempUnit(multi_register_value[188], 1);
		if(multi_register_value[188]==4||multi_register_value[188]==1)
		{	
			strTemp.Format(_T("%.1f"),(float)multi_register_value[180]/10);
			strTemp=strTemp+strUnit;
		}
		if (multi_register_value[188]==0||multi_register_value[188]==2)
		{
			//strUnit=GetTempUnit(180);
			strTemp.Format(_T("%d"),multi_register_value[180]);
		}
		if(multi_register_value[188]==3)
		{
			if(multi_register_value[180]==1)
				strTemp=_T("ON");
			if(multi_register_value[180]==0)
				strTemp=_T("OFF");
		}
		m_Input_Grid.put_TextMatrix(2,2,strTemp);
		strTemp.Empty();
		
	//	strTemp.Format(_T("Iuput %d"),2);
	//	m_Input_Grid.put_TextMatrix(2,0,g_strInName3);
	//	strTemp.Empty();
	}
	//in3 and in4 
	strUnit=GetTempUnit(multi_register_value[189],2);
	//if(nModel==4||nModel==12||nModel==18||nModel==17)//C,D,F,GnModel==4
	if(nModel==4||nModel==12||nModel==18||nModel==17||nModel == PM_TSTAT7)//tstat7
	{
		strTemp.Empty();
		if(multi_register_value[189]==4||multi_register_value[189]==1)
		{
	
			strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10);	
			strTemp=strTemp+strUnit;
		}
		if (multi_register_value[189]==0||multi_register_value[189]==2)
		{
			//strUnit=GetTempUnit(181);
			strTemp.Format(_T("%d"),multi_register_value[181]);
		}
		if(multi_register_value[189]==3)
		{
			if(multi_register_value[181]==1)
				strTemp=_T("ON");
			if(multi_register_value[181]==0)
				strTemp=_T("OFF");
		}	
		m_Input_Grid.put_TextMatrix(3,2,strTemp);
		strTemp.Empty();
		//strTemp.Format(_T("Input %d"),3);
		m_Input_Grid.put_TextMatrix(3,1,g_strInName2);
		strTemp.Empty();
	

		if (multi_register_value[190]==0)
		{
			if (multi_register_value[311]==0)
			{
				strTemp=_T("OFF");
			}else
			{
				strTemp=_T("ON");
			}
		}
		if (multi_register_value[190]==1)
		{
			if (multi_register_value[311]==1)
			{
				strTemp=_T("OFF");

			}else
			{
				strTemp=_T("ON");
			}
		}
		m_Input_Grid.put_TextMatrix(4,2,strTemp);
		//strTemp.Format(_T("Input %d"),4);
		m_Input_Grid.put_TextMatrix(4,1,g_strInName3);
		strTemp.Empty();
		
	}

	if(nModel==16 || nModel == PM_TSTAT6 ) // E & Tstat 6
	{

		
		g_strSensorName=_T("Internal Sensor");	

		m_Input_Grid.put_TextMatrix(1,1,g_strSensorName);
		m_Input_Grid.put_TextMatrix(2,1,g_strInName1);
		m_Input_Grid.put_TextMatrix(3,1,g_strInName2);
		m_Input_Grid.put_TextMatrix(4,1,g_strInName3);
		m_Input_Grid.put_TextMatrix(5,1,g_strInName4);
		m_Input_Grid.put_TextMatrix(6,1,g_strInName5);
		m_Input_Grid.put_TextMatrix(7,1,g_strInName6);
		m_Input_Grid.put_TextMatrix(8,1,g_strInName7);
		m_Input_Grid.put_TextMatrix(9,1,g_strInName8);
		m_Input_Grid.put_TextMatrix(10,1,g_strInHumName);

		strTemp.Empty();
		if(m_fFirmwareVersion>33.3)
		{
			float nValue;
			for(int i=0;i<=9;i++)
			{
				if ( i == 0)
				{
				
					float fTemp= (float)(multi_register_value[101]/10.0);
					strTemp.Format(_T("%.1f °C"),fTemp);	
					//if (multi_register_value[359] == 1)
					if (multi_register_value[121] == 1)
					{
						strTemp.Format(_T("%.1f °F"),fTemp);	
					}									
			}
				else
				{
					if(multi_register_value[359+i-1]==6)
					{
						strTemp=_T("UNUSED");
					}
					else if(multi_register_value[359+i-1]==3) // On/Off
					{
						nValue=multi_register_value[367+i-1];
						strTemp= nValue ==1 ? _T("On") : _T("Off");
					}
					else if(multi_register_value[359+i-1]==2)
					{
						nValue=multi_register_value[367+i-1];
						strTemp.Format(_T("%0.1f%%"),  nValue);
					}
					else if(multi_register_value[359+i-1]==5) // Off/On
					{
						nValue=multi_register_value[367+i-1];
						strTemp= nValue ==1 ? _T("On") : _T("Off");
					}
					else
					{						
					/*	nValue=multi_register_value[367+i-1]; 
						strTemp.Format(_T("%d"),nValue); */
						strTemp=_T("UNUSED");
					}
				}
				m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
			}
		}
		else
		{
			float nValue;
			for(int i=0;i<8;i++)
			{
				if(multi_register_value[341+i]==1)
				{
					nValue=(float)(multi_register_value[349+i]/10.0);
					strTemp.Format(_T("%.1f"),nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),multi_register_value[349+i]);
				}
				m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
			}

		}
	}

	if(nModel==PM_TSTAT6  )  // Tstat 6 // 2012-02-07, 5E型号灭有湿度
	{
		m_Input_Grid.put_TextMatrix(10,1,g_strInHumName);
		DWORD dwHum = multi_register_value[422];
		CString strHum;
		strHum.Format(_T("%0.1f%%"), dwHum/10.0);
		m_Input_Grid.put_TextMatrix(10,2,strHum);
	}

	if(nModel==19)//H
	{

	}

	if (nModel == PM_PRESSURE)
	{
		CString strRow1 = _T("Pressure sensor");
		m_Input_Grid.put_TextMatrix(1,1,strRow1);
		CString strTemp;
		strTemp.Format(_T("%.1f W.C"), multi_register_value[180]/100.0);
		m_Input_Grid.put_TextMatrix(1,2,strTemp);		
	}
}
void CT3000View::Fresh_Out()
{
	
	int nModel=multi_register_value[MODBUS_PRODUCT_MODEL];
//	switch (nModel)
	CString strTemp;
	int itemp=multi_register_value[108];
	//first 3 
	//==========================================================================
	//ABCDEFG,noH
	if(nModel==2||nModel==1||nModel==4||nModel==12||nModel==16	||nModel==17||nModel==18||nModel==3
		||nModel==PM_TSTAT6||nModel==PM_TSTAT7||nModel==PM_PRESSURE)
	{
		m_Output_Grid.put_TextMatrix(1,1,g_strOutName1);
		m_Output_Grid.put_TextMatrix(2,1,g_strOutName2);
		m_Output_Grid.put_TextMatrix(3,1,g_strOutName3);

		for(int i=1;i<=3;i++)
		{
			//strTemp.Format(_T("Output %d"),i);
			
			if(itemp &( 1<<(i-1)))
				strTemp=_T("ON");
			else
				strTemp=_T("OFF");
			m_Output_Grid.put_TextMatrix(i,2,strTemp);
			strTemp.Empty();
		}
	}

	////----------------------------------------------------------------------
	//4,5
	if(nModel==1||nModel==4||nModel==12||nModel==16||nModel==3
		||nModel==PM_TSTAT6||nModel==PM_TSTAT7||nModel==PM_PRESSURE)//B,C,D,E
	{
		if(itemp &( 1<<3))
				strTemp=_T("ON");
			else
				strTemp=_T("OFF");

		m_Output_Grid.put_TextMatrix(4,2,strTemp);
	//	strTemp.Format(_T("Output %d"),4);
	    m_Output_Grid.put_TextMatrix(4,1,g_strOutName4);

		strTemp.Empty();
		if(itemp &( 1<<4))
				strTemp=_T("ON");
		else
				strTemp=_T("OFF");
		m_Output_Grid.put_TextMatrix(5,2,strTemp);

		//strTemp.Format(_T("Output %d"),5);
		m_Output_Grid.put_TextMatrix(5,1,g_strOutName5);
		strTemp.Empty();
	}

		//:DEG 6. 7
		int nRange=multi_register_value[186];
		if(nModel==12||nModel==16||nModel==18||nModel==PM_TSTAT6||nModel==PM_TSTAT7||nModel==PM_PRESSURE)
		{
			if(nRange==0)
			{
				itemp=multi_register_value[102];

				if(itemp==0)
					strTemp=_T("OFF");
				if(itemp==1000)
					strTemp=_T("ON");
			}
			else
			{
				//strTemp.Format(_T("%.1f"),multi_register_value[102]/100.0);
				float nvalue=0.0;

				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=(float)(multi_register_value[102]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(multi_register_value[102]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(multi_register_value[102]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=float((10-multi_register_value[102]/100.0)/10.0 *100);
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_Output_Grid.put_TextMatrix(6,2,strTemp);
			strTemp.Empty();


		//	strTemp.Format(_T("Output %d"),6);
			m_Output_Grid.put_TextMatrix(6,1,g_strOutName6);
			strTemp.Empty();
			
			nRange=multi_register_value[187];
			if(nRange==0)
			{
				itemp=multi_register_value[103];
				
				if(itemp==0)
					strTemp=_T("OFF");
				if(itemp==1)
					strTemp=_T("ON");
			
			}
			else
			{
				//strTemp.Format(_T("%.1f"),multi_register_value[103]/100.0);
				float nvalue=0.0;

				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=(float)(multi_register_value[103]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(multi_register_value[103]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(multi_register_value[103]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=(float)((10-multi_register_value[103]/100)/10.0*100);
				}
				strTemp.Format(_T("%.1f%%"),nvalue);

			}
			m_Output_Grid.put_TextMatrix(7,2,strTemp);
		//	strTemp.Format(_T("Output %d"),7);
			m_Output_Grid.put_TextMatrix(7,1,g_strOutName7);
			strTemp.Empty();
		}
		if(nModel==2)//A 4,5
		{
			 nRange=multi_register_value[186];
			if(nRange==0)
			{
				itemp=multi_register_value[102];

				if(itemp==0)
					strTemp=_T("OFF");
				if(itemp==1)
					strTemp=_T("ON");
			}
			else
			{
				//strTemp.Format(_T("%.1f"),multi_register_value[102]/100.0);
					float nvalue=0.0;
				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=(float)(multi_register_value[102]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(multi_register_value[102]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(multi_register_value[102]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=(float)((10-multi_register_value[102]/100.0)/10.0 *100);
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			
			}
			m_Output_Grid.put_TextMatrix(4,2,strTemp);
			strTemp.Empty();
		//	strTemp.Format(_T("Output %d"),4);
			m_Output_Grid.put_TextMatrix(4,1,g_strOutName4);
			strTemp.Empty();
			nRange=multi_register_value[187];
			if(nRange==0)
			{
				itemp=multi_register_value[103];
				
				if(itemp==0)
					strTemp=_T("OFF");
				if(itemp==1)
					strTemp=_T("ON");
			}
			else
			{
				//strTemp.Format(_T("%.1f"),multi_register_value[103]/100.0);
					float nvalue=0.0;
				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=(float)(multi_register_value[103]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(multi_register_value[103]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(multi_register_value[103]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=(float)((10-multi_register_value[103]/100.0)/10.0 *100);
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_Output_Grid.put_TextMatrix(5,2,strTemp);
			//strTemp.Format(_T("Output %d"),5);
			m_Output_Grid.put_TextMatrix(5,1,g_strOutName5);
			strTemp.Empty();
		}//end A


		//FG 4,5
		if(nModel==17||nModel==18)
		{
			
			if(multi_register_value[283]==0)
			{
				if(itemp &( 1<<3))
						strTemp=_T("ON");
					else
						strTemp=_T("OFF");

			}
			else
			{
				strTemp.Format(_T("%d%%"),multi_register_value[348]);

			}
			m_Output_Grid.put_TextMatrix(4,2,strTemp);
			m_Output_Grid.put_TextMatrix(4,1,g_strOutName4);
			strTemp.Empty();	

			if(multi_register_value[284]==0)
			{
				if(itemp &( 1<<4))
						strTemp=_T("ON");
					else
						strTemp=_T("OFF");

			}
			else
			{
				strTemp.Format(_T("%d%%"),multi_register_value[349]);

			}
			m_Output_Grid.put_TextMatrix(5,2,strTemp);
			m_Output_Grid.put_TextMatrix(5,1,g_strOutName5);
			strTemp.Empty();	
		}
}


void CT3000View::FreshIOGridTable()
{
	if(::GetFocus()==m_outNameEdt.m_hWnd)
		return;
	if(::GetFocus()==m_inNameEdt.m_hWnd)
		return;

	m_Output_Grid.Clear();
	m_Input_Grid.Clear();
	int nModel=multi_register_value[MODBUS_PRODUCT_MODEL];
	switch (nModel)
	{
		case 3:
		case 2:
		case 1:
			{
				m_outRows=6;
				m_inRows=3;
			}
				
			break;
		case 4:
			{
				m_outRows=6;
				m_inRows=5;

			 }
			break;	
		case PM_TSTAT7:
		case PM_TSTAT5D:  // 5D 同 TStat7
			{
				m_outRows=8;
				m_inRows=5;

			 }break;
		case PM_TSTAT6:
		case PM_TSTAT5E:
			{
				m_outRows=8;
				m_inRows=11;
			}break;
		case 17:
			{
				m_outRows=6;
				m_inRows=5;
			}
				break;
		case 18:
			{
				m_outRows=8;
				m_inRows=5;
			}
			break;
		case 19:
			{
				m_outRows=8;
				m_inRows=9;
			}
			break;
		case PM_PRESSURE:
			{
				m_outRows=8;
				m_inRows=2;
			}
			break;

		default:break;
	}
// 	m_Input_Grid.put_Rows(m_inRows);
// 	m_Output_Grid.put_Rows(m_outRows);

	m_Input_Grid.put_Rows(m_inRows);
	m_Output_Grid.put_Rows(m_outRows);


	for(int i=1;i<m_inRows;i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"),i);
		m_Input_Grid.put_TextMatrix(i,0,strTemp);
	}
	
	for(int i=1;i<m_outRows;i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"),i);
		m_Output_Grid.put_TextMatrix(i,0,strTemp);
	}
	for(int i=0;i<=2;i++)
	{
		m_Input_Grid.put_ColAlignment(i,4);
		m_Output_Grid.put_ColAlignment(i,4);
	}

	m_Input_Grid.put_TextMatrix(0,1,_T("Name"));
	m_Input_Grid.put_TextMatrix(0,2,_T("Value"));
	


	m_Output_Grid.put_TextMatrix(0,1,_T("Name"));
	m_Output_Grid.put_TextMatrix(0,2,_T("Value"));

	for(int i=1;i<m_inRows;i++)
	{
		
		for(int k=0;k<=2;k++)
		{
			if (i%2==1)
			{
				m_Input_Grid.put_Row(i);m_Input_Grid.put_Col(k);m_Input_Grid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_Input_Grid.put_Row(i);m_Input_Grid.put_Col(k);m_Input_Grid.put_CellBackColor(COLOR_CELL);
			}
		}
	}

	for(int i=1;i<m_outRows;i++)
	{
		
		for(int k=0;k<=2;k++)
		{
			if (i%2==1)
			{
				m_Output_Grid.put_Row(i);m_Output_Grid.put_Col(k);m_Output_Grid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_Output_Grid.put_Row(i);m_Output_Grid.put_Col(k);m_Output_Grid.put_CellBackColor(COLOR_CELL);
			}
		}
	}





	Fresh_Out();
	Fresh_In();



}


void CT3000View::OnBnClickedGrapgicbutton()
{

	// TODO: Add your control notification handler code here
}

void CT3000View::OnBnClickedParameterbtn()
{
	g_bEnableRefreshTreeView = FALSE;




	CParameterDlg dlg;
	dlg.DoModal();
	Fresh();


	
	
	g_bEnableRefreshTreeView = TRUE;

	// TODO: Add your control notification handler code here
}


void CT3000View::OnBnClicked5ebutton()
{
	C5EInputDialog dlg;
	dlg.DoModal();
	

}
BEGIN_EVENTSINK_MAP(CT3000View, CFormView)
	ON_EVENT(CT3000View, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CT3000View::ClickInputMsflexgrid, VTS_NONE)
	ON_EVENT(CT3000View, IDC_OUTPUT_MSFLEXGRID, DISPID_CLICK, CT3000View::ClickOutputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void CT3000View::ClickInputMsflexgrid()
{

	long lRow,lCol;
	lRow = m_Input_Grid.get_RowSel();//
	lCol = m_Input_Grid.get_ColSel(); //
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_Input_Grid.GetWindowRect(rect); //
	ScreenToClient(rect); //
	CDC* pDC =GetDC();


	

}

void CT3000View::ClickOutputMsflexgrid()
{
	long lRow,lCol;
	lRow = m_Output_Grid.get_RowSel();//
	lCol = m_Output_Grid.get_ColSel(); //
	TRACE(_T("Click output grid!\n"));

}

void CT3000View::OnEnKillfocusInputnameedit()
{
	CString strText;

	


}

void CT3000View::OnEnKillfocusOutputnameedit()
{

	CString strText;
	m_outNameEdt.GetWindowText(strText);
	m_outNameEdt.ShowWindow(SW_HIDE);
	int lRow = m_Output_Grid.get_RowSel();//
	int lCol = m_Output_Grid.get_ColSel(); //

	CString strInName;
	if(lCol!=1||lRow==0)
		return;


	
}

BOOL CT3000View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
		
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			GetDlgItem(IDC_ID_EDIT)->SetFocus();
			return true;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CT3000View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(g_bPauseMultiRead)
		return;
//	TRACE("Start to Fresh view!!! \n");
	//Fresh();
// 	TRACE("End to Fresh view!!! \n");


	CTime time = CTime::GetCurrentTime();

	CString strtime = time.Format(_T("%I:%M:%S %p"));
	//CString strtime = time.Format(_T("%m/%d/%Y %H:%M:%S %a"));
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC_TIME);
	pEdit->SetWindowText(strtime);

	CString strDate = time.Format(_T("%A, %b %d, %Y"));
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC);
	pEdit->SetWindowText(strDate);

	CFormView::OnTimer(nIDEvent);
}

void CT3000View::OnDestroy()
{
	CFormView::OnDestroy();
	KillTimer(1);
	// TODO: Add your message handler code here
}

void CT3000View::OnBnClickedTrendlogview()
{

	
//	SwitchToPruductType(3);

	// TODO: Add your control notification handler code here
}

void CT3000View::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
// 	TDelayForm Dlg;
// 	Dlg.DoModal();
}

void CT3000View::OnBnClickedLedbutton()
{


}

void CT3000View::OnBnClickedOccupacheck()
{
	CString strTemp;
	//stat6



	CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);
	if(m_OcupiedBtn.GetCheck()==BST_UNCHECKED)
	{
		if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
			write_one(g_tstat_id,109,0);	
		else
			write_one(g_tstat_id,184,0);	
		pBtn->SetCheck(1);
		multi_register_value[184] = 0;
	}


	if(m_OcupiedBtn.GetCheck()==BST_CHECKED)
	{
		if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
			write_one(g_tstat_id,109,1);
		else
			write_one(g_tstat_id,184,1);

		multi_register_value[184] = 1;

		pBtn->SetCheck(0);
	}

	UpdateData(TRUE);
	//stat6
	Fresh();


}

void CT3000View::OnBnClickedUnoccupiedMark()
{



	CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);

	if(pBtn->GetCheck()==BST_UNCHECKED)
	{

			write_one(g_tstat_id,184,1);	
			multi_register_value[184] = 1;
		m_OcupiedBtn.SetCheck(1);
	}

	if(pBtn->GetCheck()==BST_CHECKED)
	{

			write_one(g_tstat_id,184,0);

			multi_register_value[184]  = 0;
		m_OcupiedBtn.SetCheck(0);
	}

UpdateData(TRUE);

Fresh();

}

void CT3000View::OnCbnSelchangeFanspeedcombo()
{

	int ret=0;



}


LRESULT CT3000View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(MsgT3000ViewFresh==message)
	{
	//	AfxMessageBox(_T("freshe!"));




		Fresh();
	}

	return CFormView::WindowProc(message, wParam, lParam);
}

void CT3000View::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);


}


void CT3000View::OnMove(int x, int y)
{
	CFormView::OnMove(x, y);


	// TODO: Add your message handler code here
}

void CT3000View::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CFormView::OnWindowPosChanging(lpwndpos);


	

}

void CT3000View::OnMoving(UINT fwSide, LPRECT pRect)
{
	CFormView::OnMoving(fwSide, pRect);

}

LRESULT CT3000View::OnFreshView(WPARAM wParam, LPARAM lParam)
{
	Fresh();
	return 0;
}



void CT3000View::OnBnClickedBtnSynctime()
{
	CTime time = CTime::GetCurrentTime();
	BYTE szTime[8] ={0};
	
	BeginWaitCursor();
	GetDlgItem(IDC_BTN_SYNCTIME)->EnableWindow(FALSE);
	
	
	
}

void CT3000View::OnBnClickedButtonSchedule()
{
	if(multi_register_value[7] < PM_TSTAT6)
	{
		AfxMessageBox(_T("This model of TStat don't support schedule!"));
		return;
	}
// 	g_bPauseMultiRead = TRUE;
// 	CTStatScheduleDlg dlg;
// 	dlg.DoModal();
// 	g_bPauseMultiRead = FALSE;
	
}


void CT3000View::InitFanSpeed()
{
	CString p[6]={_T("Fan Off"),_T("Fan On"),_T("Fan Low"),_T("Fan Mid"),_T("Fan High"),_T("Fan Aut")};
	//		UpdateData(true);
	m_FanComBox.ResetContent();
	if(multi_register_value[129]==1)   //here can't use read_multi function
	{
		m_FanComBox.AddString(p[0]);
		m_FanComBox.AddString(p[5]);			
	}
	else
	{			
		switch(multi_register_value[122]-1)
		{
		case 0:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[1]);m_FanComBox.AddString(p[5]);break;
		case 1:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[2]);m_FanComBox.AddString(p[4]);m_FanComBox.AddString(p[5]);break;
		default:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[2]);m_FanComBox.AddString(p[3]);m_FanComBox.AddString(p[4]);m_FanComBox.AddString(p[5]);break;
		}
	}
	m_FanComBox.SetCurSel(multi_register_value[137]);
	m_iCurFanSpeed = multi_register_value[137];
// int nsel = m_FanComBox.GetCurSel();
// 	CString str;
// 	m_FanComBox.GetLBText(nsel, str);

}

void CT3000View::HandleSliderSetPos( BOOL bRight )
{

	if (!bRight)
	{
		if (!m_bSliderSetPosWarning)
		{
			return;
		}
// 		CString strTips = _T("Set Point can't be set properly. Please confirm your set point value!");
// 		int nRet = AfxMessageBox(strTips , MB_RETRYCANCEL);
// 		if(nRet == IDRETRY)
// 		{
// 			m_bSliderSetPosWarning = TRUE;
// 		}
// 		else
// 		{
// 			m_bSliderSetPosWarning = FALSE;F
// 		}
		m_bSliderSetPosWarning = TRUE;
	}
	else
	{
		m_bSliderSetPosWarning = TRUE;
	}
}


void CT3000View::OnBnClickedFlash()
{
	/*CMainFrame::OnDisconnect();*/
//	SaveParameterToConfig();//把要刷新的设备参数保存到配置文件中

}
