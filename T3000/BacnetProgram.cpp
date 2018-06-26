// BacnetProgram.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgram.h"
#include "afxdialogex.h"

#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "BacnetProgramEdit.h"
#include "NewT3000ProgramEditorDlg.h"
#include "global_define.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.


#include "Dialog_Progess.h"
#include "DialogCM5_BacNet.h"
extern CDialog_Progess *WaitRead_Data_Dlg;
#include "MainFrm.h"

// CBacnetProgram dialog

IMPLEMENT_DYNAMIC(CBacnetProgram, CDialogEx)

CBacnetProgram::CBacnetProgram(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgram::IDD, pParent)
{
	program_list_line = 0;
	window_max = true;
}

CBacnetProgram::~CBacnetProgram()
{
}

void CBacnetProgram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROGRAM, m_program_list);
}


BEGIN_MESSAGE_MAP(CBacnetProgram, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, ProgramMessageCallBack)
	ON_MESSAGE(WM_HOTKEY,&CBacnetProgram::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Program_Item)
	ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_LIST,Fresh_Program_List)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_EDIT, OnBnClickedButtonProgramEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROGRAM, &CBacnetProgram::OnNMClickListProgram)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROGRAM, &CBacnetProgram::OnNMDblclkListProgram)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &CBacnetProgram::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBacnetProgram message handlers

LRESULT  CBacnetProgram::ProgramMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_Program_data.at(pInvoke->mRow),sizeof(Str_program_point),&m_temp_program_data[pInvoke->mRow],sizeof(Str_program_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_program_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_program_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_program_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}


// CBacnetProgramEdit message handlers
LRESULT CBacnetProgram::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		for (int i=0;i<m_program_list.GetItemCount();++i)
		{
			if(m_program_list.GetCellChecked(i,0))
			{
				program_list_line = i;
				break;
			}
		}
		OnBnClickedButtonProgramEdit();
	}
	if (wParam == KEY_INSERT_CONTROL)
	{
		for (int i = 0;i < m_program_list.GetItemCount();++i)
		{
			if (m_program_list.GetCellChecked(i, 0))
			{
				program_list_line = i;
				break;
			}
		}
		CNewT3000ProgramEditorDlg dlg;
		dlg.DoModal();
	}
	
	return 0;
}


BOOL CBacnetProgram::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("PROGRAM"));
	Initial_List();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_PROGRAM);
	SetIcon(m_hIcon,TRUE);
	PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	

	ShowWindow(FALSE);
	//RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetProgram::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
	RegisterHotKey(GetSafeHwnd(), KEY_INSERT_CONTROL, MOD_CONTROL , VK_INSERT);//Insert键
}

void CBacnetProgram::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	UnregisterHotKey(GetSafeHwnd(), KEY_INSERT_CONTROL);
}

void CBacnetProgram::Initial_List()
{
	m_program_list.ShowWindow(SW_HIDE);
	m_program_list.DeleteAllItems();
	while ( m_program_list.DeleteColumn (0)) ;

	m_program_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_program_list.SetExtendedStyle(m_program_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_program_list.SetExtendedStyle(m_program_list.GetExtendedStyle() |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_program_list.InsertColumn(0, _T("Program"), 80, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_program_list.InsertColumn(1, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(2, _T("Status"), 100, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(3, _T("Auto/Manual"), 100, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(4, _T("Size"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(5, _T("Run Status"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(6, _T("Label"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_program_list.Setlistcolcharlimit(1,STR_PROGRAM_DESCRIPTION_LENGTH -1);
	m_program_list.Setlistcolcharlimit(6,STR_PROGRAM_LABEL_LENGTH-1);
	m_pragram_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_pragram_dlg_hwnd;
	m_program_list.SetListHwnd(this->m_hWnd);
	CRect list_rect,win_rect;
	m_program_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_pragram_dlg_hwnd,win_rect);
	m_program_list.Set_My_WindowRect(win_rect);
	m_program_list.Set_My_ListRect(list_rect);

	m_program_list.DeleteAllItems();
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(i>=program_item_limit_count)
			break;


		temp_item.Format(_T("%d"),i+1);
		m_program_list.InsertItem(i,temp_item);
		m_program_list.SetCellEnabled(i,0,0);
		m_program_list.SetCellChecked(0,0,1);// default will set the first one checked
		if(ListCtrlEx::ComboBox == m_program_list.GetColumnType(PROGRAM_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(Auto_Manual[0]);
			strlist.push_back(Auto_Manual[1]);
			m_program_list.SetCellStringList(i, PROGRAM_AUTO_MANUAL, strlist);
		}

		if(ListCtrlEx::ComboBox == m_program_list.GetColumnType(PROGRAM_STATUS))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(ProgramStatus[0]);
			strlist.push_back(ProgramStatus[1]);
			m_program_list.SetCellStringList(i, PROGRAM_STATUS, strlist);
		}

		for (int x=0;x<PROGRAM_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_program_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_program_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_program_list.InitListData();
	m_program_list.SetCellChecked(0,0,1);
	m_program_list.ShowWindow(SW_SHOW);
}



LRESULT CBacnetProgram::Fresh_Program_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	memcpy_s(&m_temp_program_data[Changed_Item],sizeof(Str_program_point),&m_Program_data.at(Changed_Item),sizeof(Str_program_point));


	CString temp_task_info;
	CString New_CString =  m_program_list.GetItemText(Changed_Item,Changed_SubItem);


	if(Changed_SubItem == PROGRAM_LABEL)
	{
		CString cs_temp = m_program_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_PROGRAM_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(Changed_Item).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);
	}

	if(Changed_SubItem == PROGRAM_FULL_LABLE)
	{
		CString cs_temp = m_program_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_PROGRAM_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
			return 0;
		}
		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(Changed_Item).description,STR_PROGRAM_DESCRIPTION_LENGTH,cTemp1,STR_PROGRAM_DESCRIPTION_LENGTH);
	}

	if(Changed_SubItem == PROGRAM_STATUS)
	{
		CString cs_temp=m_program_list.GetItemText(Changed_Item,PROGRAM_STATUS);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Program_data.at(Changed_Item).on_off=0;
		}
		else
		{
			m_Program_data.at(Changed_Item).on_off=1;
		}
	}
	if(Changed_SubItem == PROGRAM_AUTO_MANUAL)
	{
		CString cs_temp=m_program_list.GetItemText(Changed_Item,PROGRAM_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Program_data.at(Changed_Item).auto_manual=0;
		}
		else
		{
			m_Program_data.at(Changed_Item).auto_manual=1;
		}
	}
	cmp_ret = memcmp(&m_temp_program_data[Changed_Item],&m_Program_data.at(Changed_Item),sizeof(Str_program_point));
	if(cmp_ret!=0)
	{
		m_program_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Program List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEPROGRAM_T3000,Changed_Item,Changed_Item,sizeof(Str_program_point),m_pragram_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}

	return 0;
}

LRESULT CBacnetProgram::Fresh_Program_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_program_list.IsDataNewer((char *)&m_Program_data.at(0),sizeof(Str_program_point) * BAC_PROGRAM_ITEM_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_program_list.SetListData((char *)&m_Program_data.at(0),sizeof(Str_program_point) * BAC_PROGRAM_ITEM_COUNT);
		}
		else
		{
			return 0;
		}
	}



	bac_program_pool_size = 2000;
	bac_program_size = 0;
	bac_free_memory = 2000;
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		
		if(i>=program_item_limit_count)
			break;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();

		m_program_list.SetItemText(i,PROGRAM_FULL_LABLE,temp_des);


		if(m_Program_data.at(i).auto_manual==0)
			m_program_list.SetItemText(i,PROGRAM_AUTO_MANUAL,_T("Auto"));
		else
			m_program_list.SetItemText(i,PROGRAM_AUTO_MANUAL,_T("Manual"));

		if(m_Program_data.at(i).on_off==0)
		{
			m_program_list.SetItemTextColor(i,PROGRAM_STATUS,RGB(255,0,0),false);
			m_program_list.SetItemText(i,PROGRAM_STATUS,ProgramStatus[1]);
		}
		else
		{
			m_program_list.SetItemTextColor(i,PROGRAM_STATUS,RGB(0,0,255),false);
			m_program_list.SetItemText(i,PROGRAM_STATUS,ProgramStatus[0]);
		}

		temp_value.Format(_T("%d"),m_Program_data.at(i).bytes);
		m_program_list.SetItemText(i,PROGRAM_SIZE_LIST,temp_value);



		if(m_Program_data.at(i).com_prg==0)
			m_program_list.SetItemText(i,PROGRAM_RUN_STATUS,_T("Normal"));
		else
			m_program_list.SetItemText(i,PROGRAM_RUN_STATUS,_T("Abnormal"));


		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();

		m_program_list.SetItemText(i,PROGRAM_LABEL,temp_des2);

		if(isFreshOne)
		{
			break;
		}

	}

	copy_data_to_ptrpanel(TYPE_PROGRAM);
	return 0;
}

void CBacnetProgram::OnBnClickedButton1()
{
	CString temp_show_info;
	temp_show_info.Format(_T("Reading program code %d ..."), program_list_line + 1);
	SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_show_info);

	CNewT3000ProgramEditorDlg dlg;
	dlg.DoModal();
}

void CBacnetProgram::OnBnClickedButtonProgramEdit()
{
	//2018 05 23 解决在切换panel时 ， 选中的 program_list_line 的值需要重新获取.
    for (int i = 0;i<m_program_list.GetItemCount();++i)
    {
        if (m_program_list.GetCellChecked(i, 0))
        {
            program_list_line = i;
            break;
        }
    }

	CString temp_show_info;
	temp_show_info.Format(_T("Reading program code %d ..."),program_list_line + 1);
	SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_show_info); 
	if(g_protocol == PROTOCOL_BIP_TO_MSTP)
	{
		//CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		//pFrame->Create_Thread_Read_Item(TYPE_PROGRAMCODE);
		if(WaitRead_Data_Dlg)
		{
			delete WaitRead_Data_Dlg;
			WaitRead_Data_Dlg = 0;
		}
		WaitRead_Data_Dlg = new CDialog_Progess(this,1,100);
		//创建对话框窗口
		WaitRead_Data_Dlg->Create(IDD_DIALOG10_Progress, this);
		WaitRead_Data_Dlg->ShowProgress(0,0);
		RECT RECT_SET1;
		::GetWindowRect(BacNet_hwd,&RECT_SET1);
		WaitRead_Data_Dlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 19,800,20,1);
		WaitRead_Data_Dlg->ShowWindow(SW_SHOW);
		g_bac_read_type = TYPE_PROGRAMCODE;


		int m_finished_count = 0;
		int m_persent = 0;
		int m_total_count = 1;
		bac_read_which_list = BAC_READ_PROGRAMCODE_LIST;
		for (int i=0;i<4;i++)
		{
			int ret_variable;
			ret_variable = GetProgramData_Blocking(g_bac_instance,program_list_line,program_list_line,i);
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read program item %d timeout!\r\n"),i+1);
				::PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				::PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				return ;
			}
			else
			{
				temp_cstring->Format(_T("Read program code part%d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				::PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,0);
				::PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
			}
		}
		if(g_bac_read_type == TYPE_PROGRAMCODE)
		{
			bac_programcode_read_results = true;
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,SHOW_PROGRAM_IDE,0);
		}

	}
	else
	{
		::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAMCODE);
	}


}



void CBacnetProgram::OnNMClickListProgram(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_program_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_program_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_program_list.SetCellChecked(nItem,0,1);
		program_list_line = nItem;
		for (int i=0;i<m_program_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_program_list.SetCellChecked(i,0,FALSE);
		}
	}


	


	*pResult = 0;
}
void CBacnetProgram::OnClose()
{
	 
	ShowWindow(FALSE);
	return;

	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}
void CBacnetProgram::OnCancel()
{
	
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}

void CBacnetProgram::OnTimer(UINT_PTR nIDEvent)
{
	 
	if(g_protocol == PROTOCOL_BIP_TO_MSTP)
	{
		PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	}
	else if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_PROGRAM))  )	//GSM连接时不要刷新;
	{
	PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	if(bac_select_device_online)
		Post_Refresh_Message(g_bac_instance,READPROGRAM_T3000,0,BAC_PROGRAM_ITEM_COUNT - 1,sizeof(Str_program_point),BAC_PROGRAM_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CBacnetProgram::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_program_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_pragram_dlg_hwnd,win_rect);
		m_program_list.Set_My_WindowRect(win_rect);
		m_program_list.Set_My_ListRect(list_rect);

		m_program_list.Get_clicked_mouse_position();
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
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 70,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}





BOOL CBacnetProgram::OnHelpInfo(HELPINFO* pHelpInfo)
{  
	//if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 
			HH_HELP_CONTEXT, IDH_TOPIC_6_2_PROGRAMS);
		return (hWnd != NULL);
// 	}
// 	else{
// 		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
// 	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}

int GetPrgLabel(int index,CString &ret_label)
{
	if(index >= BAC_PROGRAM_ITEM_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	return 1;
}

int GetPrgValue(int index, CString &ret_cstring)
{
	if (index >= BAC_PROGRAM_ITEM_COUNT)
	{
		ret_cstring.Empty();
		return -1;
	}
	int i = index;

	if (m_Program_data.at(i).on_off == 0)
	{
		ret_cstring = ProgramStatus[1];
	}
	else
	{
		ret_cstring = ProgramStatus[0];
	}

	return 1;
}

int GetPrgFullLabel(int index,CString &ret_full_label)
{
	if(index >= BAC_PROGRAM_ITEM_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();
	return 1;
}




void CBacnetProgram::OnNMDblclkListProgram(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnBnClickedButtonProgramEdit();
	*pResult = 0;
}

void CBacnetProgram::Reset_Program_Rect()
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



void CBacnetProgram::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_program_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		//m_program_list.MoveWindow(&rc);
		m_program_list.MoveWindow(rc.left,rc.top,rc.Width(),rc.Height() - 80);

		GetDlgItem(IDC_BUTTON_PROGRAM_EDIT)->MoveWindow(rc.left + 20 ,rc.bottom - 60 , 120,50);

		GetDlgItem(IDC_BUTTON1)->MoveWindow(rc.left + 160, rc.bottom - 60, 200, 50);
	}
}


void CBacnetProgram::OnSysCommand(UINT nID, LPARAM lParam)
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
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 70,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}
