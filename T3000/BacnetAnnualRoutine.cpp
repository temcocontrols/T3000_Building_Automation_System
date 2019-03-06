// BacnetAnnualRoutine.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAnnualRoutine.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetRange.h"

#include "AnnualRout_InsertDia.h"
// BacnetAnnualRoutine dialog
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.

IMPLEMENT_DYNAMIC(BacnetAnnualRoutine, CDialogEx)

BacnetAnnualRoutine::BacnetAnnualRoutine(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetAnnualRoutine::IDD, pParent)
{
	annual_list_line = 0;
	window_max = true;
}

BacnetAnnualRoutine::~BacnetAnnualRoutine()
{
}

void BacnetAnnualRoutine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BAC_ANNULE_LIST, m_annualr_list);
}


BEGIN_MESSAGE_MAP(BacnetAnnualRoutine, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, AnnualMessageCallBack)
	ON_MESSAGE(WM_HOTKEY,&BacnetAnnualRoutine::OnHotKey)//快捷键消息映射手动加入;
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAC_ANNULE_LIST, &BacnetAnnualRoutine::OnNMClickListBacAnnuleList)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Annual_Routine_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ANNUAL_LIST,Fresh_Annual_Routine_List)
	ON_BN_CLICKED(IDC_BUTTON_ANNUAL_EDIT, &BacnetAnnualRoutine::OnBnClickedButtonAnnualEdit)
//	ON_MESSAGE(MY_RESUME_DATA, AnnualResumeMessageCallBack)
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BAC_ANNULE_LIST, &BacnetAnnualRoutine::OnNMDblclkListAnnuleList)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// BacnetAnnualRoutine message handlers


LRESULT  BacnetAnnualRoutine::AnnualMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_Annual_data.at(pInvoke->mRow),sizeof(Str_annual_routine_point),&m_temp_annual_data[pInvoke->mRow],sizeof(Str_annual_routine_point));//还原没有改对的值;
		PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_annualr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_annualr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_annualr_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL BacnetAnnualRoutine::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_annualr_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_annual_dlg_hwnd,win_rect);
		m_annualr_list.Set_My_WindowRect(win_rect);
		m_annualr_list.Set_My_ListRect(list_rect);
		m_annualr_list.Get_clicked_mouse_position();
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
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 120 ,temp_mynew_rect.top + 70,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//const int KEY_INSERT = 1020;
LRESULT BacnetAnnualRoutine::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		OnBnClickedButtonAnnualEdit();
		//AnnualRout_InsertDia Dlg;
		//Dlg.DoModal();
	}
	return 0;
}



BOOL BacnetAnnualRoutine::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("HOLIDAY"));
	
	Initial_List();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_HOLIDAY);
	SetIcon(m_hIcon,TRUE);
	ShowWindow(FALSE);
//	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);
	PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BacnetAnnualRoutine::Initial_List()
{
	m_annualr_list.ShowWindow(SW_HIDE);
	m_annualr_list.DeleteAllItems();
	while ( m_annualr_list.DeleteColumn (0)) ;

	m_annualr_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_annualr_list.SetExtendedStyle(m_annualr_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_FULL_LABEL, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_AUTO_MANUAL, _T("Auto/Manual"), 90, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_VALUE, _T("Value"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_LABLE, _T("Label"), 90, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

	m_annualr_list.Setlistcolcharlimit(ANNUAL_ROUTINE_FULL_LABEL,STR_ANNUAL_DESCRIPTION_LENGTH -1);
	m_annualr_list.Setlistcolcharlimit(ANNUAL_ROUTINE_LABLE,STR_ANNUAL_LABEL_LENGTH-1);

	m_annual_dlg_hwnd = this->m_hWnd;

	m_annualr_list.SetListHwnd(this->m_hWnd);
	//g_hwnd_now = m_annual_dlg_hwnd;

	CRect list_rect,win_rect;
	m_annualr_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_annual_dlg_hwnd,win_rect);
	m_annualr_list.Set_My_WindowRect(win_rect);
	m_annualr_list.Set_My_ListRect(list_rect);

	m_annualr_list.DeleteAllItems();
	for (int i=0;i<(int)m_Annual_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_annualr_list.InsertItem(i,temp_item);
		m_annualr_list.SetCellEnabled(i,ANNUAL_ROUTINE_NUM,0);
		//if(ListCtrlEx::ComboBox == m_annualr_list.GetColumnType(ANNUAL_ROUTINE_AUTO_MANUAL))
		//{
		//	ListCtrlEx::CStrList strlist;
		//	strlist.push_back(_T("Auto"));
		//	strlist.push_back(_T("Manual"));
		//	m_annualr_list.SetCellStringList(i, ANNUAL_ROUTINE_AUTO_MANUAL, strlist);
		//}


		if(ListCtrlEx::ComboBox == m_annualr_list.GetColumnType(ANNUAL_ROUTINE_VALUE))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("OFF"));
			strlist.push_back(_T("ON"));
			m_annualr_list.SetCellStringList(i, ANNUAL_ROUTINE_VALUE, strlist);
		}

		for (int x=0;x<ANNUAL_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_annualr_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_annualr_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_annualr_list.InitListData();
	m_annualr_list.SetCellChecked(0,0,1);// default will choose the first one
	m_annualr_list.ShowWindow(SW_SHOW);
}


void BacnetAnnualRoutine::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	KillTimer(1);
	m_annual_dlg_hwnd = NULL;
	CDialogEx::OnClose();
}

void BacnetAnnualRoutine::OnCancel()
{
	
	//KillTimer(1);
	//m_annual_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}


void BacnetAnnualRoutine::OnNMClickListBacAnnuleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_annualr_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_annualr_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_annualr_list.SetCellChecked(nItem,0,1);
		annual_list_line = nItem;
		for (int i=0;i<m_annualr_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_annualr_list.SetCellChecked(i,0,FALSE);
		}
	}
	*pResult = 0;
	long lRow,lCol;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	CString New_CString;
	CString temp_task_info;

	if(lCol == ANNUAL_ROUTINE_AUTO_MANUAL)
	{
		//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
		memcpy_s(&m_temp_annual_data[lRow],sizeof(Str_annual_routine_point),&m_Annual_data.at(lRow),sizeof(Str_annual_routine_point));

		if(m_Annual_data.at(lRow).auto_manual == 0)
		{
			m_Annual_data.at(lRow).auto_manual = 1;
			m_annualr_list.SetItemText(lRow,ANNUAL_ROUTINE_AUTO_MANUAL,_T("Manual"));
			m_annualr_list.SetCellEnabled(lRow,ANNUAL_ROUTINE_VALUE,TRUE);
			New_CString = _T("Manual");
		}
		else
		{
			m_Annual_data.at(lRow).auto_manual = 0;
			m_annualr_list.SetItemText(lRow,ANNUAL_ROUTINE_AUTO_MANUAL,_T("Auto"));
			m_annualr_list.SetCellEnabled(lRow,ANNUAL_ROUTINE_VALUE,FALSE);
			New_CString = _T("Auto");
		}
		temp_task_info.Format(_T("Write Annual List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	}
	else
		return;

	int cmp_ret = memcmp(&m_temp_annual_data[lRow],&m_Annual_data.at(lRow),sizeof(Str_annual_routine_point));
	if(cmp_ret!=0)
	{
		m_annualr_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Annual Routine List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEHOLIDAY_T3000,lRow,lRow,sizeof(Str_annual_routine_point),m_annual_dlg_hwnd ,temp_task_info,lRow,lCol);
	}

	
}
LRESULT BacnetAnnualRoutine::Fresh_Annual_Routine_List(WPARAM wParam,LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_annualr_list.IsDataNewer((char *)&m_Annual_data.at(0),sizeof(Str_annual_routine_point) * BAC_HOLIDAY_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_annualr_list.SetListData((char *)&m_Annual_data.at(0),sizeof(Str_annual_routine_point) * BAC_HOLIDAY_COUNT);
		}
		else
		{
			return 0;
		}
	}


	for (int i=0;i<(int)m_Annual_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).description, (int)strlen((char *)m_Annual_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_FULL_LABEL,temp_des);
		if(m_Annual_data.at(i).auto_manual==0)
		{
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_AUTO_MANUAL,_T("Auto"));
			m_annualr_list.SetCellEnabled(i,ANNUAL_ROUTINE_VALUE,0);
		}
		else
		{
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_AUTO_MANUAL,_T("Manual"));
			m_annualr_list.SetCellEnabled(i,ANNUAL_ROUTINE_VALUE,1);
		}


		if(m_Annual_data.at(i).value==0)
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_VALUE,_T("OFF"));
		else
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_VALUE,_T("ON"));

	

		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).label, (int)strlen((char *)m_Annual_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_LABLE,temp_des2);

        if (temp_des2.IsEmpty())
        {
            HolLable[i].Format(_T("AR%d"), i+1);
        }
        else
        {
            HolLable[i] = temp_des2;
        }
        

		if(isFreshOne)
		{
			break;
		}
	}
	copy_data_to_ptrpanel(TYPE_WEEKLY);
	//MessageBox("1");
	return 0;
}


LRESULT BacnetAnnualRoutine::Fresh_Annual_Routine_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);

	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_annual_data[Changed_Item],sizeof(Str_annual_routine_point),&m_Annual_data.at(Changed_Item),sizeof(Str_annual_routine_point));

	if(Changed_SubItem == ANNUAL_ROUTINE_LABLE)
	{
		CString cs_temp = m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_ANNUAL_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
			return 0;
		}
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Annual_data.at(Changed_Item).label,STR_ANNUAL_LABEL_LENGTH,cTemp1,STR_ANNUAL_LABEL_LENGTH);
	}

	if(Changed_SubItem == ANNUAL_ROUTINE_FULL_LABEL)
	{
		CString cs_temp = m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_ANNUAL_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
			return 0;
		}
		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Annual_data.at(Changed_Item).description,STR_ANNUAL_DESCRIPTION_LENGTH,cTemp1,STR_ANNUAL_DESCRIPTION_LENGTH);
	}

	if(Changed_SubItem == ANNUAL_ROUTINE_VALUE)
	{
		CString cs_temp=m_annualr_list.GetItemText(Changed_Item,ANNUAL_ROUTINE_VALUE);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Annual_data.at(Changed_Item).value=0;
		}
		else
		{
			m_Annual_data.at(Changed_Item).value=1;
		}
	}
	if(Changed_SubItem == ANNUAL_ROUTINE_AUTO_MANUAL)
	{
		CString cs_temp=m_annualr_list.GetItemText(Changed_Item,ANNUAL_ROUTINE_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Annual_data.at(Changed_Item).auto_manual=0;
			m_annualr_list.SetCellEnabled(Changed_Item,ANNUAL_ROUTINE_VALUE,0);
		}
		else
		{
			m_Annual_data.at(Changed_Item).auto_manual=1;
			m_annualr_list.SetCellEnabled(Changed_Item,ANNUAL_ROUTINE_VALUE,1);
		}
	}
	cmp_ret = memcmp(&m_temp_annual_data[Changed_Item],&m_Annual_data.at(Changed_Item),sizeof(Str_annual_routine_point));
	if(cmp_ret!=0)
	{
		m_annualr_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Annual Routine List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(g_bac_instance,WRITEHOLIDAY_T3000,Changed_Item,Changed_Item,sizeof(Str_annual_routine_point),m_annual_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}

void BacnetAnnualRoutine::OnTimer(UINT_PTR nIDEvent)
{
	 
	if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_ANNUAL))  )	//GSM连接时不要刷新;
	{
	PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	if(bac_select_device_online)
		Post_Refresh_Message(g_bac_instance,READANNUALROUTINE_T3000,0,BAC_HOLIDAY_COUNT - 1,sizeof(Str_annual_routine_point),BAC_HOLIDAY_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void BacnetAnnualRoutine::OnBnClickedButtonAnnualEdit()
{
	
	for (int i=0;i<m_annualr_list.GetItemCount();++i)
	{
		if(m_annualr_list.GetCellChecked(i,0))
		{
			annual_list_line = i;
			break;
		}
	}

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUALCODE);

}

void BacnetAnnualRoutine::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void BacnetAnnualRoutine::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}


void BacnetAnnualRoutine::OnNMDblclkListAnnuleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnBnClickedButtonAnnualEdit();
	*pResult = 0;
}


BOOL BacnetAnnualRoutine::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 			
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 			
			HH_HELP_CONTEXT, IDH_TOPIC_6_8_HOLIDAYS);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}

void BacnetAnnualRoutine::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_annualr_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		//m_program_list.MoveWindow(&rc);
		m_annualr_list.MoveWindow(rc.left,rc.top,rc.Width(),rc.Height() - 80);

		GetDlgItem(IDC_BUTTON_ANNUAL_EDIT)->MoveWindow(rc.left + 20 ,rc.bottom - 60 , 120,50);
	}
}


void BacnetAnnualRoutine::OnSysCommand(UINT nID, LPARAM lParam)
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
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 120 ,temp_mynew_rect.top + 70,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


void BacnetAnnualRoutine::Reset_Annual_Rect()
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
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 90,temp_mynew_rect.top + 70,500,700, NULL);


	return;

}


int GetHolidayLabel(int index,CString &ret_label)
{
	if(index >= BAC_HOLIDAY_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).label, (int)strlen((char *)m_Annual_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	if(ret_label.IsEmpty())
	{
		ret_label.Format(_T("HOL%d"),index+1);
	}

	return 1;
}


int GetHolidayFullLabel(int index,CString &ret_full_label)
{
	if(index >= BAC_HOLIDAY_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).description, (int)strlen((char *)m_Annual_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();
	if(ret_full_label.IsEmpty())
	{
		ret_full_label.Format(_T("HOL%d"),index+1);
	}
	return 1;
}

int GetHolidayValue(int index, CString &Auto_M, CString &persend_data)
{
    CStringArray temparray;
    CString temp1;
    if (index >= BAC_HOLIDAY_COUNT)
    {
        persend_data.Empty();
        return -1;
    }
    int i = index;

    if (m_Annual_data.at(i).auto_manual == 1)
    {
        Auto_M = _T("M");
    }
    else
    {
        Auto_M.Empty();
    }

    if (m_Annual_data.at(i).value == 0)
        persend_data = _T("OFF");
    else
        persend_data = _T("ON");


    return 1;
}
