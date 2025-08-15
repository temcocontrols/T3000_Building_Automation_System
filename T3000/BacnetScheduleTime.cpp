// BacnetScheduleTime.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetScheduleTime.h"
#include "afxdialogex.h"

#include "global_function.h"
#include "global_define.h"
#include <mutex>          // std::mutex

std::mutex SCHEDULE_TIME_LIST;           // mutex for critical section
// CBacnetScheduleTime dialog
HWND m_WeeklyParent_Hwnd;
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
		ON_WM_CLOSE()
		ON_BN_CLICKED(IDC_BUTTON_SCHEDULE_COPY_BTN, &CBacnetScheduleTime::OnBnClickedCopyMon_Fri)
         ON_BN_CLICKED(IDC_BUTTON_CLEAR_SCHEDULE, &CBacnetScheduleTime::OnBnClickedClearSchedual)
    
END_MESSAGE_MAP()


// CBacnetScheduleTime message handlers

BOOL CBacnetScheduleTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_List();
	
	PostMessage(WM_REFRESH_BAC_SCHEDULE_LIST,NULL,NULL);

    //2017 12 28 Display optimization: In the popup schedule interface, prioritize displaying the label value; if the label is empty, default to displaying Schedule plus sequence number.
    //2017 12 28 �Ż���ʾ ���ڵ��� ��schedule ���� ������ʾ label ��ֵ�����labelΪ��ֵ��Ĭ����ʾSchedule�����. 
    CString temp_des2;
    MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(weekly_list_line).label, (int)strlen((char *)m_Weekly_data.at(weekly_list_line).label) + 1,
        temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
    temp_des2.ReleaseBuffer();

    CString window_text;
    if (temp_des2.IsEmpty())
    {
        window_text.Format(_T("Schedule %d"), weekly_list_line + 1);
    }
    else
    {
        window_text = temp_des2;
    }

    SetWindowText(window_text);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetScheduleTime::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if(GetFocus()->GetDlgCtrlID() == IDC_LIST_SCHEDULE_TIME)
			{
				CRect list_rect,win_rect;
				m_schedule_time_list.GetWindowRect(list_rect);
				ScreenToClient(&list_rect);
				::GetWindowRect(m_schedule_time_dlg_hwnd,win_rect);
				m_schedule_time_list.Set_My_WindowRect(win_rect);
				m_schedule_time_list.Set_My_ListRect(list_rect);
				m_schedule_time_list.Get_clicked_mouse_position();
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_DATETIMEPICKER1_SCHEDUAL)
			{
				GetDlgItem(IDC_LIST_SCHEDULE_TIME)->SetFocus();
			}
			return TRUE;
		}
		else if((pMsg->wParam == VK_DELETE) || ((pMsg->wParam == VK_BACK)))
		{
			if(GetFocus()->GetDlgCtrlID() == IDC_DATETIMEPICKER1_SCHEDUAL)
			{
				CTime TimeTemp(2019,1,1,0,0,0);
				m_schedual_time_picker.SetFormat(_T("HH:mm"));
				m_schedual_time_picker.SetTime(&TimeTemp);
				GetDlgItem(IDC_LIST_SCHEDULE_TIME)->SetFocus();
			}
            if (GetFocus()->GetDlgCtrlID() == IDC_LIST_SCHEDULE_TIME)
            {
                int temp_row; int temp_col;
                m_schedule_time_list.Get_Selected_Item(temp_row, temp_col);

                m_schedule_time_list.SetItemText(temp_row, temp_col, _T(""));
                m_Schedual_time_flag.at(weekly_list_line).Time_flag[temp_row][temp_col - 1] = 255;
                if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 492)
                {
                    if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, weekly_list_line, weekly_list_line) > 0)
                    {
                        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule success!"));
                    }
                }
                m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[temp_row][temp_col - 1].time_hours = 0;
                m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[temp_row][temp_col - 1].time_minutes = 0;
                CString temp_cs = _T("00:00");
                CString temp_task_info;
                temp_task_info.Format(_T("Write Schedule Time Item%d .Changed Time to \"%s\" "), weekly_list_line + 1, temp_cs);
                Post_Write_Message(g_bac_instance, WRITETIMESCHEDULE_T3000, weekly_list_line, weekly_list_line, WEEKLY_SCHEDULE_SIZE, BacNet_hwd, temp_task_info);

                
            }
		}

			
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


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


	m_WeeklyParent_Hwnd = g_hwnd_now;

	m_schedule_time_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_schedule_time_dlg_hwnd;

	CRect list_rect,win_rect;
	m_schedule_time_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_schedule_time_dlg_hwnd,win_rect);
	m_schedule_time_list.Set_My_WindowRect(win_rect);
	m_schedule_time_list.Set_My_ListRect(list_rect);


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

        for (int x = 0;x<SCHEDULE_TIME_COUNT;x++)
        {
            if ((i % 2) == 0)
                m_schedule_time_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
            else
                m_schedule_time_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
        }
	}
	CTime TimeTemp(2019,1,1,0,0,0);

	m_schedual_time_picker.SetFormat(_T("HH:mm"));
	m_schedual_time_picker.SetTime(&TimeTemp);
}

void CBacnetScheduleTime::OnClose()
{
	 

	g_hwnd_now = m_WeeklyParent_Hwnd;
	if(m_weekly_dlg_hwnd!=NULL)
		::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	CDialogEx::OnClose();
}


LRESULT CBacnetScheduleTime::Fresh_Schedual_List(WPARAM wParam,LPARAM lParam)
{
	SCHEDULE_TIME_LIST.lock();
	CString temp_show;
    for (int i = 0;i < 8;i++)
    {
        for (int j = 0;j < 9;j++)
        {
            //Filter unreasonable time, display empty for unreasonable time;
            //���˲������ʱ�䣬���������ʾ��;
                //if((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes >59) ||
                //	(m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours >23)	||
                //	((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes == 0)
       //                 &&(m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours == 0)))

            if ((m_Schedual_time_flag.at(weekly_list_line).Time_flag[i][j] != 0) &&
                (m_Schedual_time_flag.at(weekly_list_line).Time_flag[i][j] != 1) )
            {
                //temp_show.Format(_T("--:--"));
                temp_show.Empty();
            }
            else
            {

                temp_show.Format(_T("%02d:%02d"), m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours,
                    m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes);
            }
#if 0
            //2018 05 31 Chelsea's requirement: If the second row has a value and the first row is 00:00, it's hidden by default, so it should be displayed;
            //2018 05 31 �ж���Ҫ�� ��� �ڶ�����ֵ ����һ������ 00:00 ��ʱ�� Ĭ����������صģ���ʱ��Ҫ��ʾ����;
            if ((i == 0) && temp_show.IsEmpty() &&
                ((m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[1][j].time_minutes != 0)
                    || (m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[1][j].time_hours != 0)))
            {
                temp_show.Format(_T("00:00"));
            }
#endif
			
            m_schedule_time_list.SetItemText(i, j + 1, temp_show);
			
        }
    }
	SCHEDULE_TIME_LIST.unlock();
	return 0;
}


LRESULT CBacnetScheduleTime::Fresh_Schedual_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;



	//Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_out_point),this->m_hWnd);
	return 0;
}


void CBacnetScheduleTime::OnNMClickListScheduleTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	long lRow,lCol;
	m_schedule_time_list.Set_Edit(false);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	m_schedule_time_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_schedule_time_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	//If the click area exceeds the maximum row number, the click is invalid
	if(lRow>m_schedule_time_list.GetItemCount()) //����������������кţ���������Ч��
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
	CRect list_rect,win_rect;
	m_schedule_time_list.GetWindowRect(list_rect);
	GetWindowRect(win_rect);
	CRect myrect;
	m_schedule_time_list.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);
	myrect.left = myrect.left + list_rect.left - win_rect.left - 5;
	myrect.right = myrect.right + list_rect.left - win_rect.left ;
	myrect.top = myrect.top + 9;
	myrect.bottom = myrect.bottom + 17;
	
	m_schedual_time_picker.BringWindowToTop();
	m_schedual_time_picker.MoveWindow(myrect);

	CTime TimeTemp(2019,1,1,temp_hour,temp_minute,0);

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
	
	CTime temp_time;CString temp_cs;
	int chour,cmin;
	m_schedual_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();

    m_schedual_time_picker.ShowWindow(SW_HIDE);


	if(m_row > 0)
	{
		int is_larger = false;
		for (int i=0;i<m_row;i++)
		{
			if((chour*60 + cmin) <= (m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][m_col - 1].time_hours)*60 +  m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][m_col - 1].time_minutes )
			{
				m_schedual_time_picker.ShowWindow(SW_HIDE);
				if((chour == 0) && (cmin == 0))
				{

				}
				else
				{
					MessageBox(_T("The time is not increasing!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
					return;
				}

			}
		}


	}
    if ((m_row != 0) && (chour == 0) && (cmin == 0))
        return;
	m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[m_row][m_col - 1].time_hours = chour;
	m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[m_row][m_col - 1].time_minutes = cmin;

	

    if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 492)
    {
        if (((chour == 0) && (cmin == 0)) && ((m_row == 0) || (m_row == 1)))
        {
            if (m_row % 2 == 0)
                m_Schedual_time_flag.at(weekly_list_line).Time_flag[m_row][m_col - 1] = 1;
            else
                m_Schedual_time_flag.at(weekly_list_line).Time_flag[m_row][m_col - 1] = 0;


			if (product_type == PM_THIRD_PARTY_DEVICE)
			{
				Write_SCeduleTime_ThirdPArtyBacnet(m_row,m_col);
			}
			else if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, weekly_list_line, weekly_list_line) > 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule success!"));
            }
        }
        else if ((chour == 0) && (cmin == 0) && (m_row != 0) && (m_row != 1))
        {
            m_Schedual_time_flag.at(weekly_list_line).Time_flag[m_row][m_col - 1] = 255;
			if (product_type == PM_THIRD_PARTY_DEVICE)
			{
				Write_SCeduleTime_ThirdPArtyBacnet(m_row, m_col);
			}
			else if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, weekly_list_line, weekly_list_line) > 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule success!"));
            }
        }
        else
        {
			if (m_col > 0)
			{
				if (m_row % 2 == 0)
					m_Schedual_time_flag.at(weekly_list_line).Time_flag[m_row][m_col - 1] = 1;
				else
					m_Schedual_time_flag.at(weekly_list_line).Time_flag[m_row][m_col - 1] = 0;
			}

            temp_cs.Format(_T("%02d:%02d"), chour, cmin);
			if (product_type == PM_THIRD_PARTY_DEVICE)
			{
				Write_SCeduleTime_ThirdPArtyBacnet(m_row, m_col);
			}
            else if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, weekly_list_line, weekly_list_line) > 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule success!"));
            }
        }
    }else if (product_type == PM_THIRD_PARTY_DEVICE)
	{
		Write_SCeduleTime_ThirdPArtyBacnet(m_row, m_col);
		//m_schedule_time_list.SetItemText(m_row, m_col, temp_cs);
		//m_schedual_time_picker.ShowWindow(SW_HIDE);
		//return;
	}
	if (product_type != PM_THIRD_PARTY_DEVICE) {
		CString temp_task_info;
		temp_task_info.Format(_T("Write Schedule Time Item%d .Changed Time to \"%s\" "), weekly_list_line + 1, temp_cs);
		Post_Write_Message(g_bac_instance, WRITETIMESCHEDULE_T3000, weekly_list_line, weekly_list_line, WEEKLY_SCHEDULE_SIZE, BacNet_hwd, temp_task_info);
		
	}
	PostMessage(WM_REFRESH_BAC_SCHEDULE_LIST, NULL, NULL);
	m_schedule_time_list.SetItemText(m_row,m_col,temp_cs);

	m_schedual_time_picker.ShowWindow(SW_HIDE);
	
	*pResult = 0;
}
void CBacnetScheduleTime::Write_SCeduleTime_ThirdPArtyBacnet(int row,int col)
{
	uint8_t Temp_Buf[MAX_APDU] = { 0 };
	BACNET_READ_PROPERTY_DATA writeData;// = new BACNET_READ_PROPERTY_DATA;
	writeData.object_instance = m_Weekly_data_instance.at(weekly_list_line);
	writeData.object_property = PROP_WEEKLY_SCHEDULE;
	writeData.object_type = OBJECT_SCHEDULE;
	writeData.application_data_len = 0;
	//writeData.application_data = new uint8_t;
	//writeData.application_data[MAX_APDU] = { 0 };
	int len = 0;
	//encode_opening_tag(writeData->application_data, 3);
	//writeData->application_data_len += len;
	for (int x = 0; x < 7; x++)
	{
		//len = encode_opening_tag(writeData->application_data, 0);
		//writeData->application_data_len += len;
		Temp_Buf[writeData.application_data_len] = 0x0e;
		writeData.application_data_len += 1;
		for (int y = 0; y < 8; y++)
		{
			if (m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[y][x].time_hours == 0 &&
				m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[y][x].time_minutes == 0)
			{
				continue;
			}
			Temp_Buf[writeData.application_data_len] = 0xb4;
			writeData.application_data_len += 1;
			Temp_Buf[writeData.application_data_len] = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[y][x].time_hours;
			writeData.application_data_len += 1;
			Temp_Buf[writeData.application_data_len] = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[y][x].time_minutes;
			writeData.application_data_len += 1;
			Temp_Buf[writeData.application_data_len] = 0;//sec
			writeData.application_data_len += 1;
			Temp_Buf[writeData.application_data_len] = 0;//millSec
			writeData.application_data_len += 1;
			Temp_Buf[writeData.application_data_len] = 0x91;//valueType
			writeData.application_data_len += 1;
			int flag = 1;
			if (y % 2)
				flag = 0;

			Temp_Buf[writeData.application_data_len] = flag;//(uint8_t)m_Schedual_time_flag.at(weekly_list_line).Time_flag[row][col - 1];//valueType
			writeData.application_data_len += 1;
		}
		Temp_Buf[writeData.application_data_len] = 0x0f;
		writeData.application_data_len += 1;
		writeData.application_data = &Temp_Buf[0];
		/*len = encode_closing_tag(writeData->application_data, 0);
		writeData->application_data_len += len;*/
	}
	//len = encode_closing_tag(writeData->application_data, 3);
	//writeData->application_data_len += len;
	int invoke_id = Bacnet_Write_Properties(g_bac_instance, writeData.object_type, writeData.object_instance, writeData.object_property, NULL, 16, &writeData);
	/*writeData.application_data = NULL;
	delete writeData;
	writeData = NULL;*/
	Sleep(10);
}
void CBacnetScheduleTime::OnBnClickedClearSchedual()
{
    if (IDYES == MessageBox(_T("Are you sure you want clear the schedule ?"), _T("Confirm"), MB_YESNO))
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                m_Schedual_time_flag.at(weekly_list_line).Time_flag[i][j] = 255;
                m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_hours = 0;
                m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[i][j].time_minutes = 0;


            }
        }

        if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, weekly_list_line, weekly_list_line) > 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule part 1 success!"));
        }
        //CString temp_task_info;
        //temp_task_info.Format(_T("Write Schedule Time Item%d .Clear schedule "), weekly_list_line + 1);
        //Post_Write_Message(g_bac_instance, WRITETIMESCHEDULE_T3000, weekly_list_line, weekly_list_line, WEEKLY_SCHEDULE_SIZE, BacNet_hwd, temp_task_info);
        //PostMessage(WM_REFRESH_BAC_SCHEDULE_LIST, NULL, NULL);

        memset(m_Schedual_time_flag.at(weekly_list_line).Time_flag, 255, 72);
        if (Write_Private_Data_Blocking(WRITETIMESCHEDULE_T3000, weekly_list_line, weekly_list_line) > 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule part 2 success!"));
            PostMessage(WM_REFRESH_BAC_SCHEDULE_LIST, NULL, NULL);
        }

    }
}

void CBacnetScheduleTime::OnBnClickedCopyMon_Fri()
{
	for (int i=1;i<5;i++)
	{
		for (int j=0;j<8;j++)
		{
			m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[j][i].time_hours = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[j][0].time_hours;
			m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[j][i].time_minutes = m_Schedual_Time_data.at(weekly_list_line).Schedual_Day_Time[j][0].time_minutes;


             m_Schedual_time_flag.at(weekly_list_line).Time_flag[j][i] = m_Schedual_time_flag.at(weekly_list_line).Time_flag[j][0];

		}
	}
	PostMessage(WM_REFRESH_BAC_SCHEDULE_LIST,NULL,NULL);

	CString temp_task_info;
	temp_task_info.Format(_T("Write Schedule Time "));
	Post_Write_Message(g_bac_instance,WRITETIMESCHEDULE_T3000,weekly_list_line,weekly_list_line,WEEKLY_SCHEDULE_SIZE,BacNet_hwd,temp_task_info);





    if (Write_Private_Data_Blocking(WRITE_SCHEDUAL_TIME_FLAG, weekly_list_line, weekly_list_line) > 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change schedule success!"));
    }

}



void CBacnetScheduleTime::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		KillTimer(1);
		m_schedual_time_picker.Invalidate();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
