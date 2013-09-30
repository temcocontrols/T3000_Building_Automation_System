// BacnetScheduleTime.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetScheduleTime.h"
#include "afxdialogex.h"

#include "globle_function.h"
#include "gloab_define.h"
// CBacnetScheduleTime dialog

IMPLEMENT_DYNAMIC(CBacnetScheduleTime, CDialogEx)

CBacnetScheduleTime::CBacnetScheduleTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetScheduleTime::IDD, pParent)
{

}

CBacnetScheduleTime::~CBacnetScheduleTime()
{
}

void CBacnetScheduleTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SCHEDULE_TIME, m_schedule_time_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER1_SCHEDUAL, m_schedual_time_picker);
}

BEGIN_MESSAGE_MAP(CBacnetScheduleTime, CDialogEx)	
	//ON_MESSAGE(MY_RESUME_DATA, SchedualTimeResumeMessageCallBack)
		ON_MESSAGE(WM_REFRESH_BAC_SCHEDULE_LIST,Fresh_Schedual_List)
		ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Schedual_Item)
		ON_NOTIFY(NM_CLICK, IDC_LIST_SCHEDULE_TIME, &CBacnetScheduleTime::OnNMClickListScheduleTime)
		ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER1_SCHEDUAL, &CBacnetScheduleTime::OnNMKillfocusDatetimepicker1Schedual)
		ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetScheduleTime message handlers

//LRESULT  CBacnetScheduleTime::SchedualTimeResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
//{
//	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
//	CString temp_cs = pInvoke->task_info;
//	bool msg_result=WRITE_FAIL;
//	msg_result = MKBOOL(wParam);
//	CString Show_Results;
//	if(msg_result)
//	{
//		//CString temp_ok;
//		//temp_ok = _T("Bacnet operation success!   Request ID:") +  temp_cs;
//
//		Show_Results = temp_cs + _T("Success!");
//		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
//
//		//SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_ok);
//		#ifdef SHOW_MESSAGEBOX
//		MessageBox(Show_Results);
//		#endif
//	}
//	else
//	{
//		Show_Results = temp_cs + _T("Fail!");
//		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
//		#ifdef SHOW_ERROR_MESSAGE
//		MessageBox(Show_Results);
//		#endif
//	}
//	if(pInvoke)
//		delete pInvoke;
//	return 0;
//}

BOOL CBacnetScheduleTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_List();
	// TODO:  Add extra initialization here
	g_invoke_id = GetPrivateData(bac_gloab_device_id,READTIMESCHEDULE_T3000,weekly_list_line,weekly_list_line,16*9);
	if(g_invoke_id>=0)
	{
		CString temp_cs_show;
		temp_cs_show.Format(_T("Task ID = %d. Read weekly schedule time from item %d "),g_invoke_id,weekly_list_line);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetScheduleTime::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//const int SCHEDULE_TIME_NUM = 0;
//const int SCHEDULE_TIME_MONDAY = 1;
//const int SCHEDULE_TIME_TUESDAY = 2;
//const int SCHEDULE_TIME_WEDNESDAY = 3;
//const int SCHEDULE_TIME_THURSDAY = 4;
//const int SCHEDULE_TIME_FRIDAY = 5;
//const int SCHEDULE_TIME_SATURDAY = 6;
//const int SCHEDULE_TIME_SUNDAY = 7;
//const int SCHEDULE_TIME_HOLIDAY1 = 8;
//const int SCHEDULE_TIME_HOLIDAY2 = 9;
void CBacnetScheduleTime::Initial_List()
{
	m_schedule_time_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_schedule_time_list.SetExtendedStyle(m_schedule_time_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_NUM, _T(" "), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_MONDAY, _T("Monday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_TUESDAY, _T("Tuesday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_WEDNESDAY, _T("Wednesday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_THURSDAY, _T("Thursday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_FRIDAY, _T("Friday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_SATURDAY, _T("Saturday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_SUNDAY, _T("Sunday"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_HOLIDAY1, _T("Holiday1"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_list.InsertColumn(SCHEDULE_TIME_HOLIDAY2, _T("Holiday2"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_schedule_time_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_schedule_time_dlg_hwnd;

	m_schedule_time_list.DeleteAllItems();
	for (int i=0;i<BAC_SCHEDULE_TIME_COUNT;i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		if(i%2==0)
		temp_item.Format(_T("ON  "),i+1);
		else
		temp_item.Format(_T("OFF"),i+1);
		m_schedule_time_list.InsertItem(i,temp_item);
	}
	CTime TimeTemp(2013,1,1,0,0,0);

	m_schedual_time_picker.SetFormat(_T("HH:mm"));
	m_schedual_time_picker.SetTime(&TimeTemp);
}


LRESULT CBacnetScheduleTime::Fresh_Schedual_List(WPARAM wParam,LPARAM lParam)
{
	CString temp_show;
	for (int j=0;j<9;j++)
	{
		for (int i=0;i<8;i++)
		{
			if((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes >59) ||
				(m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours >23)	||
				((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes == 0)&&
				(m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours == 0)))
			{
				temp_show.Empty();
			}
			else
			{
				temp_show.Format(_T("%02d:%02d"),m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours,
					m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes);
			}
			m_schedule_time_list.SetItemText(i,j+1,temp_show);
		}
	}

	return 0;
}


LRESULT CBacnetScheduleTime::Fresh_Schedual_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;



	//Post_Write_Message(bac_gloab_device_id,WRITEOUTPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_out_point),this->m_hWnd);
	return 0;
}


void CBacnetScheduleTime::OnNMClickListScheduleTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	long lRow,lCol;
	m_schedule_time_list.Set_Edit(false);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_schedule_time_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_schedule_time_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_schedule_time_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	if(lCol == SCHEDULE_TIME_NUM)	//If it is item number,then return.
		return;



	//ignore the incorrect time.
	int temp_hour,temp_minute;
	temp_hour = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[lRow][lCol - 1].time_hours;
	temp_minute = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[lRow][lCol - 1].time_minutes;
	if((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[lRow][lCol -1].time_minutes >59) ||
		(m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[lRow][lCol - 1].time_hours >23)	||
		((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[lRow][lCol - 1].time_minutes == 0)&&
		(m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[lRow][lCol - 1].time_hours == 0)))
	{
		temp_hour = 0;
		temp_minute = 0;
	}

	m_row = lRow;
	m_col = lCol;
	m_hour = temp_hour;
	m_minute = temp_minute;

	m_schedule_time_list.SetItemText(lRow,lCol,_T(""));
	m_schedual_time_picker.ShowWindow(SW_SHOW);
	CRect list_rect,win_rect,client_rect;
	m_schedule_time_list.GetWindowRect(list_rect);
	GetWindowRect(win_rect);
	GetClientRect(client_rect);
	CRect myrect;
	m_schedule_time_list.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);
	myrect.left = myrect.left + list_rect.left - win_rect.left;
	myrect.right = myrect.right + list_rect.left - win_rect.left;
	myrect.top = myrect.top + 11;
	myrect.bottom = myrect.bottom + 13;
	
	m_schedual_time_picker.BringWindowToTop();
	m_schedual_time_picker.MoveWindow(myrect);

	CTime TimeTemp(2013,1,1,temp_hour,temp_minute,0);

	m_schedual_time_picker.SetFormat(_T("HH:mm"));
	m_schedual_time_picker.SetTime(&TimeTemp);
	m_schedual_time_picker.SetFocus();
		SetTimer(1,100,NULL);
		//m_schedual_time_picker.BringWindowToTop();
	//m_schedual_time_picker.MoveWindow()
	
	*pResult = 0;
}


void CBacnetScheduleTime::OnNMKillfocusDatetimepicker1Schedual(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CTime temp_time;CString temp_cs;
	int chour,cmin;
	m_schedual_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[m_row][m_col - 1].time_hours = chour;
	m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[m_row][m_col - 1].time_minutes = cmin;
	if((chour == 0) &&(cmin	==0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d"),chour,cmin);
	m_schedule_time_list.SetItemText(m_row,m_col,temp_cs);

	m_schedual_time_picker.ShowWindow(SW_HIDE);
	CString temp_task_info;
	temp_task_info.Format(_T("Write Schedule Time Item%d .Changed Time to \"%s\" "),weekly_list_line + 1,temp_cs);
	Post_Write_Message(bac_gloab_device_id,WRITETIMESCHEDULE_T3000,weekly_list_line,weekly_list_line,16*9,BacNet_hwd,temp_task_info);

	*pResult = 0;
}


void CBacnetScheduleTime::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		KillTimer(1);
		m_schedual_time_picker.Invalidate();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
