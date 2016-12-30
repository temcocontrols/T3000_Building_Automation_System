
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

#include "LedsDialog.h"
#include "TDelayForm.h"

#include "TStatScheduleDlg.h"
#include "TstatZigbeeLogic.h"

#include "../FlexSlideBar/FlexSlideWnd.h"
#include "../FlexSlideBar/FSBContainer.h"
#include "DisplayConfig.h"
#include "DisplayLEDLCDConfig.h"
#include "HtmlHelp.h"

#include "DisplayConfig.h"
#include "LedsDialog.h"
 #include "../SQLiteDriver/CppSQLite3.h"


#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


//2013 03 10
//老毛要求代码里面所有的数字 例如  product_register_value[101] 其中101要变为可变的，由数据库中读到。
//所以写了一个函数 Get 在程序运行前 加载 mdb，在程序运行中动态的查找 对应的值。product_register_value[MODBUS_TEMPRATURE_CHIP].；
//这样既保证了程序的可读性，又方便维护，日后 寄存器列表变化后只需修改 t3000.mdb的寄存器表即可。
#include "T3000RegAddress.h"
extern T3000RegAddress GloabeAddress;
extern int Mdb_Adress_Map;
extern BOOL m_active_key_mouse;
// CT3000View
//extern BOOL g_bPauseMultiRead;

IMPLEMENT_DYNCREATE(CT3000View, CFormView)
BEGIN_MESSAGE_MAP(CT3000View, CFormView)
    ON_BN_CLICKED(IDC_COOL_RADIO, &CT3000View::OnBnClickedCoolRadio)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()



    ON_BN_CLICKED(IDC_GRAPGICBUTTON, &CT3000View::OnBnClickedGrapgicbutton)
    ON_BN_CLICKED(IDC_PARAMETERBTN, &CT3000View::OnBnClickedParameterbtn)
//	ON_BN_CLICKED(IDC_OUTPUTBTN, &CT3000View::OnBnClickedOutputbtn)

    ON_EN_KILLFOCUS(IDC_INPUTNAMEEDIT, &CT3000View::OnEnKillfocusInputnameedit)
    ON_EN_KILLFOCUS(IDC_OUTPUTNAMEEDIT, &CT3000View::OnEnKillfocusOutputnameedit)
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_TRENDLOGVIEW, &CT3000View::OnBnClickedTrendlogview)


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


    ON_EN_KILLFOCUS(IDC_EDIT_CUR_SP, &CT3000View::OnEnKillfocusEditCurSp)


    ON_CBN_SELCHANGE(IDC_STATICUNINT, &CT3000View::OnCbnSelchangeStaticunint)


    ON_WM_HELPINFO()
    ON_BN_CLICKED(IDC_HELP, &CT3000View::OnBnClickedHelp)
//	ON_BN_CLICKED(IDC_BTN_TOPOLOGICAL, &CT3000View::OnBnClickedBtnTopological)
    ON_BN_CLICKED(IDC_HELP_HELP, &CT3000View::OnBnClickedHelpHelp)
    ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBackForT3000View)
    ON_MESSAGE(MY_READ_DATA_CALLBACK, ReadDataCallBack)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_EN_KILLFOCUS(IDC_DAY_EDIT, &CT3000View::OnEnKillfocusDayEdit)
    //ON_BN_CLICKED(IDC_TEST_SLIDER, &CT3000View::OnBnClickedTestSlider)
END_MESSAGE_MAP()

// CT3000View construction/destruction
UINT BackMainUIFresh(LPVOID pParam)
{

    CT3000View* pdlg = (CT3000View*)pParam;
    int multy_ret = 0;
    while(1)
    {
        if(pdlg->IsWindowVisible())
        {


            //pdlg->Fresh();

            Sleep(5000);
            continue;
            if (!is_connect())
            {
                continue;
            }
            if (!no_mouse_keyboard_event_enable_refresh)
            {
                continue;
            }
            if (g_tstat_id==255)
            {
                continue;
            }
            if (m_active_key_mouse)
            {
                continue;
            }
            if (g_fresh_T3000_background)
            {
                continue;
            }
            if(no_mouse_keyboard_event_enable_refresh)
            {
                if (MODBUS_INTERNAL_THERMISTOR>0)
                {

                    multy_ret =  Read_Multi(g_tstat_id,&multi_register_value[MODBUS_INTERNAL_THERMISTOR],MODBUS_INTERNAL_THERMISTOR,2);
                    if (multy_ret>0)
                    {
                        // memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
                        for (int i = 0; i<2; i++)
                        {
                            product_register_value[MODBUS_INTERNAL_THERMISTOR+i]=multi_register_value[MODBUS_INTERNAL_THERMISTOR+i];
                        }
                    }
                }

            }
            if (!no_mouse_keyboard_event_enable_refresh)
            {
                continue;
            }
            if(no_mouse_keyboard_event_enable_refresh)
            {
                if (MODBUS_EXTERNAL_SENSOR_0>0)
                {
                    multy_ret = Read_Multi(g_tstat_id,&multi_register_value[MODBUS_EXTERNAL_SENSOR_0],MODBUS_EXTERNAL_SENSOR_0,5);
                    if (multy_ret>0)
                    {
                        //memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
                        for (int i = 0; i<5; i++)
                        {
                            product_register_value[MODBUS_EXTERNAL_SENSOR_0+i]=multi_register_value[MODBUS_EXTERNAL_SENSOR_0+i];
                        }
                    }
                }

            }
            if (!no_mouse_keyboard_event_enable_refresh)
            {
                continue;
            }
            if(no_mouse_keyboard_event_enable_refresh)
            {
                if (MODBUS_TEMPRATURE_CHIP>0)
                {
                    multy_ret = Read_Multi(g_tstat_id,&multi_register_value[MODBUS_TEMPRATURE_CHIP],MODBUS_TEMPRATURE_CHIP,10);
                    if (multy_ret>0)
                    {
                        //memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
                        for (int i = 0; i<10; i++)
                        {
                            product_register_value[MODBUS_TEMPRATURE_CHIP+i]=multi_register_value[MODBUS_TEMPRATURE_CHIP+i];
                        }
                    }
                }

            }




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
    m_pFreshBackground=NULL;
    m_rcTabCtrl.CopyRect(CRect(620,500,850,800));//runningtime
    m_bSliderSetPosWarning = TRUE; // permit warning
    m_active_key_mouse=FALSE;

    for (int i=0; i<10; i++)
    {
        m_InRanges[i]=-1;
        m_OutRanges[i]=-1;
    }
    DayMax=0;
    DayMin=0;
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

    //DDX_Radio(pDX, IDC_OCCUPA_RADIO, m_OcupaCtrl);
    //DDX_Radio(pDX, IDC_UNOCCUPA_RADIO, m_UnOcupaCtrl);
    DDX_Check(pDX, IDC_OCCUPACHECK, m_bOccupied);
    DDX_Control(pDX, IDC_OCCUPACHECK, m_OcupiedBtn);
    DDX_Text(pDX, IDC_FREECOOLA_EDIT, m_strFreeCool);
    DDX_Text(pDX, IDC_FREECOOLF_EDIT, m_strFreeCoolFN);

    // 	DDX_Control(pDX, IDC_TEMPRETURE_SLIDER, m_TemperaureSlider);
    // 	DDX_Control(pDX, IDC_DAY_SLIDER, m_daySlider);
    // 	DDX_Control(pDX, IDC_NIGHT_SLIDER, m_nightSlider);
    // 	DDX_Control(pDX, IDC_NIGHTHEAT_SLIDER, m_nightHeatSlider);

    DDX_Control(pDX, IDC_TEMPINFO_EDIT, m_TempInfoEdit);
    DDX_Control(pDX, IDC_DAY_EDIT, m_dayInfoEdit);
    DDX_Control(pDX, IDC_NIGHT_EDIT, m_nightInfoEdit);
    DDX_Control(pDX, IDC_NIGHTHEAT_EDIT, m_nightHeatInfoEdit);
    DDX_Control(pDX, IDC_OUTPUT_MSFLEXGRID, m_Output_Grid);
    DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_Input_Grid);
    DDX_Control(pDX, IDC_INPUTNAMEEDIT, m_inNameEdt);
    DDX_Control(pDX, IDC_OUTPUTNAMEEDIT, m_outNameEdt);
    DDX_Control(pDX, IDC_SETPTSTATIC, m_SetptStatic);





    DDX_Control(pDX, IDC_EDIT_DAYCOOL, m_DayCoolEdit);
    DDX_Control(pDX, IDC_EDIT_DAYHEAT, m_DayHeatEdit);
    DDX_Control(pDX, IDC_STATIC_DAYCOOL, m_DayCoolStatic);
    DDX_Control(pDX, IDC_STATIC_DATSP, m_DaySPStatic);
    DDX_Control(pDX, IDC_STATIC_DAYHEAT, m_DayHeatStatic);

    DDX_Control(pDX, IDC_SLIDER_DAY, m_singlesliderday);

    DDX_Control(pDX, IDC_STATICUNINT, m_gUnit);


    DDX_Control(pDX, IDC_EDIT_CUR_SP, m_nightpotEdit);




  /*  DDX_Control(pDX, IDC_STATIC_ISP, m_isp);*/
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

    m_gUnit.AddString(_T(" C"));
    m_gUnit.AddString(_T(" F"));



    //GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);
    //GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);

    m_inNameEdt.ShowWindow(SW_HIDE);
    m_outNameEdt.ShowWindow(SW_HIDE);
    GetDlgItem(IDC_FANSPEEDCOMBO)->EnableWindow(FALSE);

    if(g_MainScreenLevel==1)
    {
        CAfxMessageDialog dlg;
        CString strPromption;
        strPromption.LoadString(IDS_STRNOPRIVILEGE);
        dlg.SetPromtionTxt(strPromption);
        dlg.DoModal();
    }

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

    CreateFlexSilde();





    CString lightsensor;
    lightsensor.Format(_T("%d LUX"),product_register_value[MODBUS_VALUE_SENSOR]);
    GetDlgItem(IDC_LIGHTING_SENSOR)->SetWindowText(lightsensor);

    m_DayCoolEdit.SetReadOnly(TRUE);
    COLORREF cf=RGB(0,255,255);
    m_DayCoolEdit.SetBackgroundColor(FALSE,cf);

    m_nightInfoEdit.SetReadOnly(TRUE);
    cf=RGB(0,255,255);
    m_nightInfoEdit.SetBackgroundColor(FALSE,cf);
    //---------------------------------------------
    m_DayHeatEdit.SetReadOnly(TRUE);
    cf=RGB(255,0,0);
    m_DayHeatEdit.SetBackgroundColor(FALSE,cf);

    m_nightHeatInfoEdit.SetReadOnly(TRUE);
    cf=RGB(255,0,0);
    m_nightHeatInfoEdit.SetBackgroundColor(FALSE,cf);
    //---------------------------------------------
    //m_dayInfoEdit.SetReadOnly(TRUE);
    cf=RGB(255,255,255);
    m_dayInfoEdit.SetBackgroundColor(FALSE,cf);

    //m_nightpotEdit.SetReadOnly(TRUE);
    cf=RGB(255,255,255);
    m_nightpotEdit.SetBackgroundColor(FALSE,cf);
    //---------------------------------------------
    m_TempInfoEdit.SetReadOnly(TRUE);
    cf=RGB(96,192,0);
    m_TempInfoEdit.SetBackgroundColor(FALSE,cf);

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
        m_pNightTwoSP->SetThumbColor(RGB(0,255,255)); //改变上面滑块的颜色
        m_pNightTwoSP->SetPageLength(5);
        m_pNightTwoSP->SetChannelWidth(20);//中间的坚状条大小
        m_pNightTwoSP->SetThumbWidth(34);//滑块的大小

        m_pNightTwoSP->SetRange(10, 0);
        m_pNightTwoSP->SetPos(0, 1, 2);
        m_pNightTwoSP->Setflag(1);
        // init two thumb slider

#endif
//       CStatic *s0 = (CStatic*)GetDlgItem(IDC_SLIDER_TEST);
//
//		s->GetWindowRect(&rc);
//		ScreenToClient(&rc);
//#if 1
//		m_pSlider_Test_TwoSP=new CFSBContainer;
//		m_pSlider_Test_TwoSP->SetParentWnd(this);
//
//
//
//		//int nRet = m_pNightTwoSP->SubclassWindow(s->GetSafeHwnd());
//		//int nRet = m_pNightTwoSP->SubclassDlgItem(IDC_FSB_OFFICE, this);
//		//////////////////////////////////////////////////////////////////////////
//		m_pSlider_Test_TwoSP->CreateInstance(s, s->GetStyle()|s->GetExStyle(), rc);
////m_pNightTwoSP->Create( NULL, s->GetStyle()|s->GetExStyle()|SS_SUNKEN, rc, this);
////		m_pNightTwoSP.Create(_T("CFlexSlideWnd"), SS_SUNKEN, rc, this ,10001);
////		m_pNightTwoSP.ShowWindow(SW_SHOW);
////*
//
//		m_pSlider_Test_TwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB);
//		m_pSlider_Test_TwoSP->SetFlexSlideBarRect(rc);
//		m_pSlider_Test_TwoSP->SetHorizontal(FALSE);
//		m_pSlider_Test_TwoSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
//// 		m_pNightTwoSP->SetChannelColor(RGB(192,192,192));
//// 		m_pNightTwoSP->SetThumbColor(RGB(255,128,128));
//
//		m_pSlider_Test_TwoSP->SetChannelColor(RGB(192,192,192));
//		m_pSlider_Test_TwoSP->SetThumbColor(RGB(0,0,255)); //改变上面滑块的颜色
//		m_pSlider_Test_TwoSP->SetPageLength(5);
//		m_pSlider_Test_TwoSP->SetChannelWidth(20);//中间的坚状条大小
//		m_pSlider_Test_TwoSP->SetThumbWidth(34);//滑块的大小
//
//		m_pSlider_Test_TwoSP->SetRange(30, 0);
//		m_pSlider_Test_TwoSP->SetPos(0, 10, 20);
//		m_pSlider_Test_TwoSP->Setflag(1);
//		// init two thumb slider
//
//#endif





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
        //m_pNightSingleSP->SetThumbColor(RGB(255,128,128));

        m_pNightSingleSP->SetThumbColor(RGB(0,0,255));
        m_pNightSingleSP->SetPageLength(5);
        m_pNightSingleSP->SetChannelWidth(20);
        m_pNightSingleSP->SetThumbWidth(34);

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
        m_pDayTwoSP->SetThumbColor(RGB(0,255,255));
        m_pDayTwoSP->SetPageLength(5);
        m_pDayTwoSP->SetChannelWidth(20);
        m_pDayTwoSP->SetThumbWidth(34);

        m_pDayTwoSP->SetRange(10, 0);
        m_pDayTwoSP->SetPos(0, 1, 2);
        m_pDayTwoSP->Setflag(3);

        //--------------------------------Test Slider------------------------------------

        //s = (CStatic*)GetDlgItem(IDC_SLIDER_TEST);
        //rc;
        //s->GetWindowRect(&rc);
        //ScreenToClient(&rc);
        //m_pSlider_Test_TwoSP=new CFSBContainer;
        //m_pSlider_Test_TwoSP->SetParentWnd(this);

        //m_pSlider_Test_TwoSP->CreateInstance(s, s->GetStyle()|s->GetExStyle(), rc);

        //m_pSlider_Test_TwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB);//10,12,FSB_THUMBSHAPE_TRI
        ////	m_pDayTwoSP->CreateFlexSlideBar(FALSE, FSB_STYLE_DOUBLETHUMB,10,12,FSB_THUMBSHAPE_TRI);
        //m_pSlider_Test_TwoSP->SetFlexSlideBarRect(rc);
        //m_pSlider_Test_TwoSP->SetHorizontal(FALSE);
        //m_pSlider_Test_TwoSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
        //// 		m_pDayTwoSP->SetChannelColor(RGB(192,192,192));
        //// 		m_pDayTwoSP->SetThumbColor(RGB(255,128,128));

        //m_pSlider_Test_TwoSP->SetChannelColor(RGB(192,192,192));
        //m_pSlider_Test_TwoSP->SetThumbColor(RGB(0,0,255));
        //m_pSlider_Test_TwoSP->SetPageLength(5);
        //m_pSlider_Test_TwoSP->SetChannelWidth(20);
        //m_pSlider_Test_TwoSP->SetThumbWidth(34);

        //m_pSlider_Test_TwoSP->SetRange(20, 0);
        //m_pSlider_Test_TwoSP->SetPos(8, 10, 12);
        //m_pSlider_Test_TwoSP->Setflag(5);
        //m_pSlider_Test_TwoSP->ShowWindow(TRUE);
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
        //m_pDaySingleSP->SetThumbColor(RGB(255,128,128));
        m_pDaySingleSP->SetThumbColor(RGB(0,0,255));
        m_pDaySingleSP->SetPageLength(5);
        m_pDaySingleSP->SetChannelWidth(20);
        m_pDaySingleSP->SetThumbWidth(34);

        m_pDaySingleSP->SetRange(10, 0);
        m_pDaySingleSP->SetPos(0, 1, 2);
        m_pDaySingleSP->Setflag(4);
        //


// 		s1 = (CStatic*)GetDlgItem(IDC_STATIC_TEST);
// 		m_pTemperSP= new CFSBContainer;
// 		s1->GetWindowRect(&rc);
// 		ScreenToClient(&rc);
// 		m_pTemperSP->CreateInstance(s1, s1->GetStyle()|s1->GetExStyle()|SS_SUNKEN, rc);
// 		m_pTemperSP->SetParentWnd(this);
//
// 		m_pTemperSP->CreateFlexSlideBar(FALSE, FSB_STYLE_TRIPLELINKAGE );
// 		m_pTemperSP->SetFlexSlideBarRect(rc);
// 		m_pTemperSP->SetHorizontal(FALSE);
// 		m_pTemperSP->SetFSBThumbShape( FSB_THUMBSHAPE_RECT);
// 		m_pTemperSP->SetChannelColor(RGB(192,192,192));
// 		//m_pDaySingleSP->SetThumbColor(RGB(255,128,128));
// 		m_pTemperSP->SetThumbColor(RGB(0,0,255));
// 		m_pTemperSP->SetPageLength(5);
// 		m_pTemperSP->SetChannelWidth(25);
// 		m_pTemperSP->SetThumbWidth(30);
//
// 		m_pTemperSP->SetRange(10, 0);
// 		m_pTemperSP->SetPos(3, 7, 9);
// 		m_pTemperSP->Setflag(5);


    }

//     m_tooltips.Create(GetDlgItem(IDC_FSB_DAYHOTEL));
//
//     m_tooltips.Activate(TRUE);
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
    CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    g_ifanStatus = product_register_value[MODBUS_FAN_SPEED];
    g_NEED_MULTI_READ=TRUE;
    int  Fresh_Min=(short)product_register_value[MODBUS_MIN_SETPOINT];
    int Fresh_Max=(short)product_register_value[MODBUS_MAX_SETPOINT];

    Initial_Max_Min();
    /*
    1>>input 1.....8
    2>>output 11...88
    */
    int index;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    if (SqliteDBT3000.tableExists("Value_Range"))//有Version表
    {
        CString sql;
        sql.Format(_T("Select * from Value_Range where SN=%d"),get_serialnumber());
         q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

        if (!q.eof())//有表但是没有对应序列号的值
        {
             
            while (!q.eof())
            {
                index=q.getIntField("CInputNo");
                if (index>10)//说明是output
                {
                    index%=10;
                    m_OutRanges[index]=q.getIntField("CRange");
                }
                else
                {
                    m_InRanges[index]=q.getIntField("CRange");
                }
                q.nextRow();
            }
        }
       
    }

   SqliteDBT3000.closedb();



    if (is_connect())
    {
        GetDlgItem(IDC_FANSPEEDCOMBO)->EnableWindow(TRUE);
        GetDlgItem(IDC_INPUT_MSFLEXGRID)->EnableWindow(TRUE);
        GetDlgItem(IDC_OUTPUT_MSFLEXGRID)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_FANSPEEDCOMBO)->EnableWindow(FALSE);
        GetDlgItem(IDC_INPUT_MSFLEXGRID)->EnableWindow(FALSE);
        GetDlgItem(IDC_OUTPUT_MSFLEXGRID)->EnableWindow(FALSE);
    }

    CString strTemp;
    strTemp.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
    strTemp+=_T("%");
    GetDlgItem(IDC_OUTPUT1)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270  389
    strTemp+=_T("%");
    GetDlgItem(IDC_OUTPUT2)->SetWindowText(strTemp);

    strTemp.Format(_T("Max=%d"),Fresh_Max);


    GetDlgItem(IDC_MAX2)->SetWindowText(strTemp);
    strTemp.Format(_T("Min=%d"),(short)Fresh_Min);

    GetDlgItem(IDC_MIN2)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_PIC_VERSION]);
    GetDlgItem(IDC_PIC_EDIT)->SetWindowText(strTemp);
    FreshCtrl();
    FreshIOGridTable();
    InitFanSpeed();
    m_nID=product_register_value[6];

    if(product_register_value[7]==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8)||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7)
    {
        GetDlgItem(IDC_GRAPGICBUTTON)->ShowWindow(SW_HIDE);
      //  GetDlgItem(IDC_PARAMETERBTN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SCHEDULE)->ShowWindow(SW_HIDE);

    }
    else
    {
        GetDlgItem(IDC_GRAPGICBUTTON)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_PARAMETERBTN)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BUTTON_SCHEDULE)->ShowWindow(SW_HIDE);



    } 
    if (m_pFreshBackground==NULL)
    {
        m_pFreshBackground = AfxBeginThread(BackMainUIFresh,this);
    }

    //if (product_register_value[11]!=0)
    //{
    //    m_isp.ShowWindow(TRUE);
    //    CString StrTemp;
    //    StrTemp = _T("Your device may be in ISP Model,Please check your device");
    //    m_isp.SetWindowTextW(StrTemp);
    //    m_isp.textColor(RGB(255,0,0));
    //    //m_current_product.bkColor(RGB(255,255,255));
    //    m_isp.setFont(30,10,NULL,_T("Arial"));
    //}
    //else
    //{
    //    m_isp.ShowWindow(FALSE);
    //}
    if (product_register_value[714]==0x56)
    {
        GetDlgItem(IDC_STATIC_NAME_TSTAT)->ShowWindow(TRUE);
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->ShowWindow(TRUE);
        CString TstatName;
        TstatName.Format(_T("%s%s"),GetTextFromReg(715),GetTextFromReg(719));
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->SetWindowText(TstatName);
    }
    else
    {
        GetDlgItem(IDC_STATIC_NAME_TSTAT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->ShowWindow(SW_HIDE);
    }
}

void CT3000View::Fresh_T3000View()
{
    int  Fresh_Min=(short)product_register_value[MODBUS_MIN_SETPOINT];
    int Fresh_Max=(short)product_register_value[MODBUS_MAX_SETPOINT];

    if (is_connect())
    {
        GetDlgItem(IDC_FANSPEEDCOMBO)->EnableWindow(TRUE);
        GetDlgItem(IDC_INPUT_MSFLEXGRID)->EnableWindow(TRUE);
        GetDlgItem(IDC_OUTPUT_MSFLEXGRID)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_FANSPEEDCOMBO)->EnableWindow(FALSE);
        GetDlgItem(IDC_INPUT_MSFLEXGRID)->EnableWindow(FALSE);
        GetDlgItem(IDC_OUTPUT_MSFLEXGRID)->EnableWindow(FALSE);
    }

    CString strTemp;
    strTemp.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
    strTemp+=_T("%");
    GetDlgItem(IDC_OUTPUT1)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270  389
    strTemp+=_T("%");
    GetDlgItem(IDC_OUTPUT2)->SetWindowText(strTemp);

    strTemp.Format(_T("Max=%d"),Fresh_Max);


    GetDlgItem(IDC_MAX2)->SetWindowText(strTemp);
    strTemp.Format(_T("Min=%d"),(short)product_register_value[MODBUS_MIN_SETPOINT]);

    GetDlgItem(IDC_MIN2)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_PIC_VERSION]);
    GetDlgItem(IDC_PIC_EDIT)->SetWindowText(strTemp);
    FreshCtrl();
    FreshIOGridTable();
    InitFanSpeed();








}
void CT3000View::OnBnClickedCoolRadio()
{
    // TODO: Add your control notification handler code here
}
HBRUSH CT3000View::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);


    switch(pWnd->GetDlgCtrlID())
    {

    //   case IDC_STATIC_DAYCOOL:
    //       {
    //           pDC->SetTextColor(RGB(255,255,255));
    //           pDC->SetBkColor(RGB(0,0,255));
    //           pDC->SetBkMode(TRANSPARENT);
    //           HBRUSH B = CreateSolidBrush(RGB(0,0,255));
    //           return (HBRUSH)B;
    //       }
    //case IDC_EDIT_DAYCOOL:
    //{
    //	pDC->SetTextColor(RGB(255,255,255));
    //	pDC->SetBkColor(RGB(0,0,255));
    //	pDC->SetBkMode(TRANSPARENT);
    //	HBRUSH B = CreateSolidBrush(RGB(0,0,255));
    //	return (HBRUSH)B;
    //}
    //case IDC_NIGHT_EDIT:
    //{
    //	pDC->SetTextColor(RGB(255,255,255));
    //	pDC->SetBkColor(RGB(0,0,255));
    //	pDC->SetBkMode(TRANSPARENT);
    //	HBRUSH B = CreateSolidBrush(RGB(0,0,255));
    //	return (HBRUSH)B;
    //}
    //   case IDC_STATIC_DAYHEAT:
    //   {
    //       pDC->SetTextColor(RGB(255,255,255));
    //       //pDC->SetBkColor(RGB(255,255,255));
    //       pDC->SetBkMode(TRANSPARENT);
    //       HBRUSH B = CreateSolidBrush(RGB(255,0,0));
    //       return (HBRUSH)B;
    //   }
    //	case IDC_EDIT_DAYHEAT:
    //	{
    //		pDC->SetTextColor(RGB(255,255,255));
    //		//pDC->SetBkColor(RGB(255,255,255));
    //		pDC->SetBkMode(TRANSPARENT);
    //		HBRUSH B = CreateSolidBrush(RGB(255,0,0));
    //		return (HBRUSH)B;
    //	}

    //   case IDC_TEMPINFO_EDIT:
    //       {
    //           pDC->SetTextColor(RGB(255,255,255));
    //           //pDC->SetBkColor(RGB(255,255,255));
    //           pDC->SetBkMode(TRANSPARENT);
    //           HBRUSH B = CreateSolidBrush(RGB(96,192,0));
    //           return (HBRUSH)B;
    //       }

    default:
        return CFormView::OnCtlColor(pDC,pWnd, nCtlColor);
    }

}
// 使用一个3Thumb的Slider，代替之前的3个Slider
// （A,B,C,D,F,G,H）专门给TStat5E之前的型号
#if 1
void CT3000View::InitSliderBars2()
{
    //0907以下
    // 以TSTAT5E Runar ver34.1为样本
    // 以TSTAT5E ver35.5为样本
    Initial_Max_Min();
    int nSP = 0;
    int dSP = 0;

    int nCoolSP = 0;
    int nHeatSP = 0;
    int nCoolDB = 0;
    int nHeatDB = 0;

    int dCoolSP = 0;
    int dHeatSP = 0;
    int dCoolDB = 0;
    int dHeatDB = 0;

    CString strTemp;
    int min=(short)product_register_value[MODBUS_MIN_SETPOINT];
    int max=(short)product_register_value[MODBUS_MAX_SETPOINT];

    float itemp;
    short nOccupied = product_register_value[MODBUS_INFO_BYTE];  // Day setpoint option
    BOOL bOccupied = nOccupied & 0x0001;

    /*
    int nOfficeSelReg=MODBUS_APPLICATION;
    if (bOccupied)
    {
    	if(product_register_value[7]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
    	{
    		nOfficeSelReg = MODBUS_DAYSETPOINT_OPTION;
    	}
    }
    else
    {
    	nOfficeSelReg = MODBUS_APPLICATION;
    }
    int m_hotel_or_office=product_register_value[nOfficeSelReg];




    //  		if(product_register_value[7] == PM_TSTAT5E)
    //  		{
    		itemp=(float)(short)(product_register_value[MODBUS_COOLING_SETPOINT]);
    //  		}
    //  		else
    //  		{
    // 			itemp=(float)(product_register_value[MODBUS_TWO_BYTE_SETPOINT]/10.0);
    // 			}


    		if(multi_register_value[7] == PM_TSTAT5E)
    		{
    			itemp=(float)(short)(product_register_value[135]);
    		}
    		else
    		{
    			itemp=(float)(short)(product_register_value[374]/10.0);
    		}

       nSP =(int)itemp;

    	dSP=nSP;//默认 day setpoint =night setpoint
         nOccupied = product_register_value[184];  // Day setpoint option
         bOccupied = nOccupied & 0x0001;


         nOfficeSelReg=454;
       if (bOccupied)
       {
    	   if(product_register_value[7]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)
    	   {
    		   nOfficeSelReg = 423;
    	   }
       }
       else
       {
    	   nOfficeSelReg = 125;
       }
         m_hotel_or_office=product_register_value[nOfficeSelReg];///////hotel or office
       int the_setpoint_ha_ha=(int)itemp;		// copy from old code vc2003.

    	if (m_hotel_or_office==0)
    	{
    		itemp=product_register_value[MODBUS_COOLING_DEADBAND];//119
    		dCoolDB = (int)(itemp/10);
    		itemp=product_register_value[MODBUS_HEATING_DEADBAND];//heat//120
    		dHeatDB = (int)(itemp/10);
    		dCoolSP=dSP+dCoolDB;
    		dHeatSP=dSP-dHeatDB;
    		nSP=0;
    		nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10;
    		nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10;
    		nSP = (short)(nCoolSP+nHeatSP)/2 ;
    		//nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
    		//nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));
    		//nCoolSP = nSP + nCoolDB;
    		//nHeatSP = nSP - nHeatDB;
    	}
    	else
    	{


    		itemp=product_register_value[MODBUS_COOLING_DEADBAND];//119
    		dCoolDB = (int)(itemp/10);
    		itemp=product_register_value[MODBUS_HEATING_DEADBAND];//heat//120
    		dHeatDB = (int)(itemp/10);
    		dCoolSP=dSP+dCoolDB;
    		dHeatSP=dSP-dHeatDB;


    		nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
    		nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));
    		nCoolSP = nSP + nCoolDB;
    		nHeatSP = nSP - nHeatDB;
    	}
    	*/
    int m_hotel_or_office=product_register_value[MODBUS_APPLICATION];
    if (product_type==T3000_5ABCDFG_LED_ADDRESS)
    {
        dSP=product_register_value[MODBUS_COOLING_SETPOINT];

        dCoolDB=product_register_value[MODBUS_COOLING_DEADBAND]/10;//119

        dHeatDB=product_register_value[MODBUS_HEATING_DEADBAND]/10;//heat//120

        dCoolSP=dSP+dCoolDB;
        dHeatSP=dSP-dHeatDB;

        if (m_hotel_or_office==0)// office
        {
            nSP=0;
            nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
            nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
            nSP = (short)(nCoolSP+nHeatSP)/2 ;
            nCoolDB = nCoolSP - nSP;
            nHeatDB = nSP - nHeatSP;


            /*	nSP=0;
            	nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
            	nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
            	nSP = (short)(nCoolSP+nHeatSP)/2 ;
            	nCoolDB = nCoolSP - nSP;
            	nHeatDB = nSP - nHeatSP;*/

        }
        else//Hotel
        {
            nSP=dSP;

            nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
            nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));

            nCoolSP = nSP + nCoolDB;
            nHeatSP = nSP - nHeatDB;
        }

    }


    if (product_type==T3000_5EH_LCD_ADDRESS)
    {


        if (m_hotel_or_office==0)// office
        {
            //Setpoint = ( NEW_EEP_CoolingSp + EEP_HeatingSp) / 2
            //cooling_db = NEW_EEP_CoolingSp - Setpoint;
            //heating_db = Setpoint - EEP_HeatingSp;

            dCoolSP=product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10;
            dHeatSP=product_register_value[MODBUS_HEATING_SETPOINT];
            dSP=(dCoolSP+dHeatSP)/2;
            dCoolDB=dCoolSP-dSP;
            dHeatDB=dSP-dHeatSP;

            nSP=0;
            nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
            nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
            nSP = (short)(nCoolSP+nHeatSP)/2 ;
            nCoolDB = nCoolSP - nSP;
            nHeatDB = nSP - nHeatSP;
        }
        else//Hotel
        {
            // Hotel mode:
            //Setpoint = EEP_CoolingSp
            // cooling_db = EEP_CoolDeadband ;
            //heating_db = EEP_HeatDeadband ;



            dSP=product_register_value[MODBUS_COOLING_SETPOINT];

            dCoolDB=product_register_value[MODBUS_COOLING_DEADBAND]/10;//119

            dHeatDB=product_register_value[MODBUS_HEATING_DEADBAND]/10;//heat//120

            dCoolSP=dSP+dCoolDB;
            dHeatSP=dSP-dHeatDB;

            nSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];

            nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
            nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));

            nCoolSP = nSP + nCoolDB;
            nHeatSP = nSP - nHeatDB;
        }

    }

    strTemp.Format(_T("%d"),DayMax);
    GetDlgItem(IDC_STATIC_MAX_DAY)->SetWindowText(strTemp);
    GetDlgItem(IDC_STATIC_MAX_NIGHT)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),DayMin);
    GetDlgItem(IDC_STATIC_MIN_DAY)->SetWindowText(strTemp);
    GetDlgItem(IDC_STATIC_MIN_NIGHT)->SetWindowText(strTemp);

    //////////////////////////////////////////////////////////////////////////
    //m_pDayTwoSP && bOccupied && bOccupied


    m_singlesliderday.SetThumbColor(RGB(96,192,0));
    m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
    m_singlesliderday.SetChannelColor(RGB(196, 196, 255));

    m_singlesliderday.SetRange((short)DayMin*10, (short)DayMax*10,TRUE);

    int currenttemp=(short)DayMin*10+(short)DayMax*10-(int)product_register_value[MODBUS_TEMPRATURE_CHIP];
    m_singlesliderday.SetPos(currenttemp);
#if 0
    if(m_hotel_or_office==0)  // office - occupied
    {
        // 2sp
        CString strTemp1 = _T("Two SetPoint Mode");
        m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
        m_DayHeatStatic.SetWindowText(_T("Heating SP"));



        m_daysetpoint = dSP;
        strTemp.Format(_T("%0.1f"),(float)m_daysetpoint);
        //m_dayInfoEdit.FieldText(RGB(100,100,100),strTemp);
        m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);
        if (g_unint)
        {
            strTemp.Format(_T("%.1f C"),(float)dCoolSP);
        }
        else
        {
            strTemp.Format(_T("%.1f F"),(float)dCoolSP);
        }
        //m_DayCoolEdit.FieldText(RGB(100,100,100),strTemp);
        m_DayCoolEdit.FieldText(RGB(0,0,0),strTemp);
        //m_DayCoolEdit.SetNumberDefaultTex();


        if (g_unint)
        {
            strTemp.Format(_T("%.1f C"),(float)dHeatSP);
        }
        else
        {
            strTemp.Format(_T("%.1f F"),(float)dHeatSP);
        }
        //m_DayHeatEdit.FieldText(RGB(100,100,100),strTemp);
        m_DayHeatEdit.FieldText(RGB(0,0,0),strTemp);
        //m_DayHeatEdit.SetNumberDefaultTex();
        //nSP= int(nSP/10.0);
        if (dCoolSP == dHeatSP && dCoolSP== dSP )
        {
            dSP = dHeatSP + 1;
            dCoolSP = dSP+1;
        }



// 		m_pDaySingleSP->SetRange(DayMax*10, DayMin*10);
// 		BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(dHeatSP*10, dSP*10, dCoolSP*10);
// 		HandleSliderSetPos(bRetSP);
//
// 		m_pDaySingleSP->ShowWindow(SW_NORMAL);
// 		m_pDayTwoSP->ShowWindow(SW_HIDE);


        m_pDayTwoSP->SetRange(DayMax*10, DayMin*10);
        BOOL bRetSP = m_pDayTwoSP->SetPos(dHeatSP*10, dSP*10, dCoolSP*10); // 因为只有2个thumb，所以sp不要了，设为两端的值
        HandleSliderSetPos(bRetSP);

        m_pDayTwoSP->ShowWindow(SW_NORMAL);
        m_pDaySingleSP->ShowWindow(SW_HIDE);

    }
    else  // hotel - occupied
#endif

    {
        // 1SP
        CString strTemp1 = _T("Single SetPoint Mode");


        if (g_unint)
        {
            strTemp.Format(_T("%.1fC"),float(dCoolSP));
        }
        else
        {
            strTemp.Format(_T("%.1fF"),float(dCoolSP));
        }
        //m_DayCoolEdit.FieldText(RGB(100,100,100),strTemp);
        m_DayCoolEdit.FieldText(RGB(0,0,0),strTemp);
        if (g_unint)
        {
            strTemp.Format(_T("%.1fC"),float(dHeatSP));
        }
        else
        {
            strTemp.Format(_T("%.1fF"),float(dHeatSP));
        }
        m_DayHeatEdit.FieldText(strTemp);
        m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
        m_DayHeatStatic.SetWindowText(_T("Heating SP"));
        strTemp.Format(_T("%0.1f"), (float)dSP);
        //m_dayInfoEdit.FieldText(RGB(100,100,100),strTemp);
        m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);

        if (dCoolSP == dHeatSP && dCoolSP== dSP )
        {
            dSP = dHeatSP + 1;
            dCoolSP = dSP+1;
        }
        m_daysetpoint = dSP;
        strTemp.Format(_T("%0.1f"),(float)m_daysetpoint);
        m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);


// 		m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);
// 		BOOL bRetSP = m_pDaySingleSP->SetPos(dHeatSP, dSP, dCoolSP);
// 		HandleSliderSetPos(bRetSP);
// 		m_pDayTwoSP->ShowWindow(SW_HIDE);
// 		m_pDaySingleSP->ShowWindow(SW_NORMAL);


        if (dHeatSP<dSP&&dSP<dCoolSP)
        {
        }
        else
        {
            DayMax =5;
            DayMin = 0;
            dHeatSP = 1;
            dSP = 2;
            dCoolSP = 3;
        }
        m_pDaySingleSP->SetRange(DayMax*10, DayMin*10);
        BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(dHeatSP*10, dSP*10, dCoolSP*10);
        HandleSliderSetPos(bRetSP);
        m_pDaySingleSP->ShowWindow(SW_NORMAL);
        m_pDayTwoSP->ShowWindow(SW_HIDE);






    }


    //////////////////////////////////////////////////////////////////////////
    //m_pNightTwoSP&& !bOccupied   (c+h)/2=sp  cool=sp+cdh  heat=
    if(m_hotel_or_office==0)  // office - unoccupied ,2sp
    {
        // 2sp
        strTemp = _T("Two SetPoint Mode");
        //itemp=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
        //m_nightSlider.SetPos((int)(max-(itemp-min)));
        //  nCoolSP = (int)itemp;//(int)(max-(itemp-min));
        if (g_unint)
        {
            strTemp.Format(_T("%.1f C"),(float)nCoolSP);
        }
        else
        {
            strTemp.Format(_T("%.1f F"),(float)nCoolSP);
        }
        m_nightInfoEdit.FieldText(RGB(0,0,0),strTemp);


        if (g_unint)
        {
            strTemp.Format(_T("%.1f C"),(float)nHeatSP);
        }
        else
        {
            strTemp.Format(_T("%.1f F"),(float)nHeatSP);
        }
        m_nightHeatInfoEdit.FieldText(RGB(0,0,0),strTemp);

        strTemp.Format(_T("%0.1f"),(float)nSP);
        m_nightpotEdit.FieldText(RGB(0,0,0),strTemp);




        if (nCoolSP == nHeatSP && nCoolSP== nSP )
        {
            nSP = nHeatSP + 1;
            nCoolSP = nSP+1;
        }



        //m_pNightTwoSP->SetRange(nRangeMax, nRangeMin);
        //BOOL bSetPos = m_pNightTwoSP->SetPos(nHeatSP, nCoolSP, nCoolSP+1);
        //HandleSliderSetPos(bSetPos);

        //m_pNightTwoSP->ShowWindow(SW_NORMAL);
        //m_pNightSingleSP->ShowWindow(SW_HIDE);
// 		m_pNightSingleSP->SetRange(DayMax*10, DayMin*10);
// 		BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(nHeatSP*10, nSP*10, nCoolSP*10);
// 		HandleSliderSetPos(bRetSP);
// 		m_pNightSingleSP->ShowWindow(SW_NORMAL);
// 		m_pNightTwoSP->ShowWindow(SW_HIDE);
        m_pNightTwoSP->SetRange(DayMax*10, DayMin*10);
        BOOL bSetPos = m_pNightTwoSP->SetPos(nHeatSP*10, nSP*10, nCoolSP*10);
        HandleSliderSetPos(bSetPos);

        m_pNightTwoSP->ShowWindow(SW_NORMAL);
        m_pNightSingleSP->ShowWindow(SW_HIDE);
    }
    //m_pNightSingleSP && !bOccupied
    else  // unoccupied-hotel
    {
        // 1sp
        //strTemp = _T("Hotel");


        if (g_unint)
        {
            strTemp.Format(_T("%.1f C"),(float)nCoolSP);
        }
        else
        {
            strTemp.Format(_T("%.1f F"),(float)nCoolSP);
        }
        m_nightInfoEdit.FieldText(RGB(0,0,0),strTemp);

        if (g_unint)
        {
            strTemp.Format(_T("%.1f C"),(float)nHeatSP);
        }
        else
        {
            strTemp.Format(_T("%.1f F"),(float)nHeatSP);
        }
        m_nightHeatInfoEdit.FieldText(RGB(0,0,0),strTemp);


        strTemp.Format(_T("%0.1f"), (float)nSP);
        m_nightpotEdit.FieldText(RGB(0,0,0),strTemp);

        if (nCoolSP == nHeatSP && nHeatSP == 0)
        {
            nHeatSP = nSP - 1;
            nCoolSP = nSP+1;
        }

        /*m_pNightSingleSP->SetRange(nRangeMax, nRangeMin);
        BOOL bRetSP = m_pNightSingleSP->SetPos(nHeatSP, nSP, nCoolSP);
        HandleSliderSetPos(bRetSP);
        m_pNightSingleSP->ShowWindow(SW_NORMAL);
        m_pNightTwoSP->ShowWindow(SW_HIDE);*/

        if (nHeatSP<nSP&&nSP<nCoolSP)
        {
        }
        else
        {
            DayMax =5;
            DayMin = 0;
            nHeatSP = 1;
            nSP = 2;
            nCoolSP = 3;
        }
        m_pNightSingleSP->SetRange(DayMax*10, DayMin*10);
        BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(nHeatSP*10, nSP*10, nCoolSP*10);
        HandleSliderSetPos(bRetSP);
        m_pNightSingleSP->ShowWindow(SW_NORMAL);

        m_pNightTwoSP->ShowWindow(SW_HIDE);
    }



}
#endif







void CT3000View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CT3000View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

//     if (pScrollBar->m_hWnd == GetDlgItem(IDC_FSB_DAYHOTEL)->m_hWnd)
//     {
//         AfxMessageBox(_T("OK"));
//     }
    CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
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
            || nModel==3||nModel==PM_TSTAT6||nModel==PM_TSTAT5i||nModel==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8))//A,B,C,D ,F,G,6,7
    {
        //strTemp.Format(_T("Input %d"),1);
        //Read_Multi(g_tstat_id,&product_register_value[MODBUS_INTERNAL_THERMISTOR],MODBUS_INTERNAL_THERMISTOR,2);
        //	Read_Multi(g_tstat_id,&product_register_value[MODBUS_EXTERNAL_SENSOR_0],MODBUS_EXTERNAL_SENSOR_0,5);
        //m_fTemperature=(float)product_register_value[MODBUS_TEMPRATURE_CHIP]/10;
        //strTemp.Format(_T("%0.1f"),m_fTemperature);
        //   GetDlgItem(IDC_TEMPERATURE_EDIT)->SetWindowTextW(strTemp);
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




        if (m_InRanges[1]!=-1)//有表但是没有对应序列号的值
        {
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
            if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)//188
            {


                if (m_InRanges[1]==6||m_InRanges[1]==7)
                {

                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
                        strTemp=_T("Occupied");
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
                        strTemp=_T("Unoccupied");


                }
                else
                {

                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
                        strTemp=_T("Off");
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
                        strTemp=_T("On");

                }


            }


        }
        else
        {
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
            if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)//188
            {





                if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
                    strTemp=_T("Off");
                if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
                    strTemp=_T("On");




            }
        }





        if (nModel==18)//2.5.0.98
        {
            if (product_register_value[MODBUS_ANALOG_IN1] == 0)//188
            {
                //strTemp = _T("UNUSED");
                strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);
            }
        }

        m_Input_Grid.put_TextMatrix(2,2,strTemp);
        strTemp.Empty();


    }


    if(nModel==PM_TSTAT5C||nModel==PM_TSTAT5D||nModel==17||nModel==18)
    {




        if (m_InRanges[2]!=-1)//有表但是没有对应序列号的值
        {



            strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN2], 1);//188
            if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//188
            {
                strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);//180
                strTemp=strTemp+strUnit;
            }
            if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)//188
            {
                //strUnit=GetTempUnit(180);
                strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//180
            }
            if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)//188
            {


                if (m_InRanges[2]==6||m_InRanges[2]==7)
                {


                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
                        strTemp=_T("Unoccupied");
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
                        strTemp=_T("Occupied");

                }
                else
                {

                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
                        strTemp=_T("Off");
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
                        strTemp=_T("On");

                }





            }
        }
        else
        {
            strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN2], 1);//188
            if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//188
            {
                strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);//180
                strTemp=strTemp+strUnit;
            }
            if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)//188
            {
                //strUnit=GetTempUnit(180);
                strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//180
            }
            if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)//188
            {





                if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
                    strTemp=_T("Off");
                if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
                    strTemp=_T("On");




            }
        }



        if (nModel==18)//2.5.0.98
        {
            if (product_register_value[MODBUS_ANALOG_IN2] == 0)//188
            {
                //strTemp = _T("UNUSED");
                strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);
            }
        }
        m_Input_Grid.put_TextMatrix(3,1,g_strInName2);
        m_Input_Grid.put_TextMatrix(3,2,strTemp);







        strTemp.Empty();
        m_Input_Grid.put_TextMatrix(4,1,g_strInName3);






        if (m_InRanges[3]!=-1)//有表但是没有对应序列号的值
        {


            if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_InRanges[3]==0))//190
            {

                if(product_register_value[311]==0)
                {
                    strTemp=_T("Off");
                }
                else
                {
                    strTemp=_T("On");
                }



            }
            else  if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_InRanges[3]==2))//190
            {
                CString strTemp2;
                if (product_register_value[311]==1)
                {
                    strTemp=_T("Occupied");
                }
                else
                {
                    strTemp=_T("UnOccupied");
                }


            }
            if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_InRanges[3]==1))//190
            {

                if (product_register_value[311]==0)
                {
                    strTemp=_T("Off");

                }
                else
                {
                    strTemp=_T("On");
                }


            }
            else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_InRanges[3]==3))//190
            {
                CString strTemp2;
                if (product_register_value[311]==0)
                {
                    strTemp=_T("UnOccupied");

                }
                else
                {
                    strTemp=_T("Occupied");
                }

            }


        }
        else
        {
            if (product_register_value[MODBUS_DIGITAL_IN1]==0)//190
            {
                CString strTemp2;
                if (product_register_value[311]==1)
                {
                    strTemp=_T("On");
                }
                else
                {
                    strTemp=_T("Off");
                }

            }
            if (product_register_value[MODBUS_DIGITAL_IN1]==1)//190
            {
                CString strTemp2;
                if (product_register_value[311]==0)
                {
                    strTemp=_T("Off");

                }
                else
                {
                    strTemp=_T("On");
                }

            }

        }




        m_Input_Grid.put_TextMatrix(4,2,strTemp);

    }

    strTemp.Empty();





    if(nModel == PM_TSTAT6 ||nModel == PM_TSTAT7||(product_register_value[7] == PM_TSTAT8) ||nModel == PM_TSTAT5i)// Tstat 6
    {
        int m_crange=0;
        //Mdb_Adress_Map = T3000_6_ADDRESS; //让其在 T3000_6_ADDRESS 中 进行地址 Maping;
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
		m_Input_Grid.put_TextMatrix(12,1,g_strLightingSensor);
        strTemp.Empty();
// 		if (is_connect())
// 		{
// 			int ret=Read_Multi(g_tstat_id,&product_register_value[MODBUS_TEMPRATURE_CHIP],MODBUS_TEMPRATURE_CHIP,5);
// 			ret=Read_Multi(g_tstat_id,&product_register_value[MODBUS_ANALOG_INPUT1],MODBUS_ANALOG_INPUT1,10);
// 		}

        if(m_fFirmwareVersion>33.3)
        {

            BOOL m_disable_hum,m_disable_CO2;
            float nValue;
            for(int i=0; i<9; i++)
            {
                if ( i == 0)
                {
                    //101	121	2	Full	W/R	TEMPERATURE  reading in DegC or F from the sensor used in the control loop PI1 which is configured in register 111. This can be the internal sensor, external, or an average of the two. Writing a temperature value to this register will calibrate the currently selected sensor by adjusting the associated calibration register. If average is selected then you cannot write to this register.
                    //float fTemp= (float)(product_register_value[216]/10.0);//tstat6
                    float fTemp= (float)(product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//121
                    strTemp.Format(_T("%.1f  C"),fTemp);
                    //if (product_register_value[359] == 1)
                    //121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1
                    if (product_register_value[MODBUS_DEGC_OR_F] == 1) //104
                    {
                        strTemp.Format(_T("%.1f  F"),fTemp);
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



                    if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==1) //122
                    {
                        nValue= (float)(product_register_value[MODBUS_ANALOG_INPUT1+i-1]/10.0); //131
                        strTemp.Format(_T("%.1f"),nValue);
                    }
                    else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==3||product_register_value[MODBUS_ANALOG1_RANGE+i-1]==5) // On/Off
                    {
                        if (m_InRanges[i]==9||m_InRanges[i]==10)
                        {
                            nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//131
                            strTemp= nValue ==1 ? _T("Open") : _T("Closed");
                        }
                        else if (m_InRanges[i]==7||m_InRanges[i]==8)
                        {
                            nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//131
                            strTemp= nValue ==1 ? _T("Occupied") : _T("Unoccupied");
                        }
                        else
                        {
                            nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];//131
                            strTemp= nValue ==1 ? _T("On") : _T("Off");
                        }

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
// 					else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==5) // Off/On  //122
// 					{
// 						nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-1];
// 						strTemp= nValue ==1 ? _T("On") : _T("Off");
// 					}
                    else
                    {
                        strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+i-1]);//tatat6
                        //strTemp=_T("UNUSED");
                    }

                }
                m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
            }

            if((product_register_value[20]&2)==2)
            {
                m_disable_hum=TRUE;
            }
            else
            {
                m_disable_hum=FALSE;
            }
            if((product_register_value[MODBUS_TSTAT6_CO2_AVALUE]>=0)&&(product_register_value[MODBUS_TSTAT6_CO2_AVALUE]<=3000))
            {
                m_disable_CO2=TRUE;
            }
            else
            {
                m_disable_CO2=FALSE;
            }
            if (!m_disable_hum)
            {
                for (int col=0; col<3; col++)
                {
                    m_Input_Grid.put_Col(col);
                    m_Input_Grid.put_Row(10);
                    m_Input_Grid.put_CellBackColor(DISABLE_COLOR_CELL);
                }
            }
            else
            {
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
            }

            if (!m_disable_CO2)
            {
                for (int col=0; col<3; col++)
                {
                    m_Input_Grid.put_Col(col);
                    m_Input_Grid.put_Row(11);
                    m_Input_Grid.put_CellBackColor(DISABLE_COLOR_CELL);
                }
            }
            else
            {


                strTemp.Format(_T("%dppm"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
                m_Input_Grid.put_TextMatrix(11,2,strTemp);



            }

            strTemp.Format(_T("%d LUX"),product_register_value[MODBUS_VALUE_SENSOR]);
            m_Input_Grid.put_TextMatrix(12,2,strTemp);
            /*if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
            {     strUnit=_T("ppm");
            m_FlexGrid.put_TextMatrix(11,AM_FIELD,strAuto);
            temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
            temp+=strUnit;
            m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
            }
            else
            {
            	m_FlexGrid.put_TextMatrix(11,AM_FIELD,strman);
            	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
            	temp+=strUnit;
            	m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
            }*/

        }
        else
        {

            //Mdb_Adress_Map = 1; //让其在 T3000_5ABCDFG_LED_ADDRESS 中 进行地址 Maping;
            float nValue;
            for(int i=0; i<8; i++)
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
    if(nModel==PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)) // E
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
        //m_Input_Grid.put_TextMatrix(10,1,g_strInHumName);



        //CString analog_range_0[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
        strTemp.Empty();
        if(m_fFirmwareVersion>33.3)
        {
            float nValue;
            for(int i=0; i<9; i++)
            {
                if ( i == 0)
                {
                    //float fTemp= (float)(product_register_value[216]/10.0);//tstat6
                    float fTemp= (float)(product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//101
                    strTemp.Format(_T("%.1f  C"),fTemp);
                    //if (product_register_value[359] == 1)
                    if (product_register_value[MODBUS_DEGC_OR_F] == 1)//121
                    {
                        strTemp.Format(_T("%.1f  F"),fTemp);
                    }
                }
                else
                {
                    CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-1], 1);//359
                    if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]==1)//359
                    {
                        nValue= (float)(product_register_value[MODBUS_ANALOG_INPUT1+i-1]/10.0);//stat6 如是这个则显示UNUSE，未使用  //367
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
                    }
                    else if(product_register_value[MODBUS_ANALOG1_RANGE+i-1]== 4)  // Off/On//359
                    {
                        nValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-1]/10.0);//359
                        strTemp.Format(_T("%.1f"), (float)nValue/10.0);
                        strTemp +=strValueUnit;



                    }
                    else
                    {
                        strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+i-1]);//tatat6
                        //strTemp=_T("UNUSED");
                    }
                }
                m_Input_Grid.put_TextMatrix(1+i,2,strTemp);
            }
        }
        else
        {
            float nValue;
            for(int i=0; i<8; i++)
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





    if (nModel == PM_PRESSURE)
    {
        Mdb_Adress_Map = T3000_5ABCDFG_LED_ADDRESS;

        CString strRow1 = _T("Pressure sensor");
        m_Input_Grid.put_TextMatrix(1,1,strRow1);
        CString strTemp;
        strTemp.Format(_T("%.1f W.C"), product_register_value[MODBUS_EXTERNAL_SENSOR_0]/100.0);//180
        m_Input_Grid.put_TextMatrix(1,2,strTemp);
    }


    if (nModel == PM_TSTAT7)
    {
        for (int row=6; row<=11; row++)
        {
            //   m_Input_Grid.put_TextMatrix(row,0,_T("UNUSED"));
            m_Input_Grid.put_TextMatrix(row,1,_T("UNUSED"));
            m_Input_Grid.put_TextMatrix(row,2,_T("UNUSED"));
        }

    }

    //Mdb_Adress_Map =temp_address_map  ;
}




void CT3000View::Fresh_Out()
{
//	Savetmp_Mdb_Adress_Map();
    int nRange;
    int nModel=product_register_value[MODBUS_PRODUCT_MODEL];


//	switch (nModel)
    CString strTemp;
    int itemp=product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];
    //first 3
    //==========================================================================
    //ABCDEFG,noH
    if(nModel==2||nModel==1||nModel==4||nModel==12||nModel==16||nModel==PM_PM5E	||nModel==17||nModel==18||nModel==3
            ||nModel==PM_TSTAT6||nModel==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT8)||nModel==PM_TSTAT7||nModel==PM_PRESSURE)
    {
        m_Output_Grid.put_TextMatrix(1,1,g_strOutName1);
        m_Output_Grid.put_TextMatrix(2,1,g_strOutName2);
        m_Output_Grid.put_TextMatrix(3,1,g_strOutName3);

        for(int i=1; i<=3; i++)
        {
            //strTemp.Format(_T("Output %d"),i);
            if (m_OutRanges[i]!=-1)
            {
                if (m_OutRanges[i]==0||m_OutRanges[i]==4||m_OutRanges[i]==5||m_OutRanges[i]==6)
                {
                    if(itemp&(1<<(i-1)))
                    {
                        if (m_OutRanges[i]==0||m_OutRanges[i]==4)
                        {
                            strTemp=_T("On");
                        }
                        else if (m_OutRanges[i]==5||m_OutRanges[i]==6)
                        {
                            strTemp=_T("Open");
                        }


                    }
                    else
                    {
                        if (m_OutRanges[i]==0||m_OutRanges[i]==4)
                        {
                            strTemp=_T("Off");
                        }
                        else if (m_OutRanges[i]==5||m_OutRanges[i]==6)
                        {
                            strTemp=_T("Closed");
                        }
                    }
                }
                else
                {
                    //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
                    int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
                    strTemp.Format(_T("%d%%"), nValueTemp);
                }
            }
            else
            {
                nRange=product_register_value[MODBUS_MODE_OUTPUT1+i-1];
                m_OutRanges[i] =  nRange;
                if (m_OutRanges[i]==0||m_OutRanges[i]==4||m_OutRanges[i]==5||m_OutRanges[i]==6)
                {
                    if(itemp&(1<<(i-1)))
                    {
                        if (m_OutRanges[i]==0||m_OutRanges[i]==4)
                        {
                            strTemp=_T("On");
                        }
                        else if (m_OutRanges[i]==5||m_OutRanges[i]==6)
                        {
                            strTemp=_T("Open");
                        }


                    }
                    else
                    {
                        if (m_OutRanges[i]==0||m_OutRanges[i]==4)
                        {
                            strTemp=_T("Off");
                        }
                        else if (m_OutRanges[i]==5||m_OutRanges[i]==6)
                        {
                            strTemp=_T("Closed");
                        }
                    }
                }
                else
                {
                    //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
                    int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
                    strTemp.Format(_T("%d%%"), nValueTemp);
                }

            }


            m_Output_Grid.put_TextMatrix(i,2,strTemp);
            strTemp.Empty();
        }
    }

    ////----------------------------------------------------------------------
    //4,5
    if(nModel==1||nModel==4||nModel==12||nModel==16||nModel==3||nModel==PM_PM5E
            ||nModel==PM_TSTAT6||nModel==PM_TSTAT5i||(nModel == PM_TSTAT8)||nModel==PM_TSTAT7||nModel==PM_PRESSURE)//B,C,D,E
    {


        nRange = product_register_value[MODBUS_MODE_OUTPUT4];

        if (m_OutRanges[3]!=-1)
        {
            if(m_OutRanges[3] == 0||m_OutRanges[3]==4||m_OutRanges[3]==5||m_OutRanges[3]==6 ) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
            {
                // output is on/off


                if(itemp&8)
                {
                    if (m_OutRanges[3]==0||m_OutRanges[3]==4)
                    {
                        strTemp=_T("On");
                    }
                    else if (m_OutRanges[3]==5||m_OutRanges[3]==6)
                    {
                        strTemp=_T("Open");
                    }


                }
                else
                {
                    if (m_OutRanges[3]==0||m_OutRanges[3]==4)
                    {
                        strTemp=_T("Off");
                    }
                    else if (m_OutRanges[3]==5||m_OutRanges[3]==6)
                    {
                        strTemp=_T("Closed");
                    }
                }

            }
            else // output is value
            {
                //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
                int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
                strTemp.Format(_T("%d%%"), nValueTemp);
            }

        }
        else
        {
            if(nRange == 0||nRange==4||nRange==5||nRange==6 ) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
            {
                // output is on/off


                if(itemp&8)
                {
                    if (nRange==0||nRange==4)
                    {
                        strTemp=_T("On");
                    }
                    else if (nRange==5||nRange==6)
                    {
                        strTemp=_T("Open");
                    }


                }
                else
                {
                    if (nRange==0||nRange==4)
                    {
                        strTemp=_T("Off");
                    }
                    else if (nRange==5||nRange==6)
                    {
                        strTemp=_T("Closed");
                    }
                }

            }
            else // output is value
            {
                //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
                int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
                strTemp.Format(_T("%d%%"), nValueTemp);
            }
        }



        m_Output_Grid.put_TextMatrix(4,2,strTemp);
        //	strTemp.Format(_T("Output %d"),4);
        m_Output_Grid.put_TextMatrix(4,1,g_strOutName4);

        strTemp.Empty();
        if(itemp &( 1<<4))
            strTemp=_T("ON");
        else
            strTemp=_T("OFF");


        nRange = product_register_value[MODBUS_MODE_OUTPUT5];


        if (m_OutRanges[4]!=-1)
        {
            if(m_OutRanges[4] == 0||m_OutRanges[4]==4||m_OutRanges[4]==5||m_OutRanges[4]==6 ) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
            {
                // output is on/off


                if(itemp&16)
                {
                    if (m_OutRanges[4]==0||m_OutRanges[4]==4)
                    {
                        strTemp=_T("On");
                    }
                    else if (m_OutRanges[4]==5||m_OutRanges[4]==6)
                    {
                        strTemp=_T("Open");
                    }


                }
                else
                {
                    if (m_OutRanges[4]==0||m_OutRanges[4]==4)
                    {
                        strTemp=_T("Off");
                    }
                    else if (m_OutRanges[4]==5||m_OutRanges[4]==6)
                    {
                        strTemp=_T("Closed");
                    }
                }

            }
            else // output is value
            {
                //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
                int nValueTemp = product_register_value[MODBUS_PWM_OUT5]; //348 //598
                strTemp.Format(_T("%d%%"), nValueTemp);
            }
        }
        else
        {
            if(nRange == 0||nRange==4||nRange==5||nRange==6 ) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
            {
                // output is on/off


                if(itemp&16)
                {
                    if (nRange==0||nRange==4)
                    {
                        strTemp=_T("On");
                    }
                    else if (nRange==5||nRange==6)
                    {
                        strTemp=_T("Open");
                    }


                }
                else
                {
                    if (nRange==0||nRange==4)
                    {
                        strTemp=_T("Off");
                    }
                    else if (nRange==5||nRange==6)
                    {
                        strTemp=_T("Closed");
                    }
                }

            }
            else // output is value
            {
                //comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
                int nValueTemp = product_register_value[MODBUS_PWM_OUT5]; //348 //598
                strTemp.Format(_T("%d%%"), nValueTemp);
            }
        }


        m_Output_Grid.put_TextMatrix(5,2,strTemp);

        //strTemp.Format(_T("Output %d"),5);
        m_Output_Grid.put_TextMatrix(5,1,g_strOutName5);
        strTemp.Empty();
    }

    //:DEG 6. 7
    nRange=product_register_value[MODBUS_OUTPUT1_SCALE];
    if(nModel==12||nModel==16||nModel==PM_PM5E||nModel==18||nModel==PM_TSTAT6||(product_register_value[7] == PM_TSTAT8)||nModel==PM_TSTAT5i||nModel==PM_TSTAT7||nModel==PM_PRESSURE)
    {
        if(nRange==0)
        {
            //itemp=product_register_value[102];
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
            //strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
            float nvalue=0.0;

            //if(nRange==1)//0-10v
            //{
            //nvalue=product_register_value[102]/100 /10.0 * 100%;
            nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/10.0);
            //}
            //if(nRange==2)//0-5v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/5.0);
            //}
            //if(nRange==3)//2-10v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/8.0);
            //}
            //if(nRange==4)//10-0v
            //{
            //	nvalue=float((10-product_register_value[MODBUS_COOLING_VALVE]/100.0)/10.0 *100);
            //}
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
            //itemp=product_register_value[MODBUS_HEATING_VALVE];
            itemp=product_register_value[103];
            if(itemp==0)
                strTemp=_T("OFF");
            if(itemp==1)
                strTemp=_T("ON");

        }
        else
        {
            //strTemp.Format(_T("%.1f"),product_register_value[103]/100.0);
            float nvalue=0.0;

            //if(nRange==1)//0-10v
            //{
            //nvalue=product_register_value[102]/100 /10.0 * 100%;
            nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/10.0);//T5 =102
            //}
            //if(nRange==2)//0-5v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/5.0);
            //}
            //if(nRange==3)//2-10v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/8.0);
            //}
            //if(nRange==4)//10-0v
            //{
            //	nvalue=(float)((10-product_register_value[MODBUS_HEATING_VALVE]/100)/10.0*100);
            //}
            strTemp.Format(_T("%.1f%%"),nvalue);

        }
        m_Output_Grid.put_TextMatrix(7,2,strTemp);
        //	strTemp.Format(_T("Output %d"),7);
        m_Output_Grid.put_TextMatrix(7,1,g_strOutName7);
        strTemp.Empty();
    }
    if(nModel==2||nModel==1||nModel==3)//A 4,5
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
            //strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
            float nvalue=0.0;
            //if(nRange==1)//0-10v
            //{
            //nvalue=product_register_value[102]/100 /10.0 * 100%;
            nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/10.0);
            //}
            //if(nRange==2)//0-5v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/5.0);
            //}
            //if(nRange==3)//2-10v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_COOLING_VALVE]/8.0);
            //}
            //if(nRange==4)//10-0v
            //{
            //	nvalue=(float)((10-product_register_value[MODBUS_COOLING_VALVE]/100.0)/10.0 *100);
            //}
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
            //strTemp.Format(_T("%.1f"),product_register_value[103]/100.0);
            float nvalue=0.0;
            //if(nRange==1)//0-10v
            //{
            //nvalue=product_register_value[102]/100 /10.0 * 100%;
            nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/10.0);
            //}
            //if(nRange==2)//0-5v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/5.0);
            //}
            //if(nRange==3)//2-10v
            //{
            //	nvalue=(float)(product_register_value[MODBUS_HEATING_VALVE]/8.0);
            //}
            //if(nRange==4)//10-0v
            //{
            //	nvalue=(float)((10-product_register_value[MODBUS_HEATING_VALVE]/100.0)/10.0 *100);
            //}
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

    if (product_type==T3000_6_ADDRESS)
    {

        FreshIOGridTable_Tstat6();
        return;
    }
    BOOL Is_tstat=TRUE;
    //m_Output_Grid.Clear();
    //m_Input_Grid.Clear();
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

    case PM_TSTAT5D:  // 5D 同 TStat7
    {
        m_outRows=8;
        m_inRows=5;

    }
    break;
    case PM_TSTAT7:
    case PM_TSTAT6:
    {
        m_outRows=8;
        m_inRows=13;
    }
    break;
    case PM_TSTAT5i:
    {
        m_outRows=8;
        m_inRows=13;
    }
    break;
    case PM_TSTAT8:
    {
        m_outRows = 8;
        m_inRows = 13;
    }
    case PM_TSTAT5E:
    {
        m_outRows=8;
        m_inRows=11;
    }
    break;
    case PM_PM5E:
    {
        m_outRows=8;
        m_inRows=11;
    }
    break;
    case PM_TSTATRUNAR:
    {
        m_outRows=8;
        m_inRows=11;
    }
    break;
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

    default:
    {
        Is_tstat=FALSE;

    }
    break;
    }
// 	m_Input_Grid.put_Rows(m_inRows);
// 	m_Output_Grid.put_Rows(m_outRows);

    m_Input_Grid.put_Rows(m_inRows);
    m_Output_Grid.put_Rows(m_outRows);


    for(int i=1; i<m_inRows; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"),i);
        m_Input_Grid.put_TextMatrix(i,0,strTemp);
    }

    for(int i=1; i<m_outRows; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"),i);
        m_Output_Grid.put_TextMatrix(i,0,strTemp);
    }
    for(int i=0; i<=2; i++)
    {
        m_Input_Grid.put_ColAlignment(i,4);
        m_Output_Grid.put_ColAlignment(i,4);
    }

    m_Input_Grid.put_TextMatrix(0,1,_T("Name"));
    m_Input_Grid.put_TextMatrix(0,2,_T("Value"));



    m_Output_Grid.put_TextMatrix(0,1,_T("Name"));
    m_Output_Grid.put_TextMatrix(0,2,_T("Value"));

    for(int i=1; i<m_inRows; i++)
    {

        for(int k=0; k<=2; k++)
        {
            if (i%2==1)
            {
                m_Input_Grid.put_Row(i);
                m_Input_Grid.put_Col(k);
                m_Input_Grid.put_CellBackColor(RGB(255,255,255));
            }
            else
            {
                m_Input_Grid.put_Row(i);
                m_Input_Grid.put_Col(k);
                m_Input_Grid.put_CellBackColor(COLOR_CELL);
            }
        }
    }

    for(int i=1; i<m_outRows; i++)
    {

        for(int k=0; k<=2; k++)
        {
            if (i%2==1)
            {
                m_Output_Grid.put_Row(i);
                m_Output_Grid.put_Col(k);
                m_Output_Grid.put_CellBackColor(RGB(255,255,255));
            }
            else
            {
                m_Output_Grid.put_Row(i);
                m_Output_Grid.put_Col(k);
                m_Output_Grid.put_CellBackColor(COLOR_CELL);
            }
        }
    }




    if (Is_tstat)
    {
        Fresh_Out();
        Fresh_In();
    }




}
void CT3000View::FreshIOGridTable_Tstat6()
{
    m_Input_Grid.put_Rows(13);
    m_Input_Grid.put_Cols(3);
    CString strTemp;
    m_Input_Grid.put_TextMatrix(0,1,_T("Name"));
    m_Input_Grid.put_TextMatrix(0,2,_T("Value"));
    for (int i=0; i<(int)m_tstat_input_data.size(); i++)
    {
        strTemp.Format(_T("%d"),i);
        m_Input_Grid.put_TextMatrix(i+1,0,strTemp);
        m_Input_Grid.put_TextMatrix(i+1,1,m_tstat_input_data.at(i).InputName.StrValue);
//        if (m_tstat_input_data.at(i).Unit.StrValue.CompareNoCase(NO_APPLICATION)==0)
//        {
        strTemp.Format(_T("%s"),m_tstat_input_data.at(i).Value.StrValue);
//        }
//        else
//        {
//           strTemp.Format(_T("%s%s"),m_tstat_input_data.at(i).Value.StrValue,m_tstat_input_data.at(i).Unit.StrValue);
//        }

        m_Input_Grid.put_TextMatrix(i+1,2,strTemp);
    }



    m_Output_Grid.put_Rows(8);
    m_Output_Grid.put_Cols(3);
    m_Output_Grid.put_TextMatrix(0,1,_T("Name"));
    m_Output_Grid.put_TextMatrix(0,2,_T("Value"));
    for (int i=0; i<(int)m_tstat_output_data.size(); i++)
    {
        strTemp.Format(_T("%d"),i+1);
        m_Output_Grid.put_TextMatrix(i+1,0,strTemp);
        m_Output_Grid.put_TextMatrix(i+  1,1,m_tstat_output_data.at(i).OutputName.StrValue);

        strTemp.Format(_T("%s"),m_tstat_output_data.at(i).Value.StrValue);


        m_Output_Grid.put_TextMatrix(i+1,2,strTemp);
    }

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
//pMain->m_pFreshMultiRegisters->SuspendThread();
    pMain->m_pRefreshThread->SuspendThread();

    CParameterDlg dlg(this,m_strModelName);
    dlg.DoModal();
    Fresh();

//pMain->m_pFreshMultiRegisters->ResumeThread();//tstat6
    pMain->m_pRefreshThread->ResumeThread();

    g_bEnableRefreshTreeView = TRUE;

    // TODO: Add your control notification handler code here
}

BEGIN_EVENTSINK_MAP(CT3000View, CFormView)
ON_EVENT(CT3000View, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CT3000View::ClickInputMsflexgrid, VTS_NONE)
ON_EVENT(CT3000View, IDC_OUTPUT_MSFLEXGRID, DISPID_CLICK, CT3000View::ClickOutputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void CT3000View::ClickInputMsflexgrid()
{

    long lRow,lCol;
    lRow = m_Input_Grid.get_RowSel();//获取点击的行号
    lCol = m_Input_Grid.get_ColSel(); //获取点击的列号

    CRect rect;
    m_Input_Grid.GetWindowRect(rect); //获取表格控件的窗口矩形
    ScreenToClient(rect); //转换为客户区矩形
    CDC* pDC =GetDC();

    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    //计算选中格的左上角的坐标(象素为单位)
    long y = m_Input_Grid.get_RowPos(lRow)/nTwipsPerDotY;
    long x = m_Input_Grid.get_ColPos(lCol)/nTwipsPerDotX;
    //计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
    long width = m_Input_Grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
    long height = m_Input_Grid.get_RowHeight(lRow)/nTwipsPerDotY+1;
    //形成选中个所在的矩形区域
    CRect rcCell(x,y,x+width,y+height);
    //转换成相对对话框的坐标
    rcCell.OffsetRect(rect.left+1,rect.top+1);
    ReleaseDC(pDC);
    CString strValue = m_Input_Grid.get_TextMatrix(lRow,lCol);


    if(1==lCol && lRow <=8)
    {
       // return; // 2012.2.7老毛说不允许修改
        m_inNameEdt.MoveWindow(&rcCell,1);
        m_inNameEdt.ShowWindow(SW_SHOW);
        m_inNameEdt.SetWindowText(strValue);
        m_inNameEdt.SetFocus();
        m_inNameEdt.SetCapture();//LSC
        int nLenth=strValue.GetLength();
        m_inNameEdt.SetSel(nLenth,nLenth); //全选//

    }


}

void CT3000View::ClickOutputMsflexgrid()
{
    long lRow,lCol;
    lRow = m_Output_Grid.get_RowSel();//获取点击的行号
    lCol = m_Output_Grid.get_ColSel(); //获取点击的列号

    TRACE(_T("Click output grid!\n"));

    CRect rect;
    m_Output_Grid.GetWindowRect(rect); //获取表格控件的窗口矩形
    ScreenToClient(rect); //转换为客户区矩形
    CDC* pDC =GetDC();

    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    //计算选中格的左上角的坐标(象素为单位)
    long y = m_Output_Grid.get_RowPos(lRow)/nTwipsPerDotY;
    long x = m_Output_Grid.get_ColPos(lCol)/nTwipsPerDotX;
    //计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
    long width = m_Output_Grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
    long height = m_Output_Grid.get_RowHeight(lRow)/nTwipsPerDotY+1;
    //形成选中个所在的矩形区域
    CRect rcCell(x,y,x+width,y+height);
    //转换成相对对话框的坐标
    rcCell.OffsetRect(rect.left+1,rect.top+1);
    ReleaseDC(pDC);
    CString strValue = m_Output_Grid.get_TextMatrix(lRow,lCol);


    if(1==lCol)
    {
       // return; // don not permit change by customer
        m_outNameEdt.MoveWindow(&rcCell,1);
        m_outNameEdt.ShowWindow(SW_SHOW);
        m_outNameEdt.SetWindowText(strValue);
        m_outNameEdt.SetFocus();
        int nLenth=strValue.GetLength();
        m_outNameEdt.SetSel(nLenth,nLenth); //全选//
    }
}

void CT3000View::OnEnKillfocusInputnameedit()
{
    CString strText;
    m_inNameEdt.GetWindowText(strText);
    m_inNameEdt.ShowWindow(SW_HIDE);
    int lRow = m_Input_Grid.get_RowSel();//获取点击的行号
    int lCol = m_Input_Grid.get_ColSel(); //获取点击的列号

    CString strInName;
    if(lCol!=1||lRow==0)
        return;
    switch (lRow)
    {

    case 1:
        strInName=g_strInName1;
        break;
    case 2:
        strInName=g_strInName2;
        break;
    case 3:
        strInName=g_strInName3;
        break;
    case 4:
        strInName=g_strInName4;
        break;
    case 5:
        strInName=g_strInName5;
        break;
    case 6:
        strInName=g_strInName6;
        break;
    case 7:
        strInName=g_strInName7;
        break;
    case 8:
        strInName=g_strInName8;
        break;
	case 9:
		strInName=g_strSensorName;
		break;
    case 10:
        strInName=g_strInHumName;
        break;
    }
    if(strText.CompareNoCase(strInName)==0)
        return;
	if(strText.CompareNoCase(m_tstat_input_data.at(lRow-1).InputName.StrValue)==0)
		return;
		

    //if(g_serialNum>0&&product_register_value[6]>0)

   


	if (product_register_value[7]== PM_TSTAT6||product_register_value[7]==PM_TSTAT8||product_register_value[7]==PM_TSTAT7)
	{
	 
		char cTemp1[8];
		memset(cTemp1,0,8);
		WideCharToMultiByte( CP_ACP, 0, strText.GetBuffer(), -1, cTemp1, 8, NULL, NULL );
		unsigned char UChar[8];
		for (int i=0;i<8;i++)
		{
			UChar[i]=cTemp1[i];
		}
		int ret = Write_Multi(g_tstat_id,UChar,m_tstat_input_data.at(lRow-1).InputName.regAddress,8);
		if (ret>0)
		{
			m_tstat_input_data.at(lRow-1).InputName.StrValue=strText;
		}
		FreshIOGridTable_Tstat6();
	}
	else
	{
		 if(product_register_value[6]>0)
    {
        try
        {
			CppSQLite3Table table;
			CppSQLite3Query q;

			CppSQLite3DB SqliteDBBuilding;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

            CString strSerial;
            strSerial.Format(_T("%d"),g_serialNum);

            CString StrSql;
            StrSql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
            q = SqliteDBBuilding.execQuery((UTF8MBSTR)StrSql);
            if(!q.eof())//update
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
                   
                    SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
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

                     
					SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
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
           SqliteDBBuilding.closedb();
        }
        catch(...)
        {

        }
    }
	}
}

void CT3000View::OnEnKillfocusOutputnameedit()
{

    CString strText;
    m_outNameEdt.GetWindowText(strText);
    m_outNameEdt.ShowWindow(SW_HIDE);
    int lRow = m_Output_Grid.get_RowSel();//获取点击的行号
    int lCol = m_Output_Grid.get_ColSel(); //获取点击的列号

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
	if(strText.CompareNoCase(m_tstat_output_data.at(lRow-1).OutputName.StrValue)==0)
		return;

    //if(g_serialNum>0&&product_register_value[6]>0)
	if (  product_register_value[7]== PM_TSTAT6
		||product_register_value[7]==PM_TSTAT8
		||product_register_value[7]==PM_TSTAT7
		||product_register_value[7]==PM_TSTAT5i
		)
	{
		char cTemp1[8];
		memset(cTemp1,0,8);
		WideCharToMultiByte( CP_ACP, 0, strText.GetBuffer(), -1, cTemp1, 8, NULL, NULL );
		unsigned char UChar[8];
		for (int i=0;i<8;i++)
		{
			UChar[i]=cTemp1[i];
		}
		int ret = Write_Multi(g_tstat_id,UChar,m_tstat_output_data.at(lRow-1).OutputName.regAddress,8);
		if (ret>0)
		{
			m_tstat_output_data.at(lRow-1).OutputName.StrValue=strText;
		}
		FreshIOGridTable_Tstat6();
	}
	else
	{
		if(product_register_value[MODBUS_ADDRESS]>0)//6
    {

        try
        {
			CppSQLite3Table table;
			CppSQLite3Query q;

			CppSQLite3DB SqliteDBBuilding;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

            CString strSerial;
            strSerial.Format(_T("%d"),g_serialNum);

            CString StrSql;
            StrSql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
           q = SqliteDBBuilding.execQuery((UTF8MBSTR)StrSql);
            if(!q.eof())//update
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
                    SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
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

                  SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
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

            SqliteDBBuilding.closedb();



        }
        catch (...)
        {

        }
    }
	}

}
// void CT3000View::EnableToolTips(BOOL bEnable)
// {
//
// }
BOOL CT3000View::PreTranslateMessage(MSG* pMsg)
{

// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN  )
    {
        if(pMsg->wParam == VK_RETURN)
        {
            CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
            GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
            temp_focus->SetFocus();
            return TRUE;
        }
    }
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    if (pFrame->m_pDialogInfo != NULL && pFrame->m_pDialogInfo->IsWindowVisible())
    {

        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            //  pMain->m_pDialogInfo->ShowWindow(SW_HIDE);
            ::PostMessage(MainFram_hwd,WM_LBUTTONDOWN,0,0);
        }
    }





    return CFormView::PreTranslateMessage(pMsg);
}
//时间更新.....
void CT3000View::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if(g_bPauseMultiRead)
        return;


    // 更新时间。
    CTime time = CTime::GetCurrentTime();

    CString strtime = time.Format(_T("%I:%M:%S %p"));
    //CString strtime = time.Format(_T("%m/%d/%Y %H:%M:%S %a"));
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC_TIME);
    pEdit->SetWindowText(strtime);

    CString strDate = time.Format(_T("%A, %b %d, %Y"));
    pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC);
    pEdit->SetWindowText(strDate);
    int Increment=product_register_value[MODBUS_SETPOINT_INCREASE];

    if (nIDEvent == 10)
    {
        if (m_old_deadband == m_new_deadband)
        {
            KillTimer(10);
        }
        // ++coolvalue;
        if (m_new_deadband > m_old_deadband)
        {
            m_old_deadband+=Increment;
            if (m_slider_Type==1)
            {
                product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]+=Increment;
            }
            if (m_slider_Type==2)
            {
                product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]+=Increment;
            }
            if (m_slider_Type==3)
            {
                product_register_value[MODBUS_NIGHT_SETPOINT]+=Increment;
            }
            if (m_slider_Type==4)
            {
                product_register_value[MODBUS_DAY_HEATING_DEADBAND]+=Increment;
            }
            if (m_slider_Type==5)
            {
                product_register_value[MODBUS_DAY_COOLING_DEADBAND]+=Increment;
            }
            if (m_slider_Type==6)
            {
                product_register_value[MODBUS_DAY_SETPOINT]+=Increment;
            }
        }
        if (m_new_deadband < m_old_deadband)
        {
            m_old_deadband-=Increment;

            if (m_slider_Type==1)
            {
                product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]-=Increment;
            }
            if (m_slider_Type==2)
            {
                product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]-=Increment;
            }
            if (m_slider_Type==3)
            {
                product_register_value[MODBUS_NIGHT_SETPOINT]-=Increment;
            }
            if (m_slider_Type==4)
            {
                product_register_value[MODBUS_DAY_HEATING_DEADBAND]-=Increment;
            }
            if (m_slider_Type==5)
            {
                product_register_value[MODBUS_DAY_COOLING_DEADBAND]-=Increment;
            }
            if (m_slider_Type==6)
            {
                product_register_value[MODBUS_DAY_SETPOINT]-=Increment;
            }
        }
        FlexSPN =1;
        FlexSP=1;


        SendMessage(MESSAGE_SLIDER,0,0);
    }
    /*if (nIDEvent == 11)
    {
        SendMessage(MESSAGE_SLIDER,0,0);
        --coolvalue;
    }*/
    CFormView::OnTimer(nIDEvent);
}

void CT3000View::OnDestroy()
{
    CFormView::OnDestroy();
    if (m_pFreshBackground!=NULL)
    {
        if (WaitForSingleObject(m_pFreshBackground->m_hThread, 1000) != WAIT_OBJECT_0)

            //Sleep(500);//wait for the end of the thread.
            if (m_pFreshBackground)
            {
                if (WaitForSingleObject(m_pFreshBackground->m_hThread, 3000) == WAIT_OBJECT_0)
                {

                }
                else
                {
                    BOOL bRet = TerminateThread(m_pFreshBackground->m_hThread,0);

                    m_pFreshBackground=NULL;
                }

            }
    }
    KillTimer(1);
    // TODO: Add your message handler code here
}

void CT3000View::OnBnClickedTrendlogview()
{

    //((CMainFrame*)(theApp.m_pMainWnd))->SwitchToPruductType(3);
////	SwitchToPruductType(3);
//	if ((product_register_value[7]==PM_TSTAT5E||(product_register_value[7]==PM_TSTAT5G))||(product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
//	{
//		CDisplayConfig display_cfg;
//		display_cfg.DoModal();
//	}
//	else
//	{
//		AfxMessageBox(_T("This model of TStat don't support Display Config!"));
//		return;
//	}
//CDisplayLEDLCDConfig dlg;
//dlg.DoModal();
    // TODO: Add your control notification handler code here


}


void CT3000View::OnBnClickedOccupacheck()
{

    CString strTemp;
    //stat6



    CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);

    if(m_OcupiedBtn.GetCheck()==BST_UNCHECKED)
    {
        int ret=write_one(g_tstat_id,MODBUS_INFO_BYTE,0);	//184  109

        if (ret>0)
        {
            product_register_value[MODBUS_INFO_BYTE]=0;
        }

        pBtn->SetCheck(1);
        product_register_value[MODBUS_INFO_BYTE] = 0;//184
    }


    if(m_OcupiedBtn.GetCheck()==BST_CHECKED)
    {
        int ret=write_one(g_tstat_id,MODBUS_INFO_BYTE,1);	//184  109
        //Marked by Fance ,no need to judge
        //if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
        //	write_one(g_tstat_id,_G("MODBUS_CALIBRATION"),1);//109
        //else
        //	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE"),1);//184
        if (ret>0)
        {
            product_register_value[MODBUS_INFO_BYTE]=1;
        }
        product_register_value[MODBUS_INFO_BYTE] = 1;//184

        pBtn->SetCheck(0);
    }

    UpdateData(TRUE);
    //stat6

    if (product_register_value[MODBUS_INFO_BYTE]&1)
    {
        m_singlesliderday.SetThumbColor(RGB(96,192,0));
        m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
        m_singlesliderday.SetChannelColor(RGB(196, 196, 255));

    }
    else
    {
        m_singlesliderday.SetThumbColor(RGB(255,255,255));
        m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
        m_singlesliderday.SetChannelColor(RGB(196, 196, 255));
    }


    // Fresh();

    Fresh_T3000View();


}

void CT3000View::OnBnClickedUnoccupiedMark()
{
//stat6



    CButton* pBtn = (CButton*)GetDlgItem(IDC_UNOCCUPIED_MARK);

    if(pBtn->GetCheck()==BST_UNCHECKED)
    {

        int ret=write_one(g_tstat_id,MODBUS_INFO_BYTE,1);	//184  109
        //Marked by Fance ,no need to judge
        //if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
        //	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE",3),1);	//109
        //else
        //	write_one(g_tstat_id,_G("MODBUS_INFO_BYTE"),1);	//184
        if (ret>0)
        {
            product_register_value[MODBUS_CALIBRATION]=1;
        }
        product_register_value[MODBUS_INFO_BYTE] = 1;//184
        m_OcupiedBtn.SetCheck(1);
    }

    if(pBtn->GetCheck()==BST_CHECKED)
    {
        int ret=write_one(g_tstat_id,MODBUS_INFO_BYTE,0);	//184  109
        if (ret>0)
        {
            product_register_value[MODBUS_CALIBRATION]=0;
        }
        product_register_value[MODBUS_INFO_BYTE] = 0;//184
        //if ((m_strModelName.CompareNoCase(_T("Tstat6")) == 0)||(m_strModelName.CompareNoCase(_T("Tstat7")) == 0))
        //	write_one(g_tstat_id,109,0);
        //else
        //	write_one(g_tstat_id,184,0);

        //	product_register_value[184]  = 0;
        m_OcupiedBtn.SetCheck(0);
    }

    UpdateData(TRUE);

    if (product_register_value[MODBUS_INFO_BYTE]&1)
    {
//     m_singlesliderday.SetThumbColor(RGB(96,192,0));
//     m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
//     m_singlesliderday.SetChannelColor(RGB(196, 196, 255));







    }
    else
    {
//     m_singlesliderday.SetThumbColor(RGB(255,255,255));
//     m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
//     m_singlesliderday.SetChannelColor(RGB(196, 196, 255));







    }
    int  Fresh_Min=(short)product_register_value[MODBUS_MIN_SETPOINT];
    int Fresh_Max=(short)product_register_value[MODBUS_MAX_SETPOINT];
    if (Fresh_Min>Fresh_Max)
    {
        Fresh_Min=10;
        Fresh_Max=50;
    }
    int currenttemp=Fresh_Min+Fresh_Max -(int)product_register_value[MODBUS_TEMPRATURE_CHIP]/10;
//m_singlesliderday.SetPos(currenttemp);

// Fresh();
//stat6
    Fresh_T3000View();

}
int CT3000View::get_real_fan_select()
{

    int fan_i=m_FanComBox.GetCurSel();
	
    if(product_register_value[MODBUS_AUTO_ONLY]==BST_CHECKED)
    {
        if(fan_i==0)
            fan_i=0;
        else if(fan_i==1)
            fan_i=4;
        else if(fan_i==2)
            fan_i=5;
        else if(fan_i==3)
            fan_i=6;
    }
    else
    {
        int fan_mode_i=product_register_value[MODBUS_FAN_MODE]-1;
        switch(fan_mode_i)
        {
        case 0:
            if(fan_i==0)fan_i= 0;
            if(fan_i==1)fan_i= 1;
            if(fan_i==2)fan_i= 4;
            if(fan_i==3)fan_i= 5;
            if(fan_i==4)fan_i= 6;
            break;
        case 1:
            if(fan_i==0)fan_i= 0;
            if(fan_i==1)fan_i= 1;
            if(fan_i==2)fan_i= 3;
            if(fan_i==3)fan_i= 4;
            if(fan_i==4)fan_i= 5;
            if(fan_i==5)fan_i= 6;
            break;
        case 2:
            if(fan_i==0)fan_i= 0;
            if(fan_i==1)fan_i= 1;
            if(fan_i==2)fan_i= 2;
            if(fan_i==3)fan_i= 3;
            if(fan_i==4)fan_i= 4;
            if(fan_i==5)fan_i= 5;
            if(fan_i==6)fan_i= 6;
            break;
        default:
            if(fan_i==0)fan_i= 0;
            if(fan_i==1)fan_i= 1;
            if(fan_i==2)fan_i= 2;
            if(fan_i==3)fan_i= 3;
            if(fan_i==4)fan_i= 4;
            if(fan_i==5)fan_i= 5;
            if(fan_i==6)fan_i= 6;
            break;
        }
    }

	 
		if (product_register_value[7] == PM_TSTAT8)
		{


			int fan_mode_i = product_register_value[MODBUS_FAN_MODE];
			switch (fan_mode_i)
			{
			case 0:
				if (fan_i == 0)fan_i = 0;
				if (fan_i == 1)fan_i = 4;
				break;
			case 1:
				if (fan_i == 0)fan_i = 0;
				if (fan_i == 1)fan_i = 1;
				if (fan_i == 2)fan_i = 4;

				break;
			case 2:
				if (fan_i == 0)fan_i = 0;
				if (fan_i == 1)fan_i = 1;
				if (fan_i == 2)fan_i = 2;
				if (fan_i == 3)fan_i = 4;
				break;
			case 3:
				if (fan_i == 0)fan_i = 0;
				if (fan_i == 1)fan_i = 1;
				if (fan_i == 2)fan_i = 2;
				if (fan_i == 3)fan_i = 3;
				if (fan_i == 4)fan_i = 4;
				break;
			case 4:
				if (fan_i == 0)fan_i = 0;
				if (fan_i == 1)fan_i = 1;
				if (fan_i == 2)fan_i = 2;
				if (fan_i == 3)fan_i = 3;
				if (fan_i == 4)fan_i = 4;
				break;
			 
			default:
				if (fan_i == 0)fan_i = 0;
				if (fan_i == 1)fan_i = 1;
				if (fan_i == 2)fan_i = 2;
				if (fan_i == 3)fan_i = 3;
				if (fan_i == 4)fan_i = 4;
				if (fan_i == 5)fan_i = 5;
				if (fan_i == 6)fan_i = 6;
				break;
			}

		}

	 

    return fan_i;
}
int CT3000View::set_real_fan_select()
{
    int fan_i=0;
    if(product_register_value[MODBUS_AUTO_ONLY]==1)
    {
        if(g_ifanStatus == 4)
        {
            fan_i =1;
        }
        else if (g_ifanStatus == 5)
        {
            fan_i =2;
        }
        else if (g_ifanStatus == 6)
        {
            fan_i =3;
        }
        else
        {
            fan_i =0;
        }
    }
    else
    {
        int fan_mode_i = product_register_value[MODBUS_FAN_MODE]-1;
        switch(fan_mode_i)
        {
        case 0:
            if(g_ifanStatus==0)fan_i= 0;
            if(g_ifanStatus==1)fan_i= 1;
            if(g_ifanStatus==4)fan_i= 2;
            if (g_ifanStatus == 5)fan_i= 3;
            if (g_ifanStatus == 6)fan_i= 4;
            break;
        case 1:
            if(g_ifanStatus==0)fan_i= 0;
            if(g_ifanStatus==1)fan_i= 1;
            if(g_ifanStatus==3)fan_i= 2;
            if(g_ifanStatus==4)fan_i= 3;
            if (g_ifanStatus == 5)fan_i= 4;
            if (g_ifanStatus == 6)fan_i= 5;
            break;
        case 2:
            if(g_ifanStatus==0)fan_i= 0;
            if(g_ifanStatus==1)fan_i= 1;
            if(g_ifanStatus==2)fan_i= 2;
            if(g_ifanStatus==3)fan_i= 3;
            if(g_ifanStatus==4)fan_i= 4;
            if (g_ifanStatus == 5)fan_i= 5;
            if (g_ifanStatus == 6)fan_i= 6;
            break;
        default:
            if(g_ifanStatus==0)fan_i= 0;
            if(g_ifanStatus==1)fan_i= 1;
            if(g_ifanStatus==2)fan_i= 2;
            if(g_ifanStatus==3)fan_i= 3;
            if(g_ifanStatus==4)fan_i= 4;
            if (g_ifanStatus == 5)fan_i= 5;
            if (g_ifanStatus == 6)fan_i= 6;
            break;
        }
    }

    return fan_i;
}
void CT3000View::OnCbnSelchangeFanspeedcombo()
{
    CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

    //恢复T3000主线程
    pMain->m_pFreshMultiRegisters->SuspendThread();
    pMain->m_pRefreshThread->SuspendThread();

//
//int ret=0;
//	ret = write_one(g_tstat_id,MODBUS_FAN_SPEED,m_FanComBox.GetCurSel()); //t5=137  t6=273
// 	if (product_register_value[MODBUS_AUTO_ONLY]==1)
// 	{
// 		int sel=m_FanComBox.GetCurSel();
// 		if (sel==0)//OFF
// 		{
    int sel = get_real_fan_select();
    int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,sel);
    if (ret>0)
    {

        product_register_value[MODBUS_FAN_SPEED]=sel;
    }
    /*m_FanComBox.SetCurSel(0);*/
// 		}
// 		else//Auto
// 		{
// 			int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,4);
// 			if (ret>0)
// 			{
// 				product_register_value[MODBUS_FAN_SPEED]=4;
// 			}
// 			m_FanComBox.SetCurSel(1);
// 		}
// 	}
// 	else
// 	{
// 		int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,m_FanComBox.GetCurSel());
// 		if (ret>0)
// 		{
//
// 			product_register_value[MODBUS_FAN_SPEED]=m_FanComBox.GetCurSel();
// 		    m_FanComBox.SetCurSel(product_register_value[MODBUS_FAN_SPEED]);
// 		}
// 	}

    //if (!(ret>0))
    //{
    //	CString str;
    //	str.Format(_T("setting invalid,error:%d"),ret);
    //	AfxMessageBox(_T("setting invalid!"));
    //}

    //恢复T3000主线程


	if (product_register_value[8] = PM_TSTAT8)
	{
		int sel = m_FanComBox.GetCurSel();

	}
    pMain->m_pFreshMultiRegisters->ResumeThread();
    pMain->m_pRefreshThread->ResumeThread();//
}


LRESULT CT3000View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if(MsgT3000ViewFresh==message)
    {
        if((product_register_value[7] == PM_TSTAT6) || (product_register_value[7] == PM_TSTAT7) || (product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8))
        {
            product_type =T3000_6_ADDRESS;
        }
        else if((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E) ||(product_register_value[7]==PM_TSTATRUNAR)|| (product_register_value[7] == PM_TSTAT5H))
        {
            product_type = T3000_5EH_LCD_ADDRESS;
        }
        else if((product_register_value[7] == PM_TSTAT5A) ||(product_register_value[7] == PM_TSTAT5B) ||(product_register_value[7] == PM_TSTAT5B2) ||
                (product_register_value[7] ==PM_TSTAT5C ) || (product_register_value[7] == PM_TSTAT5D) || (product_register_value[7] == PM_TSTAT5F) ||
                (product_register_value[7] == PM_TSTAT5G))
        {
            product_type =T3000_5ABCDFG_LED_ADDRESS;
        }

#if 1
        if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i))
            //if (product_register_value[7] == 6)
        {
            if ((product_register_value[7] == PM_TSTAT6&&m_fFirmwareVersion>35.5)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT5i))
            {

                //product_register_value[]列表交换。
                //读取TXT
                //	memset(newtstat6,0,sizeof(newtstat6));
                memcpy(newtstat6,product_register_value,sizeof(newtstat6));

                memset(tempchange,0,sizeof(tempchange));
                int index = 0;

                for (int i = 0; i<512; i++)
                {
                    index = reg_tstat6[i];
                    tempchange[index] = product_register_value[i];
                }

                //Fance	memcpy(product_register_value,tempchange,sizeof(product_register_value));
            }

        }//else if(m_strModelName.CompareNoCase(_T("Tstat6"))!=0)

#endif
//tstat6

        if (m_active_key_mouse)
        {
            return 0;
        }

        Fresh_T3000View();
        //Fresh();
    }
    if (MESSAGE_SLIDER == message)
    {
// InitFlexSliderBars_tstat6();
        SetFlexSliderBars_tstat6();
//    m_pSlider_Test_TwoSP->SetPos(8, coolvalue, coolvalue+1);

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
    Fresh_T3000View();
    return 0;
}

//comments by Fan
//synchronization the time ,it's not use,this button is not visible.
void CT3000View::OnBnClickedBtnSynctime()
{

#if 1
    CTime time = CTime::GetCurrentTime();
    BYTE szTime[8] = {0};

    BeginWaitCursor();
    GetDlgItem(IDC_BTN_SYNCTIME)->EnableWindow(FALSE);

    szTime[0] =(BYTE)(time.GetYear()%100);
    int nRet = write_one(g_tstat_id,MODBUS_YEAR, szTime[0]);
	if (product_register_value[7] == PM_TSTAT8) 
	{
		 
		write_one(g_tstat_id, MODBUS_YEAR, time.GetYear());
	}
    Sleep(1000);
    szTime[1] = (BYTE)(time.GetMonth());
    nRet = write_one(g_tstat_id, MODBUS_MONTH, szTime[1]);
    Sleep(1000);
    szTime[2] = (BYTE)(time.GetDayOfWeek()-1);
    nRet = write_one(g_tstat_id, MODBUS_WEEK, szTime[2]);
    Sleep(1000);
    szTime[3] = (BYTE)(time.GetDay());
    nRet = write_one(g_tstat_id, MODBUS_DAY, szTime[3]);
    Sleep(1000);
    szTime[4] = (BYTE)(time.GetHour());
    nRet = write_one(g_tstat_id, MODBUS_HOUR, szTime[4]);
    Sleep(1000);
    szTime[5] = (BYTE)(time.GetMinute());
    nRet = write_one(g_tstat_id, MODBUS_MINUTE, szTime[5]);
    Sleep(1000);
    szTime[6] = (BYTE)(time.GetSecond());
    nRet = write_one(g_tstat_id, MODBUS_SECOND, szTime[6]);

    GetDlgItem(IDC_BTN_SYNCTIME)->EnableWindow(TRUE);

    EndWaitCursor();
    AfxMessageBox(_T("Time synchronization is complete!"));
#endif

    //int nRet = Write_Multi(g_tstat_id, szTime, 450, 8, 3);  // 这个写入有问题，所以变成单个字节写多次
}

void CT3000View::OnBnClickedButtonSchedule()
{
// 	if((product_register_value[7] != PM_TSTAT6)&&(product_register_value[7] != PM_TSTAT5i))
// 	{
// 		AfxMessageBox(_T("This model of TStat don't support schedule!"));
// 		return;
// 	}
    g_bPauseMultiRead = TRUE;
    CTStatScheduleDlg dlg;
    dlg.DoModal();
    g_bPauseMultiRead = FALSE;

}




// 这个函数是给新的Model，TStat6，TStat7使用
// 使用了新的寄存器和新的寄存器逻辑
void CT3000View::InitFlexSliderBars()
{


//以下是E:\Tstat67\T3000\T3000
#if 1
    // init 3 thumb slider
    //CStatic *s = (CStatic*)GetDlgItem(IDC_FSB_OFFICE);

    // settings

    // set range of temperature
    //m_daySlider.ShowWindow(SW_HIDE);

    CString strTemp;
    int nRangeMin	=(short)product_register_value[MODBUS_MIN_SETPOINT];//132
    int nRangeMax	=(short)product_register_value[MODBUS_MAX_SETPOINT];//131
    //m_TemperaureSlider.SetPos((int)(nRangeMax-m_fTemperature+nRangeMin));//2.5.0.95
    //m_TemperaureSlider.SetRange(nRangeMin,nRangeMax);
    //m_TemperaureSlider.SetPos(nRangeMax-m_fTemperature+nRangeMin);

    if (g_unint)
        strTemp.Format(_T("%.1f C"),m_fTemperature);
    else
        strTemp.Format(_T("%.1f F"),m_fTemperature);
//	m_TempInfoEdit.FieldText(strTemp);	//2.5.0.95

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
        if((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)||(product_register_value[7]==PM_TSTAT5G)) && m_fFirmwareVersion >= 35.4)
        {
            nOfficeSelReg = 423;
        }
        nOfficeOrHotel = product_register_value[nOfficeSelReg];
        if (nOfficeOrHotel == 0) // office
        {
            nSP=product_register_value[MODBUS_COOLING_SETPOINT];//135
            nCoolSP = nSP + product_register_value[MODBUS_COOLING_DEADBAND]/10;//119
            nHeatSP = nSP - product_register_value[MODBUS_HEATING_DEADBAND]/10;//120
            //
            if (nCoolSP == nHeatSP && nHeatSP == 0 )
            {
                nCoolSP = nHeatSP + 1;
            }

            strTemp.Format(_T("%d"), nSP);
            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);

            //CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
            //pSPWnd->SetWindowText(strTemp);

            m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
            m_DayHeatStatic.SetWindowText(_T("Heating SP"));

            if (g_unint)
            {
                strTemp.Format(_T("%d C"), nCoolSP);
            }
            else
            {
                strTemp.Format(_T("% F"), nCoolSP);
            }
            m_DayCoolEdit.FieldText(strTemp);
            //m_DayCoolEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%d C"), nHeatSP);
            }
            else
            {
                strTemp.Format(_T("%d F"), nHeatSP);
            }

            m_DayHeatEdit.FieldText(strTemp);
            //	m_DayHeatEdit.SetNumberDefaultTex();

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
            strTemp.Format(_T("0.0"));
            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            strTemp.Format(_T("0.0"));
            m_nightHeatInfoEdit.FieldText(strTemp);
            //m_nightHeatInfoEdit.SetNumberDefaultTex();
            strTemp.Format(_T("%d"),0);
            CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
            pSPWnd->SetWindowText(strTemp);



        }
        else  // hotel
        {



            int nCoolingDBRegister = 119;
            int nHeatingDBRegister = MODBUS_HEATING_DEADBAND;
            //int nSPRegister = 380;
            int nCoolDeadband = product_register_value[MODBUS_COOLING_DEADBAND];//119
            nSP = product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10;	//380
            nCoolSP = product_register_value[MODBUS_COOLING_DEADBAND] + nSP;
            if((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)||(product_register_value[7]==PM_TSTAT5G)) && m_fFirmwareVersion >= 35.4)
            {
                nCoolingDBRegister = 422;
                nHeatingDBRegister = MODBUS_HEATING_SETPOINT;//136
                //nSPRegister = 135;
                nSP = product_register_value[MODBUS_COOLING_SETPOINT];//135
                nCoolDeadband = product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10;//422
                nCoolSP = nCoolDeadband + nSP;
            }


            nHeatSP = nSP - product_register_value[nHeatingDBRegister];
            if (nCoolSP == nHeatSP && nHeatSP == 0)
            {
                nSP = nHeatSP + 1;
                nCoolSP = nSP+1;
            }

            strTemp.Format(_T("%d"), nSP);
            //CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
            //pSPWnd->SetWindowText(strTemp);
            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);

            strTemp.Format(_T("%d"), nCoolDeadband);
            m_DayCoolEdit.FieldText(strTemp);
            //m_DayCoolEdit.SetNumberDefaultTex();
            strTemp.Format(_T("%0.1f"), (float)product_register_value[nHeatingDBRegister]);
            m_DayHeatEdit.FieldText(strTemp);
            //m_DayHeatEdit.SetNumberDefaultTex();
            //0903
// 			m_DayCoolStatic.SetWindowText(_T("Cooling DB"));
// 			m_DayHeatStatic.SetWindowText(_T("Heating DB"));



            //////////////////////////////////////////////////////////////////////////
            nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
            nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
            m_pDaySingleSP->SetRange(nRangeMax, nRangeMin);

            BOOL bRetSP = m_pDaySingleSP->SetPos(nHeatSP, nSP, nCoolSP);

            //	BOOL bRetSP = m_pDayTwoSP->SetPos(nHeatSP, nCoolSP, nCoolSP+1); // 因为只有2个thumb，所以sp不要了，设为两端的值
            HandleSliderSetPos(bRetSP);//tstat6

            m_pDaySingleSP->ShowWindow(SW_NORMAL);
            m_pDayTwoSP->ShowWindow(SW_HIDE);

            //stat6
            m_pNightTwoSP->ShowWindow(SW_HIDE);
            m_pNightSingleSP->ShowWindow(SW_HIDE);
            strTemp.Format(_T("0.0"));
            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            strTemp.Format(_T("0.0"));
            m_nightHeatInfoEdit.FieldText(strTemp);
            //m_nightHeatInfoEdit.SetNumberDefaultTex();
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

            strTemp.Format(_T("%0.1f"),(float)((nCoolSP + nHeatSP))/2);
            CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
            pSPWnd->SetWindowText(strTemp);

            strTemp.Format(_T("%d"), nCoolSP);
            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            strTemp.Format(_T("%0.1f"), (float)nHeatSP);
            m_nightHeatInfoEdit.FieldText(strTemp);



            nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
            nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
            m_pNightTwoSP->SetRange(nRangeMax, nRangeMin);
            BOOL bRetSP = m_pNightTwoSP->SetPos( nHeatSP, nCoolSP,nCoolSP+1);
            HandleSliderSetPos(bRetSP);//tstat6
            m_pNightTwoSP->ShowWindow(SW_NORMAL);
            m_pNightSingleSP->ShowWindow(SW_HIDE);



            //stat6

            strTemp.Format(_T("%d"), 0);
            m_DayCoolEdit.FieldText(strTemp);
            //	m_DayCoolEdit.SetNumberDefaultTex();
            strTemp.Format(_T("0.0"));
            m_DayHeatEdit.FieldText(strTemp);
            //m_DayHeatEdit.SetNumberDefaultTex();
            m_pDaySingleSP->ShowWindow(SW_HIDE);
            m_pDayTwoSP->ShowWindow(SW_HIDE);

            strTemp.Format(_T("%d"), 0);
            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);
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
            nSP = product_register_value[nSPRegister]/10;

            if((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)||(product_register_value[7]==PM_TSTAT5G)) && m_fFirmwareVersion >= 34.09)
            {
                nSPRegister = MODBUS_COOLING_SETPOINT;//135
                nSP = product_register_value[nSPRegister];
            }

            nCoolSP = product_register_value[nCoolingRegister] + nSP;
            nHeatSP = nSP - product_register_value[nHeatingRegister];



            if (nCoolSP == nHeatSP && nHeatSP == 0)
            {
                nSP = nHeatSP + 1;
                nCoolSP = nSP+1;
            }

            strTemp.Format(_T("%d"), nSP);
            CWnd *pSPWnd = GetDlgItem(IDC_EDIT_CUR_SP);
            pSPWnd->SetWindowText(strTemp);

            strTemp.Format(_T("%0.1f"),(float) product_register_value[nCoolingRegister]);
            m_nightInfoEdit.FieldText(strTemp);
            //	m_nightInfoEdit.SetNumberDefaultTex();
            strTemp.Format(_T("%0.1f"),(float) product_register_value[nHeatingRegister]);
            m_nightHeatInfoEdit.FieldText(strTemp);
//0903
// 			m_NightCoolStatic.SetWindowText(_T("Cooling DB"));
// 			m_NightHeatStatic.SetWindowText(_T("Heating DB"));



            nRangeMax = nCoolSP > nRangeMax ? nCoolSP : nRangeMax;
            nRangeMin = nHeatSP < nRangeMin ? nHeatSP : nRangeMin;
            m_pNightSingleSP->SetRange(nRangeMax, nRangeMin);
            BOOL bRetSP =  m_pNightSingleSP->SetPos( nHeatSP, nSP, nCoolSP);
            HandleSliderSetPos(bRetSP);//tstat6


            m_pNightSingleSP->ShowWindow(SW_NORMAL);
            m_pNightTwoSP->ShowWindow(SW_HIDE);


            //stat6

            strTemp.Format(_T("%d"), 0);
            m_DayCoolEdit.FieldText(strTemp);
            //	m_DayCoolEdit.SetNumberDefaultTex();
            strTemp.Format(_T("0.0"));
            m_DayHeatEdit.FieldText(strTemp);
            m_pDaySingleSP->ShowWindow(SW_HIDE);
            m_pDayTwoSP->ShowWindow(SW_HIDE);
            strTemp.Format(_T("%d"), 0);
            m_dayInfoEdit.FieldText(RGB(0,0,100),strTemp);
        }
    }

#endif
}
int CT3000View::Round_SetPoint_Max(int spvalue)
{
    int ret=spvalue;
    int Increment=product_register_value[MODBUS_SETPOINT_INCREASE];
    if (Increment!=0)
    {
        while((ret%Increment)!=0)
        {
            ++ret;
        }
    }
    return ret;
}

int CT3000View::Round_SetPoint_Min(int spvalue)
{
    int ret=spvalue;
    int Increment=product_register_value[MODBUS_SETPOINT_INCREASE];
    if (Increment!=0)
    {
        while((ret%Increment)!=0)
        {
            --ret;
        }
    }
    return ret;
}
LRESULT CT3000View::OnFlexSlideCallBack(WPARAM wParam, LPARAM lParam)
{

    //HWND  Hret =m_pDayTwoSP->m_hWnd;//GetSafeHwnd( );
    //int rett = m_pNightTwoSP->Getflag();
    int nMin, nMax;
    int nCoolSP = 0;
    int nHeatSP = 0;
    int nSP =0;
    m_active_key_mouse=TRUE;
    int nHDB=0;
    int nCDB=0;

    if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT5i))
    {
        int Increment=product_register_value[MODBUS_SETPOINT_INCREASE];
        switch(wParam)
        {
        
		case 1://右侧 晚上 2SP
        {
            //350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
            //352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
            //353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
            vector<int>  szPos(2);
            m_pNightTwoSP->GetRange(nMin, nMax);
            int nNum = m_pNightTwoSP->GetPos(szPos);

			int nHeatSP = szPos[0] + nMin;
			int nCoolSP = szPos[1] + nMin;
			nSP = (nHeatSP + nCoolSP) / 2; //自动调节sp/*product_register_value[MODBUS_NIGHT_SETPOINT]*/
			nHeatSP = GetRoundNumber(nHeatSP);
			nCoolSP = GetRoundNumber(nCoolSP);
			    nSP = GetRoundNumber(nSP);
            nHDB=nSP-nHeatSP;
            nCDB=nCoolSP-nSP;
			int	Tstat_nSP = (short)product_register_value[MODBUS_NIGHT_SETPOINT];
			int	Tstat_nCoolSP = (short)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
			int	Tstat_nHeatSP = (short)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];

			if (nHeatSP == nCoolSP)
			{
				if (Tstat_nCoolSP != nCoolSP)
				{
					nCDB = Increment;
					nHDB = 0;
				}
				if (Tstat_nHeatSP != nHeatSP)
				{
					nHDB = Increment;
					nCDB = 0;
				}
			}
			nHeatSP = nSP - nHDB;
			nCoolSP = nSP + nCDB;


            if ((nHDB<0&&nHDB>254)&&(nCDB<0&&nCDB>254))
            {

                InitFlexSliderBars_tstat6();
                m_active_key_mouse=FALSE;
                return 0;
            }

          

            int ret=0;


            BeginWaitCursor();
          
               


                
                
				ret =	write_one(g_tstat_id, MODBUS_NIGHT_SETPOINT, nSP, 5);//t6 350
                ret =	write_one(g_tstat_id, MODBUS_NIGHT_HEATING_DEADBAND,nHDB,5);//t6 354
				ret =	write_one(g_tstat_id, MODBUS_NIGHT_COOLING_DEADBAND, nCDB, 5);//t6 355
                if (ret<0)
                {
                    /*AfxMessageBox(_T("Write Fail!"));*/
                    m_active_key_mouse=FALSE;
                    InitFlexSliderBars_tstat6();
                    return 0;
                }
                product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]=nHDB;
				product_register_value[MODBUS_NIGHT_SETPOINT] = nSP;
				product_register_value[MODBUS_NIGHT_COOLING_DEADBAND] = nCDB;



				if (nCoolSP >= DayMax*10 || nHeatSP <= DayMin*10)
				{
					InitFlexSliderBars_tstat6();
				}
				else
				{


					m_pNightTwoSP->SetPos_tstat6_2pos(nHeatSP, nSP, nCoolSP);

					CString strTemp;

					if (g_unint)
					{
						strTemp.Format(_T("%0.1f C"), ((float)nCoolSP) / 10.0);
					}
					else
					{
						strTemp.Format(_T("%0.1f F"), ((float)nCoolSP) / 10.0);
					}

					m_nightInfoEdit.FieldText(strTemp);
					//m_nightInfoEdit.SetNumberDefaultTex();
					if (g_unint)
					{
						strTemp.Format(_T("%0.1f C"), ((float)nHeatSP) / 10.0);
					}
					else
					{
						strTemp.Format(_T("%0.1f F"), ((float)nHeatSP) / 10.0);
					}

					m_nightHeatInfoEdit.FieldText(strTemp);
				}

            EndWaitCursor();




           

        }
         break;

		case 2://右侧 晚上 1SP
        {
            //350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
            //354	2	Full	W/R	(Night)Unoccupied heating setpoint
            //355	2	Full	W/R	(Night)Unoccupied cooling setpoint
            int nRangeMin	=(short)product_register_value[MODBUS_MIN_SETPOINT];
            int nRangeMax	=(short)product_register_value[MODBUS_MAX_SETPOINT];


            vector<int>  szPos(3);
            m_pNightSingleSP->GetRange(nMin, nMax);
            int nNum = m_pNightSingleSP->GetPos(szPos);

            int nHeatSP = szPos[0] + nMin;
            int nSP = szPos[1] + nMin;
            int nCoolSP= szPos[2] + nMin;

			nHeatSP = GetRoundNumber(nHeatSP);
			nCoolSP = GetRoundNumber(nCoolSP);
			nSP = GetRoundNumber(nSP);

            int	Tstat_nSP= (short)product_register_value[MODBUS_NIGHT_SETPOINT];
            int	Tstat_nCoolSP = (short)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
            int	Tstat_nHeatSP = (short)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
            int NightSP = Tstat_nSP;
            if (Tstat_nSP!=nSP)
            {

                
                if (NightSP<nRangeMin*10)  //  ||
                {
                    NightSP = nRangeMin*10;
                }
                if (nSP>nRangeMax*10)
                {
                    NightSP = nRangeMax*10;
                }

                int ret=write_one(g_tstat_id, MODBUS_NIGHT_SETPOINT, NightSP,5);//t6 350
                if (ret<0)
                {
                    /*AfxMessageBox(_T("Write Fail!"));*/
                    m_active_key_mouse=FALSE;
                    InitFlexSliderBars_tstat6();
                    return 0;
                }
                product_register_value[MODBUS_NIGHT_SETPOINT]=NightSP;
				product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]=nCoolSP;
				product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=nHeatSP;

				CString strTemp;
				BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(nHeatSP, nSP, nCoolSP);
				HandleSliderSetPos(bRetSP);//tstat6

				if (g_unint)
				{
					strTemp.Format(_T("%0.1f C"), (float)nSP/10.0);
				}
				else
				{
					strTemp.Format(_T("%0.1f F"), (float)nSP/10.0);
				}

				m_nightpotEdit.FieldText(RGB(0, 0, 0), strTemp);

				if (g_unint)
				{
					strTemp.Format(_T("%0.1f C"), ((float)nCoolSP) / 10.0);
				}
				else
				{
					strTemp.Format(_T("%0.1f F"), ((float)nCoolSP) / 10.0);
				}

				m_nightInfoEdit.FieldText(strTemp);
				//m_nightInfoEdit.SetNumberDefaultTex();
				if (g_unint)
				{
					strTemp.Format(_T("%0.1f C"), ((float)nHeatSP) / 10.0);
				}
				else
				{
					strTemp.Format(_T("%0.1f F"), ((float)nHeatSP) / 10.0);
				}

				m_nightHeatInfoEdit.FieldText(strTemp);



            }




        }
        break;
        
		case 3:
            //左侧 白天 2SP
        {
            // 		345	1	Low byte	W/R	(Day)Occupied   setpoint
            // 		346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
            // 		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
            vector<int>  szPos(2);
            m_pDayTwoSP->GetRange(nMin, nMax);
            int nNum = m_pDayTwoSP->GetPos(szPos);
            int nHeatSP = szPos[0] + nMin;
            int nCoolSP = szPos[1] + nMin;
            nSP= (nHeatSP+nCoolSP)/2;
			nHeatSP = GetRoundNumber(nHeatSP);
			nCoolSP = GetRoundNumber(nCoolSP);
			nSP = GetRoundNumber(nSP);
            nHDB=nSP-nHeatSP;
            nCDB=nCoolSP-nSP;
			int	Tstat_nSP = (short)product_register_value[MODBUS_DAY_SETPOINT];
			int	Tstat_nCoolSP = (short)product_register_value[MODBUS_DAY_COOLING_SETPOINT];
			int	Tstat_nHeatSP = (short)product_register_value[MODBUS_DAY_HEATING_SETPOINT];

			if (nHeatSP==nCoolSP)
			{
				if (Tstat_nCoolSP != nCoolSP)
				{
					nCDB = Increment;
					nHDB = 0;
				}
				if (Tstat_nHeatSP != nHeatSP)
				{
					nHDB = Increment;
					nCDB = 0;
				}
			}
			
			nHeatSP = nSP - nHDB;
			nCoolSP = nSP + nCDB;
            if ((nHDB<0&&nHDB>254)&&(nCDB<0&&nCDB>254))
            {
                /*AfxMessageBox(_T("Deadband>254 or Deadband <0"));*/
                InitFlexSliderBars_tstat6();
                return 0;
            }
          
            BeginWaitCursor();
 
                
			   int ret = write_one(g_tstat_id, MODBUS_DAY_SETPOINT, nSP, 5);//345 T6
                   ret=  write_one(g_tstat_id, MODBUS_DAY_HEATING_DEADBAND,nHDB,5);//t6 349;
				   ret = write_one(g_tstat_id, MODBUS_DAY_COOLING_DEADBAND, nCDB, 5);//t6 348;
                if (ret<0)
                {
                    InitFlexSliderBars_tstat6();
                    m_active_key_mouse=FALSE;
                    return 0;
                }
                product_register_value[MODBUS_DAY_HEATING_DEADBAND]=nHDB;
				product_register_value[MODBUS_DAY_SETPOINT] = nSP;
				product_register_value[MODBUS_DAY_COOLING_DEADBAND] = nCDB;

        

          if (nCoolSP >= DayMax*10||nHeatSP<=DayMin*10)
          {
			  InitFlexSliderBars_tstat6();
          }
		  else
		  {
			  CString strTemp;
			  if (g_unint)
			  {
				  strTemp.Format(_T("%0.1f C"), ((float)nCoolSP / 10.0));
			  }
			  else
			  {
				  strTemp.Format(_T("%0.1f F"), ((float)nCoolSP / 10.0));
			  }

			  m_DayCoolEdit.FieldText(strTemp);
			  //m_DayCoolEdit.SetNumberDefaultTex();
			  if (g_unint)
			  {
				  strTemp.Format(_T("%0.1f C"), ((float)nHeatSP / 10.0));
			  }
			  else
			  {
				  strTemp.Format(_T("%0.1f F"), ((float)nHeatSP / 10.0));
			  }


			  m_DayHeatEdit.FieldText(strTemp);

			  m_pDayTwoSP->SetPos_tstat6_2pos(nHeatSP, nSP, nCoolSP);
		  }

		
            EndWaitCursor();

        }
        break;
        
		case 4://左侧 白天 1SP
        {

            //348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
            //349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)
            int nRangeMin	=(short)product_register_value[MODBUS_MIN_SETPOINT];
            int nRangeMax	=(short)product_register_value[MODBUS_MAX_SETPOINT];
            vector<int>  szPos(3);
            m_pDaySingleSP->GetRange(nMin, nMax);
            int nNum = m_pDaySingleSP->GetPos(szPos);

            int nHeatSP = szPos[0] + nMin;//最小
            int nSP = szPos[1] + nMin;
            int  nCoolSP= szPos[2] + nMin;//最大
			nHeatSP = GetRoundNumber(nHeatSP);
			nCoolSP = GetRoundNumber(nCoolSP);
			nSP = GetRoundNumber(nSP);

            int	Tstat_nSP= (short)product_register_value[MODBUS_DAY_SETPOINT];
            int	Tstat_nCoolSP = (short)product_register_value[MODBUS_DAY_COOLING_SETPOINT];
            int	Tstat_nHeatSP = (short)product_register_value[MODBUS_DAY_HEATING_SETPOINT];
            int DaySP = Tstat_nSP;
            if (Tstat_nSP!=nSP)
            {

                
                if (DaySP<nRangeMin*10) // ||
                {
                    DaySP = nRangeMin*10;
                }
                if (DaySP>nRangeMax*10)
                {
                    DaySP = nRangeMax*10;
                }
                //nSP=Round_SetPoint_Max(nSP);
                int ret= write_one(g_tstat_id, MODBUS_DAY_SETPOINT,DaySP ,5);//345 T6
                if (ret<0)
                {
                    /*AfxMessageBox(_T("Write Fail!"));*/
                    m_active_key_mouse=FALSE;
                    InitFlexSliderBars_tstat6();
                    return 0;
                }
                product_register_value[MODBUS_DAY_SETPOINT]=DaySP;

				product_register_value[MODBUS_DAY_COOLING_SETPOINT]=nCoolSP;
				product_register_value[MODBUS_DAY_HEATING_SETPOINT]=nHeatSP;

				BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(nHeatSP, nSP, nCoolSP);//tstat6

				HandleSliderSetPos(bRetSP);//tstat6
				CString strTemp;
				if (g_unint)
				{
					//strTemp.Format(_T("%d C"), nHeatSP);
					strTemp.Format(_T("%0.1f C"), (float)nSP/10.0);
				}
				else
				{

					strTemp.Format(_T("%0.1f F"), (float)nSP/10.0);
				}
				m_dayInfoEdit.FieldText(RGB(0, 0, 0), strTemp);


				if (g_unint)
				{
					strTemp.Format(_T("%0.1f C"), ((float)nCoolSP / 10.0));
				}
				else
				{
					strTemp.Format(_T("%0.1f F"), ((float)nCoolSP / 10.0));
				}
				m_DayCoolEdit.FieldText(strTemp);
			 
				if (g_unint)
				{
					strTemp.Format(_T("%0.1f C"), ((float)nHeatSP / 10.0));
				}
				else
				{
					strTemp.Format(_T("%0.1f F"), ((float)nHeatSP / 10.0));
				}
				m_DayHeatEdit.FieldText(strTemp);





                break;
            }







        }

        }

        //  Sleep(1000);

        m_active_key_mouse=FALSE;

        FlexSP = 1;

        FlexSPN = 1;


      //  InitFlexSliderBars_tstat6();

        return 1;
    }
    else
    {
        //0907
        /*CFSBContainer* pFSW = (CFSBContainer*)(wParam);*/

        int nModel = product_register_value[7];
        if (nModel==0)
        {
            return 0;
        }
        OnFlexSlideCallBackFor5ABCD((int)wParam);// 早期型号A－H的兼容，5E version(34.1 or 35.5)
        m_active_key_mouse=FALSE;
        return 1;

    }



}

//这个函数就是用来处理Slide的滑动写入的
//Tstat6
// LRESULT CT3000View::OnFlexSlideCallBack(WPARAM wParam, LPARAM lParam)
// {
//
// 	//HWND  Hret =m_pDayTwoSP->m_hWnd;//GetSafeHwnd( );
// 	//int rett = m_pNightTwoSP->Getflag();
//     BOOL slider_change=FALSE;
// 	int nMin, nMax;
// 	int nCoolSP = 0;
// 	int nHeatSP = 0;
// 	int nSP =0;
//     m_active_key_mouse=TRUE;
// 	int nHDB=0;
// 	int nCDB=0;
//     int Cool_Max =0;
//     int Heat_Max =0;
//     m_new_deadband = 0;
//     m_old_deadband = 0;
// 	if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i))
// 	{
//         int Increment=product_register_value[MODBUS_SETPOINT_INCREASE];
//
// 		switch(wParam)
// 		{
// 		case 1://右侧 晚上 2SP
// 			{
// 				//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
// 				//352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
// 				//353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
// 				    vector<int>  szPos(2);
// 				    m_pNightTwoSP->GetRange(nMin, nMax);
// 				    int nNum = m_pNightTwoSP->GetPos(szPos);
//
//
// 				    int nHeatSP = szPos[0] + nMin;
// 				    int nCoolSP = szPos[1] + nMin;
//
//
// 					nSP =  product_register_value[MODBUS_NIGHT_SETPOINT];
//                     Cool_Max = nMax-nSP;
//                     Heat_Max = nSP - nMin;
// 					if (nSP-nHeatSP < Increment)
// 					{
//                        nHeatSP = nSP - Increment;
// 					}
// 					if (nCoolSP-nSP <Increment)
// 					{
// 						nCoolSP = nSP + Increment;
// 					}
//
//                     if (nSP-nHeatSP >=Heat_Max)
//                     {
//                         nHeatSP=nSP-Heat_Max;
//                     }
//
//                     if (nCoolSP-nSP>=Cool_Max)
//                     {
//                         nCoolSP=nSP+Cool_Max;
//                     }
// 					nHDB=nSP-nHeatSP;
// 					nCDB=nCoolSP-nSP;
//
// 			        if ((nHDB<0&&nHDB>254)&&(nCDB<0&&nCDB>254))
// 			        {
//
// 						InitFlexSliderBars_tstat6();
// 						m_active_key_mouse=FALSE;
// 						return 0;
// 			        }
//
// 					int	Tstat_nSP= (short)product_register_value[MODBUS_NIGHT_SETPOINT];
// 					int	Tstat_nCoolSP = (short)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
// 					int	Tstat_nHeatSP = (short)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
//
// 					int ret=0;
//
//
// 					BeginWaitCursor();
// 					if (nHeatSP!=Tstat_nHeatSP)
// 					{
//                         slider_change = TRUE;
//                        // nHDB=nSP-nHeatSP;
//                         m_old_deadband = Tstat_nSP - Tstat_nHeatSP;
// 						 nHDB = m_old_deadband;
//                         if (nHeatSP>Tstat_nHeatSP)
// 						{
// 							 //nHDB=Round_SetPoint_Min(nHDB);
//                              nHDB-=Increment;
// 						}
// 						else
// 						{
// 							// nHDB=Round_SetPoint_Max(nHDB);
//                               nHDB+=Increment;
// 						}
//
//                         if (nHDB<Increment)
// 						{
//                             nHDB = Increment;
// 						}
//                         if (nHDB>254)
//                         {
//                             nHDB = 254;
//                         }
//
//  						int ret=0;
//  						ret =write_one(g_tstat_id, MODBUS_NIGHT_HEATING_DEADBAND,nHDB,5);//t6 354
//  						if (ret<0)
//  						{
//  							/*AfxMessageBox(_T("Write Fail!"));*/
//  							m_active_key_mouse=FALSE;
//                             InitFlexSliderBars_tstat6();
//  							return 0;
//  						}
//  						//product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]=nHDB;
//                         m_new_deadband = nHDB;
//                         m_slider_Type = 1;
//                         SetTimer(10,500,NULL);
//
// //  Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,nHDB,
// // 	product_register_value[MODBUS_NIGHT_HEATING_DEADBAND],this->m_hWnd,0,_T(""));
//
// 					}
//
//                     if (nCoolSP!=Tstat_nCoolSP)
// 					{         slider_change = TRUE;
// 						  //nCDB=nCoolSP-nSP;
//                           m_old_deadband = Tstat_nCoolSP - Tstat_nSP;
//                           nCDB = m_old_deadband;
// 						if (nCoolSP>Tstat_nCoolSP)
// 						{
// 							//nCDB=Round_SetPoint_Max(nCDB);
//                             nCDB+=Increment;
// 						}
// 						else
// 						{
// 							//nCDB=Round_SetPoint_Min(nCDB);
//                              nCDB-=Increment;
// 						}
//
//                           if (nCDB<Increment)
//                           {
//                             nCDB=Increment;
//                           }
//                           if (nCDB>=254)
//                           {
//                             nCDB=254;
//                           }
//
// 						//if ((nCDB<0&&nCDB>254))
// 						//{
// 						//	/*AfxMessageBox(_T("Deadband>254 or Deadband <1"));*/
// 						//	InitFlexSliderBars_tstat6();
// 						//	m_active_key_mouse=FALSE;
// 						//	return 0;
// 						//}
// // 						 					   Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,nCDB,
// // 						 						   product_register_value[MODBUS_NIGHT_COOLING_DEADBAND],this->m_hWnd,0,_T(""));
//
//  						int ret=0;
//  						ret =write_one(g_tstat_id, MODBUS_NIGHT_COOLING_DEADBAND,nCDB,5);//t6 355
//  						if (ret<0)
//  						{
//  							/*AfxMessageBox(_T("Write Fail!"));*/
//  							m_active_key_mouse=FALSE;
//                             InitFlexSliderBars_tstat6();
//  							return 0;
//  						}
//                         m_new_deadband=nCDB;
//                          m_slider_Type = 2;
//                          SetTimer(10,500,NULL);
//  						//product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]=nCDB;
// 					}
//
//
// 					EndWaitCursor();
//
//
//
//
//                break;
//
// 			}
// 		case 2://右侧 晚上 1SP
// 			{
// 				//350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
// 				//354	2	Full	W/R	(Night)Unoccupied heating setpoint
// 				//355	2	Full	W/R	(Night)Unoccupied cooling setpoint
// 				int nRangeMin	=(short)product_register_value[MODBUS_MIN_SETPOINT];
// 				int nRangeMax	=(short)product_register_value[MODBUS_MAX_SETPOINT];
//
//
// 				vector<int>  szPos(3);
// 				m_pNightSingleSP->GetRange(nMin, nMax);
// 				int nNum = m_pNightSingleSP->GetPos(szPos);
//
// 				int nHeatSP = szPos[0] + nMin;
// 				int nSP = szPos[1] + nMin;
// 				int nCoolSP= szPos[2] + nMin;
//
// 				int	Tstat_nSP= (short)product_register_value[MODBUS_NIGHT_SETPOINT];
// 				int	Tstat_nCoolSP = (short)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
// 				int	Tstat_nHeatSP = (short)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
//                 int NightSP = Tstat_nSP;
//                 m_old_deadband =  Tstat_nSP;
// 				if (Tstat_nSP!=nSP)
// 				{     slider_change = TRUE;
// 					if (nSP>Tstat_nSP)
// 					{
// 						//nSP=Round_SetPoint_Max(nSP);
//                          NightSP+=Increment;
// 					}
// 					else
// 					{
// 						//nSP=Round_SetPoint_Min(nSP);
//                          NightSP-=Increment;
// 					}
//                     if (nSP<nRangeMin*10)  //  ||
//                     {
//                         nSP = nRangeMin*10;
//                     }
//                     if (nSP>nRangeMax*10)
//                     {
//                         nSP = nRangeMax*10;
//                     }
//
//  					int ret=write_one(g_tstat_id, MODBUS_NIGHT_SETPOINT, nSP,5);//t6 350
//   					if (ret<0)
//   					{
//   						/*AfxMessageBox(_T("Write Fail!"));*/
//   						m_active_key_mouse=FALSE;
//                         InitFlexSliderBars_tstat6();
//   						return 0;
//   					}
//  					//product_register_value[MODBUS_NIGHT_SETPOINT]=NightSP;
//                     m_slider_Type = 3;
//                     m_new_deadband =nSP;
//                     SetTimer(10,500,NULL);
// //  					Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_SETPOINT,nSP,
// //  						product_register_value[MODBUS_NIGHT_SETPOINT],this->m_hWnd,0,_T(""));
//
// 				}
//
//
//
//
// 			}
// 			break;
// 		case 3:
// 		//左侧 白天 2SP
// 			{
// 				// 		345	1	Low byte	W/R	(Day)Occupied   setpoint
// 				// 		346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
// 				// 		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
// 				vector<int>  szPos(2);
// 				m_pDayTwoSP->GetRange(nMin, nMax);
// 				int nNum = m_pDayTwoSP->GetPos(szPos);
// 				int nHeatSP = szPos[0] + nMin;
// 				int nCoolSP = szPos[1] + nMin;
//
//                     nSP= product_register_value[MODBUS_DAY_SETPOINT];
//
//                     Cool_Max = nMax-nSP;
//                     Heat_Max = nSP - nMin;
//
//                     if (nSP-nHeatSP<Increment)
//                     {
//                         nHeatSP = nSP - Increment;
//
//                     }
//                     if (nCoolSP-nSP<Increment)
//                     {
//                         nCoolSP = nSP + Increment;
//                     }
//
//                     if (nSP-nHeatSP >=Heat_Max)
//                     {
//                         nHeatSP=nSP-Heat_Max;
//                     }
//
//                     if (nCoolSP-nSP>=Cool_Max)
//                     {
//                         nCoolSP=nSP+Cool_Max;
//                     }
//
// 					nHDB=nSP-nHeatSP;
// 					nCDB=nCoolSP-nSP;
// 					if ((nHDB<0&&nHDB>254)&&(nCDB<0&&nCDB>254))
// 					{
// 						/*AfxMessageBox(_T("Deadband>254 or Deadband <0"));*/
// 						InitFlexSliderBars_tstat6();
// 						return 0;
// 					}
// 					int	Tstat_nSP= (short)product_register_value[MODBUS_DAY_SETPOINT];
// 					int	Tstat_nCoolSP = (short)product_register_value[MODBUS_DAY_COOLING_SETPOINT];
// 					int	Tstat_nHeatSP = (short)product_register_value[MODBUS_DAY_HEATING_SETPOINT];
// 					BeginWaitCursor();
//
// 					if (nHeatSP!=Tstat_nHeatSP)
// 				    {        slider_change = TRUE;
// 						//nHDB=nSP-nHeatSP;
//                         m_old_deadband=Tstat_nSP - Tstat_nHeatSP;
//                         nHDB = m_old_deadband;
// 						if (nHeatSP>Tstat_nHeatSP)
// 						{
// 							//nHDB=Round_SetPoint_Min(nHDB);
//                             nHDB-=Increment;
// 						}
// 						else
// 						{
// 							//nHDB=Round_SetPoint_Max(nHDB);
//                              nHDB+=Increment;
// 						}
//                         if (nHDB<Increment)
//                         {
//                             nHDB=Increment;
//                         }
//                         if (nHDB>=254)
//                         {
//                             nHDB=254;
//                         }
// 						//if (nHDB<1&&nHDB>254)
// 						//{
// 						//	/*AfxMessageBox(_T("Deadband>254 or Deadband <1"));*/
// 						//	InitFlexSliderBars_tstat6();
// 						//	m_active_key_mouse=FALSE;
// 						//	return 0;
// 						//}
//
//  						int ret=  write_one(g_tstat_id, MODBUS_DAY_HEATING_DEADBAND,nHDB,5);//t6 349;
//  						if (ret<0)
//  						{
//  							 InitFlexSliderBars_tstat6();
//  							m_active_key_mouse=FALSE;
//  							return 0;
//  						}
//                         m_new_deadband = nHDB;
//                          m_slider_Type = 4;
//                          SetTimer(10,500,NULL);
//  						//product_register_value[MODBUS_DAY_HEATING_DEADBAND]=nHDB;
//
// 				    }
//
// 					if (nCoolSP!=Tstat_nCoolSP)
// 					{         slider_change = TRUE;
// 						   //nHDB=nSP-nHeatSP;
// 							//nCDB=nCoolSP-nSP;
//                             m_old_deadband= Tstat_nCoolSP - Tstat_nSP;
//                             nCDB =  m_old_deadband;
// 							if (nCoolSP>Tstat_nCoolSP)
// 							{
// 								//nCDB=Round_SetPoint_Max(nCDB);
//                                  nCDB += Increment;
// 							}
// 							else
// 							{
// 								//nCDB=Round_SetPoint_Min(nCDB);
//                                  nCDB -= Increment;
// 							}
//                             if (nCDB<Increment)
//                             {
//                                 nCDB=Increment;
//                             }
//                             if (nCDB>=254)
//                             {
//                                 nCDB=254;
//                             }
// 							//if (nCDB<0&&nCDB>254)
// 							//{
// 							///*	AfxMessageBox(_T("Deadband>254 or Deadband <1"));*/
// 							//	InitFlexSliderBars_tstat6();
// 							//	m_active_key_mouse=FALSE;
// 							//	return 0;
// 							//}
//
//  						 	int ret=write_one(g_tstat_id, MODBUS_DAY_COOLING_DEADBAND,nCDB,5);//t6 348;
//  						 		if (ret<0){
//
//                                         m_active_key_mouse=FALSE;
//  						 				return 0;
//
//  						 				}
//                                          m_slider_Type = 5;
//                                         m_new_deadband = nCDB;
//                                         SetTimer(10,500,NULL);
//  										//product_register_value[MODBUS_DAY_COOLING_DEADBAND]=nCDB;
//
//
// 					}
//
//
// 					EndWaitCursor();
//
// 				}
//
// 				break;
// 		case 4://左侧 白天 1SP
// 			{
//
// 				//348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
// 				//349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)
// 				int nRangeMin	=(short)product_register_value[MODBUS_MIN_SETPOINT];
// 				int nRangeMax	=(short)product_register_value[MODBUS_MAX_SETPOINT];
// 				vector<int>  szPos(3);
// 				m_pDaySingleSP->GetRange(nMin, nMax);
// 				int nNum = m_pDaySingleSP->GetPos(szPos);
//
// 				int nHeatSP = szPos[0] + nMin;//最小
// 				int nSP = szPos[1] + nMin;
// 				int  nCoolSP= szPos[2] + nMin;//最大
//
//
// 				int	Tstat_nSP= (short)product_register_value[MODBUS_DAY_SETPOINT];
// 				int	Tstat_nCoolSP = (short)product_register_value[MODBUS_DAY_COOLING_SETPOINT];
// 				int	Tstat_nHeatSP = (short)product_register_value[MODBUS_DAY_HEATING_SETPOINT];
// 		         int DaySP = nSP;
// 				if (Tstat_nSP!=nSP)
// 				{    slider_change = TRUE;
//
// 					if (nSP>Tstat_nSP)
// 					{
// 						//nSP=Round_SetPoint_Max(nSP);
//                         DaySP+=Increment;
// 					}
// 					else
// 					{
// 						//nSP=Round_SetPoint_Min(nSP);
//                        DaySP-=Increment;
// 					}
//                     if (DaySP<nRangeMin*10) // ||
//                     {
//                         DaySP = nRangeMin*10;
//                     }
//                     if (DaySP>nRangeMax*10)
//                     {
//                         DaySP = nRangeMax*10;
//                     }
// 					//nSP=Round_SetPoint_Max(nSP);
//   					int ret= write_one(g_tstat_id, MODBUS_DAY_SETPOINT,DaySP ,5);//345 T6
//   					if (ret<0)
//   					{
//   						/*AfxMessageBox(_T("Write Fail!"));*/
//   						m_active_key_mouse=FALSE;
//                         InitFlexSliderBars_tstat6();
//   						return 0;
//   					}
//
//  					//product_register_value[MODBUS_DAY_SETPOINT]=DaySP;
//                     m_new_deadband = DaySP;
//                       m_slider_Type = 6;
//                       SetTimer(10,500,NULL);
// 					break;
// 				}
//
//
//
//
//
//
//
// 			}
//
// 		}
//
//       //  Sleep(1000);
//
//         if ( !slider_change )
//         {
//             m_active_key_mouse=FALSE;
//
//             FlexSP = 1;
//
//             FlexSPN = 1;
//
//
//             SetFlexSliderBars_tstat6();
//         }
//
//
//       return 1;
//     }
//    else{
//     //0907
//     /*CFSBContainer* pFSW = (CFSBContainer*)(wParam);*/
//
//     int nModel = product_register_value[7];
// 	if (nModel==0)
// 	{
// 		return 0;
// 	}
//     OnFlexSlideCallBackFor5ABCD((int)wParam);// 早期型号A－H的兼容，5E version(34.1 or 35.5)
// 	m_active_key_mouse=FALSE;
//     return 1;
//
//  }
//
//
//
// }

// 这是5E以及更早的产品的兼容
// void CT3000View::OnFlexSlideCallBackFor5E()
// {
// 	///*
// 	BeginWaitCursor();
//
// 	int nMin, nMax;
// 	CString strTemp;
// 	int min=product_register_value[132];
// 	int max=product_register_value[131];
//
// 	int m_hotel_or_office=product_register_value[125];///////hotel or office
// 	m_hotel_or_office=1;
// 	if (m_hotel_or_office == 0) // office
// 	{
// 		vector<int>  szPos(3);
// 		m_pNightSingleSP->GetRange(nMin, nMax);
// 		int nNum = m_pNightSingleSP->GetPos(szPos);
// 		int nHeatSP = szPos[0] + nMin;
// 		int nSP = szPos[1] + nMin;
// 		int nCoolSP = szPos[2] + nMin;
//
// 		int nStart = GetTickCount();
// 		write_one(g_tstat_id, 135, nSP);
//
// 		int nEnd = GetTickCount();
// 		write_one(g_tstat_id, 183, nCoolSP);
// 		write_one(g_tstat_id, 182, nHeatSP);
//
//
// 		CString str;
// 		str.Format(_T("Time1 elapsed : %d \n"), nEnd-nStart);
// 		TRACE(str);
// 	}
// 	else  // hotel
// 	{
// 		vector<int>  szPos(3);
// 		m_pNightSingleSP->GetRange(nMin, nMax);
// 		int nNum = m_pNightSingleSP->GetPos(szPos);
// 		int nHeatSP = szPos[0] + nMin;
// 		int nSP = szPos[1] + nMin;
// 		int nCoolSP = szPos[2] + nMin;
// 		/*write_one(g_tstat_id, 135, nSP);
// 		write_one(g_tstat_id, 124, nCoolSP - nSP);
// 		write_one(g_tstat_id, 123, nSP - nHeatSP);*/
//
// 	}
//
//
//
// 	InitSliderBars2();
//
//
//
// 	EndWaitCursor();
// 	//*/
// }

// 除了5E以外的所有型号



void CT3000View::OnFlexSlideCallBackFor5ABCD( int response )
{

    // 目前实现的是以Runar5E，ver ＝34.1 and ver 35.5 为样本.
    //0907以下
    /*    BeginWaitCursor();*/

    int nMin, nMax;

    //  CString strTemp;
    //  int min=(short)product_register_value[MODBUS_MIN_SETPOINT];
    //  int max=(short)product_register_value[MODBUS_MAX_SETPOINT];

    //  int m_hotel_or_office=0;
    //  short nOccupied = product_register_value[184];  // Day setpoint option
    //  BOOL bOccupied = nOccupied & 0x0001;


    //
    //

    //int nOfficeSelReg=MODBUS_APPLICATION;
    //if (bOccupied)
    //{
    // if((product_register_value[7]==PM_TSTAT5E && m_fFirmwareVersion >= 35.4)||(product_register_value[7]==PM_TSTATRUNAR))
    // {
    //	 nOfficeSelReg = MODBUS_DAYSETPOINT_OPTION;
    // }
    //}
    //else
    //{
    // nOfficeSelReg = MODBUS_APPLICATION;
    //}
    // m_hotel_or_office=product_register_value[nOfficeSelReg];
    int nSP,dSP;
    nSP=dSP=0;
    int nCoolSP=0;
    int nHeatSP=0;
    int nCoolDB=0;
    int nHeatDB=0;

    int dCoolSP=0;
    int dHeatSP=0;
    int dCoolDB=0;
    int dHeatDB=0;

    int m_hotel_or_office=product_register_value[MODBUS_APPLICATION];
    if (product_type==T3000_5ABCDFG_LED_ADDRESS)
    {
        dSP=product_register_value[MODBUS_COOLING_SETPOINT];
        dCoolDB=product_register_value[MODBUS_COOLING_DEADBAND]/10;//119
        dHeatDB=product_register_value[MODBUS_HEATING_DEADBAND]/10;//heat//120
        dCoolSP=dSP+dCoolDB;
        dHeatSP=dSP-dHeatDB;
        if (m_hotel_or_office==0)// office
        {
            nSP=0;
            nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10;
            nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10;
            nSP = (short)(nCoolSP+nHeatSP)/2 ;
            nCoolDB = nCoolSP - nSP;
            nHeatDB = nSP - nHeatSP;
        }
        else//Hotel
        {
            nSP=dSP;

            nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
            nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));
            nCoolSP = nSP + nCoolDB;
            nHeatSP = nSP - nHeatDB;
        }

    }


    if (product_type==T3000_5EH_LCD_ADDRESS)
    {


        if (m_hotel_or_office==0)// office
        {
            //Setpoint = ( NEW_EEP_CoolingSp + EEP_HeatingSp) / 2
            //cooling_db = NEW_EEP_CoolingSp - Setpoint;
            //heating_db = Setpoint - EEP_HeatingSp;

            dCoolSP=product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10;
            dHeatSP=product_register_value[MODBUS_HEATING_SETPOINT]/10;
            dSP=(dCoolSP+dHeatSP)/2;
            dCoolDB=dCoolSP-dSP;
            dHeatDB=dSP-dHeatSP;

            nSP=0;
            nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
            nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
            nSP = (short)(nCoolSP+nHeatSP)/2 ;
            nCoolDB = nCoolSP - nSP;
            nHeatDB = nSP - nHeatSP;
        }
        else//Hotel
        {
            // Hotel mode:
            //Setpoint = EEP_CoolingSp
            // cooling_db = EEP_CoolDeadband ;
            //heating_db = EEP_HeatDeadband ;



            dSP=product_register_value[MODBUS_COOLING_SETPOINT]/10;

            dCoolDB=product_register_value[MODBUS_COOLING_DEADBAND]/10;//119

            dHeatDB=product_register_value[MODBUS_HEATING_DEADBAND]/10;//heat//120

            dCoolSP=dSP+dCoolDB;
            dHeatSP=dSP-dHeatDB;

            nSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];

            nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
            nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));

            nCoolSP = nSP + nCoolDB;
            nHeatSP = nSP - nHeatDB;
        }

    }

    //day
    if (response == 3 ) // office - occupied : 2SP
    {
        vector<int>  szPos(3);
        m_pDayTwoSP->GetRange(nMin, nMax);
        int nNum = m_pDayTwoSP->GetPos(szPos);

        int day_slider_HeatSP = (szPos[0] + nMin)/10;

        int day_slider_CoolSP = (szPos[1] + nMin)/10;



        if ((day_slider_CoolSP - dSP<0)||(dSP - day_slider_HeatSP<0))
        {
            //AfxMessageBox(_T("Error:>=Setpoint OR <=Setpoint"));
            CString strTemp;
            if (day_slider_CoolSP<dSP)
            {
                strTemp.Format(_T("Day Cooling Setpoint(%d) should be more than Day Setpoint(%d)"),day_slider_CoolSP,dSP);

            }
            else
            {
                strTemp.Format(_T("Day Heating Setpoint(%d) should be less than Day Setpoint(%d)"),day_slider_HeatSP,dSP);
            }
            AfxMessageBox(strTemp);
            InitSliderBars2();
            return ;
        }

        int ret1= write_one(g_tstat_id, MODBUS_COOLING_DEADBAND, (nCoolSP - nSP)*10);
        int ret2=write_one(g_tstat_id, MODBUS_HEATING_DEADBAND, (nSP - nHeatSP)*10);
        if (ret1>0&&ret2>0)
        {
            product_register_value[MODBUS_COOLING_DEADBAND]=(nCoolSP - nSP)*10;
            product_register_value[MODBUS_HEATING_DEADBAND]=(nSP - nHeatSP)*10;

        }
        else
        {
            AfxMessageBox(_T("Write Fail,Try again!"));
        }

    }
    //
    if (response == 4)
        // hotel - occupied  //  1SP
    {
        vector<int>  szPos(3);
        m_pDaySingleSP->GetRange(nMin, nMax);
        int nNum = m_pDaySingleSP->GetPos(szPos);

        int Slider_nHeatSP = (szPos[0] + nMin)/10;
        int Slider_nSP = (szPos[1] + nMin)/10;
        int Slider_nCoolSP = (szPos[2] + nMin)/10;

        nMax/=10;
        nMin/=10;

        if (Slider_nSP>nMax||Slider_nSP<nMin)
        {
            AfxMessageBox(_T("Setpoint is out of the scale"));
            InitSliderBars2();
            return;
        }


        if (Slider_nSP!=dSP)
        {
            int ret=write_one(g_tstat_id, MODBUS_COOLING_SETPOINT, Slider_nSP);	//135
            if (ret>0)
            {
                product_register_value[MODBUS_COOLING_SETPOINT]=Slider_nSP;
            }
            else
            {
                AfxMessageBox(_T("Write Fail,Try again!"));
                return  ;
            }
        }
        else
        {
            if (Slider_nHeatSP!=dHeatSP)
            {
                int ret2=write_one(g_tstat_id, MODBUS_HEATING_DEADBAND, (Slider_nSP - Slider_nHeatSP)*10);
                if (ret2>0)
                {

                    product_register_value[MODBUS_HEATING_DEADBAND]=(Slider_nSP - Slider_nHeatSP)*10;

                }
                else
                {
                    AfxMessageBox(_T("Write Fail,Try again!"));
                    return ;
                }
            }
            if (Slider_nCoolSP!=dCoolSP)
            {
                int ret2=write_one(g_tstat_id, MODBUS_COOLING_DEADBAND, (Slider_nSP - Slider_nHeatSP)*10);
                if (ret2>0)
                {

                    product_register_value[MODBUS_COOLING_DEADBAND]=(Slider_nCoolSP - Slider_nSP)*10;

                }
                else
                {
                    AfxMessageBox(_T("Write Fail,Try again!"));
                    return ;
                }
            }
        }




    }
    //night
    if (response == 1) // office - unoccupied
    {
        //2sp
        vector<int>  szPos(3);
        m_pNightTwoSP->GetRange(nMin, nMax);
        int nNum = m_pNightTwoSP->GetPos(szPos);
        int nHeatSP = (szPos[0] + nMin)/10;
        //int nSP = szPos[1] + nMin;
        //int nSP = product_register_value[135];//374
        int nCoolSP = (szPos[1] + nMin)/10;
        int nSP =(nCoolSP - nHeatSP)/2 +nHeatSP;


        int Tstat_nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10;
        int Tstat_nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10;
        if (Tstat_nCoolSP!=nCoolSP)
        {
            int ret1= write_one(g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT, nCoolSP*10);
            if (ret1>0)
            {
                product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]=nCoolSP*10;
            }
        }

        if (Tstat_nHeatSP!= nHeatSP)
        {
            int ret2=  write_one(g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, nHeatSP*10);
            if (ret2>0)
            {

                product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=nHeatSP*10;

            }
        }



    }

    if (response == 2) // hotel - unoccupied
    {
        //1sp
        vector<int>  szPos(3);
        m_pNightSingleSP->GetRange(nMin, nMax);
        int nNum = m_pNightSingleSP->GetPos(szPos);
        int Slider_nHeatSP = (szPos[0] + nMin)/10;
        int Slider_nSP = (szPos[1] + nMin)/10;
        //int nSP = product_register_value[374]/10;
        int Slider_nCoolSP = (szPos[2] + nMin)/10;

        nMin /=10;
        nMax /=10;
        if (Slider_nSP>nMax||Slider_nSP<nMin)
        {
            AfxMessageBox(_T("Setpoint is out of the scale"));
            InitSliderBars2();
            return  ;
        }
        if (nSP!=Slider_nSP)
        {
            int nCoolDB = Slider_nCoolSP-Slider_nSP;
            int nHeatDB = Slider_nSP - Slider_nHeatSP;
            int ret1 =write_one(g_tstat_id, MODBUS_COOLING_SETPOINT, Slider_nSP);
            if (ret1>0)
            {
                product_register_value[MODBUS_COOLING_SETPOINT]=nSP;
                product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=nHeatDB*10;
                product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]=nCoolDB*10;
            }
            else
            {
                AfxMessageBox(_T("Write Fail,Try again!"));
                return  ;
            }
        }
        else
        {
            if (Slider_nHeatSP!=nHeatSP)
            {

                //int ret1= write_one(g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT, nCoolSP);
                int ret2=  write_one(g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, Slider_nCoolSP);
                if (ret2>0)
                {
                    product_register_value[MODBUS_COOLING_SETPOINT]=(Slider_nCoolSP-Slider_nHeatSP)/2 + Slider_nHeatSP;
                    product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=Slider_nHeatSP;
                    product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]=Slider_nCoolSP;
                }
                else
                {
                    AfxMessageBox(_T("Write Fail,Try again!"));
                    return  ;
                }
            }
            if (Slider_nCoolSP!=nCoolSP)
            {

                //int ret1= write_one(g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT, Slider_nCoolSP);
                int ret2=  write_one(g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, Slider_nHeatSP);
                if (ret2>0)
                {
                    product_register_value[MODBUS_COOLING_SETPOINT]=(Slider_nCoolSP-Slider_nHeatSP)/2 + Slider_nHeatSP;
                    product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=Slider_nHeatSP;
                    product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]=Slider_nCoolSP;
                }
                else
                {
                    AfxMessageBox(_T("Write Fail,Try again!"));
                    return  ;
                }
            }
        }



    }


    InitSliderBars2();
    /*    EndWaitCursor();*/
}


void CT3000View::InitFanSpeed()
{


    CString p[8]= {_T("Fan Off"),_T("Fan On"),_T("Fan Low"),_T("Fan Mid"),_T("Fan High"),_T("Fan Aut"),_T("Heat Only"),_T("Cool Only")};

    p[0]= GetTextFromReg(737);

    if (p[0].IsEmpty())
    {
        p[0]=_T("Fan Off");
    }
    p[1]= GetTextFromReg(741);

    if (p[1].IsEmpty())
    {
        p[1]=_T("Fan On");
    }
    p[2]= GetTextFromReg(741);

    if (p[2].IsEmpty())
    {
        p[2]=_T("Fan Low");
    }
    p[3]= GetTextFromReg(745);

    if (p[3].IsEmpty())
    {
        p[3]=_T("Fan Mid");
    }
    p[4]= GetTextFromReg(749);
    if (p[4].IsEmpty())
    {
        p[4]=_T("Fan High");
    }
    p[5]= GetTextFromReg(753);

    if (p[5].IsEmpty())
    {
        p[5]=_T("Fan Aut");
    }
    p[6]= GetTextFromReg(757);

    if (p[6].IsEmpty())
    {
        p[6]=_T("Heat Only");
    }
    p[7]= GetTextFromReg(761);
    if (p[7].IsEmpty())
    {
        p[7]=_T("Cool Only");
    }

    m_FanComBox.ResetContent();

    if(product_register_value[MODBUS_AUTO_ONLY]==BST_CHECKED)	//129   107
    {
        m_FanComBox.AddString(p[0]);
        m_FanComBox.AddString(p[5]);
    }
    else
    {
        switch(product_register_value[MODBUS_FAN_MODE]-1)		//122   105
        {
        case 0:
            m_FanComBox.AddString(p[0]);
            m_FanComBox.AddString(p[1]);
            m_FanComBox.AddString(p[5]);
            m_FanComBox.AddString(p[6]);
            m_FanComBox.AddString(p[7]);
            break;
        case 1:
            m_FanComBox.AddString(p[0]);
            m_FanComBox.AddString(p[2]);
            m_FanComBox.AddString(p[4]);
            m_FanComBox.AddString(p[5]);
            m_FanComBox.AddString(p[6]);
            m_FanComBox.AddString(p[7]);
            break;
        case 2:
            m_FanComBox.AddString(p[0]);
            m_FanComBox.AddString(p[2]);
            m_FanComBox.AddString(p[3]);
            m_FanComBox.AddString(p[4]);
            m_FanComBox.AddString(p[5]);
            m_FanComBox.AddString(p[6]);
            m_FanComBox.AddString(p[7]);
            break;
        default:
            m_FanComBox.AddString(p[0]);
            m_FanComBox.AddString(p[2]);
            m_FanComBox.AddString(p[3]);
            m_FanComBox.AddString(p[4]);
            m_FanComBox.AddString(p[5]);
            m_FanComBox.AddString(p[6]);
            m_FanComBox.AddString(p[7]);
            break;
        }
    }


    int sel = set_real_fan_select();
    m_FanComBox.SetCurSel(sel);

	m_FanComBox.ResetContent();

	if (product_register_value[7] == PM_TSTAT8)
	{
		switch (product_register_value[MODBUS_FAN_MODE])		//122   105
		{
		case 0:
			m_FanComBox.AddString(p[0]);
			 
			m_FanComBox.AddString(p[5]);
		 
			break;
		case 1:
			m_FanComBox.AddString(p[0]);
			m_FanComBox.AddString(p[1]);
		 
			m_FanComBox.AddString(p[5]);
		 
			break;
		case 2:
			m_FanComBox.AddString(p[0]);
			m_FanComBox.AddString(p[1]);
			m_FanComBox.AddString(p[2]);
		 
			m_FanComBox.AddString(p[5]);
	 
			break;
		case 3:
			m_FanComBox.AddString(p[0]);
			m_FanComBox.AddString(p[1]);
			m_FanComBox.AddString(p[2]);
			m_FanComBox.AddString(p[3]);
			m_FanComBox.AddString(p[5]);
			break;
		case 4:
			m_FanComBox.AddString(p[0]);
			m_FanComBox.AddString(p[1]);
			m_FanComBox.AddString(p[2]);
			m_FanComBox.AddString(p[3]);
			m_FanComBox.AddString(p[4]);
			m_FanComBox.AddString(p[5]);
			break;
		default:
			m_FanComBox.AddString(p[0]);
			m_FanComBox.AddString(p[2]);
			m_FanComBox.AddString(p[3]);
			m_FanComBox.AddString(p[4]);
			m_FanComBox.AddString(p[5]);
			m_FanComBox.AddString(p[6]);
			m_FanComBox.AddString(p[7]);
			break;
		}
	}
 
	m_FanComBox.SetCurSel(product_register_value[MODBUS_FAN_MODE]);
}

void CT3000View::HandleSliderSetPos( BOOL bRight )
{

    if (!bRight)
    {
        if (!m_bSliderSetPosWarning)
        {
            return;
        }

        m_bSliderSetPosWarning = TRUE;
    }
    else
    {
        m_bSliderSetPosWarning = TRUE;
    }
}
//这个是Tstat
void CT3000View::InitFlexSliderBars_tstat6()
{


    product_register_value[MODBUS_DAY_SETPOINT]=Round_SetPoint_Max(product_register_value[MODBUS_DAY_SETPOINT]);
    product_register_value[MODBUS_DAY_COOLING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_DAY_COOLING_DEADBAND]);
    product_register_value[MODBUS_DAY_HEATING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_DAY_HEATING_DEADBAND]);

    product_register_value[MODBUS_NIGHT_SETPOINT]=Round_SetPoint_Max(product_register_value[MODBUS_NIGHT_SETPOINT]);
    product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]);
    product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]);

    BOOL Day_Default=FALSE;
    BOOL Night_Default=FALSE;
    CString strTemp;

    int nRangeMin	=(short)product_register_value[MODBUS_MIN_SETPOINT];
    int nRangeMax	=(short)product_register_value[MODBUS_MAX_SETPOINT];
    if (nRangeMax<nRangeMin)
    {
        nRangeMax=50;
        nRangeMin=0;
    }

    float nCoolSP = 0;
    float nHeatSP = 0;
    float nSP =0;
    float nCDB=0;
    float nHDB=0;


    float dCoolSP = 0;
    float dHeatSP = 0;
    float dSP =0;
    float dCDB=0;
    float dHDB=0;


    int DayCoolingSP=0;
    int DaySP=0;
    int DayHeatingSP=0;

    int NightCoolingSP=0;
    int NightSP=0;
    int NightHeatingSP=0;

    CString strInfo;

    dSP= ((float)(short)product_register_value[MODBUS_DAY_SETPOINT])/10;
    dCDB = ((float)(short)product_register_value[MODBUS_DAY_COOLING_DEADBAND])/10;
    dHDB = ((float)(short)product_register_value[MODBUS_DAY_HEATING_DEADBAND])/10;
    dCoolSP=dSP+dCDB;
    dHeatSP=dSP-dHDB;

    DaySP=(short)product_register_value[MODBUS_DAY_SETPOINT];

    product_register_value[MODBUS_DAY_COOLING_SETPOINT]=product_register_value[MODBUS_DAY_SETPOINT]+product_register_value[MODBUS_DAY_COOLING_DEADBAND];
    DayCoolingSP=product_register_value[MODBUS_DAY_COOLING_SETPOINT];

    product_register_value[MODBUS_DAY_HEATING_SETPOINT]=product_register_value[MODBUS_DAY_SETPOINT]-product_register_value[MODBUS_DAY_HEATING_DEADBAND];
    DayHeatingSP=product_register_value[MODBUS_DAY_HEATING_SETPOINT];

    nSP =((float)(short) product_register_value[MODBUS_NIGHT_SETPOINT])/10;
    nCDB=((float)(short) product_register_value[MODBUS_NIGHT_COOLING_DEADBAND])/10;
    nHDB=((float)(short) product_register_value[MODBUS_NIGHT_HEATING_DEADBAND])/10;
    NightSP=(short) product_register_value[MODBUS_NIGHT_SETPOINT];
    nCoolSP=nSP+nCDB;
    nHeatSP=nSP-nHDB;
    product_register_value[MODBUS_NIGHT_COOLING_SETPOINT ]=product_register_value[MODBUS_NIGHT_SETPOINT]+product_register_value[MODBUS_NIGHT_COOLING_DEADBAND];
    NightCoolingSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
    product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=product_register_value[MODBUS_NIGHT_SETPOINT]-product_register_value[MODBUS_NIGHT_HEATING_DEADBAND];
    NightHeatingSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
    DayMax = GetRoundMM(TRUE,dCoolSP,nCoolSP,1.1);
    DayMin = GetRoundMM(FALSE,dHeatSP,nHeatSP,0.9);

    if (!(DayHeatingSP<=DaySP&&DaySP<=DayCoolingSP))
    {
        Day_Default=TRUE;

    }
    if (DaySP<DayMin*10||DaySP>DayMax*10)
    {
        Day_Default=TRUE;
    }

    if(Day_Default)
    {
        DayMax=50;
        DayMin=0;

        DayHeatingSP=200;
        DaySP=250;
        DayCoolingSP=300;
    }

    if (!(NightHeatingSP<=NightSP&&NightSP<=NightCoolingSP))
    {
        Night_Default=TRUE;
    }
    if (NightSP<DayMin*10||NightSP>DayMax*10)
    {
        Night_Default=TRUE;
    }

    if(Night_Default)
    {
        DayMax=50;
        DayMin=0;

        NightHeatingSP=200;
        NightSP=250;
        NightCoolingSP=300;

    }


    m_singlesliderday.SetThumbColor(RGB(96,192,0));
    m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
    m_singlesliderday.SetChannelColor(RGB(196, 196, 255));

    m_singlesliderday.SetRange((short)DayMin*10, (short)DayMax*10,TRUE);

    int currenttemp=(short)DayMin*10+(short)DayMax*10-(int)product_register_value[MODBUS_TEMPRATURE_CHIP];
    m_singlesliderday.SetPos(currenttemp);

#if 1
    if (FlexSP == 1)
    {
        //以下是2 SP
        //	MDAY=1;
        // MDAY=0;

        if (MDAY==1) // office  2SP day
        {


            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_DAY)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_DAY)->SetWindowText(strInfo);
            m_pDayTwoSP->SetRange(DayMax*10,DayMin*10);

            BOOL bRetSP = m_pDayTwoSP->SetPos_tstat6_2pos(DayHeatingSP, DaySP,DayCoolingSP);
            if (bRetSP)
            {
                CString strInfo;
                strInfo.Format(_T("nHeatSP=%d;nSP=%d;nCoolSP=%d"),nCoolSP, nSP,nHeatSP);
                HandleSliderSetPos(bRetSP);//tstat6
            }
            m_pDayTwoSP->ShowWindow(SW_NORMAL);
            m_pDaySingleSP->ShowWindow(SW_HIDE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),((float)((short)product_register_value[MODBUS_DAY_COOLING_SETPOINT])/10.0) );
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),((float)((short)product_register_value[MODBUS_DAY_COOLING_SETPOINT])/10.0) );
            }

            m_DayCoolEdit.FieldText(strTemp);
            //m_DayCoolEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),((float)((short)product_register_value[MODBUS_DAY_HEATING_SETPOINT])/10.0));
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),((float)((short)product_register_value[MODBUS_DAY_HEATING_SETPOINT])/10.0));
            }


            m_DayHeatEdit.FieldText(strTemp);
            //m_DayHeatEdit.SetNumberDefaultTex();
            m_daysetpoint = nSP;
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),((float)((short)product_register_value[MODBUS_DAY_SETPOINT])/10.0));
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),((float)((short)product_register_value[MODBUS_DAY_SETPOINT])/10.0));
            }

            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);
            m_dayInfoEdit.ShowWindow(FALSE);
            UpdateData(FALSE);

            m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
            m_DayHeatStatic.SetWindowText(_T("Heating SP"));

        }
        else  // hotel 1 SP day
        {
            //GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);
            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_DAY)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_DAY)->SetWindowText(strInfo);
            if (DayHeatingSP<DaySP&&DaySP<DayCoolingSP)
            {
            }
            else
            {
                DayMax =5;
                DayMin = 0;
                DayHeatingSP = 1;
                DaySP = 2;
                DayCoolingSP = 3;
            }

            m_pDaySingleSP->SetRange(DayMax*10, DayMin*10);

            BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(DayHeatingSP, DaySP,DayCoolingSP);//tstat6

            HandleSliderSetPos(bRetSP);//tstat6






            m_pDaySingleSP->ShowWindow(SW_NORMAL);
            m_pDayTwoSP->ShowWindow(SW_HIDE);



            if (g_unint)
            {
                //strTemp.Format(_T("%d C"), nHeatSP);
                strTemp.Format(_T("%0.1f C"), (float)dSP);
            }
            else
            {

                strTemp.Format(_T("%0.1f F"), (float)dSP);
            }
            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);
            m_dayInfoEdit.ShowWindow(TRUE);
            if (g_unint)
            {
                //strTemp.Format(_T("%d C"), nHeatSP);
                strTemp.Format(_T("%0.1f C"), (float)dCoolSP);
            }
            else
            {

                strTemp.Format(_T("%0.1f F"), (float)dCoolSP);
            }
            m_DayCoolEdit.FieldText(strTemp);
            if (g_unint)
            {
                //strTemp.Format(_T("%d C"), nHeatSP);
                strTemp.Format(_T("%0.1f C"), (float)dHeatSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"), (float)dHeatSP);
            }
            m_DayHeatEdit.FieldText(strTemp);
            m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
            m_DayHeatStatic.SetWindowText(_T("Heating SP"));
            m_daysetpoint = dSP;
            UpdateData(FALSE);
        }
    }

    if(FlexSPN == 1)//以下是右侧的  右侧 NIGHT
    {

        if (MNIGHT == 1) // office
        {







            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_NIGHT)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_NIGHT)->SetWindowText(strInfo);

            m_pNightTwoSP->SetRange(DayMax*10, DayMin*10);
            BOOL bRetSP = m_pNightTwoSP->SetPos_tstat6_2pos(NightHeatingSP,NightSP,NightCoolingSP);




            HandleSliderSetPos(bRetSP);//tstat6
            m_pNightTwoSP->ShowWindow(SW_NORMAL);
            m_pNightSingleSP->ShowWindow(SW_HIDE);






            m_nightpot = nSP;
            //UpdateData(FALSE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nSP);
            }

            m_nightpotEdit.FieldText(RGB(0,0,0),strTemp);
            m_nightpotEdit.ShowWindow(FALSE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nCoolSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nCoolSP);
            }
            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nHeatSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nHeatSP);
            }

            m_nightHeatInfoEdit.FieldText(strTemp);
        }
        else
        {
            //GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);

            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_NIGHT)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_NIGHT)->SetWindowText(strInfo);
            if (NightHeatingSP<=NightSP&&NightSP<=NightCoolingSP)
            {
            }
            else
            {
                DayMax =5;
                DayMin = 0;
                NightHeatingSP = 1;
                NightSP = 2;
                NightCoolingSP = 3;
            }

            m_pNightSingleSP->SetRange(DayMax*10, DayMin*10);
            BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(NightHeatingSP,NightSP,NightCoolingSP);
            HandleSliderSetPos(bRetSP);//tstat6
            m_pNightSingleSP->ShowWindow(SW_NORMAL);
            m_pNightTwoSP->ShowWindow(SW_HIDE);
            m_nightpot = nSP;
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nSP);
            }

            m_nightpotEdit.FieldText(RGB(0,0,0),strTemp);
            m_nightpotEdit.ShowWindow(TRUE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nCoolSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nCoolSP);
            }

            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nHeatSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nHeatSP);
            }

            m_nightHeatInfoEdit.FieldText(strTemp);
        }


    }
    if (MDAY==1&&MNIGHT==1)
    {
        GetDlgItem(IDC_STATIC_DATSP)->ShowWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_STATIC_DATSP)->ShowWindow(TRUE);
    }
#endif


}

void CT3000View::SetFlexSliderBars_tstat6()
{
    // Initial_Max_Min();


    BOOL Day_Default=FALSE;
    BOOL Night_Default=FALSE;
    CString strTemp;


    float nCoolSP = 0;
    float nHeatSP = 0;
    float nSP =0;
    float nCDB=0;
    float nHDB=0;


    float dCoolSP = 0;
    float dHeatSP = 0;
    float dSP =0;
    float dCDB=0;
    float dHDB=0;


    int DayCoolingSP=0;
    int DaySP=0;
    int DayHeatingSP=0;

    int NightCoolingSP=0;
    int NightSP=0;
    int NightHeatingSP=0;

    CString strInfo;

    dSP= ((float)(short)product_register_value[MODBUS_DAY_SETPOINT])/10;
    dCDB = ((float)(short)product_register_value[MODBUS_DAY_COOLING_DEADBAND])/10;
    dHDB = ((float)(short)product_register_value[MODBUS_DAY_HEATING_DEADBAND])/10;
    dCoolSP=dSP+dCDB;
    dHeatSP=dSP-dHDB;

    DaySP=(short)product_register_value[MODBUS_DAY_SETPOINT];

    product_register_value[MODBUS_DAY_COOLING_SETPOINT]=product_register_value[MODBUS_DAY_SETPOINT]+product_register_value[MODBUS_DAY_COOLING_DEADBAND];
    DayCoolingSP=product_register_value[MODBUS_DAY_COOLING_SETPOINT];

    product_register_value[MODBUS_DAY_HEATING_SETPOINT]=product_register_value[MODBUS_DAY_SETPOINT]-product_register_value[MODBUS_DAY_HEATING_DEADBAND];
    DayHeatingSP=product_register_value[MODBUS_DAY_HEATING_SETPOINT];

    nSP =((float)(short) product_register_value[MODBUS_NIGHT_SETPOINT])/10;
    nCDB=((float)(short) product_register_value[MODBUS_NIGHT_COOLING_DEADBAND])/10;
    nHDB=((float)(short) product_register_value[MODBUS_NIGHT_HEATING_DEADBAND])/10;
    NightSP=(short) product_register_value[MODBUS_NIGHT_SETPOINT];
    nCoolSP=nSP+nCDB;
    nHeatSP=nSP-nHDB;
    product_register_value[MODBUS_NIGHT_COOLING_SETPOINT ]=product_register_value[MODBUS_NIGHT_SETPOINT]+product_register_value[MODBUS_NIGHT_COOLING_DEADBAND];
    NightCoolingSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
    product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=product_register_value[MODBUS_NIGHT_SETPOINT]-product_register_value[MODBUS_NIGHT_HEATING_DEADBAND];
    NightHeatingSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];

    if (!(DayHeatingSP<=DaySP&&DaySP<=DayCoolingSP))
    {
        Day_Default=TRUE;

    }
//         if (DaySP<DayMin*10||DaySP>DayMax*10)
//         {
//             Day_Default=TRUE;
//         }

    if(Day_Default)
    {
        DayMax=50;
        DayMin=0;

        DayHeatingSP=200;
        DaySP=250;
        DayCoolingSP=300;
    }

    if (!(NightHeatingSP<=NightSP&&NightSP<=NightCoolingSP))
    {
        Night_Default=TRUE;
    }
//         if (NightSP<DayMin*10||NightSP>DayMax*10)
//         {
//             Night_Default=TRUE;
//         }

    if(Night_Default)
    {
        DayMax=50;
        DayMin=0;

        NightHeatingSP=200;
        NightSP=250;
        NightCoolingSP=300;

    }


    m_singlesliderday.SetThumbColor(RGB(96,192,0));
    m_singlesliderday.SetSelectionColor(RGB(64, 64, 255));
    m_singlesliderday.SetChannelColor(RGB(196, 196, 255));

    m_singlesliderday.SetRange((short)DayMin*10, (short)DayMax*10,TRUE);

    int currenttemp=(short)DayMin*10+(short)DayMax*10-(int)product_register_value[MODBUS_TEMPRATURE_CHIP];
    m_singlesliderday.SetPos(currenttemp);
#if 1
    if (FlexSP == 1)
    {
        //以下是2 SP
        //	MDAY=1;
        // MDAY=0;

        if (MDAY==1) // office  2SP day
        {


            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_DAY)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_DAY)->SetWindowText(strInfo);
            m_pDayTwoSP->SetRange(DayMax*10,DayMin*10);

            BOOL bRetSP = m_pDayTwoSP->SetPos_tstat6_2pos(DayHeatingSP, DaySP,DayCoolingSP);
            if (bRetSP)
            {
                CString strInfo;
                strInfo.Format(_T("nHeatSP=%d;nSP=%d;nCoolSP=%d"),nCoolSP, nSP,nHeatSP);
                HandleSliderSetPos(bRetSP);//tstat6
            }
            m_pDayTwoSP->ShowWindow(SW_NORMAL);
            m_pDaySingleSP->ShowWindow(SW_HIDE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),((float)((short)product_register_value[MODBUS_DAY_COOLING_SETPOINT])/10.0) );
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),((float)((short)product_register_value[MODBUS_DAY_COOLING_SETPOINT])/10.0) );
            }

            m_DayCoolEdit.FieldText(strTemp);
            //m_DayCoolEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),((float)((short)product_register_value[MODBUS_DAY_HEATING_SETPOINT])/10.0));
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),((float)((short)product_register_value[MODBUS_DAY_HEATING_SETPOINT])/10.0));
            }


            m_DayHeatEdit.FieldText(strTemp);
            //m_DayHeatEdit.SetNumberDefaultTex();
            m_daysetpoint = nSP;
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),((float)((short)product_register_value[MODBUS_DAY_SETPOINT])/10.0));
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),((float)((short)product_register_value[MODBUS_DAY_SETPOINT])/10.0));
            }

            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);
            m_dayInfoEdit.ShowWindow(FALSE);
            UpdateData(FALSE);

            m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
            m_DayHeatStatic.SetWindowText(_T("Heating SP"));

        }
        else  // hotel 1 SP day
        {
            //GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);
            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_DAY)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_DAY)->SetWindowText(strInfo);
            if (DayHeatingSP<DaySP&&DaySP<DayCoolingSP)
            {

            }
            else
            {
                DayMax =5;
                DayMin = 0;
                DayHeatingSP = 1;
                DaySP = 2;
                DayCoolingSP = 3;
            }

            m_pDaySingleSP->SetRange(DayMax*10, DayMin*10);

            BOOL bRetSP = m_pDaySingleSP->SetPos_tstat6_3pos(DayHeatingSP, DaySP,DayCoolingSP);//tstat6

            HandleSliderSetPos(bRetSP);//tstat6






            m_pDaySingleSP->ShowWindow(SW_NORMAL);
            m_pDayTwoSP->ShowWindow(SW_HIDE);



            if (g_unint)
            {
                //strTemp.Format(_T("%d C"), nHeatSP);
                strTemp.Format(_T("%0.1f C"), (float)dSP);
            }
            else
            {

                strTemp.Format(_T("%0.1f F"), (float)dSP);
            }
            m_dayInfoEdit.FieldText(RGB(0,0,0),strTemp);
            m_dayInfoEdit.ShowWindow(TRUE);
            if (g_unint)
            {
                //strTemp.Format(_T("%d C"), nHeatSP);
                strTemp.Format(_T("%0.1f C"), (float)dCoolSP);
            }
            else
            {

                strTemp.Format(_T("%0.1f F"), (float)dCoolSP);
            }
            m_DayCoolEdit.FieldText(strTemp);
            if (g_unint)
            {
                //strTemp.Format(_T("%d C"), nHeatSP);
                strTemp.Format(_T("%0.1f C"), (float)dHeatSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"), (float)dHeatSP);
            }
            m_DayHeatEdit.FieldText(strTemp);
            m_DayCoolStatic.SetWindowText(_T("Cooling SP"));
            m_DayHeatStatic.SetWindowText(_T("Heating SP"));
            m_daysetpoint = dSP;
            UpdateData(FALSE);
        }
    }

    if(FlexSPN == 1)//以下是右侧的  右侧 NIGHT
    {

        if (MNIGHT == 1) // office
        {







            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_NIGHT)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_NIGHT)->SetWindowText(strInfo);

            m_pNightTwoSP->SetRange(DayMax*10, DayMin*10);
            BOOL bRetSP = m_pNightTwoSP->SetPos_tstat6_2pos(NightHeatingSP,NightSP,NightCoolingSP);




            HandleSliderSetPos(bRetSP);//tstat6
            m_pNightTwoSP->ShowWindow(SW_NORMAL);
            m_pNightSingleSP->ShowWindow(SW_HIDE);






            m_nightpot = nSP;
            //UpdateData(FALSE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nSP);
            }

            m_nightpotEdit.FieldText(RGB(0,0,0),strTemp);
            m_nightpotEdit.ShowWindow(FALSE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nCoolSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nCoolSP);
            }
            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nHeatSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nHeatSP);
            }

            m_nightHeatInfoEdit.FieldText(strTemp);
        }
        else
        {
            //GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);

            strInfo.Format(_T("%d"),DayMax);
            GetDlgItem(IDC_STATIC_MAX_NIGHT)->SetWindowText(strInfo);
            strInfo.Format(_T("%d"),DayMin);
            GetDlgItem(IDC_STATIC_MIN_NIGHT)->SetWindowText(strInfo);
            if (NightHeatingSP<NightSP&&NightSP<NightCoolingSP)
            {
            }
            else
            {
                DayMax =5;
                DayMin = 0;
                NightHeatingSP = 1;
                NightSP = 2;
                NightCoolingSP = 3;
            }

            m_pNightSingleSP->SetRange(DayMax*10, DayMin*10);
            BOOL bRetSP = m_pNightSingleSP->SetPos_tstat6_3pos(NightHeatingSP,NightSP,NightCoolingSP);
            HandleSliderSetPos(bRetSP);//tstat6
            m_pNightSingleSP->ShowWindow(SW_NORMAL);
            m_pNightTwoSP->ShowWindow(SW_HIDE);
            m_nightpot = nSP;
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nSP);
            }

            m_nightpotEdit.FieldText(RGB(0,0,0),strTemp);
            m_nightpotEdit.ShowWindow(TRUE);
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nCoolSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nCoolSP);
            }

            m_nightInfoEdit.FieldText(strTemp);
            //m_nightInfoEdit.SetNumberDefaultTex();
            if (g_unint)
            {
                strTemp.Format(_T("%0.1f C"),(float)nHeatSP);
            }
            else
            {
                strTemp.Format(_T("%0.1f F"),(float)nHeatSP);
            }

            m_nightHeatInfoEdit.FieldText(strTemp);
        }


    }
    if (MDAY==1&&MNIGHT==1)
    {
        GetDlgItem(IDC_STATIC_DATSP)->ShowWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_STATIC_DATSP)->ShowWindow(TRUE);
    }
#endif


}

void CT3000View::OnEnKillfocusEditCurSp()
{
// TODO: Add your control notification handler code here
#if 1//0907

    BeginWaitCursor();

    CString str;
    GetDlgItem(IDC_EDIT_CUR_SP)->GetWindowText(str);

    int NightSP = Round_SetPoint_Max((int)(_wtof(str)*10));

    if (product_register_value[MODBUS_NIGHT_SETPOINT]!=NightSP)
    {
        int iret =0;
        iret = write_one(g_tstat_id,MODBUS_NIGHT_SETPOINT,NightSP);//350
        if (iret>0)
        {
            product_register_value[MODBUS_NIGHT_SETPOINT]=NightSP;
        }

        FlexSPN = 1;
        FlexSP = 1;
        InitFlexSliderBars_tstat6();//tsat6
        FlexSPN = 0;
        FlexSP = 0;
    }

    EndWaitCursor();
#endif
}
void CT3000View::OnEnKillfocusDayEdit()
{
#if 1//0907

    BeginWaitCursor();

    CString str;
    GetDlgItem(IDC_DAY_EDIT)->GetWindowText(str);

    int DaySP = Round_SetPoint_Max((int)(_wtof(str)*10));

    if (product_register_value[MODBUS_DAY_SETPOINT]!=DaySP)
    {
        int iret =0;
        iret = write_one(g_tstat_id,MODBUS_DAY_SETPOINT,DaySP);//350
        if (iret>0)
        {
            product_register_value[MODBUS_DAY_SETPOINT]=DaySP;
        }

        FlexSPN = 1;
        FlexSP = 1;
        InitFlexSliderBars_tstat6();//tsat6
        FlexSPN = 0;
        FlexSP = 0;
    }

    EndWaitCursor();
#endif
}
void CT3000View::Initial_Max_Min()
{
    float nCoolSP = 0;
    float nHeatSP = 0;
    float nSP =0;
    float nCDB=0;
    float nHDB=0;


    float dCoolSP = 0;
    float dHeatSP = 0;
    float dSP =0;
    float dCDB=0;
    float dHDB=0;


    int DayCoolingSP=0;
    int DaySP=0;
    int DayHeatingSP=0;

    int NightCoolingSP=0;
    int NightSP=0;
    int NightHeatingSP=0;







    int nCoolDB;
    int nHeatDB;



    int dCoolDB;
    int dHeatDB;

    if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i))
    {
        product_register_value[MODBUS_DAY_SETPOINT]=Round_SetPoint_Max(product_register_value[MODBUS_DAY_SETPOINT]);
        product_register_value[MODBUS_DAY_COOLING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_DAY_COOLING_DEADBAND]);
        product_register_value[MODBUS_DAY_HEATING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_DAY_HEATING_DEADBAND]);

        dSP= ((float)((short)product_register_value[MODBUS_DAY_SETPOINT]))/10;
        dCDB = ((float)((short)product_register_value[MODBUS_DAY_COOLING_DEADBAND]))/10;
        dHDB = ((float)((short)product_register_value[MODBUS_DAY_HEATING_DEADBAND]))/10;
        dCoolSP=dSP+dCDB;
        dHeatSP=dSP-dHDB;

        DaySP=(short)product_register_value[MODBUS_DAY_SETPOINT];

        product_register_value[MODBUS_DAY_COOLING_SETPOINT]=product_register_value[MODBUS_DAY_SETPOINT]+product_register_value[MODBUS_DAY_COOLING_DEADBAND];
        DayCoolingSP=product_register_value[MODBUS_DAY_COOLING_SETPOINT];

        product_register_value[MODBUS_DAY_HEATING_SETPOINT]=product_register_value[MODBUS_DAY_SETPOINT]-product_register_value[MODBUS_DAY_HEATING_DEADBAND];
        DayHeatingSP=product_register_value[MODBUS_DAY_HEATING_SETPOINT];

        product_register_value[MODBUS_NIGHT_SETPOINT]=Round_SetPoint_Max(product_register_value[MODBUS_NIGHT_SETPOINT]);
        product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]);
        product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]=Round_SetPoint_Max(product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]);

        nSP =((float)(short) product_register_value[MODBUS_NIGHT_SETPOINT])/10;
        nCDB=((float)(short) product_register_value[MODBUS_NIGHT_COOLING_DEADBAND])/10;
        nHDB=((float)(short) product_register_value[MODBUS_NIGHT_HEATING_DEADBAND])/10;
        NightSP=(short) product_register_value[MODBUS_NIGHT_SETPOINT];
        nCoolSP=nSP+nCDB;
        nHeatSP=nSP-nHDB;
        product_register_value[MODBUS_NIGHT_COOLING_SETPOINT ]=product_register_value[MODBUS_NIGHT_SETPOINT]+product_register_value[MODBUS_NIGHT_COOLING_DEADBAND];
        NightCoolingSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
        product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=product_register_value[MODBUS_NIGHT_SETPOINT]-product_register_value[MODBUS_NIGHT_HEATING_DEADBAND];
        NightHeatingSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];


        DayMax=0;
        DayMin=0;

        DayMax = GetRoundMM(TRUE,dCoolSP,nCoolSP,1.1);
        DayMin = GetRoundMM(FALSE,dHeatSP,nHeatSP,0.9);

    }
    else
    {
        int m_hotel_or_office=product_register_value[MODBUS_APPLICATION];
        if (product_type==T3000_5ABCDFG_LED_ADDRESS)
        {
            dSP=product_register_value[MODBUS_COOLING_SETPOINT];
            dCoolDB=product_register_value[MODBUS_COOLING_DEADBAND]/10;//119
            dHeatDB=product_register_value[MODBUS_HEATING_DEADBAND]/10;//heat//120
            dCoolSP=dSP+dCoolDB;
            dHeatSP=dSP-dHeatDB;
            if (m_hotel_or_office==0)// office
            {
                /*nSP=0;
                nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
                nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
                nSP = (short)(nCoolSP+nHeatSP)/2 ;
                nCoolDB = nCoolSP - nSP;
                nHeatDB = nSP - nHeatSP;*/


                nSP=0;
                nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
                nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
                nSP = (short)(nCoolSP+nHeatSP)/2 ;
                nCoolDB = nCoolSP - nSP;
                nHeatDB = nSP - nHeatSP;

            }
            else//Hotel
            {
                nSP=dSP;

                nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
                nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));
                nCoolSP = nSP + nCoolDB;
                nHeatSP = nSP - nHeatDB;
            }

        }


        if (product_type==T3000_5EH_LCD_ADDRESS)
        {


            if (m_hotel_or_office==0)// office
            {
                //Setpoint = ( NEW_EEP_CoolingSp + EEP_HeatingSp) / 2
                //cooling_db = NEW_EEP_CoolingSp - Setpoint;
                //heating_db = Setpoint - EEP_HeatingSp;

                dCoolSP=product_register_value[MODBUS_NEW_COOLING_SETPOINT]/10;
                dHeatSP=product_register_value[MODBUS_HEATING_SETPOINT];
                dSP=(dCoolSP+dHeatSP)/2;
                dCoolDB=dCoolSP-dSP;
                dHeatDB=dSP-dHeatSP;

                nSP=0;
                nCoolSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
                nHeatSP=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT];
                nSP = (short)(nCoolSP+nHeatSP)/2 ;
                nCoolDB = nCoolSP - nSP;
                nHeatDB = nSP - nHeatSP;
            }
            else//Hotel
            {
                // Hotel mode:
                //Setpoint = EEP_CoolingSp
                // cooling_db = EEP_CoolDeadband ;
                //heating_db = EEP_HeatDeadband ;



                dSP=product_register_value[MODBUS_COOLING_SETPOINT];

                dCoolDB=product_register_value[MODBUS_COOLING_DEADBAND]/10;//119

                dHeatDB=product_register_value[MODBUS_HEATING_DEADBAND]/10;//heat//120

                dCoolSP=dSP+dCoolDB;
                dHeatSP=dSP-dHeatDB;

                nSP=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];

                nCoolDB = (short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//(max-(itemp-min));
                nHeatDB = (short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//(max-(itemp-min));

                nCoolSP = nSP + nCoolDB;
                nHeatSP = nSP - nHeatDB;
            }

        }

        DayMax=0;
        DayMin=0;
        DayMax = GetRoundMM(TRUE,dCoolSP,nCoolSP,1.1);
        DayMin = GetRoundMM(FALSE,dHeatSP,nHeatSP,0.9);


    }


    /*int currentTemp= product_register_value[MODBUS_TEMPRATURE_CHIP]/10;
    if (currentTemp<DayMin)
    {

    	DayMin=currentTemp;
    	while((DayMin%5)!=0){
    		--DayMin;
    	}
    }

    if (currentTemp>DayMax)
    {

    	DayMax=currentTemp;
    	while((DayMax%5)!=0){
    		++DayMax;
    	}
    }*/

    //DayMax=Round_SetPoint(DayMax);
    //DayMin=Round_SetPoint(DayMin);
}
int CT3000View::GetRoundNumber(int number)
{
	int Increment = product_register_value[MODBUS_SETPOINT_INCREASE];
	while ((number % Increment) != 0)
	{
		++number;
	}
	return number;
}
int CT3000View::GetRoundMM(BOOL is_max,int num1,int num2,float rate)
{
    int retvalue,big_retvalue,small_regvalue;

    if (is_max)
    {
        if (num1>num2)
        {
            retvalue=(int)(num1*rate);
        }
        else
        {
            retvalue=(int)(num2*rate);
        }


    }
    else
    {
        if (num1>num2)
        {
            retvalue=(int)(num2*rate);
        }
        else
        {
            retvalue=(int)(num1*rate);
        }
    }

    big_retvalue= retvalue;
    small_regvalue = retvalue;
    while((big_retvalue%5)!=0)
    {
        ++big_retvalue;
    }
    while((small_regvalue%5)!=0)
    {
        --small_regvalue;
    }
    int gap_big =  big_retvalue - retvalue;
    int gap_small = retvalue - small_regvalue;

//          if (gap_small > gap_big)
//          {
//              retvalue = big_retvalue;
//          }
//          else  if(gap_small < gap_big)
//          {
//              retvalue = small_regvalue;
//          }
//          else   //相等的情况下
//          {
    if (is_max)
    {
        if (num1>num2)
        {
            if (small_regvalue>num1)
            {
                retvalue = small_regvalue;
            }
            else
            {
                retvalue = big_retvalue;
            }
        }
        else
        {
            if (small_regvalue>num2)
            {
                retvalue = small_regvalue;
            }
            else
            {
                retvalue = big_retvalue;
            }
        }

    }
    else
    {
        if (num1>num2)
        {
            if (big_retvalue<num2)
            {
                retvalue = big_retvalue;
            }
            else
            {
                retvalue = small_regvalue;
            }
        }
        else
        {
            if (big_retvalue<num1)
            {
                retvalue = big_retvalue;
            }
            else
            {
                retvalue = small_regvalue;
            }
        }
    }
    /* }*/
    return retvalue;
}
void CT3000View::FreshCtrl()
{
    Initial_Max_Min();
    int  Fresh_Min=(short)product_register_value[MODBUS_MIN_SETPOINT];
    int Fresh_Max=(short)product_register_value[MODBUS_MAX_SETPOINT];
    if(product_register_value[MODBUS_DEGC_OR_F] == 1)	//t5= 121;t6=104
    {
        g_unint = FALSE;
        GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T(" F"));
    }
    else
    {
        g_unint = TRUE;
        GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T(" C"));
    }

    m_fFirmwareVersion=get_curtstat_version();
    m_nSerialNumber=get_serialnumber();
    m_nHardwareVersion=product_register_value[MODBUS_HARDWARE_REV];//8
    m_nID=product_register_value[MODBUS_ADDRESS];
    m_fFirmwareVersion=get_curtstat_version();
    m_nSerialNumber=get_serialnumber();

    int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
    if(nModel==0)
        return;
	m_strModelName = GetProductName(nModel);
	/*switch (nModel)
	{
	case 2:
		m_strModelName=g_strTstat5a;
		break;
	case 1:
		m_strModelName=g_strTstat5b;
		break;
	case 3:
		m_strModelName=g_strTstat5b;
		break;
	case 4:
		m_strModelName=g_strTstat5c;
		break;
	case 12:
		m_strModelName=g_strTstat5d;
		break;
	case PM_NC:
		m_strModelName=g_strnetWork;
		break;
	case PM_TSTATRUNAR:
		m_strModelName=_T("TStatRunar");
	case PM_TSTAT5E:
		m_strModelName=g_strTstat5e;
		break;
	case PM_PM5E:
		m_strModelName=_T("PM5E");
		break;
	case 17:
		m_strModelName=g_strTstat5f;
		break;
	case 18:
		m_strModelName=g_strTstat5g;
		break;
	case 19:
		m_strModelName=g_strTstat5h;
		break;
	case PM_TSTAT5i:
		m_strModelName=_T("Tstat 5i");
		break;
	case PM_TSTAT8:
		m_strModelName=_T("Tstat8");
		break;
	case PM_TSTAT6:
		m_strModelName=g_strTstat6;
		break;
	case PM_TSTAT7:
		m_strModelName=g_strTstat7;
		break;
	case PM_PRESSURE:
		m_strModelName=g_strPressure;
		break;
	case PM_HUMTEMPSENSOR:
		m_strModelName="TstatHUM";
		break;
	default:
		m_strModelName=g_strTstat5a;
		break;
	}*/

    //m_fTemperature=(float)product_register_value[MODBUS_INTERNAL_THERMISTOR]/10;	//101   121MODBUS_TEMPRATURE_CHIP
    m_fTemperature=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]))/10;
    m_iCurFanSpeed=product_register_value[MODBUS_FAN_SPEED];	//137		273
    m_nOverrideTime=product_register_value[MODBUS_OVERRIDE_TIMER];//211		111
    m_nTimeLeft=product_register_value[MODBUS_OVERRIDE_TIMER_LEFT];//212	112
    float tempflot = m_fTemperature;//(float)product_register_value[MODBUS_INTERNAL_THERMISTOR]/10; //101  121

    //float tempflot = (float)newtstat6[121]/10;
    CString  strTemp;
    if (g_unint)
        strTemp.Format(_T("%.1f C"),tempflot);
    else
        strTemp.Format(_T("%.1f F"),tempflot);




    switch(product_register_value[MODBUS_MODE_OPERATION])
    {
    case 0:
        m_nstrPID1=_T("COASTING");
        break;
    case 1:
        m_nstrPID1=_T("COOLING 1");
        break;
    case 2:
        m_nstrPID1=_T("COOLING 2");
        break;
    case 3:
        m_nstrPID1=_T("COOLING 3");
        break;
    case 4:
        m_nstrPID1=_T("HEATING 1");
        break;
    case 5:
        m_nstrPID1=_T("HEATING 2");
        break;
    case 6:
        m_nstrPID1=_T("HEATING 3");
        break;

    case 14:
        m_nstrPID1=_T("COOLING 4");
        break;
    case 15:
        m_nstrPID1=_T("COOLING 5");
        break;
    case 16:
        m_nstrPID1=_T("COOLING 6");
        break;
    case 17:
        m_nstrPID1=_T("HEATING 4");
        break;
    case 18:
        m_nstrPID1=_T("HEATING 5");
        break;
    case 19:
        m_nstrPID1=_T("HEATING 6");
        break;
    default :
        m_nstrPID1=_T("COASTING");
        break;
    }

    switch(product_register_value[MODBUS_PID2_MODE_OPERATION])
    {
    case 0:
        m_nstrPID2=_T("COASTING");
        break;
    case 1:
        m_nstrPID2=_T("COOLING 1");
        break;
    case 2:
        m_nstrPID2=_T("COOLING 2");
        break;
    case 3:
        m_nstrPID2=_T("COOLING 3");
        break;
    case 4:
        m_nstrPID2=_T("HEATING 1");
        break;
    case 5:
        m_nstrPID2=_T("HEATING 2");
        break;
    case 6:
        m_nstrPID2=_T("HEATING 3");
        break;

    case 14:
        m_nstrPID2=_T("COOLING 4");
        break;
    case 15:
        m_nstrPID2=_T("COOLING 5");
        break;
    case 16:
        m_nstrPID2=_T("COOLING 6");
        break;
    case 17:
        m_nstrPID2=_T("HEATING 4");
        break;
    case 18:
        m_nstrPID2=_T("HEATING 5");
        break;
    case 19:
        m_nstrPID2=_T("HEATING 6");
        break;
    default :
        m_nstrPID2=_T("COASTING");
        break;
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
    //switch(product_register_value[128])
    switch(product_register_value[MODBUS_KEYPAD_SELECT])	//128
    {
    case 2:
        m_CoolCtrl.EnableWindow();
        m_HeatCtrl.EnableWindow();
        break;
    case 4:
        m_CoolCtrl.EnableWindow();
        m_HeatCtrl.EnableWindow();
        break;
    case 5:
        m_CoolCtrl.EnableWindow();
        m_HeatCtrl.EnableWindow();
        break;
    default:
        m_CoolCtrl.EnableWindow(false);
        m_HeatCtrl.EnableWindow(false);
        break;
    }
    //if(product_register_value[184] & 1)
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
    //if(product_register_value[350]&0x4)//Free Cool available,yes or on
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
        //if(product_register_value[350]&0x10)//outside air temperature from NC, if the NC has an active OAT then OAT=OK, else OAT=FAIL .
    {
        m_strFreeCoolFN=_T("OK");//shoud also show the oat on the user's display
    }
    else
    {
        m_strFreeCoolFN=_T("FAIL"); //show a grey box for the OAT
    }
    if(product_register_value[MODBUS_FREE_COOL_CONFIG]&0x2)
    {
    }
    else
    {
        m_strFreeCoolFN=NO_APPLICATION;
    }

    //////////////////////////////////////////////////////////////////////////
    // just for tstat6
    if((product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT6)||(product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT7)||(product_register_value[MODBUS_PRODUCT_MODEL] == PM_TSTAT5i))
    {
        // humidity
        BOOL m_disable_hum;
        if((product_register_value[20]&2)==2)
        {
            m_disable_hum=TRUE;
        }
        else
        {
            m_disable_hum=FALSE;
        }
        if (m_disable_hum)
        {
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

    }
    else
    {
        GetDlgItem(IDC_EDIT_HUMIDITY)->SetWindowText(_T("N/A"));
    }
    UpdateData(FALSE);
    //time
    CTime time = CTime::GetCurrentTime();
    CString strtime = time.Format(_T("%I:%M:%S %p"));
    CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC_TIME);
    pEdit->SetWindowText(strtime);
    CString strDate = time.Format(_T("%A, %b %d, %Y"));
    pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RTC);
    pEdit->SetWindowText(strDate);

    m_TempInfoEdit.FieldText(strTemp);



    if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8))
    {
        Flexflash=1;

        if (Flexflash)//只有点击树型列表时才执行下面的。
        {

            //GetDlgItem(IDC_DAYSTATIC)->ShowWindow(SW_HIDE);
           GetDlgItem(IDC_OCCUPACHECK)->ShowWindow(SW_SHOW);
           GetDlgItem(IDC_UNOCCUPIED_MARK)->ShowWindow(SW_SHOW);


            GetDlgItem(IDC_STATIC2SP)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_STATIC1SP)->ShowWindow(SW_SHOW);


            //GetDlgItem(IDC_SLIDER_DAY)->ShowWindow(SW_SHOW);

            //	GetDlgItem(IDC_SLIDER_NIGHT)->ShowWindow(SW_SHOW);

            //max 365 min 366
            if ((short)Fresh_Min>(short)Fresh_Max)
            {
                Fresh_Min=0;
                Fresh_Max=50;
            }
            //m_singlesliderday.SetRange((short)Fresh_Min,(short) Fresh_Max,TRUE);




            if ((Fresh_Max <=0)||(Fresh_Max >3000))
            {
                Fresh_Max = 100;
            }




            //int currenttemp=Fresh_Min+Fresh_Max-(int)product_register_value[MODBUS_DAY_SETPOINT]/10;
            //MODBUS_TEMPRATURE_CHIP
            int currenttemp=(short)Fresh_Min+(short)Fresh_Max-(int)product_register_value[MODBUS_INTERNAL_THERMISTOR]/10;
            //m_singlesliderday.SetPos(currenttemp);



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
            }
            else if (tempd1>product_register_value[365])
            {
                tempd1 = product_register_value[365];
            }


            if ((short)Fresh_Min>(short)Fresh_Max)
            {
                Fresh_Min=0;
                Fresh_Max=50;
            }
            //m_singlesliderday.SetRange((short)Fresh_Min, (short)Fresh_Max,TRUE);




            int currenttemp=(short)Fresh_Min+(short)Fresh_Max-(int)product_register_value[MODBUS_TEMPRATURE_CHIP]/10;
            //m_singlesliderday.SetPos(currenttemp);






            FlexSP =0;
            FlexSPN =0;

        }
        else
        {
            InitFlexSliderBars_tstat6();
        }
        return;
    }
    else
    {
        if (Flexflash)
        {


            // GetDlgItem(IDC_DAYSTATIC)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_OCCUPACHECK)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_UNOCCUPIED_MARK)->ShowWindow(SW_SHOW);


            GetDlgItem(IDC_STATIC2SP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC1SP)->ShowWindow(SW_HIDE);

            //GetDlgItem(IDC_DAY_EDIT)->EnableWindow(FALSE);
            //GetDlgItem(IDC_EDIT_CUR_SP)->EnableWindow(FALSE);

            if (Fresh_Min>Fresh_Max)
            {
                Fresh_Min=0;
                Fresh_Max=50;
            }
#if 1
//            m_singlesliderday.SetRange(Fresh_Min, Fresh_Max,TRUE);




            if ((Fresh_Max <=0)||(Fresh_Max >3000))
            {
                Fresh_Max = 100;
            }



            if (Fresh_Min>Fresh_Max)
            {
                Fresh_Min=0;
                Fresh_Max=50;
            }
            //m_singlesliderday.SetRange(Fresh_Min, Fresh_Max,TRUE);


            if ((Fresh_Min<=0)||(Fresh_Min>3000))
            {
                Fresh_Min = 10;
            }
            if ((Fresh_Max>=100)||(Fresh_Max<10))
            {
                Fresh_Max = 100;
            }


            int tempd4 = Fresh_Min+Fresh_Max
                         -(int)product_register_value[MODBUS_COOLING_SETPOINT];
            if (tempd4>Fresh_Max)
            {
                tempd4 = Fresh_Max;
            }
            else if (tempd4<Fresh_Min)
            {
                tempd4 = Fresh_Min;
            }
            //  m_singlesliderday.SetPos(tempd4);


            tempd4 = Fresh_Min+Fresh_Max
                     -(int)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
            if (tempd4<Fresh_Max)
            {
                tempd4 = Fresh_Max;
            }
            else if (tempd4>Fresh_Min)
            {
                tempd4 = Fresh_Min;
            }


#endif
            int currenttemp=Fresh_Min+Fresh_Max-(int)product_register_value[MODBUS_TEMPRATURE_CHIP]/10;
            //m_singlesliderday.SetPos(currenttemp);

            Flexflash = FALSE;
        }
        if (Fresh_Min>Fresh_Max)
        {
            Fresh_Min=0;
            Fresh_Max=50;
        }
#if 1
        //m_singlesliderday.SetRange(Fresh_Min, Fresh_Max,TRUE);


        if ((Fresh_Min<=0)||(Fresh_Min>3000))
        {
            Fresh_Min = 10;
        }
        if ((Fresh_Max>=100)||(Fresh_Max<10))
        {
            Fresh_Max = 100;
        }


        int tempd4 = Fresh_Min+Fresh_Max
                     -(int)product_register_value[MODBUS_COOLING_SETPOINT];
        if (tempd4>Fresh_Max)
        {
            tempd4 = Fresh_Max;
        }
        else if (tempd4<Fresh_Min)
        {
            tempd4 = Fresh_Min;
        }
        //m_singlesliderday.SetPos(tempd4);


        tempd4 = Fresh_Min+Fresh_Max
                 -(int)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT];
        if (tempd4<Fresh_Max)
        {
            tempd4 = Fresh_Max;
        }
        else if (tempd4>Fresh_Min)
        {
            tempd4 = Fresh_Min;
        }

        //  m_singleslidernight.SetPos(tempd4);
#endif

        int currenttemp=Fresh_Min+Fresh_Max-(int)product_register_value[MODBUS_TEMPRATURE_CHIP]/10;
        //m_singlesliderday.SetPos(currenttemp);


        InitSliderBars2();
    }
    UpdateData(FALSE);
}

void CT3000View::OnCbnSelchangeStaticunint()
{
    int sel=m_gUnit.GetCurSel();
    if (sel!=product_register_value[MODBUS_DEGC_OR_F])
    {
        write_one(g_tstat_id,MODBUS_DEGC_OR_F,sel);
        product_register_value[MODBUS_DEGC_OR_F]=sel;

        if(product_register_value[MODBUS_DEGC_OR_F] == 1)	//t5= 121;t6=104
        {
            g_unint = FALSE;
            GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T(" F"));
        }
        else
        {
            g_unint = TRUE;
            GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T(" C"));
        }
        Fresh_In();
    }



}
void CT3000View::Read_SliderData()
{

    if (product_register_value[7]==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8)||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7)
    {
        Read_Multi(g_tstat_id,&product_register_value[MODBUS_DAY_SETPOINT],MODBUS_DAY_SETPOINT,20);
    }
    else
    {
        Read_Multi(g_tstat_id,&product_register_value[MODBUS_COOLING_DEADBAND],MODBUS_COOLING_DEADBAND,2);
        Read_Multi(g_tstat_id,&product_register_value[MODBUS_NIGHT_HEATING_DEADBAND],MODBUS_NIGHT_HEATING_DEADBAND,2);

        Read_Multi(g_tstat_id,&product_register_value[MODBUS_COOLING_SETPOINT],MODBUS_COOLING_SETPOINT,2);

        Read_Multi(g_tstat_id,&product_register_value[MODBUS_NIGHT_HEATING_SETPOINT],MODBUS_NIGHT_HEATING_SETPOINT,2);
    }

}






BOOL CT3000View::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // TODO: Add your message handler code here and/or call default
    /* T3000_help.chm::/T3000_Help/How%20to%20use%20T3000/How%20to%20use%20T3000,basicly.htm*/
    CString filename=g_strExePth+_T("T3000_help.chm::/T3000_Help/How to use T3000/How to use T3000,basicly.htm");
    ::HtmlHelp(
        NULL,
        filename.GetBuffer(),
        HH_DISPLAY_TOPIC,
        NULL);
    return CFormView::OnHelpInfo(pHelpInfo);
}


void CT3000View::OnBnClickedHelp()
{

}


void CT3000View::OnBnClickedHelpHelp()
{

}

LRESULT  CT3000View::ResumeMessageCallBackForT3000View(WPARAM wParam, LPARAM lParam)
{
    UINT temp_id;
    _MessageWriteOneInfo *Write_Struct_feedback =(_MessageWriteOneInfo *)lParam;
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    vector <int>::iterator Iter;
    if(msg_result)
    {

        CString temp;
        temp.Format(_T("Change \"%s\" value from %d to %d success!"),
                    Write_Struct_feedback->Changed_Name,
                    Write_Struct_feedback->old_value,
                    Write_Struct_feedback->new_value);
        temp_id = Write_Struct_feedback->CTRL_ID;
        SetPaneString(1,temp);

        product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->new_value;
        if(Write_Struct_feedback!=NULL)
            delete Write_Struct_feedback;

        //Read_SliderData();
        //InitFlexSliderBars_tstat6();
        FreshCtrl();
        m_active_key_mouse=FALSE;

    }
    else
    {
        CString temp;
        temp.Format(_T("Change \"%s\" value from %d to %d Fail!"),
                    Write_Struct_feedback->Changed_Name,
                    Write_Struct_feedback->old_value,
                    Write_Struct_feedback->new_value);

        temp_id = Write_Struct_feedback->CTRL_ID;
        SetPaneString(1,temp);
        Beep(10,100);
        product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->old_value;
        //GetDlgItem(Write_Struct_feedback->CTRL_ID)->SetWindowTextW(_T(""));

        int indexid = -1;
        for (int i=0; i<(int)Change_Color_ID.size(); i++)
        {
            if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
                continue;
            else
                indexid = i;
        }
        Iter = Change_Color_ID.begin()+indexid;
        if(indexid!=-1)
        {
            Iter = Change_Color_ID.begin()+indexid;
            Change_Color_ID.erase(Iter);
        }

        if(Write_Struct_feedback!=NULL)
        {
            delete Write_Struct_feedback;
        }

        FreshCtrl();


    }

    //GetDlgItem(temp_id)->Invalidate();

    return 0;
}


LRESULT  CT3000View::ReadDataCallBack(WPARAM wParam, LPARAM lParam)
{
    _MessageReadOneInfo *Read_Struct_feedback =(_MessageReadOneInfo *)lParam;
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    if(msg_result)
    {
        product_register_value[Read_Struct_feedback->address]=Read_Struct_feedback->new_value;
    }
    if(Read_Struct_feedback!=NULL)
        delete Read_Struct_feedback;
    return 1;
}

BOOL CT3000View::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            //  这里就是你要显示的Tooltips，可以根据需要来定制
            CString strToolTips;
            // strToolTips.Format(_T("%d"), m_tipvalue);



            pTTT->lpszText=strToolTips.AllocSysString();

            pTTT->hinst = NULL;
            return(TRUE);
        }
    }

    return(FALSE);
}
