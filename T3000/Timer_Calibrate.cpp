// Timer_Calibrate.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "Timer_Calibrate.h"
#include "global_function.h"
//#include ".\timer_calibrate.h"


// Timer_Calibrate 对话框
#define SETTIMER    SetTimer(1,10000,NULL)
#define KILLTIMER 	KillTimer(1)
	
IMPLEMENT_DYNAMIC(Timer_Calibrate, CDialog)
Timer_Calibrate::Timer_Calibrate(CWnd* pParent /*=NULL*/)
	: CDialog(Timer_Calibrate::IDD, pParent)
	, m_machine_time(_T(""))
	, m_date_time(COleDateTime::GetCurrentTime())
	, m_time_time(COleDateTime::GetCurrentTime())
	, m_time_zone(_T(""))
	, m_time_zone_str(_T(""))
{

}

Timer_Calibrate::~Timer_Calibrate()
{
}

void Timer_Calibrate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT8, m_machine_time);
	DDX_MonthCalCtrl(pDX, IDC_DATETIMEPICKER2, m_date_time);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time_time);
	DDX_Text(pDX, IDC_EDIT1, m_time_zone);
	DDX_Text(pDX, IDC_EDIT9, m_time_zone_str);
	DDX_Control(pDX, IDC_COMBO1, m_building_time_zone);
	DDX_MonthCalCtrl(pDX, IDC_DATETIMEPICKER4, m_NCDateCtrl);
	DDX_MonthCalCtrl(pDX, IDC_DATETIMEPICKER5, m_NCTimeCtrl);
	DDX_Control(pDX, IDC_EDIT8, m_DatetimeEdit);
}


BEGIN_MESSAGE_MAP(Timer_Calibrate, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATETIMEPICKER1, OnNMSetfocusDatetimepicker1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER1, OnNMKillfocusDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDCANCEL, &Timer_Calibrate::OnBnClickedCancel)
END_MESSAGE_MAP()


// Timer_Calibrate 消息处理程序

BOOL Timer_Calibrate::OnInitDialog()
{
	CDialog::OnInitDialog();	
	m_DatetimeEdit.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC101)->ShowWindow(SW_HIDE);

	
	had_select=false;
	SETTIMER;
	//SetTimer(1,30000,NULL);
	CString strtemp;
	for(int i=-12;i<=13;i++)
	{
		if(i>=0)
			strtemp.Format(_T("(GMT+%d)"),i);
		else
            strtemp.Format(_T("(GMT%d)"),i);
		m_building_time_zone.AddString(strtemp);
	}

	
	UpdateData(true);
	Read_Multi(g_tstat_id,machine_time,200,8,3);
	//unsigned char local_time[8];
//	memcpy(&local_time[0], &multi_register_value[200],8);

	//	memcpy(&machine_time[0], &multi_register_value[200],8*sizeof(unsigned short));

	if(machine_time[1]<10)
		m_machine_time.Format(_T("   %d/%d/%d0%d %d:%d:%d"),machine_time[2],machine_time[4],machine_time[0],machine_time[1],machine_time[5],machine_time[6],machine_time[7]);
	else
		m_machine_time.Format(_T("   %d/%d/%d%d %d:%d:%d"),machine_time[2],machine_time[4],machine_time[0],machine_time[1],machine_time[5],machine_time[6],machine_time[7]);
	if(machine_time[5]>12)
		m_machine_time+=_T(" PM");
	else
		m_machine_time+=_T(" AM");

	m_NCDateCtrl.SetDate(machine_time[0]*100+machine_time[1],machine_time[2],machine_time[4]);
	m_NCTimeCtrl.SetTime(machine_time[5],machine_time[6],machine_time[7]);

	if(had_select==false)
		m_time_time=m_time_time.GetCurrentTime();	
	TIME_ZONE_INFORMATION temp;
	GetTimeZoneInformation(&temp);
	m_time_zone_str=temp.StandardName;
	int i_temp=read_one(g_tstat_id,11);
	if(temp.Bias>0)
	{
		m_time_zone=_T("-");
		CString t;
		t.Format(_T("%d"),temp.Bias/60);
		m_time_zone=m_time_zone+t;
		if(i_temp==255)
		{
			m_building_time_zone.SetCurSel(12-temp.Bias/60);
			write_one(g_tstat_id,11,short(12-temp.Bias/60));
		}
		else if(i_temp>=0)
			m_building_time_zone.SetCurSel(i_temp);
	}
	else
	{
		m_time_zone=_T("+");
		CString t;
		t.Format(_T("%d"),temp.Bias/-60);
		m_time_zone=m_time_zone+t;
		if(i_temp==255)
		{
			m_building_time_zone.SetCurSel(12-temp.Bias/60);
			write_one(g_tstat_id,11,short(12-temp.Bias/60));
		}
		else if(i_temp>=0)
			m_building_time_zone.SetCurSel(i_temp);
	}
	UpdateData(false);
	SETTIMER;	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void Timer_Calibrate::OnDestroy()
{
	CDialog::OnDestroy();
	KILLTIMER;
//	write_one(THE_ID,16,0xa2,2);////////////////////////////////////////////////first step
	
}

void Timer_Calibrate::OnTimer(UINT nIDEvent)
{
	
		UpdateData(true);
		Read_Multi(g_tstat_id,machine_time,200,8,3);
		if(machine_time[1]<10)
			m_machine_time.Format(_T("   %d/%d/%d0%d %d:%d:%d"),machine_time[2],machine_time[4],machine_time[0],machine_time[1],machine_time[5],machine_time[6],machine_time[7]);
		else
			m_machine_time.Format(_T("   %d/%d/%d%d %d:%d:%d"),machine_time[2],machine_time[4],machine_time[0],machine_time[1],machine_time[5],machine_time[6],machine_time[7]);
		if(machine_time[5]>12)
			m_machine_time+=_T(" PM");
		else
			m_machine_time+=_T(" AM");
		if(had_select==false)
		
	m_NCDateCtrl.SetDate(machine_time[0]*100+machine_time[1],machine_time[2],machine_time[4]);
	m_NCTimeCtrl.SetTime(machine_time[5],machine_time[6],machine_time[7]);

		m_time_time=m_time_time.GetCurrentTime();	
		
		TIME_ZONE_INFORMATION temp;
		GetTimeZoneInformation(&temp);
		m_time_zone_str=temp.StandardName;
		int i_temp=read_one(g_tstat_id,11);
		if(temp.Bias>0)
		{
			m_time_zone=_T("-");
			CString t;
			t.Format(_T("%d"),temp.Bias/60);
			m_time_zone=m_time_zone+t;
			if(i_temp==255)
			{
				m_building_time_zone.SetCurSel(12-temp.Bias/60);
				write_one(g_tstat_id,11,short(12-temp.Bias/60));
			}
			else if(i_temp>=0)
				m_building_time_zone.SetCurSel(i_temp);
		}
		else
		{
			m_time_zone=_T("+");
			CString t;
			t.Format(_T("%d"),temp.Bias/-60);
			m_time_zone=m_time_zone+t;
			if(i_temp==255)
			{
				m_building_time_zone.SetCurSel(12-temp.Bias/60);
				write_one(g_tstat_id,11,(short)(12-temp.Bias/60));
			}
			else if(i_temp>=0)
				m_building_time_zone.SetCurSel(i_temp);
		}
		UpdateData(false);
		

	/*
	m_time_time=m_time_time.GetCurrentTime();	
	UpdateData(false);
	*/

	CDialog::OnTimer(nIDEvent);
}

void Timer_Calibrate::OnBnClickedOk()
{
	
	//OnOK();
	KILLTIMER;
	had_select=false;
	UpdateData(true);
	unsigned char t_time[8]={0};
	unsigned short temp_us=0;
	temp_us=m_date_time.GetYear();
	t_time[0]=temp_us/100;
	t_time[1]=temp_us%100;
	t_time[2]=m_date_time.GetMonth();
	t_time[3]=m_date_time.GetDayOfWeek();
// 	if(t_time[3]==1)
// 		t_time[3]=6;
// 	else 
// 		t_time[3]-=2;
		if(t_time[3]==1)
			t_time[3]=7;
		else 
			t_time[3]-=1;

	t_time[4]=m_date_time.GetDay();
	t_time[5]=m_time_time.GetHour();
	t_time[6]=m_time_time.GetMinute();
	t_time[7]=m_time_time.GetSecond();
	Write_Multi(g_tstat_id,t_time,200,8);
	int t=_wtoi(m_time_zone.Right(m_time_zone.GetLength()-1));
	if(m_time_zone.Left(1)=="+")
		t+=12;
	else
		t=12-t;
	write_one(g_tstat_id,11,t);

	Read_Multi(g_tstat_id,machine_time,200,8,3);
	if(machine_time[1]<10)
		m_machine_time.Format(_T("   %d/%d/%d0%d %d:%d:%d"),machine_time[2],machine_time[4],machine_time[0],machine_time[1],machine_time[5],machine_time[6],machine_time[7]);
	else
		m_machine_time.Format(_T("   %d/%d/%d%d %d:%d:%d"),machine_time[2],machine_time[4],machine_time[0],machine_time[1],machine_time[5],machine_time[6],machine_time[7]);
	if(machine_time[5]>12)
		m_machine_time+=_T(" PM");
	else
		m_machine_time+=_T(" AM");

	m_NCDateCtrl.SetDate(machine_time[0]*100+machine_time[1],machine_time[2],machine_time[4]);
	m_NCTimeCtrl.SetTime(machine_time[5],machine_time[6],machine_time[7]);

	if(had_select==false)
		m_time_time=m_time_time.GetCurrentTime();	
	TIME_ZONE_INFORMATION temp;
	GetTimeZoneInformation(&temp);
	m_time_zone_str=temp.StandardName;
	int i_temp=read_one(g_tstat_id,11);
	if(temp.Bias>0)
	{
		m_time_zone=_T("-");
		CString t;
		t.Format(_T("%d"),temp.Bias/60);
		m_time_zone=m_time_zone+t;
		if(i_temp==255)
		{
			m_building_time_zone.SetCurSel(12-temp.Bias/60);
			write_one(g_tstat_id,11,(short)(12-temp.Bias/60));
		}
		else if(i_temp>=0)
			m_building_time_zone.SetCurSel(i_temp);
	}
	else
	{
		m_time_zone=_T("+");
		CString t;
		t.Format(_T("%d"),temp.Bias/-60);
		m_time_zone=m_time_zone+t;
		if(i_temp==255)
		{
			m_building_time_zone.SetCurSel(12-temp.Bias/60);
			write_one(g_tstat_id,11,(short)(12-temp.Bias/60));
		}
		else if(i_temp>=0)
			m_building_time_zone.SetCurSel(i_temp);
	}
	
	UpdateData(FALSE);
	SETTIMER;	
}

void Timer_Calibrate::OnNMSetfocusDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	KILLTIMER;
	had_select=true;
	*pResult = 0;
}

void Timer_Calibrate::OnNMKillfocusDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	SETTIMER;	
	*pResult = 0;
}

void Timer_Calibrate::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	*pResult = 0;
}

void Timer_Calibrate::OnBnClickedCancel()
{
	
	OnCancel();
}
