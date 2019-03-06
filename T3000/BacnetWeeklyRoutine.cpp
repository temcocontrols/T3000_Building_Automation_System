// BacnetWeeklyRoutine.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWeeklyRoutine.h"
#include "afxdialogex.h"



#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetRange.h"
#include "BacnetScheduleTime.h"
#include "BacnetTstatSchedule.h"
extern CBacnetTstatSchedule *BacnetTstatSchedule_Window ;
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.



CString Hol_Old_Lable[BAC_HOLIDAY_COUNT] =
{
    _T("AR1"),
    _T("AR2"),
    _T("AR3"),
    _T("AR4")
};
// BacnetWeeklyRoutine dialog

IMPLEMENT_DYNAMIC(BacnetWeeklyRoutine, CDialogEx)

BacnetWeeklyRoutine::BacnetWeeklyRoutine(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetWeeklyRoutine::IDD, pParent)
{
	weekly_list_line = 0;
	window_max = true;
}

BacnetWeeklyRoutine::~BacnetWeeklyRoutine()
{
}

void BacnetWeeklyRoutine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BAC_WEEKLY, m_weeklyr_list);
}


BEGIN_MESSAGE_MAP(BacnetWeeklyRoutine, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, WeeklyMessageCallBack)
	ON_MESSAGE(WM_HOTKEY,&BacnetWeeklyRoutine::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(WM_REFRESH_BAC_WEEKLY_LIST,Fresh_Weekly_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Weekly_Routine_Item)

	ON_BN_CLICKED(IDC_BUTTON_WEEKLY_EDIT, &BacnetWeeklyRoutine::OnBnClickedButtonWeeklyScheduleEdit)
    ON_BN_CLICKED(IDC_BUTTON_SCHEDULE_SUBNET, &BacnetWeeklyRoutine::OnBnClickedButtonWeeklysubPanelScheduleEdit)
	//ON_MESSAGE(MY_RESUME_DATA, WeeklyResumeMessageCallBack)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAC_WEEKLY, &BacnetWeeklyRoutine::OnNMClickListBacWeekly)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BAC_WEEKLY, &BacnetWeeklyRoutine::OnNMDblclkListWeeklySchedule)
END_MESSAGE_MAP()


// BacnetWeeklyRoutine message handlers

LRESULT  BacnetWeeklyRoutine::WeeklyMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	else
	{
		memcpy_s(&m_Weekly_data.at(pInvoke->mRow),sizeof(Str_weekly_routine_point),&m_temp_weekly_data[pInvoke->mRow],sizeof(Str_weekly_routine_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_weeklyr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_weeklyr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_weeklyr_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL BacnetWeeklyRoutine::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_weeklyr_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_weekly_dlg_hwnd,win_rect);
		m_weeklyr_list.Set_My_WindowRect(win_rect);
		m_weeklyr_list.Set_My_ListRect(list_rect);
		m_weeklyr_list.Get_clicked_mouse_position();
		return TRUE;
	}
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 60 ,temp_mynew_rect.top + 60,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void BacnetWeeklyRoutine::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void BacnetWeeklyRoutine::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}


//const int KEY_INSERT = 1020;
LRESULT BacnetWeeklyRoutine::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		OnBnClickedButtonWeeklyScheduleEdit();
	}
	return 0;
}

BOOL BacnetWeeklyRoutine::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("SCHEDULE"));
	
	Initial_List();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_SCHEDUAL);
	SetIcon(m_hIcon,TRUE);
	ShowWindow(FALSE);
	PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

#include "BacnetAnnualRoutine.h"

void BacnetWeeklyRoutine::Initial_List()
{
	m_weeklyr_list.ShowWindow(SW_HIDE);
	m_weeklyr_list.DeleteAllItems();
	while ( m_weeklyr_list.DeleteColumn (0)) ;

	m_weeklyr_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_weeklyr_list.SetExtendedStyle(m_weeklyr_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_FULL_LABLE, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_AUTO_MANUAL, _T("Auto/Manual"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_OUTPUT, _T("Output"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_HOLIDAY1, _T("Holiday1"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_STATE1, _T("State1"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_HOLIDAY2, _T("Holiday2"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_STATE2, _T("State2"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_LABEL, _T("Label"), 90, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_weeklyr_list.Setlistcolcharlimit(WEEKLY_ROUTINE_FULL_LABLE,STR_WEEKLY_DESCRIPTION_LENGTH -1);
	m_weeklyr_list.Setlistcolcharlimit(WEEKLY_ROUTINE_LABEL,STR_WEEKLY_LABEL_LENGTH-1);

	m_weekly_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_weekly_dlg_hwnd;
	m_weeklyr_list.SetListHwnd(this->m_hWnd);

	CRect list_rect,win_rect;
	m_weeklyr_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_weekly_dlg_hwnd,win_rect);
	m_weeklyr_list.Set_My_WindowRect(win_rect);
	m_weeklyr_list.Set_My_ListRect(list_rect);


	m_weeklyr_list.DeleteAllItems();
	for (int i=0;i<(int)m_Weekly_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_weeklyr_list.InsertItem(i,temp_item);
		m_weeklyr_list.SetCellEnabled(i,WEEKLY_ROUTINE_NUM,0);
		if(ListCtrlEx::ComboBox == m_weeklyr_list.GetColumnType(WEEKLY_ROUTINE_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Auto"));
			strlist.push_back(_T("Manual"));
			m_weeklyr_list.SetCellStringList(i, WEEKLY_ROUTINE_AUTO_MANUAL, strlist);
		}


		if(ListCtrlEx::ComboBox == m_weeklyr_list.GetColumnType(WEEKLY_ROUTINE_OUTPUT))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("OFF"));
			strlist.push_back(_T("ON"));
			m_weeklyr_list.SetCellStringList(i, WEEKLY_ROUTINE_OUTPUT, strlist);
		}


		if(ListCtrlEx::ComboBox == m_weeklyr_list.GetColumnType(WEEKLY_ROUTINE_HOLIDAY1))
		{
			ListCtrlEx::CStrList strlist;
			CString temp1;
			strlist.push_back(_T(" "));
            for (int x = 1;x <= BAC_HOLIDAY_COUNT;x++)
            {

                temp1.Format(_T("AR%d"), x);
                strlist.push_back(temp1);
            }
			m_weeklyr_list.SetCellStringList(i, WEEKLY_ROUTINE_HOLIDAY1, strlist);
		}

		if(ListCtrlEx::ComboBox == m_weeklyr_list.GetColumnType(WEEKLY_ROUTINE_HOLIDAY2))
		{
			ListCtrlEx::CStrList strlist;
			CString temp1;
			strlist.push_back(_T(" "));
            for (int x = 1;x <= BAC_HOLIDAY_COUNT;x++)
            {
                temp1.Format(_T("AR%d"), x);
                strlist.push_back(temp1);
            }
			m_weeklyr_list.SetCellStringList(i, WEEKLY_ROUTINE_HOLIDAY2, strlist);
		}

		for (int x=0;x<WEEKLY_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_weeklyr_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_weeklyr_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_weeklyr_list.InitListData();
	m_weeklyr_list.SetCellChecked(0,0,1);
	m_weeklyr_list.ShowWindow(SW_SHOW);
}

LRESULT BacnetWeeklyRoutine::Fresh_Weekly_Routine_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_weeklyr_list.GetItemText(Changed_Item,Changed_SubItem);
	memcpy_s(&m_temp_weekly_data[Changed_Item],sizeof(Str_weekly_routine_point),&m_Weekly_data.at(Changed_Item),sizeof(Str_weekly_routine_point));

	if(Changed_SubItem == WEEKLY_ROUTINE_LABEL)
	{
		CString cs_temp = m_weeklyr_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_WEEKLY_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Weekly_data.at(Changed_Item).label,STR_WEEKLY_LABEL_LENGTH,cTemp1,STR_WEEKLY_LABEL_LENGTH);
	}

	if(Changed_SubItem == WEEKLY_ROUTINE_FULL_LABLE)
	{
		CString cs_temp = m_weeklyr_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_WEEKLY_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
			return 0;
		}
		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Weekly_data.at(Changed_Item).description,STR_WEEKLY_DESCRIPTION_LENGTH,cTemp1,STR_WEEKLY_DESCRIPTION_LENGTH);
	}

	if(Changed_SubItem == WEEKLY_ROUTINE_OUTPUT)
	{
		CString cs_temp=m_weeklyr_list.GetItemText(Changed_Item,WEEKLY_ROUTINE_OUTPUT);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Weekly_data.at(Changed_Item).value=0;
		}
		else
		{
			m_Weekly_data.at(Changed_Item).value=1;
		}
	}
	if(Changed_SubItem == WEEKLY_ROUTINE_AUTO_MANUAL)
	{
		CString cs_temp=m_weeklyr_list.GetItemText(Changed_Item,WEEKLY_ROUTINE_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Weekly_data.at(Changed_Item).auto_manual=0;
			m_weeklyr_list.SetCellEnabled(Changed_Item,WEEKLY_ROUTINE_OUTPUT,0);
		}
		else
		{
			m_Weekly_data.at(Changed_Item).auto_manual=1;
			m_weeklyr_list.SetCellEnabled(Changed_Item,WEEKLY_ROUTINE_OUTPUT,1);
		}
	}

	if(Changed_SubItem == WEEKLY_ROUTINE_HOLIDAY1)
	{
		CString cs_temp=m_weeklyr_list.GetItemText(Changed_Item,WEEKLY_ROUTINE_HOLIDAY1);
		if(cs_temp.CompareNoCase(_T(" ")) == 0 )
		{
			m_Weekly_data.at(Changed_Item).override_1.panel = 0;
			m_Weekly_data.at(Changed_Item).override_1.point_type = 0;	
			m_Weekly_data.at(Changed_Item).override_1.number = 0;
		}
		else
		{
            int sel_index = 1;
            for (int x = 0; x < BAC_HOLIDAY_COUNT; x++)
            {
                if (cs_temp.CompareNoCase(HolLable[x]) == 0)
                    sel_index = x + 1;
            }
            
			cs_temp = cs_temp.Right(1);
			int temp_value = _wtoi(cs_temp);
			m_Weekly_data.at(Changed_Item).override_1.panel = Station_NUM;
			m_Weekly_data.at(Changed_Item).override_1.point_type = BAC_HOL + 1;	
			m_Weekly_data.at(Changed_Item).override_1.number = temp_value - 1;
		}

		
	}

	if(Changed_SubItem == WEEKLY_ROUTINE_HOLIDAY2)
	{
		CString cs_temp=m_weeklyr_list.GetItemText(Changed_Item,WEEKLY_ROUTINE_HOLIDAY2);
		if(cs_temp.CompareNoCase(_T(" ")) == 0 )
		{
			m_Weekly_data.at(Changed_Item).override_2.panel = 0;
			m_Weekly_data.at(Changed_Item).override_2.point_type = 0;	
			m_Weekly_data.at(Changed_Item).override_2.number = 0;
		}
		else
		{
            int sel_index = 1;
            for (int x = 0; x < BAC_HOLIDAY_COUNT; x++)
            {
                if (cs_temp.CompareNoCase(HolLable[x]) == 0)
                    sel_index = x + 1;
            }

            cs_temp = cs_temp.Right(1);
            int temp_value = _wtoi(cs_temp);
            m_Weekly_data.at(Changed_Item).override_2.panel = Station_NUM;
            m_Weekly_data.at(Changed_Item).override_2.point_type = BAC_HOL + 1;
            m_Weekly_data.at(Changed_Item).override_2.number = temp_value - 1;
		}

	}

	cmp_ret = memcmp(&m_temp_weekly_data[Changed_Item],&m_Weekly_data.at(Changed_Item),sizeof(Str_weekly_routine_point));
	if(cmp_ret!=0)
	{
			m_weeklyr_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Weekly Routine List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(g_bac_instance,WRITESCHEDULE_T3000,Changed_Item,Changed_Item,sizeof(Str_weekly_routine_point),m_weekly_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}

LRESULT BacnetWeeklyRoutine::Fresh_Weekly_List(WPARAM wParam,LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;

    bool reset_combo_cell = false;
    for (int i = 0; i < BAC_HOLIDAY_COUNT; i++)
    {
        if (HolLable[i].CompareNoCase(Hol_Old_Lable[i]) != 0)
        {
            Hol_Old_Lable[i] = HolLable[i];
            reset_combo_cell = true;
        }
        else
        {
            continue;
        }
    }


	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_weeklyr_list.IsDataNewer((char *)&m_Weekly_data.at(0),sizeof(Str_weekly_routine_point) * BAC_SCHEDULE_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_weeklyr_list.SetListData((char *)&m_Weekly_data.at(0),sizeof(Str_weekly_routine_point) * BAC_SCHEDULE_COUNT);
		}
		else
		{
			//return 0;
		}
	}


	for (int i=0;i<(int)m_Weekly_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

        if (reset_combo_cell)
        {
            ListCtrlEx::CStrList strlist;
            CString temp1;
            strlist.push_back(_T(" "));
            for (int x = 1;x <= BAC_HOLIDAY_COUNT;x++)
            {
                strlist.push_back(HolLable[x-1]);
            }
            m_weeklyr_list.SetCellStringList(i, WEEKLY_ROUTINE_HOLIDAY1, strlist);
        }

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).description, (int)strlen((char *)m_Weekly_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_FULL_LABLE,temp_des);
		if(m_Weekly_data.at(i).auto_manual==0)
		{
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_AUTO_MANUAL,_T("Auto"));
			m_weeklyr_list.SetCellEnabled(i,WEEKLY_ROUTINE_OUTPUT,0);
		}
		else
		{
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_AUTO_MANUAL,_T("Manual"));
			m_weeklyr_list.SetCellEnabled(i,WEEKLY_ROUTINE_OUTPUT,1);
		}


		//uint8_t value ;  /* (1 bit; 0=off, 1=on)*/
		//uint8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
		//uint8_t override_1_value;  /* (1 bit; 0=off, 1=on)*/
		//uint8_t override_2_value;  /* (1 bit; 0=off, 1=on)*/
		//uint8_t off  ;
		//uint8_t unused	; /* (11 bits)*/
		if(m_Weekly_data.at(i).value==0)
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_OUTPUT,_T("OFF"));
		else
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_OUTPUT,_T("ON"));

		if(m_Weekly_data.at(i).override_1_value == 0)
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_STATE1,_T("OFF"));
		else
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_STATE1,_T("ON"));

		if(m_Weekly_data.at(i).override_2_value == 0)
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_STATE2,_T("OFF"));
		else
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_STATE2,_T("ON"));

		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_LABEL,temp_des2);


		if((m_Weekly_data.at(i).override_1.point_type == BAC_HOL + 1) && (m_Weekly_data.at(i).override_1.number < BAC_HOLIDAY_COUNT))
		{
				//CString temp_str;
				//temp_str.Format(_T("AR%d"),m_Weekly_data.at(i).override_1.number + 1);
				//m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_HOLIDAY1,temp_str);
                m_weeklyr_list.SetItemText(i, WEEKLY_ROUTINE_HOLIDAY1, HolLable[m_Weekly_data.at(i).override_1.number]);
                
		}
		else
		{
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_HOLIDAY1,_T(""));
		}

		if((m_Weekly_data.at(i).override_2.point_type == BAC_HOL + 1) && (m_Weekly_data.at(i).override_2.number < 8))
		{
				//CString temp_str;
				//temp_str.Format(_T("AR%d"),m_Weekly_data.at(i).override_2.number + 1);
				//m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_HOLIDAY2,temp_str);
            m_weeklyr_list.SetItemText(i, WEEKLY_ROUTINE_HOLIDAY2, HolLable[m_Weekly_data.at(i).override_2.number]);
		}
		else
		{
			m_weeklyr_list.SetItemText(i,WEEKLY_ROUTINE_HOLIDAY2,_T(""));
		}


		if(isFreshOne)
		{
			break;
		}
	}
	
	copy_data_to_ptrpanel(TYPE_WEEKLY);
	return 0;
}




void BacnetWeeklyRoutine::OnNMClickListBacWeekly(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_weeklyr_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_weeklyr_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_weeklyr_list.SetCellChecked(nItem,0,1);
		weekly_list_line = nItem;
		for (int i=0;i<m_weeklyr_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_weeklyr_list.SetCellChecked(i,0,FALSE);
		}
	}


	*pResult = 0;
}

void BacnetWeeklyRoutine::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	KillTimer(1);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}

void BacnetWeeklyRoutine::OnCancel()
{
	
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}



void BacnetWeeklyRoutine::OnTimer(UINT_PTR nIDEvent)
{
	 
	if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_WEEKLY))  )	//GSM连接时不要刷新;
	{
	PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	Post_Refresh_Message(g_bac_instance,READWEEKLYROUTINE_T3000,0,BAC_SCHEDULE_COUNT - 1,sizeof(Str_weekly_routine_point), BAC_SCHEDULE_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void BacnetWeeklyRoutine::OnNMDblclkListWeeklySchedule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnBnClickedButtonWeeklyScheduleEdit();
	*pResult = 0;
}

void BacnetWeeklyRoutine::OnBnClickedButtonWeeklysubPanelScheduleEdit()
{

    b_stop_read_tstat_schedule = false;  //是否继续读取标志，若后面数据为空则退出循环体.

                                         //在获取前清空缓存值.
    for (int i = 0;i < BAC_TSTAT_SCHEDULE;i++)
    {
        Str_tstat_schedule temp_tstat_schedule;
        memset(&m_tatat_schedule_data.at(i).all, 0, sizeof(Str_tstat_schedule));
    }

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
            n_temp_result.Format(_T("Read Tstat From %d to %d success."), (BAC_READ_TSTAT_SCHEDULE_GROUP_NUMBER)*i, end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
            Sleep(10);
            if (b_stop_read_tstat_schedule)
            {
                n_temp_result.Format(_T("Reading tstat schedule complete"));
                SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
                Sleep(10);
                break;
            }
        }
        else
        {
            n_temp_result.Format(_T("Reading tstat schedule From %d to %d failed."), (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i, end_temp_instance);
            SetPaneString(BAC_SHOW_MISSION_RESULTS, n_temp_result);
            return;
        }
    }

    b_stop_read_tstat_schedule = false;

    //显示非模态对话框;
    if (BacnetTstatSchedule_Window != NULL)
    {
        delete BacnetTstatSchedule_Window;
        BacnetTstatSchedule_Window = NULL;
    }
    BacnetTstatSchedule_Window = new CBacnetTstatSchedule;
    BacnetTstatSchedule_Window->Create(IDD_DIALOG_BACNET_TSTAT_SCHEDULE, this);
    BacnetTstatSchedule_Window->ShowWindow(SW_SHOW);
}

void BacnetWeeklyRoutine::OnBnClickedButtonWeeklyScheduleEdit()
{
	
	for (int i=0;i<m_weeklyr_list.GetItemCount();++i)
	{
		if(m_weeklyr_list.GetCellChecked(i,0))
		{
			weekly_list_line = i;
			break;
		}
	}

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLYCODE);

}

void BacnetWeeklyRoutine::Reset_Weekly_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);
	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 110,temp_mynew_rect.top + 80,500,700, NULL);


	return;

}

void BacnetWeeklyRoutine::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_weeklyr_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		m_weeklyr_list.MoveWindow(rc.left,rc.top,rc.Width(),rc.Height() - 80);
		GetDlgItem(IDC_BUTTON_WEEKLY_EDIT)->MoveWindow(rc.left + 20 ,rc.bottom - 60 , 120,50);
        GetDlgItem(IDC_BUTTON_SCHEDULE_SUBNET)->MoveWindow(rc.left + 200, rc.bottom - 60, 160, 50);  //随着窗体的移动  移动 按键 和表格的位置;
	}

}


void BacnetWeeklyRoutine::OnSysCommand(UINT nID, LPARAM lParam)
{
	 

	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 60 ,temp_mynew_rect.top + 60,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL BacnetWeeklyRoutine::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	//if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 
			HH_HELP_CONTEXT, IDH_TOPIC_6_7_SCHEDULES);
		return (hWnd != NULL);
// 	}
// 	else{
// 		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
// 	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}


int GetScheduleLabel(int index,CString &ret_label)
{
	if(index >= BAC_SCHEDULE_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	if(ret_label.IsEmpty())
	{
		ret_label.Format(_T("SCH%d"),index+1);
	}

	return 1;
}


int GetScheduleValue(int index, CString &Auto_M, CString &persend_data)
{
    if (index >= BAC_SCHEDULE_COUNT)
    {
        persend_data.Empty();
        return -1;
    }

    if (m_Annual_data.at(index).auto_manual == 1)
    {
        Auto_M = _T("M");
    }
    else
    {
        Auto_M.Empty();
    }

    if (m_Weekly_data.at(index).value == 0)
        persend_data = _T("OFF");
    else
        persend_data = _T("ON");

    return 1;
}


int GetScheduleFullLabel(int index,CString &ret_full_label)
{
	if(index >= BAC_SCHEDULE_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Weekly_data.at(i).description, (int)strlen((char *)m_Weekly_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();
	if(ret_full_label.IsEmpty())
	{
		ret_full_label.Format(_T("SCH%d"),index+1);
	}
	return 1;
}

