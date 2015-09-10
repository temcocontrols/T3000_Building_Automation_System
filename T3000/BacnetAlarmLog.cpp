// BacnetAlarmLog.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAlarmLog.h"
#include "afxdialogex.h"
#include "globle_function.h"



// CBacnetAlarmLog dialog

IMPLEMENT_DYNAMIC(CBacnetAlarmLog, CDialogEx)

CBacnetAlarmLog::CBacnetAlarmLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAlarmLog::IDD, pParent)
{

}

CBacnetAlarmLog::~CBacnetAlarmLog()
{
}

void CBacnetAlarmLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALARMLOG, m_alarmlog_list);
}


BEGIN_MESSAGE_MAP(CBacnetAlarmLog, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, AlarmLogMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_ALARMLOG_LIST,Fresh_Alarmlog_List)
	ON_WM_TIMER()
//	ON_NOTIFY(HDN_ITEMCLICK, 0, &CBacnetAlarmLog::OnHdnItemclickListAlarmlog)
ON_NOTIFY(NM_CLICK, IDC_LIST_ALARMLOG, &CBacnetAlarmLog::OnClickListAlarmlog)
ON_WM_CLOSE()

ON_WM_HELPINFO()

END_MESSAGE_MAP()


// CBacnetAlarmLog message handlers
#include "BacnetAlarmWindow.h"
extern CBacnetAlarmWindow * AlarmWindow_Window;
BOOL CBacnetAlarmLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();


	if(AlarmWindow_Window == NULL)
	{
		AlarmWindow_Window = new CBacnetAlarmWindow;
		AlarmWindow_Window->Create(IDD_DIALOG_BACNET_ALARM_WINDOW,this);	
	}

	AlarmWindow_Window->ShowWindow(SW_SHOW);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT  CBacnetAlarmLog::AlarmLogMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_alarmlog_data.at(pInvoke->mRow),sizeof(Alarm_point),&m_temp_alarmlog_data[pInvoke->mRow],sizeof(Alarm_point));//还原没有改对的值;
		PostMessage(WM_REFRESH_BAC_ALARMLOG_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_alarmlog_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_alarmlog_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_alarmlog_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}

BOOL CBacnetAlarmLog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_alarmlog_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_alarmlog_dlg_hwnd,win_rect);
		m_alarmlog_list.Set_My_WindowRect(win_rect);
		m_alarmlog_list.Set_My_ListRect(list_rect);

		m_alarmlog_list.Get_clicked_mouse_position();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBacnetAlarmLog::Initial_List()
{
	m_alarmlog_list.ShowWindow(SW_HIDE);
	m_alarmlog_list.DeleteAllItems();
	while ( m_alarmlog_list.DeleteColumn (0)) ;

	m_alarmlog_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_alarmlog_list.SetExtendedStyle(m_alarmlog_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_alarmlog_list.InsertColumn(ALARMLOG_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_alarmlog_list.InsertColumn(ALARMLOG_PANEL, _T("Panel"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_alarmlog_list.InsertColumn(ALARMLOG_MESSAGE, _T("Message"), 300, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_alarmlog_list.InsertColumn(ALARMLOG_TIME, _T("Time"), 180, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_alarmlog_list.InsertColumn(ALARMLOG_ACK, _T("Acknowlege"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_alarmlog_list.InsertColumn(ALARMLOG_RES, _T("Res"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_alarmlog_list.InsertColumn(ALARMLOG_DEL, _T("Delete"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);

	m_alarmlog_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_alarmlog_dlg_hwnd;
	m_alarmlog_list.DeleteAllItems();

	for (int i=0;i<(int)m_alarmlog_data.size();i++)
	{
		CString temp_item;
		temp_item.Format(_T("%d"),i+1);
		m_alarmlog_list.InsertItem(i,temp_item);


		for (int x=0;x<ALARMLOG_COL_NUMBER;x++)//设置背景色，白灰间隔，看起来不那么累;
		{

			//if(ListCtrlEx::ComboBox == m_alarmlog_list.GetColumnType(ALARMLOG_DEL))
			//{
			//	ListCtrlEx::CStrList strlist;
			//	for (int y=0;y<(int)sizeof(Yes_No)/sizeof(Yes_No[0]);y++)
			//	{
			//		strlist.push_back(Yes_No[y]);
			//	}
			//	m_alarmlog_list.SetCellStringList(i, ALARMLOG_DEL, strlist);		
			//}


			if((i%2)==0)
				m_alarmlog_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_alarmlog_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_alarmlog_list.ShowWindow(SW_SHOW);
}

LRESULT CBacnetAlarmLog::Fresh_Alarmlog_List(WPARAM wParam,LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_alarmlog_list.IsDataNewer((char *)&m_alarmlog_data.at(0),sizeof(Alarm_point) * BAC_ALARMLOG_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_alarmlog_list.SetListData((char *)&m_alarmlog_data.at(0),sizeof(Alarm_point) * BAC_ALARMLOG_COUNT);
		}
		//else
		//{
		//	return 0;
		//}
	}


	for (int i=0;i<(int)m_alarmlog_data.size();i++)
	{
		if(isFreshOne)
		{
			i = Fresh_Item;
		}
		CString temp_item;
		CString temp_message;
		CString temp_panel;

		temp_item.Format(_T("%d"),i+1);
		temp_panel.Format(_T("%d"),(unsigned char)m_alarmlog_data.at(i).point.panel);
		MultiByteToWideChar( CP_ACP, 0, (char *)m_alarmlog_data.at(i).alarm_message, (int)strlen((char *)m_alarmlog_data.at(i).alarm_message)+1, 
			temp_message.GetBuffer(MAX_PATH), MAX_PATH );
		temp_message.ReleaseBuffer();


		m_alarmlog_list.SetItemText(i,ALARMLOG_NUM,temp_item);
		m_alarmlog_list.SetItemText(i,ALARMLOG_MESSAGE,temp_panel);
		m_alarmlog_list.SetItemText(i,ALARMLOG_MESSAGE,temp_message);

		if(m_alarmlog_data.at(i).alarm == 1)
		{
			if(m_alarmlog_data.at(i).ddelete == 0)
			{
				m_alarmlog_list.SetItemText(i,ALARMLOG_DEL,Yes_No[0]);
			}
			else
			{
				m_alarmlog_list.SetItemText(i,ALARMLOG_DEL,Yes_No[1]);
			}
		}
		else
		{
			m_alarmlog_list.SetItemText(i,ALARMLOG_MESSAGE,_T(""));
			m_alarmlog_list.SetItemText(i,ALARMLOG_DEL,_T(""));
		}

		if(isFreshOne)
		{
			break;
		}
	}
	

	return 0;
}


void CBacnetAlarmLog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if((this->IsWindowVisible()) && (Gsm_communication == false) )	//GSM连接时不要刷新;
	{
		PostMessage(WM_REFRESH_BAC_ALARMLOG_LIST,NULL,NULL);
		if(bac_select_device_online)
			Post_Refresh_Message(g_bac_instance,READALARM_T3000,0,BAC_ALARMLOG_COUNT - 1,sizeof(Alarm_point),BAC_ALARMLOG_GROUP);
	}

	CDialogEx::OnTimer(nIDEvent);
}


//void CBacnetAlarmLog::OnHdnItemclickListAlarmlog(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


void CBacnetAlarmLog::OnClickListAlarmlog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	long lRow,lCol;
	m_alarmlog_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_alarmlog_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_alarmlog_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_alarmlog_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	if(lCol != ALARMLOG_DEL)	
		return;

	if(m_alarmlog_data.at(lRow).ddelete == 1)	//如果已经是delete 了就不在变了;
		return;
	CString temp_task_info;
	CString New_CString;
	memcpy_s(&m_temp_alarmlog_data[lRow],sizeof(Alarm_point),&m_alarmlog_data.at(lRow),sizeof(Alarm_point));

	m_alarmlog_data.at(lRow).ddelete = 1;
	m_alarmlog_list.SetItemText(lRow,ALARMLOG_DEL,Yes_No[1]);
	New_CString = Yes_No[1];
	m_alarmlog_list.Set_Edit(false);

	int cmp_ret = memcmp(&m_temp_alarmlog_data[lRow],&m_alarmlog_data.at(lRow),sizeof(Alarm_point));
	if(cmp_ret!=0)
	{
		m_alarmlog_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Alarmlog List Item %d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEALARM_T3000,lRow,lRow,sizeof(Alarm_point),m_alarmlog_dlg_hwnd,temp_task_info,lRow,lCol);
	}

	*pResult = 0;
}


void CBacnetAlarmLog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	m_alarmlog_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}


void CBacnetAlarmLog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}

BOOL CBacnetAlarmLog::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 			
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 			
			HH_HELP_CONTEXT, IDH_TOPIC_7_DATA);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}