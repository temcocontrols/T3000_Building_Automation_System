// BacnetTstatSchedule.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetTstatSchedule.h"
#include "global_function.h"
#include "afxdialogex.h"

CString ScheduleName[BAC_WEEKLYCODE_ROUTINES_COUNT + 1]; //用来存放Schedule的lable
// CBacnetTstatSchedule 对话框

IMPLEMENT_DYNAMIC(CBacnetTstatSchedule, CDialogEx)

CBacnetTstatSchedule::CBacnetTstatSchedule(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_TSTAT_SCHEDULE, pParent)
{

}

CBacnetTstatSchedule::~CBacnetTstatSchedule()
{
}

void CBacnetTstatSchedule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BAC_TSTAT_SCHEDULE, m_bac_tstat_sch_list);
}


BEGIN_MESSAGE_MAP(CBacnetTstatSchedule, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_TSTAT_SCHEDULE_LIST, Fresh_TSTAT_Schedule_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_TSTAT_Schedule_Item)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAC_TSTAT_SCHEDULE, &CBacnetTstatSchedule::OnNMClickListBacTstatSchedule)
END_MESSAGE_MAP()


// CBacnetTstatSchedule 消息处理程序


BOOL CBacnetTstatSchedule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Initial_List();
	Fresh_TSTAT_Schedule_List(NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CBacnetTstatSchedule::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CBacnetTstatSchedule::Fresh_TSTAT_Schedule_List(WPARAM wParam, LPARAM lParam)
{
		ListCtrlEx::CStrList strlist;
		strlist.clear();
		strlist.push_back(ScheduleName[0]);
		for (int i = 0;i<BAC_WEEKLYCODE_ROUTINES_COUNT;i++)
		{
			MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label) + 1,
				ScheduleName[i+1].GetBuffer(MAX_PATH), MAX_PATH);
			ScheduleName[i+1].ReleaseBuffer();
			if (ScheduleName[i+1].Trim().IsEmpty())
			{
				ScheduleName[i+1].Format(_T("Schedule%d"), i + 1);
			}
			strlist.push_back(ScheduleName[i+1]);
		}
	


	for (int i = 0;i<(int)m_tatat_schedule_data.size();i++)
	{
		if (m_tatat_schedule_data.at(i).tstat.id == 0)
		{
			for (int x = i; x < BAC_TSTAT_SCHEDULE; x++)
			{
				for (int a = 0;a<TSTAT_SCHEDULE_COL_NUMBER; a++)
				{
					m_bac_tstat_sch_list.SetItemText(x, a, _T(""));
				}
			}

			break;
		}

		m_bac_tstat_sch_list.SetCellStringList(i, TSTAT_SCHEDULE_SCHEDULES, strlist);

		CString temp_id;
		CString temp_online_status;
		temp_id.Format(_T("%d"), m_tatat_schedule_data.at(i).tstat.id);
		if (m_tatat_schedule_data.at(i).tstat.on_line)
		{
			temp_online_status = _T("Online");
		}
		else
		{
			temp_online_status = _T("Offline");
		}

		CString temp_name;
		MultiByteToWideChar(CP_ACP, 0, (char *)m_tatat_schedule_data.at(i).tstat.name, (int)strlen((char *)m_tatat_schedule_data.at(i).tstat.name) + 1,
			temp_name.GetBuffer(MAX_PATH), MAX_PATH);
		temp_name.ReleaseBuffer();
		if (temp_name.Trim().IsEmpty())
		{
			temp_name = _T("TSTAT");
		}
		m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_DEVICENAME, temp_name);

		bitset<8> char_flag ((unsigned char)m_tatat_schedule_data.at(i).tstat.flag);
		bool b_am = char_flag.test(7);
		bool b_value = char_flag.test(6);
		bool b_schedule_status = char_flag.test(5);

		m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_DEVICEID, temp_id);
		m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_ONLINE_STATUS, temp_online_status);

		if(b_am)
			m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES_AM, _T("Manual"));
		else
			m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES_AM, _T("Auto"));

		if(b_value)
			m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES_VALUE, _T("ON"));
		else
			m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES_VALUE, _T("OFF"));

		if ((m_tatat_schedule_data.at(i).tstat.schedule > BAC_WEEKLYCODE_ROUTINES_COUNT) ||
			m_tatat_schedule_data.at(i).tstat.schedule == 0)
		{
			m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES, ScheduleName[0]);	
		}
		else
		{
			m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES, ScheduleName[m_tatat_schedule_data.at(i).tstat.schedule]);
		}
	}


	return true;
}

LRESULT CBacnetTstatSchedule::Fresh_TSTAT_Schedule_Item(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	if (Changed_SubItem == TSTAT_SCHEDULE_SCHEDULES)
	{
		CString temp_cs = m_bac_tstat_sch_list.GetItemText(Changed_Item, Changed_SubItem);
		for (int i = 0;i < 9;i++) //改变后判断是变化为哪一个
		{
			if (temp_cs.CompareNoCase(ScheduleName[i]) == 0)
			{
				m_tatat_schedule_data.at(Changed_Item).tstat.schedule = i;
				break;
			}
		}
	}
	CString temp_task_info;
	//temp_task_info.Format(_T("Write  Item%d .Changed to \"%s\" "), Changed_Item + 1, New_CString);
	Post_Write_Message(g_bac_instance, WRITE_TSTATE_SCHEDULE_T3000, Changed_Item, Changed_Item, sizeof(Str_tstat_schedule), m_tstat_schedule_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);

	return true;
}

void CBacnetTstatSchedule::Initial_List()
{
	m_bac_tstat_sch_list.ShowWindow(SW_HIDE);
	m_bac_tstat_sch_list.DeleteAllItems();
	while (m_bac_tstat_sch_list.DeleteColumn(0));

	m_bac_tstat_sch_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_bac_tstat_sch_list.SetExtendedStyle(m_bac_tstat_sch_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_DEVICEID, _T("Device ID"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_DEVICENAME, _T("Name"), 110, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_ONLINE_STATUS, _T("Online"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES, _T("Schedule"), 120, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	//m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES_STATUS, _T("Status"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES_AM, _T("Auto/Manual"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES_VALUE, _T("Value"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);


	m_tstat_schedule_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_controller_dlg_hwnd;
	m_bac_tstat_sch_list.SetListHwnd(this->m_hWnd);

	CRect list_rect, win_rect;
	m_bac_tstat_sch_list.GetWindowRect(list_rect);
	//ScreenToClient(&list_rect);
	//::GetWindowRect(m_controller_dlg_hwnd, win_rect);
	//m_bac_tstat_sch_list.Set_My_WindowRect(win_rect);
	//m_bac_tstat_sch_list.Set_My_ListRect(list_rect);

	m_bac_tstat_sch_list.SetWhetherShowBkCol(false);
	m_bac_tstat_sch_list.DeleteAllItems();
	for (int i = 0;i<(int)m_tatat_schedule_data.size();i++)
	{

		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		temp_item.Format(_T("%d"), i + 1);
		m_bac_tstat_sch_list.InsertItem(i, temp_item);
		//m_bac_tstat_sch_list.SetCellEnabled(i, CONTROLLER_INPUTUNITS, 0);
		//m_bac_tstat_sch_list.SetCellEnabled(i, CONTROLLER_SETPOINTUNITS, 0);
#if 0
		if (ListCtrlEx::ComboBox == m_bac_tstat_sch_list.GetColumnType(TSTAT_SCHEDULE_SCHEDULES))
		{
			ListCtrlEx::CStrList strlist;

			for (int i = 0;i<(int)sizeof(PID_Time_Type) / sizeof(PID_Time_Type[0]);i++)
			{
				strlist.push_back(PID_Time_Type[i]);
			}
			m_bac_tstat_sch_list.SetCellStringList(i, CONTROLLER_I_TIME, strlist);
		}


		for (int x = 0;x<CONTROLLER_COL_NUMBER;x++)
		{
			if ((i % 2) == 0)
				m_bac_tstat_sch_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_bac_tstat_sch_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
		}
#endif
	}
	m_bac_tstat_sch_list.InitListData();
	m_bac_tstat_sch_list.ShowWindow(SW_SHOW);
}


void CBacnetTstatSchedule::OnNMClickListBacTstatSchedule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	long lRow, lCol;
	m_bac_tstat_sch_list.Set_Edit(true);
	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_bac_tstat_sch_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_bac_tstat_sch_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if (lRow>m_bac_tstat_sch_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if (lRow<0)
		return;

	if (lCol == TSTAT_SCHEDULE_SCHEDULES_AM)
	{
		bitset<8> char_flag((unsigned char)m_tatat_schedule_data.at(lRow).tstat.flag);
		bool b_am = char_flag.test(7);
		bool b_value = char_flag.test(6);
		bool b_schedule_status = char_flag.test(5);

		if (b_am)
		{
			m_tatat_schedule_data.at(lRow).tstat.flag &= 0x7F;
			m_bac_tstat_sch_list.SetItemText(lRow, TSTAT_SCHEDULE_SCHEDULES_AM, _T("Auto"));
		}
		else //是Manual 就改auto
		{
			m_tatat_schedule_data.at(lRow).tstat.flag |= 0x80;
			m_bac_tstat_sch_list.SetItemText(lRow, TSTAT_SCHEDULE_SCHEDULES_AM, _T("Manual"));
		}
	}
	else if (lCol == TSTAT_SCHEDULE_SCHEDULES_VALUE)
	{
		CString temp_am = m_bac_tstat_sch_list.GetItemText(lRow, TSTAT_SCHEDULE_SCHEDULES_AM);
		if (temp_am.CompareNoCase(_T("Auto")) == 0)  //Auto 的时候是不允许手动改 value 值.
		{
			return;
		}
		else
		{
			bitset<8> char_flag((unsigned char)m_tatat_schedule_data.at(lRow).tstat.flag);
			bool b_value = char_flag.test(6);
			if (b_value)
			{
				m_tatat_schedule_data.at(lRow).tstat.flag &= 0xBF;
				m_bac_tstat_sch_list.SetItemText(lRow, TSTAT_SCHEDULE_SCHEDULES_VALUE, _T("OFF"));
			}
			else
			{
				m_tatat_schedule_data.at(lRow).tstat.flag |= 0x40;
				m_bac_tstat_sch_list.SetItemText(lRow, TSTAT_SCHEDULE_SCHEDULES_VALUE, _T("ON"));
			}
		}
	}
	
	Post_Write_Message(g_bac_instance, WRITE_TSTATE_SCHEDULE_T3000, lRow, lRow, sizeof(Str_tstat_schedule), m_tstat_schedule_dlg_hwnd, _T(""), lRow, lCol);

	*pResult = 0;
}
