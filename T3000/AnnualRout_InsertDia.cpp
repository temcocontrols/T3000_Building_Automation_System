
// AnnualRout_InsertDia.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "AnnualRout_InsertDia.h"
#include "Annual_Routines.h"
#include "schedule.h"
#include "global_function.h"
#include "Schedule_grid.h"
#include "../MultipleMonthCal32/multiplemonthcal.h"

// AnnualRout_InsertDia 对话框
#define TO_CLEAR_MONTH_CTRL _T("clear")
IMPLEMENT_DYNAMIC(AnnualRout_InsertDia, CDialog)
AnnualRout_InsertDia::AnnualRout_InsertDia(unsigned char row,CString strtype,CWnd* pParent /*=NULL*/)
	: CDialog(AnnualRout_InsertDia::IDD, pParent)
	, m_date_time(COleDateTime::GetCurrentTime())
{
		m_addr=row;
		SYSTEMTIME systime;
    	::GetSystemTime(&systime);
	    CTime time(systime);
	   if(time.GetYear()%4==0)
		  is_leap=true;//leap year
	  else 
		 is_leap=false;//no leap year
		 	m_bXpOS=TRUE;
	m_strtype = strtype;
}

AnnualRout_InsertDia::AnnualRout_InsertDia(CWnd* pParent /*=NULL*/)
	: CDialog(AnnualRout_InsertDia::IDD, pParent)
{
	m_configfile_path = g_strExePth + g_strStartInterface_config;
	 
}


AnnualRout_InsertDia::~AnnualRout_InsertDia()
{
}

void AnnualRout_InsertDia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_ctrl);
	DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR1, m_date_time);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_month_ctrl);
	//DDX_Control(pDX, IDC_MONTHVIEW1, m_monthViewCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_yearComBox);
	m_month_ctrl.SetOriginalColors();
}


BEGIN_MESSAGE_MAP(AnnualRout_InsertDia, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_RBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_COMMAND(ID_ANNUALROUT_ADD, OnAnnualroutAdd)
	ON_COMMAND(ID_ANNUALROUT_CLEAR, OnAnnualroutClear)
	ON_BN_CLICKED(IDOK, &AnnualRout_InsertDia::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &AnnualRout_InsertDia::OnCbnSelchangeCombo1)

	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR1, &AnnualRout_InsertDia::OnMcnSelectBacMonthcalendar)
	ON_MESSAGE(WM_REFRESH_BAC_DAY_CAL,&AnnualRout_InsertDia::Fresh_Schedule_Day_Cal)	
	ON_MESSAGE(MY_RESUME_DATA, &AnnualRout_InsertDia::DayResumeMessageCallBack)

END_MESSAGE_MAP()

// AnnualRout_InsertDia 消息处理程序
void AnnualRout_InsertDia::leap_year()//if the year is leap year,get the holiday by this function
{
	CString str;
	int i=0;
	for(i=0;i<366;i++)
	{
		if(i<31)
		{//january 31
			str.Format(_T("%d"),i+1);
			str=_T("1-")+str;
		}
		else if(i<60)
		{//february 29
			str.Format(_T("%d"),i-30);
			str=_T("2-")+str;
		}
		else if(i<91)
		{//march 31
			str.Format(_T("%d"),i-59);
			str=_T("3-")+str;
		}
		else if(i<121)
		{//april 30
			str.Format(_T("%d"),i-90);
			str=_T("4-")+str;
		}
		else if(i<152)
		{//may 31 
			str.Format(_T("%d"),i-120);
			str=_T("5-")+str;
		}
		else if(i<182)
		{//june 30
			str.Format(_T("%d"),i-151);
			str=_T("6-")+str;
		}
		else if(i<213)
		{//july 31
			str.Format(_T("%d"),i-181);
			str=_T("7-")+str;
		}
		else if(i<244)
		{//auguest 31 
			str.Format(_T("%d"),i-212);
			str=_T("8-")+str;
		}
		else if(i<274)
		{//september 30
			str.Format(_T("%d"),i-243);
			str=_T("9-")+str;
		}
		else if(i<305)
		{//october 31
			str.Format(_T("%d"),i-273);
			str=_T("10-")+str;
		}
		else if(i<335)
		{//nobember 30
			str.Format(_T("%d"),i-304);
			str=_T("11-")+str;
		}
		else if(i<366)
		{//december 31
			str.Format(_T("%d"),i-334);
			str=_T("12-")+str;
		}
		int l=1,m;
		m=i%8;//bit day
		for(int j=0;j<m;j++)
			l*=2;
		if(the_days[i/8] & l)/////////////////////
			set_day_state(str);
	}
}
void AnnualRout_InsertDia::no_leap_year()//if the year is not a leap year,get the holiday by this function
{

	CString str;
	int i=0;
	for(i=0;i<365;i++)
	{

		if(i<31)
		{//january 31
			str.Format(_T("%d"),i+1);
			str=_T("1-")+str;
		}
		else if(i<59)
		{//february 28
			str.Format(_T("%d"),i-30);
			str=_T("2-")+str;
		}
		else if(i<90)
		{//march 31
			str.Format(_T("%d"),i-58);
			str=_T("3-")+str;
		}
		else if(i<120)
		{//april 30
			str.Format(_T("%d"),i-89);
			str=_T("4-")+str;
		}
		else if(i<151)
		{//may 31 
			str.Format(_T("%d"),i-119);
			str=_T("5-")+str;
		}
		else if(i<181)
		{//june 30
			str.Format(_T("%d"),i-150);
			str=_T("6-")+str;
		}
		else if(i<212)
		{//july 31
			str.Format(_T("%d"),i-180);
			str=_T("7-")+str;
		}
		else if(i<243)
		{//auguest 31 
			str.Format(_T("%d"),i-211);
			str=_T("8-")+str;
		}
		else if(i<273)
		{//september 30
			str.Format(_T("%d"),i-242);
			str=_T("9-")+str;
		}
		else if(i<304)
		{//october 31
			str.Format(_T("%d"),i-272);
			str=_T("10-")+str;
		}
		else if(i<334)
		{//nobember 30
			str.Format(_T("%d"),i-303);
			str=_T("11-")+str;
		}
		else if(i<365)
		{//december 31
			str.Format(_T("%d"),i-333);
			str=_T("12-")+str;
		}
		int l=1,m;
		m=i%8;//bit day
		for(int j=0;j<m;j++)
			l*=2;
		if(the_days[i/8] & l)/////////////////////
			set_day_state(str);
	}
}

void AnnualRout_InsertDia::load()
{

	set_day_state(TO_CLEAR_MONTH_CTRL);//clear month ctrl	

    if (m_offline)
	{
		for (int i=0;i<ONE_YEAR_BETYS;i++)
		{
			CString strTemp;
			strTemp.Format(_T("Day%d"), i);
			the_days[i] = GetPrivateProfileInt(_T("T3000_HOLIDAY_OFFLINEMODE"), strTemp, 0, m_configfile_path);
		}
	} 
	else
	{
		int nFlag = product_register_value[7];
		if (m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
			Read_Multi(g_tstat_id, the_days, 5752 + ONE_YEAR_BETYS*(m_addr - 1), ONE_YEAR_BETYS);//get from network
		else if (product_register_value[7] == PM_TSTAT8 || product_register_value[7] == PM_TSTAT9 || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
		{

			Read_Multi(g_tstat_id, the_days, 925, ONE_YEAR_BETYS);
		}
		else
			Read_Multi(g_tstat_id, the_days, MODBUS_AR_TIME_FIRST + ONE_YEAR_BETYS*(m_addr - 1), ONE_YEAR_BETYS);//get from network
		int i = 0, j = 0;
		for (i = 0; i < ONE_YEAR_BETYS; i++)
		{
            memcpy(&pDayState[0], &the_days[0], 4);
            //pDayState[month - 1] = the_days[i];
			if (the_days[i] != 0xFF)
				break;
		}
        unsigned char the_char_days[ONE_YEAR_BETYS];

        for (int i = 0; i < ONE_YEAR_BETYS; i++)
        {
            the_char_days[i] = the_days[i];
        }

        for (int i = 0;i<12;i++)
        {
            if (i == 0)
            {
                memcpy_s(&pDayState[i], 4, &the_char_days[i * 4], 4);
            }
            else
            {
                long long temp_data = 0;
                int start_byte = 0;
                int move_bit = 0;
                start_byte = day_in_this_year[i] / 8;// = 3  //day_in_this_year[i]/8
                move_bit = day_in_this_year[i] % 8;// = 7  day_in_this_year[i]%8

                memcpy_s(&temp_data, 5, &the_char_days[start_byte], 5);
                temp_data = temp_data >> move_bit;
                temp_data = temp_data % (0x0100000000);

                DWORD temp_dw;
                temp_dw = (DWORD)temp_data;
                memcpy_s(&pDayState[i], 4, &temp_dw, 4);

            }

        }
        m_month_ctrl.SetDayState(12, pDayState);
	}
}

BOOL AnnualRout_InsertDia::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_LIST1)->ShowWindow(0);
	GetDlgItem(IDC_YEARSTATIC)->ShowWindow(0);
	GetDlgItem(IDC_COMBO1)->ShowWindow(0);
	 
	GetDlgItem(IDC_STATIC)->ShowWindow(0);
	GetDlgItem(IDOK)->ShowWindow(0);

	//Select all sunday days for selected year 
	//m_month_ctrl.SelectDates(GetAllYearDaysForDayOfWeek(2017, 0));
	m_offline = FALSE;
	CTime temp_time =  CTime::GetCurrentTime();
	unsigned short this_year = temp_time.GetYear();

    if (this_year % 4 == 0)
        m_leap_year = 1;
    else
        m_leap_year = 0;
    if (product_register_value[7]==0 && (g_protocol == MODBUS_RS485 || g_protocol == MODBUS_TCPIP))
    {
		m_offline = TRUE;
    }
	if (g_protocol == PROTOCOL_UNKNOW)
	{
		m_offline = TRUE;
	}
	if (m_offline)
	{
		SYSTEMTIME systime;
		::GetSystemTime(&systime);
		CTime time(systime);
		if (time.GetYear() % 4 == 0)
			is_leap = true;//leap year
		else
			is_leap = false;//no leap year

		SYSTEMTIME sysFromtime;
		SYSTEMTIME sysEndtime;
		sysFromtime = systime;
		sysEndtime = systime;
		sysFromtime.wMonth = 1;
		sysEndtime.wMonth = 12;
		sysFromtime.wDay = 1;
		sysEndtime.wDay = 31;
		m_month_ctrl.SetRange(&sysFromtime, &sysEndtime);

		month_nCount = m_month_ctrl.GetMonthRange(&timeFrom, &timeUntil, GMR_DAYSTATE);
		pDayState = new MONTHDAYSTATE[month_nCount];
		memset(pDayState, 0, sizeof(MONTHDAYSTATE) * month_nCount);

		load();

	} 
	else
	{
		int nFlag = product_register_value[7];
        if (Bacnet_Private_Device(nFlag))
		//if (g_protocol == PROTOCOL_BACNET_IP)
		{								//器件。产品本来不同 界面就会有差异 都还要用一个界面。木有办法，只能 加在一起了。;
			GetDlgItem(IDC_LIST1)->ShowWindow(0);
			GetDlgItem(IDC_YEARSTATIC)->ShowWindow(0);
			GetDlgItem(IDC_COMBO1)->ShowWindow(0);

			GetDlgItem(IDC_STATIC)->ShowWindow(0);
			GetDlgItem(IDOK)->ShowWindow(0);

 			MoveWindow(100, 100, 980, 600);
 			GetDlgItem(IDC_MONTHCALENDAR1)->MoveWindow(10, 10, 980, 600);

			m_schedule_day_dlg_hwnd = this->m_hWnd;
			//g_hwnd_now = m_schedule_day_dlg_hwnd;

			SYSTEMTIME StartTime1;
			StartTime1.wYear = this_year;
			StartTime1.wMonth = 1;
			StartTime1.wDay = 1;
			SYSTEMTIME EndTime1;
			EndTime1.wYear = this_year;
			EndTime1.wMonth = 12;
			EndTime1.wDay = 31;
			m_month_ctrl.SetRange(&StartTime1, &EndTime1);
			//m_month_ctrl.SetColor(MCSC_TEXT,RGB(240,0,0));
			//int ret_results = m_month_ctrl.SetColor(MCSC_BACKGROUND,RGB(255,0,0));
// 			int	ret_results = m_month_ctrl.SetColor(MCSC_TEXT, RGB(255, 255, 0));
// 			ret_results = m_month_ctrl.GetColor(MCSC_TEXT);

			SYSTEMTIME timeFrom;
			SYSTEMTIME timeUntil;
			int nCount = m_month_ctrl.GetMonthRange(&timeFrom, &timeUntil, GMR_DAYSTATE);
			PostMessage(WM_REFRESH_BAC_DAY_CAL, NULL, NULL);


		}
		else if ((product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9) || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
		{
			GetDlgItem(IDC_LIST1)->ShowWindow(0);
			GetDlgItem(IDC_YEARSTATIC)->ShowWindow(0);
			GetDlgItem(IDC_COMBO1)->ShowWindow(0);
			GetDlgItem(IDC_COMBO1)->ShowWindow(0);
			GetDlgItem(IDC_STATIC)->ShowWindow(0);
			GetDlgItem(IDOK)->ShowWindow(0);
			MoveWindow(100, 100, 680, 800);
			GetDlgItem(IDC_MONTHCALENDAR1)->MoveWindow(10, 10, 680, 800);



			m_strtype = GetProductName(product_register_value[7]);
			SYSTEMTIME systime;
			::GetSystemTime(&systime);
			CTime time(systime);
			if (time.GetYear() % 4 == 0)
				is_leap = true;//leap year
			else
				is_leap = false;//no leap year
								/*for (int i = 0; i < 20; i++)
								{
								CString strTempYear;
								strTempYear.Format(_T("%d"), systime.wYear + i);
								m_yearComBox.AddString(strTempYear);
								m_yearComBox.SetCurSel(0);
								}*/


			SYSTEMTIME sysFromtime;
			SYSTEMTIME sysEndtime;
			sysFromtime = systime;
			sysEndtime = systime;
			sysFromtime.wMonth = 1;
			sysEndtime.wMonth = 12;
			sysFromtime.wDay = 1;
			sysEndtime.wDay = 31;
			m_month_ctrl.SetRange(&sysFromtime, &sysEndtime);



				
			CString str;
			//	description2 temp_description;
			//	Annual_Routines temp;
			for (unsigned char i = 1; i <= 16; i++)
			{//////////////////////////////////////////////////get information from network
			 //		temp_description=temp.read_addr(i-1);
			 //		str.Format("%s",temp_description.full_label);
				str.Format(_T("%d"), i);
				m_list_ctrl.AddString(str);
			}
			m_list_ctrl.SetCurSel(m_addr - 1);
			//*********************for set_day_state function


			/*
			m_monthViewCtrl.put_Enabled(TRUE);
			m_monthViewCtrl.put_MultiSelect(TRUE);
			m_monthViewCtrl.ShowWindow(SW_HIDE);

			*/

			//m_month_ctrl.SetColor(MCSC_TEXT,RGB(240,0,0));
			//m_month_ctrl.

			month_nCount = m_month_ctrl.GetMonthRange(&timeFrom, &timeUntil, GMR_DAYSTATE);
			pDayState = new MONTHDAYSTATE[month_nCount];
			memset(pDayState, 0, sizeof(MONTHDAYSTATE) * month_nCount);
			//*********************for set_day_state function
			load();


		}
		else
		{
			SYSTEMTIME systime;
			::GetSystemTime(&systime);
			CTime time(systime);
			if (time.GetYear() % 4 == 0)
				is_leap = true;//leap year
			else
				is_leap = false;//no leap year
			for (int i = 0; i<20; i++)
			{
				CString strTempYear;
				strTempYear.Format(_T("%d"), systime.wYear + i);
				m_yearComBox.AddString(strTempYear);
				m_yearComBox.SetCurSel(0);
			}
			OSVERSIONINFO Version;
			ZeroMemory(&Version, sizeof(OSVERSIONINFO));
			Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&Version);
			if (Version.dwMajorVersion <= 5)
				m_bXpOS = TRUE;
			else
				m_bXpOS = FALSE;
			if (m_bXpOS)
			{
				// 			m_yearComBox.ShowWindow(SW_HIDE);
				// 			GetDlgItem(IDC_YEARSTATIC)->ShowWindow(SW_HIDE);
			}
			else
			{
				// 			m_yearComBox.ShowWindow(SW_SHOW);
				// 			GetDlgItem(IDC_YEARSTATIC)->ShowWindow(SW_SHOW);
				SYSTEMTIME sysFromtime;
				SYSTEMTIME sysEndtime;
				sysFromtime = systime;
				sysEndtime = systime;
				sysFromtime.wMonth = 1;
				sysEndtime.wMonth = 12;
				sysFromtime.wDay = 1;
				sysEndtime.wDay = 31;
				m_month_ctrl.SetRange(&sysFromtime, &sysEndtime);
			}

			//m_month_ctrl.SetMonthView();









				
			CString str;
			//	description2 temp_description;
			//	Annual_Routines temp;
			for (unsigned char i = 1; i <= 16; i++)
			{//////////////////////////////////////////////////get information from network
			 //		temp_description=temp.read_addr(i-1);
			 //		str.Format("%s",temp_description.full_label);
				str.Format(_T("%d"), i);
				m_list_ctrl.AddString(str);
			}
			m_list_ctrl.SetCurSel(m_addr - 1);
			//*********************for set_day_state function


			/*
			m_monthViewCtrl.put_Enabled(TRUE);
			m_monthViewCtrl.put_MultiSelect(TRUE);
			m_monthViewCtrl.ShowWindow(SW_HIDE);

			*/

			//m_month_ctrl.SetColor(MCSC_TEXT,RGB(240,0,0));
			//m_month_ctrl.

			month_nCount = m_month_ctrl.GetMonthRange(&timeFrom, &timeUntil, GMR_DAYSTATE);
			pDayState = new MONTHDAYSTATE[month_nCount];
			memset(pDayState, 0, sizeof(MONTHDAYSTATE) * month_nCount);
			//*********************for set_day_state function
			load();
		}
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void AnnualRout_InsertDia::set_day_state(CString month_day)///////////////attention ,the control must be 12 month
{
	CString temp;
	int month,day;
	month=_wtoi(month_day);
	temp.Format(_T("%d"),month);
	day=_wtoi(month_day.Right(month_day.GetLength()-1-temp.GetLength()));
	if(month_day==TO_CLEAR_MONTH_CTRL)
	{	
		for(int i=0;i<month_nCount;i++)
			pDayState[i]=0;
		m_month_ctrl.SetDayState(month_nCount,pDayState);
		return ;
	}
	if(month<0 || month>12 || day<0 || day>31)
		return;
	/////////////////////////////////////////////
	
	if(timeFrom.wDay==1)
		pDayState[month-1] ^=1<<(day-1);
	else
		pDayState[month] ^=1<<(day-1);
		

		
	//pDayState[month-1] ^=1<<(day-1);
	/////////////////////////////////////////////

	m_month_ctrl.SetDayState(month_nCount,pDayState);

}

bool AnnualRout_InsertDia::the_holiday_is_exist(CString var1)
{//return value ,true is exist ,false is no exist
	for(int i=0;i<(int)holiday_list.size();i++)
		if(var1==holiday_list.at(i))
			return true;
    return false;
}

void AnnualRout_InsertDia::add_holiday(CString var1)
{//var string must be format "%d-%d"

}

void AnnualRout_InsertDia::delete_holiday(CString var1)
{//var string must be format "%d-%d"
	
}
int AnnualRout_InsertDia::the_day_number(CString var1)
{//var string must be format "%d-%d"
	int month,day,temp;
	temp=var1.Find('-');
	month=_wtoi(var1.Left(temp));
	day=_wtoi(var1.Right(var1.GetLength()-temp-1));//
	if(is_leap==true)
	{//366 days
		switch(month)
		{
		case 1:day=day;break;
		case 2:day+=31;break;
		case 3:day+=60;break;
		case 4:day+=91;break;
		case 5:day+=121;break;
		case 6:day+=152;break;
		case 7:day+=182;break;
		case 8:day+=213;break;
		case 9:day+=244;break;
		case 10:day+=274;break;
		case 11:day+=305;break;
		case 12:day+=335;break;
		}
	}
	else
	{//365 days
		switch(month)
		{
		case 1:day=day;break;
		case 2:day+=31;break;
		case 3:day+=59;break;
		case 4:day+=90;break;
		case 5:day+=120;break;
		case 6:day+=151;break;
		case 7:day+=181;break;
		case 8:day+=212;break;
		case 9:day+=243;break;
		case 10:day+=273;break;
		case 11:day+=304;break;
		case 12:day+=334;break;
		}		
	}
	return day-1;//////////////for m
}

void AnnualRout_InsertDia::OnBnClickedButton2()
{

	
	//delete

	UpdateData(true);
	CString str;
	str.Format(_T("%d-%d"),m_date_time.GetMonth(),m_date_time.GetDay());
	int day_number=the_day_number(str);
	int l=1,m;
	m=day_number%8;//bit day
	for(int j=0;j<m;j++)
		l*=2;
	the_days[day_number/8]=the_days[day_number/8] ^ l;
	unsigned char ttt[ONE_YEAR_BETYS];
	for(int i=0;i<ONE_YEAR_BETYS;i++)
		ttt[i]=(unsigned char)the_days[i];
	Write_Multi(g_tstat_id,ttt,MODBUS_AR_TIME_FIRST + ONE_YEAR_BETYS*(m_addr-1),ONE_YEAR_BETYS);
	NET_WORK_SLEEP_BETWEEN_WRITE_READ
	load();
}

void AnnualRout_InsertDia::OnDestroy()
{
	if(1)
	{
		//MessageBox(_T("Need to Add code here"));
			CDialog::OnDestroy();	
	}
	else
	{
	CDialog::OnDestroy();	
	delete [] pDayState;//for day state         function
	}
}

//void AnnualRout_InsertDia::OnBnClickedButton3()
//{
//	
//	//clear button
//	unsigned char ttt[ONE_YEAR_BETYS];
//	for(int i=0;i<ONE_YEAR_BETYS;i++)
//		ttt[i]=0;
//	Write_Multi(g_tstat_id,ttt,MODBUS_AR_TIME_FIRST + ONE_YEAR_BETYS*(m_addr-1),ONE_YEAR_BETYS);
//	NET_WORK_SLEEP_BETWEEN_WRITE_READ
//	load();
//}

void AnnualRout_InsertDia::OnRButtonDown(UINT nFlags, CPoint point)
{
	

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL AnnualRout_InsertDia::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_RBUTTONDOWN )
	{
		if((g_protocol == PROTOCOL_BACNET_IP)|| (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_GSM))
		{
			return true;
		}
		else
		{
			if(pMsg->hwnd==m_month_ctrl.GetSafeHwnd())
			{
				CMenu menu;
				menu.LoadMenu(IDR_POPUP_MENU);
				CMenu *pmenu=menu.GetSubMenu(1);	
				CPoint point=pMsg->pt;
				pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);
				return true;
			}
		}

	}
	if(pMsg->message == WM_LBUTTONDBLCLK )
	{
		return false;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void AnnualRout_InsertDia::OnLbnSelchangeList1()
{
	
	UpdateData(true);
	m_addr=m_list_ctrl.GetCurSel()+1;
	load();
}

void AnnualRout_InsertDia::OnAnnualroutAdd()
{
	
	UpdateData(true);
	
	
}

void AnnualRout_InsertDia::OnAnnualroutClear()
{
	
	if(IDOK==MessageBox(_T("Clear All?"),_T("CLEAR"),MB_OKCANCEL))
	{
		m_month_ctrl.UnselectAll();
		unsigned char ttt[ONE_YEAR_BETYS];
		for(int i=0;i<ONE_YEAR_BETYS;i++)
			ttt[i]=0;
		if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
			Write_Multi(g_tstat_id,ttt,5752 + ONE_YEAR_BETYS*(m_addr-1),ONE_YEAR_BETYS);
        else if (m_strtype.CompareNoCase(_T("Tstat8")) == 0)
        {
            for (int i = 0; i < ONE_YEAR_BETYS; i++)
            {
                write_one(g_tstat_id, 925 + i, 0);
            }
        }
		else
			Write_Multi(g_tstat_id,ttt,MODBUS_AR_TIME_FIRST + ONE_YEAR_BETYS*(m_addr-1),ONE_YEAR_BETYS);
		NET_WORK_SLEEP_BETWEEN_WRITE_READ
		load();
	}
}

void AnnualRout_InsertDia::OnBnClickedOk()
{
	
	OnOK();
}

LRESULT AnnualRout_InsertDia::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if(message == WM_LBUTTONDBLCLK )
	{

		
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void AnnualRout_InsertDia::OnCbnSelchangeCombo1()
{

	CString strYear;
	m_yearComBox.GetWindowText(strYear);
	int nYear=m_yearComBox.GetCurSel();
		
	SYSTEMTIME systime;
	::GetSystemTime(&systime);

	SYSTEMTIME sysFromtime;
	SYSTEMTIME sysEndtime;
	sysFromtime=systime;
	sysEndtime=systime;

	sysFromtime.wYear=systime.wYear+nYear;
	sysEndtime.wYear=systime.wYear+nYear;
	sysFromtime.wMonth=1;
	sysEndtime.wMonth=12;
	sysFromtime.wDay=1;
	sysEndtime.wDay=31;
	m_month_ctrl.SetRange(&sysFromtime,&sysEndtime);
	m_month_ctrl.SetRedraw();
	

	
}

#pragma region BAC_NET_CM5

LRESULT AnnualRout_InsertDia::Fresh_Schedule_Day_Cal(WPARAM wParam,LPARAM lParam)
{
	//

	for (int i=0;i<12;i++)
	{
        //MONTHDAYSTATE	pBacDayState
            memset(&pBacDayState[i], 0, sizeof(MONTHDAYSTATE));
		//pDayState[i] = (DWORD)g_DayState[annual_list_line]
		//memset((void *)pDayState[i],0,sizeof(pDayState)/sizeof(pDayState[0]));

		if(i == 0)
		{
			memcpy_s(&pBacDayState[i],4,&g_DayState[annual_list_line][i*4],4);
		}
		else
		{
			long long temp_data = 0;
			int start_byte = 0;
			int move_bit = 0;
			start_byte = day_in_this_year[i]/8;// = 3  //day_in_this_year[i]/8
			move_bit = day_in_this_year[i]%8;// = 7  day_in_this_year[i]%8

			memcpy_s(&temp_data,5,&g_DayState[annual_list_line][start_byte],5);
			temp_data = temp_data>>move_bit;
			temp_data = temp_data % (0x0100000000);

			DWORD temp_dw;
			temp_dw = (DWORD)temp_data;
			memcpy_s(&pBacDayState[i],4,&temp_dw,4);

			Sleep(1);
			//memcpy_s(&temp_data,5,&day_in_this_year[1],4);
			//memcpy_s(&pBacDayState[i],4,&g_DayState[annual_list_line][i*4],4);
		}
	}
	m_month_ctrl.SetDayState(12, pBacDayState);


	return 0;
}



LRESULT  AnnualRout_InsertDia::DayResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	CString temp_cs;
	temp_cs.Format(_T("%d"),pInvoke->Invoke_ID);
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		CString temp_ok;
		temp_ok = _T("Bacnet operation success!   Request ID:") +  temp_cs;
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_ok);
#ifdef SHOW_MESSAGEBOX
		MessageBox(temp_ok);
#endif
	}
	else
	{
		CString temp_fail;
		temp_fail = _T("Bacnet operation fail!   Request ID:") +  temp_cs;
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_fail);
#ifdef SHOW_ERROR_MESSAGE
		MessageBox(temp_fail);
#endif
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
}



void AnnualRout_InsertDia::OnMcnSelectBacMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult)
{
	//Exchanged selection handler
	LPNMSELCHANGEEX pSelChange = reinterpret_cast<LPNMSELCHANGEEX>(pNMHDR);
	
	
	//Get last selected item
	LPSELECTION_ITEM current = pSelChange->selectionInfo.first;
    if (current == NULL)
    {
        return;
    }
	while(current->next)
	{
		current = current->next;
	}
    int Clicked_year = current->date.wYear;
    m_leap_year = 0;
    if (Clicked_year % 4 == 0)
        m_leap_year = 1;
    else
        m_leap_year = 0;
    int Clicked_month = current->date.wMonth;
    int Clicked_day = current->date.wDay;

    m_month_ctrl.MONTHCAL_GetSelDay_Month(&Clicked_month, &Clicked_day);

    if (Clicked_month > 12)
        return;
    if (Clicked_day > 31)
        return;

    
	if (m_offline)
	{
		CString str;
		str.Format(_T("%d-%d"), Clicked_month, Clicked_day);
		int day_number = the_day_number(str);
		int l = 1, m;
		int address_offset = day_number / 8;
		m = day_number % 8;//bit day
		for (int j = 0; j < m; j++)
			l *= 2;
		the_days[day_number / 8] = the_days[day_number / 8] ^ l;//异或。


		CString strKeyName,strValue;
		strKeyName.Format(_T("Day%d"), address_offset);
		strValue.Format(_T("%d"), the_days[day_number / 8]);
		WritePrivateProfileStringW(_T("T3000_HOLIDAY_OFFLINEMODE"), strKeyName, strValue,m_configfile_path);
		load();
	} 
	else
	{
		int nFlag = product_register_value[7];
        if(Bacnet_Private_Device(nFlag) == false)
		//if (g_protocol != PROTOCOL_BACNET_IP)
		{
			//CString str;
			//str.Format(_T("%d-%d"), Clicked_month, Clicked_day);
			//int day_number = the_day_number(str);
			//int l = 1, m;
			//int address_offset = day_number / 8;
			//m = day_number % 8;//bit day
			//for (int j = 0; j < m; j++)
			//	l *= 2;
			//the_days[day_number / 8] = the_days[day_number / 8] ^ l;//异或。

#if 1
            int day_in_year = day_in_this_year[Clicked_month - 1] + Clicked_day;
            int charactor_control = (day_in_year - 1) / 8;
            int control_bit = (day_in_year - 1) % 8;
            if (((pDayState[Clicked_month - 1] >> Clicked_day - 1) & 0x00000001) == 1)
            {
                pDayState[Clicked_month - 1] &= ~(1 << Clicked_day - 1);   // 4th day
                the_days[charactor_control] &= ~(1 << control_bit);
            }
            else
            {
                pDayState[Clicked_month - 1] |= 1 << Clicked_day - 1;   // 4th day
                the_days[charactor_control] |= 1 << control_bit;
            }
#endif


			if ((product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9) || (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
			{
                //int address_offset = 0;
                //unsigned short nvalue = 0;
                //address_offset = charactor_control / 2;
                //if (charactor_control % 2 == 0)
                //    nvalue = the_days[charactor_control] + the_days[charactor_control + 1] * 256;
                //else
                //    nvalue = +the_days[charactor_control - 1] + the_days[charactor_control] * 256;
				int ret = write_one(g_tstat_id, 925 + charactor_control, the_days[charactor_control]);

                //static int test_int_abc = 0;
                //test_int_abc++;
                //CString temp_cs1;
                //temp_cs1.Format(_T("Trace select event %d  %d  Month:%d  Day:%d reg:%d  value:%d\r\n"), test_int_abc, the_days[charactor_control], Clicked_month, Clicked_day, 925 + charactor_control, the_days[charactor_control]);
                //DFTrace(temp_cs1);

                Sleep(1);
			}
			else
			{
				unsigned char ttt[ONE_YEAR_BETYS];
				for (int i = 0; i < ONE_YEAR_BETYS; i++)
					ttt[i] = (unsigned char)the_days[i];
				if (m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
					Write_Multi(g_tstat_id, ttt, 5752 + ONE_YEAR_BETYS*(m_addr - 1), ONE_YEAR_BETYS);
				else
					Write_Multi(g_tstat_id, ttt, MODBUS_AR_TIME_FIRST + ONE_YEAR_BETYS*(m_addr - 1), ONE_YEAR_BETYS);
			}


			NET_WORK_SLEEP_BETWEEN_WRITE_READ
//		    load();
		}
		else
		{
			int day_in_year = day_in_this_year[Clicked_month - 1] + Clicked_day;
			int charactor_control = (day_in_year - 1) / 8;
			int control_bit = (day_in_year - 1) % 8;
			if (((pBacDayState[Clicked_month - 1] >> Clicked_day - 1) & 0x00000001) == 1)
			{
				pBacDayState[Clicked_month - 1] &= ~(1 << Clicked_day - 1);   // 4th day
				g_DayState[annual_list_line][charactor_control] &= ~(1 << control_bit);
			}
			else
			{
				pBacDayState[Clicked_month - 1] |= 1 << Clicked_day - 1;   // 4th day
				g_DayState[annual_list_line][charactor_control] |= 1 << control_bit;
			}

			m_month_ctrl.SetDayState(12, pBacDayState);



			//if (g_protocol == PROTOCOL_BACNET_IP)
			//{
				//for (int i=0;i<12;i++)
				//{
				//	memcpy_s(&g_DayState[annual_list_line][i*4],4,&pBacDayState[i],4);
				//}
				CString temp_task_info;
				temp_task_info.Format(_T("Write annual schedual List Item%d ."), annual_list_line + 1);
				Post_Write_Message(g_bac_instance, WRITEANNUALSCHEDULE_T3000, annual_list_line, annual_list_line, ANNUAL_CODE_SIZE, this->m_hWnd, temp_task_info);
			//}

		}
	}


	

	

	*pResult = 0;
}





#pragma endregion BAC_NET_CM5

