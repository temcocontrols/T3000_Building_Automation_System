// WeeklySchedule.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WeeklySchedule.h"


// CWeeklySchedule dialog

IMPLEMENT_DYNAMIC(CWeeklySchedule, CDialog)

CWeeklySchedule::CWeeklySchedule(CWnd* pParent /*=NULL*/)
	: CDialog(CWeeklySchedule::IDD, pParent)
{

}

CWeeklySchedule::~CWeeklySchedule()
{
}

void CWeeklySchedule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWeeklySchedule, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CWeeklySchedule::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CWeeklySchedule::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_MF, &CWeeklySchedule::OnBnClickedButtonMf)
	ON_STN_CLICKED(IDC_STATIC11, &CWeeklySchedule::OnStnClickedStatic11)
END_MESSAGE_MAP()


// CWeeklySchedule message handlers





BOOL CWeeklySchedule::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rcMe;
	GetClientRect(&rcMe);
	rcMe.DeflateRect(20,20);



	BOOL Create(CWnd*pMum,CRect rcBd,BOOL bHorz,int iID);
	//Height or Width of the slider is 27 pixels always
	//rcMe.DeflateRect(27,40);//前一参数，数值越大，控件越短；后一参数，数值越大，离顶部越远。

	rcMe.DeflateRect(130,20);
	rcMe.DeflateRect(0,0,4,0);

	// 	//create horz 2 sliders
	rc=rcMe;


	rc.left = rc.left-62;
	rc.top = rc.top-3;

	rc.left = rc.left-2;

	slider_Mon.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);



	rc.top = rc.top-3+32;
	slider_Tue.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);

	rc.top = rc.top+27+3;
	slider_Wed.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);

	rc.top = rc.top+28;
	//	rc.left = rc.left-2;

	slider_Thu.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);

	rc.top = rc.top+29;
	slider_Fri.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);

	//rc.left = rc.left-1;
	rc.top = rc.top+29;

	slider_Sat.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);

	rc.top = rc.top+29;
	slider_Sun.Create(this,rc,TRUE,IDB_BITMAP_SCHEDULE);



	//set range and tick
	slider_Mon.SetRange(0,24,1); //20表示显示的刻度线条数之一代表的值
	slider_Mon.SetInstane(0,9);

	slider_Tue.SetRange(0,24,1);
	slider_Tue.SetInstane(2,12);

	slider_Wed.SetRange(0,24,1);
	slider_Wed.SetInstane(12,20);

	slider_Thu.SetRange(0,24,1);
	slider_Thu.SetInstane(6,18);

	slider_Fri.SetRange(0,24,1);
	slider_Fri.SetInstane(12,24);

	slider_Sat.SetRange(0,24,1);
	slider_Sat.SetInstane(8,22);

	slider_Sun.SetRange(0,24,1);
	slider_Sun.SetInstane(12,18);



	m_apply.AutoLoad(IDC_BUTTON_APPLY,this);
	m_clear.AutoLoad(IDC_BUTTON_CLEAR,this);
	m_mf.AutoLoad(IDC_BUTTON_MF,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CWeeklySchedule::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
}

void CWeeklySchedule::OnBnClickedButtonClear()
{
	
	return CDialog::OnOK();
}

void CWeeklySchedule::OnBnClickedButtonMf()
{
	// TODO: Add your control notification handler code here
}

void CWeeklySchedule::OnStnClickedStatic11()
{
	// TODO: Add your control notification handler code here
}
