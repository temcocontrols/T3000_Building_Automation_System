// BacnetGraphicSetting.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetGraphicSetting.h"
#include "afxdialogex.h"

extern unsigned int customer_define_x_time;
extern unsigned long customer_start_time;
extern unsigned long customer_end_time;
extern bool use_customer_time;
// CBacnetGraphicSetting dialog

IMPLEMENT_DYNAMIC(CBacnetGraphicSetting, CDialogEx)

CBacnetGraphicSetting::CBacnetGraphicSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetGraphicSetting::IDD, pParent)
{

}

CBacnetGraphicSetting::~CBacnetGraphicSetting()
{
}

void CBacnetGraphicSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_GRP_START_DAY, m_grp_start_day);
	DDX_Control(pDX, IDC_DATETIMEPICKER_GRP_START_TIME, m_grp_start_time);
	DDX_Control(pDX, IDC_DATETIMEPICKER_GRP_END_DAY2, m_grp_end_day);
	DDX_Control(pDX, IDC_DATETIMEPICKER_GRP_END_TIME2, m_grp_end_time);
}


BEGIN_MESSAGE_MAP(CBacnetGraphicSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBacnetGraphicSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBacnetGraphicSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBacnetGraphicSetting message handlers


BOOL CBacnetGraphicSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetGraphicSetting::OnBnClickedOk()
{
	
	CTime temp_start_day;
	CTime temp_start_time;
	m_grp_start_day.GetTime(temp_start_day);
	m_grp_start_time.GetTime(temp_start_time);
	int start_year = temp_start_day.GetYear();
	int start_month = temp_start_day.GetMonth();
	int start_day = temp_start_day.GetDay();
	int start_hour = temp_start_time.GetHour();
	int start_minute = temp_start_time.GetMinute();
	int start_sec = temp_start_time.GetSecond();

	 CTime temp_start(start_year,start_month,start_day,start_hour,start_minute,start_sec);
	 unsigned long start_long_time = temp_start.GetTime();


	 CTime temp_end_day;
	 CTime temp_end_time;
	 m_grp_end_day.GetTime(temp_end_day);
	 m_grp_end_time.GetTime(temp_end_time);
	 int end_year = temp_end_day.GetYear();
	 int end_month = temp_end_day.GetMonth();
	 int end_day = temp_end_day.GetDay();
	 int end_hour = temp_end_time.GetHour();
	 int end_minute = temp_end_time.GetMinute();
	 int end_sec = temp_end_time.GetSecond();

	 CTime temp_end(end_year,end_month,end_day,end_hour,end_minute,end_sec);
	 unsigned long end_long_time = temp_end.GetTime();

	 customer_start_time = start_long_time;
	 customer_end_time = end_long_time;
	 customer_define_x_time = customer_end_time - customer_start_time;
	 use_customer_time = true;
	CDialogEx::OnOK();
}


void CBacnetGraphicSetting::OnBnClickedCancel()
{
	
	 use_customer_time = false;
	CDialogEx::OnCancel();
}
