// BacnetWeeklyRoutine.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWeeklyRoutine.h"
#include "afxdialogex.h"



#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
#include "BacnetRange.h"
#include "BacnetScheduleTime.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
Str_weekly_routine_point m_temp_weekly_data[BAC_WEEKLY_ROUTINES_COUNT];
// BacnetWeeklyRoutine dialog

IMPLEMENT_DYNAMIC(BacnetWeeklyRoutine, CDialogEx)

BacnetWeeklyRoutine::BacnetWeeklyRoutine(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetWeeklyRoutine::IDD, pParent)
{
	weekly_list_line = 0;
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
	//ON_MESSAGE(MY_RESUME_DATA, WeeklyResumeMessageCallBack)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAC_WEEKLY, &BacnetWeeklyRoutine::OnNMClickListBacWeekly)
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_WM_HELPINFO()

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
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		memcpy_s(&m_Weekly_data.at(pInvoke->mRow),sizeof(Str_weekly_routine_point),&m_temp_weekly_data[pInvoke->mRow],sizeof(Str_weekly_routine_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	//m_weeklyr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR);

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
	// TODO: Add your specialized code here and/or call the base class
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
		//CBacnetScheduleTime DLG;
		//DLG.DoModal();
	}
	//::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAMCODE);
	return 0;
}

BOOL BacnetWeeklyRoutine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();
//	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);
	PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void BacnetWeeklyRoutine::Initial_List()
{
	m_weeklyr_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_weeklyr_list.SetExtendedStyle(m_weeklyr_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_FULL_LABLE, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_AUTO_MANUAL, _T("Auto/Manual"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_OUTPUT, _T("Output"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_HOLIDAY1, _T("Holiday1"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_STATE1, _T("State1"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_HOLIDAY2, _T("Holiday2"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_STATE2, _T("State2"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weeklyr_list.InsertColumn(WEEKLY_ROUTINE_LABEL, _T("Label"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_weekly_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_weekly_dlg_hwnd;


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
			for (int x=1;x<=8;x++)
			{
				temp1.Format(_T("AR%d"),x);
				strlist.push_back(temp1);
			}
			m_weeklyr_list.SetCellStringList(i, WEEKLY_ROUTINE_HOLIDAY1, strlist);
		}

		if(ListCtrlEx::ComboBox == m_weeklyr_list.GetColumnType(WEEKLY_ROUTINE_HOLIDAY2))
		{
			ListCtrlEx::CStrList strlist;
			CString temp1;
			for (int x=1;x<=8;x++)
			{
				temp1.Format(_T("AR%d"),x);
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
	m_weeklyr_list.SetCellChecked(0,0,1);

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
	cmp_ret = memcmp(&m_temp_weekly_data[Changed_Item],&m_Weekly_data.at(Changed_Item),sizeof(Str_weekly_routine_point));
	if(cmp_ret!=0)
	{
			m_weeklyr_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Weekly Routine List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(g_bac_instance,WRITEWEEKLYROUTINE_T3000,Changed_Item,Changed_Item,sizeof(Str_weekly_routine_point),m_weekly_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}

LRESULT BacnetWeeklyRoutine::Fresh_Weekly_List(WPARAM wParam,LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_weeklyr_list.IsDataNewer((char *)&m_Weekly_data.at(0),sizeof(Str_weekly_routine_point) * BAC_WEEKLY_ROUTINES_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_weeklyr_list.SetListData((char *)&m_Weekly_data.at(0),sizeof(Str_weekly_routine_point) * BAC_WEEKLY_ROUTINES_COUNT);
		}
		else
		{
			return 0;
		}
	}


	for (int i=0;i<(int)m_Weekly_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

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
		if(isFreshOne)
		{
			break;
		}
	}
	copy_data_to_ptrpanel(TYPE_WEEKLY);
	//MessageBox("1");
	return 0;
}




void BacnetWeeklyRoutine::OnNMClickListBacWeekly(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

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
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	KillTimer(1);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}

void BacnetWeeklyRoutine::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	//KillTimer(1);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}



void BacnetWeeklyRoutine::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if((this->IsWindowVisible()) && (Gsm_communication == false) )	//GSM连接时不要刷新;
	{
	PostMessage(WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	Post_Refresh_Message(g_bac_instance,READWEEKLYROUTINE_T3000,0,BAC_WEEKLY_ROUTINES_COUNT - 1,sizeof(Str_weekly_routine_point), BAC_WEEKLY_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void BacnetWeeklyRoutine::OnBnClickedButtonWeeklyScheduleEdit()
{
	// TODO: Add your control notification handler code here
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
BOOL BacnetWeeklyRoutine::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 
			HH_HELP_CONTEXT, IDH_TOPIC_WEEKLY_SCHEDULES);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}