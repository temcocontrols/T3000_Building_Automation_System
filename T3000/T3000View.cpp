
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

#include "TStatScheduleDlg.h"

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


//2013 03 10
//��ëҪ������������е����� ����  multi_register_value[101] ����101Ҫ��Ϊ�ɱ�ģ������ݿ��ж�����
//����д��һ������ Get �ڳ�������ǰ ���� mdb���ڳ��������ж�̬�Ĳ��� ��Ӧ��ֵ��product_register_value[MODBUS_TEMPRATURE_CHIP].��
//�����ȱ�֤�˳���Ŀɶ��ԣ��ַ���ά�����պ� �Ĵ����б�仯��ֻ���޸� t3000.mdb�ļĴ������ɡ�
#include "T3000RegAddress.h"
extern T3000RegAddress GloabeAddress;
extern int Mdb_Adress_Map;
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
	ON_MESSAGE(WM_USER_CALLBACK, OnFlexSlideCallBack)
	ON_BN_CLICKED(IDC_UNOCCUPIED_MARK, &CT3000View::OnBnClickedUnoccupiedMark)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TEMPRETURE_SLIDER, &CT3000View::OnNMReleasedcaptureTempretureSlider)
	ON_EN_KILLFOCUS(IDC_DAY_EDIT, &CT3000View::OnEnKillfocusDayEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_CUR_SP, &CT3000View::OnEnKillfocusEditCurSp)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CT3000View::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CT3000View::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_STATICUNINT, &CT3000View::OnCbnSelchangeStaticunint)
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
	, m_daysetpoint(0)//0907
	, m_nightpot(0)//0907
{
	// TODO: add construction code here
	m_hSerial=NULL;
	m_hSocket=NULL;
	m_outRows=1;
	m_inRows=1;
	m_pSetPtDayDlg=NULL;
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
	/*
	if(m_pSetPtDayDlg!=NULL)
	{
		m_pSetPtDayDlg->DestroyWindow();
		delete m_pSetPtDayDlg;
	}
	*/

	//AfxMessageBox(_T("t3000view destructer"));

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

	//DDX_Radio(pDX, IDC_OCCUPA_RADIO, m_OcupaCtrl);
	//DDX_Radio(pDX, IDC_UNOCCUPA_RADIO, m_UnOcupaCtrl);
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
	DDX_Text(pDX, IDC_DAY_EDIT, m_daysetpoint);//0907
	DDX_Text(pDX, IDC_EDIT_CUR_SP, m_nightpot);//0907
	DDX_Control(pDX, IDC_SLIDER_DAY, m_singlesliderday);
	DDX_Control(pDX, IDC_SLIDER_NIGHT, m_singleslidernight);
	DDX_Control(pDX, IDC_STATICUNINT, m_gUnit);
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

	m_gUnit.AddString(_T("��C"));
	m_gUnit.AddString(_T("��F"));
/*
	CFile myfile(_T("D:\\big.HEX"),CFile::modeRead);//test.txt
	//if(m_hex_file.Open(_T("D:\\big.HEX"),CFile::modeRead))
	//if(m_hex_file.Open(_T("D:\\test.txt"),CFile::modeRead|CFile::typeBinary))
	{
		
		CArchive ar(&myfile,CArchive::load);
			CString m_get_data_from_file;
			unsigned int ii=0;
		while(NULL!=ar.ReadString(m_get_data_from_file))//ѭ����ȡ�ļ���ֱ���ļ�����
			{
			//	ar.ReadString(m_get_data_from_file);
				int ddd=m_get_data_from_file.GetLength();
				AfxMessageBox(m_get_data_from_file);

			}
	}
	*/
	
	/*
	wifstream inf;//file
	inf.open(_T("D:\\big.HEX"),ios_base::in);	
	TCHAR buf[1024];
	CString m_get_data_from_file;
	int ddd;
	while(!inf.eof())
	{
	//	inf.getline((LPTSTR)(LPCTSTR)&m_get_data_from_file,1024);
		inf.getline(buf,1024);
		ddd=_tcslen(buf);
			
	}
	*/
//	CStdioFile m_hex_file;
	//if(m_hex_file.Open(m_hex_file_path,CFile::modeRead))

	/*
	CStdioFile myfile;
	myfile.Open(_T("D:\\big.HEX"),CFile::modeRead);
	CString strLine;
	int nlenth=myfile.GetLength();
	int nRead=0;
	while(NULL!=myfile.ReadString(strLine))
	{
		nRead=nRead+strLine.GetLength();
	}
	CString strTemp;
	strTemp.Format(_T("%d"),nRead);
	AfxMessageBox(strTemp);
	*/

	GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);
	GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);
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


	//m_SetptStatic.ShowWindow(SW_HIDE);
#if 0
	m_pSetPtDayDlg=new CSetPtDayDlg();
	m_pSetPtDayDlg->Create(IDD_SETPTDAYDLG,this);
	m_pSetPtDayDlg->LoadImage();
	//m_pSetPtDayDlg->MoveWindow(CRect(620,50,1070,650));

	CRect rcSetptWnd;
	m_SetptStatic.GetWindowRect(rcSetptWnd);
	rcSetptWnd.right=rcSetptWnd.left+450;
	rcSetptWnd.bottom=rcSetptWnd.top+600;
	m_pSetPtDayDlg->MoveWindow(&rcSetptWnd);
	m_pSetPtDayDlg->ShowWindow(SW_SHOW);

#endif

	SetTimer(1,1000,NULL);

	// Create flex slide bar
	//return;
	CreateFlexSilde();


// 	CString strTemp;
// 	strTemp.Format(_T("%d"),10);
// 	m_dayInfoEdit.SetWindowText(strTemp);




//	m_singlesliderday.SetTicFreq(10);

	m_singlesliderday.SetThumbColor(RGB(0, 0, 255));
	m_singlesliderday.SetSelectionColor(RGB(64, 64, 255)); 
	m_singlesliderday.SetChannelColor(RGB(196, 196, 255)); 



	m_singleslidernight.SetThumbColor(RGB(0, 0, 255));
	m_singleslidernight.SetSelectionColor(RGB(64, 64, 255)); 
	m_singleslidernight.SetChannelColor(RGB(196, 196, 255)); 


 	GetDlgItem(IDC_SLIDER_DAY)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_SLIDER_NIGHT)->ShowWindow(SW_HIDE);

	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMain->OnConnect();

}

void CT3000View::CreateFlexSilde()
{
	if(m_pNightTwoSP == NULL)
	{
		CStatic *s = (CStatic*)GetDlgItem(IDC_FSB_OFFICE);
		CRect rc;
		s->GetWindowRect(&rc);
		ScreenToClient(&rc);
#if 1
		m_pNightTwoSP=new CFSBContainer;
		m_pNightTwoSP->SetParentWnd(this);

		//int nRet = m_pNightTwoSP->SubclassWindow(s->GetSafeHwnd());
		//int nRet = m_pNightTwoSP->SubclassDlgItem(IDC_FSB_OFFICE, this);

		//////////////////////////////////////////////////////////////////////////

		
		m_pNightTwoSP->CreateInstance(s, s->GetStyle()|s->GetExStyle(), rc);
		//m_pNightTwoSP->Create( NULL, s->GetStyle()|s->GetExStyle()|SS_SUNKEN, rc, this);
//		m_pNightTwoSP.Create(_T("CFlexSlideWnd"), SS_SUNKEN, rc, this ,10001);  
//		m_pNightTwoSP.ShowWindow(SW_SHOW);

//*		
		m_pNightTwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB);
		m_pNightTwoSP->SetFlexSlideBarRect(rc);
		m_pNightTwoSP->SetHorizontal(FALSE);
		m_pNightTwoSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
// 		m_pNightTwoSP->SetChannelColor(RGB(192,192,192));
// 		m_pNightTwoSP->SetThumbColor(RGB(255,128,128));

		m_pNightTwoSP->SetChannelColor(RGB(192,192,192));
		m_pNightTwoSP->SetThumbColor(RGB(0,0,255)); //�ı����滬�����ɫ
		m_pNightTwoSP->SetPageLength(5);
		m_pNightTwoSP->SetChannelWidth(10);//�м�ļ�״����С
		m_pNightTwoSP->SetThumbWidth(24);//����Ĵ�С

		m_pNightTwoSP->SetRange(10, 0);
		m_pNightTwoSP->SetPos(0, 1, 2);	
		m_pNightTwoSP->Setflag(1);
		// init two thumb slider

#endif






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
		m_pNightSingleSP->Setflag(2);

	
		//////////////////////////////////////////////////////////////////////////
		// Day

		s = (CStatic*)GetDlgItem(IDC_FSB_DAYOFFICE);
		rc;
		s->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_pDayTwoSP=new CFSBContainer;
		m_pDayTwoSP->SetParentWnd(this);

		m_pDayTwoSP->CreateInstance(s, s->GetStyle()|s->GetExStyle(), rc);
	
		m_pDayTwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB);//10,12,FSB_THUMBSHAPE_TRI
	//	m_pDayTwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB,10,12,FSB_THUMBSHAPE_TRI);
		m_pDayTwoSP->SetFlexSlideBarRect(rc);
		m_pDayTwoSP->SetHorizontal(FALSE);
		m_pDayTwoSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
// 		m_pDayTwoSP->SetChannelColor(RGB(192,192,192));
// 		m_pDayTwoSP->SetThumbColor(RGB(255,128,128));

		m_pDayTwoSP->SetChannelColor(RGB(192,192,192));
		m_pDayTwoSP->SetThumbColor(RGB(0,0,255));
		m_pDayTwoSP->SetPageLength(5);
		m_pDayTwoSP->SetChannelWidth(10);
		m_pDayTwoSP->SetThumbWidth(24);

		m_pDayTwoSP->SetRange(10, 0);
		m_pDayTwoSP->SetPos(0, 1, 2);	
		m_pDayTwoSP->Setflag(3);

		
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
		m_pDaySingleSP->Setflag(4);




		//sepoint

// 		CStatic *t= (CStatic*)GetDlgItem(IDC_TEMPERSET);
// 		m_pTemperSP= new CFSBContainer;
// 		t->GetWindowRect(&rc);
// 		ScreenToClient(&rc);
// 		m_pTemperSP->CreateInstance(t, t->GetStyle()|t->GetExStyle()|SS_SUNKEN, rc);
// 		m_pTemperSP->SetParentWnd(this);
// 
// 		m_pTemperSP->CreateFlexSlideBar(FALSE, FSB_STYLE_TRIPLELINKAGE );	
// 		m_pTemperSP->SetFlexSlideBarRect(rc);
// 		m_pTemperSP->SetHorizontal(FALSE);
// 		m_pTemperSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
// 		m_pTemperSP->SetChannelColor(RGB(192,192,192));
// 		m_pTemperSP->SetThumbColor(RGB(255,128,128));
// 		m_pTemperSP->SetPageLength(5);
// 		m_pTemperSP->SetChannelWidth(10);
// 		m_pTemperSP->SetThumbWidth(24);
// 
// 		m_pTemperSP->SetRange(10, 0);
// 		m_pTemperSP->SetPos(0, 1, 2);	

#if 0

		CStatic *t= (CStatic*)GetDlgItem(IDC_TEMPERSET);
		t->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_pTemperSP=new CFSBContainer;
		m_pTemperSP->SetParentWnd(this);

		m_pTemperSP->CreateInstance(t, t->GetStyle()|t->GetExStyle(), rc);

		m_pTemperSP->CreateFlexSlideBar(FALSE,FSB_STYLE_SINGLETHUMB);//FSB_STYLE_DOUBLETHUMB,10,12,FSB_THUMBSHAPE_TRI);//FSB_STYLE_DOUBLETHUMB//1����ťFSB_STYLE_SINGLETHUMB
		m_pTemperSP->SetFlexSlideBarRect(rc);//FSB_STYLE_SINGLETHUMB
		m_pTemperSP->SetHorizontal(FALSE);
		m_pTemperSP->SetFSBThumbShape(FSB_THUMBSHAPE_RECT);//
		m_pTemperSP->SetChannelColor(RGB(192,192,192));
		m_pTemperSP->SetThumbColor(RGB(255,128,128));
		m_pTemperSP->SetPageLength(5);
		m_pTemperSP->SetChannelWidth(10);
		m_pTemperSP->SetThumbWidth(24);

		m_pTemperSP->SetRange(10, 0);
		m_pTemperSP->SetPos(0, 1, 2);	
#endif
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

// 	strInfo.Format(_T("CT3000View::Fresh():FreshIOGridTable()"));			
// 	SetPaneString(2, strInfo);
//Alex-Changed
    CString strTemp;
	strTemp.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
	strTemp+=_T("%");
	GetDlgItem(IDC_OUTPUT1)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270  389
	strTemp+=_T("%");
	GetDlgItem(IDC_OUTPUT2)->SetWindowText(strTemp);

    FreshCtrl();
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
	int min=product_register_value[MODBUS_MIN_SETPOINT];//132
	int max=product_register_value[MODBUS_MAX_SETPOINT];//131
	m_TemperaureSlider.SetRange(min,max);
	m_daySlider.SetRange(min,max);
	m_nightSlider.SetRange(min,max);
	m_nightHeatSlider.SetRange(min,max);

//	m_TemperaureSlider.SetPos((int)(max-m_fTemperature+min));//2.5.0.95
	if (g_unint)
		strTemp.Format(_T("%.1f��C"),m_fTemperature);
	else
		strTemp.Format(_T("%.1f��F"),m_fTemperature);

	//m_TempInfoEdit.SetWindowText(strTemp);//2.5.0.95

	float itemp;
	if( product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT7)//7
	{
		itemp=(float)(product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10.0);//380
	}
	else if(m_fFirmwareVersion<34.9 || product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT5E || product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT6)
	{
		itemp=(float)(product_register_value[MODBUS_COOLING_SETPOINT]);//135
	}
	else
	{
		itemp=(float)(product_register_value[MODBUS_TWO_BYTE_SETPOINT]/10.0);//374
	}
	m_daySlider.SetPos(int(max-itemp+min));

 	strTemp.Format(_T("%.1f"),itemp);
 	m_dayInfoEdit.SetWindowText(strTemp);

	int m_hotel_or_office=product_register_value[MODBUS_APPLICATION];///////hotel or office//125
	int the_setpoint_ha_ha=(int)itemp;// copy from old code vc2003.
	/*
	//m_TemperaureSlider.SetRange(20,100);
//	m_TemperaureSlider.SetPos(30);
//	m_nightSlider.SetPos(max-(itemp-min)+0.5);
//	m_nightHeatSlider.SetPos(max-(itemp-min)+0.5);
*/
#if 0
	if(m_hotel_or_office==0)
	{
		itemp=multi_register_value[183];
		if(the_setpoint_ha_ha>=itemp)
		{
			itemp=the_setpoint_ha_ha+1;
			write_one(g_tstat_id,183,itemp);
		}
		if(the_setpoint_ha_ha>max)
		{
			itemp=max;
			write_one(g_tstat_id,183,itemp);
		}
	}
	else
	{
		itemp=multi_register_value[124]+the_setpoint_ha_ha;//cool
		if(itemp>max)
			itemp=max;
	}
	m_nightSlider.SetPos(max-(itemp-min));
	strTemp.Format(_T("%.1f"),itemp);
	m_nightInfoEdit.SetWindowText(strTemp);


	if(m_hotel_or_office==0)
	{
		itemp=multi_register_value[182];//heat
		if(the_setpoint_ha_ha<=itemp)					
		{
			itemp=the_setpoint_ha_ha-1;
			write_one(g_tstat_id,182,itemp);
		}
		if(the_setpoint_ha_ha<min)
		{
			itemp=min;
			write_one(g_tstat_id,182,itemp);
		}
	}
	else
	{
		itemp=the_setpoint_ha_ha-multi_register_value[123];//heat
		if(itemp<min)
			itemp=min;
	}
	m_nightHeatSlider.SetPos(max-(itemp-min));
	strTemp.Format(_T("%.1f"),itemp);
	m_nightHeatInfoEdit.SetWindowText(strTemp);
#endif

	if(m_hotel_or_office==0)  // office
	{
		itemp=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];				//183
		m_nightSlider.SetPos((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),itemp);
		m_nightInfoEdit.SetWindowText(strTemp);

		itemp=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];//heat			//182
		m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),itemp);
		m_nightHeatInfoEdit.SetWindowText(strTemp);
		m_NightCoolStatic.SetWindowText(_T("Cooling"));
		m_NightHeatStatic.SetWindowText(_T("Heating"));
		//;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));

	}
	
	if(m_hotel_or_office==1)
	{
		itemp=(float)(product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]+the_setpoint_ha_ha);//cool	//124
		m_nightSlider.SetPos(int(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]);//124
		m_nightInfoEdit.SetWindowText(strTemp);

// 		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);//0907
// 		pSPWnd->SetWindowText(strTemp);

		itemp=(float)(the_setpoint_ha_ha-product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]);//heat	//123
		m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]);//123
		m_nightHeatInfoEdit.SetWindowText(strTemp);

		m_NightCoolStatic.SetWindowText(_T("CoolDB"));
		m_NightHeatStatic.SetWindowText(_T("HeatDB"));
		//GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Hotel"));

	}
#endif
}


// ʹ��һ��3Thumb��Slider������֮ǰ��3��Slider
// ר�Ÿ�TStat5E֮ǰ���ͺţ�A,B,C,D,F,G,H��
void CT3000View::InitSliderBars2()
{

	//0907����
	// ��TSTAT5E Runar ver34.1Ϊ����
	// ��TSTAT5E ver35.5Ϊ����
	CString strTemp;
	int min=product_register_value[MODBUS_MIN_SETPOINT];//132
	int max=product_register_value[MODBUS_MAX_SETPOINT];//131
	m_TemperaureSlider.SetRange(min,max);



	//m_nightSlider.SetRange(min,max);
	//m_nightHeatSlider.SetRange(min,max);
	m_pNightSingleSP->SetRange(max, min);


	float itemp;
	if(product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT5E)
	{
		itemp=(float)(product_register_value[MODBUS_COOLING_SETPOINT]);//135
	}
	else
	{
		itemp=(float)(product_register_value[MODBUS_TWO_BYTE_SETPOINT]/10.0);//374
	}
//0913
// 	strTemp.Format(_T("%.1f"),itemp);
// 	m_dayInfoEdit.SetWindowText(strTemp);
	m_daySlider.SetRange(min,max);
	m_daySlider.SetPos(int(max-itemp+min));
	//m_daySlider.ShowWindow(SW_NORMAL);
	int nSP =(int)itemp;//(max-itemp+min);



	short nOccupied = product_register_value[MODBUS_INFO_BYTE];  // Day setpoint option  184
	BOOL bOccupied = nOccupied & 0x0001;


	int nOfficeSelReg=454;
	if (bOccupied)
	{	
		if(product_register_value[MODBUS_PRODUCT_MODEL]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
		{
			nOfficeSelReg = 423;
		}
	}
	else
	{
		nOfficeSelReg = 125;
	}
	int m_hotel_or_office=multi_register_value[nOfficeSelReg];///////hotel or office
	int the_setpoint_ha_ha=(int)itemp;		// copy from old code vc2003.

	//////////////////////////////////////////////////////////////////////////
	if(m_hotel_or_office==0  && bOccupied)  // office - occupied
	{  // 2sp
		CString strTemp1 = _T("Two SetPoint Mode");
		CWnd *pWndMode1 = GetDlgItem(IDC_STATIC_SPMODE2);
		pWndMode1->SetWindowText(strTemp1);

		//nSP = multi_register_value[374];//(nCoolSP-nHeatSP)/2 + nHeatSP;
	   itemp=product_register_value[MODBUS_COOLING_DEADBAND];//119
	   //m_nightSlider.SetPos((int)(max-(itemp-min)));
	   int nCoolDB = (int)(itemp/10);//(int)(max-(itemp-min));

	   itemp=product_register_value[MODBUS_HEATING_DEADBAND];//heat	//120
		//m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		int nHeatDB = (int)(itemp/10);//((int)(max-(itemp-min)));


		m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
		m_DayHeatStatic.SetWindowText(_T("Heating SP"));
		//;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));

		int nCoolSP = (nCoolDB + nSP); 
		int nHeatSP =(nSP - nHeatDB);
		//0913
// 		strTemp.Format(_T("%0.1f"), nSP);
// 		//CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);//IDC_DAY_EDIT
// 		CWnd *pSPWnd = GetDlgItem(IDC_DAY_EDIT);//0913
// 		pSPWnd->SetWindowText(strTemp);
		m_daysetpoint = nSP;//0913

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

		m_pDayTwoSP->SetRange(nRangeMax, nRangeMin);
		BOOL bRetSP = m_pDayTwoSP->SetPos(nHeatSP, nCoolSP, nCoolSP+1); // ��Ϊֻ��2��thumb������sp��Ҫ�ˣ���Ϊ���˵�ֵ		
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
		int nCoolSP =(int) (itemp/10.0);//(int)(max-(itemp-min));
		strTemp.Format(_T("%.1f"),float(nCoolSP-nSP));
		m_DayCoolEdit.SetWindowText(strTemp);

		itemp=multi_register_value[136];//heat
		//m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
		int nHeatSP = (int)itemp;//((int)(max-(itemp-min)));
		strTemp.Format(_T("%.1f"),float(nSP-nHeatSP));
		m_DayHeatEdit.SetWindowText(strTemp);

 		m_DayCoolStatic.SetWindowText(_T("Cooling DB"));
 		m_DayHeatStatic.SetWindowText(_T("Heating DB"));


		strTemp.Format(_T("%d"), multi_register_value[135]);
		//0913
		//CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);//IDC_DAY_EDIT
// 		CWnd *pSPWnd = GetDlgItem(IDC_DAY_EDIT);
// 		pSPWnd->SetWindowText(strTemp);

		m_daysetpoint = multi_register_value[135];//0913


		if (nCoolSP == nHeatSP && nCoolSP== nSP )
		{
			nSP = nHeatSP + 1;
			nCoolSP = nSP+1;				
		}

		int nRangeMax = nCoolSP > max ? nCoolSP : max;
		int nRangeMin = nHeatSP < min ? nHeatSP : min;

		m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);
		BOOL bRetSP = m_pDaySingleSP->SetPos(nHeatSP, nSP, nCoolSP);		
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

	   itemp=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];//183
	   //m_nightSlider.SetPos((int)(max-(itemp-min)));
	   int nCoolSP = (int)itemp;//(int)(max-(itemp-min));
	   strTemp.Format(_T("%.1f"),itemp);
	   m_nightInfoEdit.SetWindowText(strTemp);

	   itemp=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];//heat182
	   //m_nightHeatSlider.SetPos((int)(max-(itemp-min)));
	   int nHeatSP = (int)itemp;//((int)(max-(itemp-min)));
	   strTemp.Format(_T("%.1f"),itemp);
	   m_nightHeatInfoEdit.SetWindowText(strTemp);
	   m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
	   m_NightHeatStatic.SetWindowText(_T("Heating SP"));
	   //;GetDlgItem(IDC_NIGHTSTATIC)->SetWindowText(_T("Night Office"));
	   nSP = (nCoolSP-nHeatSP)/2 + nHeatSP;
	   //0913
	   // 		strTemp.Format(_T("%d"), nSP);
	   // 		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
	   // 		pSPWnd->SetWindowText(strTemp);

		m_nightpot = nSP;//0913


		if (nCoolSP == nHeatSP && nCoolSP== nSP )
		{
			nSP = nHeatSP + 1;
			nCoolSP = nSP+1;				
		}

		int nRangeMax = nCoolSP > max ? nCoolSP : max;
		int nRangeMin = nHeatSP < min ? nHeatSP : min;

		m_pNightTwoSP->SetRange(nRangeMax, nRangeMin);
		BOOL bSetPos = m_pNightTwoSP->SetPos(nHeatSP, nCoolSP, nCoolSP+1);		
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
		//nSP = multi_register_value[REG(MODBUS_COOLING_SETPOINT)]; // 135
		nSP = product_register_value[MODBUS_COOLING_SETPOINT]; // 135
		//int nCoolSP, nHeatSP;
		//itemp=multi_register_value[124]+the_setpoint_ha_ha;//cool
		//m_nightSlider.SetPos(max-(itemp-min));
		int nCoolDB = product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
		strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10);
		m_nightInfoEdit.SetWindowText(strTemp);

		//itemp=the_setpoint_ha_ha-multi_register_value[123];//heat
		//m_nightHeatSlider.SetPos(max-(itemp-min));
		int nHeatDB = product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));
		strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10);
		m_nightHeatInfoEdit.SetWindowText(strTemp);

		strTemp.Format(_T("%d"), nSP);
//0913
// 		CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
// 		pSPWnd->SetWindowText(strTemp);

		m_nightpot = nSP;//0913

 		m_NightCoolStatic.SetWindowText(_T("Cooling DB"));
 		m_NightHeatStatic.SetWindowText(_T("Heating DB"));

//0912
//		m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
//		m_NightHeatStatic.SetWindowText(_T("Heating SP"));
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

		m_pNightSingleSP->SetRange(nRangeMax, nRangeMin);
		BOOL bRetSP = m_pNightSingleSP->SetPos(nHeatSP, nSP, nCoolSP);		
		HandleSliderSetPos(bRetSP);
		m_pNightSingleSP->ShowWindow(SW_NORMAL);
		m_pNightTwoSP->ShowWindow(SW_HIDE);

	}



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

	/*
	int nCurNight=max-m_nightSlider.GetPos()+min;
	int nCurNightHeat=max-m_nightHeatSlider.GetPos()+min;

	if(nCurDay>=nCurNight-1)
	{
		nCurDay=nCurNight-1;
	}
	if(nCurDay<=nCurNightHeat)
	{
		nCurDay=nCurNightHeat+1;
	}
	*/

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
		write_one(g_tstat_id,MODBUS_RH_SETPOINT,nCurDay*10);//T6 380
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

/*
	int m_hotel_office=multi_register_value[125];
	if(m_hotel_office==0)
	{
		//setpoint  183,182
		write_one(g_tstat_id,183,nCurNight);
		write_one(g_tstat_id,182,nCurNightHeat);
	}
	else
	{

	}
	*/
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
	int hotel_or_office=product_register_value[MODBUS_APPLICATION];//125

	/*
	if(nCurNight<=nCurDay)
	{
		nCurNight=nCurDay+1;
	}
	*/

	m_nightSlider.SetPos(max-nCurNight+min);
	CString strTemp;
	strTemp.Format(_T("%d"),nCurNight);
	m_nightInfoEdit.SetWindowText(strTemp);
	
	if(hotel_or_office==0)
	{//setpoint
		write_one(g_tstat_id,MODBUS_NIGHT_COOLING_SETPOINT,nCurNight);//183
	}
	else
	{//deadband
		write_one(g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,(nCurNight-nCurDay));//124
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
	/*
	if(nCurNightHeat>=nCurDay)
	{
		nCurNightHeat=nCurDay-1;
	}
	*/
//	m_nightHeatSlider.SetPos(max-nCurNightHeat+min);
	CString strTemp;
	strTemp.Format(_T("%d"),nCurNightHeat);
	m_nightHeatInfoEdit.SetWindowText(strTemp);
	int hotel_or_office=product_register_value[MODBUS_APPLICATION];//125
	if(hotel_or_office==0)
	{
		//setpoint
		write_one(g_tstat_id,MODBUS_NIGHT_HEATING_SETPOINT,nCurNightHeat);//182
	}
	else
	{//deadband
		write_one(g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,nCurDay-nCurNightHeat);//123
	}
	*pResult = 0;
#endif
}



void CT3000View::Fresh_In()
{

	//int temp_address_map =  Mdb_Adress_Map;
	//Mdb_Adress_Map = T3000_5ABCDFG_LED_ADDRESS;

	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	CString strUnit=GetTempUnit();
	CString strTemp;
	strTemp.Empty();
	if( nModel==2||nModel==1||nModel==4||nModel==PM_TSTAT5D||nModel==17||nModel==18
		|| nModel==3||nModel==PM_TSTAT6||nModel==PM_TSTAT7)//A,B,C,D ,F,G,6,7
	{
		//strTemp.Format(_T("Input %d"),1);
		m_Input_Grid.put_TextMatrix(1,1,g_strSensorName);

		int x = MODBUS_INTERNAL_THERMISTOR;
		int y = MODBUS_INTERNAL_THERMISTOR;
		strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216 //130
		strTemp=strTemp+strUnit;
		m_Input_Grid.put_TextMatrix(1,2,strTemp);
		TRACE(_T("INPUT1-")+strTemp+_T("\n"));

		//strTemp.Format(_T("Input %d"),2);
		m_Input_Grid.put_TextMatrix(2,1,g_strInName1);
		strTemp.Empty();

		
			
		strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
		if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
		{	
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);//180
			strTemp=strTemp+strUnit;
		}
		if (product_register_value[MODBUS_ANALOG_IN1]==0||product_register_value[MODBUS_ANALOG_IN1]==2)//188
		{
			//strUnit=GetTempUnit(180);
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
		}
		if(product_register_value[MODBUS_ANALOG_IN1]==3)//188
		{
			if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//180
				strTemp=_T("ON");
			if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
				strTemp=_T("OFF");
		}

		if (nModel==18)//2.5.0.98
		{
			if (product_register_value[MODBUS_ANALOG_IN1] == 0)//188
			{
				strTemp = _T("UNUSED");
			}
		}

		m_Input_Grid.put_TextMatrix(2,2,strTemp);
		strTemp.Empty();
		
	//	strTemp.Format(_T("Iuput %d"),2);
	//	m_Input_Grid.put_TextMatrix(2,0,g_strInName3);
	//	strTemp.Empty();
	}
	//in3 and in4 
	strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN2],2);//189
	//if(nModel==4||nModel==12||nModel==18||nModel==17)//C,D,F,GnModel==4
	if(nModel==4||nModel==12||nModel==18||nModel==17||nModel == PM_TSTAT7)//tstat7
	{
		strTemp.Empty();
		if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//189
		{
	
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);	//181 
			strTemp=strTemp+strUnit;
		}
		if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)//189
		{
			//strUnit=GetTempUnit(181);
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//181
		}
		if(product_register_value[MODBUS_ANALOG_IN2]==3)//189
		{
			if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
				strTemp=_T("ON");
			if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
				strTemp=_T("OFF");
		}	

		if (nModel == 18)//2.5.0.98
		{
			if (product_register_value[MODBUS_ANALOG_IN2] == 0)//189
			{
				strTemp = _T("UNUSED");
			}
		}

		m_Input_Grid.put_TextMatrix(3,2,strTemp);
		strTemp.Empty();
		//strTemp.Format(_T("Input %d"),3);
		m_Input_Grid.put_TextMatrix(3,1,g_strInName2);
		strTemp.Empty();
	

		if (product_register_value[MODBUS_DIGITAL_IN1]==0)//190 
		{
			if (product_register_value[MODBUS_DIGITAL_INPUT]==0) //311
			{
				strTemp=_T("OFF");
			}else
			{
				strTemp=_T("ON");
			}
		}
		if (product_register_value[MODBUS_DIGITAL_IN1]==1) //190
		{
			if (product_register_value[MODBUS_DIGITAL_INPUT]==1) //311
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
	
	if(nModel == PM_TSTAT6 )// Tstat 6
	{

		//Mdb_Adress_Map = T3000_6_ADDRESS; //������ T3000_6_ADDRESS �� ���е�ַ Maping;
		/*
		for(int i=1;i++;i<=8)
		{
			strTemp.Format(_T("Input %d"),1);
			m_Input_Grid.put_TextMatrix(i,0,strTemp);
		}
		*/
		
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
		m_Input_Grid.put_TextMatrix(11,1,g_strInCO2);
		strTemp.Empty();
		if(m_fFirmwareVersion>33.3)
		{	   BOOL m_disable_hum,m_disable_CO2;
			float nValue;
			for(int i=0;i<9;i++)
			{
				if ( i == 0)
				{
					//101	121	2	Full	W/R	TEMPERATURE  reading in DegC or F from the sensor used in the control loop PI1 which is configured in register 111. This can be the internal sensor, external, or an average of the two. Writing a temperature value to this register will calibrate the currently selected sensor by adjusting the associated calibration register. If average is selected then you cannot write to this register.   
					//float fTemp= (float)(multi_register_value[216]/10.0);//tstat6 
					float fTemp= (float)(product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);//121
					strTemp.Format(_T("%.1f ��C"),fTemp);	
					//if (multi_register_value[359] == 1)
					//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1
					if (product_register_value[MODBUS_DEGC_OR_F] == 1) //104  
					{
						strTemp.Format(_T("%.1f ��F"),fTemp);	
					}									
				}
				else
				{

// 
// 						359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						2	123	1	Low byte	W/R	ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						3	124	1	Low byte	W/R	ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						4	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						5	126	1	Low byte	W/R	ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						6	127	1	Low byte	W/R	ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						7	128	1	Low byte	W/R	ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 						8	129	1	Low byte	W/R	ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON


// 
// 						367	131	2	Full	W/R	Analog input1 value
// 						10	132	2	Full	W/R	Analog input2 value
// 						11	133	2	Full	W/R	Analog input3 value
// 						12	134	2	Full	W/R	Analog input4 value
// 						13	135	2	Full	W/R	Analog input5 value
// 						14	136	2	Full	W/R	Analog input6 value
// 						15	137	2	Full	W/R	Analog input7 value
// 						16	138	2	Full	W/R	Analog input8 value

					//int yy =_G("MODBUS_ANALOG1_RANGE");
					if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==1) //122
					{
						nValue= (float)(product_register_value[MODBUS_ANALOG_INPUT1+i-1]/10.0); //131
						strTemp.Format(_T("%.1f"),nValue);
					}
					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==3) // On/Off
					{
						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//131
						strTemp= nValue ==1 ? _T("On") : _T("Off");
					}
					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==2)
					{
						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//131
						strTemp.Format(_T("%0.1f%%"),  nValue);
					}
					else if (product_register_value[MODBUS_ANALOG1_RANGE+i-1]==4 )  // custom sensor
					{	
						CString strValueUnit=GetTempUnit(product_register_value[122+i-1], 1);
						nValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);//131
						strTemp.Format(_T("%.1f"), (float)nValue/10.0);	
						strTemp +=strValueUnit;
					}
					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==5) // Off/On  //122
					{
						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];
						strTemp= nValue ==1 ? _T("On") : _T("Off");
					}
					else
					{
						//strTemp.Format(_T("%d"),multi_register_value[367+i-1]);//tatat6
						strTemp=_T("UNUSED");
					}

				}
				m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
			}
			
			if((product_register_value[20]&2)==2)
			{m_disable_hum=TRUE;}
			else
			{
				m_disable_hum=FALSE;
			}
			if((product_register_value[20]&4)==4){
				m_disable_CO2=TRUE;}
			else
			{
				m_disable_CO2=FALSE;
			}
		
				if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
				{

					strTemp.Format(_T("%0.1f%%"),(float)(product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10));
					m_Input_Grid.put_TextMatrix(10,2,strTemp);
				}
				else
				{

					strTemp.Format(_T("%0.1f%%"),(float)(product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10));
					m_Input_Grid.put_TextMatrix(10,2,strTemp);
				}
			
				if (product_register_value[607]==0)
				{

					strTemp.Format(_T("%d"),product_register_value[139]);
					m_Input_Grid.put_TextMatrix(11,2,strTemp);
				}
				else
				{

					strTemp.Format(_T("%d"),product_register_value[608]);
					m_Input_Grid.put_TextMatrix(11,2,strTemp);
				}
			
		
		}
		else
		{
			
			//Mdb_Adress_Map = 1; //������ T3000_5ABCDFG_LED_ADDRESS �� ���е�ַ Maping;
			float nValue;
			for(int i=0;i<8;i++)
			{
				if(product_register_value[MODBUS_OUTPUT_PWM_AUTO_COAST+i]==1)//341 
				{
					nValue=(float)(product_register_value[MODBUS_PWM_OUT5+i]/10.0);	//349
					strTemp.Format(_T("%.1f"),nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[MODBUS_PWM_OUT5+i]);
				}
				m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
			}

		}
	}

	
	////////////////
	if(nModel==PM_TSTAT5E) // E 
	{
		//Mdb_Adress_Map = T3000_5EH_LCD_ADDRESS; //������ T3000_5EH_LCD_ADDRESS �� ���е�ַ Maping;
		
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
		//m_Input_Grid.put_TextMatrix(10,1,g_strInHumName);



		//CString analog_range_0[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
		strTemp.Empty();
		if(m_fFirmwareVersion>33.3)
		{
			float nValue;
			for(int i=0;i<9;i++)
			{
				if ( i == 0)
				{
					//float fTemp= (float)(multi_register_value[216]/10.0);//tstat6 
					float fTemp= (float)(product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//101
					strTemp.Format(_T("%.1f ��C"),fTemp);	
					//if (multi_register_value[359] == 1)
					if (product_register_value[MODBUS_DEGC_OR_F] == 1)//121
					{
						strTemp.Format(_T("%.1f ��F"),fTemp);	
					}									
				}
				else
				{
					CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-1], 1);//359
					if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==1)//359
					{
						nValue= (float)(product_register_value[MODBUS_ANALOG_INPUT1+i-1]/10.0);//stat6 �����������ʾUNUSE��δʹ��  //367
						strTemp.Format(_T("%.1f"),nValue);//
						//strTemp=_T("UNUSED");
					}
					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==3) // On/Off		//359
					{
						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//367
						strTemp= nValue ==1 ? _T("On") : _T("Off");
					}
					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==2)//359
					{
						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//367
						strTemp.Format(_T("%0.1f%%"),  nValue);
					}
					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==5) // Off/On//359
					{
						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//367
						strTemp= nValue ==1 ? _T("On") : _T("Off");
					}else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]== 4) // Off/On//359
					{
// 						nValue=multi_register_value[367+i-1];
// 						strTemp= nValue ==1 ? _T("On") : _T("Off");


						nValue=float(product_register_value[MODBUS_ANALOG1_RANGE+i-1]/10.0);//359
						strTemp.Format(_T("%.1f"), (float)nValue/10.0);	
						strTemp +=strValueUnit;
					}else
					{
						//strTemp.Format(_T("%d"),multi_register_value[367+i-1]);//tatat6
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
				if(product_register_value[MODBUS_OUTPUT_PWM_AUTO_COAST+i]==1)//341
				{
					nValue=(float)(product_register_value[MODBUS_PWM_OUT5+i]/10.0);	//349
					strTemp.Format(_T("%.1f"),nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[MODBUS_PWM_OUT5+i]);//349
				}
				m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
			}

		} 

		
	}
	////////////////
 


	//if(nModel==PM_TSTAT6  )  // Tstat 6 // 2012-02-07, 5E�ͺ�����ʪ��
	//{
	//	//Mdb_Adress_Map = T3000_5EH_LCD_ADDRESS;
	//	

	//	m_Input_Grid.put_TextMatrix(10,1,g_strInHumName);
	//	DWORD dwHum = product_register_value[MODBUS_NEW_COOLING_SETPOINT];//422
	//	CString strHum;
	//	strHum.Format(_T("%0.1f%%"), dwHum/10.0);
	//	m_Input_Grid.put_TextMatrix(10,2,strHum);
	//}

	//if(nModel==19)//H
	//{

	//}

	if (nModel == PM_PRESSURE)
	{
		Mdb_Adress_Map = T3000_5ABCDFG_LED_ADDRESS;

		CString strRow1 = _T("Pressure sensor");
		m_Input_Grid.put_TextMatrix(1,1,strRow1);
		CString strTemp;
		strTemp.Format(_T("%.1f W.C"), product_register_value[MODBUS_EXTERNAL_SENSOR_0]/100.0);//180   
		m_Input_Grid.put_TextMatrix(1,2,strTemp);		
	}

	//Mdb_Adress_Map =temp_address_map  ;
}
void CT3000View::Fresh_Out()
{
//	Savetmp_Mdb_Adress_Map();
	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
//	switch (nModel)
	CString strTemp;
	int itemp=product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];
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
		int nRange=product_register_value[MODBUS_OUTPUT1_SCALE];
		if(nModel==12||nModel==16||nModel==18||nModel==PM_TSTAT6||nModel==PM_TSTAT7||nModel==PM_PRESSURE)
		{
			if(nRange==0)
			{
				//itemp=multi_register_value[102];
				itemp=product_register_value[MODBUS_COOLING_VALVE];
				/*
				if(itemp==0)
					strTemp=_T("OFF");
				if(itemp==1)
					strTemp=_T("ON");
					*/
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
					nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=float((10-product_register_value[MODBUS_COOLING_VALVE]/100.0)/10.0 *100);
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_Output_Grid.put_TextMatrix(6,2,strTemp);
			strTemp.Empty();


		//	strTemp.Format(_T("Output %d"),6);
			m_Output_Grid.put_TextMatrix(6,1,g_strOutName6);
			strTemp.Empty();
			
			nRange=product_register_value[MODBUS_OUTPUT2_SCALE];
			if(nRange==0)
			{
				itemp=product_register_value[MODBUS_HEATING_VALVE];
				
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
					nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/10.0);//T5 =102
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=(float)((10-product_register_value[MODBUS_HEATING_VALVE]/100)/10.0*100);
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
			 nRange=product_register_value[MODBUS_OUTPUT1_SCALE];
			if(nRange==0)
			{
				itemp=product_register_value[MODBUS_COOLING_VALVE];
				/*
				if(itemp==0)
					strTemp=_T("OFF");
				if(itemp==1)
					strTemp=_T("ON");
					*/
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
					nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=(float)((10-product_register_value[MODBUS_COOLING_VALVE]/100.0)/10.0 *100);
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			
			}
			m_Output_Grid.put_TextMatrix(4,2,strTemp);
			strTemp.Empty();
		//	strTemp.Format(_T("Output %d"),4);
			m_Output_Grid.put_TextMatrix(4,1,g_strOutName4);
			strTemp.Empty();
			nRange=product_register_value[MODBUS_OUTPUT2_SCALE];
			if(nRange==0)
			{
				itemp=product_register_value[MODBUS_HEATING_VALVE];
				
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
					nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/10.0);
				}
				if(nRange==2)//0-5v
				{
					nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/5.0);
				}
				if(nRange==3)//2-10v
				{
					nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/8.0);
				}
				if(nRange==4)//10-0v
				{
					nvalue=(float)((10-product_register_value[MODBUS_HEATING_VALVE]/100.0)/10.0 *100);
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
			
			if(product_register_value[MODBUS_MODE_OUTPUT4]==0) //283  MODBUS_MODE_OUTPUT4
			{
				if(itemp &( 1<<3))
						strTemp=_T("ON");
					else
						strTemp=_T("OFF");

			}
			else
			{
				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT4]);//348

			}
			m_Output_Grid.put_TextMatrix(4,2,strTemp);
			m_Output_Grid.put_TextMatrix(4,1,g_strOutName4);
			strTemp.Empty();	

			if(product_register_value[MODBUS_MODE_OUTPUT5]==0)//284
			{
				if(itemp &( 1<<4))
						strTemp=_T("ON");
					else
						strTemp=_T("OFF");

			}
			else
			{
				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//349

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
	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
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
		case PM_TSTAT5D:  // 5D ͬ TStat7
			{
				m_outRows=8;
				m_inRows=5;

			 }break;
		case PM_TSTAT6:
			{
				m_outRows=8;
				m_inRows=12;
			}break;
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
	((CMainFrame*)(theApp.m_pMainWnd))->SwitchToGraphicView();
	// TODO: Add your control notification handler code here
}

void CT3000View::OnBnClickedParameterbtn()
{
	g_bEnableRefreshTreeView = FALSE;
 //20121008
   	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;//tstat6
//   	pMain->m_pFreshMultiRegisters->SuspendThread();
   	pMain->m_pRefreshThread->SuspendThread();

	CParameterDlg dlg(this,m_strModelName);
	dlg.DoModal();
	Fresh();

//   	pMain->m_pFreshMultiRegisters->ResumeThread();//tstat6
  	pMain->m_pRefreshThread->ResumeThread();
	
	g_bEnableRefreshTreeView = TRUE;

	// TODO: Add your control notification handler code here
}

/*
void CT3000View::OnBnClickedOutputbtn()
{
	//g_tstat_id_changed=TRUE;
	COutPutDlg Dlg;
	Dlg.DoModal();
	//g_tstat_id_changed=FALSE;
}
*/

void CT3000View::OnBnClicked5ebutton()
{
	C5EInputDialog dlg;
	dlg.DoModal();
	
	// TODO: Add your control notification handler code here
}
BEGIN_EVENTSINK_MAP(CT3000View, CFormView)
	ON_EVENT(CT3000View, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CT3000View::ClickInputMsflexgrid, VTS_NONE)
	ON_EVENT(CT3000View, IDC_OUTPUT_MSFLEXGRID, DISPID_CLICK, CT3000View::ClickOutputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void CT3000View::ClickInputMsflexgrid()
{

	long lRow,lCol;
	lRow = m_Input_Grid.get_RowSel();//��ȡ������к�	
	lCol = m_Input_Grid.get_ColSel(); //��ȡ������к�
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_Input_Grid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_Input_Grid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_Input_Grid.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_Input_Grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_Input_Grid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rcCell(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_Input_Grid.get_TextMatrix(lRow,lCol);


	if(1==lCol && lRow != 1)
	{
		return; // 2012.2.7��ë˵�������޸�
		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //ȫѡ//

	}
	

}

void CT3000View::ClickOutputMsflexgrid()
{
	long lRow,lCol;
	lRow = m_Output_Grid.get_RowSel();//��ȡ������к�	
	lCol = m_Output_Grid.get_ColSel(); //��ȡ������к�

	TRACE(_T("Click output grid!\n"));

	CRect rect;
	m_Output_Grid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_Output_Grid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_Output_Grid.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_Output_Grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_Output_Grid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rcCell(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_Output_Grid.get_TextMatrix(lRow,lCol);


	if(1==lCol)
	{ 
		return; // don not permit change by customer
		m_outNameEdt.MoveWindow(&rcCell,1);
		m_outNameEdt.ShowWindow(SW_SHOW);
		m_outNameEdt.SetWindowText(strValue);
		m_outNameEdt.SetFocus();
		int nLenth=strValue.GetLength();
		m_outNameEdt.SetSel(nLenth,nLenth); //ȫѡ//
	}
}

void CT3000View::OnEnKillfocusInputnameedit()
{
	CString strText;
	m_inNameEdt.GetWindowText(strText);
	m_inNameEdt.ShowWindow(SW_HIDE);
	int lRow = m_Input_Grid.get_RowSel();//��ȡ������к�	
	int lCol = m_Input_Grid.get_ColSel(); //��ȡ������к�
	
	CString strInName;
	if(lCol!=1||lRow==0)
		return;
	switch (lRow)
	{
		case 1:
			strInName=g_strSensorName;
			break;
		case 2:
			strInName=g_strInName1;
			break;
		case 3:
			strInName=g_strInName2;
			break;
		case 4:
			strInName=g_strInName3;
			break;
		case 5:
			strInName=g_strInName4;
			break;
		case 6:
			strInName=g_strInName5;
			break;
		case 7:
			strInName=g_strInName6;
			break;
		case 8:
			strInName=g_strInName7;
			break;
		case 9:
			strInName=g_strInName8;
			break;	
		case 10:
			strInName=g_strInHumName;
			break;
	}
	if(strText.CompareNoCase(strInName)==0)
		return;
		

	//if(g_serialNum>0&&multi_register_value[6]>0)
	if(multi_register_value[6]>0)
	{
		try
		{
		_ConnectionPtr m_ConTmp;
		_RecordsetPtr m_RsTmp;
		m_ConTmp.CreateInstance("ADODB.Connection");
		m_RsTmp.CreateInstance("ADODB.Recordset");
		m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);

		CString strsql;
		strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
		m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		if(VARIANT_FALSE==m_RsTmp->EndOfFile)//update
		{
			
			CString strField;
			switch (lRow)
			{
				case 1:
					strField=_T("SENSORNAME");
					break;
				case 2:
					strField=_T("INPUT1");
					break;
				case 3:
					strField=_T("INPUT2");
					break;
				case 4:
					strField=_T("INPUT3");
					break;
				case 5:
					strField=_T("INPUT4");
					break;
				case 6:
					strField=_T("INPUT5");
					break;
				case 7:
					strField=_T("INPUT6");
					break;
				case 8:
					strField=_T("INPUT7");
					break;
				case 9:
					strField=_T("INPUT8");
					break;
				case 10:
					strField=_T("INPUT9");
					break;
			}

			try
			{

			CString str_temp;
			str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
			//AfxMessageBox(str_temp );
			m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			m_Input_Grid.put_TextMatrix(lRow,lCol,strText);

		}
		else//inerst
		{
			switch (lRow)
			{
				case 1:
					g_strSensorName = strText;
					break;
				case 2:
					g_strInName1=strText;
					break;
				case 3:
					g_strInName2=strText;
					break;
				case 4:
					g_strInName3=strText;
					break;
				case 5:
					g_strInName4=strText;
					break;
				case 6:
					g_strInName5=strText;
					break;
				case 7:
					g_strInName6=strText;
					break;
				case 8:
					g_strInName7=strText;
					break;
				case 9:
					g_strInName8=strText;
					break;
				case 10:
					g_strInHumName=strText;
					break;
			}

			CString	str_temp;
			str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
				strSerial,
				g_strInName1,
				g_strInName2,
				g_strInName3,
				g_strInName4,
				g_strInName5,
				g_strInName6,
				g_strInName7,
				g_strOutName1,
				g_strOutName2,
				g_strOutName3,
				g_strOutName4,
				g_strOutName5,
				g_strOutName6,
				g_strOutName7,
				g_strInName8,
				g_strInHumName,
				g_strSensorName
				);
			try
			{

				m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			m_Input_Grid.put_TextMatrix(lRow,lCol,strText);
		}
	
		switch (lRow)
		{
			case 1:
				g_strSensorName=strText;
				break;
			case 2:
				g_strInName1=strText;
				break;
			case 3:
				g_strInName2=strText;
				break;
			case 4:
				g_strInName3=strText;
				break;
			case 5:
				g_strInName4=strText;
				break;
			case 6:
				g_strInName5=strText;
				break;
			case 7:
				g_strInName6=strText;
				break;
			case 8:
				g_strInName7=strText;
				break;
			case 9:
				g_strInName8=strText;
				break;
			case 10:
				g_strInHumName=strText;
				break;
		}
		if(m_RsTmp->State) 
			m_RsTmp->Close(); 
		if(m_ConTmp->State)
			m_ConTmp->Close();	
		}
		catch(...)
		{

		}
	}
}

void CT3000View::OnEnKillfocusOutputnameedit()
{

	CString strText;
	m_outNameEdt.GetWindowText(strText);
	m_outNameEdt.ShowWindow(SW_HIDE);
	int lRow = m_Output_Grid.get_RowSel();//��ȡ������к�	
	int lCol = m_Output_Grid.get_ColSel(); //��ȡ������к�

	CString strInName;
	if(lCol!=1||lRow==0)
		return;
	switch (lRow)
	{
		case 1:
			strInName=g_strOutName1;
			break;
		case 2:
			strInName=g_strOutName2;
			break;
		case 3:
			strInName=g_strOutName3;
			break;
		case 4:
			strInName=g_strOutName4;
			break;
		case 5:
			strInName=g_strOutName5;
			break;
		case 6:
			strInName=g_strOutName6;
			break;
		case 7:
			strInName=g_strOutName7;
			break;
	}
	if(strText.CompareNoCase(strInName)==0)
		return;
		

	//if(g_serialNum>0&&multi_register_value[6]>0)
	if(product_register_value[MODBUS_ADDRESS]>0)//6
	{
	
		try
		{
		_ConnectionPtr m_ConTmp;
		_RecordsetPtr m_RsTmp;
		m_ConTmp.CreateInstance("ADODB.Connection");
		m_RsTmp.CreateInstance("ADODB.Recordset");
		m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);

		CString strsql;
		strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
		m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		if(VARIANT_FALSE==m_RsTmp->EndOfFile)//update
		{
			
			CString strField;
			switch (lRow)
			{
				case 1:
					strField=_T("OUTPUT1");
					break;
				case 2:
					strField=_T("OUTPUT2");
					break;
				case 3:
					strField=_T("OUTPUT3");
					break;
				case 4:
					strField=_T("OUTPUT4");
					break;
				case 5:
					strField=_T("OUTPUT5");
					break;
				case 6:
					strField=_T("OUTPUT6");
					break;
				case 7:
					strField=_T("OUTPUT7");
					break;
			}

			try
			{

			CString str_temp;
			str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
			//AfxMessageBox(str_temp );
			m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			m_Output_Grid.put_TextMatrix(lRow,lCol,strText);



		}
		else//inerst
		{
			switch (lRow)
			{
				case 1:
					g_strOutName1=strText;
					break;
				case 2:
					g_strOutName2=strText;
					break;
				case 3:
					g_strOutName3=strText;
					break;
				case 4:
					g_strOutName4=strText;
					break;
				case 5:
					g_strOutName5=strText;
					break;
				case 6:
					g_strOutName6=strText;
					break;
				case 7:
					g_strOutName7=strText;
					break;
			}

			CString	str_temp;
			str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
				strSerial,
				g_strInName1,
				g_strInName2,
				g_strInName3,
				g_strInName4,
				g_strInName5,
				g_strInName6,
				g_strInName7,
				g_strOutName1,
				g_strOutName2,
				g_strOutName3,
				g_strOutName4,
				g_strOutName5,
				g_strOutName6,
				g_strOutName7,
				g_strInName8,
				g_strInHumName,
				g_strSensorName
				);
			try
			{

				m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			m_Output_Grid.put_TextMatrix(lRow,lCol,strText);
		}

		switch (lRow)
		{
			case 1:
				g_strOutName1=strText;
				break;
			case 2:
				g_strOutName2=strText;
				break;
			case 3:
				g_strOutName3=strText;
				break;
			case 4:
				g_strOutName4=strText;
				break;
			case 5:
				g_strOutName5=strText;
				break;
			case 6:
				g_strOutName6=strText;
				break;
			case 7:
				g_strOutName7=strText;
				break;
		}
	
		if(m_RsTmp->State) 
			m_RsTmp->Close(); 
		if(m_ConTmp->State)
			m_ConTmp->Close();	



				}
				catch (...)
				{

				}
	}

	
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

	// ����ʱ�䡣
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

	((CMainFrame*)(theApp.m_pMainWnd))->SwitchToPruductType(3);
//	SwitchToPruductType(3);

	// TODO: Add your control notification handler code here
}

void CT3000View::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	TDelayForm Dlg;
	Dlg.DoModal();
}

void CT3000View::OnBnClickedLedbutton()
{
	CLedsDialog dialog;
	dialog.DoModal();

}

void CT3000View::OnBnClickedOccupacheck()
{

	CString strTemp;
	//stat6



	CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);
	if(m_OcupiedBtn.GetCheck()==BST_UNCHECKED)
	{
		write_one(g_tstat_id,MODBUS_CALIBRATION,0);	//184  109

		//Marked by Fance ,no need to judge
		//if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
		//	write_one(g_tstat_id,_G("MODBUS_CALIBRATION"),0);	//109
		//else
		//	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE"),0);	//184
		pBtn->SetCheck(1);
		product_register_value[MODBUS_CALIBRATION] = 0;//184
	}
		
	
	if(m_OcupiedBtn.GetCheck()==BST_CHECKED)
	{
		write_one(g_tstat_id,MODBUS_CALIBRATION,1);	//184  109
		//Marked by Fance ,no need to judge
		//if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
		//	write_one(g_tstat_id,_G("MODBUS_CALIBRATION"),1);//109
		//else
		//	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE"),1);//184

			product_register_value[MODBUS_INFO_BYTE] = 1;//184

		pBtn->SetCheck(0);
	}
		
	UpdateData(TRUE);
		//stat6
	Fresh();

}

void CT3000View::OnBnClickedUnoccupiedMark()
{
//stat6



	CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);

	if(pBtn->GetCheck()==BST_UNCHECKED)
	{

		write_one(g_tstat_id,MODBUS_INFO_BYTE,1);	//184  109
		//Marked by Fance ,no need to judge
		//if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
		//	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE",3),1);	//109
		//else
		//	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE"),1);	//184
			product_register_value[MODBUS_INFO_BYTE] = 1;//184
		m_OcupiedBtn.SetCheck(1);
	}

	if(pBtn->GetCheck()==BST_CHECKED)
	{
		write_one(g_tstat_id,MODBUS_INFO_BYTE,0);	//184  109
		product_register_value[MODBUS_INFO_BYTE] = 0;//184
		//if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
		//	write_one(g_tstat_id,109,0);
		//else
		//	write_one(g_tstat_id,184,0);

		//	multi_register_value[184]  = 0;
		m_OcupiedBtn.SetCheck(0);
	}

UpdateData(TRUE);
//stat6
Fresh();

}

void CT3000View::OnCbnSelchangeFanspeedcombo()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		//�ָ�T3000���߳�
		pMain->m_pFreshMultiRegisters->SuspendThread();
		pMain->m_pRefreshThread->SuspendThread();//
	int ret=0;

//	ret = write_one(g_tstat_id,MODBUS_FAN_SPEED,m_FanComBox.GetCurSel()); //t5=137  t6=273
	
	if (product_register_value[MODBUS_AUTO_ONLY]==1)
	{
		int sel=m_FanComBox.GetCurSel();
		if (sel==0)//OFF
		{
			int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,sel);
			if (ret>0)
			{

				product_register_value[MODBUS_FAN_SPEED]=0;
			}
			m_FanComBox.SetCurSel(0);
		} 
		else//Auto
		{
			int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,4);
			if (ret>0)
			{
				product_register_value[MODBUS_FAN_SPEED]=4;
			}
			m_FanComBox.SetCurSel(1);
		}
	} 
	else
	{
		int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,m_FanComBox.GetCurSel());
		if (ret>0)
		{

			product_register_value[MODBUS_FAN_SPEED]=m_FanComBox.GetCurSel();
		    m_FanComBox.SetCurSel(product_register_value[MODBUS_FAN_SPEED]);
		}
	}
	
	//if (!(ret>0))
	//{
	//	CString str;
	//	str.Format(_T("setting invalid,error:%d"),ret);
	//	AfxMessageBox(_T("setting invalid!"));
	//}

	//�ָ�T3000���߳�
	pMain->m_pFreshMultiRegisters->ResumeThread();
	pMain->m_pRefreshThread->ResumeThread();//
}


LRESULT CT3000View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
if(MsgT3000ViewFresh==message)
	{
		if((product_register_value[7] == PM_TSTAT6) || (product_register_value[7] == PM_TSTAT7) )
		{
			product_type =T3000_6_ADDRESS;
		}
		else if((product_register_value[7] == PM_TSTAT5E) || (product_register_value[7] == PM_TSTAT5H))
		{
			product_type = T3000_5EH_LCD_ADDRESS;
		}
		else if((product_register_value[7] == PM_TSTAT5A) ||(product_register_value[7] == PM_TSTAT5B) ||
			(product_register_value[7] ==PM_TSTAT5C ) || (product_register_value[7] == PM_TSTAT5D) || (product_register_value[7] == PM_TSTAT5F) ||
			(product_register_value[7] == PM_TSTAT5G))
		{
			product_type =T3000_5ABCDFG_LED_ADDRESS;
		}
	//	AfxMessageBox(_T("freshe!"));
//tstat6
#if 1
		if((multi_register_value[7] == PM_TSTAT6)||(multi_register_value[7] == PM_TSTAT7))
		//if (multi_register_value[7] == 6)
		{
			if ((multi_register_value[7] == PM_TSTAT6&&m_fFirmwareVersion>35.5)||(multi_register_value[7] == PM_TSTAT7))
			{
			
			//multi_register_value[]�б�����
			//��ȡTXT
		//	memset(newtstat6,0,sizeof(newtstat6));
			memcpy(newtstat6,multi_register_value,sizeof(newtstat6));

			memset(tempchange,0,sizeof(tempchange));
			int index = 0;

			for (int i = 0;i<512;i++)
			{
				index = reg_tstat6[i];
				tempchange[index] = multi_register_value[i];
			}

		//Fance	memcpy(multi_register_value,tempchange,sizeof(multi_register_value));
			}

		}//else if(m_strModelName.CompareNoCase(_T("Tstat6"))!=0)
		
#endif
//tstat6



		Fresh();
	}

	return CFormView::WindowProc(message, wParam, lParam);
}

void CT3000View::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	/*
	if(m_pSetPtDayDlg!=NULL)
	{
			CRect rcSetptWnd;
		m_SetptStatic.GetWindowRect(rcSetptWnd);
		rcSetptWnd.right=rcSetptWnd.left+450;
		rcSetptWnd.bottom=rcSetptWnd.top+600;
		m_pSetPtDayDlg->MoveWindow(&rcSetptWnd);
		m_pSetPtDayDlg->ShowWindow(SW_SHOW);
	}
	*/

}


void CT3000View::OnMove(int x, int y)
{
	CFormView::OnMove(x, y);
	/*
	if(m_pSetPtDayDlg!=NULL)
	{	
		CRect rcSetptWnd;
		m_SetptStatic.GetWindowRect(rcSetptWnd);
		rcSetptWnd.right=rcSetptWnd.left+450;
		rcSetptWnd.bottom=rcSetptWnd.top+600;
		m_pSetPtDayDlg->MoveWindow(&rcSetptWnd);
		m_pSetPtDayDlg->ShowWindow(SW_SHOW);
	}
	*/

	// TODO: Add your message handler code here
}

void CT3000View::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CFormView::OnWindowPosChanging(lpwndpos);

	/*
	if(m_pSetPtDayDlg!=NULL)
	{
		CRect rcSetptWnd;
		m_SetptStatic.GetWindowRect(rcSetptWnd);
		rcSetptWnd.right=rcSetptWnd.left+450;
		rcSetptWnd.bottom=rcSetptWnd.top+600;
		m_pSetPtDayDlg->MoveWindow(&rcSetptWnd);
		m_pSetPtDayDlg->ShowWindow(SW_SHOW);
	}
	*/
	

}

void CT3000View::OnMoving(UINT fwSide, LPRECT pRect)
{
	CFormView::OnMoving(fwSide, pRect);
	/*
	if(m_pSetPtDayDlg!=NULL)
	{
			CRect rcSetptWnd;
		m_SetptStatic.GetWindowRect(rcSetptWnd);
		rcSetptWnd.right=rcSetptWnd.left+450;
		rcSetptWnd.bottom=rcSetptWnd.top+600;
		m_pSetPtDayDlg->MoveWindow(&rcSetptWnd);
		m_pSetPtDayDlg->ShowWindow(SW_SHOW);
	}
	*/
}

LRESULT CT3000View::OnFreshView(WPARAM wParam, LPARAM lParam)
{
	Fresh();
	return 0;
}

//comments by Fan
//synchronization the time ,it's not use,this button is not visible.
void CT3000View::OnBnClickedBtnSynctime()
{
CString m_������=_T("zhangzhenzhong");
//AfxMessageBox(m_������);
#if 1
   CTime time = CTime::GetCurrentTime();
	BYTE szTime[8] ={0};
	
	BeginWaitCursor();
	GetDlgItem(IDC_BTN_SYNCTIME)->EnableWindow(FALSE);
	
	szTime[0] = 11;//(BYTE)(time.GetYear());
	int nRet = Write_One(g_tstat_id, 349, szTime[0]);
	szTime[1] = (BYTE)(time.GetMonth());
	nRet = Write_One(g_tstat_id, 350, szTime[1]);
	szTime[2] = (BYTE)(time.GetDayOfWeek()-1);
	nRet = Write_One(g_tstat_id, 351, szTime[2]);
	szTime[3] = (BYTE)(time.GetDay());
	nRet = Write_One(g_tstat_id, 352, szTime[3]);
	szTime[4] = (BYTE)(time.GetHour());
	nRet = Write_One(g_tstat_id, 353, szTime[4]);
	szTime[5] = (BYTE)(time.GetMinute());
	nRet = Write_One(g_tstat_id, 354, szTime[5]);
	szTime[6] = (BYTE)(time.GetSecond());
	nRet = Write_One(g_tstat_id, 355, szTime[6]);

	GetDlgItem(IDC_BTN_SYNCTIME)->EnableWindow(TRUE);
	EndWaitCursor();
#endif
	
	//int nRet = Write_Multi(g_tstat_id, szTime, 450, 8, 3);  // ���д�������⣬���Ա�ɵ����ֽ�д���
}

void CT3000View::OnBnClickedButtonSchedule()
{
	if(multi_register_value[7] != PM_TSTAT6)
	{
		AfxMessageBox(_T("This model of TStat don't support schedule!"));
		return;
	}
	g_bPauseMultiRead = TRUE;
	CTStatScheduleDlg dlg;
	dlg.DoModal();
	g_bPauseMultiRead = FALSE;
	
}




// ��������Ǹ��µ�Model��TStat6��TStat7ʹ��
// ʹ�����µļĴ������µļĴ����߼�
void CT3000View::InitFlexSliderBars()
{


//������E:\Tstat67\T3000\T3000
#if 1
	// init 3 thumb slider
	//CStatic *s = (CStatic*)GetDlgItem(IDC_FSB_OFFICE);

	// settings

	// set range of temperature	
	m_daySlider.ShowWindow(SW_HIDE);

	CString strTemp;
	int nRangeMin	=product_register_value[MODBUS_MIN_SETPOINT];//132
	int nRangeMax	=product_register_value[MODBUS_MAX_SETPOINT];//131
	//m_TemperaureSlider.SetPos((int)(nRangeMax-m_fTemperature+nRangeMin));//2.5.0.95
	m_TemperaureSlider.SetRange(nRangeMin,nRangeMax);

	if (g_unint)
		strTemp.Format(_T("%.1f��C"),m_fTemperature);
	else
		strTemp.Format(_T("%.1f��F"),m_fTemperature);
//	m_TempInfoEdit.SetWindowText(strTemp);	//2.5.0.95

	// calc 
	short nOccupied = product_register_value[MODBUS_INFO_BYTE];  // Day setpoint option  //184
	BOOL bOccupied = nOccupied & 0x0001;
	int nOfficeOrHotel = 0;

	int nCoolSP = 0;
	int nHeatSP = 0;
	int nSP =0;

	if (bOccupied) // day setpoint 
	{
		int nOfficeSelReg=454;
		if(product_register_value[MODBUS_PRODUCT_MODEL]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
		{
			nOfficeSelReg = 423;
		}
		nOfficeOrHotel = multi_register_value[nOfficeSelReg];	
		if (nOfficeOrHotel == 0) // office
		{			
			// 			nCoolSP = multi_register_value[379]/10;
			// 			nHeatSP = multi_register_value[136]/10;  // Ϊ�˺���ǰ���ݣ����Բ�����10
			// 		
			// 			if (nCoolSP == nHeatSP && nHeatSP == 0 )
			// 			{
			// 				nCoolSP = nHeatSP + 1;
			// 			}
			// 
			// 			//strTemp = _T("Occupied - Office");
			// 			strTemp = _T("Two SetPoint Mode");
			// 			CWnd *pModeWnd = GetDlgItem(IDC_STATIC_DAYSPMODE);
			// 			pModeWnd->SetWindowText(strTemp);
			// 
			// 			strTemp.Format(_T("%d"), (nCoolSP+nHeatSP)/2);
			// 			m_dayInfoEdit.SetWindowText(strTemp);
			nSP=product_register_value[MODBUS_COOLING_SETPOINT];//135
			nCoolSP = nSP + product_register_value[MODBUS_COOLING_DEADBAND]/10;//119
			nHeatSP = nSP - product_register_value[MODBUS_HEATING_DEADBAND]/10;//120
			// 		
			if (nCoolSP == nHeatSP && nHeatSP == 0 )
			{
				nCoolSP = nHeatSP + 1;
			}
			// 
			// 			//strTemp = _T("Occupied - Office");
//0903
// 			strTemp = _T("Two SetPoint Mode");
// 			CWnd *pModeWnd = GetDlgItem(IDC_STATIC_DAYSPMODE);
// 			pModeWnd->SetWindowText(strTemp);
			// 
			strTemp.Format(_T("%d"), nSP);
			m_dayInfoEdit.SetWindowText(strTemp);

			//CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			//pSPWnd->SetWindowText(strTemp);	

			m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
			m_DayHeatStatic.SetWindowText(_T("Heating SP"));
			strTemp.Format(_T("%d"), nCoolSP);
			m_DayCoolEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), nHeatSP);
			m_DayHeatEdit.SetWindowText(strTemp);


			//m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
			//m_NightHeatStatic.SetWindowText(_T("Heating SP"));


			nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
			nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;

			m_pDayTwoSP->SetRange(nRangeMax, nRangeMin);
			BOOL bRetSP = m_pDayTwoSP->SetPos( nHeatSP, nCoolSP, nCoolSP+1);	

			HandleSliderSetPos(bRetSP);//tstat6

			m_pDayTwoSP->ShowWindow(SW_NORMAL);
			m_pDaySingleSP->ShowWindow(SW_HIDE);


			//stat6
			m_pNightTwoSP->ShowWindow(SW_HIDE);
			m_pNightSingleSP->ShowWindow(SW_HIDE);
			strTemp.Format(_T("%d"), 0);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), 0);
			m_nightHeatInfoEdit.SetWindowText(strTemp);

			strTemp.Format(_T("%d"),0);
			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			pSPWnd->SetWindowText(strTemp);



		}
		else  // hotel
		{
			//strTemp = _T("Occupied - Hotel");
			//0903
// 			strTemp = _T("Single SetPoint Mode");
// 			CWnd *pModeWnd = GetDlgItem(IDC_STATIC_DAYSPMODE);
// 			pModeWnd->SetWindowText(strTemp);


			int nCoolingDBRegister = 119;
			int nHeatingDBRegister = MODBUS_HEATING_DEADBAND;
			//int nSPRegister = 380;	
			int nCoolDeadband = product_register_value[MODBUS_COOLING_DEADBAND];//119
			nSP = product_register_value[MODBUS_NEW_COOLING_SETPOINT];	//380
			nCoolSP = product_register_value[MODBUS_COOLING_DEADBAND] + nSP;
			if(product_register_value[MODBUS_PRODUCT_MODEL]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
			{
				nCoolingDBRegister = 422;
				nHeatingDBRegister = MODBUS_HEATING_SETPOINT;//136
				//nSPRegister = 135;
				nSP = product_register_value[MODBUS_COOLING_SETPOINT];//135
				nCoolDeadband = product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10;//422
				nCoolSP = nCoolDeadband + nSP;
			}


			nHeatSP = nSP - multi_register_value[nHeatingDBRegister];
			if (nCoolSP == nHeatSP && nHeatSP == 0)
			{
				nSP = nHeatSP + 1;
				nCoolSP = nSP+1;	
			}

			strTemp.Format(_T("%d"), nSP);
			//CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			//pSPWnd->SetWindowText(strTemp);
			m_dayInfoEdit.SetWindowText(strTemp);

			strTemp.Format(_T("%d"), nCoolDeadband);
			m_DayCoolEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), multi_register_value[nHeatingDBRegister]);
			m_DayHeatEdit.SetWindowText(strTemp);
			//0903
// 			m_DayCoolStatic.SetWindowText(_T("Cooling DB"));
// 			m_DayHeatStatic.SetWindowText(_T("Heating DB"));

			m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
			m_NightHeatStatic.SetWindowText(_T("Heating SP"));

			//////////////////////////////////////////////////////////////////////////
			nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
			nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
			m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);

			BOOL bRetSP = m_pDaySingleSP->SetPos(nHeatSP, nSP, nCoolSP);		
				
		//	BOOL bRetSP = m_pDayTwoSP->SetPos(nHeatSP, nCoolSP, nCoolSP+1); // ��Ϊֻ��2��thumb������sp��Ҫ�ˣ���Ϊ���˵�ֵ		
			HandleSliderSetPos(bRetSP);//tstat6

			m_pDaySingleSP->ShowWindow(SW_NORMAL);
			m_pDayTwoSP->ShowWindow(SW_HIDE);

			//stat6
			m_pNightTwoSP->ShowWindow(SW_HIDE);
			m_pNightSingleSP->ShowWindow(SW_HIDE);
			strTemp.Format(_T("%d"), 0);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), 0);
			m_nightHeatInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),0);
			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			pSPWnd->SetWindowText(strTemp);
		}
	}
	else  // unoccupied
	{
		nOfficeOrHotel = product_register_value[MODBUS_APPLICATION];	//125
		if (nOfficeOrHotel == 0) // office
		{			
			//strTemp = _T("unoccupied - Office");
			//0903
// 			strTemp = _T("Two SetPoint Mode");
// 
// 			CWnd *pModeWnd = GetDlgItem(IDC_STATIC_SPMODE);
// 			pModeWnd->SetWindowText(strTemp);
			nCoolSP = product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10;//183
			nHeatSP = product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10;//182

			if (nCoolSP == nHeatSP && nHeatSP == 0)
			{
				nCoolSP = nHeatSP + 1;
			}

			strTemp.Format(_T("%d"), (nCoolSP + nHeatSP)/2);
			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			pSPWnd->SetWindowText(strTemp);

			strTemp.Format(_T("%d"), nCoolSP);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), nHeatSP);
			m_nightHeatInfoEdit.SetWindowText(strTemp);

			m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
			m_NightHeatStatic.SetWindowText(_T("Heating SP"));

			nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
			nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
			m_pNightTwoSP->SetRange(nRangeMax, nRangeMin);
			BOOL bRetSP = m_pNightTwoSP->SetPos( nHeatSP, nCoolSP,nCoolSP+1);			
			HandleSliderSetPos(bRetSP);//tstat6
			m_pNightTwoSP->ShowWindow(SW_NORMAL);
			m_pNightSingleSP->ShowWindow(SW_HIDE);



			//stat6

			strTemp.Format(_T("%d"), 0);
			m_DayCoolEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), 0);
			m_DayHeatEdit.SetWindowText(strTemp);
			m_pDaySingleSP->ShowWindow(SW_HIDE);
			m_pDayTwoSP->ShowWindow(SW_HIDE);

			strTemp.Format(_T("%d"), 0);
			m_dayInfoEdit.SetWindowText(strTemp);

		}
		else  // hotel
		{
			//strTemp = _T("unoccupied - Hotel");
			//0903
// 			strTemp = _T("Single SetPoint Mode");
// 			CWnd *pModeWnd = GetDlgItem(IDC_STATIC_SPMODE);
// 			pModeWnd->SetWindowText(strTemp);
			int nCoolingRegister = MODBUS_NIGHT_COOLING_DEADBAND;//124
			int nHeatingRegister = MODBUS_NIGHT_HEATING_DEADBAND;//123

			int nSPRegister = MODBUS_NEW_COOLING_SETPOINT;	//380
			nSP = multi_register_value[nSPRegister]/10;

			if(product_register_value[MODBUS_PRODUCT_MODEL]==PM_TSTAT5E && m_fFirmwareVersion >= 34.09)
			{	
				nSPRegister = MODBUS_COOLING_SETPOINT;//135
				nSP = multi_register_value[nSPRegister];
			}

			nCoolSP = multi_register_value[nCoolingRegister] + nSP;
			nHeatSP = nSP - multi_register_value[nHeatingRegister];



			if (nCoolSP == nHeatSP && nHeatSP == 0)
			{
				nSP = nHeatSP + 1;
				nCoolSP = nSP+1;				
			}

			strTemp.Format(_T("%d"), nSP);
			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			pSPWnd->SetWindowText(strTemp);

			strTemp.Format(_T("%d"), multi_register_value[nCoolingRegister]);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), multi_register_value[nHeatingRegister]);
			m_nightHeatInfoEdit.SetWindowText(strTemp);
//0903
// 			m_NightCoolStatic.SetWindowText(_T("Cooling DB"));
// 			m_NightHeatStatic.SetWindowText(_T("Heating DB"));

			m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
			m_NightHeatStatic.SetWindowText(_T("Heating SP"));

			nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
			nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
			m_pNightSingleSP->SetRange(nRangeMax, nRangeMin);
			BOOL bRetSP =  m_pNightSingleSP->SetPos( nHeatSP, nSP, nCoolSP);
			HandleSliderSetPos(bRetSP);//tstat6


			m_pNightSingleSP->ShowWindow(SW_NORMAL);
			m_pNightTwoSP->ShowWindow(SW_HIDE);


			//stat6

			strTemp.Format(_T("%d"), 0);
			m_DayCoolEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), 0);
			m_DayHeatEdit.SetWindowText(strTemp);
			m_pDaySingleSP->ShowWindow(SW_HIDE);
			m_pDayTwoSP->ShowWindow(SW_HIDE);
			strTemp.Format(_T("%d"), 0);
			m_dayInfoEdit.SetWindowText(strTemp);
		}
	}

#endif
}


LRESULT CT3000View::OnFlexSlideCallBack(WPARAM wParam, LPARAM lParam)
{

	//HWND  Hret =m_pDayTwoSP->m_hWnd;//GetSafeHwnd( );
	//int rett = m_pNightTwoSP->Getflag();
	int nMin, nMax;
	int nCoolSP = 0;
	int nHeatSP = 0;
	int nSP =0;

	if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
	{
		switch(wParam)
		{
		case 1://�Ҳ� ���� 2SP
			{
				//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
				//352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
				//353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
				vector<int>  szPos(2);
				m_pNightTwoSP->GetRange(nMin, nMax);
				int nNum = m_pNightTwoSP->GetPos(szPos);
				// 			int nCoolSP = szPos[0] + nMin;
				// 			int nHeatSP= szPos[1] + nMin;

				//0910
				int nHeatSP = szPos[0] + nMin;
				int nCoolSP= szPos[1] + nMin;

				//0904
				// 			tstat6flex[NcoolDB] =nCoolSP;
				// 			tstat6flex[NheatDB] = nHeatSP;


				//tstat6flex[NightSP] = nSP;
				tstat6flex[NcoolSP] =nCoolSP;
				tstat6flex[NheatSP] = nHeatSP;

				int ret=0;

				CString strTemp;


				//0910
				strTemp.Format(_T("%d"),nCoolSP);
				m_nightInfoEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"),nHeatSP);
				m_nightHeatInfoEdit.SetWindowText(strTemp);


				BeginWaitCursor();
				//0904
				// 			ret =write_one(g_tstat_id, 353, nCoolSP*10,5);
				// 			ret =write_one(g_tstat_id, 352, nHeatSP*10,5);


				ret =write_one(g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, nHeatSP*10,5);//t6 354
				ret =write_one(g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT,nCoolSP*10,5);//t6 355
				EndWaitCursor();
				break;
			}
		case 2://�Ҳ� ���� 1SP
			{
				//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
				//354	2	Full	W/R	(Night)Unoccupied heating setpoint
				//355	2	Full	W/R	(Night)Unoccupied cooling setpoint

				vector<int>  szPos(3);
				m_pNightSingleSP->GetRange(nMin, nMax);
				int nNum = m_pNightSingleSP->GetPos(szPos);
				// 			int nCoolSP = szPos[0] + nMin;
				// 			int nSP = szPos[1] + nMin;
				// 			int nHeatSP= szPos[2] + nMin;

				//0910
				int nHeatSP = szPos[0] + nMin;
				int nSP = szPos[1] + nMin;
				int nCoolSP= szPos[2] + nMin;

				// #define  DaySP		0  //address 345
				// #define  DcoolDB	1  //address 346
				// #define  DheatDB	2  //address 347
				// #define  DcoolSP	3  //address 348
				// #define  DheatSP	4  //address 349
				// #define  NightSP	5  //address 350
				// #define  NheatDB	6  //address 352
				// #define  NcoolDB	7  //address 353
				// #define  NheatSP	8  //address 354
				// #define  NcoolSP	9  //address 355
				// #define  Max		10  //address 365
				// #define  Min		11  //address 366



				tstat6flex[NightSP] = nSP;
				tstat6flex[NcoolSP] =nCoolSP;
				tstat6flex[NheatSP] = nHeatSP;

				CString strTemp;
				strTemp.Format(_T("%d"), nSP);
				CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
				pSPWnd->SetWindowText(strTemp);

				// 			strTemp.Format(_T("%d"),nHeatSP);
				// 			m_nightInfoEdit.SetWindowText(strTemp);
				// 			strTemp.Format(_T("%d"),nCoolSP);
				// 			m_nightHeatInfoEdit.SetWindowText(strTemp);
				//0910
				strTemp.Format(_T("%d"),nCoolSP);
				m_nightInfoEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"),nHeatSP);
				m_nightHeatInfoEdit.SetWindowText(strTemp);
				BeginWaitCursor();

				int ret=0;
				ret =write_one(g_tstat_id, MODBUS_NIGHT_SETPOINT, nSP*10,5);//t6 350
				ret =write_one(g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, nHeatSP*10,5);//t6 354
				ret =write_one(g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT,nCoolSP*10,5);//t6 355

				EndWaitCursor();

				break;
			}

		case 3://��� ���� 2SP
			{

				// 		345	1	Low byte	W/R	(Day)Occupied   setpoint 
				// 		346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
				// 		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
				vector<int>  szPos(2);
				m_pDayTwoSP->GetRange(nMin, nMax);
				int nNum = m_pDayTwoSP->GetPos(szPos);
				// 			int nCoolSP = szPos[0] + nMin;
				// 			int nHeatSP = szPos[1] + nMin;
				//0910
				int nHeatSP = szPos[0] + nMin;
				int nCoolSP = szPos[1] + nMin;
				//0904
				// 			tstat6flex[DcoolDB] =nCoolSP;
				// 			tstat6flex[DheatDB] = nHeatSP;


				tstat6flex[DcoolSP] =nCoolSP;
				tstat6flex[DheatSP] = nHeatSP;



				int ret=0,succenum=0;

				CString strTemp;
				// 			strTemp.Format(_T("%d"), nHeatSP);
				// 			m_DayCoolEdit.SetWindowText(strTemp);
				// 			strTemp.Format(_T("%d"), nCoolSP);
				// 			m_DayHeatEdit.SetWindowText(strTemp);

				//0910
				strTemp.Format(_T("%d"),nCoolSP );
				m_DayCoolEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"),nHeatSP );
				m_DayHeatEdit.SetWindowText(strTemp);

				BeginWaitCursor();
				ret =write_one(g_tstat_id, MODBUS_DAY_COOLING_SETPOINT,nCoolSP*10,5);//t6 348;
				//	ret =write_one(g_tstat_id, 345,nSP*10 ,5);
				ret =write_one(g_tstat_id, MODBUS_DAY_HEATING_SETPOINT,nHeatSP*10,5);//t6 349;


				EndWaitCursor();

				break;
			}

		case 4://��� ���� 1SP
			{

				//348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
				//349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)

				vector<int>  szPos(3);
				m_pDaySingleSP->GetRange(nMin, nMax);
				int nNum = m_pDaySingleSP->GetPos(szPos);
				// 			int nCoolSP = szPos[0] + nMin;//��С
				// 			int nSP = szPos[1] + nMin;
				// 			int nHeatSP = szPos[2] + nMin;//���

				//0910
				int nHeatSP = szPos[0] + nMin;//��С
				int nSP = szPos[1] + nMin;
				int  nCoolSP= szPos[2] + nMin;//���

				tstat6flex[DaySP] = nSP;
				tstat6flex[DcoolSP] =nCoolSP;
				tstat6flex[DheatSP] = nHeatSP;

				CString strTemp;
				strTemp.Format(_T("%d"), nSP);
				m_dayInfoEdit.SetWindowText(strTemp);

				// 			strTemp.Format(_T("%d"), nHeatSP);
				// 			m_DayCoolEdit.SetWindowText(strTemp);
				// 			strTemp.Format(_T("%d"), nCoolSP);
				// 			m_DayHeatEdit.SetWindowText(strTemp);
				//0910

				strTemp.Format(_T("%d"), nCoolSP);
				m_DayCoolEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"), nHeatSP);
				m_DayHeatEdit.SetWindowText(strTemp);

				BeginWaitCursor();
				int ret1=0;int ret2=0;int ret3=0;
				ret1 =write_one(g_tstat_id, MODBUS_DAY_COOLING_SETPOINT,nCoolSP*10,5);//348  t6
				ret2 =write_one(g_tstat_id, MODBUS_DAY_SETPOINT,nSP*10 ,5);//345 T6
				ret3 =write_one(g_tstat_id, MODBUS_DAY_HEATING_SETPOINT,nHeatSP*10,5);//349 T6
				if((ret1<0)||(ret2<0)||(ret2<0))
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);}
				EndWaitCursor();



				break;
			}
			return 1;


		}
	}else
	{
		//0907
		CFSBContainer* pFSW = (CFSBContainer*)(wParam);
		int nModel = product_register_value[MODBUS_PRODUCT_MODEL];//7
		OnFlexSlideCallBackFor5ABCD(pFSW);// �����ͺ�A��H�ļ��ݣ�5E version(34.1 or 35.5)
		return 1;
#if 0
		int nMin, nMax;

		if(multi_register_value[7] != PM_TSTAT7 && multi_register_value[7] != PM_TSTAT6 )
		{
			if(multi_register_value[7] == PM_TSTAT5E)
				OnFlexSlideCallBackFor5E();// ����5E�Լ�����Ĳ�Ʒ�ļ���
			else
				OnFlexSlideCallBackFor5ABCD();// ����5E�Լ�����Ĳ�Ʒ�ļ���
			return 1;
		}

		BeginWaitCursor();



		short nOccupied = multi_register_value[184];  // Day setpoint option  
		BOOL bOccupied = nOccupied & 0x0001;
		int nOfficeOrHotel = 0;

		int nCoolSP = 0;
		int nHeatSP = 0;
		int nSP =0;

		if (bOccupied) // day setpoint 
		{
			nOfficeOrHotel = multi_register_value[454];	
			if (nOfficeOrHotel == 0) // office
			{			
				vector<int>  szPos(2);
				m_pDayTwoSP->GetRange(nMin, nMax);
				int nNum = m_pDayTwoSP->GetPos(szPos);
				int nHeatSP = szPos[0] + nMin;
				int nCoolSP = szPos[1] + nMin;

				int ret=0;
				if (((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)&&m_fFirmwareVersion >35.5)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
				{
					ret = write_one(g_tstat_id, 347, nCoolSP*10,5);
					ret = write_one(g_tstat_id, 348, nHeatSP*10,5);

				}else
				{
					write_one(g_tstat_id, 379, nCoolSP*10);
					write_one(g_tstat_id, 136, nHeatSP*10);
				}


				//write_one(g_tstat_id, 183, nCoolSP*10);
				//write_one(g_tstat_id, 182, nHeatSP);
				//stat6
				CString strTemp;
				strTemp.Format(_T("%d"), nCoolSP*10);
				m_DayCoolEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"),  nHeatSP*10);
				m_DayHeatEdit.SetWindowText(strTemp);




			}
			else  // hotel
			{
				vector<int>  szPos(3);
				m_pDaySingleSP->GetRange(nMin, nMax);
				int nNum = m_pDaySingleSP->GetPos(szPos);
				int nHeatSP = szPos[0] + nMin;
				int nSP = szPos[1] + nMin;
				int nCoolSP = szPos[2] + nMin;

				int ret=0;
				if (((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)&&m_fFirmwareVersion >35.5)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
				{
					ret =write_one(g_tstat_id, 347, nSP*10,5);//?
					ret =write_one(g_tstat_id, 345, nCoolSP - nSP,5);
					ret =write_one(g_tstat_id, 346, nSP - nHeatSP,5);

				}else
				{
					write_one(g_tstat_id, 380, nSP*10);
					write_one(g_tstat_id, 119, nCoolSP - nSP);
					write_one(g_tstat_id, 120, nSP - nHeatSP);
				}

				CString strTemp;
				strTemp.Format(_T("%d"), nSP*10);
				m_dayInfoEdit.SetWindowText(strTemp);

				strTemp.Format(_T("%d"), nCoolSP - nSP);
				m_DayCoolEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"),  nSP - nHeatSP);
				m_DayHeatEdit.SetWindowText(strTemp);

			}
		}
		else  // unoccupied
		{
			nOfficeOrHotel = multi_register_value[125];	
			if (nOfficeOrHotel == 0) // office
			{		
				vector<int>  szPos(2);
				m_pNightTwoSP->GetRange(nMin, nMax);
				int nNum = m_pNightTwoSP->GetPos(szPos);
				int nHeatSP = szPos[0] + nMin;
				int nCoolSP = szPos[1] + nMin;

				//write_one(g_tstat_id, 379, nCoolSP*10);
				//write_one(g_tstat_id, 136, nHeatSP);

				int ret=0;
				if (((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)&&m_fFirmwareVersion >35.5)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
				{
					ret =write_one(g_tstat_id, 354, nCoolSP,5);
					ret =write_one(g_tstat_id, 353, nHeatSP,5);

				}else
				{
					write_one(g_tstat_id, 183, nCoolSP,5);
					write_one(g_tstat_id, 182, nHeatSP,5);
				}

				CString strTemp;

				strTemp.Format(_T("%d"), nCoolSP);
				m_nightInfoEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"), nHeatSP);
				m_nightHeatInfoEdit.SetWindowText(strTemp);


			}
			else  // hotel
			{
				vector<int>  szPos(3);
				m_pNightSingleSP->GetRange(nMin, nMax);
				int nNum = m_pNightSingleSP->GetPos(szPos);
				int nHeatSP = szPos[0] + nMin;
				int nSP = szPos[1] + nMin;
				int nCoolSP = szPos[2] + nMin;

				int ret=0;
				if (((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)&&m_fFirmwareVersion >35.5)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
				{
					ret =write_one(g_tstat_id, 347, nSP*10,5);//?
					ret =write_one(g_tstat_id, 351, nCoolSP - nSP,5);
					ret =write_one(g_tstat_id, 352, nSP - nHeatSP,5);

				}else
				{
					write_one(g_tstat_id, 380, nSP*10);
					write_one(g_tstat_id, 124, nCoolSP - nSP);
					write_one(g_tstat_id, 123, nSP - nHeatSP);
				}
				CString strTemp;
				strTemp.Format(_T("%d"), nSP*10);
				CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
				pSPWnd->SetWindowText(strTemp);

				strTemp.Format(_T("%d"), nCoolSP - nSP);
				m_nightInfoEdit.SetWindowText(strTemp);
				strTemp.Format(_T("%d"),  nSP - nHeatSP);
				m_nightHeatInfoEdit.SetWindowText(strTemp);

			}
		}


		//InitFlexSliderBars();//tstat6	
		EndWaitCursor();

		//tstat6
		// 	pMain->m_pFreshMultiRegisters->ResumeThread();
		// 	pMain->m_pRefreshThread->ResumeThread();


		return 1;

#endif
	}
	return 1;
}


// ����5E�Լ�����Ĳ�Ʒ�ļ���
void CT3000View::OnFlexSlideCallBackFor5E()
{
	///*
	BeginWaitCursor();

	int nMin, nMax;
	CString strTemp;
	int min=multi_register_value[132];
	int max=multi_register_value[131];

	int m_hotel_or_office=multi_register_value[125];///////hotel or office

	if (m_hotel_or_office == 0) // office
	{			
		vector<int>  szPos(3);
		m_pNightSingleSP->GetRange(nMin, nMax);
		int nNum = m_pNightSingleSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		int nSP = szPos[1] + nMin;
		int nCoolSP = szPos[2] + nMin;

				int nStart = GetTickCount();
		write_one(g_tstat_id, 135, nSP);	
		
		int nEnd = GetTickCount();
		write_one(g_tstat_id, 183, nCoolSP);
		write_one(g_tstat_id, 182, nHeatSP);

	
		CString str;
		str.Format(_T("Time1 elapsed : %d \n"), nEnd-nStart);
		TRACE(str);
	}
	else  // hotel
	{
			vector<int>  szPos(3);
			m_pNightSingleSP->GetRange(nMin, nMax);
			int nNum = m_pNightSingleSP->GetPos(szPos);
			int nHeatSP = szPos[0] + nMin;
			int nSP = szPos[1] + nMin;
			int nCoolSP = szPos[2] + nMin;
			write_one(g_tstat_id, 135, nSP);
			write_one(g_tstat_id, 124, nCoolSP - nSP);
			write_one(g_tstat_id, 123, nSP - nHeatSP);
		
	}



	InitSliderBars2();
	
// 	int nEnd2 = GetTickCount();
// 	CString str1.Format(_T("Time2 elapsed : %d \n"), nEnd2-nEnd);
// 	TRACE(str1);
	
	EndWaitCursor();
	//*/
}

// ����5E����������ͺ�
void CT3000View::OnFlexSlideCallBackFor5ABCD( CFSBContainer* pSlider )
{
#if 0
	BeginWaitCursor();

	int nMin, nMax;
	CString strTemp;
	int min=multi_register_value[132];
	int max=multi_register_value[131];

	int m_hotel_or_office=multi_register_value[125];///////hotel or office
	short nOccupied = multi_register_value[184];  // Day setpoint option  
	BOOL bOccupied = nOccupied & 0x0001;


	if (m_hotel_or_office == 0 && bOccupied) // office - occupied : 2SP
	{			
		vector<int>  szPos(3);
		m_pDayTwoSP->GetRange(nMin, nMax);
		int nNum = m_pDayTwoSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		//int nSP = szPos[1] + nMin;
		int nSP = multi_register_value[374]/10;
		int nCoolSP = szPos[1] + nMin;
		//write_one(g_tstat_id, 374, (nCoolSP-nHeatSP)/2 + nHeatSP);	

		// SP �ǲ���ġ�ֻ��ı�db
 		write_one(g_tstat_id, 119, (nCoolSP - nSP)*10);
 		write_one(g_tstat_id, 120, (nSP - nHeatSP)*10);
	}
	
	if (m_hotel_or_office == 1 && bOccupied) // hotel - occupied  //  1SP
	{
		vector<int>  szPos(3);
		m_pDaySingleSP->GetRange(nMin, nMax);
		int nNum = m_pDaySingleSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		int nSP = szPos[1] + nMin;
		//int nSP = multi_register_value[374]/10;
		int nCoolSP = szPos[2] + nMin;
					
		write_one(g_tstat_id, 374, nSP*10);	
		// SP �ǲ���ġ�ֻ��ı�db
		write_one(g_tstat_id, 380, nCoolSP*10 );
		write_one(g_tstat_id, 136, nHeatSP);

	}

	if (m_hotel_or_office == 0 && !bOccupied) // office - unoccupied
	{  //2sp
		vector<int>  szPos(3);
		m_pNightTwoSP->GetRange(nMin, nMax);
		int nNum = m_pNightTwoSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		//int nSP = szPos[1] + nMin;
		int nSP = multi_register_value[374]/10;
		int nCoolSP = szPos[1] + nMin;
		//write_one(g_tstat_id, 374, (nCoolSP-nHeatSP)/2 + nHeatSP);	

		// SP �ǲ���ġ�ֻ��ı�db
		write_one(g_tstat_id, 183, nCoolSP);
		write_one(g_tstat_id, 182, nHeatSP);
	}

	if (m_hotel_or_office == 1 && !bOccupied) // hotel - unoccupied
	{ //1sp
		vector<int>  szPos(3);
		m_pNightSingleSP->GetRange(nMin, nMax);
		int nNum = m_pNightSingleSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		int nSP = szPos[1] + nMin;
		//int nSP = multi_register_value[374]/10;
		int nCoolSP = szPos[2] + nMin;

		write_one(g_tstat_id, 374, nSP*10);	
		// SP �ǲ���ġ�ֻ��ı�db
		write_one(g_tstat_id, 124, nCoolSP );
		write_one(g_tstat_id, 123, nHeatSP);
	}

	InitSliderBars2();//0907
	//InitSliderBars();//0907
	EndWaitCursor();

#endif
	// Ŀǰʵ�ֵ�����Runar5E��ver ��34.1 and ver 35.5 Ϊ����.
//0907����
	BeginWaitCursor();

	int nMin, nMax;
	CString strTemp;
	int min=multi_register_value[132];
	int max=multi_register_value[131];

	int m_hotel_or_office=0;
	short nOccupied = multi_register_value[184];  // Day setpoint option  
	BOOL bOccupied = nOccupied & 0x0001;


	if (bOccupied)
	{	
		int nSelReg=454;
		if(multi_register_value[7]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
		{
			nSelReg = 423;
		}
		m_hotel_or_office=multi_register_value[nSelReg];///////hotel or office
	}
	else
	{
		m_hotel_or_office=multi_register_value[125];///////hotel or office
	}

	if (m_hotel_or_office == 0 && bOccupied) // office - occupied : 2SP
	{			
		vector<int>  szPos(3);
		m_pDayTwoSP->GetRange(nMin, nMax);
		int nNum = m_pDayTwoSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		//int nSP = szPos[1] + nMin;
		//int nSP = multi_register_value[135]; // or 374
		int nCoolSP = szPos[1] + nMin;

		int nSP = (nCoolSP-nHeatSP)/2 + nHeatSP;	
		write_one(g_tstat_id, 135, nSP);	

		// SP �ǲ���ġ�ֻ��ı�db?? ���䣬����sp ��С��heatsp
		write_one(g_tstat_id, 119, (nCoolSP - nSP)*10);
		write_one(g_tstat_id, 120, (nSP - nHeatSP)*10);
	}

	if (m_hotel_or_office == 1 && bOccupied) // hotel - occupied  //  1SP
	{
		vector<int>  szPos(3);
		m_pDaySingleSP->GetRange(nMin, nMax);
		int nNum = m_pDaySingleSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		int nSP = szPos[1] + nMin;
		//int nSP = multi_register_value[374]/10; // 135
		int nCoolSP = szPos[2] + nMin;

		write_one(g_tstat_id, 135, nSP);	//135
		// SP �ǲ���ġ�ֻ��ı�db����
		if(m_fFirmwareVersion > 34.1)
		{write_one(g_tstat_id, 422, nCoolSP*10 );}
		else {write_one(g_tstat_id, 379, nCoolSP*10 );}

		write_one(g_tstat_id, 136, nHeatSP);

	}

	if (m_hotel_or_office == 0 && !bOccupied) // office - unoccupied
	{  //2sp
		vector<int>  szPos(3);
		m_pNightTwoSP->GetRange(nMin, nMax);
		int nNum = m_pNightTwoSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		//int nSP = szPos[1] + nMin;
		//int nSP = multi_register_value[135];//374
		int nCoolSP = szPos[1] + nMin;
		//int nSP =(nCoolSP - nHeatSP)/2 +nHeatSP;
		write_one(g_tstat_id, 135, (nCoolSP-nHeatSP)/2 + nHeatSP);	//374

		// SP �ǲ���ġ�ֻ��ı�db����
		write_one(g_tstat_id, 183, nCoolSP);
		write_one(g_tstat_id, 182, nHeatSP);
	}

	if (m_hotel_or_office == 1 && !bOccupied) // hotel - unoccupied
	{ //1sp
		vector<int>  szPos(3);
		m_pNightSingleSP->GetRange(nMin, nMax);
		int nNum = m_pNightSingleSP->GetPos(szPos);
		int nHeatSP = szPos[0] + nMin;
		int nSP = szPos[1] + nMin;
		//int nSP = multi_register_value[374]/10;
		int nCoolSP = szPos[2] + nMin;

		int nCoolDB = nCoolSP-nSP;
		int nHeatDB = nSP - nHeatSP;
		write_one(g_tstat_id, 135, nSP);	
		// DB �ǲ���ġ���ı�SP 
		write_one(g_tstat_id, 124, nCoolDB*10 );
		write_one(g_tstat_id, 123, nHeatDB*10);
	}


	InitSliderBars2();
	EndWaitCursor();
}

void CT3000View::InitFanSpeed()
{


	CString p[6]={_T("Fan Off"),_T("Fan On"),_T("Fan Low"),_T("Fan Mid"),_T("Fan High"),_T("Fan Aut")};
	m_FanComBox.ResetContent();

	if(product_register_value[MODBUS_AUTO_ONLY])	//129   107
	{
		m_FanComBox.AddString(p[0]);
		m_FanComBox.AddString(p[5]);
	}
	else
	{
		switch(product_register_value[MODBUS_FAN_MODE])		//122   105
		{
		case 0:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[1]);m_FanComBox.AddString(p[5]);break;
		case 1:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[2]);m_FanComBox.AddString(p[4]);m_FanComBox.AddString(p[5]);break;
		case 2:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[2]);m_FanComBox.AddString(p[3]);m_FanComBox.AddString(p[4]);m_FanComBox.AddString(p[5]);break;
		default:m_FanComBox.AddString(p[0]);m_FanComBox.AddString(p[2]);m_FanComBox.AddString(p[3]);m_FanComBox.AddString(p[4]);m_FanComBox.AddString(p[5]);break;
		}
	}

	if ((product_register_value[MODBUS_AUTO_ONLY]==1)&&((product_register_value[MODBUS_FAN_SPEED]>1)||(product_register_value[MODBUS_FAN_SPEED]<0)))//0912	//T5 129	137  137 //T6 107  273  273
		m_FanComBox.SetCurSel(1);
	else
		m_FanComBox.SetCurSel(product_register_value[MODBUS_FAN_SPEED]);//137  273
	m_iCurFanSpeed = product_register_value[MODBUS_FAN_SPEED]; //137  273
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

void CT3000View::InitFlexSliderBars_tstat6()
{
#if 1//0907



	m_daySlider.ShowWindow(SW_HIDE);
	CString strTemp;

	int nRangeMin	=tstat6flex[Min];
	int nRangeMax	=tstat6flex[Max];

	int nCoolSP = 0;
	int nHeatSP = 0;
	int nSP =0;


//	���������� ��ߣ�DAY
#if 1
	if (FlexSP == 1)
	{
		//������2 SP
		if (MDAY==1) // office  2SP day
		{	

		// 		345	1	Low byte	W/R	(Day)Occupied   setpoint 
		// 		346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
		// 		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.	
			GetDlgItem(IDC_DAY_EDIT)->EnableWindow(TRUE);
			CString strInfo;			
			strInfo.Format(_T("nHeatSP=%d;nSP=%d;nCoolSP=%d"),nHeatSP, nSP, nCoolSP);			
			SetPaneString(2, strInfo);

			nSP= tstat6flex[DaySP];
//0904
// 			nCoolSP =  tstat6flex[DcoolDB];
// 			nHeatSP = tstat6flex[DheatDB];


			nCoolSP = tstat6flex[DcoolSP];
			nHeatSP = tstat6flex[DheatSP];

	
		//	if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			if ((nHeatSP<nRangeMin)||(nCoolSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nHeatSP>nRangeMax)||(nCoolSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			{
				//m_slidertsta6�Ǵ����ݿ��ж�������
				memcpy(tstat6flex,m_slidertsta6,sizeof(m_slidertsta6));
//0904
// 				nSP= tstat6flex[DaySP];
// 				nCoolSP =  tstat6flex[DcoolDB];
// 				nHeatSP = tstat6flex[DheatDB];


				nSP = tstat6flex[DaySP];	
				nCoolSP = tstat6flex[DcoolSP];

				nHeatSP = tstat6flex[DheatSP];





			}else 
			{


// 				if (nCoolSP<nRangeMin)
// 				{
// 					nCoolSP = nRangeMin;
// 					tstat6flex[DcoolSP] = nCoolSP;
// 				}
// 
// 				if (nCoolSP>=nSP)
// 				{
// 					nSP = nCoolSP+1;
// 					tstat6flex[DaySP] = nSP;
// 				}
// 				if (nSP>=nHeatSP)
// 				{
// 					nHeatSP = nSP+1;
// 					tstat6flex[DheatSP] = nHeatSP;
// 				}
// 				if (nHeatSP>=nRangeMax)
// 				{
// 					nRangeMax = nHeatSP+1;
// 					tstat6flex[Max] = nRangeMax;
// 				}	


//0910
				if (nHeatSP<nRangeMin)
				{
					nHeatSP = nRangeMin;
					tstat6flex[DheatSP] = nHeatSP;
				}

				if (nHeatSP>=nSP)
				{
					nSP = nHeatSP+1;
					tstat6flex[DaySP] = nSP;
				}
				if (nSP>=nCoolSP)
				{
					nCoolSP = nSP+1;
					tstat6flex[DcoolSP] = nCoolSP;
				}
				if (nCoolSP>=nRangeMax)
				{
					nRangeMax = nCoolSP+1;
					tstat6flex[Max] = nRangeMax;
				}	

			}


			//nCoolSP,nSP,nHeatSP//nMinPos < nMidPos && nMidPos < nMaxPos && nMinPos >= 0)
			//if (!(nCoolSP<nSP&&nSP<nHeatSP&&nCoolSP>=0))
			//0910
			if (!(nHeatSP<nSP&&nSP<nCoolSP&&nHeatSP>=0))
			{
				return;
			}
			//nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);

			//if (!(nCoolSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nHeatSP-nRangeMin>=0))
			//0910
			if (!(nHeatSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nCoolSP-nRangeMin>=0))
			{
				return;
			}
			//if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nHeatSP||nRangeMin>nCoolSP)
			//0910
			if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nCoolSP||nRangeMin>nHeatSP)
			{
				return;
			}


			m_pDayTwoSP->SetRange(nRangeMax, nRangeMin);


			//BOOL bRetSP = m_pDayTwoSP->SetPos_tstat6_2pos(nCoolSP,nSP,nHeatSP);
			//0910
			BOOL bRetSP = m_pDayTwoSP->SetPos_tstat6_2pos(nHeatSP,nSP,nCoolSP);
			if (bRetSP)
			{			
				CString strInfo;			
				//strInfo.Format(_T("nHeatSP=%d;nSP=%d;nCoolSP=%d"),nHeatSP, nSP, nCoolSP);	
				//0910
				strInfo.Format(_T("nHeatSP=%d;nSP=%d;nCoolSP=%d"),nCoolSP, nSP,nHeatSP);			
				SetPaneString(2, strInfo);
			}

			HandleSliderSetPos(bRetSP);//tstat6

			m_pDayTwoSP->ShowWindow(SW_NORMAL);

			m_pDaySingleSP->ShowWindow(SW_HIDE);


			// 		345	1	Low byte	W/R	(Day)Occupied   setpoint 
			// 		346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
			// 		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.	


//  			strTemp.Format(_T("%d"),nHeatSP );
//  			m_DayCoolEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"), nCoolSP);
//  			m_DayHeatEdit.SetWindowText(strTemp);
//  			m_daysetpoint = nSP;

//0910
			strTemp.Format(_T("%d"),nCoolSP );
			m_DayCoolEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),nHeatSP);
			m_DayHeatEdit.SetWindowText(strTemp);
			m_daysetpoint = nSP;



// 			strTemp.Format(_T("%d"),newtstat6[347]/10);
// 			m_DayCoolEdit.SetWindowText(strTemp);
// 			strTemp.Format(_T("%d"), newtstat6[346]/10);
// 			m_DayHeatEdit.SetWindowText(strTemp);
// 			m_daysetpoint = newtstat6[345]/10;
			
//0903
// 			m_DayCoolStatic.SetWindowText(_T("Heating DB"));
// 			m_DayHeatStatic.SetWindowText(_T("Cooling DB"));


// 			m_DayCoolStatic.SetWindowText(_T("Heating SP"));
// 			m_DayHeatStatic.SetWindowText(_T("Cooling SP"));

			//0910
			m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
			m_DayHeatStatic.SetWindowText(_T("Heating SP"));

			UpdateData(FALSE);

		}else  // hotel 1 SP day 
 		{


// #define  DaySP		0  //address 345
// #define  DcoolDB	1  //address 346
// #define  DheatDB	2  //address 347
// #define  DcoolSP	3  //address 348
// #define  DheatSP	4  //address 349
// #define  NightSP	5  //address 350
// #define  NheatDB	6  //address 352
// #define  NcoolDB	7  //address 353
// #define  NheatSP	8  //address 354
// #define  NcoolSP	9  //address 355
// #define  Max		10  //address 365
// #define  Min		11  //address 366
 
			//348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
			//349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)

			GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);
			//GetDlgItem(IDC_DAY_EDIT)->ShowWindow(SW_SHOW);

//  			int nCoolingDBRegister = 348;
//  			int nHeatingDBRegister = 349;
//  			int nSPRegister = 345;	
//  			int nCoolDeadband = newtstat6[nCoolingDBRegister];
//  			nSP = newtstat6[nSPRegister];	
//  			nCoolSP = newtstat6[nCoolingDBRegister];
//  
//  			nHeatSP = newtstat6[nHeatingDBRegister];






			nSP = tstat6flex[DaySP];	
			nCoolSP = tstat6flex[DcoolSP];

			nHeatSP = tstat6flex[DheatSP];


			//if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			//0910
			if ((nHeatSP<nRangeMin)||(nCoolSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nHeatSP>nRangeMax)||(nCoolSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			{
				//m_slidertsta6�Ǵ����ݿ��ж�������
				memcpy(tstat6flex,m_slidertsta6,sizeof(m_slidertsta6));


				nSP = tstat6flex[DaySP];	
				nCoolSP = tstat6flex[DcoolSP];

				nHeatSP = tstat6flex[DheatSP];





			}else 
			{

// 				if (nCoolSP<nRangeMin)
// 				{
// 					nCoolSP = nRangeMin;
// 					tstat6flex[DcoolSP] = nCoolSP;
// 				}
// 
// 				if (nCoolSP>=nSP)
// 				{
// 					nSP = nCoolSP+1;
// 					tstat6flex[DaySP] = nSP;
// 				}
// 				if (nSP>=nHeatSP)
// 				{
// 					nHeatSP = nSP+1;
// 					tstat6flex[DheatSP] = nHeatSP;
// 				}
// 				if (nHeatSP>=nRangeMax)
// 				{
// 					nRangeMax = nHeatSP+1;
// 					tstat6flex[Max] = nRangeMax;
// 				}



				//0910
				if (nHeatSP<nRangeMin)
				{
					nHeatSP = nRangeMin;
					tstat6flex[DheatSP] = nHeatSP;
				}
				if(nHeatSP>=nSP)	//Modify by Fance
				//if (DheatSP>=nSP)		Marked by Fance
				{
					nSP = nHeatSP+1;
					//nSP = DheatSP+1;Marked by Fance
					tstat6flex[DaySP] = nSP;
				}
				if (nSP>=nCoolSP)
				{
					nCoolSP = nSP+1;
					tstat6flex[DcoolSP] = nCoolSP;
				}
				if (nCoolSP>=nRangeMax)
				{
					nRangeMax = nCoolSP+1;
					tstat6flex[Max] = nRangeMax;
				}
			}





// 			if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin>6000)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin))
// 			{
// 				int inum = 345;
// 				for (int i =0;i<6;i++)
// 				{
// 					newtstat6[inum] = tstat6flex[i];
// 					inum++;
// 				}
// 
// 				inum = 352;
// 				for (int i =6;i<10;i++)
// 				{
// 					newtstat6[inum] = tstat6flex[i];
// 					inum++;
// 				}
// 				nRangeMax = tstat6flex[10];
// 				nRangeMin = tstat6flex[11];
// 
// 
// 				nSP = newtstat6[nSPRegister];	
// 				nCoolSP = newtstat6[nCoolingDBRegister];
// 				nHeatSP = newtstat6[nHeatingDBRegister];
// 
// 			}else if((nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
// 			{
// 				nSP = (nRangeMax+nRangeMin)/2;		
// 				nCoolSP = nSP - nSP/2;
// 				nHeatSP = nSP + nSP/2;
// 			}
// 
// 
// 
// 
// 			if (nRangeMin<0)
// 			{
// 				nRangeMin =0;
// 			}
// 
// 			if (nCoolSP<nRangeMin)
// 			{
// 				nCoolSP = nRangeMin;
// 			}
// 
// 			// 			if (nHeatSP<0)
// 			// 			{
// 			// 				nHeatSP=nRangeMin;
// 			// 			}
// 			if (nCoolSP>=nSP)
// 			{
// 				nSP = nCoolSP+1;
// 			}
// 			if (nSP>=nHeatSP)
// 			{
// 				nHeatSP = nSP+1;
// 			}

			//nCoolSP,nSP,nHeatSP//nMinPos < nMidPos && nMidPos < nMaxPos && nMinPos >= 0)
			//if (!(nCoolSP<nSP&&nSP<nHeatSP&&nCoolSP>=0))
			//0910
			if (!(nHeatSP<nSP&&nSP<nCoolSP&&nHeatSP>=0))
			{
				return;
			}
			//nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);

			//if (!(nCoolSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nHeatSP-nRangeMin>=0))
			//0910
			if (!(nHeatSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nCoolSP-nRangeMin>=0))
			{
				return;
			}

			//if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nHeatSP||nRangeMin>nCoolSP)
			//0910
			if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nCoolSP||nRangeMin>nHeatSP)
			{
				return;
			}


 
 			//////////////////////////////////////////////////////////////////////////
		//	nRangeMax = nHeatSP > nRangeMax ? nHeatSP : nRangeMax;
 		//	nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
 		//	nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
 			m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);
 
 			//BOOL bRetSP = m_pDaySingleSP->SetPos(nHeatSP, nSP, nCoolSP);	
 			//BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(nCoolSP, nSP, nHeatSP);//tstat6		
			//0910
			BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(nHeatSP, nSP,nCoolSP);//tstat6		
// 			CString strInfo;
// 			strInfo.Format(_T("bOccupied;hotel"));			
// 			SetPaneString(2, strInfo);
 			HandleSliderSetPos(bRetSP);//tstat6





 
 			m_pDaySingleSP->ShowWindow(SW_NORMAL);
 			m_pDayTwoSP->ShowWindow(SW_HIDE);
 
 			strTemp.Format(_T("%d"), nSP);
 			m_dayInfoEdit.SetWindowText(strTemp);
 
//  			strTemp.Format(_T("%d"),nHeatSP );
//  			m_DayCoolEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"),nCoolSP);
//  			m_DayHeatEdit.SetWindowText(strTemp);

			//0910
			strTemp.Format(_T("%d"),nCoolSP );
			m_DayCoolEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),nHeatSP);
			m_DayHeatEdit.SetWindowText(strTemp);



			//348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
			//349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)

// 			strTemp.Format(_T("%d"), newtstat6[345]/10);
// 			m_dayInfoEdit.SetWindowText(strTemp);
// 
// 			strTemp.Format(_T("%d"),newtstat6[349]/10);
// 			m_DayCoolEdit.SetWindowText(strTemp);
// 			strTemp.Format(_T("%d"),newtstat6[348]/10);
// 			m_DayHeatEdit.SetWindowText(strTemp);

// 			m_DayCoolStatic.SetWindowText(_T("Heating SP"));
// 			m_DayHeatStatic.SetWindowText(_T("Cooling SP"));


			m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
			m_DayHeatStatic.SetWindowText(_T("Heating SP"));
			m_daysetpoint = nSP;
			UpdateData(FALSE);


//  			//stat6
//  			m_pNightTwoSP->ShowWindow(SW_HIDE);
//  			m_pNightSingleSP->ShowWindow(SW_HIDE);
//  			strTemp.Format(_T("%d"), 0);
//  			m_nightInfoEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"), 0);
//  			m_nightHeatInfoEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"),0);
//  			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
//  			pSPWnd->SetWindowText(strTemp);
 		}
	}
	if(FlexSPN == 1)
#endif
//�������Ҳ��  �Ҳ� NIGHT
	{

		// #define  DaySP		0  //address 345
		// #define  DcoolDB	1  //address 346
		// #define  DheatDB	2  //address 347
		// #define  DcoolSP	3  //address 348
		// #define  DheatSP	4  //address 349
		// #define  NightSP	5  //address 350
		// #define  NheatDB	6  //address 352
		// #define  NcoolDB	7  //address 353
		// #define  NheatSP	8  //address 354
		// #define  NcoolSP	9  //address 355
		// #define  Max		10  //address 365
		// #define  Min		11  //address 366
 
 		if (MNIGHT == 1) // office
 		{	

			GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(TRUE);
			//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
			//352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
			//353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.



//  			nSP = newtstat6[350];
//  			nCoolSP =newtstat6[353];
//  			nHeatSP = newtstat6[352];

//0904
// 			nSP = newtstat6[NightSP];
// 			nCoolSP = newtstat6[NcoolSP];
// 			nHeatSP = newtstat6[NheatSP];



			nSP = tstat6flex[NightSP];	
			nCoolSP = tstat6flex[NcoolSP];

			nHeatSP = tstat6flex[NheatSP];

//0904
// 			nSP = tstat6flex[NightSP];
// 			nCoolSP =tstat6flex[NcoolDB];
// 			nHeatSP = tstat6flex[NheatDB];

			//if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			//0910
			if ((nHeatSP<nRangeMin)||(nCoolSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nHeatSP>nRangeMax)||(nCoolSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			{
				//m_slidertsta6�Ǵ����ݿ��ж�������
				memcpy(tstat6flex,m_slidertsta6,sizeof(m_slidertsta6));

				nSP = tstat6flex[NightSP];
				nCoolSP =tstat6flex[NcoolSP];
				nHeatSP = tstat6flex[NheatSP];

			}else 
			{


// 				if (nCoolSP<nRangeMin)
// 				{
// 					nCoolSP = nRangeMin;
// 					tstat6flex[NcoolSP] = nCoolSP;
// 				}
// 
// 				if (nCoolSP>=nSP)
// 				{
// 					nSP = nCoolSP+1;
// 					tstat6flex[NightSP] = nSP;
// 				}
// 				if (nSP>=nHeatSP)
// 				{
// 					nHeatSP = nSP+1;
// 					tstat6flex[NheatSP] = nHeatSP;
// 				}
// 				if (nHeatSP>=nRangeMax)
// 				{
// 					nRangeMax = nHeatSP+1;
// 					tstat6flex[Max] = nRangeMax;
// 				}	


				//0910
				if (nHeatSP<nRangeMin)
				{
					nHeatSP = nRangeMin;
					tstat6flex[NheatSP] = nHeatSP;
				}

				if (nHeatSP>=nSP)
				{
					nSP = nHeatSP+1;
					tstat6flex[NightSP] = nSP;
				}
				if (nSP>=nCoolSP)
				{
					nCoolSP = nSP+1;
					tstat6flex[NcoolSP] = nCoolSP;
				}
				if (nCoolSP>=nRangeMax)
				{
					nRangeMax = nCoolSP+1;
					tstat6flex[Max] = nRangeMax;
				}	

			}

			//nCoolSP,nSP,nHeatSP//nMinPos < nMidPos && nMidPos < nMaxPos && nMinPos >= 0)
			//if (!(nCoolSP<nSP&&nSP<nHeatSP&&nCoolSP>=0))
			//0910
			if (!(nHeatSP<nSP&&nSP<nCoolSP&&nHeatSP>=0))
			{
				return;
			}
			//nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);

			//if (!(nCoolSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nHeatSP-nRangeMin>=0))
			//0910
			if (!(nHeatSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nCoolSP-nRangeMin>=0))
			{
				return;
			}

			if (nRangeMin<=0||nRangeMin<=0)
			{
				return;
			}

		//	if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nHeatSP||nRangeMin>nCoolSP)
			//0910
			if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nCoolSP||nRangeMin>nHeatSP)
			{
				return;
			}


// 			if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin>6000)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin))
// 			{
// 				int inum = 345;
// 				for (int i =0;i<6;i++)
// 				{
// 					newtstat6[inum] = tstat6flex[i];
// 					inum++;
// 				}
// 
// 				inum = 352;
// 				for (int i =6;i<10;i++)
// 				{
// 					newtstat6[inum] = tstat6flex[i];
// 					inum++;
// 				}
// 				nRangeMax = tstat6flex[10];
// 				nRangeMin = tstat6flex[11];
// 
// 				nSP = newtstat6[350];
// 				nCoolSP =newtstat6[353];
// 				nHeatSP = newtstat6[352];
// 
// 
// 			}else if((nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
// 			{
// 				nSP = (nRangeMax+nRangeMin)/2;		
// 				nCoolSP = nSP - nSP/2;
// 				nHeatSP = nSP + nSP/2;
// 			}
// 
// 
// 
// 
// 			if (nRangeMin<0)
// 			{
// 				nRangeMin =0;
// 			}
// 
// 			if (nCoolSP<nRangeMin)
// 			{
// 				nCoolSP = nRangeMin;
// 			}
// 
// 			// 			if (nHeatSP<0)
// 			// 			{
// 			// 				nHeatSP=nRangeMin;
// 			// 			}
// 			if (nCoolSP>=nSP)
// 			{
// 				nSP = nCoolSP+1;
// 			}
// 			if (nSP>=nHeatSP)
// 			{
// 				nHeatSP = nSP+1;
// 			}
// 
// 
// 
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			nRangeMax = nHeatSP > nRangeMax ? nHeatSP : nRangeMax;


 

 
 			//nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
 			//nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
 
 			//nRangeMax = nSP > nRangeMax ? nSP : nRangeMax;
 
 			m_pNightTwoSP->SetRange(nRangeMax, nRangeMin);
 			//	BOOL bRetSP = m_pNightTwoSP->SetPos( nHeatSP, nCoolSP,nCoolSP+1);	
 
 
 			//BOOL bRetSP = m_pNightTwoSP->SetPos_tstat6_2pos(nCoolSP , nSP,nHeatSP);
			//0910
			BOOL bRetSP = m_pNightTwoSP->SetPos_tstat6_2pos(nHeatSP,nSP,nCoolSP);
//  			if (!bRetSP)
//  			{			
//  				CString strInfo;
//  				strInfo.Format(_T("unoccupied;office"));			
//  				SetPaneString(2, strInfo);
//  			}
 
 			HandleSliderSetPos(bRetSP);//tstat6
 			m_pNightTwoSP->ShowWindow(SW_NORMAL);
 			m_pNightSingleSP->ShowWindow(SW_HIDE);
 
 

			//strTemp.Format(_T("%d"), nSP);
			//CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			//pSPWnd->SetWindowText(strTemp);


			//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
			//352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
			//353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.

// 			m_nightpot = newtstat6[350]/10;
// 			UpdateData(FALSE);
// 
// 			strTemp.Format(_T("%d"),newtstat6[352]/10);
// 			m_nightInfoEdit.SetWindowText(strTemp);
// 			strTemp.Format(_T("%d"),newtstat6[353]/10);
// 			m_nightHeatInfoEdit.SetWindowText(strTemp);



 			m_nightpot = nSP;
 			UpdateData(FALSE);
 
//  			strTemp.Format(_T("%d"),nHeatSP);
//  			m_nightInfoEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"),nCoolSP);
//  			m_nightHeatInfoEdit.SetWindowText(strTemp);

//0910
			strTemp.Format(_T("%d"),nCoolSP);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),nHeatSP);
			m_nightHeatInfoEdit.SetWindowText(strTemp);
//0903
// 			m_NightCoolStatic.SetWindowText(_T("Heating DB"));
// 			m_NightHeatStatic.SetWindowText(_T("Cooling DB"));
// 
// 			m_NightCoolStatic.SetWindowText(_T("Heating SP"));
// 			m_NightHeatStatic.SetWindowText(_T("Cooling SP"));

			//0910

			m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
			m_NightHeatStatic.SetWindowText(_T("Heating SP"));
 
 			//stat6
 
//  			strTemp.Format(_T("%d"), 0);
//  			m_DayCoolEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"), 0);
//  			m_DayHeatEdit.SetWindowText(strTemp);
//  			m_pDaySingleSP->ShowWindow(SW_HIDE);
//  			m_pDayTwoSP->ShowWindow(SW_HIDE);
//  
//  			strTemp.Format(_T("%d"), 0);
//  			m_dayInfoEdit.SetWindowText(strTemp);
 
 		}else  // hotel  ���¼�����night  1sp	
#if 1
		{

			// #define  DaySP		0  //address 345
			// #define  DcoolDB	1  //address 346
			// #define  DheatDB	2  //address 347
			// #define  DcoolSP	3  //address 348
			// #define  DheatSP	4  //address 349
			// #define  NightSP	5  //address 350
			// #define  NheatDB	6  //address 352
			// #define  NcoolDB	7  //address 353
			// #define  NheatSP	8  //address 354
			// #define  NcoolSP	9  //address 355
			// #define  Max		10  //address 365
			// #define  Min		11  //address 366



		GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);

			//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
			//354	2	Full	W/R	(Night)Unoccupied heating setpoint
			//355	2	Full	W/R	(Night)Unoccupied cooling setpoint

			int nCoolingRegister = 355;
			int nHeatingRegister = 354;

// 			int nSPRegister = 350;	
// 			nSP = newtstat6[nSPRegister];
// 			nCoolSP = newtstat6[nCoolingRegister];
// 			nHeatSP = newtstat6[nHeatingRegister];


//0904
// 			nSP = newtstat6[NightSP];
// 			nCoolSP = newtstat6[NcoolSP];
// 			nHeatSP = newtstat6[NheatSP];


			nSP = tstat6flex[NightSP];	
			nCoolSP = tstat6flex[NcoolSP];

			nHeatSP = tstat6flex[NheatSP];


		//	if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			//0910
			if ((nHeatSP<nRangeMin)||(nCoolSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin<0)||(nRangeMax<nRangeMin)||(nHeatSP>nRangeMax)||(nCoolSP<nRangeMin)||(nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
			{
				//m_slidertsta6�Ǵ����ݿ��ж�������
				memcpy(tstat6flex,m_slidertsta6,sizeof(m_slidertsta6));

				nSP = tstat6flex[NightSP];
				nCoolSP = tstat6flex[NcoolSP];
				nHeatSP = tstat6flex[NheatSP];

			}else 
			{


// 				if (nCoolSP<nRangeMin)
// 				{
// 					nCoolSP = nRangeMin;
// 					tstat6flex[NcoolSP] = nCoolSP;
// 				}
// 
// 				if (nCoolSP>=nSP)
// 				{
// 					nSP = nCoolSP+1;
// 					tstat6flex[NightSP] = nSP;
// 				}
// 				if (nSP>=nHeatSP)
// 				{
// 					nHeatSP = nSP+1;
// 					tstat6flex[NheatSP] = nHeatSP;
// 				}
// 				if (nHeatSP>=nRangeMax)
// 				{
// 					nRangeMax = nHeatSP+1;
// 					tstat6flex[Max] = nRangeMax;
// 				}	

				//0910

				if (nHeatSP<nRangeMin)
				{
					nHeatSP = nRangeMin;
					tstat6flex[NheatSP] = nHeatSP;
				}

				if (nHeatSP>=nSP)
				{
					nSP = nHeatSP+1;
					tstat6flex[NightSP] = nSP;
				}
				if (nSP>=nCoolSP)
				{
					nCoolSP = nSP+1;
					tstat6flex[NcoolSP] = nCoolSP;
				}
				if (nCoolSP>=nRangeMax)
				{
					nRangeMax = nCoolSP+1;
					tstat6flex[Max] = nRangeMax;
				}	



			}

			//nCoolSP,nSP,nHeatSP//nMinPos < nMidPos && nMidPos < nMaxPos && nMinPos >= 0)
			//if (!(nCoolSP<nSP&&nSP<nHeatSP&&nCoolSP>=0))
			//0910
			if (!(nHeatSP<nSP&&nSP<nCoolSP&&nHeatSP>=0))
			{
				return;
			}
			//nMinPos-m_nMin >=0&& nMidPos-m_nMin >=0 && nMaxPos-m_nMin >= 0);

			//if (!(nCoolSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nHeatSP-nRangeMin>=0))
			//0910
			if (!(nHeatSP-nRangeMin>=0&&nSP-nRangeMin>=0&&nCoolSP-nRangeMin>=0))
			{
				return;
			}

			//if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nHeatSP||nRangeMin>nCoolSP)
			//0910
			if (nRangeMin<=0||nRangeMin<=0||nRangeMax<nCoolSP||nRangeMin>nHeatSP)
			{
				return;
			}


// 			if ((nCoolSP<nRangeMin)||(nHeatSP>nRangeMax)||(nRangeMax>6000)||(nRangeMax<0)||(nRangeMin>6000)||(nRangeMin>6000)||(nRangeMax<nRangeMin)||(nCoolSP>nRangeMax)||(nHeatSP<nRangeMin))
// 			{
// 				int inum = 345;
// 				for (int i =0;i<6;i++)
// 				{
// 					newtstat6[inum] = tstat6flex[i];
// 					inum++;
// 				}
// 
// 				inum = 352;
// 				for (int i =6;i<10;i++)
// 				{
// 					newtstat6[inum] = tstat6flex[i];
// 					inum++;
// 				}
// 				nRangeMax = tstat6flex[10];
// 				nRangeMin = tstat6flex[11];
// 
// 
// 				nSP = newtstat6[nSPRegister];
// 				nCoolSP = newtstat6[nCoolingRegister];
// 				nHeatSP = newtstat6[nHeatingRegister];
// 
// 
// 			}else if((nCoolSP == nHeatSP && nHeatSP == 0 )||nCoolSP>6000||nHeatSP>6000||nCoolSP<0||nHeatSP<0)
// 			{
// 				nSP = (nRangeMax+nRangeMin)/2;		
// 				nCoolSP = nSP - nSP/2;
// 				nHeatSP = nSP + nSP/2;
// 			}
// 
// 
// 
// 
// 			if (nRangeMin<0)
// 			{
// 				nRangeMin =0;
// 			}
// 
// 			if (nCoolSP<nRangeMin)
// 			{
// 				nCoolSP = nRangeMin;
// 			}
// 
// 			// 			if (nHeatSP<0)
// 			// 			{
// 			// 				nHeatSP=nRangeMin;
// 			// 			}
// 			if (nCoolSP>=nSP)
// 			{
// 				nSP = nCoolSP+1;
// 			}
// 			if (nSP>=nHeatSP)
// 			{
// 				nHeatSP = nSP+1;
// 			}
// 
// 
// 
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			nRangeMax = nHeatSP > nRangeMax ? nHeatSP : nRangeMax;
			m_pNightSingleSP->SetRange(nRangeMax, nRangeMin);	





			//	BOOL bRetSP =  m_pNightSingleSP->SetPos( nHeatSP, nSP, nCoolSP);
			//BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(nCoolSP, nSP,nHeatSP);
			//0910
			BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(nHeatSP,nSP,nCoolSP);
// 			if (!bRetSP)
// 			{			
// 				CString strInfo;
// 				strInfo.Format(_T("unoccupied;hotel"));			
// 				SetPaneString(2, strInfo);
// 			}

			HandleSliderSetPos(bRetSP);//tstat6


			m_pNightSingleSP->ShowWindow(SW_NORMAL);
			m_pNightTwoSP->ShowWindow(SW_HIDE);

		

// 			strTemp.Format(_T("%d"), nSP);
// 			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
// 			pSPWnd->SetWindowText(strTemp);


			//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
			//354	2	Full	W/R	(Night)Unoccupied heating setpoint
			//355	2	Full	W/R	(Night)Unoccupied cooling setpoint


// 			m_nightpot = newtstat6[350]/10;
// 			UpdateData(FALSE);
// 
// 			strTemp.Format(_T("%d"), newtstat6[354]/10);
// 			m_nightInfoEdit.SetWindowText(strTemp);
// 			strTemp.Format(_T("%d"), newtstat6[355]/10);
// 			m_nightHeatInfoEdit.SetWindowText(strTemp);



 			m_nightpot = nSP;
 			UpdateData(FALSE);
 
//  			strTemp.Format(_T("%d"), nHeatSP);
//  			m_nightInfoEdit.SetWindowText(strTemp);
//  			strTemp.Format(_T("%d"), nCoolSP);
//  			m_nightHeatInfoEdit.SetWindowText(strTemp);

//0910
			strTemp.Format(_T("%d"),nCoolSP );
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),nHeatSP);
			m_nightHeatInfoEdit.SetWindowText(strTemp);


// 			m_NightCoolStatic.SetWindowText(_T("Heating SP"));
// 			m_NightHeatStatic.SetWindowText(_T("Cooling SP"));

			m_NightCoolStatic.SetWindowText(_T("Cooling SP"));
			m_NightHeatStatic.SetWindowText(_T("Heating SP"));
			//stat6
// 
// 			strTemp.Format(_T("%d"), 0);
// 			m_DayCoolEdit.SetWindowText(strTemp);
// 			strTemp.Format(_T("%d"), 0);
// 			m_DayHeatEdit.SetWindowText(strTemp);
// 			m_pDaySingleSP->ShowWindow(SW_HIDE);
// 			//m_pDayTwoSP->ShowWindow(SW_HIDE);
// 			strTemp.Format(_T("%d"), 0);
// 			m_dayInfoEdit.SetWindowText(strTemp);
		}
#endif
	}




	//������E:\Tstat67\T3000\T3000
#endif
}


//It's useless.
void CT3000View::OnFlexSlideCallBackFortstat6()
{
	int nMin, nMax;
#if 1
// 	short nOccupied = multi_register_value[184];  // Day setpoint option  
// 	BOOL bOccupied = nOccupied & 0x0001;
// 	int nOfficeOrHotel = 0;

	int nCoolSP = 0;
	int nHeatSP = 0;
	int nSP =0;

	//else  // unoccupied

	//�������ұ� 1SP
	{
		//nOfficeOrHotel = multi_register_value[125];	
		//if (nOfficeOrHotel == 0) // office
		if (0)
		{		
			vector<int>  szPos(2);
			m_pNightTwoSP->GetRange(nMin, nMax);
			int nNum = m_pNightTwoSP->GetPos(szPos);
			int nHeatSP = szPos[0] + nMin;
			int nCoolSP = szPos[1] + nMin;
			int ret=0;
			ret =write_one(g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT, nCoolSP*10,5);//355���ǶԵ�
			ret =write_one(g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, nHeatSP*10,5);//354���ǶԵ� ����353
			//Marked by Fance ,no need to judge product this time
			//int ret=0;
			//if (((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)&&m_fFirmwareVersion >35.5)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
			//{
			//	ret =write_one(g_tstat_id, 354, nCoolSP,5);
			//	ret =write_one(g_tstat_id, 353, nHeatSP,5);

			//}else
			//{
			//	write_one(g_tstat_id, 183, nCoolSP,5);
			//	write_one(g_tstat_id, 182, nHeatSP,5);
			//}

			CString strTemp;

			strTemp.Format(_T("%d"), nCoolSP);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"), nHeatSP);
			m_nightHeatInfoEdit.SetWindowText(strTemp);


		}
		else if(0) // day
		{
			vector<int>  szPos(3);
			m_pNightSingleSP->GetRange(nMin, nMax);
			int nNum = m_pNightSingleSP->GetPos(szPos);
			int nHeatSP = szPos[0] + nMin;
			int nSP = szPos[1] + nMin;
			int nCoolSP = szPos[2] + nMin;

			int ret=0;
			CString strTemp;
			strTemp.Format(_T("%d"), nSP);
			CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
			pSPWnd->SetWindowText(strTemp);

			strTemp.Format(_T("%d"), nCoolSP);
			m_nightInfoEdit.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),  nHeatSP);
			m_nightHeatInfoEdit.SetWindowText(strTemp);

			{
				ret =write_one(g_tstat_id, 345, nSP,5);
				ret =write_one(g_tstat_id, 348, nCoolSP,5);
				ret =write_one(g_tstat_id, 349, nHeatSP,5);

			}

		}
	}



#endif
}

void CT3000View::TemperatureSlider()
{
	//GetDlgItem(IDC_TEMPRETURE_SLIDER)->EnableWindow(TRUE);//2.5.0.95

}
void CT3000View::OnNMReleasedcaptureTempretureSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
	{
	int icurtemp = m_TemperaureSlider.GetPos();
	int ret =0;
	ret = write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,icurtemp*10,5);//T6=121
	if(ret>0)
		SetPaneString(1,_T("Set temperature successful!"));
	else
		SetPaneString(1,_T("Set temperature failure!"));

	}
	*pResult = 0;
}


void CT3000View::OnEnKillfocusDayEdit()
{
#if 1//0907
	//345	1	Low byte	W/R	(Day)Occupied   setpoint 

// 	UpdateData();
// 	tstat6flex[0] = m_daysetpoint;

BeginWaitCursor();
	UpdateData();
	CString str,str2;
	GetDlgItem(IDC_EDIT_DAYHEAT)->GetWindowText(str);
	GetDlgItem(IDC_EDIT_DAYCOOL)->GetWindowText(str2);
	int itemp,itemp2;
	itemp = _ttoi(str);
	itemp2 = _ttoi(str2);
	if (m_daysetpoint<=itemp)
	{
		tstat6flex[DaySP] = itemp+1;
		m_daysetpoint = tstat6flex[DaySP];

	}else if (m_daysetpoint>=itemp2)
	{
		tstat6flex[DaySP] = itemp2-1;
		m_daysetpoint = tstat6flex[DaySP];
	}else
		tstat6flex[DaySP] = m_daysetpoint;
	int iret =0;
	iret = write_one(g_tstat_id,MODBUS_DAY_SETPOINT,m_daysetpoint*10); //T6 =345

	
//		newtstat6[345] = m_daysetpoint;
// 		newtstat6[346] = itemp2;
// 		newtstat6[347] = itemp;
		UpdateData(FALSE);

	FlexSP = 1;
	InitFlexSliderBars_tstat6();//tsat6
	FlexSP = 0;

GetDlgItem(IDC_SETPTSTATIC)->ShowWindow(SW_SHOW);
EndWaitCursor();
#endif
}

void CT3000View::OnEnKillfocusEditCurSp()
{
	// TODO: Add your control notification handler code here

#if 1//0907

BeginWaitCursor();
	UpdateData();
	CString str,str2;	
	GetDlgItem(IDC_NIGHT_EDIT)->GetWindowText(str2);
	GetDlgItem(IDC_NIGHTHEAT_EDIT)->GetWindowText(str);

	int itemp,itemp2;
	itemp = _ttoi(str);
	itemp2 = _ttoi(str2);
	if (m_nightpot<=itemp)
	{
		tstat6flex[NightSP] = itemp+1;
		m_nightpot = tstat6flex[5];

	}else if (m_nightpot>=itemp2)
	{
		tstat6flex[NightSP] = itemp2-1;
		m_nightpot = tstat6flex[NightSP];
	}else
	{
		tstat6flex[NightSP] = m_nightpot;
	}	
//newtstat6[350] = m_nightpot;
// 	newtstat6[352] = itemp;
// 	newtstat6[353] = itemp2;

	int iret =0;
	iret = write_one(g_tstat_id,MODBUS_NIGHT_SETPOINT,m_daysetpoint*10);//350
	UpdateData(FALSE);

	FlexSPN = 1;
	InitFlexSliderBars_tstat6();//tsat6
	FlexSPN = 0;

	GetDlgItem(IDC_SETPTSTATIC)->ShowWindow(SW_SHOW);
	EndWaitCursor();
#endif
}

void CT3000View::FreshCtrl()
{
	//	121	1	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1
	/*MNIGHT=1;
	MDAY=1;
	if (MDAY == 0)
		GetDlgItem(IDC_COMBO7)->SetWindowText(_T("1 SP"));
	else
		GetDlgItem(IDC_COMBO7)->SetWindowText(_T("2 SP"));

	if (MNIGHT == 0)
		GetDlgItem(IDC_COMBO4)->SetWindowText(_T("1 SP"));
	else
		GetDlgItem(IDC_COMBO4)->SetWindowText(_T("2 SP"));*/

	if(product_register_value[MODBUS_DEGC_OR_F] == 1)	//t5= 121;t6=104
	{
		g_unint = FALSE;
		GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("��F"));
	}else 
	{
		g_unint = TRUE;
		GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("��C"));
	}
	m_nID=g_tstat_id;
	m_fFirmwareVersion=get_curtstat_version();
	m_nSerialNumber=get_serialnumber();
	m_nHardwareVersion=product_register_value[MODBUS_HARDWARE_REV];//8
	m_nID=g_tstat_id;
	m_fFirmwareVersion=get_curtstat_version();
	m_nSerialNumber=get_serialnumber();

	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	if(nModel==0)
		return;

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

	m_fTemperature=(float)product_register_value[MODBUS_TEMPRATURE_CHIP]/10;	//101   121
	m_iCurFanSpeed=product_register_value[MODBUS_FAN_SPEED];	//137		273
	m_nOverrideTime=product_register_value[MODBUS_OVERRIDE_TIMER];//211		111
	m_nTimeLeft=product_register_value[MODBUS_OVERRIDE_TIMER_LEFT];//212	112
	float tempflot = (float)product_register_value[MODBUS_TEMPRATURE_CHIP]/10; //101  121

	//float tempflot = (float)newtstat6[121]/10;
	CString  strTemp;
	if (g_unint)
		strTemp.Format(_T("%.1f��C"),tempflot);
	else
		strTemp.Format(_T("%.1f��F"),tempflot);

	GetDlgItem(IDC_STATICDAY)->SetWindowText(strTemp);
	GetDlgItem(IDC_STATICNIGHT)->SetWindowText(strTemp);





	switch(product_register_value[MODBUS_MODE_OPERATION])
		//switch(multi_register_value[107])
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

	switch(product_register_value[MODBUS_PID2_MODE_OPERATION])
		//switch(multi_register_value[278])
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

	if (product_register_value[MODBUS_COOL_HEAT_MODE]==1)
	{
		//m_bcooling=TRUE;
		m_CoolCtrl.SetCheck(BST_CHECKED);
		m_HeatCtrl.SetCheck(BST_UNCHECKED);
	}
	if(product_register_value[MODBUS_COOL_HEAT_MODE]==2)
	{
		//	m_bcooling=FALSE;
		m_CoolCtrl.SetCheck(BST_UNCHECKED);
		m_HeatCtrl.SetCheck(BST_CHECKED);
	}
	//switch(multi_register_value[128])
	switch(product_register_value[MODBUS_KEYPAD_SELECT])	//128  
	{
	case 2:	m_CoolCtrl.EnableWindow();m_HeatCtrl.EnableWindow();break;
	case 4:	m_CoolCtrl.EnableWindow();m_HeatCtrl.EnableWindow();break;
	case 5:	m_CoolCtrl.EnableWindow();m_HeatCtrl.EnableWindow();break;
	default:m_CoolCtrl.EnableWindow(false);m_HeatCtrl.EnableWindow(false);break;
	}	
	//if(multi_register_value[184] & 1)
	if(product_register_value[MODBUS_INFO_BYTE] & 1)	//184
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
	//if(multi_register_value[350]&0x4)//Free Cool available,yes or on
	if(product_register_value[MODBUS_FREE_COOL_CONFIG]&0x4)	//350
	{
		m_strFreeCool=_T("Yes");
	}
	else
	{
		m_strFreeCool=_T("No");
	}
	// for fan speed
	InitFanSpeed();  
	//350 119
	if(product_register_value[MODBUS_FREE_COOL_CONFIG]&0x10)//outside air temperature from NC, if the NC has an active OAT then OAT=OK, else OAT=FAIL .
		//if(multi_register_value[350]&0x10)//outside air temperature from NC, if the NC has an active OAT then OAT=OK, else OAT=FAIL .
	{
		m_strFreeCoolFN=_T("OK");//shoud also show the oat on the user's display
	}
	else
	{
		m_strFreeCoolFN=_T("FAIL"); //show a grey box for the OAT
	}
	if(product_register_value[MODBUS_FREE_COOL_CONFIG]&0x2)
		//if(multi_register_value[350]&0x2)
	{
	}
	else
	{
		m_strFreeCoolFN=_T("");
	}

	//////////////////////////////////////////////////////////////////////////
	// just for tstat6
	if((product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT6)||(product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT7))
	{	
		// humidity
		BOOL m_disable_hum;
		if((product_register_value[20]&2)==2)
		{m_disable_hum=TRUE;}
		else
		{
			m_disable_hum=FALSE;
		}

		CString str;
		
			if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
			{
				str.Format(_T("%0.1f%%"),(float)(product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10));
			}
			else
			{
				str.Format(_T("%0.1f%%"),(float)(product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10));
			}




		CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HUMIDITY);


		pEdit->SetWindowText(str);
	}	
	else
	{
		GetDlgItem(IDC_EDIT_HUMIDITY)->SetWindowText(_T("N/A"));	
	}

	//time
	CTime time = CTime::GetCurrentTime();
	CString strtime = time.Format(_T("%I:%M:%S %p"));
	CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC_TIME);
	pEdit->SetWindowText(strtime);
	CString strDate = time.Format(_T("%A, %b %d, %Y"));
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC);
	pEdit->SetWindowText(strDate);


	if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
	{
		if (Flexflash)//ֻ�е�������б�ʱ��ִ������ġ�
		{
			GetDlgItem(IDC_DAYSTATIC2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DAYSTATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_OCCUPACHECK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_UNOCCUPIED_MARK)->ShowWindow(SW_HIDE);
			//	GetDlgItem(IDC_STATIC_DAYSPMODE)->ShowWindow(SW_HIDE);//0903
			//	GetDlgItem(IDC_STATIC_SPMODE)->ShowWindow(SW_HIDE);//0903
			GetDlgItem(IDC_TEMPRETURE_SLIDER)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_TEMPINFO_EDIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC1t)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STATIC2SP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC1SP)->ShowWindow(SW_SHOW);


			GetDlgItem(IDC_SLIDER_DAY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_SLIDER_NIGHT)->ShowWindow(SW_SHOW);

			//max 365 min 366

			m_singlesliderday.SetRange(product_register_value[366], product_register_value[365],TRUE);
			m_singleslidernight.SetRange(product_register_value[366], product_register_value[365],TRUE);


			if ((product_register_value[365] <=0)||(product_register_value[365] >3000))
			{
				product_register_value[365] = 50;
			}

			int tempd = product_register_value[365]+product_register_value[366]-(int)product_register_value[121]/10;

			if (tempd<product_register_value[366])
			{
				tempd = product_register_value[366];
			}else if (tempd>product_register_value[365])
			{
				tempd = product_register_value[365];
			}
			m_singleslidernight.SetPos(tempd);
			m_singlesliderday.SetPos(tempd);

			FlexSP =1;
			FlexSPN =1;
			InitFlexSliderBars_tstat6();
			FlexSP =0;
			FlexSPN =0;
			Flexflash = FALSE;

			FlexSP =1;
			FlexSPN =1;
			InitFlexSliderBars_tstat6();
			FlexSP =0;
			FlexSPN =0;
			Flexflash = FALSE;
		}
		else if((FlexSP!=0)||(FlexSPN!=0))
		{

			// 			GetDlgItem(IDC_SLIDER_DAY)->ShowWindow(SW_SHOW);
			// 			GetDlgItem(IDC_SLIDER_NIGHT)->ShowWindow(SW_SHOW);


			if ((product_register_value[365] <=0)||(product_register_value[365] >3000))
			{
				product_register_value[365] = 50;
			}



			int tempd1 = product_register_value[365]+product_register_value[366]-(int)product_register_value[121]/10;

			if (tempd1<product_register_value[366])
			{
				tempd1 = product_register_value[366];
			}else if (tempd1>product_register_value[365])
			{
				tempd1 = product_register_value[365];
			}

			m_singleslidernight.SetPos(tempd1);
			m_singlesliderday.SetPos(tempd1);


			// 			float tempflot = (float)newtstat6[121]/10;
			// 			CString  strTemp;
			// 			if (g_unint)
			// 				strTemp.Format(_T("%.1f��C"),tempflot);
			// 			else
			// 				strTemp.Format(_T("%.1f��F"),tempflot);
			// 
			// 			GetDlgItem(IDC_STATICDAY)->SetWindowText(strTemp);
			// 			GetDlgItem(IDC_STATICNIGHT)->SetWindowText(strTemp);

			InitFlexSliderBars_tstat6();
			FlexSP =0;
			FlexSPN =0;
		}
		// 		CString strInfo;
		// 		strInfo.Format(_T("InitFlexSliderBars_tstat6();"));			
		// 		SetPaneString(2, strInfo);

	}
	else
	{
		if (Flexflash)
		{

			GetDlgItem(IDC_DAYSTATIC2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DAYSTATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_OCCUPACHECK)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_UNOCCUPIED_MARK)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_STATIC_DAYSPMODE)->ShowWindow(SW_SHOW);//0903
			//GetDlgItem(IDC_STATIC_SPMODE)->ShowWindow(SW_SHOW);//0903
			//GetDlgItem(IDC_TEMPRETURE_SLIDER)->ShowWindow(SW_SHOW);//2.5.0.95

			//GetDlgItem(IDC_TEMPINFO_EDIT)->ShowWindow(SW_SHOW);//2.5.0.95
			//GetDlgItem(IDC_STATIC1t)->ShowWindow(SW_SHOW);//2.5.0.95

			GetDlgItem(IDC_STATIC2SP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC1SP)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);


			GetDlgItem(IDC_SLIDER_DAY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_SLIDER_NIGHT)->ShowWindow(SW_SHOW);

			//131	1	MAX_SETPOINT, Setpoint high, the highest setpoint a user will be able to set from the keypad.
			//132	1	MIN_SETPOINT, Setpoint Low, the lowest setpoint a user will be able to set from the keypad. 

			m_singlesliderday.SetRange(multi_register_value[132], multi_register_value[131],TRUE);
			m_singleslidernight.SetRange(multi_register_value[132], multi_register_value[131],TRUE);

			if ((multi_register_value[131]<=0)||(multi_register_value[131]>3000))
			{
				multi_register_value[131] = 50;
			}


			int tempd3 = multi_register_value[131]+multi_register_value[132]-(int)multi_register_value[101]/10;
			if (tempd3<multi_register_value[132])
			{
				tempd3 = multi_register_value[132];
			}else if (tempd3>multi_register_value[131])
			{
				tempd3 = multi_register_value[131];
			}
			m_singleslidernight.SetPos(tempd3);
			m_singlesliderday.SetPos(tempd3);





			Flexflash = FALSE;
		}


		// 		CString strInfo;
		// 		strInfo.Format(_T("CT3000View::Fresh():InitSliderBars2()multi_register_value[7]=%d"),multi_register_value[7]);			
		// 		SetPaneString(2, strInfo);
		//������
		// 		CString strTemp;
		// 		strTemp.Format(_T("%d"),100);
		//  		m_dayInfoEdit.SetWindowText(strTemp);
		// 
		// 
		// 		//CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);//IDC_DAY_EDIT
		// 		CWnd *pSPWnd = GetDlgItem(IDC_EDIT5);//0913IDC_DAY_EDIT
		// 		pSPWnd->SetWindowText(strTemp);
		// 		m_DayCoolEdit.SetWindowText(strTemp);
		// 		m_DayHeatEdit.SetWindowText(strTemp);


		if ((multi_register_value[131]<=0)||(multi_register_value[131]>3000))
		{
			multi_register_value[131] = 50;
		}

		int tempd4 = multi_register_value[131]+multi_register_value[132]-(int)multi_register_value[101]/10;
		if (tempd4<multi_register_value[132])
		{
			tempd4 = multi_register_value[132];
		}else if (tempd4>multi_register_value[131])
		{
			tempd4 = multi_register_value[131];
		}

		m_singleslidernight.SetPos(tempd4);
		m_singlesliderday.SetPos(tempd4);


		InitSliderBars2();//
	}
	UpdateData(FALSE);
}



void CT3000View::OnCbnSelchangeCombo7()
{
	// TODO: Add your control notification handler code here
	//CString str;

	//GetDlgItem(IDC_COMBO7)->GetWindowText(str);

	//if (str.CompareNoCase(_T("1 SP")) == 0)
	//	MDAY = 0;
	//else
	//	MDAY = 1;

	//FlexSP = 1;

	//FreshCtrl();
}


void CT3000View::OnCbnSelchangeCombo4()
{
	// TODO: Add your control notification handler code here
	/*CString str;

	GetDlgItem(IDC_COMBO4)->GetWindowText(str);

	if (str.CompareNoCase(_T("1 SP")) == 0)
		MNIGHT = 0;
	else
		MNIGHT = 1;
	FlexSPN = 1;

	FreshCtrl();*/
}


void CT3000View::OnCbnSelchangeStaticunint()
{     int sel=m_gUnit.GetCurSel();
    if (sel!=product_register_value[MODBUS_DEGC_OR_F])
    {
	   write_one(g_tstat_id,MODBUS_DEGC_OR_F,sel);
	   product_register_value[MODBUS_DEGC_OR_F]=sel;

	   if(product_register_value[MODBUS_DEGC_OR_F] == 1)	//t5= 121;t6=104
	   {
		   g_unint = FALSE;
		   GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("��F"));
	   }else 
	   {
		   g_unint = TRUE;
		   GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("��C"));
	   }
	   Fresh_In();
    } 
     
    
	
}
