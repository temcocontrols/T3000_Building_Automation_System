// BacnetTstatSchedule.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetTstatSchedule.h"
#include "global_function.h"
#include "afxdialogex.h"

HANDLE h_write_group_thread = NULL;
HANDLE h_read_group_thread = NULL;
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
    ON_WM_TIMER()
    //ON_NOTIFY(NM_DBLCLK, IDC_LIST_BAC_TSTAT_SCHEDULE, &CBacnetTstatSchedule::OnNMDblclkListBacTstatSchedule)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_BAC_TSTAT_SCHEDULE, &CBacnetTstatSchedule::OnNMRClickListBacTstatSchedule)
END_MESSAGE_MAP()


// CBacnetTstatSchedule 消息处理程序


BOOL CBacnetTstatSchedule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Initial_List();
	Fresh_TSTAT_Schedule_List(NULL, NULL);
    SetWindowText(_T("Group Schedule Setting"));

    //使list的长宽 和对话框一样大;
    CRect windowrect;
    GetWindowRect(windowrect);
    ::SetWindowPos(m_bac_tstat_sch_list.m_hWnd, NULL, windowrect.left, windowrect.top, windowrect.Width() - 20, windowrect.Height(), SWP_NOMOVE);

    SetTimer(1, 7000, NULL);
    ::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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
    for (int i = 0;i < BAC_WEEKLYCODE_ROUTINES_COUNT;i++)
    {
        MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label) + 1,
            ScheduleName[i + 1].GetBuffer(MAX_PATH), MAX_PATH);
        ScheduleName[i + 1].ReleaseBuffer();
        if (ScheduleName[i + 1].Trim().IsEmpty())
        {
            ScheduleName[i + 1].Format(_T("Schedule%d"), i + 1);
        }
        strlist.push_back(ScheduleName[i + 1]);
    }

    m_list_count = 0;

    for (int i = 0;i < (int)m_tatat_schedule_data.size();i++)
    {
        if (m_tatat_schedule_data.at(i).tstat.id == 0)
        {
            for (int x = i; x < BAC_TSTAT_SCHEDULE; x++)
            {
                for (int a = 0;a < TSTAT_SCHEDULE_COL_NUMBER; a++)
                {
                    m_bac_tstat_sch_list.SetItemText(x, a, _T(""));
                }
            }

            break;
        }

        m_bac_tstat_sch_list.SetCellStringList(i, TSTAT_SCHEDULE_SCHEDULES, strlist);

        CString temp_id;
        CString temp_online_status;
        temp_id.Format(_T("%u"), (unsigned char)m_tatat_schedule_data.at(i).tstat.id);
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

        bitset<8> char_flag((unsigned char)m_tatat_schedule_data.at(i).tstat.flag);
        bool b_am = char_flag.test(7);
        bool b_value = char_flag.test(6);
        bool b_schedule_status = char_flag.test(5);

        m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_DEVICEID, temp_id);
        m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_ONLINE_STATUS, temp_online_status);

        if (b_am)
            m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES_AM, _T("Manual"));
        else
            m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SCHEDULES_AM, _T("Auto"));

        if (b_value)
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

        CString temp_daysetpoint;
        CString temp_nightsetpoint;
        CString temp_awakesetpoint;
        CString temp_sleepsetpoint;
        temp_daysetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(i).tstat.daysetpoint) / 10);
        temp_nightsetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(i).tstat.nightsetpoint) / 10);
        temp_awakesetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(i).tstat.awakesetpoint) / 10);
        temp_sleepsetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(i).tstat.sleepsetpoint) / 10);
        if (m_tatat_schedule_data.at(i).tstat.daysetpoint == 0)
            temp_daysetpoint = _T("Reading");
        if (m_tatat_schedule_data.at(i).tstat.nightsetpoint == 0)
            temp_nightsetpoint = _T("Reading");

        m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_DSP_VALUE, temp_daysetpoint);
        m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_NSP_VALUE, temp_nightsetpoint);
        m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_AWAKESP_VALUE, temp_awakesetpoint);
        m_bac_tstat_sch_list.SetItemText(i, TSTAT_SCHEDULE_SLEEPSP_VALUE, temp_sleepsetpoint);

        m_list_count++;
    }

    return true;
}

LRESULT CBacnetTstatSchedule::Fresh_TSTAT_Schedule_Item(WPARAM wParam, LPARAM lParam)
{
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
     unsigned short write_sp_value = 0;
    CString temp_cs = m_bac_tstat_sch_list.GetItemText(Changed_Item, Changed_SubItem);
    if (Changed_SubItem == TSTAT_SCHEDULE_SCHEDULES)
    {
        for (int i = 0;i < 9;i++) //改变后判断是变化为哪一个
        {
            if (temp_cs.CompareNoCase(ScheduleName[i]) == 0)
            {
                m_tatat_schedule_data.at(Changed_Item).tstat.schedule = i;
                break;
            }
        }
    }

    
    //Fandu 2013-12-13  在变更 setpoint 时 ，需要优先校验值的取值范围。
    if (Changed_SubItem == TSTAT_SCHEDULE_DSP_VALUE)
    {
        int temp_int = (int)(_wtof(temp_cs) * 10);
        
        if ((temp_int > 65535) || (temp_int < 0))
        {
            CString temp_daysetpoint;
            temp_daysetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(Changed_Item).tstat.daysetpoint) / 10);
            m_bac_tstat_sch_list.SetItemText(Changed_Item, TSTAT_SCHEDULE_DSP_VALUE, temp_daysetpoint);
            return false;
        }
        write_sp_value = temp_int;
        m_tatat_schedule_data.at(Changed_Item).tstat.daysetpoint = temp_int;
    }
    else if (Changed_SubItem == TSTAT_SCHEDULE_NSP_VALUE)
    {
        unsigned short temp_int = (int)(_wtof(temp_cs) * 10);
        if ((temp_int > 65535) || (temp_int < 0))
        {
            CString temp_nightsetpoint;
            temp_nightsetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(Changed_Item).tstat.nightsetpoint) / 10);
            m_bac_tstat_sch_list.SetItemText(Changed_Item, TSTAT_SCHEDULE_NSP_VALUE, temp_nightsetpoint);
            return false;
        }
        write_sp_value = temp_int;
        m_tatat_schedule_data.at(Changed_Item).tstat.nightsetpoint = temp_int;
    }
    else if (Changed_SubItem == TSTAT_SCHEDULE_AWAKESP_VALUE)
    {
        unsigned short temp_int = (int)(_wtof(temp_cs) * 10);
        if ((temp_int > 65535) || (temp_int < 0))
        {
            CString temp_awakesetpoint;
            temp_awakesetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(Changed_Item).tstat.awakesetpoint) / 10);
            m_bac_tstat_sch_list.SetItemText(Changed_Item, TSTAT_SCHEDULE_AWAKESP_VALUE, temp_awakesetpoint);
            return false;
        }
        write_sp_value = temp_int;
        m_tatat_schedule_data.at(Changed_Item).tstat.awakesetpoint = temp_int;
    }
    else if (Changed_SubItem == TSTAT_SCHEDULE_SLEEPSP_VALUE)
    {
        unsigned short temp_int = (int)(_wtof(temp_cs) * 10);
        if ((temp_int > 65535) || (temp_int < 0))
        {
            CString temp_sleepsetpoint;
            temp_sleepsetpoint.Format(_T("%.1f"), ((float)m_tatat_schedule_data.at(Changed_Item).tstat.sleepsetpoint) / 10);
            m_bac_tstat_sch_list.SetItemText(Changed_Item, TSTAT_SCHEDULE_SLEEPSP_VALUE, temp_sleepsetpoint);
            return false;
        }
        write_sp_value = temp_int;
        m_tatat_schedule_data.at(Changed_Item).tstat.sleepsetpoint = temp_int;
    }


    if ((Changed_SubItem == TSTAT_SCHEDULE_DSP_VALUE) ||
        (Changed_SubItem == TSTAT_SCHEDULE_NSP_VALUE) ||
        (Changed_SubItem == TSTAT_SCHEDULE_AWAKESP_VALUE) ||
        (Changed_SubItem == TSTAT_SCHEDULE_SLEEPSP_VALUE))
    {
        if ((m_tatat_schedule_data.at(Changed_Item).tstat.schedule > 0) ||
            (m_tatat_schedule_data.at(Changed_Item).tstat.schedule <= 8))
        {
            SetTimer(1, 15000, NULL);
            if (h_write_group_thread == NULL)
            {
                m_group_value = write_sp_value;
                m_group_index = Changed_SubItem;

                m_group_id = m_tatat_schedule_data.at(Changed_Item).tstat.schedule; //保存需要改的group id.
                h_write_group_thread = CreateThread(NULL, NULL, WriteGroupRegThreadfun, this, NULL, NULL);
                return true;
            }
        }
    }



	CString temp_task_info;
	Post_Write_Message(g_bac_instance, WRITE_TSTATE_SCHEDULE_T3000, Changed_Item, Changed_Item, sizeof(Str_tstat_schedule), m_tstat_schedule_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);

	return true;
}

DWORD WINAPI  CBacnetTstatSchedule::WriteGroupRegThreadfun(LPVOID lpVoid)
{
    //Write_Config_Info 
    CBacnetTstatSchedule *pParent = (CBacnetTstatSchedule *)lpVoid;
    bool need_update_date[BAC_TSTAT_SCHEDULE] = {0x00};
    for (int i = 0; i < BAC_TSTAT_SCHEDULE; i++)
    {
        if (m_tatat_schedule_data.at(i).tstat.id == 0)
            continue;
        if (m_tatat_schedule_data.at(i).tstat.schedule == pParent->m_group_id)
        {
            need_update_date[i] = true;
            if(pParent->m_group_index == TSTAT_SCHEDULE_DSP_VALUE)
            {
                m_tatat_schedule_data.at(i).tstat.daysetpoint = pParent->m_group_value;
            }
            else if (pParent->m_group_index == TSTAT_SCHEDULE_NSP_VALUE)
            {
                m_tatat_schedule_data.at(i).tstat.nightsetpoint = pParent->m_group_value;
             }
            else if (pParent->m_group_index == TSTAT_SCHEDULE_AWAKESP_VALUE)
            {
                m_tatat_schedule_data.at(i).tstat.awakesetpoint = pParent->m_group_value;
            }
            else if (pParent->m_group_index == TSTAT_SCHEDULE_SLEEPSP_VALUE)
            {
                m_tatat_schedule_data.at(i).tstat.sleepsetpoint = pParent->m_group_value;
            }
        }
    }
    ::PostMessage(pParent->m_hWnd,WM_REFRESH_BAC_TSTAT_SCHEDULE_LIST, NULL, NULL);
    for (int x = 0;x < BAC_TSTAT_SCHEDULE;x++)
    {
        if (need_update_date[x])
        {
            if (Write_Private_Data_Blocking(WRITE_TSTATE_SCHEDULE_T3000, x, x) <= 0)
            {
                Sleep(1);
            }
        }
    }
    h_write_group_thread = NULL;
    return 1;
}

void CBacnetTstatSchedule::Initial_List()
{
	m_bac_tstat_sch_list.ShowWindow(SW_HIDE);
	m_bac_tstat_sch_list.DeleteAllItems();
	while (m_bac_tstat_sch_list.DeleteColumn(0));

	m_bac_tstat_sch_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_bac_tstat_sch_list.SetExtendedStyle(m_bac_tstat_sch_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_DEVICEID, _T("Device ID"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_DEVICENAME, _T("Thermostat Name"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_ONLINE_STATUS, _T("Online"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES, _T("Schedule"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES_AM, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SCHEDULES_VALUE, _T("Value"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);

	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_DSP_VALUE, _T("Day SP"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_NSP_VALUE, _T("Night SP"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_AWAKESP_VALUE, _T("Awake SP"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bac_tstat_sch_list.InsertColumn(TSTAT_SCHEDULE_SLEEPSP_VALUE, _T("Sleep SP"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

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
	if (lRow > m_list_count)
	{
		m_bac_tstat_sch_list.Set_Edit(false);
		return;
	}
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
	else
	{
		return;
	}
	
	Post_Write_Message(g_bac_instance, WRITE_TSTATE_SCHEDULE_T3000, lRow, lRow, sizeof(Str_tstat_schedule), m_tstat_schedule_dlg_hwnd, _T(""), lRow, lCol);

	*pResult = 0;
}


void CBacnetTstatSchedule::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent)
    {
    case 1:
        if (h_read_group_thread == NULL)
            h_read_group_thread = CreateThread(NULL, NULL, ReadGroupRegThreadfun, this, NULL, NULL);
        break;
    default:
        break;
    }


    CDialogEx::OnTimer(nIDEvent);
}

DWORD WINAPI  CBacnetTstatSchedule::ReadGroupRegThreadfun(LPVOID lpVoid)
{
    //Write_Config_Info 
    CBacnetTstatSchedule *pParent = (CBacnetTstatSchedule *)lpVoid;

    b_stop_read_tstat_schedule = false;  //是否继续读取标志，若后面数据为空则退出循环体.
    for (int i = 0;i<BAC_TSTAT_SCHEDULE_GROUP;i++)
    {
        int end_temp_instance = 0;
        int ret_n;
        end_temp_instance = BAC_TSTAT_SCHEDULE_REMAINDER + (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i;
        if (end_temp_instance >= BAC_TSTAT_SCHEDULE)
            end_temp_instance = BAC_TSTAT_SCHEDULE - 1;
        CString n_temp_result;
        ret_n = GetPrivateData_Blocking(g_bac_instance, READ_TSTATE_SCHEDULE_T3000, (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i, end_temp_instance, sizeof(Str_tstat_schedule));
        if (ret_n)
        {
            //n_temp_result.Format(_T("Read Tstat From %d to %d success."), (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i, end_temp_instance);
            //SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
            Sleep(10);
            if (b_stop_read_tstat_schedule)
            {
                //n_temp_result.Format(_T("Reading tstat schedule complete"));
                //SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
                Sleep(10);
                break;
            }
        }
        else
        {
            //n_temp_result.Format(_T("Reading tstat schedule From %d to %d failed."), (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i, end_temp_instance);
            //SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
            h_read_group_thread = NULL;
            return true;
        }
    }

    h_read_group_thread = NULL;
    return true;
}


//void CBacnetTstatSchedule::OnNMDblclkListBacTstatSchedule(NMHDR *pNMHDR, LRESULT *pResult)
//{
//    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//    // TODO: 在此添加控件通知处理程序代码
//
//
//    //for (int i = 0;i<m_weeklyr_list.GetItemCount();++i)
//    //{
//    //    if (m_weeklyr_list.GetCellChecked(i, 0))
//    //    {
//    //        weekly_list_line = i;
//    //        break;
//    //    }
//    //}
//
//    weekly_list_line = 2;
//
//    ::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_WEEKLYCODE);
//
//
//    *pResult = 0;
//
//
//}


void CBacnetTstatSchedule::OnNMRClickListBacTstatSchedule(NMHDR *pNMHDR, LRESULT *pResult)
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
    if (lRow > m_list_count)
    {
        m_bac_tstat_sch_list.Set_Edit(false);
        return;
    }
    if (m_tatat_schedule_data.at(lRow).tstat.schedule == 0)
        return;
    weekly_list_line = m_tatat_schedule_data.at(lRow).tstat.schedule - 1;
    //weekly_list_line = 0;

    ::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_WEEKLYCODE);

    *pResult = 0;
}
